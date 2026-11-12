# Polygon (lib): models, skeletons, animations and TIM textures

lib/polygon loads the 3D data every 3D module draws: `.RSD` model descriptions with their `.P`
geometry and `.TEX`/`.TIM` textures, `.HRC` skeletons, and binary animations. It also poses a
skeleton for one animation frame and submits its polygon sets to [graphics](../graphics/README.md),
and it runs a self-timed animation player. It is a library, not a game module: nothing enters or
leaves it. A module fills a `PfileLoadOptions` block, loads what it needs on module entry, draws a
frame per game frame, and frees the models when it leaves.

## Where the code is

`src/lib/src/polygon/`: 6 TUs, 165 functions. Five keep their retail names from `__FILE__`
strings. The sixth object (0x00699000-0x00699470) has no known name; its code is in `tim.cpp`. TU
boundaries follow retail's objects.

| TU | span | functions | holds |
|---|---|---:|---|
| `rsd.cpp` | 0x00674530-0x006750e0 | 25 | load options, RSD model handles, model sets |
| `anm.cpp` | 0x006833d0-0x00685110 | 18 | bone matrices, `Anim_DrawSkeletonFrame`, matrix baking, per-model vertex colours |
| `plytopd.cpp` | 0x00690240-0x00696d00 | 87 | PLY/MAT and RSD parsers, `.HRC` and animation loaders, animation player, animation groups |
| `tim.cpp` | 0x00696d00-0x00699000 | 11 | TIM loader, TIM to texture conversion, texture sets |
| `tim.cpp`, continued | 0x00699000-0x00699470 | 2 | TIM tiling into a texture set, TIM writer |
| `polygon.cpp` | 0x00699470-0x0069afc0 | 22 | the `.P` record: create, edit lists, load, write, weld, scale |

- `rsd.cpp` sits apart from the other five, at 0x00674530. It is identified as `rsd.cpp` from the
  `rsd.cpp` string at 0x0090cf40.
- Most calls into `rsd.cpp` come from ff7/battle and most calls into `anm.cpp` from ff7/snobo, but
  both TUs serve every 3D module: they are library code.
- `Gfx_ApplyBlendRecordToHundred` and `Gfx_GetBlendModeRecordCall`
  (`src/lib/src/graphics/graphics_apply_blend_record.cpp`) are called only by
  `Pfile_SetHundredBlendMode`. See the [graphics README](../graphics/README.md).
- `Token_StripHashComments` (0x006902c7) lives in `plytopd.cpp`, the retail object, although its name
  says token.

## Entry points

| function | address | what it does | called from |
|---|---|---|---|
| `Pfile_InitLoadOptions` | 0x0067455e | clears a 0x74-byte `PfileLoadOptions`, sets default model flags, the source mode, blend mode 4, scale 1.0 | battle, chocobo, condor, field, highway, wm, graphics (31 calls) |
| `Pfile_InitLoadOptionsWithBlend` | 0x006745e6 | clears the block, sets the source mode, +0x14 and +0x1C to 1, the caller's blend mode, scale 1.0 | every 3D module (85 calls) |
| `Pfile_SetLoadOptionsBlendMode` | 0x00674659 | writes `blendMode` (+0x20) | every 3D module (96 calls) |
| `Pfile_SetAssetSourceMode` | 0x0067453a | sets `g_PfileAssetSourceMode` (initial value 1) | 45 calls: `main.cpp` (16, mostly `GameModeDispatcher_Frame`), `credfile.cpp`, `battle.cpp`, `amptoanm.cpp`, the menu texture loaders (`callback.cpp`, `menu_render_objects_portraits.cpp`), `main_game_over_screen.cpp`, `main_cd_check_screen.cpp`, `ad_app.cpp`, `highway_minigame_frame_loop.cpp`, `sub_module_lifecycle.cpp`, `wmfile.cpp` |
| `LoadRsdModel` | 0x00674a0a | `Pfile_LoadRsdModelEx(0, ...)` | `Skel_BuildHierarchyAndLoadParts`, `Pfile_LoadModelSet`, `Gfx_LoadModelTableFromRecords`, `Psx_LoadModelTableFromRecords`, `HighwayRoster::loadModels` (highway) |
| `Pfile_LoadRsdModelEx` | 0x00674701 | parses an `.RSD`, loads its textures and geometry, builds a polygon set; returns a 0xC-byte handle | `LoadRsdModel`, `BattleModel_LoadPartMesh` |
| `Pfile_FreeRsdModel` | 0x0067466d | frees the handle's material, polygon set and RSD record | `Field_FreeModelRecord`, `Battle3d_DestroySceneObject`, `BattleStage_FreePartMesh`, `App_FreeInputStateAndRsdModel`, chocobo, condor, highway |
| `Pfile_LoadModelSet` | 0x00674c3e | loads every `*.RSD` of a directory listing, or a given name list, into a model set | `Mini_CondorLoadUnitModels` |
| `Skel_LoadHrc` | 0x00692ee1 | parses an `.HRC` into a `PolygonSkelHierarchy` | `Field_LoadSkeleton`, `World_LoadLeaderModel`, `World_LoadStageModels`, `Mini_ChocoboLoadHrcAndAttach`, `HighwaySkelItem::loadHrc` (highway) |
| `Skel_BuildHierarchyAndLoadParts` | 0x00693902 | builds the part list and loads `<name>.RSD` for each part | `BattleModel_BuildSkeleton`, `BattleModel_BuildSkeletonFromLmd`, `Field_LoadSkeleton`, `World_LoadLeaderModel`, `World_LoadStageModels`, `Mini_LoadHrcFromDiskAndAttach`, `Mini_ChocoboLoadHrcAndAttach`, highway |
| `Anim_CompileTextToBinary` | 0x0069254c | in source mode, compiles a text animation and writes it with `Anim_WriteBinary`; otherwise returns 1 | `World_LoadLeaderModel`, `World_LoadStageModels`, `Field_LoadAnimation`, `Mini_ChocoboCompileAndLoadAnimation`, `Mini_CondorLoadAnimation`, highway, `AnimPlayer_SelectAnimation` |
| `Anim_LoadBinary` | 0x006921ce | reads the 0x24-byte header (version must be 1) and the frame data | `World_LoadStageModels`, `World_LoadLeaderModel`, `Field_LoadAnimation`, `Mini_ChocoboCompileAndLoadAnimation`, highway, `AnimPlayer_SelectAnimation`, `Anim_LoadBinaryFromDisk` |
| `Anim_DrawSkeletonFrame` | 0x006840da | poses one frame on the matrix stack at context +0x2F4 and draws each part's polygon sets | `Field_RenderModels`, `Field_ExecuteModelCommand`, `BattleModel_DrawAnimFrame` (and its sub-skeleton variant), `Mini_ChocoboDrawFrame`, `World_DrawStageModels`, highway, `Gfx_BuildOffscreenRenderObject`, `AnimPlayer_Tick` |
| `Skel_LoadHrcAndCreatePlayer` | 0x006968c4 | `Skel_LoadHrcAndBuild`, `AnimPlayer_Create`, `AnimPlayer_SelectAnimation` | `Field_LoadEntitySkeletonIfAbsent`, `Mini_LoadEntitySkeletonIfAbsent`, `Mini_ChocoboLoadEntitySkeletonIfAbsent` |
| `AnimPlayer_Create` | 0x00695a2b | allocates a 0xE4-byte player and its first deadline | `Field_CreateAnimPlayerWithAnimation`, `Mini_ChocoboCreateAnimPlayerAndBind`, `Mini_CondorCreateAnimPlayerWithAnimation`, `HighwaySkelItem::makePlayer` (highway), `Skel_LoadHrcAndCreatePlayer` |
| `Pfile_LoadBinary` | 0x0069a028 | reads a `.P` file | `BattleModel_BuildSkeleton`, `BattleStage_LoadModelSet`, `Gfx_CreatePrimitiveMeshBank`, `Pfile_LoadRsdContents` |
| `Pfile_CreatePolygonData` | 0x006997c9 | allocates an empty polygon record, with edit lists on request | `Gfx_BuildPrimitiveMesh` (16 calls), `Battle_BuildPolygonDataFromLmd` |
| `Pfile_LoadTim` | 0x00697558 | loads a TIM through a stack file context | `BattleModel_LoadEnemyDatLegacy`, `BattleModel_LoadPartyDatLegacy`, `BattleStage_LoadStageDatLegacy` |
| `Gfx_ForEachModelPolygonSet` | 0x00685028 | calls a callback with each polygon set of each skeleton part | `Field_ExecuteModelCommand` (14 calls) and three more field functions, battle, `Gfx_AndModelPolygonSetRenderFlags` / `Gfx_OrModelPolygonSetRenderFlags` |
| `Anim_SetTransformFlags` | 0x006833de | sets `g_AnmTransformFlags`, which `Anim_DrawSkeletonFrame` tests | `Battle_ModuleEnter`, `Field_ModuleEnter`, `Mini_ChocoboEnter`, `Mini_CondorEnter`, `WorldMap_ModuleEnter`, the field, chocobo and condor engine setup, highway |

31 functions are not called or referenced by name anywhere, among them the whole `AnimGroup_*`
family (`AnimGroup_LoadFromTextFile`, `AnimPlayer_TickGroup` and the per-player getters and setters),
`Pfile_BuildTextureTileSetFromTim` and `Pfile_WriteTimRecordToFile`.

## Main structures

| type or global | header | what it holds |
|---|---|---|
| `PfileLoadOptions` | `include/lib/pfile.h` | 0x74-byte load options: `flags`, `drawEnabled`, `sourceMode`, `createMatrices`, `polygonFlags`/`colorKeyEnabled`, `matrixCount`, `drawPool`, `blendFlags`, `blendMode`, `directory`, `texture`, `emitterFlags`, `lightGroup`, `lightFlags`, `scale`, `loadBoneParts`, `file` (`FileOpenRequest`), `extraFlags`, `colorKey`, `paletteFlags`, `paletteRow`, and `renderFlags` |
| `PolygonRsdRecord` | `include/lib/types/polygon.h` | parsed `.RSD` (0x24 bytes): PLY name +0x00, MAT name +0x04, a third name +0x08, texture count, texture names, TIMs, textures, the mesh pointer |
| `PolygonFileArrays` | `include/lib/types/polygon.h` | the `.P` record: 0x80 bytes read whole from the file; version at +0x00, section counts from +0x0C, array pointers at +0x40..+0x74 filled after the read |
| `PolygonSkelHierarchy`, `PolygonSkelBone` | `include/lib/types/polygon.h` | `.HRC` skeleton; 0x1C-byte bone: name +0x00, parent name +0x04, parent index +0x08, length +0x0C, RSD count and names +0x10/+0x14 |
| `PolygonAnimation` | `include/lib/types/polygon.h` | animation: 0x24-byte header, frame data, 8-byte table per frame (root pose, rotations) |
| `PolygonSkeletonAnim`, `PolygonAnimBake` | `include/lib/types/polygon.h` | other views of the same animation record, used by `Anim_DrawSkeletonFrame` and the bake functions |
| `PolygonAnimPlayer`, `AnimPlayerInstance` | `include/lib/types/polygon.h` | two views of the 0xE4-byte player: frame range, step, loop flags, TSC tick and deadline at +0xBC/+0xC4, placement at +0x38 |
| `PolygonAnimPlacement` | `include/lib/types/polygon.h` | placement at player +0x38; angles, scale and translation by its accessors' names |
| `PolygonTim`, `PolygonTimBlock` | `include/lib/types/polygon.h` | a loaded TIM: its header, CLUT and image blocks, the sizes and the converter's options |
| `g_PfileAssetSourceMode` | `rsd.cpp` | 0: load `.TEX`, `.P` and binary animations; nonzero: build them from TIM, PLY/MAT and text first |
| `g_AnimTextKeywords`, `g_HrcRootName` | `plytopd.cpp` | the `:HEADER_BLOCK` .. `:RZ` keywords of HRC and animation text, and `"root"` |
| `g_AnmTransformFlags` | `anm.cpp` | bits `Anim_DrawSkeletonFrame` tests (0x400, 0x4000, 0x8000); `Anim_GetDefaultTransformFlags` gives 0x807B |

`PolygonSkeletonLoadSpec` (`include/lib/types/polygon.h`) is the skeleton-and-animation load descriptor that `AnimPlayer_Create` and `Skel_LoadHrcAndCreatePlayer` take.

## Data flow

1. Options. The caller fills `PfileLoadOptions` with `Pfile_InitLoadOptions` or
   `Pfile_InitLoadOptionsWithBlend`; both copy the global source mode into +0x08.
2. Model. `Pfile_LoadRsdModelEx` calls `Pfile_ParseRsdFile`, which tokenises the `.RSD` with
   `TokenizeTextFile` and `Token_StripHashComments`, skips the first token and takes the text after
   `=` of the next ones. `Pfile_LoadRsdContents` then loads each texture with
   `Pfile_LoadRsdTexture` and the geometry:
   - source mode 0: `Gfx_LoadTexFileCached` on `<name>.TEX` (the name as given when options flag 0x200 is set), and `Pfile_LoadBinary` on `<PLY name>.P`;
   - source mode nonzero: `Pfile_LoadTim`, `Pfile_BuildTextureFromTim`, `Gfx_WriteTexFile` to make the
     `.TEX`, and `Pfile_BuildFromPlyMat`, which parses the PLY and MAT text and writes the `.P` with
     `Pfile_WriteBinary`. The `.TEX` it wrote is then loaded with `Gfx_LoadTexFileCached`.
   `Pfile_ScaleVertices` applies the scale at options +0x38. Back in `Pfile_LoadRsdModelEx`, blend
   modes and textures go onto the hundred records (`Pfile_ApplyBlendModeToHundreds`,
   `Pfile_BindTexturesToHundreds`), `GfxLoadPolygonSet` builds the polygon set and
   `D3DAppendPolygonSetToList` registers it with the graphics context.
3. Skeleton. `Skel_LoadHrc` reads the bone count from the header block, then per bone its name,
   parent name, length (stored negated) and RSD names; a parent named `"root"` gives parent index -1.
   `Skel_BuildHierarchyAndLoadParts` loads `<name>.RSD` for each part with `LoadRsdModel` when
   options +0x3C is set.
4. Animation. `Anim_LoadBinary` reads the 0x24-byte header, rejects a version other than 1, reads
   `Anim_GetFrameDataSize` bytes and builds the per-frame table. `Anim_BakeAllFrameMatrices`
   can precompute 0x40-byte matrices for every frame.
5. Per frame. `Anim_DrawSkeletonFrame` pushes the context's matrix stack (`MatrixStack_Push`,
   `MatrixStack_Top`), builds the root matrix (`Anim_BuildRootMatrix` or the `Biased` variant),
   applies the placement's scale and angles, walks the part list, builds each bone matrix, and for
   each polygon set calls `Gfx_SetMatrixCall`, the lighting calls and `Gfx_DrawPolygonSetCall`.
6. Player. `AnimPlayer_Tick` compares `ReadTimestampCounter` with the deadline (`Compare64`), steps
   the frame, and at the end loops, holds the last frame, calls the callback at +0xD0 or hides the
   parts (`Skel_SetPartsVisible(0, ...)`); then it draws with `Anim_DrawSkeletonFrame`. Its only
   caller is `AnimPlayer_TickGroup`, which nothing calls; the modules call `Anim_DrawSkeletonFrame`
   themselves.
7. Free. `Pfile_FreeRsdModel`, `Pfile_FreeModelSet`, `Skel_Free`, `Anim_Free`, `Pfile_DestroyTim`.

Memory comes from [lib/mem](../README.md#mem) (`GameHeapAlloc` and friends), files from
[lib/file](../file/README.md) (`OpenFileEx`, `ReadFileIntoNewBuffer`, `PushFileContextDirectory`),
text parsing from [lib/token](../README.md#token), lists from [lib/list](../README.md#list), and the
matrix stack from [lib/stack](../README.md#stack).

## Notes

- `PolygonAnimation` +0x04 is the frame count and +0x08 the bone count. `AnimPlayer_SelectAnimation` does not read an animation but a `PolygonAnimSequence`: text
  files, one per play-time step, compiled and loaded as the play time reaches them.
- LIBCMT's `atoi` is called
  32 times in `plytopd.cpp` to turn tokens into integers.
- `Anim_DrawSkeletonFrame` keeps its locals as a 560-byte named stack-slot union (`frame`).
- Large and unclear: `Pfile_BuildTextureFromTimRect` (0x00697652), `Pfile_ParsePlyAndMat`
  (0x0069087e), `Pfile_BuildGroupsFromFaces` (0x00695047), `Anim_CompileTextToBinary`
  (0x0069254c), `Pfile_WeldDuplicateVertices` (0x0069a6cd).
