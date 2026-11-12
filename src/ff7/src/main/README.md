# Main: the application shell and the game-mode dispatcher

ff7/main is the code between Windows and the game modules. `App_Bootstrap` is the first thing
`WinMain` calls: it reads the install paths and graphics settings from the registry, checks the
swap space, the sound and MIDI setup and the game disc, and installs the application callbacks in
the engine context. `App_InitSubsystems` then starts sound, input and graphics, opens the LGP
archives and loads `kernel.bin`. From then on `GameModeDispatcher_Frame` turns the game-mode word
into a module: field, battle, world map, menu, the minigames, credits, disc check, game over and
the battle swirl. Every module comes back to the dispatcher when it ends. The game is left through
mode 0x13 (`GAME_MODE_QUIT`), which sends `WM_CLOSE` (`SendMessageA`); `WinMain` then calls the dispatcher's exit
and `App_Shutdown`.

## Where the code is

`src/ff7/src/main/`: 9 TUs, 98 functions. Two files keep their retail names from `__FILE__`
strings; the other seven are named after what they contain, and their place in the link order is
inferred from their neighbours. TU boundaries follow retail's objects.

| TU | span | functions | holds |
|---|---|---:|---|
| `main.cpp` | 0x00408010-0x0040a460 | 26 | bootstrap, init, shutdown, dispatcher, window message callbacks, field entry point |
| `initpath.cpp` | 0x00406d10-0x00408010 | 15 | install paths, volume settings, archive paths |
| `main_cd_check_screen.cpp` | 0x00402ef0-0x00404d80 | 12 | disc-check module, `CdCheck_GetGameDiscNumber` |
| `main_registry_ui_scaling.cpp` | 0x00404d80-0x00404e00 | 1 | `GetUiScalingMode` |
| `main_registry_graphics_config.cpp` | 0x004067a0-0x00406d10 | 3 | graphics registry config into the engine context |
| `main_registry_key_access.cpp` | 0x0040a460-0x0040aab0 | 22 | Sound/Midi/DD registry values |
| `main_clock_timer.cpp` | 0x0040aab0-0x0040ac90 | 4 | savemap clock timer |
| `main_movie_camera_records.cpp` | 0x0040ac90-0x0040adb0 | 2 | movie camera record per frame |
| `main_no_op_hooks.cpp` | 0x00414ee0-0x00414f00 | 3 | `Trace_Printf` (the trace printf) and two no-op hooks |
| `main_game_over_screen.cpp` | 0x00404e00-0x004067a0 | 10 | the game-over module (`GameOver_Module*`) |

`main_game_over_screen.cpp` sits between `main_registry_ui_scaling.cpp` and `main_registry_graphics_config.cpp`, has the
same shape as the disc-check module (fade quad, three screen-quad builders, a disc-archive loader) and
returns to the dispatcher the same way.

Code that belongs here but lives in other folders:

- `WinMain` (0x0067db30, `src/lib/src/graphics/graphics_app_entry_point.cpp`), `RequestModuleSwitch`,
  `ServiceModuleSwitch`, `CommitPendingModule` and `GetActiveModule`
  (`src/lib/src/graphics/graphics_driver_context_fields.cpp`), and `CreateGameWindow` and
  `GameWindowProc` (`src/lib/src/graphics/directx.cpp`) are in
  [lib/graphics](../../../lib/src/graphics/README.md).
- The battle swirl module (`Swirl_Module*`, `src/ff7/src/battle/yama/yama_swirl_transition_module.cpp`,
  0x00401500-0x00402ef0) sits between `src/ff7/src/battle/yama/init.cpp` and `main_cd_check_screen.cpp`. Which
  subsystem it belongs to is unknown.

Code here that may belong elsewhere:

- `main_movie_camera_records.cpp` reads the camera globals of `sm_movie.cpp` (and the field movie phase
  word) and ends where
  `sm_movie.cpp` starts. See
  [ff7/movie](../movie/README.md).
- `main_no_op_hooks.cpp` sits right before `src/lib/src/movie/movie.cpp`. `Trace_Printf` is the release build's
  trace printf: the movie and main code's log lines call it, and it ignores them (88 of its call sites are in lib/movie and ff7/movie).

## Entry points

| function | address | what it does | called from |
|---|---|---|---|
| `App_Bootstrap` | 0x0040a091 | paths, swap and config checks, disc check, engine context and callbacks | `WinMain` |
| `App_InitSubsystems` | 0x004089c5 | graphics, input, sound, MIDI, archives, kernel; sets mode 0x1B | engine context +0x9F0, from `CreateGameWindow` |
| `GameModeDispatcher_Enter` | 0x00408fa6 | disc-change redirect, game-over and ending requests | engine context +0x9F8 (module enter) |
| `GameModeDispatcher_Frame` | 0x004090e6 | movie blit, then one switch on the game mode | engine context +0xA00 (module frame) |
| `GameModeDispatcher_Exit` | 0x004090c7 | logs "Exiting MAIN", sets ctx+0xA48 | engine context +0x9FC (module exit) |
| `App_OnMouseMessage` | 0x00409df1 | stores the mouse position at ctx+0x840/+0x844 | `GameWindowProc` via +0xA04 |
| `App_OnKeyMessage` | 0x00409e39 | Ctrl/Shift state, F1, Ctrl+q sets mode 0x13 | `GameWindowProc` via +0xA08 |
| `App_OnActivateApp` | 0x00409cf2 | restores the movie surface, or stops a playing movie | `GameWindowProc` on `WM_ACTIVATEAPP` (+0xA28) |
| `App_OnDeviceChangeMessage` | 0x00409d66 | forwards to `CdCheck_HandleDeviceChange` | `GameWindowProc` on `WM_DEVICECHANGE` (+0xA2C) |
| `App_AllowCtrlQQuit` | 0x00409d7b | 0 in swirl, mode 24, ending, credits, submarine, or field with a movie | engine context +0xA5C |
| `App_Shutdown` | 0x00408edc | tears everything down | engine context +0x9F4, from `WinMain` |
| `GetUiScalingMode` | 0x00404d80 | registry `Graphics\Mode`, default 2 | 46 call sites in 10 subsystem folders |
| `Savemap_CaptureFieldEntryPoint` | 0x00408116 | writes the field position into the savemap | dispatcher, ff7/field |
| `CdCheck_ModuleEnter`/`Frame`/`Exit` | 0x00404688 / 0x004047f6 / 0x0040476c | disc-check module | dispatcher, mode 0x0C |
| `CdCheck_GetGameDiscNumber` | 0x00404a7d | volume label `FF7DISC1..3` to 1..3, else 0 | bootstrap, dispatcher, disc check |
| `GetMinigameDirFor*` | 0x004076fe..0x00407933 | `<CdDataPath>minigame/` in one shared buffer | init, minigames |
| `Savemap_GetClockTickCount` | 0x0040ac84 | the savemap play-time timer's tick count (60 a second) | ff7/wm and the disc check, as an animation clock and random seed |
| `Trace_Printf` | 0x00414ee0 | the release build's trace printf: ignores its format and arguments, returns 0 | lib/movie, ff7/movie, main.cpp |

## Main structures

| type or global | header | what it holds |
|---|---|---|
| `GameModule` | `include/lib/types/graphics.h` | a module's seven callbacks: +0x00 `init`, +0x04 `shutdown`, +0x08 `enter`, +0x0C `exit`, +0x10 `frame`, +0x14 `onMouse`, +0x18 `onKey`. A module fills one and hands it to `RequestModuleSwitch`; the engine context holds the running one and the staged one. |
| `GfxEngineContext` | `include/lib/types/graphics.h` | the engine context `CreateEngineContext` makes, documented member by member. Main sets and reads: `activeModule` (+0x9F0, a `GameModule`: `App_InitSubsystems`, `App_Shutdown` and the running module's callbacks), `pendingModule` (+0xA0C, the staged record), `moduleSwitchPending` (+0x924), `onActivateApp` (+0xA28), `onDeviceChange` (+0xA2C), `pendingWindowEvent` (+0xA54), `quitConfirmRun` (+0xA58, `Menu_QuitConfirmDialogRun`), `allowCtrlQQuit` (+0xA5C), `hwnd` (+0x5C), `presentEnable` (+0xA48), `cursorX`/`cursorY` (+0x840/+0x844, `App_OnMouseMessage`). The dispatcher's functions and the app's message handlers take it as `GfxEngineContext *`. |
| `g_EngineGameMode` | `include/ff7/api/main.h` | the current `GameMode` (0x00CBF9DC, a `short` defined in `field_script_operands.cpp`) |
| `g_EnginePreviousMode` | `include/ff7/api/main.h` | the mode the game came from (0x00CC0D84) |
| `g_FieldActorState.state.requestCode`, `.eventCmdParam` | `include/ff7/types/field.h` (`FieldState`) | the field request byte (`FieldRequestCode`, 0x00CC0D89) and its argument |
| `g_Main_AppContext` | `main.cpp` | a `MainAppContext` (16 bytes at 0x009a06d8); `inputState` (+0x8) is the 0x90-byte `MainInputState` block from `App_AllocInputState`, where `App_OnKeyMessage` keeps `ctrlDown`, `shiftDown` and `f1DebugToggle` |
| `g_InitPath_*` | `include/ff7/api/main.h` | the install paths from the registry, 256-byte heap strings (`g_InitPath_AppPath`, `g_InitPath_MoviePath`, `g_InitPath_DataDrive`, `g_InitPath_DataPath`, `g_InitPath_CdDataPath`, `g_InitPath_SoundPath`, `g_InitPath_MusicPath`, `g_InitPath_MidiPath`) and the `g_InitPath_SoundEnabled` / `g_InitPath_MidiEnabled` flags |
| savemap field entry record | `include/ff7/savemap.h` (`gSavemap`) | savemap +0xB90..+0xBA2: `fieldReturnMode`, `gameMode`, `locationId`, `coordX`, `coordY`, `coordT`, `coordD`, `fieldRandomIndex1`/`2` |
| `g_Savemap_ClockTimerData`, `g_Savemap_ClockTickCount` | `main_clock_timer.cpp` | multimedia timer id and critical section of the savemap clock; its tick count |
| `g_CdCheck*Batch`, `g_GameOverRenderObjectsMode2`, `g_GameOverRenderObjectsMode01`, `g_GameOverFadeRenderObject` | `main_cd_check_screen.cpp`, `main_game_over_screen.cpp` | render-object batches for the disc-check, fade and game-over screen quads |

## Data flow

### Startup

1. `WinMain` calls `App_Bootstrap(0, 0)`.
2. `App_Bootstrap` turns heap tracking on when `g_Main_HeapTrackingEnabled` is set, then calls
   `InitPathsFromRegistry`. That reads `DataDrive`, `Sound`, `Midi`, `AppPath`, `DataPath`,
   `MoviePath` and `FullInstall` under `HKLM\Software\Square Soft, Inc.\Final Fantasy VII`.
   `g_InitPath_CdDataPath` is `g_InitPath_DataPath` on a full install, else `g_InitPath_DataDrive` + `ff7/`.
3. Less than 50,000,000 bytes of page file shows "No Swap Space" and sets mode 0x13.
4. A missing `Graphics`, `Midi` or `Sound` key, an empty `DD_GUID` or `Sound_GUID`, or a
   `MIDI_DeviceID` of -1 runs `<AppPath>ff7config.exe` and waits; a non-zero exit code sets mode 0x13.
5. `CdCheck_GetGameDiscNumber() == 0` shows "Insert Game Disc" and sets mode 0x13.
6. `CreateEngineContext`, `ApplyConfigToContext` (registry `Graphics` values: resolution, driver,
   options), then the callbacks of the table above, then `SetEngineContext`.
7. `CreateGameWindow` calls +0x9F0 (`App_InitSubsystems`) and, when that returns non-zero, +0x9F8
   (`GameModeDispatcher_Enter`).
8. `App_InitSubsystems` returns at once in mode 0x13. Otherwise it allocates the input state, calls
   `InitGraphicsDevice`, sets key repeat 200/50, resets the field entry point, starts sound
   (`SoundInit`, `SfxLoadIntoBuffer(0x2B)`) and MIDI when the registry value is `ON` or missing, and opens LGP
   archives 4 `menu/menu_us.lgp`, 2 `battle/battle.lgp`, 3 `battle/magic.lgp`. `App_OpenCoreArchives`
   opens 0 `field/char.lgp`, 1 `field/flevel.lgp`, 7 `chocobo.lgp`, 6 `condor.lgp`,
   5 `wm/world_us.lgp` and 14 `cd/moviecam.lgp`. Any failure logs "Failed to load", sets mode 0x13
   and returns. Then display modes for field and chocobo, the chocobo and condor asset directories,
   `Field_InitModuleResources`,
   `CoInitialize`, `Kernel_LoadKernelBinFiles`, `Savemap_ResetConfigDefaults` and `Kernel_ResetLoadState`.
   On success, and also when `InitGraphicsDevice` fails, it ends with mode 0x1B, previous mode 0
   and `ApplyRegistryVolumeSettings`.

The first mode the dispatcher sees is therefore 0x1B: the credits module, variant 0 (see
[credits](../credits/README.md)). Its exit sets mode 0x14, the title menu.

### The per-frame loop (`WinMain`)

1. Pending window messages go to `GameWindowProc`, which calls the mouse (+0xA04), key (+0xA08),
   activate (+0xA28) and device-change (+0xA2C) callbacks.
2. With no message waiting and no quit dialog requested (+0xA54, which runs +0xA58 instead),
   `ServiceModuleSwitch` runs. If a switch is pending (+0x924) it calls the
   active exit (+0x9FC), copies the staged record into the active slots (`CommitPendingModule`),
   calls the new enter (+0x9F8) and clears the flag.
3. The active frame callback (+0xA00) runs when ctx+0x20 is zero and `RestoreLostSurfaces`
   succeeds, then `PresentFrame`.
4. When the context's first word is set the loop ends: exit (+0x9FC), texture cache release,
   shutdown (+0x9F4), `DestroyWindow`.

### Entering and leaving a module

- To start a module, `GameModeDispatcher_Frame` writes its five callbacks into a `GameModule` and
  calls `RequestModuleSwitch`. That only stages the record. On the next loop iteration
  `ServiceModuleSwitch` calls `GameModeDispatcher_Exit`, installs the module and calls its enter.
- To leave, a module writes the next mode into `g_EngineGameMode`, fills a
  `GameModule` with `GameModeDispatcher_Enter`, `GameModeDispatcher_Exit`, `GameModeDispatcher_Frame`, `App_OnMouseMessage` and
  `App_OnKeyMessage`, and calls `RequestModuleSwitch`. Examples: `Field_ReturnToDispatcher`
  (0x0060e521, `src/ff7/field/src/ad_app.cpp`), `CdCheck_ModuleFrame` after its fade, and
  `Credits_ModuleFrame`. The module's exit then runs, then `GameModeDispatcher_Enter`.
- `GameModeDispatcher_Enter` sends the game to the disc check: when the previous mode is 0x01,
  0x14, 0x15 or 0x16 and the savemap disc byte (+0xEA4) differs from `CdCheck_GetGameDiscNumber`, it saves
  the mode as the previous mode and sets 0x0C. `CdCheck_ModuleExit` puts it back. A field request
  0x1A sets mode 0x1A and reloads `kernel.bin`; a field request 0x0A sets mode 0x19.
- Modes 0x0D, 0x10, 0x15 and 0x16 switch no module. They change the mode word, and the
  dispatcher, still active, acts on the new mode next frame.

### `GameModeDispatcher_Frame`, case by case

Nothing runs while ctx+0x20 is set. Otherwise a playing movie is blitted into surface 1
(`MovieBltFrameToBackBuffer`), a stop request stops it, and then:

| mode | what the case does |
|---|---|
| 0x01 `GAME_MODE_FIELD` | blank screen, texture-wrap render state, `Field_Module*`; the exit slot stays 0 |
| 0x02 `GAME_MODE_BATTLE` | blank screen, `Battle_ModuleEnter`/`Battle_ModuleFrame`/`Battle_ModuleExit`, no mouse or key callback |
| 0x03 `GAME_MODE_WORLD_MAP` | capture the field entry point, set `g_FieldReturnMode` and `g_FieldNextModuleArg`, `WorldMap_Module*` |
| 0x05 `GAME_MODE_MENU` | blank screen, capture the field entry point, reload party stats when flagged, `Menu_Module*` |
| 0x06, 0x07, 0x09, 0x0B | blank screen, `Mini_Highway*`, `Mini_Chocobo*`, `Mini_Condor*`, `Mini_Coaster*` |
| 0x08, 0x0E `GAME_MODE_SNOWBOARD` | blank screen, `Mini_Snowboard*` |
| 0x0A `GAME_MODE_SUBMARINE` | asset source mode 1, blank screen, `Submarine_Module*`, no mouse or key callback |
| 0x0C `GAME_MODE_DISC_CHECK` | wrong disc: `Lgp_CloseArchivesForDiscSwap`, `CdCheck_Module*`; right disc: mode 0x01 |
| 0x0D `GAME_MODE_FIELD_SERVICE` | one of seven savemap services picked by the field request (`Script_Svc*`, `Savemap_SnapshotLevelsToBank`), `Savemap_ReconcileFieldParty`, mode 0x01 |
| 0x10 `GAME_MODE_REENTER_FIELD` | mode 0x01 |
| 0x11 `GAME_MODE_MENU_AFTER_BATTLE` | `Menu_Module*` |
| 0x13 `GAME_MODE_QUIT` | `SendMessageA(window, WM_CLOSE)` |
| 0x14 `GAME_MODE_TITLE_MENU` | music volume, `Menu_CreateLoadScreenObjects`, `Menu_Module*` |
| 0x15 `GAME_MODE_CONTINUE_GAME` | release an open movie, `Savemap_RestoreFieldEntryPoint`, start the clock timer |
| 0x16 `GAME_MODE_NEW_GAME` | `Savemap_ResetConfigDefaults`, `Savemap_ResetForNewGame`, reset and restore the entry point, start the clock timer |
| 0x17 `GAME_MODE_BATTLE_SWIRL` | `Swirl_Module*`, count the battle in savemap +0xBBC, set up the battle request (`Battle_SetupEntryFromField`) |
| 0x19 `GAME_MODE_ENDING_MOVIE` | first frame opens and runs `<MoviePath>ending2.avi`; when it has stopped, release it and set 0x1C |
| 0x1A `GAME_MODE_GAME_OVER` | blank screen, stop the clock timer, `GameOver_Module*` |
| 0x1B `GAME_MODE_CREDITS` | open `cd/cr_us.lgp` as archive 15, start section 0 after game over else 7, variant 0, `Credits_Module*` |
| 0x1C `GAME_MODE_ENDING_CREDITS` | archive 15, blank screen, variant 1, `Credits_Module*` |
| 0x00, 0x04, 0x0F, 0x12, 0x18 | no case |

`Savemap_RestoreFieldEntryPoint` copies the savemap entry record into the field globals and sets
the game mode from savemap +0xB94 (0 becomes 0x01). That is how continue and new game reach the
field or the world map.

### Savemap clock

`Savemap_StartClockTimer` starts a `timeSetEvent` timer every 40, 25 or 17 ms (by driver, UI mode
and the context's clock rate). `Savemap_ClockTick` counts ticks and, unless one of two pause words is
set, adds 0x444 to two 16-bit fractions. A carry out of the first adds one to the savemap seconds;
a carry out of the second moves the countdown seconds up or down.

### Shutdown

`App_Shutdown` sets mode 0x13, frees the movie camera buffer, stops the clock timer, shuts down the
field module, frees the input state and paths, closes all LGP archives, shuts down sound, MIDI and
input, releases menu render objects unless the previous mode was 0x1A, frees field and world
resources, shuts down the graphics device, releases the movie graph and calls `CoUninitialize`.
With heap tracking on it prints heap usage and dumps leaks.

## Named codes

From `include/ff7/enums/main.h`:

- `GameMode`: the values of the game-mode word; the cases of `GameModeDispatcher_Frame`.
- `FieldRequestCode`: the field request byte at 0x00CC0D89; `GameModeDispatcher_Enter` and the
  0x0D case switch on it.
- `TextCode`: FF7 text bytes; defined here, used by the field and world map text code.
- `PartyCharacterId`: party character ids (`PARTY_CHAR_CLOUD`..`PARTY_CHAR_SEPHIROTH`, `PARTY_CHAR_CHOCOBO`, `PARTY_CHAR_NONE`).

`main.cpp` also uses `RENDER_STATE_TEXTURE_WRAP` (`lib/enums/graphics.h`) and `SOUND_*` commands
(`lib/enums/sound.h`).

## Notes


- `main.cpp` reads `g_FieldActorState` and `gSavemap` through their canonical struct types;
  `g_FieldEntityState` is a `FieldEntity` array; `g_FieldEntryMapId` is the map id the field
  module enters (the savemap's `locationId` on a load).
