// src/ff7/src/menu/btlmenu/english/callback.cpp
// Original: C:\FF7\src\menu\btlmenu\english\callback.cpp
// Contents: Battle menu UI support: the dword list of UI entries, loading and submitting the battle
//   UI textures and batches, the ordering-table swap and sound helpers, and the per-frame battle
//   update. Several entries are empty stubs the shipped build kept.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/pfile.h"
#include "ff7/types/menu.h"
#include "lib/types/graphics.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "lib/api/token.h"
#include "ff7/api/main.h"
#include "lib/api/file.h"
#include "lib/api/polygon.h"
#include "lib/api/sound.h"
#include "lib/crt.h"
#include "lib/enums/file.h"
#include "lib/enums/graphics.h"
#include "lib/codegen.h"
#include "ff7/savemap.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/types/battle.h"
#include "ff7/enums/main.h"
extern "C" {

// clang-format off
// clang-format on


// clang-format off
// GLOBAL: FF7 0x0091bd68
int g_BtlMenuFrameParity = (-1);
// GLOBAL: FF7 0x0091bd6c
unsigned char g_BtlMenuUiMode = 0x02;
// GLOBAL: FF7 0x00dc16d8
GfxRenderObject *g_BtlMenuRenderObjColoc = 0;
// GLOBAL: FF7 0x00dc16dc
GfxRenderObject *g_BtlMenuRenderObjKetcy2b = 0;
// GLOBAL: FF7 0x00dc16e0
GfxRenderObject *g_BtlMenuRenderObjKetcy2 = 0;
// GLOBAL: FF7 0x00dc16e4
GfxRenderObject *g_BtlMenuRenderObjColob = 0;
unsigned char g_bssPad_00dc16e8[128] = {0};
// GLOBAL: FF7 0x00dc1768
unsigned char g_BtlMenuDoubleBufferedCursorRecord[0x20] = {0};
// GLOBAL: FF7 0x00dc1788
GfxRenderObject *g_BtlMenuRenderObjColo = 0;
unsigned char g_bssPad_00dc178c[4] = {0};
unsigned char g_bssPad_00dc1790[512] = {0};
// GLOBAL: FF7 0x00dc1990
GfxRenderObject *g_BtlMenuRenderObjZeni = 0;
// GLOBAL: FF7 0x00dc1994
GfxRenderObject *g_BtlMenuRenderObjColoa = 0;
// GLOBAL: FF7 0x00dc1998
GfxRenderObject *g_BtlMenuRenderObjKetcy2a = 0;
unsigned char g_bssPad_00dc199c[4] = {0};
// The list of dwords, appended through g_BtlMenuUiDwordListCount. Its span runs to the next
// named definition of the TU.
// GLOBAL: FF7 0x00dc19a0
int g_BtlMenuUiDwordList[0x598 / 4] = {0};
// GLOBAL: FF7 0x00dc1f38
int g_BtlMenuKetcyTexturesLoaded = 0;
// GLOBAL: FF7 0x00dc1f3c
unsigned short g_BtlMenuSavedMask = 0;
// GLOBAL: FF7 0x00dc1f40
int g_BtlMenuCursorBufferIndex = 0;
// GLOBAL: FF7 0x00dc1f44
int g_BtlMenuFrameCounter = 0;
// GLOBAL: FF7 0x00dc1f48
int g_BtlMenuUiDwordListCount = 0;
unsigned char g_bssPad_00dc1f4c[4] = {0};
// GLOBAL: FF7 0x00dc1f50
int g_BtlMenuTextureAuxFieldA = 0;
// GLOBAL: FF7 0x00dc1f54
int g_BtlMenuTextureAuxFieldB = 0;
// clang-format on




/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006ce7d0
void __cdecl Battle_UnusedNoOp_2(void)
{
    /* The original is empty. */
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006ce7d5
void __cdecl Battle_UnusedNoOp_3(void)
{
    /* The original is empty. */
}

/*
 * Clears the count g_BtlMenuUiDwordListCount of the dword list at
 * g_BtlMenuUiDwordList.
 */
// FUNCTION: FF7 0x006ce7da
void __cdecl BtlMenu_ClearUiDwordQueue(void)
{
    g_BtlMenuUiDwordListCount = 0;
}

/*
 * Appends value to the dword list at g_BtlMenuUiDwordList and bumps its count
 * g_BtlMenuUiDwordListCount.
 */
// FUNCTION: FF7 0x006ce7e9
void __cdecl BtlMenu_AppendUiDwordQueue(int value)
{
    g_BtlMenuUiDwordList[g_BtlMenuUiDwordListCount] = value;
    g_BtlMenuUiDwordListCount++;
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006ce80c
void __cdecl Battle_UnusedNoOp_4(void)
{
    /* The original is empty. */
}

/*
 * Sets g_BtlMenuUiMode.
 */
// FUNCTION: FF7 0x006ce811
void __cdecl Battle_SetUiModeByte(int value)
{
    g_BtlMenuUiMode = value;
}

/*
 * Swaps the battle renderer's two ordering tables (g_BattleOrderingTableA
 * and g_BattleOrderingTableB) as the current one
 * g_BattleCurrentOrderingTable and toggles the frame parity
 * g_BattleDrawFrameParity. The return value is an unset frame slot, as in
 * the original.
 */
// FUNCTION: FF7 0x006ce81e
int __cdecl Battle3d_SwapOrderingTable(void)
{
    int unsetResult;
    int nextTable;

    if (g_BattleCurrentOrderingTable == (int)&g_BattleOrderingTableA) {
        nextTable = (int)&g_BattleOrderingTableB;
    } else {
        nextTable = (int)&g_BattleOrderingTableA;
    }
    g_BattleCurrentOrderingTable = nextTable;
    g_BattleDrawFrameParity ^= 1;
    return unsetResult;
}

/*
 * Unpauses: when the game is paused (g_GamePauseLatch), issues sound command 0x98 (what Start
 * sends on unpausing; 0x99 on pausing), then clears the latch.
 */
// FUNCTION: FF7 0x006ce85e
void __cdecl Battle_Unpause(void)
{
    if (g_GamePauseLatch != 0) {
        Battle_DispatchSoundCommandSameParams(0x98);
    }
    g_GamePauseLatch = 0;
}

/*
 * Issues sound command command with its low 16 bits as both of the first two
 * parameters and the rest zero.
 */
// FUNCTION: FF7 0x006ce882
void __cdecl Battle_DispatchSoundCommandSameParams(int command)
{
    SoundDispatchCommand(command, command & 0xFFFF, command & 0xFFFF, 0, 0, 0, 0, 0, 0);
}

/*
 * One battle frame: notice a target switch, flip the double-buffered record at
 * g_BtlMenuDoubleBufferedCursorRecord (0x00DC1768), run the UI and the logic for whichever half
 * g_MenuIsSmallViewport320x240 selects, then poll input. The frame is eight bytes and
 * nothing reads a slot -- two locals the original declared and never used, which still have to be
 * declared here or the frame is the wrong size.
 */
// FUNCTION: FF7 0x006ce8b3
void __cdecl Battle_UpdateUiAndLogic(void)
{
    int pad[2];

    g_TokenState = 0;
    if (g_BtlMenuFrameParity != g_BattleDrawFrameParity) {
        g_TokenState = 1;
        g_BtlMenuSavedMask = g_BattleScene.readyActorMask;
        Battle_ResetSelectionCursorPosition();
        g_BtlMenuUiDwordListCount = 0;
    }
    g_BtlMenuCursorBufferIndex ^= 1;
    Battle_UiFrameHookNoOp();
    if (g_MenuIsSmallViewport320x240 != 1) {
        Battle_DrawTargetCursorWhenMenuStateZero2x((void *)(g_BtlMenuDoubleBufferedCursorRecord + g_BtlMenuCursorBufferIndex * 0x10));
        Battle_DrawOpenWindows2x();
    } else {
        Battle_DrawTargetCursorWhenMenuStateZero1x((void *)(g_BtlMenuDoubleBufferedCursorRecord + g_BtlMenuCursorBufferIndex * 0x10));
        Battle_DrawOpenWindows1x();
    }
    Input_Update(GetEngineContext());
    if (g_BtlMenuFrameParity != g_BattleDrawFrameParity) {
        g_BtlMenuFrameParity = g_BattleDrawFrameParity;
        g_BattleIsPaused = g_GamePauseLatch;
    }
    if (g_GamePauseLatch == 0) {
        Battle_ProcessCommandMenuInput();
    }
    if (Input_GetPressed(INPUT_START) != 0 && g_BattleSceneState == 4) {
        g_GamePauseLatch ^= 1;
        if (g_GamePauseLatch != 0) {
            Battle_DispatchSoundCommandSameParams(0x99);
        } else {
            Battle_DispatchSoundCommandSameParams(0x98);
        }
    }
    if (g_GamePauseLatch == 0) {
        Battle_TickAtbAndGauges();
    }
    g_BtlMenuFrameCounter = g_BtlMenuFrameCounter + 1;
}

/*
 * Sets a local to 0xF0 and does nothing with it: a stub the shipped build kept.
 */
// FUNCTION: FF7 0x006cea08
void __cdecl Battle_DebugDumpSceneNoOp(int width, int height, int value, char r, char g, char b)
{
    int val;

    val = 0xF0;
}

/*
 * Loads battle UI textures from asset source mode 0 and restores the previous
 * source mode afterward. Scans three party slots and sets, but never clears,
 * g_BtlMenuKetcyTexturesLoaded when a mapped character record starts with byte 6.
 */
// FUNCTION: FF7 0x006cea17
void __cdecl Battle_LoadUiTextures(void)
{
    int partyIndex;
    PfileLoadOptions loadOptions;
    GfxEngineContext *gfxCtx;
    char *name;
    int prevSourceMode;

    for (partyIndex = 0; partyIndex < 3; partyIndex++) {
        if (gSavemap.party[partyIndex] != PARTY_CHAR_NONE && gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[partyIndex]]].id == PARTY_CHAR_CAIT_SITH) {
            g_BtlMenuKetcyTexturesLoaded = 1;
        }
    }
    gfxCtx = GetEngineContext();
    prevSourceMode = Pfile_GetAssetSourceMode();
    Pfile_SetAssetSourceMode(0);
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, (PfileLoadOptions *)&loadOptions);
    loadOptions.file.fromArchive = 1;
    loadOptions.file.archiveId = 4;
    loadOptions.file.callback = 0;
    loadOptions.directory = (char *)&g_BtlMenuTextureAuxFieldA;
    loadOptions.extraFlags |= 1;
    loadOptions.renderFlags |= 0x20;
    if (GetUiScalingMode() == 2) {
        if (g_BtlMenuKetcyTexturesLoaded != 0) {
            Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)&loadOptions);
            // STRING: FF7 0x0091bd70
            name = "ketcy2a.tim";
            g_BtlMenuRenderObjKetcy2a = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)&loadOptions, name, (void *)gfxCtx->renderObjectBank);
            Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)&loadOptions);
            // STRING: FF7 0x0091bd7c
            name = "ketcy2b.tim";
            g_BtlMenuRenderObjKetcy2b = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)&loadOptions, name, (void *)gfxCtx->renderObjectBank);
        }
        loadOptions.renderFlags &= 0xFFFFFFDF;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)&loadOptions);
        // STRING: FF7 0x0091bd88
        name = "zeni_h.tim";
        g_BtlMenuRenderObjZeni = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)&loadOptions, name, (void *)gfxCtx->renderObjectBank);
        loadOptions.renderFlags |= 0x20;
        loadOptions.flags |= 0x10;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)&loadOptions);
        // STRING: FF7 0x0091bd94
        name = "coloa.tim";
        g_BtlMenuRenderObjColoa = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)&loadOptions, name, (void *)gfxCtx->renderObjectBank);
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)&loadOptions);
        // STRING: FF7 0x0091bda0
        name = "colob.tim";
        g_BtlMenuRenderObjColob = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)&loadOptions, name, (void *)gfxCtx->renderObjectBank);
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)&loadOptions);
        // STRING: FF7 0x0091bdac
        name = "coloc.tim";
        g_BtlMenuRenderObjColoc = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)&loadOptions, name, (void *)gfxCtx->renderObjectBank);
    } else {
        if (g_BtlMenuKetcyTexturesLoaded != 0) {
            Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)&loadOptions);
            // STRING: FF7 0x0091bdb8
            name = "ketcy2.tim";
            g_BtlMenuRenderObjKetcy2 = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)&loadOptions, name, (void *)gfxCtx->renderObjectBank);
        }
        loadOptions.renderFlags &= 0xFFFFFFDF;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)&loadOptions);
        // STRING: FF7 0x0091bdc4
        name = "zeni.tim";
        g_BtlMenuRenderObjZeni = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)&loadOptions, name, (void *)gfxCtx->renderObjectBank);
        loadOptions.renderFlags |= 0x20;
        loadOptions.flags |= 0x10;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)&loadOptions);
        // STRING: FF7 0x0091bdd0
        name = "colo.tim";
        g_BtlMenuRenderObjColo = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)&loadOptions, name, (void *)gfxCtx->renderObjectBank);
    }
    Battle_SelectMenuUiTablesForScalingMode();
    g_BtlMenuUiMode = 2;
    Menu_ApplyUiScalingViewport();
    Menu_SetTextSubsystemReady(1);
    Menu_ClearBattleWindowStates();
    if ((gfxCtx->flags & 1) != 0) {
        g_MenuTifaReelFadeActive = 1;
    } else {
        g_MenuTifaReelFadeActive = 0;
    }
    Pfile_SetAssetSourceMode(prevSourceMode);
}

/*
 * called from Battle_ModuleExit. Frees the battle-UI text buffer via the tracked
 * heap (__FILE__ literal, line 0x20D=525) then releases 6-8 DirectDraw surface
 * pairs depending on GetUiScalingMode()==2.
 */
// FUNCTION: FF7 0x006cedad
void __cdecl Battle_FreeUiTextures(void)
{
    if (g_pBtlMenuUiTextures != 0) {
        // STRING: FF7 0x0091bddc
        GameHeapFree((void *)g_pBtlMenuUiTextures, "C:\\FF7\\src\\menu\\btlmenu\\english\\callback.cpp", 525);
        g_pBtlMenuUiTextures = 0;
    }
    if (GetUiScalingMode() == 2) {
        if (g_BtlMenuKetcyTexturesLoaded != 0) {
            DestroyRenderObjectAndClear(&g_BtlMenuRenderObjKetcy2a);
            DestroyRenderObjectAndClear(&g_BtlMenuRenderObjKetcy2b);
            g_BtlMenuKetcyTexturesLoaded = 0;
        }
        DestroyRenderObjectAndClear(&g_BtlMenuRenderObjColoa);
        DestroyRenderObjectAndClear(&g_BtlMenuRenderObjColob);
        DestroyRenderObjectAndClear(&g_BtlMenuRenderObjColoc);
        DestroyRenderObjectAndClear(&g_BtlMenuRenderObjZeni);
    } else {
        if (g_BtlMenuKetcyTexturesLoaded != 0) {
            DestroyRenderObjectAndClear(&g_BtlMenuRenderObjKetcy2);
            g_BtlMenuKetcyTexturesLoaded = 0;
        }
        DestroyRenderObjectAndClear(&g_BtlMenuRenderObjColo);
        DestroyRenderObjectAndClear(&g_BtlMenuRenderObjZeni);
    }
}

#pragma intrinsic(abs, memcpy, memset)

/*
 * Submit every battle-UI batch list for the frame. Pass 0 carries the window
 * texture and the text, gauge and cursor batches; pass 1 the overlays.
 */
// FUNCTION: FF7 0x006cee84
void __cdecl Battle_DrawUiLists(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (g_MenuIsSmallViewport320x240 != 1) {
        Gfx_SetScenePassCall(0, ctx);
        Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
        Gfx_DrawRenderObjectCall(g_MenuFlatRectGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuWindowBgGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinABlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinBBlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuWinCBlend4DiffGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinDBlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuFontAGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuFontBGraphicsObject.batchList, ctx);
        Gfx_SetScenePassCall(1, ctx);
        Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuBandGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuBattleLimitGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuBlendWindowBgGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuGaugeBarGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinABlend0GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinABlend1GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend1GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinBBlend1GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinDBlend1GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuTextBoxQuadGraphicsObject, ctx);
        InvalidateBatchGeneration(g_MenuBandGraphicsObject);
        InvalidateBatchGeneration(g_MenuBattleLimitGraphicsObject);
        InvalidateBatchGeneration(g_MenuBlendWindowBgGraphicsObject);
        InvalidateBatchGeneration(g_MenuGaugeBarGraphicsObject);
        InvalidateBatchGeneration(g_MenuTextBoxQuadGraphicsObject);
    } else {
        Gfx_SetScenePassCall(0, ctx);
        Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
        Gfx_DrawRenderObjectCall(g_MenuFlatRectGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuWindowBgGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuWinBlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuFontBlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuWinCBlend4DiffGraphicsObject, ctx);
        Gfx_SetScenePassCall(1, ctx);
        Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuBandGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuBattleLimitGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuBlendWindowBgGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuGaugeBarGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinBlend0GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuWinBlend1GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuTextBoxQuadGraphicsObject, ctx);
        InvalidateBatchGeneration(g_MenuBandGraphicsObject);
        InvalidateBatchGeneration(g_MenuBattleLimitGraphicsObject);
        InvalidateBatchGeneration(g_MenuBlendWindowBgGraphicsObject);
        InvalidateBatchGeneration(g_MenuGaugeBarGraphicsObject);
        InvalidateBatchGeneration(g_MenuTextBoxQuadGraphicsObject);
    }
}

/*
 * Set the viewport origin and size for the scaling mode. Mode 0 is 320x240 at the origin, mode 1 is
 * the same size centred at (0xA0, 0x78), and anything else is 640x480 at the origin with
 * g_MenuIsSmallViewport320x240 cleared -- the same flag every 1x/2x pair in this file
 * branches on.
 */
// FUNCTION: FF7 0x006cf1f4
void __cdecl Menu_ApplyUiScalingViewport(void)
{
    int mode;

    mode = GetUiScalingMode();
    if (mode == 0) {
        g_MenuViewportX = 0;
        g_MenuViewportY = 0;
        g_MenuViewportWidth = 320;
        g_MenuViewportHeight = 240;
        g_MenuIsSmallViewport320x240 = 1;
    } else if (mode == 1) {
        g_MenuViewportX = 160;
        g_MenuViewportY = 120;
        g_MenuViewportWidth = 320;
        g_MenuViewportHeight = 240;
        g_MenuIsSmallViewport320x240 = 1;
    } else {
        g_MenuViewportX = 0;
        g_MenuViewportY = 0;
        g_MenuViewportWidth = 640;
        g_MenuViewportHeight = 480;
        g_MenuIsSmallViewport320x240 = 0;
    }
}

/*
 * Temporarily selects asset-source mode 0 and initializes the supplied load
 * descriptor fields, palette pointer and option bits. UI scaling mode 2 creates
 * six textured objects into handle slots 0 through 5; other modes create three
 * objects in slots 0, 1 and 4 and clear slots 2, 3 and 5.
 */
// FUNCTION: FF7 0x006cf2aa
void __cdecl Battle_LoadFontAndWindowTexturesInto(FontWindowHandles *handles, PfileLoadOptions *desc, int lgp)
{
    int saved;
    char *file;
    int mode;

    mode = GetUiScalingMode();
    saved = Pfile_GetAssetSourceMode();
    Pfile_SetAssetSourceMode(0);
    desc->sourceMode = 0;
    desc->file.fromArchive = 1;
    desc->file.archiveId = 4;
    desc->file.callback = 0;
    desc->directory = (char *)&g_BtlMenuTextureAuxFieldB;
    desc->flags |= 0x10;
    desc->extraFlags |= 1;
    desc->renderFlags |= 0x20;
    if (mode == 2) {
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)desc);
        // STRING: FF7 0x0091be0c
        file = "usfont_a_l.tim";
        handles->renderHandle4 = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)desc, file, (void *)lgp);
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)desc);
        // STRING: FF7 0x0091be1c
        file = "usfont_b_l.tim";
        handles->renderHandle5 = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)desc, file, (void *)lgp);
        desc->renderFlags &= ~0x20;
        desc->renderFlags |= 0x80;
        // STRING: FF7 0x0091be2c
        file = "btl_win_a_l.tim";
        Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, (PfileLoadOptions *)desc);
        handles->renderHandle0 = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)desc, file, (void *)lgp);
        // STRING: FF7 0x0091be3c
        file = "btl_win_b_l.tim";
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)desc);
        handles->renderHandle1 = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)desc, file, (void *)lgp);
        // STRING: FF7 0x0091be4c
        file = "btl_win_c_l.tim";
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)desc);
        handles->renderHandle2 = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)desc, file, (void *)lgp);
        // STRING: FF7 0x0091be5c
        file = "btl_win_d_l.tim";
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)desc);
        handles->renderHandle3 = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)desc, file, (void *)lgp);
    } else {
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)desc);
        // STRING: FF7 0x0091be6c
        file = "usfont_l.tim";
        handles->renderHandle4 = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)desc, file, (void *)lgp);
        desc->renderFlags &= ~0x20;
        desc->renderFlags |= 0x80;
        // STRING: FF7 0x0091be7c
        file = "btl_win_l.tim";
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, (PfileLoadOptions *)desc);
        handles->renderHandle1 = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)desc, file, (void *)lgp);
        Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, (PfileLoadOptions *)desc);
        handles->renderHandle0 = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)desc, file, (void *)lgp);
        handles->renderHandle5 = 0;
        handles->renderHandle3 = 0;
        handles->renderHandle2 = 0;
    }
    Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, (PfileLoadOptions *)desc);
    handles->renderHandle6 = (int)Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_QUAD_2D, (PfileLoadOptions *)desc, 0, (void *)lgp);
    Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, (PfileLoadOptions *)desc);
    handles->renderHandle7 = (int)Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, (PfileLoadOptions *)desc, 0, (void *)lgp);
    Pfile_SetAssetSourceMode(saved);
    desc->sourceMode = saved;
}
} /* extern "C" */
