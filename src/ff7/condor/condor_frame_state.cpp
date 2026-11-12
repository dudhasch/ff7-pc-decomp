// src/ff7/condor/condor_frame_state.cpp
// Data-only object: .bss 0x00cbee38..0x00cbf108. No code.
// A data-only member of the condor library, which its neighbour cd_app.cpp never references: Fort Condor's
// frame timing and TIM surface state.

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

// clang-format off
// GLOBAL: FF7 0x00cbee38
int g_CondorFrameLate = 0;
unsigned char g_bssPad_00cbee3c[4] = {0};
// GLOBAL: FF7 0x00cbee40
double g_CondorFrameTimeCarry = 0.0;
// GLOBAL: FF7 0x00cbee48
int g_CondorDimmedTimSurface0 = 0;
// GLOBAL: FF7 0x00cbee4c
int g_CondorDimmedTimSurface1 = 0;
// GLOBAL: FF7 0x00cbee50
int g_CondorDimmedTimSurface2 = 0;
// GLOBAL: FF7 0x00cbee54
int g_CondorTimSurface = 0;
// GLOBAL: FF7 0x00cbee58
int g_CondorTimId = 0;
// GLOBAL: FF7 0x00cbee5c
int g_CondorTimFlags = 0;
// GLOBAL: FF7 0x00cbee60
int g_CondorTimPixelData = 0;
// GLOBAL: FF7 0x00cbee64
int g_CondorTimBlockSize = 0;
// GLOBAL: FF7 0x00cbee68
short g_CondorTimClutOrigin[2] = {0};
// GLOBAL: FF7 0x00cbee6c
short g_CondorTimClutSize[2] = {0};
unsigned char g_bssPad_00cbee70[4] = {0};
// GLOBAL: FF7 0x00cbee74
int g_pCondorTimData = 0;
// GLOBAL: FF7 0x00cbee78
int g_CondorTimImageBlockSize = 0;
// GLOBAL: FF7 0x00cbee7c
short g_CondorTimImageOrigin[2] = {0};
// GLOBAL: FF7 0x00cbee80
short g_CondorTimImageSize[2] = {0};
unsigned char g_bssPad_00cbee84[4] = {0};
unsigned char g_bssPad_00cbee88[28] = {0};
// GLOBAL: FF7 0x00cbeea4
int g_pCondorTimImage2Data = 0;
// GLOBAL: FF7 0x00cbeea8
int g_CondorTimImage2BlockSize = 0;
// GLOBAL: FF7 0x00cbeeac
short g_CondorTimImage2Origin[2] = {0};
// GLOBAL: FF7 0x00cbeeb0
short g_CondorTimImage2Size[2] = {0};
unsigned char g_bssPad_00cbeeb4[4] = {0};
// GLOBAL: FF7 0x00cbeeb8
int g_CondorDisplayFormat = 0;
// GLOBAL: FF7 0x00cbeebc
struct CondorTimPixelRecord *g_pCondorTimPixelRecords = 0;
// GLOBAL: FF7 0x00cbeec0
unsigned short g_CondorTimPixelRecordSize[2] = {0};
// GLOBAL: FF7 0x00cbeec4
unsigned short g_CondorTimHeight = 0;
// GLOBAL: FF7 0x00cbeec8
int g_CondorConfigEntryBuffer = 0;
unsigned char g_bssPad_00cbeecc[4] = {0};
// GLOBAL: FF7 0x00cbeed0
int g_CondorGilDigitSprites[15] = {0};
// GLOBAL: FF7 0x00cbef0c
int g_CondorGilDigitCount = 0;
unsigned char g_bssPad_00cbef10[4] = {0};
// GLOBAL: FF7 0x00cbef14
int g_CondorGilDigitsVisible = 0;
// GLOBAL: FF7 0x00cbef18
int g_CondorCostDigitSprites[15] = {0};
// GLOBAL: FF7 0x00cbef54
int g_CondorCostDigitCount = 0;
unsigned char g_bssPad_00cbef58[4] = {0};
// GLOBAL: FF7 0x00cbef5c
int g_CondorCostDigitsVisible = 0;
// GLOBAL: FF7 0x00cbef60
int g_CondorCounterDigitSprites[15] = {0};
// GLOBAL: FF7 0x00cbef9c
int g_CondorCounterDigitCount = 0;
unsigned char g_bssPad_00cbefa0[4] = {0};
// GLOBAL: FF7 0x00cbefa4
int g_CondorCounterDigitsVisible = 0;
// GLOBAL: FF7 0x00cbefa8
int g_CondorScaleDigitSprites[15] = {0};
// GLOBAL: FF7 0x00cbefe4
int g_CondorScaleDigitCount = 0;
unsigned char g_bssPad_00cbefe8[4] = {0};
// GLOBAL: FF7 0x00cbefec
int g_CondorScaleDigitsVisible = 0;
// GLOBAL: FF7 0x00cbeff0
int g_CondorRangeDigitSprites[15] = {0};
// GLOBAL: FF7 0x00cbf02c
int g_CondorRangeDigitCount = 0;
unsigned char g_bssPad_00cbf030[4] = {0};
// GLOBAL: FF7 0x00cbf034
int g_CondorRangeDigitsVisible = 0;
unsigned char g_bssPad_00cbf038[72] = {0};
// GLOBAL: FF7 0x00cbf080
int g_CondorFirstNumberLowSlot = 0;
// GLOBAL: FF7 0x00cbf084
int g_CondorFirstNumberHighSlot = 0;
unsigned char g_bssPad_00cbf088[52] = {0};
// GLOBAL: FF7 0x00cbf0bc
int g_CondorFirstNumberDigitCount = 0;
unsigned char g_bssPad_00cbf0c0[4] = {0};
// GLOBAL: FF7 0x00cbf0c4
int g_CondorFirstNumberVisible = 0;
// GLOBAL: FF7 0x00cbf0c8
int g_CondorSecondNumberLowSlot = 0;
// GLOBAL: FF7 0x00cbf0cc
int g_CondorSecondNumberHighSlot = 0;
unsigned char g_bssPad_00cbf0d0[52] = {0};
// GLOBAL: FF7 0x00cbf104
int g_CondorSecondNumberDigitCount = 0;
// clang-format on

} /* extern "C" */
