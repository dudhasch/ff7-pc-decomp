// src/ff7/src/battle/battle_fx_magic_slow.cpp
/* Magic Effect [20]: Slow (Japanese: スロウ, dev tag: "MAGIC/SLOU" / Slou) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Slow magic: blends polygon set vertices toward target
//   configurations and animates the model in a timeline task.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;

// clang-format off
// GLOBAL: FF7 0x008fc600
struct BattleFxSlowTableVertex g_BattleMagicSlow_VertexTable[274] = {{0, 46, -104, 0}, {0, 136, -100, 0}, {0, 222, -93, 0}, {0, 301, -83, 0}, {0, 369, -70, 0}, {0, 425, -55, 0}, {0, 466, -37, 0}, {0, 491, -19, 0}, {0, 500, 0, 0}, {0, 491, 19, 0}, {0, 466, 37, 0}, {0, 425, 55, 0}, {0, 369, 70, 0}, {0, 301, 83, 0}, {0, 222, 93, 0}, {0, 136, 100, 0}, {0, 46, 104, 0}, {17, 42, -104, 0}, {52, 126, -100, 0}, {85, 205, -93, 0}, {115, 278, -83, 0}, {141, 341, -70, 0}, {162, 392, -55, 0}, {178, 430, -37, 0}, {188, 454, -19, 0}, {191, 461, 0, 0}, {188, 454, 19, 0}, {178, 430, 37, 0}, {162, 392, 55, 0}, {141, 341, 70, 0}, {115, 278, 83, 0}, {85, 205, 93, 0}, {52, 126, 100, 0}, {17, 42, 104, 0}, {32, 32, -104, 0}, {96, 96, -100, 0}, {157, 157, -93, 0}, {213, 213, -83, 0}, {261, 261, -70, 0}, {300, 300, -55, 0}, {329, 329, -37, 0}, {347, 347, -19, 0}, {353, 353, 0, 0}, {347, 347, 19, 0}, {329, 329, 37, 0}, {300, 300, 55, 0}, {261, 261, 70, 0}, {213, 213, 83, 0}, {157, 157, 93, 0}, {96, 96, 100, 0}, {32, 32, 104, 0}, {42, 17, -104, 0}, {126, 52, -100, 0}, {205, 85, -93, 0}, {278, 115, -83, 0}, {341, 141, -70, 0}, {392, 162, -55, 0}, {430, 178, -37, 0}, {454, 188, -19, 0}, {461, 191, 0, 0}, {454, 188, 19, 0}, {430, 178, 37, 0}, {392, 162, 55, 0}, {341, 141, 70, 0}, {278, 115, 83, 0}, {205, 85, 93, 0}, {126, 52, 100, 0}, {42, 17, 104, 0}, {46, 0, -104, 0}, {136, 0, -100, 0}, {222, 0, -93, 0}, {301, 0, -83, 0}, {369, 0, -70, 0}, {425, 0, -55, 0}, {466, 0, -37, 0}, {491, 0, -19, 0}, {500, 0, 0, 0}, {491, 0, 19, 0}, {466, 0, 37, 0}, {425, 0, 55, 0}, {369, 0, 70, 0}, {301, 0, 83, 0}, {222, 0, 93, 0}, {136, 0, 100, 0}, {46, 0, 104, 0}, {42, -17, -104, 0}, {126, -52, -100, 0}, {205, -85, -93, 0}, {278, -115, -83, 0}, {341, -141, -70, 0}, {392, -162, -55, 0}, {430, -178, -37, 0}, {454, -188, -19, 0}, {461, -191, 0, 0}, {454, -188, 19, 0}, {430, -178, 37, 0}, {392, -162, 55, 0}, {341, -141, 70, 0}, {278, -115, 83, 0}, {205, -85, 93, 0}, {126, -52, 100, 0}, {42, -17, 104, 0}, {32, -32, -104, 0}, {96, -96, -100, 0}, {157, -157, -93, 0}, {213, -213, -83, 0}, {261, -261, -70, 0}, {300, -300, -55, 0}, {329, -329, -37, 0}, {347, -347, -19, 0}, {353, -353, 0, 0}, {347, -347, 19, 0}, {329, -329, 37, 0}, {300, -300, 55, 0}, {261, -261, 70, 0}, {213, -213, 83, 0}, {157, -157, 93, 0}, {96, -96, 100, 0}, {32, -32, 104, 0}, {17, -42, -104, 0}, {52, -126, -100, 0}, {85, -205, -93, 0}, {115, -278, -83, 0}, {141, -341, -70, 0}, {162, -392, -55, 0}, {178, -430, -37, 0}, {188, -454, -19, 0}, {191, -461, 0, 0}, {188, -454, 19, 0}, {178, -430, 37, 0}, {162, -392, 55, 0}, {141, -341, 70, 0}, {115, -278, 83, 0}, {85, -205, 93, 0}, {52, -126, 100, 0}, {17, -42, 104, 0}, {0, -46, -104, 0}, {0, -136, -100, 0}, {0, -222, -93, 0}, {0, -301, -83, 0}, {0, -369, -70, 0}, {0, -425, -55, 0}, {0, -466, -37, 0}, {0, -491, -19, 0}, {0, -500, 0, 0}, {0, -491, 19, 0}, {0, -466, 37, 0}, {0, -425, 55, 0}, {0, -369, 70, 0}, {0, -301, 83, 0}, {0, -222, 93, 0}, {0, -136, 100, 0}, {0, -46, 104, 0}, {-17, -42, -104, 0}, {-52, -126, -100, 0}, {-85, -205, -93, 0}, {-115, -278, -83, 0}, {-141, -341, -70, 0}, {-162, -392, -55, 0}, {-178, -430, -37, 0}, {-188, -454, -19, 0}, {-191, -461, 0, 0}, {-188, -454, 19, 0}, {-178, -430, 37, 0}, {-162, -392, 55, 0}, {-141, -341, 70, 0}, {-115, -278, 83, 0}, {-85, -205, 93, 0}, {-52, -126, 100, 0}, {-17, -42, 104, 0}, {-32, -32, -104, 0}, {-96, -96, -100, 0}, {-157, -157, -93, 0}, {-213, -213, -83, 0}, {-261, -261, -70, 0}, {-300, -300, -55, 0}, {-329, -329, -37, 0}, {-347, -347, -19, 0}, {-353, -353, 0, 0}, {-347, -347, 19, 0}, {-329, -329, 37, 0}, {-300, -300, 55, 0}, {-261, -261, 70, 0}, {-213, -213, 83, 0}, {-157, -157, 93, 0}, {-96, -96, 100, 0}, {-32, -32, 104, 0}, {-42, -17, -104, 0}, {-126, -52, -100, 0}, {-205, -85, -93, 0}, {-278, -115, -83, 0}, {-341, -141, -70, 0}, {-392, -162, -55, 0}, {-430, -178, -37, 0}, {-454, -188, -19, 0}, {-461, -191, 0, 0}, {-454, -188, 19, 0}, {-430, -178, 37, 0}, {-392, -162, 55, 0}, {-341, -141, 70, 0}, {-278, -115, 83, 0}, {-205, -85, 93, 0}, {-126, -52, 100, 0}, {-42, -17, 104, 0}, {-46, 0, -104, 0}, {-136, 0, -100, 0}, {-222, 0, -93, 0}, {-301, 0, -83, 0}, {-369, 0, -70, 0}, {-425, 0, -55, 0}, {-466, 0, -37, 0}, {-491, 0, -19, 0}, {-500, 0, 0, 0}, {-491, 0, 19, 0}, {-466, 0, 37, 0}, {-425, 0, 55, 0}, {-369, 0, 70, 0}, {-301, 0, 83, 0}, {-222, 0, 93, 0}, {-136, 0, 100, 0}, {-46, 0, 104, 0}, {-42, 17, -104, 0}, {-126, 52, -100, 0}, {-205, 85, -93, 0}, {-278, 115, -83, 0}, {-341, 141, -70, 0}, {-392, 162, -55, 0}, {-430, 178, -37, 0}, {-454, 188, -19, 0}, {-461, 191, 0, 0}, {-454, 188, 19, 0}, {-430, 178, 37, 0}, {-392, 162, 55, 0}, {-341, 141, 70, 0}, {-278, 115, 83, 0}, {-205, 85, 93, 0}, {-126, 52, 100, 0}, {-42, 17, 104, 0}, {-32, 32, -104, 0}, {-96, 96, -100, 0}, {-157, 157, -93, 0}, {-213, 213, -83, 0}, {-261, 261, -70, 0}, {-300, 300, -55, 0}, {-329, 329, -37, 0}, {-347, 347, -19, 0}, {-353, 353, 0, 0}, {-347, 347, 19, 0}, {-329, 329, 37, 0}, {-300, 300, 55, 0}, {-261, 261, 70, 0}, {-213, 213, 83, 0}, {-157, 157, 93, 0}, {-96, 96, 100, 0}, {-32, 32, 104, 0}, {-17, 42, -104, 0}, {-52, 126, -100, 0}, {-85, 205, -93, 0}, {-115, 278, -83, 0}, {-141, 341, -70, 0}, {-162, 392, -55, 0}, {-178, 430, -37, 0}, {-188, 454, -19, 0}, {-191, 461, 0, 0}, {-188, 454, 19, 0}, {-178, 430, 37, 0}, {-162, 392, 55, 0}, {-141, 341, 70, 0}, {-115, 278, 83, 0}, {-85, 205, 93, 0}, {-52, 126, 100, 0}, {-17, 42, 104, 0}, {0, 0, 104, 0}, {0, 0, -104, 0}};
// GLOBAL: FF7 0x008fce90
struct BattleFxSlowTableVertex g_BattleMagicSlow_VertexBase[274] = {{0, 46, -104, 0}, {0, 136, -100, 0}, {0, 222, -93, 0}, {0, 301, -83, 0}, {0, 369, -70, 0}, {0, 425, -55, 0}, {0, 466, -37, 0}, {0, 491, -19, 0}, {0, 500, 0, 0}, {0, 491, 19, 0}, {0, 466, 37, 0}, {0, 425, 55, 0}, {0, 369, 70, 0}, {0, 301, 83, 0}, {0, 222, 93, 0}, {0, 136, 100, 0}, {0, 46, 104, 0}, {22, 42, -104, 0}, {67, 126, -100, 0}, {109, 205, -93, 0}, {147, 278, -83, 0}, {181, 341, -70, 0}, {208, 392, -55, 0}, {228, 430, -37, 0}, {241, 454, -19, 0}, {245, 461, 0, 0}, {241, 454, 19, 0}, {228, 430, 37, 0}, {208, 392, 55, 0}, {181, 341, 70, 0}, {147, 278, 83, 0}, {109, 205, 93, 0}, {67, 126, 100, 0}, {22, 42, 104, 0}, {56, 32, -104, 0}, {167, 96, -100, 0}, {272, 157, -93, 0}, {368, 213, -83, 0}, {451, 261, -70, 0}, {519, 300, -55, 0}, {570, 329, -37, 0}, {600, 347, -19, 0}, {611, 353, 0, 0}, {600, 347, 19, 0}, {570, 329, 37, 0}, {519, 300, 55, 0}, {451, 261, 70, 0}, {368, 213, 83, 0}, {272, 157, 93, 0}, {167, 96, 100, 0}, {56, 32, 104, 0}, {54, 17, -104, 0}, {162, 52, -100, 0}, {264, 85, -93, 0}, {357, 115, -83, 0}, {438, 141, -70, 0}, {504, 162, -55, 0}, {552, 178, -37, 0}, {582, 188, -19, 0}, {592, 191, 0, 0}, {582, 188, 19, 0}, {552, 178, 37, 0}, {504, 162, 55, 0}, {438, 141, 70, 0}, {357, 115, 83, 0}, {264, 85, 93, 0}, {162, 52, 100, 0}, {54, 17, 104, 0}, {94, 0, -104, 0}, {279, 0, -100, 0}, {455, 0, -93, 0}, {615, 0, -83, 0}, {754, 0, -70, 0}, {868, 0, -55, 0}, {952, 0, -37, 0}, {1003, 0, -19, 0}, {1021, 0, 0, 0}, {1003, 0, 19, 0}, {952, 0, 37, 0}, {868, 0, 55, 0}, {754, 0, 70, 0}, {615, 0, 83, 0}, {455, 0, 93, 0}, {279, 0, 100, 0}, {94, 0, 104, 0}, {54, -17, -104, 0}, {162, -52, -100, 0}, {264, -85, -93, 0}, {357, -115, -83, 0}, {438, -141, -70, 0}, {504, -162, -55, 0}, {552, -178, -37, 0}, {582, -188, -19, 0}, {592, -191, 0, 0}, {582, -188, 19, 0}, {552, -178, 37, 0}, {504, -162, 55, 0}, {438, -141, 70, 0}, {357, -115, 83, 0}, {264, -85, 93, 0}, {162, -52, 100, 0}, {54, -17, 104, 0}, {56, -32, -104, 0}, {167, -96, -100, 0}, {272, -157, -93, 0}, {368, -213, -83, 0}, {451, -261, -70, 0}, {519, -300, -55, 0}, {570, -329, -37, 0}, {600, -347, -19, 0}, {611, -353, 0, 0}, {600, -347, 19, 0}, {570, -329, 37, 0}, {519, -300, 55, 0}, {451, -261, 70, 0}, {368, -213, 83, 0}, {272, -157, 93, 0}, {167, -96, 100, 0}, {56, -32, 104, 0}, {22, -42, -104, 0}, {67, -126, -100, 0}, {109, -205, -93, 0}, {147, -278, -83, 0}, {181, -341, -70, 0}, {208, -392, -55, 0}, {228, -430, -37, 0}, {241, -454, -19, 0}, {245, -461, 0, 0}, {241, -454, 19, 0}, {228, -430, 37, 0}, {208, -392, 55, 0}, {181, -341, 70, 0}, {147, -278, 83, 0}, {109, -205, 93, 0}, {67, -126, 100, 0}, {22, -42, 104, 0}, {0, -46, -104, 0}, {0, -136, -100, 0}, {0, -222, -93, 0}, {0, -301, -83, 0}, {0, -369, -70, 0}, {0, -425, -55, 0}, {0, -466, -37, 0}, {0, -491, -19, 0}, {0, -500, 0, 0}, {0, -491, 19, 0}, {0, -466, 37, 0}, {0, -425, 55, 0}, {0, -369, 70, 0}, {0, -301, 83, 0}, {0, -222, 93, 0}, {0, -136, 100, 0}, {0, -46, 104, 0}, {-22, -42, -104, 0}, {-67, -126, -100, 0}, {-109, -205, -93, 0}, {-147, -278, -83, 0}, {-181, -341, -70, 0}, {-208, -392, -55, 0}, {-228, -430, -37, 0}, {-241, -454, -19, 0}, {-245, -461, 0, 0}, {-241, -454, 19, 0}, {-228, -430, 37, 0}, {-208, -392, 55, 0}, {-181, -341, 70, 0}, {-147, -278, 83, 0}, {-109, -205, 93, 0}, {-67, -126, 100, 0}, {-22, -42, 104, 0}, {-56, -32, -104, 0}, {-167, -96, -100, 0}, {-272, -157, -93, 0}, {-368, -213, -83, 0}, {-451, -261, -70, 0}, {-519, -300, -55, 0}, {-570, -329, -37, 0}, {-600, -347, -19, 0}, {-611, -353, 0, 0}, {-600, -347, 19, 0}, {-570, -329, 37, 0}, {-519, -300, 55, 0}, {-451, -261, 70, 0}, {-368, -213, 83, 0}, {-272, -157, 93, 0}, {-167, -96, 100, 0}, {-56, -32, 104, 0}, {-54, -17, -104, 0}, {-162, -52, -100, 0}, {-264, -85, -93, 0}, {-357, -115, -83, 0}, {-438, -141, -70, 0}, {-504, -162, -55, 0}, {-552, -178, -37, 0}, {-582, -188, -19, 0}, {-592, -191, 0, 0}, {-582, -188, 19, 0}, {-552, -178, 37, 0}, {-504, -162, 55, 0}, {-438, -141, 70, 0}, {-357, -115, 83, 0}, {-264, -85, 93, 0}, {-162, -52, 100, 0}, {-54, -17, 104, 0}, {-94, 0, -104, 0}, {-279, 0, -100, 0}, {-455, 0, -93, 0}, {-615, 0, -83, 0}, {-754, 0, -70, 0}, {-868, 0, -55, 0}, {-952, 0, -37, 0}, {-1003, 0, -19, 0}, {-1021, 0, 0, 0}, {-1003, 0, 19, 0}, {-952, 0, 37, 0}, {-868, 0, 55, 0}, {-754, 0, 70, 0}, {-615, 0, 83, 0}, {-455, 0, 93, 0}, {-279, 0, 100, 0}, {-94, 0, 104, 0}, {-54, 17, -104, 0}, {-162, 52, -100, 0}, {-264, 85, -93, 0}, {-357, 115, -83, 0}, {-438, 141, -70, 0}, {-504, 162, -55, 0}, {-552, 178, -37, 0}, {-582, 188, -19, 0}, {-592, 191, 0, 0}, {-582, 188, 19, 0}, {-552, 178, 37, 0}, {-504, 162, 55, 0}, {-438, 141, 70, 0}, {-357, 115, 83, 0}, {-264, 85, 93, 0}, {-162, 52, 100, 0}, {-54, 17, 104, 0}, {-56, 32, -104, 0}, {-167, 96, -100, 0}, {-272, 157, -93, 0}, {-368, 213, -83, 0}, {-451, 261, -70, 0}, {-519, 300, -55, 0}, {-570, 329, -37, 0}, {-600, 347, -19, 0}, {-611, 353, 0, 0}, {-600, 347, 19, 0}, {-570, 329, 37, 0}, {-519, 300, 55, 0}, {-451, 261, 70, 0}, {-368, 213, 83, 0}, {-272, 157, 93, 0}, {-167, 96, 100, 0}, {-56, 32, 104, 0}, {-22, 42, -104, 0}, {-67, 126, -100, 0}, {-109, 205, -93, 0}, {-147, 278, -83, 0}, {-181, 341, -70, 0}, {-208, 392, -55, 0}, {-228, 430, -37, 0}, {-241, 454, -19, 0}, {-245, 461, 0, 0}, {-241, 454, 19, 0}, {-228, 430, 37, 0}, {-208, 392, 55, 0}, {-181, 341, 70, 0}, {-147, 278, 83, 0}, {-109, 205, 93, 0}, {-67, 126, 100, 0}, {-22, 42, 104, 0}, {0, 0, 104, 0}, {0, 0, -104, 0}};
// GLOBAL: FF7 0x008fd720
int g_BattleMagicSlow_ResourceDefs = 0x003200B4;
int g_BattleMagicSlow_ResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleMagicSlow_ModelReq2 + 0x10)
};
int g_BattleMagicSlow_ResourceDefsDataTail[18] = {
    0x0, 0x0, 0x0, 0x3200b1, (int)((char *)&g_BattleMagicSlow_ModelReq2 + 0x1c), 0x0,
    0x0, 0x0, 0x3200b4, (int)((char *)&g_BattleMagicSlow_ModelReq2 + 0x28), 0x0, 0x0,
    0x0, 0x3200b1, (int)((char *)&g_BattleMagicSlow_ModelReq2 + 0x34), 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008fd770
struct BattleMagicSlow_ModelReq1 g_BattleMagicSlow_ModelReq1 = {0, 136, {0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008fd780
struct BattleMagicSlow_ModelReq2 g_BattleMagicSlow_ModelReq2 = {0, 136, {0}, 0, {0x20, 0, 0, 0, 0x53, 0x4c, 0x4f, 0x55, 0x31, 0x2e, 0x52, 0x53, 0x44, 0, 0, 0, 0x53, 0x4c, 0x4f, 0x55, 0x31, 0x2e, 0x52, 0x53, 0x44, 0, 0, 0, 0x48, 0x41, 0x52, 0x49, 0x2e, 0x52, 0x53, 0x44, 0, 0, 0, 0, 0x48, 0x41, 0x52, 0x49, 0x2e, 0x52, 0x53, 0x44, 0, 0, 0, 0}};
// GLOBAL: FF7 0x00be04a0
int g_BattleMagicSlow_ChainTail = 0;
// GLOBAL: FF7 0x00be04a4
short g_BattleMagicSlow_TargetMask = 0;
unsigned char g_bssPad_00be04a8[2192] = {0};
// GLOBAL: FF7 0x00be0d38
struct BattleSpecialTyokyuBasisMatrix g_BattleMagicSlow_LookBasis = {0};
// GLOBAL: FF7 0x00be0d58
struct BattleMagicSlow_Centroid g_BattleMagicSlow_Centroid = {0};
// GLOBAL: FF7 0x00be0d5c
short g_BattleMagicSlow_CentroidZ = 0;
// GLOBAL: FF7 0x00be0d60
struct GfxTableCursor *g_BattleMagicSlow_ResourceHandle = 0;
// GLOBAL: FF7 0x00be0d64
int g_BattleMagicSlow_BlendPhase1 = 0;
// GLOBAL: FF7 0x00be0d68
int g_BattleMagicSlow_BlendPhase2 = 0;
unsigned char g_bssPad_00be0d6c[4] = {0};
// GLOBAL: FF7 0x00be0d70
short g_BattleMagicSlow_Angles[3] = {0};
// clang-format on

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/* Loads the Slow magic resource package if not already loaded. */
// FUNCTION: FF7 0x005b4d80
void __cdecl BattleFx_MagicLoadResSlow(void)
{
    GfxModelTableBank *baseOffset;
    GfxResourceRequest requestRecord;
    int kind;
    char pathEnd[200];
    GfxResourceRequest *request;
    int resultDefCatalog;

    if (g_pBattle3dScene != 0 && g_BattleMagicSlow_ResourceHandle == 0) {
        kind = 4;
        resultDefCatalog = (int)&g_BattleMagicSlow_ResourceDefs;
        baseOffset = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008fd7c0
        request->resourcePath = BattleFx_BuildResourcePath("MAGIC/SLOU", pathEnd);
        Gfx_LoadModelTableIfAbsent(kind, resultDefCatalog, request, &g_BattleMagicSlow_ResourceHandle, baseOffset);
    }
}

/* Entry point for the Slow magic effect. */
// FUNCTION: FF7 0x005b4e10
void __cdecl BattleFx_MagicEnterSlow(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResSlow();
    BattleFx_MagicBeginSlow(targetMask, actorIdx);
}

/* Blends polygon set vertex positions toward target configurations for Slow. */
// FUNCTION: FF7 0x005b4e2a
void __cdecl BattleFx_MagicSlowBlendPolygonSetVertices(int vertexIndex, BattleFxSlowBlendVertex *sourceVertex, BattleFxSlowBlendVertex *blendVertex)
{
    BattleFxSlowTableVertex *baseVert;
    float phaseWeight;
    BattleFxSlowTableVertex *targetVertex;
    float nextBlendWeight;
    int tgtY;
    int baseX;
    int targetX;
    int tgtZ;
    int baseY;
    int baseZ;

    phaseWeight = g_BattleMagicSlow_BlendPhase1 / 3e+01f;
    nextBlendWeight = g_BattleMagicSlow_BlendPhase2 / 15.0f;
    targetVertex = &g_BattleMagicSlow_VertexTable[vertexIndex];
    baseVert = &g_BattleMagicSlow_VertexBase[vertexIndex];
    targetX = targetVertex->vx;
    blendVertex->x = (targetX - sourceVertex->x) * phaseWeight + sourceVertex->x + ((baseX = baseVert->vx) - blendVertex->x) * nextBlendWeight;
    tgtY = targetVertex->vy;
    blendVertex->y = (tgtY - sourceVertex->y) * phaseWeight + sourceVertex->y + ((baseY = baseVert->vy) - blendVertex->y) * nextBlendWeight;
    tgtZ = targetVertex->vz;
    blendVertex->z = (tgtZ - sourceVertex->z) * phaseWeight + sourceVertex->z + ((baseZ = baseVert->vz) - blendVertex->z) * nextBlendWeight;
}

/* Initializes Slow effect parameters and creates timeline task. */
// FUNCTION: FF7 0x005b4f14
void __cdecl BattleFx_MagicBeginSlow(int targetMask, int actorIdx)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int tableEntry;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix transposedMatrix; };
        struct { unsigned char pad_to_28[0x28]; VECTOR screenX; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxSpecialEnergyOrbSlot *particle; };
        struct { unsigned char pad_to_44[0x44]; SVECTOR dirX; };
        struct { unsigned char pad_to_50[0x50]; int screenDepth; };
        struct { unsigned char pad_to_54[0x54]; SVECTOR offsetX; };
    } frame;
#pragma pack(pop)

    g_BattleMagicSlow_BlendPhase1 = 0;
    g_BattleMagicSlow_BlendPhase2 = 0;
    frame.tableEntry = Gfx_ModelTableGetRenderObject(0, g_BattleMagicSlow_ResourceHandle);
    if (frame.tableEntry != 0) {
        Gfx_ForEachPolygonSetPart((DxGroupPartCallback)BattleFx_MagicSlowBlendPolygonSetVertices, *(int *)((char *)frame.tableEntry + 4));
    }
    g_BattleMagicSlow_TargetMask = targetMask;
    Battle_GetActorMaskCentroid(targetMask, (short *)&g_BattleMagicSlow_Centroid);
    frame.particle = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicSlowTimelineTask);
    RotTransPoint((short *)&g_BattleMagicSlow_Centroid, &frame.screenX.vx, &frame.screenDepth);
    if (frame.screenX.vx < 0) {
        frame.offsetX.vx = 0x7D0;
    } else {
        frame.offsetX.vx = 0xF830;
    }
    if (frame.screenX.vy < 0) {
        frame.offsetX.vy = 0x3E8;
    } else {
        frame.offsetX.vy = 0xFC18;
    }
    frame.offsetX.vx -= *(short *)g_BattleCameraTransform.translation;
    frame.offsetX.vy -= *(short *)&g_BattleCameraTransform.translation[1];
    frame.offsetX.vz = -(g_BattleCameraTransform.translation[2]);
    TransposeMatrix((void *)g_BattleCameraTransform.matrix, &frame.transposedMatrix);
    TransformVectorToInt16(&frame.transposedMatrix, &frame.offsetX, &frame.particle->posX);
    frame.particle->velocityX = (g_BattleMagicSlow_Centroid.targetX - frame.particle->posX) / 0x1E;
    frame.dirX.vx = frame.particle->velocityX;
    frame.particle->velocityY = (g_BattleMagicSlow_Centroid.y2 - 0x3E8 - frame.particle->posY) / 0x1E;
    frame.dirX.vy = frame.particle->velocityY;
    frame.particle->velocityZ = (g_BattleMagicSlow_CentroidZ - frame.particle->posZ) / 0x1E;
    frame.dirX.vz = frame.particle->velocityZ;
    Gte_BuildLookBasisFromDirection(&frame.dirX.vx, (short *)&g_BattleMagicSlow_LookBasis);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x41);
}

/* Manages the timeline and animates model rendering for Slow. */
// FUNCTION: FF7 0x005b50d2
void __cdecl BattleFx_MagicSlowTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[160];
        int vecBX;
        struct { unsigned char pad_to_4[0x4]; int vecBY; };
        struct { unsigned char pad_to_8[0x8]; int vecBZ; };
        struct { unsigned char pad_to_10[0x10]; int vecAX; };
        struct { unsigned char pad_to_14[0x14]; int vecAY; };
        struct { unsigned char pad_to_18[0x18]; int vecAZ; };
        struct { unsigned char pad_to_20[0x20]; int blend; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix basisAX; };
        struct { unsigned char pad_to_44[0x44]; int drawFlags; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_6C[0x6C]; int actorIndex; };
        struct { unsigned char pad_to_70[0x70]; SVECTOR angleX; };
        struct { unsigned char pad_to_78[0x78]; BattleFxSlowParticle *particle; };
        struct { unsigned char pad_to_7C[0x7C]; int timer; };
        struct { unsigned char pad_to_80[0x80]; VECTOR scaleX; };
        struct { unsigned char pad_to_90[0x90]; short crossX; };
        struct { unsigned char pad_to_94[0x94]; short crossY; };
        struct { unsigned char pad_to_98[0x98]; short crossZ; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.particle = (BattleFxSlowParticle *)BATTLE_CURRENT_TASK_SLOT;
    frame.timer = frame.particle->timer;
    if (frame.timer < 0x1E) {
        if (g_BattleIsPaused == 0) {
            g_BattleMagicSlow_BlendPhase1 = frame.timer + 1;
            frame.particle->posX += frame.particle->velocityX;
            frame.particle->posY += frame.particle->velocityY;
            frame.particle->posZ += frame.particle->velocityZ;
        }
        frame.blend = 0;
        frame.drawFlags = 0;
        frame.scaleX.vz = 0x1000;
        frame.scaleX.vy = frame.scaleX.vz;
        frame.scaleX.vx = frame.scaleX.vy;
    } else {
        frame.timer -= 0x1E;
        if (frame.timer < 0xF) {
            frame.blend = 0;
            frame.drawFlags = 0;
            frame.scaleX.vz = 0x1000;
            frame.scaleX.vy = frame.scaleX.vz;
            frame.scaleX.vx = frame.scaleX.vy;
        } else {
            frame.timer -= 0xF;
            if (frame.timer < 0xF) {
                if (g_BattleIsPaused == 0) {
                    if (frame.timer == 0) {
                        for (frame.actorIndex = 0; frame.actorIndex < 0xA; frame.actorIndex++) {
                            if ((g_BattleMagicSlow_TargetMask & 1 << frame.actorIndex) != 0) {
                                Battle_PostActorHitOrHurtMessage(frame.actorIndex);
                            }
                        }
                        g_BattleMagicSlow_Angles[2] = 0;
                        g_BattleMagicSlow_Angles[1] = g_BattleMagicSlow_Angles[2];
                        g_BattleMagicSlow_Angles[0] = g_BattleMagicSlow_Angles[1];
                    }
                    g_BattleMagicSlow_BlendPhase2 = frame.timer + 1;
                    g_BattleMagicSlow_Angles[0] += 0x104;
                    g_BattleMagicSlow_Angles[1] += 0x87;
                }
                frame.blend = (frame.timer << 0xC) / 0xF;
                frame.drawFlags = 8;
                frame.scaleX.vz = frame.timer * 0x3000 / 0xF + 0x1000;
                frame.scaleX.vy = frame.scaleX.vz;
                frame.scaleX.vx = frame.scaleX.vy;
            } else {
                frame.particle->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleMagicSlow_LookBasis.posX18 = frame.particle->posX;
    g_BattleMagicSlow_LookBasis.posY22 = frame.particle->posY;
    g_BattleMagicSlow_LookBasis.posZ26 = frame.particle->posZ;
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, (int *)&g_BattleMagicSlow_LookBasis, &frame.basisAX);
    frame.timer = frame.particle->timer;
    if (frame.timer < 0x1E) { /* empty first arm: lib/codegen.h */
    } else {
        frame.timer -= 0x1E;
        if (frame.timer < 0xF) {
            frame.vecAX = ((frame.timer << 0xC) + frame.basisAX.m[0][0] * (0xF - frame.timer)) / 0xF;
            frame.vecAY = frame.basisAX.m[1][0] * (0xF - frame.timer) / 0xF;
            frame.vecAZ = frame.basisAX.m[2][0] * (0xF - frame.timer) / 0xF;
            NormalizeVectorFixed(&frame.vecAX, &frame.vecAX);
            frame.basisAX.m[0][0] = *(short *)&frame.vecAX;
            frame.basisAX.m[1][0] = *(short *)&frame.vecAY;
            frame.basisAX.m[2][0] = *(short *)&frame.vecAZ;
            frame.vecBX = frame.basisAX.m[0][1] * (0xF - frame.timer) / 0xF;
            frame.vecBY = frame.basisAX.m[1][1];
            frame.vecBZ = frame.basisAX.m[2][1];
            NormalizeVectorFixed(&frame.vecBX, &frame.vecBX);
            frame.basisAX.m[0][1] = *(short *)&frame.vecBX;
            frame.basisAX.m[1][1] = *(short *)&frame.vecBY;
            frame.basisAX.m[2][1] = *(short *)&frame.vecBZ;
            CrossProductFixed12((VECTOR *)&frame.vecAX, (VECTOR *)&frame.vecBX, (VECTOR *)&frame.crossX);
            NormalizeVectorFixed(&frame.crossX, &frame.crossX);
            frame.basisAX.m[0][2] = frame.crossX;
            frame.basisAX.m[1][2] = frame.crossY;
            frame.basisAX.m[2][2] = frame.crossZ;
        } else {
            frame.basisAX.m[0][0] = 0x1000;
            frame.vecAX = frame.basisAX.m[0][0];
            frame.basisAX.m[2][0] = 0;
            frame.vecAZ = frame.basisAX.m[2][0];
            frame.basisAX.m[1][0] = *(short *)&frame.vecAZ;
            frame.vecAY = frame.basisAX.m[1][0];
            frame.vecBX = 0;
            frame.vecBY = frame.basisAX.m[1][1];
            frame.vecBZ = frame.basisAX.m[2][1];
            NormalizeVectorFixed(&frame.vecBX, &frame.vecBX);
            frame.basisAX.m[0][1] = *(short *)&frame.vecBX;
            frame.basisAX.m[1][1] = *(short *)&frame.vecBY;
            frame.basisAX.m[2][1] = *(short *)&frame.vecBZ;
            CrossProductFixed12((VECTOR *)&frame.vecAX, (VECTOR *)&frame.vecBX, (VECTOR *)&frame.crossX);
            NormalizeVectorFixed(&frame.crossX, &frame.crossX);
            frame.basisAX.m[0][2] = frame.crossX;
            frame.basisAX.m[1][2] = frame.crossY;
            frame.basisAX.m[2][2] = frame.crossZ;
            RotMatrix((SVECTOR *)(&g_BattleMagicSlow_Angles[0]), &frame.rotMatrix);
            ConcatMatrixIntoFirst(&frame.basisAX, &frame.rotMatrix);
        }
    }
    ScaleMatrix(&frame.basisAX, &frame.scaleX);
    SetRotMatrix(&frame.basisAX);
    SetTransMatrix(&frame.basisAX);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicSlow_ModelReq1.flags = frame.drawFlags | 0x80;
    g_BattleMagicSlow_ModelReq1.alpha = *(short *)&frame.blend;
    if ((frame.drawFlags & 8) != 0) {
        g_BattleMagicSlow_ModelReq1.modelHandle = Gfx_ModelTableGetRenderObject(1, g_BattleMagicSlow_ResourceHandle);
        g_BattleMagicSlow_ModelReq2.modelHandle = Gfx_ModelTableGetRenderObject(3, g_BattleMagicSlow_ResourceHandle);
    } else {
        g_BattleMagicSlow_ModelReq1.modelHandle = Gfx_ModelTableGetRenderObject(0, g_BattleMagicSlow_ResourceHandle);
        g_BattleMagicSlow_ModelReq2.modelHandle = Gfx_ModelTableGetRenderObject(2, g_BattleMagicSlow_ResourceHandle);
    }
    if (g_BattleMagicSlow_ModelReq1.modelHandle != 0) {
        Gfx_ForEachPolygonSetPart((DxGroupPartCallback)BattleFx_MagicSlowBlendPolygonSetVertices, *(int *)((char *)g_BattleMagicSlow_ModelReq1.modelHandle + 4));
    }
    g_BattleMagicSlow_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicSlow_ModelReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicSlow_ChainTail);
    frame.angleX.vy = 0;
    frame.angleX.vx = frame.angleX.vy;
    frame.angleX.vz = -(frame.particle->timer << 8);
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    frame.rotMatrix.t[2] = 0;
    frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
    frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
    MultiplyMatrix(&frame.basisAX, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicSlow_ModelReq2.flags = frame.drawFlags | 0x80;
    g_BattleMagicSlow_ModelReq2.alpha = *(short *)&frame.blend;
    g_BattleMagicSlow_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicSlow_ModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicSlow_ChainTail);
    if (g_BattleIsPaused == 0) {
        frame.particle->timer++;
    }
}
} /* extern "C" */
