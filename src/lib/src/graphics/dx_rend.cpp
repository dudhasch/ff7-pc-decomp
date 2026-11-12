// src/lib/src/graphics/dx_rend.cpp
// Original: C:\lib\src\graphics\dx_rend.cpp
// Contents: Builds Direct3D execute-buffer polygon sets: allocating and freeing them, compiling
//   each group into execute-buffer instructions (vertices, transform states, render and light
//   states), and rendering the deferred polygon-set lists.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/graphics.h"
#include "lib/pfile.h"
#include "lib/codegen.h"
#include "lib/api/mem.h"
#include "lib/types/graphics.h"
#include "lib/types/mem.h"
#include "lib/types/polygon.h"
#include "lib/types/list.h"
#include "lib/enums/graphics.h"
#include "lib/com_abi.h"
#include "lib/api/list.h"
#include <stdio.h>
#include <string.h>

struct GfxEngineContext;
struct GfxMatrixArray;
extern "C" {
/*
 * Replaces highest color byte with material byte when flag 0x400 is set.
 */
// FUNCTION: FF7 0x0069fca0
void __cdecl D3DExecApplyColorModulate(unsigned int *color, void *material)
{
    unsigned int result;

    if (material != 0 && (*(unsigned int *)((char *)material + 8) & 0x400) != 0) {
        result = *color;
        *((unsigned char *)&result + 3) = *((unsigned char *)material + 0x5C);
        *color = result;
    }
}


// clang-format off

// GLOBAL: FF7 0x00912288
float g_GfxIdentityMatrix[4][4] = {
    {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}};
// clang-format on
// dx_rend.cpp's literal stream in retail order: the literals of the last functions in .text (0x009122C8..0x0091248F)
// and then those of the functions before them (0x00912490..0x0091257B) precede the literals of the first ones. Each
// of them is returned by a static __inline function, a parse-time unit, in retail order; under /Ob1 the call expands to
// the plain operand.
static __inline char *DxRendLit_SourcePath_1(void)
{
    // STRING: FF7 0x009122c8
    return "C:\\lib\\src\\graphics\\dx_rend.cpp";
}
static __inline char *DxRendLit_SourcePath_2(void)
{
    // STRING: FF7 0x009122e8
    return "C:\\lib\\src\\graphics\\dx_rend.cpp";
}
static __inline char *DxRendLit_SourcePath_3(void)
{
    // STRING: FF7 0x00912308
    return "C:\\lib\\src\\graphics\\dx_rend.cpp";
}
static __inline char *DxRendLit_AfterD3dopStaterender(void)
{
    // STRING: FF7 0x00912328
    return "AFTER D3DOP_STATERENDER \n";
}
static __inline char *DxRendLit_Size_1(void)
{
    // STRING: FF7 0x00912344
    return "SIZE %u \n";
}
static __inline char *DxRendLit_Size_2(void)
{
    // STRING: FF7 0x00912350
    return "SIZE %u \n";
}
static __inline char *DxRendLit_AfterD3drenderstateZenable(void)
{
    // STRING: FF7 0x0091235c
    return "AFTER D3DRENDERSTATE_ZENABLE \n";
}
static __inline char *DxRendLit_Size_3(void)
{
    // STRING: FF7 0x0091237c
    return "SIZE %u \n";
}
static __inline char *DxRendLit_Size_4(void)
{
    // STRING: FF7 0x00912388
    return "SIZE %u \n";
}
static __inline char *DxRendLit_AfterD3drenderstateBlendenable(void)
{
    // STRING: FF7 0x00912394
    return "AFTER D3DRENDERSTATE_BLENDENABLE \n";
}
static __inline char *DxRendLit_AfterD3drenderstateSpecularenable(void)
{
    // STRING: FF7 0x009123b8
    return "AFTER D3DRENDERSTATE_SPECULARENABLE \n";
}
static __inline char *DxRendLit_AfterD3drenderstateShademode(void)
{
    // STRING: FF7 0x009123e0
    return "AFTER D3DRENDERSTATE_SHADEMODE \n";
}
static __inline char *DxRendLit_AfterD3dopStatelight(void)
{
    // STRING: FF7 0x00912404
    return "AFTER D3DOP_STATELIGHT \n";
}
static __inline char *DxRendLit_AfterD3dlightstateMaterialD3dlightstate(void)
{
    // STRING: FF7 0x00912420
    return "AFTER D3DLIGHTSTATE_MATERIAL, D3DLIGHTSTATE_AMBIENT\n";
}
static __inline char *DxRendLit_TotalLightStatesError(void)
{
    // STRING: FF7 0x00912458
    return "TOTAL LIGHT STATES ERROR \n";
}
static __inline char *DxRendLit_TotalRenderStatesError(void)
{
    // STRING: FF7 0x00912474
    return "TOTAL RENDER STATES ERROR \n";
}
static __inline char *DxRendLit_AfterVertexList(void)
{
    // STRING: FF7 0x00912490
    return "AFTER VERTEX LIST \n";
}
static __inline char *DxRendLit_AfterInstructionList(void)
{
    // STRING: FF7 0x009124a4
    return "AFTER INSTRUCTION LIST \n";
}
static __inline char *DxRendLit_AfterTransform(void)
{
    // STRING: FF7 0x009124c0
    return "AFTER TRANSFORM \n";
}
static __inline char *DxRendLit_StartingProcessVertices(void)
{
    // STRING: FF7 0x009124d4
    return "STARTING PROCESS VERTICES \n";
}
static __inline char *DxRendLit_AfterProcessVertices(void)
{
    // STRING: FF7 0x009124f0
    return "AFTER PROCESS VERTICES \n";
}
static __inline char *DxRendLit_AfterTriangleList(void)
{
    // STRING: FF7 0x0091250c
    return "AFTER TRIANGLE LIST \n";
}
static __inline char *DxRendLit_AfterD3dopExit(void)
{
    // STRING: FF7 0x00912524
    return "AFTER D3DOP_EXIT \n";
}
static __inline char *DxRendLit_ErrorExecutionBufferCompile(void)
{
    // STRING: FF7 0x00912538
    return "ERROR: EXECUTION BUFFER COMPILE SIZE %d != COMPILE DATA SIZE %d \n";
}


struct D3DExecVertex {
    int v[8];
};
#pragma intrinsic(memset)

/*
 * Releases group allocations and COM execute-buffer arrays for polygon set.
 */
// FUNCTION: FF7 0x0069fcd7
void __cdecl D3DFreePolygonSet(GfxPolygonSet *polygonSet)
{
    int index;
    if (polygonSet != 0) {
        // STRING: FF7 0x0091257c
        GameHeapFree((void *)polygonSet->elements, "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x4BF);
        Gfx_FreePolygonSetGroups(polygonSet->groupCount, (int)polygonSet->entries);
        // STRING: FF7 0x0091259c
        GameHeapFree((void *)polygonSet->groupStates, "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x4C3);
        if (polygonSet->batchList != 0) {
            for (index = 0; index < polygonSet->groupCount; index++) {
                D3D5FreeBatch((struct GfxD3d5BatchState *)polygonSet->batchList[index]);
            }
            // STRING: FF7 0x009125bc
            GameHeapFree(polygonSet->batchList, "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x4CC);
        }
        if (polygonSet->comObjectTable != 0) {
            for (index = 0; index < polygonSet->groupCount; index++) {
                if (polygonSet->comObjectTable[index] != 0) {
                    polygonSet->comObjectTable[index]->Release();
                    polygonSet->comObjectTable[index] = 0;
                }
            }
            // STRING: FF7 0x009125dc
            GameHeapFree(polygonSet->comObjectTable, "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x4DA);
        }
        if (polygonSet->execBuffers != 0) {
            for (index = 0; index < polygonSet->groupCount; index++) {
                if (polygonSet->execBuffers[index] != 0) {
                    polygonSet->execBuffers[index]->Release();
                    polygonSet->execBuffers[index] = 0;
                }
            }
            // STRING: FF7 0x009125fc
            GameHeapFree(polygonSet->execBuffers, "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x4E8);
        }
        // STRING: FF7 0x0091261c
        GameHeapFree((void *)polygonSet->execBufferAlloc, "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x4EB);
        // STRING: FF7 0x0091263c
        GameHeapFree(polygonSet, "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x4ED);
    }
}

/*
 * Allocates and initializes a D3D polygon set.
 */
// FUNCTION: FF7 0x0069feb5
int __cdecl D3DAllocPolygonSet(void)
{
    GfxPolygonSet *rec;

    // STRING: FF7 0x0091265c
    rec = (GfxPolygonSet *)((int)GameHeapAlloc(1, 0xB0, "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x4F5));
    if (rec != 0) {
        rec->drawEnabled = 1;
        rec->drawUnsorted = 1;
        rec->hasGroupState = 1;
    }
    return (int)rec;
}

/*
 * Allocates per-group arrays for polygon set.
 */
// FUNCTION: FF7 0x0069feff
int __cdecl D3DExecAllocGroupArrays(GfxExecGroup *group)
{
    int ok;

    ok = 0;
    if (group != 0) {
        // STRING: FF7 0x0091267c
        group->entryHandles = (int)GameHeapAlloc(group->entryCount, 4, "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x508);
        // STRING: FF7 0x0091269c
        group->entryStates = (int)GameHeapAlloc(group->entryCount, 4, "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x509);
        if (group->entryHandles != 0 && group->entryStates != 0) {
            ok = 1;
        }
    }
    return ok;
}

/*
 * Compiles an execute buffer group for polygon set.
 */
// FUNCTION: FF7 0x0069ff72
int __cdecl D3DExecCompileGroup(int groupIndex, struct GfxExecTransformBlock *transformBlock, int renderStates, int triangleList, int vertexBuffer, GfxPolygonSet *polygonSet, GfxEngineContext *target)
{
    int result;
    int level;
    char buf[200];
    int desc[5];
    int dataInformation[9];
    GfxExecuteBufferDesc *descPointer;
    int compileInfo[9];
    int processVerts[12];
    GfxExecuteBufferDesc *bufDesc;
    result = 0;
    descPointer = (GfxExecuteBufferDesc *)desc;
    level = GetLogVerbosity(target);
    SetLogVerbosity(0, target);
    D3DExecResetCompileInfo(compileInfo);
    D3DExecResetCompileInfo(dataInformation);
    result = 0;
    if (polygonSet != 0 && D3DExecCompileBuffer(0, transformBlock, renderStates, triangleList, vertexBuffer, compileInfo, polygonSet)) {
        memset((void *)descPointer, 0, 20);
        descPointer->structSize = 20;
        descPointer->flags = 1;
        descPointer->bufferSize = compileInfo[0];
        // STRING: FF7 0x009126bc
        sprintf(buf, "EXECUTE BUFFER COMPILE SIZE %d. \n", compileInfo[0]);
        Gfx_DebugPrint(buf);
        // STRING: FF7 0x009126e0
        sprintf(buf, "RENDER STATES %d. \n", compileInfo[7]);
        Gfx_DebugPrint(buf);
        // STRING: FF7 0x009126f4
        sprintf(buf, "LIGHT STATES  %d. \n", compileInfo[8]);
        Gfx_DebugPrint(buf);
        // STRING: FF7 0x00912708
        if (CheckDirectDrawResult(((ComDirect3DDevice *)target->d3dDevice)->CreateExecuteBuffer(descPointer, &polygonSet->execBuffers[groupIndex], 0), "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x53e)) {
            // STRING: FF7 0x00912728
            sprintf(buf, "EXECUTE BUFFER OBJECT %p \n", polygonSet->execBuffers[groupIndex]);
            Gfx_DebugPrint(buf);
            bufDesc = descPointer;
            // STRING: FF7 0x00912744
            if (CheckDirectDrawResult(polygonSet->execBuffers[groupIndex]->Lock(bufDesc), "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x557)) {
                // STRING: FF7 0x00912764
                sprintf(buf, "EXECUTE BUFFER POINTER %p  SIZE %d. \n", bufDesc->data, bufDesc->bufferSize);
                Gfx_DebugPrint(buf);
                if (bufDesc->data != 0) {
                    dataInformation[0] = compileInfo[0];
                    dataInformation[1] = bufDesc->data;
                    dataInformation[8] = compileInfo[8];
                    dataInformation[7] = compileInfo[7];
                    if (D3DExecCompileBuffer(1, transformBlock, renderStates, triangleList, vertexBuffer, dataInformation, polygonSet)) {
                        // STRING: FF7 0x0091278c
                        Gfx_DebugPrint("AFTER DATA COMPILE \n");
                        // STRING: FF7 0x009127a4
                        if (CheckDirectDrawResult(polygonSet->execBuffers[groupIndex]->Unlock(), "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x575)) {
                            // STRING: FF7 0x009127c4
                            Gfx_DebugPrint("AFTER UNLOCK \n");
                            memset(processVerts, 0, sizeof(processVerts));
                            processVerts[0] = 0x30;
                            processVerts[1] = dataInformation[2];
                            processVerts[2] = dataInformation[3];
                            processVerts[3] = dataInformation[4];
                            processVerts[4] = dataInformation[5];
                            processVerts[5] = dataInformation[6];
                            // STRING: FF7 0x009127d4
                            if (CheckDirectDrawResult(polygonSet->execBuffers[groupIndex]->SetExecuteData(processVerts), "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x58d)) {
                                // STRING: FF7 0x009127f4
                                Gfx_DebugPrint("EXECUTE BUFFER CREATED AND SET \n");
                                result = 1;
                            }
                        }
                    }
                } else {
                    // STRING: FF7 0x00912818
                    Gfx_DebugPrint("ERROR: INVALID EXECUTION BUFFER DATA POINTER. \n");
                    // STRING: FF7 0x00912848
                    CheckDirectDrawResult(polygonSet->execBuffers[groupIndex]->Unlock(), "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x59e);
                }
            }
        }
    }
    SetLogVerbosity(level, target);
    return result;
}

/*
 * Clears the 36-byte execute-buffer compile-info record.
 */
// FUNCTION: FF7 0x006a037d
void __cdecl D3DExecResetCompileInfo(void *compileInfo)
{
    memset(compileInfo, 0, 0x24);
}

/*
 * Sizes or writes execute buffer into compile-info record.
 */
// FUNCTION: FF7 0x006a0390
int __cdecl D3DExecCompileBuffer(int mode, GfxExecTransformBlock *transformBlock, int renderStates, int triangleList, int vertexBuffer, void *compileState, void *polygonSet)
{
    char line[0xC8];
    int res;
    int total;
    int writePointer;
    int vertStride;
    int vertexBytes;
    int transformSize;
    int numberTransforms;
    int rsBytes;
    int vertexCnt;
    int triListSize;
    GfxExecVertexOp *vertexOp;
    int vertexFormat;
    int actualSize;

    writePointer = 0;
    res = 0;
    if (triangleList != 0) {
        vertexCnt = ((GfxExecTriangleGroup *)triangleList)->vertexCount;
    } else if (vertexBuffer != 0) {
        vertexCnt = ((GfxExecBuffer *)vertexBuffer)->vertexCnt;
    } else {
        vertexCnt = 0;
    }
    if (mode == 0) {
        ((GfxExecCompileState *)compileState)->expectedSize = 0;
        ((GfxExecCompileState *)compileState)->writePointer = 0;
    } else {
        writePointer = ((GfxExecCompileState *)compileState)->writePointer;
    }
    vertStride = 0;
    vertexBytes = 0;
    if (vertexBuffer != 0) {
        vertStride = D3DExecVertexStride(vertexBuffer);
        vertexBytes = vertexCnt * vertStride;
        if (mode == 1) {
            writePointer = D3DExecBuildVertexArray(writePointer, renderStates, (struct GfxExecTriangleGroup *)triangleList, (struct GfxExecBuffer *)vertexBuffer, (struct GfxD3DExecuteBufferHeader *)polygonSet);
        }
    }
    Gfx_DebugPrint(DxRendLit_AfterVertexList());
    total = 0;
    if (renderStates != 0) {
        writePointer = D3DExecEmitRenderStateBlock(vertexBytes, mode, &rsBytes, writePointer, (struct PolygonHundredState *)renderStates, (struct GfxRenderStateCache *)compileState);
        total = total + rsBytes;
    }
    Gfx_DebugPrint(DxRendLit_AfterInstructionList());
    if (transformBlock != 0) {
        transformSize = 0;
        numberTransforms = 0;
        if (transformBlock->worldMatrix != 0) {
            writePointer = D3DExecEmitInstruction(D3DOP_STATETRANSFORM, 8, 1, &transformSize, writePointer);
            writePointer = D3DExecEmitTransformState(D3DTRANSFORMSTATE_WORLD, transformBlock->worldMatrix, &numberTransforms, &transformSize, writePointer);
        }
        if (transformBlock->viewMatrix != 0) {
            writePointer = D3DExecEmitInstruction(D3DOP_STATETRANSFORM, 8, 1, &transformSize, writePointer);
            writePointer = D3DExecEmitTransformState(D3DTRANSFORMSTATE_VIEW, transformBlock->viewMatrix, &numberTransforms, &transformSize, writePointer);
        }
        if (transformBlock->projectionMatrix != 0) {
            writePointer = D3DExecEmitInstruction(D3DOP_STATETRANSFORM, 8, 1, &transformSize, writePointer);
            writePointer = D3DExecEmitTransformState(D3DTRANSFORMSTATE_PROJECTION, transformBlock->projectionMatrix, &numberTransforms, &transformSize, writePointer);
        }
        total = total + transformSize;
    }
    Gfx_DebugPrint(DxRendLit_AfterTransform());
    if (vertexBuffer != 0) {
        Gfx_DebugPrint(DxRendLit_StartingProcessVertices());
        switch (((GfxExecBuffer *)vertexBuffer)->vertexFormat) {
        case GFX_D3D_VERTEX_FORMAT_VERTEX:
            vertexFormat = D3DPROCESSVERTICES_TRANSFORMLIGHT;
            break;
        case GFX_D3D_VERTEX_FORMAT_LVERTEX:
            vertexFormat = D3DPROCESSVERTICES_TRANSFORM;
            break;
        case GFX_D3D_VERTEX_FORMAT_TLVERTEX:
            vertexFormat = D3DPROCESSVERTICES_COPY;
            break;
        default:
            vertexFormat = D3DPROCESSVERTICES_TRANSFORMLIGHT;
            break;
        }
        writePointer = D3DExecEmitInstruction(D3DOP_PROCESSVERTICES, 0x10, 1, &total, writePointer);
        if (writePointer != 0) {
            vertexOp = (GfxExecVertexOp *)writePointer;
            memset((void *)vertexOp, 0, 0x10);
            vertexOp->vertexFormat = vertexFormat;
            vertexOp->startIndex = 0;
            vertexOp->destIndex = 0;
            vertexOp->vertexCount = vertexCnt;
            vertexOp->reserved = 0;
            vertexOp++;
            writePointer = (int)vertexOp;
        }
        total += 0x10;
    }
    Gfx_DebugPrint(DxRendLit_AfterProcessVertices());
    writePointer = D3DExecAlignInstructionStream(8, &total, writePointer);
    writePointer = D3DExecEmitTriangleList(mode, &triListSize, writePointer, (struct GfxTriangleRange *)triangleList, (struct GfxPolySet *)vertexBuffer);
    total = total + triListSize;
    Gfx_DebugPrint(DxRendLit_AfterTriangleList());
    writePointer = D3DExecEmitInstruction(D3DOP_EXIT, 0, 0, &total, writePointer);
    Gfx_DebugPrint(DxRendLit_AfterD3dopExit());
    if (mode == 1) {
        actualSize = writePointer - ((GfxExecCompileState *)compileState)->writePointer;
        if (actualSize == ((GfxExecCompileState *)compileState)->expectedSize) {
            ((GfxExecCompileState *)compileState)->vertexOffset = 0;
            ((GfxExecCompileState *)compileState)->vertexCnt = 0;
            if (vertexBuffer != 0) {
                ((GfxExecCompileState *)compileState)->vertexCnt = vertexCnt;
            }
            ((GfxExecCompileState *)compileState)->vertexBytes = vertexBytes;
            ((GfxExecCompileState *)compileState)->instructionLength = total;
            ((GfxExecCompileState *)compileState)->hVertexOffset = 0;
            res = 1;
        } else {
            sprintf(line, DxRendLit_ErrorExecutionBufferCompile(), ((GfxExecCompileState *)compileState)->expectedSize, actualSize);
            Gfx_DebugPrint(line);
        }
    } else {
        ((GfxExecCompileState *)compileState)->expectedSize = vertexBytes + total;
        res = 1;
    }
    return res;
}

/*
 * Counts bytes or emits instruction to execute buffer.
 */
// FUNCTION: FF7 0x006a08b6
int __cdecl D3DExecEmitInstruction(int opcode, int instrSize, int count, int *sizeOut, int stream)
{
    GfxD3DExecInstruction *ptr;

    *sizeOut += 4;
    if (stream != 0) {
        ptr = (GfxD3DExecInstruction *)stream;
        ptr->opcode = opcode;
        ptr->instrSize = instrSize;
        ptr->count = count;
        ptr++;
        stream = (int)ptr;
    }
    return stream;
}

/*
 * Appends a transform-state pair to the execute buffer.
 */
// FUNCTION: FF7 0x006a0905
int __cdecl D3DExecEmitTransformState(int stateType, int value, int *countOut, int *sizeOut, int stream)
{
    int ptr;

    *sizeOut += 8;
    (*countOut)++;
    if (stream != 0) {
        ptr = stream;
        ((GfxExecLightStateOp *)ptr)->stateType = stateType;
        ((GfxExecLightStateOp *)ptr)->value = value;
        ptr += 8;
        stream = ptr;
    }
    return stream;
}

/*
 * Copies a group's vertices out of polygon set into execute buffer.
 */
// FUNCTION: FF7 0x006a0956
int __cdecl D3DExecBuildVertexArray(int vertexOut, int modulate, GfxExecTriangleGroup *group, GfxExecBuffer *vertexBuffer, GfxD3DExecuteBufferHeader *header)
{
    int textureStart;
    int first;
    int textureIndex;
    int count;
    GfxTlVertex *vertexPointer;
    int hasTextureCoords;
    int vertexIdx;
    int colorBuf;
    int copyBuffer;
    int outPtr;
    int tmpVerts;
    float recipWidth;
    GfxTlVertex *frameWritePtr;

    hasTextureCoords = 0;
    if (group != 0) {
        count = group->vertexCount;
        first = group->firstVertex;
        textureStart = group->textureStart;
        textureIndex = textureStart;
        hasTextureCoords = group->hasTextureCoords;
    } else {
        count = vertexBuffer->vertexCnt;
        first = 0;
        textureStart = 0;
        textureIndex = textureStart;
        hasTextureCoords = vertexBuffer->texCoordCount > 0;
    }
    switch (vertexBuffer->vertexFormat) {
    case GFX_D3D_VERTEX_FORMAT_VERTEX:
        vertexPointer = (GfxTlVertex *)vertexOut;
        for (vertexIdx = first; vertexIdx < first + count; vertexIdx++) {
            vertexPointer->xBits = ((GfxExecPosition *)(vertexBuffer->positions))[vertexIdx].x;
            vertexPointer->yBits = ((GfxExecPosition *)(vertexBuffer->positions))[vertexIdx].y;
            vertexPointer->zBits = ((GfxExecPosition *)(vertexBuffer->positions))[vertexIdx].z;
            if (vertexBuffer->vectorCount > 0) {
                vertexPointer->rhwBits = ((GfxExecVertexVector *)(vertexBuffer->vectorArray))[vertexIdx].rhw;
                vertexPointer->color = ((GfxExecVertexVector *)(vertexBuffer->vectorArray))[vertexIdx].color;
                vertexPointer->specular = ((GfxExecVertexVector *)(vertexBuffer->vectorArray))[vertexIdx].specular;
            } else {
                vertexPointer->rhwBits = FLOAT_ONE_BITS;
                vertexPointer->color = 0;
                vertexPointer->specular = 0;
            }
            if (hasTextureCoords != 0 && textureIndex < vertexBuffer->texCoordCount) {
                vertexPointer->uBits = ((GfxExecTexCoord *)(vertexBuffer->texCoords))[textureIndex].tu;
                vertexPointer->vBits = ((GfxExecTexCoord *)(vertexBuffer->texCoords))[textureIndex].tv;
            } else {
                vertexPointer->uBits = 0;
                vertexPointer->vBits = 0;
            }
            vertexPointer++;
            textureIndex++;
        }
        vertexOut = (int)vertexPointer;
        break;
    case GFX_D3D_VERTEX_FORMAT_LVERTEX:
        if (vertexBuffer[1].texCoordCount != 0) {
            copyBuffer = (int)GameHeapAlloc(1, count * 0x20, (char *)(int)DxRendLit_SourcePath_1(), 0x11C);
            tmpVerts = (int)GameHeapAlloc(1, count * 0x20, (char *)(int)DxRendLit_SourcePath_2(), 0x11D);
            colorBuf = (int)GameHeapAlloc(1, count * 0x10, (char *)(int)DxRendLit_SourcePath_3(), 0x11E);
            header->structSize = 0x34;
            header->copyBuffer = copyBuffer;
            header->inVertexSize = 0x20;
            header->tmpVerts = tmpVerts;
            header->outVertexSize = 0x20;
            header->colorBuf = colorBuf;
            header->flags = 0x3F;
        }
        outPtr = vertexOut;
        for (vertexIdx = first; vertexIdx < first + count; vertexIdx++) {
            ((GfxTlVertex *)outPtr)->xBits = ((GfxExecPosition *)(vertexBuffer->positions))[vertexIdx].x;
            ((GfxTlVertex *)outPtr)->yBits = ((GfxExecPosition *)(vertexBuffer->positions))[vertexIdx].y;
            ((GfxTlVertex *)outPtr)->zBits = ((GfxExecPosition *)(vertexBuffer->positions))[vertexIdx].z;
            ((GfxTlVertex *)outPtr)->rhwBits = 0;
            ((GfxTlVertex *)outPtr)->color = ((int *)(vertexBuffer->colors))[vertexIdx];
            D3DExecApplyColorModulate((unsigned int *)(outPtr + 0x10), (void *)modulate);
            ((GfxTlVertex *)outPtr)->specular = 0xFF000000;
            if (hasTextureCoords > 0 && textureIndex < vertexBuffer->texCoordCount) {
                ((GfxTlVertex *)outPtr)->uBits = ((GfxExecTexCoord *)(vertexBuffer->texCoords))[textureIndex].tu;
                ((GfxTlVertex *)outPtr)->vBits = ((GfxExecTexCoord *)(vertexBuffer->texCoords))[textureIndex].tv;
            } else {
                ((GfxTlVertex *)outPtr)->uBits = 0;
                ((GfxTlVertex *)outPtr)->vBits = 0;
            }
            if (vertexBuffer[1].texCoordCount != 0) {
                *(struct D3DExecVertex *)(copyBuffer + vertexIdx * 0x20) = *(struct D3DExecVertex *)outPtr;
            }
            outPtr += 0x20;
            textureIndex++;
        }
        vertexOut = outPtr;
        break;
    case GFX_D3D_VERTEX_FORMAT_TLVERTEX:
        frameWritePtr = (GfxTlVertex *)vertexOut;
        for (vertexIdx = first; vertexIdx < first + count; vertexIdx++) {
            frameWritePtr->xBits = ((GfxExecPosition *)(vertexBuffer->positions))[vertexIdx].x;
            frameWritePtr->yBits = ((GfxExecPosition *)(vertexBuffer->positions))[vertexIdx].y;
            frameWritePtr->zBits = ((GfxExecPosition *)(vertexBuffer->positions))[vertexIdx].z;
            recipWidth = 1.0f;
            *(float *)&frameWritePtr->rhwBits = 1.0f / recipWidth;
            frameWritePtr->color = ((int *)(vertexBuffer->colors))[vertexIdx];
            /* Retail caller prototype (int, int) -> void preserved for /Od argument promotion on `D3DExecApplyColorModulate`. */
            ((void(__cdecl *)(int, int))D3DExecApplyColorModulate)((int)frameWritePtr + 0x10, modulate);
            frameWritePtr->specular = 0xFF000000;
            if (hasTextureCoords > 0 && textureIndex < vertexBuffer->texCoordCount) {
                frameWritePtr->uBits = ((GfxExecTexCoord *)(vertexBuffer->texCoords))[textureIndex].tu;
                frameWritePtr->vBits = ((GfxExecTexCoord *)(vertexBuffer->texCoords))[textureIndex].tv;
            } else {
                frameWritePtr->uBits = 0;
                frameWritePtr->vBits = 0;
            }
            frameWritePtr++;
            textureIndex++;
        }
        vertexOut = (int)frameWritePtr;
        break;
    }
    return vertexOut;
}

/*
 * Pads the execute buffer to an aligned boundary with NOP instruction.
 */
// FUNCTION: FF7 0x006a0e1b
int __cdecl D3DExecAlignInstructionStream(int alignment, int *sizePtr, int stream)
{
    if ((*sizePtr & alignment - 1) != 0) {
        stream = D3DExecEmitInstruction(D3DOP_TRIANGLE, 8, 0, sizePtr, stream);
    }
    return stream;
}

/*
 * Returns the 32-byte execute-buffer vertex stride.
 */
// FUNCTION: FF7 0x006a0e4d
int __cdecl D3DExecVertexStride(int buffer)
{
    int stride;

    switch (((GfxExecBuffer *)buffer)->vertexFormat) {
    case GFX_D3D_VERTEX_FORMAT_VERTEX:
        stride = 0x20;
        break;
    case GFX_D3D_VERTEX_FORMAT_LVERTEX:
        stride = 0x20;
        break;
    case GFX_D3D_VERTEX_FORMAT_TLVERTEX:
        stride = 0x20;
        break;
    default:
        stride = 0x20;
    }
    return stride;
}

/*
 * Counts or emits selected render and light states.
 */
// FUNCTION: FF7 0x006a0e99
int __cdecl D3DExecEmitRenderStateBlock(int blockOffset, int mode, int *sizeOut, int buffer, PolygonHundredState *stateBlock, GfxRenderStateCache *stateCache)
{
    /* Names preserve the VC5 frame, including the 200-byte diagnostic buffer. */
    int statusMask;
    int lightSize;
    int flag;
    char debugText[200];
    int count;
    GfxEngineContext *context;
    int prevLightCount;
    int setting;
    int renderCount;
    int previousRenderCount;
    int nextValue;
    int lightId;

    count = 0;
    statusMask = stateBlock->changeMask;
    lightSize = 0;
    renderCount = 0;
    if (mode == 0) {
        buffer = 0;
        prevLightCount = 0;
        previousRenderCount = 0;
    } else {
        prevLightCount = stateCache->lightSize;
        previousRenderCount = stateCache->renderCount;
    }
    if (stateBlock->emitStates != 0) {
        buffer = D3DExecEmitInstruction(D3DOP_STATERENDER, 8, previousRenderCount, &count, buffer);
        Gfx_DebugPrint(DxRendLit_AfterD3dopStaterender());
        if (mode == 1) {
            sprintf(debugText, DxRendLit_Size_1(), buffer - stateCache->baseOffset);
        } else {
            sprintf(debugText, DxRendLit_Size_2(), count + blockOffset);
        }
        Gfx_DebugPrint(debugText);
        if ((statusMask & 1) != 0) {
            if ((stateBlock->flags & 1) != 0) {
                setting = 2;
            } else {
                setting = 3;
            }
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_FILLMODE, setting, &renderCount, &count, buffer);
        }
        if ((statusMask & 0x200) != 0) {
            if ((stateBlock->flags & 0x200) != 0) {
                flag = 1;
            } else {
                flag = 0;
            }
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_DITHERENABLE, flag, &renderCount, &count, buffer);
        }
        if ((statusMask & 4) != 0) {
            if ((stateBlock->flags & 4) != 0) {
                context = GetEngineContext();
                if (context != 0) {
                    if (context->modeSetupDone != 0) {
                        setting = 1;
                    } else {
                        setting = 2;
                    }
                } else {
                    setting = 1;
                }
            } else {
                setting = 1;
            }
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_TEXTUREMAG, setting, &renderCount, &count, buffer);
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_TEXTUREMIN, setting, &renderCount, &count, buffer);
        }
        if ((statusMask & 8) != 0) {
            if ((stateBlock->flags & 8) != 0) {
                flag = 1;
            } else {
                flag = 0;
            }
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, flag, &renderCount, &count, buffer);
        }
        if ((statusMask & 0x4000) != 0) {
            if ((stateBlock->flags & 0x4000) != 0) {
                setting = 1;
                buffer = D3DExecEmitRenderState(D3DRENDERSTATE_CULLMODE, setting, &renderCount, &count, buffer);
            } else if ((statusMask & 0x2000) != 0) {
                if ((stateBlock->flags & 0x2000) != 0) {
                    setting = 2;
                } else {
                    setting = 3;
                }
                buffer = D3DExecEmitRenderState(D3DRENDERSTATE_CULLMODE, setting, &renderCount, &count, buffer);
            }
        } else if ((statusMask & 0x2000) != 0) {
            if ((stateBlock->flags & 0x2000) != 0) {
                setting = 2;
            } else {
                setting = 3;
            }
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_CULLMODE, setting, &renderCount, &count, buffer);
        }
        if ((statusMask & 0x8000) != 0) {
            if ((stateBlock->flags & 0x8000) != 0) {
                flag = 1;
            } else {
                flag = 0;
            }
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_ZENABLE, flag, &renderCount, &count, buffer);
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL, &renderCount, &count, buffer);
        }
        Gfx_DebugPrint(DxRendLit_AfterD3drenderstateZenable());
        if (mode == 1) {
            sprintf(debugText, DxRendLit_Size_3(), buffer - stateCache->baseOffset);
        } else {
            sprintf(debugText, DxRendLit_Size_4(), count + blockOffset);
        }
        Gfx_DebugPrint(debugText);
        if ((statusMask & 0x10000) != 0) {
            if ((stateBlock->flags & 0x10000) != 0) {
                flag = 1;
            } else {
                flag = 0;
            }
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_ZWRITEENABLE, flag, &renderCount, &count, buffer);
        }
        if ((statusMask & 0x400) != 0) {
            if ((stateBlock->flags & 0x400) != 0) {
                flag = 1;
            } else {
                flag = 0;
            }
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, flag, &renderCount, &count, buffer);
            if (flag != 0) {
                setting = stateBlock->srcBlend;
                buffer = D3DExecEmitRenderState(D3DRENDERSTATE_SRCBLEND, setting, &renderCount, &count, buffer);
                setting = stateBlock->dstBlend;
                buffer = D3DExecEmitRenderState(D3DRENDERSTATE_DESTBLEND, setting, &renderCount, &count, buffer);
            } else {
                setting = 2;
                buffer = D3DExecEmitRenderState(D3DRENDERSTATE_SRCBLEND, setting, &renderCount, &count, buffer);
                setting = 1;
                buffer = D3DExecEmitRenderState(D3DRENDERSTATE_DESTBLEND, setting, &renderCount, &count, buffer);
            }
        }
        Gfx_DebugPrint(DxRendLit_AfterD3drenderstateBlendenable());
        if ((statusMask & 0x10) != 0) {
            if ((stateBlock->flags & 0x10) != 0) {
                setting = 2;
            } else {
                setting = 2;
            }
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, setting, &renderCount, &count, buffer);
        }
        if ((statusMask & 0x800) != 0) {
            if ((stateBlock->flags & 0x800) != 0) {
                flag = 1;
            } else {
                flag = 0;
            }
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_ALPHATESTENABLE, flag, &renderCount, &count, buffer);
            if (flag != 0) {
                setting = D3DCMP_GREATER;
                nextValue = stateBlock->alphaRef;
                buffer = D3DExecEmitRenderState(D3DRENDERSTATE_ALPHAFUNC, setting, &renderCount, &count, buffer);
                buffer = D3DExecEmitRenderState(D3DRENDERSTATE_ALPHAREF, nextValue, &renderCount, &count, buffer);
            } else {
                setting = D3DCMP_ALWAYS;
                buffer = D3DExecEmitRenderState(D3DRENDERSTATE_ALPHAFUNC, setting, &renderCount, &count, buffer);
                buffer = D3DExecEmitRenderState(D3DRENDERSTATE_ALPHAREF, stateBlock->alphaRef, &renderCount, &count, buffer);
            }
        }
        if ((statusMask & 0x1000) != 0) {
            if ((stateBlock->flags & 0x1000) != 0) {
                flag = 1;
            } else {
                flag = 0;
            }
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_ANTIALIAS, flag, &renderCount, &count, buffer);
        }
        if ((statusMask & 0x100000) != 0) {
            if ((stateBlock->flags & 0x100000) != 0) {
                flag = 1;
            } else {
                flag = 0;
            }
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_FOGENABLE, flag, &renderCount, &count, buffer);
        }
        if ((statusMask & 0x40000) != 0) {
            if ((stateBlock->flags & 0x40000) != 0) {
                flag = 1;
            } else {
                flag = 0;
            }
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_SPECULARENABLE, flag, &renderCount, &count, buffer);
        }
        Gfx_DebugPrint(DxRendLit_AfterD3drenderstateSpecularenable());
        if ((statusMask & 0x20000) != 0) {
            if ((stateBlock->flags & 0x20000) != 0) {
                setting = stateBlock->shadeMode;
            } else {
                setting = 1;
            }
            buffer = D3DExecEmitRenderState(D3DRENDERSTATE_SHADEMODE, setting, &renderCount, &count, buffer);
        }
        Gfx_DebugPrint(DxRendLit_AfterD3drenderstateShademode());
        if ((statusMask & 0x80000) != 0) {
            lightId = stateBlock->ambientColor;
            buffer = D3DExecEmitInstruction(D3DOP_STATELIGHT, 8, prevLightCount, &count, buffer);
            Gfx_DebugPrint(DxRendLit_AfterD3dopStatelight());
            if (stateBlock->material != 0 && (stateBlock->flags & 0x80000) != 0) {
                buffer = D3DExecEmitLightState(D3DLIGHTSTATE_MATERIAL, *(int *)stateBlock->material, &lightSize, &count, buffer);
                buffer = D3DExecEmitLightState(D3DLIGHTSTATE_AMBIENT, lightId, &lightSize, &count, buffer);
            } else {
                buffer = D3DExecEmitLightState(D3DLIGHTSTATE_MATERIAL, 0, &lightSize, &count, buffer);
                buffer = D3DExecEmitLightState(D3DLIGHTSTATE_AMBIENT, lightId, &lightSize, &count, buffer);
            }
            Gfx_DebugPrint(DxRendLit_AfterD3dlightstateMaterialD3dlightstate());
        }
    }
    stateCache->lightSize = lightSize;
    stateCache->renderCount = renderCount;
    *sizeOut = count;
    if (mode == 1) {
        if (lightSize != prevLightCount) {
            Gfx_DebugPrint(DxRendLit_TotalLightStatesError());
        }
        if (renderCount != previousRenderCount) {
            Gfx_DebugPrint(DxRendLit_TotalRenderStatesError());
        }
    }
    return buffer;
}

/*
 * Appends one render-state pair to the execute buffer.
 */
// FUNCTION: FF7 0x006a1893
int __cdecl D3DExecEmitRenderState(int stateType, int stateValue, int *countPtr, int *sizePtr, int buffer)
{
    int cursor;

    *sizePtr += 8;
    (*countPtr)++;
    if (buffer != 0) {
        cursor = buffer;
        ((GfxExecRenderStateOp *)cursor)->stateType = stateType;
        ((GfxExecRenderStateOp *)cursor)->stateValue = stateValue;
        cursor += 8;
        buffer = cursor;
    }
    return buffer;
}

/*
 * Appends one light-state pair to the execute buffer.
 */
// FUNCTION: FF7 0x006a18e4
int __cdecl D3DExecEmitLightState(int lightStateType, int lightValue, int *countPtr, int *sizePtr, int buffer)
{
    int cursor;

    *sizePtr += 8;
    (*countPtr)++;
    if (buffer != 0) {
        cursor = buffer;
        ((GfxExecLightStateOp *)cursor)->stateType = lightStateType;
        ((GfxExecLightStateOp *)cursor)->value = lightValue;
        cursor += 8;
        buffer = cursor;
    }
    return buffer;
}

/*
 * Emits a triangle-list instruction and triangles into execute buffer.
 */
// FUNCTION: FF7 0x006a1935
int __cdecl D3DExecEmitTriangleList(int emitData, int *sizePtr, int buffer, GfxTriangleRange *range, GfxPolySet *polySet)
{
    int triCount;
    int baseIndex;
    GfxTriangleIndexRecord *cursor;
    int triIndex;

    *sizePtr = 0;
    if (polySet != 0) {
        if (range != 0) {
            triCount = range->triCount;
            baseIndex = range->baseIndex;
        } else {
            triCount = polySet->triCount;
            baseIndex = 0;
        }
        buffer = D3DExecEmitInstruction(D3DOP_TRIANGLE, 8, triCount, sizePtr, buffer);
        *sizePtr += triCount * 8;
        if (emitData == 1) {
            cursor = (GfxTriangleIndexRecord *)buffer;
            for (triIndex = baseIndex; triIndex < baseIndex + triCount; triIndex++) {
                cursor->vertexIndexA = ((GfxTriangleRecord *)(polySet->triangles))[triIndex].vertexIndexA;
                cursor->vertexIndexB = ((GfxTriangleRecord *)(polySet->triangles))[triIndex].vertexIndexB;
                cursor->vertexIndexC = ((GfxTriangleRecord *)(polySet->triangles))[triIndex].vertexIndexC;
                cursor->flags = 0x700;
                cursor++;
            }
            buffer = (int)cursor;
        }
    }
    return buffer;
}

/*
 * Renders a deferred pass via Direct3D execute buffer.
 */
// FUNCTION: FF7 0x006a1a37
void __cdecl D3DExecRenderDeferredPass(int passKind, int deferredList, GfxEngineContext *renderer)
{
    int viewportIface;
    int entryBuffer;
    ListNode *listEntry;
    int entryIndex;
    GfxDeferredPassEntry *passEnt;
    int stateMatch;
    int curShouldDrawVal;
    int flagSrc;
    int matrix;
    GfxDrawStateNode *seqPtr;
    int execBits;
    int executeIface;
    int unused;
    GlDrawGroup *group8;
    int gated;
    int mtxTable;
    int recordPointer;
    int statusFlag;
    int tmpHresult;
    GfxTextureSurface *batchObjPtr;
    int localMatrix;
    int bitsValue;
    int altBits;
    GfxDeferredPass *deferredPass;

    unused = 0;
    executeIface = renderer->d3dDevice;
    viewportIface = renderer->viewport;
    if (deferredList != 0 && renderer != 0 && executeIface != 0 && viewportIface != 0) {
        listEntry = (ListNode *)(((List *)deferredList)->head);
        while (listEntry != 0) {
            deferredPass = (GfxDeferredPass *)(listEntry->value);
            if (deferredPass != 0 && deferredPass->active != 0) {
                mtxTable = deferredPass->mtxTable;
                passEnt = &deferredPass->entry;
                gated = 0;
                if (passEnt->active != 0) {
                    gated = 1;
                    if (*(int *)passEnt->statePtr == passEnt->expected) {
                        stateMatch = 1;
                    } else {
                        stateMatch = 0;
                    }
                }
                group8 = (GlDrawGroup *)deferredPass->drawGroup;
                statusFlag = 0;
                flagSrc = deferredPass->flagSrc;
                if (flagSrc != 0 && (*(int *)flagSrc & 2) != 0) {
                    statusFlag = 1;
                }
                Gfx_SetRenderStateCall(RENDER_STATE_SPECULAR, statusFlag, (struct GfxEngineContext *)renderer);
                recordPointer = 0;
                if (deferredPass->recordSource != 0) {
                    recordPointer = *(int *)((char *)deferredPass->recordSource + 0x6C);
                }
                for (entryIndex = 0; entryIndex < deferredPass->count; entryIndex++) {
                    if (group8 != 0) {
                        switch (passKind) {
                        case GFX_SCENE_PASS_OPAQUE:
                            curShouldDrawVal = (group8->flags & 0x410) == 0;
                            break;
                        case GFX_SCENE_PASS_TRANSLUCENT:
                            curShouldDrawVal = group8->flags & 0x410;
                            break;
                        default:
                            curShouldDrawVal = 0;
                        }
                    } else {
                        curShouldDrawVal = 1;
                    }
                    if (curShouldDrawVal != 0) {
                        if (group8 != 0) {
                            batchObjPtr = (GfxTextureSurface *)(group8->batch);
                            if (batchObjPtr != 0 && batchObjPtr->surface != 0) {
                                tmpHresult = ((ComDirectDrawSurface *)(batchObjPtr->surface))->IsLost();
                                if (tmpHresult == DDERR_SURFACELOST) {
                                    group8->batch = (int)Gfx_RestoreTextureSurfaces(batchObjPtr);
                                    if (group8->batch == 0) {
                                        break;
                                    }
                                }
                            }
                        }
                        if (gated != 0) {
                            if (stateMatch != 0) {
                                seqPtr = (GfxDrawStateNode *)passEnt->nodePtr;
                                if (seqPtr != 0) {
                                    switch (seqPtr->kind) {
                                    case GFX_DRAW_OP_BATCH:
                                        do {
                                            switch (passEnt->kind) {
                                            case GFX_POLYGON_SET_TRANSFORM_NONE:
                                                matrix = 0;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE:
                                                matrix = (int)seqPtr->matrix;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                                ConcatAffineMatrix4x4(seqPtr->matrix, (void *)renderer->cameraMatrix, &g_GfxIdentityMatrix);
                                                matrix = (int)&g_GfxIdentityMatrix;
                                                break;
                                            default:
                                                matrix = 0;
                                            }
                                            if (matrix != 0 && mtxTable != 0) {
                                                Gfx_SetMatrixCall(0, matrix, mtxTable, renderer);
                                            }
                                            entryBuffer = ((int *)(deferredPass->entries))[entryIndex];
                                            if (entryBuffer != 0) {
                                                if (deferredPass->pending != 0) {
                                                    execBits = D3DEXECUTE_CLIPPED;
                                                } else {
                                                    execBits = D3DEXECUTE_UNCLIPPED;
                                                }
                                                // STRING: FF7 0x00912868
                                                CheckDirectDrawResult(((ComDirect3DDevice *)(executeIface))->Execute(entryBuffer, viewportIface, execBits), "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x69F);
                                            }
                                            seqPtr = (GfxDrawStateNode *)(seqPtr->next);
                                        } while (seqPtr != 0);
                                        break;
                                    case GFX_DRAW_OP_DRAW_LIST:
                                        do {
                                            Gfx_EmitPolygonSetPrimitive(0, (int)group8, (struct GfxDrawListNode *)seqPtr->drawNode);
                                            switch (passEnt->kind) {
                                            case GFX_POLYGON_SET_TRANSFORM_NONE:
                                                localMatrix = 0;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE:
                                                localMatrix = (int)seqPtr->matrix;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                                ConcatAffineMatrix4x4(seqPtr->matrix, (void *)renderer->cameraMatrix, &g_GfxIdentityMatrix);
                                                localMatrix = (int)&g_GfxIdentityMatrix;
                                                break;
                                            default:
                                                localMatrix = 0;
                                            }
                                            if (localMatrix != 0 && mtxTable != 0) {
                                                Gfx_SetMatrixCall(0, localMatrix, mtxTable, renderer);
                                            }
                                            entryBuffer = ((int *)(deferredPass->entries))[entryIndex];
                                            if (entryBuffer != 0) {
                                                if (deferredPass->pending != 0) {
                                                    altBits = D3DEXECUTE_CLIPPED;
                                                } else {
                                                    altBits = D3DEXECUTE_UNCLIPPED;
                                                }
                                                // STRING: FF7 0x00912888
                                                CheckDirectDrawResult(((ComDirect3DDevice *)(executeIface))->Execute(entryBuffer, viewportIface, altBits), "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x6FA);
                                            }
                                            seqPtr = (GfxDrawStateNode *)(seqPtr->next);
                                        } while (seqPtr != 0);
                                    }
                                }
                            }
                        } else {
                            entryBuffer = ((int *)(deferredPass->entries))[entryIndex];
                            if (entryBuffer != 0) {
                                if (deferredPass->pending != 0) {
                                    bitsValue = 1;
                                } else {
                                    bitsValue = 2;
                                }
                                // STRING: FF7 0x009128a8
                                CheckDirectDrawResult(((ComDirect3DDevice *)(executeIface))->Execute(entryBuffer, viewportIface, bitsValue), "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x749);
                            }
                        }
                    }
                    if (group8 != 0) {
                        group8++;
                    }
                    if (recordPointer != 0) {
                        recordPointer += 0x38;
                    }
                }
            }
            listEntry = (ListNode *)(listEntry->next);
        }
    }
}

/*
 * Restores surface and dispatches deferred passes through native COM views.
 */
// FUNCTION: FF7 0x006a1efb
void __cdecl D3D_RenderDeferredLists(int passSelect, GfxEngineContext *device)
{
    int sceneObject;
    int failed;
    int execIface;

    if (device != 0 && device->minimized == 0 && device->d3dDevice != 0) {
        failed = 0;
        if (device->zBufferSurface != 0) {
            if (((ComDirectDrawSurface *)(device->zBufferSurface))->IsLost() == DDERR_SURFACELOST) {
                // STRING: FF7 0x009128c8
                failed |= CheckDirectDrawResult(((ComDirectDrawSurface *)(device->zBufferSurface))->Restore(), "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x77D);
            }
        }
        if (failed == 0) {
            if (device->useDirect3D5 != 0) {
                if (D3D5SetRenderTargetAndViewport(0, 1, GfxGetSurfaceByIndex(1), device->viewport2, device) != 0) {
                    sceneObject = device->d3dDevice2;
                    if (sceneObject != 0) {
                        // STRING: FF7 0x009128e8
                        if (CheckDirectDrawResult(((ComDirect3DDevice2 *)(sceneObject))->BeginScene(), "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x793) != 0) {
                            if (passSelect == GFX_SCENE_PASS_OPAQUE || passSelect == GFX_SCENE_PASS_BOTH) {
                                device->opaqueSet->drawEnabled = 1;
                                device->transSet->drawEnabled = 0;
                                D3D5DrawGroupListPass(GFX_SCENE_PASS_OPAQUE, device->polygonSetList, device);
                            }
                            if (passSelect == GFX_SCENE_PASS_TRANSLUCENT || passSelect == GFX_SCENE_PASS_BOTH) {
                                device->opaqueSet->drawEnabled = 0;
                                device->transSet->drawEnabled = 1;
                                D3D5DrawGroupListPass(GFX_SCENE_PASS_TRANSLUCENT, device->polygonSetList, device);
                            }
                            // STRING: FF7 0x00912908
                            CheckDirectDrawResult(((ComDirect3DDevice2 *)(sceneObject))->EndScene(), "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x7B4);
                        }
                    }
                }
            } else {
                execIface = device->d3dDevice;
                if (execIface != 0) {
                    // STRING: FF7 0x00912928
                    if (CheckDirectDrawResult(((ComDirect3DDevice *)(execIface))->BeginScene(), "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x7C0) != 0) {
                        if (passSelect == GFX_SCENE_PASS_OPAQUE || passSelect == GFX_SCENE_PASS_BOTH) {
                            device->opaqueSet->drawEnabled = 1;
                            device->transSet->drawEnabled = 0;
                            D3DExecRenderDeferredPass(GFX_SCENE_PASS_OPAQUE, device->polygonSetList, device);
                        }
                        if (passSelect == GFX_SCENE_PASS_TRANSLUCENT || passSelect == GFX_SCENE_PASS_BOTH) {
                            device->opaqueSet->drawEnabled = 0;
                            device->transSet->drawEnabled = 1;
                            D3DExecRenderDeferredPass(GFX_SCENE_PASS_TRANSLUCENT, device->polygonSetList, device);
                        }
                        // STRING: FF7 0x00912948
                        CheckDirectDrawResult(((ComDirect3DDevice *)(execIface))->EndScene(), "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x7DE);
                    }
                }
            }
        }
    }
}

/*
 * Renders deferred display lists with primitive kind 5.
 */
// FUNCTION: FF7 0x006a219d
void __cdecl Gfx_RenderDeferredListsD3D(struct GfxEngineContext *device)
{
    D3D_RenderDeferredLists(GFX_SCENE_PASS_BOTH, device);
}

/*
 * Empty callback for graphics device list creation.
 */
// FUNCTION: FF7 0x006a21b0
void __cdecl D3DListNoopCallback(void)
{
}

/*
 * Adds polygon set to device record list.
 */
// FUNCTION: FF7 0x006a21b5
void __cdecl D3DAppendPolygonSetToList(GfxPolygonSet *newSet, struct GfxEngineContext *device)
{
    GfxPolygonSet *polySet;

    if (newSet != 0) {
        polySet = newSet;
        ListAppendValue((int)polySet, (List *)(device->polygonSetList));
    }
}

/*
 * Removes polygon set from device record list.
 */
// FUNCTION: FF7 0x006a21df
void __cdecl D3DRemovePolygonSetFromList(void *polySet, void *device)
{
    GfxPolygonSet *setPtr;

    if (polySet != 0) {
        setPtr = (GfxPolygonSet *)polySet;
        RemoveFromList((int)setPtr, (List *)(((GfxEngineContext *)device)->polygonSetList));
    }
}

/*
 * Allocates and configures render-state block for opaque pass.
 */
// FUNCTION: FF7 0x006a2209
int __cdecl D3DAllocOpaquePassState(struct GfxEngineContext *device)
{
    PolygonHundredState *stateBlock;

    stateBlock = (PolygonHundredState *)Gfx_AllocPHundred();
    if (stateBlock != 0) {
        stateBlock->flags = 0;
        if (device->modeSetupDone == 0) {
            stateBlock->flags |= 4;
        }
        stateBlock->flags |= 8;
        stateBlock->flags |= 0x200;
        stateBlock->flags |= 0x100;
        stateBlock->flags |= 0x8000;
        stateBlock->flags |= 0x10000;
        stateBlock->flags |= 0x20000;
        stateBlock->shadeMode = 2;
        stateBlock->ambientColor = 0xFFFFFFFF;
        stateBlock->srcBlend = 2;
        stateBlock->dstBlend = 1;
    }
    return (int)stateBlock;
}

/*
 * Allocates and configures render-state block for translucent pass.
 */
// FUNCTION: FF7 0x006a22cf
int __cdecl D3DAllocTranslucentPassState(struct GfxEngineContext *device)
{
    PolygonHundredState *stateBlock;

    stateBlock = (PolygonHundredState *)Gfx_AllocPHundred();
    if (stateBlock != 0) {
        stateBlock->flags = 0;
        if (device->modeSetupDone == 0) {
            stateBlock->flags |= 4;
        }
        stateBlock->flags |= 8;
        stateBlock->flags |= 0x400;
        stateBlock->flags |= 0x100;
        stateBlock->flags |= 0x8000;
        stateBlock->flags &= 0xFFFEFFFF;
        stateBlock->flags |= 0x20000;
        stateBlock->shadeMode = 2;
        stateBlock->ambientColor = 0xFFFFFFFF;
        stateBlock->srcBlend = 5;
        stateBlock->dstBlend = 6;
        stateBlock->dstBlend = 2;
    }
    return (int)stateBlock;
}

/*
 * Allocates and configures opaque and translucent polygon sets for device.
 */
// FUNCTION: FF7 0x006a239f
void __cdecl D3DSetupDevicePolygonSets(struct GfxEngineContext *device)
{
    PfileLoadOptions optBuffer;
    GfxPolygonSet *opaqueSet;
    GfxPolygonSet *transSetPtr;
    int transStatus;
    int opaqueState;
    GfxMatrixArray *mtxTable;
    PfileLoadOptions *optionsPointer;
    GfxMatrix4x4 *projMatrix;
    GfxMatrix4x4 *pRotZMatrix;
    GfxMatrix4x4 *rotYMatrix;
    if (device != 0) {
        optionsPointer = &optBuffer;
        Pfile_InitLoadOptions(optionsPointer);
        mtxTable = 0;
        opaqueState = D3DAllocOpaquePassState(device);
        transStatus = D3DAllocTranslucentPassState(device);
        if (opaqueState != 0 && transStatus != 0) {
            mtxTable = (GfxMatrixArray *)((int (__cdecl *)(void))Gfx_CreateEmptyMatrixTable)(); /* reads eax after a void call, as the original does */
            if (mtxTable != 0) {
                projMatrix = (GfxMatrix4x4 *)Gfx_AllocIdentityMatrix4x4();
                if (projMatrix != 0) {
                    BuildProjectionMatrix(GetViewportAspectRatio(device), device->fov, device->nearPlane, device->farPlane, device->offsetX, device->offsetY, (float)device->viewSizeX, (float)device->viewSizeY, (float(*)[4])(int)projMatrix);
                    Gfx_SetMatrixCall(2, (int)projMatrix, (int)mtxTable, device);
                }
                rotYMatrix = (GfxMatrix4x4 *)Gfx_AllocIdentityMatrix4x4();
                if (rotYMatrix != 0) {
                    BuildRotationYRowVec(0, (float(*)[4])(int)rotYMatrix);
                    Gfx_SetMatrixCall(1, (int)rotYMatrix, (int)mtxTable, device);
                }
                pRotZMatrix = (GfxMatrix4x4 *)Gfx_AllocIdentityMatrix4x4();
                if (pRotZMatrix != 0) {
                    BuildRotationZRowVec(0, (float(*)[4])(int)pRotZMatrix);
                    Gfx_SetMatrixCall(0, (int)pRotZMatrix, (int)mtxTable, device);
                }
                D3DResetFullscreenViewportRect(device);
            }
            opaqueSet = GfxLoadPolygonSet(mtxTable, (struct PolygonHundredState *)opaqueState, 0, (struct GfxPolygonSetSpec *)optionsPointer, device);
            if (opaqueSet != 0) {
                D3DAppendPolygonSetToList(opaqueSet, device);
                device->opaqueSet = opaqueSet;
            }
            transSetPtr = GfxLoadPolygonSet(0, (struct PolygonHundredState *)transStatus, 0, (struct GfxPolygonSetSpec *)optionsPointer, device);
            if (transSetPtr != 0) {
                D3DAppendPolygonSetToList(transSetPtr, device);
                device->transSet = transSetPtr;
            }
            SetPerspective(device->fov, device->nearPlane, device->farPlane, device->offsetX, device->offsetY, (float)device->viewSizeX, (float)device->viewSizeY, device);
        }
    }
}

/*
 * Releases and tears down device polygon sets and attached buffers.
 */
// FUNCTION: FF7 0x006a2631
void __cdecl D3DReleaseDevicePolygonSets(GfxEngineContext *slots)
{
    if (slots != 0) {
        D3DRemovePolygonSetFromList(slots->transSet, slots);
        D3DRemovePolygonSetFromList(slots->opaqueSet, slots);
        DestroyRenderMaterial((struct GfxRenderMaterial *)(slots->opaqueSet->matrices));
        Gfx_FreePHundred(slots->transSet->groupState);
        Gfx_FreePHundred(slots->opaqueSet->groupState);
        D3DFreePolygonSet(slots->transSet);
        D3DFreePolygonSet(slots->opaqueSet);
        slots->opaqueSet = 0;
    }
}

/*
 * Sets the hasGroupState flag of a polygon set.
 */
// FUNCTION: FF7 0x006a26dc
void __cdecl D3DSetPolygonSetHasGroupState(int value, void *record)
{
    if (record != 0) {
        ((GfxPolygonSet *)record)->hasGroupState = value;
    }
}

/*
 * Returns the address of group record index in the polygon set's 0x64-byte group record array.
 */
// FUNCTION: FF7 0x006a26f0
int __cdecl D3DGetPolygonSetGroup(int index, void *polySet)
{
    int group;

    group = 0;
    if (polySet != 0 && ((GfxPolygonSet *)polySet)->groupState != 0) {
        group = ((GfxPolygonSet *)polySet)->groupState + index * 0x64;
    }
    return group;
}

/*
 * Sets the useGroupStates flag of a polygon set.
 */
// FUNCTION: FF7 0x006a2722
void __cdecl D3DSetPolygonSetUseGroupStates(int value, void *record)
{
    if (record != 0) {
        ((GfxPolygonSet *)record)->useGroupStates = value;
    }
}

/*
 * Maps each group slot in polygon set to its own record.
 */
// FUNCTION: FF7 0x006a2736
void __cdecl D3DLinkPolygonSetGroupsIdentity(void *polySet)
{
    int index;

    if (polySet != 0) {
        for (index = 0; index < ((GfxPolygonSet *)polySet)->groupCount; index++) {
            D3DSetPolygonSetGroupSlot(index, ((GfxPolygonSet *)polySet)->groupState + index * 0x64, polySet);
        }
    }
}

/*
 * Sets group pointer slot in polygon set table.
 */
// FUNCTION: FF7 0x006a2782
void __cdecl D3DSetPolygonSetGroupSlot(int index, int groupRecord, void *polySet)
{
    if (polySet != 0 && groupRecord != 0 && index < ((GfxPolygonSet *)polySet)->groupCount) {
        *(int *)((char *)((GfxPolygonSet *)polySet)->groupStates + index * 4) = groupRecord;
    }
}

/*
 * Sets all group slots in polygon set table to a single record.
 */
// FUNCTION: FF7 0x006a27ad
void __cdecl D3DSetAllPolygonSetGroupSlots(int group, void *polySet)
{
    int index;

    if (polySet != 0 && group != 0) {
        for (index = 0; index < ((GfxPolygonSet *)polySet)->groupCount; index++) {
            D3DSetPolygonSetGroupSlot(index, group, polySet);
        }
    }
}

/*
 * Frees a block-heap node list and destroys its chain.
 */
// FUNCTION: FF7 0x006a27f4
void __cdecl DestroyBlockHeapNodeList(int list)
{
    if (list != 0) {
        DestroyBlockHeapChain((MemBlockHeap *)(((GfxBlockHeapNodeList *)list)->heapChain));
        // STRING: FF7 0x00912968
        GameHeapFree((void *)list, "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x930);
    }
}

/*
 * Allocates a block-heap node list with initial blocks.
 */
// FUNCTION: FF7 0x006a2823
int __cdecl CreateBlockHeapNodeList(int count)
{
    int list;

    // STRING: FF7 0x00912988
    list = (int)GameHeapAlloc(1, 0xC, "C:\\lib\\src\\graphics\\dx_rend.cpp", 0x938);
    if (list != 0) {
        ((GfxBlockHeapNodeList *)list)->heapChain = CreateBlockHeapCount(count, 0xA0, 0);
    }
    return list;
}

/*
 * Allocates and appends a node to the block-heap list.
 */
// FUNCTION: FF7 0x006a2865
int __cdecl AppendBlockHeapNode(int list)
{
    GfxPolygonSetNode *node;
    GfxPolygonSetNode *block;

    block = (GfxPolygonSetNode *)(AllocBlockFromHeap((MemBlockHeap *)(((GfxBlockHeapNodeList *)list)->heapChain)));
    node = block;
    if (node != 0) {
        node->nextNode = 0;
        if (((GfxBlockHeapNodeList *)list)->head != 0) {
            ((GfxPolygonSetNode *)((GfxBlockHeapNodeList *)list)->tail)->nextNode = (int)node;
        } else {
            ((GfxBlockHeapNodeList *)list)->head = (int)node;
        }
        ((GfxBlockHeapNodeList *)list)->tail = (int)node;
    }
    return (int)node;
}

/*
 * Clears and rewinds block-heap node list storage.
 */
// FUNCTION: FF7 0x006a28c0
void __cdecl ResetBlockHeapNodeList(GfxBlockHeapNodeList *list)
{
    if (list != 0) {
        list->head = 0;
        list->tail = 0;
        ResetBlockHeapChain((MemBlockHeap *)(list->heapChain));
    }
}
} /* extern "C" */
