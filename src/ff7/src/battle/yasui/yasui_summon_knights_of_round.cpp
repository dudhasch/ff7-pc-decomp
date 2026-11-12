// src/ff7/src/battle/yasui/yasui_summon_knights_of_round.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle summon effect for Knights of the Round: it loads the knight models and runs a
//   long master camera timeline, with the radial disc mesh, knight spin and lift effects, and
//   impact flashes and bursts for each knight.

// clang-format off
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/mem.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_boss.h"
#include "lib/types/polygon.h"
#include "ff7/enums/battle.h"
#include "lib/codegen.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/api/sound.h"
#include "lib/enums/graphics.h"
extern "C" {

struct GfxPsxSpriteQuadState;
struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x007f3858
unsigned char g_BattleSummonKotrSoundGroupRecords[256] = {0xb1, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb4, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb6, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb8, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xba, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbc, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbe, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd4, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc2, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc4, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xca, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcf, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on
// GLOBAL: FF7 0x007f3958
unsigned int g_BattleSummonKotrSoundGroups[16] = {(unsigned int)&g_BattleSummonKotrSoundGroupRecords, (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 16), (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 32), (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 48), (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 64), (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 80), (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 96), (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 112), (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 128), (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 144), (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 160), (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 176), (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 192), (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 208), (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 224), (unsigned int)((char *)&g_BattleSummonKotrSoundGroupRecords + 240)};
// GLOBAL: FF7 0x007f3998
int g_BattleSummonKotrFragmentMesh[1340] = {
    0x5a0, 0xf15afffc, 0xfd7f, 0xf131fffc, 0x1b2, 0xf12000ff,
    0xffe3, 0xfae4fffc, 0xe3c, 0xfd78fffc, 0xf02, 0xfbfb00bb,
    0xea4, 0xba0fffc, 0x9e4, 0xcebfffc, 0x828, 0xc780154,
    0x8e5, 0xcd9fffc, 0xf801, 0xaeefffc, 0xf598, 0xc06010d,
    0xf6c5, 0xfc6afffc, 0xf190, 0xfb30fffc, 0xf1eb, 0xfbaf0034,
    0xf1b9, 0xf9770d5c, 0x240, 0xfa0f0d98, 0xfdbe, 0xf9320d61,
    0x94, 0x1640dc1, 0x64b, 0xfcb10d97, 0x5bf, 0xfde20d44,
    0x6ff, 0x6000dc4, 0x277, 0x3b30daa, 0x593, 0x6330d2c,
    0x483, 0x3660de0, 0xfb15, 0x70f0d4e, 0xfe0a, 0x6250d2b,
    0xfbb2, 0xfc260dee, 0xfbee, 0x690db6, 0xf9cc, 0xfd830d23,
    0xf92e, 0x1250bdf, 0xf6d7, 0xfeba09d9, 0xf4c7, 0xfe3d0b29,
    0xf626, 0xfb25091e, 0xf535, 0xf8f70b25, 0xf8f5, 0xfaea0b0c,
    0xf74c, 0xa380b31, 0xfe69, 0xaf108f0, 0xfa5f, 0x9f70aca,
    0xfc0b, 0x8cc08b9, 0xf748, 0x5f208b3, 0xf533, 0x73a0b7a,
    0xf943, 0x3ee0baa, 0x901, 0x8200962, 0x8f6, 0x6fd0b45,
    0x797, 0xb66088c, 0x5a5, 0x9c70b80, 0x247, 0x93d0b6a,
    0x439, 0xf9790bf1, 0x681, 0xfa94089c, 0xb38, 0xfbde0b47,
    0x939, 0xfe13097a, 0xbb9, 0xed0bf2, 0x960, 0xfee90aa8,
    0xac9, 0xf7a90bfb, 0xfcb8, 0xf46c0937, 0xfe01, 0xf61c0afa,
    0xfdb9, 0xf455094a, 0x12f, 0xf4dd0867, 0x58b, 0xf5b90ab9,
    0x1fe, 0xf2e9008a, 0xf8fb, 0xf56700cf, 0xf59b, 0xf4dcfffc,
    0xf640, 0xf354fffc, 0xf857, 0xf7650252, 0xf416, 0xfa430639,
    0xf3b8, 0xf9bd0450, 0xf32f, 0xfda506cc, 0xf2e4, 0xffbc01af,
    0xf121, 0xfdf50464, 0xf1dc, 0x2bc000f, 0xf13d, 0x72d00b6,
    0xf2bf, 0x651fffc, 0xf266, 0x2defffc, 0xf146, 0x94602b5,
    0xf45a, 0x7460580, 0xf3f8, 0xab203a0, 0xf5e1, 0xc7c0676,
    0xfa25, 0xe49025b, 0xfb28, 0xceb03fe, 0xf8ff, 0xf03012d,
    0xfd4d, 0xf22006e, 0x28a, 0xf30fffc, 0x1de, 0xf2dfffc,
    0xff25, 0xe3e021a, 0x55c, 0xc3b05ce, 0x73c, 0xcf103f9,
    0x743, 0x98e06e5, 0x9d2, 0x94002ac, 0xbf7, 0xb15046a,
    0x9ad, 0x6990036, 0xdd9, 0x272000e, 0xf19, 0x28ffffc,
    0xf13, 0x62dfffc, 0xdff, 0x5e02d2, 0xeff, 0xfd5905fa,
    0xdbb, 0xfd5e03c1, 0xe83, 0xfa0205f8, 0xc92, 0xf79b01bc,
    0xc74, 0xf9fa035b, 0xd81, 0xf57d0051, 0xacc, 0xf2dc00e4,
    0x72f, 0xf369fffc, 0x80b, 0xf52bfffc, 0xa6a, 0xf1f402df,
    0x446, 0xf27b0616, 0x1d9, 0xf1d10423, 0x237, 0xf2d606d6,
    0xfe72, 0xf1f40287, 0xfbcb, 0xf1910376, 0xfeb4, 0xf13dfffc,
    0x215, 0xf170fffc, 0x2b7, 0xf1a9fffc, 0xfc6d, 0xf2c5fffc,
    0xf95c, 0xf5a2fffc, 0xae5, 0xf750fffc, 0xc28, 0xffbbfffc,
    0xf10, 0x264fffc, 0xf19, 0x92afffc, 0xc10, 0xa53fffc,
    0xb31, 0xe53fffc, 0x51e, 0xf02fffc, 0x2f1, 0xee9fffc,
    0xfcff, 0xe87fffc, 0xfbdf, 0x8cffffc, 0xf3df, 0x791fffc,
    0xf306, 0xa2a06ca, 0xf705, 0xffeefffc, 0xf125, 0xfe91fffc,
    0xf129, 0xf92ffffc, 0xf2de, 0xf7c5fffc, 0xf3e7, 0xf82a0bc7,
    0x524, 0x3b40bde, 0xf776, 0xfe6b0f01, 0x64, 0x15a0f03,
    0xfed1, 0x360efb, 0xfe29, 0xf49507a9, 0xfa44, 0xf42f04d1,
    0xf85c, 0xf34c05b3, 0xfabc, 0x540053d, 0xf2e4, 0xd7a0676,
    0x36d, 0xc9a089b, 0xff3e, 0xccf082c, 0x1dd, 0x3660813,
    0xc85, 0x77205be, 0xc1c, 0x5e1085c, 0xb64, 0xf529040e,
    0x9a1, 0xf69106c0, 0x9a1, 0xf66a084f, 0x81c, 0xf163fffc,
    0xfd41, 0xf139fffc, 0x1fa, 0xf2effffc, 0x72c, 0xf9e2fffc,
    0xdca, 0xfd95fffc, 0xf07, 0x278fffc, 0xf16, 0x697fffc,
    0xdd2, 0xf16fffc, 0x27a, 0x2bcfffc, 0xf140, 0xf554fffc,
    0xf5cb, 0xf3c30582, 0x6b8, 0x4d604af, 0xdc1, 0x1e506a2,
    0xd9f, 0xeb8043e, 0x2b, 0xe120549, 0xfd3f, 0x310058c,
    0xf260, 0x356083d, 0xf3de, 0xf6df0866, 0xf7bd, 0xfe900edf,
    0x245, 0x2240ed1, 0x26b, 0xf642065e, 0xf6b8, 0xf5ccfffc,
    0xf555, 0x2a4fffc, 0xf13c, 0xa13fffc, 0xf4c6, 0xdaffffc,
    0xf983, 0xdc4fffc, 0x683, 0x6d2fffc, 0xdb2, 0xfda4fffc,
    0xf09, 0xf936fffc, 0xd72, 0xf2bdfffc, 0x6c5, 0xf168fffc,
    0xfd22, 0xae00f4, 0x100000, 0x800008, 0x3f700168, 0x18b007c,
    0x280018, 0x800020, 0x3f7051f4, 0x67f959f7, 0x400030, 0x800038,
    0x3f70ddcf, 0xe7c0e5c7, 0x580048, 0x800050, 0x3f70e73b, 0xd627e030,
    0x700060, 0x800068, 0x3f705e04, 0x53085806, 0x880078, 0x800080,
    0x3f703d92, 0x42673a83, 0xa00090, 0x800098, 0x3f708aba, 0x5bb567c0,
    0xb800a8, 0x8000b0, 0x3f70b595, 0x9fb3b7a7, 0xd000c0, 0x8000c8,
    0x3f709c4e, 0xbf6ab754, 0xe800d8, 0x8000e0, 0x3f705656, 0x8042633b,
    0x10000f0, 0x8000f8, 0x3f708627, 0x6f156c21, 0x1180108, 0x800110,
    0x3f704f1a, 0x3a3b4c2c, 0x1300120, 0x800128, 0x3f70db6f, 0xdf4ad858,
    0x1480138, 0x800140, 0x3f70cc2e, 0xb21abf3f, 0x1600150, 0x800158,
    0x3f70a1d1, 0xc6cfbdc4, 0x1780168, 0x800170, 0x3f70e4b0, 0xd892d3a4,
    0x1900180, 0x800188, 0x3f703fba, 0x4ae354d3, 0x1a80198, 0x8001a0,
    0x3f706ae8, 0x85d671e2, 0x1c001b0, 0x8001b8, 0x3f702c5e, 0x126c1f68,
    0x1d801c8, 0x8001d0, 0x3f701088, 0x17b01c90, 0x2100200, 0x800208,
    0x3f703318, 0x4910450e, 0x2280218, 0x800220, 0x3f706708, 0x79006a03,
    0x2600250, 0x800258, 0x3f70cb1a, 0xbc13d826, 0x2780268, 0x800270,
    0x3f70ea49, 0xf454ec40, 0x2b002a0, 0x8002a8, 0x3f70f4ab, 0xe7bdecbb,
    0x2c802b8, 0x8002c0, 0x3f70d1d4, 0xcbe3dbd1, 0x30002f0, 0x8002f8,
    0x3f707efe, 0x63f665fb, 0x3180308, 0x800310, 0x3f7047ec, 0x35e747f1,
    0x3500340, 0x800348, 0x3f7003a1, 0x48d0190, 0x3680358, 0x800360,
    0x3f70066f, 0x4590072, 0x100350, 0x800368, 0x3f700190, 0x72007c,
    0x280300, 0x800318, 0x3f7065fb, 0x47f159f7, 0x4002b0, 0x8002c8,
    0x3f70ecbb, 0xdbd1e5c7, 0x580260, 0x800278, 0x3f70d826, 0xec40e030,
    0x700210, 0x800228, 0x3f70450e, 0x6a035806, 0x8801c0, 0x8001d8,
    0x3f701f68, 0x1c903a83, 0xa00190, 0x8001a8, 0x3f7054d3, 0x71e267c0,
    0xb80160, 0x800178, 0x3f70bdc4, 0xd3a4b7a7, 0xd00130, 0x800148,
    0x3f70d858, 0xbf3fb754, 0xe80100, 0x800118, 0x3f706c21, 0x4c2c633b,
    0x1b80358, 0x8001c8, 0x3f70066f, 0x1088126c, 0x3880380, 0x8001e0,
    0x3f700460, 0xe420d45, 0x2d803a0, 0x8003a8, 0x3f7079fb, 0x8ffb90fb,
    0x1880308, 0x8002f8, 0x3f7047ec, 0x63f64ae3, 0x2c003b0, 0x8003b8,
    0x3f70c7e2, 0xd1d9cbe3, 0x16802b8, 0x8002a8, 0x3f70d1d4, 0xe7bde4b0,
    0x23803e0, 0x8003e8, 0x3f70c418, 0xba12b80e, 0x1380128, 0x8003f0,
    0x3f70df4a, 0xd62ccc2e, 0x22003f8, 0x800400, 0x3f707b01, 0x6f017900,
    0x1080218, 0x800208, 0x3f706708, 0x49104f1a, 0xd801b0, 0x800080,
    0x3f702c5e, 0x42675656, 0x1800078, 0x800418, 0x3f703d92, 0x31ae3fba,
    0xb00090, 0x800150, 0x3f708aba, 0xa1d19fb3, 0x12000c8, 0x800170,
    0x3f70bf6a, 0xd892db6f, 0xf000e0, 0x800420, 0x3f708042, 0x9e2c8627,
    0x4380428, 0x800430, 0x3f706c81, 0x88727e6c, 0x4500440, 0x800448,
    0x3f70154a, 0x153a0b4f, 0x2580458, 0x800140, 0x3f70a909, 0xb21abc13,
    0x4700460, 0x800468, 0x3f70f39c, 0xee76ef8e, 0x4880478, 0x800480,
    0x3f709aed, 0xbde8b2e5, 0x4a00490, 0x800498, 0x3f701dd1, 0x27d324c7,
    0x3680350, 0x800348, 0x3f700190, 0x48d0072, 0x32804b8, 0x800330,
    0x3f700eb8, 0x13c00dba, 0x3180300, 0x8002f8, 0x3f7065fb, 0x63f647f1,
    0x2d804d0, 0x8002e0, 0x3f7090fb, 0x90fb90fb, 0x2d802e0, 0x8004d0,
    0x3f7090fb, 0x90fb90fb, 0x2e804d8, 0x8002d0, 0x3f70b2ef, 0xb3f1aef1,
    0x4002c8, 0x8002c0, 0x3f70dbd1, 0xcbe3e5c7, 0x2c802b0, 0x8002a8,
    0x3f70ecbb, 0xe7bddbd1, 0x2b00040, 0x8002a0, 0x3f70e5c7, 0xf4abecbb,
    0x28804e0, 0x800290, 0x3f70f992, 0xf98df992, 0x580278, 0x800270,
    0x3f70ec40, 0xf454e030, 0x2680278, 0x8003f0, 0x3f70ec40, 0xd62cea49,
    0x2600058, 0x800250, 0x3f70e030, 0xcb1ad826, 0x23803e8, 0x800240,
    0x3f70ba12, 0xb00db80e, 0x24804e8, 0x800230, 0x3f709203, 0x92039403,
    0x2480230, 0x8004e8, 0x3f709203, 0x92039403, 0x2180228, 0x800208,
    0x3f706a03, 0x49106708, 0x1e804f0, 0x8001f0, 0x3f702229, 0x1f2c2226,
    0x780088, 0x800418, 0x3f703a83, 0x31ae3d92, 0x1c801c0, 0x8001b8,
    0x3f701f68, 0x126c1088, 0x1b00088, 0x800080, 0x3f703a83, 0x42672c5e,
    0x9000a0, 0x8001a0, 0x3f7067c0, 0x85d68aba, 0x1a80190, 0x800198,
    0x3f7054d3, 0x6ae871e2, 0x19000a0, 0x800098, 0x3f7067c0, 0x5bb554d3,
    0xb80178, 0x8000a8, 0x3f70d3a4, 0xb595b7a7, 0x1780160, 0x800158,
    0x3f70bdc4, 0xc6cfd3a4, 0x16000b8, 0x8000b0, 0x3f70b7a7, 0x9fb3bdc4,
    0xd00148, 0x8000c0, 0x3f70bf3f, 0x9c4eb754, 0x1480130, 0x800128,
    0x3f70d858, 0xdf4abf3f, 0x1200130, 0x8000c8, 0x3f70d858, 0xbf6adb6f,
    0xe80118, 0x800110, 0x3f704c2c, 0x3a3b633b, 0x1080100, 0x8000f8,
    0x3f706c21, 0x6f154f1a, 0x10000e8, 0x8000f0, 0x3f70633b, 0x86276c21,
    0x3400328, 0x8004f8, 0x3f700dba, 0x10b803a1, 0x4500358, 0x800440,
    0x3f70066f, 0x154a0b4f, 0x3200310, 0x800490, 0x3f7035e7, 0x1dd124d8,
    0x50802f0, 0x800500, 0x3f707efe, 0xa6f48df6, 0x3080188, 0x800498,
    0x3f704ae3, 0x27d347ec, 0x50002c0, 0x800480, 0x3f70cbe3, 0xbde8a6f4,
    0x2a00288, 0x800510, 0x3f70f992, 0xfb7ef4ab, 0x2b80158, 0x800480,
    0x3f70c6cf, 0xbde8d1d4, 0x2800270, 0x800518, 0x3f70f454, 0xf865f967,
    0x2580250, 0x800458, 0x3f70cb1a, 0xa909bc13, 0x2680128, 0x800518,
    0x3f70df4a, 0xf865ea49, 0x2300220, 0x800520, 0x3f707900, 0x97069203,
    0x20001e8, 0x800448, 0x3f702226, 0x153a3318, 0x21800f8, 0x800528,
    0x3f706f15, 0x9a106708, 0x4400110, 0x800530, 0x3f703a3b, 0x2933154a,
    0x780098, 0x800538, 0x3f705bb5, 0x6d933d92, 0x9000b0, 0x800540,
    0x3f709fb3, 0x90958aba, 0x54000a8, 0x800430, 0x3f70b595, 0x88729095,
    0x43000c0, 0x800438, 0x3f709c4e, 0x7e6c8872, 0x1400528, 0x800420,
    0x3f709a10, 0x9e2cb21a, 0x4700468, 0x800120, 0x3f70ee76, 0xdb6fef8e,
    0x4780488, 0x800150, 0x3f70b2e5, 0xa1d19aed, 0x41804a0, 0x800180,
    0x3f7024c7, 0x3fba31ae, 0x4280438, 0x8000d8, 0x3f707e6c, 0x56566c81,
    0x2080530, 0x800108, 0x3f702933, 0x4f1a4910, 0x3f00140, 0x800138,
    0x3f70b21a, 0xcc2ed62c, 0x4600470, 0x800168, 0x3f70ef8e, 0xe4b0f39c,
    0x2f80508, 0x800198, 0x3f708df6, 0x6ae863f6, 0x4500448, 0x800360,
    0x3f70153a, 0x4590b4f, 0x34801d0, 0x8001c8, 0x3f7017b0, 0x1088048d,
    0x4f801d0, 0x800348, 0x3f7017b0, 0x48d10b8, 0x1e00360, 0x800448,
    0x3f700459, 0x153a0e42, 0x4780198, 0x800508, 0x3f706ae8, 0x8df69aed,
    0x2a80460, 0x800168, 0x3f70f39c, 0xe4b0e7bd, 0x2580140, 0x8003f0,
    0x3f70b21a, 0xd62cbc13, 0x5480530, 0x800208, 0x3f702933, 0x4910262b,
    0x800428, 0x8000d8, 0x3f706c81, 0x56564267, 0x1d004a0, 0x800418,
    0x3f7024c7, 0x31ae17b0, 0x1a00478, 0x800150, 0x3f709aed, 0xa1d185d6,
    0x1700470, 0x800120, 0x3f70ef8e, 0xdb6fd892, 0xf00420, 0x800528,
    0x3f709e2c, 0x9a108627, 0xe00438, 0x8000c0, 0x3f707e6c, 0x9c4e8042,
    0xc80430, 0x8000a8, 0x3f708872, 0xb595bf6a, 0x5380090, 0x800540,
    0x3f708aba, 0x90956d93, 0x4280078, 0x800538, 0x3f703d92, 0x6d936c81,
    0x1b00110, 0x800440, 0x3f703a3b, 0x154a2c5e, 0x5200218, 0x800528,
    0x3f706708, 0x9a109706, 0x5480200, 0x800448, 0x3f703318, 0x153a262b,
    0x4580230, 0x800520, 0x3f709203, 0x9706a909, 0x4680518, 0x800128,
    0x3f70f865, 0xdf4aee76, 0x2380458, 0x800250, 0x3f70a909, 0xcb1ab80e,
    0x5100280, 0x800518, 0x3f70f967, 0xf865fb7e, 0x4880480, 0x800158,
    0x3f70bde8, 0xc6cfb2e5, 0x46002a0, 0x800510, 0x3f70f4ab, 0xfb7ef39c,
    0x2d002c0, 0x800500, 0x3f70cbe3, 0xa6f4b3f1, 0x4a00498, 0x800188,
    0x3f7027d3, 0x4ae324c7, 0x2d80500, 0x8002f0, 0x3f70a6f4, 0x7efe90fb,
    0x4980490, 0x800310, 0x3f701dd1, 0x35e727d3, 0x1b80440, 0x800358,
    0x3f70154a, 0x66f126c, 0x49004f8, 0x800328, 0x3f7010b8, 0xdba1dd1,
    0xe000f0, 0x8000e8, 0x3f708627, 0x633b8042, 0x1180100, 0x800108,
    0x3f706c21, 0x4f1a4c2c, 0xd800e8, 0x800110, 0x3f70633b, 0x3a3b5656,
    0xd000c8, 0x800130, 0x3f70bf6a, 0xd858b754, 0x1380148, 0x800128,
    0x3f70bf3f, 0xdf4acc2e, 0x42000c0, 0x800148, 0x3f709c4e, 0xbf3f9e2c,
    0x1500160, 0x8000b0, 0x3f70bdc4, 0x9fb3a1d1, 0x1680178, 0x800158,
    0x3f70d3a4, 0xc6cfe4b0, 0x17000a8, 0x800178, 0x3f70b595, 0xd3a4d892,
    0x1800190, 0x800098, 0x3f7054d3, 0x5bb53fba, 0x1880198, 0x800190,
    0x3f706ae8, 0x54d34ae3, 0x1a801a0, 0x8000a0, 0x3f7085d6, 0x67c071e2,
    0x1c00088, 0x8001b0, 0x3f703a83, 0x2c5e1f68, 0x1d801c0, 0x8001c8,
    0x3f701f68, 0x10881c90, 0x1d80418, 0x800088, 0x3f7031ae, 0x3a831c90,
    0x1e001f8, 0x800388, 0x3f70123d, 0xd450e42, 0x1e80550, 0x8004f0,
    0x3f702724, 0x22292226, 0x2100070, 0x800200, 0x3f705806, 0x3318450e,
    0x2100208, 0x800228, 0x3f704910, 0x6a03450e, 0x2280220, 0x800070,
    0x3f707900, 0x58066a03, 0x5580230, 0x8004e8, 0x3f709203, 0x92039203,
    0x55804e8, 0x800230, 0x3f709203, 0x92039203, 0x26003f0, 0x800278,
    0x3f70d62c, 0xec40d826, 0x2800298, 0x8003d0, 0x3f70f976, 0xf865f967,
    0x28803c8, 0x8004e0, 0x3f70f895, 0xf992f992, 0x2b802c8, 0x8002a8,
    0x3f70dbd1, 0xe7bdd1d4, 0x57802d0, 0x8004d8, 0x3f70b3f1, 0xb2efb5ef,
    0x2d803a8, 0x8004d0, 0x3f708ffb, 0x90fb90fb, 0x2d804d0, 0x8003a8,
    0x3f7090fb, 0x8ffb90fb, 0x3080318, 0x8002f8, 0x3f7047f1, 0x63f647ec,
    0x3200338, 0x800390, 0x3f7021d3, 0x26d824d8, 0x3280590, 0x8004b8,
    0x3f700db5, 0xeb80dba, 0x3580368, 0x800348, 0x3f700072, 0x48d066f,
    0x1d004f8, 0x8004a0, 0x3f7010b8, 0x24c717b0, 0x4f80490, 0x8004a0,
    0x3f701dd1, 0x24c710b8, 0x5080500, 0x800478, 0x3f70a6f4, 0x9aed8df6,
    0x5000480, 0x800478, 0x3f70bde8, 0x9aeda6f4, 0x5180468, 0x800510,
    0x3f70ee76, 0xfb7ef865, 0x5100468, 0x800460, 0x3f70ee76, 0xf39cfb7e,
    0x5200528, 0x800458, 0x3f709a10, 0xa9099706, 0x5280140, 0x800458,
    0x3f70b21a, 0xa9099a10, 0x5300548, 0x800440, 0x3f70262b, 0x154a2933,
    0x5480448, 0x800440, 0x3f70153a, 0x154a262b, 0x5400430, 0x800538,
    0x3f708872, 0x6d939095, 0x5380430, 0x800428, 0x3f708872, 0x6c816d93,
    0xc00420, 0x8000e0, 0x3f709e2c, 0x80429c4e, (int)((char *)&g_BattleSummonKotrModelWorkPool + 0xbe00), 0x8000c8,
    0x3f70d892, 0xbf6ab595, 0x1a00150, 0x800090, 0x3f70a1d1, 0x8aba85d6,
    0x980078, 0x800180, 0x3f703d92, 0x3fba5bb5, 0x11001b0, 0x8000d8,
    0x3f702c5e, 0x56563a3b, 0xf80218, 0x800108, 0x3f706708, 0x4f1a6f15,
    0x55001e8, 0x800410, 0x3f702226, 0x36182724, 0x26803f0, 0x800128,
    0x3f70d62c, 0xdf4aea49, 0x5680270, 0x8003d8, 0x3f70f454, 0xf45bee47,
    0x15802b8, 0x800168, 0x3f70d1d4, 0xe4b0c6cf, 0x2a00570, 0x8003c0,
    0x3f70eeb3, 0xf3a7f4ab, 0x1980188, 0x8002f8, 0x3f704ae3, 0x63f66ae8,
    0x5880310, 0x800398, 0x3f7035e7, 0x33e244ed, 0x34801c8, 0x800358,
    0x3f701088, 0x66f048d, 0x5900328, 0x800378, 0x3f700dba, 0x3930db5,
    0x4500360, 0x800358, 0x3f700459, 0x66f0b4f, 0x4f80348, 0x800340,
    0x3f70048d, 0x3a110b8, 0x4900328, 0x800320, 0x3f700dba, 0x24d81dd1,
    0x4980310, 0x800308, 0x3f7035e7, 0x47ec27d3, 0x50802f8, 0x8002f0,
    0x3f7063f6, 0x7efe8df6, 0x50002d8, 0x8002d0, 0x3f7090fb, 0xb3f1a6f4,
    0x48002c0, 0x8002b8, 0x3f70cbe3, 0xd1d4bde8, 0x46002a8, 0x8002a0,
    0x3f70e7bd, 0xf4abf39c, 0x5100288, 0x800280, 0x3f70f992, 0xf967fb7e,
    0x5180270, 0x800268, 0x3f70f454, 0xea49f865, 0x3f00260, 0x800258,
    0x3f70d826, 0xbc13d62c, 0x4580238, 0x800230, 0x3f70b80e, 0x9203a909,
    0x5200220, 0x800218, 0x3f707900, 0x67089706, 0x5480208, 0x800200,
    0x3f704910, 0x3318262b, 0x44801e8, 0x8001e0, 0x3f702226, 0xe42153a,
    0x41801d8, 0x8001d0, 0x3f701c90, 0x17b031ae, 0x44001b8, 0x8001b0,
    0x3f70126c, 0x2c5e154a, 0x47801a0, 0x800198, 0x3f7085d6, 0x6ae89aed,
    0x4a00188, 0x800180, 0x3f704ae3, 0x3fba24c7, 0x4700170, 0x800168,
    0x3f70d892, 0xe4b0ef8e, 0x4880158, 0x800150, 0x3f70c6cf, 0xa1d1b2e5,
    0x4200148, 0x800140, 0x3f70bf3f, 0xb21a9e2c, 0x4680128, 0x800120,
    0x3f70df4a, 0xdb6fee76, 0x5300110, 0x800108, 0x3f703a3b, 0x4f1a2933,
    0x52800f8, 0x8000f0, 0x3f706f15, 0x86279a10, 0x43800e0, 0x8000d8,
    0x3f708042, 0x56567e6c, 0x43000c8, 0x8000c0, 0x3f70bf6a, 0x9c4e8872,
    0x54000b0, 0x8000a8, 0x3f709fb3, 0xb5959095, 0x5380098, 0x800090,
    0x3f705bb5, 0x8aba6d93, 0x4280080, 0x800078, 0x3f704267, 0x3d926c81,
    0x1d, 0x1f801e0
};
unsigned char g_rawData_007f4e88[580] = {
    0xe8, 0x01, 0xf0, 0x01, 0x42, 0x0e, 0x70, 0x3f, 0x3d, 0x12, 0x26, 0x22, 0x2c, 0x1f, 0x80, 0x00, 0x30, 0x02, 0x48, 0x02, 0x38, 0x02, 0x40, 0x02,
    0x03, 0x92, 0x70, 0x3f, 0x03, 0x94, 0x0e, 0xb8, 0x0d, 0xb0, 0x80, 0x00, 0x80, 0x02, 0x98, 0x02, 0x88, 0x02, 0x90, 0x02, 0x67, 0xf9, 0x70, 0x3f,
    0x76, 0xf9, 0x92, 0xf9, 0x8d, 0xf9, 0x80, 0x00, 0xd0, 0x02, 0xe8, 0x02, 0xd8, 0x02, 0xe0, 0x02, 0xf1, 0xb3, 0x70, 0x3f, 0xf1, 0xae, 0xfb, 0x90,
    0xfb, 0x90, 0x80, 0x00, 0x20, 0x03, 0x38, 0x03, 0x28, 0x03, 0x30, 0x03, 0xd8, 0x24, 0x70, 0x3f, 0xd3, 0x21, 0xba, 0x0d, 0xc0, 0x13, 0x80, 0x00,
    0x70, 0x03, 0x40, 0x03, 0x78, 0x03, 0x28, 0x03, 0x8e, 0x01, 0x70, 0x3f, 0xa1, 0x03, 0x93, 0x03, 0xba, 0x0d, 0x80, 0x00, 0x90, 0x03, 0x20, 0x03,
    0x98, 0x03, 0x10, 0x03, 0xd8, 0x26, 0x70, 0x3f, 0xd8, 0x24, 0xe2, 0x33, 0xe7, 0x35, 0x80, 0x00, 0xc0, 0x03, 0xa0, 0x02, 0xc8, 0x03, 0x88, 0x02,
    0xa7, 0xf3, 0x70, 0x3f, 0xab, 0xf4, 0x95, 0xf8, 0x92, 0xf9, 0x80, 0x00, 0xd0, 0x03, 0x80, 0x02, 0xd8, 0x03, 0x70, 0x02, 0x65, 0xf8, 0x70, 0x3f,
    0x67, 0xf9, 0x5b, 0xf4, 0x54, 0xf4, 0x80, 0x00, 0x08, 0x04, 0x00, 0x02, 0x10, 0x04, 0xe8, 0x01, 0x10, 0x42, 0x70, 0x3f, 0x18, 0x33, 0x18, 0x36,
    0x26, 0x22, 0x80, 0x00, 0xa8, 0x04, 0x68, 0x03, 0x00, 0x00, 0x10, 0x00, 0x67, 0x03, 0x70, 0x3f, 0x72, 0x00, 0x68, 0x01, 0x7c, 0x00, 0x80, 0x00,
    0x10, 0x00, 0x50, 0x03, 0x08, 0x00, 0xb0, 0x04, 0x7c, 0x00, 0x70, 0x3f, 0x90, 0x01, 0x8b, 0x01, 0x8d, 0x01, 0x80, 0x00, 0xc0, 0x04, 0x18, 0x03,
    0x18, 0x00, 0x28, 0x00, 0xef, 0x49, 0x70, 0x3f, 0xf1, 0x47, 0xf4, 0x51, 0xf7, 0x59, 0x80, 0x00, 0x28, 0x00, 0x00, 0x03, 0x20, 0x00, 0xc8, 0x04,
    0xf7, 0x59, 0x70, 0x3f, 0xfb, 0x65, 0xf9, 0x67, 0xf9, 0x67, 0x80, 0x00, 0x70, 0x00, 0x60, 0x00, 0x20, 0x02, 0x00, 0x04, 0x06, 0x58, 0x70, 0x3f,
    0x04, 0x5e, 0x00, 0x79, 0x01, 0x6f, 0x80, 0x00, 0x70, 0x00, 0x00, 0x02, 0x68, 0x00, 0x08, 0x04, 0x06, 0x58, 0x70, 0x3f, 0x18, 0x33, 0x08, 0x53,
    0x10, 0x42, 0x80, 0x00, 0x50, 0x02, 0x60, 0x05, 0x58, 0x00, 0x50, 0x00, 0x1a, 0xcb, 0x70, 0x3f, 0x1f, 0xd0, 0x30, 0xe0, 0x27, 0xd6, 0x80, 0x00,
    0x58, 0x00, 0x48, 0x00, 0x70, 0x02, 0x68, 0x05, 0x30, 0xe0, 0x70, 0x3f, 0x3b, 0xe7, 0x54, 0xf4, 0x47, 0xee, 0x80, 0x00, 0xa0, 0x02, 0x70, 0x05,
    0x40, 0x00, 0x38, 0x00, 0xab, 0xf4, 0x70, 0x3f, 0xb3, 0xee, 0xc7, 0xe5, 0xc0, 0xe7, 0x80, 0x00, 0x40, 0x00, 0x30, 0x00, 0xc0, 0x02, 0xb8, 0x03,
    0xc7, 0xe5, 0x70, 0x3f, 0xcf, 0xdd, 0xe3, 0xcb, 0xd9, 0xd1, 0x80, 0x00, 0x00, 0x03, 0xf0, 0x02, 0xc8, 0x04, 0x80, 0x05, 0xfb, 0x65, 0x70, 0x3f,
    0xfe, 0x7e, 0xf9, 0x67, 0xfb, 0x68, 0x80, 0x00, 0x88, 0x05, 0x10, 0x03, 0xc0, 0x04, 0x18, 0x03, 0xed, 0x44, 0x70, 0x3f, 0xe7, 0x35, 0xef, 0x49,
    0xf1, 0x47, 0x80, 0x00, 0x50, 0x03, 0x40, 0x03, 0xb0, 0x04, 0x70, 0x03, 0x90, 0x01, 0x70, 0x3f, 0xa1, 0x03, 0x8d, 0x01, 0x8e, 0x01, 0x80, 0x00,
    0x98, 0x05, 0x60, 0x03, 0xa8, 0x04, 0x68, 0x03, 0x65, 0x03, 0x70, 0x3f, 0x59, 0x04, 0x67, 0x03, 0x72, 0x00, 0x80, 0x00, 0x58, 0x05, 0x30, 0x02,
    0xf8, 0x03, 0x20, 0x02, 0x03, 0x92, 0x70, 0x3f, 0x03, 0x92, 0x01, 0x7b, 0x00, 0x79, 0x80, 0x00, 0x60, 0x05, 0x50, 0x02, 0xe0, 0x03, 0x38, 0x02,
    0x1f, 0xd0, 0x70, 0x3f, 0x1a, 0xcb, 0x18, 0xc4, 0x0e, 0xb8, 0x80, 0x00, 0x78, 0x05, 0xd0, 0x02, 0xb0, 0x03, 0xc0, 0x02, 0xef, 0xb5, 0x70, 0x3f,
    0xf1, 0xb3, 0xe2, 0xc7, 0xe3, 0xcb, 0x80, 0x00, 0x80, 0x05, 0xf0, 0x02, 0xa0, 0x03, 0xd8, 0x02, 0xfb, 0x68, 0x70, 0x3f, 0xfe, 0x7e, 0xfb, 0x79,
    0xfb, 0x90, 0x80, 0x00, 0xe0, 0x01, 0x60, 0x03, 0x80, 0x03, 0x98, 0x05, 0x42, 0x0e, 0x70, 0x3f, 0x59, 0x04, 0x60, 0x04, 0x65, 0x03, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00
};
// clang-format on

// GLOBAL: FF7 0x007f50d0
short g_BattleKotrKnightAttachIndices[13] = {0x21, 0x2f, 0x1e, 0x2b, 0x21, 0x2b, 0x1e, 0x2f, 0x1e, 0x2f, 0x2b, 0x21, 0x30};
// GLOBAL: FF7 0x007f50f0
int g_BattleKotrKnightTints[13] = {0xffc080, 0xffc080, 0xffc080, 0xffc080, 0xffc080, 0xffc080, 0xffc080, 0xffc080, 0xffc080, 0xffc080, 0xffc080, 0xffc080, 0xffc080};
// clang-format off
// GLOBAL: FF7 0x007f5128
struct BattleSummonKotrKnightLookAtPoints g_BattleSummonKotrKnightLookAtPoints = {{0}, -0xbb72af8, {0x30, 0xf8, 0, 0, 0, 0, 0xe8, 3, 0, 0, 0, 0, 0x70, 0x17, 0x40, 0x1f, 0x90, 0xe8, 0, 0, 0xe8, 3, 0, 0, 0xf4, 1, 0, 0, 0xe0, 0x2e, 0x68, 0xc5, 0x48, 0xf4, 0, 0, 0, 0, 0xf4, 1, 0x4c, 4, 0, 0, 0xe0, 0x2e, 0x84, 0xea, 0x94, 0xf8, 0, 0, 0x30, 0xf8, 0, 0, 0, 0, 0, 0, 0x30, 0xf8, 0, 0, 0xf0, 0xd8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x78, 0xec, 0x88, 0x13, 0x10, 0x27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x78, 0xec, 0x68, 0xc5, 0x10, 0x27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x58, 0x1b, 0xa8, 0xe4, 0xa8, 0xe4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xe0, 0xb1, 0xe8, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xf0, 0xd8, 0xe8, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x10, 0x27, 0, 0, 0, 0, 0x30, 0xf8, 0xd0, 7, 0, 0, 0, 0, 0xf0, 0xd8, 0xe0, 0x2e, 0, 0, 0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0, 0, 0, 0, 0}};
// clang-format on
// GLOBAL: FF7 0x007f51f8
unsigned int g_BattleSummonKotrModelRecords[95] = {0x00c840b1, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer01, 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer02, 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer03, 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer04, 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer05, 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer06, 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer07, 0x00000000, 0x00000000, 0x00000000, 0x00c800b4, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer08, 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer09, 0x00000000, 0x00000000, 0x00000000, 0x00c800b4, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer10, 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer11, 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer12, 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer13, 0x00000000, 0x00000000, 0x00000000, 0x01f400b1, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer14, 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer15, 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer16, 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer17, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer18, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer19, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer20, 0x00000000, 0x00000000, 0x00193839, (unsigned int)&g_BattleKotrFragmentData.kotrDataPointer21, 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x007f5374
int g_BattleSummonKotrOffscreenSlotIndex = (-1);
// GLOBAL: FF7 0x007f5378
unsigned char g_BattleSummonKotrCameraScript[842] = {0x8b, 0x00, 0x04, 0x00, 0x4b, 0x00, 0x04, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0xf0, 0xd8, 0x78, 0xec, 0x78, 0xec, 0x80, 0x00, 0x32, 0x00, 0x40, 0x00, 0x32, 0x00, 0x0a, 0x00, 0x00, 0x01, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0xe0, 0xb1, 0x18, 0xfc, 0x85, 0x00, 0x1e, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x88, 0x13, 0x00, 0x00, 0x45, 0x00, 0x1e, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x06, 0xff, 0x0a, 0x00, 0x00, 0x02, 0x81, 0x00, 0x00, 0x00, 0x88, 0x13, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x88, 0x13, 0x10, 0x27, 0x82, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x00, 0x19, 0x00, 0x08, 0xd5, 0x48, 0xf4, 0x30, 0xf8, 0x80, 0x00, 0x05, 0x00, 0x40, 0x00, 0x05, 0x00, 0x80, 0x00, 0x05, 0x00, 0x40, 0x00, 0x05, 0x00, 0x80, 0x00, 0x31, 0x00, 0x40, 0x00, 0x31, 0x00, 0x82, 0x00, 0x20, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00, 0x43, 0x00, 0x20, 0x00, 0x70, 0x17, 0x40, 0x1f, 0x90, 0xe8, 0x80, 0x00, 0x20, 0x00, 0x40, 0x00, 0x20, 0x00, 0x82, 0x00, 0x08, 0x00, 0xe8, 0x03, 0x00, 0x00, 0xf4, 0x01, 0x43, 0x00, 0x08, 0x00, 0xe0, 0x2e, 0x68, 0xc5, 0x48, 0xf4, 0x80, 0x00, 0x29, 0x00, 0x40, 0x00, 0x29, 0x00, 0x82, 0x00, 0x0e, 0x00, 0x00, 0x00, 0xe8, 0x03, 0xe8, 0x03, 0x43, 0x00, 0x0e, 0x00, 0xe0, 0x2e, 0x84, 0xea, 0x94, 0xf8, 0x80, 0x00, 0x0a, 0x00, 0x40, 0x00, 0x0a, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0xf0, 0xd8, 0x94, 0x11, 0x30, 0xf8, 0x80, 0x00, 0x28, 0x00, 0x40, 0x00, 0x28, 0x00, 0x82, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x00, 0x14, 0x00, 0x30, 0xf8, 0x00, 0x00, 0xf0, 0xd8, 0x80, 0x00, 0x06, 0x00, 0x46, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x04, 0x60, 0xf0, 0x00, 0x00, 0xe0, 0x2e, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00, 0x0f, 0x00, 0x82, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x00, 0x05, 0x00, 0x78, 0xec, 0x88, 0x13, 0x10, 0x27, 0x80, 0x00, 0x1b, 0x00, 0x40, 0x00, 0x1b, 0x00, 0x82, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x43, 0x00, 0x10, 0x00, 0x60, 0xf0, 0x68, 0xc5, 0x10, 0x27, 0x80, 0x00, 0x3a, 0x00, 0x40, 0x00, 0x3a, 0x00, 0x80, 0x00, 0x07, 0x00, 0x40, 0x00, 0x07, 0x00, 0x80, 0x00, 0x24, 0x00, 0x40, 0x00, 0x24, 0x00, 0x82, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x43, 0x00, 0x08, 0x00, 0xb8, 0x0b, 0xe0, 0xb1, 0x88, 0x13, 0x80, 0x00, 0x26, 0x00, 0x40, 0x00, 0x26, 0x00, 0x88, 0x00, 0x00, 0x00, 0x03, 0x00, 0x48, 0x00, 0x00, 0x00, 0x03, 0x00, 0x81, 0x00, 0x00, 0x00, 0xbc, 0x02, 0x00, 0x00, 0x41, 0x00, 0x78, 0xec, 0xbc, 0x02, 0x00, 0x00, 0x80, 0x00, 0x1e, 0x00, 0x40, 0x00, 0x1e, 0x00, 0x89, 0x00, 0x49, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x41, 0x00, 0xb8, 0x0b, 0xe0, 0xb1, 0x88, 0x13, 0x80, 0x00, 0x28, 0x00, 0x40, 0x00, 0x28, 0x00, 0x82, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x18, 0xfc, 0xe8, 0x03, 0x43, 0x00, 0x1d, 0x00, 0xa8, 0xe4, 0xf0, 0xd8, 0xe0, 0x2e, 0x88, 0x00, 0x01, 0x00, 0x03, 0x00, 0x48, 0x00, 0x01, 0x00, 0x03, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x88, 0x13, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x0a, 0x00, 0x40, 0x00, 0x0a, 0x00, 0x89, 0x00, 0x49, 0x00, 0x85, 0x00, 0x25, 0x00, 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x45, 0x00, 0x25, 0x00, 0xff, 0x02, 0x00, 0x00, 0x68, 0xc5, 0xb8, 0x0b, 0x82, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x18, 0xfc, 0x00, 0x00, 0x43, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x18, 0xfc, 0x88, 0x13, 0x80, 0x00, 0x0a, 0x00, 0x40, 0x00, 0x0a, 0x00, 0x82, 0x00, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x29, 0x00, 0x00, 0x00, 0xd0, 0x07, 0x10, 0x27, 0x82, 0x00, 0x0b, 0x00, 0x0c, 0xfe, 0x30, 0xf8, 0xd0, 0x07, 0x43, 0x00, 0x0b, 0x00, 0x00, 0x00, 0xf0, 0xd8, 0xe0, 0x2e, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00, 0x0f, 0x00, 0x88, 0x00, 0x01, 0x00, 0x03, 0x00, 0x48, 0x00, 0x01, 0x00, 0x03, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x88, 0x13, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x0a, 0x00, 0x40, 0x00, 0x0a, 0x00, 0x89, 0x00, 0x49, 0x00, 0x81, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00, 0x41, 0x00, 0x40, 0x1f, 0x40, 0x1f, 0xa8, 0xe4, 0x80, 0x00, 0x19, 0x00, 0x40, 0x00, 0x19, 0x00, 0x82, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x2d, 0x00, 0x00, 0x00, 0xe0, 0xb1, 0x88, 0x13, 0x80, 0x00, 0x32, 0x00, 0x40, 0x00, 0x32, 0x00, 0x88, 0x00, 0x00, 0x00, 0xff, 0xff, 0x48, 0x00, 0x00, 0x00, 0xff, 0xff, 0x81, 0x00, 0x00, 0x00, 0x68, 0xf7, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x68, 0xf7, 0xe8, 0x03, 0x80, 0x00, 0x10, 0x00, 0x42, 0x00, 0x10, 0x00, 0x00, 0x00, 0x68, 0xf7, 0x88, 0x13, 0x89, 0x00, 0x49, 0x00, 0x85, 0x00, 0x36, 0x00, 0xff, 0x02, 0x00, 0x00, 0x3c, 0xf6, 0x78, 0xec, 0x45, 0x00, 0x36, 0x00, 0xff, 0x02, 0x00, 0x00, 0xa0, 0x0f, 0x98, 0x3a, 0x80, 0x00, 0x0a, 0x00, 0x40, 0x00, 0x0a, 0x00, 0x82, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0c, 0xfe, 0x40, 0x1f, 0x80, 0x00, 0x05, 0x00, 0x40, 0x00, 0x05, 0x00, 0x85, 0x00, 0x1e, 0x00, 0xe0, 0x02, 0x00, 0x00, 0x3c, 0xf6, 0x00, 0x00, 0x40, 0x00, 0x1e, 0x00, 0x80, 0x00, 0x2d, 0x00, 0x40, 0x00, 0x2d, 0x00, 0xff, 0xff};
// GLOBAL: FF7 0x007f56c4
struct BattleKotrDiscBuffer *g_BattleSummonKotrDiscBuffer = (struct BattleKotrDiscBuffer *)&g_BattleSummonKotrDiscBufferStorage;
// GLOBAL: FF7 0x007f56c8
unsigned char g_BattleSummonKotrOffscreenMatrix[30] = {0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00};
// The MATRIX MultiplyMatrix takes as its second operand (diagonal 0x1000 at +0, +8, +0x10 the code rewrites, the
// translation word +0x1A); 32 bytes.
// GLOBAL: FF7 0x007f56e8
struct BattleSummonKotrOffscreenRequest g_BattleSummonKotrOffscreenRequest = {0x1000, {0, 0, 0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0, 0, 0}, 0x1000, {0}, 0, {0}};
// clang-format off
// GLOBAL: FF7 0x007f5708
struct BattleSummonKotrStageRequestArgs g_BattleSummonKotrStageRequestArgs = {832, 0, {0xc4, 0, 0xc4, 0}};
// A model draw record {model, flags, frame, 0x20} nothing names.
// GLOBAL: FF7 0x007f5710
BattleKotrModelDrawHead g_BattleSummonKotrModelDrawRequest = {0, 0xa8, 0, 0, 0x20, 0};
// MATRIX 0x7f5720..0x7f5740: diagonal 0x2F52, zero translation (retail bytes); MultiplyMatrix operand, +8 rescaled per frame.
// GLOBAL: FF7 0x007f5720
short g_BattleSummonKotrScaleMatrix[16] = {12114, 0, 0, 0, 12114, 0, 0, 0, 12114, 0, 0, 0, 0, 0, 0, 0};
// clang-format on
// GLOBAL: FF7 0x007f5740
struct BattleSummonKotrKnightEndpointModel g_BattleSummonKotrKnightEndpointModel = {0, {0x88, 0, 0, 0, 0, 0, 0, 0, 0x40, 0, 0, 0}, 0x1000, {0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007f5770
struct BattleSummonKotrBurstSprite g_BattleSummonKotrBurstSprite = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x007f5780
struct BattleSummonKotrEasedSpinScaleDraw g_BattleSummonKotrEasedSpinScaleDraw = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x007f5790
unsigned char g_BattleSummonKotrKnight1SpinScaleModel[56] = {0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x10, 0xa0, 0x02, 0x00, 0x08, 0xa0, 0x03, 0x00, 0x20, 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f57c8
struct BattleSummonKotrKnight2SpinScaleModel g_BattleSummonKotrKnight2SpinScaleModel = {0, {0x14, 0, 0, 0, 0, 0x10, 0xa0, 2, 0, 8, 0xa0, 3, 0, 0x20, 0xff, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 0, 0}, 0, {6, 0xf6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0x84, 8, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007f5818
struct BattleSummonKotrImpactSpriteParticle g_BattleSummonKotrImpactSpriteParticle = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x007f5828
struct BattleSummonKotrSpinGrowModel g_BattleSummonKotrSpinGrowModel = {0, 128, {0}, 0, {0x20, 0, 0, 0}};
// clang-format off
// A sprite draw record BattleFx_SubmitSpriteDraw takes {table group pointer, field 4, CVECTOR, frame (short)}.
// GLOBAL: FF7 0x007f5838
struct BattleSummonKotrImpactSpriteRing g_BattleSummonKotrImpactSpriteRing = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x007f5848
struct BattleFxKotrBurstSource g_BattleSummonKotrImpactBurstOrigins[4] = {{0}, {1000, 2000, 0, {0}}, {0, 2000, -500, {0}}, {0, -1000, 1000, {0}}};
// The two endpoint records BattleFx_KotrSpawnKnightEndpointTrail takes; 24 bytes each.
// GLOBAL: FF7 0x007f5868
int g_BattleSummonKotrKnight3EndpointA[6] = {0, 0xfa3c, 0, 0, 0xffc080, 0};
// GLOBAL: FF7 0x007f5880
int g_BattleSummonKotrKnight3EndpointB[6] = {0, 0, 0, 0x5ed, 0xffc080, 0};
// GLOBAL: FF7 0x007f5898
struct BattleSummonKotrKnight4SpinScaleModel g_BattleSummonKotrKnight4SpinScaleModel = {0, {0xc, 0, 0, 0, 0, 0x10, 0xa0, 2, 0, 8, 0xa0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0xff, 0xff, 0, 0}, 0, {0xaf, 0xf7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007f58d0
struct BattleSummonKotrKnight5SpinScaleModel g_BattleSummonKotrKnight5SpinScaleModel = {0, {0xa, 0, 0, 0, 0, 0x10, 0xa0, 2, 0, 8, 0xa0, 3, 0, 0x20, 0xff, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0xff, 0xff, 0, 0}, 0, {0x6b, 0xf5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007f5908
BattleKotrSpinScaleParams g_BattleKotrKnight6SpinScaleParams = {0, {0xa, 0, 0, 0, 0, 0x10, 0xff, 2, 0, 8, 0xa0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0xff, 0xff, 0, 0}};
// GLOBAL: FF7 0x007f5928
struct BattleSummonKotrKnight6SpinScaleModel g_BattleSummonKotrKnight6SpinScaleModel = {0, {0xa, 0, 0, 0, 0, 0x10, 0xff, 2, 0, 8, 0xa0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0xff, 0xff, 0, 0}, 0, {0x29, 0xf8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007f5960
BattleKotrModelDrawHead g_BattleKotrKnight7BillboardDraw = {0, 0xa8, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x007f5970
struct BattleSummonKotrKnight7ExpandingModel g_BattleSummonKotrKnight7ExpandingModel = {0, 128, {0}, 0, {0x20, 0, 0, 0}, -0x3fffb00, {0x80, 0xff, 0, 0}, 0, {0xb1, 0xfd, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0x37, 2, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007f59b8
struct BattleSummonKotrKnight8SpinScaleModel g_BattleSummonKotrKnight8SpinScaleModel = {0, {0xa, 0, 0, 0, 0, 0x10, 0, 0, 0, 8, 0xa0, 3, 0, 0x20, 0xff, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 0, 0}, 0, {0xb8, 0xf6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0xf8, 8, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007f5a08
struct BattleSummonKotrTumblingModel g_BattleSummonKotrTumblingModel = {0, {0x80, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, 0, -3000, 2000, {0}};
// GLOBAL: FF7 0x007f5a20
struct BattleSummonKotrKnight9BillboardModel g_BattleSummonKotrKnight9BillboardModel = {0, {0xa8, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, -0x4000000, {0}, 0, {0x87, 0xfa, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0x92, 5, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007f5a68
struct BattleSummonKotrKnight10SpinScaleModel g_BattleSummonKotrKnight10SpinScaleModel = {0, {0xa, 0, 0, 0, 0, 0x10, 0xff, 2, 0, 8, 0xa0, 1, 0, 0, 0xff, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 0, 0}, -0x4000000, {0xc0, 0xff, 0, 0}, 0, {0x22, 0xf7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0x3c, 9, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007f5ac0
BattleKotrSpinScaleParams g_BattleKotrKnight11SpinScaleParamsA = {0, {0xa, 0, 0, 0, 0, 0x10, 0xff, 2, 0, 8, 0, 0, 0, 0x10, 0xff, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 0, 0}};
// GLOBAL: FF7 0x007f5ae0
BattleKotrSpinScaleParams g_BattleKotrKnight11SpinScaleParamsB = {0, {0xa, 0, 0, 0, 0, 0x10, 0xff, 2, 0, 8, 0, 0, 0, 0x10, 0xff, 2, 0, 0, 0, 0, 0, 4, 0, 0, 0xff, 0xff, 0, 0}};
// GLOBAL: FF7 0x007f5b00
struct BattleSummonKotrKnight11SpinScaleModel g_BattleSummonKotrKnight11SpinScaleModel = {0, {0xa, 0, 0, 0, 0, 0x20, 0xff, 2, 0, 8, 0xa0, 3, 0, 0x10, 0xff, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 0, 0}, -0x1f40000, {0}, 0, {0x60, 0xf8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007f5b40
BattleKotrSpinScaleParams g_BattleKotrKnight12SpinScaleParams = {0, {0xa, 0, 0, 0, 0, 0x10, 0xff, 2, 0, 8, 0xa0, 3, 0, 0x10, 0xff, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0xff, 0xff, 0, 0}};
// GLOBAL: FF7 0x007f5b60
struct BattleSummonKotrKnight12ScaleModelPair g_BattleSummonKotrKnight12ScaleModelPair = {0, {0xa, 0, 0, 0, 0, 0x10, 0xff, 2, 0, 8, 0xa0, 3, 0, 0x10, 0xff, 2, 0, 0, 0, 0, 0, 6, 0, 0, 0xff, 0xff, 0, 0}, 0x300fe00, {0, 7, 0, 0}, 0, {0xe6, 0xf8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0xc0, 0xff, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007f5ba0
BattleKotrModelDrawHead g_BattleKotrKnight13FlashDraw = {0, 0xa8, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x007f5bb0
short g_BattleSummonKotrKnight13RotMatrix[4] = {256, 0, 0, 0};
// GLOBAL: FF7 0x007f5bb8
short g_BattleSummonKotrKnight13MatrixDiagY[4] = {256, 0, 0, 0};
// GLOBAL: FF7 0x007f5bc0
short g_BattleSummonKotrKnight13MatrixDiagZ[4] = {256, 0, 0, -120};
unsigned char g_rawData_007f5bc8[8] = {0xff, 0xff, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f5bd0
BattleKotrSpinScaleParams g_BattleKotrKnight13SpinScaleParams = {0, {0xc, 0, 0, 0, 0, 0x20, 0xa0, 2, 0, 8, 0xa0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007f5bf0
int g_BattleSummonKotrScalingPairDraw[52] = {
    0x0, 0xa8, 0x0, 0x20, 0xef34ff6a, 0xec78,
    0xef340096, 0xec78, 0x0, 0xe7f0, 0x0, 0x0,
    0xffc080, 0x0, 0x1000, 0x0, 0x1000, 0x0,
    0x1000, 0x0, 0x0, 0x0, 0x0, 0x88,
    0x10000000, 0x20, (int)((char *)&g_BattleSummonKotrSceneWorkPool + 0x0), 0x0, 0x0, 0x88,
    0x10000080, 0x20, (int)((char *)&g_BattleSummonKotrSceneWorkPool + 0x0), 0x0, 0x0, 0x88,
    0x10000000, 0x20, (int)((char *)&g_BattleSummonKotrSceneWorkPool + 0x0), 0x0, 0x0, 0x88,
    0x10000000, 0x20, 0x0, 0x88, 0x10000080, 0x20,
    0x0, 0x88, 0x10000000, 0x20
};
// GLOBAL: FF7 0x007f5cc0
BattleKotrModelDrawHead g_BattleKotrConvergingStreakDraw = {0, 0xa8, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x007f5cd0
BattleKotrModelDrawHead g_BattleKotrRadialStreakDraw = {0, 0xa8, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x007f5ce0
struct BattleSummonKotrFadingScrollState g_BattleSummonKotrFadingScrollState = {0, {0, 0, 0, 0, 0, 0, 0, 1, 0, 1}, -128, -128, -128, {0x64, 0x2c, 0, 8, 0x78, 0, 0, 0, 0, 0, 0, 0, 0, 0x88, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}};
// GLOBAL: FF7 0x007f5d08
BattleKotrModelDrawHead g_BattleKotrSpinningFadeDrawA = {0, 0x88, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x007f5d18
BattleKotrModelDrawHead g_BattleKotrSpinningFadeDrawB = {0, 0x88, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x007f5d28
struct BattleKotrSpinPairData g_BattleKotrSpinPairData = {0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0x404080, {0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0x54, 0, 0x20, 0, 0x1b, 0, 0x3c, 0, 0x62, 0xf6, 2, 0, 2, 0, 0, 0}, 255, {0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0x54, 0, 0x20, 0, 0x1b, 0, 0x14, 0, 0x62, 0xf6, 2, 0, 0, 0, 0xcf, 4}, 0xff0000, {0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0x1b, 0, 0x4e, 0, 0x1e, 0, 0x14, 0, 0x62, 0xf6, 2, 0, 0, 0, 0, 0}, 0x800080, {0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0x1b, 0, 0x4e, 0, 0x1e, 0, 0x3c, 0, 0x62, 0xf6, 2, 0, 0, 0, 0xcf, 4}, 0x804000, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0x1e, 0, 0x38, 0, 0x25, 0, 0x19, 0, 0x31, 0xfb, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0}, 0x8000, {0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0x25, 0, 0x22, 0, 0x24, 0, 0x3c, 0, 0x93, 0xf1, 2, 0, 0, 0, 0, 0, 0xf0, 0xf0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0x25, 0, 0x47, 0, 0x24, 0, 0x14, 0, 0x62, 0xf6, 2, 0, 1, 0, 0xcf, 4}, 0x80f0, {0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0xe8, 0xff, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0x24, 0, 0xc0, 0, 0x33, 0, 0, 0, 0x93, 0xf1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007f5ee0
unsigned char g_BattleKotrSpinModelsParamBlock[24] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x10, 0x00, 0x05, 0x00, 0x00, 0x00, 0x24, 0x00, 0xfe, 0x00, 0x4b, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f5ef8
unsigned char g_BattleKotrFadeModelParamBlock[24] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x44, 0x00, 0x11, 0x00, 0x00, 0x00, 0x6c, 0x00, 0xc6, 0x00, 0x0f, 0x00, 0x01, 0x00};
// GLOBAL: FF7 0x007f5f10
struct BattleKotrFragmentData g_BattleKotrFragmentData = {0, {0, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, 0x3000000, 0x62ffffff, 0, 0, 640, 332, {0, 0, 0, 3, 0xff, 0xff, 0xff, 0x62}, 0, 0, 640, 332, 0x4d414542, {0x5f, 0x31, 0x2e, 0x52, 0x53, 0x44, 0, 0}, 0x43524943, {0x4c, 0x45, 0x5f, 0x31, 0x2e, 0x52, 0x53, 0x44, 0, 0, 0, 0}, 0x494c5943, {0x4e, 0x44, 0x45, 0x52, 0x2e, 0x52, 0x53, 0x44, 0, 0, 0, 0}, 0x5f505845, {0x31, 0x2e, 0x52, 0x53, 0x44, 0, 0, 0}, 0x31544948, {0x2e, 0x52, 0x53, 0x44, 0, 0, 0, 0}, 0x32544948, {0x2e, 0x52, 0x53, 0x44, 0, 0, 0, 0}, 0x33544948, {0x2e, 0x52, 0x53, 0x44, 0, 0, 0, 0}, 0x5f454349, {0x31, 0x2e, 0x52, 0x53, 0x44, 0, 0, 0}, 0x5f454349, {0x31, 0x2e, 0x52, 0x53, 0x44, 0, 0, 0}, 0x4554454d, {0x4f, 0x5f, 0x41, 0x2e, 0x52, 0x53, 0x44, 0}, 0x474e4952, {0x31, 0x2e, 0x52, 0x53, 0x44, 0, 0, 0}, 0x4b4e4553, {0x4f, 0x55, 0x2e, 0x52, 0x53, 0x44, 0, 0}, 0x45485053, {0x52, 0x45, 0x5f, 0x31, 0x2e, 0x52, 0x53, 0x44, 0, 0, 0, 0}, 0x45504853, {0x52, 0x45, 0x5f, 0x32, 0x2e, 0x52, 0x53, 0x44, 0, 0, 0, 0}, 0x45485053, {0x52, 0x45, 0x5f, 0x33, 0x2e, 0x52, 0x53, 0x44, 0, 0, 0, 0}, 0x45425554, {0x5f, 0x31, 0x2e, 0x52, 0x53, 0x44, 0, 0}, 0x424d4f42, {0x5f, 0x41, 0x2e, 0x54, 0x49, 0x4d, 0, 0}, 0x424d4f42, {0x5f, 0x41, 0x2e, 0x53, 0x50, 0x54, 0, 0}, 0x5f544948, {0x31, 0x2e, 0x54, 0x49, 0x4d, 0, 0, 0}, 0x5f544948, {0x31, 0x2e, 0x53, 0x50, 0x54, 0, 0, 0}, 0x315f4742, {0x2e, 0x54, 0x49, 0x4d, 0, 0, 0, 0}};
// GLOBAL: FF7 0x00a58aa8
int g_BattleKotrColorWords = 0;
unsigned char g_bssPad_00a58aac[4] = {0};
// GLOBAL: FF7 0x00a58ab0
int g_BattleSummonKotrKnightContexts = 0;
unsigned char g_bssPad_00a58ab4[4] = {0};
unsigned char g_bssPad_00a58ab8[48] = {0};
// g_BattleSummonKotrKnightComposedPos is indexed by the code as the composedPos members of the knight
// context records. The span runs to the next named definition of the TU; the TU's pieces inside
// 0xa58ae8..0xa58bb8 are aliases.
// GLOBAL: FF7 0x00a58ae8
struct BattleSummonKotrKnightComposedPos g_BattleSummonKotrKnightComposedPos = {0};
// GLOBAL: FF7 0x00a58bb8
struct BattleFxKotrKnight7State *g_BattleKotrKnight7State = 0;
// GLOBAL: FF7 0x00a58bbc
BattleFxKotrSpinState *g_BattleKotrSpinState = 0;
// GLOBAL: FF7 0x00a58bc0
int g_BattleKotrKnight4Angle = 0;
// GLOBAL: FF7 0x00a58bc4
float g_BattleSummonKotrLayerTint = 0.0f;
unsigned char g_bssPad_00a58bc8[4] = {0};
// GLOBAL: FF7 0x00a58bcc
BattleSummonKotrLift *g_BattleKotrLiftState = 0;
// GLOBAL: FF7 0x00a58bd0
struct BattleFxKotrFadeParticle *g_BattleKotrFadeParticle = 0;
// GLOBAL: FF7 0x00a58bd4
BattleFxKotrTriangleParticle *g_BattleKotrTriangleParticle = 0;
// GLOBAL: FF7 0x00a58bd8
unsigned char g_BattleKotrKnight1TimelineTaskInit = 0;
// GLOBAL: FF7 0x00a58bdc
unsigned char g_BattleKotrKnight6TimelineTaskInit = 0;
// GLOBAL: FF7 0x00a58be0
short g_BattleKotrHandledMask = 0;
// GLOBAL: FF7 0x00a58be4
unsigned char g_BattleKotrPulsingTriangleParticleTaskInit = 0;
// GLOBAL: FF7 0x00a58be8
unsigned char g_BattleKotrScalingModelPairTaskInit = 0;
// GLOBAL: FF7 0x00a58bec
unsigned char g_BattleKotrTumblingShrinkingModelTaskInit = 0;
// GLOBAL: FF7 0x00a58bf0
unsigned char g_BattleKotrSpinningGrowingModelTaskInit = 0;
// GLOBAL: FF7 0x00a58bf4
unsigned char g_BattleKotrDrawKnightModelInit = 0;
// Camera-script operand table: negative operand -k reads element k of the shorts based at
// g_BattleSummonKotrTumbleEmitterInit.kotrScriptCountEntry.
// GLOBAL: FF7 0x00a58bf8
struct BattleSummonKotrTumbleEmitterInit g_BattleSummonKotrTumbleEmitterInit = {0};
unsigned char g_bssPad_00a58c00[20] = {0};
// GLOBAL: FF7 0x00a58c14
BattleFxKotrFragmentScale *g_BattleKotrSpinShrinkState = 0;
unsigned char g_bssPad_00a58c18[4] = {0};
// GLOBAL: FF7 0x00a58c1c
unsigned char g_BattleKotrUpdateFragmentSpinShrinkMatricesInit = 0;
// at the bare address). Its compare reads it as g_BattleKotrUpdateFragmentSpinShrinkMatricesInit + 4: through its own name VC5 would fold the
// store and the reload into one `fst`, the retail code has `fstp`/`fld`.
// GLOBAL: FF7 0x00a58c20
float g_BattleKotrTintValue = 0;
// GLOBAL: FF7 0x00a58c24
struct YasuiKotrEmitterState *g_BattleKotrEmitterState = 0;
// GLOBAL: FF7 0x00a58c28
int g_BattleKotrLayerCount = 0;
// GLOBAL: FF7 0x00a58c2c
BattleFxTriBasisScratch *g_BattleKotrTriangleScratch = 0;
// GLOBAL: FF7 0x00a58c30
BattleSummonKotrOffscreenCenterState g_BattleSummonKotrOffscreenCenter = {0};
// GLOBAL: FF7 0x00a58c50
BattleFxKotrFragmentScale *g_BattleKotrExpandState = 0;
// GLOBAL: FF7 0x00a58c54
struct BattleFxKotrFragmentState *g_BattleKotrFragmentState = 0;
// GLOBAL: FF7 0x00a58c58
unsigned char g_BattleKotrDrawPsxFragmentTrianglesInit = 0;
// GLOBAL: FF7 0x00a58c5c
BattleKotrSpinModelPose *g_BattleKotrSpinModelState = 0;
// GLOBAL: FF7 0x00a58c60
int g_BattleKotrKnight3Angle = 0;
// GLOBAL: FF7 0x00a58c64
unsigned char g_BattleKotrActorLiftTaskInit = 0;
// GLOBAL: FF7 0x00a58c68
BattleKotrOrbitPose *g_BattleKotrKnight2State = 0;
unsigned char g_bssPad_00a58c6c[4] = {0};
// GLOBAL: FF7 0x00a58c70
BattleSummonKotrScriptState g_BattleSummonKotrScriptStateA = {0};
unsigned char g_bssPad_00a58c74[4] = {0};
unsigned char g_bssPad_00a58c78[4] = {0};
// GLOBAL: FF7 0x00a58c7c
BattleSummonKotrActorAttachState g_BattleSummonKotrActorAttachA = {0};
// GLOBAL: FF7 0x00a58c80
int g_BattleKotrCameraOffset = 0;
// GLOBAL: FF7 0x00a58c84
int g_BattleSummonKotrCameraFocusOffsetZ = 0;
unsigned char g_bssPad_00a58c88[20] = {0};
// GLOBAL: FF7 0x00a58c9c
struct BattleFxKotrKnight7State *g_BattleKotrKnight9State = 0;
// GLOBAL: FF7 0x00a58ca0
BattleKotrOrbitPose *g_BattleKotrKnight3State = 0;
// GLOBAL: FF7 0x00a58ca4
unsigned char g_BattleKotrConvergingStreakModelTaskInit = 0;
// GLOBAL: FF7 0x00a58ca8
int g_BattleKotrScalingPairState = 0;
// GLOBAL: FF7 0x00a58cac
struct Vec3sPacked *g_BattleKotrKnight11State = 0;
// GLOBAL: FF7 0x00a58cb0
int g_BattleSummonKotrKnight5Scratch = 0;
// GLOBAL: FF7 0x00a58cb4
unsigned char g_BattleKotrKnight9TimelineTaskInit = 0;
// GLOBAL: FF7 0x00a58cb8
unsigned char g_BattleKotrImpactBurstTimelineTaskInit = 0;
// GLOBAL: FF7 0x00a58cbc
unsigned char g_BattleKotrFadingOffscreenQuadParticleTaskInit = 0;
// GLOBAL: FF7 0x00a58cc0
int g_BattleSummonKotrLayerDrawListHead = 0;
// GLOBAL: FF7 0x00a58cc4
int g_BattleKotrKnight1Radius = 0;
// g_BattleSummonKotrActorRepresentative is indexed by the code as an array (read by
// BattleFx_KotrBeginEffect). The span runs to the next named definition of the TU; the TU's pieces
// inside 0xa58cc8..0xa58cd4 are aliases.
// GLOBAL: FF7 0x00a58cc8
unsigned char g_BattleSummonKotrActorRepresentative[0xc] = {0};
// GLOBAL: FF7 0x00a58cd4
unsigned char g_BattleKotrKnight5TimelineTaskInit = 0;
// GLOBAL: FF7 0x00a58cd8
BattleModelBank *g_BattleKotrKnightModels[13] = {0};
// GLOBAL: FF7 0x00a58d0c
short g_BattleKotrTargetMask = 0;
// GLOBAL: FF7 0x00a58d10
unsigned char g_BattleKotrKnight8TimelineTaskInit = 0;
// GLOBAL: FF7 0x00a58d14
unsigned char g_BattleKotrImpactSpriteRingTaskInit = 0;
// GLOBAL: FF7 0x00a58d18
unsigned char g_BattleKotrKnight10TimelineTaskInit = 0;
// GLOBAL: FF7 0x00a58d1c
BattleFxKotrScatterState *g_BattleKotrScatterState = 0;
// GLOBAL: FF7 0x00a58d20
unsigned char g_BattleSummonKotrLayerDrawBuffers[2][0x5a00] = {0};
// GLOBAL: FF7 0x00a64120
struct BattleFxKotrKnight7State *g_BattleKotrKnight10State = 0;
// GLOBAL: FF7 0x00a64124
BattleKotrOrbitPose *g_BattleKotrKnight4State = 0;
// GLOBAL: FF7 0x00a64128
PsxMatrix g_BattleSummonKotrTransformState = {0};
// GLOBAL: FF7 0x00a64148
BattleFxKotrRadialStreakState *g_BattleSummonKotrRadialStreakState = 0;
// GLOBAL: FF7 0x00a6414c
int g_BattleSummonKotrFragmentRenderBanks = 0;
// GLOBAL: FF7 0x00a64150
BattleKotrPointPose *g_BattleSummonKotrBillboardPoint = 0;
// GLOBAL: FF7 0x00a64154
struct BattleFxKotrRadialDisc *g_BattleSummonKotrRadialDisc = 0;
// GLOBAL: FF7 0x00a64158
struct BattleFxKotrEndpointTrail *g_BattleSummonKotrEndpointTrail = 0;
// GLOBAL: FF7 0x00a6415c
BattleFxCameraOrbitState *g_BattleSummonKotrCameraState = 0;
// GLOBAL: FF7 0x00a64160
struct PsxMatrix *g_BattleSummonKotrFragmentMatrixBuffer = 0;
// GLOBAL: FF7 0x00a64164
unsigned char g_BattleKotrSpinningFadingModelTaskInit = 0;
// g_BattleSummonKotrActorLiftTasks is indexed by the code as an array. The span runs to the next
// named definition of the TU; the TU's pieces inside 0xa64168..0xa64190 are aliases.
// GLOBAL: FF7 0x00a64168
unsigned char g_BattleSummonKotrActorLiftTasks[0x28] = {0};
// GLOBAL: FF7 0x00a64190
BattleFxKotrEasedSpinState *g_BattleSummonKotrEasedSpinState = 0;
// GLOBAL: FF7 0x00a64194
BattleFxCameraOrbitState *g_BattleSummonKotrCameraOther = 0;
// GLOBAL: FF7 0x00a64198
unsigned char g_BattleSummonKotrAttachLocalPositions[0x4] = {0};
unsigned char g_bssPad_00a6419c[4] = {0};
unsigned char g_bssPad_00a641a0[72] = {0};
// GLOBAL: FF7 0x00a641e8
struct BattleKotrImpactBurst *g_BattleSummonKotrImpactBurst = 0;
// GLOBAL: FF7 0x00a641ec
int g_BattleSummonKotrKnight1Angle = 0;
// GLOBAL: FF7 0x00a641f0
unsigned char g_BattleKotrEasedSpinScaleModelTaskInit = 0;
// GLOBAL: FF7 0x00a641f4
BattleFxKotrFragmentScale *g_BattleSummonKotrFadeState = 0;
// GLOBAL: FF7 0x00a641f8
unsigned char g_BattleKotrExpandingFadingModelTaskInit = 0;
// GLOBAL: FF7 0x00a641fc
short g_BattleSummonKotrTargetMask = 0;
// GLOBAL: FF7 0x00a64200
struct YasuiKotrSpinState *g_BattleSummonKotrSpinState = 0;
unsigned char g_bssPad_00a64204[4] = {0};
// g_BattleSummonKotrSavedActorDrawFlags is indexed by the code as an array (read by
// BattleFx_KotrBeginEffect, BattleFx_KotrMasterTimeline,
// BattleFx_KotrFragmentSpinScatterFadeTimelineTask). The span runs to the next named definition of
// the TU; the TU's pieces inside 0xa64208..0xa64214 are aliases.
// GLOBAL: FF7 0x00a64208
unsigned char g_BattleSummonKotrSavedActorDrawFlags[0xc] = {0};
// GLOBAL: FF7 0x00a64214
unsigned char g_BattleKotrFadingStackedModelLayersTaskInit = 0;
// GLOBAL: FF7 0x00a64218
BattleFxKotrRadialStreakState *g_BattleSummonKotrConvergingStreakState = 0;
// GLOBAL: FF7 0x00a6421c
struct BattleFxModelDraw *g_BattleSummonKotrModelDraw = 0;
// GLOBAL: FF7 0x00a64220
unsigned char g_BattleKotrKnight3TimelineTaskInit = 0;
unsigned char g_bssPad_00a64224[4] = {0};
// GLOBAL: FF7 0x00a64228
unsigned char g_BattleSummonKotrDiscBufferStorage[65860] = {0};
unsigned char g_bssPad_00a7436c[4] = {0};
unsigned char g_BattleSummonKotrModelWorkPool[7864] = {0};
// GLOBAL: FF7 0x00a76228
unsigned char g_BattleSummonKotrFragmentPool[0xe800] = {0};
// GLOBAL: FF7 0x00a84a28
unsigned char g_BattleKotrKnight12TimelineTaskInit = 0;
// GLOBAL: FF7 0x00a84a2c
unsigned char g_BattleKotrKnight4TimelineTaskInit = 0;
// GLOBAL: FF7 0x00a84a30
unsigned char g_BattleKotrRadialDiscMeshTimelineTaskInit = 0;
unsigned char g_bssPad_00a84a34[4] = {0};
// GLOBAL: FF7 0x00a84a38
BattleSummonKotrScriptState g_BattleSummonKotrScriptStateB = {0};
unsigned char g_bssPad_00a84a3c[4] = {0};
unsigned char g_bssPad_00a84a40[4] = {0};
// GLOBAL: FF7 0x00a84a44
BattleSummonKotrActorAttachState g_BattleSummonKotrActorAttachB = {0};
// GLOBAL: FF7 0x00a84a48
int g_BattleSummonKotrCameraOffset = 0;
// GLOBAL: FF7 0x00a84a4c
int g_BattleSummonKotrCameraEyeOffsetZ = 0;
unsigned char g_bssPad_00a84a50[20] = {0};
// GLOBAL: FF7 0x00a84a64
int g_BattleSummonKotrKnight3Radius = 0;
unsigned char g_BattleSummonKotrSceneWorkPool[2304] = {0};
// GLOBAL: FF7 0x00a85368
unsigned char g_BattleKotrSpinningModelPairTaskInit = 0;
// GLOBAL: FF7 0x00a8536c
BattleSummonKotrKnightState *g_BattleSummonKotrKnightLocalPose = 0;
unsigned char g_bssPad_00a85370[4] = {0};
// GLOBAL: FF7 0x00a85374
unsigned char g_BattleKotrUpdateFragmentScatterMatricesInit = 0;
// GLOBAL: FF7 0x00a85378
BattleFxSummonState *g_BattleSummonKotrLayerFadeState = 0;
// GLOBAL: FF7 0x00a8537c
BattleKotrModelPose *g_BattleSummonKotrKnight1TimelineState = 0;
// GLOBAL: FF7 0x00a85380
BattleKotrPointPose *g_BattleSummonKotrKnightState = 0;
// GLOBAL: FF7 0x00a85384
int g_BattleSummonKotrKnight4Radius = 0;
// GLOBAL: FF7 0x00a85388
unsigned char g_BattleKotrKnight2TimelineTaskInit = 0;
// GLOBAL: FF7 0x00a8538c
struct BattleFxKotrKnight7State *g_BattleSummonKotrKnight12State = 0;
// GLOBAL: FF7 0x00a85390
BattleKotrOrbitPose *g_BattleSummonKotrKnight6TimelineState = 0;
// GLOBAL: FF7 0x00a85394
unsigned char g_BattleKotrSpinningModelEntry15TaskInit = 0;
// GLOBAL: FF7 0x00a85398
unsigned char g_BattleKotrKnight11TimelineTaskInit = 0;
// GLOBAL: FF7 0x00a8539c
unsigned char g_BattleKotrRadialStreakModelTaskInit = 0;
// GLOBAL: FF7 0x00a853a0
BattleKotrModelPose *g_BattleSummonKotrFragmentTimelineState = 0;
// GLOBAL: FF7 0x00a853a4
unsigned char g_BattleKotrKnightEndpointTrailTaskInit = 0;
// GLOBAL: FF7 0x00a853a8
unsigned char g_BattleKotrFragmentSpinScatterFadeTimelineTaskInit = 0;
// GLOBAL: FF7 0x00a853ac
BattleFxKotrTumbleShrinkState *g_BattleSummonKotrTumbleShrinkState = 0;
// GLOBAL: FF7 0x00a853b0
unsigned char g_BattleKotrKnight7TimelineTaskInit = 0;
unsigned char g_bssPad_00a853b4[4] = {0};
// GLOBAL: FF7 0x00a853b8
short g_BattleSummonKotrEffectAngles[2] = {0};
unsigned char g_bssPad_00a853bc[4] = {0};
// GLOBAL: FF7 0x00a853c0
int g_BattleSummonKotrImpactRecordIndex = 0;
// GLOBAL: FF7 0x00a853c4
int g_BattleSummonKotrKnightContextSlot = 0;
// GLOBAL: FF7 0x00a853c8
struct GfxTableCursor *g_BattleSummonKotrTableCursor = 0;
unsigned char g_bssPad_00a853cc[4] = {0};
// GLOBAL: FF7 0x00a853d0
unsigned char g_BattleSummonKotrBillboardMatrix[0x4] = {0};
unsigned char g_bssPad_00a853d4[4] = {0};
// GLOBAL: FF7 0x00a853d8
short g_BattleSummonKotrBillboardMatrixDiagY = 0;
unsigned char g_bssPad_00a853dc[4] = {0};
// GLOBAL: FF7 0x00a853e0
struct BattleKotrBillboardTail g_BattleSummonKotrBillboard = {0};
// GLOBAL: FF7 0x00a853f0
int g_BattleSummonKotrBufferIndex = 0;
// GLOBAL: FF7 0x00a853f4
int g_BattleSummonKotrQuadEmissionTotal = 0;
// GLOBAL: FF7 0x00a853f8
struct BattleSummonKotrPulseTriangleV0 g_BattleSummonKotrPulseTriangleV0 = {0};
unsigned char g_bssPad_00a853fc[4] = {0};
// GLOBAL: FF7 0x00a85400
int g_BattleSummonKotrPulseTriangleV1 = 0;
unsigned char g_bssPad_00a85404[4] = {0};
// GLOBAL: FF7 0x00a85408
struct BattleSummonKotrPulseTriangleV2 g_BattleSummonKotrPulseTriangleV2 = {0};
unsigned char g_bssPad_00a8540c[4] = {0};
// GLOBAL: FF7 0x00a85410
int g_BattleSummonKotrKnight7BillboardOrigin = 0;
unsigned char g_bssPad_00a85414[4] = {0};
// GLOBAL: FF7 0x00a85418
int g_BattleSummonKotrKnight8Rotation = 0;
unsigned char g_bssPad_00a8541c[4] = {0};
// GLOBAL: FF7 0x00a85420
BattleSummonKotrTumbleXYState g_BattleSummonKotrTumbleXY = {0};
// GLOBAL: FF7 0x00a85424
short g_BattleSummonKotrTumbleStateZ = 0;
// GLOBAL: FF7 0x00a85428
short g_BattleSummonKotrScalingPairMatrix = 0;
unsigned char g_bssPad_00a8542c[4] = {0};
// GLOBAL: FF7 0x00a85430
short g_BattleSummonKotrScalingPairMatrixDiagY = 0;
unsigned char g_bssPad_00a85434[4] = {0};
// GLOBAL: FF7 0x00a85438
short g_BattleSummonKotrScalingPairMatrixDiagZ = 0;
unsigned char g_bssPad_00a8543c[4] = {0};
unsigned char g_bssPad_00a85440[8] = {0};
// GLOBAL: FF7 0x00a85448
int g_BattleSummonKotrConvergingStreakEmitAccum = 0;
// GLOBAL: FF7 0x00a8544c
int g_BattleSummonKotrRadialStreakEmitAccum = 0;
// GLOBAL: FF7 0x00a85450
int g_BattleSummonKotrScrollPhase = 0;
// GLOBAL: FF7 0x00a85454
int g_BattleSummonKotrFragmentIndex = 0;
// GLOBAL: FF7 0x00a85458
int g_BattleSummonKotrLayerDrawBufferIndex = 0;
unsigned char g_bssPad_00a8545c[4] = {0};
// clang-format on

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)
#define g_BattleKotrStackedFixedTransform (*(int *)((char *)&g_BattleSummonKotrScalingPairDraw + 0x38))
#define g_BattleKotrStackedRotationX (*(int *)((char *)&g_BattleSummonKotrScalingPairDraw + 0x4a))
#define g_BattleKotrStackedRotationY (*(int *)((char *)&g_BattleSummonKotrScalingPairDraw + 0x4e))
#define g_BattleKotrStackedRotationZ (*(int *)((char *)&g_BattleSummonKotrScalingPairDraw + 0x52))
#define g_BattleKotrStackedVariantZeroSecondary (*(int *)((char *)&g_BattleSummonKotrScalingPairDraw + 0x58))
#define g_BattleKotrStackedVariantOneSecondary (*(int *)((char *)&g_BattleSummonKotrScalingPairDraw + 0x70))
#define g_BattleKotrStackedDefaultSecondary (*(int *)((char *)&g_BattleSummonKotrScalingPairDraw + 0x88))
#define g_BattleKotrStackedVariantZeroPrimary (*(int *)((char *)&g_BattleSummonKotrScalingPairDraw + 0xa0))
#define g_BattleKotrStackedVariantOnePrimary (*(int *)((char *)&g_BattleSummonKotrScalingPairDraw + 0xb0))
#define g_BattleKotrStackedDefaultPrimary (*(int *)((char *)&g_BattleSummonKotrScalingPairDraw + 0xc0))

#define g_BattleKotrScalingPairPointA (*(int *)((char *)&g_BattleSummonKotrScalingPairDraw + 0x10))
#define g_BattleKotrScalingPairPointB (*(int *)((char *)&g_BattleSummonKotrScalingPairDraw + 0x18))
#define g_BattleKotrKnight13TrailEndpoint (*(int *)((char *)&g_BattleSummonKotrScalingPairDraw + 0x20))
#define g_BattleKotrFragmentPrimColorByte1 (*(int *)((char *)&g_BattleKotrFragmentData + 0x15))
#define g_BattleKotrFragmentPrimColorByte2 (*(int *)((char *)&g_BattleKotrFragmentData + 0x16))




/*
 * When the battle scene exists and
 * g_BattleSummonKotrTableCursor is zero, initializes a default-duration resource record, writes
 * type zero and duration 0x5DC, builds SUMMON/KNIGHTS into the local path buffer and calls
 * Gfx_LoadModelTableIfAbsent with resource ID 0x13, g_BattleSummonKotrModelRecords metadata, global
 * output slot and scene context at 0x4C. Regardless of that guard or creation result then calls
 * BattleFx_KotrLoadThirteenKnightModels and registers g_BattleSummonKotrSoundGroups sound groups.
 */
// FUNCTION: FF7 0x004766c0
void __cdecl BattleFx_SummonLoadResKnightsOfRound(void)
{
    int resourceId;
    char pathBuf[200];
    GfxResourceRequest requestBuf;
    GfxModelTableBank *sceneCtx;
    GfxResourceRequest *requestPtr;
    int header;

    if (g_pBattle3dScene != 0 && g_BattleSummonKotrTableCursor == 0) {
        resourceId = 0x13;
        header = (int)&g_BattleSummonKotrModelRecords;
        sceneCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestBuf;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        requestPtr->duration = 0x5DC;
        // STRING: FF7 0x007f6050
        requestPtr->resourcePath = (char *)BattleFx_BuildResourcePath("SUMMON/KNIGHTS", pathBuf);
        Gfx_LoadModelTableIfAbsent(resourceId, header, requestPtr, &g_BattleSummonKotrTableCursor, sceneCtx);
    }
    BattleFx_KotrLoadThirteenKnightModels();
    SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleSummonKotrSoundGroups);
}

/*
 * Walks the 13 model banks in g_BattleKotrKnightModels, passing each to BattleModel_FreeBank and
 * then zeroing the slot.
 */
// FUNCTION: FF7 0x00476770
void __cdecl BattleFx_KotrFreeThirteenKnightModels(void)
{
    int i;

    for (i = 0; i < 0xD; i++) {
        BattleModel_FreeBank((BattleLimitAnimBank *)g_BattleKotrKnightModels[i]);
        g_BattleKotrKnightModels[i] = 0;
    }
}

/*
 * When the first g_BattleKotrKnightModels bank is null, builds SUMMON/KNIGHTS/KNIGHT01.DAT through KNIGHT13.DAT paths and
 * loads thirteen category-4 model banks into the g_BattleKotrKnightModels array.
 */
// FUNCTION: FF7 0x004767b3
void __cdecl BattleFx_KotrLoadThirteenKnightModels(void)
{
    KotrLoaderLocals locals;

    if (g_BattleKotrKnightModels[0] == 0) {
        for (locals.index = 0; locals.index < 0xD; locals.index++) {
            sprintf(
                locals.path,
                // STRING: FF7 0x007f6070
                "%s/KNIGHT%02d.DAT",
                BattleFx_BuildResourcePath(
                    // STRING: FF7 0x007f6060
                    "SUMMON/KNIGHTS", locals.resourcePath),
                locals.index + 1);
            g_BattleKotrKnightModels[locals.index] = (BattleModelBank *)Battle_LoadEffectModelCategory4(0, 6, (int)locals.path);
        }
    }
}

/*
 * Forwards both arguments unchanged to BattleFx_KotrBeginEffect and does nothing else.
 */
// FUNCTION: FF7 0x00476842
void __cdecl BattleFx_SummonEnterKnightsOfRound(int targetMask, int actorIdx)
{
    BattleFx_KotrBeginEffect(targetMask, actorIdx);
}

/*
 * Resets Knights of the Round state and loads its resources and thirteen models, stores the actor
 * mask and obtains its XZ bounding-box midpoint. Sets rotation 0x800 when midpoint Z is below
 * actorIdx's Z, builds the shared transform and saves Z. For actors four through nine saves flag bytes
 * and expands masks for actors sharing X/Z positions, recording their representative, then
 * schedules the master timeline and camera script and sets mask eight in g_BattleStageHideFlags.
 * Battle_GetActorMaskCentroid writes the midpoint's Z to centroidX.vz.
 */
// FUNCTION: FF7 0x00476857
void __cdecl BattleFx_KotrBeginEffect(int targetMask, int actorIdx)
{
    SVECTOR centroidX;
    int actorIndex;
    int otherIdx;
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    BattleFx_KotrResetModuleState();
    BattleFx_SummonLoadResKnightsOfRound();
    BattleFx_KotrLoadThirteenKnightModels();
    Battle_GetActorMaskCentroid(g_BattleKotrTargetMask = targetMask, &centroidX.vx);
    if (centroidX.vz < g_BattleActorRecords[actorIdx].posZ) {
        g_BattleSummonKotrEffectAngles[1] = 0x800;
    }
    RotMatrix((SVECTOR *)&g_BattleSummonKotrEffectAngles, &g_BattleSummonKotrOffscreenCenter);
    g_BattleSummonKotrOffscreenCenter.centerZ = centroidX.vz;
    g_BattleSummonKotrTargetMask = g_BattleKotrTargetMask;
    g_BattleKotrHandledMask = 0;
    for (actorIndex = 4; actorIndex < 0xA; actorIndex++) {
        g_BattleSummonKotrSavedActorDrawFlags[actorIndex] = g_BattleActorRecords[actorIndex].drawFlags;
        if ((g_BattleKotrTargetMask & 1 << actorIndex) != 0 && (g_BattleKotrHandledMask & 1 << actorIndex) == 0) {
            for (otherIdx = 4; otherIdx < 0xA; otherIdx++) {
                if (actorIndex != otherIdx && g_BattleActorRecords[actorIndex].posX == g_BattleActorRecords[otherIdx].posX && g_BattleActorRecords[actorIndex].posZ == g_BattleActorRecords[otherIdx].posZ) {
                    g_BattleSummonKotrActorRepresentative[otherIdx] = actorIndex;
                    g_BattleSummonKotrTargetMask |= (short)(1 << otherIdx);
                    g_BattleKotrHandledMask |= (short)(1 << otherIdx);
                }
            }
        }
    }
    Battle_AllocateResourceSlot(BattleFx_KotrMasterTimeline);
    BattleFx_KotrSpawnCameraScript((int)&g_BattleSummonKotrCameraScript, (int)&g_BattleSummonKotrOffscreenCenter, actorIdx);
    g_BattleStageHideFlags |= 8;
}

/*
 * Resets the Knights of the Round module state: ZeroFillMemory32 clears 0x108 words at
 * g_BattleSummonKotrKnightContexts, 8 at g_BattleSummonKotrEffectAngles, 0x1E at g_BattleSummonKotrOffscreenCenter and 0x1E at g_BattleSummonKotrTransformState, then
 * g_BattleKotrTargetMask, g_BattleSummonKotrTargetMask, g_BattleKotrHandledMask, g_BattleSummonKotrImpactRecordIndex and g_BattleSummonKotrKnightContextSlot are set to zero.
 */
// FUNCTION: FF7 0x00476a38
void __cdecl BattleFx_KotrResetModuleState(void)
{
    ZeroFillMemory32(0x108, &g_BattleSummonKotrKnightContexts);
    ZeroFillMemory32(8, &g_BattleSummonKotrEffectAngles);
    ZeroFillMemory32(0x1E, &g_BattleSummonKotrOffscreenCenter);
    ZeroFillMemory32(0x1E, &g_BattleSummonKotrTransformState);
    g_BattleKotrTargetMask = 0;
    g_BattleSummonKotrTargetMask = 0;
    g_BattleKotrHandledMask = 0;
    g_BattleSummonKotrImpactRecordIndex = 0;
    g_BattleSummonKotrKnightContextSlot = 0;
}

// Points g_BattleCameraScriptTaskCallback at BattleFx_KotrRunCameraScript and fills the dedicated
// record, task slot 99 (g_BattleTaskSlotBuffer.mainSlots[0x63]), for the camera script, clearing
// its ownerSlot. It increments g_BattleSecondaryTaskActiveCount, then stores eyeTrack and
// focusTrack as dwords and duration as a word.
// FUNCTION: FF7 0x00476aab
void __cdecl BattleFx_KotrSpawnCameraScript(int eyeTrack, int focusTrack, int duration)
{
    BattleFxKotrCameraScriptTask *task;

    g_BattleCameraScriptTaskCallback = BattleFx_KotrRunCameraScript;
    task = (BattleFxKotrCameraScriptTask *)(&g_BattleTaskSlotBuffer.mainSlots[99]);
    task->ownerSlot = 0;
    g_BattleSecondaryTaskActiveCount++;
    task->eyeTrack = eyeTrack;
    task->focusTrack = focusTrack;
    task->duration = duration;
}

/*
 * Interprets the extended Knights camera script, initializing both retained tracks from global
 * camera positions on the first unpaused call. While either track duration is zero consumes short
 * commands, selecting eye by bit 0x40: duration, absolute XYZ, linear velocity, distance-
 * following, acceleration, sine easing, two curved easing modes, actor/bone attachment or
 * detachment, suppression and global control writes. Unknown commands retire and return.
 * Attachment subtracts the transformed attachment point from track position; detachment adds it
 * back. Distance setup predicts a moving other track using the current track duration. Advances
 * eye before focus; acceleration falls through to linear motion, following normalizes the
 * direction from moving base to the other track, and curved modes combine eased target
 * displacement with perpendicular X/Z curvature, with mode seven using negative easing for its
 * lateral term. Decrements both durations. While paused retains interpreter state but still
 * publishes nonsuppressed tracks. With a task matrix transforms positions and attached offsets; an
 * unattached focus uses the previously installed matrix, including when eye publication is
 * suppressed. Without a task matrix copies track positions directly.
 */
// FUNCTION: FF7 0x00476afb
void __cdecl BattleFx_KotrRunCameraScript(void)
{
    BattleFxKotrCamera *camera;
    short cmd;
    int lateral;
    int padStorage[45]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int padValue[6]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    Vec3sPacked *offsetPtr;
    int padLocal; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unusedSlots[14]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int focusVec;
    BattleCameraScriptVec *displacement;
    SVECTOR *pOffsetVec;
    int progress;
    int track;
    int outPoint;

    camera = (BattleFxKotrCamera *)BATTLE_CURRENT_TASK_SLOT;
    pOffsetVec = (SVECTOR *)Gfx_GetPsxScratchpadPtr(0);
    offsetPtr = (Vec3sPacked *)((int)pOffsetVec + 8);
    displacement = (BattleCameraScriptVec *)((int)offsetPtr + 8);
    focusVec = (int)displacement + 0x10;
    if (g_BattleIsPaused == 0) {
        if (camera->started == 0) {
            g_BattleSummonKotrScriptStateB.scriptReady = 0;
            g_BattleSummonKotrScriptStateA.scriptReady = 0;
            (g_BattleSummonKotrActorAttachB.actorIndex) = 0xFFFF;
            (g_BattleSummonKotrActorAttachA.actorIndex) = 0xFFFF;
            camera->started = 1;
        }
        while (g_BattleSummonKotrScriptStateB.scriptReady == 0 || g_BattleSummonKotrScriptStateA.scriptReady == 0) {
            cmd = *camera->scriptPtr;
            camera->scriptPtr++;
            if ((cmd & 0x40) != 0) {
                g_BattleSummonKotrCameraState = (struct BattleFxCameraOrbitState *)&g_BattleSummonKotrScriptStateB;
                g_BattleSummonKotrCameraOther = (struct BattleFxCameraOrbitState *)&g_BattleSummonKotrScriptStateA;
            } else {
                g_BattleSummonKotrCameraState = (struct BattleFxCameraOrbitState *)&g_BattleSummonKotrScriptStateA;
                g_BattleSummonKotrCameraOther = (struct BattleFxCameraOrbitState *)&g_BattleSummonKotrScriptStateB;
            }
            g_BattleSummonKotrCameraState->mode = cmd & ~0xC0;
            switch (g_BattleSummonKotrCameraState->mode) {
            case BATTLE_KOTR_CAMERA_WAIT:
                g_BattleSummonKotrCameraState->stepCount = BattleFx_KotrFetchScriptCount(&camera->scriptPtr);
                break;
            case BATTLE_KOTR_CAMERA_SET_POSITION:
                g_BattleSummonKotrCameraState->posX = *camera->scriptPtr;
                camera->scriptPtr++;
                g_BattleSummonKotrCameraState->posY = *camera->scriptPtr;
                camera->scriptPtr++;
                g_BattleSummonKotrCameraState->posZ = *camera->scriptPtr;
                camera->scriptPtr++;
                break;
            case BATTLE_KOTR_CAMERA_MOVE_LINEAR:
                g_BattleSummonKotrCameraState->stepCount = BattleFx_KotrFetchScriptCount(&camera->scriptPtr);
                g_BattleSummonKotrCameraState->stepX = (*camera->scriptPtr - g_BattleSummonKotrCameraState->posX) / g_BattleSummonKotrCameraState->stepCount;
                camera->scriptPtr++;
                g_BattleSummonKotrCameraState->stepY = (*camera->scriptPtr - g_BattleSummonKotrCameraState->posY) / g_BattleSummonKotrCameraState->stepCount;
                camera->scriptPtr++;
                g_BattleSummonKotrCameraState->stepZ = (*camera->scriptPtr - g_BattleSummonKotrCameraState->posZ) / g_BattleSummonKotrCameraState->stepCount;
                camera->scriptPtr++;
                break;
            case BATTLE_KOTR_CAMERA_MOVE_ORBIT:
                pOffsetVec->vx = g_BattleSummonKotrCameraOther->posX - g_BattleSummonKotrCameraState->posX;
                pOffsetVec->vy = g_BattleSummonKotrCameraOther->posY - g_BattleSummonKotrCameraState->posY;
                pOffsetVec->vz = g_BattleSummonKotrCameraOther->posZ - g_BattleSummonKotrCameraState->posZ;
                g_BattleSummonKotrCameraState->progress = SqrtToInt(pOffsetVec->vx * pOffsetVec->vx + pOffsetVec->vy * pOffsetVec->vy + pOffsetVec->vz * pOffsetVec->vz);
                *(Int64Pair *)((char *)g_BattleSummonKotrCameraState + 0x18) = *(Int64Pair *)((char *)g_BattleSummonKotrCameraState + 0x10);
                g_BattleSummonKotrCameraState->stepCount = BattleFx_KotrFetchScriptCount(&camera->scriptPtr);
                pOffsetVec->vx = *camera->scriptPtr;
                camera->scriptPtr++;
                pOffsetVec->vy = *camera->scriptPtr;
                camera->scriptPtr++;
                pOffsetVec->vz = *camera->scriptPtr;
                camera->scriptPtr++;
                g_BattleSummonKotrCameraState->stepX = (pOffsetVec->vx - g_BattleSummonKotrCameraState->posX) / g_BattleSummonKotrCameraState->stepCount;
                g_BattleSummonKotrCameraState->stepY = (pOffsetVec->vy - g_BattleSummonKotrCameraState->posY) / g_BattleSummonKotrCameraState->stepCount;
                g_BattleSummonKotrCameraState->stepZ = (pOffsetVec->vz - g_BattleSummonKotrCameraState->posZ) / g_BattleSummonKotrCameraState->stepCount;
                switch (g_BattleSummonKotrCameraOther->mode) {
                case BATTLE_KOTR_CAMERA_MOVE_LINEAR:
                    offsetPtr->vx = g_BattleSummonKotrCameraOther->posX + g_BattleSummonKotrCameraOther->stepX * g_BattleSummonKotrCameraState->stepCount;
                    offsetPtr->vy = g_BattleSummonKotrCameraOther->posY + g_BattleSummonKotrCameraOther->stepY * g_BattleSummonKotrCameraState->stepCount;
                    offsetPtr->vz = g_BattleSummonKotrCameraOther->posZ + g_BattleSummonKotrCameraOther->stepZ * g_BattleSummonKotrCameraState->stepCount;
                    break;
                default:
                    offsetPtr->vx = g_BattleSummonKotrCameraOther->posX;
                    offsetPtr->vy = g_BattleSummonKotrCameraOther->posY;
                    offsetPtr->vz = g_BattleSummonKotrCameraOther->posZ;
                }
                pOffsetVec->vx -= offsetPtr->vx;
                pOffsetVec->vy -= offsetPtr->vy;
                pOffsetVec->vz -= offsetPtr->vz;
                g_BattleSummonKotrCameraState->stepSize = (SqrtToInt(pOffsetVec->vx * pOffsetVec->vx + pOffsetVec->vy * pOffsetVec->vy + pOffsetVec->vz * pOffsetVec->vz) - g_BattleSummonKotrCameraState->progress) / g_BattleSummonKotrCameraState->stepCount;
                break;
            case BATTLE_KOTR_CAMERA_MOVE_ACCEL:
                g_BattleSummonKotrCameraState->stepCount = BattleFx_KotrFetchScriptCount(&camera->scriptPtr);
                pOffsetVec->vx = *camera->scriptPtr - g_BattleSummonKotrCameraState->posX;
                camera->scriptPtr++;
                pOffsetVec->vy = *camera->scriptPtr - g_BattleSummonKotrCameraState->posY;
                camera->scriptPtr++;
                pOffsetVec->vz = *camera->scriptPtr - g_BattleSummonKotrCameraState->posZ;
                camera->scriptPtr++;
                g_BattleSummonKotrCameraState->deltaX = (pOffsetVec->vx - g_BattleSummonKotrCameraState->stepX * g_BattleSummonKotrCameraState->stepCount << 1) / (g_BattleSummonKotrCameraState->stepCount * g_BattleSummonKotrCameraState->stepCount);
                g_BattleSummonKotrCameraState->deltaY = (pOffsetVec->vy - g_BattleSummonKotrCameraState->stepY * g_BattleSummonKotrCameraState->stepCount << 1) / (g_BattleSummonKotrCameraState->stepCount * g_BattleSummonKotrCameraState->stepCount);
                g_BattleSummonKotrCameraState->deltaZ = (pOffsetVec->vz - g_BattleSummonKotrCameraState->stepZ * g_BattleSummonKotrCameraState->stepCount << 1) / (g_BattleSummonKotrCameraState->stepCount * g_BattleSummonKotrCameraState->stepCount);
                break;
            case BATTLE_KOTR_CAMERA_MOVE_EASED:
                g_BattleSummonKotrCameraState->stepCount = BattleFx_KotrFetchScriptCount(&camera->scriptPtr);
                g_BattleSummonKotrCameraState[1].mode = *camera->scriptPtr;
                camera->scriptPtr++;
                g_BattleSummonKotrCameraState->progress = 0;
                g_BattleSummonKotrCameraState->stepSize = 0x1000 / g_BattleSummonKotrCameraState->stepCount;
                g_BattleSummonKotrCameraState->deltaX = g_BattleSummonKotrCameraState->posX;
                g_BattleSummonKotrCameraState->deltaY = g_BattleSummonKotrCameraState->posY;
                g_BattleSummonKotrCameraState->deltaZ = g_BattleSummonKotrCameraState->posZ;
                g_BattleSummonKotrCameraState->stepX = *camera->scriptPtr - g_BattleSummonKotrCameraState->posX;
                camera->scriptPtr++;
                g_BattleSummonKotrCameraState->stepY = *camera->scriptPtr - g_BattleSummonKotrCameraState->posY;
                camera->scriptPtr++;
                g_BattleSummonKotrCameraState->stepZ = *camera->scriptPtr - g_BattleSummonKotrCameraState->posZ;
                camera->scriptPtr++;
                break;
            case BATTLE_KOTR_CAMERA_MOVE_ARC:
            case BATTLE_KOTR_CAMERA_MOVE_ARC_NEGATED_EASE:
                g_BattleSummonKotrCameraState->stepCount = BattleFx_KotrFetchScriptCount(&camera->scriptPtr);
                g_BattleSummonKotrCameraState[1].mode = *camera->scriptPtr;
                camera->scriptPtr++;
                g_BattleSummonKotrCameraState[1].stepCount = *camera->scriptPtr;
                camera->scriptPtr++;
                g_BattleSummonKotrCameraState->progress = 0;
                g_BattleSummonKotrCameraState->stepSize = 0x1000 / g_BattleSummonKotrCameraState->stepCount;
                g_BattleSummonKotrCameraState->deltaX = g_BattleSummonKotrCameraState->posX;
                g_BattleSummonKotrCameraState->deltaY = g_BattleSummonKotrCameraState->posY;
                g_BattleSummonKotrCameraState->deltaZ = g_BattleSummonKotrCameraState->posZ;
                g_BattleSummonKotrCameraState->stepX = *camera->scriptPtr - g_BattleSummonKotrCameraState->posX;
                camera->scriptPtr++;
                g_BattleSummonKotrCameraState->stepY = *camera->scriptPtr - g_BattleSummonKotrCameraState->posY;
                camera->scriptPtr++;
                g_BattleSummonKotrCameraState->stepZ = *camera->scriptPtr - g_BattleSummonKotrCameraState->posZ;
                camera->scriptPtr++;
                break;
            case BATTLE_KOTR_CAMERA_ATTACH:
                g_BattleSummonKotrCameraState->attachPoint = *camera->scriptPtr;
                camera->scriptPtr++;
                g_BattleSummonKotrCameraState->attachIndex = *camera->scriptPtr;
                camera->scriptPtr++;
                BattleFx_KotrAttachPointToLocal(g_BattleSummonKotrCameraState->attachPoint, g_BattleSummonKotrCameraState->attachIndex, camera->rotMatrix, pOffsetVec);
                g_BattleSummonKotrCameraState->posX -= pOffsetVec->vx;
                g_BattleSummonKotrCameraState->posY -= pOffsetVec->vy;
                g_BattleSummonKotrCameraState->posZ -= pOffsetVec->vz;
                break;
            case BATTLE_KOTR_CAMERA_DETACH:
                BattleFx_KotrAttachPointToLocal(g_BattleSummonKotrCameraState->attachPoint, g_BattleSummonKotrCameraState->attachIndex, camera->rotMatrix, pOffsetVec);
                g_BattleSummonKotrCameraState->posX += pOffsetVec->vx;
                g_BattleSummonKotrCameraState->posY += pOffsetVec->vy;
                g_BattleSummonKotrCameraState->posZ += pOffsetVec->vz;
                g_BattleSummonKotrCameraState->attachPoint = 0xFFFF;
                break;
            case BATTLE_KOTR_CAMERA_HOLD_OUTPUT:
                g_BattleSummonKotrCameraState->stepCount = BattleFx_KotrFetchScriptCount(&camera->scriptPtr);
                break;
            case BATTLE_KOTR_CAMERA_SET_SCRIPT_VALUE:
                g_BattleCameraScriptValue = *camera->scriptPtr;
                camera->scriptPtr++;
                break;
            default:
                camera->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        g_BattleSummonKotrCameraState = (struct BattleFxCameraOrbitState *)&g_BattleSummonKotrScriptStateB;
        g_BattleSummonKotrCameraOther = (struct BattleFxCameraOrbitState *)&g_BattleSummonKotrScriptStateA;
        for (track = 0; track < 2; track++) {
            switch (g_BattleSummonKotrCameraState->mode) {
            case BATTLE_KOTR_CAMERA_WAIT:
                break;
            case BATTLE_KOTR_CAMERA_MOVE_ACCEL:
                g_BattleSummonKotrCameraState->stepX += g_BattleSummonKotrCameraState->deltaX;
                g_BattleSummonKotrCameraState->stepY += g_BattleSummonKotrCameraState->deltaY;
                g_BattleSummonKotrCameraState->stepZ += g_BattleSummonKotrCameraState->deltaZ;
            case BATTLE_KOTR_CAMERA_MOVE_LINEAR:
                g_BattleSummonKotrCameraState->posX += g_BattleSummonKotrCameraState->stepX;
                g_BattleSummonKotrCameraState->posY += g_BattleSummonKotrCameraState->stepY;
                g_BattleSummonKotrCameraState->posZ += g_BattleSummonKotrCameraState->stepZ;
                break;
            case BATTLE_KOTR_CAMERA_MOVE_ORBIT:
                g_BattleSummonKotrCameraState->deltaX += g_BattleSummonKotrCameraState->stepX;
                g_BattleSummonKotrCameraState->deltaY += g_BattleSummonKotrCameraState->stepY;
                g_BattleSummonKotrCameraState->deltaZ += g_BattleSummonKotrCameraState->stepZ;
                g_BattleSummonKotrCameraState->progress += g_BattleSummonKotrCameraState->stepSize;
                ((BattleFxCameraVec *)displacement)->x = g_BattleSummonKotrCameraOther->posX - g_BattleSummonKotrCameraState->deltaX;
                ((BattleFxCameraVec *)displacement)->y = g_BattleSummonKotrCameraOther->posY - g_BattleSummonKotrCameraState->deltaY;
                ((BattleFxCameraVec *)displacement)->z = g_BattleSummonKotrCameraOther->posZ - g_BattleSummonKotrCameraState->deltaZ;
                VectorNormalToSVector((int *)displacement, (short *)pOffsetVec);
                g_BattleSummonKotrCameraState->posX = g_BattleSummonKotrCameraOther->posX - (pOffsetVec->vx * g_BattleSummonKotrCameraState->progress >> 0xC);
                g_BattleSummonKotrCameraState->posY = g_BattleSummonKotrCameraOther->posY - (pOffsetVec->vy * g_BattleSummonKotrCameraState->progress >> 0xC);
                g_BattleSummonKotrCameraState->posZ = g_BattleSummonKotrCameraOther->posZ - (pOffsetVec->vz * g_BattleSummonKotrCameraState->progress >> 0xC);
                break;
            case BATTLE_KOTR_CAMERA_MOVE_EASED:
                g_BattleSummonKotrCameraState->progress += g_BattleSummonKotrCameraState->stepSize;
                progress = g_BattleSummonKotrCameraState->progress;
                progress = BattleFx_ApplySineEasing(progress, g_BattleSummonKotrCameraState[1].mode);
                g_BattleSummonKotrCameraState->posX = g_BattleSummonKotrCameraState->deltaX + (g_BattleSummonKotrCameraState->stepX * progress >> 0xC);
                g_BattleSummonKotrCameraState->posY = g_BattleSummonKotrCameraState->deltaY + (g_BattleSummonKotrCameraState->stepY * progress >> 0xC);
                g_BattleSummonKotrCameraState->posZ = g_BattleSummonKotrCameraState->deltaZ + (g_BattleSummonKotrCameraState->stepZ * progress >> 0xC);
                break;
            case BATTLE_KOTR_CAMERA_MOVE_ARC:
                g_BattleSummonKotrCameraState->progress += g_BattleSummonKotrCameraState->stepSize;
                progress = g_BattleSummonKotrCameraState->progress;
                lateral = SinFixed(BattleFx_ApplySineEasing(progress, g_BattleSummonKotrCameraState[1].mode) / 2);
                lateral = lateral * g_BattleSummonKotrCameraState[1].stepCount >> 0xC;
                progress = BattleFx_ApplySineEasing(progress, g_BattleSummonKotrCameraState[1].mode);
                g_BattleSummonKotrCameraState->posX = g_BattleSummonKotrCameraState->deltaX + (g_BattleSummonKotrCameraState->stepX * progress >> 0xC) - (g_BattleSummonKotrCameraState->stepZ * lateral >> 0xC);
                g_BattleSummonKotrCameraState->posY = g_BattleSummonKotrCameraState->deltaY + (g_BattleSummonKotrCameraState->stepY * progress >> 0xC);
                g_BattleSummonKotrCameraState->posZ = g_BattleSummonKotrCameraState->deltaZ + (g_BattleSummonKotrCameraState->stepZ * progress >> 0xC) + (g_BattleSummonKotrCameraState->stepX * lateral >> 0xC);
                break;
            case BATTLE_KOTR_CAMERA_MOVE_ARC_NEGATED_EASE:
                g_BattleSummonKotrCameraState->progress += g_BattleSummonKotrCameraState->stepSize;
                progress = g_BattleSummonKotrCameraState->progress;
                lateral = SinFixed(BattleFx_ApplySineEasing(progress, -g_BattleSummonKotrCameraState[1].mode) / 2);
                lateral = lateral * g_BattleSummonKotrCameraState[1].stepCount >> 0xC;
                progress = BattleFx_ApplySineEasing(progress, g_BattleSummonKotrCameraState[1].mode);
                g_BattleSummonKotrCameraState->posX = g_BattleSummonKotrCameraState->deltaX + (g_BattleSummonKotrCameraState->stepX * progress >> 0xC) - (g_BattleSummonKotrCameraState->stepZ * lateral >> 0xC);
                g_BattleSummonKotrCameraState->posY = g_BattleSummonKotrCameraState->deltaY + (g_BattleSummonKotrCameraState->stepY * progress >> 0xC);
                g_BattleSummonKotrCameraState->posZ = g_BattleSummonKotrCameraState->deltaZ + (g_BattleSummonKotrCameraState->stepZ * progress >> 0xC) + (g_BattleSummonKotrCameraState->stepX * lateral >> 0xC);
            }
            (g_BattleSummonKotrCameraState->stepCount)--;
            g_BattleSummonKotrCameraState = (struct BattleFxCameraOrbitState *)&g_BattleSummonKotrScriptStateA;
            g_BattleSummonKotrCameraOther = (struct BattleFxCameraOrbitState *)&g_BattleSummonKotrScriptStateB;
        }
    }
    if (camera->rotMatrix != 0) {
        if ((g_BattleSummonKotrScriptStateB.scriptState) != BATTLE_KOTR_CAMERA_HOLD_OUTPUT) {
            SetRotMatrix(camera->rotMatrixPtr);
            SetTransMatrix(camera->rotMatrixPtr);
            if ((g_BattleSummonKotrActorAttachB.actorIndex) == -1) {
                RotTransPoint(&g_BattleSummonKotrCameraOffset, (int *)displacement, &outPoint);
            } else {
                BattleFx_KotrAttachPointToLocal((g_BattleSummonKotrActorAttachB.actorIndex), g_BattleSummonKotrActorAttachB.attachIndex, camera->rotMatrix, pOffsetVec);
                pOffsetVec->vx += ((BattleFxTimeDamageWarpPoint *)&g_BattleSummonKotrCameraOffset)->offsetX;
                pOffsetVec->vy += (((BattleFxTimeDamageWarpPoint *)&g_BattleSummonKotrCameraOffset)->offsetY);
                pOffsetVec->vz += *(short *)&g_BattleSummonKotrCameraEyeOffsetZ;
                SetRotMatrix(camera->rotMatrixPtr);
                SetTransMatrix(camera->rotMatrixPtr);
                RotTransPoint(pOffsetVec, (int *)displacement, &outPoint);
            }
            g_BattleCameraPosition.cameraPosX = displacement->x;
            g_BattleCameraPosition.cameraPosY = displacement->y;
            g_BattleCameraPosZ = displacement->z;
        }
        if ((g_BattleSummonKotrScriptStateA.scriptState) != BATTLE_KOTR_CAMERA_HOLD_OUTPUT) {
            if ((g_BattleSummonKotrActorAttachA.actorIndex) == -1) {
                RotTransPoint(&g_BattleKotrCameraOffset, (int *)displacement, &outPoint);
            } else {
                BattleFx_KotrAttachPointToLocal((g_BattleSummonKotrActorAttachA.actorIndex), g_BattleSummonKotrActorAttachA.attachIndex, camera->rotMatrix, pOffsetVec);
                pOffsetVec->vx += ((BattleFxTimeDamageWarpPoint *)&g_BattleKotrCameraOffset)->offsetX;
                pOffsetVec->vy += (((BattleFxTimeDamageWarpPoint *)&g_BattleKotrCameraOffset)->offsetY);
                pOffsetVec->vz += *(short *)&g_BattleSummonKotrCameraFocusOffsetZ;
                SetRotMatrix(camera->rotMatrixPtr);
                SetTransMatrix(camera->rotMatrixPtr);
                RotTransPoint(pOffsetVec, (int *)displacement, &outPoint);
            }
            g_BattleCameraFocalPoint.focalPointX = displacement->x;
            g_BattleCameraFocalPoint.focalPointY = displacement->y;
            g_BattleCameraFocusZ = displacement->z;
        }
    } else {
        if ((g_BattleSummonKotrScriptStateB.scriptState) != BATTLE_KOTR_CAMERA_HOLD_OUTPUT) {
            *(int *)&g_BattleCameraPosition = g_BattleSummonKotrCameraOffset;
            *(int *)&g_BattleCameraPosZ = g_BattleSummonKotrCameraEyeOffsetZ;
        }
        if ((g_BattleSummonKotrScriptStateA.scriptState) != BATTLE_KOTR_CAMERA_HOLD_OUTPUT) {
            *(int *)&g_BattleCameraFocalPoint = g_BattleKotrCameraOffset;
            *(int *)&g_BattleCameraFocusZ = g_BattleSummonKotrCameraFocusOffsetZ;
        }
    }
}

/*
 * Fetches one script operand: reads the short at the
 * cursor **index, advances that cursor by 2, and when the value is negative uses its negation as an
 * index into the short array at g_BattleSummonKotrTumbleEmitterInit.kotrScriptCountEntry and returns
 * that element. Returns the resolved short. Identical in shape to
 * BattleFx_GreatGospelReadCameraScriptOperand, which uses a different table.
 */
// FUNCTION: FF7 0x00477e35
short __cdecl BattleFx_KotrFetchScriptCount(short **index)
{
    int val;

    val = **index;
    (*index)++;
    if (val < 0) {
        val = (&g_BattleSummonKotrTumbleEmitterInit.kotrScriptCountEntry)[-val];
    }
    return (short)val;
}

/*
 * Pure function of (value, easeMode) and SinFixed. With easeMode > 0 applies value = SinFixed(value / 4)
 * (easeMode >> 8) times; with easeMode < 0 negates easeMode, mirrors value about 0x1000, applies the same
 * iteration and mirrors back; with easeMode == 0 returns value unchanged. Otherwise returns the blend
 * (value * b + origValue * (0x100 - b)) >> 8 with b = (easeMode & 0xFF) + 1.
 */
// FUNCTION: FF7 0x00477e6f
int __cdecl BattleFx_ApplySineEasing(int value, int easeMode)
{
    int iteration;
    int origValue;
    int passCount;

    origValue = value;
    if (easeMode > 0) {
        passCount = easeMode >> 8;
        for (iteration = 0; iteration < passCount; iteration++) {
            value = SinFixed(value / 4);
        }
    } else if (easeMode < 0) {
        easeMode = -easeMode;
        passCount = easeMode >> 8;
        value = 0x1000 - value;
        for (iteration = 0; iteration < passCount; iteration++) {
            value = SinFixed(value / 4);
        }
        value = 0x1000 - value;
    } else {
        return value;
    }
    passCount = (easeMode & 0xFF) + 1;
    value = value * passCount + origValue * (0x100 - passCount) >> 8;
    return value;
}

/*
 * When attachIndex is not -1, delegates to BattleFx_KotrTransformAttachPointToLocal(attachIndex, localOffset). Otherwise copies the composedPos
 * shorts (x, y, z) of knight context recordIndex (stride 0x84) into localOffset->vx, vy and vz, so the
 * local coordinate falls back to the record's own position. reserved is unused, and the frame reserves
 * eight further slots the original never reads.
 */
// FUNCTION: FF7 0x00477f5b
void __cdecl BattleFx_KotrAttachPointToLocal(int recordIndex, int attachIndex, int reserved, SVECTOR *localOffset)
{
    int padSlot[8]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if (attachIndex != -1) {
        BattleFx_KotrTransformAttachPointToLocal(attachIndex, localOffset);
    } else {
        localOffset->vx = ((BattleKotrKnightComposedPos *)&g_BattleSummonKotrKnightComposedPos)[recordIndex].x;
        localOffset->vy = ((BattleKotrKnightComposedPos *)&g_BattleSummonKotrKnightComposedPos)[recordIndex].y;
        localOffset->vz = ((BattleKotrKnightComposedPos *)&g_BattleSummonKotrKnightComposedPos)[recordIndex].z;
    }
}

/*
 * Reads the actor 3 attachment matrix,
 * subtracts the translation of g_BattleSummonKotrTransformState from the attachment matrix's
 * translation into localVec, transposes g_BattleSummonKotrTransformState into local
 * scratch, and transforms the output vector in place.
 */
// FUNCTION: FF7 0x00477fc1
void __cdecl BattleFx_KotrTransformAttachPointToLocal(int attachIndex, SVECTOR *localVec)
{
    BattleKotrAttachMatrices attachMatrix;

    Battle3d_GetActorAttachMatrix(3, attachIndex, &attachMatrix.attach);
    localVec->vx = attachMatrix.attach.t[0] - g_BattleSummonKotrTransformState.t[0];
    localVec->vy = attachMatrix.attach.t[1] - g_BattleSummonKotrTransformState.t[1];
    localVec->vz = attachMatrix.attach.t[2] - g_BattleSummonKotrTransformState.t[2];
    TransposeMatrix(&g_BattleSummonKotrTransformState, &attachMatrix.transposed);
    TransformVectorToInt16(&attachMatrix.transposed, localVec, (short *)localVec);
}

/*
 * Runs the Knights master timeline using destructive
 * subtraction of successive stage lengths, retaining empty and zero-length stages. On every call,
 * including pause, alternates the 0x5A00 scratch bank, clears its offset, refreshes resource field
 * fifteen and multiplies the retained matrices. Unpaused stages schedule opening effects, masked
 * actor callbacks, sound dispatch and a thirty-count ramp; set battle flag bits zero through two
 * and hide unselected actors four through nine with status bit four. Successive stages dispatch
 * thirteen knight callbacks through alternating 0x84 contexts, interleaving supporting tasks,
 * delayed sounds and cached-buffer releases. Later clears the control short and flag bits, restores
 * each unselected actor status bit four from saved bytes and schedules
 * BattleFx_KotrFragmentSpinScatterFadeTimelineTask; subsequent stages dispatch final sounds and
 * retain explicit no-op calls. At exhaustion allocates Battle3d_Slot3ActorWaitEffectTask,
 * clears battle flags, retires the task, calls BattleFx_KotrFreeThirteenKnightModels, releases a
 * nonnegative offscreen slot and sets it to minus one, then releases the two final cached buffers.
 * Advances the task short counter even after retirement.
 */
// FUNCTION: FF7 0x00478031
void __cdecl BattleFx_KotrMasterTimeline(void)
{
    /* The original walks a destructive countdown, including zero-length stages.
       Keep the frame fields in original stack order. */
    struct {
        int spawned;
        int unused18;
        int unused14;
        int actor;
        BattleTaskSlot *task;
        int frame;
        int duration;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */


#define KOTR_TASK(callback) \
    local.spawned = (int)BATTLE_NEW_TASK_SLOT((BattleTaskFn)(callback))
#define KOTR_KNIGHT(callback, index)                                       \
    BattleFx_KotrSpawnKnight(            \
        (BattleKotrKnight *)((int)&g_BattleSummonKotrKnightContexts + g_BattleSummonKotrKnightContextSlot * 0x84), (int)(callback), index); \
    g_BattleSummonKotrKnightContextSlot ^= 1
#define KOTR_SOUND(delay, index, first, second) \
    local.duration = delay;                     \
    BattleFx_KotrSpawnDelayedSoundTask(local.duration, index, first, second)

    local.task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSummonKotrLayerDrawListHead = (int)&g_BattleSummonKotrLayerDrawBuffers + g_BattleSummonKotrLayerDrawBufferIndex * 0x5a00;
    g_BattleSummonKotrLayerDrawBufferIndex ^= 1;
    g_BattleKotrLayerCount = 0;
    Gfx_ModelTableGetRenderObject(15, g_BattleSummonKotrTableCursor);
    MultiplyMatrix((int *)&g_BattleCameraTransform.matrix[0], (int *)&g_BattleSummonKotrOffscreenCenter, &g_BattleSummonKotrTransformState);
    if (g_BattleIsPaused) { /* empty first arm: lib/codegen.h */
    } else {
        local.frame = local.task->frameCounter;
        if (local.frame < 5) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 5) < 20) {
            if (local.frame == 10) {
                KOTR_TASK(BattleFx_KotrRadialDiscMeshTimelineTask);
                KOTR_TASK(BattleFx_KotrEmitFadingOffscreenQuadParticlesTask);
                Battle_CreateBitPatternCallbackTask(g_BattleKotrTargetMask, 0, 0, BattleFx_KotrSpawnActorLiftPerTarget);
                if (g_BattleSummonKotrTargetMask & ~g_BattleKotrTargetMask)
                    Battle_CreateBitPatternCallbackTask(g_BattleSummonKotrTargetMask & ~g_BattleKotrTargetMask, 0, 0, BattleFx_KotrSpawnActorLiftPerTarget);
                BattleFx_KotrSpawnShakeEnvelope(50, 0, 10, 30);
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 0);
            }
        } else if ((local.frame -= 20) < 30) {
            if (local.frame == 0) {
                KOTR_TASK(BattleFx_KotrSpinningModelEntry15Task);
            }
        } else if ((local.frame -= 30) < 30) {
            if (local.frame == 0) {
                g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2 = 0;
                Battle_StartFixedPointRamp(4096, 30);
            }
            if (local.frame == 29) {
                g_BattleStageHideFlags |= 7;
                for (local.actor = 4; local.actor < 10; local.actor++) {
                    if ((g_BattleSummonKotrTargetMask & (1 << local.actor)) == 0)
                        *((unsigned char *)&g_BattleActorRecords[local.actor].drawFlags) |= 4;
                }
            }
            if (local.frame == 28)
                Battle_TaskNop();
        } else if ((local.frame -= 30) < 25) {
            if (local.frame == 0) {
                KOTR_TASK(BattleFx_KotrSpawnFirstStackedModelLayerPairTask);
                KOTR_SOUND(20, 1, 0x2b5, 0x2d2);
            }
        } else if ((local.frame -= 25) < 5) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 5) < 5) {
            if (local.frame == 0) {
                KOTR_KNIGHT(BattleFx_KotrKnight1TimelineTask, 1);
            }
        } else if ((local.frame -= 5) < 49) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 49) < 0) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 0) < 32) {
            if (local.frame == 0) {
                KOTR_KNIGHT(BattleFx_KotrKnight2TimelineTask, 2);
                KOTR_SOUND(20, 2, 0x2b7, 0);
                Sfx_ReleaseCachedBuffer(0x2b1, 0);
            }
        } else if ((local.frame -= 32) < 27) {
            if (local.frame == 0) {
                KOTR_TASK(BattleFx_KotrSpawnSecondStackedModelLayerPairTask);
            }
        } else if ((local.frame -= 27) < 33) {
            if (local.frame == 0) {
                KOTR_KNIGHT(BattleFx_KotrKnight3TimelineTask, 3);
                KOTR_SOUND(5, 3, 0x2b9, 0x2d3);
                Sfx_ReleaseCachedBuffer(0x2b4, 1);
            }
        } else if ((local.frame -= 33) < 11) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 11) < 34) {
            if (local.frame == 0) {
                KOTR_KNIGHT(BattleFx_KotrKnight4TimelineTask, 4);
                KOTR_SOUND(35, 4, 0x2bb, 0);
                Sfx_ReleaseCachedBuffer(0x2b6, 2);
            }
        } else if ((local.frame -= 34) < 30) {
            if (local.frame == 0) {
                KOTR_TASK(BattleFx_KotrSpawnConvergingStreakEmitterAndLayerTask);
            }
        } else if ((local.frame -= 30) < 0) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 0) < 35) {
            if (local.frame == 0) {
                KOTR_KNIGHT(BattleFx_KotrKnight5TimelineTask, 5);
                KOTR_SOUND(0, 5, 0x2bd, 0);
                Sfx_ReleaseCachedBuffer(0x2b8, 3);
            }
        } else if ((local.frame -= 35) < 16) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 16) < 5) {
            if (local.frame == 0) {
                KOTR_KNIGHT(BattleFx_KotrKnight6TimelineTask, 6);
                KOTR_SOUND(0, 6, 0x2bf, 0);
                Sfx_ReleaseCachedBuffer(0x2ba, 4);
            }
        } else if ((local.frame -= 5) < 37) {
            if (local.frame == 0) {
                KOTR_TASK(BattleFx_KotrFadingScrollingPrimitiveSpawnTimeline);
            }
        } else if ((local.frame -= 37) < 6) {
            if (local.frame == 0) {
                KOTR_KNIGHT(BattleFx_KotrKnight7TimelineTask, 7);
                KOTR_SOUND(0, 7, 0x2c1, 0x2d4);
                Sfx_ReleaseCachedBuffer(0x2bc, 5);
            }
        } else if ((local.frame -= 6) < 58) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 58) < 7) {
            if (local.frame == 0) {
                KOTR_KNIGHT(BattleFx_KotrKnight8TimelineTask, 8);
                KOTR_SOUND(10, 8, 0x2c3, 0);
                Sfx_ReleaseCachedBuffer(0x2be, 6);
            }
        } else if ((local.frame -= 7) < 36) {
            if (local.frame == 0) {
                KOTR_TASK(BattleFx_KotrSpawnSpinningModelsAndLayerTask);
            }
        } else if ((local.frame -= 36) < 8) {
            if (local.frame == 0) {
                KOTR_KNIGHT(BattleFx_KotrKnight9TimelineTask, 9);
                KOTR_SOUND(0, 9, 0x2c5, 0);
                Sfx_ReleaseCachedBuffer(0x2d4, 7);
            }
        } else if ((local.frame -= 8) < 108) {
            if (local.frame == 0) {
                KOTR_TASK(BattleFx_KotrSpawnSpinningFadingModelTask);
            }
        } else if ((local.frame -= 108) < 0) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 0) < 0) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 0) < 9) {
            if (local.frame == 0) {
                KOTR_KNIGHT(BattleFx_KotrKnight10TimelineTask, 10);
                KOTR_SOUND(15, 10, 0x2c7, 0);
                Sfx_ReleaseCachedBuffer(0x2c2, 8);
            }
        } else if ((local.frame -= 9) < 57) {
            if (local.frame == 0) {
                KOTR_TASK(BattleFx_KotrSpawnRadialStreakEmitterTask);
            }
        } else if ((local.frame -= 57) < 0) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 0) < 0) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 0) < 10) {
            if (local.frame == 0) {
                KOTR_KNIGHT(BattleFx_KotrKnight11TimelineTask, 11);
                KOTR_SOUND(20, 11, 0x2c9, 0);
                Sfx_ReleaseCachedBuffer(0x2c4, 9);
            }
        } else if ((local.frame -= 10) < 51) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 51) < 0) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 0) < 11) {
            if (local.frame == 0) {
                KOTR_KNIGHT(BattleFx_KotrKnight12TimelineTask, 12);
                KOTR_SOUND(30, 12, 0x2cb, 0);
                Sfx_ReleaseCachedBuffer(0x2c6, 10);
            }
        } else if ((local.frame -= 11) < 60) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 60) < 15) {
            if (local.frame == 0) {
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 13);
                Sfx_ReleaseCachedBuffer(0x2c8, 11);
            }
        } else if ((local.frame -= 15) < 15) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 15) < 15) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 15) < 48) {
            if (local.frame == 0) {
                KOTR_KNIGHT(BattleFx_KotrKnight13TimelineTask, 13);
                KOTR_SOUND(5, 14, 0x2d0, 0x2d1);
                Sfx_ReleaseCachedBuffer(0x2ca, 12);
            }
        } else if ((local.frame -= 48) < 18) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 18) < 54) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 54) < 10) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 10) < 5) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 5) < 5) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 5) < 5) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 5) < 10) {
            if (local.frame == 0) {
                g_BattleFxTimelineState.startValue = 0;
                g_BattleStageHideFlags &= 0xf8;
                for (local.actor = 4; local.actor < 10; local.actor++) {
                    if ((g_BattleSummonKotrTargetMask & (1 << local.actor)) == 0)
                        *((char *)&g_BattleActorRecords[local.actor].drawFlags) =
                            (*((unsigned char *)&g_BattleActorRecords[local.actor].drawFlags) & ~4) |
                            (g_BattleSummonKotrSavedActorDrawFlags[local.actor] & 4);
                }
                KOTR_TASK(BattleFx_KotrFragmentSpinScatterFadeTimelineTask);
            }
        } else if ((local.frame -= 10) < 10) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 10) < 10) { /* empty first arm: lib/codegen.h */
        } else if ((local.frame -= 10) < 45) {
            if (local.frame == 10) {
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 15);
                Sfx_ReleaseCachedBuffer(0x2cc, 13);
            }
        } else if ((local.frame -= 45) < 15) {
            if (local.frame == 13)
                Battle_TaskNop();
        } else {
            Battle_AllocateSecondaryResourceSlot(Battle3d_Slot3ActorWaitEffectTask);
            g_BattleStageHideFlags = 0;
            local.task->ownerSlot = BATTLE_TASK_FREE;
            BattleFx_KotrFreeThirteenKnightModels();
            if (g_BattleSummonKotrOffscreenSlotIndex >= 0) {
                Battle3d_ReleaseOffscreenSlot(g_BattleSummonKotrOffscreenSlotIndex);
                g_BattleSummonKotrOffscreenSlotIndex = -1;
            }
            Sfx_ReleaseCachedBuffer(0x2cf, 14);
            Sfx_ReleaseCachedBuffer(0x2d8, 15);
        }
        (local.task->frameCounter)++;
    }
#undef KOTR_TASK
#undef KOTR_KNIGHT
#undef KOTR_SOUND
}

/*
 * Allocates BattleFx_KotrShakeEnvelopeTask and stores its four arguments as words at +4/+6/+8/+0xA.
 */
// FUNCTION: FF7 0x00478d67
void __cdecl BattleFx_KotrSpawnShakeEnvelope(int amplitude, int duration, int rate, int decay)
{
    BattleFxKotrShakeEnvelopeTask *taskPtr;

    taskPtr = (BattleFxKotrShakeEnvelopeTask *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrShakeEnvelopeTask);
    taskPtr->amplitude = amplitude;
    taskPtr->duration = duration;
    taskPtr->rate = rate;
    taskPtr->decay = decay;
}

/*
 * Returns while paused. Computes an amplitude from task
 * short four through consecutive rise, hold and fall durations at six, eight and 0xA: linear
 * fixed-point rise, full amplitude hold, then linear fall using 4096 minus the fraction. On
 * exhaustion retires and clears g_BattleCameraTransform.posY without advancing count. Otherwise
 * writes positive amplitude on odd task counts and negative amplitude on even counts to that
 * global, then increments the short count.
 */
// FUNCTION: FF7 0x00478db6
void __cdecl BattleFx_KotrShakeEnvelopeTask(void)
{
    int amplitude;
    BattleFxKotrShakeEnvelope *task;
    int relFrame;
    int ratio;
    int signedAmp;

    task = (BattleFxKotrShakeEnvelope *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    relFrame = task->frame;
    if (relFrame < task->attackFrames) {
        ratio = (relFrame << 0xC) / task->attackFrames;
        amplitude = task->amplitude * ratio >> 0xC;
    } else {
        relFrame -= task->attackFrames;
        if (relFrame < task->sustainFrames) {
            amplitude = task->amplitude;
        } else {
            relFrame -= task->sustainFrames;
            if (relFrame < task->releaseFrames) {
                ratio = (relFrame << 0xC) / task->releaseFrames;
                amplitude = task->amplitude * (0x1000 - ratio) >> 0xC;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                g_BattleCameraTransform.posY = 0;
                return;
            }
        }
    }
    if ((task->frame & 1) != 0) {
        signedAmp = amplitude;
    } else {
        signedAmp = -amplitude;
    }
    g_BattleCameraTransform.posY = signedAmp;
    (task->frame)++;
}

/*
 * Acquires retained data block once. At count
 * zero calls BattleFx_KotrPrepareStageOffscreenRequest and BattleFx_KotrBuildRadialDiscMesh and
 * increments count even while paused. Counts one through 39 update
 * BattleFx_KotrApplyDiscMeshDisplacement only when unpaused using two sine-eased
 * displacement/colour phases, then install g_BattleSummonKotrTransformState, select the offscreen
 * render object and project sixteen rows of 32 records. For flags lacking 0x60000 copies
 * neighbouring row colour words into the selected primitive bank and submits it, then toggles bank
 * even while paused. Counts forty through 69 clear five scratch shorts and project 32 triangular
 * sectors of radius 0x11D0; accepted projections repeatedly write two constant words at the same
 * render pointer without advancing or submitting it here, then publish that pointer. Subsequent
 * countdown tests subtract ten and thirty, retaining otherwise empty phases until count 110; later
 * retires and returns. Only normal post-initialization count advancement and deformation are
 * pause-gated.
 */
// FUNCTION: FF7 0x00478edf
void __cdecl BattleFx_KotrRadialDiscMeshTimelineTask(void)
{
    int frameIndex;
    int tintOuter;
    BattleTaskSlot *pTimelineTask;
    int row;
    int eased;
    KotrRenderRecord *cellPtr;
    int column;
    int angleStart;
    int angleEnd;
    BattleFxRadialDiscPacket *packet;
    int colorA;

    pTimelineTask = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrRadialDiscMeshTimelineTaskInit & 1) == 0) {
        g_BattleKotrRadialDiscMeshTimelineTaskInit |= 1;
        g_BattleSummonKotrRadialDisc = (struct BattleFxKotrRadialDisc *)(Gfx_GetPsxScratchpadPtr(0));
    }
    if (pTimelineTask->frameCounter == 0) {
        BattleFx_KotrPrepareStageOffscreenRequest();
        BattleFx_KotrBuildRadialDiscMesh();
        (pTimelineTask->frameCounter)++;
    } else {
        frameIndex = pTimelineTask->frameCounter;
        if (frameIndex < 0x28) {
            if (frameIndex < 0xA) {
                eased = (frameIndex << 0xC) / 0xA;
                if (g_BattleIsPaused == 0) {
                    eased = BattleFx_ApplySineEasing(eased, 0x2FF);
                    colorA = (eased * -0x20 >> 0xC) + 0x80;
                    tintOuter = (eased * -0x60 >> 0xC) + 0x80;
                    BattleFx_KotrApplyDiscMeshDisplacement(eased * 0x3E8 >> 0xC, colorA, tintOuter);
                }
            } else {
                eased = (frameIndex - 0xA << 0xC) / 0x1E;
                if (g_BattleIsPaused == 0) {
                    eased = BattleFx_ApplySineEasing(eased, 0);
                    colorA = (eased * -0x60 >> 0xC) + 0x60;
                    tintOuter = (eased * -0x20 >> 0xC) + 0x20;
                    BattleFx_KotrApplyDiscMeshDisplacement((eased * 0x1F4 >> 0xC) + 0x3E8, colorA, tintOuter);
                }
            }
            SetRotMatrix(&g_BattleSummonKotrTransformState);
            SetTransMatrix(&g_BattleSummonKotrTransformState);
            Gfx_SetPsxPrimQuadState(0xF, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleSummonKotrOffscreenSlotIndex));
            RefreshGlobalTransformState();
            for (row = 0; row < 0x10; row++) {
                for (column = 0; column < 0x20; column++) {
                    cellPtr = &g_BattleSummonKotrDiscBuffer->rows[row].records[column];
                    g_BattleSummonKotrRadialDisc->depth = Gfx_TransformQuadReturnDepth((void *)cellPtr->vertexA, cellPtr->vertexB, cellPtr->vertexC, cellPtr->vertexD, &g_BattleSummonKotrRadialDisc->projectScratch, &g_BattleSummonKotrRadialDisc->flags);
                    if ((g_BattleSummonKotrRadialDisc->flags & 0x60000) == 0) {
                        cellPtr->quads[g_BattleSummonKotrBufferIndex].color1 = g_BattleSummonKotrDiscBuffer->colorWord[row];
                        cellPtr->quads[g_BattleSummonKotrBufferIndex].color0 = cellPtr->quads[g_BattleSummonKotrBufferIndex].color1;
                        cellPtr->quads[g_BattleSummonKotrBufferIndex].color3 = g_BattleSummonKotrDiscBuffer->colorWord[row + 1];
                        cellPtr->quads[g_BattleSummonKotrBufferIndex].color2 = cellPtr->quads[g_BattleSummonKotrBufferIndex].color3;
                        Gfx_DrawPsxPrimitive(&cellPtr->quads[g_BattleSummonKotrBufferIndex]);
                    }
                }
            }
            g_BattleSummonKotrBufferIndex ^= 1;
        } else {
            frameIndex -= 0x28;
            if (frameIndex < 0x1E) {
                packet = (BattleFxRadialDiscPacket *)Gfx_GetPsxPacketScratchPtr();
                g_BattleSummonKotrRadialDisc->edgeEnd.vy = 0;
                g_BattleSummonKotrRadialDisc->edgeStart.vy = 0;
                g_BattleSummonKotrRadialDisc->center.vz = 0;
                g_BattleSummonKotrRadialDisc->center.vy = 0;
                g_BattleSummonKotrRadialDisc->center.vx = 0;
                SetRotMatrix(&g_BattleSummonKotrTransformState);
                SetTransMatrix(&g_BattleSummonKotrTransformState);
                RefreshGlobalTransformState();
                for (row = 0; row < 0x20; row++) {
                    angleStart = (row << 0xC) / 0x20;
                    angleEnd = ((row + 1) << 0xC) / 0x20;
                    g_BattleSummonKotrRadialDisc->edgeStart.vx = SinFixed(angleStart) * 0x11D0 >> 0xC;
                    g_BattleSummonKotrRadialDisc->edgeStart.vz = CosFixed(angleStart) * 0x11D0 >> 0xC;
                    g_BattleSummonKotrRadialDisc->edgeEnd.vx = SinFixed(angleEnd) * 0x11D0 >> 0xC;
                    g_BattleSummonKotrRadialDisc->edgeEnd.vz = CosFixed(angleEnd) * 0x11D0 >> 0xC;
                    Gfx_TransformTriangleReturnDepthAlt(&g_BattleSummonKotrRadialDisc->center, &g_BattleSummonKotrRadialDisc->edgeStart, &g_BattleSummonKotrRadialDisc->edgeEnd, &g_BattleSummonKotrRadialDisc->projectScratch, &g_BattleSummonKotrRadialDisc->flags);
                    if ((g_BattleSummonKotrRadialDisc->flags & 0x60000) == 0) {
                        packet->tag = 0x4000000;
                        packet->command = 0x20000000;
                    }
                }
                g_BattleOverlayDrawListHead = (int)packet;
            } else {
                frameIndex -= 0xA;
                if (frameIndex < 0x1E) { /* empty first arm: lib/codegen.h */
                } else {
                    frameIndex -= 0x1E;
                    if (frameIndex < 0x1E) { /* empty first arm: lib/codegen.h */
                    } else {
                        pTimelineTask->ownerSlot = BATTLE_TASK_FREE;
                        return;
                    }
                }
            }
        }
        if (g_BattleIsPaused == 0) {
            (pTimelineTask->frameCounter)++;
        }
    }
}

/*
 * Copies global primitive coordinates and
 * selected matrix values into shared state, chooses the sign of g_BattleSummonKotrOffscreenCenter +
 * 0x1A from g_BattleSummonKotrEffectAngles[2], multiplies the shared matrices into local scratch
 * and forwards that matrix and g_BattleSummonKotrStageRequestArgs to
 * BattleFx_KotrQueueStageOffscreenRequest.
 */
// FUNCTION: FF7 0x004793d9
void __cdecl BattleFx_KotrPrepareStageOffscreenRequest(void)
{
    PsxMatrix w;

    (g_BattleSummonKotrStageRequestArgs.viewportOriginX) = *(short *)&g_BattleViewportOriginX;
    g_BattleSummonKotrStageRequestArgs.summonKotrOffscreenStageWord = *(short *)&g_BattleViewportOriginY;
    g_BattleSummonKotrOffscreenRequest.summonKotrOffscreenStateWord = g_BattleSummonKotrOffscreenCenter.offscreenStateWord;
    (g_BattleSummonKotrOffscreenRequest.m00) = g_BattleSummonKotrOffscreenRequest.summonKotrOffscreenStateWord;
    if (g_BattleSummonKotrEffectAngles[1] != 0) {
        g_BattleSummonKotrOffscreenRequest.summonKotrOffscreenNegatedOffset = g_BattleSummonKotrOffscreenCenter.centerZ;
    } else {
        g_BattleSummonKotrOffscreenRequest.summonKotrOffscreenNegatedOffset = -g_BattleSummonKotrOffscreenCenter.centerZ;
    }
    MultiplyMatrix((int *)&g_BattleSummonKotrOffscreenMatrix, (short *)&g_BattleSummonKotrOffscreenRequest, &w);
    BattleFx_KotrQueueStageOffscreenRequest(&w, (BattleFxWhirlsandStageRequestArgs *)&g_BattleSummonKotrStageRequestArgs);
}

/*
 * Installs the supplied matrix, and only when the stage pointer and its first pointer are nonnull
 * acquires an offscreen request. On successful acquisition sets its mesh, converted float matrix,
 * modes and four widened input shorts, overwriting +0xD0 from one to four, and saves request
 * +0x114 in g_BattleSummonKotrOffscreenSlotIndex.
 */
// FUNCTION: FF7 0x00479463
void __cdecl BattleFx_KotrQueueStageOffscreenRequest(void *matrix, BattleFxWhirlsandStageRequestArgs *inputArgs)
{
    GfxOffscreenRequest *pRequest;
    int stage;

    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    if (g_pBattle3dScene != 0 && g_pBattle3dScene->stageId != 0) {
        stage = g_pBattle3dScene->stageId;
        pRequest = (GfxOffscreenRequest *)Battle3d_AcquireOffscreenRequest();
        if (pRequest != 0) {
            pRequest->kind = 1;
            pRequest->mesh = (int)BattleStage_GetPartMesh(1, (Battle3dStageRecord *)stage);
            GtePsxMatrixToFloat4x4Transposed(matrix, (char *)pRequest + 0x88);
            pRequest->pageFillEnabled = 1;
            pRequest->renderKind = 0xE;
            pRequest->blendMode = BLEND_ADDITIVE;
            pRequest->blendMode = BLEND_NONE;
            pRequest->viewportX = inputArgs->argA;
            pRequest->viewportY = inputArgs->argB;
            pRequest->viewportW = inputArgs->argC;
            pRequest->viewportH = inputArgs->argD;
            pRequest->pageFillStepX = 1;
            pRequest->pageFillStepY = 1;
            g_BattleSummonKotrOffscreenSlotIndex = pRequest->slotIndex;
        }
    }
}

/*
 * Builds the sixteen rows of 32 disc cells in g_BattleSummonKotrDiscBuffer->rows. Each cell points
 * at its four corners in two adjacent rings of points, wrapping the segment index modulo 32, and
 * initializes both render banks' quads with tag 0x0C000000, texture page 0x14D and radial
 * sine/cosine UVs plus 0x60. Inner UV radius is ring times six; outer radius is the next radius
 * except the last clamps to 0x5F. Both banks use 0x3C808080 except the outermost ring uses
 * 0x3C202020. Then writes the seventeen flat point rings with Y zero, X radius times sine and Z
 * negative radius times cosine; radius is ring times six times 0x30 below sixteen and 0x11D0 for
 * the final ring. Sets the seventeen colorWord entries to 0x3C808080.
 */
// FUNCTION: FF7 0x0047958c
void __cdecl BattleFx_KotrBuildRadialDiscMesh(void)
{
    int r1;
    int idx;
    int ring;
    int inSegment;
    int a0;
    KotrRenderRecord *pq;
    int a1;
    int bankIndex;

    for (ring = 0; ring < 0x10; ring++) {
        idx = ring * 6;
        r1 = idx + 6 < 0x60 ? idx + 6 : 0x5F;
        for (inSegment = 0; inSegment < 0x20; inSegment++) {
            pq = &g_BattleSummonKotrDiscBuffer->rows[ring].records[inSegment];
            pq->vertexA = &g_BattleSummonKotrDiscBuffer->points[ring][inSegment];
            pq->vertexB = &g_BattleSummonKotrDiscBuffer->points[ring][(inSegment + 1) % 0x20];
            pq->vertexC = &g_BattleSummonKotrDiscBuffer->points[ring + 1][inSegment];
            pq->vertexD = &g_BattleSummonKotrDiscBuffer->points[ring + 1][(inSegment + 1) % 0x20];
            a0 = (inSegment << 12) / 0x20;
            a1 = ((inSegment + 1) << 12) / 0x20;
            for (bankIndex = 0; bankIndex < 2; bankIndex++) {
                pq->quads[bankIndex].tag = 0xC000000;
                pq->quads[bankIndex].u0 = (idx * SinFixed(a0) >> 12) + 0x60;
                pq->quads[bankIndex].v0 = (idx * CosFixed(a0) >> 12) + 0x60;
                pq->quads[bankIndex].u1 = (idx * SinFixed(a1) >> 12) + 0x60;
                pq->quads[bankIndex].v1 = (idx * CosFixed(a1) >> 12) + 0x60;
                pq->quads[bankIndex].u2 = (r1 * SinFixed(a0) >> 12) + 0x60;
                pq->quads[bankIndex].v2 = (r1 * CosFixed(a0) >> 12) + 0x60;
                pq->quads[bankIndex].u3 = (r1 * SinFixed(a1) >> 12) + 0x60;
                pq->quads[bankIndex].v3 = (r1 * CosFixed(a1) >> 12) + 0x60;
                pq->quads[bankIndex].tpage = 0x14D;
                if (ring < 0xF) {
                    pq->quads[bankIndex].color0 = 0x3C808080;
                    pq->quads[bankIndex].color1 = 0x3C808080;
                    pq->quads[bankIndex].color2 = 0x3C808080;
                    pq->quads[bankIndex].color3 = 0x3C808080;
                } else {
                    pq->quads[bankIndex].color0 = 0x3C202020;
                    pq->quads[bankIndex].color1 = 0x3C202020;
                    pq->quads[bankIndex].color2 = 0x3C202020;
                    pq->quads[bankIndex].color3 = 0x3C202020;
                }
            }
        }
    }
    for (ring = 0; ring < 0x11; ring++) {
        idx = ring < 0x10 ? ring * 6 * 0x30 : 0x11D0;
        for (inSegment = 0; inSegment < 0x20; inSegment++) {
            a0 = (inSegment << 12) / 0x20;
            g_BattleSummonKotrDiscBuffer->points[ring][inSegment].vx = idx * SinFixed(a0) >> 12;
            g_BattleSummonKotrDiscBuffer->points[ring][inSegment].vy = 0;
            g_BattleSummonKotrDiscBuffer->points[ring][inSegment].vz = -idx * CosFixed(a0) >> 12;
        }
        g_BattleSummonKotrDiscBuffer->colorWord[ring] = 0x3C808080;
    }
}

/*
 * Fills a 16-row table at g_BattleSummonKotrDiscBuffer: for each row i it takes a sine-eased ramp k =
 * BattleFx_ApplySineEasing(i*0x1000/15, -0x3FF), writes amplitude*(0x1000-k)>>12 into vy of each of the
 * ring's 32 points, and stores a packed colour word v|0x3C000000|(v<<8)|(v<<16) in colorWord[i],
 * where v interpolates shadeFrom..shadeTo by
 * (amplitude*0x1000/0x9C4 * k)>>12.
 */
// FUNCTION: FF7 0x004799ca
void __cdecl BattleFx_KotrApplyDiscMeshDisplacement(int amplitude, int shadeFrom, int shadeTo)
{
    int level;
    int scaleFactor;
    int row;
    int ent;
    int ramp;

    scaleFactor = (amplitude << 0xC) / 0x9C4;
    for (row = 0; row < 0x10; row++) {
        ramp = (row << 0xC) / 0xF;
        ramp = BattleFx_ApplySineEasing(ramp, -0x3FF);
        for (ent = 0; ent < 0x20; ent++) {
            g_BattleSummonKotrDiscBuffer->points[row][ent].vy = amplitude * (0x1000 - ramp) >> 0xC;
        }
        level = shadeFrom + ((shadeTo - shadeFrom) * (scaleFactor * ramp >> 0xC) >> 0xC);
        g_BattleSummonKotrDiscBuffer->colorWord[row] = level | 0x3C000000 | (level << 8) | (level << 0x10);
    }
}

/*
 * Two-phase spinning-model effect on rec =
 * &g_BattleHitLivewireEmitterRecords + (g_BattleActiveTaskSlotIdx << 5) using the scratch block at
 * g_BattleKotrSpinModelState (claimed from Gfx_GetPsxScratchpadPtr(0) on first use) and the draw
 * request at g_BattleSummonKotrModelDrawRequest: each frame it rotates the model by
 * frame*0x20 about Y into the block's spin matrix, concatenates g_BattleSummonKotrTransformState, then
 * for the first 0x1E frames scales by a 0..2.0 ramp in g_BattleSummonKotrScaleMatrix and submits model entry 0xF of
 * g_BattleSummonKotrTableCursor into the g_BattleCurrentOrderingTable+0x70 bucket at shift 0xC;
 * for the next 0x1C frames (fading via 0x7F571A after frame 0x12) it stores (0x3E80 - frame*0x1F4 %
 * 0x3E80) as a float at g_BattleKotrTintValue and submits the model three times, each
 * with the tint green channel from that float's raw bits, alternating scalar override (2,
 * 0x3F7B0000) and (1, 0) and subtracting 8000.0 in between, into an uninitialised local bucket
 * pointer x at shift 0; ends the record with 0xFFFF afterwards, and increments the frame counter
 * unless g_BattleIsPaused is set.
 */
// FUNCTION: FF7 0x00479ab2
void __cdecl BattleFx_KotrSpinningModelEntry15Task(void)
{
    int frameIndex;
    int passIndex;
    int otBucket;
    BattleTaskSlot *rec;
    int rampFactor;
    int l;
    int m;
    int odd;

    rec = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrSpinningModelEntry15TaskInit & 1) == 0) {
        g_BattleKotrSpinningModelEntry15TaskInit |= 1;
        g_BattleKotrSpinModelState = (BattleKotrSpinModelPose *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonKotrModelDrawRequest.modelGroup = 0;
    SetDrawColorRGB(0, 0, 0);
    frameIndex = rec->frameCounter;
    if (frameIndex < 0x1E) {
        g_BattleKotrSpinModelState->rotation.vz = 0;
        g_BattleKotrSpinModelState->rotation.vx = 0;
        g_BattleKotrSpinModelState->rotation.vy = rec->frameCounter << 5;
        RotMatrix(&g_BattleKotrSpinModelState->rotation, &g_BattleKotrSpinModelState->spin);
        g_BattleKotrSpinModelState->spin.t[2] = 0;
        g_BattleKotrSpinModelState->spin.t[1] = 0;
        g_BattleKotrSpinModelState->spin.t[0] = 0;
        MultiplyMatrix(&g_BattleSummonKotrTransformState, &g_BattleKotrSpinModelState->spin, &g_BattleKotrSpinModelState->spin);
        rampFactor = (frameIndex << 12) / 0x1E;
        g_BattleSummonKotrScaleMatrix[4] = rampFactor * 0x2000 >> 12;
        MultiplyMatrix(&g_BattleKotrSpinModelState->spin, g_BattleSummonKotrScaleMatrix, &g_BattleKotrSpinModelState->scaled);
        SetRotMatrix(&g_BattleKotrSpinModelState->scaled);
        SetTransMatrix(&g_BattleKotrSpinModelState->scaled);
        g_BattleSummonKotrModelDrawRequest.fadeLevel = 0;
        g_BattleSummonKotrModelDrawRequest.modelGroup = Gfx_ModelTableGetRenderObject(0xF, g_BattleSummonKotrTableCursor);
        g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonKotrModelDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    } else {
        frameIndex -= 0x1E;
        if (frameIndex < 0x1C) {
            g_BattleKotrSpinModelState->rotation.vz = 0;
            g_BattleKotrSpinModelState->rotation.vx = 0;
            g_BattleKotrSpinModelState->rotation.vy = rec->frameCounter << 5;
            RotMatrix(&g_BattleKotrSpinModelState->rotation, &g_BattleKotrSpinModelState->spin);
            g_BattleKotrSpinModelState->spin.t[2] = 0;
            g_BattleKotrSpinModelState->spin.t[1] = 0;
            g_BattleKotrSpinModelState->spin.t[0] = 0;
            MultiplyMatrix(&g_BattleSummonKotrTransformState, &g_BattleKotrSpinModelState->spin, &g_BattleKotrSpinModelState->spin);
            if (frameIndex < 0x12) { /* empty first arm: lib/codegen.h */
            } else {
                frameIndex -= 0x12;
                rampFactor = (frameIndex << 12) / 0xA;
                g_BattleSummonKotrModelDrawRequest.fadeLevel = rampFactor;
                SetDrawColorRGB(0, 0, 0);
            }
            g_BattleKotrTintValue = (float)(0x3E80 - rec->frameCounter * 0x1F4 % 0x3E80);
            if (*(float *)((char *)&g_BattleKotrUpdateFragmentSpinShrinkMatricesInit + 4) < 8000.0f) {
                odd = 0;
            } else {
                g_BattleKotrTintValue -= 8000.0f;
                odd = 1;
            }
            g_BattleSummonKotrScaleMatrix[4] = 0x1000;
            MultiplyMatrix(&g_BattleKotrSpinModelState->spin, g_BattleSummonKotrScaleMatrix, &g_BattleKotrSpinModelState->scaled);
            SetRotMatrix(&g_BattleKotrSpinModelState->scaled);
            SetTransMatrix(&g_BattleKotrSpinModelState->scaled);
            for (passIndex = 0; passIndex < 3; passIndex++) {
                g_BattleSummonKotrModelDrawRequest.modelGroup = Gfx_ModelTableGetRenderObject(0xF, g_BattleSummonKotrTableCursor);
                BattleFx_LatchDrawTintRgb(0, *(int *)&g_BattleKotrTintValue, 0);
                if (odd != 0) {
                    BattleFx_LatchDrawScalarOverride(2, 0x3F7B0000);
                } else {
                    BattleFx_LatchDrawScalarOverride(1, 0);
                }
                g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonKotrModelDrawRequest, (int)otBucket, 0, g_BattleOverlayDrawListHead);
                g_BattleKotrTintValue -= 8000.0f;
                odd ^= 1;
            }
        } else {
            rec->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    if (g_BattleIsPaused == 0) {
        (rec->frameCounter)++;
    }
}

/*
 * Returns while paused. Uses
 * emission term 0x700 below count ten and a linear fixed-point fall from that value over counts ten
 * through 39; later retires and returns. Adds the term to g_BattleSummonKotrQuadEmissionTotal, but
 * emits using the local term alone, subtracting 0x100 per record and discarding its remainder.
 * Allocates BattleFx_KotrFadingOffscreenQuadParticleTask records with first short one or two,
 * random half-circle angle and sine-eased random radius scaled by 0x1200. Sets X/Z from sine/cosine
 * and Y from BattleFx_KotrGetDiscHeightAtRadius(radius), horizontal velocities from narrowed
 * position shifted left six then right twelve, and Y velocity negative random 200 through 299.
 * Writes two final bytes from the parent X/Z shorts mapped with plus 0x1200 times 0xBD divided by
 * 0x2400, then advances count.
 */
// FUNCTION: FF7 0x00479eaa
void __cdecl BattleFx_KotrEmitFadingOffscreenQuadParticlesTask(void)
{
    int angle;
    int rad;
    int relCount;
    BattleFxKotrQuadEmitter *pEmitter;
    int ratio;
    int speedY;
    BattleFxKotrQuadParticle *spawned;
    int emission;

    pEmitter = (BattleFxKotrQuadEmitter *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    relCount = pEmitter->index;
    if (relCount < 0xA) {
        emission = 0x700;
    } else {
        relCount -= 0xA;
        if (relCount < 0x1E) {
            ratio = (relCount << 0xC) / 0x1E;
            emission = (0x1000 - ratio) * 0x700 >> 0xC;
        } else {
            pEmitter->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    g_BattleSummonKotrQuadEmissionTotal += emission;
    while (emission >= 0x100) {
        spawned = (BattleFxKotrQuadParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrFadingOffscreenQuadParticleTask);
        spawned->ownerSlot = (rand() & 1) + 1;
        angle = rand() & 0x7FF;
        rad = rand() & 0xFFF;
        rad = (BattleFx_ApplySineEasing(rad, 0x1FF) << 0xB >> 0xC) + 0x800;
        rad = rad * 0x1200 >> 0xC;
        spawned->posX = SinFixed(angle) * rad >> 0xC;
        spawned->height = BattleFx_KotrGetDiscHeightAtRadius(rad);
        spawned->posZ = CosFixed(angle) * rad >> 0xC;
        spawned->velocityX = spawned->posX << 6 >> 0xC;
        if (DEAD_IN_RETAIL != 0) {
            speedY = 0x12C;
        } else {
            speedY = rand() % 0x64 + 0xC8;
        }
        spawned->velocityY = -speedY;
        spawned->velocityZ = spawned->posZ << 6 >> 0xC;
        spawned->texU = (pEmitter->posX + 0x1200) * 0xBD / 0x2400;
        spawned->texV = (pEmitter->posY + 0x1200) * 0xBD / 0x2400;
        emission -= 0x100;
    }
    (pEmitter->index)++;
}

/*
 * Divides radius by 0x30, then when the resulting index/6 is below 16 linearly interpolates between
 * neighboring g_BattleSummonKotrDiscBuffer row words at +2 using index%6 in 12-bit fixed point; otherwise returns
 * zero. No lower-bound check is present.
 */
// FUNCTION: FF7 0x0047a0c6
int __cdecl BattleFx_KotrGetDiscHeightAtRadius(int radius)
{
    int fraction;
    int lowHeight;
    int highHeight;

    radius = radius / 0x30;
    if (radius / 6 < 0x10) {
        lowHeight = g_BattleSummonKotrDiscBuffer->points[radius / 6][0].vy;
        highHeight = g_BattleSummonKotrDiscBuffer->points[radius / 6 + 1][0].vy;
        fraction = (radius % 6 << 0xC) / 6;
        return lowHeight + ((highHeight - lowHeight) * fraction >> 0xC);
    } else {
        return 0;
    }
}

/*
 * Acquires retained scratch once and
 * render storage each call. Uses equal RGB 128 below count five and linearly fades it over five
 * through nine; later retires before projection/pause. Installs g_BattleSummonKotrTransformState
 * and projects task position. For flags lacking 0x60000 builds a quad extending right/down by the
 * signed first task short, maps UV bytes from task 0x1E/0x1F with the same increment and selects
 * texture 0x12D and command 0x3C. Reinterprets the last projected NDC Z/W floats as integer bits
 * for all four blend-factor pairs, selects the offscreen render object and submits. While unpaused
 * advances count and XYZ by velocity, then damps all velocity components by 0xD00 shifted by
 * twelve.
 */
// FUNCTION: FF7 0x0047a15a
void __cdecl BattleFx_KotrFadingOffscreenQuadParticleTask(void)
{
    BattleFxOffscreenParticle *pQuadParticle;
    int rawRelTimer;
    int ofs;
    int fadeShade;
    YasuiKotrFadeQuad *packetPtr;
    int fadeFraction;
    float ndcW;
    float ndcZ;

    pQuadParticle = (BattleFxOffscreenParticle *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrFadingOffscreenQuadParticleTaskInit & 1) == 0) {
        g_BattleKotrFadingOffscreenQuadParticleTaskInit |= 1;
        g_BattleKotrFadeParticle = (struct BattleFxKotrFadeParticle *)(Gfx_GetPsxScratchpadPtr(0));
    }
    packetPtr = (YasuiKotrFadeQuad *)Gfx_GetPsxPacketScratchPtr();
    rawRelTimer = pQuadParticle->timer;
    if (rawRelTimer < 5) {
        packetPtr->color0 = 0x2C808080;
        packetPtr->color1 = 0x808080;
        packetPtr->color2 = 0x808080;
        packetPtr->color3 = 0x808080;
    } else {
        rawRelTimer -= 5;
        if (rawRelTimer < 5) {
            fadeFraction = (rawRelTimer << 0xC) / 5;
            fadeShade = 0x1000 - fadeFraction << 7 >> 0xC;
            packetPtr->color0 = fadeShade | 0x2C000000 | fadeShade << 8 | fadeShade << 0x10;
            packetPtr->color1 = fadeShade | fadeShade << 8 | fadeShade << 0x10;
            packetPtr->color2 = fadeShade | fadeShade << 8 | fadeShade << 0x10;
            packetPtr->color3 = fadeShade | fadeShade << 8 | fadeShade << 0x10;
        } else {
            pQuadParticle->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    SetRotMatrix(&g_BattleSummonKotrTransformState);
    SetTransMatrix(&g_BattleSummonKotrTransformState);
    g_BattleKotrFadeParticle->depth = ProjectPointHardware((char *)pQuadParticle + 4, (short *)((int)packetPtr + 8), (int *)(((int)g_BattleKotrFadeParticle) + 4), (int *)(((int)g_BattleKotrFadeParticle) + 8));
    if ((g_BattleKotrFadeParticle->flags & 0x60000) == 0) {
        ofs = pQuadParticle->ownerSlot;
        packetPtr->tag = 0x9000000;
        packetPtr->baseOffset1 = packetPtr->initOffset1;
        packetPtr->offset1 = packetPtr->baseOffset1 + ofs;
        packetPtr->offset1Copy = packetPtr->offset1;
        packetPtr->baseOffset0 = packetPtr->initOffset0;
        packetPtr->offset0 = packetPtr->baseOffset0 + ofs;
        packetPtr->offset0Copy = packetPtr->offset0;
        packetPtr->fadeSource0 = pQuadParticle->fadeSource0;
        packetPtr->fadeStart0 = packetPtr->fadeSource0;
        packetPtr->fadeValue0 = *(unsigned char *)&packetPtr->fadeStart0 + ofs;
        packetPtr->fadeValue0Copy = packetPtr->fadeValue0;
        packetPtr->fadeSource1 = pQuadParticle->fadeSource1;
        packetPtr->fadeStart1 = packetPtr->fadeSource1;
        packetPtr->fadeValue1 = *(unsigned char *)&packetPtr->fadeStart1 + ofs;
        packetPtr->fadeValue1Copy = packetPtr->fadeValue1;
        packetPtr->tpage = 0x12D;
        *(char *)((char *)packetPtr + 7) = 0x3C;
        ndcZ = GetLastProjectedNdcZ();
        ndcW = GetLastProjectedNdcW();
        Gfx_SetBlendFactorQuad(ndcZ, ndcW, ndcZ, ndcW, ndcZ, ndcW, ndcZ, ndcW);
        Gfx_SetPsxPrimQuadState(0xF, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleSummonKotrOffscreenSlotIndex));
        Gfx_DrawPsxPrimitive(packetPtr);
    }
    if (g_BattleIsPaused == 0) {
        (pQuadParticle->timer)++;
        pQuadParticle->posX += pQuadParticle->velocityX;
        pQuadParticle->posY += pQuadParticle->velocityY;
        pQuadParticle->posZ += pQuadParticle->velocityZ;
        pQuadParticle->velocityX = pQuadParticle->velocityX * 0xD00 >> 0xC;
        pQuadParticle->velocityY = pQuadParticle->velocityY * 0xD00 >> 0xC;
        pQuadParticle->velocityZ = pQuadParticle->velocityZ * 0xD00 >> 0xC;
    }
}

/*
 * Allocates a slot for BattleFx_KotrActorLiftTask, stores actor in its first word and rand()&0xfff at +0xC, and
 * records its address in the indexed g_BattleSummonKotrActorLiftTasks entry.
 */
// FUNCTION: FF7 0x0047a4e2
void __cdecl BattleFx_KotrSpawnActorLiftPerTarget(int actor)
{
    BattleFxActorLiftTask *taskPtr;

    taskPtr = (BattleFxActorLiftTask *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrActorLiftTask);
    taskPtr->targetIndex = actor;
    taskPtr->randomValue = rand() & 0xFFF;
    ((int *)&g_BattleSummonKotrActorLiftTasks)[actor] = (int)taskPtr;
}

/*
 * Acquires retained scratch before the pause return and selects the actor from task first short.
 * Unpaused refreshes draw resource from g_pBattle3dScene. At count zero saves eight position bytes
 * and status byte, sets status bit 0x10, obtains the actor attachment matrix and transforms it to
 * camera local coordinates to save a Y difference. For duplicate-mask actors reads phase/base Y
 * from the mapped task; otherwise uses this task. First 39 counts set actor Y to saved Y plus
 * BattleFx_KotrGetDiscHeightAtRadius of X/Z distance from retained centre divided by 0x30. If saved
 * status lacked 0x10, constructs and submits a flattened shadow matrix using actor dimensions,
 * saved Y, attachment sentinel and actor matrix. Next thirty counts add accelerating Y velocity
 * starting at zero and increasing by fifty, clamping Y above 0x61A8. Next 0x3BB counts set Y to
 * selected base plus sine of selected phase plus elapsed shifted seven times fifty shifted twelve.
 * Next 75 counts set base Y and status bit four only on entry. Thereafter restores the saved eight
 * position bytes and saved status bits 0x14 while preserving other current bits, retires and
 * returns; ordinary calls advance count.
 */
// FUNCTION: FF7 0x0047a52d
void __cdecl BattleFx_KotrActorLiftTask(void)
{
    int relCount;
    BattleFxKotrLiftState *task;
    int unusedEntry; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int distZ;
    int baseY;
    BattleActorLiftRecord *actor;
    PsxMatrix actorMtx;
    int ringRadius;
    int deltaX;
    int bobPhase;
    int padHole[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    task = (BattleFxKotrLiftState *)BATTLE_CURRENT_TASK_SLOT;
    actor = (BattleActorLiftRecord *)&g_BattleActorRecords[task->actorIndex].modelId;
    if ((g_BattleKotrActorLiftTaskInit & 1) == 0) {
        g_BattleKotrActorLiftTaskInit |= 1;
        g_BattleKotrLiftState = (BattleSummonKotrLift *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (g_BattleSummonKotrKnightEndpointModel.modelRequest) = 0;
    if (g_pBattle3dScene != 0) {
        (g_BattleSummonKotrKnightEndpointModel.modelRequest) = g_pBattle3dScene->modelRequest;
    }
    if (task->frameCount == 0) {
        *(Int64Pair *)((char *)task + 4) = *(Int64Pair *)&actor->posX;
        task->savedFlags = *(unsigned char *)&actor->flags;
        actor->flags |= 0x10;
        Battle3d_GetActorAttachMatrix(task->actorIndex, actor->attachPoint, &actorMtx);
        Battle_WorldPointToCameraLocalDup((struct BattleCameraWorldPointScratch *)&actorMtx, (BattleCameraLocalPoint2 *)g_BattleKotrLiftState);
        task->relativeHeight = task->liftHeight - g_BattleKotrLiftState->liftOffset;
    }
    if ((g_BattleKotrHandledMask & 1 << task->actorIndex) == 0) {
        bobPhase = task->bobPhase;
        baseY = task->relativeHeight;
    } else {
        bobPhase = ((BattleFxKotrActorLift *)(((int *)&g_BattleSummonKotrActorLiftTasks)[g_BattleSummonKotrActorRepresentative[task->actorIndex]]))->bobPhase;
        baseY = ((BattleFxKotrActorLift *)(((int *)&g_BattleSummonKotrActorLiftTasks)[g_BattleSummonKotrActorRepresentative[task->actorIndex]]))->relativeHeight;
    }
    relCount = task->frameCount;
    if (relCount < 0x27) {
        deltaX = actor->posX - g_BattleSummonKotrOffscreenCenter.centerX;
        distZ = actor->posZ - g_BattleSummonKotrOffscreenCenter.centerZ;
        ringRadius = SqrtToInt(deltaX * deltaX + distZ * distZ) / 0x30;
        actor->height = task->liftHeight + BattleFx_KotrGetDiscHeightAtRadius(ringRadius);
        if ((task->savedFlags & 0x10) == 0) {
            g_BattleKotrLiftState->angleZ = 0;
            g_BattleKotrLiftState->liftOffset = 0;
            g_BattleKotrLiftState->angleX = 0;
            RotMatrix((SVECTOR *)g_BattleKotrLiftState, &g_BattleKotrLiftState->matrix);
            if (actor->boneIndex == -1) {
                g_BattleKotrLiftState->matrix.t[1] = -task->liftHeight;
                g_BattleKotrLiftState->matrix.t[0] = g_BattleKotrLiftState->matrix.t[1];
                g_BattleKotrLiftState->matrix.t[2] = 0;
            } else {
                g_BattleKotrLiftState->matrix.t[0] = actor->bone0PosX;
                g_BattleKotrLiftState->matrix.t[1] = -task->liftHeight;
                g_BattleKotrLiftState->matrix.t[2] = actor->bone0PosZ + actor->boneIndex;
            }
            g_BattleKotrLiftState->scaleX = actor->scaleX;
            g_BattleKotrLiftState->scaleY = 0;
            g_BattleKotrLiftState->scaleZ = actor->scaleY;
            ScaleMatrix(&g_BattleKotrLiftState->matrix, &g_BattleKotrLiftState->scaleX);
            MultiplyMatrix((char *)actor + 0x140, &g_BattleKotrLiftState->matrix, &g_BattleKotrLiftState->matrix);
            SetRotMatrix(&g_BattleKotrLiftState->matrix);
            SetTransMatrix(&g_BattleKotrLiftState->matrix);
            g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonKotrKnightEndpointModel, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST, 0, g_BattleOverlayDrawListHead);
        }
    } else {
        relCount -= 0x27;
        if (relCount < 0x1E) {
            if (relCount == 0) {
                task->liftVelocity = 0;
            }
            actor->height += task->liftVelocity;
            if (actor->height > 0x61A8) {
                actor->height = 0x61A8;
            }
            task->liftVelocity += 0x32;
        } else {
            relCount -= 0x1E;
            if (relCount < 0x3BB) {
                actor->height = baseY + (SinFixed(bobPhase + (relCount << 7)) * 0x32 >> 0xC);
            } else {
                relCount -= 0x3BB;
                if (relCount < 0x4B) {
                    if (relCount == 0) {
                        actor->height = baseY;
                        actor->flags |= 4;
                    }
                } else {
                    *(Int64Pair *)&actor->posX = *(Int64Pair *)((char *)task + 4);
                    actor->flags = *(unsigned char *)&actor->flags & 0xFFFFFFEB | task->savedFlags & 0x14;
                    task->actorIndex = 0xFFFF;
                    return;
                }
            }
        }
    }
    (task->frameCount)++;
}

/*
 * Allocates the supplied callback and initializes its knight
 * context through BattleFx_KotrInitKnightContextHookNoOp(0, context), retaining an intervening
 * Battle_TaskNop. Clears the listed rotation, offset, animation/count fields, stores zero at 0x7C
 * and knight index minus one at 0x80, sets short 0x4E to 4096 and loads short 0x58 and pointer 0x54
 * from index-based tables. Stores the context pointer in task 0x14. Transforms the two indexed
 * eight-byte points (start point and endpoint) in g_BattleSummonKotrKnightLookAtPoints through
 * g_BattleSummonKotrOffscreenCenter, adds retained centre XYZ with short narrowing, builds their
 * look-at matrix using g_BattleOverlayVertexOffset and multiplies it with
 * g_BattleSummonKotrOffscreenCenter into context 0x5C. Increments a local index with no subsequent
 * use.
 */
// FUNCTION: FF7 0x0047a9e7
void __cdecl BattleFx_KotrSpawnKnight(BattleKotrKnight *knightContext, int callback, int knightIndex)
{
    struct {
        int sourcePoint;
        SVECTOR pointA;
        BattleFxKotrKnight1Timeline *task;
        SVECTOR pointB;
        PsxMatrix lookAt;
        unsigned char pad_36[2];
        int knightIdx;
        int endpointSource;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.task = (BattleFxKotrKnight1Timeline *)&g_BattleTaskSlotBuffer.mainSlots[(short)Battle_AllocateResourceSlot((BattleTaskFn)(callback))];
    Battle_TaskNop();
    BattleFx_KotrInitKnightContextHookNoOp(0, (int)knightContext);
    knightContext->animFrame = 0;
    knightContext->knightIndex = knightIndex - 1;
    frame.knightIdx = knightIndex - 1;
    knightContext->clearedWord = 0;
    knightContext->offsetZ = 0;
    knightContext->offsetY = 0;
    knightContext->offsetX = 0;
    knightContext->state = 0;
    knightContext->rotZ = 0;
    knightContext->rotY = 0;
    knightContext->rotX = 0;
    knightContext->scale = 0x1000;
    knightContext->attachIndex = g_BattleKotrKnightAttachIndices[frame.knightIdx];
    knightContext->fadeLevel = 0;
    knightContext->tintColor = g_BattleKotrKnightTints[frame.knightIdx];
    knightContext->flags = 0;
    frame.task->knight = knightContext;
    frame.sourcePoint = (frame.knightIdx << 1) * 8 + (int)&g_BattleSummonKotrKnightLookAtPoints;
    TransformVectorToInt16((void *)&g_BattleSummonKotrOffscreenCenter, (void *)(frame.sourcePoint), &frame.pointA);
    frame.pointA.vx += (short)g_BattleSummonKotrOffscreenCenter.centerX;
    frame.pointA.vy += g_BattleSummonKotrOffscreenCenter.stepDelta;
    frame.pointA.vz += (short)g_BattleSummonKotrOffscreenCenter.centerZ;
    frame.endpointSource = (frame.knightIdx << 1) * 8 + (int)&g_BattleSummonKotrKnightLookAtPoints.summonKotrEndpointPointList;
    TransformVectorToInt16((void *)&g_BattleSummonKotrOffscreenCenter, (void *)(frame.endpointSource), &frame.pointB);
    frame.pointB.vx += (short)g_BattleSummonKotrOffscreenCenter.centerX;
    frame.pointB.vy += g_BattleSummonKotrOffscreenCenter.stepDelta;
    frame.pointB.vz += (short)g_BattleSummonKotrOffscreenCenter.centerZ;
    Gte_BuildLookAtMatrix(&frame.lookAt, &frame.pointB, &frame.pointA, &g_BattleOverlayVertexOffset);
    MultiplyMatrix(&frame.lookAt, (int *)&g_BattleSummonKotrOffscreenCenter, (char *)knightContext + 0x5C);
    (frame.knightIdx)++;
}

/*
 * Empty hook; performs no work.
 */
// FUNCTION: FF7 0x0047abab
void __cdecl BattleFx_KotrInitKnightContextHookNoOp(int reserved, int knightContext)
{
}

/*
 * Knights-of-the-Round camera step on the record rec
 * = &g_BattleHitLivewireEmitterRecords + (g_BattleActiveTaskSlotIdx << 5) with the knight k =
 * rec->knight: on first use it claims the scratch block at g_BattleSummonKotrKnight1TimelineState
 * from Gfx_GetPsxScratchpadPtr(0); every frame it calls BattleFx_KotrDrawKnightModel(k) (or
 * BattleFx_KotrSpawnKnightEndpointTrail(k, 0x56, ...) on frame 0); unless g_BattleIsPaused is
 * set it posts the impact records on frame 0x32, then runs three timed phases (0x1E, 0x1A, 0x1E
 * frames) that ease the orbit angle g_BattleSummonKotrKnight1Angle and radius
 * g_BattleKotrKnight1Radius with BattleFx_ApplySineEasing and set k's scale, fadeLevel, rotX and rotZ and the
 * block's scaleY, ending the record with 0xFFFF after the third; the common tail builds a
 * rotation from the fixed angles 0xFD00/0xFD80/0xFF00, scales it, transforms the polar offset
 * (cos/sin of the angle times the radius) into k's position, adds 0/-0x1F4/0x2710 to it, calls
 * BattleFx_KotrBumpKnightContextCounter(k) and increments the frame counter.
 */
// FUNCTION: FF7 0x0047abb0
void __cdecl BattleFx_KotrKnight1TimelineTask(void)
{
    int frameIndex;
    int easedValue;
    int ang;
    BattleFxKotrKnight1Timeline *rec;
    BattleKotrKnight *pKnight;

    rec = (BattleFxKotrKnight1Timeline *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrKnight1TimelineTaskInit & 1) == 0) {
        g_BattleKotrKnight1TimelineTaskInit |= 1;
        g_BattleSummonKotrKnight1TimelineState = (BattleKotrModelPose *)Gfx_GetPsxScratchpadPtr(0);
    }
    pKnight = rec->knight;
    if (rec->frame != 0) {
        BattleFx_KotrDrawKnightModel(pKnight);
    } else {
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x56, (char *)&g_BattleSummonKotrKnight1SpinScaleModel + 0x20);
    }
    if (g_BattleIsPaused == 0) {
        if (rec->frame == 0x32) {
            BattleFx_KotrSpawnKnightActorBurstSprites(pKnight);
            Battle_AllocateResourceSlot(BattleFx_KotrKnight1SpinScaleModelSetupTask);
            Battle_AllocateResourceSlot(BattleFx_KotrImpactScreenFlashTask);
            BattleFx_KotrPostKnightImpactRecords();
        }
        frameIndex = rec->frame;
        if (frameIndex < 0x1E) {
            if (frameIndex == 0) {
                pKnight->state = 1;
                pKnight->offsetX = 0;
                pKnight->offsetY = -0x1F4;
                pKnight->offsetZ = 0x2710;
            }
            ang = (frameIndex << 12) / 0x1E;
            easedValue = BattleFx_ApplySineEasing(ang, -0x3D0);
            g_BattleKotrKnight1Radius = (easedValue * 0 >> 12) + 0xBB8;
            g_BattleSummonKotrKnight1Angle = easedValue * 0x800 >> 12;
            pKnight->scale = easedValue * 0x1000 >> 12;
            pKnight->fadeLevel = 0x1000 - easedValue;
            g_BattleSummonKotrKnight1TimelineState->scaleY = 0x1000;
            pKnight->rotZ = (easedValue * -0x400 >> 12) + 0x400;
            pKnight->rotX = pKnight->rotZ;
        } else {
            frameIndex -= 0x1E;
            if (frameIndex < 0x1A) {
                if (frameIndex == 0) {
                    pKnight->scale = 0x1000;
                    pKnight->fadeLevel = 0;
                    pKnight->rotZ = 0;
                    pKnight->rotX = 0;
                }
                g_BattleSummonKotrKnight1Angle = 0x800;
                g_BattleKotrKnight1Radius = 0xBB8;
                g_BattleSummonKotrKnight1TimelineState->scaleY = 0x1000;
            } else {
                frameIndex -= 0x1A;
                if (frameIndex < 0x1E) {
                    if (frameIndex == 0) {
                        pKnight->state = 2;
                        pKnight->offsetX = 0 - pKnight->posX;
                        pKnight->offsetY = -0x1F4 - pKnight->posY;
                        pKnight->offsetZ = 0x2710 - pKnight->posZ;
                    }
                    ang = (frameIndex << 12) / 0x1E;
                    easedValue = BattleFx_ApplySineEasing(ang, -0x1E0);
                    g_BattleSummonKotrKnight1Angle = (easedValue * 0x800 >> 12) + 0x800;
                    g_BattleKotrKnight1Radius = (easedValue * 0x1B58 >> 12) + 0xBB8;
                    pKnight->scale = (easedValue * -0x1000 >> 12) + 0x1000;
                    pKnight->fadeLevel = easedValue;
                    g_BattleSummonKotrKnight1TimelineState->scaleY = 0x400;
                } else {
                    rec->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
        g_BattleSummonKotrKnight1TimelineState->rotation.vx = 0xFD00;
        g_BattleSummonKotrKnight1TimelineState->rotation.vy = 0xFD80;
        g_BattleSummonKotrKnight1TimelineState->rotation.vz = 0xFF00;
        RotMatrix(&g_BattleSummonKotrKnight1TimelineState->rotation, &g_BattleSummonKotrKnight1TimelineState->matrix);
        g_BattleSummonKotrKnight1TimelineState->scaleZ = 0x1000;
        g_BattleSummonKotrKnight1TimelineState->scaleX = 0x1000;
        ScaleMatrix(&g_BattleSummonKotrKnight1TimelineState->matrix, &g_BattleSummonKotrKnight1TimelineState->scaleX);
        g_BattleSummonKotrKnight1TimelineState->rotation.vx = CosFixed(g_BattleSummonKotrKnight1Angle) * g_BattleKotrKnight1Radius >> 12;
        g_BattleSummonKotrKnight1TimelineState->rotation.vy = SinFixed(g_BattleSummonKotrKnight1Angle) * g_BattleKotrKnight1Radius >> 12;
        g_BattleSummonKotrKnight1TimelineState->rotation.vz = 0;
        TransformVectorToInt16(&g_BattleSummonKotrKnight1TimelineState->matrix, &g_BattleSummonKotrKnight1TimelineState->rotation, (char *)pKnight + 0x2A);
        pKnight->posX = pKnight->posX + 0;
        pKnight->posY += -0x1F4;
        pKnight->posZ += 0x2710;
        BattleFx_KotrBumpKnightContextCounter(pKnight);
        (rec->frame)++;
    }
}

// Allocates a task slot for BattleFx_KotrKnightEndpointTrailTask and stores knightTrail and
// endpointDef as dwords and duration as a word.
// FUNCTION: FF7 0x0047afca
void __cdecl BattleFx_KotrSpawnKnightEndpointTrail(BattleKotrKnight *knightTrail, int duration, void *endpointDef)
{
    BattleFxKotrEndpointTrailTask *task;

    task = (BattleFxKotrEndpointTrailTask *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrKnightEndpointTrailTask);
    task->knightTrail = knightTrail;
    task->duration = duration;
    task->endpointDef = endpointDef;
}

/*
 * Acquires retained scratch and reads the saved
 * endpoint definition and knight context. Gets actor three attachment matrix at the context bone,
 * installing it directly when context bit one is set or composing it relative to
 * g_BattleSummonKotrTransformState otherwise, then transforms both definition points into narrowed
 * current endpoints. Derives two endpoint radii from absolute definition Z times context scale and
 * fixed-point weights from their sum, using zero weights for zero sum. Above task count one
 * installs g_BattleSummonKotrKnightEndpointModel.summonKotrKnightEndpointMatrix or
 * g_BattleSummonKotrTransformState according to the same bit and constructs eight trail quads
 * between saved and current endpoints. Each step interpolates endpoints, computes the weighted
 * centre, normalizes each direction and restores its corresponding radius, projects the quad, and
 * for flags lacking 0x60000 shades endpoint colours by decreasing fractions and submits through the
 * same render pointer. Carries each adjusted endpoint pair into the next segment and publishes that
 * pointer. While unpaused advances count, retires when new count is at least saved duration minus
 * one and copies both current endpoints into the task even after retirement.
 */
// FUNCTION: FF7 0x0047b00a
void __cdecl BattleFx_KotrKnightEndpointTrailTask(void)
{
    int width1;
    int prevAlpha;
    int radius0;
    int stepIdx;
    PsxMatrix attachMtx;
    BattleFxKotrRenderStateBlock *primitive;
    int ratio0;
    BattleKotrTrailTask *taskPtr;
    int radius1;
    int frac;
    PsxMatrix localMatrix;
    BattleFxKnightEndpointDef *def;
    BattleFxKotrKnightTrail *pKnightTrail;

    taskPtr = (BattleKotrTrailTask *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrKnightEndpointTrailTaskInit & 1) == 0) {
        g_BattleKotrKnightEndpointTrailTaskInit |= 1;
        g_BattleSummonKotrEndpointTrail = (struct BattleFxKotrEndpointTrail *)(Gfx_GetPsxScratchpadPtr(0));
    }
    pKnightTrail = taskPtr->slot;
    def = taskPtr->endpointDef;
    if ((pKnightTrail->flags & 1) != 0) {
        Battle3d_GetActorAttachMatrix(3, pKnightTrail->attachIndex, &attachMtx);
        SetRotMatrix(&attachMtx);
        SetTransMatrix(&attachMtx);
    } else {
        Battle3d_GetActorAttachMatrix(3, pKnightTrail->attachIndex, &localMatrix);
        Battle_ComposeMatrixRelativeToCamera(&g_BattleSummonKotrTransformState.m[0][0], &localMatrix, &g_BattleSummonKotrEndpointTrail->matrix);
        SetRotMatrix(&g_BattleSummonKotrEndpointTrail->matrix);
        SetTransMatrix(&g_BattleSummonKotrEndpointTrail->matrix);
    }
    RotTransPoint((void *)def, &g_BattleSummonKotrEndpointTrail->deltaX, &g_BattleSummonKotrEndpointTrail->flags);
    g_BattleSummonKotrEndpointTrail->lastDeltaX = (short)g_BattleSummonKotrEndpointTrail->deltaX;
    g_BattleSummonKotrEndpointTrail->lastDeltaY = (short)g_BattleSummonKotrEndpointTrail->deltaY;
    g_BattleSummonKotrEndpointTrail->lastDeltaZ = (short)g_BattleSummonKotrEndpointTrail->deltaZ;
    RotTransPoint((char *)def + 8, &g_BattleSummonKotrEndpointTrail->deltaX, &g_BattleSummonKotrEndpointTrail->flags);
    g_BattleSummonKotrEndpointTrail->deltaXNow = (short)g_BattleSummonKotrEndpointTrail->deltaX;
    g_BattleSummonKotrEndpointTrail->deltaYNow = (short)g_BattleSummonKotrEndpointTrail->deltaY;
    g_BattleSummonKotrEndpointTrail->deltaZNow = (short)g_BattleSummonKotrEndpointTrail->deltaZ;
    radius0 = abs(def->radius0) * pKnightTrail->trailScale >> 0xC;
    radius1 = abs(def->radius1) * pKnightTrail->trailScale >> 0xC;
    if (radius0 + radius1 != 0) {
        ratio0 = abs((radius0 << 0xC) / (radius0 + radius1));
        width1 = abs((radius1 << 0xC) / (radius0 + radius1));
    } else {
        width1 = 0;
        ratio0 = width1;
    }
    *(Int64Pair *)&g_BattleSummonKotrEndpointTrail->prevEndpoint0X = *(Int64Pair *)&taskPtr->startEndpoint0X;
    *(Int64Pair *)&g_BattleSummonKotrEndpointTrail->prevEndpoint1X = *(Int64Pair *)&taskPtr->startEndpoint1X;
    prevAlpha = 0;
    SetDrawColorRGB(0, 0, 0);
    if (taskPtr->frame > 1) {
        if ((pKnightTrail->flags & 1) != 0) {
            SetRotMatrix(&g_BattleSummonKotrKnightEndpointModel.summonKotrKnightEndpointMatrix);
            SetTransMatrix(&g_BattleSummonKotrKnightEndpointModel.summonKotrKnightEndpointMatrix);
        } else {
            SetRotMatrix(&g_BattleSummonKotrTransformState);
            SetTransMatrix(&g_BattleSummonKotrTransformState);
        }
        RefreshGlobalTransformState();
        primitive = (BattleFxKotrRenderStateBlock *)Gfx_GetPsxPacketScratchPtr();
        for (stepIdx = 1; stepIdx <= 8; stepIdx++) {
            frac = (stepIdx << 0xC) / 8;
            g_BattleSummonKotrEndpointTrail->endpoint0X = taskPtr->startEndpoint0X + ((g_BattleSummonKotrEndpointTrail->lastDeltaX - taskPtr->startEndpoint0X) * frac >> 0xC);
            g_BattleSummonKotrEndpointTrail->endpoint0Y = taskPtr->startEndpoint0Y + ((g_BattleSummonKotrEndpointTrail->lastDeltaY - taskPtr->startEndpoint0Y) * frac >> 0xC);
            g_BattleSummonKotrEndpointTrail->endpoint0Z = taskPtr->startEndpoint0Z + ((g_BattleSummonKotrEndpointTrail->lastDeltaZ - taskPtr->startEndpoint0Z) * frac >> 0xC);
            g_BattleSummonKotrEndpointTrail->endpoint1X = taskPtr->startEndpoint1X + ((g_BattleSummonKotrEndpointTrail->deltaXNow - taskPtr->startEndpoint1X) * frac >> 0xC);
            g_BattleSummonKotrEndpointTrail->endpoint1Y = taskPtr->startEndpoint1Y + ((g_BattleSummonKotrEndpointTrail->deltaYNow - taskPtr->startEndpoint1Y) * frac >> 0xC);
            g_BattleSummonKotrEndpointTrail->endpoint1Z = taskPtr->startEndpoint1Z + ((g_BattleSummonKotrEndpointTrail->deltaZNow - taskPtr->startEndpoint1Z) * frac >> 0xC);
            g_BattleSummonKotrEndpointTrail->savedEndpoint0X = g_BattleSummonKotrEndpointTrail->endpoint0X * width1 + g_BattleSummonKotrEndpointTrail->endpoint1X * ratio0 >> 0xC;
            g_BattleSummonKotrEndpointTrail->savedEndpoint0Y = g_BattleSummonKotrEndpointTrail->endpoint0Y * width1 + g_BattleSummonKotrEndpointTrail->endpoint1Y * ratio0 >> 0xC;
            g_BattleSummonKotrEndpointTrail->savedEndpoint0Z = g_BattleSummonKotrEndpointTrail->endpoint0Z * width1 + g_BattleSummonKotrEndpointTrail->endpoint1Z * ratio0 >> 0xC;
            g_BattleSummonKotrEndpointTrail->deltaX = g_BattleSummonKotrEndpointTrail->endpoint0X - g_BattleSummonKotrEndpointTrail->savedEndpoint0X;
            g_BattleSummonKotrEndpointTrail->deltaY = g_BattleSummonKotrEndpointTrail->endpoint0Y - g_BattleSummonKotrEndpointTrail->savedEndpoint0Y;
            g_BattleSummonKotrEndpointTrail->deltaZ = g_BattleSummonKotrEndpointTrail->endpoint0Z - g_BattleSummonKotrEndpointTrail->savedEndpoint0Z;
            NormalizeVectorFixed(&g_BattleSummonKotrEndpointTrail->deltaX, &g_BattleSummonKotrEndpointTrail->deltaX);
            g_BattleSummonKotrEndpointTrail->endpoint0X = g_BattleSummonKotrEndpointTrail->savedEndpoint0X + (g_BattleSummonKotrEndpointTrail->deltaX * radius0 >> 0xC);
            g_BattleSummonKotrEndpointTrail->endpoint0Y = g_BattleSummonKotrEndpointTrail->savedEndpoint0Y + (g_BattleSummonKotrEndpointTrail->deltaY * radius0 >> 0xC);
            g_BattleSummonKotrEndpointTrail->endpoint0Z = g_BattleSummonKotrEndpointTrail->savedEndpoint0Z + (g_BattleSummonKotrEndpointTrail->deltaZ * radius0 >> 0xC);
            g_BattleSummonKotrEndpointTrail->deltaX = g_BattleSummonKotrEndpointTrail->endpoint1X - g_BattleSummonKotrEndpointTrail->savedEndpoint0X;
            g_BattleSummonKotrEndpointTrail->deltaY = g_BattleSummonKotrEndpointTrail->endpoint1Y - g_BattleSummonKotrEndpointTrail->savedEndpoint0Y;
            g_BattleSummonKotrEndpointTrail->deltaZ = g_BattleSummonKotrEndpointTrail->endpoint1Z - g_BattleSummonKotrEndpointTrail->savedEndpoint0Z;
            NormalizeVectorFixed(&g_BattleSummonKotrEndpointTrail->deltaX, &g_BattleSummonKotrEndpointTrail->deltaX);
            g_BattleSummonKotrEndpointTrail->endpoint1X = g_BattleSummonKotrEndpointTrail->savedEndpoint0X + (g_BattleSummonKotrEndpointTrail->deltaX * radius1 >> 0xC);
            g_BattleSummonKotrEndpointTrail->endpoint1Y = g_BattleSummonKotrEndpointTrail->savedEndpoint0Y + (g_BattleSummonKotrEndpointTrail->deltaY * radius1 >> 0xC);
            g_BattleSummonKotrEndpointTrail->endpoint1Z = g_BattleSummonKotrEndpointTrail->savedEndpoint0Z + (g_BattleSummonKotrEndpointTrail->deltaZ * radius1 >> 0xC);
            g_BattleSummonKotrEndpointTrail->depth = Gfx_TransformQuadReturnDepth(&g_BattleSummonKotrEndpointTrail->prevEndpoint0X, (SVECTOR *)&g_BattleSummonKotrEndpointTrail->prevEndpoint1X, (SVECTOR *)&g_BattleSummonKotrEndpointTrail->endpoint0X, (SVECTOR *)&g_BattleSummonKotrEndpointTrail->endpoint1X, &g_BattleSummonKotrEndpointTrail->quadScratch, &g_BattleSummonKotrEndpointTrail->flags);
            if ((g_BattleSummonKotrEndpointTrail->flags & 0x60000) == 0) {
                primitive->opcode = 0x8000000;
                Gfx_ShadePsxVertexColor((CVECTOR *)((int)def + 0x10), 0x1000 - prevAlpha, (CVECTOR *)&g_BattleSummonKotrEndpointTrail->shadeColor);
                primitive->color = g_BattleSummonKotrEndpointTrail->shadeColor & 0xFFFFFF | 0x3A000000;
                primitive->colorWord = primitive->color;
                Gfx_ShadePsxVertexColor((CVECTOR *)((int)def + 0x10), 0x1000 - frac, (CVECTOR *)&g_BattleSummonKotrEndpointTrail->shadeColor);
                primitive->rawColor = g_BattleSummonKotrEndpointTrail->shadeColor;
                primitive->rawColorWord = primitive->rawColor;
                Gfx_DrawPsxPrimitive(primitive);
            }
            *(Int64Pair *)&g_BattleSummonKotrEndpointTrail->prevEndpoint0X = *(Int64Pair *)&g_BattleSummonKotrEndpointTrail->endpoint0X;
            *(Int64Pair *)&g_BattleSummonKotrEndpointTrail->prevEndpoint1X = *(Int64Pair *)&g_BattleSummonKotrEndpointTrail->endpoint1X;
            prevAlpha = frac;
        }
        g_BattleOverlayDrawListHead = (int)primitive;
    }
    if (g_BattleIsPaused == 0) {
        (taskPtr->frame)++;
        if (taskPtr->frame >= taskPtr->frameCount - 1) {
            taskPtr->endFlag = 0xFFFF;
        }
        *(Int64Pair *)&taskPtr->startEndpoint0X = *(Int64Pair *)&g_BattleSummonKotrEndpointTrail->lastDeltaX;
        *(Int64Pair *)&taskPtr->startEndpoint1X = *(Int64Pair *)&g_BattleSummonKotrEndpointTrail->deltaXNow;
    }
}

/*
 * Acquires retained scratch and builds the knight's pose matrix according to knight->state: look
 * basis from position minus offset for one, from the negative offset for two, or the rotation
 * shorts at +0x22 for other states. Concatenates the rotation at rotX..rotZ and installs the
 * knight position as translation. When flags bit one is clear, composes relative to the knight's
 * matrix at +0x5C, saves the narrowed translation to composedPosX..Z and multiplies
 * g_BattleSummonKotrTransformState. Selects white draw colour and the knight's model bank; for a
 * nonnull model writes uniform float scale from knight->scale divided by 4096, float rotation from
 * rotX..rotZ times 0.087890625 and zero offsets, converts the matrix and calls
 * BattleFx_DrawModelWithFade with fadeLevel and animFrame. Independently, when
 * unpaused and fade is nonzero, composes relative to g_BattleSummonKotrTransformState and emits two
 * BattleFx_KotrPulsingTriangleParticleTask records at matrix translation plus random -200
 * through 199 per component, clearing two velocity shorts and saving rand in 0x10. Particle
 * emission also occurs for null model.
 */
// FUNCTION: FF7 0x0047b7ed
void __cdecl BattleFx_KotrDrawKnightModel(BattleKotrKnight *knight)
{
    struct {
        BattleFxKnightModelDraw *modelDraw;
        float matrix[16];
        int modelBase;
        BattleFxKotrKnightFragment *fragment;
        int fragIdx;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if ((g_BattleKotrDrawKnightModelInit & 1) == 0) {
        g_BattleKotrDrawKnightModelInit |= 1;
        g_BattleSummonKotrKnightLocalPose = (BattleSummonKotrKnightState *)Gfx_GetPsxScratchpadPtr(0);
    }
    switch (knight->state) {
    case BATTLE_KOTR_KNIGHT_LOOK_DELTA:
        g_BattleSummonKotrKnightLocalPose->posX = knight->posX - knight->offsetX;
        g_BattleSummonKotrKnightLocalPose->posY = knight->posY - knight->offsetY;
        g_BattleSummonKotrKnightLocalPose->posZ = knight->posZ - knight->offsetZ;
        Gte_BuildLookBasisFromDirection(&g_BattleSummonKotrKnightLocalPose->posX, &g_BattleSummonKotrKnightLocalPose->pose.m[0][0]);
        break;
    case BATTLE_KOTR_KNIGHT_LOOK_NEG_OFFSET:
        g_BattleSummonKotrKnightLocalPose->posX = -knight->offsetX;
        g_BattleSummonKotrKnightLocalPose->posY = -knight->offsetY;
        g_BattleSummonKotrKnightLocalPose->posZ = -knight->offsetZ;
        Gte_BuildLookBasisFromDirection(&g_BattleSummonKotrKnightLocalPose->posX, &g_BattleSummonKotrKnightLocalPose->pose.m[0][0]);
        break;
    default:
        RotMatrix((SVECTOR *)((int)knight + 0x22), &g_BattleSummonKotrKnightLocalPose->pose);
    }
    RotMatrix((SVECTOR *)((int)knight + 0x40), &g_BattleSummonKotrKnightLocalPose->local);
    ConcatMatrixIntoFirst(&g_BattleSummonKotrKnightLocalPose->pose, &g_BattleSummonKotrKnightLocalPose->local);
    g_BattleSummonKotrKnightLocalPose->pose.t[0] = knight->posX;
    g_BattleSummonKotrKnightLocalPose->pose.t[1] = knight->posY;
    g_BattleSummonKotrKnightLocalPose->pose.t[2] = knight->posZ;
    if ((knight->flags & 1) == 0) {
        Battle_ComposeMatrixRelativeToCamera((short *)((int)knight + 0x5C), &g_BattleSummonKotrKnightLocalPose->pose, &g_BattleSummonKotrKnightLocalPose->local);
        knight->composedPosX = g_BattleSummonKotrKnightLocalPose->composedX;
        knight->composedPosY = g_BattleSummonKotrKnightLocalPose->composedY;
        knight->composedPosZ = g_BattleSummonKotrKnightLocalPose->composedZ;
        MultiplyMatrix(&g_BattleSummonKotrTransformState, &g_BattleSummonKotrKnightLocalPose->local, &g_BattleSummonKotrKnightLocalPose->pose);
    }
    SetDrawColorRGB(0xFF, 0xFF, 0xFF);
    frame.modelBase = (int)g_BattleKotrKnightModels[knight->knightIndex];
    if (frame.modelBase != 0) {
        frame.modelDraw = (BattleFxKnightModelDraw *)(frame.modelBase + 0x20);
        frame.modelDraw->scaleX = (float)knight->scale / 4096.0f;
        frame.modelDraw->scaleY = (float)knight->scale / 4096.0f;
        frame.modelDraw->scaleZ = (float)knight->scale / 4096.0f;
        frame.modelDraw->rotX = (float)knight->rotX * 0.087890625f;
        frame.modelDraw->rotY = (float)knight->rotY * 0.087890625f;
        frame.modelDraw->rotZ = (float)knight->rotZ * 0.087890625f;
        frame.modelDraw->offsetX = 0;
        frame.modelDraw->offsetY = 0;
        frame.modelDraw->offsetZ = 0;
        GtePsxMatrixToFloat4x4Transposed(&g_BattleSummonKotrKnightLocalPose->pose, frame.matrix);
        BattleFx_DrawModelWithFade(knight->fadeLevel, (PolygonTransform *)frame.matrix, knight->animFrame, g_BattleKotrKnightModels[knight->knightIndex]);
    }
    if (g_BattleIsPaused == 0 && knight->fadeLevel != 0) {
        Battle_ComposeMatrixRelativeToCamera(&g_BattleSummonKotrTransformState.m[0][0], &g_BattleSummonKotrKnightLocalPose->pose, &g_BattleSummonKotrKnightLocalPose->pose);
        for (frame.fragIdx = 0; frame.fragIdx < 2; frame.fragIdx++) {
            frame.fragment = (BattleFxKotrKnightFragment *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrPulsingTriangleParticleTask);
            frame.fragment->posX = g_BattleSummonKotrKnightLocalPose->pose.t[0] + rand() % 0x190 + -0xC8;
            frame.fragment->posY = g_BattleSummonKotrKnightLocalPose->pose.t[1] + rand() % 0x190 + -0xC8;
            frame.fragment->posZ = g_BattleSummonKotrKnightLocalPose->pose.t[2] + rand() % 0x190 + -0xC8;
            frame.fragment->rotY = 0;
            frame.fragment->rotX = 0;
            frame.fragment->randomValue = rand();
        }
    }
}

/*
 * Acquires retained scratch and
 * writes two opposite global vertex Y values as sine of task count shifted by twelve divided by
 * eight times 100 shifted twelve. Builds task rotation, transforms task position through
 * g_BattleSummonKotrTransformState into its translation, installs that matrix and projects three
 * retained global vertices. For flags lacking 0x60000 preserves the render header low 24 bits with
 * length 0x07000000, writes command/colour/pattern words including 0x58, 0x55555555 and 0x5A000000,
 * and submits. While unpaused advances count and retires at eight; otherwise adds random 0x200
 * through 0x5FF to rotation short 0x10.
 */
// FUNCTION: FF7 0x0047bc2c
void __cdecl BattleFx_KotrPulsingTriangleParticleTask(void)
{
    BattleFxKotrPulseTrianglePrim *prim;
    BattleFxKotrPulsingTriangleTask *task;
    int phaseStep;

    task = (BattleFxKotrPulsingTriangleTask *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrPulsingTriangleParticleTaskInit & 1) == 0) {
        g_BattleKotrPulsingTriangleParticleTaskInit |= 1;
        g_BattleKotrTriangleParticle = (BattleFxKotrTriangleParticle *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonKotrPulseTriangleV2.kotrTriangleOffsetSin = SinFixed((task->frame << 0xC) / 8) * 0x64 >> 0xC;
    g_BattleSummonKotrPulseTriangleV0.kotrTriangleOffsetNeg = -g_BattleSummonKotrPulseTriangleV2.kotrTriangleOffsetSin;
    prim = (BattleFxKotrPulseTrianglePrim *)Gfx_GetPsxPacketScratchPtr();
    RotMatrix((SVECTOR *)((int)task + 0xC), (int *)g_BattleKotrTriangleParticle);
    SetRotMatrix(&g_BattleSummonKotrTransformState);
    SetTransMatrix(&g_BattleSummonKotrTransformState);
    RotTransPoint((char *)task + 4, (char *)g_BattleKotrTriangleParticle + 0x12, (char *)g_BattleKotrTriangleParticle + 0x28);
    SetRotMatrix((void *)g_BattleKotrTriangleParticle);
    SetTransMatrix(g_BattleKotrTriangleParticle);
    RefreshGlobalTransformState();
    g_BattleKotrTriangleParticle->depth = Gfx_TransformTriangleReturnDepthAlt(&g_BattleSummonKotrPulseTriangleV0, (SVECTOR *)&g_BattleSummonKotrPulseTriangleV1, (SVECTOR *)&g_BattleSummonKotrPulseTriangleV2, (char *)g_BattleKotrTriangleParticle + 0x24, (char *)g_BattleKotrTriangleParticle + 0x28);
    if ((g_BattleKotrTriangleParticle->flags & 0x60000) == 0) {
        prim->header = prim->header & 0xFFFFFF | 0x7000000;
        *(char *)((char *)prim + 7) = 0x58;
        prim->endMarker = 0x55555555;
        prim->colorCommand = 0x5A000000;
        prim->color = 0x00808080;
        prim->thirdColor = 0;
        Gfx_DrawPsxPrimitive(prim);
    }
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
        if (task->frame >= 8) {
            task->ownerSlot = BATTLE_TASK_FREE;
        } else {
            if (DEAD_IN_RETAIL != 0) {
                phaseStep = 0x600;
            } else {
                phaseStep = rand() % 0x400 + 0x200;
            }
            task->phase += phaseStep;
        }
    }
}

/*
 * Increments the int at knight + 0x7C. Mechanical counter bump on an untyped record.
 */
// FUNCTION: FF7 0x0047be2f
void __cdecl BattleFx_KotrBumpKnightContextCounter(BattleKotrKnight *knight)
{
    (knight->animFrame)++;
}

/*
 * When unpaused repeatedly reads the g_BattleHitTargetActors actor byte at g_BattleSummonKotrImpactRecordIndex; for non-0xFF values
 * allocates a type-ten ring record with actor and sequence index and advances that index. Returns
 * at 0xFF and otherwise continues only while the next actor byte exceeds the preceding byte; no
 * table-bound guard appears.
 */
// FUNCTION: FF7 0x0047be43
void __cdecl BattleFx_KotrPostKnightImpactRecords(void)
{
    int ringRecord;
    int actorIndex;

    if (g_BattleIsPaused != 0) {
        return;
    }
    do {
        actorIndex = g_BattleHitTargetActors[g_BattleSummonKotrImpactRecordIndex];
        if (actorIndex != 0xFF) {
            ringRecord = Battle_AllocSceneMessage(0xA);
            *(short *)ringRecord = (short)actorIndex;
            *(short *)(ringRecord + 0x10) = *(short *)&g_BattleSummonKotrImpactRecordIndex;
            g_BattleSummonKotrImpactRecordIndex++;
        } else {
            return;
        }
    } while (g_BattleHitTargetActors[g_BattleSummonKotrImpactRecordIndex] > *(unsigned char *)(g_BattleSummonKotrImpactRecordIndex + ((int)((char *)&g_BattleEffectScratch + 0x8d6f))));
}

/*
 * Two-step screen-flash driver on the 32-byte record at g_BattleHitLivewireEmitterRecords selected by g_BattleActiveTaskSlotIdx.
 * Returns immediately while g_BattleIsPaused is nonzero; on step 0 of the short at +2 it calls
 * BattleFx_StartScreenFlashRamp(1), on step 1 it calls BattleFx_FadeOutScreenFlashRamp(7) and
 * stores 0xFFFF at +0 to retire the record. The step counter at +2 is incremented on every frame
 * that gets past the early return.
 */
// FUNCTION: FF7 0x0047becb
void __cdecl BattleFx_KotrImpactScreenFlashTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        BattleFx_StartScreenFlashRamp(1);
    } else if (task->frameCounter == 1) {
        BattleFx_FadeOutScreenFlashRamp(7);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Calls BattleFx_KotrTransformAttachPointToLocal with the word at knight+0x58 and the address of local q, then passes that same
 * address to BattleFx_KotrSpawnActorBurstSprites; unreferenced padding slots preserve VC5 /Od frame size.
 */
// FUNCTION: FF7 0x0047bf3a
void __cdecl BattleFx_KotrSpawnKnightActorBurstSprites(BattleKotrKnight *knight)
{
    int padLocal; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    SVECTOR localOrigin;

    BattleFx_KotrTransformAttachPointToLocal(knight->attachIndex, &localOrigin);
    BattleFx_KotrSpawnActorBurstSprites((Vec3sPacked *)&localOrigin);
}

/*
 * When unpaused emits three BattleFx_KotrActorBurstSpriteTask slots for each selected low-ten-bit actor, copying its
 * transformed bone position, normalizing the difference from origin and reducing it by four right
 * shifts. Builds independent random rotation shorts -256 through 255 and transforms that reduced
 * direction into each child velocity.
 */
// FUNCTION: FF7 0x0047bf64
void __cdecl BattleFx_KotrSpawnActorBurstSprites(Vec3sPacked *origin)
{
    struct {
        BattleFxPulseOfLifeSprite *task;
        PsxMatrix matrix;
        unsigned char pad_22[2];
        int spriteIdx;
        SVECTOR rot;
        int actor;
        CameraScriptVector localPos;
        int delta[3];
        int unused72;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_BattleIsPaused != 0) {
        return;
    }
    for (frame.actor = 0; frame.actor < 0xA; (frame.actor)++) {
        if ((g_BattleKotrTargetMask & 1 << frame.actor) != 0) {
            for (frame.spriteIdx = 0; frame.spriteIdx < 3; (frame.spriteIdx)++) {
                frame.task = (BattleFxPulseOfLifeSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrActorBurstSpriteTask);
                BattleFx_KotrGetActorAttachLocalPosition(frame.actor, g_BattleActorRecords[frame.actor].targetBone, &frame.localPos);
                *(Int64Pair *)&frame.task->posX = *(Int64Pair *)&frame.localPos;
                frame.delta[0] = frame.localPos.x - origin->vx;
                frame.delta[1] = frame.localPos.y - origin->vy;
                frame.delta[2] = frame.localPos.z - origin->vz;
                VectorNormalToSVector(frame.delta, (short *)&frame.localPos);
                frame.localPos.x >>= 4;
                frame.localPos.y >>= 4;
                frame.localPos.z >>= 4;
                frame.rot.vx = rand() % 0x200 - 0x100;
                frame.rot.vy = rand() % 0x200 - 0x100;
                frame.rot.vz = rand() % 0x200 - 0x100;
                RotMatrix(&frame.rot, (int *)&frame.matrix);
                TransformVectorToInt16((void *)&frame.matrix, &frame.localPos, &frame.task->velocityX);
            }
        }
    }
}

/*
 * Reads the actor actorIndex attachment
 * matrix, subtracts the translation of g_BattleSummonKotrTransformState from the
 * attachment matrix's translation into localPos, transposes g_BattleSummonKotrTransformState
 * into local scratch, and transforms the output vector in place.
 */
// FUNCTION: FF7 0x0047c105
void __cdecl BattleFx_KotrGetActorAttachLocalPosition(int actorIndex, int attachIndex, void *localPos)
{
    BattleKotrAttachMatrices attachMatrix;

    Battle3d_GetActorAttachMatrix(actorIndex, attachIndex, &attachMatrix.attach);
    ((Vec3sPacked *)localPos)->vx = attachMatrix.attach.t[0] - g_BattleSummonKotrTransformState.t[0];
    ((Vec3sPacked *)localPos)->vy = attachMatrix.attach.t[1] - g_BattleSummonKotrTransformState.t[1];
    ((Vec3sPacked *)localPos)->vz = attachMatrix.attach.t[2] - g_BattleSummonKotrTransformState.t[2];
    TransposeMatrix(&g_BattleSummonKotrTransformState, &attachMatrix.transposed);
    TransformVectorToInt16(&attachMatrix.transposed, (SVECTOR *)localPos, (short *)localPos);
}

/*
 * Selects sprite field seventeen, installs its position with BattleFx_KotrInstallBillboardMatrix at scale 0x6000 and
 * draws before checking pause. When unpaused retires a null sprite immediately or advances and
 * retires at or above its frame count before motion; surviving slots integrate position and damp
 * velocity by 0xF00/0x1000.
 */
// FUNCTION: FF7 0x0047c177
void __cdecl BattleFx_KotrActorBurstSpriteTask(void)
{
    BattleFxPulseOfLifeSprite *task;

    task = (BattleFxPulseOfLifeSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSummonKotrBurstSprite.burstSpriteEntry) = Gfx_ModelTableGetLoadedRecord(0x11, g_BattleSummonKotrTableCursor);
    g_BattleSummonKotrBurstSprite.summonKotrBurstSpriteResource = Gfx_ModelTableGetSpriteObject(0x11, g_BattleSummonKotrTableCursor);
    BattleFx_KotrInstallBillboardMatrix((BattleFxSourcePoint *)((int)task + 4), 0x6000, 0);
    g_BattleSummonKotrBurstSprite.summonKotrBurstSpriteFrame = task->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonKotrBurstSprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        if ((g_BattleSummonKotrBurstSprite.burstSpriteEntry) != 0) {
            (task->frame)++;
            if (task->frame >= *(int *)((char *)*(int *)(g_BattleSummonKotrBurstSprite.burstSpriteEntry) + 4)) {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
        task->velocityX = task->velocityX * 0xF00 >> 0xC;
        task->velocityY = task->velocityY * 0xF00 >> 0xC;
        task->velocityZ = task->velocityZ * 0xF00 >> 0xC;
    }
}

/*
 * Writes the short angle into
 * g_BattleSummonKotrBillboard.angle, copies it on to g_BattleSummonKotrBillboardMatrixDiagY and
 * g_BattleSummonKotrBillboardMatrix, loads the matrix at
 * g_BattleSummonKotrTransformState (SetRotMatrix/SetTransMatrix) and RotTransPoint(sourcePoint,
 * &transX, &p) transforms sourcePoint into g_BattleSummonKotrBillboard.transX/transY/transZ. When
 * offset is nonzero it normalizes that vector (NormalizeVectorFixed) and adds unit * offset >> 0xC
 * to each component. Then loads the billboard matrix and returns its address.
 */
// FUNCTION: FF7 0x0047c2e4
int __cdecl BattleFx_KotrInstallBillboardMatrix(BattleFxSourcePoint *sourcePoint, short angle, int offset)
{
    int depth;
    VECTOR vecX;

    g_BattleSummonKotrBillboard.angle = angle;
    g_BattleSummonKotrBillboardMatrixDiagY = g_BattleSummonKotrBillboard.angle;
    *(short *)&g_BattleSummonKotrBillboardMatrix = g_BattleSummonKotrBillboardMatrixDiagY;
    SetRotMatrix(&g_BattleSummonKotrTransformState);
    SetTransMatrix(&g_BattleSummonKotrTransformState);
    RotTransPoint((void *)sourcePoint, &g_BattleSummonKotrBillboard.transX, &depth);
    if (offset != 0) {
        NormalizeVectorFixed(&g_BattleSummonKotrBillboard.transX, &vecX);
        g_BattleSummonKotrBillboard.transX += vecX.vx * offset >> 0xC;
        g_BattleSummonKotrBillboard.transY += vecX.vy * offset >> 0xC;
        g_BattleSummonKotrBillboard.transZ += vecX.vz * offset >> 0xC;
    }
    SetRotMatrix(&g_BattleSummonKotrBillboardMatrix);
    SetTransMatrix(&g_BattleSummonKotrBillboardMatrix);
    return (int)&g_BattleSummonKotrBillboardMatrix;
}

/*
 * When unpaused at counter zero records
 * the transformed bone position for every selected low-ten-bit actor into the stride-eight table at
 * g_BattleSummonKotrAttachLocalPositions. Creates a mode-zero bit-pattern task for
 * BattleFx_KotrSpawnKnight1SpinScaleModelPerTarget using g_BattleKotrTargetMask, advances its
 * counter and retires at one or above.
 */
// FUNCTION: FF7 0x0047c3bf
void __cdecl BattleFx_KotrKnight1SpinScaleModelSetupTask(void)
{
    BattleTaskSlot *taskPtr;
    int actorIndex;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frameCounter == 0) {
        for (actorIndex = 0; actorIndex < 0xA; actorIndex++) {
            if ((g_BattleKotrTargetMask & 1 << actorIndex) != 0) {
                BattleFx_KotrGetActorAttachLocalPosition(actorIndex, g_BattleActorRecords[actorIndex].targetBone, (actorIndex * 8 + (char *)&g_BattleSummonKotrAttachLocalPositions));
            }
        }
    }
    Battle_CreateBitPatternCallbackTask(g_BattleKotrTargetMask, 0, 0, BattleFx_KotrSpawnKnight1SpinScaleModelPerTarget);
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter >= 1) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Stores Gfx_ModelTableGetRenderObject(4,
 * g_BattleSummonKotrTableCursor) in g_BattleSummonKotrKnight1SpinScaleModel, then calls
 * BattleFx_KotrSpawnEasedSpinScaleModel with that global and the 8-byte record at
 * g_BattleSummonKotrAttachLocalPositions + actor * 8, which allocates a display
 * slot and copies the record's three shorts into it.
 */
// FUNCTION: FF7 0x0047c48f
void __cdecl BattleFx_KotrSpawnKnight1SpinScaleModelPerTarget(int actor)
{
    (*(int *)&g_BattleSummonKotrKnight1SpinScaleModel) = Gfx_ModelTableGetRenderObject(4, g_BattleSummonKotrTableCursor);
    BattleFx_KotrSpawnEasedSpinScaleModel(&g_BattleSummonKotrKnight1SpinScaleModel, (actor * 8 + (char *)&g_BattleSummonKotrAttachLocalPositions));
}

/*
 * Allocates BattleFx_KotrEasedSpinScaleModelTask, copies three record words into slot+4/+6/+8, and stores model as a dword at
 * +0xC.
 */
// FUNCTION: FF7 0x0047c4c1
void __cdecl BattleFx_KotrSpawnEasedSpinScaleModel(void *model, void *record)
{
    BattleFxKotrEasedSpinTask *task;

    task = (BattleFxKotrEasedSpinTask *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrEasedSpinScaleModelTask);
    task->posX = ((Vec3sPacked *)record)->vx;
    task->posY = ((Vec3sPacked *)record)->vy;
    task->posZ = ((Vec3sPacked *)record)->vz;
    task->model = (int)model;
}

/*
 * Acquires retained scratch and reads the task
 * parameter record at 0xC. Computes task count shifted by twelve divided by record duration, then
 * independently applies sine easing for yaw, uniform scale and descriptor. Builds Y rotation from
 * record short 0x10 times its eased fraction, concatenates record rotation at 0x14 and uses task
 * XYZ translation. Interpolates scale between record shorts six/eight using easing short 0xA,
 * stores easing with short 0xE in g_BattleSummonKotrEasedSpinScaleDraw.summonKotrEasedSpinScaleTime,
 * multiplies g_BattleSummonKotrTransformState and installs the result. Sets resource pointer from
 * record first dword, selects black and submits to g_BattleCurrentOrderingTable plus 0x70 with
 * shift twelve. While unpaused advances count and retires at record duration.
 */
// FUNCTION: FF7 0x0047c516
void __cdecl BattleFx_KotrEasedSpinScaleModelTask(void)
{
    BattleFxKotrSpinScaleRecord *taskPtr;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxKotrEasedSpinParams *spinParams;
    int progress;
    int easedValue;

    taskPtr = (BattleFxKotrSpinScaleRecord *)BATTLE_CURRENT_TASK_SLOT;
    spinParams = (BattleFxKotrEasedSpinParams *)(taskPtr->target);
    if ((g_BattleKotrEasedSpinScaleModelTaskInit & 1) == 0) {
        g_BattleKotrEasedSpinScaleModelTaskInit |= 1;
        g_BattleSummonKotrEasedSpinState = (BattleFxKotrEasedSpinState *)Gfx_GetPsxScratchpadPtr(0);
    }
    progress = (taskPtr->frame << 0xC) / spinParams->duration;
    easedValue = BattleFx_ApplySineEasing(progress, spinParams->yawEaseMode);
    g_BattleSummonKotrEasedSpinState->rotZ = 0;
    g_BattleSummonKotrEasedSpinState->rotX = 0;
    g_BattleSummonKotrEasedSpinState->easedAmplitude = spinParams->amplitude * easedValue >> 0xC;
    RotMatrix((SVECTOR *)g_BattleSummonKotrEasedSpinState, &g_BattleSummonKotrEasedSpinState->matrix);
    RotMatrix((SVECTOR *)((int)spinParams + 0x14), &g_BattleSummonKotrEasedSpinState->tiltMatrix);
    ConcatMatrixInPlace(&g_BattleSummonKotrEasedSpinState->tiltMatrix, &g_BattleSummonKotrEasedSpinState->matrix);
    g_BattleSummonKotrEasedSpinState->matrix.t[0] = taskPtr->posX;
    g_BattleSummonKotrEasedSpinState->matrix.t[1] = taskPtr->posY;
    g_BattleSummonKotrEasedSpinState->matrix.t[2] = taskPtr->posZ;
    easedValue = BattleFx_ApplySineEasing(progress, spinParams->scaleEaseMode);
    g_BattleSummonKotrEasedSpinState->valueNew = spinParams->startValue + ((spinParams->endValue - spinParams->startValue) * easedValue >> 0xC);
    g_BattleSummonKotrEasedSpinState->valuePrev = g_BattleSummonKotrEasedSpinState->valueNew;
    g_BattleSummonKotrEasedSpinState->valueOld = g_BattleSummonKotrEasedSpinState->valuePrev;
    ScaleMatrix(&g_BattleSummonKotrEasedSpinState->matrix, &g_BattleSummonKotrEasedSpinState->valueOld);
    easedValue = BattleFx_ApplySineEasing(progress, spinParams->drawEaseMode);
    g_BattleSummonKotrEasedSpinScaleDraw.summonKotrEasedSpinScaleTime = easedValue;
    MultiplyMatrix(&g_BattleSummonKotrTransformState, &g_BattleSummonKotrEasedSpinState->matrix, &g_BattleSummonKotrEasedSpinState->matrix);
    SetRotMatrix(&g_BattleSummonKotrEasedSpinState->matrix);
    SetTransMatrix(&g_BattleSummonKotrEasedSpinState->matrix);
    (g_BattleSummonKotrEasedSpinScaleDraw.modelGroup) = spinParams->modelGroup;
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonKotrEasedSpinScaleDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frame)++;
        if (taskPtr->frame >= spinParams->duration) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Acquires retained scratch and selects the knight
 * context at task 0x14. At count zero calls BattleFx_KotrSpawnKnightEndpointTrail twice with
 * duration 0x48 and the two retained definitions; other calls draw/update through
 * BattleFx_KotrDrawKnightModel even while paused. Unpaused count fifty calls
 * BattleFx_KotrSpawnKnightActorBurstSprites, allocates BattleFx_KotrKnight2SpinScaleModelSetupTask
 * and BattleFx_KotrImpactScreenFlashTask, then posts impact records. First 56 counts initialize
 * task vector (0,5000,0) and context rotation on entry, fade context 0x52 to zero over forty
 * counts, grow scale 0x4E from 0x100 to 0x1000 and ease task Y from 5000 toward minus 7000. Next
 * sixteen counts ease fade upward, scale downward and Y from minus 7000 toward minus 15000; later
 * retires and returns. Common unpaused tail rotates the task vector by fixed (0x380,0x800,0x800)
 * into context position, adds narrowed 0xFE0C to Y and 10000 to Z, retains the X self-assignment,
 * calls BattleFx_KotrBumpKnightContextCounter and increments count.
 */
// FUNCTION: FF7 0x0047c793
void __cdecl BattleFx_KotrKnight2TimelineTask(void)
{
    int frameIndex;
    BattleFxKotrKnightTimeline *taskPtr;
    int progress;
    int easedValue;
    BattleKotrKnight *pKnight;

    taskPtr = (BattleFxKotrKnightTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrKnight2TimelineTaskInit & 1) == 0) {
        g_BattleKotrKnight2TimelineTaskInit |= 1;
        g_BattleKotrKnight2State = (BattleKotrOrbitPose *)Gfx_GetPsxScratchpadPtr(0);
    }
    pKnight = taskPtr->knight;
    if (taskPtr->step != 0) {
        BattleFx_KotrDrawKnightModel(pKnight);
    } else {
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x48, &g_BattleSummonKotrKnight2SpinScaleModel.summonKotrKnight2EndpointPointA);
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x48, &g_BattleSummonKotrKnight2SpinScaleModel.summonKotrKnight2EndpointPointB);
    }
    if (g_BattleIsPaused == 0) {
        if (taskPtr->step == 0x32) {
            BattleFx_KotrSpawnKnightActorBurstSprites(pKnight);
            Battle_AllocateResourceSlot(BattleFx_KotrKnight2SpinScaleModelSetupTask);
            Battle_AllocateResourceSlot(BattleFx_KotrImpactScreenFlashTask);
            BattleFx_KotrPostKnightImpactRecords();
        }
        frameIndex = taskPtr->step;
        if (frameIndex < 0x38) {
            if (frameIndex == 0) {
                taskPtr->offsetX = 0;
                taskPtr->height = 0x1388;
                taskPtr->offsetZ = 0;
                pKnight->offsetX = 0x480;
                pKnight->offsetY = 0x800;
                pKnight->offsetZ = 0x800;
            }
            if (frameIndex < 0x28) {
                progress = (frameIndex << 0xC) / 0x28;
                easedValue = BattleFx_ApplySineEasing(progress, -0x3D0);
                pKnight->fadeLevel = 0x1000 - easedValue;
            } else {
                pKnight->fadeLevel = 0;
            }
            progress = (frameIndex << 0xC) / 0x38;
            easedValue = BattleFx_ApplySineEasing(progress, -0x3D0);
            pKnight->scale = (easedValue * 0xF00 >> 0xC) + 0x100;
            easedValue = BattleFx_ApplySineEasing(progress, -0x3D0);
            taskPtr->height = (easedValue * -0x2EE0 >> 0xC) + 0x1388;
        } else {
            frameIndex -= 0x38;
            if (frameIndex < 0x10) {
                progress = (frameIndex << 0xC) / 0x10;
                pKnight->fadeLevel = BattleFx_ApplySineEasing(progress, 0x1FF);
                pKnight->scale = (BattleFx_ApplySineEasing(progress, 0x1FF) * -0x1000 >> 0xC) + 0x1000;
                taskPtr->height = (BattleFx_ApplySineEasing(progress, 0x1FF) * -0x1F40 >> 0xC) - 0x1B58;
            } else {
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        g_BattleKotrKnight2State->point.vx = 0x380;
        g_BattleKotrKnight2State->point.vy = 0x800;
        g_BattleKotrKnight2State->point.vz = 0x800;
        RotMatrix(&g_BattleKotrKnight2State->point, &g_BattleKotrKnight2State->matrix);
        TransformVectorToInt16(&g_BattleKotrKnight2State->matrix, (char *)taskPtr + 4, (char *)pKnight + 0x2A);
        pKnight->posX = pKnight->posX;
        pKnight->posY += 0xFE0C;
        pKnight->posZ += 0x2710;
        BattleFx_KotrBumpKnightContextCounter(pKnight);
        (taskPtr->step)++;
    }
}

/*
 * When unpaused at counter zero records
 * the transformed bone position for every selected low-ten-bit actor into the stride-eight table at
 * g_BattleSummonKotrAttachLocalPositions. Creates a mode-zero bit-pattern task for
 * BattleFx_KotrSpawnKnight2SpinScaleModelPerTarget using g_BattleKotrTargetMask, advances its
 * counter and retires at one or above.
 */
// FUNCTION: FF7 0x0047caac
void __cdecl BattleFx_KotrKnight2SpinScaleModelSetupTask(void)
{
    BattleTaskSlot *taskPtr;
    int actorIndex;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frameCounter == 0) {
        for (actorIndex = 0; actorIndex < 0xA; actorIndex++) {
            if ((g_BattleKotrTargetMask & 1 << actorIndex) != 0) {
                BattleFx_KotrGetActorAttachLocalPosition(actorIndex, g_BattleActorRecords[actorIndex].targetBone, (actorIndex * 8 + (char *)&g_BattleSummonKotrAttachLocalPositions));
            }
        }
    }
    Battle_CreateBitPatternCallbackTask(g_BattleKotrTargetMask, 0, 0, BattleFx_KotrSpawnKnight2SpinScaleModelPerTarget);
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter >= 1) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Same shape as
 * BattleFx_KotrSpawnKnight1SpinScaleModelPerTarget with a different destination: stores
 * Gfx_ModelTableGetRenderObject(4, g_BattleSummonKotrTableCursor) in
 * g_BattleSummonKotrKnight2SpinScaleModel and passes it to BattleFx_KotrSpawnEasedSpinScaleModel
 * alongside the 8-byte record at g_BattleSummonKotrAttachLocalPositions + actor
 * * 8.
 */
// FUNCTION: FF7 0x0047cb7c
void __cdecl BattleFx_KotrSpawnKnight2SpinScaleModelPerTarget(int actor)
{
    (g_BattleSummonKotrKnight2SpinScaleModel.spinScaleModel) = Gfx_ModelTableGetRenderObject(4, g_BattleSummonKotrTableCursor);
    BattleFx_KotrSpawnEasedSpinScaleModel(&g_BattleSummonKotrKnight2SpinScaleModel, (actor * 8 + (char *)&g_BattleSummonKotrAttachLocalPositions));
}

/*
 * Acquires retained scratch and selects the knight context. At count zero calls BattleFx_KotrSpawnKnightEndpointTrail twice
 * with duration 0x44 and retained definitions, then sets context bit one; other calls BattleFx_KotrDrawKnightModel
 * before the pause check. Unpaused count 26 allocates BattleFx_KotrImpactBurstTimelineTask. First fifteen counts
 * initialize look mode one, anchor (0,-500,15000) and Y rotation 0xFD00, easing radius from 8000
 * toward 6000 and polar angle from 0x300 toward 0x380 while growing scale and reducing fade. Next
 * 28 counts hold radius 6000, angle 0x380 and set full scale/zero fade on entry. Final 25 counts
 * select mode two with anchor relative to current position, ease angle toward 0x600 and radius
 * toward 10000, alter Y rotation, and independently ease scale down/fade up. Later retires and
 * returns. Common unpaused tail rotates the polar XY vector through fixed angles
 * (0xFC80,0x400,0xFC00), places it around (0,-500,15000), calls BattleFx_KotrBumpKnightContextCounter and advances count.
 */
// FUNCTION: FF7 0x0047cbae
void __cdecl BattleFx_KotrKnight3TimelineTask(void)
{
    int frameIndex;
    BattleFxKotrKnight8Record *taskPtr;
    int progress;
    int easedValue;
    BattleKotrKnight *pKnight;

    taskPtr = (BattleFxKotrKnight8Record *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrKnight3TimelineTaskInit & 1) == 0) {
        g_BattleKotrKnight3TimelineTaskInit |= 1;
        g_BattleKotrKnight3State = (BattleKotrOrbitPose *)Gfx_GetPsxScratchpadPtr(0);
    }
    pKnight = taskPtr->knight;
    if (taskPtr->step != 0) {
        BattleFx_KotrDrawKnightModel(pKnight);
    } else {
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x44, &g_BattleSummonKotrKnight3EndpointA);
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x44, &g_BattleSummonKotrKnight3EndpointB);
        pKnight->flags |= 1;
    }
    if (g_BattleIsPaused == 0) {
        if (taskPtr->step == 0x1A) {
            Battle_AllocateResourceSlot(BattleFx_KotrImpactBurstTimelineTask);
        }
        frameIndex = taskPtr->step;
        if (frameIndex < 0xF) {
            if (frameIndex == 0) {
                pKnight->state = 1;
                pKnight->offsetX = 0;
                pKnight->offsetY = 0xFE0C;
                pKnight->offsetZ = 0x3A98;
                pKnight->rotY = 0xFD00;
            }
            progress = (frameIndex << 0xC) / 0xF;
            progress = BattleFx_ApplySineEasing(progress, 0x1FF);
            g_BattleSummonKotrKnight3Radius = (progress * -0x7D0 >> 0xC) + 0x1F40;
            g_BattleKotrKnight3Angle = (progress << 7 >> 0xC) + 0x300;
            pKnight->scale = progress << 0xC >> 0xC;
            pKnight->fadeLevel = 0x1000 - progress;
        } else {
            frameIndex -= 0xF;
            if (frameIndex < 0x1C) {
                if (frameIndex == 0) {
                    pKnight->scale = 0x1000;
                    pKnight->fadeLevel = 0;
                }
                g_BattleKotrKnight3Angle = 0x380;
                g_BattleSummonKotrKnight3Radius = 0x1770;
            } else {
                frameIndex -= 0x1C;
                if (frameIndex < 0x19) {
                    if (frameIndex == 0) {
                        pKnight->state = 2;
                        pKnight->offsetX = 0 - pKnight->posX;
                        pKnight->offsetY = -0x1F4 - pKnight->posY;
                        pKnight->offsetZ = 0x3A98 - pKnight->posZ;
                    }
                    progress = (frameIndex << 0xC) / 0x19;
                    easedValue = BattleFx_ApplySineEasing(progress, -0x3C0);
                    g_BattleKotrKnight3Angle = (easedValue * 0x280 >> 0xC) + 0x380;
                    g_BattleSummonKotrKnight3Radius = (easedValue * 0xFA0 >> 0xC) + 0x1770;
                    pKnight->rotY = (easedValue << 0xA >> 0xC) - 0x400;
                    easedValue = BattleFx_ApplySineEasing(progress, -0x4FF);
                    pKnight->scale = (easedValue * -0x1000 >> 0xC) + 0x1000;
                    pKnight->fadeLevel = easedValue;
                } else {
                    taskPtr->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
        g_BattleKotrKnight3State->point.vx = 0xFC80;
        g_BattleKotrKnight3State->point.vy = 0x400;
        g_BattleKotrKnight3State->point.vz = 0xFC00;
        RotMatrix(&g_BattleKotrKnight3State->point, &g_BattleKotrKnight3State->matrix);
        g_BattleKotrKnight3State->point.vx = CosFixed(g_BattleKotrKnight3Angle) * g_BattleSummonKotrKnight3Radius >> 0xC;
        g_BattleKotrKnight3State->point.vy = SinFixed(g_BattleKotrKnight3Angle) * g_BattleSummonKotrKnight3Radius >> 0xC;
        g_BattleKotrKnight3State->point.vz = 0;
        TransformVectorToInt16(&g_BattleKotrKnight3State->matrix, &g_BattleKotrKnight3State->point, &g_BattleKotrKnight3State->point.vx);
        pKnight->posX = g_BattleKotrKnight3State->point.vx;
        pKnight->posY = g_BattleKotrKnight3State->point.vy - 0x1F4;
        pKnight->posZ = g_BattleKotrKnight3State->point.vz + 0x3A98;
        BattleFx_KotrBumpKnightContextCounter(pKnight);
        (taskPtr->step)++;
    }
}

/*
 * Knights-of-the-Round spawn step on rec =
 * &g_BattleHitLivewireEmitterRecords + (g_BattleActiveTaskSlotIdx << 5) using the scratch block at
 * g_BattleSummonKotrImpactBurst (claimed from Gfx_GetPsxScratchpadPtr(0) on first use),
 * returning while g_BattleIsPaused is set: on frames 0..4 it composes actor 3 bone 0x1E's attach
 * matrix relative to the camera (g_BattleSummonKotrTransformState) into block+0x18, transforms the
 * origin through it, adds the translation at +0x2A/0x2E/0x32, stores minus one-sixteenth of that as
 * a velocity at +8, and spawns five BattleFx_KotrImpactSpriteParticleTask records each given the
 * position and a velocity rotated by random angles in [-0x20, 0x20); on frames 0xF, 0x12, 0x15,
 * 0x18 it spawns a BattleFx_KotrSpinningGrowingModelTask record at the (frame-0xF)/3'th 8-byte
 * position in g_BattleSummonKotrImpactBurstOrigins with three random 12-bit angles; on frames 0xF, 0x14 and 0x19
 * it spawns a BattleFx_KotrImpactSpriteRingTask record with angles 0x200, 0x400 and a random value
 * in [-0x100, 0x100); frame 0x14 posts the knight impact records and frame 0x19 ends the record
 * with 0xFFFF; the frame counter is incremented every pass.
 */
// FUNCTION: FF7 0x0047cf84
void __cdecl BattleFx_KotrImpactBurstTimelineTask(void)
{
    BattleTaskSlot *rec;
    BattleFxKotrImpactBurstSlot *spawned;
    int particleIdx;
    char rot[0x20];

    rec = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrImpactBurstTimelineTaskInit & 1) == 0) {
        g_BattleKotrImpactBurstTimelineTaskInit |= 1;
        g_BattleSummonKotrImpactBurst = (struct BattleKotrImpactBurst *)(Gfx_GetPsxScratchpadPtr(0));
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (rec->frameCounter >= 0 && rec->frameCounter < 5) {
        Battle3d_GetActorAttachMatrix(3, 0x1E, (PsxMatrix *)rot);
        Battle_ComposeMatrixRelativeToCamera(&g_BattleSummonKotrTransformState.m[0][0], rot, (PsxMatrix *)((((int)g_BattleSummonKotrImpactBurst) + 0x18)));
        g_BattleSummonKotrImpactBurst->posY = 0;
        g_BattleSummonKotrImpactBurst->posX = 0;
        g_BattleSummonKotrImpactBurst->posZ = 0;
        TransformVectorToInt16((void *)(((int)g_BattleSummonKotrImpactBurst) + 0x18), g_BattleSummonKotrImpactBurst, g_BattleSummonKotrImpactBurst);
        g_BattleSummonKotrImpactBurst->posX += g_BattleSummonKotrImpactBurst->velocityX;
        g_BattleSummonKotrImpactBurst->posY += g_BattleSummonKotrImpactBurst->velocityY;
        g_BattleSummonKotrImpactBurst->posZ += g_BattleSummonKotrImpactBurst->velocityZ;
        g_BattleSummonKotrImpactBurst->offsetX = -g_BattleSummonKotrImpactBurst->posX / 16;
        g_BattleSummonKotrImpactBurst->offsetY = -g_BattleSummonKotrImpactBurst->posY / 16;
        g_BattleSummonKotrImpactBurst->offsetZ = -g_BattleSummonKotrImpactBurst->posZ / 16;
        for (particleIdx = 0; particleIdx < 5; particleIdx++) {
            spawned = (BattleFxKotrImpactBurstSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrImpactSpriteParticleTask);
            *(Int64Pair *)((int)spawned + 4) = *(Int64Pair *)&g_BattleSummonKotrImpactBurst->posX;
            g_BattleSummonKotrImpactBurst->rotX = rand() % 0x40 - 0x20;
            g_BattleSummonKotrImpactBurst->rotY = rand() % 0x40 - 0x20;
            g_BattleSummonKotrImpactBurst->rotZ = rand() % 0x40 - 0x20;
            RotMatrix((SVECTOR *)&g_BattleSummonKotrImpactBurst->rotX, (char *)g_BattleSummonKotrImpactBurst + 0x18);
            TransformVectorToInt16((void *)(((int)g_BattleSummonKotrImpactBurst) + 0x18), &g_BattleSummonKotrImpactBurst->offsetX, (char *)spawned + 0xC);
        }
    }
    if (rec->frameCounter >= 0xF && rec->frameCounter < 0x1B && (rec->frameCounter - 0xF) % 3 == 0) {
        particleIdx = (rec->frameCounter - 0xF) / 3;
        spawned = (BattleFxKotrImpactBurstSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrSpinningGrowingModelTask);
        spawned->originX = g_BattleSummonKotrImpactBurstOrigins[particleIdx].originX;
        spawned->originY = g_BattleSummonKotrImpactBurstOrigins[particleIdx].originY;
        spawned->originZ = g_BattleSummonKotrImpactBurstOrigins[particleIdx].originZ;
        spawned->velocityX = rand() & 0xFFF;
        spawned->velocityY = rand() & 0xFFF;
        spawned->velocityZ = rand() & 0xFFF;
    }
    if (rec->frameCounter == 0xF || rec->frameCounter == 0x14 || rec->frameCounter == 0x19) {
        spawned = (BattleFxKotrImpactBurstSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrImpactSpriteRingTask);
        spawned->velocityX = 0x200;
        spawned->velocityY = 0x400;
        spawned->velocityZ = rand() % 0x200 - 0x100;
    }
    if (rec->frameCounter == 0x14) {
        BattleFx_KotrPostKnightImpactRecords();
    }
    if (rec->frameCounter == 0x19) {
        rec->ownerSlot = BATTLE_TASK_FREE;
    }
    (rec->frameCounter)++;
}

/*
 * Selects sprite field sixteen, installs its position with BattleFx_KotrInstallBillboardMatrix at scale 0x1000 and draws
 * before checking pause. When unpaused retires a null sprite immediately or advances and retires
 * at or above its frame count before motion; surviving slots integrate position.
 */
// FUNCTION: FF7 0x0047d37b
void __cdecl BattleFx_KotrImpactSpriteParticleTask(void)
{
    BattleFxPulseOfLifeSprite *task;

    task = (BattleFxPulseOfLifeSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSummonKotrImpactSpriteParticle.impactSpriteEntry) = Gfx_ModelTableGetLoadedRecord(0x10, g_BattleSummonKotrTableCursor);
    g_BattleSummonKotrImpactSpriteParticle.summonKotrImpactSpriteResource = Gfx_ModelTableGetSpriteObject(0x10, g_BattleSummonKotrTableCursor);
    BattleFx_KotrInstallBillboardMatrix((BattleFxSourcePoint *)((int)task + 4), 0x1000, 0);
    g_BattleSummonKotrImpactSpriteParticle.summonKotrImpactSpriteFrame = task->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonKotrImpactSpriteParticle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        if ((g_BattleSummonKotrImpactSpriteParticle.impactSpriteEntry) != 0) {
            (task->frame)++;
            if (task->frame >= *(int *)((char *)*(int *)(g_BattleSummonKotrImpactSpriteParticle.impactSpriteEntry) + 4)) {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
    }
}

/*
 * Acquires retained scratch. Below count ten
 * sets descriptor flags 0x80 and term zero; counts ten through nineteen OR flag eight and ramp
 * descriptor from elapsed shifted by twelve divided by ten; later retires before drawing/pause.
 * Uses sine easing 0x3FF on total count divided by twenty to derive uniform scale with the retained
 * left-thirteen/right-twelve expression. Builds saved task rotation and additional Y rotation count
 * shifted five, concatenates them preserving translation, writes task XYZ translation, multiplies
 * g_BattleSummonKotrTransformState and scales the already composed matrix. Installs it, selects
 * black and draws resource field three into g_BattleCurrentOrderingTable plus 0x70 with shift
 * twelve. Only count advancement is pause-gated.
 */
// FUNCTION: FF7 0x0047d49d
void __cdecl BattleFx_KotrSpinningGrowingModelTask(void)
{
    int frameIndex;
    BattleFxCosmoMemoryExpandingModel *taskPtr;
    int progress;

    taskPtr = (BattleFxCosmoMemoryExpandingModel *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrSpinningGrowingModelTaskInit & 1) == 0) {
        g_BattleKotrSpinningGrowingModelTaskInit |= 1;
        g_BattleSummonKotrSpinState = (struct YasuiKotrSpinState *)(Gfx_GetPsxScratchpadPtr(0));
    }
    frameIndex = taskPtr->frame;
    if (frameIndex < 0xA) {
        g_BattleSummonKotrSpinGrowModel.summonKotrSpinGrowModelFlags = 0x80;
        g_BattleSummonKotrSpinGrowModel.summonKotrSpinGrowModelScale = 0;
    } else {
        frameIndex -= 0xA;
        if (frameIndex < 0xA) {
            progress = (frameIndex << 0xC) / 0xA;
            g_BattleSummonKotrSpinGrowModel.summonKotrSpinGrowModelFlags |= 8;
            g_BattleSummonKotrSpinGrowModel.summonKotrSpinGrowModelScale = progress;
        } else {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    progress = (taskPtr->frame << 0xC) / 0x14;
    progress = BattleFx_ApplySineEasing(progress, 0x3FF);
    g_BattleSummonKotrSpinState->scaleZ = progress << 0xD >> 0xC;
    g_BattleSummonKotrSpinState->scaleY = g_BattleSummonKotrSpinState->scaleZ;
    g_BattleSummonKotrSpinState->scaleX = g_BattleSummonKotrSpinState->scaleY;
    RotMatrix((SVECTOR *)((int)taskPtr + 0xC), &g_BattleSummonKotrSpinState->taskRotation);
    g_BattleSummonKotrSpinState->rotX = 0;
    g_BattleSummonKotrSpinState->angle = taskPtr->frame << 5;
    g_BattleSummonKotrSpinState->rotZ = 0;
    RotMatrix((SVECTOR *)g_BattleSummonKotrSpinState, &g_BattleSummonKotrSpinState->matrix);
    ConcatMatrixPreservingTranslation(&g_BattleSummonKotrSpinState->taskRotation, &g_BattleSummonKotrSpinState->matrix, &g_BattleSummonKotrSpinState->matrix);
    g_BattleSummonKotrSpinState->matrix.t[0] = taskPtr->posX;
    g_BattleSummonKotrSpinState->matrix.t[1] = taskPtr->posY;
    g_BattleSummonKotrSpinState->matrix.t[2] = taskPtr->posZ;
    MultiplyMatrix(&g_BattleSummonKotrTransformState, &g_BattleSummonKotrSpinState->matrix, &g_BattleSummonKotrSpinState->matrix);
    ScaleMatrix(&g_BattleSummonKotrSpinState->matrix, &g_BattleSummonKotrSpinState->scaleX);
    SetRotMatrix(&g_BattleSummonKotrSpinState->matrix);
    SetTransMatrix(&g_BattleSummonKotrSpinState->matrix);
    SetDrawColorRGB(0, 0, 0);
    (g_BattleSummonKotrSpinGrowModel.spinGrowModelResource) = Gfx_ModelTableGetRenderObject(3, g_BattleSummonKotrTableCursor);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonKotrSpinGrowModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frame)++;
    }
}

/*
 * Acquires retained scratch and refreshes sprite
 * resource fields sixteen. Applies sine easing 0x3FF to task count shifted by twelve divided by
 * fifteen, producing radius up to 5000, and publishes the count as sprite frame. Builds saved task
 * rotation and visits sixteen equally spaced X/Z circle points at Y zero, transforming each in
 * place. Calls BattleFx_KotrInstallBillboardMatrix with scale 0x2000 and zero, then adjusts
 * returned matrix Z relative to g_BattleSummonKotrTransformState.t[2] using the
 * identical left-fourteen/right-twelve expression in both comparison branches. For adjusted Z
 * strictly between zero and 0x8000 installs translation and submits the sprite into
 * g_BattleCurrentOrderingTable plus 0x70 with shift twelve. While unpaused advances count and
 * retires at sixteen.
 */
// FUNCTION: FF7 0x0047d726
void __cdecl BattleFx_KotrImpactSpriteRingTask(void)
{
    int ringRadius;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleTaskSlot *task;
    int easedProgress;
    int spriteIdx;
    PsxMatrix *billboardMtx;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrImpactSpriteRingTaskInit & 1) == 0) {
        g_BattleKotrImpactSpriteRingTaskInit |= 1;
        g_BattleSummonKotrBillboardPoint = (BattleKotrPointPose *)Gfx_GetPsxScratchpadPtr(0);
    }
    (g_BattleSummonKotrImpactSpriteRing.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0x10, g_BattleSummonKotrTableCursor);
    g_BattleSummonKotrImpactSpriteRing.summonKotrImpactRingResource = Gfx_ModelTableGetSpriteObject(0x10, g_BattleSummonKotrTableCursor);
    easedProgress = (task->frameCounter << 0xC) / 0xF;
    easedProgress = BattleFx_ApplySineEasing(easedProgress, 0x3FF);
    ringRadius = easedProgress * 0x1388 >> 0xC;
    g_BattleSummonKotrImpactSpriteRing.summonKotrImpactRingFieldWord = task->frameCounter;
    RotMatrix((SVECTOR *)((int)task + 0xC), &g_BattleSummonKotrBillboardPoint->matrix);
    for (spriteIdx = 0; spriteIdx < 0x10; spriteIdx++) {
        g_BattleSummonKotrBillboardPoint->point.vx = ringRadius * SinFixed((spriteIdx << 0xC) / 0x10) >> 0xC;
        g_BattleSummonKotrBillboardPoint->point.vy = 0;
        g_BattleSummonKotrBillboardPoint->point.vz = ringRadius * CosFixed((spriteIdx << 0xC) / 0x10) >> 0xC;
        TransformVectorToInt16(&g_BattleSummonKotrBillboardPoint->matrix, &g_BattleSummonKotrBillboardPoint->point, &g_BattleSummonKotrBillboardPoint->point.vx);
        billboardMtx = (PsxMatrix *)BattleFx_KotrInstallBillboardMatrix((BattleFxSourcePoint *)&g_BattleSummonKotrBillboardPoint->point, 0x2000, 0);
        if (billboardMtx->t[2] < g_BattleSummonKotrTransformState.t[2]) {
            billboardMtx->t[2] = g_BattleSummonKotrTransformState.t[2] + (billboardMtx->t[2] - g_BattleSummonKotrTransformState.t[2] << 0xE >> 0xC);
        } else {
            billboardMtx->t[2] = g_BattleSummonKotrTransformState.t[2] + (billboardMtx->t[2] - g_BattleSummonKotrTransformState.t[2] << 0xE >> 0xC);
        }
        if (billboardMtx->t[2] > 0 && billboardMtx->t[2] < 0x8000) {
            SetTransMatrix(billboardMtx);
            g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonKotrImpactSpriteRing, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
        }
    }
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= 0x10) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Acquires retained scratch and selects the knight context. At count zero calls BattleFx_KotrSpawnKnightEndpointTrail with
 * duration 0x54 and retained definition; other calls BattleFx_KotrDrawKnightModel before the pause check. Unpaused
 * count 52 calls BattleFx_KotrSpawnKnightActorBurstSprites, creates masked BattleFx_KotrSpawnKnight4SpinScaleModelPerTarget tasks, allocates BattleFx_KotrImpactScreenFlashTask and posts
 * impact records in that order. First 24 counts initialize look mode one and anchor
 * (0,-1000,10000), ease polar angle toward minus 0x800 while retaining radius 3000 through
 * explicit zero multiplication, grow scale and reduce fade. Next six counts hold that angle/radius
 * with full scale and zero fade. Final 54 counts select look mode two with anchor relative to
 * current position and ease radius from 3000 toward minus 12000 while retaining angle minus 0x800.
 * After elapsed 24 computes an additional eased fraction without storing or using it. Later
 * retires and returns. Common unpaused tail rotates the polar XY vector by Y 0xFB80, writes
 * context position around (0,-1000,10000), calls BattleFx_KotrBumpKnightContextCounter and advances count.
 */
// FUNCTION: FF7 0x0047d976
void __cdecl BattleFx_KotrKnight4TimelineTask(void)
{
    int phaseFrame;
    BattleFxKotrKnight8Record *task;
    int easedProgress;
    BattleKotrKnight *pKnight;

    task = (BattleFxKotrKnight8Record *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrKnight4TimelineTaskInit & 1) == 0) {
        g_BattleKotrKnight4TimelineTaskInit |= 1;
        g_BattleKotrKnight4State = (BattleKotrOrbitPose *)Gfx_GetPsxScratchpadPtr(0);
    }
    pKnight = task->knight;
    if (task->step != 0) {
        BattleFx_KotrDrawKnightModel(pKnight);
    } else {
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x54, &g_BattleSummonKotrKnight4SpinScaleModel.summonKotrKnight4EndpointPoint);
    }
    if (g_BattleIsPaused == 0) {
        if (task->step == 0x34) {
            BattleFx_KotrSpawnKnightActorBurstSprites(pKnight);
            Battle_CreateBitPatternCallbackTask(g_BattleKotrTargetMask, 0, 0, BattleFx_KotrSpawnKnight4SpinScaleModelPerTarget);
            Battle_AllocateResourceSlot(BattleFx_KotrImpactScreenFlashTask);
            BattleFx_KotrPostKnightImpactRecords();
        }
        phaseFrame = task->step;
        if (phaseFrame < 0x18) {
            if (phaseFrame == 0) {
                pKnight->state = 1;
                pKnight->offsetX = 0;
                pKnight->offsetY = 0xFC18;
                pKnight->offsetZ = 0x2710;
            }
            easedProgress = (phaseFrame << 0xC) / 0x18;
            easedProgress = BattleFx_ApplySineEasing(easedProgress, -0x3D0);
            g_BattleSummonKotrKnight4Radius = (easedProgress * 0 >> 0xC) + 0xBB8;
            g_BattleKotrKnight4Angle = easedProgress * -0x800 >> 0xC;
            pKnight->scale = easedProgress << 0xC >> 0xC;
            pKnight->fadeLevel = 0x1000 - easedProgress;
        } else {
            phaseFrame -= 0x18;
            if (phaseFrame < 6) {
                if (phaseFrame == 0) {
                    pKnight->scale = 0x1000;
                    pKnight->fadeLevel = 0;
                }
                g_BattleKotrKnight4Angle = 0xFFFFF800;
                g_BattleSummonKotrKnight4Radius = 0xBB8;
            } else {
                phaseFrame -= 6;
                if (phaseFrame < 0x36) {
                    if (phaseFrame == 0) {
                        pKnight->state = 2;
                        pKnight->offsetX = 0 - pKnight->posX;
                        pKnight->offsetY = -0x3E8 - pKnight->posY;
                        pKnight->offsetZ = 0x2710 - pKnight->posZ;
                    }
                    easedProgress = (phaseFrame << 0xC) / 0x36;
                    easedProgress = BattleFx_ApplySineEasing(easedProgress, -0x3D0);
                    g_BattleKotrKnight4Angle = (easedProgress * 0 >> 0xC) - 0x800;
                    g_BattleSummonKotrKnight4Radius = (easedProgress * -0x3A98 >> 0xC) + 0xBB8;
                    if (phaseFrame > 0x18) {
                        easedProgress = (phaseFrame - 0x18 << 0xC) / 0x1E;
                        easedProgress = BattleFx_ApplySineEasing(easedProgress, -0x1FF);
                    }
                } else {
                    task->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
        g_BattleKotrKnight4State->point.vx = 0;
        g_BattleKotrKnight4State->point.vy = 0xFB80;
        g_BattleKotrKnight4State->point.vz = 0;
        RotMatrix(&g_BattleKotrKnight4State->point, &g_BattleKotrKnight4State->matrix);
        g_BattleKotrKnight4State->point.vx = CosFixed(g_BattleKotrKnight4Angle) * g_BattleSummonKotrKnight4Radius >> 0xC;
        g_BattleKotrKnight4State->point.vy = SinFixed(g_BattleKotrKnight4Angle) * g_BattleSummonKotrKnight4Radius >> 0xC;
        g_BattleKotrKnight4State->point.vz = 0;
        TransformVectorToInt16(&g_BattleKotrKnight4State->matrix, &g_BattleKotrKnight4State->point, &g_BattleKotrKnight4State->point.vx);
        pKnight->posX = g_BattleKotrKnight4State->point.vx;
        pKnight->posY = g_BattleKotrKnight4State->point.vy - 0x3E8;
        pKnight->posZ = g_BattleKotrKnight4State->point.vz + 0x2710;
        BattleFx_KotrBumpKnightContextCounter(pKnight);
        (task->step)++;
    }
}

/*
 * Stores Gfx_ModelTableGetRenderObject(0xA,
 * g_BattleSummonKotrTableCursor) in g_BattleSummonKotrKnight4SpinScaleModel, builds a local position with
 * BattleFx_KotrGetActorAttachLocalPosition(actor, byte at g_BattleActorRecords[actor].targetBone, &local),
 * and passes global and position to BattleFx_KotrSpawnEasedSpinScaleModel. 0x1AEC is the stride of
 * the per-combatant record at g_BattleActorRecords[0].targetBone.
 */
// FUNCTION: FF7 0x0047dd19
void __cdecl BattleFx_KotrSpawnKnight4SpinScaleModelPerTarget(int actor)
{
    SVECTOR attachPos;

    (g_BattleSummonKotrKnight4SpinScaleModel.spinScaleModel) = Gfx_ModelTableGetRenderObject(0xA, g_BattleSummonKotrTableCursor);
    BattleFx_KotrGetActorAttachLocalPosition(actor, g_BattleActorRecords[actor].targetBone, &attachPos);
    BattleFx_KotrSpawnEasedSpinScaleModel(&g_BattleSummonKotrKnight4SpinScaleModel, &attachPos);
}

/*
 * Acquires retained scratch and selects the knight context. At count zero calls BattleFx_KotrSpawnKnightEndpointTrail with
 * duration 0x38 and retained definition; other calls BattleFx_KotrDrawKnightModel before the pause check. Unpaused
 * count 35 calls BattleFx_KotrSpawnKnightActorBurstSprites, creates masked BattleFx_KotrSpawnKnight5SpinScaleModelPerTarget tasks, allocates BattleFx_KotrImpactScreenFlashTask and posts
 * impact records in that order. First fifteen counts initialize context Y rotation 0x800, grow
 * scale using negative sine easing, reduce fade using the uneased fraction and move Y from 5000
 * toward zero using positive easing while keeping X 2000/Z 8000 through explicit zero terms. Next
 * 41 counts ease position toward (-1000,-2000,20000); on entry sets full scale/zero fade and holds
 * them for 26 counts, then eases scale down/fade up over fifteen. Later retires and returns.
 * Ordinary unpaused calls update context through BattleFx_KotrBumpKnightContextCounter and advance count.
 */
// FUNCTION: FF7 0x0047dd6a
void __cdecl BattleFx_KotrKnight5TimelineTask(void)
{
    int frameIndex;
    int padSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxKotrKnight8Record *taskPtr;
    int progress;
    int easedValue;
    BattleKotrKnight *pKnight;

    taskPtr = (BattleFxKotrKnight8Record *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrKnight5TimelineTaskInit & 1) == 0) {
        g_BattleKotrKnight5TimelineTaskInit |= 1;
        g_BattleSummonKotrKnight5Scratch = Gfx_GetPsxScratchpadPtr(0);
    }
    pKnight = taskPtr->knight;
    if (taskPtr->step != 0) {
        BattleFx_KotrDrawKnightModel(pKnight);
    } else {
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x38, &g_BattleSummonKotrKnight5SpinScaleModel.summonKotrKnight5EndpointPoint);
    }
    if (g_BattleIsPaused == 0) {
        if (taskPtr->step == 0x23) {
            BattleFx_KotrSpawnKnightActorBurstSprites(pKnight);
            Battle_CreateBitPatternCallbackTask(g_BattleKotrTargetMask, 0, 0, BattleFx_KotrSpawnKnight5SpinScaleModelPerTarget);
            Battle_AllocateResourceSlot(BattleFx_KotrImpactScreenFlashTask);
            BattleFx_KotrPostKnightImpactRecords();
        }
        frameIndex = taskPtr->step;
        if (frameIndex < 0xF) {
            if (frameIndex == 0) {
                pKnight->offsetY = 0x800;
            }
            progress = (frameIndex << 0xC) / 0xF;
            easedValue = BattleFx_ApplySineEasing(progress, -0x1FF);
            pKnight->scale = easedValue << 0xC >> 0xC;
            pKnight->fadeLevel = 0x1000 - progress;
            easedValue = BattleFx_ApplySineEasing(progress, 0x1FF);
            pKnight->posX = (easedValue * 0 >> 0xC) + 0x7D0;
            pKnight->posY = (easedValue * -0x1388 >> 0xC) + 0x1388;
            pKnight->posZ = (easedValue * 0 >> 0xC) + 0x1F40;
        } else {
            frameIndex -= 0xF;
            if (frameIndex < 0x29) {
                progress = (frameIndex << 0xC) / 0x29;
                easedValue = BattleFx_ApplySineEasing(progress, -0x1FF);
                pKnight->posX = (easedValue * -0xBB8 >> 0xC) + 0x7D0;
                pKnight->posY = easedValue * -0x7D0 >> 0xC;
                pKnight->posZ = (easedValue * 0x2EE0 >> 0xC) + 0x1F40;
                if (frameIndex < 0x1A) {
                    if (frameIndex == 0) {
                        pKnight->scale = 0x1000;
                        pKnight->fadeLevel = 0;
                    }
                } else {
                    frameIndex -= 0x1A;
                    progress = (frameIndex << 0xC) / 0xF;
                    easedValue = BattleFx_ApplySineEasing(progress, -0x1FF);
                    pKnight->scale = (easedValue * -0x1000 >> 0xC) + 0x1000;
                    pKnight->fadeLevel = easedValue;
                }
            } else {
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        BattleFx_KotrBumpKnightContextCounter(pKnight);
        (taskPtr->step)++;
    }
}

/*
 * Same shape as
 * BattleFx_KotrSpawnKnight4SpinScaleModelPerTarget with field index 5: stores
 * Gfx_ModelTableGetRenderObject(5, g_BattleSummonKotrTableCursor) in g_BattleSummonKotrKnight5SpinScaleModel,
 * builds the local position from
 * BattleFx_KotrGetActorAttachLocalPosition(actor, byte at g_BattleActorRecords[0].targetBone + actor * 0x1AEC, &local),
 * and passes both to BattleFx_KotrSpawnEasedSpinScaleModel.
 */
// FUNCTION: FF7 0x0047e00d
void __cdecl BattleFx_KotrSpawnKnight5SpinScaleModelPerTarget(int actor)
{
    SVECTOR attachPos;

    (g_BattleSummonKotrKnight5SpinScaleModel.spinScaleModel) = Gfx_ModelTableGetRenderObject(5, g_BattleSummonKotrTableCursor);
    BattleFx_KotrGetActorAttachLocalPosition(actor, g_BattleActorRecords[actor].targetBone, &attachPos);
    BattleFx_KotrSpawnEasedSpinScaleModel(&g_BattleSummonKotrKnight5SpinScaleModel, &attachPos);
}

/*
 * Acquires retained scratch and selects the knight
 * context. At count zero calls BattleFx_KotrSpawnKnightEndpointTrail with duration 0x30 and
 * retained definition; other calls BattleFx_KotrDrawKnightModel before pause. Unpaused count
 * fourteen calls BattleFx_KotrSpawnKnightActorBurstSprites, creates masked
 * BattleFx_KotrSpawnKnight6SpinScaleModelPairPerTarget tasks, allocates
 * BattleFx_KotrImpactScreenFlashTask and posts impact records in order. First seven counts only set
 * Y rotation 0x900 on entry; next 27 set full scale and zero fade on entry. Final fourteen use
 * negative sine easing to reduce scale by 0xFF0 from 4096 and increase fade; later retires and
 * returns. Every ordinary unpaused call eases total count over 48 with parameter 0x1C0, rotates a Z
 * vector ranging from minus 2000 toward 12000 by (0x100,0x100,0), writes context position with X
 * minus 1500 and Z plus 10000, calls BattleFx_KotrBumpKnightContextCounter and increments count.
 */
// FUNCTION: FF7 0x0047e05e
void __cdecl BattleFx_KotrKnight6TimelineTask(void)
{
    int frameIndex;
    int padSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxKotrKnightTimeline *taskPtr;
    int progress;
    int easedValue;
    BattleKotrKnight *pKnight;

    taskPtr = (BattleFxKotrKnightTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrKnight6TimelineTaskInit & 1) == 0) {
        g_BattleKotrKnight6TimelineTaskInit |= 1;
        g_BattleSummonKotrKnight6TimelineState = (BattleKotrOrbitPose *)Gfx_GetPsxScratchpadPtr(0);
    }
    pKnight = taskPtr->knight;
    if (taskPtr->step != 0) {
        BattleFx_KotrDrawKnightModel(pKnight);
    } else {
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x30, &g_BattleSummonKotrKnight6SpinScaleModel.summonKotrKnight6EndpointPoint);
    }
    if (g_BattleIsPaused == 0) {
        if (taskPtr->step == 0xE) {
            BattleFx_KotrSpawnKnightActorBurstSprites(pKnight);
            Battle_CreateBitPatternCallbackTask(g_BattleKotrTargetMask, 0, 0, BattleFx_KotrSpawnKnight6SpinScaleModelPairPerTarget);
            Battle_AllocateResourceSlot(BattleFx_KotrImpactScreenFlashTask);
            BattleFx_KotrPostKnightImpactRecords();
        }
        frameIndex = taskPtr->step;
        if (frameIndex < 7) {
            if (frameIndex == 0) {
                pKnight->offsetY = 0x900;
            }
        } else {
            frameIndex -= 7;
            if (frameIndex < 0x1B) {
                if (frameIndex == 0) {
                    pKnight->scale = 0x1000;
                    pKnight->fadeLevel = 0;
                }
            } else {
                frameIndex -= 0x1B;
                if (frameIndex < 0xE) {
                    progress = (frameIndex << 0xC) / 0xE;
                    easedValue = BattleFx_ApplySineEasing(progress, -0x1FF);
                    pKnight->scale = (easedValue * -0xFF0 >> 0xC) + 0x1000;
                    pKnight->fadeLevel = easedValue;
                } else {
                    taskPtr->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
        progress = (taskPtr->step << 0xC) / 0x30;
        easedValue = BattleFx_ApplySineEasing(progress, 0x1C0);
        g_BattleSummonKotrKnight6TimelineState->point.vx = 0x100;
        g_BattleSummonKotrKnight6TimelineState->point.vy = 0x100;
        g_BattleSummonKotrKnight6TimelineState->point.vz = 0;
        RotMatrix(&g_BattleSummonKotrKnight6TimelineState->point, &g_BattleSummonKotrKnight6TimelineState->matrix);
        g_BattleSummonKotrKnight6TimelineState->point.vy = 0;
        g_BattleSummonKotrKnight6TimelineState->point.vx = 0;
        g_BattleSummonKotrKnight6TimelineState->point.vz = (easedValue * 0x36B0 >> 0xC) - 0x7D0;
        TransformVectorToInt16(&g_BattleSummonKotrKnight6TimelineState->matrix, &g_BattleSummonKotrKnight6TimelineState->point, &g_BattleSummonKotrKnight6TimelineState->point.vx);
        pKnight->posX = g_BattleSummonKotrKnight6TimelineState->point.vx - 0x5DC;
        pKnight->posY = g_BattleSummonKotrKnight6TimelineState->point.vy;
        pKnight->posZ = g_BattleSummonKotrKnight6TimelineState->point.vz + 0x2710;
        BattleFx_KotrBumpKnightContextCounter(pKnight);
        (taskPtr->step)++;
    }
}

/*
 * Takes two field-0xA entries
 * from Gfx_ModelTableGetRenderObject(g_BattleSummonKotrTableCursor) into g_BattleKotrKnight6SpinScaleParams
 * and g_BattleSummonKotrKnight6SpinScaleModel, builds one local position with
 * BattleFx_KotrGetActorAttachLocalPosition(actor, byte at g_BattleActorRecords[0].targetBone + actor * 0x1AEC, &local),
 * and submits both globals at that same position through two BattleFx_KotrSpawnEasedSpinScaleModel
 * calls.
 */
// FUNCTION: FF7 0x0047e2ef
void __cdecl BattleFx_KotrSpawnKnight6SpinScaleModelPairPerTarget(int actor)
{
    SVECTOR attachPos;

    g_BattleKotrKnight6SpinScaleParams.modelGroup = Gfx_ModelTableGetRenderObject(0xA, g_BattleSummonKotrTableCursor);
    (g_BattleSummonKotrKnight6SpinScaleModel.spinScaleModel) = Gfx_ModelTableGetRenderObject(0xA, g_BattleSummonKotrTableCursor);
    BattleFx_KotrGetActorAttachLocalPosition(actor, g_BattleActorRecords[actor].targetBone, &attachPos);
    BattleFx_KotrSpawnEasedSpinScaleModel(&g_BattleKotrKnight6SpinScaleParams, &attachPos);
    BattleFx_KotrSpawnEasedSpinScaleModel(&g_BattleSummonKotrKnight6SpinScaleModel, &attachPos);
}

/*
 * Acquires retained scratch and selects the knight
 * context. At count zero calls BattleFx_KotrSpawnKnightEndpointTrail twice with duration 0x47 and
 * retained definitions; later calls BattleFx_KotrDrawKnightModel before pause. Unpaused count
 * thirty allocates BattleFx_KotrShrinkExpandModelsTimelineTask. First fifteen counts initialize
 * look mode one and anchor (0,0,15000), ease polar angle toward 0x600 with radius 5000, change Z
 * rotation using the uneased fraction and grow scale from eight while reducing fade. Next 31 hold
 * angle/radius, full scale, zero fade and zero Z rotation on entry; after elapsed fifteen advance
 * Y/X rotations linearly. Final 25 keep angle 0x600 through explicit zero multiplication, ease
 * radius toward 10000 with parameter 0x2FF and independently reduce scale toward eight/increase
 * fade with negative easing. Later retires and returns. Common unpaused tail rotates the polar XY
 * vector using g_BattleSummonKotrKnight7ExpandingModel.summonKotrKnight7RotationMatrix with scale
 * (4096,2048,4096), transforms in place, adds 15000 to Z, calls
 * BattleFx_KotrBumpKnightContextCounter and increments count.
 */
// FUNCTION: FF7 0x0047e367
void __cdecl BattleFx_KotrKnight7TimelineTask(void)
{
    int theta;
    int polarRadius;
    int phaseFrame;
    BattleFxKotrKnight8Record *task;
    int ratio;
    int easedValue;
    BattleKotrKnight *pKnight;

    task = (BattleFxKotrKnight8Record *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrKnight7TimelineTaskInit & 1) == 0) {
        g_BattleKotrKnight7TimelineTaskInit |= 1;
        g_BattleKotrKnight7State = (struct BattleFxKotrKnight7State *)(Gfx_GetPsxScratchpadPtr(0));
    }
    pKnight = task->knight;
    if (task->step != 0) {
        BattleFx_KotrDrawKnightModel(pKnight);
    } else {
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x47, &g_BattleSummonKotrKnight7ExpandingModel.summonKotrKnight7EndpointPointA);
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x47, &g_BattleSummonKotrKnight7ExpandingModel.summonKotrKnight7EndpointPointB);
    }
    if (g_BattleIsPaused == 0) {
        if (task->step == 0x1E) {
            Battle_AllocateResourceSlot(BattleFx_KotrShrinkExpandModelsTimelineTask);
        }
        phaseFrame = task->step;
        if (phaseFrame < 0xF) {
            if (phaseFrame == 0) {
                pKnight->state = 1;
                pKnight->offsetX = 0;
                pKnight->offsetY = 0;
                pKnight->offsetZ = 0x3A98;
            }
            ratio = (phaseFrame << 0xC) / 0xF;
            easedValue = BattleFx_ApplySineEasing(ratio, -0x1FF);
            theta = easedValue * 0x600 >> 0xC;
            polarRadius = (easedValue * 0 >> 0xC) + 0x1388;
            pKnight->rotZ = (ratio << 0xA >> 0xC) - 0x400;
            easedValue = BattleFx_ApplySineEasing(ratio, -0x1FF);
            pKnight->scale = (easedValue * 0xFF8 >> 0xC) + 8;
            pKnight->fadeLevel = 0x1000 - easedValue;
        } else {
            phaseFrame -= 0xF;
            if (phaseFrame < 0x1F) {
                if (phaseFrame == 0) {
                    pKnight->scale = 0x1000;
                    pKnight->fadeLevel = 0;
                    pKnight->rotZ = 0;
                }
                if (phaseFrame > 0xF) {
                    ratio = ((phaseFrame - 0xF) << 0xC) / 0x10;
                    pKnight->rotY = ratio << 0xB >> 0xC;
                    pKnight->rotX = ratio * 0x300 >> 0xC;
                }
                theta = 0x600;
                polarRadius = 0x1388;
            } else {
                phaseFrame -= 0x1F;
                if (phaseFrame < 0x19) {
                    ratio = (phaseFrame << 0xC) / 0x19;
                    easedValue = BattleFx_ApplySineEasing(ratio, 0x2FF);
                    theta = (easedValue * 0 >> 0xC) + 0x600;
                    polarRadius = (easedValue * 0x1388 >> 0xC) + 0x1388;
                    easedValue = BattleFx_ApplySineEasing(ratio, -0x1FF);
                    pKnight->scale = (easedValue * -0xFF8 >> 0xC) + 0x1000;
                    pKnight->fadeLevel = easedValue;
                } else {
                    task->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
        RotMatrix((SVECTOR *)&g_BattleSummonKotrKnight7ExpandingModel.summonKotrKnight7RotationMatrix, &g_BattleKotrKnight7State->matrix);
        g_BattleKotrKnight7State->posX = CosFixed(theta) * polarRadius >> 0xC;
        g_BattleKotrKnight7State->posY = SinFixed(theta) * polarRadius >> 0xC;
        g_BattleKotrKnight7State->posZ = 0;
        g_BattleKotrKnight7State->scaleZ = 0x1000;
        g_BattleKotrKnight7State->scaleX = 0x1000;
        g_BattleKotrKnight7State->scaleY = 0x800;
        ScaleMatrix(&g_BattleKotrKnight7State->matrix, &g_BattleKotrKnight7State->scaleX);
        TransformVectorToInt16(&g_BattleKotrKnight7State->matrix, (SVECTOR *)g_BattleKotrKnight7State, &g_BattleKotrKnight7State->posX);
        pKnight->posX = g_BattleKotrKnight7State->posX;
        pKnight->posY = g_BattleKotrKnight7State->posY;
        pKnight->posZ = g_BattleKotrKnight7State->posZ + 0x3A98;
        BattleFx_KotrBumpKnightContextCounter(pKnight);
        (task->step)++;
    }
}

/*
 * When unpaused allocates BattleFx_KotrKnight7ShrinkingBillboardModelTask at counter zero and BattleFx_KotrExpandingFadingModelTask at ten with three rotation
 * shorts cleared. At twenty posts knight impact records and retires the parent, then advances its
 * counter.
 */
// FUNCTION: FF7 0x0047e725
void __cdecl BattleFx_KotrShrinkExpandModelsTimelineTask(void)
{
    BattleTaskSlot *task;
    BattleFxModelTimeline *spawned;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        spawned = (BattleFxModelTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrKnight7ShrinkingBillboardModelTask);
    }
    if (task->frameCounter == 0xA) {
        spawned = (BattleFxModelTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrExpandingFadingModelTask);
        spawned->rotZ = 0;
        spawned->rotY = 0;
        spawned->rotX = 0;
    }
    if (task->frameCounter == 0x14) {
        BattleFx_KotrPostKnightImpactRecords();
    }
    if (task->frameCounter == 0x14) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Derives a shrinking scale with sine easing for counters below ten and a second linear shrinking
 * phase for ten through nineteen, retiring at twenty or above before drawing even while paused.
 * Installs g_BattleSummonKotrKnight7BillboardOrigin through BattleFx_KotrInstallBillboardMatrix with that scale and third argument -1000, selects
 * model field one and submits it; increments its counter only when unpaused.
 */
// FUNCTION: FF7 0x0047e7f3
void __cdecl BattleFx_KotrKnight7ShrinkingBillboardModelTask(void)
{
    int scale;
    BattleFxDualCounterTask *task;
    int ctr;
    int easedValue;

    task = (BattleFxDualCounterTask *)BATTLE_CURRENT_TASK_SLOT;
    ctr = task->nextIndex;
    if (ctr < 0xA) {
        easedValue = (ctr << 0xC) / 0xA;
        easedValue = BattleFx_ApplySineEasing(easedValue, 0x1FF);
        scale = (easedValue * -0x2F00 >> 0xC) + 0x3000;
    } else {
        ctr -= 0xA;
        if (ctr < 0xA) {
            easedValue = (ctr << 0xC) / 0xA;
            easedValue = BattleFx_ApplySineEasing(easedValue, 0);
            scale = (easedValue * -0x100 >> 0xC) + 0x100;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    BattleFx_KotrInstallBillboardMatrix((BattleFxSourcePoint *)&g_BattleSummonKotrKnight7BillboardOrigin, (short)scale, 0xFFFFFC18);
    g_BattleKotrKnight7BillboardDraw.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSummonKotrTableCursor);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleKotrKnight7BillboardDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->nextIndex)++;
    }
}

/*
 * Acquires retained scratch. Counts at most one clear descriptor flag eight and term zero. Below
 * ten draws without further descriptor change; counts ten through nineteen set flag eight on entry
 * and ramp term from elapsed shifted by twelve divided by ten; later retire before draw/pause.
 * Selects resource field eight when flag eight is set or seven otherwise. Sine-eases total count
 * divided by twenty with parameter 0x2F0 to derive uniform scale up to 0x1400, builds saved task
 * rotation with zero translation, scales and multiplies g_BattleSummonKotrTransformState, installs
 * it and draws black into g_BattleCurrentOrderingTable plus 0x70 with shift twelve. While
 * unpaused retains three rotation self-assignments and advances count.
 */
// FUNCTION: FF7 0x0047e919
void __cdecl BattleFx_KotrExpandingFadingModelTask(void)
{
    int elapsed;
    BattleFxKotrExpandingModel *taskPtr;
    int ramp;

    taskPtr = (BattleFxKotrExpandingModel *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrExpandingFadingModelTaskInit & 1) == 0) {
        g_BattleKotrExpandingFadingModelTaskInit |= 1;
        g_BattleKotrExpandState = (BattleFxKotrFragmentScale *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (taskPtr->frame <= 1) {
        g_BattleSummonKotrKnight7ExpandingModel.summonKotrExpandFadeFlags &= 0xFFFFFFF7;
        g_BattleSummonKotrKnight7ExpandingModel.summonKotrExpandFadeAlpha = 0;
    }
    elapsed = taskPtr->frame;
    if (elapsed < 0xA) { /* empty first arm: lib/codegen.h */
    } else {
        elapsed -= 0xA;
        if (elapsed < 0xA) {
            if (elapsed == 0) {
                g_BattleSummonKotrKnight7ExpandingModel.summonKotrExpandFadeFlags |= 8;
            }
            ramp = (elapsed << 0xC) / 0xA;
            g_BattleSummonKotrKnight7ExpandingModel.summonKotrExpandFadeAlpha = ramp;
        } else {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    if ((g_BattleSummonKotrKnight7ExpandingModel.summonKotrExpandFadeFlags & 8) != 0) {
        (g_BattleSummonKotrKnight7ExpandingModel.expandingModelResource) = Gfx_ModelTableGetRenderObject(8, g_BattleSummonKotrTableCursor);
    } else {
        (g_BattleSummonKotrKnight7ExpandingModel.expandingModelResource) = Gfx_ModelTableGetRenderObject(7, g_BattleSummonKotrTableCursor);
    }
    ramp = (taskPtr->frame << 0xC) / 0x14;
    ramp = BattleFx_ApplySineEasing(ramp, 0x2F0);
    g_BattleKotrExpandState->scaleZ = ramp * 0x1400 >> 0xC;
    g_BattleKotrExpandState->scaleY = g_BattleKotrExpandState->scaleZ;
    g_BattleKotrExpandState->scaleX = g_BattleKotrExpandState->scaleY;
    RotMatrix((SVECTOR *)((int)taskPtr + 0xC), &g_BattleKotrExpandState->matrix);
    g_BattleKotrExpandState->matrix.t[2] = 0;
    g_BattleKotrExpandState->matrix.t[1] = 0;
    g_BattleKotrExpandState->matrix.t[0] = 0;
    ScaleMatrix(&g_BattleKotrExpandState->matrix, g_BattleKotrExpandState);
    MultiplyMatrix(&g_BattleSummonKotrTransformState, &g_BattleKotrExpandState->matrix, &g_BattleKotrExpandState->matrix);
    SetRotMatrix(&g_BattleKotrExpandState->matrix);
    SetTransMatrix(&g_BattleKotrExpandState->matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonKotrKnight7ExpandingModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        taskPtr->rotX = taskPtr->rotX;
        taskPtr->rotY = taskPtr->rotY;
        taskPtr->rotZ = taskPtr->rotZ;
        (taskPtr->frame)++;
    }
}

/*
 * Acquires retained scratch and selects the knight context. At count zero sets context bit one;
 * other calls BattleFx_KotrDrawKnightModel before pause. At count five calls BattleFx_KotrSpawnKnightEndpointTrail twice with duration 0x2E
 * and retained definitions, repeating those calls while paused at that count. Unpaused count 26
 * calls BattleFx_KotrSpawnKnightActorBurstSprites, allocates BattleFx_KotrKnight8SpinScaleModelSetupTask and BattleFx_KotrImpactScreenFlashTask, then posts impact records. First 37
 * counts ease a Z displacement from 10000 toward minus 5000; first twelve initialize X rotation
 * 0x200 and grow scale/reduce fade, then set full scale/zero fade on entry to the remaining 25.
 * Final fourteen independently ease scale down/fade up and displacement from minus 5000 toward
 * minus 15000. Later retires and returns. Common unpaused tail rotates the Z vector using
 * g_BattleSummonKotrKnight8Rotation in place, writes context position with Y minus 500 and Z plus
 * 10000, calls BattleFx_KotrBumpKnightContextCounter and advances count.
 */
// FUNCTION: FF7 0x0047eb92
void __cdecl BattleFx_KotrKnight8TimelineTask(void)
{
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int zDisplacement;
    int phaseFrame;
    BattleFxKotrKnight8Record *task;
    int ratio;
    int easedValue;
    BattleKotrKnight *pKnight;

    task = (BattleFxKotrKnight8Record *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrKnight8TimelineTaskInit & 1) == 0) {
        g_BattleKotrKnight8TimelineTaskInit |= 1;
        g_BattleSummonKotrKnightState = (BattleKotrPointPose *)Gfx_GetPsxScratchpadPtr(0);
    }
    pKnight = task->knight;
    if (task->step != 0) {
        BattleFx_KotrDrawKnightModel(pKnight);
    } else {
        pKnight->flags |= 1;
    }
    if (task->step == 5) {
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x2E, &g_BattleSummonKotrKnight8SpinScaleModel.summonKotrKnight8EndpointPointA);
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x2E, &g_BattleSummonKotrKnight8SpinScaleModel.summonKotrKnight8EndpointPointB);
    }
    if (g_BattleIsPaused == 0) {
        if (task->step == 0x1A) {
            BattleFx_KotrSpawnKnightActorBurstSprites(pKnight);
            Battle_AllocateResourceSlot(BattleFx_KotrKnight8SpinScaleModelSetupTask);
            Battle_AllocateResourceSlot(BattleFx_KotrImpactScreenFlashTask);
            BattleFx_KotrPostKnightImpactRecords();
        }
        phaseFrame = task->step;
        if (phaseFrame < 0x25) {
            ratio = (task->step << 0xC) / 0x25;
            easedValue = BattleFx_ApplySineEasing(ratio, -0x3D0);
            zDisplacement = (easedValue * -0x3A98 >> 0xC) + 0x2710;
            if (phaseFrame < 0xC) {
                if (phaseFrame == 0) {
                    pKnight->offsetX = 0x200;
                }
                ratio = (phaseFrame << 0xC) / 0xC;
                easedValue = BattleFx_ApplySineEasing(ratio, -0x1FF);
                pKnight->scale = easedValue << 0xC >> 0xC;
                pKnight->fadeLevel = 0x1000 - easedValue;
            } else {
                phaseFrame -= 0xC;
                if (phaseFrame < 0x19 && phaseFrame == 0) {
                    pKnight->scale = 0x1000;
                    pKnight->fadeLevel = 0;
                }
            }
        } else {
            phaseFrame -= 0x25;
            if (phaseFrame < 0xE) {
                ratio = (phaseFrame << 0xC) / 0xE;
                easedValue = BattleFx_ApplySineEasing(ratio, -0x1FF);
                pKnight->scale = (easedValue * -0x1000 >> 0xC) + 0x1000;
                pKnight->fadeLevel = easedValue;
                easedValue = BattleFx_ApplySineEasing(ratio, 0x3FF);
                zDisplacement = (easedValue * -0x2710 >> 0xC) - 0x1388;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        RotMatrix((SVECTOR *)&g_BattleSummonKotrKnight8Rotation, &g_BattleSummonKotrKnightState->matrix);
        g_BattleSummonKotrKnightState->point.vx = 0;
        g_BattleSummonKotrKnightState->point.vy = 0;
        g_BattleSummonKotrKnightState->point.vz = zDisplacement;
        TransformVectorToInt16(&g_BattleSummonKotrKnightState->matrix, &g_BattleSummonKotrKnightState->point, &g_BattleSummonKotrKnightState->point.vx);
        pKnight->posX = g_BattleSummonKotrKnightState->point.vx;
        pKnight->posY = g_BattleSummonKotrKnightState->point.vy - 0x1F4;
        pKnight->posZ = g_BattleSummonKotrKnightState->point.vz + 0x2710;
        BattleFx_KotrBumpKnightContextCounter(pKnight);
        (task->step)++;
    }
}

/*
 * When unpaused at counter zero records
 * the transformed bone position for every selected low-ten-bit actor into the stride-eight table at
 * g_BattleSummonKotrAttachLocalPositions. Creates a mode-zero bit-pattern task for
 * BattleFx_KotrSpawnKnight8SpinScaleModelPerTarget using g_BattleKotrTargetMask, advances its
 * counter and retires at one or above.
 */
// FUNCTION: FF7 0x0047ee9e
void __cdecl BattleFx_KotrKnight8SpinScaleModelSetupTask(void)
{
    BattleTaskSlot *taskPtr;
    int actorIndex;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frameCounter == 0) {
        for (actorIndex = 0; actorIndex < 0xA; actorIndex++) {
            if ((g_BattleKotrTargetMask & 1 << actorIndex) != 0) {
                BattleFx_KotrGetActorAttachLocalPosition(actorIndex, g_BattleActorRecords[actorIndex].targetBone, (actorIndex * 8 + (char *)&g_BattleSummonKotrAttachLocalPositions));
            }
        }
    }
    Battle_CreateBitPatternCallbackTask(g_BattleKotrTargetMask, 0, 0, BattleFx_KotrSpawnKnight8SpinScaleModelPerTarget);
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter >= 1) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Stores Gfx_ModelTableGetRenderObject(4,
 * g_BattleSummonKotrTableCursor) in g_BattleSummonKotrKnight8SpinScaleModel and passes it to
 * BattleFx_KotrSpawnEasedSpinScaleModel with the 8-byte record at
 * g_BattleSummonKotrAttachLocalPositions + actor * 8. Same shape as
 * BattleFx_KotrSpawnKnight1SpinScaleModelPerTarget and
 * BattleFx_KotrSpawnKnight2SpinScaleModelPerTarget.
 */
// FUNCTION: FF7 0x0047ef6e
void __cdecl BattleFx_KotrSpawnKnight8SpinScaleModelPerTarget(int actor)
{
    (g_BattleSummonKotrKnight8SpinScaleModel.spinScaleModel) = Gfx_ModelTableGetRenderObject(4, g_BattleSummonKotrTableCursor);
    BattleFx_KotrSpawnEasedSpinScaleModel(&g_BattleSummonKotrKnight8SpinScaleModel, (actor * 8 + (char *)&g_BattleSummonKotrAttachLocalPositions));
}

/*
 * Acquires retained scratch and selects the knight
 * context. At count zero calls BattleFx_KotrSpawnKnightEndpointTrail twice with duration 0x5F and
 * retained definitions; other calls BattleFx_KotrDrawKnightModel before pause. Unpaused count 71
 * allocates BattleFx_KotrSpawnTumblingModelsAndPulsesTimelineTask and fills its position through
 * BattleFx_KotrTransformAttachPointToLocal with bone 0x1E. First forty counts initialize look mode
 * one and anchor (0,0,20000), ease polar angle from 0x300 toward 0x680 and radius from 5000 toward
 * 12000, independently growing scale from eight/reducing fade. Next forty hold angle/radius and set
 * full scale/zero fade on entry. Final fifteen hold angle through explicit zero multiplication and
 * ease radius toward 20000 without changing scale/fade; later retire and return. Common unpaused
 * tail rotates the polar XY vector using
 * g_BattleSummonKotrKnight9BillboardModel.kotrKnight9RotMatrix with scale (4096,2048,4096),
 * transforms in place and writes position with Z plus 20000. Sets Z rotation 0x800 when narrowed
 * resulting Z exceeds 20000, otherwise zero, calls BattleFx_KotrBumpKnightContextCounter and
 * advances count.
 */
// FUNCTION: FF7 0x0047efa0
void __cdecl BattleFx_KotrKnight9TimelineTask(void)
{
    int theta;
    int polarRadius;
    int phaseFrame;
    BattleFxKotrKnight8Record *task;
    int ratio;
    int easedValue;
    BattleKotrKnight *pKnight;
    int newTask;

    task = (BattleFxKotrKnight8Record *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrKnight9TimelineTaskInit & 1) == 0) {
        g_BattleKotrKnight9TimelineTaskInit |= 1;
        g_BattleKotrKnight9State = (struct BattleFxKotrKnight7State *)(Gfx_GetPsxScratchpadPtr(0));
    }
    pKnight = task->knight;
    if (task->step != 0) {
        BattleFx_KotrDrawKnightModel(pKnight);
    } else {
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x5F, &g_BattleSummonKotrKnight9BillboardModel.kotrKnight9TrailEndpoint);
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x5F, &g_BattleSummonKotrKnight9BillboardModel.kotrKnight9TrailEndpointB);
    }
    if (g_BattleIsPaused == 0) {
        if (task->step == 0x47) {
            newTask = (int)BATTLE_NEW_TASK_SLOT(BattleFx_KotrSpawnTumblingModelsAndPulsesTimelineTask);
            BattleFx_KotrTransformAttachPointToLocal(0x1E, (SVECTOR *)(newTask + 4));
        }
        phaseFrame = task->step;
        if (phaseFrame < 0x28) {
            if (phaseFrame == 0) {
                pKnight->state = 1;
                pKnight->offsetX = 0;
                pKnight->offsetY = 0;
                pKnight->offsetZ = 0x4E20;
            }
            ratio = (phaseFrame << 0xC) / 0x28;
            easedValue = BattleFx_ApplySineEasing(ratio, -0x3D0);
            theta = (easedValue * 0x380 >> 0xC) + 0x300;
            polarRadius = (easedValue * 0x1B58 >> 0xC) + 0x1388;
            easedValue = BattleFx_ApplySineEasing(ratio, -0x1FF);
            pKnight->scale = (easedValue * 0xFF8 >> 0xC) + 8;
            pKnight->fadeLevel = 0x1000 - easedValue;
        } else {
            phaseFrame -= 0x28;
            if (phaseFrame < 0x28) {
                if (phaseFrame == 0) {
                    pKnight->scale = 0x1000;
                    pKnight->fadeLevel = 0;
                }
                theta = 0x680;
                polarRadius = 0x2EE0;
            } else {
                phaseFrame -= 0x28;
                if (phaseFrame < 0xF) {
                    ratio = (phaseFrame << 0xC) / 0xF;
                    easedValue = BattleFx_ApplySineEasing(ratio, 0x2FF);
                    theta = (easedValue * 0 >> 0xC) + 0x680;
                    polarRadius = (easedValue * 0x1F40 >> 0xC) + 0x2EE0;
                } else {
                    task->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
        RotMatrix((SVECTOR *)&g_BattleSummonKotrKnight9BillboardModel.kotrKnight9RotMatrix, &g_BattleKotrKnight9State->matrix);
        g_BattleKotrKnight9State->posX = CosFixed(theta) * polarRadius >> 0xC;
        g_BattleKotrKnight9State->posY = SinFixed(theta) * polarRadius >> 0xC;
        g_BattleKotrKnight9State->posZ = 0;
        g_BattleKotrKnight9State->scaleZ = 0x1000;
        g_BattleKotrKnight9State->scaleX = 0x1000;
        g_BattleKotrKnight9State->scaleY = 0x800;
        ScaleMatrix(&g_BattleKotrKnight9State->matrix, &g_BattleKotrKnight9State->scaleX);
        TransformVectorToInt16(&g_BattleKotrKnight9State->matrix, (SVECTOR *)g_BattleKotrKnight9State, &g_BattleKotrKnight9State->posX);
        pKnight->posX = g_BattleKotrKnight9State->posX;
        pKnight->posY = g_BattleKotrKnight9State->posY;
        pKnight->posZ = g_BattleKotrKnight9State->posZ + 0x4E20;
        if (pKnight->posZ > 0x4E20) {
            pKnight->rotZ = 0x800;
        } else {
            pKnight->rotZ = 0;
        }
        BattleFx_KotrBumpKnightContextCounter(pKnight);
        (task->step)++;
    }
}

/*
 * Returns while paused. At count
 * zero allocates BattleFx_KotrTumblingShrinkingModelEmitterTask at twice the task XYZ using
 * retained left-thirteen/right-twelve expressions and sets velocity to each narrowed negative
 * position divided by thirty. Counts thirty through 39 compute emission term ((elapsed shifted
 * seven) AND 0xFF) plus 0x80 and subtract 0x100 per BattleFx_KotrKnight9PulsingBillboardModelTask
 * record, discarding remainder; this emits one on odd elapsed counts and none on even ones. Each
 * gets random X/Z -3000 through 2999, Y zero and random size 0x800 through 0xFFF. Count thirty
 * independently posts impact records; count 35 retires, then the counter still increments.
 */
// FUNCTION: FF7 0x0047f310
void __cdecl BattleFx_KotrSpawnTumblingModelsAndPulsesTimelineTask(void)
{
    int elapsed;
    BattleFxKotrTumblingTimeline *task;
    BattleFxKotrTumblingSpawnRecord *pSpawn;
    int emission;
    int bytes;

    task = (BattleFxKotrTumblingTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->step == 0) {
        pSpawn = (BattleFxKotrTumblingSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrTumblingShrinkingModelEmitterTask);
        pSpawn->posA = task->posA << 0xD >> 0xC;
        pSpawn->posB = task->posB << 0xD >> 0xC;
        pSpawn->posC = task->posC << 0xD >> 0xC;
        pSpawn->velA = -pSpawn->posA / 0x1E;
        pSpawn->velB = -pSpawn->posB / 0x1E;
        pSpawn->velC = -pSpawn->posC / 0x1E;
    }
    if (task->step >= 0x1E && task->step < 0x28) {
        elapsed = task->step - 0x1E;
        emission = (elapsed << 7 & 0xFF) + 0x80;
        while (emission >= 0x100) {
            pSpawn = (BattleFxKotrTumblingSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrKnight9PulsingBillboardModelTask);
            pSpawn->posA = rand() % 0x1770 - 0xBB8;
            pSpawn->posB = 0;
            pSpawn->posC = rand() % 0x1770 - 0xBB8;
            if (DEAD_IN_RETAIL != 0) {
                bytes = 0x1000;
            } else {
                bytes = rand() % 0x800 + 0x800;
            }
            pSpawn->amplitude = bytes;
            emission -= 0x100;
        }
    }
    if (task->step == 0x1E) {
        BattleFx_KotrPostKnightImpactRecords();
    }
    if (task->step == 0x23) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->step)++;
}

/*
 * Acquires retained scratch before the
 * pause return. Unpaused emission term is task count shifted eight AND 0xFF plus 0x100, yielding
 * one BattleFx_KotrTumblingShrinkingModelTask record per call. Places it at retained source XYZ
 * with random X/Z -4000 through 3999, then constructs narrowed target-minus-source XYZ and scales
 * velocity by random 0x1400 through 0x17FF shifted twelve. Saves random low-24-bit colour, three
 * random bytes -2 through one and size 0x2000 through 0x2FFF. Advances count and retires at ten.
 * The current source evaluates local p times zero in X/Z direction before assigning p.
 */
// FUNCTION: FF7 0x0047f50f
void __cdecl BattleFx_KotrTumblingShrinkingModelEmitterTask(void)
{
    int speedScale;
    BattleFxKotrShrinkEmitter *pNewTask;
    int emitAccum;
    BattleTaskSlot *task;
    int pace;
    int bytes;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonKotrTumbleEmitterInit.flags & 1) == 0) {
        g_BattleSummonKotrTumbleEmitterInit.flags |= 1;
        g_BattleKotrEmitterState = (YasuiKotrEmitterState *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    emitAccum = (task->frameCounter << 8 & 0xFF) + 0x100;
    while (emitAccum >= 0x100) {
        pNewTask = (BattleFxKotrShrinkEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrTumblingShrinkingModelTask);
        g_BattleKotrEmitterState->jitterX = rand() % 0x1F40 - 0xFA0;
        g_BattleKotrEmitterState->driftY = 0;
        g_BattleKotrEmitterState->jitterZ = rand() % 0x1F40 - 0xFA0;
        pNewTask->posX = g_BattleSummonKotrTumblingModel.summonKotrTumbleShrinkBaseX + g_BattleKotrEmitterState->jitterX;
        pNewTask->posY = g_BattleSummonKotrTumblingModel.kotrTumbleBasePosY;
        pNewTask->posZ = g_BattleSummonKotrTumblingModel.kotrTumbleBasePosZ + g_BattleKotrEmitterState->jitterZ;
        g_BattleKotrEmitterState->jitterX = (g_BattleSummonKotrTumbleXY.kotrTumbleStateX) - g_BattleSummonKotrTumblingModel.summonKotrTumbleShrinkBaseX + (speedScale * 0 >> 0xC);
        g_BattleKotrEmitterState->driftY = g_BattleSummonKotrTumbleXY.kotrTumbleStateY - g_BattleSummonKotrTumblingModel.kotrTumbleBasePosY;
        g_BattleKotrEmitterState->jitterZ = g_BattleSummonKotrTumbleStateZ - g_BattleSummonKotrTumblingModel.kotrTumbleBasePosZ + (speedScale * 0 >> 0xC);
        if (DEAD_IN_RETAIL != 0) {
            pace = 0x1800;
        } else {
            pace = rand() % 0x400 + 0x1400;
        }
        speedScale = pace;
        pNewTask->offsetX = g_BattleKotrEmitterState->jitterX * speedScale >> 0xC;
        pNewTask->offsetY = g_BattleKotrEmitterState->driftY * speedScale >> 0xC;
        pNewTask->offsetZ = g_BattleKotrEmitterState->jitterZ * speedScale >> 0xC;
        pNewTask->packedAngles = rand() & 0xFFFFFF;
        pNewTask->angleStepX = rand() % 4 - 2;
        pNewTask->angleStepY = rand() % 4 - 2;
        pNewTask->angleStepZ = rand() % 4 - 2;
        if (DEAD_IN_RETAIL != 0) {
            bytes = 0x3000;
        } else {
            bytes = rand() % 0x1000 + 0x2000;
        }
        pNewTask->startSize = bytes;
        emitAccum -= 0x100;
    }
    (task->frameCounter)++;
    if (task->frameCounter >= 0xA) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Acquires retained scratch and derives a fraction from task count shifted twelve divided by
 * thirty. Uses sine easing 0x1E0 to place matrix translation at saved XYZ plus saved delta times
 * fraction and uses easing 0xFFFFFE20 to shrink uniform saved size toward zero. Builds rotation
 * from three unsigned bytes at 0x14 through 0x16 shifted four, scales, multiplies g_BattleSummonKotrTransformState and
 * installs the matrix. Refreshes resource field nine and submits into g_BattleCurrentOrderingTable plus 0x70 with
 * shift twelve, retaining prior draw colour state. While unpaused advances count and retires at
 * thirty; otherwise adds saved signed-byte increments to those three rotation bytes.
 */
// FUNCTION: FF7 0x0047f7c7
void __cdecl BattleFx_KotrTumblingShrinkingModelTask(void)
{
    BattleFxKotrTumbleModel *taskPtr;
    int normalized;
    int easedValue;

    taskPtr = (BattleFxKotrTumbleModel *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrTumblingShrinkingModelTaskInit & 1) == 0) {
        g_BattleKotrTumblingShrinkingModelTaskInit |= 1;
        g_BattleSummonKotrTumbleShrinkState = (BattleFxKotrTumbleShrinkState *)Gfx_GetPsxScratchpadPtr(0);
    }
    normalized = (taskPtr->age << 0xC) / 0x1E;
    easedValue = BattleFx_ApplySineEasing(normalized, 0x1E0);
    g_BattleSummonKotrTumbleShrinkState->angleX = taskPtr->angleX << 4;
    g_BattleSummonKotrTumbleShrinkState->angleY = taskPtr->angleY << 4;
    g_BattleSummonKotrTumbleShrinkState->angleZ = taskPtr->angleZ << 4;
    RotMatrix((SVECTOR *)g_BattleSummonKotrTumbleShrinkState, &g_BattleSummonKotrTumbleShrinkState->matrix);
    g_BattleSummonKotrTumbleShrinkState->matrix.t[0] = taskPtr->startX + (taskPtr->deltaX * easedValue >> 0xC);
    g_BattleSummonKotrTumbleShrinkState->matrix.t[1] = taskPtr->startY + (taskPtr->deltaY * easedValue >> 0xC);
    g_BattleSummonKotrTumbleShrinkState->matrix.t[2] = taskPtr->startZ + (taskPtr->deltaZ * easedValue >> 0xC);
    easedValue = BattleFx_ApplySineEasing(normalized, 0xFFFFFE20);
    g_BattleSummonKotrTumbleShrinkState->size = taskPtr->startSize * ((easedValue * -0x1000 >> 0xC) + 0x1000) >> 0xC;
    g_BattleSummonKotrTumbleShrinkState->valuePrev = g_BattleSummonKotrTumbleShrinkState->size;
    g_BattleSummonKotrTumbleShrinkState->valueOld = g_BattleSummonKotrTumbleShrinkState->valuePrev;
    ScaleMatrix(&g_BattleSummonKotrTumbleShrinkState->matrix, &g_BattleSummonKotrTumbleShrinkState->valueOld);
    MultiplyMatrix(&g_BattleSummonKotrTransformState, &g_BattleSummonKotrTumbleShrinkState->matrix, &g_BattleSummonKotrTumbleShrinkState->matrix);
    SetRotMatrix(&g_BattleSummonKotrTumbleShrinkState->matrix);
    SetTransMatrix(&g_BattleSummonKotrTumbleShrinkState->matrix);
    (g_BattleSummonKotrTumblingModel.tumblingModel) = Gfx_ModelTableGetRenderObject(9, g_BattleSummonKotrTableCursor);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonKotrTumblingModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->age)++;
        if (taskPtr->age >= 0x1E) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        } else {
            *(char *)&taskPtr->angleX += taskPtr->angleStepX;
            *(char *)&taskPtr->angleY += taskPtr->angleStepY;
            *(char *)&taskPtr->angleZ += taskPtr->angleStepZ;
        }
    }
}

/*
 * Derives scale from slot +0x1C with sine easing over a five-counter rise and five-counter fall,
 * retiring at ten or above before drawing even while paused. Installs the slot position through
 * BattleFx_KotrInstallBillboardMatrix with that scale and third argument -500, selects model field one and draws, then
 * increments its counter only when unpaused.
 */
// FUNCTION: FF7 0x0047fa51
void __cdecl BattleFx_KotrKnight9PulsingBillboardModelTask(void)
{
    int scale;
    BattleFxKotrPulseBillboardTask *task;
    int ctr;
    int easedValue;

    task = (BattleFxKotrPulseBillboardTask *)BATTLE_CURRENT_TASK_SLOT;
    ctr = task->frameCount;
    if (ctr < 5) {
        easedValue = (ctr << 0xC) / 5;
        easedValue = BattleFx_ApplySineEasing(easedValue, 0x1FF);
        scale = task->amplitude * easedValue >> 0xC;
    } else {
        ctr -= 5;
        if (ctr < 5) {
            easedValue = (ctr << 0xC) / 5;
            easedValue = BattleFx_ApplySineEasing(easedValue, 0xFFFFFE01);
            scale = task->amplitude * (0x1000 - easedValue) >> 0xC;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    BattleFx_KotrInstallBillboardMatrix((BattleFxSourcePoint *)((int)task + 4), (short)scale, 0xFFFFFE0C);
    (g_BattleSummonKotrKnight9BillboardModel.billboardModel) = Gfx_ModelTableGetRenderObject(1, g_BattleSummonKotrTableCursor);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonKotrKnight9BillboardModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCount)++;
    }
}

/*
 * Acquires retained scratch and selects the knight
 * context. At count zero calls BattleFx_KotrSpawnKnightEndpointTrail twice with duration 0x4C and
 * retained definitions; other calls BattleFx_KotrDrawKnightModel before pause. Unpaused count 51
 * calls BattleFx_KotrSpawnKnightActorBurstSprites, creates masked
 * BattleFx_KotrSpawnKnight10SpinScaleModelPerTarget tasks, allocates
 * BattleFx_KotrImpactScreenFlashTask and posts impact records. First 25 counts initialize rotation
 * (0xFC00,0,0x800), ease polar angle from 0x200 toward 0x800 and radius from 5000 toward 4000,
 * independently growing scale from eight/reducing fade. Next ten hold angle/radius and full
 * scale/zero fade while easing X rotation toward minus 0x800. Final 41 set X rotation 0xF800 on
 * entry, retain polar angle through zero multiplication and ease radius from 4000 toward minus
 * 10000; only after elapsed 26 ease scale down toward eight/fade up over fifteen. Later retires and
 * returns. Common unpaused tail rotates the polar XY vector using
 * g_BattleSummonKotrKnight10SpinScaleModel.kotrKnight10RotMatrix with uniform 4096 scale,
 * transforms in place, writes position with Z plus 10000, calls
 * BattleFx_KotrBumpKnightContextCounter and increments count.
 */
// FUNCTION: FF7 0x0047fb7d
void __cdecl BattleFx_KotrKnight10TimelineTask(void)
{
    int theta;
    int polarRadius;
    int phaseFrame;
    BattleFxKotrKnight8Record *task;
    int ratio;
    int easedValue;
    BattleKotrKnight *pKnight;

    task = (BattleFxKotrKnight8Record *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrKnight10TimelineTaskInit & 1) == 0) {
        g_BattleKotrKnight10TimelineTaskInit |= 1;
        g_BattleKotrKnight10State = (struct BattleFxKotrKnight7State *)(Gfx_GetPsxScratchpadPtr(0));
    }
    pKnight = task->knight;
    if (task->step != 0) {
        BattleFx_KotrDrawKnightModel(pKnight);
    } else {
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x4C, &g_BattleSummonKotrKnight10SpinScaleModel.kotrKnight10TrailEndpoint);
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x4C, &g_BattleSummonKotrKnight10SpinScaleModel.kotrKnight10TrailEndpointB);
    }
    if (g_BattleIsPaused == 0) {
        if (task->step == 0x33) {
            BattleFx_KotrSpawnKnightActorBurstSprites(pKnight);
            Battle_CreateBitPatternCallbackTask(g_BattleKotrTargetMask, 0, 0, BattleFx_KotrSpawnKnight10SpinScaleModelPerTarget);
            Battle_AllocateResourceSlot(BattleFx_KotrImpactScreenFlashTask);
            BattleFx_KotrPostKnightImpactRecords();
        }
        phaseFrame = task->step;
        if (phaseFrame < 0x19) {
            if (phaseFrame == 0) {
                pKnight->offsetX = 0xFC00;
                pKnight->offsetY = 0;
                pKnight->offsetZ = 0x800;
            }
            ratio = (phaseFrame << 0xC) / 0x19;
            easedValue = BattleFx_ApplySineEasing(ratio, 0x2F0);
            theta = (easedValue * 0x600 >> 0xC) + 0x200;
            polarRadius = (easedValue * -0x3E8 >> 0xC) + 0x1388;
            easedValue = BattleFx_ApplySineEasing(ratio, -0x1FF);
            pKnight->scale = (easedValue * 0xFF8 >> 0xC) + 8;
            pKnight->fadeLevel = 0x1000 - easedValue;
        } else {
            phaseFrame -= 0x19;
            if (phaseFrame < 0xA) {
                if (phaseFrame == 0) {
                    pKnight->scale = 0x1000;
                    pKnight->fadeLevel = 0;
                }
                theta = 0x800;
                polarRadius = 0xFA0;
                ratio = (phaseFrame << 0xC) / 0xA;
                easedValue = BattleFx_ApplySineEasing(ratio, 0x2FF);
                pKnight->offsetX = (easedValue * -0x400 >> 0xC) - 0x400;
            } else {
                phaseFrame -= 0xA;
                if (phaseFrame < 0x29) {
                    if (phaseFrame == 0) {
                        pKnight->offsetX = 0xF800;
                    }
                    ratio = (phaseFrame << 0xC) / 0x29;
                    easedValue = BattleFx_ApplySineEasing(ratio, -0x2F0);
                    theta = (easedValue * 0 >> 0xC) + 0x800;
                    polarRadius = (easedValue * -0x36B0 >> 0xC) + 0xFA0;
                    if (phaseFrame > 0x1A) {
                        ratio = (phaseFrame - 0x1A << 0xC) / 0xF;
                        easedValue = BattleFx_ApplySineEasing(ratio, -0x1FF);
                        pKnight->scale = (easedValue * -0xFF8 >> 0xC) + 0x1000;
                        pKnight->fadeLevel = easedValue;
                    }
                } else {
                    task->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
        RotMatrix((SVECTOR *)&g_BattleSummonKotrKnight10SpinScaleModel.kotrKnight10RotMatrix, &g_BattleKotrKnight10State->matrix);
        g_BattleKotrKnight10State->posX = CosFixed(theta) * polarRadius >> 0xC;
        g_BattleKotrKnight10State->posY = SinFixed(theta) * polarRadius >> 0xC;
        g_BattleKotrKnight10State->posZ = 0;
        g_BattleKotrKnight10State->scaleZ = 0x1000;
        g_BattleKotrKnight10State->scaleX = 0x1000;
        g_BattleKotrKnight10State->scaleY = 0x1000;
        ScaleMatrix(&g_BattleKotrKnight10State->matrix, &g_BattleKotrKnight10State->scaleX);
        TransformVectorToInt16(&g_BattleKotrKnight10State->matrix, (SVECTOR *)g_BattleKotrKnight10State, &g_BattleKotrKnight10State->posX);
        pKnight->posX = g_BattleKotrKnight10State->posX;
        pKnight->posY = g_BattleKotrKnight10State->posY;
        pKnight->posZ = g_BattleKotrKnight10State->posZ + 0x2710;
        BattleFx_KotrBumpKnightContextCounter(pKnight);
        (task->step)++;
    }
}

/*
 * Stores Gfx_ModelTableGetRenderObject(5,
 * g_BattleSummonKotrTableCursor) in g_BattleSummonKotrKnight10SpinScaleModel, builds a local
 * position with
 * BattleFx_KotrGetActorAttachLocalPosition(actor, byte at g_BattleActorRecords[actor].targetBone, &local),
 * and passes both to BattleFx_KotrSpawnEasedSpinScaleModel.
 */
// FUNCTION: FF7 0x0047ff71
void __cdecl BattleFx_KotrSpawnKnight10SpinScaleModelPerTarget(int actor)
{
    SVECTOR attachPos;

    (g_BattleSummonKotrKnight10SpinScaleModel.spinScaleModel) = Gfx_ModelTableGetRenderObject(5, g_BattleSummonKotrTableCursor);
    BattleFx_KotrGetActorAttachLocalPosition(actor, g_BattleActorRecords[actor].targetBone, &attachPos);
    BattleFx_KotrSpawnEasedSpinScaleModel(&g_BattleSummonKotrKnight10SpinScaleModel, &attachPos);
}

/*
 * Acquires retained scratch and selects the knight context. At count zero calls BattleFx_KotrSpawnKnightEndpointTrail with
 * duration 0x48 and retained definition; other calls BattleFx_KotrDrawKnightModel before pause. Unpaused count 56
 * calls BattleFx_KotrSpawnKnightActorBurstSprites, BattleFx_KotrBeginKnight11SpinScaleModels, allocates BattleFx_KotrImpactScreenFlashTask and posts impact records in order.
 * First eighteen counts use positive sine easing 0x3D0 to move Y from 5000 toward minus 1000,
 * retain X zero/Z 3000 through explicit zero terms, grow scale from eight and reduce fade. Next 38
 * hold that position and set full scale/zero fade on entry. Final sixteen use negative easing to
 * move Y from minus 1000 toward minus 10000, reduce scale toward eight and increase fade; later
 * retire and return. Common unpaused tail copies the scratch position into context with Z plus
 * 10000, calls BattleFx_KotrBumpKnightContextCounter and increments count.
 */
// FUNCTION: FF7 0x0047ffc2
void __cdecl BattleFx_KotrKnight11TimelineTask(void)
{
    int elapsed;
    int padSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxKotrKnight8Record *taskPtr;
    int normalized;
    int easedValue;
    BattleKotrKnight *pKnight;

    taskPtr = (BattleFxKotrKnight8Record *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrKnight11TimelineTaskInit & 1) == 0) {
        g_BattleKotrKnight11TimelineTaskInit |= 1;
        g_BattleKotrKnight11State = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    }
    pKnight = taskPtr->knight;
    if (taskPtr->step != 0) {
        BattleFx_KotrDrawKnightModel(pKnight);
    } else {
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x48, &g_BattleSummonKotrKnight11SpinScaleModel.kotrKnight11TrailEndpoint);
    }
    if (g_BattleIsPaused == 0) {
        if (taskPtr->step == 0x38) {
            BattleFx_KotrSpawnKnightActorBurstSprites(pKnight);
            BattleFx_KotrBeginKnight11SpinScaleModels();
            Battle_AllocateResourceSlot(BattleFx_KotrImpactScreenFlashTask);
            BattleFx_KotrPostKnightImpactRecords();
        }
        elapsed = taskPtr->step;
        if (elapsed < 0x12) {
            normalized = (elapsed << 0xC) / 0x12;
            easedValue = BattleFx_ApplySineEasing(normalized, 0x3D0);
            g_BattleKotrKnight11State->vx = easedValue * 0 >> 0xC;
            g_BattleKotrKnight11State->vy = (easedValue * -0x1770 >> 0xC) + 0x1388;
            g_BattleKotrKnight11State->vz = (easedValue * 0 >> 0xC) + 0xBB8;
            easedValue = BattleFx_ApplySineEasing(normalized, 0x3D0);
            pKnight->scale = (easedValue * 0xFF8 >> 0xC) + 8;
            pKnight->fadeLevel = 0x1000 - easedValue;
        } else {
            elapsed -= 0x12;
            if (elapsed < 0x26) {
                if (elapsed == 0) {
                    pKnight->scale = 0x1000;
                    pKnight->fadeLevel = 0;
                }
                g_BattleKotrKnight11State->vx = 0;
                g_BattleKotrKnight11State->vy = 0xFC18;
                g_BattleKotrKnight11State->vz = 0xBB8;
            } else {
                elapsed -= 0x26;
                if (elapsed < 0x10) {
                    normalized = (elapsed << 0xC) / 0x10;
                    easedValue = BattleFx_ApplySineEasing(normalized, -0x3D0);
                    g_BattleKotrKnight11State->vx = easedValue * 0 >> 0xC;
                    g_BattleKotrKnight11State->vy = (easedValue * -0x2328 >> 0xC) - 0x3E8;
                    g_BattleKotrKnight11State->vz = (easedValue * 0 >> 0xC) + 0xBB8;
                    easedValue = BattleFx_ApplySineEasing(normalized, -0x3D0);
                    pKnight->scale = (easedValue * -0xFF8 >> 0xC) + 0x1000;
                    pKnight->fadeLevel = easedValue;
                } else {
                    taskPtr->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
        pKnight->posX = g_BattleKotrKnight11State->vx;
        pKnight->posY = g_BattleKotrKnight11State->vy;
        pKnight->posZ = g_BattleKotrKnight11State->vz + 0x2710;
        BattleFx_KotrBumpKnightContextCounter(pKnight);
        (taskPtr->step)++;
    }
}

/*
 * Stores Gfx_ModelTableGetRenderObject(0xA,
 * g_BattleSummonKotrTableCursor) in g_BattleSummonKotrKnight11SpinScaleModel, calls
 * BattleFx_KotrSpawnEasedSpinScaleModel with g_BattleSummonKotrKnight11SpinScaleModel and
 * g_BattleSummonKotrKnight11SpinScaleModel.kotrKnight11SpinScaleRecord addresses, and creates a
 * task for BattleFx_KotrSpawnKnight11SpinScaleModelPairPerTarget.
 */
// FUNCTION: FF7 0x00480291
void __cdecl BattleFx_KotrBeginKnight11SpinScaleModels(void)
{
    (g_BattleSummonKotrKnight11SpinScaleModel.spinScaleModel) = Gfx_ModelTableGetRenderObject(0xA, g_BattleSummonKotrTableCursor);
    BattleFx_KotrSpawnEasedSpinScaleModel(&g_BattleSummonKotrKnight11SpinScaleModel, &g_BattleSummonKotrKnight11SpinScaleModel.kotrKnight11SpinScaleRecord);
    Battle_CreateBitPatternCallbackTask(g_BattleKotrTargetMask, 0, 0, BattleFx_KotrSpawnKnight11SpinScaleModelPairPerTarget);
}

/*
 * Takes two field-5 entries from
 * Gfx_ModelTableGetRenderObject(g_BattleSummonKotrTableCursor) into g_BattleKotrKnight11SpinScaleParamsA and
 * g_BattleKotrKnight11SpinScaleParamsB, builds one local position with
 * BattleFx_KotrGetActorAttachLocalPosition(actor, byte at g_BattleActorRecords[0].targetBone + actor * 0x1AEC, &local),
 * and submits both through BattleFx_KotrSpawnEasedSpinScaleModel at that position.
 */
// FUNCTION: FF7 0x004802d6
void __cdecl BattleFx_KotrSpawnKnight11SpinScaleModelPairPerTarget(int actor)
{
    SVECTOR attachPos;

    g_BattleKotrKnight11SpinScaleParamsA.modelGroup = Gfx_ModelTableGetRenderObject(5, g_BattleSummonKotrTableCursor);
    g_BattleKotrKnight11SpinScaleParamsB.modelGroup = Gfx_ModelTableGetRenderObject(5, g_BattleSummonKotrTableCursor);
    BattleFx_KotrGetActorAttachLocalPosition(actor, g_BattleActorRecords[actor].targetBone, &attachPos);
    BattleFx_KotrSpawnEasedSpinScaleModel(&g_BattleKotrKnight11SpinScaleParamsA, &attachPos);
    BattleFx_KotrSpawnEasedSpinScaleModel(&g_BattleKotrKnight11SpinScaleParamsB, &attachPos);
}

/*
 * Acquires retained scratch and selects the knight
 * context. At count zero calls BattleFx_KotrSpawnKnightEndpointTrail with duration 0x47 and
 * retained definition; other calls BattleFx_KotrDrawKnightModel before pause. Unpaused count 56
 * calls BattleFx_KotrSpawnKnightActorBurstSprites, creates masked
 * BattleFx_KotrSpawnKnight12SpinScaleModelPairPerTarget tasks, allocates
 * BattleFx_KotrImpactScreenFlashTask and posts impact records. First thirty counts initialize look
 * mode one and anchor (0,0,15000), ease polar angle toward 0x800 at radius 5000, reduce X/Z
 * rotations from 0x400 toward zero and grow scale from eight/reduce fade. Next eleven select look
 * mode two with anchor relative to current position on entry, clear X/Z rotations and set full
 * scale/zero fade, holding angle/radius. Final thirty keep angle through explicit zero
 * multiplication and ease radius from 5000 toward minus 10000 without changing scale/fade; later
 * retire and return. Common unpaused tail rotates the polar XY vector using
 * g_BattleSummonKotrKnight12ScaleModelPair.kotrKnight12RotMatrix with scale (4096,2048,4096),
 * transforms in place and writes position with Z plus 15000, calls
 * BattleFx_KotrBumpKnightContextCounter and increments count.
 */
// FUNCTION: FF7 0x0048034e
void __cdecl BattleFx_KotrKnight12TimelineTask(void)
{
    int theta;
    int polarRadius;
    int phaseFrame;
    BattleFxKotrKnight8Record *task;
    int ratio;
    int easedValue;
    BattleKotrKnight *pKnight;

    task = (BattleFxKotrKnight8Record *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrKnight12TimelineTaskInit & 1) == 0) {
        g_BattleKotrKnight12TimelineTaskInit |= 1;
        g_BattleSummonKotrKnight12State = (struct BattleFxKotrKnight7State *)(Gfx_GetPsxScratchpadPtr(0));
    }
    pKnight = task->knight;
    if (task->step != 0) {
        BattleFx_KotrDrawKnightModel(pKnight);
    } else {
        BattleFx_KotrSpawnKnightEndpointTrail(pKnight, 0x47, &g_BattleSummonKotrKnight12ScaleModelPair.kotrKnight12TrailEndpoint);
    }
    if (g_BattleIsPaused == 0) {
        if (task->step == 0x38) {
            BattleFx_KotrSpawnKnightActorBurstSprites(pKnight);
            Battle_CreateBitPatternCallbackTask(g_BattleKotrTargetMask, 0, 0, BattleFx_KotrSpawnKnight12SpinScaleModelPairPerTarget);
            Battle_AllocateResourceSlot(BattleFx_KotrImpactScreenFlashTask);
            BattleFx_KotrPostKnightImpactRecords();
        }
        phaseFrame = task->step;
        if (phaseFrame < 0x1E) {
            if (phaseFrame == 0) {
                pKnight->state = 1;
                pKnight->offsetX = 0;
                pKnight->offsetY = 0;
                pKnight->offsetZ = 0x3A98;
            }
            ratio = (phaseFrame << 0xC) / 0x1E;
            easedValue = BattleFx_ApplySineEasing(ratio, -0x2F0);
            theta = easedValue << 0xB >> 0xC;
            polarRadius = (easedValue * 0 >> 0xC) + 0x1388;
            pKnight->rotZ = (easedValue * -0x400 >> 0xC) + 0x400;
            pKnight->rotX = pKnight->rotZ;
            easedValue = BattleFx_ApplySineEasing(ratio, -0x2F0);
            pKnight->scale = (easedValue * 0xFF8 >> 0xC) + 8;
            pKnight->fadeLevel = 0x1000 - easedValue;
        } else {
            phaseFrame -= 0x1E;
            if (phaseFrame < 0xB) {
                if (phaseFrame == 0) {
                    pKnight->state = 2;
                    pKnight->offsetX = 0 - pKnight->posX;
                    pKnight->offsetY = 0 - pKnight->posY;
                    pKnight->offsetZ = 0x3A98 - pKnight->posZ;
                    pKnight->rotZ = 0;
                    pKnight->rotX = 0;
                    pKnight->scale = 0x1000;
                    pKnight->fadeLevel = 0;
                }
                theta = 0x800;
                polarRadius = 0x1388;
            } else {
                phaseFrame -= 0xB;
                if (phaseFrame < 0x1E) {
                    ratio = (phaseFrame << 0xC) / 0x1E;
                    easedValue = BattleFx_ApplySineEasing(ratio, -0x3F0);
                    theta = (easedValue * 0 >> 0xC) + 0x800;
                    polarRadius = (easedValue * -0x3A98 >> 0xC) + 0x1388;
                } else {
                    task->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
        RotMatrix((SVECTOR *)&g_BattleSummonKotrKnight12ScaleModelPair.kotrKnight12RotMatrix, &g_BattleSummonKotrKnight12State->matrix);
        g_BattleSummonKotrKnight12State->posX = CosFixed(theta) * polarRadius >> 0xC;
        g_BattleSummonKotrKnight12State->posY = SinFixed(theta) * polarRadius >> 0xC;
        g_BattleSummonKotrKnight12State->posZ = 0;
        g_BattleSummonKotrKnight12State->scaleZ = 0x1000;
        g_BattleSummonKotrKnight12State->scaleX = 0x1000;
        g_BattleSummonKotrKnight12State->scaleY = 0x800;
        ScaleMatrix(&g_BattleSummonKotrKnight12State->matrix, &g_BattleSummonKotrKnight12State->scaleX);
        TransformVectorToInt16(&g_BattleSummonKotrKnight12State->matrix, (SVECTOR *)g_BattleSummonKotrKnight12State, &g_BattleSummonKotrKnight12State->posX);
        pKnight->posX = g_BattleSummonKotrKnight12State->posX;
        pKnight->posY = g_BattleSummonKotrKnight12State->posY;
        pKnight->posZ = g_BattleSummonKotrKnight12State->posZ + 0x3A98;
        BattleFx_KotrBumpKnightContextCounter(pKnight);
        (task->step)++;
    }
}

/*
 * Identical in shape to
 * BattleFx_KotrSpawnKnight11SpinScaleModelPairPerTarget with a different pair of destinations: two
 * field-5 entries into g_BattleKotrKnight12SpinScaleParams and
 * g_BattleSummonKotrKnight12ScaleModelPair, one local position from
 * BattleFx_KotrGetActorAttachLocalPosition(actor, byte at g_BattleActorRecords[actor].targetBone, &local),
 * then two BattleFx_KotrSpawnEasedSpinScaleModel calls.
 */
// FUNCTION: FF7 0x004806fe
void __cdecl BattleFx_KotrSpawnKnight12SpinScaleModelPairPerTarget(int actor)
{
    SVECTOR attachPos;

    g_BattleKotrKnight12SpinScaleParams.modelGroup = Gfx_ModelTableGetRenderObject(5, g_BattleSummonKotrTableCursor);
    (g_BattleSummonKotrKnight12ScaleModelPair.spinScaleModel) = Gfx_ModelTableGetRenderObject(5, g_BattleSummonKotrTableCursor);
    BattleFx_KotrGetActorAttachLocalPosition(actor, g_BattleActorRecords[actor].targetBone, &attachPos);
    BattleFx_KotrSpawnEasedSpinScaleModel(&g_BattleKotrKnight12SpinScaleParams, &attachPos);
    BattleFx_KotrSpawnEasedSpinScaleModel(&g_BattleSummonKotrKnight12ScaleModelPair, &attachPos);
}

/*
 * Selects the knight context. At count zero calls
 * BattleFx_KotrSpawnKnightEndpointTrail with duration 120 and retained definition; other calls
 * BattleFx_KotrDrawKnightModel before pause. Unpaused count 110 allocates
 * BattleFx_KotrKnight13StretchingFlashModelTask; count 112 creates masked
 * BattleFx_KotrSpawnKnight13SpinScaleModelPerTarget tasks, calls
 * BattleFx_KotrSpawnKnightActorBurstSprites and posts impact records. First 48 counts initialize X
 * zero, Z 0xEC78 and Y rotation 0x800, then sine-ease Y from 10000 toward minus 2000 while growing
 * scale and reducing fade. Next eighteen set Y 0xF830, zero fade/full scale on entry and allocate
 * BattleFx_KotrScalingModelPairTask at elapsed five. Next 54 retain these fields; later retire and
 * return. Calls BattleFx_KotrBumpKnightContextCounter only below total count 48 or at least 66,
 * deliberately skipping it during the middle eighteen-count stage, then advances count.
 */
// FUNCTION: FF7 0x00480776
void __cdecl BattleFx_KotrKnight13TimelineTask(void)
{
    int elapsed;
    int padSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleKotrKnight *knight;
    int eased;
    BattleFxKotrKnightTimeline *taskPtr;

    taskPtr = (BattleFxKotrKnightTimeline *)BATTLE_CURRENT_TASK_SLOT;
    knight = taskPtr->knight;
    if (taskPtr->step != 0) {
        BattleFx_KotrDrawKnightModel(knight);
    } else {
        BattleFx_KotrSpawnKnightEndpointTrail(knight, 0x78, &g_BattleKotrKnight13TrailEndpoint);
    }
    if (g_BattleIsPaused == 0) {
        if (taskPtr->step == 0x6E) {
            Battle_AllocateResourceSlot(BattleFx_KotrKnight13StretchingFlashModelTask);
        }
        if (taskPtr->step == 0x70) {
            Battle_CreateBitPatternCallbackTask(g_BattleKotrTargetMask, 0, 0, BattleFx_KotrSpawnKnight13SpinScaleModelPerTarget);
            BattleFx_KotrSpawnKnightActorBurstSprites(knight);
            BattleFx_KotrPostKnightImpactRecords();
        }
        elapsed = taskPtr->step;
        if (elapsed < 0x30) {
            if (elapsed == 0) {
                knight->posX = 0;
                knight->posZ = 0xEC78;
                knight->offsetY = 0x800;
            }
            eased = (elapsed << 0xC) / 0x30;
            eased = BattleFx_ApplySineEasing(eased, 0x180);
            knight->posY = (eased * -0x2EE0 >> 0xC) + 0x2710;
            knight->fadeLevel = 0x1000 - eased;
            knight->scale = eased;
        } else {
            elapsed -= 0x30;
            if (elapsed < 0x12) {
                if (elapsed == 0) {
                    knight->posY = 0xF830;
                    knight->fadeLevel = 0;
                    knight->scale = 0x1000;
                }
                if (elapsed == 5) {
                    Battle_AllocateResourceSlot(BattleFx_KotrScalingModelPairTask);
                }
            } else {
                elapsed -= 0x12;
                if (elapsed < 0x36) { /* empty first arm: lib/codegen.h */
                } else {
                    taskPtr->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
        if (taskPtr->step < 0x30 || taskPtr->step >= 0x42) {
            BattleFx_KotrBumpKnightContextCounter(knight);
        }
        (taskPtr->step)++;
    }
}

/*
 * Writes retained matrix diagonal shorts
 * through three stages: first four counts ramp all from zero toward 0x200, next four hold 0x200,
 * then fifteen sine-ease the first toward 0x1800 while keeping the other two 0x200. At elapsed
 * eleven of that last stage starts a four-count screen flash; on exhaustion fades flash over five,
 * retires and returns before drawing. Installs the retained matrix directly, refreshes resource
 * field six and submits into g_BattleCurrentOrderingTable plus 0x4084 with shift zero. Only task
 * count advancement is pause-gated, so flash calls, matrix changes, drawing and retirement remain
 * active while paused.
 */
// FUNCTION: FF7 0x00480946
void __cdecl BattleFx_KotrKnight13StretchingFlashModelTask(void)
{
    int elapsed;
    int eased;
    BattleFxDualCounterTask *taskPtr;

    taskPtr = (BattleFxDualCounterTask *)BATTLE_CURRENT_TASK_SLOT;
    elapsed = taskPtr->nextIndex;
    if (elapsed < 4) {
        eased = (elapsed << 0xC) / 4;
        g_BattleSummonKotrKnight13MatrixDiagZ[0] = eased << 9 >> 0xC;
        g_BattleSummonKotrKnight13MatrixDiagY[0] = g_BattleSummonKotrKnight13MatrixDiagZ[0];
        g_BattleSummonKotrKnight13RotMatrix[0] = g_BattleSummonKotrKnight13MatrixDiagY[0];
    } else {
        elapsed -= 4;
        if (elapsed < 4) {
            g_BattleSummonKotrKnight13MatrixDiagZ[0] = 0x200;
            g_BattleSummonKotrKnight13MatrixDiagY[0] = g_BattleSummonKotrKnight13MatrixDiagZ[0];
            g_BattleSummonKotrKnight13RotMatrix[0] = g_BattleSummonKotrKnight13MatrixDiagY[0];
        } else {
            elapsed -= 4;
            if (elapsed < 0xF) {
                eased = (elapsed << 0xC) / 0xF;
                eased = BattleFx_ApplySineEasing(eased, 0x3FF);
                g_BattleSummonKotrKnight13RotMatrix[0] = (eased * 0x1600 >> 0xC) + 0x200;
                g_BattleSummonKotrKnight13MatrixDiagZ[0] = 0x200;
                g_BattleSummonKotrKnight13MatrixDiagY[0] = g_BattleSummonKotrKnight13MatrixDiagZ[0];
                if (elapsed == 0xB) {
                    BattleFx_StartScreenFlashRamp(4);
                }
            } else {
                BattleFx_FadeOutScreenFlashRamp(5);
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    SetRotMatrix((PsxMatrix *)g_BattleSummonKotrKnight13RotMatrix);
    SetTransMatrix(g_BattleSummonKotrKnight13RotMatrix);
    g_BattleKotrKnight13FlashDraw.modelGroup = Gfx_ModelTableGetRenderObject(6, g_BattleSummonKotrTableCursor);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleKotrKnight13FlashDraw, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->nextIndex)++;
    }
}

/*
 * Stores Gfx_ModelTableGetRenderObject(0xA,
 * g_BattleSummonKotrTableCursor) in g_BattleKotrKnight13SpinScaleParams, builds a local position with
 * BattleFx_KotrGetActorAttachLocalPosition(actor, byte at g_BattleActorRecords[actor].targetBone, &local),
 * and passes both to BattleFx_KotrSpawnEasedSpinScaleModel.
 */
// FUNCTION: FF7 0x00480ae9
void __cdecl BattleFx_KotrSpawnKnight13SpinScaleModelPerTarget(int actor)
{
    SVECTOR val;

    g_BattleKotrKnight13SpinScaleParams.modelGroup = Gfx_ModelTableGetRenderObject(0xA, g_BattleSummonKotrTableCursor);
    BattleFx_KotrGetActorAttachLocalPosition(actor, g_BattleActorRecords[actor].targetBone, &val);
    BattleFx_KotrSpawnEasedSpinScaleModel(&g_BattleKotrKnight13SpinScaleParams, &val);
}

/*
 * Acquires retained scratch. First five counts
 * sine-ease all retained matrix diagonal shorts toward 0x400; next twenty independently ease the
 * first from 0x400 toward 0x8000 with parameter 0xFFFFFE10 and the second toward zero with
 * parameter 0x4FF, leaving the third at its earlier value. Later retires before draw/pause.
 * Transforms the two retained points g_BattleKotrScalingPairPointA/B through
 * g_BattleSummonKotrTransformState into scratch translations, installs the retained diagonal matrix
 * as rotation and submits resource field one twice, using each translation and independently
 * latching blend override (2,0). Both submissions use g_BattleCurrentOrderingTable plus 0x4084
 * with shift zero. Only count advancement is pause-gated.
 */
// FUNCTION: FF7 0x00480b3a
void __cdecl BattleFx_KotrScalingModelPairTask(void)
{
    int elapsed;
    BattleFxDualCounterTask *taskPtr;
    int normalized;
    int easedValue;

    taskPtr = (BattleFxDualCounterTask *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrScalingModelPairTaskInit & 1) == 0) {
        g_BattleKotrScalingModelPairTaskInit |= 1;
        g_BattleKotrScalingPairState = Gfx_GetPsxScratchpadPtr(0);
    }
    elapsed = taskPtr->nextIndex;
    if (elapsed < 5) {
        normalized = (elapsed << 0xC) / 5;
        easedValue = BattleFx_ApplySineEasing(normalized, 0x1F0);
        g_BattleSummonKotrScalingPairMatrixDiagZ = easedValue << 0xA >> 0xC;
        g_BattleSummonKotrScalingPairMatrixDiagY = g_BattleSummonKotrScalingPairMatrixDiagZ;
        g_BattleSummonKotrScalingPairMatrix = g_BattleSummonKotrScalingPairMatrixDiagY;
    } else {
        elapsed -= 5;
        if (elapsed < 0x14) {
            normalized = (elapsed << 0xC) / 0x14;
            easedValue = BattleFx_ApplySineEasing(normalized, 0xFFFFFE10);
            g_BattleSummonKotrScalingPairMatrix = (easedValue * 0x7C00 >> 0xC) + 0x400;
            easedValue = BattleFx_ApplySineEasing(normalized, 0x4FF);
            g_BattleSummonKotrScalingPairMatrixDiagY = (easedValue * -0x400 >> 0xC) + 0x400;
        } else {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    SetRotMatrix(&g_BattleSummonKotrTransformState);
    SetTransMatrix(&g_BattleSummonKotrTransformState);
    RotTransPoint(&g_BattleKotrScalingPairPointA, (void *)(g_BattleKotrScalingPairState + 0x12), (void *)(g_BattleKotrScalingPairState + 0x3C));
    RotTransPoint(&g_BattleKotrScalingPairPointB, (void *)(g_BattleKotrScalingPairState + 0x30), (void *)(g_BattleKotrScalingPairState + 0x3C));
    SetRotMatrix(&g_BattleSummonKotrScalingPairMatrix);
    SetTransMatrix((void *)g_BattleKotrScalingPairState);
    (*(int *)&g_BattleSummonKotrScalingPairDraw) = Gfx_ModelTableGetRenderObject(1, g_BattleSummonKotrTableCursor);
    BattleFx_LatchDrawBlendOverride(2, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonKotrScalingPairDraw, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    SetTransMatrix((void *)(g_BattleKotrScalingPairState + 0x1E));
    BattleFx_LatchDrawBlendOverride(2, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonKotrScalingPairDraw, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->nextIndex)++;
    }
}

/*
 * Submits g_BattleKotrSpinPairData.layerFade0 and g_BattleKotrSpinPairData.layerFade1 through two BattleFx_KotrSpawnStackedModelLayersTask calls, each of which allocates a
 * display slot and stores the pointer at +4, then retires the 32-byte record at g_BattleHitLivewireEmitterRecords
 * selected by g_BattleActiveTaskSlotIdx by writing 0xFFFF to its short at +0.
 */
// FUNCTION: FF7 0x00480d91
void __cdecl BattleFx_KotrSpawnFirstStackedModelLayerPairTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    BattleFx_KotrSpawnStackedModelLayersTask(&g_BattleKotrSpinPairData.layerFade0);
    BattleFx_KotrSpawnStackedModelLayersTask(&g_BattleKotrSpinPairData.layerFade1);
    task->ownerSlot = BATTLE_TASK_FREE;
}

/*
 * Allocates a slot for BattleFx_KotrFadingStackedModelLayersTask and stores value as a dword at +4.
 */
// FUNCTION: FF7 0x00480dcd
void __cdecl BattleFx_KotrSpawnStackedModelLayersTask(void *value)
{
    struct BattleTaskSlot *slot;

    slot = BATTLE_NEW_TASK_SLOT(BattleFx_KotrFadingStackedModelLayersTask);
    ((BattleFxKotrFadeLayersTask *)slot)->layerFade = (int)value;
}

/*
 * Acquires retained scratch, reads the parameter pointer from task four, builds its rotation and
 * applies its scale. Across parameter durations 0x24/0x26/0x28 shades the base colour with 4096
 * minus elapsed fraction, copies the raw colour during the middle stage, then shades with elapsed
 * fraction; later retires and returns before drawing. Sets matrix translation (0,0,256), installs
 * the resulting RGB, saves geometry offset and temporarily sets it to (160,46). Calls BattleFx_KotrDrawStackedModelLayers
 * with the matrix, angle total count times parameter short 0x2A plus 0x32 and the three shorts
 * 0x2C through 0x30, then restores the saved geometry offset. While unpaused adds parameter
 * rotation increments at 0xC/0xE/0x10 to its rotation shorts and advances task count.
 */
// FUNCTION: FF7 0x00480df9
void __cdecl BattleFx_KotrFadingStackedModelLayersTask(void)
{
    int elapsed;
    BattleFxKotrFadeLayersTask *taskPtr;
    BattleFxScreenFadeTask *paramsPtr;
    int normalized;

    taskPtr = (BattleFxKotrFadeLayersTask *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrFadingStackedModelLayersTaskInit & 1) == 0) {
        g_BattleKotrFadingStackedModelLayersTaskInit |= 1;
        g_BattleSummonKotrLayerFadeState = (BattleFxSummonState *)Gfx_GetPsxScratchpadPtr(0);
    }
    paramsPtr = (BattleFxScreenFadeTask *)(taskPtr->layerFade);
    RotMatrix((SVECTOR *)((int)paramsPtr + 4), (char *)g_BattleSummonKotrLayerFadeState + 0xC);
    ScaleMatrix((void *)((int)g_BattleSummonKotrLayerFadeState + 0xC), (char *)paramsPtr + 0x14);
    elapsed = taskPtr->layerIndex;
    if (elapsed < paramsPtr->phaseLength0) {
        normalized = (elapsed << 0xC) / paramsPtr->phaseLength0;
        SetDrawColorRGB(0, 0, 0);
        Gfx_ShadePsxVertexColor((CVECTOR *)paramsPtr, 0x1000 - normalized, (CVECTOR *)g_BattleSummonKotrLayerFadeState);
    } else {
        elapsed -= paramsPtr->phaseLength0;
        if (elapsed < paramsPtr->phaseLength1) {
            *(Int32Box *)g_BattleSummonKotrLayerFadeState = *(Int32Box *)paramsPtr;
        } else {
            elapsed -= paramsPtr->phaseLength1;
            if (elapsed < paramsPtr->phaseLength2) {
                normalized = (elapsed << 0xC) / paramsPtr->phaseLength2;
                SetDrawColorRGB(0, 0, 0);
                Gfx_ShadePsxVertexColor((CVECTOR *)paramsPtr, normalized, (CVECTOR *)g_BattleSummonKotrLayerFadeState);
            } else {
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleSummonKotrLayerFadeState->transX = 0;
    g_BattleSummonKotrLayerFadeState->transY = 0;
    g_BattleSummonKotrLayerFadeState->transZ = 0x100;
    SetDrawColorRGB(g_BattleSummonKotrLayerFadeState->colorR, g_BattleSummonKotrLayerFadeState->colorG, g_BattleSummonKotrLayerFadeState->colorB);
    GetGeomOffset(&g_BattleSummonKotrLayerFadeState->offsetX, &g_BattleSummonKotrLayerFadeState->offsetY);
    SetGeomOffset(0xA0, 0x2E);
    BattleFx_KotrDrawStackedModelLayers((int)g_BattleSummonKotrLayerFadeState + 0xC, taskPtr->layerIndex * paramsPtr->layerSpacing + paramsPtr->baseOffset, paramsPtr->angleOffset, paramsPtr->layerCount, paramsPtr->variant);
    SetGeomOffset(g_BattleSummonKotrLayerFadeState->offsetX, g_BattleSummonKotrLayerFadeState->offsetY);
    if (g_BattleIsPaused == 0) {
        paramsPtr->posX += paramsPtr->velocityX;
        paramsPtr->posY += paramsPtr->velocityY;
        paramsPtr->posZ += paramsPtr->velocityZ;
        (taskPtr->layerIndex)++;
    }
}

/*
 * Selects two retained descriptors by variant
 * zero, one or default. Reduces the supplied angle modulo 0x133C; below 0x99E clears flag 0x200 in
 * both descriptor words and masks their shorts to low byte, otherwise subtracts 0x99E and sets that
 * flag. Clears two retained transform shorts, sets the third to reduced angle plus offset and
 * resets float tint to zero. Saves geometry screen, sets 256, multiplies supplied matrix with
 * g_BattleSummonKotrScalingPairDraw[14] and installs the result. For each requested iteration
 * refreshes resource field two, latches raw float tint bits, fixed blend override and
 * flag-dependent scalar override, then clears selected descriptor short eight before submission.
 * Variants below two always use the global render chain; default uses it when
 * g_BattleKotrLayerCount is at least two, otherwise the alternate g_BattleSummonKotrLayerDrawListHead
 * chain. Toggles both descriptors flag words/shorts, adds 2462.0f to tint and increments global
 * draw count. Restores geometry screen after the loop.
 */
// FUNCTION: FF7 0x00481096
void __cdecl BattleFx_KotrDrawStackedModelLayers(int matrix, int angle, int angleOffset, int count, int variant)
{
    struct {
        int angle;
        PsxMatrix matrix;
        unsigned char pad_22[6];
        int layerIndex;
        BattleFxKotrLayerState *secondary;
        int geomScreen;
        BattleFxKotrLayerRecord *primary;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    switch (variant) {
    case BATTLE_KOTR_STACKED_VARIANT_0:
        frame.primary = (BattleFxKotrLayerRecord *)&g_BattleKotrStackedVariantZeroPrimary;
        frame.secondary = (BattleFxKotrLayerState *)&g_BattleKotrStackedVariantZeroSecondary;
        break;
    case BATTLE_KOTR_STACKED_VARIANT_1:
        frame.primary = (BattleFxKotrLayerRecord *)&g_BattleKotrStackedVariantOnePrimary;
        frame.secondary = (BattleFxKotrLayerState *)&g_BattleKotrStackedVariantOneSecondary;
        break;
    default:
        frame.primary = (BattleFxKotrLayerRecord *)&g_BattleKotrStackedDefaultPrimary;
        frame.secondary = (BattleFxKotrLayerState *)&g_BattleKotrStackedDefaultSecondary;
        break;
    }
    frame.angle = angle % 0x133C;
    if (frame.angle < 0x99E) {
        frame.primary->flags &= 0xFFFFFDFF;
        frame.primary->layerMode = frame.primary->layerMode & 0xFF;
        frame.secondary->flags &= 0xFFFFFDFF;
        frame.secondary->drawFlags = frame.secondary->drawFlags & 0xFF;
    } else {
        frame.angle -= 0x99E;
        frame.primary->flags |= 0x200;
        frame.primary->layerMode = (frame.primary->layerMode & 0xFF) | 0x200;
        frame.secondary->flags |= 0x200;
        frame.secondary->drawFlags = (frame.secondary->drawFlags & 0xFF) | 0x200;
    }
    g_BattleKotrStackedRotationX = g_BattleKotrStackedRotationY = 0;
    g_BattleKotrStackedRotationZ = frame.angle + angleOffset;
    g_BattleSummonKotrLayerTint = 0.0f;
    frame.geomScreen = GetGeomScreen();
    SetGeomScreen(0x100);
    MultiplyMatrix((void *)matrix, &g_BattleKotrStackedFixedTransform, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    for (frame.layerIndex = 0; frame.layerIndex < count; (frame.layerIndex)++) {
        frame.primary->modelResource = Gfx_ModelTableGetRenderObject(2, g_BattleSummonKotrTableCursor);
        BattleFx_LatchDrawTintRgb(0, 0, *(int *)&g_BattleSummonKotrLayerTint);
        BattleFx_LatchDrawBlendOverride(2, FLOAT_0_999_BITS);
        if (frame.primary->flags & 0x200) {
            BattleFx_LatchDrawScalarOverride(2, 0x3F7B0000);
        } else {
            BattleFx_LatchDrawScalarOverride(1, 0);
        }
        frame.primary->layerMode = 0;
        if (variant < 2) {
            if (g_BattleKotrLayerCount >= 2) {
                g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.primary, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_1, 0, g_BattleOverlayDrawListHead);
            } else {
                g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.primary, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_1, 0, g_BattleOverlayDrawListHead);
            }
        } else {
            if (g_BattleKotrLayerCount >= 2) {
                g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.primary, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_1, 0, g_BattleOverlayDrawListHead);
            } else {
                g_BattleSummonKotrLayerDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.primary, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_1, 0, g_BattleSummonKotrLayerDrawListHead);
            }
        }
        frame.primary->flags ^= 0x200;
        frame.primary->layerMode ^= 0x200;
        frame.secondary->flags ^= 0x200;
        frame.secondary->drawFlags ^= 0x200;
        g_BattleSummonKotrLayerTint += 2462.0f;
        g_BattleKotrLayerCount++;
    }
    SetGeomScreen(frame.geomScreen);
}

/*
 * Same as BattleFx_KotrSpawnFirstStackedModelLayerPairTask with the other pair: BattleFx_KotrSpawnStackedModelLayersTask on g_BattleKotrSpinPairData.layerFade2 and g_BattleKotrSpinPairData.layerFade3, then
 * 0xFFFF into the short at +0 of the g_BattleHitLivewireEmitterRecords record selected by g_BattleActiveTaskSlotIdx, retiring it.
 */
// FUNCTION: FF7 0x004813bf
void __cdecl BattleFx_KotrSpawnSecondStackedModelLayerPairTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    BattleFx_KotrSpawnStackedModelLayersTask(&g_BattleKotrSpinPairData.layerFade2);
    BattleFx_KotrSpawnStackedModelLayersTask(&g_BattleKotrSpinPairData.layerFade3);
    task->ownerSlot = BATTLE_TASK_FREE;
}

/*
 * Calls BattleFx_KotrSpawnStackedModelLayersTask with the address of g_BattleKotrSpinPairData.layerFade4, allocates a slot for BattleFx_KotrConvergingStreakEmitterTask, then
 * writes 0xffff to the current resource slot first word.
 */
// FUNCTION: FF7 0x004813fb
void __cdecl BattleFx_KotrSpawnConvergingStreakEmitterAndLayerTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    BattleFx_KotrSpawnStackedModelLayersTask(&g_BattleKotrSpinPairData.layerFade4);
    Battle_AllocateResourceSlot(BattleFx_KotrConvergingStreakEmitterTask);
    task->ownerSlot = BATTLE_TASK_FREE;
}

/*
 * Returns while paused. Ramps emission contribution from zero toward 0x400 over thirty counts,
 * holds 0x400 for 56, then falls toward zero over 37; later retires and returns. Adds contribution
 * to retained accumulator g_BattleSummonKotrConvergingStreakEmitAccum and emits BattleFx_KotrConvergingStreakModelTask while it is at least 0x100,
 * preserving the remainder. Each record gets random short 0xC from 300 through 499, short 0xE as
 * its negative divided by ten and random low-twelve-bit angle 0x10. Advances task count.
 */
// FUNCTION: FF7 0x00481437
void __cdecl BattleFx_KotrConvergingStreakEmitterTask(void)
{
    int elapsed;
    BattleFxDualCounterTask *taskPtr;
    BattleFxKotrConvergingStreakTask *pStreak;
    int contribution;
    int normalized;
    int length;

    taskPtr = (BattleFxDualCounterTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    elapsed = taskPtr->nextIndex;
    if (elapsed < 0x1E) {
        normalized = (elapsed << 0xC) / 0x1E;
        contribution = normalized << 0xA >> 0xC;
    } else {
        elapsed -= 0x1E;
        if (elapsed < 0x38) {
            normalized = (elapsed << 0xC) / 0x38;
            contribution = 0x400;
        } else {
            elapsed -= 0x38;
            if (elapsed < 0x25) {
                normalized = (elapsed << 0xC) / 0x25;
                contribution = (normalized * -0x400 >> 0xC) + 0x400;
            } else {
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleSummonKotrConvergingStreakEmitAccum += contribution;
    while (g_BattleSummonKotrConvergingStreakEmitAccum >= 0x100) {
        pStreak = (BattleFxKotrConvergingStreakTask *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrConvergingStreakModelTask);
        if (DEAD_IN_RETAIL != 0) {
            length = 0x1F4;
        } else {
            length = rand() % 0xC8 + 0x12C;
        }
        pStreak->progress = length;
        pStreak->frame = -pStreak->progress / 0xA;
        pStreak->angle = rand() & 0xFFF;
        g_BattleSummonKotrConvergingStreakEmitAccum -= 0x100;
    }
    (taskPtr->nextIndex)++;
}

/*
 * Acquires retained scratch and builds Z rotation from saved task angle. Sine-eases task count
 * shifted by twelve divided by ten with parameter 0x4E0, then shrinks saved radius by 4096 minus
 * that term. Sets translation to negative radius times sine, radius times cosine and Z 512.
 * Applies scale X 32, Y the remaining fraction shifted eight/right twelve and Z zero, installs the
 * matrix directly and refreshes resource field zero. Latches blend override (2,0x3F7F3B64) and
 * submits into g_BattleCurrentOrderingTable plus 0x4064 with shift zero, retaining draw colour.
 * While unpaused advances count and retires at ten.
 */
// FUNCTION: FF7 0x004815bd
void __cdecl BattleFx_KotrConvergingStreakModelTask(void)
{
    int eased;
    BattleFxKotrRadialStreakRecord *taskPtr;
    int radius;

    taskPtr = (BattleFxKotrRadialStreakRecord *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrConvergingStreakModelTaskInit & 1) == 0) {
        g_BattleKotrConvergingStreakModelTaskInit |= 1;
        g_BattleSummonKotrConvergingStreakState = (BattleFxKotrRadialStreakState *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonKotrConvergingStreakState->rotX = 0;
    g_BattleSummonKotrConvergingStreakState->rotY = 0;
    g_BattleSummonKotrConvergingStreakState->angle = taskPtr->angle;
    RotMatrix((SVECTOR *)g_BattleSummonKotrConvergingStreakState, &g_BattleSummonKotrConvergingStreakState->matrix);
    eased = (taskPtr->frame << 0xC) / 0xA;
    eased = BattleFx_ApplySineEasing(eased, 0x4E0);
    radius = taskPtr->radius * (0x1000 - eased) >> 0xC;
    g_BattleSummonKotrConvergingStreakState->matrix.t[0] = 0 - (radius * SinFixed(taskPtr->angle) >> 0xC);
    g_BattleSummonKotrConvergingStreakState->matrix.t[1] = radius * CosFixed(taskPtr->angle) >> 0xC;
    g_BattleSummonKotrConvergingStreakState->matrix.t[2] = 0x200;
    g_BattleSummonKotrConvergingStreakState->scaleX = 0x20;
    g_BattleSummonKotrConvergingStreakState->scaleY = 0x1000 - eased << 8 >> 0xC;
    g_BattleSummonKotrConvergingStreakState->scaleZ = 0;
    ScaleMatrix(&g_BattleSummonKotrConvergingStreakState->matrix, &g_BattleSummonKotrConvergingStreakState->scaleX);
    SetRotMatrix(&g_BattleSummonKotrConvergingStreakState->matrix);
    SetTransMatrix(&g_BattleSummonKotrConvergingStreakState->matrix);
    g_BattleKotrConvergingStreakDraw.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSummonKotrTableCursor);
    BattleFx_LatchDrawBlendOverride(2, FLOAT_0_997_BITS);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleKotrConvergingStreakDraw, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_2, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frame)++;
        if (taskPtr->frame >= 0xA) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * When unpaused at counter zero allocates BattleFx_KotrFadingScrollingPrimitiveTask and stores shorts 0x25, 0x73 and 0x6C at
 * its position offsets. At counter thirty-seven calls BattleFx_KotrSpawnStackedModelLayersTask with g_BattleKotrSpinPairData.layerFade5 and retires
 * the parent; advances its counter afterward.
 */
// FUNCTION: FF7 0x004817c6
void __cdecl BattleFx_KotrFadingScrollingPrimitiveSpawnTimeline(void)
{
    BattleTaskSlot *task;
    BattleFxPositionedSpriteSlot *pSpawn;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        pSpawn = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrFadingScrollingPrimitiveTask);
        pSpawn->posX = 0x25;
        pSpawn->posY = 0x73;
        pSpawn->posZ = 0x6C;
    } else if (task->frameCounter == 0x25) {
        BattleFx_KotrSpawnStackedModelLayersTask(&g_BattleKotrSpinPairData.layerFade5);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Sets three equal retained RGB bytes
 * through task-supplied rise, hold and fall durations at four/six/eight: fixed-point rise toward
 * 128, hold 128, then fall toward zero. On exhaustion retires and returns before drawing/pause.
 * Sets primitive descriptor first word to g_BattleSummonKotrScrollPhase minus 256 and submits
 * three primitive pairs at successive values separated by 256 into g_BattleCurrentOrderingTable
 * plus 0x406C, chaining each returned render pointer. While unpaused advances the retained value by
 * four modulo 256 and increments task count.
 */
// FUNCTION: FF7 0x00481867
void __cdecl BattleFx_KotrFadingScrollingPrimitiveTask(void)
{
    int elapsed;
    BattleFxKotrFadingPrimitive *taskPtr;
    int normalized;

    taskPtr = (BattleFxKotrFadingPrimitive *)BATTLE_CURRENT_TASK_SLOT;
    elapsed = taskPtr->frameIndex;
    if (elapsed < taskPtr->phase1Frames) {
        normalized = (elapsed << 0xC) / taskPtr->phase1Frames;
        g_BattleSummonKotrFadingScrollState.kotrFadeScrollNewest = normalized << 7 >> 0xC;
        g_BattleSummonKotrFadingScrollState.kotrFadeScrollMiddle = g_BattleSummonKotrFadingScrollState.kotrFadeScrollNewest;
        g_BattleSummonKotrFadingScrollState.kotrFadeScrollOldest = g_BattleSummonKotrFadingScrollState.kotrFadeScrollMiddle;
    } else {
        elapsed -= taskPtr->phase1Frames;
        if (elapsed < taskPtr->phase2Frames) {
            g_BattleSummonKotrFadingScrollState.kotrFadeScrollNewest = 0x80;
            g_BattleSummonKotrFadingScrollState.kotrFadeScrollMiddle = g_BattleSummonKotrFadingScrollState.kotrFadeScrollNewest;
            g_BattleSummonKotrFadingScrollState.kotrFadeScrollOldest = g_BattleSummonKotrFadingScrollState.kotrFadeScrollMiddle;
        } else {
            elapsed -= taskPtr->phase2Frames;
            if (elapsed < taskPtr->phase3Frames) {
                normalized = (elapsed << 0xC) / taskPtr->phase3Frames;
                g_BattleSummonKotrFadingScrollState.kotrFadeScrollNewest = (normalized * -0x80 >> 0xC) + 0x80;
                g_BattleSummonKotrFadingScrollState.kotrFadeScrollMiddle = g_BattleSummonKotrFadingScrollState.kotrFadeScrollNewest;
                g_BattleSummonKotrFadingScrollState.kotrFadeScrollOldest = g_BattleSummonKotrFadingScrollState.kotrFadeScrollMiddle;
            } else {
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    (g_BattleSummonKotrFadingScrollState.scrollOffset) = g_BattleSummonKotrScrollPhase - 0x100;
    g_BattleOverlayDrawListHead = BattleFx_DrawPrimitivePairReturningOtArg((BattleFxBzSpriteParams *)&g_BattleSummonKotrFadingScrollState, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST, g_BattleOverlayDrawListHead);
    (g_BattleSummonKotrFadingScrollState.scrollOffset) += 0x100;
    g_BattleOverlayDrawListHead = BattleFx_DrawPrimitivePairReturningOtArg((BattleFxBzSpriteParams *)&g_BattleSummonKotrFadingScrollState, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST, g_BattleOverlayDrawListHead);
    (g_BattleSummonKotrFadingScrollState.scrollOffset) += 0x100;
    g_BattleOverlayDrawListHead = BattleFx_DrawPrimitivePairReturningOtArg((BattleFxBzSpriteParams *)&g_BattleSummonKotrFadingScrollState, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        g_BattleSummonKotrScrollPhase = g_BattleSummonKotrScrollPhase + 4 & 0xFF;
        (taskPtr->frameIndex)++;
    }
}

// Fills a render state packet from the record and draws it twice, then returns otArg untouched. The
// first draw sets the packet tag to 0x4000000, copies the record's colorCmd, packedXY, u, v, clut and
// packedSize fields into the packet, and sets bit 1 of flags. It sets state field 0xB/1 to entry 0x12 of
// g_BattleSummonKotrTableCursor and the blend factors 0x3F7FBE77 and 0x3F800000 (about 0.998 and
// 1.0), then draws. The second draw first sets posY to 0x100 and height to 0x4C, then repeats the
// same state setup.
// FUNCTION: FF7 0x00481a4e
int __cdecl BattleFx_DrawPrimitivePairReturningOtArg(BattleFxBzSpriteParams *record, int otArg, int packetCursor)
{
    BattleFxPrimitivePairPacket *packet;

    packet = (BattleFxPrimitivePairPacket *)Gfx_GetPsxPacketScratchPtr();
    packet->packetTag = 0x4000000;
    ((BattleFxDrawPacket *)packet)->color = record->colorCmd;
    packet->xyWord = record->packedXY;
    packet->texU = record->u;
    packet->texV = record->v;
    packet->clut = record->clut;
    packet->sizeWord = record->packedSize;
    packet->flags |= 2;
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0x12, g_BattleSummonKotrTableCursor)));
    Gfx_SetBlendFactorPair(FLOAT_0_999_BITS, FLOAT_ONE_BITS);
    Gfx_DrawPsxPrimitive(packet);
    ((BattleFxDrawPacket *)packet)->posY = 0x100;
    ((BattleFxDrawPacket *)packet)->height = 0x4C;
    packet->flags |= 2;
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0x12, g_BattleSummonKotrTableCursor)));
    Gfx_SetBlendFactorPair(FLOAT_0_999_BITS, FLOAT_ONE_BITS);
    Gfx_DrawPsxPrimitive(packet);
    return otArg;
}

/*
 * Allocates BattleFx_KotrSpinningModelPairTask, submits g_BattleKotrSpinPairData.layerFade6 through BattleFx_KotrSpawnStackedModelLayersTask, allocates BattleFx_KotrSpinningFadingModelTask with
 * pointer g_BattleKotrSpinModelsParamBlock and words (0, -74, 0x200) at +4/+6/+8, then retires the original current
 * slot.
 */
// FUNCTION: FF7 0x00481b5a
void __cdecl BattleFx_KotrSpawnSpinningModelsAndLayerTask(void)
{
    BattleTaskSlot *current;
    BattleFxKotrSpinningLayerTask *layerTask;
    int pairSlot;

    current = BATTLE_CURRENT_TASK_SLOT;
    pairSlot = (int)BATTLE_NEW_TASK_SLOT(BattleFx_KotrSpinningModelPairTask);
    BattleFx_KotrSpawnStackedModelLayersTask(&g_BattleKotrSpinPairData.layerFade6);
    layerTask = (BattleFxKotrSpinningLayerTask *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrSpinningFadingModelTask);
    layerTask->parameterBlock = (int)&g_BattleKotrSpinModelsParamBlock;
    layerTask->posX = 0;
    layerTask->posY = 0xFFB6;
    layerTask->posZ = 0x200;
    current->ownerSlot = BATTLE_TASK_FREE;
}

/*
 * Acquires retained scratch and selects model
 * descriptor/resource field twelve when parameter short 0x16 is zero or fourteen otherwise. Across
 * parameter durations 0x10/0x12/0x14 sets descriptor short 0xA to 4096 minus rise fraction, zero
 * during hold, then fall fraction; later retires before drawing/pause. Builds rotation from the
 * parameter first three shorts, applies uniform scale 0x300 and task XYZ translation, installing
 * directly without composing g_BattleSummonKotrTransformState here. Selects black, latches blend
 * override (2,0x3F7FBE77) and submits into g_BattleCurrentOrderingTable plus 0x4068 with shift
 * zero. While unpaused mutates parameter rotation by its increments at eight/0xA/0xC and advances
 * task count.
 */
// FUNCTION: FF7 0x00481be8
void __cdecl BattleFx_KotrSpinningFadingModelTask(void)
{
    int elapsed;
    BattleFxKotrSpinningFadeModel *task;
    BattleFxSpinFade *fadeRecord;
    int ratio;

    task = (BattleFxKotrSpinningFadeModel *)BATTLE_CURRENT_TASK_SLOT;
    fadeRecord = (BattleFxSpinFade *)task->fadeRecord;
    if ((g_BattleKotrSpinningFadingModelTaskInit & 1) == 0) {
        g_BattleKotrSpinningFadingModelTaskInit |= 1;
        g_BattleSummonKotrFadeState = (BattleFxKotrFragmentScale *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (fadeRecord->variant == 0) {
        g_BattleKotrSpinningFadeDrawA.modelGroup = Gfx_ModelTableGetRenderObject(0xC, g_BattleSummonKotrTableCursor);
        g_BattleSummonKotrModelDraw = (BattleFxModelDraw *)&g_BattleKotrSpinningFadeDrawA;
    } else {
        g_BattleKotrSpinningFadeDrawB.modelGroup = Gfx_ModelTableGetRenderObject(0xE, g_BattleSummonKotrTableCursor);
        g_BattleSummonKotrModelDraw = (BattleFxModelDraw *)&g_BattleKotrSpinningFadeDrawB;
    }
    elapsed = task->frame;
    if (elapsed < fadeRecord->phase0Length) {
        ratio = (elapsed << 0xC) / fadeRecord->phase0Length;
        g_BattleSummonKotrModelDraw->fadeLevel = 0x1000 - ratio;
    } else {
        elapsed -= fadeRecord->phase0Length;
        if (elapsed < fadeRecord->phase1Length) {
            g_BattleSummonKotrModelDraw->fadeLevel = 0;
        } else {
            elapsed -= fadeRecord->phase1Length;
            if (elapsed < fadeRecord->phase2Length) {
                ratio = (elapsed << 0xC) / fadeRecord->phase2Length;
                g_BattleSummonKotrModelDraw->fadeLevel = ratio;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    RotMatrix((SVECTOR *)fadeRecord, &g_BattleSummonKotrFadeState->matrix);
    g_BattleSummonKotrFadeState->scaleZ = 0x300;
    g_BattleSummonKotrFadeState->scaleY = 0x300;
    g_BattleSummonKotrFadeState->scaleX = 0x300;
    ScaleMatrix(&g_BattleSummonKotrFadeState->matrix, g_BattleSummonKotrFadeState);
    g_BattleSummonKotrFadeState->matrix.t[0] = task->posX;
    g_BattleSummonKotrFadeState->matrix.t[1] = task->posY;
    g_BattleSummonKotrFadeState->matrix.t[2] = task->posZ;
    SetRotMatrix(&g_BattleSummonKotrFadeState->matrix);
    SetTransMatrix(&g_BattleSummonKotrFadeState->matrix);
    SetDrawColorRGB(0, 0, 0);
    BattleFx_LatchDrawBlendOverride(2, FLOAT_0_999_BITS);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw(g_BattleSummonKotrModelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_1, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        fadeRecord->angleX += fadeRecord->stepX;
        fadeRecord->angleY += fadeRecord->stepY;
        fadeRecord->angleZ += fadeRecord->stepZ;
        (task->frame)++;
    }
}

/*
 * Acquires retained scratch. First 36 counts reduce descriptor from 4096 with scale 64. Next 0x167
 * counts build scale from 64 plus elapsed fraction times sixteen, multiplied by an increasingly
 * weighted fifteen-count sine pulse whose factor ranges from 0x800 toward 0x2000. Descriptor stays
 * zero until elapsed 0x149, then rises over thirty counts; later retires before drawing/pause.
 * Builds count-based XYZ rotations with shifts seven/six/five, translation (0,0,512) and uniform
 * scale. Installs it, selects black and draws resource field eleven with blend override
 * (2,0x3F7F3B64) into g_BattleCurrentOrderingTable plus 0x4064. Rebuilds zero rotation at the
 * same scale, installs rotation only, retaining translation, then draws resource field one with
 * override (2,0x3F7EB852) into plus 0x4060. Only count advancement is pause-gated.
 */
// FUNCTION: FF7 0x00481e7c
void __cdecl BattleFx_KotrSpinningModelPairTask(void)
{
    int elapsed;
    int scale;
    BattleFxKotrSpinningPair *taskPtr;
    int normalized;
    int wave;
    int weight;

    taskPtr = (BattleFxKotrSpinningPair *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrSpinningModelPairTaskInit & 1) == 0) {
        g_BattleKotrSpinningModelPairTaskInit |= 1;
        g_BattleKotrSpinState = (BattleFxKotrSpinState *)Gfx_GetPsxScratchpadPtr(0);
    }
    elapsed = taskPtr->spin;
    if (elapsed < 0x24) {
        normalized = (elapsed << 0xC) / 0x24;
        g_BattleKotrSpinPairData.kotrSpinPairAngle = 0x1000 - normalized;
        scale = 0x40;
    } else {
        elapsed -= 0x24;
        if (elapsed < 0x167) {
            normalized = (elapsed << 0xC) / 0x167;
            scale = (normalized << 4 >> 0xC) + 0x40;
            wave = (SinFixed((elapsed % 0xF << 0xC) / 0xF) + 0x1000) / 2;
            weight = (wave * 0x1800 >> 0xC) + 0x800;
            scale = scale * (((weight - 0x1000) * normalized >> 0xC) + 0x1000) >> 0xC;
            if (elapsed < 0x149) {
                g_BattleKotrSpinPairData.kotrSpinPairAngle = 0;
            } else {
                normalized = (elapsed - 0x149 << 0xC) / 0x1E;
                g_BattleKotrSpinPairData.kotrSpinPairAngle = normalized;
            }
        } else {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    g_BattleKotrSpinState->spinA = taskPtr->spin << 7;
    g_BattleKotrSpinState->spinB = taskPtr->spin << 6;
    g_BattleKotrSpinState->spinC = taskPtr->spin << 5;
    RotMatrix((SVECTOR *)g_BattleKotrSpinState, &g_BattleKotrSpinState->matrix);
    g_BattleKotrSpinState->matrix.t[0] = 0;
    g_BattleKotrSpinState->matrix.t[1] = 0;
    g_BattleKotrSpinState->matrix.t[2] = 0x200;
    g_BattleKotrSpinState->scaleZ = scale;
    g_BattleKotrSpinState->scaleY = scale;
    g_BattleKotrSpinState->scaleX = scale;
    ScaleMatrix(&g_BattleKotrSpinState->matrix, &g_BattleKotrSpinState->scaleX);
    SetRotMatrix(&g_BattleKotrSpinState->matrix);
    SetTransMatrix(&g_BattleKotrSpinState->matrix);
    SetDrawColorRGB(0, 0, 0);
    (g_BattleKotrSpinPairData.modelGroup) = Gfx_ModelTableGetRenderObject(0xB, g_BattleSummonKotrTableCursor);
    BattleFx_LatchDrawBlendOverride(2, FLOAT_0_997_BITS);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleKotrSpinPairData, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_2, 0, g_BattleOverlayDrawListHead);
    g_BattleKotrSpinState->spinC = 0;
    g_BattleKotrSpinState->spinB = 0;
    g_BattleKotrSpinState->spinA = 0;
    RotMatrix((SVECTOR *)g_BattleKotrSpinState, &g_BattleKotrSpinState->matrix);
    ScaleMatrix(&g_BattleKotrSpinState->matrix, &g_BattleKotrSpinState->scaleX);
    SetRotMatrix(&g_BattleKotrSpinState->matrix);
    (g_BattleKotrSpinPairData.modelGroup) = Gfx_ModelTableGetRenderObject(1, g_BattleSummonKotrTableCursor);
    BattleFx_LatchDrawBlendOverride(2, FLOAT_0_995_BITS);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleKotrSpinPairData, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_3, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->spin)++;
    }
}

/*
 * Allocates BattleFx_KotrSpinningFadingModelTask with pointer g_BattleKotrFadeModelParamBlock at +0x14 and words (0, -74, 0x200) at +4/+6/+8,
 * then retires the original current slot.
 */
// FUNCTION: FF7 0x004821e0
void __cdecl BattleFx_KotrSpawnSpinningFadingModelTask(void)
{
    BattleTaskSlot *current;
    BattleFxKotrSpinningLayerTask *fadeTask;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    current = BATTLE_CURRENT_TASK_SLOT;
    fadeTask = (BattleFxKotrSpinningLayerTask *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrSpinningFadingModelTask);
    fadeTask->parameterBlock = (int)&g_BattleKotrFadeModelParamBlock;
    fadeTask->posX = 0;
    fadeTask->posY = 0xFFB6;
    fadeTask->posZ = 0x200;
    current->ownerSlot = BATTLE_TASK_FREE;
}

/*
 * Allocates a slot for BattleFx_KotrRadialStreakEmitterTask without further initializing it, then writes 0xffff to the
 * current resource slot first word.
 */
// FUNCTION: FF7 0x00482245
void __cdecl BattleFx_KotrSpawnRadialStreakEmitterTask(void)
{
    BattleTaskSlot *current;
    int emitter;

    current = BATTLE_CURRENT_TASK_SLOT;
    emitter = (int)BATTLE_NEW_TASK_SLOT(BattleFx_KotrRadialStreakEmitterTask);
    current->ownerSlot = BATTLE_TASK_FREE;
}

/*
 * Returns while paused. Ramps emission contribution toward 0x300 over 57 counts, holds it for 132,
 * then falls over fifteen; later retires and returns. Adds contribution to retained g_BattleSummonKotrRadialStreakEmitAccum
 * and emits BattleFx_KotrRadialStreakModelTask records while it is at least 0x100, retaining the remainder. Each gets
 * random short 0xC from 300 through 499 and random low-twelve-bit angle 0x10. Advances task count.
 */
// FUNCTION: FF7 0x00482285
void __cdecl BattleFx_KotrRadialStreakEmitterTask(void)
{
    int elapsed;
    BattleFxDualCounterTask *taskPtr;
    BattleFxRadialStreakTask *pStreak;
    int contribution;
    int normalized;
    int length;

    taskPtr = (BattleFxDualCounterTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    elapsed = taskPtr->nextIndex;
    if (elapsed < 0x39) {
        normalized = (elapsed << 0xC) / 0x39;
        contribution = normalized * 0x300 >> 0xC;
    } else {
        elapsed -= 0x39;
        if (elapsed < 0x84) {
            contribution = 0x300;
        } else {
            elapsed -= 0x84;
            if (elapsed < 0xF) {
                normalized = (elapsed << 0xC) / 0xF;
                contribution = (normalized * -0x300 >> 0xC) + 0x300;
            } else {
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleSummonKotrRadialStreakEmitAccum += contribution;
    while (g_BattleSummonKotrRadialStreakEmitAccum >= 0x100) {
        pStreak = (BattleFxRadialStreakTask *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrRadialStreakModelTask);
        if (DEAD_IN_RETAIL != 0) {
            length = 0x1F4;
        } else {
            length = rand() % 0xC8 + 0x12C;
        }
        pStreak->contribution = length;
        pStreak->angle = rand() & 0xFFF;
        g_BattleSummonKotrRadialStreakEmitAccum -= 0x100;
    }
    (taskPtr->nextIndex)++;
}

/*
 * Acquires retained scratch and builds Z rotation
 * from saved task angle. Applies sine easing 0xFFFFFB20 to task count shifted twelve divided by
 * ten, deriving radius from saved short 0xC times that term. Sets translation to negative radius
 * times sine, radius times cosine and Z 512, with scale X 32, Y eased fraction shifted eight/right
 * twelve and Z zero. Installs the matrix directly, refreshes resource field zero, latches blend
 * override (2,0x3F7F3B64) and submits into g_BattleCurrentOrderingTable plus 0x4064 with shift
 * zero, retaining draw colour. While unpaused advances count and retires at ten.
 */
// FUNCTION: FF7 0x004823e4
void __cdecl BattleFx_KotrRadialStreakModelTask(void)
{
    int eased;
    BattleFxKotrRadialStreakRecord *taskPtr;
    int radius;

    taskPtr = (BattleFxKotrRadialStreakRecord *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrRadialStreakModelTaskInit & 1) == 0) {
        g_BattleKotrRadialStreakModelTaskInit |= 1;
        g_BattleSummonKotrRadialStreakState = (BattleFxKotrRadialStreakState *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonKotrRadialStreakState->rotX = 0;
    g_BattleSummonKotrRadialStreakState->rotY = 0;
    g_BattleSummonKotrRadialStreakState->angle = taskPtr->angle;
    RotMatrix((SVECTOR *)g_BattleSummonKotrRadialStreakState, &g_BattleSummonKotrRadialStreakState->matrix);
    eased = (taskPtr->frame << 0xC) / 0xA;
    eased = BattleFx_ApplySineEasing(eased, 0xFFFFFB20);
    radius = taskPtr->radius * eased >> 0xC;
    g_BattleSummonKotrRadialStreakState->matrix.t[0] = 0 - (radius * SinFixed(taskPtr->angle) >> 0xC);
    g_BattleSummonKotrRadialStreakState->matrix.t[1] = radius * CosFixed(taskPtr->angle) >> 0xC;
    g_BattleSummonKotrRadialStreakState->matrix.t[2] = 0x200;
    g_BattleSummonKotrRadialStreakState->scaleX = 0x20;
    g_BattleSummonKotrRadialStreakState->scaleY = eased << 8 >> 0xC;
    g_BattleSummonKotrRadialStreakState->scaleZ = 0;
    ScaleMatrix(&g_BattleSummonKotrRadialStreakState->matrix, &g_BattleSummonKotrRadialStreakState->scaleX);
    SetRotMatrix(&g_BattleSummonKotrRadialStreakState->matrix);
    SetTransMatrix(&g_BattleSummonKotrRadialStreakState->matrix);
    g_BattleKotrRadialStreakDraw.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSummonKotrTableCursor);
    BattleFx_LatchDrawBlendOverride(2, FLOAT_0_997_BITS);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleKotrRadialStreakDraw, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_2, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frame)++;
        if (taskPtr->frame >= 0xA) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Acquires retained scratch and
 * copies four global shorts into both primitive banks each call. At count zero calls
 * BattleFx_KotrInitFragmentMotionFromNormals. For ten selected actors whose current status has 0x80
 * and saved status lacks 0x46, calls BattleFx_KotrReturnOtArgUnchanged into the current 0x4000
 * scratch bank. Builds Y rotation 0x400 with translation (0,-2000,0), multiplying
 * g_BattleSummonKotrTransformState. First thirty counts call
 * BattleFx_KotrUpdateFragmentSpinShrinkMatrices and set selected primitive colour/command
 * 0x62808080. Next 45 randomize three shorts in each of 0x111 records on entry, call
 * BattleFx_KotrUpdateFragmentScatterMatrices with elapsed and 4096, then fade three colour bytes;
 * at elapsed 44 retire but continue drawing. Always sets g_BattleKotrFragmentData, calls
 * BattleFx_KotrDrawPsxFragmentTriangles with current bank plus 0x3FF8 and retained arguments,
 * submits the selected primitive and toggles bank. Only count advancement is pause-gated, so entry
 * initialization, randomization, retirement and bank changes can repeat while paused.
 */
// FUNCTION: FF7 0x004825e0
void __cdecl BattleFx_KotrFragmentSpinScatterFadeTimelineTask(void)
{
    int elapsed;
    BattleTaskSlot *task;
    int ratio;
    int index;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleKotrFragmentSpinScatterFadeTimelineTaskInit & 1) == 0) {
        g_BattleKotrFragmentSpinScatterFadeTimelineTaskInit |= 1;
        g_BattleSummonKotrFragmentTimelineState = (BattleKotrModelPose *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleKotrFragmentData.kotrFragmentWord20 = *(short *)&g_BattleViewportOriginX;
    g_BattleKotrFragmentData.kotrFragmentWord12 = g_BattleKotrFragmentData.kotrFragmentWord20;
    g_BattleKotrFragmentData.kotrFragmentWord21 = *(short *)&g_BattleViewportOriginY;
    g_BattleKotrFragmentData.kotrFragmentWord13 = g_BattleKotrFragmentData.kotrFragmentWord21;
    g_BattleKotrFragmentData.kotrFragmentWord22 = *(short *)&g_BattleViewportWidth;
    g_BattleKotrFragmentData.kotrFragmentWord14 = g_BattleKotrFragmentData.kotrFragmentWord22;
    g_BattleKotrFragmentData.kotrFragmentWord23 = *(short *)&g_BattleSceneViewportHeight;
    g_BattleKotrFragmentData.kotrFragmentWord15 = g_BattleKotrFragmentData.kotrFragmentWord23;
    if (task->frameCounter == 0) {
        BattleFx_KotrInitFragmentMotionFromNormals((int)&g_BattleSummonKotrFragmentMesh);
    }
    for (index = 0; index < 0xA; index++) {
        if ((g_BattleSummonKotrTargetMask & 1 << index) != 0 && (*(unsigned char *)&g_BattleActorRecords[index].drawFlags & 0x80) != 0 && (g_BattleSummonKotrSavedActorDrawFlags[index] & 0x46) == 0) {
            g_BattleOverlayDrawListHead = BattleFx_KotrReturnOtArgUnchanged(index, g_BattleSummonKotrFragmentRenderBanks + (g_BattleSummonKotrFragmentIndex << 0xE), 0xC, g_BattleOverlayDrawListHead);
        }
    }
    g_BattleSummonKotrFragmentTimelineState->rotation.vx = 0;
    g_BattleSummonKotrFragmentTimelineState->rotation.vy = 0x400;
    g_BattleSummonKotrFragmentTimelineState->rotation.vz = 0;
    RotMatrix(&g_BattleSummonKotrFragmentTimelineState->rotation, &g_BattleSummonKotrFragmentTimelineState->matrix);
    g_BattleSummonKotrFragmentTimelineState->matrix.t[2] = 0;
    g_BattleSummonKotrFragmentTimelineState->matrix.t[0] = 0;
    g_BattleSummonKotrFragmentTimelineState->matrix.t[1] = 0xFFFFF830;
    MultiplyMatrix(&g_BattleSummonKotrTransformState, &g_BattleSummonKotrFragmentTimelineState->matrix, &g_BattleSummonKotrFragmentTimelineState->matrix);
    elapsed = task->frameCounter;
    if (elapsed < 0x1E) {
        BattleFx_KotrUpdateFragmentSpinShrinkMatrices(&g_BattleSummonKotrFragmentTimelineState->matrix, task->frameCounter);
        ((BattleFxKotrFragmentPrimCommand *)&g_BattleKotrFragmentData.kotrFragmentPrimCommandWord)[g_BattleSummonKotrFragmentIndex].command = 0x62808080;
    } else {
        elapsed -= 0x1E;
        if (elapsed < 0x2D) {
            if (elapsed == 0) {
                for (index = 0; index < 0x111; index++) {
                    g_BattleKotrFragmentState[index].spinX = rand() % 0x200 - 0x100;
                    g_BattleKotrFragmentState[index].spinY = rand() % 0x200 - 0x100;
                    g_BattleKotrFragmentState[index].spinZ = rand() % 0x200 - 0x100;
                }
            }
            ratio = (elapsed << 0xC) / 0x2D;
            BattleFx_KotrUpdateFragmentScatterMatrices(&g_BattleSummonKotrFragmentTimelineState->matrix, elapsed, 0x1000);
            ((BattleFxKotrFragmentColorByte1 *)&g_BattleKotrFragmentPrimColorByte2)[g_BattleSummonKotrFragmentIndex].colorLevel = 0x1000 - ratio << 7 >> 0xC;
            ((BattleFxKotrFragmentColorByte1 *)&g_BattleKotrFragmentPrimColorByte1)[g_BattleSummonKotrFragmentIndex].colorLevel = ((BattleFxKotrFragmentColorByte1 *)&g_BattleKotrFragmentPrimColorByte2)[g_BattleSummonKotrFragmentIndex].colorLevel;
            *(char *)&((BattleFxKotrFragmentPrimCommand *)&g_BattleKotrFragmentData.kotrFragmentPrimCommandWord)[g_BattleSummonKotrFragmentIndex].command = ((BattleFxKotrFragmentColorByte1 *)&g_BattleKotrFragmentPrimColorByte1)[g_BattleSummonKotrFragmentIndex].colorLevel;
            if (elapsed == 0x2C) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
    (g_BattleKotrFragmentData.meshData) = (int)&g_BattleSummonKotrFragmentMesh;
    g_BattleOverlayDrawListHead = BattleFx_KotrDrawPsxFragmentTriangles((int *)&g_BattleKotrFragmentData, g_BattleSummonKotrFragmentRenderBanks + (g_BattleSummonKotrFragmentIndex << 0xE) + 0x3FF8, 0, g_BattleOverlayDrawListHead, (int)g_BattleSummonKotrFragmentMatrixBuffer, g_BattleKotrColorWords);
    Gfx_DrawPsxPrimitive((g_BattleSummonKotrFragmentIndex << 4) + (char *)&g_BattleKotrFragmentData.kotrFragmentPrimitiveTable);
    g_BattleSummonKotrFragmentIndex ^= 1;
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/*
 * Acquires retained 0xC0 scratch and render
 * storage. Resolves the signed-short triangle count and first three signed vertex offsets from the
 * resource rooted at argument one. For each triangle computes the narrowed average XYZ and three
 * centroid-relative vertices, installs the next thirty-byte matrix from argument five and applies
 * deferred projection. Loads the following triangle offsets at current record plus 0x10 before
 * testing projection, including after the last triangle. For transform flags lacking 0x60000 and a
 * positive front-facing result writes header 0x04000000 and the next colour word from argument six,
 * calls Gfx_ReadTransformOrderingDepth2 into an otherwise unused local and submits through the same
 * render pointer. Advances colour by four and topology record by sixteen per triangle, returning
 * that render pointer. Arguments two/three/four are unused.
 */
// FUNCTION: FF7 0x004829ce
int __cdecl BattleFx_KotrDrawPsxFragmentTriangles(void *resource, int unusedB, int unusedC, int unusedD, int matrices, int colors)
{
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    Vec3sPacked *vertQ;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int triangleCnt;
    Vec3sPacked *pVertexS;
    int missingBits;
    int supportResult;
    int otDepth;
    int unusedEntry; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int triangleIdx;
    YasuiKotrFragmentRecord *topology;
    Vec3sPacked *vertP;
    BattleFxPsxFragmentPacket *renderPtr;
    int dataBase;

    if ((g_BattleKotrDrawPsxFragmentTrianglesInit & 1) == 0) {
        g_BattleKotrDrawPsxFragmentTrianglesInit |= 1;
        g_BattleKotrTriangleScratch = (struct BattleFxTriBasisScratch *)(Gfx_GetPsxScratchpadPtr(0xC0));
    }
    dataBase = *(int *)resource + 4;
    topology = (YasuiKotrFragmentRecord *)(dataBase + *(int *)((char *)*(int *)resource));
    renderPtr = (BattleFxPsxFragmentPacket *)Gfx_GetPsxPacketScratchPtr();
    triangleCnt = topology->vertexPOffset;
    topology = (YasuiKotrFragmentRecord *)((char *)topology + 4);
    vertP = (Vec3sPacked *)(dataBase + topology->vertexPOffset);
    vertQ = (Vec3sPacked *)(dataBase + topology->vertexQOffset);
    pVertexS = (Vec3sPacked *)(dataBase + topology->vertexSOffset);
    for (triangleIdx = 0; triangleIdx < triangleCnt; triangleIdx++) {
        g_BattleKotrTriangleScratch[2].pDeltaX = (vertP->vx + vertQ->vx + pVertexS->vx) / 3;
        g_BattleKotrTriangleScratch[2].pDeltaY = (vertP->vy + vertQ->vy + pVertexS->vy) / 3;
        g_BattleKotrTriangleScratch[2].pDeltaZ = (vertP->vz + vertQ->vz + pVertexS->vz) / 3;
        g_BattleKotrTriangleScratch->pDeltaX = vertP->vx - g_BattleKotrTriangleScratch[2].pDeltaX;
        g_BattleKotrTriangleScratch->pDeltaY = vertP->vy - g_BattleKotrTriangleScratch[2].pDeltaY;
        g_BattleKotrTriangleScratch->pDeltaZ = vertP->vz - g_BattleKotrTriangleScratch[2].pDeltaZ;
        g_BattleKotrTriangleScratch->qDeltaX = vertQ->vx - g_BattleKotrTriangleScratch[2].pDeltaX;
        g_BattleKotrTriangleScratch->qDeltaY = vertQ->vy - g_BattleKotrTriangleScratch[2].pDeltaY;
        g_BattleKotrTriangleScratch->qDeltaZ = vertQ->vz - g_BattleKotrTriangleScratch[2].pDeltaZ;
        g_BattleKotrTriangleScratch[1].pDeltaX = pVertexS->vx - g_BattleKotrTriangleScratch[2].pDeltaX;
        g_BattleKotrTriangleScratch[1].pDeltaY = pVertexS->vy - g_BattleKotrTriangleScratch[2].pDeltaY;
        g_BattleKotrTriangleScratch[1].pDeltaZ = pVertexS->vz - g_BattleKotrTriangleScratch[2].pDeltaZ;
        SetRotMatrix((void *)matrices);
        SetTransMatrix((void *)matrices);
        matrices += 0x1E;
        RefreshGlobalTransformState();
        StoreDeferredDrawArgs(g_BattleKotrTriangleScratch, (char *)g_BattleKotrTriangleScratch + 8, (char *)g_BattleKotrTriangleScratch + 0x10);
        ApplyDeferredDrawArgs();
        vertP = (Vec3sPacked *)(dataBase + topology[1].vertexPOffset);
        vertQ = (Vec3sPacked *)(dataBase + topology[1].vertexQOffset);
        pVertexS = (Vec3sPacked *)(dataBase + topology[1].vertexSOffset);
        Gfx_PsxStoreClearFlag(&missingBits);
        if ((missingBits & 0x60000) == 0) {
            Gfx_PsxStoreFrontFacing(&supportResult);
            if (supportResult > 0) {
                renderPtr->tag = 0x4000000;
                Gfx_ReadTransformOrderingDepth2(&otDepth);
                renderPtr->color = *(int *)colors;
                Gfx_DrawPsxPrimitive(renderPtr);
            }
        }
        colors += 4;
        topology++;
    }
    return (int)renderPtr;
}

/*
 * Takes four arguments and returns the fourth, ignoring the first three. Mechanical; the original
 * reserves the parameter slots but only moves otArg into eax.
 */
// FUNCTION: FF7 0x00482ced
int __cdecl BattleFx_KotrReturnOtArgUnchanged(int unusedA, int unusedB, int unusedC, int otArg)
{
    return otArg;
}

/*
 * Partitions retained memory from
 * g_BattleSummonKotrFragmentPool into 0x28-byte records, thirty-byte matrices, colour words and
 * render banks using offsets 0x2AA8, 0x1FFE and 0x444. For a nonnull resource resolves its
 * signed-short triangle count and signed vertex offsets. For each triangle saves narrowed XYZ
 * centroid, forms two integer edge vectors and cross product, repeatedly arithmetic-halves all
 * normal components until each lies within plus/minus 0x4000, then normalizes. Stores normal scaled
 * by random 1000 through 1499 as a short vector, derives first-short delay from centroid Y with the
 * retained 0x300C/0x6018 mapping, clears rotation shorts and assigns random rotation increments -8
 * through seven. Sets scale 4096, random short four from four through 23 and colour 0x22FFFFFF,
 * advancing record and topology strides. Null resource still publishes all partition globals.
 */
// FUNCTION: FF7 0x00482cf5
void __cdecl BattleFx_KotrInitFragmentMotionFromNormals(int resource)
{
    struct {
        int rate;
        int speed;
        BattleFxKotrFragment *fragment;
        int unused12;
        int triIndex;
        BattleFxKotrFragmentNormal *normalPtr;
        int speedCopy;
        int triCount;
        VECTOR normal;
        VECTOR edgeB;
        VECTOR edgeA;
        int unused80;
        Vec3sPacked *pointC;
        int base;
        Vec3sPacked *pointB;
        Vec3sPacked *pointA;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    g_BattleKotrFragmentState = (struct BattleFxKotrFragmentState *)&g_BattleSummonKotrFragmentPool;
    g_BattleSummonKotrFragmentMatrixBuffer = (struct PsxMatrix *)((struct PsxMatrix *)(((int)g_BattleKotrFragmentState) + 0x2AA8));
    g_BattleKotrColorWords = ((int)((int)g_BattleSummonKotrFragmentMatrixBuffer)) + 0x1FFE;
    g_BattleSummonKotrFragmentRenderBanks = g_BattleKotrColorWords + 0x444;
    if (resource != 0) {
        frame.base = resource + 4;
        frame.normalPtr = (BattleFxKotrFragmentNormal *)(frame.base + *(int *)resource);
    }
    frame.fragment = (BattleFxKotrFragment *)g_BattleKotrFragmentState;
    if (resource != 0) {
        frame.triCount = frame.normalPtr->vertexOffsetP;
        frame.normalPtr = (BattleFxKotrFragmentNormal *)((char *)frame.normalPtr + 4);
        for (frame.triIndex = 0; frame.triIndex < frame.triCount; (frame.triIndex)++) {
            frame.pointA = (Vec3sPacked *)(frame.base + frame.normalPtr->vertexOffsetP);
            frame.pointB = (Vec3sPacked *)(frame.base + frame.normalPtr->vertexOffsetA);
            frame.pointC = (Vec3sPacked *)(frame.base + frame.normalPtr->vertexOffsetB);
            frame.fragment->centroidX = (frame.pointA->vx + frame.pointB->vx + frame.pointC->vx) / 3;
            frame.fragment->centroidY = (frame.pointA->vy + frame.pointB->vy + frame.pointC->vy) / 3;
            frame.fragment->centroidZ = (frame.pointA->vz + frame.pointB->vz + frame.pointC->vz) / 3;
            frame.edgeA.vx = frame.pointA->vx - frame.pointB->vx;
            frame.edgeA.vy = frame.pointA->vy - frame.pointB->vy;
            frame.edgeA.vz = frame.pointA->vz - frame.pointB->vz;
            frame.edgeB.vx = frame.pointB->vx - frame.pointC->vx;
            frame.edgeB.vy = frame.pointB->vy - frame.pointC->vy;
            frame.edgeB.vz = frame.pointB->vz - frame.pointC->vz;
            CrossProductInt(&frame.edgeA, &frame.edgeB, &frame.normal);
            while (frame.normal.vx < -0x4000 || frame.normal.vx > 0x4000 || frame.normal.vy < -0x4000 || frame.normal.vy > 0x4000 || frame.normal.vz < -0x4000 || frame.normal.vz > 0x4000) {
                frame.normal.vx >>= 1;
                frame.normal.vy >>= 1;
                frame.normal.vz >>= 1;
            }
            NormalizeVectorFixed(&frame.normal.vx, &frame.normal.vx);
            if (DEAD_IN_RETAIL) {
                frame.speed = 0x5DC;
            } else {
                frame.speed = rand() % 0x1F4 + 0x3E8;
            }
            frame.speedCopy = frame.speed;
            frame.fragment->velocityX = frame.normal.vx * frame.speedCopy >> 0xC;
            frame.fragment->velocityY = frame.normal.vy * frame.speedCopy >> 0xC;
            frame.fragment->velocityZ = frame.normal.vz * frame.speedCopy >> 0xC;
            frame.fragment->delay = 0x19 - (frame.fragment->centroidY + 0x300C) * 0x19 / 0x6018;
            frame.fragment->rotX = 0;
            frame.fragment->rotY = 0;
            frame.fragment->rotZ = 0;
            frame.fragment->rotSpeedX = rand() % 0x10 - 8;
            frame.fragment->rotSpeedY = rand() % 0x10 - 8;
            frame.fragment->rotSpeedZ = rand() % 0x10 - 8;
            frame.fragment->scale = 0x1000;
            if (DEAD_IN_RETAIL) {
                frame.rate = 0x18;
            } else {
                frame.rate = rand() % 0x14 + 4;
            }
            frame.fragment->shrinkRate = (short)frame.rate;
            frame.fragment++;
            frame.normalPtr = (BattleFxKotrFragmentNormal *)((char *)frame.normalPtr + 0x10);
            ((int *)g_BattleKotrColorWords)[frame.triIndex] = 0x22FFFFFF;
        }
    }
}

/*
 * Lazily acquires data block 0xC0 and traverses 273 stride-0x28 source records alongside
 * stride-0x1E destination matrices. For unpaused records whose first short lies in xMin through
 * xMin plus four advances rotation shorts and reduces scale by +4; always builds rotation,
 * translation and scale with X/Y 0x1000, composes with parentMatrix and stores each matrix.
 */
// FUNCTION: FF7 0x00483066
void __cdecl BattleFx_KotrUpdateFragmentSpinShrinkMatrices(PsxMatrix *parentMatrix, int xMin)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    SVECTOR *srcRecord;
    int i;
    int padLocal; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    PsxMatrix *matrixOut;

    if ((g_BattleKotrUpdateFragmentSpinShrinkMatricesInit & 1) == 0) {
        g_BattleKotrUpdateFragmentSpinShrinkMatricesInit |= 1;
        g_BattleKotrSpinShrinkState = (BattleFxKotrFragmentScale *)Gfx_GetPsxScratchpadPtr(0xC0);
    }
    srcRecord = (SVECTOR *)g_BattleKotrFragmentState;
    matrixOut = g_BattleSummonKotrFragmentMatrixBuffer;
    for (i = 0; i < 0x111; i++) {
        if (g_BattleIsPaused == 0 && srcRecord->vx >= xMin && srcRecord->vx < xMin + 5) {
            srcRecord[3].vx += srcRecord[4].vx;
            srcRecord[3].vy += srcRecord[4].vy;
            srcRecord[3].vz += srcRecord[4].vz;
            srcRecord->vy -= srcRecord->vz;
        }
        RotMatrixXYZ((void *)((char *)srcRecord + 0x18), &g_BattleKotrSpinShrinkState->matrix);
        g_BattleKotrSpinShrinkState->matrix.t[0] = srcRecord[1].vx;
        g_BattleKotrSpinShrinkState->matrix.t[1] = srcRecord[1].vy;
        g_BattleKotrSpinShrinkState->matrix.t[2] = srcRecord[1].vz;
        g_BattleKotrSpinShrinkState->scaleY = 0x1000;
        g_BattleKotrSpinShrinkState->scaleX = 0x1000;
        g_BattleKotrSpinShrinkState->scaleZ = srcRecord->vy;
        ScaleMatrix(&g_BattleKotrSpinShrinkState->matrix, g_BattleKotrSpinShrinkState);
        MultiplyMatrix(parentMatrix, &g_BattleKotrSpinShrinkState->matrix, matrixOut);
        srcRecord += 5;
        matrixOut++;
    }
}

/*
 * Acquires retained 0xC0 scratch and
 * visits exactly 0x111 records of stride 0x28, writing matching thirty-byte matrices. When
 * unpaused and 25 minus record delay is strictly less than supplied elapsed value, advances
 * centroid XYZ by velocity, damps velocities by 0xF00 shifted twelve with Y minus ten, and adds
 * saved rotation increments. For elapsed since activation below thirty writes equal RGB 255 minus
 * elapsed shifted eight divided by thirty with command 0x22; thereafter writes command-only black.
 * Always builds XYZ rotation from each record, sets centroid translation and scales X by saved
 * short two times supplied scale shifted twelve while Y/Z use supplied scale directly. Multiplies
 * supplied matrix into the corresponding output slot even while paused or before activation.
 */
// FUNCTION: FF7 0x00483203
void __cdecl BattleFx_KotrUpdateFragmentScatterMatrices(PsxMatrix *parentMatrix, int elapsed, int scale)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int color;
    SVECTOR *srcRecord;
    int i;
    PsxMatrix *matrixOut;
    int sinceActive;

    if ((g_BattleKotrUpdateFragmentScatterMatricesInit & 1) == 0) {
        g_BattleKotrUpdateFragmentScatterMatricesInit |= 1;
        g_BattleKotrScatterState = (BattleFxKotrScatterState *)Gfx_GetPsxScratchpadPtr(0xC0);
    }
    srcRecord = (SVECTOR *)g_BattleKotrFragmentState;
    matrixOut = g_BattleSummonKotrFragmentMatrixBuffer;
    for (i = 0; i < 0x111; i++) {
        if (g_BattleIsPaused == 0 && 0x19 - srcRecord->vx < elapsed) {
            srcRecord[1].vx += srcRecord[2].vx;
            srcRecord[1].vy += srcRecord[2].vy;
            srcRecord[1].vz += srcRecord[2].vz;
            srcRecord[2].vx = srcRecord[2].vx * 0xF00 >> 0xC;
            srcRecord[2].vy = (srcRecord[2].vy * 0xF00 >> 0xC) - 0xA;
            srcRecord[2].vz = srcRecord[2].vz * 0xF00 >> 0xC;
            srcRecord[3].vx += srcRecord[4].vx;
            srcRecord[3].vy += srcRecord[4].vy;
            srcRecord[3].vz += srcRecord[4].vz;
            sinceActive = elapsed - (0x19 - srcRecord->vx);
            if (sinceActive < 0x1E) {
                color = 0xFF - (sinceActive << 8) / 0x1E;
                color = color | 0x22000000 | color << 8 | color << 0x10;
            } else {
                color = 0x22000000;
            }
            ((int *)g_BattleKotrColorWords)[i] = color;
        }
        RotMatrixXYZ((void *)((char *)srcRecord + 0x18), &g_BattleKotrScatterState->matrix);
        g_BattleKotrScatterState->matrix.t[0] = srcRecord[1].vx;
        g_BattleKotrScatterState->matrix.t[1] = srcRecord[1].vy;
        g_BattleKotrScatterState->matrix.t[2] = srcRecord[1].vz;
        g_BattleKotrScatterState->scaledY = srcRecord->vy * scale >> 0xC;
        g_BattleKotrScatterState->scale = scale;
        g_BattleKotrScatterState->savedScale = scale;
        ScaleMatrix(&g_BattleKotrScatterState->matrix, g_BattleKotrScatterState);
        MultiplyMatrix(parentMatrix, &g_BattleKotrScatterState->matrix, matrixOut);
        srcRecord += 5;
        matrixOut++;
    }
}

/*
 * Allocates a slot for BattleFx_KotrDelayedSoundTask and stores soundId and delay as words at +0 and +4.
 */
// FUNCTION: FF7 0x0048347c
void __cdecl BattleFx_KotrSpawnDelayedSoundTask(int soundId, int delay, int firstSound, int secondSound)
{
    BattleFxDelayedSoundTask *slot;

    slot = (BattleFxDelayedSoundTask *)BATTLE_NEW_TASK_SLOT(BattleFx_KotrDelayedSoundTask);
    slot->soundId = soundId;
    slot->delay = delay;
}

/*
 * Services one 32-byte record of the table at g_BattleHitLivewireEmitterRecords, selected by the index in g_BattleActiveTaskSlotIdx
 * (record address = g_BattleHitLivewireEmitterRecords + g_BattleActiveTaskSlotIdx * 0x20). Returns immediately when g_BattleIsPaused is
 * nonzero. When the counter at +2 equals the deadline short at +0 it calls
 * Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, short at +4) and stores 0xFFFF into +0 to retire the record;
 * the counter at +2 is then incremented in either case.
 */
// FUNCTION: FF7 0x004834b4
void __cdecl BattleFx_KotrDelayedSoundTask(void)
{
    BattleFxKotrDelayedSoundTask *task;

    task = (BattleFxKotrDelayedSoundTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCount == task->triggerFrame) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, task->soundArg);
        task->triggerFrame = 0xFFFF;
    }
    (task->frameCount)++;
}
} /* extern "C" */
