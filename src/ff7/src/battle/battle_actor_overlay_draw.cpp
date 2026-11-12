// src/ff7/src/battle/battle_actor_overlay_draw.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle actor overlays and status poses: draws targeting markers, status icons with
//   countdown digits, damage numbers, spinning pointers and screen quads over actors. It also runs
//   the status pose and death-sink animation tasks, updates actor bone matrices, and drives the
//   rain, screen fades and message-window queue.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/enums/battle.h"
#include "lib/codegen.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "ff7/types/battle_actors.h"
#include "lib/pfile.h"
#include "ff7/types/menu.h"
#include "lib/types/graphics.h"
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "lib/api/polygon.h"
#include "lib/api/sound.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/savemap.h"
#include "ff7/enums/main.h"
#include "lib/enums/graphics.h"
extern "C" {

struct GfxPsxSpriteQuadState;

// clang-format off
// GLOBAL: FF7 0x008fe110
VECTOR g_BattleOverlayVertexOffset = {0, -0x1000, 0, 0};
// GLOBAL: FF7 0x008fe120
unsigned char g_BattleOverlay_Scale[16] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fe130
PsxMatrix g_BattleOverlayIdentityMatrix = {{{0x1000, 0, 0}, {0, 0x1000, 0}, {0, 0, 0x1000}}, {0, 0, 0}};
// GLOBAL: FF7 0x008fe150
struct BattleStatusTintEntry g_BattleOverlay_StatusTintTable[11] = {{0}, {1, 0, -1, 0}, {1, -1, 112, -83}, {1, -1, 0, 0}, {1, 94, 53, 16}, {1, -113, -113, -113}, {1, -1, -1, 0}, {1, 0, -1, -1}, {1, 0, 0, 0}, {1, -16, 96, 20}, {1, -113, -113, -113}};
// The corners of an actor overlay sprite (Battle3d_DrawActorOverlayQuad), around its anchor.
// GLOBAL: FF7 0x008fe180
SVECTOR g_BattleOverlay_QuadOffset0 = {-64, -64, 0, 0};
// GLOBAL: FF7 0x008fe188
SVECTOR g_BattleOverlay_QuadOffset1 = {64, -64, 0, 0};
// GLOBAL: FF7 0x008fe190
SVECTOR g_BattleOverlay_QuadOffset2 = {-64, 64, 0, 0};
// GLOBAL: FF7 0x008fe198
SVECTOR g_BattleOverlay_QuadOffset3 = {64, 64, 0, 0};
// GLOBAL: FF7 0x008fe1a0
unsigned char g_BattleOverlay_QuadTexCoord[8] = {0x40, 0x50, 0x60, 0x70, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fe1a8
unsigned char g_BattleOverlay_QuadOffset4[8] = {0xc0, 0xff, 0xc0, 0xff, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fe1b0
unsigned char g_BattleOverlay_QuadOffset5[8] = {0x00, 0x00, 0xc0, 0xff, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fe1b8
unsigned char g_BattleOverlay_QuadOffset6[8] = {0xc0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fe1c0
unsigned char g_BattleOverlay_DigitQuadData[38] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0xc0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};
// The overlay animation curves: the damage number's rise per frame (Battle_DamageNumberTask, indexed
// by its rise step), then the death sink's drop per frame (Battle_OverlayTaskDeathSinkUpdate, indexed
// by its countdown).
typedef struct {
    char damageRise[14];
    short deathSinkStep[15];
} BattleOverlayCurves;
// GLOBAL: FF7 0x008fe1e8
BattleOverlayCurves g_BattleOverlayCurves = {{0, 2, 4, 6, 7, 8, 8, 7, 6, 4, 2, 0, 0, 0}, {0, -160, -160, -160, -150, -150, -150, -140, -140, -140, -130, -130, -120, -110, -100}};
// The corner alphas of the screen band wipe's bands (BattleFx_DrawScreenGradientBand).
// GLOBAL: FF7 0x008fe214
BattleFxGradientBandColor g_BattleBandAlphaFadeDown = {0xFF, 0};
// GLOBAL: FF7 0x008fe218
BattleFxGradientBandColor g_BattleBandAlphaOpaque = {0xFF, 0xFF};
// GLOBAL: FF7 0x008fe21c
BattleFxGradientBandColor g_BattleBandAlphaFadeUp = {0, 0xFF};
// The corners of a rain splash (Battle3d_DrawRainSplash), read as PsxSVectors: each corner's z is
// the word after it.
// GLOBAL: FF7 0x008fe220
unsigned char g_BattleRainSplashCorner0[4] = {0x80, 0xff, 0x00, 0x00};
unsigned char g_rawData_008fe224[4] = {0x80, 0xff, 0x00, 0x00};
// GLOBAL: FF7 0x008fe228
unsigned char g_BattleRainSplashCorner1[4] = {0x80, 0x00, 0x00, 0x00};
unsigned char g_rawData_008fe22c[4] = {0x80, 0xff, 0x00, 0x00};
// GLOBAL: FF7 0x008fe230
unsigned char g_BattleRainSplashCorner2[4] = {0x80, 0xff, 0x00, 0x00};
unsigned char g_rawData_008fe234[4] = {0x80, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fe238
unsigned char g_BattleRainSplashCorner3[4] = {0x80, 0x00, 0x00, 0x00};
unsigned char g_rawData_008fe23c[4] = {0x80, 0x00, 0x00, 0x00};
// The top and bottom of a rain streak (Battle3d_DrawRainStreak).
// GLOBAL: FF7 0x008fe240
SVECTOR g_BattleRainStreakEnds[2] = {{0, -600, 0, 0}, {0, 0, 0, 0}};
// GLOBAL: FF7 0x00bf1eb8
BattleDisplayTextEntry g_BattleDisplayTextQueue[64] = {0};
// GLOBAL: FF7 0x00bf2038
int g_BattleEnemyModelIds[2] = {0};
// GLOBAL: FF7 0x00bf2040
char g_BattleEnemyTypeCount = 0;
// GLOBAL: FF7 0x00bf2044
int g_BattleEnemyBankModelId0 = 0;
// GLOBAL: FF7 0x00bf2048
int g_BattleEnemyBankModelId1 = 0;
// GLOBAL: FF7 0x00bf204c
int g_BattleEnemyBankModelId2 = 0;
// GLOBAL: FF7 0x00bf2050
char g_BattleEnemyCount = 0;
// GLOBAL: FF7 0x00bf2054
int g_BattleEnemySlots = 0;
// GLOBAL: FF7 0x00bf2058
unsigned char g_BattleFormationEntries[68] = {0};
unsigned char g_bssPad_00bf209c[4] = {0};
unsigned char g_bssPad_00bf20a0[124] = {0};
// GLOBAL: FF7 0x00bf211c
char g_BattleCameraScriptIndex = 0;
// GLOBAL: FF7 0x00bf2120
int g_BattleOverlay_Word = 0;
unsigned char g_bssPad_00bf2124[4] = {0};
// GLOBAL: FF7 0x00bf2128
char g_BattleSequenceStepReady = 0;
unsigned char g_bssPad_00bf212c[4] = {0};
// GLOBAL: FF7 0x00bf2130
PsxMatrix g_BattleCameraLookAt = {0};
// GLOBAL: FF7 0x00bf2150
char g_BattleScriptAnimRunFlag = 0;
// GLOBAL: FF7 0x00bf2154
short g_BattleOverlay_Short = 0;
// GLOBAL: FF7 0x00bf2158
BattleCameraPosition g_BattleCameraPosition = {0};
// GLOBAL: FF7 0x00bf215c
short g_BattleCameraPosZ = 0;
unsigned char g_bssPad_00bf2160[4] = {0};
// GLOBAL: FF7 0x00bf2164
char g_BattleLostEndState = 0;
// GLOBAL: FF7 0x00bf2168
struct Battle3dNode g_BattleStageNodes[10] = {0};
// GLOBAL: FF7 0x00bf2370
char g_BattleTargetFacingLocked = 0;
unsigned char g_bssPad_00bf2374[4] = {0};
// GLOBAL: FF7 0x00bf2378
BattleTaskFn g_BattleAuxTaskCallbackArray[16] = {0};
// GLOBAL: FF7 0x00bf23b8
short g_BattleActiveTaskSlotIdx = 0;
// GLOBAL: FF7 0x00bf23bc
char g_BattleHitReactionNoNumbers = 0;
// GLOBAL: FF7 0x00bf23c0
struct BattleActorState g_BattleActorStates[10] = {0};
// GLOBAL: FF7 0x00bf2848
int g_BattleDrawFrameParity = 0;
// GLOBAL: FF7 0x00bf284c
char g_BattleDebugDumpByte0 = 0;
// GLOBAL: FF7 0x00bf2850
char g_BattleDebugDumpByte1 = 0;
unsigned char g_bssPad_00bf2854[4] = {0};
// The main task callbacks: Battle_AllocateResourceSlot and Battle_RunMainTaskSlots index 100 slots (slot < 100).
// This object holds slots 0..98; slot 99 is g_BattleCameraScriptTaskCallback, defined next.
// GLOBAL: FF7 0x00bf2858
BattleTaskFn g_BattleMainTaskCallbacks[99] = {0};
// GLOBAL: FF7 0x00bf29e4
void (__cdecl *g_BattleCameraScriptTaskCallback)(void) = 0;
unsigned char g_bssPad_00bf29e8[64] = {0};
// GLOBAL: FF7 0x00bf2a28
char g_BattleDebugDumpByte2 = 0;
// GLOBAL: FF7 0x00bf2a2c
char g_BattleScriptBusy = 0;
// GLOBAL: FF7 0x00bf2a30
char g_EngineIsEffectLoading = 0;
// GLOBAL: FF7 0x00bf2a34
char g_BattleCameraFocusActor = 0;
// GLOBAL: FF7 0x00bf2a38
char g_BattleSequenceStepIndex = 0;
unsigned char g_bssPad_00bf2a3c[4] = {0};
// GLOBAL: FF7 0x00bf2a40
struct BattleHitReactionRecord g_BattleHitReactionRecords[5] = {0};
unsigned char g_bssPad_00bf2a7c[4] = {0};
unsigned char g_bssPad_00bf2a80[872] = {0};
// GLOBAL: FF7 0x00bf2de8
char g_BattleScreenRampActive = 0;
// GLOBAL: FF7 0x00bf2dec
char g_BattleEnemyBankLoadStep = 0;
// GLOBAL: FF7 0x00bf2df0
short g_BattleEffectActorMask = 0;
// GLOBAL: FF7 0x00bf2df4
short g_BattleSecondaryTaskActiveSlotIdx = 0;
// GLOBAL: FF7 0x00bf2df8
unsigned char g_BattleActorIdleScripts[10] = {0};
// GLOBAL: FF7 0x00bf2e04
char g_BattleRoundSequenceState = 0;
// GLOBAL: FF7 0x00bf2e08
unsigned short g_BattleSceneFlags = 0;
// GLOBAL: FF7 0x00bf2e0c
unsigned char g_BattlePartyPlaceStep = 0;
// GLOBAL: FF7 0x00bf2e10
struct BattlePartyModelLoadSlot g_BattlePartyModelLoadSlots[2] = {0};
// GLOBAL: FF7 0x00bf2e18
BattleOverlayRootLoadState g_BattleOverlayRootLoadState = {0};
// GLOBAL: FF7 0x00bf2e1c
char g_BattleSequenceStepBusy = 0;
// GLOBAL: FF7 0x00bf2e20
BattleActorScreenPos g_BattleActorScreenPositions[10] = {0};
// GLOBAL: FF7 0x00bf2e70
struct BattleOverlayTaskSlot g_BattleOverlayTaskSlotBuffer[10] = {0};
// GLOBAL: FF7 0x00bf2fb0
int g_BattleOrderingTableA = 0;
unsigned char g_bssPad_00bf2fb4[4] = {0};
unsigned char g_bssPad_00bf2fb8[16620] = {0};
// GLOBAL: FF7 0x00bf70a4
int g_BattleOrderingTableB = 0;
unsigned char g_bssPad_00bf70a8[16624] = {0};
// GLOBAL: FF7 0x00bfb198
char g_BattleSavedSpecialActorId = 0;
unsigned char g_bssPad_00bfb19c[4] = {0};
// GLOBAL: FF7 0x00bfb1a0
BattleCameraFocalPoint g_BattleCameraFocalPoint = {0};
// GLOBAL: FF7 0x00bfb1a4
short g_BattleCameraFocusZ = 0;
// GLOBAL: FF7 0x00bfb1a8
short g_BattleNewTargetMask = 0;
unsigned char g_bssPad_00bfb1ac[4] = {0};
// GLOBAL: FF7 0x00bfb1b0
BattleTaskFn g_BattleSecondaryTaskCallbacks[60] = {0};
// GLOBAL: FF7 0x00bfb2a0
char g_BattleLimitBladeBeamTargetDiffers = 0;
unsigned char g_bssPad_00bfb2a4[4] = {0};
// GLOBAL: FF7 0x00bfb2a8
int g_BattlePartyStatusIconMasks[3] = {0};
unsigned char g_bssPad_00bfb2b4[4] = {0};
// GLOBAL: FF7 0x00bfb2b8
int g_BattleModelBankSlots[7] = {0};
// The grey levels of the screen colour ramp (Battle_EffectTaskScreenColourRamp), passed to
// BattleFx_DarkenScreenGlobalAlpha as bytes; the first channel is g_BattleRainActiveCount.
typedef struct {
    short level;
    unsigned char pad_02[0x2];
} BattleScreenRampLevel;
// GLOBAL: FF7 0x00bfb2d4
BattleScreenRampLevel g_BattleScreenRampBlue = {0};
// GLOBAL: FF7 0x00bfb2d8
BattleScreenRampLevel g_BattleScreenRampGreen = {0};
// GLOBAL: FF7 0x00bfb2dc
char g_BattleCameraIsMoving = 0;
// GLOBAL: FF7 0x00bfb2e0
int g_BattleOverlayDrawListHead = 0;
// GLOBAL: FF7 0x00bfb2e4
char g_BattleHitReactionInitZero = 0;
// GLOBAL: FF7 0x00bfb2e8
short g_BattleRainActiveCount = 0;
// GLOBAL: FF7 0x00bfb2ec
char g_BattleScriptEffectFlag = 0;
unsigned char g_bssPad_00bfb2f0[1032] = {0};
// GLOBAL: FF7 0x00bfb6f8
struct BattleTaskSlotBuffer g_BattleTaskSlotBuffer = {0};
// GLOBAL: FF7 0x00bfcb20
short g_BattleActiveTargetMask = 0;
unsigned char g_bssPad_00bfcb24[4] = {0};
// GLOBAL: FF7 0x00bfcb28
char g_BattleCurrFormationCameraIdx = 0;
// GLOBAL: FF7 0x00bfcb2c
short g_BattleEffectTrail_StateShort = 0;
// GLOBAL: FF7 0x00bfcb30
struct Battle3dNode g_BattleCameraTransform = {0};
// GLOBAL: FF7 0x00bfcb64
char g_BattleStageHideFlags = 0;
// GLOBAL: FF7 0x00bfcb68
char g_BattleSequenceActorIndex = 0;
unsigned char g_bssPad_00bfcb6c[4] = {0};
// GLOBAL: FF7 0x00bfcb70
struct BattleHitDisplayEntry g_BattleHitDisplayTable[78] = {0};
// GLOBAL: FF7 0x00bfcde0
char g_BattleSpecialActorId = 0;
// GLOBAL: FF7 0x00bfcde4
char g_BattleHitReactionCount = 0;
// GLOBAL: FF7 0x00bfcde8
char g_BattleSlot3ActorState = 0;
// GLOBAL: FF7 0x00bfcdec
unsigned char g_BattleStageNodeState[4] = {0};
// g_BattleScriptFlags is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0xbfcdf0..0xbfcdf8 are aliases.
// GLOBAL: FF7 0x00bfcdf0
unsigned char g_BattleScriptFlags[8] = {0};
// GLOBAL: FF7 0x00bfcdf8
char g_BattleEscapedEndState = 0;
// GLOBAL: FF7 0x00bfcdfc
signed char g_BattleSceneState = 0;
// GLOBAL: FF7 0x00bfce00
short g_BattleEffectTrail_ShortVector[3] = {0};
// GLOBAL: FF7 0x00bfce08
struct BattleAuxTaskSlot g_BattleAuxTaskSlotBuffer[16] = {0};
// GLOBAL: FF7 0x00bfd088
char g_BattleHitDisplayCount = 0;
// GLOBAL: FF7 0x00bfd08c
short g_BattleTimelineTargetMask = 0;
// GLOBAL: FF7 0x00bfd090
short g_BattleEffectTargetMask = 0;
// GLOBAL: FF7 0x00bfd094
char g_Battle3dPendingActorEffect = 0;
// GLOBAL: FF7 0x00bfd098
short g_BattleAuxTaskActiveCount = 0;
unsigned char g_bssPad_00bfd09c[4] = {0};
// GLOBAL: FF7 0x00bfd0a0
struct BattleActorSpawnPos g_BattleActorSpawnPos[10] = {0};
// GLOBAL: FF7 0x00bfd0dc
char g_BattleHitReactionActor = 0;
// GLOBAL: FF7 0x00bfd0e0
unsigned char g_BattleLayout = BATTLE_LAYOUT_NORMAL;
// GLOBAL: FF7 0x00bfd0e4
char g_Battle3dFrameTick = 0;
// GLOBAL: FF7 0x00bfd0e8
short g_BattleOverlayTaskActiveCount = 0;
// GLOBAL: FF7 0x00bfd0ec
int g_BattleEffectEnterResult = 0;
// GLOBAL: FF7 0x00bfd0f0
char g_BattleScriptWaitFrames = 0;
// GLOBAL: FF7 0x00bfd0f4
char g_BattleEnterFramesToWait = 0;
// GLOBAL: FF7 0x00bfd0f8
BattleEffectScratchHead g_BattleEffectScratch = {0};
// GLOBAL: FF7 0x00c05e68
unsigned char g_BattleHitTargetActors[78] = {0};
// GLOBAL: FF7 0x00c05eb8
short g_BattleCameraTargetIndex = 0;
// GLOBAL: FF7 0x00c05ebc
short g_BattleSecondaryTaskActiveCount = 0;
// GLOBAL: FF7 0x00c05ec0
short g_BattleOverlayTaskSlotIdx = 0;
unsigned char g_bssPad_00c05ec4[4] = {0};
// GLOBAL: FF7 0x00c05ec8
struct BattleEffectTrailState g_BattleFxTimelineState = {0};
// GLOBAL: FF7 0x00c05f6c
char g_BattleOpeningUiMode = 0;
// GLOBAL: FF7 0x00c05f70
short g_BattleHurtMessageActorMask = 0;
// GLOBAL: FF7 0x00c05f74
short g_Battle3dReadyActorMask = 0;
// GLOBAL: FF7 0x00c05f78
char g_BattleVariationIndex = 0;
// GLOBAL: FF7 0x00c05f7c
short g_BattleCameraScriptValue = 0;
// GLOBAL: FF7 0x00c05f80
char g_BattleAnimStaggerCounter = 0;
unsigned char g_bssPad_00c05f84[4] = {0};
// GLOBAL: FF7 0x00c05f88
BattleDamageNumberPos g_BattleDamageNumberPos = {0};
// GLOBAL: FF7 0x00c05f8c
short g_BattleDamageNumberPosZ = 0;
// GLOBAL: FF7 0x00c05f90
short g_BattleDamageNumberDigits[4] = {0};
// GLOBAL: FF7 0x00c05f98
short g_BattleOverlaySpritePos[3] = {0};
// GLOBAL: FF7 0x00c05fa0
short g_BattleStatusIconPos[3] = {0};
// GLOBAL: FF7 0x00c05fa8
short g_BattleStatusIconDigits[4] = {0};
// GLOBAL: FF7 0x00c05fb0
short g_BattleOverlaySpriteCount = 0;
unsigned char g_bssPad_00c05fb4[4] = {0};
// clang-format on

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

struct Battle3dScene;

// The battle VM's register slots: references (pointers) to their storage words, defined in
// battle_vm_register_slots.cpp. g_BattleVmReg_0 is register 0.

/*
 * Draws 3D targeting indicators, status icons, and countdown overlays for active battle actors.
 */
// FUNCTION: FF7 0x005b9b30
void __cdecl Battle3d_DrawActorOverlays(char actorIndex)
{
    if (actorIndex != 3) {
        if ((*(unsigned char *)&g_BattleActorRecords[actorIndex].drawFlags & 4) == 0) {
            if ((g_BattleScene.actors[actorIndex].statusFlags & BATTLE_STATUS_DEATH_SENTENCE) != 0) {
                Battle3d_DrawActorStatusIcon((char)actorIndex, (char)g_BattleScene.actors[actorIndex].deathSentenceTimer);
            } else if ((g_BattleScene.actors[actorIndex].statusFlags & BATTLE_STATUS_SLOW_NUMB) != 0) {
                Battle3d_DrawActorStatusIcon((char)actorIndex, (char)g_BattleScene.actors[actorIndex].slowNumbTimer);
            } else if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_SLEEP) != 0) {
                Battle3d_DrawActorOverlaySprite((char)actorIndex, 0xE0);
            } else if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_SILENCE) != 0) {
                Battle3d_DrawActorOverlaySprite((char)actorIndex, 0xEF);
            }
        }
        if ((*(unsigned char *)&g_BattleActorRecords[actorIndex].drawFlags & 1) == 0) {
            if ((short)Battle_GetWindowState(1) == 2 && g_BtlActiveActor == actorIndex) {
                Battle3d_DrawActorSpinningIndicator((char)actorIndex);
            }
            if ((short)Battle_GetWindowState(19) == 2 && g_BtlEnemyIndex + 4 == actorIndex) {
                Battle3d_DrawActorSpinningIndicator((char)actorIndex);
            }
        }
    }
}

/*
 * Calculates rotation angles and submits spinning pointer indicator mesh above target actor.
 */
// FUNCTION: FF7 0x005b9c8e
void __cdecl Battle3d_DrawActorSpinningIndicator(char actorIndex)
{
    struct {
        float translation[4][4];
        float result[4][4];
        float intermediate[4][4];
        float rotation[4][4];
        int point;
        int scratch[4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if ((*(unsigned char *)&g_BattleActorRecords[actorIndex].drawFlags & 4) == 0) {
        frame.point = (int)frame.scratch;
        g_BattleActorRecords[actorIndex].savedPosY = g_BattleActorStates[actorIndex].posY;
        if (g_BattleIsPaused == 0)
            g_BattleActorRecords[actorIndex].spinAngle += 0x80;
        if (g_BattleActorRecords[actorIndex].spinAngle > 0x1000)
            g_BattleActorRecords[actorIndex].spinAngle -= 0x1000 /* 360 deg */;
        RotMatrix((SVECTOR *)&g_BattleActorRecords[actorIndex].boneNodes5[0x90E], (int *)&g_BattleActorRecords[actorIndex].boneNodes5[0x8F0]);
        if (Gfx_IsDrawEnabled()) {
            SetIdentityMatrix4x4(frame.translation);
            frame.translation[3][0] = (float)g_BattleActorRecords[actorIndex].posX;
            frame.translation[3][1] = (float)g_BattleActorStates[actorIndex].posY;
            frame.translation[3][2] = (float)g_BattleActorRecords[actorIndex].posZ;
            BuildRotationYRowVec((float)g_BattleActorRecords[actorIndex].spinAngle * 0.087890625f, frame.rotation);
            ClearMatrixColumn4((void *)frame.intermediate);
            ClearMatrixColumn4((void *)frame.result);
            ConcatAffineMatrix4x4(frame.rotation, frame.translation, frame.intermediate);
            ConcatAffineMatrix4x4(frame.intermediate, g_BattleActorRecords[actorIndex].worldMatrix, frame.result);
            Gfx_SetModelMatrix((int)frame.result, g_pBattle3dScene->markModel, GetEngineContext());
            Gfx_DrawMeshPolygonSet(g_pBattle3dScene->markModel, (int)GetEngineContext());
        }
    }
}

/*
 * Updates render states, blend modes, and color tinting parameters for a battle actor.
 */
// FUNCTION: FF7 0x005b9ec2
void __cdecl Battle3d_UpdateActorRenderState(char actorIndex)
{
    if ((g_BattleActorRecords[actorIndex].stateFlags & BATTLE_ACTOR_RECORD_MODEL_REFRESH) != 0) {
        if (actorIndex < 4) {
            g_BattlePartyStatusIconMasks[actorIndex] = g_BattleScene.actors[actorIndex].savedStatus;
        }
        Battle_ApplyFrogModelSwap((char)actorIndex);
        Battle3d_SyncMiniScaleState((char)actorIndex);
        Battle3d_ApplyStatusTint(actorIndex, Battle3d_SelectStatusTintIndex((char)actorIndex));
        g_BattleActorRecords[actorIndex].stateFlags = g_BattleActorRecords[actorIndex].stateFlags & 0xFFFFFFDF;
    }
    if (*(unsigned char *)&g_BattleActorStates[actorIndex].hidden != 0) {
        return;
    }
    if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_PETRIFY) != 0) {
        g_BattleActorStates[actorIndex].motionState = BATTLE_ACTOR_MOTION_FROZEN;
    } else {
        g_BattleActorStates[actorIndex].motionState = BATTLE_ACTOR_MOTION_NORMAL;
        if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_HASTE) != 0) {
            g_BattleActorStates[actorIndex].motionState = BATTLE_ACTOR_MOTION_HASTE;
        }
        if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_SLOW) != 0) {
            g_BattleActorStates[actorIndex].motionState = BATTLE_ACTOR_MOTION_SLOW;
        }
        if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_STOP) != 0) {
            g_BattleActorStates[actorIndex].motionState = BATTLE_ACTOR_MOTION_FROZEN;
        }
        if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_PARALYSIS) != 0) {
            g_BattleActorStates[actorIndex].motionState = BATTLE_ACTOR_MOTION_FROZEN;
        }
        if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_CONFUSION) != 0 && g_BattleActorRecords[actorIndex].scriptId == *(char *)((char *)&g_BattleActorIdleScripts + actorIndex) && g_BattleIsPaused == 0) {
            g_BattleActorRecords[actorIndex].yaw += 0x100 /* 22.5 deg */;
        }
        if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_MANIPULATE) != 0 && g_BattleActorRecords[actorIndex].scriptId == *(char *)((char *)&g_BattleActorIdleScripts + actorIndex)) {
            if (g_BattleActorRecords[actorIndex].facing == 0) {
                g_BattleActorRecords[actorIndex].yaw = 0x800 /* 180 deg */;
            } else {
                g_BattleActorRecords[actorIndex].yaw = 0;
            }
        }
        if ((*(unsigned char *)&g_BattleActorRecords[actorIndex].drawFlags & BATTLE_ACTOR_DRAW_STATUS_TINT) == 0) {
            return;
        }
        if (g_BattleIsPaused == 0) {
            if (g_BattleActorRecords[actorIndex].tintPulse < 16) {
                g_BattleActorRecords[actorIndex].tintPhase += 0x80;
            } else {
                g_BattleActorRecords[actorIndex].tintPhase -= 0x80;
            }
            (g_BattleActorRecords[actorIndex].tintPulse)--;
            g_BattleActorRecords[actorIndex].tintPulse &= 0x1F;
        }
    }
}

/*
 * Evaluates combatant status flags and selects highest priority status tint palette index.
 */
// FUNCTION: FF7 0x005ba1eb
int __cdecl Battle3d_SelectStatusTintIndex(char actorIndex)
{
    g_BattleActorRecords[actorIndex].drawFlags = *(unsigned char *)&g_BattleActorRecords[actorIndex].drawFlags | BATTLE_ACTOR_DRAW_STATUS_TINT;
    if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_SLOW_NUMB) != 0) {
        return BATTLE_STATUS_TINT_SLOW_NUMB;
    } else if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_PETRIFY) != 0) {
        return BATTLE_STATUS_TINT_PETRIFY;
    } else if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_POISON) != 0) {
        return BATTLE_STATUS_TINT_POISON;
    } else if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_BERSERK) != 0) {
        return BATTLE_STATUS_TINT_BERSERK;
    } else if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_PEERLESS) != 0) {
        return BATTLE_STATUS_TINT_PEERLESS;
    } else if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_DARKNESS) != 0) {
        return BATTLE_STATUS_TINT_DARKNESS;
    } else if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_REGEN) != 0) {
        return BATTLE_STATUS_TINT_REGEN;
    } else if ((g_BattleActorStates[actorIndex].status & BATTLE_STATUS_MANIPULATE) != 0) {
        return BATTLE_STATUS_TINT_MANIPULATE;
    } else {
        g_BattleActorRecords[actorIndex].drawFlags = *(unsigned char *)&g_BattleActorRecords[actorIndex].drawFlags & 0xFFFFFFF7;
        return BATTLE_STATUS_TINT_NONE;
    }
}

/*
 * Blends selected status tint color into actor base color channels.
 */
// FUNCTION: FF7 0x005ba335
void __cdecl Battle3d_ApplyStatusTint(int actorIndex, int tintIndex)
{
    switch (g_BattleOverlay_StatusTintTable[tintIndex].mode) {
    case BATTLE_TINT_MODE_NONE:
        g_BattleActorRecords[actorIndex].tintPhase = 0;
        break;
    case BATTLE_TINT_MODE_PULSE:
        g_BattleActorRecords[actorIndex].tintPhase = 0x800;
        break;
    case BATTLE_TINT_MODE_STATIC:
        g_BattleActorRecords[actorIndex].tintPhase = 0xc00;
        break;
    }
    g_BattleActorRecords[actorIndex].tintR = g_BattleOverlay_StatusTintTable[tintIndex].tintR;
    g_BattleActorRecords[actorIndex].tintG = g_BattleOverlay_StatusTintTable[tintIndex].tintG;
    g_BattleActorRecords[actorIndex].tintB = g_BattleOverlay_StatusTintTable[tintIndex].tintB;
    g_BattleActorRecords[actorIndex].tintPulse = 0;
}

/*
 * Projects and submits a screen-space overlay textured quad for a battle actor.
 */
// FUNCTION: FF7 0x005ba3f3
int __cdecl Battle3d_DrawActorOverlayQuad(int actorIndex, void *position, short texU, short texV)
{
    Battle3dOverlayQuad *quad;
    int slot;
    int visible;
    int margin;
    int cnt;
    int active;
    GfxHomogeneousVec4 slotValue;
    GfxProjectedVertexRecord *pVertexRec;

    visible = 1;
    quad = (Battle3dOverlayQuad *)Gfx_GetPsxPacketScratchPtr();
    quad->tag = quad->tag & 0xFFFFFF | 0x9000000;
    quad->command = 0x2C;
    quad->texturePage = 0x3F;
    quad->clutId = 0x7850;
    quad->colorR = 0x80;
    quad->colorG = 0x80;
    quad->colorB = 0x80;
    RotTransPoint(position, g_BattleOverlayIdentityMatrix.t, &g_BattleVmReg_2);
    SetRotMatrix(&g_BattleOverlayIdentityMatrix);
    SetTransMatrix(&g_BattleOverlayIdentityMatrix);
    RefreshGlobalTransformState();
    if (g_BattleUiScalingMode == MAIN_GRAPHICS_MODE_640X480) {
        Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(g_pBattle3dScene->spriteQuadState));
        Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(g_pBattle3dScene->spriteQuadState));
        texU = (int)texU << 1;
        texV = texV * 2 + -0x100;
    } else {
        Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(g_pBattle3dScene->overlayQuadState));
        Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(g_pBattle3dScene->overlayQuadState));
    }
    g_BattleVmReg_5 = 0;
    g_BattleVmReg_4 = 0;
    cnt = Gfx_ProjectQuadToScreen(&g_BattleOverlay_QuadOffset0, &g_BattleOverlay_QuadOffset1, &g_BattleOverlay_QuadOffset2, &g_BattleOverlay_QuadOffset3, &quad->xMin, &quad->xMax, &quad->xMinLower, &quad->xMaxLower, &g_BattleVmReg_0, &g_BattleVmReg_2);
    margin = (g_BattleUiScalingMode - 2 != 0 ? -15 : 0) + 31;
    g_BattleVmReg_0 = quad->xMax - quad->xMin;
    if (g_BattleVmReg_0 < margin) {
        quad->xMin = quad->xMax - margin;
        quad->xMinLower = quad->xMaxLower - margin;
        g_BattleVmReg_4 = 1;
    }
    g_BattleVmReg_0 = quad->yMax - quad->yMin;
    if (g_BattleVmReg_0 < margin) {
        quad->yInset = quad->yMax - margin;
        quad->yMin = quad->yInset;
        g_BattleVmReg_5 = 1;
    }
    active = 1;
    slot = 0;
    while (active != 0) {
        Gfx_ReadVertexSlotVector(slot, &slotValue);
        if (slotValue.w <= 0.0) {
            visible = 0;
            active = 0;
            break;
        }
        if (slot == 3) {
            break;
        }
        slot++;
    }
    if (visible != 0) {
        pVertexRec = (GfxProjectedVertexRecord *)Gfx_GetProjectedVertexRecord(0);
        Gfx_SetBlendFactorPair(pVertexRec->blendFactorA, pVertexRec->blendFactorB);
        if (g_BattleVmReg_4 == 1 && g_BattleVmReg_5 == 1) {
            quad->texU = texU;
            quad->texV = texV;
            quad->texUEnd = texU + g_BattleUiScale * 15;
            quad->texVStart = texV;
            quad->texUStart = texU;
            quad->texVEnd = texV + (g_BattleUiScale << 4);
            quad->texUEndAlt = texU + g_BattleUiScale * 15;
            quad->texVEndAlt = texV + (g_BattleUiScale << 4);
        } else {
            quad->texU = texU;
            quad->texV = texV;
            quad->texUEnd = texU + g_BattleUiScale * 15;
            quad->texVStart = texV;
            quad->texUStart = texU;
            quad->texVEnd = texV + g_BattleUiScale * 15;
            quad->texUEndAlt = texU + g_BattleUiScale * 15;
            quad->texVEndAlt = texV + g_BattleUiScale * 15;
        }
        cnt >>= 2;
        if (Gfx_IsDrawEnabled() != 0) {
            Gfx_DrawPsxPrimitive(quad);
        }
    }
    return (int)quad;
}

/*
 * Draws a status sprite over an actor: builds g_BattleOverlaySpritePos from the actor's bone-0
 * position (X and Z then replaced by its bone world position, Y from the actor state's spriteY)
 * and, unless the battle is paused, submits it through Battle3d_DrawActorOverlayQuad with an
 * animated texture column from g_BattleOverlay_QuadTexCoord and the row texV.
 */
// FUNCTION: FF7 0x005ba7c0
void __cdecl Battle3d_DrawActorOverlaySprite(char actorIndex, short texV)
{
    g_BattleOverlaySpritePos[0] = g_BattleActorRecords[actorIndex].bone0PosX;
    g_BattleOverlaySpritePos[1] = g_BattleActorStates[actorIndex].spriteY;
    g_BattleOverlaySpritePos[2] = g_BattleActorRecords[actorIndex].bone0PosZ;
    g_BattleOverlaySpritePos[0] = (short)g_BattleActorRecords[actorIndex].boneWorldPosX;
    g_BattleOverlaySpritePos[2] = (short)g_BattleActorRecords[actorIndex].boneWorldPosZ;
    if (g_BattleIsPaused == 0) {
        g_BattleOverlayDrawListHead = Battle3d_DrawActorOverlayQuad(g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, g_BattleOverlaySpritePos, g_BattleOverlay_QuadTexCoord[g_BattleOverlaySpriteCount >> 3 & 3], texV);
        g_BattleOverlaySpriteCount++;
    }
}

/*
 * Renders floating status icon quad positioned above an actor.
 */
// FUNCTION: FF7 0x005ba893
void __cdecl Battle3d_DrawActorStatusIcon(char actorIndex, unsigned char countdown)
{
    g_BattleStatusIconPos[0] = g_BattleActorRecords[actorIndex].bone0PosX;
    g_BattleStatusIconPos[1] = g_BattleActorStates[actorIndex].spriteY;
    g_BattleStatusIconPos[2] = g_BattleActorRecords[actorIndex].bone0PosZ;
    g_BattleStatusIconPos[0] = (short)g_BattleActorRecords[actorIndex].boneWorldPosX;
    g_BattleStatusIconPos[2] = (short)g_BattleActorRecords[actorIndex].boneWorldPosZ;
    if (countdown != 0xFF) {
        Battle3d_SplitDecimalDigits(countdown, g_BattleStatusIconDigits);
        g_BattleOverlayDrawListHead = Battle3d_DrawActorStatusIconDigitQuads(g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, g_BattleStatusIconPos, g_BattleStatusIconDigits[2], g_BattleStatusIconDigits[3]);
    }
}

/*
 * Splits integer value into tens and ones decimal digit indices.
 */
// FUNCTION: FF7 0x005ba95f
void __cdecl Battle3d_SplitDecimalDigits(short value, void *digits)
{
    int i;

    for (i = 0; i < 4; i++) {
        *(short *)((char *)digits + (3 - i) * 2) = value % 10;
        value = value / 10;
    }
}

/*
 * Renders two-digit numeric countdown timer quads above status icons.
 */
// FUNCTION: FF7 0x005ba9ac
int __cdecl Battle3d_DrawActorStatusIconDigitQuads(int drawListHandle, void *position, char tensDigit, char onesDigit)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[28];
        GfxProjectedVertexRecord *projRecordB;
        struct { unsigned char pad_to_4[0x4]; GfxProjectedVertexRecord *projRecordA; };
        struct { unsigned char pad_to_8[0x8]; int quadIndex; };
        struct { unsigned char pad_to_C[0xC]; int digitCode; };
        struct { unsigned char pad_to_10[0x10]; int projectResult; };
        struct { unsigned char pad_to_14[0x14]; Battle3dStatusIconDigitQuad *digitQuads[2]; };
    } frame;
#pragma pack(pop)

    frame.digitQuads[0] = (Battle3dStatusIconDigitQuad *)Gfx_GetPsxPacketScratchPtr();
    frame.digitQuads[1] = frame.digitQuads[0] + 1;
    for (frame.quadIndex = 0; frame.quadIndex < 2; frame.quadIndex++) {
        frame.digitQuads[frame.quadIndex]->tag = frame.digitQuads[frame.quadIndex]->tag & 0xFFFFFF | 0x9000000;
        frame.digitQuads[frame.quadIndex]->opcode = 0x2C;
        frame.digitQuads[frame.quadIndex]->texturePage = 0x3F;
        frame.digitQuads[frame.quadIndex]->clutId = 0x79D0;
        frame.digitQuads[frame.quadIndex]->colorR = 0x80;
        frame.digitQuads[frame.quadIndex]->colorG = 0x80;
        frame.digitQuads[frame.quadIndex]->colorB = 0x80;
    }
    RotTransPoint(position, g_BattleOverlayIdentityMatrix.t, &g_BattleVmReg_2);
    SetRotMatrix(&g_BattleOverlayIdentityMatrix);
    SetTransMatrix(&g_BattleOverlayIdentityMatrix);
    RefreshGlobalTransformState();
    Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(g_pBattle3dScene->overlayQuadState));
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(g_pBattle3dScene->overlayQuadState));
    g_BattleVmReg_5 = 0;
    g_BattleVmReg_4 = 0;
    for (frame.quadIndex = 0; frame.quadIndex < 2; frame.quadIndex++) {
        frame.projectResult = Gfx_ProjectQuadToScreen((SVECTOR *)((frame.quadIndex << 5) + (char *)&g_BattleOverlay_QuadOffset4), (SVECTOR *)((frame.quadIndex << 5) + (char *)&g_BattleOverlay_QuadOffset5), (SVECTOR *)((frame.quadIndex << 5) + (char *)&g_BattleOverlay_QuadOffset6), (SVECTOR *)((frame.quadIndex << 5) + (char *)&g_BattleOverlay_DigitQuadData), &frame.digitQuads[frame.quadIndex]->x0, &frame.digitQuads[frame.quadIndex]->x1, &frame.digitQuads[frame.quadIndex]->x2, &frame.digitQuads[frame.quadIndex]->x3, &g_BattleVmReg_0, &g_BattleVmReg_2);
        g_BattleVmReg_0 = frame.digitQuads[frame.quadIndex]->x1 - frame.digitQuads[frame.quadIndex]->x0;
        if (g_BattleVmReg_0 < g_BattleUiScale << 3) {
            if (frame.quadIndex == 0) {
                frame.digitQuads[frame.quadIndex]->x0 = frame.digitQuads[frame.quadIndex]->x1 - g_BattleUiScale * 7;
                frame.digitQuads[frame.quadIndex]->x2 = frame.digitQuads[frame.quadIndex]->x3 - g_BattleUiScale * 7;
            } else {
                frame.digitQuads[frame.quadIndex]->x1 = frame.digitQuads[frame.quadIndex]->x0 + g_BattleUiScale * 7;
                frame.digitQuads[frame.quadIndex]->x3 = frame.digitQuads[frame.quadIndex]->x2 + g_BattleUiScale * 7;
            }
            g_BattleVmReg_4 = 1;
        }
        g_BattleVmReg_0 = frame.digitQuads[frame.quadIndex]->y2 - frame.digitQuads[frame.quadIndex]->y0;
        if (g_BattleVmReg_0 < g_BattleUiScale << 3) {
            frame.digitQuads[frame.quadIndex]->y1 = frame.digitQuads[frame.quadIndex]->y2 - (g_BattleUiScale << 3);
            frame.digitQuads[frame.quadIndex]->y0 = frame.digitQuads[frame.quadIndex]->y1;
            g_BattleVmReg_5 = 1;
        }
    }
    if (g_BattleVmReg_4 == 1 && g_BattleVmReg_5 == 1) {
        if (g_BattleUiScalingMode == 2) {
            frame.digitCode = ((((unsigned char)tensDigit) + 0x11) * 8) * 2 - 0x100;
            frame.digitQuads[0]->u0 = frame.digitCode;
            frame.digitQuads[0]->v0 = 0;
            frame.digitQuads[0]->u1 = frame.digitCode + 0xE;
            frame.digitQuads[0]->v1 = 0;
            frame.digitQuads[0]->u2 = frame.digitCode;
            frame.digitQuads[0]->v2 = 0x10;
            frame.digitQuads[0]->u3 = frame.digitCode + 0xE;
            frame.digitQuads[0]->v3 = 0x10;
            frame.digitCode = ((((unsigned char)onesDigit) + 0x11) * 8) * 2 - 0x100;
            frame.digitQuads[1]->u0 = frame.digitCode;
            frame.digitQuads[1]->v0 = 0;
            frame.digitQuads[1]->u1 = frame.digitCode + 0xE;
            frame.digitQuads[1]->v1 = 0;
            frame.digitQuads[1]->u2 = frame.digitCode;
            frame.digitQuads[1]->v2 = 0x10;
            frame.digitQuads[1]->u3 = frame.digitCode + 0xE;
            frame.digitQuads[1]->v3 = 0x10;
        } else {
            frame.digitCode = ((unsigned char)tensDigit) * 8 + 0x88;
            frame.digitQuads[0]->u0 = frame.digitCode;
            frame.digitQuads[0]->v0 = 0;
            frame.digitQuads[0]->u1 = frame.digitCode + 7;
            frame.digitQuads[0]->v1 = 0;
            frame.digitQuads[0]->u2 = frame.digitCode;
            frame.digitQuads[0]->v2 = 8;
            frame.digitQuads[0]->u3 = frame.digitCode + 7;
            frame.digitQuads[0]->v3 = 8;
            frame.digitCode = ((unsigned char)onesDigit) * 8 + 0x88;
            frame.digitQuads[1]->u0 = frame.digitCode;
            frame.digitQuads[1]->v0 = 0;
            frame.digitQuads[1]->u1 = frame.digitCode + 7;
            frame.digitQuads[1]->v1 = 0;
            frame.digitQuads[1]->u2 = frame.digitCode;
            frame.digitQuads[1]->v2 = 8;
            frame.digitQuads[1]->u3 = frame.digitCode + 7;
            frame.digitQuads[1]->v3 = 8;
        }
    } else if (g_BattleUiScalingMode == 2) {
        frame.digitCode = ((((unsigned char)tensDigit) + 0x11) * 8) * 2 - 0x100;
        frame.digitQuads[0]->u0 = frame.digitCode;
        frame.digitQuads[0]->v0 = 0;
        frame.digitQuads[0]->u1 = frame.digitCode + 0xE;
        frame.digitQuads[0]->v1 = 0;
        frame.digitQuads[0]->u2 = frame.digitCode;
        frame.digitQuads[0]->v2 = 0xE;
        frame.digitQuads[0]->u3 = frame.digitCode + 0xE;
        frame.digitQuads[0]->v3 = 0xE;
        frame.digitCode = ((((unsigned char)onesDigit) + 0x11) * 8) * 2 - 0x100;
        frame.digitQuads[1]->u0 = frame.digitCode;
        frame.digitQuads[1]->v0 = 0;
        frame.digitQuads[1]->u1 = frame.digitCode + 0xE;
        frame.digitQuads[1]->v1 = 0;
        frame.digitQuads[1]->u2 = frame.digitCode;
        frame.digitQuads[1]->v2 = 0xE;
        frame.digitQuads[1]->u3 = frame.digitCode + 0xE;
        frame.digitQuads[1]->v3 = 0xE;
    } else {
        frame.digitCode = ((unsigned char)tensDigit) * 8 + 0x88;
        frame.digitQuads[0]->u0 = frame.digitCode;
        frame.digitQuads[0]->v0 = 0;
        frame.digitQuads[0]->u1 = frame.digitCode + 7;
        frame.digitQuads[0]->v1 = 0;
        frame.digitQuads[0]->u2 = frame.digitCode;
        frame.digitQuads[0]->v2 = 7;
        frame.digitQuads[0]->u3 = frame.digitCode + 7;
        frame.digitQuads[0]->v3 = 7;
        frame.digitCode = ((unsigned char)onesDigit) * 8 + 0x88;
        frame.digitQuads[1]->u0 = frame.digitCode;
        frame.digitQuads[1]->v0 = 0;
        frame.digitQuads[1]->u1 = frame.digitCode + 7;
        frame.digitQuads[1]->v1 = 0;
        frame.digitQuads[1]->u2 = frame.digitCode;
        frame.digitQuads[1]->v2 = 7;
        frame.digitQuads[1]->u3 = frame.digitCode + 7;
        frame.digitQuads[1]->v3 = 7;
    }
    frame.projectResult >>= 2;
    frame.projRecordA = (GfxProjectedVertexRecord *)Gfx_GetProjectedVertexRecord(0);
    Gfx_SetBlendFactorPair(frame.projRecordA->blendFactorA, frame.projRecordA->blendFactorB);
    if (Gfx_IsDrawEnabled() != 0) {
        Gfx_DrawPsxPrimitive(frame.digitQuads[1]);
    }
    frame.projRecordB = (GfxProjectedVertexRecord *)Gfx_GetProjectedVertexRecord(0);
    Gfx_SetBlendFactorPair(frame.projRecordB->blendFactorA, frame.projRecordB->blendFactorB);
    if (Gfx_IsDrawEnabled() != 0) {
        Gfx_DrawPsxPrimitive(frame.digitQuads[0]);
    }
    frame.digitQuads[1]++;
    return (int)frame.digitQuads[1];
}

/*
 * Submits transformed screen textured quad to battle overlay primitive list.
 */
// FUNCTION: FF7 0x005bb048
int __cdecl BattleFx_SubmitScreenTexturedQuad(int drawListHandle, int screenX, short screenY, short texU, short texV, short width, short height, short flags)
{
    BattleFxQuadRecord *quad;
    float scaledRemaining;
    float scaledTop;
    float scaledH;
    float scaledWidth;
    GfxTlVertex *pVertexA;
    GfxTlVertex *vertexB;
    GfxTlVertex *vertexDPtr;
    GfxTlVertex *vertexC;

    quad = (BattleFxQuadRecord *)(g_pBattle3dScene->overlayQuadState);
    if (Gfx_IsDrawEnabled() == 0) {
        return 0;
    } else {
        if (g_BattleUiScalingMode == 2) {
            width <<= 1;
            height <<= 1;
            texU = texU << 1;
            if (texU > 0xFF) {
                texU = texU - 0x100;
                texV = texV << 1;
                if (texV > 0xFF) {
                    quad = (BattleFxQuadRecord *)(g_pBattle3dScene->quadRecord);
                    texV = texV - 0x100;
                }
            } else {
                texV = texV << 1;
                if (texV > 0xFF) {
                    quad = (BattleFxQuadRecord *)(g_pBattle3dScene->spriteQuadState);
                    texV = texV - 0x100;
                }
            }
        }
        if (quad != 0) {
            if (AcquireBatchNode(1, quad) != 0) {
                scaledRemaining = (float)texU * quad->scaleX;
                scaledTop = (float)texV * quad->scaleY;
                scaledWidth = (float)width * quad->scaleX;
                scaledH = (float)height * quad->scaleY;
                pVertexA = (GfxTlVertex *)quad->bufferBase;
                pVertexA->x = (float)(short)screenX;
                pVertexA->y = (float)screenY;
                pVertexA->zBits = FLOAT_0_1_BITS;
                pVertexA->rhwBits = FLOAT_ONE_BITS;
                pVertexA->color = 0xFFFFFFFF;
                pVertexA->specular = 0xFF000000;
                pVertexA->u = scaledRemaining;
                pVertexA->v = scaledTop;
                vertexB = (GfxTlVertex *)(quad->bufferBase + 0x20);
                vertexB->x = (float)(short)screenX + width;
                vertexB->y = (float)screenY;
                vertexB->zBits = FLOAT_0_1_BITS;
                vertexB->rhwBits = FLOAT_ONE_BITS;
                vertexB->color = 0xFFFFFFFF;
                vertexB->specular = 0xFF000000;
                vertexB->u = scaledRemaining + scaledWidth;
                vertexB->v = scaledTop;
                vertexC = (GfxTlVertex *)(quad->bufferBase + 0x40);
                vertexC->x = (float)(short)screenX;
                vertexC->y = (float)screenY + height;
                vertexC->zBits = FLOAT_0_1_BITS;
                vertexC->rhwBits = FLOAT_ONE_BITS;
                vertexC->color = 0xFFFFFFFF;
                vertexC->specular = 0xFF000000;
                vertexC->u = scaledRemaining;
                vertexC->v = scaledTop + scaledH;
                vertexDPtr = (GfxTlVertex *)(quad->bufferBase + 0x60);
                vertexDPtr->x = (float)(short)screenX + width;
                vertexDPtr->y = (float)screenY + height;
                vertexDPtr->zBits = FLOAT_0_1_BITS;
                vertexDPtr->rhwBits = FLOAT_ONE_BITS;
                vertexDPtr->color = 0xFFFFFFFF;
                vertexDPtr->specular = 0xFF000000;
                vertexDPtr->u = scaledRemaining + scaledWidth;
                vertexDPtr->v = scaledTop + scaledH;
                if ((flags & 1) == 0) {
                    *(char *)quad->primPointer = 7;
                    quad->primCode = 7;
                } else {
                    *(char *)quad->primPointer = 4;
                    quad->primCode = 4;
                }
            }
        }
        return 0;
    }
}

/*
 * Renders red pointer marker above currently selected enemy combatant.
 */
// FUNCTION: FF7 0x005bb37a
void __cdecl Battle_DrawTargetedEnemyMarker(char targetIndex)
{
    if (g_BattleSceneState == 4 && g_BattleScriptEffectFlag == 0 && g_BattleScriptAnimRunFlag == 0) {
        if (g_BattleActorScreenPositions[targetIndex].projectedX == -1 && g_BattleActorScreenPositions[targetIndex].projectedY == -1) { /* empty first arm: lib/codegen.h */
        } else {
            g_BattleOverlayDrawListHead = BattleFx_SubmitScreenTexturedQuad(g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, g_BattleActorScreenPositions[targetIndex].projectedX + 3, g_BattleActorScreenPositions[targetIndex].projectedY - 0xE, 0, 0xD0, 48, 16, 0);
        }
    }
}

/*
 * Animates popping damage number digits and spawns digit quads.
 */
// FUNCTION: FF7 0x005bb410
void __cdecl Battle_DamageNumberTask(void)
{
    struct {
        int marginY;
        int marginX;
        float point[3];
        float screen[4];
        float clip[4];
        float matrix[4][4];
        struct BattleDamageNumberTaskSlot *slot;
    } frame;

    frame.slot = (struct BattleDamageNumberTaskSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    switch (g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter) {
    case BATTLE_TASK_FRAME_FIRST:
        frame.slot->frameCounter = BATTLE_TASK_FRAME_UPDATE;
        frame.slot->lifeFrames = 11;
        frame.slot->riseStep = 0;
        g_BattleVmReg_0 = g_BattleActorRecords[frame.slot->actorIndex].targetBone;
        Battle_GetBoneWorldPos(frame.slot->actorIndex, g_BattleVmReg_0, &g_BattleActorStates[frame.slot->actorIndex].node.posX);
        g_BattleActorStates[frame.slot->actorIndex].node.rotZ = 0;
        g_BattleActorStates[frame.slot->actorIndex].node.yaw = 0;
        g_BattleActorStates[frame.slot->actorIndex].node.rotX = 0;
        g_BattleActorStates[frame.slot->actorIndex].node.parentMatrix = (int)g_BattleStageNodes[0].matrix;
        break;
    case BATTLE_TASK_FRAME_UPDATE:
        if (frame.slot->lifeFrames == 0) {
            frame.slot->ownerSlot = -1;
            break;
        }
        RotMatrix((SVECTOR *)&g_BattleActorStates[frame.slot->actorIndex].node.rotX, (int *)g_BattleActorStates[frame.slot->actorIndex].node.matrix);
        Battle3d_ComposeNodeWithParent(&g_BattleActorStates[frame.slot->actorIndex].node);
        GtePsxMatrixToFloat4x4Transposed(GetInstalledRotMatrixPtr(), frame.matrix);
        frame.point[0] = (float)g_BattleDamageNumberPos.x;
        frame.point[1] = (float)g_BattleDamageNumberPos.y;
        frame.point[2] = (float)g_BattleDamageNumberPosZ;
        if (ProjectPointWithMatrix((int)frame.matrix, (int)frame.point, (int)frame.clip, (int)frame.screen)) {
            frame.slot->screenX = (short)frame.screen[0];
            frame.slot->baseScreenY = (short)frame.screen[1];
            frame.slot->screenY = frame.slot->baseScreenY - (g_BattleOverlayCurves.damageRise[frame.slot->riseStep] << 1);
            switch (g_BattleUiScalingMode) {
            case MAIN_GRAPHICS_MODE_320X240:
            case MAIN_GRAPHICS_MODE_320X240_CENTERED:
                frame.marginX = 24;
                frame.marginY = 16;
                break;
            case MAIN_GRAPHICS_MODE_640X480:
                frame.marginX = 48;
                frame.marginY = 32;
                break;
            }
            if (frame.slot->screenX < g_BattleViewportOriginX + frame.marginX)
                frame.slot->screenX = (short)(g_BattleViewportOriginX + frame.marginX);
            if (frame.slot->screenX > g_BattleViewportMaxX - frame.marginX)
                frame.slot->screenX = (short)(g_BattleViewportMaxX - frame.marginX);
            if (frame.slot->screenY < g_BattleViewportOriginY + frame.marginY)
                frame.slot->screenY = (short)(g_BattleViewportOriginY + frame.marginY);
            if (frame.slot->screenY > (*(int *)&g_BattleViewportMaxY) - frame.marginY)
                frame.slot->screenY = (short)((*(int *)&g_BattleViewportMaxY) - frame.marginY);
            Battle_DrawDamageNumber(frame.slot->screenX, frame.slot->screenY, frame.slot->value, (short)g_BattleOverlayCurves.damageRise[(frame.slot->riseStep)++], frame.slot->flags);
        }
        (*(short *)g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data)--;
        break;
    }
}

/*
 * Draws a damage number at the active damage-number slot's screen position: MISS, a recovery or
 * a death label for the values -1..-3, otherwise the digits from Battle_SplitDamageDigits, with
 * an extra mark when flags bit 2 is set.
 */
// FUNCTION: FF7 0x005bb756
void __cdecl Battle_DrawDamageNumber(short x, short y, short value, short rise, short flags)
{
    struct {
        int index;
        int halfWidth;
        struct BattleDamageNumberTaskSlot *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.slot = (struct BattleDamageNumberTaskSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    g_BattleVmReg_2 = Battle_SplitDamageDigits(value, g_BattleDamageNumberDigits);
    switch (value) {
    case BATTLE_DAMAGE_LABEL_MISS:
        g_BattleOverlayDrawListHead = BattleFx_SubmitScreenTexturedQuad(g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5,
                                                         frame.slot->screenX - g_BattleUiScale * 12, frame.slot->screenY,
                                                         0x80, 0x88, 24, 11, flags);
        break;
    case BATTLE_DAMAGE_LABEL_DEATH:
        g_BattleOverlayDrawListHead = BattleFx_SubmitScreenTexturedQuad(g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5,
                                                         frame.slot->screenX - (g_BattleUiScale << 4), frame.slot->screenY,
                                                         0x20, 0xE0, 32, 10, flags);
        break;
    case BATTLE_DAMAGE_LABEL_FULL_RECOVERY:
        g_BattleOverlayDrawListHead = BattleFx_SubmitScreenTexturedQuad(g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5,
                                                         frame.slot->screenX - g_BattleUiScale * 24, frame.slot->screenY,
                                                         0x20, 0xEA, 32, 10, flags);
        g_BattleOverlayDrawListHead = BattleFx_SubmitScreenTexturedQuad(g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5,
                                                         frame.slot->screenX + (g_BattleUiScale << 3), frame.slot->screenY,
                                                         0x20, 0xF4, 18, 10, flags);
        break;
    default:
        frame.halfWidth = ((g_BattleVmReg_2 << 3) / 2) * g_BattleUiScale;
        if (flags & 4) {
            g_BattleOverlayDrawListHead = BattleFx_SubmitScreenTexturedQuad(g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5,
                                                             frame.slot->screenX + frame.halfWidth + g_BattleUiScale,
                                                             frame.slot->screenY + g_BattleUiScale * 6, 0x80, 0x93, 15, 5, flags);
        }
        for (frame.index = 0; frame.index < g_BattleVmReg_2; frame.index++) {
            g_BattleOverlayDrawListHead = BattleFx_SubmitScreenTexturedQuad(g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5,
                                                             frame.slot->screenX - (g_BattleUiScale << 3) + frame.halfWidth,
                                                             frame.slot->screenY, g_BattleDamageNumberDigits[3 - frame.index], 0x88, 8, 11, flags);
            frame.halfWidth -= g_BattleUiScale * 9;
        }
        break;
    }
}

/*
 * Extracts individual base-10 digit characters from damage amount.
 */
// FUNCTION: FF7 0x005bb9c9
char __cdecl Battle_SplitDamageDigits(short damage, short *digits)
{
    int i;

    for (i = 0; i < 4; i++) {
        digits[3 - i] = (damage % 10) << 3;
        damage /= 10;
        digits[3 - i] += 0x98;
    }
    for (i = 0; i < 3; i++) {
        if (digits[i] == 0x98) {
            digits[i] = 0;
        } else {
            return 4 - i;
        }
    }
    return 1;
}

/*
 * Fades out combatant model transparency upon defeat or exit.
 */
// FUNCTION: FF7 0x005bba82
void __cdecl Battle_ActorFadeOutEffectTask(void)
{
    int task;

    switch (g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter) {
    case BATTLE_TASK_FRAME_FIRST:
        *(short *)g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].data = 0xE;
        (g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter)++;
        task = *(short *)&BATTLE_CURRENT_TASK_SLOT->data[4];
        Battle_PlayPannedSfxForActor((char)task, 0x16, BATTLE_CURRENT_TASK_SLOT->data[2]);
        g_BattleActorStates[task].motionState = BATTLE_ACTOR_MOTION_FROZEN;
        g_BattleActorRecords[task].tintR = 0xF8;
        g_BattleActorRecords[task].tintG = 0;
        g_BattleActorRecords[task].tintB = 0;
        g_BattleActorRecords[task].tintPhase = 0x800;
        Battle_SetActorFadingFlags((short)task);
        Battle_StatusPoseMode3UpdateActor((char)g_BattleActiveTaskSlotIdx);
        break;
    case BATTLE_TASK_FRAME_UPDATE:
        Battle_StatusPoseMode3UpdateActor((char)g_BattleActiveTaskSlotIdx);
        break;
    }
}

/*
 * Dispatches sound effect with stereo panning computed from actor position.
 */
// FUNCTION: FF7 0x005bbb9c
void __cdecl Battle_PlayPannedSfxForActor(char actorIndex, short sfxParam, char enabled)
{
    if (enabled != 0) {
        SoundDispatchCommand(SOUND_SFX_PLAY_CH3, g_BattleActorScreenPositions[actorIndex].prevX / 5 << 1 & 0x7F, sfxParam, 0, 0, 0, 0, 0, 0);
    }
}

/*
 * Updates actor animation parameters for status pose mode 3.
 */
// FUNCTION: FF7 0x005bbbde
void __cdecl Battle_StatusPoseMode3UpdateActor(char poseSlot)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int actorIndex;

    actorIndex = *(short *)&g_BattleTaskSlotBuffer.mainSlots[poseSlot].data[4];
    if (*(short *)g_BattleTaskSlotBuffer.mainSlots[poseSlot].data == 0) {
        g_BattleTaskSlotBuffer.mainSlots[poseSlot].ownerSlot = BATTLE_TASK_FREE;
        g_BattleActorRecords[actorIndex].drawFlags = *(unsigned char *)&g_BattleActorRecords[actorIndex].drawFlags & -0x81;
        g_BattleActorStates[actorIndex].flags = g_BattleActorStates[actorIndex].flags & -0x21;
        g_BattleActorRecords[actorIndex].isDirty = 1;
        g_BattleActorStates[actorIndex].motionState = BATTLE_ACTOR_MOTION_NORMAL;
        Battle_ResetActorPose((char)actorIndex);
    } else {
        g_BattleActorRecords[actorIndex].tintPhase += 0x80;
        g_BattleActorRecords[actorIndex].tintR -= 0x10;
        g_BattleActorRecords[actorIndex].deathActive = 1;
        g_BattleActorRecords[actorIndex].deathSpinAngle += 22.5f;
        (*(short *)g_BattleTaskSlotBuffer.mainSlots[poseSlot].data)--;
    }
}

/*
 * Task driving status pose mode 0 animation cycle.
 */
// FUNCTION: FF7 0x005bbd24
void __cdecl Battle_StatusPoseMode0Task(void)
{
    int task;

    switch (g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter) {
    case BATTLE_TASK_FRAME_FIRST:
        *(short *)g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].data = 0xE;
        (g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter)++;
        task = *(short *)&BATTLE_CURRENT_TASK_SLOT->data[4];
        Battle_PlayPannedSfxForActor((char)task, 0x16, BATTLE_CURRENT_TASK_SLOT->data[2]);
        g_BattleActorRecords[task].tintR = 0xF8;
        g_BattleActorRecords[task].tintG = 0;
        g_BattleActorRecords[task].tintB = 0;
        g_BattleActorRecords[task].tintPhase = 0x800;
        Battle_SetActorFadingFlags((short)task);
        Battle_StatusPoseMode0UpdateActor((char)g_BattleActiveTaskSlotIdx);
        break;
    case BATTLE_TASK_FRAME_UPDATE:
        Battle_StatusPoseMode0UpdateActor((char)g_BattleActiveTaskSlotIdx);
        break;
    }
}

/*
 * Updates combatant bone animations for status pose mode 0.
 */
// FUNCTION: FF7 0x005bbe32
void __cdecl Battle_StatusPoseMode0UpdateActor(char poseSlot)
{
    int actorIndex;

    actorIndex = *(short *)&g_BattleTaskSlotBuffer.mainSlots[poseSlot].data[4];
    if (*(short *)g_BattleTaskSlotBuffer.mainSlots[poseSlot].data == 0) {
        g_BattleTaskSlotBuffer.mainSlots[poseSlot].ownerSlot = BATTLE_TASK_FREE;
        g_BattleActorRecords[actorIndex].drawFlags =
            (*(unsigned char *)&g_BattleActorRecords[actorIndex].drawFlags & -0x81);
        g_BattleActorStates[actorIndex].flags = g_BattleActorStates[actorIndex].flags & -0x21;
        g_BattleActorRecords[actorIndex].isDirty = 1;
        Battle_ResetActorPose((char)actorIndex);
    } else {
        g_BattleActorRecords[actorIndex].tintPhase += 0x80;
        g_BattleActorRecords[actorIndex].tintR -= 0x10;
        (*(short *)g_BattleTaskSlotBuffer.mainSlots[poseSlot].data)--;
    }
}

/*
 * Task animating defeated enemy sinking into the ground.
 */
// FUNCTION: FF7 0x005bbf31
void __cdecl Battle_OverlayTaskDeathSink(void)
{
    int a;

    switch (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].state) {
    case BATTLE_TASK_FRAME_FIRST:
        g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown = 14;
        (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].state)++;
        a = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].actorId;
        Battle_PlayPannedSfxForActor((char)a, 0x16, g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sfxPan);
        g_BattleActorRecords[a].tintPhase = 0x800;
        g_BattleActorRecords[a].tintR = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].battleActorIndex;
        g_BattleActorRecords[a].tintG = 0;
        g_BattleActorRecords[a].tintB = 0;
        Battle_SetActorFadingFlags((short)a);
        Battle_OverlayTaskDeathSinkUpdate((char)g_BattleOverlayTaskSlotIdx);
        break;
    case BATTLE_TASK_FRAME_UPDATE:
        Battle_OverlayTaskDeathSinkUpdate((char)g_BattleOverlayTaskSlotIdx);
        break;
    }
}

/*
 * Lowers combatant vertical position during death sink animation.
 */
// FUNCTION: FF7 0x005bc04d
void __cdecl Battle_OverlayTaskDeathSinkUpdate(char taskSlot)
{
    struct {
        int actor;
        int unused;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.actor = g_BattleOverlayTaskSlotBuffer[taskSlot].actorId;
    if (g_BattleOverlayTaskSlotBuffer[taskSlot].sinkCountdown == 0) {
        g_BattleOverlayTaskSlotBuffer[taskSlot].ownerSlot = -1;
        g_BattleActorRecords[frame.actor].drawFlags = *(unsigned char *)&g_BattleActorRecords[frame.actor].drawFlags & ~0x80;
        g_BattleActorStates[frame.actor].flags = g_BattleActorStates[frame.actor].flags & ~0x20;
        g_BattleActorRecords[frame.actor].isDirty = 1;
        Battle_ResetActorPose((char)frame.actor);
    } else {
        g_BattleActorRecords[frame.actor].tintPhase += 0x80;
        if (*(unsigned char *)&g_BattleActorRecords[frame.actor].tintR != 0)
            *(unsigned char *)&g_BattleActorRecords[frame.actor].tintR -= 0x10;
        g_BattleActorRecords[frame.actor].deathActive = 1;
        g_BattleActorRecords[frame.actor].sinkOffset += (float)g_BattleOverlayCurves.deathSinkStep[g_BattleOverlayTaskSlotBuffer[taskSlot].sinkCountdown];
        g_BattleActorRecords[frame.actor].deathVelocityX += 22.5f;
        g_BattleActorRecords[frame.actor].deathSpinAngle += 22.5f;
        g_BattleActorRecords[frame.actor].deathVelocityZ += 22.5f;
        (g_BattleOverlayTaskSlotBuffer[taskSlot].sinkCountdown)--;
    }
}

/*
 * Task driving status pose mode 2 animation cycle.
 */
// FUNCTION: FF7 0x005bc21f
void __cdecl Battle_StatusPoseMode2Task(void)
{
    int task;

    switch (g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter) {
    case BATTLE_TASK_FRAME_FIRST:
        *(short *)g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].data = 0xE;
        (g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter)++;
        task = *(short *)&BATTLE_CURRENT_TASK_SLOT->data[4];
        Battle_PlayPannedSfxForActor((char)task, 0x16, BATTLE_CURRENT_TASK_SLOT->data[2]);
        g_BattleActorRecords[task].tintR = 0xF8;
        g_BattleActorRecords[task].tintG = 0;
        g_BattleActorRecords[task].tintB = 0;
        g_BattleActorRecords[task].tintPhase = 0x800;
        Battle_SetActorFadingFlags((short)task);
        Battle_StatusPoseMode2UpdateActor((char)g_BattleActiveTaskSlotIdx);
        break;
    case BATTLE_TASK_FRAME_UPDATE:
        Battle_StatusPoseMode2UpdateActor((char)g_BattleActiveTaskSlotIdx);
        break;
    }
}

/*
 * Updates combatant bone animations for status pose mode 2.
 */
// FUNCTION: FF7 0x005bc32d
void __cdecl Battle_StatusPoseMode2UpdateActor(char taskSlot)
{
    int actor;

    actor = *(short *)&g_BattleTaskSlotBuffer.mainSlots[taskSlot].data[4];
    if (*(short *)g_BattleTaskSlotBuffer.mainSlots[taskSlot].data == 0) {
        g_BattleTaskSlotBuffer.mainSlots[taskSlot].ownerSlot = BATTLE_TASK_FREE;
        g_BattleActorRecords[actor].drawFlags = *(unsigned char *)&g_BattleActorRecords[actor].drawFlags & -0x81;
        g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags & -0x21;
        g_BattleActorRecords[actor].isDirty = 1;
        Battle_ResetActorPose((char)actor);
    } else {
        g_BattleActorRecords[actor].tintPhase += 0x80;
        g_BattleActorRecords[actor].tintR -= 0x10;
        g_BattleActorStates[actor].poseAngleA += 0x100 /* 22.5 deg */;
        g_BattleActorStates[actor].poseAngleB -= 0x100 /* 22.5 deg */;
        g_BattleActorStates[actor].poseAngleC += 0x100;
        (*(short *)g_BattleTaskSlotBuffer.mainSlots[taskSlot].data)--;
    }
}

/*
 * Task driving status pose mode 17 shake and flash animation.
 */
// FUNCTION: FF7 0x005bc48c
void __cdecl Battle_StatusPoseMode17Task(void)
{
    int childSlot;
    int actorIndex;

    switch (g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter) {
    case BATTLE_TASK_FRAME_FIRST:
        *(short *)g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].data = 0x40;
        (g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter)++;
        actorIndex = *(short *)&BATTLE_CURRENT_TASK_SLOT->data[4];
        Battle_PlayPannedSfxForActor((char)actorIndex, 0x1C0, BATTLE_CURRENT_TASK_SLOT->data[2]);
        childSlot = (short)Battle_AllocateSecondaryResourceSlot(Battle_StatusPoseMode17ShakeFlashTask);
        *(short *)&g_BattleTaskSlotBuffer.secondarySlots[childSlot].data[4] = actorIndex;
        *(short *)g_BattleTaskSlotBuffer.secondarySlots[childSlot].data = 0x40;
        *(short *)&g_BattleTaskSlotBuffer.secondarySlots[childSlot].data[6] = g_BattleActorRecords[actorIndex].posZ;
        g_BattleActorRecords[actorIndex].tintR = 0xFF;
        g_BattleActorRecords[actorIndex].tintG = 0;
        g_BattleActorRecords[actorIndex].tintB = 0;
        g_BattleActorRecords[actorIndex].tintPhase = 0x800;
        Battle_SetActorFadingFlags((short)actorIndex);
        Battle_StatusPoseMode17UpdateActor(g_BattleActiveTaskSlotIdx);
        break;
    case BATTLE_TASK_FRAME_UPDATE:
        Battle_StatusPoseMode17UpdateActor(g_BattleActiveTaskSlotIdx);
    }
}

/*
 * Applies screen shake and white flash pulses for status pose mode 17.
 */
// FUNCTION: FF7 0x005bc5ec
void __cdecl Battle_StatusPoseMode17ShakeFlashTask(void)
{
    int actorIdx;
    int slot;

    actorIdx = *(short *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data[4];
    slot = g_BattleSecondaryTaskActiveSlotIdx;
    if (*(short *)g_BattleTaskSlotBuffer.secondarySlots[slot].data == 0) {
        g_BattleTaskSlotBuffer.secondarySlots[slot].ownerSlot = BATTLE_TASK_FREE;
    } else {
        if (*(short *)g_BattleTaskSlotBuffer.secondarySlots[slot].data == 0x3A || *(short *)g_BattleTaskSlotBuffer.secondarySlots[slot].data == 0x40) {
            g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(0xFA, 0xFA, 0xFA);
        }
        if ((*(short *)g_BattleTaskSlotBuffer.secondarySlots[slot].data & 1) != 0) {
            g_BattleActorRecords[actorIdx].posZ = *(short *)&g_BattleTaskSlotBuffer.secondarySlots[slot].data[6] + 0x40;
        } else {
            g_BattleActorRecords[actorIdx].posZ = *(short *)&g_BattleTaskSlotBuffer.secondarySlots[slot].data[6] - 0x40;
        }
        (*(short *)g_BattleTaskSlotBuffer.secondarySlots[slot].data)--;
    }
}

/*
 * Updates actor bone matrices for status pose mode 17.
 */
// FUNCTION: FF7 0x005bc6ed
void __cdecl Battle_StatusPoseMode17UpdateActor(char taskSlot)
{
    int actor;

    actor = *(short *)&g_BattleTaskSlotBuffer.mainSlots[taskSlot].data[4];
    if (*(short *)g_BattleTaskSlotBuffer.mainSlots[taskSlot].data == 1) {
        g_BattleTaskSlotBuffer.mainSlots[taskSlot].ownerSlot = BATTLE_TASK_FREE;
        g_BattleActorRecords[actor].drawFlags = *(unsigned char *)&g_BattleActorRecords[actor].drawFlags & -0x81;
        g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags & -0x21;
        g_BattleActorRecords[actor].isDirty = 1;
        g_BattleActorRecords[actor].tintR = 0;
        g_BattleActorRecords[actor].tintPhase = 0x1000;
        Battle_ResetActorPose((char)actor);
    } else {
        g_BattleActorRecords[actor].tintPhase += 0x20;
        g_BattleActorRecords[actor].tintR -= 4;
        (*(short *)g_BattleTaskSlotBuffer.mainSlots[taskSlot].data)--;
    }
}

/*
 * Task driving status pose mode 7 animation cycle.
 */
// FUNCTION: FF7 0x005bc812
void __cdecl Battle_StatusPoseMode7Task(void)
{
    int n;

    switch (g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter) {
    case BATTLE_TASK_FRAME_FIRST:
        *(short *)g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].data = 0x10;
        g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter += 1;
        n = *(short *)&BATTLE_CURRENT_TASK_SLOT->data[4];
        Battle_PlayPannedSfxForActor(n, 0x16, BATTLE_CURRENT_TASK_SLOT->data[2]);
        g_BattleActorRecords[n].tintR = 0x80;
        g_BattleActorRecords[n].tintG = 0x80;
        g_BattleActorRecords[n].tintB = 0x80;
        g_BattleActorRecords[n].tintPhase = 0;
        Battle_SetActorFadingFlags(n);
        Battle_StatusPoseMode7UpdateActor(g_BattleActiveTaskSlotIdx);
        break;
    case BATTLE_TASK_FRAME_UPDATE:
        Battle_StatusPoseMode7UpdateActor(g_BattleActiveTaskSlotIdx);
        break;
    }
}

/*
 * Updates combatant bone animations for status pose mode 7.
 */
// FUNCTION: FF7 0x005bc920
void __cdecl Battle_StatusPoseMode7UpdateActor(char taskSlot)
{
    int actor;

    actor = *(short *)&g_BattleTaskSlotBuffer.mainSlots[taskSlot].data[4];
    if (*(short *)g_BattleTaskSlotBuffer.mainSlots[taskSlot].data == 0) {
        g_BattleTaskSlotBuffer.mainSlots[taskSlot].ownerSlot = BATTLE_TASK_FREE;
        g_BattleActorRecords[actor].drawFlags = *(unsigned char *)&g_BattleActorRecords[actor].drawFlags & -0x81;
        g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags & -0x21;
        g_BattleActorRecords[actor].isDirty = 1;
    } else {
        g_BattleActorRecords[actor].tintPhase += 0x80;
        g_BattleActorRecords[actor].tintR -= 8;
        g_BattleActorRecords[actor].tintG -= 8;
        g_BattleActorRecords[actor].tintB -= 8;
        g_BattleActorStates[actor].poseAngleA = 0;
        g_BattleActorStates[actor].poseAngleB = 0x1000 /* 360 deg */;
        g_BattleActorStates[actor].poseAngleC = 0x1000;
        g_BattleActorRecords[actor].yaw += 0x100 /* 22.5 deg */;
        (*(short *)g_BattleTaskSlotBuffer.mainSlots[taskSlot].data)--;
    }
}

/*
 * Task driving status pose mode 8 animation cycle.
 */
// FUNCTION: FF7 0x005bcaaa
void __cdecl Battle_StatusPoseMode8Task(void)
{
    int a;

    switch (g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter) {
    case BATTLE_TASK_FRAME_FIRST:
        *(short *)g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].data = 0x10;
        g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter += 1;
        a = *(short *)&BATTLE_CURRENT_TASK_SLOT->data[4];
        Battle_PlayPannedSfxForActor(a, 0x16, BATTLE_CURRENT_TASK_SLOT->data[2]);
        g_BattleActorRecords[a].tintR = 0x80;
        g_BattleActorRecords[a].tintG = 0x80;
        g_BattleActorRecords[a].tintB = 0x80;
        g_BattleActorRecords[a].tintPhase = 0;
        Battle_SetActorFadingFlags(a);
        Battle_StatusPoseMode8UpdateActor(g_BattleActiveTaskSlotIdx);
        break;
    case BATTLE_TASK_FRAME_UPDATE:
        Battle_StatusPoseMode8UpdateActor(g_BattleActiveTaskSlotIdx);
        break;
    }
}

/*
 * Updates combatant bone animations for status pose mode 8.
 */
// FUNCTION: FF7 0x005bcbb8
void __cdecl Battle_StatusPoseMode8UpdateActor(char taskSlot)
{
    int actor;

    actor = *(short *)&g_BattleTaskSlotBuffer.mainSlots[taskSlot].data[4];
    if (*(short *)g_BattleTaskSlotBuffer.mainSlots[taskSlot].data == 0) {
        g_BattleTaskSlotBuffer.mainSlots[taskSlot].ownerSlot = BATTLE_TASK_FREE;
        g_BattleActorRecords[actor].drawFlags = *(unsigned char *)&g_BattleActorRecords[actor].drawFlags & -0x81;
        g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags & -0x21;
        g_BattleActorRecords[actor].isDirty = 1;
    } else {
        g_BattleActorRecords[actor].tintPhase += 0x80;
        g_BattleActorRecords[actor].tintR -= 8;
        g_BattleActorRecords[actor].tintG -= 8;
        g_BattleActorRecords[actor].tintB -= 8;
        g_BattleActorStates[actor].poseAngleA = 0x1000 /* 360 deg */;
        g_BattleActorStates[actor].poseAngleB = 0x1000 /* 360 deg */;
        g_BattleActorStates[actor].poseAngleC = 0;
        g_BattleActorRecords[actor].posY -= 128;
        (*(short *)g_BattleTaskSlotBuffer.mainSlots[taskSlot].data)--;
    }
}

/*
 * Interpolates full-screen color tinting ramp over multiple frames.
 */
// FUNCTION: FF7 0x005bcd42
void __cdecl Battle_EffectTaskScreenColourRamp(void)
{
    switch (g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter) {
    case BATTLE_SCREEN_RAMP_START:
        g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter = BATTLE_SCREEN_RAMP_FADE;
        *(short *)g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data = 0xF;
        *(short *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data[4] = 0x10;
        g_BattleScreenRampActive = 1;
        g_BattleScreenRampBlue.level = 0;
        g_BattleScreenRampGreen.level = g_BattleScreenRampBlue.level;
        g_BattleRainActiveCount = g_BattleScreenRampGreen.level;
        if (g_BattleDarkenScreenGlobalAlphaEnabled != 0) {
            /* Retail caller reads residual EAX/AX as `int` after `void BattleFx_DarkenScreenGlobalAlpha` returns. */
            g_BattleOverlayDrawListHead = ((int(__cdecl *)(char, char, char))BattleFx_DarkenScreenGlobalAlpha)(g_BattleRainActiveCount, g_BattleScreenRampGreen.level, g_BattleScreenRampBlue.level);
        }
        g_BattleRainActiveCount += *(short *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data[4];
        g_BattleScreenRampBlue.level = g_BattleRainActiveCount;
        g_BattleScreenRampGreen.level = g_BattleScreenRampBlue.level;
        (*(short *)g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data)--;
        break;
    case BATTLE_SCREEN_RAMP_FADE:
        if (*(short *)g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data == 0) {
            g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter = BATTLE_SCREEN_RAMP_HOLD;
            g_BattleScreenRampBlue.level = 0xFF;
            g_BattleScreenRampGreen.level = g_BattleScreenRampBlue.level;
            g_BattleRainActiveCount = g_BattleScreenRampGreen.level;
            if (g_BattleDarkenScreenGlobalAlphaEnabled != 0) {
                /* Retail caller reads residual EAX/AX as `int` after `void BattleFx_DarkenScreenGlobalAlpha` returns. */
                g_BattleOverlayDrawListHead = ((int(__cdecl *)(char, char, char))BattleFx_DarkenScreenGlobalAlpha)(g_BattleRainActiveCount, g_BattleScreenRampGreen.level, g_BattleScreenRampBlue.level);
            }
        } else {
            if (g_BattleDarkenScreenGlobalAlphaEnabled != 0) {
                /* Retail caller reads residual EAX/AX as `int` after `void BattleFx_DarkenScreenGlobalAlpha` returns. */
                g_BattleOverlayDrawListHead = ((int(__cdecl *)(char, char, char))BattleFx_DarkenScreenGlobalAlpha)(g_BattleRainActiveCount, g_BattleScreenRampGreen.level, g_BattleScreenRampBlue.level);
            }
            g_BattleRainActiveCount += *(short *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data[4];
            g_BattleScreenRampBlue.level = g_BattleRainActiveCount;
            g_BattleScreenRampGreen.level = g_BattleScreenRampBlue.level;
            (*(short *)g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data)--;
        }
        break;
    case BATTLE_SCREEN_RAMP_HOLD:
        if (g_BattleDarkenScreenGlobalAlphaEnabled != 0) {
            /* Retail caller reads residual EAX/AX as `int` after `void BattleFx_DarkenScreenGlobalAlpha` returns. */
            g_BattleOverlayDrawListHead = ((int(__cdecl *)(char, char, char))BattleFx_DarkenScreenGlobalAlpha)(g_BattleRainActiveCount, g_BattleScreenRampGreen.level, g_BattleScreenRampBlue.level);
        }
    }
}

/*
 * Drives animated screen transition wipe band.
 */
// FUNCTION: FF7 0x005bcf9d
void __cdecl BattleFx_ScreenBandWipeTask(void)
{
    switch (g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter) {
    case BATTLE_TASK_FRAME_FIRST:
        *(short *)g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data = 0x15;
        (g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter)++;
        *(short *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data[4] = g_BattleUiScale * 0x53;
        *(short *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data[6] = 0;
        BattleFx_DrawScreenBandWipe(g_BattleSecondaryTaskActiveSlotIdx);
        break;
    case BATTLE_TASK_FRAME_UPDATE:
        BattleFx_DrawScreenBandWipe(g_BattleSecondaryTaskActiveSlotIdx);
        break;
    }
}

/*
 * Renders screen wipe polygon band geometry.
 */
// FUNCTION: FF7 0x005bd050
void __cdecl BattleFx_DrawScreenBandWipe(short taskSlot)
{
    int leftHeight;
    int clippedHeight;

    if (*(short *)g_BattleTaskSlotBuffer.secondarySlots[taskSlot].data == 0) {
        g_BattleTaskSlotBuffer.secondarySlots[taskSlot].ownerSlot = BATTLE_TASK_FREE;
    } else {
        if (g_BattleDarkenScreenGlobalAlphaEnabled != 0) {
            g_BattleOverlayDrawListHead = BattleFx_DrawScreenGradientBand(*(short *)&g_BattleViewportOriginX, *(short *)&g_BattleTaskSlotBuffer.secondarySlots[taskSlot].data[6] + g_BattleViewportOriginY, *(short *)&g_BattleViewportWidth, g_BattleUiScale * 0x32, &g_BattleBandAlphaOpaque);
            g_BattleOverlayDrawListHead = BattleFx_DrawScreenGradientBand(*(short *)&g_BattleViewportOriginX, ((*(short *)&g_BattleTaskSlotBuffer.secondarySlots[taskSlot].data[6] + g_BattleUiScale * 0x32) + g_BattleViewportOriginY), *(short *)&g_BattleViewportWidth, g_BattleUiScale * 0x21, &g_BattleBandAlphaFadeDown);
            leftHeight = g_BattleSceneViewportHeight - (*(short *)&g_BattleTaskSlotBuffer.secondarySlots[taskSlot].data[4] + g_BattleUiScale * 0x21);
            if (leftHeight > 0) {
                g_BattleOverlayDrawListHead = BattleFx_DrawScreenGradientBand(*(short *)&g_BattleViewportOriginX, *(short *)&g_BattleTaskSlotBuffer.secondarySlots[taskSlot].data[4] + g_BattleViewportOriginY, *(short *)&g_BattleViewportWidth, g_BattleUiScale * 0x21, &g_BattleBandAlphaFadeUp);
                g_BattleOverlayDrawListHead = BattleFx_DrawScreenGradientBand(*(short *)&g_BattleViewportOriginX, ((*(short *)&g_BattleTaskSlotBuffer.secondarySlots[taskSlot].data[4] + g_BattleUiScale * 0x21) + g_BattleViewportOriginY), *(short *)&g_BattleViewportWidth, (short)leftHeight, &g_BattleBandAlphaOpaque);
            } else {
                clippedHeight = g_BattleUiScale * 33 + leftHeight;
                if (clippedHeight > 0) {
                    g_BattleOverlayDrawListHead = BattleFx_DrawScreenGradientBand(*(short *)&g_BattleViewportOriginX, *(short *)&g_BattleTaskSlotBuffer.secondarySlots[taskSlot].data[4] + g_BattleViewportOriginY, *(short *)&g_BattleViewportWidth, (short)clippedHeight, &g_BattleBandAlphaFadeUp);
                }
            }
            *(short *)&g_BattleTaskSlotBuffer.secondarySlots[taskSlot].data[4] += (short)(g_BattleUiScale << 2);
            *(short *)&g_BattleTaskSlotBuffer.secondarySlots[taskSlot].data[6] -= (short)(g_BattleUiScale << 2);
        }
        (*(short *)g_BattleTaskSlotBuffer.secondarySlots[taskSlot].data)--;
    }
}

/*
 * Renders vertical gradient shading band across the battle viewport.
 */
// FUNCTION: FF7 0x005bd290
int __cdecl BattleFx_DrawScreenGradientBand(short x, short y, short width, short height, BattleFxGradientBandColor *colorMask)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        struct { unsigned char pad_to_3[0x3]; char commandByte; };
        struct { unsigned char pad_to_4[0x4]; BattleFxGradientQuad *gradientQuad; };
        struct { unsigned char pad_to_8[0x8]; BattleFxGradientBandColor *bandColor; };
    } frame;
#pragma pack(pop)

    g_BattleOverlayDrawListHead = (int)&g_GfxPsxPacketScratch;
    frame.gradientQuad = (BattleFxGradientQuad *)g_BattleOverlayDrawListHead;
    frame.gradientQuad->tag = frame.gradientQuad->tag & 0xFFFFFF | 0x8000000;
    frame.gradientQuad->command = 0x38;
    if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        frame.gradientQuad->command = frame.gradientQuad->command | 2;
        frame.commandByte = frame.gradientQuad->command;
    } else {
        frame.gradientQuad->command = frame.gradientQuad->command & (char)0xFD;
        frame.commandByte = frame.gradientQuad->command;
    }
    Gfx_SetPsxBlendMode(0);
    Gfx_SetPsxBlendEnable(1);
    frame.bandColor = colorMask;
    Gfx_SetPsxCornerAlphas(frame.bandColor->alphaTop, frame.bandColor->alphaTop, frame.bandColor->alphaBottom, frame.bandColor->alphaBottom);
    frame.gradientQuad->color0R = 0;
    frame.gradientQuad->color0G = 0;
    frame.gradientQuad->color0B = 0;
    frame.gradientQuad->color1R = 0;
    frame.gradientQuad->color1G = 0;
    frame.gradientQuad->color1B = 0;
    frame.gradientQuad->color2R = 0;
    frame.gradientQuad->color2G = 0;
    frame.gradientQuad->color2B = 0;
    frame.gradientQuad->color3R = 0;
    frame.gradientQuad->color3G = 0;
    frame.gradientQuad->color3B = 0;
    frame.gradientQuad->x0 = x;
    frame.gradientQuad->y0 = y;
    frame.gradientQuad->x1 = x + width;
    frame.gradientQuad->y1 = y;
    frame.gradientQuad->x2 = x;
    frame.gradientQuad->y2 = y + height;
    frame.gradientQuad->x3 = x + width;
    frame.gradientQuad->y3 = y + height;
    Gfx_SetBlendFactorPair(0, FLOAT_ONE_BITS);
    if (Gfx_IsDrawEnabled() != 0) {
        Gfx_DrawPsxPrimitive(frame.gradientQuad);
    }
    return (int)frame.gradientQuad;
}

/*
 * Draws full-screen additive white flash polygon.
 */
// FUNCTION: FF7 0x005bd436
int __cdecl BattleFx_FlashScreenWhite(unsigned char red, unsigned char green, unsigned char blue)
{
    return BattleFx_DrawFullScreenPlate((char)red, (char)green, (char)blue, 1, FLOAT_0_1_BITS);
}

/*
 * Draws full-screen translucent dark polygon to dim the scene.
 */
// FUNCTION: FF7 0x005bd456
void __cdecl BattleFx_DarkenScreenGlobalAlpha(char alpha, char green, char blue)
{
    BattleFx_DrawFullScreenPlate((char)alpha, (char)green, (char)blue, 2, 0);
}

/*
 * Renders colored full-screen quad plate with specified blend mode.
 */
// FUNCTION: FF7 0x005bd473
int __cdecl BattleFx_DrawFullScreenPlate(unsigned char red, unsigned char green, unsigned char blue, char mode, int blendFactor)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        struct { unsigned char pad_to_3[0x3]; char commandByte; };
        struct { unsigned char pad_to_4[0x4]; BattleFxOverlayPlate *plateQuad; };
    } frame;
#pragma pack(pop)

    g_BattleOverlayDrawListHead = (int)&g_GfxPsxPacketScratch;
    frame.plateQuad = (BattleFxOverlayPlate *)g_BattleOverlayDrawListHead;
    frame.plateQuad->tag = frame.plateQuad->tag & 0xFFFFFF | 0x5000000;
    frame.plateQuad->command = 0x28;
    if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        frame.plateQuad->command = frame.plateQuad->command | 2;
        frame.commandByte = frame.plateQuad->command;
    } else {
        frame.plateQuad->command = frame.plateQuad->command & (char)0xFD;
        frame.commandByte = frame.plateQuad->command;
    }
    if (mode == 2) {
        Gfx_SetPsxBlendMode(0);
        Gfx_SetPsxBlendEnable(1);
        Gfx_SetPsxAlpha(red);
        frame.plateQuad->colorR = 0;
        frame.plateQuad->colorG = 0;
        frame.plateQuad->colorB = 0;
    } else {
        frame.plateQuad->colorR = red;
        frame.plateQuad->colorG = green;
        frame.plateQuad->colorB = blue;
    }
    frame.plateQuad->x0 = *(short *)&g_BattleViewportOriginX;
    frame.plateQuad->y0 = *(short *)&g_BattleViewportOriginY;
    frame.plateQuad->x1 = g_BattleViewportMaxX + 1;
    frame.plateQuad->y1 = *(short *)&g_BattleViewportOriginY;
    frame.plateQuad->x2 = *(short *)&g_BattleViewportOriginX;
    frame.plateQuad->y2 = (*(int *)&g_BattleViewportMaxY) + 1;
    frame.plateQuad->x3 = g_BattleViewportMaxX + 1;
    frame.plateQuad->y3 = (*(int *)&g_BattleViewportMaxY) + 1;
    Gfx_SetBlendFactorPair(blendFactor, FLOAT_ONE_BITS);
    if (Gfx_IsDrawEnabled() != 0) {
        Gfx_DrawPsxPrimitive(frame.plateQuad);
    }
    return (int)frame.plateQuad;
}

/*
 * Swaps combatant 3D model with frog geometry and applies scale factor.
 */
// FUNCTION: FF7 0x005bd5e9
void __cdecl Battle_ApplyFrogModelSwap(char actor)
{
    int scriptBase;
    int modelIndex;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int extra;

    if ((g_BattleActorStates[actor].status & BATTLE_STATUS_FROG) != 0) {
        if ((g_BattleActorStates[actor].flags & 0x40) != 0) {
            return;
        }
        g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags | 0x40;
        g_BattleActorStates[actor].frogModelIndex = 6;
        g_BattleActorRecords[actor].scriptId = 5;
    } else {
        if ((g_BattleActorStates[actor].flags & 0x40) == 0) {
            return;
        }
        g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags & -0x41;
        if (actor < 4) {
            g_BattleActorStates[actor].frogModelIndex = actor;
            Battle_UnfrogPartyActorStub((char)actor);
        } else {
            g_BattleActorStates[actor].frogModelIndex = 0;
            g_BattleActorRecords[actor].isDirty = 1;
        }
    }
    g_BattleActorRecords[actor].stateFlags = g_BattleActorRecords[actor].stateFlags | 1;
    if (actor < 4) {
        Battle_BindPartyActorToModel((char)actor);
        if (actor == g_BattleActiveActorId && (g_BattleActorRecords[actor].scriptId == 0x1D || (g_BattleActorRecords[actor].scriptId == 0x1E || (g_BattleActorRecords[actor].scriptId == 0x1F || (g_BattleActorRecords[actor].scriptId == 0x20 || g_BattleActorRecords[actor].scriptId == 0x21))))) {
            g_BattleActorRecords[actor].isDirty = 0;
            g_BattleActorRecords[actor].scriptId = 0x10;
        }
        modelIndex = g_BattleActorStates[actor].frogModelIndex;
        scriptBase = *(int *)g_BattleModelBankSlots[modelIndex];
        g_BattleActorRecords[actor].model = (BattleActorModel *)g_BattleModelBankSlots[modelIndex];
        Battle_RunActorAnimScript(actor, (int)(scriptBase + 0x68), 0, 0);
    } else {
        Battle_BindEnemyActorToModel((char)actor);
        Battle_RunEnemyActorScript(actor, modelIndex, extra);
    }
}

/*
 * Synchronizes mini status model scaling factor with animation state.
 */
// FUNCTION: FF7 0x005bd847
void __cdecl Battle3d_SyncMiniScaleState(char actor)
{
    int slot;

    if ((g_BattleActorStates[actor].status & BATTLE_STATUS_SMALL) != 0) {
        if ((g_BattleActorStates[actor].flags & 0x80) != 0) {
            return;
        }
        g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags | 0x80;
        slot = (short)Battle_AllocateSecondaryResourceSlot(Battle3d_ActorScaleRampTask);
        *(short *)&g_BattleTaskSlotBuffer.secondarySlots[slot].data[2] = (short)*(volatile char *)&actor;
        *(short *)g_BattleTaskSlotBuffer.secondarySlots[slot].data = 0x10;
        g_BattleTaskSlotBuffer.secondarySlots[slot].frameCounter = 0xFF80;
    } else {
        if ((g_BattleActorStates[actor].flags & 0x80) == 0) {
            return;
        }
        g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags & -0x81;
        slot = (short)Battle_AllocateSecondaryResourceSlot(Battle3d_ActorScaleRampTask);
        *(short *)&g_BattleTaskSlotBuffer.secondarySlots[slot].data[2] = (short)*(volatile char *)&actor;
        *(short *)g_BattleTaskSlotBuffer.secondarySlots[slot].data = 0x10;
        g_BattleTaskSlotBuffer.secondarySlots[slot].frameCounter = 128;
    }
}

/*
 * Smoothly scales actor model dimensions during size transitions.
 */
// FUNCTION: FF7 0x005bd96d
void __cdecl Battle3d_ActorScaleRampTask(void)
{
    if (*(short *)g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data == 0) {
        g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = BATTLE_TASK_FREE;
    } else {
        g_BattleActorRecords[*(short *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data[2]].scale += g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter;
        (*(short *)g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].data)--;
    }
}

/*
 * Script-controlled scale ramp task for battle combatant.
 */
// FUNCTION: FF7 0x005bda0f
void __cdecl Battle3d_ScriptActorScaleRampTask(void)
{
    if (*(short *)g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].data == 0) {
        g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
        g_BattleActorRecords[*(short *)&BATTLE_CURRENT_TASK_SLOT->data[2]].isDirty = 1;
    } else {
        g_BattleActorRecords[*(short *)&BATTLE_CURRENT_TASK_SLOT->data[2]].scale += g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter;
        (*(short *)g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].data)--;
    }
}

/*
 * Initializes battle weather rain streak and splash particle buffers.
 */
// FUNCTION: FF7 0x005bdacf
void __cdecl Battle3d_RainInit(void)
{
    Battle3dRainRecord *cursor;
    Battle3dScene *graphicsData;
    PfileLoadOptions storage;
    Battle3dNode *particle;
    GfxModelTableBank *textureDesc;
    PfileLoadOptions *optionsRef;
    int i;

    particle = g_BattleEffectScratch.rainDrops;
    cursor = g_BattleEffectScratch.rainSplashes;
    for (i = 0; i < 256; i++) {
        particle->posX = (Battle_AiVm_Random16() & 0x3FFF) - 0x2000;
        particle->posY = 0xFFFFD8F0 - Battle_AiVm_Random16() / 2;
        particle->posZ = (Battle_AiVm_Random16() & 0x3FFF) - 0x2000;
        particle->rotX = 0;
        particle->yaw = 0;
        particle->rotZ = 0;
        particle->parentMatrix = (int)&g_BattleCameraTransform;
        particle++;
        cursor->splashFrame = 0;
        cursor->splashTexV = 0;
        cursor++;
    }
    if (g_pBattle3dScene != 0) {
        graphicsData = g_pBattle3dScene;
        textureDesc = (GfxModelTableBank *)(graphicsData->modelBank);
        if (textureDesc != 0) {
            optionsRef = &storage;
            Pfile_InitLoadOptionsWithBlend(BLEND_ADDITIVE, optionsRef);
            optionsRef->colorKeyEnabled = 1;
            optionsRef->blendFlags |= 1;
            optionsRef->drawPool = textureDesc->drawPool;
            optionsRef->directory = (char *)Battle_GetDataDirectory();
            // STRING: FF7 0x008fe250
            graphicsData->rainBatch = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, optionsRef, "RAIN7.TIM", (void *)textureDesc->primitiveMeshBank);
        }
    }
}

/*
 * Simulates rain particle physics and submits streak and splash primitives.
 */
// FUNCTION: FF7 0x005bdc4f
void __cdecl Battle3d_RainUpdateAndDraw(void)
{
    Battle3dNode *nodeAddr;
    int dropIdx;
    Battle3dRainRecord *stateAddr;

    nodeAddr = g_BattleEffectScratch.rainDrops;
    stateAddr = g_BattleEffectScratch.rainSplashes;
    for (dropIdx = 0; dropIdx < 256; dropIdx++) {
        switch (stateAddr->splashFrame) {
        case BATTLE_RAIN_FALLING_STREAK:
            if (nodeAddr->posY > 0)
                stateAddr->splashFrame = 1;
            else if (g_BattleIsPaused == 0)
                nodeAddr->posY += 400;
            break;
        case BATTLE_RAIN_SPLASH_END:
            stateAddr->splashFrame = 0;
            stateAddr->splashTexV = 0;
            nodeAddr->posY = -10000 - Battle_AiVm_Random16() / 2;
            break;
        default:
            if (g_BattleIsPaused == 0)
                nodeAddr->posY = -2;
            break;
        }
        RotMatrix((SVECTOR *)&nodeAddr->rotX, (PsxMatrix *)nodeAddr);
        Battle3d_ComposeNodeWithParent(nodeAddr);
        RefreshGlobalTransformState();
        switch (stateAddr->splashFrame) {
        case BATTLE_RAIN_FALLING_STREAK:
            if (g_BattleIsPaused == 0)
                g_BattleOverlayDrawListHead = Battle3d_DrawRainStreak(g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED);
            break;
        default:
            if (g_BattleIsPaused == 0) {
                (stateAddr->splashFrame)++;
                g_BattleOverlayDrawListHead = Battle3d_DrawRainSplash(g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_2, stateAddr->splashTexV);
                stateAddr->splashTexV += 0x20;
            }
            break;
        }
        nodeAddr++;
        stateAddr++;
    }
}

/*
 * Renders expanding ground splash circle for an expired rain drop.
 */
// FUNCTION: FF7 0x005bddc7
int __cdecl Battle3d_DrawRainSplash(int unused, short baseY)
{
    BattleRainSplashPrim *prim;
    GfxPsxSpriteQuadState *spriteState;
    int padWord[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    prim = (BattleRainSplashPrim *)Gfx_GetPsxPacketScratchPtr();
    prim->cmdWord = prim->cmdWord & 0xFFFFFF | 0x9000000;
    prim->gpuCode = 0x2C;
    prim->tpage = 0x28A;
    prim->clut = 0x7F94;
    prim->colorR = 0x80;
    prim->colorG = 0x80;
    prim->colorB = 0x80;
    Gfx_TransformQuadReturnDepthAlt((SVECTOR *)g_BattleRainSplashCorner0, (SVECTOR *)g_BattleRainSplashCorner1, (SVECTOR *)g_BattleRainSplashCorner2, (SVECTOR *)g_BattleRainSplashCorner3, &g_BattleVmReg_0, &g_BattleVmReg_2);
    prim->u0 = baseY + 1;
    prim->v0 = 1;
    prim->u1 = baseY + 0x1E;
    prim->v1 = 1;
    prim->u2 = baseY + 1;
    prim->v2 = 0x1F;
    prim->u3 = baseY + 0x1E;
    prim->v3 = 0x1F;
    spriteState = (GfxPsxSpriteQuadState *)(g_pBattle3dScene->rainSplashSpriteState);
    prim->gpuCode |= 2;
    Gfx_SetPsxPrimQuadState(0xB, 1, spriteState);
    if (Gfx_IsDrawEnabled() != 0) {
        Gfx_DrawPsxPrimitive(prim);
    }
    return 0;
}

/*
 * Renders falling translucent streak line primitive for rain effect.
 */
// FUNCTION: FF7 0x005bded8
int __cdecl Battle3d_DrawRainStreak(int orderingTable)
{
    int screenPosition;
    Battle3dRenderStateBlock *pRenderBlock;
    int depthResult;
    int depthOut;

    pRenderBlock = (Battle3dRenderStateBlock *)Gfx_GetPsxPacketScratchPtr();
    pRenderBlock->colorCommand = pRenderBlock->colorCommand & 0xFFFFFF | 0x4000000;
    pRenderBlock->opcode = 0x50;
    pRenderBlock->opcode |= 2;
    Gfx_TransformVertexSlot0ReturnDepth(&g_BattleRainStreakEnds[0], &screenPosition, &depthOut);
    depthResult = Gfx_TransformVertexSlot1ReturnDepth((GfxPsxShortVertexSlot *)&g_BattleRainStreakEnds[1], &screenPosition, &depthOut);
    pRenderBlock->colorR = 0x80;
    pRenderBlock->colorG = 0x80;
    pRenderBlock->colorB = 0x80;
    pRenderBlock->startColorR = 1;
    pRenderBlock->startColorG = 1;
    pRenderBlock->startColorB = 1;
    if (Gfx_IsDrawEnabled() != 0) {
        Gfx_DrawPsxPrimitive(pRenderBlock);
    }
    return (int)pRenderBlock;
}

/*
 * Resets battle dialog message window queue and pending string slots.
 */
// FUNCTION: FF7 0x005bdf85
void __cdecl Battle_ClearMessageWindowQueue(void)
{
    int i;

    for (i = 0; i < 64; i++) {
        g_BattleDisplayTextQueue[i].textId = 0xFFFF;
    }
}

/*
 * Enqueues battle banner message text string and display duration.
 */
// FUNCTION: FF7 0x005bdfb6
void __cdecl Battle_EnqueueMessageWindow(short textId, char windowType, int duration, short param)
{
    int i;

    for (i = 0; i < 64; i++) {
        if (g_BattleDisplayTextQueue[i].textId == -1) {
            g_BattleDisplayTextQueue[i].textId = textId;
            g_BattleDisplayTextQueue[i].windowType = windowType - 1;
            g_BattleDisplayTextQueue[i].param = param;
            if ((duration & 0xFF) == 0) {
                g_BattleDisplayTextQueue[i].duration = Battle_GetTextSpeedDelay();
            } else {
                g_BattleDisplayTextQueue[i].duration = (char)duration;
            }
            break;
        }
    }
}

/*
 * Advances active message window timer and displays next queued message.
 */
// FUNCTION: FF7 0x005be052
void __cdecl Battle_StepMessageWindowQueue(void)
{
    int entryIndex;
    int passIndex;

    g_BattleSceneFlags = g_BattleSceneFlags | 2;
    if (g_BattleDisplayTextQueue[0].textId != -1) {
        if (*(unsigned char *)&g_BattleDisplayTextQueue[0].windowType == 0) {
            if (g_BattleDisplayTextQueue[0].param != 0) {
                Battle_PlaySfxCenteredMulti(0x2D7);
                g_BattleDisplayTextQueue[0].param = 0;
            }
            if (*(unsigned char *)&g_BattleDisplayTextQueue[0].duration == 0) {
                g_BattleDisplayTextQueue[0].textId = 0xFFFF;
                for (passIndex = 0; passIndex < 64; passIndex++) {
                    if (g_BattleDisplayTextQueue[0].textId == -1) {
                        for (entryIndex = 0; entryIndex < 63; entryIndex++) {
                            g_BattleDisplayTextQueue[entryIndex].textId = g_BattleDisplayTextQueue[entryIndex + 1].textId;
                            g_BattleDisplayTextQueue[entryIndex].windowType = g_BattleDisplayTextQueue[entryIndex + 1].windowType;
                            g_BattleDisplayTextQueue[entryIndex].duration = g_BattleDisplayTextQueue[entryIndex + 1].duration;
                            g_BattleDisplayTextQueue[entryIndex].param = g_BattleDisplayTextQueue[entryIndex + 1].param;
                        }
                        g_BattleDisplayTextQueue[63].textId = 0xFFFF;
                    }
                }
                return;
            }
            if (Gfx_IsDrawEnabled() != 0) {
                Battle_SetMessageWindowRequest((short)g_BattleDisplayTextQueue[0].textId);
            }
            if (g_BattleIsPaused == 0 && g_BattleIsRunning != 0) {
                g_BattleDisplayTextQueue[0].duration--;
            }
            return;
        }
        if (g_BattleIsPaused == 0 && g_BattleIsRunning != 0) {
            g_BattleDisplayTextQueue[0].windowType--;
        }
    }
    Battle_SetMessageWindowRequest(-1);
    g_BattleSceneFlags = g_BattleSceneFlags & 0xFFFFFFFD;
}

/*
 * Sorts the party's model load slots by load state (two bubble passes over three slots), keeping
 * each slot's party index in modelId. The third slot lies past this TU's two-slot definition (the
 * next definition, g_BattleOverlayRootLoadState); battle_opening_actor_setup.cpp declares all three.
 */
// FUNCTION: FF7 0x005be20b
void __cdecl Battle_SortPartyModelLoadOrder(void)
{
    struct MenuPartyStats *statBlock;
    int partyIndex;
    int sortPass;
    int temp;

    for (partyIndex = 0; partyIndex < 3; partyIndex++) {
        if (g_BattleScene.actors[partyIndex].unitIndex == -1) {
            g_BattlePartyModelLoadSlots[partyIndex].loadState = 0xC8;
        } else {
            switch ((char)g_BattleScene.actors[partyIndex].unitIndex) {
            case PARTY_CHAR_BARRET:
                statBlock = Savemap_GetPartyStatBlock(partyIndex);
                temp = statBlock->paramIndex & 0xF0;
                switch (temp) {
                case BATTLE_WEAPON_MODEL_ALT_10:
                    g_BattlePartyModelLoadSlots[partyIndex].loadState = 0xB;
                    break;
                case BATTLE_WEAPON_MODEL_ALT_20:
                    g_BattlePartyModelLoadSlots[partyIndex].loadState = 0xC;
                    break;
                case BATTLE_WEAPON_MODEL_ALT_30:
                    g_BattlePartyModelLoadSlots[partyIndex].loadState = 0xD;
                    break;
                default:
                    g_BattlePartyModelLoadSlots[partyIndex].loadState = 1;
                }
                break;
            case PARTY_CHAR_VINCENT:
                statBlock = Savemap_GetPartyStatBlock(partyIndex);
                temp = statBlock->paramIndex & 0xF0;
                switch (temp) {
                case BATTLE_WEAPON_MODEL_ALT_10:
                    g_BattlePartyModelLoadSlots[partyIndex].loadState = 0xE;
                    break;
                case BATTLE_WEAPON_MODEL_ALT_20:
                    g_BattlePartyModelLoadSlots[partyIndex].loadState = 0xF;
                    break;
                default:
                    g_BattlePartyModelLoadSlots[partyIndex].loadState = 7;
                }
                break;
            default:
                if (g_BattleScene.locationId == 0x4E && g_BattleScene.actors[partyIndex].unitIndex == 0) {
                    g_BattlePartyModelLoadSlots[partyIndex].loadState = 0x10;
                } else {
                    g_BattlePartyModelLoadSlots[partyIndex].loadState = g_BattleScene.actors[partyIndex].unitIndex;
                }
            }
        }
        g_BattlePartyModelLoadSlots[partyIndex].modelId = partyIndex;
    }
    for (sortPass = 0; sortPass < 2; sortPass++) {
        for (partyIndex = 0; partyIndex < 2; partyIndex++) {
            if (g_BattlePartyModelLoadSlots[partyIndex].loadState > g_BattlePartyModelLoadSlots[partyIndex + 1].loadState) {
                temp = g_BattlePartyModelLoadSlots[partyIndex].loadState;
                g_BattlePartyModelLoadSlots[partyIndex].loadState = g_BattlePartyModelLoadSlots[partyIndex + 1].loadState;
                g_BattlePartyModelLoadSlots[partyIndex + 1].loadState = temp;
                temp = g_BattlePartyModelLoadSlots[partyIndex].modelId;
                g_BattlePartyModelLoadSlots[partyIndex].modelId = g_BattlePartyModelLoadSlots[partyIndex + 1].modelId;
                g_BattlePartyModelLoadSlots[partyIndex + 1].modelId = temp;
            }
        }
    }
}

/*
 * Returns message display delay ticks based on user text speed setting.
 */
// FUNCTION: FF7 0x005be475
int __cdecl Battle_GetTextSpeedDelay(void)
{
    return (*(unsigned char *)&gSavemap.battleMessageSpeed >> 2) + 4;
}

/*
 * No-operation placeholder task callback.
 */
// FUNCTION: FF7 0x005be487
void __cdecl Battle_TaskNop(void)
{
}
} /* extern "C" */
