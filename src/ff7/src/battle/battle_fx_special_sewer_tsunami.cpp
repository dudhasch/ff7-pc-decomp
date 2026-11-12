// src/ff7/src/battle/battle_fx_special_sewer_tsunami.cpp
/* Enemy Special Effect [idx 72, 73]: SewerTsunami (JP: gesui1 / 下水1 (Sewer Tsunami 1), asset: "SPECIAL/GESUI1") — Sewer Tsunami (0x144 [idx 72], 0x145 [idx 73], Aps) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Gesui spell. It has two entry points that start the
//   timeline, runs the wave surface and animated sprite tasks, and includes the helpers that mirror
//   texture coordinates and sample wave height.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
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
// GLOBAL: FF7 0x0089a048
int g_BattleSpecSewerTsunamiResourceDef = 0x00192A39;
char *g_BattleSpecialSewerTsunamiResourceNames[1] = {
    // STRING: FF7 0x0089a0b8
    "BAKU1.TIM"
};
typedef struct {
    char *filename;
    unsigned int descriptor;
    char *s2;
} BattleSpecialSewerTsunamiResourceTailEntry;
BattleSpecialSewerTsunamiResourceTailEntry g_BattleSpecialSewerTsunamiResourceNames2[6] = {
    {
        // STRING: FF7 0x0089a0c4
        "BAKU1.SPT",
        0x00000000,
        0
    },
    {
        (char *)0x00192a39,
        0x00000000,
        // STRING: FF7 0x0089a0d0
        "JO_C03.SPT"
    },
    {
        0,
        0x00000000,
        (char *)0x00197a39
    },
    {
        // STRING: FF7 0x0089a0dc
        "1.TIM",
        0x00000000,
        0
    },
    {
        0,
        0x00197a3c,
        // STRING: FF7 0x0089a0e4
        "1.TIM"
    },
    {
        0,
        0x00000000,
        0
    }
};
// GLOBAL: FF7 0x0089a098
BattleSpecSewerTsunamiSpriteRequest g_BattleSpecSewerTsunamiSpriteRequest = {1, {0}, {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2c, 0, 0}, 0};
unsigned char g_rawData_0089a0b4[4] = {0x01, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b4fcc8
int g_BattleSpecSewerTsunamiState = 0;
// GLOBAL: FF7 0x00b4fccc
short g_BattleSpecSewerTsunamiActiveCount = 0;
// GLOBAL: FF7 0x00b4fcd0
short g_BattleSpecSewerTsunamiTimer = 0;
unsigned char g_bssPad_00b4fcd4[4] = {0};
// g_BattleSpecSewerTsunamiCoordBuffer is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0xb4fcd8..0xb4fed8 are aliases.
// GLOBAL: FF7 0x00b4fcd8
unsigned char g_BattleSpecSewerTsunamiCoordBuffer[0x200] = {0};
// g_BattleSpecSewerTsunamiStateBuffer is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0xb4fed8..0xb51cd8 are aliases.
// GLOBAL: FF7 0x00b4fed8
unsigned char g_BattleSpecSewerTsunamiStateBuffer[0x1e00] = {0};
// GLOBAL: FF7 0x00b51cd8
short g_BattleSpecSewerTsunamiStep = 0;
// GLOBAL: FF7 0x00b51cdc
struct GfxTableCursor *g_BattleSpecSewerTsunamiLoadedResTable = 0;
// clang-format on

/*
 * Loads the SPECIAL/GESUI1 resource table (type 4, g_BattleSpecSewerTsunamiResourceDef) when the battle scene
 * exists and no table is loaded yet.
 */
// FUNCTION: FF7 0x005125e0
void __cdecl BattleFx_SpecialLoadResSewerTsunami(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resultDefArr;

    if (g_pBattle3dScene != 0 && g_BattleSpecSewerTsunamiLoadedResTable == 0) {
        kind = 4;
        resultDefArr = (int)&g_BattleSpecSewerTsunamiResourceDef;
        stage = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089a0ec
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/GESUI1", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resultDefArr, pRequest, &g_BattleSpecSewerTsunamiLoadedResTable, stage);
    }
}

/*
 * Entry point of the Gesui1 special: loads the resources, sets the wave direction to 1, the step to 0 and
 * the wave timer to 0x190, then spawns the timeline.
 */
// FUNCTION: FF7 0x00512670
void __cdecl BattleFx_SpecialEnterSewerTsunami(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSewerTsunami();
    g_BattleSpecSewerTsunamiSpriteRequest.direction = 1;
    g_BattleSpecSewerTsunamiStep = 0;
    g_BattleSpecSewerTsunamiTimer = 0x190;
    BattleFx_SpecialSewerTsunamiSpawnTimeline(targetMask, actorIdx);
}

/*
 * Alternate entry point of the Gesui1 special: the same setup as BattleFx_SpecialEnterSewerTsunami, but it spawns
 * the timeline through BattleFx_SpecialSewerTsunamiSpawnTimelineAlt.
 */
// FUNCTION: FF7 0x005126a5
void __cdecl BattleFx_SpecialEnterSewerTsunamiAlt(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSewerTsunami();
    g_BattleSpecSewerTsunamiSpriteRequest.direction = 1;
    g_BattleSpecSewerTsunamiStep = 0;
    g_BattleSpecSewerTsunamiTimer = 0x190;
    BattleFx_SpecialSewerTsunamiSpawnTimelineAlt(targetMask, actorIdx);
}

/*
 * Stores the target mask, fills the sine table with SinFixed(i), spawns the timeline task at the caster's
 * bone position with the targets' centroid as its end, flips the wave direction when the start's Z exceeds
 * the end's, and plays the sound.
 */
// FUNCTION: FF7 0x005126da
void __cdecl BattleFx_SpecialSewerTsunamiSpawnTimeline(int targetIdx, int actorIdx)
{
    BattleFxBigWaveTimeline *recordAddress;
    short i;

    g_BattleSpecSewerTsunamiActiveCount = targetIdx;
    for (i = 0; i < 0x1000; i++) {
        ((short *)&g_BattleSpecSewerTsunamiCoordBuffer)[i] = SinFixed(i);
    }
    recordAddress = (BattleFxBigWaveTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSewerTsunamiTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone34, &recordAddress->startX);
    Battle_GetActorMaskCentroid(targetIdx, &recordAddress->targetX);
    if (recordAddress->timer > recordAddress->limit) {
        g_BattleSpecSewerTsunamiSpriteRequest.direction *= -1;
    }
    Battle_PlaySfxPannedToActor(0x132, actorIdx, 0x40);
}

/*
 * Timeline of the Gesui1 special: ramps the screen flash, starts the wave surface, spawns the splash sprites
 * around each target between frames 0x25 and 0x2B and the hit records at frame 0x28, and ends at frame 0x3C.
 */
// FUNCTION: FF7 0x005127b4
void __cdecl BattleFx_SpecialSewerTsunamiTimelineTask(void)
{
    BattleTaskSlot *pCurrent;
    short actorIdx;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSewerTsunamiTask *child;

    pCurrent = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pCurrent->frameCounter == 0) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        Battle_StartFixedPointRamp(0x1000, 4);
    }
    if (pCurrent->frameCounter == 0x14) {
        Battle_StartFixedPointRamp(0, 8);
    }
    if (pCurrent->frameCounter == 0) {
        child = (BattleFxSewerTsunamiTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSewerTsunamiWaveSurfaceTask);
    }
    if (pCurrent->frameCounter > 0x24 && pCurrent->frameCounter < 0x2C) {
        for (actorIdx = 0; actorIdx < 0xA; actorIdx++) {
            if ((g_BattleSpecSewerTsunamiActiveCount & 1 << actorIdx) != 0) {
                child = (BattleFxSewerTsunamiTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSewerTsunamiAnimatedSpriteTask);
                Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, &child->posX);
                child->posX += (short)(rand() % 0x258 - 0x12C);
                child->posY += (short)(rand() % 0x258 - 0x12C);
                child->posZ += (short)(rand() % 0x258 - 0x12C);
                child->isActive = 1;
                child = (BattleFxSewerTsunamiTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSewerTsunamiAnimatedSpriteTask);
                Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, &child->posX);
                child->posX += (short)(rand() % 0x578 - 0x2BC);
                child->posY += (short)(rand() % 0x578 - 0x2BC);
                child->posZ += (short)(rand() % 0x578 - 0x2BC);
                child->isActive = 0;
            }
        }
    }
    if (pCurrent->frameCounter == 0x28) {
        for (actorIdx = 0; actorIdx < 0xA; actorIdx++) {
            if ((g_BattleSpecSewerTsunamiActiveCount & 1 << actorIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(actorIdx);
            }
        }
    }
    (pCurrent->frameCounter)++;
    if (pCurrent->frameCounter == 0x3C) {
        pCurrent->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one splash sprite as a billboard at the task's position (scale 0x1800, or 0x2800 for variant 1), with the
 * sprite group picked by its variant, and advances its frame; the task ends at the animation's frame count.
 */
// FUNCTION: FF7 0x00512a9f
void __cdecl BattleFx_SpecialSewerTsunamiAnimatedSpriteTask(void)
{
    BattleFxSewerTsunamiAnimSprite *task;

    task = (BattleFxSewerTsunamiAnimSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, (task->variant != 0 ? 0x1000 : 0) + 0x1800, 0);
    g_BattleSpecSewerTsunamiSpriteRequest.sprite.frame = task->frame;
    g_BattleSpecSewerTsunamiSpriteRequest.sprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord((&g_BattleSpecSewerTsunamiSpriteRequest.variantTable)[task->variant], g_BattleSpecSewerTsunamiLoadedResTable);
    if (task->variant != 1) {
        g_BattleSpecSewerTsunamiSpriteRequest.sprite.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecSewerTsunamiLoadedResTable);
    } else {
        g_BattleSpecSewerTsunamiSpriteRequest.sprite.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    g_BattleSpecSewerTsunamiState = BattleFx_SubmitSpriteDraw(&g_BattleSpecSewerTsunamiSpriteRequest.sprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSewerTsunamiState);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecSewerTsunamiSpriteRequest.sprite.spriteGroup != 0) {
        (task->frame)++;
        if (task->frame == g_BattleSpecSewerTsunamiSpriteRequest.sprite.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws the wave surface as a grid of textured quads whose heights come from
 * BattleFx_SpecialSewerTsunamiSampleWaveHeight; fades it in and out through g_BattleSpecSewerTsunamiStep and ends at frame 0x32.
 */
// FUNCTION: FF7 0x00512bb4
void __cdecl BattleFx_SpecialSewerTsunamiWaveSurfaceTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        int spriteState;
        struct { unsigned char pad_to_4[0x4]; short wavePhase; };
        struct { unsigned char pad_to_C[0xC]; int elementCount; };
        struct { unsigned char pad_to_10[0x10]; short clut; };
        struct { unsigned char pad_to_18[0x18]; BattleFxSewerTsunamiSurfaceRenderStateBlock *renderState; };
        struct { unsigned char pad_to_1C[0x1C]; SVECTOR quadX3; };
        struct { unsigned char pad_to_24[0x24]; short colIndex; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR quadX2; };
        struct { unsigned char pad_to_30[0x30]; short rowIndex; };
        struct { unsigned char pad_to_34[0x34]; SVECTOR quadX1; };
        struct { unsigned char pad_to_3C[0x3C]; SVECTOR quadX0; };
        struct { unsigned char pad_to_44[0x44]; BattleTaskSlot *waveSlot; };
        struct { unsigned char pad_to_48[0x48]; short waveRow; };
        struct { unsigned char pad_to_4C[0x4C]; short tpage; };
    } frame;
#pragma pack(pop)

    frame.waveSlot = BATTLE_CURRENT_TASK_SLOT;
    frame.wavePhase = frame.waveSlot->frameCounter * g_BattleSpecSewerTsunamiTimer - 0x3E8;
    frame.tpage = 0;
    frame.clut = 0x7808;
    frame.renderState = (BattleFxSewerTsunamiSurfaceRenderStateBlock *)Gfx_GetPsxPacketScratchPtr();
    frame.renderState->primCode |= 2;
    frame.renderState->packetTag = frame.renderState->packetTag & 0xFFFFFF | 0x9000000;
    frame.renderState->primCode = 0x2C;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    for (frame.rowIndex = 0xFFFC; frame.rowIndex < 0x10; frame.rowIndex++) {
        for (frame.colIndex = 0; frame.colIndex < 0x10; frame.colIndex++) {
            frame.waveRow = frame.rowIndex - frame.wavePhase / 0x3E8;
            frame.quadX0.vx = (frame.colIndex - 8) * 0x3E8;
            frame.quadX0.vz = (frame.wavePhase + (frame.waveRow - 8) * 0x3E8) * g_BattleSpecSewerTsunamiSpriteRequest.direction;
            frame.quadX1.vx = (frame.colIndex - 8) * 0x3E8 + 0x3E8;
            frame.quadX1.vz = (frame.wavePhase + (frame.waveRow - 8) * 0x3E8) * g_BattleSpecSewerTsunamiSpriteRequest.direction;
            frame.quadX2.vx = (frame.colIndex - 8) * 0x3E8;
            frame.quadX2.vz = (frame.wavePhase + (frame.waveRow - 8) * 0x3E8 + 0x3E8) * g_BattleSpecSewerTsunamiSpriteRequest.direction;
            frame.quadX3.vx = (frame.colIndex - 8) * 0x3E8 + 0x3E8;
            frame.quadX3.vz = (frame.wavePhase + (frame.waveRow - 8) * 0x3E8 + 0x3E8) * g_BattleSpecSewerTsunamiSpriteRequest.direction;
            frame.quadX0.vy = (short)BattleFx_SpecialSewerTsunamiSampleWaveHeight(frame.waveSlot->frameCounter * 0xA0 - frame.waveRow * 0x190, (short)frame.colIndex) - 0xC8;
            frame.quadX2.vy = (short)BattleFx_SpecialSewerTsunamiSampleWaveHeight(frame.waveSlot->frameCounter * 0xA0 - (frame.waveRow + 1) * 0x190, (short)frame.colIndex) - 0xC8;
            frame.quadX1.vy = (short)BattleFx_SpecialSewerTsunamiSampleWaveHeight(frame.waveSlot->frameCounter * 0xA0 - frame.waveRow * 0x190, frame.colIndex + 1) - 0xC8;
            frame.quadX3.vy = (short)BattleFx_SpecialSewerTsunamiSampleWaveHeight(frame.waveSlot->frameCounter * 0xA0 - (frame.waveRow + 1) * 0x190, frame.colIndex + 1) - 0xC8;
            StoreDeferredDrawArgs(&frame.quadX0, &frame.quadX2, &frame.quadX1);
            ApplyDeferredDrawArgs();
            SetPendingResourceHandle((GfxPsxShortVertexSlot *)&frame.quadX3);
            ApplyPendingResourceHandle();
            Gfx_ReadTransformOrderingDepth2(&frame.elementCount);
            if (frame.elementCount > 0) {
                frame.renderState->packetTag = 0x9000000;
                frame.renderState->rgbCmd = g_BattleSpecSewerTsunamiStep * 0x10101 + (g_BattleSpecSewerTsunamiStep - 0x80 != 0 ? 0x2E000000 : 0x2C000000);
                frame.renderState->vertex0U = frame.colIndex << 6;
                frame.renderState->vertex0V = frame.waveRow << 6;
                frame.renderState->vertex1U = frame.colIndex + 1 << 6;
                frame.renderState->vertex1V = frame.waveRow << 6;
                frame.renderState->vertex2U = frame.colIndex << 6;
                frame.renderState->vertex2V = frame.waveRow + 1 << 6;
                frame.renderState->vertex3U = frame.colIndex + 1 << 6;
                frame.renderState->vertex3V = frame.waveRow + 1 << 6;
                if (*(unsigned char *)&frame.renderState->vertex1U == 0) {
                    frame.renderState->vertex3U = 0xFF;
                    frame.renderState->vertex1U = 0xFF;
                }
                if (*(unsigned char *)&frame.renderState->vertex2V == 0) {
                    frame.renderState->vertex3V = 0xFF;
                    frame.renderState->vertex2V = 0xFF;
                }
                if ((frame.colIndex & 7) >= 4) {
                    BattleFx_SpecialSewerTsunamiMirrorTexCoordPair((char *)frame.renderState + 0xC, &frame.renderState->vertex1U);
                    BattleFx_SpecialSewerTsunamiMirrorTexCoordPair((char *)frame.renderState + 0x1C, &frame.renderState->vertex3U);
                }
                if ((frame.waveRow & 7) >= 4) {
                    BattleFx_SpecialSewerTsunamiMirrorTexCoordPair((char *)frame.renderState + 0xD, &frame.renderState->vertex2V);
                    BattleFx_SpecialSewerTsunamiMirrorTexCoordPair((char *)frame.renderState + 0x15, &frame.renderState->vertex3V);
                }
                frame.renderState->tpage = frame.tpage;
                frame.renderState->clut = frame.clut;
                if (g_BattleSpecSewerTsunamiStep != 0x80) {
                    frame.spriteState = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecSewerTsunamiLoadedResTable);
                    if (frame.spriteState != 0) {
                        Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)frame.spriteState);
                    }
                } else {
                    frame.spriteState = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecSewerTsunamiLoadedResTable);
                    if (frame.spriteState != 0) {
                        Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)frame.spriteState);
                    }
                }
                Gfx_DrawPsxPrimitive(frame.renderState);
            }
        }
    }
    frame.wavePhase = frame.waveSlot->frameCounter * 0x19;
    if (DEAD_IN_RETAIL != 0) {
        for (frame.rowIndex = 0xFFFE; frame.rowIndex < 0x10; frame.rowIndex++) {
            for (frame.colIndex = 0; frame.colIndex < 0x10; frame.colIndex++) {
                frame.quadX0.vx = (frame.colIndex - 8) * 0x3E8;
                frame.quadX0.vz = (frame.wavePhase + (frame.rowIndex - 8) * 0x3E8) * g_BattleSpecSewerTsunamiSpriteRequest.direction;
                frame.quadX1.vx = (frame.colIndex - 8) * 0x3E8 + 0x3E8;
                frame.quadX1.vz = (frame.wavePhase + (frame.rowIndex - 8) * 0x3E8) * g_BattleSpecSewerTsunamiSpriteRequest.direction;
                frame.quadX2.vx = (frame.colIndex - 8) * 0x3E8;
                frame.quadX2.vz = (frame.wavePhase + (frame.rowIndex - 8) * 0x3E8 + 0x3E8) * g_BattleSpecSewerTsunamiSpriteRequest.direction;
                frame.quadX3.vx = (frame.colIndex - 8) * 0x3E8 + 0x3E8;
                frame.quadX3.vz = (frame.wavePhase + (frame.rowIndex - 8) * 0x3E8 + 0x3E8) * g_BattleSpecSewerTsunamiSpriteRequest.direction;
                frame.quadX3.vy = 0;
                frame.quadX2.vy = frame.quadX3.vy;
                frame.quadX1.vy = frame.quadX2.vy;
                frame.quadX0.vy = frame.quadX1.vy;
                StoreDeferredDrawArgs(&frame.quadX0, &frame.quadX1, &frame.quadX2);
                ApplyDeferredDrawArgs();
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)&frame.quadX3);
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&frame.elementCount);
                if (frame.elementCount > 0) {
                    frame.renderState->packetTag = 0x9000000;
                    frame.renderState->rgbCmd = g_BattleSpecSewerTsunamiStep * 0x10101 + (g_BattleSpecSewerTsunamiStep - 0x80 != 0 ? 0x2E000000 : 0x2C000000);
                    frame.renderState->vertex0U = frame.colIndex << 5;
                    frame.renderState->vertex0V = frame.rowIndex << 5;
                    frame.renderState->vertex1U = frame.colIndex + 1 << 5;
                    frame.renderState->vertex1V = frame.rowIndex << 5;
                    frame.renderState->vertex2U = frame.colIndex << 5;
                    frame.renderState->vertex2V = frame.rowIndex + 1 << 5;
                    frame.renderState->vertex3U = frame.colIndex + 1 << 5;
                    frame.renderState->vertex3V = frame.rowIndex + 1 << 5;
                    if (*(unsigned char *)&frame.renderState->vertex1U == 0) {
                        frame.renderState->vertex3U = 0xFF;
                        frame.renderState->vertex1U = 0xFF;
                    }
                    if (*(unsigned char *)&frame.renderState->vertex2V == 0) {
                        frame.renderState->vertex3V = 0xFF;
                        frame.renderState->vertex2V = 0xFF;
                    }
                    if ((frame.colIndex & 7) >= 4) {
                        BattleFx_SpecialSewerTsunamiMirrorTexCoordPair((char *)frame.renderState + 0xC, &frame.renderState->vertex1U);
                        BattleFx_SpecialSewerTsunamiMirrorTexCoordPair((char *)frame.renderState + 0x1C, &frame.renderState->vertex3U);
                    }
                    if ((frame.rowIndex & 7) >= 4) {
                        BattleFx_SpecialSewerTsunamiMirrorTexCoordPair((char *)frame.renderState + 0xD, &frame.renderState->vertex2V);
                        BattleFx_SpecialSewerTsunamiMirrorTexCoordPair((char *)frame.renderState + 0x15, &frame.renderState->vertex3V);
                    }
                    frame.renderState->tpage = frame.tpage;
                    frame.renderState->clut = frame.clut;
                    Gfx_DrawPsxPrimitive(frame.renderState);
                }
            }
        }
    }
    g_BattleSpecSewerTsunamiState = (int)frame.renderState;
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleSpecSewerTsunamiTimer -= 3;
    if (frame.waveSlot->frameCounter < 4) {
        g_BattleSpecSewerTsunamiStep += 0x20;
    }
    if (frame.waveSlot->frameCounter > 0x2A) {
        g_BattleSpecSewerTsunamiStep -= 0x10;
    }
    (frame.waveSlot->frameCounter)++;
    if (frame.waveSlot->frameCounter == 0x32) {
        frame.waveSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Mirrors two texture coordinate bytes in place: the first becomes 0xFF if it was 0, else 0x100 minus itself;
 * the second becomes 0 if it was 0xFF, else 0x100 minus itself.
 */
// FUNCTION: FF7 0x00513416
void __cdecl BattleFx_SpecialSewerTsunamiMirrorTexCoordPair(void *coordPtrA, void *coordPtrB)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if (*(unsigned char *)coordPtrA == 0) {
        *(char *)coordPtrA = 0xFF;
    } else {
        *(char *)coordPtrA = 0x100 - *(unsigned char *)coordPtrA;
    }
    if (*(unsigned char *)coordPtrB == 0xFF) {
        *(char *)coordPtrB = 0;
    } else {
        *(char *)coordPtrB = 0x100 - *(unsigned char *)coordPtrB;
    }
}

/*
 * Returns the wave height at a phase angle (0 outside 0..0x800) and a row: the negated sine table value scaled
 * by 0x9C4, multiplied by the row's amplitude from g_BattleSpecSewerTsunamiStateBuffer.
 */
// FUNCTION: FF7 0x0051346e
short __cdecl BattleFx_SpecialSewerTsunamiSampleWaveHeight(short angle, short row)
{
    short height;

    if (angle < 0 || angle > 0x800) {
        angle = 0;
    }
    height = -((short *)&g_BattleSpecSewerTsunamiCoordBuffer)[angle] * 0x9C4 >> 0xC;
    height = ((short *)&g_BattleSpecSewerTsunamiStateBuffer)[row * 0x60] * height >> 0xC;
    return (short)height;
}

/*
 * Alternate setup of the Gesui1 timeline: as BattleFx_SpecialSewerTsunamiSpawnTimeline, except that the wave
 * direction flips when the start's Z is below the end's.
 */
// FUNCTION: FF7 0x005134cc
void __cdecl BattleFx_SpecialSewerTsunamiSpawnTimelineAlt(int targetIdx, int actorIdx)
{
    BattleFxBigWaveTimeline *taskRec;
    short idx;

    g_BattleSpecSewerTsunamiActiveCount = (short)targetIdx;
    for (idx = 0; idx < 0x1000; idx++) {
        ((short *)&g_BattleSpecSewerTsunamiCoordBuffer)[idx] = SinFixed(idx);
    }
    taskRec = (BattleFxBigWaveTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSewerTsunamiTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone34, &taskRec->startX);
    Battle_GetActorMaskCentroid(targetIdx, &taskRec->targetX);
    if (taskRec->timer < taskRec->limit) {
        g_BattleSpecSewerTsunamiSpriteRequest.direction *= -1;
    }
    Battle_PlaySfxPannedToActor(0x132, (short)actorIdx, 0x40);
}
} /* extern "C" */
