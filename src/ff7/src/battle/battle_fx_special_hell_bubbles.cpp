// src/ff7/src/battle/battle_fx_special_hell_bubbles.cpp
/* Enemy Special Effect [idx 79]: HellBubbles (JP: awa2 / 泡2 (Bubble 2), asset: "SPECIAL/AWA2") — Hell Bubbles (0x127, Ceasar), Bubble (0x149, Blugu) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Awa2 attack: it loads resources and spawns a per-target
//   timeline that draws textured quad strips and particle sprites, with a particle emitter that
//   claims records from a shared pool and a fading spinning model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxHellBubblesParticleRecord;
struct GfxPsxSpriteQuadState;
struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a3ec8
int g_BattleSpecHellBubblesResourceDefs = 0x001900B1;
char *g_BattleFxSpecialHellBubblesResourceNames[1] = {
    // STRING: FF7 0x008a3f58
    "SUNA2.RSD"
};
unsigned int g_BattleSpecHellBubblesResourceDefsDataTail[18] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3f64
    (unsigned int)"AWA.TIM",
    // STRING: FF7 0x008a3f6c
    (unsigned int)"MARU.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3f78
    (unsigned int)"AWA.TIM",
    // STRING: FF7 0x008a3f80
    (unsigned int)"BAKU1.SPT",
    0x00000000,
    0x00000000,
    0x00193a39,
    // STRING: FF7 0x008a3f8c
    (unsigned int)"MIZU_1.TIM",
    0x00000000,
    0x00000000,
    0x00000000
};
#undef g_BattleSpecHellBubblesSpriteReq1
// GLOBAL: FF7 0x008a3f18
struct BattleSpecHellBubblesSpriteReq1 g_BattleSpecHellBubblesSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x008a3f30
struct BattleSpecHellBubblesSpriteReq2 g_BattleSpecHellBubblesSpriteReq2 = {-128, -128, -128, {0x2e, 0, 0, 0, 0}};
// GLOBAL: FF7 0x008a3f38
int g_BattleSpecHellBubblesModelScale = 6656;
// GLOBAL: FF7 0x008a3f3c
int g_BattleSpecHellBubblesModelStep = 4096;
// GLOBAL: FF7 0x008a3f40
struct BattleSpecHellBubblesSpriteReq3 g_BattleSpecHellBubblesSpriteReq3 = {0x1a00, {0}, 0, {8, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_008a3f54[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// clang-format on


// clang-format off
#define g_BattleSpecHellBubblesParticleRecordsAngle4 (((BattleSpecHellBubblesParticleRecords *)&(((BattleSpecHellBubblesParticleRecords *)&g_BattleSpecHellBubblesParticleRecords)->recordBase))->angle4)
#define g_BattleSpecHellBubblesParticleRecordsPhase6 (((BattleSpecHellBubblesParticleRecordsObject *)&(((BattleSpecHellBubblesParticleRecords *)&g_BattleSpecHellBubblesParticleRecords)->recordBase))->phase6)
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00b68998
short g_BattleSpecHellBubblesDrawListHead = 0;
// GLOBAL: FF7 0x00b6899c
short g_BattleSpecHellBubblesTargetMask = 0;
// GLOBAL: FF7 0x00b689a0
int g_BattleSpecHellBubblesPoolActiveFlag = 0;
// GLOBAL: FF7 0x00b689a4
short g_BattleSpecHellBubblesParticleLoopIndex = 0;
// GLOBAL: FF7 0x00b689a8
struct BattleSpecHellBubblesParticleRecordEntry g_BattleSpecHellBubblesParticleRecords[2] = {0};
unsigned char g_bssPad_00b689b8[1584] = {0};
// GLOBAL: FF7 0x00b68fe8
struct GfxTableCursor *g_BattleSpecHellBubblesLoadedResTable = 0;
unsigned char g_bssPad_00b68fec[4] = {0};
// clang-format on

/*
 * When the engine context exists and g_BattleSpecHellBubblesLoadedResTable is empty, builds SPECIAL/AWA2 with a default-
 * duration record and creates resource type 4 using g_BattleSpecHellBubblesResourceDefs.
 */
// FUNCTION: FF7 0x0053eec0
void __cdecl BattleFx_SpecialLoadResHellBubbles(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char resourceName[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecHellBubblesLoadedResTable == 0) {
        kind = 4;
        resourceDefsWord = (int)&g_BattleSpecHellBubblesResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a3f98
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/AWA2", resourceName);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecHellBubblesLoadedResTable, curOwnerCtx);
    }
}

/*
 * Entry wrapper: loads the resources, resets the strip state and registers the per-target callbacks.
 */
// FUNCTION: FF7 0x0053ef50
void __cdecl BattleFx_SpecialEnterHellBubbles(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResHellBubbles();
    g_BattleSpecHellBubblesDrawListHead = 0;
    g_BattleSpecHellBubblesTargetMask = 0xA00;
    g_BattleSpecHellBubblesParticleLoopIndex = 0;
    BattleFx_SpecialRegisterHellBubblesPerTargetCallback(targetMask, actorIdx);
}

/*
 * Creates the per-target spawn callbacks, plays the sound at the source and frees all 200 particle records.
 */
// FUNCTION: FF7 0x0053ef85
void __cdecl BattleFx_SpecialRegisterHellBubblesPerTargetCallback(int targetIndex, int sourceIndex)
{
    short i;

    Battle_CreateBitPatternCallbackTask(targetIndex, sourceIndex, 4, BattleFx_SpecialSpawnHellBubblesPerTarget);
    Battle_PlaySfxPannedToActor(0x11F, (short)sourceIndex, 0x40);
    for (i = 0; i < 200; i++) {
        g_BattleSpecHellBubblesParticleRecords[i].active = 0;
    }
}

/*
 * Allocates a timeline task whose start is the source's bone position and whose end is the
 * target's bone position, and stores the target actor in it.
 */
// FUNCTION: FF7 0x0053efe8
void __cdecl BattleFx_SpecialSpawnHellBubblesPerTarget(int actor, int sourceIndex)
{
    BattleFxSpecialHellBubblesTask *taskPtr;

    taskPtr = (BattleFxSpecialHellBubblesTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHellBubblesTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone33, &taskPtr->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &taskPtr->endX);
    taskPtr->actorIndex = actor;
}

/*
 * Timeline task of one target: starts the particle emitter and the fading spinning model, then the quad strip and sprites around the impact point, and ends after 70 frames.
 */
// FUNCTION: FF7 0x0053f05e
void __cdecl BattleFx_SpecialHellBubblesTimelineTask(void)
{
    short heading;
    short resolvedDeltaY;
    short curDeltaX;
    short dist;
    BattleFxSpecialHellBubblesTask *taskSlot;
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxHellBubblesTimeline *child;

    taskSlot = (BattleFxSpecialHellBubblesTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskSlot->timer == 0) {
        heading = ArcTangentFixed(taskSlot->endX - taskSlot->startX, taskSlot->endY - taskSlot->startY);
        child = (BattleFxHellBubblesTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHellBubblesParticleEmitterTask);
        child->posX = taskSlot->startX;
        child->jitteredX = taskSlot->baseX;
        child->posY = taskSlot->startY;
        child->stepX = (taskSlot->endX - taskSlot->startX) / 20;
        child->baseX = 0;
        child->stepY = (taskSlot->endY - taskSlot->startY) / 20;
        child->rotY = heading;
        child->phase = 0;
        child = (BattleFxHellBubblesTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHellBubblesFadingSpinModelTask);
        child->posX = taskSlot->startX;
        child->jitteredX = taskSlot->baseX;
        child->posY = taskSlot->startY;
        child->stepY = 1024;
        child->baseX = heading - 0x400;
        curDeltaX = taskSlot->endX - taskSlot->startX;
        resolvedDeltaY = taskSlot->endY - taskSlot->startY;
        dist = SqrtToInt(curDeltaX * curDeltaX + resolvedDeltaY * resolvedDeltaY);
        child->posZ = dist * 5376 / 2800;
        child->phase = 0;
    }
    if (taskSlot->timer == 60) {
        Battle_PostActorHitOrHurtMessage(taskSlot->actorIndex);
    }
    if (taskSlot->timer == 30) {
        child = (BattleFxHellBubblesTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHellBubblesQuadStripTask);
        child->posX = taskSlot->endX;
        child->jitteredX = taskSlot->jitterBaseX;
        child->posY = taskSlot->endY;
        child->posZ = 0;
        child->stripPhase = 0;
        g_BattleFxTimelineState.channel1 = 0;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        g_BattleFxTimelineState.channel2 = 0x20;
        Battle_StartFixedPointRamp(0x1000, 10);
    }
    if (taskSlot->timer > 20 && taskSlot->timer < 40) {
        child = (BattleFxHellBubblesTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHellBubblesAnimatedSpriteEntry2Task);
        child->posX = taskSlot->endX + rand() % 1400 + -700;
        child->jitteredX = taskSlot->jitterBaseX + rand() % 1400 + -700;
        child->posY = taskSlot->endY + rand() % 1400 + -700;
    }
    (taskSlot->timer)++;
    if (taskSlot->timer == 70) {
        Battle_StartFixedPointRamp(0, 10);
        taskSlot->step = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one animated sprite (entry 2 of the resource table) at the slot's position and frees the
 * slot after the animation's last frame.
 */
// FUNCTION: FF7 0x0053f3b8
void __cdecl BattleFx_SpecialHellBubblesAnimatedSpriteEntry2Task(void)
{
    BattleFxHellBubblesQuadStrip *task;

    task = (BattleFxHellBubblesQuadStrip *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecHellBubblesSpriteReq1.resPtr) = Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecHellBubblesLoadedResTable);
    g_BattleSpecHellBubblesSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecHellBubblesLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x1000 /* 1.0 */, 0);
    g_BattleSpecHellBubblesSpriteReq1.frame = task->frame;
    g_BattleSpecHellBubblesPoolActiveFlag = BattleFx_SubmitSpriteDraw(&g_BattleSpecHellBubblesSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHellBubblesPoolActiveFlag);
    if (g_BattleIsPaused == 0 && (g_BattleSpecHellBubblesSpriteReq1.resPtr) != 0) {
        (task->frame)++;
        if (task->frame >= *(int *)((char *)*(int *)(g_BattleSpecHellBubblesSpriteReq1.resPtr) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws the wavy quad strip at the slot's position, growing it for 15 frames and shrinking it after
 * frame 0x20, and frees the slot at frame 0x28.
 */
// FUNCTION: FF7 0x0053f489
void __cdecl BattleFx_SpecialHellBubblesQuadStripTask(void)
{
    BattleFxHellBubblesQuadStrip *task;

    task = (BattleFxHellBubblesQuadStrip *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, g_BattleSpecHellBubblesTargetMask, -1000);
    RefreshGlobalTransformState();
    BattleFx_SpecialHellBubblesDrawPsxTexturedQuadStrip(8, task->stripOffset, 0x100, g_BattleSpecHellBubblesDrawListHead);
    task->stripOffset += 0x100;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frame < 15) {
        g_BattleSpecHellBubblesDrawListHead += 0x10;
    }
    if (task->frame > 32) {
        g_BattleSpecHellBubblesTargetMask -= 0x12C;
        g_BattleSpecHellBubblesDrawListHead -= 0x1E;
    }
    (task->frame)++;
    if (task->frame == 40) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a wavy strip of 32 textured quads, displaced by a sine of the phase.
 */
// FUNCTION: FF7 0x0053f572
void __cdecl BattleFx_SpecialHellBubblesDrawPsxTexturedQuadStrip(short amplitude, short phase, short angleStep, short intensity)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        int stateResource;
        struct { unsigned char pad_to_4[0x4]; int quadOutput; };
        struct { unsigned char pad_to_8[0x8]; int projected; };
        struct { unsigned char pad_to_C[0xC]; short attrWord; };
        struct { unsigned char pad_to_10[0x10]; BattleFxHellBubblesQuadStripRenderStateBlock *renderBlock; };
        struct { unsigned char pad_to_14[0x14]; SVECTOR botRightX; };
        struct { unsigned char pad_to_1C[0x1C]; SVECTOR botLeftX; };
        struct { unsigned char pad_to_24[0x24]; short stripIndex; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR topRightX; };
        struct { unsigned char pad_to_30[0x30]; SVECTOR topLeftX; };
        struct { unsigned char pad_to_38[0x38]; int rgbColor; };
        struct { unsigned char pad_to_3C[0x3C]; short bandParam; };
        struct { unsigned char pad_to_40[0x40]; short waveNext; };
        struct { unsigned char pad_to_44[0x44]; int quadDepth; };
        struct { unsigned char pad_to_48[0x48]; short waveCur; };
    } frame;
#pragma pack(pop)

    frame.rgbColor = intensity << 0x10 | 0x2E000000 | intensity << 8 | intensity;
    frame.bandParam = 0;
    frame.attrWord = 0x7908;
    frame.renderBlock = (BattleFxHellBubblesQuadStripRenderStateBlock *)Gfx_GetPsxPacketScratchPtr();
    *(char *)((char *)frame.renderBlock + 7) |= 2;
    for (frame.stripIndex = 0; frame.stripIndex < 32; frame.stripIndex++) {
        frame.waveCur = SinFixed(phase + frame.stripIndex * angleStep) * amplitude >> 12;
        frame.waveNext = SinFixed(phase + (frame.stripIndex + 1) * angleStep) * amplitude >> 12;
        frame.topLeftX.vx = frame.waveCur - 120;
        frame.topRightX.vx = frame.waveCur + 120;
        frame.botLeftX.vx = frame.waveNext - 120;
        frame.botRightX.vx = frame.waveNext + 120;
        frame.topRightX.vy = ((frame.stripIndex * 240) / 32) - 120;
        frame.topLeftX.vy = frame.topRightX.vy;
        frame.botRightX.vy = (((frame.stripIndex + 1) * 240) / 32) - 120;
        frame.botLeftX.vy = frame.botRightX.vy;
        frame.topLeftX.vx <<= 3;
        frame.topRightX.vx <<= 3;
        frame.botLeftX.vx <<= 3;
        frame.botRightX.vx <<= 3;
        frame.topLeftX.vy <<= 3;
        frame.topRightX.vy <<= 3;
        frame.botLeftX.vy <<= 3;
        frame.botRightX.vy <<= 3;
        frame.botRightX.vz = 0;
        frame.botLeftX.vz = frame.botRightX.vz;
        frame.topRightX.vz = frame.botLeftX.vz;
        frame.topLeftX.vz = frame.topRightX.vz;
        frame.projected = Gfx_ProjectQuadToScreenAlt(&frame.topLeftX, &frame.topRightX, &frame.botLeftX, &frame.botRightX, (char *)frame.renderBlock + 8, (char *)frame.renderBlock + 0x10, (char *)frame.renderBlock + 0x18, (char *)frame.renderBlock + 0x20, &frame.quadDepth, &frame.quadOutput);
        if (frame.projected > 0) {
            frame.renderBlock->tag = 0x9000000;
            frame.renderBlock->rgbCode = frame.rgbColor;
            frame.renderBlock->u2 = 0;
            frame.renderBlock->u0 = 0;
            frame.renderBlock->u3 = 0xF0;
            frame.renderBlock->u1 = 0xF0;
            frame.renderBlock->bandTop = (frame.stripIndex * 240) / 32;
            frame.renderBlock->v0 = frame.renderBlock->bandTop;
            frame.renderBlock->bandBottom = ((frame.stripIndex + 1) * 240) / 32;
            frame.renderBlock->v2 = frame.renderBlock->bandBottom;
            frame.renderBlock->texturePage = frame.bandParam;
            frame.renderBlock->clut = frame.attrWord;
            frame.stateResource = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecHellBubblesLoadedResTable);
            if (frame.stateResource != 0) {
                Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)frame.stateResource);
            }
            Gfx_DrawPsxPrimitive(frame.renderBlock);
        }
    }
    g_BattleSpecHellBubblesPoolActiveFlag = (int)frame.renderBlock;
}

/*
 * Emitter task: draws the live particles, claims four new records per frame early in its life and ages the live ones until they expire.
 */
// FUNCTION: FF7 0x0053f840
void __cdecl BattleFx_SpecialHellBubblesParticleEmitterTask(void)
{
    BattleFxHellBubblesParticleRecord *taskPtr;
    short i, activeCnt;
    BattleFxClaimedParticleRecord *claimed;

    taskPtr = (BattleFxHellBubblesParticleRecord *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_SpecialHellBubblesDrawParticleSprites((BattleFxHellBubblesParticleRecord *)taskPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    taskPtr->angle += 8;
    if (taskPtr->age > 0 && taskPtr->age < 25) {
        for (i = 0; i < 4; i++) {
            claimed = (BattleFxClaimedParticleRecord *)BattleFx_SpecialHellBubblesClaimParticleRecord();
            if (claimed != 0) {
                claimed->radius = rand() % 800;
                claimed->angle = (rand() % 4096);
            }
        }
    }
    i = 0;
    for (activeCnt = 0; i < 200; i++) {
        if (g_BattleSpecHellBubblesParticleRecords[i].active != 0) {
            activeCnt++;
            *(short *)((char *)&g_BattleSpecHellBubblesParticleRecordsAngle4 + i * 8) -= 128;
            (g_BattleSpecHellBubblesParticleRecords[i].phase)++;
            if (g_BattleSpecHellBubblesParticleRecords[i].phase == 0x19) {
                g_BattleSpecHellBubblesParticleRecords[i].active = 0;
            }
        }
    }
    (taskPtr->age)++;
    if (taskPtr->age > 25 && activeCnt == 0) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Claims the first free particle record (active == 0) among the 200 of g_BattleSpecHellBubblesParticleRecords,
 * clears its phase and returns its address; returns zero when full.
 */
// FUNCTION: FF7 0x0053f9c9
int __cdecl BattleFx_SpecialHellBubblesClaimParticleRecord(void)
{
    short i;

    for (i = 0; i < 200; i++) {
        if (g_BattleSpecHellBubblesParticleRecords[i].active == 0) {
            g_BattleSpecHellBubblesParticleRecords[i].active = 1;
            *(short *)((char *)&g_BattleSpecHellBubblesParticleRecordsPhase6 + i * 8) = 0;
            return (int)((char *)&(((BattleSpecHellBubblesParticleRecords *)&g_BattleSpecHellBubblesParticleRecords)->recordBase) + i * 8);
        }
    }
    return 0;
}

/*
 * Draws a billboard sprite for every live particle record of the emitter, fading them over their life.
 */
// FUNCTION: FF7 0x0053fa2e
void __cdecl BattleFx_SpecialHellBubblesDrawParticleSprites(BattleFxHellBubblesParticleRecord *emitter)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        int frameValue;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; int depth; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR posX; };
        struct { unsigned char pad_to_18[0x18]; short activeCount; };
        struct { unsigned char pad_to_20[0x20]; short frameIndex; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_44[0x44]; short particleIndex; };
    } frame;
#pragma pack(pop)

    g_BattleSpecHellBubblesSpriteReq1.secondSpriteRecord = Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecHellBubblesLoadedResTable);
    g_BattleSpecHellBubblesSpriteReq1.secondDrawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecHellBubblesLoadedResTable);
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    frame.rotX.vy = emitter->rotY;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[2] = 0;
    frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
    frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
    frame.particleIndex = 0;
    for (frame.activeCount = 0; frame.particleIndex < 200; frame.particleIndex++) {
        if (g_BattleSpecHellBubblesParticleRecords[frame.particleIndex].active != 0) {
            frame.activeCount++;
            frame.posX.vx = CosFixed(g_BattleSpecHellBubblesParticleRecords[frame.particleIndex].angle) * g_BattleSpecHellBubblesParticleRecords[frame.particleIndex].radius * g_BattleSpecHellBubblesParticleRecords[frame.particleIndex].phase / 20 * (emitter->angle + 256) >> 20;
            frame.posX.vy = SinFixed(g_BattleSpecHellBubblesParticleRecords[frame.particleIndex].angle) * g_BattleSpecHellBubblesParticleRecords[frame.particleIndex].radius * g_BattleSpecHellBubblesParticleRecords[frame.particleIndex].phase / 20 * (emitter->angle + 256) >> 20;
            frame.posX.vz = 0;
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            RotTransPointToInt16(&frame.posX, (short *)&frame.posX, &frame.depth);
            frame.posX.vx += (short)(emitter->baseA + emitter->ampA * g_BattleSpecHellBubblesParticleRecords[frame.particleIndex].phase);
            frame.posX.vy += (short)(emitter->baseB + emitter->ampB * g_BattleSpecHellBubblesParticleRecords[frame.particleIndex].phase);
            frame.posX.vz += (short)(emitter->baseC + emitter->ampC * g_BattleSpecHellBubblesParticleRecords[frame.particleIndex].phase);
            if (g_BattleSpecHellBubblesParticleRecords[frame.particleIndex].phase < 17) {
                frame.frameValue = 8;
            } else {
                frame.frameValue = 25 - g_BattleSpecHellBubblesParticleRecords[frame.particleIndex].phase;
            }
            frame.frameIndex = *(short *)&frame.frameValue;
            BattleFx_InstallBillboardMatrix(&frame.posX, (g_BattleSpecHellBubblesParticleRecords[frame.particleIndex].phase << 6) + 0x800, 0);
            g_BattleSpecHellBubblesSpriteReq2.highByte2 = frame.frameIndex << 4;
            g_BattleSpecHellBubblesSpriteReq2.lowByte1 = g_BattleSpecHellBubblesSpriteReq2.highByte2;
            (g_BattleSpecHellBubblesSpriteReq2.red) = g_BattleSpecHellBubblesSpriteReq2.lowByte1;
            g_BattleSpecHellBubblesPoolActiveFlag = BattleFx_SubmitSpriteDraw(&g_BattleSpecHellBubblesSpriteReq1.secondSpriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHellBubblesPoolActiveFlag);
        }
    }
}

/*
 * Draws the spinning model that grows and fades over 45 frames.
 */
// FUNCTION: FF7 0x0053fce6
void __cdecl BattleFx_SpecialHellBubblesFadingSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_30[0x30]; BattleFxHellBubblesFadingSpinModelSlot *spinSlot; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxHellBubblesFadingSpinModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecHellBubblesSpriteReq3.modelHandle = Gfx_ModelTableGetRenderObject(0, g_BattleSpecHellBubblesLoadedResTable);
    frame.spinSlot->angle = -frame.spinSlot->frameCounter << 7;
    RotMatrix((SVECTOR *)&frame.spinSlot->angle, &frame.rotMatrix);
    g_BattleSpecHellBubblesModelStep = frame.spinSlot->step;
    (g_BattleSpecHellBubblesSpriteReq3.modelScale) = (frame.spinSlot->phase + 0x100) * 0x1A00 >> 8;
    g_BattleSpecHellBubblesModelScale = (g_BattleSpecHellBubblesSpriteReq3.modelScale);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecHellBubblesModelScale);
    frame.rotMatrix.t[0] = frame.spinSlot->posX;
    frame.rotMatrix.t[1] = frame.spinSlot->posY;
    frame.rotMatrix.t[2] = frame.spinSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecHellBubblesSpriteReq3.alpha = g_BattleSpecHellBubblesParticleLoopIndex << 4;
    g_BattleSpecHellBubblesPoolActiveFlag = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecHellBubblesSpriteReq3.modelHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHellBubblesPoolActiveFlag);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spinSlot->phase += 8;
    if (frame.spinSlot->frameCounter < 8) {
        g_BattleSpecHellBubblesParticleLoopIndex++;
    }
    if (frame.spinSlot->frameCounter > 37) {
        g_BattleSpecHellBubblesParticleLoopIndex--;
    }
    (frame.spinSlot->frameCounter)++;
    if (frame.spinSlot->frameCounter == 45) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
