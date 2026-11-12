// src/ff7/src/battle/battle_camera_channel_scripts.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The battle camera system: runs the eye and focus channel scripts, moves, bone following
//   and snaps, then publishes the final camera matrices each frame. Helper math covers orbiting,
//   easing and offsets.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/enums/battle.h"
#include "ff7/enums/menu.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/api/movie.h"
#include "lib/codegen.h"
#include "ff7/savemap.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008fee2c
unsigned char g_BattleCameraEyeDefaultScript[4] = {0xff, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fee30
unsigned char g_BattleCameraFocusDefaultScript[4] = {0xff, 0x00, 0x00, 0x00};
// clang-format on

/*
 * Runs one frame of the battle camera scripts. Unless an effect has taken the camera
 * (g_BattleScriptBusy, set by an effect task and cleared by the motion script's resume-camera
 * opcode or a new sequence step), it steps the focus script and then the eye script of each of
 * the three camera channels, passing `subArg` as the script parameter and `scriptArg` as the
 * script group: a group selects entry group * 3 + channel of the camera file's tables, -3 the
 * per-channel entry of the second table, -1 the empty default script and -2 the script indexed
 * by g_BattleCameraScriptIndex. Then it runs the auxiliary task slots, where the camera's move and
 * follow tasks live (Battle_RunAuxTaskSlots). The battle frame calls it with g_BattleCameraTargetIndex
 * and the active actor's effect id, then publishes the camera (Battle_PublishCameraForFrame).
 */
// FUNCTION: FF7 0x005c2070
void __cdecl Battle_RunCameraChannels(short scriptArg, short subArg)
{
    int channel;

    if (g_BattleScriptBusy == 0) {
        for (channel = 0; channel < 3; channel++) {
            Battle_RunCameraFocusScript((char)channel, (short)subArg, (short)scriptArg);
            Battle_RunCameraEyeScript((char)channel, (short)subArg, (short)scriptArg);
        }
        Battle_RunAuxTaskSlots();
    }
}

/*
 * Computes and publishes final camera eye and focus matrices for current frame.
 */
// FUNCTION: FF7 0x005c20ce
void __cdecl Battle_PublishCameraForFrame(short cameraMode)
{
    if (g_BattleScene.locationId == 0x4E) {
        gSavemap.options = gSavemap.options & ~BATTLE_CAMERA_OPTION_FIXED;
        gSavemap.options = gSavemap.options & ~0x200;
    }
    if (g_BattleRoundSequenceState == 0x64) {
        if ((gSavemap.options & BATTLE_CAMERA_OPTION_MASK) == BATTLE_CAMERA_OPTION_FIXED) {
            Battle_PublishFixedPlacementCamera();
            return;
        }
        g_BattleCameraIsMoving = 1;
        Battle_PublishCameraChannel(g_BattleVariationIndex);
    } else if (g_BattleMenuState == BATTLE_MENU_SELECT_TARGET && g_BattleScriptEffectFlag == 0) {
        Battle_PublishAutoCamera();
    } else {
        switch (cameraMode) {
        case BATTLE_CAMERA_MODE_AUTO:
            if ((gSavemap.options & BATTLE_CAMERA_OPTION_MASK) == BATTLE_CAMERA_OPTION_FIXED) {
                Battle_PublishFixedPlacementCamera();
            } else {
                Battle_PublishAutoCamera();
            }
            break;
        case BATTLE_CAMERA_MODE_CHANNELS:
            g_BattleCameraIsMoving = 1;
            Battle_PublishCameraChannel(g_BattleVariationIndex);
            break;
        default:
            if ((gSavemap.options & BATTLE_CAMERA_OPTION_MASK) == BATTLE_CAMERA_OPTION_FIXED) {
                if (cameraMode == 0x3B || (cameraMode == 0x3C || (cameraMode == 0x45 || (cameraMode == 0x59 || (cameraMode == 0x6D || (cameraMode == 0x77 || cameraMode == 0xCB)))))) {
                    if (g_BattleSequenceStepReady == 1) {
                        Battle_PublishAutoCamera();
                    } else {
                        g_BattleCameraIsMoving = 1;
                        Battle_PublishCameraChannel(g_BattleVariationIndex);
                    }
                } else {
                    Battle_PublishFixedPlacementCamera();
                }
            } else if (g_BattleSequenceStepReady == 1) {
                if (g_BattleScene.locationId == 0x4E) {
                    g_BattleCameraIsMoving = 1;
                    Battle_PublishCameraChannel(g_BattleVariationIndex);
                } else {
                    Battle_PublishAutoCamera();
                }
            } else {
                g_BattleCameraIsMoving = 1;
                Battle_PublishCameraChannel(g_BattleVariationIndex);
            }
        }
    }
}

/*
 * Publishes automatic battle tracking camera channel.
 */
// FUNCTION: FF7 0x005c22a9
void __cdecl Battle_PublishAutoCamera(void)
{
    Battle_SettleAutoCameraChannel();
    Battle_PublishCameraChannel(3);
}

/*
 * Publishes fixed placement camera position and orientation.
 */
// FUNCTION: FF7 0x005c22bd
void __cdecl Battle_PublishFixedPlacementCamera(void)
{
    g_BattleCameraPosition.cameraPosX = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].eyeX;
    g_BattleCameraPosition.cameraPosY = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].eyeY;
    g_BattleCameraPosZ = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].eyeZ;
    g_BattleCameraFocalPoint.focalPointX = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].focusX;
    g_BattleCameraFocalPoint.focalPointY = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].focusY;
    g_BattleCameraFocusZ = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].focusZ;
}

/*
 * Writes camera channel coordinates into battle view transform.
 */
// FUNCTION: FF7 0x005c2350
void __cdecl Battle_PublishCameraChannel(short channel)
{
    g_BattleCameraPosition.cameraPosX = g_BattleCameraEyeChannels[channel].x;
    g_BattleCameraPosition.cameraPosY = g_BattleCameraEyeChannels[channel].y;
    g_BattleCameraPosZ = g_BattleCameraEyeChannels[channel].z;
    g_BattleCameraFocalPoint.focalPointX = g_BattleCameraFocusChannels[channel].x;
    g_BattleCameraFocalPoint.focalPointY = g_BattleCameraFocusChannels[channel].y;
    g_BattleCameraFocusZ = g_BattleCameraFocusChannels[channel].z;
}

/*
 * Opcode interpreter running battle camera eye movement scripts.
 */
// FUNCTION: FF7 0x005c23d1
void __cdecl Battle_RunCameraEyeScript(char channel, short param, short scriptGroup)
{
    BattleCameraMoveTask *track;
    BattleCameraScriptRoot *rootPtr;
    int slotIdx;
    int *tablePtr;
    BattleActorAnimState *pActorState;
    int relOffset;

    pActorState = g_BattleActor_BufferPtr;
    pActorState->running = 1;
    rootPtr = (BattleCameraScriptRoot *)g_BattleCameraGlobalScripts;
    relOffset = rootPtr->eyeScriptAddr - g_BattleCameraFileAddressBase;
    tablePtr = (int *)(g_BattleCameraGlobalScripts + relOffset);
    relOffset = tablePtr[scriptGroup * 3 + channel] - g_BattleCameraFileAddressBase;
    pActorState->scriptPtr = (char *)(g_BattleCameraGlobalScripts + relOffset);
    if (scriptGroup == -3) {
        rootPtr = (BattleCameraScriptRoot *)g_BattleCameraGlobalScripts;
        relOffset = rootPtr->eyeScriptOffset - g_BattleCameraFileAddressBase;
        tablePtr = (int *)(g_BattleCameraGlobalScripts + relOffset);
        relOffset = tablePtr[channel] - g_BattleCameraFileAddressBase;
        pActorState->scriptPtr = (char *)(g_BattleCameraGlobalScripts + relOffset);
    }
    if (scriptGroup == -1) {
        pActorState->scriptPtr = (char *)g_BattleCameraEyeDefaultScript;
    }
    if (scriptGroup == -2) {
        pActorState->scriptPtr = (char *)g_BattleCameraPositionScripts[*(unsigned char *)&g_BattleCameraScriptIndex];
    }
    if (g_BattleCameraEyeChannels[channel].scriptOffset == 0xFF) {
        g_BattleCameraEyeChannels[channel].rotateOffsets = 0;
        g_BattleCameraEyeChannels[channel].scriptOffset = 0;
        g_BattleCameraEyeChannels[channel].waitFrames = 0;
        g_BattleCameraEyeChannels[channel].eyeLinearMoves = 0;
    }
    while (pActorState->running != 0) {
        pActorState->opcode = pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
        (g_BattleCameraEyeChannels[channel].scriptOffset)++;
        switch (*(unsigned char *)&pActorState->opcode) {
        case BATTLE_CAMERA_EYE_LOOP:
            if (g_BattleCameraEyeChannels[channel].waitFrames == 0) {
                pActorState->opcode = pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
                if (pActorState->opcode == 0xC0) {
                    g_BattleCameraEyeChannels[channel].scriptOffset = 0;
                    g_BattleCameraEyeChannels[channel].waitFrames = 0;
                }
            }
            break;
        case BATTLE_CAMERA_EYE_SET_VM_FLAG:
            g_BattleVmReg_0 = pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleVmReg_1 = pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleScriptFlags[g_BattleVmReg_0] = *(char *)&g_BattleVmReg_1;
            Battle_ApplyVmFlagArrayBit0();
            break;
        case BATTLE_CAMERA_EYE_PLAY_SFX:
            g_BattleVmReg_0 = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            Battle_PlaySfxPannedToActor(*(short *)&g_BattleVmReg_0, -1, 0x40);
            break;
        case BATTLE_CAMERA_EYE_FLASH_WHITE:
            Battle_PlaySfxPannedToActor(0x10C, -1, 0x40);
            g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(0xFA, 0xFA, 0xFA);
            break;
        case BATTLE_CAMERA_EYE_LOAD_LIVE_POS:
            g_BattleCameraEyeChannels[channel].x = g_BattleCameraPosition.cameraPosX;
            g_BattleCameraEyeChannels[channel].y = g_BattleCameraPosition.cameraPosY;
            g_BattleCameraEyeChannels[channel].z = g_BattleCameraPosZ;
            break;
        case BATTLE_CAMERA_EYE_SNAP_FORMATION_CAM:
            g_BattleCameraEyeChannels[channel].x = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].eyeX;
            g_BattleCameraEyeChannels[channel].y = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].eyeY;
            g_BattleCameraEyeChannels[channel].z = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].eyeZ;
            break;
        case BATTLE_CAMERA_EYE_SET_FORMATION_CAM:
            g_BattleCurrFormationCameraIdx = pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            break;
        case BATTLE_CAMERA_EYE_MOVE_TO_FORMATION_CAM:
            slotIdx = (short)Battle_AllocateAuxTaskSlot(Battle_CameraEyeMoveTask);
            track = (BattleCameraMoveTask *)&g_BattleAuxTaskSlotBuffer[slotIdx].ownerSlot;
            track->targetX = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].eyeX;
            track->targetY = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].eyeY;
            track->targetZ = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].eyeZ;
            track->framesLeft = (short)pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            track->startX = g_BattleCameraEyeChannels[channel].x;
            track->startY = g_BattleCameraEyeChannels[channel].y;
            track->startZ = g_BattleCameraEyeChannels[channel].z;
            track->channel = (short)channel;
            break;
        case BATTLE_CAMERA_EYE_MOVES_EASED:
            g_BattleCameraEyeChannels[channel].eyeLinearMoves = 0;
            break;
        case BATTLE_CAMERA_EYE_MOVES_LINEAR:
            g_BattleCameraEyeChannels[channel].eyeLinearMoves = 1;
            break;
        case BATTLE_CAMERA_EYE_SET_TARGET_3:
            if (pActorState->scriptPtr[(g_BattleCameraEyeChannels[channel].scriptOffset)++] != 0) {
                g_BattleCameraFocusActor = 3;
            }
            break;
        case BATTLE_CAMERA_EYE_SET_POSITION:
            g_BattleCameraEyeChannels[channel].x = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleCameraEyeChannels[channel].y = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleCameraEyeChannels[channel].z = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            break;
        case BATTLE_CAMERA_EYE_ORBIT:
            slotIdx = (short)Battle_AllocateAuxTaskSlot(Battle_CameraEyeOrbitTask);
            g_BattleAuxTaskSlotBuffer[slotIdx].angleStep = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[slotIdx].radius = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[slotIdx].orbitAngle = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[slotIdx].radiusStep = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[slotIdx].positionStep = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[slotIdx].countdown = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[slotIdx].channel = channel;
            break;
        case BATTLE_CAMERA_EYE_SET_WAIT:
            g_BattleCameraEyeChannels[channel].waitFrames = (short)pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            break;
        case BATTLE_CAMERA_EYE_WAIT:
            if (g_BattleCameraEyeChannels[channel].waitFrames != 0) {
                (g_BattleCameraEyeChannels[channel].waitFrames)--;
                (g_BattleCameraEyeChannels[channel].scriptOffset)--;
                pActorState->running = 0;
            }
            break;
        case BATTLE_CAMERA_EYE_ROTATE_OFFSETS_ON:
            g_BattleCameraEyeChannels[channel].rotateOffsets = 1;
            break;
        case BATTLE_CAMERA_EYE_ROTATE_OFFSETS_OFF:
            g_BattleCameraEyeChannels[channel].rotateOffsets = 0;
            break;
        case BATTLE_CAMERA_EYE_TARGET_GROUP:
            g_BattleCameraFocusActor = BATTLE_CAMERA_ACTOR_TARGETS;
            break;
        case BATTLE_CAMERA_EYE_POLAR_TARGET:
            g_BattleVmReg_3 = pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleVmReg_2 = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            pActorState->secondarySlot = 0x1000 - (g_BattleActorRecords[g_BattleCameraFocusActor].yaw + 0x800) + g_BattleVmReg_2;
            pActorState->angle = (g_BattleActorRecords[g_BattleActiveActorId].radius * g_BattleActorRecords[g_BattleActiveActorId].scale >> 12) + (g_BattleActorRecords[g_BattleCameraFocusActor].radius * g_BattleActorRecords[g_BattleCameraFocusActor].scale >> 12);
            g_BattleCameraEyeChannels[channel].x = g_BattleActorRecords[g_BattleCameraFocusActor].posX - ((SinFixed(pActorState->secondarySlot) * pActorState->angle) / 4096 << g_BattleVmReg_3);
            g_BattleCameraEyeChannels[channel].z = g_BattleActorRecords[g_BattleCameraFocusActor].posZ + ((CosFixed(pActorState->secondarySlot) * pActorState->angle) / 4096 << g_BattleVmReg_3);
            g_BattleCameraEyeChannels[channel].y = g_BattleCameraFocusChannels[channel].y - (short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            break;
        case BATTLE_CAMERA_EYE_POLAR_ATTACKER:
            g_BattleVmReg_3 = pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleVmReg_2 = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            pActorState->secondarySlot = 0x1000 - (g_BattleActorRecords[g_BattleActiveActorId].yaw + 0x800) + g_BattleVmReg_2;
            pActorState->angle = (g_BattleActorRecords[g_BattleActiveActorId].radius * g_BattleActorRecords[g_BattleActiveActorId].scale >> 12) + (g_BattleActorRecords[g_BattleCameraFocusActor].radius * g_BattleActorRecords[g_BattleCameraFocusActor].scale >> 12);
            g_BattleCameraEyeChannels[channel].x = g_BattleActorRecords[g_BattleActiveActorId].posX - ((SinFixed(pActorState->secondarySlot) * pActorState->angle) / 4096 << g_BattleVmReg_3);
            g_BattleCameraEyeChannels[channel].z = g_BattleActorRecords[g_BattleActiveActorId].posZ + ((CosFixed(pActorState->secondarySlot) * pActorState->angle) / 4096 << g_BattleVmReg_3);
            g_BattleCameraEyeChannels[channel].y = g_BattleCameraFocusChannels[channel].y - (short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            break;
        case BATTLE_CAMERA_EYE_SNAP_BONE_ACTOR:
            g_BattleVmReg_3 = pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleVmReg_5 = (&g_BattleActorRecords[g_BattleVmReg_3].targetBone)[pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset]];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleVmReg_0 = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleVmReg_1 = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleVmReg_2 = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            Battle_CameraEyeSnapToBone(*(short *)&g_BattleVmReg_3, *(short *)&g_BattleVmReg_5, channel, 0);
            pActorState->running = 0;
            break;
        case BATTLE_CAMERA_EYE_SNAP_BONE_TARGET:
            g_BattleVmReg_5 = (&g_BattleActorRecords[g_BattleCameraFocusActor].targetBone)[pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset]];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleVmReg_0 = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleVmReg_1 = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleVmReg_2 = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            Battle_CameraEyeSnapToBone(g_BattleCameraFocusActor, *(short *)&g_BattleVmReg_5, channel, 1);
            pActorState->running = 0;
            break;
        case BATTLE_CAMERA_EYE_SNAP_BONE_ATTACKER:
            g_BattleVmReg_5 = (&g_BattleActorRecords[g_BattleActiveActorId].targetBone)[pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset]];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleVmReg_0 = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleVmReg_1 = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleVmReg_2 = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            Battle_CameraEyeSnapToBone(g_BattleActiveActorId, *(short *)&g_BattleVmReg_5, channel, 0);
            pActorState->running = 0;
            break;
        case BATTLE_CAMERA_EYE_FOLLOW_BONE_ACTOR:
            pActorState->angle = Battle_AllocateAuxTaskSlot(Battle_CameraEyeFollowBoneTask);
            g_BattleAuxTaskSlotBuffer[pActorState->angle].angleStep = (short)pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            g_BattleVmReg_3 = g_BattleAuxTaskSlotBuffer[pActorState->angle].angleStep;
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleAuxTaskSlotBuffer[pActorState->angle].radius = (short)(&g_BattleActorRecords[g_BattleVmReg_3].targetBone)[pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset]];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleAuxTaskSlotBuffer[pActorState->angle].radiusStep = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[pActorState->angle].cameraValue = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[pActorState->angle].positionStep = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[pActorState->angle].countdown = (short)pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleAuxTaskSlotBuffer[pActorState->angle].orbitAngle = (short)channel;
            g_BattleAuxTaskSlotBuffer[pActorState->angle].channel = 0;
            break;
        case BATTLE_CAMERA_EYE_FOLLOW_BONE_TARGET:
            pActorState->angle = Battle_AllocateAuxTaskSlot(Battle_CameraEyeFollowBoneTask);
            g_BattleAuxTaskSlotBuffer[pActorState->angle].angleStep = (short)g_BattleCameraFocusActor;
            g_BattleAuxTaskSlotBuffer[pActorState->angle].radius = (short)(&g_BattleActorRecords[g_BattleCameraFocusActor].targetBone)[pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset]];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleAuxTaskSlotBuffer[pActorState->angle].radiusStep = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[pActorState->angle].cameraValue = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[pActorState->angle].positionStep = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[pActorState->angle].countdown = (short)pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleAuxTaskSlotBuffer[pActorState->angle].orbitAngle = (short)channel;
            g_BattleAuxTaskSlotBuffer[pActorState->angle].channel = 1;
            break;
        case BATTLE_CAMERA_EYE_FOLLOW_BONE_ATTACKER:
            pActorState->angle = Battle_AllocateAuxTaskSlot(Battle_CameraEyeFollowBoneTask);
            g_BattleAuxTaskSlotBuffer[pActorState->angle].angleStep = (short)g_BattleActiveActorId;
            g_BattleAuxTaskSlotBuffer[pActorState->angle].radius = (short)(&g_BattleActorRecords[g_BattleActiveActorId].targetBone)[pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset]];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleAuxTaskSlotBuffer[pActorState->angle].radiusStep = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[pActorState->angle].cameraValue = (int)(short)Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[pActorState->angle].positionStep = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            g_BattleAuxTaskSlotBuffer[pActorState->angle].countdown = (short)pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleAuxTaskSlotBuffer[pActorState->angle].orbitAngle = (short)channel;
            g_BattleAuxTaskSlotBuffer[pActorState->angle].channel = 0;
            break;
        case BATTLE_CAMERA_EYE_MOVE_TO_XYZ:
            slotIdx = (short)Battle_AllocateAuxTaskSlot(Battle_CameraEyeMoveTask);
            track = (BattleCameraMoveTask *)&g_BattleAuxTaskSlotBuffer[slotIdx].ownerSlot;
            track->targetX = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            track->targetY = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            track->targetZ = Battle_CameraEyeFetchWord(pActorState->scriptPtr, (char)channel);
            track->framesLeft = (short)pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            track->startX = g_BattleCameraEyeChannels[channel].x;
            track->startY = g_BattleCameraEyeChannels[channel].y;
            track->startZ = g_BattleCameraEyeChannels[channel].z;
            track->channel = (short)channel;
            break;
        case BATTLE_CAMERA_EYE_MOVE_TO_TARGET_CAMREC:
            g_BattleVmReg_5 = pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleVmReg_3 = pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleVmReg_0 = g_BattleActorStates[g_BattleCameraFocusActor].cameraOffsets[g_BattleVmReg_5].eyeX + g_BattleActorRecords[g_BattleCameraFocusActor].posX;
            g_BattleVmReg_2 = g_BattleActorStates[g_BattleCameraFocusActor].cameraOffsets[g_BattleVmReg_5].eyeZ + g_BattleActorRecords[g_BattleCameraFocusActor].posZ;
            if (g_BattleVmReg_5 == 0) {
                g_BattleVmReg_1 = g_BattleActorStates[g_BattleCameraFocusActor].cameraOffsets[g_BattleVmReg_5].eyeY;
            } else {
                g_BattleVmReg_1 = g_BattleActorStates[g_BattleCameraFocusActor].cameraOffsets[g_BattleVmReg_5].eyeY + g_BattleActorRecords[g_BattleCameraFocusActor].posY;
            }
            slotIdx = (short)Battle_AllocateAuxTaskSlot(Battle_CameraEyeMoveTask);
            track = (BattleCameraMoveTask *)&g_BattleAuxTaskSlotBuffer[slotIdx].ownerSlot;
            track->targetX = *(short *)&g_BattleVmReg_0;
            track->targetY = *(short *)&g_BattleVmReg_1;
            track->targetZ = *(short *)&g_BattleVmReg_2;
            track->framesLeft = *(short *)&g_BattleVmReg_3;
            track->startX = g_BattleCameraEyeChannels[channel].x;
            track->startY = g_BattleCameraEyeChannels[channel].y;
            track->startZ = g_BattleCameraEyeChannels[channel].z;
            track->channel = (short)channel;
            break;
        case BATTLE_CAMERA_EYE_MOVE_TO_BONE_FROM_TABLE:
            g_BattleVmReg_7 = g_BattleHitTargetActors[pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset]];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleVmReg_5 = (&g_BattleActorRecords[g_BattleVmReg_7].targetBone)[pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset]];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            Battle_CameraEyeQueueMoveToBone(*(short *)&g_BattleVmReg_7, (char)channel, pActorState->scriptPtr, 1);
            break;
        case BATTLE_CAMERA_EYE_MOVE_TO_BONE_TARGET:
            g_BattleVmReg_5 = (&g_BattleActorRecords[g_BattleCameraFocusActor].targetBone)[pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset]];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            Battle_CameraEyeQueueMoveToBone(g_BattleCameraFocusActor, (char)channel, pActorState->scriptPtr, 1);
            break;
        case BATTLE_CAMERA_EYE_MOVE_TO_BONE_ATTACKER:
            g_BattleVmReg_5 = (&g_BattleActorRecords[g_BattleActiveActorId].targetBone)[pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset]];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            Battle_CameraEyeQueueMoveToBone(g_BattleActiveActorId, (char)channel, pActorState->scriptPtr, 0);
            break;
        case BATTLE_CAMERA_EYE_MOVE_TO_BONE_ACTOR:
            g_BattleVmReg_4 = pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            g_BattleVmReg_5 = (&g_BattleActorRecords[g_BattleVmReg_4].targetBone)[pActorState->scriptPtr[g_BattleCameraEyeChannels[channel].scriptOffset]];
            (g_BattleCameraEyeChannels[channel].scriptOffset)++;
            Battle_CameraEyeQueueMoveToBone(*(short *)&g_BattleVmReg_4, (char)channel, pActorState->scriptPtr, 0);
            break;
        case BATTLE_CAMERA_EYE_END:
            (g_BattleCameraEyeChannels[channel].scriptOffset)--;
            pActorState->running = 0;
            break;
        default:
            pActorState->running = 0;
        }
    }
}

/*
 * Updates spherical orbiting camera eye motion around focus point.
 */
// FUNCTION: FF7 0x005c3d0d
void __cdecl Battle_CameraEyeOrbitTask(void)
{
    int taskSlot;
    int channelIdx;

    taskSlot = g_BattleCameraAuxTaskSlot;
    channelIdx = g_BattleAuxTaskSlotBuffer[taskSlot].channel;
    if (g_BattleAuxTaskSlotBuffer[taskSlot].countdown == 1) {
        g_BattleAuxTaskSlotBuffer[taskSlot].ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleAuxTaskSlotBuffer[taskSlot].orbitAngle += g_BattleAuxTaskSlotBuffer[taskSlot].angleStep;
    g_BattleCameraEyeChannels[channelIdx].x = (SinFixed(g_BattleAuxTaskSlotBuffer[taskSlot].orbitAngle) * g_BattleAuxTaskSlotBuffer[taskSlot].radius >> 9) + g_BattleCameraFocusChannels[channelIdx].x;
    g_BattleCameraEyeChannels[channelIdx].z = (CosFixed(g_BattleAuxTaskSlotBuffer[taskSlot].orbitAngle) * g_BattleAuxTaskSlotBuffer[taskSlot].radius >> 9) + g_BattleCameraFocusChannels[channelIdx].z;
    g_BattleCameraEyeChannels[channelIdx].y += g_BattleAuxTaskSlotBuffer[taskSlot].positionStep;
    g_BattleAuxTaskSlotBuffer[taskSlot].radius += g_BattleAuxTaskSlotBuffer[taskSlot].radiusStep;
    (g_BattleAuxTaskSlotBuffer[taskSlot].countdown)--;
}

/*
 * Queues smooth camera eye translation toward target actor bone.
 */
// FUNCTION: FF7 0x005c3e6f
void __cdecl Battle_CameraEyeQueueMoveToBone(short actorIndex, char channel, char *scriptPtr, int flags)
{
    BattleCameraMoveTask *move;
    int slotIdx;

    g_BattleVmReg_0 = (short)Battle_CameraEyeFetchWord(scriptPtr, (char)channel);
    g_BattleVmReg_1 = (short)Battle_CameraEyeFetchWord(scriptPtr, (char)channel);
    g_BattleVmReg_2 = (short)Battle_CameraEyeFetchWord(scriptPtr, (char)channel);
    g_BattleVmReg_3 = ((unsigned char *)scriptPtr)[g_BattleCameraEyeChannels[channel].scriptOffset];
    (g_BattleCameraEyeChannels[channel].scriptOffset)++;
    Battle_CameraEyeAddBoneOrigin((short)actorIndex, *(short *)&g_BattleVmReg_5, (char)channel, (char)flags);
    slotIdx = (short)Battle_AllocateAuxTaskSlot(Battle_CameraEyeMoveTask);
    move = (BattleCameraMoveTask *)&g_BattleAuxTaskSlotBuffer[slotIdx].ownerSlot;
    move->targetX = *(short *)&g_BattleVmReg_0;
    move->targetY = *(short *)&g_BattleVmReg_1;
    move->targetZ = *(short *)&g_BattleVmReg_2;
    move->framesLeft = *(short *)&g_BattleVmReg_3;
    move->startX = g_BattleCameraEyeChannels[channel].x;
    move->startY = g_BattleCameraEyeChannels[channel].y;
    move->startZ = g_BattleCameraEyeChannels[channel].z;
    move->channel = channel;
}

/*
 * Opcode interpreter running battle camera focus look-at scripts.
 */
// FUNCTION: FF7 0x005c3fd5
void __cdecl Battle_RunCameraFocusScript(char slot, short param, short mode)
{
    BattleCameraScriptRoot *scriptBase;
    int slotIndex;
    BattleCameraMoveTask *trackPtr;
    int ofs;
    int *table;
    BattleActorAnimState *animState;

    animState = g_BattleActor_BufferPtr;
    animState->running = 1;
    scriptBase = (BattleCameraScriptRoot *)g_BattleCameraGlobalScripts;
    ofs = scriptBase->focusScriptAddr - g_BattleCameraFileAddressBase;
    table = (int *)(g_BattleCameraGlobalScripts + ofs);
    ofs = table[mode * 3 + slot] - g_BattleCameraFileAddressBase;
    animState->scriptPtr = (char *)(g_BattleCameraGlobalScripts + ofs);
    if (mode == -3) {
        scriptBase = (BattleCameraScriptRoot *)g_BattleCameraGlobalScripts;
        ofs = scriptBase->focusScriptOffset - g_BattleCameraFileAddressBase;
        table = (int *)(g_BattleCameraGlobalScripts + ofs);
        ofs = table[slot] - g_BattleCameraFileAddressBase;
        animState->scriptPtr = (char *)(g_BattleCameraGlobalScripts + ofs);
    }
    if (mode == -1) {
        animState->scriptPtr = (char *)g_BattleCameraFocusDefaultScript;
    }
    if (mode == -2) {
        animState->scriptPtr = (char *)g_BattleCameraFocalScripts[*(unsigned char *)&g_BattleCameraScriptIndex];
    }
    if (g_BattleCameraFocusChannels[slot].scriptOffset == 0xFF) {
        g_BattleCameraFocusChannels[slot].scriptOffset = 0;
        g_BattleCameraFocusChannels[slot].waitFrames = 0;
        g_BattleCameraFocusChannels[slot].rotateOffsets = 0;
        g_BattleCameraFocusScriptActive = 0;
    }
    while (animState->running != 0) {
        animState->opcode = animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset];
        (g_BattleCameraFocusChannels[slot].scriptOffset)++;
        switch (*(unsigned char *)&animState->opcode) {
        case BATTLE_CAMERA_FOCUS_LOOP:
            if (g_BattleCameraFocusChannels[slot].waitFrames == 0) {
                animState->opcode = animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset];
                if (animState->opcode == 0xC0) {
                    g_BattleCameraFocusChannels[slot].scriptOffset = 0;
                    g_BattleCameraFocusChannels[slot].waitFrames = 0;
                }
            }
            break;
        case BATTLE_CAMERA_FOCUS_SET_POSITION:
            g_BattleCameraFocusChannels[slot].x = Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleCameraFocusChannels[slot].y = Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleCameraFocusChannels[slot].z = Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            break;
        case BATTLE_CAMERA_FOCUS_SET_WAIT:
            g_BattleCameraFocusChannels[slot].waitFrames = (short)animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            break;
        case BATTLE_CAMERA_FOCUS_WAIT:
            if (g_BattleCameraFocusChannels[slot].waitFrames != 0) {
                (g_BattleCameraFocusChannels[slot].waitFrames)--;
                (g_BattleCameraFocusChannels[slot].scriptOffset)--;
                animState->running = 0;
            }
            break;
        case BATTLE_CAMERA_FOCUS_MOVES_EASED:
            g_BattleCameraFocusChannels[slot].rotateOffsets = 0;
            break;
        case BATTLE_CAMERA_FOCUS_MOVES_LINEAR:
            g_BattleCameraFocusChannels[slot].rotateOffsets = 1;
            break;
        case BATTLE_CAMERA_FOCUS_SNAP_BONE_ACTOR:
            g_BattleVmReg_3 = animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleVmReg_5 = (&g_BattleActorRecords[g_BattleVmReg_3].targetBone)[animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset]];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleVmReg_0 = (int)(short)Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleVmReg_1 = (int)(short)Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleVmReg_2 = (int)(short)Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            Battle_CameraFocusSnapToBone(*(short *)&g_BattleVmReg_3, *(short *)&g_BattleVmReg_5, slot);
            animState->running = 0;
            break;
        case BATTLE_CAMERA_FOCUS_SNAP_BONE_TARGET:
            g_BattleVmReg_5 = (&g_BattleActorRecords[g_BattleCameraFocusActor].targetBone)[animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset]];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleVmReg_0 = (int)(short)Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleVmReg_1 = (int)(short)Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleVmReg_2 = (int)(short)Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            Battle_CameraFocusSnapToBone(g_BattleCameraFocusActor, *(short *)&g_BattleVmReg_5, slot);
            animState->running = 0;
            break;
        case BATTLE_CAMERA_FOCUS_SET_TARGET_3:
            if (animState->scriptPtr[(g_BattleCameraFocusChannels[slot].scriptOffset)++] != 0) {
                g_BattleCameraFocusActor = 3;
            }
            break;
        case BATTLE_CAMERA_FOCUS_TARGET_GROUP:
            g_BattleCameraFocusActor = BATTLE_CAMERA_ACTOR_TARGETS;
            break;
        case BATTLE_CAMERA_FOCUS_SNAP_BONE_ATTACKER:
            g_BattleVmReg_5 = (&g_BattleActorRecords[g_BattleActiveActorId].targetBone)[animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset]];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleVmReg_0 = (int)(short)Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleVmReg_1 = (int)(short)Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleVmReg_2 = (int)(short)Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            Battle_CameraFocusSnapToBone(g_BattleActiveActorId, *(short *)&g_BattleVmReg_5, slot);
            animState->running = 0;
            break;
        case BATTLE_CAMERA_FOCUS_FOLLOW_BONE_ACTOR:
            animState->angle = Battle_AllocateAuxTaskSlot(Battle_CameraFocusFollowBoneTask);
            g_BattleAuxTaskSlotBuffer[animState->angle].angleStep = (short)animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset];
            g_BattleVmReg_3 = g_BattleAuxTaskSlotBuffer[animState->angle].angleStep;
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleAuxTaskSlotBuffer[animState->angle].radius = (short)(&g_BattleActorRecords[g_BattleVmReg_3].targetBone)[animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset]];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleAuxTaskSlotBuffer[animState->angle].radiusStep = Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleAuxTaskSlotBuffer[animState->angle].cameraValue = (int)(short)Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleAuxTaskSlotBuffer[animState->angle].positionStep = Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleAuxTaskSlotBuffer[animState->angle].countdown = (short)animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleAuxTaskSlotBuffer[animState->angle].orbitAngle = (short)slot;
            break;
        case BATTLE_CAMERA_FOCUS_FOLLOW_BONE_TARGET:
            animState->angle = Battle_AllocateAuxTaskSlot(Battle_CameraFocusFollowBoneTask);
            g_BattleAuxTaskSlotBuffer[animState->angle].angleStep = (short)g_BattleCameraFocusActor;
            g_BattleAuxTaskSlotBuffer[animState->angle].radius = (short)(&g_BattleActorRecords[g_BattleCameraFocusActor].targetBone)[animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset]];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleAuxTaskSlotBuffer[animState->angle].radiusStep = Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleAuxTaskSlotBuffer[animState->angle].cameraValue = (int)(short)Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleAuxTaskSlotBuffer[animState->angle].positionStep = Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleAuxTaskSlotBuffer[animState->angle].countdown = (short)animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleAuxTaskSlotBuffer[animState->angle].orbitAngle = (short)slot;
            break;
        case BATTLE_CAMERA_FOCUS_FOLLOW_BONE_ATTACKER:
            animState->angle = Battle_AllocateAuxTaskSlot(Battle_CameraFocusFollowBoneTask);
            g_BattleAuxTaskSlotBuffer[animState->angle].angleStep = (short)g_BattleActiveActorId;
            g_BattleAuxTaskSlotBuffer[animState->angle].radius = (short)(&g_BattleActorRecords[g_BattleActiveActorId].targetBone)[animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset]];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleAuxTaskSlotBuffer[animState->angle].radiusStep = Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleAuxTaskSlotBuffer[animState->angle].cameraValue = (int)(short)Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleAuxTaskSlotBuffer[animState->angle].positionStep = Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            g_BattleAuxTaskSlotBuffer[animState->angle].countdown = (short)animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleAuxTaskSlotBuffer[animState->angle].orbitAngle = (short)slot;
            break;
        case BATTLE_CAMERA_FOCUS_LOAD_LIVE_POS:
            g_BattleCameraFocusChannels[slot].x = g_BattleCameraFocalPoint.focalPointX;
            g_BattleCameraFocusChannels[slot].y = g_BattleCameraFocalPoint.focalPointY;
            g_BattleCameraFocusChannels[slot].z = g_BattleCameraFocusZ;
            break;
        case BATTLE_CAMERA_FOCUS_SET_FORMATION_CAM:
            g_BattleCurrFormationCameraIdx = animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            break;
        case BATTLE_CAMERA_FOCUS_SNAP_FORMATION_CAM:
            g_BattleCameraFocusChannels[slot].x = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].focusX;
            g_BattleCameraFocusChannels[slot].y = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].focusY;
            g_BattleCameraFocusChannels[slot].z = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].focusZ;
            break;
        case BATTLE_CAMERA_FOCUS_MOVE_TO_FORMATION_CAM:
            slotIndex = (short)Battle_AllocateAuxTaskSlot(Battle_CameraFocusMoveTask);
            trackPtr = (BattleCameraMoveTask *)&g_BattleAuxTaskSlotBuffer[slotIndex].ownerSlot;
            trackPtr->targetX = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].focusX;
            trackPtr->targetY = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].focusY;
            trackPtr->targetZ = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].focusZ;
            trackPtr->framesLeft = (short)animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            trackPtr->startX = g_BattleCameraFocusChannels[slot].x;
            trackPtr->startY = g_BattleCameraFocusChannels[slot].y;
            trackPtr->startZ = g_BattleCameraFocusChannels[slot].z;
            trackPtr->channel = (short)slot;
            break;
        case BATTLE_CAMERA_FOCUS_MOVE_TO_XYZ:
            slotIndex = (short)Battle_AllocateAuxTaskSlot(Battle_CameraFocusMoveTask);
            trackPtr = (BattleCameraMoveTask *)&g_BattleAuxTaskSlotBuffer[slotIndex].ownerSlot;
            trackPtr->targetX = Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            trackPtr->targetY = Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            trackPtr->targetZ = Battle_CameraFocusFetchWord(animState->scriptPtr, (char)slot);
            trackPtr->framesLeft = (short)animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            trackPtr->startX = g_BattleCameraFocusChannels[slot].x;
            trackPtr->startY = g_BattleCameraFocusChannels[slot].y;
            trackPtr->startZ = g_BattleCameraFocusChannels[slot].z;
            trackPtr->channel = (short)slot;
            break;
        case BATTLE_CAMERA_FOCUS_MOVE_TO_TARGET_CAMREC:
            g_BattleVmReg_5 = animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleVmReg_3 = animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleVmReg_0 = g_BattleActorStates[g_BattleCameraFocusActor].cameraOffsets[g_BattleVmReg_5].focusX + g_BattleActorRecords[g_BattleCameraFocusActor].posX;
            g_BattleVmReg_2 = g_BattleActorStates[g_BattleCameraFocusActor].cameraOffsets[g_BattleVmReg_5].focusZ + g_BattleActorRecords[g_BattleCameraFocusActor].posZ;
            if (g_BattleVmReg_5 == 0) {
                g_BattleVmReg_1 = g_BattleActorStates[g_BattleCameraFocusActor].cameraOffsets[g_BattleVmReg_5].focusY;
            } else {
                g_BattleVmReg_1 = g_BattleActorStates[g_BattleCameraFocusActor].cameraOffsets[g_BattleVmReg_5].focusY + g_BattleActorRecords[g_BattleCameraFocusActor].posY;
            }
            slotIndex = (short)Battle_AllocateAuxTaskSlot(Battle_CameraFocusMoveTask);
            trackPtr = (BattleCameraMoveTask *)&g_BattleAuxTaskSlotBuffer[slotIndex].ownerSlot;
            trackPtr->targetX = *(short *)&g_BattleVmReg_0;
            trackPtr->targetY = *(short *)&g_BattleVmReg_1;
            trackPtr->targetZ = *(short *)&g_BattleVmReg_2;
            trackPtr->framesLeft = *(short *)&g_BattleVmReg_3;
            trackPtr->startX = g_BattleCameraFocusChannels[slot].x;
            trackPtr->startY = g_BattleCameraFocusChannels[slot].y;
            trackPtr->startZ = g_BattleCameraFocusChannels[slot].z;
            trackPtr->channel = (short)slot;
            break;
        case BATTLE_CAMERA_FOCUS_MOVE_TO_BONE_FROM_TABLE:
            g_BattleCameraFocusScriptActive = 1;
            g_BattleVmReg_7 = g_BattleHitTargetActors[animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset]];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleVmReg_5 = (&g_BattleActorRecords[g_BattleVmReg_7].targetBone)[animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset]];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            Battle_CameraFocusQueueMoveToBone(*(short *)&g_BattleVmReg_7, (char)slot, animState->scriptPtr);
            break;
        case BATTLE_CAMERA_FOCUS_MOVE_TO_BONE_TARGET:
            g_BattleVmReg_5 = (&g_BattleActorRecords[g_BattleCameraFocusActor].targetBone)[animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset]];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            Battle_CameraFocusQueueMoveToBone(g_BattleCameraFocusActor, (char)slot, animState->scriptPtr);
            break;
        case BATTLE_CAMERA_FOCUS_MOVE_TO_BONE_ATTACKER:
            g_BattleVmReg_5 = (&g_BattleActorRecords[g_BattleActiveActorId].targetBone)[animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset]];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            Battle_CameraFocusQueueMoveToBone(g_BattleActiveActorId, (char)slot, animState->scriptPtr);
            break;
        case BATTLE_CAMERA_FOCUS_MOVE_TO_BONE_ACTOR:
            g_BattleVmReg_4 = animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            g_BattleVmReg_5 = (&g_BattleActorRecords[g_BattleVmReg_4].targetBone)[animState->scriptPtr[g_BattleCameraFocusChannels[slot].scriptOffset]];
            (g_BattleCameraFocusChannels[slot].scriptOffset)++;
            Battle_CameraFocusQueueMoveToBone(*(short *)&g_BattleVmReg_4, (char)slot, animState->scriptPtr);
            break;
        case BATTLE_CAMERA_FOCUS_END:
            (g_BattleCameraFocusChannels[slot].scriptOffset)--;
            animState->running = 0;
            break;
        default:
            animState->running = 0;
        }
    }
}

/*
 * Queues smooth camera focus translation toward target actor bone.
 */
// FUNCTION: FF7 0x005c52f8
void __cdecl Battle_CameraFocusQueueMoveToBone(short actorId, char slot, char *scriptPtr)
{
    BattleCameraMoveTask *moveEntry;
    int taskSlot;

    g_BattleVmReg_0 = (short)Battle_CameraFocusFetchWord(scriptPtr, (char)slot);
    g_BattleVmReg_1 = (short)Battle_CameraFocusFetchWord(scriptPtr, (char)slot);
    g_BattleVmReg_2 = (short)Battle_CameraFocusFetchWord(scriptPtr, (char)slot);
    g_BattleVmReg_3 = ((unsigned char *)scriptPtr)[g_BattleCameraFocusChannels[slot].scriptOffset];
    (g_BattleCameraFocusChannels[slot].scriptOffset)++;
    Battle_CameraFocusAddBoneOrigin((short)actorId, *(short *)&g_BattleVmReg_5);
    taskSlot = (short)Battle_AllocateAuxTaskSlot(Battle_CameraFocusMoveTask);
    moveEntry = (BattleCameraMoveTask *)&g_BattleAuxTaskSlotBuffer[taskSlot].ownerSlot;
    moveEntry->targetX = *(short *)&g_BattleVmReg_0;
    if (g_BattleCameraFocusScriptActive != 0) {
        moveEntry->targetY = 0;
    } else {
        moveEntry->targetY = *(short *)&g_BattleVmReg_1;
    }
    moveEntry->targetZ = *(short *)&g_BattleVmReg_2;
    moveEntry->framesLeft = *(short *)&g_BattleVmReg_3;
    moveEntry->startX = g_BattleCameraFocusChannels[slot].x;
    moveEntry->startY = g_BattleCameraFocusChannels[slot].y;
    moveEntry->startZ = g_BattleCameraFocusChannels[slot].z;
    moveEntry->channel = slot;
}

/*
 * Reads the next little-endian word of a channel's eye script and advances its script offset.
 */
// FUNCTION: FF7 0x005c546b
int __cdecl Battle_CameraEyeFetchWord(char *scriptPtr, char slot)
{
    int lo;
    int hi;

    lo = ((unsigned char *)scriptPtr)[g_BattleCameraEyeChannels[slot].scriptOffset];
    (g_BattleCameraEyeChannels[slot].scriptOffset)++;
    hi = ((unsigned char *)scriptPtr)[g_BattleCameraEyeChannels[slot].scriptOffset];
    (g_BattleCameraEyeChannels[slot].scriptOffset)++;
    return (hi << 8) + lo;
}

/*
 * Reads the next little-endian word of a channel's focus script and advances its script offset.
 */
// FUNCTION: FF7 0x005c54f4
int __cdecl Battle_CameraFocusFetchWord(char *scriptPtr, char slot)
{
    int lo;
    int hi;

    lo = ((unsigned char *)scriptPtr)[g_BattleCameraFocusChannels[slot].scriptOffset];
    (g_BattleCameraFocusChannels[slot].scriptOffset)++;
    hi = ((unsigned char *)scriptPtr)[g_BattleCameraFocusChannels[slot].scriptOffset];
    (g_BattleCameraFocusChannels[slot].scriptOffset)++;
    return (hi << 8) + lo;
}

/*
 * Drives camera eye tracking following an actor bone position.
 */
// FUNCTION: FF7 0x005c557d
void __cdecl Battle_CameraEyeFollowBoneTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[8];
        SVECTOR boneX;
    } frame;
#pragma pack(pop)

    if (g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].countdown == 1) {
        g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleVmReg_3 = g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].angleStep;
    g_BattleVmReg_5 = g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].radius;
    g_BattleVmReg_6 = g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].orbitAngle;
    Battle_GetBoneWorldPos(g_BattleVmReg_3, g_BattleVmReg_5, &frame.boneX.vx);
    g_BattleVmReg_0 = g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].radiusStep;
    g_BattleVmReg_1 = *(short *)&g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].cameraValue;
    g_BattleVmReg_2 = g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].positionStep;
    Battle_CameraScaleAndRotateOffset(*(short *)&g_BattleVmReg_3, *(char *)&g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].orbitAngle, (char)g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].channel);
    g_BattleCameraEyeChannels[g_BattleVmReg_6].x = frame.boneX.vx + g_BattleVmReg_0;
    g_BattleCameraEyeChannels[g_BattleVmReg_6].y = frame.boneX.vy + g_BattleVmReg_1;
    g_BattleCameraEyeChannels[g_BattleVmReg_6].z = frame.boneX.vz + g_BattleVmReg_2;
    (g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].countdown)--;
}

/*
 * Drives camera focus tracking following an actor bone position.
 */
// FUNCTION: FF7 0x005c5714
void __cdecl Battle_CameraFocusFollowBoneTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[8];
        SVECTOR boneX;
    } frame;
#pragma pack(pop)

    if (g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].countdown == 1) {
        g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleVmReg_3 = g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].angleStep;
    g_BattleVmReg_5 = g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].radius;
    g_BattleVmReg_6 = g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].orbitAngle;
    Battle_GetBoneWorldPos(g_BattleVmReg_3, g_BattleVmReg_5, &frame.boneX.vx);
    g_BattleVmReg_1 = Battle_CameraApplyFocusHeightBias(g_BattleVmReg_1, *(char *)&g_BattleVmReg_3);
    g_BattleVmReg_0 = g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].radiusStep;
    g_BattleVmReg_1 = *(short *)&g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].cameraValue;
    g_BattleVmReg_2 = g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].positionStep;
    g_BattleCameraFocusChannels[g_BattleVmReg_6].x = frame.boneX.vx + g_BattleVmReg_0;
    g_BattleCameraFocusChannels[g_BattleVmReg_6].y = frame.boneX.vy + g_BattleVmReg_1;
    g_BattleCameraFocusChannels[g_BattleVmReg_6].z = frame.boneX.vz + g_BattleVmReg_2;
    (g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].countdown)--;
}

/*
 * Snaps camera eye coordinates directly to target actor bone position.
 */
// FUNCTION: FF7 0x005c589a
void __cdecl Battle_CameraEyeSnapToBone(short actorId, short boneIndex, short slot, char mode)
{
    short pos[4];

    if (actorId == BATTLE_CAMERA_ACTOR_TARGETS) {
        Battle_GetActorMaskCentroid(g_BattleEffectScratch.targetMask, pos);
    } else {
        Battle_GetBoneWorldPos(actorId, boneIndex, pos);
        Battle_CameraScaleAndRotateOffset(actorId, (char)slot, mode);
    }
    g_BattleCameraEyeChannels[slot].x = pos[0] + g_BattleVmReg_0;
    g_BattleCameraEyeChannels[slot].y = pos[1] + g_BattleVmReg_1;
    g_BattleCameraEyeChannels[slot].z = pos[2] + g_BattleVmReg_2;
}

/*
 * Snaps camera focus coordinates directly to target actor bone position.
 */
// FUNCTION: FF7 0x005c593c
void __cdecl Battle_CameraFocusSnapToBone(short actorId, short boneIndex, short slot)
{
    short pos[4];

    if (actorId == BATTLE_CAMERA_ACTOR_TARGETS) {
        Battle_GetActorMaskCentroid(g_BattleEffectScratch.targetMask, pos);
    } else {
        Battle_GetBoneWorldPos(actorId, boneIndex, pos);
        g_BattleVmReg_1 = Battle_CameraApplyFocusHeightBias(g_BattleVmReg_1, (char)actorId);
    }
    g_BattleCameraFocusChannels[slot].x = pos[0] + g_BattleVmReg_0;
    g_BattleCameraFocusChannels[slot].y = pos[1] + g_BattleVmReg_1;
    g_BattleCameraFocusChannels[slot].z = pos[2] + g_BattleVmReg_2;
}

/*
 * Adds actor bone world translation to camera eye coordinates.
 */
// FUNCTION: FF7 0x005c59e3
void __cdecl Battle_CameraEyeAddBoneOrigin(short actorId, short boneIndex, char slot, char mode)
{
    short pos[4];

    if (actorId == BATTLE_CAMERA_ACTOR_TARGETS) {
        Battle_GetActorMaskCentroid(g_BattleEffectScratch.targetMask, pos);
    } else {
        Battle_GetBoneWorldPos(actorId, boneIndex, pos);
        Battle_CameraScaleAndRotateOffset(actorId, slot, mode);
    }
    g_BattleVmReg_0 = pos[0] + g_BattleVmReg_0;
    g_BattleVmReg_1 = pos[1] + g_BattleVmReg_1;
    g_BattleVmReg_2 = pos[2] + g_BattleVmReg_2;
}

/*
 * Adds actor bone world translation to camera focus coordinates.
 */
// FUNCTION: FF7 0x005c5a73
void __cdecl Battle_CameraFocusAddBoneOrigin(short actorId, short boneIndex)
{
    short pos[4];

    if (actorId == BATTLE_CAMERA_ACTOR_TARGETS) {
        Battle_GetActorMaskCentroid(g_BattleEffectScratch.targetMask, pos);
    } else {
        Battle_GetBoneWorldPos(actorId, boneIndex, pos);
        g_BattleVmReg_1 = Battle_CameraApplyFocusHeightBias(g_BattleVmReg_1, (char)actorId);
    }
    g_BattleVmReg_0 = pos[0] + g_BattleVmReg_0;
    g_BattleVmReg_1 = pos[1] + g_BattleVmReg_1;
    g_BattleVmReg_2 = pos[2] + g_BattleVmReg_2;
}

/*
 * Applies vertical bias offset to camera focus height.
 */
// FUNCTION: FF7 0x005c5b08
int __cdecl Battle_CameraApplyFocusHeightBias(int height, char actorId)
{
    if (g_BattleActorRecords[actorId].scriptId == 1 && g_BattleActorStates[actorId].heightBias > 0) {
        return height;
    } else {
        return height + g_BattleActorStates[actorId].heightBias;
    }
}

/*
 * Computes absolute difference between two camera coordinates.
 */
// FUNCTION: FF7 0x005c5b4d
int __cdecl Battle_CameraAbsDelta(short first, short second)
{
    int squared;

    squared = (first - second) * (first - second);
    return SqrtToInt(squared);
}

/*
 * Copies the sign of one integer onto the magnitude of another.
 */
// FUNCTION: FF7 0x005c5b7b
int __cdecl Battle_CopySignOf(int signSource, int magnitude)
{
    int result;

    if (signSource < 0) {
        result = -magnitude;
    } else {
        result = magnitude;
    }
    return result;
}

/*
 * Dispatches eye move task initialization or per-step interpolation.
 */
// FUNCTION: FF7 0x005c5b9c
void __cdecl Battle_CameraEyeMoveTask(void)
{
    switch (g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].timerWord) {
    case BATTLE_TASK_FRAME_FIRST:
        Battle_CameraEyeMoveTaskInit(g_BattleCameraAuxTaskSlot);
        Battle_CameraEyeMoveTaskStep(g_BattleCameraAuxTaskSlot);
        break;
    case BATTLE_TASK_FRAME_UPDATE:
        Battle_CameraEyeMoveTaskStep(g_BattleCameraAuxTaskSlot);
        break;
    }
}

/*
 * Initializes camera eye move trajectory and interpolation deltas.
 */
// FUNCTION: FF7 0x005c5bf7
void __cdecl Battle_CameraEyeMoveTaskInit(short taskSlot)
{
    BattleCameraMoveTask *task;

    task = (BattleCameraMoveTask *)&g_BattleAuxTaskSlotBuffer[taskSlot].ownerSlot;
    (task->phase)++;
    if (g_BattleCameraEyeChannels[task->channel].eyeLinearMoves == 0) {
        task->elapsed = 0;
        task->totalFrames = task->framesLeft;
    } else {
        task->stepX = (Battle_CopySignOf(task->targetX - task->startX, (short)Battle_CameraAbsDelta((short)task->targetX, (short)task->startX)) + 1 << 4) / task->framesLeft;
        task->stepY = (Battle_CopySignOf(task->targetY - task->startY, (short)Battle_CameraAbsDelta((short)task->targetY, (short)task->startY)) + 1 << 4) / task->framesLeft;
        task->stepZ = (Battle_CopySignOf(task->targetZ - task->startZ, (short)Battle_CameraAbsDelta((short)task->targetZ, (short)task->startZ)) + 1 << 4) / task->framesLeft;
    }
}

/*
 * Steps camera eye interpolation along trajectory curves.
 */
// FUNCTION: FF7 0x005c5d36
void __cdecl Battle_CameraEyeMoveTaskStep(short taskSlot)
{
    BattleCameraMoveTask *eyeMove;

    eyeMove = (BattleCameraMoveTask *)&g_BattleAuxTaskSlotBuffer[taskSlot].ownerSlot;
    if (eyeMove->framesLeft == 1) {
        eyeMove->ownerSlot = BATTLE_TASK_FREE;
        g_BattleCameraEyeChannels[eyeMove->channel].x = eyeMove->targetX;
        g_BattleCameraEyeChannels[eyeMove->channel].y = eyeMove->targetY;
        g_BattleCameraEyeChannels[eyeMove->channel].z = eyeMove->targetZ;
    } else {
        if (g_BattleCameraEyeChannels[eyeMove->channel].eyeLinearMoves == 0) {
            g_BattleCameraEyeChannels[eyeMove->channel].x = Battle_CameraCosineEase(eyeMove->startX, eyeMove->targetX, eyeMove->totalFrames, eyeMove->elapsed);
            g_BattleCameraEyeChannels[eyeMove->channel].y = Battle_CameraCosineEase(eyeMove->startY, eyeMove->targetY, eyeMove->totalFrames, eyeMove->elapsed);
            g_BattleCameraEyeChannels[eyeMove->channel].z = Battle_CameraCosineEase(eyeMove->startZ, eyeMove->targetZ, eyeMove->totalFrames, eyeMove->elapsed);
            (eyeMove->elapsed)++;
        } else {
            g_BattleVmReg_0 = g_BattleCameraEyeChannels[eyeMove->channel].x << 4;
            g_BattleVmReg_1 = g_BattleCameraEyeChannels[eyeMove->channel].y << 4;
            g_BattleVmReg_2 = g_BattleCameraEyeChannels[eyeMove->channel].z << 4;
            g_BattleCameraEyeChannels[eyeMove->channel].x = g_BattleVmReg_0 + eyeMove->stepX >> 4;
            g_BattleCameraEyeChannels[eyeMove->channel].y = g_BattleVmReg_1 + eyeMove->stepY >> 4;
            g_BattleCameraEyeChannels[eyeMove->channel].z = g_BattleVmReg_2 + eyeMove->stepZ >> 4;
        }
        (eyeMove->framesLeft)--;
    }
}

/*
 * Dispatches focus move task initialization or per-step interpolation.
 */
// FUNCTION: FF7 0x005c5f5e
void __cdecl Battle_CameraFocusMoveTask(void)
{
    switch (g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].timerWord) {
    case BATTLE_TASK_FRAME_FIRST:
        Battle_CameraFocusMoveTaskInit(g_BattleCameraAuxTaskSlot);
        Battle_CameraFocusMoveTaskStep(g_BattleCameraAuxTaskSlot);
        break;
    case BATTLE_TASK_FRAME_UPDATE:
        Battle_CameraFocusMoveTaskStep(g_BattleCameraAuxTaskSlot);
        break;
    }
}

/*
 * Initializes camera focus move trajectory and interpolation deltas.
 */
// FUNCTION: FF7 0x005c5fb9
void __cdecl Battle_CameraFocusMoveTaskInit(short taskSlot)
{
    BattleCameraMoveTask *focusTask;

    focusTask = (BattleCameraMoveTask *)&g_BattleAuxTaskSlotBuffer[taskSlot].ownerSlot;
    (focusTask->phase)++;
    if (g_BattleCameraFocusChannels[focusTask->channel].rotateOffsets == 0) {
        focusTask->elapsed = 0;
        focusTask->totalFrames = focusTask->framesLeft;
    } else {
        focusTask->stepX = (Battle_CopySignOf(focusTask->targetX - focusTask->startX, (short)Battle_CameraAbsDelta((short)focusTask->targetX, (short)focusTask->startX)) + 1 << 4) / focusTask->framesLeft;
        focusTask->stepY = (Battle_CopySignOf(focusTask->targetY - focusTask->startY, (short)Battle_CameraAbsDelta((short)focusTask->targetY, (short)focusTask->startY)) + 1 << 4) / focusTask->framesLeft;
        focusTask->stepZ = (Battle_CopySignOf(focusTask->targetZ - focusTask->startZ, (short)Battle_CameraAbsDelta((short)focusTask->targetZ, (short)focusTask->startZ)) + 1 << 4) / focusTask->framesLeft;
    }
}

/*
 * Steps camera focus interpolation along trajectory curves.
 */
// FUNCTION: FF7 0x005c60f8
void __cdecl Battle_CameraFocusMoveTaskStep(short taskSlot)
{
    BattleCameraMoveTask *focusMove;

    focusMove = (BattleCameraMoveTask *)&g_BattleAuxTaskSlotBuffer[taskSlot].ownerSlot;
    if (focusMove->framesLeft == 1) {
        focusMove->ownerSlot = BATTLE_TASK_FREE;
        g_BattleCameraFocusChannels[focusMove->channel].x = focusMove->targetX;
        g_BattleCameraFocusChannels[focusMove->channel].y = focusMove->targetY;
        g_BattleCameraFocusChannels[focusMove->channel].z = focusMove->targetZ;
    } else {
        if (g_BattleCameraFocusChannels[focusMove->channel].rotateOffsets == 0) {
            g_BattleCameraFocusChannels[focusMove->channel].x = Battle_CameraCosineEase(focusMove->startX, focusMove->targetX, focusMove->totalFrames, focusMove->elapsed);
            g_BattleCameraFocusChannels[focusMove->channel].y = Battle_CameraCosineEase(focusMove->startY, focusMove->targetY, focusMove->totalFrames, focusMove->elapsed);
            g_BattleCameraFocusChannels[focusMove->channel].z = Battle_CameraCosineEase(focusMove->startZ, focusMove->targetZ, focusMove->totalFrames, focusMove->elapsed);
        } else {
            g_BattleVmReg_0 = g_BattleCameraFocusChannels[focusMove->channel].x << 4;
            g_BattleVmReg_1 = g_BattleCameraFocusChannels[focusMove->channel].y << 4;
            g_BattleVmReg_2 = g_BattleCameraFocusChannels[focusMove->channel].z << 4;
            g_BattleCameraFocusChannels[focusMove->channel].x = g_BattleVmReg_0 + focusMove->stepX >> 4;
            g_BattleCameraFocusChannels[focusMove->channel].y = g_BattleVmReg_1 + focusMove->stepY >> 4;
            g_BattleCameraFocusChannels[focusMove->channel].z = g_BattleVmReg_2 + focusMove->stepZ >> 4;
        }
        (focusMove->elapsed)++;
        (focusMove->framesLeft)--;
    }
}

/*
 * Scales and rotates local offset vector by camera orientation angles.
 */
// FUNCTION: FF7 0x005c6320
void __cdecl Battle_CameraScaleAndRotateOffset(short actorIndex, char slot, char useAltAngle)
{
    int length;
    int yaw;
    int ratio;
    int product;

    ratio = g_BattleActorStates[actorIndex].ratioX;
    if (g_BattleActorRecords[actorIndex].scale < 0x1000) {
        ratio = g_BattleActorStates[actorIndex].ratioX / 2;
        if (ratio < 256) {
            ratio = 0x100;
        }
    }
    product = g_BattleVmReg_0 * ratio;
    g_BattleVmReg_0 = product >> 8;
    ratio = g_BattleActorStates[actorIndex].ratioY;
    if (g_BattleActorRecords[actorIndex].scale < 0x1000) {
        ratio = g_BattleActorStates[actorIndex].ratioY / 2;
        if (ratio < 256) {
            ratio = 0x100;
        }
    }
    product = g_BattleVmReg_1 * ratio;
    g_BattleVmReg_1 = product >> 8;
    ratio = g_BattleActorStates[actorIndex].ratioZ;
    if (g_BattleActorRecords[actorIndex].scale < 0x1000) {
        ratio = g_BattleActorStates[actorIndex].ratioZ / 2;
        if (ratio < 256) {
            ratio = 0x100;
        }
    }
    product = g_BattleVmReg_2 * ratio;
    g_BattleVmReg_2 = product >> 8;
    if (g_BattleCameraEyeChannels[slot].rotateOffsets == 0) {
        return;
    }
    length = SqrtToInt(g_BattleVmReg_0 * g_BattleVmReg_0 + g_BattleVmReg_2 * g_BattleVmReg_2);
    if (useAltAngle != 0) {
        yaw = ArcTangentFixed(g_BattleVmReg_0, g_BattleVmReg_2) + g_BattleActorStates[actorIndex].yaw;
    } else {
        yaw = ArcTangentFixed(g_BattleVmReg_0, g_BattleVmReg_2) + g_BattleActorRecords[actorIndex].yaw;
    }
    g_BattleVmReg_0 = SinFixed(yaw) * length >> 12;
    g_BattleVmReg_2 = CosFixed(yaw) * length >> 12;
}

/*
 * Smoothly settles auto camera channel toward target position.
 */
// FUNCTION: FF7 0x005c655c
void __cdecl Battle_SettleAutoCameraChannel(void)
{
    int eyeDeltaX;
    int workDeltaFocusY;
    int eyeDeltaY;
    int focusDeltaZ;
    int eyeDeltaZ;
    int focusStepX;

    g_BattleCameraEyeChannels[3].x = g_BattleCameraPosition.cameraPosX;
    g_BattleCameraEyeChannels[3].y = g_BattleCameraPosition.cameraPosY;
    g_BattleCameraEyeChannels[3].z = g_BattleCameraPosZ;
    g_BattleCameraFocusChannels[3].x = g_BattleCameraFocalPoint.focalPointX;
    g_BattleCameraFocusChannels[3].y = g_BattleCameraFocalPoint.focalPointY;
    g_BattleCameraFocusChannels[3].z = g_BattleCameraFocusZ;
    if (g_BattleCameraIsMoving != 0) {
        eyeDeltaX = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].eyeX - g_BattleCameraEyeChannels[3].x;
        eyeDeltaX /= 2;
        g_BattleCameraEyeChannels[3].x += eyeDeltaX;
        eyeDeltaY = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].eyeY - g_BattleCameraEyeChannels[3].y;
        eyeDeltaY /= 2;
        g_BattleCameraEyeChannels[3].y += eyeDeltaY;
        eyeDeltaZ = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].eyeZ - g_BattleCameraEyeChannels[3].z;
        eyeDeltaZ /= 2;
        g_BattleCameraEyeChannels[3].z += eyeDeltaZ;
        focusStepX = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].focusX - g_BattleCameraFocusChannels[3].x;
        focusStepX /= 2;
        g_BattleCameraFocusChannels[3].x += focusStepX;
        workDeltaFocusY = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].focusY - g_BattleCameraFocusChannels[3].y;
        workDeltaFocusY /= 2;
        g_BattleCameraFocusChannels[3].y += workDeltaFocusY;
        focusDeltaZ = g_BattleScene.formationCamera[g_BattleCurrFormationCameraIdx].focusZ - g_BattleCameraFocusChannels[3].z;
        focusDeltaZ /= 2;
        g_BattleCameraFocusChannels[3].z += focusDeltaZ;
        if (eyeDeltaX == 0 && eyeDeltaY == 0 && eyeDeltaZ == 0 && focusStepX == 0 && workDeltaFocusY == 0 && focusDeltaZ == 0) {
            g_BattleCameraIsMoving = 0;
        }
    }
}

/*
 * Eases from the first value to the second over duration frames along a cosine curve and returns
 * the value after elapsed frames.
 */
// FUNCTION: FF7 0x005c6748
int __cdecl Battle_CameraCosineEase(int endValue, int startValue, int duration, int elapsed)
{
    int delta;
    short phase;

    phase = (elapsed << 11) / duration + 0x800;
    delta = startValue - endValue;
    endValue += (CosFixed(phase) + 0x1000) * delta / 8192;
    return endValue;
}
} /* extern "C" */
