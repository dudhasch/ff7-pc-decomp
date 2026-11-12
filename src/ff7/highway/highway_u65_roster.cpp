// src/ff7/highway/highway_u65_roster.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The highway model roster: it loads or shares each model from a sentinel-bounded native
//   list and frees the models it owns. A small guarded array accessor sits alongside.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/api/snobo.h"
#include "ff7/highway_palette.h"
#include "ff7/highway_psxdata.h"
#include <math.h>
#include <stdlib.h>
#include "ff7/types/highway.h"
#include "lib/enums/graphics.h"
#include "ff7/api/highway.h"
#include "lib/api/polygon.h"
#include "lib/pfile.h"
#include "lib/api/graphics.h"
#include "ff7/api/main.h"
#include "lib/api/file.h"

#pragma intrinsic(strlen, strcpy, strcat, strcmp)
extern "C" {
#pragma intrinsic(memset)
#pragma intrinsic(atan2, sqrt)

// clang-format off
// The roster of RSD model names, terminated by a null entry.
// GLOBAL: FF7 0x00909728
const char *g_HighwayModelNames[182] = {
    // STRING: FF7 0x00909a00
    "shaa.rsd",
    // STRING: FF7 0x00909a0c
    "uhaa.rsd",
    // STRING: FF7 0x00909a18
    "whaa.rsd",
    // STRING: FF7 0x00909a24
    "yhaa.rsd",
    // STRING: FF7 0x00909a30
    "aiaa.rsd",
    // STRING: FF7 0x00909a3c
    "ciaa.rsd",
    // STRING: FF7 0x00909a48
    "eiaa.rsd",
    // STRING: FF7 0x00909a54
    "giaa.rsd",
    // STRING: FF7 0x00909a60
    "iiaa.rsd",
    // STRING: FF7 0x00909a6c
    "miaa.rsd",
    // STRING: FF7 0x00909a78
    "oiaa.rsd",
    // STRING: FF7 0x00909a84
    "qiaa.rsd",
    // STRING: FF7 0x00909a90
    "siaa.rsd",
    // STRING: FF7 0x00909a9c
    "uiaa.rsd",
    // STRING: FF7 0x00909aa8
    "wiaa.rsd",
    // STRING: FF7 0x00909ab4
    "yiaa.rsd",
    // STRING: FF7 0x00909ac0
    "ajaa.rsd",
    // STRING: FF7 0x00909acc
    "cjaa.rsd",
    // STRING: FF7 0x00909ad8
    "ejaa.rsd",
    // STRING: FF7 0x00909ae4
    "gjaa.rsd",
    // STRING: FF7 0x00909af0
    "ijaa.rsd",
    // STRING: FF7 0x00909afc
    "kjaa.rsd",
    // STRING: FF7 0x00909b08
    "mjaa.rsd",
    // STRING: FF7 0x00909b14
    "ojaa.rsd",
    // STRING: FF7 0x00909b20
    "qjaa.rsd",
    // STRING: FF7 0x00909b2c
    "sjaa.rsd",
    // STRING: FF7 0x00909b38
    "ujaa.rsd",
    // STRING: FF7 0x00909b44
    "yjaa.rsd",
    // STRING: FF7 0x00909b50
    "akaa.rsd",
    // STRING: FF7 0x00909b5c
    "ckaa.rsd",
    // STRING: FF7 0x00909b68
    "ekaa.rsd",
    // STRING: FF7 0x00909b74
    "gkaa.rsd",
    // STRING: FF7 0x00909b80
    "ikaa.rsd",
    // STRING: FF7 0x00909b8c
    "kkaa.rsd",
    // STRING: FF7 0x00909b98
    "mkaa.rsd",
    // STRING: FF7 0x00909ba4
    "okaa.rsd",
    // STRING: FF7 0x00909bb0
    "qkaa.rsd",
    // STRING: FF7 0x00909bbc
    "skaa.rsd",
    // STRING: FF7 0x00909bc8
    "ukaa.rsd",
    // STRING: FF7 0x00909bd4
    "wkaa.rsd",
    // STRING: FF7 0x00909be0
    "ykaa.rsd",
    // STRING: FF7 0x00909bec
    "alaa.rsd",
    // STRING: FF7 0x00909bf8
    "claa.rsd",
    // STRING: FF7 0x00909c04
    "elaa.rsd",
    // STRING: FF7 0x00909c10
    "glaa.rsd",
    // STRING: FF7 0x00909c1c
    "ilaa.rsd",
    // STRING: FF7 0x00909c28
    "klaa.rsd",
    // STRING: FF7 0x00909c34
    "shaa.rsd",
    // STRING: FF7 0x00909c40
    "mlaa.rsd",
    // STRING: FF7 0x00909c4c
    "olaa.rsd",
    // STRING: FF7 0x00909c58
    "qlaa.rsd",
    // STRING: FF7 0x00909c64
    "slaa.rsd",
    // STRING: FF7 0x00909c70
    "ulaa.rsd",
    // STRING: FF7 0x00909c7c
    "wlaa.rsd",
    // STRING: FF7 0x00909c88
    "ylaa.rsd",
    // STRING: FF7 0x00909c94
    "amaa.rsd",
    // STRING: FF7 0x00909ca0
    "cmaa.rsd",
    // STRING: FF7 0x00909cac
    "emaa.rsd",
    // STRING: FF7 0x00909cb8
    "gmaa.rsd",
    // STRING: FF7 0x00909cc4
    "imaa.rsd",
    // STRING: FF7 0x00909cd0
    "kmaa.rsd",
    // STRING: FF7 0x00909cdc
    "mmaa.rsd",
    // STRING: FF7 0x00909ce8
    "omaa.rsd",
    // STRING: FF7 0x00909cf4
    "rmaa.rsd",
    // STRING: FF7 0x00909d00
    "tmaa.rsd",
    // STRING: FF7 0x00909d0c
    "vmaa.rsd",
    // STRING: FF7 0x00909d18
    "xmaa.rsd",
    // STRING: FF7 0x00909d24
    "zmaa.rsd",
    // STRING: FF7 0x00909d30
    "bnaa.rsd",
    // STRING: FF7 0x00909d3c
    "dnaa.rsd",
    // STRING: FF7 0x00909d48
    "fnaa.rsd",
    // STRING: FF7 0x00909d54
    "hnaa.rsd",
    // STRING: FF7 0x00909d60
    "jnaa.rsd",
    // STRING: FF7 0x00909d6c
    "lnaa.rsd",
    // STRING: FF7 0x00909d78
    "nnaa.rsd",
    // STRING: FF7 0x00909d84
    "pnaa.rsd",
    // STRING: FF7 0x00909d90
    "rnaa.rsd",
    // STRING: FF7 0x00909d9c
    "tnaa.rsd",
    // STRING: FF7 0x00909da8
    "vnaa.rsd",
    // STRING: FF7 0x00909db4
    "xnaa.rsd",
    // STRING: FF7 0x00909dc0
    "znaa.rsd",
    // STRING: FF7 0x00909dcc
    "boaa.rsd",
    // STRING: FF7 0x00909dd8
    "doaa.rsd",
    // STRING: FF7 0x00909de4
    "foaa.rsd",
    // STRING: FF7 0x00909df0
    "hoaa.rsd",
    // STRING: FF7 0x00909dfc
    "joaa.rsd",
    // STRING: FF7 0x00909e08
    "loaa.rsd",
    // STRING: FF7 0x00909e14
    "noaa.rsd",
    // STRING: FF7 0x00909e20
    "poaa.rsd",
    // STRING: FF7 0x00909e2c
    "roaa.rsd",
    // STRING: FF7 0x00909e38
    "toaa.rsd",
    // STRING: FF7 0x00909e44
    "voaa.rsd",
    // STRING: FF7 0x00909e50
    "xoaa.rsd",
    // STRING: FF7 0x00909e5c
    "zoaa.rsd",
    // STRING: FF7 0x00909e68
    "bpaa.rsd",
    // STRING: FF7 0x00909e74
    "gpaa.rsd",
    // STRING: FF7 0x00909e80
    "ipaa.rsd",
    // STRING: FF7 0x00909e8c
    "kpaa.rsd",
    // STRING: FF7 0x00909e98
    "mpaa.rsd",
    // STRING: FF7 0x00909ea4
    "opaa.rsd",
    // STRING: FF7 0x00909eb0
    "qpaa.rsd",
    // STRING: FF7 0x00909ebc
    "spaa.rsd",
    // STRING: FF7 0x00909ec8
    "upaa.rsd",
    // STRING: FF7 0x00909ed4
    "wpaa.rsd",
    // STRING: FF7 0x00909ee0
    "ypaa.rsd",
    // STRING: FF7 0x00909eec
    "aqaa.rsd",
    // STRING: FF7 0x00909ef8
    "cqaa.rsd",
    // STRING: FF7 0x00909f04
    "eqaa.rsd",
    // STRING: FF7 0x00909f10
    "gqaa.rsd",
    // STRING: FF7 0x00909f1c
    "iqaa.rsd",
    // STRING: FF7 0x00909f28
    "shaa.rsd",
    // STRING: FF7 0x00909f34
    "shaa.rsd",
    // STRING: FF7 0x00909f40
    "shaa.rsd",
    // STRING: FF7 0x00909f4c
    "shaa.rsd",
    // STRING: FF7 0x00909f58
    "shaa.rsd",
    // STRING: FF7 0x00909f64
    "shaa.rsd",
    // STRING: FF7 0x00909f70
    "shaa.rsd",
    // STRING: FF7 0x00909f7c
    "shaa.rsd",
    // STRING: FF7 0x00909f88
    "shaa.rsd",
    // STRING: FF7 0x00909f94
    "shaa.rsd",
    // STRING: FF7 0x00909fa0
    "shaa.rsd",
    // STRING: FF7 0x00909fac
    "shaa.rsd",
    // STRING: FF7 0x00909fb8
    "shaa.rsd",
    // STRING: FF7 0x00909fc4
    "shaa.rsd",
    // STRING: FF7 0x00909fd0
    "shaa.rsd",
    // STRING: FF7 0x00909fdc
    "shaa.rsd",
    // STRING: FF7 0x00909fe8
    "shaa.rsd",
    // STRING: FF7 0x00909ff4
    "shaa.rsd",
    // STRING: FF7 0x0090a000
    "shaa.rsd",
    // STRING: FF7 0x0090a00c
    "shaa.rsd",
    // STRING: FF7 0x0090a018
    "shaa.rsd",
    // STRING: FF7 0x0090a024
    "shaa.rsd",
    // STRING: FF7 0x0090a030
    "shaa.rsd",
    // STRING: FF7 0x0090a03c
    "shaa.rsd",
    // STRING: FF7 0x0090a048
    "shaa.rsd",
    // STRING: FF7 0x0090a054
    "shaa.rsd",
    // STRING: FF7 0x0090a060
    "shaa.rsd",
    // STRING: FF7 0x0090a06c
    "shaa.rsd",
    // STRING: FF7 0x0090a078
    "shaa.rsd",
    // STRING: FF7 0x0090a084
    "shaa.rsd",
    // STRING: FF7 0x0090a090
    "kqaa.rsd",
    // STRING: FF7 0x0090a09c
    "mqaa.rsd",
    // STRING: FF7 0x0090a0a8
    "oqaa.rsd",
    // STRING: FF7 0x0090a0b4
    "qqaa.rsd",
    // STRING: FF7 0x0090a0c0
    "sqaa.rsd",
    // STRING: FF7 0x0090a0cc
    "uqaa.rsd",
    // STRING: FF7 0x0090a0d8
    "wqaa.rsd",
    // STRING: FF7 0x0090a0e4
    "yqaa.rsd",
    // STRING: FF7 0x0090a0f0
    "araa.rsd",
    // STRING: FF7 0x0090a0fc
    "craa.rsd",
    // STRING: FF7 0x0090a108
    "eraa.rsd",
    // STRING: FF7 0x0090a114
    "graa.rsd",
    // STRING: FF7 0x0090a120
    "iraa.rsd",
    // STRING: FF7 0x0090a12c
    "kraa.rsd",
    // STRING: FF7 0x0090a138
    "mraa.rsd",
    // STRING: FF7 0x0090a144
    "oraa.rsd",
    // STRING: FF7 0x0090a150
    "qraa.rsd",
    // STRING: FF7 0x0090a15c
    "sraa.rsd",
    // STRING: FF7 0x0090a168
    "shaa.rsd",
    // STRING: FF7 0x0090a174
    "shaa.rsd",
    // STRING: FF7 0x0090a180
    "shaa.rsd",
    // STRING: FF7 0x0090a18c
    "shaa.rsd",
    // STRING: FF7 0x0090a198
    "uraa.rsd",
    // STRING: FF7 0x0090a1a4
    "wraa.rsd",
    // STRING: FF7 0x0090a1b0
    "zraa.rsd",
    // STRING: FF7 0x0090a1bc
    "bsaa.rsd",
    // STRING: FF7 0x0090a1c8
    "esaa.rsd",
    // STRING: FF7 0x0090a1d4
    "gsaa.rsd",
    // STRING: FF7 0x0090a1e0
    "isaa.rsd",
    // STRING: FF7 0x0090a1ec
    "ksaa.rsd",
    // STRING: FF7 0x0090a1f8
    "msaa.rsd",
    // STRING: FF7 0x0090a204
    "psaa.rsd",
    // STRING: FF7 0x0090a210
    "ssaa.rsd",
    // STRING: FF7 0x0090a21c
    "vsaa.rsd",
    // STRING: FF7 0x0090a228
    "xsaa.rsd",
    // STRING: FF7 0x0090a234
    "ataa.rsd",
    // STRING: FF7 0x0090a240
    "ctaa.rsd",
    // STRING: FF7 0x0090a24c
    "etaa.rsd",
    // STRING: FF7 0x0090a258
    "gtaa.rsd",
    // STRING: FF7 0x0090a264
    "jtaa.rsd",
    // STRING: FF7 0x0090a270
    "ntaa.rsd",
    0,
};
// GLOBAL: FF7 0x00d8d454
int g_HighwayModelCount = 0;
// clang-format on

#pragma intrinsic(abs)
} // extern "C"

/*
 * When the shared count is zero counts the null-terminated global pointer table. Allocates a dword
 * array and byte array for that count and zeroes every element of both.
 */
// FUNCTION: FF7 0x00659f50
HighwayRoster::HighwayRoster()
{
    int j;

    if (g_HighwayModelCount == 0) {
        int i = 0;
        while (g_HighwayModelNames[i] != 0) {
            i++;
        }
        g_HighwayModelCount = i;
    }
    actors = (int *)operator new(g_HighwayModelCount * 4);
    flags = (char *)operator new(g_HighwayModelCount);
    for (j = 0; j < g_HighwayModelCount; j++) {
        flags[j] = 0;
        actors[j] = 0;
    }
}

/*
 * For each shared-count entry frees its model only when both flag and actor are nonzero, then
 * clears both entries. Releases the two arrays afterward without clearing their member pointers.
 */
// FUNCTION: FF7 0x00659ff7
HighwayRoster::~HighwayRoster()
{
    int i;
    int *actorsPtr;
    char *activePtr;

    for (i = 0; i < g_HighwayModelCount; i++) {
        if (((unsigned char *)flags)[i] && actors[i]) {
            Pfile_FreeRsdModel((void *)actors[i], (void *)GetEngineContext());
        }
        actors[i] = 0;
        flags[i] = 0;
    }
    actorsPtr = actors;
    operator delete(actorsPtr);
    activePtr = flags;
    operator delete(activePtr);
}

/*
 * Loads or shares each model in the sentinel-bounded native roster.
 * Loads the highway roster models after initializing load options and changing to the minigame
 * directory. Selects scale 9 for indexes 163, 165, 167, and 174, otherwise 2, and selects blend
 * mode 1 for ten specified indexes, otherwise 6. Reuses an earlier actor pointer when filenames
 * compare equal, skipping loading and flag assignment for that entry. New loads optionally receive
 * three additional option fields and set the entry flag to one only on success. Restores the saved
 * directory afterward; failed loads and reused entries do not clear or initialize their flags.
 */
// FUNCTION: FF7 0x0065a0a2
void HighwayRoster::loadModels()
{
    struct {
        int previous;
        char directory[200];
        int unused;
        PfileLoadOptions options;
        char *context;
        int index;
        int blend;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.blend = 6;
    frame.unused = 0;
    Pfile_InitLoadOptions(&frame.options);
    frame.options.sourceMode = g_HighwayAssetSourceMode;
    frame.options.flags |= 0x20;
    frame.options.flags |= 0x8000000;
    frame.options.flags |= 0x2000000;
    frame.context = GetMinigameDirForHighway();
    ChangeDirectorySaving(frame.context, frame.directory);
    for (frame.index = 0; frame.index < g_HighwayModelCount; frame.index++) {
        if (frame.index == 163 || frame.index == 165 || frame.index == 167 || frame.index == 174) {
            frame.options.scale = 9.0f;
        } else {
            frame.options.scale = 2.0f;
        }
        if (frame.index == 153 || frame.index == 154 || frame.index == 155 || frame.index == 148 || frame.index == 149 || frame.index == 150 || frame.index == 151 || frame.index == 170 || frame.index == 171 || frame.index == 172) {
            Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, &frame.options);
        } else {
            Pfile_SetLoadOptionsBlendMode(frame.blend, &frame.options);
        }
        for (frame.previous = 0; frame.previous < frame.index; frame.previous++) {
            if (strcmp(g_HighwayModelNames[frame.index], g_HighwayModelNames[frame.previous]) == 0) {
                actors[frame.index] = actors[frame.previous];
                /* goto kept: the original jumps to a stub after the function end; the inner loop has no other way to the outer step */
                goto next_model;
            }
        }
        if (g_HighwayUseLgpArchive != 0) {
            frame.options.file.fromArchive = 1;
            frame.options.file.archiveId = 8;
            frame.options.file.callback = 0;
        }
        actors[frame.index] = LoadRsdModel(&frame.options, g_HighwayModelNames[frame.index], (void *)((int)GetEngineContext()));
        if (actors[frame.index] != 0) {
            flags[frame.index] = 1;
        }
    next_model:;
    }
    ChangeDirectorySaving(frame.directory, 0);
}

/*
 * Resolves a guarded index from the receiver's element pointer.
 * Returns zero when elements is null or index is at least g_HighwayModelCount; otherwise returns
 * elements[index]. Negative indices are not rejected.
 */
// FUNCTION: FF7 0x0065a364
int HighwayArray::get(int index)
{
    if (!elements || index >= g_HighwayModelCount) {
        return 0;
    }
    return elements[index];
}
