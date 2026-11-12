// src/ff7/src/battle/battle3d/char.cpp
// Original: C:\FF7\src\battle\battle3d\char.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "lib/enums/file.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "lib/api/file.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "ff7/types/menu.h"
#include "lib/types/file.h"
#include "lib/types/polygon.h"
#include "lib/enums/graphics.h"
#include "lib/pfile.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/crt.h"
#include <stdio.h>
#include <string.h>
extern "C" {

struct Battle3dDescriptorFile;
// clang-format off
// GLOBAL: FF7 0x00c3f390
unsigned char g_BattleModel_FlagTable[4] = {0};
unsigned char g_bssPad_00c3f394[4] = {0};
unsigned char g_bssPad_00c3f398[96] = {0};
// Pieces of the two byte tables BattleModel_LoadEnemy indexes by enemy model id: g_BattleModel_FlagTable
// (EnemyFlags<id>) and g_BattleModelEnemyBlendOverrides (EnemyBlendModes<id>), named by the first id they hold.
// GLOBAL: FF7 0x00c3f3f8
unsigned char g_BattleModel_EnemyFlags104[32] = {0};
// GLOBAL: FF7 0x00c3f418
unsigned char g_BattleModel_EnemyFlags136[76] = {0};
// GLOBAL: FF7 0x00c3f464
unsigned char g_BattleModel_EnemyFlags212[4] = {0};
#define g_BattleEnemyBlendOverrideGroup3Byte1 (*((unsigned char *)&g_BattleModel_EnemyFlags212 + 1))
#define g_BattleEnemyBlendOverrideGroup3Byte2 (*((unsigned char *)&g_BattleModel_EnemyFlags212 + 2))
unsigned char g_bssPad_00c3f468[16] = {0};
// GLOBAL: FF7 0x00c3f478
struct BattleModelEnemyFlags232 g_BattleModel_EnemyFlags232 = {0};
// GLOBAL: FF7 0x00c3f490
unsigned char g_BattleModel_EnemyFlag256 = 0;
unsigned char g_bssPad_00c3f494[4] = {0};
unsigned char g_bssPad_00c3f498[36] = {0};
// GLOBAL: FF7 0x00c3f4bc
unsigned char g_BattleModel_EnemyFlag300 = 0;
unsigned char g_bssPad_00c3f4c0[72] = {0};
// GLOBAL: FF7 0x00c3f508
BattleModelEnemyBlendOverrides g_BattleModelEnemyBlendOverrides = {0};
unsigned char g_bssPad_00c3f530[8] = {0};
// GLOBAL: FF7 0x00c3f538
struct BattleModelEnemyBlendModes48 g_BattleModel_EnemyBlendModes48 = {0};
unsigned char g_bssPad_00c3f558[16] = {0};
// GLOBAL: FF7 0x00c3f568
struct BattleEnemyBlendOverrideGroup7 g_BattleEnemyBlendOverrideGroup7 = {0};
// GLOBAL: FF7 0x00c3f588
struct BattleEnemyBlendOverrideGroup8 g_BattleEnemyBlendOverrideGroup8 = {0};
unsigned char g_bssPad_00c3f5e4[4] = {0};
unsigned char g_bssPad_00c3f5e8[28] = {0};
// GLOBAL: FF7 0x00c3f604
unsigned char g_BattleModel_EnemyBlendMode252 = 0;
unsigned char g_bssPad_00c3f608[40] = {0};
// GLOBAL: FF7 0x00c3f630
struct BattleEnemyBlendOverrideGroup9 g_BattleEnemyBlendOverrideGroup9 = {0};
// GLOBAL: FF7 0x00c3f654
unsigned char g_BattleModel_EnemyBlendMode332 = 0;
unsigned char g_bssPad_00c3f658[40] = {0};
// clang-format on
// The TU's own view of moved definitions declared with another type.

// char.cpp's .data in retail order (0x8FF2E8..0x8FFA4D): the 2-byte "D" that two functions push, the
// 23 archive-name records, then the literal stream: the 40 code literals of the functions up to the
// rename tool (0x8FF518..0x8FF8FF), the records' 23 names (0x8FF900..0x8FF9D3) and the literals of
// the functions after them. A literal returned by a static __inline function is a parse-time unit
// at the function's definition and the call expands to the plain push under /Ob1, so the
// helpers stand before the records, in retail order
// (BattleModel_RenameFilesMatchingPatternToMangledNames (0x5E22BB)'s two literals before
// BattleModel_RenameBattleFilesToMangledNames (0x5E21D7)'s six).
// GLOBAL: FF7 0x008ff2e8
char g_BattleModel_ExtD[2] = "D";
typedef struct BattleArchiveNameRecord {
    const char *name;
    int base;
    int digits;
    int digitOffset;
    int count;
    int exact;
} BattleArchiveNameRecord;
static __inline char *CharCpp_FileLine047(void)
{
    // STRING: FF7 0x008ff518
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *CharCpp_FileLine049(void)
{
    // STRING: FF7 0x008ff53c
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *CharCpp_FileLine04B(void)
{
    // STRING: FF7 0x008ff560
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *CharCpp_FileLine04D(void)
{
    // STRING: FF7 0x008ff584
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *CharCpp_FileLine056(void)
{
    // STRING: FF7 0x008ff5a8
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *CharCpp_FileLine05A(void)
{
    // STRING: FF7 0x008ff5cc
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *CharCpp_FileLine05F(void)
{
    // STRING: FF7 0x008ff5f0
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *Char_TotalDataFormat(void)
{
    // STRING: FF7 0x008ff614
    return "%s  TOTAL DATA %d \n";
}
static __inline char *Char_OffsetFormat(void)
{
    // STRING: FF7 0x008ff628
    return "OFFSET %d %d \n";
}
static __inline char *Char_TimNameFormat(void)
{
    // STRING: FF7 0x008ff638
    return "%s.TIM";
}
static __inline char *Char_TotalWeaponAnimationsFormat(void)
{
    // STRING: FF7 0x008ff640
    return "TOTAL WEAPON ANIMATIONS %d \n";
}
static __inline char *CharCpp_FileLine1CF(void)
{
    // STRING: FF7 0x008ff660
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *CharCpp_FileLine223(void)
{
    // STRING: FF7 0x008ff684
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *CharCpp_FileLine226(void)
{
    // STRING: FF7 0x008ff6a8
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *CharCpp_FileLine22E(void)
{
    // STRING: FF7 0x008ff6cc
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *CharCpp_FileLine22F(void)
{
    // STRING: FF7 0x008ff6f0
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *CharCpp_FileLine238(void)
{
    // STRING: FF7 0x008ff714
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *CharCpp_FileLine23D(void)
{
    // STRING: FF7 0x008ff738
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *CharCpp_FileLine2C2(void)
{
    // STRING: FF7 0x008ff75c
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *CharCpp_FileLine2E1(void)
{
    // STRING: FF7 0x008ff780
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *Char_ExtensionB(void)
{
    // STRING: FF7 0x008ff7a4
    return "B";
}
static __inline char *Char_LimitPathFormat(void)
{
    // STRING: FF7 0x008ff7a8
    return "LIMIT/%s";
}
static __inline char *CharCpp_FileLine376(void)
{
    // STRING: FF7 0x008ff7b4
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *Char_EnemyDatFormat(void)
{
    // STRING: FF7 0x008ff7d8
    return "ENEMY%03d.DAT";
}
static __inline char *CharCpp_FileLine478(void)
{
    // STRING: FF7 0x008ff7e8
    return "C:\\FF7\\src\\battle\\battle3d\\char.cpp";
}
static __inline char *Char_ConvertingEnemyFormat(void)
{
    // STRING: FF7 0x008ff80c
    return "CONVERTING ENEMY %d \n";
}
static __inline char *Char_FrogDatName(void)
{
    // STRING: FF7 0x008ff824
    return "FROG.DAT";
}
static __inline char *Char_ConvertingPlayerFormat(void)
{
    // STRING: FF7 0x008ff830
    return "CONVERTING PLAYER %s \n";
}
static __inline char *Char_ConvertingPlayerFormat2(void)
{
    // STRING: FF7 0x008ff848
    return "CONVERTING PLAYER %s \n";
}
static __inline char *Char_ConvertingPlayerFormat3(void)
{
    // STRING: FF7 0x008ff860
    return "CONVERTING PLAYER %s \n";
}
static __inline char *Char_ConvertingStageFormat(void)
{
    // STRING: FF7 0x008ff878
    return "CONVERTING STAGE %d \n";
}
static __inline char *Char_RenamingFormat(void)
{
    // STRING: FF7 0x008ff890
    return "RENAMING %s TO %s \n";
}
static __inline char *Char_RenamingErrorFormat(void)
{
    // STRING: FF7 0x008ff8a4
    return "ERROR: RENAMING %s TO %s \n";
}
static __inline char *Char_RenamingBattleFilesText(void)
{
    // STRING: FF7 0x008ff8c0
    return "RENAMING BATTLE FILES \n";
}
static __inline char *Char_PatternD(void)
{
    // STRING: FF7 0x008ff8d8
    return "*.D";
}
static __inline char *Char_PatternB(void)
{
    // STRING: FF7 0x008ff8dc
    return "*.B";
}
static __inline char *Char_PatternT0(void)
{
    // STRING: FF7 0x008ff8e0
    return "*.T0*";
}
static __inline char *Char_PatternP(void)
{
    // STRING: FF7 0x008ff8e8
    return "*.P*";
}
static __inline char *Char_PatternW(void)
{
    // STRING: FF7 0x008ff8f0
    return "*.W*";
}
static __inline char *Char_PatternA(void)
{
    // STRING: FF7 0x008ff8f8
    return "*.A*";
}
/* The original wrapper passes all 23 records. */
// clang-format off
// GLOBAL: FF7 0x008ff2f0
BattleArchiveNameRecord gBattleArchiveNameRecords[23] = {
    {
        // STRING: FF7 0x008ff900
        "ENEMY",
        0, 3, 5, 370, 0,
    },
    {
        // STRING: FF7 0x008ff908
        "STAGE",
        0, 2, 5, 90, 0,
    },
    {
        // STRING: FF7 0x008ff910
        "FROG",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff918
        "CLOUD",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff920
        "TIFA",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff928
        "EARITH",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff930
        "RED13",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff938
        "YUFI",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff940
        "KETCY",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff948
        "CID1",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff950
        "SEFIROS",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff958
        "BARRETT",
        0, 0, 0, 1, 1,
    },
    {
        // STRING: FF7 0x008ff960
        "BARRETT2",
        0, 0, 0, 1, 1,
    },
    {
        // STRING: FF7 0x008ff96c
        "BARRETT3",
        0, 0, 0, 1, 1,
    },
    {
        // STRING: FF7 0x008ff978
        "BARRETT4",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff984
        "VINSENT",
        0, 0, 0, 1, 1,
    },
    {
        // STRING: FF7 0x008ff98c
        "VINSENT2",
        0, 0, 0, 1, 1,
    },
    {
        // STRING: FF7 0x008ff998
        "VINSENT3",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff9a4
        "HICLOUD",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff9ac
        "GALL",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff9b4
        "DEATHGIG",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff9c0
        "HELLMASK",
        0, 0, 0, 1, 0,
    },
    {
        // STRING: FF7 0x008ff9cc
        "CHAOS",
        0, 0, 0, 1, 0,
    },
};
// clang-format on

// Frees a battle model bank in dependency order. The limit-break animations go first, then each
// sub-skeleton in skelPtrs, then each animation in animPtrs, then the root skeleton. Finally the
// heapBlock, skelPtrs and animPtrs arrays and the bank itself are freed.
// FUNCTION: FF7 0x005e0200
void __cdecl BattleModel_FreeBank(BattleLimitAnimBank *bank)
{
    int i;

    if (bank != 0) {
        Battle_FreeLimitAnims(bank);
        for (i = 0; i < bank->skelCount; i++) {
            Skel_Free((PolygonSkelHierarchy *)((int *)bank->skelPtrs)[i]);
        }
        for (i = 0; i < bank->animCount; i++) {
            Anim_Free((PolygonAnimBuffers *)((int *)bank->animPtrs)[i]);
        }
        Skel_Free((PolygonSkelHierarchy *)((*(int *)&bank->skeleton)));
        GameHeapFree((void *)bank->heapBlock, CharCpp_FileLine047(), 0x47);
        GameHeapFree((void *)bank->skelPtrs, CharCpp_FileLine049(), 0x49);
        GameHeapFree((void *)bank->animPtrs, CharCpp_FileLine04B(), 0x4B);
        GameHeapFree(bank, CharCpp_FileLine04D(), 0x4D);
    }
}

/*
 * Allocates a battle model bank structure with animation and weapon pointer arrays.
 */
// FUNCTION: FF7 0x005e02e9
void *__cdecl BattleModel_AllocBank(int animCount, int weaponCount)
{
    BattleModelBank *rec;

    rec = (BattleModelBank *)(GameHeapAlloc(1, 148, CharCpp_FileLine056(), 0x56));
    if (rec != 0) {
        rec->animCount = animCount;
        rec->anims = GameHeapAlloc(animCount, 4, CharCpp_FileLine05A(), 0x5A);
        rec->weaponCount = weaponCount;
        if (weaponCount > 0) {
            rec->weaponSkeletons = GameHeapAlloc(weaponCount, 4, CharCpp_FileLine05F(), 0x5F);
        }
        AnimPlacement_Init((PolygonAnimPlacement *)&rec->drawState);
    }
    return (void *)rec;
}

/*
 * Loads party member battle models and animations from legacy DAT format.
 */
// FUNCTION: FF7 0x005e0370
void *__cdecl BattleModel_LoadPartyDatLegacy(Battle3dPartyLoadDesc *battleLoadDesc, void *modelCtx, char *datName)
{
    /* These deliberately bucket-0 names are declared in reverse stack-slot order: VC5 emits
       locals sharing a hash bucket in reverse declaration order. */
    Battle3dEnemyModel *qu;
    void *qe;
    int q5;
    Battle3dDescriptorFile *py;
    void *pi;
    int p9;
    Battle3dDescriptorFile *on;
    void *nr;
    int nb;
    int n2;
    int mv;
    void *mf;
    void *m6;
    int lz;
    int lj;
    void *kn;
    int jr;
    int jb;
    int j2;
    int iv;
    char i6[200];
    int hz;
    unsigned int hj;
    int gn;
    unsigned int *fr;
    char fb[200];
    char f2[200];
    int ev;
    int ef;
    unsigned int *e6;
    unsigned int dz;
    int dj;
    int cn;
    int br;
    void *bb;
    BattleLimitAnimBank *b2;
    PolygonTim *av;
    int af;
    void *a6;
    char p[204];

    ef = 0;
    av = 0;
    b2 = 0;
    Battle_PushDataDirectory((int *)p);
    bb = LoadDiskFile(&cn, datName);
    if (bb != 0) {
        a6 = bb;
        e6 = (unsigned int *)a6;
        gn = *e6;
        e6++;
        fr = e6;
        if (DEBUG_TRACE) {
            sprintf(f2, Char_TotalDataFormat(), datName, gn);
            Gfx_DebugPrint(f2);
        }
        if (DEBUG_TRACE) {
            for (iv = 0; iv < gn; iv++) {
                sprintf(f2, Char_OffsetFormat(), iv, fr[iv]);
                Gfx_DebugPrint(f2);
            }
        }
        e6 += gn;
        StripFileExtension(datName, i6);
        sprintf(fb, Char_TimNameFormat(), i6);
        if (battleLoadDesc->loadValue != 0) {
            av = 0;
        } else {
            jb = gn - 0x11;
            j2 = fr[jb + 1] - fr[jb];
            WriteWholeFile(j2, fb, (char *)a6 + fr[jb]);
            av = (PolygonTim *)(((void * (__cdecl *)(char *fileName))Pfile_LoadTim)(fb)); /* reads eax after a void call, as the original does */
        }
        if (battleLoadDesc->loadValue != 0 ||
            (battleLoadDesc->loadValue == 0 && av != 0)) {
            if (battleLoadDesc->auxValue != 0) {
                hz = gn - 0x13;
                jr = 0;
                lj = hz;
            } else if (battleLoadDesc->loadValue != 0) {
                hz = gn - 2;
                jr = 0;
                lj = hz;
            } else {
                hz = gn - 0x13;
                jr = 0x10;
                lj = hz;
            }
            b2 = (BattleLimitAnimBank *)BattleModel_AllocBank(lj, jr);
            if (b2 != 0) {
                if (battleLoadDesc->secondCount != 0) {
                    lz = (int)BattleModel_OpenAnimFileWithHeader(0, lj, datName);
                }
                m6 = (char *)a6 + fr[0];
                battleLoadDesc->loaded = 0;
                kn = BattleModel_BuildSkeletonFromLmd(battleLoadDesc,
                                                      battleLoadDesc->useFaceTexturePage,
                                                      battleLoadDesc->loadKind,
                                                      (int *)m6, av, modelCtx, datName);
                b2->skeleton = kn;
                dj = battleLoadDesc->subCount;
                mf = (char *)a6 + fr[1];
                af = fr[2] - fr[1];
                *(void **)b2 = GameHeapMemDup(af, mf);
                if (battleLoadDesc->secondCount != 0) {
                    BattleModel_WriteInfoFile(af, *(void **)b2, datName);
                }
                if (kn != 0) {
                    if (battleLoadDesc->auxValue != 0) {
                        mv = lj;
                        nb = 0;
                        n2 = 0;
                    } else if (battleLoadDesc->loadValue != 0) {
                        mv = lj;
                        nb = 0;
                        n2 = 0;
                    } else {
                        mv = 0x34;
                        nb = hz - mv;
                        n2 = mv + 2;
                    }
                    if (DEBUG_TRACE) {
                        sprintf(f2, Char_TotalWeaponAnimationsFormat(), nb);
                        Gfx_DebugPrint(f2);
                    }
                    for (iv = 2; iv < mv + 2; iv++) {
                        hj = fr[iv];
                        if (iv + 1 < gn) {
                            dz = fr[iv + 1];
                        } else {
                            dz = cn;
                        }
                        af = dz - hj;
                        on = (Battle3dDescriptorFile *)((char *)a6 + hj);
                        p9 = *(int *)((char *)kn + 0x10) + 1;
                        ev = iv - 2;
                        nr = (void *)BattleModel_ConvertPackedAnimation(&br, battleLoadDesc->packedAnim, p9,
                                                                        af, on, ef, ev, datName);
                        if (nr != 0) {
                            *((void **)(*(void **)&b2->animPtrs) + ev) = nr;
                            if (iv == 2) {
                                Anim_BakeAllFrameMatrices((PolygonAnimBake *)nr, (PolygonAnimModel *)kn);
                            }
                            if (battleLoadDesc->secondCount != 0) {
                                Battle_WriteAnimDumpRecord(br, p9, af, on, lz);
                            }
                        }
                    }
                    for (iv = n2; iv < nb + n2; iv++) {
                        hj = fr[iv];
                        if (iv + 1 < gn) {
                            dz = fr[iv + 1];
                        } else {
                            dz = cn;
                        }
                        af = dz - hj;
                        py = (Battle3dDescriptorFile *)((char *)a6 + hj);
                        q5 = 1;
                        ev = iv - n2 + mv;
                        pi = (void *)BattleModel_ConvertPackedAnimation(&br, battleLoadDesc->packedAnim, q5,
                                                                        af, py, ef, ev, datName);
                        if (pi != 0) {
                            *((void **)(*(void **)&b2->animPtrs) + ev) = pi;
                            if (battleLoadDesc->secondCount != 0) {
                                Battle_WriteAnimDumpRecord(br, q5, af, py, lz);
                            }
                        }
                    }
                    if (nb > 0) {
                        Pfile_SetLoadOptionsBlendMode(BLEND_KEEP, (PfileLoadOptions *)modelCtx);
                        for (iv = gn - 0x10; iv < gn; iv++) {
                            qe = (char *)a6 + fr[iv];
                            battleLoadDesc->loaded = 1;
                            battleLoadDesc->nameId = iv - (gn - 0x10);
                            kn = BattleModel_BuildSkeletonFromLmd(
                                battleLoadDesc, battleLoadDesc->useFaceTexturePage,
                                battleLoadDesc->loadKind, (int *)qe, av, modelCtx,
                                datName);
                            if (kn != 0) {
                                *((void **)(*(void **)&b2->skelPtrs) +
                                  (iv - (gn - 0x10))) = kn;
                            }
                        }
                    }
                    if (battleLoadDesc->secondCount != 0) {
                        qu = (Battle3dEnemyModel *)((void *)BattleModel_AllocDescriptor(
                            2, *(int *)((char *)b2->skeleton + 0x10)));
                        if (qu != 0) {
                            qu->isActive = 1;
                            qu->enabled = 1;
                            qu->entryCount =
                                *(int *)((char *)b2->skeleton + 0x10);
                            qu->reservedWordA = 0;
                            qu->textureCount = dj;
                            qu->animCount = mv;
                            qu->weaponAnimStart = n2;
                            qu->weaponModelCount = jr;
                            qu->weaponAnimCount = nb;
                            qu->reservedWordB = 0;
                            BattleModel_GatherSkeletonBoneVectors((struct Battle3dBoneVector *)qu->boneVectors,
                                                                  (Battle3dSkeleton *)(*(int *)&b2->skeleton));
                            BattleModel_WriteDescriptorFile((Battle3dDescriptorFile *)qu, datName);
                            BattleModel_FreeDescriptor(1, qu);
                        }
                    }
                }
                if (battleLoadDesc->secondCount != 0) {
                    BattleModel_CloseAnimFile(lz);
                }
            }
            Pfile_DestroyTim(av);
        }
        GameHeapFree(bb, CharCpp_FileLine1CF(), 0x1CF);
    }
    Battle_PopDataDirectory((int *)p);
    return (void *)b2;
}

/*
 * Computes bone matrices and draws an animation frame for a character or weapon model bank.
 */
// FUNCTION: FF7 0x005e0d3c
void __cdecl BattleModel_DrawAnimFrame(int useWeaponBank, int animIndex, int frameIndex, BattleModelBank *bank)
{
    PolygonSkeletonAnim *ctx;

    if (bank != 0) {
        if (useWeaponBank != 0) {
            if (animIndex >= 0 && animIndex < bank->secondAnimCount) {
                ctx = ((PolygonSkeletonAnim **)bank->secondAnims)[animIndex];
                Anim_DrawSkeletonFrame(frameIndex, ctx, (PolygonBoneTransform *)&bank->drawState, (PolygonAnimDraw *)(bank->skeleton), GetEngineContext());
            }
        } else if (animIndex >= 0 && animIndex < bank->animCount) {
            ctx = ((PolygonSkeletonAnim **)bank->anims)[animIndex];
            Anim_DrawSkeletonFrame(frameIndex, ctx, (PolygonBoneTransform *)&bank->drawState, (PolygonAnimDraw *)(bank->skeleton), GetEngineContext());
        }
    }
}

// Draws one animation frame on a skeleton of the actor. A non-zero subFlag selects skelTable, with
// skelIndex checked against skelCount; zero selects subSkelTable, checked against subSkelCount.
// Either way partIndex must be below partCount and have a non-null entry in partTable, or nothing
// is drawn. The two arms repeat the same code with different tables, which is how the original
// wrote it.
// FUNCTION: FF7 0x005e0dde
void __cdecl BattleModel_DrawAnimFrameOnSubSkeleton(int subFlag, int skelIndex, int frameTime, int partIndex, Battle3dActorModel *actor)
{
    PolygonSkeletonAnim *skelAnim;

    if (actor != 0) {
        if (subFlag != 0) {
            if (skelIndex >= 0 && skelIndex < actor->skelCount) {
                skelAnim = (PolygonSkeletonAnim *)(*(int *)((char *)actor->skelTable + skelIndex * 4));
                if (partIndex < actor->partCount && actor->partTable != 0 && ((int *)(actor->partTable))[partIndex] != 0) {
                    Anim_DrawSkeletonFrame(frameTime, skelAnim, (PolygonBoneTransform *)((int)actor + 0x20), (PolygonAnimDraw *)(((int *)(actor->partTable))[partIndex]), GetEngineContext());
                }
            }
        } else if (skelIndex >= 0 && skelIndex < actor->subSkelCount) {
            skelAnim = (PolygonSkeletonAnim *)(*(int *)((char *)actor->subSkelTable + skelIndex * 4));
            if (partIndex < actor->partCount && actor->partTable != 0 && ((int *)(actor->partTable))[partIndex] != 0) {
                Anim_DrawSkeletonFrame(frameTime, skelAnim, (PolygonBoneTransform *)((int)actor + 0x20), (PolygonAnimDraw *)(((int *)(actor->partTable))[partIndex]), GetEngineContext());
            }
        }
    }
}

/*
 * MakeIndexedAssetName's first argument is the letter the index is
 * rendered with: 0x57 'W', 0x54 'T', 0x50 'P', 0x41 'A'.
 */
// FUNCTION: FF7 0x005e0ed2
void BattleModel_MakeWeaponNameCopy(int index, char *modelName, char *outPath)
{
    MakeIndexedAssetName(0x57, index, modelName, outPath);
}

/*
 * Frees a battle character model record, and its sub-buffer when freeSub is set.
 */
// FUNCTION: FF7 0x005e0eed
void __cdecl BattleModel_FreeDescriptor(int freeSub, void *model)
{
    if (model != 0) {
        if (freeSub != 0) {
            GameHeapFree(*(void **)((char *)model + 0x30), CharCpp_FileLine223(),
                         0x223);
        }
        GameHeapFree(model, CharCpp_FileLine226(), 0x226);
    }
}

/*
 * Two copies of the same __FILE__ string: VC5 does not pool
 * duplicate literals without /Gf, and the target has one at
 * 0x008FF6CC and another at 0x008FF6F0.
 */
// FUNCTION: FF7 0x005e0f2d
void __cdecl BattleModel_FreeDescriptorWithBones(void *model)
{
    if (model != 0) {
        GameHeapFree(*(void **)((char *)model + 0x30), CharCpp_FileLine22E(), 0x22e);
        GameHeapFree(model, CharCpp_FileLine22F(), 0x22f);
    }
}

// Allocates a 0x34-byte model descriptor and stores id in modelId and count in entryCount. It also
// allocates a count-element array of 0xC-byte records into boneVectors. Returns 0 if the descriptor
// allocation failed. A failed array allocation is not checked, so boneVectors can be null on
// success (original defect).
// FUNCTION: FF7 0x005e0f67
int __cdecl BattleModel_AllocDescriptor(int id, int count)
{
    Battle3dEnemyModel *descriptor;

    descriptor = (Battle3dEnemyModel *)(GameHeapAlloc(1, 52, CharCpp_FileLine238(), 0x238));
    if (descriptor != 0) {
        descriptor->modelId = id;
        descriptor->entryCount = count;
        // BUG: this allocation is not tested; when it fails the descriptor is still returned,
        // with a null boneVectors array.
        descriptor->boneVectors = (int)GameHeapAlloc(count, 12, CharCpp_FileLine23D(), 0x23D);
    }
    return (int)descriptor;
}

/*
 * Loads a battle model descriptor (.B file) containing skeleton hierarchy and bone definitions.
 */
// FUNCTION: FF7 0x005e0fc0
void *__cdecl BattleModel_LoadDescriptor(int useCallerCtx, void *fileCtx, char *modelName)
{
    char assetName[200];
    int size;
    char dirState[204];
    Battle3dModelDescriptor *descriptor;

    if (useCallerCtx == 0) {
        Battle_PushDataDirectory((int *)dirState);
    } else {
        BattleFx_PushResourceDirectory(fileCtx, (int *)dirState);
    }
    MakeExtensionAssetName(g_BattleModel_ExtD, modelName, assetName);
    descriptor = (Battle3dModelDescriptor *)((void *)LoadWholeFile(fileCtx, &size, assetName));
    if (descriptor != 0) {
        if ((unsigned int)size >= 52) {
            descriptor->entries = 0;
            if (descriptor->entryCount > 0) {
                *(void **)&descriptor->entries = (char *)descriptor + 0x34;
            }
        } else {
            BattleModel_FreeDescriptor(0, descriptor);
            descriptor = 0;
        }
    }
    if (useCallerCtx == 0) {
        Battle_PopDataDirectory((int *)dirState);
    } else {
        BattleFx_PopResourceDirectory(fileCtx, (int *)dirState);
    }
    return (void *)descriptor;
}

/*
 * Writes a battle model descriptor structure to a .B file on disk.
 */
// FUNCTION: FF7 0x005e10b9
void BattleModel_WriteDescriptorFile(Battle3dDescriptorFile *descriptor, char *modelName)
{
    char fileName[200];
    FileHandle *fileHandle;

    if (descriptor != 0) {
        MakeExtensionAssetName(g_BattleModel_ExtD, modelName, fileName);
        fileHandle = (FileHandle *)OpenFileSimple(FILE_OPEN_CREATE, fileName);
        if (fileHandle != 0) {
            WriteFileData(52, descriptor, fileHandle);
            if (descriptor->entryCount > 0) {
                WriteFileData(descriptor->entryCount * 12, descriptor->entries, fileHandle);
            }
            CloseFileEx(fileHandle);
        }
    }
}

// Gathers one vector per bone from a skeleton into the packed array at dst, which has a 0xC-byte
// stride. The vectorX, vectorY and vectorZ of each of the boneCount bones in skeleton->bones are
// copied out in turn. Nothing is written when skeleton is null.
// FUNCTION: FF7 0x005e1155
void __cdecl BattleModel_GatherSkeletonBoneVectors(Battle3dBoneVector *dst, Battle3dSkeleton *skeleton)
{
    int i;

    if (skeleton != 0) {
        for (i = 0; i < skeleton->boneCount; i++) {
            dst->vectorX = ((Battle3dSkeletonBone *)(skeleton->bones))[i].vectorX;
            dst->vectorY = ((Battle3dSkeletonBone *)(skeleton->bones))[i].vectorY;
            dst->vectorZ = ((Battle3dSkeletonBone *)(skeleton->bones))[i].vectorZ;
            dst++;
        }
    }
}

// Creates a skeleton of boneCount bones, or one bone when boneCount is not positive, and fills its
// bone array (parts). A multi-bone model takes each bone's parent, length and part count from the
// 0xC-byte-stride boneTable. A bone with a positive part count gets an 8-byte record at animation
// whose second word holds its mesh, loaded from the model's 'P' part file. The single bone gets
// parent -1, length 0 and one part, and its mesh comes from the 'W' weapon file when isWeapon is
// set, otherwise from the 'P' part file. The skeleton goes to Skel_BuildHierarchyAndLoadParts and
// is returned.
// FUNCTION: FF7 0x005e11cc
void *__cdecl BattleModel_BuildSkeleton(int isWeapon, int weaponIndex, int boneCount, int *boneTable, struct Battle3dPartyLoadDesc *descriptor, Battle3dPartyLoadDesc *battleLoadDesc, PfileLoadOptions *pfileLoadOptions, char *modelName)
{
    Battle3dModelBone *bone;
    PolygonSkelHierarchy *skel;
    int boneIndex;
    char nm[200];
    PolygonFileArrays *pWeaponFile;
    int anm;
    int wanm;
    PolygonFileArrays *obj;

    if (boneCount > 0) {
        skel = (PolygonSkelHierarchy *)Skel_Create(boneCount);
    } else {
        skel = (PolygonSkelHierarchy *)Skel_Create(boneCount + 1);
    }
    if (skel != 0) {
        bone = (Battle3dModelBone *)skel->parts;
        if (boneCount > 0) {
            if (bone != 0) {
                for (boneIndex = 0; boneIndex < boneCount; boneIndex++) {
                    bone->name = 0;
                    bone->parentName = 0;
                    bone->parentIndex = boneTable[0];
                    bone->length = boneTable[1];
                    bone->partCount = boneTable[2];
                    if (boneTable[2] > 0) {
                        bone->animation = (int)GameHeapAlloc(1, 8, CharCpp_FileLine2C2(), 0x2C2);
                        if (bone->animation != 0) {
                            anm = bone->animation;
                            BattleModel_MakePartName(boneIndex, modelName, nm);
                            obj = Pfile_LoadBinary(&pfileLoadOptions->file, 0, (int)nm);
                            *(int *)((char *)anm + 4) = (int)BattleModel_LoadPartMesh(battleLoadDesc, descriptor->partIndex, (int)modelName, obj, 0, pfileLoadOptions, 0, GetEngineContext());
                        }
                    }
                    boneTable += 3;
                    bone++;
                }
            }
        } else {
            bone->name = 0;
            bone->parentName = 0;
            bone->parentIndex = -1;
            bone->length = 0;
            bone->partCount = 1;
            bone->animation = (int)GameHeapAlloc(1, 8, CharCpp_FileLine2E1(), 0x2E1);
            if (bone->animation != 0) {
                if (isWeapon != 0) {
                    BattleModel_MakeWeaponName(weaponIndex, modelName, nm);
                } else {
                    BattleModel_MakePartName(0, modelName, nm);
                }
                pWeaponFile = Pfile_LoadBinary(&pfileLoadOptions->file, 0, (int)nm);
                wanm = bone->animation;
                *(int *)((char *)wanm + 4) = (int)BattleModel_LoadPartMesh(battleLoadDesc, descriptor->partIndex, (int)modelName, pWeaponFile, 0, pfileLoadOptions, 0, GetEngineContext());
            }
        }
        Skel_BuildHierarchyAndLoadParts(0, skel, pfileLoadOptions, (int)GetEngineContext());
    }
    return (void *)skel;
}

/*
 * Builds a battle model's info file name: the model name with the 'B' extension.
 */
// FUNCTION: FF7 0x005e1449
void __cdecl BattleModel_MakeInfoName(char *modelName, char *outPath)
{
    MakeExtensionAssetName(Char_ExtensionB(), modelName, outPath);
}

/*
 * Writes `size` bytes of `data` into the model's info file (name from BattleModel_MakeInfoName).
 */
// FUNCTION: FF7 0x005e1463
void __cdecl BattleModel_WriteInfoFile(int size, void *data, char *modelName)
{
    char name[200];

    BattleModel_MakeInfoName(modelName, name);
    WriteWholeFile(size, name, data);
}

/*
 * Builds a battle model bank for one character: pushes either the battle data directory
 * (useCallerCtx 0) or the file context's resource directory, and allocates a bank sized
 * firstCount plus secondCount animations by enabled weapons from the descriptor. It fills fileData
 * with the info file's contents when loadKind is set, skeleton with the root skeleton when
 * packedAnim is set, and the weaponIndex entry of weaponSkeletons with a weapon skeleton, then
 * loads the whole animation array through BattleModel_LoadAnimBank. The two per-animation loops
 * that would fill it one entry at a time sit behind a condition that is constant zero in retail and
 * never run. Restores the directory and returns the bank.
 */
// FUNCTION: FF7 0x005e149a
void *__cdecl BattleModel_LoadBank(int useCallerCtx, int weaponIndex, Battle3dPartyLoadDesc *descriptor, struct Battle3dPartyLoadDesc *battleLoadDesc, PfileLoadOptions *pfileLoadOptions, char *modelName)
{
    int slot[51];
    int i;
    BattleModelBank *bk;
    int aa;
    int len;
    char nm[200];
    int wi;
    int flg;

    if (useCallerCtx == 0) {
        Battle_PushDataDirectory(slot);
    } else {
        BattleFx_PushResourceDirectory(&pfileLoadOptions->file, slot);
    }
    bk = 0;
    if (descriptor != 0) {
        bk = (BattleModelBank *)BattleModel_AllocBank(descriptor->firstCount + descriptor->secondCount, descriptor->enabled);
        if (bk != 0) {
            if (descriptor->loadKind != 0) {
                BattleModel_MakeInfoName(modelName, nm);
                bk->fileData = (int)LoadWholeFile(&pfileLoadOptions->file, &len, nm);
            }
            if (descriptor->packedAnim != 0) {
                (*(int *)&bk->skeleton) = (int)BattleModel_BuildSkeleton(0, 0, descriptor->boneCount, (*(int **)&descriptor->loaded), descriptor, battleLoadDesc, pfileLoadOptions, modelName);
            }
            if (weaponIndex < descriptor->enabled) {
                if (weaponIndex < descriptor->enabled) {
                    Pfile_SetLoadOptionsBlendMode(BLEND_KEEP, pfileLoadOptions);
                    *(int *)((char *)bk->weaponSkeletons + weaponIndex * 4) = (int)BattleModel_BuildSkeleton(1, weaponIndex, 0, (*(int **)&descriptor->loaded), descriptor, battleLoadDesc, pfileLoadOptions, modelName);
                }
            }
            if (DEAD_IN_RETAIL) {
                if (descriptor->firstCount > 0) {
                    for (i = 0; i < descriptor->firstCount; i++) {
                        aa = i;
                        ((int *)(*(int *)&bk->anims))[aa] = (int)BattleModel_LoadSingleAnimation(battleLoadDesc->packedAnim, aa, &pfileLoadOptions->file, modelName);
                    }
                }
                if (descriptor->secondCount > 0) {
                    for (i = 0; i < descriptor->secondCount; i++) {
                        wi = descriptor->baseIndex + i;
                        flg = 1;
                        ((int *)(*(int *)&bk->anims))[wi] = (int)BattleModel_LoadSingleAnimation(battleLoadDesc->packedAnim, wi, &pfileLoadOptions->file, modelName);
                    }
                }
            } else {
                BattleModel_LoadAnimBank(battleLoadDesc->packedAnim, 0, *(void **)&bk->anims, &pfileLoadOptions->file, modelName);
            }
        }
    }
    if (useCallerCtx == 0) {
        Battle_PopDataDirectory(slot);
    } else {
        BattleFx_PopResourceDirectory(&pfileLoadOptions->file, slot);
    }
    return (void *)bk;
}

/*
 * Loads limit break animation bank for a character into the model context.
 */
// FUNCTION: FF7 0x005e1749
void __cdecl Battle_LoadLimitAnimBank(Battle3dLimitAnimBank *bank, Battle3dPartyLoadDesc *battleLoadDesc, PfileLoadOptions *modelCtx, char *charName)
{
    char dirState[204];
    char pathName[200];

    if (bank != 0) {
        BattleFx_PushResourceDirectory(&modelCtx->file, (int *)dirState);
        sprintf(pathName, Char_LimitPathFormat(), charName);
        bank->entryCount = 16;
        bank->entries = GameHeapAlloc(bank->entryCount, 4, CharCpp_FileLine376(), 0x376);
        if (*(int *)&bank->entries != 0) {
            BattleModel_LoadAnimBank(battleLoadDesc->packedAnim, 0, bank->entries, &modelCtx->file, pathName);
        }
        BattleFx_PopResourceDirectory(&modelCtx->file, (int *)dirState);
    }
}

/*
 * Turns on the load flags a battle model needs: 0x100000, 0x2000, 0x4000, 0x800 and 0x2000000 in
 * flags, and bits 0 and 1 in paletteFlags. Written as seven separate |= statements because the original
 * is.
 */
// FUNCTION: FF7 0x005e17fe
void __cdecl BattleModel_SetLoadFlags(PfileLoadOptions *model)
{
    model->flags |= 0x100000;
    model->flags |= 0x2000;
    model->flags |= 0x4000;
    model->flags |= 0x800;
    model->flags |= 0x2000000;
    model->paletteFlags |= 1;
    model->paletteFlags |= 2;
}

/*
 * Loads an enemy's battle model: seeds the blend-override tables, configures load category 0, sets
 * the load flags, builds the file name "ENEMY%03d.DAT" from enemyModelId, and takes the enemy's
 * blend mode from the byte table g_BattleModelEnemyBlendOverrides (BLEND_KEEP when that entry is
 * zero). Sets firstCount from the byte table g_BattleModel_FlagTable and baseIndex from whether
 * the id is 0x13F, loads the legacy .DAT bank when secondCount is set or loadCategory is clear, and
 * when loadCategory is set replaces it with a descriptor-driven bank built by BattleModel_LoadBank
 * with weapon index 0. Returns the bank.
 */
// FUNCTION: FF7 0x005e1868
void *__cdecl BattleModel_LoadEnemy(int forceLegacy, int enemyModelId, Battle3dPartyLoadDesc *battleLoadDesc, PfileLoadOptions *pfileLoadOptions)
{
    BattleLimitAnimBank *result;
    char buf[0xc8];
    int weapon;
    int dscr;

    result = 0;
    BattleModel_InitEnemyBlendOverrideTables();
    BattleModel_ConfigureLoadCategory(forceLegacy, 0, battleLoadDesc, pfileLoadOptions);
    BattleModel_SetLoadFlags(pfileLoadOptions);
    sprintf(buf, Char_EnemyDatFormat(), enemyModelId);
    if (*(unsigned char *)(enemyModelId + (char *)&g_BattleModelEnemyBlendOverrides) != 0) {
        Pfile_SetLoadOptionsBlendMode(*(unsigned char *)(enemyModelId + (char *)&g_BattleModelEnemyBlendOverrides), pfileLoadOptions);
    } else {
        Pfile_SetLoadOptionsBlendMode(BLEND_KEEP, pfileLoadOptions);
    }
    if (*(unsigned char *)(enemyModelId + (char *)&g_BattleModel_FlagTable) != 0) {
        battleLoadDesc->firstCount = 1;
    } else {
        battleLoadDesc->firstCount = 0;
    }
    if (enemyModelId == 0x13F) {
        battleLoadDesc->baseIndex = 1;
    } else {
        battleLoadDesc->baseIndex = 0;
    }
    if (battleLoadDesc->secondCount != 0 || battleLoadDesc->loadCategory == 0) {
        result = (BattleLimitAnimBank *)((int)BattleModel_LoadEnemyDatLegacy(battleLoadDesc, pfileLoadOptions, buf));
    }
    if (battleLoadDesc->loadCategory != 0) {
        weapon = 0;
        dscr = (int)BattleModel_LoadDescriptor(0, &pfileLoadOptions->file, buf);
        if (dscr != 0) {
            BattleModel_FreeBank(result);
            result = (BattleLimitAnimBank *)((int)BattleModel_LoadBank(0, weapon, (Battle3dPartyLoadDesc *)dscr, battleLoadDesc, pfileLoadOptions, buf));
            BattleModel_FreeDescriptor(0, (void *)dscr);
        }
    }
    return (void *)result;
}

/*
 * Fills the enemy blend-override byte tables: mode 4 for the listed enemy ids and
 * 1 for a few.
 */
// FUNCTION: FF7 0x005e19e6
void __cdecl BattleModel_InitEnemyBlendOverrideTables(void)
{
    g_BattleModel_EnemyBlendModes48.enemyBlendOverrideGroup6Byte29 = 4;
    g_BattleModel_EnemyBlendModes48.enemyBlendOverrideGroup6Byte30 = 4;
    g_BattleEnemyBlendOverrideGroup8.enemyBlendOverrideGroup8Byte70 = 4;
    g_BattleEnemyBlendOverrideGroup8.enemyBlendOverrideGroup8Byte71 = 4;
    g_BattleModelEnemyBlendOverrides.enemyBlendOverrideGroup5Byte19 = 4;
    g_BattleModelEnemyBlendOverrides.enemyBlendOverrideGroup5Byte30 = 4;
    (g_BattleEnemyBlendOverrideGroup7.blendMode) = 4;
    g_BattleEnemyBlendOverrideGroup7.enemyBlendOverrideGroup7Byte1 = 4;
    g_BattleEnemyBlendOverrideGroup7.enemyBlendOverrideGroup7Byte2 = 4;
    g_BattleEnemyBlendOverrideGroup8.enemyBlendOverrideGroup8Byte5 = 4;
    g_BattleModelEnemyBlendOverrides.enemyBlendOverrideGroup5Byte38 = 4;
    (g_BattleModel_EnemyBlendModes48.blendMode) = 4;
    g_BattleEnemyBlendOverrideGroup7.enemyBlendOverrideGroup7Byte31 = 4;
    (g_BattleEnemyBlendOverrideGroup8.enemyBlendOverrideGroup8Byte0) = 4;
    g_BattleEnemyBlendOverrideGroup8.enemyBlendOverrideGroup8Byte10 = 1;
    g_BattleEnemyBlendOverrideGroup8.enemyBlendOverrideGroup8Byte36 = 4;
    g_BattleEnemyBlendOverrideGroup8.enemyBlendOverrideGroup8Byte82 = 4;
    g_BattleEnemyBlendOverrideGroup8.enemyBlendOverrideGroup8Byte90 = 4;
    g_BattleModel_EnemyBlendMode252 = 4;
    (g_BattleEnemyBlendOverrideGroup9.blendMode) = 4;
    g_BattleEnemyBlendOverrideGroup9.enemyBlendOverrideGroup9Byte1 = 4;
    g_BattleEnemyBlendOverrideGroup9.enemyBlendOverrideGroup9Byte2 = 4;
    g_BattleEnemyBlendOverrideGroup9.enemyBlendOverrideGroup9Byte22 = 4;
    g_BattleEnemyBlendOverrideGroup9.enemyBlendOverrideGroup9Byte35 = 4;
    g_BattleModel_EnemyBlendMode332 = 4;
    g_BattleModel_EnemyFlags104[3] = 1;
    g_BattleModel_EnemyFlags136[2] = 1;
    g_BattleEnemyBlendOverrideGroup3Byte1 = 1;
    g_BattleEnemyBlendOverrideGroup3Byte2 = 1;
    (g_BattleModel_EnemyFlags232.blendMode) = 1;
    g_BattleModel_EnemyFlags232.enemyBlendOverrideGroup4Byte23 = 1;
    g_BattleModel_EnemyFlag256 = 1;
    g_BattleModel_EnemyFlag300 = 1;
}

// Loads a party member's battle model. It configures load category 2 and sets the load flags, then
// takes the legacy .DAT path when secondCount is set or loadCategory is zero. When loadCategory is
// set, the appearance is the low four bits of paramIndex in the party stat block, or 0 when there
// is no block. A descriptor is then loaded from the model's file request, the legacy bank is
// freed, and the bank is rebuilt through BattleModel_LoadBank. Returns the bank.
// FUNCTION: FF7 0x005e1ad2
void *__cdecl BattleModel_LoadPartyMember(int category, int partyIndex, Battle3dPartyLoadDesc *loadDesc, PfileLoadOptions *model, void *context)
{
    int appearance;
    struct MenuPartyStats *statBlock;
    BattleLimitAnimBank *animationBank;
    void *descriptor;

    animationBank = 0;
    BattleModel_ConfigureLoadCategory(category, 2, loadDesc, model);
    BattleModel_SetLoadFlags(model);
    if (loadDesc->secondCount != 0 || loadDesc->loadCategory == 0) {
        animationBank = (BattleLimitAnimBank *)(BattleModel_LoadPartyDatLegacy(loadDesc, model, (char *)context));
    }
    if (loadDesc->loadCategory != 0) {
        statBlock = Savemap_GetPartyStatBlock(partyIndex);
        if (statBlock != 0) {
            appearance = statBlock->paramIndex & 0xF;
        } else {
            appearance = 0;
        }
        descriptor = BattleModel_LoadDescriptor(0, &model->file, (char *)context);
        if (descriptor != 0) {
            BattleModel_FreeBank(animationBank);
            animationBank = (BattleLimitAnimBank *)(BattleModel_LoadBank(0, appearance, (Battle3dPartyLoadDesc *)descriptor, loadDesc, model, (char *)context));
            BattleModel_FreeDescriptor(0, descriptor);
        }
    }
    return animationBank;
}

// Loads a battle stage's model set from the battle data directory. It reads the stage descriptor,
// then allocates a 0xC-byte record holding the stage ID, the stage info file's contents (fileData)
// and a Pfile model set (modelSet) sized by the descriptor's model count. Each 'P' part file is
// loaded and its mesh stored in the model set's pointer array. The descriptor is released through
// BattleModel_FreeDescriptor, with freeSub set when loadCategory is zero. Returns the record.
// forceLegacy is unused.
// FUNCTION: FF7 0x005e1bc6
void *__cdecl BattleStage_LoadModelSet(int forceLegacy, int stageId, Battle3dPartyLoadDesc *battleLoadDesc, PfileLoadOptions *pfileLoadOptions, char *stageName)
{
    int ln;
    int slot[51];
    Battle3dStageLoad *ml;
    int partIndex;
    char nm[200];
    Battle3dStageDescriptor *md;
    PolygonFileArrays *pModelFile;

    Battle_PushDataDirectory(slot);
    md = (Battle3dStageDescriptor *)((int)BattleModel_LoadDescriptor(0, &pfileLoadOptions->file, stageName));
    if (md != 0) {
        ml = (Battle3dStageLoad *)((int)GameHeapAlloc(1, 12, CharCpp_FileLine478(), 0x478));
        if (ml != 0) {
            ml->stageId = stageId;
            BattleModel_MakeInfoName(stageName, nm);
            ml->fileData = (int)LoadWholeFile(&pfileLoadOptions->file, &ln, nm);
            ml->modelSet = Pfile_AllocModelSet(md->modelCount, GetEngineContext());
            if (ml->modelSet != 0) {
                for (partIndex = 0; partIndex < md->modelCount; partIndex++) {
                    BattleModel_MakePartName(partIndex, stageName, nm);
                    pModelFile = Pfile_LoadBinary(&pfileLoadOptions->file, 0, (int)nm);
                    if (pModelFile != 0) {
                        *(int *)((char *)*(int *)((char *)ml->modelSet + 4) + partIndex * 4) = (int)BattleModel_LoadPartMesh(battleLoadDesc, md->textureCount, (int)stageName, pModelFile, 0, pfileLoadOptions, 0, GetEngineContext());
                    }
                }
            }
        }
        BattleModel_FreeDescriptor(battleLoadDesc->loadCategory == 0, md);
    }
    Battle_PopDataDirectory(slot);
    return (void *)ml;
}
int __cdecl rename(const char *, const char *);
#pragma intrinsic(strcmp, strlen)

void __cdecl BattleModel_ResolveArchiveName(int entryCount, BattleArchiveNameRecord *nameTable,
                                            char *srcName, char *outName4);

/*
 * Pushes data directory and initializes model/load descriptors. Literal enabled override narrows
 * nominal 0..369 loop to enemy 243 only. Logs, sets descriptor special flag for listed IDs, loads
 * and frees enemy bank, clears flag and restores directory.
 */
// FUNCTION: FF7 0x005e1d93
void __cdecl Battle_ConvertEnemyModelTool(void)
{
    struct {
        void *result;
        char message[200];
        int i;
        PfileLoadOptions *opts;
        int upper;
        Battle3dPartyLoadDesc desc;
        int lower;
        int directory[51];
        Battle3dPartyLoadDesc *load;
        PfileLoadOptions options;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    Battle_PushDataDirectory(f.directory);
    f.load = &f.desc;
    f.opts = &f.options;
    Battle_InitLmdLoadDesc((int *)f.load);
    Pfile_InitLoadOptions(f.opts);
    f.lower = 0;
    f.upper = 370;
    if (1) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        f.lower = 243;
        f.upper = 244;
    }
    for (f.i = f.lower; f.i < f.upper; f.i++) {
        sprintf(f.message, Char_ConvertingEnemyFormat(), f.i);
        Gfx_DebugPrint(f.message);
        if (f.i == 13 || f.i == 15 || f.i == 138 || f.i == 217 || f.i == 243)
            f.load->useFullTimBounds = 1;
        f.result = BattleModel_LoadEnemy(1, f.i, f.load, f.opts);
        f.load->useFullTimBounds = 0;
        BattleModel_FreeBank((BattleLimitAnimBank *)f.result);
    }
    Battle_PopDataDirectory(f.directory);
}

/*
 * Pushes data directory, initializes descriptors/options and converts FROG.DAT with first flag,
 * seventeen named party models with both flags zero, then four alternate models with second flag.
 * Logs and frees each loaded bank, restores directory.
 */
// FUNCTION: FF7 0x005e1ef5
void __cdecl Battle_ConvertPartyModelsTool(void)
{
    struct {
        int i;
        PfileLoadOptions *opts;
        Battle3dPartyLoadDesc desc;
        void *result;
        char message[200];
        int dir[51];
        Battle3dPartyLoadDesc *load;
        PfileLoadOptions options;
        char *name;
    } f;
    Battle_PushDataDirectory(f.dir);
    f.load = &f.desc;
    f.opts = &f.options;
    Battle_InitLmdLoadDesc((int *)f.load);
    Pfile_InitLoadOptions(f.opts);
    f.load->loadValue = 1;
    f.load->auxValue = 0;
    f.name = Char_FrogDatName();
    sprintf(f.message, Char_ConvertingPlayerFormat(), f.name);
    Gfx_DebugPrint(f.message);
    f.result = BattleModel_LoadPartyMember(1, 0, f.load, f.opts, (void *)f.name);
    BattleModel_FreeBank((BattleLimitAnimBank *)f.result);
    f.load->loadValue = 0;
    f.load->auxValue = 0;
    for (f.i = 0; f.i < 17; f.i++) {
        f.name = (char *)((int *)&g_pPartyModelFilenames)[f.i];
        sprintf(f.message, Char_ConvertingPlayerFormat2(), f.name);
        Gfx_DebugPrint(f.message);
        f.result = BattleModel_LoadPartyMember(1, 0, (Battle3dPartyLoadDesc *)f.load, f.opts, (void *)f.name);
        BattleModel_FreeBank((BattleLimitAnimBank *)f.result);
    }
    f.load->loadValue = 0;
    f.load->auxValue = 1;
    for (f.i = 0; f.i < 4; f.i++) {
        f.name = (char *)g_VincentModelFilenames[f.i];
        sprintf(f.message, Char_ConvertingPlayerFormat3(), f.name);
        Gfx_DebugPrint(f.message);
        f.result = BattleModel_LoadPartyMember(1, 0, (Battle3dPartyLoadDesc *)f.load, f.opts, (void *)f.name);
        BattleModel_FreeBank((BattleLimitAnimBank *)f.result);
    }
    Battle_PopDataDirectory(f.dir);
}


/*
 * For nonnull context saves first stage ID, frees current stage, converts/loads/frees ninety
 * stages with progress messages then reloads saved stage.
 */
// FUNCTION: FF7 0x005e2107
void __cdecl Battle3d_ConvertAllStagesToPfile(void)
{
    struct {
        char message[200];
        int *ctx;
        int i;
        int saved;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.ctx = (int *)g_pBattle3dScene;
    if (f.ctx != 0) {
        f.saved = *(int *)*f.ctx;
        BattleStage_FreeStageDatModel((BattleStageDat *)(*f.ctx));
        *f.ctx = 0;
        for (f.i = 0; f.i < 90; f.i++) {
            sprintf(f.message, Char_ConvertingStageFormat(), f.i);
            Gfx_DebugPrint(f.message);
            Battle_LoadStageModel(1, f.i, (void *)f.ctx);
            BattleStage_FreeStageDatModel((BattleStageDat *)(*f.ctx));
            *f.ctx = 0;
        }
        Battle_LoadStageModel(0, f.saved, (void *)f.ctx);
    }
}

/*
 * Empty. A model-tool hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x005e21c3
void BattleModel_UnusedFrameHook(void)
{
    /* Empty in retail. */
}

/*
 * Runs the enemy model conversion tool and its follow-up BattleModel_RenameBattleFilesToMangledNames.
 */
// FUNCTION: FF7 0x005e21c8
void __cdecl Battle_ConvertEnemyModelToolWrapper(void)
{
    Battle_ConvertEnemyModelTool();
    BattleModel_RenameBattleFilesToMangledNames();
}

/*
 * Pushes battle data directory, logs rename message, calls rename helper for six literal patterns
 * *.D, *.B, *.T0*, *.P*, *.W*, *.A* then restores directory. Helper errors ignored.
 */
// FUNCTION: FF7 0x005e21d7
void BattleModel_RenameBattleFilesToMangledNames(void)
{
    struct {
        char *pattern;
        char message[200];
        int dir[51];
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    Battle_PushDataDirectory(f.dir);
    sprintf(f.message, Char_RenamingBattleFilesText());
    Gfx_DebugPrint(f.message);

    f.pattern = Char_PatternD();
    BattleModel_RenameFilesMatchingPatternToMangledNames((void *)f.pattern);
    f.pattern = Char_PatternB();
    BattleModel_RenameFilesMatchingPatternToMangledNames((void *)f.pattern);
    f.pattern = Char_PatternT0();
    BattleModel_RenameFilesMatchingPatternToMangledNames((void *)f.pattern);
    f.pattern = Char_PatternP();
    BattleModel_RenameFilesMatchingPatternToMangledNames((void *)f.pattern);
    f.pattern = Char_PatternW();
    BattleModel_RenameFilesMatchingPatternToMangledNames((void *)f.pattern);
    f.pattern = Char_PatternA();
    BattleModel_RenameFilesMatchingPatternToMangledNames((void *)f.pattern);
    Battle_PopDataDirectory(f.dir);
}

/*
 * Obtains wildcard file list, mangles each 32-byte name, logs rename attempt and error on failure,
 * then frees list. Uses unbounded sprintf into 200-byte messages; rename results otherwise ignored.
 */
// FUNCTION: FF7 0x005e22bb
void BattleModel_RenameFilesMatchingPatternToMangledNames(void *wildcardPattern)
{
    struct {
        int i;
        int *list;
        char mangled[200];
        char message[200];
    } f;
    f.list = (int *)BuildDirectoryListing(0, (const char *)wildcardPattern);
    if (f.list != 0) {
        for (f.i = 0; f.i < f.list[0]; f.i++) {
            BattleModel_NameManglerHook((char *)f.list[1] + (f.i << 5), f.mangled);

            sprintf(f.message, Char_RenamingFormat(), (char *)f.list[1] + (f.i << 5), f.mangled);
            Gfx_DebugPrint(f.message);
            if (rename((char *)f.list[1] + (f.i << 5), f.mangled) != 0) {
                sprintf(f.message, Char_RenamingErrorFormat(), (char *)f.list[1] + (f.i << 5), f.mangled);
                Gfx_DebugPrint(f.message);
            }
        }
        FreeDirectoryListing((FileDirectoryListing *)f.list);
    }
}

/*
 * For positive count initializes first stride 0x18 record index base zero, then each base to
 * previous base plus previous offset 0x10 count. Nonpositive count writes nothing.
 */
// FUNCTION: FF7 0x005e23f6
void __cdecl BattleModel_BuildIndexBases(int count, Battle3dModelIndexRecord *records)
{
    int i;

    for (i = 0; i < count; i++) {
        if (i == 0) {
            records[i].indexBase = 0;
        } else {
            records[i].indexBase = records[i - 1].indexBase + records[i - 1].size;
        }
    }
}

/*
 * Resolves an archive name by finding a table entry whose first four source bytes match,
 * optionally requiring an exact comparison after extension removal. Initializes index bases if
 * table entry 1 has zero base, adds the configured decimal suffix to the matching base, and
 * encodes base and extension part as four letters plus a terminator. D/B/A map to parts 0/1/78;
 * T/P/W add two extension digits to bases 0/12/62. Unsupported extensions report an error but
 * still encode part zero. No match leaves output untouched.
 */
// FUNCTION: FF7 0x005e2460
void __cdecl BattleModel_ResolveArchiveName(int entryCount, BattleArchiveNameRecord *nameTable, char *srcName, char *outName4)
{
    struct {
        int dot;
        int length;
        int digit;
        int scale;
        int base;
        int number;
        int part;
        int index;
        int found;
        char text[200];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (nameTable[1].base == 0)
        BattleModel_BuildIndexBases(entryCount, (struct Battle3dModelIndexRecord *)nameTable);
    frame.found = 0;
    for (frame.index = 0; frame.index < entryCount; frame.index++) {
        if (*(int *)nameTable->name == *(int *)srcName) {
            if (nameTable->exact != 0) {
                StripFileExtension(srcName, frame.text);
                if (strcmp(nameTable->name, frame.text) == 0) {
                    frame.found = 1;
                    break;
                }
            } else {
                frame.found = 1;
                break;
            }
        }
        nameTable++;
    }
    if (frame.found != 0) {
        frame.base = nameTable->base;
        if (nameTable->digits > 0) {
            frame.number = 0;
            for (frame.index = 0; frame.index < nameTable->digits; frame.index++) {
                frame.digit = srcName[nameTable->digitOffset + nameTable->digits - (frame.index + 1)] - '0';
                if (frame.index == 0) {
                    frame.number = frame.digit;
                    frame.scale = 10;
                } else {
                    frame.number = frame.number + frame.digit * frame.scale;
                    frame.scale = frame.scale * 10;
                }
            }
            frame.base = frame.base + frame.number;
        }
        frame.part = 0;
        frame.dot = 0;
        frame.length = strlen(srcName);
        if (frame.length > 0) {
            for (frame.dot = frame.length - 1; frame.dot >= 0; frame.dot--) {
                if (srcName[frame.dot] == '.')
                    break;
            }
        }
        switch (srcName[frame.dot + 1]) {
        case 'D':
            frame.part = 0;
            break;
        case 'B':
            frame.part = 1;
            break;
        case 'T':
            frame.part = 2;
            frame.part = frame.part + (srcName[frame.dot + 2] - '0') * 10;
            frame.part = frame.part + srcName[frame.dot + 3] - '0';
            break;
        case 'P':
            frame.part = 12;
            frame.part = frame.part + (srcName[frame.dot + 2] - '0') * 10;
            frame.part = frame.part + srcName[frame.dot + 3] - '0';
            break;
        case 'W':
            frame.part = 62;
            frame.part = frame.part + (srcName[frame.dot + 2] - '0') * 10;
            frame.part = frame.part + srcName[frame.dot + 3] - '0';
            break;
        case 'A':
            frame.part = 78;
            break;
        default:
            // STRING: FF7 0x008ff9d4
            sprintf(frame.text, "ERROR: EXTENSION NOT SUPPORTED %s \n", srcName);
            Gfx_DebugPrint(frame.text);
            break;
        }
        outName4[0] = frame.base / 26 + 'A';
        outName4[1] = frame.base % 26 + 'A';
        outName4[2] = frame.part / 26 + 'A';
        outName4[3] = frame.part % 26 + 'A';
        outName4[4] = 0;
    }
}

/*
 * Hook that mangles a battle model name through BattleModel_MangleName.
 */
// FUNCTION: FF7 0x005e2908
void __cdecl BattleModel_NameManglerHook(char *name, char *out)
{
    BattleModel_MangleName(name, out);
}

/*
 * Resolves a battle model archive name with the 23-record archive-name directory.
 */
// FUNCTION: FF7 0x005e291d
void __cdecl BattleModel_MangleName(char *srcName, char *outName4)
{
    BattleModel_ResolveArchiveName(23, gBattleArchiveNameRecords, srcName, outName4);
}

/*
 * Empty. A model-tool hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x005e2939
void BattleModel_UnusedShutdownHook(void)
{
    /* Empty in retail. */
}

/*
 * Calls BattleModel_MangleName seven times into the same output rooted at a scalar local, for
 * STAGE00.D, STAGE01.B, ENEMY000.D, ENEMY001.B, TIFA.B, BARRETT2.T00 and BARRETT3.T01. Earlier
 * outputs are overwritten and none is consumed here. The helper's output extent beyond that scalar
 * and the wider inferred local storage require native/callee evidence; filenames identify
 * requested content.
 */
// FUNCTION: FF7 0x005e293e
void __cdecl BattleModel_MangleSampleFileNames(void)
{
    int mangled[50];
    char *fileName;

    // STRING: FF7 0x008ff9f8
    fileName = "STAGE00.D";
    BattleModel_MangleName(fileName, (char *)mangled);
    // STRING: FF7 0x008ffa04
    fileName = "STAGE01.B";
    BattleModel_MangleName(fileName, (char *)mangled);
    // STRING: FF7 0x008ffa10
    fileName = "ENEMY000.D";
    BattleModel_MangleName(fileName, (char *)mangled);
    // STRING: FF7 0x008ffa1c
    fileName = "ENEMY001.B";
    BattleModel_MangleName(fileName, (char *)mangled);
    // STRING: FF7 0x008ffa28
    fileName = "TIFA.B";
    BattleModel_MangleName(fileName, (char *)mangled);
    // STRING: FF7 0x008ffa30
    fileName = "BARRETT2.T00";
    BattleModel_MangleName(fileName, (char *)mangled);
    // STRING: FF7 0x008ffa40
    fileName = "BARRETT3.T01";
    BattleModel_MangleName(fileName, (char *)mangled);
}
} /* extern "C" */
