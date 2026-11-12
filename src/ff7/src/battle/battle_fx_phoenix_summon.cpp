// src/ff7/src/battle/battle_fx_phoenix_summon.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Runs the Phoenix summon sequence: loading, the camera script with easing, the timeline,
//   rising and falling sprites, spinning quads, ring columns and grid waves, trailed models, and a
//   pooled damped sprite effect.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "lib/codegen.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_summon.h"
#include "ff7/enums/battle.h"
#include "lib/enums/graphics.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
#include "lib/api/sound.h"
extern "C" {

struct GfxRotationAngles2;
struct Vec3sPacked;
struct GfxPsxSpriteQuadState;
struct BattleFxModelDraw;

// clang-format off
unsigned char g_BattleFxPhoenixSummonSfxGroups[48] = {
    0xf1, 0x01, 0x00, 0x00, 0xf2, 0x01, 0x00, 0x00, 0xf3, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0xf5, 0x01, 0x00, 0x00,
    0xf6, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7, 0x01, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0xf9, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x0089a5b8
struct SoundGroup *g_BattleSummonPhoenixResourceDef = (struct SoundGroup *)0x0089A588;
unsigned char *g_BattleFxPhoenixSummonDataPool[1] = {
    &g_BattleFxPhoenixSummonSfxGroups[0x10]
};
unsigned char *g_BattleFxPhoenixSummonDataPool2[2] = {
    &g_BattleFxPhoenixSummonSfxGroups[0x20], 0
};
// GLOBAL: FF7 0x0089a5c8
int g_BattleSummonPhoenixSpriteDef = 0x00C800B1;
unsigned char *g_BattleFxPhoenixSummonDataPool3[1] = {
    &g_BattleFxPhoenixSummonResourceNames[0]
};
BattleFxPhoenixResourceTableTail g_BattleSummonPhoenixSpriteDefEntries = {
    {0, 0, 0},
    {
        {0xc800b1, &g_BattleFxPhoenixSummonResourceNames[0xc], 0, 0, 0},
        {0xc800b1, &g_BattleFxPhoenixSummonResourceNames[0x18], 0, 0, 0},
        {0xc800b1, &g_BattleFxPhoenixSummonResourceNames[0x24], 0, 0, 0},
        {0xc800b1, &g_BattleFxPhoenixSummonResourceNames[0x30], 0, 0, 0},
        {0xc800b1, &g_BattleFxPhoenixSummonResourceNames[0x3c], 0, 0, 0},
        {0xc82a39, &g_BattleFxPhoenixSummonResourceNames[0x48], &g_BattleFxPhoenixSummonResourceNames[0x54], 0, 0},
        {0xc82a39, &g_BattleFxPhoenixSummonResourceNames[0x60], &g_BattleFxPhoenixSummonResourceNames[0x6c], 0, 0},
        {0xc82a39, &g_BattleFxPhoenixSummonResourceNames[0x78], &g_BattleFxPhoenixSummonResourceNames[0x84], 0, 0}
    },
    0
};
// GLOBAL: FF7 0x0089a680
BattleFxPhoenixTransformData g_BattleSummonPhoenixTransformData1 = {0, -1000, 0, 0};
// GLOBAL: FF7 0x0089a688
BattleSummonPhoenixParticle *g_BattleSummonPhoenixBufferPtrTable[2] = {g_BattleSummonPhoenixBuffer2, 0};
// GLOBAL: FF7 0x0089a690
unsigned char g_BattleSummonPhoenixParam1[176] = {0x81, 0x00, 0x00, 0x00, 0x3c, 0xf6, 0x00, 0x00, 0x41, 0x00, 0x24, 0xfa, 0x30, 0xf8, 0xdc, 0x05, 0x80, 0x00, 0x0e, 0x00, 0x40, 0x00, 0x0e, 0x00, 0x82, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x0f, 0x00, 0x80, 0x00, 0x19, 0x00, 0x42, 0x00, 0x19, 0x00, 0x48, 0xf4, 0x0c, 0xfe, 0xb8, 0x0b, 0x80, 0x00, 0x14, 0x00, 0x42, 0x00, 0x14, 0x00, 0x78, 0xec, 0x18, 0xfc, 0x88, 0x13, 0x82, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x48, 0xf4, 0x00, 0x00, 0x43, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x30, 0xf8, 0x58, 0x1b, 0x80, 0x00, 0x1e, 0x00, 0x40, 0x00, 0x1e, 0x00, 0x85, 0x00, 0x05, 0x00, 0xff, 0x02, 0x00, 0x00, 0x24, 0xfa, 0x00, 0x00, 0x45, 0x00, 0x05, 0x00, 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x98, 0x3a, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00, 0x0f, 0x00, 0x82, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x78, 0xec, 0x00, 0x00, 0x42, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x81, 0x00, 0x0c, 0xfe, 0x00, 0x00, 0x70, 0x17, 0x41, 0x00, 0xc0, 0xe0, 0xa8, 0xe4, 0x10, 0x27, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00, 0x0f, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089a740
unsigned char g_BattleSummonPhoenixParam2[176] = {0x81, 0x00, 0x00, 0x00, 0x3c, 0xf6, 0x00, 0x00, 0x41, 0x00, 0x24, 0xfa, 0x30, 0xf8, 0x00, 0x00, 0x80, 0x00, 0x0e, 0x00, 0x40, 0x00, 0x0e, 0x00, 0x82, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x0f, 0x00, 0x80, 0x00, 0x19, 0x00, 0x42, 0x00, 0x19, 0x00, 0x48, 0xf4, 0x0c, 0xfe, 0x18, 0xfc, 0x80, 0x00, 0x14, 0x00, 0x42, 0x00, 0x14, 0x00, 0x78, 0xec, 0x18, 0xfc, 0x60, 0xf0, 0x82, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x48, 0xf4, 0x00, 0x00, 0x43, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x30, 0xf8, 0x58, 0x1b, 0x80, 0x00, 0x1e, 0x00, 0x40, 0x00, 0x1e, 0x00, 0x85, 0x00, 0x05, 0x00, 0xff, 0x02, 0x00, 0x00, 0x24, 0xfa, 0x00, 0x00, 0x45, 0x00, 0x05, 0x00, 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x98, 0x3a, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00, 0x0f, 0x00, 0x82, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x78, 0xec, 0x00, 0x00, 0x42, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x81, 0x00, 0x0c, 0xfe, 0x00, 0x00, 0x70, 0x17, 0x41, 0x00, 0xc0, 0xe0, 0xa8, 0xe4, 0x10, 0x27, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00, 0x0f, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089a7f0
struct BattleSummonPhoenixAnimData1 g_BattleSummonPhoenixAnimData1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};
// GLOBAL: FF7 0x0089a800
struct BattleSummonPhoenixAnimData2 g_BattleSummonPhoenixAnimData2 = {0, 0, {0x80, 0x80, 0x80, 0x2e, 0, 0}};
// GLOBAL: FF7 0x0089a810
struct BattleSummonPhoenixSpriteRequest1 g_BattleSummonPhoenixSpriteRequest1 = {0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0089a820
struct BattleSummonPhoenixSpriteRequest2 g_BattleSummonPhoenixSpriteRequest2 = {0, 0, {0x2e808080}, {0}};
// GLOBAL: FF7 0x0089a830
struct BattleSummonPhoenixCoordData1 g_BattleSummonPhoenixCoordData1 = {-100, -1000, {0}};
// GLOBAL: FF7 0x0089a838
struct BattleSummonPhoenixCoordData2 g_BattleSummonPhoenixCoordData2 = {100, -1000, {0}};
// GLOBAL: FF7 0x0089a840
struct BattleSummonPhoenixCoordData3 g_BattleSummonPhoenixCoordData3 = {-100, 0, {0}};
// GLOBAL: FF7 0x0089a848
struct BattleSummonPhoenixCoordData4 g_BattleSummonPhoenixCoordData4 = {100, 0, {0}};
unsigned char g_rawData_0089a850[8] = {0x80, 0x80, 0x40, 0x2a, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089a858
struct BattleSummonPhoenixSpriteRequest3 g_BattleSummonPhoenixSpriteRequest3 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0089a868
struct BattleSummonPhoenixSpriteRequest4 g_BattleSummonPhoenixSpriteRequest4 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0089a878
struct BattleSummonPhoenixSpriteRequest5 g_BattleSummonPhoenixSpriteRequest5 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0089a888
unsigned char g_BattleSummonPhoenixScale1[16] = {0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089a898
struct GfxRotationAngles2 g_BattleSummonPhoenixCoordData5 = {-128, 0, 0};
// GLOBAL: FF7 0x0089a8a0
short g_BattleSummonPhoenixCoordData6[3] = {0, 0, 0x600};
// GLOBAL: FF7 0x0089a8a8
short g_BattleSummonPhoenixCoordData7[3] = {-50, -3000, 0};
// GLOBAL: FF7 0x0089a8b0
struct BattleSummonPhoenixSpriteRequest6 g_BattleSummonPhoenixSpriteRequest6 = {50, {0x48, 0xf4, 0, 0, 0, 0}, {0}};
// GLOBAL: FF7 0x0089a8c0
struct BattleSummonPhoenixAnimData3 g_BattleSummonPhoenixAnimData3 = {0, 0, {0x80, 0x80, 0x80, 0x2e, 0, 0}};
// GLOBAL: FF7 0x0089a8d0
struct GfxRotationAngles2 g_BattleSummonPhoenixCoordData8 = {-128, 0, 0};
// GLOBAL: FF7 0x0089a8d8
int g_BattleSummonPhoenixParam3 = (-1);
// GLOBAL: FF7 0x0089a8e0
unsigned char g_BattleSummonPhoenixTransformMatrix1[30] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089a900
unsigned char g_BattleSummonPhoenixTransformMatrix2[30] = {0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089a920
struct BattleSummonPhoenixSpriteRequest7 g_BattleSummonPhoenixSpriteRequest7 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0089a930
struct BattleSummonPhoenixSpriteRequest8 g_BattleSummonPhoenixSpriteRequest8 = {0, 0, {0x2e808080}, {0}};
// GLOBAL: FF7 0x0089a940
struct BattleSummonPhoenixAngleData1 g_BattleSummonPhoenixAngleData1 = {0x300, 0, 0xc0, 0xc0};
// GLOBAL: FF7 0x0089a948
short g_BattleSummonPhoenixAngleData2[3] = {0, 200, 0};
// GLOBAL: FF7 0x0089a950
short g_BattleSummonPhoenixAngleData3[3] = {0, (-800), 1500};
unsigned char g_BattleFxPhoenixSummonResourceNames[144] =
    "fire_1.rsd\0\0"
    "fire_2.rsd\0\0"
    "shine_1.rsd\0"
    "rainbow.rsd\0"
    "flame1.rsd\0\0"
    "flame2.rsd\0\0"
    "tama_1.tim\0\0"
    "tama_1.spt\0\0"
    "kira_1.tim\0\0"
    "kira_1.spt\0\0"
    "tama_2.tim\0\0"
    "tama_2.spt";
// GLOBAL: FF7 0x00b51d18
struct BattleSummonPhoenixMeshWorkingBlob g_BattleSummonPhoenixMeshWorkingBlob = {0};
unsigned char g_bssPad_00b52db8[24] = {0};
// GLOBAL: FF7 0x00b52dd0
BattleFxPhoenixRingGridQuad g_BattleSummonPhoenixSourceMeshAndPoseBlob[0x20][0x10] = {0};
// GLOBAL: FF7 0x00b59dd0
CameraScriptTrack g_BattleSummonPhoenixCameraFocus = {0};
// GLOBAL: FF7 0x00b59dfc
BattleFxPhoenixBillboardState *g_BattleSummonPhoenixState = 0;
// GLOBAL: FF7 0x00b59e00
BattleFxPhoenixRotatePos *g_BattleSummonPhoenixBuffer1 = 0;
unsigned char g_bssPad_00b59e04[4] = {0};
// GLOBAL: FF7 0x00b59e08
struct BattleSummonPhoenixParticle g_BattleSummonPhoenixBuffer2[100] = {0};
unsigned char g_bssPad_00b5a5d8[200] = {0};
// GLOBAL: FF7 0x00b5a6a0
int g_BattleSummonPhoenixTargetPos = 0;
// GLOBAL: FF7 0x00b5a6a4
unsigned char g_BattleSummonPhoenixFlag1 = 0;
// GLOBAL: FF7 0x00b5a6a8
int g_BattleSummonPhoenixBufferPtr = 0;
unsigned char g_bssPad_00b5a6ac[4] = {0};
// GLOBAL: FF7 0x00b5a6b0
char g_BattleSummonPhoenixColumnUsed[20] = {0};
// GLOBAL: FF7 0x00b5a6c4
CameraScriptTrack *g_BattleSummonPhoenixCameraTrack = 0;
// GLOBAL: FF7 0x00b5a6c8
CameraScriptTrack *g_BattleSummonPhoenixCameraOtherTrack = 0;
// GLOBAL: FF7 0x00b5a6cc
unsigned char g_BattleSummonPhoenixFlag2 = 0;
unsigned char g_bssPad_00b5a6d0[4] = {0};
// GLOBAL: FF7 0x00b5a6d4
struct BattleFxPhoenixTimelineScratch *g_BattleSummonPhoenixTimelineState = 0;
// GLOBAL: FF7 0x00b5a6d8
CameraScriptTrack g_BattleSummonPhoenixCameraEye = {0};
// GLOBAL: FF7 0x00b5a704
unsigned char g_BattleSummonPhoenixOpacity = 0;
// GLOBAL: FF7 0x00b5a708
unsigned char g_BattleSummonPhoenixFlag3 = 0;
// GLOBAL: FF7 0x00b5a70c
struct BattleFxPhoenixGrowState *g_BattleSummonPhoenixLoadedResTable = 0;
// GLOBAL: FF7 0x00b5a710
struct GfxTableCursor *g_BattleSummonPhoenixTimer = 0;
unsigned char g_bssPad_00b5a714[4] = {0};
// GLOBAL: FF7 0x00b5a718
struct BattleSummonPhoenixTransformMatrix3 g_BattleSummonPhoenixTransformMatrix3 = {0};
// GLOBAL: FF7 0x00b5a738
BattleFxPhoenixAnglesXY g_BattleSummonPhoenixCoordBuffer1 = {0};
unsigned char g_bssPad_00b5a73c[4] = {0};
// GLOBAL: FF7 0x00b5a740
short g_BattleSummonPhoenixCoordBuffer2[2] = {0};
unsigned char g_bssPad_00b5a744[4] = {0};
// GLOBAL: FF7 0x00b5a748
short g_BattleSummonPhoenixStep1 = 0;
unsigned char g_bssPad_00b5a74c[4] = {0};
// GLOBAL: FF7 0x00b5a750
BattleSummonPhoenixBillboardScratch g_BattleSummonPhoenixAnimBuffer = {0};
// GLOBAL: FF7 0x00b5a760
BattleFxPhoenixAngleScalar g_BattleSummonPhoenixStateData1 = {0};
// GLOBAL: FF7 0x00b5a764
BattleFxPhoenixAngleScalar g_BattleSummonPhoenixStateData2 = {0};
// GLOBAL: FF7 0x00b5a768
int g_BattleSummonPhoenixActiveCount = 0;
unsigned char g_bssPad_00b5a76c[4] = {0};
// GLOBAL: FF7 0x00b5a770
short g_BattleSummonPhoenixStep2 = 0;
unsigned char g_bssPad_00b5a774[4] = {0};
// GLOBAL: FF7 0x00b5a778
struct BattleSummonPhoenixBuffer3 g_BattleSummonPhoenixBuffer3 = {0};
// GLOBAL: FF7 0x00b5a790
struct Vec3sPacked g_BattleSummonPhoenixCoordBuffer3 = {0};
// GLOBAL: FF7 0x00b5a798
int g_BattleSummonPhoenixStateData3 = 0;
unsigned char g_bssPad_00b5a79c[4] = {0};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/* Loads SUMMON/FENIC model/sprite resources and registers Phoenix sound groups when uncached. */
// FUNCTION: FF7 0x00515060
void __cdecl BattleFx_SummonLoadResPhoenix(void)
{
    GfxModelTableBank *calcSceneWord;
    GfxResourceRequest recordBuf;
    int kind;
    char tempPathOutName[200];
    GfxResourceRequest *pRequest;
    int *taskSpriteDef;

    if (g_pBattle3dScene != 0 && g_BattleSummonPhoenixTimer == 0) {
        kind = 9;
        taskSpriteDef = &g_BattleSummonPhoenixSpriteDef;
        calcSceneWord = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recordBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089a9e8
        pRequest->resourcePath = BattleFx_BuildResourcePath("SUMMON/FENIC", tempPathOutName);
        Gfx_LoadModelTableIfAbsent(kind, (int)taskSpriteDef, pRequest, &g_BattleSummonPhoenixTimer, calcSceneWord);
        SfxRegisterSoundGroups(&g_BattleSummonPhoenixResourceDef);
    }
}

/* Acquires the render-state block, loads Phoenix resources, and starts the Phoenix summon sequence. */
// FUNCTION: FF7 0x00515101
int __cdecl BattleFx_SummonEnterPhoenix(int targetMask, int actorIdx)
{
    g_BattleSummonPhoenixBufferPtr = Gfx_GetPsxPacketScratchPtr();
    BattleFx_SummonLoadResPhoenix();
    BattleFx_SummonBeginPhoenix(targetMask, actorIdx);
    return 0;
}

/* Orientates the summon matrix toward the target centroid, starts a camera script, and spawns the timeline task. */
// FUNCTION: FF7 0x00515127
void __cdecl BattleFx_SummonBeginPhoenix(int targetMask, int actorIdx)
{
    SVECTOR centroidBuf;

    Battle_GetActorMaskCentroid(g_BattleSummonPhoenixTargetPos = targetMask, &centroidBuf.vx);
    if (centroidBuf.vz > g_BattleActorRecords[actorIdx].posZ) {
        g_BattleSummonPhoenixTransformMatrix3.offsetZ = centroidBuf.vz - 0x1388;
    } else {
        g_BattleSummonPhoenixCoordBuffer1.rotY = 0x800;
        g_BattleSummonPhoenixTransformMatrix3.offsetZ = centroidBuf.vz + 0x1388;
    }
    RotMatrix((SVECTOR *)&g_BattleSummonPhoenixCoordBuffer1, &g_BattleSummonPhoenixTransformMatrix3.matrix);
    if ((rand() & 0x100) != 0) {
        BattleFx_PhoenixSpawnCameraScript(g_BattleSummonPhoenixParam1, &g_BattleSummonPhoenixTransformMatrix3.matrix, actorIdx);
    } else {
        BattleFx_PhoenixSpawnCameraScript(g_BattleSummonPhoenixParam2, &g_BattleSummonPhoenixTransformMatrix3.matrix, actorIdx);
    }
    Battle_AllocateResourceSlot(BattleFx_SummonPhoenixTimelineTask);
    g_BattleStageHideFlags |= BATTLE_STAGE_HIDE_RAIN;
}

/* Allocates a task slot that runs BattleFx_PhoenixRunCameraScript and fills in its record. */
// FUNCTION: FF7 0x005151f8
void __cdecl BattleFx_PhoenixSpawnCameraScript(const void *scriptParam, PsxMatrix *matrixPtr, int actorIndex)
{
    BattleFxPhoenixCameraTask *taskRecord;

    taskRecord = (BattleFxPhoenixCameraTask *)BATTLE_NEW_TASK_SLOT(BattleFx_PhoenixRunCameraScript);
    taskRecord->scriptParam = scriptParam;
    taskRecord->matrixPtr = matrixPtr;
    taskRecord->actorIndex = actorIndex;
}

/* Decodes eye/focus camera commands, advances both tracks, then writes the battle camera. */
// FUNCTION: FF7 0x00515238
void __cdecl BattleFx_PhoenixRunCameraScript(void)
{
    int curve;
    CameraScriptTask *taskPtr;
    int *wideVec;
    int projFlags, trackIndex;
    CameraScriptVector *baseScratchVec;
    int v;
    CameraScriptVector *vecA;
    int x[66];
    short opcode;

    taskPtr = (CameraScriptTask *)((char *)BATTLE_CURRENT_TASK_SLOT);
    vecA = (CameraScriptVector *)Gfx_GetPsxScratchpadPtr(0);
    baseScratchVec = vecA + 1;
    wideVec = (int *)(baseScratchVec + 1);
    if (g_BattleIsPaused == 0) {
        if (taskPtr->initialized == 0) {
            g_BattleSummonPhoenixCameraEye.frames = 0;
            g_BattleSummonPhoenixCameraFocus.frames = 0;
            g_BattleSummonPhoenixCameraEye.actor = -1;
            g_BattleSummonPhoenixCameraFocus.actor = -1;
            taskPtr->initialized = 1;
        }
        while (g_BattleSummonPhoenixCameraEye.frames == 0 || g_BattleSummonPhoenixCameraFocus.frames == 0) {
            opcode = *taskPtr->cursor++;
            if (opcode & 0x40) {
                g_BattleSummonPhoenixCameraTrack = &g_BattleSummonPhoenixCameraEye;
                g_BattleSummonPhoenixCameraOtherTrack = &g_BattleSummonPhoenixCameraFocus;
            } else {
                g_BattleSummonPhoenixCameraTrack = &g_BattleSummonPhoenixCameraFocus;
                g_BattleSummonPhoenixCameraOtherTrack = &g_BattleSummonPhoenixCameraEye;
            }
            g_BattleSummonPhoenixCameraTrack->mode = opcode & ~0xc0;
            switch (g_BattleSummonPhoenixCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                g_BattleSummonPhoenixCameraTrack->frames = BattleFx_PhoenixReadCameraScriptOperand(&taskPtr->cursor);
                break;
            case SUMMON_CAMERA_SET_POSITION:
                g_BattleSummonPhoenixCameraTrack->position.x = *taskPtr->cursor++;
                g_BattleSummonPhoenixCameraTrack->position.y = *taskPtr->cursor++;
                g_BattleSummonPhoenixCameraTrack->position.z = *taskPtr->cursor++;
                break;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_BattleSummonPhoenixCameraTrack->frames = BattleFx_PhoenixReadCameraScriptOperand(&taskPtr->cursor);
                g_BattleSummonPhoenixCameraTrack->velocity.x = (*taskPtr->cursor++ - g_BattleSummonPhoenixCameraTrack->position.x) / g_BattleSummonPhoenixCameraTrack->frames;
                g_BattleSummonPhoenixCameraTrack->velocity.y = (*taskPtr->cursor++ - g_BattleSummonPhoenixCameraTrack->position.y) / g_BattleSummonPhoenixCameraTrack->frames;
                g_BattleSummonPhoenixCameraTrack->velocity.z = (*taskPtr->cursor++ - g_BattleSummonPhoenixCameraTrack->position.z) / g_BattleSummonPhoenixCameraTrack->frames;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                vecA->x = g_BattleSummonPhoenixCameraOtherTrack->position.x - g_BattleSummonPhoenixCameraTrack->position.x;
                vecA->y = g_BattleSummonPhoenixCameraOtherTrack->position.y - g_BattleSummonPhoenixCameraTrack->position.y;
                vecA->z = g_BattleSummonPhoenixCameraOtherTrack->position.z - g_BattleSummonPhoenixCameraTrack->position.z;
                g_BattleSummonPhoenixCameraTrack->distance = SqrtToInt(vecA->x * vecA->x + vecA->y * vecA->y + vecA->z * vecA->z);
                g_BattleSummonPhoenixCameraTrack->base = g_BattleSummonPhoenixCameraTrack->position;
                g_BattleSummonPhoenixCameraTrack->frames = BattleFx_PhoenixReadCameraScriptOperand(&taskPtr->cursor);
                vecA->x = *taskPtr->cursor++;
                vecA->y = *taskPtr->cursor++;
                vecA->z = *taskPtr->cursor++;
                g_BattleSummonPhoenixCameraTrack->velocity.x = (vecA->x - g_BattleSummonPhoenixCameraTrack->position.x) / g_BattleSummonPhoenixCameraTrack->frames;
                g_BattleSummonPhoenixCameraTrack->velocity.y = (vecA->y - g_BattleSummonPhoenixCameraTrack->position.y) / g_BattleSummonPhoenixCameraTrack->frames;
                g_BattleSummonPhoenixCameraTrack->velocity.z = (vecA->z - g_BattleSummonPhoenixCameraTrack->position.z) / g_BattleSummonPhoenixCameraTrack->frames;
                switch (g_BattleSummonPhoenixCameraOtherTrack->mode) {
                case SUMMON_CAMERA_MOVE_LINEAR:
                    baseScratchVec->x = g_BattleSummonPhoenixCameraOtherTrack->position.x + g_BattleSummonPhoenixCameraOtherTrack->velocity.x * g_BattleSummonPhoenixCameraOtherTrack->frames;
                    baseScratchVec->y = g_BattleSummonPhoenixCameraOtherTrack->position.y + g_BattleSummonPhoenixCameraOtherTrack->velocity.y * g_BattleSummonPhoenixCameraOtherTrack->frames;
                    baseScratchVec->z = g_BattleSummonPhoenixCameraOtherTrack->position.z + g_BattleSummonPhoenixCameraOtherTrack->velocity.z * g_BattleSummonPhoenixCameraOtherTrack->frames;
                    break;
                default:
                    baseScratchVec->x = g_BattleSummonPhoenixCameraOtherTrack->position.x;
                    baseScratchVec->y = g_BattleSummonPhoenixCameraOtherTrack->position.y;
                    baseScratchVec->z = g_BattleSummonPhoenixCameraOtherTrack->position.z;
                    break;
                }
                vecA->x -= baseScratchVec->x;
                vecA->y -= baseScratchVec->y;
                vecA->z -= baseScratchVec->z;
                g_BattleSummonPhoenixCameraTrack->delta = (SqrtToInt(vecA->x * vecA->x + vecA->y * vecA->y + vecA->z * vecA->z) - g_BattleSummonPhoenixCameraTrack->distance) / g_BattleSummonPhoenixCameraTrack->frames;
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_BattleSummonPhoenixCameraTrack->frames = BattleFx_PhoenixReadCameraScriptOperand(&taskPtr->cursor);
                vecA->x = *taskPtr->cursor++ - g_BattleSummonPhoenixCameraTrack->position.x;
                vecA->y = *taskPtr->cursor++ - g_BattleSummonPhoenixCameraTrack->position.y;
                vecA->z = *taskPtr->cursor++ - g_BattleSummonPhoenixCameraTrack->position.z;
                g_BattleSummonPhoenixCameraTrack->base.x = ((vecA->x - g_BattleSummonPhoenixCameraTrack->velocity.x * g_BattleSummonPhoenixCameraTrack->frames) * 2) / (g_BattleSummonPhoenixCameraTrack->frames * g_BattleSummonPhoenixCameraTrack->frames);
                g_BattleSummonPhoenixCameraTrack->base.y = ((vecA->y - g_BattleSummonPhoenixCameraTrack->velocity.y * g_BattleSummonPhoenixCameraTrack->frames) * 2) / (g_BattleSummonPhoenixCameraTrack->frames * g_BattleSummonPhoenixCameraTrack->frames);
                g_BattleSummonPhoenixCameraTrack->base.z = ((vecA->z - g_BattleSummonPhoenixCameraTrack->velocity.z * g_BattleSummonPhoenixCameraTrack->frames) * 2) / (g_BattleSummonPhoenixCameraTrack->frames * g_BattleSummonPhoenixCameraTrack->frames);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_BattleSummonPhoenixCameraTrack->frames = BattleFx_PhoenixReadCameraScriptOperand(&taskPtr->cursor);
                g_BattleSummonPhoenixCameraTrack->easing = *taskPtr->cursor++;
                g_BattleSummonPhoenixCameraTrack->distance = 0;
                g_BattleSummonPhoenixCameraTrack->delta = 0x1000 / g_BattleSummonPhoenixCameraTrack->frames;
                g_BattleSummonPhoenixCameraTrack->base.x = g_BattleSummonPhoenixCameraTrack->position.x;
                g_BattleSummonPhoenixCameraTrack->base.y = g_BattleSummonPhoenixCameraTrack->position.y;
                g_BattleSummonPhoenixCameraTrack->base.z = g_BattleSummonPhoenixCameraTrack->position.z;
                g_BattleSummonPhoenixCameraTrack->velocity.x = *taskPtr->cursor++ - g_BattleSummonPhoenixCameraTrack->position.x;
                g_BattleSummonPhoenixCameraTrack->velocity.y = *taskPtr->cursor++ - g_BattleSummonPhoenixCameraTrack->position.y;
                g_BattleSummonPhoenixCameraTrack->velocity.z = *taskPtr->cursor++ - g_BattleSummonPhoenixCameraTrack->position.z;
                break;
            case SUMMON_CAMERA_ATTACH_BONE:
                if ((g_BattleSummonPhoenixCameraTrack->actor = *taskPtr->cursor++) == 0)
                    g_BattleSummonPhoenixCameraTrack->actor = taskPtr->actor;
                g_BattleSummonPhoenixCameraTrack->bone = *taskPtr->cursor++;
                break;
            case SUMMON_CAMERA_DETACH:
                g_BattleSummonPhoenixCameraTrack->actor = -1;
                break;
            case SUMMON_CAMERA_WAIT_NO_WRITEBACK:
                g_BattleSummonPhoenixCameraTrack->frames = BattleFx_PhoenixReadCameraScriptOperand(&taskPtr->cursor);
                break;
            case SUMMON_CAMERA_SET_SCRIPT_VALUE:
                g_BattleCameraScriptValue = *taskPtr->cursor++;
                break;
            default:
                taskPtr->active = -1;
                return;
            }
        }
        g_BattleSummonPhoenixCameraTrack = &g_BattleSummonPhoenixCameraEye;
        g_BattleSummonPhoenixCameraOtherTrack = &g_BattleSummonPhoenixCameraFocus;
        for (trackIndex = 0; trackIndex < 2; trackIndex++) {
            switch (g_BattleSummonPhoenixCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_BattleSummonPhoenixCameraTrack->velocity.x += g_BattleSummonPhoenixCameraTrack->base.x;
                g_BattleSummonPhoenixCameraTrack->velocity.y += g_BattleSummonPhoenixCameraTrack->base.y;
                g_BattleSummonPhoenixCameraTrack->velocity.z += g_BattleSummonPhoenixCameraTrack->base.z;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_BattleSummonPhoenixCameraTrack->position.x += g_BattleSummonPhoenixCameraTrack->velocity.x;
                g_BattleSummonPhoenixCameraTrack->position.y += g_BattleSummonPhoenixCameraTrack->velocity.y;
                g_BattleSummonPhoenixCameraTrack->position.z += g_BattleSummonPhoenixCameraTrack->velocity.z;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                g_BattleSummonPhoenixCameraTrack->base.x += g_BattleSummonPhoenixCameraTrack->velocity.x;
                g_BattleSummonPhoenixCameraTrack->base.y += g_BattleSummonPhoenixCameraTrack->velocity.y;
                g_BattleSummonPhoenixCameraTrack->base.z += g_BattleSummonPhoenixCameraTrack->velocity.z;
                g_BattleSummonPhoenixCameraTrack->distance += g_BattleSummonPhoenixCameraTrack->delta;
                wideVec[0] = g_BattleSummonPhoenixCameraOtherTrack->position.x - g_BattleSummonPhoenixCameraTrack->base.x;
                wideVec[1] = g_BattleSummonPhoenixCameraOtherTrack->position.y - g_BattleSummonPhoenixCameraTrack->base.y;
                wideVec[2] = g_BattleSummonPhoenixCameraOtherTrack->position.z - g_BattleSummonPhoenixCameraTrack->base.z;
                VectorNormalToSVector(wideVec, (short *)vecA);
                g_BattleSummonPhoenixCameraTrack->position.x = g_BattleSummonPhoenixCameraOtherTrack->position.x - ((vecA->x * g_BattleSummonPhoenixCameraTrack->distance) >> 12);
                g_BattleSummonPhoenixCameraTrack->position.y = g_BattleSummonPhoenixCameraOtherTrack->position.y - ((vecA->y * g_BattleSummonPhoenixCameraTrack->distance) >> 12);
                g_BattleSummonPhoenixCameraTrack->position.z = g_BattleSummonPhoenixCameraOtherTrack->position.z - ((vecA->z * g_BattleSummonPhoenixCameraTrack->distance) >> 12);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_BattleSummonPhoenixCameraTrack->distance += g_BattleSummonPhoenixCameraTrack->delta;
                curve = g_BattleSummonPhoenixCameraTrack->distance;
                curve = BattleFx_PhoenixApplyCameraEasing(curve, g_BattleSummonPhoenixCameraTrack->easing);
                g_BattleSummonPhoenixCameraTrack->position.x = g_BattleSummonPhoenixCameraTrack->base.x + ((g_BattleSummonPhoenixCameraTrack->velocity.x * curve) >> 12);
                g_BattleSummonPhoenixCameraTrack->position.y = g_BattleSummonPhoenixCameraTrack->base.y + ((g_BattleSummonPhoenixCameraTrack->velocity.y * curve) >> 12);
                g_BattleSummonPhoenixCameraTrack->position.z = g_BattleSummonPhoenixCameraTrack->base.z + ((g_BattleSummonPhoenixCameraTrack->velocity.z * curve) >> 12);
                break;
            }
            g_BattleSummonPhoenixCameraTrack->frames--;
            g_BattleSummonPhoenixCameraTrack = &g_BattleSummonPhoenixCameraFocus;
            g_BattleSummonPhoenixCameraOtherTrack = &g_BattleSummonPhoenixCameraEye;
        }
    }
    if (taskPtr->matrix) {
        if (g_BattleSummonPhoenixCameraEye.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            SetRotMatrix((void *)taskPtr->matrix);
            SetTransMatrix((void *)taskPtr->matrix);
            if (g_BattleSummonPhoenixCameraEye.actor == -1) {
                RotTransPoint(&g_BattleSummonPhoenixCameraEye.position, wideVec, &projFlags);
                g_BattleCameraPosition.cameraPosX = wideVec[0];
                g_BattleCameraPosition.cameraPosY = wideVec[1];
                g_BattleCameraPosZ = wideVec[2];
            } else {
                RotateSVectorByInstalledMatrix(&g_BattleSummonPhoenixCameraEye.position, wideVec);
                if (g_BattleSummonPhoenixCameraEye.bone != -1) {
                    Battle_GetBoneWorldPos(g_BattleSummonPhoenixCameraEye.actor, g_BattleSummonPhoenixCameraEye.bone, (short *)vecA);
                } else {
                    vecA->x = g_BattleActorRecords[g_BattleSummonPhoenixCameraEye.actor].posX;
                    vecA->y = g_BattleActorRecords[g_BattleSummonPhoenixCameraEye.actor].posY;
                    vecA->z = g_BattleActorRecords[g_BattleSummonPhoenixCameraEye.actor].posZ;
                }
                g_BattleCameraPosition.cameraPosX = wideVec[0] + vecA->x;
                g_BattleCameraPosition.cameraPosY = wideVec[1] + vecA->y;
                g_BattleCameraPosZ = wideVec[2] + vecA->z;
                SetRotMatrix((void *)taskPtr->matrix);
                SetTransMatrix((void *)taskPtr->matrix);
            }
        }
        if (g_BattleSummonPhoenixCameraFocus.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            if (g_BattleSummonPhoenixCameraFocus.actor == -1) {
                RotTransPoint(&g_BattleSummonPhoenixCameraFocus.position, wideVec, &projFlags);
                g_BattleCameraFocalPoint.focalPointX = wideVec[0];
                g_BattleCameraFocalPoint.focalPointY = wideVec[1];
                g_BattleCameraFocusZ = wideVec[2];
            } else {
                RotateSVectorByInstalledMatrix(&g_BattleSummonPhoenixCameraFocus.position, wideVec);
                if (g_BattleSummonPhoenixCameraFocus.bone != -1) {
                    Battle_GetBoneWorldPos(g_BattleSummonPhoenixCameraFocus.actor, g_BattleSummonPhoenixCameraFocus.bone, (short *)vecA);
                } else {
                    vecA->x = g_BattleActorRecords[g_BattleSummonPhoenixCameraFocus.actor].posX;
                    vecA->y = g_BattleActorRecords[g_BattleSummonPhoenixCameraFocus.actor].posY;
                    vecA->z = g_BattleActorRecords[g_BattleSummonPhoenixCameraFocus.actor].posZ;
                }
                g_BattleCameraFocalPoint.focalPointX = wideVec[0] + vecA->x;
                g_BattleCameraFocalPoint.focalPointY = wideVec[1] + vecA->y;
                g_BattleCameraFocusZ = wideVec[2] + vecA->z;
            }
        }
    } else {
        if (g_BattleSummonPhoenixCameraEye.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            *(int *)&g_BattleCameraPosition = *(int *)&g_BattleSummonPhoenixCameraEye.position;
            *(int *)&g_BattleCameraPosZ = *(int *)&g_BattleSummonPhoenixCameraEye.position.z;
        }
        if (g_BattleSummonPhoenixCameraFocus.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            *(int *)&g_BattleCameraFocalPoint = *(int *)&g_BattleSummonPhoenixCameraFocus.position;
            *(int *)&g_BattleCameraFocusZ = *(int *)&g_BattleSummonPhoenixCameraFocus.position.z;
        }
    }
}

/* Fetches the next camera-script short operand, resolving negative values via operandTable. */
// FUNCTION: FF7 0x0051617e
short __cdecl BattleFx_PhoenixReadCameraScriptOperand(short **cursor)
{
    int operand;

    operand = **cursor;
    *cursor += 1;
    if (operand < 0) {
        operand = g_BattleSummonPhoenixMeshWorkingBlob.operandTable[-operand];
    }
    return (short)operand;
}

/* Applies iterated sine easing and blends with the input value according to easeParam. */
// FUNCTION: FF7 0x005161b8
int __cdecl BattleFx_PhoenixApplyCameraEasing(int value, int easeParam)
{
    int i, iterCount, origValue;

    origValue = value;
    if (easeParam > 0) {
        iterCount = easeParam >> 8;
        for (i = 0; i < iterCount; i++) {
            value = SinFixed(value / 4);
        }
    } else {
        easeParam = -easeParam;
        iterCount = easeParam >> 8;
        value = 0x1000 - value;
        for (i = 0; i < iterCount; i++) {
            value = SinFixed(value / 4);
        }
        value = 0x1000 - value;
    }
    iterCount = (easeParam & 0xFF) + 1;
    value = (value * iterCount + origValue * (0x100 - iterCount)) >> 8;
    return value;
}

/* Master timeline task driving each stage of the Phoenix summon sequence. */
// FUNCTION: FF7 0x00516297
void __cdecl BattleFx_SummonPhoenixTimelineTask(void)
{
    int i8;
    BattleFxPhoenixOffset *ringRecord;
    BattleFxPhoenixSpawnRecord *spawn;
    int textureU;
    int angle;
    int posY;
    int quadH;
    int textureY;
    int spinSpeed;
    int startBytes;
    int tilt;
    BattleTaskSlot *taskPtr;
    int time;
    int bytesX;
    int color;
    int x0;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonPhoenixOpacity & 1) == 0) {
        g_BattleSummonPhoenixOpacity |= 1;
        g_BattleSummonPhoenixTimelineState = (struct BattleFxPhoenixTimelineScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleSummonPhoenixTransformMatrix3.matrix, &g_BattleSummonPhoenixMeshWorkingBlob.billboardMatrix);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (DEAD_IN_RETAIL != 0) {
        x0 = g_BattleSummonPhoenixStateData3 % 0x200;
        g_BattleSummonPhoenixStateData3++;
        posY = 0x64;
        bytesX = 0x100;
        quadH = 0x100;
        angle = 0;
        textureU = 0;
        textureY = 0;
        color = 0xFFFFFFFF;
        if (Battle3d_GetOffscreenRenderObject(g_BattleSummonPhoenixParam3) != 0) {
            Gfx_EmitPsxTexturedSpriteQuad(x0, posY, bytesX, quadH, angle, textureU, textureY, FLOAT_ONE_BITS, color, 0, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleSummonPhoenixParam3));
        }
    }
    time = taskPtr->frameCounter;
    if (time < 0xA) {
        if (time == 0) {
            g_BattleSummonPhoenixAngleData1.spriteX = *(short *)&g_BattleViewportOriginX;
            g_BattleSummonPhoenixAngleData1.spriteY = *(short *)&g_BattleViewportOriginY;
            g_BattleSummonPhoenixCoordBuffer3.vy = GetGeomScreen() * 0x18;
            BattleFx_SummonPhoenixQueueStageOffscreenRequest(&g_BattleSummonPhoenixCoordBuffer3, &g_BattleSummonPhoenixAngleData1);
            BattleFx_SummonPhoenixBuildRingGridMesh();
        }
    } else {
        time -= 0xA;
        if (time < 0x2D) {
            if (time == 0) {
                spawn = (BattleFxPhoenixSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonPhoenixRisingSpriteEmitterTask);
                spawn->posX = 0;
                spawn->posY = 0xF3CB;
                spawn->posZ = 0;
                spawn->endX = 0;
                spawn->endY = 0x9C;
                spawn->endZ = 0;
                spawn->scale = 0x1000;
                Battle_AllocateResourceSlot(BattleFx_SummonPhoenixRingGridWaveTask);
            }
            if (time == 0) {
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 0);
            }
        } else {
            time -= 0x2D;
            if (time < 0x14) {
                if (time == 0) {
                    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 1);
                }
                if (time == 0) {
                    spawn = (BattleFxPhoenixSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonPhoenixCounterSpinModelsTask);
                    spawn->posX = 0;
                    spawn->posY = 0;
                    spawn->posZ = 0;
                }
                if (time == 0x13) {
                    ringRecord = (BattleFxPhoenixOffset *)Battle_AllocSceneMessage(BATTLE_SCENE_MSG_START_SLOT3_ACTOR);
                    ringRecord->posX = g_BattleSummonPhoenixTransformMatrix3.offsetXShort;
                    ringRecord->posY = 0xFC18;
                    ringRecord->posZ = g_BattleSummonPhoenixTransformMatrix3.offsetZShort;
                    ringRecord->angle = g_BattleSummonPhoenixCoordBuffer1.rotY ^ 0x800;
                }
            } else {
                time -= 0x14;
                if (time < 0x14) {
                    if (time == 0) {
                        spawn = (BattleFxPhoenixSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonPhoenixSpawnFallingSpritesTask);
                        spawn = (BattleFxPhoenixSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonPhoenixSpawnRingColumnsTask);
                        Battle_AllocateResourceSlot(BattleFx_SummonPhoenixMoveSummonActorTask);
                        Battle_AllocateResourceSlot(BattleFx_SummonPhoenixGrowingModelTask);
                    }
                } else {
                    time -= 0x14;
                    if (time < 0x19) {
                        if (time < 0xF) {
                            Battle_ComposeMatrixRelativeToCamera(g_BattleCameraTransform.matrix, &g_BattleActorRecords[3].boneNodes5[0x104], &g_BattleSummonPhoenixTimelineState->matrix);
                            SetRotMatrix(&g_BattleSummonPhoenixTransformMatrix3.matrix);
                            SetTransMatrix(&g_BattleSummonPhoenixTimelineState->matrix);
                            RotTransPoint((SVECTOR *)g_BattleSummonPhoenixAngleData2, g_BattleSummonPhoenixTimelineState->outVec, &g_BattleSummonPhoenixTimelineState->rotTransFlag);
                            g_BattleCameraFocalPoint.focalPointX = g_BattleSummonPhoenixTimelineState->posX;
                            g_BattleCameraFocalPoint.focalPointY = g_BattleSummonPhoenixTimelineState->posY;
                            g_BattleCameraFocusZ = g_BattleSummonPhoenixTimelineState->posZ;
                            RotTransPoint((SVECTOR *)g_BattleSummonPhoenixAngleData3, g_BattleSummonPhoenixTimelineState->outVec, &g_BattleSummonPhoenixTimelineState->rotTransFlag);
                            g_BattleCameraPosition.cameraPosX = g_BattleSummonPhoenixTimelineState->posX;
                            g_BattleCameraPosition.cameraPosY = g_BattleSummonPhoenixTimelineState->posY;
                            g_BattleCameraPosZ = g_BattleSummonPhoenixTimelineState->posZ;
                        }
                    } else {
                        time -= 0x19;
                        if (time < 0xA) {
                            if (time == 0) {
                                spawn = (BattleFxPhoenixSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonPhoenixDampedSpritePoolTask);
                                for (i8 = 0; i8 < 0x64; i8++) {
                                    g_BattleSummonPhoenixBufferPtrTable[0][i8].posX = 0;
                                    g_BattleSummonPhoenixBufferPtrTable[0][i8].posY = 0xF830;
                                    g_BattleSummonPhoenixBufferPtrTable[0][i8].posZ = 0;
                                    if (DEAD_IN_RETAIL != 0) {
                                        startBytes = 0x600;
                                    } else {
                                        startBytes = rand() % 0x400 + 0x200;
                                    }
                                    g_BattleSummonPhoenixBufferPtrTable[0][i8].spriteX = startBytes;
                                    g_BattleSummonPhoenixTimelineState->vec.vx = rand() & 0x1FF;
                                    g_BattleSummonPhoenixTimelineState->vec.vy = 0;
                                    if (DEAD_IN_RETAIL != 0) {
                                        spinSpeed = 0x1000;
                                    } else {
                                        spinSpeed = rand() % 0x1000;
                                    }
                                    g_BattleSummonPhoenixTimelineState->vec.vz = spinSpeed;
                                    RotMatrix(&g_BattleSummonPhoenixTimelineState->vec, &g_BattleSummonPhoenixTimelineState->matrix);
                                    g_BattleSummonPhoenixTimelineState->vec.vx = 0;
                                    if (DEAD_IN_RETAIL != 0) {
                                        tilt = 0x1F4;
                                    } else {
                                        tilt = rand() % 0xC8 + 0x12C;
                                    }
                                    g_BattleSummonPhoenixTimelineState->vec.vy = -tilt;
                                    g_BattleSummonPhoenixTimelineState->vec.vz = 0;
                                    TransformVectorToInt16(&g_BattleSummonPhoenixTimelineState->matrix, &g_BattleSummonPhoenixTimelineState->vec, &g_BattleSummonPhoenixBufferPtrTable[0][i8].velocityX);
                                }
                            }
                        } else {
                            time -= 0xA;
                            if (time < 0x14) {
                                if (time == 0) {
                                    spawn = (BattleFxPhoenixSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonPhoenixGrowingDipModelTask);
                                    spawn->posX = 0;
                                    spawn->posY = 0xF448;
                                    spawn->posZ = 0;
                                }
                            } else {
                                time -= 0x14;
                                if (time < 0x14) {
                                    if (time == 0) {
                                        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 2);
                                    }
                                    if (time == 0) {
                                        spawn = (BattleFxPhoenixSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonPhoenixSpawnSpinQuadsTask);
                                        spawn->posX = 0;
                                        spawn->posY = 0xF448;
                                        spawn->posZ = 0xFC18;
                                        spawn = (BattleFxPhoenixSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonPhoenixTrailedModelsTask);
                                        spawn->posX = 0;
                                        spawn->posY = 0xF448;
                                        spawn->posZ = 0x36B0;
                                        spawn->endX = 0;
                                        spawn->endY = 0xF448;
                                        spawn->endZ = 0xE4A8;
                                        spawn = (BattleFxPhoenixSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonPhoenixSpawnBillboardSpritesTask);
                                    }
                                } else {
                                    time -= 0x14;
                                    if (time < 0x2D) {
                                        if (time == 0x2B) {
                                            ringRecord = (BattleFxPhoenixOffset *)Battle_AllocSceneMessage(BATTLE_SCENE_MSG_STOP_SLOT3_ACTOR);
                                        }
                                    } else {
                                        for (i8 = 0; i8 < 0xA; i8++) {
                                            if ((g_BattleSummonPhoenixTargetPos & 1 << i8) != 0) {
                                                Battle_PostActorHitOrHurtMessage(i8);
                                            }
                                        }
                                        g_BattleStageHideFlags = 0;
                                        taskPtr->ownerSlot = BATTLE_TASK_FREE;
                                        if (g_BattleSummonPhoenixParam3 >= 0) {
                                            Battle3d_ReleaseOffscreenSlot(g_BattleSummonPhoenixParam3);
                                            g_BattleSummonPhoenixParam3 = 0xFFFFFFFF;
                                        }
                                        Sfx_ReleaseCachedBuffer(0x1F1, 0xF);
                                        Sfx_ReleaseCachedBuffer(0x1F2, 0xF);
                                        Sfx_ReleaseCachedBuffer(0x1F3, 0xF);
                                        Sfx_ReleaseCachedBuffer(0x1F4, 0xF);
                                        Sfx_ReleaseCachedBuffer(0x1F5, 0xF);
                                        Sfx_ReleaseCachedBuffer(0x1F6, 0xF);
                                        Sfx_ReleaseCachedBuffer(0x1F7, 0xF);
                                        Sfx_ReleaseCachedBuffer(0x1F8, 0xF);
                                        Sfx_ReleaseCachedBuffer(0x1F9, 0xF);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    (taskPtr->frameCounter)++;
}

/* Draws the rising Phoenix emitter sprite and spawns jittered animated child sprites while rising. */
// FUNCTION: FF7 0x00516acf
void __cdecl BattleFx_SummonPhoenixRisingSpriteEmitterTask(void)
{
    BattleFxPhoenixRiseTask *task;
    BattleFxAscentSpriteSlot *pSprite;
    int accumulator;

    task = (BattleFxPhoenixRiseTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSummonPhoenixAnimData2.spriteValue = Gfx_ModelTableGetLoadedRecord(6, g_BattleSummonPhoenixTimer);
    g_BattleSummonPhoenixAnimData2.risingEmitterValue = Gfx_ModelTableGetSpriteObject(6, g_BattleSummonPhoenixTimer);
    BattleFx_SummonPhoenixInstallBillboardMatrix(&task->baseX, 0x400, 0);
    g_BattleSummonPhoenixBufferPtr = BattleFx_SubmitSpriteDraw(&g_BattleSummonPhoenixAnimData2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonPhoenixBufferPtr);
    if (g_BattleIsPaused == 0) {
        accumulator = (task->frame * 0xC0 & 0xFF) + 0xC0;
        while (accumulator >= 0x100) {
            pSprite = (BattleFxAscentSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonPhoenixAnimatedSpriteEntry7Task);
            pSprite->posX = task->baseX + rand() % 0x190 + -0xC8;
            pSprite->posY = task->baseY + rand() % 0x190 + -0xC8;
            pSprite->posZ = task->baseZ + rand() % 0x190 + -0xC8;
            pSprite->scale = 0x1000;
            accumulator -= 0x100;
        }
        (task->frame)++;
        if (task->frame >= 0x14) {
            task->ownerSlot = BATTLE_TASK_FREE;
        } else {
            task->baseY += task->riseSpeed;
        }
    }
}

/* Transforms world positionPtr into camera space, shifts along the view ray by distance, and installs the billboard matrix. */
// FUNCTION: FF7 0x00516c64
short *__cdecl BattleFx_SummonPhoenixInstallBillboardMatrix(void *positionPtr, short rotation, int distance)
{
    int flag;
    int normal[4];

    g_BattleSummonPhoenixAnimBuffer.rotation = rotation;
    g_BattleSummonPhoenixStep1 = g_BattleSummonPhoenixAnimBuffer.rotation;
    g_BattleSummonPhoenixCoordBuffer2[0] = g_BattleSummonPhoenixStep1;
    SetRotMatrix(&g_BattleSummonPhoenixMeshWorkingBlob.billboardMatrix);
    SetTransMatrix(&g_BattleSummonPhoenixMeshWorkingBlob.billboardMatrix);
    RotTransPoint((SVECTOR *)positionPtr, &g_BattleSummonPhoenixAnimBuffer.x, &flag);
    if (distance != 0) {
        NormalizeVectorFixed((VECTOR *)&g_BattleSummonPhoenixAnimBuffer.x, (VECTOR *)normal);
        g_BattleSummonPhoenixAnimBuffer.x += normal[0] * distance >> 0xC;
        g_BattleSummonPhoenixAnimBuffer.y += normal[1] * distance >> 0xC;
        g_BattleSummonPhoenixAnimBuffer.z += normal[2] * distance >> 0xC;
    }
    SetRotMatrix((PsxMatrix *)g_BattleSummonPhoenixCoordBuffer2);
    SetTransMatrix(g_BattleSummonPhoenixCoordBuffer2);
    return g_BattleSummonPhoenixCoordBuffer2;
}

/* Draws animated sprite entry 7 at the slot position and frees the slot after the final frame. */
// FUNCTION: FF7 0x00516d3f
void __cdecl BattleFx_SummonPhoenixAnimatedSpriteEntry7Task(void)
{
    BattleFxPhoenixAnimSlot *task;

    task = (BattleFxPhoenixAnimSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSummonPhoenixAnimData1.frameTable = (struct BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(7, g_BattleSummonPhoenixTimer);
    g_BattleSummonPhoenixAnimData1.tableValue = Gfx_ModelTableGetSpriteObject(7, g_BattleSummonPhoenixTimer);
    BattleFx_SummonPhoenixInstallBillboardMatrix(&task->posX, task->rotation, 0);
    g_BattleSummonPhoenixAnimData1.sourceShort = task->frameIndex;
    g_BattleSummonPhoenixBufferPtr = BattleFx_SubmitSpriteDraw(&g_BattleSummonPhoenixAnimData1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonPhoenixBufferPtr);
    if (g_BattleIsPaused == 0 && g_BattleSummonPhoenixAnimData1.frameTable != 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= g_BattleSummonPhoenixAnimData1.frameTable->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Draws the two counter-spinning Phoenix models with ramping scale and fade. */
// FUNCTION: FF7 0x00516e13
void __cdecl BattleFx_SummonPhoenixCounterSpinModelsTask(void)
{
    BattleFxPhoenixCounterSpinState *scale;
    int frameIndex;
    BattleFxPhoenixCounterSpinScratch *mtx;
    BattleFxPhoenixTrailState *anglesPtr;
    BattleFxCounterSpinState *taskPtr;
    int u;
    PsxMatrix *viewMatrix;

    taskPtr = (BattleFxCounterSpinState *)BATTLE_CURRENT_TASK_SLOT;
    mtx = (BattleFxPhoenixCounterSpinScratch *)Gfx_GetPsxScratchpadPtr(0);
    anglesPtr = &mtx->angles;
    scale = (BattleFxPhoenixCounterSpinState *)((char *)anglesPtr + 8);
    viewMatrix = (PsxMatrix *)((char *)scale + 0x10);
    frameIndex = taskPtr->frameCount;
    if (frameIndex < 0x14) {
        scale->next = (frameIndex << 0xC) / 0x14;
        scale->current = scale->next;
        scale->previous = scale->current;
        g_BattleSummonPhoenixSpriteRequest1.spinAngle = 0;
    } else {
        frameIndex -= 0x14;
        if (frameIndex < 0) {
            scale->next = 0x1000;
            scale->current = 0x1000;
            scale->previous = 0x1000;
            g_BattleSummonPhoenixSpriteRequest1.spinAngle = 0;
        } else {
            frameIndex = frameIndex;
            if (frameIndex < 0x2D) {
                scale->next = (frameIndex << 0xB) / 0x2D + 0x1000;
                scale->current = scale->next;
                scale->previous = scale->current;
                g_BattleSummonPhoenixSpriteRequest1.spinAngle = (frameIndex << 0xC) / 0x2D;
            } else {
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    anglesPtr->rotX = 0;
    anglesPtr->rotY = taskPtr->frameCount << 7;
    anglesPtr->rotZ = 0;
    RotMatrix((SVECTOR *)anglesPtr, &mtx->matrix);
    ScaleMatrix(&mtx->matrix, scale);
    mtx->posX = taskPtr->x;
    mtx->posY = taskPtr->y;
    mtx->posZ = taskPtr->z;
    MultiplyMatrix(&g_BattleSummonPhoenixMeshWorkingBlob.billboardMatrix, &mtx->matrix, viewMatrix);
    SetRotMatrix(viewMatrix);
    SetTransMatrix(viewMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSummonPhoenixSpriteRequest1.spinSpeed = Gfx_ModelTableGetRenderObject(0, g_BattleSummonPhoenixTimer);
    g_BattleSummonPhoenixBufferPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonPhoenixSpriteRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonPhoenixBufferPtr);
    anglesPtr->rotY = taskPtr->frameCount * -0x80;
    RotMatrix((SVECTOR *)anglesPtr, &mtx->matrix);
    ScaleMatrix(&mtx->matrix, scale);
    ConcatMatrixPreservingTranslation(&g_BattleSummonPhoenixMeshWorkingBlob.billboardMatrix, &mtx->matrix, viewMatrix);
    SetRotMatrix(viewMatrix);
    g_BattleSummonPhoenixSpriteRequest1.spinSpeed = Gfx_ModelTableGetRenderObject(1, g_BattleSummonPhoenixTimer);
    g_BattleSummonPhoenixBufferPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonPhoenixSpriteRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonPhoenixBufferPtr);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCount)++;
    }
}

/* Spawns three falling fading sprites per frame on a random XZ ring. */
// FUNCTION: FF7 0x005170be
void __cdecl BattleFx_SummonPhoenixSpawnFallingSpritesTask(void)
{
    int theta, dist;
    BattleFxBurstSpawnerTask *pTaskSlot;
    int i;
    BattleFxPhoenixFallingSprite *pSprite;

    pTaskSlot = (BattleFxBurstSpawnerTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (i = 0; i < 3; i++) {
        pSprite = (BattleFxPhoenixFallingSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonPhoenixFallingFadeSpriteTask);
        pSprite->scale = rand() % 0x600 + 0x200;
        dist = rand() % 0x3E8 + 0x1F4;
        theta = rand() & 0xFFF;
        pSprite->posX = SinFixed(theta) * dist >> 0xC;
        pSprite->posY = 0xFE0C;
        pSprite->posZ = CosFixed(theta) * dist >> 0xC;
        pSprite->velocityY = -(rand() % 0x1F4 + 0x1F4);
    }
    (pTaskSlot->spawnIndex)++;
    if (pTaskSlot->spawnIndex >= 0x2D) {
        pTaskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Draws a falling Phoenix sprite with damped vertical velocity and a five-frame RGB fade-out. */
// FUNCTION: FF7 0x005171e3
void __cdecl BattleFx_SummonPhoenixFallingFadeSpriteTask(void)
{
    int phaseFrame;
    BattleFxPhoenixFallingFadeSprite *task;

    task = (BattleFxPhoenixFallingFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSummonPhoenixSpriteRequest2.spriteValue = Gfx_ModelTableGetLoadedRecord(6, g_BattleSummonPhoenixTimer);
    g_BattleSummonPhoenixSpriteRequest2.fallingTableValue = Gfx_ModelTableGetSpriteObject(6, g_BattleSummonPhoenixTimer);
    phaseFrame = task->frameCount;
    if (phaseFrame < 0xA) {
        g_BattleSummonPhoenixSpriteRequest2.fallingFadeColor = 0x2E808080;
    } else {
        phaseFrame -= 0xA;
        if (phaseFrame < 5) {
            g_BattleSummonPhoenixSpriteRequest2.colorB = phaseFrame * -0x80 / 5 + 0x80;
            g_BattleSummonPhoenixSpriteRequest2.colorG = g_BattleSummonPhoenixSpriteRequest2.colorB;
            g_BattleSummonPhoenixSpriteRequest2.colorR = g_BattleSummonPhoenixSpriteRequest2.colorG;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    BattleFx_SummonPhoenixInstallBillboardMatrix(&task->posX, task->rotation, 0);
    g_BattleSummonPhoenixBufferPtr = BattleFx_SubmitSpriteDraw(&g_BattleSummonPhoenixSpriteRequest2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonPhoenixBufferPtr);
    if (g_BattleIsPaused == 0) {
        (task->frameCount)++;
        task->posY += task->velocityY;
        task->velocityY = task->velocityY * 0xD00 >> 0xC;
    }
}

/* Spawns twenty ring-column tasks in random non-repeating angular slots around the arena. */
// FUNCTION: FF7 0x0051731a
void __cdecl BattleFx_SummonPhoenixSpawnRingColumnsTask(void)
{
    BattleFxPhoenixRingColumn *taskPtr;
    int unusedSpace;
    int i;
    int stackWords;
    BattleFxPhoenixRingColumn *pColumn;

    taskPtr = (BattleFxPhoenixRingColumn *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->stepCount == 0) {
        for (i = 0; i < 0x14; i++) {
            g_BattleSummonPhoenixColumnUsed[i] = 0;
        }
    }
    if ((taskPtr->stepCount % 1) == 0) {
        pColumn = (BattleFxPhoenixRingColumn *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonPhoenixRingColumnTask);
        g_BattleSummonPhoenixStateData1.value = rand() % 0x14;
        while (g_BattleSummonPhoenixColumnUsed[g_BattleSummonPhoenixStateData1.value] != 0) {
            g_BattleSummonPhoenixStateData1.value = (g_BattleSummonPhoenixStateData1.value + 1) % 0x14;
        }
        g_BattleSummonPhoenixColumnUsed[g_BattleSummonPhoenixStateData1.value] = 1;
        g_BattleSummonPhoenixStateData1.value = (g_BattleSummonPhoenixStateData1.value << 0xC) / 0x14 + rand() % 0x99;
        pColumn->x = SinFixed(g_BattleSummonPhoenixStateData1.value) * 0x5DC >> 0xC;
        pColumn->y = 0xFE0C;
        pColumn->z = CosFixed(g_BattleSummonPhoenixStateData1.value) * 0x5DC >> 0xC;
        pColumn->rotX = 0;
        pColumn->angle = g_BattleSummonPhoenixStateData1.angle;
        pColumn->rotZ = 0;
        pColumn->peakHeight = rand() % 0xFA + 0x32;
        pColumn->rotXStep = (rand() % 0x10) + 0x10;
    }
    (taskPtr->stepCount)++;
    if (taskPtr->stepCount >= 0x14) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Draws an eight-segment vertical flat-shaded quad column along the Phoenix ring. */
// FUNCTION: FF7 0x005174dd
void __cdecl BattleFx_SummonPhoenixRingColumnTask(void)
{
    int *screenBuf;
    int spare, phaseFrame;
    BattleFxPhoenixFlatPacket *primitive;
    BattleFxPhoenixRingColumnRecord *task;
    int *flagsBuffer;
    PsxMatrix *matPtr;
    int segmentIndex;
    int height;
    int *depthPointer;
    int mtxB;

    task = (BattleFxPhoenixRingColumnRecord *)BATTLE_CURRENT_TASK_SLOT;
    matPtr = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    mtxB = (int)matPtr + 0x1E;
    depthPointer = (int *)(mtxB + 0x1E);
    screenBuf = (int *)((int)depthPointer + 4);
    flagsBuffer = (int *)((int)screenBuf + 4);
    spare = (int)flagsBuffer + 4;
    phaseFrame = task->spawnIndex;
    if (phaseFrame < 5) {
        height = task->peakHeight * phaseFrame / 5;
    } else {
        phaseFrame -= 5;
        if (phaseFrame < 0x1E) {
            height = task->peakHeight;
        } else {
            phaseFrame -= 0x1E;
            if (phaseFrame < 5) {
                if (g_BattleIsPaused == 0) {
                    task->height -= task->heightStep;
                }
                height = task->peakHeight;
            } else {
                phaseFrame -= 5;
                if (phaseFrame < 5) {
                    if (g_BattleIsPaused == 0) {
                        task->height -= task->heightStep;
                    }
                    height = task->peakHeight + (0 - task->peakHeight) * phaseFrame / 5;
                } else {
                    task->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
    }
    g_BattleSummonPhoenixCoordData3.ringColumnCoord = -height;
    g_BattleSummonPhoenixCoordData1.ringColumnCoord = g_BattleSummonPhoenixCoordData3.ringColumnCoord;
    g_BattleSummonPhoenixCoordData4.ringColumnCoord = height;
    g_BattleSummonPhoenixCoordData2.ringColumnCoord = g_BattleSummonPhoenixCoordData4.ringColumnCoord;
    RotMatrix(&task->angles, matPtr);
    matPtr->t[0] = task->posX;
    matPtr->t[1] = task->posY;
    matPtr->t[2] = task->posZ;
    MultiplyMatrix(&g_BattleSummonPhoenixMeshWorkingBlob.billboardMatrix, matPtr, matPtr);
    SetRotMatrix(matPtr);
    SetTransMatrix(matPtr);
    RefreshGlobalTransformState();
    primitive = (BattleFxPhoenixFlatPacket *)Gfx_GetPsxPacketScratchPtr();
    g_BattleSummonPhoenixCoordData2.ringColumnCoord2 = 0xFC18;
    g_BattleSummonPhoenixCoordData1.ringColumnCoord1 = g_BattleSummonPhoenixCoordData2.ringColumnCoord2;
    g_BattleSummonPhoenixCoordData4.ringColumnCoord4 = 0;
    g_BattleSummonPhoenixCoordData3.ringColumnCoord3 = g_BattleSummonPhoenixCoordData4.ringColumnCoord4;
    for (segmentIndex = 0; segmentIndex < 8; segmentIndex++) {
        *depthPointer = Gfx_TransformQuadReturnDepth(&g_BattleSummonPhoenixCoordData1, (SVECTOR *)&g_BattleSummonPhoenixCoordData2, (SVECTOR *)&g_BattleSummonPhoenixCoordData3, (SVECTOR *)&g_BattleSummonPhoenixCoordData4, screenBuf, flagsBuffer);
        if ((*flagsBuffer & 0x60000) == 0) {
            primitive->tag = 0x5000000;
            primitive->color = 0x2A408080;
            Gfx_DrawPsxPrimitive(primitive);
            primitive++;
        }
        g_BattleSummonPhoenixCoordData1.ringColumnCoord1 += 0xFC18;
        g_BattleSummonPhoenixCoordData2.ringColumnCoord2 += 0xFC18;
        g_BattleSummonPhoenixCoordData3.ringColumnCoord3 += 0xFC18;
        g_BattleSummonPhoenixCoordData4.ringColumnCoord4 += 0xFC18;
    }
    if (g_BattleIsPaused == 0) {
        (task->spawnIndex)++;
    }
}

/* Draws model entry 3 with a growing billboard scale and a fall-hold-rise fade curve. */
// FUNCTION: FF7 0x005177d5
void __cdecl BattleFx_SummonPhoenixGrowingDipModelTask(void)
{
    struct {
        BattleTaskSlot *p;
        int frame;
        int scale;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.p = BATTLE_CURRENT_TASK_SLOT;
    local.frame = local.p->frameCounter;
    if (local.frame < 5) {
        g_BattleSummonPhoenixSpriteRequest3.growingDipValue = local.frame * -0x1000 / 5 + 0x1000;
    } else {
        local.frame -= 5;
        if (local.frame < 5) {
            g_BattleSummonPhoenixSpriteRequest3.growingDipValue = 0;
        } else {
            local.frame -= 5;
            if (local.frame < 5) {
                g_BattleSummonPhoenixSpriteRequest3.growingDipValue = local.frame * 0x1000 / 5;
            } else {
                local.p->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    local.scale = local.p->frameCounter * 0x300 / 0xF + 0x800;
    BattleFx_SummonPhoenixInstallBillboardMatrix(local.p->data, (short)local.scale, -0x3E8);
    g_BattleSummonPhoenixSpriteRequest3.requestValue = Gfx_ModelTableGetRenderObject(3, g_BattleSummonPhoenixTimer);
    g_BattleSummonPhoenixBufferPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonPhoenixSpriteRequest3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonPhoenixBufferPtr);
    if (g_BattleIsPaused == 0) {
        (local.p->frameCounter)++;
    }
}

/* Draws four trailed copies of model 4 and one copy of model 5 sweeping across Z. */
// FUNCTION: FF7 0x005178f9
void __cdecl BattleFx_SummonPhoenixTrailedModelsTask(void)
{
    int phaseFrame;
    BattleFxPhoenixTrailModel *task;
    BattleFxSuperNovaCameraDelta *delta;
    PsxMatrix *finalMatrix;
    PsxMatrix *matrix;
    int copyIndex;
    BattleFxPhoenixTrailState *rotation;
    PsxMatrix *baseMatrix;

    task = (BattleFxPhoenixTrailModel *)BATTLE_CURRENT_TASK_SLOT;
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    finalMatrix = (PsxMatrix *)((int)matrix + 0x1E);
    baseMatrix = (PsxMatrix *)((int)finalMatrix + 0x1E);
    delta = (BattleFxSuperNovaCameraDelta *)((int)baseMatrix + 0x1E);
    rotation = (BattleFxPhoenixTrailState *)((int)delta + 0x10);
    phaseFrame = task->frame;
    if (phaseFrame < 0x32) {
        g_BattleSummonPhoenixSpriteRequest5.trailSpriteCurrent = 0;
        g_BattleSummonPhoenixSpriteRequest4.trailSpritePrev = g_BattleSummonPhoenixSpriteRequest5.trailSpriteCurrent;
    } else {
        phaseFrame -= 0x32;
        if (phaseFrame < 0xF) {
            g_BattleSummonPhoenixSpriteRequest5.trailSpriteCurrent = (phaseFrame << 0xC) / 0xF;
            g_BattleSummonPhoenixSpriteRequest4.trailSpritePrev = g_BattleSummonPhoenixSpriteRequest5.trailSpriteCurrent;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    RotMatrix((SVECTOR *)&g_BattleSummonPhoenixCoordData5, baseMatrix);
    baseMatrix->t[0] = 0;
    baseMatrix->t[1] = 0;
    baseMatrix->t[2] = 0;
    MultiplyMatrix(&g_BattleSummonPhoenixMeshWorkingBlob.billboardMatrix, baseMatrix, baseMatrix);
    rotation->rotX = 0;
    rotation->rotY = 0x800;
    rotation->rotZ = task->frame << 5;
    RotMatrixXYZ(rotation, matrix);
    matrix->t[0] = task->startX;
    matrix->t[1] = task->startY;
    matrix->t[2] = task->startZ;
    ScaleMatrix(matrix, &g_BattleSummonPhoenixScale1);
    MultiplyMatrix(baseMatrix, matrix, finalMatrix);
    SetRotMatrix(finalMatrix);
    SetTransMatrix(finalMatrix);
    RotateSVectorByInstalledMatrix((SVECTOR *)g_BattleSummonPhoenixCoordData6, (int *)delta);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSummonPhoenixSpriteRequest4.trailValue = Gfx_ModelTableGetRenderObject(4, g_BattleSummonPhoenixTimer);
    g_BattleSummonPhoenixBufferPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonPhoenixSpriteRequest4, g_BattleCurrentOrderingTable + 0xB18, 0, g_BattleSummonPhoenixBufferPtr);
    for (copyIndex = 1; copyIndex < 4; copyIndex++) {
        finalMatrix->t[0] += delta->deltaX;
        finalMatrix->t[1] += delta->deltaY;
        finalMatrix->t[2] += delta->deltaZ;
        SetTransMatrix(finalMatrix);
        g_BattleSummonPhoenixBufferPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonPhoenixSpriteRequest4, g_BattleCurrentOrderingTable + 0xB18, 0, g_BattleSummonPhoenixBufferPtr);
    }
    rotation->rotZ = task->frame << 5;
    RotMatrixXYZ(rotation, matrix);
    matrix->t[0] = task->endX;
    matrix->t[1] = task->endY;
    matrix->t[2] = task->endZ;
    ScaleMatrix(matrix, &g_BattleSummonPhoenixScale1);
    MultiplyMatrix(baseMatrix, matrix, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    g_BattleSummonPhoenixSpriteRequest5.trailValue = Gfx_ModelTableGetRenderObject(5, g_BattleSummonPhoenixTimer);
    g_BattleSummonPhoenixBufferPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonPhoenixSpriteRequest5, g_BattleCurrentOrderingTable + 0xB18, 0, g_BattleSummonPhoenixBufferPtr);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
    }
    task->startZ += 0x3E8;
    if (task->startZ >= 0x3A98) {
        task->startZ -= 0x2400;
    }
}

/* Emits spinning Gouraud triangle rays from the task position each frame. */
// FUNCTION: FF7 0x00517c7a
void __cdecl BattleFx_SummonPhoenixSpawnSpinQuadsTask(void)
{
    int emitAccum;
    BattleFxPhoenixSpinQuad *task;
    BattleFxPhoenixSpinQuad *child;

    task = (BattleFxPhoenixSpinQuad *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    emitAccum = (task->spawnIndex << 8 & 0xFF) + 0x100;
    while (emitAccum >= 0x100) {
        child = (BattleFxPhoenixSpinQuad *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonPhoenixSpinQuadTask);
        child->pos = task->pos;
        child->rotX = 0;
        child->rotY = 0;
        g_BattleSummonPhoenixStateData2.value = g_BattleSummonPhoenixStateData2.value + (rand() & 0x7FF) + 0x400 & 0xFFF;
        child->rotZ = g_BattleSummonPhoenixStateData2.angle;
        child->timer = rand() % 0xC8 + 0x64;
        child->spinSpeed = rand() % 0x30 + 0x10;
        if ((rand() & 1) != 0) {
            child->spinSpeed = -child->spinSpeed;
        }
        emitAccum -= 0x100;
    }
    (task->spawnIndex)++;
    if (task->spawnIndex >= 0x36) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Projects a spin-ray origin into screen space and draws a spinning Gouraud triangle ray. */
// FUNCTION: FF7 0x00517dbb
void __cdecl BattleFx_SummonPhoenixSpinQuadTask(void)
{
    BattleFxPhoenixSpinQuadTask *taskPtr;
    int time;
    BattleFxTimeDamageOrbitRing *pScreenPos;
    int *screenBuffer;
    int *bitsPtr;
    BattleFxPsxGouraudTriPacket *prim;
    int halfSize;
    int *offsetX;
    struct BattleFxOffsetTriMeshDesc *matrix;
    int *offsetY;
    int *depthPointer;

    taskPtr = (BattleFxPhoenixSpinQuadTask *)BATTLE_CURRENT_TASK_SLOT;
    matrix = (struct BattleFxOffsetTriMeshDesc *)Gfx_GetPsxScratchpadPtr(0);
    depthPointer = (int *)((int)matrix + 0x1E);
    screenBuffer = (int *)((int)depthPointer + 4);
    bitsPtr = (int *)((int)screenBuffer + 4);
    pScreenPos = (BattleFxTimeDamageOrbitRing *)((int)bitsPtr + 4);
    offsetX = (int *)((int)pScreenPos + 4);
    offsetY = (int *)((int)offsetX + 4);
    time = taskPtr->frame;
    if (time < 5) {
        halfSize = taskPtr->size * time / 5;
    } else {
        time -= 5;
        if (time < 1) {
            halfSize = taskPtr->size;
        } else {
            time--;
            if (time < 5) {
                halfSize = taskPtr->size + (0 - taskPtr->size) * time / 5;
            } else {
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleSummonPhoenixCoordData7[0] = -halfSize;
    g_BattleSummonPhoenixSpriteRequest6.vertexBX = halfSize;
    SetRotMatrix(&g_BattleSummonPhoenixMeshWorkingBlob.billboardMatrix);
    SetTransMatrix(&g_BattleSummonPhoenixMeshWorkingBlob.billboardMatrix);
    *depthPointer = RotTransPers(&taskPtr->pos, (short *)pScreenPos, screenBuffer, bitsPtr);
    GetGeomOffset(offsetX, offsetY);
    RotMatrix(&taskPtr->rot, (PsxMatrix *)matrix);
    matrix->transX = pScreenPos->screenX - *offsetX << 3;
    matrix->transY = pScreenPos->screenY - *offsetY << 3;
    matrix->transZ = GetGeomScreen() << 3;
    SetRotMatrix((PsxMatrix *)matrix);
    SetTransMatrix(matrix);
    RefreshGlobalTransformState();
    prim = (BattleFxPsxGouraudTriPacket *)Gfx_GetPsxPacketScratchPtr();
    Gfx_TransformTriangleReturnDepth(g_BattleSummonPhoenixCoordData7, (SVECTOR *)&g_BattleSummonPhoenixSpriteRequest6, &g_BattleSummonPhoenixSpriteRequest6.vertexC, screenBuffer, bitsPtr);
    if ((*bitsPtr & 0x60000) == 0) {
        prim->tag = 0x6000000;
        prim->colorWord1 = 0x32001010;
        prim->colorWord0 = 0x32001010;
        prim->colorWord2 = 0x32FFFFFF;
        Gfx_DrawPsxPrimitive(prim);
    }
    if (g_BattleIsPaused == 0) {
        (taskPtr->frame)++;
        taskPtr->angle += taskPtr->spinSpeed;
    }
}

/* Spawns two billboard sprites per frame on an XY circle of radius 1800. */
// FUNCTION: FF7 0x00518006
void __cdecl BattleFx_SummonPhoenixSpawnBillboardSpritesTask(void)
{
    int angle;
    int padWord;
    BattleFxBurstSpawnerTask *task;
    BattleFxPhoenixBillboardSprite *pSprite;
    int accumulator;
    int bytes;

    task = (BattleFxBurstSpawnerTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    accumulator = (task->spawnIndex << 9 & 0xFF) + 0x200;
    while (accumulator >= 0x100) {
        pSprite = (BattleFxPhoenixBillboardSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonPhoenixBillboardSpriteTask);
        angle = rand() & 0xFFF;
        pSprite->posX = SinFixed(angle) * 0x708 >> 0xC;
        pSprite->posY = CosFixed(angle) * 0x708 >> 0xC;
        pSprite->forwardPos = 0;
        pSprite->forwardStep = rand() % 0xC8 + 0x12C;
        if (DEAD_IN_RETAIL != 0) {
            bytes = 0x3000;
        } else {
            bytes = (rand() % 0x2000) + 0x1000;
        }
        pSprite->scale = bytes;
        accumulator -= 0x100;
    }
    (task->spawnIndex)++;
    if (task->spawnIndex >= 0x23) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Draws billboard sprite entry 8 advancing along its local forward axis. */
// FUNCTION: FF7 0x00518147
void __cdecl BattleFx_SummonPhoenixBillboardSpriteTask(void)
{
    BattleFxPhoenixBillboardSlot *task;

    task = (BattleFxPhoenixBillboardSlot *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonPhoenixFlag1 & 1) == 0) {
        g_BattleSummonPhoenixFlag1 |= 1;
        g_BattleSummonPhoenixState = (BattleFxPhoenixBillboardState *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonPhoenixAnimData3.spriteValue = Gfx_ModelTableGetLoadedRecord(8, g_BattleSummonPhoenixTimer);
    g_BattleSummonPhoenixAnimData3.billboardValue = Gfx_ModelTableGetSpriteObject(8, g_BattleSummonPhoenixTimer);
    RotMatrix((SVECTOR *)&g_BattleSummonPhoenixCoordData8, &g_BattleSummonPhoenixState->matrix);
    g_BattleSummonPhoenixState->matrix.t[0] = 0;
    g_BattleSummonPhoenixState->matrix.t[1] = 0xFFFFF448;
    g_BattleSummonPhoenixState->matrix.t[2] = 0;
    SetRotMatrix(&g_BattleSummonPhoenixState->matrix);
    SetTransMatrix(&g_BattleSummonPhoenixState->matrix);
    RotTransPoint(&task->pos, g_BattleSummonPhoenixState->posWords, &g_BattleSummonPhoenixState->rotTransFlag);
    g_BattleSummonPhoenixState->prevX = g_BattleSummonPhoenixState->posX;
    g_BattleSummonPhoenixState->prevY = g_BattleSummonPhoenixState->posY;
    g_BattleSummonPhoenixState->prevZ = g_BattleSummonPhoenixState->posZ;
    BattleFx_SummonPhoenixInstallBillboardMatrix(g_BattleSummonPhoenixState, task->rotation, 0);
    g_BattleSummonPhoenixBufferPtr = BattleFx_SubmitSpriteDraw(&g_BattleSummonPhoenixAnimData3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonPhoenixBufferPtr);
    if (g_BattleIsPaused == 0) {
        (task->frameCount)++;
        if (task->frameCount >= 0x1E) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
        task->value += task->step;
    }
}

/* Queues an offscreen capture request of battle stage part 1 for the ring grid wave texture. */
// FUNCTION: FF7 0x005182f7
void __cdecl BattleFx_SummonPhoenixQueueStageOffscreenRequest(Vec3sPacked *pos, BattleSummonPhoenixAngleData1 *shortVec)
{
    struct {
        BattlePhoenixBlock64 matrix;
        int position[3];
        int actor;
        PsxMatrix attach;
        unsigned char attachPad[2];
        int mesh;
        BattleSummonPhoenixRequest *request;
        PsxMatrix *matrix_source;
        int unused;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (g_BattleSummonPhoenixCoordBuffer1.rotY == 0) {
        ((PsxMatrix *)&g_BattleSummonPhoenixTransformMatrix1)->t[0] = -pos->vx - g_BattleSummonPhoenixTransformMatrix3.offsetX;
        ((PsxMatrix *)&g_BattleSummonPhoenixTransformMatrix1)->t[1] = -pos->vz - g_BattleSummonPhoenixTransformMatrix3.offsetZ;
        ((PsxMatrix *)&g_BattleSummonPhoenixTransformMatrix1)->t[2] = pos->vy;
        frame.matrix_source = (PsxMatrix *)g_BattleSummonPhoenixTransformMatrix1;
    } else {
        ((PsxMatrix *)&g_BattleSummonPhoenixTransformMatrix2)->t[0] = pos->vx - g_BattleSummonPhoenixTransformMatrix3.offsetX;
        ((PsxMatrix *)&g_BattleSummonPhoenixTransformMatrix2)->t[1] = pos->vz - g_BattleSummonPhoenixTransformMatrix3.offsetZ;
        ((PsxMatrix *)&g_BattleSummonPhoenixTransformMatrix2)->t[2] = pos->vy;
        frame.matrix_source = (PsxMatrix *)g_BattleSummonPhoenixTransformMatrix2;
    }
    SetRotMatrix(frame.matrix_source);
    SetTransMatrix(frame.matrix_source);
    if (g_pBattle3dScene != 0 && g_pBattle3dScene->stageId != 0) {
        frame.mesh = g_pBattle3dScene->stageId;
        frame.request = (BattleSummonPhoenixRequest *)Battle3d_AcquireOffscreenRequest();
        if (frame.request != 0) {
            frame.request->state = GFX_OFFSCREEN_REQUEST_MESH;
            frame.request->partMesh = (int)BattleStage_GetPartMesh(1, (struct Battle3dStageRecord *)frame.mesh);
            GtePsxMatrixToFloat4x4Transposed(frame.matrix_source, frame.request->worldMatrixFloats);
            if (DEAD_IN_RETAIL != 0) {
                frame.actor = 4;
                frame.request->state = GFX_OFFSCREEN_REQUEST_SKELETON;
                frame.request->animBank = g_BattleActorRecords[frame.actor].animBank;
                frame.request->animFramePtr = ((int *)(g_BattleActorRecords[frame.actor].model->frameTable))[g_BattleActorRecords[frame.actor].animFrame];
                frame.request->modelParts = g_BattleActorRecords[frame.actor].model->modelParts;
                Battle3d_GetActorAttachMatrix(frame.actor, g_BattleActorRecords[frame.actor].targetBone, &frame.attach);
                frame.position[0] = 0;
                frame.position[1] = FLOAT_625_BITS;
                frame.position[2] = FLOAT_1875_BITS;
                BuildTranslationMatrixRowVec(frame.position, (void *)frame.matrix.words);
                frame.request->worldMatrix = frame.matrix;
            }
            frame.request->colorKeyEnabled = 0;
            frame.request->otDepthShift = 0xC;
            frame.request->blendMode = BLEND_ADDITIVE;
            frame.request->blendMode = BLEND_NONE;
            frame.request->viewportX = shortVec->spriteX;
            frame.request->viewportY = shortVec->spriteY;
            frame.request->viewportW = shortVec->spriteW;
            frame.request->viewportH = shortVec->spriteH;
            frame.request->sampleStepX = 1;
            frame.request->sampleStepY = 1;
            g_BattleSummonPhoenixParam3 = frame.request->offscreenSlot;
        }
    }
}

/* Initializes the 32x16 ring grid quad packets and 33x16 polar vertex positions. */
// FUNCTION: FF7 0x00518571
void __cdecl BattleFx_SummonPhoenixBuildRingGridMesh(void)
{
    int radiusVal, dstOuterRadius, row;
    BattleFxPhoenixRingGridQuad *pRingQuad;
    int column, a0, vertexIndex, baseAngleB, bound, rowBytes;

    for (row = 0; row < 0x20; row++) {
        radiusVal = row * 3;
        if (radiusVal + 3 < 0x60) {
            bound = radiusVal + 3;
        } else {
            bound = 0x5F;
        }
        dstOuterRadius = bound;
        for (column = 0; column < 0x10; column++) {
            pRingQuad = &g_BattleSummonPhoenixSourceMeshAndPoseBlob[row][column];
            pRingQuad->vertexA = &g_BattleSummonPhoenixMeshWorkingBlob.ringGrid[row][column];
            pRingQuad->vertexB = &g_BattleSummonPhoenixMeshWorkingBlob.ringGrid[row][(column + 1) % 0x10];
            pRingQuad->vertexC = &g_BattleSummonPhoenixMeshWorkingBlob.ringGrid[row + 1][column];
            pRingQuad->vertexD = &g_BattleSummonPhoenixMeshWorkingBlob.ringGrid[row + 1][(column + 1) % 0x10];
            a0 = (column << 0xC) / 0x10;
            baseAngleB = ((column + 1) << 0xC) / 0x10;
            for (vertexIndex = 0; vertexIndex < 1; vertexIndex++) {
                pRingQuad->poly[vertexIndex].tag = 0x9000000;
                pRingQuad->poly[vertexIndex].color = 0x2C808080;
                pRingQuad->poly[vertexIndex].u0 = (radiusVal * SinFixed(a0) >> 0xC) + 0x60;
                pRingQuad->poly[vertexIndex].v0 = (radiusVal * CosFixed(a0) >> 0xC) + 0x60;
                pRingQuad->poly[vertexIndex].u1 = (radiusVal * SinFixed(baseAngleB) >> 0xC) + 0x60;
                pRingQuad->poly[vertexIndex].v1 = (radiusVal * CosFixed(baseAngleB) >> 0xC) + 0x60;
                pRingQuad->poly[vertexIndex].u2 = (dstOuterRadius * SinFixed(a0) >> 0xC) + 0x60;
                pRingQuad->poly[vertexIndex].v2 = (dstOuterRadius * CosFixed(a0) >> 0xC) + 0x60;
                pRingQuad->poly[vertexIndex].u3 = (dstOuterRadius * SinFixed(baseAngleB) >> 0xC) + 0x60;
                pRingQuad->poly[vertexIndex].v3 = (dstOuterRadius * CosFixed(baseAngleB) >> 0xC) + 0x60;
                pRingQuad->poly[vertexIndex].texPage = 0x14C;
            }
        }
    }
    for (row = 0; row < 0x21; row++) {
        if (row < 0x20) {
            rowBytes = row * 3 * 0x18;
        } else {
            rowBytes = 0x8E8;
        }
        radiusVal = rowBytes;
        for (column = 0; column < 0x10; column++) {
            a0 = (column << 0xC) / 0x10;
            g_BattleSummonPhoenixMeshWorkingBlob.ringGrid[row][column].vx = radiusVal * SinFixed(a0) >> 0xC;
            g_BattleSummonPhoenixMeshWorkingBlob.ringGrid[row][column].vy = 0;
            g_BattleSummonPhoenixMeshWorkingBlob.ringGrid[row][column].vz = -radiusVal * CosFixed(a0) >> 0xC;
        }
    }
}

/* Advances the outward height wave on the ring grid and submits all 512 textured quads. */
// FUNCTION: FF7 0x005188f3
void __cdecl BattleFx_SummonPhoenixRingGridWaveTask(void)
{
    int zA;
    BattleFxPhoenixRingGridQuad *quadPtr;
    int row;
    int stateOut;
    BattleTaskSlot *taskPtr;
    int time;
    int depthResult;
    int column;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    time = taskPtr->frameCounter;
    if (time < 0x14) { /* empty first arm: lib/codegen.h */
    } else {
        time -= 0x14;
        if (time < 0x18) {
            BattleFx_SummonPhoenixSetRingGridRowHeights(time << 1, 0x14, -(time * -150 / 0x18 + 0x96));
        } else {
            if (time == 0x18) {
                (taskPtr->frameCounter)++;
                return;
            }
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    SetRotMatrix(&g_BattleSummonPhoenixMeshWorkingBlob.billboardMatrix);
    SetTransMatrix(&g_BattleSummonPhoenixMeshWorkingBlob.billboardMatrix);
    g_BattleSummonPhoenixActiveCount = 0;
    RefreshGlobalTransformState();
    Gfx_SetPsxPrimQuadState(GFX_PSX_PRIM_POLY_FT4, BLEND_NONE, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleSummonPhoenixParam3));
    for (row = 0; row < 0x20; row++) {
        for (column = 0; column < 0x10; column++) {
            quadPtr = &g_BattleSummonPhoenixSourceMeshAndPoseBlob[row][column];
            depthResult = Gfx_TransformQuadReturnDepth((void *)quadPtr->vertexA, quadPtr->vertexB, quadPtr->vertexC, quadPtr->vertexD, &zA, &stateOut);
            Gfx_DrawPsxPrimitive(quadPtr->poly);
        }
    }
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
    }
}

/* Sets ring-grid row heights to rowHeight at rows spaced every 8 behind centerRow within range. */
// FUNCTION: FF7 0x00518a74
void __cdecl BattleFx_SummonPhoenixSetRingGridRowHeights(int centerRow, int range, int rowHeight)
{
    int value, row, column;

    for (row = 0; row < 0x21; row++) {
        if (row <= centerRow && centerRow - row < range && (centerRow - row) % 8 == 0) {
            value = rowHeight;
        } else {
            value = 0;
        }
        for (column = 0; column < 0x10; column++) {
            g_BattleSummonPhoenixMeshWorkingBlob.ringGrid[row][column].vy = value;
        }
    }
}

/* Advances the summon actor in slot 3 upward along the summon orientation matrix over 45 frames. */
// FUNCTION: FF7 0x00518aff
void __cdecl BattleFx_SummonPhoenixMoveSummonActorTask(void)
{
    BattleTaskSlot *rec;
    int c;

    rec = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonPhoenixFlag3 & 1) == 0) {
        g_BattleSummonPhoenixFlag3 |= 1;
        g_BattleSummonPhoenixBuffer1 = (BattleFxPhoenixRotatePos *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    c = rec->frameCounter;
    if (c < 0x2D) {
        g_BattleSummonPhoenixTransformData1.transformOffset = c * -0x5DC / 0x2D - 0x3E8;
    } else {
        rec->ownerSlot = BATTLE_TASK_FREE;
    }
    rec->frameCounter += 1;
    SetRotMatrix(&g_BattleSummonPhoenixTransformMatrix3.matrix);
    SetTransMatrix(&g_BattleSummonPhoenixTransformMatrix3.matrix);
    RotTransPoint((SVECTOR *)&g_BattleSummonPhoenixTransformData1, g_BattleSummonPhoenixBuffer1->outVec, &g_BattleSummonPhoenixBuffer1->flag);
    g_BattleActorRecords[3].posX = g_BattleSummonPhoenixBuffer1->posX;
    g_BattleActorRecords[3].posY = g_BattleSummonPhoenixBuffer1->posY;
    g_BattleActorRecords[3].posZ = g_BattleSummonPhoenixBuffer1->posZ;
}

/* Draws growing Phoenix model entry 2 offset along the normalized camera ray. */
// FUNCTION: FF7 0x00518c07
void __cdecl BattleFx_SummonPhoenixGrowingModelTask(void)
{
    int phaseFrame;
    int stackWords;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonPhoenixFlag2 & 1) == 0) {
        g_BattleSummonPhoenixFlag2 |= 1;
        g_BattleSummonPhoenixLoadedResTable = (struct BattleFxPhoenixGrowState *)Gfx_GetPsxScratchpadPtr(0);
    }
    phaseFrame = task->frameCounter;
    if (phaseFrame < 0x32) {
        g_BattleSummonPhoenixSpriteRequest7.growingStepAngle = 0;
    } else {
        phaseFrame -= 0x32;
        if (phaseFrame < 0xA) {
            g_BattleSummonPhoenixSpriteRequest7.growingStepAngle = (phaseFrame << 0xC) / 0xA;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    g_BattleSummonPhoenixBuffer3.growStep = (task->frameCounter << 0xC) / 0x3C + 0x800;
    g_BattleSummonPhoenixStep2 = g_BattleSummonPhoenixBuffer3.growStep;
    SetRotMatrix(&g_BattleSummonPhoenixMeshWorkingBlob.billboardMatrix);
    SetTransMatrix(&g_BattleSummonPhoenixMeshWorkingBlob.billboardMatrix);
    g_BattleSummonPhoenixLoadedResTable->pointX = g_BattleSummonPhoenixTransformData1.posX;
    g_BattleSummonPhoenixLoadedResTable->transformOffset = g_BattleSummonPhoenixTransformData1.transformOffset - 0x12C;
    g_BattleSummonPhoenixLoadedResTable->pointZ = g_BattleSummonPhoenixTransformData1.posZ;
    RotTransPoint(&g_BattleSummonPhoenixLoadedResTable->point, &g_BattleSummonPhoenixLoadedResTable->posX, &g_BattleSummonPhoenixLoadedResTable->rotTransFlag);
    NormalizeVectorFixed(&g_BattleSummonPhoenixLoadedResTable->pos, &g_BattleSummonPhoenixLoadedResTable->velocity);
    g_BattleSummonPhoenixBuffer3.growingPosX = g_BattleSummonPhoenixLoadedResTable->posX + (g_BattleSummonPhoenixLoadedResTable->velocityX * 0x3E8 >> 0xC);
    g_BattleSummonPhoenixBuffer3.growingPosY = g_BattleSummonPhoenixLoadedResTable->posY + (g_BattleSummonPhoenixLoadedResTable->velocityY * 0x3E8 >> 0xC);
    g_BattleSummonPhoenixBuffer3.growingPosZ = g_BattleSummonPhoenixLoadedResTable->posZ + (g_BattleSummonPhoenixLoadedResTable->velocityZ * 0x3E8 >> 0xC);
    SetRotMatrix((PsxMatrix *)&g_BattleSummonPhoenixStep2);
    SetTransMatrix(&g_BattleSummonPhoenixStep2);
    g_BattleSummonPhoenixSpriteRequest7.growingValue = Gfx_ModelTableGetRenderObject(2, g_BattleSummonPhoenixTimer);
    g_BattleSummonPhoenixBufferPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonPhoenixSpriteRequest7, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonPhoenixBufferPtr);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/* Draws and integrates the 100-particle damped burst sprite pool. */
// FUNCTION: FF7 0x00518e2d
void __cdecl BattleFx_SummonPhoenixDampedSpritePoolTask(void)
{
    int scale, time;
    BattleFxBurstSpawnerTask *taskSlot;
    int i, ratio;

    taskSlot = (BattleFxBurstSpawnerTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSummonPhoenixSpriteRequest8.spriteValue = Gfx_ModelTableGetLoadedRecord(6, g_BattleSummonPhoenixTimer);
    g_BattleSummonPhoenixSpriteRequest8.dampedTableValue = Gfx_ModelTableGetSpriteObject(6, g_BattleSummonPhoenixTimer);
    time = taskSlot->spawnIndex;
    if (time < 5) {
        ratio = (time << 0xC) / 5;
        scale = ratio;
        g_BattleSummonPhoenixSpriteRequest8.dampedPoolColor = 0x2E808080;
    } else {
        time -= 5;
        if (time < 0x14) {
            scale = 0x1000;
            g_BattleSummonPhoenixSpriteRequest8.dampedPoolColor = 0x2E808080;
        } else {
            time -= 0x14;
            if (time < 5) {
                ratio = (time << 0xC) / 5;
                scale = 0x1000;
                g_BattleSummonPhoenixSpriteRequest8.colorB = (ratio * -0x80 >> 0xC) + 0x80;
                g_BattleSummonPhoenixSpriteRequest8.colorG = g_BattleSummonPhoenixSpriteRequest8.colorB;
                g_BattleSummonPhoenixSpriteRequest8.colorR = g_BattleSummonPhoenixSpriteRequest8.colorG;
            } else {
                taskSlot->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    for (i = 0; i < 0x64; i++) {
        BattleFx_SummonPhoenixInstallBillboardMatrix(&g_BattleSummonPhoenixBufferPtrTable[0][i], g_BattleSummonPhoenixBufferPtrTable[0][i].spriteX * scale >> 0xC, 0);
        g_BattleSummonPhoenixBufferPtr = BattleFx_SubmitSpriteDraw(&g_BattleSummonPhoenixSpriteRequest8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonPhoenixBufferPtr);
        if (g_BattleIsPaused == 0) {
            g_BattleSummonPhoenixBufferPtrTable[0][i].posX += g_BattleSummonPhoenixBufferPtrTable[0][i].velocityX;
            g_BattleSummonPhoenixBufferPtrTable[0][i].posY += g_BattleSummonPhoenixBufferPtrTable[0][i].velocityY;
            g_BattleSummonPhoenixBufferPtrTable[0][i].posZ += g_BattleSummonPhoenixBufferPtrTable[0][i].velocityZ;
            g_BattleSummonPhoenixBufferPtrTable[0][i].velocityX = g_BattleSummonPhoenixBufferPtrTable[0][i].velocityX * 0xF00 >> 0xC;
            g_BattleSummonPhoenixBufferPtrTable[0][i].velocityY = g_BattleSummonPhoenixBufferPtrTable[0][i].velocityY * 0xF00 >> 0xC;
            g_BattleSummonPhoenixBufferPtrTable[0][i].velocityZ = g_BattleSummonPhoenixBufferPtrTable[0][i].velocityZ * 0xF00 >> 0xC;
        }
    }
    if (g_BattleIsPaused == 0) {
        (taskSlot->spawnIndex)++;
    }
}
} /* extern "C" */
