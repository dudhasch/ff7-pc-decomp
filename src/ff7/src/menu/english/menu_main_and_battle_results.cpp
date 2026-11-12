// src/ff7/src/menu/english/menu_main_and_battle_results.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The battle result screens at both UI scales (gil and item transfer, EXP, level-up,
//   materia and limit panels), with the menu popup message and a set of savemap and kernel lookup
//   helpers the menu shares.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/enums/battle.h"
#include "lib/types/mem.h"
#include "ff7/types/menu.h"
#include "lib/codegen.h"
#include "lib/types/graphics.h"
#include "ff7/api/battle.h"
#include "ff7/api/field.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"
#include "ff7/api/wm.h"
#include "ff7/api/main.h"
#include "lib/api/file.h"
#include "lib/api/mem.h"
#include "ff7/enums/main.h"
#include "lib/enums/file.h"
#include "lib/enums/graphics.h"
#include "lib/enums/sound.h"
#include "ff7/enums/menu.h"
#include "lib/crt.h"
#include "ff7/types/battle.h"
#include "ff7/savemap.h"
#include "ff7/types/battle_actors.h"
#include "ff7/field.h"
extern "C" {

// clang-format off
struct EnglishWindowFrame;
struct MenuGauge;
// clang-format on

// clang-format off
// The menu's 23 FF7-encoded 20-byte label records: the eleven main-menu commands, which the code
// indexes by command number (n * 0x14 + table), then BEGINNER, USO, USO, TIME, GIL, NEXT LEVEL, LIMIT
// LEVEL, TUTORIAL, UNDER, LEVEL UP, FURY, SADNESS, which it reads by enum MenuMainLabel. A record at
// 4 mod 8 cannot be a definition of its own, so the table is one definition.
// Text: "Item" "Magic" "Materia" "Equip" "Status" "Order" "Limit" "Config" "PHS" "Save" "Quit"
//       "Beginner" "Uso" "Uso" "Time" "Gil" "next level" "Limit level" "Tutorial" "Under"
//       "LEVEL UP" "Fury" "Sadness"
// GLOBAL: FF7 0x0091a8c0
struct GfxMenuLabelTable g_MenuMainLabels = {{0x29, 0x54, 0x45, 0x4d, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x2d, 0x41, 0x47, 0x49, 0x43, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x2d, 0x41, 0x54, 0x45, 0x52, 0x49, 0x41, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x25, 0x51, 0x55, 0x49, 0x50, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x33, 0x54, 0x41, 0x54, 0x55, 0x53, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x2f, 0x52, 0x44, 0x45, 0x52, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x2c, 0x49, 0x4d, 0x49, 0x54, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x23, 0x4f, 0x4e, 0x46, 0x49, 0x47, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x30, 0x28, 0x33, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x33, 0x41, 0x56, 0x45, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x31, 0x55, 0x49, 0x54, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x22, 0x45, 0x47, 0x49, 0x4e, 0x4e, 0x45, 0x52, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x35, 0x53, 0x4f, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x35, 0x53, 0x4f, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0x34, {0x49, 0x4d, 0x45, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0x27, {0x49, 0x4c, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0x4e, {0x45, 0x58, 0x54, 0, 0x4c, 0x45, 0x56, 0x45, 0x4c, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0x2c, {0x49, 0x4d, 0x49, 0x54, 0, 0x4c, 0x45, 0x56, 0x45, 0x4c, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0x34, 0x55, 0x54, 0x4f, 0x52, 0x49, 0x41, 0x4c, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x35, 0x4e, 0x44, 0x45, 0x52, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0x2c, {0x25, 0x36, 0x25, 0x2c, 0, 0x35, 0x30, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x26, 0x55, 0x52, 0x59, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x33, 0x41, 0x44, 0x4e, 0x45, 0x53, 0x53, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x0091aa8c
int g_TextSubsystemReady = 1;
// The frames per second Menu_ModuleEnter divides the frame time by; an object, because the literal 60.0
// would make VC5 emit a constant of its own.
// GLOBAL: FF7 0x0091aa90
double g_MenuTargetFps = 60.0;
// g_MenuMateriaSortPriority is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0x91aa98..0x91aaf8 are aliases.
// GLOBAL: FF7 0x0091aa98
unsigned char g_MenuMateriaSortPriority[96] = {0x1d, 0x1e, 0x21, 0x22, 0x23, 0x45, 0x2b, 0x24, 0x20, 0x26, 0x27, 0x28, 0x54, 0x2a, 0x46, 0x47, 0x1c, 0x04, 0x3f, 0x55, 0x56, 0x41, 0x09, 0x1f, 0x53, 0x4e, 0x30, 0x32, 0x33, 0x36, 0x38, 0x39, 0x3b, 0x3c, 0x3d, 0x50, 0x0f, 0x10, 0x05, 0x35, 0x3e, 0x2d, 0x3a, 0x4f, 0x48, 0x01, 0x02, 0x03, 0x59, 0x0a, 0x0b, 0x12, 0x0c, 0x0d, 0x0e, 0x13, 0x14, 0x15, 0x16, 0x17, 0x11, 0x1a, 0x18, 0x08, 0x37, 0x19, 0x06, 0x07, 0x1b, 0x40, 0x4b, 0x4c, 0x4d, 0x58, 0x25, 0x29, 0x2c, 0x2e, 0x2f, 0x31, 0x34, 0x42, 0x43, 0x44, 0x49, 0x4a, 0x51, 0x52, 0x57, 0x5a, 0x5b, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0091aaf8
char g_MenuCommandScreenIdMap[12] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x00};
// GLOBAL: FF7 0x0091ab04
int g_MenuCommandWindowAnimStep = 16;
// g_MenuColorFormatBitmasks is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0x91ab08..0x91ab10 are aliases.
// GLOBAL: FF7 0x0091ab08
int g_MenuColorFormatBitmasks[2] = {0x90000080, 0x90800080};
// GLOBAL: FF7 0x0091ab10
int g_MenuColorFormatMask = (int)0x90800000;
unsigned char g_rawData_0091ab14[4] = {0x00, 0x40, 0x80, 0x90};
unsigned char g_rawData_0091ab18[24] = {0x00, 0x80, 0x80, 0x90, 0x00, 0x80, 0x40, 0x90, 0x00, 0x80, 0x00, 0x90, 0x80, 0x80, 0x00, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0091ab30
int g_MenuScreenFadeColors[4] = {(int)0x807000b0, (int)0x80700050, (int)0x80700080, (int)0x80700020};
// The menu screens' init callbacks, indexed by the screen number; Menu_DrawCommandWindow2x/1x call the
// selected screen's entry with 0 when its command window has collapsed.
typedef void (__cdecl *MenuScreenInitFn)(int arg);
// GLOBAL: FF7 0x0091ab40
MenuScreenInitFn g_MenuScreenInitCallbacks[22] = {(MenuScreenInitFn)Menu_MainScreenInit, Menu_ItemScreenInit, (MenuScreenInitFn)Menu_MagicScreenInit, Menu_MateriaScreenInit, Menu_EquipScreenInit, (MenuScreenInitFn)Menu_StatusScreenInit, Menu_StubScreen6Init, Menu_LimitScreenInit, (MenuScreenInitFn)Menu_ConfigScreenInit, Menu_PartyChangeInit, Menu_SaveScreenInit, (MenuScreenInitFn)Menu_QuitScreenInit, Menu_StubScreen12Init, Menu_LimitScreenInit, Menu_ItemScreenInit, Menu_EquipScreenInit, Menu_ItemScreenInit, Menu_ItemScreenInit, Menu_ItemScreenInit, Menu_ItemScreenInit, Menu_ItemScreenInit, 0};
// The menu screens' frame callbacks, indexed by the screen number; Menu_RunTopLevelFrame calls the
// current screen's entry with the frame count.
// GLOBAL: FF7 0x0091ab98
int g_MenuScreenFrameCallbacks[22] = {(int)&Menu_DrawMainScreen, (int)&Menu_ItemScreenFrame, (int)&Menu_MagicScreenFrame, (int)&Menu_MateriaScreenFrame, (int)&Menu_EquipScreenFrame, (int)&Menu_StatusScreenFrame, (int)&Menu_StubScreen6Frame, (int)&Menu_LimitScreenFrame, (int)&Menu_ConfigScreenFrame, (int)&Menu_PhsScreenFrame, (int)&Menu_SaveScreenTick, (int)&Menu_QuitScreenFrame, (int)&Menu_StubScreen12Frame, (int)&Menu_ItemScreenFrame, (int)&Menu_ItemScreenFrame, (int)&Menu_EquipScreenFrame, (int)&Menu_ItemScreenFrame, (int)&Menu_ItemScreenFrame, (int)&Menu_ItemScreenFrame, (int)&Menu_ItemScreenFrame, (int)&Menu_ItemScreenFrame, 0};
// g_MenuMateriaGroupColorMap is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0x91abf0..0x91abf8 are aliases.
// GLOBAL: FF7 0x0091abf0
unsigned char g_MenuMateriaGroupColorMap[8] = {0x09, 0x05, 0x08, 0x04, 0x0a, 0x00, 0x00, 0x00};
// g_MenuMateriaTypeGroupMap is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0x91abf8..0x91ac08 are aliases.
// GLOBAL: FF7 0x0091abf8
unsigned char g_MenuMateriaTypeGroupMap[16] = {0x00, 0x00, 0x02, 0x02, 0x00, 0x01, 0x02, 0x02, 0x02, 0x03, 0x03, 0x04, 0x04, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0091ac08
int g_MenuPlayTimeStep36000 = 36000;
// GLOBAL: FF7 0x0091ac0c
int g_MenuPlayTimeStep3600 = 3600;
// GLOBAL: FF7 0x0091ac10
int g_MenuPlayTimeStep600 = 600;
// GLOBAL: FF7 0x0091ac14
int g_MenuPlayTimeStep60 = 60;
// GLOBAL: FF7 0x0091ac18
int g_MenuPlayTimeStep10 = 10;
// GLOBAL: FF7 0x0091ac1c
int g_MenuPlayTimeStep1 = 1;
// GLOBAL: FF7 0x0091ac20
char g_MenuPopupTextColor = 2;
// GLOBAL: FF7 0x0091ac24
short g_MenuPopupX = 180;
// GLOBAL: FF7 0x0091ac28
short g_MenuPopupY = 104;
// The HP glyph label the status panels draw (three FF7 glyphs and the FF terminator's 0xFF), with its zero
// tail up to the next label.
// GLOBAL: FF7 0x0091ac30
unsigned char g_MenuHpGlyphLabel[8] = {0xbb, 0xc3, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00};
// The MP glyph label the status panels draw (three FF7 glyphs and the FF terminator's 0xFF), with its zero
// tail up to the next label.
// GLOBAL: FF7 0x0091ac38
unsigned char g_MenuMpGlyphLabel[8] = {0xc0, 0xc3, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00};
// The LV glyph label the status panels draw (three FF7 glyphs and the FF terminator's 0xFF), with its zero
// tail up to the next label.
// GLOBAL: FF7 0x0091ac40
unsigned char g_MenuCharacterLevelLabel[8] = {0xbf, 0xc9, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00};
// The battle result screen's layout table: the arrow tops and the row base (+ 0x43 per row) of the 2x screen,
// then the same for the 1x screen (+ 0x22 per row), and five zero bytes.
// GLOBAL: FF7 0x0091ac48
struct BattleResultScreenLayout g_MenuStatusPanelLayout = {0x85, 0xb2, 0x1b1, {0}, 0x43, 0x59, 0xd9, {0}};

// GLOBAL: FF7 0x0091ac58
int g_MenuCharSlotIndexMapping[8] = {0, 1, 2, 4, 5, 6, 7, 8};
unsigned char g_rawData_0091ac78[4] = {0x54, 0x00, 0x00, 0x00};

unsigned char g_bssPad_00dc1100[8] = {0};
// GLOBAL: FF7 0x00dc1108
int g_MenuFrameClearFlag = 0;
// GLOBAL: FF7 0x00dc110c
char g_MenuCursorPartySlot = 0;
// GLOBAL: FF7 0x00dc1110
Timestamp64 g_MenuPrevFrameTimestamp = {0};
unsigned char g_bssPad_00dc1118[4] = {0};
// GLOBAL: FF7 0x00dc111c
unsigned short g_MenuVisibleMask = 0;
// GLOBAL: FF7 0x00dc1120
int g_MenuCommandRow = 0;
// GLOBAL: FF7 0x00dc1124
int g_MenuPartyLimitRefreshNeeded = 0;
// GLOBAL: FF7 0x00dc1128
int g_MenuResultGilItemsEnabled = 0;
// GLOBAL: FF7 0x00dc112c
short g_MenuTransitionTimer = 0;
// GLOBAL: FF7 0x00dc1130
unsigned short g_MenuLockedMask = 0;
// GLOBAL: FF7 0x00dc1134
unsigned short g_SaveSlotPresentMask = 0;
// GLOBAL: FF7 0x00dc1138
int g_MenuTopLevelFrameCount = 0;
// GLOBAL: FF7 0x00dc113c
unsigned char g_MenuNewMateriaReadIndex = 0;
// GLOBAL: FF7 0x00dc1140
Timestamp64 g_MenuFrameTimestamp = {0};
// GLOBAL: FF7 0x00dc1148
unsigned char g_MenuNewMateriaCount = 0;
unsigned char g_bssPad_00dc114c[4] = {0};
// GLOBAL: FF7 0x00dc1150
struct MenuMainCursorSet g_MenuMainCursors = {0};
unsigned char g_bssPad_00dc11f8[8] = {0};
// GLOBAL: FF7 0x00dc1200
int g_MenuResultItemRow = 0;
unsigned char g_bssPad_00dc1204[4] = {0};
// GLOBAL: FF7 0x00dc1208
int g_MenuSavedCommandRow = 0;
// GLOBAL: FF7 0x00dc120c
unsigned char g_MenuNewMateriaId = 0;
// GLOBAL: FF7 0x00dc1210
int g_MenuModuleId = 0;
// GLOBAL: FF7 0x00dc1214
unsigned char *g_MenuTutorialWindowTextPtr = 0;
// GLOBAL: FF7 0x00dc1218
short g_MenuResultFadeStep = 0;
// GLOBAL: FF7 0x00dc121c
unsigned char g_MenuNewMateriaDelay = 0;
// GLOBAL: FF7 0x00dc1220
int g_MenuCommandOpenScale = 0;
// GLOBAL: FF7 0x00dc1224
short g_MenuTransitionStep = 0;
// g_BattleAcquiredMateriaIds is indexed by the code as an array. The span runs to the next named definition
// of the TU; the TU's pieces inside 0xdc1228..0xdc1258 are aliases.
// GLOBAL: FF7 0x00dc1228
unsigned char g_BattleAcquiredMateriaIds[48] = {0};
// GLOBAL: FF7 0x00dc1258
Timestamp64 g_MenuTickResidue = {0};
/* Contiguous 28-byte GameModule callback block passed to GetActiveModule / RequestModuleSwitch;
 * each callback slot carries its own GLOBAL marker because Menu_ModuleFrame writes them directly. */
// GLOBAL: FF7 0x00dc1260
GameModuleCallback g_MenuExitModuleInitFn = 0;
unsigned char g_bssPad_00dc1264[4] = {0};
// GLOBAL: FF7 0x00dc1268
GameModuleCallback g_MenuExitModuleEnterFn = 0;
// GLOBAL: FF7 0x00dc126c
GameModuleCallback g_MenuExitModuleExitFn = 0;
// GLOBAL: FF7 0x00dc1270
GameModuleCallback g_MenuExitModuleFrameFn = 0;
// GLOBAL: FF7 0x00dc1274
GameModuleMessageCallback g_MenuExitModuleMouseFn = 0;
// GLOBAL: FF7 0x00dc1278
GameModuleMessageCallback g_MenuExitModuleKeyFn = 0;
// GLOBAL: FF7 0x00dc127c
short g_MenuResultFadeLevel = 0;
// GLOBAL: FF7 0x00dc1280
int g_MenuCommandWindowHeight = 0;
// GLOBAL: FF7 0x00dc1284
unsigned char g_BattleResultSavedExpActive = 0;
// GLOBAL: FF7 0x00dc1288
int g_MenuPartyMemberIndex = 0;
// GLOBAL: FF7 0x00dc128c
char g_BattleResultScreenState = 0;
// GLOBAL: FF7 0x00dc1290
int g_MenuMateriaVisible = 0;
// GLOBAL: FF7 0x00dc1294
int g_MenuPhase = 0;
// GLOBAL: FF7 0x00dc1298
int g_MenuTopLevelCloseState = 0;
unsigned char g_bssPad_00dc129c[4] = {0};
// GLOBAL: FF7 0x00dc12a0
struct EnglishExpPartySlot g_MenuExpPartySlots[3] = {0};
unsigned char g_bssPad_00dc12b4[4] = {0};
unsigned char g_bssPad_00dc12b8[4] = {0};
// GLOBAL: FF7 0x00dc12bc
char g_BattleResultGilItemsFlag = 0;
// GLOBAL: FF7 0x00dc12c0
double g_MenuFrameDuration = 0.0;
// GLOBAL: FF7 0x00dc12c8
int g_MenuRequestedNextScreen = 0;
// GLOBAL: FF7 0x00dc12cc
int g_MenuFrameThrottleFlag = 0;
// GLOBAL: FF7 0x00dc12d0
double g_MenuDuration = 0.0;
// GLOBAL: FF7 0x00dc12d8
int g_MenuFrameThrottleArmed = 0;
// GLOBAL: FF7 0x00dc12dc
int g_MenuModuleActive = 0;
// GLOBAL: FF7 0x00dc12e0
int g_MenuTitleNewGameChosen = 0;
// GLOBAL: FF7 0x00dc12e4
int g_MenuOverlayActive = 0;
// GLOBAL: FF7 0x00dc12e8
int g_MenuPreviousScreenId = 0;
// GLOBAL: FF7 0x00dc12ec
int g_MenuCurrentScreenId = 0;
// GLOBAL: FF7 0x00dc12f0
int g_MenuTopLevelActive = 0;
// GLOBAL: FF7 0x00dc12f4
int g_MenuExitRequested = 0;
// GLOBAL: FF7 0x00dc12f8
int g_MenuBattleResultFinished = 0;
unsigned char g_bssPad_00dc12fc[4] = {0};
// GLOBAL: FF7 0x00dc1300
short g_MenuBattleEndMode = 0;
// GLOBAL: FF7 0x00dc1304
char g_BattleResultExpActive = 0;
// GLOBAL: FF7 0x00dc1308
short g_BattleResultFrameStarted = 0;
// GLOBAL: FF7 0x00dc130c
int g_MenuIsSmallViewport320x240 = 0;
// GLOBAL: FF7 0x00dc1310
signed char g_MenuTutorialWindowState = 0;
// GLOBAL: FF7 0x00dc1314
int g_MenuPopupLifetime = 0;
// GLOBAL: FF7 0x00dc1318
char g_MenuPopupCenteredFlag = 0;
// GLOBAL: FF7 0x00dc131c
char g_MenuPopupStepCounter = 0;
// GLOBAL: FF7 0x00dc1320
int g_MenuOrderSwapPicked = 0;
// GLOBAL: FF7 0x00dc1324
unsigned char g_MenuMainCursorIndex = 0;
// GLOBAL: FF7 0x00dc1328
int g_MenuMainScreenInitialized = 0;
unsigned char g_bssPad_00dc132c[4] = {0};
unsigned char g_bssPad_00dc1330[8] = {0};
// GLOBAL: FF7 0x00dc1338
double g_MenuElapsedTicks = 0.0;
unsigned char g_bssPad_00dc1340[16] = {0};
// clang-format on

void __cdecl Savemap_HealMp(int slot, short amount);
void __cdecl Savemap_AddItem(int itemRef);

/*
 * Returns the popup-active flag g_MenuTutorialWindowState.
 */
// FUNCTION: FF7 0x006c4970
int __cdecl Menu_IsPopupActive(void)
{
    return g_MenuTutorialWindowState;
}

/*
 * Opens a menu popup: sets the active flag, stores the text id state, chooses a
 * lifetime of 0x14 or 40 frames from whether the engine context's frame rate
 * (`fps`) is under 30, stores textPtr in g_MenuTutorialWindowTextPtr and
 * sets g_MenuPopupStepCounter.
 */
// FUNCTION: FF7 0x006c497c
void __cdecl Menu_OpenPopupMessage(const void *textPtr, int state)
{
    g_MenuTutorialWindowState = MENU_POPUP_OPENING;
    g_MenuPopupTextColor = state;
    if ((GetEngineContext())->fps < 3e+01) {
        g_MenuPopupLifetime = 20;
    } else {
        g_MenuPopupLifetime = 40;
    }
    g_MenuTutorialWindowTextPtr = (unsigned char *)textPtr;
    g_MenuPopupStepCounter = 1;
}

/*
 * Places the popup: centered nonzero stores its position (x, y) in
 * g_MenuPopupX and g_MenuPopupY; zero closes it by clearing the active flag.
 */
// FUNCTION: FF7 0x006c49cb
void __cdecl Menu_SetPopupPlacement(int centered, int x, int y)
{
    g_MenuPopupCenteredFlag = centered;
    if (centered != 0) {
        g_MenuPopupX = x;
        g_MenuPopupY = y;
    } else {
        g_MenuTutorialWindowState = MENU_POPUP_HIDDEN;
    }
}

/*
 * Draws the popup message at g_MenuTutorialWindowTextPtr as an animation
 * keyed by g_MenuTutorialWindowState (enum MenuPopupState). OPENING grows the box
 * over g_MenuPopupStepCounter steps (a third of the text width and 17 pixels
 * per step) and shows the text at step three; SHOWN draws it until its lifetime
 * runs out or a key is pressed; CLOSING shrinks the box again.
 */
// FUNCTION: FF7 0x006c49fd
void __cdecl Menu_DrawPopupMessage2x(void)
{
    EnglishWindowFrame rect;
    int width;
    int textWidth;
    int h;
    int cx;
    int cy;

    if (g_MenuTutorialWindowState == MENU_POPUP_HIDDEN) {
        return;
    }
    textWidth = Menu_MeasureEncodedString2x(g_MenuTutorialWindowTextPtr);
    switch (g_MenuTutorialWindowState) {
    case MENU_POPUP_OPENING:
        width = textWidth / 3 * g_MenuPopupStepCounter;
        h = g_MenuPopupStepCounter * 17;
        if (g_MenuPopupCenteredFlag == 0) {
            cx = 0x12c;
            cy = 0xe5;
            Menu_SetRectShorts(&rect.x, cx - width / 2, cy - h / 2, width, h);
        } else {
            cx = g_MenuPopupX;
            cy = g_MenuPopupY;
            Menu_SetRectShorts(&rect.x, cx + (textWidth + 0x1b) / 2 - width / 2,
                          cy + 0x1a - h / 2, width, h);
        }
        g_MenuPopupStepCounter++;
        if (g_MenuPopupStepCounter == 3) {
            g_MenuPopupStepCounter = 2;
            g_MenuTutorialWindowState = MENU_POPUP_SHOWN;
        }
        break;
    case MENU_POPUP_SHOWN:
        if (g_MenuPopupCenteredFlag == 0) {
            cx = 0x12c;
            cy = 0xe5;
            Menu_DrawEncodedString2x(cx - textWidth / 2, cy - 0xe,
                              g_MenuTutorialWindowTextPtr, g_MenuPopupTextColor,
                              0.0f);
            Menu_SetRectShorts(&rect.x, cx - textWidth / 2 - 0xd, cy - 0x1b,
                          textWidth + 0x1b, 51);
        } else {
            cx = g_MenuPopupX;
            cy = g_MenuPopupY;
            Menu_DrawEncodedString2x(cx + 0xd, cy + 0xd,
                              g_MenuTutorialWindowTextPtr, g_MenuPopupTextColor,
                              0.0f);
            Menu_SetRectShorts(&rect.x, cx, cy, textWidth + 0x1b, 51);
        }
        if (g_MenuPopupLifetime != 0) {
            g_MenuPopupLifetime = g_MenuPopupLifetime - 1;
        }
        if (g_MenuPopupLifetime < 20 && 1) {
            if (g_MenuPopupCenteredFlag == 0 && g_MenuPopupLifetime == 0) {
                g_MenuTutorialWindowState = MENU_POPUP_CLOSING;
            } else if (Input_UpdateAndConsume() != 0) {
                g_MenuTutorialWindowState = MENU_POPUP_CLOSING;
            }
        }
        break;
    case MENU_POPUP_CLOSING:
        width = textWidth / 3 * g_MenuPopupStepCounter;
        h = g_MenuPopupStepCounter * 17;
        if (g_MenuPopupCenteredFlag == 0) {
            cx = 0x12c;
            cy = 0xe5;
            Menu_SetRectShorts(&rect.x, cx - width / 2, cy - h / 2, width, h);
        } else {
            cx = g_MenuPopupX;
            cy = g_MenuPopupY;
            Menu_SetRectShorts(&rect.x, cx + (textWidth + 0x1b) / 2 - width / 2,
                          cy + 0x1a - h / 2, width, h);
        }
        g_MenuPopupStepCounter--;
        if (g_MenuPopupStepCounter == 0) {
            g_MenuTutorialWindowState = MENU_POPUP_HIDDEN;
        }
        break;
    }
    Menu_SaveWindowColors();
    Menu_SetWindowColors(g_MenuScreenFadeColors);
    Menu_DrawPopupMessageBody2x(&rect, 0.0f);
    Menu_RestoreWindowColors();
}

/*
 * Menu_DrawPopupMessage2x with the 1x constants: the box grows eight pixels per
 * step, centres at (0x96, 0x73), and the margins are 0xe and 0x1a.
 */
// FUNCTION: FF7 0x006c4d6d
void __cdecl Menu_DrawPopupMessage1x(void)
{
    EnglishWindowFrame rect;
    int width;
    int textWidth;
    int h;
    int cx;
    int cy;

    if (g_MenuTutorialWindowState == MENU_POPUP_HIDDEN) {
        return;
    }
    textWidth = Menu_MeasureEncodedString1x(g_MenuTutorialWindowTextPtr);
    switch (g_MenuTutorialWindowState) {
    case MENU_POPUP_OPENING:
        width = textWidth / 3 * g_MenuPopupStepCounter;
        h = g_MenuPopupStepCounter * 8;
        if (g_MenuPopupCenteredFlag == 0) {
            cx = 0x96;
            cy = 0x73;
            Menu_SetRectShorts(&rect.x, cx - width / 2, cy - h / 2, width, h);
        } else {
            cx = g_MenuPopupX;
            cy = g_MenuPopupY;
            Menu_SetRectShorts(&rect.x, cx + (textWidth + 0xe) / 2 - width / 2,
                          cy + 0xd - h / 2, width, h);
        }
        g_MenuPopupStepCounter++;
        if (g_MenuPopupStepCounter == 3) {
            g_MenuPopupStepCounter = 2;
            g_MenuTutorialWindowState = MENU_POPUP_SHOWN;
        }
        break;
    case MENU_POPUP_SHOWN:
        if (g_MenuPopupCenteredFlag == 0) {
            cx = 0x96;
            cy = 0x73;
            Menu_DrawEncodedString1x(cx - textWidth / 2, cy - 7,
                                 g_MenuTutorialWindowTextPtr, g_MenuPopupTextColor,
                                 0.0f);
            Menu_SetRectShorts(&rect.x, cx - textWidth / 2 - 7, cy - 0xe,
                          textWidth + 0xe, 26);
        } else {
            cx = g_MenuPopupX;
            cy = g_MenuPopupY;
            Menu_DrawEncodedString1x(cx + 7, cy + 7,
                                 g_MenuTutorialWindowTextPtr, g_MenuPopupTextColor,
                                 0.0f);
            Menu_SetRectShorts(&rect.x, cx, cy, textWidth + 0xe, 26);
        }
        if (g_MenuPopupLifetime != 0) {
            g_MenuPopupLifetime = g_MenuPopupLifetime - 1;
        }
        if (g_MenuPopupLifetime < 20 && 1) {
            if (g_MenuPopupCenteredFlag == 0 && g_MenuPopupLifetime == 0) {
                g_MenuTutorialWindowState = MENU_POPUP_CLOSING;
            } else if (Input_UpdateAndConsume() != 0) {
                g_MenuTutorialWindowState = MENU_POPUP_CLOSING;
            }
        }
        break;
    case MENU_POPUP_CLOSING:
        width = textWidth / 3 * g_MenuPopupStepCounter;
        h = g_MenuPopupStepCounter * 8;
        if (g_MenuPopupCenteredFlag == 0) {
            cx = 0x96;
            cy = 0x73;
            Menu_SetRectShorts(&rect.x, cx - width / 2, cy - h / 2, width, h);
        } else {
            cx = g_MenuPopupX;
            cy = g_MenuPopupY;
            Menu_SetRectShorts(&rect.x, cx + (textWidth + 0xe) / 2 - width / 2,
                          cy + 0xd - h / 2, width, h);
        }
        g_MenuPopupStepCounter--;
        if (g_MenuPopupStepCounter == 0) {
            g_MenuTutorialWindowState = MENU_POPUP_HIDDEN;
        }
        break;
    }
    Menu_SaveWindowColors();
    Menu_SetWindowColors(g_MenuScreenFadeColors);
    Menu_DrawPopupMessageBody1x((MenuPopupBodyRect *)&rect, 0.0f);
    Menu_RestoreWindowColors();
}

/*
 * The equip mask of an item, out of whichever of the four kernel tables its id
 * falls in: items below 0x80 with a 0x1C stride, weapons to 0x100 with 0x2C,
 * armour to 0x120 with 0x24 and accessories above that with 0x10. The field is
 * 0x0A into every record, and the retail instructions fold +0x0A directly into
 * each table's base displacement.
 */
// FUNCTION: FF7 0x006c50dd
int __cdecl Kernel_GetEquipRestrictions(int itemId)
{
    if (itemId < 0x80) {
        return g_MenuKernelItemBuffer[itemId].equipRestrictions;
    }
    if (itemId < 0x100) {
        return g_KernelWeaponDataArray[(itemId - 0x80)].flagWord;
    }
    if (itemId < 0x120) {
        return g_KernelArmorDataArray[(itemId - 0x100)].flagWord;
    }
    return g_KernelAccessoryDataArray[(itemId - 0x120)].restrictions;
}

/*
 * Maps the selected party id through g_CharIdToSaveSlot, then maps that result
 * through the same table again to obtain the equipped character record.
 * MENU_EQUIP_VALUE_ATTACK reads the weapon byte from its 0x2C-stride kernel record;
 * MENU_EQUIP_VALUE_DEFENSE reads the armour byte from its 0x24-stride record; the
 * magic attack and defense kinds return 0.
 */
// FUNCTION: FF7 0x006c5154
int __cdecl Kernel_GetEquippedAttackOrDefense(int partyIndex, int kind)
{
    int result;
    int charSlot;

    charSlot = g_CharIdToSaveSlot[gSavemap.party[partyIndex]];
    switch (kind) {
    case MENU_EQUIP_VALUE_ATTACK:
        result = g_KernelWeaponDataArray[gSavemap.chars[g_CharIdToSaveSlot[charSlot]].weapon].attackBonusByte;
        break;
    case MENU_EQUIP_VALUE_DEFENSE:
        result = g_KernelArmorDataArray[gSavemap.chars[g_CharIdToSaveSlot[charSlot]].armor].physicalDefense;
        break;
    case MENU_EQUIP_VALUE_MAGIC_ATTACK:
        result = 0;
        break;
    case MENU_EQUIP_VALUE_MAGIC_DEFENSE:
        // BUG: the armour's magic defense byte is never read: this always returns 0, so the
        // armour's MDEF is left out of a party member's magic defense in battle, although the
        // Status screen shows it (the Magic Defense bug).
        result = 0;
    }
    return result;
}

/*
 * Add an equipment bonus to one of the six stat bytes of a party slot's record:
 * strength, vitality, magic, spirit, dexterity, luck. Six cases and a jump table,
 * and the frame's one slot is the switch's own temporary.
 */
// FUNCTION: FF7 0x006c51fc
void __cdecl Savemap_AddEquipStatBonus(int slot, int which,
                                       unsigned char amount)
{
    switch (which) {
    case MENU_EQUIP_STAT_STRENGTH:
        g_MenuPartyStats[slot].strength += amount;
        break;
    case MENU_EQUIP_STAT_VITALITY:
        g_MenuPartyStats[slot].vitality += amount;
        break;
    case MENU_EQUIP_STAT_MAGIC:
        g_MenuPartyStats[slot].magic += amount;
        break;
    case MENU_EQUIP_STAT_SPIRIT:
        g_MenuPartyStats[slot].spirit += amount;
        break;
    case MENU_EQUIP_STAT_DEXTERITY:
        g_MenuPartyStats[slot].dexterity += amount;
        break;
    case MENU_EQUIP_STAT_LUCK:
        g_MenuPartyStats[slot].luck += amount;
        break;
    }
}

/*
 * Or a set of element bits into one of the three defence words of a party slot's
 * stat record. Three compares and no jump table.
 */
// FUNCTION: FF7 0x006c530c
void __cdecl Savemap_AddElementDefense(int slot, int which, short bits)
{
    switch (which) {
    case MENU_ELEMENT_DEFENSE_ABSORB:
        g_MenuPartyStats[slot].absorbedElements |= bits;
        break;
    case MENU_ELEMENT_DEFENSE_NULLIFY:
        g_MenuPartyStats[slot].nullifiedElements |= bits;
        break;
    case MENU_ELEMENT_DEFENSE_HALVE:
        g_MenuPartyStats[slot].halvedElements |= bits;
        break;
    }
}

/*
 * ORs an element mask into character record charIndex's weapon attack elements
 * (g_MenuPartyStats[0].attackElements, 0x440 per record).
 */
// FUNCTION: FF7 0x006c539e
void __cdecl Savemap_AddWeaponAttackElement(int charIndex, int elementMask)
{
    g_MenuPartyStats[charIndex].attackElements |= elementMask;
}

/*
 * Returns when the bit argument is greater than 0x40; otherwise ORs 1 shifted by
 * that argument into the selected character status-attack word. Negative arguments
 * and shifts 32 through 64 are not rejected; native shift behavior requires its
 * actual instruction convention.
 */
// FUNCTION: FF7 0x006c53c7
void __cdecl Savemap_AddStatusAttackBit(int charIndex, int bit)
{
    if (bit > 0x40) {
        return;
    }
    g_MenuPartyStats[charIndex].attackStatuses |= 1 << bit;
}

/*
 * Returns when the bit argument is greater than 0x40; otherwise ORs 1 shifted by
 * that argument into the selected character status-defense word. Negative
 * arguments and shifts 32 through 64 are not rejected; native shift behavior
 * requires its actual instruction convention.
 */
// FUNCTION: FF7 0x006c53fe
void __cdecl Savemap_AddStatusDefendBit(int charIndex, int bit)
{
    if (bit > 0x40) {
        return;
    }
    g_MenuPartyStats[charIndex].defenseStatuses |= 1 << bit;
}

/*
 * ORs a whole status-defence mask into character record charIndex (g_MenuPartyStats[0].defenseStatuses, 0x440
 * per record).
 */
// FUNCTION: FF7 0x006c5435
void __cdecl Savemap_AddStatusDefendMask(int charIndex, int mask)
{
    g_MenuPartyStats[charIndex].defenseStatuses |= mask;
}

/*
 * Rebuilds the selected party member 1088-byte stat record unless the party slot
 * is 0xFF. Maps character ID to save slot, clears accumulated equipment fields,
 * copies the 44-byte weapon record and HP/MP words and applies weapon, armor and
 * optional accessory stat, element and status effects.
 */
// FUNCTION: FF7 0x006c545b
void __cdecl Savemap_BuildPartyStatsFromRecord(int partyIndex)
{
    int vitValue2Stat;
    int fortuneValue;
    KernelArmorRecord *armorPtr;
    int str;
    int dex;
    int mag;
    int spirit;
    int index;
    KernelAccessoryRecord *accessoryPtr;
    int characterIndex;

    if (gSavemap.party[partyIndex] == PARTY_CHAR_NONE) {
        return;
    }
    characterIndex = g_CharIdToSaveSlot[gSavemap.party[partyIndex]];
    g_MenuPartyStats[partyIndex].attackElements = 0;
    g_MenuPartyStats[partyIndex].halvedElements = 0;
    g_MenuPartyStats[partyIndex].nullifiedElements = 0;
    g_MenuPartyStats[partyIndex].absorbedElements = 0;
    g_MenuPartyStats[partyIndex].attackStatuses = 0;
    g_MenuPartyStats[partyIndex].defenseStatuses = 0;
    *(struct KernelWeapon *)g_MenuPartyStats[partyIndex].weapon = g_KernelWeaponDataArray[gSavemap.chars[characterIndex].weapon];
    g_MenuPartyStats[partyIndex].hp = (gSavemap.chars[characterIndex].hp);
    g_MenuPartyStats[partyIndex].mp = gSavemap.chars[characterIndex].mp;
    g_MenuPartyStats[partyIndex].maxHp = (gSavemap.chars[characterIndex].baseHp);
    g_MenuPartyStats[partyIndex].maxMp = (gSavemap.chars[characterIndex].baseMp);
    g_MenuPartyStats[partyIndex].strength = 0;
    g_MenuPartyStats[partyIndex].dexterity = 0;
    g_MenuPartyStats[partyIndex].vitality = 0;
    g_MenuPartyStats[partyIndex].spirit = 0;
    g_MenuPartyStats[partyIndex].magic = 0;
    g_MenuPartyStats[partyIndex].luck = 0;
    for (index = 0; index < 4; index++) {
        /* Retail caller prototype (int, int, int) -> void preserved for /Od argument promotion on `Savemap_AddEquipStatBonus`. */
        ((void(__cdecl *)(int, int, int))Savemap_AddEquipStatBonus)(partyIndex, g_MenuPartyStats[partyIndex].equipStatTarget[index], g_MenuPartyStats[partyIndex].equipStatAmount[index]);
    }
    Savemap_AddWeaponAttackElement(partyIndex, g_MenuPartyStats[partyIndex].elementMask);
    Savemap_AddStatusAttackBit(partyIndex, g_MenuPartyStats[partyIndex].statusAttackBits);
    armorPtr = (KernelArmorRecord *)(Kernel_GetArmorRecord(gSavemap.chars[characterIndex].armor));
    for (index = 0; index < 4; index++) {
        /* Retail caller prototype (int, int, int) -> void preserved for /Od argument promotion on `Savemap_AddEquipStatBonus`. */
        ((void(__cdecl *)(int, int, int))Savemap_AddEquipStatBonus)(partyIndex, armorPtr->equipStatTarget[index], armorPtr->equipStatAmount[index]);
    }
    /* Retail caller prototype (int, int, int) -> void preserved for /Od argument promotion on `Savemap_AddElementDefense`. */
    ((void(__cdecl *)(int, int, int))Savemap_AddElementDefense)(partyIndex, armorPtr->element, armorPtr->elementDefense);
    Savemap_AddStatusDefendBit(partyIndex, armorPtr->statusDefendBit);
    if (gSavemap.chars[characterIndex].accessory != 0xFF) {
        accessoryPtr = (KernelAccessoryRecord *)(Kernel_GetAccessoryRecord(gSavemap.chars[characterIndex].accessory));
        for (index = 0; index < 2; index++) {
            /* Retail caller prototype (int, int, int) -> void preserved for /Od argument promotion on `Savemap_AddEquipStatBonus`. */
            ((void(__cdecl *)(int, int, int))Savemap_AddEquipStatBonus)(partyIndex, accessoryPtr->equipStatTarget[index], accessoryPtr->equipStatAmount[index]);
        }
        /* Retail caller prototype (int, int, int) -> void preserved for /Od argument promotion on `Savemap_AddElementDefense`. */
        ((void(__cdecl *)(int, int, int))Savemap_AddElementDefense)(partyIndex, accessoryPtr->element, accessoryPtr->elementDefense);
        Savemap_AddStatusDefendMask(partyIndex, accessoryPtr->statusDefendMask);
    }
    str = gSavemap.chars[characterIndex].strength + gSavemap.chars[characterIndex].strengthBonus + g_MenuPartyStats[partyIndex].strength;
    dex = gSavemap.chars[characterIndex].dexterity + gSavemap.chars[characterIndex].dexterityBonus + g_MenuPartyStats[partyIndex].dexterity;
    vitValue2Stat = gSavemap.chars[characterIndex].vitality + gSavemap.chars[characterIndex].vitalityBonus + g_MenuPartyStats[partyIndex].vitality;
    spirit = gSavemap.chars[characterIndex].spirit + gSavemap.chars[characterIndex].spiritBonus + g_MenuPartyStats[partyIndex].spirit;
    mag = gSavemap.chars[characterIndex].magic + gSavemap.chars[characterIndex].magicBonus + g_MenuPartyStats[partyIndex].magic;
    fortuneValue = gSavemap.chars[characterIndex].luck + gSavemap.chars[characterIndex].luckBonus + g_MenuPartyStats[partyIndex].luck;
    if (gSavemap.chars[characterIndex].accessory + 0x120 == 0x139) {
        str += 0xF;
        dex += 0xF;
        vitValue2Stat += 0xF;
        spirit += 0xF;
        mag += 0xF;
        fortuneValue += 0xA;
    }
    if (str > 255) {
        str = 0xFF;
    }
    if (dex > 255) {
        dex = 0xFF;
    }
    if (vitValue2Stat > 255) {
        vitValue2Stat = 0xFF;
    }
    if (spirit > 255) {
        spirit = 0xFF;
    }
    if (mag > 255) {
        mag = 0xFF;
    }
    if (fortuneValue > 255) {
        fortuneValue = 0xFF;
    }
    g_MenuPartyStats[partyIndex].strength = str;
    g_MenuPartyStats[partyIndex].dexterity = dex;
    g_MenuPartyStats[partyIndex].vitality = vitValue2Stat;
    g_MenuPartyStats[partyIndex].spirit = spirit;
    g_MenuPartyStats[partyIndex].magic = mag;
    g_MenuPartyStats[partyIndex].luck = fortuneValue;
}

/*
 * Draws a double-resolution character name, level and current/maximum HP and MP
 * panel from the indexed 0x84-byte save record at the supplied origin and depth.
 * Builds packed 105-by-2 gauge records at Y offsets 66 and 88, using blue and
 * green colors and Menu_DrawResourceGauge2x.
 */
// FUNCTION: FF7 0x006c5a5a
void __cdecl Menu_DrawCharacterHpMpPanel2x(int originX, int originY, int charIndex, float depth)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[40];
        int mpColor;
        struct { unsigned char pad_to_4[0x4]; int hpColor; };
        struct { unsigned char pad_to_8[0x8]; int mpTone; };
        struct { unsigned char pad_to_C[0xC]; int hpIsZero; };
        struct { unsigned char pad_to_10[0x10]; int hpTone; };
        struct { unsigned char pad_to_14[0x14]; MenuGauge gauge; };
    } frame;
#pragma pack(pop)

    frame.gauge.x = originX + 30;
    frame.gauge.y = originY + 66;
    frame.gauge.width = 105;
    frame.gauge.height = 2;
    frame.gauge.low = gSavemap.chars[charIndex].hp;
    frame.gauge.high = gSavemap.chars[charIndex].hp;
    frame.gauge.max = gSavemap.chars[charIndex].maxHp;
    frame.gauge.direction = 0;
    frame.gauge.colour = 0x800080FF;
    Menu_DrawResourceGauge2x(&frame.gauge, depth);
    frame.gauge.x = originX + 30;
    frame.gauge.y = originY + 88;
    frame.gauge.width = 105;
    frame.gauge.height = 2;
    frame.gauge.low = gSavemap.chars[charIndex].mp;
    frame.gauge.high = gSavemap.chars[charIndex].mp;
    frame.gauge.max = gSavemap.chars[charIndex].maxMp;
    frame.gauge.direction = 0;
    frame.gauge.colour = 0x8000FF80;
    Menu_DrawResourceGauge2x(&frame.gauge, depth);
    frame.hpIsZero = gSavemap.chars[charIndex].hp == 0;
    frame.hpTone = (gSavemap.chars[charIndex].hp > gSavemap.chars[charIndex].maxHp / 4) + 6;
    frame.mpTone = ((unsigned short)gSavemap.chars[charIndex].mp > gSavemap.chars[charIndex].maxMp / 4) + 6;
    if ((gSavemap.chars[charIndex].status & BATTLE_STATUS_SADNESS) != 0) {
        Menu_DrawEncodedString2x(originX + 0x3C, originY + 0x18, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_SADNESS * 0x14, 3, depth);
    }
    if ((gSavemap.chars[charIndex].status & BATTLE_STATUS_FURY) != 0) {
        Menu_DrawEncodedString2x(originX + 0x3C, originY + 0x18, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_FURY * 0x14, 3, depth);
    }
    Menu_DrawEncodedString2x(originX, originY, gSavemap.chars[charIndex].name, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawFixedPitchString2x(originX, originY + 0x1C, g_MenuCharacterLevelLabel, 5, depth);
    Menu_DrawDecimalNumber2x(originX + 0x1E, originY + 0x1C, gSavemap.chars[charIndex].level, 2, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawFixedPitchString2x(originX, originY + 0x33, g_MenuHpGlyphLabel, 5, depth);
    if (frame.hpIsZero != 0) {
        frame.hpColor = 2;
    } else {
        frame.hpColor = frame.hpTone;
    }
    Menu_DrawDecimalNumber2x(originX + 0x1E, originY + 0x31, gSavemap.chars[charIndex].hp, 4, frame.hpColor, depth);
    Menu_DrawDecimalNumber2x(originX + 0x57, originY + 0x31, gSavemap.chars[charIndex].maxHp, 4, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawFixedPitchString2x(originX, originY + 0x49, g_MenuMpGlyphLabel, 5, depth);
    if (frame.hpIsZero != 0) {
        frame.mpColor = 2;
    } else {
        frame.mpColor = frame.mpTone;
    }
    Menu_DrawDecimalNumber2x(originX + 0x1E, originY + 0x47, (unsigned short)gSavemap.chars[charIndex].mp, 4, frame.mpColor, depth);
    Menu_DrawDecimalNumber2x(originX + 0x57, originY + 0x47, gSavemap.chars[charIndex].maxMp, 4, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawAtlasQuad2x(originX + 0x4F, originY + 0x47, 0xD8, 0, 4, 8, 1, 0, 0, depth);
    Menu_DrawAtlasQuad2x(originX + 0x4F, originY + 0x31, 0xD8, 0, 4, 8, 1, 0, 0, depth);
}

/*
 * Draws a low-resolution character name, level and current/maximum HP and MP panel
 * from the indexed 0x84-byte save record at the supplied origin and depth. Builds
 * packed 53-by-1 gauge records at Y offsets 33 and 44, using blue and green colors
 * and Menu_DrawResourceGauge1x.
 */
// FUNCTION: FF7 0x006c5e7e
void __cdecl Menu_DrawCharacterHpMpPanel1x(int originX, int originY, int charIndex, float depth)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[40];
        int mpColor;
        struct { unsigned char pad_to_4[0x4]; int hpColor; };
        struct { unsigned char pad_to_8[0x8]; int mpTone; };
        struct { unsigned char pad_to_C[0xC]; int hpIsZero; };
        struct { unsigned char pad_to_10[0x10]; int hpTone; };
        struct { unsigned char pad_to_14[0x14]; MenuGauge gauge; };
    } frame;
#pragma pack(pop)

    frame.gauge.x = originX + 15;
    frame.gauge.y = originY + 33;
    frame.gauge.width = 53;
    frame.gauge.height = 1;
    frame.gauge.low = gSavemap.chars[charIndex].hp;
    frame.gauge.high = gSavemap.chars[charIndex].hp;
    frame.gauge.max = gSavemap.chars[charIndex].maxHp;
    frame.gauge.direction = 0;
    frame.gauge.colour = 0x800080FF;
    Menu_DrawResourceGauge1x(&frame.gauge, depth);
    frame.gauge.x = originX + 15;
    frame.gauge.y = originY + 44;
    frame.gauge.width = 53;
    frame.gauge.height = 1;
    frame.gauge.low = gSavemap.chars[charIndex].mp;
    frame.gauge.high = gSavemap.chars[charIndex].mp;
    frame.gauge.max = gSavemap.chars[charIndex].maxMp;
    frame.gauge.direction = 0;
    frame.gauge.colour = 0x8000FF80;
    Menu_DrawResourceGauge1x(&frame.gauge, depth);
    frame.hpIsZero = gSavemap.chars[charIndex].hp == 0;
    frame.hpTone = (gSavemap.chars[charIndex].hp > gSavemap.chars[charIndex].maxHp / 4) + 6;
    frame.mpTone = ((unsigned short)gSavemap.chars[charIndex].mp > gSavemap.chars[charIndex].maxMp / 4) + 6;
    if ((gSavemap.chars[charIndex].status & BATTLE_STATUS_SADNESS) != 0) {
        Menu_DrawEncodedString1x(originX + 0x1E, originY + 0xC, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_SADNESS * 0x14, 3, depth);
    }
    if ((gSavemap.chars[charIndex].status & BATTLE_STATUS_FURY) != 0) {
        Menu_DrawEncodedString1x(originX + 0x1E, originY + 0xC, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_FURY * 0x14, 3, depth);
    }
    Menu_DrawEncodedString1x(originX, originY, gSavemap.chars[charIndex].name, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawFixedPitchString1x(originX, originY + 0xE, g_MenuCharacterLevelLabel, 5, depth);
    Menu_DrawDecimalNumber1x(originX + 0xF, originY + 0xE, gSavemap.chars[charIndex].level, 2, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawFixedPitchString1x(originX, originY + 0x1A, g_MenuHpGlyphLabel, 5, depth);
    if (frame.hpIsZero != 0) {
        frame.hpColor = 2;
    } else {
        frame.hpColor = frame.hpTone;
    }
    Menu_DrawDecimalNumber1x(originX + 0xF, originY + 0x19, gSavemap.chars[charIndex].hp, 4, frame.hpColor, depth);
    Menu_DrawDecimalNumber1x(originX + 0x2C, originY + 0x19, gSavemap.chars[charIndex].maxHp, 4, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawFixedPitchString1x(originX, originY + 0x25, g_MenuMpGlyphLabel, 5, depth);
    if (frame.hpIsZero != 0) {
        frame.mpColor = 2;
    } else {
        frame.mpColor = frame.mpTone;
    }
    Menu_DrawDecimalNumber1x(originX + 0xF, originY + 0x24, (unsigned short)gSavemap.chars[charIndex].mp, 4, frame.mpColor, depth);
    Menu_DrawDecimalNumber1x(originX + 0x2C, originY + 0x24, gSavemap.chars[charIndex].maxMp, 4, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawAtlasQuad1x(originX + 0x28, originY + 0x24, 216, 0, 4, 8, 1, 0, 0, depth);
    Menu_DrawAtlasQuad1x(originX + 0x28, originY + 0x19, 216, 0, 4, 8, 1, 0, 0, depth);
}

/*
 * Draws the double-resolution party stat block using signed HP/MP words from the
 * indexed 0x440-byte derived party record and the mapped 0x84-byte save record for
 * name, level and status. Emits 105-by-2 blue/green gauges at origin Y plus 66 and
 * 88.
 */
// FUNCTION: FF7 0x006c62a2
void __cdecl Menu_DrawCharStatBlock2x(int originX, int originY, int partySlot, float depth)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[44];
        int mpNumberTone;
        struct { unsigned char pad_to_4[0x4]; int hpNumberTone; };
        struct { unsigned char pad_to_8[0x8]; int saveSlot; };
        struct { unsigned char pad_to_C[0xC]; int mpTone; };
        struct { unsigned char pad_to_10[0x10]; int hpIsZero; };
        struct { unsigned char pad_to_14[0x14]; int hpTone; };
        struct { unsigned char pad_to_18[0x18]; MenuGauge gauge; };
    } frame;
#pragma pack(pop)

    frame.saveSlot = g_CharIdToSaveSlot[gSavemap.party[partySlot]];
    frame.gauge.x = originX + 30;
    frame.gauge.y = originY + 66;
    frame.gauge.width = 105;
    frame.gauge.height = 2;
    frame.gauge.low = g_MenuPartyStats[partySlot].hp;
    frame.gauge.high = g_MenuPartyStats[partySlot].hp;
    frame.gauge.max = g_MenuPartyStats[partySlot].maxHp;
    frame.gauge.direction = 0;
    frame.gauge.colour = 0x800080FF;
    Menu_DrawResourceGauge2x(&frame.gauge, depth);
    frame.gauge.x = originX + 30;
    frame.gauge.y = originY + 88;
    frame.gauge.width = 105;
    frame.gauge.height = 2;
    frame.gauge.low = g_MenuPartyStats[partySlot].mp;
    frame.gauge.high = g_MenuPartyStats[partySlot].mp;
    frame.gauge.max = g_MenuPartyStats[partySlot].maxMp;
    frame.gauge.direction = 0;
    frame.gauge.colour = 0x8000FF80;
    Menu_DrawResourceGauge2x(&frame.gauge, depth);
    frame.hpIsZero = g_MenuPartyStats[partySlot].hp == 0;
    frame.hpTone = (g_MenuPartyStats[partySlot].hp > g_MenuPartyStats[partySlot].maxHp / 4) + 6;
    frame.mpTone = (g_MenuPartyStats[partySlot].mp > g_MenuPartyStats[partySlot].maxMp / 4) + 6;
    Menu_DrawEncodedString2x(originX, originY, gSavemap.chars[frame.saveSlot].name, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawFixedPitchString2x(originX, originY + 0x1D, g_MenuCharacterLevelLabel, 5, depth);
    Menu_DrawDecimalNumber2x(originX + 0x1E, originY + 0x1D, gSavemap.chars[frame.saveSlot].level, 2, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawFixedPitchString2x(originX, originY + 0x33, g_MenuHpGlyphLabel, 5, depth);
    if (frame.hpIsZero != 0) {
        frame.hpNumberTone = 2;
    } else {
        frame.hpNumberTone = frame.hpTone;
    }
    Menu_DrawDecimalNumber2x(originX + 0x1E, originY + 0x31, g_MenuPartyStats[partySlot].hp, 4, frame.hpNumberTone, depth);
    Menu_DrawDecimalNumber2x(originX + 0x57, originY + 0x31, g_MenuPartyStats[partySlot].maxHp, 4, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawFixedPitchString2x(originX, originY + 0x49, g_MenuMpGlyphLabel, 5, depth);
    if (frame.hpIsZero != 0) {
        frame.mpNumberTone = 2;
    } else {
        frame.mpNumberTone = frame.mpTone;
    }
    Menu_DrawDecimalNumber2x(originX + 0x1E, originY + 0x47, g_MenuPartyStats[partySlot].mp, 4, frame.mpNumberTone, depth);
    Menu_DrawDecimalNumber2x(originX + 0x57, originY + 0x47, g_MenuPartyStats[partySlot].maxMp, 4, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawAtlasQuad2x(originX + 0x4F, originY + 0x47, 0xD8, 0, 4, 8, 1, 0, 0, depth);
    Menu_DrawAtlasQuad2x(originX + 0x4F, originY + 0x31, 0xD8, 0, 4, 8, 1, 0, 0, depth);
    if ((gSavemap.chars[frame.saveSlot].status & BATTLE_STATUS_SADNESS) != 0) {
        Menu_DrawEncodedString2x(originX + 0x3C, originY + 0x19, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_SADNESS * 0x14, 3, depth);
    }
    if ((gSavemap.chars[frame.saveSlot].status & BATTLE_STATUS_FURY) != 0) {
        Menu_DrawEncodedString2x(originX + 0x3C, originY + 0x19, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_FURY * 0x14, 3, depth);
    }
}

/*
 * Draws the low-resolution party stat block using signed HP/MP words from the
 * indexed 0x440-byte derived party record and the mapped 0x84-byte save record for
 * name, level and status. Emits 53-by-1 blue/green gauges at origin Y plus 33 and
 * 44.
 */
// FUNCTION: FF7 0x006c66c8
void __cdecl Menu_DrawCharStatBlock1x(int originX, int originY, int partySlot, float depth)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[44];
        int mpNumberTone;
        struct { unsigned char pad_to_4[0x4]; int hpNumberTone; };
        struct { unsigned char pad_to_8[0x8]; int saveSlot; };
        struct { unsigned char pad_to_C[0xC]; int mpTone; };
        struct { unsigned char pad_to_10[0x10]; int hpIsZero; };
        struct { unsigned char pad_to_14[0x14]; int hpTone; };
        struct { unsigned char pad_to_18[0x18]; MenuGauge gauge; };
    } frame;
#pragma pack(pop)

    frame.saveSlot = g_CharIdToSaveSlot[gSavemap.party[partySlot]];
    frame.gauge.x = originX + 15;
    frame.gauge.y = originY + 33;
    frame.gauge.width = 53;
    frame.gauge.height = 1;
    frame.gauge.low = g_MenuPartyStats[partySlot].hp;
    frame.gauge.high = g_MenuPartyStats[partySlot].hp;
    frame.gauge.max = g_MenuPartyStats[partySlot].maxHp;
    frame.gauge.direction = 0;
    frame.gauge.colour = 0x800080FF;
    Menu_DrawResourceGauge1x(&frame.gauge, depth);
    frame.gauge.x = originX + 15;
    frame.gauge.y = originY + 44;
    frame.gauge.width = 53;
    frame.gauge.height = 1;
    frame.gauge.low = g_MenuPartyStats[partySlot].mp;
    frame.gauge.high = g_MenuPartyStats[partySlot].mp;
    frame.gauge.max = g_MenuPartyStats[partySlot].maxMp;
    frame.gauge.direction = 0;
    frame.gauge.colour = 0x8000FF80;
    Menu_DrawResourceGauge1x(&frame.gauge, depth);
    frame.hpIsZero = g_MenuPartyStats[partySlot].hp == 0;
    frame.hpTone = (g_MenuPartyStats[partySlot].hp > g_MenuPartyStats[partySlot].maxHp / 4) + 6;
    frame.mpTone = (g_MenuPartyStats[partySlot].mp > g_MenuPartyStats[partySlot].maxMp / 4) + 6;
    Menu_DrawEncodedString1x(originX, originY, gSavemap.chars[frame.saveSlot].name, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawFixedPitchString1x(originX, originY + 0xF, g_MenuCharacterLevelLabel, 5, depth);
    Menu_DrawDecimalNumber1x(originX + 0xF, originY + 0xF, gSavemap.chars[frame.saveSlot].level, 2, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawFixedPitchString1x(originX, originY + 0x1A, g_MenuHpGlyphLabel, 5, depth);
    if (frame.hpIsZero != 0) {
        frame.hpNumberTone = 2;
    } else {
        frame.hpNumberTone = frame.hpTone;
    }
    Menu_DrawDecimalNumber1x(originX + 0xF, originY + 0x19, g_MenuPartyStats[partySlot].hp, 4, frame.hpNumberTone, depth);
    Menu_DrawDecimalNumber1x(originX + 0x2C, originY + 0x19, g_MenuPartyStats[partySlot].maxHp, 4, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawFixedPitchString1x(originX, originY + 0x25, g_MenuMpGlyphLabel, 5, depth);
    if (frame.hpIsZero != 0) {
        frame.mpNumberTone = 2;
    } else {
        frame.mpNumberTone = frame.mpTone;
    }
    Menu_DrawDecimalNumber1x(originX + 0xF, originY + 0x24, g_MenuPartyStats[partySlot].mp, 4, frame.mpNumberTone, depth);
    Menu_DrawDecimalNumber1x(originX + 0x2C, originY + 0x24, g_MenuPartyStats[partySlot].maxMp, 4, (frame.hpIsZero != 0 ? -5 : 0) + 7, depth);
    Menu_DrawAtlasQuad1x(originX + 0x28, originY + 0x24, 216, 0, 4, 8, 1, 0, 0, depth);
    Menu_DrawAtlasQuad1x(originX + 0x28, originY + 0x19, 216, 0, 4, 8, 1, 0, 0, depth);
    if ((gSavemap.chars[frame.saveSlot].status & BATTLE_STATUS_SADNESS) != 0) {
        Menu_DrawEncodedString1x(originX + 0x1E, originY + 0xD, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_SADNESS * 0x14, 3, depth);
    }
    if ((gSavemap.chars[frame.saveSlot].status & BATTLE_STATUS_FURY) != 0) {
        Menu_DrawEncodedString1x(originX + 0x1E, originY + 0xD, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_FURY * 0x14, 3, depth);
    }
}

/*
 * Switches the menu screen: the current id g_MenuCurrentScreenId becomes
 * the previous g_MenuPreviousScreenId and `value` becomes current.
 */
// FUNCTION: FF7 0x006c6aee
void __cdecl Menu_SetScreen(int value)
{
    g_MenuPreviousScreenId = g_MenuCurrentScreenId;
    g_MenuCurrentScreenId = value;
}

/*
 * Returns what Kernel_LookupUiText returns: Menu_DrawMateriaNameAndApStars2x hands the result
 * straight to Menu_DrawEncodedString2x as its text.
 */
// FUNCTION: FF7 0x006c6b06
unsigned char *__cdecl Kernel_LookupMateriaNameText(int value)
{
    return (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MATERIA, value, 8);
}

/*
 * Returns the item name text (UI text group 4, value) in slot 8.
 */
// FUNCTION: FF7 0x006c6b1b
unsigned char *__cdecl Kernel_LookupItemNameText(int value)
{
    return (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, value, 8);
}

/*
 * Updates the two-scale transfer/result panel and its fade. In phase two at zero
 * fade, confirming cursor position five transfers all remaining amount to the
 * total, enters phase three and reverses the fade step.
 */
// FUNCTION: FF7 0x006c6b30
void __cdecl Battle_DrawResultGilAndItemsScreen2x(void)
{
    int frameRect[2];
    int unused[3]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int itemListPtr;
    int unusedWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short arrowYPosition;
    short itemIndex;
    short arrowXPosition;
    short rowStep;
    short previousRow;

    itemListPtr = (int)g_BattleItemListBlock.slots;
    if (g_MenuBattleEndMode == MENU_BATTLE_END_GIL_ITEMS) {
        if ((BtlMenu_GetInputRepeat(INPUT_START) != 0 || BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) && g_MenuResultFadeLevel == 0 && g_MenuResultItemRow == MENU_RESULT_ITEM_ROW_CONTINUE) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
            if (g_BattleRewardTotals.gil != 0) {
                gSavemap.gil += g_BattleRewardTotals.gil;
                g_BattleRewardTotals.gil = 0;
            }
            g_MenuBattleEndMode = MENU_BATTLE_END_GIL_ITEMS_FADE_OUT;
            g_MenuResultFadeStep = 0 - g_MenuResultFadeStep;
        }
    }
    g_MenuResultFadeLevel += g_MenuResultFadeStep;
    if (g_MenuResultFadeLevel <= 0 && g_MenuBattleEndMode != MENU_BATTLE_END_GIL_ITEMS_FADE_OUT) {
        g_MenuBattleEndMode = MENU_BATTLE_END_GIL_ITEMS;
        g_MenuResultFadeLevel = 0;
        if (g_BattleResultGilItemsFlag != 0) {
            g_BattleResultGilItemsFlag = 0;
        }
        if (g_MenuResultGilItemsEnabled != 0) {
            previousRow = (short)g_MenuResultItemRow;
            if (MenuCursor_ReadRepeat(INPUT_UP, MOUSE_DIR_UP) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_MenuResultItemRow--;
                if (g_MenuResultItemRow < 0) {
                    g_MenuResultItemRow = MENU_RESULT_ITEM_ROW_CONTINUE;
                }
            } else if (MenuCursor_ReadRepeat(INPUT_DOWN, MOUSE_DIR_DOWN) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_MenuResultItemRow++;
                g_MenuResultItemRow = g_MenuResultItemRow % 6;
            }
            rowStep = g_MenuResultItemRow - previousRow;
            if (g_MenuResultItemRow != MENU_RESULT_ITEM_ROW_TAKE_ALL && g_MenuResultItemRow != MENU_RESULT_ITEM_ROW_CONTINUE) {
                while (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
                    if (g_BattleRewardItems[(g_MenuResultItemRow - 1)].itemId != -1) {
                        break;
                    }
                    g_MenuResultItemRow += rowStep;
                    if (g_MenuResultItemRow == MENU_RESULT_ITEM_ROW_TAKE_ALL || g_MenuResultItemRow == MENU_RESULT_ITEM_ROW_CONTINUE) {
                        break;
                    }
                }
            }
            if (Input_GetPressed(INPUT_START) != 0 || Input_GetPressed(INPUT_CIRCLE) != 0) {
                if (g_MenuResultItemRow == MENU_RESULT_ITEM_ROW_TAKE_ALL) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    for (itemIndex = 0; itemIndex < 4; itemIndex++) {
                        g_BattleRewardItems[itemIndex].selected = 1;
                    }
                    g_MenuResultItemRow = MENU_RESULT_ITEM_ROW_CONTINUE;
                } else if (g_MenuResultItemRow != MENU_RESULT_ITEM_ROW_TAKE_ALL && g_MenuResultItemRow != MENU_RESULT_ITEM_ROW_CONTINUE) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_BattleRewardItems[(g_MenuResultItemRow - 1)].selected ^= 1;
                }
            }
        }
        if (g_BattleRewardTotals.gil != 0) {
            g_BattleRewardTotals.gil--;
            gSavemap.gil++;
            if ((g_BattleResultScreenState & 2) != 0) {
                SfxPlayOnChannel(0x40, 0x2C, 1);
            }
        }
    } else {
        if (g_MenuResultFadeLevel > 255 && g_MenuBattleEndMode == MENU_BATTLE_END_GIL_ITEMS_FADE_OUT) {
            g_MenuResultFadeLevel = 0xFF;
            g_MenuBattleEndMode = MENU_BATTLE_END_FADED_OUT;
        }
        Menu_FillViewportQuad((char)g_MenuResultFadeLevel, 0.200000003f);
    }
    Menu_DrawAtlasQuad2x(591, 86, 0xF8, 0x10, 8, 8, 7, 0, 0, 0.400000006f);
    Menu_DrawDecimalNumber2x(470, 86, gSavemap.gil, 10, 7, 0.400000006f);
    Menu_DrawAtlasQuad2x(281, 86, 0xF8, 0x10, 8, 8, 7, 0, 0, 0.300000012f);
    Menu_DrawDecimalNumber2x(196, 86, g_BattleRewardTotals.gil, 7, 7, 0.300000012f);
    if (g_MenuResultGilItemsEnabled != 0) {
        if (g_MenuResultItemRow == MENU_RESULT_ITEM_ROW_TAKE_ALL) {
            arrowXPosition = 0;
            arrowYPosition = g_MenuStatusPanelLayout.row0ArrowTop2x;
        } else if (g_MenuResultItemRow == MENU_RESULT_ITEM_ROW_CONTINUE) {
            arrowXPosition = 0;
            arrowYPosition = (g_MenuStatusPanelLayout.row5ArrowTop2x);
        } else {
            arrowXPosition = 33;
            arrowYPosition = (g_MenuStatusPanelLayout.rowBase2x) + (g_MenuResultItemRow - 1) * 67;
        }
        Menu_DrawMoreTextArrow2x(arrowXPosition, arrowYPosition, 0.201000005f);
        for (itemIndex = 0; itemIndex < 4; itemIndex++) {
            if (g_BattleRewardItems[itemIndex].itemId != -1) {
                Menu_DrawDecimalNumber2x(305, itemIndex * 67 + 0xB2, g_BattleRewardItems[itemIndex].count, 2, (g_BattleRewardItems[itemIndex].selected != 0 ? -7 : 0) + 7, 0.200000003f);
                if (g_BattleRewardItems[itemIndex].selected != 0) {
                    Menu_DrawDecimalNumber2x(598, itemIndex * 67 + 0xB2, g_BattleRewardItems[itemIndex].count, 2, 7, 0.300000012f);
                }
            }
        }
    }
    Menu_DrawEncodedString2x(53, 81, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x18, 8), 7, 0.200000003f);
    Menu_DrawEncodedString2x(378, 81, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x19, 8), 7, 0.200000003f);
    if (g_MenuResultGilItemsEnabled != 0) {
        Menu_DrawEncodedString2x(27, 26, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x15, 8), 7, 0.200000003f);
        Menu_DrawEncodedString2x(50, 133, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x1A, 8), 7, 0.200000003f);
        Menu_DrawEncodedString2x(365, 133, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x1C, 8), 7, 0.200000003f);
        Menu_DrawEncodedString2x(50, 433, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x1B, 8), 7, 0.200000003f);
        for (itemIndex = 0; itemIndex < 4; itemIndex++) {
            if (g_BattleRewardItems[itemIndex].itemId != -1) {
                Menu_DrawEncodedString2x(95, itemIndex * 0x43 + 0xB2, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, g_BattleRewardItems[itemIndex].itemId, 8), (g_BattleRewardItems[itemIndex].selected != 0 ? -7 : 0) + 7, 0.200000003f);
                if (g_BattleRewardItems[itemIndex].selected != 0) {
                    Menu_DrawEncodedString2x(418, itemIndex * 0x43 + 0xB2, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, g_BattleRewardItems[itemIndex].itemId, 8), 7, 0.200000003f);
                }
            }
        }
    } else {
        Menu_DrawEncodedString2x(27, 26, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x16, 8), 7, 0.200000003f);
        Menu_DrawEncodedString2x(40, 133, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x1D, 8), 7, 0.200000003f);
    }
    Menu_SetRectShorts((short *)frameRect, 0, 0, 640, 69);
    Menu_DrawWindowFrame2x((EnglishWindowFrame *)frameRect, 0.300000012f);
    Menu_SetRectShorts((short *)frameRect, 0, 69, 352, 51);
    Menu_DrawWindowFrame2x((EnglishWindowFrame *)frameRect, 0.300000012f);
    Menu_SetRectShorts((short *)frameRect, 282, 69, 358, 51);
    Menu_DrawWindowFrame2x((EnglishWindowFrame *)frameRect, 0.400000006f);
    if (g_MenuResultGilItemsEnabled != 0) {
        Menu_SetRectShorts((short *)frameRect, 0, 120, 352, 360);
        Menu_DrawWindowFrame2x((EnglishWindowFrame *)frameRect, 0.201000005f);
        Menu_SetRectShorts((short *)frameRect, 282, 120, 358, 360);
        Menu_DrawWindowFrame2x((EnglishWindowFrame *)frameRect, 0.300999999f);
    } else {
        Menu_SetRectShorts((short *)frameRect, 0, 120, 640, 360);
        Menu_DrawWindowFrame2x((EnglishWindowFrame *)frameRect, 0.201000005f);
    }
}

/*
 * Updates the single-scale transfer/result panel and its fade. In phase two at
 * zero fade, confirming cursor five transfers the entire remainder to the total,
 * enters phase three and reverses the fade step.
 */
// FUNCTION: FF7 0x006c734c
void __cdecl Battle_DrawResultGilAndItemsScreen1x(void)
{
    int frameRect[2];
    int unused[3]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int itemListPtr;
    int unusedWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short arrowYPosition;
    short itemIndex;
    short arrowXPosition;
    short rowStep;
    short previousRow;

    itemListPtr = (int)g_BattleItemListBlock.slots;
    if (g_MenuBattleEndMode == MENU_BATTLE_END_GIL_ITEMS) {
        if ((BtlMenu_GetInputRepeat(INPUT_START) != 0 || BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) && g_MenuResultFadeLevel == 0 && g_MenuResultItemRow == MENU_RESULT_ITEM_ROW_CONTINUE) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
            if (g_BattleRewardTotals.gil != 0) {
                gSavemap.gil += g_BattleRewardTotals.gil;
                g_BattleRewardTotals.gil = 0;
            }
            g_MenuBattleEndMode = MENU_BATTLE_END_GIL_ITEMS_FADE_OUT;
            g_MenuResultFadeStep = 0 - g_MenuResultFadeStep;
        }
    }
    g_MenuResultFadeLevel += g_MenuResultFadeStep;
    if (g_MenuResultFadeLevel <= 0 && g_MenuBattleEndMode != MENU_BATTLE_END_GIL_ITEMS_FADE_OUT) {
        g_MenuBattleEndMode = MENU_BATTLE_END_GIL_ITEMS;
        g_MenuResultFadeLevel = 0;
        if (g_BattleResultGilItemsFlag != 0) {
            g_BattleResultGilItemsFlag = 0;
        }
        if (g_MenuResultGilItemsEnabled != 0) {
            previousRow = (short)g_MenuResultItemRow;
            if (MenuCursor_ReadRepeat(INPUT_UP, MOUSE_DIR_UP) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_MenuResultItemRow--;
                if (g_MenuResultItemRow < 0) {
                    g_MenuResultItemRow = MENU_RESULT_ITEM_ROW_CONTINUE;
                }
            } else if (MenuCursor_ReadRepeat(INPUT_DOWN, MOUSE_DIR_DOWN) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_MenuResultItemRow++;
                g_MenuResultItemRow = g_MenuResultItemRow % 6;
            }
            rowStep = g_MenuResultItemRow - previousRow;
            if (g_MenuResultItemRow != MENU_RESULT_ITEM_ROW_TAKE_ALL && g_MenuResultItemRow != MENU_RESULT_ITEM_ROW_CONTINUE) {
                while (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
                    if (g_BattleRewardItems[(g_MenuResultItemRow - 1)].itemId != -1) {
                        break;
                    }
                    g_MenuResultItemRow += rowStep;
                    if (g_MenuResultItemRow == MENU_RESULT_ITEM_ROW_TAKE_ALL || g_MenuResultItemRow == MENU_RESULT_ITEM_ROW_CONTINUE) {
                        break;
                    }
                }
            }
            if (Input_GetPressed(INPUT_START) != 0 || Input_GetPressed(INPUT_CIRCLE) != 0) {
                if (g_MenuResultItemRow == MENU_RESULT_ITEM_ROW_TAKE_ALL) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    for (itemIndex = 0; itemIndex < 4; itemIndex++) {
                        g_BattleRewardItems[itemIndex].selected = 1;
                    }
                    g_MenuResultItemRow = MENU_RESULT_ITEM_ROW_CONTINUE;
                } else if (g_MenuResultItemRow != MENU_RESULT_ITEM_ROW_TAKE_ALL && g_MenuResultItemRow != MENU_RESULT_ITEM_ROW_CONTINUE) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_BattleRewardItems[(g_MenuResultItemRow - 1)].selected ^= 1;
                }
            }
        }
        if (g_BattleRewardTotals.gil != 0) {
            g_BattleRewardTotals.gil--;
            gSavemap.gil++;
            if ((g_BattleResultScreenState & 2) != 0) {
                SfxPlayOnChannel(0x40, 0x2C, 1);
            }
        }
    } else {
        if (g_MenuResultFadeLevel > 255 && g_MenuBattleEndMode == MENU_BATTLE_END_GIL_ITEMS_FADE_OUT) {
            g_MenuResultFadeLevel = 0xFF;
            g_MenuBattleEndMode = MENU_BATTLE_END_FADED_OUT;
        }
        Menu_FillViewportQuad((char)g_MenuResultFadeLevel, 0.200000003f);
    }
    Menu_DrawAtlasQuad1x(296, 43, 248, 16, 8, 8, 7, 0, 0, 0.400000006f);
    Menu_DrawDecimalNumber1x(235, 43, gSavemap.gil, 0xA, 7, 0.400000006f);
    Menu_DrawAtlasQuad1x(141, 43, 248, 16, 8, 8, 7, 0, 0, 0.300000012f);
    Menu_DrawDecimalNumber1x(98, 43, g_BattleRewardTotals.gil, 7, 7, 0.300000012f);
    if (g_MenuResultGilItemsEnabled != 0) {
        if (g_MenuResultItemRow == MENU_RESULT_ITEM_ROW_TAKE_ALL) {
            arrowXPosition = 0;
            arrowYPosition = (g_MenuStatusPanelLayout.row0ArrowTop1x);
        } else if (g_MenuResultItemRow == MENU_RESULT_ITEM_ROW_CONTINUE) {
            arrowXPosition = 0;
            arrowYPosition = (g_MenuStatusPanelLayout.row5ArrowTop1x);
        } else {
            arrowXPosition = 17;
            arrowYPosition = (g_MenuStatusPanelLayout.rowBase1x) + (g_MenuResultItemRow - 1) * 34;
        }
        Menu_DrawMoreTextArrow1x(arrowXPosition, arrowYPosition, 0.201000005f);
        for (itemIndex = 0; itemIndex < 4; itemIndex++) {
            if (g_BattleRewardItems[itemIndex].itemId != -1) {
                Menu_DrawDecimalNumber1x(153, itemIndex * 34 + 0x59, g_BattleRewardItems[itemIndex].count, 2, (g_BattleRewardItems[itemIndex].selected != 0 ? -7 : 0) + 7, 0.200000003f);
                if (g_BattleRewardItems[itemIndex].selected != 0) {
                    Menu_DrawDecimalNumber1x(300, itemIndex * 34 + 0x59, g_BattleRewardItems[itemIndex].count, 2, 7, 0.300000012f);
                }
            }
        }
    }
    Menu_DrawEncodedString1x(27, 42, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x18, 8), 7, 0.200000003f);
    Menu_DrawEncodedString1x(189, 42, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x19, 8), 7, 0.200000003f);
    if (g_MenuResultGilItemsEnabled != 0) {
        Menu_DrawEncodedString1x(14, 13, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x15, 8), 7, 0.200000003f);
        Menu_DrawEncodedString1x(25, 67, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x1A, 8), 7, 0.200000003f);
        Menu_DrawEncodedString1x(183, 67, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x1C, 8), 7, 0.200000003f);
        Menu_DrawEncodedString1x(25, 217, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x1B, 8), 7, 0.200000003f);
        for (itemIndex = 0; itemIndex < 4; itemIndex++) {
            if (g_BattleRewardItems[itemIndex].itemId != -1) {
                Menu_DrawEncodedString1x(48, itemIndex * 0x22 + 0x59, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, g_BattleRewardItems[itemIndex].itemId, 8), (g_BattleRewardItems[itemIndex].selected != 0 ? -7 : 0) + 7, 0.200000003f);
                if (g_BattleRewardItems[itemIndex].selected != 0) {
                    Menu_DrawEncodedString1x(209, itemIndex * 0x22 + 0x59, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, g_BattleRewardItems[itemIndex].itemId, 8), 7, 0.200000003f);
                }
            }
        }
    } else {
        Menu_DrawEncodedString1x(14, 13, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x16, 8), 7, 0.200000003f);
        Menu_DrawEncodedString1x(20, 67, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x1D, 8), 7, 0.200000003f);
    }
    Menu_SetRectShorts((short *)frameRect, 0, 0, 320, 35);
    Menu_DrawWindowFrame1x((EnglishWindowFrame *)frameRect, 0.300000012f);
    Menu_SetRectShorts((short *)frameRect, 0, 35, 176, 26);
    Menu_DrawWindowFrame1x((EnglishWindowFrame *)frameRect, 0.300000012f);
    Menu_SetRectShorts((short *)frameRect, 141, 35, 179, 26);
    Menu_DrawWindowFrame1x((EnglishWindowFrame *)frameRect, 0.400000006f);
    if (g_MenuResultGilItemsEnabled != 0) {
        Menu_SetRectShorts((short *)frameRect, 0, 60, 176, 180);
        Menu_DrawWindowFrame1x((EnglishWindowFrame *)frameRect, 0.201000005f);
        Menu_SetRectShorts((short *)frameRect, 141, 60, 179, 180);
        Menu_DrawWindowFrame1x((EnglishWindowFrame *)frameRect, 0.300999999f);
    } else {
        Menu_SetRectShorts((short *)frameRect, 0, 60, 320, 180);
        Menu_DrawWindowFrame1x((EnglishWindowFrame *)frameRect, 0.201000005f);
    }
}

/*
 * Updates and draws the two-scale battle EXP/result screen, deriving sliding panel
 * positions from g_MenuTransitionTimer. When results are enabled, advances EXP
 * rows, sequences global notifications and per-character level/ability
 * notifications with timers and sounds, clearing consumed bits from one ability
 * mask while scanning another without clearing it.
 */
// FUNCTION: FF7 0x006c7b54
void __cdecl Battle_DrawResultExpScreen2x(void)
{
    struct {
        int rowColor;
        int apPanelX;
        int leveledFlags;
        short levelUpOffset;
        short gap60;
        int unused56;
        unsigned int saveSlot;
        short partyIndex;
        short gap48;
        int unused44;
        int unused40;
        EnglishWindowFrame rect;
        int expRowY;
        int rowUpdateResult;
        int materiaBoxY;
        int expPanelX;
        short textColor;
        short gap12;
        int labelX;
        int apRowY;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_BattleResultExpActive != 0) {
        frame.rowUpdateResult = Battle_UpdateExpResultRows(Input_GetPressed(INPUT_ANY));
    }
    frame.labelX = 0;
    frame.materiaBoxY = 69 - g_MenuTransitionTimer * 69 / 20;
    frame.expPanelX = 294 - g_MenuTransitionTimer * 294 / 20;
    frame.expRowY = 69;
    frame.apPanelX = g_MenuTransitionTimer * 282 / 20;
    frame.apRowY = 69;
    if (g_BattleResultExpActive != 0) {
        if (g_MenuNewMateriaDelay == 0 && g_MenuNewMateriaReadIndex < g_MenuNewMateriaCount) {
            SfxPlayOnChannel(0x40, 0x1E1, 2);
            g_MenuNewMateriaDelay = 150;
            g_MenuNewMateriaId = g_BattleAcquiredMateriaIds[g_MenuNewMateriaReadIndex];
            g_MenuNewMateriaReadIndex++;
        }
        if (g_MenuNewMateriaDelay != 0) {
            Battle_DrawResultNewMateriaBox2x(216, frame.materiaBoxY + 0x1A);
            g_MenuNewMateriaDelay--;
        }
    }
    Menu_DrawAtlasQuad2x(frame.expPanelX + 0x119, frame.expRowY + 0x13, 0xF8, 8, 8, 8, 7, 0, 0, 0.101000004f);
    Menu_DrawDecimalNumber2x(frame.expPanelX + 0xAD, frame.expRowY + 0x11, g_BattleRewardExp, 9, 7, 0.101000004f);
    Menu_DrawAtlasQuad2x(frame.apPanelX + 0x119, frame.apRowY + 0x13, 0xF8, 8, 8, 8, 7, 0, 0, 0.112999998f);
    Menu_DrawDecimalNumber2x(frame.apPanelX + 0xAD, frame.apRowY + 0x11, g_BattleRewardAp, 9, 7, 0.112999998f);
    Menu_DrawEncodedString2x(frame.labelX + 0x1B, frame.materiaBoxY + 0x1A, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x10, 8), 7, 0.100000001f);
    Menu_DrawEncodedString2x(frame.expPanelX + 0x39, frame.expRowY + 0xD, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x13, 8), 7, 0.101000004f);
    Menu_DrawEncodedString2x(frame.apPanelX + 0x78, frame.apRowY + 0xD, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x14, 8), 7, 0.112999998f);
    Menu_SetRectShorts(&frame.rect.x, frame.labelX, frame.materiaBoxY, 640, 69);
    Menu_DrawWindowFrame2x(&frame.rect, 0.101000004f);
    Menu_SetRectShorts(&frame.rect.x, frame.expPanelX, frame.expRowY, 352, 51);
    Menu_DrawWindowFrame2x(&frame.rect, 0.101000004f);
    Menu_SetRectShorts(&frame.rect.x, frame.apPanelX, frame.apRowY, 358, 51);
    Menu_DrawWindowFrame2x(&frame.rect, 0.112999998f);
    for (frame.partyIndex = 0; frame.partyIndex < 3; frame.partyIndex++) {
        *(unsigned char *)&frame.saveSlot = g_CharIdToSaveSlot[gSavemap.party[frame.partyIndex]];
        frame.levelUpOffset = (frame.partyIndex * 120 + 0x78) * g_MenuTransitionTimer / 20;
        if (gSavemap.party[frame.partyIndex] != PARTY_CHAR_NONE && g_BattleRewardTotals.party[frame.partyIndex].displayedLevel != 0) {
            if (g_MenuExpPartySlots[frame.partyIndex].displayedLevel != g_BattleRewardTotals.party[frame.partyIndex].displayedLevel) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_HEAL);
                g_MenuExpPartySlots[frame.partyIndex].countdown = 80;
            }
            if (g_MenuExpPartySlots[frame.partyIndex].countdown != 0) {
                Battle_DrawResultLevelUpBox2x(58, frame.levelUpOffset + 0x40);
                g_MenuExpPartySlots[frame.partyIndex].countdown--;
            }
            g_MenuExpPartySlots[frame.partyIndex].displayedLevel = g_BattleRewardTotals.party[frame.partyIndex].displayedLevel;
            if (g_BattleResultExpActive != 0) {
                if (g_MenuExpPartySlots[frame.partyIndex].timer == 0) {
                    while (g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex != 0x10) {
                        if (((unsigned short)g_MateriaLevelUpSlotMask[frame.partyIndex] & 1 << g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex) == 0) {
                            g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex++;
                            continue;
                        }
                        SfxPlayOnChannel(0x40, 0x1E1, 2);
                        g_MenuExpPartySlots[frame.partyIndex].timer = 80;
                        g_MateriaLevelUpSlotMask[frame.partyIndex] &= (short)~(1 << g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex);
                        g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex++;
                        break;
                    }
                }
                if (g_MenuExpPartySlots[frame.partyIndex].holdFrames == 0 && g_MenuExpPartySlots[frame.partyIndex].timer == 0) {
                    while (g_MenuExpPartySlots[frame.partyIndex].limitBitIndex != 0xA) {
                        if ((*(unsigned short *)&g_BattleRewardTotals.party[frame.partyIndex].limitUnlocks & 1 << g_MenuExpPartySlots[frame.partyIndex].limitBitIndex) == 0) {
                            g_MenuExpPartySlots[frame.partyIndex].limitBitIndex++;
                            continue;
                        }
                        SfxPlayOnChannel(0x40, 0x1E1, 2);
                        g_MenuExpPartySlots[frame.partyIndex].holdFrames = 80;
                        g_MenuExpPartySlots[frame.partyIndex].limitBitIndex++;
                        break;
                    }
                }
            }
            if (g_MenuExpPartySlots[frame.partyIndex].timer != 0) {
                if (g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex - 1 < 8) {
                    frame.leveledFlags = gSavemap.chars[frame.saveSlot & 255].weaponMateria[g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex - 1] & 0xFF;
                } else {
                    frame.leveledFlags = gSavemap.chars[frame.saveSlot & 255].armorMateria[g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex - 9] & 0xFF;
                }
                Battle_DrawResultMateriaLevelUpBox2x(251, frame.levelUpOffset + 9, frame.leveledFlags);
                g_MenuExpPartySlots[frame.partyIndex].timer--;
            }
            if (g_MenuExpPartySlots[frame.partyIndex].holdFrames != 0) {
                Battle_DrawResultLimitPanel2x(251, frame.levelUpOffset + 9, frame.partyIndex, g_MenuExpPartySlots[frame.partyIndex].limitBitIndex - 1);
                g_MenuExpPartySlots[frame.partyIndex].holdFrames--;
            }
            if (g_MenuPartyStats[frame.partyIndex].hp == 0) {
                frame.rowColor = 2;
            } else {
                frame.rowColor = (g_MenuPartyStats[frame.partyIndex].hp >= g_MenuPartyStats[frame.partyIndex].maxHp / 4) + 6;
            }
            frame.textColor = frame.rowColor;
            Menu_DrawEncodedString2x(377, frame.levelUpOffset + 0x13, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 9, 8), frame.textColor, 0.100100003f);
            Menu_DrawEncodedString2x(178, frame.levelUpOffset + 0x49, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x2B, 8), frame.textColor, 0.100100003f);
            Menu_DrawEncodedString2x(307, frame.levelUpOffset + 0x49, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x2C, 8), frame.textColor, 0.100100003f);
            Menu_DrawEncodedString2x(141, frame.levelUpOffset + 0x18, gSavemap.chars[(frame.saveSlot & 255)].name, frame.textColor, 0);
            Menu_DrawGaugeBar2x(448, frame.levelUpOffset + 0x34, g_BattleRewardTotals.party[frame.partyIndex].expBar << 1, 11, 0x90802020, 0.100100003f);
            Menu_DrawAtlasQuad2x(448, frame.levelUpOffset + 0x33, 0x88, 8, 64, 8, 7, 0, 0, 0.101099998f);
            Menu_DrawAtlasQuad2x(570, frame.levelUpOffset + 0x1C, 0xF8, 8, 8, 8, frame.textColor, 0, 0, 0.100100003f);
            Menu_DrawAtlasQuad2x(570, frame.levelUpOffset + 0x4F, 0xF8, 8, 8, 8, frame.textColor, 0, 0, 0.100100003f);
            Menu_DrawDecimalNumber2x(450, frame.levelUpOffset + 0x1A, g_BattleRewardTotals.party[frame.partyIndex].displayedExp, 10, frame.textColor, 0.100100003f);
            Menu_DrawDecimalNumber2x(450, frame.levelUpOffset + 0x4D, g_BattleRewardTotals.party[frame.partyIndex].remainingExp, 10, frame.textColor, 0.100100003f);
            Menu_DrawDecimalNumber2x(240, frame.levelUpOffset + 0x4D, g_BattleRewardTotals.party[frame.partyIndex].displayedLevel, 2, frame.textColor, 0.100100003f);
            Menu_DrawCharPortrait2x(35, frame.levelUpOffset + 9, frame.partyIndex, 0.100000001f);
        }
        Menu_SetRectShorts(&frame.rect.x, 0, frame.levelUpOffset, 640, 120);
        Menu_DrawWindowFrame2x(&frame.rect, 0.101000004f);
    }
    if (g_MenuTransitionTimer == 20 && g_BattleResultExpActive != 0) {
        if (Input_GetPressed(INPUT_ANY) != 0 && frame.rowUpdateResult == 0) {
            g_BattleResultGilItemsFlag = 1;
            g_MenuTransitionStep = 0 - g_MenuTransitionStep;
            g_MenuResultFadeStep = -6;
            g_MenuBattleEndMode = MENU_BATTLE_END_EXP_TO_GIL;
            g_MenuResultFadeLevel = 0xFF;
        }
    }
    g_MenuTransitionTimer += g_MenuTransitionStep;
    if (g_MenuBattleEndMode == MENU_BATTLE_END_EXP && g_MenuTransitionTimer < 20) {
        g_MenuTransitionTimer = 20;
        if (Input_GetPressed(INPUT_ANY) != 0) {
            g_BattleResultExpActive = 1;
            for (frame.partyIndex = 0; frame.partyIndex < 4; frame.partyIndex++) {
                g_BattleRewardItems[frame.partyIndex].selected = 0;
            }
        }
    }
    if (g_MenuTransitionTimer > 100) {
        g_MenuTransitionTimer = 100;
    }
    if (g_BattleResultExpActive != 0 && frame.rowUpdateResult != 0 && (g_BattleResultScreenState & 2) != 0) {
        SfxPlayOnChannel(0x40, 0x2D, 1);
    }
}

/*
 * Draws a 0x80 x 0x2B window frame at (originX, originY) with the encoded string
 * "LEVEL UP" label of g_MenuMainLabels inset by 13 pixels, at 2x scale.
 */
// FUNCTION: FF7 0x006c85f1
void __cdecl Battle_DrawResultLevelUpBox2x(int originX, int originY)
{
    int rect[2];

    Menu_DrawEncodedString2x(originX + 0xD, originY + 0xD, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_LEVEL_UP * 0x14, 6, 0);
    Menu_SetRectShorts((short *)rect, originX, originY, 128, 43);
    Menu_DrawWindowFrame2x((EnglishWindowFrame *)rect, 0);
}

/*
 * Draw the materia panel for a materia the caller names. Battle_DrawResultNewMateriaBox2x
 * with the materia taken from the caller instead of g_MenuNewMateriaId, and the caption replaced by
 * the "LEVEL UP" label of g_MenuMainLabels.
 */
// FUNCTION: FF7 0x006c8643
void __cdecl Battle_DrawResultMateriaLevelUpBox2x(int x, int y, int materia)
{
    int color;
    EnglishWindowFrame box;
    short junk[8];

    color = Menu_GetMateriaTypeColorIndex(materia);
    Menu_DrawAtlasQuad2x(x + 0xd, y + 0xb, 0x80, 0x20, 16, 16, color, 0, 0,
                         0.0f);
    Menu_DrawEncodedString2x(x + 0x28, y + 0xb, Kernel_LookupMateriaNameText(materia), 7, 0.0f);
    Menu_DrawEncodedString2x(x + 0x5d, y + 0x27, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_LEVEL_UP * 0x14, 6, 0.0f);
    Menu_SetRectShorts(&box.x, x, y, 208, 69);
    Menu_DrawWindowFrame2x(&box, 0.0f);
}

/*
 * Draw the materia panel: icon, caption, name and frame, at the double-size
 * metrics. The icon, the caption "materia" from UI text group 8 entry 0x25, and
 * the name Kernel_LookupMateriaNameText gives for the same byte, all placed relative to the
 * caller's corner; then the frame, drawn last over a rect the routine builds on
 * its own stack.
 */
// FUNCTION: FF7 0x006c86f6
void __cdecl Battle_DrawResultNewMateriaBox2x(int x, int y)
{
    int color;
    EnglishWindowFrame box;

    color = Menu_GetMateriaTypeColorIndex(g_MenuNewMateriaId);
    Menu_DrawAtlasQuad2x(x + 0xd, y + 0xb, 0x80, 0x20, 16, 16, color, 0, 0,
                         0.0f);
    Menu_DrawEncodedString2x(x + 0x35, y + 0x27, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x25, 8), 7,
                      0.0f);
    Menu_DrawEncodedString2x(x + 0x28, y + 0xb, Kernel_LookupMateriaNameText(g_MenuNewMateriaId), 7, 0.0f);
    Menu_SetRectShorts(&box.x, x, y, 217, 75);
    Menu_DrawWindowFrame2x(&box, 0.0f);
}

/*
 * Draw the limit-break panel: caption, the limit's name and the frame, at the
 * double-size metrics. The caption is UI text group 8 entry 0x24.
 */
// FUNCTION: FF7 0x006c87bb
void __cdecl Battle_DrawResultLimitPanel2x(int x, int y, int slot, int level)
{
    EnglishWindowFrame box;

    Menu_DrawEncodedString2x(x + 0x4e, y + 0x27, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x24, 8), 7,
                      0.0f);
    Menu_DrawEncodedString2x(x + 0xd, y + 0xb,
                      (unsigned char *)Kernel_LookupUiText(
                          KERNEL_TEXT_LIMIT,
                          Kernel_GetLimitAttackId(
                              g_CharIdToSaveSlot[gSavemap.party[slot]], level),
                          8),
                      6, 0.0f);
    Menu_SetRectShorts(&box.x, x, y, 217, 75);
    Menu_DrawWindowFrame2x(&box, 0.0f);
}

/*
 * Updates and draws the single-scale battle EXP/result screen with sliding
 * positions derived from g_MenuTransitionTimer. Enabled results advance EXP rows
 * and timed global/per-character level and ability notifications, clearing
 * consumed bits in one mask while scanning another without clearing it.
 */
// FUNCTION: FF7 0x006c885e
void __cdecl Battle_DrawResultExpScreen1x(void)
{
    struct {
        int rowColor;
        int apPanelX;
        int leveledFlags;
        short levelUpOffset;
        short gap60;
        int unused56;
        unsigned int saveSlot;
        short partyIndex;
        short gap48;
        int unused44;
        int unused40;
        EnglishWindowFrame rect;
        int expRowY;
        int rowUpdateResult;
        int materiaBoxY;
        int expPanelX;
        short textColor;
        short gap12;
        int labelX;
        int apRowY;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_BattleResultExpActive != 0) {
        frame.rowUpdateResult = Battle_UpdateExpResultRows(Input_GetPressed(INPUT_ANY));
    }
    frame.labelX = 0;
    frame.materiaBoxY = 35 - g_MenuTransitionTimer * 35 / 20;
    frame.expPanelX = 147 - g_MenuTransitionTimer * 147 / 20;
    frame.expRowY = 35;
    frame.apPanelX = g_MenuTransitionTimer * 141 / 20;
    frame.apRowY = 35;
    if (g_BattleResultExpActive != 0) {
        if (g_MenuNewMateriaDelay == 0 && g_MenuNewMateriaReadIndex < g_MenuNewMateriaCount) {
            SfxPlayOnChannel(0x40, 0x1E1, 2);
            g_MenuNewMateriaDelay = 150;
            g_MenuNewMateriaId = g_BattleAcquiredMateriaIds[g_MenuNewMateriaReadIndex];
            g_MenuNewMateriaReadIndex++;
        }
        if (g_MenuNewMateriaDelay != 0) {
            Battle_DrawResultNewMateriaBox1x(108, frame.materiaBoxY + 0xD);
            g_MenuNewMateriaDelay--;
        }
    }
    Menu_DrawAtlasQuad1x(frame.expPanelX + 0x8D, frame.expRowY + 0xA, 248, 8, 8, 8, 7, 0, 0, 0.101000004f);
    Menu_DrawDecimalNumber1x(frame.expPanelX + 0x57, frame.expRowY + 9, g_BattleRewardExp, 9, 7, 0.101000004f);
    Menu_DrawAtlasQuad1x(frame.apPanelX + 0x8D, frame.apRowY + 0xA, 248, 8, 8, 8, 7, 0, 0, 0.112999998f);
    Menu_DrawDecimalNumber1x(frame.apPanelX + 0x57, frame.apRowY + 9, g_BattleRewardAp, 9, 7, 0.112999998f);
    Menu_DrawEncodedString1x(frame.labelX + 0xE, frame.materiaBoxY + 0xD, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x10, 8), 7, 0.100000001f);
    Menu_DrawEncodedString1x(frame.expPanelX + 0x1D, frame.expRowY + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x13, 8), 7, 0.101000004f);
    Menu_DrawEncodedString1x(frame.apPanelX + 0x3C, frame.apRowY + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x14, 8), 7, 0.112999998f);
    Menu_SetRectShorts(&frame.rect.x, frame.labelX, frame.materiaBoxY, 320, 35);
    Menu_DrawWindowFrame1x(&frame.rect, 0.101000004f);
    Menu_SetRectShorts(&frame.rect.x, frame.expPanelX, frame.expRowY, 176, 26);
    Menu_DrawWindowFrame1x(&frame.rect, 0.101000004f);
    Menu_SetRectShorts(&frame.rect.x, frame.apPanelX, frame.apRowY, 179, 26);
    Menu_DrawWindowFrame1x(&frame.rect, 0.112999998f);
    for (frame.partyIndex = 0; frame.partyIndex < 3; frame.partyIndex++) {
        *(unsigned char *)&frame.saveSlot = g_CharIdToSaveSlot[gSavemap.party[frame.partyIndex]];
        frame.levelUpOffset = (frame.partyIndex * 60 + 0x3C) * g_MenuTransitionTimer / 20;
        if (gSavemap.party[frame.partyIndex] != PARTY_CHAR_NONE && g_BattleRewardTotals.party[frame.partyIndex].displayedLevel != 0) {
            if (g_MenuExpPartySlots[frame.partyIndex].displayedLevel != g_BattleRewardTotals.party[frame.partyIndex].displayedLevel) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_HEAL);
                g_MenuExpPartySlots[frame.partyIndex].countdown = 80;
            }
            if (g_MenuExpPartySlots[frame.partyIndex].countdown != 0) {
                Battle_DrawResultLevelUpBox1x(29, frame.levelUpOffset + 0x20);
                g_MenuExpPartySlots[frame.partyIndex].countdown--;
            }
            g_MenuExpPartySlots[frame.partyIndex].displayedLevel = g_BattleRewardTotals.party[frame.partyIndex].displayedLevel;
            if (g_BattleResultExpActive != 0) {
                if (g_MenuExpPartySlots[frame.partyIndex].timer == 0) {
                    while (g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex != 0x10) {
                        if (((unsigned short)g_MateriaLevelUpSlotMask[frame.partyIndex] & 1 << g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex) == 0) {
                            g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex++;
                            continue;
                        }
                        SfxPlayOnChannel(0x40, 0x1E1, 2);
                        g_MenuExpPartySlots[frame.partyIndex].timer = 80;
                        g_MateriaLevelUpSlotMask[frame.partyIndex] &= (short)~(1 << g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex);
                        g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex++;
                        break;
                    }
                }
                if (g_MenuExpPartySlots[frame.partyIndex].holdFrames == 0 && g_MenuExpPartySlots[frame.partyIndex].timer == 0) {
                    while (g_MenuExpPartySlots[frame.partyIndex].limitBitIndex != 0xA) {
                        if ((*(unsigned short *)&g_BattleRewardTotals.party[frame.partyIndex].limitUnlocks & 1 << g_MenuExpPartySlots[frame.partyIndex].limitBitIndex) == 0) {
                            g_MenuExpPartySlots[frame.partyIndex].limitBitIndex++;
                            continue;
                        }
                        SfxPlayOnChannel(0x40, 0x1E1, 2);
                        g_MenuExpPartySlots[frame.partyIndex].holdFrames = 80;
                        g_MenuExpPartySlots[frame.partyIndex].limitBitIndex++;
                        break;
                    }
                }
            }
            if (g_MenuExpPartySlots[frame.partyIndex].timer != 0) {
                if (g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex - 1 < 8) {
                    frame.leveledFlags = *(&gSavemap.chars[frame.saveSlot & 255].exp + g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex) & 0xFF;
                } else {
                    frame.leveledFlags = *(&gSavemap.chars[frame.saveSlot & 255].exp + g_MenuExpPartySlots[frame.partyIndex].materiaBitIndex) & 0xFF;
                }
                Battle_DrawResultMateriaLevelUpBox1x(125, frame.levelUpOffset + 4, frame.leveledFlags);
                g_MenuExpPartySlots[frame.partyIndex].timer--;
            }
            if (g_MenuExpPartySlots[frame.partyIndex].holdFrames != 0) {
                Battle_DrawResultLimitPanel1x(125, frame.levelUpOffset + 4, frame.partyIndex, g_MenuExpPartySlots[frame.partyIndex].limitBitIndex - 1);
                g_MenuExpPartySlots[frame.partyIndex].holdFrames--;
            }
            if (g_MenuPartyStats[frame.partyIndex].hp == 0) {
                frame.rowColor = 2;
            } else {
                frame.rowColor = (g_MenuPartyStats[frame.partyIndex].hp >= g_MenuPartyStats[frame.partyIndex].maxHp / 4) + 6;
            }
            frame.textColor = frame.rowColor;
            Menu_DrawEncodedString1x(189, frame.levelUpOffset + 0xA, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 9, 8), frame.textColor, 0.100100003f);
            Menu_DrawEncodedString1x(89, frame.levelUpOffset + 0x25, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x2B, 8), frame.textColor, 0.100100003f);
            Menu_DrawEncodedString1x(154, frame.levelUpOffset + 0x25, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x2C, 8), frame.textColor, 0.100100003f);
            Menu_DrawEncodedString1x(72, frame.levelUpOffset + 0xC, gSavemap.chars[(frame.saveSlot & 255)].name, frame.textColor, 0);
            Menu_DrawGaugeBar1x(224, frame.levelUpOffset + 0x1A, g_BattleRewardTotals.party[frame.partyIndex].expBar, 6, 0x90802020, 0.100100003f);
            Menu_DrawAtlasQuad1x(224, frame.levelUpOffset + 0x1A, 136, 8, 64, 8, 7, 0, 0, 0.101099998f);
            Menu_DrawAtlasQuad1x(285, frame.levelUpOffset + 0xE, 248, 8, 8, 8, frame.textColor, 0, 0, 0.100100003f);
            Menu_DrawAtlasQuad1x(285, frame.levelUpOffset + 0x28, 248, 8, 8, 8, frame.textColor, 0, 0, 0.100100003f);
            Menu_DrawDecimalNumber1x(225, frame.levelUpOffset + 0xD, g_BattleRewardTotals.party[frame.partyIndex].displayedExp, 0xA, frame.textColor, 0.100100003f);
            Menu_DrawDecimalNumber1x(225, frame.levelUpOffset + 0x27, g_BattleRewardTotals.party[frame.partyIndex].remainingExp, 0xA, frame.textColor, 0.100100003f);
            Menu_DrawDecimalNumber1x(120, frame.levelUpOffset + 0x27, g_BattleRewardTotals.party[frame.partyIndex].displayedLevel, 2, frame.textColor, 0.100100003f);
            Menu_DrawCharPortrait1x(18, frame.levelUpOffset + 5, frame.partyIndex, 0.100000001f);
        }
        Menu_SetRectShorts(&frame.rect.x, 0, frame.levelUpOffset, 320, 60);
        Menu_DrawWindowFrame1x(&frame.rect, 0.101000004f);
    }
    if (g_MenuTransitionTimer == 20 && g_BattleResultExpActive != 0) {
        if (Input_GetPressed(INPUT_ANY) != 0 && frame.rowUpdateResult == 0) {
            g_BattleResultGilItemsFlag = 1;
            g_MenuTransitionStep = 0 - g_MenuTransitionStep;
            g_MenuResultFadeStep = -6;
            g_MenuBattleEndMode = MENU_BATTLE_END_EXP_TO_GIL;
            g_MenuResultFadeLevel = 0xFF;
        }
    }
    g_MenuTransitionTimer += g_MenuTransitionStep;
    if (g_MenuBattleEndMode == MENU_BATTLE_END_EXP && g_MenuTransitionTimer < 20) {
        g_MenuTransitionTimer = 20;
        if (Input_GetPressed(INPUT_ANY) != 0) {
            g_BattleResultExpActive = 1;
            for (frame.partyIndex = 0; frame.partyIndex < 4; frame.partyIndex++) {
                g_BattleRewardItems[frame.partyIndex].selected = 0;
            }
        }
    }
    if (g_MenuTransitionTimer > 100) {
        g_MenuTransitionTimer = 100;
    }
    if (g_BattleResultExpActive != 0 && frame.rowUpdateResult != 0 && (g_BattleResultScreenState & 2) != 0) {
        SfxPlayOnChannel(0x40, 0x2D, 1);
    }
}

/*
 * Draws a 0x40 x 0x16 window frame at (originX, originY) with the encoded string
 * "LEVEL UP" label of g_MenuMainLabels inset by 7 pixels, at 1x scale.
 */
// FUNCTION: FF7 0x006c92e2
void __cdecl Battle_DrawResultLevelUpBox1x(int originX, int originY)
{
    int rect[2];

    Menu_DrawEncodedString1x(originX + 7, originY + 7, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_LEVEL_UP * 0x14, 6, 0);
    Menu_SetRectShorts((short *)rect, originX, originY, 64, 22);
    Menu_DrawWindowFrame1x((EnglishWindowFrame *)rect, 0);
}

/*
 * Battle_DrawResultMateriaLevelUpBox2x at the single-size metrics.
 * Battle_DrawResultNewMateriaBox2x with the materia taken from the caller instead of
 * g_MenuNewMateriaId, and the caption replaced by the "LEVEL UP" label of g_MenuMainLabels.
 */
// FUNCTION: FF7 0x006c9331
void __cdecl Battle_DrawResultMateriaLevelUpBox1x(int x, int y, int materia)
{
    int color;
    EnglishWindowFrame box;
    short junk[8];

    color = Menu_GetMateriaTypeColorIndex(materia);
    Menu_DrawAtlasQuad1x(x + 0x7, y + 0x6, 128, 32, 16, 16, color, 0, 0,
                         0.0f);
    Menu_DrawEncodedString1x(x + 0x14, y + 0x6, Kernel_LookupMateriaNameText(materia), 7, 0.0f);
    Menu_DrawEncodedString1x(x + 0x2f, y + 0x14, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_LEVEL_UP * 0x14, 6, 0.0f);
    Menu_SetRectShorts(&box.x, x, y, 104, 35);
    Menu_DrawWindowFrame1x(&box, 0.0f);
}

/*
 * Battle_DrawResultNewMateriaBox2x at the single-size metrics: the same four calls against
 * the 1x routines, every offset and the frame rect roughly halved. The icon, the caption "materia"
 * from UI text group 8 entry 0x25, and the name Kernel_LookupMateriaNameText gives for the same
 * byte, all placed relative to the caller's corner; then the frame, drawn last over a rect the
 * routine builds on its own stack.
 */
// FUNCTION: FF7 0x006c93e1
void __cdecl Battle_DrawResultNewMateriaBox1x(int x, int y)
{
    int color;
    EnglishWindowFrame box;

    color = Menu_GetMateriaTypeColorIndex(g_MenuNewMateriaId);
    Menu_DrawAtlasQuad1x(x + 0x7, y + 0x6, 128, 32, 16, 16, color, 0, 0,
                         0.0f);
    Menu_DrawEncodedString1x(x + 0x1b, y + 0x14, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x25, 8), 7,
                         0.0f);
    Menu_DrawEncodedString1x(x + 0x14, y + 0x6, Kernel_LookupMateriaNameText(g_MenuNewMateriaId), 7, 0.0f);
    Menu_SetRectShorts(&box.x, x, y, 109, 38);
    Menu_DrawWindowFrame1x(&box, 0.0f);
}

/*
 * Battle_DrawResultLimitPanel2x at the single-size metrics. The caption is UI text group 8 entry
 * 0x24.
 */
// FUNCTION: FF7 0x006c94a3
void __cdecl Battle_DrawResultLimitPanel1x(int x, int y, int slot, int level)
{
    EnglishWindowFrame box;

    Menu_DrawEncodedString1x(x + 0x27, y + 0x14, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x24, 8), 7,
                         0.0f);
    Menu_DrawEncodedString1x(x + 0x7, y + 0x6,
                         (unsigned char *)Kernel_LookupUiText(
                             KERNEL_TEXT_LIMIT,
                             Kernel_GetLimitAttackId(
                                 g_CharIdToSaveSlot[gSavemap.party[slot]], level),
                             8),
                         6, 0.0f);
    Menu_SetRectShorts(&box.x, x, y, 109, 38);
    Menu_DrawWindowFrame1x(&box, 0.0f);
}

/*
 * One frame of the menu's top-level dispatch: flip the double-buffer flag, run the phase's update
 * and draw, and count the frame. g_MenuIsSmallViewport320x240 picks between two five-case
 * tables that are the same shape -- Battle_DrawResultExpScreen2x and
 * Battle_DrawResultGilAndItemsScreen2x on one side, Battle_DrawResultExpScreen1x and
 * Battle_DrawResultGilAndItemsScreen1x on the other.
 */
// FUNCTION: FF7 0x006c9543
void __cdecl Battle_DrawResultScreenFrame(void)
{
    if (g_BattleResultFrameStarted == 0) {
        g_BattleResultFrameStarted = 1;
    }
    g_MenuModuleId ^= 1;
    if (g_MenuBattleEndMode == MENU_BATTLE_END_DONE) {
        g_BattleResultSavedExpActive = 0xff;
        Battle_AddRewardItemsToInventoryThunk();
        g_MenuEntryState = MENU_ENTRY_NONE;
        g_MenuBattleResultFinished = 1;
    }
    if (g_MenuIsSmallViewport320x240 != 1) {
        switch (g_MenuBattleEndMode) {
        case MENU_BATTLE_END_EXP:
            Battle_DrawResultExpScreen2x();
            break;
        case MENU_BATTLE_END_EXP_TO_GIL:
            Battle_DrawResultExpScreen2x();
            Battle_DrawResultGilAndItemsScreen2x();
            break;
        case MENU_BATTLE_END_GIL_ITEMS:
            g_MenuFrameClearFlag = 0;
        case MENU_BATTLE_END_GIL_ITEMS_FADE_OUT:
            Battle_DrawResultGilAndItemsScreen2x();
            break;
        case MENU_BATTLE_END_FADED_OUT:
            Menu_FillViewportQuad((unsigned char)g_MenuResultFadeLevel, 0.1f);
            g_MenuBattleEndMode = MENU_BATTLE_END_DONE;
            break;
        }
    } else {
        switch (g_MenuBattleEndMode) {
        case MENU_BATTLE_END_EXP:
            Battle_DrawResultExpScreen1x();
            break;
        case MENU_BATTLE_END_EXP_TO_GIL:
            Battle_DrawResultExpScreen1x();
            Battle_DrawResultGilAndItemsScreen1x();
            break;
        case MENU_BATTLE_END_GIL_ITEMS:
            g_MenuFrameClearFlag = 0;
        case MENU_BATTLE_END_GIL_ITEMS_FADE_OUT:
            Battle_DrawResultGilAndItemsScreen1x();
            break;
        case MENU_BATTLE_END_FADED_OUT:
            Menu_FillViewportQuad((unsigned char)g_MenuResultFadeLevel, 0.1f);
            g_MenuBattleEndMode = MENU_BATTLE_END_DONE;
            break;
        }
    }
    g_BattleResultScreenState++;
    if (g_BattleResultFrameStarted != 0 && g_MenuBattleEndMode == MENU_BATTLE_END_DONE && g_MenuEntryState == MENU_ENTRY_NONE) {
        g_MenuExitRequested = 1;
    }
}

/*
 * Initializes battle-result state, refreshes experience rows, and seeds three six-
 * byte rows from g_BattleRewardTotals.party[0].displayedLevel at stride 0xC while clearing their other five bytes.
 * Clears result counters, sets g_MenuTransitionTimer to 100 and
 * g_MenuTransitionStep to -3.
 */
// FUNCTION: FF7 0x006c96bc
void __cdecl Battle_InitResultScreenState(void)
{
    int padSlot[4]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int r;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unused[4]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unusedWord[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    g_BattleResultFrameStarted = 0;
    g_MenuNonTopLevelScreenFlag = 1;
    Battle_UpdateExpResultRows(0);
    g_MenuNewMateriaReadIndex = 0;
    g_MenuNewMateriaDelay = 0;
    for (r = 0; r < 3; r++) {
        g_MenuExpPartySlots[r].displayedLevel = g_BattleRewardTotals.party[r].displayedLevel;
        g_MenuExpPartySlots[r].materiaBitIndex = 0;
        g_MenuExpPartySlots[r].timer = 0;
        g_MenuExpPartySlots[r].countdown = 0;
        g_MenuExpPartySlots[r].limitBitIndex = 0;
        g_MenuExpPartySlots[r].holdFrames = 0;
    }
    g_MenuModuleId = 0;
    g_MenuBattleEndMode = (short)g_MenuModuleId;
    g_BattleResultExpActive = (char)g_MenuBattleEndMode;
    g_BattleResultSavedExpActive = g_BattleResultExpActive;
    g_MenuTransitionTimer = 100;
    g_MenuTransitionStep = 0xFFFD;
    g_MenuResultGilItemsEnabled = 0;
    for (r = 0; r < 4; r++) {
        if (g_BattleRewardItems[r].itemId != -1) {
            g_MenuResultGilItemsEnabled = 1;
        }
    }
    g_MenuResultItemRow = g_MenuResultGilItemsEnabled != 0 ? MENU_RESULT_ITEM_ROW_TAKE_ALL : MENU_RESULT_ITEM_ROW_CONTINUE;
    g_MenuFrameClearFlag = 1;
}

/*
 * Calls Battle_AddRewardItemsToInventory.
 */
// FUNCTION: FF7 0x006c97fe
void __cdecl Battle_AddRewardItemsToInventoryThunk(void)
{
    Battle_AddRewardItemsToInventory();
}

/*
 * Returns the menu phase g_MenuPhase.
 */
// FUNCTION: FF7 0x006c9808
int __cdecl Menu_GetPhase(void)
{
    return g_MenuPhase;
}

/*
 * Record the new phase and the one it replaces, then do whatever that phase needs.
 * The second argument is decremented into its own slot before it is stored, so the
 * caller passes a one-based value.
 */
// FUNCTION: FF7 0x006c9812
void __cdecl Menu_SetPhase(int phase, int prev)
{
    prev = prev - 1;
    g_MenuPhase = phase;
    g_MenuCommandRow = g_MenuSavedCommandRow;
    g_MenuSavedCommandRow = prev;
    switch (phase) {
    case MENU_PHASE_SCREEN:
        break;
    case MENU_PHASE_OPEN_SCREEN:
        g_MenuCommandWindowAnimStep = 16;
        break;
    case MENU_PHASE_SWITCH_SCREEN:
        g_MenuCommandWindowAnimStep = 16;
        break;
    case MENU_PHASE_CLOSE_SCREEN:
        g_MenuMainCursors.command.row = g_MenuCommandRow;
        g_MenuCommandOpenScale = 0;
        g_MenuCommandWindowAnimStep = 16;
        break;
    }
}

/*
 * Animates and draws the double-resolution command window through the MenuPhase
 * values. OPEN_SCREEN and SWITCH_SCREEN count the fade down and invoke the selected
 * screen's init callback at zero; SCREEN_FADE_IN counts it up to 15 and returns to
 * MENU_PHASE_SCREEN, setting the frame-clear flag only for the Save screen.
 */
// FUNCTION: FF7 0x006c98a6
void __cdecl Menu_DrawCommandWindow2x(int mode)
{
    unsigned char c;
    int posX;
    unsigned char done;
    EnglishWindowFrame box;
    int dy;
    unsigned char bit;

    posX = 0;
    done = 0;
    switch (g_MenuPhase) {
    case MENU_PHASE_SCREEN:
        g_MenuCommandWindowHeight = 51;
        Menu_DrawEncodedString2x(508, 13,
                          g_MenuSavedCommandRow * 0x14 + ((unsigned char *)&g_MenuMainLabels), 7,
                          0.001f);
        break;
    case MENU_PHASE_OPEN_SCREEN:
        if (g_MenuCommandWindowAnimStep != 0) {
            g_MenuCommandWindowAnimStep = g_MenuCommandWindowAnimStep - 1;
        } else {
            g_MenuScreenInitCallbacks[g_MenuCurrentScreenId](0);
            g_MenuPhase = MENU_PHASE_SCREEN_FADE_IN;
        }
        dy = g_MenuSavedCommandRow * 26 * g_MenuCommandWindowAnimStep / 16;
        Menu_DrawEncodedString2x(508, dy + 0xd,
                          g_MenuSavedCommandRow * 0x14 + ((unsigned char *)&g_MenuMainLabels), 7,
                          0.001f);
        g_MenuCommandWindowHeight = g_MenuCommandWindowAnimStep * 286 / 16 + 51;
        break;
    case MENU_PHASE_SWITCH_SCREEN:
        if (g_MenuCommandWindowAnimStep != 0) {
            g_MenuCommandWindowAnimStep = g_MenuCommandWindowAnimStep - 1;
        } else {
            g_MenuPhase = MENU_PHASE_SCREEN_FADE_IN;
            g_MenuScreenInitCallbacks[g_MenuCurrentScreenId](0);
        }
        Menu_DrawEncodedString2x(508, 13,
                          g_MenuCommandRow * 0x14 + ((unsigned char *)&g_MenuMainLabels), 7,
                          0.001f);
        g_MenuCommandWindowHeight = 51;
        break;
    case MENU_PHASE_SCREEN_FADE_IN:
        if (g_MenuCommandWindowAnimStep < 15) {
            g_MenuCommandWindowAnimStep = g_MenuCommandWindowAnimStep + 1;
        } else {
            g_MenuPhase = MENU_PHASE_SCREEN;
            if (g_MenuCurrentScreenId == MENU_SCREEN_SAVE) {
                g_MenuFrameClearFlag = 1;
            } else {
                g_MenuFrameClearFlag = 0;
            }
        }
        g_MenuCommandWindowHeight = 51;
        Menu_DrawEncodedString2x(508, 13,
                          g_MenuSavedCommandRow * 0x14 + ((unsigned char *)&g_MenuMainLabels), 7,
                          0.001f);
        break;
    case MENU_PHASE_MAIN:
        g_MenuCommandWindowHeight = 312;
        if (g_MenuTopLevelCloseState != MENU_TOP_LEVEL_ACTIVE) {
            posX = g_MenuTransitionTimer * 100 / 10 - 100;
        }
        for (bit = 0; bit < 0xb; bit++) {
            if (g_MenuVisibleMask & (1 << bit)) {
                Menu_DrawEncodedString2x(508, posX + bit * 0x1a + 0xd,
                                  bit * 0x14 + ((unsigned char *)&g_MenuMainLabels),
                                  (g_MenuLockedMask & (1 << bit)) == 0 ? 7 : 0,
                                  0.001f);
            }
        }
        break;
    case MENU_PHASE_CLOSE_SCREEN:
        if (g_MenuCommandWindowAnimStep != 0) {
            g_MenuCommandWindowAnimStep = g_MenuCommandWindowAnimStep - 1;
            if (g_MenuCommandWindowAnimStep == 0 && g_MenuTopLevelCloseState == MENU_TOP_LEVEL_CLOSING) {
                g_MenuTopLevelCloseState = MENU_TOP_LEVEL_CLOSED;
                g_MenuPhase = MENU_PHASE_MAIN;
            }
        } else {
            g_MenuPhase = MENU_PHASE_EXPAND_COMMANDS;
        }
        Menu_DrawEncodedString2x(508, 13,
                          g_MenuCommandRow * 0x14 + ((unsigned char *)&g_MenuMainLabels), 7,
                          0.001f);
        g_MenuCommandWindowHeight = 51;
        break;
    case MENU_PHASE_EXPAND_COMMANDS:
        if (g_MenuCommandWindowAnimStep < 15) {
            g_MenuCommandWindowAnimStep = g_MenuCommandWindowAnimStep + 1;
        } else {
            g_MenuPhase = MENU_PHASE_MAIN;
            g_MenuFrameClearFlag = 1;
        }
        dy = g_MenuCommandRow * 26 * g_MenuCommandOpenScale / 16;
        Menu_DrawEncodedString2x(508, dy + 0xd,
                          g_MenuCommandRow * 0x14 + ((unsigned char *)&g_MenuMainLabels), 7,
                          0.001f);
        g_MenuCommandWindowHeight = g_MenuCommandOpenScale * 261 / 16 + 51;
        g_MenuCommandOpenScale = g_MenuCommandOpenScale + 1;
        break;
    }
    Menu_SetRectShorts(&box.x, 476, posX, 164, g_MenuCommandWindowHeight);
    Menu_DrawWindowFrame2x(&box, 0.001f);
    if (g_MenuPhase != MENU_PHASE_SCREEN && g_MenuPhase != MENU_PHASE_MAIN || g_MenuTopLevelCloseState == MENU_TOP_LEVEL_CLOSED) {
        c = 0xff - g_MenuCommandWindowAnimStep * 16;
        Menu_FillViewportQuad(c, 0.1f);
    }
}

/*
 * Animates and draws the low-resolution command window through the MenuPhase values.
 * OPEN_SCREEN and SWITCH_SCREEN count the fade down and invoke the selected screen's
 * init callback at zero; SCREEN_FADE_IN counts it up to 15 and returns to
 * MENU_PHASE_SCREEN, setting the frame-clear flag only for the Save screen.
 */
// FUNCTION: FF7 0x006c9d07
void __cdecl Menu_DrawCommandWindow1x(int moduleId)
{
    unsigned char c;
    int posX;
    unsigned char done;
    EnglishWindowFrame box;
    int dy;
    unsigned char bit;

    posX = 0;
    done = 0;
    switch (g_MenuPhase) {
    case MENU_PHASE_SCREEN:
        g_MenuCommandWindowHeight = 26;
        Menu_DrawEncodedString1x(262, 7,
                             g_MenuSavedCommandRow * 0x14 + ((unsigned char *)&g_MenuMainLabels), 7,
                             0.001f);
        break;
    case MENU_PHASE_OPEN_SCREEN:
        if (g_MenuCommandWindowAnimStep != 0) {
            g_MenuCommandWindowAnimStep = g_MenuCommandWindowAnimStep - 1;
        } else {
            g_MenuScreenInitCallbacks[g_MenuCurrentScreenId](0);
            g_MenuPhase = MENU_PHASE_SCREEN_FADE_IN;
        }
        dy = g_MenuSavedCommandRow * 13 * g_MenuCommandWindowAnimStep / 16;
        Menu_DrawEncodedString1x(262, dy + 7,
                             g_MenuSavedCommandRow * 0x14 + ((unsigned char *)&g_MenuMainLabels), 7,
                             0.001f);
        g_MenuCommandWindowHeight = g_MenuCommandWindowAnimStep * 143 / 16 + 26;
        break;
    case MENU_PHASE_SWITCH_SCREEN:
        if (g_MenuCommandWindowAnimStep != 0) {
            g_MenuCommandWindowAnimStep = g_MenuCommandWindowAnimStep - 1;
        } else {
            g_MenuPhase = MENU_PHASE_SCREEN_FADE_IN;
            g_MenuScreenInitCallbacks[g_MenuCurrentScreenId](0);
        }
        Menu_DrawEncodedString1x(262, 7,
                             g_MenuCommandRow * 0x14 + ((unsigned char *)&g_MenuMainLabels), 7,
                             0.001f);
        g_MenuCommandWindowHeight = 26;
        break;
    case MENU_PHASE_SCREEN_FADE_IN:
        if (g_MenuCommandWindowAnimStep < 15) {
            g_MenuCommandWindowAnimStep = g_MenuCommandWindowAnimStep + 1;
        } else {
            g_MenuPhase = MENU_PHASE_SCREEN;
            if (g_MenuCurrentScreenId == MENU_SCREEN_SAVE) {
                g_MenuFrameClearFlag = 1;
            } else {
                g_MenuFrameClearFlag = 0;
            }
        }
        g_MenuCommandWindowHeight = 26;
        Menu_DrawEncodedString1x(262, 7,
                             g_MenuSavedCommandRow * 0x14 + ((unsigned char *)&g_MenuMainLabels), 7,
                             0.001f);
        break;
    case MENU_PHASE_MAIN:
        g_MenuCommandWindowHeight = 156;
        if (g_MenuTopLevelCloseState != MENU_TOP_LEVEL_ACTIVE) {
            posX = g_MenuTransitionTimer * 100 / 10 - 100;
        }
        for (bit = 0; bit < 0xb; bit++) {
            if (g_MenuVisibleMask & (1 << bit)) {
                Menu_DrawEncodedString1x(262, posX + bit * 0xd + 7,
                                     bit * 0x14 + ((unsigned char *)&g_MenuMainLabels),
                                     (g_MenuLockedMask & (1 << bit)) == 0 ? 7 : 0,
                                     0.001f);
            }
        }
        break;
    case MENU_PHASE_CLOSE_SCREEN:
        if (g_MenuCommandWindowAnimStep != 0) {
            g_MenuCommandWindowAnimStep = g_MenuCommandWindowAnimStep - 1;
            if (g_MenuCommandWindowAnimStep == 0 && g_MenuTopLevelCloseState == MENU_TOP_LEVEL_CLOSING) {
                g_MenuTopLevelCloseState = MENU_TOP_LEVEL_CLOSED;
                g_MenuPhase = MENU_PHASE_MAIN;
            }
        } else {
            g_MenuPhase = MENU_PHASE_EXPAND_COMMANDS;
        }
        Menu_DrawEncodedString1x(262, 7,
                             g_MenuCommandRow * 0x14 + ((unsigned char *)&g_MenuMainLabels), 7,
                             0.001f);
        g_MenuCommandWindowHeight = 26;
        break;
    case MENU_PHASE_EXPAND_COMMANDS:
        if (g_MenuCommandWindowAnimStep < 15) {
            g_MenuCommandWindowAnimStep = g_MenuCommandWindowAnimStep + 1;
        } else {
            g_MenuPhase = MENU_PHASE_MAIN;
            g_MenuFrameClearFlag = 1;
        }
        dy = g_MenuCommandRow * 13 * g_MenuCommandOpenScale / 16;
        Menu_DrawEncodedString1x(262, dy + 7,
                             g_MenuCommandRow * 0x14 + ((unsigned char *)&g_MenuMainLabels), 7,
                             0.001f);
        g_MenuCommandWindowHeight = g_MenuCommandOpenScale * 130 / 16 + 26;
        g_MenuCommandOpenScale = g_MenuCommandOpenScale + 1;
        break;
    }
    Menu_SetRectShorts(&box.x, 246, posX, 74, g_MenuCommandWindowHeight);
    Menu_DrawWindowFrame1x(&box, 0.001f);
    if (g_MenuPhase != MENU_PHASE_SCREEN && g_MenuPhase != MENU_PHASE_MAIN || g_MenuTopLevelCloseState == MENU_TOP_LEVEL_CLOSED) {
        c = 0xff - g_MenuCommandWindowAnimStep * 16;
        Menu_FillViewportQuad(c, 0.1f);
    }
}

/*
 * Initialises the main menu: the command cursor of g_MenuMainCursors over 11 entries and its party
 * and swap cursors over 3 entries each, and sets g_MenuFrameClearFlag.
 */
// FUNCTION: FF7 0x006ca165
void __cdecl Menu_MainScreenInit(int mode)
{
    MenuCursor_Init(&g_MenuMainCursors.command, 0, 0, 1, 11, 0, 0, 1, 11, 0, 0, 0, 1, 0, 0);
    MenuCursor_Init(&g_MenuMainCursors.party, 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 1, 0, 0);
    MenuCursor_Init(&g_MenuMainCursors.swap, 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 1, 0, 0);
    g_MenuFrameClearFlag = 1;
}

/*
 * Clamps input above 0x57E3F, divides by the first time-divisor global and
 * multiplies that quotient by ten, then adds the quotient of the remainder by the
 * second divisor. Returns the combined digits.
 */
// FUNCTION: FF7 0x006ca1ef
int __cdecl Menu_PlaytimeToHoursDigits(int playTime)
{
    int digits;

    if (playTime > 359999) {
        playTime = 0x57E3F;
    }
    digits = playTime / g_MenuPlayTimeStep36000;
    digits *= 0xA;
    playTime = playTime % g_MenuPlayTimeStep36000;
    digits += playTime / g_MenuPlayTimeStep3600;
    return digits;
}

/*
 * Clamps input above 0x57E3F, takes remainders through the first two time-divisor
 * globals, then combines quotients/remainders through the next two divisors as
 * tens times ten plus units. Returns the resulting minute digits; input units
 * require original divisor values and are not inferred from the parameter name.
 */
// FUNCTION: FF7 0x006ca23f
int __cdecl Menu_PlaytimeToMinutesDigits(int tenths)
{
    int digits;

    if (tenths > 359999) {
        tenths = 0x57e3f;
    }
    tenths %= g_MenuPlayTimeStep36000;
    tenths %= g_MenuPlayTimeStep3600;
    digits = tenths / g_MenuPlayTimeStep600;
    digits *= 0xa;
    tenths %= g_MenuPlayTimeStep600;
    digits += tenths / g_MenuPlayTimeStep60;
    return digits;
}

/*
 * Menu_PlaytimeToMinutesDigits two divisors further down: the two second digits, tens
 * times ten plus units.
 */
// FUNCTION: FF7 0x006ca2a9
int __cdecl Menu_PlaytimeToSecondsDigits(int tenths)
{
    int digits;

    if (tenths > 359999) {
        tenths = 0x57e3f;
    }
    tenths %= g_MenuPlayTimeStep36000;
    tenths %= g_MenuPlayTimeStep3600;
    tenths %= g_MenuPlayTimeStep600;
    tenths %= g_MenuPlayTimeStep60;
    digits = tenths / g_MenuPlayTimeStep10;
    digits *= 0xa;
    tenths %= g_MenuPlayTimeStep10;
    digits += tenths / g_MenuPlayTimeStep1;
    return digits;
}

/*
 * Sets g_MenuTopLevelCloseState to MENU_TOP_LEVEL_CLOSING and calls
 * Menu_SetRequestedScreen(0).
 */
// FUNCTION: FF7 0x006ca32d
void __cdecl Menu_BeginClosingTopLevelMenu(void)
{
    g_MenuTopLevelCloseState = MENU_TOP_LEVEL_CLOSING;
    Menu_SetRequestedScreen(0);
}

/*
 * Draws and updates the main menu in 1x or 2x layout. Initializes selection state,
 * optionally refreshes party limit tables, rebuilds command availability/lock
 * masks and handles input only in the active main-menu phase.
 */
// FUNCTION: FF7 0x006ca346
void __cdecl Menu_DrawMainScreen(int flags)
{
    GfxRenderObject *avatarPtr;
    int gaugeTint;
    int unusedEntry; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    char memberId;
    int frameRectangle[6];
    unsigned char characterSlot;
    int statBlockCur[272];
    int posY;
    int rowPos;
    int slotIdx;
    int posX;
    int portraitOffset;

    if (g_MenuMainScreenInitialized == 0 && g_MenuTopLevelCloseState != MENU_TOP_LEVEL_CLOSING) {
        g_MenuOrderSwapPicked = 0;
        g_MenuMainCursorIndex = MENU_MAIN_CURSOR_COMMAND;
        g_MenuMainScreenInitialized = 1;
    }
    if (g_MenuPartyLimitRefreshNeeded != 0) {
        Menu_RefreshPartyLimitTables();
    }
    g_MenuVisibleMask = gSavemap.menuVisible;
    g_MenuVisibleMask |= (short)(gSavemap.menuVisibleHigh << 8);
    g_MenuVisibleMask |= 1 << MENU_MAIN_COMMAND_QUIT;
    g_MenuLockedMask = gSavemap.menuLocked;
    g_MenuLockedMask |= (short)((unsigned char)gSavemap.menuFlags << 8);
    g_MenuLockedMask &= 0xFBFF;
    if ((gSavemap.snowUnused & 1) != 0) {
        g_MenuLockedMask |= 1 << MENU_MAIN_COMMAND_LIMIT;
        g_MenuLockedMask |= 1 << MENU_MAIN_COMMAND_ITEM;
    }
    if (Menu_GetPhase() == MENU_PHASE_MAIN && g_MenuTopLevelCloseState == MENU_TOP_LEVEL_ACTIVE) {
        MenuCursor_Update(&g_MenuMainCursors.list[g_MenuMainCursorIndex]);
        if (Input_GetPressed(INPUT_CIRCLE) != 0) {
            switch (*(char *)&g_MenuMainCursorIndex) {
            case MENU_MAIN_CURSOR_COMMAND:
                if ((g_MenuVisibleMask & 1 << g_MenuMainCursors.command.row) != 0 && (g_MenuLockedMask & 1 << g_MenuMainCursors.command.row) == 0) {
                    switch (g_MenuMainCursors.command.row) {
                    case MENU_MAIN_COMMAND_MAGIC:
                    case MENU_MAIN_COMMAND_MATERIA:
                    case MENU_MAIN_COMMAND_EQUIP:
                    case MENU_MAIN_COMMAND_STATUS:
                    case MENU_MAIN_COMMAND_LIMIT:
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_MenuMainCursorIndex = MENU_MAIN_CURSOR_PARTY;
                        break;
                    case MENU_MAIN_COMMAND_ORDER:
                        g_MenuMainCursorIndex = MENU_MAIN_CURSOR_ORDER;
                        break;
                    case MENU_MAIN_COMMAND_ITEM:
                    case MENU_MAIN_COMMAND_CONFIG:
                    case MENU_MAIN_COMMAND_PHS:
                    case MENU_MAIN_COMMAND_SAVE:
                        SfxPreloadAndPlayOnChannel5((g_MenuMainCursors.command.row - MENU_MAIN_COMMAND_PHS != 0 ? 0xFFFFFE3E : 0) + 0x1C3);
                        Menu_SetPhase(MENU_PHASE_OPEN_SCREEN, g_MenuCommandScreenIdMap[g_MenuMainCursors.command.row]);
                        Menu_SetScreen(g_MenuCommandScreenIdMap[g_MenuMainCursors.command.row]);
                        g_MenuMainCursorIndex = MENU_MAIN_CURSOR_COMMAND;
                        break;
                    case MENU_MAIN_COMMAND_QUIT:
                        GetEngineContext()->pendingWindowEvent = 1;
                    }
                }
                break;
            case MENU_MAIN_CURSOR_PARTY:
                if (gSavemap.party[g_MenuMainCursors.party.row] == PARTY_CHAR_NONE) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                } else {
                    Menu_SetPhase(MENU_PHASE_OPEN_SCREEN, g_MenuCommandScreenIdMap[g_MenuMainCursors.command.row]);
                    Menu_SetScreen(g_MenuCommandScreenIdMap[g_MenuMainCursors.command.row]);
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuPartyMemberIndex = g_MenuMainCursors.party.row;
                    g_MenuMainCursorIndex = MENU_MAIN_CURSOR_COMMAND;
                }
                break;
            case MENU_MAIN_CURSOR_ORDER:
                if (g_MenuOrderSwapPicked == 0) {
                    g_MenuCursorPartySlot = (char)g_MenuMainCursors.swap.row;
                    g_MenuOrderSwapPicked = 1;
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                } else {
                    g_MenuOrderSwapPicked = 0;
                    if (g_MenuCursorPartySlot == g_MenuMainCursors.swap.row) {
                        if (gSavemap.party[g_MenuCursorPartySlot] != PARTY_CHAR_NONE) {
                            gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[g_MenuCursorPartySlot]]].row ^= 1;
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        }
                    } else {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        memberId = (char)gSavemap.party[g_MenuMainCursors.swap.row];
                        gSavemap.party[g_MenuMainCursors.swap.row] = (char)gSavemap.party[g_MenuCursorPartySlot];
                        gSavemap.party[g_MenuCursorPartySlot] = memberId;
                        *(MenuPartyStatBlock *)(statBlockCur) = *(MenuPartyStatBlock *)((int)g_MenuPartyStats[g_MenuCursorPartySlot].statBlock);
                        *(MenuPartyStatBlock *)((int)g_MenuPartyStats[g_MenuCursorPartySlot].statBlock) = *(MenuPartyStatBlock *)((int)g_MenuPartyStats[g_MenuMainCursors.swap.row].statBlock);
                        *(MenuPartyStatBlock *)((int)g_MenuPartyStats[g_MenuMainCursors.swap.row].statBlock) = *(MenuPartyStatBlock *)(statBlockCur);
                        avatarPtr = g_MenuFontBGraphicsObject.portraitRenderObjects[g_MenuMainCursors.swap.row];
                        g_MenuFontBGraphicsObject.portraitRenderObjects[g_MenuMainCursors.swap.row] = g_MenuFontBGraphicsObject.portraitRenderObjects[g_MenuCursorPartySlot];
                        g_MenuFontBGraphicsObject.portraitRenderObjects[g_MenuCursorPartySlot] = avatarPtr;
                    }
                }
            }
        }
        if (Input_GetPressed(INPUT_CROSS) != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
            switch (*(char *)&g_MenuMainCursorIndex) {
            case MENU_MAIN_CURSOR_COMMAND:
                g_MenuTopLevelCloseState = MENU_TOP_LEVEL_CLOSING;
                g_MenuTransitionStep = 3;
                g_MenuMainScreenInitialized = 0;
                Menu_SetRequestedScreen(0);
                break;
            case MENU_MAIN_CURSOR_PARTY:
                g_MenuMainCursorIndex = MENU_MAIN_CURSOR_COMMAND;
                break;
            case MENU_MAIN_CURSOR_ORDER:
                if (g_MenuOrderSwapPicked == 0) {
                    g_MenuMainCursorIndex = MENU_MAIN_CURSOR_COMMAND;
                }
                g_MenuOrderSwapPicked = 0;
            }
        }
    }
    if (g_MenuIsSmallViewport320x240 != 1) {
        if (Menu_GetPhase() == MENU_PHASE_MAIN && g_MenuTopLevelCloseState == MENU_TOP_LEVEL_ACTIVE) {
            switch (*(char *)&g_MenuMainCursorIndex) {
            case MENU_MAIN_CURSOR_COMMAND:
                Menu_DrawMoreTextArrow2x(454, g_MenuMainCursors.command.row * 26 + 0x12, 9.99999975e-05f);
                break;
            case MENU_MAIN_CURSOR_ORDER:
                if ((flags & 2) != 0 && g_MenuOrderSwapPicked != 0) {
                    Menu_DrawMoreTextArrow2x(-5, g_MenuCursorPartySlot * 120 + 0x4E, 9.99999975e-05f);
                }
            case MENU_MAIN_CURSOR_PARTY:
                if ((flags & 2) != 0) {
                    Menu_DrawMoreTextArrow2x(454, g_MenuMainCursors.command.row * 26 + 0x12, 9.99999975e-05f);
                }
                Menu_DrawMoreTextArrow2x(0, g_MenuMainCursors.list[g_MenuMainCursorIndex].row * 120 + 0x5A, 0.000111000001f);
            }
        }
        Menu_DrawCommandWindow2x(g_MenuMainCursors.command.row);
        posX = 325;
        posY = 168 - g_MenuTransitionTimer * 168 / 10 + 420;
        Menu_DrawEncodedString2x(posX + 0x10, posY + 0x12, (unsigned char *)gSavemap.location, 7, 0.100000001f);
        Menu_SetRectShorts((short *)frameRectangle, posX, posY, 315, 58);
        Menu_DrawWindowFrame2x((EnglishWindowFrame *)frameRectangle, 0.100000001f);
        posX = 783 - g_MenuTransitionTimer * 307 / 10;
        posY = 348;
        Menu_DrawDecimalNumber2x(posX + 0x42, posY + 0x10, Menu_PlaytimeToHoursDigits((int)gSavemap.seconds), 2, 7, 0.200000003f);
        DrawIconGlyph16_2x(posX + 0x57, posY + 0x12, 0xD5, (unsigned int)gSavemap.millisecondCounter < (unsigned int)0x8000 ? 7 : 0, 0.200000003f);
        Menu_DrawDecimalNumberZeroPad2x(posX + 0x61, posY + 0x10, Menu_PlaytimeToMinutesDigits((int)gSavemap.seconds), 2, 7, 0.200000003f);
        DrawIconGlyph16_2x(posX + 0x76, posY + 0x12, 0xD5, 7, 0.200000003f);
        Menu_DrawDecimalNumberZeroPad2x(posX + 0x80, posY + 0x10, Menu_PlaytimeToSecondsDigits((int)gSavemap.seconds), 2, 7, 0.200000003f);
        Menu_DrawEncodedString2x(posX + 2, posY + 0xC, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_TIME * 0x14, 7, 0.200000003f);
        Menu_DrawDecimalNumber2x(posX + 0x20, posY + 0x2E, gSavemap.gil, 10, 7, 0.200000003f);
        Menu_DrawEncodedString2x(posX + 6, posY + 0x2A, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_GIL * 0x14, 7, 0.200000003f);
        Menu_SetRectShorts((short *)frameRectangle, posX - 4, posY, 168, 72);
        Menu_DrawWindowFrame2x((EnglishWindowFrame *)frameRectangle, 0.200000003f);
        posX = g_MenuTransitionTimer * 307 / 10 - 307;
        posY = 24;
        for (slotIdx = 0; slotIdx < 3; slotIdx++) {
            if (gSavemap.party[slotIdx] == PARTY_CHAR_NONE) {
                continue;
            }
            characterSlot = g_CharIdToSaveSlot[gSavemap.party[slotIdx]];
            rowPos = slotIdx * 120 + 36;
            portraitOffset = ((gSavemap.chars[characterSlot].row & 1) == 0) * 48;
            Menu_DrawCharStatBlock2x(posX + 0xAC, rowPos + 0xD, slotIdx, 0.3f);
            Menu_DrawCharPortrait2x(posX + portraitOffset + 0x23, rowPos + 0xB, slotIdx, 0.3f);
            Menu_DrawGaugeBar2x(posX + 0x157, rowPos + 0x36, ((unsigned char)gSavemap.chars[characterSlot].levelProgress) << 1, 12, 0x90802020, 0.3f);
            if (gSavemap.chars[characterSlot].limitBar == 0xFF) {
                gaugeTint = g_MenuColorFormatBitmasks[flags >> 1 & 7];
            } else if ((gSavemap.chars[characterSlot].status & BATTLE_STATUS_SADNESS) != 0) {
                gaugeTint = g_MenuColorFormatBitmasks[0];
            } else if ((gSavemap.chars[characterSlot].status & BATTLE_STATUS_FURY) != 0) {
                gaugeTint = g_MenuColorFormatMask;
            } else {
                gaugeTint = 0x90802050;
            }
            Menu_DrawGaugeBar2x(posX + 0x159, rowPos + 0x5F, gSavemap.chars[characterSlot].limitBar * 116 / 256, 12, gaugeTint, 0.3f);
            Menu_DrawAtlasQuad2x(posX + 0x155, rowPos + 0x36, 0x88, 8, 64, 8, 7, 0, 0, 0.300999999f);
            Menu_DrawAtlasQuad2x(posX + 0x155, rowPos + 0x5F, 0x88, 8, 64, 8, 7, 0, 0, 0.300999999f);
            Menu_DrawEncodedString2x(posX + 0x147, rowPos + 0x21, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_NEXT_LEVEL * 0x14, 7, 0.3f);
            Menu_DrawEncodedString2x(posX + 0x147, rowPos + 0x4A, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_LIMIT_LEVEL * 0x14, 7, 0.3f);
            Menu_DrawDecimalNumber2x(posX + 0x1B2, rowPos + 0x4E, gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[slotIdx]]].limitLevel, 1, 7, 0.3f);
        }
        Menu_SetRectShorts((short *)frameRectangle, posX, posY, 520, 422);
        Menu_DrawWindowFrame2x((EnglishWindowFrame *)frameRectangle, 0.400000006f);
    } else {
        if (Menu_GetPhase() == MENU_PHASE_MAIN && g_MenuTopLevelCloseState == MENU_TOP_LEVEL_ACTIVE) {
            switch (*(char *)&g_MenuMainCursorIndex) {
            case MENU_MAIN_CURSOR_COMMAND:
                Menu_DrawMoreTextArrow1x(237, g_MenuMainCursors.command.row * 13 + 9, 9.99999975e-05f);
                break;
            case MENU_MAIN_CURSOR_ORDER:
                if ((flags & 2) != 0 && g_MenuOrderSwapPicked != 0) {
                    Menu_DrawMoreTextArrow1x(-2, g_MenuCursorPartySlot * 60 + 0x27, 9.99999975e-05f);
                }
            case MENU_MAIN_CURSOR_PARTY:
                if ((flags & 2) != 0) {
                    Menu_DrawMoreTextArrow1x(237, g_MenuMainCursors.command.row * 13 + 9, 9.99999975e-05f);
                }
                Menu_DrawMoreTextArrow1x(0, g_MenuMainCursors.list[g_MenuMainCursorIndex].row * 60 + 0x2D, 0.000111000001f);
            }
        }
        Menu_DrawCommandWindow1x(g_MenuMainCursors.command.row);
        posX = 162;
        posY = 84 - g_MenuTransitionTimer * 84 / 10 + 210;
        Menu_DrawEncodedString1x(posX + 8, posY + 9, (unsigned char *)gSavemap.location, 7, 0.100000001f);
        Menu_SetRectShorts((short *)frameRectangle, posX, posY, 158, 29);
        Menu_DrawWindowFrame1x((EnglishWindowFrame *)frameRectangle, 0.100000001f);
        posX = 400 - g_MenuTransitionTimer * 154 / 10;
        posY = 174;
        Menu_DrawDecimalNumber1x(posX + 0x1B, posY + 8, Menu_PlaytimeToHoursDigits((int)gSavemap.seconds), 2, 7, 0.200000003f);
        DrawIconGlyph16_1x(posX + 0x26, posY + 9, 0xD5, (unsigned int)gSavemap.millisecondCounter < (unsigned int)0x8000 ? 7 : 0, 0.200000003f);
        Menu_DrawDecimalNumberZeroPad1x(posX + 0x2B, posY + 8, Menu_PlaytimeToMinutesDigits((int)gSavemap.seconds), 2, 7, 0.200000003f);
        DrawIconGlyph16_1x(posX + 0x35, posY + 9, 0xD5, 7, 0.200000003f);
        Menu_DrawDecimalNumberZeroPad1x(posX + 0x3A, posY + 8, Menu_PlaytimeToSecondsDigits((int)gSavemap.seconds), 2, 7, 0.200000003f);
        Menu_DrawEncodedString1x(posX + 1, posY + 6, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_TIME * 0x14, 7, 0.200000003f);
        Menu_DrawDecimalNumber1x(posX + 0xA, posY + 0x17, gSavemap.gil, 0xA, 7, 0.200000003f);
        Menu_DrawEncodedString1x(posX + 3, posY + 0x15, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_GIL * 0x14, 7, 0.200000003f);
        Menu_SetRectShorts((short *)frameRectangle, posX - 2, posY, 76, 36);
        Menu_DrawWindowFrame1x((EnglishWindowFrame *)frameRectangle, 0.200000003f);
        posX = g_MenuTransitionTimer * 154 / 10 - 154;
        posY = 12;
        for (slotIdx = 0; slotIdx < 3; slotIdx++) {
            if (gSavemap.party[slotIdx] == PARTY_CHAR_NONE) {
                continue;
            }
            characterSlot = g_CharIdToSaveSlot[gSavemap.party[slotIdx]];
            rowPos = slotIdx * 60 + 18;
            portraitOffset = ((gSavemap.chars[characterSlot].row & 1) == 0) * 24;
            Menu_DrawCharStatBlock1x(posX + 0x56, rowPos + 7, slotIdx, 0.3f);
            Menu_DrawCharPortrait1x(posX + portraitOffset + 0x12, rowPos + 6, slotIdx, 0.3f);
            Menu_DrawGaugeBar1x(posX + 0xAC, rowPos + 0x1B, ((unsigned char)gSavemap.chars[characterSlot].levelProgress), 6, 0x90802020, 0.3f);
            if (gSavemap.chars[characterSlot].limitBar == 0xFF) {
                gaugeTint = g_MenuColorFormatBitmasks[flags >> 1 & 7];
            } else if ((gSavemap.chars[characterSlot].status & BATTLE_STATUS_SADNESS) != 0) {
                gaugeTint = g_MenuColorFormatBitmasks[0];
            } else if ((gSavemap.chars[characterSlot].status & BATTLE_STATUS_FURY) != 0) {
                gaugeTint = g_MenuColorFormatMask;
            } else {
                gaugeTint = 0x90802050;
            }
            Menu_DrawGaugeBar1x(posX + 0xAD, rowPos + 0x30, gSavemap.chars[characterSlot].limitBar * 58 / 256, 6, gaugeTint, 0.3f);
            Menu_DrawAtlasQuad1x(posX + 0xAB, rowPos + 0x1B, 136, 8, 64, 8, 7, 0, 0, 0.300999999f);
            Menu_DrawAtlasQuad1x(posX + 0xAB, rowPos + 0x30, 136, 8, 64, 8, 7, 0, 0, 0.300999999f);
            Menu_DrawEncodedString1x(posX + 0xA4, rowPos + 0x11, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_NEXT_LEVEL * 0x14, 7, 0.3f);
            Menu_DrawEncodedString1x(posX + 0xA4, rowPos + 0x25, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_LIMIT_LEVEL * 0x14, 7, 0.3f);
            Menu_DrawDecimalNumber1x(posX + 0xD9, rowPos + 0x27, gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[slotIdx]]].limitLevel, 1, 7, 0.3f);
        }
        Menu_SetRectShorts((short *)frameRectangle, posX, posY, 260, 211);
        Menu_DrawWindowFrame1x((EnglishWindowFrame *)frameRectangle, 0.400000006f);
    }
    switch (g_MenuTopLevelCloseState) {
    case MENU_TOP_LEVEL_OPENING:
        g_MenuTransitionTimer += g_MenuTransitionStep;
        if (g_MenuTransitionTimer < 10) {
            g_MenuTransitionTimer = 10;
            g_MenuTopLevelCloseState = MENU_TOP_LEVEL_ACTIVE;
        }
        break;
    case MENU_TOP_LEVEL_CLOSING:
        g_MenuTransitionTimer += g_MenuTransitionStep;
        if (g_MenuTransitionTimer > 70) {
            g_MenuTopLevelCloseState = MENU_TOP_LEVEL_CLOSED;
        }
    }
}

/*
 * Initializes top-level menu state once, including repeat rates, scripted input
 * mode, party stat rebuilding, encounter modifiers and main-screen state. Each
 * frame snapshots input, optionally draws the scripted-input indicator and draws
 * the scale-selected popup.
 */
// FUNCTION: FF7 0x006cb56a
int __cdecl Menu_RunTopLevelFrame(int scriptStream)
{
    int positionX;
    unsigned short bits;
    int drawY;
    int w;
    int dstPhase;
    int bounds[2];

    if (g_MenuTopLevelActive == 0) {
        Input_SetRepeatRates(0xC8, 50);
        g_MenuPhase = MENU_PHASE_MAIN;
        g_MenuTopLevelCloseState = MENU_TOP_LEVEL_OPENING;
        g_MenuCommandRow = g_MenuTopLevelCloseState;
        g_MenuSavedCommandRow = g_MenuCommandRow;
        g_MenuPreviousScreenId = MENU_SCREEN_MAIN;
        g_MenuCurrentScreenId = MENU_SCREEN_MAIN;
        g_MenuNonTopLevelScreenFlag = 0;
        g_MenuCommandWindowHeight = (g_MenuIsSmallViewport320x240 != 0 ? -156 : 0) + 312;
        g_MenuTopLevelActive = 1;
        if (scriptStream == 0) {
            Menu_SetPopupPlacement(0, 0, 0);
            g_ScriptedInputActive = 0;
        } else {
            Input_SetScriptedInputStream(scriptStream);
            g_ScriptedInputActive = 1;
        }
        bits = gSavemap.menuVisible;
        bits |= (short)(gSavemap.menuVisibleHigh << 8);
        if ((bits & 1 << MENU_MAIN_COMMAND_MATERIA) != 0) {
            g_MenuMateriaVisible = 1;
        } else {
            g_MenuMateriaVisible = 0;
        }
        for (bits = 0; bits < 3; bits++) {
            if (gSavemap.party[bits] == PARTY_CHAR_NONE) {
                continue;
            }
            Savemap_BuildPartyStatsFromRecord(bits);
            Battle_LoadCombatantFromSavemap((char)bits);
            Party_RecomputeBattleModifiers();
        }
        g_MenuTransitionTimer = 70;
        g_MenuTransitionStep = 0xFFFD;
        g_MenuTopLevelCloseState = MENU_TOP_LEVEL_OPENING;
        g_MenuTopLevelFrameCount = g_MenuTopLevelCloseState;
        Menu_MainScreenInit(0);
        g_MenuModuleId = 0;
    }
    Input_SnapshotToFieldGlobals();
    if (g_ScriptedInputActive != 0 && g_MenuTopLevelCloseState != MENU_TOP_LEVEL_CLOSED && (g_MenuTopLevelFrameCount & 0x10) != 0) {
        if (g_MenuIsSmallViewport320x240 != 1) {
            positionX = 53;
            drawY = 418;
            Menu_DrawEncodedString2x(positionX + 0xD, drawY + 0xD, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_TUTORIAL * 0x14, 4, 0);
            w = Menu_MeasureEncodedString2x(((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_TUTORIAL * 0x14);
            Menu_SetRectShorts((short *)bounds, positionX, drawY, w + 0x1B, 51);
            Menu_DrawWindowFrame2x((EnglishWindowFrame *)bounds, 0);
        } else {
            positionX = 27;
            drawY = 209;
            Menu_DrawEncodedString1x(positionX + 7, drawY + 7, ((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_TUTORIAL * 0x14, 4, 0);
            w = Menu_MeasureEncodedString1x(((unsigned char *)&g_MenuMainLabels) + MENU_LABEL_TUTORIAL * 0x14);
            Menu_SetRectShorts((short *)bounds, positionX, drawY, w + 0xE, 26);
            Menu_DrawWindowFrame1x((EnglishWindowFrame *)bounds, 0);
        }
    }
    if (g_MenuIsSmallViewport320x240 != 1) {
        Menu_DrawPopupMessage2x();
    } else {
        Menu_DrawPopupMessage1x();
    }
    dstPhase = Menu_GetPhase();
    if (dstPhase == MENU_PHASE_OPEN_SCREEN || (dstPhase == MENU_PHASE_SWITCH_SCREEN || dstPhase == MENU_PHASE_CLOSE_SCREEN)) {
        /* Invokes untyped callback slot `g_MenuScreenFrameCallbacks[g_MenuPreviousScreenId]` with its runtime (int) -> int signature. */
        ((int(__cdecl *)(int))g_MenuScreenFrameCallbacks[g_MenuPreviousScreenId])(g_MenuTopLevelFrameCount);
    } else {
        /* Invokes untyped callback slot `g_MenuScreenFrameCallbacks[g_MenuCurrentScreenId]` with its runtime (int) -> int signature. */
        ((int(__cdecl *)(int))g_MenuScreenFrameCallbacks[g_MenuCurrentScreenId])(g_MenuTopLevelFrameCount);
    }
    g_MenuModuleId ^= 1;
    g_MenuTopLevelFrameCount++;
    if (g_MenuTopLevelCloseState == MENU_TOP_LEVEL_CLOSED) {
        g_ScriptedInputActive = 0;
        g_MenuTopLevelActive = 0;
        if (scriptStream == 0 && g_WorldMapInMenu == 0) {
            g_FieldActorState.state.requestCode = FIELD_REQUEST_NONE;
        }
        g_MenuExitRequested = 1;
    }
    return 0;
}

/*
 * Returns the level byte of savemap character record 0 (gSavemap.chars[0].level).
 */
// FUNCTION: FF7 0x006cb8d6
int __cdecl Savemap_GetCloudLevel(int unused)
{
    return gSavemap.chars[0].level;
}

/*
 * Returns the armour record (0x24 bytes in the kernel table) worn by party slot
 * slot, or 0 for an empty slot (0xFF).
 */
// FUNCTION: FF7 0x006cb8e2
int __cdecl Savemap_GetPartyArmourRecord(int slot)
{
    if (gSavemap.party[slot] == PARTY_CHAR_NONE) {
        return 0;
    } else {
        return (int)g_KernelArmorDataArray[gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[slot]]].armor].materiaSlotBytes;
    }
}

/*
 * Returns the weapon record (0x2C bytes in the kernel table) held by party slot
 * slot, or 0 for an empty slot.
 */
// FUNCTION: FF7 0x006cb928
int __cdecl Savemap_GetPartyWeaponRecord(int slot)
{
    if (gSavemap.party[slot] == PARTY_CHAR_NONE) {
        return 0;
    } else {
        return (int)g_KernelWeaponDataArray[gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[slot]]].weapon].materiaSlotTable;
    }
}

/*
 * Returns the address of kernel armour record id (0x24 bytes each from
 * g_KernelArmorDataArray).
 */
// FUNCTION: FF7 0x006cb96e
int __cdecl Kernel_GetArmorRecord(int id)
{
    return (int)g_KernelArmorDataArray[id].headerBytes;
}

/*
 * Returns the address of kernel accessory record id (16 bytes each from
 * g_KernelAccessoryDataArray).
 */
// FUNCTION: FF7 0x006cb97e
int __cdecl Kernel_GetAccessoryRecord(int id)
{
    return (int)&g_KernelAccessoryDataArray[id];
}

/*
 * Returns party slot slot's 0x440-byte stat block at
 * g_MenuPartyStats, or 0 for an empty slot (0xFF).
 */
// FUNCTION: FF7 0x006cb98e
struct MenuPartyStats *__cdecl Savemap_GetPartyStatBlock(int slot)
{
    if (gSavemap.party[slot] == PARTY_CHAR_NONE) {
        return 0;
    } else {
        return &g_MenuPartyStats[slot];
    }
}

/*
 * Returns the name field of the savemap character record for character id charId
 * (record slot from g_CharIdToSaveSlot, 0x84 bytes each in gSavemap.chars).
 */
// FUNCTION: FF7 0x006cb9b8
int __cdecl Savemap_GetCharacterName(int charId)
{
    return (int)gSavemap.chars[g_CharIdToSaveSlot[charId]].name;
}

/*
 * Takes a signed 16-bit amount of HP off one party slot, floors it at zero, and
 * mirrors the result into the savemap.
 */
// FUNCTION: FF7 0x006cb9d2
void __cdecl Savemap_DamageHp(int slot, int amount)
{
    int hp;
    int actor;

    if (gSavemap.party[slot] == PARTY_CHAR_NONE) {
        return;
    }
    actor = g_CharIdToSaveSlot[gSavemap.party[slot]];
    hp = g_MenuPartyStats[slot].hp - amount;
    if (hp < 0) {
        g_MenuPartyStats[slot].hp = 0;
    } else {
        g_MenuPartyStats[slot].hp = (short)hp;
    }
    (gSavemap.chars[actor].hp) =
        g_MenuPartyStats[slot].hp;
}

/*
 * Puts a signed 16-bit amount of HP back on one party slot, capped at that slot's
 * maximum, and mirrors the result into the savemap.
 */
// FUNCTION: FF7 0x006cba6a
void __cdecl Savemap_HealHp(int slot, int amount)
{
    int actor;

    if (gSavemap.party[slot] == PARTY_CHAR_NONE) {
        return;
    }
    actor = g_CharIdToSaveSlot[gSavemap.party[slot]];
    g_MenuPartyStats[slot].hp += (short)amount;
    if (g_MenuPartyStats[slot].hp >
        g_MenuPartyStats[slot].maxHp) {
        g_MenuPartyStats[slot].hp =
            g_MenuPartyStats[slot].maxHp;
    }
    (gSavemap.chars[actor].hp) =
        g_MenuPartyStats[slot].hp;
}

/*
 * Savemap_DamageHp for MP: the same routine on the MP members of both records.
 */
// FUNCTION: FF7 0x006cbb27
void __cdecl Savemap_DamageMp(int slot, int amount)
{
    int hp;
    int actor;

    if (gSavemap.party[slot] == PARTY_CHAR_NONE) {
        return;
    }
    actor = g_CharIdToSaveSlot[gSavemap.party[slot]];
    hp = g_MenuPartyStats[slot].mp - amount;
    if (hp < 0) {
        g_MenuPartyStats[slot].mp = 0;
    } else {
        g_MenuPartyStats[slot].mp = (short)hp;
    }
    gSavemap.chars[actor].mp =
        g_MenuPartyStats[slot].mp;
}

/*
 * Savemap_HealHp for MP: the same routine on the MP members of both records.
 */
// FUNCTION: FF7 0x006cbbbf
void __cdecl Savemap_HealMp(int slot, short amount)
{
    int actor;

    if (gSavemap.party[slot] == PARTY_CHAR_NONE) {
        return;
    }
    actor = g_CharIdToSaveSlot[gSavemap.party[slot]];
    g_MenuPartyStats[slot].mp += amount;
    if (g_MenuPartyStats[slot].mp >
        g_MenuPartyStats[slot].maxMp) {
        g_MenuPartyStats[slot].mp =
            g_MenuPartyStats[slot].maxMp;
    }
    gSavemap.chars[actor].mp =
        g_MenuPartyStats[slot].mp;
}

/*
 * Compares requested gil to current gil as unsigned. When greater, returns all
 * current gil and clears the balance; otherwise subtracts the request and returns
 * it.
 */
// FUNCTION: FF7 0x006cbc7c
int __cdecl Savemap_SpendGil(int amount)
{
    int spent;

    if ((unsigned int)amount > (unsigned int)gSavemap.gil) {
        spent = gSavemap.gil;
        gSavemap.gil = 0;
        return spent;
    } else {
        gSavemap.gil -= amount;
        return amount;
    }
}

/*
 * Adds the supplied value to gil unless the unsigned sum is smaller than the
 * unsigned previous balance, in which case stores 0xFFFFFFFF. Retains the signed
 * addition expression and unsigned wrap comparison.
 */
// FUNCTION: FF7 0x006cbcb9
void __cdecl Savemap_AddGil(int amount)
{
    if ((unsigned int)(gSavemap.gil + amount) < (unsigned int)gSavemap.gil) {
        gSavemap.gil = 0xFFFFFFFF;
    } else {
        gSavemap.gil += amount;
    }
}

/*
 * Returns the party's gil (gSavemap.gil).
 */
// FUNCTION: FF7 0x006cbce9
int __cdecl Savemap_GetGil(void)
{
    return gSavemap.gil;
}

/*
 * Maps a materia id to its colour index: the low nibble of the materia's type byte
 * (g_KernelMateriaDataArray[0].typeNibbleByte, 0x14 per materia) selects a group through
 * g_MenuMateriaTypeGroupMap, which selects the colour through
 * g_MenuMateriaGroupColorMap.
 */
// FUNCTION: FF7 0x006cbcf3
int __cdecl Menu_GetMateriaTypeColorIndex(int materiaId)
{
    return g_MenuMateriaGroupColorMap[g_MenuMateriaTypeGroupMap[(g_KernelMateriaDataArray[(materiaId & 0xFF)].typeNibbleByte & 0xF)]];
}

/*
 * Brings the party's limit breaks up to date: for each of the three party slots it copies the
 * three limit attacks of the character's current limit level, their ids and 0x1C-byte attack
 * records, into the slot's stat record (Menu_RebuildPartyLimitRecords), then clears
 * g_MenuPartyLimitRefreshNeeded, which the PHS screen sets when the party changes. The dispatcher
 * calls it for a new game, a continue, the menu and the battle swirl; the world map, the PHS
 * screen and the menu call it too.
 */
// FUNCTION: FF7 0x006cbd1e
void __cdecl Menu_RefreshPartyLimitTables(void)
{
    Menu_RebuildPartyLimitRecords();
    g_MenuPartyLimitRefreshNeeded = 0;
}

/*
 * Calls Menu_NameEntryModuleUpdate(value).
 */
// FUNCTION: FF7 0x006cbd32
void __cdecl Menu_NameEntryMenuEntryThunk(int value)
{
    Menu_NameEntryModuleUpdate(value);
}

/*
 * Calls Menu_PhsModuleUpdate(value).
 */
// FUNCTION: FF7 0x006cbd43
void __cdecl Menu_PhsMenuEntryThunk(int value)
{
    Menu_PhsModuleUpdate(value);
}

/*
 * Calls Menu_ShopModuleUpdate(value).
 */
// FUNCTION: FF7 0x006cbd54
void __cdecl Menu_ShopMenuEntryThunk(int value)
{
    Menu_ShopModuleUpdate(value);
}

/*
 * Runs one update of the Load menu module and returns its result.
 */
// FUNCTION: FF7 0x006cbd65
int __cdecl Menu_LoadMenuEntryThunk(void)
{
    int val;

    val = Menu_LoadScreenModuleUpdate();
    return val;
}

/*
 * Runs one update of the Save menu module.
 */
// FUNCTION: FF7 0x006cbd78
void __cdecl Menu_SaveMenuEntryThunk(void)
{
    Menu_SaveScreenModuleUpdate();
}

/*
 * Field script service (the dispatcher's field-service mode or the field's map request) that takes
 * the party's materia away into gSavemap.stolenMateria, through Savemap_HarvestAllMateriaToStash:
 * it collects up to 48 materia from the weapon and armour slots of every character in the PHS
 * and from the 200-slot stock; when more are found, a materia replaces one of the lowest kind
 * rank (g_MateriaKindById) if its own rank is not lower. The collected materia are erased from
 * where they were and the 48 entries (empty ones -1) become the stolen list.
 */
// FUNCTION: FF7 0x006cbd82
void __cdecl Script_SvcHarvestMateria(void)
{
    Savemap_HarvestAllMateriaToStash();
}

/*
 * Field script service: returns the stashed materia to the party.
 */
// FUNCTION: FF7 0x006cbd8c
void __cdecl Script_SvcReturnMateria(void)
{
    Savemap_ReturnMateriaStash();
}

/*
 * Field script service: strips character `value` of its equipment.
 */
// FUNCTION: FF7 0x006cbd96
void __cdecl Script_SvcStripCharacter(int value)
{
    Savemap_StripCharacterEquipment(value);
}

/*
 * Field script service: restores party and equipment from stash `value`.
 */
// FUNCTION: FF7 0x006cbda7
void __cdecl Script_SvcRestorePartyAndEquip(int value)
{
    Savemap_RestorePartyAndEquipFromStash(value);
}

/*
 * Field script service: backs up party and equipment to stash `value`.
 */
// FUNCTION: FF7 0x006cbdb8
void __cdecl Script_SvcBackupPartyAndEquip(int value)
{
    Savemap_BackupPartyAndEquipToStash(value);
}

/*
 * Empty. A field script service slot the shipped build left with no body.
 */
// FUNCTION: FF7 0x006cbdc9
void __cdecl Script_SvcUnusedNoOp(void)
{
    /* The original body is empty. */
}

/*
 * Copy each recruited character's level into the eight-byte bank the PHS screen reads.
 * g_MenuCharSlotIndexMapping is the slot-to-character table -- 0, 1, 2, 4, 5, 6, 7, 8
 * -- and the same id is both the bit in the visibility mask and the index into the 0x84-byte
 * savemap records at gSavemap.chars[0].level.
 */
// FUNCTION: FF7 0x006cbdce
void __cdecl Savemap_SnapshotLevelsToBank(void)
{
    int i;

    for (i = 0; i < 8; i++) {
        if ((gSavemap.phsVisibilityMask & (1 << g_MenuCharSlotIndexMapping[i])) != 0) {
            gSavemap.levelSnapshot[i] = gSavemap.chars[g_MenuCharSlotIndexMapping[i]].level;
        }
    }
}

/*
 * Field script service: scales the first five characters' HP (the
 * Savemap_ScaleFirstFiveHp routine).
 */
// FUNCTION: FF7 0x006cbe33
void __cdecl Script_SvcScaleFirstFiveHp(void)
{
    Savemap_ScaleFirstFiveHp();
}

/*
 * Field script service: checks whether materia set `value` is complete.
 */
// FUNCTION: FF7 0x006cbe3d
void __cdecl Script_SvcCheckMateriaSet(int value)
{
    Savemap_CheckMateriaSetComplete(value);
}

/*
 * Field script service for FIELD_REQUEST_REPLACE_MATERIA_GROUP (the dispatcher's field-service
 * mode or the field's map request, with the request's parameter as `value`): trades mastered
 * materia for a master materia (Savemap_ExchangeForMasterMateria). Set 0 takes one mastered copy
 * of each of the 21 magic materia, if the party has it, and gives MATERIA_MASTER_MAGIC; set 1 the
 * 16 summons for MATERIA_MASTER_SUMMON; set 2 the 7 command materia for MATERIA_MASTER_COMMAND;
 * set 3 takes nothing and gives MATERIA_BAHAMUT_ZERO. Other values do nothing. The trade itself
 * does not check that the set is complete.
 */
// FUNCTION: FF7 0x006cbe4e
void __cdecl Script_SvcReplaceMateriaGroup(int value)
{
    Savemap_ExchangeForMasterMateria(value);
}

/*
 * Searches 0x140 inventory entries for the supplied nine-bit item ID. Removes the
 * requested seven- bit quantity from the first match when less than stock,
 * otherwise removes the whole entry, returning the encoded removed item or -1 when
 * absent.
 */
// FUNCTION: FF7 0x006cbe5f
short __cdecl Savemap_RemoveItem(int itemRef)
{
    int quantity;
    short removed;
    int i;
    int id;

    id = itemRef & 0xFFFF & 0x1FF;
    quantity = (itemRef & 0xFFFF) >> 9;
    removed = 0xFFFF;
    for (i = 0; i < 320; i++) {
        if (gSavemap.items[i] != 0xFFFF && id == (gSavemap.items[i] & 0x1FF)) {
            if (quantity < gSavemap.items[i] >> 9) {
                removed = itemRef & 0xFFFF | quantity << 9;
                gSavemap.items[i] = (gSavemap.items[i] >> 9) - quantity << 9 | id;
            } else {
                removed = gSavemap.items[i];
                gSavemap.items[i] = 0xFFFF;
            }
            return (short)removed;
        }
    }
    return (short)removed;
}

/*
 * Scans the 0x140 savemap item words for the first nonempty entry whose low nine
 * bits match itemRef. Returns that complete stored word, including its quantity bits,
 * or 0xFFFF if absent; the quantity bits from itemRef do not affect the search.
 */
// FUNCTION: FF7 0x006cbf57
unsigned short __cdecl Savemap_FindItem(unsigned short itemRef)
{
    short result;
    int idx;
    int count;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int itemId;

    itemId = itemRef & 0x1FF;
    count = (itemRef) >> 9;
    result = 0xFFFF;
    for (idx = 0; idx < 320; idx++) {
        if (gSavemap.items[idx] != 0xFFFF && itemId == (gSavemap.items[idx] & 0x1FF)) {
            result = gSavemap.items[idx];
            return (short)result;
        }
    }
    return (short)result;
}

/*
 * Plays sound effect 0x19F (SFX_MENU_MATERIA_EQUIP) on channel 5.
 */
// FUNCTION: FF7 0x006cbfe8
void __cdecl Menu_PlayMateriaEquipSfx(void)
{
    SfxPreloadAndPlayOnChannel5(SFX_MENU_MATERIA_EQUIP);
}

/*
 * Extracts item id and quantity from the low 16 bits of itemRef. Searches 0x140 item
 * words for an existing nonempty id match, adds its quantity and caps the combined
 * quantity at 99, then stores the packed word.
 */
// FUNCTION: FF7 0x006cbffa
void __cdecl Savemap_AddItem(int itemRef)
{
    int quantity;
    int i;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int id;

    id = itemRef & 0xFFFF & 0x1FF;
    quantity = (itemRef & 0xFFFF) >> 9;
    for (i = 0; i < 320; i++) {
        if (gSavemap.items[i] != 0xFFFF && id == (gSavemap.items[i] & 0x1FF)) {
            quantity += gSavemap.items[i] >> 9;
            if (quantity > 99) {
                quantity = 99;
            }
            gSavemap.items[i] = quantity << 9 | id;
            return;
        }
    }
    for (i = 0; i < 320; i++) {
        if (gSavemap.items[i] == 0xFFFF) {
            gSavemap.items[i] = itemRef;
            return;
        }
    }
}

/*
 * Stores materia into the first of 200 stock entries equal to -1 and returns -1 on
 * success. Sets acquisition flag one when its low-byte materia color index is 0xA
 * and flag two for low-byte ID0x2C.
 */
// FUNCTION: FF7 0x006cc0ea
int __cdecl Savemap_AddMateria(int materia)
{
    int i;

    for (i = 0; i < 200; i++) {
        if (gSavemap.materia[i] == -1) {
            gSavemap.materia[i] = materia;
            if (Menu_GetMateriaTypeColorIndex(materia & 0xff) == 0xa) {
                gSavemap.magicMenuFlags |= 1;
            }
            if ((materia & 0xff) == 0x2c) {
                gSavemap.magicMenuFlags |= 2;
            }
            return -1;
        }
    }
    return materia;
}

/*
 * Empties the menu's key queue (g_MenuNewMateriaCount = 0).
 */
// FUNCTION: FF7 0x006cc173
void __cdecl Savemap_ClearNewMateriaQueue(void)
{
    g_MenuNewMateriaCount = 0;
}

/*
 * Stores materia in the first free stock slot, updates special acquisition flags
 * and invokes the acquisition helper. When stock is full, selects the entry with
 * the smallest byte ranking and replaces its first exact encoded match; this
 * replacement path skips the acquisition flags/helper.
 */
// FUNCTION: FF7 0x006cc17f
void __cdecl Savemap_AddMateriaToStock(int materia)
{
    int idx;
    int victim;
    unsigned char rank;

    for (idx = 0; idx < 200; idx++) {
        if (gSavemap.materia[idx] == -1) {
            gSavemap.materia[idx] = materia;
            if (Menu_GetMateriaTypeColorIndex(materia & 0xFF) == 0xA) {
                gSavemap.magicMenuFlags |= 1;
            }
            if ((materia & 0xFF) == 0x2C) {
                gSavemap.magicMenuFlags |= 2;
            }
            Menu_QueueAcquiredMateriaId(materia & 0xFF);
            return;
        }
    }
    victim = 0xFFFFFFFF;
    rank = 0xFF;
    for (idx = 0; idx < 200; idx++) {
        if (g_MenuMateriaSortPriority[(gSavemap.materia[idx] & 0xFF)] < rank) {
            rank = (char)g_MenuMateriaSortPriority[gSavemap.materia[idx] & 0xFF];
            victim = gSavemap.materia[idx];
        }
    }
    for (idx = 0; idx < 200; idx++) {
        if (gSavemap.materia[idx] == victim) {
            gSavemap.materia[idx] = materia;
            return;
        }
    }
}

/*
 * Writes the supplied low byte into the menu queue using the full
 * g_MenuNewMateriaCount index, then increments only the low byte of that count.
 */
// FUNCTION: FF7 0x006cc2c9
void __cdecl Menu_QueueAcquiredMateriaId(int materiaId)
{
    g_BattleAcquiredMateriaIds[g_MenuNewMateriaCount] = materiaId;
    g_MenuNewMateriaCount++;
}

/*
 * Always 0: a menu hook with no work in the shipped build.
 */
// FUNCTION: FF7 0x006cc2ed
int __cdecl Savemap_RemoveMateriaReturnZero(int materia, int count)
{
    return 0;
}

/*
 * Always 0: a menu hook with no work in the shipped build.
 */
// FUNCTION: FF7 0x006cc2f4
int __cdecl Savemap_CountMateriaReturnZero(int materia, int count)
{
    return 0;
}

/*
 * Menu_ReleaseBatchLists with the party loop doing its real work: release the nine COM
 * resources, then rebuild each present member's stats, combatant record and
 * portrait. The frame is 0x2228 and only twelve bytes of it are read, so 0x221C
 * bytes above the counters are declared and never touched -- big enough that VC5
 * emits the `__chkstk` probe for it.
 */
// FUNCTION: FF7 0x006cc2fb
void __cdecl Menu_ReleasePortraitsAndReloadPartyMembers(void)
{
    char scratch[0x221c];
    int party;
    int i;
    unsigned char actor;

    for (i = 0; i < 9; i++) {
        if (g_MenuCharPortraitGraphicsObjects[i] != 0) {
            DestroyRenderObjectAndClear(&g_MenuCharPortraitGraphicsObjects[i]);
        }
    }
    for (party = 0; party < 3; party++) {
        actor = gSavemap.party[party];
        if (actor == 0xff) {
            continue;
        }
        Savemap_BuildPartyStatsFromRecord(party);
        Battle_LoadCombatantFromSavemap((unsigned char)party);
        Party_RecomputeBattleModifiers();
        Menu_LoadCharPortrait(actor, party);
    }
}

/*
 * For each of three present party members, rebuilds its stat record, reloads its
 * battle combatant and recomputes encounter modifiers inside the loop. Empty 0xFF
 * slots are skipped; unreferenced padding locals preserve VC5 /Od stack layout.
 */
// FUNCTION: FF7 0x006cc3e8
void __cdecl Savemap_ReloadActivePartyStats(void)
{
    int padSlot[3]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int partySlot;
    unsigned char charId;

    for (partySlot = 0; partySlot < 3; partySlot++) {
        charId = (char)gSavemap.party[partySlot];
        if (charId == PARTY_CHAR_NONE) {
            continue;
        }
        Savemap_BuildPartyStatsFromRecord(partySlot);
        Battle_LoadCombatantFromSavemap((char)partySlot);
        Party_RecomputeBattleModifiers();
    }
}

/*
 * Restores the config defaults in gSavemap: clears g_MenuPartyLimitRefreshNeeded, rebuilds the limit
 * tables, reloads the active party stats, sets battleSpeed, battleMessageSpeed, and fieldMessageSpeed
 * to 0x80, options to 0x41, and controllerMap[0..15] to the identity mapping 0..15.
 */
// FUNCTION: FF7 0x006cc446
void __cdecl Savemap_ResetConfigDefaults(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    g_MenuPartyLimitRefreshNeeded = 0;
    Menu_RefreshPartyLimitTables();
    Savemap_ReloadActivePartyStats();
    gSavemap.battleSpeed = 128;
    gSavemap.battleMessageSpeed = 128;
    gSavemap.fieldMessageSpeed = 128;
    gSavemap.options = 0x41;
    for (q = 0; q < 16; q++) {
        gSavemap.controllerMap[q] = q;
    }
}

/*
 * For each occupied party slot whose mapped accessory id is 0x1B, heals 3 HP. If
 * its battle-side current and maximum HP words then differ, clears bit 0 in both
 * the battle-side current HP and savemap HP words.
 */
// FUNCTION: FF7 0x006cc4a8
void __cdecl Party_TickAccessoryRegen(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (gSavemap.party[i] != PARTY_CHAR_NONE) {
            if (gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[i]]].accessory +
                    0x120 ==
                0x13b) {
                Savemap_HealHp(i, 3);
                if (g_MenuPartyStats[i].hp !=
                    g_MenuPartyStats[i].maxHp) {
                    g_MenuPartyStats[i].hp &= 0xfffe;
                    gSavemap.chars[gSavemap.party[i]].hp &= 0xfffe;
                }
            }
        }
    }
}

/*
 * Release the nine COM resources at g_MenuCharPortraitGraphicsObjects, then walk the
 * party and do nothing with it. The second loop really is that: it reads the slot, tests it against
 * 0xFF and continues either way.
 */
// FUNCTION: FF7 0x006cc5a2
void __cdecl Menu_ReleaseBatchLists(void)
{
    int scratch[2];
    int i;
    int actor;

    for (i = 0; i < 9; i++) {
        if (g_MenuCharPortraitGraphicsObjects[i] != 0) {
            DestroyRenderObjectAndClear(&g_MenuCharPortraitGraphicsObjects[i]);
        }
    }
    for (i = 0; i < 3; i++) {
        actor = gSavemap.party[i];
        if (actor == 0xff) {
            continue;
        }
    }
}

/*
 * The menu module's mouse-message callback. It is empty: the menus take no mouse input and read
 * the controls each frame in Menu_ModuleFrame instead.
 */
// FUNCTION: FF7 0x006cc619
void __cdecl Menu_ModuleMouse(void)
{
    /* The original body is empty. */
}

/*
 * The menu module's keyboard-message callback. It is empty: key presses reach the menus only
 * through the per-frame input update in Menu_ModuleFrame.
 */
// FUNCTION: FF7 0x006cc61e
void __cdecl Menu_ModuleKeyboard(void)
{
    /* The original body is empty. */
}

/*
 * Runs the menu module frame: optionally throttles, timestamps, updates input
 * unless blocked, updates engine resources, clears the background and
 * dispatches/submits the menu screen within a successful scene. When
 * g_MenuFrameThrottleFlag is set, the first pass disables text, then clears that
 * flag, enables text, advances context time by one and performs a second
 * input/resource/screen pass before ending the same scene.
 */
// FUNCTION: FF7 0x006cc623
void __cdecl Menu_ModuleFrame(struct GfxEngineContext *ctx)
{
    char p[200];
    GfxClearColorBgra color;
    int sceneFlag;
    if (g_MenuFrameThrottleArmed != 0) {
        Menu_ThrottleFrameRate(ctx);
    }
    if (g_MenuFrameThrottleFlag != 0) {
        g_MenuFrameThrottleArmed = 0;
    } else {
        g_MenuFrameThrottleArmed = 1;
    }
    ReadTimestampCounter(&g_MenuPrevFrameTimestamp);
    sceneFlag = 1;
    if (g_ScriptedInputActive == 0) {
        Input_Update(0);
    }
    Gfx_AdvanceDrawPoolGeneration(ctx->drawPool);
    Gfx_ResetRenderObjectPool(ctx->renderObjectBank);
    color.r = 0;
    color.g = 0;
    color.b = 0;
    color.a = FLOAT_ONE_BITS;
    GfxSetBackgroundColorCall(&color, ctx);
    GfxClearCall(g_MenuFrameClearFlag, 1, 0, ctx);
    if (((int (__cdecl *)(int, struct GfxEngineContext *))Gfx_BeginSceneCall)(0, ctx)) { /* reads eax after a void call, as the original does */
        if (g_MenuFrameThrottleFlag != 0) {
            Menu_SetTextSubsystemReady(0);
        } else {
            Menu_SetTextSubsystemReady(1);
        }
        Menu_DispatchScreenUpdate();
        Menu_SubmitDrawLists();
        if (1 && g_MenuFrameThrottleFlag != 0) {
            g_MenuFrameThrottleArmed = 1;
            g_MenuFrameThrottleFlag = 0;
            Menu_SetTextSubsystemReady(1);
            ctx->framesThisSecond += 1.0;
            Gfx_AdvanceDrawPoolGeneration(ctx->drawPool);
            Gfx_ResetRenderObjectPool(ctx->renderObjectBank);
            ReadTimestampCounter(&g_MenuPrevFrameTimestamp);
            if (g_ScriptedInputActive == 0) {
                Input_Update(0);
            }
            Menu_DispatchScreenUpdate();
            Menu_SubmitDrawLists();
        }
        Gfx_EndSceneCall(ctx);
    }
    Gfx_StatsClearRecord(ctx->statsRecord);
    if (g_MenuExitRequested != 0) {
        if (g_MenuBattleResultFinished != 0) {
            g_EngineGameMode = (short)g_BattleResultMenuNextMode;
            g_MenuBattleResultFinished = 0;
            Menu_FreePartyPortraits();
        } else {
            switch (g_EngineGameMode) {
            case GAME_MODE_TITLE_MENU:
                g_EnginePreviousMode = GAME_MODE_TITLE_MENU;
                if (g_MenuTitleNewGameChosen != 0) {
                    g_EngineGameMode = GAME_MODE_NEW_GAME;
                    g_MenuTitleNewGameChosen = 0;
                } else {
                    g_EngineGameMode = GAME_MODE_CONTINUE_GAME;
                }
                break;
            case GAME_MODE_MENU:
                Savemap_ReconcileFieldParty();
                (g_FieldActorState.state.requestArg) = 2;
                g_EnginePreviousMode = GAME_MODE_MENU;
                g_MenuBattleResultFinished = 0;
                g_EngineGameMode = GAME_MODE_FIELD;
                switch (g_FieldActorState.state.requestCode) {
                case FIELD_REQUEST_NAME_ENTRY:
                case FIELD_REQUEST_PARTY_SELECT:
                    Menu_FreeAllCharPortraits();
                    break;
                }
                Menu_FreePartyPortraits();
                break;
            case GAME_MODE_WORLD_MAP:
                Menu_FreePartyPortraits();
                break;
            }
        }
        g_MenuExitRequested = 0;
        GetActiveModule((GameModule *)(&g_MenuExitModuleInitFn), ctx);
        if (g_WorldMapInMenu != 0) {
            g_MenuExitModuleEnterFn = WorldMap_ModuleEnter;
            g_MenuExitModuleMouseFn = (GameModuleMessageCallback)WorldMap_ModuleMouse;
            g_MenuExitModuleKeyFn = (GameModuleMessageCallback)WorldMap_ModuleKeyboard;
            g_MenuExitModuleFrameFn = WorldMap_ModuleFrame;
            g_MenuExitModuleExitFn = (GameModuleCallback)WorldMap_ModuleExit;
            RequestModuleSwitch((GameModule *)(&g_MenuExitModuleInitFn), ctx);
        } else {
            g_MenuExitModuleEnterFn = GameModeDispatcher_Enter;
            g_MenuExitModuleMouseFn = App_OnMouseMessage;
            g_MenuExitModuleKeyFn = App_OnKeyMessage;
            g_MenuExitModuleFrameFn = GameModeDispatcher_Frame;
            g_MenuExitModuleExitFn = GameModeDispatcher_Exit;
            RequestModuleSwitch((GameModule *)(&g_MenuExitModuleInitFn), ctx);
        }
    }
    ReadTimestampCounter(&g_MenuFrameTimestamp);
    Sub64(&g_MenuFrameTimestamp, &g_MenuPrevFrameTimestamp, &g_MenuTickResidue);
    g_MenuElapsedTicks = CombineSplitValueToFloat(&g_MenuTickResidue);
    SoundUpdateFades();
}

/*
 * Returns unless text is ready, then submits menu lists in two scene passes. Pass
 * 0 draws the window object and text list plus seven element lists for scale other
 * than 1, or three for scale 1.
 */
// FUNCTION: FF7 0x006cc9d3
void __cdecl Menu_SubmitDrawLists(void)
{
    GfxEngineContext *ctx;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    ctx = GetEngineContext();
    Gfx_SetScenePassCall(0, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_DrawRenderObjectCall(g_MenuFlatRectGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList((void *)g_MenuWindowBgGraphicsObject, ctx);
    if (g_MenuIsSmallViewport320x240 != 1) {
        Gfx_EmitPrimitiveBatchList(g_MenuWinABlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuWinCBlend4DiffGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinBBlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinDBlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuFontAGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuFontBGraphicsObject.batchList, ctx);
    } else {
        Gfx_EmitPrimitiveBatchList((void *)g_MenuWinBlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuWinCBlend4DiffGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuFontBlend4GraphicsObject, ctx);
    }
    if (g_MenuCurrentScreenId == MENU_SCREEN_PHS || g_MenuOverlayActive != 0) {
        Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[0], ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[1], ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[2], ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[3], ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[4], ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[5], ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[6], ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[7], ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[8], ctx);
    } else {
        Gfx_EmitPrimitiveBatchList(g_MenuFontBGraphicsObject.portraitRenderObjects[0], ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuFontBGraphicsObject.portraitRenderObjects[1], ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuFontBGraphicsObject.portraitRenderObjects[2], ctx);
    }
    if (g_EngineGameMode == GAME_MODE_TITLE_MENU) {
        Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 1, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuBusterTexGraphicsObject, ctx);
        InvalidateBatchGeneration(g_MenuBusterTexGraphicsObject);
    }
    Gfx_SetScenePassCall(1, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuBandGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList((void *)g_MenuBlendWindowBgGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuGaugeBarGraphicsObject, ctx);
    if (g_MenuIsSmallViewport320x240 != 1) {
        Gfx_EmitPrimitiveBatchList(g_MenuWinABlend0GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinABlend1GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend1GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinBBlend1GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinDBlend1GraphicsObject, ctx);
    } else {
        Gfx_EmitPrimitiveBatchList(g_MenuWinBlend0GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuWinBlend1GraphicsObject, ctx);
    }
    Gfx_SetRenderStateCall(RENDER_STATE_DITHER, 1, ctx);
    Gfx_EmitPrimitiveBatchList((void *)g_MenuTextBoxQuadGraphicsObject, ctx);
    GfxSetViewportRect(g_MenuViewportX, g_MenuViewportY, g_MenuViewportWidth, g_MenuViewportHeight, (GfxEngineContext *)ctx);
}

/*
 * Copies the NUL-terminated input with each unsigned byte reduced by 0x20 and
 * writes destination terminator 0xFF. Input NUL is not copied.
 */
// FUNCTION: FF7 0x006ccd60
void __cdecl Menu_DebugAsciiToFF7Text(char *dst, char *src)
{
    int val;

    val = 0;
    while (src[val]) {
        dst[val] = (unsigned char)src[val] - 0x20;
        val++;
    }
    dst[val] = -1;
}

/*
 * Obtains the engine context and increments gSavemapSeconds whenever the truncated
 * context+0x28 double is divisible by 60. Draws three menu debug counters,
 * including the signed low byte of g_BattleResultScreenState, in the selected display
 * scale.
 */
// FUNCTION: FF7 0x006ccda5
void __cdecl Menu_DrawDebugCounters(void)
{
    struct {
        char ascii[20];
        char encoded[20];
        GfxEngineContext *context;
        int framePad[2];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.context = GetEngineContext();
    gSavemap.seconds += (int)frame.context->frameCount % 60 == 0;
    if (g_MenuIsSmallViewport320x240 != 1) {
        Menu_DrawDecimalNumber2x(320, 0, g_MenuMainCursors.command.row, 2, 7, 0);
        Menu_DrawDecimalNumber2x(360, 0, g_BattleResultScreenState, 10, 7, 0);
        Menu_DrawDecimalNumber2x(600, 0, g_MenuRequestedNextScreen, 2, 6, 0);
    } else {
        Menu_DrawDecimalNumber1x(160, 0, g_MenuMainCursors.command.row, 2, 7, 0);
        Menu_DrawDecimalNumber1x(180, 0, g_BattleResultScreenState, 0xA, 7, 0);
        Menu_DrawDecimalNumber1x(300, 0, g_MenuRequestedNextScreen, 2, 6, 0);
    }
    if (g_MenuIsSmallViewport320x240 != 1) {
        itoa(g_MenuTopLevelCloseState, frame.ascii, 0xA);
        Menu_DebugAsciiToFF7Text(frame.encoded, frame.ascii);
        Menu_DrawEncodedString2x(0, 240, (unsigned char *)frame.encoded, 0, 0);
        itoa(g_MenuTransitionStep, frame.ascii, 0xA);
        Menu_DebugAsciiToFF7Text(frame.encoded, frame.ascii);
        Menu_DrawEncodedString2x(0, 270, (unsigned char *)frame.encoded, 0, 0);
        itoa(g_MenuCommandWindowAnimStep, frame.ascii, 0xA);
        Menu_DebugAsciiToFF7Text(frame.encoded, frame.ascii);
        Menu_DrawEncodedString2x(0, 130, (unsigned char *)frame.encoded, 0, 0);
        itoa(g_MenuPhase, frame.ascii, 0xA);
        Menu_DebugAsciiToFF7Text(frame.encoded, frame.ascii);
        Menu_DrawEncodedString2x(0, 190, (unsigned char *)frame.encoded, 0, 0);
        itoa(g_MenuTransitionTimer, frame.ascii, 0xA);
        Menu_DebugAsciiToFF7Text(frame.encoded, frame.ascii);
        Menu_DrawEncodedString2x(0, 310, (unsigned char *)frame.encoded, 0, 0);
        Menu_DrawDecimalNumber2x(0, 0, (int)(frame.context->fps + 0.5), 2, 7, 0);
    } else {
        itoa(g_MenuTopLevelCloseState, frame.ascii, 0xA);
        Menu_DebugAsciiToFF7Text(frame.encoded, frame.ascii);
        Menu_DrawEncodedString1x(0, 120, (unsigned char *)frame.encoded, 0, 0);
        itoa(g_MenuTransitionStep, frame.ascii, 0xA);
        Menu_DebugAsciiToFF7Text(frame.encoded, frame.ascii);
        Menu_DrawEncodedString1x(0, 135, (unsigned char *)frame.encoded, 0, 0);
        itoa(g_MenuCommandWindowAnimStep, frame.ascii, 0xA);
        Menu_DebugAsciiToFF7Text(frame.encoded, frame.ascii);
        Menu_DrawEncodedString1x(0, 65, (unsigned char *)frame.encoded, 0, 0);
        itoa(g_MenuPhase, frame.ascii, 0xA);
        Menu_DebugAsciiToFF7Text(frame.encoded, frame.ascii);
        Menu_DrawEncodedString1x(0, 95, (unsigned char *)frame.encoded, 0, 0);
        itoa(g_MenuTransitionTimer, frame.ascii, 0xA);
        Menu_DebugAsciiToFF7Text(frame.encoded, frame.ascii);
        Menu_DrawEncodedString1x(0, 155, (unsigned char *)frame.encoded, 0, 0);
        Menu_DrawDecimalNumber1x(0, 0, (int)(frame.context->fps + 0.5), 2, 7, 0);
    }
}

/*
 * Loads each of three present party-member portraits into its corresponding slot,
 * skipping IDs 0xFF. Uses character ID and party-slot index as separate loader
 * arguments.
 */
// FUNCTION: FF7 0x006cd13a
void __cdecl Menu_LoadPartyPortraits(void)
{
    int partySlot;
    unsigned char charId;

    for (partySlot = 0; partySlot < 3; partySlot++) {
        charId = (char)gSavemap.party[partySlot];
        if (charId == PARTY_CHAR_NONE) {
            continue;
        }
        Menu_LoadCharPortrait(charId, partySlot);
    }
}

/*
 * Releases the three party portrait render objects in g_MenuFontBGraphicsObject.portraitRenderObjects
 * that are non-null.
 */
// FUNCTION: FF7 0x006cd191
void __cdecl Menu_FreePartyPortraits(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (g_MenuFontBGraphicsObject.portraitRenderObjects[i] != 0) {
            DestroyRenderObjectAndClear(&g_MenuFontBGraphicsObject.portraitRenderObjects[i]);
        }
    }
}

/*
 * Invalidates eleven layout batch lists when GetUiScalingMode is 2 or four
 * alternate lists otherwise, then invalidates shared lists and resets the window
 * object through Gfx_EmptyRenderObjectBatch. Finally invalidates each nonzero
 * handle in the three-portrait and nine-element arrays.
 */
// FUNCTION: FF7 0x006cd1d3
void __cdecl Menu_ResetUiBatches(void)
{
    int i;

    if (GetUiScalingMode() == 2) {
        InvalidateBatchGeneration(g_MenuFontAGraphicsObject);
        InvalidateBatchGeneration(g_MenuFontBGraphicsObject.batchList);
        InvalidateBatchGeneration(g_MenuWinABlend4GraphicsObject);
        InvalidateBatchGeneration(g_MenuWinBBlend4GraphicsObject);
        InvalidateBatchGeneration(g_MenuWinCBlend4GraphicsObject);
        InvalidateBatchGeneration(g_MenuWinDBlend4GraphicsObject);
        InvalidateBatchGeneration(g_MenuWinABlend0GraphicsObject);
        InvalidateBatchGeneration(g_MenuWinABlend1GraphicsObject);
        InvalidateBatchGeneration(g_MenuWinBBlend1GraphicsObject);
        InvalidateBatchGeneration(g_MenuWinCBlend1GraphicsObject);
        InvalidateBatchGeneration(g_MenuWinDBlend1GraphicsObject);
    } else {
        InvalidateBatchGeneration(g_MenuFontBlend4GraphicsObject);
        InvalidateBatchGeneration(g_MenuWinBlend4GraphicsObject);
        InvalidateBatchGeneration(g_MenuWinBlend0GraphicsObject);
        InvalidateBatchGeneration(g_MenuWinBlend1GraphicsObject);
    }
    InvalidateBatchGeneration(g_MenuWinCBlend4DiffGraphicsObject);
    InvalidateBatchGeneration(g_MenuWindowBgGraphicsObject);
    InvalidateBatchGeneration(g_MenuBlendWindowBgGraphicsObject);
    InvalidateBatchGeneration(g_MenuGaugeBarGraphicsObject);
    Gfx_EmptyRenderObjectBatch((int)g_MenuFlatRectGraphicsObject);
    InvalidateBatchGeneration(g_MenuBandGraphicsObject);
    InvalidateBatchGeneration(g_MenuBattleLimitGraphicsObject);
    InvalidateBatchGeneration(g_MenuTextBoxQuadGraphicsObject);
    for (i = 0; i < 3; i++) {
        if (g_MenuFontBGraphicsObject.portraitRenderObjects[i] != 0) {
            InvalidateBatchGeneration(g_MenuFontBGraphicsObject.portraitRenderObjects[i]);
        }
    }
    for (i = 0; i < 9; i++) {
        if (g_MenuCharPortraitGraphicsObjects[i] != 0) {
            InvalidateBatchGeneration(g_MenuCharPortraitGraphicsObjects[i]);
        }
    }
}

/*
 * Logs menu entry, sets channel 5 volume to 127, enables g_MenuModuleActive and
 * loads the font window. Loads party portraits for menu mode 4 and selected mode 5
 * or mode 3 cases.
 */
// FUNCTION: FF7 0x006cd3b0
void __cdecl Menu_ModuleEnter(struct GfxEngineContext *ctx)
{
    int val;
    Gfx_DebugPrint(
        // STRING: FF7 0x0091ac7c
        "START OF MENU SYSTEM!!!\n");
    SfxSetChannelVolume(127, 5);
    g_MenuModuleActive = 1;
    Menu_LoadFontAndWindowTextures(1);
    if (g_MenuEntryState == MENU_ENTRY_BATTLE_RESULT) {
        Menu_LoadPartyPortraits();
    } else {
        switch (g_EngineGameMode) {
        case GAME_MODE_MENU:
            switch (g_FieldActorState.state.requestCode) {
            case FIELD_REQUEST_MAIN_MENU:
                Menu_LoadPartyPortraits();
                break;
            }
            break;
        case GAME_MODE_WORLD_MAP:
            Menu_LoadPartyPortraits();
            break;
        }
    }
    g_MenuFrameDuration = ctx->tscFrequency / g_MenuTargetFps;
    val = GetUiScalingMode();
    if (val == MAIN_GRAPHICS_MODE_320X240) {
        g_MenuViewportX = 0;
        g_MenuViewportY = 0;
        g_MenuViewportWidth = 320;
        g_MenuViewportHeight = 240;
        g_MenuIsSmallViewport320x240 = 1;
    } else if (val == MAIN_GRAPHICS_MODE_320X240_CENTERED) {
        g_MenuViewportX = 160;
        g_MenuViewportY = 120;
        g_MenuViewportWidth = 320;
        g_MenuViewportHeight = 240;
        g_MenuIsSmallViewport320x240 = 1;
    } else {
        g_MenuViewportX = 0;
        g_MenuViewportY = 0;
        g_MenuViewportWidth = 640;
        g_MenuViewportHeight = 480;
        g_MenuIsSmallViewport320x240 = 0;
    }
    switch (g_EngineGameMode) {
    case GAME_MODE_TITLE_MENU:
        g_MenuFrameClearFlag = 1;
        break;
    case GAME_MODE_MENU:
        switch (g_FieldActorState.state.requestCode) {
        case FIELD_REQUEST_PARTY_SELECT:
            Menu_ClearPartyChangeScreenActiveFlag();
        case FIELD_REQUEST_NAME_ENTRY:
            g_MenuFrameClearFlag = 0;
            break;
        case FIELD_REQUEST_SHOP:
        case FIELD_REQUEST_SAVE_MENU:
            g_MenuFrameClearFlag = 1;
            break;
        }
        break;
    }
    Menu_SetRequestedScreen(1);
    Menu_SetTextSubsystemReady(1);
    g_MenuFrameThrottleFlag = 0;
    g_MenuFrameThrottleArmed = 0;
    g_MenuTopLevelActive = 0;
    g_MenuOverlayActive = g_MenuTopLevelActive;
}

/*
 * Tear the menu module down: release the buffer the save and load screens share,
 * then reset the batches, reload the font and window textures, and say so on the
 * debug channel. The buffer is only released when the module is really finishing.
 */
// FUNCTION: FF7 0x006cd5b3
void __cdecl Menu_ModuleExit(void)
{
    if (g_WorldMapInMenu == 0) {
        if (g_EnginePreviousMode == GAME_MODE_TITLE_MENU || g_EngineGameMode == GAME_MODE_TITLE_MENU) {
            if (g_MenuBusterTexGraphicsObject != 0) {
                DestroyRenderObjectAndClear(&g_MenuBusterTexGraphicsObject);
            }
            if (g_EngineGameMode == GAME_MODE_TITLE_MENU) {
                Menu_SaveFileBufferFree();
            }
        } else if (g_EngineGameMode == GAME_MODE_MENU) {
            if (g_FieldActorState.state.requestCode == FIELD_REQUEST_SAVE_MENU || g_MenuCurrentScreenId == MENU_SCREEN_SAVE) {
                Menu_SaveFileBufferFree();
            }
        }
    }
    Menu_ResetUiBatches();
    Menu_LoadFontAndWindowTextures(0);
    g_MenuModuleActive = 0;
    // STRING: FF7 0x0091ac98
    Gfx_DebugPrint("END OF MENU SYSTEM!!!\n");
}

/*
 * When g_MenuFrameThrottleFlag is clear and batch-node acquisition succeeds, fills a
 * full viewport quad using origin
 * g_MenuViewportX/g_MenuViewportY and dimensions
 * g_MenuViewportWidth/g_MenuViewportHeight. Packs RGB zero and
 * alpha alpha into each vertex color, uses depth depth, reciprocal W 1 and black
 * specular fields.
 */
// FUNCTION: FF7 0x006cd64e
void __cdecl Menu_FillViewportQuad(unsigned char alpha, float depth)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        GfxTlVertex *vertexBottomRight;
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *vertexTopRight; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *vertexBottomLeft; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *vertexTopLeft; };
        struct { unsigned char pad_to_10[0x10]; int packedRgb; };
        struct { unsigned char pad_to_11[0x11]; char rgbByte; };
        struct { unsigned char pad_to_12[0x12]; char rgbByteCopy; };
        struct { unsigned char pad_to_13[0x13]; char alphaByte; };
    } frame;
#pragma pack(pop)

    if (g_MenuFrameThrottleFlag != 0) {
        return;
    }
    *(char *)&frame.packedRgb = 0;
    frame.rgbByte = *(char *)&frame.packedRgb;
    frame.rgbByteCopy = frame.rgbByte;
    frame.alphaByte = alpha;
    if (AcquireBatchNode(1, g_MenuTextBoxQuadGraphicsObject) != 0) {
        frame.vertexTopLeft = (GfxTlVertex *)((GfxRenderObject *)g_MenuTextBoxQuadGraphicsObject)->vertexCursor;
        frame.vertexTopLeft->x = (float)g_MenuViewportX + 0.0f;
        frame.vertexTopLeft->y = (float)g_MenuViewportY + 0.0f;
        frame.vertexTopLeft->z = depth;
        frame.vertexTopLeft->rhwBits = FLOAT_ONE_BITS;
        frame.vertexTopLeft->color = frame.packedRgb;
        frame.vertexTopLeft->specular = 0xFF000000;
        frame.vertexBottomLeft = (GfxTlVertex *)(((GfxRenderObject *)g_MenuTextBoxQuadGraphicsObject)->vertexCursor + 0x20);
        frame.vertexBottomLeft->x = (float)g_MenuViewportX + 0.0f;
        frame.vertexBottomLeft->y = (float)g_MenuViewportY + 0.0f + g_MenuViewportHeight;
        frame.vertexBottomLeft->z = depth;
        frame.vertexBottomLeft->rhwBits = FLOAT_ONE_BITS;
        frame.vertexBottomLeft->color = frame.packedRgb;
        frame.vertexBottomLeft->specular = 0xFF000000;
        frame.vertexTopRight = (GfxTlVertex *)(((GfxRenderObject *)g_MenuTextBoxQuadGraphicsObject)->vertexCursor + 0x40);
        frame.vertexTopRight->x = (float)g_MenuViewportX + 0.0f + g_MenuViewportWidth;
        frame.vertexTopRight->y = (float)g_MenuViewportY + 0.0f;
        frame.vertexTopRight->z = depth;
        frame.vertexTopRight->rhwBits = FLOAT_ONE_BITS;
        frame.vertexTopRight->color = frame.packedRgb;
        frame.vertexTopRight->specular = 0xFF000000;
        frame.vertexBottomRight = (GfxTlVertex *)(((GfxRenderObject *)g_MenuTextBoxQuadGraphicsObject)->vertexCursor + 0x60);
        frame.vertexBottomRight->x = (float)g_MenuViewportX + 0.0f + g_MenuViewportWidth;
        frame.vertexBottomRight->y = (float)g_MenuViewportY + 0.0f + g_MenuViewportHeight;
        frame.vertexBottomRight->z = depth;
        frame.vertexBottomRight->rhwBits = FLOAT_ONE_BITS;
        frame.vertexBottomRight->color = frame.packedRgb;
        frame.vertexBottomRight->specular = 0xFF000000;
    }
}

/*
 * Sets g_MenuFrameClearFlag, the main-menu redraw flag.
 */
// FUNCTION: FF7 0x006cd808
void __cdecl Menu_SetFrameClearFlag(int value)
{
    g_MenuFrameClearFlag = value;
}

/*
 * Emits the nine materia-screen batch lists in pass 0 with render state 2 and then
 * invalidates their generations.
 */
// FUNCTION: FF7 0x006cd815
void __cdecl Menu_EmitBatchListsAndInvalidate(void)
{
    GfxEngineContext *engineContext;
    int listIndex;

    engineContext = GetEngineContext();
    Gfx_SetScenePassCall(0, engineContext);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, engineContext);
    Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[0], engineContext);
    Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[1], engineContext);
    Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[2], engineContext);
    Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[3], engineContext);
    Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[4], engineContext);
    Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[5], engineContext);
    Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[6], engineContext);
    Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[7], engineContext);
    Gfx_EmitPrimitiveBatchList(g_MenuCharPortraitGraphicsObjects[8], engineContext);
    for (listIndex = 0; listIndex < 9; listIndex++) {
        InvalidateBatchGeneration(g_MenuCharPortraitGraphicsObjects[listIndex]);
    }
}

/*
 * Reads timestamp delta from g_MenuPrevFrameTimestamp and initially subtracts 274088.3
 * from the converted count. Without an active debt flag, busy-waits to
 * g_MenuFrameDuration when not late; later loop samples omit the initial
 * subtraction.
 */
// FUNCTION: FF7 0x006cd91a
void __cdecl Menu_ThrottleFrameRate(GfxEngineContext *context)
{
    struct {
        Timestamp64 delta;
        double elapsed;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.elapsed = 0.0;
    ReadTimestampCounter(&g_MenuFrameTimestamp);
    Sub64(&g_MenuFrameTimestamp, &g_MenuPrevFrameTimestamp, &frame.delta);
    frame.elapsed = CombineSplitValueToFloat(&frame.delta) - 274088.3;
    if (!g_MenuFrameThrottleFlag) {
        if (frame.elapsed <= g_MenuFrameDuration) {
            while (frame.elapsed < g_MenuFrameDuration) {
                ReadTimestampCounter(&g_MenuFrameTimestamp);
                Sub64(&g_MenuFrameTimestamp, &g_MenuPrevFrameTimestamp, &frame.delta);
                frame.elapsed = CombineSplitValueToFloat(&frame.delta);
            }
            g_MenuFrameThrottleFlag = 0;
            g_MenuDuration = 0.0;
        } else {
            g_MenuDuration = frame.elapsed - g_MenuFrameDuration;
            if (Menu_GetRequestedScreen()) {
                g_MenuFrameThrottleFlag = 1;
            } else {
                g_MenuFrameThrottleFlag = 0;
            }
        }
    } else {
        while (g_MenuFrameDuration - g_MenuDuration > frame.elapsed) {
            ReadTimestampCounter(&g_MenuFrameTimestamp);
            Sub64(&g_MenuFrameTimestamp, &g_MenuPrevFrameTimestamp, &frame.delta);
            frame.elapsed = CombineSplitValueToFloat(&frame.delta);
        }
        g_MenuFrameThrottleFlag = 0;
        g_MenuDuration = 0.0;
    }
}

/*
 * Runs the battle result screens when g_MenuEntryState is MENU_ENTRY_BATTLE_RESULT,
 * then dispatches on the game mode. The title menu runs the load menu; the field
 * menu runs the screen or service its field request names and marks completion
 * after the party backup/restore services; the world map runs the top-level menu
 * frame with no script stream.
 */
// FUNCTION: FF7 0x006cda83
void __cdecl Menu_DispatchScreenUpdate(void)
{
    switch (g_MenuEntryState) {
    case MENU_ENTRY_NONE:
        break;
    case MENU_ENTRY_BATTLE_RESULT:
        Battle_DrawResultScreenFrame();
    }
    switch (g_EngineGameMode) {
    case GAME_MODE_TITLE_MENU:
        g_MenuTitleNewGameChosen = 0;
        g_MenuTitleNewGameChosen = Menu_LoadMenuEntryThunk();
        break;
    case GAME_MODE_MENU:
        switch (g_FieldActorState.state.requestCode) {
        case FIELD_REQUEST_NAME_ENTRY:
            Menu_NameEntryMenuEntryThunk(g_FieldActorState.state.eventCmdParam);
            break;
        case FIELD_REQUEST_PARTY_SELECT:
            Menu_PhsMenuEntryThunk(g_FieldActorState.state.eventCmdParam);
            break;
        case FIELD_REQUEST_SHOP:
            Menu_ShopMenuEntryThunk(g_FieldActorState.state.eventCmdParam);
            break;
        case FIELD_REQUEST_MAIN_MENU:
            if (g_FieldActorState.state.eventCmdParam == 1) {
                Menu_RunTopLevelFrame(g_FieldDialogState);
            } else {
                Menu_RunTopLevelFrame(0);
            }
            break;
        case FIELD_REQUEST_SAVE_MENU:
            Menu_SaveMenuEntryThunk();
            break;
        case FIELD_REQUEST_BACKUP_PARTY_EQUIP:
            Script_SvcBackupPartyAndEquip(g_FieldActorState.state.eventCmdParam);
            g_MenuExitRequested = 1;
            break;
        case FIELD_REQUEST_RESTORE_PARTY_EQUIP:
            Script_SvcRestorePartyAndEquip(g_FieldActorState.state.eventCmdParam);
            g_MenuExitRequested = 1;
        }
        break;
    case GAME_MODE_WORLD_MAP:
        Menu_RunTopLevelFrame(0);
    }
}

/*
 * Returns whether the text subsystem is ready, g_TextSubsystemReady.
 */
// FUNCTION: FF7 0x006cdbf2
int __cdecl Menu_IsTextSubsystemReady(void)
{
    return g_TextSubsystemReady;
}

/*
 * Sets the text-subsystem-ready flag g_TextSubsystemReady.
 */
// FUNCTION: FF7 0x006cdbfc
void __cdecl Menu_SetTextSubsystemReady(int value)
{
    g_TextSubsystemReady = value;
}

/*
 * Sets g_MenuRequestedNextScreen, the menu's requested next screen.
 */
// FUNCTION: FF7 0x006cdc09
void __cdecl Menu_SetRequestedScreen(int value)
{
    g_MenuRequestedNextScreen = value;
}

/*
 * Returns g_MenuRequestedNextScreen, the menu's requested next screen.
 */
// FUNCTION: FF7 0x006cdc16
int __cdecl Menu_GetRequestedScreen(void)
{
    return g_MenuRequestedNextScreen;
}
} /* extern "C" */
