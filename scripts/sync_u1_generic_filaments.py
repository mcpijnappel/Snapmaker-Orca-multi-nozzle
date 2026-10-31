#!/usr/bin/env python3
"""Snapmaker Orca Max: the U1 Generic filament presets take the values of their Snapmaker twin.

A generic spool can be any brand, so its max volumetric speed is the twin's less 10 %. Generic PETG has
no Snapmaker twin: it takes Snapmaker PETG HF scaled by Bambu Lab's ratio of plain PETG to PETG HF
(Bambu PETG Basic 13 / PETG HF 18 on the A1), less 10 %. The 0.4 mm preset also takes the twin's High
Flow column (its temperatures, pressure advance, fans, flow), with the same 10 % on the max flow; its
Standard column keeps the generic values apart from the max flow.

A max flow never goes down on 0.4 to 0.8 mm (the generic value stays where Snapmaker's own brand is
lower); on 0.2 mm a value of 5 mm3/s or more goes down to the twin's (one value for every size, which
a 0.2 mm nozzle cannot extrude). A High Flow value is never below the Standard one.

Run from the repository root. Prints what it changes; --write writes the files.
"""

import json
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
FILAMENT_DIR = ROOT / "resources" / "profiles" / "Snapmaker" / "filament"
LIBRARY_DIR = ROOT / "resources" / "profiles" / "OrcaFilamentLibrary" / "filament"
SIZES = ["0.2", "0.4", "0.6", "0.8"]
SAFETY = 0.9
STANDARD, HIGH_FLOW = "Direct Drive Standard", "Direct Drive High Flow"
# Generic product -> (Snapmaker twin, extra factor).
TWINS = {
    "Generic PLA": ("Snapmaker PLA SnapSpeed", 1.0),
    "Generic PLA High Speed": ("Snapmaker PLA SnapSpeed", 1.0),
    "Generic PLA Silk": ("Snapmaker PLA Silk", 1.0),
    "Generic PLA-CF": ("Snapmaker PLA-CF", 1.0),
    "Generic PETG": ("Snapmaker PETG HF", 13 / 18),
    "Generic PETG HF": ("Snapmaker PETG HF", 1.0),
    "Generic PETG-CF": ("Snapmaker PETG-CF", 1.0),
    "Generic ABS": ("Snapmaker ABS", 1.0),
    "Generic ASA": ("Snapmaker ASA", 1.0),
    "Generic Support For PLA": ("Snapmaker Support For PLA", 1.0),
    "Generic TPU 95A HF": ("Snapmaker TPU 95A HF", 1.0),
    "Generic PVA": ("Snapmaker PVA", 1.0),
}


def variant_keys():
    """filament_options_with_variant of PrintConfig.cpp: the keys with one value per flow column."""
    source = (ROOT / "src" / "libslic3r" / "PrintConfig.cpp").read_text(encoding="utf-8")
    block = source[source.index("std::set<std::string> filament_options_with_variant = {"):]
    block = block[:block.index("};")]
    block = re.sub(r"//[^\n]*", "", block)
    return set(re.findall(r'"([a-z0-9_]+)"', block))


INDEX = {}
for directory in (FILAMENT_DIR, LIBRARY_DIR):
    for path in directory.rglob("*.json"):
        try:
            with open(path, encoding="utf-8-sig") as f:
                data = json.load(f)
        except (OSError, ValueError):
            continue
        if "name" in data:
            INDEX.setdefault(data["name"], (path, data))


def resolved(name):
    chain, seen = [], set()
    while name and name in INDEX and name not in seen:
        seen.add(name)
        data = INDEX[name][1]
        chain.append(data)
        name = data.get("inherits", "")
    out = {}
    for data in reversed(chain):
        out.update(data)
    return out


def preset_for(product, size):
    """The instantiated U1 preset of `product` for `size`, or None."""
    machine = f"Snapmaker U1 ({size} nozzle)"
    for name, (path, data) in INDEX.items():
        if path.parent != FILAMENT_DIR or data.get("instantiation") != "true":
            continue
        if (name == product or name.startswith(product + " @")) and machine in (data.get("compatible_printers") or []):
            return name
    return None


def as_list(value):
    return value if isinstance(value, list) else [value]


def number(value):
    text = f"{value:.2f}".rstrip("0").rstrip(".")
    return text


def main(write):
    keys = variant_keys()
    for product, (twin, factor) in TWINS.items():
        for size in SIZES:
            generic = preset_for(product, size)
            source = preset_for(twin, size)
            if generic is None or source is None:
                continue
            path, data = INDEX[generic]
            mine, theirs = resolved(generic), resolved(source)
            twin_speeds = [float(v) for v in as_list(theirs["filament_max_volumetric_speed"]) if v not in (None, "nil")]
            before = as_list(mine.get("filament_max_volumetric_speed"))[0]
            proposed, old = twin_speeds[0] * factor * SAFETY, float(before)
            if size == "0.2" and old >= 5.:
                kept = proposed
            else:
                kept = max(old, proposed)
            standard = number(kept)
            variants = as_list(theirs.get("filament_extruder_variant", [STANDARD]))
            high_flow = size == "0.4" and HIGH_FLOW in variants
            changed = {}
            if high_flow:
                column = variants.index(HIGH_FLOW)
                for key in sorted(keys):
                    their_values = theirs.get(key)
                    if their_values is None or not isinstance(their_values, list) or len(their_values) <= column:
                        continue
                    my_first = as_list(mine.get(key, their_values[0]))[0]
                    changed[key] = [my_first, their_values[column]]
                # Every other key of the variant set the preset writes takes two values too: the twin's
                # High Flow value where it has one, else the same.
                for key in sorted(keys):
                    if key in changed or key not in data:
                        continue
                    values = as_list(data[key])
                    if len(values) == 1:
                        their_values = theirs.get(key)
                        hf_value = their_values[column] if isinstance(their_values, list) and len(their_values) > column else values[0]
                        changed[key] = [values[0], hf_value]
                changed["filament_extruder_variant"] = [STANDARD, HIGH_FLOW]
                changed["filament_flow_support"] = ["standard", "high_flow"]
                hf = number(max(twin_speeds[column] * factor * SAFETY, kept))
                changed["filament_max_volumetric_speed"] = [standard, hf]
                print(f"{generic:40s} {size}: {before} -> {standard}, High Flow {hf} (+{len(changed) - 3} High Flow values of {source})")
            else:
                changed["filament_max_volumetric_speed"] = [standard]
                print(f"{generic:40s} {size}: {before} -> {standard}")
            if write:
                data.update(changed)
                text = json.dumps(data, indent=4, ensure_ascii=False) + "\n"
                path.write_text(text, encoding="utf-8", newline="\n")


if __name__ == "__main__":
    main("--write" in sys.argv)
