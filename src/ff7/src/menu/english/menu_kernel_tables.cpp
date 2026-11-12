// src/ff7/src/menu/english/menu_kernel_tables.cpp
// Data-only object: .bss 0x00db9580..0x00dc0e78. No code.
// A data-only member of the menu library, which its neighbour menu_quit_confirm_dialog.cpp never references:
// the token state, the kernel's command, attack, item, weapon, armour, accessory and materia tables, the menu
// party stats and the savemap.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/menu.h"
#include "lib/pfile.h"
#include "lib/enums/win32.h"
#include "lib/codegen.h"
#include "lib/types/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "ff7/api/main.h"
#include "lib/api/file.h"
#include "lib/api/sound.h"
#include "lib/crt.h"
#include "lib/enums/file.h"
#include "lib/enums/graphics.h"
#include "lib/enums/sound.h"
#include "lib/types/polygon.h"
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"
#include "lib/types/mem.h"
#include "ff7/enums/main.h"
#include "ff7/enums/menu.h"
#include "ff7/savemap.h"
#include "ff7/types/battle_actors.h"
#include "ff7/field.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00db9580
int g_TokenState = 0;
unsigned char g_bssPad_00db9584[4] = {0};
// GLOBAL: FF7 0x00db9588
short g_FieldRequestedBattleId = 0;
// GLOBAL: FF7 0x00db958c
unsigned char *g_pTextSpacing = 0;
// GLOBAL: FF7 0x00db9590
struct KernelCommand g_KernelCommandTable[32] = {0};
// GLOBAL: FF7 0x00db9690
struct BattleAttackEntry g_BattleAttackTable[128] = {0};
unsigned char g_bssPad_00dba490[8] = {0};
// GLOBAL: FF7 0x00dba498
struct MenuPartyStats g_MenuPartyStats[3] = {0};
unsigned char g_bssPad_00dbb158[6528] = {0};
// The party's battle modifiers (PartyBattleModifiers, include/ff7/types/menu.h).
// GLOBAL: FF7 0x00dbcad8
PartyBattleModifiers g_PartyBattleModifiers = {0};
unsigned char g_bssPad_00dbcadc[4] = {0};
// GLOBAL: FF7 0x00dbcae0
struct KernelAccessory g_KernelAccessoryDataArray[32] = {0};
// GLOBAL: FF7 0x00dbcce0
struct KernelArmor g_KernelArmorDataArray[32] = {0};
// GLOBAL: FF7 0x00dbd160
struct BattleItemAction g_MenuKernelItemBuffer[128] = {0};
// GLOBAL: FF7 0x00dbdf60
struct KernelMateria g_KernelMateriaDataArray[96] = {0};
unsigned char g_bssPad_00dbe6e0[80] = {0};
// GLOBAL: FF7 0x00dbe730
struct KernelWeapon g_KernelWeaponDataArray[128] = {0};
// GLOBAL: FF7 0x00dbfd30
int g_ScriptedInputActive = 0;
unsigned char g_bssPad_00dbfd34[4] = {0};
// GLOBAL: FF7 0x00dbfd38
struct Savemap gSavemap = {0};
unsigned char g_bssPad_00dc0e2c[4] = {0};
unsigned char g_bssPad_00dc0e30[60] = {0};
// GLOBAL: FF7 0x00dc0e6c
unsigned char g_BattleIsPaused = 0;
// The pause latch: Start toggles it in battle, and the save write and the load restore set it around
// their work; while it is set Savemap_ClockTick does not advance the play time and the battle menus
// take no input.
// GLOBAL: FF7 0x00dc0e70
unsigned char g_GamePauseLatch = 0;
// GLOBAL: FF7 0x00dc0e74
int g_MenuField = 0;
// clang-format on

} /* extern "C" */
