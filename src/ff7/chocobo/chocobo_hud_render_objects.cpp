// src/ff7/chocobo/chocobo_hud_render_objects.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Render objects for the chocobo race HUD: chooses the texture tables for the race class
//   and scale, creates and destroys the HUD objects, and flushes their dirty primitive batches.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/chocobo.h"
#include "lib/pfile.h"
#include "ff7/api/chocobo.h"
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include <math.h>
#include <string.h>
#include "ff7/gte.h"
#include "lib/enums/graphics.h"
#include "ff7/enums/main.h"
#include "ff7/enums/chocobo.h"
#include "lib/enums/file.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00971de0
char *g_pChocoboHudTexNames0[6] = {
    // STRING: FF7 0x00972178
    "data\\tim\\class_c",
    // STRING: FF7 0x0097218c
    "data\\tim\\class_b",
    // STRING: FF7 0x009721a0
    "data\\tim\\class_a",
    // STRING: FF7 0x009721b4
    "data\\tim\\class_s",
    // STRING: FF7 0x009721c8
    "data\\tim\\class_z",
    // STRING: FF7 0x009721dc
    "data\\tim\\kao",
};
// GLOBAL: FF7 0x00971df8
char *g_pChocoboHudTexNames1[10] = {
    // STRING: FF7 0x009721ec
    "A00-0.TIM",
    // STRING: FF7 0x009721f8
    "A00-1.TIM",
    // STRING: FF7 0x00972204
    "A01A.TIM",
    // STRING: FF7 0x00972210
    "A01B.TIM",
    // STRING: FF7 0x0097221c
    "A02A_1.TIM",
    // STRING: FF7 0x00972228
    "A02B_1.TIM",
    // STRING: FF7 0x00972234
    "A03A_1a.TIM",
    // STRING: FF7 0x00972240
    "A03A_1b.TIM",
    // STRING: FF7 0x0097224c
    "A03B_1.TIM",
    // STRING: FF7 0x00972258
    "A04.TIM",
};
// GLOBAL: FF7 0x00971e20
char *g_pChocoboHudTexNames2[10] = {
    // STRING: FF7 0x00972260
    "B00-0.TIM",
    // STRING: FF7 0x0097226c
    "B00-1.TIM",
    // STRING: FF7 0x00972278
    "B01A.TIM",
    // STRING: FF7 0x00972284
    "B01B.TIM",
    // STRING: FF7 0x00972290
    "B02A_1.TIM",
    // STRING: FF7 0x0097229c
    "B02B_1.TIM",
    // STRING: FF7 0x009722a8
    "B03A_1a.TIM",
    // STRING: FF7 0x009722b4
    "B03A_1b.TIM",
    // STRING: FF7 0x009722c0
    "B03B_1.TIM",
    // STRING: FF7 0x009722cc
    "B04.TIM",
};
// GLOBAL: FF7 0x00971e48
char *g_pChocoboHudTexNames3[10] = {
    // STRING: FF7 0x009722d4
    "C00-0.TIM",
    // STRING: FF7 0x009722e0
    "C00-1.TIM",
    // STRING: FF7 0x009722ec
    "C01A.TIM",
    // STRING: FF7 0x009722f8
    "C01B.TIM",
    // STRING: FF7 0x00972304
    "C02A_1.TIM",
    // STRING: FF7 0x00972310
    "C02B_1.TIM",
    // STRING: FF7 0x0097231c
    "C03A_1a.TIM",
    // STRING: FF7 0x00972328
    "C03A_1b.TIM",
    // STRING: FF7 0x00972334
    "C03B.TIM",
    // STRING: FF7 0x00972340
    "C04.TIM",
};
// GLOBAL: FF7 0x00971e70
char *g_pChocoboHudTexNames4[10] = {
    // STRING: FF7 0x00972348
    "S00-0.TIM",
    // STRING: FF7 0x00972354
    "S00-1.TIM",
    // STRING: FF7 0x00972360
    "S01A.TIM",
    // STRING: FF7 0x0097236c
    "S01B.TIM",
    // STRING: FF7 0x00972378
    "S02A_1.TIM",
    // STRING: FF7 0x00972384
    "S02B_1.TIM",
    // STRING: FF7 0x00972390
    "S03A_1a.TIM",
    // STRING: FF7 0x0097239c
    "S03A_1b.TIM",
    // STRING: FF7 0x009723a8
    "S03B_1.TIM",
    // STRING: FF7 0x009723b4
    "S04.TIM",
};
// GLOBAL: FF7 0x00971e98
char *g_pChocoboHudTexNames5[4] = {
    // STRING: FF7 0x009723bc
    "z01_1.tim",
    // STRING: FF7 0x009723c8
    "z02.tim",
    // STRING: FF7 0x009723d0
    "z04_1.tim",
    // STRING: FF7 0x009723dc
    "z05_1.tim",
};
// GLOBAL: FF7 0x00971ea8
char *g_pChocoboHudTexNames6[11] = {
    // STRING: FF7 0x009723e8
    "C_KAN1.TIM",
    // STRING: FF7 0x009723f4
    "C_KAN2.TIM",
    // STRING: FF7 0x00972400
    "C_NUM.TIM",
    // STRING: FF7 0x0097240c
    "C_ICON1.TIM",
    // STRING: FF7 0x00972418
    "USFONT1.TIM",
    // STRING: FF7 0x00972424
    "CLC_MINI.TIM",
    // STRING: FF7 0x00972434
    "CSC_MINI.TIM",
    // STRING: FF7 0x00972444
    "C_AUTO.TIM",
    // STRING: FF7 0x00972450
    "C_MANUAL.TIM",
    // STRING: FF7 0x00972460
    "BAR4.TIM",
    // STRING: FF7 0x0097246c
    "BAR5.TIM",
};
// GLOBAL: FF7 0x00971ed8
char *g_pChocoboHudTexNames7[10] = {
    // STRING: FF7 0x00972478
    "AA.tim",
    // STRING: FF7 0x00972480
    "AB.tim",
    // STRING: FF7 0x00972488
    "AC.tim",
    // STRING: FF7 0x00972490
    "AD.tim",
    // STRING: FF7 0x00972498
    "AE.tim",
    // STRING: FF7 0x009724a0
    "AF.tim",
    // STRING: FF7 0x009724a8
    "AG.tim",
    // STRING: FF7 0x009724b0
    "AH.tim",
    // STRING: FF7 0x009724b8
    "AI.tim",
    // STRING: FF7 0x009724c0
    "AJ.tim",
};
// GLOBAL: FF7 0x00971f00
char *g_pChocoboHudTexNames8[10] = {
    // STRING: FF7 0x009724c8
    "AK.tim",
    // STRING: FF7 0x009724d0
    "AL.tim",
    // STRING: FF7 0x009724d8
    "AM.tim",
    // STRING: FF7 0x009724e0
    "AN.tim",
    // STRING: FF7 0x009724e8
    "AO.tim",
    // STRING: FF7 0x009724f0
    "AP.tim",
    // STRING: FF7 0x009724f8
    "AQ.tim",
    // STRING: FF7 0x00972500
    "AR.tim",
    // STRING: FF7 0x00972508
    "AS.tim",
    // STRING: FF7 0x00972510
    "AT.tim",
};
// GLOBAL: FF7 0x00971f28
char *g_pChocoboHudTexNames9[10] = {
    // STRING: FF7 0x00972518
    "AU.tim",
    // STRING: FF7 0x00972520
    "AV.tim",
    // STRING: FF7 0x00972528
    "AW.tim",
    // STRING: FF7 0x00972530
    "AX.tim",
    // STRING: FF7 0x00972538
    "AY.tim",
    // STRING: FF7 0x00972540
    "AZ.tim",
    // STRING: FF7 0x00972548
    "BA.tim",
    // STRING: FF7 0x00972550
    "BB.tim",
    // STRING: FF7 0x00972558
    "BC.tim",
    // STRING: FF7 0x00972560
    "BD.tim",
};
// GLOBAL: FF7 0x00971f50
char *g_pChocoboHudTexNames10[10] = {
    // STRING: FF7 0x00972568
    "BE.tim",
    // STRING: FF7 0x00972570
    "BF.tim",
    // STRING: FF7 0x00972578
    "BG.tim",
    // STRING: FF7 0x00972580
    "BH.tim",
    // STRING: FF7 0x00972588
    "BI.tim",
    // STRING: FF7 0x00972590
    "BJ.tim",
    // STRING: FF7 0x00972598
    "BK.tim",
    // STRING: FF7 0x009725a0
    "BL.tim",
    // STRING: FF7 0x009725a8
    "BM.tim",
    // STRING: FF7 0x009725b0
    "BN.tim",
};
// GLOBAL: FF7 0x00971f78
char *g_pChocoboHudTexNames11[4] = {
    // STRING: FF7 0x009725b8
    "BO.tim",
    // STRING: FF7 0x009725c0
    "BP.tim",
    // STRING: FF7 0x009725c8
    "BQ.tim",
    // STRING: FF7 0x009725d0
    "BR.tim",
};
// GLOBAL: FF7 0x00971f88
char *g_pChocoboHudTexNames12[11] = {
    // STRING: FF7 0x009725d8
    "BS.tim",
    // STRING: FF7 0x009725e0
    "BT.tim",
    // STRING: FF7 0x009725e8
    "BU.tim",
    // STRING: FF7 0x009725f0
    "BV.tim",
    // STRING: FF7 0x009725f8
    "BW.tim",
    // STRING: FF7 0x00972600
    "BX.tim",
    // STRING: FF7 0x00972608
    "BY.tim",
    // STRING: FF7 0x00972610
    "BZ.tim",
    // STRING: FF7 0x00972618
    "CA.tim",
    // STRING: FF7 0x00972620
    "CB.tim",
    // STRING: FF7 0x00972628
    "CC.tim",
};
// GLOBAL: FF7 0x00971fb8
char *g_pChocoboHudTexNames13[10] = {
    // STRING: FF7 0x00972630
    "A00-0.TIM",
    // STRING: FF7 0x0097263c
    "A00-1.TIM",
    // STRING: FF7 0x00972648
    "A01A.TIM",
    // STRING: FF7 0x00972654
    "A01B.TIM",
    // STRING: FF7 0x00972660
    "A02A.TIM",
    // STRING: FF7 0x0097266c
    "A02B.TIM",
    // STRING: FF7 0x00972678
    "A03A.TIM",
    // STRING: FF7 0x00972684
    "NULL.TIM",
    // STRING: FF7 0x00972690
    "A03B.TIM",
    // STRING: FF7 0x0097269c
    "A04.TIM",
};
// GLOBAL: FF7 0x00971fe0
char *g_pChocoboHudTexNames14[10] = {
    // STRING: FF7 0x009726a4
    "B00-0.TIM",
    // STRING: FF7 0x009726b0
    "B00-1.TIM",
    // STRING: FF7 0x009726bc
    "B01A.TIM",
    // STRING: FF7 0x009726c8
    "B01B.TIM",
    // STRING: FF7 0x009726d4
    "B02A.TIM",
    // STRING: FF7 0x009726e0
    "B02B.TIM",
    // STRING: FF7 0x009726ec
    "B03A.TIM",
    // STRING: FF7 0x009726f8
    "NULL.TIM",
    // STRING: FF7 0x00972704
    "B03B.TIM",
    // STRING: FF7 0x00972710
    "B04.TIM",
};
// GLOBAL: FF7 0x00972008
char *g_pChocoboHudTexNames15[10] = {
    // STRING: FF7 0x00972718
    "C00-0.TIM",
    // STRING: FF7 0x00972724
    "C00-1.TIM",
    // STRING: FF7 0x00972730
    "C01A.TIM",
    // STRING: FF7 0x0097273c
    "C01B.TIM",
    // STRING: FF7 0x00972748
    "C02A.TIM",
    // STRING: FF7 0x00972754
    "C02B.TIM",
    // STRING: FF7 0x00972760
    "C03A.TIM",
    // STRING: FF7 0x0097276c
    "NULL.TIM",
    // STRING: FF7 0x00972778
    "C03B.TIM",
    // STRING: FF7 0x00972784
    "C04.TIM",
};
// GLOBAL: FF7 0x00972030
char *g_pChocoboHudTexNames16[10] = {
    // STRING: FF7 0x0097278c
    "S00-0.TIM",
    // STRING: FF7 0x00972798
    "S00-1.TIM",
    // STRING: FF7 0x009727a4
    "S01A.TIM",
    // STRING: FF7 0x009727b0
    "S01B.TIM",
    // STRING: FF7 0x009727bc
    "S02A.TIM",
    // STRING: FF7 0x009727c8
    "S02B.TIM",
    // STRING: FF7 0x009727d4
    "S03A.TIM",
    // STRING: FF7 0x009727e0
    "NULL.TIM",
    // STRING: FF7 0x009727ec
    "S03B.TIM",
    // STRING: FF7 0x009727f8
    "S04.TIM",
};
// GLOBAL: FF7 0x00972058
char *g_pChocoboHudTexNames17[4] = {
    // STRING: FF7 0x00972800
    "z01.tim",
    // STRING: FF7 0x00972808
    "z02.tim",
    // STRING: FF7 0x00972810
    "z04.tim",
    // STRING: FF7 0x00972818
    "z05.tim",
};
// GLOBAL: FF7 0x00972068
char *g_pChocoboHudTexNames18[11] = {
    // STRING: FF7 0x00972820
    "CHO_KAN1.TIM",
    // STRING: FF7 0x00972830
    "CHO_KAN2.TIM",
    // STRING: FF7 0x00972840
    "CHO_NUM.TIM",
    // STRING: FF7 0x0097284c
    "CHO_ICON.TIM",
    // STRING: FF7 0x0097285c
    "USFONT1.TIM",
    // STRING: FF7 0x00972868
    "LC_MINI.TIM",
    // STRING: FF7 0x00972874
    "SC_MINI.TIM",
    // STRING: FF7 0x00972880
    "AUTO.TIM",
    // STRING: FF7 0x0097288c
    "MANUAL.TIM",
    // STRING: FF7 0x00972898
    "BAR4.TIM",
    // STRING: FF7 0x009728a4
    "BAR5.TIM",
};
// GLOBAL: FF7 0x00972098
char *g_pChocoboHudTexNames19[10] = {
    // STRING: FF7 0x009728b0
    "AA.tim",
    // STRING: FF7 0x009728b8
    "AB.tim",
    // STRING: FF7 0x009728c0
    "AC.tim",
    // STRING: FF7 0x009728c8
    "AD.tim",
    // STRING: FF7 0x009728d0
    "RE.tim",
    // STRING: FF7 0x009728d8
    "RF.tim",
    // STRING: FF7 0x009728e0
    "RG.tim",
    // STRING: FF7 0x009728e8
    "NULL.tim",
    // STRING: FF7 0x009728f4
    "RH.tim",
    // STRING: FF7 0x009728fc
    "AJ.tim",
};
// GLOBAL: FF7 0x009720c0
char *g_pChocoboHudTexNames20[10] = {
    // STRING: FF7 0x00972904
    "AK.tim",
    // STRING: FF7 0x0097290c
    "AL.tim",
    // STRING: FF7 0x00972914
    "AM.tim",
    // STRING: FF7 0x0097291c
    "AN.tim",
    // STRING: FF7 0x00972924
    "RI.TIM",
    // STRING: FF7 0x0097292c
    "RJ.TIM",
    // STRING: FF7 0x00972934
    "RK.TIM",
    // STRING: FF7 0x0097293c
    "NULL.TIM",
    // STRING: FF7 0x00972948
    "RL.TIM",
    // STRING: FF7 0x00972950
    "AT.tim",
};
// GLOBAL: FF7 0x009720e8
char *g_pChocoboHudTexNames21[10] = {
    // STRING: FF7 0x00972958
    "AU.tim",
    // STRING: FF7 0x00972960
    "AV.tim",
    // STRING: FF7 0x00972968
    "AW.tim",
    // STRING: FF7 0x00972970
    "AX.tim",
    // STRING: FF7 0x00972978
    "RM.TIM",
    // STRING: FF7 0x00972980
    "RN.TIM",
    // STRING: FF7 0x00972988
    "RO.TIM",
    // STRING: FF7 0x00972990
    "NULL.TIM",
    // STRING: FF7 0x0097299c
    "RP.TIM",
    // STRING: FF7 0x009729a4
    "BD.tim",
};
// GLOBAL: FF7 0x00972110
char *g_pChocoboHudTexNames22[10] = {
    // STRING: FF7 0x009729ac
    "BE.tim",
    // STRING: FF7 0x009729b4
    "BF.tim",
    // STRING: FF7 0x009729bc
    "BG.tim",
    // STRING: FF7 0x009729c4
    "BH.tim",
    // STRING: FF7 0x009729cc
    "RQ.TIM",
    // STRING: FF7 0x009729d4
    "RR.TIM",
    // STRING: FF7 0x009729dc
    "RS.TIM",
    // STRING: FF7 0x009729e4
    "NULL.TIM",
    // STRING: FF7 0x009729f0
    "RT.TIM",
    // STRING: FF7 0x009729f8
    "BN.tim",
};
// GLOBAL: FF7 0x00972138
char *g_pChocoboHudTexNames23[4] = {
    // STRING: FF7 0x00972a00
    "RU.tim",
    // STRING: FF7 0x00972a08
    "BP.tim",
    // STRING: FF7 0x00972a10
    "RV.tim",
    // STRING: FF7 0x00972a18
    "RW.tim",
};
// GLOBAL: FF7 0x00972148
char *g_pChocoboHudTexNames24[11] = {
    // STRING: FF7 0x00972a20
    "RX.TIM",
    // STRING: FF7 0x00972a28
    "RY.TIM",
    // STRING: FF7 0x00972a30
    "RZ.TIM",
    // STRING: FF7 0x00972a38
    "SA.TIM",
    // STRING: FF7 0x00972a40
    "BW.tim",
    // STRING: FF7 0x00972a48
    "SB.TIM",
    // STRING: FF7 0x00972a50
    "SC.TIM",
    // STRING: FF7 0x00972a58
    "SD.TIM",
    // STRING: FF7 0x00972a60
    "SE.TIM",
    // STRING: FF7 0x00972a68
    "CB.tim",
    // STRING: FF7 0x00972a70
    "CC.tim",
};
// GLOBAL: FF7 0x00972174
int g_ChocoboRetailFlag = 1;
// The HUD render objects; the code indexes past the 24 declared entries into the globals that follow
// (entries 24 and 26 are g_ChocoboHudRenderObject24 and g_ChocoboHudRenderObject26).
// GLOBAL: FF7 0x00e72990
struct ChocoboHudRenderObject g_ChocoboHudRenderObjects[24] = {0};
// GLOBAL: FF7 0x00e72c30
int g_ChocoboHudRenderObject24 = 0;
unsigned char g_bssPad_00e72c34[4] = {0};
unsigned char g_bssPad_00e72c38[8] = {0};
// GLOBAL: FF7 0x00e72c40
int g_ChocoboHudRenderObject24Active = 0;
// GLOBAL: FF7 0x00e72c44
int g_ChocoboHudRenderObject24SlotUsed = 0;
unsigned char g_bssPad_00e72c48[32] = {0};
// GLOBAL: FF7 0x00e72c68
int g_ChocoboHudRenderObject26 = 0;
unsigned char g_bssPad_00e72c6c[4] = {0};
unsigned char g_bssPad_00e72c70[8] = {0};
// GLOBAL: FF7 0x00e72c78
int g_ChocoboHudRenderObject26Active = 0;
// GLOBAL: FF7 0x00e72c7c
int g_ChocoboHudRenderObject26SlotUsed = 0;
// GLOBAL: FF7 0x00e72c80
int g_ChocoboHudRenderObject26Ready = 0;
unsigned char g_bssPad_00e72c84[4] = {0};
unsigned char g_bssPad_00e72c88[360] = {0};
// GLOBAL: FF7 0x00e72df0
int g_ChocoboHudObj = 0;
unsigned char g_bssPad_00e72df4[4] = {0};
unsigned char g_bssPad_00e72df8[8] = {0};
// GLOBAL: FF7 0x00e72e00
int g_ChocoboHudObjInUse = 0;
// GLOBAL: FF7 0x00e72e04
int g_ChocoboHudObjDirty = 0;
unsigned char g_bssPad_00e72e08[4] = {0};
// GLOBAL: FF7 0x00e72e0c
int g_ChocoboHudObjCount = 0;
// clang-format on

#pragma intrinsic(sqrt, atan2)

#pragma intrinsic(strcpy, strcat)

/*
 * Selects class, class-Z and common racing HUD texture tables from race class, scale and variant
 * globals, then creates render objects with per-group blend settings and initializes their metadata.
 */
// FUNCTION: FF7 0x00770800
void __cdecl Mini_ChocoboCreateHudRenderObjects(GfxEngineContext *engineResource, PfileLoadOptions *optionBuffer, int includeAll, int textureIndex)
{
    struct {
        unsigned short i;
        const char **classZ;
        int hasTransparentPair;
        int blendMode;
        const char **classTextures;
        const char **uiTextures;
        char path[0x100];
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    g_ChocoboHudObjCount = 0;
    switch (textureIndex) {
    case CHOCOBO_RACE_CLASS_C:
        if (g_ChocoboRetailFlag == 0) {
            if (g_ChocoboUiScalingMode == 2) {
                local.classTextures = (const char **)g_pChocoboHudTexNames3;
            } else {
                local.classTextures = (const char **)g_pChocoboHudTexNames15;
            }
        } else {
            if (g_ChocoboUiScalingMode == 2) {
                local.classTextures = (const char **)g_pChocoboHudTexNames9;
            } else {
                local.classTextures = (const char **)g_pChocoboHudTexNames21;
            }
        }
        local.hasTransparentPair = 0;
        break;
    case CHOCOBO_RACE_CLASS_B:
        if (g_ChocoboRetailFlag == 0) {
            if (g_ChocoboUiScalingMode == 2) {
                local.classTextures = (const char **)g_pChocoboHudTexNames2;
            } else {
                local.classTextures = (const char **)g_pChocoboHudTexNames14;
            }
        } else {
            if (g_ChocoboUiScalingMode == 2) {
                local.classTextures = (const char **)g_pChocoboHudTexNames8;
            } else {
                local.classTextures = (const char **)g_pChocoboHudTexNames20;
            }
        }
        local.hasTransparentPair = 1;
        break;
    case CHOCOBO_RACE_CLASS_A:
        if (g_ChocoboRetailFlag == 0) {
            if (g_ChocoboUiScalingMode == 2) {
                local.classTextures = (const char **)g_pChocoboHudTexNames1;
            } else {
                local.classTextures = (const char **)g_pChocoboHudTexNames13;
            }
        } else {
            if (g_ChocoboUiScalingMode == 2) {
                local.classTextures = (const char **)g_pChocoboHudTexNames7;
            } else {
                local.classTextures = (const char **)g_pChocoboHudTexNames19;
            }
        }
        local.hasTransparentPair = 1;
        break;
    case CHOCOBO_RACE_CLASS_S:
        if (g_ChocoboRetailFlag == 0) {
            if (g_ChocoboUiScalingMode == 2) {
                local.classTextures = (const char **)g_pChocoboHudTexNames4;
            } else {
                local.classTextures = (const char **)g_pChocoboHudTexNames16;
            }
        } else {
            if (g_ChocoboUiScalingMode == 2) {
                local.classTextures = (const char **)g_pChocoboHudTexNames10;
            } else {
                local.classTextures = (const char **)g_pChocoboHudTexNames22;
            }
        }
        local.hasTransparentPair = 0;
        break;
    }

    strcpy(local.path, g_ChocoboAssetDirPath);
    strcat(local.path, ((const char **)&g_pChocoboHudTexNames0)[textureIndex]);
    optionBuffer->directory = local.path;
    optionBuffer->flags |= 0x10;

    for (local.i = 0; local.i < 0xA; local.i++) {
        if (g_ChocoboUiScalingMode != 2 && local.i == 7) {
            g_ChocoboHudObjCount++;
            continue;
        }
        g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].readyFlag = 0;
        if ((local.i != 2 && local.i != 3) || !local.hasTransparentPair) {
            local.blendMode = BLEND_NONE;
            Pfile_InitLoadOptionsWithBlend(local.blendMode, optionBuffer);
            if (g_ChocoboRetailFlag) {
                optionBuffer->file.fromArchive = 1;
                optionBuffer->file.archiveId = LGP_ARCHIVE_CHOCOBO;
                optionBuffer->file.callback = 0;
            }
            strcpy(local.path, g_ChocoboAssetDirPath);
            strcat(local.path, ((const char **)&g_pChocoboHudTexNames0)[textureIndex]);
            optionBuffer->directory = local.path;
            optionBuffer->flags |= 0x10;
        } else {
            g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].readyFlag = 1;
            local.blendMode = BLEND_AVERAGE;
            Pfile_InitLoadOptionsWithBlend(local.blendMode, optionBuffer);
            if (g_ChocoboRetailFlag) {
                optionBuffer->file.fromArchive = 1;
                optionBuffer->file.archiveId = LGP_ARCHIVE_CHOCOBO;
                optionBuffer->file.callback = 0;
            }
            strcpy(local.path, g_ChocoboAssetDirPath);
            strcat(local.path, ((const char **)&g_pChocoboHudTexNames0)[textureIndex]);
            optionBuffer->directory = local.path;
            optionBuffer->flags |= 0x10;
        }
        g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].renderObj = (GfxRenderObject *)Gfx_CreateRenderObject(
            1, GFX_DRAW_TEXTURED_QUAD_2D, optionBuffer, (char *)local.classTextures[local.i], (void *)engineResource->renderObjectBank);
        g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].activeFlag = 1;
        g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].slotUsedFlag = 0;
        g_ChocoboHudObjCount++;
    }

    if (local.hasTransparentPair) {
        local.blendMode = BLEND_NONE;
        Pfile_InitLoadOptionsWithBlend(local.blendMode, optionBuffer);
        if (g_ChocoboRetailFlag) {
            optionBuffer->file.fromArchive = 1;
            optionBuffer->file.archiveId = LGP_ARCHIVE_CHOCOBO;
            optionBuffer->file.callback = 0;
        }
        strcpy(local.path, g_ChocoboAssetDirPath);
        strcat(local.path, ((const char **)&g_pChocoboHudTexNames0)[textureIndex]);
        optionBuffer->directory = local.path;
        optionBuffer->flags |= 0x10;
        g_ChocoboHudRenderObject26Ready = 0;
        g_ChocoboHudRenderObject26 = (int)Gfx_CreateRenderObject(
            1, GFX_DRAW_TEXTURED_QUAD_2D, optionBuffer, (char *)local.classTextures[3], (void *)engineResource->renderObjectBank);
        g_ChocoboHudRenderObject26Active = 1;
        g_ChocoboHudRenderObject26SlotUsed = 0;
    }

    strcpy(local.path, g_ChocoboAssetDirPath);
    strcat(local.path, g_pChocoboHudTexNames0[4]);
    optionBuffer->directory = local.path;
    optionBuffer->flags |= 0x10;
    if (g_ChocoboRetailFlag == 0) {
        if (g_ChocoboUiScalingMode == 2) {
            local.classZ = (const char **)g_pChocoboHudTexNames5;
        } else {
            local.classZ = (const char **)g_pChocoboHudTexNames17;
        }
    } else {
        if (g_ChocoboUiScalingMode == 2) {
            local.classZ = (const char **)g_pChocoboHudTexNames11;
        } else {
            local.classZ = (const char **)g_pChocoboHudTexNames23;
        }
    }
    for (local.i = 0; local.i < 4; local.i++) {
        g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].readyFlag = 0;
        g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].renderObj = (GfxRenderObject *)Gfx_CreateRenderObject(
            1, GFX_DRAW_TEXTURED_QUAD_2D, optionBuffer, (char *)local.classZ[local.i], (void *)engineResource->renderObjectBank);
        g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].activeFlag = 1;
        g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].slotUsedFlag = 0;
        g_ChocoboHudObjCount++;
    }

    strcpy(local.path, g_ChocoboAssetDirPath);
    strcat(local.path, g_pChocoboHudTexNames0[5]);
    optionBuffer->directory = local.path;
    optionBuffer->flags |= 0x10;
    if (g_ChocoboRetailFlag == 0) {
        if (g_ChocoboUiScalingMode == 2) {
            local.uiTextures = (const char **)g_pChocoboHudTexNames6;
        } else {
            local.uiTextures = (const char **)g_pChocoboHudTexNames18;
        }
    } else {
        if (g_ChocoboUiScalingMode == 2) {
            local.uiTextures = (const char **)g_pChocoboHudTexNames12;
        } else {
            local.uiTextures = (const char **)g_pChocoboHudTexNames24;
        }
    }
    for (local.i = 0; local.i < 0xB; local.i++) {
        if (local.i == 4) {
            g_ChocoboHudObjCount++;
            continue;
        }
        g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].readyFlag = 0;
        if (g_ChocoboCourseVariant == 0 && local.i == 6) {
            continue;
        }
        if (g_ChocoboCourseVariant == 1 && local.i == 5) {
            continue;
        }
        if (local.i != 6 && local.i != 5) {
            local.blendMode = BLEND_NONE;
            Pfile_InitLoadOptionsWithBlend(local.blendMode, optionBuffer);
            if (g_ChocoboRetailFlag) {
                optionBuffer->file.fromArchive = 1;
                optionBuffer->file.archiveId = LGP_ARCHIVE_CHOCOBO;
                optionBuffer->file.callback = 0;
            }
            strcpy(local.path, g_ChocoboAssetDirPath);
            strcat(local.path, g_pChocoboHudTexNames0[5]);
            optionBuffer->directory = local.path;
            optionBuffer->flags |= 0x10;
        } else {
            g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].readyFlag = 1;
            local.blendMode = BLEND_AVERAGE;
            Pfile_InitLoadOptionsWithBlend(local.blendMode, optionBuffer);
            if (g_ChocoboRetailFlag) {
                optionBuffer->file.fromArchive = 1;
                optionBuffer->file.archiveId = LGP_ARCHIVE_CHOCOBO;
                optionBuffer->file.callback = 0;
            }
            strcpy(local.path, g_ChocoboAssetDirPath);
            strcat(local.path, g_pChocoboHudTexNames0[5]);
            optionBuffer->directory = local.path;
            optionBuffer->flags |= 0x10;
        }
        if (local.i < 7 || includeAll != 0) {
            if (local.i == 0xA || local.i == 9) {
                if (g_ChocoboUiScalingMode == 2) {
                    optionBuffer->renderFlags |= 0x40;
                } else {
                    optionBuffer->renderFlags |= 0x20;
                }
            }
            if (local.i == 8 || local.i == 7) {
                optionBuffer->renderFlags |= 0x20;
            }
            g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].renderObj = (GfxRenderObject *)Gfx_CreateRenderObject(
                1, GFX_DRAW_TEXTURED_QUAD_2D, optionBuffer, (char *)local.uiTextures[local.i], (void *)engineResource->renderObjectBank);
            g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].activeFlag = 1;
            g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].slotUsedFlag = 0;
            g_ChocoboHudObjCount++;
        }
    }

    g_ChocoboHudObjCount = 0x18;
    g_ChocoboHudRenderObject24 = (int)Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, optionBuffer, 0, (void *)engineResource->renderObjectBank);
    g_ChocoboHudRenderObject24Active = 1;
    g_ChocoboHudRenderObject24SlotUsed = 0;
    g_ChocoboHudObjCount++;

    local.blendMode = BLEND_AVERAGE;
    Pfile_InitLoadOptionsWithBlend(local.blendMode, optionBuffer);
    optionBuffer->renderFlags |= 0x4000;
    if (g_ChocoboRetailFlag) {
        optionBuffer->file.fromArchive = 1;
        optionBuffer->file.archiveId = LGP_ARCHIVE_CHOCOBO;
        optionBuffer->file.callback = 0;
    }
    g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].renderObj = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, optionBuffer, 0, (void *)engineResource->renderObjectBank);
    g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].activeFlag = 1;
    g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].slotUsedFlag = 0;
    g_ChocoboHudRenderObjects[g_ChocoboHudObjCount].readyFlag = 1;
    g_ChocoboHudObjCount++;

    if (g_ChocoboUiContext) {
        local.blendMode = BLEND_ADDITIVE;
        Pfile_InitLoadOptionsWithBlend(local.blendMode, optionBuffer);
        optionBuffer->renderFlags |= 0x4000;
        g_ChocoboHudObj = (int)Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, optionBuffer, 0, (void *)engineResource->renderObjectBank);
        g_ChocoboHudObjDirty = 0;
        g_ChocoboHudObjInUse = 1;
    }
}
/*
 * Tears down the HUD render objects table and additional HUD render objects.
 */
// FUNCTION: FF7 0x007714e9
void __cdecl Mini_ChocoboDestroyHudRenderObjects(void)
{
    unsigned short i;

    for (i = 0; i < 0x28; i++) {
        if (g_ChocoboHudRenderObjects[i].activeFlag != 0) {
            DestroyRenderObject(g_ChocoboHudRenderObjects[i].renderObj);
            g_ChocoboHudRenderObjects[i].renderObj = 0;
            g_ChocoboHudRenderObjects[i].activeFlag = 0;
        }
    }
    if (g_ChocoboUiContext != 0 && g_ChocoboHudObjInUse != 0) {
        DestroyRenderObject((void *)g_ChocoboHudObj);
        g_ChocoboHudObj = 0;
        g_ChocoboHudObjInUse = 0;
    }
}
/*
 * Flushes dirty primitive batches on in-use HUD render objects and clears their dirty flags.
 */
// FUNCTION: FF7 0x007715a4
void __cdecl Mini_ChocoboEmptyDirtyHudRenderObjectBatches(void)
{
    unsigned short i;

    for (i = 0; i < 0x28; i++) {
        if (g_ChocoboHudRenderObjects[i].activeFlag != 0 && g_ChocoboHudRenderObjects[i].slotUsedFlag != 0) {
            Gfx_EmptyRenderObjectBatch((int)g_ChocoboHudRenderObjects[i].renderObj);
            g_ChocoboHudRenderObjects[i].slotUsedFlag = 0;
        }
    }
    if (g_ChocoboUiContext != 0 && g_ChocoboHudObjDirty != 0) {
        Gfx_EmptyRenderObjectBatch(g_ChocoboHudObj);
        g_ChocoboHudObjDirty = 0;
    }
}
} /* extern "C" */
