// src/ff7/src/menu/english/menu_render_objects_portraits.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Creation and release of the menu's render objects for the current UI scale, including
//   the font and window textures, and loading and freeing the character portrait graphics and the
//   Buster texture.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/menu.h"
#include "lib/pfile.h"
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/api/polygon.h"
#include "ff7/api/menu.h"
#include "ff7/api/main.h"
#include "lib/crt.h"
#include "lib/enums/graphics.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00919c18
char *g_MenuPortraitTimFilenames2x =
// STRING: FF7 0x00919c78
"cloud.tim";
#define g_MenuPortraitTimFilenameTable2x (&g_MenuPortraitTimFilenames2x)
char *g_MenuPortraitAssetFirstPath[1] = {
    // STRING: FF7 0x00919c84
    "barre.tim"
};
char *g_MenuPortraitAssetPaths[9] = {
    // STRING: FF7 0x00919c90
    "tifa.tim",
    // STRING: FF7 0x00919c9c
    "earith.tim",
    // STRING: FF7 0x00919ca8
    "red.tim",
    // STRING: FF7 0x00919cb0
    "yufi.tim",
    // STRING: FF7 0x00919cbc
    "ketc.tim",
    // STRING: FF7 0x00919cc8
    "bins.tim",
    // STRING: FF7 0x00919cd4
    "cido.tim",
    // STRING: FF7 0x00919ce0
    "pcloud.tim",
    // STRING: FF7 0x00919cec
    "pcefi.tim"
};
// GLOBAL: FF7 0x00919c44
char *g_MenuPortraitTimChoco2x =
// STRING: FF7 0x00919cf8
"choco.tim";
// GLOBAL: FF7 0x00919c48
char *g_MenuPortraitTimFilenames1x =
// STRING: FF7 0x00919d04
"cloud_l.tim";
#define g_MenuPortraitTimFilenameTable1x (&g_MenuPortraitTimFilenames1x)
char *g_MenuPortraitAltFirstPath[1] = {
    // STRING: FF7 0x00919d10
    "barre_l.tim"
};
char *g_MenuPortraitAltPaths[9] = {
    // STRING: FF7 0x00919d1c
    "tifa_l.tim",
    // STRING: FF7 0x00919d28
    "earith_l.tim",
    // STRING: FF7 0x00919d38
    "red_l.tim",
    // STRING: FF7 0x00919d44
    "yufi_l.tim",
    // STRING: FF7 0x00919d50
    "ketc_l.tim",
    // STRING: FF7 0x00919d5c
    "bins_l.tim",
    // STRING: FF7 0x00919d68
    "cido_l.tim",
    // STRING: FF7 0x00919d74
    "pcloud_l.tim",
    // STRING: FF7 0x00919d84
    "pcefi_l.tim"
};
// GLOBAL: FF7 0x00919c74
char *g_MenuPortraitTimChoco1x =
// STRING: FF7 0x00919d90
"choco_l.tim";
// GLOBAL: FF7 0x00dc1058
int g_MenuViewportScale = 0;
// GLOBAL: FF7 0x00dc105c
int g_MenuViewportX = 0;
// GLOBAL: FF7 0x00dc1060
int g_MenuViewportY = 0;
// GLOBAL: FF7 0x00dc1064
int g_MenuViewportWidth = 0;
// GLOBAL: FF7 0x00dc1068
int g_MenuViewportHeight = 0;
// GLOBAL: FF7 0x00dc106c
int g_MenuTifaReelFadeActive = 0;
// GLOBAL: FF7 0x00dc1070
int g_MenuTifaReelResetWord = 0;
// GLOBAL: FF7 0x00dc1074
int g_MenuFontWindowLoadDir = 0;
// GLOBAL: FF7 0x00dc1078
int g_MenuPortraitLoadDir = 0;
// GLOBAL: FF7 0x00dc107c
int g_MenuAllPortraitsLoadDir = 0;
// GLOBAL: FF7 0x00dc1080
int g_MenuBusterLoadDir = 0;
unsigned char g_bssPad_00dc1084[4] = {0};
// clang-format on


/*
 * Loads the font and window textures, then creates the seven load-screen render objects (window
 * background, blended window background, gauge bar, flat rectangle, band, battle limit and text box
 * quad) from the engine context's render object bank. The first three are kind 0xA with blend modes
 * 4, 0 and 1; the other four are kind 8 with modes 1, 0, 1 and 0.
 */
// FUNCTION: FF7 0x006c0f60
void __cdecl Menu_CreateLoadScreenObjects(void)
{
    PfileLoadOptions loadOptions;
    GfxEngineContext *ctx;

    Menu_LoadFontAndWindowTextures(0);
    ctx = GetEngineContext();
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &loadOptions);
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &loadOptions);
    g_MenuWindowBgGraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_QUAD_2D, &loadOptions, 0, (void *)ctx->renderObjectBank);
    Pfile_InitLoadOptionsWithBlend(BLEND_AVERAGE, &loadOptions);
    g_MenuBlendWindowBgGraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_QUAD_2D, &loadOptions, 0, (void *)ctx->renderObjectBank);
    Pfile_InitLoadOptionsWithBlend(BLEND_ADDITIVE, &loadOptions);
    g_MenuGaugeBarGraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_QUAD_2D, &loadOptions, 0, (void *)ctx->renderObjectBank);
    Pfile_InitLoadOptionsWithBlend(BLEND_ADDITIVE, &loadOptions);
    g_MenuFlatRectGraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, &loadOptions, 0, (void *)ctx->renderObjectBank);
    Pfile_InitLoadOptionsWithBlend(BLEND_AVERAGE, &loadOptions);
    loadOptions.renderFlags |= 0x4000;
    g_MenuBandGraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, &loadOptions, 0, (void *)ctx->renderObjectBank);
    loadOptions.renderFlags &= 0xFFFFBFFF;
    Pfile_InitLoadOptionsWithBlend(BLEND_ADDITIVE, &loadOptions);
    g_MenuBattleLimitGraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, &loadOptions, 0, (void *)ctx->renderObjectBank);
    Pfile_InitLoadOptionsWithBlend(BLEND_AVERAGE, &loadOptions);
    loadOptions.renderFlags |= 0x4000;
    g_MenuTextBoxQuadGraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(0, GFX_DRAW_FLAT_QUAD_2D, &loadOptions, 0, (void *)ctx->renderObjectBank);
}

/*
 * Destroys the font and window render objects of the current UI scale (eleven when GetUiScalingMode
 * is 2, four otherwise), the three party portrait objects, the nine character portrait objects and
 * the eight shared load-screen objects.
 */
// FUNCTION: FF7 0x006c10ee
void __cdecl Menu_UnusedDestroyAllRenderObjects(void)
{
    int i;

    if (GetUiScalingMode() == 2) {
        DestroyRenderObject(g_MenuFontAGraphicsObject);
        DestroyRenderObject(g_MenuFontBGraphicsObject.batchList);
        DestroyRenderObject(g_MenuWinABlend4GraphicsObject);
        DestroyRenderObject(g_MenuWinBBlend4GraphicsObject);
        DestroyRenderObject(g_MenuWinCBlend4GraphicsObject);
        DestroyRenderObject(g_MenuWinDBlend4GraphicsObject);
        DestroyRenderObject(g_MenuWinABlend0GraphicsObject);
        DestroyRenderObject(g_MenuWinABlend1GraphicsObject);
        DestroyRenderObject(g_MenuWinBBlend1GraphicsObject);
        DestroyRenderObject(g_MenuWinCBlend1GraphicsObject);
        DestroyRenderObject(g_MenuWinDBlend1GraphicsObject);
    } else {
        DestroyRenderObject(g_MenuFontBlend4GraphicsObject);
        DestroyRenderObject(g_MenuWinBlend4GraphicsObject);
        DestroyRenderObject(g_MenuWinBlend0GraphicsObject);
        DestroyRenderObject(g_MenuWinBlend1GraphicsObject);
    }
    for (i = 0; i < 3; i++) {
        DestroyRenderObject(g_MenuFontBGraphicsObject.portraitRenderObjects[i]);
    }
    for (i = 0; i < 9; i++) {
        DestroyRenderObject(g_MenuCharPortraitGraphicsObjects[i]);
    }
    DestroyRenderObject(g_MenuWinCBlend4DiffGraphicsObject);
    DestroyRenderObject(g_MenuWindowBgGraphicsObject);
    DestroyRenderObject(g_MenuBlendWindowBgGraphicsObject);
    DestroyRenderObject(g_MenuGaugeBarGraphicsObject);
    DestroyRenderObject(g_MenuFlatRectGraphicsObject);
    DestroyRenderObject(g_MenuBandGraphicsObject);
    DestroyRenderObject(g_MenuBattleLimitGraphicsObject);
    DestroyRenderObject(g_MenuTextBoxQuadGraphicsObject);
}

/*
 * Releases and nulls every menu render object handle, in the same layout-dependent
 * set as Menu_UnusedDestroyAllRenderObjects, skipping handles that are
 * already 0.
 */
// FUNCTION: FF7 0x006c12b1
void __cdecl Menu_ReleaseAllRenderObjects(void)
{
    int i;

    if (GetUiScalingMode() == 2) {
        DestroyRenderObjectAndClear(&g_MenuFontAGraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuFontBGraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinABlend4GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinBBlend4GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinCBlend4GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinDBlend4GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinABlend0GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinABlend1GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinBBlend1GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinCBlend1GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinDBlend1GraphicsObject);
    } else {
        DestroyRenderObjectAndClear(&g_MenuFontBlend4GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinBlend4GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinBlend0GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinBlend1GraphicsObject);
    }
    DestroyRenderObjectAndClear(&g_MenuWinCBlend4DiffGraphicsObject);
    DestroyRenderObjectAndClear(&g_MenuWindowBgGraphicsObject);
    DestroyRenderObjectAndClear(&g_MenuBlendWindowBgGraphicsObject);
    DestroyRenderObjectAndClear(&g_MenuGaugeBarGraphicsObject);
    DestroyRenderObjectAndClear(&g_MenuFlatRectGraphicsObject);
    DestroyRenderObjectAndClear(&g_MenuBandGraphicsObject);
    DestroyRenderObjectAndClear(&g_MenuBattleLimitGraphicsObject);
    DestroyRenderObjectAndClear(&g_MenuTextBoxQuadGraphicsObject);
    for (i = 0; i < 3; i++) {
        if (g_MenuFontBGraphicsObject.portraitRenderObjects[i] != 0) {
            DestroyRenderObjectAndClear(&g_MenuFontBGraphicsObject.portraitRenderObjects[i]);
        }
    }
    for (i = 0; i < 9; i++) {
        if (g_MenuCharPortraitGraphicsObjects[i] != 0) {
            DestroyRenderObjectAndClear(&g_MenuCharPortraitGraphicsObjects[i]);
        }
    }
}

/*
 * Releases the font/window render objects for the current UI scale plus shared
 * g_MenuWinCBlend4DiffGraphicsObject, saves asset-source mode and selects mode zero while recreating
 * them. hiRes chooses high/low TIM asset variants.
 */
// FUNCTION: FF7 0x006c1468
void __cdecl Menu_LoadFontAndWindowTextures(int hiRes)
{
    GfxEngineContext *context;
    int windowAPath;
    int fontBPath;
    int windowDName;
    int fontStr;
    int prevMode;
    int fontAStr;
    int winCTextureName;
    int uiScaling;
    int srcPath;
    int windowBName;
    int winName;
    PfileLoadOptions loadConfig;

    uiScaling = GetUiScalingMode();
    context = GetEngineContext();
    if (uiScaling == 2) {
        DestroyRenderObjectAndClear(&g_MenuFontAGraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuFontBGraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinABlend4GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinBBlend4GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinCBlend4GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinDBlend4GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinABlend0GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinABlend1GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinBBlend1GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinCBlend1GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinDBlend1GraphicsObject);
    } else {
        DestroyRenderObjectAndClear(&g_MenuFontBlend4GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinBlend4GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinBlend0GraphicsObject);
        DestroyRenderObjectAndClear(&g_MenuWinBlend1GraphicsObject);
    }
    DestroyRenderObjectAndClear(&g_MenuWinCBlend4DiffGraphicsObject);
    prevMode = Pfile_GetAssetSourceMode();
    Pfile_SetAssetSourceMode(0);
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &loadConfig);
    loadConfig.file.fromArchive = 1;
    loadConfig.file.archiveId = 4;
    loadConfig.file.callback = 0;
    loadConfig.directory = (char *)&g_MenuFontWindowLoadDir;
    loadConfig.flags |= 0x10;
    loadConfig.extraFlags |= 1;
    loadConfig.renderFlags |= 0x20;
    if (uiScaling == 2) {
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &loadConfig);
        if (hiRes != 0) {
            // STRING: FF7 0x00919d9c
            fontAStr = (int)"usfont_a_h.tim";
        } else {
            // STRING: FF7 0x00919dac
            fontAStr = (int)"usfont_a_l.tim";
        }
        srcPath = fontAStr;
        g_MenuFontAGraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &loadConfig);
        if (hiRes != 0) {
            // STRING: FF7 0x00919dbc
            fontBPath = (int)"usfont_b_h.tim";
        } else {
            // STRING: FF7 0x00919dcc
            fontBPath = (int)"usfont_b_l.tim";
        }
        srcPath = fontBPath;
        g_MenuFontBGraphicsObject.batchList = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        if (hiRes != 0) {
            // STRING: FF7 0x00919ddc
            windowAPath = (int)"btl_win_a_h.tim";
        } else {
            // STRING: FF7 0x00919dec
            windowAPath = (int)"btl_win_a_l.tim";
        }
        srcPath = windowAPath;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &loadConfig);
        g_MenuWinABlend4GraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, &loadConfig);
        g_MenuWinABlend0GraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, &loadConfig);
        g_MenuWinABlend1GraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        if (hiRes != 0) {
            // STRING: FF7 0x00919dfc
            windowBName = (int)"btl_win_b_h.tim";
        } else {
            // STRING: FF7 0x00919e0c
            windowBName = (int)"btl_win_b_l.tim";
        }
        srcPath = windowBName;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &loadConfig);
        g_MenuWinBBlend4GraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, &loadConfig);
        g_MenuWinBBlend1GraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        if (hiRes != 0) {
            // STRING: FF7 0x00919e1c
            winCTextureName = (int)"btl_win_c_h.tim";
        } else {
            // STRING: FF7 0x00919e2c
            winCTextureName = (int)"btl_win_c_l.tim";
        }
        srcPath = winCTextureName;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &loadConfig);
        g_MenuWinCBlend4GraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, &loadConfig);
        g_MenuWinCBlend1GraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        loadConfig.renderFlags &= 0xFFFFFFDF;
        loadConfig.renderFlags |= 0x80;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &loadConfig);
        g_MenuWinCBlend4DiffGraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        loadConfig.renderFlags &= 0xFFFFFF7F;
        loadConfig.renderFlags |= 0x20;
        if (hiRes != 0) {
            // STRING: FF7 0x00919e3c
            windowDName = (int)"btl_win_d_h.tim";
        } else {
            // STRING: FF7 0x00919e4c
            windowDName = (int)"btl_win_d_l.tim";
        }
        srcPath = windowDName;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &loadConfig);
        g_MenuWinDBlend4GraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, &loadConfig);
        g_MenuWinDBlend1GraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
    } else {
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &loadConfig);
        if (hiRes != 0) {
            // STRING: FF7 0x00919e5c
            fontStr = (int)"usfont_h.tim";
        } else {
            // STRING: FF7 0x00919e6c
            fontStr = (int)"usfont_l.tim";
        }
        srcPath = fontStr;
        g_MenuFontBlend4GraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        if (hiRes != 0) {
            // STRING: FF7 0x00919e7c
            winName = (int)"btl_win_h.tim";
        } else {
            // STRING: FF7 0x00919e8c
            winName = (int)"btl_win_l.tim";
        }
        srcPath = winName;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &loadConfig);
        g_MenuWinBlend4GraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, &loadConfig);
        g_MenuWinBlend0GraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, &loadConfig);
        g_MenuWinBlend1GraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
        loadConfig.renderFlags &= 0xFFFFFFDF;
        loadConfig.renderFlags |= 0x80;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &loadConfig);
        g_MenuWinCBlend4DiffGraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadConfig, (char *)srcPath, (void *)context->renderObjectBank);
    }
    Pfile_SetAssetSourceMode(prevMode);
}

/*
 * Replaces the party portrait render object in slot destSlot with the portrait of character charId:
 * releases the old object, temporarily selects asset-source mode 0, takes the file name from
 * g_MenuPortraitTimFilenames2x when the UI scale mode is 2 (g_MenuPortraitTimFilenames1x otherwise)
 * and creates the render object from the engine context's render object bank.
 */
// FUNCTION: FF7 0x006c1aba
void __cdecl Menu_LoadCharPortrait(int charId, int destSlot)
{
    PfileLoadOptions characterOpts;
    int characterPath;
    GfxEngineContext *context;
    int prevMode;

    context = GetEngineContext();
    if (g_MenuFontBGraphicsObject.portraitRenderObjects[destSlot] != 0) {
        DestroyRenderObjectAndClear(destSlot * 4 + (char *)g_MenuFontBGraphicsObject.portraitRenderObjects);
    }
    prevMode = Pfile_GetAssetSourceMode();
    Pfile_SetAssetSourceMode(0);
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &characterOpts);
    characterOpts.file.fromArchive = 1;
    characterOpts.file.archiveId = 4;
    characterOpts.file.callback = 0;
    characterOpts.directory = (char *)&g_MenuPortraitLoadDir;
    Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &characterOpts);
    if (GetUiScalingMode() == 2) {
        characterPath = (int)g_MenuPortraitTimFilenameTable2x[charId];
    } else {
        characterPath = (int)g_MenuPortraitTimFilenameTable1x[charId];
    }
    g_MenuFontBGraphicsObject.portraitRenderObjects[destSlot] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &characterOpts, (char *)characterPath, (void *)context->renderObjectBank);
    Pfile_SetAssetSourceMode(prevMode);
}

/*
 * Reloads the nine character portrait render objects at the current UI scale, with asset-source mode
 * 0 and blend 4 load options. loadMode 2 also releases the three party portrait objects; loadMode 1
 * loads the chocobo portrait in place of the ninth character.
 */
// FUNCTION: FF7 0x006c1b93
void __cdecl Menu_LoadAllCharPortraits(int loadMode)
{
    int idx;
    PfileLoadOptions loadOptions;
    int portraitFile;
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    Pfile_SetAssetSourceMode(0);
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &loadOptions);
    loadOptions.file.fromArchive = 1;
    loadOptions.file.archiveId = 4;
    loadOptions.file.callback = 0;
    loadOptions.directory = (char *)&g_MenuAllPortraitsLoadDir;
    Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &loadOptions);
    if (loadMode == 2) {
        for (idx = 0; idx < 3; idx++) {
            if (g_MenuFontBGraphicsObject.portraitRenderObjects[idx] != 0) {
                DestroyRenderObjectAndClear(idx * 4 + (char *)g_MenuFontBGraphicsObject.portraitRenderObjects);
            }
        }
    }
    for (idx = 0; idx < 9; idx++) {
        if (g_MenuCharPortraitGraphicsObjects[idx] != 0) {
            DestroyRenderObjectAndClear(idx * 4 + (char *)&g_MenuCharPortraitGraphicsObjects);
        }
        if (GetUiScalingMode() == 2) {
            if (loadMode == 1 && idx == 8) {
                portraitFile = (int)g_MenuPortraitTimChoco2x;
            } else {
                portraitFile = (int)g_MenuPortraitTimFilenameTable2x[idx];
            }
        } else if (loadMode == 1 && idx == 8) {
            portraitFile = (int)g_MenuPortraitTimChoco1x;
        } else {
            portraitFile = (int)g_MenuPortraitTimFilenameTable1x[idx];
        }
        g_MenuCharPortraitGraphicsObjects[idx] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &loadOptions, (char *)portraitFile, (void *)ctx->renderObjectBank);
    }
}

/*
 * Reloads buster.tim: releases the previous g_MenuBusterTexGraphicsObject if present and creates
 * the render object from the engine context's render object bank with asset-source mode 0 and blend 4
 * load options, then restores the saved source mode.
 */
// FUNCTION: FF7 0x006c1ce7
void __cdecl Menu_LoadBusterTim(void)
{
    int busterFile;
    PfileLoadOptions busterMode;
    GfxEngineContext *context;
    int prevMode;

    context = GetEngineContext();
    prevMode = Pfile_GetAssetSourceMode();
    Pfile_SetAssetSourceMode(0);
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &busterMode);
    busterMode.file.fromArchive = 1;
    busterMode.file.archiveId = 4;
    busterMode.file.callback = 0;
    busterMode.directory = (char *)&g_MenuBusterLoadDir;
    busterMode.extraFlags |= 1;
    if (g_MenuBusterTexGraphicsObject != 0) {
        DestroyRenderObjectAndClear(&g_MenuBusterTexGraphicsObject);
    }
    // STRING: FF7 0x00919e9c
    busterFile = (int)"buster.tim";
    g_MenuBusterTexGraphicsObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &busterMode, (char *)busterFile, (void *)context->renderObjectBank);
    Pfile_SetAssetSourceMode(prevMode);
}

/*
 * Zeroes every menu texture/render object handle, including the three party portrait handles and the
 * nine character portrait handles. Also clears the viewport scale, origin and size, sets
 * g_MenuTifaReelFadeActive to 1 and clears g_MenuTifaReelResetWord.
 */
// FUNCTION: FF7 0x006c1d8d
void __cdecl Menu_ClearUiTextureHandles(void)
{
    int i;

    g_MenuWinABlend4GraphicsObject = 0;
    g_MenuWinBBlend4GraphicsObject = 0;
    g_MenuWinCBlend4GraphicsObject = 0;
    g_MenuWinDBlend4GraphicsObject = 0;
    g_MenuWinABlend0GraphicsObject = 0;
    g_MenuWinABlend1GraphicsObject = 0;
    g_MenuWinBBlend1GraphicsObject = 0;
    g_MenuWinCBlend1GraphicsObject = 0;
    g_MenuWinDBlend1GraphicsObject = 0;
    g_MenuWinCBlend4DiffGraphicsObject = 0;
    g_MenuWindowBgGraphicsObject = 0;
    g_MenuBlendWindowBgGraphicsObject = 0;
    g_MenuGaugeBarGraphicsObject = 0;
    g_MenuFlatRectGraphicsObject = 0;
    g_MenuBandGraphicsObject = 0;
    g_MenuBattleLimitGraphicsObject = 0;
    g_MenuTextBoxQuadGraphicsObject = 0;
    g_MenuFontAGraphicsObject = 0;
    g_MenuFontBGraphicsObject.batchList = 0;
    for (i = 0; i < 3; i++) {
        g_MenuFontBGraphicsObject.portraitRenderObjects[i] = 0;
    }
    for (i = 0; i < 9; i++) {
        g_MenuCharPortraitGraphicsObjects[i] = 0;
    }
    g_MenuBusterTexGraphicsObject = 0;
    g_MenuFontBlend4GraphicsObject = 0;
    g_MenuWinBlend4GraphicsObject = 0;
    g_MenuWinBlend0GraphicsObject = 0;
    g_MenuWinBlend1GraphicsObject = 0;
    g_MenuViewportScale = 0;
    g_MenuViewportX = 0;
    g_MenuViewportY = 0;
    g_MenuViewportWidth = 0;
    g_MenuViewportHeight = 0;
    g_MenuTifaReelFadeActive = 1;
    g_MenuTifaReelResetWord = 0;
}

/*
 * Releases and nulls the non-null render objects of g_MenuCharPortraitGraphicsObjects.
 */
// FUNCTION: FF7 0x006c1f1b
void __cdecl Menu_FreeAllCharPortraits(void)
{
    int i;

    for (i = 0; i < 9; i++) {
        if (g_MenuCharPortraitGraphicsObjects[i] != 0) {
            DestroyRenderObjectAndClear(&g_MenuCharPortraitGraphicsObjects[i]);
        }
    }
}
} /* extern "C" */
