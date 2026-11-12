// src/ff7/submarine/sub_module_lifecycle.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Runs the submarine minigame's lifecycle: entering the module, setting up state, tables
//   and sound, the per-frame loop and frame-rate limiting, and exit cleanup.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/mem.h"
#include "ff7/api/main.h"
#include "ff7/api/submarine.h"
#include "lib/api/file.h"
#include "lib/api/sound.h"
#define FF7_SETPERSPECTIVE_NARROW_ABI
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "ff7/types/submarine.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include "lib/types/graphics.h"
#include "ff7/enums/main.h"
#include "lib/codegen.h"
#include <stdlib.h>
#include <windows.h>
#include "lib/pfile.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include "ff7/gte.h"
#include "ff7/savemap.h"
#include "ff7/enums/submarine.h"
#include "lib/com_abi.h"
#include "lib/enums/graphics.h"

struct GfxEngineContext;
extern "C" {
#pragma intrinsic(abs)

#pragma intrinsic(strcpy, strcat)
// clang-format off
// GLOBAL: FF7 0x00e73f00
int g_SubmarineViewportTop = 0;
// GLOBAL: FF7 0x00e73f04
int g_SubmarineViewportLeft = 0;
// GLOBAL: FF7 0x00e73f08
int g_SubmarineViewportWidth = 0;
// GLOBAL: FF7 0x00e73f0c
int g_SubmarineViewportHeight = 0;
// GLOBAL: FF7 0x00e73f10
struct SubmarineTargetDims g_SubmarineTargetDims = {0};
// Submarine_FrameRateLimit's verdict on the last frame: 1 when it finished inside the frame interval (it
// waited out the rest), 0 when it ran late, which makes the frame loop run another update at once.
// GLOBAL: FF7 0x00e73f18
int g_SubmarineFrameOnTime = 0;
unsigned char g_bssPad_00e73f1c[4] = {0};
// The leading-zero count table that Bit_InitLeadingZeroCountTable fills.
// GLOBAL: FF7 0x00e73f20
struct BitCountTableEntry g_SubmarineGridBuffer[256] = {0};

// GLOBAL: FF7 0x00e74320
int g_SubmarineScreenHeight = 0;
// GLOBAL: FF7 0x00e74324
int g_SubmarineRenderMode = 0;
// How far the last late frame ran past the frame interval, in timestamp ticks.
typedef union {
    double ticks;
    int words[2]; /* cleared word by word */
} SubmarineFrameExcess;
// GLOBAL: FF7 0x00e74328
SubmarineFrameExcess g_SubmarineAccumFrameTime = {0.0};
// GLOBAL: FF7 0x00e74330
int g_SubmarineViewportCenterY = 0;
// GLOBAL: FF7 0x00e74334
int g_SubmarineViewportCenterX = 0;
// GLOBAL: FF7 0x00e74338
Timestamp64 g_SubmarinePrevTimestamp = {0};
// GLOBAL: FF7 0x00e74340
int g_SubmarineActiveFlag = 0;
unsigned char g_bssPad_00e74344[4] = {0};
// GLOBAL: FF7 0x00e74348
Timestamp64 g_SubmarineFrameTimestamp = {0};
// GLOBAL: FF7 0x00e74350
double g_SubFrameInterval = 0;
// The leading-one count table that Bit_InitLeadingOneCountTable fills; its last four entries are
// the four words that follow.
// GLOBAL: FF7 0x00e74358
struct BitCountTableEntry g_SubmarineSceneBuffer[252] = {0};
// GLOBAL: FF7 0x00e74748
int g_SubmarineLeadingOnesEntry252 = 0;
// GLOBAL: FF7 0x00e7474c
int g_SubmarineLeadingOnesEntry253 = 0;
// GLOBAL: FF7 0x00e74750
int g_SubmarineLeadingOnesEntry254 = 0;
// GLOBAL: FF7 0x00e74754
int g_SubmarineLeadingOnesEntry255 = 0;
// GLOBAL: FF7 0x00e74758
int g_SubmarineGameState = 0;
// GLOBAL: FF7 0x00e7475c
int g_SubmarineFlags = 0;
// Which submarine game runs, copied from gSavemap.submarineMissionId on entry: 0 starts
// g_SubmarineCountdown at 900 and music 0x46 and ends when the resource reaches 0, nonzero 1200 and 0x58 (and
// Select pauses); it also picks the sound setup sequence and the data blocks.
// GLOBAL: FF7 0x00e74760
unsigned char g_SubmarineMissionId = 0;
// 16-bit result word that Submarine_ModuleExit splits into the savemap's low and high bytes.
typedef struct SubmarineAudioState {
    unsigned short word;
    unsigned char pad_02[2];
} SubmarineAudioState;
// GLOBAL: FF7 0x00e74764
SubmarineAudioState g_SubmarineAudioState = {0};
// GLOBAL: FF7 0x00e74768
int g_SubmarineResultCode = 0;
// GLOBAL: FF7 0x00e7476c
int g_SubmarineControlFlags = 0;
// GLOBAL: FF7 0x00e74770
int g_SubmarineRemainingTime = 0;
// GLOBAL: FF7 0x00e74774
short g_SubmarineSubState = 0;
unsigned char g_bssPad_00e74778[24] = {0};
// GLOBAL: FF7 0x00e74790
int g_SubmarineDebugKeyHeld = 0;
unsigned char g_bssPad_00e74794[4] = {0};
unsigned char g_bssPad_00e74798[4] = {0};
// GLOBAL: FF7 0x00e7479c
int g_SubmarineViewportShrink = 0;
// GLOBAL: FF7 0x00e747a0
int g_SubmarineExcessFlag = 0;
// Empty directory strings for the load options of Submarine_ModuleEnter that read from sub.lgp.
// GLOBAL: FF7 0x00e747a4
char g_SubmarineEmptyDirectory[4] = {0};
// GLOBAL: FF7 0x00e747a8
char g_SubmarineEmptyDirectories[4][4] = {0};
// GLOBAL: FF7 0x00e747b8
struct GfxRenderObject *g_SubmarineLineBatchList = 0;
unsigned char g_bssPad_00e747bc[4] = {0};
// GLOBAL: FF7 0x00e747c0
unsigned char g_SubmarineTerrainMeshBuffer[145920] = {0};
// GLOBAL: FF7 0x00e981c0
int g_SubmarineEffectCount0 = 0;
// GLOBAL: FF7 0x00e981c4
int g_SubmarineEffectCount1 = 0;
// GLOBAL: FF7 0x00e981c8
int g_SubmarineActiveSlotCount = 0;
// GLOBAL: FF7 0x00e981cc
int g_SubmarineSlotTotalWeight = 0;
// GLOBAL: FF7 0x00e981d0
unsigned char g_SubmarineEffectSlotPad[8] = {0};
// clang-format on

#define g_SubmarineInterpStartAngleAHigh (*(short *)((char *)&g_SubmarineRecordInterpBaseA + 2))
#define g_SubmarineInterpTargetAngleAHigh (*(short *)((char *)&g_SubmarineRecordInterpTargetA + 2))
#define g_SubmarineInterpDeltaAngleAHigh (*(short *)((char *)&g_SubmarineRecordInterpDeltaA + 2))

#define g_SubmarineRenderBatchYawAngle (*(short *)((char *)&g_SubmarineRenderBatchState + 2))

/* Integer 0 for offsetX/offsetY emits VC5's 2-byte push 0 (6A 00) instead of 5-byte float push. */
extern void __cdecl SetPerspective(float fov, float nearPlane, float farPlane, int offsetXZero, int offsetYZero, float viewWidth, float viewHeight, struct GfxEngineContext *context);
void __cdecl Submarine_BuildProjectedStripBatches(void);
void __cdecl Submarine_DrawVerticalSliceQuads(void);

// clang-format off
// The TU's initialized .data (0x980DA8..0x980DD8) and function $SG literals share one #pragma data_seg(".data")
// section so the eight zero-initialized ints stay in .data ahead of the literal stream (vc5-data-rules 1).
#pragma data_seg(".data")
// GLOBAL: FF7 0x00980da8
int g_SubmarineFrameLimiterEnabled = 1;
// GLOBAL: FF7 0x00980dac
int g_SubmarineGameRunning = 1;
// GLOBAL: FF7 0x00980db0
int g_SubmarineTimerActive = 0;
// GLOBAL: FF7 0x00980db4
int g_SubmarineBaseTime = 0;
// GLOBAL: FF7 0x00980db8
struct SubmarineTimeOffsets g_SubmarineTimeOffsets = {0};
// GLOBAL: FF7 0x00980dc0
int g_SubmarineAlertState = 0;
// GLOBAL: FF7 0x00980dc4
int g_SubmarineMinutesElapsed = 0;
// GLOBAL: FF7 0x00980dc8
int g_SubmarineElapsedFrames = 0;
// GLOBAL: FF7 0x00980dcc
int g_SubmarineTimeRemaining = 0;
// GLOBAL: FF7 0x00980dd0
int g_SubmarineTimeLimit = 36000;
// GLOBAL: FF7 0x00980dd4
int g_SubmarineCountdown = 900;
unsigned char g_rawData_00980dd8[4] = {0xff, 0x05, 0x00, 0x00};
// clang-format on

// Enters the submarine minigame: logs the begin banner, initializes state, captures the mode byte,
// sets perspective/matrix layout and asset source zero, opens sub.lgp in archive slot 17 and
// prepares sixteen load-option blocks. Creates HUD/text TIM batches in multiple blend/type variants
// and eleven unnamed primitive render objects in the context's render-object bank, then closes the archive and builds
// the HUD element table. Preloads eight SFX, sets projection reference 240, resets module state and
// selects mode-dependent sound/music commands. Runs initialization helpers for player, level and
// display state, upper-clamps g_SubmarineWorkingLevelLimit to 500, and performs the initial
// movement/render calculations. Resets counters and stores timeGetTime divided by 1000 then
// multiplied by 60, captures the result global and sets the duration to 900 or 1200 according to
// mode. Does not check archive/object creation success or restore asset-source mode.
// FUNCTION: FF7 0x0077d030
void __cdecl Submarine_ModuleEnter(GfxEngineContext *ctx)
{
    // Sixteen load-option blocks, the archive path and the countdown, in the original's stack order.
    struct {
        int countdown;
        PfileLoadOptions options[15];
        char archivePath[256];
        PfileLoadOptions textOptions;
    } v;
    Gfx_DebugPrint(
        // STRING: FF7 0x00980ddc
        "----- Submarine Mini Game BEGIN -----\n");
    Submarine_InitStateAndTables();
    g_SubmarineMissionId = (gSavemap.submarineMissionId);
    SetPerspective(45.0f, 3.0f, 65536.0f, 0, 0, 640.0f, 480.0f, ctx);
    SetMatrixLayoutMode(1);
    Pfile_SetAssetSourceMode(0);
    strcpy(v.archivePath, GetMinigameDirForSubmarine());
    strcat(v.archivePath,
           // STRING: FF7 0x00980e04
           "sub.lgp");
    OpenLgpArchive(v.archivePath, LGP_ARCHIVE_SUBMARINE);
    Pfile_InitLoadOptionsWithBlend(BLEND_ADDITIVE, &v.options[5]);
    v.options[5].file.fromArchive = 1;
    v.options[5].file.archiveId = 17;
    v.options[5].file.callback = 0;
    v.options[5].directory = g_SubmarineEmptyDirectory;
    v.options[5].flags |= 0x10;
    v.options[5].renderFlags |= 0x20;
    Pfile_InitLoadOptionsWithBlend(BLEND_ADDITIVE, &v.options[1]);
    v.options[1].file.fromArchive = 1;
    v.options[1].file.archiveId = 17;
    v.options[1].file.callback = 0;
    v.options[1].directory = g_SubmarineEmptyDirectories[0];
    v.options[1].flags |= 0x10;
    v.options[1].renderFlags |= 0x20;
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &v.textOptions);
    v.textOptions.file.fromArchive = 1;
    v.textOptions.file.archiveId = 17;
    v.textOptions.file.callback = 0;
    v.textOptions.directory = g_SubmarineEmptyDirectories[1];
    v.textOptions.flags |= 0x10;
    v.textOptions.renderFlags |= 0x20;
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &v.options[12]);
    v.options[12].file.fromArchive = 1;
    v.options[12].file.archiveId = 17;
    v.options[12].file.callback = 0;
    v.options[12].directory = g_SubmarineEmptyDirectories[2];
    v.options[12].flags |= 0x10;
    v.options[12].renderFlags |= 0x20;
    Pfile_InitLoadOptionsWithBlend(BLEND_ADDITIVE, &v.options[7]);
    Pfile_InitLoadOptionsWithBlend(BLEND_ADDITIVE, &v.options[13]);
    v.options[13].file.fromArchive = 1;
    v.options[13].file.archiveId = 17;
    v.options[13].file.callback = 0;
    v.options[13].directory = g_SubmarineEmptyDirectories[3];
    v.options[13].renderFlags |= 0x20;
    Pfile_InitLoadOptionsWithBlend(BLEND_AVERAGE, &v.options[14]);
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &v.options[2]);
    v.options[2].emitterFlags |= 2;
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &v.options[6]);
    v.options[6].emitterFlags |= 2;
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &v.options[3]);
    v.options[3].emitterFlags |= 2;
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &v.options[4]);
    v.options[4].emitterFlags |= 2;
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &v.options[10]);
    v.options[10].emitterFlags |= 2;
    Pfile_InitLoadOptionsWithBlend(BLEND_AVERAGE, &v.options[8]);
    v.options[8].emitterFlags |= 2;
    Pfile_InitLoadOptionsWithBlend(BLEND_AVERAGE, &v.options[0]);
    v.options[0].emitterFlags |= 2;
    Pfile_InitLoadOptions(&v.options[11]);
    v.options[11].emitterFlags |= 2;
    Pfile_InitLoadOptionsWithBlend(BLEND_AVERAGE, &v.options[9]);
    g_SubmarineHudMainAtlas = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[5],
                                                // STRING: FF7 0x00980e0c
                                                "hud.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudMainAtlasA = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[5],
                                                // STRING: FF7 0x00980e14
                                                "huda.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudBatchTexB = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[5],
                                                // STRING: FF7 0x00980e20
                                                "hudb.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudBatchTexC = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[5],
                                                // STRING: FF7 0x00980e2c
                                                "hudc.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudBatchTexD = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[5],
                                                // STRING: FF7 0x00980e38
                                                "hudd.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudReadoutAtlas = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[12],
                                                // STRING: FF7 0x00980e44
                                                "hud.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudReadoutAtlasA = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[12],
                                                // STRING: FF7 0x00980e4c
                                                "huda.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudReadoutAtlasB = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[12],
                                                // STRING: FF7 0x00980e58
                                                "hudb.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudRenderObjectA = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[12],
                                                // STRING: FF7 0x00980e64
                                                "hudc.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudReadoutAtlasD = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[12],
                                                // STRING: FF7 0x00980e70
                                                "hudd.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudBarAtlas = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_GOURAUD_QUAD_2D, &v.options[13],
                                                // STRING: FF7 0x00980e7c
                                                "hud.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudBarAtlasA = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_GOURAUD_QUAD_2D, &v.options[13],
                                                // STRING: FF7 0x00980e84
                                                "huda.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudRenderObjectB = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_GOURAUD_QUAD_2D, &v.options[13],
                                                // STRING: FF7 0x00980e90
                                                "hudb.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudBarAtlasC = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_GOURAUD_QUAD_2D, &v.options[13],
                                                // STRING: FF7 0x00980e9c
                                                "hudc.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudBarAtlasD = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_GOURAUD_QUAD_2D, &v.options[13],
                                                // STRING: FF7 0x00980ea8
                                                "hudd.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudRenderState.renderObject = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[1],
                                                // STRING: FF7 0x00980eb4
                                                "text.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudRenderState.renderObjectAlt = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[1],
                                                // STRING: FF7 0x00980ec0
                                                "texta.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudSingleTextAtlasB = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[1],
                                                // STRING: FF7 0x00980ecc
                                                "textb.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudSingleTextAtlasC = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[1],
                                                // STRING: FF7 0x00980ed8
                                                "textc.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudRenderObjectD = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.options[1],
                                                // STRING: FF7 0x00980ee4
                                                "textd.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudTextAtlas = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.textOptions,
                                                // STRING: FF7 0x00980ef0
                                                "text.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudTextAtlasA = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.textOptions,
                                                // STRING: FF7 0x00980efc
                                                "texta.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudTextAtlasB = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.textOptions,
                                                // STRING: FF7 0x00980f08
                                                "textb.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudTextAtlasC = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.textOptions,
                                                // STRING: FF7 0x00980f14
                                                "textc.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineHudTextAtlasD = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &v.textOptions,
                                                // STRING: FF7 0x00980f20
                                                "textd.tim", (void *)(ctx->renderObjectBank)));
    g_SubmarineGridBatchListA = (struct GfxRenderObject *)((int)(Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_TRI_2D, &v.options[2], 0, (void *)(ctx->renderObjectBank))));
    g_SubmarineModeBatchList0 = (struct GfxRenderObject *)((int)(Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_TRI_3D, &v.options[6], 0, (void *)(ctx->renderObjectBank))));
    g_SubmarineModeBatchList1 = (struct GfxRenderObject *)((int)(Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_TRI_2D, &v.options[3], 0, (void *)(ctx->renderObjectBank))));
    g_SubmarineModeBatchList2 = (struct GfxRenderObject *)((int)(Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_TRI_3D, &v.options[4], 0, (void *)(ctx->renderObjectBank))));
    g_SubmarineHudRenderObjectE = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_QUAD_2D, &v.options[7], 0, (void *)(ctx->renderObjectBank)));
    g_SubmarineHudScreenTint = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, &v.options[14], 0, (void *)(ctx->renderObjectBank)));
    g_SubmarineFixedBatchList = (struct GfxRenderObject *)((int)(Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_TRI_2D, &v.options[10], 0, (void *)(ctx->renderObjectBank))));
    g_SubmarineRadialTerrainScaledBatchNode = (struct GfxRenderObject *)((int)(Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_TRI_2D, &v.options[8], 0, (void *)(ctx->renderObjectBank))));
    g_SubmarineRadialTerrainBatchNode = (struct GfxRenderObject *)((int)(Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_TRI_2D, &v.options[0], 0, (void *)(ctx->renderObjectBank))));
    g_SubmarineLineBatchList = (struct GfxRenderObject *)((int)(Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_LINE_2D, &v.options[11], 0, (void *)(ctx->renderObjectBank))));
    g_SubmarineConnectorBatchContext = (struct GfxRenderObject *)((int)(Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_QUAD_2D, &v.options[9], 0, (void *)(ctx->renderObjectBank))));
    CloseLgpArchive(LGP_ARCHIVE_SUBMARINE);
    Submarine_BuildHudElementTable();
    SfxLoadIntoBuffer(SFX_ID_CURSOR, 0);
    SfxLoadIntoBuffer(SFX_ID_SUB_SONAR, 0);
    SfxLoadIntoBuffer(SFX_ID_SUB_TORPEDO, 0);
    SfxLoadIntoBuffer(SFX_ID_SUB_MINE_HIT, 0);
    SfxLoadIntoBuffer(SFX_ID_SUB_COLLISION, 0);
    SfxLoadIntoBuffer(SFX_ID_SUB_DIVE, 0);
    SfxLoadIntoBuffer(SFX_ID_SUB_ALERT, 0);
    SfxLoadIntoBuffer(SFX_ID_SUB_EXPLOSION, 0);
    SetGeomScreenReference(240.0);
    g_SubmarineAudioState.word = 0;
    (g_SubmarineControlFlags) = 0;
    (g_SubmarineGameRunning) = 1;
    Submarine_SendSoundSetupSequence(g_SubmarineMissionId);
    if (g_SubmarineMissionId == 0) {
        SoundDispatchCommand(SOUND_MUSIC_VOLUME_FADE, 0x3fc, 0x40, 1, 0, 0, 0, 0, 0);
        SoundDispatchCommand(SOUND_MUSIC_PLAY, 0x46, 0, 0, 0, 0, 0, 0, 0);
    } else
        SoundDispatchCommand(SOUND_MUSIC_PLAY, 0x58, 0, 0, 0, 0, 0, 0, 0);
    Submarine_ReseedAndInstallViewMatrix();
    Submarine_SelectSubmarineDataBlocks(g_SubmarineMissionId);
    Submarine_LoadStateFromRecord((struct SubmarineStateRecord *)(&g_SubmarineWorkingLevelVector[4]));
    Submarine_InitTargetCountDigits();
    (g_SubmarineSubState) = 0;
    if ((g_SubmarineWorkingLevelLimit) > 500)
        (g_SubmarineWorkingLevelLimit) = 500;
    Submarine_AllocateGridRecordSlots((*(short *)&g_SubmarineWorkingLevelLimit));
    Submarine_ResetRecordTable();
    Submarine_CollectFlaggedGridCells((g_SubmarineWorkingLevelLimit));
    Submarine_UpdateMotionAndCollision();
    Submarine_RebuildViewTransform();
    Submarine_ComputeRecordPitchYaw();
    Submarine_UpdateObjectRecords();
    Submarine_UpdateHudGauges();
    (g_SubmarineEffectActive[0]) = 0;
    (g_SubmarineBaseTime) = ((int)timeGetTime() / 1000) * 60;
    (g_SubmarineTimeLimit) = (g_SubmarineWorkingLevelParams[0]);
    v.countdown = 900;
    if (g_SubmarineMissionId != 0)
        v.countdown = 1200;
    (g_SubmarineCountdown) = v.countdown;
}

// Destroys and clears thirty-six submarine render-object globals and releases eight cached sound
// buffers with mode 15. Sets the saved result word to one only when g_SubmarineResultCode is SUBMARINE_RESULT_CLEARED; both
// branches of the g_SubmarineMissionId test do the same, and the word goes to the savemap low/high bytes.
// FUNCTION: FF7 0x0077daed
void __cdecl Submarine_ModuleExit(struct GfxEngineContext *ctx)
{
    DestroyRenderObject((void *)g_SubmarineHudMainAtlas);
    g_SubmarineHudMainAtlas = 0;
    DestroyRenderObject((void *)g_SubmarineHudMainAtlasA);
    g_SubmarineHudMainAtlasA = 0;
    DestroyRenderObject((void *)g_SubmarineHudBatchTexB);
    g_SubmarineHudBatchTexB = 0;
    DestroyRenderObject((void *)g_SubmarineHudBatchTexC);
    g_SubmarineHudBatchTexC = 0;
    DestroyRenderObject((void *)g_SubmarineHudBatchTexD);
    g_SubmarineHudBatchTexD = 0;
    DestroyRenderObject((void *)g_SubmarineHudReadoutAtlas);
    g_SubmarineHudReadoutAtlas = 0;
    DestroyRenderObject((void *)g_SubmarineHudReadoutAtlasA);
    g_SubmarineHudReadoutAtlasA = 0;
    DestroyRenderObject((void *)g_SubmarineHudReadoutAtlasB);
    g_SubmarineHudReadoutAtlasB = 0;
    DestroyRenderObject((void *)g_SubmarineHudRenderObjectA);
    g_SubmarineHudRenderObjectA = 0;
    DestroyRenderObject((void *)g_SubmarineHudReadoutAtlasD);
    g_SubmarineHudReadoutAtlasD = 0;
    DestroyRenderObject((void *)g_SubmarineHudBarAtlas);
    g_SubmarineHudBarAtlas = 0;
    DestroyRenderObject((void *)g_SubmarineHudBarAtlasA);
    g_SubmarineHudBarAtlasA = 0;
    DestroyRenderObject((void *)g_SubmarineHudRenderObjectB);
    g_SubmarineHudRenderObjectB = 0;
    DestroyRenderObject((void *)g_SubmarineHudBarAtlasC);
    g_SubmarineHudBarAtlasC = 0;
    DestroyRenderObject((void *)g_SubmarineHudBarAtlasD);
    g_SubmarineHudBarAtlasD = 0;
    DestroyRenderObject((void *)g_SubmarineHudRenderState.renderObject);
    g_SubmarineHudRenderState.renderObject = 0;
    DestroyRenderObject((void *)g_SubmarineHudRenderState.renderObjectAlt);
    g_SubmarineHudRenderState.renderObjectAlt = 0;
    DestroyRenderObject((void *)g_SubmarineHudSingleTextAtlasB);
    g_SubmarineHudSingleTextAtlasB = 0;
    DestroyRenderObject((void *)g_SubmarineHudSingleTextAtlasC);
    g_SubmarineHudSingleTextAtlasC = 0;
    DestroyRenderObject((void *)g_SubmarineHudRenderObjectD);
    g_SubmarineHudRenderObjectD = 0;
    DestroyRenderObject((void *)g_SubmarineHudTextAtlas);
    g_SubmarineHudTextAtlas = 0;
    DestroyRenderObject((void *)g_SubmarineHudTextAtlasA);
    g_SubmarineHudTextAtlasA = 0;
    DestroyRenderObject((void *)g_SubmarineHudTextAtlasB);
    g_SubmarineHudTextAtlasB = 0;
    DestroyRenderObject((void *)g_SubmarineHudTextAtlasC);
    g_SubmarineHudTextAtlasC = 0;
    DestroyRenderObject((void *)g_SubmarineHudTextAtlasD);
    g_SubmarineHudTextAtlasD = 0;
    DestroyRenderObject(g_SubmarineGridBatchListA);
    g_SubmarineGridBatchListA = 0;
    DestroyRenderObject(g_SubmarineModeBatchList0);
    g_SubmarineModeBatchList0 = 0;
    DestroyRenderObject(g_SubmarineModeBatchList1);
    g_SubmarineModeBatchList1 = 0;
    DestroyRenderObject(g_SubmarineModeBatchList2);
    g_SubmarineModeBatchList2 = 0;
    DestroyRenderObject((void *)g_SubmarineHudRenderObjectE);
    g_SubmarineHudRenderObjectE = 0;
    DestroyRenderObject((void *)g_SubmarineHudScreenTint);
    g_SubmarineHudScreenTint = 0;
    DestroyRenderObject(g_SubmarineFixedBatchList);
    g_SubmarineFixedBatchList = 0;
    DestroyRenderObject(g_SubmarineRadialTerrainScaledBatchNode);
    g_SubmarineRadialTerrainScaledBatchNode = 0;
    DestroyRenderObject(g_SubmarineRadialTerrainBatchNode);
    g_SubmarineRadialTerrainBatchNode = 0;
    DestroyRenderObject(g_SubmarineLineBatchList);
    g_SubmarineLineBatchList = 0;
    DestroyRenderObject(g_SubmarineConnectorBatchContext);
    g_SubmarineConnectorBatchContext = 0;
    Sfx_ReleaseCachedBuffer(1, 15);
    Sfx_ReleaseCachedBuffer(405, 15);
    Sfx_ReleaseCachedBuffer(466, 15);
    Sfx_ReleaseCachedBuffer(553, 15);
    Sfx_ReleaseCachedBuffer(554, 15);
    Sfx_ReleaseCachedBuffer(560, 15);
    Sfx_ReleaseCachedBuffer(568, 15);
    Sfx_ReleaseCachedBuffer(579, 15);
    if (g_SubmarineMissionId == 0) {
        if (g_SubmarineResultCode == SUBMARINE_RESULT_CLEARED)
            g_SubmarineAudioState.word = 1;
        else
            g_SubmarineAudioState.word = 0;
    } else {
        if (g_SubmarineResultCode == SUBMARINE_RESULT_CLEARED)
            g_SubmarineAudioState.word = 1;
        else
            g_SubmarineAudioState.word = 0;
    }
    (gSavemap.submarineAudioStateLowByte) = g_SubmarineAudioState.word & 0xff;
    (gSavemap.submarineAudioStateHighByte) = (g_SubmarineAudioState.word >> 8) & 0xff;
    g_EnginePreviousMode = GAME_MODE_SUBMARINE;
    g_EngineGameMode = GAME_MODE_FIELD;
    Gfx_DebugPrint(
        // STRING: FF7 0x00980f2c
        "----- Submarine Mini Game END -----\n");
}

// Runs the submarine module simulation/render frame. Clears the background and draw pools, then
// updates input, projection and simulation until the frame limiter releases the loop, with an
// immediate single iteration when limiting is disabled. Handles viewport zoom/debug toggling,
// pause audio, the paused quit-selection dialog and confirmed module return. Gates control,
// object, environment and HUD updates with pause/end-state and render-frame flags; advances the
// quantized millisecond timer except while paused or debug-frozen. Checks health, elapsed limit
// and objective counts in sequence, so later success checks override earlier failure codes, then
// starts the corresponding interpolation/audio fade and end countdown. Submits scene geometry and
// HUD when BeginScene succeeds, restores the full viewport for the HUD when needed, optionally
// obtains/releases the debug surface DC for formatted diagnostics, and requests the dispatcher
// callbacks when exit is selected or activity ends.
// FUNCTION: FF7 0x0077df72
void __cdecl Submarine_ModuleFrame(struct GfxEngineContext *ctx)
{
    // The frame's locals in the original's stack order. Three switch temporaries extend this source
    // frame to the original 0x140 bytes.
    struct {
        GameModule module;      /* the field module the quit dialog returns to */
        int textY;
        int textX;
        HDC dc;
        char text[200];         /* one debug line */
        int countdownGauge;     /* the elapsed part of the end countdown, 0..80; not read */
        int result;             /* enum SubmarineResult */
        int frameTicks;         /* the time step in 1/60 s */
        int alertBlink;         /* not read */
        int quitSelected;       /* not read */
        int quitDialogState;
        unsigned short soundCommand;
        unsigned char pad_10a[2];
        int pauseVolume;        /* not read */
        int paused;
        int alarmStart;
        int scene;
        int exitRequested;
        int debugTextEnabled;
        GfxClearColorBgra backgroundColor;
    } v;
    v.exitRequested = 0;
    v.debugTextEnabled = 1;
    v.backgroundColor.r = 0;
    v.backgroundColor.g = 0;
    v.backgroundColor.b = 0;
    v.backgroundColor.a = FLOAT_ONE_BITS;
    GfxSetBackgroundColorCall(&v.backgroundColor, ctx);
    GfxClearCall(1, 1, 1, ctx);
    v.scene = 0;
    ResetFrameDrawPool(ctx);
    ResetRenderPools(ctx);
    if ((g_SubmarineFrameLimiterEnabled) == 0) {
        (g_SubmarineFrameOnTime) = 0;
        (g_SubmarineActiveFlag) = 1;
    } else
        Submarine_FrameRateLimit(ctx);
    while ((g_SubmarineFrameOnTime) == 0) {
        ReadTimestampCounter(&g_SubmarineFrameTimestamp);
        Input_Update(0);
        SetProjectionOffsetAndRebuild(g_SubmarineScreenShake.projectionOffsetX, g_SubmarineScreenShake.projectionOffsetY);
        if ((g_SubmarineGameRunning) != 0) {
            if ((g_SubmarineRenderMode) == MAIN_GRAPHICS_MODE_640X480) {
                if (Input_IsKeyDown(12)) { /* DIK_MINUS: shrink the viewport */
                    (g_SubmarineViewportShrink)++;
                    if ((g_SubmarineViewportShrink) > 60)
                        (g_SubmarineViewportShrink) = 60;
                    (g_SubmarineViewportWidth) = ctx->displayWidth - (g_SubmarineViewportShrink) * 8;
                    (g_SubmarineViewportHeight) = ctx->displayHeight - (g_SubmarineViewportShrink) * 6;
                    (g_SubmarineViewportLeft) = ctx->displayWidth / 2 - (g_SubmarineViewportWidth) / 2;
                    (g_SubmarineViewportTop) = ctx->displayHeight / 2 - (g_SubmarineViewportHeight) / 2;
                }
                if (Input_IsKeyDown(13)) { /* DIK_EQUALS: grow it back */
                    (g_SubmarineViewportShrink)--;
                    if ((g_SubmarineViewportShrink) < 0)
                        (g_SubmarineViewportShrink) = 0;
                    (g_SubmarineViewportWidth) = ctx->displayWidth - (g_SubmarineViewportShrink) * 8;
                    (g_SubmarineViewportHeight) = ctx->displayHeight - (g_SubmarineViewportShrink) * 6;
                    (g_SubmarineViewportLeft) = ctx->displayWidth / 2 - (g_SubmarineViewportWidth) / 2;
                    (g_SubmarineViewportTop) = ctx->displayHeight / 2 - (g_SubmarineViewportHeight) / 2;
                }
            }
            if (Input_IsKeyDown(60)) { /* DIK_F2: toggle debug flag 0x40 */
                if ((g_SubmarineDebugKeyHeld) == 0) {
                    g_SubmarineFlags ^= SUBMARINE_FLAG_DEBUG_WIREFRAME;
                    (g_SubmarineDebugKeyHeld) = 1;
                }
            } else
                (g_SubmarineDebugKeyHeld) = 0;
            GfxSetViewportRect((g_SubmarineViewportLeft), (g_SubmarineViewportTop), (g_SubmarineViewportWidth), (g_SubmarineViewportHeight), ctx);
            if (!((g_SubmarineControlFlags) & SUBMARINE_CTRL_ENDED)) {
                v.alarmStart = 0;
                if (!((g_SubmarineControlFlags) & SUBMARINE_CTRL_PAUSED) && (g_SubmarineAlarmActive) == 0)
                    v.alarmStart = 1;
                if (v.alarmStart) {
                    (g_SubmarineAlarmActive) = 1;
                    SoundDispatchCommand(SOUND_SFX_PLAY_CH1, 0x40, 0x195, 0, 0, 0, 0, 0, 0);
                    (g_SubmarineFadeStep) = 0;
                    g_SubmarineHudScrollPos = 16;
                }
            }
            if (!((g_SubmarineControlFlags) & SUBMARINE_CTRL_ENDED) && Input_GetPressed(INPUT_START)) {
                v.pauseVolume = 4;
                v.soundCommand = SOUND_PAUSE_ALL;
                v.paused = 0;
                (g_SubmarineControlFlags) ^= SUBMARINE_CTRL_PAUSED;
                v.paused = (g_SubmarineControlFlags) & SUBMARINE_CTRL_PAUSED;
                if (!v.paused) {
                    v.soundCommand = SOUND_RESUME_ALL;
                    v.pauseVolume = 0;
                }
                (g_SubmarineGameState) = SUBMARINE_HUD_PAUSE_BANNER | SUBMARINE_HUD_PAUSE_HELP;
                SoundDispatchCommand(v.soundCommand, 0, 0, 0, 0, 0, 0, 0, 0);
                switch (v.soundCommand) {
                case SOUND_PAUSE_ALL:
                    MusicWavePause();
                    SfxStopCachedBuffer(0x243);
                    break;
                case SOUND_RESUME_ALL:
                    MusicWaveResume();
                    SfxResumeCachedBuffer(0x243);
                    break;
                }
                (g_SubmarineControlFlags) &= ~12;
            }
            if (g_SubmarineMissionId != 0 && ((g_SubmarineControlFlags) & SUBMARINE_CTRL_PAUSED) && Input_GetPressed(INPUT_SELECT)) {
                v.quitSelected = 0;
                v.quitDialogState = 5;
                (g_SubmarineControlFlags) ^= SUBMARINE_CTRL_QUIT_DIALOG;
                (g_SubmarineControlFlags) &= ~8;
                Submarine_SetSwapQuadSelection(0);
                if (!((g_SubmarineControlFlags) & SUBMARINE_CTRL_QUIT_DIALOG)) {
                    v.quitSelected = 1;
                    v.quitDialogState = 4;
                }
                switch (v.quitDialogState) {
                case SUBMARINE_QUIT_DIALOG_CANCEL:
                    (g_SubmarineGameState) = SUBMARINE_HUD_QUIT_PROMPT;
                    break;
                case SUBMARINE_QUIT_DIALOG_CONFIRM:
                    (g_SubmarineGameState) = SUBMARINE_HUD_PAUSE_BANNER | SUBMARINE_HUD_PAUSE_HELP;
                    break;
                }
            }
            if (((g_SubmarineControlFlags) & (SUBMARINE_CTRL_PAUSED | SUBMARINE_CTRL_QUIT_DIALOG)) == (SUBMARINE_CTRL_PAUSED | SUBMARINE_CTRL_QUIT_DIALOG)) {
                if (Input_GetPressed(INPUT_LEFT) | Input_GetPressed(INPUT_RIGHT) | Input_GetPressed(INPUT_CIRCLE)) {
                    SfxPlayCachedBuffer(1, 0x40, 0x7f, 0);
                    if (Input_GetPressed(INPUT_LEFT))
                        (g_SubmarineControlFlags) |= SUBMARINE_CTRL_QUIT_CONFIRM;
                    else if (Input_GetPressed(INPUT_RIGHT))
                        (g_SubmarineControlFlags) &= ~8;
                    else if (Input_GetPressed(INPUT_CIRCLE)) {
                        if ((g_SubmarineControlFlags) & SUBMARINE_CTRL_QUIT_CONFIRM) {
                            g_SubmarineResultCode = SUBMARINE_RESULT_QUIT;
                            v.exitRequested = 1;
                        } else {
                            (g_SubmarineControlFlags) &= ~12;
                            (g_SubmarineGameState) = SUBMARINE_HUD_PAUSE_BANNER | SUBMARINE_HUD_PAUSE_HELP;
                        }
                    }
                    Submarine_SetSwapQuadSelection((g_SubmarineControlFlags) & SUBMARINE_CTRL_QUIT_CONFIRM);
                }
            }
            if (!((g_SubmarineControlFlags) & SUBMARINE_CTRL_PAUSED)) {
                (g_SubmarineGameState) &= ~2;
                (g_SubmarineGameState) &= ~4;
                (g_SubmarineGameState) &= ~1;
                Submarine_UpdatePlayerControls();
            }
            Submarine_RebuildViewTransform();
            SetGeomScreen(g_SubmarineState.geomScreen);
            if ((g_SubmarineActiveFlag))
                Submarine_DrawVerticalSliceQuads();
            g_SubmarineTriangleColorBuffer.posX = g_SubmarineState.player.posX >> 12;
            (g_SubmarineTriangleColorBuffer.posY) = g_SubmarineState.player.posY >> 12;
            (g_SubmarineTriangleColorBuffer.posZ) = g_SubmarineState.player.posZ >> 12;
            g_SubmarineRenderBatchState = g_SubmarineState.player.angle;
            g_SubmarineRenderBatchYawAngle = g_SubmarineState.player.yaw;
            g_SubmarineTriangleBatchCount = g_SubmarineState.player.roll;
            if ((g_SubmarineActiveFlag))
                Submarine_EmitObjectTriangleBatch((struct SubmarineAngleSet *)g_SubmarineState.modelDescriptor);
            if (!((g_SubmarineControlFlags) & (SUBMARINE_CTRL_PAUSED | SUBMARINE_CTRL_ENDED)))
                Submarine_UpdateMotionAndCollision();
            SetRotMatrix(&g_SubmarineViewMatrix);
            SetTransMatrix(&g_SubmarineViewMatrix);
            if (!((g_SubmarineControlFlags) & SUBMARINE_CTRL_PAUSED))
                Submarine_StepGridCellHeights();
            if ((g_SubmarineActiveFlag)) {
                Submarine_BuildCameraGridBatches();
                Submarine_EmitRadialBandTerrainBatches();
            }
            Submarine_ComputeRecordPitchYaw();
            SetRotMatrix(&g_SubmarineViewMatrix);
            SetTransMatrix(&g_SubmarineViewMatrix);
            if (!((g_SubmarineControlFlags) & (SUBMARINE_CTRL_PAUSED | SUBMARINE_CTRL_ENDED)))
                Submarine_UpdateObjectRecords();
            Submarine_UpdateActiveFlagRecord();
            if ((g_SubmarineActiveFlag)) {
                Submarine_BuildProjectedStripBatches();
                Submarine_DrawObjectRecords();
            }
            if ((g_SubmarineActiveFlag)) {
                if (!((g_SubmarineControlFlags) & SUBMARINE_CTRL_ENDED))
                    Submarine_DrawFlaggedRecordOutlines();
                if (!((g_SubmarineControlFlags) & (SUBMARINE_CTRL_PAUSED | SUBMARINE_CTRL_ENDED)))
                    Submarine_UpdateRecordFlightStates();
                Submarine_EmitRecordConnectorQuads();
            }
            if (!((g_SubmarineControlFlags) & SUBMARINE_CTRL_PAUSED))
                Submarine_UpdateScreenShake();
            if (!((g_SubmarineControlFlags) & SUBMARINE_CTRL_PAUSED)) {
                if (!((g_SubmarineControlFlags) & SUBMARINE_CTRL_ENDED)) {
                    Submarine_UpdateTorpedoAudio();
                    Submarine_UpdateEnemyAudio();
                }
                Submarine_UpdateHudGauges();
            }
            if ((g_SubmarineActiveFlag) && !((g_SubmarineControlFlags) & SUBMARINE_CTRL_ENDED))
                Submarine_UpdateAlertHudAndAlarm();
            if (g_SubmarineMissionId == 0) {
                if ((g_SubmarineAlertState) == 16)
                    SoundDispatchCommand(SOUND_MUSIC_VOLUME_TRANS, 30, 1, 0, 0, 0, 0, 0, 0);
                if ((g_SubmarineAlertState) == 17)
                    SoundDispatchCommand(SOUND_MUSIC_PLAY, 0x58, 0, 0, 0, 0, 0, 0, 0);
            }
            if (!((g_SubmarineControlFlags) & SUBMARINE_CTRL_ENDED) && (g_SubmarineAlertState) > 17)
                SoundDispatchCommand(SOUND_MUSIC_VOLUME_TRANS, 8, (g_SubmarineHudParams.musicVolume), 0, 0, 0, 0, 0, 0);
            if ((g_SubmarineAlertState) == 17)
                (g_SubmarineEffectActive[0]) = 1;
            if ((g_SubmarineElapsedFrames) >= 0 && (g_SubmarineElapsedFrames) < 240)
                v.alertBlink = (g_SubmarineElapsedFrames) / 16;
            v.frameTicks = ((int)timeGetTime() / 1000) * 60;
            ((g_SubmarineTimeOffsets.baseTime)) = v.frameTicks;
            v.frameTicks -= (g_SubmarineBaseTime);
            if ((g_SubmarineControlFlags) & SUBMARINE_CTRL_PAUSED)
                v.frameTicks = 0;
            if (g_SubmarineFlags & SUBMARINE_FLAG_FREEZE_TIMER)
                v.frameTicks = 0;
            (g_SubmarineTimeOffsets.countdownStep) = v.frameTicks;
            (g_SubmarineElapsedFrames) += v.frameTicks;
            g_SubmarineTimeRemaining = (g_SubmarineTimeLimit) - (g_SubmarineElapsedFrames);
            (g_SubmarineAlertState) = (g_SubmarineElapsedFrames) / 60;
            (g_SubmarineMinutesElapsed) = (g_SubmarineElapsedFrames) / 3600;
            (g_SubmarineBaseTime) = ((g_SubmarineTimeOffsets.baseTime));
            if (!((g_SubmarineControlFlags) & SUBMARINE_CTRL_ENDED)) {
                v.result = SUBMARINE_RESULT_NONE;
                if (g_SubmarineMissionId == 0) {
                    if (g_SubmarineState.player.playerResource <= 0)
                        v.result = SUBMARINE_RESULT_DESTROYED;
                    if ((g_SubmarineElapsedFrames) > (g_SubmarineTimeLimit))
                        v.result = SUBMARINE_RESULT_TIME_UP;
                    if (!((((SubmarineObjectState *)g_SubmarineModelVertexData)->flags) & 3))
                        v.result = SUBMARINE_RESULT_CLEARED;
                    if (g_SubmarineEffectCount1 + g_SubmarineActiveSlotCount >= g_SubmarineEffectCount0)
                        v.result = SUBMARINE_RESULT_CLEARED;
                    if (g_SubmarineEffectCount1 >= g_SubmarineEffectCount0)
                        v.result = SUBMARINE_RESULT_CLEARED;
                } else {
                    if (g_SubmarineState.player.playerResource <= 0)
                        v.result = SUBMARINE_RESULT_DESTROYED;
                    if ((g_SubmarineElapsedFrames) > (g_SubmarineTimeLimit))
                        v.result = SUBMARINE_RESULT_TIME_UP;
                    if (g_SubmarineEffectCount1 + g_SubmarineActiveSlotCount >= g_SubmarineEffectCount0)
                        v.result = SUBMARINE_RESULT_CLEARED;
                }
                g_SubmarineResultCode = v.result;
                if (v.result == SUBMARINE_RESULT_CLEARED)
                    g_SubmarineRemainingTime = g_SubmarineTimeRemaining;
                else
                    g_SubmarineRemainingTime = 0;
                switch (v.result) {
                case SUBMARINE_RESULT_DESTROYED:
                    (g_SubmarineScreenShake.accumulator) += (g_SubmarineHudOffset) * 2;
                    g_SubmarineHudOrigin = -16;
                    (g_SubmarineGameState) = SUBMARINE_HUD_SCREEN_TINT | SUBMARINE_HUD_MISSION_FAILED;
                    (g_SubmarineGameState) &= ~0x20;
                    (g_SubmarineGameState) &= ~8;
                    Submarine_StartStateLerp16();
                    (g_SubmarineEffectActive[0]) = 0;
                    SoundDispatchCommand(SOUND_SFX_ALL_VOLUME_TRANS, 300, 0, 0, 0, 0, 0, 0, 0);
                    SoundDispatchCommand(SOUND_MUSIC_VOLUME_TRANS, 300, 0, 0, 0, 0, 0, 0, 0);
                    (g_SubmarineControlFlags) |= SUBMARINE_CTRL_ENDED;
                    break;
                case SUBMARINE_RESULT_TIME_UP:
                    (g_SubmarineGameState) = SUBMARINE_HUD_SCREEN_TINT | SUBMARINE_HUD_TIME_UP;
                    (g_SubmarineGameState) &= ~8;
                    (g_SubmarineGameState) &= ~0x10;
                    Submarine_StartStateLerp16();
                    (g_SubmarineEffectActive[0]) = 0;
                    SoundDispatchCommand(SOUND_SFX_ALL_VOLUME_TRANS, 300, 0, 0, 0, 0, 0, 0, 0);
                    SoundDispatchCommand(SOUND_MUSIC_VOLUME_TRANS, 300, 0, 0, 0, 0, 0, 0, 0);
                    (g_SubmarineControlFlags) |= SUBMARINE_CTRL_ENDED;
                    break;
                case SUBMARINE_RESULT_CLEARED:
                    (g_SubmarineGameState) = SUBMARINE_HUD_MISSION_COMPLETE;
                    (g_SubmarineGameState) &= ~0x100;
                    (g_SubmarineGameState) &= ~0x20;
                    (g_SubmarineGameState) &= ~0x10;
                    Submarine_StartStateLerp16();
                    (g_SubmarineEffectActive[0]) = 0;
                    SoundDispatchCommand(SOUND_SFX_ALL_VOLUME_TRANS, 300, 0, 0, 0, 0, 0, 0, 0);
                    SoundDispatchCommand(SOUND_MUSIC_VOLUME_TRANS, 300, 0, 0, 0, 0, 0, 0, 0);
                    (g_SubmarineControlFlags) |= SUBMARINE_CTRL_ENDED;
                    break;
                }
            } else {
                (g_SubmarineCountdown) -= (g_SubmarineTimeOffsets.countdownStep);
                if ((g_SubmarineCountdown) <= 0)
                    (g_SubmarineGameRunning) = 0;
                if (g_SubmarineMissionId == 0) {
                    if (g_SubmarineResultCode == SUBMARINE_RESULT_TIME_UP && (g_SubmarineCountdown) <= 450) {
                        (g_SubmarineGameState) = SUBMARINE_HUD_MISSION_FAILED;
                        (g_SubmarineGameState) &= ~8;
                        (g_SubmarineGameState) &= ~0x20;
                    }
                } else {
                    if ((g_SubmarineCountdown) <= 600) {
                        Submarine_UpdateTwoDigitHudCounter();
                        (g_SubmarineGameState) |= SUBMARINE_HUD_TARGET_TALLY;
                        (g_SubmarineGameState) &= ~0x10;
                        (g_SubmarineGameState) &= ~8;
                        (g_SubmarineGameState) &= ~0x20;
                    }
                    if ((g_SubmarineCountdown) <= 450) {
                        Submarine_UpdateFiveDigitHudCounter();
                        (g_SubmarineGameState) |= SUBMARINE_HUD_SCORE_PANEL;
                        (g_SubmarineGameState) &= ~0x10;
                        (g_SubmarineGameState) &= ~8;
                        (g_SubmarineGameState) &= ~0x20;
                    }
                }
                if (g_SubmarineMissionId != 0) {
                    v.countdownGauge = 1200 - (g_SubmarineCountdown);
                    v.countdownGauge = v.countdownGauge * 80 / 1200;
                } else {
                    v.countdownGauge = 900 - (g_SubmarineCountdown);
                    v.countdownGauge = v.countdownGauge * 80 / 900;
                }
            }
        } else
            v.exitRequested = 1;
        if ((g_SubmarineFrameLimiterEnabled) == 0) {
            (g_SubmarineFrameOnTime) = 1;
            (g_SubmarineActiveFlag) = 1;
        } else
            Submarine_FrameRateLimit(ctx);
        if ((g_SubmarineActiveFlag) != 0 && (g_SubmarineFrameLimiterEnabled) != 0)
            (g_SubmarineActiveFlag) = 0;
    }
    if (((int (__cdecl *)(int scene, int context))Gfx_BeginSceneCall)(v.scene, (int)ctx)) { /* reads eax after a void call, as the original does */
        Gfx_BeginSortedDrawFrame(ctx);
        Gfx_ResetNodeList(ctx);
        Submarine_EmitGridBatchesByMode(ctx);
        Submarine_EmitStripBatchList(ctx);
        Submarine_DrawFixedBatchPass(ctx);
        Submarine_DrawScenePassBatches(ctx);
        Submarine_DrawRecordQuadPass(ctx);
        if ((g_SubmarineRenderMode) == MAIN_GRAPHICS_MODE_640X480 && (g_SubmarineViewportHeight) < 480)
            GfxSetViewportRect(0, 0, 640, 480, ctx);
        Submarine_DrawHud();
        Submarine_EmitHudRenderObjects(ctx);
        Gfx_EndSceneCall(ctx);
        (g_SubmarineActiveFlag) = 1;
    }
    if (ctx->minimized == 0 && ctx->backSurface != 0 && 1) {
        if (v.debugTextEnabled && ((ComDirectDrawSurface *)ctx->backSurface)->GetDC(&v.dc) == 0) {
            v.textX = 0;
            v.textY = 0;
            if (g_SubmarineFlags & SUBMARINE_FLAG_DEBUG_ROT_INC) {
                sprintf(v.text,
                        // STRING: FF7 0x00980f54
                        "fps: %.2f", ctx->fps);
                App_DrawDebugTextGdi(v.textX, 464, v.text, v.dc);
            }
            if (g_SubmarineFlags & SUBMARINE_FLAG_DEBUG_ROT_DEC) {
                sprintf(v.text,
                        // STRING: FF7 0x00980f60
                        "pos: (%i, %i, %i)  rot: (%i, %i, %i)  dir: (%i, %i, %i)", g_SubmarineState.player.posX, g_SubmarineState.player.posY, g_SubmarineState.player.posZ, g_SubmarineState.player.angle, g_SubmarineState.player.yaw, g_SubmarineState.player.roll, g_SubmarineState.directionX, g_SubmarineState.directionY, g_SubmarineState.directionZ);
                App_DrawDebugTextGdi(v.textX, v.textY, v.text, v.dc);
                v.textY += 16;
                sprintf(v.text,
                        // STRING: FF7 0x00980f98
                        "up: (%i, %i, %i)  right: (%i, %i, %i)  grid pos: (x: %i, z: %i)", (g_SubmarineState.probeOffsetX), (g_SubmarineState.probeOffsetY), (g_SubmarineState.probeOffsetZ), g_SubmarineState.rightBasisX, g_SubmarineState.rightBasisY, g_SubmarineState.rightBasisZ, ((g_SubmarineState.player.posX >> 12) + 2048) >> 6, ((g_SubmarineState.player.posZ >> 12) + 2048) >> 6);
                App_DrawDebugTextGdi(v.textX, v.textY, v.text, v.dc);
                v.textY += 16;
                sprintf(v.text,
                        // STRING: FF7 0x00980fd8
                        "cam offset: %i", g_SubmarineState.camOffset);
                App_DrawDebugTextGdi(450, 464, v.text, v.dc);
            }
            if (g_SubmarineFlags & SUBMARINE_FLAG_DEBUG_STEP) {
                sprintf(v.text,
                        // STRING: FF7 0x00980fe8
                        "cam rot: (%i, %i, %i)", g_SubmarineState.camRotX, g_SubmarineState.camRotY, g_SubmarineState.camRotZ);
                App_DrawDebugTextGdi(v.textX, v.textY, v.text, v.dc);
            }
            ((ComDirectDrawSurface *)ctx->backSurface)->ReleaseDC(v.dc);
        }
        Gfx_StatsClearRecord(ctx->statsRecord);
    }
    if (v.exitRequested) {
        GetActiveModule(&v.module, ctx);
        v.module.frame = GameModeDispatcher_Frame;
        v.module.enter = GameModeDispatcher_Enter;
        v.module.exit = GameModeDispatcher_Exit;
        v.module.onMouse = App_OnMouseMessage;
        v.module.onKey = App_OnKeyMessage;
        RequestModuleSwitch(&v.module, ctx);
    }
}
/* Reseeds random state and installs submarine view transformation matrix. */
// FUNCTION: FF7 0x0077f0e2
void __cdecl Submarine_ReseedAndInstallViewMatrix(void)
{
    srand(rand());
    g_SubmarineUpVectorX = g_SubmarineState.hudVector;
    g_SubmarineUpVectorY = g_SubmarineState.hudScalarA;
    g_SubmarineUpVectorZ = g_SubmarineState.hudScalarB;
    SetRotMatrix(&g_SubmarineViewPad);
    SetTransMatrix(&g_SubmarineViewPad);
}
// Loads one submarine level into the working tables; called by Submarine_ModuleEnter with the
// level selector it read from the savemap (g_SubmarineMissionId). Selector 0 uses level
// block 5, 1 block 4, and so on to 5, which uses block 0. For that block it builds the terrain
// grid records and grid heights from the 64 x 64 map, fills the working route table from the
// block's route table (Submarine_FillRouteRecordTable) and copies the block's twelve actor
// definitions into the object records (Submarine_CopyObjectRecordTable). Any other selector
// loads nothing and leaves the tables as they were. Then normalises two level parameters:
// g_SubmarineWorkingLevelParams[2] is clamped to 0..15 (else 0) and shifted left by 18 (one grid
// unit of 64 in 20.12 fixed point), and [1] is clamped to 0..15 (else 0).
// FUNCTION: FF7 0x0077f131
void __cdecl Submarine_SelectSubmarineDataBlocks(char levelId)
{
    int paramOne;
    int paramTwo;

    switch (*(int *)&levelId & 0xFF) {
    case SUBMARINE_LEVEL_STORY:
        Submarine_BuildGridRecordsFromCharGrid(&g_SubmarineLevel5Map);
        Submarine_InitGridHeightsFromBytes((int)&g_SubmarineLevel5Map);
        Submarine_FillRouteRecordTable((struct SubmarineRouteRecord *)&g_SubmarineWorkingLevelRouteTable, 0);
        Submarine_CopyObjectRecordTable((struct SubmarineObjectRecord *)&g_SubmarineLevel5Record.actors);
        break;
    case SUBMARINE_LEVEL_1:
        Submarine_BuildGridRecordsFromCharGrid(&g_SubmarineLevel4Map);
        Submarine_InitGridHeightsFromBytes((int)&g_SubmarineLevel4Map);
        Submarine_FillRouteRecordTable((struct SubmarineRouteRecord *)&g_SubmarineWorkingLevelRouteTable, 1);
        Submarine_CopyObjectRecordTable((struct SubmarineObjectRecord *)&g_SubmarineLevel4Record.actors);
        break;
    case SUBMARINE_LEVEL_2:
        Submarine_BuildGridRecordsFromCharGrid(&g_SubmarineLevel3Map);
        Submarine_InitGridHeightsFromBytes((int)&g_SubmarineLevel3Map);
        Submarine_FillRouteRecordTable((struct SubmarineRouteRecord *)&g_SubmarineWorkingLevelRouteTable, 2);
        Submarine_CopyObjectRecordTable((struct SubmarineObjectRecord *)&g_SubmarineLevel3Record.actors);
        break;
    case SUBMARINE_LEVEL_3:
        Submarine_BuildGridRecordsFromCharGrid(&g_SubmarineLevel2Map);
        Submarine_InitGridHeightsFromBytes((int)&g_SubmarineLevel2Map);
        Submarine_FillRouteRecordTable((struct SubmarineRouteRecord *)&g_SubmarineWorkingLevelRouteTable, 3);
        Submarine_CopyObjectRecordTable((struct SubmarineObjectRecord *)&g_SubmarineLevel2Record.actors);
        break;
    case SUBMARINE_LEVEL_4:
        Submarine_BuildGridRecordsFromCharGrid(&g_SubmarineLevel1Map);
        Submarine_InitGridHeightsFromBytes((int)&g_SubmarineLevel1Map);
        Submarine_FillRouteRecordTable((struct SubmarineRouteRecord *)&g_SubmarineWorkingLevelRouteTable, 4);
        Submarine_CopyObjectRecordTable((struct SubmarineObjectRecord *)&g_SubmarineLevel1Record.actors);
        break;
    case SUBMARINE_LEVEL_5:
        Submarine_BuildGridRecordsFromCharGrid(&g_SubmarineLevel0Map);
        Submarine_InitGridHeightsFromBytes((int)&g_SubmarineLevel0Map);
        Submarine_FillRouteRecordTable((struct SubmarineRouteRecord *)&g_SubmarineWorkingLevelRouteTable, 5);
        Submarine_CopyObjectRecordTable((struct SubmarineObjectRecord *)&g_SubmarineLevel0Record.actors);
    }
    paramTwo = g_SubmarineWorkingLevelParams[2];
    if (paramTwo < 0 || paramTwo > 0xF) {
        paramTwo = 0;
    }
    paramTwo <<= 6;
    paramTwo <<= 0xC;
    g_SubmarineWorkingLevelParams[2] = paramTwo;
    paramOne = g_SubmarineWorkingLevelParams[1];
    if (paramOne < 0 || paramOne > 0xF) {
        paramOne = 0;
    }
    g_SubmarineWorkingLevelParams[1] = paramOne;
}
// Switches the engine to the submarine minigame with a level selector; no code calls it by name.
// Stores the selector (anything above 5 becomes 5) in g_SubmarineMissionId, reads the
// active module record, puts Submarine_ModuleFrame, Submarine_ModuleEnter and
// Submarine_ModuleExit in it, clears two more words (the submarine has no mouse or keyboard
// handler) and requests the module switch. The selector does not survive the switch:
// Submarine_ModuleEnter overwrites g_SubmarineMissionId with the savemap byte
// (gSavemap.submarineMissionId) before it uses it.
// FUNCTION: FF7 0x0077f324
void __cdecl Submarine_RequestModuleSwitchWithSelector(int selector)
{
    int flags;
    GfxEngineContext *ctx;
    int reserved;
    int exit;
    int frame;
    int moduleInfo[2];
    int enterCallback;

    ctx = GetEngineContext();
    if ((selector & 0xFF) > 5) {
        g_SubmarineMissionId = 5;
    } else {
        g_SubmarineMissionId = selector;
    }
    GetActiveModule((GameModule *)(moduleInfo), ctx);
    frame = (int)Submarine_ModuleFrame;
    enterCallback = (int)Submarine_ModuleEnter;
    exit = (int)Submarine_ModuleExit;
    reserved = 0;
    flags = 0;
    RequestModuleSwitch((GameModule *)(moduleInfo), ctx);
}
/* Sends sound driver initialization and playback sequence for submarine minigame. */
// FUNCTION: FF7 0x0077f398
void __cdecl Submarine_SendSoundSetupSequence(unsigned char mode)
{
    SoundDispatchCommand(SOUND_MUSIC_VOLUME, 1, 0, 0, 0, 0, 0, 0, 0);
    SoundDispatchCommand(SOUND_SFX_CH1_VOLUME, 0x7E, 0, 0, 0, 0, 0, 0, 0);
    SoundDispatchCommand(SOUND_SFX_CH2_VOLUME, 1, 0, 0, 0, 0, 0, 0, 0);
    SoundDispatchCommand(SOUND_SFX_CH3_VOLUME, 1, 0, 0, 0, 0, 0, 0, 0);
    SoundDispatchCommand(SOUND_SFX_CH4_VOLUME, 0x7E, 0, 0, 0, 0, 0, 0, 0);
    SoundDispatchCommand(SOUND_SFX_PLAY_CH2, 0x40, 0x238, 0, 0, 0, 0, 0, 0);
    SoundDispatchCommand(SOUND_SFX_PLAY_CH3, 0x40, 0x230, 0, 0, 0, 0, 0, 0);
}

// Arms the five-channel interpolation that Submarine_StepRecordInterpolation steps, storing a
// start, an end and an end-minus-start delta for each. It snapshots
// g_SubmarineState.rotationStatePointer, g_SubmarineState.currentAngleA, g_SubmarineState.currentAngleB,
// g_SubmarineState.camOffset and g_SubmarineState.currentAngleC into g_SubmarineRecordInterpBaseA onward and sets
// the targets to -0x200, the current value, the current value, 0x2800 and 0x200 respectively; the
// second channel's delta is written as the literal 0x1000 rather than a difference. It then zeroes
// the progress g_SubmarineRecordInterpFraction, sets the per-step increment
// g_SubmarineRecordInterpStep to 0x10 and raises the interpolation-active flag.
// FUNCTION: FF7 0x0077f468
void __cdecl Submarine_StartStateLerp16(void)
{
    int current;
    int target;

    current = g_SubmarineState.rotationStatePointer;
    g_SubmarineRecordInterpBaseA = current;
    target = 0xFFFFFE00;
    g_SubmarineRecordInterpTargetA = target;
    g_SubmarineRecordInterpDeltaA = target - current;
    current = g_SubmarineState.currentAngleA;
    g_SubmarineInterpStartAngleAHigh = current;
    g_SubmarineInterpTargetAngleAHigh = current;
    g_SubmarineInterpDeltaAngleAHigh = 0x1000;
    current = g_SubmarineState.currentAngleB;
    g_SubmarineRecordInterpBaseB = current;
    g_SubmarineRecordInterpTargetB = current;
    g_SubmarineRecordInterpDeltaB = 0;
    current = g_SubmarineState.camOffset;
    g_SubmarineRecordInterpBaseC = current;
    target = 0x2800;
    g_SubmarineRecordInterpTargetC = target;
    g_SubmarineRecordInterpDeltaC = target - current;
    current = g_SubmarineState.geomScreen;
    g_SubmarineRecordInterpBaseD = current;
    target = 0x200;
    g_SubmarineRecordInterpTargetD = target;
    g_SubmarineRecordInterpDeltaD = target - current;
    g_SubmarineRecordInterpFraction = 0;
    g_SubmarineRecordInterpStep = 0x10;
    g_SubmarineRecordInterpActive = 1;
}

// Resets the submarine minigame for a new run; the first thing Submarine_ModuleEnter does.
// - Frame pacing: the frame interval is the timestamp frequency / 30 minus 10000 ticks (a
//   30 fps frame); clears the frame limiter's on-time flag g_SubmarineFrameOnTime and sets the
//   active flag.
// - Screen layout by UI scaling mode: mode 2 a 640x480 viewport at (0, 0) with the view centre at
//   (320, 270); mode 1 a 320x240 viewport at (160, 120) with the centre at (320, 255); mode 0 a
//   320x240 viewport at (0, 0) with the centre at (160, 135).
// - Clears the game state, flags, audio state, result code and timers; sets the time limit to
//   0x8CA0 (36000) and the countdown to 900 (Submarine_ModuleEnter later replaces both).
// - Copies its local initializers into the working tables: grid buffer words, the model colour
//   mask, an identity and a zero matrix, three axis vectors, the six points and eight triangles
//   of a double pyramid with its point colours, the twelve 0x70-byte object records, the HUD
//   gauge and pitch settings (pitch thresholds 0xA00 and 0xB50), the leading zero/one count
//   tables, and for level blocks 5 down to 0 each block's 64 x 64 map (4,097 bytes), level
//   record (2,048 bytes) and route table (4,096 bytes). This overwrites whatever the data
//   objects held, so their .data contents matter only until the first run.
// - Copies the speed scale table, the four 33-entry lookup tables, the table entries and the
//   player record; sets no torpedoes active, launch offsets 16 and 4 and torpedo speed 16; and
//   copies an 80-byte colour table into g_SubmarineConnectorColor0 and its first 24 bytes into
//   g_SubmarineConnectorColor1.
// FUNCTION: FF7 0x0077f56d
void __cdecl Submarine_InitStateAndTables(void)
{
    GfxEngineContext *p;
    p = GetEngineContext();
    g_SubFrameInterval = p->tscFrequency / 30.0 - 10000.0;
    g_SubmarineFrameOnTime = 0;
    g_SubmarineActiveFlag = 1;
    g_SubmarineRenderMode = GetUiScalingMode();
    switch (g_SubmarineRenderMode) {
    case MAIN_GRAPHICS_MODE_640X480:
        g_SubmarineTargetDims.targetWidth = 640;
        g_SubmarineScreenHeight = 480;
        g_SubmarineTargetDims.targetHeight = 30;
        g_SubmarineViewportCenterX = g_SubmarineTargetDims.targetWidth / 2;
        g_SubmarineViewportCenterY = g_SubmarineScreenHeight / 2 + g_SubmarineTargetDims.targetHeight;
        g_SubmarineViewportLeft = 0;
        g_SubmarineViewportTop = 0;
        g_SubmarineViewportWidth = 640;
        g_SubmarineViewportHeight = 480;
        break;
    case MAIN_GRAPHICS_MODE_320X240_CENTERED:
        g_SubmarineTargetDims.targetWidth = 320;
        g_SubmarineScreenHeight = 240;
        g_SubmarineTargetDims.targetHeight = 15;
        g_SubmarineViewportCenterX = g_SubmarineTargetDims.targetWidth;
        g_SubmarineViewportCenterY = g_SubmarineScreenHeight + g_SubmarineTargetDims.targetHeight;
        g_SubmarineViewportLeft = 160;
        g_SubmarineViewportTop = 120;
        g_SubmarineViewportWidth = 320;
        g_SubmarineViewportHeight = 240;
        break;
    case MAIN_GRAPHICS_MODE_320X240:
        g_SubmarineTargetDims.targetWidth = 320;
        g_SubmarineScreenHeight = 240;
        g_SubmarineTargetDims.targetHeight = 15;
        g_SubmarineViewportCenterX = g_SubmarineTargetDims.targetWidth / 2;
        g_SubmarineViewportCenterY = g_SubmarineScreenHeight / 2 + g_SubmarineTargetDims.targetHeight;
        g_SubmarineViewportLeft = 0;
        g_SubmarineViewportTop = 0;
        g_SubmarineViewportWidth = 320;
        g_SubmarineViewportHeight = 240;
    }
    g_SubmarineGameState = 0;
    g_SubmarineFlags = 0;
    g_SubmarineAudioState.word = 0;
    g_SubmarineResultCode = SUBMARINE_RESULT_NONE;
    g_SubmarineGameRunning = 1;
    g_SubmarineControlFlags = 0;
    g_SubmarineTimerActive = 0;
    g_SubmarineBaseTime = 0;
    (g_SubmarineTimeOffsets.baseTime) = 0;
    g_SubmarineTimeOffsets.countdownStep = 0;
    g_SubmarineAlertState = 0;
    g_SubmarineMinutesElapsed = 0;
    g_SubmarineElapsedFrames = 0;
    g_SubmarineTimeRemaining = 0;
    g_SubmarineTimeLimit = 0x8ca0;
    g_SubmarineCountdown = 900;
    g_SubmarineRemainingTime = 0;
    g_SubmarineModelBoundMinX = -8;
    g_SubmarineModelBoundMaxX = 8;
    g_SubmarineModelBoundMinY = -8;
    g_SubmarineModelBoundMaxY = 8;
    g_SubmarineModelParamA = 896;
    g_SubmarineModelParamB = 0x484;
    {
        SubInitData1 a036 = {{0, 0, 0}};
        SubInitData1 a029 = {{0, 0, 0}};
        SubInitData3 a037 = {{255, 255, 255}};
        Mem_CopyBytesFast(8, &g_SubmarineLargeGridBuffer.initZero_60000, &a036);
        Mem_CopyBytesFast(8, &g_SubmarineLargeGridBuffer.initZero_60008, &a029);
        Mem_CopyBytesFast(4, &g_SubmarineModelColorMask, &a037);
        {
            SubInitData4 a136 = {0x1000, 0, 0, 0, 0x1000, 0, 0, 0, 0x1000, 0, 0, 0};
            SubInitData4 a19 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            g_SubmarineState.geomScreen = 128;
            {
                SubInitData6 a27 = {{-0x1000, 0, 0}};
                SubInitData6 a25 = {{0, -0x1000, 0}};
                SubInitData6 a26 = {{0, 0, -0x1000}};
                Mem_CopyBytesFast(30, &g_SubmarineHudAtlasCoords, &a136);
                Mem_CopyBytesFast(30, &g_SubmarineViewPad, &a19);
                Mem_CopyBytesFast(16, &g_SubmarineState.seedVector, &a27);
                Mem_CopyBytesFast(16, &g_SubmarineState.hudVector, &a25);
                Mem_CopyBytesFast(16, &g_SubmarineState.copyVector, &a26);
                g_SubmarineAlarmActive = 0;
                g_SubmarineState.currentBandBase = 512;
                g_SubmarineState.currentBandStep = 512;
                g_SubmarineState.currentBandOffset = 16;
                g_SubmarineAccumulatedOffset = 0;
                {
                    SubInitData7 a32 = {{-16, 0, 16}, {16, 0, 16}, {-16, 0, -16}, {16, 0, -16}, {0, 32, 0}, {0, -32, 0}};
                    SubInitData8 a31 = {5, 0, 1, 0, 4, 1, 0, 0, 5, 1, 3, 0, 4, 3, 1, 0, 5, 3, 2, 0, 4, 2, 3, 0, 5, 2, 0, 0, 4, 0, 2, 0};
                    SubInitData9 a33 = {255, 0x8080, 0x8080, 255, 255, 0x8080, 0x8080, 255};
                    Mem_CopyBytesFast(48, &g_SubmarineOffsetTable, &a32);
                    Mem_CopyBytesFast(32, &g_SubmarineIndexTable, &a31);
                    Mem_CopyBytesFast(32, &g_SubmarinePointColors, &a33);
                    g_SubmarinePitchThresholdLow = 0xa00;
                    g_SubmarinePitchThresholdHigh = 0xb50;
                    g_SubmarineEffectCount0 = 0;
                    g_SubmarineEffectCount1 = 0;
                    g_SubmarineActiveSlotCount = 0;
                    g_SubmarineSlotTotalWeight = 0;
                    *(int *)&g_SubmarineEffectSlotPad = 0;
                    g_SubmarineEffectActive[0] = 1;
                    {
                        SubInitData13 a428 = {{-8, 8, -16}, {0x1000, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0x1000, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {2, 0, 0, 0, 0, 0, 0, -6, 8, -16}, {0x1000, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0x1200, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 5, 0, 0, 0, 0, 0, -4, 8, -16}, {0x1000, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0x800, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 10, 0, 0, 0, 0, 0, -2, 8, -16}, {0x1000, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0x1800, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 14, 0, 0, 0, 0, 0, 0, 8, -16}, {0x1000, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0xc00, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 8, -16}, {0x1000, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0x1000, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 5, 0, 0, 0, 0, 0, 4, 8, -16}, {0x1000, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0x2000, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 10, 0, 0, 0, 0, 0, 6, 8, -16}, {0x1000, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0x1000, 0x1000, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 15, 0, 0, 0, 0, 0, 6, 8, -16}, {0x1000, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0x1000, 0x1000, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 15, 0, 0, 0, 0, 0, 6, 8, -16}, {0x1000, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0x1000, 0x1000, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 15, 0, 0, 0, 0, 0, 6, 8, -16}, {0x1000, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0x1000, 0x1000, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 15, 0, 0, 0, 0, 0, 6, 8, -16}, {0x1000, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0x1000, 0x1000, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 15, 0, 0, 0, 0, 0};
                        Mem_CopyBytesFast(1344, &g_SubmarineModelVertexData, &a428);
                        g_SubmarineHudScale = 96;
                        g_SubmarineHudFlag = 1;
                        g_SubmarineHudParams.musicVolume = 1;
                        g_SubmarineHudClearedWord = 0;
                        g_SubmarineHudScrollPos = 0;
                        g_SubmarineHudBarAnimFrame = 0;
                        g_SubmarineHudBarAnimIndex = 0;
                        g_SubmarineHudValueQuad5Timer = 0;
                        g_SubmarineHudValueQuad1Timer = 0;
                        g_SubmarineHudValueQuad3Timer = 0;
                        g_SubmarineHudParams.screenShakeTimer = 0;
                        g_SubmarineHudRange.pitch = 0x400;
                        g_SubmarineHudRange.max = 0x3e8;
                        g_SubmarineHudOffset = 512;
                        g_SubmarineHudOrigin = -128;
                        g_SubmarineScreenShake.pitchStep = 0x673;
                        g_SubmarineScreenShake.pitchAngle = 0;
                        g_SubmarineScreenShake.yawAngle = 0x623;
                        g_SubmarineScreenShake.rollAngle = 0;
                        g_SubmarineScreenShake.accumulator = 0;
                        g_SubmarineScreenShake.projectionOffsetX = 0;
                        g_SubmarineScreenShake.projectionOffsetY = 0;
                        Bit_InitLeadingZeroCountTable();
                        Bit_InitLeadingOneCountTable();
                        {
                            // Keep adjacent 64-byte tokens on one annotation line (VC5 literal limit).
                            // clang-format off
                            // STRING: FF7 0x00981000
                            char a59[4097] = "0000000000000000000000000000000000000000000000000000000000000000" "0000222244444444444444444444444444444444444444444444444444322220" "0000222477777777777777777777777777777777777777777777777774332220" "0000222477777777777777777777777777777777777777777777777774433220" "0000222477777777777777777777777777777777777777777777777774443320" "0222222477777777777777777777777777444444444444477777777777444320" "0333334777776666777777777777777744000000000000047777777777444430" "0444447777444444777777777777444222220000000000004477777777774440" "0477777774444224777777777774442220000000000000004477777777777740" "0477777774420244667777777774442220000000000000004777777777777740" "0444447774420244667777777774400000000000000000047777777777777740" "0333347774200244667777777774400000000000000000004777777777777740" "0333347774200144667777777774444400000000000000000477777777777740" "0444447774200044667777777777444400000000000000000044777777777740" "0477777774400444777777777777777744000000000000000044477777777740" "0477777774400444477777777777777777440000000000000044777777777740" "0477777777700047777777777777777777775000000000000047777777777740" "0477777777400444444444777777777777751100000000000477777777777740" "0477777774000000000004477777777777751100000000000477777777777740" "0477777774000000000000247777777777751100000000000477777777777740" "0344447744000000000000004777777777751100000000000477777777777740" "0344444777442000000000002447777777775510000000004777777777777740" "0333344777777422200000000447777777777750000000004777777777777740" "0333344777777442200000000044777777777744000000004777777777777740" "0333344777777442000000000004447777774400000000000477777777744440" "0334447777777744400000000000447777774000000004444477777777422220" "0347777777777400000000000000047777774000000044777777777774200000" "0334477777777440000000000000477777777400444477777777777742000000" "0447777777744740000000000000477777777744777777777777777420000000" "0477777777400000000000000004777777777777777777777777774200000000" "0477777774000000000000000004777777777777777777777777774200000000" "0477777774000000000000000047777777777777777777777777742000000000" "0347777777444000000000044477777777777777777777777777420000000000" "0334444777777444444444477777777777777777777777777777420000000000" "0333444777777777777777777777777777777777777777777774200000000000" "0333444777777777777777777777777777777777777777777777420000000000" "0333447777777777777777777777777777777777444447777777742000000000" "0444777777777777777777777777777777777777400047777777774444444440" "0477777744444444444444447777777777777743000004477777777777777740" "0477774300000000000000000447777777777774300000047777777777777740" "0477774300000000000000000047777777777774300000004447777777777740" "0477774300000000000000000004777777777774300000000004777777777740" "0477774322000000000000000004777777777777400000000047777777777740" "0444777755444440000000000000477777777777400000000047777777777740" "0333777777555540000000000000047777777777743000000004444447777740" "0344777777744440000000000000477777777777777400000000000047777740" "0344777777744440000000000004777777777777777743000000000047777740" "0344777777777744200000000247777777777777777777400000000477777740" "0334777777777442000000000047777777777777777777740000004777777740" "0447777777777442000000000004777777777777777777774444447777777740" "0447777777774420000000000000477777777777777777777777777777777740" "0447777777744200000000000000047777777777777777777777777777777740" "0444777777442000000000000000047777777777777777777777777777777740" "0447777777443000000000000000477777777777777744777777777777777740" "0444777774433300000000000000047777777777444000477777777777777740" "0444777774430000000000000000004777777744400000477777777777777740" "0347777744430000000000000000047777777400000000477777777777777740" "0344777777443300000000000044477777774000000000477777777777777740" "0334477774433000000000000000444777774000000000047777777744444440" "0333477744330000000000000000044444440000000000047777744433222220" "0333447774433000000000000000033333300000000000047777444433222220" "0333444777443300000000000000000000000000000000004444444433222220" "0333344444444400000000000000000000000000000000000044444332222220" "0000000000000000000000000000000000000000000000000000000000000000";
                            // clang-format on
                            SubInitData17 a536 = {{0x8ca0, 8, 0, 200, 1, 640, 0x6000, 0, 640, 0x4000, 0, -27, 11, -28}, {0x4000, 0, 0, 0}, {0x3000, 0, 1, -27, 8, -28}, {0x1770, 0, 0, 0}, {300, 0x36b0, 1, 2, 0, -28, 10, -15}, {0x7d0, 0, 0, 0}, {300, 0x2710, 1, 0, 2, -28, 4, 0}, {0x7d0, 0, 0, 0}, {300, 0x1770, 1, 0, 16, 0, 10, -4}, {0x7d0, 0, -0x400, 0}, {0, 0x2710, 1, 0, 29, 20, 10, -2}, {0x7d0, 0, 0x600, 0}, {0, 0x1000, 1, 0, 35, -2, 10, 11}, {0x7d0, 0, 0, 0}, {0, 0x1800, 0, 0, 40, -9, 13, 17}, {0x7d0, 0, 0, 0}, {0, 0xbb8, 1, 0, 47, -2, 12, 0}, {0x7d0, 0, 0x400, 0}, {0x1000, 0x2710, 0, 0, 58, 12, 10, 3}, {0x7d0, 0, -0x400, 0}, {0, 0x7d0, 1, 0, 62, 25, 12, -9}, {0x7d0, 0, 0x800, 0}, {0x1000, 0x1000, 0, 3, 70, 20, 5, 27}, {100, 0, 0x800, 0}, {0x1830, 0x4e20, 1, 0, 87, 25, 12, -9}, {0xbb8, 0, -0x400, 0}, 0x1000, 0x2710, 0, 3, 98};
                            SubInitData18 a529 = {{-27, 10, -6}, {128, 0x5208, 1, 0, -24, 12, 9}, {128, 0x4e20, 2, 0, -25, 8, 25}, {128, 0x36b0, 3, 0, -15, 13, 27}, {128, 0x2710, 4, 0, -7, 8, 14}, {128, 0x2710, 5, 0, 3, 6, 4}, {128, 0x2710, 6, 0, -2, 14, -18}, {128, 0x3e80, 7, 0, 23, 8, -19}, {128, 0x2710, 8, 0, 27, 6, -13}, {128, 0x4030, 9, 0, 15, 10, -6}, {128, 0x4030, 10, 0, 23, 6, 16}, {128, 0x2710, 11, 0, 19, 13, 27}, {128, 0x2710, 12, 0, -9, 10, 29}, {128, 0x2710, 13, 0, -7, 6, 14}, {128, 0x3e80, 14, 0, 3, 6, 4}, {128, 0x3e80, 15, 0, -24, 10, -6}, {128, 0x1830, 2, 0, -25, 8, 25}, {128, 0x36b0, 17, 0, -15, 13, 27}, {128, 0x2710, 18, 0, -7, 8, 14}, {128, 0x2710, 19, 0, 3, 8, 4}, {128, 0x2710, 20, 0, -2, 14, -18}, {128, 0x3e80, 21, 0, 23, 7, -19}, {128, 0x2710, 22, 0, 27, 8, -13}, {128, 0x4030, 23, 0, 15, 10, -6}, {128, 0x4030, 24, 0, 23, 8, 16}, {128, 0x2710, 25, 0, 19, 8, 27}, {128, 0x2710, 26, 0, -9, 8, 29}, {128, 0x2710, 27, 0, -7, 12, 14}, {128, 0x3e80, 28, 0, 3, 8, 4}, {128, 0x2710, 100, 0, -26, 9, -3}, {128, 0x2710, 30, 0, -25, 9, 25}, {128, 0x2710, 31, 0, -17, 14, 28}, {128, 0x1830, 32, 0, 19, 14, 28}, {128, 0x2710, 33, 0, 22, 6, 8}, {128, 0x2710, 34, 0, 0, 10, -4}, {128, 0x2710, 29, 0, 25, 8, -10}, {128, 0x2710, 36, 0, 24, 13, -20}, {128, 0x2710, 37, 0, -2, 7, -14}, {128, 0x2710, 38, 0, 0, 13, -4}, {128, 0x2710, 39, 0, 16, 14, -1}, {128, 0x1830, 35, 0, -9, 10, 26}, {192, 0x1830, 41, 0, -9, 10, 27}, {192, 0x2710, 42, 0, -23, 10, 27}, {128, 0x2710, 43, 0, -27, 8, 22}, {128, 0x1830, 44, 0, -27, 10, -4}, {256, 0x4650, 45, 0, 0, 13, -4}, {256, 0x4030, 46, 0, -2, 13, 11}, {256, 0x4030, 41, 0, -9, 10, 27}, {192, 0x2710, 48, 0, -23, 10, 27}, {128, 0x2710, 49, 0, -27, 8, 22}, {128, 0x1830, 50, 0, -27, 10, -4}, {256, 0x4650, 51, 0, 0, 10, -4}, {256, 0x4030, 52, 0, 0, 10, -19}, {256, 0x4030, 53, 0, 25, 10, -19}, {256, 0x4030, 54, 0, 25, 10, -10}, {256, 0x4030, 55, 0, 14, 10, 1}, {256, 0x4030, 56, 0, 2, 10, 1}, {256, 0x4030, 47, 0, -2, 10, 11}, {256, 0x4030, 47, 0, 13, 9, 3}, {128, 0x1770, 59, 0, 7, 11, -1}, {256, 0x1830, 60, 0, -3, 9, -8}, {256, 0x1770, 61, 0, -2, 11, 0}, {256, 0x1770, 58, 0, 2, 9, 3}, {128, 0x1770, 63, 0, -4, 11, 16}, {128, 0x4030, 64, 0, -9, 9, 20}, {128, 0x4030, 65, 0, -11, 11, 27}, {128, 0x1830, 66, 0, 20, 11, 27}, {128, 0x1830, 67, 0, 22, 11, 27}, {128, 0x3e80, 68, 0, 22, 11, 9}, {128, 0x3e80, 69, 0, 12, 10, 3}, {128, 0x4030, 62, 0, 25, 12, 7}, {128, 0x2710, 71, 0, 15, 13, -4}, {128, 0x2710, 72, 0, 27, 12, -13}, {128, 0x1770, 73, 0, 23, 11, -19}, {128, 0x2710, 74, 0, -2, 8, -18}, {128, 0x2710, 75, 0, -2, 12, 4}, {128, 0x3e80, 76, 0, -4, 11, 14}, {128, 0x4030, 77, 0, -15, 10, 27}, {128, 0x4030, 78, 0, -25, 8, 27}, {128, 0x1830, 79, 0, -26, 6, 4}, {128, 0x2710, 80, 0, -26, 6, -4}, {128, 0x2710, 81, 0, -3, 13, -4}, {128, 0x3e80, 82, 0, 3, 8, 4}, {128, 0x2710, 16, 0, 3, 12, 4}, {128, 0x3e80, 84, 0, -7, 6, 14}, {128, 0x4030, 85, 0, -15, 13, 27}, {128, 0x4030, 86, 0, 25, 6, 25}, {128, 0x2710, 70, 0, -16, 5, 14}, {256, 0x4650, 88, 0, 18, 5, 4}, {256, 0x4650, 89, 0, -18, 5, -8}, {256, 0x4650, 90, 0, 18, 5, -18}, {256, 0x4650, 91, 0, -18, 5, -18}, {256, 0x4650, 92, 0, 18, 5, -5}, {256, 0x4650, 93, 0, -18, 5, 5}, {256, 0x4650, 94, 0, 18, 5, 15}, {256, 0x4650, 95, 0, -18, 5, 18}, {256, 0x4650, 96, 0, 18, 5, 18}, {256, 0x4650, 87, 0, 18, 5, 18}, {128, 0x3e80, 87, 0, 20, 6, 27}, {128, 0x3e80, 98, 0, 20, 6, 27}, {128, 0x3e80, 99, 0, -24, 6, -6}, {128, 0x1830, 101, 0, -28, 6, 0}, 128, 0x2710, 16, 0};
                            Mem_CopyBytesFast(4097, &g_SubmarineLevel5Map, &a59);
                            Mem_CopyBytesFast(2048, &g_SubmarineLevel5Record, &a536);
                            Mem_CopyBytesFast(4096, &g_SubmarineLevel5RouteTable, &a529);
                            {
                                // Keep adjacent 64-byte tokens on one annotation line (VC5 literal limit).
                                // clang-format off
                                // STRING: FF7 0x00982004
                                char a66[4097] = "0000000000000000000000000000000000000000000000000000000000000000" "0000222244444444444444444444444444444444444444444444444444322220" "0000222477777777777777777777777777777777777777777777777774332220" "0000222477777777777777777777777777777777777777777777777774433220" "0000222477777777777777777777777777777777777777777777777774443320" "0222222477777777777777777777777777444444444444477777777777444320" "0333334777776666777777777777777744000000000000047777777777444430" "0444447777444444777777777777444222220000000000004477777777774440" "0477777774444224777777777774442220000000000000004477777777777740" "0477777774420244667777777774442220000000000000004777777777777740" "0444447774420244667777777774400000000000000000047777777777777740" "0333347774200244667777777774400000000000000000004777777777777740" "0333347774200144667777777774444400000000000000000477777777777740" "0444447774200044667777777777444400000000000000000044777777777740" "0477777774400444777777777777777744000000000000000044477777777740" "0477777774400444477777777777777777440000000000000044777777777740" "0477777777700047777777777777777777775000000000000047777777777740" "0477777777400444444444777777777777751100000000000477777777777740" "0477777774000000000004477777777777751100000000000477777777777740" "0477777774000000000000247777777777751100000000000477777777777740" "0344447744000000000000004777777777751100000000000477777777777740" "0344444777442000000000002447777777775510000000004777777777777740" "0333344777777422200000000447777777777750000000004777777777777740" "0333344777777442200000000044777777777744000000004777777777777740" "0333344777777442000000000004447777774400000000000477777777744440" "0334447777777744400000000000447777774000000004444477777777422220" "0347777777777400000000000000047777774000000044777777777774200000" "0334477777777440000000000000477777777400444477777777777742000000" "0447777777744740000000000000477777777744777777777777777420000000" "0477777777400000000000000004777777777777777777777777774200000000" "0477777774000000000000000004777777777777777777777777774200000000" "0477777774000000000000000047777777777777777777777777742000000000" "0347777777444000000000044477777777777777777777777777420000000000" "0334444777777444444444477777777777777777777777777777420000000000" "0333444777777777777777777777777777777777777777777774200000000000" "0333444777777777777777777777777777777777777777777777420000000000" "0333447777777777777777777777777777777777444447777777742000000000" "0444777777777777777777777777777777777777400047777777774444444440" "0477777744444444444444447777777777777743000004477777777777777740" "0477774300000000000000000447777777777774300000047777777777777740" "0477774300000000000000000047777777777774300000004447777777777740" "0477774300000000000000000004777777777774300000000004777777777740" "0477774322000000000000000004777777777777400000000047777777777740" "0444777755444440000000000000477777777777400000000047777777777740" "0333777777555540000000000000047777777777743000000004444447777740" "0344777777744440000000000000477777777777777400000000000047777740" "0344777777744440000000000004777777777777777743000000000047777740" "0344777777777744200000000247777777777777777777400000000477777740" "0334777777777442000000000047777777777777777777740000004777777740" "0447777777777442000000000004777777777777777777774444447777777740" "0447777777774420000000000000477777777777777777777777777777777740" "0447777777744200000000000000047777777777777777777777777777777740" "0444777777442000000000000000047777777777777777777777777777777740" "0447777777443000000000000000477777777777777744777777777777777740" "0444777774433300000000000000047777777777444000477777777777777740" "0444777774430000000000000000004777777744400000477777777777777740" "0347777744430000000000000000047777777400000000477777777777777740" "0344777777443300000000000044477777774000000000477777777777777740" "0334477774433000000000000000444777774000000000047777777744444440" "0333477744330000000000000000044444440000000000047777744433222220" "0333447774433000000000000000033333300000000000047777444433222220" "0333444777443300000000000000000000000000000000004444444433222220" "0333344444444400000000000000000000000000000000000044444332222220" "0000000000000000000000000000000000000000000000000000000000000000";
                                // clang-format on
                                SubInitData17 a64 = {{0x8ca0, 8, 0, 200, 1, 640, 0x6000, 0, 640, 0x4000, 0, -27, 11, -28}, {0x4000, 0, 0, 0}, {0x3000, 0, 1, -27, 8, -28}, {0x1770, 0, 0, 0}, {300, 0x36b0, 1, 2, 0, -28, 10, -15}, {0x7d0, 0, 0, 0}, {300, 0x2af8, 1, 3, 2, -28, 4, 0}, {0x7d0, 0, 0, 0}, {300, 0x1b58, 1, 3, 16, 0, 10, -4}, {0x7d0, 0, -0x400, 0}, {0, 0x2ee0, 1, 0, 29, 20, 10, -2}, {0x7d0, 0, 0x600, 0}, {0, 0x1000, 1, 0, 35, -2, 10, 11}, {0x7d0, 0, 0, 0}, {0, 0x1800, 0, 0, 40, -9, 13, 17}, {0x7d0, 0, 0, 0}, {0, 0xbb8, 1, 0, 47, -2, 12, 0}, {0x7d0, 0, 0x400, 0}, {0x1000, 0x3e80, 0, 0, 58, 12, 10, 3}, {0x7d0, 0, -0x400, 0}, {0, 0x7d0, 1, 0, 62, 25, 12, -9}, {0x7d0, 0, 0x800, 0}, {0x1000, 0x1000, 0, 3, 70, 20, 5, 27}, {100, 0, 0x800, 0}, {0x2000, 0x4a38, 1, 3, 87, 25, 12, -9}, {0x7d0, 0, -0x400, 0}, 0x1000, 0x3e80, 0, 3, 98};
                                SubInitData18 a65 = {{-27, 10, -6}, {128, 0x4a38, 1, 0, -24, 12, 9}, {128, 0x4a38, 2, 0, -25, 8, 25}, {128, 0x3e80, 3, 0, -15, 13, 27}, {128, 0x36b0, 4, 0, -7, 8, 14}, {128, 0x3e80, 5, 0, 3, 6, 4}, {128, 0x36b0, 6, 0, -2, 14, -18}, {128, 0x4a38, 7, 0, 23, 8, -19}, {128, 0x36b0, 8, 0, 27, 6, -13}, {128, 0x4030, 9, 0, 15, 10, -6}, {128, 0x4030, 10, 0, 23, 6, 16}, {128, 0x3e80, 11, 0, 19, 13, 27}, {128, 0x3e80, 12, 0, -9, 10, 29}, {128, 0x36b0, 13, 0, -7, 6, 14}, {128, 0x4a38, 14, 0, 3, 6, 4}, {128, 0x4a38, 15, 0, -24, 10, -6}, {128, 0x2000, 2, 0, -25, 8, 25}, {128, 0x3e80, 17, 0, -15, 13, 27}, {128, 0x36b0, 18, 0, -7, 8, 14}, {128, 0x3e80, 19, 0, 3, 8, 4}, {128, 0x36b0, 20, 0, -2, 14, -18}, {128, 0x4a38, 21, 0, 23, 7, -19}, {128, 0x36b0, 22, 0, 27, 8, -13}, {128, 0x4030, 23, 0, 15, 10, -6}, {128, 0x4030, 24, 0, 23, 8, 16}, {128, 0x3e80, 25, 0, 19, 8, 27}, {128, 0x3e80, 26, 0, -9, 8, 29}, {128, 0x36b0, 27, 0, -7, 12, 14}, {128, 0x4a38, 28, 0, 3, 8, 4}, {128, 0x3e80, 100, 0, -26, 9, -3}, {128, 0x36b0, 30, 0, -25, 9, 25}, {128, 0x36b0, 31, 0, -17, 14, 28}, {128, 0x2000, 32, 0, 19, 14, 28}, {128, 0x3e80, 33, 0, 22, 6, 8}, {128, 0x3e80, 34, 0, 0, 10, -4}, {128, 0x3e80, 29, 0, 25, 8, -10}, {128, 0x3e80, 36, 0, 24, 13, -20}, {128, 0x3e80, 37, 0, -2, 7, -14}, {128, 0x36b0, 38, 0, 0, 13, -4}, {128, 0x36b0, 39, 0, 16, 13, -1}, {128, 0x2000, 35, 0, -9, 10, 26}, {192, 0x2000, 41, 0, -9, 10, 27}, {192, 0x3e80, 42, 0, -23, 10, 27}, {128, 0x3e80, 43, 0, -27, 8, 22}, {128, 0x2000, 44, 0, -27, 10, -4}, {256, 0x4650, 45, 0, 0, 13, -4}, {256, 0x4030, 46, 0, -2, 13, 11}, {256, 0x4030, 41, 0, -9, 10, 27}, {192, 0x3e80, 48, 0, -23, 10, 27}, {128, 0x3e80, 49, 0, -27, 8, 22}, {128, 0x2000, 50, 0, -27, 10, -4}, {256, 0x4650, 51, 0, 0, 10, -4}, {256, 0x4030, 52, 0, 0, 10, -19}, {256, 0x4030, 53, 0, 25, 10, -19}, {256, 0x4030, 54, 0, 25, 10, -10}, {256, 0x4030, 55, 0, 14, 10, 1}, {256, 0x4030, 56, 0, 2, 10, 1}, {256, 0x4030, 47, 0, -2, 10, 11}, {256, 0x4030, 47, 0, 13, 9, 3}, {128, 0x3000, 59, 0, 7, 11, -1}, {256, 0x2000, 60, 0, -3, 9, -8}, {256, 0x3000, 61, 0, -2, 11, 0}, {256, 0x3000, 58, 0, 2, 9, 3}, {128, 0x3000, 63, 0, -4, 11, 16}, {128, 0x4030, 64, 0, -9, 9, 20}, {128, 0x4030, 65, 0, -11, 11, 27}, {128, 0x2000, 66, 0, 20, 11, 27}, {128, 0x2000, 67, 0, 22, 11, 27}, {128, 0x4a38, 68, 0, 22, 11, 9}, {128, 0x4a38, 69, 0, 12, 10, 3}, {128, 0x4030, 62, 0, 25, 12, 7}, {128, 0x3e80, 71, 0, 15, 13, -4}, {128, 0x36b0, 72, 0, 27, 12, -13}, {128, 0x3000, 73, 0, 23, 11, -19}, {128, 0x36b0, 74, 0, -2, 8, -18}, {128, 0x36b0, 75, 0, -2, 12, 4}, {128, 0x4a38, 76, 0, -4, 11, 14}, {128, 0x4030, 77, 0, -15, 10, 27}, {128, 0x4030, 78, 0, -25, 8, 27}, {128, 0x2000, 79, 0, -26, 6, 4}, {128, 0x3e80, 80, 0, -26, 6, -4}, {128, 0x36b0, 81, 0, -3, 13, -4}, {128, 0x4a38, 82, 0, 3, 8, 4}, {128, 0x3e80, 16, 0, 3, 12, 4}, {128, 0x4a38, 84, 0, -7, 6, 14}, {128, 0x4030, 85, 0, -15, 13, 27}, {128, 0x4030, 86, 0, 25, 6, 25}, {128, 0x3e80, 70, 0, -16, 5, 14}, {256, 0x4a38, 88, 0, 18, 5, 4}, {256, 0x4a38, 89, 0, -18, 5, -8}, {256, 0x4a38, 90, 0, 18, 5, -18}, {256, 0x4a38, 91, 0, -18, 5, -18}, {256, 0x4a38, 92, 0, 18, 5, -5}, {256, 0x4a38, 93, 0, -18, 5, 5}, {256, 0x4a38, 94, 0, 18, 5, 15}, {256, 0x4a38, 95, 0, -18, 5, 18}, {256, 0x4a38, 96, 0, 18, 5, 18}, {256, 0x4a38, 87, 0, 18, 5, 18}, {128, 0x4a38, 87, 0, 20, 6, 27}, {128, 0x4a38, 98, 0, 20, 6, 27}, {128, 0x4a38, 99, 0, -24, 6, -6}, {128, 0x2000, 101, 0, -28, 6, 0}, 128, 0x3e80, 16, 0};
                                Mem_CopyBytesFast(4097, &g_SubmarineLevel4Map, &a66);
                                Mem_CopyBytesFast(2048, &g_SubmarineLevel4Record, &a64);
                                Mem_CopyBytesFast(4096, &g_SubmarineLevel4RouteTable, &a65);
                                {
                                    // Keep adjacent 64-byte tokens on one annotation line (VC5 literal limit).
                                    // clang-format off
                                    // STRING: FF7 0x00983008
                                    char a71[4097] = "0000000000000000000000000000000000000000000000000000000000000000" "0000222244444444444444444444444444444444444444444444444444322220" "0000222488888888888888888888888888888888888888888888888884332220" "0000222488888888888888888888888888888888888888888888888884433220" "0000222488888888888888888888888888888888888888888888888884443320" "0222222488888888888888888888888888888888888888888888888888444320" "0333334888886668888888888888444444444444444444446666888888844430" "0444448888444444888888888888444444422222000112224466888888884440" "0488888884444224888888888888666644422200000122224466888888888840" "0488888884420244668888888888888844422200333334444466888888888840" "0488888884420244668888888888888866664444444446666666888888888840" "0488888884200244668888888888888866666666666666666668888888888840" "0488888884200144668888888888888888888888888888888888888888888840" "0488888884222444668888888888888888888888888888888888888888888840" "0488888884444444888888888888888888888855557788888888888888888840" "0488888888888888888888888888888888888522225778888888888888888840" "0488888887744778888888887788888888875111111157888877777788888840" "0488888887400448888887774477888888751100001157888772222288888840" "0488888884000048887774444447788888751100001157888772222238888840" "0488888884000048874444222247888888751100001157888722211138888840" "0488888884000478874222202247888888751100001157888722200038888840" "0488888887444778874222002447888888885511115578887742200038888840" "0488888888888888874222024478888888888755557788887742222238888840" "0488888888888888874422244788888888888888778888888774444488888840" "0488888888888888887442447888888888888888888888888888888888888840" "0488888888888888888744478888888888888888888887778888888888888840" "0488888888888888888877788888888888888888777777766666888888888840" "0488888888888888888887888888888888888888777776665566667788888840" "0488888887744778888888888888888888888777777666555566677788888840" "0488888887400448888888888888888888877777776666556666777788888840" "0488888884000048888888888888888888877777777666666667777788888840" "0488888884000048888888888888888888877777777554444455777788888840" "0488888887444778888888888888888877766666444000000044666688888840" "0488888888888888888888888888888877766664444000000022466688888840" "0488888888888888888888888888888877766666444222222222666688888840" "0488888888888888888888888888888877766666444444444444666688888840" "0488888888888888888888888888888877777666666666666666677788888840" "0488888887777777777777888888888888888888777777777777777788888840" "0488888866666666666666665555688888888888888888888888888888888840" "0488887654333333333333334445688888888888888888888888888888888840" "0488887654333000000223333345688888888888888888888888888888888840" "0488887654322000000223333345688888888888888888888888888888888840" "0488887654322222234567888888888877788888885555558888888888888840" "0488888855444444334888888888886666677778885444458888888888888840" "0488888888555555888888888888444444477777885444458888888888888840" "0488888888888888888444488864222222555555885444458888888888888840" "0488888888888888888444488666332222444455885555558888888888888840" "0488888888888888442222448866633221110455888888888888888888888840" "0488888888888888442112448876663322000455888888888888888888888840" "0488888888888888442112448887663322011455888888888888888888888840" "0488888888888888442002448887777655422455888888888888888888888840" "0488888888888888442002448888777655422455888888888888888888888840" "0488888888888888442222448888777655444455888888888888888888888840" "0488888888888888442222448888777766444455888888888888888888888840" "0488888888888888444444447888887788888888888888888888888888844440" "0488888888888888777777777788887788888888888888888888888888444440" "0348888888888888888888888888888888888888888888888888888843322220" "0344888888888888888888888888888888888888888888888888888843322220" "0334448888888888888888888888866666688888888888888888844433222220" "0333444888888888888888888888866446688888888888888888444433222220" "0333444488888888888888888888866446688888888888888884444433222220" "0333444488888888888888888888888888888888888888888844444433222220" "0333344444444444444444444444444444444444444444444444444332222220" "0000000000000000000000000000000000000000000000000000000000000000";
                                    // clang-format on
                                    SubInitData17 a70 = {{0x8ca0, 8, 0, 200, 0, 640, 0x6000, 0, 640, 0x4000, 0, 0, 11, -28}, {0x4000, 0, 0, 0}, {0x3000, 0, 1, -22, 16, -15}, {0x7d0, 0, 0x800, 0}, {0, 0x2000, 1, 0, 0, 21, 14, -18}, {0x7d0, 0, -512, 0}, {0, 0x2000, 1, 0, 5, -28, 14, 22}, {0x7d0, 0, 0x800, 0}, {0, 0x2ee0, 1, 3, 36, 0, 13, -26}, {0xbb8, 0, 0, 0}, {0, 0x2000, 1, 0, 20, 2, 12, 1}, {0x7d0, 0, 0x800, 0}, {0, 0x2000, 1, 0, 26, 0, 14, -2}, {0x7d0, 0, -0x400, 0}, {0, 0x2000, 1, 0, 30, -9, 6, 22}, {0x7d0, 0x800, -0x400, 0}, {0, 0x2000, 0, 0, 33, -28, 12, 22}, {0x1770, 0, 0x800, 0}, {0x1000, 0x3e80, 1, 2, 36, 25, 12, 22}, {0x7d0, 0, 0x800, 0}, {0x1000, 0x1000, 0, 0, 44, 0, 15, 20}, {0xbb8, 0, 0x400, 0}, {0, 0x1000, 1, 0, 44, -28, 12, 22}, {0x7d0, 0, 0x800, 0}, {0x1000, 0x3e80, 1, 0, 49, 27, 13, 18}, {0xbb8, 0, 0x800, 0}, 0x1000, 0x3e80, 1, 3, 57};
                                    SubInitData19 a72 = {{-20, 14, -27}, {128, 0x3e80, 1, 0, -7, 12, -26}, {128, 0x2000, 2, 0, -8, 12, -16}, {128, 0x3e80, 3, 0, -10, 12, -12}, {128, 0x2000, 4, 0, -22, 14, -15}, {128, 0x3e80, 0, 0, 16, 12, -27}, {128, 0x2000, 6, 0, 0, 12, -27}, {128, 0x3e80, 7, 0, -5, 10, -22}, {128, 0x3e80, 20, 0, -10, 12, -12}, {128, 0x2000, 9, 0, 2, 12, -9}, {128, 0x3e80, 10, 0, 22, 13, -8}, {128, 0x3e80, 11, 0, 21, 12, -18}, {128, 0x3e80, 5, 0, -28, 6, 7}, {128, 0x3e80, 37, 0, -8, 6, 4}, {128, 0x36b0, 14, 0, -7, 8, 0}, {128, 0x3e80, 15, 0, 23, 8, -7}, {128, 0x36b0, 16, 0, 25, 14, 18}, {128, 0x3e80, 17, 0, -10, 8, 20}, {128, 0x36b0, 18, 0, -17, 10, 28}, {128, 0x36b0, 19, 0, -28, 14, 22}, {128, 0x3e80, 12, 0, -8, 12, -13}, {128, 0x3000, 21, 0, 12, 13, -7}, {128, 0x3000, 22, 0, 22, 13, -9}, {128, 0x2ee0, 23, 0, 22, 13, -24}, {128, 0x3000, 5, 0, 22, 14, -13}, {128, 0x2ee0, 25, 0, 24, 14, -24}, {128, 0x2000, 11, 0, -6, 10, -6}, {128, 0x36b0, 27, 0, -24, 14, -3}, {128, 0x3e80, 28, 0, -26, 12, 6}, {128, 0x3e80, 29, 0, 2, 14, 1}, {128, 0x36b0, 26, 0, -15, 14, 4}, {128, 0x2000, 31, 0, -15, 14, 16}, {128, 0x3e80, 32, 0, -2, 14, 16}, {128, 0x3e80, 33, 0, -2, 14, 4}, {128, 0x3e80, 30, 0, 1, 12, 18}, {128, 0x1000, 35, 0, -10, 12, 23}, {128, 0x2000, 26, 0, -28, 12, 7}, {128, 0x3e80, 37, 0, -8, 10, 4}, {128, 0x36b0, 38, 0, -7, 12, -2}, {128, 0x3e80, 39, 0, 24, 13, -7}, {128, 0x4a38, 40, 0, 25, 14, 18}, {128, 0x3e80, 41, 0, -10, 12, 20}, {128, 0x36b0, 42, 0, -17, 10, 28}, {128, 0x36b0, 43, 0, -28, 14, 22}, {128, 0x3e80, 36, 0, 24, 15, 20}, {128, 0x2000, 45, 0, 24, 15, 26}, {128, 0x1000, 46, 0, -9, 15, 28}, {128, 0x3e80, 47, 0, -9, 15, 20}, {128, 0x1000, 44, 0, 0, 12, 19}, {128, 0x1000, 46, 0, -28, 12, 22}, {128, 0x2000, 50, 0, -17, 11, 28}, {128, 0x2000, 51, 0, -10, 10, 20}, {128, 0x2000, 52, 0, 25, 13, 18}, {128, 0x3e80, 53, 0, 24, 10, -7}, {128, 0x1000, 54, 0, -7, 11, 0}, {128, 0x3e80, 55, 0, -8, 11, 4}, {128, 0x2000, 56, 0, -28, 10, 7}, {128, 0x2000, 49, 0, 27, 13, 7}, {128, 0x3000, 58, 0, 14, 13, 7}, {128, 0x2000, 59, 0, 15, 13, 18}, {128, 0x3000, 60, 0, 1, 13, 18}, {128, 0x2000, 61, 0, 1, 13, 7}, {128, 0x3000, 62, 0, 14, 13, 8}, {128, 0x2000, 63, 0, 15, 13, 18}, {128, 0x3000, 64, 0, 27, 13, 18}, 128, 0x2000, 57, 0};
                                    Mem_CopyBytesFast(4097, &g_SubmarineLevel3Map, &a71);
                                    Mem_CopyBytesFast(2048, &g_SubmarineLevel3Record, &a70);
                                    Mem_CopyBytesFast(4096, &g_SubmarineLevel3RouteTable, &a72);
                                    {
                                        // Keep adjacent 64-byte tokens on one annotation line (VC5 literal limit).
                                        // clang-format off
                                        // STRING: FF7 0x0098400c
                                        char a818[4097] = "0000000000000000000000000000000000000000000000000000000000000000" "0000222233334444444444444444444444444444444444444444444444320000" "0000222333334446666677777777777777777777777777777777776666330000" "0000222333334446667777777777777777777777777777777777777766633000" "0000222333344446667777777777777777777777777777777777777774443320" "0222222333344466677777777777777777777744444444444444444777444320" "0333333334444666777777777777777777744422222222222222222477444430" "0444444444446667777777777777777777422222222222222222222447774440" "0444444444666777777777777777777777433333333333333333333347777740" "0466666666677777777777777777777777433333333333333333333347777740" "0433347777777777777777777777777777422222222222222222222247777740" "0433347777444447777777777777777777742222222222222222222247777740" "0433347774222244667777777777777777774444444444444444444477777740" "0444447774222244667777777777777777777777777777777777777777777740" "0477777774444444667777777777777777777777777777777777777777777740" "0477777774444447777777777777777777777777777777777777777777777740" "0477777777777777777777777777777777777777777777777777777777777740" "0477777777777777777777777777777777777777777777777777777777777740" "0477777777777666666666677777777777777777777777777777777777777740" "0477777777766333333333366777777777777777777777777777777777777740" "0444447777766333333333336677777777777777777777777777777777777740" "0344444777776222222222226677777777777777777777777777777777777740" "0333344777776222222222226677777777777755555555555777777777777740" "0333344777777666666666666777777777777744455555544477777777777740" "0333344777777777777777777777777777774444455555544477777777744440" "0334447777777777777777777777777777774444455555554477777777422220" "0334777777777777777777777777777777774444444444444777777774422220" "0334477777777777777777777777777777777444444477777777777744422220" "0447777777777777777777777777777777777777777777777777777444222220" "0477777777777777777777777777777777777777777777777777774442222000" "0477777777777777777777766666666677777777777777777777774442220000" "0477777777777777777777744444444444777777777777777777744442220000" "0347777777777777777777444433333344447777777777777777444442222000" "0334444777777777777777444433333344447777777777777777444444222220" "0333444777777777777777744444444444447777777777777777777444422220" "0333444777777777777777777777777777777777777777777777777774442220" "0333447777777777777777777777777777777777444447777777777774442220" "0444777777444444444444777777777777777741111144777777777777744440" "0477777743333333333333334777777777777743111114477777777777777740" "0477777743333332222333333447777777777774311111147777777777777740" "0477777774444333222223333444477777777774322222244477777777777740" "0477777777744433332222233334444777777774222222224477777777777740" "0477777777774444433332222333344447777777444444444477777777777740" "0444777777777444443333222333334444777777777777777777777777777740" "0333777777777444443333222333333444477777777777777777777777777740" "0344777777777774444433332223333344447777777777777777777777777740" "0344777777777777444444333322223334447777777777777777777777777740" "0344777777777777744444433322223344447777777777777777777777777740" "0334777777777777777744443332223344447777777777777777777777777740" "0447777777777777777777444333333344477777777777777777777777777740" "0447777777777777777777744444444477777777777777777777777777777740" "0447777777777777777777777777777777777777777777777777777777777740" "0444777777777777777777777777777777777777777777777777777777777740" "0447777777777777777722222222222222222277777777777777777777777740" "0444777777777777777722222222222222222224444447777777777777777740" "0444777777777555555551111111111111112223334444777777777777777740" "0347777777755555555551111111111111112233334444777777777777777740" "0344777775555333335551111111111111112333224444444444444777777740" "0334477775533333322222222222222222222222222444444444444444444440" "0477777555333333222222222222222222222222222222223333333334444440" "0333333332222222222000000000000000000000002222223333333333333330" "0222222222222200000000000000000000000000000000022222222222222220" "0000000000000000000000000000000000000000000000000000000000000000" "0000000000000000000000000000000000000000000000000000000000000000";
                                        // clang-format on
                                        SubInitData17 a819 = {{0x8ca0, 8, 0, 256, 1, 640, 0x8ca0, 0, 640, 0x80e8, 0, 0, 9, 0}, {0x4000, 0, 0, 0}, {0x4074, 0, 1, 0, 4, 16}, {700, 0, 0x800, 0}, {0x7d0, 0x6590, 1, 2, 0, 3, 4, 16}, {700, 0, 0x800, 0}, {700, 0x6590, 1, 3, 8, -28, 4, 0}, {700, 0, 0, 0}, {700, 0x6590, 1, 0, 16, 0, 4, 20}, {700, 0, -0x400, 0}, {0, 0x6590, 1, 3, 0, 3, 4, 27}, {700, 0, 0x600, 0}, {0, 0x6590, 1, 3, 8, -2, 10, 11}, {700, 0, 0, 0}, {0, 0x6590, 1, 0, 16, -14, 13, -14}, {700, 0, 0, 0}, {0, 0x6590, 1, 3, 25, -15, 12, -21}, {700, 0, 0, 0}, {700, 0x6590, 1, 3, 25, 12, 10, 3}, {0xbb8, 0, -0x400, 0}, {0, 0x7d0, 0, 0, 0, 25, 12, -9}, {0xbb8, 0, 0x800, 0}, {0x1000, 0x1000, 0, 0, 0, 20, 5, 27}, {100, 0, 0x800, 0}, {0x2000, 0x5dc0, 0, 0, 0, 25, 12, -9}, {0xbb8, 0, -0x400, 0}, 0x1000, 0x3e80, 0, 0, 0};
                                        SubInitData20 a817 = {{0, 10, 0}, {320, 0x6590, 1, 0, -15, 4, -15}, {320, 0x6590, 2, 0, 15, 4, -20}, {320, 0x6590, 3, 0, 15, 12, -4}, {320, 0x6590, 4, 0, -15, 4, 8}, {320, 0x6590, 5, 0, 0, 12, 20}, {320, 0x6590, 6, 0, 20, 4, 14}, {320, 0x6590, 7, 0, 0, 10, 0}, {320, 0x6590, 0, 0, 0, 10, 0}, {320, 0x6590, 9, 0, 0, 6, 20}, {320, 0x6590, 10, 0, 20, 4, 14}, {320, 0x6590, 11, 0, -15, 8, 8}, {320, 0x6590, 12, 0, -15, 6, -15}, {320, 0x6590, 13, 0, 15, 8, -4}, {320, 0x6590, 14, 0, 15, 12, -20}, {320, 0x6590, 15, 0, 2, 9, 0}, {320, 0x6590, 8, 0, -14, 6, 14}, {320, 0x6590, 17, 0, 14, 9, 4}, {320, 0x6590, 18, 0, 14, 7, -14}, {320, 0x6590, 19, 0, -14, 9, -14}, {320, 0x6590, 20, 0, 14, 9, -5}, {320, 0x6590, 21, 0, -14, 6, 5}, {320, 0x6590, 22, 0, 14, 8, 14}, {320, 0x6590, 23, 0, -14, 7, 14}, {320, 0x6590, 24, 0, 14, 5, 14}, {320, 0x6590, 16, 0, -10, 8, 10}, {320, 0x6590, 26, 0, 10, 10, 14}, {320, 0x6590, 27, 0, 18, 9, 8}, {320, 0x6590, 28, 0, 0, 10, 0}, {320, 0x6590, 29, 0, 16, 8, -14}, {320, 0x6590, 30, 0, 14, 9, -15}, {320, 0x6590, 31, 0, -14, 6, 12}, {320, 0x6590, 32, 0, 0, 10, 15}, {320, 0x6590, 25, 0, 14, 5, 14}, {320, 0x6590, 25, 0, 14, 5, 14}, {320, 0x6590, 35, 0, 14, 8, 14}, {320, 0x6590, 36, 0, -14, 6, 14}, {320, 0x6590, 37, 0, -14, 5, 5}, {320, 0x6590, 38, 0, -14, 10, -14}, {320, 0x6590, 39, 0, 14, 4, -14}, {320, 0x6590, 40, 0, 14, 10, 4}, {320, 0x6590, 41, 0, 14, 10, -5}, {320, 0x6590, 42, 0, -10, 5, 10}, 320, 0x6590, 34, 0};
                                        Mem_CopyBytesFast(4097, &g_SubmarineLevel2Map, &a818);
                                        Mem_CopyBytesFast(2048, &g_SubmarineLevel2Record, &a819);
                                        Mem_CopyBytesFast(4096, &g_SubmarineLevel2RouteTable, &a817);
                                        {
                                            // Keep adjacent 64-byte tokens on one annotation line (VC5 literal limit).
                                            // clang-format off
                                            // STRING: FF7 0x00985010
                                            char a98[4097] = "0000000000000000000000000000000000000000000000000000000000000000" "0222222234444444444444444444444444444444444444444444444444444440" "0222222234555555555555666667888777777777777777777777777776666640" "0222222556776555555555566666788887777777777888888777777777666640" "0222222556776553333333344457888866666666666788886666666666666640" "0333223355676533321112333445786544444444446788876444444444466640" "0333223335676533211112233445786444433333446788876444333333446640" "0332223335677655311122334457888654433333344678887644333333444540" "0332223335677654322233344578888865444444444567888764444444444540" "0333554455666643333333444445788877666666666677888776666556667740" "0233554455665533336633444444578877666666666677888776666556667740" "0335553335665533336633444445578886544444444446788876544444444440" "0335553335666555556644444555578886442223333344678887643322233440" "0335553335566655555555555566788886442223333334678887643222233340" "0356653343556654446655556666788886544444444444678887644444444440" "0466653334566654446665566677788887666666666667788877666666666640" "0466533555666666666677777778888876666666666677888776666666666740" "0466555555888866666677777788888654444444445678887645554444445840" "0466555555888844444677788888886444444444446788876444444444458840" "0466655555888844444677778888886433333333346788876434443333346840" "0466665558888866666667777888864333333333467888764333444333468840" "0466666688888777666666677888864444444444467888764444444444588840" "0477778777777777644444477888887666666666667788877666666666678840" "0477888744444677764444447888888766666666666778887766666666667840" "0478888744444467777777777888888864444444444467888764444444445640" "0488888777744477888555588888888864334443333467888764333333334640" "0488888887744477888555588888888643344433334678887643333333444640" "0488888887744778888555588888886544444444446788876444444444467840" "0488888888877788888555888888887666666666667788877666666666678840" "0488888888888888888555888888887666666666667788877666666666678840" "0488888888888888855555555888875555555555556788876555666555568840" "0478888888888888855555555888876444554444446788877444666444468840" "0477788888888888888555888888764444554444467888764446664444468840" "0477777778888888888888888887655555555555678887655556665555678840" "0344444478888888888888888887666666666677888887766666666666778840" "0343333478888444444888888887777777777778888887777777777777778840" "0343333478888433334888888888888888888888888888888888888888888840" "0344444478888433334888888888888888888888888888888888888888888840" "0777777788888444444888888888888888888888888888888888888888888840" "0777777778888888888888888888884444444444488888888888633333688840" "0777777777788888888888888888884222422222488888888888633333688540" "0777444444478884444444888888884222422222444444448888633333666540" "0777422222478884222224888888884222422222442222248888665333336540" "0777422222478884222224888888884444444444442222248888886233226540" "0777422222478884222224888888888433333488843333348888886222226540" "0777444444478884444444888888888444444488844444448888886555556540" "0777777777778888888888888888888888888888888888888888888888888840" "0777777788888888888888888888888888888888888888888888888888888840" "0778888888888888888888888888888888888888888888888888888888888840" "0888888666666666666688888884444444448888886666666666688888888840" "0888888644444444444688888884222233348888886444444444688888888840" "0888888642222222234688888884222233348888886422333224688888888840" "0888888642222211234688888884222233348888886422333224688888888840" "0788888642222211234688888884444444448888886422333224688888887740" "0778888644444222234688888888888888888888886444444444677777777640" "0677788666664444444688888888888888888888886666666666677666666640" "0666778888866666666688888888888888888888888888888888777644444440" "0346677788888888888888888888888888888888888888888888777743333220" "0344666778888888888888888888888888888888888888888887777443333220" "0334446677788888888888888888888888888888888888888877666433332220" "0333444667777788888888888888888888888888888888887777644433322220" "0333444466666666666666666666666666666666666666666666444433322220" "0333344444444444444444444444444444444444444444444444444332222220" "0000000000000000000000000000000000000000000000000000000000000000";
                                            // clang-format on
                                            SubInitData17 a919 = {{0x8ca0, 8, 0, 400, 0, 624, 0x6400, 0, 576, 0x5ce0, 0, 0, 8, -13}, {0x4000, 0, 0, 0}, {0x3000, 1, 1, 6, 12, 30}, {0x1004, 0, 0x400, 0}, {0, 0x2000, 1, 2, 26, -3, 11, 26}, {0xc1c, 0, 512, 0}, {0, 0x2666, 1, 3, 40, -8, 13, -16}, {0x834, 0, 0x400, 0}, {0, 0x2000, 1, 0, 2, -22, 13, -27}, {0x76c, 0, 0x400, 0}, {0, 0x2000, 1, 0, 4, 0, 10, -15}, {0x960, 0, -0x400, 0}, {0, 0x2000, 1, 0, 6, 19, 12, -5}, {0xa28, 0, -0x400, 0}, {0, 0x2666, 1, 0, 8, 0, 8, 7}, {0x708, 0, -0x400, 0}, {0, 0x3000, 1, 0, 12, 30, 13, 6}, {0x76c, 0, 0, 0}, {0, 0x1800, 1, 0, 21, 20, 13, -26}, {0x801, 0, -512, 0}, {0, 0x2000, 1, 0, 0, 29, 12, 23}, {0x801, 0, -0x400, 0}, {0, 0x2000, 1, 0, 41, -20, 10, 9}, {0x801, 0, 0, 0}, {0x1000, 0x2000, 1, 3, 45, 0, 9, 19}, {0xc01, 0, 0x400, 0}, 0x1000, 0x3000, 1, 0, 16};
                                            SubInitData21 a99 = {{-6, 15, -23}, {128, 0x2666, 1, 0, -6, 12, -17}, {128, 0x1800, 2, 0, 22, 15, -15}, {128, 0x2000, 3, 0, 22, 12, -25}, {128, 0x1800, 0, 0, 7, 14, -26}, {128, 0x2666, 5, 0, 4, 12, -15}, {128, 0x2666, 6, 0, -25, 9, -16}, {128, 0x2000, 7, 0, -26, 13, -25}, {128, 0x2666, 4, 0, -4, 14, -5}, {128, 0x3000, 9, 0, -4, 8, -14}, {128, 0x2666, 10, 0, 18, 9, -14}, {128, 0x2666, 11, 0, 13, 15, -5}, {128, 0x1800, 8, 0, -16, 8, 7}, {192, 0x2666, 13, 0, 16, 8, -21}, {192, 0x3000, 14, 0, -16, 8, -21}, {192, 0x2666, 15, 0, 16, 8, 7}, {192, 0x3000, 12, 0, 16, 9, 19}, {192, 0x3000, 17, 0, -16, 9, -9}, {192, 0x36b0, 18, 0, 16, 9, -9}, {192, 0x3000, 19, 0, -16, 9, 19}, {192, 0x36b0, 16, 0, 30, 13, 9}, {128, 0x1800, 21, 0, 18, 14, 9}, {128, 0x1800, 22, 0, 11, 13, 9}, {128, 0x36b0, 23, 0, -3, 13, 9}, {128, 0x1800, 24, 0, -3, 13, 3}, {128, 0x2000, 25, 0, 29, 13, 3}, {128, 0x1800, 20, 0, 11, 15, 29}, {128, 0x3000, 27, 0, 18, 14, 22}, {128, 0x3000, 28, 0, 16, 15, 17}, {128, 0x2666, 29, 0, 12, 16, 12}, {128, 0x2666, 30, 0, 15, 15, 8}, {128, 0x2666, 31, 0, 14, 16, 5}, {128, 0x2666, 32, 0, 13, 14, 0}, {128, 0x2666, 33, 0, 8, 13, -4}, {128, 0x3000, 34, 0, -5, 12, -5}, {128, 0x36b0, 35, 0, -7, 10, 0}, {128, 0x3000, 36, 0, -3, 12, 11}, {128, 0x2666, 37, 0, -5, 13, 16}, {128, 0x3e80, 38, 0, -3, 12, 22}, {128, 0x3e80, 39, 0, -3, 11, 26}, {128, 0x3000, 40, 0, 6, 13, 28}, {128, 0x3000, 26, 0, 12, 11, 21}, {128, 0x2000, 42, 0, 0, 10, 22}, {128, 0x2666, 43, 0, 0, 10, 16}, {128, 0x2000, 44, 0, 15, 11, 17}, {128, 0x2666, 41, 0, -19, 12, 13}, {128, 0x2000, 46, 0, -22, 12, 21}, {128, 0x1800, 47, 0, -17, 13, 23}, {128, 0x999, 48, 0, -20, 14, 25}, {128, 0x999, 49, 0, -22, 14, 26}, {128, 0x999, 50, 0, -21, 15, 29}, {128, 0x999, 51, 0, -18, 12, 29}, {128, 0x999, 52, 0, -4, 12, 29}, {128, 0x1000, 53, 0, -1, 15, 19}, {128, 0x2000, 54, 0, -7, 15, -2}, {128, 0x2666, 55, 0, -17, 15, -2}, {128, 0x2000, 56, 0, -16, 14, 7}, 128, 0x2666, 45, 0};
                                            Mem_CopyBytesFast(4097, &g_SubmarineLevel1Map, &a98);
                                            Mem_CopyBytesFast(2048, &g_SubmarineLevel1Record, &a919);
                                            Mem_CopyBytesFast(4096, &g_SubmarineLevel1RouteTable, &a99);
                                            {
                                                // Keep adjacent 64-byte tokens on one annotation line (VC5 literal limit).
                                                // clang-format off
                                                // STRING: FF7 0x00986014
                                                char a102[4097] = "0000000000000000000000000000000000000000000000000000000000000000" "0444444444444444444444444444444444444444444444444444444444444440" "0477777777777777777777777777777777777777777777777777777777777740" "0477777777777777777777777777777777777777777777777777777777777740" "0477777777777777777666666666666666666666677777775557777777777740" "0477777666666777777644444444444444444444677777555555577777777740" "0477777644446777777642244442222444422224677775533333557777777740" "0477777644446777777642244442222444422224677775533333555555777740" "0477777666666677777642244444444444444444677775533333333335577740" "0477777766666667777642246666666666666666677775555333333335577740" "0477777776444467777642246777777777777777777777555333333335777740" "0477777776444467777666666777777777777777777777775555555555777740" "0477777776666667777666666777777777666666666666666667775557777740" "0477777766666667777666666777777777664444446644444466666777777740" "0477777764444677777642246777777777644222244442222444446666777740" "0477777764444677777642246777777777644222244442222444444446666640" "0477766666666677777642246777777777664444446644444444444446666640" "0477766666666677777644446777777777666666666666664444444666777740" "0477777764444677777666666777777777777777777777764222246677777740" "0477777764444677777777777777777777777777777777764222246777777740" "0477777766666666667777777777777777777777777777764444446777777740" "0477777766666666667777777777777777777777777777764444446777777740" "0477777764444677777777777777777777777777777777764222246777777740" "0477777664444677777777777777777766677777777777764222246677777740" "0477777644446677777777777777666666666667777777764444444677777740" "0477777644446777777777777666666666666666667777766644444666777740" "0477777666666777777777766666666666666666666677777666644446677740" "0477777777777777777776666666664444444666666666777776444444677740" "0477777777777777777766666666444000004446666666677776422224677740" "0477777777777777777666666664400000000046666666667776422224677740" "0477777777777777777666666644000000000044666666667776444444677740" "0477777777777777776666666640000000000004666666666776644446677740" "0477777744447777776666666640000000000004666666666777666666777740" "0477777744447777776666666640000000000004666666666777666666777740" "0477777744447777777666666640000000000004666666667777644446777740" "0477777744447777777666666644000000000044666666667777644446777740" "0477777755557777777766666664400000000046666666677777644446777740" "0477777755557777777776666666444000004446666666777777644446777740" "0477777777777777777777766666664444444666666677777777666666777740" "0477777777777777777777777666666666666666667777777777666667777740" "0477777777777777777777777777666666666667777777777776666667777740" "0477776666666677777777777777777766677777777777777776663333777740" "0477776444444677777777777777777777777777777777777766633333777740" "0477776444444677777777777777777777777777777777774333333333777740" "0477776422224666666666777777777777777777777777774333333337777740" "0477776420024444444446777777777777777777777777777777777777777740" "0477776420022222222246777777777777777777777777777777777777777740" "0477776420000002200246777777777777777777777777777777777777777740" "0477776420000002200246777777777777777777777777777777777777777740" "0477776422222222200246777777777777777777777777777777777777777740" "0477776444444444200246666667766666666776666666677666666667776640" "0477776666666664200244444466664444446666444444666644444466666440" "0477777777777764222222222444444222244444422224444442222444444440" "0477777777777764222222222444444222244444422224444442222444444440" "0477777777777764200244444466664444446666444444666644444466666440" "0477776666666664200246666667766666666776666666677666666667776640" "0477776544444444200246777777777777777777777777777777777777777740" "0477776544444444222246777777777777777777777777777777777777777740" "0477776554444444444446777777777777777777777777777777777777777740" "0477776666666666666666777777777777777777777777777777777777777740" "0477777777777777777777777777777777777777777777777777777777777740" "0477777777777777777777777777777777777777777777777777777777777740" "0444444444444444444444444444444444444444444444444444444444444440" "0000000000000000000000000000000000000000000000000000000000000000";
                                                // clang-format on
                                                SubInitData17 a103 = {{0x8ca0, 8, 0, 300, 1, 0x400, 0x8800, 0, 0x400, 0x8800, 0, 0, 8, -12}, {0x4000, 0, 0, 0}, {0x4400, 0, 1, -28, 12, 0}, {0x1388, 0, 0x800, 0}, {0x2000, 0x4000, 1, 2, 12, -10, 14, 8}, {0x834, 0, 0x400, 0}, {0x2000, 0x5000, 1, 0, 0, 12, 14, 10}, {0x834, 0, 0x800, 0}, {0x2000, 0x5000, 1, 0, 1, 10, 12, -8}, {0x834, 0, -0x400, 0}, {0x3000, 0x5000, 1, 0, 3, -14, 14, -10}, {0x834, 0, 0, 0}, {0x3000, 0x5000, 1, 0, 4, 15, 8, -12}, {0x901, 0, 0, 0}, {0x3000, 0x6000, 1, 0, 8, -15, 8, 18}, {0x901, 0, 0x800, 0}, {0x3000, 0x6000, 1, 0, 11, -18, 8, -8}, {0x901, 0, 0x400, 0}, {0x3000, 0x6000, 1, 0, 9, -28, 12, 8}, {0xd48, 0, 0x800, 0}, {0x2000, 0x4000, 1, 3, 12, -28, 14, 30}, {0xd48, 0, 0x800, 0}, {0, 0x1800, 1, 3, 39, -20, 10, 10}, {0x1000, 0, 0, 0}, {0x1000, 0x2000, 0, 0, 45, 0, 9, 20}, {0x1000, 0, 0x400, 0}, 0x1000, 0x3000, 0, 0, 16};
                                                SubInitData21 a101 = {{0, 14, 10}, {128, 0x6000, 1, 0, 14, 14, 3}, {128, 0x5000, 2, 0, 14, 14, -3}, {128, 0x5000, 3, 0, 0, 14, -10}, {128, 0x6000, 4, 0, -14, 14, -3}, {128, 0x5000, 5, 0, -14, 14, 3}, {128, 0x5000, 0, 0, 0, 10, 9}, {128, 0x6000, 11, 0, 11, 8, 4}, {128, 0x5000, 6, 0, 11, 8, -4}, {128, 0x6000, 7, 0, 0, 10, -9}, {128, 0x6000, 8, 0, -11, 8, -4}, {128, 0x5000, 9, 0, -11, 8, 4}, {128, 0x6000, 10, 0, -28, 15, -29}, {128, 0x2000, 13, 0, -6, 15, -28}, {128, 0x2000, 14, 0, -6, 15, -26}, {64, 0x1800, 15, 0, 24, 15, -25}, {128, 0x2000, 16, 0, 27, 11, -23}, {128, 0x2666, 17, 0, 27, 11, -20}, {128, 0x1800, 18, 0, 27, 13, -17}, {128, 0x2000, 19, 0, 24, 14, -17}, {128, 0x2000, 20, 0, 0, 15, -17}, {128, 0x6000, 21, 0, -10, 15, -10}, {128, 0x5000, 22, 0, -17, 15, -9}, {128, 0x36b0, 23, 0, -19, 15, -7}, {128, 0x4000, 24, 0, -28, 15, -8}, {128, 0x3000, 25, 0, -28, 15, -20}, {128, 0x2000, 26, 0, -17, 9, -21}, {128, 0x1800, 27, 0, -10, 10, -22}, {128, 0x2666, 28, 0, -9, 11, -24}, {128, 0x2666, 29, 0, 0, 12, -24}, {128, 0x36b0, 30, 0, 25, 12, -24}, {128, 0x4000, 31, 0, 27, 11, -22}, {128, 0x1800, 32, 0, 26, 11, -19}, {128, 0x1800, 33, 0, 22, 13, -17}, {128, 0x2000, 34, 0, 0, 14, -17}, {128, 0x6000, 35, 0, -9, 15, -11}, {128, 0x4000, 36, 0, -17, 15, -9}, {128, 0x36b0, 37, 0, -19, 15, -7}, {128, 0x3000, 38, 0, -28, 15, -8}, {128, 0x2666, 12, 0, -26, 14, 28}, {128, 0x2000, 40, 0, -25, 14, 24}, {128, 0x1800, 41, 0, -24, 14, 21}, {128, 0x3000, 42, 0, -24, 13, 19}, {128, 0x2000, 43, 0, -25, 13, 15}, {128, 0x2666, 44, 0, -25, 14, 13}, {128, 0x1800, 45, 0, -26, 14, 10}, {128, 0x3000, 46, 0, -26, 14, 7}, {128, 0x1800, 47, 0, -25, 15, -7}, {128, 0x4000, 48, 0, -18, 15, -7}, {128, 0x3000, 49, 0, -14, 12, 10}, {128, 0x6000, 50, 0, -2, 11, 18}, {128, 0x6000, 51, 0, 5, 12, 21}, {128, 0x5000, 52, 0, 10, 13, 21}, {128, 0x3000, 53, 0, 10, 15, 28}, {128, 0x3000, 54, 0, 7, 12, 29}, {128, 0x2666, 55, 0, -13, 15, 28}, {128, 0x2000, 56, 0, -21, 15, 28}, 128, 0x3000, 39, 0};
                                                Mem_CopyBytesFast(4097, &g_SubmarineLevel0Map, &a102);
                                                Mem_CopyBytesFast(2048, &g_SubmarineLevel0Record, &a103);
                                                Mem_CopyBytesFast(4096, &g_SubmarineLevel0RouteTable, &a101);
                                                {
                                                    SubInitData22 a112 = {-32, -16, -12, -9, -6, -4, -3, -2, -1, 0, 1, 2, 3, 4, 6, 9, 12, 16, 32};
                                                    SubInitData23 a1119 = {-64, -60, -56, -52, -48, -44, -40, -36, -32, -28, -24, -20, -16, -12, -8, -4, 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64};
                                                    SubInitData23 a1126 = {-128, -120, -112, -104, -96, -88, -80, -72, -64, -56, -48, -40, -32, -24, -16, -8, 0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128};
                                                    SubInitData23 a113 = {-32, -30, -28, -26, -24, -22, -20, -18, -16, -14, -12, -10, -8, -6, -4, -2, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
                                                    SubInitData23 a111 = {-128, -120, -112, -104, -96, -88, -80, -72, -64, -56, -48, -40, -32, -24, -16, -8, 0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128};
                                                    SubInitData25 a1118 = {{-341, 0, 0}, {256, 128, 1, 0, 16, 4, 512, 512, 16}, {-170, 0, 0}, {128, 144, 1, 0, 16, 4, 512, 512, 16}, {-910, 0, 0}, {128, 80, 0, 1, 16, 4, 512, 512, 16}, {-113, 0, 0}, {0, 144, 0, 0, 64, 48, 512, 512, 16}, {-967, 0, 0}, {0x800, 128, 0, 1, 16, 4, 0x1000, 512, 16}};
                                                    SubInitData26 a110 = {{0, 11, -28}, {0x4000, 0, 0, 0}, {0x1000, 0, 0}, {0, 0x1000, 0}, {0, 0, 0x1000}, {0, 0, 0}, {0, 0, 0}, {-341, 0, 0}, {0, 0, 0}, {0, 0, 0}, {-682, 0, 0}, {0, 0, 0}, {0, 0, 256, 0, 0, 0}, {0, 0, 0}, 0, 1, 0, 0, 9, 0, 1, 0, 0, 0, 0};
                                                    Mem_CopyBytesFast(76, &(*(unsigned char *)g_SubmarineState.speedScaleTable), &a112);
                                                    Mem_CopyBytesFast(66, g_SubmarineState.lookupTableY, &a1119);
                                                    Mem_CopyBytesFast(66, g_SubmarineState.lookupTableX, &a1126);
                                                    Mem_CopyBytesFast(66, g_SubmarineState.lookupTableZ, &a113);
                                                    Mem_CopyBytesFast(66, g_SubmarineState.batchCountTable, &a111);
                                                    Mem_CopyBytesFast(280, &g_SubmarineState.tableEntries[0], &a1118);
                                                    Mem_CopyBytesFast(188, &g_SubmarineState.player, &a110);
                                                    {
                                                        SubInitData27 a129 = {0x7fc0c0c0, 0x7f606060, 0x7f606060, 0x7f202020, 0x7f808000, 0x7f404000, 0x7f404000, 0x7f202000, 0x7f0000ff, 0x7f000080, 0x7f000080, 0x7f000020, 0x7fff0000, 0x7f800000, 0x7f800000, 0x7f200000, 0x7f00a000, 0x7f005000, 0x7f005000, 0x7f002800, 0x7fc0c0c0, 0x7f606000, 0x7f404000, 0x7f202000, 0x7f101000, 0x7f000000};
                                                        g_SubmarineActiveTorpedoCount = 0;
                                                        g_SubmarineTorpedoLaunchOffsetFactor = 16;
                                                        g_SubmarineTorpedoForwardOffsetFactor = 4;
                                                        g_SubmarineTorpedoInitialSpeed = 16;
                                                        Mem_CopyBytesFast(80, &g_SubmarineConnectorColor0, &a129);
                                                        Mem_CopyBytesFast(24, &g_SubmarineConnectorColor1, &a129);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Samples elapsed timestamp ticks against g_SubmarineFrameTimestamp and busy-waits until g_SubFrameInterval when
// the elapsed value is at most that interval, then sets the normal-state flag and clears timing
// excess. For an overlong frame records elapsed minus the interval, clears the normal-state flag,
// and adds one to ctx->framesThisSecond. When already outside normal state, permits that
// excess path once via g_SubmarineExcessFlag; a second consecutive excess resets both flags and clears
// the accumulated excess (g_SubmarineAccumFrameTime).
// FUNCTION: FF7 0x0078c9e1
void __cdecl Submarine_FrameRateLimit(struct GfxEngineContext *ctx)
{
    double elapsed;
    Timestamp64 diff64;

    elapsed = 0.0;
    ReadTimestampCounter(&g_SubmarinePrevTimestamp);
    Sub64(&g_SubmarinePrevTimestamp, &g_SubmarineFrameTimestamp, &diff64);
    elapsed = CombineSplitValueToFloat(&diff64);
    if (g_SubmarineFrameOnTime != 0) {
        if (elapsed <= g_SubFrameInterval) {
            while (elapsed < g_SubFrameInterval) {
                ReadTimestampCounter(&g_SubmarinePrevTimestamp);
                Sub64(&g_SubmarinePrevTimestamp, &g_SubmarineFrameTimestamp, &diff64);
                elapsed = CombineSplitValueToFloat(&diff64);
            }
            g_SubmarineFrameOnTime = 1;
            g_SubmarineAccumFrameTime.words[0] = 0;
            g_SubmarineAccumFrameTime.words[1] = 0;
        } else {
            g_SubmarineAccumFrameTime.ticks = elapsed - g_SubFrameInterval;
            g_SubmarineFrameOnTime = 0;
            ctx->framesThisSecond += 1.0;
        }
        g_SubmarineExcessFlag = 0;
    } else {
        if (elapsed <= g_SubFrameInterval) {
            while (elapsed < g_SubFrameInterval) {
                ReadTimestampCounter(&g_SubmarinePrevTimestamp);
                Sub64(&g_SubmarinePrevTimestamp, &g_SubmarineFrameTimestamp, &diff64);
                elapsed = CombineSplitValueToFloat(&diff64);
            }
            g_SubmarineFrameOnTime = 1;
            g_SubmarineAccumFrameTime.words[0] = 0;
            g_SubmarineAccumFrameTime.words[1] = 0;
        } else {
            if (g_SubmarineExcessFlag == 0) {
                g_SubmarineAccumFrameTime.ticks = elapsed - g_SubFrameInterval;
                g_SubmarineFrameOnTime = 0;
                g_SubmarineExcessFlag = 1;
                ctx->framesThisSecond += 1.0;
            } else {
                g_SubmarineFrameOnTime = 1;
                g_SubmarineExcessFlag = 0;
                g_SubmarineAccumFrameTime.words[0] = 0;
                g_SubmarineAccumFrameTime.words[1] = 0;
            }
        }
    }
}
} /* extern "C" */
