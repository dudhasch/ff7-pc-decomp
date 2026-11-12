// src/ff7/src/battle/battle_fx_special_ultrasound.cpp
/* Enemy Special Effect [idx 11]: Ultrasound (JP: onpa / 音波 (Sonic Wave), asset: "SPECIAL/ONPA") — Ultrasound (0x2b7, Evilhead), Spaz Voice (0x254, Razor Weed) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Onpa attack: loads resources, spawns a timeline per
//   target, and drives a radial sprite effect and a growing model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a89b8
int g_BattleSpecUltrasoundResourceDef = 0x001940B1;
int g_BattleSpecUltrasoundResourceDefHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialUltrasoundResourceNames + 0x0)
};
int g_BattleSpecUltrasoundResourceDefDataTail[8] = {
    0x0, 0x0, 0x0, 0x192a39, 0x0, (int)((char *)&g_BattleFxSpecialUltrasoundResourceNames + 0xc),
    0x0, 0x0
};
// GLOBAL: FF7 0x008a89e0
struct BattleSpecUltrasoundDrawReq g_BattleSpecUltrasoundDrawReq = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0};
unsigned char g_rawData_008a89f4[4] = {0xa8, 0x00, 0x00, 0x00};
unsigned char g_rawData_008a89f8[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a8a00
int g_BattleSpecUltrasoundRotation = 1024;
unsigned char g_BattleFxSpecialUltrasoundResourceNames[24] =
    "onpa_11.rsd\0"
    "jo_c04.spt";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00b73f8c
int g_BattleSpecUltrasoundPacketCursor = 0;
// GLOBAL: FF7 0x00b73f90
struct GfxTableCursor *g_BattleSpecUltrasoundLoadedRes = 0;
unsigned char g_bssPad_00b73f94[4] = {0};
// clang-format on

/*
 * Loads resources for the Onpa special effect when not already loaded.
 */
// FUNCTION: FF7 0x0055f7e0
void __cdecl BattleFx_SpecialLoadResUltrasound(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecUltrasoundLoadedRes == 0) {
        kind = 2;
        resourceDefsWord = (int)&g_BattleSpecUltrasoundResourceDef;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a8a20
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/ONPA", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecUltrasoundLoadedRes, ownerHandle);
    }
}

/*
 * Entry point for the Onpa special effect: forwards to BattleFx_SpecialBeginUltrasound.
 */
// FUNCTION: FF7 0x0055f870
void __cdecl BattleFx_SpecialEnterUltrasound(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginUltrasound(targetMask, actorIdx);
}

// Calls BattleFx_SpecialLoadResUltrasound, then creates a task for BattleFx_SpecialSpawnUltrasoundPerTarget with both arguments and mode 4, then
// plays sound 0x118 panned to the caster.
// FUNCTION: FF7 0x0055f885
void __cdecl BattleFx_SpecialBeginUltrasound(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResUltrasound();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnUltrasoundPerTarget);
    Battle_PlaySfxPannedToActor(0x118, (short)actorIdx, 0x40);
}

// Allocates BattleFx_SpecialUltrasoundTimelineTask with the caster's bone33 world position as its start and the target actor's
// targetBone world position as its end, then stores the target actor in actorIndex.
// FUNCTION: FF7 0x0055f8ba
void __cdecl BattleFx_SpecialSpawnUltrasoundPerTarget(int actor, int casterIndex)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxUltrasoundTimeline *taskRecord;

    taskRecord = (BattleFxUltrasoundTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialUltrasoundTimelineTask);
    Battle_GetBoneWorldPos(casterIndex, g_BattleActorRecords[casterIndex].bone33, &taskRecord->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &taskRecord->endX);
    taskRecord->actorIndex = actor;
}

// When unpaused below counter ten emits a BattleFx_SpecialUltrasoundGrowingModelTask slot copying XYZ, with velocity toward the
// stored target divided by twenty and scale 0x800. At counters sixteen through twenty-three emits
// BattleFx_SpecialUltrasoundSpriteTask with sine/cosine XY velocities of magnitude sixty, XY at the target plus ten times
// those velocities and target Z. Advances and at exactly twenty-four calls Battle_PostActorHitOrHurtMessage with
// actorIndex then retires; the initial range has no nonnegative-counter check.
// FUNCTION: FF7 0x0055f932
void __cdecl BattleFx_SpecialUltrasoundTimelineTask(void)
{
    BattleFxUltrasoundTimeline *mainSelf;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxUltrasoundParticle *spawned;

    mainSelf = (BattleFxUltrasoundTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (mainSelf->frame < 0xA && mainSelf->frame % 1 == 0) {
        spawned = (BattleFxUltrasoundParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialUltrasoundGrowingModelTask);
        spawned->posX = mainSelf->startX;
        spawned->posY = mainSelf->startY;
        spawned->posZ = mainSelf->startZ;
        spawned->velocityX = (mainSelf->endX - mainSelf->startX) / 0x14;
        spawned->velocityY = (mainSelf->endY - mainSelf->startY) / 0x14;
        spawned->velocityZ = (mainSelf->endZ - mainSelf->startZ) / 0x14;
        spawned->scale = 0x800;
    }
    if (mainSelf->frame >= 0x10 && mainSelf->frame < 0x18) {
        spawned = (BattleFxUltrasoundParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialUltrasoundSpriteTask);
        spawned->velocityX = CosFixed((mainSelf->frame - 0x10 << 9) - 0x400) * 0x3C >> 0xC;
        spawned->velocityY = SinFixed((mainSelf->frame - 0x10 << 9) - 0x400) * 0x3C >> 0xC;
        spawned->posX = mainSelf->endX + spawned->velocityX * 0xA;
        spawned->posY = mainSelf->endY + spawned->velocityY * 0xA;
        spawned->posZ = mainSelf->endZ;
    }
    (mainSelf->frame)++;
    if (mainSelf->frame == 0x18) {
        Battle_PostActorHitOrHurtMessage(mainSelf->actorIndex);
        mainSelf->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame tick for one radial sprite effect. Slot 1 of the loaded resources is the sprite
// group, and the scene's third render object is the request's field. The slot's position is
// billboarded at 4.12 scale 0x2000 (2.0), its frame is copied to the request, and the sprite goes
// to the depth-sorted draw list with shift 0xC. Unless paused, the slot moves by its velocity;
// with a sprite group present, its frame advances and it is retired (ownerSlot =
// BATTLE_TASK_FREE) at the group's frame count.
// FUNCTION: FF7 0x0055fb3c
void __cdecl BattleFx_SpecialUltrasoundSpriteTask(void)
{
    BattleFxUltrasoundParticle *task;

    task = (BattleFxUltrasoundParticle *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecUltrasoundDrawReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecUltrasoundLoadedRes);
    g_BattleSpecUltrasoundDrawReq.radialSpriteField = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000, 0);
    g_BattleSpecUltrasoundDrawReq.radialFrameIndex = task->frame;
    g_BattleSpecUltrasoundPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecUltrasoundDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecUltrasoundPacketCursor);
    if (g_BattleIsPaused == 0) {
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        if (g_BattleSpecUltrasoundDrawReq.spriteGroup != 0) {
            (task->frame)++;
            if (task->frame >= g_BattleSpecUltrasoundDrawReq.spriteGroup->header->frameCount) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

// Per-frame tick for the growing model effect. The model matrix is built from the slot's look
// direction, g_BattleSpecUltrasoundRotation, the slot's position and its scale, then multiplied by
// g_BattleCameraTransform. The draw colour is black, and the model goes to the depth-sorted draw
// list with shift 0xC. Unless the battle is paused, the slot moves by its velocity, grows its
// scale by 0x190 and counts a frame; at frame 0x14 it is retired (ownerSlot = BATTLE_TASK_FREE).
// FUNCTION: FF7 0x0055fc37
void __cdecl BattleFx_SpecialUltrasoundGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxUltrasoundParticle *onpaSlot; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_64[0x64]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.onpaSlot = (BattleFxUltrasoundParticle *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecUltrasoundDrawReq.modelField = Gfx_ModelTableGetRenderObject(0, g_BattleSpecUltrasoundLoadedRes);
    Gte_BuildLookBasisFromDirection(&frame.onpaSlot->velocityX, frame.lookBasis.m[0]);
    RotMatrix((SVECTOR *)&g_BattleSpecUltrasoundRotation, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.rotMatrix, &frame.rotMatrix);
    frame.scaleVec.vz = frame.onpaSlot->scale;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    frame.rotMatrix.t[0] = frame.onpaSlot->posX;
    frame.rotMatrix.t[1] = frame.onpaSlot->posY;
    frame.rotMatrix.t[2] = frame.onpaSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecUltrasoundPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecUltrasoundDrawReq.modelField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecUltrasoundPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.onpaSlot->posX += frame.onpaSlot->velocityX;
    frame.onpaSlot->posY += frame.onpaSlot->velocityY;
    frame.onpaSlot->posZ += frame.onpaSlot->velocityZ;
    frame.onpaSlot->scale += 0x190;
    (frame.onpaSlot->frame)++;
    if (frame.onpaSlot->frame == 0x14) {
        frame.onpaSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
