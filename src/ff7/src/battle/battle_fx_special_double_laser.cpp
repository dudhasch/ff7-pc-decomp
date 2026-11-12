// src/ff7/src/battle/battle_fx_special_double_laser.cpp
/* Enemy Special Attack [91]: Double Laser (orig. SPECIAL/WLASER, Wレーザー) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Wlaser attack: it loads resources and spawns a per-target
//   timeline that drives an animated sprite task on the secondary task slots and a beam sprite
//   task.

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
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a3188
int g_BattleSpecDoubleLaserResourceDefs = 0x00196A39;
int g_BattleSpecDoubleLaserResourceDefsEntryA[1] = {
    (int)((char *)&g_BattleSpecDoubleLaserFileNames + 0x0)
};
int g_BattleSpecDoubleLaserResourceDefsEntryB[18] = {
    (int)((char *)&g_BattleSpecDoubleLaserFileNames + 0x10), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleSpecDoubleLaserFileNames + 0x1c), (int)((char *)&g_BattleSpecDoubleLaserFileNames + 0x2c),
    0x0, 0x0, 0x192a39, 0x0, (int)((char *)&g_BattleSpecDoubleLaserFileNames + 0x38), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleSpecDoubleLaserFileNames + 0x44), (int)((char *)&g_BattleSpecDoubleLaserFileNames + 0x54), 0x0, 0x0
};
// clang-format on
// The animated sprite BattleFx_SpecialDoubleLaserAnimatedSpriteTask draws.
// clang-format off
// GLOBAL: FF7 0x008a31d8
BattleFxSpriteRequest g_BattleSpecDoubleLaserSpriteReq1 = {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2c, 0, 0};
// GLOBAL: FF7 0x008a31e8
BattleSpecDoubleLaserSpriteReq2 g_BattleSpecDoubleLaserSpriteReq2 = {{1, 2, 3}, 0, {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2e, 0, 0}};
// clang-format on
// The beam's x and y rotation angles (0x1000 a turn); RotMatrixXYZ reads its z angle from the bytes
// that follow.
// clang-format off
// GLOBAL: FF7 0x008a3208
short g_BattleSpecDoubleLaserBeamAngles[2] = {0, -0x400};
unsigned char g_BattleSpecDoubleLaserFileNames[96] =
    "wlaser02.tim\0\0\0\0"
    "reza1.spt\0\0\0"
    "wlaser01.tim\0\0\0\0"
    "maru.spt\0\0\0\0"
    "jo_c04.spt\0\0"
    "wlaser00.tim\0\0\0\0"
    "baku1.spt";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b65074
struct GfxTableCursor *g_BattleSpecDoubleLaserLoadedResTable = 0;
// clang-format on

/*
 * When the battle scene exists and g_BattleSpecDoubleLaserLoadedResTable is empty, builds SPECIAL/WLASER with a default-
 * duration record and creates resource type 4 using g_BattleSpecDoubleLaserResourceDefs.
 */
// FUNCTION: FF7 0x00539040
void __cdecl BattleFx_SpecialLoadResDoubleLaser(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecDoubleLaserLoadedResTable == 0) {
        kind = 4;
        resourceDefsWord = (int)&g_BattleSpecDoubleLaserResourceDefs;
        stage = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a3270
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/WLASER", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecDoubleLaserLoadedResTable, stage);
    }
}

/*
 * Mechanical forwarder: calls BattleFx_SpecialBeginDoubleLaser with both arguments unchanged.
 */
// FUNCTION: FF7 0x005390d0
void __cdecl BattleFx_SpecialEnterDoubleLaser(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginDoubleLaser(targetMask, actorIdx);
}

/*
 * Calls BattleFx_SpecialLoadResDoubleLaser, then creates a task for BattleFx_SpecialSpawnDoubleLaserPerTarget with both arguments and mode 4, then
 * dispatches event (0x3A, targetIndex, 0).
 */
// FUNCTION: FF7 0x005390e5
void __cdecl BattleFx_SpecialBeginDoubleLaser(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResDoubleLaser();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnDoubleLaserPerTarget);
    Battle_PlaySfxPannedToActor(0x3A, (short)actorIdx, 0);
}

// Allocates a BattleFx_SpecialDoubleLaserTimelineTask slot. The world position of actor's
// targetBone goes to the slot's targetBonePos; actor is the hit actor and targetIndex the caster.
// FUNCTION: FF7 0x00539117
void __cdecl BattleFx_SpecialSpawnDoubleLaserPerTarget(int actor, int targetIndex)
{
    BattleFxDoubleLaserTimeline *taskSlot;

    taskSlot = (BattleFxDoubleLaserTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDoubleLaserTimelineTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, taskSlot->targetBonePos);
    taskSlot->hitActor = actor;
    taskSlot->casterActor = targetIndex;
}

/*
 * Per-target timeline: at step 0 starts a beam task from each of the caster's two particle bones
 * (primaryBone, secondaryBone) to the hit actor, from step 4 spawns jittered animated sprites around
 * the hit point, plays a sound at step 0xA
 * and frees its slot at step 0x23.
 */
// FUNCTION: FF7 0x00539174
void __cdecl BattleFx_SpecialDoubleLaserTimelineTask(void)
{
    BattleFxDoubleLaserTimeline *p;
    short bone;
    BattleFxDoubleLaserBeamRecord *r;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    p = (BattleFxDoubleLaserTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (p->step == 0) {
        for (bone = 6; bone < 8; bone++) {
            r = (BattleFxDoubleLaserBeamRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDoubleLaserDrawBeamSpriteTask);
            Battle_GetBoneWorldPos(p->casterActor, (&g_BattleActorRecords[p->casterActor].targetBone)[bone], &r->posX);
            r->endX = p->targetBonePos[0];
            r->endY = p->targetBonePos[1];
            r->endZ = p->targetBonePos[2];
        }
    }
    if (p->step >= 4 && p->step < 8) {
        r = (BattleFxDoubleLaserBeamRecord *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialDoubleLaserAnimatedSpriteTask)];
        r->posX = p->targetBonePos[0] + rand() % 800 + -400;
        r->posY = p->targetBonePos[1] + rand() % 800 + -400;
        r->posZ = p->targetBonePos[2] + rand() % 800 + -400;
        r->stage = 0;
    }
    if (p->step >= 8 && p->step < 16) {
        r = (BattleFxDoubleLaserBeamRecord *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialDoubleLaserAnimatedSpriteTask)];
        r->posX = p->targetBonePos[0] + rand() % 1200 + -600;
        r->posY = p->targetBonePos[1] + rand() % 1200 + -600;
        r->posZ = p->targetBonePos[2] + rand() % 1200 + -600;
        r->stage = 1;
        r = (BattleFxDoubleLaserBeamRecord *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialDoubleLaserAnimatedSpriteTask)];
        r->posX = p->targetBonePos[0] + rand() % 1200 + -600;
        r->posY = p->targetBonePos[1] + rand() % 1200 + -600;
        r->posZ = p->targetBonePos[2] + rand() % 1200 + -600;
        r->stage = 2;
    }
    if (p->step == 10) {
        Battle_PostActorHitOrHurtMessage(p->hitActor);
        Battle_PlaySfxPannedToActor(0xFA, (short)p->hitActor, 0);
    }
    (p->step)++;
    if (p->step == 35) {
        p->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame step of an animated sprite in the secondary task slots. The table index is not a
 * constant: g_BattleSpecDoubleLaserSpriteReq2.variantTableIndex maps the slot's variant to it, and it is
 * looked up in g_BattleSpecDoubleLaserLoadedResTable for the request g_BattleSpecDoubleLaserSpriteReq1.
 * For variant 1 the draw object comes from Battle3d_GetSceneRenderObject(3, g_pBattle3dScene) instead
 * of Gfx_ModelTableGetSpriteObject. The slot is freed when its frame reaches the sprite group's frame
 * count. Drawing precedes the pause check; the frame advances only when unpaused with a nonnull
 * sprite group.
 */
// FUNCTION: FF7 0x00539482
void __cdecl BattleFx_SpecialDoubleLaserAnimatedSpriteTask(void)
{
    BattleFxTailLaserAnimSprite *task;

    task = (BattleFxTailLaserAnimSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecDoubleLaserSpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(g_BattleSpecDoubleLaserSpriteReq2.variantTableIndex[task->variant], g_BattleSpecDoubleLaserLoadedResTable);
    if (task->variant != 1) {
        g_BattleSpecDoubleLaserSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(g_BattleSpecDoubleLaserSpriteReq2.variantTableIndex[task->variant], g_BattleSpecDoubleLaserLoadedResTable);
    } else {
        g_BattleSpecDoubleLaserSpriteReq1.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    g_BattleSpecDoubleLaserSpriteReq1.frame = task->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecDoubleLaserSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleSpecDoubleLaserSpriteReq1.spriteGroup != 0) {
        (task->frame)++;
        if (task->frame == g_BattleSpecDoubleLaserSpriteReq1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws the beam sprite between the slot's start and end points, scaled to their distance, and frees
 * the slot once the sprite group's last frame has been shown.
 */
// FUNCTION: FF7 0x00539590
void __cdecl BattleFx_SpecialDoubleLaserDrawBeamSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[128];
        PsxMatrix beamMatrix;
        struct { unsigned char pad_to_20[0x20]; SVECTOR beamDirX; };
        struct { unsigned char pad_to_28[0x28]; BattleFxDoubleLaserDrawBeamSpriteSlot *beamSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_4C[0x4C]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_6C[0x6C]; short beamLength; };
        struct { unsigned char pad_to_70[0x70]; VECTOR beamScaleX; };
    } frame;
#pragma pack(pop)

    frame.beamSlot = (BattleFxDoubleLaserDrawBeamSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecDoubleLaserSpriteReq2.beamRequest.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecDoubleLaserLoadedResTable);
    g_BattleSpecDoubleLaserSpriteReq2.beamRequest.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecDoubleLaserLoadedResTable);
    frame.beamDirX.vx = frame.beamSlot->beamEndX - frame.beamSlot->beamStartX;
    frame.beamDirX.vy = frame.beamSlot->beamEndY - frame.beamSlot->beamStartY;
    frame.beamDirX.vz = frame.beamSlot->beamEndZ - frame.beamSlot->beamStartZ;
    frame.beamLength = SqrtToInt(frame.beamDirX.vx * frame.beamDirX.vx + frame.beamDirX.vy * frame.beamDirX.vy + frame.beamDirX.vz * frame.beamDirX.vz);
    Gte_BuildLookBasisFromDirection(&frame.beamDirX.vx, frame.lookBasis.m[0]);
    RotMatrixXYZ(g_BattleSpecDoubleLaserBeamAngles, &frame.beamMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.beamMatrix, &frame.beamMatrix);
    frame.beamScaleX.vx = (frame.beamLength << 12) / 2000;
    frame.beamScaleX.vz = 0x4000 /* 4.0 */;
    frame.beamScaleX.vy = frame.beamScaleX.vz;
    ScaleMatrix(&frame.beamMatrix, &frame.beamScaleX);
    frame.beamMatrix.t[0] = frame.beamSlot->beamStartX;
    frame.beamMatrix.t[1] = frame.beamSlot->beamStartY;
    frame.beamMatrix.t[2] = frame.beamSlot->beamStartZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.beamMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecDoubleLaserSpriteReq2.beamRequest.frame = frame.beamSlot->frameCounter;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecDoubleLaserSpriteReq2.beamRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecDoubleLaserSpriteReq2.beamRequest.spriteGroup != 0) {
        (frame.beamSlot->frameCounter)++;
        if (frame.beamSlot->frameCounter == g_BattleSpecDoubleLaserSpriteReq2.beamRequest.spriteGroup->header->frameCount) {
            frame.beamSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
