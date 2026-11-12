// src/ff7/chocobo/ch_chr.cpp
// Original: C:\FF7\chocobo\ch_chr.cpp
// Contents: Racer and course sprite objects for the chocobo race: builds and frees the racer
//   models, skeletons and animation arrays, and creates, updates and submits the course sprite
//   render objects.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include <string.h>
#include <stdlib.h>
#include "lib/pfile.h"
#include "lib/types/polygon.h"
#include "lib/enums/graphics.h"
#include "ff7/types/chocobo.h"
#include "lib/types/graphics.h"
#include "ff7/api/chocobo.h"
#include "lib/api/polygon.h"
#include <math.h>
#include "ff7/gte.h"
#include "lib/api/graphics.h"
extern "C" {

struct GfxEngineContext;
struct PolygonAnimation;
#pragma intrinsic(strcpy, strcat)
// clang-format on
typedef struct ChocoboAssetPrefix {
    unsigned short animationCount, pad_02;
    char *model;
    char *animations[6];
} ChocoboAssetPrefix;
typedef struct ChocoboAssetRecord {
    ChocoboAssetPrefix prefix;
    unsigned int pad_20[4];
} ChocoboAssetRecord;
typedef struct ChocoboAssetMinimum {
    ChocoboAssetRecord records[38];
    ChocoboAssetPrefix last;
    unsigned int last_pad_20[4];
} ChocoboAssetMinimum;
// The two 39-record chocobo asset tables (the 39th record's last 16 bytes zero: the table's grown tail);
// their names are their initializer literals, before the functions' __FILE__ literals.
// clang-format off
// GLOBAL: FF7 0x0096ebe0
ChocoboAssetMinimum g_ChocoboRacerAsset0 = {
    {
        {{6, 0,
          // STRING: FF7 0x009704b8
          "data\\chr\\chocobo\\rsd\\chocobo.hrc",
          {
           // STRING: FF7 0x009704dc
           "data\\chr\\chocobo\\anm\\run.anm",
           // STRING: FF7 0x009704fc
           "data\\chr\\chocobo\\anm\\turbo.anm",
           // STRING: FF7 0x0097051c
           "data\\chr\\chocobo\\anm\\win.anm",
           // STRING: FF7 0x0097053c
           "data\\chr\\chocobo\\anm\\draw.anm",
           // STRING: FF7 0x0097055c
           "data\\chr\\chocobo\\anm\\lose.anm",
           // STRING: FF7 0x0097057c
           "data\\chr\\chocobo\\anm\\tired.anm",
          }}, {0, 0, 0, 0}},
        {{5, 0,
          // STRING: FF7 0x0097059c
          "data\\chr\\metman\\rsd\\metman.hrc",
          {
           // STRING: FF7 0x009705bc
           "data\\chr\\metman\\anm\\run.anm",
           // STRING: FF7 0x009705d8
           "data\\chr\\metman\\anm\\turbo.anm",
           // STRING: FF7 0x009705f8
           "data\\chr\\cloud\\anm\\win.anm",
           // STRING: FF7 0x00970614
           "data\\chr\\metman\\anm\\draw.anm",
           // STRING: FF7 0x00970634
           "data\\chr\\metman\\anm\\lose.anm",
           0,
          }}, {0, 0, 0, 0}},
        {{6, 0,
          // STRING: FF7 0x00970654
          "data\\chr\\cloud\\rsd\\cloud.hrc",
          {
           // STRING: FF7 0x00970674
           "data\\chr\\cloud\\anm\\run.anm",
           // STRING: FF7 0x00970690
           "data\\chr\\cloud\\anm\\turbo.anm",
           // STRING: FF7 0x009706b0
           "data\\chr\\cloud\\anm\\win.anm",
           // STRING: FF7 0x009706cc
           "data\\chr\\cloud\\anm\\draw.anm",
           // STRING: FF7 0x009706e8
           "data\\chr\\cloud\\anm\\lose.anm",
           // STRING: FF7 0x00970704
           "data\\chr\\cloud\\anm\\tired.anm",
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970724
          "data\\chr\\aurora\\rsd\\aurora.hrc",
          {
           // STRING: FF7 0x00970744
           "data\\chr\\aurora\\anm\\aurora.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970764
          "data\\chr\\ufo\\rsd\\ufo.hrc",
          {
           // STRING: FF7 0x00970780
           "data\\chr\\ufo\\anm\\ufo.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x0097079c
          "data\\chr\\conya\\rsd\\conya.hrc",
          {
           // STRING: FF7 0x009707bc
           "data\\chr\\conya\\anm\\conya.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x009707dc
          "data\\chr\\canoe\\rsd\\canoe.hrc",
          {
           // STRING: FF7 0x009707fc
           "data\\chr\\canoe\\anm\\canoe.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x0097081c
          "data\\chr\\dia\\rsd\\dia1.hrc",
          {
           // STRING: FF7 0x00970838
           "data\\chr\\dia\\anm\\dia1.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970854
          "data\\chr\\dia\\rsd\\dia2.hrc",
          {
           // STRING: FF7 0x00970870
           "data\\chr\\dia\\anm\\dia2.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x0097088c
          "data\\chr\\dia\\rsd\\dia3.hrc",
          {
           // STRING: FF7 0x009708a8
           "data\\chr\\dia\\anm\\dia3.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x009708c4
          "data\\chr\\dia\\rsd\\dia4.hrc",
          {
           // STRING: FF7 0x009708e0
           "data\\chr\\dia\\anm\\dia4.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x009708fc
          "data\\chr\\dia\\rsd\\dia5.hrc",
          {
           // STRING: FF7 0x00970918
           "data\\chr\\dia\\anm\\dia5.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970934
          "data\\chr\\dia\\rsd\\dia6.hrc",
          {
           // STRING: FF7 0x00970950
           "data\\chr\\dia\\anm\\dia6.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x0097096c
          "data\\chr\\dia\\rsd\\dia7.hrc",
          {
           // STRING: FF7 0x00970988
           "data\\chr\\dia\\anm\\dia7.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x009709a4
          "data\\chr\\goal\\rsd\\goal.hrc",
          {
           // STRING: FF7 0x009709c0
           "data\\chr\\goal\\anm\\goal.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x009709dc
          "data\\chr\\goal\\rsd\\goalbar.hrc",
          {
           // STRING: FF7 0x009709fc
           "data\\chr\\goal\\anm\\goalbar.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970a1c
          "data\\chr\\star\\rsd\\star.hrc",
          {
           // STRING: FF7 0x00970a38
           "data\\chr\\star\\anm\\star.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970a54
          "data\\chr\\shell1\\rsd\\shell1.hrc",
          {
           // STRING: FF7 0x00970a74
           "data\\chr\\shell1\\anm\\shell1.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970a94
          "data\\chr\\shell2\\rsd\\shell2.hrc",
          {
           // STRING: FF7 0x00970ab4
           "data\\chr\\shell2\\anm\\shell2.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970ad4
          "data\\chr\\shell1\\rsd\\pearl1.hrc",
          {
           // STRING: FF7 0x00970af4
           "data\\chr\\shell1\\anm\\pearl1.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970b14
          "data\\chr\\shell2\\rsd\\pearl2.hrc",
          {
           // STRING: FF7 0x00970b34
           "data\\chr\\shell2\\anm\\pearl2.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970b54
          "data\\chr\\hane\\rsd\\hane.hrc",
          {
           // STRING: FF7 0x00970b70
           "data\\chr\\hane\\anm\\hane.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970b8c
          "data\\chr\\hane\\rsd\\hanebig.hrc",
          {
           // STRING: FF7 0x00970bac
           "data\\chr\\hane\\anm\\hanebig.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970bcc
          "data\\chr\\ice\\rsd\\ice1.hrc",
          {
           // STRING: FF7 0x00970be8
           "data\\chr\\ice\\anm\\ice1.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970c04
          "data\\chr\\ice\\rsd\\ice2.hrc",
          {
           // STRING: FF7 0x00970c20
           "data\\chr\\ice\\anm\\ice2.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970c3c
          "data\\chr\\ice\\rsd\\ice3.hrc",
          {
           // STRING: FF7 0x00970c58
           "data\\chr\\ice\\anm\\ice3.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970c74
          "data\\chr\\shark\\rsd\\shark.hrc",
          {
           // STRING: FF7 0x00970c94
           "data\\chr\\shark\\anm\\shark1.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970cb4
          "data\\chr\\shark\\rsd\\shark.hrc",
          {
           // STRING: FF7 0x00970cd4
           "data\\chr\\shark\\anm\\shark2.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970cf4
          "data\\chr\\fla\\rsd\\fla.hrc",
          {
           // STRING: FF7 0x00970d10
           "data\\chr\\fla\\anm\\fla.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970d2c
          "data\\chr\\thunder\\rsd\\thunderl.hrc",
          {
           // STRING: FF7 0x00970d50
           "data\\chr\\thunder\\anm\\thunderl.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970d74
          "data\\chr\\thunder\\rsd\\thunderr.hrc",
          {
           // STRING: FF7 0x00970d98
           "data\\chr\\thunder\\anm\\thunderr.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970dbc
          "data\\chr\\fuusha\\rsd\\fuusha.hrc",
          {
           // STRING: FF7 0x00970ddc
           "data\\chr\\fuusha\\anm\\fuusha.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970dfc
          "data\\chr\\taki\\rsd\\taki_r.hrc",
          {
           // STRING: FF7 0x00970e1c
           "data\\chr\\taki\\anm\\taki.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970e38
          "data\\chr\\fuusha\\rsd\\fuusha.hrc",
          {
           // STRING: FF7 0x00970e58
           "data\\chr\\fuusha\\anm\\fuusha.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970e78
          "data\\chr\\morgly\\rsd\\morgly.hrc",
          {
           // STRING: FF7 0x00970e98
           "data\\chr\\morgly\\anm\\win.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00970eb4
          "data\\chr\\tobira\\rsd\\tobira.hrc",
          {
           // STRING: FF7 0x00970ed4
           "data\\chr\\tobira\\anm\\tobira.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{5, 0,
          // STRING: FF7 0x00970ef4
          "data\\chr\\metman1\\rsd\\metman1.hrc",
          {
           // STRING: FF7 0x00970f18
           "data\\chr\\cloud\\anm\\run.anm",
           // STRING: FF7 0x00970f34
           "data\\chr\\metman\\anm\\turbo.anm",
           // STRING: FF7 0x00970f54
           "data\\chr\\metman\\anm\\win.anm",
           // STRING: FF7 0x00970f70
           "data\\chr\\metman\\anm\\draw.anm",
           // STRING: FF7 0x00970f90
           "data\\chr\\metman\\anm\\lose.anm",
           0,
          }}, {0, 0, 0, 0}},
        {{6, 0,
          // STRING: FF7 0x00970fb0
          "data\\chr\\cid\\rsd\\cid.hrc",
          {
           // STRING: FF7 0x00970fcc
           "data\\chr\\cloud\\anm\\run.anm",
           // STRING: FF7 0x00970fe8
           "data\\chr\\cloud\\anm\\turbo.anm",
           // STRING: FF7 0x00971008
           "data\\chr\\cloud\\anm\\win.anm",
           // STRING: FF7 0x00971024
           "data\\chr\\cloud\\anm\\draw.anm",
           // STRING: FF7 0x00971040
           "data\\chr\\cloud\\anm\\lose.anm",
           // STRING: FF7 0x0097105c
           "data\\chr\\cloud\\anm\\tired.anm",
          }}, {0, 0, 0, 0}},
    },
    {6, 0,
      // STRING: FF7 0x0097107c
      "data\\chr\\tifa\\rsd\\tifa.hrc",
      {
       // STRING: FF7 0x00971098
       "data\\chr\\cloud\\anm\\run.anm",
       // STRING: FF7 0x009710b4
       "data\\chr\\cloud\\anm\\turbo.anm",
       // STRING: FF7 0x009710d4
       "data\\chr\\tifa\\anm\\win.anm",
       // STRING: FF7 0x009710f0
       "data\\chr\\cloud\\anm\\draw.anm",
       // STRING: FF7 0x0097110c
       "data\\chr\\cloud\\anm\\lose.anm",
       // STRING: FF7 0x00971128
       "data\\chr\\cloud\\anm\\tired.anm",
      }}};

// GLOBAL: FF7 0x0096f330
ChocoboAssetMinimum g_ChocoboRacerAsset1 = {
    {
        {{6, 0,
          // STRING: FF7 0x00971148
          "CD.hrc",
          {
           // STRING: FF7 0x00971150
           "DB.anm",
           // STRING: FF7 0x00971158
           "DC.anm",
           // STRING: FF7 0x00971160
           "DD.anm",
           // STRING: FF7 0x00971168
           "DE.anm",
           // STRING: FF7 0x00971170
           "DF.anm",
           // STRING: FF7 0x00971178
           "DG.anm",
          }}, {0, 0, 0, 0}},
        {{5, 0,
          // STRING: FF7 0x00971180
          "DH.hrc",
          {
           // STRING: FF7 0x00971188
           "DV.anm",
           // STRING: FF7 0x00971190
           "DW.anm",
           // STRING: FF7 0x00971198
           "DX.anm",
           // STRING: FF7 0x009711a0
           "DY.anm",
           // STRING: FF7 0x009711a8
           "DZ.anm",
           0,
          }}, {0, 0, 0, 0}},
        {{6, 0,
          // STRING: FF7 0x009711b0
          "EA.hrc",
          {
           // STRING: FF7 0x009711b8
           "EO.anm",
           // STRING: FF7 0x009711c0
           "EP.anm",
           // STRING: FF7 0x009711c8
           "EQ.anm",
           // STRING: FF7 0x009711d0
           "ER.anm",
           // STRING: FF7 0x009711d8
           "ES.anm",
           // STRING: FF7 0x009711e0
           "ET.anm",
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x009711e8
          "EU.hrc",
          {
           // STRING: FF7 0x009711f0
           "EY.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x009711f8
          "EZ.hrc",
          {
           // STRING: FF7 0x00971200
           "FH.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971208
          "FI.hrc",
          {
           // STRING: FF7 0x00971210
           "GJ.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971218
          "GK.hrc",
          {
           // STRING: FF7 0x00971220
           "GV.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971228
          "GW.hrc",
          {
           // STRING: FF7 0x00971230
           "GY.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971238
          "GZ.hrc",
          {
           // STRING: FF7 0x00971240
           "HB.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971248
          "HC.hrc",
          {
           // STRING: FF7 0x00971250
           "HE.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971258
          "HF.hrc",
          {
           // STRING: FF7 0x00971260
           "HH.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971268
          "HI.hrc",
          {
           // STRING: FF7 0x00971270
           "HK.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971278
          "HL.hrc",
          {
           // STRING: FF7 0x00971280
           "HN.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971288
          "HO.hrc",
          {
           // STRING: FF7 0x00971290
           "HQ.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971298
          "HR.hrc",
          {
           // STRING: FF7 0x009712a0
           "IA.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x009712a8
          "IB.hrc",
          {
           // STRING: FF7 0x009712b0
           "IE.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x009712b8
          "IF.hrc",
          {
           // STRING: FF7 0x009712c0
           "IK.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x009712c8
          "IL.hrc",
          {
           // STRING: FF7 0x009712d0
           "IP.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x009712d8
          "IQ.hrc",
          {
           // STRING: FF7 0x009712e0
           "IU.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x009712e8
          "IV.hrc",
          {
           // STRING: FF7 0x009712f0
           "IX.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x009712f8
          "IY.hrc",
          {
           // STRING: FF7 0x00971300
           "JA.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971308
          "JB.hrc",
          {
           // STRING: FF7 0x00971310
           "JE.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971318
          "JF.hrc",
          {
           // STRING: FF7 0x00971320
           "JK.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971328
          "JL.hrc",
          {
           // STRING: FF7 0x00971330
           "JN.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971338
          "JO.hrc",
          {
           // STRING: FF7 0x00971340
           "JQ.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971348
          "JR.hrc",
          {
           // STRING: FF7 0x00971350
           "JT.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971358
          "JU.hrc",
          {
           // STRING: FF7 0x00971360
           "KD.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971368
          "KE.hrc",
          {
           // STRING: FF7 0x00971370
           "KN.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971378
          "KO.hrc",
          {
           // STRING: FF7 0x00971380
           "KQ.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971388
          "data\\chr\\thunder\\rsd\\thunderl.hrc",
          {
           // STRING: FF7 0x009713ac
           "data\\chr\\thunder\\anm\\thunderl.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x009713d0
          "data\\chr\\thunder\\rsd\\thunderr.hrc",
          {
           // STRING: FF7 0x009713f4
           "data\\chr\\thunder\\anm\\thunderr.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971418
          "KR.hrc",
          {
           // STRING: FF7 0x00971420
           "KT.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971428
          "KU.hrc",
          {
           // STRING: FF7 0x00971430
           "KW.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971438
          "KX.hrc",
          {
           // STRING: FF7 0x00971440
           "KZ.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971448
          "LA.hrc",
          {
           // STRING: FF7 0x00971450
           "LJ.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{1, 0,
          // STRING: FF7 0x00971458
          "LK.hrc",
          {
           // STRING: FF7 0x00971460
           "LO.anm",
           0,
           0,
           0,
           0,
           0,
          }}, {0, 0, 0, 0}},
        {{5, 0,
          // STRING: FF7 0x00971468
          "LP.hrc",
          {
           // STRING: FF7 0x00971470
           "MD.anm",
           // STRING: FF7 0x00971478
           "ME.anm",
           // STRING: FF7 0x00971480
           "MF.anm",
           // STRING: FF7 0x00971488
           "MG.anm",
           // STRING: FF7 0x00971490
           "MH.anm",
           0,
          }}, {0, 0, 0, 0}},
        {{6, 0,
          // STRING: FF7 0x00971498
          "MI.hrc",
          {
           // STRING: FF7 0x009714a0
           "MW.anm",
           // STRING: FF7 0x009714a8
           "MX.anm",
           // STRING: FF7 0x009714b0
           "MY.anm",
           // STRING: FF7 0x009714b8
           "MZ.anm",
           // STRING: FF7 0x009714c0
           "NA.anm",
           // STRING: FF7 0x009714c8
           "NB.anm",
          }}, {0, 0, 0, 0}},
    },
    {6, 0,
      // STRING: FF7 0x009714d0
      "NC.hrc",
      {
       // STRING: FF7 0x009714d8
       "NQ.anm",
       // STRING: FF7 0x009714e0
       "NR.anm",
       // STRING: FF7 0x009714e8
       "NS.anm",
       // STRING: FF7 0x009714f0
       "NT.anm",
       // STRING: FF7 0x009714f8
       "NU.anm",
       // STRING: FF7 0x00971500
       "NV.anm",
      }}};
// GLOBAL: FF7 0x0096fa80
int g_ChocoboSpriteTimings0[31][2] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {2, 0},
    {37, 0},
    {38, 0},
    {36, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {34, 0},
    {32, 1},
    {35, 0},
    {31, 0},
    {28, 1},
    {23, 1},
    {26, 0},
    {27, 0},
    {24, 1},
    {25, 1},
    {14, 0},
    {15, 1},
    {3, 1},
    {5, 0},
    {5, 0},
};
// GLOBAL: FF7 0x0096fb78
int g_ChocoboSpriteTimings1[35][2] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {2, 0},
    {37, 0},
    {38, 0},
    {36, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {34, 0},
    {17, 0},
    {19, 1},
    {18, 0},
    {20, 1},
    {21, 0},
    {22, 0},
    {16, 0},
    {7, 1},
    {8, 1},
    {9, 1},
    {10, 1},
    {11, 1},
    {12, 1},
    {13, 1},
    {6, 0},
    {14, 0},
    {15, 1},
    {4, 1},
};
// clang-format on
// The five __FILE__ literals of the two functions below come after the asset tables' names and before
// the model tables' (retail 0x971508..0x971594): parse-time units, each returned by a static __inline
// function that /Ob1 expands to the plain operand.
static __inline char *ChChrCpp_FileLine8B(void)
{
    // STRING: FF7 0x00971508
    return "C:\\FF7\\chocobo\\ch_chr.cpp";
}
static __inline char *ChChrCpp_FileLine99(void)
{
    // STRING: FF7 0x00971524
    return "C:\\FF7\\chocobo\\ch_chr.cpp";
}
static __inline char *ChChrCpp_FileLine9E(void)
{
    // STRING: FF7 0x00971540
    return "C:\\FF7\\chocobo\\ch_chr.cpp";
}
static __inline char *ChChrCpp_FileLineBB(void)
{
    // STRING: FF7 0x0097155c
    return "C:\\FF7\\chocobo\\ch_chr.cpp";
}
static __inline char *ChChrCpp_FileLineFB(void)
{
    // STRING: FF7 0x00971578
    return "C:\\FF7\\chocobo\\ch_chr.cpp";
}

/*
 * Frees the racer table and associated models, skeletons, and animation arrays.
 */
// FUNCTION: FF7 0x0076eff0
int __cdecl Mini_ChocoboFreeRacers(void)
{
    int racerIndex;
    int animIndex;

    if (g_pChocoboRacerTable != 0) {
        for (racerIndex = 0; racerIndex < g_ChocoboRacerCounts[0]; racerIndex++) {
            if (g_pChocoboRacerTable[racerIndex].animPlayer != 0) {
                if (g_pChocoboRacerTable[racerIndex].player->skeleton != 0) {
                    Skel_Free((PolygonSkelHierarchy *)(g_pChocoboRacerTable[racerIndex].player->skeleton));
                }
                GameHeapFree((void *)g_pChocoboRacerTable[racerIndex].animPlayer, ChChrCpp_FileLine8B(), 0x8B);
                g_pChocoboRacerTable[racerIndex].animPlayer = 0;
            }
            for (animIndex = 0; animIndex < g_pChocoboRacerTable[racerIndex].animCount; animIndex++) {
                if (g_pChocoboRacerTable[racerIndex].animTable != 0 && ((int *)(g_pChocoboRacerTable[racerIndex].animTable))[animIndex] != 0) {
                    Anim_Free((PolygonAnimBuffers *)(((int *)(g_pChocoboRacerTable[racerIndex].animTable))[animIndex]));
                    ((int *)(g_pChocoboRacerTable[racerIndex].animTable))[animIndex] = 0;
                }
            }
            if (g_pChocoboRacerTable[racerIndex].animTable != 0) {
                GameHeapFree((void *)g_pChocoboRacerTable[racerIndex].animTable, ChChrCpp_FileLine99(), 0x99);
                g_pChocoboRacerTable[racerIndex].animTable = 0;
            }
        }
        GameHeapFree(g_pChocoboRacerTable, ChChrCpp_FileLine9E(), 0x9E);
        g_pChocoboRacerTable = 0;
    }
    return 1;
}
/*
 * Allocates and builds racer model objects, animations, and render structures for the race.
 */
// FUNCTION: FF7 0x0076f1b0
int __cdecl Mini_ChocoboBuildRaceObjects(PfileLoadOptions *options, GfxEngineContext *engine, int variant)
{
    char basePath[256];
    char dirName[256];
    int selRacerCount;
    ChocoboRacerEntry *racerPointer;
    unsigned short currIndex;
    unsigned short animId;
    char extension[256];
    char dirPath[256];
    int(*x)[2];
    char tempStem[256];
    char z[4];
    if (variant == 0) {
        selRacerCount = 31;
        x = g_ChocoboSpriteTimings0;
    } else {
        selRacerCount = 35;
        x = g_ChocoboSpriteTimings1;
    }
    g_ChocoboRacerCounts[0] = selRacerCount;
    g_ChocoboRacerCounts[1] = 0;
    g_pChocoboRacerTable = (ChocoboRacerEntry *)GameHeapAllocRaw(g_ChocoboRacerCounts[0] * 60, ChChrCpp_FileLineBB(), 0xbb);
    if (g_pChocoboRacerTable == 0)
        return 0;
    if (g_ChocoboRetailFlag != 0) {
        options->file.fromArchive = 1;
        options->file.archiveId = 7;
        options->file.callback = 0;
    }
    for (currIndex = 0; currIndex < g_ChocoboRacerCounts[0]; currIndex++) {
        racerPointer = &g_pChocoboRacerTable[currIndex];
        racerPointer->posX = 0;
        racerPointer->posY = 0;
        racerPointer->posZ = 0;
        racerPointer->colorB = 128;
        racerPointer->colorG = 128;
        racerPointer->colorR = 128;
        racerPointer->scale = 640;
        if (x[currIndex][1] == 1)
            racerPointer->additive = 1;
        else
            racerPointer->additive = 0;
        if (racerPointer->additive == 0) {
            Pfile_InitLoadOptions(options);
            if (g_ChocoboRetailFlag != 0) {
                options->file.fromArchive = 1;
                options->file.archiveId = 7;
                options->file.callback = 0;
            }
            options->flags |= 0x2100000;
            Pfile_SetLoadOptionsBlendMode(BLEND_NONE, options);
        } else {
            Pfile_InitLoadOptions(options);
            if (g_ChocoboRetailFlag != 0) {
                options->file.fromArchive = 1;
                options->file.archiveId = 7;
                options->file.callback = 0;
            }
            options->flags |= 0x2100000;
            Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, options);
        }
        options->scale = (float)racerPointer->scale / 4096.0f * 30.4309f;
        racerPointer->modelIndex = currIndex;
        racerPointer->animCount = ((ChocoboRacerAssetEntry *)&g_ChocoboRacerAsset0)[x[currIndex][0]].animCount;
        if (currIndex >= 16 || currIndex < 6)
            racerPointer->noGroundOffset = 1;
        else
            racerPointer->noGroundOffset = 0;
        racerPointer->drawPending = 0;
        racerPointer->syncEnabled = 1;
        racerPointer->animId = 0;
        racerPointer->prevAnimId = 0;
        racerPointer->headingFlipped = 0;
        if (variant != 0 && (currIndex == 17 || currIndex == 19 || currIndex == 22 || currIndex == 31))
            racerPointer->headingFlipped = 1;
        racerPointer->animTable = (int)GameHeapAllocRaw(racerPointer->animCount * 4, ChChrCpp_FileLineFB(), 0xfb);
        for (animId = 0; animId < racerPointer->animCount; animId++) {
            strcpy(basePath, g_ChocoboAssetDirPath);
            if (g_ChocoboRetailFlag == 0)
                strcat(basePath, ((ChocoboRacerAssetEntry *)&g_ChocoboRacerAsset0)[x[currIndex][0]].animPaths[animId]);
            else
                strcat(basePath, ((ChocoboRacerAssetEntry *)&g_ChocoboRacerAsset1)[x[currIndex][0]].animPaths[animId]);
            ((int *)racerPointer->animTable)[animId] = Mini_ChocoboCompileAndLoadAnimation(options, (int)basePath);
            Anim_ScaleRootTranslation((float)racerPointer->scale / 4096.0f * 30.4309f, (PolygonAnimation *)(((int *)racerPointer->animTable)[animId]));
        }
        if (racerPointer->animCount > 0) {
            strcpy(basePath, g_ChocoboAssetDirPath);
            if (g_ChocoboRetailFlag == 0)
                strcat(basePath, ((ChocoboRacerAssetEntry *)&g_ChocoboRacerAsset0)[x[currIndex][0]].pathSuffix);
            else
                strcat(basePath, ((ChocoboRacerAssetEntry *)&g_ChocoboRacerAsset1)[x[currIndex][0]].pathSuffix);
            _splitpath(basePath, z, dirPath, tempStem, extension);
            strcpy(dirName, z);
            strcat(dirName, dirPath);
            racerPointer->animPlayer = Mini_ChocoboLoadHrcAndCreateAnimPlayer(30, **(int **)&racerPointer->animTable, (int)options, engine, (int)basePath, (int)dirName);
            for (animId = 0; animId < racerPointer->animCount; animId++)
                Anim_BakeAllFrameMatrices((PolygonAnimBake *)(((int *)racerPointer->animTable)[animId]), (PolygonAnimModel *)racerPointer->player->skeleton);
            Mini_ChocoboSelectAnimationIndex((struct ChocoboRacerEntry *)(char *)racerPointer, 0);
            if (racerPointer->animPlayer == 0)
                return 0;
            **(int **)&racerPointer->animPlayer = 1;
        }
    }
    return 1;
}

// The complete descriptor pointer targets have separate original identities.

// clang-format off
// The four seven-name and four thirty-six-name model tables; their TIM
// names are their initializer literals, after the __FILE__ literals of the functions above.
// GLOBAL: FF7 0x0096fc90
char *g_pChocoboSpriteNames0[7] = {
    // STRING: FF7 0x00971594
    "FIRE.TIM",
    // STRING: FF7 0x009715a0
    "HANA1.TIM",
    // STRING: FF7 0x009715ac
    "HANA2.TIM",
    // STRING: FF7 0x009715b8
    "HANA3.TIM",
    // STRING: FF7 0x009715c4
    "HANA4.TIM",
    // STRING: FF7 0x009715d0
    "KOTORI.TIM",
    // STRING: FF7 0x009715dc
    "MATE.TIM",
};
// GLOBAL: FF7 0x0096fcb0
char *g_pChocoboSpriteNames1[7] = {
    // STRING: FF7 0x009715e8
    "PN.TIM",
    // STRING: FF7 0x009715f0
    "PO.TIM",
    // STRING: FF7 0x009715f8
    "PP.TIM",
    // STRING: FF7 0x00971600
    "PQ.TIM",
    // STRING: FF7 0x00971608
    "PR.TIM",
    // STRING: FF7 0x00971610
    "PS.TIM",
    // STRING: FF7 0x00971618
    "PT.TIM",
};
// GLOBAL: FF7 0x0096fcd0
char *g_pChocoboSpriteNames2[7] = {
    // STRING: FF7 0x00971620
    "FIRE_1.TIM",
    // STRING: FF7 0x0097162c
    "HANA1_1.TIM",
    // STRING: FF7 0x00971638
    "HANA2_1.TIM",
    // STRING: FF7 0x00971644
    "HANA3_1.TIM",
    // STRING: FF7 0x00971650
    "HANA4_2.TIM",
    // STRING: FF7 0x0097165c
    "KOTORI_1.TIM",
    // STRING: FF7 0x0097166c
    "MATE_1.TIM",
};
// GLOBAL: FF7 0x0096fcf0
char *g_pChocoboSpriteNames3[7] = {
    // STRING: FF7 0x00971678
    "NW.tim",
    // STRING: FF7 0x00971680
    "NX.tim",
    // STRING: FF7 0x00971688
    "NY.tim",
    // STRING: FF7 0x00971690
    "NZ.tim",
    // STRING: FF7 0x00971698
    "OA.tim",
    // STRING: FF7 0x009716a0
    "OB.tim",
    // STRING: FF7 0x009716a8
    "OC.tim",
};
// GLOBAL: FF7 0x0096fd10
char *g_pChocoboSpriteNames4[36] = {
    // STRING: FF7 0x009716b0
    "AMMONITE.TIM",
    // STRING: FF7 0x009716c0
    "ANEMONE.TIM",
    // STRING: FF7 0x009716cc
    "BOAT.TIM",
    // STRING: FF7 0x009716d8
    "BUBBLE.TIM",
    // STRING: FF7 0x009716e4
    "CAW.TIM",
    // STRING: FF7 0x009716ec
    "CONDOR.TIM",
    // STRING: FF7 0x009716f8
    "FIRE.TIM",
    // STRING: FF7 0x00971704
    "FISH.TIM",
    // STRING: FF7 0x00971710
    "FUNE.TIM",
    // STRING: FF7 0x0097171c
    "GRASS.TIM",
    // STRING: FF7 0x00971728
    "HELI.TIM",
    // STRING: FF7 0x00971734
    "HETIMA1.TIM",
    // STRING: FF7 0x00971740
    "HETIMA2.TIM",
    // STRING: FF7 0x0097174c
    "HETIMA3.TIM",
    // STRING: FF7 0x00971758
    "IKADA.TIM",
    // STRING: FF7 0x00971764
    "INDIAN.TIM",
    // STRING: FF7 0x00971770
    "KINOKO.TIM",
    // STRING: FF7 0x0097177c
    "KOTORI.TIM",
    // STRING: FF7 0x00971788
    "KUMA.TIM",
    // STRING: FF7 0x00971794
    "NAPO.TIM",
    // STRING: FF7 0x009717a0
    "PATER.TIM",
    // STRING: FF7 0x009717ac
    "PENGUIN1.TIM",
    // STRING: FF7 0x009717bc
    "PENGUIN2.TIM",
    // STRING: FF7 0x009717cc
    "PIG1.TIM",
    // STRING: FF7 0x009717d8
    "PIG2.TIM",
    // STRING: FF7 0x009717e4
    "POLL.TIM",
    // STRING: FF7 0x009717f0
    "S_KABA1.TIM",
    // STRING: FF7 0x009717fc
    "S_KABA2.TIM",
    // STRING: FF7 0x00971808
    "SARDINE.TIM",
    // STRING: FF7 0x00971814
    "SEAWEED.TIM",
    // STRING: FF7 0x00971820
    "SHELL.TIM",
    // STRING: FF7 0x0097182c
    "SHIP.TIM",
    // STRING: FF7 0x00971838
    "SMOKE.TIM",
    // STRING: FF7 0x00971844
    "TAIMATU.TIM",
    // STRING: FF7 0x00971850
    "TAIMATUT.TIM",
    // STRING: FF7 0x00971860
    "WHALE.TIM",
};
// GLOBAL: FF7 0x0096fda0
char *g_pChocoboSpriteNames5[36] = {
    // STRING: FF7 0x0097186c
    "PU.TIM",
    // STRING: FF7 0x00971874
    "PV.TIM",
    // STRING: FF7 0x0097187c
    "PW.TIM",
    // STRING: FF7 0x00971884
    "PX.TIM",
    // STRING: FF7 0x0097188c
    "PY.TIM",
    // STRING: FF7 0x00971894
    "PZ.TIM",
    // STRING: FF7 0x0097189c
    "QA.TIM",
    // STRING: FF7 0x009718a4
    "QB.TIM",
    // STRING: FF7 0x009718ac
    "QC.TIM",
    // STRING: FF7 0x009718b4
    "QD.TIM",
    // STRING: FF7 0x009718bc
    "QE.TIM",
    // STRING: FF7 0x009718c4
    "QF.TIM",
    // STRING: FF7 0x009718cc
    "QG.TIM",
    // STRING: FF7 0x009718d4
    "QH.TIM",
    // STRING: FF7 0x009718dc
    "QI.TIM",
    // STRING: FF7 0x009718e4
    "QJ.TIM",
    // STRING: FF7 0x009718ec
    "QK.TIM",
    // STRING: FF7 0x009718f4
    "QL.TIM",
    // STRING: FF7 0x009718fc
    "QM.TIM",
    // STRING: FF7 0x00971904
    "QN.TIM",
    // STRING: FF7 0x0097190c
    "QO.TIM",
    // STRING: FF7 0x00971914
    "QP.TIM",
    // STRING: FF7 0x0097191c
    "QQ.TIM",
    // STRING: FF7 0x00971924
    "QR.TIM",
    // STRING: FF7 0x0097192c
    "QS.TIM",
    // STRING: FF7 0x00971934
    "QT.TIM",
    // STRING: FF7 0x0097193c
    "QU.TIM",
    // STRING: FF7 0x00971944
    "QV.TIM",
    // STRING: FF7 0x0097194c
    "QW.TIM",
    // STRING: FF7 0x00971954
    "QX.TIM",
    // STRING: FF7 0x0097195c
    "QY.TIM",
    // STRING: FF7 0x00971964
    "QZ.TIM",
    // STRING: FF7 0x0097196c
    "RA.TIM",
    // STRING: FF7 0x00971974
    "RB.TIM",
    // STRING: FF7 0x0097197c
    "RC.TIM",
    // STRING: FF7 0x00971984
    "RD.TIM",
};
// GLOBAL: FF7 0x0096fe30
char *g_pChocoboSpriteNames6[36] = {
    // STRING: FF7 0x0097198c
    "AMMONIT1.TIM",
    // STRING: FF7 0x0097199c
    "ANEMONE1.TIM",
    // STRING: FF7 0x009719ac
    "BOAT_1.TIM",
    // STRING: FF7 0x009719b8
    "BUBBLE_1.TIM",
    // STRING: FF7 0x009719c8
    "CAW_1.TIM",
    // STRING: FF7 0x009719d4
    "CONDOR_1.TIM",
    // STRING: FF7 0x009719e4
    "FIRE_1.TIM",
    // STRING: FF7 0x009719f0
    "FISH_1.TIM",
    // STRING: FF7 0x009719fc
    "FUNE_1.TIM",
    // STRING: FF7 0x00971a08
    "GRASS_1.TIM",
    // STRING: FF7 0x00971a14
    "HELI_1.TIM",
    // STRING: FF7 0x00971a20
    "HETIMA11.TIM",
    // STRING: FF7 0x00971a30
    "HETIMA21.TIM",
    // STRING: FF7 0x00971a40
    "HETIMA31.TIM",
    // STRING: FF7 0x00971a50
    "IKADA_1.TIM",
    // STRING: FF7 0x00971a5c
    "INDIAN_1.TIM",
    // STRING: FF7 0x00971a6c
    "KINOKO_1.TIM",
    // STRING: FF7 0x00971a7c
    "KOTORI_1.TIM",
    // STRING: FF7 0x00971a8c
    "KUMA_1.TIM",
    // STRING: FF7 0x00971a98
    "NAPO_1.TIM",
    // STRING: FF7 0x00971aa4
    "PATER_1.TIM",
    // STRING: FF7 0x00971ab0
    "PENGU1_1.TIM",
    // STRING: FF7 0x00971ac0
    "PENGU2_1.TIM",
    // STRING: FF7 0x00971ad0
    "PIG1_1.TIM",
    // STRING: FF7 0x00971adc
    "PIG2_1.TIM",
    // STRING: FF7 0x00971ae8
    "POLL_1.TIM",
    // STRING: FF7 0x00971af4
    "S_KABA11.TIM",
    // STRING: FF7 0x00971b04
    "S_KABA21.TIM",
    // STRING: FF7 0x00971b14
    "SARD_1.TIM",
    // STRING: FF7 0x00971b20
    "SEAW_1.TIM",
    // STRING: FF7 0x00971b2c
    "SHELL_1.TIM",
    // STRING: FF7 0x00971b38
    "SHIP_1.TIM",
    // STRING: FF7 0x00971b44
    "SMOKE_1.TIM",
    // STRING: FF7 0x00971b50
    "TAIM_1.TIM",
    // STRING: FF7 0x00971b5c
    "TAIMT_1.TIM",
    // STRING: FF7 0x00971b68
    "WHALE_1.TIM",
};
// GLOBAL: FF7 0x0096fec0
char *g_pChocoboSpriteNames7[36] = {
    // STRING: FF7 0x00971b74
    "OD.tim",
    // STRING: FF7 0x00971b7c
    "OE.tim",
    // STRING: FF7 0x00971b84
    "OF.tim",
    // STRING: FF7 0x00971b8c
    "OG.tim",
    // STRING: FF7 0x00971b94
    "OH.tim",
    // STRING: FF7 0x00971b9c
    "OI.tim",
    // STRING: FF7 0x00971ba4
    "OJ.tim",
    // STRING: FF7 0x00971bac
    "OK.tim",
    // STRING: FF7 0x00971bb4
    "OL.tim",
    // STRING: FF7 0x00971bbc
    "OM.tim",
    // STRING: FF7 0x00971bc4
    "ON.tim",
    // STRING: FF7 0x00971bcc
    "OO.tim",
    // STRING: FF7 0x00971bd4
    "OP.tim",
    // STRING: FF7 0x00971bdc
    "OQ.tim",
    // STRING: FF7 0x00971be4
    "OR.tim",
    // STRING: FF7 0x00971bec
    "OS.tim",
    // STRING: FF7 0x00971bf4
    "OT.tim",
    // STRING: FF7 0x00971bfc
    "OU.tim",
    // STRING: FF7 0x00971c04
    "OV.tim",
    // STRING: FF7 0x00971c0c
    "OW.tim",
    // STRING: FF7 0x00971c14
    "OX.tim",
    // STRING: FF7 0x00971c1c
    "OY.tim",
    // STRING: FF7 0x00971c24
    "OZ.tim",
    // STRING: FF7 0x00971c2c
    "PA.tim",
    // STRING: FF7 0x00971c34
    "PB.tim",
    // STRING: FF7 0x00971c3c
    "PC.tim",
    // STRING: FF7 0x00971c44
    "PD.tim",
    // STRING: FF7 0x00971c4c
    "PE.tim",
    // STRING: FF7 0x00971c54
    "PF.tim",
    // STRING: FF7 0x00971c5c
    "PG.tim",
    // STRING: FF7 0x00971c64
    "PH.tim",
    // STRING: FF7 0x00971c6c
    "PI.tim",
    // STRING: FF7 0x00971c74
    "PJ.tim",
    // STRING: FF7 0x00971c7c
    "PK.tim",
    // STRING: FF7 0x00971c84
    "PL.tim",
    // STRING: FF7 0x00971c8c
    "PM.tim",
};
// The two model parameter tables Mini_ChocoboBuildRaceObjects selects
// (sized to reach the following .data definition).
// GLOBAL: FF7 0x0096ff50
int g_ChocoboRacerLut0[256] = {
    0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 13, 13, 14,
    14, 14, 14, 15, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 19,
    19, 19, 19, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23,
    23, 23, 23, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25,
    25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26,
    26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28,
    28, 28, 28, 29, 29, 29, 29, 30, 30, 30, 30, 31, 31, 31, 32, 32,
    32, 32, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 34,
    34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 35, 35, 35, 0,
};
// GLOBAL: FF7 0x00970350
int g_ChocoboRacerLut1[90] = {
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 3, 3, 3, 3,
    3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 4, 4, 4, 4, 4,
    4, 4, 4, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 5, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
};
// clang-format on

#pragma intrinsic(sqrt, atan2)

// Original scalar: written by the sorting routine and read at 7741A6/774988.

#pragma intrinsic(strcpy, strcat)

/*
 * Selects long-course or short-course sprite name and timing tables, creating 36 or 7 render objects.
 */
// FUNCTION: FF7 0x0076f920
void __cdecl Mini_ChocoboRaceCreateCourseSpriteObjects(PfileLoadOptions *loadOptions, GfxEngineContext *engine, int variant)
{
    char fname[128];
    int *models;
    int i;

    if (g_ChocoboRetailFlag != 0) {
        loadOptions->file.fromArchive = 1;
        loadOptions->file.archiveId = 7;
        loadOptions->file.callback = 0;
    }
    if (variant == 0) {
        g_ChocoboSpriteRecordCount = 36;
        if (g_ChocoboRetailFlag == 0) {
            if (g_ChocoboUiScalingMode == 2) {
                models = (int *)&g_pChocoboSpriteNames6;
            } else {
                models = (int *)&g_pChocoboSpriteNames4;
            }
        } else {
            if (g_ChocoboUiScalingMode == 2) {
                models = (int *)&g_pChocoboSpriteNames7;
            } else {
                models = (int *)&g_pChocoboSpriteNames5;
            }
        }
        strcpy(fname, g_ChocoboAssetDirPath);
        strcat(fname, "data\\long\\lsprite");
        g_ChocoboSpriteRecordMap = ((int)&g_ChocoboRacerLut0);
    } else {
        g_ChocoboSpriteRecordCount = 7;
        if (g_ChocoboRetailFlag == 0) {
            if (g_ChocoboUiScalingMode == 2) {
                models = (int *)&g_pChocoboSpriteNames2;
            } else {
                models = (int *)&g_pChocoboSpriteNames0;
            }
        } else {
            if (g_ChocoboUiScalingMode == 2) {
                models = (int *)&g_pChocoboSpriteNames3;
            } else {
                models = (int *)&g_pChocoboSpriteNames1;
            }
        }
        strcpy(fname, g_ChocoboAssetDirPath);
        strcat(fname, "data\\short\\ssprite");
        g_ChocoboSpriteRecordMap = ((int)&g_ChocoboRacerLut1);
    }
    for (i = 0; i < g_ChocoboSpriteRecordCount; i++) {
        if (variant == 0) {
            if (i != 3 && i != 6 && i != 33) {
                Pfile_InitLoadOptionsWithBlend(BLEND_NONE, loadOptions);
                if (g_ChocoboRetailFlag != 0) {
                    loadOptions->file.fromArchive = 1;
                    loadOptions->file.archiveId = 7;
                    loadOptions->file.callback = 0;
                }
                g_ChocoboSpriteRecords[i].flag = 0;
            } else {
                Pfile_InitLoadOptionsWithBlend(BLEND_ADDITIVE, loadOptions);
                if (g_ChocoboRetailFlag != 0) {
                    loadOptions->file.fromArchive = 1;
                    loadOptions->file.archiveId = 7;
                    loadOptions->file.callback = 0;
                }
                g_ChocoboSpriteRecords[i].flag = 1;
            }
            loadOptions->directory = (char *)fname;
            loadOptions->flags |= 0x10;
        } else {
            if (i != 0) {
                Pfile_InitLoadOptionsWithBlend(BLEND_NONE, loadOptions);
                if (g_ChocoboRetailFlag != 0) {
                    loadOptions->file.fromArchive = 1;
                    loadOptions->file.archiveId = 7;
                    loadOptions->file.callback = 0;
                }
                g_ChocoboSpriteRecords[i].flag = 0;
            } else {
                Pfile_InitLoadOptionsWithBlend(BLEND_ADDITIVE, loadOptions);
                if (g_ChocoboRetailFlag != 0) {
                    loadOptions->file.fromArchive = 1;
                    loadOptions->file.archiveId = 7;
                    loadOptions->file.callback = 0;
                }
                g_ChocoboSpriteRecords[i].flag = 1;
            }
            loadOptions->directory = (char *)fname;
            loadOptions->flags |= 0x10;
        }
        g_ChocoboSpriteRecords[i].renderObject = (int)Gfx_CreateRenderObject(
            1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)models[i], (void *)engine->renderObjectBank);
        g_ChocoboSpriteRecords[i].active = 1;
        g_ChocoboSpriteRecords[i].dirtyFlag = 0;
        g_ChocoboSpriteRecords[i].sizeA = g_ChocoboCourseSpriteSize;
        g_ChocoboSpriteRecords[i].sizeB = g_ChocoboCourseSpriteSize;
        g_ChocoboSpriteRecords[i].tint[2] = 0x80;
        g_ChocoboSpriteRecords[i].tint[1] = 0x80;
        g_ChocoboSpriteRecords[i].tint[0] = 0x80;
        g_ChocoboSpriteRecords[i].paletteIndex = 0;
    }
}
/*
 * Tears down the render objects of the sprite record array.
 */
// FUNCTION: FF7 0x0076fd68
void __cdecl Mini_ChocoboRaceDestroyCourseSpriteObjects(void)
{
    int i;

    for (i = 0; i < g_ChocoboSpriteRecordCount; i++) {
        if (g_ChocoboSpriteRecords[i].active != 0) {
            DestroyRenderObject((void *)g_ChocoboSpriteRecords[i].renderObject);
            g_ChocoboSpriteRecords[i].renderObject = 0;
            g_ChocoboSpriteRecords[i].active = 0;
        }
    }
}
/*
 * Walks the sprite record array and clears the dirty flag of every in-use entry.
 */
// FUNCTION: FF7 0x0076fdd3
void __cdecl Mini_ChocoboClearSpriteObjectDirtyFlags(void)
{
    unsigned short i;

    for (i = 0; i < g_ChocoboSpriteRecordCount; i++) {
        if (g_ChocoboSpriteRecords[i].active != 0 && g_ChocoboSpriteRecords[i].dirtyFlag != 0) {
            g_ChocoboSpriteRecords[i].dirtyFlag = 0;
        }
    }
}
/*
 * Submits the first group of the sprite record array to primitive batch list rendering.
 */
// FUNCTION: FF7 0x0076fe41
void __cdecl Mini_ChocoboEmitSpriteObjectsGroup0(GfxEngineContext *renderContext)
{
    unsigned short i;

    for (i = 0; i < g_ChocoboSpriteRecordCount; i++) {
        if (g_ChocoboSpriteRecords[i].active != 0 && g_ChocoboSpriteRecords[i].dirtyFlag != 0 && g_ChocoboSpriteRecords[i].flag == 0) {
            Gfx_EmitPrimitiveBatchList((void *)g_ChocoboSpriteRecords[i].renderObject, renderContext);
        }
    }
}
/*
 * Submits the second group of the sprite record array to primitive batch list rendering.
 */
// FUNCTION: FF7 0x0076fecc
void __cdecl Mini_ChocoboEmitSpriteObjectsGroup1(GfxEngineContext *renderContext)
{
    unsigned short i;

    for (i = 0; i < g_ChocoboSpriteRecordCount; i++) {
        if (g_ChocoboSpriteRecords[i].active != 0 && g_ChocoboSpriteRecords[i].dirtyFlag != 0 && g_ChocoboSpriteRecords[i].flag != 0) {
            Gfx_EmitPrimitiveBatchList((void *)g_ChocoboSpriteRecords[i].renderObject, renderContext);
        }
    }
}
} /* extern "C" */
