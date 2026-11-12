// src/ff7/src/menu/btlmenu/english/battle_menu_return_state.cpp
// Data-only object: .data 0x0091ef90..0x0091ef9e. No code.
// A data-only member of the menu library, which its neighbour battle_target_selection.cpp never references:
// the battle menu's return state, window frame id and menu state.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/enums/battle.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include <stdlib.h>
#include "ff7/types/menu.h"
#include "ff7/api/menu.h"
#include "lib/api/file.h"
#include "lib/api/sound.h"
#include "ff7/api/battle.h"
#include "ff7/types/battle.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x0091ef90
short g_BattleMenuReturnState = -1;
unsigned char g_rawData_0091ef94[4] = {0xff, 0xff, 0x00, 0x00};
// The battle menu writes 0x18 here beside g_BattleMenuState; the two bytes after it are the
// alignment padding before g_BattleMenuState.
// GLOBAL: FF7 0x0091ef98
short g_BattleWindowFrameId = -1;
// A signed word; zero picks the fixed placement over the pointer-driven one.
// GLOBAL: FF7 0x0091ef9c
short g_BattleMenuState = -1;
// clang-format on

} /* extern "C" */
