#include <catch2/catch_all.hpp>

#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "libslic3r/PresetBundle.hpp"
#include "libslic3r/PrintConfig.hpp"

using namespace Slic3r;

// Snapmaker Orca Max: the shipped U1 process presets for tool heads with different nozzle sizes,
// loaded as the application does.

namespace {

const char *const U1_MACHINE_0_4 = "Snapmaker U1 (0.4 nozzle)";
const char *const U1_STANDARD    = "0.20mm Standard @Snapmaker U1 (0.4 nozzle)";
const char *const U1_MULTI       = "Multi-nozzle Normal @Snapmaker U1";

std::unique_ptr<PresetBundle> load_snapmaker_bundle()
{
    auto bundle = std::make_unique<PresetBundle>();
    bundle->load_vendor_configs_from_json(PROFILES_DIR, "Snapmaker", PresetBundle::LoadSystem,
                                          ForwardCompatibilitySubstitutionRule::EnableSilent, nullptr,
                                          /*allow_cache=*/false);
    return bundle;
}

bool is_multi_nozzle(const Preset &preset)
{
    const auto *strategy = preset.config.option<ConfigOptionEnum<MultiNozzleLayerHeights>>("multi_nozzle_layer_heights");
    return strategy != nullptr && strategy->value != mnlhOff && strategy->value != mnlhCustom;
}

// The visible system process presets the list offers for the edited printer.
std::vector<const Preset *> listed_system_processes(const PresetBundle &bundle)
{
    std::vector<const Preset *> out;
    for (const Preset &preset : bundle.prints)
        if (preset.is_system && preset.is_visible && preset.is_compatible)
            out.emplace_back(&preset);
    return out;
}

} // namespace

TEST_CASE("Every selectable U1 process preset prints walls with Arachne", "[Profiles][MultiNozzle]")
{
    auto bundle = load_snapmaker_bundle();
    size_t u1_presets = 0;
    for (const Preset &preset : bundle->prints) {
        if (!preset.is_system || !preset.is_visible || preset.name.find("@Snapmaker U1") == std::string::npos)
            continue;
        ++u1_presets;
        INFO(preset.name);
        const auto *generator = preset.config.option<ConfigOptionEnum<PerimeterGeneratorType>>("wall_generator");
        REQUIRE(generator != nullptr);
        CHECK(generator->value == PerimeterGeneratorType::Arachne);
    }
    CHECK(u1_presets > 0);
}

TEST_CASE("Mixed nozzle sizes list only the multi-nozzle process presets and the selected one", "[Profiles][MultiNozzle]")
{
    auto bundle = load_snapmaker_bundle();
    REQUIRE(bundle->printers.select_preset_by_name(U1_MACHINE_0_4, true));
    REQUIRE(bundle->prints.select_preset_by_name(U1_STANDARD, true));

    SECTION("one nozzle size: the single-size presets stay listed")
    {
        bundle->update_compatible(PresetSelectCompatibleType::Never);
        size_t single_size = 0;
        for (const Preset *preset : listed_system_processes(*bundle))
            single_size += is_multi_nozzle(*preset) ? 0 : 1;
        CHECK(single_size > 1);
    }

    SECTION("a 0.2 mm nozzle on the fourth tool head: multi-nozzle presets plus the selected one")
    {
        bundle->printers.get_edited_preset().config.option<ConfigOptionFloats>("nozzle_diameter")->values = {0.4, 0.4, 0.4, 0.2};
        bundle->update_compatible(PresetSelectCompatibleType::Never);
        size_t multi_nozzle = 0;
        for (const Preset *preset : listed_system_processes(*bundle)) {
            INFO(preset->name);
            if (is_multi_nozzle(*preset))
                ++multi_nozzle;
            else
                CHECK(preset->name == U1_STANDARD);
        }
        // Ultra fine, Fine, Normal, Coarse, Super coarse, Fine High Quality, Normal High Quality.
        CHECK(multi_nozzle == 7);
        // The selected single-size preset is kept, so a project that uses it keeps it.
        CHECK(bundle->prints.get_selected_preset().name == U1_STANDARD);
        const Preset *multi = bundle->prints.find_preset(U1_MULTI, false, true);
        REQUIRE(multi != nullptr);
        CHECK(multi->is_compatible);
    }
}

TEST_CASE("Every multi-nozzle preset ships a hidden preset per other nozzle size at the strategy's layer height", "[Profiles][MultiNozzle]")
{
    // The strategy's share of the nozzle diameter (Slicing.hpp multi_nozzle_strategy_height).
    const std::vector<std::pair<std::string, double>> strategies = {
        {"Ultra fine", 0.25}, {"Fine", 0.4}, {"Normal", 0.5}, {"Coarse", 0.6}, {"Super coarse", 0.7},
        {"Fine High Quality", 0.4}, {"Normal High Quality", 0.5}};
    auto bundle = load_snapmaker_bundle();
    for (const auto &[strategy, share] : strategies)
        for (const char *size : {"0.2", "0.6", "0.8"}) {
            const std::string name = "Multi-nozzle " + strategy + " @Snapmaker U1 (" + size + " nozzle)";
            INFO(name);
            const Preset *preset = bundle->prints.find_preset(name, false);
            REQUIRE(preset != nullptr);
            CHECK(preset->is_system);
            CHECK_FALSE(preset->is_visible);
            const double nozzle = std::stod(size);
            CHECK_THAT(preset->config.opt_float("layer_height"), Catch::Matchers::WithinAbs(share * nozzle, 1e-6));
            CHECK(preset->config.option<ConfigOptionStrings>("compatible_printers")->values ==
                  std::vector<std::string>{std::string("Snapmaker U1 (") + size + " nozzle)"});
            // Speeds and line widths of the size: no strategy of its own.
            const auto *own_strategy = preset->config.option<ConfigOptionEnum<MultiNozzleLayerHeights>>("multi_nozzle_layer_heights");
            CHECK((own_strategy == nullptr || own_strategy->value == mnlhOff));
        }
    // The 0.4 mm presets carry their values themselves: no hidden 0.4 mm preset.
    CHECK(bundle->prints.find_preset("Multi-nozzle Normal @Snapmaker U1 (0.4 nozzle)", false) == nullptr);
}

TEST_CASE("Every 0.4 mm multi-nozzle preset carries a High Flow column", "[Profiles][MultiNozzle][HighFlow]")
{
    auto bundle = load_snapmaker_bundle();
    for (const char *strategy : {"Ultra fine", "Fine", "Normal", "Coarse", "Super coarse", "Fine High Quality", "Normal High Quality"}) {
        const std::string name = std::string("Multi-nozzle ") + strategy + " @Snapmaker U1";
        INFO(name);
        const Preset *preset = bundle->prints.find_preset(name, false);
        REQUIRE(preset != nullptr);
        CHECK(preset->config.option<ConfigOptionStrings>("print_extruder_variant")->values ==
              std::vector<std::string>{"Direct Drive Standard", "Direct Drive High Flow"});
        const auto *outer = dynamic_cast<const ConfigOptionVectorBase *>(preset->config.option("outer_wall_speed"));
        REQUIRE(outer != nullptr);
        const std::vector<std::string> columns = outer->vserialize();
        REQUIRE(columns.size() == 2);
        // High Flow is never slower than Standard.
        CHECK(std::stod(columns[1]) >= std::stod(columns[0]));
    }
}

TEST_CASE("Every nozzle size has a hidden height preset one grid step from each strategy's height", "[Profiles][MultiNozzle]")
{
    // One step finer or coarser than the 25 / 40 / 50 / 60 / 70 % heights, within the U1 machines' limits.
    const std::vector<std::pair<std::string, std::vector<std::string>>> heights = {
        {"0.2", {"0.10"}},
        {"0.4", {"0.08", "0.10", "0.12", "0.14", "0.15", "0.24", "0.30"}},
        {"0.6", {"0.16", "0.20", "0.24", "0.28", "0.32", "0.40"}},
        {"0.8", {"0.24", "0.25", "0.30", "0.36", "0.40", "0.42", "0.50"}}};
    auto bundle = load_snapmaker_bundle();
    size_t count = 0;
    for (const Preset &preset : bundle->prints)
        count += preset.is_system && preset.name.find("mm Multi-nozzle @Snapmaker U1 (") != std::string::npos ? 1 : 0;
    CHECK(count == 21);
    for (const auto &[size, list] : heights)
        for (const std::string &height : list) {
            const std::string name = height + "mm Multi-nozzle @Snapmaker U1 (" + size + " nozzle)";
            INFO(name);
            const Preset *preset = bundle->prints.find_preset(name, false);
            REQUIRE(preset != nullptr);
            CHECK(preset->is_system);
            CHECK_FALSE(preset->is_visible);
            CHECK_THAT(preset->config.opt_float("layer_height"), Catch::Matchers::WithinAbs(std::stod(height), 1e-6));
        }
}

TEST_CASE("The High Quality strategy presets slow the walls and accelerations of every nozzle size", "[Profiles][MultiNozzle]")
{
    auto bundle = load_snapmaker_bundle();
    auto first = [](const Preset &preset, const char *key) {
        const auto *option = dynamic_cast<const ConfigOptionVectorBase *>(preset.config.option(key));
        REQUIRE(option != nullptr);
        return std::stod(option->vserialize().front());
    };
    for (const char *strategy : {"Fine", "Normal"})
        for (const char *suffix : {"", " (0.2 nozzle)", " (0.6 nozzle)", " (0.8 nozzle)"}) {
            const std::string standard = std::string("Multi-nozzle ") + strategy + " @Snapmaker U1" + suffix;
            const std::string quality  = std::string("Multi-nozzle ") + strategy + " High Quality @Snapmaker U1" + suffix;
            INFO(quality);
            const Preset *s = bundle->prints.find_preset(standard, false);
            const Preset *q = bundle->prints.find_preset(quality, false);
            REQUIRE(s != nullptr);
            REQUIRE(q != nullptr);
            CHECK(first(*q, "outer_wall_speed") <= 60.);
            CHECK(first(*q, "default_acceleration") <= 4000.);
            CHECK(first(*q, "outer_wall_acceleration") <= 2000.);
            CHECK(first(*q, "inner_wall_speed") <= first(*s, "inner_wall_speed"));
            CHECK(first(*q, "sparse_infill_speed") <= first(*s, "sparse_infill_speed"));
            CHECK_THAT(q->config.opt_float("layer_height"), Catch::Matchers::WithinAbs(s->config.opt_float("layer_height"), 1e-6));
        }
}
