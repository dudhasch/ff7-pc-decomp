// src/ff7/coaster/coaster_helpers.cpp
// Original: C:\FF7\coaster\coaster_helpers.cpp
// Contents: Pool allocators for the coaster minigame: resets the model, vertex and triangle
//   storage, and hands out model records and vertex and triangle blocks from them.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/coaster.h"
#include "lib/codegen.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/coaster_psxdata.h"
#include "ff7/coaster_renderer.h"
#include "ff7/api/coaster.h"
#include "lib/pfile.h"

#pragma intrinsic(abs)
//
// Six coaster helper functions called by Mini_CoasterLoadData and the track/projectile update loop.

extern "C" {

#pragma intrinsic(memset)

// Coaster model record pool; a view of the 100 records Mini_CoasterLoadData takes.

// clang-format off
// GLOBAL: FF7 0x00c5bf58
int g_CoasterTriangleStorageBase = 0;
unsigned char g_bssPad_00c5bf5c[4] = {0};
// GLOBAL: FF7 0x00c5bf60
unsigned char g_CoasterModelRecordPool[3200] = {0};
unsigned char g_bssPad_00c5cbe0[1280] = {0};
// GLOBAL: FF7 0x00c5d0e0
int g_CoasterVertexCount = 0;
// GLOBAL: FF7 0x00c5d0e4
struct CoasterModelTemplate *g_CoasterModelTemplates = 0;
// GLOBAL: FF7 0x00c5d0e8
int g_CoasterVertexStorageBase = 0;
// GLOBAL: FF7 0x00c5d0ec
int g_CoasterModelRecordCount = 0;
// The 100 coaster model handles Mini_CoasterLoadData stores (coaster.cpp indexes g_CoasterModelHandles up to 99).
// One int[100] span in the retail .bss, split here at the interior 4-mod-8 symbols other TUs address directly
// (0xC5D22C and 0xC5D25C) because VC5 8-byte-aligns arrays >= 8 bytes; the sub-spans between them place every
// element at its retail offset.
//
CoasterModelRecord *g_CoasterModelHandles[79] = {0};
CoasterModelRecord *g_CoasterCarModelHandle = 0;
unsigned char g_bssPad_00c5d230[44] = {0};
void *g_CoasterExplosionModel = 0;
unsigned char g_bssPad_00c5d260[32] = {0};
unsigned char g_bssPad_00c5d280[160] = {0};
// GLOBAL: FF7 0x00c5d320
int g_CoasterTriangleCount = 0;
unsigned char g_bssPad_00c5d324[4] = {0};
// clang-format on

/*
 * Resets allocation counters and initializes storage pointers for model records,
 * vertices, triangles, and model templates from the loaded PSX dataset.
 */
// FUNCTION: FF7 0x005ee7f0
void __cdecl Mini_CoasterInitModelPools(void)
{
    int vertexOffset;
    int vertexBlock;
    int triangleOffset;
    int triangleBlock;
    int curTemplatesVal;
    int templateOffset;

    g_CoasterVertexCount = 0;
    g_CoasterTriangleCount = 0;
    g_CoasterModelRecordCount = 0;
    vertexOffset = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(0xA) << 2));
    if (vertexOffset >= g_CoasterPsxDataSet.binSize) {
        vertexBlock = 0;
    } else {
        vertexBlock = g_CoasterPsxDataSet.binData + vertexOffset;
    }
    g_CoasterVertexStorageBase = vertexBlock;
    triangleOffset = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(0x10) << 2));
    if (triangleOffset >= g_CoasterPsxDataSet.binSize) {
        triangleBlock = 0;
    } else {
        triangleBlock = g_CoasterPsxDataSet.binData + triangleOffset;
    }
    g_CoasterTriangleStorageBase = triangleBlock;
    templateOffset = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(1) << 2));
    if (templateOffset >= g_CoasterPsxDataSet.binSize) {
        curTemplatesVal = 0;
    } else {
        curTemplatesVal = g_CoasterPsxDataSet.binData + templateOffset;
    }
    g_CoasterModelTemplates = (struct CoasterModelTemplate *)(curTemplatesVal);
}

/*
 * Allocates a 32-byte model record and initializes it from the indexed template,
 * assigning vertex and triangle buffer memory from the pool arenas.
 */
// FUNCTION: FF7 0x005ee8cf
CoasterModelRecord *__cdecl Mini_CoasterBuildModelFromTemplate(int templateIndex)
{
    int vertexCount;
    int triangleCnt;
    CoasterModelRecord *modelRecordPtr;

    modelRecordPtr = (CoasterModelRecord *)Mini_CoasterAllocModelRecord();
    vertexCount = g_CoasterModelTemplates[templateIndex].vertexCount;
    triangleCnt = g_CoasterModelTemplates[templateIndex].triangleCount;
    modelRecordPtr->minX = g_CoasterModelTemplates[templateIndex].minX;
    modelRecordPtr->maxX = g_CoasterModelTemplates[templateIndex].maxX;
    modelRecordPtr->minZ = g_CoasterModelTemplates[templateIndex].minZ;
    modelRecordPtr->maxZ = g_CoasterModelTemplates[templateIndex].maxZ;
    modelRecordPtr->clearedShort0 = 0;
    modelRecordPtr->vertexCount = vertexCount;
    modelRecordPtr->triangleCount = triangleCnt;
    modelRecordPtr->clearedShort1 = 0;
    modelRecordPtr->totalCount = vertexCount + triangleCnt;
    modelRecordPtr->vertices = Mini_CoasterAllocVertexStorage(vertexCount);
    modelRecordPtr->triangles = Mini_CoasterAllocTriangleStorage(triangleCnt);
    return modelRecordPtr;
}

/*
 * Allocates the next 32-byte model record from the model record pool.
 */
// FUNCTION: FF7 0x005ee9c2
int __cdecl Mini_CoasterAllocModelRecord(void)
{
    CoasterModelRecord *rec;

    rec = (CoasterModelRecord *)((g_CoasterModelRecordCount << 5) + (int)g_CoasterModelRecordPool);
    g_CoasterModelRecordCount++;
    return (int)rec;
}

/*
 * Allocates a contiguous block of vertex storage from the vertex arena.
 */
// FUNCTION: FF7 0x005ee9ec
int __cdecl Mini_CoasterAllocVertexStorage(int vertexCount)
{
    int block;

    block = g_CoasterVertexStorageBase + g_CoasterVertexCount * 36;
    g_CoasterVertexCount += vertexCount;
    return block;
}

/*
 * Allocates a contiguous block of polygon/triangle storage from the triangle arena.
 */
// FUNCTION: FF7 0x005eea19
int __cdecl Mini_CoasterAllocTriangleStorage(int count)
{
    int result;

    result = g_CoasterTriangleStorageBase + g_CoasterTriangleCount * 40;
    g_CoasterTriangleCount += count;
    return result;
}
} /* extern "C" */
