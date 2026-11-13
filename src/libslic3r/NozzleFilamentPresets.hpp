#pragma once

// Snapmaker Orca: filament presets follow the nozzle size of the tool head that prints them.
// The U1 ships one filament preset per nozzle size, pinned by "compatible_printers"; these helpers
// map slots to tool heads and machine presets and pick the version a slot should hold.

#include <cstddef>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Preset.hpp"

namespace Slic3r {

class DynamicPrintConfig;
class Preset;
class PresetBundle;
class PresetCollection;
class PrinterPresetCollection;

namespace NozzleFilament {

// A filament slot no tool head of its own prints: a slot beyond the tool heads, a mixed
// (virtual) slot.
constexpr size_t no_head = size_t(-1);

// The 0-based tool head of each filament, as the engine decides it: a binding (1-based)
// `filament_map` is followed; otherwise, and for invalid entries, filament i goes to head i and
// filaments beyond the heads to `master_head` (ToolOrdering::get_recommended_filament_maps).
std::vector<size_t> filament_heads(size_t filament_count, size_t head_count, const std::vector<int> &filament_map, bool map_is_binding, size_t master_head = 0);

// "0.4" as a number, whatever the decimal separator of the user's locale is; 0 when the text
// is no size.
double parse_nozzle_size(const std::string &text);

// The machine preset of printer model `model` for the nozzle size spelled `variant_label`
// ("0.2", the spelling of "printer_variant"): the system preset, else a custom one. Visibility
// plays no role. nullptr when the model has no profile of that size.
const Preset* head_machine_preset(const PrinterPresetCollection &printers, const std::string &model, const std::string &variant_label);
// The same by value, for a caller that holds the diameter and not the spelling of the profile.
const Preset* head_machine_preset(const PrinterPresetCollection &printers, const std::string &model, double nozzle_size);

// The machine preset that stands for a tool head of nozzle size `size` on the edited printer preset:
// the edited preset itself when it was made for that size or names no size, else the preset of
// that size of the printer model (head_machine_preset). nullptr when the model has no profile of
// that size. The one place a tool head's size is mapped onto the vendor's per-size printer
// profiles; every rule below asks it with the head's own size, never with the printer's.
const Preset* size_machine(const PresetBundle &bundle, double size);

// Snapmaker Orca Max: a printer model with one printer preset for every nozzle size carries a size
// table (VendorProfile::NozzleSize). The table of the edited printer preset's model; nullptr for a
// model with a printer preset per size.
const VendorProfile::PrinterModel* size_table(const PresetBundle &bundle);
// The entry of the table for `size`; nullptr without a table or for a size it has no values for.
const VendorProfile::NozzleSize*   size_entry(const PresetBundle &bundle, double size);
// The sizes of the table, as it spells them ("0.2"); empty without a table.
std::vector<std::string>           printer_nozzle_sizes(const PresetBundle &bundle);
// The process preset a head of `size` starts with: the table's, else the size's printer preset's.
std::string                        size_default_process(const PresetBundle &bundle, double size);
// `printer_config` with tool head i set to nozzle size sizes[i] and the values the table holds for
// it, in every column of the head. A head whose size the table lacks keeps its values.
DynamicPrintConfig                 head_setup_config(const DynamicPrintConfig &printer_config, const VendorProfile::PrinterModel &model,
                                                     const std::vector<double> &sizes);
// The printer values of `size` on every head: head_setup_config() of the selected printer preset with
// a table, else the printer preset of that size. nullopt for a size without values.
std::optional<DynamicPrintConfig>  size_config(const PresetBundle &bundle, double size);
// The name projects and G-code store for printer preset `name`: a printer preset for every size is
// written as the per-size preset of its own variant ("Snapmaker U1" -> "Snapmaker U1 (0.4 nozzle)"),
// which Snapmaker Orca knows; every other name as it is.
std::string file_printer_name(const PresetBundle &bundle, const std::string &name);
// The same for a printer preset at hand (a system preset knows its vendor).
std::string file_printer_name(const Preset &printer);
// The "printer_variant" files carry for printer preset `printer` (a user preset: its system parent's):
// the size of that per-size preset for a printer preset for every size, else its own.
std::string file_printer_variant(const PresetBundle &bundle, const Preset &printer);
std::string file_printer_variant(const Preset &printer);
// The table says `size` has High Flow values (any size for a `size` <= 0); false without a table.
bool                               size_has_high_flow_values(const PresetBundle &bundle, double size);

struct State
{
    // Level 1: the rule applies to this printer at all (PresetBundle::nozzle_filament_enabled, FFF,
    // more than one nozzle, no single extruder multi material, a Snapmaker printer preset).
    bool rule_on { false };
    // Level 2: rule_on, and the tool heads carry different nozzle sizes.
    bool mixed { false };
    // Per tool head: "nozzle_diameter". Empty unless rule_on.
    std::vector<double>        head_size;
    // Per filament slot: its tool head, or no_head. Empty unless rule_on.
    std::vector<size_t>        slot_head;

    size_t head_of(size_t slot) const { return slot < slot_head.size() ? slot_head[slot] : no_head; }
    // The nozzle size of the tool head of `slot`; 0 for a slot without a tool head.
    double size_of(size_t slot) const
    {
        const size_t head = this->head_of(slot);
        return head < head_size.size() ? head_size[head] : 0.;
    }
};
State state(const PresetBundle &bundle);
// The same without the gate on PresetBundle::nozzle_filament_enabled: which tool head carries
// which size, whatever the filament rule is set to. The process rule
// (PerHeadProcess) reads it under its own preference.
State head_state(const PresetBundle &bundle);

// Printer sync: keep `edited` when it matches the model and nozzle size of `picked` (from
// PresetBundle::get_similar_printer_preset()), so the sync only sets tool head sizes and flow
// types without re-selecting a modified preset. False: the sync selects `picked`.
bool sync_keeps_printer_preset(const Preset &edited, const Preset &picked);

// The first system preset along "inherits", the preset itself when it is one. nullptr for a user
// preset without a system ancestor.
const Preset* system_ancestor(const PresetCollection &filaments, const Preset &preset);

// The preset restricts the printers it serves, by "compatible_printers" or by
// "compatible_printers_condition". One that does neither fits every printer and is the version of
// no nozzle size.
bool restricts_printers(const Preset &preset);

// The system filament preset of the family of `system_preset` (Preset::system_inherits + alias)
// made for a tool head of nozzle size `size`. nullptr when no member or two members (logged) serve
// it, or without a family.
const Preset* version_for(const PresetBundle &bundle, const Preset &system_preset, double size);

// The nozzle size a filament or process preset is made for (its size tag): the size of the machine
// presets it lists, when they agree. 0 when the preset is made for no single size.
double preset_nozzle_size(const PresetBundle &bundle, const Preset &preset);

// The nozzle a filament made for no size is assumed to be tuned for (SlotTarget::size_agnostic).
constexpr double standard_nozzle_size = 0.4;

// A filament slot whose preset is made for another nozzle size than the tool head that prints it.
struct SizeMismatch
{
    size_t      slot { 0 };
    size_t      head { 0 };
    std::string preset;
    double      preset_size { 0. };
    double      head_size { 0. };
};
// The slots of `slots` (0-based) whose preset is made for another size (preset_nozzle_size) than
// their tool head `filament_head[slot]` of size `head_size[head]` carries. A preset made for no
// size, and a slot without a tool head, never mismatch. The plate does not slice with one.
std::vector<SizeMismatch> size_mismatches(const PresetBundle &bundle, const std::vector<size_t> &filament_head, const std::vector<double> &head_size,
                                          const std::vector<size_t> &slots);

// Combo label with the size first ("0.6 mm · Generic PLA") so a narrow combo cuts the name, not
// the size. `label` unchanged when `size_text` is empty. Display only.
std::string size_marked_label(const std::string &label, const std::string &size_text);
// The marker of size_marked_label() in a slot on a High Flow extruder: "HF" after the size text
// ("0.4 mm · HF") for a preset with High Flow values, "HF" alone without a size; `size_text`
// unchanged for a preset without them.
std::string high_flow_marker(const std::string &size_text, bool has_high_flow_values);

// `filament` may print on a tool head of nozzle size `size`: made for that size or for none, compatible
// with the printer for that size (size_machine()), not excluded from the edited printer preset (the
// arm of the filament library) and compatible with the edited process preset.
bool fits(const PresetBundle &bundle, const Preset &filament, double size);

// `filament` may print on this printer with a nozzle of some size: fits() for one of the sizes the
// printer model has a profile for, or for the edited printer preset.
bool fits_printer(const PresetBundle &bundle, const Preset &filament);

// The visible user presets made from system preset `system` (system_ancestor()) that may print
// on a tool head of nozzle size `size` (fits()); the preset named `exclude` is left out.
std::vector<const Preset*> user_children(const PresetBundle &bundle, const Preset &system, double size, const std::string &exclude = {});

// Printer a user filament saved from an unrestricted system filament is pinned to ("compatible_printers"):
// the printer for the nozzle size of slot `slot`'s head (size_machine()) when that is another preset than
// the edited one, else the printer preset's parent.
std::string printer_to_pin(const PresetBundle &bundle, int slot);

// `filament` may print on an extruder set to High Flow ("nozzle_volume_type" of the project): fits() with
// the nozzle size of that extruder (head_state()).
bool fits_high_flow_extruder(const PresetBundle &bundle, const Preset &filament);

// The filament slots holding preset `from` whose tool head `to` may print on (fits() with the nozzle
// size of the head, head_state(); fits_printer() for a slot without one): the slots a preset saved
// from `from` under a new name takes over.
std::vector<size_t> slots_to_switch(const PresetBundle &bundle, const std::string &from, const Preset &to);

// The family of `preset` as one string (parent and alias of its system ancestor); empty for a
// preset without a family. The first half of a key of PresetBundle::nozzle_filament_memory.
std::string family_key(const PresetCollection &filaments, const Preset &preset);
// Notes for the session that slot `slot` leaves user preset `name` because it does not fit the
// slot's tool head, so target_for_slot() restores it when a tool head returns to that size.
// A user preset that fits is not noted. No-op unless the rule is on.
void remember_user_preset(PresetBundle &bundle, size_t slot, const std::string &name);

enum class Reason {
    Unchanged,          // the slot holds a preset that fits its tool head
    Switched,           // `to` is the version for the size of the tool head
    NoVersion,          // no version for that size; the slot keeps its preset
    NoFamily,           // the preset belongs to no family (no parent, no alias)
    UserPresetKept,     // a user preset without exactly one counterpart for that size
    DirtyKept,          // the preset carries unsaved changes that no other slot would keep
    Skipped             // no tool head, no name, a placeholder, an unknown preset, rule off
};

struct SlotTarget
{
    size_t      slot { 0 };
    size_t      head { no_head };
    std::string from;                       // the name the slot holds, as it holds it
    std::string to;                         // the name it should hold: another preset when Switched,
                                            // else the preset it holds (under its present name, had it been renamed)
    Reason      reason { Reason::Skipped };
    bool        size_agnostic { false };    // fits, restricts no printer, on a tool head of another size than standard_nozzle_size
    std::string suggestion;                 // UserPresetKept: the system version for the size of the tool head
    double      head_size { 0. };
    double      preset_size { 0. };         // preset_nozzle_size() of `from`

    bool switches() const { return reason == Reason::Switched && to != from; }
};

// What slot `slot` should hold. `heads` names the tool heads a pass moves (empty: all of them); it
// only matters for DirtyKept, where a slot of another tool head counts as one that keeps its
// preset. Never yields another material: `to` is `from` or a preset of its family.
SlotTarget target_for_slot(const PresetBundle &bundle, const State &state, size_t slot, const std::vector<size_t> &heads = {});

// The name of a reason ("Switched"), for the log.
const char* reason_name(Reason reason);
// One log line per slot: "NozzleFilament: trigger= filament= head= size= flow= from= to= reason="
// plus preset_size, size_agnostic and suggestion when set. `head_flow`: NozzleVolumeType per tool
// head (Standard beyond it). A slot without a tool head prints "-" for head, size and flow.
std::string describe_target(const SlotTarget &target, const std::string &trigger, const std::vector<int> &head_flow);
// describe_target() of every target on the info level of the log; the flow types are
// "nozzle_volume_type" of the project config. `trigger` names the event that made the pass, or
// the check that only reports.
void log_targets(const PresetBundle &bundle, const std::vector<SlotTarget> &targets, const std::string &trigger);

// The name resources/info/nozzle_incompatibles.json uses for `preset_name`: the alias of its
// system ancestor, else the part of the name in front of the '@'.
std::string blacklist_name(const PresetCollection &filaments, const std::string &preset_name);

// The blacklisted names for a nozzle of the given size and flow type (NozzleVolumeType as int).
using NozzleBlacklist = std::function<std::vector<std::string>(double nozzle_size, int nozzle_volume_type)>;
// (nozzle size, flow type) -> the `names` blacklisted for that nozzle, each filament rated against
// the nozzle of its tool head `filament_head[f]`; a filament without a tool head is not rated.
// A head beyond `head_volume_type` takes the first type.
std::map<std::pair<double, int>, std::set<std::string>> blacklisted_by_head(const std::vector<std::string> &names, const std::vector<size_t> &filament_head,
                                                                           const std::vector<double> &head_size, const std::vector<int> &head_volume_type,
                                                                           const NozzleBlacklist &blacklist);

} // namespace NozzleFilament
} // namespace Slic3r
