// src/ff7/src/battle/battle_command_ring_atb.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Manages the battle command ring and ATB timing: resetting the ring, freezing and
//   ticking the gauges, queuing actions and turn order, and taking snapshots of HP and MP.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "ff7/battle.h"
#include "ff7/enums/battle.h"
#include "ff7/enums/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "lib/codegen.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include "lib/enums/file.h"
#include "ff7/types/menu.h"
extern "C" {


// clang-format off
// GLOBAL: FF7 0x009ae178
int g_BattleCommandRingHead = 0;
// GLOBAL: FF7 0x009ae17c
int g_BattleFrameCount = 0;
// GLOBAL: FF7 0x009ae180
int g_BattleAtbFreezeAccumulator = 0;
// GLOBAL: FF7 0x009ae184
int g_BattleCommandRingTail = 0;
// GLOBAL: FF7 0x009ae188
struct BattleCommandRingEntry g_BattleCommandRing[16] = {0};
// GLOBAL: FF7 0x009ae208
int g_BattleAtbFreezeFlag = 0;
unsigned char g_bssPad_009ae20c[4] = {0};
unsigned char g_bssPad_009ae210[8] = {0};
// clang-format on

/* An ATB gauge is full at 0xFFFF; a limit gauge (its high byte) at 0xFF. */
#define BATTLE_ATB_GAUGE_FULL 0xFFFF
#define BATTLE_LIMIT_GAUGE_FULL 0xFF
/* The escape check runs each time the battle clock passes this many units. */
#define BATTLE_ESCAPE_PERIOD 0x2000

/*
 * Resets the battle command ring head, tail, and associated ATB state.
 */
// FUNCTION: FF7 0x00432040
void __cdecl Battle_ResetCommandRing(void)
{
    g_BattleFrameCount = 0;
    g_BattleAtbFreezeAccumulator = 0;
    g_BattleAtbFreezeFlag = 0;
    g_BattleCommandRingHead = 0;
    g_BattleCommandRingTail = 0;
}

/*
 * Get-or-set accessor for the ATB freeze flag. If mode is negative, returns the
 * current flag value. Otherwise, updates the flag, clears the freeze accumulator,
 * and returns 0.
 */
// FUNCTION: FF7 0x00432077
int __cdecl Battle_GetSetAtbFreezeFlag(int mode)
{
    int ret;

    ret = 0;
    if (mode < 0) {
        ret = g_BattleAtbFreezeFlag;
    } else {
        g_BattleAtbFreezeFlag = mode;
        g_BattleAtbFreezeAccumulator = 0;
    }
    return ret;
}

/*
 * The ATB clock, run once a battle frame: it fills the time gauges, gives full ones their turn,
 * makes escaping progress and fills the limit gauges.
 *
 * Nothing but the limit gauges moves while the ATB is frozen (Battle_IsAtbFrozen: Wait mode in a
 * submenu, a scene that pauses the battle, a Battle Square break). Otherwise:
 *
 * - Escaping. While the escape counter (g_BattleState.sceneMode) is running, the frames with L1+R1
 *   held are counted. Each time the battle clock passes BATTLE_ESCAPE_PERIOD units, the counter goes
 *   down by one if the buttons were held for more than half of that period, with 1-in-4 odds
 *   (turnRandomWord < 0x4000), every time in a pre-emptive or side-attack battle (layout types 1
 *   and 3). Battle_ServiceTurnQueueAndEscape lets the party flee when it reaches 0.
 * - Time gauges. Each combatant's gauge (0..0xFFFF) gains its atbTick, which
 *   Battle_RefreshActorTickRates derives from Dexterity, Haste and Slow; bit 0 of its ATB flags
 *   quadruples the gain until the gauge fills. A combatant whose menu is suppressed
 *   (actorMenuSuppressMask, Battle_EventSetActorMenuSuppressed) gains nothing.
 * - Turns. A full gauge gives an enemy, or a party member under AI control, its AI script's action
 *   (command -1); a party member under auto control (Battle_EventEnterAutoControl) its base command;
 *   any other party member opens the command menu.
 * - Limit gauges. The shown gauge climbs 4 a frame toward the value its owner's hits loaded; at
 *   0xFF the Limit is ready and the banner shows. Under BATTLE_ARENA_FIXED_ATB the goal is the time
 *   since the battle started instead, full after gBattleAiGlobals.atbPeriod frames.
 */
// FUNCTION: FF7 0x004320ac
void __cdecl Battle_TickAtbAndGauges(void)
{
    int actorIdx;
    int heldMask;
    int tickRate;
    int gaugeValue;
    BattleTurnQueueEntry *partyBlock;
    BattleAtbRecord *actorAtb;
    int limitGoal;
    int limitCurrent;

    heldMask = g_InputHeldMask;
    Battle_FlushEventQueue(BATTLE_EVENT_QUEUE_ATB);
    g_BattleAtbFrozen |= 1;
    if (Battle_IsAtbFrozen() == 0) {
        *(unsigned short *)&g_BattleAtbFrozen &= 0xFFFE;
        // An escape in progress: the counter is nonzero and the run buttons were held last frame.
        if (*(unsigned short *)&g_BattleState.sceneMode != 0 && *(unsigned short *)&g_BattleState.sceneMode == (g_BattleState.scenePrevMode)) {
            if ((heldMask & (INPUT_L1 | INPUT_R1)) == (INPUT_L1 | INPUT_R1)) {
                g_BattleState.atbTickCountA++;
            }
            g_BattleState.atbTickCountB++;
            g_BattleState.atbAccumulator += g_BattleState.atbSpeedValue;
            if (*(unsigned short *)&g_BattleState.atbAccumulator > BATTLE_ESCAPE_PERIOD) {
                g_BattleState.atbAccumulator -= BATTLE_ESCAPE_PERIOD;
                // Held for more than half the period, then a 1-in-4 roll unless pre-emptive or side attack.
                if (*(unsigned short *)&g_BattleState.atbTickCountB < *(unsigned short *)&g_BattleState.atbTickCountA << 1 && ((g_BattleState.turnRandomWord) < 0x4000 || ((g_BattleState.layoutType) == 1 || (g_BattleState.layoutType) == 3))) {
                    g_BattleState.sceneMode--;
                }
                g_BattleState.atbTickCountB = 0;
                g_BattleState.atbTickCountA = 0;
            }
        }
        Battle_AccumulateTimeSlices();
        for (actorIdx = 0; actorIdx < 0xA; actorIdx++) {
            if ((1 << actorIdx & ((g_BattleScene.targetMask | BATTLE_ACTOR_MASK_ENEMIES) & ((*(unsigned short *)&g_BattleScene.aiTargetableMask) | BATTLE_ACTOR_MASK_PARTY))) != 0) {
                actorAtb = (BattleAtbRecord *)&g_BattleState.actors[actorIdx];
                gaugeValue = actorAtb->gauge;
                tickRate = actorAtb->atbTick;
                if (gaugeValue != BATTLE_ATB_GAUGE_FULL) {
                    if ((actorAtb->flags & 1) != 0) {
                        tickRate <<= 2;
                    }
                    if ((g_BattleState.actorMenuSuppressMask & 1 << actorIdx) != 0) {
                        tickRate = 0;
                    }
                    gaugeValue += tickRate;
                    if (gaugeValue > BATTLE_ATB_GAUGE_FULL) {
                        gaugeValue = BATTLE_ATB_GAUGE_FULL;
                    }
                    actorAtb->gauge = gaugeValue;
                    // The gauge is full: this combatant's turn.
                    if (gaugeValue == BATTLE_ATB_GAUGE_FULL) {
                        if (actorIdx >= 3 || (g_aCombatants[actorIdx].flags & BATTLE_COMBATANT_AI_CONTROLLED) != 0) {
                            // An enemy, or a party member the AI drives: its script picks the action.
                            Battle_PostCommand((short)actorIdx, -1, 0, 0);
                        } else if ((*(unsigned short *)&g_BattleState.actorAutoControlMask & 1 << actorIdx) != 0) {
                            Battle_PostCommand((short)actorIdx, Battle_GetActorBaseCommandId(actorIdx), 0, 0);
                        } else {
                            Battle_RequestCommandMenu(actorIdx);
                            g_BattleState.actorCommandPendingMask |= (short)(1 << actorIdx);
                        }
                        actorAtb->flags &= 0xFFFE;
                        g_BattleAtbFreezeFlag |= 2;
                    }
                }
            }
        }
    }
    Battle_UpdatePartyGaugeHud();
    for (actorIdx = 0; actorIdx < 3; actorIdx++) {
        partyBlock = (BattleTurnQueueEntry *)g_MenuPartyStats[actorIdx].statBlock;
        limitCurrent = partyBlock->limitGauge >> 8;
        limitGoal = *(unsigned short *)&g_BattleState.partyStats[actorIdx].loadLimitGauge;
        if ((gBattleAiGlobals.arenaFlags & 8) != 0) {
            limitGoal = g_BattleFrameCount * BATTLE_LIMIT_GAUGE_FULL / gBattleAiGlobals.atbPeriod;
            if (limitGoal > BATTLE_LIMIT_GAUGE_FULL) {
                limitGoal = BATTLE_LIMIT_GAUGE_FULL;
            }
        }
        if (limitCurrent < limitGoal) {
            limitCurrent += 4;
            if (limitCurrent > limitGoal) {
                limitCurrent = limitGoal;
            }
            if (limitCurrent == BATTLE_LIMIT_GAUGE_FULL) {
                Battle_SetLimitReady(actorIdx);
                Battle_PostEvent(BATTLE_EVENT_QUEUE_TURN, actorIdx, BATTLE_TURN_EVENT_SHOW_LIMIT_BANNER, 0);
            }
            partyBlock->limitGauge = limitCurrent << 8;
        }
    }
    for (actorIdx = 0; actorIdx < 3; actorIdx++) {
        g_MenuPartyStats[actorIdx].atbGauge = g_BattleState.actors[actorIdx].atbGauge;
    }
    Battle_CollectDropsToInventory();
}

/*
 * Whether the battle clock stands still this frame. It does in Wait mode while the player is in a
 * submenu or picking a target (the command list itself does not stop it), while a scene pauses the
 * battle (Battle_IsSceneInterruptible), and during a Battle Square break (arenaFlags bits 0-1).
 *
 * In Recommended and Wait mode Battle_SignalAtbWaitState sets the freeze flag to 3 while a party
 * command plays and another waits behind it. The clock then runs on for 0x4000 units and stops
 * until the flag changes.
 */
// FUNCTION: FF7 0x004324b6
int __cdecl Battle_IsAtbFrozen(void)
{
    int frozen;

    frozen = 0;
    if (g_BattleAtbFreezeFlag == 3) {
        g_BattleAtbFreezeAccumulator += g_BattleState.atbSpeedValue;
    }
    if ((*(unsigned short *)&g_BattleState.atbMode) == BATTLE_ATB_WAIT) {
        switch (g_BattleMenuState) {
        case BATTLE_MENU_SELECT_TARGET:
        case BATTLE_MENU_ENEMY_SKILL_LIST:
        case BATTLE_MENU_ITEM_LIST:
        case BATTLE_MENU_MAGIC_LIST:
        case BATTLE_MENU_SUMMON_LIST:
        case BATTLE_MENU_UNUSED_08:
        case BATTLE_MENU_LIMIT_LIST:
        case BATTLE_MENU_CAIT_SITH_SLOTS:
        case BATTLE_MENU_TIFA_SLOTS:
            frozen = 1;
        }
    }
    frozen |= Battle_IsSceneInterruptible();
    frozen |= (gBattleAiGlobals.arenaFlags & 3) != 0;
    if (g_BattleAtbFreezeAccumulator > 0x4000) {
        frozen = 1;
    }
    return frozen;
}

/*
 * Puts an action in the battle's action queue (64 slots), numbered within its priority level so
 * that actions of one level run in the order they were queued. Queuing an action of level 2 or
 * higher ends the actor's Defend, and starts it again if the action is Defend: Defend lasts until
 * the defender's next action.
 */
// FUNCTION: FF7 0x0043258a
void __cdecl Battle_EnqueueAction(void *action)
{
    BattleQueuedAction *pQueued;
    int actionType;
    int slotIdx;

    pQueued = (BattleQueuedAction *)action;
    actionType = pQueued->level;
    for (slotIdx = 0; slotIdx < 0x40; slotIdx++) {
        if (g_BattleState.queuedActions[slotIdx].level == 0xFF) {
            pQueued->sequence = (*(char *)&g_BattleState.actionEnqueueSequence[actionType]);
            *(Int64Pair *)&g_BattleState.queuedActions[slotIdx] = *(Int64Pair *)pQueued;
            ((*(char *)&g_BattleState.actionEnqueueSequence[actionType]))++;
            g_BattleState.selectedTargetIndex = actionType;
            if (pQueued->level >= 2) {
                g_aCombatants[pQueued->actorIndex].flags &= ~BATTLE_COMBATANT_DEFENDING;
                if ((pQueued->commandId & 0x3F) == BATTLE_COMMAND_DEFEND) {
                    g_aCombatants[pQueued->actorIndex].flags |= BATTLE_COMBATANT_DEFENDING;
                }
            }
            return;
        }
    }
}

/*
 * Builds an action record from individual parameters and enqueues it.
 */
// FUNCTION: FF7 0x00432687
void __cdecl Battle_EnqueueActionDirect(int actor, int priority, int commandId, int actionIdx, int targetMask)
{
    struct {
        unsigned char priority;
        unsigned char pad;
        unsigned char actor;
        unsigned char commandId;
        unsigned short actionIdx;
        unsigned short targetMask;
    } action;

    action.priority = priority;
    action.actor = actor;
    action.commandId = commandId;
    action.actionIdx = actionIdx;
    action.targetMask = targetMask;
    Battle_EnqueueAction(&action);
}

/*
 * The per-frame turn service: it starts or refuses an escape, moves the commands the player and
 * the AI posted into the action queue, picks each party member's idle pose and shows the Death
 * Sentence and Slow-numb countdowns.
 *
 * Escaping: holding L1+R1 runs the escape counter that Battle_TickAtbAndGauges counts down, unless
 * the formation forbids it (g_BattleScene.flags bit 3), every living party member is stopped, or
 * enemies stand on both sides (a pincer attack); then message 0x36 shows instead. When the counter
 * reaches 0, the battle ends as an escape (gBattleAiGlobals.flags bit 2) and the party turns to
 * run (pose 0xD).
 *
 * Idle poses: 2 for a member with Death; 1, the weak stance, when count * 4 < limit in the
 * member's menu stat block or under Sleep, Poison, Silence, Death Sentence, Paralysis, Darkness or
 * Dual Drain; 0xD while running.
 */
// FUNCTION: FF7 0x004326bf
void __cdecl Battle_ServiceTurnQueueAndEscape(void)
{
    int partyIndex;
    int escapeHeld;
    int waitingMask;
    int actorBit;
    BattleTurnQueueEntry *turnEntryPtr;
    int memberIdx;
    BattleCombatant *pUnitPtr;
    char *statePtrPtr;
    int idleScript;
    int timerVal;
    int stateBits;
    int lowFlag;
    BattleSlipEntry *slipEntry;

    waitingMask = 0;
    escapeHeld = (g_InputHeldMask & (INPUT_L1 | INPUT_R1)) == (INPUT_L1 | INPUT_R1);
    g_BattleFrameCount++;
    g_BattleState.turnRandomWord = Battle_NextRandom16();
    Battle_FlushEventQueue(BATTLE_EVENT_QUEUE_TURN);
    for (partyIndex = 0; partyIndex < 3; partyIndex++) {
        actorBit = 1 << partyIndex;
        if (gBattleAiGlobals.targetMask & actorBit) {
            if (g_BattleState.actors[partyIndex].atbTick == 0) {
                waitingMask |= actorBit;
            }
        }
    }
    if (gBattleAiGlobals.flags == 0 && escapeHeld != 0) {
        // Escape allowed: by the formation, with someone able to move, and not in a pincer.
        if ((g_BattleScene.flags & 8) == 0 &&
            waitingMask != gBattleAiGlobals.targetMask &&
            g_BattleScene.occupiedEnemySideCount < 2) {
            // The counter has just reached 0: the party escapes.
            if (g_BattleState.scenePrevMode != *(unsigned short *)&g_BattleState.sceneMode &&
                *(unsigned short *)&g_BattleState.sceneMode == 0) {
                *(short *)&gBattleAiGlobals.flags |= 4;
                for (memberIdx = 0; memberIdx < 3; memberIdx++) {
                    g_aCombatants[memberIdx].idleOverride = 0xD;
                }
            }
            g_BattleState.scenePrevMode = g_BattleState.sceneMode;
        } else if (Gfx_IsDrawEnabled()) {
            Battle_SetMessageWindowRequest(0x36);
        }
    }
    if (*(unsigned short *)&g_BattleState.limitBanner != 0) {
        if (Gfx_IsDrawEnabled()) {
            Battle_SetMessageWindowRequest(g_BattleState.messageWindowRequest);
        }
        if (g_BattleIsRunning != 0) {
            g_BattleState.limitBanner -= 1;
        }
    }
    while (g_BattleCommandRingTail != g_BattleCommandRingHead) {
        Battle_EnqueueAction(&g_BattleCommandRing[g_BattleCommandRingTail]);
        g_BattleCommandRingTail = (g_BattleCommandRingTail + 1) & 0xF;
    }
    for (partyIndex = 0; partyIndex < 3; partyIndex++) {
        statePtrPtr = (char *)&g_BattleState.actors[partyIndex];
        turnEntryPtr = (BattleTurnQueueEntry *)g_MenuPartyStats[partyIndex].statBlock;
        pUnitPtr = &g_aCombatants[partyIndex];
        lowFlag = turnEntryPtr->count * 4 < turnEntryPtr->limit;
        idleScript = *(unsigned char *)&pUnitPtr->idleOverride;
        if (g_BattleScene.actors[partyIndex].statusFlags & BATTLE_STATUS_DEATH) {
            idleScript = 2;
        } else if (gBattleAiGlobals.flags == 0 &&
                   idleScript == 0) {
            stateBits = g_BattleScene.actors[partyIndex].statusFlags;
            if (lowFlag != 0 || (stateBits & (BATTLE_STATUS_SLEEP | BATTLE_STATUS_POISON | BATTLE_STATUS_SILENCE | BATTLE_STATUS_DEATH_SENTENCE | BATTLE_STATUS_PARALYSIS | BATTLE_STATUS_DARKNESS | BATTLE_STATUS_DUAL_DRAIN))) {
                idleScript = 1;
            }
            if (escapeHeld != 0) {
                if ((waitingMask & (1 << partyIndex)) == 0) {
                    idleScript = 0xD;
                }
            }
        }
        pUnitPtr->scriptId = idleScript;
    }
    for (partyIndex = 0; partyIndex < 10; partyIndex++) {
        slipEntry = (BattleSlipEntry *)&g_BattleState.actors[partyIndex];
        timerVal = 0xFF;
        if (g_BattleScene.actors[partyIndex].statusFlags & BATTLE_STATUS_DEATH_SENTENCE) {
            timerVal = slipEntry->deathSentenceTimer;
            if (timerVal != 0xFF) {
                timerVal -= Battle_GetSlipClockElapsed(partyIndex);
            }
            if (timerVal < 0) {
                timerVal = 0;
            }
        }
        g_BattleScene.actors[partyIndex].deathSentenceTimer = timerVal;
        timerVal = 0xFF;
        if (g_BattleScene.actors[partyIndex].statusFlags & BATTLE_STATUS_SLOW_NUMB) {
            timerVal = slipEntry->slowNumbTimer - Battle_GetSlipClockElapsed(partyIndex);
            if (timerVal < 0) {
                timerVal = 0;
            }
        }
        g_BattleScene.actors[partyIndex].slowNumbTimer = timerVal;
    }
    for (partyIndex = 0; partyIndex < 3; partyIndex++) {
        g_BattleScene.actors[partyIndex].scriptId =
            g_aCombatants[partyIndex].scriptId;
    }
}

/*
 * Posts a chosen command (from the menu, the AI or auto control) to the 16-entry command ring that
 * Battle_ServiceTurnQueueAndEscape moves into the action queue. A Limit is queued at level 5, any
 * other command at level 6. An Item, Throw or W-Item command claims its item record at once
 * (Battle_ClaimPendingDropRecord). The command menu closes and the actor no longer waits for one.
 * A full ring drops the command.
 */
// FUNCTION: FF7 0x00432aa2
void __cdecl Battle_PostCommand(short actor, short commandId, short target, short extra)
{
    int commander;
    BattleCommandRingEntry *entry;
    int kind;

    if (g_BattleCommandRingTail != (g_BattleCommandRingHead + 1 & 0xF)) {
        entry = &g_BattleCommandRing[g_BattleCommandRingHead];
        entry->kind = (commandId != BATTLE_COMMAND_LIMIT) + 5;
        entry->actor = actor;
        entry->commandId = commandId;
        entry->target = target;
        entry->extra = extra;
        kind = commandId;
        if (kind != BATTLE_COMMAND_ITEM && kind != BATTLE_COMMAND_THROW && kind != BATTLE_COMMAND_W_ITEM) { /* empty first arm: lib/codegen.h */
        } else {
            Battle_ClaimPendingDropRecord((short)actor, (short)target);
        }
        commander = Battle_ResolveCommandingActor(actor);
        Battle_ReleaseCommandMenu(commander);
        g_BattleState.actorCommandPendingMask &= (short)~(1 << actor);
        g_BattleState.actorCommandIssuedMask |= (short)(1 << actor);
        g_BattleCommandRingHead = g_BattleCommandRingHead + 1 & 0xF;
    }
}

/*
 * Copies HP and MP values for all 10 combatants into the snapshot buffer.
 */
// FUNCTION: FF7 0x00432ba0
void __cdecl Battle_SnapshotHpMp(void)
{
    int actor;

    for (actor = 0; actor < 0xA; actor++) {
        g_BattleState.actors[actor].hp = *(short *)&g_aCombatants[actor].curHp;
        g_BattleState.actors[actor].mp = *(short *)&g_aCombatants[actor].curMp;
    }
}

/*
 * For puppet/controlled actors (>= 4), resolves the commanding party slot.
 * Otherwise, returns the actor index unchanged.
 */
// FUNCTION: FF7 0x00432bf6
int __cdecl Battle_ResolveCommandingActor(int actor)
{
    int slot;

    if (actor >= 4 && (g_BattleState.actorMenuSuppressMask & 1 << actor) != 0) {
        for (slot = 0; slot < 3; slot++) {
            if (g_BattleState.partyStats[slot].manipulatedSlot == actor) {
                actor = slot;
                break;
            }
        }
    }
    return actor;
}
} /* extern "C" */
