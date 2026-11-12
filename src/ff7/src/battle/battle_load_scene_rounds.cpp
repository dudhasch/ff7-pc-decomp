// src/ff7/src/battle/battle_load_scene_rounds.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Sets up the battle scene from a formation for the first round and loads later rounds
//   through a state machine, and seeds the ATB gauges and the speed setting.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"
#include "lib/codegen.h"
#include "ff7/battle.h"
#include "lib/crt.h"
#include "ff7/savemap.h"
#include "ff7/types/menu.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x009aeae4
int g_BattleNextRoundLoadState = 0;
// clang-format on

extern int g_BattleExpShownByPartySlot; /* Scalar head view of g_BattleExpShownByPartySlot[3]. */


/*
 * Sets up a battle from the given formation ID. Seeds scene flags, loads party
 * members from savemap, recalculates encounter modifiers, resets action sequence,
 * program tables, event queues, and combatant state. Initializes enemy combatants,
 * configures battle speed and ATB mode, refreshes actor tick rates, runs AI init events,
 * seeds initial ATB gauges, takes HP/MP snapshots, and applies formation flags.
 */
// FUNCTION: FF7 0x00437db0
void __cdecl Battle_LoadSceneFirstRound(int formationId)
{
    BattleCombatant *combatant;
    int slot;
    char *messageTextPtr;
    int unused;
    int expBit;
    int atbPeriod;

    Battle_UnpackSceneFlagsToBytes(rand());
    unused = 0;
    for (slot = 0; slot < 3; slot++) {
        Savemap_BuildPartyStatsFromRecord(slot);
        Battle_LoadCombatantFromSavemap(slot);
    }
    Party_RecomputeBattleModifiers();
    messageTextPtr = Kernel_GetBattleMessageExpanded(0x7E);
    g_BattleDigitGlyphBase = *messageTextPtr;
    g_BattleSequenceBuffer.textFormatBase = messageTextPtr[1];
    Battle_ResetSequence();
    Battle_BuildAttackProgramTable();
    Battle_BuildDamageProgramTable();
    for (slot = 0; slot < 0x40; slot++) {
        g_BattleState.queuedActions[slot].level = 0xFF;
    }
    for (slot = 0; slot < 10; slot++) {
        *((char *)&g_BattleState.actorResumeMarker + slot * 8) = 0xFF;
    }
    for (slot = 0; slot < 2; slot++) {
        *((char *)&g_BattleState.actionSlotActor + slot * 8) = 0xFF;
    }
    Battle_ResetEventQueues();
    *(int *)&g_BattleMessageWindowTextId = -1;
    Battle_SetMessageWindowRequest(*(short *)&g_BattleMessageWindowTextId);
    for (slot = 0; slot < 10; slot++) {
        g_aCombatants[slot].unitId = 0xFF;
        g_aCombatants[slot].animStep = 0x10;
    }
    Battle_ResetPendingDrops();
    Battle_LoadPartyActorsFromEquipment();
    Kernel_BuildBattleItemList();
    Battle_LoadSceneRecord(formationId, 0);
    g_BattleSceneRecordLoaded = 0;
    Battle_InitEnemyCombatants();
    Battle_SetSpeedFromConfig(*(unsigned char *)&gSavemap.battleSpeed);
    g_BattleState.atbMode = (gSavemap.options & 0xC0) >> 6;
    for (slot = 0; slot < 10; slot++) {
        combatant = &g_aCombatants[slot];
        Battle_RefreshActorTickRates(slot);
        if (combatant->unitId != -1) {
            gBattleAiGlobals.actorMask |= 1 << slot;
        }
    }
    gBattleAiGlobals.formationId = formationId;
    gBattleAiGlobals.layout = g_BattleScene.layout;
    Battle_AssignActorSideMasks();
    Battle_RecomputeActorMasks();
    Battle_RunPartyAiInitEvents();
    Battle_RunEnemyAiInitEvents();
    Battle_RunFormationAiEvents();
    Battle_RecomputeActorMasks();
    Battle_SeedInitialAtbGauges();
    Battle_SnapshotHpMp();
    g_BattleAtbFrozen |= 1;
    for (slot = 0; slot < 3; slot++) {
        Battle_RollReactionTriggers(slot, 1);
    }
    if (gBattleAiGlobals.arenaFlags & BATTLE_ARENA_FIXED_ATB) {
        Battle_SetSpeedFromConfig(0x80);
        g_BattleState.atbMode = BATTLE_ATB_ACTIVE;
        for (slot = 0; slot < 3; slot++) {
            Battle_ResetLimitBreakSlots(slot);
        }
    } else {
        Battle_CheckLucky7s();
    }
    if (gBattleAiGlobals.arenaFlags & BATTLE_ARENA_KEEP_ROUND_HP) {
        if ((g_BattleLoadState & 4) == 0) {
            g_BattleLoadState |= 4;
            g_BattleExpShownByPartySlot = -1;
        }
        g_BattleArenaRoundIndex = gBattleAiGlobals.atbPeriod;
        atbPeriod = g_BattleArenaRoundIndex;
        expBit = 1 << (atbPeriod * 6);
        for (slot = 0; slot < 6; slot++) {
            if (g_BattleExpShownByPartySlot & expBit) {
                g_BattleExpShownByPartySlot &= ~expBit;
            } else if (g_aCombatants[slot + 4].unitId != -1) {
                g_aCombatants[slot + 4].curHp =
                    *(int *)((char *)&g_BattleArenaSavedRoundHp + atbPeriod * 0x18 + slot * 4);
                if (g_aCombatants[slot + 4].curHp == 0) {
                    g_aCombatants[slot + 4].status |= BATTLE_STATUS_DEATH;
                    g_aCombatants[slot + 4].prevStatusFlags |= 1;
                    g_aCombatants[slot + 4].flags &= ~(BATTLE_COMBATANT_TARGETABLE | BATTLE_COMBATANT_AI_CONTROLLED);
                }
            }
            expBit <<= 1;
        }
        Battle_RecomputeActorMasks();
    }
    g_BattleSceneLoadDone = 1;
}

/*
 * Loads the next-round scene through a three-state machine (BattleNextRoundLoadState). PREPARE
 * removes queued actors at least four, or all nonnegative queued actors and posts the ATB events
 * REOPEN_PENDING_MENUS and CLEAR_ENEMY_MENU_MASKS when g_BattleScene.roundCounter is nonzero; clears party links and optionally reloads party actors.
 * Falls through to load the scene with Battle_PlaySequence as callback, waiting for g_BattleSceneRecordLoaded.
 * On readiness initializes enemies, clears/rebuilds the presence mask while refreshing all ten tick
 * rates, records formation and side masks, runs enemy init AI and recomputes masks. Seeds all six
 * enemy gauges from masked Battle_RandBelow(64) plus 0x80 shifted by eight, stops their status
 * effects, resets state and marks loaded. Unknown states also reset/mark loaded.
 */
// FUNCTION: FF7 0x004381d5
void __cdecl Battle_LoadSceneNextRound(int formationId)
{
    int minActor;
    int i;
    BattleCombatant *combatantPointer;

    switch (g_BattleNextRoundLoadState) {
    case BATTLE_NEXT_ROUND_LOAD_PREPARE:
        minActor = 4;
        if (g_BattleScene.roundCounter != 0) {
            minActor = 0;
            Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, 0, BATTLE_ATB_EVENT_REOPEN_PENDING_MENUS, 0);
            Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, 0, BATTLE_ATB_EVENT_CLEAR_ENEMY_MENU_MASKS, 0);
        }
        for (i = 0; i < 0x40; i++) {
            if (g_BattleState.queuedActions[i].actorIndex >= minActor) {
                g_BattleState.queuedActions[i].actorIndex = 0xFF;
            }
        }
        for (i = 0; i < 3; i++) {
            g_BattleState.partyStats[i].manipulatedSlot = 0;
        }
        if (g_BattleScene.roundCounter != 0) {
            Battle_ReloadPartyActorsFromSavemap();
        }
        g_BattleNextRoundLoadState = BATTLE_NEXT_ROUND_LOAD_SCENE;
    case BATTLE_NEXT_ROUND_LOAD_SCENE:
        Battle_LoadSceneRecord(formationId, (void *)Battle_PlaySequence);
        if (g_BattleSceneRecordLoaded != 0) {
            g_BattleSceneRecordLoaded = 0;
            g_BattleNextRoundLoadState = BATTLE_NEXT_ROUND_LOAD_INIT_ENEMIES;
        } else {
            break;
        }
    case BATTLE_NEXT_ROUND_LOAD_INIT_ENEMIES:
        Battle_InitEnemyCombatants();
        gBattleAiGlobals.actorMask = 0;
        for (i = 0; i < 0xA; i++) {
            combatantPointer = &g_aCombatants[i];
            Battle_RefreshActorTickRates(i);
            if (combatantPointer->unitId != -1) {
                gBattleAiGlobals.actorMask |= (short)(1 << i);
            }
        }
        gBattleAiGlobals.formationId = formationId;
        gBattleAiGlobals.layout = g_BattleScene.layout;
        Battle_AssignActorSideMasks();
        Battle_RecomputeActorMasks();
        Battle_RunEnemyAiInitEvents();
        Battle_RecomputeActorMasks();
        for (i = 4; i < 0xA; i++) {
            g_BattleState.actors[i].atbGauge = (Battle_RandBelow(0x40) & 0xFF) + 0x80 << 8;
            Battle_StopActorStatusEffect(i);
        }
        g_BattleNextRoundLoadState = BATTLE_NEXT_ROUND_LOAD_PREPARE;
        g_BattleSceneLoadDone = 1;
        break;
    default:
        g_BattleNextRoundLoadState = BATTLE_NEXT_ROUND_LOAD_PREPARE;
        g_BattleSceneLoadDone = 1;
    }
}

/*
 * Clears ten actor gauges and draws one half-range AI random value for every actor in
 * g_BattleScene.targetMask, retaining the maximum. By g_BattleState.layoutType: a normal battle
 * (or the front-row-locked layout) shifts selected gauges so the maximum becomes 0xE000; a back or
 * pincer attack zeros slots below four and shifts the others toward 0xF000; the other types
 * (pre-emptive, side attack) set slots below four to 0xFFFE and divide enemy values by eight.
 * BATTLE_ARENA_FIXED_ATB overrides this with party slots below three at 0xFFFE and all others zero. Stores selected values as shorts and
 * copies the three party gauges to HUD words.
 */
// FUNCTION: FF7 0x004383f5
void __cdecl Battle_SeedInitialAtbGauges(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[56];
        int roll;
        struct { unsigned char pad_to_4[0x4]; int actorIdx; };
        struct { unsigned char pad_to_8[0x8]; int maxRoll; };
        struct { unsigned char pad_to_C[0xC]; int targetMask; };
        struct { unsigned char pad_to_10[0x10]; int gaugeVals[10]; };
    } frame;
#pragma pack(pop)

    frame.maxRoll = 0;
    frame.targetMask = g_BattleScene.targetMask;
    for (frame.actorIdx = 0; frame.actorIdx < 0xA; frame.actorIdx++) {
        frame.roll = 0;
        g_BattleState.actors[frame.actorIdx].atbGauge = 0;
        if ((frame.targetMask & 1 << frame.actorIdx) != 0) {
            frame.roll = Battle_AiVm_Random16() >> 1;
            if (frame.maxRoll < frame.roll) {
                frame.maxRoll = frame.roll;
            }
        }
        frame.gaugeVals[frame.actorIdx] = frame.roll;
    }
    for (frame.actorIdx = 0; frame.actorIdx < 0xA; frame.actorIdx++) {
        if ((frame.targetMask & 1 << frame.actorIdx) != 0) {
            switch (g_BattleState.layoutType) {
            case BATTLE_LAYOUT_NORMAL:
            case BATTLE_LAYOUT_TYPE_FRONT_ROW_LOCKED:
                frame.gaugeVals[frame.actorIdx] += 0xE000 - frame.maxRoll;
                break;
            case BATTLE_LAYOUT_BACK_ATTACK:
            case BATTLE_LAYOUT_PINCER_ATTACK:
                if (frame.actorIdx < 4) {
                    frame.gaugeVals[frame.actorIdx] = 0;
                } else {
                    frame.gaugeVals[frame.actorIdx] += 0xF000 - frame.maxRoll;
                }
                break;
            default:
                if (frame.actorIdx < 4) {
                    frame.gaugeVals[frame.actorIdx] = 0xFFFE;
                } else {
                    frame.gaugeVals[frame.actorIdx] >>= 3;
                }
            }
            if ((gBattleAiGlobals.arenaFlags & BATTLE_ARENA_FIXED_ATB) != 0) {
                if (frame.actorIdx < 3) {
                    frame.gaugeVals[frame.actorIdx] = 0xFFFE;
                } else {
                    frame.gaugeVals[frame.actorIdx] = 0;
                }
            }
            g_BattleState.actors[frame.actorIdx].atbGauge = *(short *)&frame.gaugeVals[frame.actorIdx];
        }
    }
    for (frame.actorIdx = 0; frame.actorIdx < 3; frame.actorIdx++) {
        g_MenuPartyStats[frame.actorIdx].atbGauge = g_BattleState.actors[frame.actorIdx].atbGauge;
    }
}

/*
 * The Battle Speed setting of the Config menu (gSavemap.battleSpeed, 0..255; a battle with
 * BATTLE_ARENA_FIXED_ATB uses 0x80 in Active mode) as the battle clock's step a frame:
 * 0x10000 / (2 * (120 + setting * 480 / 256)), 273 at 0 down to 54 at 255, so a higher setting is
 * a slower battle. Every ATB gauge and status
 * timer runs on this step (Battle_RefreshActorTickRates, Battle_AccumulateTimeSlices).
 */
// FUNCTION: FF7 0x004385cc
void __cdecl Battle_SetSpeedFromConfig(int speedConfig)
{
    int speedModifier;

    speedModifier = 0x78;
    speedModifier += ((speedConfig << 3) * 0x3C) / 256;
    g_BattleState.atbSpeedValue = 0x10000 / (speedModifier << 1);
}
} /* extern "C" */
