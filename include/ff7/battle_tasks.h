/* include/ff7/battle_tasks.h: the battle task slots, spelled once.
 * Macros only; the TU declares g_BattleTaskSlotBuffer, Battle_AllocateResourceSlot and
 * g_BattleActiveTaskSlotIdx as before. */
#ifndef FF7_BATTLE_TASKS_H
#define FF7_BATTLE_TASKS_H

#include "ff7/enums/battle.h"

/* The slot of a new task that runs fn: Battle_AllocateResourceSlot claims one of the main slots of
 * g_BattleTaskSlotBuffer and returns its index. */
#define BATTLE_NEW_TASK_SLOT(fn) (&g_BattleTaskSlotBuffer.mainSlots[(short)Battle_AllocateResourceSlot(fn)])

/* The slot of the task that runs now (the task loop sets g_BattleActiveTaskSlotIdx). */
#define BATTLE_CURRENT_TASK_SLOT (&g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx])

#endif
