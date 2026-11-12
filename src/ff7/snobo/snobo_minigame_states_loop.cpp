// src/ff7/snobo/snobo_minigame_states_loop.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The snowboard minigame's main module: setup and shutdown, the frame timing, and the
//   state machine that runs its intro, race, end sequences, result menu and fades. It also sets up
//   the UI scaling viewport and projection, preloads and releases the sound effects, and stores
//   course results.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/mem.h"
#include "ff7/snobo_course.h"
#include "ff7/types/snobo.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "lib/crt.h"
#include <math.h>
#include <string.h>
#include "ff7/api/snobo.h"
#include "ff7/api/snowboard.h"
#include "lib/api/sound.h"
#include "ff7/api/main.h"
#include "lib/api/file.h"
#define FF7_SETPERSPECTIVE_NARROW_ABI
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/codegen.h"
#include "lib/pfile.h"
#include "ff7/snobo_primitives.h"
#include "ff7/savemap.h"
#include "ff7/enums/main.h"
#include "ff7/enums/snobo.h"
#include "lib/enums/sound.h"
#include "lib/enums/file.h"

struct GfxEngineContext;

#pragma intrinsic(abs)
#pragma intrinsic(memset)
// Uses SnowboardClipper from ff7/snobo_primitives.h (shared class layout and member methods
// across the snowboard TUs).
extern "C" {
__declspec(dllimport) unsigned long __stdcall timeGetTime();
struct SnowboardSelectorTable {
    unsigned char selector[30];
    short triggerStageWeights[5];
};


struct SnowboardMatrixRecord;


#pragma intrinsic(abs)
#pragma intrinsic(cos)
#pragma intrinsic(sin)
#pragma intrinsic(sqrt, atan2)


// Mini_SnowboardInit's spelling: the original ChangeDirectorySaving passes this buffer to
// _getcwd with count 200.


/* Integer 0 for offsetX/offsetY emits VC5's 2-byte push 0 (6A 00) instead of 5-byte float push. */
extern void __cdecl SetPerspective(float fov, float nearPlane, float farPlane, int offsetXZero, int offsetYZero, float viewWidth, float viewHeight, struct GfxEngineContext *context);



// clang-format off

// The TU's initialized .data comes before its first function: VC5 binds a global's section where the code
// first uses it, so a definition after a use under #pragma data_seg stays in the default .data.
struct ColorPair {
    int a, b;
};
// The TU's initialized data is one pragma'd .data section in definition order: the zero word after the
// state root is a definition of its own, which the default .data would send to .bss (vc5-data-rules 1).
#pragma data_seg(".data")
// The snowboard state root: the code reaches its 660-byte state through it.
// GLOBAL: FF7 0x00926290
struct SnowboardRaceState *g_SnoboStatePtr = (struct SnowboardRaceState *)&g_SnoboContextState;
unsigned char g_rawData_00926294[4] = {0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00926298
short g_SnoboCourse1SectionTable[8] = {0, 301, 451, 351, 401, 501, 551, 0};
// GLOBAL: FF7 0x009262a8
short g_SnoboCourse2SectionTable[8] = {0, 8, 206, 215, 421, 0, 0, 0};
// GLOBAL: FF7 0x009262b8
short g_SnoboCourse1BlockTable[8] = {300, 350, 500, 400, 450, 550, 600, 0};
// GLOBAL: FF7 0x009262c8
short g_SnoboCameraPosWord[4] = {0x40, 0x800, 0, 0};
// The snowboard's six 16-byte course records (runs[] indexed by triggerStage).
// GLOBAL: FF7 0x009262d0
struct SnoboCourseRecords g_SnoboCourseRecords = {{0, 128, 320, 0}, {0x40, 0x800, 0, 0}, {0, 0x80, 0x140, 0}, {{0, 0, 0, {0}, 0, 0}, {0, 0, 0, {0}, 0, (int)0x99999999}, {0, 0, 0, {0}, 0, (int)0x99999999}, {0, 0, 0, {0}, 0, (int)0x99999999}}, 1, {0}};
// The fourteen sound effects the minigame preloads: the code indexes g_SnoboSfxIds[0..13], which runs on
// through the first twelve words of g_SnoboSfxIdsAndClockReset. Its word 12 starts at 1: while it is
// set, Mini_SnowboardAccumulateFrameTime restarts the frame clock and clears it.
// GLOBAL: FF7 0x00926330
int g_SnoboSfxIds[2] = {0xED, 0xEE};
// GLOBAL: FF7 0x00926338
int g_SnoboSfxIdsAndClockReset[14] = {0xf0, 0xf9, 0x105, 0x10c, 0x10f, 0x189, 0x135, 0x1ab, 0xef, 0x1b9, 0x38, 0xcd, 1, 0};
// The course-block vertex Mini_SnowboardUpdateRiderPosition places the rider on, by (triangle >> 1) % 6.
// GLOBAL: FF7 0x00926370
short g_SnoboRiderPoseVertex[8] = {0x2b, 0x2c, 0x2d, 0x2d, 0x2e, 0x2f, 0, 0};
// g_SnoboCourseResultBySegment: Mini_SnowboardStoreCourseResult stores its byte for the state's segment
// into gSavemap.snowboardCourseResult. The span runs to the next named
// definition of the TU; the TU's pieces inside 0x926380..0x926388 are aliases.
// GLOBAL: FF7 0x00926380
char g_SnoboCourseResultBySegment[0x8] = {0x00, 0x00, 0x00, 0x02, 0x01, 0x04, 0x08, 0x00};
// GLOBAL: FF7 0x00926388
short g_SnoboAnimFrameActionPairs[4] = {210, 209, 224, 223};
// GLOBAL: FF7 0x00926390
short g_SnoboAnimFrameTailActions[4] = {281, 283, 282, 283};
// The colour pairs the course mesh selects by the node's segmentIndex: seven pairs.
// GLOBAL: FF7 0x00926398
ColorPair g_SnoboColorPairs[7] = {{0x20f8e8e0, 0x20f0c8b0}, {0x20f8e8e0, 0x20f0c8b0}, {0x20e0f8e0, 0x20b0f0b0}, {0x20e0f8e0, 0x20b0f0b0}, {0x20f8e0e0, 0x20f0c0c0}, {0x20d8f8f8, 0x20a8e8e8}, {0x20f8e0f8, 0x20f0c0f0}};
// GLOBAL: FF7 0x009263d0
short g_SnoboNodeMarkerVertexSlots[6] = {7, 13, 21, 27, 35, 41};
// GLOBAL: FF7 0x009263e0
unsigned char g_SnoboProbeHandlerIndex[64] = {0x02, 0x02, 0x02, 0x02, 0x08, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x08, 0x01, 0x01, 0x01, 0x01, 0x08, 0x02, 0x02, 0x02, 0x08, 0x00, 0x00, 0x08, 0x01, 0x03, 0x00, 0x08, 0x01, 0x01, 0x08, 0x08, 0x02, 0x02, 0x02, 0x08, 0x08, 0x00, 0x00, 0x08, 0x08, 0x00, 0x03, 0x01, 0x08, 0x08, 0x01, 0x01, 0x08, 0x02, 0x02, 0x08, 0x08, 0x00, 0x00, 0x08, 0x04, 0x06, 0x07, 0x05, 0x04, 0x04, 0x05, 0x05};
// The course-record opcode handlers Mini_SnowboardProbeFixedPositions dispatches through.
// GLOBAL: FF7 0x00926420
SnowboardOpcode g_SnoboProbeHandlers[9] = {(SnowboardOpcode)Mini_SnowboardProbeRow, (SnowboardOpcode)Mini_SnowboardAdvanceSegment, (SnowboardOpcode)Mini_SnowboardRetreatSegment, (SnowboardOpcode)Mini_SnowboardProbeRowOrAdvance, (SnowboardOpcode)Mini_SnowboardLoadLinkedBlockAt4, (SnowboardOpcode)Mini_SnowboardLoadLinkedBlockAt5, (SnowboardOpcode)Mini_SnowboardProbeOrLoadLinkedBlockAt4, (SnowboardOpcode)Mini_SnowboardLocateCourseTriangle, (SnowboardOpcode)Mini_SnowboardCourseRecordSlot8RequestRetry};
// The 30-byte selector table indexed by the low byte of the state's riderState, then the per-course
// shorts indexed by triggerStage.
// GLOBAL: FF7 0x00926448
struct SnowboardSelectorTable g_SnoboSelectorTable = {{0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00}, {0, 0x5a, 0x69, 0x78, 0}};
// The three 32-byte course threshold records (eight words each).
// GLOBAL: FF7 0x00926470
unsigned int g_SnoboThresholdRecords[3][8] = {{53000, 54000, 55000, 60000, 65000, 75000, 90000, 0xffffffff}, {68000, 70000, 72000, 75000, 80000, 100000, 120000, 0xffffffff}, {75000, 77000, 81000, 85000, 95000, 110000, 130000, 0xffffffff}};
#pragma data_seg()
// clang-format on

/*
 * Picks the viewport from GetUiScalingMode: MAIN_GRAPHICS_MODE_320X240 gives
 * GfxSetViewportRect(0, 0, 0x140, 0xF0, context), MAIN_GRAPHICS_MODE_320X240_CENTERED gives
 * (0xA0, 0x78, 0x140, 0xF0) so the 320x240 image sits in the middle of a 640x480 target, and
 * anything else gives the full (0, 0, 0x280, 0x1E0).
 */
// FUNCTION: FF7 0x00722e60
void __cdecl Gfx_SetUiScalingViewport(struct GfxEngineContext *context)
{
    int mode;

    mode = GetUiScalingMode();
    if (mode == MAIN_GRAPHICS_MODE_320X240) {
        GfxSetViewportRect(0, 0, 0x140, 0xF0, context);
    } else if (mode == MAIN_GRAPHICS_MODE_320X240_CENTERED) {
        GfxSetViewportRect(0xA0, 0x78, 0x140, 0xF0, context);
    } else {
        GfxSetViewportRect(0, 0, 0x280, 0x1E0, context);
    }
}

/*
 * Initializes two identity matrices and passes them to
 * Gfx_SetContextMatrix/Gfx_RebuildContextScreenTransform, calls
 * Gfx_SetUiScalingViewport, configures perspective using the context's
 * viewSizeX/viewSizeY dimensions and constants 56.15, 64 and 16384, then returns 1.
 */
// FUNCTION: FF7 0x00722ed1
int __cdecl Mini_SnowboardSetupProjection(struct GfxEngineContext *context)
{
    float contextMtx[4][4];
    float screenMatrix[4][4];

    SetIdentityMatrix4x4(contextMtx);
    Gfx_SetContextMatrix((void *)contextMtx, context);
    SetIdentityMatrix4x4(screenMatrix);
    Gfx_RebuildContextScreenTransform(screenMatrix, context);
    Gfx_SetUiScalingViewport(context);
    SetPerspective(56.15f, 64.0f, 16384.0f, 0, 0,
                   (float)context->viewSizeX,
                   (float)context->viewSizeY, context);
    return 1;
}

// Sets limitHigh to 4096.0f and limitLow to 3072.0f, then installs one of two course-resource
// pointer sets into the snowboard state, chosen by g_SnoboModeFlag. The Gold Saucer mode selects the
// first set of course tables and clears blockTable. The event mode selects the alternate set
// (g_SnoboCourse1Header and its companions) and points blockTable at g_SnoboCourse1BlockTable.
// FUNCTION: FF7 0x00722f60
void __cdecl Mini_SnowboardInstallCourseDataPointers(void)
{
    g_SnoboStatePtr->limitHigh = FLOAT_4096_BITS;
    g_SnoboStatePtr->limitLow = FLOAT_3072_BITS;
    if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
        g_SnoboStatePtr->courseHeader = (SnowboardCourseBlock *)&g_SnoboCourse2Header;
        g_SnoboStatePtr->blockIndexTable = g_SnoboCourse2BlockIndexTable;
        g_SnoboStatePtr->courseDataA = (int)&g_SnoboCourse2ActionTable;
        g_SnoboStatePtr->effectPosTable = (int)&g_SnoboCourse2EffectPosTable;
        g_SnoboStatePtr->courseDataB = (int)&g_SnoboCourse2AngleTable;
        g_SnoboStatePtr->courseDataC = (int)&g_SnoboCourse2ChainBitTable;
        g_SnoboStatePtr->courseDataD = (int)&g_SnoboCourse2ChainIndexTable;
        g_SnoboStatePtr->segmentTable = (SnowboardSegment *)&g_SnoboCourse2SegmentTable;
        g_SnoboStatePtr->sectionNames = (int)&g_SnoboCourse2SectionNames;
        g_SnoboStatePtr->sectionTable = g_SnoboCourse2SectionTable;
        g_SnoboStatePtr->blockTable = 0;
    } else {
        g_SnoboStatePtr->courseHeader = (SnowboardCourseBlock *)&g_SnoboCourse1Header;
        g_SnoboStatePtr->blockIndexTable = g_SnoboCourse1BlockIndexTable;
        g_SnoboStatePtr->courseDataA = (int)&g_SnoboCourse1ActionTable;
        g_SnoboStatePtr->effectPosTable = (int)&g_SnoboCourse1EffectPosTable;
        g_SnoboStatePtr->courseDataB = (int)&g_SnoboCourse1AngleTable;
        g_SnoboStatePtr->courseDataC = (int)&g_SnoboCourse1ChainBitTable;
        g_SnoboStatePtr->courseDataD = (int)&g_SnoboCourse1ChainIndexTable;
        g_SnoboStatePtr->segmentTable = (SnowboardSegment *)&g_SnoboCourse1SegmentTable;
        g_SnoboStatePtr->sectionNames = (int)&g_SnoboCourse1SectionNames;
        g_SnoboStatePtr->sectionTable = g_SnoboCourse1SectionTable;
        g_SnoboStatePtr->blockTable = g_SnoboCourse1BlockTable;
    }
}

/*
 * Changes to the snowboard directory, opens snowboard-us.lgp as archive 10 and
 * creates clipper, loader, course assets and renderer objects. Loads
 * for_ev.tmd for event mode or for_gs.tmd otherwise, selecting course kind
 * from gSavemap.snowTempVar, then initializes assets, renderer and primitive buffer.
 */
// FUNCTION: FF7 0x007230ef
int __cdecl Mini_SnowboardInit(struct GfxEngineContext *ctx)
{
    ChangeDirectorySaving(GetMinigameDirForSnowboard(), g_SnoboSavedDir);
    if (!OpenLgpArchive(
            // STRING: FF7 0x009264d0
            "snowboard-us.lgp", LGP_ARCHIVE_SNOWBOARD))
        return 0;
    Mini_SnowboardInstallCourseDataPointers();
    g_SnoboStatePtr->clipper = new SnowboardClipper;
    if (!Mini_SnowboardSetupProjection(ctx))
        return 0;
    g_SnoboStatePtr->courseLoader = new SnowboardCourseLoader;
    if (g_SnoboModeFlag == SNOWBOARD_MODE_EVENT) {
        if (!g_SnoboStatePtr->courseLoader->load((int)
                                                                             // STRING: FF7 0x009264e4
                                                                             "for_ev.tmd",
                                                                             0))
            return 0;
        g_SnoboStatePtr->courseIndex = 0;
    } else {
        if (!g_SnoboStatePtr->courseLoader->load((int)
                                                                             // STRING: FF7 0x009264f0
                                                                             "for_gs.tmd",
                                                                             0))
            return 0;
        switch (gSavemap.snowTempVar) {
        case SNOWBOARD_SAVEMAP_COURSE_B:
            g_SnoboStatePtr->courseIndex = 1;
            break;
        case SNOWBOARD_SAVEMAP_TIME_ATTACK:
            g_SnoboStatePtr->courseIndex = 2;
            break;
        default:
            g_SnoboStatePtr->courseIndex = 0;
        }
    }
    g_SnoboActiveFlagMask = 1;
    g_SnoboStatePtr->courseAssets = new SnowboardCourseAssets(g_SnoboStatePtr->courseLoader);
    g_SnoboStatePtr->courseAssets->initialize(g_SnoboStatePtr->courseIndex);
    g_pSnoboRenderer = new SnowboardRenderer;
    if (!g_pSnoboRenderer)
        return 0;
    if (!g_pSnoboRenderer->initialize())
        return 0;
    Mini_InitPrimBuffer(0x18, 0xf0, 0x280, 0xf0, 0, 0);
    Mini_SnowboardPreloadSfx();
    if (g_SnoboModeFlag == SNOWBOARD_MODE_EVENT) {
        g_SnoboStatePtr->soundCommand->command = SOUND_SFX_ALL_PAN;
        g_SnoboStatePtr->soundCommand->volume = 0x40;
        SoundDispatchCommand(g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, 0, 0, 0, 0, 0, 0, 0);
        g_SnoboStatePtr->soundCommand->command = SOUND_SFX_ALL_VOLUME;
        g_SnoboStatePtr->soundCommand->volume = 0x7f;
        SoundDispatchCommand(g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, 0, 0, 0, 0, 0, 0, 0);
        g_SnoboStatePtr->soundCommand->command = SOUND_SFX_ALL_TRANSPOSE;
        g_SnoboStatePtr->soundCommand->volume = 0x0;
        SoundDispatchCommand(g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, 0, 0, 0, 0, 0, 0, 0);
        g_SnoboStatePtr->soundCommand->command = SOUND_MUSIC_PLAY;
        g_SnoboStatePtr->soundCommand->volume = 0x41;
        SoundDispatchCommand(g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, 0, 0, 0, 0, 0, 0, 0);
        g_SnoboStatePtr->soundCommand->command = SOUND_MUSIC_VOLUME;
        g_SnoboStatePtr->soundCommand->volume = 0x7f;
        SoundDispatchCommand(g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, 0, 0, 0, 0, 0, 0, 0);
    }
    if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
        Mini_SnowboardLoadContextColorsAndClearSlots();
        g_SnoboActive = 1;
        g_SnoboStateHandler = Mini_SnowboardStateBeginFadeIn;
    } else {
        Mini_SnowboardInitRunAtCourseEntry(0, 0, 0x10);
        Mini_SnowboardResetRiderRecord();
        g_SnoboActive = 1;
        g_SnoboStatePtr->flags = 0;
        g_SnoboStatePtr->rotationY = 0;
        g_SnoboFadeLevel = 0;
        g_SnoboStateHandler = Mini_SnowboardStateFadeIn;
    }
    return 1;
}
} // extern "C"

/*
 * In the Gold Saucer mode, runs
 * Mini_SnowboardStoreBestRecordToSavemap/Mini_SnowboardReleaseRunSampleBuffers
 * and stops MIDI; in the event mode, dispatches SOUND_MUSIC_VOLUME with zero
 * arguments and stores the course result. Always runs Mini_SnowboardReleaseSfx
 * and Mini_ReleasePrimBuffer, deletes the renderer and three
 * loader/assets/clipper objects, closes archive 0xA, and restores the saved
 * directory.
 */
// FUNCTION: FF7 0x007235ea
extern "C" void __cdecl Mini_SnowboardShutdown(void)
{
    if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
        Mini_SnowboardStoreBestRecordToSavemap();
        Mini_SnowboardReleaseRunSampleBuffers();
        MidiStop();
    }
    if (g_SnoboModeFlag == SNOWBOARD_MODE_EVENT) {
        g_SnoboStatePtr->soundCommand->command = SOUND_MUSIC_VOLUME;
        g_SnoboStatePtr->soundCommand->volume = 0;
        SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, 0, 0, 0, 0, 0, 0, 0);
        Mini_SnowboardStoreCourseResult();
    }
    Mini_SnowboardReleaseSfx();
    Mini_ReleasePrimBuffer();
    delete g_pSnoboRenderer;
    delete g_SnoboStatePtr->courseLoader;
    delete g_SnoboStatePtr->courseAssets;
    delete g_SnoboStatePtr->clipper;
    CloseLgpArchive(LGP_ARCHIVE_SNOWBOARD);
    ChangeDirectorySaving(g_SnoboSavedDir, 0);
}
extern "C" {

// Writes posX, posY and posZ to out. Each is the matching relPosX, relPosY or riderPos field scaled
// by approximately 1/4096 (0.00024414062f), minus originX, originY or originZ.
// FUNCTION: FF7 0x007237ae
void __cdecl Mini_SnowboardWriteRelativePosition(SnowboardRelativePosition *out)
{
    float scale;

    scale = 0.00024414062f;
    out->posX = g_SnoboStatePtr->relPosX * scale - g_SnoboStatePtr->originX;
    out->posY = g_SnoboStatePtr->relPosY * scale - g_SnoboStatePtr->originY;
    out->posZ = g_SnoboStatePtr->riderPos * scale - g_SnoboStatePtr->originZ;
}

/*
 * For an active snowboard simulation, updates timing/input/course and runs
 * state callbacks until the processed frame count catches up with accumulated
 * time, marking only the final callback for rendering. Updates HUD, runs race-
 * only and shared race/end/menu rendering helpers and increments the short
 * frame counter.
 */
// FUNCTION: FF7 0x0072381c
int __cdecl Mini_SnowboardUpdate(struct GfxEngineContext *timing)
{
    if (g_SnoboActive != 0) {
        Mini_SnowboardAccumulateFrameTime(timing);
        Mini_SnowboardUpdateInputMasks();
        g_pSnoboRenderer->buildCourse();
        while (g_SnoboStatePtr->framesRun + 1.0f < g_SnoboStatePtr->frameTime) {
            g_SnoboStatePtr->renderFrame = 0;
            g_SnoboStateHandler();
            g_SnoboStatePtr->framesRun += 1.0f;
        }
        g_SnoboStatePtr->renderFrame = 1;
        g_SnoboStateHandler();
        g_SnoboStatePtr->framesRun += 1.0f;
        Mini_SnowboardFlushMatrixRecords(0, 0, 0);
        ((SnowboardHud *)g_pSnoboRenderer)->updateHud();
        if (g_SnoboStateHandler == Mini_SnowboardStateRace) {
            Mini_SnowboardRacePauseMenu();
            Mini_SnowboardEmitOverlay((int)g_pSnoboRenderer);
        } else if (g_SnoboStateHandler == Mini_SnowboardStateEndHold0 || g_SnoboStateHandler == Mini_SnowboardStateEndHold1 || g_SnoboStateHandler == Mini_SnowboardStateMenu) {
            Mini_SnowboardEmitOverlay((int)g_pSnoboRenderer);
        }
        g_SnoboStatePtr->tick++;
    }
    return g_SnoboActive == 0;
}


#pragma intrinsic(abs)

#pragma intrinsic(memset)


__declspec(dllimport) unsigned long __stdcall timeGetTime();

// clang-format off
// GLOBAL: FF7 0x00dd7c38
Timestamp64 g_SnoboLastFrameTicks = {0};
// GLOBAL: FF7 0x00dd7c40
short g_SnoboPauseMenuCursor = 0;
unsigned char g_bssPad_00dd7c44[4] = {0};
// GLOBAL: FF7 0x00dd7c48
int g_SnoboFadeLevel = 0;
// GLOBAL: FF7 0x00dd7c4c
int g_SnoboAltEndTimer = 0;
// GLOBAL: FF7 0x00dd7c50
int g_SnoboMenuSelection = 0;
// Set to 0x10 when a Gold Saucer run reaches the course end; nothing reads it.
// GLOBAL: FF7 0x00dd7c54
int g_SnoboFinishCode = 0;
// GLOBAL: FF7 0x00dd7c58
int g_SnoboCourseTimeRank = 0;
// GLOBAL: FF7 0x00dd7c5c
int g_SnoboResultClass = 0;
// GLOBAL: FF7 0x00dd7c60
int g_SnoboMenuSavedInputFlags = 0;
// GLOBAL: FF7 0x00dd7c64
int g_SnoboMenuSavedModeFlags = 0;
// GLOBAL: FF7 0x00dd7c68
float g_SnoboCameraPos[3] = {0.0f};
// GLOBAL: FF7 0x00dd7c74
float g_SnoboCameraX = 0.0f;
// GLOBAL: FF7 0x00dd7c78
float g_SnoboCameraY = 0.0f;
// GLOBAL: FF7 0x00dd7c7c
float g_SnoboCameraZ = 0.0f;
// GLOBAL: FF7 0x00dd7c80
struct SnowboardRelativePosition g_SnoboRelativePos0 = {0};
// GLOBAL: FF7 0x00dd7c8c
short g_SnoboPauseMenuState = 0;
// GLOBAL: FF7 0x00dd7c90
struct SnowboardRelativePosition g_SnoboRelativePos1 = {0};
unsigned char g_bssPad_00dd7c9c[4] = {0};
// GLOBAL: FF7 0x00dd7ca0
struct SnowboardRelativePosition g_SnoboRiderRelativePos = {0};
// GLOBAL: FF7 0x00dd7cac
int g_SnoboActive = 0;
// GLOBAL: FF7 0x00dd7cb0
struct SnowboardTriangleVertex g_SnoboSurfaceTriangle[3] = {0};
// GLOBAL: FF7 0x00dd7cc8
int g_SnoboRunFinished = 0;
// GLOBAL: FF7 0x00dd7ccc
int g_SnoboCourseEndReached = 0;
// GLOBAL: FF7 0x00dd7cd0
int g_SnoboLastSfxBits = 0;
// GLOBAL: FF7 0x00dd7cd4
SnowboardRenderer *g_pSnoboRenderer = 0;
unsigned char g_bssPad_00dd7cd8[20] = {0};
// GLOBAL: FF7 0x00dd7cec
SnoboStateFn g_SnoboStateHandler = 0;
// GLOBAL: FF7 0x00dd7cf0
SnowboardCourseActionRecord g_SnoboCourseActionRecords[20] = {0};
unsigned char g_bssPad_00dd82e0[4] = {0};
// GLOBAL: FF7 0x00dd82e4
short g_SnoboActiveFlagMask = 0;
// GLOBAL: FF7 0x00dd82e8
char g_SnoboSavedDir[200] = {0};
// GLOBAL: FF7 0x00dd83b0
struct SnowboardTriangleVertex *g_SnoboSurfaceSeamVertices = 0;
unsigned char g_bssPad_00dd83b4[4] = {0};
// GLOBAL: FF7 0x00dd83b8
unsigned char g_SnoboContextState[660] = {0};
unsigned char g_bssPad_00dd864c[4] = {0};
unsigned char g_bssPad_00dd8650[8] = {0};
// GLOBAL: FF7 0x00dd8658
int g_SnoboCameraStepCount = 0;
// GLOBAL: FF7 0x00dd865c
int g_SnoboModeFlag = 0;
// clang-format on
// The TU's own view of moved definitions declared with another type.

#pragma intrinsic(abs)
#pragma intrinsic(cos)
#pragma intrinsic(sin)
#pragma intrinsic(sqrt, atan2)

/*
 * On timing reset, records the timestamp and clears both frame accumulators.
 * Otherwise subtracts the previous timestamp, divides by the engine timing
 * scale and multiplies by sixty, caps the increment at 32 frames, adds it to
 * accumulated time and stores the new timestamp.
 */
// FUNCTION: FF7 0x00723960
void __cdecl Mini_SnowboardAccumulateFrameTime(struct GfxEngineContext *timing)
{
    int elapsed[2];
    int ticks[2];
    double ratio;
    float increment;

    if (g_SnoboSfxIdsAndClockReset[12] != 0) {
        ReadTimestampCounter(&g_SnoboLastFrameTicks);
        g_SnoboStatePtr->frameTime = 0.0f;
        g_SnoboStatePtr->framesRun = 0.0f;
        g_SnoboSfxIdsAndClockReset[12] = 0;
    } else {
        ReadTimestampCounter((struct Timestamp64 *)ticks);
        Sub64((struct Timestamp64 *)ticks, &g_SnoboLastFrameTicks, (struct Timestamp64 *)elapsed);
        increment = (float)((ratio = CombineSplitValueToFloat((struct Timestamp64 *)elapsed) / timing->tscFrequency) * 60.0);
        if (*(volatile float *)&increment >= 32.0f) {
            increment = 32.0f;
        }
        g_SnoboStatePtr->frameTime += increment;
        g_SnoboLastFrameTicks.lo = ticks[0];
        g_SnoboLastFrameTicks.hi = ticks[1];
    }
}

/*
 * Calls the fade/setup helpers, sets MidiSetVolume(0) only in the Gold Saucer
 * mode, calls Mini_SnowboardRendererFillColor(0)
 * and selects Mini_SnowboardStateFadeIn in g_SnoboStateHandler.
 */
// FUNCTION: FF7 0x00723a30
void __cdecl Mini_SnowboardStateBeginFadeIn(void)
{
    Mini_SnowboardResetSoundState();
    Mini_SnowboardStoreBestRecordToSavemap();
    Mini_SnowboardInitRunAtCourseEntry(0, 0, 0x10);
    if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
        MidiSetVolume(0);
    }
    Mini_SnowboardStartSong();
    Mini_SnowboardRendererFillColor(0);
    g_SnoboStateHandler = Mini_SnowboardStateFadeIn;
}

/*
 * Passes the low byte of g_SnoboFadeLevel repeated into RGB to
 * Mini_SnowboardRendererFillColor, increments g_SnoboFadeLevel by four,
 * and at 0x100 resets it to zero and selects Mini_SnowboardStateIntro.
 */
// FUNCTION: FF7 0x00723a79
void __cdecl Mini_SnowboardStateFadeIn(void)
{
    Mini_SnowboardRendererFillColor(*(unsigned char *)&g_SnoboFadeLevel << 0x10 | *(unsigned char *)&g_SnoboFadeLevel << 8 | *(unsigned char *)&g_SnoboFadeLevel);
    g_SnoboFadeLevel += 4;
    if (g_SnoboFadeLevel >= 0x100) {
        g_SnoboFadeLevel = 0;
        g_SnoboStateHandler = Mini_SnowboardStateIntro;
    }
}

/*
 * Advances snowboard intro counter, allowing input bit 0x800 to skip to its
 * final frame and restore MIDI volume. During intro fades rendering and volume
 * and interpolates fixed camera fields; afterward copies target camera values.
 */
// FUNCTION: FF7 0x00723ad5
void __cdecl Mini_SnowboardStateIntro(void)
{
    int relPosition[3];
    int fade;

    if ((g_SnoboStatePtr->modeFlags & INPUT_START) != 0) {
        Mini_SnowboardMarkSlotFlag(0);
        g_SnoboFadeLevel = 0xFF;
        if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
            MidiSetVolume(0x7F);
        }
    }
    g_SnoboFadeLevel++;
    fade = g_SnoboFadeLevel;
    if (fade < 0x80) {
        Mini_SnowboardRendererFadeWhite(0x100 - (fade << 1));
        if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
            MidiSetVolume(fade);
        }
    }
    if (fade < 0x100) {
        g_SnoboCameraPosWord[0] = (fade * 0x60 >> 8) - 0x20;
        g_SnoboCameraPosWord[1] = 0x1000 - (fade << 0xB >> 8);
        g_SnoboCameraPosWord[2] = 0;
        g_SnoboCourseRecords.cameraOffset[0] = 0;
        g_SnoboCourseRecords.cameraOffset[1] = (fade << 6 >> 8) + 0x40;
        g_SnoboCourseRecords.cameraOffset[2] = (fade * 0xE0 >> 8) + 0x60;
    } else {
        *(int *)&g_SnoboCameraPosWord[0] = *(int *)&g_SnoboCourseRecords.introEndAngles[0];
        *(int *)&g_SnoboCameraPosWord[2] = *(int *)&g_SnoboCourseRecords.introEndAngles[2];
        *(int *)&g_SnoboCourseRecords.cameraOffset[0] = *(int *)&g_SnoboCourseRecords.introEndOffset[0];
        *(int *)&g_SnoboCourseRecords.cameraOffset[2] = *(int *)&g_SnoboCourseRecords.introEndOffset[2];
    }
    Mini_SnowboardProbeFloatPositions(&g_SnoboStatePtr->relPosX, &g_SnoboStatePtr->relPosX);
    Mini_SnowboardLoadSegmentVector();
    /* Retail caller passes -1 arg(s) to 0-parameter `Mini_SnowboardUpdateCameraFromContext` (__cdecl stack cleanup matches retail). */
    ((void(__cdecl *)(...))Mini_SnowboardUpdateCameraFromContext)(Gfx_GetPsxScratchpadPtr(0));
    Mini_SnowboardEmitCourseChain(1, 2);
    Mini_SnowboardWriteRelativePosition((struct SnowboardRelativePosition *)(relPosition));
    Mini_SnowboardRunActionFrame(relPosition, Gfx_GetPsxScratchpadPtr(0), 1, 1);
    if (((unsigned short)g_SnoboStatePtr->riderState & 0xFF) != 0x10 && g_SnoboFadeLevel > 0xFF) {
        g_SnoboStateHandler = Mini_SnowboardStateRace;
        if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
            g_SnoboFadeLevel = 0;
        } else {
            g_SnoboStatePtr->triggerLatched = 1;
        }
    }
}

/*
 * Stores the course result: reads the byte at g_SnoboCourseResultBySegment indexed by
 * the state's segment and writes it to
 * gSavemap.snowboardCourseResult.
 */
// FUNCTION: FF7 0x00723cbb
void __cdecl Mini_SnowboardStoreCourseResult(void)
{
    gSavemap.snowboardCourseResult = g_SnoboCourseResultBySegment[g_SnoboStatePtr->segment];
}

/*
 * Clears input fields, updates end-state rendering and shows the menu during a
 * postincremented hold counter. At old counter 0x50 or later, selects
 * animation code from score thresholds 100/70/50/30, initializes menu state
 * and changes the callback to the menu.
 */
// FUNCTION: FF7 0x00723cd8
void __cdecl Mini_SnowboardStateEndHold0(void)
{
    int animCode;
    int lowTier;
    int midTier;

    g_SnoboStatePtr->modeFlags = 0;
    g_SnoboStatePtr->inputFlags = 0;
    Mini_SnowboardEmitCameraFrame();
    g_pSnoboRenderer->showMenu(Gfx_GetPsxScratchpadPtr(0), g_SnoboFadeLevel);
    if (g_SnoboFadeLevel++ >= 0x50) {
        if (g_SnoboStatePtr->resultTotal >= 0x64) {
            animCode = 0x1B;
        } else {
            if (g_SnoboStatePtr->resultTotal >= 0x46) {
                midTier = 0x1E;
            } else {
                if (g_SnoboStatePtr->resultTotal >= 0x32) {
                    lowTier = 0;
                } else {
                    lowTier = g_SnoboStatePtr->resultTotal >= 0x1E ? 0x1D : 0x1F;
                }
                midTier = lowTier;
            }
            animCode = midTier;
        }
        g_SnoboStatePtr->riderState = animCode;
        g_SnoboMenuSelection = 0;
        g_SnoboResultClass = Mini_SnowboardClassifyResult();
        g_SnoboStateHandler = Mini_SnowboardStateMenu;
    }
}

/*
 * Optionally plays the result sound at the first hold frame, clears input and
 * renders race results. After counter 0x24, chooses the result animation from
 * the special-result flag or thresholds 30/15, dispatches SOUND_SFX_CH4_VOLUME_TRANS
 * with duration 300, initializes menu state and changes to the menu callback.
 */
// FUNCTION: FF7 0x00723dde
void __cdecl Mini_SnowboardStateEndHold1(void)
{
    int animCode;
    int lowTier;

    if (g_SnoboAltEndTimer != 0 && g_SnoboFadeLevel == 0) {
        Mini_SnowboardPlaySfxOnChannel(4, 0x1B9, 0x7F);
    }
    g_SnoboStatePtr->modeFlags = 0;
    g_SnoboStatePtr->inputFlags = 0;
    Mini_SnowboardEmitCameraFrame();
    g_pSnoboRenderer->renderRaceState(g_SnoboStatePtr->triggerStage - 1, g_SnoboCourseTimeRank, g_SnoboFadeLevel);
    g_SnoboFadeLevel++;
    if (g_SnoboFadeLevel > 0x24) {
        if (g_SnoboAltEndTimer != 0) {
            animCode = 0x1E;
        } else {
            if (g_SnoboStatePtr->resultPartB >= 0x1E) {
                lowTier = 0;
            } else {
                lowTier = g_SnoboStatePtr->resultPartB >= 0xF ? 0x1D : 0x1F;
            }
            animCode = lowTier;
        }
        g_SnoboStatePtr->riderState = animCode;
        g_SnoboStatePtr->soundCommand->command = SOUND_SFX_CH4_VOLUME_TRANS;
        g_SnoboStatePtr->soundCommand->volume = 0x12C;
        g_SnoboStatePtr->soundCommand->pitch = 0;
        SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, g_SnoboStatePtr->soundCommand->pitch, 0, 0, 0, 0, 0, 0);
        g_SnoboMenuSelection = 0;
        g_SnoboResultClass = Mini_SnowboardClassifyResult();
        g_SnoboStateHandler = Mini_SnowboardStateMenu;
    }
}

/*
 * Changes the two-choice result-menu selection from directional input, saves
 * and clears input around the scene update then restores it for menu
 * processing. Renders the selected menu plus race results or normal menu, and
 * on input mask 0x860 resets the counter and chooses quit or fade-out
 * according to selection.
 */
// FUNCTION: FF7 0x00723f60
void __cdecl Mini_SnowboardStateMenu(void)
{
    SnoboStateFn nextState;

    if (g_SnoboMenuSelection != 0) {
        if ((g_SnoboStatePtr->modeFlags & INPUT_UP) != 0) {
            g_SnoboMenuSelection = 0;
        }
    } else if ((g_SnoboStatePtr->modeFlags & INPUT_DOWN) != 0) {
        g_SnoboMenuSelection = 1;
    }
    g_SnoboMenuSavedInputFlags = g_SnoboStatePtr->inputFlags;
    g_SnoboMenuSavedModeFlags = g_SnoboStatePtr->modeFlags;
    g_SnoboStatePtr->modeFlags = 0;
    g_SnoboStatePtr->inputFlags = 0;
    Mini_SnowboardEmitCameraFrame();
    g_SnoboStatePtr->inputFlags = g_SnoboMenuSavedInputFlags;
    g_SnoboStatePtr->modeFlags = g_SnoboMenuSavedModeFlags;
    g_pSnoboRenderer->selectMenu(*(short *)&g_SnoboMenuSelection);
    g_pSnoboRenderer->renderStateMenu(g_SnoboResultClass);
    if (g_SnoboStatePtr->activeFlags != 0) {
        g_pSnoboRenderer->renderRaceState(g_SnoboStatePtr->triggerStage - 1, g_SnoboCourseTimeRank, g_SnoboFadeLevel);
    } else {
        g_pSnoboRenderer->showMenu(Gfx_GetPsxScratchpadPtr(0), g_SnoboFadeLevel);
    }
    if ((g_SnoboStatePtr->modeFlags & (INPUT_START | INPUT_CROSS | INPUT_CIRCLE)) != 0) {
        g_SnoboFadeLevel = 0;
        if (g_SnoboMenuSelection != 0) {
            nextState = Mini_SnowboardStateQuit;
        } else {
            nextState = Mini_SnowboardStateFadeOut;
        }
        g_SnoboStateHandler = nextState;
    }
}

/*
 * Starts the sound fade at counter zero and updates fades thereafter,
 * increasing visual fade by eight. Before 0x100 clears input and an update
 * flag and runs the scene helper.
 */
// FUNCTION: FF7 0x007240d6
void __cdecl Mini_SnowboardStateFadeOut(void)
{
    if (g_SnoboFadeLevel == 0) {
        Mini_SnowboardBeginSoundFade();
    } else {
        SoundUpdateFades();
    }
    g_SnoboFadeLevel += 8;
    if (g_SnoboFadeLevel < 0x100) {
        Mini_SnowboardRendererFadeWhite(g_SnoboFadeLevel);
        g_SnoboStatePtr->modeFlags = 0;
        g_SnoboStatePtr->inputFlags = 0;
        g_SnoboStatePtr->running = 0;
        Mini_SnowboardEmitRiderFrame();
    } else if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
        if (g_SnoboRunFinished != 0 && g_SnoboStatePtr->activeFlags != 0 && g_SnoboCourseRecords.runs[0].frameIndex < 0x2220 && (unsigned int)g_SnoboStatePtr->courseTime < (unsigned int)g_SnoboCourseRecords.runs[g_SnoboStatePtr->triggerStage].courseTime) {
            Mini_SnowboardStoreRunSamplesToSlot();
        }
        Mini_SnowboardRendererFillColor(0xFFFFFF);
        g_SnoboStateHandler = Mini_SnowboardStateRestart;
    }
}

/*
 * Calls Mini_SnowboardStartSong and
 * Mini_SnowboardInitRunAtCourseEntry(0,0,0x10), sets
 * Mini_SnowboardRendererFillColor(0xFFFFFF) and selects
 * Mini_SnowboardStateIntro.
 */
// FUNCTION: FF7 0x007241b6
void __cdecl Mini_SnowboardStateRestart(void)
{
    Mini_SnowboardStartSong();
    Mini_SnowboardInitRunAtCourseEntry(0, 0, 0x10);
    Mini_SnowboardRendererFillColor(0xFFFFFF);
    g_SnoboStateHandler = Mini_SnowboardStateIntro;
}

/*
 * Quit state of the snowboard minigame, driving a fade. On the first tick,
 * when g_SnoboFadeLevel is zero, it calls Mini_SnowboardBeginSoundFade.
 */
// FUNCTION: FF7 0x007241e5
void __cdecl Mini_SnowboardStateQuit(void)
{
    if (g_SnoboFadeLevel == 0) {
        Mini_SnowboardBeginSoundFade();
    }
    g_SnoboFadeLevel += 8;
    if (g_SnoboFadeLevel < 0x100) {
        Mini_SnowboardRendererFadeBlack(g_SnoboFadeLevel);
        g_SnoboStatePtr->modeFlags = 0;
        g_SnoboStatePtr->inputFlags = 0;
        g_SnoboStatePtr->running = 0;
        Mini_SnowboardEmitRiderFrame();
    } else {
        Mini_SnowboardRendererFillColor(0);
        g_SnoboActive = 0;
    }
}

/*
 * Updates snowboard race simulation, derives its active-rider flag, runs
 * camera/animation/rider updates and maintains a short countdown. In normal
 * mode, records/checks race state and on the course-end flag stores result
 * state, switches to finish and replaces MIDI with song 0x2E.
 */
// FUNCTION: FF7 0x00724266
void __cdecl Mini_SnowboardStateRace(void)
{
    SnowboardVec3 direction;

    Mini_SnowboardSaveRiderSnapshot();
    g_SnoboStatePtr->running = g_SnoboStatePtr->paused == 0;
    if (g_SnoboStatePtr->running != 0) {
        Mini_SnowboardUpdateCourseSurface();
    }
    Mini_SnowboardWriteRelativePosition((struct SnowboardRelativePosition *)&direction);
    Mini_SnowboardEmitCourseChain(1, g_SnoboStatePtr->segmentPtr->chainIndexTable[g_SnoboStatePtr->chainIndex] + 1);
    if (g_SnoboStatePtr->running != 0) {
        Mini_SnowboardApproachTargetAngles();
    }
    Mini_SnowboardRunActionFrame((int *)&direction, Gfx_GetPsxScratchpadPtr(0), g_SnoboStatePtr->running, 1);
    if (g_SnoboStatePtr->running != 0) {
        if ((g_SnoboStatePtr->timer)-- < 0 && g_SnoboStatePtr->motionStage < 4) {
            g_SnoboStatePtr->timer = 8;
        }
        if ((g_SnoboStatePtr->activeFlags & g_SnoboActiveFlagMask) == 0) {
            ((SnowboardRaceSpriteRenderer *)g_SnoboStatePtr->courseLoader)->draw((int)&g_SnoboStatePtr->motionFlags, g_SnoboStatePtr->running);
        }
    }
    if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
        if (g_SnoboStatePtr->activeFlags != 0) {
            Mini_SnowboardUpdateMidiTempo();
            Mini_SnowboardAppendRiderSample();
            Mini_SnowboardEmitAnimFrameActions();
        }
        if ((g_SnoboStatePtr->segmentPtr->chainBitTable[g_SnoboStatePtr->chainIndex] & 0x80) != 0) {
            g_SnoboStatePtr->triggerLatched = 0;
            g_SnoboFadeLevel = 0;
            g_SnoboAltEndTimer = g_SnoboStatePtr->activeFlags != 0 && g_SnoboStatePtr->stageTimes[g_SnoboStatePtr->triggerStage] != 0 && (unsigned int)g_SnoboStatePtr->stageTimes[g_SnoboStatePtr->triggerStage] > (unsigned int)g_SnoboStatePtr->courseTime;
            Mini_SnowboardMeasureCountDelta();
            g_SnoboFinishCode = 0x10;
            g_SnoboRunFinished = 1;
            g_SnoboStateHandler = Mini_SnowboardStateFinish;
            if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
                g_SnoboStatePtr->soundCommand->command = SOUND_SFX_CH1_VOLUME;
                g_SnoboStatePtr->soundCommand->volume = 0;
                SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, 0, 0, 0, 0, 0, 0, 0);
                MidiStop();
                MidiPlaySong(0x2E, 0, 0x7F);
            }
        }
    } else if ((g_SnoboStatePtr->segmentPtr->chainBitTable[g_SnoboStatePtr->chainIndex] & 0x80) != 0) {
        Mini_SnowboardUpdateRiderPosition((struct SnowboardRiderPose *)(Gfx_GetPsxScratchpadPtr(0)));
        g_SnoboStateHandler = Mini_SnowboardStateAltEndWait;
    }
}

/*
 * Resolves a course segment and six-phase vertex sample, builds its angle
 * matrix and transforms the selected short XYZ sample with Z minus 0x80. Adds
 * the segment translation and stores the resulting three position fields in
 * the snowboard state.
 */
// FUNCTION: FF7 0x00724548
void __cdecl Mini_SnowboardUpdateRiderPosition(SnowboardRiderPose *pose)
{
    SnowboardEffectPos *effectPtr;
    SnowboardCourseBlock *base;
    SnowboardTriangleVertex *sampleAddr;
    short phase;
    short block;

    block = g_SnoboStatePtr->blockTable[g_SnoboStatePtr->segment];
    Matrix30SetRotationY((short)g_SnoboStatePtr->rotationY, &pose->matrix);
    phase = g_SnoboRiderPoseVertex[(g_SnoboStatePtr->triangle >> 1) % 6];
    base = &g_SnoboStatePtr->courseHeader[g_SnoboStatePtr->blockIndexTable[block]];
    sampleAddr = &base->vertices[phase];
    pose->posX = sampleAddr->posX;
    pose->posY = sampleAddr->posY;
    pose->posZ = sampleAddr->posZ - 0x80;
    PsxApplyMatrix33ToLongVector((struct SnoboMatrix *)&pose->matrix, (struct SnoboVector *)&pose->posX, &pose->offsetX);
    effectPtr = (SnowboardEffectPos *)(g_SnoboStatePtr->effectPosTable + block * 0xC);
    g_SnoboStatePtr->originX = pose->offsetX + effectPtr->posX;
    g_SnoboStatePtr->originY = pose->offsetY + effectPtr->posY;
    g_SnoboStatePtr->originZ = pose->offsetZ + effectPtr->posZ;
}

/*
 * Clears input and updates the alternate end sequence. Returns until the
 * current segment matches the target, then changes animation codes 1/0x11/0x12
 * to three after frame 0x3C.
 */
// FUNCTION: FF7 0x00724695
void __cdecl Mini_SnowboardStateAltEndWait(void)
{
    g_SnoboStatePtr->modeFlags = 0;
    g_SnoboStatePtr->inputFlags = 0;
    Mini_SnowboardUpdateTerrainMotion(1, 1);
    if (g_SnoboStatePtr->section != g_SnoboStatePtr->blockTable[g_SnoboStatePtr->segment])
        return;
    if (g_SnoboStatePtr->triangle >= 0x3C) {
        switch ((char)g_SnoboStatePtr->riderState) {
        case SNOWBOARD_RIDER_ACTION_CONTROLS:
        case SNOWBOARD_RIDER_ACTION_TURN_LEFT:
        case SNOWBOARD_RIDER_ACTION_TURN_RIGHT:
            g_SnoboStatePtr->riderState = 3;
            break;
        }
    }
    if (g_SnoboCourseEndReached != 0) {
        g_SnoboAltEndTimer = 0;
        g_SnoboFadeLevel = g_SnoboAltEndTimer;
        g_SnoboStateHandler = Mini_SnowboardStateAltEndFadeUp;
    }
}

/*
 * Decreases the state's fadeLevel by 3072 and supplies synthetic input bit
 * 0x40 for animation codes three through six, otherwise clears input. Updates
 * the alternate sequence and increments its counter; after 0x3C increases fade
 * by two until 0xFC then changes callback.
 */
// FUNCTION: FF7 0x00724764
void __cdecl Mini_SnowboardStateAltEndFadeUp(void)
{
    g_SnoboStatePtr->fadeLevel -= 3072.0f;
    if ((char)g_SnoboStatePtr->riderState >= 3 && (char)g_SnoboStatePtr->riderState <= 6) {
        g_SnoboStatePtr->inputFlags |= INPUT_CROSS;
        g_SnoboStatePtr->modeFlags = 0;
    } else {
        g_SnoboStatePtr->modeFlags = 0;
        g_SnoboStatePtr->inputFlags = 0;
    }
    Mini_SnowboardUpdateTerrainMotion(1, 1);
    g_SnoboFadeLevel++;
    if (g_SnoboFadeLevel > 0x3C) {
        g_SnoboAltEndTimer += 2;
        Mini_SnowboardRendererFadeWhite(g_SnoboAltEndTimer);
        if (g_SnoboAltEndTimer >= 0xFC) {
            g_SnoboStateHandler = Mini_SnowboardStateAltEndFadeDown;
        }
    }
}

/*
 * Fade step counting down: subtracts 4 from g_SnoboAltEndTimer and passes
 * its low byte replicated into all three colour channels, (p & 0xFF) << 16 |
 * (p & 0xFF) << 8 | (p & 0xFF), to Mini_SnowboardRendererFillColor. When
 * g_SnoboAltEndTimer reaches zero or below it clears g_SnoboActive.
 */
// FUNCTION: FF7 0x0072484b
void __cdecl Mini_SnowboardStateAltEndFadeDown(void)
{
    int val;

    g_SnoboAltEndTimer -= 4;
    val = g_SnoboAltEndTimer;
    Mini_SnowboardRendererFillColor((val & 0xFF) << 0x10 | (val & 0xFF) << 8 | val & 0xFF);
    if (g_SnoboAltEndTimer <= 0) {
        g_SnoboActive = 0;
    }
}

/*
 * Runs the snowboard terrain and motion update, setting the active flag and
 * optionally computing triangle normal and velocity changes. When the special
 * mode is clear, updates local position and course index.
 */
// FUNCTION: FF7 0x007248a9
void __cdecl Mini_SnowboardUpdateTerrainMotion(int integrate, int emitChain)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[44];
        SnowboardTriangleVertex *prevCourseBlock;
        struct { unsigned char pad_to_4[0x4]; int vertexIndex; };
        struct { unsigned char pad_to_8[0x8]; int triCorner; };
        struct { unsigned char pad_to_C[0xC]; int triVertex; };
        struct { unsigned char pad_to_E[0xE]; short vertexDelta; };
        struct {
            unsigned char pad[16];
            int value;
        } at28;
        struct { unsigned char pad_to_14[0x14]; int triPointA; };
        struct { unsigned char pad_to_1C[0x1C]; int triPointB; };
        struct { unsigned char pad_to_24[0x24]; SnowboardCourseBlock *courseBlock; };
        struct { unsigned char pad_to_28[0x28]; SnowboardTriangleVertex *refBlock; };
    } frame;
#pragma pack(pop)

    Mini_SnowboardSaveRiderSnapshot();
    g_SnoboStatePtr->running = 1;
    if (integrate != 0) {
        Mini_SnowboardCourseTriangleNormal(g_SnoboStatePtr->chainIndex, g_SnoboStatePtr->triangle, &g_SnoboStatePtr->courseNormal, (SnowboardNormalScratch *)Gfx_GetPsxScratchpadPtr(0));
        Mini_SnowboardIntegrateHorizontalMotion((struct SnowboardMotionState *)&g_SnoboStatePtr->motionFlags, Gfx_GetPsxScratchpadPtr(0));
    }
    if (g_SnoboCourseEndReached == 0) {
        Mini_SnowboardProbeFloatPositions(&g_SnoboStatePtr->relPosX, &g_SnoboStatePtr->posX);
        g_SnoboStatePtr->section = g_SnoboStatePtr->chainIndex + g_SnoboStatePtr->sectionTable[g_SnoboStatePtr->segment];
    }
    frame.courseBlock = &g_SnoboStatePtr->courseHeader[g_SnoboStatePtr->segmentPtr->blockIndexTable[g_SnoboStatePtr->chainIndex]];
    /* Retail caller passes 1 arg(s) to 0-parameter `Mini_SnowboardSetCameraFromDirection` (__cdecl stack cleanup matches retail). */
    ((void(__cdecl *)(int))Mini_SnowboardSetCameraFromDirection)(Gfx_GetPsxScratchpadPtr(0));
    if (g_SnoboStatePtr->chainIndex > 0) {
        frame.prevCourseBlock = &g_SnoboStatePtr->courseHeader[g_SnoboStatePtr->segmentPtr->blockIndexTable[g_SnoboStatePtr->chainIndex + -1]].vertices[42];
    } else {
        frame.prevCourseBlock = frame.courseBlock->vertices;
    }
    frame.refBlock = frame.prevCourseBlock;
    for (frame.triCorner = 0; frame.triCorner < 3; frame.triCorner++) {
        frame.vertexIndex = frame.courseBlock->faces[g_SnoboStatePtr->triangle].vertexIndex[frame.triCorner];
        *(Int64Pair *)(((char *)&frame.triVertex + frame.triCorner * 8)) = *(Int64Pair *)&frame.courseBlock->vertices[frame.vertexIndex];
        if (frame.vertexIndex < 7) {
            *(short *)(((char *)&frame.vertexDelta + frame.triCorner * 8)) = frame.refBlock[frame.vertexIndex].posY - frame.refBlock->posY;
        }
    }
    if (g_SnoboCourseEndReached != 0) {
        g_SnoboStatePtr->terrainHeight = 2147483648.0f;
    } else {
        g_SnoboStatePtr->terrainHeight = (float)(g_SnoboStatePtr->segmentPtr->vectors[g_SnoboStatePtr->chainIndex].y + Mini_SnowboardInterpolateTriangleHeight((SnowboardHeightPoint *)&g_SnoboStatePtr->probeDeltaX, (struct SnowboardTriangleVertex *)&frame.triVertex, (struct SnowboardTriangleVertex *)&frame.triPointA, (struct SnowboardTriangleVertex *)&frame.triPointB)) * 4096.0f;
    }
    if (integrate != 0) {
        Mini_SnowboardIntegrateVerticalMotion(&g_SnoboStatePtr->motionFlags);
    }
    if (g_SnoboStatePtr->motionStage > 8) {
        g_SnoboStatePtr->targetAngle1 = 0.0f;
        g_SnoboStatePtr->targetAngle0 = 0.0f;
    } else {
        Mini_SnowboardDirectionToAngles(&g_SnoboStatePtr->courseNormal.normalX, &g_SnoboStatePtr->targetAngle0);
    }
    Mini_SnowboardWriteRelativePosition(&g_SnoboRiderRelativePos);
    if (emitChain != 0) {
        Mini_SnowboardEmitCourseChain(2 - (g_SnoboStatePtr->blockTable[g_SnoboStatePtr->segment] - g_SnoboStatePtr->section) + 5, 8);
    }
    Mini_SnowboardApproachTargetAngles();
    Mini_SnowboardRunActionFrame((int *)&g_SnoboRiderRelativePos, Gfx_GetPsxScratchpadPtr(0), g_SnoboStatePtr->running, 0);
    Mini_SnowboardSyncSfxOnTableBit();
}

/*
 * Maps selected finish animation codes to 0x1C or zero, optionally runs the
 * special-result helper, clears input and updates the finish scene. Increments
 * the counter and, once the low animation byte becomes zero, clears
 * g_SnoboCameraStepCount, runs the transition helper and switches to end dispatch.
 */
// FUNCTION: FF7 0x00724c0f
void __cdecl Mini_SnowboardStateFinish(void)
{
    switch ((char)g_SnoboStatePtr->riderState) {
    case SNOWBOARD_RIDER_ACTION_CONTROLS:
    case SNOWBOARD_RIDER_ACTION_STEER_PUSH:
    case SNOWBOARD_RIDER_ACTION_TURN_LEFT:
    case SNOWBOARD_RIDER_ACTION_TURN_RIGHT:
    case SNOWBOARD_RIDER_ACTION_BRAKE:
        g_SnoboStatePtr->riderState = SNOWBOARD_RIDER_ACTION_TURN_TO_TARGET;
        break;
    case SNOWBOARD_RIDER_ACTION_STOP_13:
    case SNOWBOARD_RIDER_ACTION_STOP_14:
    case SNOWBOARD_RIDER_ACTION_STOP_15:
    case SNOWBOARD_RIDER_ACTION_STOP_19:
        g_SnoboStatePtr->riderState = 0;
    }
    if (g_SnoboAltEndTimer != 0) {
        g_pSnoboRenderer->family1();
    }
    g_SnoboStatePtr->modeFlags = 0;
    g_SnoboStatePtr->inputFlags = 0;
    Mini_SnowboardEmitRiderFrame();
    g_SnoboFadeLevel++;
    if (((unsigned short)g_SnoboStatePtr->riderState & 0xFF) == 0) {
        g_SnoboCameraStepCount = 0;
        Mini_SnowboardComputeResultScores();
        g_SnoboStateHandler = Mini_SnowboardStateEndDispatch;
    }
}

/*
 * Optionally updates the special result, clears input and updates the end
 * scene. After the old postincremented counter exceeds 0xF0, resets it and
 * either inserts the race time into rankings or runs the non-race results
 * helper, then chooses the corresponding end-hold callback.
 */
// FUNCTION: FF7 0x00724d0f
void __cdecl Mini_SnowboardStateEndDispatch(void)
{
    SnoboStateFn nextState;

    if (g_SnoboAltEndTimer != 0) {
        g_pSnoboRenderer->family1();
    }
    g_SnoboStatePtr->modeFlags = 0;
    g_SnoboStatePtr->inputFlags = 0;
    Mini_SnowboardEmitCameraFrame();
    if (g_SnoboFadeLevel++ > 0xF0) {
        g_SnoboFadeLevel = 0;
        if (g_SnoboStatePtr->activeFlags != 0) {
            g_SnoboCourseTimeRank = Mini_SnowboardInsertCourseTimeRank();
        } else {
            Mini_SnowboardUpdateCourseBestTimeAndScore();
        }
        if (g_SnoboStatePtr->activeFlags != 0) {
            nextState = Mini_SnowboardStateEndHold1;
        } else {
            nextState = Mini_SnowboardStateEndHold0;
        }
        g_SnoboStateHandler = nextState;
    }
}

/*
 * Handles snowboard pause/menu input. In the event mode, input 0x800 toggles
 * the pause short and dispatches SOUND_PAUSE_ALL or SOUND_RESUME_ALL every
 * call. In the Gold Saucer mode it opens the pause menu and runs its pages
 * (enum SnowboardPauseMenuState).
 */
// FUNCTION: FF7 0x00724dbf
void __cdecl Mini_SnowboardRacePauseMenu(void)
{
    if (g_SnoboModeFlag == SNOWBOARD_MODE_EVENT) {
        if ((g_SnoboStatePtr->modeFlags & INPUT_START) != 0) {
            g_SnoboStatePtr->paused ^= 1;
        }
        if (g_SnoboStatePtr->paused != 0) {
            g_SnoboStatePtr->soundCommand->command = SOUND_PAUSE_ALL;
            SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, 0, 0, 0, 0, 0, 0, 0, 0);
        } else {
            g_SnoboStatePtr->soundCommand->command = SOUND_RESUME_ALL;
            SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, 0, 0, 0, 0, 0, 0, 0, 0);
        }
    } else if (g_SnoboStatePtr->paused == 0) {
        if ((g_SnoboStatePtr->modeFlags & INPUT_START) != 0) {
            g_SnoboStatePtr->paused = 1;
            MidiPause();
            if (g_SnoboStatePtr->activeFlags != 0) {
                g_SnoboPauseMenuState = SNOWBOARD_PAUSE_MENU_MAIN_WITH_OPTIONS;
                g_SnoboPauseMenuCursor = 1;
            } else {
                g_SnoboPauseMenuState = SNOWBOARD_PAUSE_MENU_MAIN;
                g_SnoboPauseMenuCursor = 0;
            }
            g_SnoboStatePtr->soundCommand->command = SOUND_PAUSE_ALL;
            SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, 0, 0, 0, 0, 0, 0, 0, 0);
        }
    } else {
        if ((g_SnoboStatePtr->modeFlags & INPUT_UP) != 0) {
            g_SnoboPauseMenuCursor--;
        }
        if ((g_SnoboStatePtr->modeFlags & INPUT_DOWN) != 0) {
            g_SnoboPauseMenuCursor++;
        }
        switch (g_SnoboPauseMenuState) {
        case SNOWBOARD_PAUSE_MENU_MAIN:
            if (g_SnoboPauseMenuCursor < 0) {
                g_SnoboPauseMenuCursor = 0;
            } else if (g_SnoboPauseMenuCursor > 2) {
                g_SnoboPauseMenuCursor = 2;
            }
            g_pSnoboRenderer->select0((short)g_SnoboPauseMenuCursor);
            break;
        case SNOWBOARD_PAUSE_MENU_MAIN_WITH_OPTIONS:
            if (g_SnoboPauseMenuCursor < 0) {
                g_SnoboPauseMenuCursor = 0;
            } else if (g_SnoboPauseMenuCursor > 3) {
                g_SnoboPauseMenuCursor = 3;
            }
            g_pSnoboRenderer->select1((short)g_SnoboPauseMenuCursor);
            break;
        case SNOWBOARD_PAUSE_MENU_OPTIONS:
            if (g_SnoboPauseMenuCursor < 0) {
                g_SnoboPauseMenuCursor = 0;
            } else if (g_SnoboPauseMenuCursor > 1) {
                g_SnoboPauseMenuCursor = 1;
            }
            g_pSnoboRenderer->select2((short)g_SnoboPauseMenuCursor);
            break;
        case SNOWBOARD_PAUSE_MENU_REPLAY_STYLE:
            if (g_SnoboPauseMenuCursor <= 0) {
                g_SnoboPauseMenuCursor = 0;
                g_SnoboCourseRecords.animFrameVariantWord = 0;
            } else if (g_SnoboPauseMenuCursor == 1) {
                g_SnoboCourseRecords.animFrameVariantWord = 1;
            } else if (g_SnoboPauseMenuCursor == 2) {
                g_SnoboCourseRecords.animFrameVariantWord = 2;
            } else if (g_SnoboPauseMenuCursor >= 3) {
                g_SnoboPauseMenuCursor = 3;
                g_SnoboCourseRecords.animFrameVariantWord = 3;
            }
            g_pSnoboRenderer->select3((short)g_SnoboPauseMenuCursor);
            break;
        case SNOWBOARD_PAUSE_MENU_ON_OFF:
            if (g_SnoboPauseMenuCursor <= 0) {
                g_SnoboPauseMenuCursor = 0;
                g_SnoboActiveFlagMask = 0;
            } else if (g_SnoboPauseMenuCursor >= 1) {
                g_SnoboPauseMenuCursor = 1;
                g_SnoboActiveFlagMask = 1;
            }
            g_pSnoboRenderer->select4((short)g_SnoboPauseMenuCursor);
        }
        if ((g_SnoboStatePtr->modeFlags & (INPUT_START | INPUT_CROSS | INPUT_CIRCLE)) != 0) {
            switch (g_SnoboPauseMenuState) {
            case SNOWBOARD_PAUSE_MENU_MAIN_WITH_OPTIONS:
                if (g_SnoboPauseMenuCursor == SNOWBOARD_PAUSE_ROW_OPTIONS) {
                    g_SnoboPauseMenuState = SNOWBOARD_PAUSE_MENU_OPTIONS;
                    break;
                }
                g_SnoboPauseMenuCursor--;
            case SNOWBOARD_PAUSE_MENU_MAIN:
                switch (g_SnoboPauseMenuCursor) {
                case SNOWBOARD_PAUSE_ROW_RESUME:
                    g_SnoboStatePtr->paused = 0;
                    break;
                case SNOWBOARD_PAUSE_ROW_RETRY:
                    g_SnoboStateHandler = Mini_SnowboardStateFadeOut;
                    MidiStop();
                    g_SnoboStatePtr->paused = 0;
                    break;
                case SNOWBOARD_PAUSE_ROW_QUIT:
                    g_SnoboStateHandler = Mini_SnowboardStateQuit;
                    MidiStop();
                    g_SnoboStatePtr->paused = 0;
                }
                break;
            case SNOWBOARD_PAUSE_MENU_OPTIONS:
                switch (g_SnoboPauseMenuCursor) {
                case SNOWBOARD_PAUSE_OPTIONS_ROW_REPLAY_STYLE:
                    g_SnoboPauseMenuState = SNOWBOARD_PAUSE_MENU_REPLAY_STYLE;
                    g_SnoboPauseMenuCursor = g_SnoboCourseRecords.animFrameVariantWord;
                    break;
                case SNOWBOARD_PAUSE_OPTIONS_ROW_ON_OFF:
                    g_SnoboPauseMenuState = SNOWBOARD_PAUSE_MENU_ON_OFF;
                    g_SnoboPauseMenuCursor = g_SnoboActiveFlagMask;
                }
                break;
            case SNOWBOARD_PAUSE_MENU_REPLAY_STYLE:
                switch (g_SnoboPauseMenuCursor) {
                case SNOWBOARD_REPLAY_OFF:
                    g_SnoboCourseRecords.animFrameVariantWord = 0;
                    break;
                case SNOWBOARD_REPLAY_COURSE_A:
                    g_SnoboCourseRecords.animFrameVariantWord = 1;
                    break;
                case SNOWBOARD_REPLAY_COURSE_B:
                    g_SnoboCourseRecords.animFrameVariantWord = 2;
                    break;
                case SNOWBOARD_REPLAY_COURSE_C:
                    g_SnoboCourseRecords.animFrameVariantWord = 3;
                }
                g_SnoboStatePtr->paused = 0;
                break;
            case SNOWBOARD_PAUSE_MENU_ON_OFF:
                switch (g_SnoboPauseMenuCursor) {
                case SNOWBOARD_TOGGLE_OFF:
                    g_SnoboActiveFlagMask = 0;
                    break;
                case SNOWBOARD_TOGGLE_ON:
                    g_SnoboActiveFlagMask = 1;
                }
                g_SnoboStatePtr->paused = 0;
            }
        }
        if (g_SnoboStatePtr->paused == 0 && g_SnoboStateHandler != Mini_SnowboardStateFadeOut && g_SnoboStateHandler != Mini_SnowboardStateQuit) {
            g_SnoboStatePtr->soundCommand->command = SOUND_RESUME_ALL;
            SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, 0, 0, 0, 0, 0, 0, 0, 0);
            if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
                MidiResume();
            }
        }
    }
}

/*
 * Loads the fourteen sound effects whose ids sit in the int array at
 * g_SnoboSfxIds, iterating downwards from index 0xD and calling
 * SfxLoadIntoBuffer(id, 0), then sets the state's soundCommand
 * to point at the block just after the state.
 */
// FUNCTION: FF7 0x00725378
void __cdecl Mini_SnowboardPreloadSfx(void)
{
    int i;

    for (i = 0xD; i >= 0; i--) {
        SfxLoadIntoBuffer(g_SnoboSfxIds[i], 0);
    }
    g_SnoboStatePtr->soundCommand = (SnowboardSoundCommand *)(g_SnoboStatePtr + 1);
}

/*
 * Undo of Mini_SnowboardPreloadSfx: releases the same fourteen sound effects
 * from g_SnoboSfxIds, index 0xD down to 0, with
 * Sfx_ReleaseCachedBuffer(id, 0xF).
 */
// FUNCTION: FF7 0x007253c5
void __cdecl Mini_SnowboardReleaseSfx(void)
{
    int i;

    for (i = 0xD; i >= 0; i--) {
        Sfx_ReleaseCachedBuffer(g_SnoboSfxIds[i], 0xF);
    }
}

/*
 * Assembles three little-endian dwords and three trailing bytes from the saved
 * snowboard data into current state. Initializes each five-entry ranking list
 * with that saved dword as its first entry and four zero entries.
 */
// FUNCTION: FF7 0x007253fc
void __cdecl Mini_SnowboardLoadContextColorsAndClearSlots(void)
{
    SnowboardColorEntry *colorEntriesPtr;
    union {
        unsigned int value;
        struct {
            unsigned int b : 8, g : 8, r : 8, a : 8;
        } bits;
    } colorWord;
    int i;
    int j;

    colorEntriesPtr = (SnowboardColorEntry *)gSavemap.snowBegFastTime;
    for (i = 0; i < 3; i++) {
        colorWord.bits.b = colorEntriesPtr[i].blue;
        colorWord.bits.g = colorEntriesPtr[i].green;
        colorWord.bits.r = colorEntriesPtr[i].red;
        colorWord.bits.a = colorEntriesPtr[i].alpha;
        g_SnoboStatePtr->bestTimes[i] = colorWord.value;
        g_SnoboStatePtr->bestScores[i] = *((char *)colorEntriesPtr + i + 0xC);
    }
    for (i = 0; i < 3; i++) {
        *(DirectWord *)&g_SnoboStatePtr->rankings[i][0] = *(DirectWord *)&g_SnoboStatePtr->bestTimes[i];
        for (j = 1; j < 5; j++) {
            g_SnoboStatePtr->rankings[i][j] = 0;
        }
    }
}

// Stores the three dwords of the state's bestTimes as twelve little-endian bytes in
// gSavemap.snowBegFastTime. It then copies the three bytes of bestScores into the savemap bytes
// after them.
// FUNCTION: FF7 0x0072553c
void __cdecl Mini_SnowboardStoreBestRecordToSavemap(void)
{
    SnowboardRecordBytes *size;
    int index;
    unsigned int dword;

    size = (SnowboardRecordBytes *)gSavemap.snowBegFastTime;
    for (index = 0; index < 3; index++) {
        dword = g_SnoboStatePtr->bestTimes[index];
        size[index].loByte = dword & 0xFF;
        size[index].midLoByte = dword >> 8 & 0xFF;
        size[index].midHiByte = dword >> 0x10 & 0xFF;
        size[index].hiByte = dword >> 0x18 & 0xFF;
        *((char *)size + index + 0xC) = g_SnoboStatePtr->bestScores[index];
    }
}

/*
 * Updates two per-stage records in g_SnoboStatePtr at the index p
 * taken from triggerStage minus one. If bestTimes[p] is zero or
 * unsigned-greater than courseTime it is replaced by that value, so the array
 * keeps the smallest; if bestScores[p] is less than resultTotal it is raised
 * to it, so that array keeps the largest.
 */
// FUNCTION: FF7 0x007255e3
void __cdecl Mini_SnowboardUpdateCourseBestTimeAndScore(void)
{
    int val;

    val = g_SnoboStatePtr->triggerStage - 1;
    if (g_SnoboStatePtr->bestTimes[val] == 0 || (unsigned int)g_SnoboStatePtr->bestTimes[val] > (unsigned int)g_SnoboStatePtr->courseTime) {
        g_SnoboStatePtr->bestTimes[val] = g_SnoboStatePtr->courseTime;
    }
    if (g_SnoboStatePtr->bestScores[val] < g_SnoboStatePtr->resultTotal) {
        g_SnoboStatePtr->bestScores[val] = *(char *)&g_SnoboStatePtr->resultTotal;
    }
}

/*
 * Inserts courseTime into the current course's five-entry rankings (ascending, zero for an empty
 * slot), shifting the slower times down; a new best also goes into stageTimes[triggerStage].
 * Returns the rank, 5 when the time did not place.
 */
// FUNCTION: FF7 0x00725665
int __cdecl Mini_SnowboardInsertCourseTimeRank(void)
{
    struct Local {
        int r;
        int t;
        int *p;
        int s;
        int q;
    } x;

    x.p = g_SnoboStatePtr->rankings[g_SnoboStatePtr->triggerStage - 1];
    x.q = g_SnoboStatePtr->courseTime;
    for (x.r = 0; x.r < 5; x.r++) {
        x.s = x.p[x.r];
        if (x.s == 0 || (unsigned int)x.q < (unsigned int)x.s) {
            break;
        }
    }
    if (x.r < 5) {
        for (x.t = 3; x.t >= x.r; x.t--) {
            ((struct DirectWord *)x.p)[x.t + 1] = ((struct DirectWord *)x.p)[x.t];
        }
        x.p[x.r] = x.q;
        if (x.r == 0) {
            g_SnoboStatePtr->stageTimes[g_SnoboStatePtr->triggerStage] = x.q;
        }
    }
    return x.r;
}

/*
 * Saves state inside g_SnoboStatePtr: copies the 12 bytes of relPosX..riderPos
 * to posX..posZ and chainIndex, section and triangle to snapshotIndex,
 * snapshotSection and snapshotTriangle.
 */
// FUNCTION: FF7 0x0072572d
void __cdecl Mini_SnowboardSaveRiderSnapshot(void)
{
    *(SnowboardVec3Copy *)&g_SnoboStatePtr->posX = *(SnowboardVec3Copy *)&g_SnoboStatePtr->relPosX;
    g_SnoboStatePtr->snapshotIndex = g_SnoboStatePtr->chainIndex;
    g_SnoboStatePtr->snapshotSection = g_SnoboStatePtr->section;
    g_SnoboStatePtr->snapshotTriangle = g_SnoboStatePtr->triangle;
}

/*
 * The reverse of Mini_SnowboardSaveRiderSnapshot: copies the 12 bytes of
 * posX..posZ back to relPosX..riderPos and snapshotIndex, snapshotSection and
 * snapshotTriangle back to chainIndex, section and triangle.
 */
// FUNCTION: FF7 0x007257a6
void __cdecl Mini_SnowboardRestoreRiderSnapshot(void)
{
    *(SnowboardVec3Copy *)&g_SnoboStatePtr->relPosX = *(SnowboardVec3Copy *)&g_SnoboStatePtr->posX;
    g_SnoboStatePtr->chainIndex = g_SnoboStatePtr->snapshotIndex;
    g_SnoboStatePtr->section = g_SnoboStatePtr->snapshotSection;
    g_SnoboStatePtr->triangle = g_SnoboStatePtr->snapshotTriangle;
}

/*
 * Copies the origin of the current node (segmentPtr->vectors[chainIndex]) into originX, originY and
 * originZ.
 */
// FUNCTION: FF7 0x0072581f
void __cdecl Mini_SnowboardLoadSegmentVector(void)
{
    SnowboardVec *rec;

    rec = &g_SnoboStatePtr->segmentPtr->vectors[g_SnoboStatePtr->chainIndex];
    g_SnoboStatePtr->originX = rec->x;
    g_SnoboStatePtr->originY = rec->y;
    g_SnoboStatePtr->originZ = rec->z;
}

/*
 * Moves two float orientation fields toward their respective target fields
 * using quantized signed differences scaled by 651.8986206. Uses one-eighth
 * and one-quarter steps with minimum magnitude one and converts each step back
 * with 0.0015339808; zero quantized difference leaves its field unchanged.
 */
// FUNCTION: FF7 0x0072587f
void __cdecl Mini_SnowboardApproachTargetAngles(void)
{
    int stepAmt;
    int quantDiff;

    quantDiff = (int)((g_SnoboStatePtr->targetAngle1 - g_SnoboStatePtr->currentAngle1) * 651.8986206f);
    if (quantDiff != 0) {
        stepAmt = abs(quantDiff) >> 3;
        if (stepAmt == 0) {
            stepAmt = 1;
        }
        if (quantDiff < 0) {
            stepAmt = -stepAmt;
        }
        g_SnoboStatePtr->currentAngle1 = (float)stepAmt * 0.0015339808f + g_SnoboStatePtr->currentAngle1;
    }
    quantDiff = (int)((g_SnoboStatePtr->targetAngle0 - g_SnoboStatePtr->currentAngle0) * 651.8986206f);
    if (quantDiff != 0) {
        stepAmt = abs(quantDiff) >> 2;
        if (stepAmt == 0) {
            stepAmt = 1;
        }
        if (quantDiff < 0) {
            stepAmt = -stepAmt;
        }
        g_SnoboStatePtr->currentAngle0 = (float)stepAmt * 0.0015339808f + g_SnoboStatePtr->currentAngle0;
    }
}

/*
 * Frees the four buffers held in g_SnoboCourseRecords.runs[0].frames,
 * g_SnoboCourseRecords.runs[1].frames, g_SnoboCourseRecords.runs[2].frames and
 * g_SnoboCourseRecords.runs[3].frames by passing each to operator delete, then clears
 * only g_SnoboCourseRecords.runs[0].frames, which is the flag
 * Mini_SnowboardInitRiderSampleBuffers tests before reallocating.
 */
// FUNCTION: FF7 0x00725971
void __cdecl Mini_SnowboardReleaseRunSampleBuffers(void)
{
    int frames0;
    int frames2;
    int runBlock1;
    int run3Frames;

    frames0 = g_SnoboCourseRecords.runs[0].frames;
    operator delete((void *)frames0);
    runBlock1 = g_SnoboCourseRecords.runs[1].frames;
    operator delete((void *)runBlock1);
    frames2 = g_SnoboCourseRecords.runs[2].frames;
    operator delete((void *)frames2);
    run3Frames = g_SnoboCourseRecords.runs[3].frames;
    operator delete((void *)run3Frames);
    g_SnoboCourseRecords.runs[0].frames = 0;
}

/*
 * Selects g_SnoboCourseRecords.runs[triggerStage], writes its enabled, frameCount and courseTime
 * fields and copies 0x8000 dwords from g_SnoboCourseRecords.runs[0].frames into that run's
 * frames buffer without bounds checks.
 */
// FUNCTION: FF7 0x007259d7
void __cdecl Mini_SnowboardStoreRunSamplesToSlot(void)
{
    int x[4];

    x[0] = (int)&g_SnoboCourseRecords.runs[g_SnoboStatePtr->triggerStage];
    ((SnowboardRunRecord *)x[0])->enabled = 1;
    ((SnowboardRunRecord *)x[0])->frameCount = g_SnoboCourseRecords.runs[0].frameIndex;
    ((SnowboardRunRecord *)x[0])->courseTime = g_SnoboStatePtr->courseTime;
    x[3] = g_SnoboCourseRecords.runs[0].frames;
    x[2] = ((SnowboardRunRecord *)x[0])->frames;
    x[1] = 0x8000;
    while (x[1]-- != 0) {
        *(int *)x[2] = *(int *)x[3];
        x[2] += 4;
        x[3] += 4;
    }
}

/*
 * When a nonzero course and active-rider flag permit and recording count is
 * below 0x2220, appends a fifteen-byte run sample and increments the count.
 * Stores the upper three bytes of each converted XYZ coordinate and two bytes
 * of each of three scaled orientation values in big-endian byte order.
 */
// FUNCTION: FF7 0x00725a61
void __cdecl Mini_SnowboardAppendRiderSample(void)
{
    int value;
    SnowboardRiderSample *sample;
    if (g_SnoboStatePtr->triggerStage == 0) {
        return;
    }
    if (g_SnoboStatePtr->running == 0) {
        return;
    }
    value = g_SnoboCourseRecords.runs[0].frameIndex;
    if (value >= 0x2220) {
        return;
    }
    sample = (SnowboardRiderSample *)(g_SnoboCourseRecords.runs[0].frames + value * 15);
    g_SnoboCourseRecords.runs[0].frameIndex = value + 1;
    value = (int)g_SnoboStatePtr->relPosX;
    sample->posXByte3 = (value >> 24) & 255;
    sample->posXByte2 = (value >> 16) & 255;
    sample->posXByte1 = (value >> 8) & 255;
    value = (int)g_SnoboStatePtr->relPosY;
    sample->posYByte3 = (value >> 24) & 255;
    sample->posYByte2 = (value >> 16) & 255;
    sample->posYByte1 = (value >> 8) & 255;
    value = (int)g_SnoboStatePtr->riderPos;
    sample->posZByte3 = (value >> 24) & 255;
    sample->posZByte2 = (value >> 16) & 255;
    sample->posZByte1 = (value >> 8) & 255;
    value = (int)(g_SnoboStatePtr->currentAngle0 * 651.8986206054688f);
    sample->angleAByte1 = (value >> 8) & 255;
    sample->angleAByte0 = value & 255;
    value = (int)(g_SnoboStatePtr->velocity * 651.8986206054688f);
    sample->angleBByte1 = (value >> 8) & 255;
    sample->angleBByte0 = value & 255;
    value = (int)(g_SnoboStatePtr->currentAngle1 * 651.8986206054688f);
    sample->angleCByte1 = (value >> 8) & 255;
    sample->angleCByte0 = value & 255;
}

/*
 * For an enabled selected animation record, advances its frame while active
 * and below the last frame, then returns if frame is negative or action kind
 * is zero. When course rendering is enabled, decodes a 15-byte frame: big-
 * endian angles build a matrix and three signed packed coordinates become
 * block-relative translation.
 */
// FUNCTION: FF7 0x00725c4c
void __cdecl Mini_SnowboardEmitAnimFrameActions(void)
{
    SnowboardAnimActionBlock *buf;
    SnowboardRunRecord *animationRecord;
    int kindTag;
    SnowboardMatrixRecord *secondRecord;
    int frameIdx;
    int matrix[16];
    SnowboardMatrixRecord *matrixRecord;
    SnowboardMatrixRecord *tailRecord;
    SnowboardMatrixRecord *pFirstRecord;
    SnowboardAnimFrame *pFrame;

    if (g_SnoboStatePtr->triggerStage == 0) {
        return;
    }
    animationRecord = &g_SnoboCourseRecords.runs[g_SnoboStatePtr->triggerStage];
    if (animationRecord->enabled == 0) {
        return;
    }
    buf = (SnowboardAnimActionBlock *)Gfx_GetPsxScratchpadPtr(0);
    frameIdx = animationRecord->frameIndex;
    if (g_SnoboStatePtr->running != 0 && frameIdx + 1 < animationRecord->frameCount) {
        frameIdx++;
        animationRecord->frameIndex = frameIdx;
    }
    if (frameIdx < 0) {
        return;
    }
    if (g_SnoboCourseRecords.animFrameVariantWord == 0) {
        return;
    }
    if (g_SnoboStatePtr->renderFrame != 0) {
        pFrame = (SnowboardAnimFrame *)(animationRecord->frames + frameIdx * 0xF);
        buf->angleX = pFrame->angleXHi << 8 | pFrame->angleXLo;
        buf->angleY = 0;
        buf->angleZ = pFrame->angleZHi << 8 | pFrame->angleZLo;
        Matrix30SetRotationFromAnglesYXZ((struct SnowboardAngles *)&buf->angleX, &buf->matrix);
        Matrix30SetRotationY(-(short)(pFrame->rotationYHi << 8 | pFrame->rotationYLo), &buf->yawMatrix);
        PsxMultiplyMatrix33ViaLocal((PsxMatrix *)&buf->matrix, (PsxMatrix *)&buf->yawMatrix, (PsxMatrix *)&buf->matrix);
        frameIdx = pFrame->posXByte3 << 0x18 | pFrame->posXByte2 << 0x10 | pFrame->posXByte1 << 8;
        ((PsxMatrix *)&buf->matrix)->t[0] = (frameIdx >> 0xC) - g_SnoboStatePtr->originX;
        frameIdx = pFrame->posYByte3 << 0x18 | pFrame->posYByte2 << 0x10 | pFrame->posYByte1 << 8;
        ((PsxMatrix *)&buf->matrix)->t[1] = (frameIdx >> 0xC) - g_SnoboStatePtr->originY;
        frameIdx = pFrame->posZByte3 << 0x18 | pFrame->posZByte2 << 0x10 | pFrame->posZByte1 << 8;
        ((PsxMatrix *)&buf->matrix)->t[2] = (frameIdx >> 0xC) - g_SnoboStatePtr->originZ;
        GtePsxMatrixToFloat4x4((PsxMatrix *)&buf->matrix, matrix);
        kindTag = 3;
        if (g_SnoboCourseRecords.animFrameVariantWord < 3) {
            frameIdx = g_SnoboCourseRecords.animFrameVariantWord * 2 - 2;
            matrixRecord = &g_SnoboMatrixRecords[g_SnoboMatrixRecordCount];
            g_SnoboMatrixRecordCount++;
            matrixRecord->matrix = *(Copy64 *)matrix;
            matrixRecord->kind = 0;
            matrixRecord->actionCode = g_SnoboAnimFrameActionPairs[frameIdx];
            matrixRecord->animFrame = kindTag;
            matrixRecord->drawFlags = 0;
            secondRecord = &g_SnoboMatrixRecords[g_SnoboMatrixRecordCount];
            g_SnoboMatrixRecordCount++;
            secondRecord->kind = 2;
            secondRecord->actionCode = (&g_SnoboAnimFrameActionPairs[1])[frameIdx];
            secondRecord->animFrame = kindTag;
            secondRecord->drawFlags = 0;
        } else {
            pFirstRecord = &g_SnoboMatrixRecords[g_SnoboMatrixRecordCount];
            g_SnoboMatrixRecordCount++;
            pFirstRecord->matrix = *(Copy64 *)matrix;
            pFirstRecord->kind = 0;
            pFirstRecord->actionCode = 0x118;
            pFirstRecord->animFrame = kindTag;
            pFirstRecord->drawFlags = 0;
            tailRecord = &g_SnoboMatrixRecords[g_SnoboMatrixRecordCount];
            g_SnoboMatrixRecordCount++;
            tailRecord->kind = 2;
            tailRecord->actionCode = g_SnoboAnimFrameTailActions[g_SnoboStatePtr->tick >> 1 & 3];
            tailRecord->animFrame = kindTag;
            tailRecord->drawFlags = 0;
        }
    }
}

/*
 * Resets snowboard motion and event fields and selects the course segment,
 * block and starting triangle from segment and triangle. Copies saved parameters,
 * initializes rotation, computes and rotates a starting midpoint and combines
 * it with block position into float fixed-point coordinates scaled by 4096.
 */
// FUNCTION: FF7 0x00726015
void __cdecl Mini_SnowboardInitRunAtCourseEntry(int segment, int triangle, int riderState)
{
    short rotation[0x10];
    short pos[4];
    SnowboardCourseEntry *dat;

    g_SnoboStatePtr->activeFlags = 0;
    g_SnoboStatePtr->courseTime = 0;
    g_SnoboStatePtr->triggerLatched = 0;
    g_SnoboStatePtr->displayedSpeed = 0;
    g_SnoboStatePtr->paused = 0;
    g_SnoboStatePtr->timer = 0;
    g_SnoboStatePtr->spinCount = 0;
    g_SnoboStatePtr->pickupTotal = 0;
    g_SnoboStatePtr->pickupFlag = 0;
    g_SnoboStatePtr->prevNowMs = 0;
    if (g_SnoboModeFlag == SNOWBOARD_MODE_EVENT) {
        g_SnoboLastSfxBits = 0;
        g_SnoboCourseEndReached = 0;
    }
    g_SnoboStatePtr->triggerStage = 0;
    g_SnoboStatePtr->segment = (short)segment;
    g_SnoboStatePtr->segmentPtr = &g_SnoboStatePtr->segmentTable[segment];
    g_SnoboStatePtr->chainIndex = (short)triangle;
    dat = (SnowboardCourseEntry *)&g_SnoboStatePtr->courseHeader[g_SnoboStatePtr->segmentPtr->blockIndexTable[triangle]];
    *(int *)&g_SnoboCameraPosWord[0] = *(int *)&g_SnoboCourseRecords.introEndAngles[0];
    *(int *)&g_SnoboCameraPosWord[2] = *(int *)&g_SnoboCourseRecords.introEndAngles[2];
    *(int *)&g_SnoboCourseRecords.cameraOffset[0] = *(int *)&g_SnoboCourseRecords.introEndOffset[0];
    *(int *)&g_SnoboCourseRecords.cameraOffset[2] = *(int *)&g_SnoboCourseRecords.introEndOffset[2];
    g_SnoboCameraPos[0] = 0.0f;
    g_SnoboCameraPos[1] = 3.1415927f;
    g_SnoboCameraPos[2] = 0.0f;
    Matrix30SetRotationY(-g_SnoboStatePtr->segmentPtr->angleTable[triangle],
                         rotation);
    pos[0] = (short)((dat->x0 + dat->x1) >> 1);
    pos[1] = (short)((dat->y0 + dat->y1) >> 1);
    pos[2] = (short)((dat->z0 + dat->z1) >> 1);
    PsxApplyMatrix33ToShortVector(rotation, pos, pos);
    g_SnoboStatePtr->originX =
        g_SnoboStatePtr->segmentPtr->vectors[triangle].x;
    g_SnoboStatePtr->originY =
        g_SnoboStatePtr->segmentPtr->vectors[triangle].y;
    g_SnoboStatePtr->originZ =
        g_SnoboStatePtr->segmentPtr->vectors[triangle].z;
    g_SnoboStatePtr->relPosX =
        (float)((g_SnoboStatePtr->originX + pos[0]) << 0xC);
    g_SnoboStatePtr->relPosY =
        (float)((g_SnoboStatePtr->originY + pos[1]) << 0xC);
    g_SnoboStatePtr->riderPos =
        (float)((g_SnoboStatePtr->originZ + pos[2]) << 0xC);
    g_SnoboStatePtr->velocityX = 0.0f;
    g_SnoboStatePtr->fadeLevel = 0.0f;
    g_SnoboStatePtr->velocityZ = 0.0f;
    g_SnoboStatePtr->clearedRunWordA = 0;
    g_SnoboStatePtr->velocity =
        g_SnoboStatePtr->segmentPtr->angleTable[triangle] *
        0.0015339808f;
    g_SnoboStatePtr->clearedRunWordB = 0;
    g_SnoboStatePtr->currentAngle0 = 0.0f;
    g_SnoboStatePtr->targetAngle0 = 0.0f;
    g_SnoboStatePtr->currentAngleY = 0;
    g_SnoboStatePtr->targetAngleY = 0;
    g_SnoboStatePtr->currentAngle1 = 0.0f;
    g_SnoboStatePtr->targetAngle1 = 0.0f;
    g_SnoboStatePtr->riderState = (short)riderState;
    g_SnoboStatePtr->followAngle = 0;
    pos[0] = (short)((int)g_SnoboStatePtr->relPosX >> 0xC);
    pos[1] = (short)((int)g_SnoboStatePtr->relPosY >> 0xC);
    pos[2] = (short)((int)g_SnoboStatePtr->riderPos >> 0xC);
    g_SnoboStatePtr->triangle = (short)((SnoboSpawnFn)Mini_SnowboardFindCourseTriangleIndex)(pos, (int)dat, Gfx_GetPsxScratchpadPtr(0));
    Mini_SnowboardResetRiderRecord();
    Mini_SnowboardSelectEffectCallbacks();
    Mini_SnowboardInvokeClearEffectCallback();
    if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
        Mini_SnowboardInitRiderSampleBuffers();
    }
    g_SnoboStatePtr->flags = 0;
    g_SnoboStatePtr->rotationY = 0;
    if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
        g_SnoboStatePtr->segmentTable[2].linkedBlock =
            g_SnoboStatePtr->bestScores[0] < 0x46;
        g_SnoboStatePtr->segmentTable[4].linkedBlock =
            g_SnoboStatePtr->bestScores[1] < 0x46;
    }
    g_SnoboRunFinished = 0;
    g_SnoboFadeLevel = 0;
}

/*
 * Resets the buffer bookkeeping: zeroes g_SnoboCourseRecords.runs[0].frameIndex and
 * g_SnoboCourseRecords.runs[0].frameCount, sets
 * g_SnoboCourseRecords.runs[1].frameIndex,
 * g_SnoboCourseRecords.runs[2].frameIndex and
 * g_SnoboCourseRecords.runs[3].frameIndex to 0xFFFF, and if
 * g_SnoboCourseRecords.runs[0].frames is still zero allocates four 0x20000-byte blocks with
 * operator new into g_SnoboCourseRecords.runs[0].frames,
 * g_SnoboCourseRecords.runs[1].frames, g_SnoboCourseRecords.runs[2].frames and
 * g_SnoboCourseRecords.runs[3].frames.
 */
// FUNCTION: FF7 0x007264dd
void __cdecl Mini_SnowboardInitRiderSampleBuffers(void)
{
    g_SnoboCourseRecords.runs[0].frameIndex = 0;
    g_SnoboCourseRecords.runs[0].frameCount = 0;
    g_SnoboCourseRecords.runs[1].frameIndex = 0xFFFF;
    g_SnoboCourseRecords.runs[2].frameIndex = 0xFFFF;
    g_SnoboCourseRecords.runs[3].frameIndex = 0xFFFF;
    if (g_SnoboCourseRecords.runs[0].frames == 0) {
        g_SnoboCourseRecords.runs[0].frames = (int)operator new(0x20000);
        g_SnoboCourseRecords.runs[1].frames = (int)operator new(0x20000);
        g_SnoboCourseRecords.runs[2].frames = (int)operator new(0x20000);
        g_SnoboCourseRecords.runs[3].frames = (int)operator new(0x20000);
    }
}


#pragma intrinsic(abs)

#pragma intrinsic(memset)

// SnowboardClipper is provided by ff7/snobo_primitives.h above.
//


__declspec(dllimport) unsigned long __stdcall timeGetTime();

#pragma intrinsic(abs)
#pragma intrinsic(cos)
#pragma intrinsic(sin)
#pragma intrinsic(sqrt, atan2)

} // extern "C"
inline unsigned int SnowboardPackBatchColor(unsigned int c) { return (((c & 0xff0000) >> 16) | 0xff000000) | (c & 0xff00) | ((c & 255) << 16); }
extern "C" {
/*
 * Emits course records into data block 0. Starts refPos rows behind the state's current row of its
 * current course segment (32-byte records of segmentTable, 12-byte rows); a negative row
 * falls back into the previous segment (its row count plus the offset, clamped at row 0). Unless
 * renderFrame is zero, transforms, emits and appends one record per row for up to recordCount
 * rows; a row of kind 3 continues in the segment it names, and a second branch segment is walked
 * afterwards with the remaining count.
 */
// FUNCTION: FF7 0x00726560
void __cdecl Mini_SnowboardEmitCourseChain(int refPos, int recordCount)
{
    short fraction;
    SnowboardCourseNodeState *node;
    short previous;
    short course;
    node = (SnowboardCourseNodeState *)Gfx_GetPsxScratchpadPtr(0);
    node->branchSegment = 0;
    node->active = g_SnoboStatePtr->activeFlags & g_SnoboActiveFlagMask;
    node->rowsLeft = (short)recordCount;
    node->segment = g_SnoboStatePtr->segment;
    node->chainIndex = g_SnoboStatePtr->chainIndex;
    node->baseX = g_SnoboStatePtr->originX;
    node->baseY = g_SnoboStatePtr->originY;
    node->baseZ = g_SnoboStatePtr->originZ;
    node->recordCount = 0;
    fraction = g_SnoboStatePtr->chainIndex - (short)refPos;
    course = g_SnoboStatePtr->segment;
    if (fraction < 0) {
        previous = g_SnoboStatePtr->segmentPtr->retreatSegment;
        if (previous == 255)
            fraction = 0;
        else {
            fraction = g_SnoboStatePtr->segmentTable[previous].nodeCount + fraction;
            if (fraction < 0)
                fraction = 0;
            course = previous;
        }
    }
    node->segmentIndex = course;
    node->nodeIndex = fraction;
    node->courseNodePtr = &g_SnoboStatePtr->segmentTable[course];
    node->posPtr = &node->courseNodePtr->vectors[node->nodeIndex];
    Mini_SnowboardUpdateCourseNodeTransform(node);
    if (g_SnoboStatePtr->renderFrame == 0) { /* empty first arm: lib/codegen.h */
    } else {
        Mini_SnowboardRefreshCourseBlockVertices(node);
        Mini_SnowboardEmitCourseMesh(node);
        Mini_SnowboardAppendCourseRecord(node);
        for (;;) {
            node->chainFlag = 0;
            while (node->rowsLeft-- > 0) {
                if (node->courseBlock->kind == 3) {
                    node->segmentIndex = node->courseNodePtr->linkSegmentA;
                    node->branchSegment = node->courseNodePtr->linkSegmentB;
                    node->courseNodePtr = &g_SnoboStatePtr->segmentTable[node->segmentIndex];
                    node->nodeIndex = 0;
                    node->posPtr = node->courseNodePtr->vectors;
                    node->rowsLeft--;
                    node->savedRowsLeft = node->rowsLeft;
                    node->continuedSegment = 1;
                } else {
                    node->nodeIndex++;
                    if (node->nodeIndex >= node->courseNodePtr->nodeCount)
                        break;
                    node->posPtr++;
                    node->continuedSegment = 0;
                }
                Mini_SnowboardUpdateCourseNodeTransform(node);
                if (node->continuedSegment)
                    Mini_SnowboardTransformCourseVertices(node, node->edgeRow, node->edgeColors);
                else
                    Mini_SnowboardTransformCourseVertices(node, &node->vertices[42], &node->colors[42]);
                Mini_SnowboardEmitCourseMesh(node);
                Mini_SnowboardAppendCourseRecord(node);
            }
            if (node->branchSegment) {
                node->segmentIndex = node->branchSegment;
                node->branchSegment = 0;
                node->courseNodePtr = &g_SnoboStatePtr->segmentTable[node->segmentIndex];
                node->nodeIndex = 0;
                node->posPtr = node->courseNodePtr->vectors;
                node->rowsLeft = node->savedRowsLeft;
                Mini_SnowboardUpdateCourseNodeTransform(node);
                if (Mini_SnowboardTransformCourseVertices(node, &node->edgeRow[6], &node->edgeColors[6]) == 0) {
                    Mini_SnowboardEmitCourseMesh(node);
                    Mini_SnowboardAppendCourseRecord(node);
                    continue;
                }
            }
            break;
        }
        g_SnoboStatePtr->clipper->flush();
        Mini_SnowboardEmitCourseRecordActions(node);
    tail:
        Mini_SnowboardInvokeRecordVisitCallbacks(node);
        Mini_SnowboardUpdateEffectNodes(node);
        return;
    }
    /* goto kept: the empty then-branch jumps here past the block's return, and this jumps back into the block */
    goto tail;
}

/*
 * Builds the object yaw matrix from its indexed short angle, fills its
 * translation from target minus current coordinates and installs it in the
 * clipper. Resolves the current course-block pointer through the segment byte-
 * index table.
 */
// FUNCTION: FF7 0x0072698c
void __cdecl Mini_SnowboardUpdateCourseNodeTransform(SnowboardCourseNodeState *node)
{
    struct {
        GfxMatrix4x4 *matrix;
        float sine, cosine, angle;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.angle = -((float)node->courseNodePtr->angleTable[node->nodeIndex] * 0.00153398083f);
    frame.matrix = (GfxMatrix4x4 *)&node->matrixBlock;
    frame.sine = (double)(float)sin(frame.angle);
    frame.cosine = (double)(float)cos(frame.angle);
    frame.matrix->_11 = frame.cosine;
    frame.matrix->_12 = 0;
    frame.matrix->_13 = frame.sine;
    frame.matrix->_14 = 0;
    frame.matrix->_21 = 0;
    frame.matrix->_22 = 1;
    frame.matrix->_23 = 0;
    frame.matrix->_24 = 0;
    frame.matrix->_31 = -frame.sine;
    frame.matrix->_32 = 0;
    frame.matrix->_33 = frame.cosine;
    frame.matrix->_34 = 0;
    frame.matrix->_41 = 0;
    frame.matrix->_42 = 0;
    frame.matrix->_43 = 0;
    frame.matrix->_44 = 1;
    node->relX = (float)(node->posPtr->x - node->baseX);
    node->relY = (float)(node->posPtr->y - node->baseY);
    node->relZ = (float)(node->posPtr->z - node->baseZ);
    g_SnoboStatePtr->clipper->setMatrix((int)&node->matrixBlock);
    node->courseBlock = &g_SnoboStatePtr->courseHeader[node->courseNodePtr->blockIndexTable[node->nodeIndex]];
}

/*
 * Copies seven supplied transformed vertex records and associated values, then
 * transforms the remaining vertices through index 0x30 from the current course
 * block and copies their associated values. Runs the extra setup helper for
 * block type three and always returns zero.
 */
// FUNCTION: FF7 0x00726b1c
int __cdecl Mini_SnowboardTransformCourseVertices(SnowboardCourseNodeState *block, SnowboardClipInputVertex *srcVerts, int *srcColors)
{
    struct {
        SnowboardTriangleVertex *srcVert;
        int vertIdx;
        float posX, posY, posZ;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    for (frame.vertIdx = 0; frame.vertIdx < 7; frame.vertIdx++) {
        block->vertices[frame.vertIdx] = srcVerts[frame.vertIdx];
        block->colors[frame.vertIdx] = srcColors[frame.vertIdx];
    }
    for (frame.vertIdx = 7; frame.vertIdx < 0x31; frame.vertIdx++) {
        frame.srcVert = &block->courseBlock->vertices[frame.vertIdx];
        frame.posX = (float)frame.srcVert->posX;
        frame.posY = (float)frame.srcVert->posY;
        frame.posZ = (float)frame.srcVert->posZ;
        g_SnoboStatePtr->clipper->transform((int)&frame.posX, (int)&block->vertices[frame.vertIdx]);
        block->colors[frame.vertIdx] = block->courseBlock->colors[frame.vertIdx];
    }
    if (block->courseBlock->kind == 3) {
        Mini_SnowboardSubdivideCourseEdgeRow(block);
    }
    return 0;
}

/*
 * Transforms all 0x31 course-block vertices through the installed clipper.
 * Chooses the first seven associated values from the preceding segment, the
 * linked preceding block or the current block fallback, then copies the
 * remaining values from the current block.
 */
// FUNCTION: FF7 0x00726c47
void __cdecl Mini_SnowboardRefreshCourseBlockVertices(SnowboardCourseNodeState *block)
{
    struct {
        SnowboardTriangleVertex *srcVert;
        SnowboardSegment *prevSegment;
        int *srcColorPtr;
        int *dstColorPtr;
        int prevNodeIdx, retreatSegIdx;
        float posX, posY, posZ;
        int vertexIndex;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    for (frame.vertexIndex = 0; frame.vertexIndex < 0x31; frame.vertexIndex++) {
        frame.srcVert = &block->courseBlock->vertices[frame.vertexIndex];
        frame.posX = (float)frame.srcVert->posX;
        frame.posY = (float)frame.srcVert->posY;
        frame.posZ = (float)frame.srcVert->posZ;
        g_SnoboStatePtr->clipper->transform((int)&frame.posX, (int)&block->vertices[frame.vertexIndex]);
    }
    frame.prevNodeIdx = block->nodeIndex - 1;
    if (frame.prevNodeIdx < 0) {
        frame.retreatSegIdx = block->courseNodePtr->retreatSegment;
        if (frame.retreatSegIdx == 0xFF) {
            frame.srcColorPtr = block->courseBlock->colors;
        } else {
            frame.prevSegment = &g_SnoboStatePtr->segmentTable[frame.retreatSegIdx];
            frame.prevNodeIdx = frame.prevSegment->nodeCount - 1;
            frame.srcColorPtr = &g_SnoboStatePtr->courseHeader[frame.prevSegment->blockIndexTable[frame.prevNodeIdx]].colors[42];
        }
    } else {
        frame.srcColorPtr = &g_SnoboStatePtr->courseHeader[block->courseNodePtr->blockIndexTable[frame.prevNodeIdx]].colors[42];
    }
    frame.dstColorPtr = block->colors;
    for (frame.vertexIndex = 0; frame.vertexIndex < 7; frame.vertexIndex++) {
        *frame.dstColorPtr = *frame.srcColorPtr;
        frame.dstColorPtr++;
        frame.srcColorPtr++;
    }
    for (frame.srcColorPtr = &block->courseBlock->colors[7]; frame.vertexIndex < 0x31; frame.vertexIndex++) {
        *frame.dstColorPtr = *frame.srcColorPtr;
        frame.dstColorPtr++;
        frame.srcColorPtr++;
    }
    if (block->courseBlock->kind == 3) {
        Mini_SnowboardSubdivideCourseEdgeRow(block);
    }
}

/*
 * Expands seven consecutive four-float entries starting at source index 42
 * into thirteen entries of edgeRow, alternating copied originals with
 * componentwise midpoints. Copies each original packed color and averages the
 * three low color bytes for the midpoint while retaining the first color alpha
 * byte; copies the final original and color after the loop.
 */
// FUNCTION: FF7 0x00726e1e
void __cdecl Mini_SnowboardSubdivideCourseEdgeRow(SnowboardCourseNodeState *block)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[32];
        SnowboardClipInputVertex *midPtr;
        struct { unsigned char pad_to_4[0x4]; SnowboardClipInputVertex *endPtr; };
        struct { unsigned char pad_to_8[0x8]; SnowboardClipInputVertex *startPtr; };
        struct { unsigned char pad_to_C[0xC]; int midColor; };
        struct { unsigned char pad_to_D[0xD]; char midByte1; };
        struct { unsigned char pad_to_E[0xE]; char midByte2; };
        struct { unsigned char pad_to_F[0xF]; char midAlpha; };
        struct { unsigned char pad_to_10[0x10]; int colorEnd; };
        struct { unsigned char pad_to_11[0x11]; int endByte1; };
        struct { unsigned char pad_to_12[0x12]; int endByte2; };
        struct { unsigned char pad_to_14[0x14]; int colorStart; };
        struct { unsigned char pad_to_15[0x15]; int startByte1; };
        struct { unsigned char pad_to_16[0x16]; int startByte2; };
        struct { unsigned char pad_to_17[0x17]; char startAlpha; };
        struct { unsigned char pad_to_18[0x18]; int srcIndex; };
        struct { unsigned char pad_to_1C[0x1C]; int dstIndex; };
    } frame;
#pragma pack(pop)

    frame.dstIndex = 0;
    for (frame.srcIndex = 0x2A; frame.srcIndex < 0x30; frame.dstIndex += 2, frame.srcIndex++) {
        block->edgeRow[frame.dstIndex] = block->vertices[frame.srcIndex];
        frame.midPtr = &block->edgeRow[frame.dstIndex + 1];
        frame.endPtr = &block->vertices[frame.srcIndex + 1];
        frame.startPtr = &block->vertices[frame.srcIndex];
        frame.midPtr->x = (frame.startPtr->x + frame.endPtr->x) * 0.5f;
        frame.midPtr->y = (frame.startPtr->y + frame.endPtr->y) * 0.5f;
        frame.midPtr->clipZ = (frame.startPtr->clipZ + frame.endPtr->clipZ) * 0.5f;
        frame.midPtr->w = (frame.startPtr->w + frame.endPtr->w) * 0.5f;
        frame.colorStart = block->colors[frame.srcIndex];
        frame.colorEnd = (&block->colors[1])[frame.srcIndex];
        block->edgeColors[frame.dstIndex] = frame.colorStart;
        *(char *)&frame.midColor = (frame.colorStart & 0xFF) + (frame.colorEnd & 0xFF) >> 1;
        frame.midByte1 = (frame.startByte1 & 0xFF) + (frame.endByte1 & 0xFF) >> 1;
        frame.midByte2 = (frame.startByte2 & 0xFF) + (frame.endByte2 & 0xFF) >> 1;
        frame.midAlpha = frame.startAlpha;
        (&block->edgeColors[1])[frame.dstIndex] = frame.midColor;
    }
    block->edgeRow[frame.dstIndex] = block->vertices[frame.srcIndex];
    block->edgeColors[frame.dstIndex] = block->colors[frame.srcIndex];
}

/*
 * Emits the node's course block through the clipper: when the node is not active, its 72 faces as
 * triangles with their vertex colours; otherwise the 6x6 grid of quads in the segment's alternating
 * colour pair.
 */
// FUNCTION: FF7 0x0072700c
void __cdecl Mini_SnowboardEmitCourseMesh(SnowboardCourseNodeState *node)
{
    struct {
        int color, index, column;
        int vertices[4];
        int sourceColors[2];
        unsigned int packed;
        int component;
        SnowboardCourseFace *pointer;
        int indices[3];
        SnowboardClipInputVertex triangle[3];
        int face;
        int colors[3];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (node->active == 0) {
        frame.pointer = node->courseBlock->faces;
        for (frame.face = 0; frame.face < 0x48; frame.face++) {
            frame.packed = frame.pointer->word;
            frame.pointer++;
            frame.indices[0] = frame.packed & 255;
            frame.indices[2] = (frame.packed >> 8) & 255;
            frame.indices[1] = (frame.packed >> 16) & 255;
            for (frame.component = 0; frame.component < 3; frame.component++) {
                frame.triangle[frame.component] = node->vertices[frame.indices[frame.component]];
                frame.colors[frame.component] = SnowboardPackBatchColor(node->colors[frame.indices[frame.component]]);
            }
            g_SnoboStatePtr->clipper->emitTriangle((int)frame.triangle, (int)frame.colors);
        }
    } else {
        frame.column = 0;
        frame.sourceColors[0] = g_SnoboColorPairs[node->segmentIndex].a;
        frame.sourceColors[1] = g_SnoboColorPairs[node->segmentIndex].b;
        for (frame.index = 0; frame.index < 0x2a; frame.index++) {
            frame.column++;
            if (frame.column >= 7) {
                frame.column = 0;
                continue;
            }
            frame.vertices[0] = frame.index;
            frame.vertices[1] = frame.index + 1;
            frame.vertices[2] = frame.index + 7;
            frame.vertices[3] = frame.index + 8;
            frame.color = SnowboardPackBatchColor(frame.sourceColors[frame.index & 1]);
            g_SnoboStatePtr->clipper->emitQuad((int)&node->vertices[frame.vertices[0]], (int)&node->vertices[frame.vertices[1]], (int)&node->vertices[frame.vertices[2]], (int)&node->vertices[frame.vertices[3]], frame.color);
        }
    }
}

/*
 * Appends one SnowboardCourseActionRecord (the node's segment, node, course block and matrix) to the
 * table at g_SnoboCourseActionRecords, at index recordCount; it does nothing once recordCount reaches 0x14.
 */
// FUNCTION: FF7 0x00727265
void __cdecl Mini_SnowboardAppendCourseRecord(SnowboardCourseNodeState *cursor)
{
    SnowboardCourseActionRecord *recordPtr;

    if (cursor->recordCount >= 0x14) {
        return;
    }
    recordPtr = (SnowboardCourseActionRecord *)(cursor->recordCount * 0x4C + (char *)&g_SnoboCourseActionRecords);
    recordPtr->blockIndex = cursor->segmentIndex;
    recordPtr->nodeIndex = cursor->nodeIndex;
    recordPtr->vertexBase = cursor->courseBlock;
    recordPtr->matrix = cursor->matrixBlock;
    (cursor->recordCount)++;
}

/*
 * When the state's renderFrame is set, walks the
 * signed-short count of 76-byte input records and copies each transform into
 * courseRecord's matrixBlock. A nonzero packed action word emits one transform-bearing output
 * record followed by type 2 records for its remaining nonzero bytes.
 */
// FUNCTION: FF7 0x007272dd
void __cdecl Mini_SnowboardEmitCourseRecordActions(SnowboardCourseNodeState *courseRecord)
{
    SnowboardCourseActionRecord *inputRecord;
    unsigned int actionWord;
    SnowboardMatrixRecord *extraRecord;
    float accumX;
    SnowboardMatrixRecord *modeRecord;
    SnowboardTriangleVertex *sourceVertex;
    GfxHomogeneousPoint deltaX;
    SnowboardMatrixRecord *altRecord;
    int numRecords;
    SnowboardMatrixRecord *actionRecordPtr;
    float savedYTerm;
    float prevZ;
    int vertexSlot;
    GfxHomogeneousPoint vertX;

    if (g_SnoboStatePtr->renderFrame == 0) {
        return;
    }
    numRecords = courseRecord->recordCount;
    for (inputRecord = (SnowboardCourseActionRecord *)&g_SnoboCourseActionRecords; numRecords-- != 0; inputRecord++) {
        courseRecord->matrixBlock = inputRecord->matrix;
        actionWord = g_SnoboStatePtr->segmentTable[inputRecord->blockIndex].actionTable[inputRecord->nodeIndex];
        if (actionWord != 0) {
            actionRecordPtr = &g_SnoboMatrixRecords[g_SnoboMatrixRecordCount];
            g_SnoboMatrixRecordCount++;
            actionRecordPtr->matrix = courseRecord->matrixBlock;
            actionRecordPtr->kind = 0;
            actionRecordPtr->actionCode = actionWord & 0xFF;
            actionRecordPtr->animFrame = 0;
            actionRecordPtr->drawFlags = 0;
            for (actionWord >>= 8; actionWord != 0; actionWord >>= 8) {
                extraRecord = &g_SnoboMatrixRecords[g_SnoboMatrixRecordCount];
                g_SnoboMatrixRecordCount++;
                extraRecord->kind = 2;
                extraRecord->actionCode = actionWord & 0xFF;
                extraRecord->animFrame = 0;
                extraRecord->drawFlags = 0;
            }
        } else {
            accumX = courseRecord->relX;
            savedYTerm = courseRecord->relY;
            prevZ = courseRecord->relZ;
            for (vertexSlot = 0; vertexSlot < 6; vertexSlot++) {
                sourceVertex = &inputRecord->vertexBase->vertices[g_SnoboNodeMarkerVertexSlots[vertexSlot]];
                vertX.x = (float)sourceVertex->posX;
                vertX.y = (float)sourceVertex->posY;
                vertX.z = (float)sourceVertex->posZ;
                RotateVector3RowVec(&courseRecord->matrixBlock, &vertX.x, &deltaX.x);
                courseRecord->relX = deltaX.x + accumX;
                courseRecord->relY = deltaX.y + savedYTerm;
                courseRecord->relZ = deltaX.z + prevZ;
                if (g_SnoboModeFlag == SNOWBOARD_MODE_EVENT) {
                    modeRecord = &g_SnoboMatrixRecords[g_SnoboMatrixRecordCount];
                    g_SnoboMatrixRecordCount++;
                    modeRecord->matrix = courseRecord->matrixBlock;
                    modeRecord->kind = 0;
                    modeRecord->actionCode = 0xE7;
                    modeRecord->animFrame = 2;
                    modeRecord->drawFlags = 0;
                } else {
                    altRecord = &g_SnoboMatrixRecords[g_SnoboMatrixRecordCount];
                    g_SnoboMatrixRecordCount++;
                    altRecord->matrix = courseRecord->matrixBlock;
                    altRecord->kind = 0;
                    altRecord->actionCode = 0xAF;
                    altRecord->animFrame = 2;
                    altRecord->drawFlags = 0;
                }
            }
        }
    }
}

/*
 * Walks the cursor's recordCount through 0x4C-byte records at
 * g_SnoboCourseActionRecords, calling Mini_SnowboardInvokeVisitCallback with the
 * sectionTable word indexed by each record's blockIndex plus its nodeIndex.
 */
// FUNCTION: FF7 0x007275d6
void __cdecl Mini_SnowboardInvokeRecordVisitCallbacks(SnowboardCourseNodeState *cursor)
{
    short count;
    SnowboardCourseActionRecord *recordPtr;

    count = cursor->recordCount;
    recordPtr = (SnowboardCourseActionRecord *)&g_SnoboCourseActionRecords;
    for (; count--; recordPtr++) {
        Mini_SnowboardInvokeVisitCallback(g_SnoboStatePtr->sectionTable[recordPtr->blockIndex] + recordPtr->nodeIndex);
    }
}

/*
 * Converts the two float triples at posA and posB into fixed-point vectors on
 * the frame with GteFloat3ToVector, passes both to
 * Mini_SnowboardProbeFixedPositions, and writes the
 * results back over posA and posB with GteVectorToFloat3. Unreferenced padding
 * locals preserve the retail VC5 /Od stack frame size.
 */
// FUNCTION: FF7 0x0072763b
void __cdecl Mini_SnowboardProbeFloatPositions(float *posA, float *posB)
{
    int vecA[4];
    int vecB[4];

    GteFloat3ToVector(vecA, posA);
    GteFloat3ToVector(vecB, posB);
    /* Retail caller passes -1 arg(s) to 2-parameter `Mini_SnowboardProbeFixedPositions` (__cdecl stack cleanup matches retail). */
    ((void(__cdecl *)(...))Mini_SnowboardProbeFixedPositions)(vecA, vecB);
    GteVectorToFloat3(vecA, posA);
    GteVectorToFloat3(vecB, posB);
}

/*
 * Initializes the shared course-probe block with current segment and two
 * positions shifted by twelve. Classifies its two probe values against three
 * boundaries each, uses block type and those classifications to dispatch a
 * probe handler, and either copies/retries the second position for a nonzero
 * result below four attempts or stores the resulting probe values in current
 * state.
 */
// FUNCTION: FF7 0x00727695
void __cdecl Mini_SnowboardProbeFixedPositions(Copy16 *posA, Copy16 *posB)
{
    SnowboardProbe *pProbe;
    int opIndex;
    int bandX;
    int bandZ;
    SnowboardTriangleVertex *boundaries;
    pProbe = (SnowboardProbe *)Gfx_GetPsxScratchpadPtr(0);
    pProbe->retryCount = 0;
    pProbe->segment = g_SnoboStatePtr->segment;
    pProbe->advanceCount = g_SnoboStatePtr->chainIndex;
    pProbe->courseTable = g_SnoboStatePtr->segmentPtr;
    pProbe->p1X = posA->v[0] >> 12;
    pProbe->p1Z = posA->v[2] >> 12;
    pProbe->p2X = posB->v[0] >> 12;
    pProbe->p2Z = posB->v[2] >> 12;
    Mini_SnowboardFillCourseRecord(pProbe, pProbe->advanceCount);
    opIndex = pProbe->deltaX;
    boundaries = pProbe->course->vertices;
    for (bandX = 0; bandX < 3; bandX++) {
        if (opIndex < boundaries[bandX].bound) {
            break;
        }
    }
    opIndex = pProbe->deltaZ;
    for (bandZ = 0; bandZ < 3; bandZ++) {
        if (opIndex > (&boundaries[3])[bandZ].bound) {
            break;
        }
    }
    opIndex = g_SnoboProbeHandlerIndex[bandX + bandZ * 4 + (pProbe->course->kind << 4)];
    pProbe->opResult = g_SnoboProbeHandlers[opIndex]((int)pProbe);
    if (pProbe->opResult != 0 && ++pProbe->retryCount < 4) {
        *posA = *posB;
        ((void(__cdecl *)(...))Mini_SnowboardUpdateCourseHeading)((int)pProbe, posA, posB);
    } else {
        g_SnoboStatePtr->probeDeltaX = pProbe->deltaX;
        g_SnoboStatePtr->probeDeltaZ = pProbe->deltaZ;
    }
}


#pragma intrinsic(abs)

#pragma intrinsic(memset)


__declspec(dllimport) unsigned long __stdcall timeGetTime();

#pragma intrinsic(abs)
#pragma intrinsic(cos)
#pragma intrinsic(sin)
#pragma intrinsic(sqrt, atan2)

// Fills the probe for element index. The entry-index byte table selects a 0x374-byte course record
// from the courseHeader array, stored as probe->course. The rotation table gives a Y angle, which
// is negated and turned into probe->matrix by Matrix30SetRotationY. The point at
// pointTable gives x and z, which are subtracted from p1X and p1Z into deltaX and deltaZ.
// PsxApplyMatrix33ToShortVector then transforms the short vector at probe->deltaX in place by that
// matrix.
// FUNCTION: FF7 0x00727870
void __cdecl Mini_SnowboardFillCourseRecord(SnowboardProbe *probe, short index)
{
    SnowboardVec *posPtr;

    probe->course = &g_SnoboStatePtr->courseHeader[probe->courseTable->blockIndexTable[index]];
    Matrix30SetRotationY(-probe->courseTable->angleTable[index], &probe->matrix);
    posPtr = &probe->courseTable->vectors[index];
    probe->deltaX = probe->p1X - posPtr->x;
    probe->deltaZ = probe->p1Z - posPtr->z;
    PsxApplyMatrix33ToShortVector(&probe->matrix, &probe->deltaX, &probe->deltaX);
}

/*
 * Searches the course block's 72 faces in reverse order for the one containing the X/Z point.
 * Rejects candidates when any of their three vertices differs from the point by more than the
 * search radius (vertices[6].bound) on either axis.
 */
// FUNCTION: FF7 0x0072791e
int __cdecl Mini_SnowboardFindCourseTriangleIndex(short *point, SnowboardCourseBlock *course, SnowboardProbe *probe)
{
    struct {
        union {
            int i;
            short s;
            short zx[2]; /* the point's z, then x */
        } at148;
        union {
            int i;
            short s;
            short zx[2]; /* the point's z, then x */
        } at144;
        union {
            int i;
            short s;
            short zx[2]; /* the point's z, then x */
        } at140;
        union {
            int i;
            short s;
        } at136;
        union {
            int i;
            short s;
        } at132;
        union {
            int i;
            short s;
            short *pt;
        } at128;
        union {
            int i;
            short s;
            short *pt;
        } at124;
        union {
            int i;
            short s;
        } at120;
        union {
            int i;
            short s;
            short *pt;
        } at116;
        union {
            int i;
            short s;
        } at112;
        union {
            int i;
            short s;
            short zx[2]; /* the point's z, then x */
        } at108;
        union {
            int i;
            short s;
            short zx[2]; /* the point's z, then x */
        } at104;
        union {
            int i;
            short s;
            short zx[2]; /* the point's z, then x */
        } at100;
        union {
            int i;
            short s;
        } at96;
        union {
            int i;
            short s;
        } at92;
        union {
            int i;
            short s;
            short *pt;
        } at88;
        union {
            int i;
            short s;
            short *pt;
        } at84;
        union {
            int i;
            short s;
        } at80;
        union {
            int i;
            short s;
            short *pt;
        } at76;
        union {
            int i;
            short s;
        } at72;
        union {
            int i;
            short s;
            short zx[2]; /* the point's z, then x */
        } at68;
        union {
            int i;
            short s;
            short zx[2]; /* the point's z, then x */
        } at64;
        union {
            int i;
            short s;
            short zx[2]; /* the point's z, then x */
        } at60;
        union {
            int i;
            short s;
        } at56;
        union {
            int i;
            short s;
        } at52;
        union {
            int i;
            short s;
            short *pt;
        } at48;
        union {
            int i;
            short s;
            short *pt;
        } at44;
        union {
            int i;
            short s;
        } at40;
        union {
            int i;
            short s;
            short *pt;
        } at36;
        union {
            int i;
            short s;
        } at32;
        union {
            int i;
            short s;
        } at28;
        union {
            int i;
            short s;
            SnowboardTriangleVertex *vertex;
        } at24;
        union {
            int i;
            short s;
        } at20;
        union {
            int i;
            short s;
        } at16;
        union {
            int i;
            short s;
        } at12;
        union {
            int i;
            short s;
        } at8;
        union {
            int i;
            short s;
        } at4;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.at12.i = ((unsigned short *)point)[0] << 16 | ((unsigned short *)point)[2];
    frame.at4.i = course->vertices[6].bound;
    frame.at28.i = 72;
    while (frame.at28.i--) {
        if (abs(course->vertices[course->faces[frame.at28.i].vertexIndex[0]].posX - point[0]) > frame.at4.i)
            continue;
        if (abs(course->vertices[course->faces[frame.at28.i].vertexIndex[0]].posZ - point[2]) > frame.at4.i)
            continue;
        if (abs(course->vertices[course->faces[frame.at28.i].vertexIndex[1]].posX - point[0]) > frame.at4.i)
            continue;
        if (abs(course->vertices[course->faces[frame.at28.i].vertexIndex[1]].posZ - point[2]) > frame.at4.i)
            continue;
        if (abs(course->vertices[course->faces[frame.at28.i].vertexIndex[2]].posX - point[0]) > frame.at4.i)
            continue;
        if (abs(course->vertices[course->faces[frame.at28.i].vertexIndex[2]].posZ - point[2]) > frame.at4.i)
            continue;
        probe->triangleEntry = course->faces[frame.at28.i].word;
        frame.at24.vertex = &course->vertices[probe->vertexIndexA];
        frame.at8.i = frame.at24.vertex->posX << 16 | *(unsigned short *)&frame.at24.vertex->posZ;
        frame.at24.vertex = &course->vertices[probe->vertexIndexC];
        frame.at20.i = frame.at24.vertex->posX << 16 | *(unsigned short *)&frame.at24.vertex->posZ;
        frame.at68.i = frame.at8.i;
        frame.at64.i = frame.at20.i;
        frame.at60.i = frame.at12.i;
        frame.at36.pt = frame.at60.zx;
        frame.at44.pt = frame.at64.zx;
        frame.at48.pt = frame.at68.zx;
        frame.at32.s = frame.at44.pt[0] - frame.at36.pt[0];
        frame.at52.s = frame.at44.pt[1] - frame.at36.pt[1];
        frame.at40.s = frame.at48.pt[0] - frame.at36.pt[0];
        frame.at56.s = frame.at48.pt[1] - frame.at36.pt[1];
        probe->edgeCross = frame.at32.s * frame.at56.s - frame.at40.s * frame.at52.s;
        if (probe->edgeCross < 0)
            continue;
        frame.at24.vertex = &course->vertices[probe->vertexIndexB];
        frame.at16.i = frame.at24.vertex->posX << 16 | *(unsigned short *)&frame.at24.vertex->posZ;
        frame.at108.i = frame.at16.i;
        frame.at104.i = frame.at8.i;
        frame.at100.i = frame.at12.i;
        frame.at76.pt = frame.at100.zx;
        frame.at84.pt = frame.at104.zx;
        frame.at88.pt = frame.at108.zx;
        frame.at72.s = frame.at84.pt[0] - frame.at76.pt[0];
        frame.at92.s = frame.at84.pt[1] - frame.at76.pt[1];
        frame.at80.s = frame.at88.pt[0] - frame.at76.pt[0];
        frame.at96.s = frame.at88.pt[1] - frame.at76.pt[1];
        probe->edgeCross = frame.at72.s * frame.at96.s - frame.at80.s * frame.at92.s;
        if (probe->edgeCross < 0)
            continue;
        frame.at148.i = frame.at20.i;
        frame.at144.i = frame.at16.i;
        frame.at140.i = frame.at12.i;
        frame.at116.pt = frame.at140.zx;
        frame.at124.pt = frame.at144.zx;
        frame.at128.pt = frame.at148.zx;
        frame.at112.s = frame.at124.pt[0] - frame.at116.pt[0];
        frame.at132.s = frame.at124.pt[1] - frame.at116.pt[1];
        frame.at120.s = frame.at128.pt[0] - frame.at116.pt[0];
        frame.at136.s = frame.at128.pt[1] - frame.at116.pt[1];
        probe->edgeCross = frame.at112.s * frame.at136.s - frame.at120.s * frame.at132.s;
        if (probe->edgeCross < 0)
            continue;
        break;
    }
    return frame.at28.i;
}

// Stores the triangle index from Mini_SnowboardFindCourseTriangleIndex in the probe and returns 1
// if it is negative. Otherwise it publishes the triangle and probeFlags to the snowboard state and
// returns 0.
// FUNCTION: FF7 0x00727cfd
int __cdecl Mini_SnowboardProbeRow(SnowboardProbe *probe)
{
    probe->triangleIndex = Mini_SnowboardFindCourseTriangleIndex(&probe->deltaX, probe->course, probe);
    if (probe->triangleIndex < 0) {
        return 1;
    }
    g_SnoboStatePtr->triangle = probe->triangleIndex;
    g_SnoboStatePtr->probeFlags = probe->stateFlags;
    return 0;
}

/*
 * Advances the segment index. At course end returns two in normal mode or sets
 * the alternate completion flag and returns zero.
 */
// FUNCTION: FF7 0x00727d5d
int __cdecl Mini_SnowboardAdvanceSegment(SnowboardProbe *probe)
{
    (probe->advanceCount)++;
    if (probe->advanceCount >= probe->courseTable->nodeCount) {
        if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
            return 2;
        }
        g_SnoboCourseEndReached = 1;
        return 0;
    } else {
        Mini_SnowboardFillCourseRecord(probe, (short)probe->advanceCount);
        if (probe->deltaZ > 0) {
            probe->deltaZ = 0;
        }
        probe->triangleIndex = Mini_SnowboardFindCourseTriangleIndex(&probe->deltaX, probe->course, probe);
        if (probe->triangleIndex < 0) {
            return 1;
        } else {
            g_SnoboStatePtr->chainIndex = probe->advanceCount;
            g_SnoboStatePtr->triangle = probe->triangleIndex;
            g_SnoboStatePtr->probeFlags = probe->stateFlags;
            return 0;
        }
    }
}

/*
 * Decrements the segment index, following the linked previous course block
 * when it falls below zero. Returns one when no link exists or row lookup
 * fails.
 */
// FUNCTION: FF7 0x00727e43
int __cdecl Mini_SnowboardRetreatSegment(SnowboardProbe *probe)
{
    (probe->advanceCount)--;
    if (probe->advanceCount < 0) {
        if (probe->courseTable->retreatSegment == 0xFF) {
            return 1;
        }
        probe->segment = probe->courseTable->retreatSegment;
        probe->courseTable = &g_SnoboStatePtr->segmentTable[probe->segment];
        probe->advanceCount = probe->courseTable->nodeCount - 1;
        Mini_SnowboardFillCourseRecord(probe, (short)probe->advanceCount);
        probe->triangleIndex = Mini_SnowboardFindCourseTriangleIndex(&probe->deltaX, probe->course, probe);
        if (probe->triangleIndex < 0) {
            return 1;
        }
        g_SnoboStatePtr->segment = probe->segment;
        g_SnoboStatePtr->segmentPtr = probe->courseTable;
    } else {
        Mini_SnowboardFillCourseRecord(probe, (short)probe->advanceCount);
        probe->triangleIndex = Mini_SnowboardFindCourseTriangleIndex(&probe->deltaX, probe->course, probe);
        if (probe->triangleIndex < 0) {
            return 1;
        }
    }
    g_SnoboStatePtr->chainIndex = probe->advanceCount;
    g_SnoboStatePtr->triangle = probe->triangleIndex;
    g_SnoboStatePtr->probeFlags = probe->stateFlags;
    return 0;
}

// Stores the triangle index from Mini_SnowboardFindCourseTriangleIndex in the probe. A negative
// result returns Mini_SnowboardAdvanceSegment(probe). Otherwise the triangle and probeFlags are
// published to the snowboard state and the function returns 0.
// FUNCTION: FF7 0x00727fbb
int __cdecl Mini_SnowboardProbeRowOrAdvance(SnowboardProbe *probe)
{
    probe->triangleIndex = Mini_SnowboardFindCourseTriangleIndex(&probe->deltaX, probe->course, probe);
    if (probe->triangleIndex < 0) {
        return Mini_SnowboardAdvanceSegment(probe);
    } else {
        g_SnoboStatePtr->triangle = probe->triangleIndex;
        g_SnoboStatePtr->probeFlags = probe->stateFlags;
        return 0;
    }
}

/*
 * Moves the probe into the segment probe->courseTable->linkSegmentA names. Returns 3 when
 * g_SnoboModeFlag is SNOWBOARD_MODE_GOLD_SAUCER and that segment's linkedBlock is set (a locked branch); otherwise writes
 * the segment to probe->segment and returns Mini_SnowboardSelectCourseRecord(probe).
 */
// FUNCTION: FF7 0x00728022
int __cdecl Mini_SnowboardLoadLinkedBlockAt4(SnowboardProbe *probe)
{
    if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER && g_SnoboStatePtr->segmentTable[probe->courseTable->linkSegmentA].linkedBlock != 0) {
        return 3;
    } else {
        probe->segment = probe->courseTable->linkSegmentA;
        return Mini_SnowboardSelectCourseRecord(probe);
    }
}

// Selects the course record for the probe's segment. courseTable becomes
// &segmentTable[segment], advanceCount is cleared, and Mini_SnowboardFillCourseRecord runs once. The
// triangle index is stored in the probe. A negative result returns 1. Otherwise the segment,
// chainIndex, triangle, segmentPtr and probeFlags are published to the snowboard state and it
// returns 0.
// FUNCTION: FF7 0x00728075
int __cdecl Mini_SnowboardSelectCourseRecord(SnowboardProbe *probe)
{
    probe->courseTable = &g_SnoboStatePtr->segmentTable[probe->segment];
    probe->advanceCount = 0;
    Mini_SnowboardFillCourseRecord(probe, (short)probe->advanceCount);
    probe->triangleIndex = Mini_SnowboardFindCourseTriangleIndex(&probe->deltaX, probe->course, probe);
    if (probe->triangleIndex < 0) {
        return 1;
    } else {
        g_SnoboStatePtr->segment = probe->segment;
        g_SnoboStatePtr->chainIndex = probe->advanceCount;
        g_SnoboStatePtr->triangle = probe->triangleIndex;
        g_SnoboStatePtr->segmentPtr = probe->courseTable;
        g_SnoboStatePtr->probeFlags = probe->stateFlags;
        return 0;
    }
}

/*
 * Moves the probe into the segment probe->courseTable->linkSegmentB names. Returns 4 when
 * g_SnoboModeFlag is SNOWBOARD_MODE_GOLD_SAUCER and that segment's linkedBlock is set; otherwise writes the segment to
 * probe->segment and returns Mini_SnowboardSelectCourseRecord(probe).
 */
// FUNCTION: FF7 0x00728148
int __cdecl Mini_SnowboardLoadLinkedBlockAt5(SnowboardProbe *probe)
{
    if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER && g_SnoboStatePtr->segmentTable[probe->courseTable->linkSegmentB].linkedBlock != 0) {
        return 4;
    } else {
        probe->segment = probe->courseTable->linkSegmentB;
        return Mini_SnowboardSelectCourseRecord(probe);
    }
}

/*
 * Stores Mini_SnowboardFindCourseTriangleIndex in probe->triangleIndex. On a negative
 * result, checks the segment courseTable->linkSegmentA names; returns 3 when its
 * linkedBlock is set and g_SnoboModeFlag is SNOWBOARD_MODE_GOLD_SAUCER, otherwise moves the probe there with
 * Mini_SnowboardSelectCourseRecord.
 */
// FUNCTION: FF7 0x0072819b
int __cdecl Mini_SnowboardProbeOrLoadLinkedBlockAt4(SnowboardProbe *probe)
{
    probe->triangleIndex = Mini_SnowboardFindCourseTriangleIndex(&probe->deltaX, probe->course, probe);
    if (probe->triangleIndex < 0) {
        if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER && g_SnoboStatePtr->segmentTable[probe->courseTable->linkSegmentA].linkedBlock != 0) {
            return 3;
        }
        probe->segment = probe->courseTable->linkSegmentA;
        return Mini_SnowboardSelectCourseRecord(probe);
    } else {
        g_SnoboStatePtr->triangle = probe->triangleIndex;
        g_SnoboStatePtr->probeFlags = probe->stateFlags;
        return 0;
    }
}

/*
 * Stores Mini_SnowboardFindCourseTriangleIndex in probe->triangleIndex. On a negative
 * result, checks the segment courseTable->linkSegmentB names; returns 4 when its
 * linkedBlock is set, otherwise moves the probe there with
 * Mini_SnowboardSelectCourseRecord.
 */
// FUNCTION: FF7 0x00728243
int __cdecl Mini_SnowboardLocateCourseTriangle(SnowboardProbe *probe)
{
    probe->triangleIndex = Mini_SnowboardFindCourseTriangleIndex(&probe->deltaX, probe->course, probe);
    if (probe->triangleIndex < 0) {
        if (g_SnoboStatePtr->segmentTable[probe->courseTable->linkSegmentB].linkedBlock != 0) {
            return 4;
        }
        probe->segment = probe->courseTable->linkSegmentB;
        return Mini_SnowboardSelectCourseRecord(probe);
    } else {
        g_SnoboStatePtr->triangle = probe->triangleIndex;
        g_SnoboStatePtr->probeFlags = probe->stateFlags;
        return 0;
    }
}

/*
 * Returns the constant 1 without other source-visible effects.
 */
// FUNCTION: FF7 0x007282e2
int __cdecl Mini_SnowboardCourseRecordSlot8RequestRetry(void)
{
    return 1;
}

// Clears headingX and headingY when the low byte of riderState is 2, or when the signed followAngle
// word is below 0x80. Otherwise it takes the angle from
// Mini_SnowboardReflectHeadingAroundCourse(cursor), reflects values above 0x800 around 0x1000, and
// passes that angle with the action state (motionFlags) to Mini_SnowboardSelectActionFromTable.
// FUNCTION: FF7 0x007282ec
void __cdecl Mini_SnowboardUpdateCourseHeading(SnowboardProbe *cursor)
{
    int angle;

    if (((unsigned short)g_SnoboStatePtr->riderState & 0xff) == 2) {
        g_SnoboStatePtr->velocityX = 0.0f;
        g_SnoboStatePtr->velocityZ = 0.0f;
    } else if (g_SnoboStatePtr->followAngle < 0x80) {
        g_SnoboStatePtr->velocityX = 0.0f;
        g_SnoboStatePtr->velocityZ = 0.0f;
    } else {
        angle = Mini_SnowboardReflectHeadingAroundCourse(cursor);
        if (angle > 0x800) {
            angle = 0x1000 - angle;
        }
        Mini_SnowboardSelectActionFromTable((struct SnowboardActionState *)&g_SnoboStatePtr->motionFlags, angle);
    }
}

/*
 * Resolves the current course block and reflects the current direction angle
 * around a selected course/row angle for mode one or a state-derived angle for
 * modes two through four. Builds a yaw matrix, rotates the direction vector in
 * place and returns its fixed angle.
 */
// FUNCTION: FF7 0x007283a0
int __cdecl Mini_SnowboardReflectHeadingAroundCourse(SnowboardProbe *cursor)
{
    struct {
        GfxMatrix4x4 *matrixPointer;
        float sine, cosine;
        SnowboardMatrix64 matrix;
        float radians;
        SnowboardVec3 target;
        int q, p;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    cursor->courseTable = g_SnoboStatePtr->segmentPtr;
    cursor->advanceCount = g_SnoboStatePtr->chainIndex;
    cursor->course = &g_SnoboStatePtr->courseHeader[cursor->courseTable->blockIndexTable[cursor->advanceCount]];
    frame.p = ArcTangentFixed((int)g_SnoboStatePtr->velocityX, (int)g_SnoboStatePtr->velocityZ) & 0xfff;
    switch (cursor->opResult) {
    case SNOWBOARD_HEADING_QUAD_1:
        frame.q = (&cursor->course->vertices[8])[g_SnoboStatePtr->triangle / 6].bound + cursor->courseTable->angleTable[cursor->advanceCount] + 0x800;
        break;
    case SNOWBOARD_HEADING_QUAD_2:
    case SNOWBOARD_HEADING_QUAD_3:
    case SNOWBOARD_HEADING_QUAD_4:
        frame.q = g_SnoboStatePtr->flags + 0x400;
        break;
    }
    frame.q = ((0x800 - (frame.p - (frame.q & 0x7ff))) << 1) & 0xfff;
    frame.radians = (float)frame.q * 0.00153398083f;
    frame.matrixPointer = (GfxMatrix4x4 *)&frame.matrix;
    frame.sine = (double)(float)sin(frame.radians);
    frame.cosine = (double)(float)cos(frame.radians);
    frame.matrixPointer->_11 = frame.cosine;
    frame.matrixPointer->_12 = 0;
    frame.matrixPointer->_13 = frame.sine;
    frame.matrixPointer->_14 = 0;
    frame.matrixPointer->_21 = 0;
    frame.matrixPointer->_22 = 1;
    frame.matrixPointer->_23 = 0;
    frame.matrixPointer->_24 = 0;
    frame.matrixPointer->_31 = -frame.sine;
    frame.matrixPointer->_32 = 0;
    frame.matrixPointer->_33 = frame.cosine;
    frame.matrixPointer->_34 = 0;
    frame.matrixPointer->_41 = 0;
    frame.matrixPointer->_42 = 0;
    frame.matrixPointer->_43 = 0;
    frame.matrixPointer->_44 = 1;
    RotateVector3ColVec(&frame.matrix, &g_SnoboStatePtr->velocityX, &frame.target);
    *(SnowboardVec3 *)&g_SnoboStatePtr->velocityX = frame.target;
    return frame.q;
}

/*
 * Interpolates a height across the triangle whose three vertices are vertexA,
 * vertexB and vertexC, each a short x at +0, y at +2 and z at +4, at the x and z
 * given by the shorts at point+0 and point+4. It forms the plane normal by two
 * 2D cross products, divides the summed x and z terms by the y term, and
 * returns that plus vertexA's y; when the y term is zero the division is skipped
 * and the result is just vertexA's y.
 */
// FUNCTION: FF7 0x007285ce
int __cdecl Mini_SnowboardInterpolateTriangleHeight(SnowboardHeightPoint *point, SnowboardTriangleVertex *vertexA, SnowboardTriangleVertex *vertexB, SnowboardTriangleVertex *vertexC)
{
    int planeY;
    int numeratorX;
    int numeratorZ;

    numeratorX = ((vertexA->posY - vertexB->posY) * (vertexA->posZ - vertexC->posZ) - (vertexA->posY - vertexC->posY) * (vertexA->posZ - vertexB->posZ)) * (vertexA->posX - point->x);
    planeY = (vertexA->posZ - vertexB->posZ) * (vertexA->posX - vertexC->posX) - (vertexA->posZ - vertexC->posZ) * (vertexA->posX - vertexB->posX);
    numeratorZ = ((vertexA->posX - vertexB->posX) * (vertexA->posY - vertexC->posY) - (vertexA->posX - vertexC->posX) * (vertexA->posY - vertexB->posY)) * (vertexA->posZ - point->z);
    if (planeY != 0) {
        planeY = (numeratorX + numeratorZ) / planeY;
    }
    return planeY + vertexA->posY;
}

// Direction angles: angles[0] is atan2(-z, y), angles[1] zero and angles[2] atan2(x, y). Both go through an
// inline double-returning helper; its argument copies and float-rounded result are the frame's six
// slots (/Ob1 expansion), and atan2 is the x87 _CIatan2 intrinsic.
inline double SnowboardDirectionAngle(float a, float b)
{
    return (float)atan2(a, b);
}

/*
 * Turns a direction (x, y, z) into rotation angles: angles[0] = atan2(-z, y), angles[1] = 0 and
 * angles[2] = atan2(x, y).
 */
// FUNCTION: FF7 0x007286f6
void __cdecl Mini_SnowboardDirectionToAngles(float *direction, float *angles)
{
    angles[0] = SnowboardDirectionAngle(-direction[2], direction[1]);
    angles[1] = 0.0f;
    angles[2] = SnowboardDirectionAngle(direction[0], direction[1]);
}

/*
 * Returns without output for a negative triangle index. Otherwise selects the
 * course block from segment, reads three byte vertex indices from its four-byte
 * triangle record and computes two signed-short edge vectors.
 */
// FUNCTION: FF7 0x00728753
void __cdecl Mini_SnowboardCourseTriangleNormal(int segment, int triangle, SnowboardCourseNormal *normal, SnowboardNormalScratch *probe)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[52];
        int edge2Z;
        struct { unsigned char pad_to_4[0x4]; int edge1Z; };
        struct { unsigned char pad_to_8[0x8]; int edge2Y; };
        struct { unsigned char pad_to_C[0xC]; int edge1Y; };
        struct { unsigned char pad_to_10[0x10]; int edge2X; };
        struct { unsigned char pad_to_14[0x14]; int edge1X; };
        struct { unsigned char pad_to_18[0x18]; SnowboardTriangleVertex *vertexBase; };
        struct { unsigned char pad_to_1C[0x1C]; SnowboardCourseFace *courseTriangle; };
        struct { unsigned char pad_to_20[0x20]; SnowboardTriangleVertex *vertexC; };
        struct { unsigned char pad_to_24[0x24]; SnowboardTriangleVertex *vertexB; };
        struct { unsigned char pad_to_28[0x28]; SnowboardTriangleVertex *vertexA; };
        struct { unsigned char pad_to_2C[0x2C]; SVECTOR rotNormalX; };
    } frame;
#pragma pack(pop)

    if (triangle < 0) {
        return;
    }
    frame.courseTriangle = &g_SnoboStatePtr->courseHeader[g_SnoboStatePtr->segmentPtr->blockIndexTable[segment]].faces[triangle];
    frame.vertexBase = g_SnoboStatePtr->courseHeader[g_SnoboStatePtr->segmentPtr->blockIndexTable[segment]].vertices;
    frame.vertexA = &frame.vertexBase[frame.courseTriangle->vertexIndex[0]];
    frame.vertexB = &frame.vertexBase[frame.courseTriangle->vertexIndex[1]];
    frame.vertexC = &frame.vertexBase[frame.courseTriangle->vertexIndex[2]];
    frame.edge1X = frame.vertexB->posX - frame.vertexA->posX;
    frame.edge1Y = frame.vertexB->posY - frame.vertexA->posY;
    frame.edge1Z = frame.vertexB->posZ - frame.vertexA->posZ;
    frame.edge2X = frame.vertexC->posX - frame.vertexB->posX;
    frame.edge2Y = frame.vertexC->posY - frame.vertexB->posY;
    frame.edge2Z = frame.vertexC->posZ - frame.vertexB->posZ;
    probe->normalX = -(frame.edge1Y * frame.edge2Z - frame.edge1Z * frame.edge2Y) >> 3;
    probe->normalY = frame.edge1Z * frame.edge2X - frame.edge1X * frame.edge2Z >> 3;
    probe->normalZ = -(frame.edge1X * frame.edge2Y - frame.edge1Y * frame.edge2X) >> 3;
    PsxApplyMatrix33ToShortVector(&g_SnoboStatePtr->rotationMatrix, &probe->normalX, &frame.rotNormalX);
    normal->normalX = (float)frame.rotNormalX.vx;
    normal->normalY = (float)frame.rotNormalX.vy;
    normal->normalZ = (float)frame.rotNormalX.vz;
}

/*
 * Builds g_SnoboCameraX/78/7c from scaled context position floats
 * minus integer offsets and g_SnoboCameraPos from angle words scaled
 * by 0.00153398083, normalizing its middle component with
 * Mini_SnowboardWrapCameraAngle2Pi. Converts g_SnoboCourseRecords
 * to a float vector and passes all three to Mini_SnowboardSetCameraPose.
 */
// FUNCTION: FF7 0x0072890f
void __cdecl Mini_SnowboardUpdateCameraFromContext(void)
{
    SnowboardVec3 target;
    g_SnoboCameraX = g_SnoboStatePtr->relPosX * 0.000244140625f - g_SnoboStatePtr->originX;
    g_SnoboCameraY = g_SnoboStatePtr->relPosY * 0.000244140625f - g_SnoboStatePtr->originY;
    g_SnoboCameraZ = g_SnoboStatePtr->riderPos * 0.000244140625f - g_SnoboStatePtr->originZ;
    g_SnoboCameraPos[0] = (float)g_SnoboCameraPosWord[0] * 0.00153398083f;
    g_SnoboCameraPos[1] = Mini_SnowboardWrapCameraAngle2Pi((float)(-g_SnoboStatePtr->rotationY + g_SnoboCameraPosWord[1]) * 0.00153398083f);
    g_SnoboCameraPos[2] = (float)g_SnoboCameraPosWord[2] * 0.00153398083f;
    GteSVectorToFloat3(&g_SnoboCourseRecords, &target);
    Mini_SnowboardSetCameraPose((struct SnowboardCameraAngles *)g_SnoboCameraPos, &g_SnoboCameraX, (struct SnowboardRelativePosition *)&target);
}

/*
 * Wraps a float by repeated addition/subtraction of 6.2831854820251465 into
 * the inclusive 0..2pi interval; an input equal to the upper endpoint is
 * retained.
 */
// FUNCTION: FF7 0x00728a12
float __cdecl Mini_SnowboardWrapCameraAngle2Pi(float angle)
{
    while (angle > 6.2831854820251465f) {
        angle -= 6.2831854820251465f;
    }
    while (angle < 0.0f) {
        angle += 6.2831854820251465f;
    }
    return angle;
}

/*
 * Builds a rotation from the supplied angle vector into the engine matrix,
 * rotates the supplied offset through it and sets matrix translation to target
 * minus that rotated offset. Unreferenced padding slots preserve the VC5 /Od frame size.
 */
// FUNCTION: FF7 0x00728a56
void __cdecl Mini_SnowboardSetCameraPose(SnowboardCameraAngles *angles, void *offset, SnowboardRelativePosition *target)
{
    SnowboardCameraMatrix *matPtr;
    SnowboardRotationAngles vecX;
    int rotMatrix[16];

    matPtr = (SnowboardCameraMatrix *)((GetEngineContext())->cameraMatrix);
    vecX.angleX = angles->angleX;
    vecX.angleY = angles->angleY;
    vecX.angle = angles->angleZ;
    SetRotationMatrix4x4FromAngles_2(&vecX, (struct SnowboardFloatRowB *)(rotMatrix));
    TransposeMatrix3x3(rotMatrix, matPtr);
    RotateVector3RowVec(matPtr, offset, &vecX.angleX);
    matPtr->offsetX = target->posX - vecX.angleX;
    matPtr->offsetY = target->posY - vecX.angleY;
    matPtr->offsetZ = target->posZ - vecX.angle;
}

// Turns the direction vector from Mini_SnowboardWriteRelativePosition into camera angles. Pitch is
// atan2(y, the horizontal length of x and z), yaw is the negated atan2(x, z), and roll is zero. The
// rotation matrix built from those angles is transposed into the engine's cameraMatrix, and the
// translation offsetX, offsetY and offsetZ is then set to 0, 96 and 0.
// FUNCTION: FF7 0x00728ae8
void __cdecl Mini_SnowboardSetCameraFromDirection()
{
    struct {
        float yawRound, pitchRound, sqrtRound, zArg, xArg, yArg, sumRound;
        SnowboardMatrix64 matrix;
        SnowboardCameraMatrix *output;
        SnowboardVec3 rotation;
        float distance;
        SnowboardVec3 direction;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.output = (SnowboardCameraMatrix *)((GetEngineContext())->cameraMatrix);
    Mini_SnowboardWriteRelativePosition((struct SnowboardRelativePosition *)&frame.direction);
    frame.distance = +(frame.sqrtRound = (float)sqrt(frame.sumRound = frame.direction.x * frame.direction.x + frame.direction.z * frame.direction.z));
    frame.yArg = frame.direction.y;
    frame.rotation.x = +(frame.pitchRound = (float)atan2(frame.yArg, frame.distance));
    frame.zArg = frame.direction.z;
    frame.xArg = frame.direction.x;
    frame.rotation.y = -(frame.yawRound = (float)atan2(frame.xArg, frame.zArg));
    frame.rotation.z = 0.0f;
    SetRotationMatrix4x4FromAngles_2((struct SnowboardRotationAngles *)&frame.rotation, (struct SnowboardFloatRowB *)&frame.matrix);
    TransposeMatrix3x3(&frame.matrix, frame.output);
    frame.output->offsetX = 0.0f;
    frame.output->offsetY = 96.0f;
    frame.output->offsetZ = 0.0f;
}

/*
 * Runs Mini_SnowboardSaveRiderSnapshot, conditionally calls
 * Mini_SnowboardUpdateCourseSurface/Mini_SnowboardApproachTargetAngles for
 * the state's running flag, updates g_SnoboRelativePos1 with
 * Mini_SnowboardWriteRelativePosition, calls Mini_SnowboardEmitCourseChain
 * with the indexed context record byte plus one and
 * Mini_SnowboardRunActionFrame with data block 0. When
 * g_SnoboModeFlag is SNOWBOARD_MODE_GOLD_SAUCER and the state's activeFlags is nonzero, calls
 * Mini_SnowboardAppendRiderSample and Mini_SnowboardEmitAnimFrameActions.
 */
// FUNCTION: FF7 0x00728ba3
void __cdecl Mini_SnowboardEmitRiderFrame(void)
{
    Mini_SnowboardSaveRiderSnapshot();
    if (g_SnoboStatePtr->running != 0) {
        Mini_SnowboardUpdateCourseSurface();
    }
    Mini_SnowboardWriteRelativePosition(&g_SnoboRelativePos1);
    Mini_SnowboardEmitCourseChain(1, g_SnoboStatePtr->segmentPtr->chainIndexTable[g_SnoboStatePtr->chainIndex] + 1);
    if (g_SnoboStatePtr->running != 0) {
        Mini_SnowboardApproachTargetAngles();
    }
    Mini_SnowboardRunActionFrame((int *)&g_SnoboRelativePos1.posX, Gfx_GetPsxScratchpadPtr(0), g_SnoboStatePtr->running, 1);
    if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER && g_SnoboStatePtr->activeFlags != 0) {
        Mini_SnowboardAppendRiderSample();
        Mini_SnowboardEmitAnimFrameActions();
    }
}

/*
 * Conditionally calls
 * Mini_SnowboardUpdateCourseSurface/Mini_SnowboardApproachTargetAngles for
 * the state's running flag, updates g_SnoboRelativePos0 with
 * Mini_SnowboardWriteRelativePosition and runs Mini_SnowboardStepCamera on
 * data block 0. Calls Mini_SnowboardEmitCourseChain with the indexed record
 * byte plus one and Mini_SnowboardRunActionFrame; calls
 * Mini_SnowboardEmitAnimFrameActions when g_SnoboModeFlag is SNOWBOARD_MODE_GOLD_SAUCER and
 * the state's activeFlags is nonzero.
 */
// FUNCTION: FF7 0x00728c56
void __cdecl Mini_SnowboardEmitCameraFrame(void)
{
    if (g_SnoboStatePtr->running != 0) {
        Mini_SnowboardUpdateCourseSurface();
    }
    Mini_SnowboardWriteRelativePosition(&g_SnoboRelativePos0);
    Mini_SnowboardStepCamera(Gfx_GetPsxScratchpadPtr(0));
    Mini_SnowboardEmitCourseChain(1, g_SnoboStatePtr->segmentPtr->chainIndexTable[g_SnoboStatePtr->chainIndex] + 1);
    if (g_SnoboStatePtr->running != 0) {
        Mini_SnowboardApproachTargetAngles();
    }
    Mini_SnowboardRunActionFrame((int *)&g_SnoboRelativePos0.posX, Gfx_GetPsxScratchpadPtr(0), g_SnoboStatePtr->running, 1);
    if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER && g_SnoboStatePtr->activeFlags != 0) {
        Mini_SnowboardEmitAnimFrameActions();
    }
}

/*
 * Increments g_SnoboCameraStepCount up to 0x40, subtracts its shifted value from
 * g_SnoboCameraPosWord[2], decreases
 * g_SnoboCourseRecords.cameraOffset[1]/g_SnoboCourseRecords.cameraOffset[2] while above 0x30/0x100, then
 * calls Mini_SnowboardUpdateCameraFromContext through the retained variadic cast.
 */
// FUNCTION: FF7 0x00728d12
void __cdecl Mini_SnowboardStepCamera(int context)
{
    short stepCount;

    stepCount = *(short *)&g_SnoboCameraStepCount;
    if (stepCount < 0x40) {
        stepCount++;
        g_SnoboCameraStepCount = stepCount;
    }
    g_SnoboCameraPosWord[1] -= (short)(stepCount >> 4);
    if (g_SnoboCourseRecords.cameraOffset[1] > 0x30) {
        g_SnoboCourseRecords.cameraOffset[1]--;
    }
    if (g_SnoboCourseRecords.cameraOffset[2] > 0x100) {
        g_SnoboCourseRecords.cameraOffset[2]--;
    }
    /* Retail caller passes -1 arg(s) to 0-parameter `Mini_SnowboardUpdateCameraFromContext` (__cdecl stack cleanup matches retail). */
    ((void(__cdecl *)(...))Mini_SnowboardUpdateCameraFromContext)(context);
}

/*
 * Updates course direction, rider and position data, resolves current and
 * preceding course geometry and copies the three selected surface vertices
 * with seam-height adjustments. Computes fixed-point ground height, updates
 * rider state and sets target orientation to zero for state above eight or
 * derives it through the direction-angle helper otherwise.
 */
// FUNCTION: FF7 0x00728da3
void __cdecl Mini_SnowboardUpdateCourseSurface(void)
{
    int vertexIndex;
    int slotIdx;
    SnowboardCourseBlock *currentBlock;
    SnowboardTriangleVertex *prev;

    Mini_SnowboardCourseTriangleNormal(g_SnoboStatePtr->chainIndex, g_SnoboStatePtr->triangle, &g_SnoboStatePtr->courseNormal, (SnowboardNormalScratch *)Gfx_GetPsxScratchpadPtr(0));
    Mini_SnowboardIntegrateHorizontalMotion((struct SnowboardMotionState *)&g_SnoboStatePtr->motionFlags, Gfx_GetPsxScratchpadPtr(0));
    Mini_SnowboardProbeFloatPositions(&g_SnoboStatePtr->relPosX, &g_SnoboStatePtr->posX);
    g_SnoboStatePtr->section = g_SnoboStatePtr->chainIndex + g_SnoboStatePtr->sectionTable[g_SnoboStatePtr->segment];
    currentBlock = &g_SnoboStatePtr->courseHeader[g_SnoboStatePtr->segmentPtr->blockIndexTable[g_SnoboStatePtr->chainIndex]];
    Mini_SnowboardLoadSegmentVector();
    Mini_SnowboardUpdateHeadingAndCamera(0);
    if (g_SnoboStatePtr->chainIndex > 0) {
        prev = &g_SnoboStatePtr->courseHeader[g_SnoboStatePtr->segmentPtr->blockIndexTable[g_SnoboStatePtr->chainIndex + -1]].vertices[42];
    } else {
        prev = currentBlock->vertices;
    }
    g_SnoboSurfaceSeamVertices = prev;
    for (slotIdx = 0; slotIdx < 3; slotIdx++) {
        vertexIndex = currentBlock->faces[g_SnoboStatePtr->triangle].vertexIndex[slotIdx];
        g_SnoboSurfaceTriangle[slotIdx] = currentBlock->vertices[vertexIndex];
        if (vertexIndex < 7) {
            g_SnoboSurfaceTriangle[slotIdx].posY = g_SnoboSurfaceSeamVertices[vertexIndex].posY - g_SnoboSurfaceSeamVertices->posY;
        }
    }
    g_SnoboStatePtr->terrainHeight = (float)(Mini_SnowboardInterpolateTriangleHeight((SnowboardHeightPoint *)&g_SnoboStatePtr->probeDeltaX, g_SnoboSurfaceTriangle, &g_SnoboSurfaceTriangle[1], &g_SnoboSurfaceTriangle[2]) + g_SnoboStatePtr->segmentPtr->vectors[g_SnoboStatePtr->chainIndex].y << 0xC);
    Mini_SnowboardIntegrateVerticalMotion(&g_SnoboStatePtr->motionFlags);
    if (g_SnoboStatePtr->motionStage > 8) {
        g_SnoboStatePtr->targetAngle1 = 0.0f;
        g_SnoboStatePtr->targetAngle0 = 0.0f;
    } else {
        Mini_SnowboardDirectionToAngles(&g_SnoboStatePtr->courseNormal.normalX, &g_SnoboStatePtr->targetAngle0);
    }
}

// Selects the course block of the current node through segmentPtr->blockIndexTable. It
// stores the 12-bit heading in flags and the rotationY word, calling Matrix30SetRotationY for each.
// Mini_SnowboardComputeRelativeCameraPos runs with data block 0. A nonzero mode then calls
// Mini_SnowboardCameraModeHookNoOp with the mode, and zero calls Mini_SnowboardUpdateRiderFollowCamera.
// FUNCTION: FF7 0x0072904d
void __cdecl Mini_SnowboardUpdateHeadingAndCamera(int mode)
{
    SnowboardStateRecord *record;

    record = (SnowboardStateRecord *)&g_SnoboStatePtr->courseHeader[g_SnoboStatePtr->segmentPtr->blockIndexTable[g_SnoboStatePtr->chainIndex]];
    g_SnoboStatePtr->flags = (short)Mini_SnowboardComputeHeading((struct SnowboardHeadingPos *)&g_SnoboStatePtr->probeDeltaX, record) & 0xFFF;
    Matrix30SetRotationY((short)g_SnoboStatePtr->flags, &g_SnoboStatePtr->headingMatrix);
    g_SnoboStatePtr->rotationY = g_SnoboStatePtr->segmentPtr->angleTable[g_SnoboStatePtr->chainIndex];
    Matrix30SetRotationY((short)g_SnoboStatePtr->rotationY, &g_SnoboStatePtr->rotationMatrix);
    Mini_SnowboardComputeRelativeCameraPos((SnowboardHeightPoint *)&g_SnoboStatePtr->probeDeltaX, record, (SnowboardRelativeCamera *)Gfx_GetPsxScratchpadPtr(0));
    if (mode != 0) {
        /* Retail caller passes -1 arg(s) to 0-parameter `Mini_SnowboardCameraModeHookNoOp` (__cdecl stack cleanup matches retail). */
        ((void(__cdecl *)(...))Mini_SnowboardCameraModeHookNoOp)(Gfx_GetPsxScratchpadPtr(0), mode);
    } else {
        /* Retail caller passes -1 arg(s) to 0-parameter `Mini_SnowboardUpdateRiderFollowCamera` (__cdecl stack cleanup matches retail). */
        ((void(__cdecl *)(...))Mini_SnowboardUpdateRiderFollowCamera)(Gfx_GetPsxScratchpadPtr(0));
    }
}

// Sets heading from ArcTangentFixed of pos->offsetZ against a horizontal distance built from
// record->turn. When turn is positive, heading is the negated angle and the distance is turn plus
// offsetX. When turn is negative, the distance is -turn plus (centerX minus offsetX). When turn is
// zero, heading is zero. Returns heading added to segmentPtr->angleTable[chainIndex].
// FUNCTION: FF7 0x0072917e
int __cdecl Mini_SnowboardComputeHeading(SnowboardHeadingPos *pos, SnowboardStateRecord *record)
{
    unsigned short baseAngle;

    baseAngle = g_SnoboStatePtr->segmentPtr->angleTable[g_SnoboStatePtr->chainIndex];
    if (record->turn > 0) {
        g_SnoboStatePtr->heading = -(short)ArcTangentFixed(pos->offsetZ, record->turn + pos->offsetX);
    } else if (record->turn < 0) {
        g_SnoboStatePtr->heading = ArcTangentFixed(pos->offsetZ, -record->turn + (record->centerX - pos->offsetX));
    } else {
        g_SnoboStatePtr->heading = 0;
    }
    return g_SnoboStatePtr->segmentPtr->angleTable[g_SnoboStatePtr->chainIndex] + g_SnoboStatePtr->heading;
}

/*
 * Derives two course-height centers relative to current position and
 * interpolates between them using probe parameters. Builds a rotated probe
 * offset for nonzero curvature or a direct offset otherwise, then clamps
 * computed Y against rider-relative Y plus 0x40 and stores the three relative
 * position floats.
 */
// FUNCTION: FF7 0x00729268
void __cdecl Mini_SnowboardComputeRelativeCameraPos(SnowboardHeightPoint *riderPos, SnowboardStateRecord *record, SnowboardRelativeCamera *relCam)
{
    int turn;
    int midY;
    int farY;
    int startHeight;
    int baseHeight;
    int yClamped;
    SnowboardCourseEntry *entPtr;
    int height1;

    baseHeight = g_SnoboStatePtr->segmentPtr->vectors[g_SnoboStatePtr->chainIndex].y;
    entPtr = (SnowboardCourseEntry *)g_SnoboStatePtr->courseHeader[g_SnoboStatePtr->segmentPtr->blockIndexTable[g_SnoboStatePtr->chainIndex]].vertices;
    startHeight = baseHeight;
    height1 = baseHeight + entPtr->height;
    midY = (startHeight + height1 >> 1) - g_SnoboStatePtr->originY;
    startHeight = baseHeight + entPtr->startHeight;
    height1 = baseHeight + entPtr->heightOffset;
    farY = (startHeight + height1 >> 1) - g_SnoboStatePtr->originY;
    turn = record->turn;
    if (turn != 0) {
        relCam->offsetY = 0;
        relCam->riderZ = 0;
        Matrix30SetRotationY((short)g_SnoboStatePtr->heading, &relCam->matrix);
        if (turn > 0) {
            relCam->yawOffset = turn + record->turnOffset;
            PsxApplyMatrix33ToShortVector(&relCam->matrix, &relCam->yawOffset, &relCam->yawOffset);
            relCam->yawOffset -= turn;
        } else {
            turn = -turn + record->turnOffset;
            relCam->yawOffset = -(short)turn;
            PsxApplyMatrix33ToShortVector(&relCam->matrix, &relCam->yawOffset, &relCam->yawOffset);
            relCam->yawOffset += (short)(turn + record->turnOffset);
        }
        midY = (farY - midY) * abs(g_SnoboStatePtr->heading) / record->headingRange + midY;
    } else {
        relCam->yawOffset = record->turnOffset;
        relCam->riderZ = riderPos->z;
        midY = (farY - midY) * riderPos->z / record->riderRange + midY;
    }
    farY = (int)(g_SnoboStatePtr->relPosY * 0.00024414062f) - g_SnoboStatePtr->originY + 0x40;
    if (midY < farY) {
        yClamped = midY;
    } else {
        yClamped = farY;
    }
    g_SnoboCameraY = (float)yClamped;
    g_SnoboCameraX = g_SnoboStatePtr->relPosX * 0.00024414062f - g_SnoboStatePtr->originX;
    g_SnoboCameraZ = g_SnoboStatePtr->riderPos * 0.00024414062f - g_SnoboStatePtr->originZ;
}

/*
 * Returns when the current course record index is negative. For selected
 * motion kinds, derives a 12-bit target heading from velocity or current
 * heading, flips opposite headings and limits the heading difference before
 * smoothing model rotation with Mini_SnowboardApproachAngle.
 */
// FUNCTION: FF7 0x00729517
void __cdecl Mini_SnowboardUpdateRiderFollowCamera(void)
{
    struct {
        SnowboardVec3 position;
        float step;
        float initialAngle;
        float *rotation;
        SnowboardVec3 target;
        SnowboardMatrix64 matrix;
        int angle;
        struct GfxEngineContext *context;
        float delta;
        float projected[4];
        int difference;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.initialAngle = (float)g_SnoboStatePtr->flags * 0.0015339808305725455f;
    if (g_SnoboStatePtr->chainIndex < 0)
        return;
    frame.rotation = g_SnoboCameraPos;
    switch (g_SnoboSelectorTable.selector[(unsigned short)g_SnoboStatePtr->riderState & 0xff]) {
    case SNOWBOARD_FOLLOW_CAM_STANDARD:
        frame.angle = g_SnoboStatePtr->followAngle > 0x80
                          ? (frame.angle = ArcTangentFixed(-(int)g_SnoboStatePtr->velocityX, -(int)g_SnoboStatePtr->velocityZ) & 0xfff)
                          : g_SnoboStatePtr->flags;
        break;
    case SNOWBOARD_FOLLOW_CAM_CLOSE:
        frame.angle = g_SnoboStatePtr->flags;
        break;
    default:
        /* goto kept: the default case jumps to a stub after the function end that jumps on; a structured form jumps directly */
        goto updatePosition;
    }
    frame.difference = (frame.angle - g_SnoboStatePtr->flags) & 0xfff;
    if (frame.difference > 0x400 && frame.difference < 0xc00)
        frame.angle = (frame.angle + 0x800) & 0xfff;
    frame.difference = (frame.angle - g_SnoboStatePtr->flags) & 0xfff;
    if (frame.difference > 0x200 && frame.difference < 0x800)
        frame.angle = g_SnoboStatePtr->flags + 0x200;
    else if (frame.difference > 0x800 && frame.difference < 0xe00)
        frame.angle = g_SnoboStatePtr->flags - 0x200;
    frame.difference = g_SnoboStatePtr->followAngle >> 4;
    if (frame.difference < 4)
        frame.difference = 4;
    frame.angle = (0x800 - frame.angle) & 0xfff;
    frame.delta = (float)frame.angle * 0.0015339808305725455f;
    frame.step = (float)frame.difference * 0.0015339808305725455f;
    frame.rotation[1] = Mini_SnowboardApproachAngle(frame.delta, frame.rotation[1], frame.step);
updatePosition:
    if ((g_SnoboStatePtr->segmentPtr->chainBitTable[g_SnoboStatePtr->chainIndex] & 8) != 0)
        frame.angle = 0x20;
    else if (g_SnoboStatePtr->motionStage > 8)
        frame.angle = 0x80;
    else
        frame.angle = g_SnoboStatePtr->courseHeader[g_SnoboStatePtr->segmentPtr->blockIndexTable[g_SnoboStatePtr->chainIndex]].cameraPitch * 3;
    frame.angle = frame.angle + 0x40;
    frame.angle = frame.angle - (int)(frame.rotation[0] * 651.8986206054688f);
    if (frame.angle > 4)
        frame.angle = 4;
    if (frame.angle < -4)
        frame.angle = -4;
    frame.rotation[0] += (float)frame.angle * 0.0015339808305725455f;
    frame.rotation[2] = 0.0f;
    frame.target.x = 0.0f;
    frame.target.y = 128.0f;
    frame.target.z = 320.0f;
    Mini_SnowboardSetCameraPose((struct SnowboardCameraAngles *)g_SnoboCameraPos, &g_SnoboCameraX, (struct SnowboardRelativePosition *)&frame.target);
    SetIdentityMatrix4x4(& frame.matrix);
    Mini_SnowboardWriteRelativePosition((struct SnowboardRelativePosition *)&frame.position);
    g_SnoboStatePtr->clipper->setMatrix((int)&frame.matrix);
    g_SnoboStatePtr->clipper->transformProject((int)&frame.position, (int)&frame.projected);
    frame.context = GetEngineContext();
    frame.step = (float)frame.context->viewSizeY * 0.8500000238418579f;
    frame.delta = frame.projected[1] - (float)frame.context->clipTop;
    if (frame.delta < 0.0f)
        frame.delta = -frame.delta;
    else if (frame.delta > frame.step)
        frame.delta = -(frame.delta - frame.step);
    else
        frame.delta = 0.0f;
    ((SnowboardCameraMatrix *)frame.context->cameraMatrix)->offsetY += frame.delta;
}

/*
 * Empty function; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x0072990d
void __cdecl Mini_SnowboardCameraModeHookNoOp(void)
{
}

/*
 * Fades the renderer at g_pSnoboRenderer to white with alpha from alpha's
 * low byte (SnowboardRenderer::fadeWhite).
 */
// FUNCTION: FF7 0x00729912
void __cdecl Mini_SnowboardRendererFadeWhite(int alpha)
{
    g_pSnoboRenderer->fadeWhite(alpha);
}

/*
 * Fades the renderer at g_pSnoboRenderer to black with alpha from alpha's
 * low byte (SnowboardRenderer::fadeBlack).
 */
// FUNCTION: FF7 0x00729926
void __cdecl Mini_SnowboardRendererFadeBlack(int alpha)
{
    g_pSnoboRenderer->fadeBlack(alpha);
}

/*
 * Fills the renderer at g_pSnoboRenderer with packed color color
 * (SnowboardRenderer::fillColor).
 */
// FUNCTION: FF7 0x0072993a
void __cdecl Mini_SnowboardRendererFillColor(int color)
{
    g_pSnoboRenderer->fillColor(color);
}

/*
 * Computes a collision score as thirty minus one-third of the counter clamped
 * at zero, derives a time score from split-time outputs and the course
 * reference clamped to zero through thirty, and caps the third score at forty.
 * Stores all three component scores and their sum; split-time output depends
 * on the retained contiguous local frame layout.
 */
// FUNCTION: FF7 0x0072994e
void __cdecl Mini_SnowboardComputeResultScores(void)
{
    int timeDigits[7];
    int partScore;
    int pickupScore;

    partScore = 0x1E - g_SnoboStatePtr->spinCount / 3;
    if (partScore < 0) {
        partScore = 0;
    }
    g_SnoboStatePtr->resultPartA = partScore;
    g_pSnoboRenderer->splitTime((int)&g_SnoboStatePtr->courseTime, (int)timeDigits);
    partScore = (timeDigits[3] + timeDigits[2] * 0xA + timeDigits[1] * 0x3C + timeDigits[0] * 0x258 - g_SnoboSelectorTable.triggerStageWeights[g_SnoboStatePtr->triggerStage]) / 2;
    partScore = 0x1E - partScore;
    if (partScore > 0x1E) {
        partScore = 0x1E;
    } else if (partScore < 0) {
        partScore = 0;
    }
    g_SnoboStatePtr->resultPartB = partScore;
    if (g_SnoboStatePtr->pickupTotal > 0x28) {
        pickupScore = 0x28;
    } else {
        pickupScore = g_SnoboStatePtr->pickupTotal;
    }
    g_SnoboStatePtr->resultPartC = pickupScore;
    g_SnoboStatePtr->resultTotal = g_SnoboStatePtr->resultPartA + g_SnoboStatePtr->resultPartB + g_SnoboStatePtr->resultPartC;
}

// Copies segment, chainIndex and segmentPtr into data block 0 and sets p1X and p1Z from posX and
// posZ, converted to integers and shifted right twelve. It runs Mini_SnowboardFillCourseRecord once
// and saves deltaZ as zSampleA. It then repeats with relPosX and riderPos for zSampleB, and
// countDelta is the absolute difference of zSampleA and zSampleB.
// FUNCTION: FF7 0x00729a71
void __cdecl Mini_SnowboardMeasureCountDelta(void)
{
    SnowboardProbe *rec;
    rec = (SnowboardProbe *)Gfx_GetPsxScratchpadPtr(0);
    rec->segment = g_SnoboStatePtr->segment;
    rec->advanceCount = g_SnoboStatePtr->chainIndex;
    rec->courseTable = g_SnoboStatePtr->segmentPtr;
    rec->p1X = (int)g_SnoboStatePtr->posX >> 12;
    rec->p1Z = (int)g_SnoboStatePtr->posZ >> 12;
    Mini_SnowboardFillCourseRecord(rec, rec->advanceCount);
    rec->zSampleA = rec->deltaZ;
    rec->p1X = (int)g_SnoboStatePtr->relPosX >> 12;
    rec->p1Z = (int)g_SnoboStatePtr->riderPos >> 12;
    Mini_SnowboardFillCourseRecord(rec, rec->advanceCount);
    rec->zSampleB = rec->deltaZ;
    rec->countDelta = abs(rec->zSampleA - rec->zSampleB);
}

/*
 * Builds sound commands in the buffer at g_SnoboStatePtr->soundCommand and
 * dispatches them twice for channel channel (1 through 4). First it writes
 * command id 0x27 + channel with parameters 0x40 and pitch and calls
 * SoundDispatchCommand; then it writes id 0x9F + channel with parameter volume, or
 * 0x7F when volume is zero, and dispatches again.
 */
// FUNCTION: FF7 0x00729b81
void __cdecl Mini_SnowboardPlaySfxOnChannel(int channel, int pitch, int volume)
{
    switch (channel) {
    case SOUND_SFX_CHANNEL_1:
        g_SnoboStatePtr->soundCommand->command = SOUND_SFX_PLAY_CH1;
        break;
    case SOUND_SFX_CHANNEL_2:
        g_SnoboStatePtr->soundCommand->command = SOUND_SFX_PLAY_CH2;
        break;
    case SOUND_SFX_CHANNEL_3:
        g_SnoboStatePtr->soundCommand->command = SOUND_SFX_PLAY_CH3;
        break;
    case SOUND_SFX_CHANNEL_4:
        g_SnoboStatePtr->soundCommand->command = SOUND_SFX_PLAY_CH4;
    }
    g_SnoboStatePtr->soundCommand->volume = 0x40;
    g_SnoboStatePtr->soundCommand->pitch = pitch;
    SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, g_SnoboStatePtr->soundCommand->pitch, 0, 0, 0, 0, 0, 0);
    switch (channel) {
    case SOUND_SFX_CHANNEL_1:
        g_SnoboStatePtr->soundCommand->command = SOUND_SFX_CH1_VOLUME;
        break;
    case SOUND_SFX_CHANNEL_2:
        g_SnoboStatePtr->soundCommand->command = SOUND_SFX_CH2_VOLUME;
        break;
    case SOUND_SFX_CHANNEL_3:
        g_SnoboStatePtr->soundCommand->command = SOUND_SFX_CH3_VOLUME;
        break;
    case SOUND_SFX_CHANNEL_4:
        g_SnoboStatePtr->soundCommand->command = SOUND_SFX_CH4_VOLUME;
    }
    if (volume != 0) {
        g_SnoboStatePtr->soundCommand->volume = volume;
        SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, 0, 0, 0, 0, 0, 0, 0);
    } else {
        g_SnoboStatePtr->soundCommand->volume = 0x7F;
        SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, 0, 0, 0, 0, 0, 0, 0);
    }
}

/*
 * Resets the music and sound state: MidiSetVolume(0x7F), then three
 * SoundDispatchCommand calls built in the buffer at
 * g_SnoboStatePtr->soundCommand -- SOUND_SFX_PLAY_4 with parameters 0x40 and
 * four zeroes, SOUND_SFX_ALL_VOLUME with 0x7F, and SOUND_SFX_ALL_TRANSPOSE with 0.
 */
// FUNCTION: FF7 0x00729d78
void __cdecl Mini_SnowboardResetSoundState(void)
{
    MidiSetVolume(0x7F);
    g_SnoboStatePtr->soundCommand->command = SOUND_SFX_PLAY_4;
    g_SnoboStatePtr->soundCommand->volume = 0x40;
    g_SnoboStatePtr->soundCommand->pitch = 0;
    g_SnoboStatePtr->soundCommand->param3 = 0;
    g_SnoboStatePtr->soundCommand->param4 = 0;
    g_SnoboStatePtr->soundCommand->param5 = 0;
    SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, g_SnoboStatePtr->soundCommand->pitch, g_SnoboStatePtr->soundCommand->param3, g_SnoboStatePtr->soundCommand->param4, g_SnoboStatePtr->soundCommand->param5, 0, 0, 0);
    g_SnoboStatePtr->soundCommand->command = SOUND_SFX_ALL_VOLUME;
    g_SnoboStatePtr->soundCommand->volume = 0x7F;
    SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, 0, 0, 0, 0, 0, 0, 0);
    g_SnoboStatePtr->soundCommand->command = SOUND_SFX_ALL_TRANSPOSE;
    g_SnoboStatePtr->soundCommand->volume = 0;
    SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, 0, 0, 0, 0, 0, 0, 0);
}

/*
 * Fades all sound effects to volume 0 over 0x1E steps: writes SOUND_SFX_ALL_VOLUME_TRANS
 * into the sound command buffer at g_SnoboStatePtr->soundCommand and calls
 * SoundDispatchCommand once.
 */
// FUNCTION: FF7 0x00729f0f
void __cdecl Mini_SnowboardBeginSoundFade(void)
{
    g_SnoboStatePtr->soundCommand->command = SOUND_SFX_ALL_VOLUME_TRANS;
    g_SnoboStatePtr->soundCommand->volume = 0x1E;
    g_SnoboStatePtr->soundCommand->pitch = 0;
    SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, g_SnoboStatePtr->soundCommand->pitch, 0, 0, 0, 0, 0, 0);
}

/*
 * Starts MIDI song 0x2C at volume 0x7F via MidiPlaySong in the Gold Saucer
 * mode, and does nothing otherwise.
 */
// FUNCTION: FF7 0x00729f8b
void __cdecl Mini_SnowboardStartSong(void)
{
    if (g_SnoboModeFlag == SNOWBOARD_MODE_GOLD_SAUCER) {
        MidiPlaySong(0x2C, 0, 0x7F);
    }
}

// Samples bit 2 of the chainBitTable byte at chainIndex and compares it with the value cached in
// g_SnoboLastSfxBits. On a change it updates the cache and dispatches command 0xC1 through
// soundCommand. The volume is 0x1E and the pitch 1 when the bit is set, or volume 8 and pitch 0x7F
// when it is clear. The sound state therefore changes only on the transition.
// FUNCTION: FF7 0x00729fa7
void __cdecl Mini_SnowboardSyncSfxOnTableBit(void)
{
    unsigned char val;

    val = g_SnoboStatePtr->segmentPtr->chainBitTable[g_SnoboStatePtr->chainIndex] & 4;
    if (val != g_SnoboLastSfxBits) {
        g_SnoboLastSfxBits = val;
        if (g_SnoboLastSfxBits != 0) {
            g_SnoboStatePtr->soundCommand->volume = 0x1E;
            g_SnoboStatePtr->soundCommand->pitch = 1;
        } else {
            g_SnoboStatePtr->soundCommand->volume = 8;
            g_SnoboStatePtr->soundCommand->pitch = 0x7F;
        }
        g_SnoboStatePtr->soundCommand->command = SOUND_MUSIC_VOLUME_TRANS;
        SoundDispatchCommand((short)g_SnoboStatePtr->soundCommand->command, g_SnoboStatePtr->soundCommand->volume, g_SnoboStatePtr->soundCommand->pitch, 0, 0, 0, 0, 0, 0);
    }
}

/*
 * Every sixteenth tick -- when the low four bits of the unsigned short
 * tick are zero -- calls MidiSetTempo with bits 5
 * through 10 of followAngle, that is (value >>
 * 5) & 0x3F.
 */
// FUNCTION: FF7 0x0072a0a0
void __cdecl Mini_SnowboardUpdateMidiTempo(void)
{
    if ((g_SnoboStatePtr->tick & 0xF) == 0) {
        MidiSetTempo(g_SnoboStatePtr->followAngle >> 5 & 0x3F);
    }
}

/*
 * For non-race mode returns result codes 11/10/9/8 by unsigned score
 * thresholds 100/70/30. For race mode scans the selected unsigned time-
 * threshold table until current time is less than a threshold and returns that
 * index; the scan has no explicit bound.
 */
// FUNCTION: FF7 0x0072a0d3
int __cdecl Mini_SnowboardClassifyResult(void)
{
    int value;
    int i;

    if (g_SnoboStatePtr->activeFlags == 0) {
        value = g_SnoboStatePtr->resultTotal;
        if ((unsigned int)value >= 100) {
            return 11;
        }
        if ((unsigned int)value >= 70) {
            return 10;
        }
        if ((unsigned int)value >= 30) {
            return 9;
        }
        return 8;
    }
    value = g_SnoboStatePtr->courseTime;
    for (i = 0;; i++) {
        if ((unsigned int)value >= g_SnoboThresholdRecords[g_SnoboStatePtr->triggerStage - 1][i]) {
            continue;
        }
        break;
    }
    return i;
}
} // extern "C"
