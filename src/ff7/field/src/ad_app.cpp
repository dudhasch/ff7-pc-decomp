// src/ff7/field/src/ad_app.cpp
// Original: C:\FF7\field\src\ad_app.cpp
// Contents: Field module lifecycle: viewport and display mode globals, map configuration and
//   runtime setup, splash textures, module init and shutdown, the debug text helper, and the return
//   to the mode dispatcher.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/codegen.h"
#include "ff7/types/field.h"
#include "ff7/enums/main.h"
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include "ff7/api/field.h"
#include "ff7/api/movie.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "lib/api/sound.h"
#include "lib/api/file.h"
#include <windows.h>
#include "lib/pfile.h"
#include "ff7/api/main.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/field.h"
#include "lib/enums/file.h"
extern "C" {

struct GfxEngineContext;

// clang-format off
// The head of the TU's .data: the flags the field loop sets and tests, then the map-id tables.
// GLOBAL: FF7 0x00905ac8
int g_FieldCanContinue = 1;
// GLOBAL: FF7 0x00905acc
int g_FieldMapInitFrame = 1;
// GLOBAL: FF7 0x00905ad0
int g_FieldDisplayModeDirty = 1;
// GLOBAL: FF7 0x00905ad4
int g_FieldEnableRendering = 1;
// GLOBAL: FF7 0x00905ad8
int g_FieldLoadAssets = 1;
// GLOBAL: FF7 0x00905adc
int g_FieldAnimPausedEntity = (-1);
// Reader-bounded table (indexed i < 12, stride 4).
// GLOBAL: FF7 0x00905ae0
int g_FieldMapList_ClearLightFlag8[12] = {347, 547, 526, 411, 773, 420, 174, 185, 697, 699, 590, 520};
// GLOBAL: FF7 0x00905b10
int g_FieldMapList_BgOffsets[2] = {470, 694};
// GLOBAL: FF7 0x00905b18
int g_FieldMapValues_BgOffsets[2] = {2, 4};
// Reader-bounded table (indexed i < 9, stride 4).
// GLOBAL: FF7 0x00905b20
int g_FieldMapList_AnimPausedEntity[9] = {127, 160, 225, 267, 360, 442, 510, 517, 370};
// GLOBAL: FF7 0x00905b44
int g_FieldMapValues_AnimPausedEntity = 5;
unsigned char g_rawData_00905b48[32] = {0x05, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00};
// Reader-bounded table (indexed i < 5, stride 4; the 6th word (0) is a zero tail).
// GLOBAL: FF7 0x00905b68
int g_FieldMapList_LockedSkipsLines[6] = {457, 627, 762, 487, 169, 0};
// Reader-bounded table (indexed i < 55, stride 4).
// GLOBAL: FF7 0x00905b80
int g_FieldMapList_NearPlane512[55] = {658, 659, 660, 661, 662, 663, 664, 665, 666, 667, 668, 669, 670, 671, 672, 673, 674, 675, 676, 677, 678, 679, 680, 681, 682, 683, 684, 178, 462, 205, 464, 195, 173, 214, 172, 222, 720, 148, 149, 154, 174, 177, 749, 230, 120, 495, 330, 323, 548, 324, 400, 712, 713, 727, 443};
// GLOBAL: FF7 0x00cc2278
unsigned char g_FieldTileDepthThreshold[4] = {0};
unsigned char g_bssPad_00cc227c[4] = {0};
// The field map config table: 1,200 records of 0xD0 bytes (Field_InitMapConfigTable writes them all).
// GLOBAL: FF7 0x00cc2280
FieldMapRecord g_FieldMapRecords[1200] = {0};
// GLOBAL: FF7 0x00cff180
unsigned char g_FieldModelCommandGate[72] = {0};
// GLOBAL: FF7 0x00cff1c8
struct FieldClearColor g_FieldClearColor = {0};
// GLOBAL: FF7 0x00cff1d8
int g_FieldDataReady = 0;
// GLOBAL: FF7 0x00cff1dc
int g_FieldDisplayMode = 0;
// GLOBAL: FF7 0x00cff1e0
int g_FieldViewportX = 0;
// GLOBAL: FF7 0x00cff1e4
int g_FieldViewportY = 0;
// GLOBAL: FF7 0x00cff1e8
int g_FieldViewportWidth = 0;
// GLOBAL: FF7 0x00cff1ec
int g_FieldViewportHeight = 0;
// GLOBAL: FF7 0x00cff1f0
int g_FieldDisplayScaleFactor = 0;
// GLOBAL: FF7 0x00cff1f4
int g_FieldDisplayCenterX = 0;
// GLOBAL: FF7 0x00cff1f8
int g_FieldDisplayCenterY = 0;
// GLOBAL: FF7 0x00cff1fc
int g_FieldDisplayHalfCenterX = 0;
// GLOBAL: FF7 0x00cff200
int g_FieldDisplayHalfCenterY = 0;
// GLOBAL: FF7 0x00cff204
int g_FieldDisplayOffsetX = 0;
// GLOBAL: FF7 0x00cff208
int g_FieldDisplayHeight = 0;
// GLOBAL: FF7 0x00cff20c
int g_FieldDisplayClipWidth = 0;
// GLOBAL: FF7 0x00cff210
int g_FieldDisplayClipHeight = 0;
// GLOBAL: FF7 0x00cff214
float g_FieldDisplayCenterXFloat = 0.0f;
// GLOBAL: FF7 0x00cff218
float g_FieldDisplayCenterYFloat = 0.0f;
// GLOBAL: FF7 0x00cff21c
float g_FieldDisplayScaleX = 0.0f;
// GLOBAL: FF7 0x00cff220
float g_FieldDisplayScaleY = 0.0f;
// GLOBAL: FF7 0x00cff224
float g_FieldDisplayAspectScale = 0.0f;
// GLOBAL: FF7 0x00cff228
unsigned char g_FieldHandTimName[11] = {0};
unsigned char g_bssPad_00cff234[4] = {0};
unsigned char g_bssPad_00cff238[16] = {0};
// GLOBAL: FF7 0x00cff248
unsigned char g_FieldBgLayer2OffsetY[4] = {0};
// GLOBAL: FF7 0x00cff24c
int g_FieldBgOriginOffsetY = 0;
unsigned char g_bssPad_00cff250[4] = {0};
// GLOBAL: FF7 0x00cff254
int g_FieldModuleEntered = 0;
// GLOBAL: FF7 0x00cff258
int g_FieldReloadPending = 0;
// GLOBAL: FF7 0x00cff25c
int g_FieldClearModelLightFlag8 = 0;
// GLOBAL: FF7 0x00cff260
char g_FieldLockedSkipsLines = 0;
// GLOBAL: FF7 0x00cff264
int g_FieldFrameReloadMode = 0;
// GLOBAL: FF7 0x00cff268
int g_FieldResumingFromBattle = 0;
unsigned char g_bssPad_00cff26c[4] = {0};
unsigned char g_bssPad_00cff270[16] = {0};
// GLOBAL: FF7 0x00cff280
int g_FieldMapChangePending = 0;
unsigned char g_bssPad_00cff284[4] = {0};
unsigned char g_bssPad_00cff288[4] = {0};
// GLOBAL: FF7 0x00cff28c
int g_FieldBgLayersDirty = 0;
// GLOBAL: FF7 0x00cff290
int g_FieldHoldAnimAfterWait = 0;
// GLOBAL: FF7 0x00cff294
int g_FieldSplashHeightBias = 0;
unsigned char g_bssPad_00cff298[20] = {0};
// GLOBAL: FF7 0x00cff2ac
int g_FieldMusicIdAlt = 0;
// The field data directory (strcpy destination), 0x100 bytes; g_FieldCameraMatrixWork follows it.
// GLOBAL: FF7 0x00cff2b0
char g_FieldDataDir[0x100] = {0};
// GLOBAL: FF7 0x00cff3b0
short g_FieldCameraMatrixWork[15] = {0};
// clang-format on
char *__cdecl strcpy(char *, const char *);
#pragma intrinsic(strcpy)
char *__cdecl strcat(char *, const char *);
unsigned int __cdecl strlen(const char *);
#pragma intrinsic(strcat)
#pragma intrinsic(strlen)

void *__cdecl memset(void *, int, unsigned int);
#pragma intrinsic(memset)

#pragma intrinsic(abs)

/*
 * Stores its four arguments into the viewport globals: viewportX into g_FieldViewportX, viewportY into
 * g_FieldViewportY, viewportWidth into g_FieldViewportWidth and viewportHeight into g_FieldViewportHeight. The writes are ordered viewportX,
 * viewportWidth, viewportY, viewportHeight, which is what the original emits.
 */
// FUNCTION: FF7 0x0060d810
void __cdecl Field_SetViewportRect(int viewportX, int viewportY, int viewportWidth, int viewportHeight)
{
    g_FieldViewportX = viewportX;
    g_FieldViewportWidth = viewportWidth;
    g_FieldViewportY = viewportY;
    g_FieldViewportHeight = viewportHeight;
}

/*
 * The two cursor picture names are the first literals of the TU, right after the .data tables: a
 * literal returned by a static __inline function is a parse-time unit, and under /Ob1 the call
 * expands to the plain operand, so strcpy keeps its generic copy loop.
 */
static __inline char *Field_HandTim1Name(void)
{
    // STRING: FF7 0x00905c5c
    return "hand_1.tim";
}
static __inline char *Field_HandTimName(void)
{
    // STRING: FF7 0x00905c68
    return "hand.tim";
}

/*
 * Stores field readiness/display mode and installs viewport/scaling/layout globals.
 * Mode two uses 640 by 448 doubled layout; mode zero uses 320 by 224 origin layout;
 * all other values use centered 320 by 224 viewport at 160/120. Does not validate
 * mode.
 */
// FUNCTION: FF7 0x0060d837
void __cdecl Field_SetDisplayMode(int dataReady, int displayMode)
{
    g_FieldDataReady = dataReady;
    g_FieldDisplayModeDirty = 1;
    g_FieldDisplayMode = displayMode;
    if (displayMode == 2) {
        Field_SetViewportRect(0, 0, 640, 448);
        g_FieldDisplayScaleFactor = 2;
        g_FieldDisplayCenterX = 320;
        g_FieldDisplayCenterY = 240;
        g_FieldDisplayHalfCenterX = 320;
        g_FieldDisplayHalfCenterY = 224;
        *(int *)&g_FieldDisplayCenterYFloat = FLOAT_224_BITS;
        *(int *)&g_FieldDisplayScaleX = 0x42000000;
        *(int *)&g_FieldDisplayCenterXFloat = 0x41f80000;
        *(int *)&g_FieldDisplayScaleY = FLOAT_0_125_BITS;
        *(int *)&g_FieldDisplayAspectScale = 0x3f781062;
        g_FieldDisplayOffsetX = 0;
        g_FieldDisplayHeight = 0;
        g_FieldDisplayClipWidth = 580;
        g_FieldDisplayClipHeight = 34;
        strcpy((char *)&g_FieldHandTimName, Field_HandTim1Name());
    } else {
        g_FieldDisplayScaleFactor = 1;
        *(int *)&g_FieldDisplayScaleX = 0x41800000;
        strcpy((char *)&g_FieldHandTimName, Field_HandTimName());
        g_FieldDisplayCenterX = 160;
        g_FieldDisplayCenterY = 120;
        g_FieldDisplayHalfCenterX = 160;
        g_FieldDisplayHalfCenterY = 112;
        *(int *)&g_FieldDisplayCenterYFloat = FLOAT_224_BITS;
        *(int *)&g_FieldDisplayCenterXFloat = 0x41700000;
        *(int *)&g_FieldDisplayScaleY = FLOAT_0_125_BITS;
        *(int *)&g_FieldDisplayAspectScale = 0x3f6f1aa0;
        if (displayMode == 0) {
            Field_SetViewportRect(0, 0, 320, 224);
            g_FieldDisplayOffsetX = 0;
            g_FieldDisplayHeight = 0;
            g_FieldDisplayClipWidth = 290;
            g_FieldDisplayClipHeight = 17;
        } else {
            g_FieldDisplayCenterX = 320;
            g_FieldDisplayCenterY = 240;
            Field_SetViewportRect(160, 120, 320, 224);
            g_FieldDisplayOffsetX = 160;
            g_FieldDisplayHeight = 120;
            g_FieldDisplayClipWidth = 450;
            g_FieldDisplayClipHeight = 137;
        }
    }
}

// Shared flag storage is defined in ad_data.c.
#pragma intrinsic(strcpy, strcat, memcpy)
void __cdecl Field_DrawDebugTextGdi(int, int, char *text, HDC hdc);

/*
 * Frees a field model record: its RSD model through the context, then the record.
 */
// FUNCTION: FF7 0x0060da59
void __cdecl Field_FreeModelRecord(FieldModelDescriptor *record, void *ctx)
{
    if (record != 0) {
        Pfile_FreeRsdModel(record->rsdModel, ctx);
        // STRING: FF7 0x00905c74
        GameHeapFree(record, "C:\\FF7\\field\\src\\ad_app.cpp",
                     244);
    }
}

/*
 * Allocate a sixteen-byte descriptor and prime three of its four
 * dwords. The fourth (rsdModel) is left as the allocator returned it.
 * The path literal is the TU's second copy of its own __FILE__; the
 * first belongs to Field_FreeModelRecord. The compiler pooled neither.
 */
// FUNCTION: FF7 0x0060da8d
void *__cdecl Field_AllocModelRecord(void)
{
    FieldModelDescriptor *block;

    // STRING: FF7 0x00905c90
    block = (FieldModelDescriptor *)GameHeapAlloc(1, 16, "C:\\FF7\\field\\src\\ad_app.cpp", 252);
    if (block != 0) {
        block->primed[0] = 1;
        block->primed[1] = 1;
        block->primed[2] = 4;
    }
    return block;
}

/*
 * Copies the selected map's configuration record into the live field globals, including
 * the clear color, line values, flags, and the map-specific sound reset for map 0x185.
 */
// FUNCTION: FF7 0x0060dad4
void __cdecl Field_ApplyMapConfig(struct GfxEngineContext *ctx)
{
    int i;

    for (i = 0; i < 18; i++) {
        ((int *)&g_FieldModelCommandGate)[i] = 1;
    }
    *(int *)&ctx->nearPlane = *(int *)&g_FieldMapRecords[g_FieldCurrentMapId].nearPlane;
    *(int *)&g_FieldTileDepthThreshold[0x0] = *(int *)&ctx->nearPlane;
    *(char *)&g_FieldMapInitFrame = 1;
    g_FieldClearModelLightFlag8 = g_FieldMapRecords[g_FieldCurrentMapId].clearLightFlag8;
    *(int *)&g_FieldBgLayer2OffsetY = g_FieldMapRecords[g_FieldCurrentMapId].bgLayer2OffsetY;
    g_FieldBgOriginOffsetY = g_FieldMapRecords[g_FieldCurrentMapId].bgOriginOffsetY;
    g_FieldAnimPausedEntity = g_FieldMapRecords[g_FieldCurrentMapId].animPausedEntity;
    g_FieldHoldAnimAfterWait = g_FieldMapRecords[g_FieldCurrentMapId].holdAnimAfterWait;
    g_FieldFxFadeBlue[0] = 0;
    g_FieldFxFadeGreen[0] = g_FieldFxFadeBlue[0];
    g_FieldFxFadeRed[0] = g_FieldFxFadeGreen[0];
    for (i = 0; i < 18; i++) {
        g_FieldModelOffsetY[i] =
            g_FieldMapRecords[g_FieldCurrentMapId].modelOffsetY[i];
        g_FieldModelOffsetZ[i] =
            g_FieldMapRecords[g_FieldCurrentMapId].modelOffsetZ[i];
        g_FieldModelVisible[i] = g_FieldMapRecords[g_FieldCurrentMapId].modelVisible[i];
    }
    g_FieldLockedSkipsLines = g_FieldMapRecords[g_FieldCurrentMapId].lockedSkipsLines;
    g_FieldFileUncompressedSize = g_FieldMapRecords[g_FieldCurrentMapId].fileSize;
    (*&g_FieldClearColor) = g_FieldMapRecords[g_FieldCurrentMapId].clearColor;
    Field_LoadTutorialText(g_FieldCurrentMapId);
    /* Entering map 0x185 (389) stops every sound effect; which map that is, the code does not say. */
    if (g_FieldCurrentMapId == 0x185) {
        SfxStopAll();
    }
}

/*
 * Brings a loaded map's runtime up, the last step of Field_ModuleEnter: builds the line-draw
 * table, resets the palette scroll origins, sets the camera-scroll flag and the fade colour
 * (0x80FFFFFF), parses the palette image blocks (g_FieldLoadAssets clear) or uploads the palette,
 * rebuilds the tile records and the background section, resets the projection offset and the
 * movie flag, parks the hand cursor at 0x7FFF and makes `ctx` the engine context. Then it creates
 * the texture pages (blend 4; flags 0x24 when g_FieldTileStateDc0 is 2) and the map's models
 * (options reset, flag 0x2000000, BLEND_KEEP, from LGP archive 0 when g_FieldLoadAssets is set) with one load-options block, and resets the map state. Returns 0 if
 * the models do not come up, else 1.
 *
 * Codegen: the block is reached through the pointer local `pp`. An unreferenced dword sits between
 * the block and the pointer (`pad`) to preserve VC5 /Od stack-slot layout.
 */
// FUNCTION: FF7 0x0060dced
int __cdecl Field_InitMapRuntime(struct GfxEngineContext *ctx)
{
    PfileLoadOptions opts;
    int pad;
    PfileLoadOptions *pp;

    pp = &opts;
    Field_InitLineDrawTable(ctx, pp);
    g_FieldPaletteCurrentOrgX = 0;
    g_FieldPaletteCurrentOrgY = 0;
    g_FieldPaletteTargetOrgX = 0;
    g_FieldPaletteTargetOrgY = 0;
    g_FieldCameraScrollEcc = 1;
    g_FieldFxFadeColor = 0x80ffffff;
    g_FieldBgLayersDirty = 0;
    if (g_FieldLoadAssets == 0) {
        Field_ParsePaletteImageBlocks((int)g_pFieldPalette);
    } else {
        Field_UploadPalette((int)g_pFieldPalette);
    }
    Field_AllocTileRecords();
    Field_ParseBackgroundSection();
    SetProjectionOffsetAndRebuild(0, 0);
    g_MovieFlag = 0;
    g_FieldHandCursorX = 0x7fff;
    g_FieldHandCursorY = 0x7fff;
    SetEngineContext((int)ctx);
    // `if (1)` is the original's: /Od keeps `mov eax, 1` / `test eax, eax` and the branch.
    if (1) {
        Field_ScanMapIdRangeAlt();
        Pfile_InitLoadOptionsWithBlend(BLEND_NONE, pp);
        if (g_FieldTileStateDc0 == 2) {
            pp->flags |= 0x24;
        }
        Field_CreateTexturePages(ctx, pp);
        Pfile_InitLoadOptions(pp);
        pp->flags |= 0x2000000;
        Pfile_SetLoadOptionsBlendMode(BLEND_KEEP, pp);
        if (g_FieldLoadAssets != 0) {
            pp->file.fromArchive = 1;
            pp->file.archiveId = 0;
            pp->file.callback = 0;
        }
        if (Field_InstantiateModels(pp, ctx) == 0) {
            return 0;
        }
        Field_ResetMapState();
    } else {
        return 0;
    }
    return 1;
}

/*
 * Loads the four water-splash textures sibuki1.tim..sibuki4.tim from the current directory into
 * g_pFieldSplashTexture[0..3], with one load-options block (blend 0; from LGP archive 1 when
 * g_FieldLoadAssets is set: the same 1, 1, 0 Field_InitModuleResources writes, which the
 * initialiser leaves alone).
 *
 * Codegen: four copies of the call, not a loop.
 */
// FUNCTION: FF7 0x0060de79
void __cdecl Field_LoadSplashTextures(struct GfxEngineContext *ctx)
{
    PfileLoadOptions opts;

    Pfile_InitLoadOptionsWithBlend(BLEND_AVERAGE, &opts);
    if (g_FieldLoadAssets != 0) {
        opts.file.fromArchive = 1;
        opts.file.archiveId = 1;
        opts.file.callback = 0;
    }
    // STRING: FF7 0x00905cac
    opts.directory = ".";
    // STRING: FF7 0x00905cb0
    g_pFieldSplashTexture[0] = Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, "sibuki1.tim",
                                                      (void *)ctx->renderObjectBank);
    // STRING: FF7 0x00905cbc
    g_pFieldSplashTexture[1] = Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, "sibuki2.tim",
                                                      (void *)ctx->renderObjectBank);
    // STRING: FF7 0x00905cc8
    g_pFieldSplashTexture[2] = Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, "sibuki3.tim",
                                                      (void *)ctx->renderObjectBank);
    // STRING: FF7 0x00905cd4
    g_pFieldSplashTexture[3] = Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, "sibuki4.tim",
                                                      (void *)ctx->renderObjectBank);
}

/*
 * Destroys the four splash textures and clears their slots.
 */
// FUNCTION: FF7 0x0060df46
void __cdecl Field_FreeSplashTextures(void)
{
    int i;

    for (i = 0; i < 4; i++) {
        if (g_pFieldSplashTexture[i] != 0) {
            DestroyRenderObject(g_pFieldSplashTexture[i]);
            g_pFieldSplashTexture[i] = 0;
        }
    }
}

/*
 * Bring the field module up: clear two flags, set the data directory,
 * build the map tables, allocate the human and object buffers, load
 * the splash textures, and leave the script pointer null for the map
 * loader to fill.
 * The options block here is initialised without a blend and then given
 * blend mode 4 through Pfile_SetLoadOptionsBlendMode before the same
 * three members Field_LoadSplashTextures writes are written, this time
 * unconditionally.
 */
// FUNCTION: FF7 0x0060df96
void __cdecl Field_InitModuleResources(struct GfxEngineContext *ctx, char *dir,
                                       char *objArg)
{
    PfileLoadOptions opts;

    g_FieldResumingFromBattle = 0;
    g_pFieldFileBuffer = 0;
    Field_CacheDirectDrawSurfaceHandles(ctx);
    Field_InitMapConfigTable();
    Field_AllocAdListBuffer();
    Field_SetDataDir(dir);
    Field_SetModelDataDir(objArg);
    Field_LoadMapListFromLgp();
    Field_AllocHumanBuffers();
    Field_BindFxBlock();
    g_FieldTileStateDbc = 1;
    Pfile_InitLoadOptions(&opts);
    Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &opts);
    opts.file.fromArchive = 1;
    opts.file.archiveId = 1;
    opts.file.callback = 0;
    Field_InitHandCursor(ctx, &opts);
    Field_LoadSplashTextures(ctx);
    Field_CreateFxRenderObjects(ctx);
    g_FieldActorState.state.fadeType = 0;
    g_FieldModuleEntered = 0;
    g_FieldScripts = 0;
}

/*
 * Tear the field module's resources down, in the reverse of the order Field_InitModuleResources
 * brought them up: the map tables, the list and palette buffers, the human buffers, one loose
 * render object, then the four splash textures. The last store clears the flag Field_ShutdownModule
 * reads to decide whether the field file buffers still need releasing. The sixth call,
 * Field_DestroyFxRenderObjects (field_fx_fade_overlays.cpp), is Field_FreeSplashTextures's twin over
 * the three render objects of g_pFieldFxRenderObjects.
 */
// FUNCTION: FF7 0x0060e06f
void __cdecl Field_FreeModuleResources(void)
{
    Field_FreeAdDataBuffer();
    Field_FreeAdListBuffer();
    Field_FreePaletteWorkBuffers();
    Field_FreeHumanBuffers();
    if (g_pFieldHandImageBuffer != 0) {
        DestroyRenderObject(g_pFieldHandImageBuffer);
        g_pFieldHandImageBuffer = 0;
    }
    Field_DestroyFxRenderObjects();
    Field_FreeSplashTextures();
    g_FieldResumingFromBattle = 0;
}

/*
 * Enters the field module. Logs "Field Start", runs Field_InitModelRuntimeTables, and sets
 * g_FieldResumingFromBattle when the previous game mode (g_EnginePreviousMode) is
 * GAME_MODE_BATTLE with a field file already loaded. Back from GAME_MODE_MENU (5) or
 * GAME_MODE_FIELD_SERVICE (0xD) it only re-applies the camera (near/far planes 50 and 32000,
 * SetGeomScreenReference, Field_ApplyMapConfig, transform flags 0x4051, the viewport) and, unless
 * the mode is 0xD, resets the fade state (g_FieldFxStepCounter and g_FieldActorState.state.fadeType/fadeSpeed/fadeAdjust). From any other mode it loads the map: builds `name\\name.ate` from
 * Field_GetMapNameById, records g_FieldCurrentMapId, loads and parses it (Field_ReturnToDispatcher
 * when the section g_FieldFieldFileLoaded tracks is missing), then sets up the camera and
 * flags as above, binds the request block, frees any background layers, stores the
 * frame-time-derived g_FieldFrameTimeDelta, and runs Field_InitMapRuntime. g_FieldModuleEntered is
 * set at the end.
 */
// FUNCTION: FF7 0x0060e0bd
void __cdecl Field_ModuleEnter(GfxEngineContext *ctx)
{
    char olddir[256];
    char w[376];
    int ret;
    int unitAttr;
    int attrs;
    char full[128];
    char name[128];

    g_FieldLogicExitState = 0;
    g_FieldFrameCounter = 0;
    // STRING: FF7 0x00905ce0
    Gfx_DebugPrint("Field Start\n");
    Field_InitModelRuntimeTables();
    if (g_EnginePreviousMode == GAME_MODE_BATTLE && g_pFieldFileBuffer != 0) {
        g_FieldResumingFromBattle = 1;
    } else {
        g_FieldResumingFromBattle = 0;
    }
    g_FieldCanContinue = 1;
    if (g_EnginePreviousMode == GAME_MODE_MENU || g_EnginePreviousMode == GAME_MODE_FIELD_SERVICE) {
        ctx->nearPlane = 50.0f;
        ctx->farPlane = 32000.0f;
        SetGeomScreenReference(g_FieldDisplayCenterYFloat);
        Field_ApplyMapConfig(ctx);
        Anim_GetDefaultTransformFlags(&unitAttr);
        unitAttr = 0x4051;
        Anim_SetTransformFlags(&unitAttr);
        GfxSetViewportRect(g_FieldViewportX, g_FieldViewportY, g_FieldViewportWidth, g_FieldViewportHeight, ctx);
        if (g_EnginePreviousMode != GAME_MODE_FIELD_SERVICE) {
            g_FieldFxStepCounter = 0;
            g_FieldActorState.state.fadeType = 1;
            g_FieldActorState.state.fadeSpeed = 16;
            g_FieldActorState.state.fadeAdjust = 256;
            g_FieldActorState.state.red = 0;
            g_FieldActorState.state.green = 0;
            g_FieldActorState.state.blue = 0;
        }
    } else {
        Field_GetMapNameById(g_FieldEntryMapId, name);
        strcpy(full, name);
        // STRING: FF7 0x00905cf0
        strcat(full, "\\");
        strcat(full, name);
        // STRING: FF7 0x00905cf4
        strcat(full, ".ate");
        g_FieldCurrentMapId = g_FieldEntryMapId;
        if (Field_LoadMapByName(full) != 0) {
            Field_ParseAllSections();
            if (g_FieldFieldFileLoaded == 0) {
                Field_ReturnToDispatcher(ctx, 1);
            }
            g_FieldReloadPending = 1;
        }
        g_FieldLoadAssets = 1;
        ret = 0;
        if (1) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
            ctx->nearPlane = 50.0f;
            ctx->farPlane = 32000.0f;
            SetGeomScreenReference(g_FieldDisplayCenterYFloat);
            Anim_GetDefaultTransformFlags(&attrs);
            attrs = 0x4051;
            Anim_SetTransformFlags(&attrs);
            Field_ApplyMapConfig(ctx);
            Pfile_SetDefaultModelFlags(0x100000);
            Gfx_SetScenePassCall(0, ctx);
            Field_RebuildCameraMatrices(ctx);
            ret = 1;
            Field_ReadSurfacePixelFormat(ctx);
            GetCurrentDirectoryA(256, olddir);
            if (g_FieldLoadAssets != 0) {
                Pfile_SetAssetSourceMode(0);
            }
            Field_BindRequestBlock();
            Field_FreeBackgroundLayers();
            GfxSetViewportRect(g_FieldViewportX, g_FieldViewportY, g_FieldViewportWidth, g_FieldViewportHeight, ctx);
            g_FieldFrameTimeDelta = ctx->tscFrequency / 30.0 - 10000.0;
            ret = Field_InitMapRuntime(ctx);
            g_FieldModuleEntered = 1;
        }
    }
}

/*
 * Tears the field module down: frees the tutorial buffer, the background layers, the palette
 * buffers, the models, the tile buffers and the line-draw object, and logs "Field Quit". The field
 * file buffers stay when the game is going into a battle (GAME_MODE_BATTLE_SWIRL or
 * GAME_MODE_BATTLE), so the map comes back without a reload; g_FieldResumingFromBattle carries
 * that decision and is cleared afterwards. `ctx` is never read (the caller passes it and cleans the
 * stack for it).
 */
// FUNCTION: FF7 0x0060e45d
void __cdecl Field_ShutdownModule(struct GfxEngineContext *ctx)
{
    if (g_EngineGameMode == GAME_MODE_BATTLE_SWIRL || g_EngineGameMode == GAME_MODE_BATTLE) {
        g_FieldResumingFromBattle = 1;
    } else {
        g_FieldResumingFromBattle = 0;
    }
    Field_FreeTutaddrBuffer();
    Field_FreeBackgroundLayers();
    Field_FreeAdPalBuffer();
    Field_FreePaletteWorkBuffers();
    Field_FreeModels();
    Field_FreeTileBuffers();
    Field_DestroyLineDrawRenderObject();
    if (g_FieldResumingFromBattle == 0) {
        Field_ReleaseFieldFileBuffers();
    }
    g_FieldResumingFromBattle = 0;
    Field_ReleaseGraphicsNoOp();
    // STRING: FF7 0x00905cfc
    Gfx_DebugPrint("Field Quit\n");
}

/*
 * Draw one line of debug text through GDI: blue ground, yellow ink,
 * length measured by lstrlenA rather than strlen.
 */
// FUNCTION: FF7 0x0060e4dd
void __cdecl Field_DrawDebugTextGdi(int x, int y, char *text, HDC hdc)
{
    SetBkColor(hdc, 0x00ff0000);
    SetTextColor(hdc, 0x0000ffff);
    TextOutA(hdc, x, y, text, lstrlenA(text));
}

/*
 * Leaves the field for the top-level mode dispatcher: shuts the field module down when `shutdown`
 * is set, records GAME_MODE_FIELD as the previous mode, clears the input button latches and
 * switches to a module record of GameModeDispatcher's frame, enter and exit handlers and the
 * app's mouse and key handlers. RequestModuleSwitch copies the record's seven dwords into the
 * engine context's pendingModule (a GameModule); only five are written here, and the two at the front go
 * across as whatever the frame held.
 */
// FUNCTION: FF7 0x0060e521
void __cdecl Field_ReturnToDispatcher(struct GfxEngineContext *ctx, int shutdown)
{
    GameModule module;

    // A null test on a function of this file: /Od keeps it (`mov eax, offset
    // _Field_ShutdownModule`, `test eax, eax`).
    if (shutdown != 0 && Field_ShutdownModule != 0) {
        Field_ShutdownModule(ctx);
    }
    g_EnginePreviousMode = GAME_MODE_FIELD;
    Field_MapResetHookNoOp();
    Input_ClearButtonLatches();
    module.frame = GameModeDispatcher_Frame;
    module.onMouse = App_OnMouseMessage;
    module.onKey = App_OnKeyMessage;
    module.enter = GameModeDispatcher_Enter;
    module.exit = GameModeDispatcher_Exit;
    RequestModuleSwitch(&module, ctx);
}

/*
 * Aborts the field to the quit state: sets the return mode g_EngineGameMode to 0x13 and returns to the
 * dispatcher with flag 1.
 */
// FUNCTION: FF7 0x0060e58c
void __cdecl Field_AbortToQuit(struct GfxEngineContext *ctx, void *message)
{
    g_EngineGameMode = GAME_MODE_QUIT;
    Field_ReturnToDispatcher(ctx, 1);
}

/*
 * Restarts the field's frame count: sets g_FieldFrameCounter to 0, as Field_ModuleEnter does.
 * Field_ModuleFrame and Field_ModuleCatchUpFrame count it up by one per frame and stop at 10; no
 * code reads it otherwise. Field_EndMoviePlayback calls it when a movie ends.
 */
// FUNCTION: FF7 0x0060e5a8
void __cdecl Field_ResetFrameCounter(void)
{
    g_FieldFrameCounter = 0;
}

/*
 * Runs one field frame or reloads the pending map. The normal path applies the map clear color,
 * updates model instances, and renders; the reload path tears down map resources, rebuilds the
 * `<map>\<map>.ate` path, parses the replacement, and reinitializes the runtime.
 */
// FUNCTION: FF7 0x0060e5b7
void __cdecl Field_ModuleFrame(struct GfxEngineContext *ctx)
{
    FieldClearColor color;
    int state;
    char mapName[128];
    char message[128];
    char full[128];

    state = 0;
    ReadTimestampCounter(&g_FieldTimestampCounter);
    if (*(signed char *)&g_FieldMapInitFrame == 0) {
        ctx->presentEnable = 1;
    } else {
        *(char *)&g_FieldMapInitFrame = 0;
    }
    if (g_FieldFrameCounter < 10) {
        g_FieldFrameCounter++;
    }
    if (Field_UpdateLogic() == 0) {
        state = Field_GetExitRequest();
        if (state != 0) {
            if (state == 1) {
                Field_ReturnToDispatcher(ctx, 0);
            } else {
                Field_ReturnToDispatcher(ctx, 1);
            }
            return;
        }
        g_FieldMapChangePending = 0;
        Gfx_ResetNodeList(ctx);
        color = (*&g_FieldClearColor);
        GfxSetBackgroundColorCall(&color, ctx);
        GfxClearAllCall(ctx);
        Field_PromotePaletteDirtyFlag();
        Field_UpdateModelInstances();
        Field_RenderFrame(ctx);
        return;
    }

    g_FieldFrameReloadMode = 1;
    g_FieldResumingFromBattle = 0;
    if (g_FieldFxFadeRed[0] >= 200 &&
        g_FieldFxFadeGreen[0] >= 200 &&
        g_FieldFxFadeBlue[0] >= 200) {
        color.r = g_FieldFxFadeRed[0] / 255;
        color.g = g_FieldFxFadeGreen[0] / 255;
        color.b = g_FieldFxFadeBlue[0] / 255;
        color.a = 1.0f;
    } else {
        color.r = 0.0f;
        color.g = 0.0f;
        color.b = 0.0f;
        color.a = 1.0f;
    }
    g_FieldFxFadeRed[0] = 0;
    g_FieldFxFadeGreen[0] = 0;
    g_FieldFxFadeBlue[0] = 0;
    GfxSetBackgroundColorCall(&color, ctx);
    GfxClearAllCall(ctx);
    Field_PresentEngineFrame();
    GfxClearAllCall(ctx);
    Field_FreeTutaddrBuffer();
    Field_FreePaletteWorkBuffers();
    Field_FreeAdPalBuffer();
    Field_FreeBackgroundLayers();
    Field_FreeModels();
    Field_ReleaseFieldFileBuffers();
    Field_FreeTileBuffers();
    Field_DestroyLineDrawRenderObject();
    if (g_FieldCanContinue == 0) {
        Field_AbortToQuit(ctx, &g_FieldEventCompleteMessage);
        return;
    }
    g_FieldRenderActive = 0;
    Gfx_ResetNodeList(ctx);
    Field_GetMapNameById(g_FieldActorState.state.eventCmdParam, mapName);
    strcpy(full, mapName);
    // STRING: FF7 0x00905d08
    strcat(full, "\\");
    strcat(full, mapName);
    // STRING: FF7 0x00905d0c
    strcat(full, ".ate");
    g_FieldCurrentMapId = g_FieldActorState.state.eventCmdParam;
    if (Field_LoadMapByName(full) != 0) {
        Field_ParseAllSections();
        if (g_FieldFieldFileLoaded == 0) {
            Field_AbortToQuit(ctx, message);
        } else {
            g_FieldModuleEntered = 1;
            Field_RebuildCameraMatrices(ctx);
            Field_InitMapRuntime(ctx);
            g_FieldMapChangePending = 1;
            ctx->presentEnable = 0;
        }
    } else {
        Field_AbortToQuit(ctx, message);
    }
}

/*
 * Alternate field-frame path. It renders or reloads the map like
 * Field_ModuleFrame, but processes the exit request after rendering and reports whether the module
 * should keep running.
 */
// FUNCTION: FF7 0x0060e96c
int __cdecl Field_ModuleCatchUpFrame(struct GfxEngineContext *ctx)
{
    FieldClearColor color;
    int state;
    char mapName[128];
    char message[128];
    char full[128];

    state = 0;
    ReadTimestampCounter(&g_FieldTimestampCounter);
    if (*(signed char *)&g_FieldMapInitFrame == 0) {
        ctx->presentEnable = 1;
    } else {
        *(char *)&g_FieldMapInitFrame = 0;
    }
    if (g_FieldFrameCounter < 10) {
        g_FieldFrameCounter++;
    }
    if (Field_UpdateLogic() == 0) {
        g_FieldMapChangePending = 0;
        Gfx_ResetNodeList(ctx);
        color = (*&g_FieldClearColor);
        GfxSetBackgroundColorCall(&color, ctx);
        GfxClearAllCall(ctx);
        Field_PromotePaletteDirtyFlag();
        Field_UpdateModelInstances();
        state = Field_GetExitRequest();
        if (state != 0) {
            if (state == 1) {
                Field_ReturnToDispatcher(ctx, 0);
            } else {
                Field_RenderFrameAndResetPools(ctx);
                Field_ReturnToDispatcher(ctx, 1);
            }
            return 0;
        }
    } else {
        g_FieldFrameReloadMode = 2;
        g_FieldResumingFromBattle = 0;
        if (g_FieldFxFadeRed[0] >= 200 &&
            g_FieldFxFadeGreen[0] >= 200 &&
            g_FieldFxFadeBlue[0] >= 200) {
            color.r = g_FieldFxFadeRed[0] / 255;
            color.g = g_FieldFxFadeGreen[0] / 255;
            color.b = g_FieldFxFadeBlue[0] / 255;
            color.a = 1.0f;
        } else {
            color.r = 0.0f;
            color.g = 0.0f;
            color.b = 0.0f;
            color.a = 1.0f;
        }
        g_FieldFxFadeRed[0] = 0;
        g_FieldFxFadeGreen[0] = 0;
        g_FieldFxFadeBlue[0] = 0;
        GfxSetBackgroundColorCall(&color, ctx);
        GfxClearAllCall(ctx);
        Field_PresentEngineFrame();
        GfxClearAllCall(ctx);
        Field_FreeTutaddrBuffer();
        Field_FreePaletteWorkBuffers();
        Field_FreeAdPalBuffer();
        Field_FreeBackgroundLayers();
        Field_FreeModels();
        Field_ReleaseFieldFileBuffers();
        Field_FreeTileBuffers();
        Field_DestroyLineDrawRenderObject();
        if (g_FieldCanContinue == 0) {
            Field_AbortToQuit(ctx, &g_FieldEventCompleteMessage);
            return 0;
        }
        g_FieldRenderActive = 0;
        Gfx_ResetNodeList(ctx);
        Field_GetMapNameById(g_FieldActorState.state.eventCmdParam, mapName);
        strcpy(full, mapName);
        // STRING: FF7 0x00905d14
        strcat(full, "\\");
        strcat(full, mapName);
        // STRING: FF7 0x00905d18
        strcat(full, ".ate");
        g_FieldCurrentMapId = g_FieldActorState.state.eventCmdParam;
        if (Field_LoadMapByName(full) != 0) {
            Field_ParseAllSections();
            if (g_FieldFieldFileLoaded == 0) {
                Field_AbortToQuit(ctx, message);
                return 0;
            }
            g_FieldModuleEntered = 1;
            Field_RebuildCameraMatrices(ctx);
            Field_InitMapRuntime(ctx);
            g_FieldMapChangePending = 1;
            ctx->presentEnable = 0;
        } else {
            Field_AbortToQuit(ctx, message);
            return 0;
        }
    }
    return 1;
}

/*
 * The field module's mouse callback: stores the cursor position, packed as two shorts in `lparam`,
 * in the engine context's cursorX/cursorY.
 *
 * Codegen: `junk` is spelled so that it lands in its original stack slot;
 * nothing reads it.
 */
// FUNCTION: FF7 0x0060ed31
void __cdecl Field_ModuleMouse(int message, int wparam, unsigned int lparam, struct GfxEngineContext *ctx)
{
    int curX;
    int junk;
    int cursorY;

    curX = lparam & 0xffff;
    cursorY = (unsigned short)((lparam >> 16) & 0xffff);
    ctx->cursorX = curX;
    ctx->cursorY = cursorY;
    junk = message;
}

/*
 * The field module's keyboard callback. It does nothing: its one statement stores the name of a
 * debug capture file, "TEST.A", in a local.
 *
 * Codegen: the frame still reserves 0x18 bytes. `s` and `filename` are named for their slots: both
 * spellings land in the same VC5 hash bucket, so declaring the array first puts it below the
 * pointer.
 */
// FUNCTION: FF7 0x0060ed79
void __cdecl Field_ModuleKeyboard(void)
{
    char s[20];
    // STRING: FF7 0x00905d20
    char *filename = "TEST.A";
}

/*
 * Turns heap tracking off and creates an engine context for the field: 16-bit colour, full
 * screen, the mode flags, device 0, the cursor shown, near/far planes 50 and 32000, Direct3D 5;
 * sets the screen reference and transform flags 0x4051, reads APP.CFG into it and makes it the
 * engine context.
 *
 * Codegen: modeSetupDone is set to 1 and then tested; the test is in the image, so it is in the
 * source. The screen reference is passed as a double. `attr` is spelled so that it lands ahead of
 * the context pointer.
 */
// FUNCTION: FF7 0x0060ed8a
void __cdecl Field_InitEngineContext(int context, int config)
{
    GfxEngineContext *ctx;
    int attr;
    char *name;

    SetGameHeapTrackingEnabled(0);
    ctx = (GfxEngineContext *)CreateEngineContext();
    if (ctx != 0) {
        ctx->colorDepth = 0x10;
        ctx->fullScreen = 1;
        ctx->modeActive = 1;
        ctx->modeTransition = 0;
        ctx->modeSetupDone = 1;
        if (ctx->modeSetupDone != 0) {
            ctx->modeLoaded = 1;
        }
        ctx->deviceIndex = 0;
        ctx->showCursor = 1;
        ctx->nearPlane = 50.0f;
        ctx->farPlane = 32000.0f;
        ctx->useDirect3D5 = 1;
        SetGeomScreenReference(g_FieldDisplayCenterYFloat);
        Anim_GetDefaultTransformFlags(&attr);
        attr = 0x4051;
        Anim_SetTransformFlags(&attr);
        // STRING: FF7 0x00905d28
        name = "APP.CFG";
        Gfx_LoadAppConfigFile(name, ctx);
        SetEngineContext((int)ctx);
    }
}

/*
 * Dead scratch-buffer helper. The unreachable intrinsic leaves only its
 * esi/edi register saves around the 0xD4-byte frame in the shipped build.
 */
// FUNCTION: FF7 0x0060eea1
void __cdecl Field_RenderFrameNoOp(GfxEngineContext *context)
{
    char unused[212];

    // The debug texts of the original frame overlay: retail keeps their literals (after APP.CFG and
    // before flevel.siz) and no code; discarded expressions emit them here and generate none.
    // STRING: FF7 0x00905d30
    (void)"User Control Off";
    // STRING: FF7 0x00905d44
    (void)"User Control On";
    // STRING: FF7 0x00905d54
    (void)"Love Points:Erith %d,Tifa %d,Yufi %d,Barret%d";
    // STRING: FF7 0x00905d84
    (void)"FPS: %.2f";
    // STRING: FF7 0x00905d90
    (void)"Total Polygons: %d ";
    // STRING: FF7 0x00905da4
    (void)"quit flag:%d fade flag %d,fade color %d bk color r %d g %d b %d";
    // STRING: FF7 0x00905de4
    (void)"Skipping random battle";
    return;
    memcpy(unused, unused, sizeof(unused));
}

/*
 * Initializes 1,200 map configuration records, applies the hard-coded per-map exceptions, overlays
 * six compact lookup tables, then loads the size values from flevel.siz.
 */
// FUNCTION: FF7 0x0060eeb2
void __cdecl Field_InitMapConfigTable(void)
{
    int mapIndex;
    int modelIndex;

    g_FieldHoldAnimAfterWait = 0;
    for (mapIndex = 0; mapIndex < 1200; mapIndex++) {
        g_FieldMapRecords[mapIndex].nearPlane = 50.0f;
        g_FieldMapRecords[mapIndex].bgLayer2OffsetY = 0;
        g_FieldMapRecords[mapIndex].bgOriginOffsetY = 0;
        g_FieldMapRecords[mapIndex].animPausedEntity = -1;
        g_FieldMapRecords[mapIndex].holdAnimAfterWait = 0;
        g_FieldMapRecords[mapIndex].lockedSkipsLines = 0;
        if (mapIndex != 90) {
            g_FieldMapRecords[mapIndex].clearColor.r = 0.0f;
            g_FieldMapRecords[mapIndex].clearColor.g = 0.0f;
            g_FieldMapRecords[mapIndex].clearColor.b = 0.0f;
            g_FieldMapRecords[mapIndex].clearColor.a = 1.0f;
        } else {
            g_FieldMapRecords[mapIndex].clearColor.r = 0.0f;
            g_FieldMapRecords[mapIndex].clearColor.g = 0.047f;
            g_FieldMapRecords[mapIndex].clearColor.b = 0.09375f;
            g_FieldMapRecords[mapIndex].clearColor.a = 1.0f;
        }
        g_FieldMapRecords[mapIndex].clearLightFlag8 = 0;
        for (modelIndex = 0; modelIndex < 0x12; modelIndex++) {
            g_FieldMapRecords[mapIndex].modelOffsetY[modelIndex] = 0;
            g_FieldMapRecords[mapIndex].modelOffsetZ[modelIndex] = 0;
            g_FieldMapRecords[mapIndex].modelVisible[modelIndex] = 1;
        }
        if (mapIndex == 466) {
            g_FieldMapRecords[mapIndex].nearPlane = 1.0f;
            for (modelIndex = 0; modelIndex < 0x12; modelIndex++) {
                g_FieldMapRecords[mapIndex].modelOffsetY[modelIndex] = 0x1f;
                g_FieldMapRecords[mapIndex].modelOffsetZ[modelIndex] = -0x10;
            }
        }
        if (mapIndex == 154) {
            g_FieldMapRecords[mapIndex].modelOffsetZ[0] = 9;
            g_FieldMapRecords[mapIndex].modelOffsetZ[1] = 7;
        }
        if (mapIndex == 155) {
            g_FieldMapRecords[mapIndex].modelOffsetZ[0] = 9;
        }
        if (mapIndex == 204) {
            g_FieldMapRecords[mapIndex].modelOffsetY[6] = 0xb;
            g_FieldMapRecords[mapIndex].modelOffsetY[7] = 0xb;
            g_FieldMapRecords[mapIndex].modelOffsetZ[7] = -0x22;
            g_FieldMapRecords[mapIndex].modelOffsetZ[6] = -0x22;
        }
        if (mapIndex == 294) {
            g_FieldMapRecords[mapIndex].modelOffsetZ[0] = 4;
            g_FieldMapRecords[mapIndex].modelOffsetY[0] = -10;
            g_FieldMapRecords[mapIndex].modelOffsetZ[1] = 4;
            g_FieldMapRecords[mapIndex].modelOffsetY[1] = -10;
            g_FieldMapRecords[mapIndex].modelOffsetZ[2] = 4;
            g_FieldMapRecords[mapIndex].modelOffsetY[2] = -10;
            g_FieldMapRecords[mapIndex].modelOffsetZ[3] = 0;
            g_FieldMapRecords[mapIndex].modelOffsetY[3] = -10;
        }
        if (mapIndex == 295) {
            g_FieldMapRecords[mapIndex].modelOffsetZ[0] = 8;
            g_FieldMapRecords[mapIndex].modelOffsetY[0] = -10;
            g_FieldMapRecords[mapIndex].modelOffsetZ[1] = 2;
            g_FieldMapRecords[mapIndex].modelOffsetY[1] = -12;
            g_FieldMapRecords[mapIndex].modelOffsetZ[2] = 4;
            g_FieldMapRecords[mapIndex].modelOffsetY[2] = -10;
            g_FieldMapRecords[mapIndex].modelOffsetZ[3] = 0;
            g_FieldMapRecords[mapIndex].modelOffsetY[3] = -10;
        }
        if (mapIndex == 322) {
            g_FieldMapRecords[mapIndex].modelOffsetZ[4] = 0x19;
            g_FieldMapRecords[mapIndex].modelOffsetZ[5] = 0x19;
        }
        if (mapIndex == 141) {
            for (modelIndex = 0; modelIndex < 0x12; modelIndex++) {
                if (modelIndex == 1) {
                    g_FieldMapRecords[mapIndex].modelOffsetY[modelIndex] = 0x16;
                } else {
                    g_FieldMapRecords[mapIndex].modelOffsetY[modelIndex] = 0;
                }
                g_FieldMapRecords[mapIndex].modelOffsetZ[modelIndex] = 0;
            }
        }
        if (mapIndex == 202) {
            for (modelIndex = 0; modelIndex < 0x12; modelIndex++) {
                g_FieldMapRecords[mapIndex].modelOffsetZ[modelIndex] = 0xb;
            }
        }
        if (mapIndex == 704) {
            g_FieldMapRecords[mapIndex].modelVisible[1] = 0;
            g_FieldMapRecords[mapIndex].modelVisible[2] = 0;
        }
        if (mapIndex == 444) {
            for (modelIndex = 0; modelIndex < 0x12; modelIndex++) {
                g_FieldMapRecords[mapIndex].modelOffsetZ[modelIndex] = 0xa;
            }
        }
        if (mapIndex == 323 || mapIndex == 324) {
            for (modelIndex = 0; modelIndex < 0x12; modelIndex++) {
                g_FieldMapRecords[mapIndex].modelOffsetZ[modelIndex] = 0xc;
            }
        }
        if (mapIndex == 258) {
            for (modelIndex = 0; modelIndex < 0x12; modelIndex++) {
                g_FieldMapRecords[mapIndex].modelOffsetZ[modelIndex] = 0x1c;
            }
        }
        if (mapIndex == 138) {
            g_FieldMapRecords[mapIndex].nearPlane = 16.0f;
        }
        if (mapIndex == 139) {
            g_FieldMapRecords[mapIndex].nearPlane = 30.0f;
        }
        if (mapIndex == 253) {
            g_FieldMapRecords[mapIndex].holdAnimAfterWait = 1;
            g_FieldMapRecords[mapIndex].nearPlane = 56.0f;
        }
        if (mapIndex == 697) {
            g_FieldMapRecords[mapIndex].holdAnimAfterWait = 1;
        }
        if (mapIndex == 414) {
            g_FieldMapRecords[mapIndex].holdAnimAfterWait = 1;
        }
    }
    for (mapIndex = 0; mapIndex < 55; mapIndex++) {
        g_FieldMapRecords[g_FieldMapList_NearPlane512[mapIndex]].nearPlane = 512.0f;
    }
    for (mapIndex = 0; mapIndex < 12; mapIndex++) {
        g_FieldMapRecords[g_FieldMapList_ClearLightFlag8[mapIndex]].clearLightFlag8 = 1;
    }
    for (mapIndex = 0; mapIndex < 1; mapIndex++) {
        g_FieldMapRecords[g_FieldMapList_BgOffsets[mapIndex]].bgLayer2OffsetY = g_FieldMapValues_BgOffsets[mapIndex];
    }
    for (mapIndex = 1; mapIndex < 2; mapIndex++) {
        g_FieldMapRecords[g_FieldMapList_BgOffsets[mapIndex]].bgOriginOffsetY = g_FieldMapValues_BgOffsets[mapIndex];
    }
    for (mapIndex = 0; mapIndex < 9; mapIndex++) {
        g_FieldMapRecords[g_FieldMapList_AnimPausedEntity[mapIndex]].animPausedEntity = ((int *)&g_FieldMapValues_AnimPausedEntity)[mapIndex];
    }
    for (mapIndex = 0; mapIndex < 5; mapIndex++) {
        g_FieldMapRecords[g_FieldMapList_LockedSkipsLines[mapIndex]].lockedSkipsLines = 1;
    }
    Field_LoadFieldSizeTable();
}

/*
 * flevel.siz is 787 dwords, one per field map, and each becomes the
 * fileSize of that map's FieldMapRecord with 0x1E8480 (2,000,000)
 * added; whatever the file holds, the record wants it biased.
 * The two `__FILE__` strings are separate literals, one for the allocation
 * and one for the free, because VC5 does not pool duplicate literals. Each
 * needs its own STRING marker.
 * The whole body is the `then` of an if/else whose `else` is a bare
 * `return 0`: the image jumps forward past the loop and the free to
 * reach it, which a `if (f == 0) { return 0; }` guard would not do.
 * An unreferenced local still takes its stack slot, and `junk` is here to
 * hold it.
 */
// FUNCTION: FF7 0x0060f65a
int __cdecl Field_LoadFieldSizeTable(void)
{
    int size;
    void *buf;
    int *tab;
    void *f;
    int junk;
    int no;

    if (g_FieldLoadAssets == 0) {
        return 1;
    }
    // STRING: FF7 0x00905dfc
    f = FindLgpFile("flevel.siz", LGP_ARCHIVE_FIELD_LEVEL);
    if (f != 0) {
        size = GetLgpEntrySize((int)f, LGP_ARCHIVE_FIELD_LEVEL);
        // STRING: FF7 0x00905e08
        buf = GameHeapAllocRaw(size, "C:\\FF7\\field\\src\\ad_app.cpp", 1941);
        ReadLgpEntry(f, LGP_ARCHIVE_FIELD_LEVEL, buf, size);
        tab = (int *)buf;
        for (no = 0; no < 787; no++) {
            g_FieldMapRecords[no].fileSize = tab[no] + 0x1e8480;
        }
        if (buf != 0) {
            // STRING: FF7 0x00905e24
            GameHeapFree(buf, "C:\\FF7\\field\\src\\ad_app.cpp", 1954);
            buf = 0;
        }
    } else {
        return 0;
    }
    // The last two literals of the TU (after the __FILE__ above and before field_script_operands.cpp's
    // data): a mode and a file name that no code in the image reads, discarded expressions that emit no code.
    // STRING: FF7 0x00905e40
    (void)"wb";
    // STRING: FF7 0x00905e44
    (void)"on_line.sav";
    return 1;
}
} /* extern "C" */
