// src/ff7/chocobo/chocobo_race_hud_submit.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Race HUD submission for the chocobo minigame: updates and draws the racer HUD and map
//   markers, and initializes the racer selection table from saved data.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/chocobo.h"
#include "ff7/enums/chocobo.h"
#include "lib/codegen.h"
#include "ff7/api/chocobo.h"
#include "ff7/savemap.h"
extern "C" {


/* The HUD's atlas cell numbers: one per racer target index, then one per finishing place. */
typedef struct {
    int atlasCellByTarget[5];
    int atlasCellByPlace[7];
} ChocoboLayoutTable;

// clang-format off
// clang-format on

// clang-format off
// GLOBAL: FF7 0x0096e878
struct ChocoboColor g_ChocoboRacerHudColors[6] = {{0, -1, 0, {0}}, {0, -1, -1, {0}}, {-1, -128, -128, {0}}, {-1, 0, 0, {0}}, {0, 0, -1, {0}}, {-1, -1, -1, {0}}};
// GLOBAL: FF7 0x0096e890
struct ChocoboColor g_ChocoboRaceColorRgb[5] = {{-1, -1, 0, {0}}, {0, -1, 0, {0}}, {0, 0, -1, {0}}, {-128, -128, -128, {0}}, {-1, -1, 60, {0}}};
// GLOBAL: FF7 0x0096e8a8
ChocoboLayoutTable g_ChocoboLayoutTable = {{1, 4, 5, 0, 0}, {3, 5, 6, 0, 0, 0, 3}};
// GLOBAL: FF7 0x0096e8d8
unsigned char g_ChocoboHudSpriteRecord0[132] = {0x0e, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x53, 0x63, 0x3e, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x3f, 0x00, 0x00, 0x40, 0x3f, 0x00, 0x00, 0x40, 0x42, 0x00, 0x00, 0x40, 0x42, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x53, 0x63, 0x3e, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x3f, 0x00, 0x00, 0x40, 0x3f};
unsigned char g_rawData_0096e95c[4] = {0x00, 0x00, 0x40, 0x42};
unsigned char g_rawData_0096e960[8] = {0x00, 0x00, 0x40, 0x42, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0096e968
unsigned char g_ChocoboHudSpriteRecord1[132] = {0x14, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x53, 0x63, 0x3e, 0x40, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0xe1, 0x7a, 0x3f, 0xae, 0x47, 0x61, 0x3f, 0x00, 0x00, 0x00, 0x43, 0x00, 0x00, 0xe0, 0x42, 0x00, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x53, 0x63, 0x3e, 0x40, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0xe1, 0x7a, 0x3f, 0xae, 0x47, 0x61, 0x3f};
unsigned char g_rawData_0096e9ec[4] = {0x00, 0x00, 0x00, 0x43};
unsigned char g_rawData_0096e9f0[8] = {0x00, 0x00, 0xe0, 0x42, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0096e9f8
unsigned char g_ChocoboHudSpriteRecord2[132] = {0x0e, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x53, 0x63, 0x3e, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x3f, 0x00, 0x00, 0x40, 0x3f, 0x00, 0x00, 0xc0, 0x41, 0x00, 0x00, 0xc0, 0x41, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x53, 0x63, 0x3e, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x3f, 0x00, 0x00, 0x40, 0x3f};
unsigned char g_rawData_0096ea7c[4] = {0x00, 0x00, 0xc0, 0x41};
unsigned char g_rawData_0096ea80[8] = {0x00, 0x00, 0xc0, 0x41, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0096ea88
unsigned char g_ChocoboHudSpriteRecord3[132] = {0x14, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x53, 0x63, 0x3e, 0x40, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0xe1, 0x7a, 0x3f, 0xae, 0x47, 0x61, 0x3f, 0x00, 0x00, 0x80, 0x42, 0x00, 0x00, 0x60, 0x42, 0x00, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x53, 0x63, 0x3e, 0x40, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0xe1, 0x7a, 0x3f, 0xae, 0x47, 0x61, 0x3f};
unsigned char g_rawData_0096eb0c[4] = {0x00, 0x00, 0x80, 0x42};
unsigned char g_rawData_0096eb10[8] = {0x00, 0x00, 0x60, 0x42, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0096eb18
int g_ChocoboOffsets[6] = {18, 26, 34, 34, 34, 34};
// IEEE-754 float bit patterns stored as int (retail object emits no __fltused; see ChocoboHudFloatBits).
// GLOBAL: FF7 0x0096eb30
int g_ChocoboFloatTable0[6] = {0, CHOCOBO_FLOAT_0_1875_BITS, FLOAT_0_375_BITS, 0, CHOCOBO_FLOAT_0_1875_BITS, FLOAT_0_375_BITS};
// GLOBAL: FF7 0x0096eb48
int g_ChocoboFloatTable1[8] = {0, CHOCOBO_FLOAT_0_25_BITS, FLOAT_HALF_BITS, FLOAT_0_75_BITS, 0, CHOCOBO_FLOAT_0_25_BITS, FLOAT_HALF_BITS, 0};
// GLOBAL: FF7 0x0096eb68
int g_ChocoboHudLaneIconTexUSize2x = CHOCOBO_FLOAT_0_1875_BITS;
// GLOBAL: FF7 0x0096eb6c
int g_ChocoboHudLaneIconTexVSize2x = FLOAT_0_375_BITS;
// GLOBAL: FF7 0x0096eb70
int g_ChocoboHudAtlasCellTexUSize2x = CHOCOBO_FLOAT_0_25_BITS;
// GLOBAL: FF7 0x0096eb74
int g_ChocoboHudAtlasCellTexVSize2x = FLOAT_HALF_BITS;
// GLOBAL: FF7 0x0096eb78
int g_ChocoboFloatTable2[6] = {0, CHOCOBO_FLOAT_0_09_BITS, CHOCOBO_FLOAT_0_19_BITS, CHOCOBO_FLOAT_0_28_BITS, CHOCOBO_FLOAT_0_38_BITS, CHOCOBO_FLOAT_0_47_BITS};
// GLOBAL: FF7 0x0096eb90
int g_ChocoboFloatTable3[8] = {0, CHOCOBO_FLOAT_0_13_BITS, CHOCOBO_FLOAT_0_25_BITS, CHOCOBO_FLOAT_0_38_BITS, FLOAT_HALF_BITS, CHOCOBO_FLOAT_0_63_BITS, FLOAT_0_75_BITS, 0};
// GLOBAL: FF7 0x0096ebb0
int g_ChocoboHudLaneIconTexUSize1x = CHOCOBO_FLOAT_0_09_BITS;
// GLOBAL: FF7 0x0096ebb4
int g_ChocoboHudLaneIconTexVSize1x = CHOCOBO_FLOAT_0_97_BITS;
// GLOBAL: FF7 0x0096ebb8
int g_ChocoboHudAtlasCellTexUSize1x = CHOCOBO_FLOAT_0_121_BITS;
// GLOBAL: FF7 0x0096ebbc
int g_ChocoboHudAtlasCellTexVSize1x = CHOCOBO_FLOAT_0_97_BITS;
// Six animation states, bounded by every writer of the racer's animation phase.
// GLOBAL: FF7 0x0096ebc0
int g_ChocoboIconOffsets[6] = {24, 18, 20, 20, 20, 18};
unsigned char g_rawData_0096ebd8[8] = {0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00e71670
struct ChocoboSpriteRecord g_ChocoboHudRacerRecords0[6] = {0};
// GLOBAL: FF7 0x00e71820
int g_ChocoboLaneHudStates = 0;
#define g_ChocoboLaneHudPaletteTable (&g_ChocoboLaneHudStates)
// GLOBAL: FF7 0x00e71824
int g_ChocoboLaneHudPalette1 = 0;
// GLOBAL: FF7 0x00e71828
int g_ChocoboLaneHudPalette2 = 0;
// GLOBAL: FF7 0x00e7182c
int g_ChocoboLaneHudPalette3 = 0;
// GLOBAL: FF7 0x00e71830
int g_ChocoboLaneHudPalette4 = 0;
// GLOBAL: FF7 0x00e71834
int g_ChocoboLaneHudPalette5 = 0;
// GLOBAL: FF7 0x00e71838
struct ChocoboSpriteRecord g_ChocoboHudRacerRecords1[6] = {0};
// clang-format on
/*
 * Updates and draws the six-racer HUD and map markers. Every sixteen frames sorts racer/index
 * pairs by the active progress metric and rebuilds ordered portrait/icon records with scale-
 * dependent atlas coordinates and sizes. Each frame submits mode-specific HUD elements, optional
 * repeated blinking indicators and ordered racer panels, then builds map markers for nonexcluded
 * racers with selected-racer blinking color and depth.
 */
// FUNCTION: FF7 0x0076e2b0
void __cdecl Mini_ChocoboSubmitRaceHud(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        int stateBit8;
        struct { unsigned char pad_to_4[0x4]; int sortIndex; };
        struct { unsigned char pad_to_8[0x8]; int sortedSlot; };
        struct { unsigned char pad_to_C[0xC]; ChocoboRacer *sortedRacer; };
        struct {
            unsigned char pad_to_10[0x10];
            ChocoboRaceSortEntry sortEntries[6];
            ChocoboRaceSortEntry swapTmp;
        };
        struct {
            unsigned char pad_to_18[0x18];
            ChocoboRaceSortEntry nextSortEntries[5];
        };
        struct { unsigned char pad_to_48[0x48]; int racerIndex; };
        struct { unsigned char pad_to_4C[0x4C]; ChocoboRacer *racer; };
    } frame;
#pragma pack(pop)

    if (g_ChocoboFrameCounter % 16 == 0) {
        for (frame.sortedSlot = 0; (unsigned int)frame.sortedSlot < (unsigned int)6; frame.sortedSlot++) {
            if (g_ChocoboRacers[frame.sortedSlot].finished != 0) {
                frame.sortEntries[frame.sortedSlot].sortKey = g_ChocoboRacers[frame.sortedSlot].place;
            } else {
                frame.sortEntries[frame.sortedSlot].sortKey = g_ChocoboRacers[frame.sortedSlot].rank + 0xA;
            }
            frame.sortEntries[frame.sortedSlot].racerIndex = frame.sortedSlot;
        }
        for (frame.sortedSlot = 0; (unsigned int)frame.sortedSlot < (unsigned int)6; frame.sortedSlot++) {
            for (frame.sortIndex = 0; (unsigned int)frame.sortIndex < (unsigned int)5; frame.sortIndex++) {
                if (frame.sortEntries[frame.sortIndex].sortKey > frame.nextSortEntries[frame.sortIndex].sortKey) {
                    frame.swapTmp = frame.sortEntries[frame.sortIndex];
                    frame.sortEntries[frame.sortIndex] = frame.nextSortEntries[frame.sortIndex];
                    frame.nextSortEntries[frame.sortIndex] = frame.swapTmp;
                }
            }
        }
        for (frame.sortedSlot = 0; (unsigned int)frame.sortedSlot < (unsigned int)6; frame.sortedSlot++) {
            frame.sortedRacer = &g_ChocoboRacers[frame.sortEntries[frame.sortedSlot].racerIndex];
            g_ChocoboHudRacerRecords0[frame.sortedSlot].renderObjectIndex = 17;
            g_ChocoboHudRacerRecords0[frame.sortedSlot].colorB = 0x80;
            g_ChocoboHudRacerRecords0[frame.sortedSlot].colorG = 0x80;
            g_ChocoboHudRacerRecords0[frame.sortedSlot].colorR = 0x80;
            g_ChocoboHudRacerRecords1[frame.sortedSlot].renderObjectIndex = 16;
            if (g_ChocoboPlayerRides != 0) {
                g_ChocoboHudRacerRecords0[frame.sortedSlot].left = frame.sortedSlot * 40 + 8;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].left = *(unsigned short *)&g_ChocoboHudRacerRecords0[frame.sortedSlot].left + 8;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].vertexAY = (short)g_ChocoboOffsets[frame.sortedSlot];
                g_ChocoboHudRacerRecords0[frame.sortedSlot].vertexAY = *(unsigned short *)&g_ChocoboHudRacerRecords1[frame.sortedSlot].vertexAY + 0x16;
            } else {
                g_ChocoboHudRacerRecords0[frame.sortedSlot].left = frame.sortedSlot * 50 + 16;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].left = *(unsigned short *)&g_ChocoboHudRacerRecords0[frame.sortedSlot].left + 8;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].vertexAY = g_ChocoboOffsets[frame.sortedSlot] + 0x8C;
                g_ChocoboHudRacerRecords0[frame.sortedSlot].vertexAY = *(unsigned short *)&g_ChocoboHudRacerRecords1[frame.sortedSlot].vertexAY + 0x16;
            }
            if (g_ChocoboUiScalingMode == 2) {
                if (frame.sortedRacer->lane < 3) {
                    g_ChocoboHudRacerRecords1[frame.sortedSlot].texX = frame.sortedRacer->lane * 48;
                    g_ChocoboHudRacerRecords1[frame.sortedSlot].texY = 0;
                    g_ChocoboHudRacerRecords1[frame.sortedSlot].texV = 0;
                } else {
                    g_ChocoboHudRacerRecords1[frame.sortedSlot].texX = (frame.sortedRacer->lane - 3) * 48;
                    g_ChocoboHudRacerRecords1[frame.sortedSlot].texY = 0x30;
                    g_ChocoboHudRacerRecords1[frame.sortedSlot].texV = FLOAT_0_375_BITS;
                }
                g_ChocoboHudRacerRecords1[frame.sortedSlot].texU = g_ChocoboFloatTable0[frame.sortedRacer->lane];
                g_ChocoboHudRacerRecords1[frame.sortedSlot].height = 24;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].width = 24;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].texelHeight = CHOCOBO_FLOAT_48_BITS;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].texelWidth = CHOCOBO_FLOAT_48_BITS;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].texUSize = g_ChocoboHudLaneIconTexUSize2x;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].texVSize = g_ChocoboHudLaneIconTexVSize2x;
                if (frame.sortedRacer->finished != 0) {
                    g_ChocoboHudRacerRecords0[frame.sortedSlot].texU = g_ChocoboFloatTable1[g_ChocoboLayoutTable.atlasCellByPlace[frame.sortedRacer->place]];
                    if (g_ChocoboLayoutTable.atlasCellByPlace[frame.sortedRacer->place] < 4) {
                        g_ChocoboHudRacerRecords0[frame.sortedSlot].texY = 0;
                        g_ChocoboHudRacerRecords0[frame.sortedSlot].texV = 0;
                        g_ChocoboHudRacerRecords0[frame.sortedSlot].texX = g_ChocoboLayoutTable.atlasCellByPlace[frame.sortedRacer->place] << 6;
                    } else {
                        g_ChocoboHudRacerRecords0[frame.sortedSlot].texY = 0x40;
                        g_ChocoboHudRacerRecords0[frame.sortedSlot].texV = FLOAT_HALF_BITS;
                        g_ChocoboHudRacerRecords0[frame.sortedSlot].texX = g_ChocoboLayoutTable.atlasCellByPlace[frame.sortedRacer->place] - 4 << 6;
                    }
                } else {
                    g_ChocoboHudRacerRecords0[frame.sortedSlot].texU = g_ChocoboFloatTable1[g_ChocoboLayoutTable.atlasCellByTarget[frame.sortedRacer->targetIndex]];
                    if (g_ChocoboLayoutTable.atlasCellByTarget[frame.sortedRacer->targetIndex] < 4) {
                        g_ChocoboHudRacerRecords0[frame.sortedSlot].texX = g_ChocoboLayoutTable.atlasCellByTarget[frame.sortedRacer->targetIndex] << 6;
                        g_ChocoboHudRacerRecords0[frame.sortedSlot].texY = 0;
                        g_ChocoboHudRacerRecords0[frame.sortedSlot].texV = 0;
                    } else {
                        g_ChocoboHudRacerRecords0[frame.sortedSlot].texX = g_ChocoboLayoutTable.atlasCellByTarget[frame.sortedRacer->targetIndex] - 4 << 6;
                        g_ChocoboHudRacerRecords0[frame.sortedSlot].texY = 0x40;
                        g_ChocoboHudRacerRecords0[frame.sortedSlot].texV = FLOAT_HALF_BITS;
                    }
                }
                g_ChocoboHudRacerRecords1[frame.sortedSlot].palette = 0;
                g_ChocoboHudRacerRecords0[frame.sortedSlot].palette = g_ChocoboLaneHudPaletteTable[frame.sortedRacer->lane];
                g_ChocoboHudRacerRecords0[frame.sortedSlot].width = 31;
                g_ChocoboHudRacerRecords0[frame.sortedSlot].height = 32;
                g_ChocoboHudRacerRecords0[frame.sortedSlot].texelWidth = CHOCOBO_FLOAT_63_BITS;
                g_ChocoboHudRacerRecords0[frame.sortedSlot].texelHeight = CHOCOBO_FLOAT_64_BITS;
                g_ChocoboHudRacerRecords0[frame.sortedSlot].texUSize = g_ChocoboHudAtlasCellTexUSize2x;
                g_ChocoboHudRacerRecords0[frame.sortedSlot].texVSize = g_ChocoboHudAtlasCellTexVSize2x;
            } else {
                g_ChocoboHudRacerRecords1[frame.sortedSlot].texX = frame.sortedRacer->lane * 24;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].texY = 0;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].texU = g_ChocoboFloatTable2[frame.sortedRacer->lane];
                g_ChocoboHudRacerRecords1[frame.sortedSlot].texV = 0;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].height = 24;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].width = 24;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].texelHeight = CHOCOBO_FLOAT_24_BITS;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].texelWidth = CHOCOBO_FLOAT_24_BITS;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].texUSize = g_ChocoboHudLaneIconTexUSize1x;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].texVSize = g_ChocoboHudLaneIconTexVSize1x;
                if (frame.sortedRacer->finished != 0) {
                    g_ChocoboHudRacerRecords0[frame.sortedSlot].texX = g_ChocoboLayoutTable.atlasCellByPlace[frame.sortedRacer->place] << 5;
                    g_ChocoboHudRacerRecords0[frame.sortedSlot].texU = g_ChocoboFloatTable3[g_ChocoboLayoutTable.atlasCellByPlace[frame.sortedRacer->place]];
                } else {
                    g_ChocoboHudRacerRecords0[frame.sortedSlot].texU = g_ChocoboFloatTable3[g_ChocoboLayoutTable.atlasCellByTarget[frame.sortedRacer->targetIndex]];
                    g_ChocoboHudRacerRecords0[frame.sortedSlot].texX = g_ChocoboLayoutTable.atlasCellByTarget[frame.sortedRacer->targetIndex] << 5;
                }
                g_ChocoboHudRacerRecords0[frame.sortedSlot].texY = 0;
                g_ChocoboHudRacerRecords0[frame.sortedSlot].texV = 0;
                g_ChocoboHudRacerRecords1[frame.sortedSlot].palette = 0;
                g_ChocoboHudRacerRecords0[frame.sortedSlot].palette = g_ChocoboLaneHudPaletteTable[frame.sortedRacer->lane];
                g_ChocoboHudRacerRecords0[frame.sortedSlot].width = 31;
                g_ChocoboHudRacerRecords0[frame.sortedSlot].height = 32;
                g_ChocoboHudRacerRecords0[frame.sortedSlot].texelWidth = CHOCOBO_FLOAT_31_BITS;
                g_ChocoboHudRacerRecords0[frame.sortedSlot].texelHeight = CHOCOBO_FLOAT_32_BITS;
                g_ChocoboHudRacerRecords0[frame.sortedSlot].texUSize = g_ChocoboHudAtlasCellTexUSize1x;
                g_ChocoboHudRacerRecords0[frame.sortedSlot].texVSize = g_ChocoboHudAtlasCellTexVSize1x;
            }
        }
    }
    if (g_ChocoboPlayerRides != 0) {
        if (g_ChocoboRacers[0].autoPilot != 0) {
            Mini_ChocoboEmitSpriteBatchQuad(g_ChocoboHudSpriteB);
        } else {
            Mini_ChocoboEmitSpriteBatchQuad(&g_ChocoboHudSpriteB[1]);
        }
        g_ChocoboSubmittedRecordCount++;
    }
    for (frame.racerIndex = 0; frame.racerIndex < 6; frame.racerIndex++) {
        if (g_ChocoboSkipPromptActive != 0) {
            frame.stateBit8 = g_ChocoboFrameCounter & 8;
            if (frame.stateBit8 != 0) {
                g_ChocoboHudSpriteA->palette = 0;
            } else {
                g_ChocoboHudSpriteA->palette = 1;
            }
            if (g_ChocoboPlayerRides == 0) {
                g_ChocoboHudSpriteA->uiY = 166;
                g_ChocoboHudSpriteA->uiX = 23;
            }
            Mini_ChocoboEmitSpriteBatchQuad(g_ChocoboHudSpriteA);
            g_ChocoboSubmittedRecordCount++;
            if (frame.stateBit8 != 0) {
                g_ChocoboHudSpriteA[1].palette = 0;
            } else {
                g_ChocoboHudSpriteA[1].palette = 1;
            }
            if (g_ChocoboPlayerRides == 0) {
                g_ChocoboHudSpriteA[1].vertexAY = 0xAE;
                g_ChocoboHudSpriteA[1].left = 71;
            }
            Mini_ChocoboEmitSpriteBatchQuad(&g_ChocoboHudSpriteA[1]);
            g_ChocoboSubmittedRecordCount++;
        }
        if (g_ChocoboRaceAdvanceEnabled != 0) {
            Mini_ChocoboEmitSpriteBatchQuad(&g_ChocoboHudRacerRecords1[frame.racerIndex]);
            g_ChocoboSubmittedRecordCount++;
            Mini_ChocoboEmitSpriteBatchQuad(&g_ChocoboHudRacerRecords0[frame.racerIndex]);
            g_ChocoboSubmittedRecordCount++;
        }
        frame.racer = &g_ChocoboRacers[frame.racerIndex];
        if (frame.racer->finished != 0) {
            continue;
        }
        g_ChocoboRacerTable[frame.racerIndex].vertexBX = frame.racer->posX / 650 + 0x11A;
        g_ChocoboRacerTable[frame.racerIndex].left = g_ChocoboRacerTable[frame.racerIndex].vertexBX;
        // BUG: nothing in ff7.exe sets g_ChocoboMarkerYTable, so each racer's minimap marker y is
        // read from near address 0x186B8; where that page is unmapped the race faults on its
        // first frame, elsewhere the marker sits wherever those bytes put it.
        g_ChocoboRacerTable[frame.racerIndex].vertexDX = *(short *)(g_ChocoboMarkerYTable + frame.racerIndex * 0x18 + 0x186B8) + 3;
        g_ChocoboRacerTable[frame.racerIndex].right = g_ChocoboRacerTable[frame.racerIndex].vertexDX;
        g_ChocoboRacerTable[frame.racerIndex].vertexCY = frame.racer->posZ / 610 + 0x28;
        g_ChocoboRacerTable[frame.racerIndex].vertexAY = g_ChocoboRacerTable[frame.racerIndex].vertexCY;
        g_ChocoboRacerTable[frame.racerIndex].vertexDY = *(unsigned short *)&g_ChocoboRacerTable[frame.racerIndex].vertexAY + 3;
        g_ChocoboRacerTable[frame.racerIndex].vertexBY = g_ChocoboRacerTable[frame.racerIndex].vertexDY;
        g_ChocoboRacerTable[frame.racerIndex].height = 3;
        g_ChocoboRacerTable[frame.racerIndex].width = 3;
        if (frame.racerIndex == g_ChocoboViewedRacer && (g_ChocoboFrameCounter & 8) != 0) {
            g_ChocoboRacerTable[frame.racerIndex].colorR = g_ChocoboRacerHudColors[frame.racerIndex].color;
            g_ChocoboRacerTable[frame.racerIndex].colorG = g_ChocoboRacerHudColors[frame.racerIndex].colorValue;
            g_ChocoboRacerTable[frame.racerIndex].colorB = g_ChocoboRacerHudColors[frame.racerIndex].colorB;
        } else {
            g_ChocoboRacerTable[frame.racerIndex].colorR = *(unsigned char *)&g_ChocoboRacerHudColors[frame.racerIndex].color / 2;
            g_ChocoboRacerTable[frame.racerIndex].colorG = *(unsigned char *)&g_ChocoboRacerHudColors[frame.racerIndex].colorValue / 2;
            g_ChocoboRacerTable[frame.racerIndex].colorB = *(unsigned char *)&g_ChocoboRacerHudColors[frame.racerIndex].colorB / 2;
        }
        if (frame.racerIndex == g_ChocoboViewedRacer) {
            g_ChocoboRacerTable[frame.racerIndex].depth = CHOCOBO_FLOAT_0_22_BITS;
        } else {
            g_ChocoboRacerTable[frame.racerIndex].depth = CHOCOBO_FLOAT_0_23_BITS;
        }
        Mini_ChocoboEmitFlatSpriteRect((struct ChocoboFlatSpriteRect *)&g_ChocoboRacerTable[frame.racerIndex]);
    }
    Mini_ChocoboEmitSpriteBatchQuad(&g_ChocoboCourseRecord0);
    frame.racer = &g_ChocoboRacers[0];
    if (g_ChocoboPlayerRides != 0) {
        if (frame.racer->dashTimer != 0 && (g_ChocoboFrameCounter & 4) != 0) {
            g_ChocoboStaminaGaugeRecord.renderObjectIndex = 23;
        } else {
            g_ChocoboStaminaGaugeRecord.renderObjectIndex = 22;
        }
        Mini_ChocoboEmitSpriteBatchQuad(&g_ChocoboStaminaGaugeRecord);
        frame.racerIndex = 208 - frame.racer->stamina * 68 / frame.racer->staminaMax;
        if (frame.racerIndex > 209) {
            frame.racerIndex = 209;
        }
        g_ChocoboCourseRecord1.vertexCY = *(short *)&frame.racerIndex;
        g_ChocoboCourseRecord1.vertexAY = g_ChocoboCourseRecord1.vertexCY;
        g_ChocoboCourseRecord1.height = *(unsigned short *)&g_ChocoboCourseRecord1.vertexDY - *(unsigned short *)&g_ChocoboCourseRecord1.vertexCY;
        Mini_ChocoboEmitFlatSpriteRect((struct ChocoboFlatSpriteRect *)&g_ChocoboCourseRecord1);
    }
}
/*
 * Initializes six selection values, modifies the first from saved subtype when its enable flag is
 * set and the second from saved flag gSavemap.racerEnabled. In an additional mode selects the palette value
 * from the saved subtype table; the saved flag also sets all three stored color bytes to 0x80.
 */
// FUNCTION: FF7 0x0076eeb9
void __cdecl Mini_ChocoboRaceInitLaneHudStatesFromSavedColor(void)
{
    int hudStep;
    int laneCnt;
    int width;
    int positionY;

    width = 624;
    positionY = 250;
    hudStep = 16;
    laneCnt = 6;
    g_ChocoboLaneHudStates = 0;
    g_ChocoboLaneHudPalette1 = 3;
    g_ChocoboLaneHudPalette2 = 8;
    g_ChocoboLaneHudPalette3 = 5;
    g_ChocoboLaneHudPalette4 = 6;
    g_ChocoboLaneHudPalette5 = 7;
    if (g_ChocoboHudPlayerRides != 0) {
        switch (gSavemap.raceChocoboColor) {
        case CHOCOBO_COLOR_YELLOW:
            g_ChocoboLaneHudStates = 1;
            break;
        case CHOCOBO_COLOR_GREEN:
            g_ChocoboLaneHudStates = 2;
            break;
        case CHOCOBO_COLOR_BLUE:
            g_ChocoboLaneHudStates = 3;
            break;
        case CHOCOBO_COLOR_BLACK:
            g_ChocoboLaneHudStates = 4;
            break;
        case CHOCOBO_COLOR_GOLD:
            g_ChocoboLaneHudStates = 1;
        }
    }
    if (gSavemap.racerEnabled != 0) {
        g_ChocoboLaneHudPalette1 = 4;
    }
    if (g_ChocoboPlayerRides != 0) {
        g_ChocoboRacerHudColors[0] = g_ChocoboRaceColorRgb[gSavemap.raceChocoboColor];
    }
    if (gSavemap.racerEnabled != 0) {
        g_ChocoboRacerHudColors[1].colorB = 0x80;
        g_ChocoboRacerHudColors[1].colorValue = g_ChocoboRacerHudColors[1].colorB;
        g_ChocoboRacerHudColors[1].color = g_ChocoboRacerHudColors[1].colorValue;
    }
}
} /* extern "C" */
