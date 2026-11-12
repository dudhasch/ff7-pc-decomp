# Submarine: the submarine minigame's level and model data

The submarine minigame is game mode 0x0A (`GAME_MODE_SUBMARINE`). `GameModeDispatcher_Frame` (0x004090e6,
[../src/main/main.cpp](../src/main/main.cpp)) calls `Pfile_SetAssetSourceMode(1)` and
`App_BlankScreenForModeSwitch`, then installs `Submarine_ModuleEnter`, `Submarine_ModuleFrame` and
`Submarine_ModuleExit`, with no mouse or keyboard handler. The run returns to the `GameModeDispatcher_*`
callbacks from the end of `Submarine_ModuleFrame`.

## Where the code is

- `src/ff7/submarine/`: 15 TUs, 80 functions. Twelve TUs hold
  the code, three only data.
- No TU has a retail `__FILE__` string. The twelve code TUs are named after what they contain, and their
  place in the link order is inferred from their neighbours. The three data TUs are placed by their
  retail `.data` spans.
- TU boundaries follow retail's objects.

| data TU | `.data` span | what it holds |
|---|---|---|
| `sub_hud_tables.cpp` | 0x00981000..0x00987018 | nothing: the bytes are literals of the local initializers in `Submarine_InitStateAndTables`, starting with the first 64 x 64 map at 0x00981000, so the TU is empty |
| `sub_terrain_data2.cpp` | 0x0098a6e8..0x00999718 | six level blocks of 0x2808 bytes: map (4,097 bytes), `SubmarineLevelBlock` (2,048 bytes), route table (`int[128][8]`) |
| `sub_ending_tables.cpp` | 0x00999718..0x0099b4d0 | eight tables `g_SubmarineEndingTable0..7` that the model descriptors point at |

### The code

Twelve TUs (0x0077d030..0x007a5060, 80 functions) hold the submarine code:

| TU | what it holds |
|---|---|
| [sub_module_lifecycle.cpp](sub_module_lifecycle.cpp) | the module callbacks, `Submarine_InitStateAndTables`, `Submarine_SelectSubmarineDataBlocks`, the frame limiter |
| [sub_record_update_and_draw.cpp](sub_record_update_and_draw.cpp) | `Submarine_FillRouteRecordTable`, `Submarine_CopyObjectRecordTable`, object records and strips; its `.data` is 0x00987018..0x0098a6e8: its own words, the HUD's initialized globals (0x00987028..0x00987078) and the working level (map, level record in pieces, route table; the four model descriptors, model bounds, two index tables, model vertex data), because `sub_module_lifecycle.obj` references 0x009884f8 before `g_SubmarineEffectActive` and the HUD code |
| [sub_hud_draw_and_update.cpp](sub_hud_draw_and_update.cpp) | the HUD, gauges, alarm, torpedo and enemy audio, screen shake |
| [sub_player_motion_collision.cpp](sub_player_motion_collision.cpp), [sub_record_flight_update.cpp](sub_record_flight_update.cpp) | player controls, motion and grid collision; record orientation, flight states, torpedo launch |
| [sub_grid_init_and_step.cpp](sub_grid_init_and_step.cpp), [sub_grid_camera_batches.cpp](sub_grid_camera_batches.cpp), [sub_flagged_terrain_batches.cpp](sub_flagged_terrain_batches.cpp) | the terrain grid and its batches |
| [sub_view_matrix_build.cpp](sub_view_matrix_build.cpp), [sub_object_emit_batches.cpp](sub_object_emit_batches.cpp), [sub_atlas_quad_emit.cpp](sub_atlas_quad_emit.cpp) | view transform, object triangles, atlas quads |
| [sub_bit_count_tables.cpp](sub_bit_count_tables.cpp) | `Bit_InitLeadingZeroCountTable`, `Bit_InitLeadingOneCountTable`, `Bit_CountLeadingSignBits`; only submarine TUs call them, and the TU sits inside the submarine range |

## Entry points

The data has no functions. These are the functions that read or write it.

| function | address | what it does | called from |
|---|---|---|---|
| `Submarine_ModuleEnter` | 0x0077d030 | opens `sub.lgp` as archive 17, creates the HUD and text TIM render objects, loads eight SFX, picks the level, sets the time limit | module enter (main.cpp) |
| `Submarine_ModuleFrame` | 0x0077df72 | input, simulation and drawing until the frame limiter releases the loop; the module switch at the end of a run | module frame |
| `Submarine_ModuleExit` | 0x0077daed | destroys the render objects, releases the SFX, writes the result | module exit |
| `Submarine_InitStateAndTables` | 0x0077f56d | resets the state and copies the six level blocks of `sub_terrain_data2.cpp` from its local initializers (`Mem_CopyBytesFast` 4,097, 2,048 and 4,096 bytes each) | `Submarine_ModuleEnter` |
| `Submarine_SelectSubmarineDataBlocks` | 0x0077f131 | builds the grid from one level map, fills the working route table, copies the level's actor table | `Submarine_ModuleEnter` |
| `Submarine_FillRouteRecordTable` | 0x0078e503 | fills the working route table from one level's route table, with links (`index << 5`) into the working table | `Submarine_SelectSubmarineDataBlocks` |
| `Submarine_CopyObjectRecordTable` | 0x0078e882 | object records from a level's actor table, each with a route entry and a `g_SubmarineModelDescriptors` entry | `Submarine_SelectSubmarineDataBlocks` |
| `Submarine_RequestModuleSwitchWithSelector` | 0x0077f324 | stores a level selector (above 5 becomes 5) and switches to the submarine callbacks; `Submarine_ModuleEnter` overwrites the selector from the savemap | no caller by name |

## Main structures

| type or global | header | what it holds |
|---|---|---|
| `SubmarineLevelBlock`, `g_SubmarineLevel0Record` .. `g_SubmarineLevel5Record` | [include/ff7/types/submarine.h](../../../include/ff7/types/submarine.h), [include/ff7/api/submarine.h](../../../include/ff7/api/submarine.h) | 2,048 bytes: `header[21]` level parameters, `actors[12][12]` actor definitions, zero tail |
| `g_SubmarineLevel0Map` .. `g_SubmarineLevel5Map` | [include/ff7/api/submarine.h](../../../include/ff7/api/submarine.h) | 64 x 64 digit characters and a NUL |
| `g_SubmarineLevel0RouteTable` .. `g_SubmarineLevel5RouteTable`, `g_SubmarineWorkingLevelRouteTable` | same | `int[128][8]` route rows |
| `g_SubmarineWorkingLevelParams` .. `g_SubmarineWorkingLevelDepth` | same | the working level record at 0x009884f8, written as pieces because code names its header words one by one |
| `SubmarineModelDescriptor`, `g_SubmarineModelDescriptors[4]` | [include/ff7/types/submarine.h](../../../include/ff7/types/submarine.h), the table in `sub_record_update_and_draw.cpp` | vertices, triangles, colors pointers and a count (151 or 105) into `g_SubmarineEndingTable0..7` |
| `g_SubmarineModelIndicesA`, `g_SubmarineModelIndicesB` | [include/ff7/api/submarine.h](../../../include/ff7/api/submarine.h) | not model indices: the square-root and reciprocal square-root tables (192 `unsigned short` entries, 4096 * sqrt(n / 64) and 4096 / sqrt(n / 64) for n = 64..255) of the fixed-point vector normalisers. The file comment says code reads them from index -64 |

## Data flow

1. `Submarine_ModuleEnter` calls `Submarine_InitStateAndTables` first. It overwrites all six level blocks from its
   local initializers, so the `.data` contents of `sub_terrain_data2.cpp` matter only until the first entry.
2. It reads `gSavemap.submarineMissionId` (savemap +0xF01) into `g_SubmarineMissionId`.
   That byte selects the music, the level and the run length.
3. `Submarine_SelectSubmarineDataBlocks` maps selector 0 to level block 5, 1 to 4, ..., 5 to 0. It builds the
   grid (`Submarine_BuildGridRecordsFromCharGrid`, `Submarine_InitGridHeightsFromBytes`) from that map, fills the
   working route table and copies the actor table. It then clamps `g_SubmarineWorkingLevelParams[1]` to 0..15
   and turns `[2]` into a 0..15 value shifted left by 18.
4. `Submarine_ModuleEnter` clamps `g_SubmarineWorkingLevelLimit` to 500 and sizes the grid record slots with it,
   takes the time limit from `g_SubmarineWorkingLevelParams[0]` and sets `g_SubmarineCountdown` to 900 (selector 0)
   or 1200.
5. Each frame `Submarine_ModuleFrame` updates player controls, motion and grid collision, terrain batches,
   object and torpedo records, HUD and audio, then draws the grid, strip, fixed, scene, record and HUD passes
   between `Gfx_BeginSceneCall` and `Gfx_EndSceneCall`.
6. `Submarine_ModuleExit` writes a 16-bit result (1 when `g_SubmarineResultCode` is 1, else 0) to 0x00dc0c3a and
   0x00dc0c3b (savemap +0xF02, +0xF03).

## Named codes

[include/ff7/enums/submarine.h](../../../include/ff7/enums/submarine.h) defines `SubmarineFlightState`, `SubmarineLauncherState`, `SubmarineGridCellFlag`, `SubmarineContactState`, `SubmarineResult`, `SubmarineAtlasPage`, and `SubmarineQuitDialogState`. The submarine TUs also use `SoundCommand`
(`SOUND_MUSIC_PLAY`, `SOUND_SFX_PLAY_CH1`, ...) from [include/lib/enums/sound.h](../../../include/lib/enums/sound.h),
`InputButton` (`INPUT_START`, `INPUT_CIRCLE`, ...) from [include/lib/enums/file.h](../../../include/lib/enums/file.h)
and `RENDER_STATE_*` from [include/lib/enums/graphics.h](../../../include/lib/enums/graphics.h).

## Notes

What reading showed:

- `sub_ending_tables.cpp`: "Ending" is not backed by code. The tables are model data reached through
  `g_SubmarineModelDescriptors`. The file's header comment sits at the end of the file.
- `sub_hud_tables.cpp`: the name says HUD, but the span is the six maps of `Submarine_InitStateAndTables`.
- `g_SubmarineWorkingMap` (0x009874f0) is defined, but no code names it.
- In the code: savemap bytes as `gSavemap.submarineMissionId`..`submarineAudioStateHighByte`; render objects as `g_SubmarineHudRenderObjectA`..`E` and
  `g_SubmarineRadialTerrainScaledBatchNode`; `Submarine_ModuleEnter` keeps its stack frame as `int p[0x844 / 4]` and reads the engine context
  as `*(int *)(ctx + 0x910)`.

## Data-only files

Data runs that their link neighbour never references, each in a file of its own:

| file | span | next to | what it holds |
|---|---|---|---|
| `sub_mode_batches.cpp` | .bss 0x00eed688..0x00f4d6a8 | `sub_record_flight_update.cpp` | the mode batch lists and the large grid buffer |
