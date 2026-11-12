// src/ff7/chocobo/chocobo_race_state.cpp
// Data-only object: .bss 0x00e71130..0x00e71628. No code.
// A data-only member of the chocobo library, which its neighbour ch_app.cpp never references: the chocobo
// race class and winner, the six racers and the selected one, the camera matrix, timers and input latches.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/main.h"
#include "lib/codegen.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "lib/api/sound.h"
#include "ff7/api/chocobo.h"
#include "lib/api/graphics.h"
#include "ff7/types/chocobo.h"
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include "ff7/enums/main.h"
#include "lib/pfile.h"
#include "ff7/gte.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00e71130
int g_ChocoboRaceClass = 0;
// GLOBAL: FF7 0x00e71134
int g_ChocoboRaceWinnerIndex = 0;
// GLOBAL: FF7 0x00e71138
unsigned char g_ChocoboCameraLookAtMatrix[30] = {0};
// GLOBAL: FF7 0x00e71158
struct ChocoboRacer g_ChocoboRacers[6] = {0};
// GLOBAL: FF7 0x00e71530
struct ChocoboRacer g_ChocoboSelectedRacerState = {0};
// GLOBAL: FF7 0x00e715d4
int g_ChocoboFadeStep = 0;
// GLOBAL: FF7 0x00e715d8
int g_ChocoboStartTimer = 0;
// GLOBAL: FF7 0x00e715dc
int g_ChocoboSelectedRacerIndex = 0;
unsigned char g_bssPad_00e715e0[28] = {0};
// GLOBAL: FF7 0x00e715fc
struct ChocoboCourseWaypoint *g_pChocoboCourseWaypoints = 0;
// GLOBAL: FF7 0x00e71600
int g_ChocoboLateGamePrizesEnabled = 0;
unsigned char g_bssPad_00e71604[4] = {0};
unsigned char g_bssPad_00e71608[24] = {0};
// GLOBAL: FF7 0x00e71620
unsigned int g_ChocoboPackedInputLatches = 0;
// GLOBAL: FF7 0x00e71624
int g_ChocoboViewedRacer = 0;
// clang-format on

} /* extern "C" */
