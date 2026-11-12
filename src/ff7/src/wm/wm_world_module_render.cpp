// src/ff7/src/wm/wm_world_module_render.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Runs the world map module's enter, exit and per-frame update, and its render dispatch
//   for the normal, underwater and snow terrain passes. It also holds the frame pacing and the
//   terrain render toggles.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/mem.h"
#include "lib/api/polygon.h"
#include "ff7/api/field.h"
#include "ff7/api/menu.h"
#include "ff7/api/wm.h"
#include "ff7/types/wm.h"
#include "lib/api/sound.h"
#include "lib/types/polygon.h"
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include "ff7/enums/main.h"
#include "lib/codegen.h"
#include "ff7/api/main.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "ff7/enums/wm.h"
#include "ff7/types/field.h"
#include "ff7/field.h"
extern "C" {

struct GfxEngineContext;

// clang-format off
// Never read. 0x00969940 before it is "Success", the last literal of creative_sfman_error_messages.obj.
// GLOBAL: FF7 0x00969948
int g_WorldUnusedFlag = 1;
// GLOBAL: FF7 0x0096994c
int g_WorldTerrainRenderEnabled = 1;
// GLOBAL: FF7 0x00969950
int g_WorldFrameTimingFlag = 1;
// GLOBAL: FF7 0x00969958
double g_WorldTargetFps30 = 30.0;
// GLOBAL: FF7 0x00969960
double g_WorldTargetFps22 = 22.0;
/* The debug banner WorldMap_ModuleEnter prints, "-=-=[START OF WORLD MAP!!!]=-=-\n",
 * stored as a 36-byte char array in .data (WorldMap_ModuleExit's END banner is
 * an inline string literal). */
// GLOBAL: FF7 0x00969968
char g_WorldStartBanner[36] = {0x2d, 0x3d, 0x2d, 0x3d, 0x5b, 0x53, 0x54, 0x41, 0x52, 0x54, 0x20, 0x4f, 0x46, 0x20, 0x57, 0x4f, 0x52, 0x4c, 0x44, 0x20, 0x4d, 0x41, 0x50, 0x21, 0x21, 0x21, 0x5d, 0x3d, 0x2d, 0x3d, 0x2d, 0x0a, 0x00, 0x00, 0x00, 0x00};
/* The viewport is g_WorldRenderWidth by g_WorldRenderHeight pixels (320x240, 640x480 at 2x). */
// GLOBAL: FF7 0x00de67e8
int g_WorldRenderHeight = 0;
unsigned char g_bssPad_00de67ec[4] = {0};
// The frame-start timestamp: WorldMap_ModuleFrame stores the processor's time-stamp counter here
// through ReadTimestampCounter; World_WaitForFrameInterval subtracts it (Sub64).
// GLOBAL: FF7 0x00de67f0
Timestamp64 g_WorldFrameStartTimestamp = {0};
unsigned char g_bssPad_00de67f8[256] = {0};
// GLOBAL: FF7 0x00de68f8
int g_WorldRenderWidth = 0;
typedef void (__cdecl *WorldMapModuleFn)(void);
/* The terrain pass of the current world map: World_FlushTerrainBatches, ..Underwater or ..Snow,
 * which wmfile.cpp stores when it loads the map. */
// GLOBAL: FF7 0x00de68fc
WorldMapModuleFn g_WorldTerrainFlushFn = 0;
// The UI scale factor (1.0f or 2.0f) WorldMap_ModuleEnter sets; World_UpdateSkyHorizonPolygon divides by it.
// GLOBAL: FF7 0x00de6900
float g_WorldUiScaleFactor = 0.0f;
unsigned char g_bssPad_00de6904[4] = {0};
// GLOBAL: FF7 0x00de6908
double g_WorldTimestampDiff = 0.0;
// The current timestamp World_WaitForFrameInterval reads through ReadTimestampCounter.
// GLOBAL: FF7 0x00de6910
Timestamp64 g_WorldCurrentTimestamp = {0};
// The module record WorldMap_ModuleFrame fills (GetActiveModule stores the active module's fields, the
// frame then sets the five callbacks) and hands to RequestModuleSwitch, which copies it.
// GLOBAL: FF7 0x00de6918
GameModule g_WorldModuleRecord = {0};
unsigned char g_bssPad_00de6934[4] = {0};
// GLOBAL: FF7 0x00de6938
double g_WorldFpsInterval = 0.0;
// WorldMap_MainUpdate's result (World_GetRenderBufferIndex), stored each frame; WorldMap_ModuleExit
// copies its low word to g_FieldWorldMapResult.
// GLOBAL: FF7 0x00de6940
int g_WorldMainUpdateResult = 0;
unsigned char g_bssPad_00de6944[4] = {0};
unsigned char g_bssPad_00de6948[144] = {0};
// GLOBAL: FF7 0x00de69d8
int g_WorldDisplayScale = 0;
// GLOBAL: FF7 0x00de69dc
int g_WorldMapExitRequested = 0;
// GLOBAL: FF7 0x00de69e0
int g_WorldMapInitialized = 0;
// GLOBAL: FF7 0x00de69e4
int g_WorldModeResetDone = 0;
unsigned char g_bssPad_00de69e8[4] = {0};
// GLOBAL: FF7 0x00de69ec
int g_WorldFrameCount = 0;
/* How many timestamp ticks the last slow frame ran over its interval; the next frame waits that
 * much less. */
// GLOBAL: FF7 0x00de69f0
double g_WorldFrameOverrun = 0.0;
// GLOBAL: FF7 0x00de69f8
int g_WorldFrameWaitPending = 0;
unsigned char g_bssPad_00de69fc[4] = {0};
// clang-format on
__declspec(dllimport) unsigned long __stdcall SetBkColor(void *, unsigned long);
__declspec(dllimport) unsigned long __stdcall SetTextColor(void *, unsigned long);
__declspec(dllimport) int __stdcall TextOutA(void *, int, int, const char *, int);
__declspec(dllimport) int __stdcall lstrlenA(const char *);


#define WORLD_LEADER_MODEL_COUNT 3
#define WORLD_STAGE_MODEL_COUNT 0xD


/*
 * The world map module's mouse-message callback. It is empty: the world map takes no mouse input
 * and reads the controls each frame through Input_Update in WorldMap_ModuleFrame instead.
 */
// FUNCTION: FF7 0x0074ba80
void __cdecl WorldMap_ModuleMouse(void)
{
    /* The original body is empty. */
}

/*
 * The world map module's keyboard-message callback. It is empty: key presses reach the world map
 * only through the per-frame Input_Update in WorldMap_ModuleFrame.
 */
// FUNCTION: FF7 0x0074ba85
void __cdecl WorldMap_ModuleKeyboard(void)
{
    /* The original body is empty. */
}

/*
 * Draws debug text overlay using GDI TextOutA on device context.
 */
// FUNCTION: FF7 0x0074ba8a
void __cdecl World_DrawDebugTextOverlay(int x, int y, const char *text, void *dc)
{
    SetBkColor(dc, 0xff0000);
    SetTextColor(dc, 0xffff);
    TextOutA(dc, x, y, text, lstrlenA(text));
    TextOutA(dc, 0, g_WorldRenderHeight - 32, (const char *)g_WorldWalkDebugText, lstrlenA((const char *)g_WorldWalkDebugText));
}

/*
 * Enters the world map. On a fresh entry (g_WorldMapInMenu clear, so not a return from the menu
 * the world map was suspended under) it:
 * - sets the animation transform flags to 0x4051 and clears the playing ambience id and the
 *   texture page counts;
 * - picks the layout from GetUiScalingMode: mode 2 renders 640x480 at the origin with display
 *   scale 2 (UI scale 2.0); mode 1 renders 320x240 centred at (160,120); mode 0 renders 320x240 at
 *   the origin; modes 1 and 0 use scale 1 (UI scale 1.0). The menu viewport follows the same mode;
 * - sets the GTE matrix layout mode 1 and projection distance 240, and the input auto-repeat to a
 *   first delay of 200 and an interval of 50;
 * - allocates the world arena (World_AllocArena), clears the initialised, exit-requested and
 *   first-frame flags and the model handles of the 3 leader and 13 stage model records, so the
 *   first frame loads them;
 * - sets the viewport rectangle, marks no texture set loaded (-1) and sets the near and far planes
 *   to 500 and 40000.
 * Every entry, returns from the menu included, then sets the frame-pacing intervals from the TSC
 * frequency: g_WorldFpsInterval to one 30 fps frame and g_WorldTimestampDiff to one 22 fps frame
 * (World_WaitForFrameInterval waits for the first and runs a double frame past the second). It
 * clears the frame count, enables terrain rendering and the double-frame timing flag, and clears
 * the screen flash and its sound.
 */
// FUNCTION: FF7 0x0074baf5
void __cdecl WorldMap_ModuleEnter(GfxEngineContext *ctx)
{
    int flags;
    int i;
    int mode;

    Gfx_DebugPrint(g_WorldStartBanner);
    if (g_WorldMapInMenu == 0) {
        mode = GetUiScalingMode();
        flags = 0x4051;
        Anim_SetTransformFlags(&flags);
        World_ClearAmbienceId();
        g_WorldTexturePageCount = 0;
        g_WorldTerrainTextureCount = g_WorldTexturePageCount;
        if (mode == 2) {
            g_WorldViewportX = 0;
            g_WorldViewportY = 0;
            g_WorldDisplayScale = 2;
            g_WorldUiScaleFactor = 2.0f;
            g_WorldRenderWidth = 0x280;
            g_WorldRenderHeight = 0x1E0;
            Menu_ApplyUiScalingViewportForMode(2);
        } else {
            if (mode == 1) {
                g_WorldViewportX = 0xA0;
                g_WorldViewportY = 0x78;
                Menu_ApplyUiScalingViewportForMode(1);
            } else {
                g_WorldViewportX = 0;
                g_WorldViewportY = 0;
                Menu_ApplyUiScalingViewportForMode(0);
            }
            g_WorldDisplayScale = 1;
            g_WorldUiScaleFactor = 1.0f;
            g_WorldRenderWidth = 0x140;
            g_WorldRenderHeight = 0xF0;
        }
        SetMatrixLayoutMode(1);
        SetGeomScreenReference(240.0);
        Input_SetRepeatRates(0xC8, 0x32);
        World_AllocArena();
        g_WorldMapInitialized = 0;
        g_WorldMapExitRequested = g_WorldMapInitialized;
        g_WorldModeResetDone = g_WorldMapExitRequested;
        for (i = 0; i < WORLD_LEADER_MODEL_COUNT; i++) {
            g_WorldLeaderModels[i].active = 0;
        }
        for (i = 0; i < WORLD_STAGE_MODEL_COUNT; i++) {
            g_WorldStageModels[i].active = 0;
        }
        GfxSetViewportRect(g_WorldViewportX, g_WorldViewportY, g_WorldRenderWidth, g_WorldRenderHeight, ctx);
        g_WorldCurrentTextureSetId = -1;
        ctx->nearPlane = 500.0f;
        ctx->farPlane = 40000.0f;
    }
    g_WorldFpsInterval = ctx->tscFrequency / g_WorldTargetFps30;
    g_WorldFrameCount = 0;
    g_WorldFrameWaitPending = 0;
    World_SetTerrainRenderEnabled(1);
    World_SetDoubleFrameEnabled(1);
    World_DisarmScreenFlash();
    World_StopFlashSoundAndClearScreenFlash();
    g_WorldTimestampDiff = ctx->tscFrequency / g_WorldTargetFps22;
}

/*
 * Releases the world stage and model resources while the world map is suspended under the menu
 * (g_WorldMapInMenu). WorldMap_ModuleExit releases the same five when it is not.
 */
// FUNCTION: FF7 0x0074bd50
void __cdecl World_ReleaseResourcesIfSuspended(void)
{
    if (g_WorldMapInMenu != 0) {
        World_ReleaseStageTextures();
        World_FreeStageModels();
        World_FreeLeaderModels();
        World_FreeArena();
        World_FreeLightGroups();
    }
}

/*
 * Leaves the world map. When it is not only suspended under the menu (g_WorldMapInMenu clear), it
 * releases the stage textures, stage and leader models, the arena and the light groups, then:
 * - if a field map was preloaded (g_FieldPreloadMapTarget nonzero) and it is the current field
 *   map id, clears the preload target and state;
 * - stores the low word of WorldMap_MainUpdate's last result in g_FieldWorldMapResult and sets the current
 *   field map id from g_FieldActorState.state.eventCmdParam;
 * - by g_FieldReturnMode: WORLD_EXIT_FIELD switches to GAME_MODE_FIELD, WORLD_EXIT_BATTLE to
 *   GAME_MODE_BATTLE_SWIRL (both with GAME_MODE_WORLD_MAP as the previous mode),
 *   WORLD_EXIT_PLAIN_RETURN posts FIELD_REQUEST_WORLD_ENDING_MOVIE; any other value changes nothing.
 * It always prints the end-of-world-map debug line.
 */
// FUNCTION: FF7 0x0074bd77
void __cdecl WorldMap_ModuleExit(void)
{
    if (!g_WorldMapInMenu) {
        World_ReleaseStageTextures();
        World_FreeStageModels();
        World_FreeLeaderModels();
        World_FreeArena();
        World_FreeLightGroups();
        /* the then-arm is empty in the original */
        if (g_FieldEntryMapId != g_FieldPreloadMapTarget || g_FieldPreloadMapTarget == 0) { /* empty first arm: lib/codegen.h */
        } else {
            g_FieldPreloadMapTarget = 0;
            g_FieldPreloadState = 0;
        }
        g_FieldWorldMapResult = *(short *)&g_WorldMainUpdateResult;
        g_FieldEntryMapId = g_FieldActorState.state.eventCmdParam;
        switch (g_FieldReturnMode) {
        case WORLD_EXIT_FIELD:
            g_EnginePreviousMode = GAME_MODE_WORLD_MAP;
            g_EngineGameMode = GAME_MODE_FIELD;
            break;
        case WORLD_EXIT_BATTLE:
            g_EnginePreviousMode = GAME_MODE_WORLD_MAP;
            g_EngineGameMode = GAME_MODE_BATTLE_SWIRL;
            break;
        case WORLD_EXIT_PLAIN_RETURN:
            g_FieldActorState.state.requestCode = FIELD_REQUEST_WORLD_ENDING_MOVIE;
            break;
        }
    }
    // STRING: FF7 0x0096998c
    Gfx_DebugPrint("-=-=[END OF WORLD MAP!!!]=-=-\n");
}

/*
 * Main per-frame update and render dispatcher for the world map module. When the last frame ran
 * slow (g_WorldFrameCount), it runs a second update and draw in the same scene. Then it hands over
 * to the game-mode dispatcher when the world map ends, or to the menu module while the menu is
 * open over it.
 */
// FUNCTION: FF7 0x0074be49
void __cdecl WorldMap_ModuleFrame(struct GfxEngineContext *ctx)
{
    GfxClearColorBgra color;
    int step;
    char block[208];

    step = 1;
    if (g_WorldFrameWaitPending != 0) {
        World_WaitForFrameInterval(ctx);
    }
    if (g_WorldFrameCount != 0) {
        g_WorldFrameWaitPending = 0;
    } else {
        g_WorldFrameWaitPending = 1;
    }
    ReadTimestampCounter(&g_WorldFrameStartTimestamp);
    Input_Update(0);
    SoundUpdateFades();
    Gfx_ResetNodeList(ctx);
    Gfx_BeginSortedDrawFrame(ctx);
    ResetFrameDrawPool(ctx);
    ResetRenderPools(ctx);
    color.r = 0;
    color.g = 0;
    color.b = 0;
    color.a = FLOAT_ONE_BITS;
    GfxSetBackgroundColorCall(&color, ctx);
    GfxClearCall(1, 1, 1, ctx);
    if (((int (__cdecl *)(int scene, struct GfxEngineContext *context))Gfx_BeginSceneCall)(0, ctx) != 0) { /* reads EAX left by driver beginScene */
        if (g_WorldFrameCount != 0) {
            World_SetTerrainRenderEnabled(0);
        } else {
            World_SetTerrainRenderEnabled(1);
        }
        g_WorldMainUpdateResult = WorldMap_MainUpdate(&g_FieldReturnMode, &g_FieldNextModuleArg, &g_WorldBattleRequest, 0);
        g_WorldTerrainFlushFn();
        GfxFlushFrameDrawList(ctx);
        Gfx_EndSortedDrawFrame(ctx);
        Gfx_SetScenePassCall(1, ctx);
        Gfx_EmitPrimitiveBatchList(g_WorldScreenOverlayRenderObjects, ctx);
        /* `1 &&`: the original tests a constant here, which VC5 does not fold */
        if (1 && g_WorldFrameCount != 0) {
            g_WorldFrameWaitPending = 1;
            g_WorldFrameCount = 0;
            World_SetTerrainRenderEnabled(1);
            ctx->framesThisSecond = ctx->framesThisSecond + 1.0;
            Gfx_ResetNodeList(ctx);
            Gfx_BeginSortedDrawFrame(ctx);
            ResetFrameDrawPool(ctx);
            ResetRenderPools(ctx);
            ReadTimestampCounter(&g_WorldFrameStartTimestamp);
            Input_Update(0);
            g_WorldMainUpdateResult = WorldMap_MainUpdate(&g_FieldReturnMode, &g_FieldNextModuleArg, &g_WorldBattleRequest, 0);
            g_WorldTerrainFlushFn();
            GfxFlushFrameDrawList(ctx);
            Gfx_EndSortedDrawFrame(ctx);
            Gfx_SetScenePassCall(1, ctx);
            Gfx_EmitPrimitiveBatchList(g_WorldScreenOverlayRenderObjects, ctx);
        }
        Gfx_EndSceneCall(ctx);
    }
    if (g_WorldMapExitRequested != 0) {
        if (REG_ALLOC_HINT)
            ;
        if (REG_ALLOC_HINT)
            ;
        g_WorldMapExitRequested = g_WorldMapInitialized = 0;
        GetActiveModule(&g_WorldModuleRecord, ctx);
        g_WorldModuleRecord.onMouse = App_OnMouseMessage;
        g_WorldModuleRecord.onKey = App_OnKeyMessage;
        g_WorldModuleRecord.frame = GameModeDispatcher_Frame;
        g_WorldModuleRecord.enter = GameModeDispatcher_Enter;
        g_WorldModuleRecord.exit = GameModeDispatcher_Exit;
        RequestModuleSwitch(&g_WorldModuleRecord, ctx);
    } else if (g_WorldMapInMenu != 0) {
        GetActiveModule(&g_WorldModuleRecord, ctx);
        g_WorldModuleRecord.onMouse = (GameModuleMessageCallback)Menu_ModuleMouse;
        g_WorldModuleRecord.onKey = (GameModuleMessageCallback)Menu_ModuleKeyboard;
        g_WorldModuleRecord.frame = Menu_ModuleFrame;
        g_WorldModuleRecord.enter = Menu_ModuleEnter;
        g_WorldModuleRecord.exit = (GameModuleCallback)Menu_ModuleExit;
        RequestModuleSwitch(&g_WorldModuleRecord, ctx);
    }
}

/*
 * Emits terrain texture batches and draws stage models for standard rendering.
 */
// FUNCTION: FF7 0x0074c179
void __cdecl World_FlushTerrainBatches(void)
{
    GfxEngineContext *ctx;
    unsigned int i;

    ctx = GetEngineContext();
    if (World_IsTerrainRenderEnabled() == 0) {
        return;
    }
    Gfx_SetScenePassCall(0, ctx);
    for (i = 0; i < 0x16; i++) {
        Gfx_EmitPrimitiveBatchList(g_WorldTexturePageRenderObjects[g_WorldAnimTexturePages[i]], ctx);
    }
    for (i = 0; i < g_WorldTexturePageCount; i++) {
        if (World_IsAnimatedTexturePage(i) == 0) {
            Gfx_EmitPrimitiveBatchList(g_WorldTexturePageRenderObjects[i], ctx);
        }
    }
    Gfx_EmitPrimitiveBatchList(g_WorldSkyRenderObject, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldMidgarRenderObject, ctx);
    Gfx_SetScenePassCall(1, ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldCloudRenderObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldMeteorRenderObject, ctx);
    Gfx_SetScenePassCall(1, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldShadowRenderObject, ctx);
    Gfx_SetScenePassCall(0, ctx);
    World_DrawStageModels(ctx);
    Gfx_SetScenePassCall(1, ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldTerrainRenderObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldMiniMapRenderObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldRadarRenderObject, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldDfxRenderObject0, ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldDfxRenderObject1, ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldDfxRenderObject2, ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldDfxRenderObject3, ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldMapRenderObject, ctx);
    if (g_WorldMsgInputUpdated != 0) {
        Menu_SetMessageWindowYOffset(0);
        Menu_RenderMessageWindows(g_FieldWindows, 1, World_GetRenderBufferIndex() == 0);
        Menu_FlushUiBatches();
        g_WorldMsgInputUpdated = 0;
        GfxSetViewportRect(g_WorldViewportX, g_WorldViewportY, g_WorldRenderWidth, g_WorldRenderHeight, ctx);
    }
}

/*
 * Emits underwater terrain batches and models in scene pass order.
 */
// FUNCTION: FF7 0x0074c3f0
void __cdecl World_FlushTerrainBatchesUnderwater(void)
{
    struct {
        unsigned int i;
        struct GfxEngineContext *ctx;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.ctx = GetEngineContext();
    if (World_IsTerrainRenderEnabled() == 0)
        return;
    Gfx_SetScenePassCall(0, s.ctx);
    for (s.i = 0; s.i < g_WorldTexturePageCount; s.i++)
        Gfx_EmitPrimitiveBatchList(g_WorldTexturePageRenderObjects[s.i], s.ctx);
    World_DrawStageModels(s.ctx);
    Gfx_SetScenePassCall(1, s.ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldTerrainRenderObject, s.ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldMiniMapRenderObject, s.ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldRadarRenderObject, s.ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, s.ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldDfxRenderObject0, s.ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldDfxRenderObject1, s.ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldDfxRenderObject2, s.ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldDfxRenderObject3, s.ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldShadowRenderObject, s.ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldMapRenderObject, s.ctx);
    if (g_WorldMsgInputUpdated) {
        Menu_SetMessageWindowYOffset(0);
        Menu_RenderMessageWindows(g_FieldWindows, 1, World_GetRenderBufferIndex() == 0);
        Menu_FlushUiBatches();
        g_WorldMsgInputUpdated = 0;
        GfxSetViewportRect(g_WorldViewportX, g_WorldViewportY, g_WorldRenderWidth, g_WorldRenderHeight, s.ctx);
    }
}

/*
 * Emits snow terrain batches and models in scene pass order.
 */
// FUNCTION: FF7 0x0074c589
void __cdecl World_FlushTerrainBatchesSnow(void)
{
    struct {
        unsigned int i;
        struct GfxEngineContext *ctx;
    } s;
    s.ctx = GetEngineContext();
    if (World_IsTerrainRenderEnabled() == 0)
        return;
    Gfx_SetScenePassCall(0, s.ctx);
    for (s.i = 0; s.i < g_WorldTexturePageCount; s.i++)
        Gfx_EmitPrimitiveBatchList(g_WorldTexturePageRenderObjects[s.i], s.ctx);
    Gfx_SetScenePassCall(0, s.ctx);
    World_DrawStageModels(s.ctx);
    Gfx_SetScenePassCall(1, s.ctx);
    Gfx_EmitPrimitiveBatchList((*(void **)&g_WorldSnowRenderObjects), s.ctx);
    Gfx_EmitPrimitiveBatchList((void *)g_WorldSnow5RenderObject, s.ctx);
    Gfx_EmitPrimitiveBatchList(g_WorldShadowRenderObject, s.ctx);
    if (g_WorldMsgInputUpdated) {
        Menu_SetMessageWindowYOffset(0);
        Menu_RenderMessageWindows(g_FieldWindows, 1, World_GetRenderBufferIndex() == 0);
        Menu_FlushUiBatches();
        g_WorldMsgInputUpdated = 0;
        GfxSetViewportRect(g_WorldViewportX, g_WorldViewportY, g_WorldRenderWidth, g_WorldRenderHeight, s.ctx);
    }
}

/*
 * Draws leader and stage model skeletons for the current terrain view.
 */
// FUNCTION: FF7 0x0074c6b0
void __cdecl World_DrawStageModels(GfxEngineContext *drawContext)
{
    int count;
    int i;

    if (World_IsTerrainRenderEnabled() == 0) {
        return;
    }
    if (g_WorldLeaderModels[0].active != 0) {
        Anim_DrawSkeletonFrame(g_WorldLeaderModels[0].frame, (PolygonSkeletonAnim *)g_WorldLeaderModels[0].animations[g_WorldLeaderModels[0].animation], (PolygonBoneTransform *)g_WorldLeaderModels[0].bonePlacement, (PolygonAnimDraw *)g_WorldLeaderModels[0].skeleton, drawContext);
    }
    count = World_GetLoadedStageModelCount();
    for (i = 0; i < count; i++) {
        if (g_WorldStageModels[i].active != 0) {
            if (g_WorldStageModels[i].needsScenePass != 0) {
                Gfx_SetScenePassCall(1, drawContext);
            }
            Anim_DrawSkeletonFrame(g_WorldStageModels[i].frame, (PolygonSkeletonAnim *)g_WorldStageModels[i].animations[g_WorldStageModels[i].animation], (PolygonBoneTransform *)g_WorldStageModels[i].bonePlacement, (PolygonAnimDraw *)g_WorldStageModels[i].skeleton, drawContext);
            if (g_WorldStageModels[i].needsScenePass != 0) {
                Gfx_SetScenePassCall(0, drawContext);
            }
        }
    }
}

/*
 * Paces the frame by busy-waiting on the timestamp counter: up to the 30 fps interval, or, after
 * a frame that ran past the 22 fps interval, sets g_WorldFrameCount so that the next frame runs
 * twice and waits for the overrun. The first reading subtracts 358484.5 ticks, the later ones do
 * not. ctx is not used.
 */
// FUNCTION: FF7 0x0074c7e4
void __cdecl World_WaitForFrameInterval(struct GfxEngineContext *ctx)
{
    struct {
        Timestamp64 delta;
        double elapsed;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.elapsed = 0;
    ReadTimestampCounter(&g_WorldCurrentTimestamp);
    Sub64(&g_WorldCurrentTimestamp, &g_WorldFrameStartTimestamp, &s.delta);
    s.elapsed = CombineSplitValueToFloat(&s.delta) - 358484.5;
    if (!g_WorldFrameCount) {
        if (s.elapsed <= g_WorldFpsInterval) {
            while (s.elapsed < g_WorldFpsInterval) {
                ReadTimestampCounter(&g_WorldCurrentTimestamp);
                Sub64(&g_WorldCurrentTimestamp, &g_WorldFrameStartTimestamp, &s.delta);
                s.elapsed = CombineSplitValueToFloat(&s.delta);
            }
            g_WorldFrameCount = 0;
            g_WorldFrameOverrun = 0;
        } else {
            if (s.elapsed <= g_WorldTimestampDiff) {
                g_WorldFrameCount = 0;
            } else {
                g_WorldFrameOverrun = s.elapsed - g_WorldTimestampDiff;
                if (World_GetDoubleFrameEnabled())
                    g_WorldFrameCount = 1;
                else
                    g_WorldFrameCount = 0;
            }
        }
    } else {
        while (s.elapsed < g_WorldTimestampDiff - g_WorldFrameOverrun) {
            ReadTimestampCounter(&g_WorldCurrentTimestamp);
            Sub64(&g_WorldCurrentTimestamp, &g_WorldFrameStartTimestamp, &s.delta);
            s.elapsed = CombineSplitValueToFloat(&s.delta);
        }
        g_WorldFrameCount = 0;
        g_WorldFrameOverrun = 0;
    }
}

/*
 * Returns whether terrain rendering is enabled.
 */
// FUNCTION: FF7 0x0074c969
int __cdecl World_IsTerrainRenderEnabled(void)
{
    return g_WorldTerrainRenderEnabled;
}

/*
 * Sets terrain rendering enabled state.
 */
// FUNCTION: FF7 0x0074c973
void __cdecl World_SetTerrainRenderEnabled(int value)
{
    g_WorldTerrainRenderEnabled = value;
}

/*
 * Sets the world render frame timing flag.
 */
// FUNCTION: FF7 0x0074c980
void __cdecl World_SetDoubleFrameEnabled(int value)
{
    g_WorldFrameTimingFlag = value;
}

/*
 * Returns the world render frame timing flag.
 */
// FUNCTION: FF7 0x0074c98d
int __cdecl World_GetDoubleFrameEnabled(void)
{
    return g_WorldFrameTimingFlag;
}
} /* extern "C" */
