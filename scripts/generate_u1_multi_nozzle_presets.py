#!/usr/bin/env python3
"""Snapmaker Orca Max: writes the per nozzle size presets of the U1 multi-nozzle process presets.

A "Multi-nozzle <Strategy> @Snapmaker U1" preset is made for 0.4 mm nozzles. A tool head with another
nozzle size prints with the speeds, accelerations and line widths of the hidden preset
"Multi-nozzle <Strategy> @Snapmaker U1 (<size> nozzle)" (src/libslic3r/PerHeadProcess.cpp). The presets
are instantiated, so the application loads them, and hidden from every list when loaded
(PerHeadProcess::hide_multi_nozzle_size_presets). That preset
is the Standard preset of the strategy's layer height for its size when Snapmaker ships one, else
Snapmaker's base of that height for the size plus the settings the nearest Standard preset of the size
adds on top of its own base.

The 0.4 mm presets whose parent has no High Flow column get one, so a High Flow tool head never falls
back to another preset.

Run from the repository root. A new preset needs an index entry in Snapmaker.json and a setting_id
(scripts/orca_profile_tool.py generate-id --setting-id).
"""

import json
import pathlib
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
from orca_profile_tool import NORMALIZE_FIELD_ORDER, OBSOLETE_KEYS  # noqa: E402

PROCESS_DIR = pathlib.Path(__file__).resolve().parent.parent / "resources" / "profiles" / "Snapmaker" / "process"

# Strategy -> layer height per nozzle size (the strategy's share of the diameter, Slicing.cpp
# multi_nozzle_strategy_height): 25 / 40 / 50 / 60 / 70 %.
STRATEGIES = {
    "Ultra fine":   {"0.2": "0.05", "0.6": "0.15", "0.8": "0.20"},
    "Fine":         {"0.2": "0.08", "0.6": "0.24", "0.8": "0.32"},
    "Normal":       {"0.2": "0.10", "0.6": "0.30", "0.8": "0.40"},
    "Coarse":       {"0.2": "0.12", "0.6": "0.36", "0.8": "0.48"},
    "Super coarse": {"0.2": "0.14", "0.6": "0.42", "0.8": "0.56"},
}

# The Standard preset whose own settings a size's presets without an exact match take.
NEAREST_STANDARD = {
    "0.2": "0.12mm Standard @Snapmaker U1 (0.2 nozzle)",
    "0.6": "0.30mm Standard @Snapmaker U1 (0.6 nozzle)",
    "0.8": "0.40mm Standard @Snapmaker U1 (0.8 nozzle)",
}

# Keys of a preset that are not settings.
META_KEYS = {"type", "name", "inherits", "from", "setting_id", "instantiation", "compatible_printers",
             "description", "renamed_from", "print_settings_id"}

# High Flow speeds of the 0.4 mm presets (the High Flow column of 0.20mm Standard, the only one
# Snapmaker ships). Ultra fine builds on the slow 0.12mm Standard: its High Flow column keeps the
# Standard speeds, a High Flow nozzle gains nothing at those speeds.
HIGH_FLOW_SPEEDS = {"outer_wall_speed": "500", "inner_wall_speed": "600", "sparse_infill_speed": "600",
                    "internal_solid_infill_speed": "600"}
HIGH_FLOW_PRESETS = {"Ultra fine": False, "Fine": True, "Coarse": True, "Super coarse": True}


def load(name):
    with open(PROCESS_DIR / f"{name}.json", encoding="utf-8-sig") as f:
        return json.load(f)


def resolved(name):
    chain = []
    while name:
        preset = load(name)
        chain.append(preset)
        name = preset.get("inherits", "")
    out = {}
    for preset in reversed(chain):
        out.update(preset)
    return out


def scalar(value):
    return value[0] if isinstance(value, list) else value


def nearest_base(size, height):
    """The base fdm_process_U1_<h>_nozzle_<size> nearest to `height` (a tie goes to the finer one)."""
    heights = sorted(path.stem.split("_")[3] for path in PROCESS_DIR.glob(f"fdm_process_U1_*_nozzle_{size}.json"))
    best = min(heights, key=lambda h: (abs(float(h) - float(height)), float(h)))
    return f"fdm_process_U1_{best}_nozzle_{size}", best


def write(name, preset):
    # The shape orca_profile_tool.py normalize gives a file: identifying keys first, no obsolete keys.
    preset = {key: value for key, value in preset.items() if key not in OBSOLETE_KEYS}
    ordered = {key: preset[key] for key in NORMALIZE_FIELD_ORDER if key in preset}
    ordered.update((key, value) for key, value in preset.items() if key not in ordered)
    text = json.dumps(ordered, indent="\t", ensure_ascii=False) + "\n"
    (PROCESS_DIR / f"{name}.json").write_text(text, encoding="utf-8", newline="\n")


def size_preset(strategy, size, height):
    name = f"Multi-nozzle {strategy} @Snapmaker U1 ({size} nozzle)"
    machine = f"Snapmaker U1 ({size} nozzle)"
    exact = f"{height}mm Standard @Snapmaker U1 ({size} nozzle)"
    preset = {"type": "process", "name": name, "from": "system", "instantiation": "true"}
    # Keep the id orca_profile_tool.py generate-id gave the preset.
    if (PROCESS_DIR / f"{name}.json").exists():
        setting_id = load(name).get("setting_id")
        if setting_id:
            preset["setting_id"] = setting_id
    if (PROCESS_DIR / f"{exact}.json").exists():
        preset["inherits"] = exact
        preset["description"] = (f"Speeds, accelerations and line widths of a {size} mm nozzle under Multi-nozzle "
                                 f"{strategy}: {exact}.")
    else:
        base, base_height = nearest_base(size, height)
        nearest = NEAREST_STANDARD[size]
        preset["inherits"] = base
        values = (f"Snapmaker's {base_height} mm values" if base_height == height else
                  f"Snapmaker's {base_height} mm values (the nearest it has to {height} mm)")
        preset["description"] = (f"Speeds, accelerations and line widths of a {size} mm nozzle under Multi-nozzle "
                                 f"{strategy}: {values} for the {size} mm nozzle, with the settings of {nearest}.")
        for key, value in load(nearest).items():
            if key not in META_KEYS:
                preset[key] = value
        # A base of another height: the layer height the strategy prints this size at.
        if base_height != height:
            preset["layer_height"] = height
    preset["compatible_printers"] = [machine]
    write(name, preset)


def add_high_flow_column(strategy, faster):
    name = f"Multi-nozzle {strategy} @Snapmaker U1"
    preset = load(name)
    parent = resolved(preset["inherits"])
    for key, high_flow in HIGH_FLOW_SPEEDS.items():
        standard = scalar(parent[key])
        preset[key] = [standard, high_flow if faster else standard]
    preset["print_extruder_id"] = ["1", "1"]
    preset["print_extruder_variant"] = ["Direct Drive Standard", "Direct Drive High Flow"]
    write(name, preset)


def main():
    for strategy, heights in STRATEGIES.items():
        for size, height in heights.items():
            size_preset(strategy, size, height)
    for strategy, faster in HIGH_FLOW_PRESETS.items():
        add_high_flow_column(strategy, faster)


if __name__ == "__main__":
    main()
