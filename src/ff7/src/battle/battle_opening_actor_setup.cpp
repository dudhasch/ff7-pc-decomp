// src/ff7/src/battle/battle_opening_actor_setup.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Runs the battle opening and next-round sequences, loads the stage and enemy model
//   banks, and binds party and enemy actors to their models before starting their scripts. Also
//   builds the formation table and places the party in the opening layout.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "lib/pfile.h"
#include "ff7/types/battle_actors.h"
#include "ff7/types/menu.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "lib/enums/sound.h"
#include "lib/types/polygon.h"
#include "ff7/types/condor.h"
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "ff7/api/field.h"
#include "ff7/api/menu.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/movie.h"
#include "lib/api/token.h"
#include "lib/api/mem.h"
#include "lib/api/sound.h"
#include "lib/api/polygon.h"
#include <string.h>
#include <stdio.h>
#include "ff7/savemap.h"
#include "ff7/enums/main.h"
#include "ff7/enums/battle.h"
extern "C" {

// clang-format off
int g_BattleCommandCategoryNamePtrs[8] = {
    (int)(g_BattleMagicTypePrefix + 0x0), (int)(g_BattleAssetAndCategoryStrings + 0x8), (int)(g_BattleAssetAndCategoryStrings + 0x14), (int)(g_BattleAssetAndCategoryStrings + 0x20), (int)(g_BattleAssetAndCategoryStrings + 0x2c), (int)(g_BattleAssetAndCategoryStrings + 0x38),
    0xd040302, 0x2014
};
// GLOBAL: FF7 0x007c21e8
char g_BattleOverrideStepMode = 2;
// GLOBAL: FF7 0x007c21ec
int g_Battle3d_StageDrawEnabled = 1;
// GLOBAL: FF7 0x007c21f0
struct OpeningPositions162 g_BattleOpeningPartyPositions_Default = {{-1600, 0, 1700, 0, 0, 1700, 1600, 0, 1700, -1600, 0, 1700, 0, 0, 1700, 1600, 0, 1700, -1600, 0, -1700, 0, 0, -1700, 1600, 0, -1700, -1000, 0, 2500, 0, 0, -2500, 1000, 0, 2500, -1600, 0, -200, 0, 0, 200, 1600, 0, -200, -1000, 0, 3500, 0, 0, -3500, 1000, 0, 3500, -1000, 0, 0x1194, 0, 0, -0x1194, 1000, 0, 0x1194, -1000, 0, -2500, 0, 0, 2500, 1000, 0, -2500, -1600, 0, 1700, 0, 0, 1700, 1600, 0, 1700}};
// clang-format off
// GLOBAL: FF7 0x007c2298
struct OpeningPositions162 g_BattleOpeningPartyPositions = {{-1600, 0, 1700, 0, 0, 1700, 1600, 0, 1700, -1600, 0, 1700, 0, 0, 1700, 1600, 0, 1700, -1600, 0, -1700, 0, 0, -1700, 1600, 0, -1700, -1000, 0, 2500, 0, 0, -2500, 1000, 0, 2500, -1600, 0, -200, 0, 0, 200, 1600, 0, -200, -1000, 0, 3500, 0, 0, -3500, 1000, 0, 3500, -1000, 0, 0x1194, 0, 0, -0x1194, 1000, 0, 0x1194, -1000, 0, -2500, 0, 0, 2500, 1000, 0, -2500, -1600, 0, 1700, 0, 0, 1700, 1600, 0, 1700}};
// clang-format on
// GLOBAL: FF7 0x007c2340
struct OpeningPositions108 g_BattleOpeningEnemyPositions_Default = {{-1000, 0, 1700, 1000, 0, 1700, -1000, 0, 1700, 1000, 0, 1700, -1000, 0, -1700, 1000, 0, -1700, 0, 0, 2500, 0, 0, -2500, -1000, 0, -200, 1000, 0, 200, 0, 0, 3500, 0, 0, -3500, 0, 0, 0x1194, 0, 0, -0x1194, 0, 0, 2500, 0, 0, -2500, -1000, 0, 1700, 1000, 0, 1700}};
// clang-format off
// GLOBAL: FF7 0x007c23b0
struct OpeningPositions108 g_BattleOpeningEnemyPositions = {{-1000, 0, 1700, 1000, 0, 1700, -1000, 0, 1700, 1000, 0, 1700, -1000, 0, -1700, 1000, 0, -1700, 0, 0, 2500, 0, 0, -2500, -1000, 0, -200, 1000, 0, 200, 0, 0, 3500, 0, 0, -3500, 0, 0, 0x1194, 0, 0, -0x1194, 0, 0, 2500, 0, 0, -2500, -1000, 0, 1700, 1000, 0, 1700}};
// clang-format on
// GLOBAL: FF7 0x007c2420
struct OpeningFacing54 g_BattleOpeningPartyFacing_Default = {{0, 0, 0, 0, 0, 0, 2048, 2048, 2048, 0, 2048, 0, 0, 2048, 0, 0, 2048, 0, 0, 2048, 0, 2048, 0, 2048, 0, 0, 0}};
// GLOBAL: FF7 0x007c2458
struct OpeningFacing54 g_BattleOpeningPartyFacing = {{0, 0, 0, 0, 0, 0, 2048, 2048, 2048, 0, 2048, 0, 0, 2048, 0, 0, 2048, 0, 0, 2048, 0, 2048, 0, 2048, 0, 0, 0}};
// GLOBAL: FF7 0x007c2490
struct OpeningFacing36 g_BattleOpeningEnemyFacing_Default = {{0, 0, 0, 0, 2048, 2048, 0, 2048, 0, 2048, 0, 2048, 0, 2048, 0, 2048, 0, 0}};
// GLOBAL: FF7 0x007c24b8
struct OpeningFacing36 g_BattleOpeningEnemyFacing = {{0, 0, 0, 0, 2048, 2048, 0, 2048, 0, 2048, 0, 2048, 0, 2048, 0, 2048, 0, 0}};
// clang-format off
unsigned char g_rawData_007c24e0[72] = {
0xf8, 0x45, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0xf8, 0x45, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00,
0x04, 0x46, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x0e, 0x46, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00,
0x0e, 0x46, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x0e, 0x46, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00,
0x0e, 0x46, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x0e, 0x46, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00,
0xf8, 0x45, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00};
// clang-format on
// The battle camera data files by battle scene group ("camdat0.bin" .. "camdat2.bin", the strings the raw fill
// g_BattleAssetAndCategoryStrings holds at +0x48..+0xa8), nine names and a terminating zero; Battle3d_LoadStageAndCameraData loads
// the one g_BattleLayout selects into g_BattleCameraGlobalScripts.
// GLOBAL: FF7 0x007c2528
char *g_aBattleCameraDataFiles[10] = {(char *)&g_BattleAssetAndCategoryStrings[0x48], (char *)&g_BattleAssetAndCategoryStrings[0x54], (char *)&g_BattleAssetAndCategoryStrings[0x60], (char *)&g_BattleAssetAndCategoryStrings[0x6c], (char *)&g_BattleAssetAndCategoryStrings[0x78], (char *)&g_BattleAssetAndCategoryStrings[0x84], (char *)&g_BattleAssetAndCategoryStrings[0x90], (char *)&g_BattleAssetAndCategoryStrings[0x9c], (char *)&g_BattleAssetAndCategoryStrings[0xa8], 0};
// clang-format off
unsigned char g_rawData_007c2550[360] = {
    0x8f, 0xc7, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x8b, 0xb2, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00,
    0xe7, 0xc6, 0x00, 0x00, 0x00, 0x48, 0x01, 0x00, 0x1c, 0xba, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x27, 0xb3, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00,
    0x88, 0xc8, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0xa0, 0xc8, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x51, 0xc9, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
    0x61, 0xc9, 0x00, 0x00, 0x00, 0x60, 0x01, 0x00, 0x8d, 0xc9, 0x00, 0x00, 0x00, 0x58, 0x01, 0x00, 0xb8, 0xc9, 0x00, 0x00, 0x00, 0xc8, 0x00, 0x00,
    0x5a, 0xca, 0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x80, 0xca, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x95, 0xca, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00,
    0xa7, 0xca, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0xb8, 0xca, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc8, 0xca, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00,
    0xdb, 0xca, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0xec, 0xca, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0xfd, 0xca, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
    0x0d, 0xcb, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x1d, 0xcb, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x30, 0xcb, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00,
    0x41, 0xcb, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x54, 0xcb, 0x00, 0x00, 0x00, 0xb8, 0x00, 0x00, 0x6b, 0xcb, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00,
    0x72, 0xb7, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x5e, 0xcd, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0xd1, 0xc9, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
    0x16, 0xf1, 0x01, 0x00, 0x00, 0x38, 0x01, 0x00, 0x3d, 0xf1, 0x01, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x58, 0xf1, 0x01, 0x00, 0x00, 0x10, 0x01, 0x00,
    0x7a, 0xf1, 0x01, 0x00, 0x00, 0x98, 0x00, 0x00, 0x8d, 0xf1, 0x01, 0x00, 0x00, 0xa8, 0x00, 0x00, 0xa2, 0xf1, 0x01, 0x00, 0x00, 0x20, 0x01, 0x00,
    0xc6, 0xf1, 0x01, 0x00, 0x00, 0x60, 0x00, 0x00, 0xd2, 0xf1, 0x01, 0x00, 0x00, 0xb0, 0x00, 0x00, 0xe8, 0xf1, 0x01, 0x00, 0x00, 0x38, 0x01, 0x00,
    0x0f, 0xf2, 0x01, 0x00, 0x00, 0x48, 0x01, 0x00, 0x38, 0xf2, 0x01, 0x00, 0x00, 0x58, 0x00, 0x00, 0x43, 0xf2, 0x01, 0x00, 0x00, 0x38, 0x00, 0x00,
    0x4a, 0xf2, 0x01, 0x00, 0x00, 0x48, 0x00, 0x00, 0x53, 0xf2, 0x01, 0x00, 0x00, 0x78, 0x01, 0x00, 0x82, 0xf2, 0x01, 0x00, 0x00, 0xf8, 0x00, 0x00,
    0xa1, 0xf2, 0x01, 0x00, 0x00, 0xa8, 0x00, 0x00
};
// GLOBAL: FF7 0x007c26b8
char g_szCloudDat[10] = "CLOUD.DAT";
// GLOBAL: FF7 0x007c26c8
char g_szBarrettDat[12] = "BARRETT.DAT";
// GLOBAL: FF7 0x007c26d8
char g_szTifaDat[9] = "TIFA.DAT";
// GLOBAL: FF7 0x007c26e8
char g_szEarithDat[11] = "EARITH.DAT";
// GLOBAL: FF7 0x007c26f8
char g_szRed13Dat[10] = "RED13.DAT";
// GLOBAL: FF7 0x007c2708
char g_szYufiDat[9] = "YUFI.DAT";
// GLOBAL: FF7 0x007c2718
char g_szKetcyDat[10] = "KETCY.DAT";
// GLOBAL: FF7 0x007c2728
char g_szVinsentDat[12] = "VINSENT.DAT";
// GLOBAL: FF7 0x007c2738
char g_szCid1Dat[9] = "CID1.DAT";
// GLOBAL: FF7 0x007c2748
char g_szSefirosDat[12] = "SEFIROS.DAT";
// GLOBAL: FF7 0x007c2758
char g_szBarrett2Dat[13] = "BARRETT2.DAT";
// GLOBAL: FF7 0x007c2768
char g_szBarrett3Dat[13] = "BARRETT3.DAT";
// GLOBAL: FF7 0x007c2778
char g_szBarrett4Dat[13] = "BARRETT4.DAT";
// GLOBAL: FF7 0x007c2788
char g_szVinsent2Dat[13] = "VINSENT2.DAT";
// GLOBAL: FF7 0x007c2798
char g_szVinsent3Dat[13] = "VINSENT3.DAT";
// GLOBAL: FF7 0x007c27a8
char g_szHiCloudDat[12] = "HICLOUD.DAT";
// GLOBAL: FF7 0x007c27b8
unsigned int g_pPartyModelFilenames[17] = {(unsigned int)&g_szCloudDat, (unsigned int)&g_szBarrettDat, (unsigned int)&g_szTifaDat, (unsigned int)&g_szEarithDat, (unsigned int)&g_szRed13Dat, (unsigned int)&g_szYufiDat, (unsigned int)&g_szKetcyDat, (unsigned int)&g_szVinsentDat, (unsigned int)&g_szCid1Dat, (unsigned int)&g_szCloudDat, (unsigned int)&g_szSefirosDat, (unsigned int)&g_szBarrett2Dat, (unsigned int)&g_szBarrett3Dat, (unsigned int)&g_szBarrett4Dat, (unsigned int)&g_szVinsent2Dat, (unsigned int)&g_szVinsent3Dat, (unsigned int)&g_szHiCloudDat};
unsigned char g_BattleMagicTypePrefix[4] = {0x4d, 0x41, 0x47, 0x49};
unsigned char g_BattleAssetAndCategoryStrings[180] = {
    0x43, 0x5f, 0x54, 0x59, 0x50, 0x45, 0x00, 0x00, 0x53, 0x55, 0x4d, 0x4d, 0x4f, 0x4e, 0x5f, 0x54,
    0x59, 0x50, 0x45, 0x00, 0x49, 0x54, 0x45, 0x4d, 0x5f, 0x54, 0x59, 0x50, 0x45, 0x00, 0x00, 0x00,
    0x42, 0x4c, 0x55, 0x45, 0x5f, 0x54, 0x59, 0x50, 0x45, 0x00, 0x00, 0x00, 0x4c, 0x49, 0x4d, 0x49,
    0x54, 0x5f, 0x54, 0x59, 0x50, 0x45, 0x00, 0x00, 0x4d, 0x4f, 0x4e, 0x53, 0x54, 0x45, 0x52, 0x4d,
    0x41, 0x47, 0x5f, 0x54, 0x59, 0x50, 0x45, 0x00, 0x63, 0x61, 0x6d, 0x64, 0x61, 0x74, 0x30, 0x2e,
    0x62, 0x69, 0x6e, 0x00, 0x63, 0x61, 0x6d, 0x64, 0x61, 0x74, 0x30, 0x2e, 0x62, 0x69, 0x6e, 0x00,
    0x63, 0x61, 0x6d, 0x64, 0x61, 0x74, 0x31, 0x2e, 0x62, 0x69, 0x6e, 0x00, 0x63, 0x61, 0x6d, 0x64,
    0x61, 0x74, 0x32, 0x2e, 0x62, 0x69, 0x6e, 0x00, 0x63, 0x61, 0x6d, 0x64, 0x61, 0x74, 0x32, 0x2e,
    0x62, 0x69, 0x6e, 0x00, 0x63, 0x61, 0x6d, 0x64, 0x61, 0x74, 0x32, 0x2e, 0x62, 0x69, 0x6e, 0x00,
    0x63, 0x61, 0x6d, 0x64, 0x61, 0x74, 0x32, 0x2e, 0x62, 0x69, 0x6e, 0x00, 0x63, 0x61, 0x6d, 0x64,
    0x61, 0x74, 0x32, 0x2e, 0x62, 0x69, 0x6e, 0x00, 0x63, 0x61, 0x6d, 0x64, 0x61, 0x74, 0x30, 0x2e,
    0x62, 0x69, 0x6e, 0x00};
// clang-format on
// GLOBAL: FF7 0x009adfe0
int g_Battle3d_ActiveOt = 0;
unsigned char g_bssPad_009adfe4[4] = {0};
unsigned char g_bssPad_009adfe8[256] = {0};
// GLOBAL: FF7 0x009ae0e8
int g_Battle3d_PendingDrawFlag = 0;
unsigned char g_bssPad_009ae0ec[4] = {0};
unsigned char g_bssPad_009ae0f0[4] = {0};
// GLOBAL: FF7 0x009ae0f4
int g_BattlePendingMusicTrack = 0;
// GLOBAL: FF7 0x009ae0f8
BattleNodeMatrixScratch *g_Battle3d_NodeMatrixScratch = 0;
// GLOBAL: FF7 0x009ae0fc
unsigned char g_BattleSequenceActive = 0;
unsigned char g_bssPad_009ae100[4] = {0};
// GLOBAL: FF7 0x009ae104
char g_BattleOverrideStepLevel = 0;
// GLOBAL: FF7 0x009ae108
int g_BattleOpeningSequenceState = 0;
// GLOBAL: FF7 0x009ae10c
int g_BattleNextRoundSequenceState = 0;
// GLOBAL: FF7 0x009ae110
int g_BattleNextRoundInitialTurnFlag = 0;
// GLOBAL: FF7 0x009ae114
int g_Battle3d_ShadowRenderActive = 0;
// GLOBAL: FF7 0x009ae118
int g_Battle3d_ShadowVertexCallCount = 0;
// clang-format on

#pragma intrinsic(memset)

// Advances the opening sequence through g_BattleOpeningSequenceState and the shared substate
// g_BattleSceneState. The init state copies the retained opening position/facing tables, resets flags,
// initializes battle 3D/stage/camera state, schedules the screen-band wipe and steps a frame; the next
// state steps another frame. The menu wait state waits for g_MenuEntryState to clear, calls
// Battle_InitOpeningUiState and falls through to the model/script sequence. Its substates reload enemy banks with countdown 0x3D, wait for model and
// effect readiness, load the frog bank/place enemies, start actor scripts and set enemy flags,
// then wait for three loaded actors before setting up party actors and their flags. Stepped frames
// continue until the countdown reaches zero, when it sets g_BattleRoundSequenceDone and resets the outer state.
// Unexpected substates also finish/reset; unexpected outer states do nothing.
// FUNCTION: FF7 0x00429ac0
void __cdecl Battle_RunOpeningSequence(void)
{
    int slot;

    switch (g_BattleOpeningSequenceState) {
    case BATTLE_OPENING_SEQ_INIT:
        g_BattleOpeningPartyPositions = g_BattleOpeningPartyPositions_Default;
        g_BattleOpeningEnemyPositions = g_BattleOpeningEnemyPositions_Default;
        g_BattleOpeningPartyFacing = g_BattleOpeningPartyFacing_Default;
        g_BattleOpeningEnemyFacing = g_BattleOpeningEnemyFacing_Default;
        Battle_ResetAnimDebugFlags();
        g_pBattle3dScene = (Battle3dScene *)Battle_Init3dData();
        g_BattleCurrentOrderingTable = (int)&g_BattleOrderingTableA;
        g_BattleDrawFrameParity = g_BattleOverlay_Word = 0;
        g_BattleCameraScriptValue = 0x200;
        Battle3d_LoadStageAndCameraData();
        Battle3d_ResetStageNodes();
        g_BattleDebugDumpByte0 = g_BattleDebugDumpByte1 = g_BattleDebugDumpByte2 = 0;
        Battle3d_DebugDumpSceneStub();
        Battle_ResetSequenceStateFromSetup();
        Battle_ApplySequenceCamera();
        Battle_AllocateSecondaryResourceSlot(BattleFx_ScreenBandWipeTask);
        Battle_Step3dFrame();
        g_BattleOpeningSequenceState = BATTLE_OPENING_SEQ_STEP_FRAME;
        break;
    case BATTLE_OPENING_SEQ_STEP_FRAME:
        Battle_Step3dFrame();
        g_BattleOpeningSequenceState = BATTLE_OPENING_SEQ_WAIT_MENU;
        break;
    case BATTLE_OPENING_SEQ_WAIT_MENU:
        if (g_MenuEntryState != 0) {
            return;
        }
        Battle_InitOpeningUiState();
        g_BattleOpeningSequenceState = BATTLE_OPENING_SEQ_LOAD_ACTORS;
    case BATTLE_OPENING_SEQ_LOAD_ACTORS:
        switch (g_BattleSceneState) {
        case BATTLE_SCENE_STATE_OPENING_LOAD_ENEMIES:
            g_BattleEnterFramesToWait = 61;
            Battle_ReloadAllEnemyModelBanks();
            Battle_Step3dFrame();
            g_BattleSceneState = BATTLE_SCENE_STATE_OPENING_WAIT_ENEMIES;
            break;
        case BATTLE_SCENE_STATE_OPENING_WAIT_ENEMIES:
            Battle_Step3dFrame();
            if (g_BattleEnemyTypeCount == g_BattleEnemyBankLoadStep && g_EngineIsEffectLoading == 0) {
                Battle_LoadFrogModelBank();
                Battle_PlaceEnemyActors();
                g_BattleSceneState = BATTLE_SCENE_STATE_OPENING_START_ENEMIES;
            }
            break;
        case BATTLE_SCENE_STATE_OPENING_START_ENEMIES:
            Battle_Step3dFrame();
            Battle_StartAllActorScripts();
            for (slot = 4; slot < g_BattleEnemyCount + 4; slot++) {
                g_BattleActorRecords[slot].stateFlags = g_BattleActorRecords[slot].stateFlags | 4;
            }
            g_BattleSceneState = BATTLE_SCENE_STATE_OPENING_WAIT_PARTY;
            break;
        case BATTLE_SCENE_STATE_OPENING_WAIT_PARTY:
            Battle_Step3dFrame();
            if (g_BattleEnemyBankLoadStep == 3 && g_EngineIsEffectLoading == 0) {
                Battle_SetupRoundActors();
                g_BattleActorRecords[0].stateFlags = g_BattleActorRecords[0].stateFlags | 4;
                g_BattleActorRecords[1].stateFlags = g_BattleActorRecords[1].stateFlags | 4;
                g_BattleActorRecords[2].stateFlags = g_BattleActorRecords[2].stateFlags | 4;
                g_BattleSceneState = BATTLE_SCENE_STATE_OPENING_COUNTDOWN;
            }
            break;
        case BATTLE_SCENE_STATE_OPENING_COUNTDOWN:
            Battle_Step3dFrame();
            if (g_BattleEnterFramesToWait == 0) {
                g_BattleSceneState = BATTLE_SCENE_STATE_RUNNING;
                g_BattleRoundSequenceDone = 1;
                g_BattleOpeningSequenceState = BATTLE_OPENING_SEQ_INIT;
            }
            break;
        default:
            g_BattleRoundSequenceDone = 1;
            g_BattleOpeningSequenceState = BATTLE_OPENING_SEQ_INIT;
        }
    }
}

// Advances the next-round sequence. Initial state sets enemy flags 0x40 and clears 0x80 for actors
// 4 through 9, rebuilds formation, sets the record marker to 0xFFFE, selects/applies the camera
// and clears sequence controls, then falls through to a wait state. While its wait flag is set it
// rescans six actor flag-0x20 values, steps a frame and returns even when that scan clears the
// flag. Subsequent fallthrough resets enemy poses, reloads enemy banks with countdown 0x1E, waits
// for model/effect readiness, places enemies, starts scripts and marks the loaded enemies. When
// the countdown reaches zero sets completion and resets its outer state; unexpected substates also
// finish/reset.
// FUNCTION: FF7 0x00429d8a
void __cdecl Battle_RunNextRoundSequence(void)
{
    int i;

    switch (g_BattleNextRoundSequenceState) {
    case BATTLE_NEXT_ROUND_SEQ_INIT:
        g_BattleSceneState = BATTLE_SCENE_STATE_ROUND_END;
        for (i = 4; i < 10; i++) {
            g_BattleActorRecords[i].drawFlags = (unsigned char)g_BattleActorRecords[i].drawFlags | 0x40;
            g_BattleActorRecords[i].drawFlags = (unsigned char)g_BattleActorRecords[i].drawFlags & ~0x80;
        }
        Battle_Unpause();
        Battle_BuildFormationTable();
        g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].targetIndex = 0xFFFE;
        g_BattleCameraScriptIndex = 0x54;
        Battle_ApplySequenceCamera();
        g_BattleRoundSequenceState = 0;
        g_BattleEscapedEndState = g_BattleRoundSequenceState;
        g_BattleLostEndState = g_BattleEscapedEndState;
        g_BattleNextRoundInitialTurnFlag = 1;
        g_BattleNextRoundSequenceState = BATTLE_NEXT_ROUND_SEQ_WAIT_ENEMIES_GONE;
    case BATTLE_NEXT_ROUND_SEQ_WAIT_ENEMIES_GONE:
        if (g_BattleNextRoundInitialTurnFlag != 0) {
            g_BattleNextRoundInitialTurnFlag = 1;
            if ((g_BattleActorStates[4].flags & 0x20) == 0 && (g_BattleActorStates[5].flags & 0x20) == 0 && (g_BattleActorStates[6].flags & 0x20) == 0 && (g_BattleActorStates[7].flags & 0x20) == 0 && (g_BattleActorStates[8].flags & 0x20) == 0 && (g_BattleActorStates[9].flags & 0x20) == 0) {
                g_BattleNextRoundInitialTurnFlag = 0;
            }
            Battle_Step3dFrame();
            return;
        }
        g_BattleNextRoundSequenceState = BATTLE_NEXT_ROUND_SEQ_RESET_POSES;
    case BATTLE_NEXT_ROUND_SEQ_RESET_POSES:
        for (i = 4; i < 10; i++) {
            Battle_ResetActorPose((char)i);
        }
        g_BattleSceneState = BATTLE_SCENE_STATE_NEXT_ROUND_LOAD_ENEMIES;
        g_BattleNextRoundSequenceState = BATTLE_NEXT_ROUND_SEQ_LOAD_ACTORS;
    case BATTLE_NEXT_ROUND_SEQ_LOAD_ACTORS:
        switch (g_BattleSceneState) {
        case BATTLE_SCENE_STATE_NEXT_ROUND_LOAD_ENEMIES:
            g_BattleEnterFramesToWait = 30;
            Battle_ReloadAllEnemyModelBanks();
            Battle_Step3dFrame();
            g_BattleSceneState = BATTLE_SCENE_STATE_NEXT_ROUND_WAIT_ENEMIES;
            break;
        case BATTLE_SCENE_STATE_NEXT_ROUND_WAIT_ENEMIES:
            Battle_Step3dFrame();
            if (g_BattleEnemyTypeCount == g_BattleEnemyBankLoadStep && g_EngineIsEffectLoading == 0) {
                Battle_PlaceEnemyActors();
                g_BattleSceneState = BATTLE_SCENE_STATE_NEXT_ROUND_START_ENEMIES;
            }
            break;
        case BATTLE_SCENE_STATE_NEXT_ROUND_START_ENEMIES:
            Battle_Step3dFrame();
            Battle_StartAllActorScripts();
            for (i = 4; i < g_BattleEnemyCount + 4; i++) {
                g_BattleActorRecords[i].stateFlags = g_BattleActorRecords[i].stateFlags | 4;
            }
            g_BattleSceneState = BATTLE_SCENE_STATE_NEXT_ROUND_COUNTDOWN;
            break;
        case BATTLE_SCENE_STATE_NEXT_ROUND_COUNTDOWN:
            Battle_Step3dFrame();
            if (g_BattleEnterFramesToWait == 0) {
                g_BattleSceneState = BATTLE_SCENE_STATE_RUNNING;
                g_BattleNextRoundSequenceState = BATTLE_NEXT_ROUND_SEQ_INIT;
                g_BattleRoundSequenceDone = 1;
            }
            break;
        default:
            g_BattleNextRoundSequenceState = BATTLE_NEXT_ROUND_SEQ_INIT;
            g_BattleRoundSequenceDone = 1;
        }
    }
}

// Calls Battle3d_GetStageDescriptor with g_pBattle3dScene, discards the result, then calls
// Battle_DebugDumpSceneNoOp with dimensions 0x140 by 0xF0 and the four stored global arguments.
// FUNCTION: FF7 0x0042a06a
void __cdecl Battle3d_DebugDumpSceneStub(void)
{
    BattleStageDescriptor *rec;

    rec = (BattleStageDescriptor *)Battle3d_GetStageDescriptor(g_pBattle3dScene);
    Battle_DebugDumpSceneNoOp(320, 240, g_BattleCameraScriptValue, g_BattleDebugDumpByte0, g_BattleDebugDumpByte1, g_BattleDebugDumpByte2);
}

// Sets g_BattleOpeningUiMode to 4, calls Battle_SetUiModeByte, Battle_ResetMenuScreenState, and Battle_OpenWindow(-1, -1, 0) in order,
// then sets g_MenuEntryState to 2.
// FUNCTION: FF7 0x0042a0b1
void __cdecl Battle_InitOpeningUiState(void)
{
    g_BattleOpeningUiMode = 4;
    /* Battle_SetUiModeByte takes an int; this call pushes the byte as loaded. */
    ((void(__cdecl *)(char))Battle_SetUiModeByte)(g_BattleOpeningUiMode);
    Battle_ResetMenuScreenState();
    Battle_OpenWindow(-1, -1, 0);
    g_MenuEntryState = 2;
}

// Reads g_FieldRequestedBattleId into an otherwise unused local, rebuilds the formation table,
// saves/selects the battle data directory and loads the file indexed by g_BattleLayout from
// g_aBattleCameraDataFiles into g_BattleCameraGlobalScripts. Restores the directory, then passes
// g_BattleScene.locationId to Battle_LoadStageModel with category argument zero and
// g_pBattle3dScene as output.
// FUNCTION: FF7 0x0042a0e7
void __cdecl Battle3d_LoadStageAndCameraData(void)
{
    int savedDirectory[51];
    int stageId;
    int battleId;

    battleId = (unsigned short)g_FieldRequestedBattleId;
    Battle_BuildFormationTable();
    Battle_PushDataDirectory(savedDirectory);
    LoadFileBlock((int)g_aBattleCameraDataFiles[g_BattleLayout], 0, 0, (void **)&g_BattleCameraGlobalScripts, 0);
    Battle_PopDataDirectory(savedDirectory);
    stageId = g_BattleScene.locationId;
    Battle_LoadStageModel(0, stageId, g_pBattle3dScene);
}

// Returns element bankIndex of the pointer array at g_BattleEnemyBankModelId0 minus 0x14, that is the address 20
// bytes before the stored pointer. Unchecked index.
// FUNCTION: FF7 0x0042a16b
int __cdecl Battle_GetEnemyBankSetupModelId(int bankIndex)
{
    return g_BattleEnemyBankModelIds[bankIndex] - 0x14;
}

// Initializes model/load descriptors and reloads g_BattleEnemyTypeCount enemy banks. For each bank
// clears matching model pointers for actors 4 through 9 before freeing the nonnull old bank and
// zeroing its slot; loads its replacement using Battle_GetEnemyBankSetupModelId and stores only a
// nonzero result. After all banks polls the default magic-effect thread and calls
// Battle_EnemyBankLoadStep1.
// FUNCTION: FF7 0x0042a17d
void __cdecl Battle_ReloadAllEnemyModelBanks(void)
{
    PfileLoadOptions loadOpts;
    int bankIdx;
    Battle3dPartyLoadDesc desc;
    BattleLimitAnimBank *bankPointer;
    int actorIndex;
    Battle3dPartyLoadDesc *descPtr;
    PfileLoadOptions *optionsPtr;

    descPtr = &desc;
    optionsPtr = &loadOpts;
    Battle_InitLmdLoadDesc((int *)descPtr);
    Pfile_InitLoadOptions(optionsPtr);
    for (bankIdx = 0; bankIdx < g_BattleEnemyTypeCount; bankIdx++) {
        for (actorIndex = 4; actorIndex < 10; actorIndex++) {
            if ((BattleActorModel *)(&g_BattleModelBankSlots[3])[bankIdx] == g_BattleActorRecords[actorIndex].model) {
                g_BattleActorRecords[actorIndex].model = 0;
            }
        }
        bankPointer = (BattleLimitAnimBank *)((&g_BattleModelBankSlots[3])[bankIdx]);
        if (bankPointer != 0) {
            BattleModel_FreeBank(bankPointer);
            (&g_BattleModelBankSlots[3])[bankIdx] = 0;
        }
        bankPointer = (BattleLimitAnimBank *)(BattleModel_LoadEnemy(0, Battle_GetEnemyBankSetupModelId(bankIdx), descPtr, optionsPtr));
        if (bankPointer != 0) {
            (&g_BattleModelBankSlots[3])[bankIdx] = (int)bankPointer;
        }
    }
    Battle_PollMagicEffectThreadDefault();
    Battle_EnemyBankLoadStep1();
}

// First step of a three-step chain: calls Battle_EnemyBankLoadStepNoOp(0), sets the step counter g_BattleEnemyBankLoadStep to
// 1, and when g_BattleEnemyTypeCount is greater than 1 calls Battle_PollMagicEffectThreadDefault and falls
// through to Battle_EnemyBankLoadStep2 for the next step.
// FUNCTION: FF7 0x0042a2c2
void __cdecl Battle_EnemyBankLoadStep1(void)
{
    Battle_EnemyBankLoadStepNoOp(0);
    g_BattleEnemyBankLoadStep = 1;
    if (g_BattleEnemyTypeCount > 1) {
        Battle_PollMagicEffectThreadDefault();
        Battle_EnemyBankLoadStep2();
    }
}

// Second step of the chain begun by Battle_EnemyBankLoadStep1: calls Battle_EnemyBankLoadStepNoOp(1), sets g_BattleEnemyBankLoadStep to 2,
// and when g_BattleEnemyTypeCount is greater than 2 calls Battle_PollMagicEffectThreadDefault and continues
// into Battle_EnemyBankLoadStep3.
// FUNCTION: FF7 0x0042a2ee
void __cdecl Battle_EnemyBankLoadStep2(void)
{
    Battle_EnemyBankLoadStepNoOp(1);
    g_BattleEnemyBankLoadStep = 2;
    if (g_BattleEnemyTypeCount > 2) {
        Battle_PollMagicEffectThreadDefault();
        Battle_EnemyBankLoadStep3();
    }
}

// Last step of the chain: calls Battle_EnemyBankLoadStepNoOp(2) and sets the step counter g_BattleEnemyBankLoadStep to 3. No
// further call and no guard.
// FUNCTION: FF7 0x0042a31a
void __cdecl Battle_EnemyBankLoadStep3(void)
{
    Battle_EnemyBankLoadStepNoOp(2);
    g_BattleEnemyBankLoadStep = 3;
}

// Initializes model/load descriptors and stores loadValue/auxValue into descriptor offsets
// 0x10/0x14. If party bank slot bankSlot is nonzero, frees it and clears that slot, then scans
// actor model pointers 0 through 2 comparing against the already-zeroed slot: this clears null
// pointers rather than references to the freed bank. Loads the requested party model using
// bankSlot/flags, stores only a nonzero result in the slot and returns the load result.
// FUNCTION: FF7 0x0042a330
int __cdecl Battle_LoadModelIntoBankSlot(int loadValue, int auxValue, int bankSlot, int flags)
{
    int result;
    Battle3dPartyLoadDesc descriptor;
    Battle3dPartyLoadDesc *loadDescPtr;
    PfileLoadOptions options;
    int actorIndex;
    PfileLoadOptions *optionsPtr;

    result = 0;
    loadDescPtr = &descriptor;
    optionsPtr = &options;
    Battle_InitLmdLoadDesc((int *)loadDescPtr);
    Pfile_InitLoadOptions(optionsPtr);
    loadDescPtr->loadValue = loadValue;
    loadDescPtr->auxValue = auxValue;
    if (g_BattleModelBankSlots[bankSlot] != 0) {
        BattleModel_FreeBank((BattleLimitAnimBank *)(g_BattleModelBankSlots[bankSlot]));
        g_BattleModelBankSlots[bankSlot] = 0;
        for (actorIndex = 0; actorIndex < 3; actorIndex++) {
            if ((BattleActorModel *)g_BattleModelBankSlots[bankSlot] == g_BattleActorRecords[actorIndex].model) {
                g_BattleActorRecords[actorIndex].model = 0;
            }
        }
    }
    result = (int)BattleModel_LoadPartyMember(0, bankSlot, loadDescPtr, optionsPtr, (void *)flags);
    if (result != 0) {
        g_BattleModelBankSlots[bankSlot] = result;
    }
    return result;
}

// Three-argument wrapper for Battle_LoadModelIntoBankSlot: forwards loadValue, a literal 0, bankSlot and
// flags, so it is the call site that supplies the zero second argument.
// FUNCTION: FF7 0x0042a441
void __cdecl Battle_LoadEnemyModelIntoBankSlot(int loadValue, int bankSlot, int flags)
{
    Battle_LoadModelIntoBankSlot(loadValue, 0, bankSlot, flags);
}

// Calls Battle_SortPartyModelLoadOrder, clears g_BattleModelBankSlots[6], then loads the file name "FROG.DAT"
// through Battle_LoadEnemyModelIntoBankSlot(1, 6, ...), polls the
// magic-effect loader thread and continues the load chain at Battle_PollEffectThreadThenLoadPartyModels.
// FUNCTION: FF7 0x0042a45c
void __cdecl Battle_LoadFrogModelBank(void)
{
    Battle_SortPartyModelLoadOrder();
    g_BattleModelBankSlots[6] = 0;
    // STRING: FF7 0x007c28b4
    Battle_LoadEnemyModelIntoBankSlot(1, 6, (int)("FROG.DAT"));
    Battle_PollMagicEffectThreadDefault();
    Battle_PollEffectThreadThenLoadPartyModels();
}

// Polls the magic-effect loader thread through Battle_PollMagicEffectThreadDefault, then calls
// Battle_LoadFirstTwoPartyModels.
// FUNCTION: FF7 0x0042a48b
void __cdecl Battle_PollEffectThreadThenLoadPartyModels(void)
{
    Battle_PollMagicEffectThreadDefault();
    Battle_LoadFirstTwoPartyModels();
}

// Loads the first g_BattlePartyModelLoadSlots entry's model into bank slot zero and sets the party root parent.
// When g_BattlePartyModelLoadSlots[1].loadState is not 0xC8, polls the magic thread, loads the second
// entry's model, and calls Battle_LoadThirdPartyModel; otherwise sets g_BattleEnemyBankLoadStep to three.
// FUNCTION: FF7 0x0042a49a
void __cdecl Battle_LoadFirstTwoPartyModels(void)
{
    int firstFile;
    int modelFile1;
    int firstIndex;
    int secondSlot;

    firstIndex = 0;
    firstFile = g_pPartyModelFilenames[g_BattlePartyModelLoadSlots[firstIndex].loadState];
    Battle_LoadEnemyModelIntoBankSlot(0, g_BattlePartyModelLoadSlots[firstIndex].modelId, firstFile);
    Battle_SetPartyActorRootParent(g_BattlePartyModelLoadSlots[0].modelId);
    if (g_BattlePartyModelLoadSlots[1].loadState != 0xC8) {
        Battle_PollMagicEffectThreadDefault();
        secondSlot = 1;
        modelFile1 = g_pPartyModelFilenames[g_BattlePartyModelLoadSlots[secondSlot].loadState];
        Battle_LoadEnemyModelIntoBankSlot(0, g_BattlePartyModelLoadSlots[secondSlot].modelId, modelFile1);
        Battle_LoadThirdPartyModel();
    } else {
        g_BattleEnemyBankLoadStep = 3;
    }
}

// Sets the party actor root parent from g_BattlePartyModelLoadSlots[1].modelId, then either finishes or loads one more
// model. When g_BattleOverlayRootLoadState is not 0xC8 it polls the loader thread, takes entry 2 of
// g_BattlePartyModelLoadSlots (a filename index in loadState and a bank slot in modelId), looks the index
// up in the pointer table g_pPartyModelFilenames, passes both to Battle_LoadEnemyModelIntoBankSlot and
// continues at Battle_FinishPartyModelLoadChain; otherwise it just sets the load-state global g_BattleEnemyBankLoadStep to 3.
// FUNCTION: FF7 0x0042a542
void __cdecl Battle_LoadThirdPartyModel(void)
{
    int modelFile;
    int thirdIdx;

    Battle_SetPartyActorRootParent(g_BattlePartyModelLoadSlots[1].modelId);
    if (g_BattleOverlayRootLoadState.loadState != 0xC8) {
        Battle_PollMagicEffectThreadDefault();
        thirdIdx = 2;
        modelFile = g_pPartyModelFilenames[g_BattlePartyModelLoadSlots[thirdIdx].loadState];
        Battle_LoadEnemyModelIntoBankSlot(0, g_BattlePartyModelLoadSlots[thirdIdx].modelId, modelFile);
        Battle_FinishPartyModelLoadChain();
    } else {
        g_BattleEnemyBankLoadStep = 3;
    }
}

// Tail of the model-load chain: calls Battle_SetPartyActorRootParent with g_BattleOverlayRootLoadState.rootActorId and sets
// the load-state global g_BattleEnemyBankLoadStep to 3.
// FUNCTION: FF7 0x0042a5b4
void __cdecl Battle_FinishPartyModelLoadChain(void)
{
    Battle_SetPartyActorRootParent(g_BattleOverlayRootLoadState.rootActorId);
    g_BattleEnemyBankLoadStep = 3;
}

// Calls Battle_BindEnemyActors, Battle_ResetActorRange(4, 0xA) - actors 4 through 9 - and
// Battle_StartEnemyActorScripts. No state of its own.
// FUNCTION: FF7 0x0042a5d0
void __cdecl Battle_StartAllActorScripts(void)
{
    Battle_BindEnemyActors();
    Battle_ResetActorRange(4, 0xA);
    Battle_StartEnemyActorScripts();
}

// Returns when the shared battle flag has bit two; otherwise visits actors from five up to enemy
// count plus four, repeatedly loads each selected bank pointer into the actor and starts its
// animation script when that bank exists, with repetition count increasing by four per actor.
// FUNCTION: FF7 0x0042a5eb
void __cdecl Battle_StartEnemyActorScripts(void)
{
    int actorIdx;
    int loopCount;
    int rep;
    int bankHead;
    int bankIndex;

    if ((g_BattleScene.setupFlags & 2) != 0) {
        return;
    }
    loopCount = 4;
    for (actorIdx = 5; actorIdx < g_BattleEnemyCount + 4; actorIdx++) {
        for (rep = 0; rep < loopCount; rep++) {
            bankIndex = ((struct BattleFormationEntry *)&g_BattleEnemySlots)[actorIdx - 4].slotId;
            if (g_BattleModelBankSlots[bankIndex] != 0) {
                bankHead = *(int *)g_BattleModelBankSlots[bankIndex];
                g_BattleActorRecords[actorIdx].model = (BattleActorModel *)g_BattleModelBankSlots[bankIndex];
                Battle_RunActorAnimScript((short)actorIdx, bankHead + 0x68, 0, 0);
            }
        }
        loopCount += 4;
    }
}

// Places party actors and resets ranges (0, 3) and (3, 3); if g_BattleScene.locationId equals
// 0x39, sets bit 0x10 in each of ten actor drawFlags bytes.
// FUNCTION: FF7 0x0042a6bf
void __cdecl Battle_SetupRoundActors(void)
{
    int i;

    Battle_PlacePartyActors();
    Battle_ResetActorRange(0, 3);
    Battle_ResetActorRange(3, 3);
    if (g_BattleScene.locationId == 0x39) {
        for (i = 0; i < 10; i++) {
            g_BattleActorRecords[i].drawFlags = (unsigned char)g_BattleActorRecords[i].drawFlags | 0x10;
        }
    }
}

// Resets the retained battle sequence globals, selecting actor/record zero, setting g_BattleSequenceStepReady
// and g_BattleCameraIsMoving to one and clearing the listed controls/masks. Sets completion byte one for all
// ten actor records and clears three g_BattlePartyStatusIconMasks entries. Clears actor-zero effect ID, sets
// g_BattleScriptWaitFrames to 0xE and the current battle-record short to 0xFFFE, then resets all task slots
// and clears the message window queue.
// FUNCTION: FF7 0x0042a72d
void __cdecl Battle_ResetSequenceStateFromSetup(void)
{
    /* Block-scoped extern retained from retail declaration scope (battle_window_panels.cpp has the same). */
    extern char g_BattleScreenRampActive;
    int task;

    g_BattleSceneState = BATTLE_SCENE_STATE_OPENING_LOAD_ENEMIES;
    g_BattlePsxHookCalled = 0;
    g_BattleScreenRampActive = 0;
    g_BattleSequenceStepReady = 1;
    g_BattleVariationIndex = 0;
    g_BattleCameraIsMoving = 1;
    g_BattleSequenceStepIndex = 0;
    g_BattleHitReactionInitZero = 0;
    g_BattleHitReactionCount = g_BattleHitReactionInitZero;
    g_BattleActiveActorId = 0;
    g_BattleSpecialActorId = 0;
    g_BattleTaskSlotBuffer.animRunning = 0;
    g_BattleScriptAnimRunFlag = g_BattleTaskSlotBuffer.animRunning;
    g_BattleScriptEffectFlag = 0;
    g_BattleRoundSequenceState = 0;
    g_BattleEscapedEndState = g_BattleRoundSequenceState;
    g_BattleLostEndState = g_BattleEscapedEndState;
    g_BattleLoseMusicRequested = g_BattleLostEndState;
    g_BattleApproachHoldFrames = 0;
    g_Battle3dReadyActorMask = 0;
    g_BattleStageHideFlags = 0;
    for (task = 0; task < 10; task++) {
        g_BattleActorRecords[task].isDirty = 1;
    }
    for (task = 0; task < 3; task++) {
        g_BattlePartyStatusIconMasks[task] = 0;
    }
    g_BattleActorRecords[g_BattleActiveActorId].effectId = 0;
    g_BattleScriptWaitFrames = 14;
    g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].targetIndex = 0xFFFE;
    Battle_ResetAllTaskSlots();
    Battle_ClearMessageWindowQueue();
}

// Copies the setup formation/camera bytes, clears g_BattleCurrFormationCameraIdx and the bank count, and derives
// three enemy model IDs: unused types become -1, used types add 0x14 to their setup short. Counts
// non--1 entries among six setup actors and copies their four shorts plus final int into the
// twelve-byte formation records. Adjusts non--1 actor model indices for missing first/second
// types, shifts missing model IDs out of the first two slots with bounded repeated passes, then
// adds three to every non--1 actor index. The six records are retained in place rather than
// compacted.
// FUNCTION: FF7 0x0042a869
void __cdecl Battle_BuildFormationTable(void)
{
    int i;

    g_BattleLayout = g_BattleScene.layout;
    g_BattleCameraScriptIndex = g_BattleScene.cameraScriptIndex;
    g_BattleCurrFormationCameraIdx = 0;
    g_BattleEnemyTypeCount = 0;
    for (i = 0; i < 3; i++) {
        if (Battle_IsEnemyTypeUsed(i) == -1) {
            g_BattleEnemyBankModelIds[i] = 0xFFFFFFFF;
        } else {
            g_BattleEnemyBankModelIds[i] = g_BattleScene.enemyIds[i] + 0x14;
        }
    }
    g_BattleEnemyCount = 0;
    for (i = 0; i < 6; i++) {
        if (g_BattleScene.enemies[i].enemyIndex != -1) {
            g_BattleEnemyCount++;
        }
    }
    for (i = 0; i < 6; i++) {
        ((struct BattleFormationEntry *)&g_BattleEnemySlots)[i].slotId = g_BattleScene.enemies[i].enemyIndex;
        ((struct BattleFormationEntry *)&g_BattleEnemySlots)[i].posX = g_BattleScene.enemies[i].posX;
        ((struct BattleFormationEntry *)&g_BattleEnemySlots)[i].posY = g_BattleScene.enemies[i].posY;
        ((struct BattleFormationEntry *)&g_BattleEnemySlots)[i].posZ = g_BattleScene.enemies[i].posZ;
        ((struct BattleFormationEntry *)&g_BattleEnemySlots)[i].flags = g_BattleScene.enemies[i].flags;
    }
    if (g_BattleEnemyBankModelId0 == -1) {
        for (i = 0; i < 6; i++) {
            if (((struct BattleFormationEntry *)&g_BattleEnemySlots)[i].slotId != -1) {
                ((struct BattleFormationEntry *)&g_BattleEnemySlots)[i].slotId--;
            }
        }
    }
    if (g_BattleEnemyBankModelId1 == -1) {
        for (i = 0; i < 6; i++) {
            if (((struct BattleFormationEntry *)&g_BattleEnemySlots)[i].slotId != -1 && ((struct BattleFormationEntry *)&g_BattleEnemySlots)[i].slotId != 0) {
                ((struct BattleFormationEntry *)&g_BattleEnemySlots)[i].slotId--;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        if (g_BattleEnemyBankModelId0 == -1) {
            g_BattleEnemyBankModelId0 = g_BattleEnemyBankModelId1;
            g_BattleEnemyBankModelId1 = g_BattleEnemyBankModelId2;
            g_BattleEnemyBankModelId2 = 0xFFFFFFFF;
        }
    }
    for (i = 0; i < 2; i++) {
        if (g_BattleEnemyBankModelId1 == -1) {
            g_BattleEnemyBankModelId1 = g_BattleEnemyBankModelId2;
            g_BattleEnemyBankModelId2 = 0xFFFFFFFF;
        }
    }
    for (i = 0; i < 6; i++) {
        if (((struct BattleFormationEntry *)&g_BattleEnemySlots)[i].slotId != -1) {
            ((struct BattleFormationEntry *)&g_BattleEnemySlots)[i].slotId += 3;
        }
    }
}

// Searches the six g_BattleScene.enemies[].enemyIndex words for enemyType; on the first match increments the low byte
// of g_BattleEnemyTypeCount and returns 0, otherwise returns -1.
// FUNCTION: FF7 0x0042ab4a
int __cdecl Battle_IsEnemyTypeUsed(int enemyType)
{
    int index;

    for (index = 0; index < 6; index++) {
        if (g_BattleScene.enemies[index].enemyIndex == enemyType) {
            g_BattleEnemyTypeCount++;
            return 0;
        }
    }
    return -1;
}

// Resets stage controls, clears the two retained buffers and six shorts in each of ten stage
// nodes, then sets the root parent matrix. A split-scroll stage parents four nodes and writes 0x30D4;
// a depth-scroll stage parents all ten and writes the layers' Z offsets, writing node 8's posZ
// first with 0x8AD0 then 0x63C0 (node 9 keeps 0). A roll-spin stage parents three additional nodes
// and falls through to the yaw-spin case to parent another three. Clears ten actor position triples
// and counters, resets the retained effect bytes/int and calls rain initialization when the
// battle location is 0x3A. The
// descriptor and stage reference are dereferenced without null checks.
// FUNCTION: FF7 0x0042ab91
void __cdecl Battle3d_ResetStageNodes(void)
{
    BattleStageDescriptor *descriptor;
    int i;

    ((short *)&g_BattleStageNodeState)[0] = 0;
    ((short *)&g_BattleStageNodeState)[1] = 0x10;
    memset(&g_BattleCameraTransform, 0, 52);
    memset(((unsigned char *)&g_BattleCameraLookAt), 0, 30);
    for (i = 0; i < 10; i++) {
        g_BattleStageNodes[i].rotZ = 0;
        g_BattleStageNodes[i].yaw = 0;
        g_BattleStageNodes[i].rotX = 0;
        g_BattleStageNodes[i].posZ = 0;
        g_BattleStageNodes[i].posY = 0;
        g_BattleStageNodes[i].posX = 0;
    }
    Battle_SetNodeParentMatrix(&g_BattleCameraTransform, &g_BattleStageNodes[0]);
    descriptor = (BattleStageDescriptor *)Battle3d_GetStageDescriptor(g_pBattle3dScene);
    switch (descriptor->stageType) {
    case BATTLE_STAGE_TYPE_SPLIT_SCROLL:
        Battle_SetNodeParentMatrix(&g_BattleCameraTransform, &g_BattleStageNodes[1]);
        Battle_SetNodeParentMatrix(&g_BattleCameraTransform, &g_BattleStageNodes[2]);
        Battle_SetNodeParentMatrix(&g_BattleCameraTransform, &g_BattleStageNodes[3]);
        Battle_SetNodeParentMatrix(&g_BattleCameraTransform, &g_BattleStageNodes[4]);
        g_BattleStageNodes[4].posY = 12500;
        break;
    case BATTLE_STAGE_TYPE_DEPTH_SCROLL:
        for (i = 0; i < 10; i++) {
            Battle_SetNodeParentMatrix(&g_BattleCameraTransform, &g_BattleStageNodes[i]);
        }
        g_BattleStageNodes[2].posZ = 30000;
        g_BattleStageNodes[3].posZ = 20000;
        g_BattleStageNodes[4].posZ = 10000;
        g_BattleStageNodes[5].posZ = 0;
        g_BattleStageNodes[6].posZ = 0xD8F0;
        g_BattleStageNodes[7].posZ = 0xB1E0;
        g_BattleStageNodes[8].posZ = 0x8AD0;
        g_BattleStageNodes[8].posZ = 25536;
        break;
    case BATTLE_STAGE_TYPE_ROLL_SPIN:
        Battle_SetNodeParentMatrix(&g_BattleCameraTransform, &g_BattleStageNodes[6]);
        Battle_SetNodeParentMatrix(&g_BattleCameraTransform, &g_BattleStageNodes[5]);
        Battle_SetNodeParentMatrix(&g_BattleCameraTransform, &g_BattleStageNodes[4]);
    case BATTLE_STAGE_TYPE_YAW_SPIN:
        Battle_SetNodeParentMatrix(&g_BattleCameraTransform, &g_BattleStageNodes[1]);
        Battle_SetNodeParentMatrix(&g_BattleCameraTransform, &g_BattleStageNodes[2]);
        Battle_SetNodeParentMatrix(&g_BattleCameraTransform, &g_BattleStageNodes[3]);
    }
    for (i = 0; i < 10; i++) {
        g_BattleActorRecords[i].posZ = 0;
        g_BattleActorRecords[i].posY = 0;
        g_BattleActorRecords[i].posX = 0;
        g_BattleActorStates[i].frameCounter = 0;
    }
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.startValue = 0;
    g_BattleFxTimelineState.initMask = 0x1F;
    if (g_BattleScene.locationId == 0x3A) {
        Battle3d_RainInit();
    }
}

// Assigns the three party record indices, adds each model base to 74 offsets at 0x68 and eight at
// 0x24 in the shared bank and every occupied party bank, counts occupied slots and binds all three
// actors. Applies opening placement by party count: one actor uses zero X and the three-entry Y/Z
// table; two actors use compact occupied indices in the two-entry table, adjust its stored Z
// values by 0x204 for setup flag 1 according to facing, and set yaw (zero for formation 2). Other
// counts use the three-entry position/facing path. Saves default script bytes and copies all three
// saved positions into actor position fields. Mutates model offsets and the two-party placement
// table in place, so repeated calls reapply additions.
// FUNCTION: FF7 0x0042ae73
void __cdecl Battle_PlacePartyActors(void)
{
    BattleModelFileOffsets *modelBase;
    int actorIndex;
    int offsetIndex;

    for (actorIndex = 0; actorIndex < 3; actorIndex++) {
        g_BattleActorStates[actorIndex].frogModelIndex = actorIndex;
    }
    modelBase = *(BattleModelFileOffsets **)g_BattleModelBankSlots[6];
    for (offsetIndex = 0; offsetIndex < 0x4A; offsetIndex++) {
        modelBase->animOffsets[offsetIndex] += *(int *)g_BattleModelBankSlots[6];
    }
    for (offsetIndex = 0; offsetIndex < 8; offsetIndex++) {
        modelBase->partOffsets[offsetIndex] += *(int *)g_BattleModelBankSlots[6];
    }
    g_BattlePartyPlaceStep = 0;
    for (actorIndex = 0; actorIndex < 3; actorIndex++) {
        if (g_BattleScene.actors[actorIndex].unitIndex != -1) {
            g_BattlePartyPlaceStep++;
        }
    }
    for (actorIndex = 0; actorIndex < 3; actorIndex++) {
        if (g_BattleScene.actors[actorIndex].unitIndex != -1) {
            modelBase = *(BattleModelFileOffsets **)g_BattleModelBankSlots[actorIndex];
            for (offsetIndex = 0; offsetIndex < 0x4A; offsetIndex++) {
                modelBase->animOffsets[offsetIndex] += *(int *)g_BattleModelBankSlots[actorIndex];
            }
            for (offsetIndex = 0; offsetIndex < 8; offsetIndex++) {
                modelBase->partOffsets[offsetIndex] += *(int *)g_BattleModelBankSlots[actorIndex];
            }
            /* Retail caller passes 1 arg(s) to 0-parameter `Battle_EmptyDebugLoop` (__cdecl stack cleanup matches retail). */
            ((void(__cdecl *)(char))Battle_EmptyDebugLoop)((char)actorIndex);
        }
    }
    Battle_BindPartyActorToModel(0);
    Battle_BindPartyActorToModel(1);
    Battle_BindPartyActorToModel(2);
    Battle_EmptyNodeSetupStub(0, 0);
    Battle_EmptyNodeSetupStub(1, 0x10);
    Battle_EmptyNodeSetupStub(2, 0x11);
    switch (g_BattlePartyPlaceStep) {
    case BATTLE_PARTY_PLACE_CENTERED_X:
        Battle_ShiftPartyOpeningPositionZForFlag();
        for (actorIndex = 0; actorIndex < 3; actorIndex++) {
            g_BattleActorSpawnPos[actorIndex].posX = 0;
            g_BattleActorSpawnPos[actorIndex].posY = g_BattleOpeningPartyPositions.slot[g_BattleLayout][actorIndex].posY;
            g_BattleActorSpawnPos[actorIndex].posZ = g_BattleOpeningPartyPositions.slot[g_BattleLayout][actorIndex].posZ;
        }
        Battle_SetPartyFacingFromLayout();
        break;
    case BATTLE_PARTY_PLACE_ENEMY_SIDE:
        offsetIndex = 0;
        for (actorIndex = 0; actorIndex < 3; actorIndex++) {
            if (g_BattleScene.actors[actorIndex].unitIndex != -1) {
                if ((g_BattleScene.actors[actorIndex].flags & 1) != 0) {
                    if (g_BattleOpeningEnemyFacing.facing[g_BattleLayout][offsetIndex] == 0) {
                        g_BattleOpeningEnemyPositions.slot[g_BattleLayout][offsetIndex].posZ += 516;
                    } else {
                        g_BattleOpeningEnemyPositions.slot[g_BattleLayout][offsetIndex].posZ -= 516;
                    }
                }
                offsetIndex++;
            }
        }
        offsetIndex = 0;
        for (actorIndex = 0; actorIndex < 3; actorIndex++) {
            if (g_BattleScene.actors[actorIndex].unitIndex != -1) {
                g_BattleActorSpawnPos[actorIndex].posX = g_BattleOpeningEnemyPositions.slot[g_BattleLayout][offsetIndex].posX;
                g_BattleActorSpawnPos[actorIndex].posY = g_BattleOpeningEnemyPositions.slot[g_BattleLayout][offsetIndex].posY;
                g_BattleActorSpawnPos[actorIndex].posZ = g_BattleOpeningEnemyPositions.slot[g_BattleLayout][offsetIndex].posZ;
                offsetIndex++;
            }
        }
        offsetIndex = 0;
        for (actorIndex = 0; actorIndex < 3; actorIndex++) {
            if (g_BattleScene.actors[actorIndex].unitIndex != -1) {
                g_BattleActorRecords[actorIndex].facing = g_BattleOpeningEnemyFacing.facing[g_BattleLayout][offsetIndex];
                if (g_BattleLayout == BATTLE_LAYOUT_BACK_ATTACK) {
                    g_BattleActorRecords[actorIndex].yaw = 0;
                } else {
                    g_BattleActorRecords[actorIndex].yaw = g_BattleOpeningEnemyFacing.facing[g_BattleLayout][offsetIndex];
                }
                g_BattleActorRecords[actorIndex].rotZ = 0;
                g_BattleActorRecords[actorIndex].rotX = 0;
                offsetIndex++;
            }
        }
        break;
    default:
        Battle_ShiftPartyOpeningPositionZForFlag();
        for (actorIndex = 0; actorIndex < 3; actorIndex++) {
            g_BattleActorSpawnPos[actorIndex].posX = g_BattleOpeningPartyPositions.slot[g_BattleLayout][actorIndex].posX;
            g_BattleActorSpawnPos[actorIndex].posY = g_BattleOpeningPartyPositions.slot[g_BattleLayout][actorIndex].posY;
            g_BattleActorSpawnPos[actorIndex].posZ = g_BattleOpeningPartyPositions.slot[g_BattleLayout][actorIndex].posZ;
        }
        Battle_SetPartyFacingFromLayout();
    }
    for (actorIndex = 0; actorIndex < 3; actorIndex++) {
        g_BattleActorIdleScripts[actorIndex] = g_BattleScene.actors[actorIndex].scriptId;
    }
    for (actorIndex = 0; actorIndex < 3; actorIndex++) {
        g_BattleActorRecords[actorIndex].posX = g_BattleActorSpawnPos[actorIndex].posX;
        g_BattleActorRecords[actorIndex].posY = g_BattleActorSpawnPos[actorIndex].posY;
        g_BattleActorRecords[actorIndex].posZ = g_BattleActorSpawnPos[actorIndex].posZ;
    }
}

// For each of the three entries p = 0..2, if bit 0 of g_BattleScene.actors[p].flags is set,
// steps the short at g_BattleOpeningPartyPositions + 4 + g_BattleLayout*0x12 + p*6 by 0x204, subtracting when the
// direction short at g_BattleOpeningPartyFacing + g_BattleLayout*6 + p*2 is non-zero and adding when it is zero.
// g_BattleLayout selects an 0x12-byte row of three 6-byte entries.
// FUNCTION: FF7 0x0042b4af
void __cdecl Battle_ShiftPartyOpeningPositionZForFlag(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        if ((g_BattleScene.actors[i].flags & 1) != 0) {
            if (g_BattleOpeningPartyFacing.facing[g_BattleLayout][i] == 0) {
                g_BattleOpeningPartyPositions.slot[g_BattleLayout][i].posZ += 516;
            } else {
                g_BattleOpeningPartyPositions.slot[g_BattleLayout][i].posZ -= 516;
            }
        }
    }
}

// Copies three layout-dependent g_BattleOpeningPartyFacing words to the party actors' facing and yaw,
// except yaw is zero for layout 2 and both are zero for battle ID 0x3D6; clears each actor
// g_BattleActorRecords[0].rotZ and g_BattleActorRecords[0].rotX word.
// FUNCTION: FF7 0x0042b579
void __cdecl Battle_SetPartyFacingFromLayout(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        g_BattleActorRecords[i].facing = g_BattleOpeningPartyFacing.facing[g_BattleLayout][i];
        if (g_BattleLayout == BATTLE_LAYOUT_BACK_ATTACK) {
            g_BattleActorRecords[i].yaw = 0;
        } else if ((unsigned short)g_FieldRequestedBattleId == 0x3D6) {
            g_BattleActorRecords[i].yaw = 0;
            g_BattleActorRecords[i].facing = 0;
        } else {
            g_BattleActorRecords[i].yaw = g_BattleOpeningPartyFacing.facing[g_BattleLayout][i];
        }
        g_BattleActorRecords[i].rotZ = 0;
        g_BattleActorRecords[i].rotX = 0;
    }
}

// For an occupied party actor, initializes its node chain and selects the model bank using the
// actor record byte at g_BattleActorStates[0].frogModelIndex. Copies the retained model header fields into actor and battle
// records, including four shorts at 0x44 and two six-short groups at 0x4C/0x58, clears the battle
// UV short and copies sixteen bytes from model offset 0x12. When the copied flag byte has bit 0x80
// sets the auxiliary node parent and clears its translation. Actor, bank and model pointers are
// unchecked beyond the occupied-slot test.
// FUNCTION: FF7 0x0042b66a
void __cdecl Battle_BindPartyActorToModel(char actorIndex)
{
    BattleBindPartyRecord *record;
    int counter;
    int index;

    if (g_BattleScene.actors[actorIndex].unitIndex != -1) {
        index = g_BattleActorStates[actorIndex].frogModelIndex;
        Battle_InitActorNodeChain(actorIndex, 0, 0);
        record = (BattleBindPartyRecord *)(*(int *)g_BattleModelBankSlots[index]);
        g_BattleActorRecords[actorIndex].modelFlags = record->modelFlags;
        g_BattleActorRecords[actorIndex].radius = record->radius;
        g_BattleActorRecords[actorIndex].scaleX = record->scaleX;
        g_BattleActorRecords[actorIndex].scaleY = record->scaleY;
        g_BattleActorRecords[actorIndex].modelSlot = record->modelSlot;
        g_BattleActorRecords[actorIndex].modelId = record->modelId;
        g_BattleActorStates[actorIndex].posY = record->posY;
        g_BattleActorStates[actorIndex].bindIndex = record->bindIndex;
        g_BattleActorStates[actorIndex].spriteY = record->spriteY;
        g_BattleActorStates[actorIndex].mirrorWord = 0;
        g_BattleActorStates[actorIndex].heightBias = record->heightBias;
        for (counter = 0; counter < 4; counter++) {
            (&g_BattleActorStates[actorIndex].ratioX)[counter] = record->ratios[counter];
        }
        for (counter = 0; counter < 6; counter++) {
            g_BattleActorStates[actorIndex].statusOffsetsA[counter] = record->statusOffsetsA[counter];
            g_BattleActorStates[actorIndex].statusOffsetsB[counter] = record->statusOffsetsB[counter];
        }
        if ((g_BattleActorRecords[actorIndex].modelFlags & 0x80) != 0) {
            Battle_SetNodeParentMatrix(&g_BattleActorRecords[actorIndex].root, &g_BattleActorTransformRecords[actorIndex].aux);
            Battle_ClearAuxNodeTranslation(&g_BattleActorTransformRecords[actorIndex].boneCount, 0, actorIndex);
        }
        for (index = 0; index < 16; index++) {
            (&g_BattleActorRecords[actorIndex].targetBone)[index] = record->boneTable[index];
        }
    }
}

// Clears the loaded enemy actor bank-selector bytes and adds each bank base to eight model offsets
// at 0x24 and 32 at 0x68 for bank slots 3 through g_BattleEnemyTypeCount plus 2. Binds loaded enemy actors,
// saves their default script bytes and formation position triples, and resets two rotation shorts.
// Formation 1 sets base yaw 0x800 but current yaw zero; formations 3/5/6/7 choose zero or 0x800
// from the saved Z sign, while other formations use the inverse choice. This writes the saved
// positions, without copying them into current actor position fields here. Repeated calls reapply
// pointer additions and the first bank loop is empty.
// FUNCTION: FF7 0x0042b8d2
void __cdecl Battle_PlaceEnemyActors(void)
{
    int actorIndex;
    BattleModelFileOffsets *src;
    int offsetIndex;

    for (actorIndex = 4; actorIndex < g_BattleEnemyCount + 4; actorIndex++) {
        g_BattleActorStates[actorIndex].frogModelIndex = 0;
    }
    for (actorIndex = 3; actorIndex < g_BattleEnemyTypeCount + 3; actorIndex++) {
    }
    for (actorIndex = 3; actorIndex < g_BattleEnemyTypeCount + 3; actorIndex++) {
        src = *(BattleModelFileOffsets **)g_BattleModelBankSlots[actorIndex];
        for (offsetIndex = 0; offsetIndex < 8; offsetIndex++) {
            src->partOffsets[offsetIndex] += *(int *)g_BattleModelBankSlots[actorIndex];
        }
    }
    for (actorIndex = 3; actorIndex < g_BattleEnemyTypeCount + 3; actorIndex++) {
        src = *(BattleModelFileOffsets **)g_BattleModelBankSlots[actorIndex];
        for (offsetIndex = 0; offsetIndex < 0x20; offsetIndex++) {
            src->animOffsets[offsetIndex] += *(int *)g_BattleModelBankSlots[actorIndex];
        }
    }
    for (actorIndex = 4; actorIndex < g_BattleEnemyCount + 4; actorIndex++) {
        Battle_BindEnemyActorToModel((char)actorIndex);
    }
    for (actorIndex = 4; actorIndex < g_BattleEnemyCount + 4; actorIndex++) {
        g_BattleActorIdleScripts[actorIndex] = g_BattleScene.actors[actorIndex].scriptId;
        g_BattleActorSpawnPos[actorIndex].posX = ((struct BattleFormationEntry *)&g_BattleEnemySlots)[actorIndex - 4].posX;
        g_BattleActorSpawnPos[actorIndex].posY = ((struct BattleFormationEntry *)&g_BattleEnemySlots)[actorIndex - 4].posY;
        g_BattleActorSpawnPos[actorIndex].posZ = ((struct BattleFormationEntry *)&g_BattleEnemySlots)[actorIndex - 4].posZ;
        g_BattleActorRecords[actorIndex].rotZ = 0;
        g_BattleActorRecords[actorIndex].rotX = 0;
        switch (g_BattleLayout) {
        case BATTLE_LAYOUT_PREEMPTIVE:
            g_BattleActorRecords[actorIndex].facing = 0x800;
            g_BattleActorRecords[actorIndex].yaw = 0;
            break;
        case BATTLE_LAYOUT_SIDE_ATTACK_1:
        case BATTLE_LAYOUT_SIDE_ATTACK_2:
        case BATTLE_LAYOUT_SIDE_ATTACK_3:
        case BATTLE_LAYOUT_SIDE_ATTACK_4:
            if (g_BattleActorSpawnPos[actorIndex].posZ < 0) {
                g_BattleActorRecords[actorIndex].yaw = 0;
                g_BattleActorRecords[actorIndex].facing = 0;
            } else {
                g_BattleActorRecords[actorIndex].yaw = 0x800 /* 180 deg */;
                g_BattleActorRecords[actorIndex].facing = 0x800;
            }
            break;
        default:
            if (g_BattleActorSpawnPos[actorIndex].posZ < 0) {
                g_BattleActorRecords[actorIndex].yaw = 0x800 /* 180 deg */;
                g_BattleActorRecords[actorIndex].facing = 0x800;
            } else {
                g_BattleActorRecords[actorIndex].yaw = 0;
                g_BattleActorRecords[actorIndex].facing = 0;
            }
            break;
        }
    }
}

// Selects model bank 6 when the actor selector byte equals 6, otherwise uses the actor formation
// short, and initializes the enemy node chain. Copies model header fields, clears three actor
// bytes, takes the actor model short from g_BattleEnemyModelIds and sets flag 0x40 when the first
// model short is nonzero. Copies battle parameters (including model short 0xC minus 0x384), four
// and two six-short groups, clears the UV short and writes the low six model flag bits to the HUD
// pose byte. Flag 0x80 sets the auxiliary parent and clears its translation; finally copies sixteen
// model bytes at 0x12.
// FUNCTION: FF7 0x0042bc15
void __cdecl Battle_BindEnemyActorToModel(char actorIndex)
{
    int idx;
    int loopIndex;
    BattleBindEnemyRecord *pRecord;

    if (g_BattleActorStates[actorIndex].frogModelIndex == 6) {
        idx = 6;
    } else {
        idx = ((struct BattleFormationEntry *)&g_BattleEnemySlots)[actorIndex - 4].slotId;
    }
    Battle_InitActorNodeChain(actorIndex, 0, 1);
    pRecord = (BattleBindEnemyRecord *)(*(int *)g_BattleModelBankSlots[idx]);
    g_BattleActorRecords[actorIndex].modelFlags = pRecord->modelFlags;
    g_BattleActorRecords[actorIndex].radius = pRecord->radius;
    g_BattleActorRecords[actorIndex].scaleX = pRecord->scaleX;
    g_BattleActorRecords[actorIndex].scaleY = pRecord->scaleY;
    g_BattleActorRecords[actorIndex].modelSlot = pRecord->modelSlot;
    g_BattleActorRecords[actorIndex].tintB = 0;
    g_BattleActorRecords[actorIndex].tintG = 0;
    g_BattleActorRecords[actorIndex].tintR = 0;
    g_BattleActorRecords[actorIndex].modelId = (short)g_BattleEnemyModelIds[idx];
    if (pRecord->modelId != 0) {
        g_BattleActorRecords[actorIndex].modelFlags = g_BattleActorRecords[actorIndex].modelFlags | 0x40;
    }
    g_BattleActorStates[actorIndex].posY = pRecord->posY;
    g_BattleActorStates[actorIndex].bindIndex = pRecord->bindIndex;
    g_BattleActorStates[actorIndex].spriteY = pRecord->posY - 900;
    g_BattleActorStates[actorIndex].mirrorWord = 0;
    g_BattleActorStates[actorIndex].heightBias = pRecord->heightBias;
    for (loopIndex = 0; loopIndex < 4; loopIndex++) {
        (&g_BattleActorStates[actorIndex].ratioX)[loopIndex] = pRecord->ratios[loopIndex];
    }
    for (loopIndex = 0; loopIndex < 6; loopIndex++) {
        g_BattleActorStates[actorIndex].statusOffsetsA[loopIndex] = pRecord->statusOffsetsA[loopIndex];
        g_BattleActorStates[actorIndex].statusOffsetsB[loopIndex] = pRecord->statusOffsetsB[loopIndex];
    }
    g_BattleScene.actors[actorIndex].state = g_BattleActorRecords[actorIndex].modelFlags & 0x3F;
    if ((g_BattleActorRecords[actorIndex].modelFlags & 0x80) != 0) {
        Battle_SetNodeParentMatrix(&g_BattleActorRecords[actorIndex].root, &g_BattleActorTransformRecords[actorIndex].aux);
        Battle_ClearAuxNodeTranslation(&g_BattleActorTransformRecords[actorIndex].boneCount, 0, actorIndex);
    }
    for (idx = 0; idx < 16; idx++) {
        (&g_BattleActorRecords[actorIndex].targetBone)[idx] = pRecord->boneTable[idx];
    }
}

// Matches enemy bank identifiers to actor slots four through nine and calls the empty-node setup
// with stage-dependent index offsets, writes seven fixed node words with a stage-0x4E exception,
// then parents all six enemy actors and copies their spawn-position triples.
// FUNCTION: FF7 0x0042bf13
void __cdecl Battle_BindEnemyActors(void)
{
    int enemyType;
    int actorSlot;

    for (enemyType = 0; enemyType < g_BattleEnemyTypeCount; enemyType++) {
        for (actorSlot = 4; actorSlot < 10; actorSlot++) {
            if (g_BattleActorRecords[actorSlot].modelId == g_BattleEnemyBankModelIds[enemyType]) {
                if (g_BattleScene.locationId == 0x4E) {
                    Battle_EmptyNodeSetupStub((short)actorSlot, enemyType + 0x13);
                } else {
                    Battle_EmptyNodeSetupStub((short)actorSlot, enemyType + 0x12);
                }
                break;
            }
        }
    }
    g_BattleActorRecords[3].rootYaw = 0x240;
    if (g_BattleScene.locationId == 0x4E) {
        g_BattleActorRecords[4].rootYaw = 0;
    } else {
        g_BattleActorRecords[4].rootYaw = 0x300 /* 67.5 deg */;
    }
    g_BattleActorRecords[5].rootYaw = 0x3C0;
    g_BattleActorRecords[6].rootYaw = 0x480;
    g_BattleActorRecords[7].rootYaw = 0x540;
    g_BattleActorRecords[8].rootYaw = 0x600 /* 135 deg */;
    g_BattleActorRecords[9].rootYaw = 0x6C0;
    for (enemyType = 4; enemyType < 0xA; enemyType++) {
        Battle_SetNodeParentMatrix(&g_BattleActorRecords[enemyType].root, &g_BattleActorRecords[enemyType].nodes[49]);
        g_BattleActorRecords[enemyType].posX = g_BattleActorSpawnPos[enemyType].posX;
        g_BattleActorRecords[enemyType].posY = g_BattleActorSpawnPos[enemyType].posY;
        g_BattleActorRecords[enemyType].posZ = g_BattleActorSpawnPos[enemyType].posZ;
    }
}

// Resets one battle actor's pose to its spawn state. The position is copied from the actor's entry in
// g_BattleActorSpawnPos. The scale and the three pose angles are set to 0x1000, which
// is one in 12-bit fixed point. The state flags and status are zeroed. Then bone0PosY of each
// bone node, 0x34 bytes apart, is cleared for the actor's boneCount parts.
// FUNCTION: FF7 0x0042c0a7
void __cdecl Battle_ResetActorPose(char actorIndex)
{
    int partIndex;

    g_BattleActorRecords[actorIndex].posX = g_BattleActorSpawnPos[actorIndex].posX;
    g_BattleActorRecords[actorIndex].posY = g_BattleActorSpawnPos[actorIndex].posY;
    g_BattleActorRecords[actorIndex].posZ = g_BattleActorSpawnPos[actorIndex].posZ;
    g_BattleActorRecords[actorIndex].scale = 0x1000 /* 1.0 */;
    g_BattleActorStates[actorIndex].poseAngleC = 0x1000;
    g_BattleActorStates[actorIndex].poseAngleB = 0x1000 /* 360 deg */;
    g_BattleActorStates[actorIndex].poseAngleA = 0x1000 /* 360 deg */;
    g_BattleActorStates[actorIndex].flags = 0;
    g_BattleActorStates[actorIndex].status = 0;
    for (partIndex = 0; partIndex < g_BattleActorRecords[actorIndex].boneCount; partIndex++) {
        g_BattleActorRecords[actorIndex].nodes[partIndex].posY = 0;
    }
}

// Reads g_Battle_partyStat, gets the party-stat block for partySlot, and reads its paramIndex masked with
// 0xf into a local; no result is returned or stored globally.
// FUNCTION: FF7 0x0042c1b4
void __cdecl Battle_TouchPartyStatBlock(int partySlot)
{
    int partyStat;
    int paramSlot;
    struct MenuPartyStats *pStatBlock;

    partyStat = g_Battle_partyStat;
    pStatBlock = Savemap_GetPartyStatBlock(partySlot);
    paramSlot = pStatBlock->paramIndex & 0xF;
}

// Loads g_Battle_partyVal into an unused stack local and executes an empty signed loop from four up to
// signed byte g_BattleEnemyCount plus four. Returns with no externally visible writes or calls.
// FUNCTION: FF7 0x0042c1e6
void __cdecl Battle_EnemyBankLoadStepNoOp(int step)
{
    int saved;
    int n;

    saved = g_Battle_partyVal;
    if (REG_ALLOC_HINT) {
    }
    for (n = 4; n < g_BattleEnemyCount + 4; n++) {
    }
}

// For a party slot whose save-map byte is not -1, stores yaw zero/0xC0/0x180 for actor
// zero/one/two and parents the actor node to its root matrix; the yaw local is not initialized for
// other actor values. Roots the selected actor node.
// FUNCTION: FF7 0x0042c21b
void __cdecl Battle_SetPartyActorRootParent(int actorIndex)
{
    int actorNum;
    int nodeId;
    int val;
    int yaw;

    switch (actorIndex) {
    case BATTLE_PARTY_SLOT_0:
        nodeId = 0;
        actorNum = 0;
        yaw = 0;
        break;
    case BATTLE_PARTY_SLOT_1:
        nodeId = 0x10;
        actorNum = 1;
        yaw = 0xC0;
        break;
    case BATTLE_PARTY_SLOT_2:
        nodeId = 0x11;
        actorNum = 2;
        yaw = 0x180;
    }
    if (g_BattleScene.actors[actorIndex].unitIndex != -1) {
        val = g_Battle_partyVal;
        g_BattleActorRecords[actorIndex].rootYaw = yaw;
        Battle_SetNodeParentMatrix(&g_BattleActorRecords[actorIndex].root, &g_BattleActorRecords[actorIndex].nodes[49]);
    }
}

// Sets bit 3 (0x8) in two flag bytes for actor actorIndex: g_BattleActorRecords[actorIndex].fadeFlags and the
// fadeFlags of its g_BattleActorTransformRecords entry.
// FUNCTION: FF7 0x0042c2d7
void __cdecl Battle_SetActorFadingFlags(short actorIndex)
{
    g_BattleActorRecords[actorIndex].fadeFlags |= 8;
    g_BattleActorTransformRecords[actorIndex].fadeFlags |= 8;
}

// Consumes actor pending fade bits independently. Bit 2 schedules fade-out/hide, clears that bit,
// stores actor and g_BattleScriptWaitFrames duration, clears the actor short at
// g_BattleActorRecords[0].tintPhase, and computes three byte-component steps plus 0x1000 divided by
// the duration before calling Battle_SetActorFadingFlags. Bit 4 similarly schedules fade-in, sets
// the actor short to 0x1000 and clears actor flag 4 for party actors or flagged formation enemies.
// Actor 3 calls Battle_SetActorFadingFlags in its special branch and again at the end; both pending
// bits can schedule tasks in one invocation. Duration zero and allocation/actor bounds have no
// guards.
// FUNCTION: FF7 0x0042c31c
void __cdecl Battle_ProcessActorFadeRequests(short actorId)
{
    if (g_BattleActorRecords[actorId].stateFlags & 2) {
        g_BattleActorRecords[actorId].stateFlags =
            g_BattleActorRecords[actorId].stateFlags & ~2;
        g_BattleVmReg_0 = Battle_AllocateOverlayTaskSlot(Battle_OverlayTaskFadeOutAndHide);
        g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_0].sinkCountdown = (short)g_BattleScriptWaitFrames;
        g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_0].sfxPan = actorId;
        g_BattleActorRecords[actorId].tintPhase = 0;
        g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_0].actorId =
            (unsigned char)g_BattleActorRecords[actorId].tintR / g_BattleScriptWaitFrames;
        g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_0].battleActorIndex =
            (unsigned char)g_BattleActorRecords[actorId].tintG / g_BattleScriptWaitFrames;
        g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_0].velocityX =
            (unsigned char)g_BattleActorRecords[actorId].tintB / g_BattleScriptWaitFrames;
        g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_0].velocityZ = 4096 / g_BattleScriptWaitFrames;
        Battle_SetActorFadingFlags(actorId);
    }
    if (g_BattleActorRecords[actorId].stateFlags & 4) {
        g_BattleActorRecords[actorId].stateFlags =
            g_BattleActorRecords[actorId].stateFlags & ~4;
        g_BattleVmReg_0 = Battle_AllocateOverlayTaskSlot(Battle_OverlayTaskFadeInActor);
        g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_0].sinkCountdown = (short)g_BattleScriptWaitFrames;
        g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_0].sfxPan = actorId;
        g_BattleActorRecords[actorId].tintPhase = 0x1000;
        g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_0].actorId =
            (unsigned char)g_BattleActorRecords[actorId].tintR / g_BattleScriptWaitFrames;
        g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_0].battleActorIndex =
            (unsigned char)g_BattleActorRecords[actorId].tintG / g_BattleScriptWaitFrames;
        g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_0].velocityX =
            (unsigned char)g_BattleActorRecords[actorId].tintB / g_BattleScriptWaitFrames;
        g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_0].velocityZ = 4096 / g_BattleScriptWaitFrames;
        if (actorId == 3) {
            g_BattleActorRecords[actorId].drawFlags =
                (unsigned char)g_BattleActorRecords[actorId].drawFlags & ~4;
            Battle_SetActorFadingFlags(actorId);
        }
        if (actorId < 4) {
            g_BattleActorRecords[actorId].drawFlags =
                (unsigned char)g_BattleActorRecords[actorId].drawFlags & ~4;
        } else if (((struct BattleFormationEntry *)&g_BattleEnemySlots)[actorId - 4].flags & 1) {
            g_BattleActorRecords[actorId].drawFlags =
                (unsigned char)g_BattleActorRecords[actorId].drawFlags & ~4;
        }
        Battle_SetActorFadingFlags(actorId);
    }
}

// For the current overlay actor, while countdown +4 is nonzero adds the +0xE step to
// g_BattleActorRecords[0].tintPhase, subtracts three byte steps from
// g_BattleActorRecords[0].tintR..g_BattleActorRecords[0].tintB, sets its
// g_BattleActorStates[0].hidden byte to 1, and decrements countdown. At zero clears
// g_BattleActorRecords[0].tintPhase, sets actor flag 4, retires the slot, and clears
// g_BattleActorStates[0].hidden.
// FUNCTION: FF7 0x0042c66d
void __cdecl Battle_OverlayTaskFadeOutAndHide(void)
{
    g_BattleVmReg_2 = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sfxPan;
    if (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown == 0) {
        g_BattleActorRecords[g_BattleVmReg_2].tintPhase = 0;
        g_BattleActorRecords[g_BattleVmReg_2].drawFlags = (unsigned char)g_BattleActorRecords[g_BattleVmReg_2].drawFlags | 4;
        g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
        g_BattleActorStates[g_BattleVmReg_2].hidden = 0;
    } else {
        g_BattleActorRecords[g_BattleVmReg_2].tintPhase += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityZ;
        g_BattleActorRecords[g_BattleVmReg_2].tintR -= (char)g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].actorId;
        g_BattleActorRecords[g_BattleVmReg_2].tintG -= (char)g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].battleActorIndex;
        g_BattleActorRecords[g_BattleVmReg_2].tintB -= (char)g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityX;
        g_BattleActorStates[g_BattleVmReg_2].hidden = 1;
        (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown)--;
    }
}

// For the overlay actor, while its duration is nonzero subtracts the stored fade step, adds three
// byte-channel steps, marks the secondary actor flag one and decrements duration. At zero clears
// the fade word and two flag-bit eights, retires the task, clears that secondary flag and sets
// actor flag bit 0x20.
// FUNCTION: FF7 0x0042c823
void __cdecl Battle_OverlayTaskFadeInActor(void)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    g_BattleVmReg_2 = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sfxPan;
    if (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown == 0) {
        g_BattleActorRecords[g_BattleVmReg_2].tintPhase = 0;
        g_BattleActorRecords[g_BattleVmReg_2].fadeFlags = (char)*(volatile char *)&g_BattleActorRecords[g_BattleVmReg_2].fadeFlags & (char)0xF7;
        g_BattleActorTransformRecords[g_BattleVmReg_2].fadeFlags = (int)*(volatile char *)&g_BattleActorTransformRecords[g_BattleVmReg_2].fadeFlags & 0xFFFFFFF7;
        g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
        g_BattleActorStates[g_BattleVmReg_2].hidden = 0;
        g_BattleActorRecords[g_BattleVmReg_2].stateFlags = g_BattleActorRecords[g_BattleVmReg_2].stateFlags | 0x20;
    } else {
        g_BattleActorRecords[g_BattleVmReg_2].tintPhase -= g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityZ;
        g_BattleActorRecords[g_BattleVmReg_2].tintR += (char)g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].actorId;
        g_BattleActorRecords[g_BattleVmReg_2].tintG += (char)g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].battleActorIndex;
        g_BattleActorRecords[g_BattleVmReg_2].tintB += (char)g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityX;
        g_BattleActorStates[g_BattleVmReg_2].hidden = 1;
        (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown)--;
    }
}

// Resets actor fields for the half-open first/end range, initializes three scale words to 0x1000,
// and derives enemy flag bits from g_BattleFormationEntries + 4 for actors four and above.
// FUNCTION: FF7 0x0042ca34
void __cdecl Battle_ResetActorRange(int first, int end)
{
    int actor;

    for (actor = first; actor < end; actor++) {
        g_BattleActorRecords[actor].tintPulse = 0x1F;
        g_BattleActorRecords[actor].tintPhase = 0;
        g_BattleActorRecords[actor].tintR = 0;
        g_BattleActorRecords[actor].tintG = 0;
        g_BattleActorRecords[actor].tintB = 0;
        g_BattleActorRecords[actor].drawFlags = 0x80;
        if (actor >= 4) {
            if ((((struct BattleFormationEntry *)&g_BattleEnemySlots)[actor - 4].flags & 4) == 0) {
                g_BattleActorRecords[actor].drawFlags = (unsigned char)g_BattleActorRecords[actor].drawFlags & ~0x80;
            }
            if ((((struct BattleFormationEntry *)&g_BattleEnemySlots)[actor - 4].flags & 1) == 0) {
                g_BattleActorRecords[actor].drawFlags = (unsigned char)g_BattleActorRecords[actor].drawFlags | 4;
            }
        }
        g_BattleActorRecords[actor].stateFlags = 0;
        g_BattleActorRecords[actor].stateFlags = g_BattleActorRecords[actor].stateFlags | 1;
        g_BattleActorRecords[actor].stateFlags = g_BattleActorRecords[actor].stateFlags & ~8;
        g_BattleActorRecords[actor].scriptId = 0;
        g_BattleActorRecords[actor].scale = 0x1000 /* 1.0 */;
        g_BattleActorStates[actor].poseAngleC = 0x1000;
        g_BattleActorStates[actor].poseAngleB = 0x1000 /* 360 deg */;
        g_BattleActorStates[actor].poseAngleA = 0x1000 /* 360 deg */;
        g_BattleActorRecords[actor].isDirty = 1;
        g_BattleActorStates[actor].motionState = 0;
    }
}

// Optionally runs the effect override, then interprets the current twelve-byte battle-sequence
// record. A restore-status step copies the actor's saved status and, unless that status is 0x10 or
// 0x20, its base yaw; a screen-ramp step schedules a screen colour ramp and a message step enqueues
// its message; each steps a frame and advances the record. Scale, chained-action and action steps
// initialize once through g_BattleSequenceStepReady and wait for it to be raised before advancing: a
// scale step schedules a scale-change task with an unchecked duration divisor; chained-action and
// action steps apply camera and actor effects, consume hit records until target -1, and build target
// masks. An action step selects single-target versus multi-target globals from the accumulated hit
// count, with the source kept for a hit record of animation 9. A chained action sets
// g_Battle3dPendingActorEffect to one; an action step clears it every invocation. Sentinel records
// step a frame and, when g_BattleIsRunning is nonzero, reset the record index and set g_BattleSequenceEndReached.
// Unknown types do nothing.
// FUNCTION: FF7 0x0042cbf9
void __cdecl Battle_RunSequenceStep(void)
{
    int task;

    if (g_BattleSequenceActive != 0) {
        Battle_ForceEffectOverrideStep();
    }
    if (g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actorIndex != -1) {
        switch (g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actionType) {
        case BATTLE_SEQ_STEP_RESTORE_STATUS:
            g_BattleActorRecords[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actorIndex].stateFlags = g_BattleActorRecords[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actorIndex].stateFlags | 0x20;
            g_BattleActorStates[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actorIndex].status = g_BattleScene.actors[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actorIndex].savedStatus;
            if (g_BattleActorStates[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actorIndex].status != 0x10 && g_BattleActorStates[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actorIndex].status != 0x20) {
                g_BattleActorRecords[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actorIndex].yaw = g_BattleActorRecords[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actorIndex].facing;
            }
            Battle_Step3dFrame();
            g_BattleSequenceStepIndex++;
            break;
        case BATTLE_SEQ_STEP_SCREEN_RAMP:
            Battle_AllocateSecondaryResourceSlot(Battle_EffectTaskScreenColourRamp);
            Battle_Step3dFrame();
            g_BattleSequenceStepIndex++;
            break;
        case BATTLE_SEQ_STEP_MESSAGE:
            /* Battle_EnqueueMessageWindow takes an int duration; this call pushes the byte as loaded. */
            ((void(__cdecl *)(short, char, char, short))Battle_EnqueueMessageWindow)((short)g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].power, (char)g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].animId, (char)g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].stepFlags, (unsigned char)g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].effectId);
            Battle_Step3dFrame();
            g_BattleSequenceStepIndex++;
            break;
        case BATTLE_SEQ_STEP_SCALE_ACTOR:
            if (g_BattleSequenceStepReady != 0) {
                g_BattleSequenceStepReady = 0;
                g_BattleVmReg_1 = g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].animId << 8;
                g_BattleVmReg_2 = g_BattleActorRecords[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actorIndex].scale;
                task = (short)Battle_AllocateResourceSlot(Battle3d_ScriptActorScaleRampTask);
                if (g_BattleVmReg_1 < g_BattleVmReg_2) {
                    g_BattleTaskSlotBuffer.mainSlots[task].frameCounter = (g_BattleVmReg_2 - g_BattleVmReg_1) / g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].power;
                } else {
                    g_BattleTaskSlotBuffer.mainSlots[task].frameCounter = (g_BattleVmReg_1 - g_BattleVmReg_2) / g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].power;
                }
                g_BattleActorRecords[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actorIndex].isDirty = 0;
                g_BattleTaskSlotBuffer.mainSlots[task].scaleRamp.actor = (short)*(volatile char *)&g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actorIndex;
                g_BattleTaskSlotBuffer.mainSlots[task].scaleRamp.duration = g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].power;
                g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].targetIndex = 0xFFFF;
            }
            Battle_Step3dFrame();
            if (g_BattleSequenceStepReady != 0) {
                g_BattleSequenceStepIndex++;
            }
            break;
        case BATTLE_SEQ_STEP_CHAINED_ACTION:
            if (g_BattleSequenceStepReady != 0) {
                g_BattleSequenceStepReady = 0;
                Battle_ApplySequenceCamera();
                g_BattleActiveActorId = g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].sourceActor;
                g_BattleActorRecords[g_BattleActiveActorId].isDirty = 1;
                Battle_BindSequenceEffectToActor();
                g_BattleHitDisplayCount = 0;
                while (g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].targetActor != -1) {
                    Battle3d_PushHitDisplayEntry();
                    (g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex)++;
                }
                g_Battle3dPendingActorEffect = 1;
                Battle_BuildEffectTargetMasks();
            }
            Battle_Step3dFrame();
            if (g_BattleSequenceStepReady != 0) {
                g_BattleSequenceStepIndex++;
            }
            break;
        case BATTLE_SEQ_STEP_ACTION:
            if (g_BattleSequenceStepReady != 0) {
                g_BattleSequenceStepReady = 0;
                Battle_ApplySequenceCamera();
                g_BattleActiveActorId = g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actorIndex;
                g_BattleActorRecords[g_BattleActiveActorId].stateFlags = g_BattleActorRecords[g_BattleActiveActorId].stateFlags | 1;
                g_BattleActorRecords[g_BattleActiveActorId].isDirty = 0;
                Battle_BindSequenceEffectToActor();
                g_BattleHitDisplayCount = 0;
                while (g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].targetActor != -1) {
                    g_BattleSequenceActorIndex = g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].sourceActor;
                    Battle3d_PushHitDisplayEntry();
                    if (g_BattleHitDisplayCount == 1) {
                        g_BattleSpecialActorId = g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].targetActor;
                        g_BattleSavedSpecialActorId = g_BattleSpecialActorId;
                        g_BattleCameraFocusActor = g_BattleSavedSpecialActorId;
                        if (g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].animId == 9) {
                            g_BattleSavedSpecialActorId = g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].sourceActor;
                        }
                        g_BattleSequenceStepBusy = 0;
                    } else {
                        g_BattleSpecialActorId = 0xF;
                        g_BattleCameraFocusActor = g_BattleSpecialActorId;
                        g_BattleSavedSpecialActorId = 0xF;
                        g_BattleSequenceStepBusy = 1;
                    }
                    (g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex)++;
                }
                Battle_BuildEffectTargetMasks();
            }
            g_Battle3dPendingActorEffect = 0;
            Battle_Step3dFrame();
            if (g_BattleSequenceStepReady != 0) {
                g_BattleSequenceStepIndex++;
            }
        }
    } else {
        Battle_Step3dFrame();
        if (g_BattleIsRunning != 0) {
            g_BattleSequenceStepIndex = 0;
            g_BattleSequenceEndReached = 1;
        }
    }
}

// Sets current-actor facing and yaw to 0x800 for layout/actor-side combinations (2, actor<4) or
// (1, actor>=4), copies four sequence fields into actor state and g_BattleActorStates[0].power, clears
// g_BattleHitReactionCount and g_BattleTargetFacingLocked, then clears the hit-display table.
// FUNCTION: FF7 0x0042d227
void __cdecl Battle_BindSequenceEffectToActor(void)
{
    if (g_BattleLayout == BATTLE_LAYOUT_BACK_ATTACK && g_BattleActiveActorId < 4) {
        g_BattleActorRecords[g_BattleActiveActorId].facing = 0x800;
        g_BattleActorRecords[g_BattleActiveActorId].yaw = g_BattleActorRecords[g_BattleActiveActorId].facing;
    }
    if (g_BattleLayout == BATTLE_LAYOUT_PREEMPTIVE && g_BattleActiveActorId >= 4) {
        g_BattleActorRecords[g_BattleActiveActorId].facing = 0x800;
        g_BattleActorRecords[g_BattleActiveActorId].yaw = g_BattleActorRecords[g_BattleActiveActorId].facing;
    }
    g_BattleActorRecords[g_BattleActiveActorId].scriptId = (short)*(volatile char *)&g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].animId;
    g_BattleActorRecords[g_BattleActiveActorId].effectId = g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].effectId;
    g_BattleActorRecords[g_BattleActiveActorId].effectCategory = g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].stepFlags;
    g_BattleActorStates[g_BattleActiveActorId].power = g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].power;
    g_BattleHitReactionCount = 0;
    g_BattleTargetFacingLocked = 0;
    Battle3d_ClearHitDisplayTable();
}

// Clears the hit-display table: for the 78 entries it writes 0xFF to the byte at g_BattleHitTargetActors +
// p and zeroes the 8-byte record at g_BattleHitDisplayTable + p*8 field by field (byte at +0, short at +2,
// int at +4).
// FUNCTION: FF7 0x0042d379
void __cdecl Battle3d_ClearHitDisplayTable(void)
{
    int i;

    for (i = 0; i < 78; i++) {
        g_BattleHitTargetActors[i] = 0xFF;
        g_BattleHitDisplayTable[i].battleActorIndex = 0;
        g_BattleHitDisplayTable[i].targetMask = 0;
        g_BattleHitDisplayTable[i].status = 0;
    }
}

// Copies the current sequence hit actor, flags and value into the next eight-byte display entry
// and actor mirror, writes actor and effect-coordinate fields into its twelve-byte companion
// record, substitutes -1 for the last two coordinates when the effect byte is -1, and increments
// g_BattleHitDisplayCount.
// FUNCTION: FF7 0x0042d3cb
void __cdecl Battle3d_PushHitDisplayEntry(void)
{
    g_BattleHitDisplayTable[g_BattleHitDisplayCount].battleActorIndex = g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].targetActor;
    g_BattleHitTargetActors[g_BattleHitDisplayCount] = g_BattleHitDisplayTable[g_BattleHitDisplayCount].battleActorIndex;
    g_BattleHitDisplayTable[g_BattleHitDisplayCount].targetMask = (short)g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].flags;
    g_BattleHitDisplayTable[g_BattleHitDisplayCount].status = g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].status;
    g_BattleActorRecords[g_BattleHitDisplayTable[g_BattleHitDisplayCount].battleActorIndex].state = (short)*(volatile char *)&g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].animId;
    g_BattleHitReactionRecords[g_BattleHitDisplayCount].targetParam = g_BattleDamageDisplayRecords[g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].damageDisplayIndex].damage;
    g_BattleHitReactionRecords[g_BattleHitDisplayCount].damage = g_BattleDamageDisplayRecords[g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].damageDisplayIndex].displayFlags;
    g_BattleHitReactionRecords[g_BattleHitDisplayCount].reactionParam = (short)*(volatile char *)&g_BattleEffectOrderTable[(g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex)].damageDisplayIndex;
    g_BattleHitReactionRecords[g_BattleHitDisplayCount].hitRecordIndex = g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex;
    if (g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].damageDisplayIndex == -1) {
        g_BattleHitReactionRecords[g_BattleHitDisplayCount].damageValue = 0xFFFF;
        g_BattleHitReactionRecords[g_BattleHitDisplayCount].linkIndex = 0xFFFF;
    } else {
        g_BattleHitReactionRecords[g_BattleHitDisplayCount].damageValue = g_BattleDamageDisplayRecords[g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].damageDisplayIndex].displayValue;
        g_BattleHitReactionRecords[g_BattleHitDisplayCount].linkIndex = g_BattleDamageDisplayRecords[g_BattleEffectOrderTable[g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].hitRecordIndex].damageDisplayIndex].secondaryValue;
    }
    g_BattleHitDisplayCount++;
}

// Rebuilds seven target bitmasks from the 8-byte records at g_BattleHitDisplayTable. It first zeroes
// g_BattleEffectActorMask, g_BattleEffectTargetMask, g_BattleNewTargetMask, g_BattleActiveTargetMask, g_BattleHurtMessageActorMask, g_BattleTimelineTargetMask and
// g_BattleEffectScratch through one chained assignment, then for each of the g_BattleHitDisplayCount records ors 1 <<
// (byte at +0) into g_BattleEffectScratch unconditionally and into one further mask per flag bit of the
// short at +2: 0x1 -> g_BattleTimelineTargetMask, 0x2 -> g_BattleHurtMessageActorMask, 0x4 -> g_BattleActiveTargetMask, 0x8 -> g_BattleNewTargetMask,
// 0x10 -> g_BattleEffectTargetMask, 0x20 -> g_BattleEffectActorMask.
// FUNCTION: FF7 0x0042d61b
void __cdecl Battle_BuildEffectTargetMasks(void)
{
    int i;

    g_BattleEffectActorMask = 0;
    g_BattleEffectTargetMask = g_BattleEffectActorMask;
    g_BattleNewTargetMask = g_BattleEffectTargetMask;
    g_BattleActiveTargetMask = g_BattleNewTargetMask;
    g_BattleHurtMessageActorMask = g_BattleActiveTargetMask;
    g_BattleTimelineTargetMask = g_BattleHurtMessageActorMask;
    g_BattleEffectScratch.targetMask = g_BattleTimelineTargetMask;
    for (i = 0; i < g_BattleHitDisplayCount; i++) {
        g_BattleEffectScratch.targetMask = 1 << g_BattleHitDisplayTable[i].battleActorIndex | g_BattleEffectScratch.targetMask;
        if ((g_BattleHitDisplayTable[i].targetMask & 1) != 0) {
            g_BattleTimelineTargetMask = 1 << g_BattleHitDisplayTable[i].battleActorIndex | g_BattleTimelineTargetMask;
        }
        if ((g_BattleHitDisplayTable[i].targetMask & 2) != 0) {
            g_BattleHurtMessageActorMask = 1 << g_BattleHitDisplayTable[i].battleActorIndex | g_BattleHurtMessageActorMask;
        }
        if ((g_BattleHitDisplayTable[i].targetMask & 4) != 0) {
            g_BattleActiveTargetMask = 1 << g_BattleHitDisplayTable[i].battleActorIndex | g_BattleActiveTargetMask;
        }
        if ((g_BattleHitDisplayTable[i].targetMask & 8) != 0) {
            g_BattleNewTargetMask = 1 << g_BattleHitDisplayTable[i].battleActorIndex | g_BattleNewTargetMask;
        }
        if ((g_BattleHitDisplayTable[i].targetMask & 0x10) != 0) {
            g_BattleEffectTargetMask = 1 << g_BattleHitDisplayTable[i].battleActorIndex | g_BattleEffectTargetMask;
        }
        if ((g_BattleHitDisplayTable[i].targetMask & 0x20) != 0) {
            g_BattleEffectActorMask = 1 << g_BattleHitDisplayTable[i].battleActorIndex | g_BattleEffectActorMask;
        }
    }
}

// Steps a battle 3D frame: polls the effect thread, selects the retained address constant from
// g_BattleDrawFrameParity, marks profiling section 1, steps messages and updates the scene. Recomputes
// g_BattleSequenceStepReady from all ten actor completion bytes and g_BattleSecondaryTaskActiveCount, then services turns/escape
// and marks section 2. Draws battle windows when enabled, performs the one-time Gfx_PsxHookNoOp_1 call,
// optionally draws the savemap countdown, clears g_Battle3d_PendingDrawFlag and polls again. When drawing is
// enabled swaps the ordering table; always sets geometry screen, increments the frame counter,
// marks profiling, calls Battle_LatchActorProjectedPoints and copies the retained input short.
// FUNCTION: FF7 0x0042d808
void __cdecl Battle_Step3dFrame(void)
{
    int i;

    Battle_PollMagicEffectThreadDefault();
    g_BattleOverlayDrawListHead = (g_BattleDrawFrameParity != 0 ? 0x80184000u : 0x80168000u);
    Battle3d_ProfileSectionMark(1);
    Battle_StepMessageWindowQueue();
    Battle_Update3dScene();
    for (i = 0; i < 10; i++) {
        if ((unsigned char)g_BattleActorRecords[i].isDirty == 0) {
            g_BattleSequenceStepReady = 0;
            break;
        }
        if (g_BattleSecondaryTaskActiveCount == 0) {
            g_BattleSequenceStepReady = 1;
        } else {
            g_BattleSequenceStepReady = 0;
        }
    }
    Battle_ServiceTurnQueueAndEscape();
    Battle3d_ProfileSectionMark(2);
    if (Gfx_IsDrawEnabled() != 0) {
        Menu_DrawBattleWindowSet(g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_29);
    }
    if (g_BattlePsxHookCalled == 0) {
        Gfx_PsxHookNoOp_1(1);
        g_BattlePsxHookCalled = 1;
    }
    if ((g_BattleScene.flags & 2) != 0) {
        Battle_DrawCountdownTimer(g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_29, 16, 16, gSavemap.countdownSeconds);
    }
    g_Battle3d_PendingDrawFlag = 0;
    Battle_PollMagicEffectThreadDefault();
    if (Gfx_IsDrawEnabled() != 0) {
        g_Battle3d_ActiveOt = Battle3d_SwapOrderingTable();
    }
    SetGeomScreen(g_BattleCameraScriptValue);
    g_Battle3dFrameTick++;
    Battle3d_PollEffectThread();
    Battle_LatchActorProjectedPoints();
    g_Battle3dReadyActorMask = (short)g_BtlMenuSavedMask;
}

// Despite the name, the whole body is a single call to Battle_PollMagicEffectThreadDefault; it
// touches nothing else.
// FUNCTION: FF7 0x0042d962
void __cdecl Battle3d_PollEffectThread(void)
{
    Battle_PollMagicEffectThreadDefault();
}

// Empty profiling hook; performs no work.
// FUNCTION: FF7 0x0042d96c
void __cdecl Battle3d_ProfileSectionMark(int section)
{
}

// One call: Battle3d_BuildNodeMatrixFromCamera with the node at g_BattleCameraTransform and the three camera
// globals g_BattleCameraLookAt, g_BattleCameraPosition and g_BattleCameraFocalPoint.
// FUNCTION: FF7 0x0042d971
void __cdecl Battle3d_UpdateCameraNode(void)
{
    Battle3d_BuildNodeMatrixFromCamera(&g_BattleCameraTransform, &g_BattleCameraLookAt, (Vec3sPacked *)&g_BattleCameraPosition, (Vec3sPacked *)&g_BattleCameraFocalPoint);
}

// Updates/draws the stage and decrements a nonzero sequence countdown. The opening's first three
// scene states skip actor drawing; its party-wait state conditionally runs enemy scripts/overlay
// tasks and always queues/sweeps enemies. Other scene states conditionally run both script sets, pending actor effects and overlay
// tasks, then poll, queue/sweep all actors and drain scene messages. Always polls again, copies
// the retained battle flag and checks end transitions. When g_BattleIsRunning permits runs camera
// channels and publishes camera only if drawing is enabled. Then synchronizes idle poses, draws
// HUD sprites, sets black draw colour, conditionally runs effect slots and always runs main slots.
// FUNCTION: FF7 0x0042d992
void __cdecl Battle_Update3dScene(void)
{
    Battle3d_UpdateAndDrawStage();
    if (g_BattleEnterFramesToWait != 0) {
        g_BattleEnterFramesToWait--;
    }
    switch (g_BattleSceneState) {
    case BATTLE_SCENE_STATE_OPENING_LOAD_ENEMIES:
    case BATTLE_SCENE_STATE_OPENING_WAIT_ENEMIES:
    case BATTLE_SCENE_STATE_OPENING_START_ENEMIES:
        break;
    case BATTLE_SCENE_STATE_OPENING_WAIT_PARTY:
        if (g_BattleIsRunning != 0) {
            Battle_RunEnemyActorScripts();
            Battle_RunOverlayTaskSlots();
        }
        Battle3d_QueueEnemyActorDraws();
        Battle3d_SweepActorDrawTable(0);
        break;
    default:
        if (g_BattleIsRunning != 0) {
            Battle_RunPartyActorScripts();
            Battle_RunEnemyActorScripts();
            Battle3d_RunPendingActorEffect(g_Battle3dPendingActorEffect);
            Battle_RunOverlayTaskSlots();
        }
        Battle_PollMagicEffectThreadDefault();
        Battle3d_QueueAllActorDraws();
        Battle3d_SweepActorDrawTable(0);
        Battle3d_DrainSceneMessageQueue();
    }
    Battle_PollMagicEffectThreadDefault();
    g_BattleOverlay_Short = g_BattleScene.targetMask;
    Battle3d_HandleEndOfBattleTransitions();
    if (g_BattleIsRunning != 0) {
        Battle_RunCameraChannels(g_BattleCameraTargetIndex, g_BattleActorRecords[g_BattleActiveActorId].effectId);
        if (Gfx_IsDrawEnabled() != 0) {
            Battle_PublishCameraForFrame(g_BattleCameraTargetIndex);
        }
    }
    Battle3d_SyncActorIdlePoses();
    Battle_DrawHudSprites(0);
    SetDrawColorRGB(0, 0, 0);
    if (g_BattleIsRunning != 0) {
        Battle_RunEffectTaskSlots();
    }
    Battle_RunMainTaskSlots();
}

// Runs the pending actor effect when requested, then clears the pending-effect flag.
// FUNCTION: FF7 0x0042dae5
void __cdecl Battle3d_RunPendingActorEffect(int flag)
{
    if (flag != 0) {
        Battle_RunActorEffect(g_BattleActiveActorId);
        g_Battle3dPendingActorEffect = 0;
    }
}

// Copies ten g_BattleScene.actors[0].scriptId bytes to g_BattleActorIdleScripts; for actors without
// flag 8 whose pose word differs and g_BattleActorRecords[0].isDirty is 1, sets
// g_BattleActorRecords[0].stateFlags bit 1 and copies the signed pose byte into
// g_BattleActorRecords[0].scriptId. Finally clears g_BattleActorIdleScripts[3].
// FUNCTION: FF7 0x0042db05
void __cdecl Battle3d_SyncActorIdlePoses(void)
{
    int i;

    for (i = 0; i < 10; i++) {
        g_BattleActorIdleScripts[i] = g_BattleScene.actors[i].scriptId;
        if ((g_BattleActorStates[i].flags & 8) == 0 && g_BattleActorRecords[i].scriptId != (char)g_BattleActorIdleScripts[i] && ((unsigned char)g_BattleActorRecords[i].isDirty) == 1) {
            g_BattleActorRecords[i].stateFlags = g_BattleActorRecords[i].stateFlags | 1;
            g_BattleActorRecords[i].scriptId = (short)(char)g_BattleActorIdleScripts[i];
        }
    }
    g_BattleActorIdleScripts[3] = 0;
}

// If any of the first three g_BattleActorStates[].flags has bit 1 set, queues enemies then party and
// returns; otherwise queues party then enemies.
// FUNCTION: FF7 0x0042dbd2
void __cdecl Battle3d_QueueAllActorDraws(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        if ((g_BattleActorStates[i].flags & 1) != 0) {
            Battle3d_QueueEnemyActorDraws();
            Battle3d_QueuePartyActorDraws();
            return;
        }
    }
    Battle3d_QueuePartyActorDraws();
    Battle3d_QueueEnemyActorDraws();
}

// Processes four independent end-state conditions, each guarded by its retained latch. The win
// short requests win music, enters BATTLE_SCENE_STATE_ROUND_END, calls Battle_Unpause, writes latch
// 0x64 and sequence marker 0xFFFD, applies camera and sets party status-copy flag 0x20.
// FIELD_BATTLE_FLAG_LOST similarly enters the round end and applies marker 0xFFFF/camera. The loss
// short requests lose music once. FIELD_BATTLE_FLAG_ESCAPED restores all three default party scripts
// with flags 1/0x20 and copied statuses, enters the round end and applies marker 0xFFFF/camera. Because tests are independent, later conditions may
// overwrite an earlier marker in the same invocation.
// FUNCTION: FF7 0x0042dc20
void __cdecl Battle3d_HandleEndOfBattleTransitions(void)
{
    int i;

    if (g_BattleRoundSequenceState != 0x64 && *(unsigned short *)&g_BattleMenuGraphicsPathBuffer.victoryMusicPending != 0) {
        Battle_RequestEndMusicWin();
        g_BattleSceneState = BATTLE_SCENE_STATE_ROUND_END;
        Battle_Unpause();
        g_BattleRoundSequenceState = 0x64;
        g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].targetIndex = 0xFFFD;
        Battle_ApplySequenceCamera();
        for (i = 0; i < 3; i++) {
            g_BattleActorRecords[i].stateFlags = g_BattleActorRecords[i].stateFlags | 0x20;
            g_BattleActorStates[i].status = g_BattleScene.actors[i].savedStatus;
        }
    }
    if (g_BattleLostEndState != 0x64 && (g_BattleResultWord & FIELD_BATTLE_FLAG_LOST) != 0) {
        g_BattleSceneState = BATTLE_SCENE_STATE_ROUND_END;
        Battle_Unpause();
        g_BattleLostEndState = 0x64;
        g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].targetIndex = 0xFFFF;
        Battle_ApplySequenceCamera();
    }
    if (g_BattleLoseMusicRequested == 0 && *(unsigned short *)&g_BattleDefeatMusicPending != 0) {
        Battle_RequestEndMusicLose();
        g_BattleLoseMusicRequested = 1;
    }
    if (g_BattleEscapedEndState != 0x64 && (g_BattleResultWord & FIELD_BATTLE_FLAG_ESCAPED) != 0) {
        for (i = 0; i < 3; i++) {
            g_BattleActorRecords[i].stateFlags = g_BattleActorRecords[i].stateFlags | 1;
            g_BattleActorRecords[i].scriptId = (unsigned short)*(volatile unsigned char *)&g_BattleScene.actors[i].scriptId;
            g_BattleActorRecords[i].stateFlags = g_BattleActorRecords[i].stateFlags | 0x20;
            g_BattleActorStates[i].status = g_BattleScene.actors[i].savedStatus;
        }
        g_BattleEscapedEndState = 0x64;
        g_BattleSceneState = BATTLE_SCENE_STATE_ROUND_END;
        Battle_Unpause();
        g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].targetIndex = 0xFFFF;
        Battle_ApplySequenceCamera();
    }
}

// Returns the first index 0..0x4D whose unsigned g_BattleHitTargetActors byte equals a1, or -1 if none
// matches.
// FUNCTION: FF7 0x0042de25
short __cdecl Battle_FindActorInEffectOrder(int actorId)
{
    int i;
    for (i = 0; i < 78; i++) {
        if (actorId == g_BattleHitTargetActors[i]) {
            return (short)i;
        }
    }
    return -1;
}

// Drains ring-buffer messages (enum BattleSceneMessage) until dequeue returns zero. A hit reaction
// clears g_BattleTaskSlotBuffer.isBusy, schedules the reaction, sets actor flag 0x20 and copies its
// effect-order status; a reaction-only message clears that global and schedules the reaction. A
// multi-hit reaction sets it, calls Battle_SpawnHitReactionTaskForEffectOrderEntry with actor and
// payload short, copies status/flag and increments g_BattleHitReactionCount. Starting the slot-3
// actor saves three coordinates and a short, schedules the slot-3 actor task and clears
// g_BattleSlot3ActorState; stopping it sets that global to -1. A hurt message triggers actor hurt and
// marks the actor dirty, a poll message polls the effect thread; other codes are discarded.
// FUNCTION: FF7 0x0042de61
void __cdecl Battle3d_DrainSceneMessageQueue(void)
{
    int messageType;
    int recordPtr;
    int actor;
    while ((messageType = Battle_DequeueSceneMessage(&recordPtr)) != 0) {
        switch (messageType) {
        case BATTLE_SCENE_MSG_HIT_REACTION:
        case BATTLE_SCENE_MSG_HIT_REACTION_ALT:
            g_BattleTaskSlotBuffer.isBusy = 0;
            actor = *(short *)recordPtr;
            Battle_SpawnHitReactionTaskForActor(actor);
            g_BattleActorRecords[actor].stateFlags = g_BattleActorRecords[actor].stateFlags | 0x20;
            g_BattleActorStates[actor].status = g_BattleHitDisplayTable[Battle_FindActorInEffectOrder(actor)].status;
            break;
        case BATTLE_SCENE_MSG_HIT_REACTION_ONLY:
            g_BattleTaskSlotBuffer.isBusy = 0;
            actor = *(short *)recordPtr;
            Battle_SpawnHitReactionTaskForActor(actor);
            break;
        case BATTLE_SCENE_MSG_MULTI_HIT_REACTION:
            g_BattleTaskSlotBuffer.isBusy = 1;
            actor = *(short *)recordPtr;
            Battle_SpawnHitReactionTaskForEffectOrderEntry(actor, ((Battle3dSceneMessage *)recordPtr)->effectOrder);
            g_BattleActorRecords[actor].stateFlags = g_BattleActorRecords[actor].stateFlags | 0x20;
            g_BattleActorStates[actor].status = g_BattleHitDisplayTable[Battle_FindActorInEffectOrder(actor)].status;
            g_BattleHitReactionCount++;
            break;
        case BATTLE_SCENE_MSG_START_SLOT3_ACTOR:
            g_BattleEffectTrail_ShortVector[0] = ((Battle3dSceneMessage *)recordPtr)->vectorX;
            g_BattleEffectTrail_ShortVector[1] = ((Battle3dSceneMessage *)recordPtr)->vectorY;
            g_BattleEffectTrail_ShortVector[2] = ((Battle3dSceneMessage *)recordPtr)->vectorZ;
            g_BattleEffectTrail_StateShort = ((Battle3dSceneMessage *)recordPtr)->effectOrder;
            Battle_AllocateResourceSlot(Battle3d_Slot3ActorTask);
            g_BattleSlot3ActorState = 0;
            break;
        case BATTLE_SCENE_MSG_STOP_SLOT3_ACTOR:
            g_BattleSlot3ActorState = -1;
            break;
        case BATTLE_SCENE_MSG_HURT:
            actor = *(short *)recordPtr;
            Battle_TriggerActorHurtSequence(actor);
            g_BattleActorRecords[actor].isDirty = 1;
            break;
        case BATTLE_SCENE_MSG_POLL_EFFECT_THREAD:
            Battle_PollMagicEffectThreadDefault();
        }
    }
}

// Allocates a Battle_HitReactionTask slot, clears its +4 word, stores actor at +8, finds the actor
// effect-order index, and passes that index and slot number to Battle_InitHitReactionTaskFromHitRecord.
// FUNCTION: FF7 0x0042e05a
void __cdecl Battle_SpawnHitReactionTaskForActor(int actor)
{
    int slot;
    int hitRecord;

    slot = (short)Battle_AllocateResourceSlot(Battle_HitReactionTask);
    *(short *)g_BattleTaskSlotBuffer.mainSlots[slot].data = 0;
    *(short *)&g_BattleTaskSlotBuffer.mainSlots[slot].data[4] = actor;
    hitRecord = (short)Battle_FindActorInEffectOrder(actor);
    Battle_InitHitReactionTaskFromHitRecord(hitRecord, slot);
}

// Allocates a Battle_HitReactionTask slot, clears its +4 word, stores actor at +8, initializes it
// through Battle_InitHitReactionTaskFromHitRecord(hitRecord, slot), and sets or clears actor flag 0x10 according to effect-order
// record bit 4.
// FUNCTION: FF7 0x0042e0b9
void __cdecl Battle_SpawnHitReactionTaskForEffectOrderEntry(int actor, int hitRecord)
{
    int slot;

    slot = (short)Battle_AllocateResourceSlot(Battle_HitReactionTask);
    *(short *)g_BattleTaskSlotBuffer.mainSlots[slot].data = 0;
    *(short *)&g_BattleTaskSlotBuffer.mainSlots[slot].data[4] = actor;
    Battle_InitHitReactionTaskFromHitRecord(hitRecord, slot);
    if ((g_BattleHitDisplayTable[hitRecord].targetMask & 4) != 0) {
        g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags | 0x10;
    } else {
        g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags & ~0x10;
    }
}

// Copies the selected g_BattleHitReactionRecords record's words and byte into the selected
// main task fields, sign-extending two words into dwords, and stores g_BattleActiveActorId and g_BattleHitReactionNoNumbers
// in the task.
// FUNCTION: FF7 0x0042e156
void __cdecl Battle_InitHitReactionTaskFromHitRecord(int recordIndex, int taskSlot)
{
    *(short *)&g_BattleTaskSlotBuffer.mainSlots[taskSlot].data[6] = g_BattleHitReactionRecords[recordIndex].targetParam;
    *(short *)&g_BattleTaskSlotBuffer.mainSlots[taskSlot].data[10] = g_BattleHitReactionRecords[recordIndex].damage;
    *(short *)&g_BattleTaskSlotBuffer.mainSlots[taskSlot].data[8] = (short)g_BattleActiveActorId;
    *(int *)&g_BattleTaskSlotBuffer.mainSlots[taskSlot].data[12] = (int)*(volatile short *)&g_BattleHitReactionRecords[recordIndex].damageValue;
    *(int *)&g_BattleTaskSlotBuffer.mainSlots[taskSlot].data[16] = (int)*(volatile short *)&g_BattleHitReactionRecords[recordIndex].linkIndex;
    *(short *)&g_BattleTaskSlotBuffer.mainSlots[taskSlot].data[2] = g_BattleHitReactionRecords[recordIndex].reactionParam;
    g_BattleTaskSlotBuffer.mainSlots[taskSlot].data[20] = g_BattleHitReactionNoNumbers;
    g_BattleTaskSlotBuffer.mainSlots[taskSlot].data[21] = g_BattleHitReactionRecords[recordIndex].hitKind;
}

// Sets g_BattleActorRecords[actorId].isDirty to 1 and clears bit 0x80 of its drawFlags and bit 0x20
// of g_BattleActorStates[actorId].flags.
// FUNCTION: FF7 0x0042e219
void __cdecl Battle_RestoreActorAfterEffect(char actorId)
{
    g_BattleActorRecords[actorId].isDirty = 1;
    g_BattleActorRecords[actorId].drawFlags = (unsigned char)g_BattleActorRecords[actorId].drawFlags & ~0x80;
    g_BattleActorStates[actorId].flags = g_BattleActorStates[actorId].flags & 0xFFFFFFDF;
}

// For each present party actor zero through two calls Battle_ServiceActorPendingFade, then when actor flag bit two
// is clear and its selected model bank exists stores that bank pointer and starts its actor
// animation script at bank base +0x68.
// FUNCTION: FF7 0x0042e275
void __cdecl Battle_RunPartyActorScripts(void)
{
    int scriptBase;
    int partySlot;
    int bankIndex;

    for (partySlot = 0; partySlot < 3; partySlot++) {
        if (g_BattleScene.actors[partySlot].unitIndex != -1) {
            Battle_ServiceActorPendingFade((short)partySlot);
            if (((unsigned char)g_BattleActorRecords[partySlot].drawFlags & 2) == 0) {
                bankIndex = g_BattleActorStates[partySlot].frogModelIndex;
                if (g_BattleModelBankSlots[bankIndex] != 0) {
                    scriptBase = *(int *)g_BattleModelBankSlots[bankIndex];
                    g_BattleActorRecords[partySlot].model = (BattleActorModel *)g_BattleModelBankSlots[bankIndex];
                    Battle_RunActorAnimScript((short)partySlot, scriptBase + 0x68, 0, 0);
                }
            }
        }
    }
}

// Forwards its short argument to Battle_ProcessActorFadeRequests and does nothing else.
// FUNCTION: FF7 0x0042e338
void __cdecl Battle_ServiceActorPendingFade(short actorId)
{
    Battle_ProcessActorFadeRequests((short)actorId);
}

// Selects enemy animation bank 6 when actor g_BattleActorStates[0].frogModelIndex is 6, otherwise uses
// g_BattleEnemySlots[actorId - 4]; for a nonnull bank stores it in g_BattleActorRecords[actorId].model and runs the actor
// script at the bank first pointer plus 0x68.
// FUNCTION: FF7 0x0042e34a
void __cdecl Battle_RunEnemyActorScript(int actorId, int bankIndex, int scriptPtr)
{
    if (g_BattleActorStates[actorId].frogModelIndex == 6) {
        bankIndex = 6;
    } else {
        bankIndex = ((struct BattleFormationEntry *)&g_BattleEnemySlots)[actorId - 4].slotId;
    }
    if (g_BattleModelBankSlots[bankIndex] != 0) {
        scriptPtr = *(int *)g_BattleModelBankSlots[bankIndex];
        g_BattleActorRecords[actorId].model = (BattleActorModel *)g_BattleModelBankSlots[bankIndex];
        Battle_RunActorAnimScript((short)actorId, scriptPtr + 0x68, 0, 0);
    }
}

// Walks enemy actors 4 through g_BattleEnemyCount+3, calling Battle_ServiceActorPendingFade and
// running each one's script when its drawFlags bits 0x40 and 2 are clear; a second pass runs the
// script again for those whose stateFlags bit 0x1 and g_BattleActorStates[].flags bit 0x4 are both
// set.
// FUNCTION: FF7 0x0042e3ca
void __cdecl Battle_RunEnemyActorScripts(void)
{
    int actorIdx;
    int scriptArgA;
    int unusedB;

    for (actorIdx = 4; actorIdx < g_BattleEnemyCount + 4; actorIdx++) {
        Battle_ServiceActorPendingFade((short)actorIdx);
        if (((unsigned char)g_BattleActorRecords[actorIdx].drawFlags & 0x40) == 0 && ((unsigned char)g_BattleActorRecords[actorIdx].drawFlags & 2) == 0) {
            Battle_RunEnemyActorScript(actorIdx, scriptArgA, unusedB);
        }
    }
    for (actorIdx = 4; actorIdx < g_BattleEnemyCount + 4; actorIdx++) {
        if (((unsigned char)g_BattleActorRecords[actorIdx].drawFlags & 0x40) == 0 && ((unsigned char)g_BattleActorRecords[actorIdx].drawFlags & 2) == 0 && (g_BattleActorRecords[actorIdx].stateFlags & 1) != 0 && (g_BattleActorStates[actorIdx].flags & 4) != 0) {
            Battle_RunEnemyActorScript(actorIdx, scriptArgA, unusedB);
        }
    }
}

// Over ten 8-byte records starting at g_BattleActorScreenPositions, copies the two shorts at +4 and +6 down to +0
// and +2, so each record's second pair becomes its first.
// FUNCTION: FF7 0x0042e4e0
void __cdecl Battle_LatchActorProjectedPoints(void)
{
    int i;

    for (i = 0; i < 10; i++) {
        g_BattleActorScreenPositions[i].prevX = g_BattleActorScreenPositions[i].projectedX;
        g_BattleActorScreenPositions[i].prevY = g_BattleActorScreenPositions[i].projectedY;
    }
}

// Updates and draws stage parts according to the descriptor's stage type (enum BattleStageType),
// using g_BattleStageHideFlags to suppress part groups. Draws the rain of location 0x3A unless
// BATTLE_STAGE_HIDE_RAIN is set, sets retained colour and composes the root rotation. A depth-scroll
// stage draws flicker and eight scrolling layers; static stages divide the remaining parts at the
// descriptor's nodeCount. A yaw-spin stage rotates three groups by 1/2/4 while unpaused; a roll-spin
// stage rotates them by 8/6/4 and fixes their translations. A split-scroll stage draws paired
// repeated groups with two wrapped translations, then the last rotated part and two unpaused
// oscillators; transforms are composed before its translation decrement, and the same part ranges
// are drawn twice. Unknown types still perform rain/root setup.
// FUNCTION: FF7 0x0042e52e
void __cdecl Battle3d_UpdateAndDrawStage(void)
{
    BattleStageDescriptor *stageDesc;
    int partCount;
    int partIndex;

    partCount = Battle3d_GetStagePartCount(g_pBattle3dScene);
    partCount++;
    if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_RAIN) == 0 && g_BattleScene.locationId == 0x3A) {
        Battle3d_RainUpdateAndDraw();
    }
    SetDrawColorRGB(g_BattleFxTimelineState.red, g_BattleFxTimelineState.channel1, g_BattleFxTimelineState.channel2);
    RotMatrix((SVECTOR *)&g_BattleStageNodes[0].rotX, g_BattleStageNodes[0].matrix);
    Battle3d_ComposeNodeWithParent((Battle3dNode *)g_BattleStageNodes[0].matrix);
    stageDesc = (BattleStageDescriptor *)Battle3d_GetStageDescriptor(g_pBattle3dScene);
    switch (stageDesc->stageType) {
    case BATTLE_STAGE_TYPE_DEPTH_SCROLL:
        if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_PART_1_GROUP) == 0) {
            Battle3d_TickStageFlicker();
            Battle3d_DrawStagePart(1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            Battle3d_DrawStagePart(5, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            Battle3d_DrawStagePart(6, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            Battle3d_DrawStagePart(7, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            Battle3d_DrawStagePart(0xB, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
        }
        if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_OTHER_PARTS) == 0) {
            if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_PARTS_2_TO_4) == 0) {
                RotMatrix((SVECTOR *)&g_BattleStageNodes[1].rotX, g_BattleStageNodes[1].matrix);
                Battle3d_ComposeNodeWithParent((Battle3dNode *)g_BattleStageNodes[1].matrix);
                Battle3d_DrawStagePart(4, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
                Battle3d_DrawStagePart(3, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
                Battle3d_DrawStagePart(2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
            }
            Battle3d_DrawScrollingStageLayer(2, 1000, 0x7530, 0x9C40);
            Battle3d_DrawScrollingStageLayer(3, 1000, 0x4E20, 30000);
            Battle3d_DrawScrollingStageLayer(4, 1000, 0x2710, 20000);
            Battle3d_DrawScrollingStageLayer(5, 1000, 0, 10000);
            Battle3d_DrawScrollingStageLayer(6, 1000, 0xFFFFD8F0, 0);
            Battle3d_DrawScrollingStageLayer(7, 1000, 0xFFFFB1E0, 0xFFFFD8F0);
            Battle3d_DrawScrollingStageLayer(8, 1000, 0xFFFF8AD0, 0xFFFFB1E0);
            Battle3d_DrawScrollingStageLayer(9, 1000, 0xFFFF63C0, 0xFFFF8AD0);
        }
        break;
    case BATTLE_STAGE_TYPE_YAW_SPIN:
        if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_PART_1_GROUP) == 0) {
            Battle3d_DrawStagePart(1, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_0, 0, 0);
        }
        if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_OTHER_PARTS) == 0) {
            if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_PARTS_2_TO_4) == 0) {
                Battle3d_DrawStagePart(2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
                Battle3d_DrawStagePart(3, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
                Battle3d_DrawStagePart(4, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
            }
            if (g_BattleIsPaused == 0) {
                g_BattleStageNodes[1].yaw++;
            }
            RotMatrix((SVECTOR *)&g_BattleStageNodes[1].rotX, g_BattleStageNodes[1].matrix);
            Battle3d_ComposeNodeWithParent((Battle3dNode *)g_BattleStageNodes[1].matrix);
            Battle3d_DrawStagePart(5, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            if (g_BattleIsPaused == 0) {
                g_BattleStageNodes[2].yaw += 2;
            }
            RotMatrix((SVECTOR *)&g_BattleStageNodes[2].rotX, g_BattleStageNodes[2].matrix);
            Battle3d_ComposeNodeWithParent((Battle3dNode *)g_BattleStageNodes[2].matrix);
            Battle3d_DrawStagePart(6, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            if (g_BattleIsPaused == 0) {
                g_BattleStageNodes[3].yaw += 4;
            }
            RotMatrix((SVECTOR *)&g_BattleStageNodes[3].rotX, g_BattleStageNodes[3].matrix);
            Battle3d_ComposeNodeWithParent((Battle3dNode *)g_BattleStageNodes[3].matrix);
            Battle3d_DrawStagePart(7, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
        }
        break;
    case BATTLE_STAGE_TYPE_STATIC:
        if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_PART_1_GROUP) == 0) {
            Battle3d_DrawStagePart(1, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_0, 0, 0);
            for (partIndex = 5; partIndex < stageDesc->nodeCount; partIndex++) {
                Battle3d_DrawStagePart(partIndex, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_0, 0, 0);
            }
        }
        if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_OTHER_PARTS) == 0) {
            if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_PARTS_2_TO_4) == 0) {
                Battle3d_DrawStagePart(2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
                Battle3d_DrawStagePart(3, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
                Battle3d_DrawStagePart(4, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
            }
            for (partIndex = stageDesc->nodeCount; partIndex < partCount; partIndex++) {
                Battle3d_DrawStagePart(partIndex, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            }
        }
        break;
    case BATTLE_STAGE_TYPE_SPLIT_SCROLL:
        if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_OTHER_PARTS) == 0) {
            Battle3d_DrawStagePart(1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            Battle3d_DrawStagePart(7, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            RotMatrix((SVECTOR *)&g_BattleStageNodes[1].rotX, g_BattleStageNodes[1].matrix);
            Battle3d_ComposeNodeWithParent((Battle3dNode *)g_BattleStageNodes[1].matrix);
            Battle3d_DrawStagePart(2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            Battle3d_DrawStagePart(8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            RotMatrix((SVECTOR *)&g_BattleStageNodes[2].rotX, g_BattleStageNodes[2].matrix);
            Battle3d_ComposeNodeWithParent((Battle3dNode *)g_BattleStageNodes[2].matrix);
            if (g_BattleStageNodes[2].posY == 0xFFFFCF2C) {
                g_BattleStageNodes[2].posY = 0;
            }
            if (g_BattleIsPaused == 0) {
                g_BattleStageNodes[2].posY -= 100;
            }
            for (partIndex = 3; partIndex < 6; partIndex++) {
                Battle3d_DrawStagePart(partIndex, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            }
            for (partIndex = 9; partIndex < 11; partIndex++) {
                Battle3d_DrawStagePart(partIndex, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            }
            RotMatrix((SVECTOR *)&g_BattleStageNodes[4].rotX, g_BattleStageNodes[4].matrix);
            Battle3d_ComposeNodeWithParent((Battle3dNode *)g_BattleStageNodes[4].matrix);
            if (g_BattleStageNodes[4].posY == 0) {
                g_BattleStageNodes[4].posY = 12500;
            }
            if (g_BattleIsPaused == 0) {
                g_BattleStageNodes[4].posY -= 100;
            }
            for (partIndex = 3; partIndex < 6; partIndex++) {
                Battle3d_DrawStagePart(partIndex, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            }
            for (partIndex = 9; partIndex < 11; partIndex++) {
                Battle3d_DrawStagePart(partIndex, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            }
            RotMatrix((SVECTOR *)&g_BattleStageNodes[3].rotX, g_BattleStageNodes[3].matrix);
            Battle3d_ComposeNodeWithParent((Battle3dNode *)g_BattleStageNodes[3].matrix);
            Battle3d_DrawStagePart(6, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            if (g_BattleIsPaused == 0) {
                Battle3d_TickStageOscillator(0);
                Battle3d_TickStageOscillator(1);
            }
        }
        break;
    case BATTLE_STAGE_TYPE_ROLL_SPIN:
        if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_PART_1_GROUP) == 0) {
            Battle3d_DrawStagePart(1, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 3, 0);
            Battle3d_DrawStagePart(5, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 3, 0);
        }
        if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_OTHER_PARTS) == 0) {
            if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_PARTS_2_TO_4) == 0) {
                Battle3d_DrawStagePart(2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
                RotMatrix((SVECTOR *)&g_BattleStageNodes[4].rotX, g_BattleStageNodes[4].matrix);
                Battle3d_ComposeNodeWithParent((Battle3dNode *)g_BattleStageNodes[4].matrix);
                Battle3d_DrawStagePart(3, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
                Battle3d_DrawStagePart(4, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
            }
            if (g_BattleIsPaused == 0) {
                g_BattleStageNodes[1].rotZ += 8;
                g_BattleStageNodes[1].posY = 0xF63C;
            }
            RotMatrix((SVECTOR *)&g_BattleStageNodes[1].rotX, g_BattleStageNodes[1].matrix);
            Battle3d_ComposeNodeWithParent((Battle3dNode *)g_BattleStageNodes[1].matrix);
            if (g_BattleIsPaused == 0) {
                g_BattleStageNodes[2].rotZ += 6;
                g_BattleStageNodes[2].posY = 0xF63C;
            }
            RotMatrix((SVECTOR *)&g_BattleStageNodes[2].rotX, g_BattleStageNodes[2].matrix);
            Battle3d_ComposeNodeWithParent((Battle3dNode *)g_BattleStageNodes[2].matrix);
            if (g_BattleIsPaused == 0) {
                g_BattleStageNodes[3].rotZ += 4;
                g_BattleStageNodes[3].posY = 0xF63C;
            }
            RotMatrix((SVECTOR *)&g_BattleStageNodes[3].rotX, g_BattleStageNodes[3].matrix);
            Battle3d_ComposeNodeWithParent((Battle3dNode *)g_BattleStageNodes[3].matrix);
            Battle3d_DrawStagePart(8, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_0, 0, 0);
        }
        break;
    case BATTLE_STAGE_TYPE_STATIC_SORTED:
        if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_PART_1_GROUP) == 0) {
            Battle3d_DrawStagePart(1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            for (partIndex = 5; partIndex < stageDesc->nodeCount; partIndex++) {
                Battle3d_DrawStagePart(partIndex, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            }
        }
        if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_OTHER_PARTS) == 0) {
            if (((unsigned char)g_BattleStageHideFlags & BATTLE_STAGE_HIDE_PARTS_2_TO_4) == 0) {
                Battle3d_DrawStagePart(2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
                Battle3d_DrawStagePart(3, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
                Battle3d_DrawStagePart(4, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_13, 4, 0);
            }
            for (partIndex = stageDesc->nodeCount; partIndex < partCount; partIndex++) {
                Battle3d_DrawStagePart(partIndex, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
            }
        }
    }
}

// Draws one scrolling stage layer. The stage node records are 0x34 bytes apart in
// g_BattleStageNodes. The node matrix is built with RotMatrix from the node's rotation and composed
// with its parent. The node's posZ is reset to resetValue when it has reached wrapLimit, and is
// advanced by scrollStep while the battle is not paused. Battle3d_DrawStagePart then draws parts 8,
// 9 and 0xA, each into the overlay draw list at handle + 0x70 with shift 0xC.
// FUNCTION: FF7 0x0042efb2
void __cdecl Battle3d_DrawScrollingStageLayer(char layerIndex, int scrollStep, int resetValue, int wrapLimit)
{
    RotMatrix((SVECTOR *)&g_BattleStageNodes[layerIndex].rotX, g_BattleStageNodes[layerIndex].matrix);
    Battle3d_ComposeNodeWithParent((Battle3dNode *)g_BattleStageNodes[layerIndex].matrix);
    if (g_BattleStageNodes[layerIndex].posZ == wrapLimit) {
        g_BattleStageNodes[layerIndex].posZ = resetValue;
    }
    if (g_BattleIsPaused == 0) {
        g_BattleStageNodes[layerIndex].posZ += scrollStep;
    }
    Battle3d_DrawStagePart(8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
    Battle3d_DrawStagePart(9, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
    Battle3d_DrawStagePart(0xA, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
}

// Each unpaused frame rebuilds the stage brightness offset g_BattleStageNodes[0].posY from zero: minus 0x28 when
// the four-frame counter g_BattleStageNodeState[0] is at zero and minus 0x50 when the 32-frame counter
// g_BattleStageNodeState[1] is, then steps both counters down modulo their period.
// FUNCTION: FF7 0x0042f088
void __cdecl Battle3d_TickStageFlicker(void)
{
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleStageNodes[0].posY = 0;
    if (((short *)&g_BattleStageNodeState)[0] == 0) {
        g_BattleStageNodes[0].posY -= 40;
    }
    if (((short *)&g_BattleStageNodeState)[1] == 0) {
        g_BattleStageNodes[0].posY -= 80;
    }
    ((short *)&g_BattleStageNodeState)[0]--;
    ((short *)&g_BattleStageNodeState)[0] &= 3;
    ((short *)&g_BattleStageNodeState)[1]--;
    ((short *)&g_BattleStageNodeState)[1] &= 0x1F;
}

// Ticks one stage oscillator. The short counter array at g_BattleStageNodeState is indexed by index: while
// the counter is below 0x10 the layer angle g_BattleStageNodes[index].posY is stepped by -0x19 and
// above it by +0x19, then the counter is decremented and masked with 0x1F, giving a 32-step back-
// and-forth cycle.
// FUNCTION: FF7 0x0042f126
void __cdecl Battle3d_TickStageOscillator(int index)
{
    if (((short *)&g_BattleStageNodeState)[index] < 16) {
        g_BattleStageNodes[index].posY -= 25;
    } else {
        g_BattleStageNodes[index].posY += 25;
    }
    (((short *)&g_BattleStageNodeState)[index])--;
    ((short *)&g_BattleStageNodeState)[index] &= 0x1F;
}

// Returns unless the stage drawing flag and graphics draw enable are set; when the scene and stage
// pointer exist replaces packed draw-color alpha with the current fade conversion and submits the
// requested stage part geometry.
// FUNCTION: FF7 0x0042f1ad
void __cdecl Battle3d_DrawStagePart(int part, int otEntry, int depthShift, int flags)
{
#pragma pack(push, 1)
    struct {
        int at16;
        int at12;
        union {
            int word;
            struct {
                char pad[3];
                char alpha;
            } bytes;
        } at8;
        int at4;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    if (!g_Battle3d_StageDrawEnabled || !Gfx_IsDrawEnabled())
        return;
    if (g_pBattle3dScene != 0) {
        frame.at12 = g_pBattle3dScene->stageId;
        if (frame.at12 != 0) {
            frame.at4 = Battle3d_FadeLevelToAlpha(g_BattleFxTimelineState.startValue);
            frame.at16 = GetDrawColorPacked();
            frame.at8.word = frame.at16;
            frame.at8.bytes.alpha = (char)frame.at4;
            BattleStage_SubmitPartGeometry(frame.at8.word, part, (Battle3dStageRecord *)frame.at12);
        }
    }
}

// For party actors 0..2, when actor flag 2 is clear updates render state and queues draw (1, 0,
// actor); otherwise queues draw (0, 0, actor).
// FUNCTION: FF7 0x0042f21f
void __cdecl Battle3d_QueuePartyActorDraws(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (((unsigned char)g_BattleActorRecords[i].drawFlags & 2) == 0) {
            Battle3d_UpdateActorRenderState((char)i);
            Battle3d_QueueActorDraw(1, 0, i);
        } else {
            Battle3d_QueueActorDraw(0, 0, i);
        }
    }
}

// For enemy actors 4..g_BattleEnemyCount+3, updates render state and queues draw (1, 1, actor) only when
// actor flag 0x80 is set and bit 2 is clear; otherwise queues draw (0, 1, actor).
// FUNCTION: FF7 0x0042f287
void __cdecl Battle3d_QueueEnemyActorDraws(void)
{
    int i;

    for (i = 4; i < g_BattleEnemyCount + 4; i++) {
        if (((unsigned char)g_BattleActorRecords[i].drawFlags & 0x80) != 0) {
            if (((unsigned char)g_BattleActorRecords[i].drawFlags & 2) == 0) {
                Battle3d_UpdateActorRenderState((char)i);
                Battle3d_QueueActorDraw(1, 1, i);
            } else {
                Battle3d_QueueActorDraw(0, 1, i);
            }
        } else {
            Battle3d_QueueActorDraw(0, 1, i);
        }
    }
}

// Shadow-render begin hook: reads the engine context with GetEngineContext and sets g_Battle3d_ShadowRenderActive
// to 1 when bit 0 of the context's flags is set and to 0 otherwise, then clears the vertex
// counter g_Battle3d_ShadowVertexCallCount.
// FUNCTION: FF7 0x0042f327
void __cdecl Battle3d_ShadowBeginCallback(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if ((ctx->flags & 1) != 0) {
        g_Battle3d_ShadowRenderActive = 1;
    } else {
        g_Battle3d_ShadowRenderActive = 0;
    }
    g_Battle3d_ShadowVertexCallCount = 0;
}

// Per-vertex alpha hook for the shadow pass: writes the alpha byte at vertex+3 from the source alpha
// at srcVertex+0xB. For an opaque source (0xFF) it writes 0xFF when g_Battle3d_ShadowRenderActive is set and
// g_Battle3d_ShadowVertexCallCount is 2, else 0x60; for any other source alpha it passes the value through under the
// same condition and otherwise scales it by 0x60/0xFF. g_Battle3d_ShadowVertexCallCount is incremented on every call,
// so only the third vertex of a run takes the unscaled path.
// FUNCTION: FF7 0x0042f367
void __cdecl Battle3d_ShadowVertexAlphaCallback(PolygonBgraColor *vertex, GfxDrawSpec *srcVertex)
{
    if (*(unsigned char *)((char *)srcVertex + 0xB) == 0xFF) {
        if (g_Battle3d_ShadowRenderActive != 0 && g_Battle3d_ShadowVertexCallCount == 2) {
            *(char *)((char *)vertex + 3) = 0xFF;
        } else {
            *(char *)((char *)vertex + 3) = 0x60;
        }
    } else if (g_Battle3d_ShadowRenderActive != 0 && g_Battle3d_ShadowVertexCallCount == 2) {
        *(char *)((char *)vertex + 3) = *(char *)((char *)srcVertex + 0xB);
    } else {
        *(char *)((char *)vertex + 3) = *(unsigned char *)((char *)srcVertex + 0xB) * 0x60 / 0xFF;
    }
    g_Battle3d_ShadowVertexCallCount++;
}

// Returns immediately when drawing is disabled. Derives uniform or per-axis actor scale, sets
// actor RGB and configures a model draw specification with fade alpha, special model-ID modes and
// optional packed UV offsets. For a nonnull model writes float scale/rotation/position and actor
// matrix pointers, selects stage or attachment parent, draws the animation frame, saves its
// transform, draws overlays and optionally updates party attachment nodes. Projects the selected
// bone into actor screen shorts, writing -1 on projection failure. When flags 4/0x10 and model ID
// 0x65 permit, builds a flattened scaled floor shadow with fade callbacks, choosing its floor
// height from the stage property and actor offset. Finally submits the model unless flag 4
// suppresses it, applying and clearing a context override when the retained mode requests it;
// model ID 0x9E additionally toggles state 0xE. The final non-override submit can receive a null
// model object. Many matrix, projection and specification aggregates are represented by addresses
// of individual scalar locals, so current C layout is not a portable aggregate guarantee.
// FUNCTION: FF7 0x0042f3e8
void __cdecl Battle_DrawActor(int actorIndex)
{
    int specialMode;
    int overrideFlag;
    int parentRef;
    PolygonModel *modelHandle;
    float uvOffsetU;
    int floorY;
    BattleModelBank *animationBank;
    BattleActorDrawState *pDrawState;
    int baseMatrix[16];
    GfxEngineContext *stepGfxContext;
    int baseOfs;
    int mesh;
    int stageRef8;
    int scaleBuf[16];
    int shadowBgra;
    GfxHomogeneousPoint pointX;
    int colorA;
    int attachMatrix[8];
    int modelMtx[16];
    GfxGroupDrawSpec specBase;
    int projectedZ[4];
    int yAxis[4][4];
    int zScaleFixed;
    int toggleFlag;
    GfxPolygonSet *polySet;
    int scaleYFix;
    int alpha;
    int localJointIndex;
    GfxDrawSpec *pSpecPtr;
    int trailSlot;
    BattleStageDescriptor *stageInfoPtr;
    float uvOffsetV;
    int localBonePoint;
    GfxHomogeneousVec4 pxX;
    int scaleXRaw;
    BattleEffectPrimitive *primitivePtr;
    GfxMatrix4x4Dual mixMtx;
    int shadowMode;
    GfxVec3Dual scaleVectorX;

    baseOfs = g_BattleActor_Offset00c06228;
    primitivePtr = g_BattleActor_Offset00c06328;
    if (Gfx_IsDrawEnabled() == 0) {
        return;
    }
    g_BattleVmReg_3 = 0;
    if ((g_BattleActorStates[actorIndex].flags & 0x20) != 0) {
        g_BattleVmReg_0 = g_BattleActorStates[actorIndex].poseAngleA * g_BattleActorRecords[actorIndex].scale >> 12;
        g_BattleVmReg_1 = g_BattleActorStates[actorIndex].poseAngleB * g_BattleActorRecords[actorIndex].scale >> 12;
        g_BattleVmReg_2 = g_BattleActorStates[actorIndex].poseAngleC * g_BattleActorRecords[actorIndex].scale >> 12;
    } else {
        g_BattleVmReg_2 = g_BattleActorRecords[actorIndex].scale;
        g_BattleVmReg_1 = g_BattleVmReg_2;
        g_BattleVmReg_0 = g_BattleVmReg_1;
    }
    scaleXRaw = g_BattleVmReg_0;
    scaleYFix = g_BattleVmReg_1;
    zScaleFixed = g_BattleVmReg_2;
    SetDrawColorRGB((unsigned char)g_BattleActorRecords[actorIndex].tintR, (unsigned char)g_BattleActorRecords[actorIndex].tintG, (unsigned char)g_BattleActorRecords[actorIndex].tintB);
    pSpecPtr = &specBase.spec;
    overrideFlag = 0;
    modelHandle = 0;
    animationBank = 0;
    stepGfxContext = GetEngineContext();
    if (g_BattleActorRecords[actorIndex].modelId == 0x21) {
        specialMode = 1;
    } else {
        specialMode = 0;
    }
    if (g_BattleActorRecords[actorIndex].modelId == 0x9E) {
        toggleFlag = 1;
    } else {
        toggleFlag = 0;
    }
    if (g_BattleActorRecords[actorIndex].modelId == 0x65) {
        shadowMode = 1;
    } else {
        shadowMode = 0;
    }
    animationBank = (BattleModelBank *)(g_BattleActorRecords[actorIndex].model);
    if (animationBank != 0) {
        Gfx_InitDrawSpec(0, pSpecPtr);
        colorA = GetDrawColorPacked();
        pSpecPtr->packedColor = colorA;
        alpha = Battle3d_FadeLevelToAlpha(g_BattleActorRecords[actorIndex].tintPhase);
        pSpecPtr->alpha = alpha;
        if (toggleFlag != 0) {
            g_BattleActorRecords[actorIndex].fadeFlags |= 8;
        }
        if ((g_BattleActorRecords[actorIndex].fadeFlags & 8) != 0) {
            if ((pSpecPtr->packedColor & 0xFFFFFF) != 0) {
                if (actorIndex == 3) {
                    pSpecPtr->kind = 9;
                } else {
                    pSpecPtr->kind = 0xB;
                }
            } else if (actorIndex == 3) {
                pSpecPtr->kind = 0xA;
            } else {
                pSpecPtr->kind = 0xC;
            }
            overrideFlag = 1;
        } else if ((pSpecPtr->packedColor & 0xFFFFFF) != 0) {
            if (actorIndex == 3) {
                pSpecPtr->kind = 9;
            } else if (specialMode != 0) {
                pSpecPtr->kind = 9;
            } else {
                pSpecPtr->kind = 0xB;
            }
        } else if (actorIndex == 3) {
            pSpecPtr->kind = 0xA;
        } else if (specialMode != 0) {
            pSpecPtr->kind = 0xA;
        } else {
            pSpecPtr->kind = 0xC;
        }
        if ((unsigned char)g_BattleActorStates[actorIndex].drawEnabled != 0) {
            if (g_BattleActorStates[actorIndex].mirrorWord != 0) {
                pSpecPtr->uvMode = 2;
                uvOffsetU = (float)((unsigned short)g_BattleActorStates[actorIndex].mirrorWord & 0xFF);
                uvOffsetV = (float)((unsigned short)g_BattleActorStates[actorIndex].mirrorWord >> 8);
                pSpecPtr->scrollU = uvOffsetU * 0.00390625f;
                pSpecPtr->scrollV = uvOffsetV * 0.00390625f;
            } else {
                pSpecPtr->uvMode = 1;
            }
        }
        modelHandle = (PolygonModel *)(animationBank->skeleton);
        modelHandle->drawEnabled = 0;
        Gfx_ApplyDrawSpecToModel(pSpecPtr, modelHandle);
        pDrawState = (BattleActorDrawState *)&animationBank->drawState;
        pDrawState->scaleX = (float)scaleXRaw / 4096.0f;
        pDrawState->scaleY = (float)scaleYFix / 4096.0f;
        pDrawState->scaleZ = (float)zScaleFixed / 4096.0f;
        pDrawState->rotX = (float)g_BattleActorRecords[actorIndex].rotX * 0.087890625f;
        pDrawState->yaw = (float)g_BattleActorRecords[actorIndex].yaw * 0.087890625f;
        pDrawState->rotZ = (float)g_BattleActorRecords[actorIndex].rotZ * 0.087890625f;
        pDrawState->posX = (float)g_BattleActorRecords[actorIndex].posX;
        pDrawState->posY = (float)g_BattleActorRecords[actorIndex].posY;
        pDrawState->posZ = (float)g_BattleActorRecords[actorIndex].posZ;
        pDrawState->boneWorldPosX = &g_BattleActorRecords[actorIndex].boneWorldPosX;
        pDrawState->worldMatrixPtr = &g_BattleActorRecords[actorIndex].worldMatrix[0x40];
        if (g_BattleActorRecords[actorIndex].deathActive != 0) {
            modelHandle->deathActivePtr = &g_BattleActorRecords[actorIndex].deathActive;
        } else {
            modelHandle->deathActivePtr = 0;
        }
        stageInfoPtr = (BattleStageDescriptor *)Battle3d_GetStageDescriptor(g_pBattle3dScene);
        if (stageInfoPtr->stageType == BATTLE_STAGE_TYPE_SPLIT_SCROLL) {
            if (actorIndex < 3) {
                parentRef = (int)g_BattleStageNodes[0].matrix;
            } else {
                parentRef = (int)g_BattleStageNodes[1].matrix;
            }
        } else if (actorIndex == 3) {
            parentRef = (int)&g_BattleCameraTransform;
        } else {
            parentRef = (int)g_BattleStageNodes[0].matrix;
        }
        if ((g_BattleActorStates[actorIndex].flags & 1) != 0) {
            Battle3d_GetActorAttachMatrix(g_BattleActorRecords[actorIndex].attachActor, g_BattleActorRecords[actorIndex].attachBone, (PsxMatrix *)attachMatrix);
            parentRef = (int)attachMatrix;
        }
        GtePsxMatrixToFloat4x4Transposed((void *)parentRef, g_BattleActorRecords[actorIndex].worldMatrix);
        Gfx_SetContextMatrix((void *)g_BattleActorRecords[actorIndex].worldMatrix, stepGfxContext);
        BattleModel_DrawAnimFrame(g_BattleActorRecords[actorIndex].useSecondAngleTable, g_BattleActorRecords[actorIndex].animFrame, g_BattleActorRecords[actorIndex].animBank, animationBank);
        *(BattleModelDrawState *)(actorIndex * 0x74 + (char *)&g_BattleSummonOdinZTrackBuffer) = animationBank->drawState;
        ClearMatrixColumn4(&g_BattleActorRecords[actorIndex].worldMatrix[0x40]);
        ConcatAffineMatrix4x4((char *)modelHandle + 0x64, g_BattleActorRecords[actorIndex].worldMatrix, &g_BattleActorRecords[actorIndex].worldMatrix[0x40]);
        SetRotMatrix((void *)parentRef);
        SetTransMatrix((void *)parentRef);
        Battle3d_DrawActorOverlays((char)actorIndex);
        if ((gSavemap.options & 0x300) == 0x100) {
            trailSlot = actorIndex;
            if (trailSlot < 3) {
                g_BattleFxTimelineState.nodes[trailSlot].parentMatrix = g_BattleActorRecords[trailSlot].rootParentMatrix;
                g_BattleFxTimelineState.nodes[trailSlot].posX = g_BattleActorRecords[trailSlot].posX;
                g_BattleFxTimelineState.nodes[trailSlot].posY = 0xFE0C;
                g_BattleFxTimelineState.nodes[trailSlot].posZ = g_BattleActorRecords[trailSlot].posZ;
                g_BattleFxTimelineState.nodes[trailSlot].rotX = g_BattleActorRecords[trailSlot].rotX;
                g_BattleFxTimelineState.nodes[trailSlot].yaw = g_BattleActorRecords[trailSlot].yaw;
                g_BattleFxTimelineState.nodes[trailSlot].rotZ = g_BattleActorRecords[trailSlot].rotZ;
                RotMatrix((SVECTOR *)&g_BattleFxTimelineState.nodes[trailSlot].rotX, &g_BattleFxTimelineState.nodes[trailSlot]);
                Battle3d_ComposeNodeWithParent(&g_BattleFxTimelineState.nodes[trailSlot]);
            }
        }
        localJointIndex = g_BattleActorRecords[actorIndex].targetBone;
        localBonePoint = (int)&g_BattleActorRecords[actorIndex].boneWorldPosX + localJointIndex * 0xC;
        if (ProjectPointWithMatrix((int)g_BattleActorRecords[actorIndex].worldMatrix, localBonePoint, (int)projectedZ, (int)&pxX.x) != 0) {
            g_BattleActorScreenPositions[actorIndex].projectedX = (int)pxX.x;
            g_BattleActorScreenPositions[actorIndex].projectedY = (int)pxX.y;
        } else {
            g_BattleActorScreenPositions[actorIndex].projectedX = 0xFFFF;
            g_BattleActorScreenPositions[actorIndex].projectedY = 0xFFFF;
        }
    }
    if (((unsigned char)g_BattleActorRecords[actorIndex].drawFlags & 4) == 0 && ((unsigned char)g_BattleActorRecords[actorIndex].drawFlags & 0x10) == 0 && animationBank != 0 && shadowMode == 0) {
        if ((g_BattleScene.targetMask & 1 << actorIndex) == 0) {
            SetDrawColorRGB((int)(unsigned char)g_BattleActorRecords[actorIndex].tintR / 4, (int)(unsigned char)g_BattleActorRecords[actorIndex].tintR / 4, (int)(unsigned char)g_BattleActorRecords[actorIndex].tintR / 4);
        } else {
            SetDrawColorRGB(0, 0, 0);
        }
        if (modelHandle != 0) {
            mixMtx.f = *(GfxMatrix4x4 *)((int)modelHandle + 0x64);
            BuildRotationYRowVec((float)g_BattleActorRecords[actorIndex].yaw * 0.087890625f, yAxis);
            mixMtx.bits[0][0] = yAxis[0][0];
            mixMtx.bits[0][1] = yAxis[0][1];
            mixMtx.bits[0][2] = yAxis[0][2];
            mixMtx.bits[1][0] = yAxis[1][0];
            mixMtx.bits[1][1] = yAxis[1][1];
            mixMtx.bits[1][2] = yAxis[1][2];
            mixMtx.bits[2][0] = yAxis[2][0];
            mixMtx.bits[2][1] = yAxis[2][1];
            mixMtx.bits[2][2] = yAxis[2][2];
            stageRef8 = g_pBattle3dScene->stageId;
            if (BattleStage_UsesDepthTestedFloor(*(int *)stageRef8) != 0) {
                floorY = 0xC1200000;
            } else {
                floorY = 0xC0800000;
            }
            if (g_BattleActorRecords[actorIndex].modelSlot == -1) {
                mixMtx.m[3][0] = 0.0f;
                mixMtx.bits[3][1] = floorY;
                mixMtx.m[3][2] = 0.0f;
            } else {
                scaleVectorX.x = 0.0f;
                scaleVectorX.yBits = 0;
                scaleVectorX.z = (float)g_BattleActorRecords[actorIndex].modelSlot;
                TransformPoint3RowVec(yAxis, &scaleVectorX.x, &pointX.x);
                mixMtx.m[3][0] += pointX.x;
                mixMtx.bits[3][1] = floorY;
                mixMtx.m[3][2] += pointX.z;
            }
            scaleVectorX.x = (float)g_BattleActorRecords[actorIndex].scaleX / 4096.0f;
            scaleVectorX.yBits = FLOAT_ONE_BITS;
            scaleVectorX.z = (float)g_BattleActorRecords[actorIndex].scaleY / 4096.0f;
            scaleVectorX.x = (float)scaleXRaw / 4096.0f * scaleVectorX.x;
            scaleVectorX.z = (float)zScaleFixed / 4096.0f * scaleVectorX.z;
            *(GfxMatrix4x4 *)(baseMatrix) = mixMtx.f;
            BuildScaleMatrixXYZ((GfxExecPosition *)&scaleVectorX.x, (GfxScaleMatrix *)scaleBuf);
            ConcatAffineMatrix4x4(scaleBuf, baseMatrix, &mixMtx.f);
            MultiplyMatrix4x4Full(&mixMtx.f, g_BattleActorRecords[actorIndex].worldMatrix, modelMtx);
            mesh = (int)g_pBattle3dScene->model;
            polySet = (GfxPolygonSet *)(*(int *)((char *)mesh + 4));
            Gfx_InitDrawSpec(0x10, pSpecPtr);
            pSpecPtr->mode = 1;
            shadowBgra = GetDrawColorPacked();
            pSpecPtr->packedColor = shadowBgra;
            alpha = Battle3d_FadeLevelToAlpha(g_BattleActorRecords[actorIndex].tintPhase);
            pSpecPtr->alpha = alpha;
            pSpecPtr->beginCallback = Battle3d_ShadowBeginCallback;
            pSpecPtr->vertexAlphaFn = (void (__cdecl *)(void *, void *))Battle3d_ShadowVertexAlphaCallback;
            polySet->drawSpecApplied = 1;
            polySet->drawSpec = (int)pSpecPtr;
            Gfx_SetModelMatrix((int)modelMtx, mesh, stepGfxContext);
            if (stepGfxContext->scenePass == 0) { /* empty first arm: lib/codegen.h */
            } else {
                Gfx_ApplyDrawSpecToPolygonSet(pSpecPtr, polySet);
            }
            Gfx_DrawMeshPolygonSet(mesh, (int)stepGfxContext);
        }
    }
    if (((unsigned char)g_BattleActorRecords[actorIndex].drawFlags & 4) == 0) {
        if (overrideFlag != 0) {
            Gfx_ApplyPresetStateCall(1, stepGfxContext);
            SelectContextSlotOverride(1, stepGfxContext);
            if (toggleFlag != 0) {
                Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 1, stepGfxContext);
                Gfx_DrawModelPolygonSets(modelHandle, stepGfxContext);
                Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 0, stepGfxContext);
            } else {
                Gfx_DrawModelPolygonSets(modelHandle, stepGfxContext);
            }
            ClearContextSlotOverride(stepGfxContext);
        } else {
            Gfx_DrawModelPolygonSets(modelHandle, stepGfxContext);
        }
    }
}

// For each actor node, stores its parentMatrix pointer in the scratch at g_BattleActor_BufferPtr and copies the pointed-to
// PsxMatrix into the scratch's matrix. The same scratch slot is overwritten for every node;
// g_Battle3d_NodeMatrixScratch is set to g_BattleActor_BufferPtr.
// FUNCTION: FF7 0x00430266
void __cdecl Battle3d_PushActorNodeMatrices(int actorIndex)
{
    int nodePointer;
    int i;
    g_Battle3d_NodeMatrixScratch = (BattleNodeMatrixScratch *)g_BattleActor_BufferPtr;
    for (i = 0; i < g_BattleActorRecords[actorIndex].boneCount; i++) {
        nodePointer = (int)g_BattleActorRecords[actorIndex].boneNodes + i * 52;
        g_Battle3d_NodeMatrixScratch->parentMatrix = *(int *)(nodePointer + 0x30);
        g_Battle3d_NodeMatrixScratch->matrix = *(PsxMatrix *)g_Battle3d_NodeMatrixScratch->parentMatrix;
    }
}

// Copies the node parent matrix into the scratch matrix at g_BattleActor_BufferPtr, concatenates it with node, installs
// the scratch matrix, transforms the node's position into its translation, then installs node as
// rotation and translation matrix.
// FUNCTION: FF7 0x004302e8
void __cdecl Battle3d_ComposeNodeWithParent(Battle3dNode *node)
{
    int depth;
    g_Battle3d_NodeMatrixScratch = (BattleNodeMatrixScratch *)g_BattleActor_BufferPtr;
    g_Battle3d_NodeMatrixScratch->parentMatrix = node->parentMatrix;
    g_Battle3d_NodeMatrixScratch->matrix = *(PsxMatrix *)g_Battle3d_NodeMatrixScratch->parentMatrix;
    ConcatMatrixInPlace(&g_Battle3d_NodeMatrixScratch->matrix, node);
    SetRotMatrix(&g_Battle3d_NodeMatrixScratch->matrix);
    SetTransMatrix(&g_Battle3d_NodeMatrixScratch->matrix);
    RotTransPoint(&node->pos, node->translation, &depth);
    SetRotMatrix(node);
    SetTransMatrix(node);
}

// Scales the matrix at matrix by the vector at scaleVec with ScaleMatrix and then installs it in the GTE
// with SetRotMatrix and SetTransMatrix.
// FUNCTION: FF7 0x00430391
void __cdecl Gte_ScaleAndInstallMatrix(void *matrix, void *scaleVec)
{
    ScaleMatrix(matrix, scaleVec);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
}

// Mechanical setter: stores pass into the int at polySet+0x2C. Nobody has named that field.
// FUNCTION: FF7 0x004303be
void __cdecl Battle3d_SetPolygonSetHook(int pass, GfxPolygonSet *polySet)
{
    *(int *)((char *)polySet + 0x2C) = pass;
}

// When drawing is enabled and actor flag 4 is clear, sets actor RGB and obtains the party stat
// block. Selects the weapon polygon model using its low item nibble when actor model, weapon table
// and selected entry are nonnull. Builds mode 9 for nonblack colour or 0xA for black with fade
// alpha; the flag-8 colour branches are identical. For actor model ID zero with item nibble 0xF,
// applies the retained polygon hooks, ordering bias -4 and zero-HP ratio, with the untextured
// preset when flag 8 is set. Applies the draw spec, clears two body matrix pointers, draws the
// sub-skeleton using animation offset 8 when the retained frame pointer is nonzero or 0x34
// otherwise, then copies its cached transform into the auxiliary actor matrix.
// FUNCTION: FF7 0x004303cc
void __cdecl Battle3d_DrawActorWeapon(int actorIndex)
{
    int alpha;
    struct MenuPartyStats *statBlock;
    int otShift;
    Battle3dActorModel *anim;
    GfxDrawSpec *req;
    int flag;
    GfxGroupDrawSpec state;
    PolygonModel *polySet;
    int itemId;
    int otBias;
    int pass;
    int frac0;
    BattleActorDrawState *body;
    int base;
    int nextPMatrix;
    int rgba1;

    if (Gfx_IsDrawEnabled() == 0) {
        return;
    }
    if (((unsigned char)g_BattleActorRecords[actorIndex].drawFlags & 4) == 0) {
        SetDrawColorRGB((unsigned char)g_BattleActorRecords[actorIndex].tintR,
                        (unsigned char)g_BattleActorRecords[actorIndex].tintG,
                        (unsigned char)g_BattleActorRecords[actorIndex].tintB);
        statBlock = Savemap_GetPartyStatBlock(actorIndex);
        if (statBlock != 0) {
            itemId = statBlock->paramIndex & 0xF;
            if (g_BattleActorRecords[actorIndex].modelId == 0 && itemId == 0xF) {
                flag = 1;
            } else {
                flag = 0;
            }
            anim = (Battle3dActorModel *)(g_BattleActorRecords[actorIndex].model);
            if (anim != 0) {
                if (anim->partTable != 0) {
                    polySet = (PolygonModel *)(((int *)(anim->partTable))[itemId]);
                    if (polySet != 0) {
                        req = &state.spec;
                        Gfx_InitDrawSpec(0, req);
                        rgba1 = GetDrawColorPacked();
                        req->packedColor = rgba1;
                        alpha = Battle3d_FadeLevelToAlpha(
                            g_BattleActorRecords[actorIndex].tintPhase);
                        req->alpha = (char)alpha;
                        if ((g_BattleActorRecords[actorIndex].fadeFlags & 8) != 0) {
                            if ((unsigned char)g_BattleActorRecords[actorIndex].tintR != 0 ||
                                (unsigned char)g_BattleActorRecords[actorIndex].tintG != 0 ||
                                (unsigned char)g_BattleActorRecords[actorIndex].tintB != 0) {
                                req->kind = 9;
                            } else {
                                req->kind = 0xA;
                            }
                        } else {
                            if ((unsigned char)g_BattleActorRecords[actorIndex].tintR != 0 ||
                                (unsigned char)g_BattleActorRecords[actorIndex].tintG != 0 ||
                                (unsigned char)g_BattleActorRecords[actorIndex].tintB != 0) {
                                req->kind = 9;
                            } else {
                                req->kind = 0xA;
                            }
                        }
                        if (flag != 0) {
                            if ((g_BattleActorRecords[actorIndex].fadeFlags & 8) != 0) {
                                pass = 0;
                                Gfx_ApplyStateUntexturedPresetB(GetEngineContext());
                            } else {
                                pass = 1;
                            }
                            Gfx_ForEachModelPolygonSet(pass, (GfxPolygonSetFn)Battle3d_SetPolygonSetHook, polySet);
                            otBias = -4;
                            Gfx_AndModelPolygonSetRenderFlags(otBias, polySet);
                            frac0 = Menu_GetCharZeroHpRatio();
                            Gfx_ForEachModelPolygonSet(frac0, (GfxPolygonSetFn)Gfx_MaskFirstGroupSavedBlueToGreenRed, polySet);
                        }
                        Gfx_ApplyDrawSpecToModel(req, polySet);
                        body = (BattleActorDrawState *)((int)anim + 0x20);
                        body->outPos = 0;
                        body->transformBase = 0;
                        if (g_BattleActorRecords[actorIndex].useSecondAngleTable != 0) {
                            otShift = 8;
                            BattleModel_DrawAnimFrameOnSubSkeleton(
                                g_BattleActorRecords[actorIndex].useSecondAngleTable,
                                g_BattleActorRecords[actorIndex].animFrame + otShift,
                                g_BattleActorRecords[actorIndex].animBank, itemId, anim);
                        } else {
                            otShift = 0x34;
                            BattleModel_DrawAnimFrameOnSubSkeleton(
                                g_BattleActorRecords[actorIndex].useSecondAngleTable,
                                g_BattleActorRecords[actorIndex].animFrame + otShift,
                                g_BattleActorRecords[actorIndex].animBank, itemId, anim);
                        }
                        base = (int)polySet->polySets->entries->modelRef->polySet;
                        nextPMatrix = GetCachedDeviceTransform(0, (void *)(*(int *)((char *)base + 0x40)));
                        GteFloat4x4TransposedToPsxMatrix((PsxMatrix *)g_BattleActorTransformRecords[actorIndex].auxNode,
                                                         (float *)nextPMatrix);
                    }
                }
            }
        }
    }
}

// When drawing is enabled and actor flag 4 is clear, sets actor RGB and draws submodel zero from a
// nonnull actor model. Dereferences its submodel table without a null check, skips a null selected
// entry, and builds mode 9 for any nonzero colour or 0xA for black with fade alpha; flag 8 does
// not change those choices. Applies the draw spec, clears body offset 0x6C and draws sub-skeleton
// zero with animation offset 0x10.
// FUNCTION: FF7 0x0043076b
void __cdecl Battle3d_DrawActorSubModel(int actorIndex)
{
    GfxGroupDrawSpec specBuffer;
    PolygonModel *submodel;
    GfxDrawSpec *spec;
    Battle3dActorModel *pModelRoot;
    BattleActorDrawState *bodyPtr;
    int subIndex;
    int colorPacked;
    int val;

    if (Gfx_IsDrawEnabled() == 0) {
        return;
    }
    if (((unsigned char)g_BattleActorRecords[actorIndex].drawFlags & 4) == 0) {
        SetDrawColorRGB((unsigned char)g_BattleActorRecords[actorIndex].tintR, (unsigned char)g_BattleActorRecords[actorIndex].tintG, (unsigned char)g_BattleActorRecords[actorIndex].tintB);
        subIndex = 0;
        pModelRoot = (Battle3dActorModel *)(g_BattleActorRecords[actorIndex].model);
        if (pModelRoot != 0) {
            submodel = (PolygonModel *)(((int *)(pModelRoot->partTable))[subIndex]);
            if (submodel != 0) {
                spec = &specBuffer.spec;
                Gfx_InitDrawSpec(0, spec);
                colorPacked = GetDrawColorPacked();
                spec->packedColor = colorPacked;
                val = Battle3d_FadeLevelToAlpha(g_BattleActorRecords[actorIndex].tintPhase);
                spec->alpha = val;
                if ((g_BattleActorRecords[actorIndex].fadeFlags & 8) != 0) {
                    if ((unsigned char)g_BattleActorRecords[actorIndex].tintR != 0 || ((unsigned char)g_BattleActorRecords[actorIndex].tintG != 0 || (unsigned char)g_BattleActorRecords[actorIndex].tintB != 0)) {
                        spec->kind = 9;
                    } else {
                        spec->kind = 0xA;
                    }
                } else if ((unsigned char)g_BattleActorRecords[actorIndex].tintR != 0 || ((unsigned char)g_BattleActorRecords[actorIndex].tintG != 0 || (unsigned char)g_BattleActorRecords[actorIndex].tintB != 0)) {
                    spec->kind = 9;
                } else {
                    spec->kind = 0xA;
                }
                Gfx_ApplyDrawSpecToModel(spec, submodel);
                bodyPtr = (BattleActorDrawState *)((int)pModelRoot + 0x20);
                bodyPtr->boneWorldPosX = 0;
                BattleModel_DrawAnimFrameOnSubSkeleton(g_BattleActorRecords[actorIndex].useSecondAngleTable, g_BattleActorRecords[actorIndex].animFrame + 0x10, g_BattleActorRecords[actorIndex].animBank, subIndex, pModelRoot);
            }
        }
    }
}

// Empty node-setup hook; performs no work.
// FUNCTION: FF7 0x0043097c
void __cdecl Battle_EmptyNodeSetupStub(short actorSlot, int nodeId)
{
}

// Clears the actor node work region, selects a root matrix from stage type two and variant or the
// actor-three exception, parents two actor node matrices in sequence, and calls
// Battle_ClearActorNodeTranslation with mode.
// FUNCTION: FF7 0x00430981
void __cdecl Battle_InitActorNodeChain(short actorIndex, int mode, int variant)
{
    BattleStageDescriptor *stage;

    stage = (BattleStageDescriptor *)Battle3d_GetStageDescriptor(g_pBattle3dScene);
    ZeroFillMemory32(28, &g_BattleActorRecords[actorIndex].deathActive);
    if (stage->stageType == BATTLE_STAGE_TYPE_SPLIT_SCROLL) {
        if (variant == 0) {
            Battle_SetNodeParentMatrix((void *)g_BattleStageNodes[0].matrix, &g_BattleActorRecords[actorIndex].root);
        } else {
            Battle_SetNodeParentMatrix((void *)g_BattleStageNodes[1].matrix, &g_BattleActorRecords[actorIndex].root);
        }
    } else if (actorIndex == 3) {
        Battle_SetNodeParentMatrix(&g_BattleCameraTransform, &g_BattleActorRecords[actorIndex].root);
    } else {
        Battle_SetNodeParentMatrix((void *)g_BattleStageNodes[0].matrix, &g_BattleActorRecords[actorIndex].root);
    }
    Battle_SetNodeParentMatrix(&g_BattleActorRecords[actorIndex].root, g_BattleActorRecords[actorIndex].nodes);
    Battle_ClearActorNodeTranslation(&g_BattleActorRecords[actorIndex].boneCount, mode, (short)actorIndex);
}

// Stores parentMatrix in the 32-bit field at node+0x30.
// FUNCTION: FF7 0x00430a99
void __cdecl Battle_SetNodeParentMatrix(void *parentMatrix, Battle3dNode *node)
{
    *(int *)((char *)node + 0x30) = (int)parentMatrix;
}

// Unless the current sequence camera word is -4, stores it as the active camera, clears
// g_BattleScriptBusy, sets eight control bytes to 255 and resets auxiliary tasks; updates g_BattleVariationIndex
// from the low two g_Battle3dFrameTick bits only when neither value is three.
// FUNCTION: FF7 0x00430aa7
void __cdecl Battle_ApplySequenceCamera(void)
{
    int val;

    if (g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].targetIndex != -4) {
        g_BattleCameraTargetIndex = g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].targetIndex;
        g_BattleScriptBusy = 0;
        g_BattleCameraFocusChannels[3].scriptOffset = 0xFF;
        g_BattleCameraFocusChannels[2].scriptOffset = g_BattleCameraFocusChannels[3].scriptOffset;
        g_BattleCameraFocusChannels[1].scriptOffset = g_BattleCameraFocusChannels[2].scriptOffset;
        g_BattleCameraFocusChannels[0].scriptOffset = g_BattleCameraFocusChannels[1].scriptOffset;
        g_BattleCameraEyeChannels[3].scriptOffset = g_BattleCameraFocusChannels[0].scriptOffset;
        g_BattleCameraEyeChannels[2].scriptOffset = g_BattleCameraEyeChannels[3].scriptOffset;
        g_BattleCameraEyeChannels[1].scriptOffset = g_BattleCameraEyeChannels[2].scriptOffset;
        g_BattleCameraEyeChannels[0].scriptOffset = g_BattleCameraEyeChannels[1].scriptOffset;
        Battle_ResetAuxTaskSlots();
        if (g_BattleVariationIndex != 3) {
            val = g_Battle3dFrameTick & 3;
            if (val != 3) {
                g_BattleVariationIndex = val;
            }
        }
    }
}

// Builds the matrix of the node at node from a camera. Gte_BuildLookAtMatrix makes the look-at
// matrix in outMatrix from the eye, lookAt and the vertex offset
// g_BattleOverlayVertexOffset. RotMatrix applies the node's rotation at +0x1E, and
// SetMatrixTranslation takes its translation from +0x26. The node matrix is concatenated into
// outMatrix with ConcatMatrixInPlace, which is then installed with SetRotMatrix and SetTransMatrix.
// RotTransPoint transforms the node's +0x26 point into +0x12, and its flag output goes to a
// discarded local. Finally Gte_ScaleAndInstallMatrix scales and installs the node matrix with
// g_BattleOverlay_Scale.
// FUNCTION: FF7 0x00430b7b
void __cdecl Battle3d_BuildNodeMatrixFromCamera(Battle3dNode *node, PsxMatrix *outMatrix, Vec3sPacked *eye, Vec3sPacked *lookAt)
{
    int flags;

    Gte_BuildLookAtMatrix(outMatrix, (SVECTOR *)eye, (SVECTOR *)lookAt, &g_BattleOverlayVertexOffset);
    RotMatrix(&node->rot, node);
    SetMatrixTranslation(node, &node->pos);
    ConcatMatrixInPlace(outMatrix, node);
    SetRotMatrix(outMatrix);
    SetTransMatrix(outMatrix);
    RotTransPoint(&node->pos, node->translation, &flags);
    Gte_ScaleAndInstallMatrix(node, &g_BattleOverlay_Scale);
}

// Requests the battle-won end music. When bit 5 (0x20) of g_BattleScene.flags is clear it polls the
// magic-effect loader thread, sets the pending track g_BattlePendingMusicTrack to 0x2E and calls
// Battle_StopMusicAndPlayPending; otherwise it only clears g_BattleDefeatMusicPending and g_BattleMenuGraphicsPathBuffer.victoryMusicPending.
// FUNCTION: FF7 0x00430c15
void __cdecl Battle_RequestEndMusicWin(void)
{
    if ((g_BattleScene.flags & 0x20) == 0) {
        Battle_PollMagicEffectThreadDefault();
        g_BattlePendingMusicTrack = 0x2E;
        Battle_StopMusicAndPlayPending();
    } else {
        g_BattleDefeatMusicPending = 0;
        g_BattleMenuGraphicsPathBuffer.victoryMusicPending = 0;
    }
}

// Requests the battle-lost end music: polls the magic-effect loader thread, sets the pending track
// g_BattlePendingMusicTrack to 0x3A and calls Battle_StopMusicAndPlayPending.
// FUNCTION: FF7 0x00430c51
void __cdecl Battle_RequestEndMusicLose(void)
{
    Battle_PollMagicEffectThreadDefault();
    g_BattlePendingMusicTrack = 0x3A;
    Battle_StopMusicAndPlayPending();
}

// Clears g_BattleDefeatMusicPending and g_BattleMenuGraphicsPathBuffer.victoryMusicPending, then issues SoundDispatchCommand(0x10, g_BattlePendingMusicTrack, 0, 0,
// 0, 0, 0, 0, 0) to play the track number left in g_BattlePendingMusicTrack by the caller.
// FUNCTION: FF7 0x00430c6a
void __cdecl Battle_StopMusicAndPlayPending(void)
{
    g_BattleDefeatMusicPending = 0;
    g_BattleMenuGraphicsPathBuffer.victoryMusicPending = 0;
    SoundDispatchCommand(SOUND_MUSIC_PLAY, g_BattlePendingMusicTrack, 0, 0, 0, 0, 0, 0, 0);
}

// Calls SfxSetChannelVolume with volume 0x7f and channel 1.
// FUNCTION: FF7 0x00430c9f
void __cdecl Battle_RestoreSfxMasterVolume(void)
{
    SfxSetChannelVolume(0x7F, 1);
}

// Calls Battle_RestoreSfxMasterVolume, then SfxStopAll; no arguments or direct global accesses.
// FUNCTION: FF7 0x00430cb0
void __cdecl Battle_StopAllSfxAndRestoreVolume(void)
{
    Battle_RestoreSfxMasterVolume();
    SfxStopAll();
}

// Issues one sound command through SoundDispatchCommand with
// opcode 0xC1, first parameter 0x12C (300) and the remaining seven parameters zero. That call is
// the entire body; no global is read or written here and the return value is discarded.
// FUNCTION: FF7 0x00430cbf
void __cdecl Battle_FadeOutMusic(void)
{
    SoundDispatchCommand(SOUND_MUSIC_VOLUME_TRANS, 0x12C, 0, 0, 0, 0, 0, 0, 0);
}

// Masks sfxId to its low 16 bits and forwards it to SfxPreloadAndPlayOnChannel5, discarding any
// return value.
// FUNCTION: FF7 0x00430ce4
void __cdecl Battle_PreloadAndPlaySfx(int sfxId)
{
    SfxPreloadAndPlayOnChannel5(sfxId & 0xFFFF);
}

// Calls SfxPlayOnChannel(0x40, sfxId & 0xffff, 4), discarding any return value.
// FUNCTION: FF7 0x00430cfa
void __cdecl Battle_PlaySfxCentered(int sfxId)
{
    SfxPlayOnChannel(0x40, sfxId & 0xFFFF, 4);
}

// Calls SfxPlayUpToFour(0x40, sfxId & 0xffff, 0, 0, 0), discarding any return value.
// FUNCTION: FF7 0x00430d14
void __cdecl Battle_PlaySfxCenteredMulti(int sfxId)
{
    SfxPlayUpToFour(0x40, sfxId & 0xFFFF, 0, 0, 0);
}

// Plays sfxId panned to actor: pan = its g_BattleActorScreenPositions x / 5 * 2 & 0x7F,
// or fixedPan's low byte when actor is -1. SfxPlayUpToFour's first argument is the pan (0x40,
// which Battle_PlaySfxCenteredMulti passes, is the centre).
// FUNCTION: FF7 0x00430d32
void __cdecl Battle_PlaySfxPannedToActor(short sfxId, short actor, int fixedPan)
{
    unsigned char volume; /* the pan SfxPlayUpToFour takes first */

    if (actor == -1) {
        volume = fixedPan;
    } else {
        volume = g_BattleActorScreenPositions[actor].prevX / 5 << 1 & 0x7F;
    }
    /* SfxPlayUpToFour takes an int pan; this call pushes its low byte, as SfxPlayOnChannel takes it. */
    ((void(__cdecl *)(char, int, int, int, int))SfxPlayUpToFour)((char)volume, *(int *)&sfxId & 0xFFFF, 0, 0, 0);
}

// Sets the first sequence step's actionType to BATTLE_SEQ_STEP_ACTION; when its actorIndex is zero, sets its animId to
// 0x1D, copies g_BattleOverrideStepMode and g_BattleOverrideStepLevel into its stepFlags and
// effectId, and sets its targetIndex to 0xFFFF.
// FUNCTION: FF7 0x00430d83
void __cdecl Battle_ForceEffectOverrideStep(void)
{
    g_BattleSequenceBuffer.steps[0].actionType = BATTLE_SEQ_STEP_ACTION;
    if ((g_BattleSequenceBuffer.steps[0].actorIndex) == 0) {
        g_BattleSequenceBuffer.steps[0].animId = 0x1D;
        g_BattleSequenceBuffer.steps[0].stepFlags = g_BattleOverrideStepMode;
        g_BattleSequenceBuffer.steps[0].effectId = g_BattleOverrideStepLevel;
        g_BattleSequenceBuffer.steps[0].targetIndex = 0xFFFF;
    }
}
} /* extern "C" */
