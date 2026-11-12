// src/ff7/src/battle/battle_fx_special_freeze_breath_effect.cpp
/* Enemy Special Attack [95]: Freeze Breath (orig. SPECIAL/REITO, 冷凍) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Reito': a per-target timeline that sorts draw order, with
//   scaled, scattered and orbiting sprites, staged face-offset models, spinning emitters and a
//   moving model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxFreezeBreathDrawPrim;
struct BattleFxFreezeBreathDrawRecord;
struct GfxPsxSpriteQuadState;
struct BattleFxModelDraw;
// clang-format off

// GLOBAL: FF7 0x0089fd58
unsigned int g_BattleSpecFreezeBreathResourceDefs[76] = {0x001900b1, (unsigned int)"GAN1.RSD", 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)"KO1.RSD", 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)"KO2.RSD", 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)"KO3.RSD", 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)"KO4.RSD", 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)"KO5.RSD", 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)"KO6.RSD", 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)"KO7.RSD", 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)"KO8.RSD", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"KEMU.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"BAKU1.TIM", (unsigned int)"BAKU1.SPT", 0x00000000, 0x00000000, 0x00192a39, 0x00000000, (unsigned int)"JO_1.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"HASSYA.TIM", (unsigned int)"HASSYA.SPT", 0x00000000, 0x00000000, 0x00197c3c, (unsigned int)"KO-2.TIM", 0x00000000, 0x00000000, 0x00000000, 0x00197c39, (unsigned int)"KO-2.TIM", 0x00000000, 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x0089fe88
int g_BattleSpecFreezeBreathWorkingTemplate = 336;
// GLOBAL: FF7 0x0089fe8c
int g_BattleSpecFreezeBreathTemplateField = (int)0xFE300000;
unsigned char g_rawData_0089fe90[1632] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x91, 0xff, 0x45, 0xff, 0x00, 0x00, 0xac, 0x00, 0x3e, 0xff,
0xbb, 0xff, 0x00, 0x00, 0x67, 0x00, 0x2a, 0xff, 0x95, 0x00, 0x00, 0x00, 0x7d, 0xff, 0x4f, 0xff,
0x7d, 0x00, 0x00, 0x00, 0x6a, 0xff, 0x91, 0xff, 0xdb, 0xff, 0x00, 0x00, 0x83, 0x00, 0x6f, 0x00,
0x4c, 0xff, 0x00, 0x00, 0xac, 0x00, 0x6f, 0x00, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6f, 0x00,
0xdf, 0x00, 0x00, 0x00, 0x2c, 0xff, 0x6f, 0x00, 0x45, 0x00, 0x00, 0x00, 0x7d, 0xff, 0x6f, 0x00,
0x4c, 0xff, 0x00, 0x00, 0x29, 0x00, 0xa7, 0x01, 0x00, 0x00, 0x00, 0x00, 0xa1, 0xff, 0x0d, 0x01,
0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x01, 0x49, 0x00, 0x00, 0x00, 0x65, 0x00, 0x14, 0x01,
0x28, 0x00, 0x00, 0x00, 0x44, 0x00, 0x33, 0x01, 0xb0, 0xff, 0x00, 0x00, 0xb3, 0xff, 0x0b, 0x01,
0x96, 0xff, 0x00, 0x00, 0x36, 0xff, 0x83, 0x00, 0xbf, 0xff, 0x00, 0x00, 0x55, 0xff, 0x00, 0x00,
0x9b, 0xff, 0x00, 0x00, 0x41, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0xff, 0x83, 0x00,
0xac, 0x00, 0x00, 0x00, 0x50, 0xff, 0x00, 0x00, 0x92, 0x00, 0x00, 0x00, 0xb3, 0xff, 0x00, 0x00,
0xca, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x83, 0x00, 0xac, 0x00, 0x00, 0x00, 0x4d, 0x00, 0xcb, 0xff,
0xd1, 0x00, 0x00, 0x00, 0xae, 0x00, 0xae, 0xff, 0x6e, 0x00, 0x00, 0x00, 0xa5, 0x00, 0x83, 0x00,
0xbf, 0xff, 0x00, 0x00, 0xdf, 0x00, 0xbb, 0xff, 0x00, 0x00, 0x00, 0x00, 0xae, 0x00, 0xc7, 0xff,
0x7e, 0xff, 0x00, 0x00, 0xb3, 0xff, 0x00, 0x00, 0x4f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x83, 0x00,
0x2c, 0xff, 0x00, 0x00, 0x69, 0x00, 0xce, 0xff, 0x39, 0xff, 0x00, 0x00, 0xa6, 0xff, 0x7d, 0xff,
0x67, 0xff, 0x00, 0x00, 0x36, 0xff, 0x71, 0xff, 0x41, 0x00, 0x00, 0x00, 0x83, 0xff, 0xd6, 0xfe,
0xd8, 0xff, 0x00, 0x00, 0x00, 0x00, 0x3a, 0xff, 0xa1, 0x00, 0x00, 0x00, 0xb3, 0xff, 0xc6, 0xfe,
0x35, 0x00, 0x00, 0x00, 0x78, 0x00, 0x39, 0xff, 0x41, 0x00, 0x00, 0x00, 0x17, 0x00, 0xa3, 0xfe,
0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xfe, 0xa6, 0xff, 0x00, 0x00, 0x55, 0x00, 0x34, 0xff,
0x7c, 0xff, 0x00, 0x00, 0x59, 0x00, 0xdd, 0xfe, 0xd8, 0xff, 0x00, 0x00, 0x50, 0x00, 0x2c, 0x00,
0x40, 0x01, 0x08, 0x00, 0x38, 0x01, 0x80, 0x00, 0x8b, 0x37, 0x08, 0x79, 0x5e, 0x4c, 0x5e, 0x19,
0x48, 0x01, 0x10, 0x00, 0x40, 0x01, 0x80, 0x00, 0x8f, 0x24, 0x08, 0x79, 0xbc, 0x3a, 0x8b, 0x37,
0x38, 0x01, 0x00, 0x00, 0x48, 0x01, 0x80, 0x00, 0x5e, 0x19, 0x08, 0x79, 0x5e, 0x00, 0x8f, 0x24,
0x28, 0x01, 0x10, 0x00, 0x48, 0x01, 0x80, 0x00, 0x75, 0x37, 0x08, 0x79, 0x30, 0x39, 0x47, 0x24,
0x30, 0x01, 0x18, 0x00, 0x28, 0x01, 0x80, 0x00, 0x7e, 0x18, 0x08, 0x79, 0xa0, 0x35, 0x75, 0x37,
0x48, 0x01, 0x00, 0x00, 0x30, 0x01, 0x80, 0x00, 0x47, 0x24, 0x08, 0x79, 0x64, 0x00, 0x7e, 0x18,
0x18, 0x01, 0x18, 0x00, 0x30, 0x01, 0x80, 0x00, 0x3d, 0x3c, 0x08, 0x79, 0x0f, 0x38, 0x4d, 0x19,
0x20, 0x01, 0x20, 0x00, 0x18, 0x01, 0x80, 0x00, 0x7b, 0x22, 0x08, 0x79, 0x82, 0x42, 0x3d, 0x3c,
0x30, 0x01, 0x00, 0x00, 0x20, 0x01, 0x80, 0x00, 0x4d, 0x19, 0x08, 0x79, 0x66, 0x00, 0x7b, 0x22,
0x08, 0x01, 0x20, 0x00, 0x20, 0x01, 0x80, 0x00, 0x38, 0x43, 0x08, 0x79, 0x19, 0x3d, 0x42, 0x1f,
0x10, 0x01, 0x28, 0x00, 0x08, 0x01, 0x80, 0x00, 0x71, 0x23, 0x08, 0x79, 0x6e, 0x4b, 0x38, 0x43,
0x20, 0x01, 0x00, 0x00, 0x10, 0x01, 0x80, 0x00, 0x42, 0x1f, 0x08, 0x79, 0x5e, 0x00, 0x71, 0x23,
0x00, 0x01, 0x28, 0x00, 0x10, 0x01, 0x80, 0x00, 0xad, 0x47, 0x08, 0x79, 0x6e, 0x4b, 0x71, 0x23,
0x38, 0x01, 0x08, 0x00, 0x00, 0x01, 0x80, 0x00, 0x5e, 0x19, 0x08, 0x79, 0x5e, 0x4c, 0x2a, 0x47,
0x10, 0x01, 0x00, 0x00, 0x38, 0x01, 0x80, 0x00, 0x16, 0x23, 0x08, 0x79, 0x5e, 0x00, 0x5e, 0x19,
0xf0, 0x00, 0x50, 0x00, 0xe8, 0x00, 0x80, 0x00, 0x5e, 0x80, 0x08, 0x79, 0x13, 0x7b, 0x30, 0x64,
0xf8, 0x00, 0x30, 0x00, 0xf0, 0x00, 0x80, 0x00, 0x95, 0x59, 0x08, 0x79, 0xa5, 0x7b, 0x5e, 0x80,
0xe8, 0x00, 0x08, 0x00, 0xf8, 0x00, 0x80, 0x00, 0x30, 0x64, 0x08, 0x79, 0x5e, 0x4c, 0x95, 0x59,
0xe0, 0x00, 0x30, 0x00, 0xf8, 0x00, 0x80, 0x00, 0xbc, 0x57, 0x08, 0x79, 0xa5, 0x7b, 0x95, 0x59,
0x40, 0x01, 0x10, 0x00, 0xe0, 0x00, 0x80, 0x00, 0x8b, 0x37, 0x08, 0x79, 0xbc, 0x3a, 0xbc, 0x57,
0xf8, 0x00, 0x08, 0x00, 0x40, 0x01, 0x80, 0x00, 0x95, 0x59, 0x08, 0x79, 0x5e, 0x4c, 0x8b, 0x37,
0xd0, 0x00, 0x30, 0x00, 0xe0, 0x00, 0x80, 0x00, 0x33, 0x7f, 0x08, 0x79, 0x00, 0x7b, 0x14, 0x56,
0xd8, 0x00, 0x38, 0x00, 0xd0, 0x00, 0x80, 0x00, 0x4d, 0x53, 0x08, 0x79, 0x72, 0x7b, 0x33, 0x7f,
0xe0, 0x00, 0x10, 0x00, 0xd8, 0x00, 0x80, 0x00, 0x14, 0x56, 0x08, 0x79, 0x30, 0x39, 0x4d, 0x53,
0xc8, 0x00, 0x38, 0x00, 0xd8, 0x00, 0x80, 0x00, 0x86, 0x51, 0x08, 0x79, 0x72, 0x7b, 0x4d, 0x53,
0x28, 0x01, 0x18, 0x00, 0xc8, 0x00, 0x80, 0x00, 0x75, 0x37, 0x08, 0x79, 0xa0, 0x35, 0x86, 0x51,
0xd8, 0x00, 0x10, 0x00, 0x28, 0x01, 0x80, 0x00, 0x4d, 0x53, 0x08, 0x79, 0x30, 0x39, 0x75, 0x37,
0xb8, 0x00, 0x38, 0x00, 0xc8, 0x00, 0x80, 0x00, 0xa9, 0x7f, 0x08, 0x79, 0x72, 0x7b, 0x86, 0x51,
0xc0, 0x00, 0x40, 0x00, 0xb8, 0x00, 0x80, 0x00, 0x0c, 0x5c, 0x08, 0x79, 0x2b, 0x80, 0x00, 0x82,
0xc8, 0x00, 0x18, 0x00, 0xc0, 0x00, 0x80, 0x00, 0x86, 0x51, 0x08, 0x79, 0xa0, 0x35, 0xbd, 0x57,
0xb0, 0x00, 0x40, 0x00, 0xc0, 0x00, 0x80, 0x00, 0x53, 0x68, 0x08, 0x79, 0x2b, 0x80, 0x0c, 0x5c,
0x18, 0x01, 0x20, 0x00, 0xb0, 0x00, 0x80, 0x00, 0x3d, 0x3c, 0x08, 0x79, 0x82, 0x42, 0x53, 0x68,
0xc0, 0x00, 0x18, 0x00, 0x18, 0x01, 0x80, 0x00, 0x0c, 0x5c, 0x08, 0x79, 0x0f, 0x38, 0x3d, 0x3c,
0xa0, 0x00, 0x40, 0x00, 0xb0, 0x00, 0x80, 0x00, 0x72, 0x85, 0x08, 0x79, 0x2b, 0x80, 0x53, 0x68,
0xa8, 0x00, 0x48, 0x00, 0xa0, 0x00, 0x80, 0x00, 0x0c, 0x63, 0x08, 0x79, 0x35, 0x7a, 0x00, 0x7f,
0xb0, 0x00, 0x20, 0x00, 0xa8, 0x00, 0x80, 0x00, 0x53, 0x68, 0x08, 0x79, 0x82, 0x42, 0x91, 0x68,
0x98, 0x00, 0x48, 0x00, 0xa8, 0x00, 0x80, 0x00, 0x5b, 0x63, 0x08, 0x79, 0x35, 0x7a, 0x0c, 0x63,
0x08, 0x01, 0x28, 0x00, 0x98, 0x00, 0x80, 0x00, 0x38, 0x43, 0x08, 0x79, 0x6e, 0x4b, 0x5b, 0x63,
0xa8, 0x00, 0x20, 0x00, 0x08, 0x01, 0x80, 0x00, 0x0c, 0x63, 0x08, 0x79, 0x19, 0x3d, 0x38, 0x43,
0x88, 0x00, 0x48, 0x00, 0x98, 0x00, 0x80, 0x00, 0x7d, 0x7f, 0x08, 0x79, 0x35, 0x7a, 0x5b, 0x63,
0x90, 0x00, 0x50, 0x00, 0x88, 0x00, 0x80, 0x00, 0x90, 0x63, 0x08, 0x79, 0xbc, 0x7b, 0x7d, 0x7f,
0x98, 0x00, 0x28, 0x00, 0x90, 0x00, 0x80, 0x00, 0x5b, 0x63, 0x08, 0x79, 0x6e, 0x4b, 0x90, 0x63,
0xe8, 0x00, 0x50, 0x00, 0x90, 0x00, 0x80, 0x00, 0xb9, 0x63, 0x08, 0x79, 0xbc, 0x7b, 0x90, 0x63,
0x00, 0x01, 0x08, 0x00, 0xe8, 0x00, 0x80, 0x00, 0x2a, 0x47, 0x08, 0x79, 0x5e, 0x4c, 0x30, 0x64,
0x90, 0x00, 0x28, 0x00, 0x00, 0x01, 0x80, 0x00, 0x90, 0x63, 0x08, 0x79, 0x6e, 0x4b, 0xad, 0x47,
0xf0, 0x00, 0x30, 0x00, 0x78, 0x00, 0x80, 0x00, 0x5e, 0x80, 0x08, 0x79, 0xa5, 0x7b, 0x82, 0xa5,
0x80, 0x00, 0x50, 0x00, 0xf0, 0x00, 0x80, 0x00, 0x30, 0x9d, 0x08, 0x79, 0x13, 0x7b, 0x5e, 0x80,
0x78, 0x00, 0x58, 0x00, 0x80, 0x00, 0x80, 0x00, 0x82, 0xa5, 0x08, 0x79, 0x75, 0xbe, 0x30, 0x9d,
0xd0, 0x00, 0x38, 0x00, 0x70, 0x00, 0x80, 0x00, 0x33, 0x7f, 0x08, 0x79, 0x72, 0x7b, 0x6c, 0x9e,
0x78, 0x00, 0x30, 0x00, 0xd0, 0x00, 0x80, 0x00, 0x36, 0xa5, 0x08, 0x79, 0x00, 0x7b, 0x33, 0x7f,
0x70, 0x00, 0x58, 0x00, 0x78, 0x00, 0x80, 0x00, 0x6c, 0x9e, 0x08, 0x79, 0x5e, 0xbe, 0x36, 0xa5,
0xb8, 0x00, 0x40, 0x00, 0x68, 0x00, 0x80, 0x00, 0x00, 0x82, 0x08, 0x79, 0x2b, 0x80, 0x53, 0xa6,
0x70, 0x00, 0x38, 0x00, 0xb8, 0x00, 0x80, 0x00, 0x6c, 0x9e, 0x08, 0x79, 0x72, 0x7b, 0xa9, 0x7f,
0x68, 0x00, 0x58, 0x00, 0x70, 0x00, 0x80, 0x00, 0x86, 0xa4, 0x08, 0x79, 0x5e, 0xbe, 0x6c, 0x9e,
0xa0, 0x00, 0x48, 0x00, 0x60, 0x00, 0x80, 0x00, 0x00, 0x7f, 0x08, 0x79, 0x35, 0x7a, 0x48, 0x9d,
0x68, 0x00, 0x40, 0x00, 0xa0, 0x00, 0x80, 0x00, 0x53, 0xa6, 0x08, 0x79, 0x2b, 0x80, 0x72, 0x85,
0x60, 0x00, 0x58, 0x00, 0x68, 0x00, 0x80, 0x00, 0x48, 0x9d, 0x08, 0x79, 0x5e, 0xbe, 0x38, 0xa5,
0x88, 0x00, 0x50, 0x00, 0x80, 0x00, 0x80, 0x00, 0x7d, 0x7f, 0x08, 0x79, 0xbc, 0x7b, 0x97, 0x9c,
0x60, 0x00, 0x48, 0x00, 0x88, 0x00, 0x80, 0x00, 0x48, 0x9d, 0x08, 0x79, 0x35, 0x7a, 0x7d, 0x7f,
0x80, 0x00, 0x58, 0x00, 0x60, 0x00, 0x80, 0x00, 0x97, 0x9c, 0x08, 0x79, 0x5e, 0xbe, 0x48, 0x9d,
0x80, 0x00, 0x60, 0x00, 0x88, 0x00, 0x80, 0x00, 0x97, 0x9c, 0x08, 0x79, 0x48, 0x9d, 0x7d, 0x7f,
0x60, 0x00, 0x68, 0x00, 0xa0, 0x00, 0x80, 0x00, 0x48, 0x9d, 0x08, 0x79, 0x38, 0xa5, 0x00, 0x7f,
0x68, 0x00, 0x70, 0x00, 0xb8, 0x00, 0x80, 0x00, 0x86, 0xa4, 0x08, 0x79, 0x6c, 0x9e, 0xa9, 0x7f,
0x70, 0x00, 0x78, 0x00, 0xd0, 0x00, 0x80, 0x00, 0x6c, 0x9e, 0x08, 0x79, 0x36, 0xa5, 0x33, 0x7f,
0x78, 0x00, 0x80, 0x00, 0xf0, 0x00, 0x80, 0x00, 0x82, 0xa5, 0x08, 0x79, 0x30, 0x9d, 0x5e, 0x80,
0x90, 0x00, 0x00, 0x01, 0xe8, 0x00, 0x80, 0x00, 0x90, 0x63, 0x08, 0x79, 0xad, 0x47, 0xb9, 0x63,
0x98, 0x00, 0x90, 0x00, 0x88, 0x00, 0x80, 0x00, 0x5b, 0x63, 0x08, 0x79, 0x90, 0x63, 0x7d, 0x7f,
0xa8, 0x00, 0x08, 0x01, 0x98, 0x00, 0x80, 0x00, 0x0c, 0x63, 0x08, 0x79, 0x38, 0x43, 0x5b, 0x63,
0xb0, 0x00, 0xa8, 0x00, 0xa0, 0x00, 0x80, 0x00, 0x53, 0x68, 0x08, 0x79, 0x91, 0x68, 0x72, 0x85,
0xc0, 0x00, 0x18, 0x01, 0xb0, 0x00, 0x80, 0x00, 0x0c, 0x5c, 0x08, 0x79, 0x3d, 0x3c, 0x53, 0x68,
0xc8, 0x00, 0xc0, 0x00, 0xb8, 0x00, 0x80, 0x00, 0x86, 0x51, 0x08, 0x79, 0xbd, 0x57, 0xa9, 0x7f,
0xd8, 0x00, 0x28, 0x01, 0xc8, 0x00, 0x80, 0x00, 0x4d, 0x53, 0x08, 0x79, 0x75, 0x37, 0x86, 0x51,
0xe0, 0x00, 0xd8, 0x00, 0xd0, 0x00, 0x80, 0x00, 0x14, 0x56, 0x08, 0x79, 0x4d, 0x53, 0x33, 0x7f,
0xf8, 0x00, 0x40, 0x01, 0xe0, 0x00, 0x80, 0x00, 0x95, 0x59, 0x08, 0x79, 0x8b, 0x37, 0xbc, 0x57,
0xe8, 0x00, 0xf8, 0x00, 0xf0, 0x00, 0x80, 0x00, 0x30, 0x64, 0x08, 0x79, 0x95, 0x59, 0x5e, 0x80,
0x10, 0x01, 0x38, 0x01, 0x00, 0x01, 0x80, 0x00, 0x16, 0x23, 0x08, 0x79, 0x5e, 0x19, 0x2a, 0x47,
0x20, 0x01, 0x10, 0x01, 0x08, 0x01, 0x80, 0x00, 0x42, 0x1f, 0x08, 0x79, 0x71, 0x23, 0x38, 0x43,
0x30, 0x01, 0x20, 0x01, 0x18, 0x01, 0x80, 0x00, 0x4d, 0x19, 0x08, 0x79, 0x7b, 0x22, 0x3d, 0x3c,
0x48, 0x01, 0x30, 0x01, 0x28, 0x01, 0x80, 0x00, 0x47, 0x24, 0x08, 0x79, 0x7e, 0x18, 0x75, 0x37,
0x38, 0x01, 0x48, 0x01, 0x40, 0x01, 0x80, 0x00, 0x5e, 0x19, 0x08, 0x79, 0x8f, 0x24, 0x8b, 0x37,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a04f0
int g_BattleSpecFreezeBreathInitialTemplate[0x199] = {336, (-30408704), 0, (-7274496), 65349, (-12713812), 65467, (-14024601), 149, (-11534467), 125, (-7209110), 65499, 7274627, 65356, 7274668, 69, 7274496, 223, 7339820, 69, 7339901, 65356, 27721769, 0, 17694625, 40, 19791872, 73, 18088037, 40, 20119620, 65456, 17563571, 65430, 8650550, 65471, 65365, 65435, 65345, 0, 8650628, 172, 65360, 146, 65459, 202, 8585340, 172, (-3473331), 209, (-5373778), 110, 8585381, 65471, (-4521761), 0, (-3735378), 65406, 65459, 65359, 8585216, 65324, (-3276695), 65337, (-8519770), 65383, (-9306314), 65, (-19464317), 65496, (-12976128), 161, (-20512845), 53, (-13041544), 65, (-22872041), 59, (-22609920), 65446, (-13369259), 65404, (-19070887), 65496, 2883664, 524608, 8388920, 2030581643, 425610334, 1048904, 8388928, 2030576783, 931871420, 312, 8388936, 2030573918, 613351518, 1048872, 8388936, 2030581621, 608647472, 1573168, 8388904, 2030573694, 930428320, 328, 8388912, 2030576711, 410910820, 1573144, 8388912, 2030582845, 424491023, 2097440, 8388888, 2030576251, 1010647682, 304, 8388896, 2030573901, 578486374, 2097416, 8388896, 2030584632, 524434713, 2621712, 8388872, 2030576497, 1127762798, 288, 8388880, 2030575426, 594608222, 2621696, 8388880, 2030585773, 594627438, 524600, 8388864, 2030573918, 1193954398, 272, 8388920, 2030576406, 425590878, 5243120, 8388840, 2030600286, 1680898835, 3145976, 8388848, 2030590357, (-2141291611), 524520, 8388856, 2030593072, 1502956638, 3145952, 8388856, 2030589884, 1502968741, 1048896, 8388832, 2030581643, 1471953596, 524536, 8388928, 2030590357, 931875934, 3145936, 8388832, 2030599987, 1444182784, 3670232, 8388816, 2030588749, 2134080370, 1048800, 8388824, 2030589460, 1397569840, 3670216, 8388824, 2030588294, 1397586802, 1573160, 8388808, 2030581621, 1367750048, 1048792, 8388904, 2030588749, 930429232, 3670200, 8388808, 2030600105, 1367767922, 4194496, 8388792, 2030590988, (-2113896405), 1573064, 8388800, 2030588294, 1472017824, 4194480, 8388800, 2030594131, 1544323115, 2097432, 8388784, 2030582845, 1750286978, 1573056, 8388888, 2030590988, 1010645007, 4194464, 8388784, 2030601586, 1750302763, 4718760, 8388768, 2030592780, 2130737717, 2097328, 8388776, 2030594131, 1754350210, 4718744, 8388776, 2030592859, 1661762101, 2621704, 8388760, 2030584632, 1666927470, 2097320, 8388872, 2030592780, 1127759129, 4718728, 8388760, 2030600061, 1666939445, 5243024, 8388744, 2030592912, 2138930108, 2621592, 8388752, 2030592859, 1670400878, 5243112, 8388752, 2030592953, 1670413244, 524544, 8388840, 2030585642, 1680886878, 2621584, 8388864, 2030592912, 1202539374, 3145968, 8388728, 2030600286, (-1518175323), 5243008, 8388848, 2030607664, (-2141291757), 5767288, 8388736, 2030609794, (-1657749899), 3670224, 8388720, 2030599987, (-1637057678), 3145848, 8388816, 2030609718, 2134080256, 5767280, 8388728, 2030607980, (-1523138978), 4194488, 8388712, 2030600704, (-1504477141), 3670128, 8388792, 2030607980, 2141813618, 5767272, 8388720, 2030609542, (-1637040546), 4718752, 8388704, 2030599936, (-1656194507), 4194408, 8388768, 2030610003, (-2056093653), 5767264, 8388712, 2030607688, (-1523007906), 5243016, 8388736, 2030600061, (-1667793988), 4718688, 8388744, 2030607688, 2138929717, 5767296, 8388704, 2030607511, (-1656177058), 6291584, 8388744, 2030607511, 2138938696, 6815840, 8388768, 2030607688, 2130748728, 7340136, 8388792, 2030609542, 2141822572, 7864432, 8388816, 2030607980, 2134091062, 8388728, 8388848, 2030609794, (-2141283024), 16777360, 8388840, 2030592912, 1673086893, 9437336, 8388744, 2030592859, 2138923920, 17301672, 8388760, 2030592780, 1666925368, 11010224, 8388768, 2030594131, (-2056099695), 18350272, 8388784, 2030590988, 1750285373, 12583112, 8388792, 2030588294, 2141804477, 19398872, 8388808, 2030588749, 1367750517, 14156000, 8388816, 2030589460, 2134070093, 20971768, 8388832, 2030590357, 1471952779, 16253160, 8388848, 2030593072, (-2141300331), 20447504, 8388864, 2030576406, 1193941342, 17826080, 8388872, 2030575426, 1127752561, 18874672, 8388888, 2030573901, 1010639483, 19923272, 8388904, 2030576711, 930420862, 21496120, 8388928, 2030573918, 931865743, 0, 0, 0};
// GLOBAL: FF7 0x008a0b58
struct BattleSpecFreezeBreathSpriteReq g_BattleSpecFreezeBreathSpriteReq = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
// GLOBAL: FF7 0x008a0b68
int g_BattleSpecFreezeBreathRotAngles1 = (int)0xFC000000;
// GLOBAL: FF7 0x008a0b70
BattleSpecFreezeBreathScaleVector1 g_BattleSpecFreezeBreathScaleVector1 = {{0x1200, 0x1200, 0x1200}, {0, 0, -128, -128, -128, 0x2e}, {0, 0, -128, -128, -128, 0x2c}};
// GLOBAL: FF7 0x008a0ba0
struct BattleSpecFreezeBreathScaleVector2 g_BattleSpecFreezeBreathScaleVector2 = {{0, 0x28, 0, 0, 0, 0x28, 0, 0, 0, 0x28, 0, 0, 0, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0, 0x80, 0, 0x20, 0, 0, 0}, 0};
unsigned char g_rawData_008a0bc4[4] = {0xa8, 0x00, 0x00, 0x00};
unsigned char g_rawData_008a0bc8[8] = {0x00, 0x00, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00};
// The growing ice model's eight stage models.
// GLOBAL: FF7 0x008a0bd0
int g_BattleSpecFreezeBreathModelIndices[8] = {1, 2, 3, 4, 5, 6, 7, 8};
// GLOBAL: FF7 0x008a0bf0
unsigned char g_BattleSpecFreezeBreathModelReq[8] = {0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00};
unsigned char g_rawData_008a0bf8[8] = {0x00, 0x00, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a0c00
struct BattleSpecFreezeBreathScaleVector3 g_BattleSpecFreezeBreathScaleVector3 = {{0, 4, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_008a0c14[4] = {0x88, 0x00, 0x00, 0x00};
unsigned char g_rawData_008a0c18[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a0c20
int g_BattleSpecFreezeBreathRotAngles2[2] = {64512, 0};
// GLOBAL: FF7 0x00b5e468
struct BattleSpecialHarrierLookMatrix g_BattleSpecFreezeBreathBasisMatrix = {0};
// Per-face flags: set while ordering a face, then once the face is released.
// GLOBAL: FF7 0x00b5e488
short g_BattleSpecFreezeBreathFaceActiveFlags[0x80] = {0};
// GLOBAL: FF7 0x00b5e588
BattleSpecFreezeBreathFaceIndex g_BattleSpecFreezeBreathFaceIndex = {0};
unsigned char g_bssPad_00b5e598[1020] = {0};
// GLOBAL: FF7 0x00b5e994
int g_BattleSpecFreezeBreathDrawListHead = 0;
// The faces in descending depth, the order in which they are released.
// GLOBAL: FF7 0x00b5e998
short g_BattleSpecFreezeBreathFaceOrder[0x80] = {0};
// GLOBAL: FF7 0x00b5ea98
struct GfxTableCursor *g_BattleSpecFreezeBreathLoadedResTable = 0;
unsigned char g_bssPad_00b5ea9c[4] = {0};
// GLOBAL: FF7 0x00b5eaa0
int g_BattleSpecFreezeBreathRotationVector = 0;
unsigned char g_bssPad_00b5eaa4[4] = {0};
unsigned char g_bssPad_00b5eaa8[8] = {0};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * When the engine context exists and g_BattleSpecFreezeBreathLoadedResTable is empty, builds SPECIAL/REITO with a default-
 * duration record and creates resource type 15 using g_BattleSpecFreezeBreathResourceDefs.
 */
// FUNCTION: FF7 0x0052d8b0
void __cdecl BattleFx_SpecialLoadResFreezeBreath(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecFreezeBreathLoadedResTable == 0) {
        kind = 0xF;
        resourceDefsWord = (int)&g_BattleSpecFreezeBreathResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a0cd4
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/REITO", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecFreezeBreathLoadedResTable, curOwnerCtx);
    }
}

/*
 * Restores the working ice mesh from g_BattleSpecFreezeBreathInitialTemplate, loads the resources, resets
 * the face release index and begins the effect.
 */
// FUNCTION: FF7 0x0052d940
void __cdecl BattleFx_SpecialEnterFreezeBreath(int targetMask, int actorIdx)
{
    *(FreezeBreathTemplate *)&g_BattleSpecFreezeBreathWorkingTemplate = *(FreezeBreathTemplate *)g_BattleSpecFreezeBreathInitialTemplate;
    BattleFx_SpecialLoadResFreezeBreath();
    g_BattleSpecFreezeBreathFaceIndex.faceIndex = 0;
    BattleFx_SpecialBeginFreezeBreath(targetMask, actorIdx);
}

/*
 * Creates mode-four BattleFx_SpecialSpawnFreezeBreathPerTarget actor-mask callbacks, dispatches (0x65, narrowed targetActor, 0), calls
 * BattleFx_SpecialFreezeBreathBuildDescendingOrder, sets the global color triple to (0, 0, 0x40), and starts a 0xC00 ramp with
 * duration argument ten.
 */
// FUNCTION: FF7 0x0052d978
void __cdecl BattleFx_SpecialBeginFreezeBreath(int targetMask, int actorIdx)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnFreezeBreathPerTarget);
    Battle_PlaySfxPannedToActor(0x65, (short)actorIdx, 0);
    BattleFx_SpecialFreezeBreathBuildDescendingOrder();
    g_BattleFxTimelineState.channel1 = 0;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    g_BattleFxTimelineState.channel2 = 0x40;
    Battle_StartFixedPointRamp(0xC00, 0xA);
}

/*
 * Clears the face flags and offsets, then orders the mesh's 0x50 faces by descending depth (the
 * second short of each face's first vertex) into g_BattleSpecFreezeBreathFaceOrder.
 */
// FUNCTION: FF7 0x0052d9d1
void __cdecl BattleFx_SpecialFreezeBreathBuildDescendingOrder(void)
{
    int facePointer;
    short chosen;
    int meshAddress;
    short maxDepth;
    int table;
    short pass, faceIndex;

    for (pass = 0; pass < 0x50; pass++) {
        g_BattleSpecFreezeBreathFaceActiveFlags[pass] = 0;
        g_BattleSpecFreezeBreathFaceIndex.offsets[pass].z = 0;
        g_BattleSpecFreezeBreathFaceIndex.offsets[pass].y = 0;
        g_BattleSpecFreezeBreathFaceIndex.offsets[pass].x = 0;
    }
    meshAddress = (int)&g_BattleSpecFreezeBreathTemplateField;
    table = meshAddress + g_BattleSpecFreezeBreathWorkingTemplate;
    table += 4;
    for (pass = 0; pass < 0x50; pass++) {
        maxDepth = 0x8000;
        for (faceIndex = 0; faceIndex < 0x50; faceIndex++) {
            if (g_BattleSpecFreezeBreathFaceActiveFlags[faceIndex] == 0) {
                facePointer = meshAddress + *(short *)((char *)table + (faceIndex << 3) * 2);
                if (*(short *)((char *)facePointer + 2) > maxDepth) {
                    maxDepth = *(short *)((char *)facePointer + 2);
                    chosen = faceIndex;
                }
            }
        }
        g_BattleSpecFreezeBreathFaceOrder[pass] = chosen;
        g_BattleSpecFreezeBreathFaceActiveFlags[chosen] = 1;
    }
    for (pass = 0; pass < 0x50; pass++) {
        g_BattleSpecFreezeBreathFaceActiveFlags[pass] = 0;
    }
}

// Spawns a BattleFx_SpecialFreezeBreathTimelineTask slot for a source and target actor. The target's
// primary bone world position goes to the start position (startX, startY, startZ) and the source's
// target bone position to the end position. The source actor is stored in actor. The
// direction is the start-to-end difference divided by 8, which builds g_BattleSpecFreezeBreathBasisMatrix
// through Gte_BuildLookBasisFromDirection.
// FUNCTION: FF7 0x0052db2c
void __cdecl BattleFx_SpecialSpawnFreezeBreathPerTarget(int actor, int targetActor)
{
    BattleFxFreezeBreathTimeline *timelinePtr;
    short direction[3];

    timelinePtr = (BattleFxFreezeBreathTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFreezeBreathTimelineTask);
    Battle_GetBoneWorldPos(targetActor, g_BattleActorRecords[targetActor].primaryBone, &timelinePtr->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &timelinePtr->endX);
    timelinePtr->sourceActor = actor;
    direction[0] = (timelinePtr->endX - timelinePtr->startX) / 8;
    direction[1] = (timelinePtr->endY - timelinePtr->startY) / 8;
    direction[2] = (timelinePtr->endZ - timelinePtr->startZ) / 8;
    Gte_BuildLookBasisFromDirection(direction, (short *)&g_BattleSpecFreezeBreathBasisMatrix);
}

/*
 * Reito timeline: on frame 0 spawns the scaled sprite at the start, for 15 frames three moving
 * models a frame from the start towards the target, on frames 9 to 17 scattered sprites around the
 * target, at frame 13 the face-offset ice mesh and the spin models, at frame 65 the target's typed
 * record; at frame 70 it ramps the colour back and frees the slot.
 */
// FUNCTION: FF7 0x0052dc0c
void __cdecl BattleFx_SpecialFreezeBreathTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        struct { unsigned char pad_to_8[0x8]; int depth; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_34[0x34]; BattleFxSpecialEnergyOrbSlot *spawnSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short burstIndex; };
        struct { unsigned char pad_to_40[0x40]; short stepX; };
        struct { unsigned char pad_to_42[0x42]; short stepY; };
        struct { unsigned char pad_to_44[0x44]; short stepZ; };
        struct { unsigned char pad_to_48[0x48]; BattleFxOilTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_4C[0x4C]; SVECTOR orbitX; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxOilTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.offsetX.vx = 0;
        frame.offsetX.vy = 0;
        frame.offsetX.vz = 0xC8;
        g_BattleSpecFreezeBreathBasisMatrix.transX = frame.timelineSlot->startX;
        g_BattleSpecFreezeBreathBasisMatrix.transY = frame.timelineSlot->startY;
        g_BattleSpecFreezeBreathBasisMatrix.transZ = frame.timelineSlot->startZ;
        SetRotMatrix(&g_BattleSpecFreezeBreathBasisMatrix);
        SetTransMatrix(&g_BattleSpecFreezeBreathBasisMatrix);
        RotTransPointToInt16(&frame.offsetX, (short *)((int)frame.timelineSlot + 4), &frame.depth);
        frame.spawnSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFreezeBreathDrawScaledSpriteTask);
        frame.spawnSlot->posX = frame.timelineSlot->startX;
        frame.spawnSlot->posY = frame.timelineSlot->startY;
        frame.spawnSlot->posZ = frame.timelineSlot->startZ;
    }
    if (frame.timelineSlot->frameCounter < 0xF) {
        g_BattleSpecFreezeBreathBasisMatrix.transX = frame.timelineSlot->startX;
        g_BattleSpecFreezeBreathBasisMatrix.transY = frame.timelineSlot->startY;
        g_BattleSpecFreezeBreathBasisMatrix.transZ = frame.timelineSlot->startZ;
        SetRotMatrix(&g_BattleSpecFreezeBreathBasisMatrix);
        SetTransMatrix(&g_BattleSpecFreezeBreathBasisMatrix);
        frame.stepX = (frame.timelineSlot->endX - frame.timelineSlot->startX) / 8;
        frame.stepY = (frame.timelineSlot->endY - frame.timelineSlot->startY) / 8;
        frame.stepZ = (frame.timelineSlot->endZ - frame.timelineSlot->startZ) / 8;
        for (frame.burstIndex = 0; frame.burstIndex < 3; frame.burstIndex++) {
            frame.spawnSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFreezeBreathMovingModelTask);
            frame.orbitX.vx = CosFixed((frame.timelineSlot->frameCounter << 9) + frame.burstIndex * 0x555) * 0x50 >> 0xC;
            frame.orbitX.vy = SinFixed((frame.timelineSlot->frameCounter << 9) + frame.burstIndex * 0x555) * 0x50 >> 0xC;
            frame.orbitX.vz = rand() % 0x64 - 0x32;
            RotTransPointToInt16(&frame.orbitX, (short *)((int)frame.spawnSlot + 4), &frame.depth);
            frame.spawnSlot->velocityX = frame.stepX;
            frame.spawnSlot->velocityY = frame.stepY;
            frame.spawnSlot->velocityZ = frame.stepZ;
        }
    }
    if (frame.timelineSlot->frameCounter > 8 && frame.timelineSlot->frameCounter < 0x12) {
        frame.spawnSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFreezeBreathScatteredAnimatedSpriteTask);
        frame.spawnSlot->posX = frame.timelineSlot->endX + rand() % 0x320 + -0x190;
        frame.spawnSlot->posY = frame.timelineSlot->endY + rand() % 0x320 + -0x190;
        frame.spawnSlot->posZ = frame.timelineSlot->endZ + rand() % 0x320 + -0x190;
    }
    if (frame.timelineSlot->frameCounter == 0xD) {
        frame.spawnSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFreezeBreathStagedFaceOffsetModelTask);
        frame.spawnSlot->posX = frame.timelineSlot->endX;
        frame.spawnSlot->posY = frame.timelineSlot->endY;
        frame.spawnSlot->posZ = frame.timelineSlot->endZ;
        frame.spawnSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFreezeBreathSpinModelsEmitTask);
        frame.spawnSlot->posX = frame.timelineSlot->endX;
        frame.spawnSlot->posY = frame.timelineSlot->endY - 0x578;
        frame.spawnSlot->posZ = frame.timelineSlot->endZ;
        Battle_PlaySfxPannedToActor(0x187, (short)frame.timelineSlot->actorIndex, 0);
    }
    if (frame.timelineSlot->frameCounter == 0x41) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 0x46) {
        Battle_StartFixedPointRamp(0, 0xA);
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the sprite of g_BattleSpecFreezeBreathSpriteReq at the slot, scaled by
 * g_BattleSpecFreezeBreathScaleVector1.scale, and frees the slot at frame 20.
 */
// FUNCTION: FF7 0x0052e026
void __cdecl BattleFx_SpecialFreezeBreathDrawScaledSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[36];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxCosmoMemoryExpandingModel *sprite; };
    } frame;
#pragma pack(pop)

    frame.sprite = (BattleFxCosmoMemoryExpandingModel *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecFreezeBreathSpriteReq.spriteResource) = Gfx_ModelTableGetLoadedRecord(0xC, g_BattleSpecFreezeBreathLoadedResTable);
    g_BattleSpecFreezeBreathSpriteReq.resourceHandle = Gfx_ModelTableGetSpriteObject(0xC, g_BattleSpecFreezeBreathLoadedResTable);
    RotMatrix((SVECTOR *)&g_BattleSpecFreezeBreathRotAngles1, &frame.matrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecFreezeBreathBasisMatrix, &frame.matrix, &frame.matrix);
    ScaleMatrix(&frame.matrix, &g_BattleSpecFreezeBreathScaleVector1.scale);
    frame.matrix.t[0] = frame.sprite->posX;
    frame.matrix.t[1] = frame.sprite->posY;
    frame.matrix.t[2] = frame.sprite->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    g_BattleSpecFreezeBreathSpriteReq.frame = frame.sprite->frame % *(int *)((char *)*(int *)(g_BattleSpecFreezeBreathSpriteReq.spriteResource) + 4);
    g_BattleSpecFreezeBreathDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecFreezeBreathSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecFreezeBreathDrawListHead);
    if (g_BattleIsPaused == 0) {
        (frame.sprite->frame)++;
        if (frame.sprite->frame == 0x14) {
            frame.sprite->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Per-frame tick for the effect record at (g_BattleActiveTaskSlotIdx << 5) +
 * &g_BattleHitLivewireEmitterRecords using animation entry 0xA of resource
 * g_BattleSpecFreezeBreathLoadedResTable in g_BattleSpecFreezeBreathScaleVector1.scatterSprite, billboarded at
 * scale 0x1000, with the frame counter copied into the sprite's frame and
 * the draw chained into g_BattleSpecFreezeBreathDrawListHead. When unpaused with a nonnull sprite it
 * advances the counter and frees the slot when it equals the animation's frame count.
 */
// FUNCTION: FF7 0x0052e160
void __cdecl BattleFx_SpecialFreezeBreathScatteredAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecFreezeBreathScaleVector1.scatterSprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0xA, g_BattleSpecFreezeBreathLoadedResTable);
    g_BattleSpecFreezeBreathScaleVector1.scatterSprite.drawObj = Gfx_ModelTableGetSpriteObject(0xA, g_BattleSpecFreezeBreathLoadedResTable);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x1000, 0);
    g_BattleSpecFreezeBreathScaleVector1.scatterSprite.frame = task->frameCounter;
    g_BattleSpecFreezeBreathDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecFreezeBreathScaleVector1.scatterSprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecFreezeBreathDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleSpecFreezeBreathScaleVector1.scatterSprite.spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter == g_BattleSpecFreezeBreathScaleVector1.scatterSprite.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws the growing ice model through its eight stages for 30 frames, then the ice mesh face by
 * face with each face's offset; past frame 30 it releases four more faces a frame and moves every
 * released face by 0x14 in Y.
 */
// FUNCTION: FF7 0x0052e231
void __cdecl BattleFx_SpecialFreezeBreathStagedFaceOffsetModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[88];
        int modelIndex;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix meshPosition; };
        struct { unsigned char pad_to_24[0x24]; int matrix; };
        struct { unsigned char pad_to_36[0x36]; int slotField04; };
        struct { unsigned char pad_to_3A[0x3A]; int slotField06; };
        struct { unsigned char pad_to_3E[0x3E]; int slotField08; };
        struct { unsigned char pad_to_48[0x48]; short faceLoop; };
        struct { unsigned char pad_to_4C[0x4C]; int templateBase; };
        struct { unsigned char pad_to_50[0x50]; BattleFxOilTargetModelSlot *slot; };
        struct { unsigned char pad_to_54[0x54]; int templateAddr; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    RotMatrix((SVECTOR *)&g_BattleSpecFreezeBreathRotationVector, &frame.matrix);
    ScaleMatrix(&frame.matrix, (int *)&g_BattleSpecFreezeBreathScaleVector2);
    frame.slotField04 = frame.slot->posX;
    frame.slotField06 = frame.slot->posY;
    frame.slotField08 = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetDrawColorRGB(0, 0, 0);
    if (frame.slot->frameCounter < 0x1E) {
        SetRotMatrix(&frame.matrix);
        SetTransMatrix(&frame.matrix);
        if (frame.slot->frameCounter < 0x10) {
            frame.modelIndex = frame.slot->frameCounter >> 1;
        } else {
            frame.modelIndex = 7;
        }
        g_BattleSpecFreezeBreathScaleVector2.modelResource = Gfx_ModelTableGetRenderObject(g_BattleSpecFreezeBreathModelIndices[frame.modelIndex], g_BattleSpecFreezeBreathLoadedResTable);
        g_BattleSpecFreezeBreathDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecFreezeBreathScaleVector2.modelResource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecFreezeBreathDrawListHead);
    } else {
        RotMatrix((SVECTOR *)&g_BattleSpecFreezeBreathRotationVector, &frame.meshPosition);
        g_BattleSpecFreezeBreathScaleVector2.meshTemplate = (int)&g_BattleSpecFreezeBreathWorkingTemplate;
        Gfx_SetPsxPrimQuadState(9, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0xD, g_BattleSpecFreezeBreathLoadedResTable)));
        Gfx_SetPsxPrimQuadState(9, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0xE, g_BattleSpecFreezeBreathLoadedResTable)));
        g_BattleSpecFreezeBreathDrawListHead = BattleFx_SpecialFreezeBreathDrawPerFaceOffsetMesh((BattleFxFreezeBreathDrawRecord *)&g_BattleSpecFreezeBreathScaleVector2.meshTemplate, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, (BattleFxFreezeBreathDrawPrim *)g_BattleSpecFreezeBreathDrawListHead, (Vec3sPacked *)g_BattleSpecFreezeBreathFaceIndex.offsets, &frame.matrix, (struct BattleFxShardTransformBatch *)&frame.meshPosition);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCounter >= 0x1E) {
        if (g_BattleSpecFreezeBreathFaceIndex.faceIndex != 0x50) {
            for (frame.faceLoop = 0; frame.faceLoop < 4; frame.faceLoop++, g_BattleSpecFreezeBreathFaceIndex.faceIndex++) {
                g_BattleSpecFreezeBreathFaceActiveFlags[g_BattleSpecFreezeBreathFaceOrder[g_BattleSpecFreezeBreathFaceIndex.faceIndex]] = 1;
            }
        }
        frame.templateAddr = (int)&g_BattleSpecFreezeBreathTemplateField;
        frame.templateBase = frame.templateAddr + g_BattleSpecFreezeBreathWorkingTemplate;
        frame.templateBase += 4;
        for (frame.faceLoop = 0; frame.faceLoop < 0x50; frame.faceLoop++) {
            if (g_BattleSpecFreezeBreathFaceActiveFlags[frame.faceLoop] == 1) {
                g_BattleSpecFreezeBreathFaceIndex.offsets[frame.faceLoop].y += 0x14;
                if (*(short *)((char *)frame.templateBase + (frame.faceLoop << 3) * 2 + 6) != 0) {
                    *(short *)((char *)frame.templateBase + (frame.faceLoop << 3) * 2 + 6) -= 0x10;
                }
            }
        }
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 0x3C) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the ice mesh of drawRecord face by face, each face moved by its entry of offsetCursor, as
 * flat or textured triangles shaded by the draw record's brightness.
 */
// FUNCTION: FF7 0x0052e502
int __cdecl BattleFx_SpecialFreezeBreathDrawPerFaceOffsetMesh(BattleFxFreezeBreathDrawRecord *drawRecord, int unused, int unused3, BattleFxFreezeBreathDrawPrim *unused4, Vec3sPacked *offsetCursor, void *matrix, struct BattleFxShardTransformBatch *position)
{
    Vec3sPacked *startVertex;
    Vec3sPacked *vertBPointer;
    int modelPtr;
    int faceTotal;
    BattleFxFreezeBreathDrawPrim *prim;
    int curCapFlagsItems[2];
    BattleFxPsxTexturedQuad *curPointer;
    int faceIdx;
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int scratchMtx;
    int numElements;
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    unsigned short primFlagBits;
    Vec3sPacked *localVertexC;
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short brightness;
    BattleFxFaceOffsets *pOffsetBlock;
    BattleFxFreezeBreathDrawPrim *drawState;

    pOffsetBlock = (BattleFxFaceOffsets *)Gfx_GetPsxScratchpadPtr(0xC0);
    scratchMtx = (int)pOffsetBlock + 0x28;
    modelPtr = drawRecord->modelData + 4;
    curPointer = (BattleFxPsxTexturedQuad *)(modelPtr + *(int *)drawRecord->modelData);
    drawState = (BattleFxFreezeBreathDrawPrim *)Gfx_GetPsxPacketScratchPtr();
    faceTotal = curPointer->flags;
    curPointer = (BattleFxPsxTexturedQuad *)((char *)curPointer + 2);
    primFlagBits = curPointer->flags & 0x19F | drawRecord->textureFlags;
    curPointer = (BattleFxPsxTexturedQuad *)((char *)curPointer + 2);
    startVertex = (Vec3sPacked *)(modelPtr + curPointer->flags);
    vertBPointer = (Vec3sPacked *)(modelPtr + curPointer->vertex1);
    localVertexC = (Vec3sPacked *)(modelPtr + curPointer->vertex2);
    for (faceIdx = 0; faceIdx < faceTotal; faceIdx++) {
        pOffsetBlock->centroidX = (startVertex->vx + vertBPointer->vx + localVertexC->vx) / 3;
        pOffsetBlock->centroidY = (startVertex->vy + vertBPointer->vy + localVertexC->vy) / 3;
        pOffsetBlock->centroidZ = (startVertex->vz + vertBPointer->vz + localVertexC->vz) / 3;
        pOffsetBlock->vertex0X = startVertex->vx - pOffsetBlock->centroidX;
        pOffsetBlock->vertex0Y = startVertex->vy - pOffsetBlock->centroidY;
        pOffsetBlock->vertex0Z = startVertex->vz - pOffsetBlock->centroidZ;
        pOffsetBlock->vertex1X = vertBPointer->vx - pOffsetBlock->centroidX;
        pOffsetBlock->vertex1Y = vertBPointer->vy - pOffsetBlock->centroidY;
        pOffsetBlock->vertex1Z = vertBPointer->vz - pOffsetBlock->centroidZ;
        pOffsetBlock->vertex2X = localVertexC->vx - pOffsetBlock->centroidX;
        pOffsetBlock->vertex2Y = localVertexC->vy - pOffsetBlock->centroidY;
        pOffsetBlock->vertex2Z = localVertexC->vz - pOffsetBlock->centroidZ;
        position->posX = pOffsetBlock->centroidX + offsetCursor->vx;
        position->posY = pOffsetBlock->centroidY + offsetCursor->vy;
        position->posZ = pOffsetBlock->centroidZ + offsetCursor->vz;
        offsetCursor = (Vec3sPacked *)((char *)offsetCursor + 8);
        MultiplyMatrix(matrix, position, (void *)scratchMtx);
        SetRotMatrix((void *)scratchMtx);
        SetTransMatrix((void *)scratchMtx);
        RefreshGlobalTransformState();
        StoreDeferredDrawArgs(pOffsetBlock, (char *)pOffsetBlock + 8, (char *)pOffsetBlock + 0x10);
        ApplyDeferredDrawArgs();
        startVertex = (Vec3sPacked *)(modelPtr + curPointer->offsetC);
        vertBPointer = (Vec3sPacked *)(modelPtr + curPointer->offsetD);
        localVertexC = (Vec3sPacked *)(modelPtr + curPointer[1].flags);
        Gfx_PsxStoreClearFlag(curCapFlagsItems);
        if ((curCapFlagsItems[0] & 0x60000) == 0) {
            drawState->tag = 0x7000000;
            Gfx_ReadTransformOrderingDepth2(&numElements);
            drawState->colorWord = curPointer->colorWord;
            drawState->uvWord0 = curPointer->uvWord0;
            drawState->uvWord1 = curPointer->uvWord1;
            drawState->flagBits = primFlagBits;
            if ((drawRecord->drawFlags & 0x80) != 0) {
                brightness = curPointer->vertex3 & 0xFF;
                *(int *)((char *)drawState + 4) = brightness << 0x10 | 0x24000000 | brightness << 8 | brightness;
            } else {
                brightness = drawRecord->shade;
                *(int *)((char *)drawState + 4) = brightness << 0x10 | 0x24000000 | brightness << 8 | brightness;
            }
            drawState->polyCommand |= (char)((drawRecord->drawFlags & 8) != 0 ? 2 : 0);
            Gfx_DrawPsxPrimitive(drawState);
        }
        curPointer = (BattleFxPsxTexturedQuad *)((char *)curPointer + 0x10);
    }
    curPointer = (BattleFxPsxTexturedQuad *)((char *)curPointer + 4);
    faceTotal = curPointer->flags;
    curPointer = (BattleFxPsxTexturedQuad *)((char *)curPointer + 4);
    prim = (BattleFxFreezeBreathDrawPrim *)drawState;
    startVertex = (Vec3sPacked *)(modelPtr + curPointer->flags);
    vertBPointer = (Vec3sPacked *)(modelPtr + curPointer->vertex1);
    localVertexC = (Vec3sPacked *)(modelPtr + curPointer->vertex2);
    for (faceIdx = 0; faceIdx < faceTotal; faceIdx++) {
        pOffsetBlock->centroidX = (startVertex->vx + vertBPointer->vx + localVertexC->vx) / 3;
        pOffsetBlock->centroidY = (startVertex->vy + vertBPointer->vy + localVertexC->vy) / 3;
        pOffsetBlock->centroidZ = (startVertex->vz + vertBPointer->vz + localVertexC->vz) / 3;
        pOffsetBlock->vertex0X = startVertex->vx - pOffsetBlock->centroidX;
        pOffsetBlock->vertex0Y = startVertex->vy - pOffsetBlock->centroidY;
        pOffsetBlock->vertex0Z = startVertex->vz - pOffsetBlock->centroidZ;
        pOffsetBlock->vertex1X = vertBPointer->vx - pOffsetBlock->centroidX;
        pOffsetBlock->vertex1Y = vertBPointer->vy - pOffsetBlock->centroidY;
        pOffsetBlock->vertex1Z = vertBPointer->vz - pOffsetBlock->centroidZ;
        pOffsetBlock->vertex2X = localVertexC->vx - pOffsetBlock->centroidX;
        pOffsetBlock->vertex2Y = localVertexC->vy - pOffsetBlock->centroidY;
        pOffsetBlock->vertex2Z = localVertexC->vz - pOffsetBlock->centroidZ;
        position->posX = pOffsetBlock->centroidX + offsetCursor->vx;
        position->posY = pOffsetBlock->centroidY + offsetCursor->vy;
        position->posZ = pOffsetBlock->centroidZ + offsetCursor->vz;
        offsetCursor = (Vec3sPacked *)((char *)offsetCursor + 8);
        MultiplyMatrix(matrix, position, (void *)scratchMtx);
        SetRotMatrix((void *)scratchMtx);
        SetTransMatrix((void *)scratchMtx);
        RefreshGlobalTransformState();
        StoreDeferredDrawArgs(pOffsetBlock, (char *)pOffsetBlock + 8, (char *)pOffsetBlock + 0x10);
        ApplyDeferredDrawArgs();
        startVertex = (Vec3sPacked *)(modelPtr + curPointer[1].flags);
        vertBPointer = (Vec3sPacked *)(modelPtr + curPointer[1].vertex1);
        localVertexC = (Vec3sPacked *)(modelPtr + curPointer[1].vertex2);
        Gfx_PsxStoreClearFlag(curCapFlagsItems);
        if ((curCapFlagsItems[0] & 0x60000) == 0) {
            prim->tag = 0x6000000;
            Gfx_ReadTransformOrderingDepth2(&numElements);
            Gfx_ShadePsxTriVertexColors((CVECTOR *)((char *)curPointer + 8), (CVECTOR *)((char *)curPointer + 0xC), (CVECTOR *)((char *)curPointer + 0x10), drawRecord->shade, (CVECTOR *)((int)prim + 4), (CVECTOR *)((int)prim + 0xC), (CVECTOR *)((int)prim + 0x14));
            prim->polyCommand = ((drawRecord->drawFlags & 8) != 0 ? 2 : 0) + 0x30;
            Gfx_DrawPsxPrimitive(prim);
        }
        curPointer++;
    }
    return (int)prim;
}

/*
 * Draws the spin model twice, turning both ways and pulsing in width with the frame counter.
 */
// FUNCTION: FF7 0x0052ebe8
void __cdecl BattleFx_SpecialFreezeBreathSpinModelsEmitTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix matrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFreezeBreathSpinModel *spinModel; };
        struct { unsigned char pad_to_2C[0x2C]; short spawnIndex; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxOilTargetModelSlot *emitSlot; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.emitSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (*(int *)&g_BattleSpecFreezeBreathModelReq) = Gfx_ModelTableGetRenderObject(9, g_BattleSpecFreezeBreathLoadedResTable);
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    frame.rotX.vy = frame.emitSlot->frameCounter << 8;
    RotMatrix(&frame.rotX, &frame.matrix);
    frame.scaleX.vy = 0x3000;
    frame.scaleX.vz = (SinFixed(frame.emitSlot->frameCounter << 7) << 0xE) + 0x2000 >> 0xC;
    frame.scaleX.vx = frame.scaleX.vz;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    frame.matrix.t[0] = frame.emitSlot->posX;
    frame.matrix.t[1] = frame.emitSlot->posY;
    frame.matrix.t[2] = frame.emitSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    g_BattleSpecFreezeBreathDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecFreezeBreathModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecFreezeBreathDrawListHead);
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    frame.rotX.vy = -frame.emitSlot->frameCounter << 8;
    RotMatrix(&frame.rotX, &frame.matrix);
    frame.scaleX.vz = SinFixed(frame.emitSlot->frameCounter << 7) * 0x3000 + 0x2000 >> 0xC;
    frame.scaleX.vx = frame.scaleX.vz;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    frame.matrix.t[0] = frame.emitSlot->posX;
    frame.matrix.t[1] = frame.emitSlot->posY;
    frame.matrix.t[2] = frame.emitSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    g_BattleSpecFreezeBreathDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecFreezeBreathModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecFreezeBreathDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.emitSlot->posY += 0xBE;
    for (frame.spawnIndex = 0; frame.spawnIndex < 3; frame.spawnIndex++) {
        frame.spinModel = (BattleFreezeBreathSpinModel *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFreezeBreathOrbitingSpriteTask);
        frame.spinModel->posX = frame.emitSlot->posX;
        frame.spinModel->posZ = frame.emitSlot->posZ;
        frame.spinModel->posY = frame.emitSlot->posY;
        frame.spinModel->phase = (rand() % 4096);
        frame.spinModel->radius = SinFixed(frame.emitSlot->frameCounter << 7) * 0x320 + 0x190 >> 0xC;
    }
    (frame.emitSlot->frameCounter)++;
    if (frame.emitSlot->frameCounter == 0xF) {
        frame.emitSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws an animated sprite orbiting the slot's centre; when unpaused it turns by 0x100 a frame and
 * frees the slot at the sprite's last frame.
 */
// FUNCTION: FF7 0x0052eec2
void __cdecl BattleFx_SpecialFreezeBreathOrbitingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        SVECTOR posX;
        struct { unsigned char pad_to_8[0x8]; BattleFxFreezeBreathOrbitSprite *orbitSprite; };
    } frame;
#pragma pack(pop)

    frame.orbitSprite = (BattleFxFreezeBreathOrbitSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecFreezeBreathScaleVector1.orbitSprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0xB, g_BattleSpecFreezeBreathLoadedResTable);
    g_BattleSpecFreezeBreathScaleVector1.orbitSprite.drawObj = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    frame.posX.vx = frame.orbitSprite->centerX + (CosFixed(frame.orbitSprite->angle) * frame.orbitSprite->radius >> 0xC);
    frame.posX.vz = frame.orbitSprite->centerZ + (SinFixed(frame.orbitSprite->angle) * frame.orbitSprite->radius >> 0xC);
    frame.posX.vy = frame.orbitSprite->posY;
    BattleFx_InstallBillboardMatrix(&frame.posX, 0x2000, 0);
    g_BattleSpecFreezeBreathScaleVector1.orbitSprite.frame = frame.orbitSprite->step;
    g_BattleSpecFreezeBreathDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecFreezeBreathScaleVector1.orbitSprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecFreezeBreathDrawListHead);
    if (g_BattleIsPaused == 0) {
        frame.orbitSprite->posY -= 0x32;
        frame.orbitSprite->angle += 0x100;
        if (g_BattleSpecFreezeBreathScaleVector1.orbitSprite.spriteGroup != 0) {
            (frame.orbitSprite->step)++;
            if (frame.orbitSprite->step == g_BattleSpecFreezeBreathScaleVector1.orbitSprite.spriteGroup->header->frameCount) {
                frame.orbitSprite->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

/*
 * Draws the moving model at the slot and moves it by its velocity; the slot is freed at frame 7.
 */
// FUNCTION: FF7 0x0052f017
void __cdecl BattleFx_SpecialFreezeBreathMovingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix matrix;
        struct { unsigned char pad_to_30[0x30]; BattleFxHellBomberRocketSlot *movingSlot; };
    } frame;
#pragma pack(pop)

    frame.movingSlot = (BattleFxHellBomberRocketSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecFreezeBreathScaleVector3.modelHandle = Gfx_ModelTableGetRenderObject(0, g_BattleSpecFreezeBreathLoadedResTable);
    RotMatrixXYZ(g_BattleSpecFreezeBreathRotAngles2, &frame.matrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecFreezeBreathBasisMatrix, &frame.matrix, &frame.matrix);
    ScaleMatrix(&frame.matrix, (int *)&g_BattleSpecFreezeBreathScaleVector3);
    frame.matrix.t[0] = frame.movingSlot->posX;
    frame.matrix.t[1] = frame.movingSlot->posY;
    frame.matrix.t[2] = frame.movingSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecFreezeBreathDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecFreezeBreathScaleVector3.modelHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecFreezeBreathDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.movingSlot->posX += frame.movingSlot->velocityX;
    frame.movingSlot->posY += frame.movingSlot->velocityY;
    frame.movingSlot->posZ += frame.movingSlot->velocityZ;
    (frame.movingSlot->frameCounter)++;
    if (frame.movingSlot->frameCounter == 7) {
        frame.movingSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
