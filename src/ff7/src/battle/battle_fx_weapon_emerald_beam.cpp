// src/ff7/src/battle/battle_fx_weapon_emerald_beam.cpp
/* Enemy Special Attack [153, 156]: Emerald Beam / Ruby Ray (orig. weapon/beam1) */
// Contents: Battle effect for the first weapon beam: it loads the beam resources, starts the beam
//   timeline, and draws the burst and delayed sprites plus the beam and shrinking models aimed at
//   the targets.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/graphics.h"
#include "ff7/camera_script.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"
#include "ff7/api/menu.h"
#include <stdlib.h>
extern "C" {

struct BattleFxModelDraw;


// clang-format off
unsigned char g_rawData_007d83d8[32] = {
    0x41, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd2, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x007d83f8
int g_WeaponFxEmeraldBeamSoundGroups[4] = {8225752, 8225768, 0, 0};
// GLOBAL: FF7 0x007d8408
int g_WeaponFxEmeraldBeamResourceDefs = 0x00192A39;
int g_WeaponFxEmeraldBeamResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxWeaponEmeraldBeamResourceNames + 0x0)
};
int g_WeaponFxEmeraldBeamResourceDefsDataTail[28] = {
    (int)((char *)&g_BattleFxWeaponEmeraldBeamResourceNames + 0xc), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxWeaponEmeraldBeamResourceNames + 0x18), (int)((char *)&g_BattleFxWeaponEmeraldBeamResourceNames + 0x24),
    0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxWeaponEmeraldBeamResourceNames + 0x30), 0x0, 0x0,
    0x0, 0x1900b1, (int)((char *)&g_BattleFxWeaponEmeraldBeamResourceNames + 0x3c), 0x0, 0x0, 0x0,
    0x3200b1, (int)((char *)&g_BattleFxWeaponEmeraldBeamResourceNames + 0x48), 0x0, 0x0, 0x0, 0x3200b1,
    (int)((char *)&g_BattleFxWeaponEmeraldBeamResourceNames + 0x54), 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x007d8480
int g_WeaponFxEmeraldBeamNegativeYAxisRef[3] = {0, -4096, 0};
// GLOBAL: FF7 0x007d8490
int g_WeaponFxEmeraldBeamNegativeXAxisRef[3] = {-4096, 0, 0};
// GLOBAL: FF7 0x007d84a0
struct WeaponFxEmeraldBeamDelayedSpriteReq g_WeaponFxEmeraldBeamBurstSpriteReq = {0, 0, -128, -128, -128, {0x2e, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007d84b0
struct WeaponFxEmeraldBeamFadingSpriteReq g_WeaponFxEmeraldBeamSecondarySpriteReq = {0, 0, -128, -128, -128, {0x2e}, 0, {0}};
unsigned char g_BattleFxWeaponEmeraldBeamResourceNames[96] =
    "AWA_1.TIM\0\0\0"
    "AWA_1.SPT\0\0\0"
    "HIT_1.TIM\0\0\0"
    "HIT_1.SPT\0\0\0"
    "BEAM1.rsd\0\0\0"
    "BEAM2.rsd\0\0\0"
    "ENERGY1.rsd\0"
    "ENERGY2.rsd";
// GLOBAL: FF7 0x009f2918
int g_WeaponFxEmeraldBeamSourceActor = 0;
unsigned char g_bssPad_009f291c[4] = {0};
// GLOBAL: FF7 0x009f2920
WeaponFxEmeraldBeamPoint g_WeaponFxEmeraldBeamCentroidPos = {0};
// GLOBAL: FF7 0x009f2928
WeaponFxEmeraldBeamPoint g_WeaponFxEmeraldBeamBonePos = {0};
// GLOBAL: FF7 0x009f2930
int g_WeaponFxEmeraldBeamTargetMask = 0;
// GLOBAL: FF7 0x009f2934
int g_WeaponFxEmeraldBeamVariantFlag = 0;
// GLOBAL: FF7 0x009f2938
int g_WeaponFxEmeraldBeamTargetActor = 0;
unsigned char g_bssPad_009f293c[4] = {0};
// GLOBAL: FF7 0x009f2940
int g_WeaponFxEmeraldBeamDirectionVector[3] = {0};
unsigned char g_bssPad_009f294c[4] = {0};
// GLOBAL: FF7 0x009f2950
struct GfxTableCursor *g_WeaponFxEmeraldBeamResourceContext = 0;
unsigned char g_bssPad_009f2954[4] = {0};
// clang-format on

/*
 * Loads weapon/beam1 resources into the battle scene context if not already loaded.
 */
// FUNCTION: FF7 0x00447ab0
void __cdecl BattleFx_WeaponLoadResEmeraldBeam(void)
{
    GfxModelTableBank *sceneContext;
    GfxResourceRequest requestBuffer;
    int resultId;
    char pathBuffer[200];
    GfxResourceRequest *pReq;
    int defsPtr;

    if (g_pBattle3dScene != 0 && g_WeaponFxEmeraldBeamResourceContext == 0) {
        resultId = 6;
        defsPtr = (int)&g_WeaponFxEmeraldBeamResourceDefs;
        sceneContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestBuffer;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007d8520
        pReq->resourcePath = BattleFx_BuildResourcePath("weapon/beam1", pathBuffer);
        Gfx_LoadModelTableIfAbsent(resultId, defsPtr, pReq, &g_WeaponFxEmeraldBeamResourceContext, sceneContext);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_WeaponFxEmeraldBeamSoundGroups);
    }
}

/*
 * Entry point to initialize Weapon Beam 1 with variant flag set to 0.
 */
// FUNCTION: FF7 0x00447b51
void __cdecl BattleFx_WeaponEnterEmeraldBeamVariant0(int targetMask, int actorIdx)
{
    g_WeaponFxEmeraldBeamVariantFlag = 0;
    BattleFx_WeaponLoadResEmeraldBeam();
    BattleFx_WeaponBeginEmeraldBeam(targetMask, actorIdx);
}

/*
 * Entry point to initialize Weapon Beam 1 with variant flag set to 1.
 */
// FUNCTION: FF7 0x00447b75
void __cdecl BattleFx_WeaponEnterEmeraldBeamVariant1(int targetMask, int actorIdx)
{
    g_WeaponFxEmeraldBeamVariantFlag = 1;
    BattleFx_WeaponLoadResEmeraldBeam();
    BattleFx_WeaponBeginEmeraldBeam(targetMask, actorIdx);
}

/*
 * Initializes target bitmask, registers sound groups, and starts Beam 1 timeline task.
 */
// FUNCTION: FF7 0x00447b99
void __cdecl BattleFx_WeaponBeginEmeraldBeam(int targetMask, int casterIdx)
{
    int actorIdx;
    int targetCount;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_WeaponEmeraldBeamSpawnTimeline);
    g_WeaponFxEmeraldBeamTargetMask = targetMask;
    actorIdx = 0;
    for (targetCount = 0; actorIdx < 0xA; actorIdx++) {
        if ((targetMask & 1 << actorIdx) != 0) {
            targetCount++;
            if (targetCount == 1) {
                g_WeaponFxEmeraldBeamSourceActor = actorIdx;
            }
        }
    }
    g_WeaponFxEmeraldBeamTargetActor = casterIdx;
}

/*
 * Calculates centroid of target actor group and initializes Beam 1 main timeline task.
 */
// FUNCTION: FF7 0x00447c2a
void __cdecl BattleFx_WeaponEmeraldBeamSpawnTimeline(void)
{
    BattleFxWeaponEmeraldBeamTimeline *task;

    task = (BattleFxWeaponEmeraldBeamTimeline *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxWeaponEmeraldBeamTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponEmeraldBeamTimelineTask);
    task->beamState = 0;
    Battle_GetBoneWorldPos(g_WeaponFxEmeraldBeamSourceActor, g_BattleActorRecords[g_WeaponFxEmeraldBeamSourceActor].targetBone, &g_WeaponFxEmeraldBeamBonePos.posX);
    g_WeaponFxEmeraldBeamBonePos.radius = g_BattleActorRecords[g_WeaponFxEmeraldBeamSourceActor].radius + 0x100;
}

/*
 * Coordinates Beam 1 effect sequence: camera shakes, sprites, beam models, and sounds.
 */
// FUNCTION: FF7 0x00447cb8
void __cdecl BattleFx_WeaponEmeraldBeamTimelineTask(void)
{
    int idx;
    int sizeBase;
    int angle;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int magnitude;
    BattleTaskSlot *task;
    BattleFxEmeraldBeamTimelineRecord *pRecord;
    BattleFxWeaponEmeraldBeamSprite *pSprite;
    int panVal;

    Battle_GetBoneWorldPos(g_WeaponFxEmeraldBeamTargetActor, g_BattleActorRecords[g_WeaponFxEmeraldBeamTargetActor].targetBone, &g_WeaponFxEmeraldBeamCentroidPos.posX);
    g_WeaponFxEmeraldBeamCentroidPos.radius = g_BattleActorRecords[g_WeaponFxEmeraldBeamTargetActor].radius + 0x100;
    g_WeaponFxEmeraldBeamDirectionVector[0] = g_WeaponFxEmeraldBeamBonePos.posX - g_WeaponFxEmeraldBeamCentroidPos.posX;
    g_WeaponFxEmeraldBeamDirectionVector[1] = g_WeaponFxEmeraldBeamBonePos.posY - g_WeaponFxEmeraldBeamCentroidPos.posY;
    g_WeaponFxEmeraldBeamDirectionVector[2] = g_WeaponFxEmeraldBeamBonePos.posZ - g_WeaponFxEmeraldBeamCentroidPos.posZ;
    NormalizeVectorFixed(&g_WeaponFxEmeraldBeamDirectionVector[0], &g_WeaponFxEmeraldBeamDirectionVector[0]);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task = BATTLE_CURRENT_TASK_SLOT;
    if (task->frameCounter == 1) {
        pRecord = (BattleFxEmeraldBeamTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponEmeraldBeamShrinkingModelTask);
        pRecord->scale = 0x6000;
        pRecord->shrinkSpeed = pRecord->scale / 5;
    }
    if (task->frameCounter == 6) {
        pRecord = (BattleFxEmeraldBeamTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponEmeraldBeamBeamModelTask);
        pRecord->scale = 0x666;
    }
    if (task->frameCounter == 0x10) {
        sizeBase = 0x2440;
        for (idx = 0; idx < 3; idx++) {
            pRecord = (BattleFxEmeraldBeamTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponEmeraldBeamDelayedSpriteTask);
            *(Int64Pair *)&pRecord->posX = *(Int64Pair *)&g_WeaponFxEmeraldBeamBonePos;
            pRecord->posX += (short)(0x190 - rand() % 0x320);
            pRecord->posY += (short)(0x190 - rand() % 0x320);
            pRecord->posZ += (short)(0x190 - rand() % 0x320);
            angle = rand() % 0xA00 + 0xC00;
            pRecord->scale = sizeBase * angle >> 0xC;
            pRecord->variant = rand() % 6;
        }
    }
    if (task->frameCounter >= 0x12 && task->frameCounter <= 0x15) {
        sizeBase = 0x666;
        for (idx = 0; idx < 0xA; idx++) {
            pSprite = (BattleFxWeaponEmeraldBeamSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponEmeraldBeamFadingSpriteTask);
            angle = (rand() % 4096);
            magnitude = (g_WeaponFxEmeraldBeamBonePos.radius >> 1) + rand() % 0x64;
            pSprite->posX = g_WeaponFxEmeraldBeamBonePos.posX + (CosFixed(angle) * magnitude >> 0xC);
            pSprite->posY = g_WeaponFxEmeraldBeamBonePos.posY + rand() % 0xB4 + 0x1E;
            pSprite->posZ = g_WeaponFxEmeraldBeamBonePos.posZ + (SinFixed(angle) * magnitude >> 0xC);
            magnitude = rand() % 0xB4 + 0x32;
            pSprite->velocityX = CosFixed(angle) * magnitude >> 0xC;
            pSprite->velocityY = -(rand() % 0x3C + 0x14);
            pSprite->velocityZ = SinFixed(angle) * magnitude >> 0xC;
            angle = rand() % 0xA00 + 0x800;
            pSprite->scale = sizeBase * angle >> 0xC;
        }
    }
    if (task->frameCounter == 7) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_WeaponFxEmeraldBeamCentroidPos), 0x241);
    }
    if (task->frameCounter == 0x10) {
        panVal = Battle_ComputeScreenPanFromPosition(&g_WeaponFxEmeraldBeamBonePos);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, panVal, 0x1D2);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH3, panVal, 0x1D2);
    }
    if (task->frameCounter == 0x1A) {
        Battle_PostActorHitOrHurtMessage(g_WeaponFxEmeraldBeamSourceActor);
    }
    if (task->frameCounter >= 0x28) {
        task->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x241, 0xF);
        Sfx_ReleaseCachedBuffer(0x1D2, 0xF);
    }
    (task->frameCounter)++;
}

/*
 * Renders expanding and fading beam burst sprite with sinusoidal transparency.
 */
// FUNCTION: FF7 0x00448152
void __cdecl BattleFx_WeaponEmeraldBeamFadingSpriteTask(void)
{
    unsigned char brightness;
    BattleFxBeamFadeSprite *pSprite;

    pSprite = (BattleFxBeamFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    if (pSprite->frame >= 0xA) {
        brightness = 0x80 - (pSprite->frame - 0xA) * 0x15;
    } else {
        brightness = 0x80;
    }
    g_WeaponFxEmeraldBeamBurstSpriteReq.b = brightness;
    g_WeaponFxEmeraldBeamBurstSpriteReq.g = g_WeaponFxEmeraldBeamBurstSpriteReq.b;
    g_WeaponFxEmeraldBeamBurstSpriteReq.r = g_WeaponFxEmeraldBeamBurstSpriteReq.g;
    BattleFx_InstallBillboardMatrix(&pSprite->posA, (short)pSprite->angle, -(pSprite->angle >> 3));
    (g_WeaponFxEmeraldBeamBurstSpriteReq.sprite) = Gfx_ModelTableGetLoadedRecord(0, g_WeaponFxEmeraldBeamResourceContext);
    g_WeaponFxEmeraldBeamBurstSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(0, g_WeaponFxEmeraldBeamResourceContext);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_WeaponFxEmeraldBeamBurstSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (pSprite->frame >= 0x10) {
        pSprite->endFlag = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    pSprite->posA += pSprite->velA;
    pSprite->posB += pSprite->velB;
    pSprite->posC += pSprite->velC;
    pSprite->velA -= (short)(pSprite->velA >> 3);
    pSprite->velB += (short)(pSprite->velB >> 3);
    pSprite->velC -= (short)(pSprite->velC >> 3);
    (pSprite->frame)++;
}

/*
 * Renders secondary fading beam sprite with fixed delay and sinusoidal fading.
 */
// FUNCTION: FF7 0x004482f1
void __cdecl BattleFx_WeaponEmeraldBeamDelayedSpriteTask(void)
{
    BattleFxWeaponBeamDelayedSprite *task;

    task = (BattleFxWeaponBeamDelayedSprite *)BATTLE_CURRENT_TASK_SLOT;
    if (task->delay > 0) {
        if (g_BattleIsPaused == 0) {
            (task->delay)--;
        }
    } else {
        if (g_WeaponFxEmeraldBeamVariantFlag != 0) {
            g_WeaponFxEmeraldBeamSecondarySpriteReq.r = 0x80;
            g_WeaponFxEmeraldBeamSecondarySpriteReq.g = 0x70;
            g_WeaponFxEmeraldBeamSecondarySpriteReq.b = 0x20;
        } else {
            g_WeaponFxEmeraldBeamSecondarySpriteReq.r = 0x80;
            g_WeaponFxEmeraldBeamSecondarySpriteReq.g = 0x80;
            g_WeaponFxEmeraldBeamSecondarySpriteReq.b = 0x80;
        }
        g_WeaponFxEmeraldBeamSecondarySpriteReq.frame = task->frameCount;
        BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, -(task->scale >> 3));
        (g_WeaponFxEmeraldBeamSecondarySpriteReq.sprite) = Gfx_ModelTableGetLoadedRecord(1, g_WeaponFxEmeraldBeamResourceContext);
        g_WeaponFxEmeraldBeamSecondarySpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(1, g_WeaponFxEmeraldBeamResourceContext);
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_WeaponFxEmeraldBeamSecondarySpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        if (task->frameCount >= 0xF) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
        if (g_BattleIsPaused != 0) {
            return;
        }
        (task->frameCount)++;
    }
}

/*
 * Renders oriented stretching beam cylinder model from source bone toward target centroid.
 */
// FUNCTION: FF7 0x00448422
void __cdecl BattleFx_WeaponEmeraldBeamBeamModelTask(void)
{
    VECTOR a;
    BattleFxModelDraw *requestPtr;
    VECTOR c;
    VECTOR baseCamVec;
    VECTOR rayVector;
    PsxMatrix currMatA;
    SVECTOR euler;
    int h;
    int i;
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int totalLength;
    VECTOR flatVector;
    SVECTOR foot;
    BattleFxWeaponEmeraldBeamBeam *task8;
    PsxMatrix outMat;

    task8 = (BattleFxWeaponEmeraldBeamBeam *)BATTLE_CURRENT_TASK_SLOT;
    flatVector.vx = g_WeaponFxEmeraldBeamBonePos.posX - g_WeaponFxEmeraldBeamCentroidPos.posX;
    flatVector.vy = 0;
    flatVector.vz = g_WeaponFxEmeraldBeamBonePos.posZ - g_WeaponFxEmeraldBeamCentroidPos.posZ;
    h = NormalizeVectorFixed(&flatVector, &flatVector);
    i = ComputeAngleBetweenVectors((VECTOR *)g_WeaponFxEmeraldBeamNegativeXAxisRef, &flatVector, &c);
    Gte_BuildAxisAngleMatrix(i, &currMatA, &c);
    c.vx = g_WeaponFxEmeraldBeamBonePos.posX - g_WeaponFxEmeraldBeamCentroidPos.posX;
    c.vy = g_WeaponFxEmeraldBeamBonePos.posY - g_WeaponFxEmeraldBeamCentroidPos.posY;
    c.vz = g_WeaponFxEmeraldBeamBonePos.posZ - g_WeaponFxEmeraldBeamCentroidPos.posZ;
    totalLength = NormalizeVectorFixed(&c, &c);
    i = ComputeAngleBetweenVectors(&flatVector, &c, &baseCamVec);
    Gte_BuildAxisAngleMatrix(i, &outMat, &baseCamVec);
    ConcatMatrixInPlace(&outMat, &currMatA);
    rayVector.vx = g_WeaponFxEmeraldBeamBonePos.posX - g_WeaponFxEmeraldBeamCentroidPos.posX;
    rayVector.vy = g_WeaponFxEmeraldBeamBonePos.posY - g_WeaponFxEmeraldBeamCentroidPos.posY;
    rayVector.vz = g_WeaponFxEmeraldBeamBonePos.posZ - g_WeaponFxEmeraldBeamCentroidPos.posZ;
    baseCamVec.vx = g_BattleCameraPosition.cameraPosX - g_WeaponFxEmeraldBeamCentroidPos.posX;
    baseCamVec.vy = g_BattleCameraPosition.cameraPosY - g_WeaponFxEmeraldBeamCentroidPos.posY;
    baseCamVec.vz = g_BattleCameraPosZ - g_WeaponFxEmeraldBeamCentroidPos.posZ;
    i = (rayVector.vx * baseCamVec.vx + rayVector.vy * baseCamVec.vy + rayVector.vz * baseCamVec.vz) >> 0xC;
    *(int *)&foot = ((int *)&g_WeaponFxEmeraldBeamCentroidPos)[0];
    *(int *)((char *)&foot + 4) = ((int *)&g_WeaponFxEmeraldBeamCentroidPos)[1];
    foot.vx += (short)(i * c.vx >> 0xC);
    foot.vy += (short)(i * c.vy >> 0xC);
    foot.vz += (short)(i * c.vz >> 0xC);
    baseCamVec.vx = g_BattleCameraPosition.cameraPosX - foot.vx;
    baseCamVec.vy = g_BattleCameraPosition.cameraPosY - foot.vy;
    baseCamVec.vz = g_BattleCameraPosZ - foot.vz;
    NormalizeVectorFixed(&baseCamVec, &baseCamVec);
    i = ComputeAngleBetweenVectors((VECTOR *)g_WeaponFxEmeraldBeamNegativeYAxisRef, &baseCamVec, &rayVector);
    CrossProductFixed12(&c, &baseCamVec, &rayVector);
    if (rayVector.vy > 0) {
        i = -i;
    }
    euler.vx = (short)i;
    euler.vy = 0;
    euler.vz = 0;
    RotMatrixXYZ(&euler, &outMat);
    outMat.t[0] = g_WeaponFxEmeraldBeamCentroidPos.posX;
    outMat.t[1] = g_WeaponFxEmeraldBeamCentroidPos.posY;
    outMat.t[2] = g_WeaponFxEmeraldBeamCentroidPos.posZ;
    ConcatMatrixInPlace(&currMatA, &outMat);
    i = (SqrtToInt(totalLength) << 0xC) / 0x5F5;
    if (task8->stage < 3) {
        i = i / 3 * task8->stage;
    }
    a.vx = i;
    a.vz = task8->thicknessScale;
    a.vy = a.vz;
    ScaleMatrix(&outMat, &a);
    MultiplyMatrix(&g_BattleCameraTransform, &outMat, &outMat);
    SetRotMatrix(&outMat);
    SetTransMatrix(&outMat);
    requestPtr = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    requestPtr->modelGroup = Gfx_ModelTableGetRenderObject((g_WeaponFxEmeraldBeamVariantFlag != 0) + 2, g_WeaponFxEmeraldBeamResourceContext);
    requestPtr->flags = 0x88;
    requestPtr->uvScroll = 0;
    requestPtr->fadeLevel = 0;
    requestPtr->stateFlags = 0;
    requestPtr->texturePageBits = 0;
    if (task8->stage >= 4) {
        requestPtr->fadeLevel = (task8->stage - 4) * 0x2AA;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(requestPtr, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task8->stage >= 0xA) {
        task8->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task8->stage)++;
}

/*
 * Renders shrinking beam core model along the trajectory vector.
 */
// FUNCTION: FF7 0x004488d2
void __cdecl BattleFx_WeaponEmeraldBeamShrinkingModelTask(void)
{
    int padSlot[4]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int padGap[7]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxModelDraw *draw;
    int stackWords[8]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxWeaponBeamShrink *task;

    task = (BattleFxWeaponBeamShrink *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&g_WeaponFxEmeraldBeamCentroidPos.posX, (short)task->length, -(task->length >> 3));
    draw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    draw->modelGroup = Gfx_ModelTableGetRenderObject((g_WeaponFxEmeraldBeamVariantFlag != 0) + 4, g_WeaponFxEmeraldBeamResourceContext);
    draw->flags = 0x88;
    draw->uvScroll = 0;
    draw->fadeLevel = 0;
    draw->stateFlags = 0;
    draw->texturePageBits = 0;
    if (task->frame < 6) {
        draw->fadeLevel = 0x1000 - task->frame * 0x2AA;
    } else if (task->frame >= 0xA) {
        draw->fadeLevel = (task->frame - 0xA) * 0x2AA;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(draw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frame >= 0x10) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frame < 6) {
        task->length -= task->shrinkSpeed;
        task->shrinkSpeed -= (short)(task->shrinkSpeed / 9);
    }
    (task->frame)++;
}
} /* extern "C" */
