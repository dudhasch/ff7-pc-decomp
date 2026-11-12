// src/ff7/src/battle/battle_fx_limit_lucky_girl.cpp
// Limit Break [59] (Cait Sith Slots): Lucky Girl (Japanese: ラッキーガール, dev tag: "limit/lgirl" / Lgirl)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Lgirl limit break effect: a timeline schedules fading billboard sprites, trailing
//   sprites that follow a stored position history, and sine-scaled sprites.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(memset)

// clang-format off
// GLOBAL: FF7 0x0082c318
int g_BattleLimitLuckyGirlResourceDef = 0x00192A39;
int g_BattleLimitLuckyGirlResourceDefHeadPtr[1] = {
    (int)((char *)&g_BattleFxLimitLuckyGirlResourceNames + 0x0)
};
// clang-format on
// The rest of the two resource descriptors g_BattleLimitLuckyGirlResourceDef starts, then the texture
// and CLUT of the fading billboard sprite's draw request; its colour is g_BattleLimitLuckyGirlColorData.
typedef struct {
    int resourceDefTail[8];
    int texture;
    int clut;
} BattleLimitLgirlModelReq;
// clang-format off
// GLOBAL: FF7 0x0082c320
BattleLimitLgirlModelReq g_BattleLimitLuckyGirlModelReq = {
    {(int)((char *)&g_BattleFxLimitLuckyGirlResourceNames + 0xc), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxLimitLuckyGirlResourceNames + 0x18), (int)((char *)&g_BattleFxLimitLuckyGirlResourceNames + 0x24),
    0x0, 0x0}, 0x0, 0x0
};
// GLOBAL: FF7 0x0082c348
unsigned char g_BattleLimitLuckyGirlColorData[4] = {0x80, 0x80, 0x80, 0x2e};
// GLOBAL: FF7 0x0082c350
BattleLimitLgirlSpriteRequest g_BattleLimitLuckyGirlTrailSpriteReq = {0, 0, {128, 128, 128, 46}, 0, 0};
// The sine-scaled sprite's draw request.
// GLOBAL: FF7 0x0082c360
BattleLimitLgirlSpriteRequest g_BattleLimitLuckyGirlSineSpriteReq = {0, 0, {128, 128, 128, 46}, 0, 0};
unsigned char g_BattleFxLimitLuckyGirlResourceNames[48] =
    "lucky00.tim\0"
    "lucky.SPT\0\0\0"
    "heart01.tim\0"
    "heart.SPT";
// The trail's ring header as a BattleLimitLuckyGirlTrail (index, readIndex, count, capacity, writeIndex,
// prevWriteIndex), then its six position addresses into g_BattleLimitLuckyGirlTrailPositions.
// GLOBAL: FF7 0x00b038c8
BattleLimitLuckyGirlTrail g_BattleLimitLuckyGirlTrailHistory[2] = {0};
// The source actor's target bone, raised by 800: where the effect starts.
// GLOBAL: FF7 0x00b03938
struct Int64Pair g_BattleLimitLuckyGirlSourcePos = {0};
// The trailing sprite's last six positions, one 8-byte point each.
// GLOBAL: FF7 0x00b03940
Int64Pair g_BattleLimitLuckyGirlTrailPositions[6] = {0};
unsigned char g_bssPad_00b03970[80] = {0};
// GLOBAL: FF7 0x00b039c0
int g_BattleLimitLuckyGirlTargetActorMask = 0;
// GLOBAL: FF7 0x00b039c4
int g_BattleLimitLuckyGirlSourceActor = 0;
// GLOBAL: FF7 0x00b039c8
struct GfxTableCursor *g_BattleLimitLuckyGirlLoadedResTable = 0;
unsigned char g_bssPad_00b039cc[4] = {0};
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Loads the limit/lgirl resources once the battle scene exists and they are not loaded yet.
 */
// FUNCTION: FF7 0x004c3f80
void __cdecl BattleFx_LimitLoadResLuckyGirl(void)
{
    GfxModelTableBank *drawDev;
    GfxResourceRequest requestSlot;
    int kindId;
    char loadPath[200];
    GfxResourceRequest *pRequest;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_BattleLimitLuckyGirlLoadedResTable == 0) {
        kindId = 2;
        stepResourceDef = (int)&g_BattleLimitLuckyGirlResourceDef;
        drawDev = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0082c3a0
        pRequest->resourcePath = BattleFx_BuildResourcePath("limit/lgirl", loadPath);
        Gfx_LoadModelTableIfAbsent(kindId, stepResourceDef, pRequest, &g_BattleLimitLuckyGirlLoadedResTable, drawDev);
    }
}

/*
 * Calls BattleFx_LimitLoadResLuckyGirl, then BattleFx_LimitBeginLuckyGirl with both arguments, and returns zero.
 */
// FUNCTION: FF7 0x004c4010
int __cdecl BattleFx_LimitEnterLuckyGirl(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResLuckyGirl();
    BattleFx_LimitBeginLuckyGirl(targetMask, actorIdx);
    return 0;
}

/*
 * Stores the allocated BattleFx_SelectDrawListBuffer slot address in g_BattleFxDrawListBufferTaskSlotPtr, allocates a
 * slot for BattleFx_LimitLuckyGirlSpawnTimeline, and stores the arguments in g_BattleLimitLuckyGirlTargetActorMask and g_BattleLimitLuckyGirlSourceActor.
 */
// FUNCTION: FF7 0x004c402c
void __cdecl BattleFx_LimitBeginLuckyGirl(int targetMask, int actorIdx)
{
    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_LimitLuckyGirlSpawnTimeline);
    g_BattleLimitLuckyGirlTargetActorMask = targetMask;
    g_BattleLimitLuckyGirlSourceActor = actorIdx;
}

/*
 * Retires the current slot, captures the source actor's bone position into g_BattleLimitLuckyGirlSourcePos,
 * raises it by 800 (Y += 0xFCE0), and allocates BattleFx_LimitLuckyGirlTimelineTask.
 */
// FUNCTION: FF7 0x004c406d
void __cdecl BattleFx_LimitLuckyGirlSpawnTimeline(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_BattleLimitLuckyGirlSourceActor, g_BattleActorRecords[g_BattleLimitLuckyGirlSourceActor].targetBone, ((short *)&g_BattleLimitLuckyGirlSourcePos));
    ((short *)&g_BattleLimitLuckyGirlSourcePos)[1] += 0xFCE0;
    task = BATTLE_NEW_TASK_SLOT(BattleFx_LimitLuckyGirlTimelineTask);
}

/*
 * When unpaused, spawns a fading billboard sprite at g_BattleLimitLuckyGirlSourcePos at frame six and a
 * view-relative trailing sprite at fourteen, resetting the trail's ring (capacity six) and playing
 * sound 0x10D. At thirty spawns a sine-scaled sprite at each target's bone and plays sound 0x11C.
 * At fifty-five creates each target's typed record (the hit), retires from sixty and advances.
 */
// FUNCTION: FF7 0x004c40e5
void __cdecl BattleFx_LimitLuckyGirlTimelineTask(void)
{
    int q;
    int actorIdx;
    int r;
    BattleLimitLuckyGirlSprite *sprite;
    BattleTaskSlot *task;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = BATTLE_CURRENT_TASK_SLOT;
    if (task->frameCounter == 6) {
        sprite = (BattleLimitLuckyGirlSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitLuckyGirlFadingBillboardSpriteTask);
        *(Int64Pair *)&sprite->posX = *(Int64Pair *)&g_BattleLimitLuckyGirlSourcePos;
        sprite->scale = 0xC00 /* 0.75 */;
    }
    if (task->frameCounter == 14) {
        sprite = (BattleLimitLuckyGirlSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitLuckyGirlTrailingSpriteTask);
        sprite->posX = 230;
        sprite->posY = 0xFEF2;
        sprite->posZ = 0xFDA8;
        sprite->bobAmplitude = 0xC8;
        *(Int64Pair *)&sprite->orbitX = *(Int64Pair *)&sprite->posX;
        sprite->orbitRadius = 60;
        sprite->scaleRate = 0x199;
        sprite->scale = 0x199;
        sprite->angle = 0x800 /* 180 deg */;
        sprite->angleStep = 0x1F4;
        sprite->historyIndex = 0;
        /* count, readIndex and index start at 0, writeIndex at -1, capacity at 6 */
        g_BattleLimitLuckyGirlTrailHistory[0].count = 0;
        g_BattleLimitLuckyGirlTrailHistory[0].readIndex = g_BattleLimitLuckyGirlTrailHistory[0].count;
        g_BattleLimitLuckyGirlTrailHistory[0].index = g_BattleLimitLuckyGirlTrailHistory[0].readIndex;
        g_BattleLimitLuckyGirlTrailHistory[0].writeIndex = -1;
        g_BattleLimitLuckyGirlTrailHistory[0].capacity = 6;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(((short *)&g_BattleLimitLuckyGirlSourcePos)), 0x10D);
    }
    if (task->frameCounter == 30) {
        for (actorIdx = 0; actorIdx < 10; actorIdx++) {
            if ((g_BattleLimitLuckyGirlTargetActorMask & 1 << actorIdx) != 0) {
                sprite = (BattleLimitLuckyGirlSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitLuckyGirlSineScaledSpriteTask);
                Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, &sprite->posX);
                sprite->scale = 0;
                sprite->scaleRate = 0x4B00 /* 4.6875 */;
                sprite->angleStep = 0x15E;
                sprite->angle = 0x15E;
            }
        }
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleLimitLuckyGirlTargetActorMask), 0x11C);
    }
    if (task->frameCounter == 55) {
        for (actorIdx = 0; actorIdx < 10; actorIdx++) {
            if ((g_BattleLimitLuckyGirlTargetActorMask & 1 << actorIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(actorIdx);
            }
        }
    }
    if (task->frameCounter >= 60) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Draws sprite resource entry zero at task position with stored scale and a forward offset of
 * negative scale divided by eight. Replicates gray rising over eight counts, holding through
 * forty-one and falling afterward; selects opcode 0x2E during ramps and 0x2C during the hold,
 * leaving the parallel local flag unused. Drawing and retirement from fifty occur while paused;
 * unpaused calls advance.
 */
// FUNCTION: FF7 0x004c4372
void __cdecl BattleFx_LimitLuckyGirlFadingBillboardSpriteTask(void)
{
    unsigned char level;
    int q;
    int r;
    int s;
    int ramping;
    BattleFxLuckyGirlFadeSlot *task;

    task = (BattleFxLuckyGirlFadeSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (task->timer < 8) {
        g_BattleLimitLuckyGirlColorData[3] = 0x2E;
        ramping = 1;
        level = task->timer << 4;
    } else if (task->timer >= 42) {
        g_BattleLimitLuckyGirlColorData[3] = 0x2E;
        ramping = 1;
        level = 0x80 - (task->timer - 0x2A << 4);
    } else {
        g_BattleLimitLuckyGirlColorData[3] = 0x2C;
        ramping = 0;
        level = 0x80;
    }
    g_BattleLimitLuckyGirlColorData[2] = level;
    g_BattleLimitLuckyGirlColorData[1] = g_BattleLimitLuckyGirlColorData[2];
    g_BattleLimitLuckyGirlColorData[0] = g_BattleLimitLuckyGirlColorData[1];
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, -(task->scale >> 3));
    g_BattleLimitLuckyGirlModelReq.texture = Gfx_ModelTableGetLoadedRecord(0, g_BattleLimitLuckyGirlLoadedResTable);
    g_BattleLimitLuckyGirlModelReq.clut = Gfx_ModelTableGetSpriteObject(0, g_BattleLimitLuckyGirlLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleLimitLuckyGirlModelReq.texture, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->timer >= 50) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->timer)++;
}

/*
 * Unpaused calls append task position to its selected circular history before drawing. Builds a
 * view-relative matrix and draws sprite entry one with six-count gray ramps around a hold, then
 * draws previous positions with scale and gray repeatedly reduced by integer division by three
 * then doubled. Drawing and retirement from twenty occur while paused. Unpaused calls advance
 * accelerated X/base-Y motion, sine Y offset, scale and phase, then advance age.
 */
// FUNCTION: FF7 0x004c44c2
void __cdecl BattleFx_LimitLuckyGirlTrailingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[128];
        PsxMatrix viewMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxLuckyGirlTrail *trailTask; };
        struct { unsigned char pad_to_24[0x24]; BattleFxSourcePoint *trailPoint; };
        struct { unsigned char pad_to_28[0x28]; BattleLimitLuckyGirlTrail *history; };
        struct { unsigned char pad_to_2C[0x2C]; int trailScale; };
        struct { unsigned char pad_to_30[0x30]; int tmpLimit; };
        struct { unsigned char pad_to_34[0x34]; int trailStep; };
        struct { unsigned char pad_to_38[0x38]; Int64Pair *historyPairBase; };
        struct { unsigned char pad_to_3C[0x3C]; int prevIndex; };
        struct { unsigned char pad_to_40[0x40]; int histIndex; };
        struct { unsigned char pad_to_44[0x44]; SVECTOR rotX; };
        struct { unsigned char pad_to_4C[0x4C]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_6C[0x6C]; int gray; };
        struct { unsigned char pad_to_70[0x70]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.trailTask = (BattleFxLuckyGirlTrail *)BATTLE_CURRENT_TASK_SLOT;
    frame.history = &g_BattleLimitLuckyGirlTrailHistory[frame.trailTask->historyIndex];
    frame.historyPairBase = (Int64Pair *)((frame.trailTask->historyIndex << 6) + (char *)&g_BattleLimitLuckyGirlTrailPositions);
    if (g_BattleIsPaused == 0) {
        frame.historyPairBase[frame.history->index] = *(Int64Pair *)&frame.trailTask->posX;
        BattleFx_LimitLuckyGirlPushTrailPosition(frame.history, (int)&frame.historyPairBase[frame.history->index]);
    }
    if (frame.trailTask->phase < 6) {
        g_BattleLimitLuckyGirlTrailSpriteReq.colour[3] = 0x2E;
        frame.tmpLimit = 1;
        *(char *)&frame.gray = frame.trailTask->phase * 0x15;
    } else if (frame.trailTask->phase >= 14) {
        g_BattleLimitLuckyGirlTrailSpriteReq.colour[3] = 0x2E;
        frame.tmpLimit = 1;
        *(char *)&frame.gray = 0x80 - (frame.trailTask->phase - 0xE) * 0x15;
    } else {
        g_BattleLimitLuckyGirlTrailSpriteReq.colour[3] = 0x2C;
        frame.tmpLimit = 0;
        *(char *)&frame.gray = 0x80;
    }
    g_BattleLimitLuckyGirlTrailSpriteReq.colour[2] = *(char *)&frame.gray;
    g_BattleLimitLuckyGirlTrailSpriteReq.colour[1] = g_BattleLimitLuckyGirlTrailSpriteReq.colour[2];
    g_BattleLimitLuckyGirlTrailSpriteReq.colour[0] = g_BattleLimitLuckyGirlTrailSpriteReq.colour[1];
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.viewMatrix);
    BattleFx_MakeViewSpacePointPushed(((short *)&g_BattleLimitLuckyGirlSourcePos), 0, (Vec3sPacked *)&frame.viewMatrix);
    *(PsxMatrix *)(&frame.drawMatrix) = *(PsxMatrix *)(&frame.viewMatrix);
    frame.scaleX.vz = frame.trailTask->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.drawMatrix, &frame.scaleX);
    frame.drawMatrix.t[0] = frame.trailTask->posX;
    frame.drawMatrix.t[1] = frame.trailTask->posY;
    frame.drawMatrix.t[2] = frame.trailTask->posZ - (frame.trailTask->scale >> 3);
    MultiplyMatrix(&frame.viewMatrix, &frame.drawMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleLimitLuckyGirlTrailSpriteReq.texture = Gfx_ModelTableGetLoadedRecord(1, g_BattleLimitLuckyGirlLoadedResTable);
    g_BattleLimitLuckyGirlTrailSpriteReq.clut = Gfx_ModelTableGetSpriteObject(1, g_BattleLimitLuckyGirlLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleLimitLuckyGirlTrailSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.history->count > 1) {
        frame.histIndex = frame.history->prevWriteIndex;
        frame.tmpLimit = frame.history->count - 1;
        frame.trailScale = frame.trailTask->scale;
        for (frame.trailStep = 1; frame.trailStep <= frame.tmpLimit; frame.trailStep++, frame.histIndex--) {
            if (frame.histIndex < 0) {
                frame.histIndex = frame.history->capacity - 1;
            }
            frame.prevIndex = frame.histIndex - 1;
            if (frame.prevIndex < 0) {
                frame.prevIndex = frame.history->capacity - 1;
            }
            frame.trailPoint = (BattleFxSourcePoint *)frame.history->positions[frame.histIndex];
            *(PsxMatrix *)(&frame.drawMatrix) = *(PsxMatrix *)(&frame.viewMatrix);
            frame.trailScale = frame.trailScale / 3 << 1;
            frame.scaleX.vz = frame.trailScale;
            frame.scaleX.vy = frame.scaleX.vz;
            frame.scaleX.vx = frame.scaleX.vy;
            ScaleMatrix(&frame.drawMatrix, &frame.scaleX);
            frame.drawMatrix.t[0] = frame.trailPoint->x;
            frame.drawMatrix.t[1] = frame.trailPoint->y;
            frame.drawMatrix.t[2] = frame.trailPoint->z;
            MultiplyMatrix(&frame.viewMatrix, &frame.drawMatrix, &frame.drawMatrix);
            SetRotMatrix(&frame.drawMatrix);
            SetTransMatrix(&frame.drawMatrix);
            *(char *)&frame.gray = (frame.gray & 0xFF) / 3 << 1;
            g_BattleLimitLuckyGirlTrailSpriteReq.colour[2] = *(char *)&frame.gray;
            g_BattleLimitLuckyGirlTrailSpriteReq.colour[1] = g_BattleLimitLuckyGirlTrailSpriteReq.colour[2];
            g_BattleLimitLuckyGirlTrailSpriteReq.colour[0] = g_BattleLimitLuckyGirlTrailSpriteReq.colour[1];
            g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleLimitLuckyGirlTrailSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        }
    }
    if (frame.trailTask->phase >= 20) {
        frame.trailTask->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.trailTask->orbitX += (short)-(frame.trailTask->orbitRadius * SinFixed(1300) >> 12);
    frame.trailTask->orbitY += (short)(frame.trailTask->orbitRadius * CosFixed(1300) >> 12);
    frame.trailTask->orbitRadius += (short)(frame.trailTask->orbitRadius >> 4);
    frame.trailTask->posX = frame.trailTask->orbitX;
    frame.trailTask->posY = frame.trailTask->orbitY + (frame.trailTask->bobAmplitude * SinFixed(frame.trailTask->angle) >> 12);
    frame.trailTask->bobAmplitude += (short)(frame.trailTask->bobAmplitude >> 4);
    frame.trailTask->scale += frame.trailTask->scaleRate;
    frame.trailTask->angle += frame.trailTask->angleStep;
    frame.trailTask->angleStep -= (short)(frame.trailTask->angleStep >> 4);
    (frame.trailTask->phase)++;
}

// Pushes a position into the Lgirl trail ring buffer. It is the same push as
// BattleFx_SummonHadesPushPositionHistory against a header one word longer: before the live index
// is saved into writeIndex, the old writeIndex moves into prevWriteIndex, so two generations of the
// previous write index are kept. The position goes in at the write index, and writeIndex and
// count advance with a wrap at capacity. When the write index catches readIndex, the oldest entry
// is dropped by advancing readIndex and decrementing count.
// FUNCTION: FF7 0x004c499e
void __cdecl BattleFx_LimitLuckyGirlPushTrailPosition(BattleLimitLuckyGirlTrail *trail, int position)
{
    int writeIndex;
    int readIndex;
    int capacity;
    int bytes;

    trail->prevWriteIndex = trail->writeIndex;
    trail->writeIndex = trail->index;
    writeIndex = trail->writeIndex;
    readIndex = trail->readIndex;
    bytes = trail->count;
    capacity = trail->capacity;
    trail->positions[writeIndex] = position;
    writeIndex++;
    bytes++;
    if (writeIndex >= capacity) {
        writeIndex = 0;
    }
    if (bytes >= capacity) {
        bytes = capacity;
    }
    if (writeIndex == readIndex) {
        readIndex++;
        bytes--;
        if (readIndex >= capacity) {
            readIndex = 0;
        }
    }
    trail->index = writeIndex;
    trail->readIndex = readIndex;
    trail->count = bytes;
    trail->capacity = capacity;
}

/*
 * Uses equal color bytes 0x80 below counter six and a byte-converted linear decline afterward,
 * installs a billboard with slot scale and negative scale-eighth third argument, selects sprite
 * field one and draws. Retires at counter twelve or above before the pause guard; when unpaused
 * recomputes scale from sine, advances its angle and reduces angular step by its signed quotient
 * divided by six, then advances the counter.
 */
// FUNCTION: FF7 0x004c4a6b
void __cdecl BattleFx_LimitLuckyGirlSineScaledSpriteTask(void)
{
    unsigned char brightness;
    BattleFxLuckyGirlSineSprite *task;

    task = (BattleFxLuckyGirlSineSprite *)BATTLE_CURRENT_TASK_SLOT;
    if (task->step >= 6) {
        brightness = 0x80 - (task->step - 6) * 21;
    } else {
        brightness = 0x80;
    }
    g_BattleLimitLuckyGirlSineSpriteReq.colour[2] = brightness;
    g_BattleLimitLuckyGirlSineSpriteReq.colour[1] = g_BattleLimitLuckyGirlSineSpriteReq.colour[2];
    g_BattleLimitLuckyGirlSineSpriteReq.colour[0] = g_BattleLimitLuckyGirlSineSpriteReq.colour[1];
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, -(task->scale >> 3));
    g_BattleLimitLuckyGirlSineSpriteReq.texture = Gfx_ModelTableGetLoadedRecord(1, g_BattleLimitLuckyGirlLoadedResTable);
    g_BattleLimitLuckyGirlSineSpriteReq.clut = Gfx_ModelTableGetSpriteObject(1, g_BattleLimitLuckyGirlLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleLimitLuckyGirlSineSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->step >= 12) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->scale = task->scaleRate * SinFixed(task->angle) >> 12;
    task->angle += task->angleStep;
    task->angleStep -= (short)(task->angleStep / 6);
    (task->step)++;
}
} /* extern "C" */
