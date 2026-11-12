// src/ff7/src/battle/battle_fx_chocomog_summon.cpp
// Contents: Runs the ChocoMog summon sequence: swapping the chocobo model part pointers in and out,
//   loading resources, the camera script, the timeline, and the pulsing, orbiting, rising and
//   bone-attached sprite effects.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_summon.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include "ff7/enums/battle.h"
#include "lib/types/polygon.h"
#include "lib/codegen.h"
#include <stdlib.h>
#include "lib/api/sound.h"
#include "lib/api/polygon.h"
extern "C" {

struct GfxRotationAngles2;
// clang-format off
// GLOBAL: FF7 0x00898540
unsigned char g_BattleSummonChocoMogSoundGroup1[16] = {0x4d, 0x02, 0x00, 0x00, 0x4e, 0x02, 0x00, 0x00, 0x4f, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00898550
unsigned char g_BattleSummonChocoMogSoundGroup2[16] = {0x50, 0x02, 0x00, 0x00, 0x51, 0x02, 0x00, 0x00, 0x52, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00898560
unsigned int g_BattleSummonChocoMogSoundGroups[4] = {(unsigned int)&g_BattleSummonChocoMogSoundGroup1, (unsigned int)&g_BattleSummonChocoMogSoundGroup2, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00898570
int g_BattleSummonChocoMogResourceDefs = 0x00192A39;
int g_BattleSummonChocoMogResourceDefsHeadPtr[1] = {
    (int)&g_BattleFxChocomogSummonResourceNames[0x10]
};
int g_BattleSummonChocoMogResourceDefsDataTail[94] = {
    (int)&g_BattleFxChocomogSummonResourceNames[0x20], 0x0, 0x0, 0x192a39, (int)&g_BattleFxChocomogSummonResourceNames[0x30], (int)&g_BattleFxChocomogSummonResourceNames[0x40],
    0x0, 0x0, 0x192a39, (int)&g_BattleFxChocomogSummonResourceNames[0x50], (int)&g_BattleFxChocomogSummonResourceNames[0x60], 0x0,
    0x0, 0x192a39, (int)&g_BattleFxChocomogSummonResourceNames[0x70], (int)&g_BattleFxChocomogSummonResourceNames[0x80], 0x0, 0x0,
    0x192a39, (int)&g_BattleFxChocomogSummonResourceNames[0x90], (int)&g_BattleFxChocomogSummonResourceNames[0xa0], 0x0, 0x0, 0x192a39,
    (int)&g_BattleFxChocomogSummonResourceNames[0xb0], (int)&g_BattleFxChocomogSummonResourceNames[0xc0], 0x0, 0x0, 0x192a39, (int)&g_BattleFxChocomogSummonResourceNames[0xd0],
    (int)&g_BattleFxChocomogSummonResourceNames[0xe0], 0x0, 0x0, 0x192a39, (int)&g_BattleFxChocomogSummonResourceNames[0xf0], (int)&g_BattleFxChocomogSummonResourceNames[0x100],
    0x0, 0x0, 0x192a39, (int)&g_BattleFxChocomogSummonResourceNames[0x110], (int)&g_BattleFxChocomogSummonResourceNames[0x120], 0x0,
    0x0, 0x192a39, (int)&g_BattleFxChocomogSummonResourceNames[0x130], (int)&g_BattleFxChocomogSummonResourceNames[0x140], 0x0, 0x0,
    0x192a39, (int)&g_BattleFxChocomogSummonResourceNames[0x150], (int)&g_BattleFxChocomogSummonResourceNames[0x160], 0x0, 0x0, 0x192a39,
    (int)&g_BattleFxChocomogSummonResourceNames[0x170], (int)&g_BattleFxChocomogSummonResourceNames[0x180], 0x0, 0x0, 0x192a39, (int)&g_BattleFxChocomogSummonResourceNames[0x190],
    (int)&g_BattleFxChocomogSummonResourceNames[0x1a0], 0x0, 0x0, 0x192a39, (int)&g_BattleFxChocomogSummonResourceNames[0x1b0], (int)&g_BattleFxChocomogSummonResourceNames[0x1c0],
    0x0, 0x0, 0x192a39, (int)&g_BattleFxChocomogSummonResourceNames[0x1d0], (int)&g_BattleFxChocomogSummonResourceNames[0x1e0], 0x0,
    0x0, 0x192a39, (int)&g_BattleFxChocomogSummonResourceNames[0x1f0], (int)&g_BattleFxChocomogSummonResourceNames[0x200], 0x0, 0x0,
    0x192a39, (int)&g_BattleFxChocomogSummonResourceNames[0x210], (int)&g_BattleFxChocomogSummonResourceNames[0x220], 0x0, 0x0, 0x192a3c,
    (int)&g_BattleFxChocomogSummonResourceNames[0x22c], (int)&g_BattleFxChocomogSummonResourceNames[0x23c], 0x0, 0x0, 0x192a3c, (int)&g_BattleFxChocomogSummonResourceNames[0x248],
    (int)&g_BattleFxChocomogSummonResourceNames[0x254], 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008986f0
int g_BattleSummonChocoMogScaleValues[2] = {0, 1};
// GLOBAL: FF7 0x008986f8
struct BattleSummonChocoMogEffectDescriptor g_BattleSummonChocoMogEffectDescriptor = {{2, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5, 0, 0, 0, 6, 0, 0, 0, 7, 0, 0, 0, 8, 0, 0, 0, 9, 0, 0, 0, 0xa, 0, 0, 0, 0xb, 0, 0, 0, 0xc, 0, 0, 0, 0xd, 0, 0, 0, 0xe, 0, 0, 0, 0xf, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x00898734
short g_BattleSummonChocoMogOriginZ = -15000;
// GLOBAL: FF7 0x00898738
unsigned char g_BattleSummonChocoMogCameraScript1[48] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0xc5, 0x41, 0x00, 0x00, 0x00, 0x0c, 0xfe, 0x88, 0x13, 0x80, 0x00, 0x18, 0x00, 0x40, 0x00, 0x18, 0x00, 0x86, 0x00, 0x03, 0x00, 0x00, 0x00, 0x46, 0x00, 0x03, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x0c, 0xfe};
unsigned char g_rawData_00898768[112] = {
0x18, 0xfc, 0xe8, 0x03, 0x42, 0x00, 0x14, 0x00, 0xe0, 0xfc, 0x50, 0xfb, 0xb8, 0x0b, 0x42, 0x00,
0x14, 0x00, 0x30, 0xf8, 0x24, 0xfa, 0x88, 0x13, 0x80, 0x00, 0x14, 0x00, 0x40, 0x00, 0x14, 0x00,
0x87, 0x00, 0x47, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x78, 0xec,
0x78, 0xec, 0x78, 0xec, 0x80, 0x00, 0x28, 0x00, 0x40, 0x00, 0x28, 0x00, 0x81, 0x00, 0x00, 0x00,
0x38, 0xff, 0x78, 0xec, 0x41, 0x00, 0x00, 0x00, 0xd4, 0xfe, 0xa8, 0xe4, 0x80, 0x00, 0x32, 0x00,
0x40, 0x00, 0x32, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0xf0, 0xd8,
0x78, 0xec, 0x78, 0xec, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00, 0x0f, 0x00, 0xff, 0xff, 0x00, 0x00};
// GLOBAL: FF7 0x008987d8
unsigned char g_BattleSummonChocoMogCameraScript2[44] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0xc5, 0x41, 0x00, 0x00, 0x00, 0x18, 0xfc, 0x88, 0x13, 0x80, 0x00, 0x18, 0x00, 0x40, 0x00, 0x18, 0x00, 0x86, 0x00, 0x03, 0x00, 0x00, 0x00, 0x46, 0x00, 0x03, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x03};
unsigned char g_rawData_00898804[4] = {0x41, 0x00, 0xe0, 0xfc};
unsigned char g_rawData_00898808[104] = {
0x0c, 0xfe, 0xe8, 0x03, 0x42, 0x00, 0x14, 0x00, 0x90, 0xe8, 0x0c, 0xfe, 0xd0, 0x07, 0x80, 0x00,
0x14, 0x00, 0x40, 0x00, 0x14, 0x00, 0x87, 0x00, 0x47, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x41, 0x00, 0x30, 0xf8, 0x30, 0xf8, 0xa8, 0xe4, 0x80, 0x00, 0x28, 0x00, 0x40, 0x00,
0x28, 0x00, 0x81, 0x00, 0x00, 0x00, 0x38, 0xff, 0x78, 0xec, 0x41, 0x00, 0x00, 0x00, 0xd4, 0xfe,
0xa8, 0xe4, 0x80, 0x00, 0x32, 0x00, 0x40, 0x00, 0x32, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x41, 0x00, 0xf0, 0xd8, 0x78, 0xec, 0x78, 0xec, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00,
0x0f, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00898870
BattleSummonChocoMogSpriteConfig g_BattleSummonChocoMogSpriteConfig = {0, 0, {0x80, 0x80, 0x80, 0x2e}};
// GLOBAL: FF7 0x00898880
BattleSummonChocoMogModelDrawRequest g_BattleSummonChocoMogModelDrawRequest = {{-128, -96, 0, 0, 0xff, 0xbf, 0x80, 0x80, 0x80, {0x2c}, 0x8d, 0x3ff0}};
// GLOBAL: FF7 0x00898898
short g_BattleSummonChocoMogScaleFixed12 = 4096;
// GLOBAL: FF7 0x008988a0
BattleSummonChocoMogTransformMatrix g_BattleSummonChocoMogTransformMatrix = {0x1000, {0, 0, 0, 0, 0, 0, 0, 0x10}, 0, {0}, {0, 0, -128, -128, -128, 0x2c}};
unsigned char g_rawData_008988d0[8] = {0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008988d8
unsigned char g_BattleSummonChocoMogAttachPoint1[8] = {0xce, 0xff, 0x78, 0x00, 0xa6, 0xff, 0x00, 0x00};
// GLOBAL: FF7 0x008988e0
unsigned char g_BattleSummonChocoMogAttachPoint2[8] = {0x32, 0x00, 0x78, 0x00, 0xa6, 0xff, 0x00, 0x00};
// GLOBAL: FF7 0x008988e8
BattleSummonChocoMogVectorConfig g_BattleSummonChocoMogVectorConfig = {{{{0x200, 0, 0}, {0, 0x200, 0}, {0, 0, 0x200}}}};
// GLOBAL: FF7 0x00898908
BattleSummonChocoMogSpriteRequest g_BattleSummonChocoMogSpriteRequest = {{{{0x200, 0, 0}, {0, 0x200, 0}, {0, 0, 0x200}}}, {0}, {0, 0, -128, -128, -128, 0x2c}};
unsigned char g_BattleFxChocomogSummonResourceNames[612] = {
0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x58, 0x31, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x58, 0x32, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x58, 0x33, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x58, 0x34, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x58, 0x35, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x58, 0x36, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x58, 0x37, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x58, 0x38, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x58, 0x39, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x31, 0x30, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x31, 0x31, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x31, 0x32, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x31, 0x33, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x31, 0x34, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x31, 0x35, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x31, 0x36, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00,
0x43, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x30, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
0x53, 0x54, 0x41, 0x52, 0x53, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x43, 0x48, 0x4f, 0x43,
0x4f, 0x42, 0x30, 0x33, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x45, 0x59, 0x45, 0x5f,
0x31, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x42, 0x4f, 0x4f, 0x4d, 0x5f, 0x41, 0x2e, 0x54,
0x49, 0x4d, 0x00, 0x00, 0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x58, 0x31, 0x2e, 0x53, 0x50, 0x54,
0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00b4f9e0
int g_BattleSummonChocoMogPartsTaken = 0;
// GLOBAL: FF7 0x00b4f9e4
int g_BattleSummonChocoMogPartStage = 0;
// GLOBAL: FF7 0x00b4f9e8
int g_BattleSummonChocoMogSavedPartA = 0;
// GLOBAL: FF7 0x00b4f9ec
int g_BattleSummonChocoMogSavedPartB = 0;
// GLOBAL: FF7 0x00b4f9f0
struct BattleSummonChocoMogActorBonePos g_BattleSummonChocoMogActorBonePos = {0};
unsigned char g_bssPad_00b4f9f8[24] = {0};
// GLOBAL: FF7 0x00b4fa10
struct BattleSummonChocoMogCameraTrack g_BattleSummonChocoMogCameraFocus = {0};
// GLOBAL: FF7 0x00b4fa38
struct Vec3sPacked *g_BattleSummonChocoMogOrbitPoint = 0;
// GLOBAL: FF7 0x00b4fa3c
int g_BattleSummonChocoMogBattleTargetMask = 0;
// GLOBAL: FF7 0x00b4fa40
unsigned char g_BattleSummonChocoMogDescentInitFlag = 0;
// GLOBAL: FF7 0x00b4fa44
struct BattleFxChocoMogRotResult *g_BattleSummonChocoMogRotResult = 0;
// GLOBAL: FF7 0x00b4fa48
struct PsxMatrix g_BattleSummonChocoMogLightMatrix = {0};
// GLOBAL: FF7 0x00b4fa68
struct BattleSummonChocoMogCameraTrack *g_BattleSummonChocoMogCameraTrack = 0;
// GLOBAL: FF7 0x00b4fa6c
struct BattleSummonChocoMogCameraTrack *g_BattleSummonChocoMogCameraOther = 0;
// GLOBAL: FF7 0x00b4fa70
struct BattleSummonChocoMogCameraTrack g_BattleSummonChocoMogCameraEye = {0};
// GLOBAL: FF7 0x00b4fa98
unsigned char g_BattleSummonChocoMogOrbitInitFlag = 0;
unsigned char g_bssPad_00b4fa9c[4] = {0};
// GLOBAL: FF7 0x00b4faa0
struct PsxMatrix g_BattleSummonChocoMogTransformMatrix2 = {0};
// GLOBAL: FF7 0x00b4fac0
struct GfxTableCursor *g_BattleSummonChocoMogSpriteTable = 0;
unsigned char g_bssPad_00b4fac4[4] = {0};
// GLOBAL: FF7 0x00b4fac8
struct GfxRotationAngles2 g_BattleSummonChocoMogAngles = {0};
// GLOBAL: FF7 0x00b4fad0
struct PsxMatrix g_BattleSummonChocoMogBillboardMatrix = {0};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// The camera script's variable bank: operands below zero index the shorts from here on.
// clang-format off
#define g_BattleSummonChocoMogOperandTable (*(short *)((char *)&g_BattleSummonChocoMogActorBonePos + 6))
#define g_BattleSummonChocoMogOperands (&g_BattleSummonChocoMogOperandTable)
// clang-format on

// Moves up to two resource pointers from a choco-mog object's model parts into a detach record. Only
// parts of the model header's partList whose counts member is nonzero count. When the record's stage
// is 2, the part's pointer from partPointerTable goes to savedA, its slot address goes to slotA, the
// table entry is cleared, and active is set to 1. At stage 3 the same happens with savedB and slotB.
// stage is incremented once for each counted part. Does nothing if object or its part list is null.
// FUNCTION: FF7 0x005097c0
void __cdecl BattleFx_SummonChocoMogDetachModelPartPointers(BattleFxModelPartDetach *detach, BattleFxChocoMogDetachState *object)
{
    BattleFxModelPartDetach *part;
    int partIndex;
    BattleFxChocoMogModelPart *partPtr;

    part = detach;
    if (object != 0) {
        partPtr = object->modelHeader->partList;
        if (partPtr != 0) {
            for (partIndex = 0; partIndex < object->modelHeader->partCount; partIndex++) {
                if (partPtr->counts != 0) {
                    if (part->stage == 2) {
                        part->savedA = object->partPointerTable[partIndex];
                        part->partSlotA = &object->partPointerTable[partIndex];
                        object->partPointerTable[partIndex] = 0;
                        part->active = 1;
                    }
                    if (part->stage == 3) {
                        part->savedB = object->partPointerTable[partIndex];
                        part->partSlotB = &object->partPointerTable[partIndex];
                        object->partPointerTable[partIndex] = 0;
                        part->active = 1;
                    }
                    (part->stage)++;
                }
                partPtr++;
            }
        }
    }
}

// Undoes a saved two-word patch: when g_BattleSummonChocoMogPartsTaken is nonzero it writes
// g_BattleSummonChocoMogSavedPartA back through g_BattleSummonChocoMogActorBonePos.partSlotA and
// g_BattleSummonChocoMogSavedPartB back through slotB, then clears the flag. Does nothing when the
// flag is already zero, so it is safe to call twice.
// FUNCTION: FF7 0x005098ca
void __cdecl BattleFx_SummonChocoMogRestoreModelPartPointers(void)
{
    if (g_BattleSummonChocoMogPartsTaken != 0) {
        *g_BattleSummonChocoMogActorBonePos.partSlotA = g_BattleSummonChocoMogSavedPartA;
        *g_BattleSummonChocoMogActorBonePos.partSlotB = g_BattleSummonChocoMogSavedPartB;
        g_BattleSummonChocoMogPartsTaken = 0;
    }
}

// When g_BattleSummonChocoMogPartsTaken is zero clears g_BattleSummonChocoMogPartStage and visits the
// model's polygon sets through BattleFx_SummonChocoMogDetachModelPartPointers with context.
// FUNCTION: FF7 0x005098fc
void __cdecl BattleFx_SummonChocoMogTakeModelPartPointers(PolygonModel *context)
{
    int partAnchor;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if (g_BattleSummonChocoMogPartsTaken == 0) {
        g_BattleSummonChocoMogPartStage = 0;
        partAnchor = (int)&g_BattleSummonChocoMogPartsTaken;
        Gfx_ForEachModelPolygonSet(partAnchor, (GfxPolygonSetFn)BattleFx_SummonChocoMogDetachModelPartPointers, context);
    }
}

// When the battle scene exists and the ChocoMog sprite table is not loaded, requests SUMMON/CHOCO
// as resource type 19 using g_BattleSummonChocoMogResourceDefs, and registers the sound groups.
// FUNCTION: FF7 0x00509935
void __cdecl BattleFx_SummonLoadResChocoMog(void)
{
    GfxModelTableBank *auxContext;
    GfxResourceRequest srcRequestBuf;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSummonChocoMogSpriteTable == 0) {
        kind = 0x13;
        resourceDefsWord = (int)&g_BattleSummonChocoMogResourceDefs;
        auxContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &srcRequestBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x00898b9c
        pRequest->resourcePath = BattleFx_BuildResourcePath("SUMMON/CHOCO", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSummonChocoMogSpriteTable, auxContext);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleSummonChocoMogSoundGroups);
    }
}

// Loads ChocoMog resources, clears g_BattleSummonChocoMogEffectDescriptor.moveY and moveX, sets
// g_BattleSummonChocoMogOriginZ to its starting depth (-15000), calls BattleFx_SummonBeginChocoMog
// with both arguments, and returns zero.
// FUNCTION: FF7 0x005099d6
int __cdecl BattleFx_SummonEnterChocoMog(int targetMask, int actorIdx)
{
    BattleFx_SummonLoadResChocoMog();
    g_BattleSummonChocoMogEffectDescriptor.moveY = 0;
    g_BattleSummonChocoMogEffectDescriptor.moveX = g_BattleSummonChocoMogEffectDescriptor.moveY;
    g_BattleSummonChocoMogOriginZ = 0xC568;
    BattleFx_SummonBeginChocoMog(targetMask, actorIdx);
    return 0;
}

// Finds the selected actor-mask XZ bounding-box midpoint, clears the X and Y translation of
// g_BattleSummonChocoMogTransformMatrix2, stores the midpoint Z and turns the Y angle by 0x800 only
// when the midpoint is below the actor's Z. Builds the matrix, schedules
// BattleFx_SummonChocoMogTimelineTask and chooses camera script g_BattleSummonChocoMogCameraScript1
// or g_BattleSummonChocoMogCameraScript2 with rand. The stack layout of midZ and centroid lets the
// centroid output supply the midpoint Z; separately declared C locals do not guarantee this adjacency.
// FUNCTION: FF7 0x00509a10
void __cdecl BattleFx_SummonBeginChocoMog(int targetMask, int actorIdx)
{
    SVECTOR centroid;

    Battle_GetActorMaskCentroid(g_BattleSummonChocoMogBattleTargetMask = targetMask, &centroid.vx);
    g_BattleSummonChocoMogTransformMatrix2.t[1] = 0;
    g_BattleSummonChocoMogTransformMatrix2.t[0] = g_BattleSummonChocoMogTransformMatrix2.t[1];
    g_BattleSummonChocoMogTransformMatrix2.t[2] = centroid.vz;
    if (centroid.vz < g_BattleActorRecords[actorIdx].posZ) {
        g_BattleSummonChocoMogAngles.angleY = 0x800;
    }
    RotMatrix(&g_BattleSummonChocoMogAngles, &g_BattleSummonChocoMogTransformMatrix2);
    Battle_AllocateResourceSlot(BattleFx_SummonChocoMogTimelineTask);
    if ((rand() & 0x100) != 0) {
        BattleFx_ChocoMogSpawnCameraScript(&g_BattleSummonChocoMogCameraScript1, &g_BattleSummonChocoMogTransformMatrix2, actorIdx);
    } else {
        BattleFx_ChocoMogSpawnCameraScript(&g_BattleSummonChocoMogCameraScript2, &g_BattleSummonChocoMogTransformMatrix2, actorIdx);
    }
}

// Allocates a task slot for BattleFx_ChocoMogRunCameraScript and stores in it the script cursor, the
// matrix pointer and, from flags, the actor that attach commands with operand 0 use.
// FUNCTION: FF7 0x00509ad0
void __cdecl BattleFx_ChocoMogSpawnCameraScript(void *scriptCursor, void *auxPtr, int flags)
{
    CameraScriptTask *task;

    task = (CameraScriptTask *)BATTLE_NEW_TASK_SLOT(BattleFx_ChocoMogRunCameraScript);
    task->cursor = (short *)scriptCursor;
    task->matrix = (int)auxPtr;
    task->actor = flags;
}

// Runs the Choco/Mog summon camera script, one operation at a time, unless the battle is paused.
// Each operation is a 16-bit word read from the task's cursor. Bit 0x40 chooses which of the two
// camera tracks it drives (set: the eye, clear: the focus), and the mode is the operation with bits
// 0xC0 removed. On the task's first run both tracks have their frame counts cleared and their actor
// set to -1. The loop keeps reading operations while either track has no frames left, so it ends once
// both are set. The orbit mode keeps its working vectors in data block 0.
// FUNCTION: FF7 0x00509b10
void __cdecl BattleFx_ChocoMogRunCameraScript(void)
{
    int distance;
    CameraScriptTask *taskSlotPtr;
    BattleFxDebrisSpawnTask *ofs;
    int projectedZ;
    int i;
    CameraScriptVector *delta;
    CameraScriptVector *targetVector;
    short opcode;

    taskSlotPtr = (CameraScriptTask *)BATTLE_CURRENT_TASK_SLOT;
    delta = (CameraScriptVector *)Gfx_GetPsxScratchpadPtr(0);
    targetVector = delta + 1;
    ofs = (BattleFxDebrisSpawnTask *)(targetVector + 1);
    if (g_BattleIsPaused == 0) {
        if (taskSlotPtr->initialized == 0) {
            g_BattleSummonChocoMogCameraEye.frames = 0;
            g_BattleSummonChocoMogCameraFocus.frames = 0;
            g_BattleSummonChocoMogCameraEye.actor = -1;
            g_BattleSummonChocoMogCameraFocus.actor = -1;
            taskSlotPtr->initialized = 1;
        }
        while (g_BattleSummonChocoMogCameraEye.frames == 0 || g_BattleSummonChocoMogCameraFocus.frames == 0) {
            opcode = *taskSlotPtr->cursor++;
            if ((opcode & 0x40) != 0) {
                g_BattleSummonChocoMogCameraTrack = &g_BattleSummonChocoMogCameraEye;
                g_BattleSummonChocoMogCameraOther = &g_BattleSummonChocoMogCameraFocus;
            } else {
                g_BattleSummonChocoMogCameraTrack = &g_BattleSummonChocoMogCameraFocus;
                g_BattleSummonChocoMogCameraOther = &g_BattleSummonChocoMogCameraEye;
            }
            g_BattleSummonChocoMogCameraTrack->mode = opcode & ~0xC0;
            switch (g_BattleSummonChocoMogCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                g_BattleSummonChocoMogCameraTrack->frames = BattleFx_ChocoMogReadCameraScriptOperand(&taskSlotPtr->cursor);
                break;
            case SUMMON_CAMERA_SET_POSITION:
                g_BattleSummonChocoMogCameraTrack->position.x = *taskSlotPtr->cursor++;
                g_BattleSummonChocoMogCameraTrack->position.y = *taskSlotPtr->cursor++;
                g_BattleSummonChocoMogCameraTrack->position.z = *taskSlotPtr->cursor++;
                break;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_BattleSummonChocoMogCameraTrack->frames = BattleFx_ChocoMogReadCameraScriptOperand(&taskSlotPtr->cursor);
                g_BattleSummonChocoMogCameraTrack->velocity.x = (*taskSlotPtr->cursor++ - g_BattleSummonChocoMogCameraTrack->position.x) / g_BattleSummonChocoMogCameraTrack->frames;
                g_BattleSummonChocoMogCameraTrack->velocity.y = (*taskSlotPtr->cursor++ - g_BattleSummonChocoMogCameraTrack->position.y) / g_BattleSummonChocoMogCameraTrack->frames;
                g_BattleSummonChocoMogCameraTrack->velocity.z = (*taskSlotPtr->cursor++ - g_BattleSummonChocoMogCameraTrack->position.z) / g_BattleSummonChocoMogCameraTrack->frames;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                delta->x = g_BattleSummonChocoMogCameraOther->position.x - g_BattleSummonChocoMogCameraTrack->position.x;
                delta->y = g_BattleSummonChocoMogCameraOther->position.y - g_BattleSummonChocoMogCameraTrack->position.y;
                delta->z = g_BattleSummonChocoMogCameraOther->position.z - g_BattleSummonChocoMogCameraTrack->position.z;
                g_BattleSummonChocoMogCameraTrack->distance = SqrtToInt(delta->x * delta->x + delta->y * delta->y + delta->z * delta->z);
                g_BattleSummonChocoMogCameraTrack->base = g_BattleSummonChocoMogCameraTrack->position;
                g_BattleSummonChocoMogCameraTrack->frames = BattleFx_ChocoMogReadCameraScriptOperand(&taskSlotPtr->cursor);
                delta->x = *taskSlotPtr->cursor++;
                delta->y = *taskSlotPtr->cursor++;
                delta->z = *taskSlotPtr->cursor++;
                g_BattleSummonChocoMogCameraTrack->velocity.x = (delta->x - g_BattleSummonChocoMogCameraTrack->position.x) / g_BattleSummonChocoMogCameraTrack->frames;
                g_BattleSummonChocoMogCameraTrack->velocity.y = (delta->y - g_BattleSummonChocoMogCameraTrack->position.y) / g_BattleSummonChocoMogCameraTrack->frames;
                g_BattleSummonChocoMogCameraTrack->velocity.z = (delta->z - g_BattleSummonChocoMogCameraTrack->position.z) / g_BattleSummonChocoMogCameraTrack->frames;
                switch (g_BattleSummonChocoMogCameraOther->mode) {
                case SUMMON_CAMERA_MOVE_LINEAR:
                    targetVector->x = g_BattleSummonChocoMogCameraOther->position.x + g_BattleSummonChocoMogCameraOther->velocity.x * g_BattleSummonChocoMogCameraOther->frames;
                    targetVector->y = g_BattleSummonChocoMogCameraOther->position.y + g_BattleSummonChocoMogCameraOther->velocity.y * g_BattleSummonChocoMogCameraOther->frames;
                    targetVector->z = g_BattleSummonChocoMogCameraOther->position.z + g_BattleSummonChocoMogCameraOther->velocity.z * g_BattleSummonChocoMogCameraOther->frames;
                    break;
                default:
                    targetVector->x = g_BattleSummonChocoMogCameraOther->position.x;
                    targetVector->y = g_BattleSummonChocoMogCameraOther->position.y;
                    targetVector->z = g_BattleSummonChocoMogCameraOther->position.z;
                }
                delta->x -= targetVector->x;
                delta->y -= targetVector->y;
                delta->z -= targetVector->z;
                g_BattleSummonChocoMogCameraTrack->delta = (SqrtToInt(delta->x * delta->x + delta->y * delta->y + delta->z * delta->z) - g_BattleSummonChocoMogCameraTrack->distance) / g_BattleSummonChocoMogCameraTrack->frames;
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_BattleSummonChocoMogCameraTrack->frames = BattleFx_ChocoMogReadCameraScriptOperand(&taskSlotPtr->cursor);
                delta->x = *taskSlotPtr->cursor++ - g_BattleSummonChocoMogCameraTrack->position.x;
                delta->y = *taskSlotPtr->cursor++ - g_BattleSummonChocoMogCameraTrack->position.y;
                delta->z = *taskSlotPtr->cursor++ - g_BattleSummonChocoMogCameraTrack->position.z;
                g_BattleSummonChocoMogCameraTrack->base.x = (delta->x - g_BattleSummonChocoMogCameraTrack->velocity.x * g_BattleSummonChocoMogCameraTrack->frames << 1) / (g_BattleSummonChocoMogCameraTrack->frames * g_BattleSummonChocoMogCameraTrack->frames);
                g_BattleSummonChocoMogCameraTrack->base.y = (delta->y - g_BattleSummonChocoMogCameraTrack->velocity.y * g_BattleSummonChocoMogCameraTrack->frames << 1) / (g_BattleSummonChocoMogCameraTrack->frames * g_BattleSummonChocoMogCameraTrack->frames);
                g_BattleSummonChocoMogCameraTrack->base.z = (delta->z - g_BattleSummonChocoMogCameraTrack->velocity.z * g_BattleSummonChocoMogCameraTrack->frames << 1) / (g_BattleSummonChocoMogCameraTrack->frames * g_BattleSummonChocoMogCameraTrack->frames);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_BattleSummonChocoMogCameraTrack->frames = BattleFx_ChocoMogReadCameraScriptOperand(&taskSlotPtr->cursor);
                g_BattleSummonChocoMogCameraTrack->distance = 0;
                g_BattleSummonChocoMogCameraTrack->delta = 0x1000 / g_BattleSummonChocoMogCameraTrack->frames;
                g_BattleSummonChocoMogCameraTrack->base.x = g_BattleSummonChocoMogCameraTrack->position.x;
                g_BattleSummonChocoMogCameraTrack->base.y = g_BattleSummonChocoMogCameraTrack->position.y;
                g_BattleSummonChocoMogCameraTrack->base.z = g_BattleSummonChocoMogCameraTrack->position.z;
                g_BattleSummonChocoMogCameraTrack->velocity.x = *taskSlotPtr->cursor++ - g_BattleSummonChocoMogCameraTrack->position.x;
                g_BattleSummonChocoMogCameraTrack->velocity.y = *taskSlotPtr->cursor++ - g_BattleSummonChocoMogCameraTrack->position.y;
                g_BattleSummonChocoMogCameraTrack->velocity.z = *taskSlotPtr->cursor++ - g_BattleSummonChocoMogCameraTrack->position.z;
                break;
            case SUMMON_CAMERA_ATTACH_BONE:
                if ((g_BattleSummonChocoMogCameraTrack->actor = *taskSlotPtr->cursor++) == 0) {
                    g_BattleSummonChocoMogCameraTrack->actor = taskSlotPtr->actor;
                }
                g_BattleSummonChocoMogCameraTrack->bone = *taskSlotPtr->cursor++;
                break;
            case SUMMON_CAMERA_DETACH:
                g_BattleSummonChocoMogCameraTrack->actor = -1;
                break;
            default:
                taskSlotPtr->active = BATTLE_TASK_FREE;
                return;
            }
        }
        g_BattleSummonChocoMogCameraTrack = &g_BattleSummonChocoMogCameraEye;
        g_BattleSummonChocoMogCameraOther = &g_BattleSummonChocoMogCameraFocus;
        for (i = 0; i < 2; i++) {
            switch (g_BattleSummonChocoMogCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_BattleSummonChocoMogCameraTrack->velocity.x += g_BattleSummonChocoMogCameraTrack->base.x;
                g_BattleSummonChocoMogCameraTrack->velocity.y += g_BattleSummonChocoMogCameraTrack->base.y;
                g_BattleSummonChocoMogCameraTrack->velocity.z += g_BattleSummonChocoMogCameraTrack->base.z;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_BattleSummonChocoMogCameraTrack->position.x += g_BattleSummonChocoMogCameraTrack->velocity.x;
                g_BattleSummonChocoMogCameraTrack->position.y += g_BattleSummonChocoMogCameraTrack->velocity.y;
                g_BattleSummonChocoMogCameraTrack->position.z += g_BattleSummonChocoMogCameraTrack->velocity.z;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                g_BattleSummonChocoMogCameraTrack->base.x += g_BattleSummonChocoMogCameraTrack->velocity.x;
                g_BattleSummonChocoMogCameraTrack->base.y += g_BattleSummonChocoMogCameraTrack->velocity.y;
                g_BattleSummonChocoMogCameraTrack->base.z += g_BattleSummonChocoMogCameraTrack->velocity.z;
                g_BattleSummonChocoMogCameraTrack->distance += g_BattleSummonChocoMogCameraTrack->delta;
                ofs->offsetX = g_BattleSummonChocoMogCameraOther->position.x - g_BattleSummonChocoMogCameraTrack->base.x;
                ofs->offsetY = g_BattleSummonChocoMogCameraOther->position.y - g_BattleSummonChocoMogCameraTrack->base.y;
                ofs->offsetZ = g_BattleSummonChocoMogCameraOther->position.z - g_BattleSummonChocoMogCameraTrack->base.z;
                VectorNormalToSVector((int *)ofs, (short *)delta);
                g_BattleSummonChocoMogCameraTrack->position.x = g_BattleSummonChocoMogCameraOther->position.x - (delta->x * g_BattleSummonChocoMogCameraTrack->distance >> 0xC);
                g_BattleSummonChocoMogCameraTrack->position.y = g_BattleSummonChocoMogCameraOther->position.y - (delta->y * g_BattleSummonChocoMogCameraTrack->distance >> 0xC);
                g_BattleSummonChocoMogCameraTrack->position.z = g_BattleSummonChocoMogCameraOther->position.z - (delta->z * g_BattleSummonChocoMogCameraTrack->distance >> 0xC);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_BattleSummonChocoMogCameraTrack->distance += g_BattleSummonChocoMogCameraTrack->delta;
                distance = g_BattleSummonChocoMogCameraTrack->distance;
                distance = SinFixed(SinFixed(distance / 4) / 4);
                g_BattleSummonChocoMogCameraTrack->position.x = g_BattleSummonChocoMogCameraTrack->base.x + (g_BattleSummonChocoMogCameraTrack->velocity.x * distance >> 0xC);
                g_BattleSummonChocoMogCameraTrack->position.y = g_BattleSummonChocoMogCameraTrack->base.y + (g_BattleSummonChocoMogCameraTrack->velocity.y * distance >> 0xC);
                g_BattleSummonChocoMogCameraTrack->position.z = g_BattleSummonChocoMogCameraTrack->base.z + (g_BattleSummonChocoMogCameraTrack->velocity.z * distance >> 0xC);
            }
            (g_BattleSummonChocoMogCameraTrack->frames)--;
            g_BattleSummonChocoMogCameraTrack = &g_BattleSummonChocoMogCameraFocus;
            g_BattleSummonChocoMogCameraOther = &g_BattleSummonChocoMogCameraEye;
        }
    }
    if (taskSlotPtr->matrix != 0) {
        SetRotMatrix((void *)taskSlotPtr->matrix);
        SetTransMatrix((void *)taskSlotPtr->matrix);
        if (g_BattleSummonChocoMogCameraEye.actor == -1) {
            RotTransPoint(&g_BattleSummonChocoMogCameraEye.position, (int *)ofs, &projectedZ);
            g_BattleCameraPosition.cameraPosX = (short)ofs->offsetX;
            g_BattleCameraPosition.cameraPosY = (short)ofs->offsetY;
            g_BattleCameraPosZ = (short)ofs->offsetZ;
        } else {
            RotateSVectorByInstalledMatrix(&g_BattleSummonChocoMogCameraEye.position, (int *)ofs);
            if (g_BattleSummonChocoMogCameraEye.bone != -1) {
                Battle_GetBoneWorldPos(g_BattleSummonChocoMogCameraEye.actor, g_BattleSummonChocoMogCameraEye.bone, (short *)delta);
            } else {
                delta->x = g_BattleActorRecords[g_BattleSummonChocoMogCameraEye.actor].posX;
                delta->y = g_BattleActorRecords[g_BattleSummonChocoMogCameraEye.actor].posY;
                delta->z = g_BattleActorRecords[g_BattleSummonChocoMogCameraEye.actor].posZ;
            }
            g_BattleCameraPosition.cameraPosX = ofs->offsetX + delta->x;
            g_BattleCameraPosition.cameraPosY = ofs->offsetY + delta->y;
            g_BattleCameraPosZ = ofs->offsetZ + delta->z;
            SetRotMatrix((void *)taskSlotPtr->matrix);
            SetTransMatrix((void *)taskSlotPtr->matrix);
        }
        if (g_BattleSummonChocoMogCameraFocus.actor == -1) {
            RotTransPoint(&g_BattleSummonChocoMogCameraFocus.position, (int *)ofs, &projectedZ);
            g_BattleCameraFocalPoint.focalPointX = (short)ofs->offsetX;
            g_BattleCameraFocalPoint.focalPointY = (short)ofs->offsetY;
            g_BattleCameraFocusZ = (short)ofs->offsetZ;
        } else {
            RotateSVectorByInstalledMatrix(&g_BattleSummonChocoMogCameraFocus.position, (int *)ofs);
            if (g_BattleSummonChocoMogCameraFocus.bone != -1) {
                Battle_GetBoneWorldPos(g_BattleSummonChocoMogCameraFocus.actor, g_BattleSummonChocoMogCameraFocus.bone, (short *)delta);
            } else {
                delta->x = g_BattleActorRecords[g_BattleSummonChocoMogCameraFocus.actor].posX;
                delta->y = g_BattleActorRecords[g_BattleSummonChocoMogCameraFocus.actor].posY;
                delta->z = g_BattleActorRecords[g_BattleSummonChocoMogCameraFocus.actor].posZ;
            }
            g_BattleCameraFocalPoint.focalPointX = ofs->offsetX + delta->x;
            g_BattleCameraFocalPoint.focalPointY = ofs->offsetY + delta->y;
            g_BattleCameraFocusZ = ofs->offsetZ + delta->z;
        }
    } else {
        *(int *)&g_BattleCameraPosition = *(int *)&g_BattleSummonChocoMogCameraEye.position;
        *(int *)&g_BattleCameraPosZ = *(int *)&g_BattleSummonChocoMogCameraEye.position.z;
        *(int *)&g_BattleCameraFocalPoint = *(int *)&g_BattleSummonChocoMogCameraFocus.position;
        *(int *)&g_BattleCameraFocusZ = *(int *)&g_BattleSummonChocoMogCameraFocus.position.z;
    }
}

// Fetches one 16-bit operand through the cursor stored at cursor: reads the short it points to and
// advances the cursor by one short. A negative value is not a literal but an index -- it is replaced
// by element -value of the camera script's variable bank (g_BattleSummonChocoMogOperandTable) -- and the
// resulting short is returned.
// FUNCTION: FF7 0x0050a9a6
short __cdecl BattleFx_ChocoMogReadCameraScriptOperand(short **cursor)
{
    int value;

    value = **cursor;
    (*cursor)++;
    if (value < 0) {
        value = g_BattleSummonChocoMogOperands[-value];
    }
    return (short)value;
}

// Drives the summon: the ring record, the animated, pulsing, rising, orbiting and bone sprite tasks and
// the move-camera task are started at fixed frames, then the targets' typed records are allocated and
// the task frees itself.
// FUNCTION: FF7 0x0050a9e0
void __cdecl BattleFx_SummonChocoMogTimelineTask(void)
{
    int frame;
    BattleFxPositionedSpriteSlot *pSubtask;
    int targetIndex;
    BattleFxRingWaveSlot *pRecord;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleSummonChocoMogTransformMatrix2, &g_BattleSummonChocoMogLightMatrix);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame = task->frameCounter;
    if (frame < 5) {
        if (frame == 4) {
            pRecord = (BattleFxRingWaveSlot *)Battle_AllocSceneMessage(1);
            pRecord->flags = 0;
            pRecord->lifetime = 0;
            pRecord->transZ = g_BattleSummonChocoMogTransformMatrix2.t[2] - 0x3A98;
            pRecord->angleY = g_BattleSummonChocoMogAngles.angleY + 0x800;
        }
    } else {
        frame -= 5;
        if (frame < 0x14) {
            if (frame == 0) {
                pSubtask = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonChocoMogSpawnAnimatedSpritesTask);
                Battle_AllocateResourceSlot(BattleFx_SummonChocoMogMoveCameraTask);
            }
            if (frame == 0) {
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 0);
            }
        } else {
            frame -= 0x14;
            if (frame < 0x14) { /* empty first arm: lib/codegen.h */
            } else {
                frame -= 0x14;
                if (frame < 0x14) { /* empty first arm: lib/codegen.h */
                } else {
                    frame -= 0x14;
                    if (frame < 0x14) {
                        if (frame == 0) {
                            pSubtask = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonChocoMogPulsingSpriteTask);
                            pSubtask = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonChocoMogSpawnRisingSpritesTask);
                        }
                        if (frame == 0) {
                            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 1);
                        }
                    } else {
                        frame -= 0x14;
                        if (frame < 0x19) {
                            if (frame == 0) {
                                pSubtask = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonChocoMogOrbitingSpritesTask);
                                pSubtask->posX = 0x32;
                                pSubtask->posY = 0xFE0C;
                                pSubtask->posZ = 0xEC78;
                                pSubtask = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonChocoMogBoneSpritePairTask);
                            }
                        } else {
                            frame -= 0x19;
                            if (frame < 5) { /* empty first arm: lib/codegen.h */
                            } else {
                                frame -= 5;
                                if (frame < 0x14) {
                                    if (frame == 0x12) {
                                        pRecord = (BattleFxRingWaveSlot *)Battle_AllocSceneMessage(2);
                                    }
                                } else {
                                    frame -= 0x14;
                                    if (frame < 0xF) {
                                        if (frame == 0) {
                                            for (targetIndex = 0; targetIndex < 0xA; targetIndex++) {
                                                if ((g_BattleSummonChocoMogBattleTargetMask & 1 << targetIndex) != 0) {
                                                    Battle_PostActorHitOrHurtMessage(targetIndex);
                                                }
                                            }
                                        }
                                    } else {
                                        task->ownerSlot = BATTLE_TASK_FREE;
                                        Sfx_ReleaseCachedBuffer(0x24D, 0xF);
                                        Sfx_ReleaseCachedBuffer(0x24E, 0xF);
                                        Sfx_ReleaseCachedBuffer(0x24F, 0xF);
                                        Sfx_ReleaseCachedBuffer(0x250, 0xF);
                                        Sfx_ReleaseCachedBuffer(0x251, 0xF);
                                        Sfx_ReleaseCachedBuffer(0x252, 0xF);
                                        BattleFx_SummonChocoMogRestoreModelPartPointers();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    (task->frameCounter)++;
}

// Spawns three animated sprites on each of 60 frames while the battle is not paused, then retires
// the task. Each sprite starts at the point (moveX, moveY, g_BattleSummonChocoMogOriginZ) of the
// effect descriptor. Each sprite gets a random heading, a horizontal speed of 100 to 199 units, a
// vertical velocity of -20 to -49, and a scale from 0x1000 to 0x17FF. The
// fixed-value arms of these choices are never taken, since their condition is the constant zero.
// FUNCTION: FF7 0x0050acda
void __cdecl BattleFx_SummonChocoMogSpawnAnimatedSpritesTask(void)
{
    BattleTaskSlot *pCurrent;
    int heading;
    int speed, spriteIndex;
    BattleFxChocoMogAnimatedSprite *sprite;
    int pick, liftPick, bytesPick;

    pCurrent = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (spriteIndex = 0; spriteIndex < 3; spriteIndex++) {
        sprite = (BattleFxChocoMogAnimatedSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonChocoMogAnimatedSpriteTask);
        sprite->pos.vx = g_BattleSummonChocoMogEffectDescriptor.moveX;
        sprite->pos.vy = g_BattleSummonChocoMogEffectDescriptor.moveY;
        sprite->pos.vz = g_BattleSummonChocoMogOriginZ;
        if (DEAD_IN_RETAIL != 0) {
            pick = 0xC8;
        } else {
            pick = rand() % 0x64 + 0x64;
        }
        speed = pick;
        heading = rand() & 0x7FF;
        sprite->velocity.vx = CosFixed(heading) * speed >> 0xC;
        if (DEAD_IN_RETAIL != 0) {
            liftPick = 0x32;
        } else {
            liftPick = rand() % 0x1E + 0x14;
        }
        sprite->velocity.vy = -liftPick;
        sprite->velocity.vz = -SinFixed(heading) * speed >> 0xC;
        if (DEAD_IN_RETAIL != 0) {
            bytesPick = 0x1800;
        } else {
            bytesPick = rand() % 0x800 + 0x1000;
        }
        sprite->scale = bytesPick;
    }
    (pCurrent->frameCounter)++;
    if (pCurrent->frameCounter >= 0x3C) {
        pCurrent->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame step of one animated ChocoMog sprite. It places the sprite with
// BattleFx_SummonChocoMogInstallSpriteMatrix at its position and scale, builds the sprite request
// g_BattleSummonChocoMogSpriteConfig from Gfx_ModelTableGetLoadedRecord and Gfx_ModelTableGetSpriteObject for
// resource g_BattleSummonChocoMogScaleValues[step] of g_BattleSummonChocoMogSpriteTable,
// and submits it via BattleFx_SubmitSpriteDraw into the ordering table at
// g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, shift 0xC, threading the packet cursor
// g_BattleOverlayDrawListHead. While g_BattleIsPaused is zero it advances the step, retires the task
// once the step reaches 0x10, and otherwise adds the velocity to the position and damps the X and Z
// velocities by 0xE00 >> 12, that is by a factor of 0.875 per frame.
// FUNCTION: FF7 0x0050ae60
void __cdecl BattleFx_SummonChocoMogAnimatedSpriteTask(void)
{
    BattleFxChocoMogSpriteState *task;

    task = (BattleFxChocoMogSpriteState *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_SummonChocoMogInstallSpriteMatrix(&task->pos, (short)task->spriteIndex, 0);
    g_BattleSummonChocoMogSpriteConfig.spriteResource = Gfx_ModelTableGetLoadedRecord(g_BattleSummonChocoMogScaleValues[task->step], g_BattleSummonChocoMogSpriteTable);
    g_BattleSummonChocoMogSpriteConfig.value = Gfx_ModelTableGetSpriteObject(g_BattleSummonChocoMogScaleValues[task->step], g_BattleSummonChocoMogSpriteTable);
    g_BattleOverlayDrawListHead = (int)BattleFx_SubmitSpriteDraw(&g_BattleSummonChocoMogSpriteConfig, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->step)++;
        if (task->step >= 0x10) {
            task->ownerSlot = BATTLE_TASK_FREE;
        } else {
            task->pos.vx += task->velocity.vx;
            task->pos.vy += task->velocity.vy;
            task->pos.vz += task->velocity.vz;
            task->velocity.vx = task->velocity.vx * 0xE00 >> 0xC;
            task->velocity.vz = task->velocity.vz * 0xE00 >> 0xC;
        }
    }
}

// Writes scale into the three diagonal entries of g_BattleSummonChocoMogBillboardMatrix, loads the
// rotation and translation of g_BattleSummonChocoMogLightMatrix, rotates vector into the matrix's
// translation with RotTransPoint (flags into a local), and when offset is non-zero normalises that
// translation and adds unit * offset >> 12 to each of its three ints. Then loads the billboard matrix
// and returns its address.
// FUNCTION: FF7 0x0050afa2
int __cdecl BattleFx_SummonChocoMogInstallSpriteMatrix(Vec3sPacked *vector, short scale, int offset)
{
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int flags;
    int unit[3];

    g_BattleSummonChocoMogBillboardMatrix.m[2][2] = scale;
    g_BattleSummonChocoMogBillboardMatrix.m[1][1] = g_BattleSummonChocoMogBillboardMatrix.m[2][2];
    g_BattleSummonChocoMogBillboardMatrix.m[0][0] = g_BattleSummonChocoMogBillboardMatrix.m[1][1];
    SetRotMatrix(&g_BattleSummonChocoMogLightMatrix);
    SetTransMatrix(&g_BattleSummonChocoMogLightMatrix);
    RotTransPoint(vector, g_BattleSummonChocoMogBillboardMatrix.t, &flags);
    if (offset != 0) {
        NormalizeVectorFixed(g_BattleSummonChocoMogBillboardMatrix.t, unit);
        g_BattleSummonChocoMogBillboardMatrix.t[0] += unit[0] * offset >> 0xC;
        g_BattleSummonChocoMogBillboardMatrix.t[1] += unit[1] * offset >> 0xC;
        g_BattleSummonChocoMogBillboardMatrix.t[2] += unit[2] * offset >> 0xC;
    }
    SetRotMatrix(&g_BattleSummonChocoMogBillboardMatrix);
    SetTransMatrix(&g_BattleSummonChocoMogBillboardMatrix);
    return (int)&g_BattleSummonChocoMogBillboardMatrix;
}

// Selects resource field eighteen and cycles the pulse matrix's X/Y scale over a seven-frame
// triangular phase between 0x1000 and 0x2000. Stores eight times the geometry-screen value as its
// Z translation, installs that matrix and draws a sprite quad before checking pause; when unpaused
// advances and retires at twenty or above.
// FUNCTION: FF7 0x0050b07d
void __cdecl BattleFx_SummonChocoMogPulsingSpriteTask(void)
{
    int cycle;
    BattleFxPhaseControllerTask *task;

    task = (BattleFxPhaseControllerTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSummonChocoMogModelDrawRequest.quad.quadState = Gfx_ModelTableGetSpriteObject(0x12, g_BattleSummonChocoMogSpriteTable);
    cycle = task->frameCount % 7;
    if (cycle < 3) {
        g_BattleSummonChocoMogTransformMatrix.pulseScale = (cycle << 0xC) / 3 + 0x1000;
        g_BattleSummonChocoMogScaleFixed12 = g_BattleSummonChocoMogTransformMatrix.pulseScale;
    } else {
        cycle -= 3;
        g_BattleSummonChocoMogTransformMatrix.pulseScale = cycle * -0x1000 / 4 + 0x2000;
        g_BattleSummonChocoMogScaleFixed12 = g_BattleSummonChocoMogTransformMatrix.pulseScale;
    }
    g_BattleSummonChocoMogTransformMatrix.geomScreenScale = GetGeomScreen() << 3;
    SetRotMatrix(&g_BattleSummonChocoMogScaleFixed12);
    SetTransMatrix(&g_BattleSummonChocoMogScaleFixed12);
    g_BattleOverlayDrawListHead = (int)BattleFx_SubmitSpriteQuadDraw(&g_BattleSummonChocoMogModelDrawRequest.quad, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCount)++;
        if (task->frameCount >= 0x14) {
            task->stage = BATTLE_TASK_FREE;
        }
    }
}

// Moves actor 3 through the descent, one step per frame while the battle is not paused. On the
// first call it sets bit 1 of g_BattleSummonChocoMogDescentInitFlag and stores data block 0 in
// g_BattleSummonChocoMogRotResult, even while paused. Steps 0 to 59 raise
// g_BattleSummonChocoMogOriginZ linearly from -0x3A98 towards zero. At step 79 actor 3's yaw turns
// by 0x400, the initial Y becomes 0xEC78 and the descriptor's moveX becomes 0xFD12. Each step then
// sets the matrices, rotates and translates the descriptor into the target parameter, and copies
// that position to actor 3. At step 130 the task retires by setting ownerSlot to 0xFFFF, before the
// copy and the step advance.
// FUNCTION: FF7 0x0050b1a3
void __cdecl BattleFx_SummonChocoMogMoveCameraTask(void)
{
    int frame;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonChocoMogDescentInitFlag & 1) == 0) {
        g_BattleSummonChocoMogDescentInitFlag |= 1;
        g_BattleSummonChocoMogRotResult = (BattleFxChocoMogRotResult *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame = task->frameCounter;
    if (frame < 0x3C) {
        g_BattleSummonChocoMogOriginZ = frame * 0x3A98 / 0x3C - 0x3A98;
    } else {
        frame -= 0x3C;
        if (frame < 0x14) {
            if (frame == 0x13) {
                g_BattleActorRecords[3].yaw += 0x400;
                g_BattleSummonChocoMogOriginZ = 0xEC78;
                g_BattleSummonChocoMogEffectDescriptor.moveX = 0xFD12;
            }
        } else {
            frame -= 0x14;
            if (frame < 0x32) { /* empty first arm: lib/codegen.h */
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    SetRotMatrix(&g_BattleSummonChocoMogTransformMatrix2);
    SetTransMatrix(&g_BattleSummonChocoMogTransformMatrix2);
    RotTransPoint(&g_BattleSummonChocoMogEffectDescriptor.moveX, (int *)g_BattleSummonChocoMogRotResult, &g_BattleSummonChocoMogRotResult->flags);
    g_BattleActorRecords[3].posX = (short)g_BattleSummonChocoMogRotResult->vx;
    g_BattleActorRecords[3].posY = (short)g_BattleSummonChocoMogRotResult->vy;
    g_BattleActorRecords[3].posZ = (short)g_BattleSummonChocoMogRotResult->vz;
    (task->frameCounter)++;
}

// Lazily acquires data block zero and selects sprite field sixteen, then draws four sprites at
// quarter-turn-separated XZ offsets of radius 150 around the slot position with scale 0x500. When
// unpaused advances and retires at thirty-five or above, otherwise adds 0x40 to the stored angular
// offset.
// FUNCTION: FF7 0x0050b2f7
void __cdecl BattleFx_SummonChocoMogOrbitingSpritesTask(void)
{
    int angle;
    BattleFxChocoMogOrbitSprite *task;
    int spriteIdx;

    task = (BattleFxChocoMogOrbitSprite *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonChocoMogOrbitInitFlag & 1) == 0) {
        g_BattleSummonChocoMogOrbitInitFlag |= 1;
        g_BattleSummonChocoMogOrbitPoint = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonChocoMogTransformMatrix.orbitSprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0x10, g_BattleSummonChocoMogSpriteTable);
    g_BattleSummonChocoMogTransformMatrix.orbitSprite.drawObj = Gfx_ModelTableGetSpriteObject(0x10, g_BattleSummonChocoMogSpriteTable);
    angle = task->phase;
    for (spriteIdx = 0; spriteIdx < 4; spriteIdx++) {
        g_BattleSummonChocoMogOrbitPoint->vx = task->centerX + (SinFixed(angle) * 0x96 >> 0xC);
        g_BattleSummonChocoMogOrbitPoint->vy = task->centerY;
        g_BattleSummonChocoMogOrbitPoint->vz = task->centerZ + (CosFixed(angle) * 0x96 >> 0xC);
        BattleFx_SummonChocoMogInstallSpriteMatrix(g_BattleSummonChocoMogOrbitPoint, 0x500, 0);
        g_BattleOverlayDrawListHead = (int)BattleFx_SubmitSpriteDraw(&g_BattleSummonChocoMogTransformMatrix.orbitSprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
        angle += 0x400;
    }
    if (g_BattleIsPaused == 0) {
        (task->frameCount)++;
        if (task->frameCount >= 0x23) {
            task->ownerSlot = BATTLE_TASK_FREE;
        } else {
            task->phase += 0x40;
        }
    }
}

// Spawns three rising sprites on each frame while the battle is not paused, for 50 frames, then
// retires the task. Each sprite starts at height 0, at a random x from -0x7D0 to 0x7CF and a random
// z from -0xBB8 to -1. Its vertical velocity is -20 to -49, its two other fields are zeroed, and
// its scale is 0x1000 to 0x2FFF. The fixed-value arms of these choices are never taken, since their
// condition is the constant zero. The timer counts the frames, and the task retires when it reaches
// 0x32.
// FUNCTION: FF7 0x0050b483
void __cdecl BattleFx_SummonChocoMogSpawnRisingSpritesTask(void)
{
    int spriteIndex;
    BattleTaskSlot *taskPtr;
    BattleFxChocoMogRisingSprite *sprite;
    int xValue, zPick, liftPick, bytesPick;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (spriteIndex = 0; spriteIndex < 3; spriteIndex++) {
        sprite = (BattleFxChocoMogRisingSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonChocoMogRisingSpriteTask);
        if (DEAD_IN_RETAIL != 0) {
            xValue = 0x7D0;
        } else {
            xValue = rand() % 0xFA0 - 0x7D0;
        }
        sprite->pos.vx = xValue;
        sprite->pos.vy = 0;
        if (DEAD_IN_RETAIL != 0) {
            zPick = 0;
        } else {
            zPick = rand() % 0xBB8 - 0xBB8;
        }
        sprite->pos.vz = zPick;
        sprite->velocity.vx = 0;
        if (DEAD_IN_RETAIL != 0) {
            liftPick = 0x32;
        } else {
            liftPick = rand() % 0x1E + 0x14;
        }
        sprite->velocity.vy = -liftPick;
        sprite->velocity.vz = 0;
        if (DEAD_IN_RETAIL != 0) {
            bytesPick = 0x3000;
        } else {
            bytesPick = rand() % 0x2000 + 0x1000;
        }
        sprite->scale = bytesPick;
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter >= 0x32) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame step for one rising Choco/Mog sprite at the active task slot, with the same shape as
// BattleFx_SummonChocoMogAnimatedSpriteTask and a simpler motion. It places the sprite with
// BattleFx_SummonChocoMogInstallSpriteMatrix at the position and the scale. The rise draw and rise
// aux values come from the scale table entry for the current step, and the draw is submitted into
// the ordering table at overlay draw-list handle + 0x70, shift 0xC, through
// g_BattleOverlayDrawListHead. While the battle is not paused the step advances, and once it
// reaches 0x10 the sprite retires by setting ownerSlot to 0xFFFF. Until then the sprite moves only
// in y, by its velocityY.
// FUNCTION: FF7 0x0050b5f0
void __cdecl BattleFx_SummonChocoMogRisingSpriteTask(void)
{
    BattleFxChocoMogRisingSprite *task;

    task = (BattleFxChocoMogRisingSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_SummonChocoMogInstallSpriteMatrix(&task->pos, (short)task->scale, 0);
    g_BattleSummonChocoMogTransformMatrix.riseSpriteGroup = Gfx_ModelTableGetLoadedRecord(g_BattleSummonChocoMogScaleValues[task->stepIndex], g_BattleSummonChocoMogSpriteTable);
    g_BattleSummonChocoMogTransformMatrix.riseDrawObj = Gfx_ModelTableGetSpriteObject(g_BattleSummonChocoMogScaleValues[task->stepIndex], g_BattleSummonChocoMogSpriteTable);
    g_BattleOverlayDrawListHead = (int)BattleFx_SubmitSpriteDraw(&g_BattleSummonChocoMogTransformMatrix.riseSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->stepIndex)++;
        if (task->stepIndex >= 0x10) {
            task->ownerSlot = BATTLE_TASK_FREE;
        } else {
            task->pos.vy += task->velocity.vy;
        }
    }
}

// Per-frame step that draws a pair of bone-attached sprites. While actor 3's model is set it calls
// BattleFx_SummonChocoMogTakeModelPartPointers with the model's parts. It builds the sprite block from
// resource 0x11 of g_BattleSummonChocoMogSpriteTable, fetches the attach matrix for actor 3 bone 0xD
// with Battle3d_GetActorAttachMatrix, installs it, and transforms the two attach points
// (g_BattleSummonChocoMogAttachPoint1 and 2) with RotTransPoint into the translations of the two
// bone sprite matrices. The sprite frame gets the frame count masked to 3 bits,
// then the same sprite block is submitted twice, once under each of the two matrices
// (g_BattleSummonChocoMogVectorConfig and g_BattleSummonChocoMogSpriteRequest), into the ordering table
// at g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, shift 0. While g_BattleIsPaused is zero the frame count
// advances and the task retires at 0x32.
// FUNCTION: FF7 0x0050b6d6
void __cdecl BattleFx_SummonChocoMogBoneSpritePairTask(void)
{
    BattleFxPhaseControllerTask *pCurrent;
    int projScratch;
    PsxMatrix matrix;

    pCurrent = (BattleFxPhaseControllerTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleActorRecords[3].model != 0) {
        BattleFx_SummonChocoMogTakeModelPartPointers((PolygonModel *)g_BattleActorRecords[3].model->modelParts);
    }
    g_BattleSummonChocoMogSpriteRequest.sprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0x11, g_BattleSummonChocoMogSpriteTable);
    g_BattleSummonChocoMogSpriteRequest.sprite.drawObj = Gfx_ModelTableGetSpriteObject(0x11, g_BattleSummonChocoMogSpriteTable);
    Battle3d_GetActorAttachMatrix(3, 0xD, &matrix);
    SetRotMatrix(&matrix);
    SetTransMatrix(&matrix);
    RotTransPoint(&g_BattleSummonChocoMogAttachPoint1, g_BattleSummonChocoMogVectorConfig.matrix.t, &projScratch);
    RotTransPoint(&g_BattleSummonChocoMogAttachPoint2, g_BattleSummonChocoMogSpriteRequest.matrix.t, &projScratch);
    g_BattleSummonChocoMogSpriteRequest.sprite.frame = pCurrent->frameCount & 7;
    SetRotMatrix(&g_BattleSummonChocoMogVectorConfig.matrix);
    SetTransMatrix(&g_BattleSummonChocoMogVectorConfig.matrix);
    g_BattleOverlayDrawListHead = (int)BattleFx_SubmitSpriteDraw(&g_BattleSummonChocoMogSpriteRequest.sprite, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    SetRotMatrix(&g_BattleSummonChocoMogSpriteRequest.matrix);
    SetTransMatrix(&g_BattleSummonChocoMogSpriteRequest.matrix);
    g_BattleOverlayDrawListHead = (int)BattleFx_SubmitSpriteDraw(&g_BattleSummonChocoMogSpriteRequest.sprite, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (pCurrent->frameCount)++;
        if (pCurrent->frameCount >= 0x32) {
            pCurrent->stage = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
