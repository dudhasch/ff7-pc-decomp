// src/ff7/src/battle/battle_effect_summon_actors.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Loads and plays battle effect models and summon animations, and runs the slot-3 actor
//   tasks that drive their transforms, animation scripts, fades and draws, along with the freezing
//   of non-target combatants.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_summon.h"
#include "lib/pfile.h"
#include "lib/codegen.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/savemap.h"
#include "ff7/api/menu.h"
#include "ff7/enums/battle.h"
#include "ff7/enums/main.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)




/*
 * Loads category-5 effect model bank with blending parameters.
 */
// FUNCTION: FF7 0x005c0ca0
int __cdecl Battle_LoadEffectModelCategory5(int blendMode, int loadId)
{
    Battle3dPartyLoadDesc *desc;
    int group;
    PfileLoadOptions optionsBuffer;
    PfileLoadOptions *loadOptions;
    Battle3dPartyLoadDesc descSpace;
    void *descHandle;
    BattleLimitAnimBank *pBank;
    int secondary;

    group = 0;
    pBank = 0;
    if (loadId != 0) {
        desc = &descSpace;
        loadOptions = &optionsBuffer;
        Battle_InitLmdLoadDesc((int *)desc);
        Pfile_InitLoadOptions(loadOptions);
        Pfile_SetLoadOptionsBlendMode(blendMode, loadOptions);
        BattleModel_ConfigureLoadCategory(group, 5, desc, loadOptions);
        loadOptions->flags |= 0x100000;
        loadOptions->flags |= 0x2000;
        loadOptions->flags |= 0x4000;
        loadOptions->flags |= 0x800;
        loadOptions->flags |= 0x2000000;
        if (desc->secondCount != 0 || desc->loadCategory == 0) {
            pBank = (BattleLimitAnimBank *)(BattleModel_LoadEnemyDatLegacy(desc, loadOptions, (char *)loadId));
        }
        if (desc->loadCategory != 0) {
            secondary = 0;
            descHandle = BattleModel_LoadDescriptor(1, &loadOptions->file, (char *)loadId);
            if (descHandle != 0) {
                BattleModel_FreeBank(pBank);
                pBank = (BattleLimitAnimBank *)(BattleModel_LoadBank(1, secondary, (Battle3dPartyLoadDesc *)descHandle, desc, loadOptions, (char *)loadId));
                BattleModel_FreeDescriptor(0, descHandle);
            }
        }
    }
    return (int)pBank;
}

/*
 * Allocates task to initialize and execute summon animation.
 */
// FUNCTION: FF7 0x005c0e39
void __cdecl Battle_RunSummonEffect(void)
{
    Battle_AllocateResourceSlot(Battle_SummonEffectTask);
}

/*
 * Drives summon effect state machine, loading model bank and dispatching summon handler.
 */
// FUNCTION: FF7 0x005c0e4b
void __cdecl Battle_SummonEffectTask(void)
{
    int actorIdx;
    int modelRecord;
    BattleLimitAnimBank *pAnimBank;
    int fileLink;
    int effectId;

    g_BattleStatusOverlayAttached = 0;
    switch (g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter) {
    case BATTLE_TASK_FRAME_FIRST:
        (g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter)++;
        ((BattleSummonWaitSlot *)BATTLE_CURRENT_TASK_SLOT)->waitFrames = 0xE;
        g_BattleScriptWaitFrames = 0xE;
        SfxRetriggerCachedBuffer(0x29);
        for (actorIdx = 0; actorIdx < 3; actorIdx++) {
            g_BattleActorRecords[actorIdx].stateFlags = g_BattleActorRecords[actorIdx].stateFlags | 2;
        }
        if (g_BattleLayout == BATTLE_LAYOUT_PINCER_ATTACK) {
            Battle_SummonFreezeNonTargetActors();
        }
        break;
    case BATTLE_TASK_FRAME_UPDATE:
        if (((BattleSummonWaitSlot *)BATTLE_CURRENT_TASK_SLOT)->waitFrames == 0) {
            g_BattleScriptEffectFlag = 1;
            g_BattleActorRecords[0].drawFlags = *(unsigned char *)&g_BattleActorRecords[0].drawFlags | 2;
            g_BattleActorRecords[1].drawFlags = *(unsigned char *)&g_BattleActorRecords[1].drawFlags | 2;
            g_BattleActorRecords[2].drawFlags = *(unsigned char *)&g_BattleActorRecords[2].drawFlags | 2;
            (g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter)++;
            ((BattleSummonWaitSlot *)BATTLE_CURRENT_TASK_SLOT)->waitFrames = 0x2D;
            if (g_BattleSkipFreeEffectGroupTag0 != 0) { /* empty first arm: lib/codegen.h */
            } else {
                Battle_EffectFreeGroupTag0();
            }
            effectId = g_BattleActorRecords[g_BattleActiveActorId].effectId;
            modelRecord = (int)g_BattleSummonModelFileTable[effectId].path;
            fileLink = g_BattleSummonModelFileTable[effectId].blendMode;
            pAnimBank = (BattleLimitAnimBank *)Battle_LoadEffectModelCategory5(fileLink, modelRecord);
            Battle3d_SetEffectModelBank(pAnimBank, (int)g_pBattle3dScene);
            switch (g_BattleActorRecords[g_BattleActiveActorId].effectId) {
            case BATTLE_SUMMON_BAHAMUT:
                g_BattleEffectEnterResult = BattleFx_SummonEnterBahamut(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_CHOCOBO_MOG:
                g_BattleEffectEnterResult = BattleFx_SummonEnterChocoMog(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_FAT_CHOCOBO:
                g_BattleEffectEnterResult = BattleFx_SummonEnterFatChocobo(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_SHIVA:
                g_BattleEffectEnterResult = BattleFx_SummonEnterShiva(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_HADES:
                g_BattleEffectEnterResult = BattleFx_SummonEnterHades(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_PHOENIX:
                g_BattleEffectEnterResult = BattleFx_SummonEnterPhoenix(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_ALEXANDER:
                g_BattleEffectEnterResult = BattleFx_SummonEnterAlexander(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_KJATA:
                g_BattleEffectEnterResult = BattleFx_SummonEnterKjata(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_RAMUH:
                g_BattleEffectEnterResult = BattleFx_SummonEnterRamuh(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_LEVIATHAN:
                g_BattleEffectEnterResult = BattleFx_SummonEnterLeviathan(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_TITAN:
                g_BattleEffectEnterResult = BattleFx_SummonEnterTitan(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_NEO_BAHAMUT:
                g_BattleEffectEnterResult = BattleFx_SummonEnterNeoBahamut(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_BAHAMUT_ZERO:
                g_BattleEffectEnterResult = BattleFx_SummonEnterBahamutZero(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_ODIN_ZANTETSUKEN:
                g_BattleEffectEnterResult = BattleFx_SummonEnterOdinZantetsuken(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_ODIN_GUNGNIR:
                g_BattleEffectEnterResult = BattleFx_SummonEnterOdinGungnir(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_IFRIT:
                g_BattleEffectEnterResult = BattleFx_SummonEnterIfrit(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_TYPHON:
                g_BattleEffectEnterResult = BattleFx_SummonEnterTyphon(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_SUMMON_KNIGHTS_OF_ROUND:
                BattleFx_SummonEnterKnightsOfRound(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
                return;
            }
            Battle_ResetEffectModelActor();
            g_BattleActorRecords[3].drawFlags = *(unsigned char *)&g_BattleActorRecords[3].drawFlags | 0x10;
            g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
        } else {
            ((BattleSummonWaitSlot *)BATTLE_CURRENT_TASK_SLOT)->waitFrames--;
        }
    }
}

/*
 * Freezes non-target combatants during summon animation.
 */
// FUNCTION: FF7 0x005c133f
void __cdecl Battle_SummonFreezeNonTargetActors(void)
{
    int actorIdx;
    int otherIdx;

    for (actorIdx = 4; actorIdx < 0xA; actorIdx++) {
        *(char *)((char *)&g_BattleSummonSavedDrawFlags + actorIdx) = g_BattleActorRecords[actorIdx].drawFlags;
        g_BattleActorRecords[actorIdx].stateFlags = g_BattleActorRecords[actorIdx].stateFlags | 2;
    }
    for (actorIdx = 4; actorIdx < 0xA; actorIdx++) {
        if ((*(unsigned char *)((char *)&g_BattleSummonSavedDrawFlags + actorIdx) & 4) != 0) {
            g_BattleActorRecords[actorIdx].stateFlags = g_BattleActorRecords[actorIdx].stateFlags & 0xFFFFFFFD;
        }
        if ((g_BattleEffectScratch.targetMask & 1 << actorIdx) != 0) {
            for (otherIdx = 4; otherIdx < 0xA; otherIdx++) {
                if (g_BattleActorSpawnPos[actorIdx].posX == g_BattleActorSpawnPos[otherIdx].posX && g_BattleActorSpawnPos[actorIdx].posZ == g_BattleActorSpawnPos[otherIdx].posZ) {
                    g_BattleActorRecords[otherIdx].stateFlags = g_BattleActorRecords[otherIdx].stateFlags & 0xFFFFFFFD;
                }
            }
        }
    }
}

/*
 * Runs the enter function of the running effect (by its BattleEffectCategory and id) and, for
 * the effects that show one, loads and attaches its status overlay model.
 */
// FUNCTION: FF7 0x005c1484
void __cdecl Battle_EffectAttachStatusOverlayModel(int effectParam, int unusedArg)
{
    struct {
        int category;
        int bank;
        int filename;
        int selected;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.selected = -1;
    if (g_BattleSkipFreeEffectGroupTag0 != 0) { /* empty first arm: lib/codegen.h */
    } else {
        Battle_EffectFreeGroupTag0();
    }
    g_BattleStatusOverlayAttached = 1;
    switch (g_BattleActorRecords[g_BattleActiveActorId].effectCategory) {
    case BATTLE_EFFECT_CATEGORY_LIMIT:
        switch (g_BattleActorRecords[g_BattleActiveActorId].effectId) {
        case BATTLE_LIMIT_EFFECT_DEATH:
            g_BattleEffectEnterResult = BattleFx_MagicEnterDeath(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_DEATH;
            break;
        case BATTLE_LIMIT_EFFECT_TOY_SOLDIER:
            g_BattleEffectEnterResult = BattleFx_LimitEnterToySoldier(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_TOY_SOLDIER;
            break;
        case BATTLE_LIMIT_EFFECT_MOG_DANCE:
            g_BattleEffectEnterResult = BattleFx_LimitEnterMogDance(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_MOG_DANCE;
            break;
        }
        break;
    case BATTLE_EFFECT_CATEGORY_ENEMY_SKILL:
        switch (g_BattleActorRecords[g_BattleActiveActorId].effectId) {
        case BATTLE_BLUE_EFFECT_CHOCOBUCKLE:
            g_BattleEffectEnterResult = BattleFx_BlueEnterChocobuckleAlt(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_CONF;
            break;
        case BATTLE_BLUE_EFFECT_DEATH_SENTENCE:
            g_BattleEffectEnterResult = BattleFx_BlueEnterRoulette(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_L5_DEATH;
            break;
        case BATTLE_BLUE_EFFECT_ROULETTE:
            g_BattleEffectEnterResult = BattleFx_BlueEnterRouletteAlt(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_L5_DEATH;
            break;
        case BATTLE_BLUE_EFFECT_ANGEL_WHISPER:
            g_BattleEffectEnterResult = BattleFx_BlueEnterAngelWhisper(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_ANGEL;
            break;
        }
        break;
    case BATTLE_EFFECT_CATEGORY_SPECIAL:
        switch (g_BattleActorRecords[g_BattleActiveActorId].effectId) {
        case BATTLE_SPECIAL_EFFECT_JOKER_DEATH:
            g_BattleEffectEnterResult = BattleFx_SpecialEnterJokerCard(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_L5_DEATH;
            break;
        case BATTLE_SPECIAL_EFFECT_ANGEL_REVIVE:
            g_BattleEffectEnterResult = BattleFx_BlueEnterAngelWhisperD(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_ANGEL;
            break;
        }
        break;
    case BATTLE_EFFECT_CATEGORY_MAGIC:
        switch (g_BattleActorRecords[g_BattleActiveActorId].effectId) {
        case BATTLE_MAGIC_EFFECT_REVIVE_1:
            g_BattleEffectEnterResult = BattleFx_BlueEnterAngelWhisperB(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_ANGEL;
            break;
        case BATTLE_MAGIC_EFFECT_REVIVE_2:
            g_BattleEffectEnterResult = BattleFx_BlueEnterAngelWhisperC(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_ANGEL;
            break;
        case BATTLE_MAGIC_EFFECT_DEATH:
            g_BattleEffectEnterResult = BattleFx_MagicEnterDeath(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_DEATH;
            break;
        case BATTLE_MAGIC_EFFECT_CONFUSE:
            g_BattleEffectEnterResult = BattleFx_BlueEnterChocobuckle(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_CONF;
            break;
        }
        break;
    case BATTLE_EFFECT_CATEGORY_GENERAL:
        switch (g_BattleActorRecords[g_BattleActiveActorId].effectId) {
        case BATTLE_GENERAL_EFFECT_DEATH:
            g_BattleEffectEnterResult = BattleFx_MagicEnterDeath(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_DEATH;
            break;
        case BATTLE_GENERAL_EFFECT_CONFUSE:
            g_BattleEffectEnterResult = BattleFx_BlueEnterChocobuckle(effectParam, g_BattleActiveActorId);
            frame.selected = BATTLE_OVERLAY_MODEL_CONF;
            break;
        }
        break;
    }
    if (frame.selected >= 0 && frame.selected < BATTLE_OVERLAY_MODEL_COUNT) {
        frame.filename = (int)g_BattleLimitModelFileTable[frame.selected];
        frame.category = 6;
        frame.bank = Battle_LoadEffectModelCategory5(frame.category, frame.filename);
        Battle3d_SetEffectModelBank((BattleLimitAnimBank *)frame.bank, (int)g_pBattle3dScene);
    }
    g_BattleActorRecords[3].isDirty = 0;
    Battle_ResetEffectModelActor();
    g_BattleActorRecords[3].drawFlags = *(unsigned char *)&g_BattleActorRecords[3].drawFlags | 0x10;
}

/*
 * Waits for active effect completion and unfreezes party combatants.
 */
// FUNCTION: FF7 0x005c18bc
void __cdecl Battle3d_Slot3ActorWaitEffectTask(void)
{
    int task;

    switch (g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter) {
    case BATTLE_TASK_FRAME_FIRST:
        if (g_BattleSecondaryTaskActiveCount == 0) {
            for (task = 0; task < 3; task++) {
                g_BattleActorRecords[task].stateFlags = g_BattleActorRecords[task].stateFlags | 4;
                if (gSavemap.party[task] != PARTY_CHAR_NONE) {
                    g_BattleActorRecords[task].drawFlags = *(unsigned char *)&g_BattleActorRecords[task].drawFlags & 0xFFFFFFFD;
                }
            }
            if (g_BattleLayout == BATTLE_LAYOUT_PINCER_ATTACK) {
                Battle_ResolveEnemyActorFlagsBySelectedMask();
            }
            g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter = 1;
            ((BattleSummonWaitSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx])->waitFrames = 0xE;
            g_BattleScriptWaitFrames = 0xE;
        }
        break;
    default:
        if (((BattleSummonWaitSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx])->waitFrames == 0) {
            g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = BATTLE_TASK_FREE;
            g_BattleScriptEffectFlag = 0;
            (g_BattleActorRecords[3].isDirty) = 1;
        } else {
            ((BattleSummonWaitSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx])->waitFrames--;
        }
    }
}

/*
 * Updates freeze and visibility flags for unselected enemies.
 */
// FUNCTION: FF7 0x005c1a05
void __cdecl Battle_ResolveEnemyActorFlagsBySelectedMask(void)
{
    int actorIdx;
    int otherIdx;

    for (actorIdx = 4; actorIdx < 0xA; actorIdx++) {
        if ((g_BattleEffectScratch.targetMask & 1 << actorIdx) == 0 && (*(unsigned char *)((char *)&g_BattleSummonSavedDrawFlags + actorIdx) & 4) == 0) {
            g_BattleActorRecords[actorIdx].stateFlags = g_BattleActorRecords[actorIdx].stateFlags | 4;
            g_BattleActorRecords[actorIdx].drawFlags = *(unsigned char *)&g_BattleActorRecords[actorIdx].drawFlags & 0xFFFFFFFB;
        }
    }
    for (actorIdx = 4; actorIdx < 0xA; actorIdx++) {
        if ((g_BattleEffectScratch.targetMask & 1 << actorIdx) != 0) {
            for (otherIdx = 4; otherIdx < 0xA; otherIdx++) {
                if (actorIdx != otherIdx && g_BattleActorSpawnPos[actorIdx].posX == g_BattleActorSpawnPos[otherIdx].posX && g_BattleActorSpawnPos[actorIdx].posZ == g_BattleActorSpawnPos[otherIdx].posZ) {
                    g_BattleActorRecords[otherIdx].stateFlags = g_BattleActorRecords[otherIdx].stateFlags & -5;
                    g_BattleActorRecords[otherIdx].drawFlags = *(unsigned char *)&g_BattleActorRecords[otherIdx].drawFlags & -5;
                }
            }
        }
    }
}

/*
 * Drives transform updates, animation script, and queued draw for slot-3 actor.
 */
// FUNCTION: FF7 0x005c1b81
void __cdecl Battle3d_Slot3ActorTask(void)
{
    if (*(unsigned char *)&g_BattleSlot3ActorState == 0xFF) {
        g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
        if (g_BattleStatusOverlayAttached == 0) {
            Battle_AllocateSecondaryResourceSlot(Battle3d_Slot3ActorWaitEffectTask);
        } else {
            Battle_AllocateSecondaryResourceSlot(Battle3d_Slot3ActorTimedTask);
        }
    }
    if (g_BattleSlot3ActorState == 0) {
        g_BattleActorRecords[3].posX = g_BattleEffectTrail_ShortVector[0];
        g_BattleActorRecords[3].posY = g_BattleEffectTrail_ShortVector[1];
        g_BattleActorRecords[3].posZ = g_BattleEffectTrail_ShortVector[2];
        g_BattleActorRecords[3].yaw = g_BattleEffectTrail_StateShort;
        g_BattleSlot3ActorState = 1;
        g_BattleCameraIsMoving = 1;
    }
    Battle_ProcessActorFadeRequests(3);
    Battle_RunExtraActorScript();
    if (g_BattleStatusOverlayAttached != 0 && g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter == 0) {
        g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter = 1;
        return;
    }
    if (REG_ALLOC_HINT) {
    }
    if (*(unsigned char *)&g_BattleSlot3ActorState == 0xFF) {
        Battle3d_QueueActorDraw(0, 0, 3);
    } else {
        Battle3d_QueueActorDraw(1, 0, 3);
    }
}

/*
 * Timed animation task controlling slot-3 actor fade and queued draw.
 */
// FUNCTION: FF7 0x005c1c8f
void __cdecl Battle3d_Slot3ActorTimedTask(void)
{
    switch (g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter) {
    case BATTLE_TASK_FRAME_FIRST:
        g_BattleActorRecords[3].stateFlags = g_BattleActorRecords[3].stateFlags | 2;
        g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter = 1;
        ((BattleSummonWaitSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx])->waitFrames = 0xE;
        g_BattleScriptWaitFrames = 0xE;
        break;
    default:
        if (((BattleSummonWaitSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx])->waitFrames == 0) {
            g_BattleActorRecords[3].drawFlags = *(unsigned char *)&g_BattleActorRecords[3].drawFlags | 2;
            g_BattleActorRecords[3].drawFlags = *(unsigned char *)&g_BattleActorRecords[3].drawFlags & ~0x10;
            g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = BATTLE_TASK_FREE;
            g_BattleActorRecords[3].isDirty = 1;
            Battle3d_QueueActorDraw(0, 0, 3);
            return;
        }
        ((BattleSummonWaitSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx])->waitFrames -= 1;
        break;
    }
    Battle_ProcessActorFadeRequests(3);
    Battle_RunExtraActorScript();
    Battle3d_QueueActorDraw(1, 0, 3);
}

/*
 * Advances extra actor animation script attached to battle 3D scene.
 */
// FUNCTION: FF7 0x005c1d9a
void __cdecl Battle_RunExtraActorScript(void)
{
    int scriptHead;
    int *scriptState;

    if ((*(unsigned char *)&g_BattleActorRecords[3].drawFlags & 2) == 0 && g_pBattle3dScene != 0) {
        scriptState = (int *)g_pBattle3dScene->scriptState;
        if (scriptState != 0) {
            scriptHead = *scriptState;
            g_BattleActorRecords[3].model = (BattleActorModel *)scriptState;
            Battle_RunActorAnimScript(3, scriptHead, 0, 0);
        }
    }
}

/*
 * Resets effect model node chain and state fields for slot-3 actor.
 */
// FUNCTION: FF7 0x005c1df0
void __cdecl Battle_ResetEffectModelActor(void)
{
    int *header;
    int enterResult;
    int i;
    int *scriptState;

    enterResult = g_BattleEffectEnterResult;
    if (g_pBattle3dScene != 0) {
        scriptState = (int *)g_pBattle3dScene->scriptState;
        if (scriptState != 0) {
            header = (int *)*scriptState;
            for (i = 0; i < 8; i++) {
                header[i] += *scriptState;
            }
        }
        Battle_InitActorNodeChain(3, 0, 0);
    }
    g_BattleActorRecords[3].scaleX = 0;
    g_BattleActorRecords[3].scaleY = 0;
    g_BattleActorRecords[3].modelFlags = 0;
    g_BattleActorRecords[3].scale = 0x1000;
    g_BattleActorStates[3].motionState = 0;
    g_BattleActorStates[3].heightBias = 0;
    g_BattleActorStates[3].ratioX = 0x1C0;
    g_BattleActorStates[3].ratioY = 0x200;
    g_BattleActorStates[3].ratioZ = 0x1C0;
    g_BattleActorStates[3].ratioExtra = 0;
    for (i = 0; i < 6; i++) {
        ((short *)&g_BattleActorStates[3].cameraOffsets[0])[i] = ((short *)&g_BattleStatusOverlayOffsets.views[0])[i];
        ((short *)&g_BattleActorStates[3].cameraOffsets[1])[i] = ((short *)&g_BattleStatusOverlayOffsets.views[1])[i];
    }
    g_BattleActorRecords[3].rootYaw = 0;
    g_BattleActorRecords[3].tintPhase = g_BattleActorRecords[3].rootYaw;
    g_BattleActorStates[3].mirrorWord = g_BattleActorRecords[3].tintPhase;
    g_BattleActorRecords[3].posZ = 0;
    g_BattleActorRecords[3].posY = g_BattleActorRecords[3].posZ;
    g_BattleActorRecords[3].posX = g_BattleActorRecords[3].posY;
    g_BattleActorRecords[3].rotZ = 0;
    g_BattleActorRecords[3].rotX = g_BattleActorRecords[3].rotZ;
    g_BattleActorRecords[3].yaw = 0;
    /* The original clears these 16 bytes of slot 3 starting at scaleY's high byte. */
    for (i = 0; i < 0x10; i++) {
        *((char *)&g_BattleActorRecords + 0x50ef + i) = 0;
    }
    g_BattleActorRecords[3].drawFlags = 0x80;
    g_BattleActorRecords[3].stateFlags = 0;
    g_BattleActorRecords[3].stateFlags = g_BattleActorRecords[3].stateFlags | 1;
    g_BattleActorIdleScripts[3] = 0;
    g_BattleActorRecords[3].scriptId = (char)g_BattleActorIdleScripts[3];
}

/*
 * Pre-caches and frees all summon effect model files.
 */
// FUNCTION: FF7 0x005c1fbc
void __cdecl Battle_LoadAndFreeSummonEffectModels(void)
{
    int fileId;
    BattleLimitAnimBank *pBank;
    int linkWord;
    int summonIdx;

    for (summonIdx = 0; summonIdx < BATTLE_SUMMON_COUNT; summonIdx++) {
        fileId = (int)g_BattleSummonModelFileTable[summonIdx].path;
        linkWord = g_BattleSummonModelFileTable[summonIdx].blendMode;
        pBank = (BattleLimitAnimBank *)Battle_LoadEffectModelCategory5(linkWord, fileId);
        BattleModel_FreeBank(pBank);
    }
}

/*
 * Pre-caches and frees limit and enemy attack effect models.
 */
// FUNCTION: FF7 0x005c2019
void __cdecl Battle_LoadAndFreeLimitEffectModels(void)
{
    int fileId;
    BattleLimitAnimBank *pBank;
    int category;
    int limit;

    for (limit = 0; limit < BATTLE_OVERLAY_MODEL_COUNT; limit++) {
        fileId = (int)g_BattleLimitModelFileTable[limit];
        category = 6;
        pBank = (BattleLimitAnimBank *)Battle_LoadEffectModelCategory5(category, fileId);
        BattleModel_FreeBank(pBank);
    }
}
// clang-format off
// GLOBAL: FF7 0x008feba0
BattleSummonModelFile g_BattleSummonModelFileTable[18] = {{"LIMIT/MOGURIDA.DAT", 4}, {"LIMIT/CYVADAT.DAT", 6}, {"LIMIT/IFREETDA.DAT", 6}, {"LIMIT/LAMDAT.DAT", 6}, {"LIMIT/TITANDAT.DAT", 6}, {"LIMIT/ODIN_ZDA.DAT", 6}, {"LIMIT/RIVADAT.DAT", 6}, {"LIMIT/BAHAMDAT.DAT", 6}, {"LIMIT/KUJATADA.DAT", 6}, {"LIMIT/ALEXDAT.DAT", 6}, {"LIMIT/PHEONIXD.DAT", 6}, {"LIMIT/BAHAMRDA.DAT", 6}, {"LIMIT/HADESDAT.DAT", 6}, {"LIMIT/TUPONDAT.DAT", 6}, {"LIMIT/BAHAMZDA.DAT", 6}, {0, 6}, {"LIMIT/DEBCYODA.DAT", 6}, {"LIMIT/ODIN_GDA.DAT", 6}};
// Model files of the six status overlay models, indexed by BattleOverlayModelFile.
// GLOBAL: FF7 0x008fec30
const char *g_BattleLimitModelFileTable[6] = {
    // STRING: FF7 0x008fedb4
    "LIMIT/DEATHDAT.DAT",
    // STRING: FF7 0x008fedc8
    "LIMIT/TOYDAT.DAT",
    // STRING: FF7 0x008feddc
    "LIMIT/MOGDAT.DAT",
    // STRING: FF7 0x008fedf0
    "LIMIT/5DETHDAT.DAT",
    // STRING: FF7 0x008fee04
    "LIMIT/ANGELDAT.DAT",
    // STRING: FF7 0x008fee18
    "LIMIT/CONFDAT.DAT",
};
// GLOBAL: FF7 0x008fec48
struct BattleStatusOverlayOffsets g_BattleStatusOverlayOffsets = {{{0, -1000, -3500, 0, 0, 0}, {0, -1000, 5000, 0, 0, 0}}};
// clang-format on
} /* extern "C" */
