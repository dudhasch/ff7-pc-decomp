// src/ff7/src/battle/battle3d/enemy.cpp
// Original: C:\FF7\src\battle\battle3d\enemy.cpp
// Contents: Loads a battle enemy from its legacy DAT file: unpacks and converts the file, then
//   loads the enemy's skeleton, textures, animations and other parts.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/file.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "lib/types/polygon.h"
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include <stdio.h>
extern "C" {

struct Battle3dDescriptorFile;

/*
 * Unpacks and converts an enemy DAT file, loading its skeleton, textures,
 * animations, and weapon models into the battle model system.
 */
// FUNCTION: FF7 0x005e3900
void *__cdecl BattleModel_LoadEnemyDatLegacy(Battle3dPartyLoadDesc *battleLoadDesc, void *modelCtx, char *datName)
{
    /* These deliberately bucket-0 names are declared in reverse stack-slot order: VC5 emits
       locals sharing a hash bucket in reverse declaration order. */
    Battle3dEnemyModel *sm;
    void *rq;
    int ra;
    int r1;
    Battle3dDescriptorFile *qu;
    void *qe;
    int q5;
    int py;
    Battle3dDescriptorFile *pi;
    void *p9;
    void *on;
    void *nr;
    int nb;
    int n2;
    int mv;
    int mf;
    void *m6;
    int lz;
    int lj;
    int kn;
    int jr;
    int jb;
    int j2;
    char iv[0xC8];
    int i6;
    int hz;
    int hj;
    unsigned int *gn;
    char fr[0xC8];
    char fb[0xC8];
    int f2;
    int ev;
    int ef;
    unsigned int *e6;
    unsigned int dz;
    int dj;
    int cn;
    unsigned int br;
    void *bb;
    Battle3dEnemyModelBank *b2;
    void *av;
    int af;
    void *a6;
    char p[0xCC];

    ef = 0;
    av = 0;
    b2 = 0;
    Battle_PushDataDirectory((int *)p);
    f2 = 1;
    bb = LoadDiskFile(&cn, datName);
    if (bb != 0) {
        a6 = bb;
        e6 = (unsigned int *)a6;
        hj = *e6;
        e6++;
        gn = e6;
        if (DEBUG_TRACE) {
            // STRING: FF7 0x008ffc00
            sprintf(fb, "%s  TOTAL DATA %d \n", datName, hj);
            Gfx_DebugPrint(fb);
        }
        if (DEBUG_TRACE) {
            for (j2 = 0; j2 < hj; j2++) {
                // STRING: FF7 0x008ffc14
                sprintf(fb, "OFFSET %d %d \n", j2, gn[j2]);
                Gfx_DebugPrint(fb);
            }
        }
        e6 += hj;
        if (battleLoadDesc->enabled != 0) {
            i6 = 4;
            f2 = 0;
        } else {
            i6 = 1;
            StripFileExtension(datName, iv);
            // STRING: FF7 0x008ffc24
            sprintf(fr, "%s.TIM", iv);
            jr = hj - 1;
            jb = cn - gn[jr] * i6;
            WriteWholeFile(jb, fr, (char *)a6 + gn[jr]);
            av = ((void * (__cdecl *)(char *fileName))Pfile_LoadTim)(fr); /* reads eax after a void call, as the original does */
            if (av != 0) {
                f2 = 0;
            }
        }
        if (f2 == 0) {
            if (battleLoadDesc->enabled != 0) {
                lj = 0;
                hz = 1;
                kn = 0;
                lz = hz;
                mf = 0;
            } else {
                hz = hj - 3;
                lj = 0;
                if (hz == 0x21) {
                    lj = 1;
                    if (battleLoadDesc->baseIndex != 0) {
                        lj = 0;
                    }
                }
                if (lj != 0) {
                    kn = 1;
                    lz = 0x10;
                    mf = 0x10;
                } else {
                    kn = 0;
                    lz = hz;
                    mf = 0;
                }
                if (DEBUG_TRACE) {
                    // STRING: FF7 0x008ffc2c
                    sprintf(fb, "TOTAL ANIMATIONS %d WEAPONS %d \n", lz, kn);
                    Gfx_DebugPrint(fb);
                }
            }
            b2 = (Battle3dEnemyModelBank *)BattleModel_AllocBank(lz + mf, kn);
            if (b2 != 0) {
                if (battleLoadDesc->secondCount != 0) {
                    nb = (int)BattleModel_OpenAnimFileWithHeader(0, lz + mf, datName);
                }
                nr = (char *)a6 + gn[0] * i6;
                battleLoadDesc->loaded = 0;
                m6 = BattleModel_BuildSkeletonFromLmd(
                    (Battle3dPartyLoadDesc *)battleLoadDesc, battleLoadDesc->useFaceTexturePage,
                    battleLoadDesc->loadKind, (int *)nr, (PolygonTim *)av, modelCtx, datName);
                b2->header = m6;
                dj = battleLoadDesc->subCount;
                if (battleLoadDesc->enabled != 0) {
                    mv = 1;
                    n2 = lz + mv;
                } else {
                    on = (char *)a6 + gn[1] * i6;
                    af = gn[2] * i6 - gn[1] * i6;
                    *(void **)b2 = GameHeapMemDup(af, on);
                    if (battleLoadDesc->secondCount != 0) {
                        BattleModel_WriteInfoFile(af, *(void **)b2, datName);
                    }
                    mv = 2;
                    n2 = lz + 2;
                }
                if (m6 != 0) {
                    for (j2 = mv; j2 < n2; j2++) {
                        q5 = gn[j2] * i6;
                        if (j2 + 1 < hj) {
                            dz = gn[j2 + 1] * i6;
                        } else {
                            dz = cn;
                        }
                        af = dz - q5;
                        pi = (Battle3dDescriptorFile *)((char *)a6 + q5);
                        if (battleLoadDesc->firstCount != 0) {
                            py = 1;
                        } else {
                            py = *(int *)((char *)m6 + 0x10) + 1;
                        }
                        ev = j2 - mv;
                        p9 = (void *)BattleModel_ConvertPackedAnimation(&br, battleLoadDesc->packedAnim, py,
                                                                af, pi, ef, ev, datName);
                        if (p9 != 0) {
                            *((void **)b2->partTable + ev) = p9;
                            if (battleLoadDesc->secondCount != 0) {
                                Battle_WriteAnimDumpRecord(br, py, af, pi, nb);
                            }
                        }
                    }
                    for (j2 = lz + 2; j2 < lz + mf + 2; j2++) {
                        ra = gn[j2] * i6;
                        if (j2 + 1 < hj) {
                            dz = gn[j2 + 1] * i6;
                        } else {
                            dz = cn;
                        }
                        af = dz - ra;
                        qu = (Battle3dDescriptorFile *)((char *)a6 + ra);
                        r1 = 1;
                        ev = j2 - 2;
                        qe = (void *)BattleModel_ConvertPackedAnimation(&br, battleLoadDesc->packedAnim, r1,
                                                                af, qu, ef, ev, datName);
                        if (qe != 0) {
                            *((void **)b2->partTable + ev) = qe;
                            if (battleLoadDesc->secondCount != 0) {
                                Battle_WriteAnimDumpRecord(br, r1, af, qu, nb);
                            }
                        }
                    }
                    if (kn > 0) {
                        j2 = hj - 2;
                        rq = (char *)a6 + gn[j2] * i6;
                        battleLoadDesc->loaded = 1;
                        battleLoadDesc->nameId = 0;
                        m6 = BattleModel_BuildSkeletonFromLmd(
                            (Battle3dPartyLoadDesc *)battleLoadDesc, battleLoadDesc->useFaceTexturePage,
                            battleLoadDesc->loadKind, (int *)rq, (PolygonTim *)av, modelCtx,
                            datName);
                        if (m6 != 0) {
                            **(void ***)&b2->weaponSkeletons = m6;
                        }
                    }
                    if (battleLoadDesc->secondCount != 0) {
                        sm = (Battle3dEnemyModel *)((void *)BattleModel_AllocDescriptor(
                            0, *(int *)((char *)b2->header + 0x10)));
                        if (sm != 0) {
                            if (battleLoadDesc->enabled != 0) {
                                sm->isActive = 0;
                            } else {
                                sm->isActive = 1;
                            }
                            sm->enabled = 1;
                            sm->entryCount =
                                *(int *)((char *)b2->header + 0x10);
                            sm->reservedWordA = 0;
                            sm->textureCount = dj;
                            sm->animCount = lz;
                            sm->weaponAnimStart = lz + 2;
                            sm->weaponModelCount = kn;
                            sm->weaponAnimCount = mf;
                            sm->reservedWordB = 0;
                            BattleModel_GatherSkeletonBoneVectors((struct Battle3dBoneVector *)sm->boneVectors,
                                                                  (Battle3dSkeleton *)(*(int *)&b2->header));
                            BattleModel_WriteDescriptorFile((Battle3dDescriptorFile *)sm, datName);
                            BattleModel_FreeDescriptorWithBones(sm);
                        }
                    }
                }
                if (battleLoadDesc->secondCount != 0) {
                    BattleModel_CloseAnimFile(nb);
                }
            }
            Pfile_DestroyTim((struct PolygonTim *)av);
        }
        // STRING: FF7 0x008ffc50
        GameHeapFree(bb, "C:\\FF7\\src\\battle\\battle3d\\enemy.cpp", 0x1A2);
    }
    Battle_PopDataDirectory((int *)p);
    return (void *)b2;
}
} /* extern "C" */
