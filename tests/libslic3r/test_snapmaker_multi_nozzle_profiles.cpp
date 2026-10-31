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
