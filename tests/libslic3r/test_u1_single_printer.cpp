// Snapmaker Orca Max: one size-less "Snapmaker U1" printer preset instead of one per nozzle size
// (scripts/convert_u1_single_printer.py). The vendor data, the per-size rules on the joined preset, and
// the file boundary with Snapmaker Orca: its projects name "Snapmaker U1 (<size> nozzle)".
#include <catch2/catch_all.hpp>

#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include <boost/algorithm/string/predicate.hpp>

#include "libslic3r/AppConfig.hpp"
#include "libslic3r/NozzleFilamentPresets.hpp"
#include "libslic3r/PerHeadProcess.hpp"
#include "libslic3r/Preset.hpp"
#include "libslic3r/PresetBundle.hpp"
#include "libslic3r/PrintConfig.hpp"
#include "libslic3r/Semver.hpp"

using namespace Slic3r;

namespace {

const char *const U1         = "Snapmaker U1";
const char *const U1_04_OLD  = "Snapmaker U1 (0.4 nozzle)";
const char *const PROCESS_04 = "0.20mm Standard @Snapmaker U1 (0.4 nozzle)";
const char *const PROCESS_06 = "0.30mm Standard @Snapmaker U1 (0.6 nozzle)";
const std::vector<std::string> SIZES{ "0.2", "0.4", "0.6", "0.8" };

// The loader of one vendor leaves the rename maps to the application (PresetBundle::update_system_maps).
struct RenameMapAccess : PresetCollection
{
    using PresetCollection::update_map_system_profile_renamed;
};

std::unique_ptr<PresetBundle> load_snapmaker_bundle()
{
    auto bundle = std::make_unique<PresetBundle>();
    bundle->load_vendor_configs_from_json(PROFILES_DIR, "Snapmaker", PresetBundle::LoadSystem, ForwardCompatibilitySubstitutionRule::EnableSilent,
                                          nullptr, /*allow_cache=*/false);
    for (PresetCollection *presets : { static_cast<PresetCollection *>(&bundle->printers), static_cast<PresetCollection *>(&bundle->prints),
                                       static_cast<PresetCollection *>(&bundle->filaments) })
        (presets->*(&RenameMapAccess::update_map_system_profile_renamed))();
    return bundle;
}

// The per-size printer preset of Snapmaker's layout, flat (tests/data/u1_per_size/make_fixtures.py).
DynamicPrintConfig old_printer(const std::string &size)
{
    DynamicPrintConfig                 config;
    std::map<std::string, std::string> key_values;
    std::string                        reason;
    ConfigSubstitutionContext          context{ ForwardCompatibilitySubstitutionRule::Enable };
    config.load_from_json(std::string(TEST_DATA_DIR) + "/u1_per_size/printer_" + size + ".json", context, true, key_values, reason);
    REQUIRE(reason.empty());
    Preset::normalize(config);
    return config;
}

double size_of(const std::string &size) { return NozzleFilament::parse_nozzle_size(size); }

// The value of head `head` for a per-extruder key: its first column.
std::string head_value(const DynamicPrintConfig &config, const std::string &key, size_t head)
{
    const auto *option = dynamic_cast<const ConfigOptionVectorBase *>(config.option(key));
    REQUIRE(option != nullptr);
    const std::vector<size_t> columns = extruder_option_columns(config, key, head);
    REQUIRE_FALSE(columns.empty());
    return option->vserialize()[columns.front()];
}

void select_u1(PresetBundle &bundle, const std::vector<double> &sizes, const char *process = PROCESS_04)
{
    REQUIRE(bundle.printers.select_preset_by_name(U1, true));
    REQUIRE(bundle.prints.select_preset_by_name(process, true));
    bundle.nozzle_filament_enabled = true;
    const VendorProfile::PrinterModel *table = NozzleFilament::size_table(bundle);
    REQUIRE(table != nullptr);
    bundle.printers.get_edited_preset().config = NozzleFilament::head_setup_config(bundle.printers.get_selected_preset().config, *table, sizes);
    bundle.filament_presets = std::vector<std::string>(sizes.size(), "Generic PLA");
    REQUIRE(bundle.filaments.select_preset_by_name("Generic PLA", true));
    bundle.project_config.option<ConfigOptionInts>("filament_map", true)->values = { 1, 2, 3, 4 };
    bundle.project_config.option<ConfigOptionEnumsGeneric>("nozzle_volume_type", true)->values = std::vector<int>(sizes.size(), int(nvtStandard));
}

// A project as Snapmaker Orca writes it for the per-size printer preset of `size`, unchanged: the
// printer's keys of that preset, its name, nothing listed as changed.
DynamicPrintConfig stock_project(PresetBundle &bundle, const std::string &size)
{
    select_u1(bundle, std::vector<double>(4, size_of(size)));
    DynamicPrintConfig project = bundle.full_config_secure();
    project.set_key_value("filament_colour", new ConfigOptionStrings({ "#FF0000", "#00FF00", "#0000FF", "#FFFF00" }));
    const DynamicPrintConfig printer = old_printer(size);
    // The printer's variant layout as the per-size preset states it, else the defaults Snapmaker Orca
    // writes for a preset without one.
    for (const char *key : { "printer_extruder_id", "printer_extruder_variant", "extruder_variant_list" }) {
        project.erase(key);
        project.option(key, true);   // the default
    }
    for (const std::string &key : printer.keys())
        if (project.has(key))
            project.set_key_value(key, printer.option(key)->clone());
    project.option<ConfigOptionString>("printer_settings_id", true)->value = "Snapmaker U1 (" + size + " nozzle)";
    project.option<ConfigOptionStrings>("inherits_group", true)->values = std::vector<std::string>(6, std::string());
    project.option<ConfigOptionStrings>("different_settings_to_system", true)->values = std::vector<std::string>(6, std::string());
    return project;
}

} // namespace

TEST_CASE("The U1 is one printer preset with a nozzle size table", "[U1Printer][Profiles]")
{
    auto bundle = load_snapmaker_bundle();
    const Preset *u1 = bundle->printers.find_preset(U1, false);
    REQUIRE(u1 != nullptr);
    CHECK(u1->name == U1);
    CHECK(u1->is_system);
    size_t u1_printers = 0;
    for (const Preset &preset : bundle->printers)
        if (preset.is_system && preset.config.opt_string("printer_model") == U1)
            ++u1_printers;
    CHECK(u1_printers == 1);
    // The old names lead to it.
    CHECK(u1->renamed_from.size() == SIZES.size());
    for (const std::string &size : SIZES) {
        INFO(size);
        const Preset *old = bundle->printers.find_preset("Snapmaker U1 (" + size + " nozzle)", false);
        REQUIRE(old != nullptr);
        CHECK(old->name == U1);
    }

    REQUIRE(bundle->printers.select_preset_by_name(U1, true));
    const VendorProfile::PrinterModel *table = NozzleFilament::size_table(*bundle);
    REQUIRE(table != nullptr);
    CHECK(NozzleFilament::printer_nozzle_sizes(*bundle) == SIZES);
    for (const std::string &size : SIZES) {
        INFO(size);
        const VendorProfile::NozzleSize *entry = NozzleFilament::size_entry(*bundle, size_of(size));
        REQUIRE(entry != nullptr);
        CHECK(entry->printer == "Snapmaker U1 (" + size + " nozzle)");
        CHECK(entry->high_flow == (size == "0.4"));
        CHECK(bundle->prints.find_preset(entry->default_print_profile, false) != nullptr);
        // The values of the size are those of its old preset.
        const DynamicPrintConfig old = old_printer(size);
        for (const auto &[key, value] : entry->values)
            CHECK(head_value(old, key, 0) == value);
    }
    CHECK(NozzleFilament::size_entry(*bundle, 0.5) == nullptr);
}

TEST_CASE("U1 process and filament presets carry their nozzle size", "[U1Printer][Profiles]")
{
    auto bundle = load_snapmaker_bundle();
    REQUIRE(bundle->printers.select_preset_by_name(U1, true));
    auto size = [&bundle](PresetCollection &presets, const std::string &name) {
        const Preset *preset = presets.find_preset(name, false);
        REQUIRE(preset != nullptr);
        return NozzleFilament::preset_nozzle_size(*bundle, *preset);
    };
    CHECK_THAT(size(bundle->prints, PROCESS_04), Catch::Matchers::WithinAbs(0.4, 1e-9));
    CHECK_THAT(size(bundle->prints, PROCESS_06), Catch::Matchers::WithinAbs(0.6, 1e-9));
    CHECK_THAT(size(bundle->prints, "Multi-nozzle Normal @Snapmaker U1"), Catch::Matchers::WithinAbs(0., 1e-9));
    CHECK_THAT(size(bundle->prints, "Multi-nozzle Normal @Snapmaker U1 (0.2 nozzle)"), Catch::Matchers::WithinAbs(0.2, 1e-9));
    CHECK_THAT(size(bundle->filaments, "Generic PLA"), Catch::Matchers::WithinAbs(0.4, 1e-9));
    CHECK_THAT(size(bundle->filaments, "Generic PLA @U1 0.2 nozzle"), Catch::Matchers::WithinAbs(0.2, 1e-9));
    // Every U1 preset names the one printer.
    for (PresetCollection *presets : { static_cast<PresetCollection *>(&bundle->prints), static_cast<PresetCollection *>(&bundle->filaments) })
        for (const Preset &preset : *presets)
            if (const auto *list = preset.config.option<ConfigOptionStrings>("compatible_printers"); preset.is_system && list != nullptr)
                for (const std::string &name : list->values) {
                    INFO(preset.name);
                    CHECK(name.find("Snapmaker U1 (") == std::string::npos);
                }
}

TEST_CASE("Tool heads of one size list that size's presets on the U1", "[U1Printer][Profiles]")
{
    auto bundle = load_snapmaker_bundle();
    auto compatible = [&bundle](PresetCollection &presets, const std::string &name) {
        const Preset *preset = presets.find_preset(name, false);
        REQUIRE(preset != nullptr);
        return preset->is_compatible;
    };
    SECTION("four 0.6 mm heads") {
        select_u1(*bundle, { 0.6, 0.6, 0.6, 0.6 });
        bundle->update_compatible(PresetSelectCompatibleType::Always);
        CHECK(compatible(bundle->prints, PROCESS_06));
        CHECK_FALSE(compatible(bundle->prints, PROCESS_04));
        CHECK(bundle->prints.get_selected_preset().name == PROCESS_06);
        CHECK(compatible(bundle->filaments, "Generic PLA @U1 0.6 nozzle"));
        CHECK_FALSE(compatible(bundle->filaments, "Generic PLA"));
    }
    SECTION("mixed heads: the multi-nozzle presets") {
        select_u1(*bundle, { 0.4, 0.6, 0.4, 0.4 });
        bundle->update_compatible(PresetSelectCompatibleType::Never);
        CHECK(compatible(bundle->prints, "Multi-nozzle Normal @Snapmaker U1"));
        CHECK_FALSE(compatible(bundle->prints, PROCESS_06));
        CHECK_FALSE(compatible(bundle->prints, "0.20mm High Quality @Snapmaker U1 (0.4 nozzle)"));
    }
    SECTION("a head of each size derives its process from presets of its size") {
        select_u1(*bundle, { 0.4, 0.2, 0.6, 0.4 });
        bundle->process_follows_nozzle = true;
        const std::vector<PerHeadProcess::Source> sources = PerHeadProcess::head_sources(*bundle);
        REQUIRE(sources.size() == 4);
        CHECK(sources[0].reason == PerHeadProcess::Reason::SelectedSize);
        REQUIRE(sources[1].preset != nullptr);
        CHECK_THAT(NozzleFilament::preset_nozzle_size(*bundle, *sources[1].preset), Catch::Matchers::WithinAbs(0.2, 1e-9));
        REQUIRE(sources[2].preset != nullptr);
        CHECK_THAT(NozzleFilament::preset_nozzle_size(*bundle, *sources[2].preset), Catch::Matchers::WithinAbs(0.6, 1e-9));
    }
    SECTION("a head's filament follows its size") {
        select_u1(*bundle, { 0.2, 0.4, 0.4, 0.4 });
        const NozzleFilament::State state = NozzleFilament::state(*bundle);
        const NozzleFilament::SlotTarget target = NozzleFilament::target_for_slot(*bundle, state, 0);
        CHECK(target.reason == NozzleFilament::Reason::Switched);
        CHECK(target.to == "Generic PLA @U1 0.2 nozzle");
        CHECK(NozzleFilament::target_for_slot(*bundle, state, 1).reason == NozzleFilament::Reason::Unchanged);
    }
    SECTION("High Flow is offered for the sizes the table says") {
        select_u1(*bundle, { 0.4, 0.6, 0.4, 0.4 });
        CHECK(NozzleFilament::size_has_high_flow_values(*bundle, 0.4));
        CHECK_FALSE(NozzleFilament::size_has_high_flow_values(*bundle, 0.6));
        CHECK(NozzleFilament::size_has_high_flow_values(*bundle, 0.));
    }
}

TEST_CASE("Four heads of one size on the U1 print with the values of that size's old preset", "[U1Printer][Profiles]")
{
    // Same-size heads print like Snapmaker Orca's per-size preset: the per-head values are equal.
    auto bundle = load_snapmaker_bundle();
    for (const std::string &size : SIZES) {
        INFO(size);
        select_u1(*bundle, std::vector<double>(4, size_of(size)));
        const DynamicPrintConfig &joined = bundle->printers.get_edited_preset().config;
        const DynamicPrintConfig  old    = old_printer(size);
        for (const std::string &key : old.keys()) {
            if (key == "printer_settings_id" || key == "printer_variant" || key == "default_print_profile" || key == "machine_start_gcode" ||
                key == "renamed_from" || boost::starts_with(key, "machine_max_") || key == "printer_extruder_id" ||
                key == "printer_extruder_variant" || key == "extruder_variant_list" || key == "default_nozzle_volume_type" ||
                key == "printer_flow_support")
                continue;
            const ConfigOption *mine = joined.option(key);
            REQUIRE(mine != nullptr);
            if (const auto *vector = dynamic_cast<const ConfigOptionVectorBase *>(old.option(key));
                vector != nullptr && (printer_options_with_variant_1.count(key) > 0 || printer_options_with_variant_2.count(key) > 0)) {
                INFO(key);
                // The fixture is the file as written: a key it states once holds for every head.
                const std::vector<std::string> values = vector->vserialize();
                for (size_t head = 0; head < 4; ++head)
                    CHECK(head_value(joined, key, head) == values[head < values.size() ? head : 0]);
            } else if (vector != nullptr && mine->is_vector()) {
                INFO(key);
                const std::vector<std::string> values = vector->vserialize();
                const std::vector<std::string> own    = static_cast<const ConfigOptionVectorBase *>(mine)->vserialize();
                for (size_t i = 0; i < own.size() && !values.empty(); ++i)
                    if (!values[i < values.size() ? i : 0].empty())   // empty: the file leaves it to the default
                        CHECK(own[i] == values[i < values.size() ? i : 0]);
            } else {
                INFO(key);
                CHECK(mine->serialize() == old.option(key)->serialize());
            }
        }
    }
}

TEST_CASE("A project of Snapmaker Orca naming a per-size U1 preset opens on the U1 with that size", "[U1Printer][FileBoundary]")
{
    for (const std::string &size : SIZES) {
        INFO(size);
        auto bundle = load_snapmaker_bundle();
        const DynamicPrintConfig project = stock_project(*bundle, size);
        // Another plate was open before.
        select_u1(*bundle, { 0.4, 0.4, 0.4, 0.4 });
        bundle->load_config_model("stock.3mf", project, Semver(2, 3, 0));

        const Preset &printer = bundle->printers.get_edited_preset();
        CHECK(printer.name == U1);
        CHECK(printer.config.option<ConfigOptionFloats>("nozzle_diameter")->values == std::vector<double>(4, size_of(size)));
        // The joined preset's layout, the values of the size.
        CHECK(printer.config.option<ConfigOptionStrings>("printer_extruder_variant")->size() ==
              bundle->printers.get_selected_preset().config.option<ConfigOptionStrings>("printer_extruder_variant")->size());
        const VendorProfile::NozzleSize *entry = NozzleFilament::size_entry(*bundle, size_of(size));
        REQUIRE(entry != nullptr);
        for (const auto &[key, value] : entry->values)
            for (size_t head = 0; head < 4; ++head) {
                INFO(key << " head " << head);
                CHECK(head_value(printer.config, key, head) == value);
            }
    }
}

TEST_CASE("A project of Snapmaker Orca with a changed setting keeps it on the U1", "[U1Printer][FileBoundary]")
{
    auto               bundle  = load_snapmaker_bundle();
    DynamicPrintConfig project = stock_project(*bundle, "0.6");
    project.option<ConfigOptionFloats>("retraction_length")->values = { 0.9, 0.9, 0.9, 0.9 };
    project.option<ConfigOptionStrings>("different_settings_to_system")->values[5] = "retraction_length";
    bundle->load_config_model("stock.3mf", project, Semver(2, 3, 0));
    const DynamicPrintConfig &printer = bundle->printers.get_edited_preset().config;
    for (size_t head = 0; head < 4; ++head) {
        CHECK(head_value(printer, "retraction_length", head) == "0.9");
        CHECK(head_value(printer, "retraction_minimum_travel", head) == "3");
    }
}

TEST_CASE("Max writes the U1 under the name of Snapmaker Orca and reads it back", "[U1Printer][FileBoundary]")
{
    auto bundle = load_snapmaker_bundle();
    select_u1(*bundle, { 0.4, 0.6, 0.4, 0.2 });
    const DynamicPrintConfig written = bundle->full_config_secure();
    CHECK(written.opt_string("printer_settings_id") == U1_04_OLD);
    CHECK(NozzleFilament::file_printer_name(*bundle, U1) == U1_04_OLD);
    CHECK(NozzleFilament::file_printer_name(*bundle, "Snapmaker J1 (0.4 nozzle)") == "Snapmaker J1 (0.4 nozzle)");
    // The changed settings are those against the plain preset: the sizes count as changes there.
    const auto *different = written.option<ConfigOptionStrings>("different_settings_to_system");
    REQUIRE(different != nullptr);
    CHECK(different->values.back().find("nozzle_diameter") != std::string::npos);

    auto other = load_snapmaker_bundle();
    select_u1(*other, { 0.4, 0.4, 0.4, 0.4 });
    other->load_config_model("max.3mf", written, Semver(2, 3, 0));
    const DynamicPrintConfig &printer = other->printers.get_edited_preset().config;
    CHECK(other->printers.get_edited_preset().name == U1);
    CHECK(printer.option<ConfigOptionFloats>("nozzle_diameter")->values == std::vector<double>{ 0.4, 0.6, 0.4, 0.2 });
    for (const std::string &key : { "retraction_length", "min_layer_height", "max_layer_height", "nozzle_type", "wipe_distance" })
        for (size_t head = 0; head < 4; ++head) {
            INFO(key << " head " << head);
            CHECK(head_value(printer, key, head) == head_value(bundle->printers.get_edited_preset().config, key, head));
        }
}

TEST_CASE("The U1 printer preset names no nozzle size; files for Snapmaker Orca name the 0.4 mm one", "[U1Printer][FileBoundary]")
{
    auto bundle = load_snapmaker_bundle();
    select_u1(*bundle, { 0.6, 0.6, 0.6, 0.6 });
    const Preset &u1 = bundle->printers.get_selected_preset();
    CHECK(u1.config.opt_string("printer_variant") == "U1");
    CHECK(NozzleFilament::parse_nozzle_size(u1.config.opt_string("printer_variant")) == 0.);
    const DynamicPrintConfig written = bundle->full_config_secure();
    CHECK(written.opt_string("printer_settings_id") == U1_04_OLD);
    CHECK(written.opt_string("printer_variant") == "0.4");
    // The edited printer keeps its own.
    CHECK(bundle->printers.get_edited_preset().config.opt_string("printer_variant") == "U1");
}

TEST_CASE("An installation with any U1 nozzle size installed has the U1 installed", "[U1Printer][Profiles]")
{
    auto bundle = load_snapmaker_bundle();
    AppConfig config;
    config.set_variant(PresetBundle::SM_BUNDLE, U1, "0.2", true);
    CHECK_FALSE(config.get_variant(PresetBundle::SM_BUNDLE, U1, "U1"));
    bundle->migrate_renamed_printer_settings(config);
    CHECK(config.get_variant(PresetBundle::SM_BUNDLE, U1, "U1"));
    // Settings remembered under the old name in use last are taken over.
    AppConfig selections;
    selections.set("presets", PRESET_PRINTER_NAME, std::string("Snapmaker U1 (0.6 nozzle)"));
    selections.set_printer_setting("Snapmaker U1 (0.6 nozzle)", PRESET_PRINT_NAME, "0.30mm Standard @Snapmaker U1 (0.6 nozzle)");
    selections.set_printer_setting(U1_04_OLD, PRESET_PRINT_NAME, PROCESS_04);
    bundle->migrate_renamed_printer_settings(selections);
    CHECK(selections.get_printer_setting(U1, PRESET_PRINT_NAME) == "0.30mm Standard @Snapmaker U1 (0.6 nozzle)");
}

TEST_CASE("An installation that chose any U1 nozzle size shows the U1", "[U1Printer][Profiles]")
{
    auto    bundle = load_snapmaker_bundle();
    Preset *u1     = bundle->printers.find_preset(U1, false);
    REQUIRE(u1 != nullptr);
    AppConfig config;
    config.set_variant(PresetBundle::SM_BUNDLE, U1, "0.6", true);
    u1->set_visible_from_appconfig(config);
    CHECK(u1->is_visible);
    AppConfig none;
    u1->set_visible_from_appconfig(none);
    CHECK_FALSE(u1->is_visible);
}

TEST_CASE("The nozzle size of a tool head is no change of the U1 printer preset", "[U1Printer][HeadSetup]")
{
    auto bundle = load_snapmaker_bundle();
    select_u1(*bundle, { 0.4, 0.6, 0.4, 0.2 });
    PresetCollection &printers = bundle->printers;
    CHECK_FALSE(printers.current_is_dirty());
    CHECK(printers.current_dirty_options().empty());

    // A real edit is one.
    DynamicPrintConfig &edited = printers.get_edited_preset().config;
    auto *lengths = edited.option<ConfigOptionFloats>("retraction_length");
    for (size_t column : extruder_option_columns(edited, "retraction_length", 1))
        lengths->values[column] = 2.5;
    CHECK(printers.current_is_dirty());
    CHECK(printers.current_dirty_options() == std::vector<std::string>{ "retraction_length" });

    // Discarding it keeps the head setup.
    printers.discard_current_changes();
    CHECK(printers.get_edited_preset().config.option<ConfigOptionFloats>("nozzle_diameter")->values == std::vector<double>{ 0.4, 0.6, 0.4, 0.2 });
    CHECK(head_value(printers.get_edited_preset().config, "retraction_length", 1) == "1.4");
    CHECK_FALSE(printers.current_is_dirty());

    // The file still lists the sizes as changed against the plain preset, for Snapmaker Orca.
    const DynamicPrintConfig written = bundle->full_config_secure();
    CHECK(written.option<ConfigOptionStrings>("different_settings_to_system")->values.back().find("nozzle_diameter") != std::string::npos);
}

TEST_CASE("The head setup remembered for the U1 is applied with the values of each size", "[U1Printer][HeadSetup]")
{
    auto bundle = load_snapmaker_bundle();
    select_u1(*bundle, { 0.4, 0.4, 0.4, 0.4 });
    AppConfig config;
    CHECK(bundle->head_setup_key() == U1);
    CHECK_FALSE(bundle->apply_remembered_head_setup(config));   // nothing remembered

    config.save_nozzle_diameters_to_config(U1, "0.4,0.6,0.4,0.8");
    CHECK(bundle->apply_remembered_head_setup(config));
    const DynamicPrintConfig &edited = bundle->printers.get_edited_preset().config;
    CHECK(edited.option<ConfigOptionFloats>("nozzle_diameter")->values == std::vector<double>{ 0.4, 0.6, 0.4, 0.8 });
    CHECK(head_value(edited, "retraction_minimum_travel", 1) == "3");
    CHECK(head_value(edited, "max_layer_height", 3) == "0.56");
    CHECK_FALSE(bundle->printers.current_is_dirty());
    // Applied again: nothing to do.
    CHECK_FALSE(bundle->apply_remembered_head_setup(config));

    // A setup for another number of heads is left alone.
    config.save_nozzle_diameters_to_config(U1, "0.4,0.6");
    CHECK_FALSE(bundle->apply_remembered_head_setup(config));
}
