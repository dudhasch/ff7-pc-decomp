// src/ff7/src/battle/yasui/yasui_special_bizarro_energy_effect.cpp
/* Enemy Special Attack [142]: Bizarro Energy / Stigma (orig. SPECIAL/LASBOSS1) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Lasboss1 special effect: a shared eye and focus camera script
//   with easing and shake, a white screen flash, and a timeline that drives sprite and model
//   particles and audio cues.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/enums/sound.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/codegen.h"
extern "C" {

struct GfxRotationAngles2;
struct BattleFxModelDraw;

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x007e3250
int g_YasuiSpecBizarroEnergyResourceDef = 0x003200B6;
char *g_YasuiSpecialBizarroEnergyEffectResourceNames[1] = {
    // STRING: FF7 0x007e3458
    "LEZER_1.RSD"
};
unsigned int g_YasuiSpecBizarroEnergyResourceDefDataTail[48] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x007e3464
    (unsigned int)"LEZER_2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x007e3470
    (unsigned int)"LIGHT_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x003200b1,
    // STRING: FF7 0x007e347c
    (unsigned int)"SENKOU.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007e3488
    (unsigned int)"SMOKE3.TIM",
    // STRING: FF7 0x007e3494
    (unsigned int)"HIKARI1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007e34a0
    (unsigned int)"PLAZMA_1.TIM",
    // STRING: FF7 0x007e34b0
    (unsigned int)"PLAZMA_1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007e34c0
    (unsigned int)"PLAZMA_2.TIM",
    // STRING: FF7 0x007e34d0
    (unsigned int)"PLAZMA_2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007e34e0
    (unsigned int)"SMOKE3.TIM",
    // STRING: FF7 0x007e34ec
    (unsigned int)"SMOKE2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007e34f8
    (unsigned int)"SMOKE1.TIM",
    // STRING: FF7 0x007e3504
    (unsigned int)"SMOKE1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007e3510
    (unsigned int)"SMOKE2.TIM",
    // STRING: FF7 0x007e351c
    (unsigned int)"SMOKE1B.SPT",
    0x00000000,
    0x00000000
};
// The sliding model's scale and the three zero shorts after it, up to g_YasuiSpecBizarroEnergyDrawMode: 8 bytes
// (VC5 cannot emit the zero run alone).
// GLOBAL: FF7 0x007e3318
short g_YasuiSpecBizarroEnergySlidingModelScale[4] = {12288, 0, 0, 0};
// GLOBAL: FF7 0x007e3320
int g_YasuiSpecBizarroEnergyDrawMode = 129;
// clang-format off
unsigned char g_rawData_007e3324[4] = {0x18, 0xfc, 0x18, 0xfc};
unsigned char g_rawData_007e3328[104] = {
    0x41, 0x00, 0x00, 0x00, 0x30, 0xf8, 0x70, 0x17, 0x80, 0x00, 0x28, 0x00, 0x40, 0x00, 0x28, 0x00, 0x41, 0x00, 0xa0, 0x0f, 0x30, 0xf8, 0xe0, 0x2e,
    0x80, 0x00, 0x1e, 0x00, 0x40, 0x00, 0x1e, 0x00, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00, 0x0f, 0x00, 0x82, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x18, 0xfc,
    0xd0, 0x07, 0x43, 0x00, 0x0a, 0x00, 0x40, 0x1f, 0x18, 0xfc, 0x70, 0x17, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00, 0x0f, 0x00, 0x80, 0x00, 0x13, 0x00,
    0x40, 0x00, 0x13, 0x00, 0x81, 0x00, 0x00, 0x00, 0x0c, 0xfe, 0xe8, 0x03, 0x41, 0x00, 0x40, 0x1f, 0x18, 0xfc, 0xe8, 0x03, 0x80, 0x00, 0x50, 0x00,
    0x40, 0x00, 0x50, 0x00, 0xff, 0xff, 0x00, 0x00
};
// GLOBAL: FF7 0x007e3390
struct YasuiSpecBizarroEnergyDrawReqPool g_YasuiSpecBizarroEnergyDrawReqPool = {0, 0, {0x80, 0x80, 0x80, 0x2c, 0, 0, 0, 0}, 0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, {0x88, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, 0, {0xd4, 0xfe, 0, 0}, 0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x007e33f8
int g_YasuiSpecBizarroEnergyScale = 24576;
unsigned char g_rawData_007e33fc[4] = {0x00, 0x60, 0x00, 0x00};
// clang-format on
unsigned char g_rawData_007e3400[8] = {0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007e3408
YasuiSpecBizarroEnergyModelDrawRequest g_YasuiSpecBizarroEnergyModel3DrawRequest = {0, 0xa8, 0, 0, 0x20, 0};
// clang-format off
// GLOBAL: FF7 0x007e3418
struct YasuiSpecBizarroEnergyModel0Scale g_YasuiSpecBizarroEnergyModel0ScaleVector = {{0, 0x18, 0, 0}, 0x3000, {0, 0x18, 0, 0}};
// clang-format on
// GLOBAL: FF7 0x007e3428
struct GfxRotationAngles2 g_YasuiSpecBizarroEnergyModel0RotationAngles = {1024, 2048, 0};
// GLOBAL: FF7 0x007e3430
YasuiSpecBizarroEnergyModelDrawRequest g_YasuiSpecBizarroEnergyModel0DrawRequest = {0, 0x180, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x007e3440
struct GfxRotationAngles2 g_YasuiSpecBizarroEnergyModel1RotationAngles = {1024, 2048, 0};
// GLOBAL: FF7 0x007e3448
YasuiSpecBizarroEnergyModelDrawRequest g_YasuiSpecBizarroEnergyModel1DrawRequest = {0, 0x180, 0, 0, 0x20, 0};
// The camera-script operand table of the Lasboss 1 effect: BattleFx_BizarroEnergyReadCameraScriptOperand
// reads entry -p - 1 for a negative operand p, so the code names the table 2 bytes before its head.
// 12 entries.
// GLOBAL: FF7 0x00a38aa8
short g_YasuiSpecBizarroEnergyCameraScriptOperandTable[12] = {0};
// GLOBAL: FF7 0x00a38ac0
int g_YasuiSpecBizarroEnergyCameraScriptActive = 0;
unsigned char g_bssPad_00a38ac4[4] = {0};
// GLOBAL: FF7 0x00a38ac8
struct YasuiSpecBizarroEnergyCameraEyeTrack g_YasuiSpecBizarroEnergyCameraEyeTrack = {0};
unsigned char g_bssPad_00a38acc[4] = {0};
unsigned char g_bssPad_00a38ad0[4] = {0};
// GLOBAL: FF7 0x00a38ad4
BattleFxFallMotionParams g_YasuiSpecBizarroEnergyCameraScriptPtr = {0};
// GLOBAL: FF7 0x00a38ad8
int g_YasuiSpecBizarroEnergyCameraScriptDelay = 0;
// GLOBAL: FF7 0x00a38adc
int g_YasuiSpecBizarroEnergyCameraScriptLoop = 0;
unsigned char g_bssPad_00a38ae0[20] = {0};
// GLOBAL: FF7 0x00a38af4
int g_YasuiSpecBizarroEnergyTargetMask = 0;
// GLOBAL: FF7 0x00a38af8
short g_YasuiSpecBizarroEnergyCameraShakeOffset = 0;
// GLOBAL: FF7 0x00a38afc
short g_YasuiSpecBizarroEnergyCameraShakeAmplitude = 0;
// GLOBAL: FF7 0x00a38b00
BattleFxCameraOrbitState *g_YasuiSpecBizarroEnergyCurrentCameraTrack = 0;
// GLOBAL: FF7 0x00a38b04
BattleFxCameraOrbitState *g_YasuiSpecBizarroEnergyOtherCameraTrack = 0;
// GLOBAL: FF7 0x00a38b08
struct YasuiSpecBizarroEnergyCameraFocusTrack g_YasuiSpecBizarroEnergyCameraFocusTrack = {0};
unsigned char g_bssPad_00a38b0c[4] = {0};
unsigned char g_bssPad_00a38b10[4] = {0};
// GLOBAL: FF7 0x00a38b14
struct YasuiSpecBizarroEnergyCameraTargetActor g_YasuiSpecBizarroEnergyCameraTargetActor = {0};
// GLOBAL: FF7 0x00a38b18
struct YasuiSpecBizarroEnergyCameraMatrix g_YasuiSpecBizarroEnergyCameraMatrix = {0};
// GLOBAL: FF7 0x00a38b58
struct GfxTableCursor *g_YasuiSpecBizarroEnergyResourceContext = 0;
unsigned char g_bssPad_00a38b5c[4] = {0};
// GLOBAL: FF7 0x00a38b60
int g_YasuiSpecBizarroEnergyRotationAngles = 0;
unsigned char g_bssPad_00a38b64[4] = {0};
// clang-format on

// clang-format off


/*
 * Loads Lasboss1 special effect resources (SPECIAL/LASBOSS1) into
 * g_YasuiSpecBizarroEnergyResourceContext if not already loaded.
 */
// FUNCTION: FF7 0x00457a90
void __cdecl BattleFx_SpecialLoadResBizarroEnergy(void)
{
    int resourceId;
    char pathBuf[200];
    GfxResourceRequest requestBuf;
    GfxModelTableBank *sceneCtx;
    GfxResourceRequest *requestPtr;
    int header;

    if (g_pBattle3dScene != 0 && g_YasuiSpecBizarroEnergyResourceContext == 0) {
        resourceId = 0xA;
        header = (int)&g_YasuiSpecBizarroEnergyResourceDef;
        sceneCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestBuf;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x007e3528
        requestPtr->resourcePath = BattleFx_BuildResourcePath("SPECIAL/LASBOSS1", pathBuf);
        Gfx_LoadModelTableIfAbsent(resourceId, header, requestPtr, &g_YasuiSpecBizarroEnergyResourceContext, sceneCtx);
    }
}

/*
 * Entry point for the Lasboss1 special effect: sets up effect counter,
 * clears camera shake amplitude, loads resources, and starts the effect.
 */
// FUNCTION: FF7 0x00457b20
void __cdecl BattleFx_SpecialEnterBizarroEnergy(int targetMask, int actorIdx)
{
    g_YasuiSpecBizarroEnergySlidingModelScale[0] = 0x3000;
    g_YasuiSpecBizarroEnergyCameraShakeAmplitude = 0;
    BattleFx_SpecialLoadResBizarroEnergy();
    BattleFx_SpecialBeginBizarroEnergy(targetMask, actorIdx);
}

/*
 * Initializes Lasboss1 effect: stores target mask, computes actor centroid,
 * creates timeline task, sets up camera script and camera shake task.
 */
// FUNCTION: FF7 0x00457b4c
void __cdecl BattleFx_SpecialBeginBizarroEnergy(int targetMask, int actorIdx)
{
    BattleFxBizarroEnergyTimeline *slot;

    g_YasuiSpecBizarroEnergyCameraShakeOffset = targetMask;
    Battle_GetActorMaskCentroid(targetMask, (short *)&g_YasuiSpecBizarroEnergyCameraScriptActive);
    slot = (BattleFxBizarroEnergyTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergyTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone3a, &slot->radius);
    Battle_GetActorMaskCentroid(g_YasuiSpecBizarroEnergyCameraShakeOffset, &slot->centroidX);
    g_YasuiSpecBizarroEnergyCameraMatrix.rotationMatrix.t[1] = 0;
    g_YasuiSpecBizarroEnergyCameraMatrix.rotationMatrix.t[0] = g_YasuiSpecBizarroEnergyCameraMatrix.rotationMatrix.t[1];
    g_YasuiSpecBizarroEnergyCameraMatrix.rotationMatrix.t[2] = 0;
    RotMatrix(&g_YasuiSpecBizarroEnergyRotationAngles, &g_YasuiSpecBizarroEnergyCameraMatrix.rotationMatrix);
    BattleFx_BizarroEnergySpawnCameraScript((short *)&g_YasuiSpecBizarroEnergyDrawMode, &g_YasuiSpecBizarroEnergyCameraMatrix.rotationMatrix, actorIdx);
    Battle_AllocateResourceSlot(BattleFx_SpecialBizarroEnergyCameraShakeTask);
    g_BattleScriptEffectFlag = 1;
}

/*
 * Spawns the camera script execution task for Lasboss1.
 */
// FUNCTION: FF7 0x00457c20
void __cdecl BattleFx_BizarroEnergySpawnCameraScript(short *scriptCursor, PsxMatrix *stateArg, int mode)
{
    BattleFxBizarroEnergyCameraTask *effect;

    effect = (BattleFxBizarroEnergyCameraTask *)BATTLE_NEW_TASK_SLOT(BattleFx_BizarroEnergyRunCameraScript);
    effect->scriptCursor = scriptCursor;
    effect->matrixPtr = stateArg;
    effect->attachPoint = mode;
}

/*
 * Interprets the shared eye/focus camera script for the Lasboss1 effect.
 */
// FUNCTION: FF7 0x00457c60
void __cdecl BattleFx_BizarroEnergyRunCameraScript(void)
{
    int easedVal;
    int projFlags;
    BattleFxBizarroEnergyCameraTask *taskSlot;
    BattleFxSuperNovaCameraDelta *camDelta;
    int trackIndex;
    SVECTOR *vecA;
    Vec3sPacked *scratchBPtr;
    short opcode;

    taskSlot = (BattleFxBizarroEnergyCameraTask *)BATTLE_CURRENT_TASK_SLOT;
    vecA = (SVECTOR *)Gfx_GetPsxScratchpadPtr(0);
    scratchBPtr = (Vec3sPacked *)((int)vecA + 8);
    camDelta = (BattleFxSuperNovaCameraDelta *)((int)scratchBPtr + 8);
    if (g_BattleIsPaused == 0) {
        if (taskSlot->initialized == 0) {
            g_YasuiSpecBizarroEnergyCameraFocusTrack.isReady = 0;
            g_YasuiSpecBizarroEnergyCameraEyeTrack.finished = 0;
            (g_YasuiSpecBizarroEnergyCameraTargetActor.targetActorIndex) = 0xFFFF;
            (g_YasuiSpecBizarroEnergyCameraScriptPtr.actorIndex) = 0xFFFF;
            taskSlot->initialized = 1;
        }
        while (g_YasuiSpecBizarroEnergyCameraFocusTrack.isReady == 0 || g_YasuiSpecBizarroEnergyCameraEyeTrack.finished == 0) {
            opcode = *taskSlot->scriptCursor;
            taskSlot->scriptCursor++;
            if ((opcode & 0x40) != 0) {
                g_YasuiSpecBizarroEnergyCurrentCameraTrack = (BattleFxCameraOrbitState *)&g_YasuiSpecBizarroEnergyCameraFocusTrack;
                g_YasuiSpecBizarroEnergyOtherCameraTrack = (BattleFxCameraOrbitState *)&g_YasuiSpecBizarroEnergyCameraEyeTrack;
            } else {
                g_YasuiSpecBizarroEnergyCurrentCameraTrack = (BattleFxCameraOrbitState *)&g_YasuiSpecBizarroEnergyCameraEyeTrack;
                g_YasuiSpecBizarroEnergyOtherCameraTrack = (BattleFxCameraOrbitState *)&g_YasuiSpecBizarroEnergyCameraFocusTrack;
            }
            g_YasuiSpecBizarroEnergyCurrentCameraTrack->mode = opcode & ~0xC0;
            switch (g_YasuiSpecBizarroEnergyCurrentCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount = BattleFx_BizarroEnergyReadCameraScriptOperand(&taskSlot->scriptCursor);
                break;
            case SUMMON_CAMERA_SET_POSITION:
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->posX = *taskSlot->scriptCursor;
                taskSlot->scriptCursor++;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->posY = *taskSlot->scriptCursor;
                taskSlot->scriptCursor++;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->posZ = *taskSlot->scriptCursor;
                taskSlot->scriptCursor++;
                break;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount = BattleFx_BizarroEnergyReadCameraScriptOperand(&taskSlot->scriptCursor);
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepX = (*taskSlot->scriptCursor - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posX) / g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount;
                taskSlot->scriptCursor++;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepY = (*taskSlot->scriptCursor - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posY) / g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount;
                taskSlot->scriptCursor++;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepZ = (*taskSlot->scriptCursor - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posZ) / g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount;
                taskSlot->scriptCursor++;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                vecA->vx = g_YasuiSpecBizarroEnergyOtherCameraTrack->posX - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posX;
                vecA->vy = g_YasuiSpecBizarroEnergyOtherCameraTrack->posY - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posY;
                vecA->vz = g_YasuiSpecBizarroEnergyOtherCameraTrack->posZ - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posZ;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->progress = SqrtToInt(vecA->vx * vecA->vx + vecA->vy * vecA->vy + vecA->vz * vecA->vz);
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->delta = g_YasuiSpecBizarroEnergyCurrentCameraTrack->pos;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount = BattleFx_BizarroEnergyReadCameraScriptOperand(&taskSlot->scriptCursor);
                vecA->vx = *taskSlot->scriptCursor;
                taskSlot->scriptCursor++;
                vecA->vy = *taskSlot->scriptCursor;
                taskSlot->scriptCursor++;
                vecA->vz = *taskSlot->scriptCursor;
                taskSlot->scriptCursor++;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepX = (vecA->vx - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posX) / g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepY = (vecA->vy - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posY) / g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepZ = (vecA->vz - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posZ) / g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount;
                switch (g_YasuiSpecBizarroEnergyOtherCameraTrack->mode) {
                case SUMMON_CAMERA_MOVE_LINEAR:
                    scratchBPtr->vx = g_YasuiSpecBizarroEnergyOtherCameraTrack->posX + g_YasuiSpecBizarroEnergyOtherCameraTrack->stepX * g_YasuiSpecBizarroEnergyOtherCameraTrack->stepCount;
                    scratchBPtr->vy = g_YasuiSpecBizarroEnergyOtherCameraTrack->posY + g_YasuiSpecBizarroEnergyOtherCameraTrack->stepY * g_YasuiSpecBizarroEnergyOtherCameraTrack->stepCount;
                    scratchBPtr->vz = g_YasuiSpecBizarroEnergyOtherCameraTrack->posZ + g_YasuiSpecBizarroEnergyOtherCameraTrack->stepZ * g_YasuiSpecBizarroEnergyOtherCameraTrack->stepCount;
                    break;
                default:
                    scratchBPtr->vx = g_YasuiSpecBizarroEnergyOtherCameraTrack->posX;
                    scratchBPtr->vy = g_YasuiSpecBizarroEnergyOtherCameraTrack->posY;
                    scratchBPtr->vz = g_YasuiSpecBizarroEnergyOtherCameraTrack->posZ;
                }
                vecA->vx -= scratchBPtr->vx;
                vecA->vy -= scratchBPtr->vy;
                vecA->vz -= scratchBPtr->vz;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepSize = (SqrtToInt(vecA->vx * vecA->vx + vecA->vy * vecA->vy + vecA->vz * vecA->vz) - g_YasuiSpecBizarroEnergyCurrentCameraTrack->progress) / g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount;
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount = BattleFx_BizarroEnergyReadCameraScriptOperand(&taskSlot->scriptCursor);
                vecA->vx = *taskSlot->scriptCursor - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posX;
                taskSlot->scriptCursor++;
                vecA->vy = *taskSlot->scriptCursor - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posY;
                taskSlot->scriptCursor++;
                vecA->vz = *taskSlot->scriptCursor - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posZ;
                taskSlot->scriptCursor++;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaX = (vecA->vx - g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepX * g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount << 1) / (g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount * g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount);
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaY = (vecA->vy - g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepY * g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount << 1) / (g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount * g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount);
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaZ = (vecA->vz - g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepZ * g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount << 1) / (g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount * g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount = BattleFx_BizarroEnergyReadCameraScriptOperand(&taskSlot->scriptCursor);
                g_YasuiSpecBizarroEnergyCurrentCameraTrack[1].mode = *taskSlot->scriptCursor;
                taskSlot->scriptCursor++;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->progress = 0;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepSize = 0x1000 / g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaX = g_YasuiSpecBizarroEnergyCurrentCameraTrack->posX;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaY = g_YasuiSpecBizarroEnergyCurrentCameraTrack->posY;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaZ = g_YasuiSpecBizarroEnergyCurrentCameraTrack->posZ;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepX = *taskSlot->scriptCursor - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posX;
                taskSlot->scriptCursor++;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepY = *taskSlot->scriptCursor - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posY;
                taskSlot->scriptCursor++;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepZ = *taskSlot->scriptCursor - g_YasuiSpecBizarroEnergyCurrentCameraTrack->posZ;
                taskSlot->scriptCursor++;
                break;
            case SUMMON_CAMERA_ATTACH_BONE:
                if ((g_YasuiSpecBizarroEnergyCurrentCameraTrack->attachPoint = *taskSlot->scriptCursor++) == 0) {
                    g_YasuiSpecBizarroEnergyCurrentCameraTrack->attachPoint = taskSlot->attachPoint;
                }
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->attachIndex = *taskSlot->scriptCursor;
                taskSlot->scriptCursor++;
                break;
            case SUMMON_CAMERA_DETACH:
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->attachPoint = 0xFFFF;
                break;
            case SUMMON_CAMERA_WAIT_NO_WRITEBACK:
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount = BattleFx_BizarroEnergyReadCameraScriptOperand(&taskSlot->scriptCursor);
                break;
            case SUMMON_CAMERA_SET_SCRIPT_VALUE:
                g_BattleCameraScriptValue = *taskSlot->scriptCursor;
                taskSlot->scriptCursor++;
                break;
            default:
                taskSlot->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        g_YasuiSpecBizarroEnergyCurrentCameraTrack = (BattleFxCameraOrbitState *)&g_YasuiSpecBizarroEnergyCameraFocusTrack;
        g_YasuiSpecBizarroEnergyOtherCameraTrack = (BattleFxCameraOrbitState *)&g_YasuiSpecBizarroEnergyCameraEyeTrack;
        for (trackIndex = 0; trackIndex < 2; trackIndex++) {
            switch (g_YasuiSpecBizarroEnergyCurrentCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepX += g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaX;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepY += g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaY;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepZ += g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaZ;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->posX += g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepX;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->posY += g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepY;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->posZ += g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepZ;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaX += g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepX;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaY += g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepY;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaZ += g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepZ;
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->progress += g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepSize;
                camDelta->deltaX = g_YasuiSpecBizarroEnergyOtherCameraTrack->posX - g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaX;
                camDelta->deltaY = g_YasuiSpecBizarroEnergyOtherCameraTrack->posY - g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaY;
                camDelta->deltaZ = g_YasuiSpecBizarroEnergyOtherCameraTrack->posZ - g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaZ;
                VectorNormalToSVector((int *)camDelta, (short *)vecA);
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->posX = g_YasuiSpecBizarroEnergyOtherCameraTrack->posX - (vecA->vx * g_YasuiSpecBizarroEnergyCurrentCameraTrack->progress >> 0xC);
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->posY = g_YasuiSpecBizarroEnergyOtherCameraTrack->posY - (vecA->vy * g_YasuiSpecBizarroEnergyCurrentCameraTrack->progress >> 0xC);
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->posZ = g_YasuiSpecBizarroEnergyOtherCameraTrack->posZ - (vecA->vz * g_YasuiSpecBizarroEnergyCurrentCameraTrack->progress >> 0xC);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->progress += g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepSize;
                easedVal = g_YasuiSpecBizarroEnergyCurrentCameraTrack->progress;
                easedVal = BattleFx_BizarroEnergyApplyCameraEasing(easedVal, g_YasuiSpecBizarroEnergyCurrentCameraTrack[1].mode);
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->posX = g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaX + (g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepX * easedVal >> 0xC);
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->posY = g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaY + (g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepY * easedVal >> 0xC);
                g_YasuiSpecBizarroEnergyCurrentCameraTrack->posZ = g_YasuiSpecBizarroEnergyCurrentCameraTrack->deltaZ + (g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepZ * easedVal >> 0xC);
            }
            (g_YasuiSpecBizarroEnergyCurrentCameraTrack->stepCount)--;
            g_YasuiSpecBizarroEnergyCurrentCameraTrack = (BattleFxCameraOrbitState *)&g_YasuiSpecBizarroEnergyCameraEyeTrack;
            g_YasuiSpecBizarroEnergyOtherCameraTrack = (BattleFxCameraOrbitState *)&g_YasuiSpecBizarroEnergyCameraFocusTrack;
        }
    }
    if (taskSlot->matrixPtr != 0) {
        if ((g_YasuiSpecBizarroEnergyCameraFocusTrack.trackStep) != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            SetRotMatrix(taskSlot->matrixPtr);
            SetTransMatrix(taskSlot->matrixPtr);
            if ((g_YasuiSpecBizarroEnergyCameraTargetActor.targetActorIndex) == -1) {
                RotTransPoint(&g_YasuiSpecBizarroEnergyCameraMatrix, (int *)camDelta, &projFlags);
                g_BattleCameraPosition.cameraPosX = *(short *)&camDelta->deltaX;
                g_BattleCameraPosition.cameraPosY = *(short *)&camDelta->deltaY;
                g_BattleCameraPosZ = *(short *)&camDelta->deltaZ;
            } else {
                RotateSVectorByInstalledMatrix(&g_YasuiSpecBizarroEnergyCameraMatrix, camDelta);
                if (g_YasuiSpecBizarroEnergyCameraTargetActor.boneIdx != -1) {
                    Battle_GetBoneWorldPos((g_YasuiSpecBizarroEnergyCameraTargetActor.targetActorIndex), g_YasuiSpecBizarroEnergyCameraTargetActor.boneIdx, &vecA->vx);
                } else {
                    vecA->vx = g_BattleActorRecords[(g_YasuiSpecBizarroEnergyCameraTargetActor.targetActorIndex)].posX;
                    vecA->vy = g_BattleActorRecords[(g_YasuiSpecBizarroEnergyCameraTargetActor.targetActorIndex)].posY;
                    vecA->vz = g_BattleActorRecords[(g_YasuiSpecBizarroEnergyCameraTargetActor.targetActorIndex)].posZ;
                }
                g_BattleCameraPosition.cameraPosX = camDelta->deltaX + vecA->vx;
                g_BattleCameraPosition.cameraPosY = camDelta->deltaY + vecA->vy;
                g_BattleCameraPosZ = camDelta->deltaZ + vecA->vz;
                SetRotMatrix(taskSlot->matrixPtr);
                SetTransMatrix(taskSlot->matrixPtr);
            }
        }
        if ((g_YasuiSpecBizarroEnergyCameraEyeTrack.trackState) != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            if ((g_YasuiSpecBizarroEnergyCameraScriptPtr.actorIndex) == -1) {
                RotTransPoint(&g_YasuiSpecBizarroEnergyCameraScriptDelay, (int *)camDelta, &projFlags);
                g_BattleCameraFocalPoint.focalPointX = *(short *)&camDelta->deltaX;
                g_BattleCameraFocalPoint.focalPointY = *(short *)&camDelta->deltaY;
                g_BattleCameraFocusZ = *(short *)&camDelta->deltaZ;
            } else {
                RotateSVectorByInstalledMatrix(&g_YasuiSpecBizarroEnergyCameraScriptDelay, camDelta);
                if (g_YasuiSpecBizarroEnergyCameraScriptPtr.boneIdx != -1) {
                    Battle_GetBoneWorldPos((g_YasuiSpecBizarroEnergyCameraScriptPtr.actorIndex), g_YasuiSpecBizarroEnergyCameraScriptPtr.boneIdx, &vecA->vx);
                } else {
                    vecA->vx = g_BattleActorRecords[(g_YasuiSpecBizarroEnergyCameraScriptPtr.actorIndex)].posX;
                    vecA->vy = g_BattleActorRecords[(g_YasuiSpecBizarroEnergyCameraScriptPtr.actorIndex)].posY;
                    vecA->vz = g_BattleActorRecords[(g_YasuiSpecBizarroEnergyCameraScriptPtr.actorIndex)].posZ;
                }
                g_BattleCameraFocalPoint.focalPointX = camDelta->deltaX + vecA->vx;
                g_BattleCameraFocalPoint.focalPointY = camDelta->deltaY + vecA->vy;
                g_BattleCameraFocusZ = camDelta->deltaZ + vecA->vz;
            }
        }
    } else {
        if ((g_YasuiSpecBizarroEnergyCameraFocusTrack.trackStep) != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            *(int *)&g_BattleCameraPosition = (g_YasuiSpecBizarroEnergyCameraMatrix.cameraPosition);
            *(int *)&g_BattleCameraPosZ = g_YasuiSpecBizarroEnergyCameraMatrix.lasboss1CameraMatrixValueA;
        }
        if ((g_YasuiSpecBizarroEnergyCameraEyeTrack.trackState) != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            *(int *)&g_BattleCameraFocalPoint = g_YasuiSpecBizarroEnergyCameraScriptDelay;
            *(int *)&g_BattleCameraFocusZ = g_YasuiSpecBizarroEnergyCameraScriptLoop;
        }
    }
}

/*
 * Reads the next 16-bit operand from the camera script cursor, resolving
 * negative values via the operand table.
 */
// FUNCTION: FF7 0x00458b82
short __cdecl BattleFx_BizarroEnergyReadCameraScriptOperand(short **cursor)
{
    int operand;

    operand = **cursor;
    (*cursor)++;
    if (operand < 0) {
        operand = g_YasuiSpecBizarroEnergyCameraScriptOperandTable[-operand - 1];
    }
    return (short)operand;
}

/*
 * Evaluates sine-based easing curve for camera motion interpolation.
 */
// FUNCTION: FF7 0x00458bbc
int __cdecl BattleFx_BizarroEnergyApplyCameraEasing(int progress, int easeRate)
{
    int i;
    int blend;
    int startValue;

    startValue = progress;
    if (easeRate > 0) {
        blend = easeRate >> 8;
        for (i = 0; i < blend; i++) {
            progress = SinFixed(progress / 4);
        }
    } else {
        easeRate = -easeRate;
        blend = easeRate >> 8;
        progress = 0x1000 - progress;
        for (i = 0; i < blend; i++) {
            progress = SinFixed(progress / 4);
        }
        progress = 0x1000 - progress;
    }
    blend = (easeRate & 0xFF) + 1;
    progress = progress * blend + startValue * (0x100 - blend) >> 8;
    return progress;
}

/*
 * Applies camera shake jitter to camera focal point and eye position over 70 frames.
 */
// FUNCTION: FF7 0x00458c9b
void __cdecl BattleFx_SpecialBizarroEnergyCameraShakeTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 0x46) {
        g_YasuiSpecBizarroEnergyCameraShakeAmplitude++;
    }
    g_BattleCameraFocalPoint.focalPointY += -g_YasuiSpecBizarroEnergyCameraShakeAmplitude / 2 + (task->frameCounter % 2) * g_YasuiSpecBizarroEnergyCameraShakeAmplitude;
    g_BattleCameraPosition.cameraPosY += -g_YasuiSpecBizarroEnergyCameraShakeAmplitude / 2 + (task->frameCounter % 2) * g_YasuiSpecBizarroEnergyCameraShakeAmplitude;
    (task->frameCounter)++;
    if (task->frameCounter == 0x46) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Main timeline task driving sprite and model particle spawning and audio cues
 * across 200 frames for Lasboss1.
 */
// FUNCTION: FF7 0x00458d86
void __cdecl BattleFx_SpecialBizarroEnergyTimelineTask(void)
{
    BattleFxBizarroEnergyTimeline *timeline;
    short loopIndex;
    BattleFxBizarroEnergyTimelineRecord *spawned;

    timeline = (BattleFxBizarroEnergyTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (timeline->frame == 0) {
        spawned = (BattleFxBizarroEnergyTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergyScalingModelEmitterTask);
        spawned->radius = timeline->radius;
        spawned->heightOffset = timeline->heightOffset;
        spawned->angle = timeline->angle;
        spawned->lifetime = 0;
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, 0x40, 0x13D, 0, 0, 0, 0, 0, 0);
    }
    if (timeline->frame < 0x46) {
        if (timeline->frame < 0x28) {
            spawned = (BattleFxBizarroEnergyTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergySpiralingSpriteTask);
            spawned->radius = timeline->radius;
            spawned->heightOffset = 0xFE70;
            spawned->angle = timeline->angle;
            spawned->accelY = 0xFFCE;
            spawned->velocityX = rand() % 0x800 - 0x400;
            spawned->lifetime = rand() % 0x3E8 + 0x1F4;
            spawned->scale = 0x3000;
        } else {
            for (loopIndex = 0; loopIndex < 2; loopIndex++) {
                spawned = (BattleFxBizarroEnergyTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergySpiralingSpriteTask);
                spawned->radius = timeline->radius;
                spawned->heightOffset = 0xFE70;
                spawned->angle = timeline->angle;
                spawned->accelY = 0xFFCE;
                spawned->velocityX = rand() % 0x800 - 0x400;
                spawned->lifetime = rand() % 0xBB8 + 0x3E8;
                spawned->scale = 0x4000;
            }
        }
    }
    if (timeline->frame == 0x46) {
        Battle_AllocateResourceSlot(BattleFx_SpecialBizarroEnergyScreenFlashTask);
        spawned = (BattleFxBizarroEnergyTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergySpinningModelTask);
        spawned->radius = timeline->radius;
        spawned->heightOffset = timeline->heightOffset;
        spawned->angle = timeline->angle;
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, 0x40, 0x182, 0, 0, 0, 0, 0, 0);
        SoundDispatchCommand(SOUND_SFX_PLAY_CH2, 0x40, 0x241, 0, 0, 0, 0, 0, 0);
    }
    if (timeline->frame == 0x6E) {
        spawned = (BattleFxBizarroEnergyTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergyGrowingModelTask);
        spawned->radius = timeline->radius;
        spawned->heightOffset = timeline->heightOffset;
        spawned->angle = timeline->angle;
    }
    if (timeline->frame == 0x82) {
        SoundDispatchCommand(SOUND_SFX_PLAY_CH3, 0x40, 0x13E, 0, 0, 0, 0, 0, 0);
    }
    if (timeline->frame >= 0x82 && timeline->frame < 0xBE) {
        if (timeline->frame % 0xA == 0) {
            spawned = (BattleFxBizarroEnergyTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergySlidingModelTask);
            spawned->radius = timeline->radius;
            spawned->heightOffset = timeline->heightOffset;
            spawned->angle = timeline->angle;
            spawned = (BattleFxBizarroEnergyTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergySlidingModelTask);
            spawned->radius = timeline->radius;
            spawned->heightOffset = timeline->heightOffset;
            spawned->angle = timeline->angle;
            spawned->angle -= 0x1B53;
        }
        if (rand() % 2 == 0) {
            spawned = (BattleFxBizarroEnergyTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergyScaledSpriteTask);
            spawned->radius = 0x3E8;
            spawned->heightOffset = timeline->heightOffset + rand() % 0x3E8 + -0x1F4;
            spawned->angle = rand() % 0xFA0 - 0x7D0;
            spawned->rotOrVelX = 0;
            spawned->accelY = 0;
            spawned->velocityY = rand() % 0x400 - 0x600;
        }
        for (loopIndex = 0; loopIndex < 3; loopIndex++) {
            spawned = (BattleFxBizarroEnergyTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergyRisingSpriteTask);
            spawned->radius = rand() % 0x258 + 0x640;
            spawned->angle = rand() % 0xFA0 - 0x3E8;
            spawned->heightOffset = -rand() % 0x190;
            spawned->rotOrVelX = 0;
            spawned->accelY = 0xFF38;
            spawned->velocityY = rand() % 0x64 + 0x190;
        }
        if (timeline->frame >= 0xAA) {
            g_YasuiSpecBizarroEnergySlidingModelScale[0] -= 0x258;
        }
    }
    if (timeline->frame == 0xB4) {
        for (loopIndex = 0; loopIndex < 0xA; loopIndex++) {
            if ((g_YasuiSpecBizarroEnergyCameraShakeOffset & 1 << loopIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(loopIndex);
            }
        }
    }
    (timeline->frame)++;
    if (timeline->frame == 0xC8) {
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, 0x40, 0, 0, 0, 0, 0, 0, 0);
        g_BattleScriptEffectFlag = 0;
        timeline->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Triggers white screen flash with decaying intensity over 60 frames.
 */
// FUNCTION: FF7 0x004593a2
void __cdecl BattleFx_SpecialBizarroEnergyScreenFlashTask(void)
{
    BattleFxLivewireSwarm *taskPtr;
    short redValue;
    short greenBlueValue;

    taskPtr = (BattleFxLivewireSwarm *)BATTLE_CURRENT_TASK_SLOT;
    redValue = 0x3C - taskPtr->frame << 2;
    greenBlueValue = 0x3C - taskPtr->frame * 3 << 2;
    if (greenBlueValue < 0) {
        greenBlueValue = 0;
    }
    g_BattleOverlayDrawListHead = (int)BattleFx_FlashScreenWhite((char)redValue, (char)greenBlueValue, (char)greenBlueValue);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (taskPtr->frame)++;
    if (taskPtr->frame == 0x3C) {
        taskPtr->step = 0xFFFF;
    }
}

/*
 * Renders scaled sprite particle (field 5) with matrix transformation and rotation.
 */
// FUNCTION: FF7 0x00459446
void __cdecl BattleFx_SpecialBizarroEnergyScaledSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        PsxMatrix rotation;
        struct { unsigned char pad_to_20[0x20]; PsxMatrix scale; };
        struct { unsigned char pad_to_40[0x40]; short index; };
        struct { unsigned char pad_to_44[0x44]; int projFlags; };
        struct { unsigned char pad_to_48[0x48]; BattleFxBizarroEnergyScaledSpriteSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxBizarroEnergyScaledSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1ScaledSpriteResource = Gfx_ModelTableGetLoadedRecord(5, g_YasuiSpecBizarroEnergyResourceContext);
    g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1ScaledSpriteResourceB = Gfx_ModelTableGetSpriteObject(5, g_YasuiSpecBizarroEnergyResourceContext);
    for (frame.index = 0; frame.index < 3; frame.index++) {
        frame.scale.m[2][frame.index] = 0;
        frame.scale.m[1][frame.index] = 0;
        frame.scale.m[0][frame.index] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(&frame.slot->posX, frame.scale.t, &frame.projFlags);
    frame.scale.m[2][2] = 0x3000;
    frame.scale.m[1][1] = frame.scale.m[2][2];
    frame.scale.m[0][0] = frame.scale.m[1][1];
    RotMatrix(&frame.slot->rotation, &frame.rotation);
    ConcatMatrixIntoFirst(&frame.scale, &frame.rotation);
    SetRotMatrix(&frame.scale);
    SetTransMatrix(&frame.scale);
    g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1ScaledSpriteFieldWord = frame.slot->frameIndex;
    g_BattleOverlayDrawListHead = (int)BattleFx_SubmitSpriteDraw(&g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1ScaledSpriteResource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, (int)(g_BattleOverlayDrawListHead));
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1ScaledSpriteResource != 0) {
        frame.slot->frameIndex++;
        if (frame.slot->frameIndex == *(int *)(*(int *)g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1ScaledSpriteResource + 4)) {
            frame.slot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Renders scaling billboard model (field 2) and emits drifting/scaled sprites.
 */
// FUNCTION: FF7 0x004595c7
void __cdecl BattleFx_SpecialBizarroEnergyScalingModelEmitterTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        SVECTOR angles;
        struct { unsigned char pad_to_8[0x8]; int projFlags; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix matrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxSpecialEnergyOrbSlot *particle; };
        struct { unsigned char pad_to_30[0x30]; short spawnIndex; };
        struct { unsigned char pad_to_34[0x34]; short spawnAngle; };
        struct { unsigned char pad_to_38[0x38]; BattleFxBizarroEnergyScalingEmitter *emitter; };
        struct { unsigned char pad_to_3C[0x3C]; short spawnRadius; };
        struct { unsigned char pad_to_40[0x40]; short spawnX; };
        struct { unsigned char pad_to_42[0x42]; short spawnY; };
        struct { unsigned char pad_to_44[0x44]; short spawnZ; };
    } frame;
#pragma pack(pop)

    frame.emitter = (BattleFxBizarroEnergyScalingEmitter *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1ScalingModelResource = Gfx_ModelTableGetRenderObject(2, g_YasuiSpecBizarroEnergyResourceContext);
    BattleFx_InstallBillboardMatrix(&frame.emitter->posX, frame.emitter->scale, 0xFFFFFE70);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiSpecBizarroEnergyTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1ScalingModelResource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiSpecBizarroEnergyTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.emitter->timer < 0x3C) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
            frame.particle = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergyDriftingSpriteTask);
            frame.particle->posX = frame.emitter->posX;
            frame.particle->posY = frame.emitter->posY;
            frame.particle->posZ = frame.emitter->posZ;
            frame.angles.vx = rand() % 0x640 - 0x320;
            frame.angles.vy = rand() % 0x640 - 0x320;
            frame.angles.vz = 0;
            RotMatrixZYX(&frame.angles, &frame.matrix);
            frame.matrix.t[2] = 0;
            frame.matrix.t[1] = frame.matrix.t[2];
            frame.matrix.t[0] = frame.matrix.t[1];
            SetRotMatrix(&frame.matrix);
            SetTransMatrix(&frame.matrix);
            RotTransPointToInt16(&g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1ScalingModelPoint, &frame.particle->velocityX, &frame.projFlags);
            frame.particle->posX += (short)(-frame.particle->velocityX * 0xA);
            frame.particle->posY += (short)(-frame.particle->velocityY * 0xA);
            frame.particle->posZ += (short)(-frame.particle->velocityZ * 0xA);
        }
        if (rand() % 4 == 0) {
            frame.spawnAngle = rand() % 0x1000;
            frame.spawnRadius = frame.emitter->scale / 5 + 0xC8;
            frame.spawnX = frame.emitter->posX + (CosFixed(frame.spawnAngle) * frame.spawnRadius >> 0xC);
            frame.spawnY = frame.emitter->posY + (SinFixed(frame.spawnAngle) * frame.spawnRadius >> 0xC);
            frame.spawnZ = frame.emitter->posZ;
            frame.particle = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergyScaledSpriteTask);
            frame.particle->posX = frame.spawnX;
            frame.particle->posY = frame.spawnY;
            frame.particle->posZ = frame.spawnZ;
            frame.particle->velocityX = 0;
            frame.particle->velocityY = 0;
            frame.particle->velocityZ = rand() % 0x320 + 0x670;
            frame.particle = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergyAnimatedSpriteTask);
            frame.particle->posX = frame.spawnX;
            frame.particle->posY = frame.spawnY;
            frame.particle->posZ = frame.spawnZ;
        }
    }
    if (frame.emitter->timer < 0x46) {
        frame.emitter->scale += 0x50;
    } else {
        frame.emitter->scale -= 0x28;
    }
    frame.emitter->timer++;
    if (frame.emitter->timer == 0x6E) {
        frame.emitter->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Renders drifting billboard sprite (field 4) with 3D velocity integration over 10 frames.
 */
// FUNCTION: FF7 0x0045994c
void __cdecl BattleFx_SpecialBizarroEnergyDriftingSpriteTask(void)
{
    BattleFxMeteorModelSlot *task;

    task = (BattleFxMeteorModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1DriftingSpriteAnimBlock) = Gfx_ModelTableGetLoadedRecord(4, g_YasuiSpecBizarroEnergyResourceContext);
    g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1DriftingSpriteResource = Gfx_ModelTableGetSpriteObject(4, g_YasuiSpecBizarroEnergyResourceContext);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x1000, 0);
    g_YasuiSpecBizarroEnergyTargetMask = BattleFx_SubmitSpriteDraw(&g_YasuiSpecBizarroEnergyDrawReqPool, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiSpecBizarroEnergyTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    (task->frameCount)++;
    if (task->frameCount == 0xA) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Renders animated billboard sprite (field 6) until reaching animation frame count.
 */
// FUNCTION: FF7 0x00459a3f
void __cdecl BattleFx_SpecialBizarroEnergyAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1AnimSpriteResourceA = Gfx_ModelTableGetLoadedRecord(6, g_YasuiSpecBizarroEnergyResourceContext);
    g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1AnimSpriteResourceB = Gfx_ModelTableGetSpriteObject(6, g_YasuiSpecBizarroEnergyResourceContext);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2000, 0);
    g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1AnimSpriteFieldWord = task->frameCounter;
    g_YasuiSpecBizarroEnergyTargetMask = BattleFx_SubmitSpriteDraw(&g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1AnimSpriteResourceA, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiSpecBizarroEnergyTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1AnimSpriteResourceA != 0) {
        (task->frameCounter)++;
        if (task->frameCounter == *(int *)((char *)*(int *)g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1AnimSpriteResourceA + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Renders orbiting billboard sprite (field 7) with expanding radius and vertical velocity.
 */
// FUNCTION: FF7 0x00459b12
void __cdecl BattleFx_SpecialBizarroEnergySpiralingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        Vec3sPacked pos;
        struct { unsigned char pad_to_8[0x8]; BattleFxSpiralSprite *sprite; };
    } frame;
#pragma pack(pop)

    frame.sprite = (BattleFxSpiralSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1SpiralSpriteResourceA = Gfx_ModelTableGetLoadedRecord(7, g_YasuiSpecBizarroEnergyResourceContext);
    g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1SpiralSpriteResourceB = Gfx_ModelTableGetSpriteObject(7, g_YasuiSpecBizarroEnergyResourceContext);
    frame.pos.vx = frame.sprite->centerX + (SinFixed(frame.sprite->angle) * frame.sprite->phase >> 0xC);
    frame.pos.vz = frame.sprite->centerY + (CosFixed(frame.sprite->angle) * frame.sprite->phase >> 0xC);
    frame.pos.vy = frame.sprite->radius;
    BattleFx_InstallBillboardMatrix(&frame.pos.vx, frame.sprite->scale, 0);
    g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1SpiralSpriteFieldWord = frame.sprite->frameIndex;
    g_YasuiSpecBizarroEnergyTargetMask = BattleFx_SubmitSpriteDraw(&g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1SpiralSpriteResourceA, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiSpecBizarroEnergyTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.sprite->radius += frame.sprite->radiusVelocity;
    frame.sprite->angle -= 0x40;
    frame.sprite->phase += 0x28;
    frame.sprite->radiusVelocity -= 0x28;
    if (g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1SpiralSpriteResourceA != 0) {
        frame.sprite->frameIndex++;
        if (frame.sprite->frameIndex == *(int *)((char *)*(int *)g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1SpiralSpriteResourceA + 4)) {
            frame.sprite->step = 0xFFFF;
        }
    }
}

/*
 * Renders rising billboard sprite (fields 8 and 9) with upward velocity over 6 frames.
 */
// FUNCTION: FF7 0x00459c95
void __cdecl BattleFx_SpecialBizarroEnergyRisingSpriteTask(void)
{
    BattleFxBizarroEnergyRisingSpriteRecord *task;

    task = (BattleFxBizarroEnergyRisingSpriteRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (task->phase < 2) {
        g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1RisingSpritePhase = task->phase;
        g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1RisingSpriteResourceA = Gfx_ModelTableGetLoadedRecord(8, g_YasuiSpecBizarroEnergyResourceContext);
        g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1RisingSpriteResourceB = Gfx_ModelTableGetSpriteObject(8, g_YasuiSpecBizarroEnergyResourceContext);
    } else {
        g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1RisingSpritePhase = task->phase - 2;
        g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1RisingSpriteResourceA = Gfx_ModelTableGetLoadedRecord(9, g_YasuiSpecBizarroEnergyResourceContext);
        g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1RisingSpriteResourceB = Gfx_ModelTableGetSpriteObject(9, g_YasuiSpecBizarroEnergyResourceContext);
    }
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2000, 0);
    g_YasuiSpecBizarroEnergyTargetMask = BattleFx_SubmitSpriteDraw(&g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1RisingSpriteResourceA, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiSpecBizarroEnergyTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posA += task->velA;
    task->posB += task->velB;
    task->posC += task->velC;
    task->velB -= 0x32;
    if (g_YasuiSpecBizarroEnergyDrawReqPool.lasboss1RisingSpriteResourceA != 0) {
        (task->phase)++;
        if (task->phase == 6) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Renders rotating model (field 3) with scale and translation over 60 frames.
 */
// FUNCTION: FF7 0x00459dfd
void __cdecl BattleFx_SpecialBizarroEnergySpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR angles;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotation; };
        struct { unsigned char pad_to_28[0x28]; BattleFxCosmoMemoryTriSpin *slot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix product; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiSpecBizarroEnergyModel3DrawRequest.modelGroup = Gfx_ModelTableGetRenderObject(3, g_YasuiSpecBizarroEnergyResourceContext);
    frame.angles.vx = frame.slot->step << 8;
    frame.angles.vy = frame.slot->step << 8;
    frame.angles.vz = 0;
    RotMatrix(&frame.angles, &frame.rotation);
    ScaleMatrix(&frame.rotation, &g_YasuiSpecBizarroEnergyScale);
    frame.rotation.t[0] = frame.slot->posX;
    frame.rotation.t[1] = frame.slot->posY;
    frame.rotation.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotation, &frame.product);
    SetRotMatrix(&frame.product);
    SetTransMatrix(&frame.product);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiSpecBizarroEnergyTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiSpecBizarroEnergyModel3DrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiSpecBizarroEnergyTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->step++;
    if (frame.slot->step == 0x3C) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Renders expanding model (field 0) and spawns rising sprite tasks over 20 frames.
 */
// FUNCTION: FF7 0x00459f23
void __cdecl BattleFx_SpecialBizarroEnergyGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        PsxMatrix rotation;
        struct { unsigned char pad_to_20[0x20]; BattleFxPositionedSpriteSlot *spawner; };
        struct { unsigned char pad_to_24[0x24]; BattleFxBizarroEnergyGrowRecord *slot; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix product; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxBizarroEnergyGrowRecord *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiSpecBizarroEnergyModel0DrawRequest.modelGroup = Gfx_ModelTableGetRenderObject(0, g_YasuiSpecBizarroEnergyResourceContext);
    RotMatrix(&g_YasuiSpecBizarroEnergyModel0RotationAngles, &frame.rotation);
    g_YasuiSpecBizarroEnergyModel0ScaleVector.scaleY = frame.slot->growStep * 0x640;
    ScaleMatrix(&frame.rotation, &g_YasuiSpecBizarroEnergyModel0ScaleVector);
    frame.rotation.t[0] = frame.slot->posX;
    frame.rotation.t[1] = frame.slot->posY;
    frame.rotation.t[2] = frame.slot->growAmount;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotation, &frame.product);
    SetRotMatrix(&frame.product);
    SetTransMatrix(&frame.product);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiSpecBizarroEnergyTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiSpecBizarroEnergyModel0DrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiSpecBizarroEnergyTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->growStep < 0xF) {
        frame.spawner = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergySpawnRisingSpritesTask);
        frame.spawner->posX = 0x7D0;
        frame.spawner->posZ = frame.slot->growAmount + frame.slot->growStep * 0x320;
        frame.spawner->posY = 0;
    }
    frame.slot->growStep++;
    if (frame.slot->growStep == 0x14) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Spawner task generating randomized rising sprites on even frames over 10 frames.
 */
// FUNCTION: FF7 0x0045a091
void __cdecl BattleFx_SpecialBizarroEnergySpawnRisingSpritesTask(void)
{
    BattleFxBizarroEnergySpawnState *spawnerPtr;
    BattleFxRisingSpriteSlot *sprite;

    spawnerPtr = (BattleFxBizarroEnergySpawnState *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (spawnerPtr->timer % 2 == 0) {
        sprite = (BattleFxRisingSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBizarroEnergyRisingSpriteTask);
        sprite->posX = spawnerPtr->posX;
        sprite->posZ = spawnerPtr->posZ + rand() % 0x320 - 0x190;
        sprite->posY = -rand() % 0x190;
        sprite->velocityX = 0x64;
        sprite->velocityY = 0xFE70;
        sprite->velocityZ = -0x190 - rand() % 0xC8;
    }
    (spawnerPtr->timer)++;
    if (spawnerPtr->timer == 0xA) {
        spawnerPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Renders sliding model (field 1) moving along the Z axis over 10 frames.
 */
// FUNCTION: FF7 0x0045a18f
void __cdecl BattleFx_SpecialBizarroEnergySlidingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotation;
        struct { unsigned char pad_to_20[0x20]; YasuiBizarroEnergySlideState *slot; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix product; };
        struct { unsigned char pad_to_44[0x44]; int scale[3]; };
    } frame;
#pragma pack(pop)

    frame.slot = (YasuiBizarroEnergySlideState *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiSpecBizarroEnergyModel1DrawRequest.modelGroup = Gfx_ModelTableGetRenderObject(1, g_YasuiSpecBizarroEnergyResourceContext);
    RotMatrix(&g_YasuiSpecBizarroEnergyModel1RotationAngles, &frame.rotation);
    frame.scale[1] = 0x5000;
    frame.scale[2] = g_YasuiSpecBizarroEnergySlidingModelScale[0];
    frame.scale[0] = frame.scale[2];
    ScaleMatrix(&frame.rotation, frame.scale);
    frame.rotation.t[0] = frame.slot->posX;
    frame.rotation.t[1] = frame.slot->posY;
    frame.rotation.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotation, &frame.product);
    SetRotMatrix(&frame.product);
    SetTransMatrix(&frame.product);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiSpecBizarroEnergyTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiSpecBizarroEnergyModel1DrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiSpecBizarroEnergyTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->posZ += 0x2BA;
    frame.slot->frameCounter++;
    if (frame.slot->frameCounter == 0xA) {
        frame.slot->endMarker = 0xFFFF;
    }
}
} /* extern "C" */
