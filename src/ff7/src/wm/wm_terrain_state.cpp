// src/ff7/src/wm/wm_terrain_state.cpp
// Data-only object: .bss 0x00e36110..0x00e38938. No code.
// A data-only member of the wm library, which its neighbour wm_world_message_window.cpp never references: the
// world map's selected and player terrain triangles, terrain and field tables, message buffer, arctangent
// table and mesh vertices.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/wm.h"
#include <string.h>
#include "ff7/world.h"
#include "lib/codegen.h"
#include "ff7/api/field.h"
#include "ff7/api/wm.h"
#include "ff7/api/menu.h"
#include "lib/api/token.h"
#include "ff7/enums/field.h"
#include "ff7/field.h"
#include "ff7/types/field.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00e36110
WorldTerrainTriangle *g_pWorldSelectedTerrainTriangle = 0;
// GLOBAL: FF7 0x00e36114
int g_pWorldTerrainTable = 0;
// GLOBAL: FF7 0x00e36118
unsigned char g_WorldMsgHeader[8] = {0};
// GLOBAL: FF7 0x00e36120
short g_WorldMsgBuffer[0x800] = {0};
// GLOBAL: FF7 0x00e37120
short g_WorldArcTangentTable[512] = {0};
// GLOBAL: FF7 0x00e37520
int g_WorldTerrainHasWater = 0;
// GLOBAL: FF7 0x00e37524
int g_WorldTerrainFogDistance = 0;
unsigned char g_bssPad_00e37528[2048] = {0};
// GLOBAL: FF7 0x00e37d28
int g_WorldTerrainOcclusionDepth = 0;
unsigned char g_bssPad_00e37d2c[4] = {0};
// GLOBAL: FF7 0x00e37d30
int g_WorldFieldTable = 0;
unsigned char g_bssPad_00e37d34[4] = {0};
unsigned char g_bssPad_00e37d38[1528] = {0};
// GLOBAL: FF7 0x00e38330
int g_WorldPlayerTerrainTriangle = 0;
unsigned char g_bssPad_00e38334[4] = {0};
// GLOBAL: FF7 0x00e38338
WorldMeshVertex g_WorldMeshVertices[0x80] = {0};
// clang-format on

} /* extern "C" */
