// src/ff7/chocobo/chocobo_racer_state_update.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Racer records for the chocobo race: builds the racers with lane weights and
//   save-derived stats, applies event codes, ranks and updates them each frame, and advances the
//   selected racer along the route and track.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/codegen.h"
#include "ff7/types/chocobo.h"
#include "ff7/enums/chocobo.h"
#include "ff7/api/chocobo.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ff7/gte.h"
#include "ff7/savemap.h"
#include "lib/api/polygon.h"
#include "lib/enums/file.h"
extern "C" {

struct GfxFloatColorAddend;
struct PolygonModelVertexColors;
#pragma intrinsic(sqrt, atan2)
#pragma intrinsic(strcpy, strcat)

// clang-format off
// GLOBAL: FF7 0x0097a3e0
ChocoboClassStats g_ChocoboClassStats[4] = {
    {2500, 500, 1700, 200},
    {3000, 600, 2200, 300},
    {3500, 700, 3000, 500},
    {5000, 800, 4000, 500}
};
unsigned char g_rawData_0097a400[64] = {
0x1b, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00,
0x35, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
0x21, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00};
// GLOBAL: FF7 0x00e72eec
int g_ChocoboRaceState = 0;
/*
 * Mechanical two-way selector over unnamed constants: returns 0x3DE for variant of zero and 0x320 otherwise.
 */
// FUNCTION: FF7 0x00772340
int __cdecl Mini_ChocoboGetRacerStatBaseByVariant(int variant)
{
    if (variant == 0) {
        return 0x3DE;
    } else {
        return 0x320;
    }
}

/*
 * Builds six racing records with shuffled lane weights, player save-derived statistics or class-
 * randomized opponents, initial course-interpolated positions, normalized facing and cleared race/animation state.
 */
// FUNCTION: FF7 0x00772357
void __cdecl Mini_ChocoboRaceBuildRacers(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[308];
        ChocoboRacerEntry *racerEntry;
        struct { unsigned char pad_to_10[0x10]; int charIndex; };
        struct { unsigned char pad_to_1C[0x1C]; int nameSrc; };
        struct { unsigned char pad_to_24[0x24]; int savedTargetX; };
        struct { unsigned char pad_to_28[0x28]; int savedTargetZ; };
        struct { unsigned char pad_to_2C[0x2C]; int savedPosX; };
        struct { unsigned char pad_to_30[0x30]; int savedPosZ; };
        struct { unsigned char pad_to_34[0x34]; int laneBlendTmp; };
        struct { unsigned char pad_to_38[0x38]; int racerSwapB; };
        struct { unsigned char pad_to_3C[0x3C]; int racerSwapA; };
        struct { unsigned char pad_to_40[0x40]; int inverseWeight; };
        struct { unsigned char pad_to_44[0x44]; int laneTmp; };
        struct { unsigned char pad_to_48[0x48]; int laneSwapA; };
        struct { unsigned char pad_to_4C[0x4C]; short racerIndex; };
        struct { unsigned char pad_to_50[0x50]; int permutationIndex; };
        struct { unsigned char pad_to_54[0x54]; int laneSwapB; };
        struct { unsigned char pad_to_58[0x58]; ChocoboRacer *racer; };
        struct { unsigned char pad_to_5C[0x5C]; VECTOR dir; };
        struct { unsigned char pad_to_6C[0x6C]; int engineContext; };
        struct { unsigned char pad_to_70[0x70]; int permutation[45]; };
        struct { unsigned char pad_to_124[0x124]; GfxFloatColorAddend addend; };
        struct { unsigned char pad_to_130[0x130]; int addendAlpha; };
    } frame;
#pragma pack(pop)

    frame.engineContext = (int)GetEngineContext();
    for (frame.racerIndex = 0; frame.racerIndex < 6; frame.racerIndex++) {
        frame.racer = &g_ChocoboRacers[frame.racerIndex];
        frame.racer->laneBlend = frame.racerIndex * 224 / 6 + 0x10;
    }
    for (frame.racerIndex = 0; frame.racerIndex < 40; frame.racerIndex++) {
        frame.racerSwapA = rand() % 6;
        frame.racerSwapB = rand() % 6;
        frame.laneBlendTmp = g_ChocoboRacers[frame.racerSwapA].laneBlend;
        g_ChocoboRacers[frame.racerSwapA].laneBlend = g_ChocoboRacers[frame.racerSwapB].laneBlend;
        g_ChocoboRacers[frame.racerSwapB].laneBlend = *(short *)&frame.laneBlendTmp;
    }
    for (frame.racerIndex = 0; frame.racerIndex < 6; frame.racerIndex++) {
        frame.racer = &g_ChocoboRacers[frame.racerIndex];
        if (g_ChocoboPlayerRides != 0 && frame.racerIndex == 0) {
            frame.racer->topSpeed = gSavemap.topSpeedLow + (gSavemap.topSpeedHigh << 8);
            frame.racer->baseSpeed = gSavemap.baseSpeedLow + (gSavemap.baseSpeedHigh << 8);
            frame.racer->acceleration = gSavemap.raceAcceleration;
            frame.racer->cooperation = gSavemap.raceCooperation;
            frame.racer->mode = rand() % 4;
            frame.racer->staminaMax = gSavemap.staminaLow + (gSavemap.staminaHigh << 8);
            frame.racer->stamina = frame.racer->staminaMax;
            frame.racer->autoPilot = 1;
            frame.racer->personality = gSavemap.racePersonality;
            frame.racer->intelligenceDiv8 = frame.racer->intelligence / 10;
            switch (gSavemap.raceChocoboColor) {
            case CHOCOBO_COLOR_GREEN:
                frame.racer->terrainPenaltyMask = CHOCOBO_TRACK_FLAG_SLOW_1;
                break;
            case CHOCOBO_COLOR_BLUE:
                frame.racer->terrainPenaltyMask = CHOCOBO_TRACK_FLAG_SLOW_2;
                break;
            case CHOCOBO_COLOR_BLACK:
            case CHOCOBO_COLOR_GOLD:
                frame.racer->terrainPenaltyMask = 0;
                break;
            default:
                frame.racer->terrainPenaltyMask = CHOCOBO_TRACK_FLAG_SLOW_1 | CHOCOBO_TRACK_FLAG_SLOW_2;
            }
            frame.racer->intelligence = gSavemap.raceIntelligence;
        } else {
            frame.racer->topSpeed = g_ChocoboClassStats[gSavemap.raceClass].topSpeedBase + rand() % g_ChocoboClassStats[gSavemap.raceClass].topSpeedRange;
            frame.racer->baseSpeed = g_ChocoboClassStats[gSavemap.raceClass].baseSpeedBase + rand() % g_ChocoboClassStats[gSavemap.raceClass].baseSpeedRange;
            frame.racer->acceleration = 50;
            frame.racer->cooperation = 0x64;
            frame.racer->mode = rand() % 4;
            frame.racer->staminaMax = g_ChocoboClassStats[gSavemap.raceClass].topSpeedBase + rand() % 300;
            frame.racer->stamina = frame.racer->staminaMax;
            frame.racer->autoPilot = 1;
            /* CHOCOBO_PERSONALITY_DASHES_WHEN_SPURRED one time in eight, else NONE */
            frame.racer->personality = ((rand() & 7) != 0 ? -2 : 0) + 2;
            frame.racer->intelligenceDiv8 = frame.racer->intelligence / 10;
            frame.racer->terrainPenaltyMask = CHOCOBO_TRACK_FLAG_SLOW_1 | CHOCOBO_TRACK_FLAG_SLOW_2;
            frame.racer->intelligence = (rand() & 1) * 50 + 50;
        }
        if (frame.racer->intelligence < 40) {
            frame.racer->racingLine = 3;
            frame.racer->steerStep = (40 - frame.racer->intelligence) / 10;
        } else if (frame.racer->intelligence < 60) {
            frame.racer->racingLine = 2;
            frame.racer->steerStep = (60 - frame.racer->intelligence) / 5;
        } else if (frame.racer->intelligence < 80) {
            frame.racer->racingLine = 1;
            frame.racer->steerStep = (80 - frame.racer->intelligence) / 5;
        } else {
            frame.racer->racingLine = 0;
            frame.racer->steerStep = (110 - frame.racer->intelligence) / 5;
        }
        frame.racer->intelligenceDiv8 = (frame.racer->intelligence) / 8;
        frame.racer->dashSegment = *(short *)&g_ChocoboTrackData->segmentTable;
        frame.racer->rank = frame.racerIndex;
        frame.racer->stepSpeed = frame.racer->baseSpeed / 2;
        frame.racer->speed = frame.racer->baseSpeed;
        frame.racer->segment = 0xA;
        frame.racer->nextSegment = 0xB;
        frame.racer->stuckFrames = 0;
        frame.racer->dashAllowed = 0xFFFF;
        frame.racer->hasStartBoost = 0;
        frame.racer->clearedModifierFlag = 0;
        frame.racer->unusedEvent7Flag = 0;
        frame.racer->locksBaseSpeed = 0;
        frame.inverseWeight = 0x100 - frame.racer->laneBlend;
        frame.racer->homeLaneBlend = frame.racer->laneBlend;
        frame.racer->posX = (g_pChocoboCourseWaypoints[frame.racer->segment].x * frame.racer->laneBlend + g_pChocoboCourseWaypoints[frame.racer->segment].auxX * frame.inverseWeight) / 256;
        frame.racer->posY = (g_pChocoboCourseWaypoints[frame.racer->segment].y * frame.racer->laneBlend + g_pChocoboCourseWaypoints[frame.racer->segment].auxY * frame.inverseWeight) / 256;
        frame.racer->posZ = (g_pChocoboCourseWaypoints[frame.racer->segment].z * frame.racer->laneBlend + g_pChocoboCourseWaypoints[frame.racer->segment].auxZ * frame.inverseWeight) / 256;
        frame.racer->startPosX = frame.racer->posX;
        frame.racer->startPosY = frame.racer->posY;
        frame.racer->startPosZ = frame.racer->posZ;
        frame.racer->targetX = (g_pChocoboCourseWaypoints[frame.racer->nextSegment].x * frame.racer->laneBlend + g_pChocoboCourseWaypoints[frame.racer->nextSegment].auxX * frame.inverseWeight) / 256;
        frame.racer->targetY = (g_pChocoboCourseWaypoints[frame.racer->nextSegment].y * frame.racer->laneBlend + g_pChocoboCourseWaypoints[frame.racer->nextSegment].auxY * frame.inverseWeight) / 256;
        frame.racer->targetZ = (g_pChocoboCourseWaypoints[frame.racer->nextSegment].z * frame.racer->laneBlend + g_pChocoboCourseWaypoints[frame.racer->nextSegment].auxZ * frame.inverseWeight) / 256;
        frame.racer->clearedHeadingX = 0;
        frame.racer->heading = 0;
        frame.racer->clearedHeadingZ = 0;
        frame.racer->clearedTargetHeadingX = 0;
        frame.racer->targetHeading = 0;
        frame.racer->clearedTargetHeadingZ = 0;
        frame.racer->heading = ((short (__cdecl *)(int fromPos, int toPos))Mini_ChocoboHeadingFromPositionDelta)((int)&g_pChocoboCourseWaypoints[0x800], (int)&g_pChocoboCourseWaypoints[0x801]); /* reads eax after a void call, as the original does */
        frame.racer->place = 0;
        frame.racer->animPhase = 0;
        frame.racer->stepRate = frame.racer->stepSpeed / g_ChocoboRacerStepDivisor;
        frame.racer->finished = 0;
        frame.racer->targetIndex = 0;
        frame.dir.vx = frame.racer->targetX - frame.racer->posX;
        frame.dir.vy = frame.racer->targetY - frame.racer->posY;
        frame.dir.vz = frame.racer->targetZ - frame.racer->posZ;
        NormalizeVectorFixed(&frame.dir, &frame.dir);
        frame.racer->targetHeading = Mini_ChocoboHeadingFromXZ(*(short *)&frame.dir.vx, *(short *)&frame.dir.vz);
        frame.racer->heading = frame.racer->targetHeading;
    }
    if (gSavemap.racerEnabled != 0) {
        frame.savedPosX = *(int *)&g_ChocoboRacers[1].posX;
        frame.savedPosZ = *(int *)&g_ChocoboRacers[1].posZ;
        frame.savedTargetX = *(int *)&g_ChocoboRacers[1].targetX;
        frame.savedTargetZ = *(int *)&g_ChocoboRacers[1].targetZ;
        g_ChocoboRacers[1] = g_ChocoboRacers[0];
        *(int *)&g_ChocoboRacers[1].posX = frame.savedPosX;
        *(int *)&g_ChocoboRacers[1].posZ = frame.savedPosZ;
        *(int *)&g_ChocoboRacers[1].startPosX = frame.savedPosX;
        *(int *)&g_ChocoboRacers[1].startPosZ = frame.savedPosZ;
        *(int *)&g_ChocoboRacers[1].targetX = frame.savedTargetX;
        *(int *)&g_ChocoboRacers[1].targetZ = frame.savedTargetZ;
        g_ChocoboRacers[1].staminaMax += g_ChocoboRacers[1].staminaMax / 4;
        g_ChocoboRacers[1].stamina += g_ChocoboRacers[1].stamina / 4;
        g_ChocoboRacers[1].baseSpeed += (short)(g_ChocoboRacers[1].baseSpeed / 10);
        g_ChocoboRacers[1].topSpeed += (short)(g_ChocoboRacers[1].topSpeed / 10);
        g_ChocoboRacers[1].acceleration = 100;
        g_ChocoboRacers[1].intelligence = 0x64;
        g_ChocoboRacers[1].tableIndex = 2;
        g_ChocoboRacers[1].mode = CHOCOBO_RACER_MODE_STEADY_HOME_LANE;
        g_ChocoboRacers[1].terrainPenaltyMask = 0;
        g_ChocoboRacers[1].tableIndex = 9;
        g_ChocoboRacers[1].lane = 1;
    }
    for (frame.permutationIndex = 0; frame.permutationIndex < 45; frame.permutationIndex++) {
        frame.permutation[frame.permutationIndex] = frame.permutationIndex;
    }
    for (frame.permutationIndex = 0; frame.permutationIndex < 200; frame.permutationIndex++) {
        frame.laneSwapA = rand() % 45;
        frame.laneSwapB = rand() % 45;
        frame.laneTmp = frame.permutation[frame.laneSwapA];
        frame.permutation[frame.laneSwapA] = frame.permutation[frame.laneSwapB];
        frame.permutation[frame.laneSwapB] = frame.laneTmp;
    }
    for (frame.racerIndex = 0; frame.racerIndex < 6; frame.racerIndex++) {
        if (gSavemap.racerEnabled != 0 && frame.racerIndex == 1) {
            for (frame.charIndex = 0; frame.charIndex < 8; frame.charIndex++) {
                *(char *)((char *)&g_ChocoboRacers[0].nameText + (frame.racerIndex * 0xA4 + frame.charIndex)) = *(char *)((char *)&g_ChocoboTeiohName + frame.charIndex);
            }
        } else {
            frame.nameSrc = frame.permutation[frame.racerIndex] * 7 + (int)((const unsigned char *)&g_ChocoboJockeyNames);
            for (frame.charIndex = 0; frame.charIndex < 6; frame.charIndex++) {
                *(char *)((char *)&g_ChocoboRacers[0].nameText + (frame.racerIndex * 0xA4 + frame.charIndex)) = *(char *)frame.nameSrc;
                frame.nameSrc++;
            }
            *(char *)((char *)&g_ChocoboRacers[0].nameText + (frame.racerIndex * 0xA4 + frame.charIndex)) = 0xFF;
            if (g_ChocoboPlayerRides != 0 && frame.racerIndex != 0) {
                Mini_ChocoboRaceApplyTwoRacerEventCodes(frame.racerIndex, 0, 0);
            }
        }
    }
    if (g_ChocoboPlayerRides != 0) {
        g_ChocoboRacers[0].nameText = *(int *)&gSavemap.chocoboName;
        g_ChocoboRacers[0].nameTail = gSavemap.chocoboNameTail;
        g_ChocoboRacers[0].nameTerminator = 0xFF;
    }
    for (frame.racerIndex = 0; frame.racerIndex < 6; frame.racerIndex++) {
        frame.racer = &g_ChocoboRacers[frame.racerIndex];
        frame.racerEntry = &g_pChocoboRacerTable[frame.racer->tableIndex];
        if (gSavemap.racerEnabled != 0 && frame.racerIndex == 1) {
            continue;
        }
        if (g_ChocoboPlayerRides != 0 && frame.racerIndex == 0) {
            continue;
        }
        switch (frame.racer->mode) {
        case CHOCOBO_RACER_MODE_STEADY:
            frame.racerEntry->colorB = 0;
            frame.racerEntry->colorG = 0;
            frame.racerEntry->colorR = 0;
            frame.addend.red = (float)*(unsigned char *)&frame.racerEntry->colorR / 256.0f;
            frame.addend.green = (float)*(unsigned char *)&frame.racerEntry->colorG / 256.0f;
            frame.addend.blue = (float)*(unsigned char *)&frame.racerEntry->colorB / 256.0f;
            frame.addendAlpha = FLOAT_ONE_BITS;
            Gfx_AddColorToModelVertexColors(&frame.addend, (PolygonModelVertexColors *)frame.racerEntry->player->skeleton, frame.engineContext);
            break;
        case CHOCOBO_RACER_MODE_DASH_WHEN_LAST:
            frame.racerEntry->colorR = 0x40;
            frame.racerEntry->colorG = 0x40;
            frame.racerEntry->colorB = 0;
            frame.addend.red = (float)*(unsigned char *)&frame.racerEntry->colorR / 256.0f;
            frame.addend.green = (float)*(unsigned char *)&frame.racerEntry->colorG / 256.0f;
            frame.addend.blue = (float)*(unsigned char *)&frame.racerEntry->colorB / 256.0f;
            frame.addendAlpha = FLOAT_ONE_BITS;
            Gfx_AddColorToModelVertexColors(&frame.addend, (PolygonModelVertexColors *)frame.racerEntry->player->skeleton, frame.engineContext);
            break;
        case CHOCOBO_RACER_MODE_EARLY_SPRINT:
            frame.racerEntry->colorR = 0x40;
            frame.racerEntry->colorG = 0x10;
            frame.racerEntry->colorB = 0;
            frame.addend.red = (float)*(unsigned char *)&frame.racerEntry->colorR / 256.0f;
            frame.addend.green = (float)*(unsigned char *)&frame.racerEntry->colorG / 256.0f;
            frame.addend.blue = (float)*(unsigned char *)&frame.racerEntry->colorB / 256.0f;
            frame.addendAlpha = FLOAT_ONE_BITS;
            Gfx_AddColorToModelVertexColors(&frame.addend, (PolygonModelVertexColors *)frame.racerEntry->player->skeleton, frame.engineContext);
            break;
        case CHOCOBO_RACER_MODE_STEADY_HOME_LANE:
            frame.racerEntry->colorR = 0;
            frame.racerEntry->colorG = 0x10;
            frame.racerEntry->colorB = 0x40;
            frame.addend.red = (float)*(unsigned char *)&frame.racerEntry->colorR / 256.0f;
            frame.addend.green = (float)*(unsigned char *)&frame.racerEntry->colorG / 256.0f;
            frame.addend.blue = (float)*(unsigned char *)&frame.racerEntry->colorB / 256.0f;
            frame.addendAlpha = FLOAT_ONE_BITS;
            Gfx_AddColorToModelVertexColors(&frame.addend, (PolygonModelVertexColors *)frame.racerEntry->player->skeleton, frame.engineContext);
        }
    }
}
/*
 * Calls Mini_ChocoboRaceApplyRacerEventCode for two consecutive event code arguments.
 */
// FUNCTION: FF7 0x00773487
void __cdecl Mini_ChocoboRaceApplyTwoRacerEventCodes(int racerIndex, int eventCode0, int eventCode1)
{
    Mini_ChocoboRaceApplyRacerEventCode(racerIndex, eventCode0);
    Mini_ChocoboRaceApplyRacerEventCode(racerIndex, eventCode1);
}
/*
 * Applies a racer modifier (enum ChocoboRacerEvent) to the indexed racer record. Code 7 sets unusedEvent7Flag
 * and code 0xD racing line 3, whose effects are not known.
 */
// FUNCTION: FF7 0x007734ac
void __cdecl Mini_ChocoboRaceApplyRacerEventCode(int racerIndex, int actionCode)
{
    ChocoboRacer *racerState;

    racerState = &g_ChocoboRacers[racerIndex];
    switch (actionCode) {
    case CHOCOBO_RACER_EVENT_LATE_DASH:
        racerState->dashSegment = g_ChocoboTrackData->segmentTable + 0x14;
        break;
    case CHOCOBO_RACER_EVENT_MORE_STAMINA:
        racerState->staminaMax += (racerState->staminaMax << 2) / 3;
        racerState->stamina = racerState->staminaMax;
        break;
    case CHOCOBO_RACER_EVENT_EARLY_DASH:
        racerState->dashSegment = g_ChocoboTrackData->segmentTable - 0x14;
        break;
    case CHOCOBO_RACER_EVENT_DASH_WHEN_SPURRED:
        racerState->personality = CHOCOBO_PERSONALITY_DASHES_WHEN_SPURRED;
        break;
    case CHOCOBO_RACER_EVENT_NO_DASH:
        racerState->dashAllowed = 0;
        break;
    case CHOCOBO_RACER_EVENT_SMART:
        racerState->intelligence = 0x64;
        racerState->racingLine = 0;
        racerState->mode = CHOCOBO_RACER_MODE_STEADY_HOME_LANE;
        break;
    case CHOCOBO_RACER_EVENT_FLAG_9C:
        racerState->unusedEvent7Flag = 0xFFFF;
        break;
    case CHOCOBO_RACER_EVENT_HALF_STAMINA:
        racerState->staminaMax = ((racerState->staminaMax) / 2);
        racerState->stamina = racerState->staminaMax;
        break;
    case CHOCOBO_RACER_EVENT_IGNORE_SLOW_1:
        racerState->terrainPenaltyMask &= CHOCOBO_TRACK_FLAG_SLOW_2;
        break;
    case CHOCOBO_RACER_EVENT_IGNORE_SLOW_2:
        racerState->terrainPenaltyMask &= CHOCOBO_TRACK_FLAG_SLOW_1;
        break;
    case CHOCOBO_RACER_EVENT_START_BOOST:
        racerState->hasStartBoost = 0xFFFF;
        if ((rand() & 1) != 0) {
            racerState->staminaMax = ((racerState->staminaMax) / 2);
        }
        break;
    case CHOCOBO_RACER_EVENT_LOCK_BASE_SPEED:
        racerState->locksBaseSpeed = 0xFFFF;
        break;
    case CHOCOBO_RACER_EVENT_OUTER_RACING_LINE:
        racerState->racingLine = 3;
    }
}
/*
 * Sorts six participant records into rank order and calls per-racer animation and route update helpers.
 */
// FUNCTION: FF7 0x00773668
void __cdecl Mini_ChocoboRaceRankAndUpdateRacers(void)
{
    int racerOrder[6];
    int i;
    int j;
    int segments[6];
    int swapTemp;
    for (i = 0; i < 6; i++) {
        segments[i] = g_ChocoboRacers[i].segment;
        racerOrder[i] = i;
    }
    for (i = 0; i < 6; i++) {
        for (j = i; j < 6; j++) {
            if (segments[i] < segments[j]) {
                swapTemp = segments[i];
                segments[i] = segments[j];
                segments[j] = swapTemp;
                swapTemp = racerOrder[i];
                racerOrder[i] = racerOrder[j];
                racerOrder[j] = swapTemp;
            }
        }
    }
    for (i = 0; i < 6; i++) {
        g_ChocoboRacers[racerOrder[i]].rank = (short)i;
    }
    g_ChocoboRaceState = racerOrder[5];
    for (i = 0; i < 6; i++) {
        Mini_ChocoboRaceUpdateRacerAnimationAndCollisions((short)i);
    }
    if (g_ChocoboPlayerRides != 0)
        Mini_ChocoboRaceAdvanceSelectedRacerAlongRoute(1);
    else
        Mini_ChocoboRaceAdvanceSelectedRacerAlongRoute(0);
}
/*
 * Updates animation frames, tests pairwise racer proximity and collisions, and submits 3D model transforms.
 */
// FUNCTION: FF7 0x007737b4
void __cdecl Mini_ChocoboRaceUpdateRacerAnimationAndCollisions(short racerIndex)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        int soundArg;
        struct { unsigned char pad_to_4[0x4]; int selfStepDivisor; };
        struct { unsigned char pad_to_8[0x8]; int pushHeading; };
        struct { unsigned char pad_to_C[0xC]; VECTOR dir; };
        struct { unsigned char pad_to_1C[0x1C]; ChocoboRacer *otherRacer; };
        struct { unsigned char pad_to_20[0x20]; int otherStepDivisor; };
        struct { unsigned char pad_to_24[0x24]; ChocoboRacerEntry *racerRecord; };
        struct { unsigned char pad_to_28[0x28]; struct { short x, y; } screenPos; };
        struct { unsigned char pad_to_30[0x30]; int deltaZ; };
        struct { unsigned char pad_to_34[0x34]; ChocoboRacerEntry *laneState; };
        struct { unsigned char pad_to_38[0x38]; int otherIndex; };
        struct { unsigned char pad_to_3C[0x3C]; int distance; };
        struct { unsigned char pad_to_40[0x40]; int depth; };
        struct { unsigned char pad_to_44[0x44]; int projFlags; };
        struct { unsigned char pad_to_48[0x48]; ChocoboRacer *racer; };
        struct { unsigned char pad_to_4C[0x4C]; int projScale; };
    } frame;
#pragma pack(pop)

    frame.racer = &g_ChocoboRacers[racerIndex];
    if (g_ChocoboRaceAdvanceEnabled == 0) {
        frame.racer->targetIndex = 3;
        frame.racer->stepRate = 256;
        frame.racer->stepSpeed = 10;
    } else if (frame.racer->stepSpeed != 0) {
        Mini_ChocoboRaceAdvanceRacerAlongCourse((short)racerIndex);
        frame.racer->stepRate = frame.racer->stepSpeed / g_ChocoboRacerStepDivisor;
    } else {
        g_ChocoboRaceTimer++;
        if (gSavemap.racerEnabled != 0 && frame.racer->lane == 1) {
            /* Literal finishing placement rank (1st/2nd/6th). */
            switch (frame.racer->place) {
            case CHOCOBO_PLACE_1ST:
                frame.racer->targetIndex = 2;
                break;
            case CHOCOBO_PLACE_2ND:
                frame.racer->targetIndex = 3;
                break;
            default:
                frame.racer->targetIndex = 4;
            }
        } else {
            /* Literal finishing placement rank (1st/2nd/6th). */
            switch (frame.racer->place) {
            case CHOCOBO_PLACE_1ST:
                frame.racer->targetIndex = 2;
                break;
            case CHOCOBO_PLACE_6TH:
                frame.racer->targetIndex = 4;
                break;
            default:
                frame.racer->targetIndex = 3;
            }
        }
        frame.racer->stepRate = 256;
    }
    frame.racer->animPhase += frame.racer->stepRate;
    frame.racer->trackPhase += frame.racer->stepRate;
    frame.laneState = &g_pChocoboRacerTable[frame.racer->lane];
    frame.racerRecord = &g_pChocoboRacerTable[frame.racer->tableIndex];
    if (frame.racer->animPhase >> 8 >= g_ChocoboIconOffsets[frame.racer->targetIndex]) {
        if (frame.racer->lastIndex != frame.racer->targetIndex) {
            frame.racer->lastIndex = frame.racer->targetIndex;
            frame.racer->animPhase = 0;
        } else {
            frame.racer->animPhase &= 0xFF;
        }
    }
    for (frame.otherIndex = 0; frame.otherIndex < 6; frame.otherIndex++) {
        if (frame.otherIndex == racerIndex) {
            continue;
        }
        frame.otherRacer = &g_ChocoboRacers[frame.otherIndex];
        frame.distance = frame.racer->posX - frame.otherRacer->posX;
        frame.deltaZ = frame.racer->posZ - frame.otherRacer->posZ;
        frame.distance = SqrtToInt(frame.distance * frame.distance + frame.deltaZ * frame.deltaZ);
        if (frame.distance < 150) {
            if (frame.otherRacer->finished != 0) {
                frame.otherRacer->stepSpeed = frame.racer->stepSpeed;
                frame.racer->stepSpeed = 10;
            }
            if (frame.otherRacer->stepSpeed == 0 || frame.racer->stepSpeed == 0) {
                continue;
            }
            frame.dir.vx = frame.racer->posX - frame.otherRacer->posX;
            frame.dir.vy = 0;
            frame.dir.vz = frame.racer->posZ - frame.otherRacer->posZ;
            frame.selfStepDivisor = 0x80000 / frame.otherRacer->stepSpeed;
            NormalizeVectorFixed(&frame.dir, &frame.dir);
            frame.otherStepDivisor = 0x80000 / frame.racer->stepSpeed;
            frame.pushHeading = (short)Mini_ChocoboHeadingFromXZ(*(short *)&frame.dir.vx, *(short *)&frame.dir.vz);
            if (frame.racer->lane == 0 && frame.racer->countdown < 0) {
                frame.racer->countdown = 30;
                frame.soundArg = 0x7F - (g_ChocoboRacerScreenDepth[racerIndex] >> 5);
                g_pChocoboSoundCommand->soundCommand = 0xA3;
                g_pChocoboSoundCommand->soundArg = frame.soundArg;
                SoundDispatchCommand((short)g_pChocoboSoundCommand->soundCommand, g_pChocoboSoundCommand->soundArg, 0, 0, 0, 0, 0, 0, 0);
                g_pChocoboSoundCommand->soundCommand = 0x2B;
                g_pChocoboSoundCommand->soundArg = frame.racer->laneBlend / 2;
                g_pChocoboSoundCommand->soundParam1 = (rand() & 1) + 276;
                SoundDispatchCommand((short)g_pChocoboSoundCommand->soundCommand, g_pChocoboSoundCommand->soundArg, g_pChocoboSoundCommand->soundParam1, 0, 0, 0, 0, 0, 0);
            }
            if (frame.selfStepDivisor != 0) {
                frame.racer->posX += (short)(SinFixed(frame.pushHeading) / frame.selfStepDivisor);
                frame.racer->posZ += (short)(-CosFixed(frame.pushHeading) / frame.selfStepDivisor);
                frame.racer->stepSpeed -= (short)(frame.racer->stepSpeed / 8);
                if (frame.racer->stepSpeed < 100) {
                    frame.racer->stepSpeed = 100;
                }
            }
            if (frame.otherStepDivisor != 0) {
                frame.otherRacer->posX -= (short)(SinFixed(frame.pushHeading) / frame.otherStepDivisor);
                frame.otherRacer->posZ -= (short)(-CosFixed(frame.pushHeading) / frame.otherStepDivisor);
            }
        }
        if (frame.racer->personality == CHOCOBO_PERSONALITY_DASHES_WHEN_SPURRED && frame.distance < 170) {
            rand();
            if (DEAD_IN_RETAIL != 0) {
                frame.racer->dashTimer = 4;
            }
        }
    }
    g_ChocoboRacerOnScreen[racerIndex] = 0;
    frame.depth = RotTransPers((SVECTOR *)&frame.racer->posX, &frame.screenPos.x, &frame.projScale, &frame.projFlags);
    if (frame.depth < 2200 && frame.screenPos.x > -20 && frame.screenPos.x < 350 && frame.screenPos.y > 0 && frame.screenPos.y < 320) {
        frame.racerRecord->headingFlipped = 0;
        frame.laneState->headingFlipped = 0;
        frame.laneState->yaw = 0x100 - (frame.racer->heading >> 4);
        frame.racerRecord->yaw = frame.laneState->yaw;
        frame.laneState->posX = frame.racer->posX;
        frame.racerRecord->posX = frame.laneState->posX;
        frame.laneState->posY = frame.racer->posY;
        frame.racerRecord->posY = frame.laneState->posY;
        frame.laneState->posZ = frame.racer->posZ;
        frame.racerRecord->posZ = frame.laneState->posZ;
        g_ChocoboRacerOnScreen[racerIndex] = 1;
        g_ChocoboRacerScreenDepth[racerIndex] = frame.depth;
    }
}
/*
 * Updates indexed racer record against track strips, handles steering, stamina, track events, and finishes.
 */
// FUNCTION: FF7 0x00773dd8
void __cdecl Mini_ChocoboRaceAdvanceRacerAlongCourse(short racerIndex)
{
    int distance;
    int nodeSteerDelta;
    ChocoboRacer *racerPtr;
    VECTOR travelX;
    VECTOR planeNormalX;
    int lookaheadZ;
    int padEntry; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int place;
    VECTOR edgeDx;
    ChocoboCourseWaypoint *pNode;
    int loopIndex;
    VECTOR toTarget;
    int waypointIndex;
    int weightSecond;
    int segGap;
    int nextX;
    int restBlend;
    VECTOR toLookX;
    int scanEnd;
    short workEvent;
    int lookaheadY;
    ChocoboWatPoint *pointPtr;

    racerPtr = &g_ChocoboRacers[racerIndex];
    if (racerPtr->finished != 0) {
        racerPtr->stepSpeed -= (short)((racerPtr->place + 1) * 80 + ((racerPtr->stepSpeed) / 64));
        if (racerPtr->stepSpeed < 100) {
            racerPtr->stepSpeed = 0;
        } else {
            racerPtr->targetIndex = 0;
            racerPtr->dashTimer = 0;
            if (racerPtr->personality == CHOCOBO_PERSONALITY_DASHES_WHEN_SPURRED) {
                racerPtr->animPhase = 0;
                racerPtr->targetIndex = 1;
                racerPtr->stepSpeed -= 50;
            }
        }
    }
    distance = racerPtr->stepSpeed;
    pNode = &g_pChocoboCourseWaypoints[racerPtr->nextSegment];
    pointPtr = (ChocoboWatPoint *)&g_pChocoboCourseWaypoints[racerPtr->nextSegment].auxX;
    edgeDx.vx = pointPtr->x - pNode->x;
    edgeDx.vy = 0;
    edgeDx.vz = pointPtr->z - pNode->z;
    travelX.vx = racerPtr->posX - pNode->x;
    travelX.vy = 0;
    travelX.vz = racerPtr->posZ - pNode->z;
    CrossProductFixed12(&edgeDx, &travelX, &edgeDx);
    if (edgeDx.vy >= 0) {
        racerPtr->stuckFrames = 0;
        scanEnd = racerPtr->segment + 0x14;
        for (nodeSteerDelta = racerPtr->segment - 5; nodeSteerDelta < scanEnd; nodeSteerDelta++) {
            segGap = (nodeSteerDelta + g_ChocoboCourseSegmentCount) % g_ChocoboCourseSegmentCount;
            pNode = &g_pChocoboCourseWaypoints[segGap];
            pointPtr = (ChocoboWatPoint *)&g_pChocoboCourseWaypoints[segGap].auxX;
            edgeDx.vx = pointPtr->x - pNode->x;
            edgeDx.vy = 0;
            edgeDx.vz = pointPtr->z - pNode->z;
            travelX.vx = racerPtr->posX - pNode->x;
            travelX.vy = 0;
            travelX.vz = racerPtr->posZ - pNode->z;
            CrossProductFixed12(&edgeDx, &travelX, &edgeDx);
            if (edgeDx.vy < 0) {
                break;
            }
        }
        racerPtr->segment = racerPtr->nextSegment;
        racerPtr->nextSegment = segGap + g_ChocoboCourseSegmentCount;
        racerPtr->nextSegment = racerPtr->nextSegment % g_ChocoboCourseSegmentCount;
        if (racerIndex != 0 || (g_ChocoboPlayerRides == 0 || racerPtr->autoPilot != 0)) {
            if (rand() < 5000) {
                switch (racerPtr->mode) {
                case CHOCOBO_RACER_MODE_STEADY_HOME_LANE:
                    if (racerPtr->dashSegment < racerPtr->segment) {
                        racerPtr->laneBlend = (racerPtr->laneBlend * 7 + racerPtr->homeLaneBlend) / 8;
                    }
                case CHOCOBO_RACER_MODE_STEADY:
                    for (nodeSteerDelta = 0; nodeSteerDelta < 6; nodeSteerDelta++) {
                        if (g_ChocoboRacers[nodeSteerDelta].rank == 0) {
                            break;
                        }
                    }
                    if (nodeSteerDelta != racerIndex) {
                        if (g_ChocoboRacers[nodeSteerDelta].rank - racerPtr->rank > 30) {
                            racerPtr->actionTimer = 10;
                        } else if (racerPtr->speed > (racerPtr->baseSpeed * 5) / 4) {
                            racerPtr->actionTimer = 0xFFFB;
                        } else if (racerPtr->speed < racerPtr->baseSpeed) {
                            racerPtr->actionTimer = 3;
                        } else {
                            racerPtr->actionTimer = 0;
                        }
                    }
                    break;
                case CHOCOBO_RACER_MODE_DASH_WHEN_LAST:
                    if (g_ChocoboRaceState == racerIndex) {
                        if (racerPtr->dashTimer == 0) {
                            racerPtr->dashTimer = rand() & 0x28;
                        }
                    } else if (racerPtr->speed > (racerPtr->baseSpeed * 5) / 4) {
                        racerPtr->actionTimer = 0xFFFB;
                    } else if (racerPtr->speed < racerPtr->baseSpeed * 9 / 10) {
                        racerPtr->actionTimer = 3;
                    } else {
                        racerPtr->actionTimer = 0;
                    }
                    break;
                case CHOCOBO_RACER_MODE_EARLY_SPRINT:
                    if (racerPtr->stamina > racerPtr->staminaMax / 3) {
                        racerPtr->actionTimer = 20;
                        if (racerPtr->dashSegment > racerPtr->segment && racerPtr->dashAllowed != 0) {
                            racerPtr->dashTimer = 10;
                        }
                    } else if (racerPtr->speed > racerPtr->baseSpeed * 10 / 9) {
                        racerPtr->actionTimer = 0xFFF6;
                    } else {
                        racerPtr->actionTimer = 2;
                    }
                }
                if (racerPtr->dashSegment < racerPtr->segment && racerPtr->mode != CHOCOBO_RACER_MODE_EARLY_SPRINT) {
                    if (racerPtr->dashAllowed != 0) {
                        racerPtr->dashTimer = 50;
                    }
                    if (racerPtr->dashTimer != 0) {
                        g_ChocoboDashActiveFlag = 0xFFFFFFFF;
                    }
                    racerPtr->laneBlend = (racerPtr->laneBlend * 7 + racerPtr->homeLaneBlend) / 8;
                }
            }
            racerPtr->smoothedLaneBlend = (racerPtr->smoothedLaneBlend + racerPtr->laneBlend) / 2;
        } else {
            if (racerPtr->dashSegment < racerPtr->segment && racerPtr->dashTimer != 0) {
                g_ChocoboDashActiveFlag = 0xFFFFFFFF;
            }
            if (racerPtr->stamina > 0 && racerPtr->finished == 0) {
                if ((*(int *)&g_ChocoboPackedInputLatches & INPUT_SQUARE) != 0) {
                    racerPtr->actionTimer = 2;
                }
                if ((*(int *)&g_ChocoboPackedInputLatches & INPUT_CROSS) != 0) {
                    racerPtr->actionTimer = 0xFFFE;
                }
                if ((*(int *)&g_ChocoboPackedInputLatches & INPUT_CIRCLE) != 0 && racerPtr->stamina > 0) {
                    racerPtr->dashTimer = 2;
                }
            }
        }
        if ((rand() & 0x3F) < racerPtr->cooperation && racerPtr->dashTimer == 0) {
            racerPtr->speed += (short)(racerPtr->speed * racerPtr->actionTimer / 100);
            nodeSteerDelta = (racerPtr->baseSpeed * 5) / 4;
            if (racerPtr->speed > nodeSteerDelta) {
                racerPtr->speed = nodeSteerDelta;
            }
            if (racerPtr->stamina < 0) {
                racerPtr->speed = racerPtr->baseSpeed;
            }
            if (racerPtr->speed > racerPtr->baseSpeed) {
                racerPtr->stamina -= (racerPtr->speed - racerPtr->baseSpeed) / 128;
            }
            if (racerPtr->actionTimer > 0) {
                (racerPtr->actionTimer)--;
            } else if (racerPtr->actionTimer < 0) {
                (racerPtr->actionTimer)++;
            }
        }
        if (racerPtr->dashTimer != 0) {
            (racerPtr->dashTimer)--;
            if (racerPtr->dashTimer < 0) {
                racerPtr->dashTimer = 0;
            }
            racerPtr->stamina -= racerPtr->staminaMax / 300;
            if (racerPtr->stamina <= 0) {
                racerPtr->dashTimer = 0;
                racerPtr->stamina = 0;
            }
            if (racerPtr->dashTimer == 0) {
                racerPtr->targetIndex = 0;
            } else {
                racerPtr->targetIndex = 1;
            }
            if (racerPtr->hasStartBoost != 0 && g_ChocoboStartTimer < 150) {
                racerPtr->currentSpeed = 5500;
            } else {
                racerPtr->currentSpeed = racerPtr->topSpeed;
            }
        } else if (racerPtr->locksBaseSpeed != 0) {
            racerPtr->currentSpeed = racerPtr->baseSpeed;
        } else {
            racerPtr->currentSpeed += (short)((racerPtr->speed - racerPtr->currentSpeed) * racerPtr->acceleration / 100);
        }
        if (racerPtr->rank > 1 && racerPtr->personality == CHOCOBO_PERSONALITY_SLOWS_WHEN_BEHIND) {
            racerPtr->currentSpeed = racerPtr->currentSpeed * 9 / 10;
        }
        if (racerPtr->autoPilot == 0 && g_ChocoboPlayerRides != 0) {
            if ((rand() & 0x3F) < racerPtr->cooperation) {
                if ((*(int *)&g_ChocoboPackedInputLatches & INPUT_LEFT) != 0) {
                    racerPtr->laneBlend += (short)(racerPtr->steerStep + ((racerPtr->cooperation) / 32));
                }
                if ((*(int *)&g_ChocoboPackedInputLatches & INPUT_RIGHT) != 0) {
                    racerPtr->laneBlend -= (short)(racerPtr->steerStep + ((racerPtr->cooperation) / 32));
                }
            }
            if ((*(int *)&g_ChocoboPackedInputLatches & INPUT_ANY_BUTTON) != 0) {
                if (racerPtr->cooperation < (rand() & 0x1F)) {
                    nodeSteerDelta = 0x19 - ((racerPtr->cooperation) / 2);
                    racerPtr->laneBlend += (short)(((rand() & 3) - 1) * nodeSteerDelta);
                    if (racerPtr->stamina > 0) {
                        if ((rand() & 7) == 0) {
                            racerPtr->stamina -= racerPtr->staminaMax / 500;
                            if (racerPtr->personality == CHOCOBO_PERSONALITY_DASHES_WHEN_SPURRED) {
                                racerPtr->dashTimer = 10;
                            } else {
                                racerPtr->stepSpeed -= (short)((racerPtr->stepSpeed) / 4);
                            }
                        }
                    }
                }
            }
        } else {
            nodeSteerDelta = *(unsigned char *)((char *)(g_ChocoboTrackData->racingLineTable + racerPtr->segment * 8) + racerPtr->racingLine);
            nodeSteerDelta -= racerPtr->laneBlend;
            if (nodeSteerDelta < 0) {
                if (-nodeSteerDelta > racerPtr->steerStep) {
                    nodeSteerDelta = -racerPtr->steerStep + racerPtr->steerBias;
                }
            } else if (nodeSteerDelta > racerPtr->steerStep) {
                nodeSteerDelta = racerPtr->steerStep + racerPtr->steerBias;
            }
            for (scanEnd = 0; scanEnd < 6; scanEnd++) {
                if (scanEnd == racerIndex) {
                    continue;
                }
                segGap = racerPtr->segment - g_ChocoboRacers[scanEnd].segment;
                segGap = segGap < 0 ? -segGap : segGap;
                if (segGap < 20) {
                    segGap = racerPtr->laneBlend - g_ChocoboRacers[scanEnd].laneBlend;
                    segGap = segGap < 0 ? -segGap : segGap;
                    if ((segGap < 16 && (rand() & 0x1F) == 0) || racerPtr->finished != 0) {
                        nodeSteerDelta += (rand() & 7) - 3 << 4;
                    }
                }
            }
            racerPtr->laneBlend += nodeSteerDelta;
        }
        if (racerPtr->finished == 0) {
            racerPtr->stepSpeed = ((*(char *)((char *)(g_ChocoboTrackData->racingLineTable + racerPtr->segment * 8) + racerPtr->racingLine + 4) * racerPtr->currentSpeed) / 1024) + racerPtr->currentSpeed;
            if (racerPtr->stamina <= 5) {
                racerPtr->stepSpeed = racerPtr->stepSpeed - (racerPtr->stepSpeed) / 4;
                racerPtr->targetIndex = 5;
            } else if (g_ChocoboRaceState == racerIndex) {
                racerPtr->stepSpeed += (short)((racerPtr->stepSpeed) / 8);
            }
        }
        if (racerPtr->dashTimer == 0 && racerPtr->rank < 3) {
            if (racerPtr->mode != CHOCOBO_RACER_MODE_EARLY_SPRINT) {
                if (racerPtr->segment < racerPtr->dashSegment) {
                    racerPtr->stepSpeed -= (short)(racerPtr->stepSpeed / (racerPtr->rank + 7));
                }
            } else if (racerPtr->segment < racerPtr->dashSegment) {
                racerPtr->stepSpeed -= (short)(racerPtr->stepSpeed / (racerPtr->rank + 8));
            }
        }
        nodeSteerDelta = racerPtr->terrainPenaltyMask & g_pChocoboCourseWaypoints[racerPtr->segment].flags;
        if (nodeSteerDelta != 0) {
            racerPtr->stepSpeed -= (short)((racerPtr->stepSpeed * nodeSteerDelta) / 4);
        }
        if (racerPtr->laneBlend > 0xD8) {
            racerPtr->laneBlend = 0xD8;
        } else if (racerPtr->laneBlend < 0x30) {
            racerPtr->laneBlend = 0x30;
        }
        weightSecond = 0x100 - racerPtr->laneBlend;
        waypointIndex = racerPtr->nextSegment;
        racerPtr->targetX = (g_pChocoboCourseWaypoints[waypointIndex].x * racerPtr->laneBlend + g_pChocoboCourseWaypoints[waypointIndex].auxX * weightSecond) / 256;
        racerPtr->targetY = (g_pChocoboCourseWaypoints[waypointIndex].y * racerPtr->laneBlend + g_pChocoboCourseWaypoints[waypointIndex].auxY * weightSecond) / 256;
        racerPtr->targetZ = (g_pChocoboCourseWaypoints[waypointIndex].z * racerPtr->laneBlend + g_pChocoboCourseWaypoints[waypointIndex].auxZ * weightSecond) / 256;
        waypointIndex = (racerPtr->nextSegment + 1) % g_ChocoboCourseSegmentCount;
        nextX = (g_pChocoboCourseWaypoints[waypointIndex].x * racerPtr->laneBlend + g_pChocoboCourseWaypoints[waypointIndex].auxX * weightSecond) / 256;
        lookaheadY = (g_pChocoboCourseWaypoints[waypointIndex].y * racerPtr->laneBlend + g_pChocoboCourseWaypoints[waypointIndex].auxY * weightSecond) / 256;
        lookaheadZ = (g_pChocoboCourseWaypoints[waypointIndex].z * racerPtr->laneBlend + g_pChocoboCourseWaypoints[waypointIndex].auxZ * weightSecond) / 256;
        toLookX.vx = nextX - racerPtr->posX;
        toLookX.vy = 0;
        toLookX.vz = lookaheadZ - racerPtr->posZ;
        NormalizeVectorFixed(&toLookX, &toLookX);
        racerPtr->targetHeading = Mini_ChocoboHeadingFromXZ((short)toLookX.vx, (short)toLookX.vz);
        workEvent = g_pChocoboCourseWaypoints[racerPtr->segment].pendingEvent;
        if (workEvent != 0) {
            Mini_ChocoboRaceSetCameraModeFromRouteEvent((short)workEvent, (short)racerIndex);
            g_pChocoboCourseWaypoints[racerPtr->segment - 1].pendingEvent = 0;
            g_pChocoboCourseWaypoints[racerPtr->segment].pendingEvent = 0;
            g_pChocoboCourseWaypoints[racerPtr->segment + 1].pendingEvent = 0;
        }
        workEvent = g_pChocoboCourseWaypoints[racerPtr->segment].eventId;
        if (workEvent != 0) {
            Mini_ChocoboRaceSetSceneryAnimationStep((short)workEvent);
        }
    } else {
        restBlend = 0x100 - racerPtr->laneBlend;
        (racerPtr->stuckFrames)++;
        if (racerPtr->stuckFrames > 15 && g_ChocoboRaceAdvanceEnabled != 0 && racerPtr->finished == 0) {
            racerPtr->segment += 2;
            racerPtr->nextSegment += 2;
            racerPtr->posX = (g_pChocoboCourseWaypoints[racerPtr->segment].x * racerPtr->laneBlend + g_pChocoboCourseWaypoints[racerPtr->segment].auxX * restBlend) / 256;
            racerPtr->posY = (g_pChocoboCourseWaypoints[racerPtr->segment].y * racerPtr->laneBlend + g_pChocoboCourseWaypoints[racerPtr->segment].auxY * restBlend) / 256;
            racerPtr->posZ = (g_pChocoboCourseWaypoints[racerPtr->segment].z * racerPtr->laneBlend + g_pChocoboCourseWaypoints[racerPtr->segment].auxZ * restBlend) / 256;
            racerPtr->targetX = (g_pChocoboCourseWaypoints[racerPtr->nextSegment].x * racerPtr->laneBlend + g_pChocoboCourseWaypoints[racerPtr->nextSegment].auxX * restBlend) / 256;
            racerPtr->targetY = (g_pChocoboCourseWaypoints[racerPtr->nextSegment].y * racerPtr->laneBlend + g_pChocoboCourseWaypoints[racerPtr->nextSegment].auxY * restBlend) / 256;
            racerPtr->targetZ = (g_pChocoboCourseWaypoints[racerPtr->nextSegment].z * racerPtr->laneBlend + g_pChocoboCourseWaypoints[racerPtr->nextSegment].auxZ * restBlend) / 256;
            racerPtr->speed = racerPtr->baseSpeed;
            racerPtr->currentSpeed = racerPtr->speed;
            racerPtr->stepSpeed = racerPtr->currentSpeed;
            racerPtr->stuckFrames = 0;
            toTarget.vx = racerPtr->targetX - racerPtr->posX;
            toTarget.vy = 0;
            toTarget.vz = racerPtr->targetZ - racerPtr->posZ;
            NormalizeVectorFixed(&toTarget, &toTarget);
            racerPtr->targetHeading = Mini_ChocoboHeadingFromXZ((short)toTarget.vx, (short)toTarget.vz);
            racerPtr->heading = racerPtr->targetHeading;
        }
    }
    if (racerPtr->segment > g_ChocoboCourseSegmentCount - 0xA && racerPtr->finished == 0) {
        racerPtr->finished = 0xFFFF;
        racerPtr->targetX = racerPtr->startPosX;
        racerPtr->targetY = racerPtr->startPosY;
        racerPtr->targetZ = racerPtr->startPosZ;
        place = 1;
        for (loopIndex = 0; loopIndex < 6; loopIndex++) {
            if (g_ChocoboRacers[loopIndex].place >= place) {
                place = g_ChocoboRacers[loopIndex].place + 1;
            }
        }
        racerPtr->place = place;
        if (place == 1) {
            g_ChocoboRaceWinnerIndex = racerIndex;
            if (g_ChocoboPlayerRides == 0) {
                g_pChocoboSoundCommand->soundCommand = 0x10;
                g_pChocoboSoundCommand->soundArg = 0x3B;
                SoundDispatchCommand((short)g_pChocoboSoundCommand->soundCommand, g_pChocoboSoundCommand->soundArg, 0, 0, 0, 0, 0, 0, 0);
            }
        }
        if (place == 2) {
            g_ChocoboSkipPromptActive = 0xFFFFFFFF;
        }
        if (g_ChocoboPlayerRides != 0 && racerIndex == 0) {
            if (g_ChocoboRacers[0].place == 1) {
                g_pChocoboSoundCommand->soundArg = 0x3B;
            } else {
                g_pChocoboSoundCommand->soundArg = 0x3C;
            }
            g_pChocoboSoundCommand->soundCommand = 0x10;
            SoundDispatchCommand((short)g_pChocoboSoundCommand->soundCommand, g_pChocoboSoundCommand->soundArg, 0, 0, 0, 0, 0, 0, 0);
        }
    }
    if (racerPtr->heading - racerPtr->targetHeading > 0xBB8) {
        racerPtr->heading -= 0x1000 /* 360 deg */;
    }
    if (racerPtr->heading - racerPtr->targetHeading < -3000) {
        racerPtr->heading += 0x1000 /* 360 deg */;
    }
    racerPtr->heading = (racerPtr->heading * 3 + racerPtr->targetHeading) / 4;
    if (distance != 0) {
        racerPtr->subPosX += (SinFixed(racerPtr->heading) * distance) / 64;
        racerPtr->subPosZ += (-CosFixed(racerPtr->heading) * distance) / 64;
        racerPtr->posX += (short)((racerPtr->subPosX) / 4096);
        racerPtr->posZ += (short)((racerPtr->subPosZ) / 4096);
        racerPtr->subPosX &= 0x80000FFF;
        racerPtr->subPosZ &= 0x80000FFF;
    }
    edgeDx.vx = g_pChocoboCourseWaypoints[racerPtr->nextSegment].auxX - g_pChocoboCourseWaypoints[racerPtr->nextSegment].x;
    edgeDx.vy = g_pChocoboCourseWaypoints[racerPtr->nextSegment].auxY - g_pChocoboCourseWaypoints[racerPtr->nextSegment].y;
    edgeDx.vz = g_pChocoboCourseWaypoints[racerPtr->nextSegment].auxZ - g_pChocoboCourseWaypoints[racerPtr->nextSegment].z;
    travelX.vx = g_pChocoboCourseWaypoints[racerPtr->nextSegment].x - g_pChocoboCourseWaypoints[racerPtr->segment].x;
    travelX.vy = g_pChocoboCourseWaypoints[racerPtr->nextSegment].y - g_pChocoboCourseWaypoints[racerPtr->segment].y;
    travelX.vz = g_pChocoboCourseWaypoints[racerPtr->nextSegment].z - g_pChocoboCourseWaypoints[racerPtr->segment].z;
    CrossProductFixed12(&travelX, &edgeDx, &planeNormalX);
    if (planeNormalX.vy != 0) {
        nodeSteerDelta = ((g_pChocoboCourseWaypoints[racerPtr->segment].x - racerPtr->posX) * planeNormalX.vx + (g_pChocoboCourseWaypoints[racerPtr->segment].z - racerPtr->posZ) * planeNormalX.vz + planeNormalX.vy * g_pChocoboCourseWaypoints[racerPtr->segment].y) / planeNormalX.vy;
    } else {
        nodeSteerDelta = racerPtr->posY;
    }
    racerPtr->posY = (racerPtr->prevPosY + nodeSteerDelta) / 2;
    racerPtr->prevPosY = racerPtr->posY;
}
/*
 * Copies racer into working state if not already selected and interpolates course position and heading.
 */
// FUNCTION: FF7 0x0077554d
void __cdecl Mini_ChocoboRaceSelectRacerStateIfChanged(int racerIndex)
{
    ChocoboRacer *racerPtr;
    VECTOR dirVector;
    ChocoboRacer *dst;
    int weightB;

    if (g_ChocoboSelectedRacerIndex == racerIndex) {
        return;
    }
    g_ChocoboSelectedRacerIndex = racerIndex;
    dst = &g_ChocoboSelectedRacerState;
    racerPtr = &g_ChocoboRacers[racerIndex];
    *dst = *racerPtr;
    dst->laneBlend = 0x7F;
    weightB = 0x100 - dst->laneBlend;
    dst->posX =
        (g_pChocoboCourseWaypoints[dst->segment].x *
             dst->laneBlend +
         g_pChocoboCourseWaypoints[dst->segment].auxX * weightB) /
        256;
    dst->posY =
        (g_pChocoboCourseWaypoints[dst->segment].y *
             dst->laneBlend +
         g_pChocoboCourseWaypoints[dst->segment].auxY * weightB) /
        256;
    dst->posZ =
        (g_pChocoboCourseWaypoints[dst->segment].z *
             dst->laneBlend +
         g_pChocoboCourseWaypoints[dst->segment].auxZ * weightB) /
        256;
    dst->targetX =
        (g_pChocoboCourseWaypoints[dst->nextSegment].x *
             dst->laneBlend +
         g_pChocoboCourseWaypoints[dst->nextSegment].auxX * weightB) /
        256;
    dst->targetY =
        (g_pChocoboCourseWaypoints[dst->nextSegment].y *
             dst->laneBlend +
         g_pChocoboCourseWaypoints[dst->nextSegment].auxY * weightB) /
        256;
    dst->targetZ =
        (g_pChocoboCourseWaypoints[dst->nextSegment].z *
             dst->laneBlend +
         g_pChocoboCourseWaypoints[dst->nextSegment].auxZ * weightB) /
        256;
    dirVector.vx = dst->targetX - dst->posX;
    dirVector.vy = 0;
    dirVector.vz = dst->targetZ - dst->posZ;
    NormalizeVectorFixed(&dirVector, &dirVector);
    dst->targetHeading = Mini_ChocoboHeadingFromXZ(dirVector.vx, dirVector.vz);
}
/*
 * Copies racer into working state at a specific route point and interpolates heading and position.
 */
// FUNCTION: FF7 0x007757ce
void __cdecl Mini_ChocoboRaceSelectRacerStateAtRoutePoint(int racerIndex, int progress, int segment)
{
    VECTOR dirVector;
    int nextSegment;
    ChocoboRacer *dst;
    int weightB;

    g_ChocoboSelectedRacerIndex = racerIndex;
    dst = &g_ChocoboSelectedRacerState;
    *dst = g_ChocoboRacers[racerIndex];
    nextSegment = (segment + g_ChocoboCourseSegmentCount + 1) % g_ChocoboCourseSegmentCount;
    dst->nextSegment = nextSegment;
    dst->segment = segment;
    dst->laneBlend = 0x7F;
    weightB = 0x100 - dst->laneBlend;
    dst->stepSpeed = progress;
    dst->posX =
        (g_pChocoboCourseWaypoints[segment].x *
             dst->laneBlend +
         g_pChocoboCourseWaypoints[segment].auxX * weightB) /
        256;
    dst->posY =
        (g_pChocoboCourseWaypoints[segment].y *
             dst->laneBlend +
         g_pChocoboCourseWaypoints[segment].auxY * weightB) /
        256;
    dst->posZ =
        (g_pChocoboCourseWaypoints[segment].z *
             dst->laneBlend +
         g_pChocoboCourseWaypoints[segment].auxZ * weightB) /
        256;
    dst->targetX =
        (g_pChocoboCourseWaypoints[nextSegment].x *
             dst->laneBlend +
         g_pChocoboCourseWaypoints[nextSegment].auxX * weightB) /
        256;
    dst->targetY =
        (g_pChocoboCourseWaypoints[nextSegment].y *
             dst->laneBlend +
         g_pChocoboCourseWaypoints[nextSegment].auxY * weightB) /
        256;
    dst->targetZ =
        (g_pChocoboCourseWaypoints[nextSegment].z *
             dst->laneBlend +
         g_pChocoboCourseWaypoints[nextSegment].auxZ * weightB) /
        256;
    dirVector.vx = dst->targetX - dst->posX;
    dirVector.vy = 0;
    dirVector.vz = dst->targetZ - dst->posZ;
    NormalizeVectorFixed(&dirVector, &dirVector);
    dst->targetHeading = Mini_ChocoboHeadingFromXZ(dirVector.vx, dirVector.vz);
}
/*
 * Updates current chocobo path-following record along route sections and smooths heading and height.
 */
// FUNCTION: FF7 0x00775a45
void __cdecl Mini_ChocoboRaceAdvanceSelectedRacerAlongRoute(int updateSpeed)
{
    ChocoboRacer *racerState;
    VECTOR dxValue;
    int searchEnd;
    VECTOR curNormalXValue;
    int nextX;
    int yGoal;
    int zDest;
    VECTOR vectorX;
    int nodeIndex;
    int unusedWord[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int i;
    int nodeNumber;
    int complWeight;
    VECTOR stepDir;
    ChocoboCourseWaypoint *pRoute;
    int eventNumber;
    int routeDelta;
    ChocoboWatPoint *posPtr;

    racerState = &g_ChocoboSelectedRacerState;
    pRoute = &g_pChocoboCourseWaypoints[racerState->nextSegment];
    posPtr = (ChocoboWatPoint *)&g_pChocoboCourseWaypoints[racerState->nextSegment].auxX;
    vectorX.vx = posPtr->x - pRoute->x;
    vectorX.vy = 0;
    vectorX.vz = posPtr->z - pRoute->z;
    dxValue.vx = racerState->posX - pRoute->x;
    dxValue.vy = 0;
    dxValue.vz = racerState->posZ - pRoute->z;
    CrossProductFixed12(&vectorX, &dxValue, &vectorX);
    if (vectorX.vy >= 0) {
        racerState->stuckFrames = 0;
        searchEnd = racerState->segment + 0x3C;
        for (nodeIndex = racerState->segment; nodeIndex < searchEnd; nodeIndex++) {
            routeDelta = (nodeIndex + g_ChocoboCourseSegmentCount) % g_ChocoboCourseSegmentCount;
            pRoute = &g_pChocoboCourseWaypoints[routeDelta];
            posPtr = (ChocoboWatPoint *)&g_pChocoboCourseWaypoints[routeDelta].auxX;
            vectorX.vx = posPtr->x - pRoute->x;
            vectorX.vy = 0;
            vectorX.vz = posPtr->z - pRoute->z;
            dxValue.vx = racerState->posX - pRoute->x;
            dxValue.vy = 0;
            dxValue.vz = racerState->posZ - pRoute->z;
            CrossProductFixed12(&vectorX, &dxValue, &vectorX);
            if (vectorX.vy < 0) {
                break;
            }
        }
        racerState->segment = racerState->nextSegment;
        racerState->nextSegment = routeDelta + g_ChocoboCourseSegmentCount;
        racerState->nextSegment = racerState->nextSegment % g_ChocoboCourseSegmentCount;
        complWeight = 0x100 - racerState->laneBlend;
        nodeNumber = racerState->nextSegment;
        racerState->targetX = ((g_pChocoboCourseWaypoints[nodeNumber].x * racerState->laneBlend + g_pChocoboCourseWaypoints[nodeNumber].auxX * complWeight) / 256);
        racerState->targetY = ((g_pChocoboCourseWaypoints[nodeNumber].y * racerState->laneBlend + g_pChocoboCourseWaypoints[nodeNumber].auxY * complWeight) / 256);
        racerState->targetZ = ((g_pChocoboCourseWaypoints[nodeNumber].z * racerState->laneBlend + g_pChocoboCourseWaypoints[nodeNumber].auxZ * complWeight) / 256);
        nodeNumber = (racerState->nextSegment + 1) % g_ChocoboCourseSegmentCount;
        nextX = ((g_pChocoboCourseWaypoints[nodeNumber].x * racerState->laneBlend + g_pChocoboCourseWaypoints[nodeNumber].auxX * complWeight) / 256);
        yGoal = ((g_pChocoboCourseWaypoints[nodeNumber].y * racerState->laneBlend + g_pChocoboCourseWaypoints[nodeNumber].auxY * complWeight) / 256);
        zDest = ((g_pChocoboCourseWaypoints[nodeNumber].z * racerState->laneBlend + g_pChocoboCourseWaypoints[nodeNumber].auxZ * complWeight) / 256);
        stepDir.vx = nextX - racerState->posX;
        stepDir.vy = 0;
        stepDir.vz = zDest - racerState->posZ;
        NormalizeVectorFixed(&stepDir, &stepDir);
        racerState->targetHeading = Mini_ChocoboHeadingFromXZ((short)stepDir.vx, (short)stepDir.vz);
        eventNumber = g_pChocoboCourseWaypoints[racerState->segment].eventId;
        if (eventNumber != 0) {
            Mini_ChocoboRaceSetSceneryAnimationStep((short)eventNumber);
        }
        if (g_ChocoboPlayerRides != 0) {
            eventNumber = g_pChocoboCourseWaypoints[racerState->segment].eventNumber;
            if (eventNumber != 0) {
                eventNumber--;
                g_pChocoboSoundCommand->soundCommand = *(short *)((char *)g_ChocoboTrackData->courseSounds + (eventNumber << 4));
                for (i = 0; i < 6; i++) {
                    *(int *)((char *)g_pChocoboSoundCommand + i * 4 + 4) = *(unsigned short *)((char *)(g_ChocoboTrackData->courseSounds + (eventNumber << 4)) + i * 2 + 2);
                }
                SoundDispatchCommand((short)g_pChocoboSoundCommand->soundCommand, g_pChocoboSoundCommand->soundArg, g_pChocoboSoundCommand->soundParam1, g_pChocoboSoundCommand->soundParam2, g_pChocoboSoundCommand->soundParam3, g_pChocoboSoundCommand->soundParam4, g_pChocoboSoundCommand->soundParam5, 0, 0);
            }
        }
    }
    if (racerState->heading - racerState->targetHeading > 0xBB8) {
        racerState->heading -= 0x1000 /* 360 deg */;
    }
    if (racerState->heading - racerState->targetHeading < -0xBB8) {
        racerState->heading += 0x1000 /* 360 deg */;
    }
    racerState->heading = ((racerState->heading * 7 + racerState->targetHeading) / 8);
    racerState->subPosX += ((SinFixed(racerState->heading) * racerState->stepSpeed) / 64);
    racerState->subPosZ += ((-CosFixed(racerState->heading) * racerState->stepSpeed) / 64);
    racerState->posX += (short)(((racerState->subPosX) / 4096));
    racerState->posZ += (short)(((racerState->subPosZ) / 4096));
    racerState->subPosX &= 0x80000FFF;
    racerState->subPosZ &= 0x80000FFF;
    vectorX.vx = g_pChocoboCourseWaypoints[racerState->nextSegment].auxX - g_pChocoboCourseWaypoints[racerState->nextSegment].x;
    vectorX.vy = g_pChocoboCourseWaypoints[racerState->nextSegment].auxY - g_pChocoboCourseWaypoints[racerState->nextSegment].y;
    vectorX.vz = g_pChocoboCourseWaypoints[racerState->nextSegment].auxZ - g_pChocoboCourseWaypoints[racerState->nextSegment].z;
    dxValue.vx = g_pChocoboCourseWaypoints[racerState->nextSegment].x - g_pChocoboCourseWaypoints[racerState->segment].x;
    dxValue.vy = g_pChocoboCourseWaypoints[racerState->nextSegment].y - g_pChocoboCourseWaypoints[racerState->segment].y;
    dxValue.vz = g_pChocoboCourseWaypoints[racerState->nextSegment].z - g_pChocoboCourseWaypoints[racerState->segment].z;
    CrossProductFixed12(&dxValue, &vectorX, &curNormalXValue);
    if (curNormalXValue.vy != 0) {
        nodeIndex = ((g_pChocoboCourseWaypoints[g_ChocoboRacers[g_ChocoboViewedRacer].segment].x - racerState->posX) * curNormalXValue.vx + (g_pChocoboCourseWaypoints[g_ChocoboRacers[g_ChocoboViewedRacer].segment].z - racerState->posZ) * curNormalXValue.vz + curNormalXValue.vy * g_pChocoboCourseWaypoints[g_ChocoboRacers[g_ChocoboViewedRacer].segment].y) / curNormalXValue.vy;
        g_ChocoboCameraSmoothedNode = ((g_ChocoboCameraSmoothedNode * 7 + nodeIndex) / 8);
        nodeIndex = ((g_pChocoboCourseWaypoints[racerState->segment].x - racerState->posX) * curNormalXValue.vx + (g_pChocoboCourseWaypoints[racerState->segment].z - racerState->posZ) * curNormalXValue.vz + curNormalXValue.vy * g_pChocoboCourseWaypoints[racerState->segment].y) / curNormalXValue.vy;
    } else {
        nodeIndex = racerState->posY;
    }
    racerState->posY = ((racerState->prevPosY + nodeIndex) / 2);
    racerState->prevPosY = racerState->posY;
    if (updateSpeed != 0 && g_ChocoboRaceAdvanceEnabled != 0) {
        routeDelta = g_ChocoboRacers[g_ChocoboViewedRacer].segment - racerState->segment;
        racerState->stepSpeed = g_ChocoboRacers[g_ChocoboViewedRacer].stepSpeed;
        if (g_ChocoboRacers[g_ChocoboViewedRacer].finished == 0) {
            if (routeDelta < 1) {
                racerState->stepSpeed = 0;
            } else if (routeDelta > 2) {
                racerState->stepSpeed += (short)(routeDelta * 150);
            }
        }
    }
}
} /* extern "C" */
