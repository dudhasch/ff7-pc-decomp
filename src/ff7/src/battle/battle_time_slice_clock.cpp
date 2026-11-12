// src/ff7/src/battle/battle_time_slice_clock.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The battle time-slice clock: accumulates actor and party time into two alternating
//   buffers, and previews the party's gauge HUD values from the buffer currently filling.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/enums/battle.h"
#include "lib/codegen.h"
#include "ff7/battle.h"
#include "lib/crt.h"
#include "ff7/api/menu.h"
#include "ff7/types/menu.h"
extern "C" {

// clang-format off
// clang-format on


// clang-format off
// GLOBAL: FF7 0x009ae8a0
int g_BattleSlipBufferIndex = 0;
unsigned char g_bssPad_009ae8a4[4] = {0};
// GLOBAL: FF7 0x009ae8a8
int g_BattleSlipRegenAccumulators[10] = {0};
// GLOBAL: FF7 0x009ae8d0
int g_BattleSlipSharedTimeAccumulators[2] = {0};
// GLOBAL: FF7 0x009ae8d8
int g_BattleSlipSecondTimeAccumulators[10] = {0};
// GLOBAL: FF7 0x009ae900
int g_BattleSlipFirstTimeAccumulators[10] = {0};
// GLOBAL: FF7 0x009ae928
int g_BattleSlipSliceBufferA[2][10] = {0};
// GLOBAL: FF7 0x009ae978
int g_BattleSlipSliceBufferB[2][10] = {0};
// clang-format on

/*
 * How far the actor's Death Sentence and Slow-numb countdowns have run since the last update: the
 * whole units in the buffer now filling. Battle_ServiceTurnQueueAndEscape subtracts it from the
 * shown countdowns, so the numbers tick down every frame, not only when an action ends.
 */
// FUNCTION: FF7 0x00435250
int __cdecl Battle_GetSlipClockElapsed(int actor)
{
    return g_BattleSlipSliceBufferB[g_BattleSlipBufferIndex][actor] >> 0xC;
}

/*
 * Lets the time that passed take effect, once after each action (BATTLE_PHASE_APPLY_ACTION): HP
 * slip and the status timers of all ten combatants. Battle_AccumulateTimeSlices filled the buffer
 * frame by frame; this swaps buffers and spends the full one, keeping the remainders.
 *
 * - HP slip: slipRate is +maxHp/32 under Regen and -maxHp/32 under Dual Drain
 *   (Battle_RefreshSlipHpRate), paid per 0x8000 units of the actor's speed stream. HP stops at
 *   max, and a drain below 0 kills.
 * - Status timers count down in three clocks. Stop runs on the battle clock alone, so Haste and
 *   Slow do not shorten or lengthen it. Death Sentence and Slow-numb run on the actor's tick rate
 *   (Battle_RefreshActorTickRates: 34 a frame under Slow, 68 normally, 136 under Haste). All
 *   others (Paralysis, Barrier, MBarrier, Poison, Sleep, Regen, ...) run on the actor's speed
 *   stream. A timer that reaches 0 posts its expiry event to the action queue.
 */
// FUNCTION: FF7 0x0043526a
void __cdecl Battle_ApplyTimeSlices(void)
{
    int *bufferB;
    int *bufferA;
    int slot;
    int secondAdd;
    int secondSlices;
    int slipRate;
    int regen;
    int step;
    int timer;
    int newHp;
    int maxHp;
    int globalSlices;
    int bufferSel;
    int firstTicks;
    int timerIndex;
    int firstAdd;

    bufferSel = g_BattleSlipBufferIndex;
    g_BattleSlipBufferIndex ^= 1;
    bufferA = (int *)(bufferSel * 0x28 + (char *)&g_BattleSlipSliceBufferA);
    bufferB = (int *)(bufferSel * 0x28 + (char *)&g_BattleSlipSliceBufferB);
    globalSlices = g_BattleSlipSharedTimeAccumulators[bufferSel] >> 0xD;
    g_BattleSlipSharedTimeAccumulators[bufferSel] &= 0x1FFF;
    for (slot = 0; slot < 0xA; slot++) {
        slipRate = g_BattleState.actors[slot].slipRate;
        firstAdd = bufferA[slot];
        secondAdd = bufferB[slot];
        g_BattleSlipFirstTimeAccumulators[slot] += firstAdd;
        firstTicks = g_BattleSlipFirstTimeAccumulators[slot] >> 0xD;
        g_BattleSlipFirstTimeAccumulators[slot] &= 0x1FFF;
        g_BattleSlipSecondTimeAccumulators[slot] += secondAdd;
        secondSlices = g_BattleSlipSecondTimeAccumulators[slot] >> 0xC;
        g_BattleSlipSecondTimeAccumulators[slot] &= 0xFFF;
        g_BattleSlipRegenAccumulators[slot] += firstAdd * slipRate;
        regen = g_BattleSlipRegenAccumulators[slot] >> 0xF;
        g_BattleSlipRegenAccumulators[slot] &= 0x7FFF;
        bufferA[slot] = 0;
        bufferB[slot] = 0;
        if ((g_aCombatants[slot].status & BATTLE_STATUS_DEATH) == 0) {
            newHp = g_aCombatants[slot].curHp + regen;
            maxHp = g_aCombatants[slot].maxHp;
            if (newHp < 0) {
                g_aCombatants[slot].status |= BATTLE_STATUS_DEATH;
                Battle_HandleActorDeath(slot, slot, 1);
                Battle_RunFormationAiEvents();
                newHp = 0;
            } else if (newHp > maxHp) {
                newHp = maxHp;
            }
            g_aCombatants[slot].curHp = newHp;
        }
        for (timerIndex = 0; timerIndex < 0x10; timerIndex++) {
            timer = (unsigned char)g_BattleState.actors[slot].statusTimers[timerIndex];
            if (timer != 0) {
                switch (timerIndex) {
                case BATTLE_STATUS_TIMER_STOP:
                    step = globalSlices;
                    break;
                case BATTLE_STATUS_TIMER_DEATH_SENTENCE:
                case BATTLE_STATUS_TIMER_SLOW_NUMB:
                    step = secondSlices;
                    break;
                default:
                    step = firstTicks;
                }
                timer -= step;
                if (timer < 0) {
                    timer = 0;
                }
                if (timer == 0) {
                    Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, slot, timerIndex, timerIndex);
                }
                g_BattleState.actors[slot].statusTimers[timerIndex] = timer;
            }
        }
    }
}

/*
 * Banks one frame of time for the status clocks: the battle clock's step (atbSpeedValue) and each
 * combatant's speed and tick rates, which Haste, Slow and Stop set. Battle_ApplyTimeSlices spends
 * it after the next action.
 */
// FUNCTION: FF7 0x0043553f
void __cdecl Battle_AccumulateTimeSlices(void)
{
    int *bufferB;
    int actorIdx;
    int *bufferA;
    int *sharedAccum;

    bufferA = (int *)(g_BattleSlipBufferIndex * 0x28 + (char *)&g_BattleSlipSliceBufferA);
    bufferB = (int *)(g_BattleSlipBufferIndex * 0x28 + (char *)&g_BattleSlipSliceBufferB);
    sharedAccum = (int *)(g_BattleSlipBufferIndex * 4 + (char *)&g_BattleSlipSharedTimeAccumulators);
    *sharedAccum += g_BattleState.atbSpeedValue;
    for (actorIdx = 0; actorIdx < 0xA; actorIdx++) {
        bufferA[actorIdx] += g_BattleState.actors[actorIdx].speedRate;
        bufferB[actorIdx] += *(unsigned short *)&g_BattleState.actors[actorIdx].tickRate;
    }
}

/*
 * Shows the party's HP and their Barrier and MBarrier gauges as they will be once the banked time
 * is spent, so Regen and the barriers run down smoothly on screen between actions. It reads the
 * filling buffer and changes nothing. During a Battle Square break the HP and MP shown are kept.
 */
// FUNCTION: FF7 0x004355f0
void __cdecl Battle_UpdatePartyGaugeHud(void)
{
    int *bufferAddr;
    int barrierGauge;
    int partyIndex;
    int hp;
    int mBarrier;
    int slicesNow;
    int mp;
    int slipRate;

    bufferAddr = (int *)(g_BattleSlipBufferIndex * 0x28 + (char *)&g_BattleSlipSliceBufferA);
    for (partyIndex = 0; partyIndex < 3; partyIndex++) {
        slipRate = g_BattleState.actors[partyIndex].slipRate;
        hp = g_BattleState.actors[partyIndex].hp;
        mp = (*(unsigned short *)&g_BattleState.actors[partyIndex].mp);
        barrierGauge = g_BattleState.actors[partyIndex].gaugeRate;
        mBarrier = g_BattleState.actors[partyIndex].gaugeUnit;
        slicesNow = bufferAddr[partyIndex];
        hp += slipRate * slicesNow >> 0xF;
        if (hp < 0) {
            hp = 0;
        } else if ((unsigned int)hp > (unsigned int)g_aCombatants[partyIndex].maxHp) {
            hp = g_aCombatants[partyIndex].maxHp;
        }
        barrierGauge -= slicesNow >> 0xD;
        mBarrier -= slicesNow >> 0xD;
        if (barrierGauge < 0) {
            barrierGauge = 0;
        }
        if (mBarrier < 0) {
            mBarrier = 0;
        }
        if ((gBattleAiGlobals.arenaFlags & BATTLE_ARENA_BREAK_ACTIVE) == 0) {
            g_MenuPartyStats[partyIndex].hp = hp;
            g_MenuPartyStats[partyIndex].mp = mp;
        }
        g_MenuPartyStats[partyIndex].barrierGauge = barrierGauge << 1;
        g_MenuPartyStats[partyIndex].mBarrierGauge = mBarrier << 1;
    }
}
} /* extern "C" */
