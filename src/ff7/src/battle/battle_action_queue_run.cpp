// src/ff7/src/battle/battle_action_queue_run.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Runs the battle action queue and the retained action state machine, closes an actor's
//   turn, builds the attack program index table, and checks the Lucky 7s party condition.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/enums/battle.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/battle.h"
#include "lib/codegen.h"
#include "lib/crt.h"
#include "ff7/api/menu.h"
#include "lib/api/token.h"
#include "ff7/types/menu.h"
extern "C" {

/* The queue of 64 pending actions (g_BattleState.queuedActions) and the two remembered action slots
 * inside BattleState. */
#define BATTLE_ACTION_QUEUE_SIZE 0x40
#define BATTLE_ACTION_SLOTS ((BattleActionSlot *)&g_BattleState.actionSlotType)
/* The queue runner's two runs of state words, which start at g_BattleActionDoneFlag and g_BattleActionQueuePhase (see
 * their definitions below). */
#define BATTLE_QUEUE ((BattleActionQueue *)&g_BattleActionDoneFlag)
#define BATTLE_QUEUE_STATE ((BattleActionQueueState *)&g_BattleActionQueuePhase)

// clang-format off
// GLOBAL: FF7 0x007b7510
extern const BattleCommandProperty g_BattleCommandProperties[38] = {
    {0x00, 0x00, 0x0000}, {0x14, 0x11, 0x0000}, {0x1d, 0x00, 0x0000}, {0x1f, 0x00, 0x0000},
    {0x21, 0x37, 0x0000}, {0x22, 0x00, 0x0006}, {0x26, 0x00, 0x0412}, {0x21, 0x0a, 0x0000},
    {0x21, 0x09, 0x0000}, {0x28, 0xb1, 0x2000}, {0x2a, 0x11, 0x0000}, {0x2c, 0x90, 0x0000},
    {0x00, 0x00, 0x0000}, {0x1e, 0x00, 0x0000}, {0x00, 0x00, 0x0000}, {0x00, 0x00, 0x0000},
    {0x00, 0x00, 0x0000}, {0x24, 0x11, 0x0004}, {0x00, 0x11, 0x0002}, {0x00, 0x00, 0x0000},
    {0x00, 0x00, 0x0000}, {0x1d, 0x00, 0x0000}, {0x1f, 0x00, 0x0000}, {0x21, 0x00, 0x0000},
    {0x15, 0x11, 0x0000}, {0x16, 0x11, 0x1000}, {0x1c, 0x11, 0x0002}, {0x18, 0x11, 0x1000},
    {0x00, 0x00, 0x0000}, {0x00, 0x00, 0x0000}, {0x00, 0x00, 0x0000}, {0x00, 0x00, 0x0000},
    {0x00, 0x00, 0x0000}, {0x00, 0x00, 0x0000}, {0x00, 0x00, 0x0000}, {0x2e, 0x04, 0x0000},
    {0x00, 0x00, 0x0000}, {0x00, 0x00, 0x0000}
};
// GLOBAL: FF7 0x007b75a8
extern const unsigned char g_BattleAttackPrograms[109] = {0x1f, 0x0e, 0x09, 0x1f, 0x00, 0x0c, 0x09, 0x1f, 0x01, 0x0c, 0x09, 0x1f, 0x02, 0x0d, 0x09, 0x1f, 0x1e, 0x09, 0x1f, 0x0a, 0x16, 0x09, 0x1f, 0x1d, 0x09, 0x1f, 0x19, 0x09, 0x1f, 0x0e, 0x1c, 0x09, 0x1f, 0x0e, 0x1b, 0x09, 0x1f, 0x1a, 0x09, 0x1f, 0x17, 0x1f, 0x03, 0x0c, 0x09, 0x1f, 0x1f, 0x1f, 0x1f, 0x0e, 0x09, 0x1f, 0x04, 0x0b, 0x0f, 0x1f, 0x05, 0x1f, 0x06, 0x0c, 0x09, 0x1f, 0x00, 0x0c, 0x09, 0x1f, 0x01, 0x0c, 0x09, 0x1f, 0x02, 0x0d, 0x09, 0x1f, 0x0e, 0x09, 0x1f, 0x12, 0x0e, 0x09, 0x1f, 0x0e, 0x18, 0x09, 0x1f, 0x10, 0x0e, 0x09, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x07, 0x0c, 0x09, 0x1f, 0x08, 0x1f, 0x11, 0x1f, 0x13, 0x09, 0x1f, 0x14, 0x1f, 0x15, 0x0f, 0x1f};
// GLOBAL: FF7 0x007b7618
extern const BattleCommandRemap g_BattleCommandRemapTable[3] = {{0x15, 2}, {0x16, 3}, {0x17, 4}};
// GLOBAL: FF7 0x007c2acc
int g_BattleTrackedTurnActor = (-1);
// GLOBAL: FF7 0x009ae9c8
int g_BattleAttackProgramOffsets[38] = {0};
// GLOBAL: FF7 0x009aea60
int g_BattleActionYield = 0;
// GLOBAL: FF7 0x009aea64
int g_BattleActionActor = 0;
// GLOBAL: FF7 0x009aea68
int g_BattleSweepComplete = 0;
// The queue runner's state is two runs of int fields, each a head and the globals after it, read
// through BattleActionQueue (g_BattleActionDoneFlag: the action-done flag Battle_ExecuteAction sets, then
// g_BattleActionQueueWorkState as "active" and g_BattleActionQueuePriority as "priority") and
// BattleActionQueueState words (g_BattleActionQueuePhase followed by state, busy, phase and
// subPhase). Defined as individual 4-byte globals because VC5 8-byte-aligns structs/arrays >= 8 bytes,
// whereas g_BattleActionQueuePhase starts at 0x009AEA6C (4 mod 8).
int g_BattleActionDoneFlag = 0;
// GLOBAL: FF7 0x009aea70
int g_BattleActionQueueWorkState = 0;
// GLOBAL: FF7 0x009aea74
int g_BattleActionQueuePriority = 0;
// GLOBAL: FF7 0x009aea78
int g_BattleAttackOpContinue = 0;
// GLOBAL: FF7 0x009aea7c
int g_BattleActionQueueIndex = 0;
// GLOBAL: FF7 0x009aea80
int g_BattleActionQueueActor = 0;
int g_BattleActionQueuePhase = 0;
// GLOBAL: FF7 0x009aea88
int g_BattleActionQueueState = 0;
// GLOBAL: FF7 0x009aea8c
int g_BattleActionQueueYield = 0;
// GLOBAL: FF7 0x009aea90
int g_BattleActionQueueSubState = 0;
// GLOBAL: FF7 0x009aea94
int g_BattleActionQueueSubState2 = 0;
// clang-format on

/* Clears the tracked turn actor to -1 if it is the given actor. */
// FUNCTION: FF7 0x00435770
void __cdecl Battle_ClearTrackedTurnActorIf(int actor)
{
    if (g_BattleTrackedTurnActor == actor) {
        g_BattleTrackedTurnActor = -1;
    }
}

/*
 * Advances nested action queue states up to maxPriority, dispatching actions,
 * executing AI and actor turns, and closing turns as appropriate.
 * Returns 1 if the current priority reached maxPriority, 0 otherwise.
 */
// FUNCTION: FF7 0x00435789
int __cdecl Battle_RunActionQueue(int maxPriority)
{
    int forceAttack;
    BattleActorQueueRecord *pEnemyRecord;
    int fallbackId;
    int partySlot;

    while (g_BattleSweepComplete == 0 && BATTLE_QUEUE_STATE->busy == 0) {
        switch (BATTLE_QUEUE_STATE->state) {
        case BATTLE_QUEUE_STEP_RESET:
            BATTLE_QUEUE->priority = 0;
            g_BattleActionQueueIndex = 0;
            BATTLE_QUEUE->active = 0;
            BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_CHECK_LEVEL;
        case BATTLE_QUEUE_STEP_CHECK_LEVEL:
            if (BATTLE_QUEUE->priority == maxPriority) {
                BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_FINISH;
                break;
            }
            g_BattleState.runningActionPriority = *(short *)&BATTLE_QUEUE->priority;
            if (g_BattleState.actionRunSequence[BATTLE_QUEUE->priority] != g_BattleState.actionEnqueueSequence[BATTLE_QUEUE->priority]) {
                BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_FIND_ENTRY;
            } else {
                BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_NEXT_LEVEL;
                break;
            }
        case BATTLE_QUEUE_STEP_FIND_ENTRY:
            for (g_BattleActionQueueIndex = 0; g_BattleActionQueueIndex < BATTLE_ACTION_QUEUE_SIZE; g_BattleActionQueueIndex++) {
                if (g_BattleState.queuedActions[g_BattleActionQueueIndex].level == BATTLE_QUEUE->priority && g_BattleState.queuedActions[g_BattleActionQueueIndex].sequence == g_BattleState.actionRunSequence[BATTLE_QUEUE->priority]) {
                    *(Int64Pair *)&g_BattleState.currentActionKind = *(Int64Pair *)&g_BattleState.queuedActions[g_BattleActionQueueIndex];
                    g_BattleState.queuedActions[g_BattleActionQueueIndex].level = 0xFF;
                    break;
                }
            }
            g_BattleState.actionRunSequence[BATTLE_QUEUE->priority]++;
            if (g_BattleActionQueueIndex != BATTLE_ACTION_QUEUE_SIZE) {
                BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_RUN_ENTRY;
            } else {
                BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_NEXT_LEVEL;
                break;
            }
        case BATTLE_QUEUE_STEP_RUN_ENTRY:
            switch (BATTLE_QUEUE_STATE->phase) {
            case BATTLE_QUEUE_PHASE_TAKE_ACTOR:
                g_BattleActionQueueActor = g_BattleState.queuedActorSlot;
                if (g_BattleActionQueueActor == -1) {
                    BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_FINISH;
                    break;
                }
                if (g_BattleActionQueueActor != 3) {
                    BATTLE_QUEUE_STATE->phase = BATTLE_QUEUE_PHASE_CHECK_ACTOR;
                } else {
                    BATTLE_QUEUE_STATE->phase = BATTLE_QUEUE_PHASE_PICK_COMMAND;
                    break;
                }
            case BATTLE_QUEUE_PHASE_CHECK_ACTOR:
                if (g_aCombatants[g_BattleActionQueueActor].unitId == -1) {
                    BATTLE_QUEUE_STATE->phase = BATTLE_QUEUE_PHASE_TAKE_ACTOR;
                    BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_FINISH;
                    break;
                }
                if (BATTLE_QUEUE->priority > 0 && (g_aCombatants[g_BattleActionQueueActor].status & BATTLE_STATUS_DEATH) != 0) {
                    BATTLE_QUEUE_STATE->phase = BATTLE_QUEUE_PHASE_TAKE_ACTOR;
                    BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_FINISH;
                    break;
                }
                if (BATTLE_QUEUE->priority >= 5) {
                    if (g_BattleState.actors[g_BattleActionQueueActor].atbTick == 0) {
                        ((Int64Pair *)&g_BattleState.actorResumeMarker)[g_BattleActionQueueActor] = *(Int64Pair *)&g_BattleState.currentActionKind;
                        BATTLE_QUEUE_STATE->phase = BATTLE_QUEUE_PHASE_TAKE_ACTOR;
                        BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_FINISH;
                        break;
                    }
                    Battle_CloseActorTurn(g_BattleActionQueueActor);
                }
                BATTLE_QUEUE_STATE->phase = BATTLE_QUEUE_PHASE_PICK_COMMAND;
            case BATTLE_QUEUE_PHASE_PICK_COMMAND:
                if (g_BattleState.queuedBaseCommandId == -1) {
                    forceAttack = 0;
                    fallbackId = 0;
                    if (g_BattleActionQueueActor >= BATTLE_FIRST_ENEMY_SLOT) {
                        pEnemyRecord = (BattleActorQueueRecord *)(g_BattleState.actors[g_BattleActionQueueActor].enemyRecord);
                        if ((g_aCombatants[g_BattleActionQueueActor].status & BATTLE_STATUS_SILENCE) != 0 && (pEnemyRecord->flags & 1) == 0) {
                            forceAttack = 1;
                        }
                        if ((g_aCombatants[g_BattleActionQueueActor].status & BATTLE_STATUS_BERSERK) != 0) {
                            forceAttack = 1;
                        }
                        if (forceAttack != 0) {
                            fallbackId = pEnemyRecord->fallbackAttackId;
                        }
                    }
                    if ((g_aCombatants[g_BattleActionQueueActor].status & BATTLE_STATUS_MANIPULATE) != 0) {
                        BATTLE_QUEUE_STATE->phase = BATTLE_QUEUE_PHASE_TAKE_ACTOR;
                        BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_FINISH;
                        break;
                    }
                    if (forceAttack != 0) {
                        Battle_EnqueueActionDirect(g_BattleActionQueueActor, 2, BATTLE_COMMAND_ENEMY_ATTACK, Battle_FindSceneAttackIndex(fallbackId), 0);
                    } else if ((g_aCombatants[g_BattleActionQueueActor].flags & BATTLE_COMBATANT_AI_CONTROLLED) != 0) {
                        Battle_SignalAtbWaitState(1);
                        Battle_RunAiScriptEvent(g_BattleActionQueueActor, 1, 2);
                        if (g_BattleState.actionRunSequence[2] == g_BattleState.actionEnqueueSequence[2]) {
                            Battle_CloseActorTurn(g_BattleActionQueueActor);
                        }
                        BATTLE_QUEUE->active = 1;
                    }
                    BATTLE_QUEUE_STATE->phase = BATTLE_QUEUE_PHASE_FORMATION_AI;
                    break;
                }
                BATTLE_QUEUE_STATE->phase = BATTLE_QUEUE_PHASE_EXECUTE;
            case BATTLE_QUEUE_PHASE_EXECUTE:
                switch (BATTLE_QUEUE_STATE->subPhase) {
                case BATTLE_QUEUE_SUBPHASE_PREPARE:
                    if ((gBattleAiGlobals.arenaFlags & 8) != 0) {
                        for (partySlot = 0; partySlot < BATTLE_PARTY_SIZE; partySlot++) {
                            Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, partySlot, BATTLE_ATB_EVENT_RELEASE_COMMAND_MENU, 0);
                        }
                    }
                    if (g_BattleActionQueueActor < BATTLE_PARTY_SIZE && g_BattleState.currentActionKind >= 5 && (g_aCombatants[g_BattleActionQueueActor].status & BATTLE_STATUS_BERSERK) != 0) {
                        g_BattleState.queuedBaseCommandId = Battle_GetActorBaseCommandId(g_BattleActionQueueActor);
                        g_BattleState.actionQueueScratch = 0;
                        g_BattleState.actionQueueReset = 0;
                        Battle_InvalidateQueuedActionsForActor(g_BattleActionQueueActor, 6);
                    }
                    BATTLE_QUEUE_STATE->subPhase = BATTLE_QUEUE_SUBPHASE_RUN;
                case BATTLE_QUEUE_SUBPHASE_RUN:
                    Battle_ExecuteAction((BattleAction *)&g_BattleState.currentActionKind);
                    if (BATTLE_QUEUE->actionDone != 0) {
                        BATTLE_QUEUE_STATE->subPhase = BATTLE_QUEUE_SUBPHASE_PREPARE;
                        BATTLE_QUEUE_STATE->phase = BATTLE_QUEUE_PHASE_FORMATION_AI;
                    } else {
                        BATTLE_QUEUE_STATE->busy = 1;
                    }
                }
                break;
            case BATTLE_QUEUE_PHASE_FORMATION_AI:
                Battle_RunFormationAiEvents();
                BATTLE_QUEUE_STATE->phase = BATTLE_QUEUE_PHASE_TAKE_ACTOR;
                BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_FINISH;
            }
            break;
        case BATTLE_QUEUE_STEP_NEXT_LEVEL:
            BATTLE_QUEUE->priority++;
            BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_CHECK_LEVEL;
            break;
        case BATTLE_QUEUE_STEP_FINISH:
            if (BATTLE_QUEUE->active != 0) {
                BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_RESET;
            } else {
                BATTLE_QUEUE_STATE->state = BATTLE_QUEUE_STEP_RESET;
                g_BattleSweepComplete = 1;
                if (BATTLE_QUEUE->priority == maxPriority) {
                    Battle_CloseActorTurn(-1);
                }
            }
        }
    }
    BATTLE_QUEUE_STATE->busy = 0;
    return BATTLE_QUEUE->priority == maxPriority;
}

/*
 * Executes the retained battle action state machine for the specified action record,
 * initializing context, running the command microprogram, and resolving turn effects.
 */
// FUNCTION: FF7 0x00435d81
void __cdecl Battle_ExecuteAction(BattleAction *actionPtr)
{
    int hpMpMask;
    int hitBits;
    int loopIndex;
    int counterBits;
    int programPosition;
    int supportCode;
    BattleActionEntry *commandTable;
    int opNum;
    short actorParameter;
    int workSlotNo;
    int actionTimer;
    int liveMask;

    switch (BATTLE_QUEUE_STATE->executeStage) {
    case BATTLE_EXEC_STAGE_SETUP:
        BATTLE_QUEUE->actionDone = 0;
        g_BattleActionYield = 0;
        g_BattleState.actionPending = 0;
        Battle_AdvanceActionSequenceCounter();
        if (actionPtr->actionType >= 2) {
            g_BattleState.currentActorBitMask = 1 << actionPtr->actorIndex;
        }
        gBattleAiGlobals.commandIndex = actionPtr->flags & 0x3F;
        gBattleAiGlobals.attackId = actionPtr->attackId & 0xFFFF;
        if (actionPtr->actionType > 0) {
            for (loopIndex = 0; loopIndex < BATTLE_ACTOR_COUNT; loopIndex++) {
                Battle_RunAiScriptEvent(loopIndex, 7, 0);
            }
        }
        for (loopIndex = 0; loopIndex < 256; loopIndex++) {
            ((int *)g_pBattleCommandCtx)[loopIndex] = 0;
        }
        g_BattleActionActor = actionPtr->actorIndex;
        supportCode = -1;
        if ((actionPtr->flags & 0x40) != 0) {
            for (loopIndex = 0; loopIndex < 2; loopIndex++) {
                if (g_BattleState.partyStats[g_BattleActionActor].queuedSupportCodes[loopIndex] != BATTLE_SUPPORT_CODE_NONE) {
                    supportCode = g_BattleState.partyStats[g_BattleActionActor].queuedSupportCodes[loopIndex];
                    g_BattleState.partyStats[g_BattleActionActor].queuedSupportCodes[loopIndex] = BATTLE_SUPPORT_CODE_NONE;
                    break;
                }
            }
        }
        actionPtr->flags &= 0x3F;
        if (g_BattleActionActor < BATTLE_PARTY_SIZE && (g_aCombatants[g_BattleActionActor].status & BATTLE_STATUS_FROG) != 0) {
            if (actionPtr->flags == Battle_GetActorBaseCommandId(g_BattleActionActor)) {
                actionPtr->flags = 1;
                actionPtr->targetMask = Battle_PickRandomBitFromMask(actionPtr->targetMask);
            }
        }
        g_pBattleCommandCtx->attackerSlot = g_BattleActionActor;
        g_pBattleCommandCtx->commandIndex = actionPtr->flags;
        g_pBattleCommandCtx->attackIdBase = actionPtr->attackId & 0xFFFF;
        g_pBattleCommandCtx->targetMask = actionPtr->targetMask;
        g_pBattleCommandCtx->attackerLevel = g_aCombatants[g_BattleActionActor].level;
        g_pBattleCommandCtx->attackerStatus = g_aCombatants[g_BattleActionActor].status;
        g_pBattleCommandCtx->actionType = 1;
        g_pBattleCommandCtx->stepId = g_BattleCommandProperties[g_pBattleCommandCtx->commandIndex].stepId;
        g_pBattleCommandCtx->damageCalcByte = g_BattleCommandProperties[g_pBattleCommandCtx->commandIndex].damageCalcByte;
        g_pBattleCommandCtx->attackFlags = g_BattleCommandProperties[g_pBattleCommandCtx->commandIndex].attackFlags;
        g_pBattleCommandCtx->magicIndex = 0xFFFFFFFF;
        g_pBattleCommandCtx->displayValueA = 0xFFFFFFFF;
        g_pBattleCommandCtx->displayValueB = 0xFFFFFFFF;
        g_pBattleCommandCtx->displayValueC = 0xFFFFFFFF;
        g_pBattleCommandCtx->secondaryDisplayValue = 0xFFFFFFFF;
        g_pBattleCommandCtx->enemyRecordIndex = 0xFFFFFFFF;
        g_pBattleCommandCtx->specialFlags = 0xFFFFFFFF;
        g_pBattleCommandCtx->animParam = 0xFFFFFFFF;
        g_pBattleCommandCtx->animParamAlt = 0xFFFFFFFF;
        g_pBattleCommandCtx->effectId = 0xFFFFFFFF;
        g_pBattleCommandCtx->messageId = 0xFFFFFFFF;
        g_pBattleCommandCtx->payload = 0xFFFFFFFF;
        g_pBattleCommandCtx->actorBit = 1 << g_BattleActionActor;
        g_pBattleCommandCtx->resolvedTargetMask = g_pBattleCommandCtx->targetMask;
        g_pBattleCommandCtx->stepPower = g_pBattleCommandCtx->attackIdBase;
        g_pBattleCommandCtx->repeatRemaining = 1;
        g_pBattleCommandCtx->statusChangeId = 0xFF;
        for (loopIndex = 0; loopIndex < 8; loopIndex++) {
            g_pBattleCommandCtx->followUpAttackIds[loopIndex] = 0xFF;
        }
        if (g_pBattleCommandCtx->resolvedTargetMask == 0) {
            if (g_BattleActionActor < BATTLE_PARTY_SIZE) {
                g_pBattleCommandCtx->attackFlags |= BATTLE_ATTACK_FLAG_MULTIPLE_TARGETS;
            }
        } else if (Battle_PopCountMask(g_pBattleCommandCtx->resolvedTargetMask) > 1) {
            g_pBattleCommandCtx->attackFlags |= BATTLE_ATTACK_FLAG_MULTIPLE_TARGETS;
        }
        g_pBattleCommandCtx->targetFlags = 0xFF;
        if (g_pBattleCommandCtx->commandIndex < BATTLE_COMMAND_ENEMY_ATTACK) {
            g_pBattleCommandCtx->targetFlags = g_KernelCommandTable[g_pBattleCommandCtx->commandIndex].targetFlags;
        }
        if (supportCode != -1) {
            if (supportCode >> 8 != 0) {
                Battle_SetRepeatCasts(supportCode >> 8);
            }
            g_pBattleCommandCtx->supportBits = supportCode & 0xFF;
            g_pBattleCommandCtx->attackFlags |= BATTLE_ATTACK_FLAG_SUPPORT_CODE;
        }
        if (g_BattleActionActor < BATTLE_PARTY_SIZE) {
            Battle_FindAbilitySlotForCommand(g_BattleActionActor);
            commandTable = (BattleActionEntry *)g_MenuPartyStats[g_BattleActionActor].commands;
            if ((g_pBattleCommandCtx->attackFlags & BATTLE_ATTACK_FLAG_SUPPORT_CODE) == 0) {
                for (loopIndex = 0; loopIndex < 16; loopIndex++) {
                    if (commandTable[loopIndex].commandIndex == g_pBattleCommandCtx->commandIndex) {
                        if (g_BattleState.currentActionKind >= 5) {
                            g_pBattleCommandCtx->supportBits = commandTable[loopIndex].supportBits;
                        }
                        switch (g_pBattleCommandCtx->commandIndex) {
                        case BATTLE_COMMAND_STEAL:
                        case BATTLE_COMMAND_SENSE:
                        case BATTLE_COMMAND_MORPH:
                        case BATTLE_COMMAND_DEATHBLOW:
                        case BATTLE_COMMAND_MANIPULATE:
                        case BATTLE_COMMAND_MUG:
                            if ((g_pBattleCommandCtx->attackFlags & BATTLE_ATTACK_FLAG_MULTIPLE_TARGETS) != 0) {
                                if (commandTable[loopIndex].counter != 0) {
                                    commandTable[loopIndex].counter--;
                                    Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, g_BattleActionActor, BATTLE_ACTION_EVENT_SET_DIRTY_FLAGS, 1);
                                } else {
                                    g_pBattleCommandCtx->attackFlags |= BATTLE_ATTACK_FLAG_NO_ALL_USES;
                                }
                            }
                            g_pBattleCommandCtx->targetFlags = commandTable[loopIndex].targetFlags;
                        }
                        break;
                    }
                }
            }
        }
        if ((g_BattleState.actors[g_BattleActionActor].timingFlags & 0x80) != 0) {
            g_BattleState.actors[g_BattleActionActor].timingFlags &= 0x7F;
            if (g_BattleState.actors[g_BattleActionActor].hp == BATTLE_LUCKY_7S_HP) {
                g_BattleState.messageWindowRequest = 0x51;
                g_BattleState.limitBanner = 47;
            }
        }
        g_pBattleCommandCtx->stepFlags = g_pBattleCommandCtx->commandIndex;
        for (loopIndex = 0; loopIndex < 3; loopIndex++) {
            if (g_pBattleCommandCtx->stepFlags == g_BattleCommandRemapTable[loopIndex].commandIndex) {
                g_pBattleCommandCtx->stepFlags = g_BattleCommandRemapTable[loopIndex].stepFlags;
            }
        }
        if (g_pBattleCommandCtx->commandIndex < BATTLE_COMMAND_ENEMY_ATTACK) {
            g_pBattleCommandCtx->animParam = g_KernelCommandTable[g_pBattleCommandCtx->commandIndex].animParam;
            g_pBattleCommandCtx->animParamAlt = g_KernelCommandTable[g_pBattleCommandCtx->commandIndex].animParamAlt;
        }
        if (g_BattleActionActor >= BATTLE_FIRST_ENEMY_SLOT) {
            g_pBattleCommandCtx->enemyRecordIndex = g_BattleScene.enemies[g_BattleActionActor - BATTLE_FIRST_ENEMY_SLOT].enemyIndex;
        }
        if ((g_BattleState.actors[g_BattleActionActor].timingFlags & 4) != 0) {
            actorParameter = *(short *)&g_BattleActionActor;
            Battle_PostBattleMessageFormatted(g_BattleActionActor, 0x7A, 1, &actorParameter);
            g_BattleState.actors[g_BattleActionActor].timingFlags &= 0xFB;
        }
        BATTLE_QUEUE_STATE->executeStage = BATTLE_EXEC_STAGE_RUN_PROGRAM;
    case BATTLE_EXEC_STAGE_RUN_PROGRAM:
        if (0, 1) {
            programPosition = g_BattleAttackProgramOffsets[g_pBattleCommandCtx->commandIndex];
            while ((opNum = g_BattleAttackPrograms[programPosition++]) != BATTLE_ATTACK_OP_END) {
                if (opNum != BATTLE_ATTACK_OP_AWAIT) {
                    g_BattleAttackOpCallbacks[opNum]();
                    BATTLE_QUEUE_STATE->executeStage = BATTLE_EXEC_STAGE_FINISH;
                } else {
                    g_BattleAttackOpContinue = 0;
                    g_BattleAttackOpCallbacks[opNum]();
                    if (g_BattleAttackOpContinue != 0) {
                        g_BattleActionYield = 0;
                        BATTLE_QUEUE_STATE->executeStage = BATTLE_EXEC_STAGE_FINISH;
                    } else {
                        g_BattleActionYield = 1;
                        break;
                    }
                }
            }
        }
        if (g_BattleActionYield != 0) {
            return;
        }
    case BATTLE_EXEC_STAGE_FINISH:
        Battle_PruneSequenceWaitFlags();
        if (g_BattleActionActor < BATTLE_PARTY_SIZE && g_pBattleCommandCtx->commandIndex != BATTLE_COMMAND_MIME && g_pBattleCommandCtx->commandIndex < BATTLE_COMMAND_ENEMY_ATTACK) {
            actionTimer = g_BattleState.actors[g_BattleActionActor].turnCount;
            for (workSlotNo = 0; workSlotNo < 2; workSlotNo++) {
                if (actionPtr->actionType != 3 && (BATTLE_ACTION_SLOTS[workSlotNo].actor != g_BattleActionActor || (BATTLE_ACTION_SLOTS[workSlotNo].type != actionTimer || g_pBattleCommandCtx->commandIndex < BATTLE_COMMAND_W_MAGIC))) {
                    BATTLE_ACTION_SLOTS[workSlotNo].actor = *(char *)&g_BattleActionActor;
                    BATTLE_ACTION_SLOTS[workSlotNo].type = actionTimer;
                    BATTLE_ACTION_SLOTS[workSlotNo].commandId = *(char *)&g_pBattleCommandCtx->commandIndex;
                    BATTLE_ACTION_SLOTS[workSlotNo].param = *(short *)&g_pBattleCommandCtx->attackIdBase;
                    BATTLE_ACTION_SLOTS[workSlotNo].extra = *(short *)&g_pBattleCommandCtx->resolvedTargetMask;
                    ((short *)&g_BattleState.actor)[workSlotNo] = *(short *)&g_pBattleCommandCtx->supportBits;
                    if (g_pBattleCommandCtx->repeatCount != 0) {
                        ((short *)&g_BattleState.actor)[workSlotNo] |= (short)(g_pBattleCommandCtx->repeatCount << 8);
                    }
                    if (workSlotNo + 1 < 2) {
                        BATTLE_ACTION_SLOTS[workSlotNo + 1].actor = 0xFF;
                    }
                    break;
                }
            }
        }
        if ((g_pBattleCommandCtx->attackFlags & BATTLE_ATTACK_FLAG_ADDED_CUT) != 0) {
            Battle_EnqueueActionDirect(g_BattleActionActor, 3, BATTLE_COMMAND_ATTACK, 0, g_pBattleCommandCtx->resolvedTargetMask);
        }
        if (g_pBattleCommandCtx->stepFlags == 6) {
            g_pBattleCommandCtx->hitMask = 0;
            g_pBattleCommandCtx->hitTargetBits = 0;
        }
        gBattleAiGlobals.specialFlags = *(short *)&g_pBattleCommandCtx->specialFlags;
        gBattleAiGlobals.commandIndex = *(char *)&g_pBattleCommandCtx->commandIndex;
        gBattleAiGlobals.attackId = *(char *)&g_pBattleCommandCtx->attackIdBase;
        gBattleAiGlobals.fullAttackId = *(short *)&g_pBattleCommandCtx->attackId;
        gBattleAiGlobals.elementMask = *(short *)&g_pBattleCommandCtx->elementMask;
        liveMask = ~(g_BattleState.excludedActorMask | *(unsigned short *)&g_BattleState.deathClearedMask);
        hitBits = g_pBattleCommandCtx->hitMask & liveMask;
        counterBits = g_pBattleCommandCtx->hitTargetBits & liveMask;
        hpMpMask = g_pBattleCommandCtx->hpMpPoolMask & liveMask;
        for (loopIndex = 0; loopIndex < BATTLE_ACTOR_COUNT; loopIndex++) {
            if ((hpMpMask & 1 << loopIndex) != 0) {
                g_aCombatants[loopIndex].attackerMask = 1 << g_BattleActionActor;
                Battle_HandleActorDeath(g_BattleActionActor, loopIndex, 0);
            } else if ((hitBits & 1 << loopIndex) != 0 && (*(unsigned short *)&g_BattleState.currentActorBitMask & 1 << loopIndex) == 0 && (g_aCombatants[loopIndex].status & (BATTLE_STATUS_SLEEP | BATTLE_STATUS_STOP | BATTLE_STATUS_PETRIFY | BATTLE_STATUS_MANIPULATE | BATTLE_STATUS_PARALYSIS)) == 0) {
                g_BattleState.currentActorBitMask |= (short)(1 << loopIndex);
                g_aCombatants[loopIndex].attackerMask = 1 << g_BattleActionActor;
                Battle_RunAiScriptEvent(loopIndex, 2, 0);
                if ((counterBits & 1 << loopIndex) != 0) {
                    if (g_BattleActionActor >= BATTLE_FIRST_ENEMY_SLOT) {
                        Battle_RollReactionTriggers(loopIndex, 2);
                    }
                    if ((*(unsigned short *)&gBattleAiGlobals.specialFlags & 4) != 0) {
                        g_aCombatants[loopIndex].magicalAttackerMask = 1 << g_BattleActionActor;
                        Battle_RunAiScriptEvent(loopIndex, 5, 0);
                    } else {
                        g_aCombatants[loopIndex].physicalAttackerMask = 1 << g_BattleActionActor;
                        Battle_RunAiScriptEvent(loopIndex, 4, 0);
                    }
                }
            }
        }
        Battle_CheckLucky7s();
        if (g_BattleReplayActionCount != 0) {
            g_BattleReplayActionCount--;
        } else if (actionPtr->actionType <= 2) {
            g_BattleState.actors[g_BattleActionActor].turnCount++;
        }
        BATTLE_QUEUE_STATE->executeStage = BATTLE_EXEC_STAGE_SETUP;
        BATTLE_QUEUE->actionDone = 1;
    }
}

/*
 * Closes the active turn for the tracked actor, releasing pending drops and posting
 * turn completion events if the actor has no remaining queued actions.
 */
// FUNCTION: FF7 0x00436a5b
void __cdecl Battle_CloseActorTurn(int actor)
{
    int i;

    if (g_BattleTrackedTurnActor != -1) {
        if (g_BattleTrackedTurnActor != actor) {
            for (i = 0; i < BATTLE_ACTION_QUEUE_SIZE; i++) {
                if (g_BattleState.queuedActions[i].level == 6 && g_BattleState.queuedActions[i].actorIndex == g_BattleTrackedTurnActor) {
                    break;
                }
            }
            if (i == BATTLE_ACTION_QUEUE_SIZE) {
                g_BattleState.actors[g_BattleTrackedTurnActor].turnCount++;
                Battle_ReleasePendingDrops(*(short *)&g_BattleTrackedTurnActor);
                Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, g_BattleTrackedTurnActor, BATTLE_ATB_EVENT_RESET_ATB, 0);
            }
            g_BattleTrackedTurnActor = actor;
        }
    } else {
        g_BattleTrackedTurnActor = actor;
    }
}

/*
 * Builds the index lookup table of attack program byte offsets by scanning
 * g_BattleAttackPrograms for 0x1F command sequence terminators.
 */
// FUNCTION: FF7 0x00436b1b
void __cdecl Battle_BuildAttackProgramTable(void)
{
    int bytePos;
    int nextStart;
    int tableIdx;

    tableIdx = 0;
    bytePos = 0;
    for (nextStart = 0; (unsigned int)bytePos < sizeof g_BattleAttackPrograms; bytePos++) {
        if (bytePos == nextStart) {
            g_BattleAttackProgramOffsets[tableIdx] = bytePos;
            tableIdx++;
        }
        if (g_BattleAttackPrograms[bytePos] == BATTLE_ATTACK_OP_END) {
            nextStart = bytePos + 1;
        }
    }
}

/*
 * Checks party members for 7777 HP (0x1E61 Lucky 7s condition), triggering
 * immediate attack actions if the condition is met and limit count is below 64.
 */
// FUNCTION: FF7 0x00436b84
void __cdecl Battle_CheckLucky7s(void)
{
    int i;
    for (i = 0; i < BATTLE_PARTY_SIZE; i++) {
        if (g_aCombatants[i].curHp == BATTLE_LUCKY_7S_HP) {
            if ((g_BattleState.actors[i].timingFlags & 0x80) == 0) {
                if (g_BattleState.lucky7Counter++ < 64) {
                    g_BattleState.actors[i].timingFlags |= 0x80;
                    Battle_EnqueueActionDirect(i, 1, BATTLE_COMMAND_ATTACK, 0, 0);
                }
            }
        }
    }
}
} /* extern "C" */
