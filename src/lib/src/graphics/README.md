# Graphics: the window, the engine context and the renderer every module draws with

lib/graphics is the engine library under all game modules. It owns the Windows message loop, the
engine context, the graphics driver (Direct3D, the DirectDraw software rasterizer, OpenGL or a
driver DLL), textures, render objects, primitive batches and a PlayStation GTE stand-in for the
fixed-point math the game code still uses. The process starts in `WinMain` (0x0067db30): it calls
`App_Bootstrap` (ff7/main), which creates the engine context and installs its callbacks (`WinMain`
creates one itself only if none is set), and `CreateGameWindow` calls the context's init
callback (`App_InitSubsystems`, which calls `InitGraphicsDevice`) and its enter callback. Each loop
iteration then services a pending module switch (`ServiceModuleSwitch`), runs the active module's
frame callback and presents the frame (`PresentFrame`). Game modules come and go through
`RequestModuleSwitch`. The loop ends when the context's first word is set (`WM_CLOSE` ->
`RequestEngineQuit`); `WinMain` then calls the exit and shutdown callbacks, releases the texture
cache and destroys the window and the context. What the modules do with this is in
[ff7/main](../../../ff7/src/main/README.md).

## Where the code is

`src/lib/src/graphics/`: 63 source files, 1,205 functions.
29 files keep their retail names from `__FILE__` strings (`C:\lib\src\graphics\*.cpp`, including
`sw\sw.cpp`, `sw\z.cpp`, `sw\sw_vert.cpp`, `gl\gl.cpp` and `gl\gl_code.cpp`). The other 35 are named
after what they contain, and their place in the link order is inferred from their neighbours.
`texcache.cpp` (retail `TexCache.cpp`) also emits the `std::map`/`std::list` code the texture cache
instantiates, 19 COMDATs at 0x0069da10..0x0069f910 in retail's order; it needs the VS97 SP3 C++ front
end. TU boundaries follow retail's objects.

The code sits at 0x00660540..0x006ce7d0, interleaved with lib/mem, lib/polygon, lib/file, lib/list,
lib/thread, lib/trans, lib/stack, lib/sort and lib/token objects.

| area | files | what is in it |
|---|---|---|
| shell and context | `graphics_app_entry_point.cpp`, `directx.cpp`, `graphics_driver_context_fields.cpp` | `WinMain`, window and message procedure, engine context, DirectDraw/Direct3D setup and shutdown, driver install, module switch, per-frame pool resets |
| driver interface | `driver.cpp`, `g_drv.cpp`, `graphics_bind_driver_d3d.cpp`, `graphics_bind_driver_ddraw.cpp`, `graphics_gl_driver_ops_bind.cpp` | the 0xF0-byte driver ops table, its three binders and the `*Call` wrappers the game calls |
| Direct3D back end | `dx_rend.cpp`, `dx_rend5.cpp`, `dx_rendi.cpp`, `dx_rendx.cpp`, `dx_mesh.cpp`, `dx_view.cpp`, `dx_mat.cpp`, `dx_pal.cpp`, `dx_graph.cpp` | execute buffers (`D3DExec*`), Direct3D 5 batches (`D3D5*`), render states and presets, scenes, viewports, materials and matrix handles, palettes, pixel formats, texture surfaces |
| texture cache | `texcache.cpp` (its template markers in `include/lib/texcache_templates.h`) | surfaces keyed by texture header and palette, reused and evicted |
| render objects and batches | `dx_sfx.cpp`, `dx_spr.cpp`, `instance.cpp`, `dx_3d2d.cpp`, `graphics_matrix_transform_asm.cpp` (flag helpers), `matrix_lib.cpp` (the assembly 4x4 matrix routines), `graphics_group_vertex_draw_spec.cpp`, `shp.cpp` | `Gfx_CreateRenderObject`, primitive emitters, batch nodes, draw pool, depth-sorted queue, 4x4 matrix math, draw specs, `.shp`/`.spt` files |
| lighting and vertex colour | `light.cpp`, `graphics_vertex_color_arithmetic.cpp` | light records, `Gfx_LightPolygonSet`, vertex colour scaling and blending |
| PSX layer | `psx.cpp`, `psxgraph.cpp`, `render.cpp` | GTE state, fixed-point matrices and trigonometry, PSX-style primitive emitters, model tables |
| software rasterizer | `sw/sw.cpp`, `sw/z.cpp`, `sw/sw_vert.cpp`, `sw/sw_gfx_raster_*.cpp`, `sw/sw_debug_line_formatting.cpp`, `graphics_raster_triangle_dispatch.cpp`, `graphics_surface_pixel_raster.cpp`, `graphics_raster_dda_span.cpp`, `graphics_raster_line_clip.cpp`, `graphics_raster_line_pairs.cpp`, `sw/sw_raster_rect_clip_test.cpp`, `sw/sw_blit_emit_begin_draw.cpp`, `sw/sw_gfx_raster_asm_spans.cpp` | the `Blit_*` driver, triangle and line setup, span writers, clipping, Z buffer |
| OpenGL back end | `gl/gl.cpp`, `gl/gl_code.cpp`, `gl/gl_node_draw_emitters.cpp` | the `Gl_Ops*` driver and its `gl*`/`wgl*` wrappers |
| config, offscreen, colour rows | `graphics_app_config_loader.cpp`, `graphics_offscreen_render_slots.cpp`, `graphics_palette_row_fill.cpp`, `graphics_apply_blend_record.cpp`, `graphics_blend_record_lookup.cpp` | `Gfx_LoadAppConfigFile`, rendering a model into a texture, the colour-row helpers of the DLL export table, the blend-record helpers |
| debug and stats | `dx_dbg.cpp`, `dx_stat.cpp`, `graphics_capability_logging.cpp` | DirectDraw error strings, the log target, caps dumps, frame statistics |

Neighbouring files:

- Five TUs at 0x006bf4e0..0x006ce7d0, directly before the menu's `callback.cpp`, are menu code
  and live in [ff7/menu](../../../ff7/src/menu/README.md)
  (`menu_main_and_battle_results.cpp`, `menu_config_screen.cpp`, `menu_input_config.cpp`, `menu_render_objects_portraits.cpp`,
  `menu_quit_confirm_dialog.cpp`). They make all of this subsystem's calls into the game.
- Nine TUs next to the OpenGL objects hold software-rasterizer code, not OpenGL code. They call and are
  called by `sw/*`, `graphics_raster_triangle_dispatch.cpp` and each other; none calls `gl.cpp` or `gl_code.cpp`.
  Eight are in `sw/`: `sw_blit_emit_begin_draw.cpp` (the `Blit_Emit*` slots `GfxBindDriverOpsDDrawBlit`
  installs), `sw_gfx_raster_asm_spans.cpp`, `sw/raster_float.cpp` (its float conversion helpers) and the six `sw_gfx_raster_*.cpp` (the raster build TU is
  `sw_gfx_raster_gradient_basis.cpp`, beside the retail-named folder's own `sw_gfx_raster_edge_setup.cpp`). The ninth is
  `graphics_raster_dda_span.cpp`, beside `graphics_raster_line_clip.cpp` and `graphics_raster_line_pairs.cpp`. A tenth TU
  from that run, `graphics_blend_record_lookup.cpp`, holds `Gfx_FindBlendModeRecordD3D`, which only the Direct3D binder
  installs; it is in this folder's root.
- Four graphics TUs sit among the lib/file, lib/list, lib/mem and lib/token objects and are in
  this folder: `graphics_offscreen_render_slots.cpp` (`Gfx_*Offscreen*`), `graphics_palette_row_fill.cpp`
  (four colour-row functions in the DLL export table below), `graphics_apply_blend_record.cpp`
  (`Gfx_ApplyBlendRecordToHundred`, `Gfx_GetBlendModeRecordCall`) and `graphics_app_config_loader.cpp`
  (`Gfx_LoadAppConfigFile`).

`psx.cpp`, `psxgraph.cpp` and `render.cpp` keep their retail `C:\lib\src\graphics\` names, although ff7/battle is their main caller.

## Entry points

Call-site counts leave out calls from inside lib/graphics.

| function | address | what it does | called from |
|---|---|---|---|
| `WinMain` | 0x0067db30 | bootstrap, context, window, message loop, per-frame calls, shutdown | process entry |
| `GameWindowProc` | 0x00676619 | window messages to context callbacks: mouse (+0xA04), keys (+0xA08), `WM_ACTIVATEAPP` (+0xA28), `WM_DEVICECHANGE` (+0xA2C); `WM_SIZE` sets +0x20 when minimised and clears it otherwise; Ctrl+Q requests the quit dialog (+0xA54); `WM_CLOSE` quits | Windows, class registered by `CreateGameWindow` |
| `CreateGameWindow` | 0x0067692a | registers the class, creates the window, reads the desktop size and depth, calls init (+0x9F0) then enter (+0x9F8) | `WinMain` |
| `CreateEngineContext` | 0x00676e7e | allocates the context with its defaults | `App_Bootstrap`, `WinMain`, the field, chocobo and condor engine setup |
| `GetEngineContext` | 0x00676578 | returns the active context (`g_pGfxEngineContext`) | 182 call sites in 16 subsystems |
| `InitGraphicsDevice` | 0x0067806e | binds the driver ops, sets up DirectDraw and Direct3D, runs driver init, clears and presents once, `GfxInitRenderResources` | `App_InitSubsystems`; the field, chocobo and condor `*_ddraw.cpp` wrappers |
| `ShutdownGraphicsDevice` | 0x00679864 | frees render resources, releases D3D/DirectDraw objects, frees the ops, unloads a driver DLL | `App_Shutdown`; chocobo and condor wrappers |
| `RequestModuleSwitch` | 0x00666cf2 | stages a `GameModule` (0x1C bytes) in the context's `pendingModule` (+0xA0C) unless a switch is pending | 32 call sites in 7 subsystems |
| `ServiceModuleSwitch` | 0x00666c78 | old exit (+0x9FC), `CommitPendingModule`, `Gfx_TrimRenderPoolOverflow`, new enter (+0x9F8) | `WinMain` |
| `PresentFrame` | 0x0066059c | driver `present` when +0xA48 is set | `WinMain`, 7 subsystems |
| `Gfx_BeginSceneCall`, `Gfx_EndSceneCall` | 0x00660ec0, 0x00660eeb | driver `beginScene`/`endScene` | module frames in 7 subsystems |
| `Gfx_SetScenePassCall` | 0x00660e95 | driver `selectScenePass` | 78 call sites |
| `Gfx_SetRenderStateCall` | 0x00660c3a | driver `setRenderState` with a `RenderState` code | 96 call sites |
| `GfxClearAllCall`, `GfxSetViewportRect`, `GfxSetBackgroundColorCall` | 0x00660626, 0x0066067a, 0x0066075c | clear, viewport (also stores the rect and calls `SetPerspective`), background colour | module enters and frames |
| `ResetFrameDrawPool`, `ResetRenderPools`, `Gfx_ResetNodeList` | 0x00666da3, 0x00666dc0, 0x00666ddd | start-of-frame resets of the draw pool (+0x300), the render-object pool (+0x910) and the node list (+0x938) | module frames |
| `GfxFlushFrameDrawList` | 0x00666df4 | draws the node list at +0x938 | module frames |
| `Gfx_BeginSortedDrawFrame`, `Gfx_EndSortedDrawFrame` | 0x006656a4, 0x006656d1 | reset and flush the depth-sorted queue (+0x920) when +0x91C is set | `battle.cpp`, the battle swirl, the submarine module, the world-map module, lib/file |
| `Gfx_CreateRenderObject` | 0x006710ac | builds a render object of one `GfxDrawKind` from load options | 182 call sites in 11 subsystems |
| `DestroyRenderObject`, `DestroyRenderObjectAndClear` | 0x00670fd3, 0x00671082 | frees a render object; frees and zeroes a slot | 100 and 81 call sites |
| `AcquireBatchNode` | 0x0066e272 | reserves `count` primitives in a render object's batch for this frame | 257 call sites in 12 subsystems |
| `Gfx_EmitPrimitiveBatchList` | 0x0066e641 | draws a batch through its callbacks, or queues it for depth sort | 281 call sites in 11 subsystems |
| `Gfx_DrawRenderObjectCall` | 0x00660e6a | driver `drawRenderObject` | 21 call sites |
| `Gfx_EmitFlatRect` and the other `Gfx_Emit*Rect/Triangle/Sprite*` | 0x00671d2a.. | write one primitive's vertices into a render object (`DrawIndexedPrimitive` batches, not execute buffers) | `Gfx_EmitFlatRect`: menu, field, chocobo, condor, highway, submarine; `Gfx_EmitGouraudRect`: submarine; `Gfx_EmitTexturedRect`: chocobo, condor; `Gfx_EmitTexturedRectFlipped`: condor; `Gfx_EmitScreenSprite`: `dx_spr.cpp`; the other eight have no call in `src/` |
| `SinFixed`, `CosFixed`, `RotMatrixXYZ`, `SetRotMatrix`, `SetTransMatrix`, `RotTransPoint` | 0x00662538, 0x006624fd, 0x006628de, 0x00663673, 0x00663707, 0x00662ecc | GTE-style math on the installed matrix | 688, 423, 416, 1,326, 1,308, 254 call sites |
| `Gfx_GetPsxScratchpadPtr` | 0x00661000 | `&g_GfxPsxDataBuffer + offset` | 656 call sites |
| `Gfx_DebugPrint` | 0x00664e30 | writes a debug-log line when the context's log flags allow | 214 call sites in 12 subsystems |

## Main structures

| type or global | header | what it holds |
|---|---|---|
| engine context | `GfxEngineContext` (`lib/types/graphics.h`) | 0xAA0 bytes from `AllocEngineContext`; `GetEngineContext()` returns it, module callbacks take it as `int`. +0x00 quit flag; +0x20 minimised; +0x5C window; +0x60 exit code; +0x74/+0x78 `IDirectDraw`/`IDirectDraw2`; +0x7C.. front, back and more surfaces; +0x2C0 `IDirect3D`; +0x2C4 and +0x2E0 the two device paths; +0x2FC model matrix; +0x300 draw pool; +0x81C in-scene flag; +0x834..+0x83C log flags and target; +0x848..+0x85C viewport; +0x910 render-object bank; +0x91C/+0x920 depth sort flag and queue; +0x924 switch pending; +0x930 render state; +0x934 driver ops; +0x938 node list; +0x954/+0x958/+0x95C width, height, depth; +0x960 buffer count (1..3); +0x964 full screen; +0x984 Direct3D 5 path; +0x99C..+0x9A4 perspective (default 90, 5, 2000; `App_Bootstrap` sets 90, 125, 50000); +0x9F0..+0xA08 init, shutdown, enter, exit, frame, mouse, key; +0xA0C staged module; +0xA48 present enable; +0xA60 driver type; +0xA74 setup flags; +0xA8C DLL export table; +0xA90 texture cache on |
| `GameModule` | [lib/types/graphics.h](../../../../include/lib/types/graphics.h) | a module's seven callbacks (`init`, `shutdown`, then enter, exit, frame, mouse and key at +0x08..+0x18); the context holds `activeModule` (+0x9F0) and `pendingModule` (+0xA0C) |
| `GfxDriverOps` | [lib/types/graphics.h](../../../../include/lib/types/graphics.h) | 0xF0-byte typed callback table and background colour state from `GfxAllocDriverOps`: init/cleanup, lock/unlock, present (+0x10), clears, viewport, background colour (+0x24..+0x38), matrices, textures, palettes, render states (+0x64..+0x74), `drawPolygonSet` (+0x78), `drawBatchNode` (+0x7C), `drawRenderObject` (+0x80), scene calls (+0x84..+0x8C), the 2D/3D/line `GfxBatchEmitterFn` slots (+0x94..+0xE8), `renderDeferredLists` (+0xEC) |
| `g_GfxDllDriverExportTable[36]` | `graphics_driver_context_fields.cpp` | engine services (heap, texture formats, pixel packing, palettes, `D3D5CompileGroup`, `Gfx_LightPolygonSet`) stored at context +0xA8C for a driver DLL |
| `GfxRenderObject` | [lib/types/graphics.h](../../../../include/lib/types/graphics.h) | 0xE0-byte render object and per-frame batch state: +0x00 kind, +0x0C render-state record ("hundred"), +0x10 material, +0x14 polygon set, +0x2C `meshBank`, +0x30 `drawPoolPtr`, +0x34..+0x6C primitive and line strides/index buffers, +0x70..+0x8C batch reservation cursors and node chain, +0x90/+0x94 `beginEmitter`/`drawEmitter`, +0x98..+0xDF world matrix |
| `GfxPrimitiveMeshBank` | [lib/types/graphics.h](../../../../include/lib/types/graphics.h) | 0x70-byte bank at context +0x910: block heaps at +0x00 (render objects), +0x04 (batch primitives), +0x08 (batch nodes), +0x0C (array heap), sixteen per-kind template meshes at +0x10, shared index runs at +0x58..+0x68, engine context at +0x6C |
| `PfileLoadOptions` | [lib/pfile.h](../../../../include/lib/pfile.h) | load options for models, render objects and sprites: draw pool (+0x18), blend mode (+0x20), directory (+0x24), caller-owned texture (+0x28), scale (+0x38), the `FileOpenRequest` at +0x40 (LGP archive id at +0x48) |
| `GfxDrawPoolList`, `GfxDrawPoolNode` | lib/types/graphics.h | per-frame node lists; the pool's first word is the frame generation |
| `GfxPolygonSet` | lib/types/graphics.h | polygon set as the lighting and colour loops read it (vertex count, groups, lighting state) |
| `GfxMatrix4x4` | lib/types/graphics.h | float 4x4 matrix |
| `MatrixStack` | lib/types/stack.h | capacity, depth and an array of matrix pointers |
| `PsxMatrix`, `SVECTOR`, `VECTOR`, `CVECTOR` | lib/types/graphics.h | the PSX geometry types the GTE helpers take; `PsxMatrix` is 30 bytes, its translation at +0x12 |
| `g_GteState` / `g_pGteState` | `GraphicsGteState` (`lib/types/graphics.h`), defined in `psx.cpp` | the GTE stand-in: the installed rotation and translation (`rotMatrix`), the geometry and projection offsets, the matrix layout mode, the secondary and draw colours |
| `g_GfxPsxDataBuffer[1508]` | lib/api/graphics.h | the buffer `Gfx_GetPsxScratchpadPtr` indexes |
| `GfxDrawSpec` | lib/types/graphics.h | colour, UV and texture spec applied to each group by `Gfx_ApplyDrawSpecToGroup` |
| `GfxModelTable`, `GfxModelTableBank` | lib/types/graphics.h | model tables of `render.cpp` (battle is the main user) |
| `TextureCache::entries` | `texcache.cpp` | `std::map<std::pair<GfxTexHeader *, unsigned int>, LPDIRECTDRAWSURFACE>`: (texture header, the palette the surface was made with) to the surface; counters `g_TexCacheCount`, `g_TexCacheTotalBytes` and their maxima. The class name is not retail's (unknown) |

`GfxMatrix4x4`, `GfxPolygonGroupDrawSpec` and `GfxDriverLoadSpec` represent fixed-size driver and matrix blocks copied by assignment.

## Data flow

Startup:

1. `ApplyConfigToContext` (ff7/main) sets the driver type +0xA60 from the registry: value 0 software
   (1), 1 Direct3D (0), 2 OpenGL (3), 3 a driver DLL (2, installed by `InstallGraphicsDriver`, which
   falls back to 1 on failure). It also sets full screen (+0x964 = 1), the Direct3D 5 path
   (+0x984 = 1) and the texture cache flag (+0xA90, from option bit 0x2).
2. `InitGraphicsDevice` binds the ops: `GfxBindDriverOpsD3D` (setup flags 3), `GfxBindDriverOpsDDrawBlit`
   (flags 1), the DLL's procedure, or `Gl_BindDriverOps`. Flag 1 runs the DirectDraw part:
   `DirectDrawEnumerateA`, `DirectDrawCreate`, display-mode and Direct3D device enumeration,
   cooperative level and display mode, then the primary surface with 1..3 buffers in full screen,
   or a clipper and separate back surfaces in a window. Flag 2 runs the
   Direct3D part: device pick (`D3DPickDeviceCallback`), Z buffer (`PickZBufferDepth`), device,
   texture formats, viewports, material. Then `Blit_BuildColorRampTables`, the driver's `init`, a
   full-window viewport, clear, present, clear.
3. `GfxInitRenderResources` fills the context: preset polygon sets (+0x944), two stats records, a
   file registry, two matrix stacks and the model matrix, the sorted queue (0x800 blocks, depth
   range 0x10000, 0x200 buckets), the draw pool (0x200 nodes), the render-object bank, a 0x14-entry
   PSX matrix stack (+0x918) and the node list (0x64).
4. `Gfx_CreatePrimitiveMeshBank` builds sixteen template meshes, one per `GfxDrawKind` 0..15, writes
   each with `Pfile_WriteBinary` as `TF2D.P` .. `SGT3D.P` and loads it back with `Pfile_LoadBinary`.

Per frame (`WinMain`, then a module frame such as `Swirl_ModuleFrame`):

1. `ServiceModuleSwitch`; the frame callback runs when the window is not minimised and
   `RestoreLostSurfaces` succeeds (it restores lost surfaces only for driver 0).
2. The module resets the pools: `ResetFrameDrawPool` advances the draw pool generation
   (`Gfx_AdvanceDrawPoolGeneration`). `AcquireBatchNode` and `Gfx_AllocDrawPoolListNode` empty any
   batch or list whose stored generation is older, so last frame's primitives drop out.
3. `Gfx_BeginSceneCall(0)`: on Direct3D `Gfx_BeginSceneD3D` begins the scene and selects pass 0.
4. The module fills batches (`AcquireBatchNode` plus vertex writes, or `Gfx_Emit*`) and draws them
   (`Gfx_EmitPrimitiveBatchList`, `Gfx_DrawRenderObjectCall`, `Gfx_DrawPolygonSetCall`). With depth
   sort on, `Gfx_EmitPrimitiveBatchList` hands batches to `Gfx_EnqueuePrimitiveBatchForDepthSort`;
   `Gfx_InsertSortedDrawRecord` buckets them by `1 - depth`.
5. `D3D5SubmitPolygonSet` and `Gfx_SubmitPolygonSetFallback` append some groups to the node list
   (+0x938) instead of drawing them. `GfxFlushFrameDrawList` later applies a node's draw spec
   where its group asks for it and draws every node through `drawBatchNode`.
6. `Gfx_SetScenePassCall(1)`, `GfxFlushFrameDrawList`, `Gfx_EndSortedDrawFrame`,
   `Gfx_EndSceneCall`.
7. `PresentFrame` -> the driver's `present`. Direct3D uses `GfxPresentBackBuffer`; the software
   driver's `Gfx_BlitContextRectsAndPresent` copies its rectangles into surface 1 and then calls
   it. In full screen it flips, retrying on a lost or busy surface; in a window it blits the back
   buffer to the window rectangle. Then `UpdateFrameTiming`.

Textures:

- A textured render object gets its texture from `Gfx_AllocTexturedHundred`: `Pfile_LoadRsdTexture`
  (lib/polygon) unless the options carry one, then `Gfx_AcquireTextureForRequest`.
- The Direct3D `loadTexture` slot is `GfxCreateTextureSurfaces` (`dx_graph.cpp`). With +0xA90 set it
  first asks `Gfx_TextureCacheFindSurface`. When surface creation fails it calls
  `Gfx_TextureCacheEvictForBytes`, which looks only at entries whose palette is no longer their
  header's palette in use (`GfxTexHeader::row`, +0xD0), and retries. A new surface is added with
  `Gfx_TextureCacheInsertSurface`, which counts 0x20000 bytes per entry whatever its size.

Files: `Gfx_ShpLoadFile` reads a `.shp` file (`LoadDiskFile`), `Gfx_SptLoadRecord` a `.spt` file
(`LoadWholeFile`). The library itself reads no LGP archive, `kernel.bin` or savemap field.

Leaving: `WinMain` calls the exit (+0x9FC), `Gfx_TextureCacheReleaseAllSurfaces`, the shutdown
callback (+0x9F4, `App_Shutdown`, which calls `ShutdownGraphicsDevice`), `DestroyWindow` and
`DestroyEngineContext`.

## Named codes

From [lib/enums/graphics.h](../../../../include/lib/enums/graphics.h):

| enum | used here by | what it names |
|---|---|---|
| `GfxDrawKind` | `Gfx_CreateRenderObject` (`dx_sfx.cpp`) | render object kinds: bit 0 3D, bit 1 Gouraud, bit 2 textured, bit 3 quad; 0x11 sprite batch, 0x12/0x13 lines |
| `RenderState` | `Gfx_SetRenderStateCall` callers in `dx_3d2d.cpp`, `dx_rend.cpp`, `dx_rend5.cpp`, `dx_rendi.cpp`, `psxgraph.cpp`, `render.cpp` | driver slot +0x64 switches (filter, perspective, colour key, dither, culling, depth, specular, wrap) |
| `BlendMode` | ff7/menu and the other game modules | semi-transparency modes in load options +0x20 |

## Notes

- `GfxEngineContext`, `GfxDriverOps`, `GfxRenderObject` and `GfxPrimitiveMeshBank` are unified typed
  records; `g_drv.cpp` and the DirectX/GL/software backends dispatch through typed `GfxDriverOps`
  function-pointer slots.
- A smaller set of helper structs in `lib/types/graphics.h` keep `pad_*` fields where only a subset of offsets is touched by a given Direct3D, GL or software helper.
- COM calls dispatch through full DirectX 5 and DirectShow pure-virtual vtable interfaces
  (`include/lib/com_abi.h`: `ComDirectDraw`, `ComDirectDrawSurface`, `ComDirect3DDevice2`, ...),
  and the render-state, primitive, and vertex-type constants use DirectX SDK enumerators
  (`SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL)`, from `lib/enums/graphics.h` where
  `<d3d.h>` is not in scope). The engine context's interface members carry what they hold in `GfxEngineContext`. Only `texcache.cpp`, `dx_spr.cpp`'s transform
  callbacks and one `Blt` call in `sw/sw.cpp` use the SDK interfaces themselves.
- Unreferenced hooks are named (`Gfx_PsxHookNoOp_1`, `Gfx_ClearTexturePageFill`).
- Large functions that are still dense: `InitGraphicsDevice` (about 640 lines, stack-slot union `frame`), `D3D5SubmitPolygonSet` (about 420), `Gfx_SubmitPolygonSetFallback`,
  `Gfx_EnqueueGroupForDepthSort`, `Gfx_CreateRenderObject` and the span writers in `sw_gfx_raster_asm_spans.cpp`.
- The `render.cpp` and much of the `psxgraph.cpp` documentation is still the one-line comment over each
  function.
