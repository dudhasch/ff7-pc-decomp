// src/ff7/src/menu/english/menu_quit_confirm_dialog.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The quit confirmation dialog: a two-choice box that polls input, with the viewport save
//   and restore it needs, the centred message and text batch list for menu dialogs, and the vector
//   scaling for its backdrop.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/menu.h"
#include "lib/pfile.h"
#include "lib/enums/win32.h"
#include "lib/codegen.h"
#include "lib/types/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "ff7/api/main.h"
#include "lib/api/file.h"
#include "lib/api/sound.h"
#include "lib/crt.h"
#include "lib/enums/file.h"
#include "lib/enums/graphics.h"
#include "lib/enums/sound.h"
#include "lib/types/polygon.h"
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"
#include "lib/types/mem.h"
#include "ff7/enums/main.h"
#include "ff7/enums/menu.h"
#include "ff7/savemap.h"
#include "ff7/types/battle_actors.h"
#include "ff7/field.h"
extern "C" {

// clang-format off
// clang-format on

struct EnglishWindowFrame;
// Character id to savemap party record; ids 9 and 10 share the records of 6 and 7. The trailing zeros
// are unreferenced padding up to the next object.
// GLOBAL: FF7 0x00919928
int g_CharIdToSaveSlot[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 6, 7, 0, 0, 0};
// clang-format off
// The quit dialog's two window rectangles (x, y, w, h) at 2x and 1x.
// GLOBAL: FF7 0x00919960
struct EnglishWindowFrame g_MenuQuitDialogRect2x = {120, 140, 400, 200};
// GLOBAL: FF7 0x00919968
struct EnglishWindowFrame g_MenuQuitDialogRect1x = {60, 70, 200, 100};
// The three lines of the quit question in field text, 0x1E bytes each ("Do you want to quit",
// "Playing Final Fantasy VII", "and return to Windows?"): Menu_QuitConfirmDialogFrame draws line i
// of 3 at g_MenuQuitDialogPromptText + i * 0x1E. The six zero bytes after the third line have no reader.
// Text: "Do you want to quit" "playing Final Fantasy VII" "and return to Windows?"
// GLOBAL: FF7 0x00919970
unsigned char g_MenuQuitDialogPromptText[0x60] = {
    0x24, 0x4f, 0x00, 0x59, 0x4f, 0x55, 0x00, 0x57, 0x41, 0x4e, 0x54, 0x00, 0x54, 0x4f, 0x00, 0x51, 0x55, 0x49, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x4c, 0x41, 0x59, 0x49, 0x4e, 0x47, 0x00, 0x26, 0x49, 0x4e, 0x41, 0x4c, 0x00, 0x26, 0x41, 0x4e, 0x54,
    0x41, 0x53, 0x59, 0x00, 0x36, 0x29, 0x29, 0xff, 0x00, 0x00, 0x00, 0x00, 0x41, 0x4e, 0x44, 0x00, 0x52, 0x45, 0x54, 0x55, 0x52, 0x4e, 0x00, 0x54,
    0x4f, 0x00, 0x37, 0x49, 0x4e, 0x44, 0x4f, 0x57, 0x53, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// The two answer labels, measured and drawn by Menu_QuitConfirmDialogFrame.
// Text: "Yes"
// GLOBAL: FF7 0x009199d0
unsigned char g_MenuQuitDialogYesText[4] = {0x39, 0x45, 0x53, 0xff};
// GLOBAL: FF7 0x009199d4
unsigned char g_MenuQuitDialogNoText[4] = {0x2e, 0x4f, 0xff, 0x00};
// Menu_DialogInitSession's origin record: x and y of the dialog centre (170.0f, 190.0f, or the viewport
// origin plus 85.0f/95.0f) and a third word it zeroes. Menu_ScaleAndOffsetThreeVectors adds it to each
// triangle record below. The 4 zero bytes after it pad to the 8-alignment of g_MenuQuitBackdropTriRecord_01.
// GLOBAL: FF7 0x009199d8
float g_MenuDialogOriginRecord[3] = {170.0f, 190.0f, 0.0f};
// GLOBAL: FF7 0x009199e8
float g_MenuQuitBackdropTriRecord_01[9] = {0.0f, -12.0f, 0.0f, -12.0f, 12.0f, 0.0f, 12.0f, 12.0f, 0.0f};
// GLOBAL: FF7 0x00919a10
float g_MenuQuitBackdropTriRecord_02[9] = {0.0f, -8.0f, 0.0f, -9.0f, 10.0f, 0.0f, 9.0f, 10.0f, 0.0f};
// GLOBAL: FF7 0x00919a38
float g_MenuQuitBackdropTriRecord_03[9] = {1.0f, 7.0f, 0.0f, -1.0f, 7.0f, 0.0f, 1.0f, 9.0f, 0.0f};
// GLOBAL: FF7 0x00919a60
float g_MenuQuitBackdropTriRecord_04[9] = {-1.0f, 7.0f, 0.0f, -1.0f, 9.0f, 0.0f, 1.0f, 9.0f, 0.0f};
// GLOBAL: FF7 0x00919a88
float g_MenuQuitBackdropTriRecord_05[9] = {-3.0f, -1.0f, 0.0f, -3.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f};
// GLOBAL: FF7 0x00919ab0
float g_MenuQuitBackdropTriRecord_06[9] = {-3.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f};
// GLOBAL: FF7 0x00919ad8
float g_MenuQuitBackdropTriRecord_07[9] = {-2.0f, -3.0f, 0.0f, -3.0f, -1.0f, 0.0f, 2.0f, -3.0f, 0.0f};
// GLOBAL: FF7 0x00919b00
float g_MenuQuitBackdropTriRecord_08[9] = {-3.0f, -1.0f, 0.0f, 3.0f, -1.0f, 0.0f, 2.0f, -3.0f, 0.0f};
// GLOBAL: FF7 0x00919b28
float g_MenuQuitBackdropTriRecord_09[9] = {1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 3.0f, -1.0f, 0.0f};
// GLOBAL: FF7 0x00919b50
float g_MenuQuitBackdropTriRecord_10[9] = {1.0f, 1.0f, 0.0f, 3.0f, 1.0f, 0.0f, 3.0f, -1.0f, 0.0f};
// GLOBAL: FF7 0x00919b78
float g_MenuQuitBackdropTriRecord_11[9] = {1.0f, 1.0f, 0.0f, -1.0f, 3.0f, 0.0f, 3.0f, 1.0f, 0.0f};
// GLOBAL: FF7 0x00919ba0
float g_MenuQuitBackdropTriRecord_12[9] = {-1.0f, 3.0f, 0.0f, 1.0f, 3.0f, 0.0f, 3.0f, 1.0f, 0.0f};
// GLOBAL: FF7 0x00919bc8
float g_MenuQuitBackdropTriRecord_13[9] = {-1.0f, 3.0f, 0.0f, -1.0f, 6.0f, 0.0f, 1.0f, 3.0f, 0.0f};
// GLOBAL: FF7 0x00919bf0
float g_MenuQuitBackdropTriRecord_14[9] = {-1.0f, 6.0f, 0.0f, 1.0f, 6.0f, 0.0f, 1.0f, 3.0f, 0.0f};
// GLOBAL: FF7 0x00dc0e78
int g_MenuDialogMessageBuffer = 0;
unsigned char g_bssPad_00dc0e7c[4] = {0};
unsigned char g_bssPad_00dc0e80[280] = {0};
// GLOBAL: FF7 0x00dc0f98
float g_MenuDepth = 0.0f;
unsigned char g_bssPad_00dc0f9c[4] = {0};
// GLOBAL: FF7 0x00dc0fa0
int g_MenuQuitConfirmCursor = 0;
// GLOBAL: FF7 0x00dc0fa4
int g_MenuSavedViewportLeft = 0;
// GLOBAL: FF7 0x00dc0fa8
int g_MenuSavedViewportTop = 0;
// GLOBAL: FF7 0x00dc0fac
int g_MenuSavedViewportWidth = 0;
// GLOBAL: FF7 0x00dc0fb0
int g_MenuSavedViewportHeight = 0;
// GLOBAL: FF7 0x00dc0fb4
int g_MenuQuitConfirmDone = 0;
// GLOBAL: FF7 0x00dc0fb8
int g_MenuQuitConfirmOpen = 0;
unsigned char g_bssPad_00dc0fbc[4] = {0};
// GLOBAL: FF7 0x00dc0fc0
GfxRenderObject *g_pMenuDialogBatchList = 0;
unsigned char g_bssPad_00dc0fc4[4] = {0};
// GLOBAL: FF7 0x00dc0fc8
GfxRenderObject *g_MenuWinABlend4GraphicsObject = 0;
// GLOBAL: FF7 0x00dc0fcc
GfxRenderObject *g_MenuWinBBlend4GraphicsObject = 0;
// GLOBAL: FF7 0x00dc0fd0
GfxRenderObject *g_MenuWinCBlend4GraphicsObject = 0;
// GLOBAL: FF7 0x00dc0fd4
GfxRenderObject *g_MenuWinDBlend4GraphicsObject = 0;
// GLOBAL: FF7 0x00dc0fd8
GfxRenderObject *g_MenuWinCBlend4DiffGraphicsObject = 0;
// GLOBAL: FF7 0x00dc0fdc
GfxRenderObject *g_MenuWinABlend0GraphicsObject = 0;
// GLOBAL: FF7 0x00dc0fe0
GfxRenderObject *g_MenuWinABlend1GraphicsObject = 0;
// GLOBAL: FF7 0x00dc0fe4
GfxRenderObject *g_MenuWinBBlend1GraphicsObject = 0;
// GLOBAL: FF7 0x00dc0fe8
GfxRenderObject *g_MenuWinCBlend1GraphicsObject = 0;
// GLOBAL: FF7 0x00dc0fec
GfxRenderObject *g_MenuWinDBlend1GraphicsObject = 0;
// GLOBAL: FF7 0x00dc0ff0
GfxRenderObject *g_MenuWindowBgGraphicsObject = 0;
// GLOBAL: FF7 0x00dc0ff4
GfxRenderObject *g_MenuBlendWindowBgGraphicsObject = 0;
// GLOBAL: FF7 0x00dc0ff8
GfxRenderObject *g_MenuGaugeBarGraphicsObject = 0;
// GLOBAL: FF7 0x00dc0ffc
GfxRenderObject *g_MenuFlatRectGraphicsObject = 0;
// GLOBAL: FF7 0x00dc1000
GfxRenderObject *g_MenuBandGraphicsObject = 0;
// GLOBAL: FF7 0x00dc1004
GfxRenderObject *g_MenuBattleLimitGraphicsObject = 0;
// GLOBAL: FF7 0x00dc1008
GfxRenderObject *g_MenuTextBoxQuadGraphicsObject = 0;
// GLOBAL: FF7 0x00dc100c
GfxRenderObject *g_MenuFontAGraphicsObject = 0;
// GLOBAL: FF7 0x00dc1010
MenuFontBGraphicsObject g_MenuFontBGraphicsObject = {0};
// GLOBAL: FF7 0x00dc1020
GfxRenderObject *g_MenuCharPortraitGraphicsObjects[9] = {0};
// GLOBAL: FF7 0x00dc1044
GfxRenderObject *g_MenuBusterTexGraphicsObject = 0;
// GLOBAL: FF7 0x00dc1048
GfxRenderObject *g_MenuFontBlend4GraphicsObject = 0;
// GLOBAL: FF7 0x00dc104c
GfxRenderObject *g_MenuWinBlend4GraphicsObject = 0;
// GLOBAL: FF7 0x00dc1050
GfxRenderObject *g_MenuWinBlend0GraphicsObject = 0;
// GLOBAL: FF7 0x00dc1054
GfxRenderObject *g_MenuWinBlend1GraphicsObject = 0;
// clang-format on

/* Win32 user32/kernel32 imports with int HWND parameter matching this TU without <windows.h>. */
extern __declspec(dllimport) int __stdcall SendMessageA(int, unsigned int, unsigned int, long);
extern __declspec(dllimport) void __stdcall Sleep(unsigned long); /* kernel32 import without <windows.h>. */


// Scale three vectors by g_MenuDepth and offset each by the matching
// component of a fixed one: out[k] = a[k] * scale + b[k], nine times.
//
// The loop advances `out` and `a` by twelve bytes a turn and leaves `b`
// where it is, so the offset is one vector reused three times. Both
// cursors are the parameter slots themselves.

/*
 * Writes three consecutive output XYZ triples as each source triple times
 * g_MenuDepth plus the same unadvanced second vector. Advances output and the
 * first source by three floats after each iteration.
 */
// FUNCTION: FF7 0x006bf4e0
void __cdecl Menu_ScaleAndOffsetThreeVectors(float *out, float *srcVecs, float *offsetVec)
{
    int i;

    for (i = 0; i < 3; i++) {
        out[0] = srcVecs[0] * g_MenuDepth + offsetVec[0];
        out[1] = srcVecs[1] * g_MenuDepth + offsetVec[1];
        out[2] = srcVecs[2] * g_MenuDepth + offsetVec[2];
        out += 3;
        srcVecs += 3;
    }
}

/*
 * When acquisition of a 14-triangle batch succeeds, calls
 * Menu_ScaleAndOffsetThreeVectors for fourteen successive triangle records
 * beginning at g_MenuQuitBackdropTriRecord_01 (three XYZ corners each), with shared
 * g_MenuDialogOriginRecord input, and copies the resulting frame-held XY pairs
 * into three 32-byte vertices per triangle. Every vertex uses depth 0.01,
 * reciprocal W 1 and secondary color 0xFF000000.
 */
// FUNCTION: FF7 0x006bf559
void __cdecl Menu_DrawQuitConfirmBackdropTriangles(void)
{
    int scaledPts[3][3];
    GfxTlVertex *vert01Second;
    GfxTlVertex *firstVertex01Ptr;
    GfxTlVertex *pVertex02Zero;
    GfxTlVertex *vertex03SecondPtr;
    GfxTlVertex *vertex04ThirdPtr;
    GfxTlVertex *vertex07First;
    GfxTlVertex *thirdVert06;
    GfxTlVertex *vertex07SecondPtr;
    GfxTlVertex *vert09First;
    GfxTlVertex *vertex10FirstPtr;
    GfxTlVertex *firstVertex03Ptr;
    GfxTlVertex *vertex04SecondPtr;
    GfxTlVertex *firstVertex04;
    GfxTlVertex *vert06One;
    GfxTlVertex *vert06First;
    GfxTlVertex *vertex09Third;
    GfxTlVertex *vertex11FirstPtr;
    GfxTlVertex *vertex12Third;
    GfxTlVertex *vert14Second;
    GfxTlVertex *vert03Third;
    GfxTlVertex *vert05C;
    GfxTlVertex *vert01Third;
    GfxTlVertex *vertex02Third;
    GfxTlVertex *pVertex09Second;
    GfxTlVertex *thirdVertex10;
    GfxTlVertex *vertex05One;
    GfxTlVertex *pVertex08Third;
    GfxTlVertex *vertex14First2;
    GfxTlVertex *vertex02SecondPtr;
    GfxTlVertex *vert08One;
    GfxTlVertex *vert10Second;
    GfxTlVertex *firstVert05;
    GfxTlVertex *vertex08FirstPtr;
    GfxTlVertex *vertex13Second;
    GfxTlVertex *vertex14Two;
    GfxTlVertex *vertex12Second2;
    GfxTlVertex *firstVertex12;
    GfxTlVertex *pVertex07Third;
    GfxTlVertex *pVertex13A;
    GfxTlVertex *vert13Third;
    GfxTlVertex *thirdVertex11;
    GfxTlVertex *vertex11Second;

    if (AcquireBatchNode(0xE, g_pMenuDialogBatchList) != 0) {
        Menu_ScaleAndOffsetThreeVectors((float *)scaledPts, g_MenuQuitBackdropTriRecord_01, g_MenuDialogOriginRecord);
        firstVertex01Ptr = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor);
        firstVertex01Ptr->xBits = scaledPts[0][0];
        firstVertex01Ptr->yBits = scaledPts[0][1];
        firstVertex01Ptr->zBits = FLOAT_0_01_BITS;
        firstVertex01Ptr->rhwBits = FLOAT_ONE_BITS;
        firstVertex01Ptr->color = 0x80FF0000;
        firstVertex01Ptr->specular = 0xFF000000;
        vert01Second = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x20);
        vert01Second->xBits = scaledPts[1][0];
        vert01Second->yBits = scaledPts[1][1];
        vert01Second->zBits = FLOAT_0_01_BITS;
        vert01Second->rhwBits = FLOAT_ONE_BITS;
        vert01Second->color = 0x80800000;
        vert01Second->specular = 0xFF000000;
        vert01Third = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x40);
        vert01Third->xBits = scaledPts[2][0];
        vert01Third->yBits = scaledPts[2][1];
        vert01Third->zBits = FLOAT_0_01_BITS;
        vert01Third->rhwBits = FLOAT_ONE_BITS;
        vert01Third->color = 0x80000000;
        vert01Third->specular = 0xFF000000;
        g_pMenuDialogBatchList->vertexCursor += g_pMenuDialogBatchList->cursorStep;
        Menu_ScaleAndOffsetThreeVectors((float *)scaledPts, g_MenuQuitBackdropTriRecord_02, g_MenuDialogOriginRecord);
        pVertex02Zero = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor);
        pVertex02Zero->xBits = scaledPts[0][0];
        pVertex02Zero->yBits = scaledPts[0][1];
        pVertex02Zero->zBits = FLOAT_0_01_BITS;
        pVertex02Zero->rhwBits = FLOAT_ONE_BITS;
        pVertex02Zero->color = 0x80FFFFFF;
        pVertex02Zero->specular = 0xFF000000;
        vertex02SecondPtr = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x20);
        vertex02SecondPtr->xBits = scaledPts[1][0];
        vertex02SecondPtr->yBits = scaledPts[1][1];
        vertex02SecondPtr->zBits = FLOAT_0_01_BITS;
        vertex02SecondPtr->rhwBits = FLOAT_ONE_BITS;
        vertex02SecondPtr->color = 0x80808080;
        vertex02SecondPtr->specular = 0xFF000000;
        vertex02Third = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x40);
        vertex02Third->xBits = scaledPts[2][0];
        vertex02Third->yBits = scaledPts[2][1];
        vertex02Third->zBits = FLOAT_0_01_BITS;
        vertex02Third->rhwBits = FLOAT_ONE_BITS;
        vertex02Third->color = 0x80000000;
        vertex02Third->specular = 0xFF000000;
        g_pMenuDialogBatchList->vertexCursor += g_pMenuDialogBatchList->cursorStep;
        Menu_ScaleAndOffsetThreeVectors((float *)scaledPts, g_MenuQuitBackdropTriRecord_03, g_MenuDialogOriginRecord);
        firstVertex03Ptr = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor);
        firstVertex03Ptr->xBits = scaledPts[0][0];
        firstVertex03Ptr->yBits = scaledPts[0][1];
        firstVertex03Ptr->zBits = FLOAT_0_01_BITS;
        firstVertex03Ptr->rhwBits = FLOAT_ONE_BITS;
        firstVertex03Ptr->color = 0x80000000;
        firstVertex03Ptr->specular = 0xFF000000;
        vertex03SecondPtr = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x20);
        vertex03SecondPtr->xBits = scaledPts[1][0];
        vertex03SecondPtr->yBits = scaledPts[1][1];
        vertex03SecondPtr->zBits = FLOAT_0_01_BITS;
        vertex03SecondPtr->rhwBits = FLOAT_ONE_BITS;
        vertex03SecondPtr->color = 0x80000000;
        vertex03SecondPtr->specular = 0xFF000000;
        vert03Third = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x40);
        vert03Third->xBits = scaledPts[2][0];
        vert03Third->yBits = scaledPts[2][1];
        vert03Third->zBits = FLOAT_0_01_BITS;
        vert03Third->rhwBits = FLOAT_ONE_BITS;
        vert03Third->color = 0x80000000;
        vert03Third->specular = 0xFF000000;
        g_pMenuDialogBatchList->vertexCursor += g_pMenuDialogBatchList->cursorStep;
        Menu_ScaleAndOffsetThreeVectors((float *)scaledPts, g_MenuQuitBackdropTriRecord_04, g_MenuDialogOriginRecord);
        firstVertex04 = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor);
        firstVertex04->xBits = scaledPts[0][0];
        firstVertex04->yBits = scaledPts[0][1];
        firstVertex04->zBits = FLOAT_0_01_BITS;
        firstVertex04->rhwBits = FLOAT_ONE_BITS;
        firstVertex04->color = 0x80000000;
        firstVertex04->specular = 0xFF000000;
        vertex04SecondPtr = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x20);
        vertex04SecondPtr->xBits = scaledPts[1][0];
        vertex04SecondPtr->yBits = scaledPts[1][1];
        vertex04SecondPtr->zBits = FLOAT_0_01_BITS;
        vertex04SecondPtr->rhwBits = FLOAT_ONE_BITS;
        vertex04SecondPtr->color = 0x80000000;
        vertex04SecondPtr->specular = 0xFF000000;
        vertex04ThirdPtr = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x40);
        vertex04ThirdPtr->xBits = scaledPts[2][0];
        vertex04ThirdPtr->yBits = scaledPts[2][1];
        vertex04ThirdPtr->zBits = FLOAT_0_01_BITS;
        vertex04ThirdPtr->rhwBits = FLOAT_ONE_BITS;
        vertex04ThirdPtr->color = 0x80000000;
        vertex04ThirdPtr->specular = 0xFF000000;
        g_pMenuDialogBatchList->vertexCursor += g_pMenuDialogBatchList->cursorStep;
        Menu_ScaleAndOffsetThreeVectors((float *)scaledPts, g_MenuQuitBackdropTriRecord_05, g_MenuDialogOriginRecord);
        firstVert05 = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor);
        firstVert05->xBits = scaledPts[0][0];
        firstVert05->yBits = scaledPts[0][1];
        firstVert05->zBits = FLOAT_0_01_BITS;
        firstVert05->rhwBits = FLOAT_ONE_BITS;
        firstVert05->color = 0x80000000;
        firstVert05->specular = 0xFF000000;
        vertex05One = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x20);
        vertex05One->xBits = scaledPts[1][0];
        vertex05One->yBits = scaledPts[1][1];
        vertex05One->zBits = FLOAT_0_01_BITS;
        vertex05One->rhwBits = FLOAT_ONE_BITS;
        vertex05One->color = 0x80000000;
        vertex05One->specular = 0xFF000000;
        vert05C = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x40);
        vert05C->xBits = scaledPts[2][0];
        vert05C->yBits = scaledPts[2][1];
        vert05C->zBits = FLOAT_0_01_BITS;
        vert05C->rhwBits = FLOAT_ONE_BITS;
        vert05C->color = 0x80000000;
        vert05C->specular = 0xFF000000;
        g_pMenuDialogBatchList->vertexCursor += g_pMenuDialogBatchList->cursorStep;
        Menu_ScaleAndOffsetThreeVectors((float *)scaledPts, g_MenuQuitBackdropTriRecord_06, g_MenuDialogOriginRecord);
        vert06First = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor);
        vert06First->xBits = scaledPts[0][0];
        vert06First->yBits = scaledPts[0][1];
        vert06First->zBits = FLOAT_0_01_BITS;
        vert06First->rhwBits = FLOAT_ONE_BITS;
        vert06First->color = 0x80000000;
        vert06First->specular = 0xFF000000;
        vert06One = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x20);
        vert06One->xBits = scaledPts[1][0];
        vert06One->yBits = scaledPts[1][1];
        vert06One->zBits = FLOAT_0_01_BITS;
        vert06One->rhwBits = FLOAT_ONE_BITS;
        vert06One->color = 0x80000000;
        vert06One->specular = 0xFF000000;
        thirdVert06 = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x40);
        thirdVert06->xBits = scaledPts[2][0];
        thirdVert06->yBits = scaledPts[2][1];
        thirdVert06->zBits = FLOAT_0_01_BITS;
        thirdVert06->rhwBits = FLOAT_ONE_BITS;
        thirdVert06->color = 0x80000000;
        thirdVert06->specular = 0xFF000000;
        g_pMenuDialogBatchList->vertexCursor += g_pMenuDialogBatchList->cursorStep;
        Menu_ScaleAndOffsetThreeVectors((float *)scaledPts, g_MenuQuitBackdropTriRecord_07, g_MenuDialogOriginRecord);
        vertex07First = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor);
        vertex07First->xBits = scaledPts[0][0];
        vertex07First->yBits = scaledPts[0][1];
        vertex07First->zBits = FLOAT_0_01_BITS;
        vertex07First->rhwBits = FLOAT_ONE_BITS;
        vertex07First->color = 0x80000000;
        vertex07First->specular = 0xFF000000;
        vertex07SecondPtr = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x20);
        vertex07SecondPtr->xBits = scaledPts[1][0];
        vertex07SecondPtr->yBits = scaledPts[1][1];
        vertex07SecondPtr->zBits = FLOAT_0_01_BITS;
        vertex07SecondPtr->rhwBits = FLOAT_ONE_BITS;
        vertex07SecondPtr->color = 0x80000000;
        vertex07SecondPtr->specular = 0xFF000000;
        pVertex07Third = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x40);
        pVertex07Third->xBits = scaledPts[2][0];
        pVertex07Third->yBits = scaledPts[2][1];
        pVertex07Third->zBits = FLOAT_0_01_BITS;
        pVertex07Third->rhwBits = FLOAT_ONE_BITS;
        pVertex07Third->color = 0x80000000;
        pVertex07Third->specular = 0xFF000000;
        g_pMenuDialogBatchList->vertexCursor += g_pMenuDialogBatchList->cursorStep;
        Menu_ScaleAndOffsetThreeVectors((float *)scaledPts, g_MenuQuitBackdropTriRecord_08, g_MenuDialogOriginRecord);
        vertex08FirstPtr = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor);
        vertex08FirstPtr->xBits = scaledPts[0][0];
        vertex08FirstPtr->yBits = scaledPts[0][1];
        vertex08FirstPtr->zBits = FLOAT_0_01_BITS;
        vertex08FirstPtr->rhwBits = FLOAT_ONE_BITS;
        vertex08FirstPtr->color = 0x80000000;
        vertex08FirstPtr->specular = 0xFF000000;
        vert08One = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x20);
        vert08One->xBits = scaledPts[1][0];
        vert08One->yBits = scaledPts[1][1];
        vert08One->zBits = FLOAT_0_01_BITS;
        vert08One->rhwBits = FLOAT_ONE_BITS;
        vert08One->color = 0x80000000;
        vert08One->specular = 0xFF000000;
        pVertex08Third = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x40);
        pVertex08Third->xBits = scaledPts[2][0];
        pVertex08Third->yBits = scaledPts[2][1];
        pVertex08Third->zBits = FLOAT_0_01_BITS;
        pVertex08Third->rhwBits = FLOAT_ONE_BITS;
        pVertex08Third->color = 0x80000000;
        pVertex08Third->specular = 0xFF000000;
        g_pMenuDialogBatchList->vertexCursor += g_pMenuDialogBatchList->cursorStep;
        Menu_ScaleAndOffsetThreeVectors((float *)scaledPts, g_MenuQuitBackdropTriRecord_09, g_MenuDialogOriginRecord);
        vert09First = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor);
        vert09First->xBits = scaledPts[0][0];
        vert09First->yBits = scaledPts[0][1];
        vert09First->zBits = FLOAT_0_01_BITS;
        vert09First->rhwBits = FLOAT_ONE_BITS;
        vert09First->color = 0x80000000;
        vert09First->specular = 0xFF000000;
        pVertex09Second = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x20);
        pVertex09Second->xBits = scaledPts[1][0];
        pVertex09Second->yBits = scaledPts[1][1];
        pVertex09Second->zBits = FLOAT_0_01_BITS;
        pVertex09Second->rhwBits = FLOAT_ONE_BITS;
        pVertex09Second->color = 0x80000000;
        pVertex09Second->specular = 0xFF000000;
        vertex09Third = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x40);
        vertex09Third->xBits = scaledPts[2][0];
        vertex09Third->yBits = scaledPts[2][1];
        vertex09Third->zBits = FLOAT_0_01_BITS;
        vertex09Third->rhwBits = FLOAT_ONE_BITS;
        vertex09Third->color = 0x80000000;
        vertex09Third->specular = 0xFF000000;
        g_pMenuDialogBatchList->vertexCursor += g_pMenuDialogBatchList->cursorStep;
        Menu_ScaleAndOffsetThreeVectors((float *)scaledPts, g_MenuQuitBackdropTriRecord_10, g_MenuDialogOriginRecord);
        vertex10FirstPtr = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor);
        vertex10FirstPtr->xBits = scaledPts[0][0];
        vertex10FirstPtr->yBits = scaledPts[0][1];
        vertex10FirstPtr->zBits = FLOAT_0_01_BITS;
        vertex10FirstPtr->rhwBits = FLOAT_ONE_BITS;
        vertex10FirstPtr->color = 0x80000000;
        vertex10FirstPtr->specular = 0xFF000000;
        vert10Second = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x20);
        vert10Second->xBits = scaledPts[1][0];
        vert10Second->yBits = scaledPts[1][1];
        vert10Second->zBits = FLOAT_0_01_BITS;
        vert10Second->rhwBits = FLOAT_ONE_BITS;
        vert10Second->color = 0x80000000;
        vert10Second->specular = 0xFF000000;
        thirdVertex10 = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x40);
        thirdVertex10->xBits = scaledPts[2][0];
        thirdVertex10->yBits = scaledPts[2][1];
        thirdVertex10->zBits = FLOAT_0_01_BITS;
        thirdVertex10->rhwBits = FLOAT_ONE_BITS;
        thirdVertex10->color = 0x80000000;
        thirdVertex10->specular = 0xFF000000;
        g_pMenuDialogBatchList->vertexCursor += g_pMenuDialogBatchList->cursorStep;
        Menu_ScaleAndOffsetThreeVectors((float *)scaledPts, g_MenuQuitBackdropTriRecord_11, g_MenuDialogOriginRecord);
        vertex11FirstPtr = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor);
        vertex11FirstPtr->xBits = scaledPts[0][0];
        vertex11FirstPtr->yBits = scaledPts[0][1];
        vertex11FirstPtr->zBits = FLOAT_0_01_BITS;
        vertex11FirstPtr->rhwBits = FLOAT_ONE_BITS;
        vertex11FirstPtr->color = 0x80000000;
        vertex11FirstPtr->specular = 0xFF000000;
        vertex11Second = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x20);
        vertex11Second->xBits = scaledPts[1][0];
        vertex11Second->yBits = scaledPts[1][1];
        vertex11Second->zBits = FLOAT_0_01_BITS;
        vertex11Second->rhwBits = FLOAT_ONE_BITS;
        vertex11Second->color = 0x80000000;
        vertex11Second->specular = 0xFF000000;
        thirdVertex11 = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x40);
        thirdVertex11->xBits = scaledPts[2][0];
        thirdVertex11->yBits = scaledPts[2][1];
        thirdVertex11->zBits = FLOAT_0_01_BITS;
        thirdVertex11->rhwBits = FLOAT_ONE_BITS;
        thirdVertex11->color = 0x80000000;
        thirdVertex11->specular = 0xFF000000;
        g_pMenuDialogBatchList->vertexCursor += g_pMenuDialogBatchList->cursorStep;
        Menu_ScaleAndOffsetThreeVectors((float *)scaledPts, g_MenuQuitBackdropTriRecord_12, g_MenuDialogOriginRecord);
        firstVertex12 = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor);
        firstVertex12->xBits = scaledPts[0][0];
        firstVertex12->yBits = scaledPts[0][1];
        firstVertex12->zBits = FLOAT_0_01_BITS;
        firstVertex12->rhwBits = FLOAT_ONE_BITS;
        firstVertex12->color = 0x80000000;
        firstVertex12->specular = 0xFF000000;
        vertex12Second2 = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x20);
        vertex12Second2->xBits = scaledPts[1][0];
        vertex12Second2->yBits = scaledPts[1][1];
        vertex12Second2->zBits = FLOAT_0_01_BITS;
        vertex12Second2->rhwBits = FLOAT_ONE_BITS;
        vertex12Second2->color = 0x80000000;
        vertex12Second2->specular = 0xFF000000;
        vertex12Third = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x40);
        vertex12Third->xBits = scaledPts[2][0];
        vertex12Third->yBits = scaledPts[2][1];
        vertex12Third->zBits = FLOAT_0_01_BITS;
        vertex12Third->rhwBits = FLOAT_ONE_BITS;
        vertex12Third->color = 0x80000000;
        vertex12Third->specular = 0xFF000000;
        g_pMenuDialogBatchList->vertexCursor += g_pMenuDialogBatchList->cursorStep;
        Menu_ScaleAndOffsetThreeVectors((float *)scaledPts, g_MenuQuitBackdropTriRecord_13, g_MenuDialogOriginRecord);
        pVertex13A = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor);
        pVertex13A->xBits = scaledPts[0][0];
        pVertex13A->yBits = scaledPts[0][1];
        pVertex13A->zBits = FLOAT_0_01_BITS;
        pVertex13A->rhwBits = FLOAT_ONE_BITS;
        pVertex13A->color = 0x80000000;
        pVertex13A->specular = 0xFF000000;
        vertex13Second = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x20);
        vertex13Second->xBits = scaledPts[1][0];
        vertex13Second->yBits = scaledPts[1][1];
        vertex13Second->zBits = FLOAT_0_01_BITS;
        vertex13Second->rhwBits = FLOAT_ONE_BITS;
        vertex13Second->color = 0x80000000;
        vertex13Second->specular = 0xFF000000;
        vert13Third = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x40);
        vert13Third->xBits = scaledPts[2][0];
        vert13Third->yBits = scaledPts[2][1];
        vert13Third->zBits = FLOAT_0_01_BITS;
        vert13Third->rhwBits = FLOAT_ONE_BITS;
        vert13Third->color = 0x80000000;
        vert13Third->specular = 0xFF000000;
        g_pMenuDialogBatchList->vertexCursor += g_pMenuDialogBatchList->cursorStep;
        Menu_ScaleAndOffsetThreeVectors((float *)scaledPts, g_MenuQuitBackdropTriRecord_14, g_MenuDialogOriginRecord);
        vertex14First2 = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor);
        vertex14First2->xBits = scaledPts[0][0];
        vertex14First2->yBits = scaledPts[0][1];
        vertex14First2->zBits = FLOAT_0_01_BITS;
        vertex14First2->rhwBits = FLOAT_ONE_BITS;
        vertex14First2->color = 0x80000000;
        vertex14First2->specular = 0xFF000000;
        vert14Second = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x20);
        vert14Second->xBits = scaledPts[1][0];
        vert14Second->yBits = scaledPts[1][1];
        vert14Second->zBits = FLOAT_0_01_BITS;
        vert14Second->rhwBits = FLOAT_ONE_BITS;
        vert14Second->color = 0x80000000;
        vert14Second->specular = 0xFF000000;
        vertex14Two = (GfxTlVertex *)(g_pMenuDialogBatchList->vertexCursor + 0x40);
        vertex14Two->xBits = scaledPts[2][0];
        vertex14Two->yBits = scaledPts[2][1];
        vertex14Two->zBits = FLOAT_0_01_BITS;
        vertex14Two->rhwBits = FLOAT_ONE_BITS;
        vertex14Two->color = 0x80000000;
        vertex14Two->specular = 0xFF000000;
    }
}

/*
 * Set the viewport origin and size from the UI scaling mode, and record whether
 * the game is running at the low resolution. Mode 0 is 320x240 at the origin, mode
 * 1 the same size offset to the middle of a 640x480 screen, and anything else is
 * 640x480 itself.
 */
// FUNCTION: FF7 0x006c052c
void __cdecl Menu_DialogApplyUiScalingViewport(void)
{
    int mode;

    mode = GetUiScalingMode();
    if (mode == 0) {
        g_MenuViewportX = 0;
        g_MenuViewportY = 0;
        g_MenuViewportWidth = 0x140;
        g_MenuViewportHeight = 0xf0;
        g_MenuIsSmallViewport320x240 = 1;
    } else if (mode == 1) {
        g_MenuViewportX = 0xa0;
        g_MenuViewportY = 0x78;
        g_MenuViewportWidth = 0x140;
        g_MenuViewportHeight = 0xf0;
        g_MenuIsSmallViewport320x240 = 1;
    } else {
        g_MenuViewportX = 0;
        g_MenuViewportY = 0;
        g_MenuViewportWidth = 0x280;
        g_MenuViewportHeight = 0x1e0;
        g_MenuIsSmallViewport320x240 = 0;
    }
}

/*
 * Draws a two-choice confirmation box and reads its input. On first entry the selection at
 * g_MenuQuitConfirmCursor (0x00dc0fa0) starts at 1 (the right-hand choice) and g_MenuQuitConfirmOpen
 * marks the box open.
 */
// FUNCTION: FF7 0x006c05e2
void __cdecl Menu_QuitConfirmDialogFrame(void)
{
    int leftX;
    int rightX;
    int textWidth;
    int i;
    int arrowX;

    if (g_MenuQuitConfirmOpen != 1) {
        g_MenuQuitConfirmCursor = 1;
        g_MenuQuitConfirmOpen = 1;
    }
    Menu_DrawQuitConfirmBackdropTriangles();
    if (g_MenuIsSmallViewport320x240 != 1) {
        Menu_DrawWindowFrame2x(&g_MenuQuitDialogRect2x, 0.1f);
        for (i = 0; i < 3; i++) {
            Menu_DrawEncodedString2x(g_MenuQuitDialogRect2x.x + 0x64,
                              g_MenuQuitDialogRect2x.y + i * 0x1a + 0x12,
                              (unsigned char *)&g_MenuQuitDialogPromptText[i * 0x1e], 7,
                              0.01f);
        }
        textWidth = Menu_MeasureEncodedString2x((unsigned char *)&g_MenuQuitDialogYesText);
        leftX = g_MenuQuitDialogRect2x.x + 0x64 - textWidth / 4;
        Menu_DrawEncodedString2x(leftX, g_MenuQuitDialogRect2x.y + 0x9c,
                          (unsigned char *)&g_MenuQuitDialogYesText,
                          g_MenuQuitConfirmCursor ? 0 : 7, 0.01f);
        textWidth = Menu_MeasureEncodedString2x((unsigned char *)&g_MenuQuitDialogNoText);
        rightX =
            g_MenuQuitDialogRect2x.x + g_MenuQuitDialogRect2x.width - 0x64 - textWidth / 4;
        Menu_DrawEncodedString2x(rightX, g_MenuQuitDialogRect2x.y + 0x9c,
                          (unsigned char *)&g_MenuQuitDialogNoText,
                          g_MenuQuitConfirmCursor ? 7 : 0, 0.01f);
        arrowX = g_MenuQuitConfirmCursor ? rightX - 0x32 : leftX - 0x32;
        Menu_DrawMoreTextArrow2x(arrowX, g_MenuQuitDialogRect2x.y + 0xa4, 0.001f);
    } else {
        Menu_DrawWindowFrame1x(&g_MenuQuitDialogRect1x, 0.1f);
        for (i = 0; i < 3; i++) {
            Menu_DrawEncodedString1x(g_MenuQuitDialogRect1x.x + 0x32,
                                 g_MenuQuitDialogRect1x.y + i * 0xd + 9,
                                 (unsigned char *)&g_MenuQuitDialogPromptText[i * 0x1e], 7,
                                 0.01f);
        }
        textWidth = Menu_MeasureEncodedString1x((unsigned char *)&g_MenuQuitDialogYesText);
        leftX = g_MenuQuitDialogRect1x.x + 0x32 - textWidth / 4;
        Menu_DrawEncodedString1x(leftX, g_MenuQuitDialogRect1x.y + 0x4e,
                             (unsigned char *)&g_MenuQuitDialogYesText,
                             g_MenuQuitConfirmCursor ? 0 : 7, 0.01f);
        textWidth = Menu_MeasureEncodedString1x((unsigned char *)&g_MenuQuitDialogNoText);
        rightX =
            g_MenuQuitDialogRect1x.x + g_MenuQuitDialogRect1x.width - 0x32 - textWidth / 4;
        Menu_DrawEncodedString1x(rightX, g_MenuQuitDialogRect1x.y + 0x4e,
                             (unsigned char *)&g_MenuQuitDialogNoText,
                             g_MenuQuitConfirmCursor ? 7 : 0, 0.01f);
        arrowX = g_MenuQuitConfirmCursor ? rightX - 0x19 : leftX - 0x19;
        Menu_DrawMoreTextArrow1x(arrowX, g_MenuQuitDialogRect1x.y + 0x52, 0.001f);
    }
    if (BtlMenu_GetInputRepeat(INPUT_LEFT) || BtlMenu_GetInputRepeat(INPUT_RIGHT)) {
        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
        g_MenuQuitConfirmCursor ^= 1;
    }
    if (Input_GetPressed(INPUT_CIRCLE)) {
        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
        g_MenuQuitConfirmDone = 1;
        g_MenuQuitConfirmOpen = 0;
    }
}

/*
 * Copies the menu viewport rectangle (clipLeft, clipTop, viewSizeX, viewSizeY) out of the engine context
 * into g_MenuSavedViewportLeft..g_MenuSavedViewportHeight.
 */
// FUNCTION: FF7 0x006c0954
void __cdecl Menu_SaveViewportRect(GfxEngineContext *engineContext)
{
    g_MenuSavedViewportLeft = engineContext->clipLeft;
    g_MenuSavedViewportTop = engineContext->clipTop;
    g_MenuSavedViewportWidth = engineContext->viewSizeX;
    g_MenuSavedViewportHeight = engineContext->viewSizeY;
}

/*
 * Applies the saved menu viewport rectangle g_MenuSavedViewportLeft..g_MenuSavedViewportHeight to the
 * context.
 */
// FUNCTION: FF7 0x006c0994
void __cdecl Menu_RestoreViewportRect(GfxEngineContext *engineContext)
{
    GfxSetViewportRect(g_MenuSavedViewportLeft, g_MenuSavedViewportTop, g_MenuSavedViewportWidth, g_MenuSavedViewportHeight, engineContext);
}

/*
 * Temporarily configures the engine/menu context, encodes message into g_MenuDialogMessageBuffer
 * and calls the two resource helpers. Sets a black background and clears the
 * frame.
 */
// FUNCTION: FF7 0x006c09c0
void __cdecl Menu_DialogShowCenteredMessage(int message)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[32];
        int boxWidth;
        struct { unsigned char pad_to_4[0x4]; GfxClearColorBgra bgColor; };
        struct { unsigned char pad_to_14[0x14]; GfxEngineContext *engineContext; };
        struct { unsigned char pad_to_18[0x18]; EnglishWindowFrame rect; };
    } frame;
#pragma pack(pop)

    frame.engineContext = GetEngineContext();
    Menu_SaveViewportRect(frame.engineContext);
    Menu_DialogApplyUiScalingViewport();
    Text_EncodeAsciiStringRebuildingTables((unsigned char *)&g_MenuDialogMessageBuffer, (char *)message);
    Gfx_AdvanceDrawPoolGeneration(frame.engineContext->drawPool);
    Gfx_ResetRenderObjectPool(frame.engineContext->renderObjectBank);
    frame.bgColor.r = 0;
    frame.bgColor.g = 0;
    frame.bgColor.b = 0;
    frame.bgColor.a = FLOAT_ONE_BITS;
    GfxSetBackgroundColorCall(&frame.bgColor, frame.engineContext);
    GfxClearCall(1, 1, 0, frame.engineContext);
    if (((int (__cdecl *)(int, struct GfxEngineContext *))Gfx_BeginSceneCall)(0, frame.engineContext) != 0) { /* reads eax after a void call, as the original does */
        if (g_MenuIsSmallViewport320x240 != 1) {
            frame.boxWidth = Menu_MeasureEncodedString2x((unsigned char *)&g_MenuDialogMessageBuffer) + 0x14;
            Menu_SetRectShorts(&frame.rect.x, 0x140 - (frame.boxWidth / 2), 0xD7, frame.boxWidth, 0x33);
            Menu_DrawWindowFrame2x(&frame.rect, 0.1f);
            Menu_DrawEncodedString2x(frame.rect.x + 0xC, frame.rect.y + 0xE, (unsigned char *)&g_MenuDialogMessageBuffer, 7, 0);
        } else {
            frame.boxWidth = Menu_MeasureEncodedString1x((unsigned char *)&g_MenuDialogMessageBuffer) + 0xA;
            Menu_SetRectShorts(&frame.rect.x, 0xA0 - (frame.boxWidth / 2), 0x6B, frame.boxWidth, 0x1A);
            Menu_DrawWindowFrame1x(&frame.rect, 0.1f);
            Menu_DrawEncodedString1x(frame.rect.x + 6, frame.rect.y + 7, (unsigned char *)&g_MenuDialogMessageBuffer, 7, 0);
        }
        Menu_DialogSubmitDrawLists();
        Gfx_EndSceneCall(frame.engineContext);
    }
    PresentFrame(frame.engineContext);
    Gfx_StatsClearRecord(frame.engineContext->statsRecord);
    Menu_RestoreViewportRect(frame.engineContext);
}

/*
 * Calls Menu_QuitConfirmDialogFrame.
 */
// FUNCTION: FF7 0x006c0b87
void __cdecl Menu_DialogDrawConfirmBoxThunk(void)
{
    Menu_QuitConfirmDialogFrame();
}

/*
 * Flushes menu primitive lists in scene pass 0 with render state 2: common lists
 * (g_pMenuDialogBatchList and the window background), then seven lists when g_MenuIsSmallViewport320x240 != 1 or three lists when
 * it equals 1. Switches to pass 1 and emits g_MenuWinABlend0GraphicsObject or g_MenuWinBlend0GraphicsObject
 * respectively, then restores the global viewport.
 */
// FUNCTION: FF7 0x006c0b91
void __cdecl Menu_DialogSubmitDrawLists(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    Gfx_SetScenePassCall(0, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_pMenuDialogBatchList, ctx);
    Gfx_EmitPrimitiveBatchList((void *)g_MenuWindowBgGraphicsObject, ctx);
    if (g_MenuIsSmallViewport320x240 != 1) {
        Gfx_EmitPrimitiveBatchList(g_MenuWinABlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuWinCBlend4DiffGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinBBlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuWinDBlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuFontAGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuFontBGraphicsObject.batchList, ctx);
    } else {
        Gfx_EmitPrimitiveBatchList((void *)g_MenuWinBlend4GraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuWinCBlend4DiffGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList((void *)g_MenuFontBlend4GraphicsObject, ctx);
    }
    Gfx_SetScenePassCall(1, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    if (g_MenuIsSmallViewport320x240 != 1) {
        Gfx_EmitPrimitiveBatchList(g_MenuWinABlend0GraphicsObject, ctx);
    } else {
        Gfx_EmitPrimitiveBatchList(g_MenuWinBlend0GraphicsObject, ctx);
    }
    GfxSetViewportRect(g_MenuViewportX, g_MenuViewportY, g_MenuViewportWidth, g_MenuViewportHeight, ctx);
}

/*
 * Initializes local render-object load options with blend mode four, explicitly
 * sets that blend mode again and creates a render object with arguments one/two
 * and the engine context's renderObjectBank. Stores the result in g_pMenuDialogBatchList;
 * unreferenced padding slots preserve VC5 /Od stack layout.
 */
// FUNCTION: FF7 0x006c0d23
void __cdecl Menu_DialogCreateTextBatchList(void)
{
    GfxEngineContext *pEngine;
    PfileLoadOptions loadOptions;

    pEngine = GetEngineContext();
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &loadOptions);
    Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &loadOptions);
    g_pMenuDialogBatchList = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_TRI_2D, &loadOptions, 0, (void *)pEngine->renderObjectBank);
}

/*
 * Releases the menu's text batch list g_pMenuDialogBatchList if it exists.
 */
// FUNCTION: FF7 0x006c0d72
void __cdecl Menu_DialogReleaseTextBatchList(void)
{
    if (g_pMenuDialogBatchList != 0) {
        DestroyRenderObjectAndClear(&g_pMenuDialogBatchList);
    }
}

/*
 * Recomputes the menu's scale factor and its centre from the viewport
 * Menu_DialogApplyUiScalingViewport has just set. The scale is 1.5 when
 * g_MenuIsSmallViewport320x240 is set and 3.0 when it is not.
 */
// FUNCTION: FF7 0x006c0d8d
void __cdecl Menu_DialogInitSession(void)
{
    Menu_DialogApplyUiScalingViewport();
    Menu_DialogCreateTextBatchList();
    g_MenuDepth = (g_MenuIsSmallViewport320x240 != 0) ? 1.5f : 3.0f;
    if (g_MenuIsSmallViewport320x240 != 1) {
        g_MenuDialogOriginRecord[0] = 170.0f;
        g_MenuDialogOriginRecord[1] = 190.0f;
        *(int *)&g_MenuDialogOriginRecord[2] = 0;
    } else {
        g_MenuDialogOriginRecord[0] = (float)g_MenuViewportX + 85.0f;
        g_MenuDialogOriginRecord[1] = (float)g_MenuViewportY + 95.0f;
        *(int *)&g_MenuDialogOriginRecord[2] = 0;
    }
    g_MenuQuitConfirmDone = 0;
    g_MenuQuitConfirmOpen = g_MenuQuitConfirmDone;
}

/*
 * Initializes the confirmation dialog, then repeatedly polls input, updates engine
 * resources, clears to opaque black, draws and flushes menu batches when scene
 * begin succeeds, presents, updates timers and sound fades until g_MenuQuitConfirmDone is
 * nonzero. If g_MenuQuitConfirmCursor is zero, sends WM_CLOSE to the engine window.
 */
// FUNCTION: FF7 0x006c0e2d
void __cdecl Menu_QuitConfirmDialogRun(struct GfxEngineContext *engineContext)
{
    float color[4];
    Menu_SaveViewportRect(engineContext);
    Menu_DialogInitSession();
    do {
        Input_Update(0);
        Gfx_AdvanceDrawPoolGeneration(engineContext->drawPool);
        Gfx_ResetRenderObjectPool(engineContext->renderObjectBank);
        color[2] = 0.0f;
        color[1] = 0.0f;
        color[0] = 0.0f;
        color[3] = 1.0f;
        GfxSetBackgroundColorCall(color, engineContext);
        GfxClearCall(1, 1, 0, engineContext);
        if (((int (__cdecl *)(int, struct GfxEngineContext *))Gfx_BeginSceneCall)(0, engineContext) != 0) { /* reads eax after a void call, as the original does */
            Menu_DialogDrawConfirmBoxThunk();
            Menu_DialogSubmitDrawLists();
            Gfx_EndSceneCall(engineContext);
        }
        PresentFrame(engineContext);
        Gfx_StatsClearRecord(engineContext->statsRecord);
        SoundUpdateFades();
    } while (g_MenuQuitConfirmDone == 0);
    if (g_MenuQuitConfirmCursor == 0) {
        SendMessageA(engineContext->hwnd, WM_CLOSE, engineContext->result, 0);
    }
    Menu_DialogReleaseTextBatchList();
    Input_Update(0);
    Sleep(250);
    Menu_RestoreViewportRect(engineContext);
}
} /* extern "C" */
