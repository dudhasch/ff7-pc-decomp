// src/ff7/condor/condor_engine_frame_render.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Owns the Fort Condor minigame's 3D engine context: creates and destroys it, paces and
//   presents frames, ticks the simulation and seeds its random number generator, loads skeletons
//   and animations, and renders the battlefield scene.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <windows.h>
#include "lib/pfile.h"
#include "lib/api/mem.h"
#include "lib/api/graphics.h"
#include <string.h>
#include "ff7/api/condor.h"
#include "lib/api/polygon.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/enums/graphics.h"
#include "ff7/types/condor.h"
#include "lib/codegen.h"
#include <stdlib.h>
#include <time.h>
extern "C" {

// clang-format off
// The condor world matrix pointer: the address of condor_game_state_hud.cpp's .bss fill.
// GLOBAL: FF7 0x00901a90
int g_CondorWorldMatrixPtr = (int)g_CondorWorldMatrixBuffer;
// A byte table indexed by a record's kind halfword (Mini_CondorRefreshSelectionAuxRecords subtracts
// the entry from the record's selection value).
// GLOBAL: FF7 0x00901a98
unsigned char g_CondorSelectionAuxByteTable[24] = {0x00, 0x28, 0x28, 0x2c, 0x28, 0x14, 0x1c, 0x2c, 0x2c, 0x00, 0x00, 0x00, 0x28, 0x28, 0x14, 0x14, 0x30, 0x26, 0x1e, 0x28, 0x14, 0x14, 0x14, 0x14};
// clang-format on
#pragma intrinsic(strcpy, strcat)
/*
 * Sets the active 3D graphics engine context.
 */
// FUNCTION: FF7 0x005f4c70
void __cdecl Mini_CondorSetEngineContext(int context)
{
    SetEngineContext(context);
}
/*
 * Queries the active 3D graphics engine context.
 */
// FUNCTION: FF7 0x005f4c81
void __cdecl Mini_CondorGetEngineContext(void)
{
    GetEngineContext();
}
/*
 * Requests termination of the graphics engine subsystem.
 */
// FUNCTION: FF7 0x005f4c8b
void __cdecl Mini_CondorRequestEngineQuit(GfxEngineContext *quitCode)
{
    RequestEngineQuit((int)quitCode);
}
/*
 * Destroys the current 3D graphics engine context.
 */
// FUNCTION: FF7 0x005f4c9c
void __cdecl Mini_CondorDestroyEngineContext(void)
{
    DestroyEngineContext();
}
/*
 * Synchronizes frame rate by busy-waiting until the target frame interval has elapsed.
 */
// FUNCTION: FF7 0x005f4ca6
void __cdecl Mini_CondorWaitForFrameTime(GfxEngineContext *context)
{
    double elapsed;
    int stampDiff[2];

    elapsed = 0;
    ReadTimestampCounter(&g_CondorTimestampNow);
    Sub64(&g_CondorTimestampNow, &g_CondorFrameTimestamp, (struct Timestamp64 *)stampDiff);
    elapsed = CombineSplitValueToFloat((struct Timestamp64 *)stampDiff);
    if (g_CondorFrameLate == 0) {
        if (elapsed <= g_CondorTicksPerFrame) {
            while (elapsed < g_CondorTicksPerFrame) {
                ReadTimestampCounter(&g_CondorTimestampNow);
                Sub64(&g_CondorTimestampNow, &g_CondorFrameTimestamp, (struct Timestamp64 *)stampDiff);
                elapsed = CombineSplitValueToFloat((struct Timestamp64 *)stampDiff);
            }
            g_CondorFrameLate = 0;
            g_CondorFrameTimeCarry = 0;
        } else {
            g_CondorFrameTimeCarry = elapsed - g_CondorTicksPerFrame;
            g_CondorFrameLate = 1;
        }
    } else {
        while (g_CondorTicksPerFrame - g_CondorFrameTimeCarry > elapsed) {
            ReadTimestampCounter(&g_CondorTimestampNow);
            Sub64(&g_CondorTimestampNow, &g_CondorFrameTimestamp, (struct Timestamp64 *)stampDiff);
            elapsed = CombineSplitValueToFloat((struct Timestamp64 *)stampDiff);
        }
        g_CondorFrameLate = 0;
        g_CondorFrameTimeCarry = 0;
    }
}
/*
 * Advances game frame, ticks minigame state, and presents the rendered frame.
 */
// FUNCTION: FF7 0x005f4df4
void __cdecl Mini_CondorTickAndPresent(GfxEngineContext *context)
{
    int phase;

    if (g_CondorStage < 4) {
        Mini_CondorRenderScene(context);
        if (g_CondorMinigameRunning == 0) {
            Mini_CondorSceneHookNoOp(context);
        } else {
            if (g_CondorFrameLate == 0) {
                Mini_CondorSceneHookNoOp(context);
                g_CondorFrameLate = 0;
            } else {
                Mini_CondorWaitForFrameTime(context);
                g_CondorFrameLate = 0;
                Mini_CondorReleaseBatchesAndResetDrawPools(context);
                Mini_CondorPerFrameHookNoOp();
                Input_ClearLatchedFlags();
                context->framesThisSecond += 1.0;
                ReadTimestampCounter(&g_CondorFrameTimestamp);
                phase = Mini_CondorUpdateFrameInput((int *)&g_CondorCameraMatrix, g_CondorMatchListState.cameraWorkMatrix);
                Mini_CondorDispatchPhase((short)phase);
                if (g_CondorStage >= 4) {
                    return;
                }
                Mini_CondorRenderScene(context);
                Mini_CondorSceneHookNoOp(context);
            }
            Mini_CondorWaitForFrameTime(context);
        }
        PresentFrame(context);
        Mini_CondorReleaseBatchesAndResetDrawPools(context);
        Mini_CondorPerFrameHookNoOp();
        Input_ClearLatchedFlags();
    }
}
/*
 * Ticks minigame logic and directly presents frame without secondary buffer swap.
 */
// FUNCTION: FF7 0x005f4f11
void __cdecl Mini_CondorTickAndPresentDirect(GfxEngineContext *context)
{
    if (g_CondorStage < 4) {
        Mini_CondorRenderScene(context);
        if (g_CondorMinigameRunning == 0) {
            Mini_CondorSceneHookNoOp(context);
        } else {
            if (g_CondorFrameLate == 0) {
                Mini_CondorSceneHookNoOp(context);
                g_CondorFrameLate = 0;
            } else {
                Mini_CondorWaitForFrameTime(context);
                g_CondorFrameLate = 0;
                Mini_CondorReleaseBatchesAndResetDrawPools(context);
                Mini_CondorPerFrameHookNoOp();
                Input_ClearLatchedFlags();
                context->framesThisSecond += 1.0;
                ReadTimestampCounter(&g_CondorFrameTimestamp);
                GfxClearAllCall(context);
                Mini_CondorUpdateFrameInput((int *)&g_CondorCameraMatrix, g_CondorMatchListState.cameraWorkMatrix);
                Mini_CondorCommitPendingCommand();
                Mini_CondorUpdateRecordSelectPanel();
                Mini_CondorUpdateUnitAnims();
                Mini_CondorDrawHudFrame();
                context->frameCount += 1.0;
                Mini_CondorRenderScene(context);
                Mini_CondorSceneHookNoOp(context);
            }
            Mini_CondorWaitForFrameTime(context);
        }
        PresentFrame(context);
        Mini_CondorReleaseBatchesAndResetDrawPools(context);
        Mini_CondorPerFrameHookNoOp();
        Input_ClearLatchedFlags();
    }
}
/*
 * Ends a Fort Condor frame: draws it and holds it to 60 fps, catching up after a late one.
 * Called by `Mini_CondorFrame` while the exit word g_CondorStage is below 4 (it does nothing
 * otherwise). Draws the scene, which `Mini_CondorRenderScene` skips while the late flag
 * g_CondorFrameLate is set. With pacing on (g_CondorMinigameRunning):
 * - not late: busy-waits the rest of the frame (`Mini_CondorWaitForFrameTime`, which sets the
 *   late flag when the frame overran);
 * - late: waits the frame minus the overrun, clears the flag, releases the batches and resets
 *   the draw pools and input latches, stamps a new frame start, adds 1 to the context's
 *   tickCount and runs one extra input and phase step; returns at once if that step set the exit
 *   word to 4 or more, otherwise draws the scene and waits the frame again.
 * Last, releases the batches, resets the draw pools and clears the latched input. Unlike
 * `Mini_CondorTickAndPresent` it does not present the frame.
 */
// FUNCTION: FF7 0x005f5042
void __cdecl Mini_CondorTick(GfxEngineContext *context)
{
    int phase;

    if (g_CondorStage < 4) {
        Mini_CondorRenderScene(context);
        if (g_CondorMinigameRunning == 0) {
            Mini_CondorSceneHookNoOp(context);
        } else {
            if (g_CondorFrameLate == 0) {
                Mini_CondorSceneHookNoOp(context);
                g_CondorFrameLate = 0;
            } else {
                Mini_CondorWaitForFrameTime(context);
                g_CondorFrameLate = 0;
                Mini_CondorReleaseBatchesAndResetDrawPools(context);
                Mini_CondorPerFrameHookNoOp();
                Input_ClearLatchedFlags();
                ReadTimestampCounter(&g_CondorFrameTimestamp);
                context->framesThisSecond += 1.0;
                phase = Mini_CondorUpdateFrameInput((int *)&g_CondorCameraMatrix, g_CondorMatchListState.cameraWorkMatrix);
                Mini_CondorDispatchPhase((short)phase);
                if (g_CondorStage >= 4) {
                    return;
                }
                Mini_CondorRenderScene(context);
                Mini_CondorSceneHookNoOp(context);
            }
            Mini_CondorWaitForFrameTime(context);
        }
        Mini_CondorReleaseBatchesAndResetDrawPools(context);
        Mini_CondorPerFrameHookNoOp();
        Input_ClearLatchedFlags();
    }
}
/*
 * Seeds pseudorandom number generator using tick timestamp from engine context.
 */
// FUNCTION: FF7 0x005f5153
void __cdecl Mini_CondorSeedRandomFromContext(struct GfxEngineContext *context)
{
    int seed;

    if (context->randomSeed != 0) {
        srand(context->randomSeed);
    } else {
        time((long *)&seed);
        srand(seed);
    }
}
/*
 * Allocates and initializes engine context for the Fort Condor minigame.
 */
// FUNCTION: FF7 0x005f5193
void __cdecl Mini_CondorCreateEngineContext(void)
{
    CreateEngineContext();
}
/*
 * Wrapper to initialize engine context and apply graphics parameters.
 */
// FUNCTION: FF7 0x005f519d
void __cdecl Mini_CondorInitEngineContextWrapper(int context, int params)
{
    Mini_CondorInitEngineContext(context, params);
}
/*
 * Submits a 2D textured quad primitive into the scene draw batch.
 */
// FUNCTION: FF7 0x005f51b2
void __cdecl Mini_CondorEmitTexturedQuad(int posX, int posY, int width, int height, int texU,
                                         int texV, int texW, int texH, int color, int paletteIndex,
                                         int depthBits, int unused, GfxRenderObject *renderObj)
{
    int *p;
    int *q;
    int *r;
    int *s;

    if (AcquireBatchNode(1, renderObj)) {
        *(char *)renderObj->colorIndex = (char)paletteIndex;
        renderObj->paletteIndex = paletteIndex;
        p = (int *)renderObj->vertexCursor;
        *(float *)p = (float)posX;
        *(float *)(p + 1) = (float)posY;
        p[2] = depthBits;
        *(float *)(p + 3) = 1.0f;
        p[4] = color;
        p[5] = 0xff000000;
        *(float *)(p + 6) = (float)texU;
        *(float *)(p + 7) = (float)texV;
        q = (int *)renderObj->vertexCursor + 8;
        *(float *)q = (float)posX;
        *(float *)(q + 1) = (float)posY + height;
        q[2] = depthBits;
        *(float *)(q + 3) = 1.0f;
        q[4] = color;
        q[5] = 0xff000000;
        *(float *)(q + 6) = (float)texU;
        *(float *)(q + 7) = (float)texV + texH;
        r = (int *)renderObj->vertexCursor + 16;
        *(float *)r = (float)posX + width;
        *(float *)(r + 1) = (float)posY;
        r[2] = depthBits;
        *(float *)(r + 3) = 1.0f;
        r[4] = color;
        r[5] = 0xff000000;
        *(float *)(r + 6) = (float)texU + texW;
        *(float *)(r + 7) = (float)texV;
        s = (int *)renderObj->vertexCursor + 24;
        *(float *)s = (float)posX + width;
        *(float *)(s + 1) = (float)posY + height;
        s[2] = depthBits;
        *(float *)(s + 3) = 1.0f;
        s[4] = color;
        s[5] = 0xff000000;
        *(float *)(s + 6) = (float)texU + texW;
        *(float *)(s + 7) = (float)texV + texH;
    }
}
/*
 * Applies transformation matrix to the active graphics rendering context.
 */
// FUNCTION: FF7 0x005f534f
int __cdecl Mini_CondorSetContextMatrix(GfxEngineContext *context, int matrix)
{
    Gfx_SetContextMatrix(&matrix, context);
    return 1;
}
/*
 * Checks if entity skeleton model is loaded, loading it from disk if not present.
 */
// FUNCTION: FF7 0x005f5369
int __cdecl Mini_LoadEntitySkeletonIfAbsent(struct GfxEngineContext *context, PolygonSkeletonLoadSpec *load, int blendMode)
{
    AnimPlayerInstance *animPlayer;
    int resource;
    PfileLoadOptions *loadOptions;
    PfileLoadOptions optionBlock;
    int option;

    if (load == 0) {
        return 0;
    } else {
        animPlayer = (AnimPlayerInstance *)(load->animPlayer);
        if (animPlayer == 0) {
            loadOptions = &optionBlock;
            Pfile_InitLoadOptions(loadOptions);
            Pfile_SetLoadOptionsBlendMode(blendMode, loadOptions);
            option = 0;
            resource = load->resource;
            animPlayer = (AnimPlayerInstance *)Skel_LoadHrcAndCreatePlayer(resource, option, load, (int)loadOptions, context);
            if (animPlayer != 0) {
                load->animPlayer = (int)animPlayer;
            }
        }
        return 1;
    }
}
/*
 * Loads an HRC hierarchy model from disk and attaches it to the entity.
 */
// FUNCTION: FF7 0x005f5403
int __cdecl Mini_LoadHrcFromDiskAndAttach(int hrcName, int entity, int flags, struct GfxEngineContext *context)
{
    PolygonSkelHierarchy *hierarchy;
    int mode;

    mode = 0;
    hierarchy = (PolygonSkelHierarchy *)Skel_LoadHrcFromDisk(mode, hrcName);
    if (hierarchy != 0) {
        Skel_BuildHierarchyAndLoadParts(entity, hierarchy, (struct PfileLoadOptions *)flags, (int)context);
        Skel_SetPartsVisible(1, hierarchy);
    }
    return (int)hierarchy;
}
/*
 * Instantiates an animation player object and binds the requested animation.
 */
// FUNCTION: FF7 0x005f5456
int __cdecl Mini_CondorCreateAnimPlayerWithAnimation(int playerId, PolygonSkelHierarchy *skel, struct GfxEngineContext *context, int animId)
{
    PolygonAnimPlayer *player;

    player = (PolygonAnimPlayer *)AnimPlayer_Create(playerId, 0, skel, (int)context);
    if (player != 0) {
        Mini_CondorBindAnimPlayerAnimationThunk(0, animId, player);
        player->tickEnabled = 0;
    }
    return (int)player;
}
/*
 * Loads animation data from disk or compiles text animation definition.
 */
// FUNCTION: FF7 0x005f549c
int __cdecl Mini_CondorLoadAnimation(char *path)
{
    PolygonAnimation *result;
    char buf[0x100];
    char dir[256];
    char ext[256];
    char drive[4];
    char file[256];

    result = 0;
    _splitpath(path, drive, dir, file, ext);
    // STRING: FF7 0x00901ab0
    strcpy(ext, UNFOLDED(".a"));
    strcpy(buf, drive);
    strcat(buf, dir);
    strcat(buf, file);
    strcat(buf, ext);
    if (Anim_CompileTextToBinary((int)path, (int)buf)) {
        result = (PolygonAnimation *)((int (__cdecl *)(char *openFlags))Anim_LoadBinaryFromDisk)(buf); /* reads eax after a void call, as the original does */
    }
    return (int)result;
}
/*
 * Binds an animation to the animation player.
 */
// FUNCTION: FF7 0x005f55fa
void __cdecl Mini_CondorBindAnimPlayerAnimationThunk(int slot, int animId, PolygonAnimPlayer *player)
{
    if (REG_ALLOC_HINT) {
    }
    AnimPlayer_BindAnimation(slot, animId, player);
}
/*
 * Loads unit skeleton model and constructs corresponding animation player.
 */
// FUNCTION: FF7 0x005f5613
int __cdecl Mini_CondorLoadSkeletonAndCreateAnimPlayer(int playerId, int animId, int mode, struct GfxEngineContext *context, int hrcName, int entity)
{
    PolygonAnimPlayer *player;
    PolygonSkelHierarchy *hierarchy;

    player = 0;
    hierarchy = (PolygonSkelHierarchy *)Mini_LoadHrcFromDiskAndAttach(hrcName, entity, mode, context);
    if (hierarchy != 0) {
        player = (PolygonAnimPlayer *)Mini_CondorCreateAnimPlayerWithAnimation(playerId, hierarchy, context, animId);
    }
    return (int)player;
}
/*
 * Converts a fixed-point PSX matrix into a 4x4 floating-point transform matrix.
 */
// FUNCTION: FF7 0x005f5663
void __cdecl Mini_CondorPsxMatrixToFloat4x4Thunk(PsxMatrix *psxMatrix, GfxFloat4 *dst)
{
    GtePsxMatrixToFloat4x4ColVec(psxMatrix, dst);
}
/*
 * Empties active render object batches and resets graphics draw pools.
 */
// FUNCTION: FF7 0x005f5678
void __cdecl Mini_CondorReleaseBatchesAndResetDrawPools(GfxEngineContext *pool)
{
    unsigned short i;
    unsigned short j;

    for (i = 0; i < 8; i++) {
        Gfx_EmptyRenderObjectBatch(((CondorRenderObject *)&g_CondorRenderObjects)[i].renderObject);
        ((CondorRenderObject *)&g_CondorRenderObjectsTail)[i].renderObject = 0;
    }
    for (i = 0; i < 13; i++) {
        if (((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[i].drawn != 0) {
            Gfx_EmptyRenderObjectBatch(((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[i].renderObject);
            ((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[i].drawn = 0;
        }
    }
    for (i = 0; i < 7; i++) {
        if (g_CondorUnitModelTable[i].ready != 0) {
            for (j = 0; j < 4; j++) {
                if (g_CondorUnitModelTable[i].parts[j].batched != 0) {
                    Gfx_EmptyRenderObjectBatch(g_CondorUnitModelTable[i].parts[j].renderObject);
                    g_CondorUnitModelTable[i].parts[j].batched = 0;
                }
            }
        }
    }
    ResetFrameDrawPool(pool);
    ResetRenderPools(pool);
}
/*
 * Draws the Fort Condor board for one frame; called by the tick. Returns without drawing after a
 * late frame (g_CondorFrameLate set) and while the battle state g_CondorMainState is 0xE (the
 * state whose handler begins the record select). Otherwise, in one scene:
 * - pass 0 with linear filtering and depth writes off: emits each of the eight batch lists whose
 *   pending flag is set;
 * - with depth writes and filtering on: the active units, then
 *   `Mini_CondorDrawPhaseSevenUnitModels`;
 * - with filtering off: the drawn render-object slots of mode 0 (13 slots) and the parts of the
 *   unit models of draw pass 0;
 * - pass 1, filtering off: the slots of mode 1 and the unit models of draw pass 1 (models 2
 *   and 3).
 * Ends the scene and, when the context has a stats record, stores the draw time at its +0xA0.
 */
// FUNCTION: FF7 0x005f5828
void __cdecl Mini_CondorRenderScene(GfxEngineContext *renderContext)
{
    struct Timestamp64 endTime;
    struct Timestamp64 beginTime;
    unsigned short i;
    unsigned short j;

    if (g_CondorFrameLate != 0) {
        return;
    }
    ReadTimestampCounter(&beginTime);
    if (g_CondorMainState == 0xE) {
        return;
    }
    Gfx_BeginSceneCall(0, renderContext);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, renderContext);
    Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_WRITE, 0, renderContext);
    for (i = 0; i < 8; i++) {
        if (((CondorRenderObject *)&g_CondorRenderObjectsTail)[i].renderObject != 0) {
            Gfx_EmitPrimitiveBatchList((void *)((CondorRenderObject *)&g_CondorRenderObjects)[i].renderObject, renderContext);
        }
    }
    Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_WRITE, 1, renderContext);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 1, renderContext);
    Mini_CondorDrawActiveUnits(renderContext, g_CondorSubViewIndex);
    g_CondorPeakValue = 0;
    Mini_CondorDrawPhaseSevenUnitModels(renderContext, g_CondorSubViewIndex);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, renderContext);
    for (i = 0; i < 13; i++) {
        if (((CondorRenderObjectSlot3 *)&g_CondorRenderObjectSlotsTail)[i].mode == 0 && ((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[i].drawn != 0 && ((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[i].renderObject != 0) {
            Gfx_DrawRenderObjectCall((GfxRenderObject *)(((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[i].renderObject), renderContext);
        }
    }
    for (i = 0; i < 7; i++) {
        if (g_CondorUnitModelTable[i].ready != 0 && g_CondorUnitModelTable[i].drawPass == 0) {
            for (j = 0; j < 4; j++) {
                if (g_CondorUnitModelTable[i].parts[j].batched != 0) {
                    Gfx_DrawRenderObjectCall((GfxRenderObject *)(g_CondorUnitModelTable[i].parts[j].renderObject), renderContext);
                }
            }
        }
    }
    Gfx_SetScenePassCall(1, renderContext);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, renderContext);
    for (i = 0; i < 13; i++) {
        if (((CondorRenderObjectSlot3 *)&g_CondorRenderObjectSlotsTail)[i].mode == 1 && ((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[i].drawn != 0 && ((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[i].renderObject != 0) {
            Gfx_DrawRenderObjectCall((GfxRenderObject *)(((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[i].renderObject), renderContext);
        }
    }
    for (i = 0; i < 7; i++) {
        if (g_CondorUnitModelTable[i].ready != 0 && g_CondorUnitModelTable[i].drawPass == 1) {
            for (j = 0; j < 4; j++) {
                if (g_CondorUnitModelTable[i].parts[j].batched != 0) {
                    Gfx_DrawRenderObjectCall((GfxRenderObject *)(g_CondorUnitModelTable[i].parts[j].renderObject), renderContext);
                }
            }
        }
    }
    Gfx_EndSceneCall(renderContext);
    ReadTimestampCounter(&endTime);
    if (renderContext->statsRecord != 0) {
        Sub64(&endTime, &beginTime, &renderContext->statsRecord->timeA);
    }
}
} /* extern "C" */
