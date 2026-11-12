// src/ff7/src/battle/battle_scene_misc_helpers.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Small battle scene helpers: an interruptibility check, awarding AP to equipped materia,
//   the UI-ready yield for the attack microprogram, idle pose get and set, and an unused no-op
//   hook.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "lib/codegen.h"
#include "ff7/battle.h"
#include "lib/crt.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
extern "C" {


/* Unprototyped retail call signature (parameter/return width differs from callee definition). */
extern void __cdecl Savemap_AwardApToEquippedMateria(short partySlot, short apAmount);

/*
 * Returns 0 if g_BattleSceneState is 4 or 5 while bit 0 of g_BattleSceneFlags is clear and
 * g_BattleScriptEffectFlag is 0; otherwise returns 1.
 */
// FUNCTION: FF7 0x004372b0
int __cdecl Battle_IsSceneInterruptible(void)
{
    int isInterruptible;

    isInterruptible = 1;
    if (g_BattleSceneState == 4 || g_BattleSceneState == 5) {
        if ((g_BattleSceneFlags & 1) == 0 && g_BattleScriptEffectFlag == 0) {
            isInterruptible = 0;
        }
    }
    return isInterruptible;
}

/*
 * Awards AP to materia equipped in the specified party member slot and equipment slot.
 */
// FUNCTION: FF7 0x004372fc
void __cdecl Battle_AwardApToEquippedMateria(int partySlot, int equipSlot)
{
    Savemap_AwardApToEquippedMateria((short)partySlot, (short)equipSlot);
}

/*
 * Signals the action microprogram to continue unless bit 1 of g_BattleSceneFlags is set.
 */
// FUNCTION: FF7 0x00437313
void __cdecl Battle_AttackOpYieldUntilUiReady(void)
{
    if ((g_BattleSceneFlags & 2) == 0) {
        g_BattleAttackOpContinue = 1;
    }
}

/*
 * Gets or sets the idle pose index for the specified actor. If pose is not -1,
 * updates the actor's idle pose before returning it.
 */
// FUNCTION: FF7 0x00437331
int __cdecl Battle_GetSetActorIdlePose(int actorIdx, int pose)
{
    if (pose != -1) {
        g_BattleActorRecords[actorIdx].targetBone = pose;
    }
    return g_BattleActorRecords[actorIdx].targetBone;
}

/*
 * Unused empty hook.
 */
// FUNCTION: FF7 0x0043735e
void __cdecl Battle_UnusedNoOp_1(void)
{
}
} /* extern "C" */
