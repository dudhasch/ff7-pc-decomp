// src/ff7/highway/highway_effect_records_pool.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Manages the highway minigame's effect pool: a free-slot list, effect records that
//   create and destroy their actors, per-frame updates of active effects, and spawning of new
//   effects at track coordinates.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "lib/api/graphics.h"
#include "ff7/highway_palette.h"
#include "ff7/highway_psxdata.h"
#include <math.h>
#include <stdlib.h>
#include "ff7/types/highway.h"
#include "ff7/api/highway.h"
#include "lib/pfile.h"
#include "ff7/enums/highway.h"

#pragma intrinsic(strlen, strcpy, strcat, strcmp)

extern "C" {
#pragma intrinsic(memset)
#pragma intrinsic(atan2, sqrt)

#pragma intrinsic(abs)
// clang-format off
// g_HighwayEffectFreeList links the free effect slots: entry i holds the slot that follows slot i.
// GLOBAL: FF7 0x00d859d0
short g_HighwayEffectFreeList[100] = {0};
// GLOBAL: FF7 0x00d85a98
int g_HighwaySpawnEffectX = 0;
// GLOBAL: FF7 0x00d85a9c
int g_HighwaySpawnEffectY = 0;
// GLOBAL: FF7 0x00d85aa0
int g_HighwaySpawnEffectZ = 0;
unsigned char g_bssPad_00d85aa4[4] = {0};
unsigned char g_bssPad_00d85aa8[24] = {0};
// GLOBAL: FF7 0x00d85ac0
int g_HighwaySpawnEffectKind = 0;
// GLOBAL: FF7 0x00d85ac4
int g_HighwaySpawnEffectTypeId = 0;
unsigned char g_bssPad_00d85ac8[4] = {0};
// GLOBAL: FF7 0x00d85acc
int g_HighwaySpawnEffectInitialized = 0;
unsigned char g_bssPad_00d85ad0[112] = {0};
// GLOBAL: FF7 0x00d85b40
int g_HighwaySpawnEffectType = 0;
unsigned char g_bssPad_00d85b44[4] = {0};
unsigned char g_bssPad_00d85b48[44] = {0};
// GLOBAL: FF7 0x00d85b74
short g_HighwayActiveEffectCount = 0;
// GLOBAL: FF7 0x00d85b78
short g_HighwayEffectFreeHead = 0;
unsigned char g_bssPad_00d85b7c[4] = {0};
// GLOBAL: FF7 0x00d85b80
struct HighwayEffectRecordsPool g_HighwayEffectRecordsPool = {0};
// clang-format on
/*
 * Initializes the 100-record effect pool and resets the free-slot index chain.
 */
// FUNCTION: FF7 0x00652670
void __cdecl Mini_HighwayInitEffectPool(void)
{
    int i;

    for (i = 0; i < 100; i++) {
        g_HighwayEffectRecordsPool.entries[i].slotIndex = 0xFFFF;
        g_HighwayEffectRecordsPool.entries[i].active = 0;
    }
    g_HighwayEffectFreeHead = 0;
    for (i = 0; i < 100; i++) {
        g_HighwayEffectFreeList[i] = i + 1;
    }
}
/*
 * Allocates an effect slot, initializes its state blob, and instantiates the effect actor.
 */
// FUNCTION: FF7 0x006526ea
short __cdecl Mini_HighwayCreateEffectRecord(HighwayEffectPoolEntry *sourceRecord, short parentSlot)
{
    short typeId;
    short effectSlot;

    if (g_HighwayActiveEffectCount < 99) {
        g_HighwayActiveEffectCount++;
        effectSlot = Mini_HighwayAllocEffectSlot();
        g_HighwayEffectRecordsPool.entries[effectSlot] = *sourceRecord;
        typeId = sourceRecord->typeId;
        g_HighwayEffectRecordsPool.entries[effectSlot].slotIndex = effectSlot;
        g_HighwayEffectRecordsPool.entries[effectSlot].active = 1;
        if (parentSlot == 0) {
            g_HighwayEffectRecordsPool.entries[effectSlot].actor =
                (int)g_pHighwayIndexBlock->createRaw(typeId, (HighwayModelNode *)g_pHighwayIndexBlock, sourceRecord->posX,
                                sourceRecord->posY, sourceRecord->posZ,
                                sourceRecord->rotX, sourceRecord->rotY,
                                sourceRecord->rotZ);
        } else {
            g_HighwayEffectRecordsPool.entries[effectSlot].actor =
                (int)g_pHighwayIndexBlock->createRaw(typeId, (HighwayModelNode *)g_HighwayEffectRecordsPool.entries[parentSlot].actor,
                                sourceRecord->posX, sourceRecord->posY, sourceRecord->posZ,
                                sourceRecord->rotX, sourceRecord->rotY,
                                sourceRecord->rotZ);
        }
    } else {
        effectSlot = -1;
    }
    return effectSlot;
}
/*
 * Unlinks and destroys the actor for an effect record and returns its slot to the free list.
 */
// FUNCTION: FF7 0x00652836
void __cdecl Mini_HighwayReleaseEffectRecord(HighwayEffectPoolEntry *record)
{
    if (record->slotIndex != -1) {
        g_HighwayActiveEffectCount--;
        Mini_HighwayActorUnlinkAndRelease((void *)record->actor);
        Mini_HighwayFreeEffectSlot((short)record->slotIndex);
        record->slotIndex = 0xFFFF;
        record->active = 0;
    }
}
/*
 * Allocates a free effect slot by popping the head of the free-slot list.
 */
// FUNCTION: FF7 0x00652895
short __cdecl Mini_HighwayAllocEffectSlot(void)
{
    unsigned short val;

    val = g_HighwayEffectFreeHead;
    g_HighwayEffectFreeHead = g_HighwayEffectFreeList[g_HighwayEffectFreeHead];
    return (short)val;
}
/*
 * Frees an effect slot by pushing it onto the head of the free-slot list.
 */
// FUNCTION: FF7 0x006528c1
void __cdecl Mini_HighwayFreeEffectSlot(short slot)
{
    g_HighwayEffectFreeList[slot] = g_HighwayEffectFreeHead;
    g_HighwayEffectFreeHead = slot;
}
/*
 * Runs the short-lived effects of the chase for one frame; called by `Mini_HighwayRunFrame`.
 * Walks all 100 records of the effect pool and, for each active one, steps it by its kind:
 * - 1: follows actor slot `groupIndex`: places itself on the track at the render offset plus a
 *   row offset that falls by 0x28 per frame, rises by a random 0..19 more each frame, and ends
 *   after 11 frames;
 * - 2: cycles model animations 0xAA..0xAC and ends after 3 frames;
 * - 3: cycles model animations 0xB2..0xB4 and ends after 2 frames;
 * - 4: spins 0x100 per frame about Y and flies in an arc (vertical step -30, growing by 1 per
 *   frame) and ends after 61 frames.
 * Then rebuilds the record's actor rotation and position and renders it. A record released in
 * this pass is still transformed and rendered once: the actor pointer stays in the record after
 * `Mini_HighwayActorUnlinkAndRelease` has given the actor back.
 */
// FUNCTION: FF7 0x006528e4
void __cdecl Mini_HighwayUpdateEffectRecords(void)
{
    HighwayEffectRecord *pEffect;
    short unused;
    struct HighwayEffectTickScratch *pActor;
    HighwaySlotState *pSlot;
    int i;
    char rotFlag;
    char noDraw;
    HighwayEffectPoolEntry *pPooled;

    for (i = 0; i < 100; i++) {
        pPooled = &g_HighwayEffectRecordsPool.entries[i];
        pEffect = &pPooled->effect;
        unused = 0;
        if (pPooled->active != 0) {
            rotFlag = 2;
            noDraw = 0;
            switch (pEffect->kind) {
            case HIGHWAY_EFFECT_SMOKE_TRAIL:
                pSlot = (HighwaySlotState *)(pEffect->groupIndex * 0x1A8 + (g_HighwaySlotActors + 0x68));
                if (pEffect->initialized == 0) {
                    pEffect->initialized = 1;
                    pEffect->timer = 0;
                    pEffect->rowOffset = 0;
                    pEffect->jitter = 0;
                }
                g_pHighwayFieldManager->load(g_HighwayActorRenderOffset + pEffect->rowOffset + (pSlot->posZ >> 8), pSlot->posX >> 8, &pPooled->posX, (int *)&pPooled->rotX);
                (pEffect->timer)++;
                pEffect->rowOffset -= 0x28;
                pEffect->jitter += rand() % 20;
                pPooled->posY -= pEffect->jitter;
                if (pEffect->timer > 10) {
                    Mini_HighwayReleaseEffectRecord(pPooled);
                }
                break;
            case HIGHWAY_EFFECT_SPARK_A:
                if (pEffect->initialized == 0) {
                    pEffect->initialized = 1;
                    pEffect->timer = 0;
                    pEffect->rowOffset = 0;
                    pEffect->jitter = 0;
                    pEffect->variant = 0;
                }
                (pEffect->variant)++;
                pEffect->variant = pEffect->variant % 3;
                pEffect->animId = pEffect->variant + 0xAA;
                ((HighwayModelNode *)pPooled->actor)->select(*(short *)&pEffect->animId);
                (pEffect->timer)++;
                if (pEffect->timer > 2) {
                    Mini_HighwayReleaseEffectRecord(pPooled);
                }
                break;
            case HIGHWAY_EFFECT_SPARK_B:
                if (pEffect->initialized == 0) {
                    pEffect->initialized = 1;
                    pEffect->timer = 0;
                    pEffect->rowOffset = 0;
                    pEffect->jitter = 0;
                    pEffect->variant = 0;
                }
                (pEffect->variant)++;
                pEffect->variant = pEffect->variant % 3;
                pEffect->animId = pEffect->variant + 0xB2;
                ((HighwayModelNode *)pPooled->actor)->select(*(short *)&pEffect->animId);
                (pEffect->timer)++;
                if (pEffect->timer > 1) {
                    Mini_HighwayReleaseEffectRecord(pPooled);
                }
                break;
            case HIGHWAY_EFFECT_FLYING_DEBRIS:
                if (pEffect->initialized == 0) {
                    pEffect->initialized = 1;
                    pEffect->timer = 0;
                    pEffect->rowOffset = 0;
                    pEffect->jitter = 0xFFFFFFE2;
                    pEffect->variant = 0;
                }
                (pEffect->variant)++;
                pPooled->rotY += 0x100 /* 22.5 deg */;
                pPooled->posY += pEffect->jitter;
                (pEffect->jitter)++;
                (pEffect->timer)++;
                if (pEffect->timer > 60) {
                    Mini_HighwayReleaseEffectRecord(pPooled);
                }
            }
            pActor = (struct HighwayEffectTickScratch *)pPooled->actor;
            if (rotFlag == 2) {
                RotMatrixXYZ(&pPooled->rotX, (void *)pActor);
            }
            pActor->posX = pPooled->posX;
            pActor->jitterPos = pPooled->posY;
            pActor->posZ = pPooled->posZ;
            if (noDraw == 0) {
                ((HighwayModelNode *)pPooled->actor)->render(0);
            }
        }
    }
}
/*
 * Configures spawn parameters and creates a new effect record at the specified coordinates.
 */
// FUNCTION: FF7 0x00652cc4
int __cdecl Mini_HighwaySpawnEffect(int posX, int posY, int posZ, short rotY, short rotZ)
{
    short slot;

    g_HighwaySpawnEffectX = posX;
    g_HighwaySpawnEffectY = posY;
    g_HighwaySpawnEffectZ = posZ;
    g_HighwaySpawnEffectKind = rotY;
    g_HighwaySpawnEffectInitialized = 1;
    g_HighwaySpawnEffectTypeId = rotZ;
    g_HighwaySpawnEffectType = 0;
    slot = Mini_HighwayCreateEffectRecord((HighwayEffectPoolEntry *)&g_HighwaySpawnEffectX, 0);
    if (slot >= 0) {
        return (int)&g_HighwayEffectRecordsPool.entries[slot];
    }
    return 0;
}
} /* extern "C" */
