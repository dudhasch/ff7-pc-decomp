# Coaster: the Speed Square shooting ride

This folder holds the Gold Saucer roller-coaster shooting game: the car rides a fixed track, the player moves a reticle and fires a beam at targets, and the score goes back to the savemap. It is game mode `GAME_MODE_JET_COASTER` (0x0B). `GameModeDispatcher_Frame` (0x004090e6) in [../src/main/main.cpp](../src/main/main.cpp) installs `Mini_CoasterEnter`, `Mini_CoasterExit`, `Mini_CoasterFrame`, `Mini_CoasterMouse` and `Mini_CoasterKeyboard` through `RequestModuleSwitch`. Those five callbacks live in `coaster_module_entry_frame.cpp` (see below). The ride leaves from `Mini_CoasterFrame`: when it ends, `Mini_CoasterStopMusic` and `Mini_CoasterReturnToField` switch back to the `GameModeDispatcher_*` callbacks.

## Where the code is

- `src/ff7/coaster/`: 13 TUs, 117 functions.
- 1 file keeps its retail name from a `__FILE__` string (`C:\FF7\coaster\psxdata_c.cpp`). The other 12 are named after what they contain, and their place in the link order is inferred from their neighbours. `coaster.cpp` and `coaster_helpers.cpp` may be the original names (unconfirmed); the other file names describe their first function.
- TU boundaries follow retail's objects.

| TU | what it holds |
|---|---|
| `coaster.cpp` | data load, `coaster.lgp`, music and the sound-command helpers, the `CoasterPsxDataSet` instance |
| `psxdata_c.cpp` | `CoasterPsxDataSet`: loads `xbin.bin` and `xbinadr.bin` from LGP slot 9 |
| `coaster_scene_draw_camera.cpp` | `Mini_CoasterInit`, actor draw and reticle hit test, rail and scenery streams, camera matrix, track sampler, score popup |
| `coaster_hud_draw_passes.cpp` | HUD passes and `Mini_CoasterShutdown` |
| `coaster_track_tables_advance.cpp` | track tables, `Mini_CoasterAdvanceRide`, rail-set selection |
| `coaster_actor_pool_spawn.cpp` | actor pool, spawn script, `Mini_CoasterUpdateActors`, beam damage |
| `coaster_track_stream_lists.cpp` | track cursor and the streamed rail and scenery lists |
| `coaster_scene_node_pool.cpp` | scene-node pool |
| `coaster_clip_plane_tests.cpp` | the two clip planes and point and sphere tests |
| `coaster_helpers.cpp` | model pools, models built from templates |
| `coaster_input_poll.cpp` | `Mini_CoasterPollInput` |
| `coaster_scene_renderer.cpp` | the `CoasterRenderer` class: textures, projection, rail, scenery, reticle, beam and HUD drawing |

### The module callbacks

`coaster_module_entry_frame.cpp` (0x005e8a70..0x005e9150) holds the module callbacks,
camera and viewport setup, `Mini_CoasterReturnToField` and `Mini_CoasterTick`. It ends where `coaster.cpp`
starts.

Other folders use the same short names for other things (`HighwaySkelList::load` and `HighwayRenderOwner::drawNumber` in highway, a `drawNumber` call in `snobo_renderer_hud.cpp`, a parameter named `load` in `condor_engine_frame_render.cpp`); none of them reaches the coaster. `CoasterRenderer::load` is called only by the `CoasterRenderer` constructor.

## Entry points

| function | address | what it does | called from |
|---|---|---|---|
| `Mini_CoasterEnter` | 0x005e8d49 | camera (`Mini_CoasterSetupCamera`), viewport by UI scaling mode, render states, then `Mini_CoasterInit` | installed enter callback |
| `Mini_CoasterInit` | 0x005e98e0 | changes to the coaster directory, `Mini_CoasterLoadData`, creates the `CoasterRenderer`, selects rail sets 0 and 1, starts the music | `Mini_CoasterEnter` |
| `Mini_CoasterLoadData` | 0x005e91b5 | opens the archive, resets lists and pools, resolves the track, builds 100 models, sets speed 0x2710 and score 0 | `Mini_CoasterInit` |
| `Mini_CoasterFrame` | 0x005e8e7e | ends the ride, or runs the catch-up ticks and one drawing tick inside a scene | installed frame callback |
| `Mini_CoasterTick` | 0x005e9051 | one simulation step, see Data flow | `Mini_CoasterFrame` |
| `Mini_CoasterReturnToField` | 0x005e8e0b | writes the score to the savemap and requests the dispatcher callbacks | `Mini_CoasterFrame` |
| `Mini_CoasterExit` | 0x005e8dd8 | clears, presents, clears, `Mini_CoasterShutdown` | installed exit callback |
| `Mini_CoasterShutdown` | 0x005ea847 | deletes the renderer, closes LGP slot 9, releases SFX and PSX data, restores the directory | `Mini_CoasterExit` |
| `Mini_CoasterMouse`, `Mini_CoasterKeyboard` | 0x005e9108, 0x005e910d | empty; the keyboard switch has no live case | installed |

## Main structures

| type or global | header | what it holds |
|---|---|---|
| `CoasterPsxDataSet`, `g_CoasterPsxDataSet` | [include/ff7/coaster_psxdata.h](../../../include/ff7/coaster_psxdata.h) | five words: the `xbin.bin` buffer and its size, the first address-table entry, the rebased `xbinadr.bin` table, a loaded flag. Built by a VC5 dynamic initializer in `coaster.cpp`. |
| `CoasterRenderer`, `g_pCoasterRenderer` | [include/ff7/coaster_renderer.h](../../../include/ff7/coaster_renderer.h) | render objects for `score.tim`, `co1.tim`, `waku2.tim`, `pause.tim`, `rail.tim`, `sight.tim`, `beam1..3.tim`, a fade object, and two matrices |
| `CoasterActor`, `CoasterObjectState` | [include/ff7/types/coaster.h](../../../include/ff7/types/coaster.h) | one actor: position, rotation, behaviour state (`type`, `hp`, `params[20]`), animation path, scene node, bounding box. Used in `coaster_actor_pool_spawn.cpp`. |
| `CoasterSpawnScriptRecord`, `CoasterKillAward` | [include/ff7/types/coaster.h](../../../include/ff7/types/coaster.h) | spawn-script record and hit-kill point award |
| `CoasterHalfSpaceState`, `CoasterNode`, `CoasterModelRecord` | [include/ff7/types/coaster.h](../../../include/ff7/types/coaster.h) | clip planes, scene nodes, the pooled 0x20-byte model record |
| `g_CoasterScore`, `g_CoasterRideSpeed`, `g_CoasterRidePosition`, `g_CoasterTrackCursor` | [include/ff7/api/coaster.h](../../../include/ff7/api/coaster.h) | score; ride speed; track position (speed is added each tick); track progress counter that drives the spawn script |

## Data flow

1. `Mini_CoasterOpenArchive` opens `coaster.lgp` as LGP slot 9, loads `xbinadr.bin` and `xbin.bin` into `g_CoasterPsxDataSet` (0x00c3f738) and loads the 23 sounds of `g_CoasterSfxTable`. `Mini_CoasterResolveTrackTables` takes the track data and the rail-set table from address-table entries 6 and 5; `Mini_CoasterSelectRailSet` uses entry 4.
2. The `CoasterRenderer` constructor loads the TIM textures through `CoasterRenderer::load` (`Gfx_CreateRenderObject` with LGP slot 9). `Mini_CoasterStartMusic` plays song 0x37.
3. `Mini_CoasterAccumulateStepTarget` turns elapsed timestamp time into 60 ticks per second, at most 32 per frame. `Mini_CoasterFrame` runs the owed ticks with `g_CoasterDrawThisTick` = 0, then one more with 1; the draw helpers draw only when it is 1.
4. `Mini_CoasterTick`: `Mini_CoasterPollInput`; unless paused, `Mini_CoasterBuildCameraMatrix`, `Mini_CoasterAdvanceRide` (samples the track; the slope changes the speed within 43000..120000), `Mini_CoasterAdvanceTrackCursor`, the rail and scenery streams, the score popup, `Mini_CoasterRetireStreamedEntries`, `Mini_CoasterUpdateActors` (spawn script, behaviours by `type`, beam hits) and `Mini_CoasterDrawHud`. When paused it draws `Mini_CoasterDrawPausedHud`. It then sends the engine and beam loop volumes (`Mini_CoasterUpdateLoopVolumes`).
5. Input: left, right and the masks 0x1000 and 0x4000 move the reticle (x 0..0x140, y 0..0xF0); `INPUT_CIRCLE` (0x20) fires the beam and its loop sound; `INPUT_START` (0x800) toggles `g_CoasterPaused`. When `g_CoasterRideMode` is 0 the same buttons tune the scenery fade distance range (`g_CoasterFadeNearDepth`/`g_CoasterFadeFarDepth`) and speed instead; `Mini_CoasterResolveTrackTables` sets it to 1.
6. The ride ends when the actor behaviour 0xFD has faded out for 0x80 frames and set `g_CoasterRideFinished`, or when four times the progress counter passes `g_CoasterTrackLength` (the first word of the track data) minus 16. `Mini_CoasterReturnToField` writes the score bytes to 0x00dc0a3e and 0x00dc0a3f (savemap +0xD06, +0xD07), sets `g_EnginePreviousMode` to 0xB and `g_EngineGameMode` to 1.

## Named codes

[include/ff7/enums/coaster.h](../../../include/ff7/enums/coaster.h) defines `CoasterBehaviour` (the actor behaviours dispatched in `Mini_CoasterUpdateActors`) and `CoasterKillType`; the callbacks TU also uses `RenderState` from [include/lib/enums/graphics.h](../../../include/lib/enums/graphics.h).

## Notes

- The `.bss` globals `DAT_00c3f700`/`DAT_00c3f724`/`DAT_00c3f728` keep their linker names for VC5 `.bss` hash ordering and are read through `#define` aliases (`g_CoasterPsxDataSet`, `g_CoasterCameraMatrixBuf`, `g_CoasterScorePopupState`, ...) in [include/ff7/api/coaster.h](../../../include/ff7/api/coaster.h).
- Sound commands are written through `g_pCoasterSoundBuffer` (`CoasterSoundCommand *`).
- `Mini_CoasterDrawPausedHud` is drawn while paused (`g_CoasterPaused`), and `Mini_CoasterSubmitRendererBatches` submits every queued batch list when `g_CoasterDrawThisTick` is set.

## Data-only files

Data runs that their link neighbour never references, each in a file of its own:

| file | span | next to | what it holds |
|---|---|---|---|
| `coaster_clip_state.cpp` | .bss 0x00c3f788..0x00c3f870 | `coaster.cpp` | the clip rectangle (from its top edge) and the saved directory |
