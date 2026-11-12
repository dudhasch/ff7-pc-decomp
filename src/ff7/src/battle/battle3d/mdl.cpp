// src/ff7/src/battle/battle3d/mdl.cpp
// Original: C:\FF7\src\battle\battle3d\mdl.cpp
// Contents: Battle model skeletons: builds a bone hierarchy from LMD model data and loads the parts
//   it uses. It also holds a debug helper that loads and tests the Arthur model.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/pfile.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "lib/types/polygon.h"

#include "lib/api/file.h"
#include "lib/api/polygon.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
extern "C" {

/*
 * Builds a skeleton hierarchy from LMD model data and loads its constituent parts.
 */
// FUNCTION: FF7 0x005e8550
void *__cdecl BattleModel_BuildSkeletonFromLmd(Battle3dPartyLoadDesc *battleLoadDesc, int unusedA, int unusedB, int *modelBlob, struct PolygonTim *timObject, void *loadOptions, char *baseName)
{
    /* These deliberately bucket-0 names are declared in reverse stack-slot order: VC5 emits
       locals sharing a hash bucket in reverse declaration order. */
    int i6;
    void *hz;
    Battle3dSkeletonPartBuffer *hj;
    void *gn;
    void *fr;
    int *fb;
    void *f2;
    Battle3dSkeletonPartBuffer *ev;
    void *ef;
    void *e6;
    int *dz;
    int dj;
    float cn;
    int br;
    Battle3dLmdFaceData *bb;
    int *b2;
    Battle3dSkeletonRoot *av;
    Battle3dSkeletonState *af;
    int a6;
    int p;

    av = 0;
    a6 = 0;
    if (modelBlob != 0) {
        b2 = modelBlob;
        a6 = *b2;
        b2++;
        if (a6 > 0) {
            av = (Battle3dSkeletonRoot *)Skel_Create(a6);
        } else {
            av = (Battle3dSkeletonRoot *)Skel_Create(a6 + 1);
        }
        if (av != 0) {
            bb = (Battle3dLmdFaceData *)Lmd_CreateTextureAtlas(2, (void *)timObject);
            if (bb != 0) {
                (*(char **)&bb->fileName) = baseName;
                bb->loadOptions = (PfileLoadOptions *)loadOptions;
            }
            if (a6 > 0) {
                dj = *(short *)b2;
                b2 = (int *)((char *)b2 + 2);
                cn = (float)*(short *)b2;
                b2 = (int *)((char *)b2 + 2);
                p = *b2;
                b2++;
                /* The empty test is the original's retained fld/fcomp at /Od. */
                if (cn == 0.0f) {
                }
                bb->faceKind = 0;
                dz = b2;
                for (br = 0; br < av->boneCount; br++) {
                    b2 = (int *)((char *)b2 + 2);
                    b2 = (int *)((char *)b2 + 2);
                    p = *b2;
                    b2++;
                    if (p != 0) {
                        e6 = (char *)modelBlob + p;
                        battleLoadDesc->nameId = br;
                        ef = (void *)Battle_BuildPolygonDataFromLmd(battleLoadDesc, (int)e6, bb, timObject);
                    }
                }
                b2 = dz;
                Lmd_BuildAtlasTextures(bb, timObject);
                bb->faceKind = 1;
                af = av->skeletonState;
                for (br = 0; br < av->boneCount; br++) {
                    af->name = 0;
                    af->parentName = 0;
                    af->lastIndex = *(short *)b2;
                    b2 = (int *)((char *)b2 + 2);
                    af->lastIndex -= 1;
                    af->boneOffset = (float)*(short *)b2;
                    b2 = (int *)((char *)b2 + 2);
                    p = *b2;
                    b2++;
                    af->childTable = 0;
                    if (p != 0) {
                        af->active = 1;
                    } else {
                        af->active = 0;
                    }
                    if (af->active > 0) {
                        // STRING: FF7 0x00900120
                        af->buffer = (Battle3dSkeletonPartBuffer *)GameHeapAlloc(1, 8, "C:\\FF7\\src\\battle\\battle3d\\mdl.cpp", 0xA7);
                        if (af->buffer != 0) {
                            ev = af->buffer;
                            f2 = (char *)modelBlob + p;
                            if (battleLoadDesc->loaded == 0) {
                                battleLoadDesc->nameId = br;
                            }
                            ev->part = Lmd_BuildAndRegisterPart(battleLoadDesc, (int)f2, timObject, loadOptions, bb, baseName);
                        }
                    }
                    af++;
                }
            } else {
                af = av->skeletonState;
                if (af != 0) {
                    bb->faceKind = 0;
                    fb = b2;
                    b2 = (int *)((char *)b2 + 2);
                    b2 = (int *)((char *)b2 + 2);
                    p = *b2;
                    b2++;
                    if (p != 0) {
                        fr = (char *)modelBlob + p;
                        if (battleLoadDesc->loaded == 0) {
                            battleLoadDesc->nameId = 0;
                        }
                        gn = (void *)Battle_BuildPolygonDataFromLmd(battleLoadDesc, (int)fr, bb, timObject);
                    }
                    b2 = fb;
                    Lmd_BuildAtlasTextures(bb, timObject);
                    bb->faceKind = 1;
                    af->name = 0;
                    af->parentName = 0;
                    af->lastIndex = -1;
                    b2 = (int *)((char *)b2 + 2);
                    af->boneOffset = (float)*(short *)b2;
                    af->boneOffset = 0;
                    b2 = (int *)((char *)b2 + 2);
                    p = *b2;
                    b2++;
                    af->active = 1;
                    // STRING: FF7 0x00900144
                    af->buffer = (Battle3dSkeletonPartBuffer *)GameHeapAlloc(1, 8, "C:\\FF7\\src\\battle\\battle3d\\mdl.cpp", 0x103);
                    if (af->buffer != 0) {
                        hj = af->buffer;
                        hz = (char *)modelBlob + p;
                        hj->part = Lmd_BuildAndRegisterPart(battleLoadDesc, (int)hz, timObject, loadOptions, bb, baseName);
                    }
                }
            }
            Lmd_DestroyTextureAtlas(bb);
            i6 = 0;
            Skel_BuildHierarchyAndLoadParts(i6, (PolygonSkelHierarchy *)av, (PfileLoadOptions *)loadOptions, (int)GetEngineContext());
        }
    }
    return (void *)av;
}

/*
 * Debug helper that loads and tests ARTHUR model LMD/TIM resources.
 */
// FUNCTION: FF7 0x005e89ab
void __cdecl Lmd_DebugLoadArthurTest(void)
{
    Battle3dPartyLoadDesc *loadDescPtr;
    int size;
    void *modelFile;
    struct PolygonTim *tim;
    PfileLoadOptions optBuffer;
    Battle3dPartyLoadDesc backingWords;
    void *loadOptions;
    void *modelBytes;
    char *modelName;
    void *skeleton;

    loadOptions = &optBuffer;
    Pfile_InitLoadOptions((PfileLoadOptions *)loadOptions);
    loadDescPtr = &backingWords;
    Battle_InitLmdLoadDesc((int *)loadDescPtr);
    tim = 0;
    // STRING: FF7 0x00900168
    modelName = "ARTHUR.MDL";
    modelFile = LoadDiskFile(&size, modelName);
    if (modelFile != 0) {
        modelBytes = modelFile;
        skeleton = BattleModel_BuildSkeletonFromLmd(loadDescPtr, 0, 0, (int *)modelBytes, tim, loadOptions, modelName);
        // STRING: FF7 0x00900174
        GameHeapFree(modelFile, "C:\\FF7\\src\\battle\\battle3d\\mdl.cpp", 0x14F);
    }
}
} /* extern "C" */
