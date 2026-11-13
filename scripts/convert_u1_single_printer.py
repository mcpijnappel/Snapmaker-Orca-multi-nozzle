#!/usr/bin/env python3
"""Snapmaker Orca Max: one size-less "Snapmaker U1" printer preset instead of one per nozzle size.

Snapmaker ships the U1 as four printer presets, "Snapmaker U1 (0.2|0.4|0.6|0.8 nozzle)", that differ
only in per-tool-head values, and pins each process and filament preset to one of them. Max has one
printer preset, "Snapmaker U1", whose tool heads each carry their own nozzle size. This script turns
Snapmaker's layout into Max's:

- machine/Snapmaker U1 printer.json: the entry preset "Snapmaker U1", the content of the 0.4 mm preset
  (the one with the High Flow columns), its setting_id kept, renamed_from the four old names so that
  projects and user presets naming them load onto it.
- machine/Snapmaker U1.json (the printer model): "nozzle_diameter" names the entry's one variant, and
  "nozzle_sizes" holds per size the values a tool head of that size takes (the keys in which the four
  presets differ), the size's default process preset, whether it has High Flow values, and the old
  printer name (what a project of Snapmaker Orca names for that size).
- Process and filament presets: "compatible_printers" names "Snapmaker U1"; a preset made for one size
  says so in "nozzle_size". A preset for every size (the multi-nozzle presets) gets no tag.
- The four per-size machine files are deleted and Snapmaker.json lists the entry.

Run it after every merge of Snapmaker's profiles; a second run changes nothing. Run from the
repository root. Prints what it changes; --write writes the files. Afterwards run
`python3 scripts/orca_profile_tool.py update-index --vendor Snapmaker` and bump the bundle version.
"""

import json
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
VENDOR_DIR = ROOT / "resources" / "profiles" / "Snapmaker"
INDEX = ROOT / "resources" / "profiles" / "Snapmaker.json"
MODEL = "Snapmaker U1"
ENTRY = "Snapmaker U1"
ENTRY_FILE = "machine/Snapmaker U1 printer.json"
SIZES = ["0.2", "0.4", "0.6", "0.8"]
ENTRY_SIZE = "0.4"
OLD_NAME = "Snapmaker U1 ({} nozzle)"
OLD_PATTERN = re.compile(r"Snapmaker U1 \((0\.[0-9]+) nozzle\)")
# Keys that are no per-head value even where the presets differ: identity, the variant layout of the
# 0.4 mm preset (its High Flow columns), and keys the presets restate with an inherited value.
NOT_HEAD_VALUES = {
    "name", "setting_id", "printer_variant", "nozzle_diameter", "default_print_profile", "renamed_from",
    "default_bed_type", "default_nozzle_volume_type", "support_multi_bed_types", "extruder_variant_list",
    "printer_extruder_id", "printer_extruder_variant", "printer_flow_support",
    # A whole-printer value; Snapmaker updates the 0.4 mm preset first, and the entry keeps its version.
    "machine_start_gcode",
}


def load(path):
    with open(path, encoding="utf-8-sig") as f:
        return json.load(f)


def dump(data):
    return json.dumps(data, indent=4, ensure_ascii=False) + "\n"


def is_machine_limit(key):
    return key.startswith("machine_max_")


def head_value(values, key, name):
    """The one value a per-size preset gives every tool head for `key`."""
    if not isinstance(values, list):
        return values
    distinct = set(values)
    if len(distinct) != 1:
        sys.exit(f"{name}: {key} differs between its columns ({values}); the size table holds one value per size")
    return values[0]


def build_table(machines):
    """The "nozzle_sizes" table of the printer model from the four per-size machine presets."""
    keys = set()
    for size, data in machines.items():
        keys |= set(data)
    differing = []
    for key in sorted(keys):
        if key in NOT_HEAD_VALUES or is_machine_limit(key):
            continue
        # The values as a set: the 0.4 mm preset writes a column per flow type, the others one per head.
        values = {size: json.dumps(sorted(set(data[key])) if isinstance(data[key], list) else data[key])
                  for size, data in machines.items() if key in data}
        if len(values) == len(machines) and len(set(values.values())) > 1:
            differing.append(key)
    table = {}
    for size, data in machines.items():
        entry = {
            "printer": data["name"],
            "default_print_profile": data.get("default_print_profile", ""),
            "high_flow": "1" if "extruder_variant_list" in data and "High Flow" in json.dumps(data["extruder_variant_list"]) else "0",
        }
        entry.update({key: head_value(data[key], key, data["name"]) for key in differing})
        table[size] = entry
    return table


def convert_machines(write):
    machine_dir = VENDOR_DIR / "machine"
    paths = {size: machine_dir / f"{OLD_NAME.format(size)}.json" for size in SIZES}
    present = {size: path for size, path in paths.items() if path.exists()}
    if not present:
        return False
    if len(present) != len(SIZES):
        sys.exit(f"only {sorted(present)} of the per-size U1 machine presets exist; expected all or none")
    machines = {size: load(path) for size, path in paths.items()}

    entry = {}
    for key, value in machines[ENTRY_SIZE].items():
        entry[key] = value
        if key == "name":
            entry["name"] = ENTRY
            entry["renamed_from"] = ";".join(OLD_NAME.format(size) for size in SIZES)
    entry_path = VENDOR_DIR / ENTRY_FILE

    model_path = machine_dir / f"{MODEL}.json"
    model = load(model_path)
    model["nozzle_diameter"] = ENTRY_SIZE
    model["nozzle_sizes"] = build_table(machines)

    print(f"write {entry_path.relative_to(ROOT)} (from {paths[ENTRY_SIZE].name})")
    print(f"write {model_path.relative_to(ROOT)}: nozzle_sizes {', '.join(model['nozzle_sizes'])}, "
          f"head keys {', '.join(k for k in model['nozzle_sizes'][ENTRY_SIZE] if k not in ('printer', 'default_print_profile', 'high_flow'))}")
    for path in paths.values():
        print(f"delete {path.relative_to(ROOT)}")
    if write:
        entry_path.write_text(dump(entry), encoding="utf-8")
        model_path.write_text(dump(model), encoding="utf-8")
        for path in paths.values():
            path.unlink()
    return True


LIST_PATTERN = re.compile(r'(?P<indent>[ \t]*)"compatible_printers":\s*\[(?P<items>[^\]]*)\](?P<comma>,?)')


def convert_preset(path, write):
    text = path.read_text(encoding="utf-8-sig")
    match = LIST_PATTERN.search(text)
    if match is None:
        return False
    items = json.loads("[" + match.group("items") + "]")
    sizes = {OLD_PATTERN.fullmatch(item).group(1) for item in items if OLD_PATTERN.fullmatch(item)}
    if not sizes:
        return False
    others = [item for item in items if not OLD_PATTERN.fullmatch(item) and item != ENTRY]
    if others:
        sys.exit(f"{path.name}: names other printers besides the U1 ({others}); convert it by hand")
    data = json.loads(text)
    if "nozzle_size" in data:
        sys.exit(f"{path.name}: already has a nozzle_size and still names a per-size U1 printer")
    indent = match.group("indent")
    inner = indent + ("\t" if "\t" in indent else "    ")
    replacement = f'{indent}"compatible_printers": [\n{inner}"{ENTRY}"\n{indent}]'
    if len(sizes) == 1:
        replacement += f',\n{indent}"nozzle_size": "{next(iter(sizes))}"'
    replacement += match.group("comma")
    new_text = text[:match.start()] + replacement + text[match.end():]
    json.loads(new_text)    # still valid
    tag = next(iter(sizes)) if len(sizes) == 1 else "any size"
    print(f"preset {path.relative_to(VENDOR_DIR)}: {tag}")
    if write:
        path.write_text(new_text, encoding="utf-8")
    return True


def index_block(name):
    """An entry of the index naming `name`, with the separator in front of it."""
    return re.compile(r'(?P<sep>,\s*)?\{\s*"name":\s*"' + re.escape(name) + r'",\s*"sub_path":\s*"[^"]*"\s*\}')


def convert_index(write):
    """Edits the index in place, so that its order and layout stay Snapmaker's: the entry takes the
    place of the 0.4 mm preset, the other sizes go."""
    text = INDEX.read_text(encoding="utf-8-sig")
    original = text
    entry_listed = '"sub_path": "' + ENTRY_FILE + '"' in text
    for size in [ENTRY_SIZE] + [size for size in SIZES if size != ENTRY_SIZE]:
        match = index_block(OLD_NAME.format(size)).search(text)
        if match is None:
            continue
        block = text[match.start("sep") if match.group("sep") else match.start():match.end()]
        if not entry_listed:
            replacement = re.sub(r'"name":\s*"[^"]*"', f'"name": "{ENTRY}"', block)
            replacement = re.sub(r'"sub_path":\s*"[^"]*"', f'"sub_path": "{ENTRY_FILE}"', replacement)
            text = text.replace(block, replacement, 1)
            entry_listed = True
        elif match.group("sep"):
            text = text[:match.start()] + text[match.end():]
        else:
            # The first entry of its list: the separator after it goes.
            after = re.compile(r"\s*,", re.S).match(text, match.end())
            text = text[:match.start()] + text[after.end() if after else match.end():].lstrip()
    if not entry_listed:
        sys.exit(f"{INDEX.name}: no per-size U1 machine entry to replace and no entry for {ENTRY}; add it by hand")
    json.loads(text)    # still valid
    if text == original:
        return False
    print(f"index {INDEX.relative_to(ROOT)}: machine_list lists {ENTRY} instead of the per-size presets")
    if write:
        INDEX.write_text(text, encoding="utf-8")
    return True


def main():
    write = "--write" in sys.argv[1:]
    changed = convert_machines(write)
    for kind in ("process", "filament"):
        for path in sorted((VENDOR_DIR / kind).glob("*.json")):
            changed = convert_preset(path, write) or changed
    changed = convert_index(write) or changed
    if not changed:
        print("nothing to convert")
    elif not write:
        print("dry run; --write writes the files")


if __name__ == "__main__":
    main()
