// src/ff7/src/battle/battle_globals.cpp
// Contents: the battle module's file and scene buffer pointers, the party value/stat words and the
//   global camera script pointer: a data-only member of the battle library, which nothing beside it
//   in the image uses (battle.cpp, init.cpp, coloss.cpp and the camera scripts read them by name).
// Original: unknown (no retail __FILE__ path names this TU)
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/movie.h"
extern "C" {
// GLOBAL: FF7 0x009a13a8
int g_Battle_fileBuffer = 0;
// GLOBAL: FF7 0x009a13ac
int g_Battle_sceneBuffer = 0;
unsigned char g_bssPad_009a13b0[4] = {0};
// GLOBAL: FF7 0x009a13b4
int g_Battle_partyVal = 0;
// GLOBAL: FF7 0x009a13b8
int g_Battle_partyStat = 0;
// GLOBAL: FF7 0x009a13bc
int g_BattleCameraGlobalScripts = 0;
unsigned char g_bssPad_009a13c0[8] = {0};
} /* extern "C" */
