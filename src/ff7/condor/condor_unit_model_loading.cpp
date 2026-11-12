// src/ff7/condor/condor_unit_model_loading.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Loads, names and frees the Fort Condor unit model sets in the global model table, and
//   draws a unit model with its transform and color state.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/api/condor.h"
#include "lib/api/graphics.h"
#include "lib/api/polygon.h"
#include "ff7/types/condor.h"
#include "lib/types/graphics.h"
#include "lib/pfile.h"
#include "lib/types/polygon.h"
#include <windows.h>
#include <ddraw.h>
#include "lib/types/mem.h"
#include "lib/codegen.h"
#include "ff7/enums/condor.h"
#include "ff7/savemap.h"

#pragma intrinsic(strcpy, strcat)
extern "C" {

#define g_CondorUnitModelFileTable (((CondorUnitModelSlotView *)&g_CondorUnitModelDefs)->fileTable)

// clang-format on

// GLOBAL: FF7 0x00901b70
int g_CondorBannerIndex = (-1);
// GLOBAL: FF7 0x00901b74
int g_CondorRecordDisplaySlot = (-1);
// GLOBAL: FF7 0x00901b78
int g_CondorSelectedReadoutUnit = -1;
// clang-format off
// GLOBAL: FF7 0x00901b80
unsigned int g_CondorUnitModelDefs[908] = {0x00000009, (unsigned int)((unsigned char *)g_CondorUnitModelDefs + 0xe0c), (unsigned int)&g_CondorRsdPath_un00_31, (unsigned int)(g_CondorRsdPath_un00_31 + 36), (unsigned int)&g_CondorRsdPath_un00_33, (unsigned int)(g_CondorRsdPath_un00_33 + 36), (unsigned int)&g_CondorRsdPath_un00_41, (unsigned int)(g_CondorRsdPath_un00_41 + 36), (unsigned int)&g_CondorRsdPath_un00_43, (unsigned int)(g_CondorRsdPath_un00_43 + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000019, (unsigned int)&g_CondorRsdPath_un01_00, (unsigned int)(g_CondorRsdPath_un01_00 + 36), (unsigned int)&g_CondorRsdPath_un01_12, (unsigned int)(g_CondorRsdPath_un01_12 + 36), (unsigned int)&g_CondorRsdPath_un01_14, (unsigned int)(g_CondorRsdPath_un01_14 + 36), (unsigned int)&g_CondorRsdPath_un01_16, (unsigned int)(g_CondorRsdPath_un01_16 + 36), (unsigned int)&g_CondorRsdPath_un01_22, (unsigned int)(g_CondorRsdPath_un01_22 + 36), (unsigned int)&g_CondorRsdPath_un01_24, (unsigned int)(g_CondorRsdPath_un01_24 + 36), (unsigned int)&g_CondorRsdPath_un01_26, (unsigned int)(g_CondorRsdPath_un01_26 + 36), (unsigned int)&g_CondorRsdPath_un01b_00, (unsigned int)(g_CondorRsdPath_un01b_00 + 36), (unsigned int)&g_CondorRsdPath_un01b_22, (unsigned int)(g_CondorRsdPath_un01b_22 + 36), (unsigned int)&g_CondorRsdPath_un01b_24, (unsigned int)(g_CondorRsdPath_un01b_24 + 36), (unsigned int)&g_CondorRsdPath_un01b_26, (unsigned int)(g_CondorRsdPath_un01b_26 + 36), (unsigned int)&g_CondorRsdPath_un01_31, (unsigned int)(g_CondorRsdPath_un01_31 + 36), (unsigned int)&g_CondorRsdPath_un01_33, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000017, (unsigned int)(g_CondorRsdPath_un01_33 + 36), (unsigned int)&g_CondorRsdPath_un02_11, (unsigned int)(g_CondorRsdPath_un02_11 + 36), (unsigned int)&g_CondorRsdPath_un02_13, (unsigned int)(g_CondorRsdPath_un02_13 + 36), (unsigned int)&g_CondorRsdPath_un02_15, (unsigned int)(g_CondorRsdPath_un02_15 + 36), (unsigned int)&g_CondorRsdPath_un02_31, (unsigned int)(g_CondorRsdPath_un02_31 + 36), (unsigned int)&g_CondorRsdPath_un02_33, (unsigned int)(g_CondorRsdPath_un02_33 + 36), (unsigned int)&g_CondorRsdPath_un02_22, (unsigned int)(g_CondorRsdPath_un02_22 + 36), (unsigned int)&g_CondorRsdPath_un02_24, (unsigned int)(g_CondorRsdPath_un02_24 + 36), (unsigned int)&g_CondorRsdPath_un02_26, (unsigned int)(g_CondorRsdPath_un02_26 + 36), (unsigned int)&g_CondorRsdPath_un02b_21, (unsigned int)(g_CondorRsdPath_un02b_21 + 36), (unsigned int)&g_CondorRsdPath_un02b_23, (unsigned int)(g_CondorRsdPath_un02b_23 + 36), (unsigned int)&g_CondorRsdPath_un02b_25, (unsigned int)(g_CondorRsdPath_un02b_25 + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000001a, (unsigned int)&g_CondorRsdPath_un03_00, (unsigned int)(g_CondorRsdPath_un03_00 + 36), (unsigned int)&g_CondorRsdPath_un03_12, (unsigned int)(g_CondorRsdPath_un03_12 + 36), (unsigned int)&g_CondorRsdPath_un03_14, (unsigned int)(g_CondorRsdPath_un03_14 + 36), (unsigned int)&g_CondorRsdPath_un03_16, (unsigned int)(g_CondorRsdPath_un03_16 + 36), (unsigned int)&g_CondorRsdPath_un03_32, (unsigned int)(g_CondorRsdPath_un03_32 + 36), (unsigned int)&g_CondorRsdPath_un03_21, (unsigned int)(g_CondorRsdPath_un03_21 + 36), (unsigned int)&g_CondorRsdPath_un03_23, (unsigned int)(g_CondorRsdPath_un03_23 + 36), (unsigned int)&g_CondorRsdPath_un03_25, (unsigned int)(g_CondorRsdPath_un03_25 + 36), (unsigned int)&g_CondorRsdPath_un03_27, (unsigned int)(g_CondorRsdPath_un03_27 + 36), (unsigned int)&g_CondorRsdPath_un03b_21, (unsigned int)(g_CondorRsdPath_un03b_21 + 36), (unsigned int)&g_CondorRsdPath_un03b_23, (unsigned int)(g_CondorRsdPath_un03b_23 + 36), (unsigned int)&g_CondorRsdPath_un03b_25, (unsigned int)(g_CondorRsdPath_un03b_25 + 36), (unsigned int)&g_CondorRsdPath_un03b_27, (unsigned int)(g_CondorRsdPath_un03b_27 + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000000d, (unsigned int)&g_CondorRsdPath_un04_00, (unsigned int)(g_CondorRsdPath_un04_00 + 36), (unsigned int)&g_CondorRsdPath_un04_12, (unsigned int)(g_CondorRsdPath_un04_12 + 36), (unsigned int)&g_CondorRsdPath_un04_14, (unsigned int)(g_CondorRsdPath_un04_14 + 36), (unsigned int)&g_CondorRsdPath_un04_22, (unsigned int)(g_CondorRsdPath_un04_22 + 36), (unsigned int)&g_CondorRsdPath_un04_24, (unsigned int)(g_CondorRsdPath_un04_24 + 36), (unsigned int)&g_CondorRsdPath_un04_31, (unsigned int)(g_CondorRsdPath_un04_31 + 36), (unsigned int)&g_CondorRsdPath_un04_33, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000010, (unsigned int)(g_CondorRsdPath_un04_33 + 36), (unsigned int)&g_CondorRsdPath_un05_21, (unsigned int)(g_CondorRsdPath_un05_21 + 36), (unsigned int)&g_CondorRsdPath_un05_23, (unsigned int)(g_CondorRsdPath_un05_23 + 36), (unsigned int)&g_CondorRsdPath_un05_25, (unsigned int)(g_CondorRsdPath_un05_25 + 36), (unsigned int)&g_CondorRsdPath_un05_32, (unsigned int)(g_CondorRsdPath_un05_32 + 36), (unsigned int)&g_CondorRsdPath_un05_42, (unsigned int)(g_CondorRsdPath_un05_42 + 36), (unsigned int)&g_CondorRsdPath_un05_44, (unsigned int)(g_CondorRsdPath_un05_44 + 36), (unsigned int)&g_CondorRsdPath_un05_52, (unsigned int)(g_CondorRsdPath_un05_52 + 36), (unsigned int)&g_CondorRsdPath_un05_54, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000010, (unsigned int)(g_CondorRsdPath_un05_54 + 36), (unsigned int)&g_CondorRsdPath_un06_21, (unsigned int)(g_CondorRsdPath_un06_21 + 36), (unsigned int)&g_CondorRsdPath_un06_23, (unsigned int)(g_CondorRsdPath_un06_23 + 36), (unsigned int)&g_CondorRsdPath_un06_31, (unsigned int)(g_CondorRsdPath_un06_31 + 36), (unsigned int)&g_CondorRsdPath_un06_41, (unsigned int)(g_CondorRsdPath_un06_41 + 36), (unsigned int)&g_CondorRsdPath_un06_43, (unsigned int)(g_CondorRsdPath_un06_43 + 36), (unsigned int)&g_CondorRsdPath_un06_45, (unsigned int)(g_CondorRsdPath_un06_45 + 36), (unsigned int)&g_CondorRsdPath_un06_52, (unsigned int)(g_CondorRsdPath_un06_52 + 36), (unsigned int)&g_CondorRsdPath_un06_54, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000000f, (unsigned int)(g_CondorRsdPath_un06_54 + 36), (unsigned int)&g_CondorRsdPath_un07_21, (unsigned int)(g_CondorRsdPath_un07_21 + 36), (unsigned int)&g_CondorRsdPath_un07_23, (unsigned int)(g_CondorRsdPath_un07_23 + 36), (unsigned int)&g_CondorRsdPath_un07_31, (unsigned int)(g_CondorRsdPath_un07_31 + 36), (unsigned int)&g_CondorRsdPath_un07_41, (unsigned int)(g_CondorRsdPath_un07_41 + 36), (unsigned int)&g_CondorRsdPath_un07_43, (unsigned int)(g_CondorRsdPath_un07_43 + 36), (unsigned int)&g_CondorRsdPath_un07_51, (unsigned int)(g_CondorRsdPath_un07_51 + 36), (unsigned int)&g_CondorRsdPath_un07_53, (unsigned int)(g_CondorRsdPath_un07_53 + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000013, (unsigned int)&g_CondorRsdPath_un08_00, (unsigned int)(g_CondorRsdPath_un08_00 + 36), (unsigned int)&g_CondorRsdPath_un08_02, (unsigned int)(g_CondorRsdPath_un08_02 + 36), (unsigned int)&g_CondorRsdPath_un08_04, (unsigned int)(g_CondorRsdPath_un08_04 + 36), (unsigned int)&g_CondorRsdPath_un08_22, (unsigned int)(g_CondorRsdPath_un08_22 + 36), (unsigned int)&g_CondorRsdPath_un08_24, (unsigned int)(g_CondorRsdPath_un08_24 + 36), (unsigned int)&g_CondorRsdPath_un08_32, (unsigned int)(g_CondorRsdPath_un08_32 + 36), (unsigned int)&g_CondorRsdPath_un08_42, (unsigned int)(g_CondorRsdPath_un08_42 + 36), (unsigned int)&g_CondorRsdPath_un08_44, (unsigned int)(g_CondorRsdPath_un08_44 + 36), (unsigned int)&g_CondorRsdPath_un08_52, (unsigned int)(g_CondorRsdPath_un08_52 + 36), (unsigned int)&g_CondorRsdPath_un08_54, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, (unsigned int)(g_CondorRsdPath_un08_54 + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, (unsigned int)&g_CondorRsdPath_un10_00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000005, (unsigned int)(g_CondorRsdPath_un10_00 + 36), (unsigned int)&g_CondorRsdPath_un11_01, (unsigned int)(g_CondorRsdPath_un11_01 + 36), (unsigned int)&g_CondorRsdPath_un11_03, (unsigned int)(g_CondorRsdPath_un11_03 + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000001c, (unsigned int)&g_CondorRsdPath_un12_00, (unsigned int)(g_CondorRsdPath_un12_00 + 36), (unsigned int)&g_CondorRsdPath_un12_12, (unsigned int)(g_CondorRsdPath_un12_12 + 36), (unsigned int)&g_CondorRsdPath_un12_14, (unsigned int)(g_CondorRsdPath_un12_14 + 36), (unsigned int)&g_CondorRsdPath_un12_32, (unsigned int)(g_CondorRsdPath_un12_32 + 36), (unsigned int)&g_CondorRsdPath_un12_21, (unsigned int)(g_CondorRsdPath_un12_21 + 36), (unsigned int)&g_CondorRsdPath_un12_23, (unsigned int)(g_CondorRsdPath_un12_23 + 36), (unsigned int)&g_CondorRsdPath_un12_25, (unsigned int)(g_CondorRsdPath_un12_25 + 36), (unsigned int)&g_CondorRsdPath_un12b_00, (unsigned int)(g_CondorRsdPath_un12b_00 + 36), (unsigned int)&g_CondorRsdPath_un12b_22, (unsigned int)(g_CondorRsdPath_un12b_22 + 36), (unsigned int)&g_CondorRsdPath_un12b_24, (unsigned int)(g_CondorRsdPath_un12b_24 + 36), (unsigned int)&g_CondorRsdPath_un12b_26, (unsigned int)(g_CondorRsdPath_un12b_26 + 36), (unsigned int)&g_CondorRsdPath_un12_62, (unsigned int)(g_CondorRsdPath_un12_62 + 36), (unsigned int)&g_CondorRsdPath_un12_64, (unsigned int)(g_CondorRsdPath_un12_64 + 36), (unsigned int)&g_CondorRsdPath_un12_66, (unsigned int)(g_CondorRsdPath_un12_66 + 36), 0x00000000, 0x00000000, 0x00000014, (unsigned int)&g_CondorRsdPath_un13_00, (unsigned int)(g_CondorRsdPath_un13_00 + 36), (unsigned int)&g_CondorRsdPath_un13_12, (unsigned int)(g_CondorRsdPath_un13_12 + 36), (unsigned int)&g_CondorRsdPath_un13_14, (unsigned int)(g_CondorRsdPath_un13_14 + 36), (unsigned int)&g_CondorRsdPath_un13_32, (unsigned int)(g_CondorRsdPath_un13_32 + 36), (unsigned int)&g_CondorRsdPath_un13_21, (unsigned int)(g_CondorRsdPath_un13_21 + 36), (unsigned int)&g_CondorRsdPath_un13_23, (unsigned int)(g_CondorRsdPath_un13_23 + 36), (unsigned int)&g_CondorRsdPath_un13_25, (unsigned int)(g_CondorRsdPath_un13_25 + 36), (unsigned int)&g_CondorRsdPath_un13_61, (unsigned int)(g_CondorRsdPath_un13_61 + 36), (unsigned int)&g_CondorRsdPath_un13_63, (unsigned int)(g_CondorRsdPath_un13_63 + 36), (unsigned int)&g_CondorRsdPath_un13_65, (unsigned int)(g_CondorRsdPath_un13_65 + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, (unsigned int)&g_CondorRsdPath_un10_00_copy2, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, (unsigned int)(g_CondorRsdPath_un10_00_copy2 + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000010, (unsigned int)&g_CondorRsdPath_un16_00, (unsigned int)(g_CondorRsdPath_un16_00 + 36), (unsigned int)&g_CondorRsdPath_un16_12, (unsigned int)(g_CondorRsdPath_un16_12 + 36), (unsigned int)&g_CondorRsdPath_un16_14, (unsigned int)(g_CondorRsdPath_un16_14 + 36), (unsigned int)&g_CondorRsdPath_un16_16, (unsigned int)(g_CondorRsdPath_un16_16 + 36), (unsigned int)&g_CondorRsdPath_un16_32, (unsigned int)(g_CondorRsdPath_un16_32 + 36), (unsigned int)&g_CondorRsdPath_un16_21, (unsigned int)(g_CondorRsdPath_un16_21 + 36), (unsigned int)&g_CondorRsdPath_un16_23, (unsigned int)(g_CondorRsdPath_un16_23 + 36), (unsigned int)&g_CondorRsdPath_un16_25, (unsigned int)(g_CondorRsdPath_un16_25 + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000013, (unsigned int)&g_CondorRsdPath_un17_00, (unsigned int)(g_CondorRsdPath_un17_00 + 36), (unsigned int)&g_CondorRsdPath_un17_12, (unsigned int)(g_CondorRsdPath_un17_12 + 36), (unsigned int)&g_CondorRsdPath_un17_14, (unsigned int)(g_CondorRsdPath_un17_14 + 36), (unsigned int)&g_CondorRsdPath_un17_16, (unsigned int)(g_CondorRsdPath_un17_16 + 36), (unsigned int)&g_CondorRsdPath_un17_22, (unsigned int)(g_CondorRsdPath_un17_22 + 36), (unsigned int)&g_CondorRsdPath_un17_24, (unsigned int)(g_CondorRsdPath_un17_24 + 36), (unsigned int)&g_CondorRsdPath_un17_26, (unsigned int)(g_CondorRsdPath_un17_26 + 36), (unsigned int)&g_CondorRsdPath_un17_28, (unsigned int)(g_CondorRsdPath_un17_28 + 36), (unsigned int)&g_CondorRsdPath_un17_31, (unsigned int)(g_CondorRsdPath_un17_31 + 36), (unsigned int)&g_CondorRsdPath_un17_33, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000000f, (unsigned int)(g_CondorRsdPath_un17_33 + 36), (unsigned int)&g_CondorRsdPath_un18_11, (unsigned int)(g_CondorRsdPath_un18_11 + 36), (unsigned int)&g_CondorRsdPath_un18_13, (unsigned int)(g_CondorRsdPath_un18_13 + 36), (unsigned int)&g_CondorRsdPath_un18_15, (unsigned int)(g_CondorRsdPath_un18_15 + 36), (unsigned int)&g_CondorRsdPath_un18_31, (unsigned int)(g_CondorRsdPath_un18_31 + 36), (unsigned int)&g_CondorRsdPath_un18_33, (unsigned int)(g_CondorRsdPath_un18_33 + 36), (unsigned int)&g_CondorRsdPath_un18_22, (unsigned int)(g_CondorRsdPath_un18_22 + 36), (unsigned int)&g_CondorRsdPath_un18_24, (unsigned int)(g_CondorRsdPath_un18_24 + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000000d, (unsigned int)&g_CondorRsdPath_un19_00, (unsigned int)(g_CondorRsdPath_un19_00 + 36), (unsigned int)&g_CondorRsdPath_un19_12, (unsigned int)(g_CondorRsdPath_un19_12 + 36), (unsigned int)&g_CondorRsdPath_un19_14, (unsigned int)(g_CondorRsdPath_un19_14 + 36), (unsigned int)&g_CondorRsdPath_un19_32, (unsigned int)(g_CondorRsdPath_un19_32 + 36), (unsigned int)&g_CondorRsdPath_un19_21, (unsigned int)(g_CondorRsdPath_un19_21 + 36), (unsigned int)&g_CondorRsdPath_un19_23, (unsigned int)(g_CondorRsdPath_un19_23 + 36), (unsigned int)&g_CondorRsdPath_un19_25, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, (unsigned int)(g_CondorRsdPath_un19_25 + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, (unsigned int)&g_CondorRsdPath_un10_00_copy3, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, (unsigned int)(g_CondorRsdPath_un10_00_copy3 + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, (unsigned int)&g_CondorRsdPath_un10_00_copy4, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000006, (unsigned int)(g_CondorRsdPath_un10_00_copy4 + 36), (unsigned int)&g_CondorRsdPath_un06_obj, (unsigned int)(g_CondorRsdPath_un06_obj + 36), (unsigned int)&g_CondorRsdPath_un08_obj, (unsigned int)(g_CondorRsdPath_un08_obj + 36), (unsigned int)&g_CondorRsdPath_un19_obj, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, (unsigned int)(g_CondorRsdPath_un19_obj + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, (unsigned int)&g_CondorRsdPath_yazi, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000003, (unsigned int)(g_CondorRsdPath_yazi + 36), (unsigned int)&g_CondorRsdPath_leng01, (unsigned int)(g_CondorRsdPath_leng01 + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000006, (unsigned int)&g_CondorRsdPath_un01b_00_copy2, (unsigned int)(g_CondorRsdPath_un01b_00_copy2 + 36), (unsigned int)&g_CondorRsdPath_un03b_00, (unsigned int)(g_CondorRsdPath_un03b_00 + 36), (unsigned int)&g_CondorRsdPath_un12b_00_copy2, (unsigned int)(g_CondorRsdPath_un12b_00_copy2 + 36), 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3766665c, 0x6e6f635c, 0x5c726f64, 0x72616863, 0x306e755c, 0x6e755c30, 0x305f3030, 0x73722e30, 0x00000064};
// GLOBAL: FF7 0x009029b0
unsigned char g_CondorRsdPath_un00_31[72] =
    "\\ff7\\condor\\char\\un00\\un00_31.rsd\0\0\0"
    "\\ff7\\condor\\char\\un00\\un00_32.rsd";
// GLOBAL: FF7 0x009029f8
unsigned char g_CondorRsdPath_un00_33[72] =
    "\\ff7\\condor\\char\\un00\\un00_33.rsd\0\0\0"
    "\\ff7\\condor\\char\\un00\\un00_34.rsd";
// GLOBAL: FF7 0x00902a40
unsigned char g_CondorRsdPath_un00_41[72] =
    "\\ff7\\condor\\char\\un00\\un00_41.rsd\0\0\0"
    "\\ff7\\condor\\char\\un00\\un00_42.rsd";
// GLOBAL: FF7 0x00902a88
unsigned char g_CondorRsdPath_un00_43[72] =
    "\\ff7\\condor\\char\\un00\\un00_43.rsd\0\0\0"
    "\\ff7\\condor\\char\\un00\\un00_44.rsd";
// GLOBAL: FF7 0x00902ad0
unsigned char g_CondorRsdPath_un01_00[72] =
    "\\ff7\\condor\\char\\un01\\un01_00.rsd\0\0\0"
    "\\ff7\\condor\\char\\un01\\un01_11.rsd";
// GLOBAL: FF7 0x00902b18
unsigned char g_CondorRsdPath_un01_12[72] =
    "\\ff7\\condor\\char\\un01\\un01_12.rsd\0\0\0"
    "\\ff7\\condor\\char\\un01\\un01_13.rsd";
// GLOBAL: FF7 0x00902b60
unsigned char g_CondorRsdPath_un01_14[72] =
    "\\ff7\\condor\\char\\un01\\un01_14.rsd\0\0\0"
    "\\ff7\\condor\\char\\un01\\un01_15.rsd";
// GLOBAL: FF7 0x00902ba8
unsigned char g_CondorRsdPath_un01_16[72] =
    "\\ff7\\condor\\char\\un01\\un01_16.rsd\0\0\0"
    "\\ff7\\condor\\char\\un01\\un01_21.rsd";
// GLOBAL: FF7 0x00902bf0
unsigned char g_CondorRsdPath_un01_22[72] =
    "\\ff7\\condor\\char\\un01\\un01_22.rsd\0\0\0"
    "\\ff7\\condor\\char\\un01\\un01_23.rsd";
// GLOBAL: FF7 0x00902c38
unsigned char g_CondorRsdPath_un01_24[72] =
    "\\ff7\\condor\\char\\un01\\un01_24.rsd\0\0\0"
    "\\ff7\\condor\\char\\un01\\un01_25.rsd";
// GLOBAL: FF7 0x00902c80
unsigned char g_CondorRsdPath_un01_26[72] =
    "\\ff7\\condor\\char\\un01\\un01_26.rsd\0\0\0"
    "\\ff7\\condor\\char\\un01\\un01_27.rsd";
// GLOBAL: FF7 0x00902cc8
unsigned char g_CondorRsdPath_un01b_00[72] =
    "\\ff7\\condor\\char\\un01b\\un01b_00.rsd\0"
    "\\ff7\\condor\\char\\un01b\\un01b_21.rsd";
// GLOBAL: FF7 0x00902d10
unsigned char g_CondorRsdPath_un01b_22[72] =
    "\\ff7\\condor\\char\\un01b\\un01b_22.rsd\0"
    "\\ff7\\condor\\char\\un01b\\un01b_23.rsd";
// GLOBAL: FF7 0x00902d58
unsigned char g_CondorRsdPath_un01b_24[72] =
    "\\ff7\\condor\\char\\un01b\\un01b_24.rsd\0"
    "\\ff7\\condor\\char\\un01b\\un01b_25.rsd";
// GLOBAL: FF7 0x00902da0
unsigned char g_CondorRsdPath_un01b_26[72] =
    "\\ff7\\condor\\char\\un01b\\un01b_26.rsd\0"
    "\\ff7\\condor\\char\\un01b\\un01b_27.rsd";
// GLOBAL: FF7 0x00902de8
unsigned char g_CondorRsdPath_un01_31[72] =
    "\\ff7\\condor\\char\\un01\\un01_31.rsd\0\0\0"
    "\\ff7\\condor\\char\\un01\\un01_32.rsd";
// GLOBAL: FF7 0x00902e30
unsigned char g_CondorRsdPath_un01_33[72] =
    "\\ff7\\condor\\char\\un01\\un01_33.rsd\0\0\0"
    "\\ff7\\condor\\char\\un02\\un02_00.rsd";
// GLOBAL: FF7 0x00902e78
unsigned char g_CondorRsdPath_un02_11[72] =
    "\\ff7\\condor\\char\\un02\\un02_11.rsd\0\0\0"
    "\\ff7\\condor\\char\\un02\\un02_12.rsd";
// GLOBAL: FF7 0x00902ec0
unsigned char g_CondorRsdPath_un02_13[72] =
    "\\ff7\\condor\\char\\un02\\un02_13.rsd\0\0\0"
    "\\ff7\\condor\\char\\un02\\un02_14.rsd";
// GLOBAL: FF7 0x00902f08
unsigned char g_CondorRsdPath_un02_15[72] =
    "\\ff7\\condor\\char\\un02\\un02_15.rsd\0\0\0"
    "\\ff7\\condor\\char\\un02\\un02_16.rsd";
// GLOBAL: FF7 0x00902f50
unsigned char g_CondorRsdPath_un02_31[72] =
    "\\ff7\\condor\\char\\un02\\un02_31.rsd\0\0\0"
    "\\ff7\\condor\\char\\un02\\un02_32.rsd";
// GLOBAL: FF7 0x00902f98
unsigned char g_CondorRsdPath_un02_33[72] =
    "\\ff7\\condor\\char\\un02\\un02_33.rsd\0\0\0"
    "\\ff7\\condor\\char\\un02\\un02_21.rsd";
// GLOBAL: FF7 0x00902fe0
unsigned char g_CondorRsdPath_un02_22[72] =
    "\\ff7\\condor\\char\\un02\\un02_22.rsd\0\0\0"
    "\\ff7\\condor\\char\\un02\\un02_23.rsd";
// GLOBAL: FF7 0x00903028
unsigned char g_CondorRsdPath_un02_24[72] =
    "\\ff7\\condor\\char\\un02\\un02_24.rsd\0\0\0"
    "\\ff7\\condor\\char\\un02\\un02_25.rsd";
// GLOBAL: FF7 0x00903070
unsigned char g_CondorRsdPath_un02_26[72] =
    "\\ff7\\condor\\char\\un02\\un02_26.rsd\0\0\0"
    "\\ff7\\condor\\char\\un02b\\un02b_00.rsd";
// GLOBAL: FF7 0x009030b8
unsigned char g_CondorRsdPath_un02b_21[72] =
    "\\ff7\\condor\\char\\un02b\\un02b_21.rsd\0"
    "\\ff7\\condor\\char\\un02b\\un02b_22.rsd";
// GLOBAL: FF7 0x00903100
unsigned char g_CondorRsdPath_un02b_23[72] =
    "\\ff7\\condor\\char\\un02b\\un02b_23.rsd\0"
    "\\ff7\\condor\\char\\un02b\\un02b_24.rsd";
// GLOBAL: FF7 0x00903148
unsigned char g_CondorRsdPath_un02b_25[72] =
    "\\ff7\\condor\\char\\un02b\\un02b_25.rsd\0"
    "\\ff7\\condor\\char\\un02b\\un02b_26.rsd";
// GLOBAL: FF7 0x00903190
unsigned char g_CondorRsdPath_un03_00[72] =
    "\\ff7\\condor\\char\\un03\\un03_00.rsd\0\0\0"
    "\\ff7\\condor\\char\\un03\\un03_11.rsd";
// GLOBAL: FF7 0x009031d8
unsigned char g_CondorRsdPath_un03_12[72] =
    "\\ff7\\condor\\char\\un03\\un03_12.rsd\0\0\0"
    "\\ff7\\condor\\char\\un03\\un03_13.rsd";
// GLOBAL: FF7 0x00903220
unsigned char g_CondorRsdPath_un03_14[72] =
    "\\ff7\\condor\\char\\un03\\un03_14.rsd\0\0\0"
    "\\ff7\\condor\\char\\un03\\un03_15.rsd";
// GLOBAL: FF7 0x00903268
unsigned char g_CondorRsdPath_un03_16[72] =
    "\\ff7\\condor\\char\\un03\\un03_16.rsd\0\0\0"
    "\\ff7\\condor\\char\\un03\\un03_31.rsd";
// GLOBAL: FF7 0x009032b0
unsigned char g_CondorRsdPath_un03_32[72] =
    "\\ff7\\condor\\char\\un03\\un03_32.rsd\0\0\0"
    "\\ff7\\condor\\char\\un03\\un03_33.rsd";
// GLOBAL: FF7 0x009032f8
unsigned char g_CondorRsdPath_un03_21[72] =
    "\\ff7\\condor\\char\\un03\\un03_21.rsd\0\0\0"
    "\\ff7\\condor\\char\\un03\\un03_22.rsd";
// GLOBAL: FF7 0x00903340
unsigned char g_CondorRsdPath_un03_23[72] =
    "\\ff7\\condor\\char\\un03\\un03_23.rsd\0\0\0"
    "\\ff7\\condor\\char\\un03\\un03_24.rsd";
// GLOBAL: FF7 0x00903388
unsigned char g_CondorRsdPath_un03_25[72] =
    "\\ff7\\condor\\char\\un03\\un03_25.rsd\0\0\0"
    "\\ff7\\condor\\char\\un03\\un03_26.rsd";
// GLOBAL: FF7 0x009033d0
unsigned char g_CondorRsdPath_un03_27[72] =
    "\\ff7\\condor\\char\\un03\\un03_27.rsd\0\0\0"
    "\\ff7\\condor\\char\\un03\\un03_28.rsd";
// GLOBAL: FF7 0x00903418
unsigned char g_CondorRsdPath_un03b_21[72] =
    "\\ff7\\condor\\char\\un03b\\un03b_21.rsd\0"
    "\\ff7\\condor\\char\\un03b\\un03b_22.rsd";
// GLOBAL: FF7 0x00903460
unsigned char g_CondorRsdPath_un03b_23[72] =
    "\\ff7\\condor\\char\\un03b\\un03b_23.rsd\0"
    "\\ff7\\condor\\char\\un03b\\un03b_24.rsd";
// GLOBAL: FF7 0x009034a8
unsigned char g_CondorRsdPath_un03b_25[72] =
    "\\ff7\\condor\\char\\un03b\\un03b_25.rsd\0"
    "\\ff7\\condor\\char\\un03b\\un03b_26.rsd";
// GLOBAL: FF7 0x009034f0
unsigned char g_CondorRsdPath_un03b_27[72] =
    "\\ff7\\condor\\char\\un03b\\un03b_27.rsd\0"
    "\\ff7\\condor\\char\\un03b\\un03b_28.rsd";
// GLOBAL: FF7 0x00903538
unsigned char g_CondorRsdPath_un04_00[72] =
    "\\ff7\\condor\\char\\un04\\un04_00.rsd\0\0\0"
    "\\ff7\\condor\\char\\un04\\un04_11.rsd";
// GLOBAL: FF7 0x00903580
unsigned char g_CondorRsdPath_un04_12[72] =
    "\\ff7\\condor\\char\\un04\\un04_12.rsd\0\0\0"
    "\\ff7\\condor\\char\\un04\\un04_13.rsd";
// GLOBAL: FF7 0x009035c8
unsigned char g_CondorRsdPath_un04_14[72] =
    "\\ff7\\condor\\char\\un04\\un04_14.rsd\0\0\0"
    "\\ff7\\condor\\char\\un04\\un04_21.rsd";
// GLOBAL: FF7 0x00903610
unsigned char g_CondorRsdPath_un04_22[72] =
    "\\ff7\\condor\\char\\un04\\un04_22.rsd\0\0\0"
    "\\ff7\\condor\\char\\un04\\un04_23.rsd";
// GLOBAL: FF7 0x00903658
unsigned char g_CondorRsdPath_un04_24[72] =
    "\\ff7\\condor\\char\\un04\\un04_24.rsd\0\0\0"
    "\\ff7\\condor\\char\\un04\\un04_25.rsd";
// GLOBAL: FF7 0x009036a0
unsigned char g_CondorRsdPath_un04_31[72] =
    "\\ff7\\condor\\char\\un04\\un04_31.rsd\0\0\0"
    "\\ff7\\condor\\char\\un04\\un04_32.rsd";
// GLOBAL: FF7 0x009036e8
unsigned char g_CondorRsdPath_un04_33[72] =
    "\\ff7\\condor\\char\\un04\\un04_33.rsd\0\0\0"
    "\\ff7\\condor\\char\\un05\\un05_00.rsd";
// GLOBAL: FF7 0x00903730
unsigned char g_CondorRsdPath_un05_21[72] =
    "\\ff7\\condor\\char\\un05\\un05_21.rsd\0\0\0"
    "\\ff7\\condor\\char\\un05\\un05_22.rsd";
// GLOBAL: FF7 0x00903778
unsigned char g_CondorRsdPath_un05_23[72] =
    "\\ff7\\condor\\char\\un05\\un05_23.rsd\0\0\0"
    "\\ff7\\condor\\char\\un05\\un05_24.rsd";
// GLOBAL: FF7 0x009037c0
unsigned char g_CondorRsdPath_un05_25[72] =
    "\\ff7\\condor\\char\\un05\\un05_25.rsd\0\0\0"
    "\\ff7\\condor\\char\\un05\\un05_31.rsd";
// GLOBAL: FF7 0x00903808
unsigned char g_CondorRsdPath_un05_32[72] =
    "\\ff7\\condor\\char\\un05\\un05_32.rsd\0\0\0"
    "\\ff7\\condor\\char\\un05\\un05_41.rsd";
// GLOBAL: FF7 0x00903850
unsigned char g_CondorRsdPath_un05_42[72] =
    "\\ff7\\condor\\char\\un05\\un05_42.rsd\0\0\0"
    "\\ff7\\condor\\char\\un05\\un05_43.rsd";
// GLOBAL: FF7 0x00903898
unsigned char g_CondorRsdPath_un05_44[72] =
    "\\ff7\\condor\\char\\un05\\un05_44.rsd\0\0\0"
    "\\ff7\\condor\\char\\un05\\un05_51.rsd";
// GLOBAL: FF7 0x009038e0
unsigned char g_CondorRsdPath_un05_52[72] =
    "\\ff7\\condor\\char\\un05\\un05_52.rsd\0\0\0"
    "\\ff7\\condor\\char\\un05\\un05_53.rsd";
// GLOBAL: FF7 0x00903928
unsigned char g_CondorRsdPath_un05_54[72] =
    "\\ff7\\condor\\char\\un05\\un05_54.rsd\0\0\0"
    "\\ff7\\condor\\char\\un06\\un06_00.rsd";
// GLOBAL: FF7 0x00903970
unsigned char g_CondorRsdPath_un06_21[72] =
    "\\ff7\\condor\\char\\un06\\un06_21.rsd\0\0\0"
    "\\ff7\\condor\\char\\un06\\un06_22.rsd";
// GLOBAL: FF7 0x009039b8
unsigned char g_CondorRsdPath_un06_23[72] =
    "\\ff7\\condor\\char\\un06\\un06_23.rsd\0\0\0"
    "\\ff7\\condor\\char\\un06\\un06_24.rsd";
// GLOBAL: FF7 0x00903a00
unsigned char g_CondorRsdPath_un06_31[72] =
    "\\ff7\\condor\\char\\un06\\un06_31.rsd\0\0\0"
    "\\ff7\\condor\\char\\un06\\un06_32.rsd";
// GLOBAL: FF7 0x00903a48
unsigned char g_CondorRsdPath_un06_41[72] =
    "\\ff7\\condor\\char\\un06\\un06_41.rsd\0\0\0"
    "\\ff7\\condor\\char\\un06\\un06_42.rsd";
// GLOBAL: FF7 0x00903a90
unsigned char g_CondorRsdPath_un06_43[72] =
    "\\ff7\\condor\\char\\un06\\un06_43.rsd\0\0\0"
    "\\ff7\\condor\\char\\un06\\un06_44.rsd";
// GLOBAL: FF7 0x00903ad8
unsigned char g_CondorRsdPath_un06_45[72] =
    "\\ff7\\condor\\char\\un06\\un06_45.rsd\0\0\0"
    "\\ff7\\condor\\char\\un06\\un06_51.rsd";
// GLOBAL: FF7 0x00903b20
unsigned char g_CondorRsdPath_un06_52[72] =
    "\\ff7\\condor\\char\\un06\\un06_52.rsd\0\0\0"
    "\\ff7\\condor\\char\\un06\\un06_53.rsd";
// GLOBAL: FF7 0x00903b68
unsigned char g_CondorRsdPath_un06_54[72] =
    "\\ff7\\condor\\char\\un06\\un06_54.rsd\0\0\0"
    "\\ff7\\condor\\char\\un07\\un07_00.rsd";
// GLOBAL: FF7 0x00903bb0
unsigned char g_CondorRsdPath_un07_21[72] =
    "\\ff7\\condor\\char\\un07\\un07_21.rsd\0\0\0"
    "\\ff7\\condor\\char\\un07\\un07_22.rsd";
// GLOBAL: FF7 0x00903bf8
unsigned char g_CondorRsdPath_un07_23[72] =
    "\\ff7\\condor\\char\\un07\\un07_23.rsd\0\0\0"
    "\\ff7\\condor\\char\\un07\\un07_24.rsd";
// GLOBAL: FF7 0x00903c40
unsigned char g_CondorRsdPath_un07_31[72] =
    "\\ff7\\condor\\char\\un07\\un07_31.rsd\0\0\0"
    "\\ff7\\condor\\char\\un07\\un07_32.rsd";
// GLOBAL: FF7 0x00903c88
unsigned char g_CondorRsdPath_un07_41[72] =
    "\\ff7\\condor\\char\\un07\\un07_41.rsd\0\0\0"
    "\\ff7\\condor\\char\\un07\\un07_42.rsd";
// GLOBAL: FF7 0x00903cd0
unsigned char g_CondorRsdPath_un07_43[72] =
    "\\ff7\\condor\\char\\un07\\un07_43.rsd\0\0\0"
    "\\ff7\\condor\\char\\un07\\un07_44.rsd";
// GLOBAL: FF7 0x00903d18
unsigned char g_CondorRsdPath_un07_51[72] =
    "\\ff7\\condor\\char\\un07\\un07_51.rsd\0\0\0"
    "\\ff7\\condor\\char\\un07\\un07_52.rsd";
// GLOBAL: FF7 0x00903d60
unsigned char g_CondorRsdPath_un07_53[72] =
    "\\ff7\\condor\\char\\un07\\un07_53.rsd\0\0\0"
    "\\ff7\\condor\\char\\un07\\un07_54.rsd";
// GLOBAL: FF7 0x00903da8
unsigned char g_CondorRsdPath_un08_00[72] =
    "\\ff7\\condor\\char\\un08\\un08_00.rsd\0\0\0"
    "\\ff7\\condor\\char\\un08\\un08_01.rsd";
// GLOBAL: FF7 0x00903df0
unsigned char g_CondorRsdPath_un08_02[72] =
    "\\ff7\\condor\\char\\un08\\un08_02.rsd\0\0\0"
    "\\ff7\\condor\\char\\un08\\un08_03.rsd";
// GLOBAL: FF7 0x00903e38
unsigned char g_CondorRsdPath_un08_04[72] =
    "\\ff7\\condor\\char\\un08\\un08_04.rsd\0\0\0"
    "\\ff7\\condor\\char\\un08\\un08_21.rsd";
// GLOBAL: FF7 0x00903e80
unsigned char g_CondorRsdPath_un08_22[72] =
    "\\ff7\\condor\\char\\un08\\un08_22.rsd\0\0\0"
    "\\ff7\\condor\\char\\un08\\un08_23.rsd";
// GLOBAL: FF7 0x00903ec8
unsigned char g_CondorRsdPath_un08_24[72] =
    "\\ff7\\condor\\char\\un08\\un08_24.rsd\0\0\0"
    "\\ff7\\condor\\char\\un08\\un08_31.rsd";
// GLOBAL: FF7 0x00903f10
unsigned char g_CondorRsdPath_un08_32[72] =
    "\\ff7\\condor\\char\\un08\\un08_32.rsd\0\0\0"
    "\\ff7\\condor\\char\\un08\\un08_41.rsd";
// GLOBAL: FF7 0x00903f58
unsigned char g_CondorRsdPath_un08_42[72] =
    "\\ff7\\condor\\char\\un08\\un08_42.rsd\0\0\0"
    "\\ff7\\condor\\char\\un08\\un08_43.rsd";
// GLOBAL: FF7 0x00903fa0
unsigned char g_CondorRsdPath_un08_44[72] =
    "\\ff7\\condor\\char\\un08\\un08_44.rsd\0\0\0"
    "\\ff7\\condor\\char\\un08\\un08_51.rsd";
// GLOBAL: FF7 0x00903fe8
unsigned char g_CondorRsdPath_un08_52[72] =
    "\\ff7\\condor\\char\\un08\\un08_52.rsd\0\0\0"
    "\\ff7\\condor\\char\\un08\\un08_53.rsd";
// GLOBAL: FF7 0x00904030
unsigned char g_CondorRsdPath_un08_54[72] =
    "\\ff7\\condor\\char\\un08\\un08_54.rsd\0\0\0"
    "\\ff7\\condor\\char\\un09\\un09_00.rsd";
// GLOBAL: FF7 0x00904078
unsigned char g_CondorRsdPath_un10_00[72] =
    "\\ff7\\condor\\char\\un10\\un10_00.rsd\0\0\0"
    "\\ff7\\condor\\char\\un11\\un11_00.rsd";
// GLOBAL: FF7 0x009040c0
unsigned char g_CondorRsdPath_un11_01[72] =
    "\\ff7\\condor\\char\\un11\\un11_01.rsd\0\0\0"
    "\\ff7\\condor\\char\\un11\\un11_02.rsd";
// GLOBAL: FF7 0x00904108
unsigned char g_CondorRsdPath_un11_03[72] =
    "\\ff7\\condor\\char\\un11\\un11_03.rsd\0\0\0"
    "\\ff7\\condor\\char\\un11\\un11_04.rsd";
// GLOBAL: FF7 0x00904150
unsigned char g_CondorRsdPath_un12_00[72] =
    "\\ff7\\condor\\char\\un12\\un12_00.rsd\0\0\0"
    "\\ff7\\condor\\char\\un12\\un12_11.rsd";
// GLOBAL: FF7 0x00904198
unsigned char g_CondorRsdPath_un12_12[72] =
    "\\ff7\\condor\\char\\un12\\un12_12.rsd\0\0\0"
    "\\ff7\\condor\\char\\un12\\un12_13.rsd";
// GLOBAL: FF7 0x009041e0
unsigned char g_CondorRsdPath_un12_14[72] =
    "\\ff7\\condor\\char\\un12\\un12_14.rsd\0\0\0"
    "\\ff7\\condor\\char\\un12\\un12_31.rsd";
// GLOBAL: FF7 0x00904228
unsigned char g_CondorRsdPath_un12_32[72] =
    "\\ff7\\condor\\char\\un12\\un12_32.rsd\0\0\0"
    "\\ff7\\condor\\char\\un12\\un12_33.rsd";
// GLOBAL: FF7 0x00904270
unsigned char g_CondorRsdPath_un12_21[72] =
    "\\ff7\\condor\\char\\un12\\un12_21.rsd\0\0\0"
    "\\ff7\\condor\\char\\un12\\un12_22.rsd";
// GLOBAL: FF7 0x009042b8
unsigned char g_CondorRsdPath_un12_23[72] =
    "\\ff7\\condor\\char\\un12\\un12_23.rsd\0\0\0"
    "\\ff7\\condor\\char\\un12\\un12_24.rsd";
// GLOBAL: FF7 0x00904300
unsigned char g_CondorRsdPath_un12_25[72] =
    "\\ff7\\condor\\char\\un12\\un12_25.rsd\0\0\0"
    "\\ff7\\condor\\char\\un12\\un12_26.rsd";
// GLOBAL: FF7 0x00904348
unsigned char g_CondorRsdPath_un12b_00[72] =
    "\\ff7\\condor\\char\\un12b\\un12b_00.rsd\0"
    "\\ff7\\condor\\char\\un12b\\un12b_21.rsd";
// GLOBAL: FF7 0x00904390
unsigned char g_CondorRsdPath_un12b_22[72] =
    "\\ff7\\condor\\char\\un12b\\un12b_22.rsd\0"
    "\\ff7\\condor\\char\\un12b\\un12b_23.rsd";
// GLOBAL: FF7 0x009043d8
unsigned char g_CondorRsdPath_un12b_24[72] =
    "\\ff7\\condor\\char\\un12b\\un12b_24.rsd\0"
    "\\ff7\\condor\\char\\un12b\\un12b_25.rsd";
// GLOBAL: FF7 0x00904420
unsigned char g_CondorRsdPath_un12b_26[72] =
    "\\ff7\\condor\\char\\un12b\\un12b_26.rsd\0"
    "\\ff7\\condor\\char\\un12\\un12_61.rsd";
// GLOBAL: FF7 0x00904468
unsigned char g_CondorRsdPath_un12_62[72] =
    "\\ff7\\condor\\char\\un12\\un12_62.rsd\0\0\0"
    "\\ff7\\condor\\char\\un12\\un12_63.rsd";
// GLOBAL: FF7 0x009044b0
unsigned char g_CondorRsdPath_un12_64[72] =
    "\\ff7\\condor\\char\\un12\\un12_64.rsd\0\0\0"
    "\\ff7\\condor\\char\\un12\\un12_65.rsd";
// GLOBAL: FF7 0x009044f8
unsigned char g_CondorRsdPath_un12_66[72] =
    "\\ff7\\condor\\char\\un12\\un12_66.rsd\0\0\0"
    "\\ff7\\condor\\char\\un12\\un12_67.rsd";
// GLOBAL: FF7 0x00904540
unsigned char g_CondorRsdPath_un13_00[72] =
    "\\ff7\\condor\\char\\un13\\un13_00.rsd\0\0\0"
    "\\ff7\\condor\\char\\un13\\un13_11.rsd";
// GLOBAL: FF7 0x00904588
unsigned char g_CondorRsdPath_un13_12[72] =
    "\\ff7\\condor\\char\\un13\\un13_12.rsd\0\0\0"
    "\\ff7\\condor\\char\\un13\\un13_13.rsd";
// GLOBAL: FF7 0x009045d0
unsigned char g_CondorRsdPath_un13_14[72] =
    "\\ff7\\condor\\char\\un13\\un13_14.rsd\0\0\0"
    "\\ff7\\condor\\char\\un13\\un13_31.rsd";
// GLOBAL: FF7 0x00904618
unsigned char g_CondorRsdPath_un13_32[72] =
    "\\ff7\\condor\\char\\un13\\un13_32.rsd\0\0\0"
    "\\ff7\\condor\\char\\un13\\un13_33.rsd";
// GLOBAL: FF7 0x00904660
unsigned char g_CondorRsdPath_un13_21[72] =
    "\\ff7\\condor\\char\\un13\\un13_21.rsd\0\0\0"
    "\\ff7\\condor\\char\\un13\\un13_22.rsd";
// GLOBAL: FF7 0x009046a8
unsigned char g_CondorRsdPath_un13_23[72] =
    "\\ff7\\condor\\char\\un13\\un13_23.rsd\0\0\0"
    "\\ff7\\condor\\char\\un13\\un13_24.rsd";
// GLOBAL: FF7 0x009046f0
unsigned char g_CondorRsdPath_un13_25[72] =
    "\\ff7\\condor\\char\\un13\\un13_25.rsd\0\0\0"
    "\\ff7\\condor\\char\\un13\\un13_26.rsd";
// GLOBAL: FF7 0x00904738
unsigned char g_CondorRsdPath_un13_61[72] =
    "\\ff7\\condor\\char\\un13\\un13_61.rsd\0\0\0"
    "\\ff7\\condor\\char\\un13\\un13_62.rsd";
// GLOBAL: FF7 0x00904780
unsigned char g_CondorRsdPath_un13_63[72] =
    "\\ff7\\condor\\char\\un13\\un13_63.rsd\0\0\0"
    "\\ff7\\condor\\char\\un13\\un13_64.rsd";
// GLOBAL: FF7 0x009047c8
unsigned char g_CondorRsdPath_un13_65[72] =
    "\\ff7\\condor\\char\\un13\\un13_65.rsd\0\0\0"
    "\\ff7\\condor\\char\\un13\\un13_66.rsd";
// GLOBAL: FF7 0x00904810
unsigned char g_CondorRsdPath_un10_00_copy2[72] =
    "\\ff7\\condor\\char\\un10\\un10_00.rsd\0\0\0"
    "\\ff7\\condor\\char\\un10\\un10_00.rsd";
// GLOBAL: FF7 0x00904858
unsigned char g_CondorRsdPath_un16_00[72] =
    "\\ff7\\condor\\char\\un16\\un16_00.rsd\0\0\0"
    "\\ff7\\condor\\char\\un16\\un16_11.rsd";
// GLOBAL: FF7 0x009048a0
unsigned char g_CondorRsdPath_un16_12[72] =
    "\\ff7\\condor\\char\\un16\\un16_12.rsd\0\0\0"
    "\\ff7\\condor\\char\\un16\\un16_13.rsd";
// GLOBAL: FF7 0x009048e8
unsigned char g_CondorRsdPath_un16_14[72] =
    "\\ff7\\condor\\char\\un16\\un16_14.rsd\0\0\0"
    "\\ff7\\condor\\char\\un16\\un16_15.rsd";
// GLOBAL: FF7 0x00904930
unsigned char g_CondorRsdPath_un16_16[72] =
    "\\ff7\\condor\\char\\un16\\un16_16.rsd\0\0\0"
    "\\ff7\\condor\\char\\un16\\un16_31.rsd";
// GLOBAL: FF7 0x00904978
unsigned char g_CondorRsdPath_un16_32[72] =
    "\\ff7\\condor\\char\\un16\\un16_32.rsd\0\0\0"
    "\\ff7\\condor\\char\\un16\\un16_33.rsd";
// GLOBAL: FF7 0x009049c0
unsigned char g_CondorRsdPath_un16_21[72] =
    "\\ff7\\condor\\char\\un16\\un16_21.rsd\0\0\0"
    "\\ff7\\condor\\char\\un16\\un16_22.rsd";
// GLOBAL: FF7 0x00904a08
unsigned char g_CondorRsdPath_un16_23[72] =
    "\\ff7\\condor\\char\\un16\\un16_23.rsd\0\0\0"
    "\\ff7\\condor\\char\\un16\\un16_24.rsd";
// GLOBAL: FF7 0x00904a50
unsigned char g_CondorRsdPath_un16_25[72] =
    "\\ff7\\condor\\char\\un16\\un16_25.rsd\0\0\0"
    "\\ff7\\condor\\char\\un16\\un16_26.rsd";
// GLOBAL: FF7 0x00904a98
unsigned char g_CondorRsdPath_un17_00[72] =
    "\\ff7\\condor\\char\\un17\\un17_00.rsd\0\0\0"
    "\\ff7\\condor\\char\\un17\\un17_11.rsd";
// GLOBAL: FF7 0x00904ae0
unsigned char g_CondorRsdPath_un17_12[72] =
    "\\ff7\\condor\\char\\un17\\un17_12.rsd\0\0\0"
    "\\ff7\\condor\\char\\un17\\un17_13.rsd";
// GLOBAL: FF7 0x00904b28
unsigned char g_CondorRsdPath_un17_14[72] =
    "\\ff7\\condor\\char\\un17\\un17_14.rsd\0\0\0"
    "\\ff7\\condor\\char\\un17\\un17_15.rsd";
// GLOBAL: FF7 0x00904b70
unsigned char g_CondorRsdPath_un17_16[72] =
    "\\ff7\\condor\\char\\un17\\un17_16.rsd\0\0\0"
    "\\ff7\\condor\\char\\un17\\un17_21.rsd";
// GLOBAL: FF7 0x00904bb8
unsigned char g_CondorRsdPath_un17_22[72] =
    "\\ff7\\condor\\char\\un17\\un17_22.rsd\0\0\0"
    "\\ff7\\condor\\char\\un17\\un17_23.rsd";
// GLOBAL: FF7 0x00904c00
unsigned char g_CondorRsdPath_un17_24[72] =
    "\\ff7\\condor\\char\\un17\\un17_24.rsd\0\0\0"
    "\\ff7\\condor\\char\\un17\\un17_25.rsd";
// GLOBAL: FF7 0x00904c48
unsigned char g_CondorRsdPath_un17_26[72] =
    "\\ff7\\condor\\char\\un17\\un17_26.rsd\0\0\0"
    "\\ff7\\condor\\char\\un17\\un17_27.rsd";
// GLOBAL: FF7 0x00904c90
unsigned char g_CondorRsdPath_un17_28[72] =
    "\\ff7\\condor\\char\\un17\\un17_28.rsd\0\0\0"
    "\\ff7\\condor\\char\\un17\\un17_29.rsd";
// GLOBAL: FF7 0x00904cd8
unsigned char g_CondorRsdPath_un17_31[72] =
    "\\ff7\\condor\\char\\un17\\un17_31.rsd\0\0\0"
    "\\ff7\\condor\\char\\un17\\un17_32.rsd";
// GLOBAL: FF7 0x00904d20
unsigned char g_CondorRsdPath_un17_33[72] =
    "\\ff7\\condor\\char\\un17\\un17_33.rsd\0\0\0"
    "\\ff7\\condor\\char\\un18\\un18_00.rsd";
// GLOBAL: FF7 0x00904d68
unsigned char g_CondorRsdPath_un18_11[72] =
    "\\ff7\\condor\\char\\un18\\un18_11.rsd\0\0\0"
    "\\ff7\\condor\\char\\un18\\un18_12.rsd";
// GLOBAL: FF7 0x00904db0
unsigned char g_CondorRsdPath_un18_13[72] =
    "\\ff7\\condor\\char\\un18\\un18_13.rsd\0\0\0"
    "\\ff7\\condor\\char\\un18\\un18_14.rsd";
// GLOBAL: FF7 0x00904df8
unsigned char g_CondorRsdPath_un18_15[72] =
    "\\ff7\\condor\\char\\un18\\un18_15.rsd\0\0\0"
    "\\ff7\\condor\\char\\un18\\un18_16.rsd";
// GLOBAL: FF7 0x00904e40
unsigned char g_CondorRsdPath_un18_31[72] =
    "\\ff7\\condor\\char\\un18\\un18_31.rsd\0\0\0"
    "\\ff7\\condor\\char\\un18\\un18_32.rsd";
// GLOBAL: FF7 0x00904e88
unsigned char g_CondorRsdPath_un18_33[72] =
    "\\ff7\\condor\\char\\un18\\un18_33.rsd\0\0\0"
    "\\ff7\\condor\\char\\un18\\un18_21.rsd";
// GLOBAL: FF7 0x00904ed0
unsigned char g_CondorRsdPath_un18_22[72] =
    "\\ff7\\condor\\char\\un18\\un18_22.rsd\0\0\0"
    "\\ff7\\condor\\char\\un18\\un18_23.rsd";
// GLOBAL: FF7 0x00904f18
unsigned char g_CondorRsdPath_un18_24[72] =
    "\\ff7\\condor\\char\\un18\\un18_24.rsd\0\0\0"
    "\\ff7\\condor\\char\\un18\\un18_25.rsd";
// GLOBAL: FF7 0x00904f60
unsigned char g_CondorRsdPath_un19_00[72] =
    "\\ff7\\condor\\char\\un19\\un19_00.rsd\0\0\0"
    "\\ff7\\condor\\char\\un19\\un19_11.rsd";
// GLOBAL: FF7 0x00904fa8
unsigned char g_CondorRsdPath_un19_12[72] =
    "\\ff7\\condor\\char\\un19\\un19_12.rsd\0\0\0"
    "\\ff7\\condor\\char\\un19\\un19_13.rsd";
// GLOBAL: FF7 0x00904ff0
unsigned char g_CondorRsdPath_un19_14[72] =
    "\\ff7\\condor\\char\\un19\\un19_14.rsd\0\0\0"
    "\\ff7\\condor\\char\\un19\\un19_31.rsd";
// GLOBAL: FF7 0x00905038
unsigned char g_CondorRsdPath_un19_32[72] =
    "\\ff7\\condor\\char\\un19\\un19_32.rsd\0\0\0"
    "\\ff7\\condor\\char\\un19\\un19_33.rsd";
// GLOBAL: FF7 0x00905080
unsigned char g_CondorRsdPath_un19_21[72] =
    "\\ff7\\condor\\char\\un19\\un19_21.rsd\0\0\0"
    "\\ff7\\condor\\char\\un19\\un19_22.rsd";
// GLOBAL: FF7 0x009050c8
unsigned char g_CondorRsdPath_un19_23[72] =
    "\\ff7\\condor\\char\\un19\\un19_23.rsd\0\0\0"
    "\\ff7\\condor\\char\\un19\\un19_24.rsd";
// GLOBAL: FF7 0x00905110
unsigned char g_CondorRsdPath_un19_25[72] =
    "\\ff7\\condor\\char\\un19\\un19_25.rsd\0\0\0"
    "\\ff7\\condor\\char\\un10\\un10_00.rsd";
// GLOBAL: FF7 0x00905158
unsigned char g_CondorRsdPath_un10_00_copy3[72] =
    "\\ff7\\condor\\char\\un10\\un10_00.rsd\0\0\0"
    "\\ff7\\condor\\char\\un10\\un10_00.rsd";
// GLOBAL: FF7 0x009051a0
unsigned char g_CondorRsdPath_un10_00_copy4[72] =
    "\\ff7\\condor\\char\\un10\\un10_00.rsd\0\0\0"
    "\\ff7\\condor\\char\\un05\\un05_obj.rsd";
// GLOBAL: FF7 0x009051e8
unsigned char g_CondorRsdPath_un06_obj[72] =
    "\\ff7\\condor\\char\\un06\\un06_obj.rsd\0\0"
    "\\ff7\\condor\\char\\un07\\un07_obj.rsd";
// GLOBAL: FF7 0x00905230
unsigned char g_CondorRsdPath_un08_obj[72] =
    "\\ff7\\condor\\char\\un08\\un08_obj.rsd\0\0"
    "\\ff7\\condor\\char\\un04\\un04_obj.rsd";
// GLOBAL: FF7 0x00905278
unsigned char g_CondorRsdPath_un19_obj[72] =
    "\\ff7\\condor\\char\\un19\\un19_obj.rsd\0\0"
    "\\ff7\\condor\\char\\sonota\\mezi.rsd";
// GLOBAL: FF7 0x009052c0
unsigned char g_CondorRsdPath_yazi[72] =
    "\\ff7\\condor\\char\\sonota\\yazi.rsd\0\0\0\0"
    "\\ff7\\condor\\char\\sonota\\leng00.rsd";
// GLOBAL: FF7 0x00905308
unsigned char g_CondorRsdPath_leng01[72] =
    "\\ff7\\condor\\char\\sonota\\leng01.rsd\0\0"
    "\\ff7\\condor\\char\\sonota\\leng02.rsd";
// GLOBAL: FF7 0x00905350
unsigned char g_CondorRsdPath_un01b_00_copy2[72] =
    "\\ff7\\condor\\char\\un01b\\un01b_00.rsd\0"
    "\\ff7\\condor\\char\\un02b\\un02b_00.rsd";
// GLOBAL: FF7 0x00905398
unsigned char g_CondorRsdPath_un03b_00[72] =
    "\\ff7\\condor\\char\\un03b\\un03b_00.rsd\0"
    "\\ff7\\condor\\char\\un03b\\un03b_00.rsd";
// GLOBAL: FF7 0x009053e0
unsigned char g_CondorRsdPath_un12b_00_copy2[72] =
    "\\ff7\\condor\\char\\un12b\\un12b_00.rsd\0"
    "\\ff7\\condor\\char\\un12b\\un12b_00.rsd";
extern "C" unsigned char g_rawData_00905428[52] = {
    0x72, 0x00, 0x00, 0x00, 0x2e, 0x72, 0x73, 0x00, 0x77, 0x00, 0x00, 0x00, 0x2e, 0x50, 0x4c, 0x59, 0x0a, 0x00, 0x00, 0x00, 0x2e, 0x4d, 0x41, 0x54,
    0x0a, 0x00, 0x00, 0x00, 0x2e, 0x47, 0x52, 0x50, 0x0a, 0x00, 0x00, 0x00, 0x72, 0x73, 0x64, 0x5c, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x72, 0x73, 0x64,
    0x00, 0x00, 0x00, 0x00
};

#pragma intrinsic(strcpy, memset)
// Prototypes of callees the C sources used implicitly (definitions elsewhere), required in C++.

/*
 * Loads 29 Fort Condor unit model sets into the global model table.
 */
// FUNCTION: FF7 0x005fa280
void __cdecl Mini_CondorLoadUnitModels(PfileLoadOptions *loadContext, struct GfxEngineContext *context)
{
    int padSlot[32]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    PolygonModelFileList loadReq;
    unsigned short unitIdx;

    if (g_CondorUsesLgp != 0) {
        Mini_CondorFillRsdFileNames();
        loadContext->file.fromArchive = 1;
        loadContext->file.archiveId = 6;
        loadContext->file.callback = 0;
    }
    for (unitIdx = 0; unitIdx < 29; unitIdx++) {
        ((CondorUnitModelEntry *)&g_CondorUnitModelSetKinds)[unitIdx].modelKind = (int)*(volatile char *)((char *)&g_CondorUnitModelDefs + unitIdx * 0x7C);
        Pfile_InitModelSetRequest((int)&loadReq);
        loadReq.count = ((CondorUnitModelEntry *)&g_CondorUnitModelSetKinds)[unitIdx].modelKind;
        loadReq.hasFiles = 0;
        loadReq.entries = unitIdx * 0x7C + (int)&g_CondorUnitModelFileTable;
        loadReq.isEnumerated = 1;
        /* 16.0f */
        *(int *)&loadContext->scale = 0x41800000;
        ((CondorUnitModelSlot *)&g_CondorUnitModelSets)[unitIdx].modelSet = Pfile_LoadModelSet(&loadReq, loadContext, context);
    }
}

/*
 * Generates sequential .rsd model filenames for Fort Condor unit groups.
 */
// FUNCTION: FF7 0x005fa3a9
void __cdecl Mini_CondorFillRsdFileNames(void)
{
    char nameBuf[64];
    int entryCnt;
    int j;
    int groupIdx;
    int ctr;
    ctr = 0;
    for (groupIdx = 0; groupIdx < 29; groupIdx++) {
        entryCnt = *(char *)((char *)&g_CondorUnitModelDefs + groupIdx * 124);
        for (j = 0; j < entryCnt; j++) {
            Mini_CondorIndexToLetterPair(ctr, nameBuf);
            // STRING: FF7 0x0090545c
            strcat(nameBuf, ".rsd");
            strcpy(*(char **)((char *)&g_CondorUnitModelFileTable + groupIdx * 124 + j * 4), nameBuf);
            ctr++;
        }
    }
}
/*
 * Converts a numeric index into a two-letter string identifier (\'AA\', \'AB\', ...).
 */
// FUNCTION: FF7 0x005fa472
void __cdecl Mini_CondorIndexToLetterPair(int index, char *output)
{
    char name[3];
    name[0] = index / 26 + 'A';
    name[1] = index - index / 26 * 26 + 'A';
    name[2] = 0;
    strcpy(output, name);
}

// Walks the 29 unit model slots of g_CondorUnitModelSets and calls Pfile_FreeModelSet on each slot's modelSet, passing context through unchanged.
// FUNCTION: FF7 0x005fa4cf
void __cdecl Mini_CondorFreeUnitModels(int context)
{
    unsigned short i;

    for (i = 0; i < 29; i++) {
        Pfile_FreeModelSet((void *)(((CondorUnitModelSlot *)&g_CondorUnitModelSets)[i].modelSet), (void *)(context));
    }
}

/*
 * Renders a Fort Condor unit 3D model using the given transform matrix and color state.
 */
// FUNCTION: FF7 0x005fa519
void __cdecl Mini_CondorDrawUnitModel(GfxEngineContext *context, void *unit, int psxMatrix, int overlay, int colorMode)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        GfxMatrix4x4 unitMatrix;
        struct { unsigned char pad_to_40[0x40]; CondorUnitModelSet *modelSet; };
        struct { unsigned char pad_to_44[0x44]; int vertexColor; };
        struct { unsigned char pad_to_45[0x45]; char colorBias; };
        struct { unsigned char pad_to_46[0x46]; char colorTint; };
        struct { unsigned char pad_to_47[0x47]; char colorFull; };
    } frame;
#pragma pack(pop)

    if (colorMode == 1) {
        frame.colorTint = 0x80;
        frame.colorBias = 0;
        *(char *)&frame.vertexColor = 0;
        frame.colorFull = 0xFF;
    } else {
        frame.colorTint = ((CondorRecord *)unit)->retired << 1;
        frame.colorBias = 0;
        *(char *)&frame.vertexColor = 0;
        frame.colorFull = 0xFF;
    }
    GtePsxMatrixToFloat4x4((PsxMatrix *)psxMatrix, &frame.unitMatrix);
    frame.modelSet = (CondorUnitModelSet *)(((CondorUnitModelSlot *)&g_CondorUnitModelSets)[((CondorRecord *)unit)->unitIndex].modelSet);
    Pfile_SetModelSetMatrix((int)(&frame.unitMatrix), (struct PolygonModelSetMatrix *)frame.modelSet, context);
    if (overlay != 0) {
        Gfx_SetScenePassCall(1, (struct GfxEngineContext *)context);
        if (g_CondorUsesSoftwareDriver != 0) {
            Gfx_ApplyPresetStateCall(1, context);
            SelectContextSlotOverride(1, context);
        }
    } else {
        Gfx_SetScenePassCall(0, (struct GfxEngineContext *)context);
    }
    if (colorMode != 0) {
        Pfile_SetModelVertexColor(frame.vertexColor, ((int *)(frame.modelSet->polySetTable))[*(unsigned char *)&((CondorRecord *)unit)->operand]);
    }
    Gfx_DrawMeshPolygonSet(((int *)(frame.modelSet->polySetTable))[*(unsigned char *)&((CondorRecord *)unit)->operand], (int)context);
    if (colorMode > 1) {
        Pfile_RestoreModelVertexColors(((int *)(frame.modelSet->polySetTable))[*(unsigned char *)&((CondorRecord *)unit)->operand]);
    }
    if (overlay != 0 && g_CondorUsesSoftwareDriver != 0) {
        ClearContextSlotOverride(context);
    }
}
} /* extern "C" */
