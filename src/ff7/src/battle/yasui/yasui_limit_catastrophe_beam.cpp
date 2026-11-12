// src/ff7/src/battle/yasui/yasui_limit_catastrophe_beam.cpp
// Limit Break [13] (Barret Lv 4): Catastrophe (Japanese: カタストロフィ, dev tag: "LIMIT/CATASTRO" / Catastro)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Catastrophe limit break effect for Barret: he jumps, a beam sweeps between the targets
//   in battle slot order with homing endpoints, and smoke sprites, flashes and jitter accompany the
//   discharge.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "ff7/enums/battle.h"
extern "C" {

struct BattleOrthoMatrix;
struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x007ed688
unsigned char g_YasuiLimitCatastropheSoundGroup1[16] = {0x8c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007ed698
unsigned char g_YasuiLimitCatastropheSoundGroup2[16] = {0x82, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007ed6a8
unsigned char g_YasuiLimitCatastropheSoundGroup3[16] = {0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007ed6b8
unsigned int g_YasuiLimitCatastropheSoundGroups[4] = {(unsigned int)&g_YasuiLimitCatastropheSoundGroup1, (unsigned int)&g_YasuiLimitCatastropheSoundGroup2, (unsigned int)&g_YasuiLimitCatastropheSoundGroup3, 0x00000000};

// GLOBAL: FF7 0x007ed6c8
unsigned int g_YasuiLimitCatastropheResourceDef[30] = {0x00c800b1, (unsigned int)"BEAM_1.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)"BEAM_2.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)"BEAM_3A.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"SMOKE_1.TIM", (unsigned int)"SMOKE_1.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"HIT_1.TIM", (unsigned int)"HIT_1.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"SMOKE_1.TIM", (unsigned int)"SMOKE_2.SPT", 0x00000000, 0x00000000};

// GLOBAL: FF7 0x007ed740
unsigned char g_YasuiLimitCatastropheCameraScript[8] = {0x86, 0x00, 0x00, 0x00, 0xff, 0xff, 0x46, 0x00};

unsigned char g_rawData_007ed748[184] = {
    0x00, 0x00, 0xff, 0xff, 0x81, 0x00, 0x00, 0x00, 0xe0, 0xfc, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0xe0, 0xfc, 0xe8, 0x03, 0x80, 0x00, 0x0a, 0x00,
    0x40, 0x00, 0x0a, 0x00, 0x85, 0x00, 0x2b, 0x00, 0xff, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x45, 0x00, 0x2b, 0x00, 0xff, 0x04, 0x00, 0x00,
    0xe0, 0xfc, 0x40, 0x1f, 0x82, 0x00, 0x18, 0x00, 0x00, 0x00, 0x44, 0xfd, 0x00, 0x00, 0x42, 0x00, 0x18, 0x00, 0x2c, 0x01, 0x50, 0xfb, 0xe8, 0x03,
    0x80, 0x00, 0x05, 0x00, 0x40, 0x00, 0x05, 0x00, 0x82, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x45, 0x00, 0x0f, 0x00, 0xff, 0x01,
    0x40, 0x1f, 0x30, 0xf8, 0x40, 0x1f, 0x87, 0x00, 0x47, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xfc, 0x41, 0x00, 0x78, 0xec, 0xc0, 0xe0,
    0xc0, 0xe0, 0x80, 0x00, 0x64, 0x00, 0x43, 0x00, 0x64, 0x00, 0x00, 0x00, 0x68, 0xc5, 0xf0, 0xd8, 0x86, 0x00, 0x00, 0x00, 0xff, 0xff, 0x46, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x48, 0xf4, 0x0c, 0xfe, 0xb8, 0x0b, 0x80, 0x00, 0x14, 0x00,
    0x42, 0x00, 0x14, 0x00, 0x48, 0xf4, 0x30, 0xf8, 0xb8, 0x0b, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00
};

// GLOBAL: FF7 0x007ed800
struct YasuiLimitCatastropheModelDrawReq1 g_YasuiLimitCatastropheModelDrawReq1 = {{0, 0, 0, 0, 0xa8, 0, 0, 0, 0, 0}, 0x200000, {0}};

// GLOBAL: FF7 0x007ed810
struct YasuiLimitCatastropheSpriteDrawReq1 g_YasuiLimitCatastropheSpriteDrawReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};

// GLOBAL: FF7 0x007ed820
struct YasuiLimitCatastropheSpriteDrawReq2 g_YasuiLimitCatastropheSpriteDrawReq2 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};

// GLOBAL: FF7 0x007ed830
struct YasuiLimitCatastropheModelDrawReq2 g_YasuiLimitCatastropheModelDrawReq2 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};

// GLOBAL: FF7 0x007ed840
unsigned char g_YasuiLimitCatastropheTransformData1[30] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6a, 0xff, 0xff, 0xff};

// GLOBAL: FF7 0x007ed860
unsigned char g_YasuiLimitCatastropheModelDrawReq3[16] = {0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007ed870
unsigned char g_YasuiLimitCatastropheTransformData2[30] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6a, 0xff, 0xff, 0xff};

// GLOBAL: FF7 0x007ed890
struct YasuiLimitCatastropheScaleVector g_YasuiLimitCatastropheScaleVector = {3328, 3328, {0, 0xd, 0, 0}};

// GLOBAL: FF7 0x007ed8a0
struct YasuiLimitCatastropheSpriteDrawReq3 g_YasuiLimitCatastropheSpriteDrawReq3 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};

// GLOBAL: FF7 0x007ed8b0
unsigned char g_YasuiLimitCatastropheOffsetVector[6] = {0x00, 0x00, 0x00, 0x00, 0x6a, 0xff};

// GLOBAL: FF7 0x00a41ed8
YasuiCatastroEndpoint g_YasuiLimitCatastropheBeamEndpoint = {0};

// GLOBAL: FF7 0x00a41edc
short g_YasuiLimitCatastropheBeamEndpointZ = 0;

// GLOBAL: FF7 0x00a41ee0
int g_YasuiLimitCatastropheBeamStep = 0;

unsigned char g_bssPad_00a41ee4[4] = {0};
unsigned char g_bssPad_00a41ee8[4488] = {0};

// GLOBAL: FF7 0x00a43070
struct YasuiLimitCatastropheTargetActor g_YasuiLimitCatastropheTargetActor = {0};

unsigned char g_bssPad_00a43078[20] = {0};

// GLOBAL: FF7 0x00a4308c
YasuiCatastroGrowBlock *g_YasuiLimitCatastropheMatrixPtr = 0;

// GLOBAL: FF7 0x00a43090
int g_YasuiLimitCatastropheBeamStepDelta = 0;

// GLOBAL: FF7 0x00a43094
int g_YasuiLimitCatastropheAdvanceStage = 0;

// GLOBAL: FF7 0x00a43098
int g_YasuiLimitCatastropheTargetIndex = 0;

unsigned char g_bssPad_00a4309c[4] = {0};
unsigned char g_bssPad_00a430a0[61440] = {0};

// GLOBAL: FF7 0x00a520a0
CameraScriptTrack g_YasuiLimitCatastropheCameraFocus = {0};

unsigned char g_bssPad_00a520cc[4] = {0};

// GLOBAL: FF7 0x00a520d0
PsxMatrix g_YasuiLimitCatastropheCameraRotMatrix = {0};

// GLOBAL: FF7 0x00a520f0
struct BattleLimitHighwindLoadedResTable g_YasuiLimitCatastropheActorPos = {0};

// GLOBAL: FF7 0x00a520f4
short g_YasuiLimitCatastropheActorOffsetInit = 0;

// GLOBAL: FF7 0x00a520f8
unsigned char g_YasuiLimitCatastropheActorOffsetActive = 0;

unsigned char g_bssPad_00a520fc[4] = {0};

// GLOBAL: FF7 0x00a52100
int g_YasuiLimitCatastropheDeltaX = 0;

// GLOBAL: FF7 0x00a52104
int g_YasuiLimitCatastropheDeltaY = 0;

// GLOBAL: FF7 0x00a52108
int g_YasuiLimitCatastropheDeltaZ = 0;

unsigned char g_bssPad_00a5210c[4] = {0};

// GLOBAL: FF7 0x00a52110
int g_YasuiLimitCatastropheRenderPacketCursor = 0;

unsigned char g_bssPad_00a52114[4] = {0};

// GLOBAL: FF7 0x00a52118
struct YasuiLimitCatastropheCameraAngles g_YasuiLimitCatastropheCameraAngles = {0};

// GLOBAL: FF7 0x00a5211c
short g_YasuiLimitCatastropheCameraAnglesInit = 0;

// GLOBAL: FF7 0x00a52120
int g_YasuiLimitCatastropheTargetMask = 0;

// GLOBAL: FF7 0x00a52124
char g_YasuiLimitCatastropheTargetMaskActive = 0;

// GLOBAL: FF7 0x00a52128
int g_YasuiLimitCatastropheModelPacketCursor = 0;

// GLOBAL: FF7 0x00a5212c
int g_YasuiLimitCatastropheSourceActor = 0;

// GLOBAL: FF7 0x00a52130
int g_YasuiLimitCatastropheDurationSecondHalf = 0;

// GLOBAL: FF7 0x00a52134
int g_YasuiLimitCatastropheDurationFirstHalf = 0;

// GLOBAL: FF7 0x00a52138
int g_YasuiLimitCatastropheTargetVectorX = 0;

// GLOBAL: FF7 0x00a5213c
int g_YasuiLimitCatastropheTargetVectorY = 0;

// GLOBAL: FF7 0x00a52140
int g_YasuiLimitCatastropheTargetVectorZ = 0;

unsigned char g_bssPad_00a52144[4] = {0};

// GLOBAL: FF7 0x00a52148
struct YasuiLimitCatastropheSlot g_YasuiLimitCatastropheTargetSlotTable[10] = {0};

// GLOBAL: FF7 0x00a52170
CameraScriptTrack *g_pYasuiLimitCatastropheCameraTrack = 0;

// GLOBAL: FF7 0x00a52174
char g_YasuiLimitCatastropheJitterActive = 0;

// GLOBAL: FF7 0x00a52178
struct Vec3sPacked *g_YasuiLimitCatastropheJitterPosPtr = 0;

unsigned char g_bssPad_00a5217c[4] = {0};

// GLOBAL: FF7 0x00a52180
YasuiCatastroPoint g_YasuiLimitCatastropheGunAttachPoint = {0};

// GLOBAL: FF7 0x00a52188
CameraScriptTrack *g_pYasuiLimitCatastropheCameraOther = 0;

// GLOBAL: FF7 0x00a5218c
Vec3sPacked *g_YasuiLimitCatastropheGunPointPtr = 0;

// GLOBAL: FF7 0x00a52190
struct BattleFxVertex *g_YasuiLimitCatastropheBeamPointPtr = 0;

unsigned char g_bssPad_00a52194[4] = {0};

// GLOBAL: FF7 0x00a52198
PsxMatrix g_YasuiLimitCatastropheBeamMatrix = {0};

// GLOBAL: FF7 0x00a521b8
int g_YasuiLimitCatastropheHomingSpeed = 0;

// GLOBAL: FF7 0x00a521bc
char g_YasuiLimitCatastropheGunActive = 0;

// GLOBAL: FF7 0x00a521c0
struct BattleFxMatrixVector *g_YasuiLimitCatastropheGunMatrixPtr = 0;

// GLOBAL: FF7 0x00a521c4
int g_YasuiLimitCatastropheCurrentSortedTarget = 0;

// GLOBAL: FF7 0x00a521c8
CameraScriptTrack g_YasuiLimitCatastropheCameraEye = {0};

// GLOBAL: FF7 0x00a521f4
int g_YasuiLimitCatastropheHookTaskSlot = 0;

// GLOBAL: FF7 0x00a521f8
int g_YasuiLimitCatastropheCameraBasisMatrix = 0;

unsigned char g_bssPad_00a521fc[4] = {0};
unsigned char g_bssPad_00a52200[8] = {0};

// GLOBAL: FF7 0x00a52208
struct YasuiLimitCatastropheCenterPos g_YasuiLimitCatastropheCenterPos = {0};

unsigned char g_bssPad_00a52214[4] = {0};
unsigned char g_bssPad_00a52218[32] = {0};

// GLOBAL: FF7 0x00a52238
YasuiCatastroPoint g_YasuiLimitCatastropheCentroid = {0};

// GLOBAL: FF7 0x00a52240
int g_YasuiLimitCatastropheBillboardMatrixPtr = 0;

// GLOBAL: FF7 0x00a52244
int g_YasuiLimitCatastropheDuration = 0;

// GLOBAL: FF7 0x00a52248
char g_YasuiLimitCatastropheTrailActive = 0;

// GLOBAL: FF7 0x00a5224c
char g_YasuiLimitCatastropheLineActive = 0;

// GLOBAL: FF7 0x00a52250
struct YasuiLimitCatastropheResourceHandle g_YasuiLimitCatastropheResourceHandle = {0};

// GLOBAL: FF7 0x00a5225c
short g_YasuiLimitCatastropheFloorHeight = 0;

unsigned char g_bssPad_00a52260[8] = {0};
// clang-format on

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)


/* Loads resource and sound packages for Barret's Catastrophe limit break. */
// FUNCTION: FF7 0x004685f0
void __cdecl BattleFx_LimitLoadResCatastrophe(void)
{
    GfxModelTableBank *graphicsOwner;
    GfxResourceRequest reqBuffer;
    int kind;
    char tmpBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && (g_YasuiLimitCatastropheResourceHandle.resourceHandle) == 0) {
        kind = 6;
        resourceTable = (int)&g_YasuiLimitCatastropheResourceDef;
        graphicsOwner = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x007ed924
        pRequest->resourcePath = BattleFx_BuildResourcePath("LIMIT/CATASTRO", tmpBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_YasuiLimitCatastropheResourceHandle, graphicsOwner);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_YasuiLimitCatastropheSoundGroups);
    }
}

/* Validates target and initializes task state for Catastrophe sequence. */
// FUNCTION: FF7 0x00468691
int __cdecl BattleFx_LimitEnterCatastrophe(int targetMask, int actorIdx)
{
    g_YasuiLimitCatastropheModelPacketCursor = Gfx_GetPsxPacketScratchPtr();
    g_YasuiLimitCatastropheAdvanceStage = 0;
    (g_YasuiLimitCatastropheTargetActor.actorIndex) = 0xFFFFFFFF;
    g_YasuiLimitCatastropheTargetIndex = 0;
    g_YasuiLimitCatastropheActorOffsetInit = 0;
    g_YasuiLimitCatastropheActorPos.posY = g_YasuiLimitCatastropheActorOffsetInit;
    (g_YasuiLimitCatastropheActorPos.posX) = g_YasuiLimitCatastropheActorPos.posY;
    g_YasuiLimitCatastropheCurrentSortedTarget = 0;
    g_YasuiLimitCatastropheCameraAnglesInit = 0;
    g_YasuiLimitCatastropheCameraAngles.cameraAngle = g_YasuiLimitCatastropheCameraAnglesInit;
    (g_YasuiLimitCatastropheCameraAngles.cameraAngleY) = g_YasuiLimitCatastropheCameraAngles.cameraAngle;
    BattleFx_LimitLoadResCatastrophe();
    BattleFx_LimitBeginCatastrophe(targetMask, actorIdx);
    return 0;
}

/* Spawns root timeline, camera script, and beam emitter tasks for Catastrophe. */
// FUNCTION: FF7 0x00468725
void __cdecl BattleFx_LimitBeginCatastrophe(int targetMask, int actorIdx)
{
    int timeline;
    int targetCnt;

    Battle_AllocateResourceSlot(BattleFx_LimitCatastropheWaitEffectEndTask);
    g_YasuiLimitCatastropheTargetMask = targetMask;
    g_YasuiLimitCatastropheSourceActor = actorIdx;
    Battle_GetActorMaskCentroid(g_YasuiLimitCatastropheTargetMask, &g_YasuiLimitCatastropheCentroid.x);
    if (g_YasuiLimitCatastropheCentroid.z < g_BattleActorRecords[actorIdx].posZ) {
        g_YasuiLimitCatastropheCameraAngles.cameraAngle = 0x800;
    }
    RotMatrix((SVECTOR *)&g_YasuiLimitCatastropheCameraAngles, &g_YasuiLimitCatastropheCameraBasisMatrix);
    g_YasuiLimitCatastropheCenterPos.transX = 0;
    g_YasuiLimitCatastropheCenterPos.transY = 0;
    *(int *)&g_YasuiLimitCatastropheCenterPos.limitCatastroCenterFloorY = g_YasuiLimitCatastropheCentroid.z;
    targetCnt = Battle_PopCountActorMask(targetMask);
    g_YasuiLimitCatastropheDuration = 0x5A / targetCnt;
    g_YasuiLimitCatastropheDurationFirstHalf = g_YasuiLimitCatastropheDuration / 2;
    g_YasuiLimitCatastropheDurationSecondHalf = g_YasuiLimitCatastropheDuration - g_YasuiLimitCatastropheDurationFirstHalf;
    BattleFx_LimitCatastropheBuildSortedSlotTable();
    timeline = (int)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCatastropheTimelineTask);
    *(Int64Pair *)((char *)timeline + 4) = g_YasuiLimitCatastropheCentroid.raw;
    Battle_AllocateResourceSlot(BattleFx_LimitCatastropheActorYOffsetTask);
    BattleFx_CatastropheSpawnCameraScript((int)&g_YasuiLimitCatastropheCameraScript, (int)&g_YasuiLimitCatastropheCameraBasisMatrix, actorIdx);
    g_BattleScriptEffectFlag = 1;
    g_BattleStageHideFlags |= 8;
}

/* Allocates and initializes camera tracking script task for Catastrophe. */
// FUNCTION: FF7 0x00468862
void __cdecl BattleFx_CatastropheSpawnCameraScript(int scriptPtr, int matrixPtr, int sourceActor)
{
    BattleFxCatastropheCameraSlot *slot;

    slot = (BattleFxCatastropheCameraSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_CatastropheRunCameraScript);
    slot->scriptAddr = scriptPtr;
    slot->matrixAddr = matrixPtr;
    slot->sourceActor = sourceActor;
}

/* Executes scripted camera movement steps and focal point updates. */
// FUNCTION: FF7 0x004688a2
void __cdecl BattleFx_CatastropheRunCameraScript(void)
{
    /* VC5 hash order fixes the slots: p=easing, q=wide vector, r=task,
       s=transform flags, t=track index, u/w=scratch vectors. v and x retain
       the original unused frame space (v is the easing-loop index in the
       nine-command dialect). The opcode i follows that space. */
    CameraScriptTask *taskPtr;
    int *wideVec;
    int easedVal;
    CameraScriptVector *scratchVector;
    int trackIdx;
    int bits;
    CameraScriptVector *baseScratchVec;
    int v;
    int x[66];
    short opcode;

    taskPtr = (CameraScriptTask *)((char *)BATTLE_CURRENT_TASK_SLOT);
    scratchVector = (CameraScriptVector *)Gfx_GetPsxScratchpadPtr(0);
    baseScratchVec = scratchVector + 1;
    wideVec = (int *)(baseScratchVec + 1);
    if (g_BattleIsPaused == 0) {
        if (taskPtr->initialized == 0) {
            g_YasuiLimitCatastropheCameraEye.frames = 0;
            g_YasuiLimitCatastropheCameraFocus.frames = 0;
            g_YasuiLimitCatastropheCameraEye.actor = -1;
            g_YasuiLimitCatastropheCameraFocus.actor = -1;
            taskPtr->initialized = 1;
        }
        while (g_YasuiLimitCatastropheCameraEye.frames == 0 || g_YasuiLimitCatastropheCameraFocus.frames == 0) {
            opcode = *taskPtr->cursor++;
            if (opcode & 0x40) {
                g_pYasuiLimitCatastropheCameraTrack = &g_YasuiLimitCatastropheCameraEye;
                g_pYasuiLimitCatastropheCameraOther = &g_YasuiLimitCatastropheCameraFocus;
            } else {
                g_pYasuiLimitCatastropheCameraTrack = &g_YasuiLimitCatastropheCameraFocus;
                g_pYasuiLimitCatastropheCameraOther = &g_YasuiLimitCatastropheCameraEye;
            }
            g_pYasuiLimitCatastropheCameraTrack->mode = opcode & ~0xc0;
            switch (g_pYasuiLimitCatastropheCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                g_pYasuiLimitCatastropheCameraTrack->frames = BattleFx_CatastropheReadCameraScriptOperand(&taskPtr->cursor);
                break;
            case SUMMON_CAMERA_SET_POSITION:
                g_pYasuiLimitCatastropheCameraTrack->position.x = *taskPtr->cursor++;
                g_pYasuiLimitCatastropheCameraTrack->position.y = *taskPtr->cursor++;
                g_pYasuiLimitCatastropheCameraTrack->position.z = *taskPtr->cursor++;
                break;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_pYasuiLimitCatastropheCameraTrack->frames = BattleFx_CatastropheReadCameraScriptOperand(&taskPtr->cursor);
                g_pYasuiLimitCatastropheCameraTrack->velocity.x = (*taskPtr->cursor++ - g_pYasuiLimitCatastropheCameraTrack->position.x) / g_pYasuiLimitCatastropheCameraTrack->frames;
                g_pYasuiLimitCatastropheCameraTrack->velocity.y = (*taskPtr->cursor++ - g_pYasuiLimitCatastropheCameraTrack->position.y) / g_pYasuiLimitCatastropheCameraTrack->frames;
                g_pYasuiLimitCatastropheCameraTrack->velocity.z = (*taskPtr->cursor++ - g_pYasuiLimitCatastropheCameraTrack->position.z) / g_pYasuiLimitCatastropheCameraTrack->frames;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                scratchVector->x = g_pYasuiLimitCatastropheCameraOther->position.x - g_pYasuiLimitCatastropheCameraTrack->position.x;
                scratchVector->y = g_pYasuiLimitCatastropheCameraOther->position.y - g_pYasuiLimitCatastropheCameraTrack->position.y;
                scratchVector->z = g_pYasuiLimitCatastropheCameraOther->position.z - g_pYasuiLimitCatastropheCameraTrack->position.z;
                g_pYasuiLimitCatastropheCameraTrack->distance = SqrtToInt(scratchVector->x * scratchVector->x + scratchVector->y * scratchVector->y + scratchVector->z * scratchVector->z);
                g_pYasuiLimitCatastropheCameraTrack->base = g_pYasuiLimitCatastropheCameraTrack->position;
                g_pYasuiLimitCatastropheCameraTrack->frames = BattleFx_CatastropheReadCameraScriptOperand(&taskPtr->cursor);
                scratchVector->x = *taskPtr->cursor++;
                scratchVector->y = *taskPtr->cursor++;
                scratchVector->z = *taskPtr->cursor++;
                g_pYasuiLimitCatastropheCameraTrack->velocity.x = (scratchVector->x - g_pYasuiLimitCatastropheCameraTrack->position.x) / g_pYasuiLimitCatastropheCameraTrack->frames;
                g_pYasuiLimitCatastropheCameraTrack->velocity.y = (scratchVector->y - g_pYasuiLimitCatastropheCameraTrack->position.y) / g_pYasuiLimitCatastropheCameraTrack->frames;
                g_pYasuiLimitCatastropheCameraTrack->velocity.z = (scratchVector->z - g_pYasuiLimitCatastropheCameraTrack->position.z) / g_pYasuiLimitCatastropheCameraTrack->frames;
                switch (g_pYasuiLimitCatastropheCameraOther->mode) {
                case SUMMON_CAMERA_MOVE_LINEAR:
                    baseScratchVec->x = g_pYasuiLimitCatastropheCameraOther->position.x + g_pYasuiLimitCatastropheCameraOther->velocity.x * g_pYasuiLimitCatastropheCameraOther->frames;
                    baseScratchVec->y = g_pYasuiLimitCatastropheCameraOther->position.y + g_pYasuiLimitCatastropheCameraOther->velocity.y * g_pYasuiLimitCatastropheCameraOther->frames;
                    baseScratchVec->z = g_pYasuiLimitCatastropheCameraOther->position.z + g_pYasuiLimitCatastropheCameraOther->velocity.z * g_pYasuiLimitCatastropheCameraOther->frames;
                    break;
                default:
                    baseScratchVec->x = g_pYasuiLimitCatastropheCameraOther->position.x;
                    baseScratchVec->y = g_pYasuiLimitCatastropheCameraOther->position.y;
                    baseScratchVec->z = g_pYasuiLimitCatastropheCameraOther->position.z;
                    break;
                }
                scratchVector->x -= baseScratchVec->x;
                scratchVector->y -= baseScratchVec->y;
                scratchVector->z -= baseScratchVec->z;
                g_pYasuiLimitCatastropheCameraTrack->delta = (SqrtToInt(scratchVector->x * scratchVector->x + scratchVector->y * scratchVector->y + scratchVector->z * scratchVector->z) - g_pYasuiLimitCatastropheCameraTrack->distance) / g_pYasuiLimitCatastropheCameraTrack->frames;
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_pYasuiLimitCatastropheCameraTrack->frames = BattleFx_CatastropheReadCameraScriptOperand(&taskPtr->cursor);
                scratchVector->x = *taskPtr->cursor++ - g_pYasuiLimitCatastropheCameraTrack->position.x;
                scratchVector->y = *taskPtr->cursor++ - g_pYasuiLimitCatastropheCameraTrack->position.y;
                scratchVector->z = *taskPtr->cursor++ - g_pYasuiLimitCatastropheCameraTrack->position.z;
                g_pYasuiLimitCatastropheCameraTrack->base.x = ((scratchVector->x - g_pYasuiLimitCatastropheCameraTrack->velocity.x * g_pYasuiLimitCatastropheCameraTrack->frames) * 2) / (g_pYasuiLimitCatastropheCameraTrack->frames * g_pYasuiLimitCatastropheCameraTrack->frames);
                g_pYasuiLimitCatastropheCameraTrack->base.y = ((scratchVector->y - g_pYasuiLimitCatastropheCameraTrack->velocity.y * g_pYasuiLimitCatastropheCameraTrack->frames) * 2) / (g_pYasuiLimitCatastropheCameraTrack->frames * g_pYasuiLimitCatastropheCameraTrack->frames);
                g_pYasuiLimitCatastropheCameraTrack->base.z = ((scratchVector->z - g_pYasuiLimitCatastropheCameraTrack->velocity.z * g_pYasuiLimitCatastropheCameraTrack->frames) * 2) / (g_pYasuiLimitCatastropheCameraTrack->frames * g_pYasuiLimitCatastropheCameraTrack->frames);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_pYasuiLimitCatastropheCameraTrack->frames = BattleFx_CatastropheReadCameraScriptOperand(&taskPtr->cursor);
                g_pYasuiLimitCatastropheCameraTrack->easing = *taskPtr->cursor++;
                g_pYasuiLimitCatastropheCameraTrack->distance = 0;
                g_pYasuiLimitCatastropheCameraTrack->delta = 0x1000 / g_pYasuiLimitCatastropheCameraTrack->frames;
                g_pYasuiLimitCatastropheCameraTrack->base.x = g_pYasuiLimitCatastropheCameraTrack->position.x;
                g_pYasuiLimitCatastropheCameraTrack->base.y = g_pYasuiLimitCatastropheCameraTrack->position.y;
                g_pYasuiLimitCatastropheCameraTrack->base.z = g_pYasuiLimitCatastropheCameraTrack->position.z;
                g_pYasuiLimitCatastropheCameraTrack->velocity.x = *taskPtr->cursor++ - g_pYasuiLimitCatastropheCameraTrack->position.x;
                g_pYasuiLimitCatastropheCameraTrack->velocity.y = *taskPtr->cursor++ - g_pYasuiLimitCatastropheCameraTrack->position.y;
                g_pYasuiLimitCatastropheCameraTrack->velocity.z = *taskPtr->cursor++ - g_pYasuiLimitCatastropheCameraTrack->position.z;
                break;
            case SUMMON_CAMERA_ATTACH_BONE:
                if ((g_pYasuiLimitCatastropheCameraTrack->actor = *taskPtr->cursor++) == 0)
                    g_pYasuiLimitCatastropheCameraTrack->actor = taskPtr->actor;
                g_pYasuiLimitCatastropheCameraTrack->bone = *taskPtr->cursor++;
                break;
            case SUMMON_CAMERA_DETACH:
                g_pYasuiLimitCatastropheCameraTrack->actor = -1;
                break;
            case SUMMON_CAMERA_WAIT_NO_WRITEBACK:
                g_pYasuiLimitCatastropheCameraTrack->frames = BattleFx_CatastropheReadCameraScriptOperand(&taskPtr->cursor);
                break;
            case SUMMON_CAMERA_SET_SCRIPT_VALUE:
                g_BattleCameraScriptValue = *taskPtr->cursor++;
                break;
            default:
                taskPtr->active = -1;
                return;
            }
        }
        g_pYasuiLimitCatastropheCameraTrack = &g_YasuiLimitCatastropheCameraEye;
        g_pYasuiLimitCatastropheCameraOther = &g_YasuiLimitCatastropheCameraFocus;
        for (trackIdx = 0; trackIdx < 2; trackIdx++) {
            switch (g_pYasuiLimitCatastropheCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_pYasuiLimitCatastropheCameraTrack->velocity.x += g_pYasuiLimitCatastropheCameraTrack->base.x;
                g_pYasuiLimitCatastropheCameraTrack->velocity.y += g_pYasuiLimitCatastropheCameraTrack->base.y;
                g_pYasuiLimitCatastropheCameraTrack->velocity.z += g_pYasuiLimitCatastropheCameraTrack->base.z;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_pYasuiLimitCatastropheCameraTrack->position.x += g_pYasuiLimitCatastropheCameraTrack->velocity.x;
                g_pYasuiLimitCatastropheCameraTrack->position.y += g_pYasuiLimitCatastropheCameraTrack->velocity.y;
                g_pYasuiLimitCatastropheCameraTrack->position.z += g_pYasuiLimitCatastropheCameraTrack->velocity.z;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                g_pYasuiLimitCatastropheCameraTrack->base.x += g_pYasuiLimitCatastropheCameraTrack->velocity.x;
                g_pYasuiLimitCatastropheCameraTrack->base.y += g_pYasuiLimitCatastropheCameraTrack->velocity.y;
                g_pYasuiLimitCatastropheCameraTrack->base.z += g_pYasuiLimitCatastropheCameraTrack->velocity.z;
                g_pYasuiLimitCatastropheCameraTrack->distance += g_pYasuiLimitCatastropheCameraTrack->delta;
                wideVec[0] = g_pYasuiLimitCatastropheCameraOther->position.x - g_pYasuiLimitCatastropheCameraTrack->base.x;
                wideVec[1] = g_pYasuiLimitCatastropheCameraOther->position.y - g_pYasuiLimitCatastropheCameraTrack->base.y;
                wideVec[2] = g_pYasuiLimitCatastropheCameraOther->position.z - g_pYasuiLimitCatastropheCameraTrack->base.z;
                VectorNormalToSVector(wideVec, (short *)scratchVector);
                g_pYasuiLimitCatastropheCameraTrack->position.x = g_pYasuiLimitCatastropheCameraOther->position.x - ((scratchVector->x * g_pYasuiLimitCatastropheCameraTrack->distance) >> 12);
                g_pYasuiLimitCatastropheCameraTrack->position.y = g_pYasuiLimitCatastropheCameraOther->position.y - ((scratchVector->y * g_pYasuiLimitCatastropheCameraTrack->distance) >> 12);
                g_pYasuiLimitCatastropheCameraTrack->position.z = g_pYasuiLimitCatastropheCameraOther->position.z - ((scratchVector->z * g_pYasuiLimitCatastropheCameraTrack->distance) >> 12);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_pYasuiLimitCatastropheCameraTrack->distance += g_pYasuiLimitCatastropheCameraTrack->delta;
                easedVal = g_pYasuiLimitCatastropheCameraTrack->distance;
                easedVal = BattleFx_CatastropheApplyCameraEasing(easedVal, g_pYasuiLimitCatastropheCameraTrack->easing);
                g_pYasuiLimitCatastropheCameraTrack->position.x = g_pYasuiLimitCatastropheCameraTrack->base.x + ((g_pYasuiLimitCatastropheCameraTrack->velocity.x * easedVal) >> 12);
                g_pYasuiLimitCatastropheCameraTrack->position.y = g_pYasuiLimitCatastropheCameraTrack->base.y + ((g_pYasuiLimitCatastropheCameraTrack->velocity.y * easedVal) >> 12);
                g_pYasuiLimitCatastropheCameraTrack->position.z = g_pYasuiLimitCatastropheCameraTrack->base.z + ((g_pYasuiLimitCatastropheCameraTrack->velocity.z * easedVal) >> 12);
                break;
            }
            g_pYasuiLimitCatastropheCameraTrack->frames--;
            g_pYasuiLimitCatastropheCameraTrack = &g_YasuiLimitCatastropheCameraFocus;
            g_pYasuiLimitCatastropheCameraOther = &g_YasuiLimitCatastropheCameraEye;
        }
    }
    if (taskPtr->matrix) {
        if (g_YasuiLimitCatastropheCameraEye.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            SetRotMatrix((void *)taskPtr->matrix);
            SetTransMatrix((void *)taskPtr->matrix);
            if (g_YasuiLimitCatastropheCameraEye.actor == -1) {
                RotTransPoint(&g_YasuiLimitCatastropheCameraEye.position, wideVec, &bits);
                g_BattleCameraPosition.cameraPosX = wideVec[0];
                g_BattleCameraPosition.cameraPosY = wideVec[1];
                g_BattleCameraPosZ = wideVec[2];
            } else {
                RotateSVectorByInstalledMatrix(&g_YasuiLimitCatastropheCameraEye.position, wideVec);
                if (g_YasuiLimitCatastropheCameraEye.bone != -1) {
                    Battle_GetBoneWorldPos(g_YasuiLimitCatastropheCameraEye.actor, g_YasuiLimitCatastropheCameraEye.bone, (short *)scratchVector);
                } else {
                    scratchVector->x = g_BattleActorRecords[g_YasuiLimitCatastropheCameraEye.actor].posX;
                    scratchVector->y = g_BattleActorRecords[g_YasuiLimitCatastropheCameraEye.actor].posY;
                    scratchVector->z = g_BattleActorRecords[g_YasuiLimitCatastropheCameraEye.actor].posZ;
                }
                g_BattleCameraPosition.cameraPosX = wideVec[0] + scratchVector->x;
                g_BattleCameraPosition.cameraPosY = wideVec[1] + scratchVector->y;
                g_BattleCameraPosZ = wideVec[2] + scratchVector->z;
                SetRotMatrix((void *)taskPtr->matrix);
                SetTransMatrix((void *)taskPtr->matrix);
            }
        }
        if (g_YasuiLimitCatastropheCameraFocus.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            if (g_YasuiLimitCatastropheCameraFocus.actor == -1) {
                RotTransPoint(&g_YasuiLimitCatastropheCameraFocus.position, wideVec, &bits);
                g_BattleCameraFocalPoint.focalPointX = wideVec[0];
                g_BattleCameraFocalPoint.focalPointY = wideVec[1];
                g_BattleCameraFocusZ = wideVec[2];
            } else {
                RotateSVectorByInstalledMatrix(&g_YasuiLimitCatastropheCameraFocus.position, wideVec);
                if (g_YasuiLimitCatastropheCameraFocus.bone != -1) {
                    Battle_GetBoneWorldPos(g_YasuiLimitCatastropheCameraFocus.actor, g_YasuiLimitCatastropheCameraFocus.bone, (short *)scratchVector);
                } else {
                    scratchVector->x = g_BattleActorRecords[g_YasuiLimitCatastropheCameraFocus.actor].posX;
                    scratchVector->y = g_BattleActorRecords[g_YasuiLimitCatastropheCameraFocus.actor].posY;
                    scratchVector->z = g_BattleActorRecords[g_YasuiLimitCatastropheCameraFocus.actor].posZ;
                }
                g_BattleCameraFocalPoint.focalPointX = wideVec[0] + scratchVector->x;
                g_BattleCameraFocalPoint.focalPointY = wideVec[1] + scratchVector->y;
                g_BattleCameraFocusZ = wideVec[2] + scratchVector->z;
            }
        }
    } else {
        if (g_YasuiLimitCatastropheCameraEye.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            *(int *)&g_BattleCameraPosition = *(int *)&g_YasuiLimitCatastropheCameraEye.position;
            *(int *)&g_BattleCameraPosZ = *(int *)&g_YasuiLimitCatastropheCameraEye.position.z;
        }
        if (g_YasuiLimitCatastropheCameraFocus.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            *(int *)&g_BattleCameraFocalPoint = *(int *)&g_YasuiLimitCatastropheCameraFocus.position;
            *(int *)&g_BattleCameraFocusZ = *(int *)&g_YasuiLimitCatastropheCameraFocus.position.z;
        }
    }
}

/* Decodes variable-length operand from camera script instruction stream. */
// FUNCTION: FF7 0x004697e8
short __cdecl BattleFx_CatastropheReadCameraScriptOperand(void *track)
{
    int val;

    val = *(short *)((char *)*(int *)track);
    *(int *)track += 2;
    if (val < 0) {
        val = (&g_YasuiLimitCatastropheTargetActor.limitCatastroCameraOperandTable)[-val];
    }
    return (short)val;
}

/* Computes sine-eased camera position delta between current and target vectors. */
// FUNCTION: FF7 0x00469822
int __cdecl BattleFx_CatastropheApplyCameraEasing(int current, int target)
{
    int idx;
    int src;
    int exponent;

    src = current;
    exponent = target >> 8;
    if (exponent > 0) {
        for (idx = 0; idx < exponent; idx++) {
            current = SinFixed(current / 4);
        }
    } else {
        current = 0x1000 - current;
        for (idx = 0; idx < -exponent; idx++) {
            current = SinFixed(current / 4);
        }
        current = 0x1000 - current;
    }
    exponent = target & 0xFF;
    current = current * exponent + src * (0x100 - exponent) >> 8;
    return current;
}

/* Offsets Barret's vertical position and sets jumping animation flag. */
// FUNCTION: FF7 0x004698ef
void __cdecl BattleFx_LimitCatastropheActorYOffsetTask(void)
{
    int frameIndex;
    int r;
    BattleTaskSlot *taskPtr;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    frameIndex = taskPtr->frameCounter + 1;
    if (frameIndex < 0x36) { /* empty first arm: lib/codegen.h */
    } else {
        frameIndex -= 0x36;
        if (frameIndex < 0xA) {
            if (frameIndex == 0) {
                Battle_AllocateResourceSlot(BattleFx_LimitCatastropheActorWorldSpriteBurstTask);
            }
            g_BattleActorRecords[g_YasuiLimitCatastropheSourceActor].posY = frameIndex * frameIndex * 0xC8 / 2 + frameIndex * -0x7D0;
        } else {
            frameIndex -= 0xA;
            if (frameIndex < 0x13) { /* empty first arm: lib/codegen.h */
            } else {
                frameIndex -= 0x13;
                if (frameIndex < 0x73) {
                    if (frameIndex == 0x31) {
                        ((ActorStatusBits *)((char *)&g_BattleActorStates[g_YasuiLimitCatastropheSourceActor].flags))->bit1 = 1;
                    } else if (frameIndex == 0x72) {
                        ((ActorStatusBits *)((char *)&g_BattleActorStates[g_YasuiLimitCatastropheSourceActor].flags))->bit1 = 0;
                    }
                } else {
                    frameIndex -= 0x73;
                    if (frameIndex < 0xA) {
                        g_BattleActorRecords[g_YasuiLimitCatastropheSourceActor].posY = frameIndex * frameIndex * 0xC8 / 2 - 0x2710;
                    } else {
                        Battle_AllocateResourceSlot(BattleFx_LimitCatastropheActorWorldSpriteBurstTask);
                        g_BattleActorRecords[g_YasuiLimitCatastropheSourceActor].posY = 0;
                        taskPtr->ownerSlot = BATTLE_TASK_FREE;
                        return;
                    }
                }
            }
        }
    }
    (taskPtr->frameCounter)++;
}

/* Emits radial burst of world-space sprites around casting actor. */
// FUNCTION: FF7 0x00469a7c
void __cdecl BattleFx_LimitCatastropheActorWorldSpriteBurstTask(void)
{
    int scale;
    BattleTaskSlot *taskPtr;
    int i;
    int bytes;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if ((*(unsigned char *)&g_YasuiLimitCatastropheJitterActive & 1) == 0) {
        g_YasuiLimitCatastropheJitterActive |= 1;
        g_YasuiLimitCatastropheJitterPosPtr = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        g_YasuiLimitCatastropheJitterPosPtr->vx = g_BattleActorRecords[g_YasuiLimitCatastropheSourceActor].posX + rand() % 0x3E8 + -0x1F4;
        g_YasuiLimitCatastropheJitterPosPtr->vy = g_BattleActorRecords[g_YasuiLimitCatastropheSourceActor].posY + 0x64;
        g_YasuiLimitCatastropheJitterPosPtr->vz = g_BattleActorRecords[g_YasuiLimitCatastropheSourceActor].posZ + rand() % 0x3E8 + -0x1F4;
        if (DEAD_IN_RETAIL != 0) {
            bytes = 0x1800;
        } else {
            bytes = rand() % 0x800 + 0x1000;
        }
        scale = bytes;
        Battle_SpawnWorldSpriteTask((Vec3sPacked *)g_YasuiLimitCatastropheJitterPosPtr, scale, scale);
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter >= 4) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Drives main timing sequence, spawning beams, flashes, and sound effects. */
// FUNCTION: FF7 0x00469be5
void __cdecl BattleFx_LimitCatastropheTimelineTask(void)
{
    int frameIndex;
    BattleFxCatastropheTimelineRecord *ring;
    BattleTaskSlot *taskPtr;
    int t;
    int selChild;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if ((*(unsigned char *)&g_YasuiLimitCatastropheGunActive & 1) == 0) {
        g_YasuiLimitCatastropheGunActive |= 1;
        g_YasuiLimitCatastropheGunMatrixPtr = (BattleFxMatrixVector *)Gfx_GetPsxScratchpadPtr(0);
    }
    Battle_ComposeMatrixRelativeToCamera((short *)g_BattleCameraTransform.matrix, (void *)((g_YasuiLimitCatastropheSourceActor << 6) + (int)g_BattleActorTransformRecords[0].auxNode), (PsxMatrix *)g_YasuiLimitCatastropheGunMatrixPtr);
    TransformVectorToInt16(g_YasuiLimitCatastropheGunMatrixPtr, (int *)&g_YasuiLimitCatastropheOffsetVector, &g_YasuiLimitCatastropheGunAttachPoint.x);
    g_YasuiLimitCatastropheGunAttachPoint.x += g_YasuiLimitCatastropheGunMatrixPtr->x;
    g_YasuiLimitCatastropheGunAttachPoint.y += g_YasuiLimitCatastropheGunMatrixPtr->y;
    g_YasuiLimitCatastropheGunAttachPoint.z += g_YasuiLimitCatastropheGunMatrixPtr->z;
    if (g_BattleIsPaused != 0) {
        return;
    }
    frameIndex = taskPtr->frameCounter;
    if (frameIndex < 0x36) {
        if (frameIndex == 0) {
            g_BattleFxTimelineState.red = 0x40;
            g_BattleFxTimelineState.channel1 = 0x10;
            g_BattleFxTimelineState.channel2 = 0;
            Battle_StartFixedPointRamp(0x500, 0xA);
        }
        if (frameIndex == 0x1E) {
            selChild = (int)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCatastropheSpawnSpriteTrailTask);
            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitCatastropheSourceActor].posX), 0x18C);
        }
    } else {
        frameIndex -= 0x36;
        if (frameIndex < 0xA) { /* empty first arm: lib/codegen.h */
        } else {
            frameIndex -= 0xA;
            if (frameIndex < 0x13) {
                if (frameIndex == 0) {
                    g_YasuiLimitCatastropheHookTaskSlot = (int)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCatastropheHookDriverTask);
                }
            } else {
                frameIndex -= 0x13;
                if (frameIndex < 0x73) {
                    if (frameIndex == 0) {
                        Battle_AllocateResourceSlot(BattleFx_LimitCatastropheBeamModelTask);
                        selChild = (int)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCatastropheBillboardModelTask);
                        *(Int64Pair *)((char *)selChild + 4) = g_YasuiLimitCatastropheGunAttachPoint.raw;
                        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_YasuiLimitCatastropheGunAttachPoint), 0x182);
                    }
                    if (frameIndex == 0xF) {
                        Battle_AllocateResourceSlot(BattleFx_LimitCatastropheJitterTask);
                    }
                    if (frameIndex >= 0xF) {
                        frameIndex -= 0xF;
                        if (frameIndex % 9 == 4 && g_YasuiLimitCatastropheCurrentSortedTarget < 0xA) {
                            ring = (BattleFxCatastropheTimelineRecord *)Battle_AllocSceneMessage(0xA);
                            ring->targetSlot = g_YasuiLimitCatastropheTargetSlotTable[g_YasuiLimitCatastropheCurrentSortedTarget].targetSlot;
                            ring->slotIndex = g_YasuiLimitCatastropheTargetSlotTable[g_YasuiLimitCatastropheCurrentSortedTarget].slotIndex;
                            g_YasuiLimitCatastropheCurrentSortedTarget++;
                        }
                    }
                } else {
                    frameIndex -= 0x73;
                    if (frameIndex < 0xA) {
                        if (frameIndex == 0) {
                            Battle_AllocateResourceSlot(BattleFx_LimitCatastropheScreenFlashTask);
                        }
                    } else {
                        frameIndex -= 0xA;
                        if (frameIndex < 0xA) {
                            if (frameIndex == 0) {
                                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitCatastropheSourceActor].posX), 0x38);
                            }
                        } else {
                            g_BattleStageHideFlags = 0;
                            taskPtr->ownerSlot = BATTLE_TASK_FREE;
                            Sfx_ReleaseCachedBuffer(0x18C, 0xF);
                            Sfx_ReleaseCachedBuffer(0x182, 0xF);
                            Sfx_ReleaseCachedBuffer(0x38, 0xF);
                        }
                    }
                }
            }
        }
    }
    (taskPtr->frameCounter)++;
}

/* Adds randomized jitter displacement to casting actor's coordinates. */
// FUNCTION: FF7 0x00469f3a
void __cdecl BattleFx_LimitCatastropheJitterTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if ((task->frameCounter & 1) != 0) {
        g_BattleCameraTransform.posY = 0x64;
    } else {
        g_BattleCameraTransform.posY = 0xFF9C;
    }
    (task->frameCounter)++;
    if (task->frameCounter >= 0x64) {
        g_BattleCameraTransform.posY = 0;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Submits billboard model draw requests with expanding scale. */
// FUNCTION: FF7 0x00469fb3
void __cdecl BattleFx_LimitCatastropheBillboardModelTask(void)
{
    BattleFxLimitCatastropheBillboard *rec;
    int frameIndex;
    int scale;

    rec = (BattleFxLimitCatastropheBillboard *)BATTLE_CURRENT_TASK_SLOT;
    ((BattleFxModelDrawRequest *)&g_YasuiLimitCatastropheModelDrawReq1)->modelHandle = Gfx_ModelTableGetRenderObject(1, g_YasuiLimitCatastropheResourceHandle.resourceHandle);
    frameIndex = rec->frameCounter;
    if (frameIndex < 3) {
        scale = (frameIndex << 0xD) / 3;
        *(short *)&g_YasuiLimitCatastropheModelDrawReq1.limitCatastroModelDrawReq1Fraction = 0;
    } else {
        frameIndex -= 3;
        if (frameIndex < 5) {
            scale = 0x2000;
            *(short *)&g_YasuiLimitCatastropheModelDrawReq1.limitCatastroModelDrawReq1Fraction = (frameIndex << 0xC) / 5;
        } else {
            rec->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    BattleFx_InstallBillboardMatrix(&rec->billboardMatrix, scale, 0);
    g_YasuiLimitCatastropheModelPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitCatastropheModelDrawReq1, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_YasuiLimitCatastropheModelPacketCursor);
    if (g_BattleIsPaused == 0) {
        (rec->frameCounter)++;
    }
}

/* Drives periodic callback hooks during Catastrophe beam discharge. */
// FUNCTION: FF7 0x0046a0a1
void __cdecl BattleFx_LimitCatastropheHookDriverTask(void)
{
    int paramA;
    int paramB;
    int argC;
    int argD;
    BattleTaskSlot *taskSlot;
    int u;
    int v;

    taskSlot = BATTLE_CURRENT_TASK_SLOT;
    argD = 0x380;
    argC = 0;
    paramB = 0x80;
    paramA = 0x100;
    if (taskSlot->frameCounter == 0) {
        g_YasuiLimitCatastropheFloorHeight = g_YasuiLimitCatastropheCenterPos.limitCatastroCenterFloorY;
        BattleFx_LimitCatastropheInitHookNoOp((int)&g_YasuiLimitCatastropheResourceHandle.hookState, &argD);
        g_YasuiLimitCatastropheResourceHandle.hookFlag = 0;
    }
    BattleFx_LimitCatastropheUpdateHookNoOp((int)&g_YasuiLimitCatastropheResourceHandle.hookState);
    if (g_BattleIsPaused == 0) {
        (taskSlot->frameCounter)++;
        if (taskSlot->frameCounter >= 0x8A) {
            taskSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Default no-op initialization hook callback. */
// FUNCTION: FF7 0x0046a14c
void __cdecl BattleFx_LimitCatastropheInitHookNoOp(int hookState, int *param)
{
}

/* Default no-op update hook callback. */
// FUNCTION: FF7 0x0046a151
void __cdecl BattleFx_LimitCatastropheUpdateHookNoOp(int hookState)
{
}

/* Computes beam geometry, endpoint homing, and rendering between weapon and target. */
// FUNCTION: FF7 0x0046a156
void __cdecl BattleFx_LimitCatastropheBeamModelTask(void)
{
    BattleTaskSlot *taskPtr;
    int nextFrame;
    int calcRad;
    int theta;
    int beamLength;
    int cnt;
    int t;
    BattleFxCatastropheBeamSegment *pChild;
    int magnitude;
    int fadeOffset;
    int startY;
    int lenValue;
    int lateralOffset;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if ((g_YasuiLimitCatastropheActorOffsetActive & 1) == 0) {
        g_YasuiLimitCatastropheActorOffsetActive |= 1;
        g_YasuiLimitCatastropheBillboardMatrixPtr = Gfx_GetPsxScratchpadPtr(0);
    }
    ((BattleFxModelDrawRequest *)&g_YasuiLimitCatastropheModelDrawReq3)->modelHandle = Gfx_ModelTableGetRenderObject(0, g_YasuiLimitCatastropheResourceHandle.resourceHandle);
    nextFrame = taskPtr->frameCounter;
    if (nextFrame < 0xF) {
        if (g_BattleIsPaused == 0 && taskPtr->frameCounter % 5 == 0) {
            Battle_AllocateResourceSlot(BattleFx_LimitCatastropheGrowingBlackModelTask);
        }
        MultiplyMatrix((void *)((g_YasuiLimitCatastropheSourceActor << 6) + (char *)g_BattleActorTransformRecords[0].auxNode), &g_YasuiLimitCatastropheTransformData2, &g_YasuiLimitCatastropheBeamMatrix);
        Gte_OrthonormaliseFromYAxis((BattleOrthoMatrix *)&g_YasuiLimitCatastropheBeamMatrix);
        g_YasuiLimitCatastropheScaleVector.limitCatastroScaleVectorX = nextFrame * 0xD00 / 0xF;
        ScaleMatrix(&g_YasuiLimitCatastropheBeamMatrix, &g_YasuiLimitCatastropheScaleVector);
    } else {
        nextFrame -= 0xF;
        if (nextFrame < 0x64) {
            if (g_BattleIsPaused == 0) {
                BattleFx_LimitCatastropheAdvanceBeamPointAlongTargets((Vec3sPacked *)&g_YasuiLimitCatastropheBeamEndpoint);
                BattleFx_LimitCatastrophePointHookNoOp((int)&g_YasuiLimitCatastropheBeamEndpoint);
                for (cnt = 0; cnt < 3; cnt++) {
                    pChild = (BattleFxCatastropheBeamSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCatastropheAnimatedSpriteTask);
                    theta = (cnt << 0xC) / 3 + rand() % 0x555;
                    pChild->posX = g_YasuiLimitCatastropheBeamEndpoint.x + (SinFixed(theta) * 0x3E8 >> 0xC);
                    pChild->posY = 0;
                    pChild->posZ = g_YasuiLimitCatastropheBeamEndpointZ + (CosFixed(theta) * 0x3E8 >> 0xC);
                }
                for (cnt = 0; cnt < 2; cnt++) {
                    pChild = (BattleFxCatastropheBeamSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCatastropheRisingSpriteTask);
                    theta = (cnt << 0xC) / 2 + rand() % 0x800;
                    calcRad = rand() % 0x2BC + 0x1F4;
                    pChild->posX = g_YasuiLimitCatastropheBeamEndpoint.x + (SinFixed(theta) * calcRad >> 0xC);
                    pChild->posY = 0;
                    pChild->posZ = g_YasuiLimitCatastropheBeamEndpointZ + (CosFixed(theta) * calcRad >> 0xC);
                    if (0, 0) {
                        magnitude = 0x6000;
                    } else {
                        magnitude = rand() % 0x2000 + 0x4000;
                    }
                    pChild->beamX = (short)magnitude;
                    if (0, 0) {
                        fadeOffset = 0x64;
                    } else {
                        fadeOffset = rand() % 0x32 + 0x32;
                    }
                    pChild->offset = (short)fadeOffset;
                }
            }
            beamLength = BattleFx_LimitCatastropheBuildBeamMatrixFromEndpoints((Vec3sPacked *)&g_YasuiLimitCatastropheGunAttachPoint, (Vec3sPacked *)&g_YasuiLimitCatastropheBeamEndpoint, (BattleRotationMatrix *)&g_YasuiLimitCatastropheCameraRotMatrix);
            g_YasuiLimitCatastropheScaleVector.limitCatastroScaleVectorX = taskPtr->frameCounter & 1 ? 0xD00 : 0xD00;
            g_YasuiLimitCatastropheScaleVector.limitCatastroScaleVectorY = (beamLength << 0xC) / 0x30D4;
            MultiplyMatrix((void *)g_BattleCameraTransform.matrix, &g_YasuiLimitCatastropheCameraRotMatrix, &g_YasuiLimitCatastropheBeamMatrix);
            Gte_OrthonormaliseFromYAxis((BattleOrthoMatrix *)&g_YasuiLimitCatastropheBeamMatrix);
            ScaleMatrix(&g_YasuiLimitCatastropheBeamMatrix, &g_YasuiLimitCatastropheScaleVector);
        } else {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    SetRotMatrix(&g_YasuiLimitCatastropheBeamMatrix);
    SetTransMatrix(&g_YasuiLimitCatastropheBeamMatrix);
    g_YasuiLimitCatastropheModelPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitCatastropheModelDrawReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitCatastropheModelPacketCursor);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
        if (taskPtr->frameCounter < 0x6D) {
            for (cnt = 0; cnt < 5; cnt++) {
                pChild = (BattleFxCatastropheBeamSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCatastropheLineSegmentTask);
                if (0, 0) {
                    lateralOffset = 0x5DC;
                } else {
                    lateralOffset = rand() % 0xBB8 - 0x5DC;
                }
                pChild->beamZ = (short)lateralOffset;
                pChild->posX = (short)lateralOffset;
                if (0, 0) {
                    lenValue = 0x7D0;
                } else {
                    lenValue = rand() % 0x5DC + 0x1F4;
                }
                theta = lenValue;
                if (0x30D4 - theta == 0x3E8) {
                    startY = 0x30D4 - theta;
                } else {
                    startY = rand() % (0x30D4 - theta - 0x3E8) + 0x3E8;
                }
                pChild->posY = (short)startY;
                pChild->offset = (short)startY + theta;
                pChild->endZ = 0;
                pChild->posZ = 0;
            }
        }
    }
}

/* Builds oriented cylinder transformation matrix connecting two endpoints. */
// FUNCTION: FF7 0x0046a66f
int __cdecl BattleFx_LimitCatastropheBuildBeamMatrixFromEndpoints(Vec3sPacked *startPos, Vec3sPacked *endPos, BattleRotationMatrix *outMatrix)
{
    VECTOR a;
    VECTOR direction = {0, -0x1000, 0};
    VECTOR cross;
    int len;

    direction.vx = endPos->vx - startPos->vx;
    direction.vy = endPos->vy - startPos->vy;
    direction.vz = endPos->vz - startPos->vz;
    len = SqrtToInt(NormalizeVectorFixed((int *)&direction, (int *)&direction));
    a.vx = direction.vy;
    a.vy = -direction.vx;
    a.vz = 0;
    NormalizeVectorFixed((int *)&a, (int *)&a);
    CrossProductFixed12((VECTOR *)&a, (VECTOR *)&direction, (VECTOR *)&cross);
    NormalizeVectorFixed((int *)&cross, (int *)&cross);
    outMatrix->m00 = a.vx;
    outMatrix->m10 = a.vy;
    outMatrix->m20 = a.vz;
    outMatrix->m01 = direction.vx;
    outMatrix->m11 = direction.vy;
    outMatrix->m21 = direction.vz;
    outMatrix->m02 = cross.vx;
    outMatrix->m12 = cross.vy;
    outMatrix->m22 = cross.vz;
    *(int *)((char *)outMatrix + 0x12) = startPos->vx;
    *(int *)((char *)outMatrix + 0x16) = startPos->vy;
    ((PsxMatrix *)outMatrix)->t[2] = startPos->vz;
    return len;
}

/* Default no-op point processing callback. */
// FUNCTION: FF7 0x0046a7ba
void __cdecl BattleFx_LimitCatastrophePointHookNoOp(int point)
{
}

/* Renders animated smoke sprite particle with frame progression. */
// FUNCTION: FF7 0x0046a7bf
void __cdecl BattleFx_LimitCatastropheAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitCatastropheSpriteDrawReq1.animTable) = Gfx_ModelTableGetLoadedRecord(4, g_YasuiLimitCatastropheResourceHandle.resourceHandle);
    g_YasuiLimitCatastropheSpriteDrawReq1.drawObj = Gfx_ModelTableGetSpriteObject(4, g_YasuiLimitCatastropheResourceHandle.resourceHandle);
    BattleFx_InstallBillboardMatrix((char *)task + 4, 0x5000, 0);
    g_YasuiLimitCatastropheSpriteDrawReq1.frame = task->frameCounter;
    g_YasuiLimitCatastropheModelPacketCursor = BattleFx_SubmitSpriteDraw(&g_YasuiLimitCatastropheSpriteDrawReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitCatastropheModelPacketCursor);
    if (g_BattleIsPaused == 0 && (g_YasuiLimitCatastropheSpriteDrawReq1.animTable) != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)(g_YasuiLimitCatastropheSpriteDrawReq1.animTable) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Renders smoke sprite drifting upward with damping and alpha fade. */
// FUNCTION: FF7 0x0046a890
void __cdecl BattleFx_LimitCatastropheRisingSpriteTask(void)
{
    BattleFxCatastropheRisingSprite *sprite;
    int matrix;

    sprite = (BattleFxCatastropheRisingSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitCatastropheSpriteDrawReq2.spriteTable = Gfx_ModelTableGetLoadedRecord(3, g_YasuiLimitCatastropheResourceHandle.resourceHandle);
    g_YasuiLimitCatastropheSpriteDrawReq2.drawObj = Gfx_ModelTableGetSpriteObject(3, g_YasuiLimitCatastropheResourceHandle.resourceHandle);
    matrix = (int)BattleFx_InstallBillboardMatrix((short *)((int)sprite + 4), sprite->scale, 0);
    g_YasuiLimitCatastropheSpriteDrawReq2.frame = sprite->frame;
    if (*(int *)(matrix + 0x1A) > 0) {
        g_YasuiLimitCatastropheModelPacketCursor = BattleFx_SubmitSpriteDraw(&g_YasuiLimitCatastropheSpriteDrawReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitCatastropheModelPacketCursor);
    }
    if (g_BattleIsPaused == 0) {
        if (g_YasuiLimitCatastropheSpriteDrawReq2.spriteTable != 0) {
            sprite->frame += 1;
            if (sprite->frame >= *(int *)(**(int **)&g_YasuiLimitCatastropheSpriteDrawReq2 + 4)) {
                sprite->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        sprite->posY -= sprite->velocityY;
    }
}

/* Renders expanding dark blast sphere model with scaling transformation. */
// FUNCTION: FF7 0x0046a989
void __cdecl BattleFx_LimitCatastropheGrowingBlackModelTask(void)
{
    BattleTaskSlot *task;
    int ratio;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((*(unsigned char *)&g_YasuiLimitCatastropheLineActive & 1) == 0) {
        g_YasuiLimitCatastropheLineActive |= 1;
        g_YasuiLimitCatastropheMatrixPtr = (YasuiCatastroGrowBlock *)Gfx_GetPsxScratchpadPtr(0);
    }
    (g_YasuiLimitCatastropheModelDrawReq2.catastroModelResource) = Gfx_ModelTableGetRenderObject(2, g_YasuiLimitCatastropheResourceHandle.resourceHandle);
    ratio = (task->frameCounter << 0xC) / 0xA;
    g_YasuiLimitCatastropheMatrixPtr->scaleZ = ratio * 0x3000 >> 0xC;
    g_YasuiLimitCatastropheMatrixPtr->scaleY = g_YasuiLimitCatastropheMatrixPtr->scaleZ;
    g_YasuiLimitCatastropheMatrixPtr->scaleX = g_YasuiLimitCatastropheMatrixPtr->scaleY;
    g_YasuiLimitCatastropheModelDrawReq2.limitCatastroModelDrawReq2Value = ratio << 0xC >> 0xC;
    MultiplyMatrix((void *)((g_YasuiLimitCatastropheSourceActor << 6) + (char *)g_BattleActorTransformRecords[0].auxNode), (int *)&g_YasuiLimitCatastropheTransformData1, g_YasuiLimitCatastropheMatrixPtr);
    ScaleMatrix(g_YasuiLimitCatastropheMatrixPtr, &g_YasuiLimitCatastropheMatrixPtr->scaleX);
    SetRotMatrix(g_YasuiLimitCatastropheMatrixPtr);
    SetTransMatrix(g_YasuiLimitCatastropheMatrixPtr);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitCatastropheModelPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitCatastropheModelDrawReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitCatastropheModelPacketCursor);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= 0xA) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Renders connecting beam line segment with fading opacity. */
// FUNCTION: FF7 0x0046ab00
void __cdecl BattleFx_LimitCatastropheLineSegmentTask(void)
{
    BattleFxCatastropheLinePacket *pPacket;
    int frame;
    BattleFxLimitCatastropheSegmentTask *task;
    int ratio;

    task = (BattleFxLimitCatastropheSegmentTask *)BATTLE_CURRENT_TASK_SLOT;
    if ((*(unsigned char *)&g_YasuiLimitCatastropheTargetMaskActive & 1) == 0) {
        g_YasuiLimitCatastropheTargetMaskActive |= 1;
        g_YasuiLimitCatastropheBeamPointPtr = (struct BattleFxVertex *)(Gfx_GetPsxScratchpadPtr(0));
    }
    frame = task->frameIndex;
    if (frame < 3) {
        ratio = (frame << 0xC) / 3;
        g_YasuiLimitCatastropheBeamPointPtr->x = task->startX;
        g_YasuiLimitCatastropheBeamPointPtr->baseY = task->startY;
        g_YasuiLimitCatastropheBeamPointPtr->baseZ = task->startZ;
        g_YasuiLimitCatastropheBeamPointPtr->endX = task->startX + ((task->endX - task->startX) * ratio >> 0xC);
        g_YasuiLimitCatastropheBeamPointPtr->y = task->startY + ((task->endY - task->startY) * ratio >> 0xC);
        g_YasuiLimitCatastropheBeamPointPtr->z = task->startZ + ((task->endZ - task->startZ) * ratio >> 0xC);
    } else {
        frame -= 3;
        if (frame < 3) {
            ratio = 0x1000 - (frame << 0xC) / 3;
            g_YasuiLimitCatastropheBeamPointPtr->x = task->startX + ((task->endX - task->startX) * ratio >> 0xC);
            g_YasuiLimitCatastropheBeamPointPtr->baseY = task->startY + ((task->endY - task->startY) * ratio >> 0xC);
            g_YasuiLimitCatastropheBeamPointPtr->baseZ = task->startZ + ((task->endZ - task->startZ) * ratio >> 0xC);
            g_YasuiLimitCatastropheBeamPointPtr->endX = task->endX;
            g_YasuiLimitCatastropheBeamPointPtr->y = task->endY;
            g_YasuiLimitCatastropheBeamPointPtr->z = task->endZ;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    pPacket = (BattleFxCatastropheLinePacket *)g_YasuiLimitCatastropheModelPacketCursor;
    SetRotMatrix(&g_YasuiLimitCatastropheBeamMatrix);
    SetTransMatrix(&g_YasuiLimitCatastropheBeamMatrix);
    RefreshGlobalTransformState();
    g_YasuiLimitCatastropheBeamPointPtr->depth = Gfx_TransformTriangleReturnDepthAlt((void *)(((int)g_YasuiLimitCatastropheBeamPointPtr)), (SVECTOR *)(((int)g_YasuiLimitCatastropheBeamPointPtr) + 8), (SVECTOR *)(((int)g_YasuiLimitCatastropheBeamPointPtr) + 8), (void *)(((int)g_YasuiLimitCatastropheBeamPointPtr) + 0x34), (void *)(((int)g_YasuiLimitCatastropheBeamPointPtr) + 0x38));
    if ((g_YasuiLimitCatastropheBeamPointPtr->flags & 0x60000) == 0) {
        pPacket->primitiveWord = 0x3000000;
        pPacket->color = 0x40FFFFFF;
        Gfx_DrawPsxPrimitive(pPacket);
    }
    if (g_BattleIsPaused == 0) {
        (task->frameIndex)++;
    }
}

/* Advances energy beam impact point smoothly between sequential targets. */
// FUNCTION: FF7 0x0046adbd
int __cdecl BattleFx_LimitCatastropheAdvanceBeamPointAlongTargets(Vec3sPacked *outPoint)
{
    int complement;
    int easeWeight;
    SVECTOR bonePos;

    if (g_YasuiLimitCatastropheAdvanceStage == 0) {
        while ((g_YasuiLimitCatastropheTargetMask & 1 << g_YasuiLimitCatastropheTargetIndex) == 0) {
            g_YasuiLimitCatastropheTargetIndex++;
        }
        Battle_GetBoneWorldPos(g_YasuiLimitCatastropheTargetIndex, g_BattleActorRecords[g_YasuiLimitCatastropheTargetIndex].targetBone, &bonePos.vx);
        outPoint->vx = bonePos.vx + rand() % 0x7D0 + -0x3E8;
        outPoint->vy = 0;
        outPoint->vz = bonePos.vz + rand() % 0x7D0 + -0x3E8;
        g_YasuiLimitCatastropheBeamStep = 0;
        g_YasuiLimitCatastropheAdvanceStage++;
    }
    if (g_YasuiLimitCatastropheBeamStep == 0) {
        (g_YasuiLimitCatastropheTargetActor.actorIndex) = g_YasuiLimitCatastropheTargetIndex;
        g_YasuiLimitCatastropheTargetIndex++;
        if (g_YasuiLimitCatastropheTargetMask < 1 << g_YasuiLimitCatastropheTargetIndex) {
            g_YasuiLimitCatastropheTargetIndex = 0xFFFFFFFF;
        } else {
            while ((g_YasuiLimitCatastropheTargetMask & 1 << g_YasuiLimitCatastropheTargetIndex) == 0) {
                g_YasuiLimitCatastropheTargetIndex++;
            }
        }
        Battle_GetBoneWorldPos((g_YasuiLimitCatastropheTargetActor.actorIndex), g_BattleActorRecords[(g_YasuiLimitCatastropheTargetActor.actorIndex)].targetBone, &bonePos.vx);
        g_YasuiLimitCatastropheDeltaX = bonePos.vx - outPoint->vx;
        g_YasuiLimitCatastropheDeltaY = 0;
        g_YasuiLimitCatastropheDeltaZ = bonePos.vz - outPoint->vz;
        g_YasuiLimitCatastropheBeamStepDelta = SqrtToInt(NormalizeVectorFixed(&g_YasuiLimitCatastropheDeltaX, &g_YasuiLimitCatastropheDeltaX)) / g_YasuiLimitCatastropheDurationFirstHalf;
        g_YasuiLimitCatastropheBeamStep = g_YasuiLimitCatastropheDuration;
    }
    if (g_YasuiLimitCatastropheBeamStep > g_YasuiLimitCatastropheDurationSecondHalf) {
        outPoint->vx += (short)(g_YasuiLimitCatastropheDeltaX * g_YasuiLimitCatastropheBeamStepDelta >> 0xC);
        outPoint->vz += (short)(g_YasuiLimitCatastropheDeltaZ * g_YasuiLimitCatastropheBeamStepDelta >> 0xC);
    } else if (g_YasuiLimitCatastropheTargetIndex != -1) {
        Battle_GetBoneWorldPos(g_YasuiLimitCatastropheTargetIndex, g_BattleActorRecords[g_YasuiLimitCatastropheTargetIndex].targetBone, &bonePos.vx);
        g_YasuiLimitCatastropheTargetVectorX = bonePos.vx - outPoint->vx;
        g_YasuiLimitCatastropheTargetVectorY = 0;
        g_YasuiLimitCatastropheTargetVectorZ = bonePos.vz - outPoint->vz;
        g_YasuiLimitCatastropheHomingSpeed = SqrtToInt(NormalizeVectorFixed(&g_YasuiLimitCatastropheTargetVectorX, &g_YasuiLimitCatastropheTargetVectorX)) / g_YasuiLimitCatastropheDurationSecondHalf;
        easeWeight = (g_YasuiLimitCatastropheBeamStep << 0xC) / g_YasuiLimitCatastropheDurationSecondHalf;
        easeWeight = BattleFx_CatastropheApplyCameraEasing(easeWeight, 0x130);
        complement = 0x1000 - easeWeight;
        outPoint->vx += (short)((g_YasuiLimitCatastropheDeltaX * easeWeight + g_YasuiLimitCatastropheTargetVectorX * complement >> 0xC) * (g_YasuiLimitCatastropheBeamStepDelta * easeWeight + g_YasuiLimitCatastropheHomingSpeed * complement >> 0xC) >> 0xC);
        outPoint->vz += (short)((g_YasuiLimitCatastropheDeltaZ * easeWeight + g_YasuiLimitCatastropheTargetVectorZ * complement >> 0xC) * (g_YasuiLimitCatastropheBeamStepDelta * easeWeight + g_YasuiLimitCatastropheHomingSpeed * complement >> 0xC) >> 0xC);
    } else {
        outPoint->vx += (short)(g_YasuiLimitCatastropheDeltaX * g_YasuiLimitCatastropheBeamStepDelta >> 0xC);
        outPoint->vz += (short)(g_YasuiLimitCatastropheDeltaZ * g_YasuiLimitCatastropheBeamStepDelta >> 0xC);
        g_YasuiLimitCatastropheBeamStep++;
    }
    g_YasuiLimitCatastropheBeamStep--;
    return 0;
}

/* Emits trailing smoke and hit spark particles along beam trajectory. */
// FUNCTION: FF7 0x0046b175
void __cdecl BattleFx_LimitCatastropheSpawnSpriteTrailTask(void)
{
    int angle;
    BattleTaskSlot *pTaskSlot;
    int i;
    int radialSpeed;
    int ratio;
    BattleFxLimitCatastropheTrailSprite *pSprite;
    int pickA;
    int pickB;

    pTaskSlot = BATTLE_CURRENT_TASK_SLOT;
    if ((*(unsigned char *)&g_YasuiLimitCatastropheTrailActive & 1) == 0) {
        g_YasuiLimitCatastropheTrailActive |= 1;
        g_YasuiLimitCatastropheGunPointPtr = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        pSprite = (BattleFxLimitCatastropheTrailSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCatastropheDampedSpriteParticleTask);
        g_YasuiLimitCatastropheGunPointPtr->vx = g_YasuiLimitCatastropheGunAttachPoint.x;
        g_YasuiLimitCatastropheGunPointPtr->vy = g_YasuiLimitCatastropheGunAttachPoint.y;
        g_YasuiLimitCatastropheGunPointPtr->vz = g_YasuiLimitCatastropheGunAttachPoint.z;
        if (pTaskSlot->frameCounter != 0) {
            ratio = (i << 0xC) / 4;
            g_YasuiLimitCatastropheGunPointPtr->vx = (g_YasuiLimitCatastropheActorPos.posX) * ratio + g_YasuiLimitCatastropheGunPointPtr->vx * (0x1000 - ratio) >> 0xC;
            g_YasuiLimitCatastropheGunPointPtr->vy = g_YasuiLimitCatastropheActorPos.posY * ratio + g_YasuiLimitCatastropheGunPointPtr->vy * (0x1000 - ratio) >> 0xC;
            g_YasuiLimitCatastropheGunPointPtr->vz = g_YasuiLimitCatastropheActorOffsetInit * ratio + g_YasuiLimitCatastropheGunPointPtr->vz * (0x1000 - ratio) >> 0xC;
        }
        pSprite->posX = g_YasuiLimitCatastropheGunPointPtr->vx;
        pSprite->posY = g_YasuiLimitCatastropheGunPointPtr->vy;
        pSprite->posZ = g_YasuiLimitCatastropheGunPointPtr->vz;
        if (DEAD_IN_RETAIL != 0) {
            pickA = 0x96;
        } else {
            pickA = rand() % 0x64 + 0x32;
        }
        radialSpeed = pickA;
        angle = rand() & 0xFFF;
        pSprite->radialSin = radialSpeed * SinFixed(angle) >> 0xC;
        pSprite->velocityY = 0;
        pSprite->radialCos = radialSpeed * CosFixed(angle) >> 0xC;
        if (DEAD_IN_RETAIL != 0) {
            pickB = 0x1000;
        } else {
            pickB = rand() % 0x800 + 0x800;
        }
        pSprite->spriteScale = pickB;
    }
    (g_YasuiLimitCatastropheActorPos.posX) = pSprite->posX;
    g_YasuiLimitCatastropheActorPos.posY = pSprite->posY;
    g_YasuiLimitCatastropheActorOffsetInit = pSprite->posZ;
    (pTaskSlot->frameCounter)++;
    if (pTaskSlot->frameCounter >= 0x23) {
        pTaskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Updates physics and rendering for velocity-damped explosion sparks. */
// FUNCTION: FF7 0x0046b40d
void __cdecl BattleFx_LimitCatastropheDampedSpriteParticleTask(void)
{
    BattleFxCatastropheParticle *task;

    task = (BattleFxCatastropheParticle *)BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitCatastropheSpriteDrawReq3.animTable) = Gfx_ModelTableGetLoadedRecord(5, g_YasuiLimitCatastropheResourceHandle.resourceHandle);
    g_YasuiLimitCatastropheSpriteDrawReq3.drawObj = Gfx_ModelTableGetSpriteObject(5, g_YasuiLimitCatastropheResourceHandle.resourceHandle);
    BattleFx_InstallBillboardMatrix((char *)task + 4, (short)task->spriteIndex, 0);
    g_YasuiLimitCatastropheSpriteDrawReq3.limitCatastroSpriteDrawReq3Frame = task->frame;
    g_YasuiLimitCatastropheRenderPacketCursor = BattleFx_SubmitSpriteDraw(&g_YasuiLimitCatastropheSpriteDrawReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitCatastropheRenderPacketCursor);
    if (g_BattleIsPaused == 0) {
        if ((g_YasuiLimitCatastropheSpriteDrawReq3.animTable) != 0) {
            (task->frame)++;
            if (task->frame >= *(int *)((char *)*(int *)(g_YasuiLimitCatastropheSpriteDrawReq3.animTable) + 4)) {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
        task->velocityX = task->velocityX * 0xA00 >> 0xC;
        task->velocityY = (task->velocityY * 0xA00 >> 0xC) - 0xA;
        task->velocityZ = task->velocityZ * 0xA00 >> 0xC;
    }
}

/* Coordinates white screen flash fade envelope during impact climax. */
// FUNCTION: FF7 0x0046b571
void __cdecl BattleFx_LimitCatastropheScreenFlashTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        BattleFx_StartScreenFlashRamp(4);
    }
    if (task->frameCounter == 4) {
        g_BattleFxTimelineState.startValue = 0;
    }
    if (task->frameCounter == 5) {
        BattleFx_FadeOutScreenFlashRamp(5);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/* Polls effect termination condition before releasing combat animation lock. */
// FUNCTION: FF7 0x0046b5f3
void __cdecl BattleFx_LimitCatastropheWaitEffectEndTask(void)
{
    BattleFxIcicleDropRamp *task;

    task = (BattleFxIcicleDropRamp *)BATTLE_CURRENT_TASK_SLOT;
    task->toggle ^= 1;
    if (g_BattleSecondaryTaskActiveCount <= 1) {
        task->ownerSlot = BATTLE_TASK_FREE;
        g_BattleScriptEffectFlag = 0;
    }
}

/* Builds target list sorted by battle slot index for sequential beam sweep. */
// FUNCTION: FF7 0x0046b63a
void __cdecl BattleFx_LimitCatastropheBuildSortedSlotTable(void)
{
    int slot;
    int outCnt;
    int hitIdx;

    outCnt = 0;
    for (slot = 0; slot < 0xA; slot++) {
        for (hitIdx = 0; hitIdx < 0xA; hitIdx++) {
            if (g_BattleHitTargetActors[hitIdx] == slot) {
                g_YasuiLimitCatastropheTargetSlotTable[outCnt].targetSlot = slot;
                g_YasuiLimitCatastropheTargetSlotTable[outCnt].slotIndex = hitIdx;
                outCnt++;
            }
        }
    }
}
} /* extern "C" */
