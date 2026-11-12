# Condor: the Fort Condor battle minigame

This folder holds Fort Condor, the board battle in which the player spends gil on units that then move and fight (`Mini_CondorTryPurchaseRecordSlot` and `Mini_CondorUpdateUnits`, by their names). It is game mode `GAME_MODE_FORT_CONDOR` (0x09). `GameModeDispatcher_Frame` (0x004090e6) in [../src/main/main.cpp](../src/main/main.cpp) installs `Mini_CondorEnter`, `Mini_CondorExit`, `Mini_CondorFrame`, `Mini_CondorMouse` and `Mini_CondorKeyboard` through `RequestModuleSwitch`. The minigame leaves when the exit word `g_CondorStage` reaches 4 or more: `Mini_CondorFrame` then calls `Mini_CondorReturnToField`, which sets `g_EnginePreviousMode` to 9 and `g_EngineGameMode` to 1 and switches back to the `GameModeDispatcher_*` callbacks. One TU in this folder (`condor_unit_hud_readouts.cpp`) also defines the field opcode dispatch table (see below).

## Where the code is

- `src/ff7/condor/`: 17 TUs, 333 functions.
- 4 files keep their retail names from `__FILE__` strings (`C:\FF7\condor\cd_app.cpp`, `cd_init.cpp`, `cd_tim.cpp`, `cd_ddraw.cpp`). The other 13 are named after their first function, and their place in the link order is inferred from their neighbours.
- TU boundaries follow retail's objects.

| TU | what it holds |
|---|---|
| `cd_app.cpp` | module callbacks, return to field, engine context init |
| `cd_init.cpp` | asset path, `Mini_CondorLoadFile`, the `data\*.bin` and TIM loaders, texture render objects, row and slot tables |
| `cd_tim.cpp` | TIM parsing, palette scaling, pixel expansion into surfaces |
| `cd_ddraw.cpp` | DirectDraw objects, surface lock, blit and colour-key helpers |
| `condor_scene_viewport.cpp` | the condor viewport preset. Its two functions are named `Field_*`, but they write only condor globals; the code is condor's |
| `condor_engine_frame_render.cpp` | engine context, frame wait, `Mini_CondorTick`, `Mini_CondorRenderScene`, skeleton loaders |
| `condor_game_state_hud.cpp` | savemap load and store, `Mini_CondorDispatchPhase`, game reset, outcomes, `Mini_CondorRunBattleFrame`, HUD drawing |
| `condor_unit_model_loading.cpp` | the 29 unit model sets (`.rsd`) and their draw |
| `condor_input_latch_cursor.cpp` | input latches and alternate key bindings |
| `condor_sound_motion_state.cpp` | music and SFX helpers, record-select panel, unit attack, motion, the state handler that sets the exit word |
| `condor_run_cursor_camera.cpp` | `Mini_CondorUpdateFrameInput`, cursor, camera scroll |
| `condor_unit_records.cpp` | unit simulation: commands, purchase, effects, damage, path segments, placement and region tests |
| `condor_unit_hud_readouts.cpp` | counter effects, sprite slots, digit displays; also defines the field opcode table (below) |

### Neighbouring field code

The two adjacent field TUs (`field_map_script_state.cpp`, the field script VM core, and `field_encounter_random.cpp`, random encounters) live in [ff7/field](../field/README.md).
`g_FieldScriptOpTable`, the field script opcode table, is in `condor_unit_hud_readouts.cpp`: the
rest of that TU is condor code. `FieldScriptHeader` and `FieldEncounterEntry`, which the field TUs use,
are in [include/ff7/types/condor.h](../../../include/ff7/types/condor.h).

## Entry points

| function | address | what it does | called from |
|---|---|---|---|
| `Mini_CondorApplyViewportPreset` | 0x005f4630 | sets the condor viewport, offsets and screen scale for UI scaling mode 0, 1 or 2 | `App_InitSubsystems` (0x004089c5) |
| `Mini_CondorSetAssetDirPath` | 0x005f2f18 | copies `GetMinigameDirForCondor()` into `g_CondorAssetDirBuffer` | `App_InitSubsystems` |
| `Mini_CondorEnter` | 0x005f47b5 | sets `g_CondorUsesLgp`, frame time (context +0x30 / 60), sound table, viewport; loads unit models; `Mini_CondorLoadSavemapState` | installed enter callback |
| `Mini_CondorLoadSavemapState` | 0x005f7756 | reads the savemap, `Mini_CondorLoadAllResources`, `Mini_CondorResetGameState`, music | `Mini_CondorEnter` |
| `Mini_CondorFrame` | 0x005f4a47 | clears, reads input, dispatches the phase, then `Mini_CondorTick` or `Mini_CondorReturnToField` | installed frame callback |
| `Mini_CondorUpdateFrameInput` | 0x005fd958 | packs the input latches; runs `Mini_CondorDispatchStateHandler` while the battle state is non-zero, else handles the selection; returns the phase | `Mini_CondorFrame`, `Mini_CondorTick` |
| `Mini_CondorDispatchPhase` | 0x005f7893 | phase 0: `Mini_CondorRunBattleFrame`; 1, 2, 3: restart variants; other: stop music | `Mini_CondorFrame`, `Mini_CondorTick` |
| `Mini_CondorRunBattleFrame` | 0x005f7e10 | one battle step: clock, scheduled spawns, units, outcome checks, `Mini_CondorDrawHudFrame` | phase 0 |
| `Mini_CondorTick` | 0x005f5042 | `Mini_CondorRenderScene`, `Mini_CondorWaitForFrameTime` (timestamp busy-wait); after a late frame (`g_CondorFrameLate` set) one extra input and phase step | `Mini_CondorFrame` |
| `Mini_CondorRenderScene` | 0x005f5828 | draws eight batch lists, the active units, unit models and render-object slots in one scene; skipped after a late frame | `Mini_CondorTick` |
| `Mini_CondorExit` | 0x005f49c0 | `Mini_CondorStoreSavemapState`, frees textures, data files, TIM surfaces, SFX buffers and unit models | installed exit callback |
| `Mini_CondorMouse` | 0x005f4abc | empty | installed |
| `Mini_CondorKeyboard` | 0x005f4ac1 | F11 toggles `g_CondorAppFlag` (nothing reads it), F12 toggles the cursor mode | installed |

## Main structures

| type or global | header | what it holds |
|---|---|---|
| `CondorRecord` | [include/ff7/types/condor.h](../../../include/ff7/types/condor.h) | the 0x78-byte unit record: flags, timers, counters, target and contact slots. Used in 7 TUs. |
| `g_CondorRecordTable` | `CondorRecordTable`, defined in `condor_game_state_hud.cpp` (8,284 bytes) | the unit records, `records[69]` of `CondorRecord`, indexed by `g_CondorSelectedRecord` and others. |
| `g_CondorCounterEffects` | `CondorCounterEffect [16]`, `condor_game_state_hud.cpp` | the counter effect slots (`Mini_CondorFindInactiveCounterEffectSlot`): `counter` counts up while nonzero, bit 15 a flag |
| `CondorEffectSlotEntry`, `CondorEffectSequence`, `CondorSlotMotion` | [include/ff7/types/condor.h](../../../include/ff7/types/condor.h) | HUD effect slot entries, effect sequences, cursor and panel motion |
| `CondorTextureRenderObjects`, `g_CondorTextureRenderObjects` | types/condor.h, `cd_init.cpp` | the render objects for the condor TIM textures |
| `CondorUnitModel`, `g_CondorUnitModelTable` | types/condor.h, `cd_init.cpp` | seven 0x38-byte entries, each four parts (a render object and its batched flag), a ready flag and the draw pass; `Mini_CondorRenderScene` draws them |
| `CondorSoundCommand` | types/condor.h | the sound command block that the music and SFX helpers fill |
| `g_CondorStage` | [include/ff7/api/condor.h](../../../include/ff7/api/condor.h) | the exit word: 1..3 request a restart phase, 4 or 5 leave |
| `g_CondorMainState` | api/condor.h | the battle state that `Mini_CondorRunBattleFrame` and `Mini_CondorDispatchStateHandler` switch on |
| `g_CondorFunds` | api/condor.h | the gil copy, loaded from and stored to savemap +0xB7C |

`cd_init.cpp` resets the unit table as `CondorUnitModel` records, the effect slots as `CondorEffectSequence` and the record table as `CondorRecord`.

## Data flow

1. `App_OpenCoreArchives` (0x004082bf) opens `condor.lgp` from `GetMinigameDirForCondor()` as LGP slot 6. With `g_CondorUsesLgp` set by `Mini_CondorEnter`, `Mini_CondorLoadFile` reads from slot 6; the `Field_DebugFile*` path is the other branch.
2. `Mini_CondorEnter` loads the 29 unit model sets (`Mini_CondorLoadUnitModels`). `Mini_CondorLoadSavemapState` reads savemap +0xC4D (0..6, else 0), +0xC57 and the gil at +0xB7C, then `Mini_CondorLoadAllResources` loads `data\data.bin`, `data\camera.bin` (rotation, translation and screen distance), `data\id.bin` with `data\vert.bin`, and `data\ehelp.tim` (`ehelp1.tim` in scaling mode 2), and creates the texture render objects (`map%d.tim`, `eft00.tim`, `emes*.tim`, `eunit*.tim`).
3. Each frame `Mini_CondorUpdateFrameInput` returns phase 1 when the button mask 0x90F is held, or the exit word when it is non-zero, else 0. `Mini_CondorDispatchPhase` runs `Mini_CondorRunBattleFrame` for phase 0 and restarts through `Mini_CondorResetGameState` for phases 1 to 3.
4. `Mini_CondorRunBattleFrame` advances the clock, spawns scheduled records (`Mini_CondorSpawnScheduledRecord`), updates units (`Mini_CondorUpdateUnits`, `Mini_CondorTickUnitTimers`) and checks the two end conditions: `Mini_CondorBeginOutcome1` when `g_CondorSpawnCount` is 0, `Mini_CondorBeginOutcome2` when `g_CondorPlayerUnitCount` is 0 and the gil is below 400. While the mode word `g_CondorPhase` is 1 the units only animate.
5. `Mini_CondorTick` renders through `Gfx_EmitPrimitiveBatchList` and `Gfx_DrawRenderObjectCall`, waits for the frame time and resets the draw pools. Sound goes through `SoundDispatchCommand` (`Mini_CondorPlayMappedMusic`, `Mini_CondorSubmitSfxPlay`).
6. `Mini_CondorTickCounterOrRequestState4` and `...State5` set the exit word to 4 or 5. `Mini_CondorExit` then writes savemap +0xC4E, +0xC4F, +0xC53, +0xC50 (1 when the exit word is 4) and the gil back to +0xB7C.

## Named codes

[include/ff7/enums/condor.h](../../../include/ff7/enums/condor.h) defines `CondorUnitAction`, `CondorCommandCode`, `CondorMainState`, `CondorStage`, `CondorRecordMode`, `CondorPanelPhase`, `CondorAttackStep`, `CondorShotKind`, `CondorDisplayMode`, `CondorAttackResult`, `CondorBlockDir`, and `CondorTargetMode`. `condor_engine_frame_render.cpp` and `cd_app.cpp` also use `RenderState` and `BlendMode` from [include/lib/enums/graphics.h](../../../include/lib/enums/graphics.h).

## Notes

- Phase, state, command, and attack constants use the enumerators in `include/ff7/enums/condor.h`, and HUD digit-group tables use `CondorDigitGroup` (`g_CondorDigitGroupTable`).
- The savemap is read and written through an `int` copy of `&gSavemap` plus offsets (+0xC4D, +0xB7C, ...).
- Loaded asset buffers: `data.bin` is stored in `g_pCondorDataBin`, `camera.bin` in `g_pCondorCameraBin`, `id.bin` in `g_pCondorIdBin`, and `Mini_CondorLoadAuxFile`'s buffer in `g_CondorAuxFileBuffer`.

## Data-only files

Data runs that their link neighbour never references, each in a file of its own:

| file | span | next to | what it holds |
|---|---|---|---|
| `condor_frame_state.cpp` | .bss 0x00cbee38..0x00cbf108 | `cd_app.cpp` | the frame timing and TIM surface state |
