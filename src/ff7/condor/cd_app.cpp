// src/ff7/condor/cd_app.cpp
// Original: C:\FF7\condor\cd_app.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/condor.h"
#include "lib/pfile.h"
#include "lib/enums/graphics.h"
#include "ff7/api/main.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "lib/api/sound.h"
#include "ff7/types/condor.h"
#include "lib/types/graphics.h"
#include "ff7/enums/main.h"
#include <windows.h>
#include <string.h>
#include "lib/types/mem.h"
#include "lib/codegen.h"
#include "ff7/enums/condor.h"
#include "ff7/savemap.h"
extern "C" {

struct GfxEngineContext;
/*
 * Releases an RSD model instance and frees its holding container block.
 */
// FUNCTION: FF7 0x005f4740
void __cdecl Mini_CondorFreeRsdModelHolder(CondorRsdModelHolder *holder, void *flags)
{
    if (holder != 0) {
        Pfile_FreeRsdModel(holder->model, flags);
        // STRING: FF7 0x00901a4c
        GameHeapFree(holder, "C:\\FF7\\condor\\cd_app.cpp", 0x77);
    }
}
/*
 * Allocates and initializes a 16-byte RSD model holder.
 */
// FUNCTION: FF7 0x005f4771
int __cdecl Mini_CondorAllocRsdModelHolder(void)
{
    CondorRsdModelHolder *rec;

    // STRING: FF7 0x00901a68
    rec = (CondorRsdModelHolder *)GameHeapAlloc(1, 0x10, "C:\\FF7\\condor\\cd_app.cpp", 0x7F);
    if (rec != 0) {
        rec->initWord0 = 1;
        rec->initWord1 = 1;
        rec->initWord2 = 4;
    }
    return (int)rec;
}

#pragma intrinsic(strcpy, strcat)

__declspec(dllimport) unsigned long __stdcall GetCurrentDirectoryA(unsigned long size, char *buf);
// clang-format off
// The head of cd_app.cpp's .data (it sets the flag to 1 in its mode entry; cd_init.cpp, condor_engine_frame_render.cpp and
// condor_unit_model_loading.cpp read it).
// GLOBAL: FF7 0x00901a40
int g_CondorUsesLgp = 1;
// GLOBAL: FF7 0x00901a44
int g_CondorAppFlag = 1;
// GLOBAL: FF7 0x00901a48
int g_CondorMinigameRunning = 1;
// GLOBAL: FF7 0x00c60960
int g_CondorAppState = 0;
unsigned char g_bssPad_00c60964[4] = {0};
unsigned char g_bssPad_00c60968[16] = {0};
// GLOBAL: FF7 0x00c60978
unsigned short g_CondorFrameCounter = 0;
// GLOBAL: FF7 0x00c6097c
short g_CondorSelectedRecord = 0;
// GLOBAL: FF7 0x00c60980
CondorMatchListState g_CondorMatchListState = {0};
// GLOBAL: FF7 0x00c60ac0
int g_CondorCameraZoomInit = 0;
// GLOBAL: FF7 0x00c60ac4
short g_CondorPendingRequest = 0;
// GLOBAL: FF7 0x00c60ac8
int g_CondorMotionAccX = 0;
// GLOBAL: FF7 0x00c60acc
int g_CondorMotionAccY = 0;
// GLOBAL: FF7 0x00c60ad0
int g_CondorPlayerUnitCount = 0;
// GLOBAL: FF7 0x00c60ad4
short g_CondorDisplayMode = 0;
unsigned char g_bssPad_00c60ad8[4] = {0};
// GLOBAL: FF7 0x00c60adc
short g_CondorRequestedDisplayMode = 0;
// GLOBAL: FF7 0x00c60ae0
unsigned short g_CondorBattleFrame = 0;
// GLOBAL: FF7 0x00c60ae4
short g_CondorChoiceTimer = 0;
// GLOBAL: FF7 0x00c60ae8
unsigned char g_CondorHighestUnitY[0x4] = {0};
// GLOBAL: FF7 0x00c60aec
short g_CondorRedFadeLevel = 0;
// GLOBAL: FF7 0x00c60af0
int g_CondorCameraOffsetInit = 0;
unsigned char g_bssPad_00c60af4[4] = {0};
// GLOBAL: FF7 0x00c60af8
struct CondorResetBufferA g_CondorRotationAngles = {0};
// GLOBAL: FF7 0x00c60b00
unsigned short g_CondorCameraX = 0;
// GLOBAL: FF7 0x00c60b04
unsigned short g_CondorCameraY = 0;
unsigned char g_bssPad_00c60b08[2072] = {0};
// GLOBAL: FF7 0x00c61320
int g_CondorStepSpanX = 0;
// GLOBAL: FF7 0x00c61324
int g_CondorStepSpanY = 0;
// GLOBAL: FF7 0x00c61328
int g_CondorStepSpanZ = 0;
unsigned char g_bssPad_00c6132c[4] = {0};
// GLOBAL: FF7 0x00c61330
CondorEffectSequence g_CondorEffectSlots[8] = {0};
// GLOBAL: FF7 0x00c613b0
short g_CondorDeselectAfterAction = 0;
unsigned char g_bssPad_00c613b4[4] = {0};
// GLOBAL: FF7 0x00c613b8
short g_CondorScratchPair = 0;
// GLOBAL: FF7 0x00c613bc
short g_CondorUnitWorldZ = 0;
// GLOBAL: FF7 0x00c613c0
char g_CondorSavedSubPixelX = 0;
// GLOBAL: FF7 0x00c613c4
char g_CondorSavedSubPixelY = 0;
// GLOBAL: FF7 0x00c613c8
short g_CondorWheelSpinOffset = 0;
// GLOBAL: FF7 0x00c613cc
short g_CondorSnapshotX = 0;
// GLOBAL: FF7 0x00c613d0
struct CondorResetBufferA g_CondorUnitRotationReset = {0};
// GLOBAL: FF7 0x00c613d8
short g_CondorSnapshotY = 0;
unsigned char g_bssPad_00c613dc[4] = {0};
// GLOBAL: FF7 0x00c613e0
short g_CondorDigitStarted = 0;
unsigned char g_bssPad_00c613e4[4] = {0};
unsigned char g_bssPad_00c613e8[2048] = {0};
// GLOBAL: FF7 0x00c61be8
short g_CondorWheelOffset = 0;
// GLOBAL: FF7 0x00c61bec
short g_CondorHudCursorYLimit = 0;
// GLOBAL: FF7 0x00c61bf0
short g_CondorSavedCameraY = 0;
// GLOBAL: FF7 0x00c61bf4
short g_CondorRecordLayoutCount = 0;
// GLOBAL: FF7 0x00c61bf8
unsigned char g_CondorActiveRecords[162] = {0};
unsigned char g_bssPad_00c61c9c[4] = {0};
// GLOBAL: FF7 0x00c61ca0
struct CondorCounterEffect g_CondorCounterEffects[16] = {0};
// GLOBAL: FF7 0x00c61da0
unsigned short g_CondorBlinkTick = 0;
unsigned char g_bssPad_00c61da4[4] = {0};
unsigned char g_bssPad_00c61da8[2048] = {0};
// GLOBAL: FF7 0x00c625a8
int g_CondorScheduleClock = 0;
// GLOBAL: FF7 0x00c625ac
short g_CondorSavedDisplayMode = 0;
// GLOBAL: FF7 0x00c625b0
unsigned char g_CondorCameraMatrix[30] = {0};
// GLOBAL: FF7 0x00c625d0
short g_CondorWheelAngle = 0;
// GLOBAL: FF7 0x00c625d4
int g_CondorPhase = 0;
// GLOBAL: FF7 0x00c625d8
short g_CondorLayoutCounter = 0;
// GLOBAL: FF7 0x00c625dc
short g_CondorBannerTarget = 0;
// GLOBAL: FF7 0x00c625e0
int g_CondorMainState = 0;
unsigned char g_bssPad_00c625e4[4] = {0};
// GLOBAL: FF7 0x00c625e8
CondorRegionRecord g_CondorUnitTable = {0};
unsigned char g_bssPad_00c62634[4] = {0};
unsigned char g_bssPad_00c62638[67504] = {0};
// GLOBAL: FF7 0x00c72de8
int g_CondorHudBlinkPhase = 0;
// GLOBAL: FF7 0x00c72dec
short g_CondorActiveActionRecord = 0;
// GLOBAL: FF7 0x00c72df0
short g_CondorPhaseFrames = 0;
// GLOBAL: FF7 0x00c72df4
short g_CondorPanelRectX = 0;
// GLOBAL: FF7 0x00c72df8
short g_CondorPanelRectY = 0;
// GLOBAL: FF7 0x00c72dfc
short g_CondorBannerCounter = 0;
// GLOBAL: FF7 0x00c72e00
int g_CondorHudBackgroundMode = 0;
unsigned char g_bssPad_00c72e04[4] = {0};
unsigned char g_bssPad_00c72e08[32] = {0};
// GLOBAL: FF7 0x00c72e28
int g_CondorWheelScaleX = 0;
// GLOBAL: FF7 0x00c72e2c
int g_CondorWheelScaleY = 0;
// GLOBAL: FF7 0x00c72e30
int g_CondorWheelScaleZ = 0;
unsigned char g_bssPad_00c72e34[4] = {0};
// GLOBAL: FF7 0x00c72e38
short g_CondorStepDivisor = 0;
// GLOBAL: FF7 0x00c72e3c
short g_CondorActionRecordSlot = 0;
// GLOBAL: FF7 0x00c72e40
int g_CondorMotionStepsX = 0;
// GLOBAL: FF7 0x00c72e44
int g_CondorMotionStepsY = 0;
// GLOBAL: FF7 0x00c72e48
short g_CondorNextSpawnTime = 0;
// GLOBAL: FF7 0x00c72e4c
short g_CondorNumberSpriteBaseField = 0;
// GLOBAL: FF7 0x00c72e50
short g_CondorRampLimit = 0;
unsigned char g_bssPad_00c72e54[4] = {0};
unsigned char g_CondorWorldMatrixBuffer[32] = {0};
// GLOBAL: FF7 0x00c72e78
short g_CondorDeltaRecordCount = 0;
// GLOBAL: FF7 0x00c72e7c
short g_CondorUnitChoiceTier = 0;
// GLOBAL: FF7 0x00c72e80
int g_CondorInputMask = 0;
unsigned char g_bssPad_00c72e84[4] = {0};
// GLOBAL: FF7 0x00c72e88
CondorSpriteEntry g_CondorSpriteEntries[5] = {0};
unsigned char g_bssPad_00c72f50[20] = {0};
// Sprite slot 5's position source: the cursor record the slot is drawn at.
// GLOBAL: FF7 0x00c72f64
CondorCursor *g_pCondorSpriteSlot5PosSource = 0;
unsigned char g_bssPad_00c72f68[20] = {0};
// GLOBAL: FF7 0x00c72f7c
CondorSpriteSizeX g_CondorSpriteSlot6HeightX = {0};
// GLOBAL: FF7 0x00c72f80
short g_CondorSpriteSlot6ScreenY = 0;
unsigned char g_bssPad_00c72f84[4] = {0};
unsigned char g_bssPad_00c72f88[4] = {0};
// GLOBAL: FF7 0x00c72f8c
CondorCursor *g_pCondorSpriteSlot6PosSource = 0;
unsigned char g_bssPad_00c72f90[140] = {0};
// GLOBAL: FF7 0x00c7301c
CondorSpriteSizeX g_CondorSpriteSlotAHeightX = {0};
// GLOBAL: FF7 0x00c73020
short g_CondorSpriteSlotAScreenY = 0;
unsigned char g_bssPad_00c73024[4] = {0};
unsigned char g_bssPad_00c73028[264] = {0};
// GLOBAL: FF7 0x00c73130
CondorSpriteHeadOffsetX g_CondorSpriteSlot11OffsetX = {0};
// GLOBAL: FF7 0x00c73134
CondorHeightPosPair g_CondorSpriteSlot17HeightX = {0};
// GLOBAL: FF7 0x00c73138
short g_CondorSpriteSlot11ScreenY = 0;
unsigned char g_bssPad_00c7313c[4] = {0};
unsigned char g_bssPad_00c73140[24] = {0};
// GLOBAL: FF7 0x00c73158
CondorSpriteHeadOffsetX g_CondorSpriteSlot12OffsetX = {0};
// GLOBAL: FF7 0x00c7315c
CondorHeightPosPair g_CondorSpriteSlot18HeightX = {0};
// GLOBAL: FF7 0x00c73160
short g_CondorSpriteSlot12ScreenY = 0;
unsigned char g_bssPad_00c73164[4] = {0};
unsigned char g_bssPad_00c73168[28] = {0};
// GLOBAL: FF7 0x00c73184
CondorSpriteSizeX g_CondorSpriteSlot13HeightX = {0};
// GLOBAL: FF7 0x00c73188
short g_CondorSpriteSlot13ScreenY = 0;
unsigned char g_bssPad_00c7318c[4] = {0};
unsigned char g_bssPad_00c73190[68] = {0};
// GLOBAL: FF7 0x00c731d4
CondorSpriteSizeX g_CondorSpriteSlot15HeightX = {0};
// GLOBAL: FF7 0x00c731d8
short g_CondorSpriteSlot15ScreenY = 0;
unsigned char g_bssPad_00c731dc[4] = {0};
unsigned char g_bssPad_00c731e0[44] = {0};
// GLOBAL: FF7 0x00c7320c
struct CondorSpriteOffset *g_pCondorReadoutOrigin = 0;
unsigned char g_bssPad_00c73210[180] = {0};
// GLOBAL: FF7 0x00c732c4
CondorSpriteSizeX g_CondorSpriteSlot1BHeightX = {0};
// GLOBAL: FF7 0x00c732c8
short g_CondorSpriteSlot1BScreenY = 0;
unsigned char g_bssPad_00c732cc[4] = {0};
unsigned char g_bssPad_00c732d0[28] = {0};
// GLOBAL: FF7 0x00c732ec
CondorSpriteSizeX g_CondorSpriteSlot1CHeightX = {0};
// GLOBAL: FF7 0x00c732f0
short g_CondorSpriteSlot1CScreenY = 0;
unsigned char g_bssPad_00c732f4[4] = {0};
unsigned char g_bssPad_00c732f8[28] = {0};
// GLOBAL: FF7 0x00c73314
CondorSpriteSizeX g_CondorSpriteSlot1DHeightX = {0};
// GLOBAL: FF7 0x00c73318
short g_CondorSpriteSlot1DScreenY = 0;
unsigned char g_bssPad_00c7331c[4] = {0};
unsigned char g_bssPad_00c73320[28] = {0};
// GLOBAL: FF7 0x00c7333c
CondorSpriteSizeX g_CondorSpriteSlot1EHeightX = {0};
// GLOBAL: FF7 0x00c73340
short g_CondorSpriteSlot1EScreenY = 0;
unsigned char g_bssPad_00c73344[4] = {0};
unsigned char g_bssPad_00c73348[28] = {0};
// GLOBAL: FF7 0x00c73364
CondorSpriteSizeX g_CondorSpriteSlot1FHeightX = {0};
unsigned char g_bssPad_00c73368[36] = {0};
// GLOBAL: FF7 0x00c7338c
CondorSpriteSizeX g_CondorSpriteSlot20HeightX = {0};
unsigned char g_bssPad_00c73390[36] = {0};
// GLOBAL: FF7 0x00c733b4
CondorSpriteSizeX g_CondorSpriteSlot21HeightX = {0};
unsigned char g_bssPad_00c733b8[36] = {0};
// GLOBAL: FF7 0x00c733dc
CondorSpriteSizeX g_CondorSpriteSlot22HeightX = {0};
unsigned char g_bssPad_00c733e0[36] = {0};
// GLOBAL: FF7 0x00c73404
CondorSpriteSizeX g_CondorSpriteSlot23HeightX = {0};
unsigned char g_bssPad_00c73408[36] = {0};
// GLOBAL: FF7 0x00c7342c
CondorSpriteSizeX g_CondorSpriteSlot24HeightX = {0};
unsigned char g_bssPad_00c73430[36] = {0};
// GLOBAL: FF7 0x00c73454
CondorSpriteSizeX g_CondorSpriteSlot25HeightX = {0};
// GLOBAL: FF7 0x00c73458
short g_CondorSpriteSlot25ScreenY = 0;
unsigned char g_bssPad_00c7345c[4] = {0};
unsigned char g_bssPad_00c73460[28] = {0};
// GLOBAL: FF7 0x00c7347c
CondorSpriteSizeX g_CondorSpriteSlot26HeightX = {0};
// GLOBAL: FF7 0x00c73480
short g_CondorSpriteSlot26ScreenY = 0;
unsigned char g_bssPad_00c73484[4] = {0};
unsigned char g_bssPad_00c73488[28] = {0};
// GLOBAL: FF7 0x00c734a4
CondorSpriteSizeX g_CondorSpriteSlot27HeightX = {0};
// GLOBAL: FF7 0x00c734a8
short g_CondorSpriteSlot27ScreenY = 0;
unsigned char g_bssPad_00c734ac[4] = {0};
unsigned char g_bssPad_00c734b0[28] = {0};
// GLOBAL: FF7 0x00c734cc
CondorSpriteSizeX g_CondorSpriteSlot28HeightX = {0};
// GLOBAL: FF7 0x00c734d0
short g_CondorSpriteSlot28ScreenY = 0;
unsigned char g_bssPad_00c734d4[4] = {0};
unsigned char g_bssPad_00c734d8[28] = {0};
// GLOBAL: FF7 0x00c734f4
CondorSpriteSizeX g_CondorSpriteSlot29HeightX = {0};
// GLOBAL: FF7 0x00c734f8
short g_CondorSpriteSlot29ScreenY = 0;
unsigned char g_bssPad_00c734fc[4] = {0};
unsigned char g_bssPad_00c73500[28] = {0};
// GLOBAL: FF7 0x00c7351c
CondorSpriteSizeX g_CondorSpriteSlot2AHeightX = {0};
// GLOBAL: FF7 0x00c73520
short g_CondorSpriteSlot2AScreenY = 0;
unsigned char g_bssPad_00c73524[4] = {0};
unsigned char g_bssPad_00c73528[28] = {0};
// GLOBAL: FF7 0x00c73544
CondorSpriteSizeX g_CondorSpriteSlot2BHeightX = {0};
// GLOBAL: FF7 0x00c73548
short g_CondorSpriteSlot2BScreenY = 0;
unsigned char g_bssPad_00c7354c[4] = {0};
unsigned char g_bssPad_00c73550[28] = {0};
// GLOBAL: FF7 0x00c7356c
CondorSpriteSizeX g_CondorSpriteSlot2CHeightX = {0};
// GLOBAL: FF7 0x00c73570
short g_CondorSpriteSlot2CScreenY = 0;
unsigned char g_bssPad_00c73574[4] = {0};
unsigned char g_bssPad_00c73578[108] = {0};
// GLOBAL: FF7 0x00c735e4
CondorSpriteSizeX g_CondorSpriteSlot2FHeightX = {0};
// GLOBAL: FF7 0x00c735e8
short g_CondorSpriteSlot2FScreenY = 0;
unsigned char g_bssPad_00c735ec[4] = {0};
unsigned char g_bssPad_00c735f0[68] = {0};
// GLOBAL: FF7 0x00c73634
CondorSpriteSizeX g_CondorSpriteSlot31HeightX = {0};
// GLOBAL: FF7 0x00c73638
short g_CondorSpriteSlot31ScreenY = 0;
unsigned char g_bssPad_00c7363c[4] = {0};
unsigned char g_bssPad_00c73640[4] = {0};
// GLOBAL: FF7 0x00c73644
CondorCursor *g_pCondorSpriteSlot31PosSource = 0;
unsigned char g_bssPad_00c73648[60] = {0};
// GLOBAL: FF7 0x00c73684
CondorSpriteSizeX g_CondorSpriteSlot33HeightX = {0};
// GLOBAL: FF7 0x00c73688
short g_CondorSpriteSlot33ScreenY = 0;
unsigned char g_bssPad_00c7368c[4] = {0};
unsigned char g_bssPad_00c73690[268] = {0};
// GLOBAL: FF7 0x00c7379c
CondorSpriteSizeX g_CondorSpriteSlot3AHeightX = {0};
// GLOBAL: FF7 0x00c737a0
short g_CondorSpriteSlot3AScreenY = 0;
unsigned char g_bssPad_00c737a4[4] = {0};
unsigned char g_bssPad_00c737a8[5264] = {0};
// GLOBAL: FF7 0x00c74c38
int g_CondorCameraTranslateX = 0;
// GLOBAL: FF7 0x00c74c3c
int g_CondorCameraTranslateY = 0;
// GLOBAL: FF7 0x00c74c40
int g_CondorCameraTranslateZ = 0;
unsigned char g_bssPad_00c74c44[4] = {0};
// GLOBAL: FF7 0x00c74c48
int g_CondorFrameInput = 0;
// GLOBAL: FF7 0x00c74c4c
int g_CondorPrevInputMask = 0;
// GLOBAL: FF7 0x00c74c50
int g_CondorRecordMode = 0;
// GLOBAL: FF7 0x00c74c54
int g_CondorInputPressed = 0;
unsigned char g_bssPad_00c74c58[16] = {0};
// GLOBAL: FF7 0x00c74c68
short g_CondorLayoutIndex = 0;
unsigned char g_bssPad_00c74c6c[4] = {0};
unsigned char g_bssPad_00c74c70[32] = {0};
// GLOBAL: FF7 0x00c74c90
int g_CondorInitialSpawnTime = 0;
unsigned char g_bssPad_00c74c94[4] = {0};
// GLOBAL: FF7 0x00c74c98
short g_CondorWheelOrigin = 0;
// GLOBAL: FF7 0x00c74c9c
short g_CondorWheelOriginZ = 0;
// GLOBAL: FF7 0x00c74ca0
CondorCursor g_CondorHudCursor = {0};
unsigned char g_bssPad_00c74ca4[4] = {0};
// GLOBAL: FF7 0x00c74ca8
unsigned char g_CondorActionRowA = 0;
// GLOBAL: FF7 0x00c74cac
short g_CondorScheduleSeconds = 0;
// GLOBAL: FF7 0x00c74cb0
unsigned char g_CondorActionRowB = 0;
// GLOBAL: FF7 0x00c74cb4
short g_CondorCounterDisplayOffset = 0;
// GLOBAL: FF7 0x00c74cb8
unsigned char g_Condor_Byte = 0;
// GLOBAL: FF7 0x00c74cbc
short g_CondorPairUnitIndex = 0;
// GLOBAL: FF7 0x00c74cc0
int g_CondorSequenceCounter = 0;
// GLOBAL: FF7 0x00c74cc4
short g_CondorRecordDisplayOffset = 0;
// GLOBAL: FF7 0x00c74cc8
short g_CondorSectorAngle = 0;
unsigned char g_bssPad_00c74ccc[4] = {0};
// GLOBAL: FF7 0x00c74cd0
unsigned char g_CondorRunRecordLayouts[42] = {0};
unsigned char g_bssPad_00c74cfc[4] = {0};
unsigned char g_bssPad_00c74d00[1364] = {0};
// GLOBAL: FF7 0x00c75254
short g_CondorRingBase = 0;
// GLOBAL: FF7 0x00c75258
short g_CondorLayoutStart = 0;
unsigned char g_bssPad_00c7525c[4] = {0};
unsigned char g_bssPad_00c75260[4] = {0};
// GLOBAL: FF7 0x00c75264
short g_CondorChoiceSectorCount = 0;
// GLOBAL: FF7 0x00c75268
CondorCursor g_CondorCursor = {0};
unsigned char g_bssPad_00c7526c[4] = {0};
// GLOBAL: FF7 0x00c75270
int g_CondorReadoutValue = 0;
// GLOBAL: FF7 0x00c75274
int g_CondorLinearMotionSaved = 0;
// GLOBAL: FF7 0x00c75278
char g_CondorChoiceRing[11] = {0};
// GLOBAL: FF7 0x00c75284
short g_CondorRecordAngle = 0;
// GLOBAL: FF7 0x00c75288
short g_CondorWedgeVertexAngle = 0;
// GLOBAL: FF7 0x00c7528c
short g_CondorWedgeSpanAngle = 0;
// GLOBAL: FF7 0x00c75290
short g_CondorUnitPosition = 0;
// GLOBAL: FF7 0x00c75294
short g_CondorUnitPositionZ = 0;
// GLOBAL: FF7 0x00c75298
short g_CondorWedgeTestAngle = 0;
// GLOBAL: FF7 0x00c7529c
short g_CondorLayoutPanelHeight = 0;
// GLOBAL: FF7 0x00c752a0
short g_CondorRecordPanelHeight = 0;
// GLOBAL: FF7 0x00c752a4
short g_CondorCollectedRecordCount = 0;
// GLOBAL: FF7 0x00c752a8
unsigned char g_CondorScreenDistanceBuf[4] = {0};
// GLOBAL: FF7 0x00c752ac
int g_CondorSubViewIndex = 0;
// GLOBAL: FF7 0x00c752b0
char g_CondorCommandCode = 0;
// GLOBAL: FF7 0x00c752b4
short g_CondorTickStep = 0;
// GLOBAL: FF7 0x00c752b8
unsigned short g_CondorCursorScreenX = 0;
// GLOBAL: FF7 0x00c752bc
unsigned short g_CondorCursorScreenY = 0;
// GLOBAL: FF7 0x00c752c0
short g_CondorDisplayModePending = 0;
// GLOBAL: FF7 0x00c752c4
short g_CondorDisplayModeRequest = 0;
// GLOBAL: FF7 0x00c752c8
short g_CondorPurchaseSlot = 0;
// GLOBAL: FF7 0x00c752cc
int g_CondorVecX = 0;
// GLOBAL: FF7 0x00c752d0
int g_CondorVecY = 0;
// GLOBAL: FF7 0x00c752d4
unsigned char g_CondorActionRowIndex = 0;
// GLOBAL: FF7 0x00c752d8
int g_CondorMotionStepX = 0;
// GLOBAL: FF7 0x00c752dc
int g_CondorMotionStepY = 0;
unsigned char g_bssPad_00c752e0[291760] = {0};
// 2-byte extent (second short at +2 is unattributed .bss gap).
// GLOBAL: FF7 0x00cbc690
short g_CondorIndexRange = 0;
unsigned char g_bssPad_00cbc694[4] = {0};
// GLOBAL: FF7 0x00cbc698
struct CondorConfigEntry *g_pCondorConfigEntry = 0;
unsigned char g_bssPad_00cbc69c[4] = {0};
// g_CondorDeltaRecords, g_CondorDeltaRecords[0].listValue are indexed by the code as fields of
// records. The span runs to the next named definition of the TU; the TU's pieces inside
// 0xcbc6a0..0xcbc7a0 are aliases.
// GLOBAL: FF7 0x00cbc6a0
struct CondorDeltaRecord g_CondorDeltaRecords[32] = {0};
// GLOBAL: FF7 0x00cbc7a0
int g_CondorMoveFlags = 0;
// GLOBAL: FF7 0x00cbc7a4
int g_CondorSpawnCount = 0;
// GLOBAL: FF7 0x00cbc7a8
short g_CondorChoiceUnaffordable = 0;
unsigned char g_bssPad_00cbc7ac[4] = {0};
// GLOBAL: FF7 0x00cbc7b0
short g_CondorCommandResetShortA = 0;
// GLOBAL: FF7 0x00cbc7b4
short g_CondorCommandResetShortB = 0;
// GLOBAL: FF7 0x00cbc7b8
short g_CondorCycleTimer = 0;
// GLOBAL: FF7 0x00cbc7bc
short g_CondorHeldStepCount = 0;
// GLOBAL: FF7 0x00cbc7c0
short g_CondorUnitDeathActive = 0;
// GLOBAL: FF7 0x00cbc7c4
short g_CondorDyingUnitIndex = 0;
// GLOBAL: FF7 0x00cbc7c8
int g_CondorCommittedCommandCount = 0;
unsigned char g_bssPad_00cbc7cc[4] = {0};
unsigned char g_bssPad_00cbc7d0[8] = {0};
// GLOBAL: FF7 0x00cbc7d8
short g_CondorSelectRowY = 0;
// GLOBAL: FF7 0x00cbc7dc
short g_CondorSelectRowX = 0;
// GLOBAL: FF7 0x00cbc7e0
int g_CondorFunds = 0;
// GLOBAL: FF7 0x00cbc7e4
int g_CondorRecordCost = 0;
// GLOBAL: FF7 0x00cbc7e8
unsigned char g_CondorUnitFrameIndex[32] = {0};
// GLOBAL: FF7 0x00cbc808
unsigned char g_CondorSelectedAttackRangeBuf[4] = {0};
// GLOBAL: FF7 0x00cbc80c
int g_CondorStage = 0;
// g_CondorCollectedRecords, g_CondorCollectedRecords[0].kind are indexed by the code as fields of
// records. The span runs to the next named definition of the TU; the TU's pieces inside
// 0xcbc810..0xcbc910 are aliases.
// GLOBAL: FF7 0x00cbc810
struct CondorLinkRecord g_CondorCollectedRecords[32] = {0};
// GLOBAL: FF7 0x00cbc910
int g_CondorHudInitWordA = 0;
// GLOBAL: FF7 0x00cbc914
int g_CondorHudInitWordB = 0;
// GLOBAL: FF7 0x00cbc918
int g_CondorHudInitScale = 0;
unsigned char g_bssPad_00cbc91c[4] = {0};
// GLOBAL: FF7 0x00cbc920
int g_CondorHudResetWord = 0;
unsigned char g_bssPad_00cbc924[4] = {0};
// GLOBAL: FF7 0x00cbc928
int g_CondorWheelRotation = 0;
// GLOBAL: FF7 0x00cbc92c
short g_CondorWheelRotationZ = 0;
// GLOBAL: FF7 0x00cbc930
short g_CondorActionCursor = 0;
unsigned char g_bssPad_00cbc934[4] = {0};
unsigned char g_bssPad_00cbc938[844] = {0};
// GLOBAL: FF7 0x00cbcc84
int g_CondorBattleClearWordA = 0;
// GLOBAL: FF7 0x00cbcc88
int g_CondorPeakValue = 0;
// GLOBAL: FF7 0x00cbcc8c
unsigned char g_CondorScheduledCount[0x4] = {0};
// GLOBAL: FF7 0x00cbcc90
int g_CondorLinearMotionActive = 0;
// GLOBAL: FF7 0x00cbcc94
short g_CondorDefaultPickedRecord = 0;
// GLOBAL: FF7 0x00cbcc98
short g_CondorMatchCount = 0;
// GLOBAL: FF7 0x00cbcc9c
short g_CondorCursorProbeFlag = 0;
// GLOBAL: FF7 0x00cbcca0
short g_CondorPrevWheelSector = 0;
// GLOBAL: FF7 0x00cbcca4
short g_CondorSelectPanelPhase = 0;
// GLOBAL: FF7 0x00cbcca8
int g_CondorBattleClearWordB = 0;
// GLOBAL: FF7 0x00cbccac
unsigned char g_CondorAlertLevel[0x4] = {0};
// GLOBAL: FF7 0x00cbccb0
int g_CondorCommandPosX = 0;
// GLOBAL: FF7 0x00cbccb4
int g_CondorCommandPosY = 0;
// GLOBAL: FF7 0x00cbccb8
int g_CondorCommandDoubleSpan = 0;
unsigned char g_bssPad_00cbccbc[4] = {0};
// GLOBAL: FF7 0x00cbccc0
CondorCursor g_CondorCursorPos = {0};
unsigned char g_bssPad_00cbccc4[4] = {0};
// GLOBAL: FF7 0x00cbccc8
short g_CondorRampLimitTarget = 0;
// GLOBAL: FF7 0x00cbcccc
short g_CondorInitResetShort = 0;
// GLOBAL: FF7 0x00cbccd0
short g_CondorSavedCameraX = 0;
// GLOBAL: FF7 0x00cbccd4
short g_CondorPickedRecordIndex = 0;
// GLOBAL: FF7 0x00cbccd8
struct CondorRecordTable g_CondorRecordTable = {0};
unsigned char g_bssPad_00cbed34[4] = {0};
unsigned char g_bssPad_00cbed38[112] = {0};
// GLOBAL: FF7 0x00cbeda8
struct CondorSelectAngleState g_CondorSelectAngleState = {0};
// GLOBAL: FF7 0x00cbedac
short g_CondorHudResetShort = 0;
unsigned char g_bssPad_00cbedb0[4] = {0};
// GLOBAL: FF7 0x00cbedb4
short g_CondorPrevOutcomeIndex = 0;
// GLOBAL: FF7 0x00cbedb8
short g_CondorWobbleTimer = 0;
// GLOBAL: FF7 0x00cbedbc
int g_CondorHudBlinkCounter = 0;
// GLOBAL: FF7 0x00cbedc0
short g_CondorBattleOutcome = 0;
// GLOBAL: FF7 0x00cbedc4
int g_CondorBattleClearCopy = 0;
// GLOBAL: FF7 0x00cbedc8
short g_CondorPendingCommandCount = 0;
// GLOBAL: FF7 0x00cbedcc
short g_CondorSelectRowBaseX = 0;
// GLOBAL: FF7 0x00cbedd0
short g_CondorSelectRowBase = 0;
// GLOBAL: FF7 0x00cbedd4
short g_CondorWheelAngleOffset = 0;
// GLOBAL: FF7 0x00cbedd8
short g_CondorOutcomeIndex = 0;
// GLOBAL: FF7 0x00cbeddc
short g_CondorSavedSelectedRecord = 0;
// GLOBAL: FF7 0x00cbede0
double g_CondorTicksPerFrame = 0.0;
// GLOBAL: FF7 0x00cbede8
Timestamp64 g_CondorTimestampNow = {0};
unsigned char g_bssPad_00cbedf0[64] = {0};
// GLOBAL: FF7 0x00cbee30
Timestamp64 g_CondorFrameTimestamp = {0};
// clang-format on
void __cdecl Mini_CondorDrawTextWithColors(int, int, char *text, HDC dc);

/*
 * Enters Fort Condor (game mode GAME_MODE_FORT_CONDOR), the module's installed enter callback.
 * Sets the frame duration to the timestamp frequency / 60 (a 60 fps battle), turns frame pacing
 * on (g_CondorMinigameRunning) and selects loading from condor.lgp (g_CondorUsesLgp). Then, under a
 * constant `if (1)`:
 * - sets the near and far planes to 100.0 and 32000.0 and selects Direct3D 5;
 * - applies the GTE screen distance g_CondorScreenReferenceScale (220.0, set by the viewport
 *   preset) and sets the animation transform flags to 0x4051;
 * - clears g_CondorAppState, sets the default model load flags to 0x100000 and scene pass 0, and
 *   caches the DirectDraw object and surfaces;
 * - reads the current directory into a local buffer nothing uses;
 * - sets up the sound command buffer, loads the sound table, sets all SFX channels to volume
 *   0x7F and resets the cursor state;
 * - caches the display pixel format and applies the viewport the UI scaling preset chose;
 * - loads the 29 unit model sets with load flag 0x2000000 and no blending;
 * - resets the load options with blend mode 4 (BLEND_NONE) and hands them to
 *   `Mini_CondorLoadSavemapState`, which reads the savemap, loads the battle data and textures,
 *   resets the game and starts the music.
 */
// FUNCTION: FF7 0x005f47b5
void __cdecl Mini_CondorEnter(GfxEngineContext *context)
{
    char buf[0x80];
    int initialized;
    PfileLoadOptions optsBuf;
    PfileLoadOptions *opts;
    int mode;

    initialized = 0;
    opts = &optsBuf;
    g_CondorTicksPerFrame = context->tscFrequency / 60.0;
    g_CondorMinigameRunning = 1;
    g_CondorUsesLgp = 1;
    if (1) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        context->nearPlane = 100.0f;
        context->farPlane = 32000.0f;
        context->useDirect3D5 = 1;
        SetGeomScreenReference(*(float *)&g_CondorScreenReferenceScale);
        Anim_GetDefaultTransformFlags(&mode);
        mode = 0x4051;
        Anim_SetTransformFlags(&mode);
        initialized = 1;
        g_CondorAppState = 0;
        Pfile_SetDefaultModelFlags(0x100000);
        Gfx_SetScenePassCall(0, context);
        Mini_CondorCacheDirectDrawObjects(context);
        GetCurrentDirectoryA(0x100, buf);
        Mini_CondorInitSfxCommandBuffer();
        Mini_CondorLoadSoundTable();
        SfxSetAllChannelsVolume(0x7f);
        Input_ResetCursorState();
        Mini_CondorCacheDisplayFormat(context);
        GfxSetViewportRect(g_CondorViewportX, g_CondorViewportY, g_CondorViewportWidth, g_CondorViewportHeight,
                           context);
        Pfile_InitLoadOptions(opts);
        opts->flags |= 0x2000000;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, opts);
        Mini_CondorLoadUnitModels(opts, context);
        Pfile_InitLoadOptionsWithBlend(BLEND_NONE, opts);
        Mini_CondorLoadSavemapState(context, opts);
    }
}
/*
 * Cleans up Fort Condor resources and requests module switch back to the field map.
 */
// FUNCTION: FF7 0x005f4971
void __cdecl Mini_CondorReturnToField(GfxEngineContext *switchParam)
{
    GameModule moduleTable;
    g_EnginePreviousMode = GAME_MODE_FORT_CONDOR;
    g_EngineGameMode = GAME_MODE_FIELD;
    moduleTable.frame = GameModeDispatcher_Frame;
    moduleTable.onMouse = App_OnMouseMessage;
    moduleTable.onKey = App_OnKeyMessage;
    moduleTable.enter = GameModeDispatcher_Enter;
    moduleTable.exit = GameModeDispatcher_Exit;
    RequestModuleSwitch(&moduleTable, switchParam);
}
/*
 * Leaves Fort Condor, the module's installed exit callback, which the dispatcher runs after
 * `Mini_CondorReturnToField` requested the switch. Writes the result back to the savemap: the
 * player and enemy unit counts, the spawns left, the flag at +0xC50 (1 when the exit word
 * g_CondorStage is 4, the end of outcome 2: no player units and under 400 gil) and the gil at
 * +0xB7C. Then releases the texture render objects and data files, the TIM surfaces and
 * palettes, the cached SFX buffers and the unit models, clears the cached DirectDraw object and
 * primary surface, and sets g_CondorAppState to 1.
 */
// FUNCTION: FF7 0x005f49c0
void __cdecl Mini_CondorExit(int context)
{
    Mini_CondorStoreSavemapState();
    Mini_CondorReleaseTexturesAndDataFiles();
    Mini_CondorFreeTimSurfaceAndPaletteRecords();
    Mini_CondorReleaseCachedSfxBuffers();
    Mini_CondorFreeUnitModels(context);
    g_CondorDirectDraw = 0;
    g_CondorPrimarySurface = 0;
    g_CondorAppState = 1;
}
/*
 * Renders debug or status text string to a device context at specified coordinates.
 */
// FUNCTION: FF7 0x005f4a03
void __cdecl Mini_CondorDrawTextWithColors(int x, int y, char *text, HDC dc)
{
    SetBkColor(dc, 0xff0000);
    SetTextColor(dc, 0xffff);
    TextOutA(dc, x, y, text, lstrlenA(text));
}
/*
 * Fort Condor's installed frame callback. Stamps the frame start time for the frame pacing,
 * clears the screen, reads the input and runs the battle state handlers
 * (`Mini_CondorUpdateFrameInput`, which returns the phase: 1 to restart when the button mask
 * 0x90F is held, the exit word when it is set, else 0), and runs that phase
 * (`Mini_CondorDispatchPhase`). When the exit word g_CondorStage has reached 4 or more the battle
 * is over and `Mini_CondorReturnToField` switches back to the field; otherwise `Mini_CondorTick`
 * draws the scene, holds the frame to 60 fps and resets the draw pools and input latches.
 */
// FUNCTION: FF7 0x005f4a47
void __cdecl Mini_CondorFrame(GfxEngineContext *context)
{
    int flag;
    int phase;

    ReadTimestampCounter(&g_CondorFrameTimestamp);
    flag = 1;
    GfxClearCall(0, 1, 1, context);
    phase = Mini_CondorUpdateFrameInput((int *)&g_CondorCameraMatrix, g_CondorMatchListState.cameraWorkMatrix);
    Mini_CondorDispatchPhase((short)phase);
    if (g_CondorStage >= 4) {
        Mini_CondorReturnToField(context);
    } else {
        Mini_CondorTick(context);
    }
}
/*
 * Fort Condor's installed mouse callback. It does nothing: the mouse buttons and cursor are read
 * by the input latch code each frame when the cursor mode (F12) is on.
 */
// FUNCTION: FF7 0x005f4abc
void __cdecl Mini_CondorMouse(void)
{
    // The original is empty (five code bytes plus any alignment padding).
}
/*
 * Fort Condor's installed keyboard callback; only WM_KEYDOWN (0x100) does anything:
 * - F11 (0x7A) toggles g_CondorAppFlag, which no other code reads;
 * - F12 (0x7B) toggles the cursor mode g_CondorCursorMode; turning it on also resets the cursor
 *   tracking. With the cursor mode on, the mouse buttons act as Circle and Cross and the mouse
 *   moves the cursor; with it off, the cursor scrolls the camera at the screen edges.
 * Shift (0x10) has an empty case.
 */
// FUNCTION: FF7 0x005f4ac1
void __cdecl Mini_CondorKeyboard(int message, int key)
{
    int val;
    switch (message) {
    case WM_KEYDOWN:
        val = key;
        switch (val) {
        case VK_SHIFT:
            break;
        case VK_F11:
            if (g_CondorAppFlag != 0) {
                g_CondorAppFlag = 0;
            } else {
                g_CondorAppFlag = 1;
            }
            break;
        case VK_F12:
            if (g_CondorCursorMode != 0) {
                g_CondorCursorMode = 0;
            } else {
                g_CondorCursorMode = 1;
                Input_ResetCursorStateKeepMode();
            }
            break;
        }
        break;
    }
}
/*
 * Creates and initializes the 3D graphics engine context and configuration for Fort Condor.
 */
// FUNCTION: FF7 0x005f4b43
void __cdecl Mini_CondorInitEngineContext(int context, int params)
{
    GfxEngineContext *ctx;
    int attr;
    char *name;
    SetGameHeapTrackingEnabled(0);
    ctx = (GfxEngineContext *)CreateEngineContext();
    if (ctx != 0) {
        ctx->colorDepth = 0x10;
        ctx->fullScreen = 1;
        ctx->modeActive = 1;
        ctx->modeTransition = 0;
        ctx->modeSetupDone = 1;
        if (ctx->modeSetupDone != 0) {
            ctx->modeLoaded = 1;
        }
        ctx->deviceIndex = 0;
        ctx->showCursor = 1;
        ctx->nearPlane = 100.0f;
        ctx->farPlane = 32000.0f;
        ctx->useDirect3D5 = 1;
        SetGeomScreenReference(*(float *)&g_CondorScreenReferenceScale);
        Anim_GetDefaultTransformFlags(&attr);
        attr = 0x4051;
        Anim_SetTransformFlags(&attr);
        // STRING: FF7 0x00901a84
        name = "APP.CFG";
        Gfx_LoadAppConfigFile(name, (GfxEngineContext *)ctx);
        SetEngineContext((int)ctx);
    }
}
/*
 * Empty scene hook function.
 */
// FUNCTION: FF7 0x005f4c5a
void __cdecl Mini_CondorSceneHookNoOp(GfxEngineContext *context)
{
    char unused[0xD4];
}
} /* extern "C" */
