// src/ff7/src/credits/credfile.cpp
// Original: C:\FF7\src\Credits\credfile.cpp
// Contents: Credits screen resources: creating and releasing the render-object sets for the logo,
//   background and text, loading the people data file, setting the movie timing, and opening the
//   movie to play.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "ff7/api/credits.h"
#include "lib/api/polygon.h"
#include "ff7/api/main.h"
#include "lib/api/graphics.h"
#include "lib/api/movie.h"
#include "ff7/types/credits.h"
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include <math.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include "ff7/gte.h"
#include "ff7/enums/main.h"
#include <stdlib.h>
#include "lib/pfile.h"
#include "lib/crt.h"
#include "lib/api/file.h"
#include "lib/enums/file.h"

extern "C" {

#pragma intrinsic(abs)

typedef struct PfileLoadOptions PfileLoadOptions;


#pragma intrinsic(strcpy, strcat)


#pragma data_seg(".data$u77_lut")
#pragma bss_seg(".data$u77_lut")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(sqrt)

// Declarations only. g_CreditsFadeOverlayBatch is defined in
// src/ff7/src/credits/credits_store_scaled.c.

/*
 * Releases and clears five render-object handles at g_CreditsQuadBatchListD, saves the
 * asset-source mode and switches it to 0. Initializes the native contiguous
 * options block for blend 4, assigns its explicit fields including
 * g_CreditsRenderObjectHandle and option bit 0x400, and creates comesoon.tim
 * into the first handle with type 1 and argument 0xC through the engine context's
 * renderObjectBank.
 */
// FUNCTION: FF7 0x007a98f0
void __cdecl Credits_ReleaseRenderObjectsAndCreateComesoon(void)
{
    int idx;
    PfileLoadOptions block;
    GfxEngineContext *ctx;
    int modePrev;

    ctx = GetEngineContext();
    for (idx = 0; idx < 5; idx++) {
        DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsD[idx]);
    }
    modePrev = Pfile_GetAssetSourceMode();
    Pfile_SetAssetSourceMode(0);
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &block);
    block.file.fromArchive = 1;
    block.file.archiveId = 0xF;
    block.file.callback = 0;
    block.directory = (char *)&g_CreditsRenderObjectHandle;
    block.renderFlags |= 0x400;
    Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &block);
    // STRING: FF7 0x0099c9c4
    g_CreditsQuadBatchListD = (GfxRenderObject *)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &block, "comesoon.tim", (void *)(ctx->renderObjectBank)));
    Pfile_SetAssetSourceMode(modePrev);
}

/*
 * Runs the credits initialization helper, obtains engine context and
 * initializes the same local load-options block first with blend four then
 * blend zero. Sets flag 0x4000 through the retained adjacent local slot and
 * creates/stores a render object with arguments zero/eight.
 */
// FUNCTION: FF7 0x007a99b5
void __cdecl Credits_CreateRenderSetsAndObject(void)
{
    GfxEngineContext *ctx;
    PfileLoadOptions loadOptions;

    Credits_CreateRenderObjectSets();
    ctx = GetEngineContext();
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &loadOptions);
    Pfile_InitLoadOptionsWithBlend(BLEND_AVERAGE, &loadOptions);
    loadOptions.renderFlags |= 0x4000;
    g_CreditsFadeOverlayBatch.renderObject = (int)(Gfx_CreateRenderObject(0, GFX_DRAW_FLAT_QUAD_2D, &loadOptions, 0, (void *)(ctx->renderObjectBank)));
}

/*
 * Loads what the ending staff roll (credits variant 1) draws; Credits_ModuleEnter calls it for
 * that variant. It allocates g_CreditsScriptDataBase for the 0x21B 64-byte line records
 * (0x86C0 bytes), reads people.bin from archive 15 (cd/cr_us.lgp) into it and, when the file was
 * found, centres every line on the viewport and halves the line heights in the 320x240 modes
 * (Credits_CenterRecordTexts). Then it creates the five render objects from font.tim,
 * font_ua.tim, font_ub.tim, font_s.tim and after_1.tim (Credits_CreateFiveRenderObjects) and the
 * full-screen fade overlay object (g_CreditsFadeOverlayBatch.renderObject, render flag 0x4000).
 * The heap block is not checked for NULL before the read.
 */
// FUNCTION: FF7 0x007a9a12
void __cdecl Credits_LoadPeopleBinAndCreateRenderObjects(void)
{
    GfxEngineContext *ctx;
    PfileLoadOptions loadOptions;
    int result;

    ctx = GetEngineContext();
    // STRING: FF7 0x0099c9d4
    g_CreditsScriptDataBase = (struct CreditsScriptRecord *)((struct CreditsScriptRecord *)((int)(GameHeapAlloc(539, 64, "C:\\FF7\\src\\Credits\\credfile.cpp", 269))));
    // STRING: FF7 0x0099c9f4
    result = Credits_ReadLgpFileByName("people.bin", 0x86C0, g_CreditsScriptDataBase);
    if (result == 0) {
        Credits_CenterRecordTexts();
    }
    Credits_CreateFiveRenderObjects();
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &loadOptions);
    Pfile_InitLoadOptionsWithBlend(BLEND_AVERAGE, &loadOptions);
    loadOptions.renderFlags |= 0x4000;
    g_CreditsFadeOverlayBatch.renderObject = (int)(Gfx_CreateRenderObject(0, GFX_DRAW_FLAT_QUAD_2D, &loadOptions, 0, (void *)(ctx->renderObjectBank)));
}

/*
 * Frees g_CreditsScriptDataBase and destroys the five g_CreditsQuadBatchListsD
 * render objects and g_CreditsFadeOverlayBatch.renderObject.
 */
// FUNCTION: FF7 0x007a9ab3
void __cdecl Credits_ReleasePeopleBinAndRenderObjects(void)
{
    int i;

    if (g_CreditsScriptDataBase != 0) {
        // STRING: FF7 0x0099ca00
        GameHeapFree(g_CreditsScriptDataBase, "C:\\FF7\\src\\Credits\\credfile.cpp", 318);
        g_CreditsScriptDataBase = 0;
    }
    for (i = 0; i < 5; i++) {
        DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsD[i]);
    }
    DestroyRenderObjectAndClear(&g_CreditsFadeOverlayBatch.renderObject);
}

#pragma intrinsic(strcat)

// clang-format off
// GLOBAL: FF7 0x0099c6b0
char *g_CreditsOpeningBackTimFiles[6] = {(char *)&g_CreditsOpbackTimNameA, (char *)&g_CreditsOpbackTimNameB, (char *)&g_CreditsOpbackTimNameC, (char *)&g_CreditsOpbackTimNameD, (char *)&g_CreditsOpbackTimNameE, (char *)&g_CreditsOpbackTimNameF};
// GLOBAL: FF7 0x0099c6c8
char *g_CreditsLogoTimFiles[4] = {(char *)&g_CreditsLogoTimNameA, (char *)&g_CreditsLogoTimNameB, (char *)&g_CreditsLogoTimNameC, (char *)&g_CreditsLogoTimNameD};
// GLOBAL: FF7 0x0099c6d8
char *g_CreditsNameCardLargeTimFiles[16] = {
    (char *)&g_CreditsNameTimName,
    // STRING: FF7 0x0099c81c
    "name1_b.tim",
    // STRING: FF7 0x0099c828
    "name2_a.tim",
    // STRING: FF7 0x0099c834
    "name2_b.tim",
    // STRING: FF7 0x0099c840
    "name3_a.tim",
    // STRING: FF7 0x0099c84c
    "name3_b.tim",
    // STRING: FF7 0x0099c858
    "name4_a.tim",
    // STRING: FF7 0x0099c864
    "name4_b.tim",
    // STRING: FF7 0x0099c870
    "name5_a.tim",
    // STRING: FF7 0x0099c87c
    "name5_b.tim",
    // STRING: FF7 0x0099c888
    "name6_a.tim",
    // STRING: FF7 0x0099c894
    "name6_b.tim",
    // STRING: FF7 0x0099c8a0
    "name7_a.tim",
    // STRING: FF7 0x0099c8ac
    "name7_b.tim",
    // STRING: FF7 0x0099c8b8
    "name8_a.tim",
    // STRING: FF7 0x0099c8c4
    "name8_b.tim"
};
// GLOBAL: FF7 0x0099c718
char *g_CreditsOpbackSmallTimFiles[2] = {
    // STRING: FF7 0x0099c8d0
    "opback1.tim",
    // STRING: FF7 0x0099c8dc
    "opback2.tim"
};
// STRING: FF7 0x0099c8e8
// GLOBAL: FF7 0x0099c720
char *g_CreditsLogoTimPath = "ff7logo.tim";
// GLOBAL: FF7 0x0099c728
char *g_CreditsNameCardSmallTimFiles[8] = {
    // STRING: FF7 0x0099c8f4
    "name1.tim",
    // STRING: FF7 0x0099c900
    "name2.tim",
    // STRING: FF7 0x0099c90c
    "name3.tim",
    // STRING: FF7 0x0099c918
    "name4.tim",
    // STRING: FF7 0x0099c924
    "name5.tim",
    // STRING: FF7 0x0099c930
    "name6.tim",
    // STRING: FF7 0x0099c93c
    "name7.tim",
    // STRING: FF7 0x0099c948
    "name8.tim"
};
// GLOBAL: FF7 0x0099c748
char *g_CreditsFontTimFiles[5] = {
    // STRING: FF7 0x0099c954
    "font.tim",
    // STRING: FF7 0x0099c960
    "font_ua.tim",
    // STRING: FF7 0x0099c96c
    "font_ub.tim",
    // STRING: FF7 0x0099c978
    "font_s.tim",
    // STRING: FF7 0x0099c984
    "after_1.tim"
};
// GLOBAL: FF7 0x0099c760
char *g_CreditsMovieFiles[4] = {
    // STRING: FF7 0x0099c990
    "eidoslogo.avi",
    // STRING: FF7 0x0099c9a0
    "sqlogo.avi",
    // STRING: FF7 0x0099c9ac
    "explode.avi",
    // STRING: FF7 0x0099c9b8
    "ending3.avi"
};
// GLOBAL: FF7 0x0099c770
unsigned char g_CreditsOpbackTimNameA[13] = "opback_a.tim";
// GLOBAL: FF7 0x0099c780
unsigned char g_CreditsOpbackTimNameB[13] = "opback_b.tim";
// GLOBAL: FF7 0x0099c790
unsigned char g_CreditsOpbackTimNameC[13] = "opback_c.tim";
// GLOBAL: FF7 0x0099c7a0
unsigned char g_CreditsOpbackTimNameD[13] = "opback_d.tim";
// GLOBAL: FF7 0x0099c7b0
unsigned char g_CreditsOpbackTimNameE[13] = "opback_e.tim";
// GLOBAL: FF7 0x0099c7c0
unsigned char g_CreditsOpbackTimNameF[13] = "opback_f.tim";
// GLOBAL: FF7 0x0099c7d0
unsigned char g_CreditsLogoTimNameA[14] = "ff7logo_a.tim";
// GLOBAL: FF7 0x0099c7e0
unsigned char g_CreditsLogoTimNameB[14] = "ff7logo_b.tim";
// GLOBAL: FF7 0x0099c7f0
unsigned char g_CreditsLogoTimNameC[14] = "ff7logo_c.tim";
// GLOBAL: FF7 0x0099c800
unsigned char g_CreditsLogoTimNameD[14] = "ff7logo_d.tim";
// GLOBAL: FF7 0x0099c810
unsigned char g_CreditsNameTimName[12] = "name1_a.tim";
// GLOBAL: FF7 0x00f4f490
char g_CreditsMoviePath[256] = {0};
// GLOBAL: FF7 0x00f4f590
int g_CreditsRenderObjectHandle = 0;
// GLOBAL: FF7 0x00f4f594
char g_CreditsRenderFlag0 = 0;
// GLOBAL: FF7 0x00f4f598
char g_CreditsRenderFlag1 = 0;
// GLOBAL: FF7 0x00f4f59c
char g_CreditsRenderFlag2 = 0;
// clang-format on


/*
 * Releases every credits-screen render object: the six at g_CreditsFadeOverlayBatch.backdropSlotA,
 * the sixteen at g_CreditsQuadBatchListA and g_CreditsQuadBatchListB, and the four through
 * Credits_ReleaseFourRenderObjects; both UI scaling branches do the same work.
 */
// FUNCTION: FF7 0x007a9b20
void __cdecl Credits_ReleaseRenderObjectSets(void)
{
    int i;

    if (GetUiScalingMode() == 2) {
        for (i = 0; i < 6; i++) {
            DestroyRenderObjectAndClear(&g_CreditsFadeOverlayBatch.backdropSlots[i]);
        }
        for (i = 0; i < 16; i++) {
            DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsA[i]);
            DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsB[i]);
        }
        Credits_ReleaseFourRenderObjects();
    } else {
        for (i = 0; i < 6; i++) {
            DestroyRenderObjectAndClear(&g_CreditsFadeOverlayBatch.backdropSlots[i]);
        }
        for (i = 0; i < 16; i++) {
            DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsA[i]);
            DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsB[i]);
        }
        Credits_ReleaseFourRenderObjects();
    }
}

/*
 * Releases every credits-screen render object as
 * Credits_ReleaseRenderObjectSets does, and the background object g_CreditsFadeOverlayBatch
 * as well.
 */
// FUNCTION: FF7 0x007a9c18
void __cdecl Credits_ReleaseRenderObjectSetsAndBackground(void)
{
    int i;

    if (GetUiScalingMode() == 2) {
        for (i = 0; i < 6; i++) {
            DestroyRenderObjectAndClear(&g_CreditsFadeOverlayBatch.backdropSlots[i]);
        }
        for (i = 0; i < 16; i++) {
            DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsA[i]);
            DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsB[i]);
        }
        Credits_ReleaseFourRenderObjects();
    } else {
        for (i = 0; i < 6; i++) {
            DestroyRenderObjectAndClear(&g_CreditsFadeOverlayBatch.backdropSlots[i]);
        }
        for (i = 0; i < 16; i++) {
            DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsA[i]);
            DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsB[i]);
        }
        Credits_ReleaseFourRenderObjects();
    }
    DestroyRenderObjectAndClear(&g_CreditsFadeOverlayBatch.renderObject);
}

/*
 * Releases all four render-object slots at g_CreditsLogoQuadBatches, saves and temporarily
 * clears the asset source mode, initializes blend load options and selects
 * archive 0xF. UI scale mode 2 creates four objects using the four names at
 * g_CreditsLogoTimFiles; other modes create only the first object from
 * g_CreditsLogoTimPath and clear the remaining slots.
 */
// FUNCTION: FF7 0x007a9d1d
void __cdecl Credits_CreateFourRenderObjects(void)
{
    int idx;
    int scaleMode;
    PfileLoadOptions loadOptions;
    GfxEngineContext *ctx;
    int modeSnap;

    scaleMode = GetUiScalingMode();
    ctx = GetEngineContext();
    if (scaleMode == 2) {
        for (idx = 0; idx < 4; idx++) {
            DestroyRenderObjectAndClear(&g_CreditsLogoQuadBatchList[idx]);
        }
    } else {
        for (idx = 0; idx < 4; idx++) {
            DestroyRenderObjectAndClear(&g_CreditsLogoQuadBatchList[idx]);
        }
    }
    modeSnap = Pfile_GetAssetSourceMode();
    Pfile_SetAssetSourceMode(0);
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &loadOptions);
    loadOptions.file.fromArchive = 1;
    loadOptions.file.archiveId = 0xF;
    loadOptions.file.callback = 0;
    loadOptions.directory = (char *)&g_CreditsRenderFlag0;
    loadOptions.flags |= 0x10;
    loadOptions.renderFlags |= 0x20;
    loadOptions.renderFlags |= 0x400;
    if (scaleMode == 2) {
        Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, &loadOptions);
        for (idx = 0; idx < 4; idx++) {
            g_CreditsLogoQuadBatchList[idx] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadOptions, g_CreditsLogoTimFiles[idx], (void *)ctx->renderObjectBank);
        }
    } else {
        Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, &loadOptions);
        g_CreditsLogoQuadBatches = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadOptions, g_CreditsLogoTimPath, (void *)ctx->renderObjectBank);
        for (idx = 1; idx < 4; idx++) {
            g_CreditsLogoQuadBatchList[idx] = 0;
        }
    }
    Pfile_SetAssetSourceMode(modeSnap);
}

/*
 * Releases the four render objects at g_CreditsLogoQuadBatches.
 */
// FUNCTION: FF7 0x007a9ece
void __cdecl Credits_ReleaseFourRenderObjects(void)
{
    int i;

    for (i = 0; i < 4; i++) {
        DestroyRenderObjectAndClear(&g_CreditsLogoQuadBatchList[i]);
    }
}

/*
 * Releases six credits render objects and two sixteen-slot arrays, saves and clears asset source
 * mode and configures archive 0xF load options with path g_CreditsRenderFlag1. UI scale
 * mode 2 creates six primary objects and sixteen objects in each paired array; other modes create
 * two primary and eight per paired array, clearing unused slots.
 */
// FUNCTION: FF7 0x007a9f03
void __cdecl Credits_CreateRenderObjectSets(void)
{
    int p;
    GfxEngineContext *ctx;
    int val;
    PfileLoadOptions opts;
    int ui;

    ui = GetUiScalingMode();
    ctx = GetEngineContext();
    if (ui == 2) {
        for (p = 0; p < 6; p++) {
            DestroyRenderObjectAndClear(&g_CreditsFadeOverlayBatch.backdropSlots[p]);
        }
        for (p = 0; p < 16; p++) {
            DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsA[p]);
            DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsB[p]);
        }
    } else {
        for (p = 0; p < 6; p++) {
            DestroyRenderObjectAndClear(&g_CreditsFadeOverlayBatch.backdropSlots[p]);
        }
        for (p = 0; p < 16; p++) {
            DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsA[p]);
            DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsB[p]);
        }
    }
    val = Pfile_GetAssetSourceMode();
    Pfile_SetAssetSourceMode(0);
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &opts);
    opts.file.fromArchive = 1;
    opts.file.archiveId = 0xF;
    opts.file.callback = 0;
    opts.directory = &g_CreditsRenderFlag1;
    opts.flags |= 0x10;
    opts.extraFlags |= 1;
    opts.renderFlags |= 0x20;
    opts.renderFlags |= 0x400;
    if (ui == 2) {
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &opts);
        for (p = 0; p < 6; p++) {
            g_CreditsFadeOverlayBatch.backdropSlots[p] = (int)Gfx_CreateRenderObject(
                1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, g_CreditsOpeningBackTimFiles[p],
                *(void **)&ctx->renderObjectBank);
        }
        for (p = 0; p < 16; p++) {
            Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, &opts);
            g_CreditsQuadBatchListsA[p] = (int)Gfx_CreateRenderObject(
                1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, g_CreditsNameCardLargeTimFiles[p],
                *(void **)&ctx->renderObjectBank);
            Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, &opts);
            g_CreditsQuadBatchListsB[p] = (int)Gfx_CreateRenderObject(
                1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, g_CreditsNameCardLargeTimFiles[p],
                *(void **)&ctx->renderObjectBank);
        }
    } else {
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &opts);
        for (p = 0; p < 2; p++) {
            g_CreditsFadeOverlayBatch.backdropSlots[p] = (int)Gfx_CreateRenderObject(
                1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, g_CreditsOpbackSmallTimFiles[p],
                *(void **)&ctx->renderObjectBank);
        }
        for (p = 0; p < 8; p++) {
            Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, &opts);
            g_CreditsQuadBatchListsA[p] = (int)Gfx_CreateRenderObject(
                1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, g_CreditsNameCardSmallTimFiles[p],
                *(void **)&ctx->renderObjectBank);
            Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, &opts);
            g_CreditsQuadBatchListsB[p] = (int)Gfx_CreateRenderObject(
                1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, g_CreditsNameCardSmallTimFiles[p],
                *(void **)&ctx->renderObjectBank);
        }
        for (p = 2; p < 6; p++) {
            g_CreditsFadeOverlayBatch.backdropSlots[p] = 0;
        }
        for (p = 8; p < 16; p++) {
            g_CreditsQuadBatchListsA[p] = 0;
            g_CreditsQuadBatchListsB[p] = 0;
        }
    }
    Pfile_SetAssetSourceMode(val);
}

/*
 * Clears the credits-screen state: every render object handle, the scroll and
 * timing words, and sets the initial flags (g_CreditsUnusedFlag34 = 1).
 */
// FUNCTION: FF7 0x007aa2eb
void __cdecl Credits_ClearState(void)
{
    int i;

    g_CreditsFadeOverlayBatch.renderObject = 0;
    g_CreditsScriptDataBase = 0;
    for (i = 0; i < 6; i++) {
        g_CreditsFadeOverlayBatch.backdropSlots[i] = 0;
    }
    for (i = 0; i < 16; i++) {
        g_CreditsQuadBatchListsA[i] = 0;
        g_CreditsQuadBatchListsB[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        g_CreditsLogoQuadBatchList[i] = 0;
        g_CreditsQuadBatchListsD[i] = 0;
    }
    g_CreditsTextState = 0;
    g_CreditsTextPhase = 0;
    g_CreditsSequenceState = g_CreditsTextPhase;
    g_CreditsVariant = g_CreditsSequenceState;
    g_CreditsStartSection = 0;
    g_CreditsUnusedFlag34 = 1;
    g_CreditsUnusedFlag38 = 0;
    g_CreditsSmallViewport = g_CreditsUnusedFlag38;
    g_CreditsMovieRunning = 0;
    g_CreditsTextDisplayTimer = g_CreditsMovieRunning;
    g_CreditsMoviePlaybackDone = 0;
    g_CreditsEndOverlayFlag = g_CreditsMoviePlaybackDone;
    g_CreditsTextScrollMode = g_CreditsEndOverlayFlag;
}

/*
 * Clears movie and scroll counters, sets speed and running flags to 1, and
 * writes the low/high dwords (0,0x404e8000) to the shared divisor storage.
 * Interpreted as the double used by Credits_SetFrameDurationDivisor, those
 * words represent 61.0.
 */
// FUNCTION: FF7 0x007aa425
void __cdecl Credits_InitMovieTimingVariant0(void)
{
    g_CreditsMovieRunning = 0;
    g_CreditsEndOverlayFlag = 0;
    g_CreditsTextScrollMode = g_CreditsEndOverlayFlag;
    g_CreditsWindowTimer = 1;
    g_CreditsFrameDurationDivisorLo = 0;
    g_CreditsFrameDurationDivisorHi = 0x404E8000;
    g_CreditsTextDisplayTimer = 1;
}

/*
 * Sets the running flag, clears movie and scroll counters, sets phase and
 * speed flags to 1, and writes the low/high dwords (0,0x40438000) to the
 * shared divisor storage. Interpreted as the double used by
 * Credits_SetFrameDurationDivisor, those words represent 39.0.
 */
// FUNCTION: FF7 0x007aa470
void __cdecl Credits_InitMovieTimingVariantNonZero(void)
{
    g_CreditsTextDisplayTimer = 1;
    g_CreditsMovieRunning = 0;
    g_CreditsEndOverlayFlag = 0;
    g_CreditsTextScrollMode = g_CreditsEndOverlayFlag;
    g_CreditsMoviePlaybackDone = 1;
    g_CreditsWindowTimer = 1;
    g_CreditsFrameDurationDivisorLo = 0;
    g_CreditsFrameDurationDivisorHi = 0x40438000;
}

/*
 * Opens credits movie index (0..3, other values map to 1): builds the path from
 * the movie directory g_InitPath_MoviePath and the name table
 * g_CreditsMovieFiles and opens it with the context's DirectDraw surfaces.
 */
// FUNCTION: FF7 0x007aa4c5
void __cdecl Credits_OpenMovie(int index)
{
    char *movieName;
    GfxEngineContext *context;

    context = GetEngineContext();
    if (index < 0 || index > 3) {
        index = 1;
    }
    movieName = g_CreditsMovieFiles[index];
    // STRING: FF7 0x0099ca20
    sprintf(g_CreditsMoviePath, "%s%s", g_InitPath_MoviePath, movieName);
    Movie_Open((int)g_CreditsMoviePath, 0, context->ddObject, context->ddObject2);
}

/*
 * Releases five render-object handles at g_CreditsQuadBatchListD, saves the asset-source
 * mode and switches to 0. Initializes native options for blend 4 with explicit
 * fields including g_CreditsRenderFlag2 and option bits 0x20 and 0x400.
 */
// FUNCTION: FF7 0x007aa530
void __cdecl Credits_CreateFiveRenderObjects(void)
{
    int idx;
    PfileLoadOptions loadOptions;
    GfxEngineContext *ctx;
    int storedMode;

    ctx = GetEngineContext();
    for (idx = 0; idx < 5; idx++) {
        DestroyRenderObjectAndClear(&g_CreditsQuadBatchListsD[idx]);
    }
    storedMode = Pfile_GetAssetSourceMode();
    Pfile_SetAssetSourceMode(0);
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &loadOptions);
    loadOptions.file.fromArchive = 1;
    loadOptions.file.archiveId = 0xF;
    loadOptions.file.callback = 0;
    loadOptions.directory = (char *)&g_CreditsRenderFlag2;
    loadOptions.renderFlags |= 0x20;
    loadOptions.renderFlags |= 0x400;
    Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &loadOptions);
    for (idx = 0; idx < 5; idx++) {
        g_CreditsQuadBatchListsD[idx] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadOptions, g_CreditsFontTimFiles[idx], (void *)ctx->renderObjectBank);
    }
    Pfile_SetAssetSourceMode(storedMode);
}

/*
 * Splits path into pathname components, concatenates the basename and
 * extension, and looks that name up in LGP archive 0xF. Reads size bytes into
 * buffer with ReadLgpEntry when found and returns 0; returns 1 when the lookup
 * fails.
 */
// FUNCTION: FF7 0x007aa623
int __cdecl Credits_ReadLgpFileByName(char *path, int size, void *buffer)
{
    char fname[256];
    int lgpEntry;
    char dir[256];
    char ext[256];
    char drive[4];

    _splitpath(path, drive, dir, fname, ext);
    strcat(fname, ext);
    lgpEntry = (int)FindLgpFile(fname, LGP_ARCHIVE_CREDITS);
    if (lgpEntry != 0) {
        ReadLgpEntry((void *)lgpEntry, LGP_ARCHIVE_CREDITS, buffer, size);
        return 0;
    }
    return 1;
}

// Resets a credits line record to its defaults: drawerKind is 0, color is 1, posX is -1, and the six
// header bytes from textGlyph0 to textGlyph5 are set. The wrapWidth store of 0x7FFF is overwritten at once by
// 0x50.
// FUNCTION: FF7 0x007aa6d5
void __cdecl Credits_ResetRecordDefaults(CreditsLineRecord *record)
{
    record->drawerKind = 0;
    record->color = 1;
    record->posX = 0xFFFFFFFF;
    record->wrapWidth = 0x7FFF;
    record->textGlyph0 = 4;
    record->textGlyph1 = 0x11;
    record->textGlyph2 = 0x11;
    record->textGlyph3 = 0xE;
    record->textGlyph4 = 0x11;
    record->textGlyph5 = 0x7F;
    record->wrapWidth = 80;
}

/*
 * Processes 0x21B credits records of 0x40 bytes. Resets kinds outside 0..5
 * through Credits_ResetRecordDefaults, measures each text with kind/3
 * plus the mode != 2 flag, centers it against g_CreditsViewportWidth, and halves each
 * record's duration for mode != 2.
 */
// FUNCTION: FF7 0x007aa735
void __cdecl Credits_CenterRecordTexts(void)
{
    int recordKind;
    int w;
    int index;
    int notScaled;
    int fontSize;

    notScaled = GetUiScalingMode() != 2;
    for (index = 0; index < 539; index++) {
        recordKind = ((CreditsLineRecord *)g_CreditsScriptDataBase)[index].drawerKind;
        if (recordKind < 0 || recordKind > 5) {
            Credits_ResetRecordDefaults(&((CreditsLineRecord *)g_CreditsScriptDataBase)[index]);
        }
        fontSize = recordKind / 3 + notScaled;
        w = Credits_MeasureGlyphStringWidth(&((CreditsLineRecord *)g_CreditsScriptDataBase)[index].textGlyph0, fontSize);
        ((CreditsLineRecord *)g_CreditsScriptDataBase)[index].posX = g_CreditsViewportWidth - w >> 1;
        g_CreditsScriptDataBase[index].duration >>= notScaled;
    }
}
} /* extern "C" */
