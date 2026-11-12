# World map: the overworld, underwater and snowfield maps

The world map module runs whenever the party is outside a field: walking, riding vehicles,
random encounters, and the underwater and snowfield variants. The game-mode dispatcher enters it
for `GAME_MODE_WORLD_MAP` by installing the five `WorldMap_Module*` callbacks
(`GameModeDispatcher_Frame`, [../main/](../main/README.md)). It leaves to a field, a battle or the
menu by filling a module record and calling `RequestModuleSwitch` from `WorldMap_ModuleFrame`.
The menu hands control back by installing the same callbacks again (`Menu_ModuleFrame`,
`src/ff7/src/menu/english/menu_main_and_battle_results.cpp`).

## Where the code is

| Folder | TUs | Functions |
|---|---|---|
| `src/ff7/src/wm/` | 16 | 496 |

- Four TUs, `wm_world_module_render.cpp`, `wm_world_camera_accessors.cpp`, `wm_world_terrain_streaming.cpp` and
  `wm_world_zones_sky.cpp`, sit at `0x0074ba80..0x007592e0`, directly before `wmfile.cpp`; the
  whole world map is one run of retail objects from `0x0074ba80` to `0x0076b850`. They hold the module callbacks, the main loop, the camera, terrain
  streaming, sky, zones, fades and the Zolom.
- `creative_sfman_error_messages.cpp` (`0x0074b8c0`, in `lib/sound/creative/`) is sound code: it holds
  `SfManErrorText`, which `sfutils.cpp` and `sound_soundfont_open_close.cpp` call, `SfManFormatUnknownError`,
  which only `SfManErrorText` calls, and `SfManIsAbsBelow100`, which nothing calls by name.
- Declarations: `include/ff7/api/wm.h` for all sixteen TUs. Types: `include/ff7/types/wm.h` and `include/ff7/world.h` (`WorldActor`, `WorldScriptCallFrame`, `WorldStreamFocusData`).
- TU boundaries follow retail's objects; files without a retail `__FILE__` name are named after what they contain.

## Entry points

| Function | Address | What it does | Called from |
|---|---|---|---|
| `WorldMap_ModuleEnter` | 0x0074baf5 | Unless returning from the menu (`g_WorldMapInMenu`), sets viewport and UI scale from `GetUiScalingMode`, calls `World_AllocArena`, clears model handles; always sets the 30 and 22 fps frame intervals | `GameModeDispatcher_Frame`, `Menu_ModuleFrame` (by address) |
| `WorldMap_ModuleFrame` | 0x0074be49 | Frame pacing, input, `SoundUpdateFades`, scene begin, `WorldMap_MainUpdate`, the terrain flush pointer, scene end, then the module switch | same |
| `WorldMap_ModuleExit` | 0x0074bd77 | Unless going to the menu, frees textures, models, arena and lights and sets the next game mode | same |
| `WorldMap_ModuleMouse`, `WorldMap_ModuleKeyboard` | 0x0074ba80, 0x0074ba85 | Empty | same |
| `World_ReleaseResourcesIfSuspended` | 0x0074bd50 | Frees the world resources if the world is suspended under the menu | `App_Shutdown` |
| `WorldMap_MainUpdate` | 0x0074db8c | The main loop: first-call initialisation, the per-frame update, departure | `WorldMap_ModuleFrame` |
| `WorldScript_RequestSystem` | 0x007640bc | Requests a system script function by number (0 at start, 1 at departure, 2 and 6 to 9 from other world code) | `WorldMap_MainUpdate`, `World_UpdatePlayerControl` (6), `WorldScript_ExecControlFlow` (2), `World_UpdateZolom` (7), `World_UpdateTerrainEffect` (8), `World_ArmVehicleExit` (9) |
| `WorldMap_RequestBattle` | 0x0074d7cd | Starts the exit sequence toward a battle | `World_UpdateEncounters`, `WorldScript_ExecCommand` |
| `WorldMap_RequestFieldTransition` | 0x0074d74c | Starts the exit sequence toward a field entry | `WorldScript_ExecCommand` |
| `WorldEntity_GetPlayerPosition` | 0x00762798 | Copies the player actor's position | 24 call sites across the terrain-streaming, zone, entity and camera TUs |

## Main structures

| Type or global | Header | What it holds |
|---|---|---|
| `WorldActor` | `include/ff7/world.h` | An actor: list links, `pos`/`altPos`, facing, `scriptIdx`, `actorType` (model id), `flags1`. Member names follow the PlayStation version's naming (unconfirmed). `WorldEntity_InitPool` threads 16 records of 0xC0 bytes at `g_WorldActorPool` |
| `WorldPosition`, `WorldPoint` | `include/ff7/types/wm.h` | World and actor coordinate records |
| `WorldModelRecord` | `include/ff7/types/wm.h` | A 0xB8-byte loaded model (skeleton, animations, matrix); returned by `World_GetModelRecord` |
| `WorldShadowQuad`, `WorldEffectSprite` | `include/ff7/types/wm.h` | Shadow and effect quads (`World_DrawShadowQuad`, `World_DrawEffectNodeQuad`, `World_SetShadowQuadTextureRect`) |
| `GfxTlVertex` | `include/lib/types/graphics.h` | Mini-map quad vertices (`World_DrawMiniMapBackdrop*`, `World_DrawMiniMapMarker`) |
| `WorldBlockLoadRequest` | `include/ff7/types/wm.h` | Terrain streaming: block load queue node |
| `WorldMeshSlot`, `WorldMeshRecord` | `include/ff7/types/wm.h` | Terrain streaming: decompressed meshes |
| `t_wm_SkyPoly` | `include/ff7/types/wm.h` | Sky gradient polygon |
| `WorldSkySprite`, `WorldSkyVertexQuad` | `include/ff7/types/wm.h` | Sky layers |
| `WorldTerrainTriangle`, `WorldTerrainChunkInfo`, `WorldGroundProbe` | `include/ff7/types/wm.h` | Terrain triangles and ground probes |
| `gWorldArena` | `include/ff7/api/wm.h` | 0xE7801 bytes from `World_AllocArena`, cut into eight `g_pWorldArenaRegion*` pointers |
| `g_WorldMode` | `wm_world_camera_accessors.cpp` | The map state, `enum WorldMapState` (`include/ff7/enums/wm.h`): `WORLD_MAP_STATE_LOADED` (-15) after loading, counting up; 0 the camera intro, 1 playing, 2 fading out to the menu or a field, 3 leaving, 4/5 the submarine diving/surfacing, 6 and above departing (`World_UpdateMapStateMachine`) |
| `g_WorldMapType` | same | The map variant; `World_GetMapVariant` returns it |
| `g_WorldFieldTransitionState`, `g_WorldFieldTransitionArg`, `g_WorldFieldTransitionEntryWord` | `include/ff7/api/wm.h` | The pending exit, exported by `World_ExportTransitionRequestAndSaveState` |
| `g_World*RenderObject` (0x00e2c3d8..: radar, mini-map, terrain, map, sky, cloud, meteor), `g_WorldTexturePageRenderObjects` | `include/ff7/api/wm.h` | Render objects created by `World_LoadStageTextures` and emitted by the flush functions |

## Data flow

1. Load (`WorldMap_MainUpdate`, first call). It picks the map variant from the saved
   variant or the entry field. The stage is 9 or 10 for entry fields 36..39, variant + 9 for a
   non-zero variant, otherwise `World_SelectMapVariantFromStoryProgress` (the story progress word
   `gSavemap.storyProgressLo`/`Hi`, savemap +0xBA4). `WorldMap_LoadStageResources` then calls `World_LoadStageAssets`
   (textures, light groups, models through `Skel_*` in lib/polygon) and loads from archive 5
   (`wm/world_us.lgp`, `BuildArchivePath`) with `WorldMap_LoadLgpAsset`: `wm.ta`, `WM<n>.EV`
   (the event script, 0x7000 bytes), `mes`, `field.tbl`, `atn.tbl`, `enc_w.bin`.
2. Terrain comes from loose files built by `World_BuildLoosePath`: `World_PreloadBlocksFromBot`
   reads 0x2E000 bytes of `wm/WM<n>.BOT`; `World_ReadBlockFromMapFile` reads 0xB800-byte blocks of
   `wm/WM<n>.MAP` on a 9 x 7 block grid (`World_BlockIndexFromGrid`). `World_DecompressMeshIntoSlot`
   LZS-decompresses meshes (`World_LzsDecompress`). Positions wrap at 0x48000 x 0x38000
   (`World_WrapPositionToMapBounds`).
3. Savemap: `World_RestoreStateFromSavemap` checks the sum of `gSavemap.scriptBanks` bytes 0x380..0x3FF,
   restores camera yaw and mode, mini-map state and party slots, seeds the random table from
   `gSavemapSeconds`, and restores terrain currents and the vehicle slots (`WorldEntity_InitPool` on `gSavemap.lWorld`).
   `World_SaveStateToSavemap` writes them back and fixes the checksum byte. World script
   variable bank 0 starts at `gSavemap.storyProgressLo`, the first script bank (`WorldScript_Init`).
4. Per frame (`WorldMap_MainUpdate`, state below 6, not under the menu): projection and render
   buffer flip, terrain texture animation, `World_BeginEntityFrame`, camera height smoothing,
   `World_UpdatePlayerControl`, `World_AdvanceScriptedCamera`, `WorldZone_ApplyToCamera`,
   `WorldScript_RunAllEntities`, `World_BuildCameraMatrix`, `World_UpdateTerrainStreaming`,
   `World_UpdateMapStateMachine`, message window input, sky, fades and overlays, `World_DrawMiniMap`,
   `World_UpdateVehicleTransition`, actor models and shadows, effect nodes, `World_UpdateZolom`,
   and `World_UpdateEncounters` in state 1 only.
5. Script VM: `WorldScript_ExecEntity` reads 16-bit opcodes and dispatches below 0x100 to
   `WorldScript_ExecOperator`, below 0x200 to `WorldScript_PushOperand`, below 0x300 to
   `WorldScript_ExecControlFlow`, the rest to `WorldScript_ExecCommand`.
6. Graphics: `World_LoadTerrainTextures` stores one of `World_FlushTerrainBatches`,
   `World_FlushTerrainBatchesUnderwater` or `World_FlushTerrainBatchesSnow` in
   `g_WorldTerrainFlushFn`; `WorldMap_ModuleFrame` calls it after the update. The flush
   functions emit the render objects with `Gfx_EmitPrimitiveBatchList`, draw models with
   `World_DrawStageModels` (`Anim_DrawSkeletonFrame`) and draw the first record of `g_FieldWindows`
   with the menu's `Menu_RenderMessageWindows`.
7. Sound: `World_SwitchMusicTrack` sends `SoundDispatchCommand` 0x18, 0x10 or 0x14;
   `World_RequestAmbienceSfx` uses `SfxPlayUpToFour`; `World_StopAllSfxAndResetTranspose` and
   `World_SetMidiVolume` wrap lib/sound.
8. Frame pacing: `World_WaitForFrameInterval` busy-waits on the timestamp counter. When the last
   frame ran past the 22 fps interval, `WorldMap_ModuleFrame` runs `WorldMap_MainUpdate` twice and
   renders terrain only in the second pass.
9. Menu: input bit 0x10 in `World_UpdatePlayerControl` enters state 2 for player models in mask
   0x47 (model 3 requests system script 6 instead). `World_UpdateMapStateMachine`
   saves to the savemap and sets `g_WorldMapInMenu`; `WorldMap_ModuleFrame` then switches to the
   `Menu_Module*` callbacks. On return the same state restores control and clears the flag.
10. Leave: at state 6 or above `WorldMap_MainUpdate` runs system script 1 to completion, stops
    streaming and calls `World_ExportTransitionRequestAndSaveState` into `g_FieldReturnMode`,
    `g_FieldNextModuleArg` and `g_WorldBattleRequest` (the battle request). States 6 to 8 clear the initialised
    flag, so the next call loads again (state 6 selects variant 2). State 9 sets
    `g_WorldMapExitRequested`; `WorldMap_ModuleFrame` switches to the `GameModeDispatcher_*` callbacks
    and `WorldMap_ModuleExit` sets game mode 1 (field), 0x17 (battle swirl) or field request 10 by
    `g_FieldReturnMode`.

## Named codes

| Enum | Header | Used by |
|---|---|---|
| `WorldScriptOp` | `include/ff7/enums/wm.h` | World script opcodes; `WorldScript_ExecOperator`, `WorldScript_ExecControlFlow`, `WorldScript_ExecCommand` (`wm_world_entity_pool.cpp`) |
| `WorldStage` | `include/ff7/enums/wm.h` | Stage ids 0..12 (overworld sets, warship, underwater 11, snowfield 12); `World_LoadStageModels` (`wmfile.cpp`) |
| `RENDER_STATE_*`, `BLEND_*` | `include/lib/enums/graphics.h` | Render state and blend mode in the flush functions and texture loaders |

## Notes


- Key world-map state globals: `World_SetStageVariantFlag` / `World_GetStageVariantFlag` access
  `g_WorldLeaderModelIndex`, `World_PartyHasMultipleMembers` counts party members, and the actor
  free-list head is `g_pWorldActorFreeList`
  (`WorldEntity_InitPool`, `WorldEntity_Alloc`), the init-step mask `g_WorldInitStepsDone` (`World_TryEnterInitStep`), and the music state `g_WorldMusicChangeGate`, `g_WorldMusicTrack`, and `g_WorldPrevMusicTrack`.
- `wm_world_module_render.cpp` reads the model records directly from `g_WorldLeaderModels` /
  `g_WorldStageModels` (`WorldModelRecord`, 0xB8 bytes, `active` at +0x24);
  `include/ff7/types/wm.h` defines another `WorldModelRecord` (`animationCount` at +4).
- Named VC5 stack-slot unions (`frame` / `s` / `f`) remain where `/Od` stack layout requires them. Gotos remain in
  `World_SubmitTerrainChunkTriangles` and `World_TriangleHeightAt` (`wm_world_terrain_geometry.cpp`),
  `WorldMsg_StepWindowTextReveal` and `WorldMsg_DecodeMessageToTextBuffer` (`wm_world_message_helpers.cpp`),
  and twice in `wm_world_terrain_streaming.cpp`.
- Large and only partly explained: `WorldScript_ExecCommand` (about 450 lines),
  `World_SubmitTerrainChunkTriangles`, `World_WalkTerrainAndPlaceEntities`,
  `WorldMsg_StepWindowTextReveal`, `World_UpdatePlayerControl`, `WorldEntity_SpawnTerrainEffects`.
- `World_RoundFloatToInt` is inline assembly (`fistp`).

## Data-only files

Data runs that their link neighbour never references, each in a file of its own:

| file | span | next to | what it holds |
|---|---|---|---|
| `wm_terrain_state.cpp` | .bss 0x00e36110..0x00e38938 | `wm_world_message_window.cpp` | the selected and player terrain triangles, terrain and field tables, message buffer, arctangent table, mesh vertices |
