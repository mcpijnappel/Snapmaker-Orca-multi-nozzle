# Snapmaker Orca Max: todo

Last updated 2026-11-13. This is the working list for the `snorca-max` branch: what is open, in which
order, and the findings needed to pick an item up. Findings marked "read from code" were not confirmed by
a slice or a print.

## Recently done

| What | Commits |
|---|---|
| One size-less "Snapmaker U1" printer with a nozzle size table; tool heads look up their own size | `8342e78a`, `bb1b783b`, `7d6242ac` |
| Every tool head lays its own first layer from the bed under a strategy; finest nozzle first; first-layer warnings; elephant foot per region | `62553c9d` |

Not yet checked on screen after these: the setup wizard and other screens with the size-less U1, the first
layer field and the per-head first layer lines. The new texts are not in the translation files.

## In progress: preset overhaul

Decided 2026-11-13: keep only our own preset format for the U1's filaments and process presets.

- **Filaments:** one entry per material, shown under one name. Your own changes (temperature and the like)
  apply on every nozzle size; size-specific values such as the flow limit are kept per size in that entry.
- **Process:** a few intents plus the multi-nozzle strategies, with per-size speeds in the size table and
  line widths from a formula.
- **Stock files:** one-way import. Stock and older Max projects still open; projects saved by Max no longer
  have to open in stock Snapmaker Orca.
- **Updates:** a converter is the only way Snapmaker's profiles enter the repo.

The inventory of the current presets is done: `doc/u1-preset-inventory.md`, with the per-preset tables as
CSV files in `doc/u1-preset-inventory/`. Next: design and implementation together in one session.

What the inventory found (43 filament materials in 129 presets, 34 of them in more than one size; 90
process files: 18 stock, 7 Multi-nozzle plates, 42 hidden, 23 bases):

- **Line widths** are exactly nozzle + 0.02 mm. Exceptions: 0.45 mm on 0.4 for inner wall and sparse infill,
  and for top surface at 0.24-0.28 mm layers; the first-layer width is 125% of the nozzle on 0.2 and 0.4.
- **Support top and bottom Z distance** are min(layer height, 0.20), with one stock exception (0.4
  "0.10mm Color Mixing" has 0.12).
- **Speeds** need a table, not a formula. 0.2, 0.6 and 0.8 share one set with a handful of exceptions; on
  0.4 they step with layer height (0.08-0.12 slow, 0.16 and up fast) plus the High Quality preset. A flow
  cap of about 25 mm³/s fits 0.4 only at 0.24 and 0.28 mm layers.
- **Accelerations** come in two sets, 4000/2000 and 10000/5000. High Quality presets and 0.4 at 0.08-0.12 mm
  use the slow set, everything else the fast one.
- **Prime volume** is about 225 x layer height in the stock presets.
- **First layer:** width and speeds are a few numbers per size; the height is not (it varies per preset).
- **Hidden and plate presets carry values of the base they inherit**, not of their own layer height: a
  hidden 0.6 preset at 0.16 mm has prime volume 68, plates have the 0.4 stock prime volume at 0.05-0.14 mm
  layers, and support Z deviates the same way. The overhaul should compute these.
- **Data oddities** in our files and in Snapmaker's 02.03.08.00 are listed in section 4 of the inventory
  (missing flow limit on PLA Rainbow 0.2, duplicate preset ids, leaked comparison text in Snapmaker's new
  presets, inconsistent column markers).

## Faults to settle

1. **Overhang walls under a combined run are classified too late** (read from code).
   The detector compares a run with the raw layer one grid step below, so it sees one step of overhang where
   the real overhang is the whole run. Example: 0.8 head, Normal, 45° slope: real 0.40 mm (44% of the bead),
   detected 0.10 mm. Overhang speed, fan and flow start too late; only where the surface grows upward.
   Fix: compare against the previous run's printed shape in `LayerRegion::make_perimeters`
   (`src/libslic3r/LayerRegion.cpp:310-323`). No test covers it. Confirm first with test slices 1-3 below.
2. **Seams can jump between runs** (read from code). A coarse region's seam is chosen once per run with
   nothing linking it to the run above or below (`find_seam_string`, `src/libslic3r/GCode/SeamPlacer.cpp:1239-1296`;
   dummy point at `:469-473`). Fix: skip layers where the region prints nothing so run tops form one string.
   Confirm first with test slices 6-7.
3. **Print-by-object lift above the printable height.** Snapmaker fixed the U1 tool-change and end G-code
   in their commit `e023ce577`; ours still has the older G-code. Data only.
4. **Bridge anchors on a ring a run did not print** (unverified). Bridge direction is detected against the
   raw layer below (`src/libslic3r/LayerRegion.cpp:1049`), which can include area the previous run left out.
   Test slice 4.
5. **Command-line slicer refuses our own projects.** A project names the printer "Snapmaker U1 (0.4 nozzle)"
   (written on purpose for stock compatibility) and the command line does not map that name as the app does:
   `src/Snapmaker_Orca.cpp:2969` looks the name up in the process preset's printer list, which holds
   "Snapmaker U1" (read from code, not run; inventory section 3.5). May resolve itself in the preset overhaul, which drops two-way compatibility.

## Profiles

The overhaul replaces the converter, so do the import through the new one unless it is needed sooner.

6. **Converter problems on Snapmaker 02.03.08.00** (`scripts/convert_u1_single_printer.py`, run on a scratch copy):
   High Flow is read from `extruder_variant_list`, which Snapmaker's files lack (use `printer_flow_support`);
   our variant keys are dropped on a plain overwrite; three per-size default process names point at presets
   that do not exist ("0.10 Standard ...", without "mm"); the new per-size `default_filament_profile` lands in
   the size table; the bundle version is not bumped.
7. **Import 02.03.08.00.** New there: 28 filaments renamed with the nozzle size in the name (with
   `renamed_from`), 56 new presets (Snapmaker Nylon, PETG, PLA Gradient, Marble and Starlight; Generic PA,
   PA-CF and PVA at 0.6 and 0.8), value changes on TPU, PETG HF, ASA. Decisions needed:
   - nozzle types swapped in their files (0.4 stainless, 0.6 hardened, the reverse of ours): take it or not;
   - whether the real Snapmaker PETG replaces our scaled Generic PETG;
   - whether their new Generic PA, PA-CF and PVA values win over `scripts/sync_u1_generic_filaments.py`;
   - whether the per-size default filament joins the size table;
   - the new bundle version (ours is 02.03.03.21).
   Their profiles now live in a separate repository (github.com/Snapmaker/snapmaker-profiles); do not merge
   Snapmaker `main`, it deleted the tracked profile folder.
8. **Preset fixes from Snapmaker:** `314075253` (renamed filament presets when repairing selections) and the
   preset items of `22fae6ccb` (`renamed_from` on import, empty slots in `validate_presets`).

## Multi-nozzle quality

9. **Walls and shells per nozzle.** Per-head values cover speeds, accelerations and line widths only; a 0.2
   head on a 2-loop preset gets about 0.5 mm of wall. Bambu Studio 2.9.1 sets per diameter (0.2/0.4/0.6/0.8):
   wall loops 4/2/2/2, top shell layers 7/5/3/3, bottom shell layers 5/3/3/3, support Z gap 0.1/0.2/0.2/0.2,
   and counts shell layers in printed layers, not grid layers. Suggested: loops = max(2, round(0.8 mm / line
   width)), shells as a thickness.
10. **Slice a one-size plate directly at that size's height.** A 0.6-only plate under Normal slices at 0.1
    and combines three layers; a direct 0.3 mm slice gives a better surface on curves. Test slice 8.
11. **Show where each head's value comes from**, and show size-derived settings as read-only "Auto".
12. **Per-part quality choice** instead of "Layer height steps" (-3..+3): the same strategy names or "same as
    plate", only heights that fit the plate's grid, bringing that height's speeds with it.
13. **Drop stale overrides when a head's nozzle changes**, with a notice and undo.
14. **Pre-flight checks with one-click fixes** before slicing (part on a head that cannot print its layer,
    filament with no version for a head's size, part off the bed).
15. Ideas, not planned: comparing two slices (time, filament, changed settings); filament auto-match per head
    by type and nearest colour when the printer reports what is loaded; a G-code editor with placeholder
    completion.

## Small upstream picks

16. **OrcaSlicer crash fixes** (fork point `a5413efc3`, 2026-10-01; Orca is about 1,800 commits ahead, a trial
    merge conflicts in 167 of 377 shared files, so pick, do not merge):
    apply cleanly: `24cf1c6bc` (user printer whose parent is not loaded), `7ac46c9a6` (lightning infill
    assert), `1466c0e57` (thread pool); need a hand port: `d7534d7fb`, `eda4062d6`, `018c4f49e` (partly
    present), `b00237807`, `3c72223aa`, `71c9f556f`.
17. **Snapmaker Orca fixes:** wipe tower crash `0a75d9638`, tree support leak `9bf2f18ea`.
18. **OrcaSlicer's "copy to all tools" button** for per-nozzle settings (`34de2b49d`); expect `Tab.cpp` conflicts.

## Checks

19. Look at the wizard, the first layer field and the per-head lines in the app (see "Recently done").
20. A dump of which filament and process presets each head size gets, compared after every profile import.
21. A few fixed multi-head slices compared against a reference before each AppImage.
22. Test the printer connection against the U1. Snapmaker replaced the old connection paths with their
    Connect gateway; the repositories cannot tell whether the old paths keep working with current firmware.

### Test slices for faults 1, 2, 4 and item 10

Normal strategy unless stated. Control for each: the same model on a stock single-nozzle preset at the run
height (0.3 mm for 0.6, 0.4 mm for 0.8), strategy Off. Compare `;TYPE:Overhang wall` counts, `;HEIGHT:`
per layer, outer wall feed rates and the XY extent of outer walls.

1. Growing frustum, 45° (diameter 10 to 30 mm over 10 mm), 0.6 head: wall radius per Z, overhang walls.
2. The same frustum flipped (shrinking), 0.6 head: no overhang walls expected.
3. Steep growing frustum, 60° from vertical, 0.8 head, also Super coarse: missing overhang speed and fan lines.
4. Mushroom (10 mm stem, cap flaring 45° to 30 mm), 0.6 head: bridges on the first cap layers and their anchor.
5. The frustums with auto support at 30° and 45°, 0.6 head: support area per layer, gap under the run.
6. Cylinder (20 mm diameter, 20 mm high), seam Aligned, 0.6 head: seam angle per printed layer.
7. Two-part cube, left half on a 0.2 head, right half on a 0.6 head, seam Aligned: coarse seam across runs.
8. A 0.6-only plate under Normal against a stock 0.3 mm slice: stair-steps on curves, slicing time.

## Parked

- A full merge with OrcaSlicer or Snapmaker Orca `main`, and Snapmaker's Connect gateway.
- 0.6 High Flow, until such a hotend exists for the U1.
- The single-printer layout for the J1, A-series and Artisan.

## How other slicers solve mixed nozzle sizes (for reference)

- **Bambu Studio 2.9.1** (H3C only): the same principle as ours, one fine grid and thick extrusions per
  several grid layers. It keeps one printer preset per size plus a project switch. Its thick extrusion takes
  the shape of the band's top layer and can overhang by the whole band; ours prints the shape common to the
  run's layers and never prints over air. Speeds for a 0.8 region come from the 0.4 preset; ours uses values
  per size.
- **OrcaSlicer main:** one hand-made printer preset per size combination (only "0.4+0.6" exists), one layer
  height per object, a warning for a prime tower with mixed sizes.
- **PancakeSlicer:** OrcaSlicer's engine behind a server and web UI, single extruder only. Useful ideas:
  values computed from a rule table, speeds with a visible flow cap, and a dump of evaluated compatibility
  as a profile check.
