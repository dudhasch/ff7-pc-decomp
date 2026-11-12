// src/ff7/field/src/condor_display_state.cpp
// Data-only object: .bss 0x00cbf108..0x00cc0d88. No code.
// A data-only member of the field library, which its neighbour field_encounter_random.cpp never references:
// the Fort Condor HUD readout, unit model, DirectDraw and display-format state.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/field.h"
#include "ff7/api/menu.h"
#include "ff7/types/field.h"
#include "ff7/field.h"
#include "ff7/savemap.h"
#include "ff7/enums/main.h"
#include "ff7/enums/battle.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/types/menu.h"
#include "lib/types/mem.h"
#include "ff7/types/condor.h"
#include "lib/codegen.h"
#include "lib/types/graphics.h"
#include "ff7/enums/condor.h"
#include "lib/pfile.h"
#include "lib/types/polygon.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include "ff7/enums/field.h"
extern "C" {

// clang-format off
unsigned char g_bssPad_00cbf108[4] = {0};
// GLOBAL: FF7 0x00cbf10c
int g_CondorSecondNumberVisible = 0;
// GLOBAL: FF7 0x00cbf110
int g_CondorSelectedUnitIndex = 0;
// GLOBAL: FF7 0x00cbf114
int g_CondorTensDigitSlot = 0;
// GLOBAL: FF7 0x00cbf118
int g_CondorOnesDigitSlot = 0;
// GLOBAL: FF7 0x00cbf11c
int g_CondorSelectedReadoutVisible = 0;
// GLOBAL: FF7 0x00cbf120
int g_CondorCommandsCommitted = 0;
unsigned char g_bssPad_00cbf124[4] = {0};
// GLOBAL: FF7 0x00cbf128
int g_CondorUnitModelSetKinds = 0;
// GLOBAL: FF7 0x00cbf12c
int g_CondorUnitModelSets = 0;
unsigned char g_bssPad_00cbf130[344] = {0};
// GLOBAL: FF7 0x00cbf288
int g_CondorDisplayFormatValid = 0;
// GLOBAL: FF7 0x00cbf28c
int g_CondorPixelMaskRed = 0;
// GLOBAL: FF7 0x00cbf290
int g_CondorPixelMaskGreen = 0;
// GLOBAL: FF7 0x00cbf294
int g_CondorPixelMaskBlue = 0;
// GLOBAL: FF7 0x00cbf298
int g_CondorDirectDraw = 0;
// GLOBAL: FF7 0x00cbf29c
int g_CondorPrimarySurface = 0;
// GLOBAL: FF7 0x00cbf2a0
int g_CondorBackSurface = 0;
unsigned char g_bssPad_00cbf2a4[4] = {0};
// GLOBAL: FF7 0x00cbf2a8
int g_CondorCursorMode = 0;
// GLOBAL: FF7 0x00cbf2ac
int g_CondorCursorTargetX = 0;
// GLOBAL: FF7 0x00cbf2b0
int g_CondorCursorTargetY = 0;
// GLOBAL: FF7 0x00cbf2b4
int g_CondorCursorPosX = 0;
// GLOBAL: FF7 0x00cbf2b8
int g_CondorCursorPosY = 0;
// GLOBAL: FF7 0x00cbf2bc
int g_CondorCursorSpeedX = 0;
// GLOBAL: FF7 0x00cbf2c0
int g_CondorCursorSpeedY = 0;
// GLOBAL: FF7 0x00cbf2c4
int g_CondorCursorPrevX = 0;
// GLOBAL: FF7 0x00cbf2c8
int g_CondorCursorPrevY = 0;
// GLOBAL: FF7 0x00cbf2cc
int g_CondorCursorResetFlag = 0;
// GLOBAL: FF7 0x00cbf2d0
int g_CondorInputLeft = 0;
// GLOBAL: FF7 0x00cbf2d4
int g_CondorInputUp = 0;
// GLOBAL: FF7 0x00cbf2d8
int g_CondorInputDown = 0;
// GLOBAL: FF7 0x00cbf2dc
int g_CondorInputRight = 0;
unsigned char g_bssPad_00cbf2e0[4] = {0};
// GLOBAL: FF7 0x00cbf2e4
int g_CondorInputSquare = 0;
// GLOBAL: FF7 0x00cbf2e8
int g_CondorInputCircle = 0;
// GLOBAL: FF7 0x00cbf2ec
int g_CondorInputTriangle = 0;
// GLOBAL: FF7 0x00cbf2f0
int g_CondorInputCross = 0;
// GLOBAL: FF7 0x00cbf2f4
int g_CondorInputR1 = 0;
// GLOBAL: FF7 0x00cbf2f8
int g_CondorInputR2 = 0;
// GLOBAL: FF7 0x00cbf2fc
int g_CondorInputL1 = 0;
// GLOBAL: FF7 0x00cbf300
int g_CondorInputL2 = 0;
// GLOBAL: FF7 0x00cbf304
int g_CondorInputStart = 0;
// GLOBAL: FF7 0x00cbf308
int g_CondorInputSelect = 0;
unsigned char g_bssPad_00cbf30c[4] = {0};
// GLOBAL: FF7 0x00cbf310
struct CondorSoundCommand *g_CondorSoundCommandCursor = 0;
unsigned char g_bssPad_00cbf314[4] = {0};
// GLOBAL: FF7 0x00cbf318
struct CondorSoundCommand g_CondorSoundCommandBuffer[3] = {0};
// GLOBAL: FF7 0x00cbf33c
int g_CondorPhasedMotionStep = 0;
// GLOBAL: FF7 0x00cbf340
int g_CondorActionSelectState = 0;
unsigned char g_bssPad_00cbf344[4] = {0};
// The draw state of the eight effect slots, indexed by the same slot as g_CondorEffectSlots.
// GLOBAL: FF7 0x00cbf348
CondorEffectSlotEntry g_CondorEffectSpriteSlots[8] = {0};
// GLOBAL: FF7 0x00cbf568
int g_CondorFadeAlpha = 0;
unsigned char g_bssPad_00cbf56c[4] = {0};
unsigned char g_bssPad_00cbf570[8] = {0};
// GLOBAL: FF7 0x00cbf578
int g_FieldWindowTextPointers[4] = {0};
// GLOBAL: FF7 0x00cbf588
unsigned char g_FieldRandomIndex = 0;
unsigned char g_bssPad_00cbf58c[4] = {0};
// GLOBAL: FF7 0x00cbf590
unsigned short g_FieldModelDefaultSpeed[24] = {0};
// GLOBAL: FF7 0x00cbf5c0
short g_FieldModelIdleAnimSpeed[16] = {0};
// GLOBAL: FF7 0x00cbf5e0
short g_FieldWindowDelay[4] = {0};
// GLOBAL: FF7 0x00cbf5e8
struct FieldScriptHeader *g_FieldScripts = 0;
unsigned char g_bssPad_00cbf5ec[4] = {0};
// GLOBAL: FF7 0x00cbf5f0
short g_FieldWindowSpeed[4] = {0};
unsigned char g_bssPad_00cbf5f8[8] = {0};
// GLOBAL: FF7 0x00cbf600
unsigned char g_FieldEntityLine[184] = {0};
// GLOBAL: FF7 0x00cbf6b8
unsigned char g_FieldRequestPosted = 0;
unsigned char g_bssPad_00cbf6bc[4] = {0};
// GLOBAL: FF7 0x00cbf6c0
unsigned short g_FieldScriptPriorityPC[48][8] = {0};
// GLOBAL: FF7 0x00cbf9c0
char g_FieldAudioDisabled = 0;
unsigned char g_bssPad_00cbf9c4[4] = {0};
// GLOBAL: FF7 0x00cbf9c8
unsigned char g_FieldScriptExecFlags[16] = {0};
// GLOBAL: FF7 0x00cbf9d8
struct FieldState *g_pFieldState = 0;
// GLOBAL: FF7 0x00cbf9dc
short g_EngineGameMode = GAME_MODE_NONE;
unsigned char g_bssPad_00cbf9e0[8] = {0};
// GLOBAL: FF7 0x00cbf9e8
unsigned char g_FieldEntityScriptTrack[8] = {0};
unsigned char g_bssPad_00cbf9f0[376] = {0};
// GLOBAL: FF7 0x00cbfb68
unsigned char g_FieldMovieRequested = 0;
// GLOBAL: FF7 0x00cbfb6c
char g_FieldMoviePending = 0;
// GLOBAL: FF7 0x00cbfb70
unsigned char g_FieldEntityModel[48] = {0};
// GLOBAL: FF7 0x00cbfba0
unsigned char g_FieldEntityScriptTable[64] = {0};
// GLOBAL: FF7 0x00cbfbe0
unsigned char g_WindowReplaceBank[4][8] = {0};
// GLOBAL: FF7 0x00cbfc00
short g_FieldWindowPos[4] = {0};
// GLOBAL: FF7 0x00cbfc08
unsigned short g_FieldPalettes[64][16] = {0};
unsigned char g_bssPad_00cc0408[4] = {0};
// GLOBAL: FF7 0x00cc040c
char g_FieldDialogueLock = 0;
// GLOBAL: FF7 0x00cc0410
short g_FieldHandCursorY = 0;
// GLOBAL: FF7 0x00cc0414
short g_FieldHandCursorX = 0;
// GLOBAL: FF7 0x00cc0418
short g_FieldWindowRevealProgress[4] = {0};
unsigned char g_bssPad_00cc0420[8] = {0};
// GLOBAL: FF7 0x00cc0428
unsigned char g_FieldWindowText[4][256] = {0};
// How a menu screen is being entered over another module: 1 while the field fades out to the menu
// (Field_UpdateLogic; the fade's end clears it), 2 once the battle has opened its command menu
// (Battle_InitOpeningUiState; Battle_ModuleFrame runs the battle UI while it is 2), 4 when the
// battle ends into the result screens (Battle_RunEndSequence; main then enters
// GAME_MODE_MENU_AFTER_BATTLE). The battle's opening waits for 0.
// GLOBAL: FF7 0x00cc0828
short g_MenuEntryState = 0;
unsigned char g_bssPad_00cc082c[4] = {0};
// GLOBAL: FF7 0x00cc0830
unsigned char g_FieldFollowerState[48] = {0};
// GLOBAL: FF7 0x00cc0860
char g_FieldLineDrawIntensity = 0;
unsigned char g_bssPad_00cc0864[4] = {0};
// GLOBAL: FF7 0x00cc0868
short g_FieldFollowerCoords[4] = {0};
// GLOBAL: FF7 0x00cc0870
char g_FieldPlayerCrossedLine = 0;
unsigned char g_bssPad_00cc0874[4] = {0};
// 0xCC0878..0xCC08E0 was the 48-byte registry storage object g_FieldModelIdleAnimId (with the
// public g_FieldModelIndex) and the adjacent state globals g_PartyStatsDirty, g_FieldScriptOpcode,
// g_FieldModelCount, g_FieldLineCount (0xCC088C) and g_WindowReplaceBankAddr.
// Defined as contiguous objects in this TU's .bss span so each symbol lands at its
// exact retail address.
// GLOBAL: FF7 0x00cc0878
unsigned char g_FieldModelIdleAnimId[16] = {0};
// GLOBAL: FF7 0x00cc0888
unsigned char g_PartyStatsDirty = 0;
// GLOBAL: FF7 0x00cc088c
short g_FieldLineCount = 0;
// GLOBAL: FF7 0x00cc0890
unsigned char g_FieldScriptOpcode = 0;
// GLOBAL: FF7 0x00cc0894
unsigned char g_FieldModelIndex = 0;
// GLOBAL: FF7 0x00cc0898
unsigned char g_FieldModelCount = 0;
unsigned char g_bssPad_00cc089c[4] = {0};
// GLOBAL: FF7 0x00cc08a0
unsigned short g_WindowReplaceBankAddr[4][8] = {0};
unsigned char g_bssPad_00cc08e0[8] = {0};
// GLOBAL: FF7 0x00cc08e8
void *g_FieldDialogTextPtr = 0;
// GLOBAL: FF7 0x00cc08ec
unsigned char g_FieldDialogTextState[4] = {0};
// GLOBAL: FF7 0x00cc08f0
int g_FieldDialogState = 0;
unsigned char g_bssPad_00cc08f4[4] = {0};
unsigned char g_bssPad_00cc08f8[8] = {0};
// GLOBAL: FF7 0x00cc0900
unsigned short g_FieldScriptWait[48] = {0};
// GLOBAL: FF7 0x00cc0960
unsigned char g_FieldWindowOwner[4] = {0};
// GLOBAL: FF7 0x00cc0964
unsigned char g_CurrentEntity = 0;
unsigned char g_bssPad_00cc0968[8] = {0};
// GLOBAL: FF7 0x00cc0970
unsigned char g_FieldHandCursorState = 0;
// GLOBAL: FF7 0x00cc0974
short g_FieldWorldMapResult = 0;
unsigned char g_bssPad_00cc0978[8] = {0};
// GLOBAL: FF7 0x00cc0980
unsigned char g_FieldModelAnimMode[16] = {0};
// GLOBAL: FF7 0x00cc0990
unsigned char g_FieldBlinkRandomIndex = 0;
unsigned char g_bssPad_00cc0994[4] = {0};
// GLOBAL: FF7 0x00cc0998
FieldCharEntityMap g_FieldCharEntity = {0};
// GLOBAL: FF7 0x00cc0b30
unsigned char g_FieldScriptPriority[48] = {0};
// GLOBAL: FF7 0x00cc0b60
struct FieldEntity *g_pFieldEntities = 0;
// GLOBAL: FF7 0x00cc0b64
char g_FieldOpenWindowCount = 0;
// GLOBAL: FF7 0x00cc0b68
char g_FieldMoviePlaying = 0;
unsigned char g_bssPad_00cc0b6c[4] = {0};
// GLOBAL: FF7 0x00cc0b70
short g_FieldMovieFrame = 0;
unsigned char g_bssPad_00cc0b74[4] = {0};
// GLOBAL: FF7 0x00cc0b78
unsigned char g_FieldScriptPriorityState[48][8] = {0};
// GLOBAL: FF7 0x00cc0cf8
unsigned short g_FieldScriptPC[52] = {0};
// GLOBAL: FF7 0x00cc0d60
short g_FieldModelAnimSpeed[16] = {0};
// The battle request the world map writes (WorldMap_MainUpdate's `battle`): bit 31 and bit 29 are
// flags main's battle-swirl case turns into the encounter state, bits 0..29 the battle scene.
// GLOBAL: FF7 0x00cc0d80
int g_WorldBattleRequest = 0;
// GLOBAL: FF7 0x00cc0d84
short g_EnginePreviousMode = GAME_MODE_NONE;
// clang-format on

} /* extern "C" */
