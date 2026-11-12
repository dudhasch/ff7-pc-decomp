// src/ff7/src/battle/battle_fx_special_cold_air_effect.cpp
/* Enemy Special Attack [98]: Cold Air (orig. SPECIAL/REIKI, 冷気) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Reiki': a white flash pulse, a timeline with expanding and
//   spinning emitting models, an animated sprite, and rising spinning models and fading sprites.

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

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a25d8
unsigned int g_BattleSpecColdAirResourceDefs[100] = {0x001900b1, (unsigned int)&g_BattleSpecColdAirResNameWa, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecColdAirResNameNetu1Uchi1, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecColdAirResNameNetu1Uchi1[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecColdAirResNameUchi2Uchi3, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecColdAirResNameUchi2Uchi3[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecColdAirResNameUchi4Uchi5, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecColdAirResNameUchi4Uchi5[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecColdAirResNameUchi6Uchi7, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecColdAirResNameUchi6Uchi7[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecColdAirResNameUchi8Uchi9, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecColdAirResNameUchi8Uchi9[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecColdAirResNameUchi10Isi1, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecColdAirResNameUchi10Isi1[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecColdAirResNameIsi2Isi3, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecColdAirResNameIsi2Isi3[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecColdAirResNameIsi4Netu03, 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecColdAirResNameIsi4Netu03[12], (unsigned int)&g_BattleSpecColdAirResNameHahen1Kemu1, 0x00000000, 0x00000000, 0x00192a39, 0x00000000, (unsigned int)&g_BattleSpecColdAirResNameHahen1Kemu1[12], 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecColdAirResNameNetu03Hikari, (unsigned int)&g_BattleSpecColdAirResNameNetu03Hikari[12], 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecColdAirResNameNetu03Kage, (unsigned int)&g_BattleSpecColdAirResNameNetu03Kage[12], 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008a2768
struct BattleSpecColdAirSpriteReq1 g_BattleSpecColdAirSpriteReq1 = {0, 0, {0}, 0, {0x20, 0, 0, 0}};
// Model ids of the expanding models' inner model; the code indexes it with frameCounter % 10, so
// indices 2..9 read the ids 4..11 that open g_BattleSpecColdAirSpriteReq2.
// GLOBAL: FF7 0x008a2778
int g_BattleSpecColdAirModelIndices1[2] = {2, 3};
// GLOBAL: FF7 0x008a2780
struct BattleSpecColdAirSpriteReq2 g_BattleSpecColdAirSpriteReq2 = {{4, 0, 0, 0, 5, 0, 0, 0, 6, 0, 0, 0, 7, 0, 0, 0, 8, 0, 0, 0, 9, 0, 0, 0, 0xa, 0, 0, 0, 0xb, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x008a27a8
BattleSpecColdAirSpriteReq3 g_BattleSpecColdAirSpriteReq3 = {-128, -128, -128, 0x2e, {0}, 0, 0, -128, -128, -128, 0x2e};
unsigned char g_rawData_008a27cc[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a27d0
BattleSpecColdAirModelIndices2 g_BattleSpecColdAirModelIndices2 = {{0xc, 0xd, 0xe, 0xf}};
// GLOBAL: FF7 0x008a27e8
BattleSpecColdAirSpriteReq4 g_BattleSpecColdAirSpriteReq4 = {-128, -128, -128, 0x2c, 0, {0}, {0, 0x28, 0, 0x80, 0x20}};
// GLOBAL: FF7 0x008a2808
struct BattleSpecHeatBlastColor g_BattleSpecColdAirColorData = {-128, -128, -128, {0x2c}};
// GLOBAL: FF7 0x008a2810
unsigned char g_BattleSpecColdAirResNameWa[7] = "wa.rsd";
// GLOBAL: FF7 0x008a2818
unsigned char g_BattleSpecColdAirResNameNetu1Uchi1[22] =
    "netu1.rsd\0\0\0"
    "uchi1.rsd";
// GLOBAL: FF7 0x008a2830
unsigned char g_BattleSpecColdAirResNameUchi2Uchi3[22] =
    "uchi2.rsd\0\0\0"
    "uchi3.rsd";
// GLOBAL: FF7 0x008a2848
unsigned char g_BattleSpecColdAirResNameUchi4Uchi5[22] =
    "uchi4.rsd\0\0\0"
    "uchi5.rsd";
// GLOBAL: FF7 0x008a2860
unsigned char g_BattleSpecColdAirResNameUchi6Uchi7[22] =
    "uchi6.rsd\0\0\0"
    "uchi7.rsd";
// GLOBAL: FF7 0x008a2878
unsigned char g_BattleSpecColdAirResNameUchi8Uchi9[22] =
    "uchi8.rsd\0\0\0"
    "uchi9.rsd";
// GLOBAL: FF7 0x008a2890
unsigned char g_BattleSpecColdAirResNameUchi10Isi1[21] =
    "uchi10.rsd\0\0"
    "isi1.rsd";
// GLOBAL: FF7 0x008a28a8
unsigned char g_BattleSpecColdAirResNameIsi2Isi3[21] =
    "isi2.rsd\0\0\0\0"
    "isi3.rsd";
// GLOBAL: FF7 0x008a28c0
unsigned char g_BattleSpecColdAirResNameIsi4Netu03[24] =
    "isi4.rsd\0\0\0\0"
    "netsu03.tim";
// GLOBAL: FF7 0x008a28d8
unsigned char g_BattleSpecColdAirResNameHahen1Kemu1[22] =
    "hahen1.spt\0\0"
    "kemu1.spt";
// GLOBAL: FF7 0x008a28f0
unsigned char g_BattleSpecColdAirResNameNetu03Hikari[23] =
    "netsu03.tim\0"
    "hikari.spt";
// GLOBAL: FF7 0x008a2908
unsigned char g_BattleSpecColdAirResNameNetu03Kage[21] =
    "netsu03.tim\0"
    "kage.spt";
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b64f80
int g_BattleSpecColdAirPacketCursor = 0;
// GLOBAL: FF7 0x00b64f84
short g_BattleSpecColdAirTargetMask = 0;
// GLOBAL: FF7 0x00b64f88
int g_BattleSpecColdAirScaleVector1_X = 0;
// GLOBAL: FF7 0x00b64f8c
int g_BattleSpecColdAirScaleVector1_Y = 0;
// GLOBAL: FF7 0x00b64f90
int g_BattleSpecColdAirScaleVector1_Z = 0;
unsigned char g_bssPad_00b64f94[4] = {0};
// GLOBAL: FF7 0x00b64f98
int g_BattleSpecColdAirScaleVector2_X = 0;
// GLOBAL: FF7 0x00b64f9c
int g_BattleSpecColdAirScaleVector2_Y = 0;
// GLOBAL: FF7 0x00b64fa0
int g_BattleSpecColdAirScaleVector2_Z = 0;
unsigned char g_bssPad_00b64fa4[4] = {0};
// GLOBAL: FF7 0x00b64fa8
int g_BattleSpecColdAirScaleVector3_X = 0;
// GLOBAL: FF7 0x00b64fac
int g_BattleSpecColdAirScaleVector3_Y = 0;
// GLOBAL: FF7 0x00b64fb0
int g_BattleSpecColdAirScaleVector3_Z = 0;
unsigned char g_bssPad_00b64fb4[4] = {0};
// GLOBAL: FF7 0x00b64fb8
short g_BattleSpecColdAirScaleFactor = 0;
// GLOBAL: FF7 0x00b64fbc
short g_BattleSpecColdAirFlashIntensity = 0;
// GLOBAL: FF7 0x00b64fc0
struct GfxTableCursor *g_BattleSpecColdAirLoadedResTable = 0;
// clang-format on

/*
 * Clears shared flash intensity and sets its duration to sixteen. With a nonnull stage object and
 * absent shared resource initializes a default-duration record, sets its first int zero and
 * resource path SPECIAL/REIKI, then calls Gfx_LoadModelTableIfAbsent with selector twenty, descriptor
 * g_BattleSpecColdAirResourceDefs and the scene's model-table bank; no result or pause guard appears.
 */
// FUNCTION: FF7 0x00534e80
void __cdecl BattleFx_SpecialLoadResColdAir(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    g_BattleSpecColdAirFlashIntensity = 0;
    g_BattleSpecColdAirScaleFactor = 0x10;
    if (g_pBattle3dScene != 0 && g_BattleSpecColdAirLoadedResTable == 0) {
        kind = 0x14;
        resourceDefsWord = (int)&g_BattleSpecColdAirResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a2920
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/REIKI", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecColdAirLoadedResTable, curOwnerCtx);
    }
}

/*
 * Mechanical forwarder: calls BattleFx_SpecialBeginColdAir with both arguments unchanged and does nothing else.
 */
// FUNCTION: FF7 0x00534f22
void __cdecl BattleFx_SpecialEnterColdAir(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginColdAir(targetMask, actorIdx);
}

/*
 * Calls BattleFx_SpecialLoadResColdAir, stores targetMask, allocates
 * BattleFx_SpecialColdAirTimelineTask with the actorIndex actor bone selected at
 * g_BattleActorRecords[0].bone34 and targetMask actor-mask centroid, and dispatches (0x164,
 * narrowed actorIndex, 0).
 */
// FUNCTION: FF7 0x00534f37
void __cdecl BattleFx_SpecialBeginColdAir(int targetMask, int actorIdx)
{
    int taskPtr;

    BattleFx_SpecialLoadResColdAir();
    g_BattleSpecColdAirTargetMask = targetMask;
    taskPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialColdAirTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone34, (short *)(taskPtr + 4));
    Battle_GetActorMaskCentroid(targetMask, (short *)(taskPtr + 0xC));
    Battle_PlaySfxPannedToActor(0x164, (short)actorIdx, 0);
}

/*
 * Reiki timeline: on its first frame it spawns the expanding model and spin model emitters, clears
 * the timeline colour and starts a 16-frame ramp to 0x1000; at frame 12 a white flash, at frame 28
 * a ramp back to 0, at frame 35 a typed record per target; the slot is freed at frame 50.
 */
// FUNCTION: FF7 0x00534fb5
void __cdecl BattleFx_SpecialColdAirTimelineTask(void)
{
    BattleTaskSlot *mainSelf;
    short bitIdx;
    BattleFxColdAirTimeline2 *pParticle;
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

    mainSelf = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (mainSelf->frameCounter == 0) {
        pParticle = (BattleFxColdAirTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialColdAirDrawExpandingModelsTask);
        pParticle->posX = 0;
        pParticle->posY = 0xFB50;
        pParticle->posZ = 0;
        pParticle = (BattleFxColdAirTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialColdAirDrawSpinModelsEmitTask);
        pParticle->posX = 0;
        pParticle->posY = 0xFB50;
        pParticle->posZ = 0;
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        Battle_StartFixedPointRamp(0x1000, 0x10);
    }
    if (mainSelf->frameCounter == 0xC) {
        Battle_AllocateResourceSlot(BattleFx_SpecialColdAirWhiteFlashPulseTask);
    }
    if (mainSelf->frameCounter == 0x1C) {
        Battle_StartFixedPointRamp(0, 0x10);
    }
    if (mainSelf->frameCounter == 0x23) {
        for (bitIdx = 0; bitIdx < 0xA; bitIdx++) {
            if ((g_BattleSpecColdAirTargetMask & 1 << bitIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(bitIdx);
            }
        }
    }
    (mainSelf->frameCounter)++;
    if (mainSelf->frameCounter == 0x32) {
        mainSelf->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a screen flash with shared intensity as the third argument before checking pause. When
 * unpaused adds fifteen below counter sixteen, subtracts fifteen above sixteen and holds exactly
 * at sixteen; advances and retires exactly at thirty-two.
 */
// FUNCTION: FF7 0x00535132
void __cdecl BattleFx_SpecialColdAirWhiteFlashPulseTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(0, 0, *(char *)&g_BattleSpecColdAirFlashIntensity);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 0x10) {
        g_BattleSpecColdAirFlashIntensity += 0xF;
    }
    if (task->frameCounter > 0x10) {
        g_BattleSpecColdAirFlashIntensity -= 0xF;
    }
    (task->frameCounter)++;
    if (task->frameCounter == 0x20) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the growing ring model and the cycling inner model at the slot, then the glow sprite whose
 * colour fades with g_BattleSpecColdAirScaleFactor, and shakes the camera focal point; the
 * scale factor drops after frame 14 and the slot is freed at frame 30.
 */
// FUNCTION: FF7 0x005351d4
void __cdecl BattleFx_SpecialColdAirDrawExpandingModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxOilTargetModelSlot *effectSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix worldMatrix; };
    } frame;
#pragma pack(pop)

    frame.effectSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecColdAirSpriteReq2.drawBlock = Gfx_ModelTableGetLoadedRecord(0x12, g_BattleSpecColdAirLoadedResTable);
    g_BattleSpecColdAirSpriteReq2.handle = Gfx_ModelTableGetSpriteObject(0x12, g_BattleSpecColdAirLoadedResTable);
    frame.angleX.vz = 0;
    frame.angleX.vx = frame.angleX.vz;
    frame.angleX.vy = 0;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecColdAirScaleVector3_Z = frame.effectSlot->frameCounter << 0xA;
    g_BattleSpecColdAirScaleVector3_Y = g_BattleSpecColdAirScaleVector3_Z;
    g_BattleSpecColdAirScaleVector3_X = g_BattleSpecColdAirScaleVector3_Y;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecColdAirScaleVector3_X);
    frame.rotMatrix.t[0] = frame.effectSlot->posX;
    frame.rotMatrix.t[1] = frame.effectSlot->posY;
    frame.rotMatrix.t[2] = frame.effectSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.worldMatrix);
    SetRotMatrix(&frame.worldMatrix);
    SetTransMatrix(&frame.worldMatrix);
    (g_BattleSpecColdAirSpriteReq1.modelResource) = Gfx_ModelTableGetRenderObject(1, g_BattleSpecColdAirLoadedResTable);
    g_BattleSpecColdAirSpriteReq1.size = 0xA8;
    g_BattleSpecColdAirSpriteReq1.scaleWord = 0x1000 - (g_BattleSpecColdAirScaleFactor << 7);
    g_BattleSpecColdAirPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecColdAirSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecColdAirPacketCursor);
    (g_BattleSpecColdAirSpriteReq1.modelResource) = Gfx_ModelTableGetRenderObject(g_BattleSpecColdAirModelIndices1[frame.effectSlot->frameCounter % 0xA], g_BattleSpecColdAirLoadedResTable);
    g_BattleSpecColdAirSpriteReq1.size = 0x28;
    g_BattleSpecColdAirSpriteReq1.scaleWord = g_BattleSpecColdAirScaleFactor << 3;
    g_BattleSpecColdAirPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecColdAirSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecColdAirPacketCursor);
    BattleFx_InstallBillboardMatrix(&frame.effectSlot->posX, 0x2000, 0);
    g_BattleSpecColdAirSpriteReq3.blue = g_BattleSpecColdAirScaleFactor << 3;
    g_BattleSpecColdAirSpriteReq3.green = g_BattleSpecColdAirSpriteReq3.blue;
    g_BattleSpecColdAirSpriteReq3.red = g_BattleSpecColdAirSpriteReq3.green;
    g_BattleSpecColdAirPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecColdAirSpriteReq2.drawBlock, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecColdAirPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleCameraFocalPoint.focalPointY += (short)(0x20 - ((frame.effectSlot->frameCounter % 2) << 6));
    if (frame.effectSlot->frameCounter > 0xE) {
        g_BattleSpecColdAirScaleFactor--;
    }
    (frame.effectSlot->frameCounter)++;
    if (frame.effectSlot->frameCounter == 0x1E) {
        frame.effectSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the spinning model and the flat ground glow, both growing and fading with the frame
 * counter, and each unpaused frame spawns rising fade sprites, one rising spinning model and four
 * animated sprites on a circle that widens by 0xF0 a frame; the slot is freed at frame 20.
 */
// FUNCTION: FF7 0x00535463
void __cdecl BattleFx_SpecialColdAirDrawSpinModelsEmitTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[96];
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxColdAirEmitterTask *spawnSlot; };
        struct { unsigned char pad_to_30[0x30]; short spawnIndex; };
        struct { unsigned char pad_to_34[0x34]; BattleFxOilTargetModelSlot *emitSlot; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix worldMatrix; };
        struct { unsigned char pad_to_58[0x58]; short radius; };
        struct { unsigned char pad_to_5C[0x5C]; short spawnAngle; };
    } frame;
#pragma pack(pop)

    frame.emitSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecColdAirSpriteReq4.modelDraw.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecColdAirLoadedResTable);
    g_BattleSpecColdAirSpriteReq4.drawBlock = Gfx_ModelTableGetLoadedRecord(0x13, g_BattleSpecColdAirLoadedResTable);
    g_BattleSpecColdAirSpriteReq4.handle = Gfx_ModelTableGetSpriteObject(0x13, g_BattleSpecColdAirLoadedResTable);
    frame.angleX.vx = 0;
    frame.angleX.vy = frame.emitSlot->frameCounter << 6;
    frame.angleX.vz = 0;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecColdAirScaleVector2_Z = frame.emitSlot->frameCounter * 0x7D0;
    g_BattleSpecColdAirScaleVector2_Y = g_BattleSpecColdAirScaleVector2_Z;
    g_BattleSpecColdAirScaleVector2_X = g_BattleSpecColdAirScaleVector2_Y;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecColdAirScaleVector2_X);
    frame.rotMatrix.t[0] = frame.emitSlot->posX;
    frame.rotMatrix.t[1] = frame.emitSlot->posY;
    frame.rotMatrix.t[2] = frame.emitSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.worldMatrix);
    SetRotMatrix(&frame.worldMatrix);
    SetTransMatrix(&frame.worldMatrix);
    g_BattleSpecColdAirSpriteReq4.modelDraw.fadeLevel = 0x80 - frame.emitSlot->frameCounter * 6;
    g_BattleSpecColdAirPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecColdAirSpriteReq4.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecColdAirPacketCursor);
    frame.angleX.vy = 0;
    frame.angleX.vz = frame.angleX.vy;
    frame.angleX.vx = 0x400;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecColdAirScaleVector2_Z = frame.emitSlot->frameCounter * 0x320;
    g_BattleSpecColdAirScaleVector2_Y = g_BattleSpecColdAirScaleVector2_Z;
    g_BattleSpecColdAirScaleVector2_X = g_BattleSpecColdAirScaleVector2_Y;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecColdAirScaleVector2_X);
    frame.rotMatrix.t[0] = frame.emitSlot->posX;
    frame.rotMatrix.t[1] = 0;
    frame.rotMatrix.t[2] = frame.emitSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.worldMatrix);
    SetRotMatrix(&frame.worldMatrix);
    SetTransMatrix(&frame.worldMatrix);
    g_BattleSpecColdAirColorData.blue = 0x80 - frame.emitSlot->frameCounter * 6;
    g_BattleSpecColdAirColorData.green = g_BattleSpecColdAirColorData.blue;
    (g_BattleSpecColdAirColorData.red) = g_BattleSpecColdAirColorData.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecColdAirSpriteReq4.drawBlock, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.radius = frame.emitSlot->frameCounter * 0xF0;
    frame.spawnSlot = (BattleFxColdAirEmitterTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialColdAirRisingFadeSpriteTask);
    for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
        frame.spawnAngle = (rand() % 4096);
        frame.spawnSlot->offsetX = CosFixed(frame.spawnAngle) * frame.radius >> 0xC;
        frame.spawnSlot->offsetZ = SinFixed(frame.spawnAngle) * frame.radius >> 0xC;
        frame.spawnSlot->offsetY = 0;
        frame.spawnSlot->randomIndex = (rand() % 16);
    }
    frame.spawnSlot = (BattleFxColdAirEmitterTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialColdAirRisingSpinningModelTask);
    frame.spawnAngle = (rand() % 4096);
    frame.spawnSlot->offsetX = CosFixed(frame.spawnAngle) * frame.radius >> 0xC;
    frame.spawnSlot->offsetZ = SinFixed(frame.spawnAngle) * frame.radius >> 0xC;
    frame.spawnSlot->offsetY = 0;
    frame.spawnSlot->randomIndex = (rand() % 4);
    frame.spawnSlot->randomAngle = (rand() % 4096);
    for (frame.spawnIndex = 0; frame.spawnIndex < 4; frame.spawnIndex++) {
        frame.spawnSlot = (BattleFxColdAirEmitterTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialColdAirAnimatedSpriteTask);
        frame.spawnAngle = (rand() % 4096);
        frame.spawnSlot->offsetX = CosFixed(frame.spawnAngle) * frame.radius >> 0xC;
        frame.spawnSlot->offsetZ = SinFixed(frame.spawnAngle) * frame.radius >> 0xC;
        frame.spawnSlot->offsetY = 0;
    }
    (frame.emitSlot->frameCounter)++;
    if (frame.emitSlot->frameCounter == 0x14) {
        frame.emitSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame step for an animated sprite. Takes the sprite from entry 0x11 of
 * g_BattleSpecColdAirLoadedResTable into g_BattleSpecColdAirSpriteReq3.drawBlock, but the second handle
 * from Battle3d_GetSceneRenderObject(1, g_pBattle3dScene) rather than from the table, installs the
 * billboard matrix from the slot's position at scale 0x2000, copies the frame counter into the
 * request's frameIndex and submits it. When unpaused and the sprite is loaded it advances the
 * counter and frees the slot once it reaches the sprite's frame count.
 */
// FUNCTION: FF7 0x005358e0
void __cdecl BattleFx_SpecialColdAirAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecColdAirSpriteReq3.drawBlock = Gfx_ModelTableGetLoadedRecord(0x11, g_BattleSpecColdAirLoadedResTable);
    g_BattleSpecColdAirSpriteReq3.sceneObject = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix(task->data, 0x2000, 0);
    g_BattleSpecColdAirSpriteReq3.frameIndex = task->frameCounter;
    g_BattleSpecColdAirPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecColdAirSpriteReq3.drawBlock, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecColdAirPacketCursor);
    if (g_BattleIsPaused == 0 && g_BattleSpecColdAirSpriteReq3.drawBlock != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)g_BattleSpecColdAirSpriteReq3.drawBlock + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws one of four models at the slot, turned by its spin angle and faded by 8 a frame; when
 * unpaused it rises by 0x64 and spins by 0x100 a frame and is freed at frame 15.
 */
// FUNCTION: FF7 0x005359b1
void __cdecl BattleFx_SpecialColdAirRisingSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxColdAirRisingSpinningModelSlot *risingSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix worldMatrix; };
    } frame;
#pragma pack(pop)

    frame.risingSlot = (BattleFxColdAirRisingSpinningModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleX.vy = 0;
    frame.angleX.vx = frame.angleX.vy;
    frame.angleX.vz = frame.risingSlot->spinAngle;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecColdAirScaleVector1_Z = 0x1000;
    g_BattleSpecColdAirScaleVector1_Y = g_BattleSpecColdAirScaleVector1_Z;
    g_BattleSpecColdAirScaleVector1_X = g_BattleSpecColdAirScaleVector1_Y;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecColdAirScaleVector1_X);
    frame.rotMatrix.t[0] = frame.risingSlot->posX;
    frame.rotMatrix.t[1] = frame.risingSlot->posY;
    frame.rotMatrix.t[2] = frame.risingSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.worldMatrix);
    SetRotMatrix(&frame.worldMatrix);
    SetTransMatrix(&frame.worldMatrix);
    g_BattleSpecColdAirSpriteReq3.fadeLevel = 0x80 - (frame.risingSlot->frameCounter << 3);
    g_BattleSpecColdAirSpriteReq3.modelGroup = Gfx_ModelTableGetRenderObject(g_BattleSpecColdAirModelIndices2.modelIds[frame.risingSlot->modelIndex], g_BattleSpecColdAirLoadedResTable);
    g_BattleSpecColdAirPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecColdAirSpriteReq3.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecColdAirPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.risingSlot->posY -= 0x64;
    frame.risingSlot->spinAngle += 0x100;
    (frame.risingSlot->frameCounter)++;
    if (frame.risingSlot->frameCounter == 0xF) {
        frame.risingSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame step for a rising, fading sprite. Reads entry 0x10 of g_BattleSpecColdAirLoadedResTable
 * into the rising fade handles of g_BattleSpecColdAirModelIndices2, installs the billboard matrix
 * from the slot's position at scale 0x2000, and takes the request's frameIndex from the slot's
 * frame rather than from its fade step. The three colour bytes of g_BattleSpecColdAirSpriteReq4 all
 * get 0x80 - (fadeStep << 3), a fade from 128 by 8 a frame. When unpaused it raises the slot by
 * 0x12C a frame and frees it after 15 frames.
 */
// FUNCTION: FF7 0x00535b28
void __cdecl BattleFx_SpecialColdAirRisingFadeSpriteTask(void)
{
    BattleFxColdAirRisingFadeSprite *task;

    task = (BattleFxColdAirRisingFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecColdAirModelIndices2.risingFadeDrawBlock = Gfx_ModelTableGetLoadedRecord(0x10, g_BattleSpecColdAirLoadedResTable);
    g_BattleSpecColdAirModelIndices2.risingFadeHandle = Gfx_ModelTableGetSpriteObject(0x10, g_BattleSpecColdAirLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000, 0);
    g_BattleSpecColdAirSpriteReq4.frameIndex = task->frame;
    g_BattleSpecColdAirSpriteReq4.blue = 0x80 - (task->fadeStep << 3);
    g_BattleSpecColdAirSpriteReq4.green = g_BattleSpecColdAirSpriteReq4.blue;
    g_BattleSpecColdAirSpriteReq4.red = g_BattleSpecColdAirSpriteReq4.green;
    g_BattleSpecColdAirPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecColdAirModelIndices2.risingFadeDrawBlock, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecColdAirPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posY -= 0x12C;
    (task->fadeStep)++;
    if (task->fadeStep == 0xF) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
