# Menu: the field menu, battle menu, shops and save/load screens

This is the game's menu layer. It draws every window, text string, cursor and
number the player reads outside the 3D scenes. It runs as its own engine module
for the title screen's load menu, the field menu (Item, Magic, Materia, Equip,
Status, Limit, Config, PHS, Save, Quit), shops, name entry and the party
switch: `GameModeDispatcher_Frame` (main.cpp) and `WorldMap_ModuleFrame` install
`Menu_ModuleEnter` / `Menu_ModuleFrame` / `Menu_ModuleExit` with `RequestModuleSwitch`. When the screen is
done, `Menu_ModuleFrame` picks the next game mode and switches back to the world
map module or to the game mode dispatcher. The battle menu is not a module of
its own: `Battle_ModuleFrame` (battle.cpp) calls `Battle_UpdateUiAndLogic` four
times a frame while `g_MenuEntryState` is 2. The same TUs also hold the text, window and cursor helpers that the field
dialogue, the world map and the chocobo race call.

## Where the code is

| folder | TUs | functions | what is in it |
|---|---|---|---|
| `btlmenu/english/` | 9 | 286 | battle menu (windows, target selection, HUD lists), the menu text, window and cursor helpers, the scripted input stream, the text character tables |
| `english/` | 23 | 392 | the module callbacks, screen tables, main menu, popups, battle result screens, the field-menu screens (Item, Magic, Materia, Equip, Status, Limit, PHS, shop, name entry, save, quit), Config and input binding screens, menu textures, the kernel text lookup; load menu and save file I/O |

Totals: 32 TUs, 678 functions. Two files keep their retail names from
`__FILE__` strings: [btlmenu/english/callback.cpp](btlmenu/english/callback.cpp)
(`C:\FF7\src\menu\btlmenu\english\callback.cpp`, 0x006ce7d0) and
[english/loadmenu.cpp](english/loadmenu.cpp) (`C:\FF7\src\menu\English\loadmenu.cpp`,
0x00720ef0). The other 30 are named after what they contain, and their place in the link order is
inferred from their neighbours. Of the 22 TUs between those two, the 14 field-menu screens and
savemap helpers live in `english/` as `menu_*.cpp`, while the 8 battle-menu and shared-helper TUs
live in `btlmenu/english/`; for these the folder is not known from the EXE. TU boundaries follow
retail's objects.

Six TUs in `english/` hold menu screens and helpers; five sit in the address run 0x006bf4e0..0x006ce7d0
directly before `callback.cpp`:

| TU | functions | what it holds |
|---|---|---|
| [menu_main_and_battle_results.cpp](english/menu_main_and_battle_results.cpp) | 113 | the module callbacks, the screen tables, the main menu, popups, battle result screens, savemap and inventory helpers, field script services |
| [menu_config_screen.cpp](english/menu_config_screen.cpp) | 7 | Config screen, window colour editor, volume sliders |
| [menu_input_config.cpp](english/menu_input_config.cpp) | 10 | input binding screen, `ff7input.cfg` load and save |
| [menu_render_objects_portraits.cpp](english/menu_render_objects_portraits.cpp) | 9 | font, window and portrait textures |
| [menu_quit_confirm_dialog.cpp](english/menu_quit_confirm_dialog.cpp) | 13 | the quit confirmation dialog |

The sixth, [menu_kernel_text_pool.cpp](english/menu_kernel_text_pool.cpp) (kernel
text lookup, `Kernel_LookupUiText`; 175 calls from menu, none from lib/file), sits
at 0x00419360 among the library objects. Its neighbour
[file_input_poll_bindings.cpp](../../../lib/src/file/file_input_poll_bindings.cpp) (`Input_*`; 140
calls from menu, but also called by chocobo, coaster, field and graphics) is a
game-wide input layer and is in lib/file. See
[../../../lib/src/file/README.md](../../../lib/src/file/README.md).

The snowboard minigame's five TUs (0x00722c10..0x00731aa0, 216 functions) live in [../../snobo/](../../snobo/README.md). `Mini_SnowboardEnter` (0x00722c10) is the
module enter that main.cpp installs for `GAME_MODE_SNOWBOARD` and
`GAME_MODE_SNOWBOARD_2`, and the run ends
where `C:\FF7\snobo\tmd.cpp` (0x00731aa0) begins. They are described in
[../../snobo/README.md](../../snobo/README.md) and are left out below.

## Entry points

| function | address | what it does | called from |
|---|---|---|---|
| `Menu_ModuleEnter` | 0x006cd3b0 | loads the high-res (`*_h.tim`) font and window textures (`Menu_LoadFontAndWindowTextures(1)`), party portraits for some entries, sets the 320x240 or 640x480 viewport from `GetUiScalingMode`, sets `Menu_SetRequestedScreen(1)` | main.cpp `GameModeDispatcher_Frame` (modes `GAME_MODE_TITLE_MENU`, `GAME_MODE_MENU`, `GAME_MODE_MENU_AFTER_BATTLE`), `WorldMap_ModuleFrame` |
| `Menu_ModuleFrame` | 0x006cc623 | the menu main loop, one frame (see Data flow) | engine module table |
| `Menu_ModuleExit` | 0x006cd5b3 | frees the save buffer when the title or save menu ran, resets the batches, reloads the low-res (`*_l.tim`) textures with `Menu_LoadFontAndWindowTextures(0)` | engine module table |
| `Menu_ModuleMouse`, `Menu_ModuleKeyboard` | 0x006cc619, 0x006cc61e | empty in retail | engine module table |
| `Menu_DispatchScreenUpdate` | 0x006cda83 | picks the sub-module from the game mode and `g_FieldActorState.state.requestCode` | `Menu_ModuleFrame` |
| `Menu_RunTopLevelFrame` | 0x006cb56a | the field menu: first-frame setup, popup, then the current screen's frame callback | `Menu_DispatchScreenUpdate` |
| `Menu_LoadScreenModuleUpdate` | 0x00722393 | title screen load menu; its return chooses new game or continue | `Menu_LoadMenuEntryThunk` |
| `Menu_SaveScreenModuleUpdate` | 0x006fffaa | save screen opened by the field | `Menu_SaveMenuEntryThunk` |
| `Menu_ShopModuleUpdate` | 0x0071ff95 | shop screen for one shop id | `Menu_ShopMenuEntryThunk` |
| `Menu_PhsModuleUpdate` | 0x00701ee4 | party switch screen | `Menu_PhsMenuEntryThunk` |
| `Menu_NameEntryModuleUpdate` | 0x00719c08 | character name entry | `Menu_NameEntryMenuEntryThunk` |
| `Menu_CreateLoadScreenObjects` | 0x006c0f60 | creates the window background render objects | main.cpp, before the title menu |
| `Menu_RefreshPartyLimitTables` | 0x006cbd1e | rebuilds the party limit records (`Menu_RebuildPartyLimitRecords`) | main.cpp (new game, continue, menu, battle swirl) |
| `Script_SvcHarvestMateria` .. `Script_SvcReplaceMateriaGroup` | 0x006cbd82.. | field script services that move materia and equipment | main.cpp `GAME_MODE_FIELD_SERVICE`; the backup and restore pair from `Menu_DispatchScreenUpdate` |
| `Menu_QuitConfirmDialogRun` | 0x006c0e2d | quit confirmation dialog | stored by main.cpp at engine context +0xA58 |
| `Battle_LoadUiTextures`, `Battle_FreeUiTextures` | 0x006cea17, 0x006cedad | battle UI textures | `Battle_ModuleEnter`, `Battle_ModuleExit` |
| `Battle_UpdateUiAndLogic` | 0x006ce8b3 | one battle UI frame: cursors, open windows, input, pause, ATB tick | `Battle_ModuleFrame` |
| `Battle_DrawUiLists` | 0x006cee84 | submits the battle UI batch lists | `Battle_ModuleFrame` |
| `Battle_OpenWindow` | 0x006d0af9 | opens battle window `id` and runs its open handler | battle_arena_penalty_rounds.cpp, battle_opening_actor_setup.cpp |
| `Battle_SetMessageWindowRequest` | 0x006d721c | queues a battle message by text id (-1 clears) | battle_command_ring_atb.cpp, battle_actor_overlay_draw.cpp, battle_load_scene_rounds.cpp, battle_end_sequence.cpp |
| `Menu_RenderMessageWindows`, `Menu_FlushUiBatches`, `Menu_SetMessageWindowYOffset` | 0x006ebf2c, 0x006eca68, 0x006f19f3 | field and world map dialogue windows | field_engine_model_anim.cpp, wm_world_module_render.cpp |
| `Menu_DrawEncodedString2x`, `Menu_DrawEncodedString1x` | 0x006f5b03, 0x006fab2f | draw an FF7-encoded string, 640x480 and 320x240 | menu TUs, chocobo |
| `Menu_DrawWindowFrame2x`, `Menu_DrawWindowFrame1x` | 0x006e7d20, 0x006ede1d | window frame from an `EnglishWindowFrame` | menu TUs |
| `MenuCursor_Init`, `MenuCursor_Update` | 0x006f4d30, 0x006f4db2 | menu cursor block setup and per-frame movement | menu TUs |

## Main structures

| type or global | header | what it holds |
|---|---|---|
| `g_MenuScreenInitCallbacks[22]`, `g_MenuScreenFrameCallbacks[22]` | [lib/api/graphics.h](../../../../include/lib/api/graphics.h) | init and frame callback per field-menu screen id: 0 main, 1 item, 2 magic, 3 materia, 4 equip, 5 status, 6 stub screen 6, 7 limit, 8 config, 9 PHS, 10 save, 11 quit, 12 stub screen 12, 13..20 more item, limit and equip entries |
| `g_MenuCurrentScreenId`, `g_MenuPreviousScreenId` | defined in menu_main_and_battle_results.cpp | current and previous screen id; `Menu_SetScreen` (0x006c6aee) moves current to previous |
| `MenuCursor`, `MenuMainCursorSet` | [ff7/types/menu.h](../../../../include/ff7/types/menu.h) | 14-int (0x38-byte) cursor block filled by `MenuCursor_Init`, and the main-menu cursor set `g_MenuMainCursors` |
| `EnglishWindowFrame` | [ff7/types/menu.h](../../../../include/ff7/types/menu.h) | x, y, width, height as shorts for `Menu_DrawWindowFrame2x` / `Menu_DrawWindowFrame1x` |
| `MenuPopupBodyRect` | [ff7/types/menu.h](../../../../include/ff7/types/menu.h) | popup body rectangles for `Menu_DrawPopupMessageBody2x` / `Menu_DrawPopupMessageBody1x` |
| `FontWindowHandles` | [ff7/types/menu.h](../../../../include/ff7/types/menu.h) | font and window texture handles filled by `Battle_LoadFontAndWindowTexturesInto`; battle uses them too |
| `g_Menu*GraphicsObject` at 0x00DC0FC8 .. 0x00DC1054 (`g_MenuWinABlend4GraphicsObject` .. `g_MenuWinBlend1GraphicsObject`) | defined in menu_main_and_battle_results.cpp, declared in [ff7/api/menu.h](../../../../include/ff7/api/menu.h) | render objects and batch lists of the menu: fonts, window blends, window backgrounds, portraits, the title screen texture |
| `BattleUiBox` | [ff7/battle.h](../../../../include/ff7/battle.h) | one 0x98-byte battle window: position, size, half size, up to 16 sub-rectangles; `g_BattleMenuData` points to the array |
| `g_BattleWindowStates` | defined in battle_window_panels.cpp | state of the 32 battle windows: 0 closed, 1 opening, 2 active, 3 closing |
| `g_BtlMenuWindowOpenHandlers[0x20]`, `g_BtlMenuWindowActiveHandlers[0x20]`, `g_BtlMenuWindowUnusedHandlers[0x20]` | defined in battle_window_panels.cpp | per-window open and active handlers (`BattleUiHandlerFn`), and a third table no code reads, of functions that only return |
| `WindowData` | [ff7/window.h](../../../../include/ff7/window.h) | the 0x30-byte dialogue window `Menu_RenderMessageWindows` draws (the field's `g_FieldWindows`); the function reads it through a local struct of its own names |
| `gSaveFileBuffer` | [ff7/api/menu.h](../../../../include/ff7/api/menu.h) | pointer to the 0xFE4C-byte heap buffer from `Menu_SaveFileBufferAlloc`: one save file, 15 slots of 0x10F4 bytes |
| `gSavemap` | [ff7/savemap.h](../../../../include/ff7/savemap.h) (base 0x00DBFD38) | the live savemap; save and load copy 0x10F4 bytes of it; the first dword holds the checksum |
| `MenuInputBindingTable`, `MenuPartyStatBlock`, `MenuShopRecord` | [ff7/types/menu.h](../../../../include/ff7/types/menu.h) | fixed-size input-binding, party-stat and shop inventory records |

## Data flow

Loading:
- `menu/menu_us.lgp` is opened by `App_InitSubsystems` (main.cpp) at startup.
- `Menu_LoadFontAndWindowTextures(hiRes)` (menu_render_objects_portraits.cpp) loads
  `usfont_a_*`, `usfont_b_*` and `btl_win_a_*` .. `btl_win_d_*` in UI scaling
  mode 2, otherwise `usfont_*` and `btl_win_*`; `hiRes` picks the `_h.tim` or
  `_l.tim` file. Which archive the names resolve in is not shown by this code.
  `Menu_LoadCharPortrait` loads `<name>.tim` in UI scaling mode 2 and
  `<name>_l.tim` otherwise (cloud, barre, tifa, earith, red, yufi, ketc, bins,
  cido, pcloud, pcefi; choco has its own entry).
- Text comes from `Kernel_LookupUiText` (menu_kernel_text_pool.cpp). Its groups
  are `KernelTextGroup` in [lib/enums/file.h](../../../../include/lib/enums/file.h),
  which says they map to kernel2.bin banks. The save screen texts are FF7-encoded
  tables in loadmenu.cpp (`g_SaveLoadTextRows`).
- Equipment data is read from the kernel tables at
  `g_KernelArmorDataArray` and `g_KernelWeaponDataArray`, arrays of
  `struct KernelArmor` (0x24) and `struct KernelWeapon` (0x2C) in `include/ff7/types/menu.h`
  (`Savemap_GetPartyArmourRecord`, `Savemap_GetPartyWeaponRecord`).

One menu frame, `Menu_ModuleFrame`:
1. `Menu_ThrottleFrameRate` busy-waits on the timestamp counter when the last
   frame was on time.
2. `Input_Update(0)` unless a scripted input stream is active
   (`g_ScriptedInputActive`, which `Menu_RunTopLevelFrame` sets when it is
   given a stream).
3. Clear, `Gfx_BeginSceneCall`, `Menu_DispatchScreenUpdate`,
   `Menu_SubmitDrawLists`, `Gfx_EndSceneCall`. When the throttle reported a late
   frame (`g_MenuFrameThrottleFlag`), the first pass runs with text disabled and a
   second input/dispatch/submit pass follows in the same scene.
4. `SoundUpdateFades`.

`Menu_DispatchScreenUpdate` chooses:
- `GAME_MODE_TITLE_MENU`: `Menu_LoadMenuEntryThunk` -> `Menu_LoadScreenModuleUpdate`.
- `GAME_MODE_MENU` by `g_FieldActorState.state.requestCode`: name entry, party select
  (PHS), shop, main menu, save menu, or the party/equipment backup and restore
  services. `g_FieldActorState.state.eventCmdParam` is the argument.
- `GAME_MODE_WORLD_MAP`: `Menu_RunTopLevelFrame(0)`.
- First, when `g_MenuEntryState` is 4, `Battle_DrawResultScreenFrame`
  (battle_end_sequence.cpp sets that global from `g_BattleEndResultScreenFlag`).

The field menu (`Menu_RunTopLevelFrame`):
- First frame: repeat rates, popup placement, `Savemap_BuildPartyStatsFromRecord`,
  `Battle_LoadCombatantFromSavemap` and `Party_RecomputeBattleModifiers` for each
  party slot, `Menu_MainScreenInit`. When the field passes a script stream
  (`g_FieldActorState.state.eventCmdParam` is 1) it calls `Input_SetScriptedInputStream`,
  and while the stream runs a hint window (`g_MenuTutorialWindowTextPtr`)
  is drawn.
- Every frame: `Input_SnapshotToFieldGlobals`, the popup, then
  `g_MenuScreenFrameCallbacks[screen]` (the previous screen's during phases 2,
  4 and 5). The main screen's command window
  (`Menu_DrawCommandWindow2x` / `Menu_DrawCommandWindow1x`) runs `g_MenuScreenInitCallbacks[screen]` when
  its collapse animation ends.
- Screens read and write the savemap directly: party (`gSavemapParty`),
  character records (0x84 bytes each), inventory, materia stock (200 entries),
  gil, window colours.

Drawing:
- Draw helpers acquire batch nodes (`AcquireBatchNode`) and write quads into
  the menu's batch lists. Nearly every helper has a 2x (640x480) and a 1x
  (320x240) twin; `g_MenuIsSmallViewport320x240`, set in
  `Menu_ModuleEnter`, picks one.
- `Menu_SubmitDrawLists` emits the lists in two scene passes with
  `Gfx_EmitPrimitiveBatchList` and sets the viewport. Nothing is drawn unless the
  text subsystem flag (`Menu_IsTextSubsystemReady`) is set.
- Sound effects go through `SfxPreloadAndPlayOnChannel5` and `SfxPlayOnChannel`.

Save and load (loadmenu.cpp, menu_save_screen.cpp, menu_string_copy_savemap.cpp):
- `Menu_ScanSaveDirectory` creates `<app>save` if needed and probes
  `save/save00.ff7` .. `save09.ff7`.
- A file is a dword 0x06277371, one byte, a dword slot mask, then 0xFE4C bytes
  (`Menu_LoadSaveFileToBuffer`, `Menu_WriteSaveSlot`).
- Save: `Savemap_BuildPreviewHeader`, then `Menu_SaveChecksumCrc16(0x10F0, ...)` into
  the savemap's first dword, then the 0x10F4-byte savemap is copied into its slot
  of `gSaveFileBuffer` and the whole file is written.
- Load: `Menu_RestoreSaveSlotToSavemap` copies the slot back and reloads the window
  colours; `Menu_LoadScreenTick` compares the stored checksum and opens an error popup
  when it differs.

Battle menu (`Battle_UpdateUiAndLogic`, callback.cpp):
- draws the target cursor and the open windows (`Battle_DrawOpenWindows2x` / `Battle_DrawOpenWindows1x`),
  calls `Input_Update`, then `Battle_ProcessCommandMenuInput` unless paused.
- `Battle_ProcessCommandMenuInput` walks the 32 windows: state 1 grows the box
  (`Menu_BattleWindowOpenStep`), 2 runs its active handler, 3 shrinks it
  (`Menu_BattleWindowCloseStep`). In `BATTLE_MENU_SELECT_TARGET` it runs
  `Battle_LoadTargetMaskForMode` and `Battle_UpdateTargetSelection`.
- Start toggles pause (`g_GamePauseLatch`) when
  `g_BattleSceneState` is 4, and sends sound command
  0x99 or 0x98; `Battle_TickAtbAndGauges` runs while not paused.

Leaving: when a sub-module sets `g_MenuExitRequested`, `Menu_ModuleFrame` sets
the next mode (title: `GAME_MODE_NEW_GAME` or `GAME_MODE_CONTINUE_GAME` from the load menu's result;
field menu: `Savemap_ReconcileFieldParty` and back to field mode), frees the
party portraits after the field menu and the world map menu, and switches to `WorldMap_Module*` when `g_WorldMapInMenu` is set
or to `GameModeDispatcher_*` otherwise.

## Named codes

[include/ff7/enums/menu.h](../../../../include/ff7/enums/menu.h):

| enum | used in | what it names |
|---|---|---|
| `ItemId` | menu_item_sort_screen.cpp | items whose field use `Menu_ItemScreenFrame` handles |
| `MateriaId` | menu_materia_screen.cpp, menu_shop_screen.cpp | materia special-cased by the detail panels |
| `EquipKind` | menu_equip_screen.cpp | weapon, armor, accessory slot under the equip cursor |
| `SlotGrowth` | menu_equip_screen.cpp, menu_shop_screen.cpp | materia slot AP growth (none, normal, double, triple) |
| `MateriaMenuState` | menu_materia_screen.cpp | input states of `Menu_MateriaScreenFrame` |
| `ShopMenuState` | menu_shop_screen.cpp | input states of `Menu_ShopScreenFrame` |
| `BattleMenuState` | battle_window_panels.cpp | battle menu state, equal to the active battle window id |

Also used: `GameMode` and `FieldRequestCode` (`FIELD_REQUEST_*`) from
[ff7/enums/main.h](../../../../include/ff7/enums/main.h), `InputButton` and
`KernelTextGroup` from lib/enums/file.h, render states and blend modes from
[lib/enums/graphics.h](../../../../include/lib/enums/graphics.h).

## Notes

What reading the code showed:

- Shop and input state globals: in `Menu_ShopModuleUpdate`, `g_MenuShopStarted`, `g_MenuShopFrameCounter`, `g_MenuShopFrameParity`, and `g_MenuShopPhase` (0 fading in, 1 running, 2 fading out, -1 done); `Menu_LoadScreenTick`'s delay `g_LoadMenuInputDelay`; `g_ScriptedInputActive` and `g_pInputScriptStream`.
- Raw reads `*(T *)(p + 0xNN)` and pointer casts are densest in
  menu_text_primitives.cpp and message_window_draw.cpp (about 760 lines
  with a pointer cast in each; the vertex writers) and battle_window_panels.cpp.
- Fixed-size aggregate-copy helpers (`Int64Pair`, `Int32Value`) are used where VC5 `/Od` emits struct moves. The battle per-actor cursor blocks (0x700 bytes) are written as `p[n]` dwords.
- Gotos are in `Menu_ItemScreenFrame`,
  `Menu_MateriaScreenFrame` and `Battle_LoadTargetMaskForMode`. Each carries a `goto kept` comment saying
  why no structured form gives the same code.
- Large frame functions are still hard to follow: `Menu_MagicScreenFrame`,
  `Menu_ItemScreenFrame`, `Menu_ShopScreenFrame` (each about 770 to 820 lines),
  `Menu_DrawBattleWindow`, `Menu_LoadScreenTick`, `Menu_SaveScreenTick`.

## Data-only files

Data runs that their link neighbour never references, each in a file of its own:

| file | span | next to | what it holds |
|---|---|---|---|
| `english/menu_kernel_tables.cpp` | .bss 0x00db9580..0x00dc0e78 | `menu_quit_confirm_dialog.cpp (before it)` | the token state, the kernel command/attack/item/weapon/armour/accessory/materia tables, the menu party stats, `gSavemap` |
| `btlmenu/english/battle_menu_return_state.cpp` | .data 0x0091ef90..0x0091ef9e | `battle_target_selection.cpp` | the battle menu's return state, window frame id and menu state |
