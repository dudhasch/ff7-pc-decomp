// src/ff7/src/battle/battle_fx_blue_death_force_effect.cpp
// Enemy Skill [7]: Death Force (Japanese: デスフォース, dev tag: "blue/dethfo" / Dethfo)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Death Force blue magic effect: a spinning spark model and sprite around the target,
//   and wave strips drawn from a sine lookup table.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/enums/main.h"

struct GfxPsxSpriteQuadState;
struct BattleFxModelDraw;
extern "C" {

// The second half-wave table of the sine lookup lies inside g_BattleMagicFire3MultiPsxMeshBlob, at +0x508.
#define g_BattleSineQuarterTable ((short *)((char *)&g_BattleMagicFire3MultiPsxMeshBlob + 0x508))
// clang-format off
extern short g_BattleBlueDethfoSineTable[];
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off

// GLOBAL: FF7 0x008aa550
unsigned int g_BattleBlueDeathForceResourceDefs[10] = {0x00013839, (unsigned int)"DETHFO_1.TIM", 0x00000000, 0x00000000, 0x00000000, 0x003200b1, (unsigned int)"LIGHT1.RSD", 0x00000000, 0x00000000, 0x00000000};
// 1024 entries: BattleFx_LookupSineFromQuarterTable bounds its index below 1024.
// GLOBAL: FF7 0x008aa578
extern "C" short g_BattleBlueDethfoSineTable[1024] = {
    0, 6, 13, 19, 25, 31, 38, 44, 50, 57, 63, 69, 75, 82, 88, 94,
    101, 107, 113, 119, 126, 132, 138, 144, 151, 157, 163, 170, 176, 182, 188, 195,
    201, 207, 214, 220, 226, 232, 239, 245, 251, 257, 264, 270, 276, 283, 289, 295,
    301, 308, 314, 320, 326, 333, 339, 345, 351, 358, 364, 370, 376, 383, 389, 395,
    401, 408, 414, 420, 426, 433, 439, 445, 451, 458, 464, 470, 476, 483, 489, 495,
    501, 508, 514, 520, 526, 533, 539, 545, 551, 557, 564, 570, 576, 582, 589, 595,
    601, 607, 613, 620, 626, 632, 638, 644, 651, 657, 663, 669, 675, 682, 688, 694,
    700, 706, 713, 719, 725, 731, 737, 744, 750, 756, 762, 768, 774, 781, 787, 793,
    799, 805, 811, 818, 824, 830, 836, 842, 848, 854, 861, 867, 873, 879, 885, 891,
    897, 904, 910, 916, 922, 928, 934, 940, 946, 953, 959, 965, 971, 977, 983, 989,
    995, 1001, 1007, 1014, 1020, 1026, 1032, 1038, 1044, 1050, 1056, 1062, 1068, 1074, 1080, 1086,
    1092, 1099, 1105, 1111, 1117, 1123, 1129, 1135, 1141, 1147, 1153, 1159, 1165, 1171, 1177, 1183,
    1189, 1195, 1201, 1207, 1213, 1219, 1225, 1231, 1237, 1243, 1249, 1255, 1261, 1267, 1273, 1279,
    1285, 1291, 1297, 1303, 1309, 1315, 1321, 1327, 1332, 1338, 1344, 1350, 1356, 1362, 1368, 1374,
    1380, 1386, 1392, 1398, 1404, 1409, 1415, 1421, 1427, 1433, 1439, 1445, 1451, 1457, 1462, 1468,
    1474, 1480, 1486, 1492, 1498, 1503, 1509, 1515, 1521, 1527, 1533, 1538, 1544, 1550, 1556, 1562,
    1567, 1573, 1579, 1585, 1591, 1596, 1602, 1608, 1614, 1620, 1625, 1631, 1637, 1643, 1648, 1654,
    1660, 1666, 1671, 1677, 1683, 1689, 1694, 1700, 1706, 1711, 1717, 1723, 1729, 1734, 1740, 1746,
    1751, 1757, 1763, 1768, 1774, 1780, 1785, 1791, 1797, 1802, 1808, 1813, 1819, 1825, 1830, 1836,
    1842, 1847, 1853, 1858, 1864, 1870, 1875, 1881, 1886, 1892, 1898, 1903, 1909, 1914, 1920, 1925,
    1931, 1936, 1942, 1947, 1953, 1958, 1964, 1970, 1975, 1981, 1986, 1992, 1997, 2002, 2008, 2013,
    2019, 2024, 2030, 2035, 2041, 2046, 2052, 2057, 2062, 2068, 2073, 2079, 2084, 2090, 2095, 2100,
    2106, 2111, 2117, 2122, 2127, 2133, 2138, 2143, 2149, 2154, 2159, 2165, 2170, 2175, 2181, 2186,
    2191, 2197, 2202, 2207, 2213, 2218, 2223, 2228, 2234, 2239, 2244, 2249, 2255, 2260, 2265, 2270,
    2276, 2281, 2286, 2291, 2296, 2302, 2307, 2312, 2317, 2322, 2328, 2333, 2338, 2343, 2348, 2353,
    2359, 2364, 2369, 2374, 2379, 2384, 2389, 2394, 2399, 2405, 2410, 2415, 2420, 2425, 2430, 2435,
    2440, 2445, 2450, 2455, 2460, 2465, 2470, 2475, 2480, 2485, 2490, 2495, 2500, 2505, 2510, 2515,
    2520, 2525, 2530, 2535, 2540, 2545, 2550, 2555, 2559, 2564, 2569, 2574, 2579, 2584, 2589, 2594,
    2598, 2603, 2608, 2613, 2618, 2623, 2628, 2632, 2637, 2642, 2647, 2652, 2656, 2661, 2666, 2671,
    2675, 2680, 2685, 2690, 2694, 2699, 2704, 2709, 2713, 2718, 2723, 2727, 2732, 2737, 2741, 2746,
    2751, 2755, 2760, 2765, 2769, 2774, 2779, 2783, 2788, 2792, 2797, 2802, 2806, 2811, 2815, 2820,
    2824, 2829, 2833, 2838, 2843, 2847, 2852, 2856, 2861, 2865, 2870, 2874, 2878, 2883, 2887, 2892,
    2896, 2901, 2905, 2910, 2914, 2918, 2923, 2927, 2932, 2936, 2940, 2945, 2949, 2953, 2958, 2962,
    2967, 2971, 2975, 2979, 2984, 2988, 2992, 2997, 3001, 3005, 3009, 3014, 3018, 3022, 3026, 3031,
    3035, 3039, 3043, 3048, 3052, 3056, 3060, 3064, 3068, 3073, 3077, 3081, 3085, 3089, 3093, 3097,
    3102, 3106, 3110, 3114, 3118, 3122, 3126, 3130, 3134, 3138, 3142, 3146, 3150, 3154, 3158, 3162,
    3166, 3170, 3174, 3178, 3182, 3186, 3190, 3194, 3198, 3202, 3206, 3210, 3214, 3217, 3221, 3225,
    3229, 3233, 3237, 3241, 3244, 3248, 3252, 3256, 3260, 3264, 3267, 3271, 3275, 3279, 3282, 3286,
    3290, 3294, 3297, 3301, 3305, 3309, 3312, 3316, 3320, 3323, 3327, 3331, 3334, 3338, 3342, 3345,
    3349, 3352, 3356, 3360, 3363, 3367, 3370, 3374, 3378, 3381, 3385, 3388, 3392, 3395, 3399, 3402,
    3406, 3409, 3413, 3416, 3420, 3423, 3426, 3430, 3433, 3437, 3440, 3444, 3447, 3450, 3454, 3457,
    3461, 3464, 3467, 3471, 3474, 3477, 3481, 3484, 3487, 3490, 3494, 3497, 3500, 3504, 3507, 3510,
    3513, 3516, 3520, 3523, 3526, 3529, 3532, 3536, 3539, 3542, 3545, 3548, 3551, 3555, 3558, 3561,
    3564, 3567, 3570, 3573, 3576, 3579, 3582, 3585, 3588, 3591, 3594, 3597, 3600, 3603, 3606, 3609,
    3612, 3615, 3618, 3621, 3624, 3627, 3630, 3633, 3636, 3639, 3642, 3644, 3647, 3650, 3653, 3656,
    3659, 3661, 3664, 3667, 3670, 3673, 3675, 3678, 3681, 3684, 3686, 3689, 3692, 3695, 3697, 3700,
    3703, 3705, 3708, 3711, 3713, 3716, 3719, 3721, 3724, 3727, 3729, 3732, 3734, 3737, 3739, 3742,
    3745, 3747, 3750, 3752, 3755, 3757, 3760, 3762, 3765, 3767, 3770, 3772, 3775, 3777, 3779, 3782,
    3784, 3787, 3789, 3791, 3794, 3796, 3798, 3801, 3803, 3805, 3808, 3810, 3812, 3815, 3817, 3819,
    3822, 3824, 3826, 3828, 3831, 3833, 3835, 3837, 3839, 3842, 3844, 3846, 3848, 3850, 3852, 3854,
    3857, 3859, 3861, 3863, 3865, 3867, 3869, 3871, 3873, 3875, 3877, 3879, 3881, 3883, 3885, 3887,
    3889, 3891, 3893, 3895, 3897, 3899, 3901, 3903, 3905, 3907, 3909, 3910, 3912, 3914, 3916, 3918,
    3920, 3921, 3923, 3925, 3927, 3929, 3930, 3932, 3934, 3936, 3937, 3939, 3941, 3943, 3944, 3946,
    3948, 3949, 3951, 3953, 3954, 3956, 3958, 3959, 3961, 3962, 3964, 3965, 3967, 3969, 3970, 3972,
    3973, 3975, 3976, 3978, 3979, 3981, 3982, 3984, 3985, 3987, 3988, 3989, 3991, 3992, 3994, 3995,
    3996, 3998, 3999, 4001, 4002, 4003, 4005, 4006, 4007, 4008, 4010, 4011, 4012, 4014, 4015, 4016,
    4017, 4019, 4020, 4021, 4022, 4023, 4024, 4026, 4027, 4028, 4029, 4030, 4031, 4032, 4034, 4035,
    4036, 4037, 4038, 4039, 4040, 4041, 4042, 4043, 4044, 4045, 4046, 4047, 4048, 4049, 4050, 4051,
    4052, 4053, 4053, 4054, 4055, 4056, 4057, 4058, 4059, 4060, 4060, 4061, 4062, 4063, 4064, 4064,
    4065, 4066, 4067, 4067, 4068, 4069, 4070, 4070, 4071, 4072, 4072, 4073, 4074, 4074, 4075, 4076,
    4076, 4077, 4077, 4078, 4079, 4079, 4080, 4080, 4081, 4081, 4082, 4082, 4083, 4083, 4084, 4084,
    4085, 4085, 4086, 4086, 4087, 4087, 4088, 4088, 4088, 4089, 4089, 4089, 4090, 4090, 4090, 4091,
    4091, 4091, 4092, 4092, 4092, 4092, 4093, 4093, 4093, 4093, 4094, 4094, 4094, 4094, 4094, 4095,
    4095, 4095, 4095, 4095, 4095, 4095, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096
};
extern "C" unsigned char g_rawData_008aad78[8] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008aad80
struct BattleSpriteQuadRequest g_BattleBlueDeathForceSpriteQuad = {{0xf0, 0xff, 0xf0, 0xff}, 0, {0x20, 0x1f, 0x1f}, 0x2e808080, {0x3a, 0, 0x47, 0x78}, 0};
// clang-format on




// clang-format off
// GLOBAL: FF7 0x00ba4a50
int g_BattleBlueDeathForceTargetIndex = 0;
// GLOBAL: FF7 0x00ba4a54
struct GfxTableCursor *g_BattleBlueDeathForceModelTable = 0;
// clang-format on

/* Loads Death Force blue magic resources. */
// FUNCTION: FF7 0x0056f2b0
void __cdecl BattleFx_BlueLoadResDeathForce(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathCopyBuf[200];
    GfxResourceRequest *pRequest;
    int resultDefArr;

    if (g_pBattle3dScene != 0 && g_BattleBlueDeathForceModelTable == 0) {
        kind = 2;
        resultDefArr = (int)&g_BattleBlueDeathForceResourceDefs;
        stage = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008aadb0
        pRequest->resourcePath = BattleFx_BuildResourcePath("blue/dethfo", pathCopyBuf);
        Gfx_LoadModelTableIfAbsent(kind, resultDefArr, pRequest, &g_BattleBlueDeathForceModelTable, stage);
    }
}

/* Entry point for Death Force blue magic effect. */
// FUNCTION: FF7 0x0056f340
void __cdecl BattleFx_BlueEnterDeathForce(int targetMask, int userData)
{
    BattleFx_BlueLoadResDeathForce();
    /* Retail caller passes 2 arg(s) to 1-parameter `BattleFx_BlueBeginDeathForce` (__cdecl stack cleanup matches retail). */
    ((void(__cdecl *)(int, int))BattleFx_BlueBeginDeathForce)(targetMask, userData);
}

/* Initializes timeline and setup tasks for Death Force. */
// FUNCTION: FF7 0x0056f35a
void __cdecl BattleFx_BlueBeginDeathForce(int targetMask)
{
    int idx, count;

    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    idx = 0;
    for (count = 0; idx < 10; idx++) {
        if ((targetMask & 1 << idx) != 0) {
            count++;
            if (count == 1) {
                g_BattleBlueDeathForceTargetIndex = idx;
            }
        }
    }
    Battle_AllocateResourceSlot(BattleFx_BlueDeathForceSetupTask);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x169);
}

/* Sets up target position and spawns rotating spark and timeline tasks. */
// FUNCTION: FF7 0x0056f3f6
void __cdecl BattleFx_BlueDeathForceSetupTask(void)
{
    int rad;
    BattleFxDeathForceTask *task;

    task = (BattleFxDeathForceTask *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxDeathForceTask *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueDeathForceTimelineTask);
    Battle_GetBoneWorldPos(g_BattleBlueDeathForceTargetIndex, g_BattleActorRecords[g_BattleBlueDeathForceTargetIndex].targetBone, &task->posX);
    rad = g_BattleActorRecords[g_BattleBlueDeathForceTargetIndex].radius + 0x180;
    if (rad > 1900) {
        rad = 0x76C;
    }
    task->radius = rad;
}

/* Updates timeline and spawns wave strip tasks for Death Force. */
// FUNCTION: FF7 0x0056f496
void __cdecl BattleFx_BlueDeathForceTimelineTask(void)
{
    unsigned short frameFadeLevel;
    int sineValue, width, angle, opacity;
    BattleFxDeathForceTask *task;
    BattleFxDeathForceTask *pNewTask;

    task = (BattleFxDeathForceTask *)BATTLE_CURRENT_TASK_SLOT;
    width = 32;
    angle = task->frame << 7;
    if (task->frame <= 20) {
        sineValue = CosFixed(task->frame * 51);
        sineValue = sineValue << 7 >> 12;
    } else if (task->frame >= 50) {
        sineValue = SinFixed((task->frame - 0x32) * 51);
        sineValue = sineValue << 8 >> 12;
    } else {
        sineValue = 0;
    }
    if (task->frame < 16) {
        opacity = task->frame << 3;
    } else if (task->frame >= 58) {
        opacity = 0x80 - (task->frame - 0x3A) * 10;
    } else {
        opacity = 0x80;
    }
    if (task->frame == 21) {
        pNewTask = (BattleFxDeathForceTask *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueDeathForceSpinModelSparksTask);
        *(Int64Pair *)&pNewTask->posX = *(Int64Pair *)&task->posX;
        pNewTask->angle = 0;
        pNewTask->angleSpeed = 0x84;
        pNewTask->tilt = 0;
        pNewTask->tiltSpeed = 48;
        pNewTask->turn = 0;
        pNewTask->turnSpeed = 0xFFAE /* -82 */;
        pNewTask->modelScale = (task->radius << 12) / 800;
        pNewTask->sparkRadius = task->radius;
    }
    BattleFx_BlueDeathForceDrawWaveStrips(sineValue, width, angle, opacity);
    if (task->frame < 12) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        frameFadeLevel = task->frame << 8;
    } else if (task->frame >= 62) {
        frameFadeLevel = 3072 - (task->frame - 62) * 384;
    } else {
        frameFadeLevel = 3072;
    }
    if (task->frame >= 70) {
        frameFadeLevel = 0;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = frameFadeLevel;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frame == 55) {
        Battle_PostActorHitOrHurtMessage(g_BattleBlueDeathForceTargetIndex);
    }
    (task->frame)++;
}

/* Animates spinning sparks model around target for Death Force. */
// FUNCTION: FF7 0x0056f6f0
void __cdecl BattleFx_BlueDeathForceSpinModelSparksTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        SVECTOR angle;
        struct { unsigned char pad_to_8[0x8]; BattleFxDeathForceTask *slot; };
        struct { unsigned char pad_to_C[0xC]; BattleFxDeathForceTask *spriteSlot; };
        struct { unsigned char pad_to_10[0x10]; int scaleRange; };
        struct { unsigned char pad_to_14[0x14]; int scale; };
        struct { unsigned char pad_to_18[0x18]; int magnitudeScale; };
        struct { unsigned char pad_to_1C[0x1C]; int spriteIndex; };
        struct { unsigned char pad_to_20[0x20]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_40[0x40]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxDeathForceTask *)BATTLE_CURRENT_TASK_SLOT;
    frame.angle.vx = frame.slot->angle;
    frame.angle.vy = frame.slot->tilt;
    frame.angle.vz = frame.slot->turn;
    RotMatrixXYZ(&frame.angle, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.slot->posX;
    frame.rotMatrix.t[1] = frame.slot->posY;
    frame.rotMatrix.t[2] = frame.slot->posZ;
    frame.scaleX.vz = frame.slot->modelScale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleBlueDeathForceModelTable);
    frame.modelDraw->flags = 0x88;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.slot->frame < 8) {
        frame.modelDraw->fadeLevel = 4096 - (frame.slot->frame << 9);
    } else if (frame.slot->frame >= 16) {
        frame.modelDraw->fadeLevel = frame.slot->frame - 16 << 9;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = (int)BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.slot->frame >= 24) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frame < 20) {
        frame.magnitudeScale = (frame.slot->sparkRadius << 12) / 350;
        frame.scale = frame.slot->sparkRadius;
        frame.scaleRange = frame.scale << 1;
        for (frame.spriteIndex = 0; frame.spriteIndex < 4; frame.spriteIndex++) {
            frame.spriteSlot = (BattleFxDeathForceTask *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueDeathForceAnimatedSpriteTask);
            *(Int64Pair *)&frame.spriteSlot->posX = *(Int64Pair *)&frame.slot->posX;
            frame.spriteSlot->posX += (short)(rand() % frame.scaleRange - frame.scale);
            frame.spriteSlot->posY += (short)(rand() % frame.scaleRange - frame.scale);
            frame.spriteSlot->posZ += (short)(rand() % frame.scaleRange - frame.scale);
            frame.spriteSlot->magnitude = (rand() % 4096) + 2048;
            frame.spriteSlot->magnitude = frame.spriteSlot->magnitude * frame.magnitudeScale >> 12;
        }
    }
    frame.slot->angle += frame.slot->angleSpeed;
    frame.slot->tilt += frame.slot->tiltSpeed;
    frame.slot->turn += frame.slot->turnSpeed;
    (frame.slot->frame)++;
}

/* Submits animated sprite quad for Death Force effect. */
// FUNCTION: FF7 0x0056f9fd
void __cdecl BattleFx_BlueDeathForceAnimatedSpriteTask(void)
{
    BattleFxDeathForceTask *task;

    g_BattleBlueDeathForceSpriteQuad.resource = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    task = (BattleFxDeathForceTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleBlueDeathForceSpriteQuad.fadeLevel = task->frame << 5;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->magnitude, -(task->magnitude >> 4));
    g_BattleFxActiveDrawListPtr = (int)BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleBlueDeathForceSpriteQuad, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frame >= 7) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frame)++;
}
/* Renders wave strips around target using lookup sine table. */
// FUNCTION: FF7 0x0056fab8
void __cdecl BattleFx_BlueDeathForceDrawWaveStrips(int amplitude, int angleStep, int startAngle, int gray)
{
    struct {
        unsigned char quad[40];
        unsigned char tile[20];
        int offset, tex;
        BattleFxWaveStripQuad *quadptr;
        int sine, dims;
        short row, rowpad;
        int angle, i, unused;
        short x, xpad;
        int color;
        BattleFxWaveTile *tileptr;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.tileptr = (BattleFxWaveTile *)&frame.tile;
    frame.quadptr = (BattleFxWaveStripQuad *)&frame.quad;
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0, g_BattleBlueDeathForceModelTable)));
    frame.angle = startAngle;
    frame.color = (gray << 16) + (gray + (gray << 8));
    frame.x = 77;
    frame.row = 0;
    frame.tex = FLOAT_1_875_BITS;
    frame.dims = 0x200A6;
    for (frame.i = 0; frame.i < 166; frame.i++) {
        if (amplitude != 0) {
            frame.sine = BattleFx_LookupSineFromQuarterTable(frame.angle);
            frame.angle += angleStep;
            frame.offset = amplitude * frame.sine >> 12;
        } else {
            frame.offset = 0;
        }
        frame.tileptr->color = frame.color;
        frame.tileptr->x = frame.x + frame.offset;
        frame.tileptr->row = frame.row;
        frame.row++;
        frame.tileptr->texture = frame.tex;
        frame.tileptr->dims = frame.dims;
        frame.tex += 0x100;
        frame.tileptr->command = frame.tileptr->command & 0xFFFFFF | 0x4000000;
        frame.tileptr->primCode = 0x66;
        switch (g_BattleUiScalingMode) {
        case MAIN_GRAPHICS_MODE_320X240:
            frame.quadptr->x0 = frame.tileptr->x;
            frame.quadptr->y0 = frame.tileptr->row;
            frame.quadptr->x2 = frame.tileptr->x;
            frame.quadptr->y2 = frame.tileptr->row + frame.tileptr->height;
            frame.quadptr->x1 = frame.tileptr->x + frame.tileptr->width;
            frame.quadptr->y1 = frame.tileptr->row;
            frame.quadptr->x3 = frame.tileptr->x + frame.tileptr->width;
            frame.quadptr->y3 = frame.tileptr->row + frame.tileptr->height;
            break;
        case MAIN_GRAPHICS_MODE_320X240_CENTERED:
            frame.tileptr->x += 160;
            frame.tileptr->row += 120;
            frame.quadptr->x0 = frame.tileptr->x;
            frame.quadptr->y0 = frame.tileptr->row;
            frame.quadptr->x2 = frame.tileptr->x;
            frame.quadptr->y2 = frame.tileptr->row + frame.tileptr->height;
            frame.quadptr->x1 = frame.tileptr->x + frame.tileptr->width;
            frame.quadptr->y1 = frame.tileptr->row;
            frame.quadptr->x3 = frame.tileptr->x + frame.tileptr->width;
            frame.quadptr->y3 = frame.tileptr->row + frame.tileptr->height;
            break;
        case MAIN_GRAPHICS_MODE_640X480:
            frame.tileptr->x <<= 1;
            frame.tileptr->row <<= 1;
            frame.quadptr->x0 = frame.tileptr->x;
            frame.quadptr->y0 = frame.tileptr->row;
            frame.quadptr->x2 = frame.tileptr->x;
            frame.quadptr->y2 = frame.tileptr->row + frame.tileptr->height * 2;
            frame.quadptr->x1 = frame.tileptr->x + frame.tileptr->width * 2;
            frame.quadptr->y1 = frame.tileptr->row;
            frame.quadptr->x3 = frame.tileptr->x + frame.tileptr->width * 2;
            frame.quadptr->y3 = frame.tileptr->row + frame.tileptr->height * 2;
        }
        frame.quadptr->command = frame.quadptr->command & 0xFFFFFF | 0x9000000;
        frame.quadptr->flags = 0x2C;
        frame.quadptr->colorR = frame.tileptr->colorR;
        frame.quadptr->colorG = frame.tileptr->colorG;
        frame.quadptr->colorB = frame.tileptr->colorB;
        frame.quadptr->u0 = frame.tileptr->u;
        frame.quadptr->v0 = frame.tileptr->v;
        frame.quadptr->u2 = frame.tileptr->u;
        frame.quadptr->v2 = frame.tileptr->v + frame.tileptr->height;
        frame.quadptr->u1 = frame.tileptr->u + frame.tileptr->width;
        frame.quadptr->v1 = frame.tileptr->v;
        frame.quadptr->u3 = frame.tileptr->u + frame.tileptr->width;
        frame.quadptr->v3 = frame.tileptr->v + frame.tileptr->height;
        Gfx_SetBlendFactorPair(FLOAT_HALF_BITS, FLOAT_ONE_BITS);
        UNFOLDED(1) ? (frame.quadptr->flags = frame.quadptr->flags | 2) : (frame.quadptr->flags = frame.quadptr->flags & ~2);
        Gfx_DrawPsxPrimitive(frame.quadptr);
    }
}

/* Computes full-period sine value from a 1024-entry quarter-wave table. */
// FUNCTION: FF7 0x0056ff42
int __cdecl BattleFx_LookupSineFromQuarterTable(int angle)
{
    struct {
        int neg;
        int result;
        short pad;
        short value;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    angle &= 0xFFF;
    if (angle < 0x800) {
        if (angle < 0x400) {
            local.value = g_BattleBlueDethfoSineTable[angle];
        } else {
            local.value = g_BattleBlueDethfoSineTable[0x800 - angle];
        }
        local.result = local.value;
    } else {
        if (angle < 0xC00) {
            local.neg = -g_BattleSineQuarterTable[angle];
        } else {
            local.neg = -g_BattleBlueDethfoSineTable[0x1000 - angle];
        }
        local.result = local.neg;
    }
    return local.result;
}
} /* extern "C" */
