# Field: the walkable maps, their scripts, dialog and transitions

The field module runs the maps the party walks on: a pre-rendered background,
3D character models on a walkmesh, the per-map event script, message windows,
and the doors and gateways out. The top-level dispatcher in
[../src/main/main.cpp](../src/main/main.cpp) enters it: `GameModeDispatcher_Frame`
(0x004090e6) installs `Field_ModuleEnter`, `Field_ModuleFrame`,
`Field_ModuleMouse` and `Field_ModuleKeyboard` when the game mode is
`GAME_MODE_FIELD`. The field leaves the same way. Each frame, `Field_UpdateLogic`
turns a script request into the next game mode and an exit request, and
`Field_ModuleFrame` hands control back through `Field_ReturnToDispatcher`.
A jump to another field map does not leave the module. `Field_ModuleFrame`
reloads the map in place.

## Where the code is

- `src/ff7/field/src/`: 43 TUs, 711 functions.
- 12 files keep their retail names from `__FILE__` strings (`C:\FF7\field\src\ad_*.cpp`,
  `tutaddr.cpp`, `ad_human.CPP`). The other 31 have unknown original names: their file names
  describe the first function, and their place in the link order is inferred from their neighbours.
- TU boundaries follow retail's objects, even where one
  object holds unrelated code (for example `field_script_operands.cpp` also
  holds the `Field_File*` FILE wrappers that chocobo and condor call).

| TU | what it holds |
|---|---|
| `ad_app.cpp` | module callbacks, map config table, display modes, splash textures |
| `field_script_operands.cpp` | operand banks, all 238 opcode handlers, `Savemap_*` party helpers |
| `field_file_io.cpp` | the `Field_File*` FILE wrappers |
| `ad_data.cpp` | map list, section parsers, tile sprites, background parser |
| `ad_image.cpp` | background compositing and clipping, palette upload, adjacent-map preload |
| `ad_cdr.cpp` | LZS decompressor, flevel entry loader, section pointers |
| `field_empty_stubs.cpp` | four empty functions |
| `field_message_window.cpp` | the four message windows (`g_FieldWindows`) and text formatting |
| `ad_human.cpp` | four 0xA0-byte actor buffers; entity movement, walkmesh steps, collision, line, gateway and door triggers |
| `field_engine_model_anim.cpp` | frame throttle, render frame, model loading and per-frame model update |
| `tutaddr.cpp` | tutorial text (`*.tut`) |
| `field_fx_fade_overlays.cpp` | screen fades and colour blends (`Field_UpdateFieldFx`) |
| `field_exit_map_requests.cpp` | exit request, map-state reset, `Field_UpdateLogic` |
| `ad_obj.cpp` | model list (section 2), model loading and freeing |
| `ad_tile.cpp` | the 42 tile texture pages; texture pages and the four background layers |
| `ad_ddraw.cpp` | DirectDraw surfaces, locks, blits, present |
| `ad_bk.cpp` | background teardown, camera transitions, screen shake, camera scroll |
| `field_line_draw.cpp` | script lines (`g_FieldLines`) drawn as a line batch |
| `field_indexed_record_command.cpp`, `field_execute_model.cpp`, `field_model_vertex_lighting.cpp`, `field_apply_*.cpp`, `field_model_ramp_delta.cpp`, `field_run_polygon.cpp`, `field_container_lighting.cpp` | model commands, vertex colour and lighting effects |
| `field_request_and_sound.cpp` | request and fx block pointers, door sound |
| `ad_list.cpp` | background quad banks |
| `ad_pal.cpp` | 16-bit pixel conversions; palette records, brightness tables, palette dirty flags |
| `field_input_buttons.cpp` | button latches and `Field_ReadInput` |
| `field_math_psx_helpers.cpp`, `field_make_point.cpp` | fixed-point vector helpers, PSX TPage/CLUT decoders |
| `field_gateway_arrows.cpp` | `Field_DrawGatewayArrows` |

### Field code in other folders

`field_map_script_state.cpp` (`Field_LoadMapByName`, `Script_BeginMap`, `Script_UpdateFrame`, `Script_ResetState`, `Script_RunAllInitScripts`, `Script_ExecuteFrame`, `Script_TriggerEntityScript`, `Field_UpdateModelAnimStates`, the hand cursor) and `field_encounter_random.cpp` (`Field_CheckRandomEncounter`, called from `Field_UpdateEntities`) live here; two adjacent pieces are in other folders:

- [../condor/condor_unit_hud_readouts.cpp](../condor/condor_unit_hud_readouts.cpp) defines
  `g_FieldScriptOpTable`, the 256-entry opcode table. The rest of the TU is condor code.
- `Script_Svc*`, the handlers for the materia and party service requests, are in
  [../src/menu/english/menu_main_and_battle_results.cpp](../src/menu/english/menu_main_and_battle_results.cpp).

### Neighbouring code

The adjacent highway module TUs live in [ff7/highway](../highway/README.md), and the two adjacent battle data TUs (the dispatch tables in `battle_limit_fx_thunks.cpp` at 0x008fe398 and `battle_camera_script_tables.cpp` at 0x00900198) live in [ff7/battle](../src/battle/README.md).

`field_model_init_tables.cpp` holds `Field_InitModelRuntimeTables`, called from `Field_ModuleEnter`
to populate the field model command tables (`field_execute_model.cpp`, `field_container_lighting.cpp`):
the 18 light caches `g_FieldModelLightCaches`, the sine/cosine table `g_FieldTrigTable`, the unit
vectors and the two index lists.

## Entry points

| function | address | what it does | called from |
|---|---|---|---|
| `Field_SetDisplayMode` | 0x0060d837 | sets the viewport and centre globals for display mode 0, 2 or other | `App_InitSubsystems` (0x004089c5) |
| `Field_InitModuleResources` | 0x0060df96 | builds the map config table, loads `MAPLIST`, allocates actor and list buffers, splash and fx render objects | `App_InitSubsystems` |
| `Field_ModuleEnter` | 0x0060e0bd | enter callback: loads the current map unless coming back from the menu (mode 5) or a field service (mode 0xD), then `Field_InitMapRuntime` | `GameModeDispatcher_Frame` (installed) |
| `Field_ModuleFrame` | 0x0060e5b7 | frame callback: `Field_UpdateLogic`, then render, or tear down and reload the next map | engine frame loop (installed) |
| `Field_ModuleMouse` | 0x0060ed31 | stores the mouse position in the engine context | installed |
| `Field_ModuleKeyboard` | 0x0060ed79 | empty in the shipped build | installed |
| `Field_ReturnToDispatcher` | 0x0060e521 | optionally `Field_ShutdownModule`, then `RequestModuleSwitch` back to `GameModeDispatcher_*` | `Field_ModuleFrame`, `Field_ModuleCatchUpFrame`, `Field_AbortToQuit`, `Field_ModuleEnter` |
| `Field_ShutdownModule` | 0x0060e45d | frees per-map buffers; keeps the field file when the next mode is battle | `Field_ReturnToDispatcher`, `App_Shutdown` (0x00408edc) |
| `Field_FreeModuleResources` | 0x0060e06f | frees what `Field_InitModuleResources` made | `App_Shutdown` |
| `Field_UpdateLogic` | 0x0063c17f | the per-frame logic step and the request-to-mode switch | `Field_ModuleFrame`, `Field_ModuleCatchUpFrame` |
| `Script_ExecuteFrame` | 0x0060c94d | the script scheduler (in `field_map_script_state.cpp`) | `Script_UpdateFrame` |
| `Field_RenderFrame` | 0x006388ee | `Field_RenderModels`, frame throttle, batch pool reset | `Field_ModuleFrame` |
| `Field_RenderModels` | 0x0063a60b | draws background, models, cursor, splashes, lines, fade, message windows | `Field_RenderFrame` |
| `Field_LoadFieldFile` | 0x00630734 | reads and decompresses one `flevel.lgp` entry | `Field_LoadMapByName` |
| `Field_GetSectionPtr` | 0x006308ca | pointer to section N of the loaded field file | section parsers, `Field_LoadMapByName` |
| `Savemap_ReconcileFieldParty` | 0x0061c577 | by its name: brings the field party in line with the savemap | `Field_UpdateLogic`, `Script_OpFieldRequest`; ff7/main, lib/graphics |
| `Field_FileOpen` .. `Field_FileWrite` | 0x00620510 .. 0x006206b3 | `FILE *` wrappers | ff7/chocobo, ff7/condor, and the field data dumps of `ad_data.cpp`, `ad_obj.cpp`, `ad_tile.cpp` |
| `Field_ResetFrameCounter` | 0x0060e5a8 | clears `g_FieldFrameCounter` | ff7/movie |

## Main structures

| type or global | header | what it holds |
|---|---|---|
| `FieldState`, `*g_pFieldState` | [include/ff7/field.h](../../../include/ff7/field.h) | the script-facing field state; `Script_BeginMap` points it at `g_FieldActorState` (0x00cc0d88). `requestCode` (+0x01) and `eventCmdParam` (+0x02) carry the field request; key masks, shake, layer scroll, blocked accesses. Field members are named directly. |
| `FieldEntity`, `*g_pFieldEntities` | [include/ff7/field.h](../../../include/ff7/field.h) | 0x88 bytes per model. `g_FieldEntityState` is `FieldEntity g_FieldEntityState[16]` and is read as `g_FieldEntityState[i].posX`; `Field_BeginMapScripts` passes it. The entity code in `ad_human.cpp` and `field_map_script_state.cpp` still add offsets: their own `g_FieldLines` declaration clashes with `ff7/field.h`. |
| `FieldLine`, `g_FieldLines[0x20]` | [include/ff7/field.h](../../../include/ff7/field.h) | 0x18-byte script lines with talk, touch and cross flags |
| `FieldRequest`, `*g_pFieldRequest` | [include/ff7/field.h](../../../include/ff7/field.h) | code and six args; bound by `Field_BindRequestBlock`, passed to `SoundDispatchCommand` |
| `g_FieldScripts`, `g_FieldScriptPC[52]`, `g_FieldScriptWait`, `g_CurrentEntity` | [include/ff7/field.h](../../../include/ff7/field.h) | the loaded section 0, one program counter and one wait counter per entity, the running entity |
| `FieldScriptHeader` | [include/ff7/types/condor.h](../../../include/ff7/types/condor.h) | entity count and script-table offset of section 0; it sits in the condor header |
| `g_FieldScriptOpTable[256]` | declared per TU | the opcode dispatch table (238 distinct handlers, 13 slots point to `Script_OpUnimplementedStall`) |
| `WindowData`, `g_FieldWindows[4]` | [include/ff7/window.h](../../../include/ff7/window.h) | the four 0x30-byte message windows at 0x00cff5b8, defined in `field_message_window.cpp` |
| `g_pFieldFileBuffer`, `g_FieldSectionOffsets[9]` | defined in `ad_cdr.cpp` | the decompressed field file and its section offsets |
| `FieldMapRecord` (0xD0 bytes), `g_FieldMapRecords[1200]` | [include/ff7/types/field.h](../../../include/ff7/types/field.h) | one config record per map id at 0x00cc2280, defined in `ad_data.cpp`: near plane, per-model y/z offsets and visibility, background offsets, the paused-animation entity, file size, clear colour. `Field_InitMapConfigTable` fills all 1,200, `Field_ApplyMapConfig` copies the current map's into the field globals |
| `FieldModelSlot` (0x190 bytes), `g_pFieldModelSlots` | [include/ff7/types/field.h](../../../include/ff7/types/field.h) | one record per model; `animations`, `model` (anim player) |
| `FieldModelInstance` | [include/ff7/types/field.h](../../../include/ff7/types/field.h) | a view of the anim player at model record +0x178 as `Field_RenderModels` uses it (enabled, skeleton, frame data, draw data) |
| `FieldScreenShake`, `FieldScriptLineTrigger`, `FieldGateway`, `FieldDoorTriggerRecord` | [include/ff7/types/field.h](../../../include/ff7/types/field.h) | screen-shake axis state, line and gateway tests, door segments |
| `GameModule` | [include/lib/types/graphics.h](../../../include/lib/types/graphics.h) | the seven-callback module record `Field_ReturnToDispatcher` hands to `RequestModuleSwitch` |
| `PfileLoadOptions` | [include/lib/pfile.h](../../../include/lib/pfile.h) | render-object and model load options used by `Field_InitMapRuntime` and `Field_InstantiateModels` |

The functions and globals other TUs use are declared in [include/ff7/api/field.h](../../../include/ff7/api/field.h).

## Data flow

Start-up, once:

1. `Field_InitModuleResources` runs `Field_InitMapConfigTable`, which seeds the
   per-map records and calls `Field_LoadFieldSizeTable` to read `flevel.siz` from
   archive 1 (`field/flevel.lgp`, see `BuildArchivePath`). Then it reads
   `MAPLIST` from the same archive (`Field_LoadMapListFromLgp`), allocates the
   actor buffers (`Field_AllocHumanBuffers`) and creates the splash textures
   `sibuki1..4.tim` (`Field_LoadSplashTextures`) and the fx render objects.

Entering a map (`Field_ModuleEnter`):

1. The map id comes from `g_FieldEntryMapId`. `Field_GetMapNameById`
   gives the name and the path becomes `name\name.ate`.
2. `Field_LoadMapByName` (`field_map_script_state.cpp`) applies the map config and calls
   `Field_LoadFieldFile`. That reads the `flevel.lgp` entry, decompresses it with
   `Field_DecompressLzs` into a buffer of the size from `flevel.siz` (plus
   2,000,000), and records the section offsets. Section 0 becomes
   `g_FieldScripts`.
3. `Field_ParseAllSections` points the globals at the other sections: 3 palette,
   5 tile map, 7 triggers, 1 camera (`SetGeomScreen`), 6 encounters, 4 walkmesh,
   2 model list (`Field_ParseModelSection`). Each call also builds a file name
   (`.fpl`, `.map`, `.inf`, `.ca`, `.enc`, `.id`, `.fcc`); the section parsers
   ignore it. The `.mim`/`.fcr` branch runs only when `g_FieldLoadAssets` is 0,
   and `Field_ModuleEnter` sets it to 1.
4. `Field_InitMapRuntime` uploads the palette, allocates tile records, parses
   section 8 (`Field_ParseBackgroundSection`), creates the texture pages and
   loads models and animations (`Field_InstantiateModels`). Then it calls
   `Field_ResetMapState`. On a fresh entry (most previous modes) that resets
   camera and scroll, places the player on the walkmesh
   (`Field_PlacePlayerOnWalkmesh`), initialises the door triggers and starts
   the scripts through `Field_BeginMapScripts` -> `Script_BeginMap`.
5. `Script_BeginMap` resets the message windows and the VM state
   (`Script_ResetState`, which clears the 0x100-byte temp bank), resets the
   savemap for a new game when needed (`Savemap_ResetForNewGame`), and runs every
   entity's init script to its first `RET` (`Script_RunAllInitScripts`).
6. Coming back from battle, `Field_ShutdownModule` has kept the field file and
   `Field_ModuleEnter` sets `g_FieldResumingFromBattle`. `Field_LoadFieldFile`
   then returns at once without reloading.

Each frame (`Field_ModuleFrame`):

1. `Field_UpdateLogic` runs a pending service request (mode 0xD:
   `Script_Svc*`), then `SoundUpdateFades`, `Field_ServiceMovieRequest`,
   `Field_UpdateMovieCamera` and `Field_ReadInput`.
2. `Script_UpdateFrame` -> `Script_ExecuteFrame`, unless the mode is already
   `GAME_MODE_MENU`. The scheduler writes the play clock into savemap bytes,
   starts talk, touch and line scripts through `Script_TriggerEntityScript`, then
   gives each entity up to eight opcodes through the dispatch table. A handler
   that returns non-zero ends that entity's turn for the frame.
3. Camera: `Field_BeginCameraTransitionFromRequest`,
   `Field_StepCameraScrollTransition`, `Field_UpdateScreenShakeAxis` (twice),
   `Field_UpdateCameraScroll`.
4. Request codes in `g_FieldActorState.state.requestCode`
   become a game mode in `g_EngineGameMode` and an exit request
   (`Field_RequestExit`). Input bit 0x10 opens the main menu while
   `g_FieldActorState.state.menuLocked` and `g_FieldLogicActive` are clear.
5. `Field_UpdateEntities` moves the player and the scripted walkers on the
   walkmesh (`Field_StepEntityOnWalkmesh`) and rolls random encounters after a
   step (`Field_CheckRandomEncounter`, which writes `FIELD_REQUEST_BATTLE` to
   `g_FieldActorState.state.requestCode` and an id from the encounter section to
   `g_FieldActorState.state.eventCmdParam`). Then line triggers, the talk check
   (`Field_FlagNearestFacedEntityOnInputEdge`), the line batch, gateway arrows
   and `Field_UpdateFieldFx`.
6. If `Field_UpdateLogic` returns 0 and no exit is requested,
   `Field_ModuleFrame` clears to the map colour, runs `Field_UpdateModelInstances`
   and `Field_RenderFrame`. `Field_RenderModels` begins the scene, draws the
   background layers (`Field_DrawBackgroundLayers`) and texture pages, the models
   (`Anim_DrawSkeletonFrame`, lib/polygon), the hand cursor and splashes, the
   script lines and the fade overlay. While `g_FieldOpenWindowCount` is nonzero it also
   draws the four message windows through `Menu_RenderMessageWindows` (ff7/menu).

Leaving:

- Exit request 1 (menu) returns to the dispatcher without `Field_ShutdownModule`.
  Exit request 2 shuts the module down first.
- A map jump (`FIELD_REQUEST_MAP_JUMP`) to id 1..64 sets `GAME_MODE_WORLD_MAP`.
  Any other id makes `Field_UpdateLogic` return 1, and `Field_ModuleFrame` frees
  the map and loads the next one by `g_FieldActorState.state.eventCmdParam` without
  leaving the module.
- `FIELD_REQUEST_MINIGAME` picks a minigame mode from `g_FieldActorState.state.minigameKind` (`enum FieldMinigame`: `FIELD_MINIGAME_HIGHWAY` .. `FIELD_MINIGAME_SNOWBOARD_2`).
- Sound goes out through `SoundDispatchCommand`, `MidiPlaySong` and `SfxStopAll`
  (lib/sound); the script opcodes post most of it through `g_pFieldRequest`.
- Savemap: the operand banks read and write the 0x500-byte script bank at
  savemap +0xBA4 (`Script_ReadOperand8`/`16`, `Script_WriteOperand8`/`16`); see
  [include/ff7/savemap.h](../../../include/ff7/savemap.h).

## What the script opcodes do

Each of a map's 48 entities runs bytecode in eight priority slots (0 most urgent, 7 its main loop).
A handler in `field_script_operands.cpp` reads its operands from the script; `Script_ReadOperand8/16`
and `Script_WriteOperand8/16` resolve banked operands through bank nibbles: nibble 0 is a literal,
the others index the savemap's script variables (blocks 0x000..0x400) or the unsaved temporary
bank. Returning 0 runs the next opcode in the same frame; nonzero ends the entity's turn, and an
opcode that waits returns nonzero without moving the pc. Each handler's header gives the community
mnemonic, the operands in script terms, whether it waits and its length.

- **Flow control:** RET/RETTO, JMPF/JMPB (a backward jump always yields), IFUB/IFSW/IFUW and their
  long forms (`Script_EvalCompare8/16*`), IFKEY/IFKEYON/IFKEYOFF, IFPRTYQ/IFMEMBQ, WAIT, NOP.
- **Calling other entities:** REQ/REQSW/REQEW and PREQ/PRQSW/PRQEW, all through
  `Script_RequestEntityScript`.
- **Variables:** SETBYTE/SETWORD, the arithmetic family (PLUS..MOD2; the `!` forms saturate),
  BITON/BITOFF/BITXOR, SETX/GETX/SEARCHX, RANDOM/RDMSD, SIN/COS.
- **Field requests** (post a request, wait for the engine): BATTLE, MAPJUMP, MINIGAME, MENU (shops,
  PHS, name entry, save, materia services), TUTOR, DSKCG, PMVIE/MOVIE, GAMEOVER; their settings
  BTLMD/BTMD2, BTLTB, BTLON.
- **Windows and text:** WINDOW, WSIZW, WMOVE, WMODE, WROW, WCLS/WCLSE, MESSAGE, ASK, MPARA/MPRA2,
  WSPCL/WNUMB, STTIM, MPNAM, GWCOL/SWCOL.
- **Sound:** AKAO/AKAO2, SOUND, MUSIC/MUSVT/MUSVM/CMUSC (`Field_SubmitMusicRequest`), BMUSC, FMUSC,
  MULCK, CHMST.
- **Models and movement:** CHAR, PC, CC, VISI, SOLID, TLKON, XYZI/XYZ/XYI,
  MOVE/FMOVE/CMOVE/MOVA/PMOVA, JUMP, LADER, OFST/OFSTW, DIR/DIRA/TURA/TURN/TURNGEN/TURNW, MSPED,
  TALKR/SLIDR, the ANIME/ANIM!/CANIM/CANM! families, DFANM, ANIMW/ANIMB, KAWAI/KAWIW, BLINK,
  LINE/SLINE/LINON.
- **Party and inventory:** PRTYP/PRTYM/PRTYE/SPTYE/PTYE (`Savemap_SetFieldParty` and the two-way
  party sync), JOIN/SPLIT, MMBUD/MMBLK/MMBUK, STITM/DLITM/CKITM, SMTRA, GOLDU/GOLDD/CHGLD,
  HMPMAX/MHMMX, HPu/HPd/MPu/MPd, SPECIAL.
- **Camera, background, palettes, screen:** SCR2D/SCR2DC/SCR2DL/SCRLC/SCRLA/SCRLP/SCRCC/SCRLW, VWOFT,
  BGON/BGOFF/BGROL/BGCLR, BGSCR/BGPDH, the palette store (STPAL/LDPAL/CPPAL/RTPAL/ADPAL/MPPAL and
  their range forms), FADE/NFADE/FADEW, SHAKE, MPDSP, IDLCK, MPJPO, UC, MENU2.

Original bugs the handlers note (`BUG:`): IFSW/IFUW narrow the test to a byte, so bitwise tests of
the high byte fail; MINUS2! and DEC2! clamp the wrong way and write -32768; BTMD2 loses its high
byte; PNAME/SPECIAL ignore the destination operand; WNUMB sign-extends a low word of 0x8000 or more;
KAWAI, CANIM and CANM! read uninitialised locals without model data; PMOVA, PDIRA, PTURA, SCRLP and
PXYZI index `g_FieldEntityModel[0xFF]` for an empty party slot.

## Named codes

| enum | header | use here |
|---|---|---|
| `FieldScriptBank` | [include/ff7/enums/field.h](../../../include/ff7/enums/field.h) | operand bank nibbles decoded by `Script_ReadOperand8` / `Script_ReadOperand16` and the writers |
| `FieldCompareOp` | [include/ff7/enums/field.h](../../../include/ff7/enums/field.h) | the `IF` comparison operators |
| `MessageWindowState` | [include/ff7/enums/field.h](../../../include/ff7/enums/field.h) | window state at +0x2C, run by `Field_WindowShowMessage` |
| `FieldRequestCode` | [include/ff7/enums/main.h](../../../include/ff7/enums/main.h) | `FieldState.requestCode`, posted by opcodes, switched on by `Field_UpdateLogic` |
| `GameMode` | [include/ff7/enums/main.h](../../../include/ff7/enums/main.h) | the next module chosen by `Field_UpdateLogic` and tested by `Field_ShutdownModule` |
| `TextCode` | [include/ff7/enums/main.h](../../../include/ff7/enums/main.h) | text control bytes in `Field_WindowStepTextReveal` |

Mode writes and minigame selectors in `Field_UpdateLogic` use `GAME_MODE_*` and `FIELD_MINIGAME_*`.

## Notes

What reading the code shows:

- `FieldState` lives at `g_FieldActorState` (`FieldActorState` in
  `field_script_operands.cpp`, declared in `include/ff7/api/field.h`), and TUs
  access its members through `g_FieldActorState.state`.
- Raw reads: `*(short *)((char *)*(int *)&g_pFieldState + 0x26)` in the VM,
  `p * 0x88 + 0x6C` for entities,
  `*(int *)((char *)ctx + 0x9a0)` for the engine context.
- Ten TUs keep VC5 stack-slot unions (`frame.<member>`): `field_engine_model_anim.cpp`
  (`Field_UpdateModelInstances`), `field_execute_model.cpp`
  (`Field_ExecuteModelCommand`, 4 KB), `field_indexed_record_command.cpp`,
  `field_model_vertex_lighting.cpp`, `field_run_polygon.cpp` and others. These are the
  least clear functions.
- Many comments in the model-effect TUs (`field_model_vertex_lighting.cpp`,
  `field_execute_model.cpp`, `field_math_psx_helpers.cpp`) and the encounter TU are one
  line from the function name. Treat them as names, not descriptions.
- The `goto`s of this folder are in `field_message_window.cpp` (the window
  state machine) and `Script_ExecuteFrame` (`field_map_script_state.cpp`), each
  with a comment on why.

## Data-only files

Data runs that their link neighbour never references, each in a file of its own:

| file | span | next to | what it holds |
|---|---|---|---|
| `src/condor_display_state.cpp` | .bss 0x00cbf108..0x00cc0d88 | `field_encounter_random.cpp (before it)` | Fort Condor's HUD readout, unit model, DirectDraw and display-format state |
| `src/field_encounter_state.cpp` | .bss 0x00cc1658..0x00cc1670 | `field_map_script_state.cpp (before it)` | the random-encounter trigger, step counter, rate accumulator and random indexes |
| `src/field_file_load_state.cpp` | .bss 0x00cff5b0..0x00cff5b8 | `ad_cdr.cpp` | the file load request flag and parameter |
| `src/field_head_trig_table.cpp` | .data 0x00908e30..0x00909230 | `field_head_mesh_commands.cpp` | the head-mesh trigonometry table |
