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

It also writes one hidden preset per nozzle size and layer height, "<h>mm Multi-nozzle @Snapmaker U1
(<size> nozzle)", for every height one grid step finer or coarser than a strategy's height of that size,
within the nozzle's minimum and maximum layer height (the U1 machine presets): the per-extruder picker
of the Process tab offers them, and a chosen one sets the head's layer height as well as its speeds and
line widths. Built like the strategy presets; a 0.4 mm height without its own Standard preset inherits
the nearest 0.4 mm Standard preset. Height presets of heights no longer offered are removed.

Run from the repository root. A new preset needs an index entry in Snapmaker.json and a setting_id
(scripts/orca_profile_tool.py generate-id --setting-id).
"""

import json
import pathlib
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
from orca_profile_tool import NORMALIZE_FIELD_ORDER, OBSOLETE_KEYS  # noqa: E402

PROCESS_DIR = pathlib.Path(__file__).resolve().parent.parent / "resources" / "profiles" / "Snapmaker" / "process"
MACHINE_DIR = PROCESS_DIR.parent / "machine"

# The strategies' shares of the nozzle diameter (Slicing.cpp multi_nozzle_strategy_fraction); the grid is
# the 0.2 mm nozzle's height (multi_nozzle_strategy_grid).
FRACTIONS = [0.25, 0.4, 0.5, 0.6, 0.7]
SIZES = ["0.2", "0.4", "0.6", "0.8"]

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


def machine_limits(size):
    """min / max layer height of the U1 machine preset of `size`, through its parents."""
    name, out = f"Snapmaker U1 ({size} nozzle)", {}
    chain = []
    while name:
        with open(MACHINE_DIR / f"{name}.json", encoding="utf-8-sig") as f:
            preset = json.load(f)
        chain.append(preset)
        name = preset.get("inherits", "")
    for preset in reversed(chain):
        out.update(preset)
    return float(scalar(out["min_layer_height"])), float(scalar(out["max_layer_height"]))


def step_heights(size):
    """The heights one grid step finer or coarser than a strategy's height of `size`, within the nozzle's
    limits, as "0.30" (Slicing.cpp multi_nozzle_strategy_height, in um)."""
    low, high = (round(v * 1000) for v in machine_limits(size))
    nozzle = round(float(size) * 1000)
    heights = set()
    for fraction in FRACTIONS:
        grid = round(fraction * 200 / 5) * 5
        steps = max(1, round(fraction * nozzle / grid))
        n_min, n_max = max(1, -(-low // grid)), high // grid
        steps = min(max(steps, n_min), n_max) if n_min <= n_max else steps
        own = steps * grid
        for height in (own - grid, own + grid):
            if low <= height <= high:
                heights.add(height)
    return [f"{h / 1000:.2f}" for h in sorted(heights)]


def standard_presets(size):
    """The heights of the Standard presets Snapmaker ships for `size`."""
    suffix = f"mm Standard @Snapmaker U1 ({size} nozzle).json"
    return sorted(path.name[:-len(suffix)] for path in PROCESS_DIR.glob(f"*{suffix}"))


def short(name):
    """A preset name without its printer ("0.12mm Standard")."""
    return name.split(" @")[0]


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
        preset["description"] = f"{size} mm nozzle, Multi-nozzle {strategy}: {short(exact)}."
    else:
        base, base_height = nearest_base(size, height)
        nearest = NEAREST_STANDARD[size]
        preset["inherits"] = base
        values = f"Snapmaker's {base_height} mm base" + ("" if base_height == height else " (nearest)")
        preset["description"] = f"{size} mm nozzle, Multi-nozzle {strategy}: {values} with {short(nearest)} settings."
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


def height_preset(size, height):
    name = f"{height}mm Multi-nozzle @Snapmaker U1 ({size} nozzle)"
    machine = f"Snapmaker U1 ({size} nozzle)"
    exact = f"{height}mm Standard @Snapmaker U1 ({size} nozzle)"
    preset = {"type": "process", "name": name, "from": "system", "instantiation": "true"}
    if (PROCESS_DIR / f"{name}.json").exists():
        setting_id = load(name).get("setting_id")
        if setting_id:
            preset["setting_id"] = setting_id
    what = f"{size} mm nozzle, {height} mm layers"
    if (PROCESS_DIR / f"{exact}.json").exists():
        preset["inherits"] = exact
        preset["description"] = f"{what}: {short(exact)}."
    elif size == "0.4":
        # The 0.4 mm Standard presets set their own speeds: the nearest one, at this height.
        nearest = min(standard_presets(size), key=lambda h: (abs(float(h) - float(height)), float(h)))
        preset["inherits"] = f"{nearest}mm Standard @Snapmaker U1 ({size} nozzle)"
        preset["description"] = f"{what}: {short(preset['inherits'])} (nearest)."
        preset["layer_height"] = height
    else:
        base, base_height = nearest_base(size, height)
        nearest = NEAREST_STANDARD[size]
        preset["inherits"] = base
        values = f"Snapmaker's {base_height} mm base" + ("" if base_height == height else " (nearest)")
        preset["description"] = f"{what}: {values} with {short(nearest)} settings."
        for key, value in load(nearest).items():
            if key not in META_KEYS:
                preset[key] = value
        if base_height != height:
            preset["layer_height"] = height
    preset["compatible_printers"] = [machine]
    # A 0.4 mm High Flow nozzle: a High Flow column where the parent has none, faster from 0.16 mm on
    # (as the strategy presets).
    if size == "0.4":
        parent = resolved(preset["inherits"])
        variants = parent.get("print_extruder_variant", [])
        if "Direct Drive High Flow" not in (variants if isinstance(variants, list) else [variants]):
            faster = float(height) >= 0.16
            for key, high_flow in HIGH_FLOW_SPEEDS.items():
                standard = scalar(parent[key])
                preset[key] = [standard, high_flow if faster else standard]
            preset["print_extruder_id"] = ["1", "1"]
            preset["print_extruder_variant"] = ["Direct Drive Standard", "Direct Drive High Flow"]
    write(name, preset)
    return name


def main():
    for strategy, heights in STRATEGIES.items():
        for size, height in heights.items():
            size_preset(strategy, size, height)
    for strategy, faster in HIGH_FLOW_PRESETS.items():
        add_high_flow_column(strategy, faster)
    written = []
    for size in SIZES:
        heights = step_heights(size)
        for height in heights:
            written.append(height_preset(size, height))
        # Height presets of heights no longer offered.
        for path in PROCESS_DIR.glob(f"*mm Multi-nozzle @Snapmaker U1 ({size} nozzle).json"):
            if path.name.split("mm Multi-nozzle")[0] not in heights:
                path.unlink()
                print(f"removed {path.stem}", file=sys.stderr)
    print("\n".join(written))


if __name__ == "__main__":
    main()
