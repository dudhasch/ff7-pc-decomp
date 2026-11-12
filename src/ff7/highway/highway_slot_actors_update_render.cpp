// src/ff7/highway/highway_slot_actors_update_render.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The highway minigame's slot actors: their loading, setup and release, the per-frame
//   movement, steering and collision logic, transforms and animation, and rendering. A top-level
//   driver updates and renders all active slot actors each frame.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/highway_palette.h"
#include "ff7/highway_psxdata.h"
#include <math.h>
#include <stdlib.h>
#include "ff7/types/highway.h"
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/api/highway.h"
#include "lib/api/graphics.h"
#include "lib/pfile.h"
#include "ff7/enums/highway.h"

#pragma intrinsic(strlen, strcpy, strcat, strcmp)
struct GfxFloatColorAddend;
struct PolygonModelVertexColors;
struct PolygonPlacement;
extern "C" {

#pragma intrinsic(memset)
#pragma intrinsic(atan2, sqrt)
/* g_HighwaySlotActors is six 0x1A8-byte slot records: each starts with a HighwayRenderSlotActor and
 * its HighwaySlotState begins at +0x68. */
#define g_HighwaySlotHeads ((HighwayRenderSlotActor *)g_HighwaySlotActors)
#define g_HighwaySlotStates ((HighwaySlotState *)(g_HighwaySlotActors + 0x68))
// clang-format off
// GLOBAL: FF7 0x00909398
extern "C" int g_HighwayActorConfigTable[82] = {
    0, 4, 2, 3, 10, 10, 5, 1, 2, 10, 10, 10, 10, 10,
    0, 1, 10, 10, 10, 10, 10, 1, 2, 3, 1, 10, 10, 10,
    0, 5, 1, 2, 3, 4, 10, 0, 1, 2, 3, 4, 10, 10,
    0, 5, 1, 2, 3, 4, 10, 0, 1, 2, 3, 4, 10, 10,
    0, 5, 1, 2, 3, 4, 10, 0, 1, 2, 3, 4, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 0, 10, 10, 10, 10};
extern "C" unsigned char g_rawData_009094e0[8] = {0x0a, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x009094e8
int g_HighwaySlotAnimFlags[18] = {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// GLOBAL: FF7 0x00909530
int g_HighwaySlotMinIndex = 1;
// GLOBAL: FF7 0x00909534
int g_HighwaySlotMaxIndex = 36;
// GLOBAL: FF7 0x00909538
int g_HighwayActorZOffset = 512;
// GLOBAL: FF7 0x00d8b170
char g_HighwayActiveSlotCount = 0;
// GLOBAL: FF7 0x00d8b174
int g_HighwaySlotBaseIndex = 0;
// GLOBAL: FF7 0x00d8b178
int g_HighwaySlotActor3ModelBase = 0;
// GLOBAL: FF7 0x00d8b17c
int g_HighwaySlotActor2ModelBase = 0;
// GLOBAL: FF7 0x00d8b180
int g_HighwaySlotActor2AltModelBase = 0;
// GLOBAL: FF7 0x00d8b184
int g_HighwaySlotActor1ModelBase = 0;
unsigned char g_bssPad_00d8b188[384] = {0};
// GLOBAL: FF7 0x00d8b308
char g_HighwaySpawnCooldown = 0;
// GLOBAL: FF7 0x00d8b30c
int g_HighwayActorContext = 0;
// GLOBAL: FF7 0x00d8b310
int g_HighwayActorFieldList[2] = {0};
// GLOBAL: FF7 0x00d8b318
HighwayAnimItem *g_pHighwayActorAnimItems = 0;
unsigned char g_bssPad_00d8b31c[4] = {0};
// GLOBAL: FF7 0x00d8b320
unsigned char g_HighwaySlotActors[8480] = {0};
// GLOBAL: FF7 0x00d8d440
struct HighwayFieldManager *g_pHighwayFieldManager = 0;
// clang-format off
// GLOBAL: FF7 0x00d8d444
int g_pHighwayRenderBlock = 0;
// clang-format on
// GLOBAL: FF7 0x00d8d448
struct HighwayModeController *g_pHighwayModeBlock = 0;
// GLOBAL: FF7 0x00d8d44c
struct HighwayIndex *g_pHighwayIndexBlock = 0;
// GLOBAL: FF7 0x00d8d450
struct HighwayArray *g_HighwayRosterBlock = 0;
// clang-format on
extern int g_HighwayActorConfigTable[82];
void __cdecl Gfx_RestoreModelVertexColors(struct PolygonModelVertexColors *object, int context);
void __cdecl Gfx_AddColorToModelVertexColors(struct GfxFloatColorAddend *color, struct PolygonModelVertexColors *object, int context);

void __cdecl AnimPlacement_SetScale(float scale, struct PolygonPlacement *placement);
void __cdecl Anim_SetTransformFlags(int *src);

#pragma intrinsic(abs)

#pragma bss_seg(".bss$u652")
static HighwayEmptyStaticInit s_HighwayEmptyStaticInit;
#pragma bss_seg()
/*
 * Initializes the 18-element slot array, zeroing active slot states and resetting parameters.
 */
// FUNCTION: FF7 0x00652d4f
int __cdecl Mini_HighwayInitSlotArray(int context)
{
    g_HighwayActorContext = context;
    g_HighwayActiveSlotCount = 0;
    g_HighwaySpawnCooldown = 0x78;
    g_HighwaySlotActor3ModelBase = 0x8E;
    g_HighwaySlotActor2ModelBase = 0x95;
    g_HighwaySlotActor2AltModelBase = 0x94;
    g_HighwaySlotActor1ModelBase = 0xB1;
    Mini_HighwayResetSlot(0);
    Mini_HighwayResetSlot(1);
    Mini_HighwayResetSlot(2);
    Mini_HighwayResetSlot(3);
    Mini_HighwayResetSlot(4);
    Mini_HighwayResetSlot(5);
    return Mini_HighwayInitActorGroups(context);
}
/*
 * Runs the chase's six actor slots for one frame; called by `Mini_HighwayRunFrame` with
 * g_HighwayActorRenderOffset (the camera's track position plus 0x2300), which it stores in
 * g_HighwaySlotBaseIndex for the slot code to place the actors against. In order: spawns new
 * actors, moves the active slots, builds their transforms, clamps them, renders them, steps
 * their animations, draws the HRC skeleton list and last tests the slots against each other for
 * collisions.
 */
// FUNCTION: FF7 0x00652dda
void __cdecl Mini_HighwayUpdateAndRenderActors(int slotBase, struct GfxEngineContext *context)
{
    g_HighwaySlotBaseIndex = slotBase;
    Mini_HighwayUpdateSpawns();
    Mini_HighwayUpdateActiveSlotMovement();
    Mini_HighwayUpdateActiveSlotTransforms();
    Mini_HighwayClampActiveSlots();
    Mini_HighwayRenderActiveSlotActors();
    Mini_HighwayUpdateActiveSlotAnimations();
    ((HighwayFieldList *)&g_HighwayActorFieldList)->draw((int)context);
    Mini_HighwayActorsCollidePairs();
}
/*
 * Resets per-slot actor transform, position, and status fields to default values.
 */
// FUNCTION: FF7 0x00652e18
void __cdecl Mini_HighwayResetSlot(int slot)
{
    int i;
    int *base;

    base = (int *)&g_HighwaySlotStates[slot];
    g_HighwaySlotHeads[slot].posX = 0;
    g_HighwaySlotHeads[slot].posY = 0;
    g_HighwaySlotHeads[slot].posZ = 0;
    g_HighwaySlotHeads[slot].rotX = 0;
    g_HighwaySlotHeads[slot].rotY = 0;
    g_HighwaySlotHeads[slot].rotZ = 0;
    for (i = 0; i < 80; i++) {
        base[i] = 0;
    }
    *base = 5;
}
/*
 * Loads animation data files from disk or archive for all slot actor types.
 */
// FUNCTION: FF7 0x00652ed2
int __cdecl Mini_HighwayLoadAnimations(int animSet)
{
    struct {
        int player;
        int secondIndex;
        int animation;
        HighwayAnimItem *item;
        int firstIndex;
        PfileLoadOptions *options;
        int index;
        PfileLoadOptions loadOptions;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.options = &frame.loadOptions;
    Pfile_InitLoadOptions(frame.options);
    frame.options->flags |= 0x100000;
    frame.options->flags |= 0x800;
    frame.options->flags |= 0x2000000;
    Pfile_SetLoadOptionsBlendMode(BLEND_NONE, frame.options);
    frame.options->scale = 1.0f;
    if (g_HighwayUseLgpArchive != 0) {
        frame.options->file.fromArchive = 1;
        frame.options->file.archiveId = 8;
        frame.options->file.callback = 0;
    }
    if (((HighwaySkelList *)&g_HighwayActorFieldList)->load((int)frame.options, animSet) == 0) {
        // STRING: FF7 0x0090953c
        Gfx_DebugPrint("ERROR: Couldn't initialize animations!!!\n");
        return 0;
    } else {
        for (frame.index = 0; frame.index < g_HighwayActorFieldList[0]; frame.index++) {
            frame.firstIndex = frame.index;
            if (frame.firstIndex < 0 || frame.firstIndex >= g_HighwayActorFieldList[0]) {
                frame.firstIndex = 0;
            }
            frame.item = g_pHighwayActorAnimItems + frame.firstIndex;
            frame.animation = g_HighwaySlotAnimFlags[frame.index];
            frame.item->tableIndex = frame.animation;
            ((HighwayPart *)frame.item)->visible(0);
            frame.secondIndex = frame.index;
            if (frame.secondIndex < 0 || frame.secondIndex >= g_HighwayActorFieldList[0]) {
                frame.secondIndex = 0;
            }
            frame.player = (g_pHighwayActorAnimItems + frame.secondIndex)->player;
            AnimPlacement_SetScale(2.3f, (PolygonPlacement *)frame.player);
        }
        return 1;
    }
}
/*
 * Releases allocated skeleton list structures for highway actors.
 */
// FUNCTION: FF7 0x00653067
void __cdecl Mini_HighwayReleaseSkeletonList(void)
{
    ((HighwaySkelList *)&g_HighwayActorFieldList)->release();
}
/*
 * Initializes actor group models, skeletons, and animation bindings for minigame actors.
 */
// FUNCTION: FF7 0x00653076
int __cdecl Mini_HighwayInitActorGroups(int animSet)
{
    int stateIndex;
    HighwaySlotState *childGroup;
    int flags;
    HighwayRenderSlotActor *pGroupRef;
    int altPhase;
    int stageIndex;
    int statusB;
    int lastPhase;

    if (Mini_HighwayLoadAnimations(animSet) == 0) {
        return 0;
    } else {
        flags = 0x407B;
        Anim_SetTransformFlags(&flags);
        Mini_HighwayResetSlot(0);
        pGroupRef = &g_HighwaySlotHeads[0];
        childGroup = &g_HighwaySlotStates[0];
        childGroup->state = HIGHWAY_SLOT_STATE_NORMAL;
        childGroup->mode = HIGHWAY_SLOT_MODE_PLAYER;
        childGroup->actorCount = 4;
        childGroup->partCount = 2;
        childGroup->firstId = 1;
        childGroup->secondId = 0;
        childGroup->posX = 0;
        childGroup->posZ = 0x3E800;
        childGroup->maxPosZ = 0xDAC00;
        childGroup->minPosZ = 0x1F400;
        childGroup->clearedWord = 0;
        childGroup->isActive = 1;
        childGroup->segmentStartZ = 20;
        childGroup->segmentEndZ = 100;
        childGroup->startRadius = 36;
        childGroup->endRadius = 36;
        childGroup->lateralStep = 48;
        childGroup->lateralReturnStep = 48;
        pGroupRef->actors[0] = (int)g_pHighwayIndexBlock->createRaw(97, (HighwayModelNode *)g_pHighwayIndexBlock, 0, 0, 0, 0, 0, 0);
        pGroupRef->actors[1] = (int)g_pHighwayIndexBlock->createRaw(155, (HighwayModelNode *)pGroupRef->actors[0], 0, -10, 85, 0, 0, 0);
        pGroupRef->actors[2] = (int)g_pHighwayIndexBlock->createRaw(149, (HighwayModelNode *)pGroupRef->actors[0], -80, 0, -40, 0, 0, 0);
        pGroupRef->actors[3] = (int)g_pHighwayIndexBlock->createRaw(97, (HighwayModelNode *)pGroupRef->actors[0], 0, 0, 0, 0, 0, 0);
        pGroupRef->hideQueued = 0;
        pGroupRef->isVisible = 0;
        pGroupRef->freezeQueued = 0;
        pGroupRef->isFrozen = 0;
        pGroupRef->unusedSlotA = 0xFFFFFFFF;
        pGroupRef->unusedSlotB = 0xFFFFFFFF;
        pGroupRef->animIndexA = 0;
        pGroupRef->animIndexB = 1;
        stateIndex = pGroupRef->animIndexA;
        if (stateIndex < 0 || stateIndex >= g_HighwayActorFieldList[0]) {
            stateIndex = 0;
        }
        pGroupRef->animItemA = g_pHighwayActorAnimItems + stateIndex;
        stageIndex = pGroupRef->animIndexB;
        if (stageIndex < 0 || stageIndex >= g_HighwayActorFieldList[0]) {
            stageIndex = 0;
        }
        pGroupRef->animItemB = g_pHighwayActorAnimItems + stageIndex;
        ((HighwayPart *)pGroupRef->animItemA)->visible(1);
        ((HighwayPart *)pGroupRef->animItemB)->visible(1);
        Mini_HighwayResetSlot(1);
        pGroupRef = &g_HighwaySlotHeads[1];
        childGroup = &g_HighwaySlotStates[1];
        childGroup->state = HIGHWAY_SLOT_STATE_NORMAL;
        childGroup->mode = HIGHWAY_SLOT_MODE_ESCORTED;
        childGroup->actorCount = 3;
        childGroup->partCount = 3;
        if (g_HighwayGameMode == 0) {
            childGroup->firstId = 2;
            childGroup->secondId = 0xD;
            pGroupRef->animIndexB = 2;
            pGroupRef->animIndexA = 17;
        } else {
            childGroup->firstId = 0x11;
            childGroup->secondId = 0x13;
            pGroupRef->animIndexB = 3;
            pGroupRef->animIndexA = 0xFFFFFFFF;
        }
        childGroup->posX = 0;
        childGroup->posZ = 0x9C400;
        childGroup->maxPosZ = 0x9C400;
        childGroup->minPosZ = 0x5DC00;
        childGroup->isActive = 1;
        childGroup->randomChance = 0x800;
        childGroup->lateralStep = 500;
        childGroup->segmentStartZ = 0xFFFFFFE2;
        childGroup->segmentEndZ = 100;
        childGroup->startRadius = 75;
        childGroup->endRadius = 75;
        pGroupRef->actors[0] = (int)g_pHighwayIndexBlock->createRaw(97, (HighwayModelNode *)g_pHighwayIndexBlock, 0, 0, 0, 0, 0, 0);
        pGroupRef->actors[1] = (int)g_pHighwayIndexBlock->createRaw(153, (HighwayModelNode *)pGroupRef->actors[0], 0, 0, 0, 0, 0, 0);
        pGroupRef->actors[2] = (int)g_pHighwayIndexBlock->createRaw(154, (HighwayModelNode *)pGroupRef->actors[0], 0, 3, -6, 0, 0, 0);
        pGroupRef->hideQueued = 0;
        pGroupRef->isVisible = 0;
        pGroupRef->freezeQueued = 0;
        pGroupRef->isFrozen = 0;
        pGroupRef->unusedSlotA = 0xFFFFFFFF;
        pGroupRef->unusedSlotB = 0xFFFFFFFF;
        altPhase = pGroupRef->animIndexB;
        if (altPhase < 0 || altPhase >= g_HighwayActorFieldList[0]) {
            altPhase = 0;
        }
        pGroupRef->animItemB = g_pHighwayActorAnimItems + altPhase;
        if (pGroupRef->animIndexA != -1) {
            statusB = pGroupRef->animIndexA;
            if (statusB < 0 || statusB >= g_HighwayActorFieldList[0]) {
                statusB = 0;
            }
            pGroupRef->animItemA = g_pHighwayActorAnimItems + statusB;
        } else {
            pGroupRef->animItemA = 0;
        }
        ((HighwayPart *)pGroupRef->animItemB)->visible(1);
        if (pGroupRef->animIndexA != -1) {
            ((HighwayPart *)pGroupRef->animItemA)->visible(1);
        }
        Mini_HighwayResetSlot(5);
        pGroupRef = &g_HighwaySlotHeads[5];
        childGroup = &g_HighwaySlotStates[5];
        childGroup->state = HIGHWAY_SLOT_STATE_NORMAL;
        childGroup->state = HIGHWAY_SLOT_STATE_INACTIVE;
        childGroup->mode = HIGHWAY_SLOT_MODE_BOSS;
        childGroup->actorCount = 2;
        childGroup->partCount = 1;
        childGroup->firstId = 0xC;
        childGroup->secondId = 0x13;
        childGroup->posX = 0x3E800;
        childGroup->posZ = 0xFFFFE4A8;
        childGroup->maxPosZ = 0x5DC00;
        childGroup->minPosZ = 0x1F400;
        childGroup->isActive = 1;
        childGroup->segmentStartZ = 0xFFFFFFB0;
        childGroup->segmentEndZ = 75;
        childGroup->startRadius = 132;
        childGroup->endRadius = 240;
        childGroup->randomChance = 0x100;
        childGroup->lateralReturnStep = 4096;
        childGroup->lateralStep = 250;
        childGroup->randomRange = 30;
        childGroup->hp = 0x270F;
        childGroup->maxHp = childGroup->hp;
        pGroupRef->actors[0] = (int)g_pHighwayIndexBlock->createRaw(97, (HighwayModelNode *)g_pHighwayIndexBlock, 0, 0, 0, 0, 0, 0);
        pGroupRef->actors[1] = (int)g_pHighwayIndexBlock->createRaw(97, (HighwayModelNode *)pGroupRef->actors[0], 0, 0, 0, 0, 0, 0);
        pGroupRef->animIndexB = 4;
        pGroupRef->animIndexA = 0xFFFFFFFF;
        pGroupRef->hideQueued = 0;
        pGroupRef->isVisible = 0;
        pGroupRef->freezeQueued = 0;
        pGroupRef->isFrozen = 0;
        pGroupRef->unusedSlotA = 0xFFFFFFFF;
        pGroupRef->unusedSlotB = 0xFFFFFFFF;
        lastPhase = pGroupRef->animIndexB;
        if (lastPhase < 0 || lastPhase >= g_HighwayActorFieldList[0]) {
            lastPhase = 0;
        }
        pGroupRef->animItemB = g_pHighwayActorAnimItems + lastPhase;
        pGroupRef->animItemA = 0;
        return 1;
    }
}
/*
 * Instantiates the 3D actor objects for each active slot.
 */
// FUNCTION: FF7 0x0065372a
void __cdecl Mini_HighwayCreateSlotActors(int slot)
{
    HighwayRenderSlotActor *slotActor;
    int animIndex;
    HighwaySlotState *slotStatus;

    g_HighwayActiveSlotCount++;
    slotActor = &g_HighwaySlotHeads[slot];
    slotStatus = &g_HighwaySlotStates[slot];
    slotStatus->state = HIGHWAY_SLOT_STATE_INACTIVE;
    slotStatus->mode = HIGHWAY_SLOT_MODE_ENEMY;
    slotStatus->actorCount = 3;
    slotStatus->partCount = 2;
    slotStatus->maxPosZ = 0xBB800;
    slotStatus->minPosZ = 0x1F400;
    slotStatus->isActive = 1;
    slotStatus->segmentStartZ = 20;
    slotStatus->segmentEndZ = 100;
    slotStatus->startRadius = 36;
    slotStatus->endRadius = 36;
    slotStatus->randomChance = 0x400;
    slotStatus->lateralReturnStep = 1024;
    slotStatus->lateralStep = 450;
    slotStatus->randomRange = 20;
    slotActor->actors[0] =
        (int)g_pHighwayIndexBlock->createRaw(97, (HighwayModelNode *)g_pHighwayIndexBlock, 0, 0, 0, 0, 0, 0);
    slotActor->actors[1] =
        (int)g_pHighwayIndexBlock->createRaw(155, (HighwayModelNode *)slotActor->actors[0], 0, 0, 120, 0, 0, 0);
    slotActor->actors[2] =
        (int)g_pHighwayIndexBlock->createRaw(148, (HighwayModelNode *)slotActor->actors[0], 0, 0, 0, 0, 0, 0);
    switch (slot) {
    case HIGHWAY_ENEMY_SLOT_LEFT:
        slotStatus->posX = 0xFFFF3800;
        slotStatus->posZ = 0xFFF44800;
        break;
    case HIGHWAY_ENEMY_SLOT_CENTER:
        slotStatus->posX = 0;
        slotStatus->posZ = 0xFFF44800;
        break;
    case HIGHWAY_ENEMY_SLOT_RIGHT:
        slotStatus->posX = 0xC800;
        slotStatus->posZ = 0xFFF44800;
    }
    slotActor->animIndexA = slot + 3;
    slotActor->hideQueued = 0;
    slotActor->isVisible = 0;
    slotActor->freezeQueued = 0;
    slotActor->isFrozen = 0;
    slotActor->unusedSlotA = -1;
    slotActor->unusedSlotB = -1;
    animIndex = slotActor->animIndexA;
    if (animIndex < 0 || animIndex >= g_HighwayActorFieldList[0]) {
        animIndex = 0;
    }
    slotActor->animItemA = g_pHighwayActorAnimItems + animIndex;
}
/*
 * Iterates through all active slots to clamp their positions within course boundaries.
 */
// FUNCTION: FF7 0x0065394d
void __cdecl Mini_HighwayClampActiveSlots(void)
{
    Mini_HighwayClampSlotPosition(0);
    Mini_HighwayClampSlotPosition(1);
    if (g_HighwaySlotStates[2].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayClampSlotPosition(2);
    }
    if (g_HighwaySlotStates[3].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayClampSlotPosition(3);
    }
    if (g_HighwaySlotStates[4].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayClampSlotPosition(4);
    }
    if (g_HighwaySlotStates[5].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayClampSlotPosition(5);
    }
}

// Derives cyclic frame and width-dependent horizontal bound, clamps actor field with special
// margin for actor five. Actor zero also clamps longitudinal field, enables UI globals and derives
// edge-dependent value from first qualifying side or fixed512. Negative width can invert bounds.
// FUNCTION: FF7 0x006539b2
void __cdecl Mini_HighwayClampSlotPosition(int slot)
{
    int roadLimit;
    HighwaySlotState *slotActor;
    int rightGap;
    int leftGap;
    int frameIdx;

    slotActor = &g_HighwaySlotStates[slot];
    frameIdx = (g_HighwayActorRenderOffset + (slotActor->posZ >> 8) >> 8) % 80;
    roadLimit = (((HighwayFrameRecord *)(g_pHighwayFieldManager->frames + frameIdx * 0xB0))->roadWidth >> 1) - 0x60 << 8;
    if (slot == 5) {
        if (slotActor->posX > roadLimit - 0x7800) {
            slotActor->posX = roadLimit - 30720;
        }
        if (slotActor->posX < -roadLimit + 0x7800) {
            slotActor->posX = -roadLimit + 30720;
        }
    } else {
        if (slotActor->posX > roadLimit) {
            slotActor->posX = roadLimit;
        }
        if (slotActor->posX < -roadLimit) {
            slotActor->posX = -roadLimit;
        }
    }
    if (slot == 0) {
        if (slotActor->posZ > slotActor->maxPosZ) {
            slotActor->posZ = slotActor->maxPosZ;
        }
        if (slotActor->posZ < slotActor->minPosZ) {
            slotActor->posZ = slotActor->minPosZ;
        }
        g_HighwaySlotMinIndex = 1;
        g_HighwaySlotMaxIndex = 36;
        if (roadLimit - slotActor->posX < 32768) {
            leftGap = roadLimit - slotActor->posX;
            g_HighwayActorZOffset = (leftGap * 312 / 128 >> 8) + 0xC8;
        } else if (slotActor->posX - -roadLimit < 32768) {
            rightGap = slotActor->posX - -roadLimit;
            g_HighwayActorZOffset = (rightGap * 472 / 128 >> 8) + 0x28;
        } else {
            g_HighwayActorZOffset = 0x200;
        }
    }
}
/*
 * Checks spawn triggers and updates cooldown counters for enemy actor slots.
 */
// FUNCTION: FF7 0x00653b60
void __cdecl Mini_HighwayUpdateSpawns(void)
{
    if (g_HighwaySpawningStopped == 0 && g_HighwaySpawnCooldown == 0) {
        if (rand() % 60 == 0 && g_HighwaySlotStates[2].state == HIGHWAY_SLOT_STATE_INACTIVE) {
            Mini_HighwaySpawnSlot(2);
        }
        if (rand() % 60 == 0 && g_HighwaySlotStates[3].state == HIGHWAY_SLOT_STATE_INACTIVE) {
            Mini_HighwaySpawnSlot(3);
        }
        if (rand() % 60 == 0 && g_HighwaySlotStates[4].state == HIGHWAY_SLOT_STATE_INACTIVE) {
            Mini_HighwaySpawnSlot(4);
        }
    }
    if (g_HighwaySpawnCooldown > 0) {
        g_HighwaySpawnCooldown--;
    }
}
/*
 * Spawns an actor into the specified slot, initializing initial coordinates and speeds.
 */
// FUNCTION: FF7 0x00653bfe
void __cdecl Mini_HighwaySpawnSlot(int slot)
{
    int nextIndex;
    int pick;
    HighwayRenderSlotActor *slotActor;
    HighwaySlotState *spawnPtr;
    int partnerSlot;
    int animIndex;

    Mini_HighwayResetSlot(slot);
    Mini_HighwayCreateSlotActors(slot);
    spawnPtr = &g_HighwaySlotStates[slot];
    slotActor = &g_HighwaySlotHeads[slot];
    pick = rand() % 3;
    switch (pick) {
    case HIGHWAY_SPAWN_VARIANT_LIGHT:
        nextIndex = slot + 1;
        spawnPtr->hp = 5;
        spawnPtr->maxHp = spawnPtr->hp;
        slotActor->animIndexB = slot + 6;
        break;
    case HIGHWAY_SPAWN_VARIANT_HEAVY:
        nextIndex = slot + 7;
        spawnPtr->hp = 0x3C;
        spawnPtr->maxHp = spawnPtr->hp;
        slotActor->animIndexB = slot + 9;
        break;
    case HIGHWAY_SPAWN_VARIANT_MEDIUM:
        nextIndex = slot + 12;
        spawnPtr->hp = 0x1E;
        spawnPtr->maxHp = spawnPtr->hp;
        slotActor->animIndexB = slot + 12;
        break;
    default:
        nextIndex = slot + 1;
        spawnPtr->hp = 0x28;
        spawnPtr->maxHp = spawnPtr->hp;
    }
    partnerSlot = slot + 4;
    spawnPtr = &g_HighwaySlotStates[slot];
    spawnPtr->state = HIGHWAY_SLOT_STATE_NORMAL;
    spawnPtr->firstId = nextIndex;
    spawnPtr->secondId = partnerSlot;
    spawnPtr->aiMode = pick + 0xA;
    animIndex = slotActor->animIndexB;
    if (animIndex < 0 || animIndex >= g_HighwayActorFieldList[0]) {
        animIndex = 0;
    }
    slotActor->animItemB = g_pHighwayActorAnimItems + animIndex;
    ((HighwayPart *)slotActor->animItemA)->visible(1);
    ((HighwayPart *)slotActor->animItemB)->visible(1);
}
/*
 * Despawns an actor from the specified slot, resetting its active state.
 */
// FUNCTION: FF7 0x00653dcf
void __cdecl Mini_HighwayDespawnSlot(int slot)
{
    int idx;
    HighwayRenderSlotActor *slotActor;
    HighwaySlotState *spawnStatus;

    spawnStatus = &g_HighwaySlotStates[slot];
    slotActor = &g_HighwaySlotHeads[slot];
    if (slotActor->animItemA != 0) {
        ((HighwayPart *)slotActor->animItemA)->visible(0);
    }
    ((HighwayPart *)slotActor->animItemB)->visible(0);
    spawnStatus->state = HIGHWAY_SLOT_STATE_INACTIVE;
    for (idx = 0; idx < spawnStatus->actorCount; idx++) {
        Mini_HighwayActorUnlinkAndRelease((void *)slotActor->actors[idx]);
    }
}
/*
 * Updates motion and physics calculations across all active actor slots.
 */
// FUNCTION: FF7 0x00653e56
void __cdecl Mini_HighwayUpdateActiveSlotMovement(void)
{
    if (g_HighwaySlotStates[0].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotMovement(0);
    }
    if (g_HighwaySlotStates[1].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotMovement(1);
    }
    if (g_HighwaySlotStates[5].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotMovement(5);
    }
    if (g_HighwaySlotStates[2].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayStepSlotAndClampPosition(2);
    }
    if (g_HighwaySlotStates[3].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayStepSlotAndClampPosition(3);
    }
    if (g_HighwaySlotStates[4].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayStepSlotAndClampPosition(4);
    }
}
/*
 * Updates physics, acceleration, steering, and velocities for a specific slot actor.
 */
// FUNCTION: FF7 0x00653ecd
void __cdecl Mini_HighwayUpdateSlotMovement(int slotIndex)
{
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int nextSample[3];
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    HighwaySlotState *slotData;
    int trackPosition;
    int progressBPart;
    int laneNow;
    int stepPosition;
    int laneZ;
    int curSample[3];
    HighwayRenderSlotActor *actorRecord;
    int rootVal;
    int steerAngle;
    int lanePos;
    int laneId;
    int aheadPos;

    slotData = &g_HighwaySlotStates[slotIndex];
    actorRecord = &g_HighwaySlotHeads[slotIndex];
    switch (slotData->mode) {
    case HIGHWAY_SLOT_MODE_PLAYER:
        if (slotData->subState == 0) {
            rootVal = actorRecord->animItemA->player;
            if (slotData->timer == 0) {
                if (g_HighwayInputAction == HIGHWAY_ACTION_SQUARE) {
                    slotData->timer = 29;
                }
                if (g_HighwayInputAction == HIGHWAY_ACTION_CIRCLE) {
                    slotData->timer = 0xFFFFFFE3;
                }
            }
            if (g_HighwayInputDirection == HIGHWAY_DIR_DOWN_LEFT) {
                steerAngle = 0xA00 /* 225 deg */;
            }
            if (g_HighwayInputDirection == HIGHWAY_DIR_DOWN) {
                steerAngle = 0xC00 /* 270 deg */;
            }
            if (g_HighwayInputDirection == HIGHWAY_DIR_DOWN_RIGHT) {
                steerAngle = 0xE00 /* 315 deg */;
            }
            if (g_HighwayInputDirection == HIGHWAY_DIR_LEFT) {
                steerAngle = 0x800 /* 180 deg */;
            }
            if (g_HighwayInputDirection == HIGHWAY_DIR_RIGHT) {
                steerAngle = 0;
            }
            if (g_HighwayInputDirection == HIGHWAY_DIR_UP_LEFT) {
                steerAngle = 0x600 /* 135 deg */;
            }
            if (g_HighwayInputDirection == HIGHWAY_DIR_UP) {
                steerAngle = 0x400 /* 90 deg */;
            }
            if (g_HighwayInputDirection == HIGHWAY_DIR_UP_RIGHT) {
                steerAngle = 0x200 /* 45 deg */;
            }
            if (slotData->state == 6) {
                g_HighwayInputLock = 1;
                if (slotData->posX > 46080) {
                    g_HighwayInputDirection = HIGHWAY_DIR_LEFT;
                    steerAngle = 0x800 /* 180 deg */;
                }
                if (slotData->posX < 42240) {
                    g_HighwayInputDirection = HIGHWAY_DIR_RIGHT;
                    steerAngle = 0;
                }
            }
            if (g_HighwayGameMode == 0 && slotData->counter > 1) {
                slotData->posX -= 1536;
                slotData->posZ += 2048;
            }
            if (g_HighwayInputDirection != HIGHWAY_DIR_NONE) {
                slotData->velocityX += CosFixed(steerAngle) * 0x658 >> 12;
                slotData->velocityZ += SinFixed(steerAngle) * 0x658 >> 11;
            }
            slotData->velocityX = (slotData->velocityX << 3) / 10;
            slotData->velocityZ = (slotData->velocityZ << 3) / 10;
            slotData->posX += slotData->velocityX;
            slotData->posZ += slotData->velocityZ;
            if (g_HighwayInputDirection == HIGHWAY_DIR_NONE) {
                if (slotData->angle > 0x3FF) {
                    slotData->angle -= 0xC00 /* 270 deg */;
                }
                if (slotData->angle < (-1023)) {
                    slotData->angle += 0xC00 /* 270 deg */;
                }
                if (slotData->angle > (-1024) && slotData->angle < 0x400) {
                    slotData->angle = 0;
                }
            } else {
                slotData->angle += CosFixed(steerAngle) >> 1;
                if (slotData->angle > 0x4400) {
                    slotData->angle = 0x4400 /* 1530 deg */;
                }
                if (slotData->angle < (-17408)) {
                    slotData->angle = (-17408);
                }
                slotData->absAngle = abs(slotData->angle >> 9);
            }
        } else {
            lanePos = slotData->laneId;
            trackPosition = slotData->trackPos;
            ((HighwaySkelItem *)g_pHighwaySceneBlock)->samplePosition(trackPosition, lanePos, curSample, 1);
            laneId = slotData->laneId;
            aheadPos = slotData->trackPos + 0x80000;
            ((HighwaySkelItem *)g_pHighwaySceneBlock)->samplePosition(aheadPos, laneId, nextSample, 1);
            slotData->zOffset += nextSample[0] - curSample[0] >> 1;
            slotData->zOffset -= slotData->prevSampleX - curSample[0] << 2;
            if (slotData->prevSampleX < curSample[0]) {
                slotData->zOffset += slotData->lateralStep * 2;
            }
            if (slotData->prevSampleX > curSample[0]) {
                slotData->zOffset -= slotData->lateralStep << 1;
            }
            if (slotData->zOffset > g_HighwayActorZOffset) {
                slotData->zOffset = g_HighwayActorZOffset;
            }
            if (slotData->zOffset < -g_HighwayActorZOffset) {
                slotData->zOffset = -g_HighwayActorZOffset;
            }
            slotData->posX = (curSample[0] << 7) + slotData->prevPosX;
            slotData->posZ = (curSample[2] << 7) + slotData->prevPosZ;
            slotData->prevSampleX = curSample[0];
            slotData->trackPos += 0x10000;
            if (slotData->trackPos > slotData->trackLimit) {
                slotData->subState = 0;
                slotData->state = HIGHWAY_SLOT_STATE_NORMAL;
                g_HighwaySlotStates[0].partCount--;
                g_HighwaySlotStates[0].counter = 73;
                g_HighwaySlotStates[1].animCountdown = 49;
            }
        }
        break;
    case HIGHWAY_SLOT_MODE_ESCORTED:
        if (slotData->subState == 0) {
            if (slotData->targetAngle != 0) {
                slotData->lateralStep = 0;
            }
            if (slotData->posZ > slotData->maxPosZ) {
                slotData->countdown = 1;
                slotData->moveAngle = 0xC00 /* 270 deg */;
            }
            if (slotData->posZ < slotData->minPosZ) {
                slotData->countdown = 1;
                slotData->moveAngle = 0x400 /* 90 deg */;
            }
            if (slotData->countdown == 0) {
                if (rand() % 4096 < slotData->randomChance) {
                    slotData->countdown = 10;
                    slotData->moveAngle = rand() % 0x1000;
                    slotData->velocityX = (slotData->velocityX << 3) / 10;
                    slotData->velocityZ = (slotData->velocityZ << 3) / 10;
                }
            } else {
                (slotData->countdown)--;
                slotData->velocityX += CosFixed(slotData->moveAngle) * slotData->lateralStep >> 12;
                slotData->velocityZ += SinFixed(slotData->moveAngle) * slotData->lateralStep >> 11;
                slotData->posX += slotData->velocityX;
                slotData->posZ += slotData->velocityZ;
                slotData->velocityX = (slotData->velocityX << 3) / 10;
                slotData->velocityZ = (slotData->velocityZ << 3) / 10;
                slotData->prevPosX = slotData->posX;
                slotData->prevPosZ = slotData->posZ;
            }
        } else if (slotData->subState == 1) {
            laneNow = slotData->laneId;
            stepPosition = slotData->trackPos;
            ((HighwaySkelItem *)g_pHighwaySceneBlock)->samplePosition(stepPosition, laneNow, curSample, 1);
            slotData->posX = (curSample[0] << 7) + slotData->prevPosX;
            slotData->posZ = (curSample[2] << 7) + slotData->prevPosZ;
            slotData->trackPos += 0x10000;
            if (slotData->trackPos > slotData->trackLimit) {
                slotData->subState = 2;
            }
        }
        break;
    case HIGHWAY_SLOT_MODE_BOSS:
        if (slotData->subState == 0) {
            if (rand() % 30 == 0) {
                if (slotData->angle == 0) {
                    Mini_HighwayDispatchSoundCategory(0x12D, 2, 0);
                }
                slotData->angle = 3;
            }
            if (slotData->posZ > slotData->maxPosZ) {
                slotData->moveAngle = 1;
                slotData->targetAngle = 0xC00;
            }
            if (slotData->posZ < slotData->minPosZ) {
                slotData->moveAngle = 1;
                slotData->targetAngle = 0x400;
            }
            if (slotData->moveAngle == 0) {
                if (rand() % 4096 < slotData->randomChance) {
                    if (rand() % 4096 < slotData->randomThreshold) {
                        slotData->targetAngle = Mini_HighwayAngleBetweenRecords(&g_HighwaySlotHeads[slotIndex], &g_HighwaySlotHeads[1]);
                    } else {
                        slotData->targetAngle = rand() % 4096;
                    }
                    slotData->moveAngle = rand() % (slotData->randomRange + 1);
                }
            } else {
                (slotData->moveAngle)--;
                slotData->velocityX += CosFixed(slotData->targetAngle) * slotData->lateralStep >> 12;
                slotData->velocityZ += SinFixed(slotData->targetAngle) * slotData->lateralStep >> 11;
            }
            slotData->velocityX = (slotData->velocityX << 3) / 10;
            slotData->velocityZ = (slotData->velocityZ << 3) / 10;
            slotData->posX += slotData->velocityX;
            slotData->posZ += slotData->velocityZ;
            slotData->prevPosX = slotData->posX;
            slotData->prevPosZ = slotData->posZ;
        } else {
            if (slotData->subState == 1) {
                laneZ = slotData->laneId;
                progressBPart = slotData->trackPos;
                ((HighwaySkelItem *)g_pHighwaySceneBlock)->samplePosition(progressBPart, laneZ, curSample, 1);
                slotData->posX = (curSample[0] << 7) + slotData->prevPosX;
                slotData->posZ = (curSample[2] << 7) + slotData->prevPosZ;
                slotData->trackPos += 0x10000;
                if (slotData->trackPos > slotData->trackLimit) {
                    slotData->subState = 2;
                }
            }
            if (slotData->laneCode != 0) {
                slotData->useConfigB = 1;
                slotData->configIndexB = 0;
                slotData->animFrameB = slotData->laneCode >> 2;
                slotData->animFrameB = slotData->animFrameB % 11;
            }
        }
    }
}
/*
 * Integrates slot velocity, steps position along the track, and clamps boundaries.
 */
// FUNCTION: FF7 0x006549d2
void __cdecl Mini_HighwayStepSlotAndClampPosition(int slotIndex)
{
    int flags;
    HighwaySlotState *slotPtr;

    flags = 0;
    slotPtr = &g_HighwaySlotStates[slotIndex];
    if (slotPtr->state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayActorIntegrateMotion(slotIndex, flags);
        if (slotPtr->posZ > slotPtr->maxPosZ) {
            slotPtr->posZ = slotPtr->maxPosZ;
        }
    }
}
/*
 * Updates world transformation matrices for all active actor slots.
 */
// FUNCTION: FF7 0x00654a26
void __cdecl Mini_HighwayUpdateActiveSlotTransforms(void)
{
    if (g_HighwaySlotStates[0].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotTransform(0);
    }
    if (g_HighwaySlotStates[1].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotTransform(1);
    }
    if (g_HighwaySlotStates[2].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotTransform(2);
    }
    if (g_HighwaySlotStates[3].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotTransform(3);
    }
    if (g_HighwaySlotStates[4].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotTransform(4);
    }
    if (g_HighwaySlotStates[5].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotTransform(5);
    }
}
/*
 * Recomputes rotation, position, and model matrices for a specific slot actor.
 */
// FUNCTION: FF7 0x00654a9d
void __cdecl Mini_HighwayUpdateSlotTransform(int slotIndex)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        int actorDistance;
        struct { unsigned char pad_to_4[0x4]; HighwayEffectPoolEntry *effectRecord; };
        struct { unsigned char pad_to_8[0x8]; int farPoint[3]; };
        struct { unsigned char pad_to_18[0x18]; int angleBetween; };
        struct { unsigned char pad_to_1C[0x1C]; HighwaySlotState *slotActor; };
        struct { unsigned char pad_to_20[0x20]; int clampedIndex; };
        struct { unsigned char pad_to_24[0x24]; HighwayRootPlacement *rootActor; };
        struct { unsigned char pad_to_28[0x28]; char angleInRange; };
        struct { unsigned char pad_to_2C[0x2C]; char moveFlag; };
        struct { unsigned char pad_to_30[0x30]; char neighborFlag; };
        struct { unsigned char pad_to_34[0x34]; short delta; };
        struct { unsigned char pad_to_38[0x38]; int nearPoint[3]; };
        struct { unsigned char pad_to_48[0x48]; HighwayRenderSlotActor *slotGroup; };
        struct { unsigned char pad_to_4C[0x4C]; int actorIndex; };
        struct { unsigned char pad_to_50[0x50]; short farAngles[4]; };
        struct { unsigned char pad_to_58[0x58]; short angles[4]; };
        struct { unsigned char pad_to_60[0x60]; char nearFlags; };
        struct { unsigned char pad_to_62[0x62]; char tmpFlagA; };
        struct { unsigned char pad_to_63[0x63]; char tmpFlagB; };
        struct { unsigned char pad_to_64[0x64]; char tmpFlagC; };
    } frame;
#pragma pack(pop)

    frame.slotActor = &g_HighwaySlotStates[slotIndex];
    frame.rootActor = (HighwayRootPlacement *)g_HighwaySlotHeads[slotIndex].actors[0];
    frame.slotGroup = &g_HighwaySlotHeads[slotIndex];
    switch (frame.slotActor->mode) {
    case HIGHWAY_SLOT_MODE_PLAYER:
        frame.slotActor->useConfigA = 0;
        g_pHighwayFieldManager->load(g_HighwaySlotBaseIndex + (frame.slotActor->posZ >> 8), frame.slotActor->posX >> 8, &frame.nearPoint[0], (int *)&frame.angles[0]);
        if ((g_HighwayInputButtonMask & HIGHWAY_BUTTON_RIGHT) != 0 && frame.slotActor->lateralOffset < g_HighwayActorZOffset) {
            frame.slotActor->lateralOffset += frame.slotActor->lateralStep;
        }
        if ((g_HighwayInputButtonMask & HIGHWAY_BUTTON_LEFT) != 0 && frame.slotActor->lateralOffset > -g_HighwayActorZOffset) {
            frame.slotActor->lateralOffset -= frame.slotActor->lateralStep;
        }
        if (g_HighwayInputDirection == HIGHWAY_DIR_NONE) {
            if (frame.slotActor->lateralOffset > frame.slotActor->lateralReturnStep) {
                frame.slotActor->lateralOffset -= frame.slotActor->lateralReturnStep;
            }
            if (frame.slotActor->lateralOffset < -frame.slotActor->lateralReturnStep) {
                frame.slotActor->lateralOffset += frame.slotActor->lateralReturnStep;
            }
            if (frame.slotActor->lateralOffset > -frame.slotActor->lateralReturnStep && frame.slotActor->lateralOffset < frame.slotActor->lateralReturnStep) {
                frame.slotActor->lateralOffset = 0;
            }
        }
        if (frame.slotActor->timer != 0) {
            frame.slotActor->useConfigA = 1;
            if (frame.slotActor->timer > 0) {
                (frame.slotActor->timer)--;
                frame.slotActor->configIndexA = 2;
                frame.slotActor->animFrameA = 29 - frame.slotActor->timer;
            }
            if (frame.slotActor->timer < 0) {
                (frame.slotActor->timer)++;
                frame.slotActor->configIndexA = 3;
                frame.slotActor->animFrameA = frame.slotActor->timer + 29;
            }
            frame.moveFlag = 0;
            if (frame.slotActor->timer < -10 && frame.slotActor->timer > -23) {
                frame.moveFlag = 1;
            }
            if (frame.slotActor->timer > 10 && frame.slotActor->timer < 20) {
                frame.moveFlag = 1;
            }
            if (frame.moveFlag != 0) {
                frame.moveFlag = 0;
                frame.neighborFlag = 0;
                frame.tmpFlagA = 0;
                frame.tmpFlagB = 0;
                frame.tmpFlagC = 0;
                Mini_HighwaySendChannelCommandA0(g_HighwaySoundParam, 2);
                Mini_HighwayDispatchSoundCategory(5, 2, 14);
                Mini_HighwaySendChannelCommandB0(0, 2);
                for (frame.actorIndex = 2; frame.actorIndex <= 5; frame.actorIndex++) {
                    if (g_HighwaySlotStates[frame.actorIndex].state == HIGHWAY_SLOT_STATE_NORMAL || g_HighwaySlotStates[frame.actorIndex].state == HIGHWAY_SLOT_STATE_HIT) {
                        frame.actorDistance = Mini_HighwayActorDistance(&g_HighwaySlotHeads[0], &g_HighwaySlotHeads[frame.actorIndex]);
                        if (frame.actorDistance < 160) {
                            frame.angleBetween = Mini_HighwayAngleBetweenRecords(&g_HighwaySlotHeads[0], &g_HighwaySlotHeads[frame.actorIndex]);
                            if (frame.angleBetween < 0) {
                                frame.angleBetween += 0x1000 /* 360 deg */;
                            }
                            frame.angleInRange = 0;
                            if (frame.slotActor->timer < 0) {
                                if (frame.angleBetween > 0 && frame.angleBetween < 0x384) {
                                    frame.angleInRange = 1;
                                }
                                if (frame.angleBetween > 0xE70 && frame.angleBetween < 0x1000) {
                                    frame.angleInRange = 1;
                                }
                            }
                            if (frame.slotActor->timer > 0 && frame.angleBetween > 0x47C && frame.angleBetween < 0x990) {
                                frame.angleInRange = 1;
                            }
                            if (frame.angleInRange == 1) {
                                (&frame.nearFlags)[frame.actorIndex] = 1;
                                frame.neighborFlag = 1;
                            }
                        }
                    }
                }
                if (frame.neighborFlag == 1) {
                    Mini_HighwaySendChannelCommandA0(g_HighwaySoundParam, 2);
                    Mini_HighwayDispatchSoundCategory(0x12, 2, -30);
                    Mini_HighwaySendChannelCommandB0(0, 2);
                    for (frame.actorIndex = 2; frame.actorIndex <= 4; frame.actorIndex++) {
                        if ((&frame.nearFlags)[frame.actorIndex] == 1) {
                            Mini_HighwayActorApplyHit(frame.actorIndex, 50);
                        }
                    }
                }
            }
        }
        if (frame.slotActor->counter == 0) {
            if (frame.slotActor->lateralOffset > g_HighwayActorZOffset) {
                frame.slotActor->lateralOffset = g_HighwayActorZOffset;
            }
            if (frame.slotActor->lateralOffset < -g_HighwayActorZOffset) {
                frame.slotActor->lateralOffset = -g_HighwayActorZOffset;
            }
            frame.delta = (short)frame.slotActor->lateralOffset;
            if (rand() % 10 == 0) {
                frame.slotActor->animDirty = 1;
            }
            if (rand() % 30 == 0) {
                if (frame.slotActor->animCountA == 0) {
                    Mini_HighwayDispatchSoundCategory(0x12D, 2, 0);
                }
                frame.slotActor->animCountA = 3;
            }
        } else {
            frame.delta = 0;
            if (frame.slotActor->counter > 1) {
                (frame.slotActor->counter)--;
            }
            frame.slotActor->useConfigA = 1;
            frame.slotActor->useConfigB = 1;
            frame.slotActor->configIndexA = 6;
            frame.slotActor->animFrameA = 73 - frame.slotActor->counter;
            frame.slotActor->configIndexB = 1;
            frame.slotActor->animFrameB = 73 - frame.slotActor->counter;
        }
        frame.angles[2] += frame.delta;
        if (frame.delta > -50 && frame.delta < 50) {
            frame.delta = 0;
        }
        frame.clampedIndex = (frame.delta >> 4) + (frame.slotActor->angle >> 10) + 18;
        if (frame.clampedIndex < g_HighwaySlotMinIndex) {
            frame.clampedIndex = g_HighwaySlotMinIndex;
        } else if (frame.clampedIndex > g_HighwaySlotMaxIndex) {
            frame.clampedIndex = g_HighwaySlotMaxIndex;
        }
        if (frame.delta != 0 && frame.slotActor->useConfigB == 0 && frame.slotActor->timer == 0) {
            frame.slotActor->useConfigA = 1;
            frame.slotActor->configIndexA = 1;
            frame.slotActor->animFrameA = frame.clampedIndex;
        }
        RotMatrix((SVECTOR *)frame.angles, frame.rootActor);
        frame.rootActor->posX = frame.nearPoint[0];
        frame.rootActor->posY = frame.nearPoint[1];
        frame.rootActor->posZ = frame.nearPoint[2];
        break;
    case HIGHWAY_SLOT_MODE_ESCORTED:
        g_pHighwayFieldManager->load(g_HighwaySlotBaseIndex + (frame.slotActor->posZ >> 8), frame.slotActor->posX >> 8, &frame.nearPoint[0], (int *)&frame.angles[0]);
        g_pHighwayFieldManager->load(g_HighwaySlotBaseIndex + (frame.slotActor->posZ >> 8) + 0xFA0, frame.slotActor->posX >> 8, &frame.farPoint[0], (int *)&frame.farAngles[0]);
        if (frame.slotActor->state == HIGHWAY_SLOT_STATE_HIT) {
            if ((frame.slotActor->stunTimer)-- == 0) {
                frame.slotActor->state = HIGHWAY_SLOT_STATE_NORMAL;
            }
            frame.slotActor->useConfigB = 1;
            frame.slotActor->configIndexB = 3;
            frame.slotActor->animFrameB = 18 - frame.slotActor->stunTimer;
        }
        if (frame.slotActor->animCountdown > 1) {
            if (frame.slotActor->animCountdown > 2) {
                (frame.slotActor->animCountdown)--;
            }
            frame.slotActor->useConfigB = 1;
            frame.slotActor->useConfigA = 1;
            frame.slotActor->configIndexB = 2;
            frame.slotActor->configIndexA = 1;
            frame.slotActor->animFrameB = 49 - frame.slotActor->animCountdown;
            frame.slotActor->animFrameA = 49 - frame.slotActor->animCountdown;
        }
        frame.delta = frame.farAngles[1] - frame.angles[1];
        if (frame.delta > 2048) {
            frame.delta -= 0x1000;
        }
        if (frame.delta < -2048) {
            frame.delta += 0x1000;
        }
        frame.angles[2] += (short)(-frame.delta >> 1);
        RotMatrix((SVECTOR *)frame.angles, frame.rootActor);
        frame.rootActor->posX = frame.nearPoint[0];
        frame.rootActor->posY = frame.nearPoint[1];
        frame.rootActor->posZ = frame.nearPoint[2];
        break;
    case HIGHWAY_SLOT_MODE_BOSS:
        g_pHighwayFieldManager->load(g_HighwaySlotBaseIndex + (frame.slotActor->posZ >> 8), frame.slotActor->posX >> 8, &frame.nearPoint[0], (int *)&frame.angles[0]);
        RotMatrix((SVECTOR *)frame.angles, frame.rootActor);
        frame.rootActor->posX = frame.nearPoint[0];
        frame.rootActor->posY = frame.nearPoint[1];
        frame.rootActor->posZ = frame.nearPoint[2];
        break;
    case HIGHWAY_SLOT_MODE_ENEMY:
        g_pHighwayFieldManager->load(g_HighwaySlotBaseIndex + (frame.slotActor->posZ >> 8), frame.slotActor->posX >> 8, &frame.nearPoint[0], (int *)&frame.angles[0]);
        g_pHighwayFieldManager->load(g_HighwaySlotBaseIndex + (frame.slotActor->posZ >> 8) + 0xFA0, frame.slotActor->posX >> 8, &frame.farPoint[0], (int *)&frame.farAngles[0]);
        frame.delta = frame.farAngles[1] - frame.angles[1] >> 1;
        if (frame.delta > 2048) {
            frame.delta -= 0x1000;
        }
        if (frame.delta < -2048) {
            frame.delta += 0x1000;
        }
        frame.angles[2] += (short)(frame.delta << 1);
        frame.clampedIndex = (frame.delta >> 4) + 9;
        if (frame.clampedIndex < 1) {
            frame.clampedIndex = 1;
        }
        if (frame.clampedIndex > 17) {
            frame.clampedIndex = 17;
        }
        if (frame.delta != 0 && frame.slotActor->useConfigA == 0) {
            frame.slotActor->useConfigA = 1;
            frame.slotActor->configIndexA = 1;
            frame.slotActor->animFrameA = frame.clampedIndex;
        }
        if (frame.slotActor->state == HIGHWAY_SLOT_STATE_HIT) {
            if ((frame.slotActor->timer)-- == 0) {
                frame.slotActor->state = HIGHWAY_SLOT_STATE_NORMAL;
            }
            frame.slotActor->useConfigA = 1;
            frame.slotActor->configIndexA = 2;
            frame.slotActor->animFrameA = 28 - frame.slotActor->timer;
            frame.slotActor->useConfigB = 1;
            frame.slotActor->configIndexB = 1;
            frame.slotActor->animFrameB = 28 - frame.slotActor->timer;
        }
        if (frame.slotActor->state == HIGHWAY_SLOT_STATE_DESTROYED) {
            if ((frame.slotActor->counter)-- == 0) {
                Mini_HighwayDespawnSlot(slotIndex);
            }
            frame.slotActor->useConfigA = 1;
            frame.slotActor->configIndexA = frame.slotActor->variant + 3;
            frame.slotActor->animFrameA = 59 - frame.slotActor->counter;
            frame.slotActor->useConfigB = 1;
            frame.slotActor->configIndexB = frame.slotActor->variant + 2;
            frame.slotActor->animFrameB = 59 - frame.slotActor->counter;
            frame.slotActor->lateralOffset += 0x200;
            frame.slotActor->posZ -= frame.slotActor->lateralOffset;
            frame.effectRecord = (HighwayEffectPoolEntry *)Mini_HighwaySpawnEffect(frame.nearPoint[0], frame.nearPoint[1], frame.nearPoint[2], 2, 0xAA);
            frame.effectRecord->rotX = frame.angles[0];
            frame.effectRecord->rotY = frame.angles[1];
            frame.effectRecord->rotZ = frame.angles[2];
        }
        RotMatrix((SVECTOR *)frame.angles, frame.rootActor);
        frame.rootActor->posX = frame.nearPoint[0];
        frame.rootActor->posY = frame.nearPoint[1];
        frame.rootActor->posZ = frame.nearPoint[2];
    }
}
/*
 * Iterates through active actor slots and issues draw commands for each actor.
 */
// FUNCTION: FF7 0x00655667
void __cdecl Mini_HighwayRenderActiveSlotActors(void)
{
    if (g_HighwaySlotStates[0].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayRenderSlotActor(0);
    }
    if (g_HighwaySlotStates[1].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayRenderSlotActor(1);
    }
    if (g_HighwaySlotStates[2].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayRenderSlotActor(2);
    }
    if (g_HighwaySlotStates[3].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayRenderSlotActor(3);
    }
    if (g_HighwaySlotStates[4].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayRenderSlotActor(4);
    }
    if (g_HighwaySlotStates[5].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayRenderSlotActor(5);
    }
}
/*
 * Renders the 3D model and animated meshes for a specific slot actor.
 */
// FUNCTION: FF7 0x006556de
void __cdecl Mini_HighwayRenderSlotActor(int slotIndex)
{
    struct {
        int temp16c;
        int temp168;
        HighwayAnimItem *temp164;
        int temp160;
        HighwayAnimItem *temp15c;
        HighwayAnimItem *temp158;
        int temp154;
        HighwayAnimItem *temp150;
        struct PolygonModelVertexColors *temp14c;
        struct PolygonModelVertexColors *temp148;
        int temp144;
        int temp140;
        HighwayAnimItem *temp13c;
        int temp138;
        HighwayAnimItem *temp134;
        HighwayAnimItem *temp130;
        int temp12c;
        int temp128;
        HighwayAnimItem *temp124;
        struct PolygonModelVertexColors *temp120;
        struct PolygonModelVertexColors *temp11c;
        float secondTint[4];
        float firstTint[4];
        float combined[16];
        HighwaySlotState *state;
        int psx[8];
        int secondId;
        float converted[16];
        int *matrix;
        HighwayRenderSlotActor *header;
        int index;
        float rotation[16];
        int firstId;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.matrix = &frame.psx[0];
    frame.state = &g_HighwaySlotStates[slotIndex];
    frame.header = &g_HighwaySlotHeads[slotIndex];
    frame.firstId = frame.state->firstId;
    frame.secondId = frame.state->secondId;
    ((HighwayModelNode *)g_HighwaySlotHeads[slotIndex].actors[0])->adjustedMatrix(&frame.psx[0]);
    frame.matrix = &frame.psx[0];
    for (frame.index = 1; frame.index < frame.state->partCount; frame.index++) {
        ((HighwayModelNode *)frame.header->actors[frame.index])->render(0);
    }
    GtePsxMatrixToFloat4x4Transposed(frame.matrix, &frame.converted[0]);
    BuildRotationYRowVec(180.0f, &frame.rotation[0]);
    MultiplyMatrix4x4Full(&frame.rotation[0], &frame.converted[0], &frame.combined[0]);
    if (frame.firstId != 0x13) {
        if (frame.header->isVisible != 0) {
            frame.temp11c = frame.header->animItemB->colors;
            Gfx_RestoreModelVertexColors(frame.temp11c, g_HighwayActorContext);
            frame.header->isVisible = 0;
        } else if (frame.header->hideQueued != 0) {
            frame.firstTint[0] = 0.3f;
            frame.firstTint[1] = 0.3f;
            frame.firstTint[2] = 0.3f;
            frame.firstTint[3] = 1.0f;
            frame.temp120 = frame.header->animItemB->colors;
            Gfx_AddColorToModelVertexColors((GfxFloatColorAddend *)frame.firstTint, frame.temp120, g_HighwayActorContext);
            frame.header->isVisible = 1;
            frame.header->hideQueued = 0;
        }
        if (frame.state->useConfigB == 0) {
            frame.temp128 = *(int *)((char *)&g_HighwayActorConfigTable[7] + slotIndex * 0x38);
            frame.temp124 = frame.header->animItemB;
            frame.temp124->tableIndex = frame.temp128;
            frame.temp130 = frame.header->animItemB;
            (frame.temp130->cursor)++;
            frame.temp12c = frame.temp130->entryTable[frame.temp130->tableIndex]->count;
            frame.temp130->cursor = frame.temp130->cursor % frame.temp12c;
        } else {
            frame.temp138 = *(int *)((char *)&g_HighwayActorConfigTable[7] + (slotIndex * 0x38 + frame.state->configIndexB * 4));
            frame.temp134 = frame.header->animItemB;
            frame.temp134->tableIndex = frame.temp138;
            frame.temp140 = frame.state->animFrameB;
            frame.temp13c = frame.header->animItemB;
            frame.temp13c->cursor = frame.temp140;
            frame.state->useConfigB = 0;
        }
        frame.temp144 = frame.header->animItemB->player;
        MultiplyMatrix4x4Full(&frame.combined[0], g_pHighwayFileBlock->baseMatrix, (void *)(frame.temp144 + 0x64));
    }
    if (frame.secondId != 0x13) {
        if (frame.header->isFrozen != 0) {
            frame.temp148 = frame.header->animItemA->colors;
            Gfx_RestoreModelVertexColors(frame.temp148, g_HighwayActorContext);
            frame.header->isFrozen = 0;
        } else if (frame.header->freezeQueued != 0) {
            frame.secondTint[0] = 0.3f;
            frame.secondTint[1] = 0.3f;
            frame.secondTint[2] = 0.3f;
            frame.secondTint[3] = 1.0f;
            frame.temp14c = frame.header->animItemA->colors;
            Gfx_AddColorToModelVertexColors((GfxFloatColorAddend *)frame.secondTint, frame.temp14c, g_HighwayActorContext);
            frame.header->isFrozen = 1;
            frame.header->freezeQueued = 0;
        }
        if (frame.state->useConfigA == 0) {
            frame.temp150 = frame.header->animItemA;
            frame.temp150->tableIndex = 0;
            frame.temp158 = frame.header->animItemA;
            (frame.temp158->cursor)++;
            frame.temp154 = frame.temp158->entryTable[frame.temp158->tableIndex]->count;
            frame.temp158->cursor = frame.temp158->cursor % frame.temp154;
        } else {
            frame.temp160 = *(int *)((char *)&g_HighwayActorConfigTable + (slotIndex * 0x38 + frame.state->configIndexA * 4));
            frame.temp15c = frame.header->animItemA;
            frame.temp15c->tableIndex = frame.temp160;
            frame.temp168 = frame.state->animFrameA;
            frame.temp164 = frame.header->animItemA;
            frame.temp164->cursor = frame.temp168;
            frame.state->useConfigA = 0;
        }
        frame.temp16c = frame.header->animItemA->player;
        MultiplyMatrix4x4Full(&frame.combined[0], g_pHighwayFileBlock->baseMatrix, (void *)(frame.temp16c + 0x64));
    }
}
/*
 * Updates animation frames and pose interpolation for all active slot actors.
 */
// FUNCTION: FF7 0x00655b9e
void __cdecl Mini_HighwayUpdateActiveSlotAnimations(void)
{
    if (g_HighwaySlotStates[0].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotAnimationCounters(0);
    }
    if (g_HighwaySlotStates[1].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotAnimationCounters(1);
    }
    if (g_HighwaySlotStates[2].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotAnimationCounters(2);
    }
    if (g_HighwaySlotStates[3].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotAnimationCounters(3);
    }
    if (g_HighwaySlotStates[4].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotAnimationCounters(4);
    }
    if (g_HighwaySlotStates[5].state != HIGHWAY_SLOT_STATE_INACTIVE) {
        Mini_HighwayUpdateSlotAnimationCounters(5);
    }
}
/*
 * Advances the animation cycle counter and frame indices for a specific slot actor.
 */
// FUNCTION: FF7 0x00655c15
void __cdecl Mini_HighwayUpdateSlotAnimationCounters(int slotIndex)
{
    HighwayRenderSlotActor *slotRecord;
    HighwaySlotState *slotState;

    slotState = &g_HighwaySlotStates[slotIndex];
    slotRecord = &g_HighwaySlotHeads[slotIndex];
    switch (slotState->mode) {
    case HIGHWAY_SLOT_MODE_PLAYER:
        if (slotState->animCountA != 0) {
            (slotState->animCountA)--;
            ((HighwayModelNode *)slotRecord->actors[3])->select((short)(g_HighwaySlotActor3ModelBase - slotState->animCountA));
            ((HighwayModelNode *)slotRecord->actors[3])->render(0);
        }
        if (slotState->animDirty != 0) {
            slotState->animDirty = 0;
            (slotState->animIndex)++;
            slotState->animIndex = slotState->animIndex % 3;
            ((HighwayModelNode *)slotRecord->actors[2])->select((short)(g_HighwaySlotActor2ModelBase + slotState->animIndex));
            ((HighwayModelNode *)slotRecord->actors[2])->render(0);
        }
        break;
    case HIGHWAY_SLOT_MODE_BOSS:
        if (slotState->animCountB != 0) {
            (slotState->animCountB)--;
            ((HighwayModelNode *)slotRecord->actors[1])->select((short)(g_HighwaySlotActor1ModelBase - slotState->animCountB));
            ((HighwayModelNode *)slotRecord->actors[1])->render(0);
        }
        break;
    }
}
/*
 * Computes angular heading and relative angle between two actor records.
 */
// FUNCTION: FF7 0x00655d63
int __cdecl Mini_HighwayAngleBetweenRecords(HighwayRenderSlotActor *fromRecord, HighwayRenderSlotActor *toRecord)
{
    int deltaZ;
    int deltaX;

    deltaX = toRecord->statePosX - fromRecord->statePosX;
    deltaZ = toRecord->statePosZ - fromRecord->statePosZ;
    return ArcTangentFixed(deltaZ, deltaX);
}
/*
 * Calculates Euclidean distance between two actor records on the track plane.
 */
// FUNCTION: FF7 0x00655da7
int __cdecl Mini_HighwayActorDistance(HighwayRenderSlotActor *groupA, HighwayRenderSlotActor *groupB)
{
    int dist;
    struct HighwayModelBatchContext *rootA;
    struct HighwayModelBatchContext *actorB;

    rootA = (struct HighwayModelBatchContext *)(groupA->actors[0]);
    actorB = (struct HighwayModelBatchContext *)(groupB->actors[0]);
    dist = SqrtToInt((actorB->posX - rootA->posX) * (actorB->posX - rootA->posX) + (actorB->posY - rootA->posY) * (actorB->posY - rootA->posY) + (actorB->posZ - rootA->posZ) * (actorB->posZ - rootA->posZ));
    return dist;
}
/*
 * Computes collision detection segment endpoints for an actor\'s bounding box.
 */
// FUNCTION: FF7 0x00655e29
void __cdecl Mini_HighwayActorComputeEndpoints(short slotIndex, HighwayEndPoint *startPoint, HighwayEndPoint *endPoint, void *flag)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[28];
        SVECTOR endOffsetX;
        struct { unsigned char pad_to_8[0x8]; SVECTOR startOffsetX; };
        struct { unsigned char pad_to_10[0x10]; int boxExtentX; };
        struct { unsigned char pad_to_14[0x14]; HighwayRootActorShortPos *actorRecord; };
        struct { unsigned char pad_to_18[0x18]; int boxExtentZ; };
    } frame;
#pragma pack(pop)

    frame.boxExtentX = g_HighwaySlotStates[slotIndex].segmentStartZ;
    frame.boxExtentZ = g_HighwaySlotStates[slotIndex].segmentEndZ;
    frame.startOffsetX.vx = 0;
    frame.startOffsetX.vy = 0;
    frame.startOffsetX.vz = (short)frame.boxExtentX;
    frame.endOffsetX.vx = 0;
    frame.endOffsetX.vy = 0;
    frame.endOffsetX.vz = (short)frame.boxExtentZ;
    *(short *)flag = 0;
    frame.actorRecord = (HighwayRootActorShortPos *)g_HighwaySlotHeads[slotIndex].actors[0];
    TransformVectorToInt16((void *)frame.actorRecord, &frame.startOffsetX, (void *)startPoint);
    TransformVectorToInt16((void *)frame.actorRecord, &frame.endOffsetX, (void *)endPoint);
    startPoint->posX += frame.actorRecord->offsetX;
    startPoint->posY += frame.actorRecord->offsetY;
    startPoint->posZ += frame.actorRecord->offsetZ;
    endPoint->posX += frame.actorRecord->offsetX;
    endPoint->posY += frame.actorRecord->offsetY;
    endPoint->posZ += frame.actorRecord->offsetZ;
}
/*
 * Checks pairwise collisions among active highway actors and resolves impacts.
 */
// FUNCTION: FF7 0x00655f3e
void __cdecl Mini_HighwayActorsCollidePairs(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[100];
        short otherIndex;
        struct { unsigned char pad_to_4[0x4]; int paramA; };
        struct { unsigned char pad_to_8[0x8]; int paramB; };
        struct { unsigned char pad_to_C[0xC]; int distance; };
        struct { unsigned char pad_to_10[0x10]; short actorIndex; };
        struct { unsigned char pad_to_14[0x14]; short fieldShorts[4]; };
        struct { unsigned char pad_to_1C[0x1C]; short actorCount; };
        struct { unsigned char pad_to_20[0x20]; int distSq; };
        struct { unsigned char pad_to_24[0x24]; int fieldWords[3]; };
        struct { unsigned char pad_to_34[0x34]; HighwaySlotState *otherActor; };
        struct { unsigned char pad_to_38[0x38]; HighwaySlotState *actor; };
        struct { unsigned char pad_to_3C[0x3C]; short endB[4]; };
        struct { unsigned char pad_to_44[0x44]; int impactAngle; };
        struct { unsigned char pad_to_48[0x48]; int radiusSum; };
        struct { unsigned char pad_to_4C[0x4C]; short endA[4]; };
        struct { unsigned char pad_to_54[0x54]; short startB[4]; };
        struct { unsigned char pad_to_5C[0x5C]; short startA[4]; };
    } frame;
#pragma pack(pop)

    frame.actorCount = 6;
    for (frame.actorIndex = 0; frame.actorIndex < frame.actorCount; frame.actorIndex++) {
        frame.actor = &g_HighwaySlotStates[frame.actorIndex];
        if (frame.actor->state != HIGHWAY_SLOT_STATE_INACTIVE) {
            Mini_HighwayActorComputeEndpoints((short)frame.actorIndex, (struct HighwayEndPoint *)&frame.startA[0], (struct HighwayEndPoint *)&frame.endA[0], &frame.paramA);
            for (frame.otherIndex = frame.actorIndex + 1; frame.otherIndex < frame.actorCount; frame.otherIndex++) {
                frame.otherActor = &g_HighwaySlotStates[frame.otherIndex];
                if (frame.otherActor->state != HIGHWAY_SLOT_STATE_INACTIVE) {
                    Mini_HighwayActorComputeEndpoints((short)frame.otherIndex, (struct HighwayEndPoint *)&frame.startB[0], (struct HighwayEndPoint *)&frame.endB[0], &frame.paramB);
                    g_pHighwayFieldManager->load(g_HighwaySlotBaseIndex + frame.otherActor->posZ, frame.otherActor->posX, &frame.fieldWords[0], (int *)&frame.fieldShorts[0]);
                    frame.impactAngle = 0xFFFF;
                    frame.radiusSum = frame.actor->startRadius + frame.otherActor->startRadius + frame.actor->extraRadius + frame.otherActor->extraRadius;
                    frame.distSq = (frame.startB[0] - frame.startA[0]) * (frame.startB[0] - frame.startA[0]) + (frame.startB[1] - frame.startA[1]) * (frame.startB[1] - frame.startA[1]) + (frame.startB[2] - frame.startA[2]) * (frame.startB[2] - frame.startA[2]);
                    if (frame.distSq >= 0) {
                        frame.distance = SqrtToInt(frame.distSq);
                        if (frame.distance < frame.radiusSum) {
                            frame.impactAngle = ArcTangentFixed(frame.startB[2] - frame.startA[2], frame.startB[0] - frame.startA[0]) + frame.fieldShorts[1] + -0x800;
                        }
                    }
                    if (frame.impactAngle == 0xFFFF) {
                        frame.radiusSum = frame.actor->endRadius + frame.otherActor->startRadius + frame.actor->extraRadius + frame.otherActor->extraRadius;
                        frame.distSq = (frame.startB[0] - frame.endA[0]) * (frame.startB[0] - frame.endA[0]) + (frame.startB[1] - frame.endA[1]) * (frame.startB[1] - frame.endA[1]) + (frame.startB[2] - frame.endA[2]) * (frame.startB[2] - frame.endA[2]);
                        if (frame.distSq >= 0) {
                            frame.distance = SqrtToInt(frame.distSq);
                            if (frame.distance < frame.radiusSum) {
                                frame.impactAngle = ArcTangentFixed(frame.startB[2] - frame.endA[2], frame.startB[0] - frame.endA[0]) + frame.fieldShorts[1] + -0x800;
                            }
                        }
                    }
                    if (frame.impactAngle == 0xFFFF) {
                        frame.radiusSum = frame.actor->startRadius + frame.otherActor->endRadius + frame.actor->extraRadius + frame.otherActor->extraRadius;
                        frame.distSq = (frame.endB[0] - frame.startA[0]) * (frame.endB[0] - frame.startA[0]) + (frame.endB[1] - frame.startA[1]) * (frame.endB[1] - frame.startA[1]) + (frame.endB[2] - frame.startA[2]) * (frame.endB[2] - frame.startA[2]);
                        if (frame.distSq >= 0) {
                            frame.distance = SqrtToInt(frame.distSq);
                            if (frame.distance < frame.radiusSum) {
                                frame.impactAngle = ArcTangentFixed(frame.endB[2] - frame.startA[2], frame.endB[0] - frame.startA[0]) + frame.fieldShorts[1] + -0x800;
                            }
                        }
                    }
                    if (frame.impactAngle == 0xFFFF) {
                        frame.radiusSum = frame.actor->endRadius + frame.otherActor->endRadius + frame.actor->extraRadius + frame.otherActor->extraRadius;
                        frame.distSq = (frame.endB[0] - frame.endA[0]) * (frame.endB[0] - frame.endA[0]) + (frame.endB[1] - frame.endA[1]) * (frame.endB[1] - frame.endA[1]) + (frame.endB[2] - frame.endA[2]) * (frame.endB[2] - frame.endA[2]);
                        if (frame.distSq >= 0) {
                            frame.distance = SqrtToInt(frame.distSq);
                            if (frame.distance < frame.radiusSum) {
                                frame.impactAngle = ArcTangentFixed(frame.endB[2] - frame.endA[2], frame.endB[0] - frame.endA[0]) + frame.fieldShorts[1] + -0x800;
                            }
                        }
                    }
                    if (frame.impactAngle != 0xFFFF) {
                        Mini_HighwayActorApplyCollisionVelocity((short)frame.actorIndex, (short)frame.otherIndex, frame.impactAngle);
                    }
                }
            }
        }
    }
}
/*
 * Applies collision impulse and bounce velocity to an actor following an impact.
 */
// FUNCTION: FF7 0x00656361
void __cdecl Mini_HighwayActorApplyCollisionVelocity(short slotA, short slotB, int angle)
{
    int impulseZ;
    int impact;
    int impulseX;

    impact = 0;
    if (slotA == 0 && slotB != 1) {
        impact = 1;
    }
    if (slotA == 1 && slotB != 1) {
        impact = 1;
    }
    if (impact == 1) {
        Mini_HighwayDispatchSoundCategory(0x91, 2, 5);
        Mini_HighwaySendChannelCommandA0(0x6F, 2);
        Mini_HighwaySendChannelCommandB0(0, 2);
    }
    angle = angle % 0x1000;
    if (angle < 0) {
        angle += 0x1000 /* 360 deg */;
    }
    impulseZ = SinFixed(angle) * 0x1388 >> 11;
    impulseX = CosFixed(angle) * 0x1388 >> 12;
    if (slotA != 0) {
        g_HighwaySlotStates[slotA].velocityX = impulseX;
        g_HighwaySlotStates[slotA].velocityZ = impulseZ;
    } else {
        g_HighwaySlotStates[slotA].velocityX = impulseX >> 1;
        g_HighwaySlotStates[slotA].velocityZ = impulseZ >> 1;
    }
    g_HighwaySlotStates[slotB].velocityX = -impulseX;
    g_HighwaySlotStates[slotB].velocityZ = -impulseZ;
    if (slotA > 1 && slotB == 5) {
        g_HighwaySlotStates[slotA].velocityX = impulseX << 1;
        g_HighwaySlotStates[slotA].velocityZ = impulseZ << 1;
        g_HighwaySlotStates[slotB].velocityX = 0;
        g_HighwaySlotStates[slotB].velocityZ = 0;
    }
    if (g_HighwaySlotStates[slotB].state == HIGHWAY_SLOT_STATE_DESTROYED && g_HighwaySlotStates[slotA].state != HIGHWAY_SLOT_STATE_DESTROYED && slotA > 1 && slotB > 1) {
        Mini_HighwayActorApplyHit(slotA, 15);
    }
    if (g_HighwaySlotStates[slotA].state == HIGHWAY_SLOT_STATE_DESTROYED && g_HighwaySlotStates[slotB].state != HIGHWAY_SLOT_STATE_DESTROYED && slotA > 1 && slotB > 1) {
        Mini_HighwayActorApplyHit(slotB, 15);
    }
    if (slotA == 1 && slotB != 0) {
        Mini_HighwayActorDirectionalCue(slotB, angle);
    }
    if (slotA == 0) {
        if (slotB != 1) {
            Mini_HighwayDamageCharacter(0, 64);
        }
        if (g_HighwaySlotStates[slotB].state != HIGHWAY_SLOT_STATE_DESTROYED && slotB != 1) {
            Mini_HighwayActorApplyHit(slotB, 1);
        }
    }
    if (slotB == 5) {
        Mini_HighwayActorApplyHit(slotA, 20);
    }
}
/*
 * Applies a collision hit of `amount` to a slot's actor: mode 1 stuns it, mode 2 takes hit points.
 */
// FUNCTION: FF7 0x00656620
void __cdecl Mini_HighwayActorApplyHit(int slotIndex, int amount)
{
    HighwayRenderSlotActor *header;
    HighwaySlotState *slotPointer;
    int angle;
    int sceneKind;

    slotPointer = &g_HighwaySlotStates[slotIndex];
    header = &g_HighwaySlotHeads[slotIndex];
    switch (slotPointer->mode) {
    case HIGHWAY_SLOT_MODE_PLAYER:
        break;
    case HIGHWAY_SLOT_MODE_ESCORTED:
        angle = Mini_HighwayAngleBetweenRecords(&g_HighwaySlotHeads[slotIndex], &g_HighwaySlotHeads[1]);
        slotPointer->state = HIGHWAY_SLOT_STATE_HIT;
        slotPointer->stunTimer = 18;
        break;
    case HIGHWAY_SLOT_MODE_ENEMY:
        if (slotPointer->hp < 1) {
            g_HighwayCurrentScore += 500;
            Mini_HighwayDispatchSoundCategory(0x138, 1, 10);
            slotPointer->state = HIGHWAY_SLOT_STATE_DESTROYED;
            slotPointer->variant = rand() % 3;
            slotPointer->counter = 59;
            slotPointer->lateralOffset = 0;
            slotPointer->partCount -= 1;
            if (g_HighwayActiveSlotCount != 1) {
                sceneKind = g_pHighwaySceneBlock->sceneKind;
                if (sceneKind == 1) {
                    ((HighwayEffect *)g_pHighwaySceneBlock)->init((char)(rand() % 2 + 3), 4, 0x9C40);
                }
            }
            g_HighwayActiveSlotCount--;
        } else {
            slotPointer->hp -= amount;
            slotPointer->state = HIGHWAY_SLOT_STATE_HIT;
            slotPointer->timer = 28;
            header->freezeQueued = 1;
        }
        break;
    }
}
/*
 * Evaluates directional proximity cues and steers AI actors relative to target.
 */
// FUNCTION: FF7 0x006567b0
void __cdecl Mini_HighwayActorDirectionalCue(int slotIndex, int angle)
{
    HighwayRenderSlotActor *target;

    target = &g_HighwaySlotHeads[1];
    if (g_HighwayCurrentScore > 0) {
        g_HighwayCurrentScore -= 50;
    }
    angle += 0x800 /* 180 deg */;
    angle = angle % 0x1000;
    if (angle >= 0 && angle < 0x400) {
        Mini_HighwayDamageCharacter(2, 400);
    }
    if (angle >= 0x400 && angle < 0x800) {
        Mini_HighwayDamageCharacter(3, 400);
    }
    if (angle >= 0x800 && angle < 0xC00) {
        Mini_HighwayDamageCharacter(1, 400);
    }
    if (angle >= 0xC00 && angle < 0x1000) {
        Mini_HighwayDamageCharacter(4, 400);
    }
    target->hideQueued = 1;
}
/*
 * Integrates linear and angular motion vectors for an actor over the current time step.
 */
// FUNCTION: FF7 0x00656880
void __cdecl Mini_HighwayActorIntegrateMotion(int slotIndex, int unused)
{
    int targetAngle;
    int value;
    int speed;
    int targetDistance;
    int slotDistance;
    HighwaySlotState *actor;
    int slotAngle;

    value = 0;
    actor = &g_HighwaySlotStates[slotIndex];
    if (actor->isFlyingAway != 0) {
        speed = 4000;
        actor->targetAngle = 0xC00;
        actor->velocityX += CosFixed(actor->targetAngle) * speed >> 12;
        actor->velocityZ += SinFixed(actor->targetAngle) * speed >> 11;
        actor->velocityX = (actor->velocityX << 3) / 10;
        actor->velocityZ = (actor->velocityZ << 3) / 10;
        actor->posX += actor->velocityX;
        actor->posZ += actor->velocityZ;
        if (actor->posZ < -5000) {
            Mini_HighwayDespawnSlot(slotIndex);
        }
    } else {
        switch (actor->aiMode) {
        case HIGHWAY_SLOT_MODE_BOSS:
            speed = 1000;
            if (actor->state == HIGHWAY_SLOT_STATE_HIT) {
                speed = 0;
            }
            targetAngle = Mini_HighwayAngleBetweenRecords(&g_HighwaySlotHeads[slotIndex], &g_HighwaySlotHeads[1]);
            slotAngle = Mini_HighwayAngleBetweenRecords(&g_HighwaySlotHeads[slotIndex], &g_HighwaySlotHeads[0]);
            targetDistance = Mini_HighwayActorDistance(&g_HighwaySlotHeads[slotIndex], &g_HighwaySlotHeads[1]);
            slotDistance = Mini_HighwayActorDistance(&g_HighwaySlotHeads[slotIndex], &g_HighwaySlotHeads[0]);
            /* Numeric active enemy bike count (1..3). */
            switch ((char)g_HighwayActiveSlotCount) {
            case 1:
                speed = 1000;
                actor->targetAngle = targetAngle;
            case 2:
                speed = 1000;
                actor->targetAngle = targetAngle;
            case 3:
                speed = 1000;
                actor->targetAngle = targetAngle;
            }
            if (slotDistance < 500 && g_HighwaySlotStates[0].timer != 0) {
                speed = 1300;
                actor->targetAngle = slotAngle + 0x800;
            }
            if (actor->minPosZ > actor->posZ) {
                speed = 2000;
                actor->targetAngle = 0x400;
            }
            actor->velocityX += CosFixed(actor->targetAngle) * speed >> 12;
            actor->velocityZ += SinFixed(actor->targetAngle) * speed >> 11;
            actor->velocityX = (actor->velocityX << 3) / 10;
            actor->velocityZ = (actor->velocityZ << 3) / 10;
            actor->posX += actor->velocityX;
            actor->posZ += actor->velocityZ;
            break;
        case HIGHWAY_SLOT_MODE_BOSS_CHARGE:
            speed = 800;
            if (actor->state == HIGHWAY_SLOT_STATE_HIT) {
                speed = 0;
            }
            actor->targetAngle = Mini_HighwayAngleBetweenRecords(&g_HighwaySlotHeads[slotIndex], &g_HighwaySlotHeads[1]);
            if (actor->minPosZ > actor->posZ) {
                speed = 2000;
                actor->targetAngle = 0x400;
            }
            actor->velocityX += CosFixed(actor->targetAngle) * speed >> 12;
            actor->velocityZ += SinFixed(actor->targetAngle) * speed >> 11;
            actor->velocityX = (actor->velocityX << 3) / 10;
            actor->velocityZ = (actor->velocityZ << 3) / 10;
            actor->posX += actor->velocityX;
            actor->posZ += actor->velocityZ;
            break;
        case HIGHWAY_SLOT_MODE_BOSS_RETREAT:
            speed = 800;
            if (actor->state == HIGHWAY_SLOT_STATE_HIT) {
                speed = 0;
            }
            targetAngle = Mini_HighwayAngleBetweenRecords(&g_HighwaySlotHeads[slotIndex], &g_HighwaySlotHeads[1]);
            slotAngle = Mini_HighwayAngleBetweenRecords(&g_HighwaySlotHeads[slotIndex], &g_HighwaySlotHeads[0]);
            if (actor->wanderTimer == 0 && rand() % 100 == 0) {
                actor->wanderTimer = rand() % 90;
            }
            if (actor->wanderTimer == 0) {
                actor->targetAngle = targetAngle;
            } else {
                actor->targetAngle = slotAngle + 0x200;
                (actor->wanderTimer)--;
            }
            if (actor->minPosZ > actor->posZ) {
                speed = 2000;
                actor->targetAngle = 0x400;
            }
            actor->velocityX += CosFixed(actor->targetAngle) * speed >> 12;
            actor->velocityZ += SinFixed(actor->targetAngle) * speed >> 11;
            actor->velocityX = (actor->velocityX << 3) / 10;
            actor->velocityZ = (actor->velocityZ << 3) / 10;
            actor->posX += actor->velocityX;
            actor->posZ += actor->velocityZ;
        }
        if (actor->posZ > actor->maxPosZ) {
            actor->posZ = actor->maxPosZ;
        }
    }
}
} /* extern "C" */
