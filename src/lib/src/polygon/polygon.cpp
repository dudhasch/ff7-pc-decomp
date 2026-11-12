// src/lib/src/polygon/polygon.cpp
// Original: C:\lib\src\polygon\polygon.cpp
// Contents: Polygon record management: creates and frees polygon data, collects vertices, polygons,
//   normals, blend states and groups in edit lists that are flattened into arrays, and loads,
//   saves, welds and scales the geometry.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/enums/file.h"
#include "lib/types/file.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/types/list.h"
#include "lib/codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/api/list.h"
#include "lib/api/polygon.h"
#include "lib/api/graphics.h"
#include "lib/api/file.h"
extern "C" {

#pragma intrinsic(memcpy)

typedef struct PolygonColor {
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
} PolygonColor;

// polygon.cpp's literal stream follows the original source lines: Pfile_CreateEditLists' __FILE__ literal
// (line 0x86) precedes Pfile_CreatePolygonData's (0x9A), although .text places Pfile_CreateEditLists after its
// caller. The literals up to it are returned by static __inline functions, parse-time units; under /Ob1 the call
// expands to the plain operand.
static __inline char *PolygonLit_SourcePath_1(void)
{
    // STRING: FF7 0x00911a9c
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_2(void)
{
    // STRING: FF7 0x00911abc
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_3(void)
{
    // STRING: FF7 0x00911adc
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_4(void)
{
    // STRING: FF7 0x00911afc
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_5(void)
{
    // STRING: FF7 0x00911b1c
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_6(void)
{
    // STRING: FF7 0x00911b3c
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_7(void)
{
    // STRING: FF7 0x00911b5c
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_8(void)
{
    // STRING: FF7 0x00911b7c
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_9(void)
{
    // STRING: FF7 0x00911b9c
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_10(void)
{
    // STRING: FF7 0x00911bbc
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_11(void)
{
    // STRING: FF7 0x00911bdc
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_12(void)
{
    // STRING: FF7 0x00911bfc
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_13(void)
{
    // STRING: FF7 0x00911c1c
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_14(void)
{
    // STRING: FF7 0x00911c3c
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_15(void)
{
    // STRING: FF7 0x00911c5c
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
static __inline char *PolygonLit_SourcePath_16(void)
{
    // STRING: FF7 0x00911c7c
    return "C:\\lib\\src\\polygon\\polygon.cpp";
}
/*
 * Destroys all edit lists and frees the list container for a polygon record.
 */
// FUNCTION: FF7 0x00699470
void __cdecl Pfile_DestroyEditLists(PolygonFileArrays *record)
{
    if (record != 0 && record->editSource != 0) {
        DestroyListAndNull(&record->editSource->positions);
        DestroyListAndNull(&record->editSource->normals);
        DestroyListAndNull(&record->editSource->unknown08);
        DestroyListAndNull(&record->editSource->texCoords);
        DestroyListAndNull(&record->editSource->vertexColors);
        DestroyListAndNull(&record->editSource->polygons);
        DestroyListAndNull(&record->editSource->hundreds);
        DestroyListAndNull(&record->editSource->groups);
        DestroyListAndNull(&record->editSource->sourceIndices);
        GameHeapFree(record->editSource, PolygonLit_SourcePath_1(), 31);
        record->editSource = 0;
    }
}

/*
 * Frees allocated vertex, normal, color, and index arrays of a polygon record.
 */
// FUNCTION: FF7 0x0069954b
void __cdecl Pfile_FreeArrays(int freeRecord, PolygonFileArrays *record)
{
    if (record != 0) {
        if (record->vertices != 0) {
            GameHeapFree((void *)record->vertices, PolygonLit_SourcePath_2(), 43);
            record->vertices = 0;
        }
        if (record->normals != 0) {
            GameHeapFree((void *)record->normals, PolygonLit_SourcePath_3(), 48);
            record->normals = 0;
        }
        if (record->xyzRecords != 0) {
            GameHeapFree((void *)record->xyzRecords, PolygonLit_SourcePath_4(), 53);
            record->xyzRecords = 0;
        }
        if (record->texCoords != 0) {
            GameHeapFree((void *)record->texCoords, PolygonLit_SourcePath_5(), 58);
            record->texCoords = 0;
        }
        if (record->vertexColors != 0) {
            GameHeapFree((void *)record->vertexColors, PolygonLit_SourcePath_6(), 63);
            record->vertexColors = 0;
        }
        if (record->polygonColors != 0) {
            GameHeapFree((void *)record->polygonColors, PolygonLit_SourcePath_7(), 68);
            record->polygonColors = 0;
        }
        if (record->edges != 0) {
            GameHeapFree((void *)record->edges, PolygonLit_SourcePath_8(), 74);
            record->edges = 0;
        }
        if (record->polygons != 0) {
            GameHeapFree((void *)record->polygons, PolygonLit_SourcePath_9(), 80);
            record->polygons = 0;
        }
        if (record->hundreds != 0) {
            GameHeapFree((void *)record->hundreds, PolygonLit_SourcePath_10(), 85);
            record->hundreds = 0;
        }
        if (record->groups != 0) {
            GameHeapFree((void *)record->groups, PolygonLit_SourcePath_11(), 90);
            record->groups = 0;
        }
        if (record->extraPolys != 0) {
            GameHeapFree((void *)record->extraPolys, PolygonLit_SourcePath_12(), 96);
            record->extraPolys = 0;
        }
        if (record->auxByteTriplets != 0) {
            free((void *)record->auxByteTriplets);
            record->auxByteTriplets = 0;
        }
        if (record->boundingBoxes != 0) {
            GameHeapFree((void *)record->boundingBoxes, PolygonLit_SourcePath_13(), 106);
            record->boundingBoxes = 0;
        }
        if (record->sourceIndices != 0) {
            GameHeapFree((void *)record->sourceIndices, PolygonLit_SourcePath_14(), 112);
            record->sourceIndices = 0;
        }
        if (freeRecord != 0) {
            Pfile_DestroyEditLists(record);
            GameHeapFree(record, PolygonLit_SourcePath_15(), 120);
        }
    }
}

/*
 * Frees a polygon record and all associated member arrays.
 */
// FUNCTION: FF7 0x006997b6
void __cdecl Pfile_Free(PolygonFileArrays *record)
{
    Pfile_FreeArrays(1, record);
}

/*
 * Creates an empty `.P` polygon record for geometry built in code and returns it, or 0: `version`
 * 1, `initFlag` 1, `kind`, `editMode` 1 (so `Pfile_FlattenEditLists` also keeps each vertex's source
 * index), all counts and arrays 0. With `createEdits` set it also makes the
 * nine edit lists (`Pfile_CreateEditLists`) that `Pfile_AddVertex`, `Pfile_AddPolygon`,
 * `Pfile_AddHundred` and `Pfile_AddGroup` append to before the lists are flattened into arrays.
 * `Gfx_BuildPrimitiveMesh` and battle's LMD model builder use it.
 */
// FUNCTION: FF7 0x006997c9
PolygonFileArrays *__cdecl Pfile_CreatePolygonData(int createEdits, int kind)
{
    PolygonFileArrays *record;

    // STRING: FF7 0x00911c9c
    record = (PolygonFileArrays *)GameHeapAlloc(1, sizeof(PolygonFileArrays), "C:\\lib\\src\\polygon\\polygon.cpp", 154);
    if (record != 0) {
        record->version = 1;
        record->initFlag = 1;
        record->kind = kind;
        record->editMode = 1;
        record->hostTransformFlag = 0;
        if (createEdits != 0) {
            Pfile_CreateEditLists(record);
        }
    }
    return record;
}

/*
 * Allocates linked lists for editing polygon vertices, normals, and groups.
 */
// FUNCTION: FF7 0x00699838
void __cdecl Pfile_CreateEditLists(PolygonFileArrays *record)
{
    if (record != 0) {
        record->editSource = (PolygonEditLists *)GameHeapAlloc(1, sizeof(PolygonEditLists), PolygonLit_SourcePath_16(), 134);
        if (record->editSource != 0) {
            record->editSource->positions = CreateList(ListFreeValueGameHeap);
            record->editSource->normals = CreateList(ListFreeValueGameHeap);
            record->editSource->unknown08 = CreateList(ListFreeValueGameHeap);
            record->editSource->texCoords = CreateList(ListFreeValueGameHeap);
            record->editSource->vertexColors = CreateList(0);
            record->editSource->polygons = CreateList(ListFreeValueGameHeap);
            record->editSource->hundreds = CreateList(ListFreeValueGameHeap);
            record->editSource->groups = CreateList(ListFreeValueGameHeap);
            record->editSource->sourceIndices = CreateList(0);
        }
    }
}

/*
 * Allocates a heap block and clones a fixed-size record into it.
 */
// FUNCTION: FF7 0x0069992f
int __cdecl Pfile_CloneRecordToHeap(int size, void *src, char *file, int line)
{
    int block;
    block = (int)GameHeapAllocRaw(size, file, line);
    if (block != 0) {
        memcpy((void *)block, src, size);
    }
    return block;
}

/*
 * Appends a vertex to the edit lists: copies of its position, normal and texture coordinate, its
 * colour word and, in edit mode, its index in the source mesh. Any of the pointers may be 0.
 */
// FUNCTION: FF7 0x00699972
void __cdecl Pfile_AddVertex(int sourceIndex, int *color, void *texCoord, void *normal, void *position, PolygonFileArrays *record)
{
    int elemValue;
    PolygonEditLists *edits;
    int normalNode;
    int positionNode;
    int texNode;

    if (record != 0 && record->editSource != 0) {
        edits = record->editSource;
        if (position != 0) {
            // STRING: FF7 0x00911cbc
            positionNode = Pfile_CloneRecordToHeap(12, position, "C:\\lib\\src\\polygon\\polygon.cpp", 202);
            ListAppendValue(positionNode, edits->positions);
        }
        if (normal != 0) {
            // STRING: FF7 0x00911cdc
            normalNode = Pfile_CloneRecordToHeap(12, normal, "C:\\lib\\src\\polygon\\polygon.cpp", 206);
            ListAppendValue(normalNode, edits->normals);
        }
        if (texCoord != 0) {
            // STRING: FF7 0x00911cfc
            texNode = Pfile_CloneRecordToHeap(8, texCoord, "C:\\lib\\src\\polygon\\polygon.cpp", 210);
            ListAppendValue(texNode, edits->texCoords);
        }
        if (color != 0) {
            elemValue = *color;
            ListAppendValue(elemValue, edits->vertexColors);
        }
        if (record->editMode != 0) {
            elemValue = sourceIndex;
            ListAppendValue(elemValue, edits->sourceIndices);
        }
    }
}

/*
 * Appends a copy of a 0x18-byte polygon to the polygon edit list.
 */
// FUNCTION: FF7 0x00699a7a
void __cdecl Pfile_AddPolygon(void *polygon, PolygonFileArrays *record)
{
    PolygonEditLists *lists;
    int clone;

    if (record != 0) {
        lists = record->editSource;
        if (lists != 0 && polygon != 0) {
            // STRING: FF7 0x00911d1c
            clone = Pfile_CloneRecordToHeap(24, polygon, "C:\\lib\\src\\polygon\\polygon.cpp", 235);
            ListAppendValue(clone, lists->polygons);
        }
    }
}

/*
 * Appends a copy of a 0x18-byte polygon and, when given, of a 12-byte record to the third list
 * (no source calls it, so what the record holds is not shown).
 */
// FUNCTION: FF7 0x00699acd
void __cdecl Pfile_AddPolygonWithNormal(void *unknown08, void *polygon, PolygonFileArrays *record)
{
    PolygonEditLists *lists;
    int polyNode;
    int clone;

    if (record != 0) {
        lists = record->editSource;
        if (lists != 0 && polygon != 0) {
            if (unknown08 != 0) {
                // STRING: FF7 0x00911d3c
                clone = Pfile_CloneRecordToHeap(12, unknown08, "C:\\lib\\src\\polygon\\polygon.cpp", 251);
                ListAppendValue(clone, lists->unknown08);
            }
            // STRING: FF7 0x00911d5c
            polyNode = Pfile_CloneRecordToHeap(24, polygon, "C:\\lib\\src\\polygon\\polygon.cpp", 254);
            ListAppendValue(polyNode, lists->polygons);
        }
    }
}

/*
 * Appends a copy of a 12-byte normal to the normal edit list.
 */
// FUNCTION: FF7 0x00699b54
void __cdecl Pfile_AddNormal(void *normal, PolygonFileArrays *record)
{
    PolygonEditLists *lists;
    int clone;

    if (record != 0 && record->editSource != 0) {
        lists = record->editSource;
        if (normal != 0) {
            // STRING: FF7 0x00911d7c
            clone = Pfile_CloneRecordToHeap(12, normal, "C:\\lib\\src\\polygon\\polygon.cpp", 268);
            ListAppendValue(clone, lists->normals);
        }
    }
}

/*
 * Appends a copy of a 0x64-byte hundred (a render state) to the hundred edit list.
 */
// FUNCTION: FF7 0x00699baa
void __cdecl Pfile_AddHundred(void *hundred, PolygonFileArrays *record)
{
    PolygonEditLists *lists;
    int clone;

    if (record != 0 && record->editSource != 0 && hundred != 0) {
        lists = record->editSource;
        // STRING: FF7 0x00911d9c
        clone = Pfile_CloneRecordToHeap(100, hundred, "C:\\lib\\src\\polygon\\polygon.cpp", 281);
        ListAppendValue(clone, lists->hundreds);
    }
}

/*
 * Appends a copy of a 0x38-byte group (its polygon, vertex and texture-coordinate ranges) to the
 * group edit list.
 */
// FUNCTION: FF7 0x00699c00
void __cdecl Pfile_AddGroup(void *group, PolygonFileArrays *record)
{
    PolygonEditLists *lists;
    int clone;

    if (record != 0 && record->editSource != 0 && group != 0) {
        lists = record->editSource;
        // STRING: FF7 0x00911dbc
        clone = Pfile_CloneRecordToHeap(56, group, "C:\\lib\\src\\polygon\\polygon.cpp", 293);
        ListAppendValue(clone, lists->groups);
    }
}

/*
 * Flattens polygon edit lists into flat arrays and frees list nodes.
 */
// FUNCTION: FF7 0x00699c56
void __cdecl Pfile_FlattenEditLists(PolygonFileArrays *record)
{
    PolygonEditLists *lists;

    if (record != 0 && record->editSource != 0) {
        Pfile_FreeArrays(0, record);
        lists = record->editSource;
        record->vertexCount = ListGetCount(lists->positions);
        record->normalCount = ListGetCount(lists->normals);
        record->xyzCount = ListGetCount(lists->unknown08);
        record->texCoordCount = ListGetCount(lists->texCoords);
        record->vertexColorCount = ListGetCount(lists->vertexColors);
        record->polygonCount = ListGetCount(lists->polygons);
        record->extraPolyCount = 0;
        record->auxByteTripletCount = 0;
        record->hundredCount = ListGetCount(lists->hundreds);
        record->groupCount = ListGetCount(lists->groups);
        record->boundingBoxCount = 0;
        record->vertices = (int)ListToArray(1, 12, lists->positions);
        record->normals = (int)ListToArray(1, 12, lists->normals);
        record->xyzRecords = (int)ListToArray(1, 12, lists->unknown08);
        record->texCoords = (int)ListToArray(1, 8, lists->texCoords);
        record->vertexColors = (int)ListToArray(0, 4, lists->vertexColors);
        record->polygons = (int)ListToArray(1, 24, lists->polygons);
        record->hundreds = (int)ListToArray(1, 100, lists->hundreds);
        record->groups = (int)ListToArray(1, 56, lists->groups);
        if (record->editMode != 0) {
            record->sourceIndices = (int)ListToArray(0, 4, lists->sourceIndices);
        }
    }
}

/*
 * Stub function for post-load polygon data fixups; the loader passes the arrays it read.
 */
// FUNCTION: FF7 0x00699e3a
void __cdecl Pfile_PostLoadFixupStub(PolygonFileArrays *arrays)
{
}

/*
 * Converts float color components into packed 32-bit integer colors.
 */
// FUNCTION: FF7 0x00699e3f
void __cdecl Pfile_BuildPolygonColors(PolygonFileArrays *object)
{
    struct {
        int vertex;
        float red;
        float alpha;
        int iblue;
        float green;
        int igreen;
        int ired;
        float blue;
        PolygonColor output;
        PolygonColor input;
        int ialpha;
        int vertexLoop;
        int polygonLoop;
        PolygonTriangle *polygon;
        PolygonFileGroup *group;
        int groupLoop;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (object != 0) {
        // STRING: FF7 0x00911ddc
        object->polygonColors = (int)GameHeapAlloc(object->polygonCount, 4, "C:\\lib\\src\\polygon\\polygon.cpp", 354);
        if (object->polygonColors != 0) {
            frame.group = (PolygonFileGroup *)object->groups;
            for (frame.groupLoop = 0; frame.groupLoop < object->groupCount; frame.groupLoop++) {
                frame.polygon = (PolygonTriangle *)(object->polygons + frame.group->firstPolygon * 0x18);
                for (frame.polygonLoop = 0; frame.polygonLoop < frame.group->polygonCount; frame.polygonLoop++) {
                    frame.red = 0;
                    frame.green = 0;
                    frame.blue = 0;
                    frame.alpha = 0;
                    for (frame.vertexLoop = 0; frame.vertexLoop < 3; frame.vertexLoop++) {
                        frame.vertex = (&frame.polygon->vertexA)[frame.vertexLoop];
                        frame.input = *(PolygonColor *)(object->vertexColors + (frame.group->firstPosition + frame.vertex) * 4);
                        frame.red += (*(int *)((char *)&frame.input + 2) & 0xFF);
                        frame.green += (*(int *)((char *)&frame.input + 1) & 0xFF);
                        frame.blue += (*(int *)&frame.input & 0xFF);
                        frame.alpha += (*(int *)((char *)&frame.input + 3) & 0xFF);
                    }
                    frame.red *= 0.3333333432674408f;
                    frame.green *= 0.3333333432674408f;
                    frame.blue *= 0.3333333432674408f;
                    frame.alpha *= 0.3333333432674408f;
                    /* Retail rounds via bare x87 fistp rather than VC5's truncating _ftol. */
                    __asm { fld frame.red }
                    __asm { fistp frame.ired }
                    __asm { fld frame.green }
                    __asm { fistp frame.igreen }
                    __asm { fld frame.blue }
                    __asm { fistp frame.iblue }
                    __asm { fld frame.alpha }
                    __asm {fistp frame.ialpha} frame.output.r = (unsigned char)frame.ired;
                    frame.output.g = (unsigned char)frame.igreen;
                    frame.output.b = (unsigned char)frame.iblue;
                    frame.output.a = (unsigned char)frame.ialpha;
                    *(PolygonColor *)(object->polygonColors + (frame.group->firstPolygon + frame.polygonLoop) * 4) = frame.output;
                    frame.polygon++;
                }
                frame.group++;
            }
        }
    }
}

/*
 * Loads the `.P` geometry file `fileName` (opened with `request`, from disk or an LGP archive)
 * and returns its polygon record, or 0. Battle models and stages, the primitive mesh bank and
 * `Pfile_LoadRsdContents` (for an RSD's PLY name) load geometry this way.
 *
 * The 0x80-byte file header is read straight into a new record and its array pointers are
 * cleared. A version word other than 1 prints "INVALID VERSION IN POLYGON FILE" and fails.
 * Otherwise the arrays follow in file order, each counted by its header word: 12-byte vertex
 * positions, 12-byte normals, a third 12-byte array, 8-byte texture coordinates, 4-byte vertex
 * colours, one 4-byte colour word per polygon, `edgeCount` 4-byte edges, 0x18-byte polygons, `extraPolyCount`
 * 0x18-byte records, `auxByteTripletCount` 3-byte records, 0x64-byte hundreds (render states), 0x38-byte
 * groups and `boundingBoxCount` 0x1C-byte bounding boxes; with `editMode` set, one 4-byte source index per
 * vertex. With `withEdits` set, empty edit lists are added.
 *
 * Defects: the array reads are not checked, so a short file leaves arrays null with their counts
 * set; when the header read itself comes up short, the record is freed with the pointer words
 * it got from the file.
 */
// FUNCTION: FF7 0x0069a028
PolygonFileArrays *__cdecl Pfile_LoadBinary(FileOpenRequest *request, int withEdits, int fileName)
{
    struct {
        FileHandle *fileHandle;
        int loadSucceeded;
        PolygonFileArrays *fileSections;
        char errorMsg[200];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.loadSucceeded = 1;
    frame.fileSections = Pfile_CreatePolygonData(0, 0);
    if (frame.fileSections != 0) {
        frame.fileHandle = OpenFileEx(request, (const char *)fileName);
        if (frame.fileHandle != 0) {
            if (ReadFileData(sizeof(PolygonFileArrays), frame.fileSections, frame.fileHandle) != 0) {
                frame.fileSections->vertices = 0;
                frame.fileSections->normals = 0;
                frame.fileSections->xyzRecords = 0;
                frame.fileSections->texCoords = 0;
                frame.fileSections->vertexColors = 0;
                frame.fileSections->polygonColors = 0;
                frame.fileSections->edges = 0;
                frame.fileSections->polygons = 0;
                frame.fileSections->extraPolys = 0;
                frame.fileSections->auxByteTriplets = 0;
                frame.fileSections->hundreds = 0;
                frame.fileSections->groups = 0;
                frame.fileSections->editSource = 0;
                frame.fileSections->boundingBoxes = 0;
                frame.fileSections->sourceIndices = 0;
                if (frame.fileSections->version == 1) {
                    frame.fileSections->vertices = ReadFileIntoNewBuffer(12, frame.fileSections->vertexCount, frame.fileHandle);
                    frame.fileSections->normals = ReadFileIntoNewBuffer(12, frame.fileSections->normalCount, frame.fileHandle);
                    frame.fileSections->xyzRecords = ReadFileIntoNewBuffer(12, frame.fileSections->xyzCount, frame.fileHandle);
                    frame.fileSections->texCoords = ReadFileIntoNewBuffer(8, frame.fileSections->texCoordCount, frame.fileHandle);
                    frame.fileSections->vertexColors = ReadFileIntoNewBuffer(4, frame.fileSections->vertexColorCount, frame.fileHandle);
                    frame.fileSections->polygonColors = ReadFileIntoNewBuffer(4, frame.fileSections->polygonCount, frame.fileHandle);
                    frame.fileSections->edges = ReadFileIntoNewBuffer(4, frame.fileSections->edgeCount, frame.fileHandle);
                    frame.fileSections->polygons = ReadFileIntoNewBuffer(24, frame.fileSections->polygonCount, frame.fileHandle);
                    frame.fileSections->extraPolys = ReadFileIntoNewBuffer(24, frame.fileSections->extraPolyCount, frame.fileHandle);
                    frame.fileSections->auxByteTriplets = ReadFileIntoNewBuffer(3, frame.fileSections->auxByteTripletCount, frame.fileHandle);
                    frame.fileSections->hundreds = ReadFileIntoNewBuffer(100, frame.fileSections->hundredCount, frame.fileHandle);
                    frame.fileSections->groups = ReadFileIntoNewBuffer(56, frame.fileSections->groupCount, frame.fileHandle);
                    frame.fileSections->boundingBoxes = ReadFileIntoNewBuffer(28, frame.fileSections->boundingBoxCount, frame.fileHandle);
                    if (frame.fileSections->editMode != 0) {
                        frame.fileSections->sourceIndices = ReadFileIntoNewBuffer(4, frame.fileSections->vertexCount, frame.fileHandle);
                    }
                    if (withEdits != 0) {
                        Pfile_CreateEditLists(frame.fileSections);
                        Pfile_PostLoadFixupStub(frame.fileSections);
                    }
                    frame.loadSucceeded = 0;
                } else {
                    // STRING: FF7 0x00911dfc
                    sprintf(frame.errorMsg, "ERROR: INVALID VERSION IN POLYGON FILE %s \n", fileName);
                    Gfx_DebugPrint(frame.errorMsg);
                }
            }
            CloseFileEx(frame.fileHandle);
        }
    }
    if (frame.loadSucceeded != 0) {
        Pfile_Free(frame.fileSections);
        frame.fileSections = 0;
    }
    return frame.fileSections;
}

/*
 * Writes polygon record headers and array data to a binary file or archive.
 */
// FUNCTION: FF7 0x0069a408
void __cdecl Pfile_WriteBinaryEx(int skipWeld, int fileName, PolygonFileArrays *record)
{
    int handle;

    if (record != 0) {
        if (skipWeld != 0) { /* empty first arm: lib/codegen.h */
        } else {
            Pfile_WeldDuplicateVertices(record);
        }
        handle = OpenFileSimple(FILE_OPEN_CREATE, (void *)fileName);
        if (handle != 0) {
            if (WriteFileData(sizeof(PolygonFileArrays), record, (FileHandle *)handle) != 0) {
                WriteFileData(record->vertexCount * 0xC, (const void *)record->vertices, (FileHandle *)handle);
                WriteFileData(record->normalCount * 0xC, (const void *)record->normals, (FileHandle *)handle);
                WriteFileData(record->xyzCount * 0xC, (const void *)record->xyzRecords, (FileHandle *)handle);
                WriteFileData(record->texCoordCount << 3, (const void *)record->texCoords, (FileHandle *)handle);
                WriteFileData(record->vertexColorCount << 2, (const void *)record->vertexColors, (FileHandle *)handle);
                WriteFileData(record->polygonCount << 2, (const void *)record->polygonColors, (FileHandle *)handle);
                WriteFileData(record->edgeCount << 2, (const void *)record->edges, (FileHandle *)handle);
                WriteFileData(record->polygonCount * 0x18, (const void *)record->polygons, (FileHandle *)handle);
                WriteFileData(record->extraPolyCount * 0x18, (const void *)record->extraPolys, (FileHandle *)handle);
                WriteFileData(record->auxByteTripletCount * 3, (const void *)record->auxByteTriplets, (FileHandle *)handle);
                WriteFileData(record->hundredCount * 0x64, (const void *)record->hundreds, (FileHandle *)handle);
                WriteFileData(record->groupCount * 0x38, (const void *)record->groups, (FileHandle *)handle);
                WriteFileData(record->boundingBoxCount * 0x1C, (const void *)record->boundingBoxes, (FileHandle *)handle);
                if (record->editMode != 0) {
                    WriteFileData(record->vertexCount << 2, (const void *)record->sourceIndices, (FileHandle *)handle);
                }
            }
            CloseFileEx((FileHandle *)handle);
        }
    }
}

/*
 * Writes a polygon record to a standalone binary file.
 */
// FUNCTION: FF7 0x0069a611
void __cdecl Pfile_WriteBinary(int fileName, PolygonFileArrays *record)
{
    Pfile_WriteBinaryEx(0, fileName, record);
}

/*
 * Compares two texture coordinate pairs for exact bitwise equality.
 */
// FUNCTION: FF7 0x0069a628
int __cdecl Pfile_TexCoordEquals(PolygonTexCoord *uvA, PolygonTexCoord *uvB)
{
    int result;

    result = 0;
    if (uvA != 0 && uvB != 0 && uvA->u == uvB->u && uvA->v == uvB->v) {
        result = 1;
    }
    return result;
}

/*
 * Compares vertex coordinates and color values for exact equality.
 */
// FUNCTION: FF7 0x0069a671
int __cdecl Pfile_VertexAndColorEqual(PolygonTranslation *vertexA, PolygonTranslation *vertexB, int *colorA, int *colorB)
{
    int result;

    result = 0;
    if (vertexA->x == vertexB->x && vertexA->y == vertexB->y && vertexA->z == vertexB->z && *colorA == *colorB) {
        result = 1;
    }
    return result;
}

/*
 * Removes duplicate vertices and updates polygon index references.
 */
// FUNCTION: FF7 0x0069a6cd
void __cdecl Pfile_WeldDuplicateVertices(PolygonFileArrays *fileArrays)
{
    struct {
        int lastVertexIdx;
        int shiftPolyIdx;
        PolygonTriangle *shiftTriCursor;
        int origCornerVert;
        char texMatchMsg[200];
        char vertMatchMsg[200];
        PolygonTexCoord *candTexCoord;
        PolygonTexCoord *curTexCoord;
        PolygonTranslation *curPosition;
        int *candColor;
        PolygonTranslation *candPosition;
        int *vertexColor;
        PolygonFileGroup *fileBlock;
        int cornerIdx;
        int groupRemovedTex;
        int candVertIdx;
        int *groupTexShift;
        int polyBase;
        int totalRemovedTex;
        int groupIdx;
        int removedCount;
        int dataDOffset;
        int texMatchCount;
        int vertIdx;
        int *globalTexShift;
        int polyIdx;
        int vertMatchCount;
        int *vertCanonicalIdx;
        int groupRemovedVerts;
        PolygonTriangle *groupEntry;
        int *groupVertShift;
        int *texCanonicalIdx;
        int vertBase;
        int *globalVertShift;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (fileArrays != 0) {
        // STRING: FF7 0x00911e28
        frame.vertCanonicalIdx = (int *)GameHeapAlloc(fileArrays->vertexCount, 4, "C:\\lib\\src\\polygon\\polygon.cpp", 670);
        // STRING: FF7 0x00911e48
        frame.globalVertShift = (int *)GameHeapAlloc(fileArrays->vertexCount, 4, "C:\\lib\\src\\polygon\\polygon.cpp", 671);
        // STRING: FF7 0x00911e68
        frame.groupVertShift = (int *)GameHeapAlloc(fileArrays->vertexCount, 4, "C:\\lib\\src\\polygon\\polygon.cpp", 672);
        // STRING: FF7 0x00911e88
        frame.texCanonicalIdx = (int *)GameHeapAlloc(fileArrays->vertexCount, 4, "C:\\lib\\src\\polygon\\polygon.cpp", 673);
        // STRING: FF7 0x00911ea8
        frame.globalTexShift = (int *)GameHeapAlloc(fileArrays->vertexCount, 4, "C:\\lib\\src\\polygon\\polygon.cpp", 674);
        // STRING: FF7 0x00911ec8
        frame.groupTexShift = (int *)GameHeapAlloc(fileArrays->vertexCount, 4, "C:\\lib\\src\\polygon\\polygon.cpp", 675);
        if (frame.vertCanonicalIdx != 0 && frame.globalVertShift != 0 && frame.groupVertShift != 0 && frame.texCanonicalIdx != 0) {
            frame.fileBlock = (PolygonFileGroup *)fileArrays->groups;
            for (frame.groupIdx = 0; frame.groupIdx < fileArrays->groupCount; frame.groupIdx++) {
                frame.vertMatchCount = 0;
                frame.texMatchCount = 0;
                for (frame.vertIdx = 0; frame.vertIdx < frame.fileBlock->vertexCount; frame.vertIdx++) {
                    frame.vertCanonicalIdx[frame.fileBlock->firstPosition + frame.vertIdx] = frame.vertIdx;
                    if (frame.fileBlock->hasTexture != 0) {
                        frame.texCanonicalIdx[frame.fileBlock->firstTexCoord + frame.vertIdx] = frame.vertIdx;
                    }
                    frame.curPosition = (PolygonTranslation *)(fileArrays->vertices + (frame.fileBlock->firstPosition + frame.vertIdx) * 0xC);
                    frame.vertexColor = (int *)(fileArrays->vertexColors + (frame.fileBlock->firstPosition + frame.vertIdx) * 4);
                    for (frame.candVertIdx = 0; frame.candVertIdx < frame.vertIdx; frame.candVertIdx++) {
                        if (frame.vertIdx != frame.candVertIdx) {
                            frame.candPosition = (PolygonTranslation *)(fileArrays->vertices + (frame.fileBlock->firstPosition + frame.candVertIdx) * 0xC);
                            frame.candColor = (int *)(fileArrays->vertexColors + (frame.fileBlock->firstPosition + frame.candVertIdx) * 4);
                            if (Pfile_VertexAndColorEqual(frame.curPosition, frame.candPosition, frame.vertexColor, frame.candColor) != 0) {
                                if (frame.fileBlock->hasTexture != 0) {
                                    frame.curTexCoord = (PolygonTexCoord *)(fileArrays->texCoords + (frame.fileBlock->firstTexCoord + frame.vertIdx) * 8);
                                    frame.candTexCoord = (PolygonTexCoord *)(fileArrays->texCoords + (frame.fileBlock->firstTexCoord + frame.candVertIdx) * 8);
                                    if (Pfile_TexCoordEquals(frame.curTexCoord, frame.candTexCoord) != 0) {
                                        frame.vertCanonicalIdx[frame.fileBlock->firstPosition + frame.vertIdx] = frame.candVertIdx;
                                        frame.texCanonicalIdx[frame.fileBlock->firstTexCoord + frame.vertIdx] = frame.candVertIdx;
                                        frame.texMatchCount++;
                                        frame.vertMatchCount++;
                                        break;
                                    }
                                } else {
                                    frame.vertCanonicalIdx[frame.fileBlock->firstPosition + frame.vertIdx] = frame.candVertIdx;
                                    frame.vertMatchCount++;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (DEBUG_TRACE != 0) {
                    // STRING: FF7 0x00911ee8
                    sprintf(frame.vertMatchMsg, "MATCHES: %d OUT OF %d \n", frame.vertMatchCount, frame.fileBlock->vertexCount);
                    Gfx_DebugPrint(frame.vertMatchMsg);
                }
                if (DEAD_IN_RETAIL != 0) {
                    if (frame.fileBlock->hasTexture != 0) {
                        // STRING: FF7 0x00911f00
                        sprintf(frame.texMatchMsg, "T MATCHES: %d OUT OF %d \n", frame.texMatchCount, frame.fileBlock->vertexCount);
                        Gfx_DebugPrint(frame.texMatchMsg);
                    }
                }
                frame.fileBlock++;
            }
            frame.vertBase = 0;
            frame.polyBase = 0;
            frame.fileBlock = (PolygonFileGroup *)fileArrays->groups;
            for (frame.groupIdx = 0; frame.groupIdx < fileArrays->groupCount; frame.groupIdx++) {
                frame.groupEntry = (PolygonTriangle *)(fileArrays->polygons + frame.polyBase * 0x18);
                for (frame.polyIdx = 0; frame.polyIdx < frame.fileBlock->polygonCount; frame.polyIdx++) {
                    for (frame.cornerIdx = 0; frame.cornerIdx < 3; frame.cornerIdx++) {
                        frame.origCornerVert = (&frame.groupEntry->vertexA)[frame.cornerIdx];
                        (&frame.groupEntry->vertexA)[frame.cornerIdx] = *(short *)&frame.vertCanonicalIdx[frame.vertBase + frame.origCornerVert];
                    }
                    frame.groupEntry++;
                }
                frame.vertBase += frame.fileBlock->vertexCount;
                frame.polyBase += frame.fileBlock->polygonCount;
                frame.fileBlock++;
            }
            frame.removedCount = 0;
            frame.vertBase = 0;
            frame.totalRemovedTex = 0;
            frame.dataDOffset = 0;
            frame.fileBlock = (PolygonFileGroup *)fileArrays->groups;
            for (frame.groupIdx = 0; frame.groupIdx < fileArrays->groupCount; frame.groupIdx++) {
                frame.groupRemovedVerts = 0;
                frame.groupRemovedTex = 0;
                for (frame.vertIdx = 0; frame.vertIdx < frame.fileBlock->vertexCount; frame.vertIdx++) {
                    if (frame.vertIdx != frame.vertCanonicalIdx[frame.vertBase + frame.vertIdx]) {
                        frame.removedCount++;
                        frame.groupRemovedVerts++;
                        if (frame.fileBlock->hasTexture != 0) {
                            frame.totalRemovedTex++;
                            frame.groupRemovedTex++;
                        }
                    } else {
                        *(PolygonTranslation *)(fileArrays->vertices + (frame.vertBase + frame.vertIdx - frame.removedCount) * 0xC) = *(PolygonTranslation *)(fileArrays->vertices + (frame.vertBase + frame.vertIdx) * 0xC);
                        *(PolygonColor *)(fileArrays->vertexColors + (frame.vertBase + frame.vertIdx - frame.removedCount) * 4) = *(PolygonColor *)(fileArrays->vertexColors + (frame.vertBase + frame.vertIdx) * 4);
                        if (fileArrays->editMode != 0) {
                            ((int *)(fileArrays->sourceIndices))[frame.vertBase + frame.vertIdx - frame.removedCount] = ((int *)(fileArrays->sourceIndices))[frame.vertBase + frame.vertIdx];
                        }
                        if (frame.fileBlock->hasTexture != 0) {
                            *(PolygonTexCoord *)(fileArrays->texCoords + (frame.dataDOffset + frame.vertIdx - frame.totalRemovedTex) * 8) = *(PolygonTexCoord *)(fileArrays->texCoords + (frame.dataDOffset + frame.vertIdx) * 8);
                        }
                    }
                    frame.groupVertShift[frame.vertBase + frame.vertIdx] = frame.groupRemovedVerts;
                    frame.globalVertShift[frame.vertBase + frame.vertIdx] = frame.removedCount;
                    if (frame.fileBlock->hasTexture != 0) {
                        frame.groupTexShift[frame.dataDOffset + frame.vertIdx] = frame.groupRemovedTex;
                        frame.globalTexShift[frame.dataDOffset + frame.vertIdx] = frame.totalRemovedTex;
                    }
                }
                frame.vertBase += frame.fileBlock->vertexCount;
                if (frame.fileBlock->hasTexture != 0) {
                    frame.dataDOffset += frame.fileBlock->vertexCount;
                }
                frame.fileBlock++;
            }
            frame.vertBase = 0;
            frame.polyBase = 0;
            frame.fileBlock = (PolygonFileGroup *)fileArrays->groups;
            for (frame.groupIdx = 0; frame.groupIdx < fileArrays->groupCount; frame.groupIdx++) {
                frame.shiftTriCursor = (PolygonTriangle *)(fileArrays->polygons + frame.polyBase * 0x18);
                for (frame.shiftPolyIdx = 0; frame.shiftPolyIdx < frame.fileBlock->polygonCount; frame.shiftPolyIdx++) {
                    for (frame.cornerIdx = 0; frame.cornerIdx < 3; frame.cornerIdx++) {
                        (&frame.shiftTriCursor->vertexA)[frame.cornerIdx] = (&frame.shiftTriCursor->vertexA)[frame.cornerIdx] - frame.groupVertShift[frame.vertBase + (&frame.shiftTriCursor->vertexA)[frame.cornerIdx]];
                    }
                    frame.shiftTriCursor++;
                }
                frame.vertBase += frame.fileBlock->vertexCount;
                frame.polyBase += frame.fileBlock->polygonCount;
                frame.fileBlock++;
            }
            frame.fileBlock = (PolygonFileGroup *)fileArrays->groups;
            for (frame.groupIdx = 0; frame.groupIdx < fileArrays->groupCount; frame.groupIdx++) {
                frame.lastVertexIdx = frame.fileBlock->firstPosition + frame.fileBlock->vertexCount + -1 - *(int *)((char *)frame.globalVertShift + (frame.fileBlock->firstPosition + frame.fileBlock->vertexCount) * 4 + -4);
                frame.fileBlock->firstPosition -= frame.globalVertShift[frame.fileBlock->firstPosition];
                frame.fileBlock->vertexCount = frame.lastVertexIdx - frame.fileBlock->firstPosition + 1;
                if (frame.fileBlock->hasTexture != 0) {
                    frame.fileBlock->firstTexCoord -= frame.globalTexShift[frame.fileBlock->firstTexCoord];
                }
                frame.fileBlock++;
            }
            fileArrays->vertexCount -= *(int *)((char *)frame.globalVertShift + fileArrays->vertexCount * 4 + -4);
            fileArrays->vertexColorCount = fileArrays->vertexCount;
            if (fileArrays->texCoordCount > 0) {
                fileArrays->texCoordCount -= *(int *)((char *)frame.globalTexShift + fileArrays->texCoordCount * 4 + -4);
            }
        }
        // STRING: FF7 0x00911f1c
        GameHeapFree(frame.vertCanonicalIdx, "C:\\lib\\src\\polygon\\polygon.cpp", 930);
        // STRING: FF7 0x00911f3c
        GameHeapFree(frame.globalVertShift, "C:\\lib\\src\\polygon\\polygon.cpp", 931);
        // STRING: FF7 0x00911f5c
        GameHeapFree(frame.groupVertShift, "C:\\lib\\src\\polygon\\polygon.cpp", 932);
        // STRING: FF7 0x00911f7c
        GameHeapFree(frame.texCanonicalIdx, "C:\\lib\\src\\polygon\\polygon.cpp", 933);
        // STRING: FF7 0x00911f9c
        GameHeapFree(frame.globalTexShift, "C:\\lib\\src\\polygon\\polygon.cpp", 934);
        // STRING: FF7 0x00911fbc
        GameHeapFree(frame.groupTexShift, "C:\\lib\\src\\polygon\\polygon.cpp", 935);
    }
}

/*
 * Scales all vertex coordinates in a polygon record by a float factor.
 */
// FUNCTION: FF7 0x0069af4f
void __cdecl Pfile_ScaleVertices(float scale, PolygonFileArrays *arrays)
{
    PolygonTranslation *vertexPointer;
    int i;

    if (arrays != 0) {
        vertexPointer = (PolygonTranslation *)arrays->vertices;
        for (i = 0; i < arrays->vertexCount; i++) {
            vertexPointer->x *= scale;
            vertexPointer->y *= scale;
            vertexPointer->z *= scale;
            vertexPointer++;
        }
    }
}
} /* extern "C" */
