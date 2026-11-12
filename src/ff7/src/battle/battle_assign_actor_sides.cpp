// src/ff7/src/battle/battle_assign_actor_sides.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Assigns each combatant its side and targeting masks from the formation arrangement.

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

// clang-format off
// GLOBAL: FF7 0x007b76e8
extern const unsigned char g_BattleFormationMessageIds[6] = {0xff, 0x32, 0x33, 0x34, 0x35, 0xff};
// clang-format on

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Assigns combatant side and targeting masks according to the formation arrangement,
 * handling normal, pre-emptive, back-attack, and pincer battle layouts.
 */
// FUNCTION: FF7 0x005d0ca0
void __cdecl Battle_AssignActorSideMasks(void)
{
    int partyMask;
    int message;
    int frontMask;
    unsigned short mirrorMask;
    unsigned short maskTable[3];
    int foeMask;
    int activeBackRowFlag;
    int groupIndex;
    int index;
    foeMask = *(unsigned short *)&gBattleAiGlobals.enemyMask;
    partyMask = gBattleAiGlobals.targetMask;
    frontMask = 5;
    if (g_BattleScene.layout == BATTLE_LAYOUT_SIDE_ATTACK_4)
        frontMask ^= -1;
    message = g_BattleFormationMessageIds[g_BattleState.layoutType];
    if (message != 0xff && *(unsigned short *)&gBattleAiGlobals.formationId != 0x3d6)
        Battle_PostBattleMessage(message);
    mirrorMask = 0;
    maskTable[0] = 0;
    maskTable[1] = 0;
    maskTable[2] = 0;
    switch (g_BattleState.layoutType) {
    case BATTLE_LAYOUT_NORMAL:
        mirrorMask = foeMask;
    case BATTLE_LAYOUT_PREEMPTIVE:
        maskTable[0] = partyMask;
        maskTable[1] = foeMask;
        break;
    case BATTLE_LAYOUT_BACK_ATTACK:
        mirrorMask = partyMask;
        maskTable[0] = foeMask;
        maskTable[1] = partyMask;
        break;
    case BATTLE_LAYOUT_PINCER_ATTACK:
        maskTable[1] = partyMask;
        for (index = 0; index < 6; index++) {
            if ((foeMask & (1 << (index + 4))) != 0) {
                groupIndex = (g_aCombatants[index + 4].flags & BATTLE_COMBATANT_SECOND_GROUP) != 0 ? 2 : 0;
                maskTable[groupIndex] |= 1 << (index + 4);
            }
        }
        mirrorMask = (partyMask & 2) | maskTable[2];
        if (*(unsigned short *)&gBattleAiGlobals.formationId == 0x3d6)
            mirrorMask &= ~partyMask;
        break;
    default:
        maskTable[0] = partyMask & frontMask;
        maskTable[1] = foeMask;
        maskTable[2] = partyMask & ~frontMask;
        mirrorMask = maskTable[2];
        for (index = 0; index < 6; index++) {
            if ((foeMask & (1 << (index + 4))) != 0 && g_BattleScene.enemies[index].posZ >= 0) {
                mirrorMask |= 1 << (index + 4);
            }
        }
        break;
    }
    for (index = 0; index < 10; index++) {
        g_aCombatants[index].flags &= ~(BATTLE_COMBATANT_SECOND_GROUP | BATTLE_COMBATANT_FACING_FLIPPED);
        if ((maskTable[2] & (1 << index)) != 0)
            g_aCombatants[index].flags |= BATTLE_COMBATANT_SECOND_GROUP;
        if ((mirrorMask & (1 << index)) != 0)
            g_aCombatants[index].flags |= BATTLE_COMBATANT_FACING_FLIPPED;
    }
    for (index = 0; index < 3; index++) {
        activeBackRowFlag = (g_aCombatants[index].flags & BATTLE_COMBATANT_BACK_ROW) != 0 ? 1 : 0;
        switch (g_BattleState.layoutType) {
        case BATTLE_LAYOUT_NORMAL:
        case BATTLE_LAYOUT_PREEMPTIVE:
            break;
        case BATTLE_LAYOUT_BACK_ATTACK:
            activeBackRowFlag = activeBackRowFlag == 0;
            g_aCombatants[index].flags ^= BATTLE_COMBATANT_BACK_ROW;
            break;
        default:
            activeBackRowFlag = 0;
            g_aCombatants[index].flags &= ~BATTLE_COMBATANT_BACK_ROW;
            break;
        }
        g_BattleScene.actors[index].flags = activeBackRowFlag;
    }
    g_BattleScene.targetMaskTable[0] = maskTable[0];
    g_BattleScene.targetMaskTable[1] = maskTable[1];
    g_BattleScene.targetMaskTable[2] = maskTable[2];
}
} /* extern "C" */
