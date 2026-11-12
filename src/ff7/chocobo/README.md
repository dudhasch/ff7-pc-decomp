# Chocobo: the Gold Saucer chocobo race

This folder holds the chocobo race minigame: the betting board, the race itself with six racers on one of two courses, and the result screen. It is game mode `GAME_MODE_CHOCOBO_RACE` (0x07). `GameModeDispatcher_Frame` (0x004090e6) in [../src/main/main.cpp](../src/main/main.cpp) installs `Mini_ChocoboEnter`, `Mini_ChocoboExit`, `Mini_ChocoboFrame`, `Mini_ChocoboMouse` and `Mini_ChocoboKeyboard` through `RequestModuleSwitch`. The race leaves when `g_ChocoboExitFlag` is set: `Mini_ChocoboFrame` then calls `Mini_ChocoboReturnToField`, which sets `g_EnginePreviousMode` to 7 and `g_EngineGameMode` to 1 and switches back to the `GameModeDispatcher_*` callbacks.

## Where the code is

- `src/ff7/chocobo/`: 18 TUs, 153 functions, all of them the race.
- 4 files keep their retail names from `__FILE__` strings (`C:\FF7\chocobo\ch_app.cpp`, `ch_init.cpp`, `ch_chr.cpp`, `ch_ddraw.cpp`). 12 are named after their first function, and their place in the link order is inferred from their neighbours. 2 are data-only objects.
- TU boundaries follow retail's objects.

| TU | what it holds |
|---|---|---|
| `ch_app.cpp` | module callbacks, UI scaling config, camera setup, track and module teardown |
| `ch_init.cpp` | savemap read, render and asset init, `chocobo.wat` loader, course vertex tables |
| `ch_chr.cpp` | racer skeletons and animations (`*.hrc`, `*.anm`), course sprite objects |
| `ch_ddraw.cpp` | DirectDraw interface cache, surface lock, blit and colour-key helpers |
| `chocobo_engine_frame_sprites.cpp` | frame pacing, `Mini_ChocoboTick`, the draw pass, sprite and primitive emitters, the race state machine |
| `chocobo_racer_state_update.cpp` | building the six racers, ranking, movement along the route |
| `chocobo_bet_and_result_screens.cpp` | betting board, result screen, fade overlay (the file name comes from its first, empty function) |
| `chocobo_race_camera.cpp` | race camera, scenery animation step |
| `chocobo_course_render_objects.cpp`, `chocobo_hud_render_objects.cpp` | fixed and HUD render objects, course triangle and quad submission |
| `chocobo_race_hud_submit.cpp` | `Mini_ChocoboSubmitRaceHud` |
| `chocobo_input_latches.cpp` | the fourteen input latches |
| `chocobo_course_sounds.cpp` | course sound lists |
| `chocobo_heading_angles.cpp`, `chocobo_palette_load.cpp`, `chocobo_variant_table_select.cpp` | heading from XZ, palette table, one variant table getter |
| data in `chocobo_bet_and_result_screens.cpp` and `chocobo_engine_frame_sprites.cpp` | HUD layout tables from 0x0097a4f8 (first file); bet choice tables, prize and jockey names from 0x0097c8a8 (second file) |

### Neighbouring code

The adjacent submarine TUs (`0x0077d030..0x007a5060`) live in [ff7/submarine](../submarine/README.md) as `sub_*.cpp`, and the adjacent credits TUs (`0x007a5060..0x007a98f0`) live in [ff7/credits](../src/credits/README.md) as `credits_*.cpp`.

`mem_fill_copy_routines.cpp` and `mem_timestamp_int64_math.cpp` are generic memory and timestamp helpers that the race, submarine and credits TUs call.

## Entry points

| function | address | what it does | called from |
|---|---|---|---|
| `Mini_ChocoboApplyUiScalingModeConfig` | 0x0076d320 | picks the viewport, HUD layout tables and number drawer (`Mini_ChocoboDrawNumber1x` or `2x`) for UI scaling mode 0, 1 or 2 | `App_InitSubsystems` (0x004089c5) |
| `Mini_ChocoboRaceSetAssetDirPath` | 0x0076b850 | copies `GetMinigameDirForChocobo()` into `g_ChocoboAssetDirPath` | `App_InitSubsystems` |
| `Mini_ChocoboEnter` | 0x0076d597 | sets `g_ChocoboRetailFlag`, render flags, frame time (context +0x30 / 30), caches DirectDraw, then `Mini_ChocoboSetupCamera` | installed enter callback |
| `Mini_ChocoboSetupCamera` | 0x0076d6df | runs `Mini_ChocoboStartRaceSequence` and sets the GTE camera matrix | `Mini_ChocoboEnter` |
| `Mini_ChocoboStartRaceSequence` | 0x0077c430 | `Mini_ChocoboRaceInitFromSavemap`, `Mini_ChocoboRaceInitRenderAndAssets`, `Mini_ChocoboRaceBuildRacers`, route point 0, sound command 0x23 | `Mini_ChocoboSetupCamera` |
| `Mini_ChocoboFrame` | 0x0076dbd1 | clears, polls input, runs `Mini_ChocoboRunRaceResultLoop`, then `Mini_ChocoboTick` or `Mini_ChocoboReturnToField` | installed frame callback |
| `Mini_ChocoboRunRaceResultLoop` | 0x0077c462 | the state machine (states 0..6 in `g_ChocoboRacePhase`), see Data flow | `Mini_ChocoboFrame`, `Mini_ChocoboStepFrame` |
| `Mini_ChocoboTick` | 0x00779e14 | frame pacing with `Mini_ChocoboWaitFrameSlice`; after a late frame runs one extra step through `Mini_ChocoboStepFrame` | `Mini_ChocoboFrame`, `Mini_ChocoboStepFrame` |
| `Mini_ChocoboDrawFrame` | 0x0077b2ca | the draw pass: HUD slots, font batches, course batches, sprite groups, racer skeletons (`Anim_DrawSkeletonFrame`) | state 3 of the state machine, `Mini_ChocoboRaceUpdateBetScreen`, `Mini_ChocoboRaceUpdateResultScreen` |
| `Mini_ChocoboExit` | 0x0076da88 | frees racers, HUD objects, track data, sprite and fixed render objects | installed exit callback |
| `Mini_ChocoboMouse`, `Mini_ChocoboKeyboard` | 0x0076dcab, 0x0076dcb5 | empty | installed |

## Main structures

| type or global | header | what it holds |
|---|---|---|
| `g_ChocoboRacers` | `struct ChocoboRacer [6]`, defined in `ch_app.cpp`, declared in [include/ff7/api/chocobo.h](../../../include/ff7/api/chocobo.h) | six 0xA4-byte racer records, read as `g_ChocoboRacers[i].field`. |
| `ChocoboRacer` | [include/ff7/types/chocobo.h](../../../include/ff7/types/chocobo.h) | the 0xA4-byte racer record: segment, position, heading, speed, boost, runtime stats and penalties |
| `g_pChocoboRacerTable`, `g_ChocoboRacerCounts` | defined in `ch_chr.cpp`; the table pointer is declared in api/chocobo.h, the count only locally | the 0x3C-byte display entries built by `Mini_ChocoboBuildRaceObjects`; +0x34 points at the skeleton and animation player. `ChocoboRacerEntry` is a view of one entry. |
| `g_ChocoboTrackData` | api/chocobo.h | the 85-byte course header from `Mini_ChocoboLoadAllAssets`: counts and pointers into `chocobo.wat`. `ChocoboRaceState` and `ChocoboRaceAssetState` in `include/ff7/types/chocobo.h` describe the course header and asset state. |
| `ChocoboSpriteRecord` | types/chocobo.h | working state of the selected racer while it follows the route (`Mini_ChocoboRaceAdvanceSelectedRacerAlongRoute`) |
| `ChocoboRacerEntity`, `*g_pChocoboSoundCommand` | types/chocobo.h | despite the name, the sound command block passed to `SoundDispatchCommand` (`soundCommand`, `soundArg`, `soundParam1..5`) |
| `g_ChocoboRacerAsset0`, `g_ChocoboRacerAsset1` | local types in `ch_chr.cpp` | the two 39-entry model tables: one `.hrc` and up to six `.anm` paths per entry. `Mini_ChocoboBuildRaceObjects` reads table 1 when `g_ChocoboRetailFlag` is set, table 0 otherwise |

`ChocoboRaceRecordCopy` in `include/ff7/types/chocobo.h` copies 20-byte race records. `ChocoboRacer` (0xA4 bytes) is the one type of
`g_ChocoboRacers[6]` and `g_ChocoboSelectedRacerState`. Its members hold the racer's runtime stats (acceleration,
cooperation, intelligence, personality, autoPilot, rank, terrainPenaltyMask); the savemap bytes they
come from are `gSavemap.raceAcceleration`..`racePersonality`.

## Data flow

1. `App_OpenCoreArchives` (0x004082bf) opens `chocobo.lgp` from `GetMinigameDirForChocobo()` as LGP slot 7 at start-up.
2. `Mini_ChocoboRaceInitFromSavemap` reads the savemap bytes at 0x00dc0ae3 (course variant, `g_ChocoboCourseVariant`), 0x00dc0ae4 (when non-zero, skip the betting board), 0x00dc0ae5 and 0x00dc0af3, compares two bank-1 bytes (0x00dc08dc, 0x00dc08dd) with 1000, may write 0x00dc0ae2 from held input bits, and resets the race counters.
3. `Mini_ChocoboRaceInitRenderAndAssets` creates the HUD, racer, fixed and sprite render objects, then `Mini_ChocoboLoadAllAssets` reads `chocobo.wat` from LGP 7 (offset 0 or 0x9e078 by variant) and builds the vertex tables, and `Mini_ChocoboLoadCourseSounds` loads 9 or 7 sounds. `Mini_ChocoboBuildRaceObjects` (`ch_chr.cpp`) loads the `.hrc` skeletons and `.anm` animations. With the retail flag set, `Mini_ChocoboLoadFile` reads from LGP 7; the `Field_DebugFile*` path is the non-retail branch.
4. Each frame `Mini_ChocoboFrame` polls input into latches (`Mini_ChocoboPollInputLatches`) and runs `Mini_ChocoboRunRaceResultLoop`:
   - state 0: betting board (`Mini_ChocoboRaceResetBetScreen`) or straight to 2;
   - state 1: `Mini_ChocoboRaceUpdateBetScreen` each frame; it moves to 2;
   - state 2: one-shot race start (doubles two per-racer fixed-point values, sound command 0x10);
   - state 3: the race: camera (`Mini_ChocoboRaceUpdateCameraTargets`), course submission, `Mini_ChocoboRaceRankAndUpdateRacers`, HUD (`Mini_ChocoboSubmitRaceHud`), fade, then `Mini_ChocoboDrawFrame`;
   - state 4: assigns places to racers that did not finish, sound command 0xE4;
   - state 5: `Mini_ChocoboRaceUpdateResultScreen`, which writes 0x00dc0af5 and 0x00dc0af8 and moves to 6;
   - state 6: writes the first racer's place minus one to 0x00dc0af5 and sets `g_ChocoboExitFlag`.
5. Graphics go out through `Gfx_EmitPrimitiveBatchList`, `Gfx_DrawRenderObjectCall` and `Anim_DrawSkeletonFrame`; sound through `SoundDispatchCommand` and the cached SFX buffers.
6. In the same `Mini_ChocoboFrame` call, `Mini_ChocoboReturnToField` sees the exit flag and switches the module; the dispatcher then calls `Mini_ChocoboExit`.

## Named codes

[include/ff7/enums/chocobo.h](../../../include/ff7/enums/chocobo.h) defines `ChocoboCourseRecordKind`, `ChocoboRaceColor`, `ChocoboPersonality`, `ChocoboTrackFlag`, `ChocoboCameraEventKind`, `ChocoboRacerMode`, `ChocoboRacerEvent`, `ChocoboRaceClass`, `ChocoboRacePhase`, `ChocoboBetCellMode`, `ChocoboBetCellState`, and `ChocoboHudFloatBits`. The race TUs also use `RenderState` and `BlendMode` from [include/lib/enums/graphics.h](../../../include/lib/enums/graphics.h).

## Notes

In the race TUs:

- Race globals (`g_ChocoboRaceTimer`, `g_ChocoboRaceHeapBlock`, `g_ChocoboFrameStartTimestamp`, ...) describe what the race code reads and writes; `Mini_ChocoboLoadHrcAndAttach` and `Mini_ChocoboLoadEntitySkeletonIfAbsent` sit in the shared race engine TU.
- Racer and display tables are read as `*(T *)(base + i * 0xA4 + 0xNN)` and `+ i * 0x3C + 0xNN`.
- `Mini_ChocoboRunRaceResultLoop` (2,205 bytes), `Mini_ChocoboRaceBuildRacers` and the betting board keep their stack as `frame.atNN.value` unions.
- DirectDraw cache globals in `ch_ddraw.cpp` are named directly (`g_ChocoboDirectDraw`, `g_ChocoboPrimarySurface`, `g_ChocoboBackSurface`, `g_ChocoboPixelFormatMode`).

## Data-only files

Data runs that their link neighbour never references, each in a file of its own:

| file | span | next to | what it holds |
|---|---|---|---|
| `chocobo_race_state.cpp` | .bss 0x00e71130..0x00e71628 | `ch_app.cpp` | the race class and winner, the six racers and the selected one, the camera matrix, timers, input latches |
| `chocobo_bet_screen_panels.cpp` | .data 0x0097c478..0x0097c748 | `chocobo_bet_and_result_screens.cpp` | the bet screen's 1x panel layout |
