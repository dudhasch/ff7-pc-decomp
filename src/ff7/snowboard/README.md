# Snowboard: the snowboard minigame's two course tables

This folder holds no code. It holds the two data-only objects that describe the snowboard courses. The
minigame itself is described in [../snobo/README.md](../snobo/README.md). `Mini_SnowboardInit` (0x007230ef)
calls `Mini_SnowboardInstallCourseDataPointers` (0x00722f60), which points the snowboard state block at one of
the two tables:

| `g_SnoboModeFlag` | game mode | course model | course table |
|---|---|---|---|
| 1 | `GAME_MODE_SNOWBOARD` (0x08) | `for_ev.tmd` | `snobo_course_data1.cpp` |
| 0 | `GAME_MODE_SNOWBOARD_2` (0x0E) | `for_gs.tmd` | `snobo_course_data2.cpp` |

## Where the code is

- `src/ff7/snowboard/`: 2 TUs, no functions.
- Neither TU has a retail `__FILE__` string. They are placed by their retail `.data`
  spans, right after `src/ff7/snobo/snobo_minigame_states_loop.cpp`.
- TU boundaries follow retail's objects.

| TU | `.data` span | used when |
|---|---|---|
| `snobo_course_data1.cpp` | 0x00926500..0x0093a280 | mode flag 1 |
| `snobo_course_data2.cpp` | 0x0093a280..0x0094a340 | mode flag 0 |

## Main structures

Both TUs have the same shape. `Mini_SnowboardInstallCourseDataPointers` stores these addresses in the
`SnowboardRaceState` block ([include/ff7/types/snobo.h](../../../include/ff7/types/snobo.h)):

| object (data1 / data2) | state field | what the code does with it |
|---|---|---|
| `g_SnoboCourse1Header` / `g_SnoboCourse2Header` | `courseHeader` | base of course offsets in `snobo_minigame_states_loop.cpp` |
| `g_SnoboCourse1BlockIndexTable` / `g_SnoboCourse2BlockIndexTable` | `blockIndexTable` | course array |
| `g_SnoboCourse1ActionTable` / `g_SnoboCourse2ActionTable` | `courseDataA` | course array |
| `g_SnoboCourse1EffectPosTable` / `g_SnoboCourse2EffectPosTable` | `effectPosTable` | 12-byte entries indexed by object or event slot (`snobo_effect_record_updates.cpp`, `snobo_effect_hit_callbacks.cpp`) |
| `g_SnoboCourse1AngleTable` / `g_SnoboCourse2AngleTable` | `courseDataB` | course array |
| `g_SnoboCourse1ChainBitTable` / `g_SnoboCourse2ChainBitTable` | `courseDataC` | course array |
| `g_SnoboCourse1ChainIndexTable` / `g_SnoboCourse2ChainIndexTable` | `courseDataD` | course array |
| `g_SnoboCourse1SegmentTable` / `g_SnoboCourse2SegmentTable` | `segmentTable` | course array |
| `g_SnoboCourse1SectionNames[76]` / `g_SnoboCourse2SectionNames[58]` | `sectionNames` | 8-character section names (`"B150000N"`, `"L153006A"`, ...) |

Each TU also holds a table of base pointers and offsets into its own arrays (`g_SnoboCourse1SectionDataTail` and `g_SnoboCourse2SectionDataTail`). Their comments call it the course table. Two more state fields come from
`snobo_minigame_states_loop.cpp`, not from here: `sectionTable` (`g_SnoboCourse1SectionTable` or `g_SnoboCourse2SectionTable`) and `blockTable`
(`g_SnoboCourse1BlockTable` for mode flag 1, zero for mode flag 0).

The declarations are in [include/ff7/api/snowboard.h](../../../include/ff7/api/snowboard.h).

## Data flow

- The tables are static `.data`. No code in the snowboard TUs writes them.
- `Mini_SnowboardInstallCourseDataPointers` picks one set per run. The course code in
  `snobo_minigame_states_loop.cpp` and the effect TUs in [../snobo/](../snobo/README.md) read them through the
  `SnowboardRaceState` fields (for example `courseHeader + ...` in the course code,
  `effectPosTable + slot * 0xC` in the effect TUs).

## Notes

- Most bytes are static course `.data` arrays (`g_SnoboCourse1EffectPosTable`, `g_SnoboCourse2EffectPosTable`, and their trailing binary table spans).
- The course arrays are `unsigned char` or `int` blobs (`g_SnoboCourse1EffectPosTable`); no struct
  describes their records yet; the state fields that point at them are `SnowboardRaceState`'s
  `blockIndexTable` .. `segmentTable` (+0x240..+0x258).
- The section-name strings are kept as `STRING` literals; what the letters and digits encode is not known.
