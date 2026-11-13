# Snapmaker U1 filament and process presets: inventory

- **Date:** 2026-11-13.
- **Taken at commit:** `62553c9d` (`git rev-parse --short HEAD` when the files were read; branch `snorca-max`). HEAD is now `8b48a55b`, which only added `doc/snorca-max-todo.md`. The preset files, scripts and C++ named here are unchanged between the two.
- **Compared against:** the clone `~/Projects/snapmaker/snapmaker-profiles` (HEAD `ba75baf`, bundle version `02.03.08.00`). Called "theirs" below. This repo is "ours".
- **Read versus run:** every number marked (script) was computed by a Python script that loads the JSON preset files and resolves `inherits`. Items marked (read) were read in the code or files by eye. Items marked (inferred) are my deduction, not read directly. The command-line refusal in 3.5 was **read, not run**. `orca_profile_tool.py normalize` and `update-index` were **not** run. Nothing in the repo was changed by the inventory work except adding this file and its CSV folder.
- **Purpose:** facts for a later design session. No design proposals are in this file.
- **Companion CSV files:** `doc/u1-preset-inventory/*.csv`, listed in section 6. The tables in this file are summaries; the CSV files hold the per-preset detail.

## How to read the tables

- "Standard column" means the first value of a variant vector (`Direct Drive Standard`). "High Flow" or "second column" means the second value.
- A preset has one or two **columns**. Two columns means its `filament_extruder_variant` (filaments) or `print_extruder_variant` (process) has two entries. Keys in `filament_options_with_variant` (filaments) or the process variant key set then hold two values.
- Sizes are nozzle diameters in mm: 0.2, 0.4, 0.6, 0.8.
- Process line widths are stored as a percent of the nozzle diameter. The tables show the stored text. Section 2.3 converts to mm.
- `S` = Standard, `HQ` = High Quality, `CM` = Color Mixing.
- Blank or `·` in a table = key absent in that preset after inheritance.

---

# 1. Filaments (`resources/profiles/Snapmaker/filament`)

## 1.1 Families and sizes (script)

- 129 presets in 43 families. Every one is `instantiation: true`, has `compatible_printers: ["Snapmaker U1"]` and a `nozzle_size` tag (0.2, 0.4, 0.6 or 0.8). Each inherits `<family> @U1 base`, a base with 5 keys and no tag.
- Presets per size: 0.2 = 19, 0.4 = 43, 0.6 = 34, 0.8 = 33.
- 34 families have more than one size. 9 are 0.4-only.
- Families that have all four sizes (18): Generic ABS, ASA, PETG, PETG HF, PLA, PLA High Speed, PLA Silk, Support For PLA; Snapmaker ABS, ASA, PETG HF, PETG Translucent, PLA Matte, PLA Rainbow, PLA Silk, PLA SnapSpeed, PLA Translucent, Support For PLA.
- 15 families lack 0.2: Generic PC, PETG-CF, PETG-GF, PLA-CF, TPU, TPU 95A HF; Snapmaker PEBA 90A, PETG-CF, PLA Glow, PLA Wood, PLA-CF, PVA, TPU, TPU 90A, TPU 95A HF.
- 1 family lacks 0.8: Snapmaker PLA Full Spectrum (0.2, 0.4, 0.6).
- 9 families are 0.4-only: Generic BVOH, PA, PA-CF, PCTG, PVA; Polymaker General, Silk, Tough PLA Family; Snapmaker PLA Basic.
- Naming (read): the 0.4 preset of most families has no size in its name (`Generic PLA`, `Snapmaker PLA Matte @U1`). Other sizes end `… @U1 0.6 nozzle`. A few 0.4 presets are named `… @U1 0.4 nozzle` (for example Generic ASA). The full preset-name list per family and size is in `filaments_ours_presets.csv`.
- `setting_id` / `filament_id` differ by size in 12 families (for example Snapmaker PETG HF: `GFSG96_01`, `_25`, `_00`; PLA SnapSpeed `1195313935011`, `…16`, `…18`). The same id is shared across families in 10 groups (for example `GFSR99` in four TPU families; `GFSG50` in Generic PETG-CF and PETG-GF; `1195313935011` in four PLA families).

### Table: sizes present, and number of columns of the 0.4 preset (ours)

| family | 0.2 | 0.4 | 0.6 | 0.8 | 0.4 columns |
|---|---|---|---|---|---|
| Generic ABS | x | x | x | x | 2 |
| Generic ASA | x | x | x | x | 2 |
| Generic BVOH |  | x |  |  | 1 |
| Generic PA |  | x |  |  | 1 |
| Generic PA-CF |  | x |  |  | 1 |
| Generic PC |  | x | x | x | 1 |
| Generic PCTG |  | x |  |  | 1 |
| Generic PETG | x | x | x | x | 2 |
| Generic PETG HF | x | x | x | x | 2 |
| Generic PETG-CF |  | x | x | x | 2 |
| Generic PETG-GF |  | x | x | x | 1 |
| Generic PLA | x | x | x | x | 2 |
| Generic PLA High Speed | x | x | x | x | 2 |
| Generic PLA Silk | x | x | x | x | 2 |
| Generic PLA-CF |  | x | x | x | 2 |
| Generic PVA |  | x |  |  | 2 |
| Generic Support For PLA | x | x | x | x | 2 |
| Generic TPU |  | x | x | x | 1 |
| Generic TPU 95A HF |  | x | x | x | 1 |
| Polymaker General PLA Family |  | x |  |  | 1 |
| Polymaker Silk PLA Family |  | x |  |  | 1 |
| Polymaker Tough PLA Family |  | x |  |  | 1 |
| Snapmaker ABS | x | x | x | x | 2 |
| Snapmaker ASA | x | x | x | x | 2 |
| Snapmaker PEBA 90A |  | x | x | x | 1 |
| Snapmaker PETG HF | x | x | x | x | 2 |
| Snapmaker PETG Translucent | x | x | x | x | 2 |
| Snapmaker PETG-CF |  | x | x | x | 2 |
| Snapmaker PLA Basic |  | x |  |  | 1 |
| Snapmaker PLA Full Spectrum | x | x | x |  | 2 |
| Snapmaker PLA Glow |  | x | x | x | 2 |
| Snapmaker PLA Matte | x | x | x | x | 2 |
| Snapmaker PLA Rainbow | x | x | x | x | 1 |
| Snapmaker PLA Silk | x | x | x | x | 2 |
| Snapmaker PLA SnapSpeed | x | x | x | x | 2 |
| Snapmaker PLA Translucent | x | x | x | x | 2 |
| Snapmaker PLA Wood |  | x | x | x | 2 |
| Snapmaker PLA-CF |  | x | x | x | 2 |
| Snapmaker PVA |  | x | x | x | 2 |
| Snapmaker Support For PLA | x | x | x | x | 2 |
| Snapmaker TPU |  | x | x | x | 1 |
| Snapmaker TPU 90A |  | x | x | x | 1 |
| Snapmaker TPU 95A HF |  | x | x | x | 1 |

### How the 12 synced Generic families are written (read)

`scripts/sync_u1_generic_filaments.py` (154 lines) writes the max volumetric speed of Generic presets from a Snapmaker "twin":

| Generic | Twin |
|---|---|
| PLA, PLA High Speed | Snapmaker PLA SnapSpeed |
| PLA Silk | Snapmaker PLA Silk |
| PLA-CF | Snapmaker PLA-CF |
| PETG | Snapmaker PETG HF, multiplied by 13/18 |
| PETG HF | Snapmaker PETG HF |
| PETG-CF | Snapmaker PETG-CF |
| ABS, ASA | Snapmaker ABS, ASA |
| Support For PLA | Snapmaker Support For PLA |
| TPU 95A HF | Snapmaker TPU 95A HF |
| PVA | Snapmaker PVA |

Rules in the script: value = twin's value x 0.9. On 0.4–0.8 the value never goes down. On 0.2 a current value of 5 or more goes down to the twin's. The 0.4 preset also gets the twin's High Flow column, with High Flow at least Standard. It reads the key list `filament_options_with_variant` from `src/libslic3r/PrintConfig.cpp`, the Snapmaker filament dir and OrcaFilamentLibrary. `--write` writes; without it, it only reports.

## 1.2 Keys that differ between sizes (script)

Comparison is on the Standard column, after resolving `inherits`, over the 34 multi-size families.

- 155 keys exist in the union. 61 differ in value in at least one family. 94 never differ. No key differs in all 34 families.
- 11 Generic families have 0.2/0.6/0.8 presets with about 46 keys the 0.4 preset lacks (`filament_dev_ams_drying_*`, `filament_scarf_*`, `hole_coef_*`, `counter_coef_*`, ramming keys). Where present the value is the same; these are presence differences, not value differences. They are listed with `kind = presence only` in `filaments_ours_size_diffs.csv`.
- Differing keys per family: Generic PC 0 (identical at 0.4/0.6/0.8), Generic ABS and ASA 1, Support For PLA 3, Generic PLA 6, Snapmaker PLA Matte 21, Snapmaker PLA SnapSpeed 27 (highest).

### Table: how many of the 34 families differ in each key (value differences)

| Families that differ | Keys |
|---|---|
| 31 | `filament_max_volumetric_speed` |
| 24 | `filament_multitool_ramming_flow` |
| 23 | `supertack_plate_temp`, `supertack_plate_temp_initial_layer` |
| 22 | `filament_multitool_ramming_volume` |
| 20 | `pressure_advance` |
| 18 | `filament_retraction_length` |
| 15 | `filament_flow_ratio` |
| 14 | `filament_retract_length_toolchange` |
| 13 | `filament_cost`, `nozzle_temperature` |
| 12 | `filament_end_gcode` |
| 10 | `nozzle_temperature_initial_layer`, `filament_minimal_purge_on_wipe_tower`, `filament_wipe_distance` |
| 9 | `fan_min_speed`, `slow_down_layer_time`, `enable_pressure_advance` |
| 8 | `fan_max_speed`, `filament_density` |
| 7 | `additional_cooling_fan_speed`, `filament_z_hop_types` |
| 6 | `filament_retraction_speed` |
| 5 | `eng_plate_temp`, `eng_plate_temp_initial_layer`, `filament_retraction_minimum_travel`, `filament_wipe` |
| 4 | `nozzle_temperature_range_low`, `filament_start_gcode`, `fan_cooling_layer_time`, `filament_z_hop`, `filament_deretraction_speed` |
| 3 | `overhang_fan_threshold`, `filament_loading_speed` |
| 1–2 | 27 further keys (full list: `filaments_ours_key_counts.csv`) |

`filament_max_volumetric_speed` by size pattern (script): 0.2 differs alone while 0.4 = 0.6 = 0.8 in 11 families; 0.4 differs alone while 0.6 = 0.8 in 8 families; all four differ in 2. The 0.2 values are 0.45–2 mm³/s; 0.4–0.8 values are about 9–22 (3.2–16 for TPU/PEBA).

### Table: values by size for the six most common keys (ours)

Rows: multi-size families only. The "0.4 High Flow" column is the second column of the 0.4 preset where it has one; blank means no second column. Values are the stored text (`·` absent).

#### `filament_max_volumetric_speed` (mm³/s)

| family | 0.2 | 0.4 | 0.4 High Flow | 0.6 | 0.8 |
|---|---|---|---|---|---|
| Generic ABS | 1.8 | 15 | 19.8 | 16.2 | 18 |
| Generic ASA | 1.8 | 13.5 | 19.8 | 16.2 | 16.2 |
| Generic PC |  | 16 |  | 16 | 16 |
| Generic PETG | 1.3 | 13 | 19.5 | 13 | 13 |
| Generic PETG HF | 1.8 | 18 | 27 | 18 | 18 |
| Generic PETG-CF |  | 11 | 16.2 | 12.6 | 12.6 |
| Generic PETG-GF |  | 10 |  | 11.5 | 11.5 |
| Generic PLA | 1.8 | 18 | 36 | 18 | 18 |
| Generic PLA High Speed | 2 | 18 | 36 | 18 | 18 |
| Generic PLA Silk | 2 | 9 | 10.8 | 9 | 9 |
| Generic PLA-CF |  | 13.5 | 18 | 16.2 | 16.2 |
| Generic Support For PLA | 0.45 | 12 | 16.2 | 12 | 12 |
| Generic TPU |  | 3.2 |  | 3.2 | 3.2 |
| Generic TPU 95A HF |  | 10.5 |  | 10.8 | 14.4 |
| Snapmaker ABS | 2 | 15 | 22 | 18 | 20 |
| Snapmaker ASA | 2 | 15 | 22 | 18 | 18 |
| Snapmaker PEBA 90A |  | 5 |  | 15 | 15 |
| Snapmaker PETG HF | 2 | 20 | 30 | 20 | 20 |
| Snapmaker PETG Translucent | 1 | 10 | 10 | 16 | 16 |
| Snapmaker PETG-CF |  | 12 | 18 | 14 | 14 |
| Snapmaker PLA Full Spectrum | 2 | 15 | 15 | 15 |  |
| Snapmaker PLA Glow |  | 16 | 18 | 18 | 18 |
| Snapmaker PLA Matte | 2 | 22 | 40 | 20 | 20 |
| Snapmaker PLA Rainbow | · | 16 |  | 22 | 22 |
| Snapmaker PLA Silk | 2 | 10 | 12 | 10 | 10 |
| Snapmaker PLA SnapSpeed | 2 | 20 | 40 | 20 | 20 |
| Snapmaker PLA Translucent | 1.6 | 12 | 12 | 12 | 12 |
| Snapmaker PLA Wood |  | 18 | 20 | 18 | 20 |
| Snapmaker PLA-CF |  | 15 | 20 | 18 | 18 |
| Snapmaker PVA |  | 5 | 6 | 6 | 6 |
| Snapmaker Support For PLA | 0.5 | 12 | 18 | 12 | 12 |
| Snapmaker TPU |  | 10.5 |  | 10.5 | 10.5 |
| Snapmaker TPU 90A |  | 3.2 |  | 3.2 | 3.5 |
| Snapmaker TPU 95A HF |  | 9 |  | 12 | 16 |

#### `filament_multitool_ramming_flow`

| family | 0.2 | 0.4 | 0.4 High Flow | 0.6 | 0.8 |
|---|---|---|---|---|---|
| Generic ABS | 15 | 15 | 30 | 15 | 15 |
| Generic ASA | 12 | 12 | 20 | 12 | 12 |
| Generic PC |  | 16 |  | 16 | 16 |
| Generic PETG | 2 | 15 | 30 | 18 | 18 |
| Generic PETG HF | 2 | 20 | 30 | 20 | 20 |
| Generic PETG-CF |  | 13 | 13 | 16 | 16 |
| Generic PETG-GF |  | 10 |  | 16 | 16 |
| Generic PLA | 2 | 20 | 40 | 18 | 18 |
| Generic PLA High Speed | 3 | 25 | 40 | 25 | 25 |
| Generic PLA Silk | 5 | 10 | 25 | 11.5 | 11 |
| Generic PLA-CF |  | 20 | 30 | 18 | 18 |
| Generic Support For PLA | 15 | 15 | 20 | 15 | 15 |
| Generic TPU |  | 3.2 |  | 3.2 | 3.2 |
| Generic TPU 95A HF |  | 10 |  | 10 | 15 |
| Snapmaker ABS | 2 | 20 | 30 | 20 | 20 |
| Snapmaker ASA | 5 | 20 | 20 | 20 | 20 |
| Snapmaker PEBA 90A |  | · |  | 3.2 | 3.2 |
| Snapmaker PETG HF | 3 | 20 | 30 | 20 | 20 |
| Snapmaker PETG Translucent | 5 | 20 | 20 | 20 | 30 |
| Snapmaker PETG-CF |  | 10 | 13 | 16 | 16 |
| Snapmaker PLA Full Spectrum | 30 | 25 | 25 | 30 |  |
| Snapmaker PLA Glow |  | 20 | 20 | 18 | 18 |
| Snapmaker PLA Matte | 4 | 40 | 40 | 40 | 40 |
| Snapmaker PLA Rainbow | 2 | 30 |  | 18 | 18 |
| Snapmaker PLA Silk | 3 | 25 | 25 | 40 | 40 |
| Snapmaker PLA SnapSpeed | 30 | 30 | 40 | 30 | 30 |
| Snapmaker PLA Translucent | 5 | 20 | 20 | 20 | 20 |
| Snapmaker PLA Wood |  | 40 | 40 | 40 | 40 |
| Snapmaker PLA-CF |  | 20 | 30 | 25 | 25 |
| Snapmaker PVA |  | 10 | 10 | 20 | 20 |
| Snapmaker Support For PLA | 5 | 20 | 20 | 20 | 20 |
| Snapmaker TPU |  | · |  | · | 15 |
| Snapmaker TPU 90A |  | 5 |  | 5 | 5 |
| Snapmaker TPU 95A HF |  | 15 |  | 10 | 15 |

#### `pressure_advance`

| family | 0.2 | 0.4 | 0.4 High Flow | 0.6 | 0.8 |
|---|---|---|---|---|---|
| Generic ABS | 0.02 | 0.02 | 0.02 | 0.02 | 0.02 |
| Generic ASA | 0.02 | 0.02 | 0.02 | 0.02 | 0.02 |
| Generic PC |  | 0.02 |  | 0.02 | 0.02 |
| Generic PETG | 0.02 | 0.04 | 0.035 | 0.02 | 0.02 |
| Generic PETG HF | 0.02 | 0.04 | 0.035 | 0.02 | 0.02 |
| Generic PETG-CF |  | 0.02 | 0.028 | 0.02 | 0.02 |
| Generic PETG-GF |  | 0.03 |  | 0.02 | 0.02 |
| Generic PLA | 0.02 | 0.02 | 0.024 | 0.02 | 0.02 |
| Generic PLA High Speed | 0.02 | 0.02 | 0.024 | 0.02 | 0.02 |
| Generic PLA Silk | 0.02 | 0.02 | 0.015 | 0.02 | 0.02 |
| Generic PLA-CF |  | 0.02 | 0.02 | 0.02 | 0.02 |
| Generic Support For PLA | 0.03 | 0.03 | 0.03 | 0.03 | 0.03 |
| Generic TPU |  | 0.4 |  | 0.02 | 0.02 |
| Generic TPU 95A HF |  | 0.15 |  | 0.02 | 0.02 |
| Snapmaker ABS | 0.1 | 0.02 | 0.02 | 0.01 | 0.01 |
| Snapmaker ASA | 0.15 | 0.02 | 0.02 | 0.015 | 0.015 |
| Snapmaker PEBA 90A |  | 0.4 |  | 0.11 | 0.08 |
| Snapmaker PETG HF | 0.02 | 0.02 | 0.035 | 0.02 | 0.02 |
| Snapmaker PETG Translucent | 0.23 | 0.04 | 0.05 | 0.02 | 0.02 |
| Snapmaker PETG-CF |  | 0.02 | 0.028 | 0.02 | 0.02 |
| Snapmaker PLA Full Spectrum | 0.2 | 0.02 | 0.024 | 0.02 |  |
| Snapmaker PLA Glow |  | 0.02 | 0.023 | 0.02 | 0.01 |
| Snapmaker PLA Matte | 0.025 | 0.02 | 0.024 | 0.015 | 0.015 |
| Snapmaker PLA Rainbow | 0.156 | 0.012 |  | 0.01 | 0.01 |
| Snapmaker PLA Silk | 0.015 | 0.015 | 0.015 | 0.015 | 0.015 |
| Snapmaker PLA SnapSpeed | 0.2 | 0.02 | 0.024 | 0.02 | 0.018 |
| Snapmaker PLA Translucent | 0.15 | 0.02 | 0.024 | 0.02 | 0.02 |
| Snapmaker PLA Wood |  | 0.025 | 0.024 | 0.014 | 0.007 |
| Snapmaker PLA-CF |  | 0.01 | 0.02 | 0.015 | 0.015 |
| Snapmaker PVA |  | 0.03 | 0.03 | 0.03 | 0.03 |
| Snapmaker Support For PLA | 0.2 | 0.03 | 0.03 | 0.02 | 0.015 |
| Snapmaker TPU |  | 0.15 |  | · | · |
| Snapmaker TPU 90A |  | 0.4 |  | 0.02 | 0.02 |
| Snapmaker TPU 95A HF |  | 0.23 |  | 0.14 | 0.12 |

#### `filament_flow_ratio`

| family | 0.2 | 0.4 | 0.4 High Flow | 0.6 | 0.8 |
|---|---|---|---|---|---|
| Generic ABS | 0.95 | 0.95 | 0.95 | 0.95 | 0.95 |
| Generic ASA | 0.95 | 0.95 | 0.94 | 0.95 | 0.95 |
| Generic PC |  | 0.94 |  | 0.94 | 0.94 |
| Generic PETG | 0.95 | 0.95 | 0.955 | 0.95 | 0.95 |
| Generic PETG HF | 0.95 | 0.95 | 0.955 | 0.95 | 0.95 |
| Generic PETG-CF |  | 0.95 | 1.01 | 0.95 | 0.95 |
| Generic PETG-GF |  | 0.95 |  | 0.95 | 0.95 |
| Generic PLA | 0.98 | 0.98 | 0.966 | 0.98 | 0.98 |
| Generic PLA High Speed | 0.98 | 0.98 | 0.966 | 0.98 | 0.98 |
| Generic PLA Silk | 0.98 | 0.98 | 0.98 | 0.98 | 0.98 |
| Generic PLA-CF |  | 0.95 | 0.98 | 0.95 | 0.95 |
| Generic Support For PLA | 1 | 1 | 0.95 | 1 | 1 |
| Generic TPU |  | 1 |  | 1 | 1 |
| Generic TPU 95A HF |  | 1 |  | 1 | 1 |
| Snapmaker ABS | 0.975 | 0.95 | 0.95 | 0.95 | 0.95 |
| Snapmaker ASA | 0.96 | 0.95 | 0.94 | 0.95 | 0.95 |
| Snapmaker PEBA 90A |  | 1.045 |  | 1.045 | 1.045 |
| Snapmaker PETG HF | 0.95 | 0.95 | 0.955 | 0.95 | 0.95 |
| Snapmaker PETG Translucent | 0.98 | 0.97 | 0.97 | 0.98 | 0.97 |
| Snapmaker PETG-CF |  | 0.95 | 1.01 | 0.95 | 0.95 |
| Snapmaker PLA Full Spectrum | 1.01 | 0.98 | 0.98 | 0.97 |  |
| Snapmaker PLA Glow |  | 0.97 | 0.98 | 0.98 | 0.98 |
| Snapmaker PLA Matte | 1.05 | 1 | 0.99 | 0.99 | 0.98 |
| Snapmaker PLA Rainbow | 0.966 | 0.96 |  | 0.95 | 0.97 |
| Snapmaker PLA Silk | 1.02 | 0.98 | 0.98 | 0.97 | 0.97 |
| Snapmaker PLA SnapSpeed | 1.01 | 0.966 | 0.966 | 0.97 | 0.98 |
| Snapmaker PLA Translucent | 1.02 | 0.97 | 0.97 | 0.98 | 0.95 |
| Snapmaker PLA Wood |  | 0.97 | 0.97 | 0.98 | 0.965 |
| Snapmaker PLA-CF |  | 0.98 | 0.98 | 0.98 | 0.98 |
| Snapmaker PVA |  | 1 | 0.97 | 0.98 | 0.96 |
| Snapmaker Support For PLA | 1 | 1 | 0.95 | 0.95 | 0.93 |
| Snapmaker TPU |  | · |  | · | · |
| Snapmaker TPU 90A |  | 1.045 |  | 1.093 | 1.095 |
| Snapmaker TPU 95A HF |  | 1.067 |  | 1.067 | 1.045 |

#### `nozzle_temperature`

| family | 0.2 | 0.4 | 0.4 High Flow | 0.6 | 0.8 |
|---|---|---|---|---|---|
| Generic ABS | 270 | 270 | 280 | 270 | 270 |
| Generic ASA | 260 | 260 | 280 | 260 | 260 |
| Generic PC |  | 280 |  | 280 | 280 |
| Generic PETG | 255 | 255 | 245 | 255 | 255 |
| Generic PETG HF | 220 | 230 | 245 | 220 | 220 |
| Generic PETG-CF |  | 255 | 260 | 255 | 255 |
| Generic PETG-GF |  | 255 |  | 255 | 255 |
| Generic PLA | 220 | 220 | 220 | 220 | 220 |
| Generic PLA High Speed | 220 | 220 | 220 | 220 | 220 |
| Generic PLA Silk | 220 | 220 | 230 | 220 | 220 |
| Generic PLA-CF |  | 220 | 230 | 220 | 220 |
| Generic Support For PLA | 230 | 230 | 210 | 230 | 230 |
| Generic TPU |  | 240 |  | 220 | 215 |
| Generic TPU 95A HF |  | 235 |  | 220 | 220 |
| Snapmaker ABS | 270 | 265 | 280 | 265 | 265 |
| Snapmaker ASA | 270 | 270 | 280 | 270 | 270 |
| Snapmaker PEBA 90A |  | 235 |  | 240 | 240 |
| Snapmaker PETG HF | 245 | 245 | 245 | 245 | 245 |
| Snapmaker PETG Translucent | 245 | 250 | 250 | 245 | 245 |
| Snapmaker PETG-CF |  | 255 | 260 | 255 | 255 |
| Snapmaker PLA Full Spectrum | 220 | 220 | 220 | 220 |  |
| Snapmaker PLA Glow |  | 220 | 220 | 220 | 220 |
| Snapmaker PLA Matte | 215 | 215 | 220 | 215 | 215 |
| Snapmaker PLA Rainbow | 230 | 230 |  | 230 | 230 |
| Snapmaker PLA Silk | 230 | 230 | 230 | 230 | 230 |
| Snapmaker PLA SnapSpeed | 220 | 220 | 220 | 220 | 215 |
| Snapmaker PLA Translucent | 220 | 220 | 220 | 220 | 220 |
| Snapmaker PLA Wood |  | 220 | 220 | 220 | 220 |
| Snapmaker PLA-CF |  | 240 | 230 | 220 | 220 |
| Snapmaker PVA |  | 240 | 240 | 230 | 225 |
| Snapmaker Support For PLA | 240 | 210 | 210 | 210 | 210 |
| Snapmaker TPU |  | 235 |  | 220 | · |
| Snapmaker TPU 90A |  | 230 |  | 220 | 220 |
| Snapmaker TPU 95A HF |  | 230 |  | 215 | 215 |

#### `filament_retraction_length`

| family | 0.2 | 0.4 | 0.4 High Flow | 0.6 | 0.8 |
|---|---|---|---|---|---|
| Generic ABS | nil | nil | 0.2 | nil | nil |
| Generic ASA | nil | nil | 0.6 | nil | nil |
| Generic PC |  | nil |  | nil | nil |
| Generic PETG | nil | nil | 0.8 | nil | nil |
| Generic PETG HF | nil | nil | 0.8 | 0.4 | 0.4 |
| Generic PETG-CF |  | nil | 0.6 | nil | nil |
| Generic PETG-GF |  | nil |  | nil | nil |
| Generic PLA | nil | nil | 1.2 | nil | nil |
| Generic PLA High Speed | nil | nil | 1.2 | nil | nil |
| Generic PLA Silk | 0.5 | 0.5 | 0.2 | 0.5 | 0.5 |
| Generic PLA-CF |  | nil | 0.8 | nil | nil |
| Generic Support For PLA | nil | nil | 1 | nil | nil |
| Generic TPU |  | 1.5 |  | 1.5 | 1.5 |
| Generic TPU 95A HF |  | 1.5 |  | 1.5 | 1.5 |
| Snapmaker ABS | 0.4 | 0.4 | 0.2 | 0.4 | 0.4 |
| Snapmaker ASA | 0.4 | 0.2 | 0.6 | 1.5 | 0.4 |
| Snapmaker PEBA 90A |  | 1 |  | 0.6 | 0.8 |
| Snapmaker PETG HF | 1.5 | nil | 0.8 | 2.5 | 1 |
| Snapmaker PETG Translucent | 1 | 1 | 1 | 1.2 | 1 |
| Snapmaker PETG-CF |  | nil | 0.6 | nil | nil |
| Snapmaker PLA Full Spectrum | nil | nil | 0.5 | 1.6 |  |
| Snapmaker PLA Glow |  | 0.4 | 0.4 | 0.8 | 0.8 |
| Snapmaker PLA Matte | nil | nil | 1 | 0.6 | 0.6 |
| Snapmaker PLA Rainbow | 1.5 | 0.5 |  | 0.5 | 0.8 |
| Snapmaker PLA Silk | 0.4 | 0.2 | 0.2 | 0.4 | 0.4 |
| Snapmaker PLA SnapSpeed | nil | 1.2 | 1.2 | 1.6 | 1.6 |
| Snapmaker PLA Translucent | 0.2 | 0.2 | 0.2 | 0.5 | 1 |
| Snapmaker PLA Wood |  | 0.6 | 0.6 | 0.4 | 0.4 |
| Snapmaker PLA-CF |  | 1 | 0.8 | 2 | 3 |
| Snapmaker PVA |  | 0.4 | 0.6 | nil | nil |
| Snapmaker Support For PLA | nil | 1 | 1 | nil | 3 |
| Snapmaker TPU |  | · |  | 1.5 | · |
| Snapmaker TPU 90A |  | 1.2 |  | 0.6 | 1.2 |
| Snapmaker TPU 95A HF |  | 1.5 |  | 0.8 | 1 |

The remaining 55 differing keys per family and size are in `filaments_ours_size_diffs.csv` (long format: family, key, kind, size, preset, value).

## 1.3 High Flow columns (script)

- 26 presets have two columns. All are 0.4. The other 103 presets have one column.
- Markers on those 26: `filament_extruder_variant = ["Direct Drive Standard","Direct Drive High Flow"]` and `filament_flow_support = ["standard","high_flow"]`. A preset without High Flow has `filament_flow_support = ["standard"]`. 27 one-column presets also carry `filament_extruder_variant = ["Direct Drive Standard"]` (11 on 0.6, 11 on 0.8, 5 on 0.2).
- `filament_flow_support` is a Snapmaker Orca 2.4 key kept as a marker; code treats it as legacy (3.1).
- The 26: 11 Generic (ABS, ASA, PETG, PETG HF, PETG-CF, PLA, PLA High Speed, PLA Silk, PLA-CF, PVA, Support For PLA) and 15 Snapmaker (ABS, ASA, PETG HF, PETG Translucent, PETG-CF, PLA Full Spectrum, PLA Glow, PLA Matte, PLA Silk, PLA SnapSpeed, PLA Translucent, PLA Wood, PLA-CF, PVA, Support For PLA). The exact list is `filaments_ours_presets.csv` (column `columns` = 2).
- The 17 0.4 presets without High Flow: Generic BVOH, PA, PA-CF, PC, PCTG, PETG-GF, TPU, TPU 95A HF; Polymaker General, Silk, Tough PLA Family; Snapmaker PEBA 90A, PLA Basic, PLA Rainbow, TPU, TPU 90A, TPU 95A HF. Every 0.2, 0.6 and 0.8 preset has one column.
- Each two-column preset holds 11–56 keys with two values. No preset has two identical columns.

### Keys whose High Flow value differs from Standard (count of the 26 presets)

| Presets | Keys |
|---|---|
| 22 | `filament_max_volumetric_speed` |
| 19 | `filament_retraction_length` |
| 17 | `pressure_advance` |
| 16 | `filament_flow_ratio` |
| 15 | `filament_multitool_ramming_flow`, `filament_retract_length_toolchange` |
| 14 | `filament_z_hop_types` |
| 13 | `enable_pressure_advance`, `nozzle_temperature` |
| 12 | `nozzle_temperature_initial_layer` |
| 10 | `filament_multitool_ramming_volume` |
| 9 | `additional_cooling_fan_speed` |
| 8 | `filament_minimal_purge_on_wipe_tower` |
| 7 | `fan_max_speed` |
| 4 or fewer | `filament_wipe_distance`, `fan_min_speed`, `filament_retraction_speed`, `filament_deretraction_speed`, `filament_z_hop`, `activate_air_filtration`, three `*_when_cut` keys, `filament_retract_before_wipe` |

Every (preset, key, Standard, High Flow) triple is in `filaments_ours_high_flow_diffs.csv` (263 rows).

(read) Typical High Flow max flow is 1.2–2x Standard (15 -> 22, 20 -> 40, 18 -> 36).

## 1.4 The clone at 02.03.08.00 (script unless marked)

### Counts

- 48 families, 152 U1 filament presets, against 43 and 129 in ours.
- 5 new families: Snapmaker Nylon (0.4/0.6/0.8), PETG (0.2/0.4/0.6/0.8), PLA Gradient (0.2/0.4/0.6/0.8), PLA Marble (0.4/0.6/0.8), PLA Starlight (0.4/0.6/0.8): 17 presets.
- Generic PA, PA-CF and PVA gain 0.6 and 0.8: 6 presets.
- Filament list sizes: 352 entries in theirs against 312 in ours. 68 names exist only in theirs: 28 renamed 0.4 presets plus 40 new (23 U1 presets, 5 U1 bases, 11 Artisan/J1 presets for Nylon, PETG and PETG HF, 1 category `fdm_filament_nylon_category`). The brief mentioned "56 new presets"; my count does not match that figure and I do not know what it counted.

### Layout differences

- Every U1 preset is named `<family> @U1 <size> nozzle` (or the generic names without a size for 0.4 in Generic). Each is pinned with `compatible_printers: ["Snapmaker U1 (<size> nozzle)"]` and has no `nozzle_size` tag.
- 28 presets are renamed 0.4 presets. Each has `renamed_from` set to the old name: 18 Generic (ABS, BVOH, PA, PA-CF, PC, PCTG, PETG, PETG HF, PETG-CF, PETG-GF, PLA, PLA High Speed, PLA Silk, PLA-CF, PVA, Support For PLA, TPU, TPU 95A HF, each `Generic X` -> `Generic X @U1 0.4 nozzle`; Generic ASA was already named that way), 3 Polymaker (`… Family @U1` -> `… @U1 0.4 nozzle`), and Snapmaker PETG HF, PLA Basic, PLA Matte, PLA Silk, PLA SnapSpeed, PVA, TPU 90A. The authoritative list is `filaments_theirs_presets.csv`, column `status_vs_ours`.
- Their High Flow marker is `filament_flow_support` alone, with no `filament_extruder_variant`: 23 presets declare two columns (22 on 0.4, 1 on 0.2), and 35 presets have two-element vectors in total (see 4).
- 62 keys differ by size across the 42 multi-size families; 93 never differ (ours: 61 and 94).

### Value differences, shared presets

- Non-Generic presets: 74 compared, 34 identical. Differences by key (preset counts): `graphic_effect_plate_temp` and `_initial_layer` 28 (for example 0 vs 70 on PLA-CF, 0 vs 80 on PETG Translucent); `filament_minimal_purge_on_wipe_tower` 17 (theirs 15 where ours is 20, 30 or 50); `hot_plate_temp` 7; `textured_plate_temp` 5; `close_fan_the_first_x_layers` 5 (1 vs 3 on PLA Translucent); `filament_adhesiveness_category` 5 (new key, for example 805 on Support For PLA). A few single speed or temperature edits remain (ASA 0.8 `filament_max_volumetric_speed` 18 vs 16 and `hot_plate_temp` 100 vs 110; TPU 95A HF 0.6/0.8 temperatures and speed).
- Generic presets: 55 compared, 12 identical. `filament_max_volumetric_speed` differs in 35; these are our own sync edits. Ours adds High Flow columns on Generic 0.4; theirs has none there.
- Per-row detail: `filaments_theirs_vs_ours.csv` (249 rows). Per-size diffs inside theirs: `filaments_theirs_size_diffs.csv`, `filaments_theirs_key_counts.csv`; their second-column diffs: `filaments_theirs_second_column_diffs.csv`.

### Table: sizes present in theirs

| family | 0.2 | 0.4 | 0.6 | 0.8 | 0.4 columns |
|---|---|---|---|---|---|
| Generic ABS | x | x | x | x | 1 |
| Generic ASA | x | x | x | x | 1 |
| Generic BVOH |  | x |  |  | 1 |
| Generic PA |  | x | x | x | 1 |
| Generic PA-CF |  | x | x | x | 1 |
| Generic PC |  | x | x | x | 1 |
| Generic PCTG |  | x |  |  | 1 |
| Generic PETG | x | x | x | x | 1 |
| Generic PETG HF | x | x | x | x | 1 |
| Generic PETG-CF |  | x | x | x | 1 |
| Generic PETG-GF |  | x | x | x | 1 |
| Generic PLA | x | x | x | x | 1 |
| Generic PLA High Speed | x | x | x | x | 1 |
| Generic PLA Silk | x | x | x | x | 1 |
| Generic PLA-CF |  | x | x | x | 1 |
| Generic PVA |  | x | x | x | 1 |
| Generic Support For PLA | x | x | x | x | 1 |
| Generic TPU |  | x | x | x | 1 |
| Generic TPU 95A HF |  | x | x | x | 1 |
| Polymaker General PLA Family |  | x |  |  | 1 |
| Polymaker Silk PLA Family |  | x |  |  | 1 |
| Polymaker Tough PLA Family |  | x |  |  | 1 |
| Snapmaker ABS | x | x | x | x | 2 |
| Snapmaker ASA | x | x | x | x | 2 |
| Snapmaker Nylon |  | x | x | x | 1 |
| Snapmaker PEBA 90A |  | x | x | x | 2 |
| Snapmaker PETG | x | x | x | x | 1 |
| Snapmaker PETG HF | x | x | x | x | 2 |
| Snapmaker PETG Translucent | x | x | x | x | 2 |
| Snapmaker PETG-CF |  | x | x | x | 2 |
| Snapmaker PLA Basic |  | x |  |  | 1 |
| Snapmaker PLA Full Spectrum | x | x | x |  | 2 |
| Snapmaker PLA Glow |  | x | x | x | 2 |
| Snapmaker PLA Gradient | x | x | x | x | 2 |
| Snapmaker PLA Marble |  | x | x | x | 2 |
| Snapmaker PLA Matte | x | x | x | x | 2 |
| Snapmaker PLA Rainbow | x | x | x | x | 2 |
| Snapmaker PLA Silk | x | x | x | x | 2 |
| Snapmaker PLA SnapSpeed | x | x | x | x | 2 |
| Snapmaker PLA Starlight |  | x | x | x | 2 |
| Snapmaker PLA Translucent | x | x | x | x | 2 |
| Snapmaker PLA Wood |  | x | x | x | 2 |
| Snapmaker PLA-CF |  | x | x | x | 2 |
| Snapmaker PVA |  | x | x | x | 2 |
| Snapmaker Support For PLA | x | x | x | x | 2 |
| Snapmaker TPU |  | x | x | x | 1 |
| Snapmaker TPU 90A |  | x | x | x | 2 |
| Snapmaker TPU 95A HF |  | x | x | x | 2 |

Notes on this table: the "0.4 columns" cell for `Snapmaker PETG` and the `Snapmaker PLA Gradient` row is unreliable because their column markers are inconsistent (see 4, items 15–16).

---

# 2. Process presets (`resources/profiles/Snapmaker/process`)

## 2.1 What exists (script)

90 files have `U1` in their name.

| Kind | Count | Hidden | Origin |
|---|---|---|---|
| Stock `0.NNmm <Quality> @Snapmaker U1 (<size> nozzle)` | 18 | no | Snapmaker's files; `convert_u1_single_printer.py` retags them |
| Plate `Multi-nozzle <Strategy> @Snapmaker U1` | 7 | no | the 5 base strategies hand-authored (inferred: the generator does not create their main keys); `generate_u1_multi_nozzle_presets.py` adds their High Flow column and writes the 2 High Quality plates (Fine, Normal) |
| Strategy per size `Multi-nozzle <Strategy> [High Quality] @Snapmaker U1 (<size> nozzle)` | 21 | yes | the generator (`size_preset`, `high_quality_presets`) |
| Height per size `0.NNmm Multi-nozzle @Snapmaker U1 (<size> nozzle)` | 21 | yes | the generator (`height_preset`) |
| Bases `fdm_process_U1`, `_common`, per-height and per-size | 23 | not instantiated | Snapmaker's |

- "Hidden" is not stored in any file. `PerHeadProcess::hide_multi_nozzle_size_presets` (`src/libslic3r/PerHeadProcess.cpp:267`) hides, after the bundle loads (`PresetBundle.cpp:8469`), system presets named `<plate> (<size> nozzle)` and any height preset (`is_multi_nozzle_height_preset`, `PerHeadProcess.cpp:280`).
- A plate is a preset whose `multi_nozzle_layer_heights` is set and not `off` or `custom` (`plans_strategy`, `PerHeadProcess.cpp:95`).
- All 67 non-base files are `instantiation: true`. Only `process_u1_all.csv` has all 90 rows with 31 key values.

### Stock presets (18)

| Nozzle | Presets |
|---|---|
| 0.2 | 0.08 HQ, 0.10 HQ, 0.12 S |
| 0.4 | 0.08 S, 0.10 Color Mixing, 0.12 S, 0.16 S, 0.20 HQ, 0.20 S (two columns), 0.24 S, 0.28 S |
| 0.6 | 0.18 S, 0.24 S, 0.30 S |
| 0.8 | 0.24 S, 0.32 S, 0.40 S, 0.40 Strength |

### Plates (7). Values are stored text; `a | b` = Standard | High Flow column

| name | nozzle_size | inherits | layer_height | initial_layer_print_height | outer_wall_speed | inner_wall_speed | sparse_infill_speed | default_acceleration | prime_volume | support_top_z_distance | multi_nozzle_layer_heights |
|---|---|---|---|---|---|---|---|---|---|---|---|
| Multi-nozzle Coarse @Snapmaker U1 | 0.4 | 0.24mm Standard @Snapmaker U1 (0.4 nozzle) | 0.12 | 0.12 | 200 \| 500 | 230 \| 600 | 230 \| 600 | 10000 | 54 | 0.2 | coarse |
| Multi-nozzle Fine @Snapmaker U1 | 0.4 | 0.16mm Standard @Snapmaker U1 (0.4 nozzle) | 0.08 | 0.08 | 200 \| 500 | 300 \| 600 | 270 \| 600 | 10000 | 36 | 0.16 | fine |
| Multi-nozzle Fine High Quality @Snapmaker U1 | 0.4 | Multi-nozzle Fine @Snapmaker U1 | 0.08 | 0.08 | 60 | 150 | 200 | 4000 | 36 | 0.16 | fine |
| Multi-nozzle Normal @Snapmaker U1 | 0.4 | 0.20mm Standard @Snapmaker U1 (0.4 nozzle) | 0.1 | 0.1 | 200 \| 500 | 300 \| 600 | 270 \| 600 | 10000 | 45 | 0.2 | normal |
| Multi-nozzle Normal High Quality @Snapmaker U1 | 0.4 | Multi-nozzle Normal @Snapmaker U1 | 0.1 | 0.1 | 60 | 150 | 200 | 4000 | 45 | 0.2 | normal |
| Multi-nozzle Super coarse @Snapmaker U1 | 0.4 | 0.28mm Standard @Snapmaker U1 (0.4 nozzle) | 0.14 | 0.14 | 200 \| 500 | 200 \| 600 | 200 \| 600 | 10000 | 63 | 0.2 | super_coarse |
| Multi-nozzle Ultra fine @Snapmaker U1 | 0.4 | 0.12mm Standard @Snapmaker U1 (0.4 nozzle) | 0.05 | 0.1 | 60 | 150 | 180 | 4000 | 27 | 0.12 | ultra_fine |

`layer_height` on a plate is the 0.2 nozzle's grid height, not the 0.4 head's. Strategy layer heights by nozzle size (from the generator's `STRATEGIES`, read):

| Strategy | 0.2 | 0.6 | 0.8 |
|---|---|---|---|
| Ultra fine | 0.05 | 0.15 | 0.20 |
| Fine | 0.08 | 0.24 | 0.32 |
| Normal | 0.10 | 0.30 | 0.40 |
| Coarse | 0.12 | 0.36 | 0.48 |
| Super coarse | 0.14 | 0.42 | 0.56 |

The 0.4 head's height under a strategy is not in this table: the plate inherits a 0.4 stock preset (0.12, 0.16, 0.16, 0.20, 0.20, 0.24, 0.28 for Ultra fine, Fine, Fine HQ, Normal, Normal HQ, Coarse, Super coarse).

### Strategy-size presets (21, hidden)

| name | nozzle_size | inherits | layer_height | initial_layer_print_height | outer_wall_speed | inner_wall_speed | sparse_infill_speed | default_acceleration | prime_volume | support_top_z_distance | multi_nozzle_layer_heights |
|---|---|---|---|---|---|---|---|---|---|---|---|
| Multi-nozzle Coarse @Snapmaker U1 (0.2 nozzle) | 0.2 | 0.12mm Standard @Snapmaker U1 (0.2 nozzle) | 0.12 | 0.14 | 120 | 150 | 100 | 10000 | 27 | 0.12 | · |
| Multi-nozzle Coarse @Snapmaker U1 (0.6 nozzle) | 0.6 | fdm_process_U1_0.36_nozzle_0.6 | 0.36 | 0.3 | 120 | 150 | 100 | 10000 | 68 | 0.2 | · |
| Multi-nozzle Coarse @Snapmaker U1 (0.8 nozzle) | 0.8 | fdm_process_U1_0.48_nozzle_0.8 | 0.48 | 0.4 | 120 | 150 | 100 | 10000 | 90 | 0.2 | · |
| Multi-nozzle Fine @Snapmaker U1 (0.2 nozzle) | 0.2 | fdm_process_U1_0.08_nozzle_0.2 | 0.08 | 0.14 | 120 | 150 | 100 | 10000 | 27 | 0.08 | · |
| Multi-nozzle Fine @Snapmaker U1 (0.6 nozzle) | 0.6 | 0.24mm Standard @Snapmaker U1 (0.6 nozzle) | 0.24 | 0.3 | 120 | 150 | 150 | 10000 | 55 | 0.2 | · |
| Multi-nozzle Fine @Snapmaker U1 (0.8 nozzle) | 0.8 | 0.32mm Standard @Snapmaker U1 (0.8 nozzle) | 0.32 | 0.4 | 120 | 150 | 100 | 10000 | 72 | 0.2 | · |
| Multi-nozzle Fine High Quality @Snapmaker U1 (0.2 nozzle) | 0.2 | 0.08mm High Quality @Snapmaker U1 (0.2 nozzle) | 0.08 | 0.1 | 60 | 150 | 100 | 4000 | 18 | 0.08 | · |
| Multi-nozzle Fine High Quality @Snapmaker U1 (0.6 nozzle) | 0.6 | Multi-nozzle Fine @Snapmaker U1 (0.6 nozzle) | 0.24 | 0.3 | 60 | 150 | 150 | 4000 | 55 | 0.2 | · |
| Multi-nozzle Fine High Quality @Snapmaker U1 (0.8 nozzle) | 0.8 | Multi-nozzle Fine @Snapmaker U1 (0.8 nozzle) | 0.32 | 0.4 | 60 | 150 | 100 | 4000 | 72 | 0.2 | · |
| Multi-nozzle Normal @Snapmaker U1 (0.2 nozzle) | 0.2 | fdm_process_U1_0.10_nozzle_0.2 | 0.1 | 0.14 | 120 | 150 | 100 | 10000 | 27 | 0.1 | · |
| Multi-nozzle Normal @Snapmaker U1 (0.6 nozzle) | 0.6 | 0.30mm Standard @Snapmaker U1 (0.6 nozzle) | 0.3 | 0.3 | 120 | 150 | 100 | 10000 | 68 | 0.2 | · |
| Multi-nozzle Normal @Snapmaker U1 (0.8 nozzle) | 0.8 | 0.40mm Standard @Snapmaker U1 (0.8 nozzle) | 0.4 | 0.4 | 120 | 150 | 100 | 10000 | 90 | 0.2 | · |
| Multi-nozzle Normal High Quality @Snapmaker U1 (0.2 nozzle) | 0.2 | 0.10mm High Quality @Snapmaker U1 (0.2 nozzle) | 0.1 | 0.12 | 60 | 150 | 100 | 4000 | 25 | 0.1 | · |
| Multi-nozzle Normal High Quality @Snapmaker U1 (0.6 nozzle) | 0.6 | Multi-nozzle Normal @Snapmaker U1 (0.6 nozzle) | 0.3 | 0.3 | 60 | 150 | 100 | 4000 | 68 | 0.2 | · |
| Multi-nozzle Normal High Quality @Snapmaker U1 (0.8 nozzle) | 0.8 | Multi-nozzle Normal @Snapmaker U1 (0.8 nozzle) | 0.4 | 0.4 | 60 | 150 | 100 | 4000 | 90 | 0.2 | · |
| Multi-nozzle Super coarse @Snapmaker U1 (0.2 nozzle) | 0.2 | fdm_process_U1_0.14_nozzle_0.2 | 0.14 | 0.14 | 120 | 150 | 100 | 10000 | 27 | 0.14 | · |
| Multi-nozzle Super coarse @Snapmaker U1 (0.6 nozzle) | 0.6 | fdm_process_U1_0.42_nozzle_0.6 | 0.42 | 0.3 | 120 | 150 | 100 | 10000 | 68 | 0.2 | · |
| Multi-nozzle Super coarse @Snapmaker U1 (0.8 nozzle) | 0.8 | fdm_process_U1_0.56_nozzle_0.8 | 0.56 | 0.4 | 120 | 150 | 100 | 10000 | 90 | 0.2 | · |
| Multi-nozzle Ultra fine @Snapmaker U1 (0.2 nozzle) | 0.2 | fdm_process_U1_0.06_nozzle_0.2 | 0.05 | 0.14 | 120 | 150 | 100 | 10000 | 27 | 0.06 | · |
| Multi-nozzle Ultra fine @Snapmaker U1 (0.6 nozzle) | 0.6 | fdm_process_U1_0.18_nozzle_0.6 | 0.15 | 0.3 | 120 | 150 | 100 | 10000 | 68 | 0.18 | · |
| Multi-nozzle Ultra fine @Snapmaker U1 (0.8 nozzle) | 0.8 | fdm_process_U1_0.24_nozzle_0.8 | 0.20 | 0.4 | 120 | 150 | 100 | 10000 | 90 | 0.2 | · |

### Height-size presets (21, hidden)

Heights per nozzle: 0.2: 0.10. 0.4: 0.08, 0.10, 0.12, 0.14, 0.15, 0.24, 0.30. 0.6: 0.16, 0.20, 0.24, 0.28, 0.32, 0.40. 0.8: 0.24, 0.25, 0.30, 0.36, 0.40, 0.42, 0.50.

| name | nozzle_size | inherits | layer_height | initial_layer_print_height | outer_wall_speed | inner_wall_speed | sparse_infill_speed | default_acceleration | prime_volume | support_top_z_distance | multi_nozzle_layer_heights |
|---|---|---|---|---|---|---|---|---|---|---|---|
| 0.08mm Multi-nozzle @Snapmaker U1 (0.4 nozzle) | 0.4 | 0.08mm Standard @Snapmaker U1 (0.4 nozzle) | 0.08 | 0.2 | 90 | 120 | 200 | 4000 | 18 | 0.08 | · |
| 0.10mm Multi-nozzle @Snapmaker U1 (0.2 nozzle) | 0.2 | fdm_process_U1_0.10_nozzle_0.2 | 0.1 | 0.14 | 120 | 150 | 100 | 10000 | 27 | 0.1 | · |
| 0.10mm Multi-nozzle @Snapmaker U1 (0.4 nozzle) | 0.4 | 0.12mm Standard @Snapmaker U1 (0.4 nozzle) | 0.10 | 0.2 | 60 | 150 | 180 | 4000 | 27 | 0.12 | · |
| 0.12mm Multi-nozzle @Snapmaker U1 (0.4 nozzle) | 0.4 | 0.12mm Standard @Snapmaker U1 (0.4 nozzle) | 0.12 | 0.2 | 60 | 150 | 180 | 4000 | 27 | 0.12 | · |
| 0.14mm Multi-nozzle @Snapmaker U1 (0.4 nozzle) | 0.4 | 0.16mm Standard @Snapmaker U1 (0.4 nozzle) | 0.14 | 0.2 | 200 | 300 | 270 | 10000 | 36 | 0.16 | · |
| 0.15mm Multi-nozzle @Snapmaker U1 (0.4 nozzle) | 0.4 | 0.16mm Standard @Snapmaker U1 (0.4 nozzle) | 0.15 | 0.2 | 200 | 300 | 270 | 10000 | 36 | 0.16 | · |
| 0.16mm Multi-nozzle @Snapmaker U1 (0.6 nozzle) | 0.6 | fdm_process_U1_0.18_nozzle_0.6 | 0.16 | 0.3 | 120 | 150 | 100 | 10000 | 68 | 0.18 | · |
| 0.20mm Multi-nozzle @Snapmaker U1 (0.6 nozzle) | 0.6 | fdm_process_U1_0.18_nozzle_0.6 | 0.20 | 0.3 | 120 | 150 | 100 | 10000 | 68 | 0.18 | · |
| 0.24mm Multi-nozzle @Snapmaker U1 (0.4 nozzle) | 0.4 | 0.24mm Standard @Snapmaker U1 (0.4 nozzle) | 0.24 | 0.2 | 200 \| 500 | 230 \| 600 | 230 \| 600 | 10000 | 54 | 0.2 | · |
| 0.24mm Multi-nozzle @Snapmaker U1 (0.6 nozzle) | 0.6 | 0.24mm Standard @Snapmaker U1 (0.6 nozzle) | 0.24 | 0.3 | 120 | 150 | 150 | 10000 | 55 | 0.2 | · |
| 0.24mm Multi-nozzle @Snapmaker U1 (0.8 nozzle) | 0.8 | 0.24mm Standard @Snapmaker U1 (0.8 nozzle) | 0.24 | 0.32 | 120 | 150 | 100 | 10000 | 54 | 0.2 | · |
| 0.25mm Multi-nozzle @Snapmaker U1 (0.8 nozzle) | 0.8 | fdm_process_U1_0.24_nozzle_0.8 | 0.25 | 0.4 | 120 | 150 | 100 | 10000 | 90 | 0.2 | · |
| 0.28mm Multi-nozzle @Snapmaker U1 (0.6 nozzle) | 0.6 | fdm_process_U1_0.30_nozzle_0.6 | 0.28 | 0.3 | 120 | 150 | 100 | 10000 | 68 | 0.2 | · |
| 0.30mm Multi-nozzle @Snapmaker U1 (0.4 nozzle) | 0.4 | 0.28mm Standard @Snapmaker U1 (0.4 nozzle) | 0.30 | 0.2 | 200 \| 500 | 200 \| 600 | 200 \| 600 | 10000 | 63 | 0.2 | · |
| 0.30mm Multi-nozzle @Snapmaker U1 (0.8 nozzle) | 0.8 | fdm_process_U1_0.32_nozzle_0.8 | 0.30 | 0.4 | 120 | 150 | 100 | 10000 | 90 | 0.2 | · |
| 0.32mm Multi-nozzle @Snapmaker U1 (0.6 nozzle) | 0.6 | fdm_process_U1_0.30_nozzle_0.6 | 0.32 | 0.3 | 120 | 150 | 100 | 10000 | 68 | 0.2 | · |
| 0.36mm Multi-nozzle @Snapmaker U1 (0.8 nozzle) | 0.8 | fdm_process_U1_0.32_nozzle_0.8 | 0.36 | 0.4 | 120 | 150 | 100 | 10000 | 90 | 0.2 | · |
| 0.40mm Multi-nozzle @Snapmaker U1 (0.6 nozzle) | 0.6 | fdm_process_U1_0.42_nozzle_0.6 | 0.40 | 0.3 | 120 | 150 | 100 | 10000 | 68 | 0.2 | · |
| 0.40mm Multi-nozzle @Snapmaker U1 (0.8 nozzle) | 0.8 | 0.40mm Standard @Snapmaker U1 (0.8 nozzle) | 0.4 | 0.4 | 120 | 150 | 100 | 10000 | 90 | 0.2 | · |
| 0.42mm Multi-nozzle @Snapmaker U1 (0.8 nozzle) | 0.8 | fdm_process_U1_0.40_nozzle_0.8 | 0.42 | 0.4 | 120 | 150 | 100 | 10000 | 90 | 0.2 | · |
| 0.50mm Multi-nozzle @Snapmaker U1 (0.8 nozzle) | 0.8 | fdm_process_U1_0.48_nozzle_0.8 | 0.50 | 0.4 | 120 | 150 | 100 | 10000 | 90 | 0.2 | · |

### Bases (23, `instantiation: false`)

| name | instantiation | inherits | layer_height | initial_layer_print_height | prime_volume | support_top_z_distance | wall_loops | line_width |
|---|---|---|---|---|---|---|---|---|
| fdm_process_U1 | false | · | · | 0.2 | · | 0.15 | 2 | 112.5% |
| fdm_process_U1_0.06_nozzle_0.2 | false | fdm_process_U1_common | 0.06 | 0.1 | · | 0.06 | 4 | 110% |
| fdm_process_U1_0.08 | false | fdm_process_U1_common | 0.08 | 0.2 | · | 0.08 | 2 | 105% |
| fdm_process_U1_0.08_nozzle_0.2 | false | fdm_process_U1_common | 0.08 | 0.1 | · | 0.08 | 4 | 110% |
| fdm_process_U1_0.10_nozzle_0.2 | false | fdm_process_U1_common | 0.1 | 0.1 | · | 0.1 | 4 | 110% |
| fdm_process_U1_0.12 | false | fdm_process_U1_common | 0.12 | 0.2 | · | 0.12 | 2 | 105% |
| fdm_process_U1_0.12_nozzle_0.2 | false | fdm_process_U1_common | 0.12 | 0.1 | · | 0.12 | 4 | 110% |
| fdm_process_U1_0.14_nozzle_0.2 | false | fdm_process_U1_common | 0.14 | 0.1 | · | 0.14 | 4 | 110% |
| fdm_process_U1_0.16 | false | fdm_process_U1_common | 0.16 | 0.2 | · | 0.16 | 2 | 105% |
| fdm_process_U1_0.18_nozzle_0.6 | false | fdm_process_U1_common | 0.18 | 0.3 | · | 0.18 | 2 | 103.33% |
| fdm_process_U1_0.20 | false | fdm_process_U1_common | 0.2 | 0.2 | · | 0.2 | 2 | 105% |
| fdm_process_U1_0.24 | false | fdm_process_U1_common | 0.24 | 0.2 | · | 0.2 | 2 | 105% |
| fdm_process_U1_0.24_nozzle_0.6 | false | fdm_process_U1_common | 0.24 | 0.3 | · | 0.2 | 2 | 103.33% |
| fdm_process_U1_0.24_nozzle_0.8 | false | fdm_process_U1_common | 0.24 | 0.4 | · | 0.2 | 2 | 102.5% |
| fdm_process_U1_0.28 | false | fdm_process_U1_common | 0.28 | 0.2 | · | 0.2 | 2 | 105% |
| fdm_process_U1_0.30_nozzle_0.6 | false | fdm_process_U1_common | 0.3 | 0.3 | · | 0.2 | 2 | 103.33% |
| fdm_process_U1_0.32_nozzle_0.8 | false | fdm_process_U1_common | 0.32 | 0.4 | · | 0.2 | 2 | 102.5% |
| fdm_process_U1_0.36_nozzle_0.6 | false | fdm_process_U1_common | 0.36 | 0.3 | · | 0.2 | 2 | 103.33% |
| fdm_process_U1_0.40_nozzle_0.8 | false | fdm_process_U1_common | 0.4 | 0.4 | · | 0.2 | 2 | 102.5% |
| fdm_process_U1_0.42_nozzle_0.6 | false | fdm_process_U1_common | 0.42 | 0.3 | · | 0.2 | 2 | 103.33% |
| fdm_process_U1_0.48_nozzle_0.8 | false | fdm_process_U1_common | 0.48 | 0.4 | · | 0.2 | 2 | 102.5% |
| fdm_process_U1_0.56_nozzle_0.8 | false | fdm_process_U1_common | 0.56 | 0.4 | · | 0.2 | 2 | 102.5% |
| fdm_process_U1_common | false | fdm_process_U1 | 0.2 | 0.2 | · | 0.2 | 2 | 105% |

### Two-column process presets (script)

15 presets have two columns: the 7 plates, `0.20mm Standard (0.4)`, and 7 hidden height presets (0.08, 0.10, 0.12, 0.14, 0.15, 0.24, 0.30 on 0.4). In 7 of the 15 the High Flow column differs from Standard, only in the outer, inner, sparse and solid-infill speeds (500/600/600/600 mm/s): the stock 0.20 S, the plates Fine, Normal, Coarse and Super coarse, and the hidden 0.24 and 0.30. In the other 8 (the plates Ultra fine, Fine HQ and Normal HQ, and the hidden 0.08, 0.10, 0.12, 0.14, 0.15) the two columns are identical. Prime volume in the High Flow column equals Standard. `process_flow_support` is only a marker.

## 2.2 Keys that vary across the 18 stock presets (script)

The 18 stock presets have 133 keys: 77 identical in all 18 (the full list is at the end of this section) and 56 that vary. The "depends on" classification below takes the smallest of (size, height, quality, size+height, ...) that predicts the key without conflict. With 18 presets there are only two pairs of the same (size, height) with different quality: 0.4/0.20 (HQ vs S) and 0.8/0.40 (S vs Strength). So "size+height" really means "not predicted by either alone and the two quality pairs agree".

| Depends on | Keys |
|---|---|
| size only (15) | `bridge_speed`, `initial_layer_line_width`, `inner_wall_line_width`, `line_width`, `outer_wall_line_width`, `sparse_infill_line_width`, `internal_solid_infill_line_width`, `support_line_width`, `top_surface_pattern`, `filter_out_gap_fill`, `gap_fill_target`, `internal_bridge_speed`, `overhang_4_4_speed`, `wipe_tower_cone_angle`, `wipe_tower_filament` |
| height only (6) | `layer_height`, `sparse_infill_pattern`, `internal_solid_infill_pattern`, `precise_z_height`, `preheat_time`, `wipe_speed` |
| quality only (1) | `sparse_infill_density` (30% on Strength, 15% elsewhere) |
| size + height (14) | `initial_layer_print_height`, `initial_layer_speed`, `initial_layer_infill_speed`, `gap_infill_speed`, `ironing_flow`, `min_width_top_surface`, `prime_tower_width`, `prime_volume`, `seam_gap`, `support_top_z_distance`, `support_bottom_z_distance`, `support_threshold_angle`, `top_shell_thickness`, `top_surface_line_width` |
| size + quality (2) | `inner_wall_acceleration`, `wall_loops` |
| height + quality (6) | `bottom_shell_layers`, `bridge_density`, `bridge_flow`, and three High Flow marker keys (`print_extruder_id`, `print_extruder_variant`, `process_flow_support`) |
| size + height + quality (12) | `default_acceleration`, `outer_wall_acceleration`, `inner_wall_speed`, `outer_wall_speed`, `sparse_infill_speed`, `internal_solid_infill_speed`, `top_surface_speed`, `overhang_1_4_speed`, `overhang_2_4_speed`, `overhang_3_4_speed`, `top_shell_layers`, `top_color_penetration_layers` |

Across all 67 instantiated U1 presets (stock, plates, generated) 75 keys are constant; the generated ones copy a few more keys from other presets.

### The 56 varying keys, by preset (stock). A `a | b` entry is Standard | High Flow

| key | 0.08 High Quality (0.2 nozzle) | 0.10 High Quality (0.2 nozzle) | 0.12 Standard (0.2 nozzle) | 0.08 Standard (0.4 nozzle) | 0.10 Color Mixing (0.4 nozzle) | 0.12 Standard (0.4 nozzle) | 0.16 Standard (0.4 nozzle) | 0.20 High Quality (0.4 nozzle) | 0.20 Standard (0.4 nozzle) | 0.24 Standard (0.4 nozzle) | 0.28 Standard (0.4 nozzle) | 0.18 Standard (0.6 nozzle) | 0.24 Standard (0.6 nozzle) | 0.30 Standard (0.6 nozzle) | 0.24 Standard (0.8 nozzle) | 0.32 Standard (0.8 nozzle) | 0.40 Standard (0.8 nozzle) | 0.40 Strength (0.8 nozzle) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| bottom_shell_layers | 5 | 5 | 5 | 7 | 5 | 5 | 4 | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 5 |
| bridge_density | · | · | · | · | · | · | · | 80% | 100% | · | · | · | · | · | · | 70% | 60% | 60% |
| bridge_flow | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 0.8 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 0.7 | 0.8 | 0.8 |
| bridge_speed | 50 | 50 | 50 | 50 | 50 | 50 | 50 | 50 | 50 | 50 | 50 | 30 | 30 | 30 | 30 | 30 | 30 | 30 |
| default_acceleration | 4000 | 4000 | 10000 | 4000 | 4000 | 4000 | 10000 | 4000 | 10000 | 10000 | 10000 | 10000 | 10000 | 10000 | 10000 | 10000 | 10000 | 10000 |
| filter_out_gap_fill | · | · | · | · | · | · | · | · | · | · | · | 1 | 1 | 1 | 1 | 1 | 1 | 1 |
| gap_fill_target | · | · | · | topbottom | topbottom | topbottom | topbottom | topbottom | topbottom | topbottom | topbottom | topbottom | topbottom | topbottom | topbottom | topbottom | topbottom | topbottom |
| gap_infill_speed | 50 | 50 | 50 | 150 | 230 | 230 | 250 | 250 | 250 | 230 | 200 | 50 | 50 | 50 | 50 | 50 | 50 | 50 |
| initial_layer_infill_speed | 60 | 70 | 70 | 105 | 105 | 105 | 105 | 105 | 105 | 105 | 105 | 55 | 55 | 55 | 55 | 55 | 55 | 55 |
| initial_layer_line_width | 125% | 125% | 125% | 125% | 125% | 125% | 125% | 125% | 125% | 125% | 125% | 103.33% | 103.33% | 103.33% | 102.5% | 102.5% | 102.5% | 102.5% |
| initial_layer_print_height | 0.1 | 0.12 | 0.14 | 0.2 | 0.1 | 0.2 | 0.2 | 0.25 | 0.25 | 0.2 | 0.2 | 0.24 | 0.3 | 0.3 | 0.32 | 0.4 | 0.4 | 0.4 |
| initial_layer_speed | 30 | 40 | 40 | 50 | 50 | 50 | 50 | 50 | 50 | 50 | 50 | 35 | 35 | 35 | 35 | 35 | 35 | 35 |
| inner_wall_acceleration | · | · | · | · | 4000 | · | · | 4000 | · | · | · | · | · | · | · | · | · | · |
| inner_wall_line_width | 110% | 110% | 110% | 112.5% | 112.5% | 112.5% | 112.5% | 112.5% | 112.5% | 112.5% | 112.5% | 103.33% | 103.33% | 103.33% | 102.5% | 102.5% | 102.5% | 102.5% |
| inner_wall_speed | 150 | 150 | 150 | 120 | 150 | 150 | 300 | 150 | 300 \| 600 | 230 | 200 | 150 | 150 | 150 | 150 | 150 | 150 | 100 |
| internal_bridge_speed | · | · | · | · | · | · | · | · | · | · | · | 100% | 100% | 100% | 100% | 100% | 100% | 100% |
| internal_solid_infill_line_width | 110% | 110% | 110% | 105% | 105% | 105% | 105% | 105% | 105% | 105% | 105% | 103.33% | 103.33% | 103.33% | 102.5% | 102.5% | 102.5% | 102.5% |
| internal_solid_infill_pattern | · | · | · | · | · | · | · | rectilinear | rectilinear | · | · | · | · | · | · | · | · | · |
| internal_solid_infill_speed | 150 | 150 | 150 | 150 | 180 | 180 | 250 | 200 | 250 \| 600 | 230 | 200 | 150 | 150 | 150 | 150 | 150 | 150 | 150 |
| ironing_flow | 10% | 10% | 10% | 8% | 10% | 10% | 10% | 10% | 10% | 10% | 10% | 10% | 10% | 10% | 10% | 10% | 10% | 10% |
| layer_height | 0.08 | 0.1 | 0.12 | 0.08 | 0.1 | 0.12 | 0.16 | 0.2 | 0.2 | 0.24 | 0.28 | 0.18 | 0.24 | 0.3 | 0.24 | 0.32 | 0.4 | 0.4 |
| line_width | 110% | 110% | 110% | 105% | 105% | 105% | 105% | 105% | 105% | 105% | 105% | 103.33% | 103.33% | 103.33% | 102.5% | 102.5% | 102.5% | 102.5% |
| min_width_top_surface | · | · | · | · | · | · | · | 200% | 200% | · | · | · | · | · | 90 | · | 200% | 200% |
| outer_wall_acceleration | 2000 | 2000 | 5000 | 2000 | 2000 | 2000 | 5000 | 2000 | 5000 | 5000 | 5000 | 5000 | 5000 | 5000 | 5000 | 5000 | 5000 | 5000 |
| outer_wall_line_width | 110% | 110% | 110% | 105% | 105% | 105% | 105% | 105% | 105% | 105% | 105% | 103.33% | 103.33% | 103.33% | 102.5% | 102.5% | 102.5% | 102.5% |
| outer_wall_speed | 60 | 60 | 120 | 90 | 60 | 60 | 200 | 60 | 200 \| 500 | 200 | 200 | 120 | 120 | 120 | 120 | 120 | 120 | 80 |
| overhang_1_4_speed | 0 | 0 | 0 | 60 | 60 | 60 | 60 | 60 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| overhang_2_4_speed | 50 | 50 | 50 | 30 | 30 | 30 | 30 | 30 | 50 | 50 | 50 | 50 | 50 | 50 | 50 | 50 | 50 | 50 |
| overhang_3_4_speed | 30 | 30 | 30 | 10 | 10 | 10 | 10 | 10 | 30 | 30 | 30 | 15 | 15 | 15 | 25 | 25 | 25 | 25 |
| overhang_4_4_speed | 10 | 10 | 10 | 10 | 10 | 10 | 10 | 10 | 10 | 10 | 10 | 10 | 10 | 10 | 5 | 5 | 5 | 5 |
| precise_z_height | · | · | · | · | · | · | · | 0 | 0 | · | · | · | · | · | · | · | · | · |
| preheat_time | · | · | · | · | · | · | · | 30 | 30 | · | · | · | · | · | · | · | · | · |
| prime_tower_width | 30 | 28 | 28 | 30 | 30 | 30 | 30 | 30 | 30 | 30 | 30 | 30 | 30 | 30 | 30 | 30 | 30 | 30 |
| prime_volume | 18 | 25 | 27 | 18 | 27 | 27 | 36 | 45 | 45 | 54 | 63 | 41 | 55 | 68 | 54 | 72 | 90 | 90 |
| print_extruder_id | · | · | · | · | · | · | · | · | 1 | · | · | · | · | · | · | · | · | · |
| print_extruder_variant | · | · | · | · | · | · | · | · | Direct Drive Standard \| Direct Drive High Flow | · | · | · | · | · | · | · | · | · |
| process_flow_support | · | · | · | · | · | · | · | · | standard \| high_flow | · | · | · | · | · | · | · | · | · |
| seam_gap | · | · | · | · | · | · | · | 15% | 15% | · | · | 15% | 15% | 15% | 15% | 15% | 15% | 15% |
| sparse_infill_density | 15% | 15% | 15% | 15% | 15% | 15% | 15% | 15% | 15% | 15% | 15% | 15% | 15% | 15% | 15% | 15% | 15% | 30% |
| sparse_infill_line_width | 110% | 110% | 110% | 112.5% | 112.5% | 112.5% | 112.5% | 112.5% | 112.5% | 112.5% | 112.5% | 103.33% | 103.33% | 103.33% | 102.5% | 102.5% | 102.5% | 102.5% |
| sparse_infill_pattern | gyroid | gyroid | gyroid | gyroid | gyroid | gyroid | gyroid | grid | grid | grid | grid | gyroid | grid | grid | grid | grid | grid | grid |
| sparse_infill_speed | 100 | 100 | 100 | 200 | 180 | 180 | 270 | 200 | 270 \| 600 | 230 | 200 | 100 | 150 | 100 | 100 | 100 | 100 | 100 |
| support_bottom_z_distance | 0.08 | 0.1 | 0.12 | 0.08 | 0.12 | 0.12 | 0.16 | 0.2 | 0.2 | 0.2 | 0.2 | 0.18 | 0.2 | 0.2 | 0.2 | 0.2 | 0.2 | 0.2 |
| support_line_width | 110% | 110% | 110% | 105% | 105% | 105% | 105% | 105% | 105% | 105% | 105% | 103.33% | 103.33% | 103.33% | 102.5% | 102.5% | 102.5% | 102.5% |
| support_threshold_angle | 30 | 30 | 30 | 15 | 25 | 25 | 30 | 30 | 30 | 35 | 40 | 30 | 35 | 35 | 30 | 35 | 40 | 40 |
| support_top_z_distance | 0.08 | 0.1 | 0.12 | 0.08 | 0.12 | 0.12 | 0.16 | 0.2 | 0.2 | 0.2 | 0.2 | 0.18 | 0.2 | 0.2 | 0.2 | 0.2 | 0.2 | 0.2 |
| top_color_penetration_layers | 5 | 5 | 5 | 5 | 5 | 5 | 5 | 5 | 5 | 4 | 4 | 3 | 3 | 3 | 3 | 3 | 3 | 5 |
| top_shell_layers | 7 | 7 | 7 | 9 | 5 | 5 | 6 | 5 | 5 | 4 | 4 | 3 | 3 | 3 | 3 | 3 | 3 | 5 |
| top_shell_thickness | 0.8 | 0.8 | 0.8 | 0.8 | 0.6 | 0.6 | 1.0 | 1.0 | 1.0 | 1.0 | 1.0 | 0.8 | 0.8 | 0.8 | 0.8 | 0.8 | 0.8 | 0.8 |
| top_surface_line_width | 110% | 110% | 110% | 105% | 105% | 105% | 105% | 105% | 105% | 112.5% | 112.5% | 103.33% | 103.33% | 103.33% | 102.5% | 102.5% | 102.5% | 102.5% |
| top_surface_pattern | monotonicline | monotonicline | monotonicline | monotonicline | monotonicline | monotonicline | monotonicline | monotonicline | monotonicline | monotonicline | monotonicline | monotonicline | monotonicline | monotonicline | monotonic | monotonic | monotonic | monotonic |
| top_surface_speed | 150 | 150 | 150 | 90 | 150 | 150 | 200 | 150 | 200 | 200 | 200 | 150 | 150 | 150 | 150 | 150 | 150 | 150 |
| wall_loops | 4 | 4 | 4 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 6 |
| wipe_speed | · | · | · | · | · | · | · | 50 | 50 | · | · | · | · | · | · | · | · | · |
| wipe_tower_cone_angle | · | · | · | 15 | 15 | 15 | 15 | 15 | 15 | 15 | 15 | · | · | · | · | · | · | · |
| wipe_tower_filament | · | · | · | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | · | · | · | · | · | · | · |

### The 77 constant keys (same value in all 18 stock presets)

| key | value |
|---|---|
| `adaptive_layer_height` | 0 |
| `bottom_color_penetration_layers` | 3 |
| `bottom_shell_thickness` | 0 |
| `bottom_surface_pattern` | monotonic |
| `bridge_no_support` | 0 |
| `brim_object_gap` | 0.1 |
| `brim_width` | 5 |
| `detect_overhang_wall` | 1 |
| `detect_thin_wall` | 0 |
| `draft_shield` | disabled |
| `elefant_foot_compensation` | 0.15 |
| `enable_arc_fitting` | 0 |
| `enable_prime_tower` | 1 |
| `enable_support` | 0 |
| `exclude_object` | 1 |
| `filename_format` | {input_filename_base}_{filament_type[initial_tool]}_{print_time}.gcode |
| `infill_combination` | 0 |
| `infill_direction` | 45 |
| `infill_wall_overlap` | 15% |
| `initial_layer_acceleration` | 500 |
| `interface_shells` | 0 |
| `internal_bridge_support_thickness` | 0.8 |
| `ironing_spacing` | 0.15 |
| `ironing_speed` | 30 |
| `ironing_type` | no ironing |
| `max_bridge_length` | 0 |
| `max_travel_detour_distance` | 0 |
| `minimum_sparse_infill_area` | 15 |
| `only_one_wall_top` | 1 |
| `ooze_prevention` | 1 |
| `overhang_totally_speed` | 50 |
| `precise_outer_wall` | 0 |
| `prime_tower_brim_width` | 5 |
| `print_sequence` | by layer |
| `raft_first_layer_expansion` | -1 |
| `raft_layers` | 0 |
| `reduce_crossing_wall` | 0 |
| `reduce_infill_retraction` | 1 |
| `resolution` | 0.012 |
| `seam_position` | aligned |
| `skirt_distance` | 2 |
| `skirt_height` | 1 |
| `skirt_loops` | 0 |
| `slowdown_for_curled_perimeters` | 0 |
| `smooth_coefficient` | 150 |
| `spiral_mode` | 0 |
| `standby_temperature_delta` | -150 |
| `support_base_pattern` | default |
| `support_base_pattern_spacing` | 2.5 |
| `support_expansion` | 0 |
| `support_filament` | 0 |
| `support_interface_bottom_layers` | 2 |
| `support_interface_filament` | 0 |
| `support_interface_loop_pattern` | 0 |
| `support_interface_pattern` | auto |
| `support_interface_spacing` | 0.5 |
| `support_interface_speed` | 80 |
| `support_interface_top_layers` | 2 |
| `support_object_xy_distance` | 0.35 |
| `support_on_build_plate_only` | 0 |
| `support_speed` | 150 |
| `support_style` | default |
| `support_type` | tree(auto) |
| `top_surface_acceleration` | 2000 |
| `travel_speed` | 500 |
| `tree_support_branch_angle` | 45 |
| `tree_support_branch_angle_organic` | 45 |
| `tree_support_branch_diameter` | 2 |
| `tree_support_wall_count` | 0 |
| `wall_generator` | arachne |
| `wall_infill_order` | inner wall/outer wall/infill |
| `wipe_tower_extra_rib_length` | 8 |
| `wipe_tower_extra_spacing` | 120% |
| `wipe_tower_no_sparse_layers` | 0 |
| `wipe_tower_wall_type` | rib |
| `xy_contour_compensation` | 0 |
| `xy_hole_compensation` | 0 |

### Line widths in mm (derived from the stored percent of the nozzle)

| Nozzle | line, outer, solid, support | inner, sparse | top surface | first layer |
|---|---|---|---|---|
| 0.2 | 0.22 | 0.22 | 0.22 | 0.25 |
| 0.4 | 0.42 | 0.45 | 0.42 (0.45 at 0.24 and 0.28) | 0.50 |
| 0.6 | 0.62 | 0.62 | 0.62 | 0.62 |
| 0.8 | 0.82 | 0.82 | 0.82 | 0.82 |

Stored text: 110% (0.2), 105% and 112.5% (0.4), 103.33% (0.6), 102.5% (0.8); `initial_layer_line_width` 125% on 0.2 and 0.4.

### Volumetric flow, speed x width x layer height (script, mm³/s), stock presets, Standard column

Columns: outer, inner, sparse, solid, top, gap.

| Nozzle / height / quality | outer | inner | sparse | solid | top | gap |
|---|---|---|---|---|---|---|
| 0.2 / 0.08 HQ | 1.1 | 2.6 | 1.8 | 2.6 | 2.6 | 0.9 |
| 0.2 / 0.10 HQ | 1.3 | 3.3 | 2.2 | 3.3 | 3.3 | 1.1 |
| 0.2 / 0.12 S | 3.2 | 4.0 | 2.6 | 4.0 | 4.0 | 1.3 |
| 0.4 / 0.08 S | 3.0 | 4.3 | 7.2 | 5.0 | 3.0 | 5.0 |
| 0.4 / 0.10 CM | 2.5 | 6.8 | 8.1 | 7.6 | 6.3 | 9.7 |
| 0.4 / 0.12 S | 3.0 | 8.1 | 9.7 | 9.1 | 7.6 | 11.6 |
| 0.4 / 0.16 S | 13.4 | 21.6 | 19.4 | 16.8 | 13.4 | 16.8 |
| 0.4 / 0.20 HQ | 5.0 | 13.5 | 18.0 | 16.8 | 12.6 | 21.0 |
| 0.4 / 0.20 S | 16.8 | 27.0 | 24.3 | 21.0 | 16.8 | 21.0 |
| 0.4 / 0.20 S, High Flow | 42.0 | 54.0 | 54.0 | 50.4 | 16.8 | 21.0 |
| 0.4 / 0.24 S | 20.2 | 24.8 | 24.8 | 23.2 | 21.6 | 23.2 |
| 0.4 / 0.28 S | 23.5 | 25.2 | 25.2 | 23.5 | 25.2 | 23.5 |
| 0.6 / 0.18 S | 13.4 | 16.7 | 11.2 | 16.7 | 16.7 | 5.6 |
| 0.6 / 0.24 S | 17.9 | 22.3 | 22.3 | 22.3 | 22.3 | 7.4 |
| 0.6 / 0.30 S | 22.3 | 27.9 | 18.6 | 27.9 | 27.9 | 9.3 |
| 0.8 / 0.24 S | 23.6 | 29.5 | 19.7 | 29.5 | 29.5 | 9.8 |
| 0.8 / 0.32 S | 31.5 | 39.4 | 26.2 | 39.4 | 39.4 | 13.1 |
| 0.8 / 0.40 S | 39.4 | 49.2 | 32.8 | 49.2 | 49.2 | 16.4 |
| 0.8 / 0.40 Strength | 26.2 | 32.8 | 32.8 | 49.2 | 49.2 | 16.4 |

(The 0.4 High Flow rows for plates and other 0.4 presets with a second column are computed the same way from the speeds in section 2.1; only `0.20 S` is shown.)

## 2.3 Seven earlier findings, checked against the files (script)

| Finding | Verdict |
|---|---|
| Line widths are nozzle + 0.02 mm, except 0.45 mm on 0.4 for inner wall and sparse infill, and for top surface at 0.24–0.28 | **Holds exactly** for the 18 stock presets, plates and hidden presets. One more exception: the first-layer width is 0.25 on 0.2 (125%) and 0.50 on 0.4 (125%). 0.6 and 0.8 first layers follow the rule. The clone stores mm, ours stores percent; the resolved values are the same. Count of deviations in the 67 instantiated presets: 22 on 0.4 for inner wall, sparse and first layer, 6 on 0.4 for top surface, 11 on 0.2 for first layer. |
| Support top and bottom Z are min(layer height, 0.20) | **Approximately.** Stock exception: 0.4 `0.10mm Color Mixing` has 0.12 (inherits the 0.12 base). Generated presets deviate when they inherit a base of another height (full list in this table's next lines). |
| Speeds: one table shared by 0.2, 0.6 and 0.8, one for 0.4 | **Approximately.** 0.2/0.6/0.8 share outer 120, inner 150, solid 150, top 150, gap 50, sparse 100. Exceptions: 0.2 HQ outer 60; 0.8 Strength inner 100 and outer 80; 0.6/0.24 sparse 150; `bridge_speed` is 50 on 0.2 and 30 on 0.6/0.8; `overhang_3_4_speed` is 30/15/25 for 0.2/0.6/0.8 (read from the stock table above); `overhang_4_4_speed` is 5 on 0.8 and 10 elsewhere; first-layer speeds differ per size. 0.4 is not one table: speeds step with layer height (0.08–0.12 slow, 0.16 and up fast) plus the HQ preset. |
| On 0.4 at 0.16–0.28 speeds match a flow cap of about 25 mm³/s | **Approximately, only at 0.24 and 0.28.** 0.16 peaks at 21.6 (limited by the 300 mm/s inner-wall speed). 0.20 S has inner 27.0 and sparse 24.3. 0.24 gives 20.2–24.8; 0.28 gives 23.5–25.2. The 0.6 and 0.8 presets reach 22–49 mm³/s (table above). |
| Accelerations depend on the quality level, not on the nozzle | **Approximately.** Two sets: 4000/2000 (default/outer-wall) and 10000/5000. HQ presets (0.2 and 0.4) and 0.4 at 0.08, 0.10, 0.12 use 4000/2000. All others use 10000/5000, including 0.2/0.12 Standard. So 0.4/0.12 is slow and 0.2/0.12 is fast. The split is quality plus a height rule on 0.4, not purely quality. `initial_layer_acceleration` is 500 and `top_surface_acceleration` 2000 everywhere. |
| Prime volume is about 225 x layer height | **Stock: yes within about +/-1** (0.4 exact: 18, 27, 36, 45, 54, 63; 0.8: 54, 72, 90). Stock exceptions: 0.2/0.10 HQ 25 (22.5), 0.4/0.10 CM 27 (22.5), 0.6/0.24 55 (54), 0.6/0.18 41 (40.5). **Generated: no.** Hidden 0.6 presets from 0.16 to 0.40 all carry 68 (except 55 at 0.24); hidden 0.8 presets from 0.25 up all carry 90; the 0.2/0.10 one carries 27; plates carry the 0.4 stock value (27, 36, 45, 54, 63) against their small layer heights (0.05–0.14). |
| First-layer width, height and speeds are a few numbers per size | **Width and speeds: yes.** Speeds 30/40/40 and 60/70/70 on 0.2 (by HQ/Standard), 50/105 on 0.4, 35/55 on 0.6 and 0.8. Width 125%, 125%, 103.33%, 102.5%. **Height: no.** `initial_layer_print_height`: 0.2 nozzle 0.10/0.12/0.14 (layer height + 0.02); 0.4 mostly 0.2, 0.25 at 0.20 HQ and S, 0.10 on Color Mixing; 0.6 0.24/0.30/0.30; 0.8 0.32/0.40/0.40/0.40. |

### Support Z deviations from min(h, 0.2) (generated and plate presets)

| Preset | h | top = bottom | min(h, 0.2) |
|---|---|---|---|
| 0.10mm Multi-nozzle (0.4) | 0.10 | 0.12 | 0.10 |
| 0.14mm Multi-nozzle (0.4) | 0.14 | 0.16 | 0.14 |
| 0.15mm Multi-nozzle (0.4) | 0.15 | 0.16 | 0.15 |
| 0.16mm Multi-nozzle (0.6) | 0.16 | 0.18 | 0.16 |
| 0.20mm Multi-nozzle (0.6) | 0.20 | 0.18 | 0.20 |
| Plate Ultra fine | 0.05 | 0.12 | 0.05 |
| Plate Fine, Fine HQ | 0.08 | 0.16 | 0.08 |
| Plate Normal, Normal HQ | 0.10 | 0.20 | 0.10 |
| Plate Coarse | 0.12 | 0.20 | 0.12 |
| Plate Super coarse | 0.14 | 0.20 | 0.14 |
| Strategy Ultra fine (0.2) | 0.05 | 0.06 | 0.05 |
| Strategy Ultra fine (0.6) | 0.15 | 0.18 | 0.15 |
| Stock 0.10mm Color Mixing (0.4) | 0.10 | 0.12 | 0.10 |

### Prime volume deviations from 225 x h, generated and plate presets (selected)

| Preset | h | prime_volume | 225 x h |
|---|---|---|---|
| 0.16 / 0.20 / 0.28 / 0.32 / 0.40 Multi-nozzle (0.6) | 0.16 / 0.20 / 0.28 / 0.32 / 0.40 | 68 | 36 / 45 / 63 / 72 / 90 |
| 0.24 Multi-nozzle (0.6) | 0.24 | 55 | 54 |
| 0.25 / 0.30 / 0.36 / 0.42 / 0.50 Multi-nozzle (0.8) | same | 90 | 56 / 68 / 81 / 95 / 113 |
| 0.10 Multi-nozzle (0.2) and (0.4) | 0.10 | 27 | 22.5 |
| 0.14 / 0.15 / 0.30 Multi-nozzle (0.4) | same | 36 / 36 / 63 | 31.5 / 33.8 / 67.5 |
| Plate Ultra fine / Fine / Normal / Coarse / Super coarse | 0.05 / 0.08 / 0.10 / 0.12 / 0.14 | 27 / 36 / 45 / 54 / 63 | 11.2 / 18 / 22.5 / 27 / 31.5 |
| Strategy Ultra fine (0.6) / (0.8) | 0.15 / 0.20 | 68 / 90 | 33.8 / 45 |
| Strategy Coarse (0.6) / (0.8) | 0.36 / 0.48 | 68 / 90 | 81 / 108 |
| Strategy Super coarse (0.6) / (0.8) | 0.42 / 0.56 | 68 / 90 | 94.5 / 126 |

## 2.4 First layer, before and after commit `62553c9d`

### Presets (script)

- The 5 base plates carry `initial_layer_print_height` = Ultra fine 0.10, Fine 0.08, Normal 0.10, Coarse 0.12, Super coarse 0.14. The two HQ plates inherit those values.
- Hidden per-size and per-height presets keep first layers inherited from their base: mostly 0.14 on 0.2, 0.2 on 0.4, 0.3 on 0.6, 0.4 on 0.8 (see the tables in 2.1).

### Code (read)

- Commit `62553c9d` ("Multi-nozzle: every tool head lays its own first layer from the bed"): under a multi-nozzle strategy each head's first layer is `multi_nozzle_first_layer_height(strategy, head height, nozzle, max_lh)` (`src/libslic3r/Slicing.cpp:1123`). The factor is 2 for Ultra fine and 1 otherwise (`multi_nozzle_first_layer_factor`, `Slicing.cpp:1117`), capped by the nozzle diameter and the max layer height.
- `effective_extruder_layer_heights` (`Slicing.cpp:1207`) fills `first_layers` at `Slicing.cpp:1247`. The bed row is the smallest of the heads' first layers. `apply_extruder_layer_height_plan` (`Slicing.cpp:1265`, applying at about 1272–1284) writes it into `initial_layer_print_height`. So under a strategy the preset's own `initial_layer_print_height` is overwritten; the plate presets say what a printer with a 0.2 head gets.
- `multi_nozzle_first_layer_tops` (`Slicing.cpp:1136`) gives each head's first-layer top.
- Derived from the plan: `initial_layer_print_height`, the per-head tops, brim height and per-head first-layer temperature timing (from the commit message; not re-traced).
- Still read from the preset's composed per-head columns: `initial_layer_line_width` (via `Flow::width_at(..., column)`, `LayerRegion.cpp:126`; a run from the bed counts as first layer through `LayerRegion::first_layer_flow`, `Layer.cpp:98`), `initial_layer_speed` and `initial_layer_infill_speed` (`GCode.cpp:10598–10634`; also `GCode.cpp:2207`), `initial_layer_acceleration`, `initial_layer_jerk`, `first_layer_flow_ratio`, `bottom_surface_pattern`. Filament side: `nozzle_temperature_initial_layer`, `close_fan_the_first_x_layers`.

## 2.5 Process, clone against ours (script)

- U1 names are the same set in both trees: 41 non-multi-nozzle files. 17 are stock presets.
- Theirs pins `compatible_printers` to old per-size names; ours has `["Snapmaker U1"]` plus `nozzle_size`. 15 of the 41 are renamed in theirs, with `renamed_from` (example: `0.40mm Standard (0.8)` from the old 0.40/0.48/0.56 names).
- Differences in values (rows in `process_ours_vs_theirs.csv`, 222 rows; line widths compared in mm):
  - `wall_generator` is `classic` in theirs and `arachne` in ours on 40 files (our commit `72f7b766`).
  - `max_bridge_length` 0 and `raft_first_layer_expansion` -1 exist in ours on all 41 and are absent in theirs.
  - `top_color_penetration_layers` differs in 13 presets and `bottom_color_penetration_layers` in 7 (for example 0.4/0.12: theirs 6 and 8, ours 3 and 5; 0.6/0.24 theirs 4, ours 3).
  - `internal_solid_infill_pattern` differs in the two 0.20 presets (theirs `monotonicline`, ours `rectilinear`).
  - Line widths are the same in mm.
- High Flow: in theirs only `0.20mm Standard (0.4)` carries a High Flow speed set (`process_flow_support [standard, high_flow]`; outer 200/500, inner 300/600, ...). Ours has the same speeds plus `print_extruder_id` and `print_extruder_variant` markers.
- Theirs writes two-element speed arrays (`["90","90"]`, `["200","200"]`) on other 0.4 presets with no variant key. Ours has one value there.
- I cannot separate Snapmaker's own edits from our fork's in the untagged differences: I did not diff against the earlier import.

---

# 3. How the code handles them

Line numbers verified against the tree at HEAD `8b48a55b` (same C++ as `62553c9d`) by grep. A `~` marks a range I did not re-open.

## 3.1 Filament variant columns

**Keys**
- `filament_extruder_variant`: defined at `src/libslic3r/PrintConfig.cpp:6651`. `filament_self_index`: `PrintConfig.cpp:6658`.
- Legacy Snapmaker Orca 2.4 keys `filament_flow_support`, `process_flow_support`, `printer_flow_support`, `filament_volume_type`: `PrintConfig.cpp:6507`. They exist so that a file carrying them can still be read.
- `normalize_snapmaker_flow_config` (`src/libslic3r/SnapmakerFlowCompat.cpp:268`) turns them into variant columns and drops them (`drop_snapmaker_flow_keys`, `:262`). A config that already has `filament_extruder_variant` (a shipped preset carrying the marker) is not rewritten (read at `:275–278`).

**Load a preset**
- `extend_default_config_length` (`src/libslic3r/Preset.cpp:290`) pads variant keys to the preset's variant-list length. Called at `Preset.cpp:939`, `:2022`, `:2038`, `:2376`, `:2743`, `:2754`.
- `filament_columns_consistent` (`src/libslic3r/FilamentFlowColumns.cpp:122`) and `filament_repair_columns` (`FilamentFlowColumns.cpp:135`) fix vectors whose width differs from the variant list. Called at `Preset.cpp:813`, `:945`, `:4001`.
- `normalize_filament_values_to_variants` (`PrintConfig.cpp:11865`; called at `PrintConfig.cpp:10328`) spreads a project's per-filament values over their variant columns using `filament_self_index` (for projects saved before a key joined the variant set).

**Store in a project / G-code**
- `PresetBundle::full_fff_config` (`PresetBundle.cpp` about 5509–5800) appends each filament's columns in order and builds `filament_self_index` (`:5695–5710` ~). `filament_settings_id`, `printer_settings_id` and `inherits_group` are written at about `:5757–5799`.

**Pick the column when slicing**
- `PresetBundle::construct_full_config` (`PresetBundle.cpp` ~424–495) collapses columns to the printer's extruder variants.
- `Print::get_config_index` (`src/libslic3r/Print.cpp:5782`; two callers at `:5729`, `:5769`) asks the nozzle-group result for the head's extruder type and nozzle volume type. It calls `get_config_index_base` (`PrintConfig.cpp:728`). That matches the variant string (`Direct Drive Standard` / `Direct Drive High Flow`) and the filament's 1-based id. If nothing matches it takes that id's first column.
- Bed temperature uses `first_filament_variant_column` (`PrintConfig.cpp:10518`).

## 3.2 A tool head finds its size's filament and process

**Filament** (`src/libslic3r/NozzleFilamentPresets.cpp`)
- The `nozzle_size` vendor key is read as `Preset::nozzle_size` (`Preset.hpp` ~92 for the key, ~339 for the accessor; the comment at `Preset.hpp:337` describes the fallback).
- `preset_nozzle_size` (`NozzleFilamentPresets.cpp:399`) uses the tag, then (for a user preset) the system ancestor's tag, then the old per-size `compatible_printers`.
- A family is the same `system_inherits` plus `alias` (`is_family_member`, `:45`).
- `version_for_machine` (`:102`) and `version_for` (`:393`) pick the single family member that restricts printers and fits the size. Two members is logged and gives no version.
- `update_compatible_internal` (`Preset.cpp` ~3929–3967) uses `nozzle_size_on_printer` (`Preset.cpp:3931`, used at `:3967`) to make a tagged preset compatible only if some head carries its size.
- `size_machine` (`NozzleFilamentPresets.cpp:305`) maps a size to the one U1 printer preset.
- `target_for_slot` (`NozzleFilamentPresets.cpp` ~645) decides what a slot holds. A size mismatch blocks slicing (`size_mismatches`, read, not re-grepped).

**Process** (`src/libslic3r/PerHeadProcess.cpp`)
- Per head, a source preset is chosen: a chosen preset, the multi-nozzle hidden preset, or the nearest layer height within the plate's quality class (`head_sources`, read).
- `composed_keys()` (`PerHeadProcess.cpp:30`) lists the 41 keys taken from the head's source:
  - 17 speeds: `initial_layer_speed`, `initial_layer_infill_speed`, `outer_wall_speed`, `inner_wall_speed`, `small_perimeter_speed`, `sparse_infill_speed`, `internal_solid_infill_speed`, `top_surface_speed`, `overhang_1_4_speed` to `overhang_4_4_speed`, `bridge_speed`, `internal_bridge_speed`, `gap_infill_speed`, `support_speed`, `support_interface_speed`.
  - 8 accelerations: `default_acceleration`, `bridge_acceleration`, `initial_layer_acceleration`, `outer_wall_acceleration`, `inner_wall_acceleration`, `sparse_infill_acceleration`, `internal_solid_infill_acceleration`, `top_surface_acceleration`.
  - 7 jerk and junction keys: `default_jerk`, `outer_wall_jerk`, `inner_wall_jerk`, `infill_jerk`, `top_surface_jerk`, `initial_layer_jerk`, `default_junction_deviation`.
  - 9 line widths: `line_width`, `initial_layer_line_width`, `outer_wall_line_width`, `inner_wall_line_width`, `top_surface_line_width`, `sparse_infill_line_width`, `internal_solid_infill_line_width`, `support_line_width`, `bridge_line_width`.
- Every other key stays the selected preset's: layer height, shells, walls, patterns, prime volume, support Z distances and `initial_layer_print_height`.
- A multi-nozzle plate's head prints with the hidden preset named `<plate> (<size> nozzle)`: `multi_nozzle_size_preset` (`PerHeadProcess.cpp:259`). `multi_nozzle_owner_name` is at `:103`. Hiding: `hide_multi_nozzle_size_presets` (`:267`) is called at `PresetBundle.cpp:8469`.
- A chosen height preset changes the head's layer height through `chosen_layer_height` (`PerHeadProcess.cpp` ~290, not re-grepped).

## 3.3 User presets (read only)

**Storage rules**
- A user filament or process preset stores `inherits`, its own keys and, only when it differs from its parent's, its own `nozzle_size` (`Preset::save`, `Preset.cpp` ~870–910; own-size handling at `:893`).
- On load a user preset takes its parent's size tag (`Preset.cpp` ~2021) unless the file names one (~2041).
- A filament saved from a slot is pinned through `printer_to_pin` (`NozzleFilamentPresets.cpp` ~570): the printer preset for the head's size, else the printer preset's parent. With the single printer that is `Snapmaker U1`.

**What is on disk in `~/.config/Snapmaker_Orca_Max/user/default` (read only, nothing changed)**

| Type | Name | Parent | Notes |
|---|---|---|---|
| filament | `Generic PETG - Max` | `Generic PETG` | 8 keys, no `compatible_printers`, no `nozzle_size` (takes the parent's 0.4) |
| filament | `Snapmaker PLA Basic @U1 - Max` | `Snapmaker PLA Basic @U1` | 8 keys, same |
| machine | `Snapmaker U1 (0.2 nozzle) - Max` | `Snapmaker U1 (0.2 nozzle)` | 9 keys. The parent's name now exists only as a `renamed_from` of the single printer |
| machine | `Snapmaker U1 Max` | `Generic PETG` (a filament name) | 192 keys, `printer_model` Snapmaker U1, `printer_variant` 0.2 |

There are no user process presets.

## 3.4 What a project and the G-code header carry

- A project's full config holds the three preset names (`print_settings_id`, `filament_settings_id`, `printer_settings_id`) and `inherits_group` (process, filaments, printer). It also holds `different_settings_to_system` (`PresetBundle.cpp:5798`), `compatible_machine_expression_group`, `print_compatible_printers`, `filament_self_index`, `filament_extruder_variant` (concatenated across filaments) and `filament_map`.
- `printer_settings_id` is written as `NozzleFilament::file_printer_name(...)` (`PresetBundle.cpp:5777`), which gives `Snapmaker U1 (0.4 nozzle)` for the single printer. `printer_variant` is `file_printer_variant` (`:5778`). The printer's `inherits` entry is mapped the same way (`:5721`). Definitions: `NozzleFilamentPresets.cpp:266–290`. `file_printer` is a key in `resources/profiles/Snapmaker/machine/Snapmaker U1.json`.
- `print_compatible_printers` is the print preset's raw `compatible_printers` (`PresetBundle.cpp:5514–5522`), so it holds `["Snapmaker U1"]`.
- The G-code `CONFIG_BLOCK` is the full print config through `GCode::append_full_config` (`GCode.cpp:9512`, called at `:4176` and `:4707`). It erases four transient keys (`print_extruder_source_column`, `print_extruder_source_flow`, `print_extruder_flow_count`, `extruder_layer_height_planned`, around `GCode.cpp:9517–9520`).

**Loading a project with a name the bundle does not know** (`PresetCollection::load_external_preset`, `Preset.cpp:2938`)
1. Looks the name up, then tries `find_preset_renamed` (matches `renamed_from`): `Preset.cpp:2958`, and again at `:3009`, `:3041`, `:3065` for `inherits`.
2. The single printer preset lists the four old per-size names as `renamed_from`, so `Snapmaker U1 (0.4 nozzle)` loads onto it.
3. For a printer with a size table the project's `nozzle_diameter` is taken, and `give_per_head_printer_layout` plus `NozzleFilament::head_setup_config` give the per-head layout (about `Preset.cpp:2968–3000`).
4. A filament or process name that is not found becomes a project-embedded preset named `<name>(file.3mf)` (about `Preset.cpp:3118–3190`).

## 3.5 Command-line refusal for "Snapmaker U1 (0.4 nozzle)" (read, not run)

Location: `src/Snapmaker_Orca.cpp`.

1. `current_printer_name` = the 3MF's `printer_settings_id` (`:1864`), here `Snapmaker U1 (0.4 nozzle)`.
2. `current_printer_system_name` is the last `inherits_group` entry when the printer has an `inherits` (`:1903`), otherwise the same as `current_printer_name` (`:1899`, `:1927`). The single printer has no `inherits`, so it equals `Snapmaker U1 (0.4 nozzle)`.
3. `current_print_compatible_printers` = the 3MF's `print_compatible_printers` (`:1934`) = `["Snapmaker U1"]`.
4. For a pure reprocess (no new printer, no new process) with the "full" configs empty (they only resolve for BBL profiles; see the comment at about `:2940`): `process_compatible = std::find(current_print_compatible_printers, current_printer_system_name) != end` (`:2969`). `"Snapmaker U1 (0.4 nozzle)"` is not in `["Snapmaker U1"]`, so it is false.
5. The only fallback is an **empty** list (`:2970–2973`), which does not apply here.
6. The branch at `:2978` needs a different `new_printer_name`, so it does not apply.
7. `!process_compatible` exits with `CLI_PROCESS_NOT_COMPATIBLE` (`:3006–3009`; message text at `:152`).

The branches with a new printer or new process use `is_compatible_with_printer(...)` (helper `check_compat`, defined at about `:2893`), which also evaluates conditions; they name `new_printer_system_name` or `current_printer_system_name`, so (inferred) the same name mismatch would apply there when the list is non-empty.

The app's own load path never compares these names: it resolves the printer through `renamed_from`. Not run; the claim rests on reading the code above.

## 3.6 The three profile scripts (read)

| Script | Reads | Does |
|---|---|---|
| `scripts/convert_u1_single_printer.py` (246 lines) | Snapmaker's four per-size machine files and all presets | Writes the single `Snapmaker U1` printer (the 0.4 content, `renamed_from` the four old names, `printer_variant` "U1"). Builds the model's `nozzle_sizes` table from the keys in which the four machines differ. Adds `compatible_printers ["Snapmaker U1"]` and a `nozzle_size` tag to process and filament presets. Deletes the four machine files and edits the index. To be run after every Snapmaker merge; a second run changes nothing. After it: `update-index --vendor Snapmaker` and a version bump, a deliberate step by the owner (not run here). |
| `scripts/generate_u1_multi_nozzle_presets.py` (331) | The process dir and the model table in `machine/Snapmaker U1.json` (min/max heights) | Writes the 21 strategy-size, 21 height-size and 6 HQ per-size presets and the 2 HQ plates. Adds High Flow columns to the plates (500/600/600/600). Removes height presets no longer offered. |
| `scripts/sync_u1_generic_filaments.py` (154) | `filament_options_with_variant` from `PrintConfig.cpp`; Snapmaker and OrcaFilamentLibrary filaments | See 1.1. |

`orca_profile_tool.py` (the managed-bundle tool; its `normalize` and `update-index` were deliberately not run, see the header) is described in the `orca-profiles` skill.

## 3.7 Tests, one line each

- `tests/libslic3r/test_nozzle_filament_presets.cpp` (30 cases): census of U1 filament families and sizes, `version_for`, gate and state, `target_for_slot`, filament-to-head mapping, compatibility per head, repair, project keeps presets, blacklist, size labels, user-preset pinning to the one U1 printer, strict slot lists, FilamentFlow user pins.
- `tests/libslic3r/test_per_head_process.cpp` (61): composed key set, quality class, compose layout, vendor sources and rule, project record, wide layout and per-head overrides, High Flow rule, chosen source and flow, width columns, multi-nozzle size and height presets, picker.
- `tests/libslic3r/test_u1_single_printer.cpp` (12): one printer with a size table, size tags and per-size lists, the file boundary (name round-trip with the old name), install/show, head setup per size.
- `tests/libslic3r/test_snapmaker_multi_nozzle_profiles.cpp` (6): Arachne in every U1 process preset, plate listing, hidden size presets, every 0.4 plate has a High Flow column, height presets, HQ.
- `tests/fff_print/test_nozzle_filament.cpp` (1): a slot prints with its size preset's values.
- `tests/fff_print/test_per_head_process.cpp` (31): per-head G-code speeds and accelerations, widths and their validation, High Flow feed rates, prime tower speeds, overrides.

---

# 4. Surprising or inconsistent data

**In our files**
1. Snapmaker PLA Rainbow 0.2 has no `filament_max_volumetric_speed` (resolves to the default). Its 0.4/0.6/0.8 values are 16/22/22.
2. `pressure_advance` of TPU 0.4 is 0.4 against 0.02 on 0.6/0.8 (Generic TPU); Snapmaker PEBA 90A is 0.4/0.11/0.08. (inferred) It may be a typo or deliberate; I cannot tell.
3. Generic TPU 95A HF `fan_min_speed` is 10 at 0.4 and 100 at 0.6/0.8; `overhang_fan_threshold` is 25% against 95%.
4. Snapmaker PETG HF `filament_retraction_length` is 1.5 / nil / 2.5 / 1 across 0.2 / 0.4 / 0.6 / 0.8 with no visible reason.
5. Snapmaker PLA Matte 0.4 has a stray `filament_settings_id: Snapmaker_PLA` inside the preset.
6. Generic PLA High Speed 0.8 carries a `filament_start_gcode` with bed-temperature logic that no other size has.
7. Duplicate ids: process `GP029` is shared by 0.24 Standard on 0.6 and on 0.8, and `GP009` by 0.40 Standard and Strength on 0.8. Filament ids are shared across families (1.1).
8. `0.10mm Color Mixing` (0.4) inherits the 0.12 base: prime volume 27, support Z 0.12, but layer height 0.10 and first layer 0.1 (every other 0.4 stock preset has 0.2 or 0.25).
9. First layer on stock 0.4 goes 0.2, 0.25, 0.2 as layer height goes 0.16, 0.20, 0.24.
10. Hidden height and plate presets keep the nearest base's prime volume and support Z (2.3): a hidden 0.6 preset at 0.16 mm has prime volume 68, a 0.8 one at 0.25 has 90.
11. Our Generic 0.4 filaments differ from the other sizes in `supertack_plate_temp` (0.4: 35/40/0 against 35–70) and in `filament_end_gcode` (empty or `\n` against `; filament end gcode`) in a way no Snapmaker family shows.
12. The sync script sets Generic 0.2 max flow to values below 2 mm³/s (1.8, 1.3, 0.45): the twin's value x 0.9, and the twins are 2 (0.5 for Support For PLA).
13. The user printer `Snapmaker U1 Max` inherits `Generic PETG`, a filament name.
14. Plate `layer_height` is the 0.2 grid, not the 0.4 head's height (2.1).

**In Snapmaker's 02.03.08.00**
15. New presets carry leaked internal comparison text in `filament_settings_id`, in Chinese, for example `对比：U1-pa旧（0.6喷嘴）（复制Bambu generic pa）` on Snapmaker Nylon 0.6 and 0.8, and one on PETG 0.6.
16. Column markers are inconsistent: 10 Generic presets (PLA High Speed 0.2/0.6/0.8, PETG HF 0.2/0.6/0.8, PETG-CF 0.6/0.8, PETG-GF 0.6/0.8) have 2-value vectors with a one-column marker and identical values. `Snapmaker PETG @U1 0.4` has `filament_flow_support ["standard"]` but 18 two-value keys. `Snapmaker PLA Gradient @U1 0.2` has two-value keys and no marker; its `filament_max_volumetric_speed` is `["5","2"]`. `Snapmaker PLA Matte @U1 0.2` is marked `[standard, high_flow]` with one-value vectors. Many 0.4 process presets carry two-value speed arrays with no variant key. My family analysis of theirs misreads the two-column keys of `Snapmaker PETG 0.4` and `Gradient 0.2`; treat those two rows in the theirs CSV files as unreliable.

---

# 5. Method and limits

**Computed by script** (Python 3, `json`, run against the repo files and the clone; scripts were in a session scratchpad, not in the repo):
- all family, size and column counts in section 1 and the key counts;
- the sync-script facts that I read from `scripts/sync_u1_generic_filaments.py`: read, not computed;
- all process counts, the stock key classification, the seven checks and the flow table in section 2;
- the clone comparisons in 1.4 and 2.5.

Method of the scripts: load every `*.json` with `utf-8-sig`; resolve `inherits` chains; split a resolved filament into columns by the length of `filament_extruder_variant` (or of `filament_flow_support` if there is no variant key), and a process preset by `print_extruder_variant`; compare the Standard column (first value) unless a High Flow column is named. A single-element vector is repeated for all columns.

**Read by eye**
- all C++ and script facts in section 3 (greps verified the line numbers; those marked `~` were not re-opened);
- the commit message of `62553c9d`;
- the `~/.config/Snapmaker_Orca_Max/user` files;
- the "typical High Flow ratio" remark and the oddities in section 4 that are not counts.

**Not done**
- The CLI was not run; the refusal in 3.5 is from reading.
- `normalize` and `update-index` were not run, not even on a copy.
- I did not diff the clone against the earlier Snapmaker import, so I cannot say which differences in 1.4 and 2.5 are Snapmaker's own edits and which are our fork's (the Generic max-flow differences and `wall_generator` are known to be ours).
- A first loader bug skipped files with a UTF-8 BOM, which dropped one process file in the clone. Fixed; all comparisons were rerun after the fix.
- My column detection for theirs misreads two presets (section 4, item 16).
- Process line widths in the clone are absolute mm, ours percent; comparisons converted to mm.

---

# 6. Companion CSV files (`doc/u1-preset-inventory/`)

All files are UTF-8, comma separated, with a header row. They were generated from the repo's preset files and the clone by a throwaway script (not in the repo). "Value" columns hold the stored JSON text (lists as JSON).

| File | Rows | What it holds |
|---|---|---|
| `filaments_ours_presets.csv` | 129 | One row per U1 filament preset: family, size, preset, `setting_id`, `filament_id`, `inherits`, `compatible_printers`, `nozzle_size` tag, number of columns, column names, `renamed_from` |
| `filaments_ours_size_diffs.csv` | 3396 | Long format for the 34 multi-size families: family, key, kind (`value differs` / `presence only`), size, preset, Standard-column value |
| `filaments_ours_key_counts.csv` | 61 | Per key: how many families differ in value, and which |
| `filaments_ours_high_flow_diffs.csv` | 263 | Per two-column preset and key where Standard and High Flow differ |
| `filaments_theirs_presets.csv` | 152 | Same as the first file for the clone, plus `status_vs_ours` (new family, new size in existing family, same name, renamed) |
| `filaments_theirs_size_diffs.csv` | 4046 | The size-diff table for the clone |
| `filaments_theirs_key_counts.csv` | 62 | Key counts for the clone |
| `filaments_theirs_second_column_diffs.csv` | 147 | Second-column diffs in the clone (see the caveat in section 4, item 16) |
| `filaments_theirs_vs_ours.csv` | 249 | Per shared (family, size) and key: ours vs theirs value, Standard and second column |
| `process_u1_all.csv` | 90 | All U1 process files: kind, name, file, instantiation, size, inherits, `setting_id`, `compatible_printers`, columns, generator, hidden-at-load, and 31 key values (`a \| b` when two columns differ) |
| `process_stock_variable_keys.csv` | 56 | Key x 18 stock presets, for keys that vary |
| `process_stock_constant_keys.csv` | 77 | Key and the one value shared by all 18 stock presets |
| `process_ours_vs_theirs.csv` | 222 | Per shared non-multi-nozzle U1 preset and key: our raw value vs theirs (line widths compared in mm; raw text shown) |

The most useful lookups: per-family size diffs for one key (filter `filaments_ours_size_diffs.csv` by `key`), the roster of two-column filaments (`filaments_ours_presets.csv`, `columns` = 2), and a preset's full first-layer and speed values (`process_u1_all.csv`).
