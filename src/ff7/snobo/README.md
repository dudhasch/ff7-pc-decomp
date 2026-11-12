# Snobo: the snowboard minigame, its effects and its TMD course renderer

`snobo` is the retail folder name (`C:\FF7\snobo\`) of the snowboard minigame. It runs in two game modes:
`GAME_MODE_SNOWBOARD` (0x08) and `GAME_MODE_SNOWBOARD_2` (0x0E). `GameModeDispatcher_Frame` (0x004090e6,
[../src/main/main.cpp](../src/main/main.cpp)) installs `Mini_SnowboardEnter`, `Mini_SnowboardFrame`,
`Mini_SnowboardExit`, `Mini_SnowboardMouse` and `Mini_SnowboardKeyboard` for both. `Mini_SnowboardEnter` sets
`g_SnoboModeFlag` to 1 for mode 0x08 and to 0 for mode 0x0E. `Mini_SnowboardInit`,
`Mini_SnowboardShutdown`, the course tables and the effect callbacks branch on that flag. The run ends when `Mini_SnowboardUpdate` returns nonzero (`g_SnoboActive` is 0) or when the init
failed: `Mini_SnowboardReturnToField` sets `g_EnginePreviousMode` to 11 (`GAME_MODE_JET_COASTER`, the
coaster's mode), the game mode word to 1 (field)
and requests the `GameModeDispatcher_*` callbacks again.

## Where the code is

- `src/ff7/snobo/`: 18 TUs, 840 functions.
- Two files keep their retail names from `__FILE__` strings: `tmd.cpp` (`C:\FF7\snobo\tmd.cpp`) and `memory.cpp`
  (`C:\FF7\snobo\memory.cpp`). The other 16 are named after what they contain, and their place in the link
  order is inferred from their neighbours.
- TU boundaries follow retail's objects.

| TU | address | what it holds |
|---|---|---|
| `memory.cpp` | 0x0065ec20 | the global `operator new` and `operator delete` (game heap, tagged with this file) |
| `snobo_prim_buffer_support.cpp` | 0x0065ec60 | a text buffer: `Mini_InitPrimBuffer`, `Mini_HighwayAppendPrimFormat` (`vsprintf`), reset, release |
| `snobo_psx_matrix_math.cpp` | 0x0065ed50 | PSX 3x3 matrix and `Matrix30` helpers (`PsxMultiplyMatrix33`, `Matrix30SetRotationY`, ...) |
| `tmd.cpp` | 0x00731aa0 | `SnowboardCourseLoader` (TMD load), `SnowboardDecoder`, `SnowboardTextureLoader`, `SnowboardTmdRenderer` (TMD primitive submission), `SnowboardRaceSpriteRenderer`; the COMDAT of the inline `SnowboardColor::SnowboardColor` (0x00733340, `snobo_primitives.h`) |
| `snobo_clipper.cpp` | 0x00733360 | `SnowboardClipper`: transform, project, clip, emit triangles and quads |
| `snobo_matrix_record_flush.cpp` | 0x00735220 | `Mini_SnowboardFlushMatrixRecords`, `Gfx_SubmitModelRecord` |
| `snobo_effect_callback_select.cpp` | 0x00735370 | picks and calls the effect callbacks for the current mode |
| `snobo_effect_record_updates.cpp` | 0x00735400 | effect node allocation and update functions (78) |
| `snobo_effect_record_updates.cpp`, continued | 0x007388a0 | the event slot spawners (`Mini_SnowboardSlot053SpawnEffectAbove`, ...), `g_SnoboEffectCallbacks`, the effect record update (243 functions) |
| `snobo_effect_hit_callbacks.cpp` | 0x0073bb60 | more effect update functions (145) |
| `snobo_effect_hit_callbacks.cpp`, continued | 0x0073efa0, 0x0073fd70 | the mode-1 ("Alt") slot spawners and record update |

### The main loop, the course and the HUD

The main loop, the state machine, the course and rider simulation and the HUD are five TUs
(0x00722c10..0x00731aa0, 216 functions), ending where `tmd.cpp` begins:

| TU | address | what it holds |
|---|---|---|
| [snobo_module_lifecycle.cpp](snobo_module_lifecycle.cpp) | 0x00722c10 | the module callbacks, `Mini_SnowboardReturnToField`, render-state helpers |
| [snobo_minigame_states_loop.cpp](snobo_minigame_states_loop.cpp) | 0x00722e60 | `Mini_SnowboardInit`, `Mini_SnowboardShutdown`, `Mini_SnowboardUpdate`, the `Mini_SnowboardState*` callbacks, course probing, camera, sound, savemap writes (96 functions) |
| [snobo_script_opcodes.cpp](snobo_script_opcodes.cpp) | 0x0072a170 | the rider action opcodes and rider physics (`Mini_SnowboardRunOpcodeFrame`, `Mini_SnowboardAction*`) |
| [snobo_renderer_hud.cpp](snobo_renderer_hud.cpp) | 0x0072ca90 | `SnowboardRenderer` and `SnowboardHud`: HUD, menus, digits, fades, course emission |
| [snobo_course_assets.cpp](snobo_course_assets.cpp) | 0x0072fe00 | `SnowboardMatrixState`, `SnowboardCourseAssets`: course frames, easing, blending |

The course tables are two data-only TUs in [../snowboard/](../snowboard/README.md).

### Code here that serves other minigames

`memory.cpp`, `snobo_prim_buffer_support.cpp` and `snobo_psx_matrix_math.cpp` sit at 0x0065ec20..0x0065fac0,
right after the highway TUs, far from the rest of this folder. The global `operator new` and `operator delete`
are called by seven highway TUs, `coaster_scene_renderer.cpp` and two snowboard TUs. The matrix
helpers are called by the highway and snowboard TUs. See [../highway/README.md](../highway/README.md).

No TU outside this folder names `SnowboardCourseLoader`,
`SnowboardTmdRenderer`, `SnowboardTextureLoader`, `SnowboardDecoder` or `SnowboardClipper`.

## Entry points

| function | address | what it does | called from |
|---|---|---|---|
| `Mini_SnowboardEnter` | 0x00722c10 | sets the mode flag, matrix layout 0, calls `Mini_SnowboardInit` | module enter (main.cpp) |
| `Mini_SnowboardFrame` | 0x00722ccf | clear, `Input_Update`, begin scene, render states, `Mini_SnowboardUpdate`, end scene | module frame |
| `Mini_SnowboardExit` | 0x00722c52 | clears and presents, `Mini_SnowboardShutdown` | module exit |
| `Mini_SnowboardInit` | 0x007230ef | opens `snowboard-us.lgp`, installs the course tables, creates loader, assets, clipper and renderer | `Mini_SnowboardEnter` |
| `Mini_SnowboardUpdate` | 0x0072381c | the fixed-step loop around the state callback | `Mini_SnowboardFrame` |
| `Mini_SnowboardShutdown` | 0x007235ea | stores results, releases SFX, buffers and objects, closes archive 10 | `Mini_SnowboardExit` |
| `operator new`, `operator delete` | 0x0065ec20, 0x0065ec3a | game heap alloc and free | highway, coaster, field, menu TUs |
| `Mini_InitPrimBuffer`, `Mini_ResetPrimBuffer`, `Mini_ReleasePrimBuffer` | 0x0065ec65, 0x0065ed18, 0x0065ed27 | allocate (`width * height + 1` bytes), rewind, free the text buffer | highway init, frame and exit; `Mini_SnowboardInit`, `Mini_SnowboardShutdown` |
| `SnowboardCourseLoader::load` | 0x00731aa0 | `loadTmd`, then `loadTextures` | `Mini_SnowboardInit` |
| `Mini_SnowboardSelectEffectCallbacks` | 0x00735370 | installs the normal or the Alt effect update, clear and visit callbacks | `Mini_SnowboardInitRunAtCourseEntry` |
| `Mini_SnowboardUpdateEffectNodes` | 0x007353c6 | calls the installed effect update | `Mini_SnowboardEmitCourseChain` |
| `Mini_SnowboardInvokeVisitCallback` | 0x007353e3 | calls the installed visit callback with an event slot | `Mini_SnowboardInvokeRecordVisitCallbacks` |
| `Mini_SnowboardFlushMatrixRecords` | 0x00735220 | submits the queued 0x4C-byte model records through `SnowboardTmdRenderer::model`, then `SnowboardTmdRenderer::emit` | `Mini_SnowboardUpdate` |

About 420 functions are reached through tables; most are the slot spawners in
`g_SnoboEffectCallbacks` (636 entries, many zero) and the effect update callbacks stored in effect
templates (`SnowboardEffectTemplate.update`) such as `g_SnoboHopWaitEffectDef`.

## Main structures

| type or global | header | what it holds |
|---|---|---|
| `SnowboardRaceState`, `g_SnoboStatePtr` | [include/ff7/types/snobo.h](../../../include/ff7/types/snobo.h) | the state block `g_SnoboContextState` (660 bytes), the one type of it: `courseLoader`, `courseAssets`, `courseIndex`, the frame counter `tick`, the course table pointers (`courseHeader` .. `blockTable`), the float accumulators `frameTime`/`framesRun`, `renderFrame` (set for the update's last callback run, the drawn one), `clipper` and the `soundCommand` block |
| `SnowboardCourseLoader`, `SnowboardCourseAssets`, `SnowboardRenderer`, `SnowboardMatrixState` | [include/ff7/snobo_course.h](../../../include/ff7/snobo_course.h) | TMD course model; course frames; HUD and menu renderer |
| `SnowboardTmdRenderer`, `SnowboardTextureLoader`, `SnowboardClipper`, `SnowboardColor` | [include/ff7/snobo_primitives.h](../../../include/ff7/snobo_primitives.h) | TMD primitive submission; 16 textures from `g_SnoboTextureRecords`; 0x120-byte clipper |
| `g_SnoboEffectCallbacks` | `snobo_effect_record_updates.cpp` | 636 `SnowboardCallback` entries, indexed by event slot |
| `g_SnoboEffectRecords` | `snobo_effect_record_updates.cpp` | 32 effect records of 0x90 bytes (4,608 bytes, still `unsigned char`), read through `V(...)` |
| `SnowboardEffect` | [include/ff7/types/snobo.h](../../../include/ff7/types/snobo.h) | one 0x90-byte effect node (two pools of 32, at `g_SnoboEffectRecords` and `g_SnoboAltEffectRecords`): section, two model ids, contact cylinder and responses, rotation and cached matrix, position, update callback, per-kind words (unions), velocities, counters, fade. The meanings come from `Mini_SnowboardUpdateEffectRecords` |
| `SnowboardEffectTemplate` | [include/ff7/types/snobo.h](../../../include/ff7/types/snobo.h) | an effect template (72 bytes): the node's +0x04..+0x4C, which the two allocators copy over a new node. The 41 `g_SnoboEffectDef_*`/`g_Snobo*EffectDef` globals (and `g_SnoboEffectTemplateAtObject`, `g_SnoboLinkedEffectTopDef`, `g_SnoboRaisedEffectDef`) are templates; a trigger fills one's model, rotY and position, then allocates |
| `SnowboardEffectDesc`, `SnowboardEffectPosOffset` | [include/ff7/types/snobo.h](../../../include/ff7/types/snobo.h) | an effect descriptor a trigger passes its spawner: yaw and offset from the object, and up to four parameters (`g_Snobo<Kind>Desc_<addr>`, named by the spawner they go to) |
| `SnowboardSpawnPos` | [include/ff7/types/snobo.h](../../../include/ff7/types/snobo.h) | a spawn position (20.12 fixed point) the F8 and WithId spawners copy into a template's sidePos, height and forward |
| `g_SnoboMatrixRecords`, `g_SnoboMatrixRecordCount` | `snobo_matrix_record_flush.cpp` | the queue of 0x4C-byte model records (0x2600 bytes) |
| `PsxMatrix` | [include/lib/types/graphics.h](../../../include/lib/types/graphics.h) | PSX matrix: nine shorts and three ints, packed to 30 bytes |
| `SnoboMatrix` | same | one more view of a 3x3 short matrix |
| `Copy16`, `Copy30`, `Copy64`, `SnowboardVec3Copy` | same | fixed-size block-copy helpers |

## Data flow

1. `Mini_SnowboardInit` changes to the minigame folder, opens `snowboard-us.lgp` as archive 10 and calls
   `Mini_SnowboardInstallCourseDataPointers`, which points the state block at the course tables of
   [../snowboard/](../snowboard/README.md).
2. It creates the `SnowboardClipper`, sets up the projection, and loads the course with `SnowboardCourseLoader::load`:
   `for_ev.tmd` when the mode flag is 1, `for_gs.tmd` when it is 0. `SnowboardCourseLoader::loadTmd` reads the file from
   archive 10 and rebases the object offsets; `SnowboardTextureLoader::loadTextures` creates 16 render objects.
3. With mode flag 0 it reads the savemap byte at 0x00dc0b5b (savemap +0xE23, `gSavemap.snowTempVar`)
   to pick course 0, 1 or 2. It creates `SnowboardCourseAssets` and `SnowboardRenderer`, the text buffer and the SFX. Mode
   flag 1 also sends five raw sound commands and starts in `Mini_SnowboardStateFadeIn`; mode flag 0 starts in
   `Mini_SnowboardStateBeginFadeIn`.
4. Each frame `Mini_SnowboardUpdate` adds the elapsed time (`Mini_SnowboardAccumulateFrameTime`), reads input,
   builds the course (`SnowboardRenderer::buildCourse`), and calls the state callback in `g_SnoboStateHandler`
   until the processed frames catch up. Only the last call has the render flag set. It then flushes the model
   records, updates the HUD and, in the race state, runs the pause menu.
5. `Mini_SnowboardInvokeRecordVisitCallbacks` passes event slots from the state block's `sectionTable` to
   `Mini_SnowboardInvokeVisitCallback`.
   `Mini_SnowboardVisitEventSlot` marks the slot visited and calls `g_SnoboEffectCallbacks[slot]`,
   which spawns effect records. `Mini_SnowboardUpdateEffectRecords` retires old ones, runs their callbacks, queues
   their model records and tests them against the rider position, changing the rider state on contact.
6. `SnowboardTmdRenderer` (`tmd.cpp`) submits the TMD primitives to the graphics batches (`Gfx_EmitPrimitiveBatchList`,
   `AcquireBatchNode`). `SnowboardClipper` transforms, clips and emits triangles and quads, by its method names.
7. `Mini_SnowboardShutdown`, mode flag 0: `Mini_SnowboardStoreBestRecordToSavemap` writes three times and three
   scores to 0x00dc0b4c (savemap +0xE14..+0xE22), then MIDI stops. Mode flag 1: sound command 0xC0 and
   `Mini_SnowboardStoreCourseResult` writes one byte to 0x00dc0aaa (savemap +0xD72). Both release the SFX, the
   text buffer and the four objects, close archive 10 and restore the folder.

## Named codes

[include/ff7/enums/snobo.h](../../../include/ff7/enums/snobo.h) defines `SnowboardModelDrawMode`, `SnowboardMode`, `SnowboardPauseMenuState`, `SnowboardPauseMainRow`, `SnowboardResultRow`, `SnowboardRiderAction`, `SnowboardActionTransition`, and `SnowboardSplitSpawn`. `Mini_SnowboardEnter` also tests `GAME_MODE_SNOWBOARD` from
[include/ff7/enums/main.h](../../../include/ff7/enums/main.h). The render-state helpers use `RenderState`
(`RENDER_STATE_*`) from [include/lib/enums/graphics.h](../../../include/lib/enums/graphics.h).

## Notes

What reading the code showed:

- `snobo_effect_record_updates.cpp` and `snobo_effect_hit_callbacks.cpp` read the effect records through a
  `V(type, base, offset)` macro (about 70 lines each).
- The slot spawners are named by slot number (`Mini_SnowboardAltSlot236SpawnTemplateF0AndQuad`); what each
  event is in the game is not known.
- The object at offset 0 of the state block is a `SnowboardCourseLoader` in `Mini_SnowboardInit` and a `SnowboardTmdRenderer`
  in `Mini_SnowboardFlushMatrixRecords`.
- `tmd.cpp`: `Mini_SnowboardReleaseTmdModel` calls `SnowboardTextureLoader::releaseTextures` and `SnowboardCourseLoader::freeTmd`; `SnowboardCourseLoader::~SnowboardCourseLoader` calls it.
- `Mini_SnowboardEmitCourseChain` reads its course node through `S`/`I`/`C(off)` macros over an `int`.
- The sound commands in `Mini_SnowboardInit` (0xBA, 0xB8, 0xBC, 0x10, 0xC0) are literals.
