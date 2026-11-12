// src/ff7/src/battle/battle_fx_sbaria_barrier.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: S-Barrier battle effect: loads its resources, starts the barrier on an actor in either
//   variant, and runs the fading actor-model tasks that draw the model in passes.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "ff7/battle.h"
#include "lib/crt.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

struct BattleFxModelDraw;

// clang-format off
// GLOBAL: FF7 0x007c3248
unsigned int g_BattleSBariaResourceDefs[15] = {0x003200b1, (unsigned int)"JO_BA.RSD", 0x00000000, 0x00000000, 0x00000000, 0x003240b1, (unsigned int)"JO_MBA.RSD", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"JO_RIF.RSD", 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x007c3288
struct BattleFxSecondaryModelDrawRequest g_BattleSBariaModelDrawRequest = {0, 136, {0}, 0, {0x20, 0, 0, 0}};
// clang-format on


// clang-format off
// GLOBAL: FF7 0x009aeaec
struct GfxTableCursor *g_BattleFxSBariaResource = 0;
// clang-format on

/*
 * Loads the EFFECT/S_BARIA model table into g_BattleFxSBariaResource when the battle scene exists and
 * nothing is loaded yet.
 */
// FUNCTION: FF7 0x00438c80
void __cdecl BattleFx_EffectLoadResSBaria(void)
{
    GfxModelTableBank *sceneContext;
    GfxResourceRequest record;
    int resultId;
    char pathOut[200];
    GfxResourceRequest *request;
    int descriptor;

    if (g_pBattle3dScene != 0 && g_BattleFxSBariaResource == 0) {
        resultId = 3;
        descriptor = (int)&g_BattleSBariaResourceDefs;
        sceneContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &record;
        Gfx_InitResourceRequest(request);
        request->groupTag = 1;
        // STRING: FF7 0x007c32bc
        (request->resourcePath) = BattleFx_BuildResourcePath("EFFECT/S_BARIA", pathOut);
        Gfx_LoadModelTableIfAbsent(resultId, descriptor, request, &g_BattleFxSBariaResource, sceneContext);
    }
}

/*
 * Ensures barrier resources are loaded, sets model draw flags to 0x88, and spawns fading actor model variant 0.
 */
// FUNCTION: FF7 0x00438d10
void __cdecl BattleFx_SBariaBeginActorVariant0(int actorIdx)
{
    BattleFx_EffectLoadResSBaria();
    g_BattleSBariaModelDrawRequest.drawFlags = 0x88;
    BattleFx_SBariaSpawnFadingActorModel(actorIdx, 0);
}

/*
 * Starts a fading actor model task for the actor and picks the barrier model field of the given variant.
 */
// FUNCTION: FF7 0x00438d32
void __cdecl BattleFx_SBariaSpawnFadingActorModel(int actorIdx, int modelVariant)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        BattleFxSBariaFadingActor *fadingActor;
        struct { unsigned char pad_to_4[0x4]; int variantId0[3]; };
    } frame;
#pragma pack(pop)

    frame.variantId0[0] = 0;
    frame.variantId0[1] = 1;
    frame.variantId0[2] = 2;
    frame.fadingActor = (BattleFxSBariaFadingActor *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SBariaFadingActorModelTask)]);
    frame.fadingActor->actorIndex = actorIdx;
    frame.fadingActor->model = Gfx_ModelTableGetRenderObject(frame.variantId0[modelVariant], g_BattleFxSBariaResource);
}

/*
 * Builds the effect matrix from the actor's rotation, scale and target-bone world position (offset along
 * the actor's yaw), computes a sixteen-frame opacity envelope and draws the model until it expires.
 */
// FUNCTION: FF7 0x00438d93
void __cdecl BattleFx_SBariaFadingActorModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        PsxMatrix effectMatrix;
        struct { unsigned char pad_to_20[0x20]; short opacity; };
        struct { unsigned char pad_to_24[0x24]; int fadeFraction; };
        struct { unsigned char pad_to_28[0x28]; BattleFxSBariaFadeModel *fadeSlot; };
        struct { unsigned char pad_to_2C[0x2C]; SVECTOR boneWorldX; };
        struct { unsigned char pad_to_34[0x34]; struct BattleActor *actorState; };
        struct { unsigned char pad_to_38[0x38]; int frameIndex; };
        struct { unsigned char pad_to_3C[0x3C]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.fadeSlot = (BattleFxSBariaFadeModel *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    frame.actorState = &g_BattleActorRecords[frame.fadeSlot->actorIndex];
    RotMatrix((SVECTOR *)&frame.actorState->rotX, &frame.effectMatrix);
    frame.scaleX.vz = frame.actorState->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.effectMatrix, &frame.scaleX);
    Battle_GetBoneWorldPos(frame.fadeSlot->actorIndex, frame.actorState->targetBone, &frame.boneWorldX.vx);
    frame.boneWorldX.vx -= (short)(SinFixed(frame.actorState->yaw) * (frame.actorState->radius + 100) >> 12);
    frame.boneWorldX.vz -= (short)(CosFixed(frame.actorState->yaw) * (frame.actorState->radius + 100) >> 12);
    frame.effectMatrix.t[0] = frame.boneWorldX.vx * frame.actorState->scale >> 12;
    frame.effectMatrix.t[1] = frame.boneWorldX.vy * frame.actorState->scale >> 12;
    frame.effectMatrix.t[2] = frame.boneWorldX.vz * frame.actorState->scale >> 12;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.effectMatrix, &frame.effectMatrix);
    SetRotMatrix(&frame.effectMatrix);
    SetTransMatrix(&frame.effectMatrix);
    frame.frameIndex = frame.fadeSlot->frame;
    if (frame.frameIndex < 4) {
        frame.fadeFraction = (frame.frameIndex << 12) / 4;
        frame.opacity = 0x1000 - frame.fadeFraction;
    } else {
        frame.frameIndex -= 4;
        if (frame.frameIndex < 8) {
            frame.opacity = 0;
        } else {
            frame.frameIndex -= 8;
            if (frame.frameIndex < 4) {
                frame.fadeFraction = (frame.frameIndex << 12) / 4;
                frame.opacity = *(short *)&frame.fadeFraction;
            } else {
                frame.fadeSlot->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    if (g_BattleIsPaused == 0) {
        (frame.fadeSlot->frame)++;
    }
    BattleFx_SBariaDrawModelFourPasses(frame.fadeSlot->model, (short)frame.opacity);
}

/*
 * Stores modelHandle and fadeAlpha, sets draw color black, and submits the model across four passes
 * while toggling draw request mask bits.
 */
// FUNCTION: FF7 0x00438f90
void __cdecl BattleFx_SBariaDrawModelFourPasses(int modelHandle, short fadeAlpha)
{
    g_BattleSBariaModelDrawRequest.modelHandle = modelHandle;
    g_BattleSBariaModelDrawRequest.fadeAlpha = fadeAlpha;
    SetDrawColorRGB(0, 0, 0);
    PushRotMatrix();
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSBariaModelDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    PopRotMatrix();
    PushRotMatrix();
    g_BattleSBariaModelDrawRequest.drawFlags |= 1;
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSBariaModelDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    PopRotMatrix();
    PushRotMatrix();
    g_BattleSBariaModelDrawRequest.drawFlags |= 2;
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSBariaModelDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    PopRotMatrix();
    g_BattleSBariaModelDrawRequest.drawFlags &= 0xFFFFFFFE;
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSBariaModelDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    g_BattleSBariaModelDrawRequest.drawFlags &= 0xFFFFFFFD;
}

/*
 * Ensures barrier resources are loaded, sets model draw flags to 0xA8, and spawns fading actor model variant 1.
 */
// FUNCTION: FF7 0x004390a0
void __cdecl BattleFx_SBariaBeginActorVariant1(int actorIdx)
{
    BattleFx_EffectLoadResSBaria();
    g_BattleSBariaModelDrawRequest.drawFlags = 0xA8;
    BattleFx_SBariaSpawnFadingActorModel(actorIdx, 1);
}

/*
 * Ensures barrier resources are loaded, sets model draw flags to 0x88, and spawns fading actor model variant 2.
 */
// FUNCTION: FF7 0x004390c2
void __cdecl Battle_TriggerActorHurtSequence(int actorIdx)
{
    BattleFx_EffectLoadResSBaria();
    g_BattleSBariaModelDrawRequest.drawFlags = 0x88;
    BattleFx_SBariaSpawnFadingActorModel(actorIdx, 2);
}
} /* extern "C" */
