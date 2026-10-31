#include <catch2/catch_all.hpp>

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
        CHECK(multi_nozzle == 5);
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
        {"Ultra fine", 0.25}, {"Fine", 0.4}, {"Normal", 0.5}, {"Coarse", 0.6}, {"Super coarse", 0.7}};
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
    for (const char *strategy : {"Ultra fine", "Fine", "Normal", "Coarse", "Super coarse"}) {
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
