#!/usr/bin/env python3
"""Snapmaker Orca Max: the four per-size U1 printer presets of Snapmaker's layout, resolved through
their inherits chains, as a project of Snapmaker Orca stores the printer: one flat dict per size.

Max joined them into one printer preset "Snapmaker U1" (scripts/convert_u1_single_printer.py); the
tests load these to check that a project naming "Snapmaker U1 (<size> nozzle)" opens with that size.
Usage: make_fixtures.py <repository> <output dir> [<git ref with the per-size presets>]
"""
import json, subprocess, sys, os

REPO, OUT = sys.argv[1], sys.argv[2]
REF = sys.argv[3] if len(sys.argv) > 3 else "8342e78a"
VENDOR = "resources/profiles/Snapmaker"
META = {"type", "name", "from", "instantiation", "setting_id", "inherits", "description", "renamed_from", "version"}


def show(path):
    return subprocess.run(["git", "-C", REPO, "show", f"{REF}:{path}"], capture_output=True, text=True, check=True).stdout


index = json.loads(show(f"{VENDOR}.json"))
paths = {entry["name"]: entry["sub_path"] for entry in index["machine_list"]}


def resolve(name):
    preset = json.loads(show(f"{VENDOR}/{paths[name]}"))
    parent = preset.get("inherits", "")
    merged = resolve(parent) if parent else {}
    merged.update(preset)
    return merged


os.makedirs(OUT, exist_ok=True)
for size in ("0.2", "0.4", "0.6", "0.8"):
    name = f"Snapmaker U1 ({size} nozzle)"
    machine = {key: value for key, value in resolve(name).items() if key not in META}
    machine["printer_settings_id"] = name
    with open(os.path.join(OUT, f"printer_{size}.json"), "w", encoding="utf-8") as f:
        json.dump(machine, f, indent=1, ensure_ascii=False)
        f.write("\n")
