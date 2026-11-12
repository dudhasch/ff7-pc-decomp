// src/ff7/src/battle/battle_ai_script_dispatch.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle AI dispatch: latches AI event bits, finds the enemy AI script for an actor, and
//   runs the formation AI events. Also records enemy deaths and holds the party AI override.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"
#include "ff7/battle.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

/*
 * Latches the requested AI event bit and finds an enemy script for actors four or above or a
 * remapped party script for actors below three; actor three has no script lookup. When found,
 * snapshots three bytes from all ten combatants into explicit packed storage, prepares globals,
 * runs the AI VM, and compares two of those bytes afterward. Changed animStep bytes enqueue
 * sequence code four with flag 16, and changed animId bytes retarget animation pose using old
 * and new values. The third snapshot byte is not compared.
 */
// FUNCTION: FF7 0x005c86e0
void __cdecl Battle_RunAiScriptEvent(int actor, int event, int eventParam)
{

#pragma pack(push, 1)
    typedef struct BattleSavedAnimState {
        unsigned char animStep;
        unsigned char scriptId;
        unsigned char animId;
        unsigned short reserved;
    } BattleSavedAnimState;

    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        BattleSavedAnimState savedAnim[10];
        struct { unsigned char pad_to_34[0x34]; int combatantIndex; };
        struct { unsigned char pad_to_38[0x38]; int aiScript; };
        struct { unsigned char pad_to_3C[0x3C]; int scriptIndex; };
    } frame;
#pragma pack(pop)

    frame.aiScript = 0;
    frame.scriptIndex = 0xFFFFFFFF;
    g_BattleState.pendingAiEventMask |= (short)(1 << event);
    if (actor >= 4) {
        frame.aiScript = Battle_FindAiScript(&g_BattleState.sceneRecordBlockE[0x200], g_BattleScene.enemies[actor - 4].enemyIndex, event);
    } else if (actor < 3) {
        frame.scriptIndex = g_BattleScene.actors[actor].unitIndex;
        if (frame.scriptIndex != -1 && g_BattlePartyAiEntityOverrides[frame.scriptIndex] != 0xFF) {
            frame.scriptIndex = g_BattlePartyAiEntityOverrides[frame.scriptIndex];
        }
        frame.aiScript = Battle_FindAiScript(g_BattleKernelData.aiScripts, frame.scriptIndex, event);
    }
    if (frame.aiScript != 0) {
        for (frame.combatantIndex = 0; frame.combatantIndex < 0xA; frame.combatantIndex++) {
            frame.savedAnim[frame.combatantIndex].animStep = g_aCombatants[frame.combatantIndex].animStep;
            frame.savedAnim[frame.combatantIndex].scriptId = g_aCombatants[frame.combatantIndex].scriptId;
            frame.savedAnim[frame.combatantIndex].animId = g_aCombatants[frame.combatantIndex].animId;
        }
        Battle_AiVm_PrepareGlobals(actor, eventParam, 0);
        Battle_AiVm_Run(actor, (unsigned char *)frame.aiScript, frame.scriptIndex);
        for (frame.combatantIndex = 0; frame.combatantIndex < 0xA; frame.combatantIndex++) {
            if (frame.savedAnim[frame.combatantIndex].animStep != g_aCombatants[frame.combatantIndex].animStep) {
                Battle_PushSequenceStep(frame.combatantIndex, 4, g_aCombatants[frame.combatantIndex].animStep, 0x10);
            }
            if (frame.savedAnim[frame.combatantIndex].animId != g_aCombatants[frame.combatantIndex].animId) {
                Battle_RetargetActorAnimPose(frame.combatantIndex, frame.savedAnim[frame.combatantIndex].animId, g_aCombatants[frame.combatantIndex].animId, 0);
            }
        }
    }
}

/*
 * For index other than minus one resolves two unsigned16 offset tables, treating 0xffff as absent.
 * First offset rounded down even before second lookup; returns base plus second offset or zero.
 */
// FUNCTION: FF7 0x005c88c7
int __cdecl Battle_FindAiScript(unsigned char *tableBase, int index, int subIndex)
{
    int result;
    int base;

    result = 0;
    if (index != -1) {
        index = ((unsigned short *)tableBase)[index];
        if (index != 0xFFFF) {
            tableBase += index / 2 * 2;
            index = ((unsigned short *)tableBase)[subIndex];
            if (index != 0xFFFF) {
                base = (int)tableBase;
                result = base + index;
            }
        }
    }
    return result;
}

/*
 * Prepares AI globals then consumes eight formation-event bits before resolving/running each
 * script using formation selector modulo four. Missing scripts still consume bits; callbacks may
 * alter remaining flags.
 */
// FUNCTION: FF7 0x005c8931
void __cdecl Battle_RunFormationAiEvents(void)
{
    struct {
        int script;
        int eventBitIndex;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.script = 0;
    Battle_AiVm_PrepareGlobals(-1, 0, 0);
    for (frame.eventBitIndex = 0; frame.eventBitIndex < 8; frame.eventBitIndex++) {
        if ((g_BattleState.pendingAiEventMask & 1 << frame.eventBitIndex) != 0) {
            g_BattleState.pendingAiEventMask &= ~(1 << frame.eventBitIndex);
            frame.script = Battle_FindAiScript(g_BattleState.sceneRecordBlockE, *(unsigned short *)&gBattleAiGlobals.formationId % 4, frame.eventBitIndex);
            if (frame.script != 0) {
                Battle_AiVm_Run(3, (unsigned char *)frame.script, -1);
            }
        }
    }
}

/*
 * Marks first enemy death and increments killer party byte once. On first death-processing flag,
 * records attacker mask, optionally rolls reactions and runs AI death event. Changed event byte or
 * forced forced may queue action 0x25 unless other flag; terminal flag also requests hold cleanup
 * only when forced zero.
 */
// FUNCTION: FF7 0x005c89e2
void __cdecl Battle_HandleActorDeath(int attacker, int victim, int forced)
{
    unsigned char prevEvent;
    int needsCleanup;
    int attackerMask;

    /* Unprototyped retail call signature (parameter width differs from callee definition). */
    extern void __cdecl Battle_AiVm_PostAction(short, int, int);

    needsCleanup = 0;
    if (victim >= 4 && (g_BattleState.actors[victim].timingFlags & 0x20) == 0) {
        g_BattleState.actors[victim].timingFlags |= 0x20;
        if (attacker < 3) {
            g_BattleState.partyStats[attacker].killCount++;
        }
    }
    if ((g_aCombatants[victim].flags & BATTLE_COMBATANT_DEATH_HANDLED) == 0) {
        prevEvent = g_BattleState.actionEnqueueSequence[0];
        g_aCombatants[victim].flags |= BATTLE_COMBATANT_DEATH_HANDLED;
        if (attacker >= 4) {
            Battle_RollReactionTriggers(victim, 0);
        }
        if (attacker == victim) {
            attackerMask = 0;
        } else {
            attackerMask = 1 << attacker;
        }
        g_aCombatants[victim].attackerMask = attackerMask;
        Battle_RunAiScriptEvent(victim, 3, 0);
        if (g_BattleState.actionEnqueueSequence[0] != prevEvent || forced != 0) {
            if ((g_aCombatants[victim].flags & BATTLE_COMBATANT_IMMUNE_TO_DEATH) == 0) {
                gBattleAiGlobals.lastActorMask = 1 << victim;
                Battle_AiVm_PostAction((short)victim, 0x25, 0);
            }
            needsCleanup = 1;
        }
    }
    if ((g_aCombatants[victim].flags & BATTLE_COMBATANT_IMMUNE_TO_DEATH) != 0) {
        needsCleanup = 1;
    }
    if (needsCleanup != 0 && forced == 0) {
        Battle_ClearActorAnimHoldFlags(victim);
    }
}

/*
 * One-line setter: stores the byte override into g_BattlePartyAiEntityOverrides at partyIndex.
 */
// FUNCTION: FF7 0x005c8b65
void __cdecl Battle_SetPartyAiEntityOverride(int partyIndex, int override)
{
    g_BattlePartyAiEntityOverrides[partyIndex] = override;
}
// clang-format off
// GLOBAL: FF7 0x008fee38
unsigned char g_BattlePartyAiEntityOverrides[4] = {0xff, 0xff, 0xff, 0xff};
unsigned char g_rawData_008fee3c[4] = {0xff, 0xff, 0xff, 0xff};
unsigned char g_rawData_008fee40[8] = {0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00};
} /* extern "C" */
