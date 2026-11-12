// src/ff7/field/src/field_encounter_state.cpp
// Data-only object: .bss 0x00cc1658..0x00cc1670. No code. The unread word 0x00CC166C before
// field_map_script_state.cpp's 8-aligned .bss is this member's fill.
// A data-only member of the field library, which its neighbour field_map_script_state.cpp never references:
// the field's random-encounter trigger, step counter, rate accumulator and random indexes.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "lib/codegen.h"
#include "ff7/enums/main.h"
#include "ff7/enums/field.h"
#include "ff7/api/field.h"
#include "ff7/api/main.h"
#include "ff7/field.h"
#include "ff7/savemap.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include <stdlib.h>
#include <string.h>
#include "lib/enums/file.h"
#include "ff7/types/condor.h"
#include "ff7/enums/condor.h"
#include "lib/types/graphics.h"
#include "ff7/enums/battle.h"
#include "ff7/types/menu.h"
#include "lib/enums/sound.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00cc1658
unsigned char g_FieldEncounterTriggered = 0;
// GLOBAL: FF7 0x00cc165c
char g_FieldRandomIndex1 = 0;
// GLOBAL: FF7 0x00cc1660
char g_FieldRandomIndex2 = 0;
// GLOBAL: FF7 0x00cc1664
unsigned char g_FieldEncounterStepCounter = 0;
// GLOBAL: FF7 0x00cc1668
short g_FieldEncounterRateAccum = 0;
unsigned char g_bssPad_00cc166c[4] = {0};
// clang-format on

} /* extern "C" */
