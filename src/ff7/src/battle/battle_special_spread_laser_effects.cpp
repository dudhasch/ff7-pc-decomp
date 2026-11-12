// src/ff7/src/battle/battle_special_spread_laser_effects.cpp
/* Enemy Special Attack [140]: Spread Laser (orig. SPECIAL/KAKUSAN, 拡散レーザー) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Kakusan spell. It loads its resources, starts the
//   timeline, and runs the aimed, streak, model, and beam sprite tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct Vec3sPacked;
struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008990f8
unsigned int g_BattleSpecialSpreadLaserResourceDefs[50] = {0x001900b4, (unsigned int)&g_BattleSpecialSpreadLaserModelNames1, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)(unsigned int)&g_BattleSpecialSpreadLaserModelNames1[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecialSpreadLaserModelNames2, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)(unsigned int)&g_BattleSpecialSpreadLaserModelNames2[12], 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialSpreadLaserTextureSpriteNames1, (unsigned int)(unsigned int)&g_BattleSpecialSpreadLaserTextureSpriteNames1[12], 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialSpreadLaserTextureSpriteNames2, (unsigned int)(unsigned int)&g_BattleSpecialSpreadLaserTextureSpriteNames2[12], 0x00000000, 0x00000000, 0x00196a39, (unsigned int)(unsigned int)&g_BattleSpecialSpreadLaserTextureSpriteNames2[20], (unsigned int)&g_BattleSpecialSpreadLaserTextureSpriteNames3, 0x00000000, 0x00000000, 0x00196a39, (unsigned int)(unsigned int)&g_BattleSpecialSpreadLaserTextureSpriteNames3[12], (unsigned int)&g_BattleSpecialSpreadLaserTextureSpriteNames4, 0x00000000, 0x00000000, 0x00196a39, (unsigned int)(unsigned int)&g_BattleSpecialSpreadLaserTextureSpriteNames4[12], (unsigned int)&g_BattleSpecialSpreadLaserTextureSpriteNames5, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)(unsigned int)&g_BattleSpecialSpreadLaserTextureSpriteNames5[12], (unsigned int)&g_BattleSpecialSpreadLaserSpriteName1, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008991c0
struct BattleSpecMizufukiSpriteRequest2 g_BattleSpecialSpreadLaserSpriteConfig1 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};

// GLOBAL: FF7 0x008991d0
struct BattleSpecialSpreadLaserSpriteConfig2 g_BattleSpecialSpreadLaserSpriteConfig2 = {0, 0, -128, -128, -128, {0x2e}, 0};
// GLOBAL: FF7 0x008991e0
struct BattleSpecialSpreadLaserSpriteConfig3 g_BattleSpecialSpreadLaserSpriteConfig3 = {0, 0, -128, -128, -128, {0x2e}, 0};
// GLOBAL: FF7 0x008991f0
int g_BattleSpecialSpreadLaserScaleFixed12_1 = 4096;
// GLOBAL: FF7 0x008991f4
int g_BattleSpecialSpreadLaserScaleFixed12_2 = 4096;
// GLOBAL: FF7 0x008991f8
int g_BattleSpecialSpreadLaserScaleFixed12_3 = 4096;
// GLOBAL: FF7 0x00899200
unsigned char g_BattleSpecialSpreadLaserSpriteIndices1[40] = {0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x08, 0x00, 0x09, 0x00, 0x0a, 0x00, 0x0b, 0x00, 0x0c, 0x00, 0x0d, 0x00, 0x0e, 0x00, 0x0f, 0x00, 0x10, 0x00, 0x0d, 0x00, 0x0e, 0x00, 0x0f, 0x00, 0x10, 0x00};
// GLOBAL: FF7 0x00899228
unsigned char g_BattleSpecialSpreadLaserSpriteIndices2[24] = {0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x08, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x08, 0x00};
// GLOBAL: FF7 0x00899240
unsigned int g_BattleSpecialSpreadLaserSpriteIndicesTable[2] = {(unsigned int)&g_BattleSpecialSpreadLaserSpriteIndices1, (unsigned int)&g_BattleSpecialSpreadLaserSpriteIndices2};
// GLOBAL: FF7 0x00899248
struct BattleSpecialSpreadLaserSpriteConfig4 g_BattleSpecialSpreadLaserSpriteConfig4 = {0, 0, -128, -128, -128, {0x2e}, 0};
// GLOBAL: FF7 0x00899258
struct BattleSpecialSpreadLaserEffectConfig g_BattleSpecialSpreadLaserEffectConfig = {0, {0x28, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_00899264[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00899268
unsigned char g_BattleSpecialSpreadLaserScaleVector[16] = {0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00899278
struct BattleSpecialSpreadLaserSpriteConfig5 g_BattleSpecialSpreadLaserSpriteConfig5 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};

// GLOBAL: FF7 0x00899288
unsigned char g_BattleSpecialSpreadLaserModelOffset[6] = {0x00, 0x00, 0x00, 0xfc, 0x00, 0x00};
// GLOBAL: FF7 0x00899290
unsigned char g_BattleSpecialSpreadLaserModelNames1[22] =
    "GEKI1.RSD\0\0\0"
    "GEKI2.RSD";
// GLOBAL: FF7 0x008992a8
unsigned char g_BattleSpecialSpreadLaserModelNames2[22] =
    "GEKI3.RSD\0\0\0"
    "GEKI4.RSD";
// GLOBAL: FF7 0x008992c0
unsigned char g_BattleSpecialSpreadLaserTextureSpriteNames1[24] =
    "KAKU01.TIM\0\0"
    "HIKARI1.SPT";
// GLOBAL: FF7 0x008992d8
unsigned char g_BattleSpecialSpreadLaserTextureSpriteNames2[31] =
    "KAKU00.TIM\0\0"
    "HAN.SPT\0"
    "KAKU02.TIM";
// GLOBAL: FF7 0x008992f8
unsigned char g_BattleSpecialSpreadLaserTextureSpriteNames3[23] =
    "REZA1.SPT\0\0\0"
    "KAKU03.TIM";
// GLOBAL: FF7 0x00899310
unsigned char g_BattleSpecialSpreadLaserTextureSpriteNames4[23] =
    "BIMU1.SPT\0\0\0"
    "KAKU03.TIM";
// GLOBAL: FF7 0x00899328
unsigned char g_BattleSpecialSpreadLaserTextureSpriteNames5[23] =
    "BIMU2.SPT\0\0\0"
    "KAKU00.TIM";
// GLOBAL: FF7 0x00899340
unsigned char g_BattleSpecialSpreadLaserSpriteName1[9] = "HIT1.SPT";
// clang-format on
// GLOBAL: FF7 0x00b4fb78
PsxMatrix g_BattleSpecialSpreadLaserLookBasis = {0};
// GLOBAL: FF7 0x00b4fb98
struct BattleSpecialScorchCentroidPos g_BattleSpecialSpreadLaserTargetCentroid = {0};
// GLOBAL: FF7 0x00b4fb9c
short g_BattleSpecialSpreadLaserTargetCentroidZ = 0;
unsigned char g_bssPad_00b4fba0[4] = {0};
// GLOBAL: FF7 0x00b4fba4
short g_BattleSpecialSpreadLaserFlag1 = 0;
// GLOBAL: FF7 0x00b4fba8
int g_BattleSpecialSpreadLaserMeshDrawHandle = 0;
// GLOBAL: FF7 0x00b4fbac
short g_BattleSpecialSpreadLaserTargetMask = 0;
// GLOBAL: FF7 0x00b4fbb0
short g_BattleSpecialSpreadLaserDistance = 0;
// GLOBAL: FF7 0x00b4fbb4
short g_BattleSpecialSpreadLaserFlag2 = 0;
// GLOBAL: FF7 0x00b4fbb8
struct GfxTableCursor *g_BattleSpecialSpreadLaserLoadedResTable = 0;
unsigned char g_bssPad_00b4fbbc[4] = {0};
// GLOBAL: FF7 0x00b4fbc0
int g_BattleSpecialSpreadLaserZeroRot = 0;
unsigned char g_bssPad_00b4fbc4[4] = {0};
// clang-format on


// When the engine context exists and g_BattleSpecialSpreadLaserLoadedResTable is empty, builds SPECIAL/KAKUSAN with a default-
// duration record and creates resource type 10 using g_BattleSpecialSpreadLaserResourceDefs.
// FUNCTION: FF7 0x0050e2b0
void __cdecl BattleFx_SpecialLoadResSpreadLaser(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resultDefArr;

    if (g_pBattle3dScene != 0 && g_BattleSpecialSpreadLaserLoadedResTable == 0) {
        kind = 0xA;
        resultDefArr = (int)&g_BattleSpecialSpreadLaserResourceDefs;
        stage = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089934c
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/KAKUSAN", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resultDefArr, pRequest, &g_BattleSpecialSpreadLaserLoadedResTable, stage);
    }
}

// Mechanical entry point for the kakusan effect: sets g_BattleSpecialSpreadLaserFlag1 and g_BattleSpecialSpreadLaserFlag2 to 8, then
// forwards targetMask and actorIdx to BattleFx_SpecialSpreadLaserSpawnTimeline.
// FUNCTION: FF7 0x0050e340
void __cdecl BattleFx_SpecialEnterSpreadLaser(int targetMask, int actorIdx)
{
    g_BattleSpecialSpreadLaserFlag1 = 8;
    g_BattleSpecialSpreadLaserFlag2 = 8;
    BattleFx_SpecialSpreadLaserSpawnTimeline(targetMask, actorIdx);
}

// Loads Kakusan resources, stores the target mask and allocates BattleFx_SpecialSpreadLaserTimelineTask at the actorIdx
// bone selected by its signed bone byte. Finds the target-mask bone centroid, builds a look basis
// from its short-coordinate difference and stores the square-root distance. Dispatches sequence
// 0x131, clears screen color and starts a 0xC00 ramp over ten.
// FUNCTION: FF7 0x0050e367
void __cdecl BattleFx_SpecialSpreadLaserSpawnTimeline(int targetMask, int actorIdx)
{
    struct {
        BattleFxPositionedSpriteSlot *p;
        short direction[3];
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    BattleFx_SpecialLoadResSpreadLaser();
    g_BattleSpecialSpreadLaserTargetMask = targetMask;
    local.p = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSpreadLaserTimelineTask);
    Battle_GetBoneWorldPos(actorIdx,
                           g_BattleActorRecords[actorIdx].bone34,
                           &local.p->posX);
    Battle_GetTargetMaskBoneCentroid(targetMask, (Vec3sPacked *)&g_BattleSpecialSpreadLaserTargetCentroid);
    local.direction[0] = (g_BattleSpecialSpreadLaserTargetCentroid.centroidX) - local.p->posX;
    local.direction[1] = g_BattleSpecialSpreadLaserTargetCentroid.y - local.p->posY;
    local.direction[2] = g_BattleSpecialSpreadLaserTargetCentroidZ - local.p->posZ;
    Gte_BuildLookBasisFromDirection(local.direction, (short *)&g_BattleSpecialSpreadLaserLookBasis);
    g_BattleSpecialSpreadLaserDistance = SqrtToInt(local.direction[0] * local.direction[0] +
                             local.direction[1] * local.direction[1] +
                             local.direction[2] * local.direction[2]);
    Battle_PlaySfxPannedToActor(0x131, (short)actorIdx, 0);
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0xC00, 10);
}

// Per-frame timeline task for the Kakusan special, with the timeline's position taken from posX,
// posY and posZ. When its timer is 0 it allocates 20 streak sprite tasks with random angle, angle
// jitter and variant, plus one beam sprite task, one model sprite task, and one aimed sprite task
// whose angle points from the timeline toward the target centroid. On every even timer below 0x28,
// each target in g_BattleSpecialSpreadLaserTargetMask gets an animated sprite task at its target bone,
// offset by a random amount of about 0x2BC per axis. At timer 0x28, each masked target gets a
// hit-mark script at its actor. The task returns at once while g_BattleIsPaused is set.
// FUNCTION: FF7 0x0050e493
void __cdecl BattleFx_SpecialSpreadLaserTimelineTask(void)
{
    BattleFxSpecialSpreadLaserTimeline *pCurrent;
    short actorIdx;
    BattleFxSpreadLaserRecord *newTaskPtr;
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

    pCurrent = (BattleFxSpecialSpreadLaserTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pCurrent->timer == 0) {
        for (actorIdx = 0; actorIdx < 20; actorIdx++) {
            newTaskPtr = (BattleFxSpreadLaserRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSpreadLaserStreakSpriteTask);
            newTaskPtr->posX = pCurrent->posX;
            newTaskPtr->posY = pCurrent->posY;
            newTaskPtr->posZ = pCurrent->posZ;
            newTaskPtr->travelLength = g_BattleSpecialSpreadLaserDistance * 6144 / 2000;
            newTaskPtr->angleJitter = rand() % 0x200 - 0x100;
            newTaskPtr->angle = rand() % 0x200 + 0x300;
            newTaskPtr->lift = 0x400;
            newTaskPtr->variant = rand() % 8;
            newTaskPtr->side = actorIdx % 2;
        }
        newTaskPtr = (BattleFxSpreadLaserRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSpreadLaserBeamSpriteTask);
        newTaskPtr->posX = pCurrent->posX;
        newTaskPtr->posY = pCurrent->posY;
        newTaskPtr->posZ = pCurrent->posZ;
        newTaskPtr = (BattleFxSpreadLaserRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSpreadLaserModelSpriteTask);
        newTaskPtr->posX = pCurrent->posX;
        newTaskPtr->posY = pCurrent->posY;
        newTaskPtr->posZ = pCurrent->posZ;
        newTaskPtr = (BattleFxSpreadLaserRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSpreadLaserAimedSpriteTask);
        newTaskPtr->posX = pCurrent->posX;
        newTaskPtr->posY = pCurrent->posY;
        newTaskPtr->posZ = pCurrent->posZ;
        newTaskPtr->posX = pCurrent->posX;
        newTaskPtr->posY = 0;
        newTaskPtr->posZ = pCurrent->posZ;
        newTaskPtr->angleJitter = 0x400 /* 90 deg */;
        newTaskPtr->angle = ArcTangentFixed((g_BattleSpecialSpreadLaserTargetCentroid.centroidX) - pCurrent->posX, g_BattleSpecialSpreadLaserTargetCentroidZ - pCurrent->posZ);
        newTaskPtr->lift = 0;
    }
    if (pCurrent->timer < 40 && pCurrent->timer % 2 == 0) {
        for (actorIdx = 0; actorIdx < 10; actorIdx++) {
            if ((g_BattleSpecialSpreadLaserTargetMask & 1 << actorIdx) != 0) {
                newTaskPtr = (BattleFxSpreadLaserRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSpreadLaserAnimatedSpriteTask);
                Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, (short *)((int)newTaskPtr + 4));
                newTaskPtr->posX += (short)(rand() % 1400 - 700);
                newTaskPtr->posY += (short)(rand() % 1400 - 700);
                newTaskPtr->posZ += (short)(rand() % 1400 - 700);
                newTaskPtr->side = 1;
            }
        }
    }
    if (pCurrent->timer == 40) {
        for (actorIdx = 0; actorIdx < 10; actorIdx++) {
            if ((g_BattleSpecialSpreadLaserTargetMask & 1 << actorIdx) != 0) {
                BattleFx_HitMarkSpawnScriptAtActorBone(actorIdx, 0x3A, 0x1000 /* 1.0 */);
                Battle_PostActorHitOrHurtMessage(actorIdx);
            }
        }
        Battle_PlaySfxPannedToActor(0x95, -1, 0x40);
    }
    (pCurrent->timer)++;
    if (pCurrent->timer == 50) {
        Battle_StartFixedPointRamp(0, 20);
        pCurrent->step = 0xFFFF;
    }
}

// Per-frame step for the record at index g_BattleActiveTaskSlotIdx of the table at g_BattleHitLivewireEmitterRecords. It fills
// g_BattleSpecialSpreadLaserSpriteConfig1/g_BattleSpecialSpreadLaserSpriteConfig1.drawObj from resource 9 of g_BattleSpecialSpreadLaserLoadedResTable, installs a billboard matrix from the
// position at scale 0x2000, copies the frame counter into g_BattleSpecialSpreadLaserSpriteConfig1.frame, and submits
// into OT bucket g_BattleCurrentOrderingTable+0x70 at shift 0xC through the packet cursor g_BattleSpecialSpreadLaserMeshDrawHandle. While
// g_BattleIsPaused is zero and the resource is non-null the frame advances and the record is retired
// (ownerSlot -1) at the resource's frame count, the int at offset 4 of its header.
// FUNCTION: FF7 0x0050e8cf
void __cdecl BattleFx_SpecialSpreadLaserAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialSpreadLaserSpriteConfig1.spriteResource) = Gfx_ModelTableGetLoadedRecord(9, g_BattleSpecialSpreadLaserLoadedResTable);
    g_BattleSpecialSpreadLaserSpriteConfig1.drawObj = Gfx_ModelTableGetSpriteObject(9, g_BattleSpecialSpreadLaserLoadedResTable);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2000 /* 2.0 */, 0);
    g_BattleSpecialSpreadLaserSpriteConfig1.frame = task->frameCounter;
    g_BattleSpecialSpreadLaserMeshDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSpreadLaserSpriteConfig1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSpreadLaserMeshDrawHandle);
    if (g_BattleIsPaused == 0 && (g_BattleSpecialSpreadLaserSpriteConfig1.spriteResource) != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)(g_BattleSpecialSpreadLaserSpriteConfig1.spriteResource) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Per-frame task for the aimed Kakusan sprite. It loads sprite resource table 5 and zeroes the
// short transform columns xformCol0 to xformCol2. It projects the sprite's position vector through
// the camera transform. It then builds a matrix from a 0x2000 scale, the camera transform and the
// sprite's rotation angles, and sets it as the current rotation and translation. A sprite draw is
// submitted with flag1 shifted by 4 and frameIndex mod 3 as the index word. Only when not paused
// (the draw above still runs) does g_BattleSpecialSpreadLaserFlag1 drop by one once frameIndex passes
// 0x20, frameIndex advance, and ownerSlot get set to 0xFFFF at frame 0x28.
// FUNCTION: FF7 0x0050e9a0
void __cdecl BattleFx_SpecialSpreadLaserAimedSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; PsxMatrix xformCol2; };
        struct { unsigned char pad_to_40[0x40]; short axisIndex; };
        struct { unsigned char pad_to_44[0x44]; int projectedDepth; };
        struct { unsigned char pad_to_48[0x48]; BattleTaskSlot *aimedSprite; };
    } frame;
#pragma pack(pop)

    frame.aimedSprite = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialSpreadLaserSpriteConfig2.spriteResField) = Gfx_ModelTableGetLoadedRecord(5, g_BattleSpecialSpreadLaserLoadedResTable);
    g_BattleSpecialSpreadLaserSpriteConfig2.config2ResField = Gfx_ModelTableGetSpriteObject(5, g_BattleSpecialSpreadLaserLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.xformCol2.m[frame.axisIndex][2] = 0;
        frame.xformCol2.m[frame.axisIndex][1] = 0;
        frame.xformCol2.m[frame.axisIndex][0] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(frame.aimedSprite->data, frame.xformCol2.t, &frame.projectedDepth);
    frame.xformCol2.m[2][2] = 0x2000;
    frame.xformCol2.m[1][1] = frame.xformCol2.m[2][2];
    frame.xformCol2.m[0][0] = frame.xformCol2.m[1][1];
    ConcatMatrixIntoFirst(&frame.xformCol2, &g_BattleCameraTransform);
    RotMatrix((SVECTOR *)((int)frame.aimedSprite + 0xC), &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.xformCol2, &frame.rotMatrix);
    SetRotMatrix(&frame.xformCol2);
    SetTransMatrix(&frame.xformCol2);
    g_BattleSpecialSpreadLaserSpriteConfig2.config2Byte3 = g_BattleSpecialSpreadLaserFlag1 << 4;
    g_BattleSpecialSpreadLaserSpriteConfig2.config2Byte2 = g_BattleSpecialSpreadLaserSpriteConfig2.config2Byte3;
    g_BattleSpecialSpreadLaserSpriteConfig2.config2Byte1 = g_BattleSpecialSpreadLaserSpriteConfig2.config2Byte2;
    g_BattleSpecialSpreadLaserSpriteConfig2.config2IndexWord = frame.aimedSprite->frameCounter % 3;
    g_BattleSpecialSpreadLaserMeshDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSpreadLaserSpriteConfig2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSpreadLaserMeshDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.aimedSprite->frameCounter > 32) {
        g_BattleSpecialSpreadLaserFlag1--;
    }
    (frame.aimedSprite->frameCounter)++;
    if (frame.aimedSprite->frameCounter == 40) {
        frame.aimedSprite->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame task of the Kakusan streak sprite.
// FUNCTION: FF7 0x0050eb76
void __cdecl BattleFx_SpecialSpreadLaserStreakSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[128];
        int fadeLevel;
        struct { unsigned char pad_to_34[0x34]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_54[0x54]; short fadeWord; };
        struct { unsigned char pad_to_58[0x58]; short stepLimit; };
        struct { unsigned char pad_to_5C[0x5C]; BattleFxSpreadLaserStreakSpriteSlot *streakSprite; };
        struct { unsigned char pad_to_60[0x60]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.streakSprite = (BattleFxSpreadLaserStreakSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.streakSprite->frameCounter < 32) {
        frame.fadeLevel = 8;
    } else {
        frame.fadeLevel = 40 - frame.streakSprite->frameCounter;
    }
    frame.fadeWord = *(short *)&frame.fadeLevel;
    RotMatrixXYZ(&frame.streakSprite->rotX, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialSpreadLaserLookBasis, &frame.rotMatrix, &frame.rotMatrix);
    g_BattleSpecialSpreadLaserScaleFixed12_2 = frame.streakSprite->scale;
    g_BattleSpecialSpreadLaserScaleFixed12_3 = 0x3000;
    g_BattleSpecialSpreadLaserScaleFixed12_1 = g_BattleSpecialSpreadLaserScaleFixed12_3;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialSpreadLaserScaleFixed12_1);
    frame.rotMatrix.t[0] = frame.streakSprite->posX;
    frame.rotMatrix.t[1] = frame.streakSprite->posY;
    frame.rotMatrix.t[2] = frame.streakSprite->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialSpreadLaserSpriteConfig3.config3IndexShort = *(short *)((char *)g_BattleSpecialSpreadLaserSpriteIndicesTable[frame.streakSprite->direction] + frame.streakSprite->stepCount * 2);
    if (frame.streakSprite->direction == 0) {
        (g_BattleSpecialSpreadLaserSpriteConfig3.spriteResource) = Gfx_ModelTableGetLoadedRecord(7, g_BattleSpecialSpreadLaserLoadedResTable);
        g_BattleSpecialSpreadLaserSpriteConfig3.config3ResField = Gfx_ModelTableGetSpriteObject(7, g_BattleSpecialSpreadLaserLoadedResTable);
    } else {
        (g_BattleSpecialSpreadLaserSpriteConfig3.spriteResource) = Gfx_ModelTableGetLoadedRecord(8, g_BattleSpecialSpreadLaserLoadedResTable);
        g_BattleSpecialSpreadLaserSpriteConfig3.config3ResField = Gfx_ModelTableGetSpriteObject(8, g_BattleSpecialSpreadLaserLoadedResTable);
    }
    g_BattleSpecialSpreadLaserSpriteConfig3.config3Byte3 = frame.fadeWord << 4;
    g_BattleSpecialSpreadLaserSpriteConfig3.config3Byte2 = g_BattleSpecialSpreadLaserSpriteConfig3.config3Byte3;
    g_BattleSpecialSpreadLaserSpriteConfig3.config3Byte1 = g_BattleSpecialSpreadLaserSpriteConfig3.config3Byte2;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSpreadLaserSpriteConfig3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.stepLimit = (frame.streakSprite->direction != 0 ? 0xFFFFFFF8 : 0) + 20;
    (frame.streakSprite->stepCount)++;
    if (frame.streakSprite->stepCount == frame.stepLimit) {
        frame.streakSprite->stepCount -= 4;
    }
    (frame.streakSprite->frameCounter)++;
    if (frame.streakSprite->frameCounter == 40) {
        frame.streakSprite->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame task of the Kakusan model sprite.
// FUNCTION: FF7 0x0050edae
void __cdecl BattleFx_SpecialSpreadLaserModelSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; int frameIdx0[4]; };
        struct { unsigned char pad_to_30[0x30]; BattleFxOilTargetModelSlot *modelSprite; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.modelSprite = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialSpreadLaserSpriteConfig4.spriteResField) = Gfx_ModelTableGetLoadedRecord(4, g_BattleSpecialSpreadLaserLoadedResTable);
    g_BattleSpecialSpreadLaserSpriteConfig4.config4ResField = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecialSpreadLaserLoadedResTable);
    frame.frameIdx0[0] = 0;
    frame.frameIdx0[1] = 1;
    frame.frameIdx0[2] = 2;
    frame.frameIdx0[3] = 3;
    RotMatrix((SVECTOR *)&g_BattleSpecialSpreadLaserZeroRot, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialSpreadLaserScaleVector);
    frame.rotMatrix.t[0] = frame.modelSprite->posX;
    frame.rotMatrix.t[1] = frame.modelSprite->posY;
    frame.rotMatrix.t[2] = frame.modelSprite->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialSpreadLaserEffectConfig.flag2Shifted = g_BattleSpecialSpreadLaserFlag2 << 4;
    (g_BattleSpecialSpreadLaserEffectConfig.modelResField) = Gfx_ModelTableGetRenderObject(frame.frameIdx0[((frame.modelSprite->frameCounter) % 4)], g_BattleSpecialSpreadLaserLoadedResTable);
    g_BattleSpecialSpreadLaserMeshDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSpreadLaserEffectConfig, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSpreadLaserMeshDrawHandle);
    BattleFx_InstallBillboardMatrix((void *)((int)frame.modelSprite + 4), 0x2000 /* 2.0 */, 0);
    g_BattleSpecialSpreadLaserSpriteConfig4.config4Byte3 = g_BattleSpecialSpreadLaserFlag2 << 4;
    g_BattleSpecialSpreadLaserSpriteConfig4.config4Byte2 = g_BattleSpecialSpreadLaserSpriteConfig4.config4Byte3;
    g_BattleSpecialSpreadLaserSpriteConfig4.config4Byte1 = g_BattleSpecialSpreadLaserSpriteConfig4.config4Byte2;
    g_BattleSpecialSpreadLaserSpriteConfig4.config4FrameParity = ((frame.modelSprite->frameCounter) % 2);
    g_BattleSpecialSpreadLaserMeshDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSpreadLaserSpriteConfig4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSpreadLaserMeshDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.modelSprite->frameCounter > 32) {
        g_BattleSpecialSpreadLaserFlag2--;
    }
    (frame.modelSprite->frameCounter)++;
    if (frame.modelSprite->frameCounter == 40) {
        frame.modelSprite->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame task for a Kakusan beam sprite. It loads sprite resource table 6 and builds a rotation
// from g_BattleSpecialSpreadLaserModelOffset, concatenated with the look basis. It scales by
// (g_BattleSpecialSpreadLaserDistance << 12) / 0x7D0 on X and by 0x4000 on Y and Z. It copies the
// sprite's position into the frame, multiplies with g_BattleCameraTransform, and draws with
// frameCounter as the config short. Only when not paused (the draw above still runs), and only
// while a sprite resource is loaded, does frameCounter advance. ownerSlot is set to 0xFFFF once
// frameCounter reaches the count stored in that resource.
// FUNCTION: FF7 0x0050efab
void __cdecl BattleFx_SpecialSpreadLaserBeamSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxOilTargetModelSlot *beamSprite; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.beamSprite = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialSpreadLaserSpriteConfig5.spriteResource) = Gfx_ModelTableGetLoadedRecord(6, g_BattleSpecialSpreadLaserLoadedResTable);
    g_BattleSpecialSpreadLaserSpriteConfig5.config5ResField = Gfx_ModelTableGetSpriteObject(6, g_BattleSpecialSpreadLaserLoadedResTable);
    RotMatrixXYZ(&g_BattleSpecialSpreadLaserModelOffset, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialSpreadLaserLookBasis, &frame.rotMatrix, &frame.rotMatrix);
    frame.scaleX.vx = (g_BattleSpecialSpreadLaserDistance << 12) / 2000;
    frame.scaleX.vz = 0x4000 /* 4.0 */;
    frame.scaleX.vy = frame.scaleX.vz;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.beamSprite->posX;
    frame.rotMatrix.t[1] = frame.beamSprite->posY;
    frame.rotMatrix.t[2] = frame.beamSprite->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialSpreadLaserSpriteConfig5.config5Short = frame.beamSprite->frameCounter;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSpreadLaserSpriteConfig5, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if ((g_BattleSpecialSpreadLaserSpriteConfig5.spriteResource) != 0) {
        (frame.beamSprite->frameCounter)++;
        if (frame.beamSprite->frameCounter >= *(int *)((char *)*(int *)(g_BattleSpecialSpreadLaserSpriteConfig5.spriteResource) + 4)) {
            frame.beamSprite->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
