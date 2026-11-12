// src/ff7/src/battle/battle_fx_special_big_wave.cpp
/* Enemy Special Effect [idx 26]: BigWave (JP: tunami / 津波 (Tsunami), asset: "SPECIAL/TUNAMI") — Big Wave (0x1d5, 0x2f9, Bottomswell / Submarine Crew), Huge Tidal Wave (0x293), Leviathan Scales (0x364) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Tunami attack: it builds a spawn timeline and a wave
//   surface task whose vertex heights come from a wave height helper, alongside an animated sprite
//   task and a UV byte-pair helper.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include "lib/codegen.h"
#include <string.h>
#include <stdlib.h>
extern "C" {

struct GfxPsxShortVertexSlot;
struct GfxPsxSpriteQuadState;
// clang-format off
// GLOBAL: FF7 0x008a4288
int g_BattleSpecBigWaveResourceDefs = 0x00192A39;
char *g_BattleFxSpecialBigWaveResourceNames[1] = {
    // STRING: FF7 0x008a42f0
    "BAKU1.TIM"
};
typedef struct {
    char *filename;
    unsigned int descriptor;
    char *s2;
} BattleSpecialBigWaveResourceTailEntry;
BattleSpecialBigWaveResourceTailEntry g_BattleFxSpecialBigWaveResourceNames2[6] = {
    {
        // STRING: FF7 0x008a42fc
        "BAKU1.SPT",
        0x00000000,
        0
    },
    {
        (char *)0x00192a39,
        0x00000000,
        // STRING: FF7 0x008a4308
        "JO_C03.SPT"
    },
    {
        0,
        0x00000000,
        (char *)0x00197a3c
    },
    {
        // STRING: FF7 0x008a4314
        "1.TIM",
        0x00000000,
        0
    },
    {
        0,
        0x00197a39,
        // STRING: FF7 0x008a431c
        "1.TIM"
    },
    {
        0,
        0x00000000,
        0
    }
};
// GLOBAL: FF7 0x008a42d8
short g_BattleSpecBigWaveWaveActiveFlag = 1;
// GLOBAL: FF7 0x008a42dc
short g_BattleSpecBigWaveWaveHeightBase = 400;
// GLOBAL: FF7 0x008a42e0
BattleFxSpriteRequest g_BattleSpecBigWaveSpriteReq1 = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x00b6ac40
int g_BattleSpecBigWaveDrawListHead = 0;
// GLOBAL: FF7 0x00b6ac44
short g_BattleSpecBigWaveLoadedResTable = 0;
// g_BattleSpecBigWaveWaveTable1 is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0xb6ac48..0xb6ae48 are aliases.
// GLOBAL: FF7 0x00b6ac48
unsigned char g_BattleSpecBigWaveWaveTable1[0x200] = {0};
// g_BattleSpecBigWaveWaveTable2 is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0xb6ae48..0xb6cc48 are aliases.
// GLOBAL: FF7 0x00b6ae48
unsigned char g_BattleSpecBigWaveWaveTable2[0x1e00] = {0};
// GLOBAL: FF7 0x00b6cc48
struct GfxTableCursor *g_BattleSpecBigWaveWaveProgress = 0;
// GLOBAL: FF7 0x00b6cc4c
short g_BattleSpecBigWaveWavePhase = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)
#define PACKED_FRAME_MEMBER(type, field, padding) \
    struct {                                      \
        unsigned char pad[padding];               \
        type value;                               \
    } field


/*
 * Loads the SPECIAL/TUNAMI resources once the battle scene exists and they are not loaded yet.
 */
// FUNCTION: FF7 0x00541810
void __cdecl BattleFx_SpecialLoadResBigWave(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecBigWaveWaveProgress == 0) {
        kind = 4;
        resourceDefsWord = (int)&g_BattleSpecBigWaveResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a4324
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/TUNAMI", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecBigWaveWaveProgress, curOwnerCtx);
    }
}

/*
 * Loads the Tunami resources, resets the wave (active, phase 0, height base 0x190) and starts the
 * timeline.
 */
// FUNCTION: FF7 0x005418a0
void __cdecl BattleFx_SpecialEnterBigWave(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResBigWave();
    g_BattleSpecBigWaveWaveActiveFlag = 1;
    g_BattleSpecBigWaveWavePhase = 0;
    g_BattleSpecBigWaveWaveHeightBase = 0x190;
    BattleFx_SpecialBigWaveSpawnTimeline(targetMask, actorIdx);
}

/*
 * Stores the target mask, fills the wave sine table, and allocates the timeline task with the
 * source bone position and the targets' centroid. The wave direction flips when the source is
 * farther than the targets. Plays the effect sound at the source actor.
 */
// FUNCTION: FF7 0x005418d5
void __cdecl BattleFx_SpecialBigWaveSpawnTimeline(int actorMask, int sourceIndex)
{
    short idx;
    BattleFxBigWaveTimeline *slotPtr;

    g_BattleSpecBigWaveLoadedResTable = actorMask;
    for (idx = 0; idx < 0x1000; idx++) {
        ((short *)&g_BattleSpecBigWaveWaveTable1)[idx] = SinFixed(idx);
    }
    slotPtr = (BattleFxBigWaveTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBigWaveTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone34, &slotPtr->startX);
    Battle_GetActorMaskCentroid(actorMask, &slotPtr->targetX);
    if (slotPtr->timer > slotPtr->limit) {
        g_BattleSpecBigWaveWaveActiveFlag *= -1;
    }
    Battle_PlaySfxPannedToActor(0x132, sourceIndex, 0x40);
}

/*
 * Runs the 0x3C-frame timeline: starts the wave surface and the fixed-point ramp, scatters animated
 * sprites over the targets in the middle frames and creates their typed records at frame 0x28.
 */
// FUNCTION: FF7 0x005419af
void __cdecl BattleFx_SpecialBigWaveTimelineTask(void)
{
    BattleTaskSlot *p;
    short actorIdx;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBigWaveSlot *child;

    p = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (p->frameCounter == 0) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        Battle_StartFixedPointRamp(0x1000, 4);
    }
    if (p->frameCounter == 0x14) {
        Battle_StartFixedPointRamp(0, 8);
    }
    if (p->frameCounter == 0) {
        child = (BattleFxBigWaveSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBigWaveWaveSurfaceTask);
    }
    if (p->frameCounter > 0x24 && p->frameCounter < 0x2C) {
        for (actorIdx = 0; actorIdx < 0xA; actorIdx++) {
            if ((g_BattleSpecBigWaveLoadedResTable & 1 << actorIdx) != 0) {
                child = (BattleFxBigWaveSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBigWaveAnimatedSpriteTask);
                Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, &child->posX);
                child->posX += (short)(rand() % 0x258 - 0x12C);
                child->posY += (short)(rand() % 0x258 - 0x12C);
                child->posZ += (short)(rand() % 0x258 - 0x12C);
                child->active = 1;
                child = (BattleFxBigWaveSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBigWaveAnimatedSpriteTask);
                Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, &child->posX);
                child->posX += (short)(rand() % 0x578 - 0x2BC);
                child->posY += (short)(rand() % 0x578 - 0x2BC);
                child->posZ += (short)(rand() % 0x578 - 0x2BC);
                child->active = 0;
            }
        }
    }
    if (p->frameCounter == 0x28) {
        for (actorIdx = 0; actorIdx < 0xA; actorIdx++) {
            if ((g_BattleSpecBigWaveLoadedResTable & 1 << actorIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(actorIdx);
            }
        }
    }
    (p->frameCounter)++;
    if (p->frameCounter == 0x3C) {
        p->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one animated sprite at its position (larger when its variant is set), variant 1 taking the
 * scene's own draw object; frees the slot when the animation has played all its frames.
 */
// FUNCTION: FF7 0x00541c9a
void __cdecl BattleFx_SpecialBigWaveAnimatedSpriteTask(void)
{
    BattleFxOilSpriteSlot *task;

    task = (BattleFxOilSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, (task->variant != 0 ? 0x1000 : 0) + 0x1800, 0);
    g_BattleSpecBigWaveSpriteReq1.frame = task->frameIndex;
    if (task->variant == 1) {
        g_BattleSpecBigWaveSpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecBigWaveWaveProgress);
        g_BattleSpecBigWaveSpriteReq1.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    } else {
        g_BattleSpecBigWaveSpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecBigWaveWaveProgress);
        g_BattleSpecBigWaveSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecBigWaveWaveProgress);
    }
    g_BattleSpecBigWaveDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecBigWaveSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecBigWaveDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecBigWaveSpriteReq1.spriteGroup != 0) {
        (task->frameIndex)++;
        if (task->frameIndex == g_BattleSpecBigWaveSpriteReq1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws the wave surface for 0x32 frames as a grid of textured quads whose heights come from
 * BattleFx_SpecialBigWaveGetWaveVertexHeight, fading the wave in and out through the wave phase.
 */
// FUNCTION: FF7 0x00541db9
void __cdecl BattleFx_SpecialBigWaveWaveSurfaceTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        int quadState;
        struct { unsigned char pad_to_4[0x4]; short waveLevel; };
        struct { unsigned char pad_to_C[0xC]; int elementCount; };
        struct { unsigned char pad_to_10[0x10]; short waveAttr; };
        struct { unsigned char pad_to_18[0x18]; BattleFxBigWaveWavePacket *renderBlock; };
        struct { unsigned char pad_to_1C[0x1C]; SVECTOR x3; };
        struct { unsigned char pad_to_24[0x24]; short colIndex; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR x2; };
        struct { unsigned char pad_to_30[0x30]; short rowIndex; };
        struct { unsigned char pad_to_34[0x34]; SVECTOR x1; };
        PACKED_FRAME_MEMBER(short, at30, 54);
        struct { unsigned char pad_to_3C[0x3C]; SVECTOR x0; };
        PACKED_FRAME_MEMBER(short, at22, 62);
        struct { unsigned char pad_to_44[0x44]; BattleTaskSlot *waveSlot; };
        struct { unsigned char pad_to_48[0x48]; short cellRow; };
        struct { unsigned char pad_to_4C[0x4C]; short waveBias; };
    } frame;
#pragma pack(pop)

    frame.waveSlot = BATTLE_CURRENT_TASK_SLOT;
    frame.waveLevel = frame.waveSlot->frameCounter * g_BattleSpecBigWaveWaveHeightBase - 0x3E8;
    frame.waveBias = 0;
    frame.waveAttr = 0x7808;
    frame.renderBlock = (BattleFxBigWaveWavePacket *)Gfx_GetPsxPacketScratchPtr();
    frame.renderBlock->color0.cd |= 2;
    frame.renderBlock->tag = frame.renderBlock->tag & 0xFFFFFF | 0x9000000;
    frame.renderBlock->color0.cd = 0x2C;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    for (frame.rowIndex = 0xFFFC; frame.rowIndex < 0x10; frame.rowIndex++) {
        for (frame.colIndex = 0; frame.colIndex < 0x10; frame.colIndex++) {
            frame.cellRow = frame.rowIndex - frame.waveLevel / 0x3E8;
            frame.x0.vx = (frame.colIndex - 8) * 0x3E8;
            frame.x0.vz = (frame.waveLevel + (frame.cellRow - 8) * 0x3E8) * g_BattleSpecBigWaveWaveActiveFlag;
            frame.x1.vx = (frame.colIndex - 8) * 0x3E8 + 0x3E8;
            frame.x1.vz = (frame.waveLevel + (frame.cellRow - 8) * 0x3E8) * g_BattleSpecBigWaveWaveActiveFlag;
            frame.x2.vx = (frame.colIndex - 8) * 0x3E8;
            frame.x2.vz = (frame.waveLevel + (frame.cellRow - 8) * 0x3E8 + 0x3E8) * g_BattleSpecBigWaveWaveActiveFlag;
            frame.x3.vx = (frame.colIndex - 8) * 0x3E8 + 0x3E8;
            frame.x3.vz = (frame.waveLevel + (frame.cellRow - 8) * 0x3E8 + 0x3E8) * g_BattleSpecBigWaveWaveActiveFlag;
            frame.at22.value = (short)BattleFx_SpecialBigWaveGetWaveVertexHeight(frame.waveSlot->frameCounter * 0xA0 - frame.cellRow * 0x190, (short)frame.colIndex) - 0xC8;
            frame.x2.vy = (short)BattleFx_SpecialBigWaveGetWaveVertexHeight(frame.waveSlot->frameCounter * 0xA0 - (frame.cellRow + 1) * 0x190, (short)frame.colIndex) - 0xC8;
            frame.at30.value = (short)BattleFx_SpecialBigWaveGetWaveVertexHeight(frame.waveSlot->frameCounter * 0xA0 - frame.cellRow * 0x190, frame.colIndex + 1) - 0xC8;
            frame.x3.vy = (short)BattleFx_SpecialBigWaveGetWaveVertexHeight(frame.waveSlot->frameCounter * 0xA0 - (frame.cellRow + 1) * 0x190, frame.colIndex + 1) - 0xC8;
            StoreDeferredDrawArgs(&frame.x0, &frame.x2, &frame.x1);
            ApplyDeferredDrawArgs();
            SetPendingResourceHandle((GfxPsxShortVertexSlot *)&frame.x3);
            ApplyPendingResourceHandle();
            Gfx_ReadTransformOrderingDepth2(&frame.elementCount);
            if (frame.elementCount > 0) {
                frame.renderBlock->tag = 0x9000000;
                frame.renderBlock->colorCommand = g_BattleSpecBigWaveWavePhase * 0x10101 + (g_BattleSpecBigWaveWavePhase - 0x80 != 0 ? 0x2E000000 : 0x2C000000);
                frame.renderBlock->u0 = frame.colIndex << 6;
                frame.renderBlock->v0 = frame.cellRow << 6;
                frame.renderBlock->u1 = frame.colIndex + 1 << 6;
                frame.renderBlock->v1 = frame.cellRow << 6;
                frame.renderBlock->u2 = frame.colIndex << 6;
                frame.renderBlock->v2 = frame.cellRow + 1 << 6;
                frame.renderBlock->u3 = frame.colIndex + 1 << 6;
                frame.renderBlock->v3 = frame.cellRow + 1 << 6;
                if (frame.renderBlock->u1 == 0) {
                    frame.renderBlock->u3 = 0xFF;
                    frame.renderBlock->u1 = 0xFF;
                }
                if (frame.renderBlock->v2 == 0) {
                    frame.renderBlock->v3 = 0xFF;
                    frame.renderBlock->v2 = 0xFF;
                }
                if ((frame.colIndex & 7) >= 4) {
                    BattleFx_SpecialBigWaveInvertUvBytePair(&frame.renderBlock->u0, &frame.renderBlock->u1);
                    BattleFx_SpecialBigWaveInvertUvBytePair(&frame.renderBlock->u2, &frame.renderBlock->u3);
                }
                if ((frame.cellRow & 7) >= 4) {
                    BattleFx_SpecialBigWaveInvertUvBytePair(&frame.renderBlock->v0, &frame.renderBlock->v2);
                    BattleFx_SpecialBigWaveInvertUvBytePair(&frame.renderBlock->v1, &frame.renderBlock->v3);
                }
                frame.renderBlock->waveBias = frame.waveBias;
                frame.renderBlock->waveAttr = frame.waveAttr;
                if (g_BattleSpecBigWaveWavePhase == 0x80) {
                    frame.quadState = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecBigWaveWaveProgress);
                    if (frame.quadState != 0) {
                        Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)frame.quadState);
                    }
                } else {
                    frame.quadState = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecBigWaveWaveProgress);
                    if (frame.quadState != 0) {
                        Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)frame.quadState);
                    }
                }
                Gfx_DrawPsxPrimitive(frame.renderBlock);
            }
        }
    }
    frame.waveLevel = frame.waveSlot->frameCounter * 0x19;
    if (DEAD_IN_RETAIL != 0) {
        for (frame.rowIndex = 0xFFFE; frame.rowIndex < 0x10; frame.rowIndex++) {
            for (frame.colIndex = 0; frame.colIndex < 0x10; frame.colIndex++) {
                frame.x0.vx = (frame.colIndex - 8) * 0x3E8;
                frame.x0.vz = (frame.waveLevel + (frame.rowIndex - 8) * 0x3E8) * g_BattleSpecBigWaveWaveActiveFlag;
                frame.x1.vx = (frame.colIndex - 8) * 0x3E8 + 0x3E8;
                frame.x1.vz = (frame.waveLevel + (frame.rowIndex - 8) * 0x3E8) * g_BattleSpecBigWaveWaveActiveFlag;
                frame.x2.vx = (frame.colIndex - 8) * 0x3E8;
                frame.x2.vz = (frame.waveLevel + (frame.rowIndex - 8) * 0x3E8 + 0x3E8) * g_BattleSpecBigWaveWaveActiveFlag;
                frame.x3.vx = (frame.colIndex - 8) * 0x3E8 + 0x3E8;
                frame.x3.vz = (frame.waveLevel + (frame.rowIndex - 8) * 0x3E8 + 0x3E8) * g_BattleSpecBigWaveWaveActiveFlag;
                frame.x3.vy = 0;
                frame.x2.vy = frame.x3.vy;
                frame.at30.value = frame.x2.vy;
                frame.at22.value = frame.at30.value;
                StoreDeferredDrawArgs(&frame.x0, &frame.x1, &frame.x2);
                ApplyDeferredDrawArgs();
                Gfx_SetPendingVertexInput(&frame.x3);
                Gfx_TransformPendingVertexSlot0();
                Gfx_ReadTransformOrderingDepth2(&frame.elementCount);
                if (frame.elementCount > 0) {
                    frame.renderBlock->tag = 0x9000000;
                    frame.renderBlock->colorCommand = g_BattleSpecBigWaveWavePhase * 0x10101 + (g_BattleSpecBigWaveWavePhase - 0x80 != 0 ? 0x2E000000 : 0x2C000000);
                    frame.renderBlock->u0 = frame.colIndex << 5;
                    frame.renderBlock->v0 = frame.rowIndex << 5;
                    frame.renderBlock->u1 = frame.colIndex + 1 << 5;
                    frame.renderBlock->v1 = frame.rowIndex << 5;
                    frame.renderBlock->u2 = frame.colIndex << 5;
                    frame.renderBlock->v2 = frame.rowIndex + 1 << 5;
                    frame.renderBlock->u3 = frame.colIndex + 1 << 5;
                    frame.renderBlock->v3 = frame.rowIndex + 1 << 5;
                    if (frame.renderBlock->u1 == 0) {
                        frame.renderBlock->u3 = 0xFF;
                        frame.renderBlock->u1 = 0xFF;
                    }
                    if (frame.renderBlock->v2 == 0) {
                        frame.renderBlock->v3 = 0xFF;
                        frame.renderBlock->v2 = 0xFF;
                    }
                    if ((frame.colIndex & 7) >= 4) {
                        BattleFx_SpecialBigWaveInvertUvBytePair(&frame.renderBlock->u0, &frame.renderBlock->u1);
                        BattleFx_SpecialBigWaveInvertUvBytePair(&frame.renderBlock->u2, &frame.renderBlock->u3);
                    }
                    if ((frame.rowIndex & 7) >= 4) {
                        BattleFx_SpecialBigWaveInvertUvBytePair(&frame.renderBlock->v0, &frame.renderBlock->v2);
                        BattleFx_SpecialBigWaveInvertUvBytePair(&frame.renderBlock->v1, &frame.renderBlock->v3);
                    }
                    frame.renderBlock->waveBias = frame.waveBias;
                    frame.renderBlock->waveAttr = frame.waveAttr;
                    Gfx_DrawPsxPrimitive(frame.renderBlock);
                    frame.renderBlock++;
                }
            }
        }
    }
    g_BattleSpecBigWaveDrawListHead = (int)frame.renderBlock;
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleSpecBigWaveWaveHeightBase -= 3;
    if (frame.waveSlot->frameCounter < 4) {
        g_BattleSpecBigWaveWavePhase += 0x20;
    }
    if (frame.waveSlot->frameCounter > 0x2A) {
        g_BattleSpecBigWaveWavePhase -= 0x10;
    }
    (frame.waveSlot->frameCounter)++;
    if (frame.waveSlot->frameCounter == 0x32) {
        frame.waveSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Negates the byte at each of the two addresses in place: the first becomes 0xFF if it was 0, the
 * second becomes 0 if it was 0xFF, otherwise both become 0x100 minus their value.
 */
// FUNCTION: FF7 0x00542629
void __cdecl BattleFx_SpecialBigWaveInvertUvBytePair(unsigned char *firstUvByte, unsigned char *secondUvByte)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if (*firstUvByte == 0) {
        *firstUvByte = 0xFF;
    } else {
        *firstUvByte = 0x100 - *firstUvByte;
    }
    if (*secondUvByte == 0xFF) {
        *secondUvByte = 0;
    } else {
        *secondUvByte = 0x100 - *secondUvByte;
    }
}

/*
 * Returns the wave height at a vertex: the sine table entry for waveIndex (0 outside 0..0x800) scaled
 * by 0x9C4, then by the row's entry of the second table, with twelve-bit shifts.
 */
// FUNCTION: FF7 0x00542681
short __cdecl BattleFx_SpecialBigWaveGetWaveVertexHeight(short waveIndex, short rowIndex)
{
    short height;

    if (waveIndex < 0 || waveIndex > 0x800) {
        waveIndex = 0;
    }
    height = -((short *)&g_BattleSpecBigWaveWaveTable1)[waveIndex] * 0x9C4 >> 0xC;
    height = ((short *)&g_BattleSpecBigWaveWaveTable2)[rowIndex * 0x60] * height >> 0xC;
    return (short)height;
}
} /* extern "C" */
