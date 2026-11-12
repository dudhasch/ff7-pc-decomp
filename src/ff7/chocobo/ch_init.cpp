// src/ff7/chocobo/ch_init.cpp
// Original: C:\FF7\chocobo\ch_init.cpp
// Contents: Chocobo race setup: seeds the race globals from the savemap, sets the asset directory,

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <stdlib.h>
#include <string.h>
#include "ff7/api/chocobo.h"
#include "ff7/types/chocobo.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "lib/api/sound.h"
#include "lib/api/token.h"
#include "ff7/enums/chocobo.h"
#include "lib/codegen.h"
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include "lib/types/mem.h"
#include "lib/types/polygon.h"
#include "ff7/savemap.h"
#include "ff7/enums/main.h"
#include <stdio.h>
#include "ff7/api/field.h"
#include "lib/enums/file.h"
extern "C" {

#pragma intrinsic(strcpy, memcpy)

struct GfxFloatColorAddend;
struct PolygonModelVertexColors;


// clang-format off
// The race's colour and camera parameter table: six BGRA racer colours, the special colours from racerColors[6]
// (indexed by gSavemap.raceChocoboColor) and the two course sprite records copied from course0* and course2*.
// GLOBAL: FF7 0x0096e0f0
ChocoboRacerStats g_ChocoboColorHudInitTable = {
    {
        {0x00, 0x50, 0x00, {0x00}}, {0x00, 0x00, 0x50, {0x00}},
        {0x50, 0x14, 0x28, {0x00}}, {0x50, 0xd8, 0xd8, {0x00}},
        {0x28, 0xd8, 0x50, {0x00}}, {0x50, 0x50, 0x50, {0x00}},
        {0x50, 0x50, 0x00, {0x00}}, {0x00, 0x50, 0x00, {0x00}},
        {0x00, 0x50, 0x50, {0x00}}, {0xb0, 0xb0, 0xb0, {0x00}},
        {0x63, 0x2f, 0x8b, {0x00}}, {0x00, 0x00, 0x00, {0x00}},
        {0x50, 0x50, 0x00, {0x00}}, {0x00, 0x50, 0x00, {0x00}},
        {0x00, 0x50, 0x50, {0x00}}, {0xb0, 0xb0, 0xb0, {0x00}},
        {0x33, 0x09, 0xe5, {0x00}}, {0x00, 0x00, 0x00, {0x00}},
        {0x00, 0x00, 0x00, {0x00}}
    },
    0x80, 0x80, 0x80, {0x00},
    0xfc, 0x18, 0x00, 0x00, {0x00, 0x00},
    0x013c, 0x18, {0x40, 0x00, 0x00, 0x00},
    0xfc, 0x38, {0x00, 0x20, 0x00, 0x00},
    0x013c, 0x38, {0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    0x80, 0x80, 0x80, {0x00},
    0x14, 0x78, {0xec, 0x9f, 0x00, 0x00}, 0x24, 0x78,
    {0xfb, 0x9f, 0x00, 0x00}, 0x14, 0xd8,
    {0xec, 0xff, 0x00, 0x00}, 0x24, 0xd8, {0xfb, 0xff, 0x00, 0x00}};
// The six racers' BGRA vertex colour bias, set by Mini_ChocoboRaceInitRacerModels.
// GLOBAL: FF7 0x00e3b628
BgraValue g_ChocoboRaceCourseConfig[6] = {0};
// GLOBAL: FF7 0x00e3b640
char g_ChocoboAssetDirPath[256] = {0};
// GLOBAL: FF7 0x00e3b740
int g_ChocoboRacePhase = 0;
// GLOBAL: FF7 0x00e3b744
int g_ChocoboHudPlayerRides = 0;
// GLOBAL: FF7 0x00e3b748
int g_ChocoboHudRaceClass = 0;
// GLOBAL: FF7 0x00e3b74c
int g_ChocoboCourseVariant = 0;
// The race record's quadCount, copied at race init.
// GLOBAL: FF7 0x00e3b750
int g_ChocoboCourseQuadCount = 0;
// GLOBAL: FF7 0x00e3b754
int g_ChocoboRaceHeapBlock = 0;
// GLOBAL: FF7 0x00e3b758
struct ChocoboSpriteRecord g_ChocoboRacerTable[6] = {0};
// The race camera's sprite record, in front of g_ChocoboCourseRecord0; it has the same layout as the records of
// g_ChocoboRacerTable, g_ChocoboCourseRecord0 and g_ChocoboStaminaGaugeRecord.
// GLOBAL: FF7 0x00e3b908
struct ChocoboSpriteRecord g_ChocoboCameraRecord = {0};
// GLOBAL: FF7 0x00e3b950
struct ChocoboSpriteRecord g_ChocoboCourseRecord0 = {0};
// GLOBAL: FF7 0x00e3b998
struct ChocoboSpriteRecord g_ChocoboCourseRecord1 = {0};
// GLOBAL: FF7 0x00e3b9e0
struct ChocoboSpriteRecord g_ChocoboStaminaGaugeRecord = {0};
// GLOBAL: FF7 0x00e3ba28
char *g_pChocoboWatBuffer = 0;
unsigned char g_bssPad_00e3ba2c[4] = {0};
// GLOBAL: FF7 0x00e3ba30
Timestamp64 g_ChocoboTimestampNow = {0};
// GLOBAL: FF7 0x00e3ba38
Timestamp64 g_ChocoboFrameStartTimestamp = {0};
// clang-format on


void __cdecl World_SubmitMeshTriangles(void);

// The literals up to the three table builders' are parse-time units: retail emits Mini_ChocoboLoadFile's three
// literals and the builders' __FILE__ copies before Mini_ChocoboLoadAllAssets's literals, while the builders' code
// follows it, and VC5 emits a function's literals in function order. Each helper returns one literal and is defined
// before the first function; under /Ob1 its call is the plain push.
// Mini_ChocoboLoadFile's fopen mode.
// STRING: FF7 0x0096e188
static __inline char *Chocobo_LoadFileMode(void) { return "rb"; }
// Mini_ChocoboLoadFile's __FILE__, the disk path.
// STRING: FF7 0x0096e18c
static __inline char *Chocobo_LoadFileName1(void) { return "C:\\FF7\\chocobo\\ch_init.cpp"; }
// Mini_ChocoboLoadFile's __FILE__, the LGP path.
// STRING: FF7 0x0096e1a8
static __inline char *Chocobo_LoadFileName2(void) { return "C:\\FF7\\chocobo\\ch_init.cpp"; }
// Mini_ChocoboBuildFloatVertexTable's __FILE__.
// STRING: FF7 0x0096e1c4
static __inline char *Chocobo_VertexTableFileName(void) { return "C:\\FF7\\chocobo\\ch_init.cpp"; }
// Mini_ChocoboRaceBuildCourseTriangleVertices's __FILE__.
// STRING: FF7 0x0096e1e0
static __inline char *Chocobo_Table4cFileName(void) { return "C:\\FF7\\chocobo\\ch_init.cpp"; }
// Mini_ChocoboRaceBuildCourseQuadVertices's __FILE__.
// STRING: FF7 0x0096e1fc
static __inline char *Chocobo_Table50FileName(void) { return "C:\\FF7\\chocobo\\ch_init.cpp"; }

/*
 * Sets the directory the chocobo race loads its loose files from. `App_InitSubsystems` passes
 * `GetMinigameDirForChocobo()` at start-up; the string is copied with strcpy into the 256-byte
 * buffer g_ChocoboAssetDirPath, which the `.hrc`, `.anm` and texture loaders later prefix to
 * each file name. The copy has no length check: a source longer than 255 characters overruns
 * the buffer.
 */
// FUNCTION: FF7 0x0076b850
void __cdecl Mini_ChocoboRaceSetAssetDirPath(char *src)
{
    strcpy(g_ChocoboAssetDirPath, src);
}

/*
 * Seeds chocobo-race globals from saved flags and race class: derives the
 * enabled flag, race variant and route count 0x49D or 0x3B1; sets the story-
 * threshold flag from the combined saved bytes compared with 1000. Calls
 * Mini_ChocoboPollInputLatches and Mini_ChocoboPackInputLatches, then
 * interprets bits from g_ChocoboPackedInputLatches to update gSavemap.playerRacerRow, with bit 2
 * overriding bit 8.
 */
// FUNCTION: FF7 0x0076b87e
void __cdecl Mini_ChocoboRaceInitFromSavemap(void)
{
    /* The engine context is fetched and dropped -- nothing here reads it.
       It is declared because the original reserves the slot. */
    void *ctx;
    unsigned int bits;

    ctx = GetEngineContext();
    if (gSavemap.chocoboRaceNoBetting != 0) {
        gSavemap.chocoboPlayerRides = 0xFF;
    }
    if (gSavemap.chocoboPlayerRides != 0) {
        g_ChocoboPlayerRides = -1;
    } else {
        g_ChocoboPlayerRides = 0;
    }
    g_ChocoboHudPlayerRides = g_ChocoboPlayerRides;
    if (gSavemap.chocoboCourse != 0) {
        g_ChocoboCourseVariant = 1;
    } else {
        g_ChocoboCourseVariant = 0;
    }
    g_ChocoboHudRaceClass = gSavemap.raceClass;
    g_ChocoboRacePhase = 0;
    g_ChocoboRaceClass = gSavemap.raceClass;
    if (g_ChocoboCourseVariant == 0) {
        g_ChocoboCourseSegmentCount = 1181;
    } else {
        g_ChocoboCourseSegmentCount = 945;
    }
    g_ChocoboSkipPromptActive = 0;
    g_ChocoboFadeLevel = 0xFF;
    g_ChocoboFadeStep = -16;
    g_ChocoboLateGamePrizesEnabled = gSavemap.storyProgressLo + (gSavemap.storyProgressHi << 8) >= 1000 ? -1 : 0;
    g_ChocoboCameraSnapFlag = 1;
    Mini_ChocoboPollInputLatches();
    Mini_ChocoboPackInputLatches();
    bits = g_ChocoboPackedInputLatches >> 16;
    if ((bits & 0xF0) == 0xF0) {
        if ((bits & 8) != 0) {
            gSavemap.playerRacerRow = 1;
        }
        if ((bits & 2) != 0) {
            gSavemap.playerRacerRow = 2;
        }
    }
    g_ChocoboRacerStepDivisor = g_ChocoboRaceClass * 2 + 6;
    g_ChocoboFrameCounter = 0;
    g_ChocoboCameraClearedWord = 0;
    g_ChocoboViewedRacer = 0;
    g_ChocoboRaceResetSentinel = -1;
    g_ChocoboRouteEventRacer = -1;
    g_ChocoboStartTimer = 0;
    g_ChocoboRaceTimer = 0;
    g_ChocoboRaceAdvanceEnabled = 0;
    g_ChocoboCameraInitIndex = 1;
    g_ChocoboSelectedRacerIndex = -1;
    g_ChocoboDashActiveFlag = 0;
    g_ChocoboRaceWinnerIndex = -1;
    g_ChocoboRacers[0].countdown = 30;
    g_ChocoboViewedRacer = 0;
}

/*
 * Sends sound command 0x23 with argument 0x40 and four zero parameters through
 * the command block at g_pChocoboSoundCommand.
 */
// FUNCTION: FF7 0x0076ba66
void __cdecl Mini_ChocoboRaceSendSoundCommand23(void)
{
    g_pChocoboSoundCommand->soundCommand = 0x23;
    g_pChocoboSoundCommand->soundArg = 0x40;
    g_pChocoboSoundCommand->soundParam4 = 0;
    g_pChocoboSoundCommand->soundParam3 = 0;
    g_pChocoboSoundCommand->soundParam2 = 0;
    g_pChocoboSoundCommand->soundParam1 = 0;
    SoundDispatchCommand(g_pChocoboSoundCommand->soundCommand, g_pChocoboSoundCommand->soundArg, g_pChocoboSoundCommand->soundParam1, g_pChocoboSoundCommand->soundParam2, g_pChocoboSoundCommand->soundParam3, g_pChocoboSoundCommand->soundParam4, 0, 0, 0);
}

/*
 * Initializes chocobo race rendering and assets through the supplied
 * engine/options pointers, sets geometry origin 160,120 and screen distance
 * 270, clears three rotation shorts and loads race assets selected by saved
 * settings. Copies the first route event and the quad count from the track
 * buffers, then unconditionally overwrites the copied event kind with 0xFF,
 * making the earlier conditional overwrite redundant.
 */
// FUNCTION: FF7 0x0076bafd
void __cdecl Mini_ChocoboRaceInitRenderAndAssets(GfxEngineContext *engineCtx, void *loadOptions)
{
    /* The twenty bytes of g_ChocoboCurrentRouteEvent come out of the route
       event table as one `rep movsd`, which is a structure assignment and needs a type. */
    int rect[4];
    int i;

    rect[0] = 0;
    rect[1] = 0;
    rect[2] = 0x140;
    rect[3] = 0x1E0;
    Mini_ChocoboCreateHudRenderObjects(engineCtx, (struct PfileLoadOptions *)loadOptions, g_ChocoboHudPlayerRides, g_ChocoboHudRaceClass);
    Mini_ChocoboBuildRaceObjects((struct PfileLoadOptions *)loadOptions, engineCtx, g_ChocoboCourseVariant);
    Pfile_InitLoadOptions((struct PfileLoadOptions *)loadOptions);
    *(int *)loadOptions |= 0x2000000;
    Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (struct PfileLoadOptions *)loadOptions);
    Mini_ChocoboCreateFixedRenderObjects(engineCtx, loadOptions);
    Mini_ChocoboRaceCreateCourseSpriteObjects((struct PfileLoadOptions *)loadOptions, engineCtx, g_ChocoboCourseVariant);
    SetGeomOffset(0xA0, 0x78);
    SetGeomScreen(270);
    g_ChocoboFlags.clearedShort0 = 0;
    g_ChocoboFlags.clearedShort1 = 0;
    g_ChocoboFlags.clearedShort2 = 0;
    Mini_ChocoboRaceInitHookNoOp();
    Mini_ChocoboLoadAllAssets(gSavemap.chocoboCourse, gSavemap.raceClass);
    Mini_ChocoboLoadCourseSounds();
    (*(ChocoboRaceRecordCopy *)&g_ChocoboCurrentRouteEvent) = *(ChocoboRaceRecordCopy *)g_ChocoboTrackData->routeEvents;
    if (g_ChocoboPlayerRides != 0) {
        g_ChocoboCurrentRouteEvent.eventKind = 0xFF;
    }
    g_ChocoboCourseQuadCount = g_ChocoboTrackData->segmentCount;
    g_ChocoboCurrentRouteEvent.eventKind = 0xFF;
    for (i = 0; i < 6; i++) {
        g_ChocoboRacerTable[i].renderObjectIndex = 25;
        g_ChocoboRacerTable[i].colorR = (i & 2) != 0 ? 0xFF : 0;
        g_ChocoboRacerTable[i].colorG = (i & 4) != 0 ? 0xFF : 0;
        g_ChocoboRacerTable[i].colorB = (i & 1) != 0 ? 0xFF : 0;
        g_ChocoboRacerTable[i].texY = 0;
        g_ChocoboRacerTable[i].texX = 0;
        g_ChocoboRacerTable[i].depth = FLOAT_0_222_BITS;
    }
    g_ChocoboCameraRecord.vertexCY = 0;
    g_ChocoboCameraRecord.vertexAY = g_ChocoboCameraRecord.vertexCY;
    g_ChocoboCameraRecord.vertexBX = g_ChocoboCameraRecord.vertexAY;
    g_ChocoboCameraRecord.left = g_ChocoboCameraRecord.vertexBX;
    g_ChocoboCameraRecord.vertexDX = 320;
    g_ChocoboCameraRecord.right = g_ChocoboCameraRecord.vertexDX;
    g_ChocoboCameraRecord.vertexDY = 232;
    g_ChocoboCameraRecord.vertexBY = g_ChocoboCameraRecord.vertexDY;
    g_ChocoboCameraRecord.width = 320;
    g_ChocoboCameraRecord.height = 232;
    g_ChocoboCameraRecord.depth = FLOAT_0_001_BITS;
    g_ChocoboCameraRecord.texY = 0;
    g_ChocoboCameraRecord.texX = g_ChocoboCameraRecord.texY;
    g_ChocoboCameraRecord.isActive = 1;
    g_ChocoboCameraRecord.renderObjectIndex = 25;
    g_ChocoboCourseRecord0.colorR = g_ChocoboColorHudInitTable.course0ColorR;
    g_ChocoboCourseRecord0.colorG = g_ChocoboColorHudInitTable.course0ColorG;
    g_ChocoboCourseRecord0.colorB = g_ChocoboColorHudInitTable.course0ColorB;
    g_ChocoboCourseRecord0.left = g_ChocoboColorHudInitTable.course0Left;
    g_ChocoboCourseRecord0.vertexAY = g_ChocoboColorHudInitTable.course0VertexAY;
    g_ChocoboCourseRecord0.right = g_ChocoboColorHudInitTable.course0Right;
    g_ChocoboCourseRecord0.vertexCY = g_ChocoboColorHudInitTable.course0VertexCY;
    g_ChocoboCourseRecord0.vertexBX = g_ChocoboColorHudInitTable.course0VertexBX;
    g_ChocoboCourseRecord0.vertexBY = g_ChocoboColorHudInitTable.course0VertexBY;
    g_ChocoboCourseRecord0.vertexDX = g_ChocoboColorHudInitTable.course0VertexDX;
    g_ChocoboCourseRecord0.vertexDY = g_ChocoboColorHudInitTable.course0VertexDY;
    g_ChocoboCourseRecord0.texX = g_ChocoboColorHudInitTable.course0TexX;
    g_ChocoboCourseRecord0.texY = g_ChocoboColorHudInitTable.course0TexY;
    g_ChocoboCourseRecord0.width = 64;
    g_ChocoboCourseRecord0.height = 32;
    g_ChocoboCourseRecord0.texUSize = CHOCOBO_FLOAT_0_98_BITS;
    g_ChocoboCourseRecord0.texVSize = CHOCOBO_FLOAT_0_97_BITS;
    g_ChocoboCourseRecord0.depth = CHOCOBO_FLOAT_0_444_BITS;
    g_ChocoboCourseRecord0.renderObjectIndex = 19;
    g_ChocoboCourseRecord0.palette = 0;
    g_ChocoboCourseRecord0.isActive = 1;
    g_ChocoboStaminaGaugeRecord.colorR = g_ChocoboColorHudInitTable.course2ColorR;
    g_ChocoboStaminaGaugeRecord.colorG = g_ChocoboColorHudInitTable.course2ColorG;
    g_ChocoboStaminaGaugeRecord.colorB = g_ChocoboColorHudInitTable.course2ColorB;
    g_ChocoboStaminaGaugeRecord.left = g_ChocoboColorHudInitTable.course2Left;
    g_ChocoboStaminaGaugeRecord.vertexAY = g_ChocoboColorHudInitTable.course2VertexAY;
    g_ChocoboStaminaGaugeRecord.right = g_ChocoboColorHudInitTable.course2Right;
    g_ChocoboStaminaGaugeRecord.vertexCY = g_ChocoboColorHudInitTable.course2VertexCY;
    g_ChocoboStaminaGaugeRecord.vertexBX = g_ChocoboColorHudInitTable.course2VertexBX;
    g_ChocoboStaminaGaugeRecord.vertexBY = g_ChocoboColorHudInitTable.course2VertexBY;
    g_ChocoboStaminaGaugeRecord.vertexDX = g_ChocoboColorHudInitTable.course2VertexDX;
    g_ChocoboStaminaGaugeRecord.vertexDY = g_ChocoboColorHudInitTable.course2VertexDY;
    g_ChocoboStaminaGaugeRecord.texX = 0;
    g_ChocoboStaminaGaugeRecord.texY = 0;
    g_ChocoboStaminaGaugeRecord.texU = 0;
    g_ChocoboStaminaGaugeRecord.texV = 0;
    g_ChocoboStaminaGaugeRecord.width = 16;
    g_ChocoboStaminaGaugeRecord.height = 96;
    g_ChocoboStaminaGaugeRecord.texelWidth = CHOCOBO_FLOAT_16_BITS;
    g_ChocoboStaminaGaugeRecord.texelHeight = CHOCOBO_FLOAT_96_BITS;
    g_ChocoboStaminaGaugeRecord.texUSize = CHOCOBO_FLOAT_0_94_BITS;
    g_ChocoboStaminaGaugeRecord.texVSize = FLOAT_0_75_BITS;
    g_ChocoboStaminaGaugeRecord.renderObjectIndex = 22;
    g_ChocoboStaminaGaugeRecord.palette = 0;
    g_ChocoboStaminaGaugeRecord.isActive = 0;
    g_ChocoboCourseRecord1.vertexBX = 0x19;
    g_ChocoboCourseRecord1.left = g_ChocoboCourseRecord1.vertexBX;
    g_ChocoboCourseRecord1.vertexDX = 30;
    g_ChocoboCourseRecord1.right = g_ChocoboCourseRecord1.vertexDX;
    g_ChocoboCourseRecord1.vertexDY = 209;
    g_ChocoboCourseRecord1.vertexBY = g_ChocoboCourseRecord1.vertexDY;
    g_ChocoboCourseRecord1.width = 5;
    g_ChocoboCourseRecord1.colorR = 0xFF;
    g_ChocoboCourseRecord1.colorG = 0x14;
    g_ChocoboCourseRecord1.colorB = 0x14;
    g_ChocoboCourseRecord1.renderObjectIndex = 24;
    Mini_ChocoboRaceInitRacerModels(gSavemap.chocoboCourse);
    Mini_ChocoboRaceInitLaneHudStatesFromSavedColor();
}

/*
 * Assigns six racer model indices and sets the scale of selected model records
 * to 0x280. Applies either a special dark tint to racer 1, a selected palette
 * tint to racer 0, or default per-racer colors, converting signed color bytes
 * to float divided by 256 for the skeleton helper.
 */
// FUNCTION: FF7 0x0076c01f
void __cdecl Mini_ChocoboRaceInitRacerModels(int course)
{
    struct {
        unsigned int color;
        struct ChocoboRacerEntry *record;
        unsigned int racerIndex;
        int unused;
        void *context;
        GfxFloatColorAddend addend;
        float alpha;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.context = GetEngineContext();
    for (frame.racerIndex = 0; frame.racerIndex < 6; frame.racerIndex++) {
        g_ChocoboRacers[frame.racerIndex].lane = frame.racerIndex;
        frame.record = &g_pChocoboRacerTable[frame.racerIndex];
        frame.record->scale = 0x280;
        if (gSavemap.racerEnabled && frame.racerIndex == 1) {
            frame.addend.red = -0.390625f;
            frame.addend.green = -0.390625f;
            frame.addend.blue = -0.390625f;
            frame.alpha = 1.0f;
            Gfx_AddColorToModelVertexColors(&frame.addend, (PolygonModelVertexColors *)frame.record->player->skeleton, (int)frame.context);
            g_ChocoboRaceCourseConfig[frame.racerIndex].c.r = 0x40;
            g_ChocoboRaceCourseConfig[frame.racerIndex].c.g = 0x40;
            g_ChocoboRaceCourseConfig[frame.racerIndex].c.b = 0x40;
        } else if (g_ChocoboPlayerRides && frame.racerIndex == 0) {
            frame.color = gSavemap.raceChocoboColor;
            g_ChocoboRaceCourseConfig[frame.racerIndex] = *(BgraValue *)&g_ChocoboColorHudInitTable.racerColors[6 + frame.color];
            frame.addend.red = g_ChocoboColorHudInitTable.racerColors[6 + frame.color].colorR / 256.0f;
            frame.addend.green = g_ChocoboColorHudInitTable.racerColors[6 + frame.color].colorG / 256.0f;
            frame.addend.blue = g_ChocoboColorHudInitTable.racerColors[6 + frame.color].colorB / 256.0f;
            frame.alpha = 1.0f;
            Gfx_AddColorToModelVertexColors(&frame.addend, (PolygonModelVertexColors *)frame.record->player->skeleton, (int)frame.context);
        } else {
            g_ChocoboRaceCourseConfig[frame.racerIndex] = *(BgraValue *)&g_ChocoboColorHudInitTable.racerColors[frame.racerIndex];
            frame.record->colorR = g_ChocoboColorHudInitTable.racerColors[frame.racerIndex].colorR;
            frame.record->colorG = g_ChocoboColorHudInitTable.racerColors[frame.racerIndex].colorG;
            frame.record->colorB = g_ChocoboColorHudInitTable.racerColors[frame.racerIndex].colorB;
            frame.addend.red = g_ChocoboColorHudInitTable.racerColors[frame.racerIndex].colorR / 256.0f;
            frame.addend.green = g_ChocoboColorHudInitTable.racerColors[frame.racerIndex].colorG / 256.0f;
            frame.addend.blue = g_ChocoboColorHudInitTable.racerColors[frame.racerIndex].colorB / 256.0f;
            frame.alpha = 1.0f;
            Gfx_AddColorToModelVertexColors(&frame.addend, (PolygonModelVertexColors *)frame.record->player->skeleton, (int)frame.context);
        }
    }
    frame.unused = 6;
    if (g_ChocoboPlayerRides) {
        g_ChocoboRacers[0].tableIndex = gSavemap.playerRacerRow + 6;
        frame.record = &g_pChocoboRacerTable[(gSavemap.playerRacerRow + 6)];
    } else {
        g_ChocoboRacers[0].tableIndex = 10;
        frame.record = &g_pChocoboRacerTable[10];
    }
    frame.record->scale = 0x280;
    if (gSavemap.racerEnabled) {
        g_ChocoboRacers[1].tableIndex = 9;
        frame.record = &g_pChocoboRacerTable[9];
    } else {
        g_ChocoboRacers[1].tableIndex = 11;
        frame.record = &g_pChocoboRacerTable[11];
    }
    frame.record->scale = 0x280;
    for (frame.racerIndex = 12; frame.racerIndex < 16; frame.racerIndex++) {
        g_ChocoboRacers[(frame.racerIndex - 10)].tableIndex = frame.racerIndex;
        frame.record = &g_pChocoboRacerTable[frame.racerIndex];
        frame.record->scale = 0x280;
    }
    for (frame.racerIndex = 16; frame.racerIndex < (*(unsigned short *)&g_ChocoboRacerCounts); frame.racerIndex++) {
        frame.record = &g_pChocoboRacerTable[frame.racerIndex];
        frame.record->scale = 0x280;
    }
    for (frame.racerIndex = 0; frame.racerIndex < (*(unsigned short *)&g_ChocoboRacerCounts); frame.racerIndex++) {
        frame.record = &g_pChocoboRacerTable[frame.racerIndex];
        frame.record->angleX = 0xc0;
        frame.record->yaw = 0;
        frame.record->angleZ = 0x80;
    }
}

#pragma intrinsic(strcpy, strcat)

// The original assembles low RGB bytes through WORD and BYTE writes, then
// reads the DWORD and forces alpha to FF. The fourth byte is never initialized.
typedef union ChocoboPackedColor {
    unsigned int value;
    struct {
        unsigned short rg;
        unsigned char blue;
        unsigned char alpha;
    } c;
} ChocoboPackedColor;

/*
 * Returns the allocated file contents.
 */
// FUNCTION: FF7 0x0076c3c1
char *__cdecl Mini_ChocoboLoadFile(char *path)
{
    int size;
    char *buffer;
    char baseName[256];
    char suffix[256];
    FILE *fp;
    void *pHandle;
    char drive[4];
    char directory[256];
    buffer = 0;
    if (g_ChocoboRetailFlag == 0) {
        fp = Field_FileOpen(path, Chocobo_LoadFileMode());
        if (fp == 0)
            return 0;
        size = Field_FileSize(fp);
        buffer = (char *)GameHeapAllocRaw(size + 2, Chocobo_LoadFileName1(), 515);
        if (buffer == 0)
            return 0;
        Field_FileRead(fp, buffer, size);
        buffer[size] = 0;
        Field_FileClose(fp);
    } else {
        _splitpath(path, drive, directory, baseName, suffix);
        strcat(baseName, suffix);
        pHandle = FindLgpFile(baseName, LGP_ARCHIVE_CHOCOBO);
        if (pHandle) {
            size = GetLgpEntrySize((int)pHandle, LGP_ARCHIVE_CHOCOBO);
            buffer = (char *)GameHeapAllocRaw(size + 2, Chocobo_LoadFileName2(), 533);
            if (buffer == 0)
                return 0;
            ReadLgpEntry(pHandle, LGP_ARCHIVE_CHOCOBO, buffer, size);
            buffer[size] = 0;
        } else {
            return 0;
        }
    }
    return buffer;
}

/*
 * Only variant is read.
 */
// FUNCTION: FF7 0x0076c57e
void __cdecl Mini_ChocoboLoadAllAssets(int variant, int unused)
{
    int bytePos;
    int size;
    char *cursor;
    char path[256];
    void *ent;
    int cnt;
    FILE *pStream;
    /* Original frame reserves EBP-11C, which no instruction accesses.
       This retains that measured reservation; its source meaning is unknown. */
    int w;
    int numberRecords;
    // STRING: FF7 0x0096e218
    g_ChocoboTrackData = (ChocoboTrackData *)GameHeapAllocRaw(85, "C:\\FF7\\chocobo\\ch_init.cpp", 749);
    if (g_ChocoboTrackData == 0)
        return;
    g_ChocoboTrackData->racerFieldBase = Mini_ChocoboGetRacerStatBaseByVariant(variant);
    g_ChocoboTrackData->routeEvents = Mini_ChocoboGetCameraScriptByVariant(variant);
    g_ChocoboTrackData->scenerySlots = Mini_ChocoboGetSplineTableByVariant(variant);
    g_ChocoboTrackData->courseSounds = (int)Mini_ChocoboSelectCourseSounds(variant);
    if (g_ChocoboCourseVariant == 0) {
        g_ChocoboTrackData->courseId = 0x49d;
        g_ChocoboTrackData->segmentCount = 15;
        g_ChocoboTrackData->entryCount40 = 4713;
        g_ChocoboTrackData->entryCount28 = 11849;
        g_ChocoboTrackData->entryCount36 = 414;
        g_ChocoboTrackData->subEntryCount40 = 0xff;
        bytePos = 0;
        numberRecords = 1185;
        cnt = 15416;
    } else {
        g_ChocoboTrackData->courseId = 0x3b1;
        g_ChocoboTrackData->segmentCount = 19;
        g_ChocoboTrackData->entryCount40 = 4157;
        g_ChocoboTrackData->entryCount28 = 10146;
        g_ChocoboTrackData->entryCount36 = 414;
        g_ChocoboTrackData->subEntryCount40 = 90;
        bytePos = 0x9e078;
        numberRecords = 949;
        cnt = 12170;
    }
    strcpy(path, g_ChocoboAssetDirPath);
    // STRING: FF7 0x0096e234
    strcat(path, "chocobo.wat");
    if (g_ChocoboRetailFlag == 0) {
        // STRING: FF7 0x0096e240
        pStream = Field_FileOpen(path, "rb");
        Field_FileSeek(pStream, bytePos, 0);
        Field_FileSeek(pStream, 2, 1);
        // STRING: FF7 0x0096e244
        cursor = (char *)GameHeapAllocRaw(g_ChocoboTrackData->courseId * 24, "C:\\FF7\\chocobo\\ch_init.cpp", 795);
        Field_FileRead(pStream, cursor, g_ChocoboTrackData->courseId * 24);
        g_ChocoboTrackData->watBuffer = cursor;
        Field_FileSeek(pStream, 6, 1);
        // STRING: FF7 0x0096e260
        cursor = (char *)GameHeapAllocRaw(g_ChocoboTrackData->courseId * 8, "C:\\FF7\\chocobo\\ch_init.cpp", 801);
        Field_FileRead(pStream, cursor, g_ChocoboTrackData->courseId * 8);
        g_ChocoboTrackData->entries8 = cursor;
        // STRING: FF7 0x0096e27c
        cursor = (char *)GameHeapAllocRaw(numberRecords * 2, "C:\\FF7\\chocobo\\ch_init.cpp", 805);
        Field_FileSeek(pStream, 12, 1);
        Field_FileRead(pStream, cursor, numberRecords * 2);
        g_ChocoboTrackData->entries2 = cursor;
        // STRING: FF7 0x0096e298
        cursor = (char *)GameHeapAllocRaw(cnt * 4, "C:\\FF7\\chocobo\\ch_init.cpp", 810);
        Field_FileSeek(pStream, 11, 1);
        Field_FileRead(pStream, cursor, cnt * 4);
        g_ChocoboTrackData->entries4 = cursor;
        // STRING: FF7 0x0096e2b4
        cursor = (char *)GameHeapAllocRaw(g_ChocoboTrackData->entryCount36 * 36, "C:\\FF7\\chocobo\\ch_init.cpp", 815);
        Field_FileSeek(pStream, 9, 1);
        Field_FileRead(pStream, cursor, g_ChocoboTrackData->entryCount36 * 36);
        g_ChocoboTrackData->entries36 = cursor;
        // STRING: FF7 0x0096e2d0
        cursor = (char *)GameHeapAllocRaw(g_ChocoboTrackData->entryCount28 * 28, "C:\\FF7\\chocobo\\ch_init.cpp", 820);
        Field_FileSeek(pStream, 6, 1);
        Field_FileRead(pStream, cursor, g_ChocoboTrackData->entryCount28 * 28);
        g_ChocoboTrackData->entries28 = cursor;
        // STRING: FF7 0x0096e2ec
        cursor = (char *)GameHeapAllocRaw(g_ChocoboTrackData->entryCount40 * 40, "C:\\FF7\\chocobo\\ch_init.cpp", 825);
        Field_FileSeek(pStream, 6, 1);
        Field_FileRead(pStream, cursor, g_ChocoboTrackData->entryCount40 * 40);
        g_ChocoboTrackData->entries40 = cursor;
        // STRING: FF7 0x0096e308
        cursor = (char *)GameHeapAllocRaw(g_ChocoboTrackData->subEntryCount40 * 40, "C:\\FF7\\chocobo\\ch_init.cpp", 830);
        Field_FileSeek(pStream, 9, 1);
        Field_FileRead(pStream, cursor, g_ChocoboTrackData->subEntryCount40 * 40);
        g_ChocoboTrackData->subEntries40 = cursor;
        Field_FileClose(pStream);
    } else {
        // STRING: FF7 0x0096e324
        ent = FindLgpFile("chocobo.wat", LGP_ARCHIVE_CHOCOBO);
        if (ent) {
            size = GetLgpEntrySize((int)ent, LGP_ARCHIVE_CHOCOBO);
            // STRING: FF7 0x0096e330
            cursor = (char *)GameHeapAllocRaw(size + 2, "C:\\FF7\\chocobo\\ch_init.cpp", 841);
            ReadLgpEntry(ent, LGP_ARCHIVE_CHOCOBO, cursor, size);
            g_pChocoboWatBuffer = cursor;
            cursor[size] = 0;
            cursor += bytePos + 2;
            g_ChocoboTrackData->watBuffer = cursor;
            cursor += g_ChocoboTrackData->courseId * 24 + 6;
            g_ChocoboTrackData->entries8 = cursor;
            cursor += g_ChocoboTrackData->courseId * 8 + 12;
            g_ChocoboTrackData->entries2 = cursor;
            cursor += numberRecords * 2 + 11;
            g_ChocoboTrackData->entries4 = cursor;
            cursor += cnt * 4 + 9;
            g_ChocoboTrackData->entries36 = cursor;
            cursor += g_ChocoboTrackData->entryCount36 * 36 + 6;
            g_ChocoboTrackData->entries28 = cursor;
            cursor += g_ChocoboTrackData->entryCount28 * 28 + 6;
            g_ChocoboTrackData->entries40 = cursor;
            cursor += g_ChocoboTrackData->entryCount40 * 40 + 9;
            g_ChocoboTrackData->subEntries40 = cursor;
        }
    }
    Mini_ChocoboBuildFloatVertexTable();
    Mini_ChocoboRaceBuildCourseTriangleVertices();
    Mini_ChocoboRaceBuildCourseQuadVertices();
    g_pChocoboCourseWaypoints = (ChocoboCourseWaypoint *)g_ChocoboTrackData->watBuffer;
    g_ChocoboRaceHeapBlock = 0;
}

// Builds the float vertex table for the track. It allocates the table into the second
// ChocoboTrackData->builtFloatVertices slot, sized by floatVertexCount, and fills one 0x30-byte entry per record of
// trackVertexTable (0x24 bytes each). Each entry gets its three colours (color0 to color2) with
// alpha forced to 0xFF and their red and blue bytes swapped. Nine shorts per record (v0x to v2z)
// are widened to floats. The one-int locals r to z are how the original staged each conversion.
// FUNCTION: FF7 0x0076cca1
void __cdecl Mini_ChocoboBuildFloatVertexTable(void)
{
    ChocoboTrackVertex *trackVertexPtr;
    unsigned short recordIdx;
    int ax;
    int ay;
    int az;
    int bx;
    int by;
    int bz;
    int cx;
    int cy;
    int cz;
    unsigned char p;
    ChocoboTriangleEntry *destEntry;

    g_ChocoboTrackData->builtFloatVertices = (int)GameHeapAllocRaw(g_ChocoboTrackData->floatVertexCount * 48 + 1, Chocobo_VertexTableFileName(), 558);
    for (recordIdx = 0; recordIdx < g_ChocoboTrackData->floatVertexCount; recordIdx++) {
        destEntry = (ChocoboTriangleEntry *)(g_ChocoboTrackData->builtFloatVertices + recordIdx * 0x30);
        trackVertexPtr = (ChocoboTrackVertex *)(g_ChocoboTrackData->trackVertexTable + recordIdx * 0x24);
        ((ChocoboFloatVertex *)destEntry)->color0 = trackVertexPtr->color0 | 0xFF000000;
        p = destEntry->pairA0;
        destEntry->pairA0 = destEntry->pairB0;
        destEntry->pairB0 = p;
        ((ChocoboFloatVertex *)destEntry)->color1 = trackVertexPtr->color1 | 0xFF000000;
        p = destEntry->pairA1;
        destEntry->pairA1 = destEntry->pairB1;
        destEntry->pairB1 = p;
        ((ChocoboFloatVertex *)destEntry)->color2 = trackVertexPtr->color2 | 0xFF000000;
        p = destEntry->pairA2;
        destEntry->pairA2 = destEntry->pairB2;
        destEntry->pairB2 = p;
        ax = trackVertexPtr->v0x;
        destEntry->vertexAX = ax;
        ay = trackVertexPtr->v0y;
        destEntry->vertexAY = ay;
        az = trackVertexPtr->v0z;
        destEntry->vertexAZ = az;
        bx = trackVertexPtr->v1x;
        destEntry->vertexBX = bx;
        by = trackVertexPtr->v1y;
        destEntry->vertexBY = by;
        bz = trackVertexPtr->v1z;
        destEntry->vertexBZ = bz;
        cx = trackVertexPtr->v2x;
        destEntry->vertexCX = cx;
        cy = trackVertexPtr->v2y;
        destEntry->vertexCY = cy;
        cz = trackVertexPtr->v2z;
        destEntry->vertexCZ = cz;
    }
}

/*
 * Converts the course's packed triangles into float vertices for drawing: allocates the 48-byte
 * entry table (triangleCount entries) and, for each 28-byte packed record, writes three vertices
 * with opaque colours (alpha 0xFF) from the record's colour bytes, swaps each vertex's A and B
 * texture-pair bytes, and converts the short coordinates to floats.
 */
// FUNCTION: FF7 0x0076ce76
void __cdecl Mini_ChocoboRaceBuildCourseTriangleVertices(void)
{
    ChocoboPackedColor rgbBuffer;
    unsigned int c1;
    unsigned int c2;
    unsigned char swapByte;
    unsigned short triangleIndex;
    ChocoboTriangleEntry *dstEntry;
    ChocoboCourseVertex *sourceVertex;
    int color0;
    g_ChocoboTrackData->builtTriangles = (int)GameHeapAllocRaw(g_ChocoboTrackData->triangleCount * 48 + 1, Chocobo_Table4cFileName(), 610);
    for (triangleIndex = 0; triangleIndex < g_ChocoboTrackData->triangleCount; triangleIndex++) {
        dstEntry = (ChocoboTriangleEntry *)(g_ChocoboTrackData->builtTriangles + triangleIndex * 48);
        sourceVertex = (ChocoboCourseVertex *)(g_ChocoboTrackData->floatVertexTable + triangleIndex * 28);
        rgbBuffer.c.rg = sourceVertex->colorRG;
        rgbBuffer.c.blue = sourceVertex->colorBA & 255;
        c1 = rgbBuffer.value;
        color0 = sourceVertex->color0;
        rgbBuffer.c.rg = sourceVertex->colorRG;
        rgbBuffer.c.blue = sourceVertex->colorBA & 255;
        rgbBuffer.c.rg = sourceVertex->colorRG2;
        rgbBuffer.c.blue = sourceVertex->colorBA >> 8;
        c2 = rgbBuffer.value;
        ((ChocoboFloatVertex *)dstEntry)->color0 = color0 | 0xff000000;
        swapByte = dstEntry->pairA0;
        dstEntry->pairA0 = dstEntry->pairB0;
        dstEntry->pairB0 = swapByte;
        ((ChocoboFloatVertex *)dstEntry)->color1 = c1 | 0xff000000;
        swapByte = dstEntry->pairA1;
        dstEntry->pairA1 = dstEntry->pairB1;
        dstEntry->pairB1 = swapByte;
        ((ChocoboFloatVertex *)dstEntry)->color2 = c2 | 0xff000000;
        swapByte = dstEntry->pairA2;
        dstEntry->pairA2 = dstEntry->pairB2;
        dstEntry->pairB2 = swapByte;
        dstEntry->vertexAX = (float)sourceVertex->x;
        dstEntry->vertexAY = (float)sourceVertex->y;
        dstEntry->vertexAZ = (float)sourceVertex->z;
        dstEntry->vertexBX = (float)sourceVertex->vertexBX;
        dstEntry->vertexBY = (float)sourceVertex->vertexBY;
        dstEntry->vertexBZ = (float)sourceVertex->vertexBZ;
        dstEntry->vertexCX = (float)sourceVertex->vertexCX;
        dstEntry->vertexCY = (float)sourceVertex->vertexCY;
        dstEntry->vertexCZ = (float)sourceVertex->vertexCZ;
    }
}

/*
 * The same conversion for the course's packed quads: allocates the 64-byte entry table
 * (quadCount entries) and converts each 40-byte packed quad into four float vertices with opaque
 * colours.
 */
// FUNCTION: FF7 0x0076d09f
void __cdecl Mini_ChocoboRaceBuildCourseQuadVertices(void)
{
    ChocoboPackedColor rgbBuffer;
    unsigned int c1;
    unsigned int c2;
    unsigned char swapByte;
    int c3;
    ChocoboRaceCourseQuad *srcQuadPtr;
    unsigned short quadIndex;
    ChocoboCourseQuad *entry;
    int color0;
    g_ChocoboTrackData->builtQuads = (int)GameHeapAllocRaw((g_ChocoboTrackData->quadCount << 6) + 1, Chocobo_Table50FileName(), 675);
    for (quadIndex = 0; quadIndex < g_ChocoboTrackData->quadCount; quadIndex++) {
        entry = (ChocoboCourseQuad *)(g_ChocoboTrackData->builtQuads + quadIndex * 64);
        srcQuadPtr = (ChocoboRaceCourseQuad *)(g_ChocoboTrackData->block + quadIndex * 40);
        color0 = srcQuadPtr->color0;
        c3 = srcQuadPtr->color3;
        rgbBuffer.c.blue = srcQuadPtr->colorBA >> 8;
        rgbBuffer.c.rg = srcQuadPtr->colorRG;
        c1 = rgbBuffer.value;
        rgbBuffer.c.rg = srcQuadPtr->colorRG2;
        rgbBuffer.c.blue = *((unsigned char *)&srcQuadPtr->color3 + 3);
        c2 = rgbBuffer.value;
        entry->color0 = color0 | 0xff000000;
        swapByte = entry->color0Blue;
        entry->color0Blue = entry->color0Red;
        entry->color0Red = swapByte;
        entry->color1 = c1 | 0xff000000;
        swapByte = entry->color1Blue;
        entry->color1Blue = entry->color1Red;
        entry->color1Red = swapByte;
        entry->color2 = c2 | 0xff000000;
        swapByte = entry->color2Blue;
        entry->color2Blue = entry->color2Red;
        entry->color2Red = swapByte;
        entry->color3 = c3 | 0xff000000;
        swapByte = entry->color3Blue;
        entry->color3Blue = entry->color3Red;
        entry->color3Red = swapByte;
        entry->corner0X = (float)srcQuadPtr->corner0X;
        entry->corner0Y = (float)srcQuadPtr->corner0Y;
        entry->corner0Z = (float)srcQuadPtr->corner0Z;
        entry->corner1X = (float)srcQuadPtr->corner1X;
        entry->corner1Y = (float)srcQuadPtr->corner1Y;
        entry->corner1Z = (float)srcQuadPtr->corner1Z;
        entry->corner2X = (float)srcQuadPtr->corner2X;
        entry->corner2Y = (float)srcQuadPtr->corner2Y;
        entry->corner2Z = (float)srcQuadPtr->corner2Z;
        entry->corner3X = (float)srcQuadPtr->corner3X;
        entry->corner3Y = (float)srcQuadPtr->corner3Y;
        entry->corner3Z = (float)srcQuadPtr->corner3Z;
    }
}
#if 0
// Exact interior views of the actual compiler-emitted strcat literal.
// STRING: FF7 0x0096e238
"obo.wat";
// STRING: FF7 0x0096e23c
"wat";
#endif

/*
 * Empty function; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x0076d316
void __cdecl Mini_ChocoboRaceInitHookNoOp(void)
{
}
} /* extern "C" */
