# Highway: the motorcycle chase minigame

The highway module is the motorcycle chase minigame, game mode 6 (`GAME_MODE_HIGHWAY`).
`GameModeDispatcher_Frame` (0x004090e6, [../src/main/main.cpp](../src/main/main.cpp)) calls
`Pfile_SetAssetSourceMode(0)` and `App_BlankScreenForModeSwitch`, then installs `Mini_HighwayEnter`,
`Mini_HighwayFrame`, `Mini_HighwayExit`, `Mini_HighwayMouse` and `Mini_HighwayKeyboard`.
`Mini_HighwayFrame` leaves through `Mini_HighwayReturnToField` when the init failed
(`g_HighwayRunningFlag` is 0) or when `g_HighwayExitRequested` is set. That function sets
`g_EnginePreviousMode` to 6 and the game mode word to 1 (field), and asks
`RequestModuleSwitch` for the `GameModeDispatcher_*` callbacks again.

## Where the code is

- `src/ff7/highway/`: 17 TUs, 172 functions.
- One file keeps its retail name from a `__FILE__` string: `psxdata.cpp` (`C:\FF7\highway\psxdata.cpp`). The other 16
  are named after their first function or class, and their place in the link order is inferred from
  their neighbours.
- TU boundaries follow retail's objects. The code runs from 0x00650310 (`highway_minigame_lifecycle.cpp`) to 0x0065ec20.

| TU | what it holds |
|---|---|
| `psxdata.cpp` | `HighwayFileLoader::load`, `HighwayPsxDataSet20` load, free, destructor |
| `highway_effect_records_pool.cpp` | the effect record pool (`Mini_HighwayInitEffectPool` .. `Mini_HighwaySpawnEffect`) |
| `highway_slot_actors_update_render.cpp` | the six actor slots: spawn, movement, transforms, animation, collision |
| `highway_field_frames_motion.cpp` | `HighwayTrackFrame`, `HighwayFieldContext` (the track), `HighwayFieldManager` sampling |
| `highway_hud_render_setup.cpp` | `HighwayRenderOwner`: the HUD object's constructor, effect slots, digits, banner and fade |
| `highway_mode_controller_layers.cpp` | `HighwayModeController` (constructor), `createLayers`, `drawLayers` |
| `highway.cpp` | `HighwayModelNode` and the `HighwayIndex` actor pool |
| `highway_u65_roster.cpp` | `HighwayRoster`: the RSD model roster |
| `highway_scene_camera_effects.cpp` | `HighwayScene` (camera and scene) and `HighwayEffect` |
| `highway_load_hrc.cpp` | `HighwaySkelItem`, `HighwaySkelList`, `HighwayFieldItem`: HRC skeletons and animations |
| `highway_field_list_draw.cpp` | `HighwayFieldList::draw` |
| `highway_render_group_clipping.cpp` | `HighwayRenderGroup`: textured track segments, z-plane clipping |
| `highway_palette_sampling.cpp` | `HighwayPaletteFieldBlock::reset`, `HighwayPalette::sample`, `sampleFrame`; the COMDAT of the inline `HighwayOffsetTable::lookup` (`highway_palette.h`) |
| `highway_sprite_group_quads.cpp` | `HighwayScaledSpriteGroup`: 2D sprite quads |

### The module TUs and the shared helpers

Three TUs sit at 0x00650310..0x006523b0, directly before `psxdata.cpp`:

| TU | what it holds |
|---|---|
| `highway_minigame_lifecycle.cpp` (7 functions) | the module callbacks and `Mini_HighwayReturnToField` |
| `highway_minigame_frame_loop.cpp` (25 functions) | `Mini_HighwayInit`, `Mini_HighwayResetRun`, `Mini_Mode6ExitStoreScores`, `Mini_HighwayRunFrame`, `Mini_HighwayRunEventTimeline`, the sound commands, `HighwayFileLoader`, `g_HighwayPsxDataSet` |
| `highway_input_state_polling.cpp` (3 functions) | the input snapshot (`Mini_HighwayReadInput`) |

Shared helpers are in [../snobo/](../snobo/README.md): `memory.cpp` (retail name `C:\FF7\snobo\memory.cpp`;
the global `operator new` and `operator delete`, the heap calls every highway class uses),
`snobo_prim_buffer_support.cpp` and `snobo_psx_matrix_math.cpp` (the text buffer and the PSX matrix helpers).


## Entry points

| function | address | what it does | called from |
|---|---|---|---|
| `Mini_HighwayEnter` | 0x00650310 | zeroes the frame counter, changes to the minigame folder, calls `Mini_HighwayInit` | module enter (main.cpp) |
| `Mini_HighwayFrame` | 0x00650412 | per frame: clear, `Input_Update`, `Mini_HighwayReadInput`, `Mini_HighwayRunFrame`, exit test | module frame |
| `Mini_HighwayExit` | 0x00650340 | clears and presents, `Mini_Mode6ExitStoreScores`, restores the folder | module exit |
| `Mini_HighwayMouse`, `Mini_HighwayKeyboard` | 0x006504a7, 0x006504ac | empty | module input |
| `Mini_HighwayInit` | 0x0065076d | opens `high-us.lgp`, loads the data set, creates every block, loads SFX, starts music | `Mini_HighwayEnter` |
| `Mini_HighwayRunFrame` | 0x00650f36 | the frame body between `Gfx_BeginSceneCall` and `Gfx_EndSceneCall` | `Mini_HighwayFrame` |
| `Mini_HighwayRunEventTimeline` | 0x0065114e | runs the next entry of `g_pHighwayTrackEventTable` (8 bytes: track progress, event code) once `g_HighwayTrackProgress` minus 0x2D has passed it, at most one per frame; it also counts the scene steps that set `g_HighwayExitRequested` | `Mini_HighwayRunFrame` |
| `Mini_HighwayUpdateAndRenderActors` | 0x00652dda | spawns, moves, transforms, clamps, renders and animates the actor slots, then tests collisions | `Mini_HighwayRunFrame` |
| `Mini_HighwayUpdateEffectRecords` | 0x006528e4 | updates the effect pool | `Mini_HighwayRunFrame` |
| `HighwayFieldContext::update`, `HighwaySlotList::clearActive` | 0x006576b3, 0x00657c81 | advance the track one step | `Mini_HighwayAdvanceTimeline` |
| `HighwayFieldContext::draw` | 0x00657ce2 | draws the track | `Mini_HighwayDrawFieldContext` |
| `HighwayRenderOwner::dispatch` | 0x0065950c | variant 1: `drawHud`; variant 0: `tick` | `Mini_HighwayDispatchModeController` |
| `HighwayRenderOwner::drawFade`, `HighwayModeController::drawLayers` | 0x00659532, 0x006596d7 | black 640x480 fade rectangle; the layer pass | `Mini_HighwayRunFrame`, `Mini_HighwayDrawLayers` |
| `HighwayScene::update` | 0x0065a7d8 | scene and camera update | `Mini_HighwayUpdateScene` |
| `HighwayPsxDataSet20::~HighwayPsxDataSet20` | 0x00652605 | frees the data set | the at-exit thunk 0x006504fa that the CRT initializer of `g_HighwayPsxDataSet` registers |

`highway_minigame_frame_loop.cpp` calls the track, HUD, layer and scene methods directly; and the four block
destructors `Mini_Mode6ExitStoreScores` runs use `#define` aliases (`HighwayFieldContext_Destroy`, `HighwayRenderOwner_Destroy`, `HighwayFileLoader_Destroy`, `HighwayRoster_Destroy`) over
their weak aliases, because `p->~T()` compiles to the call plus a delete-flag test of its own.

## Main structures

The `Highway*` class names are reconstructions, not the original names.

| type or global | header | what it holds |
|---|---|---|
| `HighwayPsxDataSet20`, `g_HighwayPsxDataSet` | [include/ff7/highway_psxdata.h](../../../include/ff7/highway_psxdata.h) | five words: the `baaa.bin` buffer, its size, the first `aaaa.adr` entry, the rebased `aaaa.adr` offset table, a loaded flag. The name is a placeholder (20 bytes) |
| `HighwayScene`, `g_pHighwaySceneBlock` | [include/ff7/types/highway.h](../../../include/ff7/types/highway.h) | the 0xC0-byte scene and camera block |
| `HighwayFieldContext`, `g_pHighwayFieldManager` | same | the track: 80 `HighwayTrackFrame`s, two `HighwayRenderGroup`s built from the track TIM name tables, a 0x4E0-byte work block |
| `HighwayRoster`, `g_HighwayRosterBlock` | same | one RSD model per name of the null-terminated `g_HighwayModelNames` (182 slots); equal names share one model |
| `HighwayIndex`, `HighwayModelNode`, `g_pHighwayIndexBlock` | same | the 0xC87C-byte actor pool |
| `HighwayRenderOwner`, `g_pHighwayRenderBlock` | same | the 0x78-byte HUD block: the banner, a `HighwayScaledSpriteGroup` of TIM sprites, five effect slot records, a render object, the fade byte |
| `g_pHighwayModeBlock` | same | the 0x18-byte layer block (`createLayers`, `drawLayers`) |
| `HighwaySkelList`, `HighwaySkelItem`, `g_HighwayActorFieldList` | same | the HRC skeletons and their animations |
| `g_HighwaySlotActors`, `HighwayActorGroup` | same | the actor groups that `Mini_HighwayInitActorGroups` sets up |
| `HighwayRenderSlotActor`, `HighwaySlotState`, `HighwayAnimItem`, `HighwayRootActorShortPos` | same | the 0x1A8-byte slot record: its head (animation items, highlight flags, model nodes) and the `HighwaySlotState` that starts at +0x68 (the kinds' per-mode names are anonymous unions) |
| `HighwayEffectRecord`, `HighwayEffectPoolEntry` | same | views of the effect pool records |

## Data flow

1. `Mini_HighwayEnter` changes to the folder `GetMinigameDirForHighway` returns, saving the old one in
   `g_HighwaySavedDirectory`.
2. `Mini_HighwayInit` sets the matrix layout mode and the asset source. If `g_HighwayUseLgpArchive` is set it
   opens `high-us.lgp` as archive 8; `HighwayFileLoader::load` then reads files from it, otherwise from disk
   (`LoadDiskFile`).
3. `HighwayPsxDataSet20::Mini_HighwayLoadPsxDataSet` loads `baaa.bin` and `aaaa.adr` and subtracts the first
   `aaaa.adr` word from every entry. Other code turns an entry into a pointer into `baaa.bin`: entry 0x11 or
   0x1F gives the track event table (`Mini_HighwayResetRun`), entry 0xF or 0x1D the track data
   (`HighwayFieldContext::HighwayFieldContext`).
4. `Mini_HighwayResetRun` reads the savemap: the variant byte at +0xD17 into `g_HighwayGameMode` and the high
   score from +0xE11 and +0xE12. `g_HighwayGameMode` (0 or 1) picks the event table, the track data, the
   sprite TIM table (`HighwayRenderOwner::HighwayRenderOwner`) and the HUD pass (`HighwayRenderOwner::dispatch`).
5. `Mini_HighwayInit` creates the file, scene, roster, index, layer, HUD and track blocks. `HighwayRoster::loadModels`
   loads the RSD models. `Mini_HighwayInitSlotArray` resets six actor slots and, through
   `Mini_HighwayInitActorGroups` and `Mini_HighwayLoadAnimations`, has `HighwaySkelList::load` load the ten HRC
   skeletons and their animations of `g_HighwaySkelTable` (`caaa.hrc`, `pbaa.hrc`, ...). It then sets
   up the effect pool and a text buffer (`Mini_InitPrimBuffer`), loads ten SFX from `g_HighwaySfxIdTable` and
   starts the music.
6. Each frame `Mini_HighwayRunFrame` resets the draw pools and render states, then runs
   `Mini_HighwayRunEventTimeline`, `Mini_HighwayUpdateFrameDelta` (TSC timer, capped at 16),
   `Mini_HighwayAdvanceTimeline` (track scroll), `Mini_HighwayUpdateAndRenderActors`, the layer, track,
   effect, HUD and scene passes, flushes the frame draw list and draws the fade.
7. Sound goes out through `SoundDispatchCommand` (`Mini_HighwayDispatchSoundCategory`,
   `Mini_HighwaySendChannelCommandA0`/`A8`/`B0`).
8. `Mini_Mode6ExitStoreScores` stops the MIDI and the SFX and frees the text buffer. In variant 1 it writes the
   high score to savemap +0xE11/+0xE12 and the last score to +0xE0F/+0xE10. In variant 0 it writes five 16-bit
   values from `g_HighwayHpBarTable` to memory bank 1 at +0x174 (savemap +0xD18..+0xD21). It then frees
   every block and the data set and closes archive 8.

Savemap fields are read and written through `gSavemap` in [include/ff7/savemap.h](../../../include/ff7/savemap.h).

## Named codes

[include/ff7/enums/highway.h](../../../include/ff7/enums/highway.h) defines `HighwayFrameInfo`, `HighwayTrackEventType`, `HighwayInputButton`, `HighwayInputAction`, `HighwayInputDirection`, `HighwaySlotMode`, `HighwaySlotCondition`, and `HighwaySceneCameraMode`. The highway TUs also use `RenderState`
(`RENDER_STATE_*`) and `BlendMode` (`BLEND_*`) from
[include/lib/enums/graphics.h](../../../include/lib/enums/graphics.h), and `INPUT_CIRCLE`/`INPUT_SQUARE` from
`InputButton` in [include/lib/enums/file.h](../../../include/lib/enums/file.h).

## Notes

- The one `goto` is kept in `HighwayRoster::loadModels`; its comment says the original jumps to a stub after the function end.
- Core runtime state globals: `g_pHighwayRenderBlock`, `g_pHighwayModeBlock`, `g_HighwayCountdown`, `g_HighwaySlotCountdown`.
- `HighwayRenderOwner` describes the 0x78-byte render block (`g_pHighwayRenderBlock`), while `HighwayModeController` describes the 0x18-byte mode/layer block (`g_pHighwayModeBlock`).
- Raw reads: `*(short *)(((char *)&g_HighwayHpBarTable + 0x4) + p * 0xA)` in `Mini_HighwayResetRun`;
  the slot arrays are written as `*(int *)((char *)&g_HighwaySlotPositionX + slot * 0x1A8)` in
  `Mini_HighwayResetSlot`.
- Sound commands (0xB8, 0xB9, ...) still have literal uses alongside `HighwayTrackEventType` and `HighwaySlotMode`.

## Data-only files

Data runs that their link neighbour never references, each in a file of its own:

| file | span | next to | what it holds |
|---|---|---|---|
| `highway_scene_block.cpp` | .bss 0x00d8d458..0x00d8d45c | `highway_scene_camera_effects.cpp (before it)` | the scene block pointer |
