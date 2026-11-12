// src/ff7/field/src/field_engine_model_anim.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Holds the thin forwarders to the engine context create, init, destroy and quit calls,
//   plus the frame throttle and render loop. It also has helpers that place and scale models, drive
//   animation players, load skeletons and transform matrices.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "lib/types/mem.h"
#include "lib/codegen.h"
#include "lib/pfile.h"
#include "ff7/api/main.h"
#include "lib/api/mem.h"
#include "lib/api/movie.h"
#include "ff7/types/field.h"
#include "lib/types/file.h"
#include "lib/types/polygon.h"
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include "ff7/api/field.h"
#include "ff7/api/menu.h"
#include "ff7/api/movie.h"
#include "lib/api/graphics.h"
#include "lib/api/polygon.h"
#include <stdlib.h>
#include <time.h>
#include "lib/crt.h"
#include "ff7/gte.h"
#include "ff7/field.h"
extern "C" {

struct GfxEngineContext;
struct PolygonPlacement;
struct FileOpenRequest;

// clang-format off
// GLOBAL: FF7 0x009069c4
float g_FieldTileScaleFactor = 1.40625f;
// GLOBAL: FF7 0x00cff8e0
Timestamp64 g_FieldTimestampDelta = {0};
// GLOBAL: FF7 0x00cff8e8
int g_FieldModelOffsetZ[18] = {0};
// GLOBAL: FF7 0x00cff930
int g_FieldModelOffsetY[18] = {0};
unsigned char g_bssPad_00cff978[8] = {0};
// GLOBAL: FF7 0x00cff980
struct GfxFloat4 g_FieldModelContextMatrix[4] = {0};
// GLOBAL: FF7 0x00cff9c0
struct GfxTransposedMatrix g_FieldTransposedMatrix = {0};
// g_FieldModelSortBuffer is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0xcffa00..0xcffa48 are aliases.
// GLOBAL: FF7 0x00cffa00
unsigned char g_FieldModelSortBuffer[0x48] = {0};
// GLOBAL: FF7 0x00cffa48
int g_FieldModelVisible[18] = {0};
// GLOBAL: FF7 0x00cffa90
int g_FieldMovieCameraActive = 0;
// GLOBAL: FF7 0x00cffa94
int g_FieldFrameCounter = 0;
// GLOBAL: FF7 0x00cffa98
int g_FieldRenderActive = 0;
unsigned char g_bssPad_00cffa9c[4] = {0};
// GLOBAL: FF7 0x00cffaa0
double g_FieldRenderTimeAccum = 0.0;
// GLOBAL: FF7 0x00cffaa8
int g_FieldRenderStateMode = 0;
// GLOBAL: FF7 0x00cffaac
char *g_pFieldTutorialTextBuffer = 0;
// clang-format on

#define g_FieldCameraTranslation (*(int(*)[3])((char *)&g_FieldCameraTransform + 0x12))

/* Unprototyped retail call signature (parameter/return width differs from callee definition). */
extern int __cdecl Field_BltTileSpriteRange(int startIndex, short endIndex, short centerX, short centerY, int normalScale, short threshold);

/*
 * One-line forwarder: passes context to SetEngineContext in directx.cpp and drops the return value.
 */
// FUNCTION: FF7 0x006384b0
void __cdecl Field_SetEngineContextThunk(int context)
{
    SetEngineContext(context);
}

/*
 * Ten-byte thunk that calls GetEngineContext and discards the result; the assumed void signature
 * means whatever the original returned is not visible here.
 */
// FUNCTION: FF7 0x006384c1
void __cdecl Field_GetEngineContextThunk(void)
{
    GetEngineContext();
}

/*
 * One-line forwarder: passes exitCode to RequestEngineQuit in directx.cpp and drops the return value.
 */
// FUNCTION: FF7 0x006384cb
void __cdecl Field_RequestEngineQuitThunk(GfxEngineContext *exitCode)
{
    RequestEngineQuit((int)exitCode);
}

/*
 * Ten-byte thunk that calls DestroyEngineContext and discards the result.
 */
// FUNCTION: FF7 0x006384dc
void __cdecl Field_DestroyEngineContextThunk(void)
{
    DestroyEngineContext();
}

/*
 * Returns after clearing compensation flag when movie predicate is zero and g_MovieFlag is zero.
 * Otherwise measures timestamp delta; without compensation busy-waits up to target or records
 * overshoot. With compensation waits to target minus overshoot and clears compensation. Arg1 is
 * unused; units and calibration require other evidence.
 */
// FUNCTION: FF7 0x006384e6
void __cdecl Field_ThrottleFrameRate(GfxEngineContext *unused)
{
    double elapsed;
    int diff64[2];
    elapsed = 0;
    if (Movie_IsNotPlaying() == 0 && g_MovieFlag == 0) {
        g_FieldRenderActive = 0;
        return;
    }
    ReadTimestampCounter(&g_FieldTimestampDelta);
    Sub64(&g_FieldTimestampDelta, &g_FieldTimestampCounter, (struct Timestamp64 *)diff64);
    elapsed = CombineSplitValueToFloat((struct Timestamp64 *)diff64);
    if (g_FieldRenderActive == 0) {
        if (elapsed <= g_FieldFrameTimeDelta) {
            while (elapsed < g_FieldFrameTimeDelta) {
                ReadTimestampCounter(&g_FieldTimestampDelta);
                Sub64(&g_FieldTimestampDelta, &g_FieldTimestampCounter, (struct Timestamp64 *)diff64);
                elapsed = CombineSplitValueToFloat((struct Timestamp64 *)diff64);
            }
            g_FieldRenderActive = 0;
            g_FieldRenderTimeAccum = 0;
        } else {
            g_FieldRenderTimeAccum = elapsed - g_FieldFrameTimeDelta;
            g_FieldRenderActive = 1;
        }
    } else {
        while (g_FieldFrameTimeDelta - g_FieldRenderTimeAccum > elapsed) {
            ReadTimestampCounter(&g_FieldTimestampDelta);
            Sub64(&g_FieldTimestampDelta, &g_FieldTimestampCounter, (struct Timestamp64 *)diff64);
            elapsed = CombineSplitValueToFloat((struct Timestamp64 *)diff64);
        }
        g_FieldRenderActive = 0;
        g_FieldRenderTimeAccum = 0;
    }
}

/*
 * Measures timestamp delta unless the movie/global early-return condition clears compensation.
 * Without compensation waits to target or records overshoot. With compensation and
 * overshoot-2000000<=target waits to target minus overshoot and clears it; otherwise accumulates
 * elapsed minus target and retains compensation. Arg1 is unused.
 */
// FUNCTION: FF7 0x00638655
void __cdecl Field_ThrottleFrameRateCapped(GfxEngineContext *unused)
{
    double elapsed;
    int diff64[2];
    elapsed = 0;
    if (Movie_IsNotPlaying() == 0 && g_MovieFlag == 0) {
        g_FieldRenderActive = 0;
        return;
    }
    ReadTimestampCounter(&g_FieldTimestampDelta);
    Sub64(&g_FieldTimestampDelta, &g_FieldTimestampCounter, (struct Timestamp64 *)diff64);
    elapsed = CombineSplitValueToFloat((struct Timestamp64 *)diff64);
    if (g_FieldRenderActive == 0) {
        if (elapsed <= g_FieldFrameTimeDelta) {
            while (elapsed < g_FieldFrameTimeDelta) {
                ReadTimestampCounter(&g_FieldTimestampDelta);
                Sub64(&g_FieldTimestampDelta, &g_FieldTimestampCounter, (struct Timestamp64 *)diff64);
                elapsed = CombineSplitValueToFloat((struct Timestamp64 *)diff64);
            }
            g_FieldRenderActive = 0;
            g_FieldRenderTimeAccum = 0;
        } else {
            g_FieldRenderTimeAccum = elapsed - g_FieldFrameTimeDelta;
            g_FieldRenderActive = 1;
        }
    } else {
        if (g_FieldRenderTimeAccum - 2000000.0 <= g_FieldFrameTimeDelta) {
            while (g_FieldFrameTimeDelta - g_FieldRenderTimeAccum > elapsed) {
                ReadTimestampCounter(&g_FieldTimestampDelta);
                Sub64(&g_FieldTimestampDelta, &g_FieldTimestampCounter, (struct Timestamp64 *)diff64);
                elapsed = CombineSplitValueToFloat((struct Timestamp64 *)diff64);
            }
            g_FieldRenderActive = 0;
            g_FieldRenderTimeAccum = 0;
        } else {
            g_FieldRenderTimeAccum = g_FieldRenderTimeAccum + elapsed - g_FieldFrameTimeDelta;
            g_FieldRenderActive = 1;
        }
    }
}

/*
 * Clears g_FieldRenderActive, then calls Field_RenderModels(context), Input_ClearButtonLatches(), Field_ReleaseBatchesAndResetDrawPools(context) --
 * the pool reset below -- and Field_ResetAdListBankCounts(), in that order. context is passed through unchanged to
 * the two callees that take it.
 */
// FUNCTION: FF7 0x00638801
void __cdecl Field_RenderFrameAndResetPools(GfxEngineContext *context)
{
    g_FieldRenderActive = 0;
    Field_RenderModels(context);
    Input_ClearButtonLatches();
    Field_ReleaseBatchesAndResetDrawPools(context);
    Field_ResetAdListBankCounts();
}

/*
 * Renders models then, while compensation flag remains set, runs capped throttle and three state
 * helpers, increments context frame double, returns on failed Field_ModuleCatchUpFrame or global exit flag,
 * otherwise renders again. After loop runs throttle and the three helpers once more.
 */
// FUNCTION: FF7 0x00638832
void __cdecl Field_RenderFrameUnused(GfxEngineContext *context)
{
    int unusedValue; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unused;

    unused = 0;
    Field_RenderModels(context);
    if (g_FieldRenderActive == 0) {
        g_FieldRenderActive = 0;
    } else {
        while (g_FieldRenderActive != 0) {
            Field_ThrottleFrameRateCapped(context);
            Input_ClearButtonLatches();
            Field_ReleaseBatchesAndResetDrawPools(context);
            Field_ResetAdListBankCounts();
            context->framesThisSecond += 1.0;
            if (Field_ModuleCatchUpFrame(context) == 0) {
                return;
            }
            if (g_FieldMapChangePending != 0) {
                return;
            }
            Field_RenderModels(context);
        }
    }
    Field_ThrottleFrameRateCapped(context);
    Input_ClearButtonLatches();
    Field_ReleaseBatchesAndResetDrawPools(context);
    Field_ResetAdListBankCounts();
}

/*
 * Renders models. With global mode zero calls Field_RenderFrameNoOp directly; otherwise when compensation
 * is set throttles, clears it, runs three helpers, increments context frame double and exits on
 * failed continuation or global flag before another render and Field_RenderFrameNoOp. The mode path then
 * throttles, and all completing paths run the three helpers.
 */
// FUNCTION: FF7 0x006388ee
void __cdecl Field_RenderFrame(GfxEngineContext *context)
{
    int unusedValue; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unused;

    unused = 0;
    Field_RenderModels(context);
    if (g_FieldEnableRendering == 0) {
        Field_RenderFrameNoOp(context);
    } else {
        if (g_FieldRenderActive == 0) {
            Field_RenderFrameNoOp(context);
            g_FieldRenderActive = 0;
        } else {
            Field_ThrottleFrameRate(context);
            g_FieldRenderActive = 0;
            Input_ClearButtonLatches();
            Field_ReleaseBatchesAndResetDrawPools(context);
            Field_ResetAdListBankCounts();
            context->framesThisSecond += 1.0;
            if (Field_ModuleCatchUpFrame(context) == 0) {
                return;
            }
            if (g_FieldMapChangePending != 0) {
                return;
            }
            Field_RenderModels(context);
            Field_RenderFrameNoOp(context);
        }
        Field_ThrottleFrameRate(context);
    }
    Input_ClearButtonLatches();
    Field_ReleaseBatchesAndResetDrawPools(context);
    Field_ResetAdListBankCounts();
}

/*
 * Seeds the CRT random generator from the engine context's randomSeed, or from time() when
 * that is zero. One of three identical copies (Mini_CondorSeedRandomFromContext (0x005F5153) and
 * Mini_ChocoboSeedRandomFromContext are the others); the address suffix keeps the label unique.
 */
// FUNCTION: FF7 0x006389db
void __cdecl Field_SeedRandomFromContext(struct GfxEngineContext *context)
{
    long seed;

    if (context->randomSeed != 0) {
        srand(context->randomSeed);
    } else {
        time(&seed);
        srand(seed);
    }
}

/*
 * Ten-byte thunk that calls CreateEngineContext and discards the result.
 */
// FUNCTION: FF7 0x00638a1b
void __cdecl Field_CreateEngineContextThunk(void)
{
    CreateEngineContext();
}

/*
 * Two-argument forwarder to Field_InitEngineContext(context, config); it does nothing else.
 */
// FUNCTION: FF7 0x00638a25
void __cdecl Field_InitEngineContextThunk(int context, int config)
{
    Field_InitEngineContext(context, config);
}

/*
 * Scales three integer components by float scale, calls transform helper through separately
 * declared float local and int output storage. Nonzero applyMatrix applies output helper to
 * context; otherwise nonnull placement adds three scaled locals to position fields. Always returns
 * one.
 */
// FUNCTION: FF7 0x00638a3a
int __cdecl Field_ApplyScaledOffsetToPlacement(struct GfxEngineContext *context, FieldPlacement *placement, int offsetX, int offsetY, int offsetZ, int applyMatrix, float scale)
{
    GfxHomogeneousPoint scaledX;
    int matrixOut[16];

    scaledX.x = offsetX * scale;
    scaledX.y = offsetY * scale;
    scaledX.z = offsetZ * scale;
    BuildTranslationMatrixRowVec(&scaledX.x, matrixOut);
    if (applyMatrix != 0) {
        Gfx_MultiplyContextMatrix(matrixOut, context);
    } else if (placement != 0) {
        placement->posX += scaledX.x;
        placement->posY += scaledX.y;
        placement->posZ += scaledX.z;
    }
    return 1;
}

/*
 * Forwards to AnimPlacement_SetScale(scale, placement), which stores scale as the placement's uniform scale and
 * clears its flag 0x40 when the value is 1.0f, setting it otherwise. Returns 1 unconditionally.
 */
// FUNCTION: FF7 0x00638abf
int __cdecl Field_SetModelUniformScale(PolygonPlacement *placement, int scale)
{
    AnimPlacement_SetScale(scale, placement);
    return 1;
}

/*
 * Subtracts 360 once from angleDegrees when at least 360, calls AnimPlacement_SetAngleY through a float/int cdecl
 * signature with the result and placement, then returns one.
 */
// FUNCTION: FF7 0x00638ad9
int __cdecl Field_SetPlacementAngleYWrapped(int placement, float angleDegrees)
{
    float angle;

    angle = 0.0f;
    angle = angleDegrees;
    if (angle >= 3.6e+02f) {
        angle -= 3.6e+02f;
    }
    /* Retail caller reads residual EAX/AX as `int` after `void AnimPlacement_SetAngleY` returns. */
    ((int(__cdecl *)(float, int))AnimPlacement_SetAngleY)(angle, placement);
    return 1;
}

/*
 * Stores pauseFlag into the animation player's isPlaying field when animPlayer is non-null, and
 * returns 1 either way.
 */
// FUNCTION: FF7 0x00638b1f
int __cdecl Field_SetAnimPlayerPauseFlag(PolygonAnimPlayer *animPlayer, int pauseFlag)
{
    if (animPlayer != 0) {
        animPlayer->isPlaying = pauseFlag;
    }
    return 1;
}

/*
 * Stores the low 16 bits of frameIndex as the player's currentFrame, but only when animPlayer is
 * non-null and that value is below its frameCount. Returns 1 whether or not the store happened.
 */
// FUNCTION: FF7 0x00638b38
int __cdecl Field_SetAnimPlayerFrame(FieldAnimPlayer *animPlayer, int frameIndex)
{
    if (animPlayer != 0 && (frameIndex & 0xFFFF) < animPlayer->frameCount) {
        animPlayer->currentFrame = frameIndex & 0xFFFF;
    }
    return 1;
}

/*
 * Passes Gfx_SetContextMatrix the address of its second argument and the value of its first, and returns
 * 1.
 */
// FUNCTION: FF7 0x00638b67
int __cdecl Field_SetContextMatrixFromStackArgs(GfxEngineContext *context, int matrix)
{
    Gfx_SetContextMatrix(&matrix, context);
    return 1;
}

/*
 * Returns 0 when player is null; otherwise calls AnimPlayer_SelectAnimation(slot, 0, animIndex, player) and
 * returns 1.
 */
// FUNCTION: FF7 0x00638b81
int __cdecl Field_SelectAnimPlayerAnimationIndex(AnimPlayerInstance *player, int animIndex, int slot)
{
    if (player == 0) {
        return 0;
    } else {
        AnimPlayer_SelectAnimation(slot, 0, animIndex, player);
        return 1;
    }
}

// Points a model at one of its animations. Returns 0 when model is null or when animIndex is not
// below the unsigned short animCount. Otherwise it takes the animIndex-th int of animTable, binds
// it to animPlayer through Field_BindAnimPlayerAnimationThunk, records animIndex as a short in the
// model, and returns 1.
// FUNCTION: FF7 0x00638bab
int __cdecl Field_SetModelAnimation(FieldModel *model, int animIndex)
{
    if (model == 0) {
        return 0;
    } else if (animIndex >= model->animCount) {
        return 0;
    } else {
        Field_BindAnimPlayerAnimationThunk(0, *(int *)((char *)model->animTable + animIndex * 4), (PolygonAnimPlayer *)(model->animPlayer));
        model->animIndex = animIndex;
        return 1;
    }
}

/*
 * Calls Field_InvalidateTexturePageBatches, then, if g_FieldHandImageType is set, retires the object in g_pFieldHandImageBuffer with
 * InvalidateBatchGeneration and Gfx_EmptyRenderObjectBatch and clears g_FieldHandImageType; finally resets both pools
 * for pool with ResetFrameDrawPool and ResetRenderPools.
 */
// FUNCTION: FF7 0x00638c03
void __cdecl Field_ReleaseBatchesAndResetDrawPools(GfxEngineContext *pool)
{
    Field_InvalidateTexturePageBatches();
    if (g_FieldHandImageType != 0) {
        InvalidateBatchGeneration((void *)(*(int *)&g_pFieldHandImageBuffer));
        Gfx_EmptyRenderObjectBatch(*(int *)&g_pFieldHandImageBuffer);
        g_FieldHandImageType = 0;
    }
    ResetFrameDrawPool(pool);
    ResetRenderPools(pool);
}

/*
 * Returns zero for null entityLoad. Otherwise if its model is absent initializes load
 * options/blend, builds skeleton and player and stores a nonnull result. Returns one for every
 * nonnull entityLoad even if loading fails.
 */
// FUNCTION: FF7 0x00638c55
int __cdecl Field_LoadEntitySkeletonIfAbsent(struct GfxEngineContext *context, PolygonSkeletonLoadSpec *entityLoad, int blendMode)
{
    struct {
        PfileLoadOptions *opts;
        int resource, unused, model;
        PfileLoadOptions options;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (entityLoad == 0) {
        return 0;
    } else {
        frame.model = entityLoad->animPlayer;
        if (frame.model == 0) {
            frame.opts = &frame.options;
            Pfile_InitLoadOptions(frame.opts);
            Pfile_SetLoadOptionsBlendMode(blendMode, frame.opts);
            frame.unused = 0;
            frame.resource = entityLoad->resource;
            frame.model = Skel_LoadHrcAndCreatePlayer(frame.resource, frame.unused, entityLoad, (int)frame.opts, context);
            if (frame.model != 0) {
                entityLoad->animPlayer = frame.model;
            }
        }
        return 1;
    }
}

/*
 * Loads a skeleton for the name name: Skel_LoadHrcFromDisk(0, name) when g_FieldLoadAssets is zero,
 * otherwise Skel_LoadHrc(loader+0x40, 0, name) -- the same g_FieldLoadAssets branch ad_app.cpp uses to pick
 * between the two load paths. On success it calls Skel_BuildHierarchyAndLoadParts(context, skeleton,
 * loader, options) and Skel_SetPartsVisible(1, skeleton). Returns the skeleton pointer, or 0.
 */
// FUNCTION: FF7 0x00638cef
int __cdecl Field_LoadSkeleton(int name, int context, int loader, struct GfxEngineContext *engine)
{
    PolygonSkelHierarchy *pSkeleton;
    int reserved;

    reserved = 0;
    if (g_FieldLoadAssets == 0) {
        pSkeleton = (PolygonSkelHierarchy *)Skel_LoadHrcFromDisk(reserved, name);
    } else {
        pSkeleton = (PolygonSkelHierarchy *)Skel_LoadHrc(loader + 0x40, reserved, name);
    }
    if (pSkeleton != 0) {
        Skel_BuildHierarchyAndLoadParts(context, pSkeleton, (struct PfileLoadOptions *)loader, (int)engine);
        Skel_SetPartsVisible(1, pSkeleton);
    }
    return (int)pSkeleton;
}

/*
 * Creates an animation player with AnimPlayer_Create(context, 0, skeleton, options) and, if that succeeded,
 * binds animation animIndex to it through Field_BindAnimPlayerAnimationThunk and clears the player's tickEnabled. Returns the player
 * or 0. Mini_CondorCreateAnimPlayerWithAnimation in the minigame code is the same body and differs only
 * in which forwarder it binds through.
 */
// FUNCTION: FF7 0x00638d67
int __cdecl Field_CreateAnimPlayerWithAnimation(int context, PolygonSkelHierarchy *skeleton, struct GfxEngineContext *engine, int animIndex)
{
    PolygonAnimPlayer *player;

    player = (PolygonAnimPlayer *)AnimPlayer_Create(context, 0, skeleton, (int)engine);
    if (player != 0) {
        Field_BindAnimPlayerAnimationThunk(0, animIndex, player);
        player->tickEnabled = 0;
    }
    return (int)player;
}

/*
 * Splits arg2 path, replaces extension with .a and concatenates drive, directory, basename,
 * extension into a256-byte path. If text compilation succeeds loads binary from disk or archive
 * context according to g_FieldLoadAssets. Returns pointer or zero.
 */
// FUNCTION: FF7 0x00638dad
#pragma intrinsic(strcpy, strcat)
int __cdecl Field_LoadAnimation(int loader, int fileName)
{
    struct {
        char prefix[4], part1[256], part2[256], part3[256], path[256];
        int result;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.result = 0;
    _splitpath((const char *)fileName, frame.prefix, frame.part2, frame.part1, frame.part3);
    // STRING: FF7 0x009069c8
    strcpy(frame.part3, UNFOLDED(".a"));
    strcpy(frame.path, frame.prefix);
    strcat(frame.path, frame.part2);
    strcat(frame.path, frame.part1);
    strcat(frame.path, frame.part3);
    if (Anim_CompileTextToBinary(fileName, (int)frame.path) != 0) {
        if (g_FieldLoadAssets == 0) {
            frame.result = ((int (__cdecl *)(int openFlags))Anim_LoadBinaryFromDisk)((int)frame.path); /* reads eax after a void call, as the original does */
        } else {
            frame.result = Anim_LoadBinary((FileOpenRequest *)(loader + 0x40), (int)frame.path);
        }
    }
    return frame.result;
}
#pragma function(strcpy, strcat)

/*
 * Thin forwarder: passes mode, animIndex and player straight to AnimPlayer_BindAnimation in plytopd.c.
 */
// FUNCTION: FF7 0x00638f2f
void __cdecl Field_BindAnimPlayerAnimationThunk(int mode, int animIndex, PolygonAnimPlayer *player)
{
    AnimPlayer_BindAnimation(mode, animIndex, player);
}

/*
 * Loads a skeleton with Field_LoadSkeleton(name, skeletonOwner, loader, options) and, only if that succeeded,
 * creates the animation player with Field_CreateAnimPlayerWithAnimation(context, skeleton, options, animIndex). Returns the player,
 * or 0 if either step failed.
 */
// FUNCTION: FF7 0x00638f48
int __cdecl Field_LoadSkeletonAndCreateAnimPlayer(int context, int animIndex, int loader, struct GfxEngineContext *engine, int name, int skeletonOwner)
{
    PolygonAnimPlayer *player;
    PolygonSkelHierarchy *skeleton;

    player = 0;
    skeleton = (PolygonSkelHierarchy *)Field_LoadSkeleton(name, skeletonOwner, loader, engine);
    if (skeleton != 0) {
        player = (PolygonAnimPlayer *)Field_CreateAnimPlayerWithAnimation(context, skeleton, engine, animIndex);
    }
    return (int)player;
}

// Rewrites the 4x4 matrix in place. It swaps rows 1 and 2 entirely, m10 through m13 with m20
// through m23, so the fourth column (m13 and m23) moves with them. It then swaps columns 1 and 2 in
// every row: m01 with m02, m11 with m12, m21 with m22 and m31 with m32. Together that conjugates
// the transform by a swap of the second and third axes. The fourth column is only ever moved by the
// row swap.
// FUNCTION: FF7 0x00638f98
void __cdecl Field_SwapMatrixYZAxes(FieldMatrix4x4 *matrix)
{
    int temp;

    temp = matrix->m10;
    matrix->m10 = matrix->m20;
    matrix->m20 = temp;
    temp = matrix->m11;
    matrix->m11 = matrix->m21;
    matrix->m21 = temp;
    temp = matrix->m12;
    matrix->m12 = matrix->m22;
    matrix->m22 = temp;
    temp = matrix->m13;
    matrix->m13 = matrix->m23;
    matrix->m23 = temp;
    temp = matrix->m01;
    matrix->m01 = matrix->m02;
    matrix->m02 = temp;
    temp = matrix->m11;
    matrix->m11 = matrix->m12;
    matrix->m12 = temp;
    temp = matrix->m21;
    matrix->m21 = matrix->m22;
    matrix->m22 = temp;
    temp = matrix->m31;
    matrix->m31 = matrix->m32;
    matrix->m32 = temp;
}

/*
 * One-line forwarder: calls GtePsxMatrixToFloat4x4ColVec(psxMatrix, dst) and does nothing else.
 */
// FUNCTION: FF7 0x00639090
void __cdecl Field_PsxMatrixToFloat4x4Thunk(int psxMatrix, GfxFloat4 *dst)
{
    GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)psxMatrix, dst);
}

/*
 * Copies x, y and z into three consecutive stack ints and calls TransformPointHomogeneous(matrix,
 * &triple, out), so the three scalars reach the callee as one vector passed by address. Nothing
 * else is read or written.
 */
// FUNCTION: FF7 0x006390a5
void __cdecl Field_TransformPointByMatrix(struct GfxMatrix4x4 *matrix, int x, int y, int z, struct GfxHomogeneousVec4 *out)
{
    GfxVec3Dual vec;

    vec.xBits = x;
    vec.yBits = y;
    vec.zBits = z;
    TransformPointHomogeneous(matrix, (struct GfxHomogeneousPoint *)&vec, out);
}

/*
 * Fills the 4x4 float matrix at matrix with SetIdentityMatrix4x4, then overwrites the words at 0x14
 * with 0, 0x18 with 0x3F800000 (1.0f), 0x24 with 0xBF800000 (-1.0f) and 0x28 with 0. That leaves a
 * quarter-turn about the X axis, which exchanges the Y and Z rows.
 */
// FUNCTION: FF7 0x006390d5
void __cdecl Field_BuildQuarterTurnAboutXMatrix(FieldRotationMatrix *matrix)
{
    SetIdentityMatrix4x4(matrix);
    matrix->m11 = 0;
    matrix->m12 = FLOAT_ONE_BITS;
    matrix->m21 = FLOAT_MINUS_ONE_BITS;
    matrix->m22 = 0;
}

/*
 * Builds a model's transform into the 4x4 at dst: Field_PsxMatrixToFloat4x4Thunk fills one matrix from psxMatrix,
 * Field_BuildQuarterTurnAboutXMatrix fills another, MultiplyMatrix4x4Full multiplies them into dst, and
 * ClearMatrixTranslationAndW drops the translation and w.
 */
// FUNCTION: FF7 0x0063910e
void __cdecl Field_BuildAxisSwappedRotationMatrix(int psxMatrix, void *dst)
{
    float floatMtx[4][4];
    float matB[4][4];

    Field_PsxMatrixToFloat4x4Thunk(psxMatrix, (GfxFloat4 *)floatMtx);
    Field_BuildQuarterTurnAboutXMatrix((FieldRotationMatrix *)matB);
    MultiplyMatrix4x4Full(floatMtx, matB, dst);
    ClearMatrixTranslationAndW(dst);
}

/*
 * Rebuilds the field's camera matrices. It makes a local identity 4x4, passes it and context to
 * Gfx_SetContextMatrix, converts the PSX rotation matrix g_FieldCameraTransform into the float matrix g_FieldModelContextMatrix
 * through Field_PsxMatrixToFloat4x4Thunk, calls Matrix4x4Invert(&g_FieldModelContextMatrix, local), writes the transpose of the
 * local matrix into g_FieldTransposedMatrix with TransposeMatrix4x4, strips that copy's translation and W
 * with ClearMatrixTranslationAndW, and finally sets g_FieldModelRenderState to the axis-swap matrix built by
 * Field_BuildQuarterTurnAboutXMatrix. The frame reserves further slots the original used and this body does not read.
 */
// FUNCTION: FF7 0x00639157
void __cdecl Field_RebuildCameraMatrices(GfxEngineContext *context)
{
    int inverse[16];

    SetIdentityMatrix4x4(inverse);
    Gfx_SetContextMatrix(inverse, context);
    Field_PsxMatrixToFloat4x4Thunk((int)&g_FieldCameraTransform, (GfxFloat4 *)&g_FieldModelContextMatrix);
    Matrix4x4Invert((int)&g_FieldModelContextMatrix, inverse);
    TransposeMatrix4x4((struct GfxTransposedMatrix *)inverse, (struct GfxTransposedMatrix *)&g_FieldTransposedMatrix);
    ClearMatrixTranslationAndW(&g_FieldTransposedMatrix);
    Field_BuildQuarterTurnAboutXMatrix((FieldRotationMatrix *)&g_FieldModelRenderState);
}

/*
 * Sorts the active field model records by depth.
 */
// FUNCTION: FF7 0x006391cb
void __cdecl Field_SortModelsByDepth(void)
{
    qsort((int *)&g_FieldModelSortRecords, g_FieldModelSortCount, 0x10, (int(__cdecl *)(const void *, const void *))Field_CompareModelDepthDescending);
}

/*
 * qsort comparator on the unsigned depth of two sort records, sorting descending: -1 when the
 * first is greater, 0 when they are equal, 1 otherwise.
 */
// FUNCTION: FF7 0x006391ed
int __cdecl Field_CompareModelDepthDescending(FieldModelSortRecord *lhs, FieldModelSortRecord *rhs)
{
    if ((unsigned short)lhs->depth > (unsigned short)rhs->depth) {
        return -1;
    }
    if ((unsigned short)lhs->depth == (unsigned short)rhs->depth) {
        return 0;
    }
    return 1;
}

// Stores x, y and z into transformedX, transformedY and transformedZ when instance is non-null.
// Nothing is read back and there is no return value.
// FUNCTION: FF7 0x0063922c
void __cdecl Field_SetModelInstanceTransformedPosition(FieldModelInstance *instance, int x, int y, int z)
{
    if (instance != 0) {
        instance->transformedX = x;
        instance->transformedY = y;
        instance->transformedZ = z;
    }
}

/*
 * Returns one when the override global is nonzero or when short-narrowed coordinates fall strictly
 * inside the configured origin-relative rectangle x(-40, +400), y(-120, +460); otherwise zero.
 */
// FUNCTION: FF7 0x00639252
int __cdecl Field_IsOnScreen(int x, int y)
{
    if (g_FieldBgFlag != 0) {
        return 1;
    } else if ((short)y > g_FieldDisplayHeight - 0x78 && (short)y < g_FieldDisplayHeight + 0x1CC && (short)x > g_FieldDisplayOffsetX - 0x28 && (short)x < g_FieldDisplayOffsetX + 0x190) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Updates model instances from fixed-point entity positions and interpolation offsets, projects
 * visible enabled models, collects depth-sort entries and updates skeleton visibility, animation
 * frame, color flags and transformation matrices. Hidden or offscreen models clear their
 * active-render flag. A second pass advances pending entity/model type transitions using the
 * unoffset projected position, and a third dispatches alternating model actions with a random
 * 64-to-95 tick countdown for active eligible entities. Stores the visible count and sorts models
 * by depth; a null model-array pointer skips all work. Model subobject pointers are dereferenced
 * before later null checks.
 */
// FUNCTION: FF7 0x006392bb
void __cdecl Field_UpdateModelInstances(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[268];
        Timestamp64 timestamp;
        struct { unsigned char pad_to_8[0x8]; int worldX; };
        struct { unsigned char pad_to_A[0xA]; short worldY; };
        struct { unsigned char pad_to_C[0xC]; int worldZ; };
        struct { unsigned char pad_to_10[0x10]; int sortCount; };
        struct { unsigned char pad_to_14[0x14]; int entityIndex; };
        struct { unsigned char pad_to_18[0x18]; GfxMatrix4x4 rotZMatrix; };
        struct { unsigned char pad_to_58[0x58]; int dispatchIndex; };
        struct { unsigned char pad_to_5C[0x5C]; GfxMatrix4x4 contextRotMatrix; };
        struct { unsigned char pad_to_9C[0x9C]; char nextPreset; };
        struct { unsigned char pad_to_A4[0xA4]; GfxMatrix4x4 modelMatrix; };
        struct { unsigned char pad_to_E4[0xE4]; FieldCameraCoordsRect screenX; };
        struct { unsigned char pad_to_EC[0xEC]; int transformedX; };
        struct { unsigned char pad_to_F0[0xF0]; int transformedY; };
        struct { unsigned char pad_to_F4[0xF4]; int transformedZ; };
        struct { unsigned char pad_to_FC[0xFC]; float rotAngle; };
        struct { unsigned char pad_to_100[0x100]; int modelState; };
        struct { unsigned char pad_to_104[0x104]; int depth; };
        struct { unsigned char pad_to_108[0x108]; FieldModelInstance *model; };
    } frame;
#pragma pack(pop)

    frame.depth = 0;
    frame.sortCount = 0;
    if (((int)g_pFieldModelSlots) != 0) {
        ReadTimestampCounter(&frame.timestamp);
        for (frame.entityIndex = 0; frame.entityIndex < g_FieldHumanAnimState.modelListCountCopy; frame.entityIndex++) {
            *(short *)&frame.worldX = (g_FieldEntityState[frame.entityIndex].posX >> 0xC) + g_FieldEntityState[frame.entityIndex].offsetX;
            frame.worldY = (g_FieldEntityState[frame.entityIndex].posY >> 0xC) + g_FieldEntityState[frame.entityIndex].offsetY + g_FieldModelOffsetY[frame.entityIndex];
            *(short *)&frame.worldZ = (g_FieldEntityState[frame.entityIndex].posZ >> 0xC) + g_FieldEntityState[frame.entityIndex].offsetZ + g_FieldModelOffsetZ[frame.entityIndex] + -0xA;
            g_pFieldModelSlots[frame.entityIndex].worldX = (int)*(short *)&frame.worldX;
            g_pFieldModelSlots[frame.entityIndex].worldY = (int)frame.worldY;
            g_pFieldModelSlots[frame.entityIndex].worldZ = (int)*(short *)&frame.worldZ;
            frame.model = (FieldModelInstance *)(g_pFieldModelSlots[frame.entityIndex].model);
            frame.modelState = frame.model->skeletonAnim;
            if (g_pFieldModelSlots[frame.entityIndex].entityVisible != 0 && g_FieldModelVisible[frame.entityIndex] != 0) {
                frame.depth = Field_ProjectWorldToScreen((PsxSVectorZ32 *)&frame.worldX, &frame.screenX);
                if (frame.depth < 0xF00 && Field_IsOnScreen(frame.screenX.x + g_FieldPaletteTargetOrgX + 0xA0, frame.screenX.y + g_FieldPaletteTargetOrgY + 0x70) != 0) {
                    g_FieldModelSortRecords[frame.sortCount].sortKey = frame.entityIndex;
                    g_FieldModelSortRecords[frame.sortCount].depth = *(short *)&frame.depth;
                    if (frame.model->enabled == 0) {
                        Skel_SetPartsVisible(1, (PolygonSkelHierarchy *)(frame.model->skeleton));
                    }
                    frame.model->enabled = 1;
                    if (g_FieldFxFadeColor != 0x80FFFFFF && g_FieldFxFadeColor != -1) {
                        g_FieldModelLightData[frame.entityIndex]->flags |= 4;
                        g_FieldModelLightData[frame.entityIndex]->fadeColor = g_FieldFxFadeColor;
                    } else {
                        g_FieldModelLightData[frame.entityIndex]->flags &= 0xFFFFFFFB;
                    }
                    if (g_pFieldModelSlots[frame.entityIndex].effectPreset == 1) {
                        g_FieldModelLightData[frame.entityIndex]->flags &= 0xFFFFFFFE;
                    }
                    if (frame.model != 0 && frame.model->enabled != 0) {
                        frame.model->isPlaying = 1;
                        frame.model->frameIndex = g_FieldEntityState[frame.entityIndex].animCurrentFrame >> 4;
                        g_pFieldModelSlots[frame.entityIndex].active = 1;
                        frame.model->holdLastFrame = 1;
                    }
                    frame.sortCount++;
                    g_pFieldModelSlots[frame.entityIndex].depth = *(short *)&frame.depth;
                    g_pFieldModelSlots[frame.entityIndex].direction = (short)g_FieldEntityState[frame.entityIndex].dir;
                    /* Retail caller prototype (int, float, float, float, void *) -> void preserved for /Od argument promotion on `Field_TransformPointByMatrix`. */
                    ((void(__cdecl *)(int, float, float, float, void *))Field_TransformPointByMatrix)((int)&g_FieldModelContextMatrix, (float)*(short *)&frame.worldX, (float)frame.worldY, (float)*(short *)&frame.worldZ, &frame.transformedX);
                    Field_SetModelInstanceTransformedPosition(frame.model, frame.transformedX, frame.transformedY, frame.transformedZ);
                    frame.rotAngle = (float)*(unsigned short *)&g_pFieldModelSlots[frame.entityIndex].direction * g_FieldTileScaleFactor;
                    BuildRotationZColVec(frame.rotAngle, &frame.rotZMatrix);
                    MultiplyMatrix4x4Core(&g_FieldModelContextMatrix, &frame.rotZMatrix, &frame.contextRotMatrix);
                    MultiplyMatrix4x4Core(&frame.contextRotMatrix, &g_FieldModelRenderState, &frame.modelMatrix);
                    ClearMatrixTranslationAndW((char *)frame.model + 0x64);
                    TransposeMatrix3x3(&frame.modelMatrix, (char *)frame.model + 0x64);
                } else {
                    if (frame.model->enabled == 1) {
                        Skel_SetPartsVisible(0, (PolygonSkelHierarchy *)(frame.model->skeleton));
                    }
                    frame.model->enabled = 0;
                    g_pFieldModelSlots[frame.entityIndex].active = 0;
                }
            } else {
                if (frame.model->enabled == 1) {
                    Skel_SetPartsVisible(0, (PolygonSkelHierarchy *)(frame.model->skeleton));
                }
                frame.model->enabled = 0;
                g_pFieldModelSlots[frame.entityIndex].active = 0;
            }
        }
        for (frame.dispatchIndex = 0; frame.dispatchIndex < g_FieldHumanAnimState.modelListCountCopy; frame.dispatchIndex++) {
            if (g_FieldEntityState[frame.dispatchIndex].kawaiOp1 != 1) {
                continue;
            }
            *(short *)&frame.worldX = g_FieldEntityState[frame.dispatchIndex].posX >> 0xC;
            frame.worldY = g_FieldEntityState[frame.dispatchIndex].posY >> 0xC;
            *(short *)&frame.worldZ = (g_FieldEntityState[frame.dispatchIndex].posZ >> 0xC) - 0xA;
            frame.depth = Field_ProjectWorldToScreen((PsxSVectorZ32 *)&frame.worldX, &frame.screenX);
            if (frame.depth < 0xF00) {
                if (g_pFieldModelSlots[frame.dispatchIndex].effectPreset == 2 || g_pFieldModelSlots[frame.dispatchIndex].effectPreset == 6) {
                    g_FieldModelLightData[frame.dispatchIndex]->flags &= 0xFFFFFFFE;
                }
                Field_SelectRenderStateModeByMap(g_pFieldModelSlots[frame.dispatchIndex].effectPreset, (FieldEffectPosition *)&frame.screenX.x);
                if (g_pFieldModelSlots[frame.dispatchIndex].effectPreset == 1) {
                    ((int *)&g_FieldModelSortBuffer)[frame.dispatchIndex] = *(unsigned char *)((char *)*(int *)&g_FieldEntityState[frame.dispatchIndex].kawaiDataOffset);
                }
                if (Field_ExecuteModelCommand(g_pFieldModelSlots[frame.dispatchIndex].effectPreset, (FieldModelInstance *)(g_pFieldModelSlots[frame.dispatchIndex].model), (FieldModelCommandRecord *)(*(int *)&g_FieldEntityState[frame.dispatchIndex].kawaiDataOffset), frame.worldX, frame.worldZ, frame.dispatchIndex, (int *)&frame.nextPreset) == 1) {
                    g_FieldEntityState[frame.dispatchIndex].kawaiOp1 = 2;
                }
                g_pFieldModelSlots[frame.dispatchIndex].effectPreset = frame.nextPreset;
            }
        }
        for (frame.dispatchIndex = 0; frame.dispatchIndex < g_FieldHumanAnimState.modelListCountCopy; frame.dispatchIndex++) {
            if (*(char *)&g_FieldEntityState[frame.dispatchIndex].blinkOn == 1) {
                continue;
            }
            if (g_pFieldModelSlots[frame.dispatchIndex].active == 0) {
                continue;
            }
            if (*(short *)&g_FieldEntityState[frame.dispatchIndex].kawaiB == 0) {
                *(char *)&g_FieldHeadMeshCommands = 2;
                g_FieldHeadMeshCommands[1] = 2;
                g_FieldHeadMeshCommands[2] = 0;
                g_FieldHeadMeshCommands[3] = *(char *)&frame.dispatchIndex;
                *(short *)&g_FieldEntityState[frame.dispatchIndex].kawaiB = (unsigned short)(unsigned char)((g_FieldRandomTable[g_FieldBlinkRandomIndex] & 0x1F) + 0x40);
                (*(char *)&g_FieldBlinkRandomIndex)++;
            } else {
                *(char *)&g_FieldHeadMeshCommands = 1;
                g_FieldHeadMeshCommands[1] = 1;
                g_FieldHeadMeshCommands[2] = 0;
                g_FieldHeadMeshCommands[3] = *(char *)&frame.dispatchIndex;
                (*(short *)&g_FieldEntityState[frame.dispatchIndex].kawaiB)--;
            }
            Field_ApplyHeadMeshCommands((FieldActorRecord *)(((int)g_pFieldModelSlots) + frame.dispatchIndex * 0x190), (unsigned char *)&g_FieldHeadMeshCommands);
        }
        *(short *)&g_FieldModelSortCount = *(short *)&frame.sortCount;
        Field_SortModelsByDepth();
    }
}

/*
 * Returns one without state changes for mode other than eight/nine. Otherwise clears g_FieldRenderStateMode,
 * selects values 1..6 for six specific field identifiers and rejects three of them below their
 * short-coordinate thresholds; unlisted fields return zero.
 */
// FUNCTION: FF7 0x00639b85
int __cdecl Field_SelectRenderStateModeByMap(int mode, FieldEffectPosition *position)
{
    if (mode != 8 && mode != 9) {
        return 1;
    } else {
        g_FieldRenderStateMode = 0;
        if (g_FieldCurrentMapId == 0x58) {
            if (position->xOffset < 0x1E) {
                return 0;
            }
            g_FieldRenderStateMode = 2;
            return 1;
        } else if (g_FieldCurrentMapId == 0x1AD) {
            if (position->yOffset < -0x41) {
                return 0;
            }
            g_FieldRenderStateMode = 3;
            return 1;
        } else if (g_FieldCurrentMapId == 0x1AE) {
            g_FieldRenderStateMode = 4;
            return 1;
        } else if (g_FieldCurrentMapId == 0x1B3) {
            g_FieldRenderStateMode = 5;
            return 1;
        } else if (g_FieldCurrentMapId == 0x1C1) {
            if (position->yOffset < 0x10) {
                return 0;
            }
            g_FieldRenderStateMode = 1;
            return 1;
        } else if (g_FieldCurrentMapId == 0x281) {
            g_FieldRenderStateMode = 6;
            return 1;
        } else {
            return 0;
        }
    }
}

// Emits one textured quad into the batch at node, doing nothing if AcquireBatchNode(1, node) fails.
// When mode is not 2, metaId is stored both as a byte through outBytePtr and as the metaId field.
// Four 0x20-byte vertices are filled at quadBuffers, with corners (x,y), (x,y+height), (x+width,y)
// and (x+width,y+height), a shared z, rhw 1.0f, specular 0xFF000000, and texture coordinates
// (uOrigin,vOrigin) extended by uSpan in u and vSpan in v. The diffuse colour is 0x80FFFFFF when
// the int at +0x44 of the object that hundredRecord points to is zero, and 0xFFFFFFFF otherwise.
// FUNCTION: FF7 0x00639cab
void __cdecl Field_EmitBatchNodeTexturedQuad(FieldBatchNode *node, float x, float y, float z, float uOrigin, float vOrigin, float uSpan, float vSpan, int metaId, float width, float height, int mode)
{
    GfxTlVertex *bottomLeftVertex;
    GfxTlVertex *topLeftVertex;
    GfxTlVertex *pOpaqueTopRight;
    GfxTlVertex *opaqueBottomLeftPtr;
    GfxTlVertex *opaqueTopLeftPtr;
    GfxTlVertex *bottomRightVertexPtr;
    GfxTlVertex *topRightVertex;
    GfxTlVertex *opaqueBottomRight;

    if (AcquireBatchNode(1, node) != 0) {
        if (mode != 2) {
            *(char *)node->outBytePtr = metaId;
            node->metaId = metaId;
        }
        if (*(int *)((char *)node->hundredRecord + 0x44) == 0) {
            topLeftVertex = (GfxTlVertex *)(node->quadBuffers);
            topLeftVertex->x = x;
            topLeftVertex->y = y;
            topLeftVertex->z = z;
            topLeftVertex->rhwBits = FLOAT_ONE_BITS;
            topLeftVertex->color = 0x80FFFFFF;
            topLeftVertex->specular = 0xFF000000;
            topLeftVertex->u = uOrigin;
            topLeftVertex->v = vOrigin;
            bottomLeftVertex = (GfxTlVertex *)(node->quadBuffers + 0x20);
            bottomLeftVertex->x = x;
            bottomLeftVertex->y = y + height;
            bottomLeftVertex->z = z;
            bottomLeftVertex->rhwBits = FLOAT_ONE_BITS;
            bottomLeftVertex->color = 0x80FFFFFF;
            bottomLeftVertex->specular = 0xFF000000;
            bottomLeftVertex->u = uOrigin;
            bottomLeftVertex->v = vOrigin + vSpan;
            topRightVertex = (GfxTlVertex *)(node->quadBuffers + 0x40);
            topRightVertex->x = x + width;
            topRightVertex->y = y;
            topRightVertex->z = z;
            topRightVertex->rhwBits = FLOAT_ONE_BITS;
            topRightVertex->color = 0x80FFFFFF;
            topRightVertex->specular = 0xFF000000;
            topRightVertex->u = uOrigin + uSpan;
            topRightVertex->v = vOrigin;
            bottomRightVertexPtr = (GfxTlVertex *)(node->quadBuffers + 0x60);
            bottomRightVertexPtr->x = x + width;
            bottomRightVertexPtr->y = y + height;
            bottomRightVertexPtr->z = z;
            bottomRightVertexPtr->rhwBits = FLOAT_ONE_BITS;
            bottomRightVertexPtr->color = 0x80FFFFFF;
            bottomRightVertexPtr->specular = 0xFF000000;
            bottomRightVertexPtr->u = uOrigin + uSpan;
            bottomRightVertexPtr->v = vOrigin + vSpan;
        } else {
            opaqueTopLeftPtr = (GfxTlVertex *)(node->quadBuffers);
            opaqueTopLeftPtr->x = x;
            opaqueTopLeftPtr->y = y;
            opaqueTopLeftPtr->z = z;
            opaqueTopLeftPtr->rhwBits = FLOAT_ONE_BITS;
            opaqueTopLeftPtr->color = 0xFFFFFFFF;
            opaqueTopLeftPtr->specular = 0xFF000000;
            opaqueTopLeftPtr->u = uOrigin;
            opaqueTopLeftPtr->v = vOrigin;
            opaqueBottomLeftPtr = (GfxTlVertex *)(node->quadBuffers + 0x20);
            opaqueBottomLeftPtr->x = x;
            opaqueBottomLeftPtr->y = y + height;
            opaqueBottomLeftPtr->z = z;
            opaqueBottomLeftPtr->rhwBits = FLOAT_ONE_BITS;
            opaqueBottomLeftPtr->color = 0xFFFFFFFF;
            opaqueBottomLeftPtr->specular = 0xFF000000;
            opaqueBottomLeftPtr->u = uOrigin;
            opaqueBottomLeftPtr->v = vOrigin + vSpan;
            pOpaqueTopRight = (GfxTlVertex *)(node->quadBuffers + 0x40);
            pOpaqueTopRight->x = x + width;
            pOpaqueTopRight->y = y;
            pOpaqueTopRight->z = z;
            pOpaqueTopRight->rhwBits = FLOAT_ONE_BITS;
            pOpaqueTopRight->color = 0xFFFFFFFF;
            pOpaqueTopRight->specular = 0xFF000000;
            pOpaqueTopRight->u = uOrigin + uSpan;
            pOpaqueTopRight->v = vOrigin;
            opaqueBottomRight = (GfxTlVertex *)(node->quadBuffers + 0x60);
            opaqueBottomRight->x = x + width;
            opaqueBottomRight->y = y + height;
            opaqueBottomRight->z = z;
            opaqueBottomRight->rhwBits = FLOAT_ONE_BITS;
            opaqueBottomRight->color = 0xFFFFFFFF;
            opaqueBottomRight->specular = 0xFF000000;
            opaqueBottomRight->u = uOrigin + uSpan;
            opaqueBottomRight->v = vOrigin + vSpan;
        }
    }
}

/*
 * Acquired node receives four 32-byte float quad records from origin/size and UV origin/span with
 * raw depth, reciprocal one, shared color packed color and secondary 0xff000000. Publishes metaId
 * metadata byte/full dword. Failure causes no explicit record writes; no geometry clipping.
 */
// FUNCTION: FF7 0x00639fd4
void __cdecl Field_EmitBatchNodeTexturedQuadWithColor(GfxRenderObject *node, float x, float y, int z, float uOrigin, float vOrigin, float uSpan, float vSpan, int metaId, float width, float height, int color)
{
    GfxTlVertex *bottomLeftVertex;
    GfxTlVertex *topLeftVertex;
    GfxTlVertex *topRightVertex;
    GfxTlVertex *bottomRightVertex;

    if (AcquireBatchNode(1, node) != 0) {
        *(char *)node->colorIndex = metaId;
        node->texturePage = metaId;
        topLeftVertex = (GfxTlVertex *)(node->vertexCursor);
        topLeftVertex->x = x;
        topLeftVertex->y = y;
        topLeftVertex->zBits = z;
        topLeftVertex->rhwBits = FLOAT_ONE_BITS;
        topLeftVertex->color = color;
        topLeftVertex->specular = 0xFF000000;
        topLeftVertex->u = uOrigin;
        topLeftVertex->v = vOrigin;
        bottomLeftVertex = (GfxTlVertex *)(node->vertexCursor + 0x20);
        bottomLeftVertex->x = x;
        bottomLeftVertex->y = y + height;
        bottomLeftVertex->zBits = z;
        bottomLeftVertex->rhwBits = FLOAT_ONE_BITS;
        bottomLeftVertex->color = color;
        bottomLeftVertex->specular = 0xFF000000;
        bottomLeftVertex->u = uOrigin;
        bottomLeftVertex->v = vOrigin + vSpan;
        topRightVertex = (GfxTlVertex *)(node->vertexCursor + 0x40);
        topRightVertex->x = x + width;
        topRightVertex->y = y;
        topRightVertex->zBits = z;
        topRightVertex->rhwBits = FLOAT_ONE_BITS;
        topRightVertex->color = color;
        topRightVertex->specular = 0xFF000000;
        topRightVertex->u = uOrigin + uSpan;
        topRightVertex->v = vOrigin;
        bottomRightVertex = (GfxTlVertex *)(node->vertexCursor + 0x60);
        bottomRightVertex->x = x + width;
        bottomRightVertex->y = y + height;
        bottomRightVertex->zBits = z;
        bottomRightVertex->rhwBits = FLOAT_ONE_BITS;
        bottomRightVertex->color = color;
        bottomRightVertex->specular = 0xFF000000;
        bottomRightVertex->u = uOrigin + uSpan;
        bottomRightVertex->v = vOrigin + vSpan;
    }
}

// Emits one quad from the FieldQuadRecord rather than from loose arguments: four vertices written
// into the batch node the record names. The record's flags byte goes through the node's flags
// pointer, and the whole dword holding the flags byte is stored in the node's texturePage. Each vertex takes x and y from
// its record vertex fields, the shared quadZ as z, and its texture pair from the record. Diffuse is
// the constant 0xFFFFFFFF, specular 0xFF000000 and rhw 1.0f. Does nothing if AcquireBatchNode
// fails.
// FUNCTION: FF7 0x0063a171
void __cdecl Field_EmitQuadFromRecord(FieldQuadRecord *record)
{
    GfxTlVertex *vertex0;
    GfxTlVertex *vertex1;
    GfxTlVertex *vertex2;
    GfxTlVertex *vertex3;

    if (AcquireBatchNode(1, record->batchHandle) != 0) {
        *(char *)record->batchHandle->colorIndex = record->flags;
        record->batchHandle->texturePage = *(int *)((char *)record + 0x48);
        vertex0 = (GfxTlVertex *)(record->batchHandle->vertexCursor);
        vertex0->xBits = record->vertex0X;
        vertex0->yBits = record->vertex0Y;
        vertex0->zBits = record->quadZ;
        vertex0->rhwBits = FLOAT_ONE_BITS;
        vertex0->color = 0xFFFFFFFF;
        vertex0->specular = 0xFF000000;
        vertex0->uBits = record->vertex0Tu;
        vertex0->vBits = record->vertex0Tv;
        vertex1 = (GfxTlVertex *)(record->batchHandle->vertexCursor + 0x20);
        vertex1->xBits = record->vertex1X;
        vertex1->yBits = record->vertex1Y;
        vertex1->zBits = record->quadZ;
        vertex1->rhwBits = FLOAT_ONE_BITS;
        vertex1->color = 0xFFFFFFFF;
        vertex1->specular = 0xFF000000;
        vertex1->uBits = record->vertex1Tu;
        vertex1->vBits = record->vertex1Tv;
        vertex2 = (GfxTlVertex *)(record->batchHandle->vertexCursor + 0x40);
        vertex2->xBits = record->vertex2X;
        vertex2->yBits = record->vertex2Y;
        vertex2->zBits = record->quadZ;
        vertex2->rhwBits = FLOAT_ONE_BITS;
        vertex2->color = 0xFFFFFFFF;
        vertex2->specular = 0xFF000000;
        vertex2->uBits = record->vertex2Tu;
        vertex2->vBits = record->vertex2Tv;
        vertex3 = (GfxTlVertex *)(record->batchHandle->vertexCursor + 0x60);
        vertex3->xBits = record->vertex3X;
        vertex3->yBits = record->vertex3Y;
        vertex3->zBits = record->quadZ;
        vertex3->rhwBits = FLOAT_ONE_BITS;
        vertex3->color = 0xFFFFFFFF;
        vertex3->specular = 0xFF000000;
        vertex3->uBits = record->vertex3Tu;
        vertex3->vBits = record->vertex3Tv;
    }
}

/*
 * Walks the texture page table g_FieldLayers and calls Gfx_UploadRenderObjectPalette(context, mode, slot, value, page renderObject)
 * for every page whose enabled field is non-zero. It returns immediately when g_FieldTileStateDc0 is 2.
 * Three index ranges are visited: g_FieldLayer2StateE0c up to g_FieldLayer2StateE0c.sectionWord, then, only if g_FieldDoDrawLayer3 is
 * set, g_FieldMinMapId through g_FieldMaxMapId inclusive, then, only if g_FieldDoDrawLayer4 is set,
 * g_FieldLayer4StateE98.pageStart up to g_FieldLayer4StateE9c.
 */
// FUNCTION: FF7 0x0063a34a
void __cdecl Field_UploadTexturePagePalettes(int context, int mode, BattleRgbaColor *slot, int value)
{
    unsigned short pageIndex;

    if (g_FieldTileStateDc0 == 2) {
        return;
    }
    for (pageIndex = (g_FieldLayer2StateE0c.pageStart); pageIndex < (g_FieldLayer2StateE0c.sectionWord); pageIndex++) {
        if (g_FieldLayers[pageIndex]->enabled != 0) {
            Gfx_UploadRenderObjectPalette(context, mode, (int)slot, value, g_FieldLayers[pageIndex]->renderObject);
        }
    }
    if (g_FieldDoDrawLayer3 != 0) {
        for (pageIndex = *(short *)&g_FieldMinMapId; pageIndex <= g_FieldMaxMapId; pageIndex++) {
            if (g_FieldLayers[pageIndex]->enabled != 0) {
                Gfx_UploadRenderObjectPalette(context, mode, (int)slot, value, g_FieldLayers[pageIndex]->renderObject);
            }
        }
    }
    if (g_FieldDoDrawLayer4 != 0) {
        for (pageIndex = (g_FieldLayer4StateE98.pageStart); pageIndex < (g_FieldLayer4StateE9c.pageEnd); pageIndex++) {
            if (g_FieldLayers[pageIndex]->enabled != 0) {
                Gfx_UploadRenderObjectPalette(context, mode, (int)slot, value, g_FieldLayers[pageIndex]->renderObject);
            }
        }
    }
}

/*
 * When movie-control conditions choose the ordinary camera, restores 30 bytes from saved camera
 * state only if movie camera was installed, rebuilds camera matrices, restores its record pointer
 * and clears movie-active flag. Otherwise fetches movie camera, copies its three int translations
 * and nine short rotation entries, rebuilds matrices and sets movie-active flag.
 */
// FUNCTION: FF7 0x0063a4c8
#pragma intrinsic(memcpy)
void __cdecl Field_UpdateMovieCamera(void)
{
    int row, record, colIndex;
    if (g_FieldLogicActive == 0 || *(char *)&g_FieldActorState.state.movieCameraDisabled == 1) {
        if (g_FieldLogicStateFlag != 0) {
            g_FieldLogicStateFlag = 0;
            memcpy((*(short (*)[3][3])&g_FieldCameraTransform), g_FieldCameraMatrixWork, 30);
            Field_RebuildCameraMatrices(GetEngineContext());
            g_pFieldCameraData = g_pFieldSection1;
        }
        g_FieldMovieCameraActive = 0;
    } else {
        g_FieldLogicStateFlag = 1;
        record = (int)Movie_FetchCameraRecord();
        g_pFieldCameraData = (struct FieldCameraData *)record;
        for (row = 0; row < 3; row++) {
            g_FieldCameraTranslation[row] = g_pFieldCameraData->translation[row];
            for (colIndex = 0; colIndex < 3; colIndex++) {
                (*(short (*)[3][3])&g_FieldCameraTransform)[row][colIndex] = g_pFieldCameraData->matrix[row][colIndex];
            }
        }
        Field_RebuildCameraMatrices(GetEngineContext());
        g_FieldMovieCameraActive = 1;
    }
}
#pragma function(memcpy)

/*
 * Calls Field_BltTileSpriteRange with zero, the shorts at g_FieldLayer2TilesNum, g_FieldBgLayer2ScrollX and g_FieldBgLayer2ScrollY, zero,
 * and rangeEnd narrowed to short.
 */
// FUNCTION: FF7 0x0063a5de
void __cdecl Field_BltLayer2TileSpritesDoubled(int rangeEnd)
{
    Field_BltTileSpriteRange(0, *(short *)&g_FieldLayer2TilesNum, g_FieldBgLayer2ScrollX, g_FieldBgLayer2ScrollY, 0, (short)rangeEnd);
}

/*
 * Renders a field scene when model storage exists and always returns one. An alternate global flag
 * runs a helper and returns before scene begin; otherwise begins the scene, optionally blits a
 * movie frame, and unless suppressed draws background/page ranges, ordinary models, optional
 * overlay batches, four splash batches, then flushes. A second pass applies context override for
 * flagged models, marks their batch records, clears the override, draws foreground page ranges
 * under the same visibility conditions, and runs effect helpers. Independently renders four message
 * windows and restores the viewport when enabled, ends the scene, and optionally records elapsed
 * timestamp ticks in the profiling block. Begin-scene results are ignored.
 */
// FUNCTION: FF7 0x0063a60b
int __cdecl Field_RenderModels(GfxEngineContext *context)
{
    FieldModelInstance *animRecord;
    int endTicks[2];
    int workStatus;
    int startTimestamp[2];
    int temp;
    int i;
    int animationIndex;
    int flagBits;
    int batchIndex;

    temp = 0;
    workStatus = 0;
    flagBits = 0;
    if (*(int *)&g_pFieldModelSlots != 0) {
        ReadTimestampCounter((struct Timestamp64 *)startTimestamp);
        Field_SyncProjectionOffsetToCamera();
        if (g_FieldRenderActive != 0) {
            Field_ToggleHandCursorBlink();
            return 1;
        }
        Gfx_BeginSceneCall(0, context);
        if (Movie_IsNotPlaying() == 0) {
            MovieBltFrameToBackBuffer(GfxGetSurfaceByIndex(1));
        }
        if (*(char *)&g_FieldActorState.state.fieldHidden == 0) {
            if (g_FieldLogicActive == 0 || g_FieldMoviePlaying == 1 || g_MovieFlag == 1) {
                Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, context);
                Field_DrawBackgroundLayers(g_FieldBgLayer2ScrollX, g_FieldBgLayer2ScrollY);
                Field_DrawTexturePageRange(context, 0, 0xF);
                Field_DrawTexturePageRange(context, 0x1A, 0x21);
            }
            Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 1, context);
            for (i = 0; i < g_FieldHumanAnimState.modelListCountCopy; i++) {
                if (((int *)&g_FieldModelSortBuffer)[i] != 0) {
                    continue;
                }
                animationIndex = i;
                animRecord = (FieldModelInstance *)(((FieldHumanAnimEntry *)(*(int *)&g_pFieldModelSlots))[animationIndex].animRecord);
                if (animRecord->enabled != 0) {
                    Anim_DrawSkeletonFrame(animRecord->frameIndex, (PolygonSkeletonAnim *)(animRecord->skeletonAnim),
                                           (PolygonBoneTransform *)((int)animRecord + 0x38), (PolygonAnimDraw *)(animRecord->skeleton),
                                           (GfxEngineContext *)animRecord->drawContext);
                }
            }
            if (g_FieldLogicActive == 0 || g_FieldMoviePlaying == 1 || g_MovieFlag == 1) {
                Field_UpdateHandCursor();
                if (g_FieldHandImageType != 0) {
                    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, context);
                    Gfx_EmitPrimitiveBatchList(g_pFieldHandImageBuffer, context);
                }
            }
            Gfx_SetScenePassCall(1, context);
            Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, context);
            for (batchIndex = 0; batchIndex < 4; batchIndex++) {
                Gfx_EmitPrimitiveBatchList(((void **)&g_pFieldSplashTexture)[batchIndex], context);
            }
            GfxFlushFrameDrawList(context);
            Gfx_ApplyPresetStateCall(0, context);
            SelectContextSlotOverride(0, context);
            for (i = 0; i < g_FieldHumanAnimState.modelListCountCopy; i++) {
                if (((int *)&g_FieldModelSortBuffer)[i] != 0) {
                    animationIndex = i;
                    animRecord = (FieldModelInstance *)(((FieldHumanAnimEntry *)(*(int *)&g_pFieldModelSlots))[animationIndex].animRecord);
                    Field_SetModelVertexAlpha192((FieldModelAlphaGroups *)animRecord->skeleton, context);
                    if (animRecord->enabled != 0) {
                        Anim_DrawSkeletonFrame(animRecord->frameIndex,
                                               (PolygonSkeletonAnim *)(animRecord->skeletonAnim), (PolygonBoneTransform *)((int)animRecord + 0x38),
                                               (PolygonAnimDraw *)(animRecord->skeleton),
                                               (GfxEngineContext *)animRecord->drawContext);
                    }
                    g_FieldModelLightData[i]->flags |= 1;
                }
            }
            ClearContextSlotOverride(context);
            if (g_FieldLogicActive == 0 || g_FieldMoviePlaying == 1 || g_MovieFlag == 1) {
                Field_DrawTexturePageRange(context, 0xF, 0x1A);
                Field_DrawTexturePageRange(context, 0x21, 0x2A);
            }
            Field_EmitLineDrawBatch();
            Field_FxSubmitFadeOverlay(context);
        }
        if (g_FieldOpenWindowCount != 0) {
            Menu_SetMessageWindowYOffset(0);
            Menu_RenderMessageWindows(g_FieldWindows, 4, g_pFieldState->windowPhase ^ 1);
            Menu_FlushUiBatches();
            GfxSetViewportRect(g_FieldViewportX, g_FieldViewportY, g_FieldViewportWidth, g_FieldViewportHeight,
                               context);
        }
        Gfx_EndSceneCall(context);
        ReadTimestampCounter((struct Timestamp64 *)endTicks);
        if (context->statsRecord != 0) {
            Sub64((struct Timestamp64 *)endTicks, (struct Timestamp64 *)startTimestamp, &context->statsRecord->timeA);
        }
    }
    return 1;
}
} /* extern "C" */
