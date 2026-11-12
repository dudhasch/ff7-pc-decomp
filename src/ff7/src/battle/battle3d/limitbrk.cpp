// src/ff7/src/battle/battle3d/limitbrk.cpp
// Original: C:\FF7\src\battle\battle3d\limitbrk.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/crt.h"
#include "lib/api/file.h"
#include "lib/api/polygon.h"
#include "ff7/types/battle.h"
#include "lib/types/polygon.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
extern "C" {

struct Battle3dDescriptorFile;

/*
 * Frees the animation array of bank: each of its entryCount animations through Anim_Free, then
 * the array itself, with the count zeroed and the pointer nulled. A null bank, or one with no
 * array, is accepted.
 */
// FUNCTION: FF7 0x005e2a10
void __cdecl Battle_FreeLimitAnims(BattleLimitAnimBank *bank)
{
    int i;

    if (bank != 0 && bank->entryPtrs != 0) {
        for (i = 0; i < bank->entryCount; i++) {
            Anim_Free(((PolygonAnimBuffers **)bank->entryPtrs)[i]);
        }
        bank->entryCount = 0;
        // STRING: FF7 0x008ffa50
        GameHeapFree(bank->entryPtrs, "C:\\FF7\\src\\battle\\battle3d\\limitbrk.cpp", 0x2D);
        bank->entryPtrs = 0;
    }
}

/*
 * Converts legacy character limit break animation records into packed runtime format.
 */
// FUNCTION: FF7 0x005e2a85
void __cdecl Battle_ConvertLimitAnimsLegacy(int unused, Battle3dPartyLoadDesc *battleLoadDesc, BattleLimitAnimBank *bank, char *charName)
{
    /* These deliberately bucket-0 names are declared in reverse stack-slot order: VC5 emits
       locals sharing a hash bucket in reverse declaration order. */
    int lz;
    Battle3dDescriptorFile *lj;
    void *kn;
    int jr;
    Battle3dDescriptorFile *jb;
    void *j2;
    int iv;
    int i6;
    int hz;
    int hj;
    int gn;
    Battle3dSkeleton *fr;
    unsigned int fb;
    int f2;
    unsigned int *ev;
    int ef;
    int e6;
    unsigned int *dz;
    unsigned int dj;
    char cn[0xC8];
    int br;
    int bb;
    void *b2;
    char av[0xC8];
    int af;
    void *a6;
    char p[0xCC];

    e6 = 0;
    Battle_PushDataDirectory((int *)p);
    // STRING: FF7 0x008ffa78
    sprintf(cn, "LIMIT/%s", charName);
    b2 = LoadDiskFile(&br, cn);
    if (b2 != 0) {
        fr = (Battle3dSkeleton *)bank->skeleton;
        a6 = b2;
        dz = (unsigned int *)a6;
        f2 = *dz;
        dz++;
        ev = dz;
        if (f2 < 0x10) {
            // STRING: FF7 0x008ffa84
            sprintf(av, "ERROR: %s TOTAL LIMIT DATA %d < 16 \n", cn, f2);
            Gfx_DebugPrint(av);
        } else {
            hj = 8;
            i6 = 8;
            hz = 8;
            if (bank->entryPtrs != 0) {
                Battle_FreeLimitAnims(bank);
            }
            bank->entryCount = hj + i6;
            // STRING: FF7 0x008ffaac
            bank->entryPtrs = GameHeapAlloc(bank->entryCount, 4, "C:\\FF7\\src\\battle\\battle3d\\limitbrk.cpp", 0x76);
            if (bank->entryPtrs != 0) {
                if (battleLoadDesc->secondCount != 0) {
                    iv = (int)BattleModel_OpenAnimFileWithHeader(0, bank->entryCount, cn);
                }
                for (gn = 0; gn < hj; gn++) {
                    fb = ev[gn];
                    if (gn + 1 < f2) {
                        dj = ev[gn + 1];
                    } else {
                        dj = br;
                    }
                    af = dj - fb;
                    jb = (Battle3dDescriptorFile *)((char *)a6 + fb);
                    jr = fr->boneCount + 1;
                    ef = gn;
                    j2 = (void *)BattleModel_ConvertPackedAnimation(&bb, battleLoadDesc->packedAnim, jr, af, jb, e6, ef, cn);
                    if (j2 != 0) {
                        *((void **)bank->entryPtrs + ef) = j2;
                        if (battleLoadDesc->secondCount != 0) {
                            Battle_WriteAnimDumpRecord(bb, jr, af, jb, iv);
                        }
                    }
                }
                for (gn = hz; gn < i6 + hz; gn++) {
                    fb = ev[gn];
                    if (gn + 1 < f2) {
                        dj = ev[gn + 1];
                    } else {
                        dj = br;
                    }
                    af = dj - fb;
                    lj = (Battle3dDescriptorFile *)((char *)a6 + fb);
                    lz = 1;
                    ef = gn;
                    kn = (void *)BattleModel_ConvertPackedAnimation(&bb, battleLoadDesc->packedAnim, lz, af, lj, e6, ef, cn);
                    if (kn != 0) {
                        *((void **)bank->entryPtrs + gn) = kn;
                        if (battleLoadDesc->secondCount != 0) {
                            Battle_WriteAnimDumpRecord(bb, lz, af, lj, iv);
                        }
                    }
                }
                if (battleLoadDesc->secondCount != 0) {
                    BattleModel_CloseAnimFile(iv);
                }
            }
        }
        // STRING: FF7 0x008ffad4
        GameHeapFree(b2, "C:\\FF7\\src\\battle\\battle3d\\limitbrk.cpp", 0xD7);
    }
    Battle_PopDataDirectory((int *)p);
}
} /* extern "C" */
