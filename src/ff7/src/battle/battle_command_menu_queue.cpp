// src/ff7/src/battle/battle_command_menu_queue.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Command menu queue and command availability for battle actors: it saves base commands
//   under the Frog status, queues menus for controlled actors, tracks limit break readiness and
//   refreshes spell and command availability.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/enums/battle.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "lib/codegen.h"
#include "ff7/battle.h"
#include "lib/crt.h"
#include "ff7/types/menu.h"
#include "lib/api/token.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x007c2ad0
unsigned char g_BattleCommandMenuActorQueue[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// GLOBAL: FF7 0x009aeab8
BattleActorBaseCommand g_BattleFrogCommands = {0};
unsigned char g_bssPad_009aeac0[16] = {0};
// GLOBAL: FF7 0x009aead0
BattleActorBaseCommand g_BattleActorBaseCommands = {0};
#define g_BattleFrogCommandSlots (&g_BattleFrogCommands)
#define g_BattleActorBaseCommandSlots (&g_BattleActorBaseCommands)
unsigned char g_bssPad_009aead8[12] = {0};
// clang-format on


/*
 * Saves the actor's current base command from the party command table into
 * g_BattleActorBaseCommands and prepares its frog substitute in g_BattleFrogCommands.
 */
// FUNCTION: FF7 0x00437370
void __cdecl Battle_BuildFrogCommandSubstitute(int actorIdx)
{
    g_BattleActorBaseCommandSlots[actorIdx] = *(BattleActorBaseCommand *)g_MenuPartyStats[actorIdx].commands;
    g_BattleFrogCommandSlots[actorIdx] = g_BattleActorBaseCommandSlots[actorIdx];
    g_BattleFrogCommandSlots[actorIdx].commandId = 1;
    g_BattleFrogCommandSlots[actorIdx].subCommand =
        (g_BattleState.actors[actorIdx].timingFlags & 2) ? 0 : 7;
    g_BattleFrogCommandSlots[actorIdx].targetFlags =
        (*(char *)&g_BattleState.weaponActionRows[actorIdx].targetFlags);
}

/*
 * Returns the base command ID for the specified actor from g_BattleActorBaseCommands.
 */
// FUNCTION: FF7 0x0043740b
int __cdecl Battle_GetActorBaseCommandId(int actorIdx)
{
    return g_BattleActorBaseCommandSlots[actorIdx].commandId;
}

/*
 * Appends the specified actor to the command menu queue in g_BattleCommandMenuActorQueue
 * if not already present and the queue is enabled.
 */
// FUNCTION: FF7 0x00437420
void __cdecl Battle_RequestCommandMenu(int actorIdx)
{
    int i;

    if (gBattleAiGlobals.flags == 0) {
        for (i = 0; (unsigned int)i < (unsigned int)0x10; i++) {
            if (g_BattleCommandMenuActorQueue[i] == actorIdx) {
                return;
            }
            if (g_BattleCommandMenuActorQueue[i] == 0xFF) {
                g_BattleCommandMenuActorQueue[i] = actorIdx;
                return;
            }
        }
    }
}

/*
 * Removes the specified actor from g_BattleCommandMenuActorQueue and shifts remaining
 * entries forward. Returns the actor if found, or -1 if absent.
 */
// FUNCTION: FF7 0x00437481
int __cdecl Battle_ReleaseCommandMenu(int actorIdx)
{
    int result;
    int queueIndex;
    int shiftIndex;

    result = 0xFFFFFFFF;
    for (queueIndex = 0; (unsigned int)queueIndex < (unsigned int)0x10; queueIndex++) {
        if (g_BattleCommandMenuActorQueue[queueIndex] == actorIdx) {
            for (shiftIndex = queueIndex; (unsigned int)shiftIndex < (unsigned int)0xF; shiftIndex++) {
                g_BattleCommandMenuActorQueue[shiftIndex] = g_BattleCommandMenuActorQueue[shiftIndex + 1];
                if (g_BattleCommandMenuActorQueue[shiftIndex] == 0xFF) {
                    break;
                }
            }
            result = actorIdx;
            break;
        }
    }
    return result;
}

/*
 * Returns the controller-mapped actor currently at the head of the command menu
 * queue, or -1 if the queue is empty.
 */
// FUNCTION: FF7 0x00437506
short __cdecl Battle_GetActiveMenuActor(void)
{
    short val;

    val = 0xFFFF;
    if (g_BattleCommandMenuActorQueue[0] != 0xFF) {
        val = g_BattleCommandMenuActorQueue[0];
        val = Battle_MapPartySlotToController(val);
    }
    return (short)val;
}

/*
 * Maps a party slot (0..2) to its controlling actor if the slot is charmed/controlled
 * and the controlling actor is valid and active.
 */
// FUNCTION: FF7 0x00437543
int __cdecl Battle_MapPartySlotToController(int partySlot)
{
    int val;

    if (partySlot < 3) {
        val = g_BattleState.partyStats[partySlot].manipulatedSlot;
        if (val >= 4 && (g_BattleState.actorMenuSuppressMask & 1 << val) != 0) {
            partySlot = val;
        }
    }
    return partySlot;
}

/*
 * Re-evaluates and refreshes the command menu for the actor currently at the head
 * of the command menu queue.
 */
// FUNCTION: FF7 0x0043758a
void __cdecl Battle_ReopenQueuedCommandMenu(void)
{
    unsigned char val;

    val = g_BattleCommandMenuActorQueue[0];
    if (val != 0xFF) {
        Battle_ReleaseCommandMenu(val);
        Battle_RequestCommandMenu(val);
    }
}

/*
 * Marks limit break readiness for the specified party actor, setting the limit
 * ready bitmask and actor state flags.
 */
// FUNCTION: FF7 0x004375ce
void __cdecl Battle_SetLimitReady(int actorIdx)
{
    if ((unsigned char)g_BattleState.partyStats[actorIdx].limitLevelIndex != 0xFF) {
        *(short *)&g_BattleScene.readyActorMask |= (short)(1 << actorIdx);
        *(short *)&g_BattleState.actors[actorIdx].limitFlags |= 1;
        g_BattleState.actors[actorIdx].dirtyFlags |= BATTLE_ACTOR_DIRTY_COMMANDS;
    }
}

/*
 * Clears limit break readiness for the specified actor.
 */
// FUNCTION: FF7 0x0043763a
void __cdecl Battle_ClearLimitReady(int actorIdx)
{
    g_BattleState.actors[actorIdx].limitFlags &= 0xFFFE;
    g_BattleState.actors[actorIdx].dirtyFlags |= BATTLE_ACTOR_DIRTY_COMMANDS;
}

/*
 * Refreshes the availability flags of one of the actor's menu lists (`selector`, a
 * BattleCommandMenuList) when the list's dirty bit is set.
 */
// FUNCTION: FF7 0x00437679
void __cdecl Battle_RefreshPendingCommandAvailability(int actorIdx, int selector)
{
    int val;

    if (actorIdx < 3 && (*(unsigned char *)&g_BattleState.actors[actorIdx].dirtyFlags & 1 << selector) != 0) {
        val = g_BattleScene.actors[actorIdx].savedStatus;
        g_BattleState.actors[actorIdx].dirtyFlags &= (char)~(1 << selector);
        switch (selector) {
        case BATTLE_MENU_LIST_COMMANDS:
            Battle_UpdateCommandAvailability(actorIdx, val);
            break;
        case BATTLE_MENU_LIST_MAGIC:
            g_BattleState.actors[actorIdx].dirtyFlags &= ~BATTLE_ACTOR_MAGIC_UNAVAILABLE;
            if (Battle_UpdateSpellMenuAvailability(actorIdx, val, 0, 56) != 0) {
                g_BattleState.actors[actorIdx].dirtyFlags |= BATTLE_ACTOR_MAGIC_UNAVAILABLE;
            }
            break;
        case BATTLE_MENU_LIST_SUMMON:
            g_BattleState.actors[actorIdx].dirtyFlags &= ~BATTLE_ACTOR_SUMMON_UNAVAILABLE;
            if (Battle_UpdateSpellMenuAvailability(actorIdx, val, 0x38, 16) != 0) {
                g_BattleState.actors[actorIdx].dirtyFlags |= BATTLE_ACTOR_SUMMON_UNAVAILABLE;
            }
            break;
        case BATTLE_MENU_LIST_ENEMY_SKILL:
            g_BattleState.actors[actorIdx].dirtyFlags &= ~BATTLE_ACTOR_ENEMY_SKILL_UNAVAILABLE;
            if (Battle_UpdateSpellMenuAvailability(actorIdx, val, 0x48, 24) != 0) {
                g_BattleState.actors[actorIdx].dirtyFlags |= BATTLE_ACTOR_ENEMY_SKILL_UNAVAILABLE;
            }
        }
    }
}

/*
 * Scans ability records in the specified range for actorIdx, updating MP cost,
 * usage counts, and availability flags based on current status and MP.
 * Returns 0 if any spell is available, or 1 if all are unavailable.
 */
// FUNCTION: FF7 0x00437814
int __cdecl Battle_UpdateSpellMenuAvailability(int actorIdx, int statusMask, int groupOffset, int count)
{
    int notBlocked;
    int abilityIndex;
    int res;
    int availableMp;
    BattleAbilitySlot *abilitySlotsPtr;
    int canSpeak;
    int menuFlags;
    int abilityId;
    int targetFlags;
    int fullId;
    int localNeedsAll;

    res = 1;
    availableMp = (*(unsigned short *)&g_BattleState.actors[actorIdx].mp);
    canSpeak = (statusMask & BATTLE_STATUS_SILENCE) == 0;
    notBlocked = (statusMask & BATTLE_STATUS_FROG) == 0;
    abilitySlotsPtr = &g_MenuPartyStats[actorIdx].abilities[groupOffset];
    for (abilityIndex = 0; abilityIndex < count; abilityIndex++) {
        abilityId = abilitySlotsPtr[abilityIndex].attackId;
        menuFlags = BATTLE_MENU_SLOT_GREYED;
        if (abilityId != 0xFF) {
            fullId = abilityId + groupOffset;
            targetFlags = g_BattleAttackTable[fullId].targetFlags;
            localNeedsAll = 0;
            if (fullId < 0x38) {
                if ((targetFlags & BTL_TARGETF_TOGGLE_MULTIPLE) != 0 && abilitySlotsPtr[abilityIndex].allCount == 0 && abilitySlotsPtr[abilityIndex].quadraCount == 0) {
                    targetFlags &= ~(BTL_TARGETF_DEFAULT_MULTIPLE | BTL_TARGETF_TOGGLE_MULTIPLE);
                }
                if ((targetFlags & BTL_TARGETF_TOGGLE_MULTIPLE) != 0) {
                    menuFlags |= BATTLE_MENU_SLOT_MULTI_ARROW;
                }
                if (abilitySlotsPtr[abilityIndex].quadraCount != 0) {
                    menuFlags |= BATTLE_MENU_SLOT_MULTI_ARROW;
                    targetFlags &= ~BTL_TARGETF_TOGGLE_MULTIPLE;
                    targetFlags |= BTL_TARGETF_DEFAULT_MULTIPLE;
                }
            } else if (fullId < 0x48 && abilitySlotsPtr[abilityIndex].allCount == 0) {
                localNeedsAll = 1;
            }
            abilitySlotsPtr[abilityIndex].targetFlags = targetFlags;
            if (availableMp >= abilitySlotsPtr[abilityIndex].mpCost && (localNeedsAll == 0 && (canSpeak != 0 && (notBlocked != 0 || fullId == 0xA)))) {
                menuFlags &= ~BATTLE_MENU_SLOT_GREYED;
                res = 0;
            }
        }
        abilitySlotsPtr[abilityIndex].menuFlags = menuFlags;
    }
    return res;
}

/*
 * Updates availability and metadata flags for the actor's top command and all 15
 * subsequent command entries based on limit readiness, frog status, and battle conditions.
 */
// FUNCTION: FF7 0x004379be
void __cdecl Battle_UpdateCommandAvailability(int actorIdx, int statusMask)
{
    int restricted;
    BattleCommandSlot *commandSlots;
    int entryIndex;
    int commandId;
    int menuFlags;

    commandSlots = g_MenuPartyStats[actorIdx].commands;
    restricted = (statusMask & BATTLE_STATUS_FROG) != 0;
    commandSlots->menuFlags = 0;
    if ((g_BattleScene.readyActorMask & (1 << actorIdx)) != 0 && restricted == 0 || (gBattleAiGlobals.arenaFlags & 8) != 0) {
        commandSlots->commandId = BATTLE_COMMAND_LIMIT;
        commandSlots->cursorAction = g_KernelCommandTable[BATTLE_COMMAND_LIMIT].cursorAction;
        commandSlots->targetFlags = *(char *)&g_KernelCommandTable[BATTLE_COMMAND_LIMIT].targetFlags;
        commandSlots->supportFlags = 0;
    } else if (commandSlots->allCount != 0 && restricted == 0) {
        commandSlots->commandId = *(char *)&g_BattleActorBaseCommandSlots[actorIdx].commandId;
        commandSlots->cursorAction = g_BattleActorBaseCommandSlots[actorIdx].subCommand;
        commandSlots->targetFlags = g_BattleActorBaseCommandSlots[actorIdx].targetFlags;
        commandSlots->supportFlags = g_BattleActorBaseCommandSlots[actorIdx].param;
    } else {
        commandSlots->commandId = g_BattleFrogCommandSlots[actorIdx].commandId;
        commandSlots->cursorAction = g_BattleFrogCommandSlots[actorIdx].subCommand;
        commandSlots->targetFlags = g_BattleFrogCommandSlots[actorIdx].targetFlags;
        commandSlots->supportFlags = g_BattleFrogCommandSlots[actorIdx].param;
    }
    for (entryIndex = 1; entryIndex < 16; entryIndex++) {
        commandId = commandSlots[entryIndex].commandId;
        if (commandId != 0xFF) {
            menuFlags = commandSlots[entryIndex].menuFlags;
            menuFlags &= ~(BATTLE_MENU_SLOT_GREYED | BATTLE_MENU_SLOT_MULTI_ARROW);
            switch (commandId) {
            case BATTLE_COMMAND_MAGIC:
            case BATTLE_COMMAND_W_MAGIC:
                Battle_RefreshPendingCommandAvailability(actorIdx, BATTLE_MENU_LIST_MAGIC);
                if (((*(unsigned char *)&g_BattleState.actors[actorIdx].dirtyFlags) & BATTLE_ACTOR_MAGIC_UNAVAILABLE) != 0) {
                    menuFlags |= BATTLE_MENU_SLOT_GREYED;
                }
                break;
            case BATTLE_COMMAND_SUMMON:
            case BATTLE_COMMAND_W_SUMMON:
                Battle_RefreshPendingCommandAvailability(actorIdx, BATTLE_MENU_LIST_SUMMON);
                if (((*(unsigned char *)&g_BattleState.actors[actorIdx].dirtyFlags) & BATTLE_ACTOR_SUMMON_UNAVAILABLE) != 0) {
                    menuFlags |= BATTLE_MENU_SLOT_GREYED;
                }
                break;
            case BATTLE_COMMAND_ENEMY_SKILL:
                Battle_RefreshPendingCommandAvailability(actorIdx, BATTLE_MENU_LIST_ENEMY_SKILL);
                if (((*(unsigned char *)&g_BattleState.actors[actorIdx].dirtyFlags) & BATTLE_ACTOR_ENEMY_SKILL_UNAVAILABLE) != 0) {
                    menuFlags |= BATTLE_MENU_SLOT_GREYED;
                }
                break;
            case BATTLE_COMMAND_ITEM:
            case BATTLE_COMMAND_W_ITEM:
                if ((*(unsigned char *)&g_BtlHandicapFlags & 0x20) != 0) {
                    menuFlags |= BATTLE_MENU_SLOT_GREYED;
                }
                break;
            case BATTLE_COMMAND_STEAL:
            case BATTLE_COMMAND_MUG:
                if (commandSlots[entryIndex].allCount == 0) {
                    commandSlots[entryIndex].cursorAction = BATTLE_CURSOR_ACTION_TARGET_VALID_MASK;
                }
            }
            if (restricted != 0) {
                switch (commandId) {
                case BATTLE_COMMAND_MAGIC:
                case BATTLE_COMMAND_ITEM:
                case BATTLE_COMMAND_W_MAGIC:
                case BATTLE_COMMAND_W_ITEM:
                    break;
                default:
                    menuFlags |= BATTLE_MENU_SLOT_GREYED;
                }
            }
            if ((commandSlots[entryIndex].targetFlags & BTL_TARGETF_TOGGLE_MULTIPLE) != 0 && commandSlots[entryIndex].allCount == 0) {
                *(char *)&commandSlots[entryIndex].targetFlags &= ~(BTL_TARGETF_DEFAULT_MULTIPLE | BTL_TARGETF_TOGGLE_MULTIPLE);
            }
            if ((commandSlots[entryIndex].targetFlags & (BTL_TARGETF_DEFAULT_MULTIPLE | BTL_TARGETF_TOGGLE_MULTIPLE)) == (BTL_TARGETF_DEFAULT_MULTIPLE | BTL_TARGETF_TOGGLE_MULTIPLE)) {
                menuFlags |= BATTLE_MENU_SLOT_MULTI_ARROW;
            }
            commandSlots[entryIndex].menuFlags = menuFlags;
        }
    }
}

/*
 * Initializes an 8-byte ability table record for the specified actor and ability index.
 */
// FUNCTION: FF7 0x00437d4e
void __cdecl Battle_InitActorAbilityRecord(int actorIdx, int abilityIndex)
{
    BattleAbilitySlot *rec;

    rec = &g_MenuPartyStats[actorIdx].abilities[abilityIndex];
    rec->menuFlags = BATTLE_MENU_SLOT_GREYED;
    rec->targetFlags = *(char *)&g_BattleAttackTable[abilityIndex].targetFlags;
    rec->mpCost = *(char *)&g_BattleAttackTable[abilityIndex].mpCost;
    rec->attackId = abilityIndex - 0x48;
}
} /* extern "C" */
