// Contents: Formats definition and log lines with sprintf into buffers and dumps driver
//   capabilities to the configured log path. It also holds a no-op dispatch guard for the raster
//   path.
// clang-format off
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include <io.h>
#include "lib/api/graphics.h"
#include <stdio.h>
extern "C" {

// GLOBAL: FF7 0x00917d88
char g_GfxLogNewline[8] = {10};
// GLOBAL: FF7 0x00917d90
int g_GfxOffsetErrorMsg = MAKE_FOURCC('E', 'R', 'R', 'O');
unsigned char g_rawData_00917d94[4] = {0x52, 0x3a, 0x20, 0x4f};
unsigned char g_rawData_00917d98[12] = {0x46, 0x46, 0x53, 0x45, 0x54, 0x20, 0x07, 0x20, 0x0a, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00917da4
int g_GfxFormatDefineLine = 0x66656423;
unsigned char g_rawData_00917da8[12] = {0x69, 0x6e, 0x65, 0x20, 0x25, 0x73, 0x20, 0x25, 0x64, 0x20, 0x0a, 0x00};
// GLOBAL: FF7 0x00917db4
int g_GfxFormatDefineVertexSize = 0x66656423;
// GLOBAL: FF7 0x00917db8
unsigned char g_GfxOffsetHeaderPathData[44] = {
    0x69, 0x6e, 0x65, 0x20, 0x25, 0x73, 0x20, 0x25, 0x64, 0x20, 0xa, 0,
    0x63, 0x3a, 0x2f, 0x6c, 0x69, 0x62, 0x2f, 0x68, 0x2f, 0x67, 0x72, 0x61,
    0x70, 0x68, 0x69, 0x63, 0x73, 0x2f, 0x73, 0x77, 0x2f, 0x6f, 0x66, 0x66,
    0x73, 0x65, 0x74, 0x2e, 0x68, 0x70, 0x70
};
// clang-format on

// clang-format off
#define g_GfxOffsetHeaderPathBuffer (*(char (*)[0x20])((char *)&g_GfxOffsetHeaderPathData + 12))
// clang-format on

/*
 * Formats a "#define <name> <value>" line for value and second into buffer.
 */
// FUNCTION: FF7 0x006af160
void __cdecl FormatDefineLine(const char *name, int second, void *buffer)
{
    sprintf((char *)buffer, (const char *)&g_GfxFormatDefineLine, name, second);
}

/*
 * Formats a vertex-size define line for value and second into buffer.
 */
// FUNCTION: FF7 0x006af17e
void __cdecl FormatDefineLineVertexSize(int value, int second, void *buffer)
{
    sprintf((char *)buffer, (const char *)&g_GfxFormatDefineVertexSize, value, second);
}

/*
 * Opens the configured output path and, if that succeeds, writes a fixed catalogue of graphics
 * structure offsets and vertex sizes as formatted define lines separated by
 * the shared blank-line string. Covers application framebuffer fields, byte
 * color and fixed-point layout, matrices, gradients, software triangle
 * interpolation and texture state, vertices, render/type/edge data and
 * software renderer tables.
 */
// FUNCTION: FF7 0x006af19c
void __cdecl Gfx_DumpDriverCapsToLog(void)
{
    struct {
        GfxDebugLogTarget *file;
        char *path;
        char line[200];
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.path = g_GfxOffsetHeaderPathBuffer;
    unlink(local.path);
    local.file = (GfxDebugLogTarget *)Gfx_CreateDebugLogTargetTruncating((int)local.path);
    if (local.file != 0) {
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917de4
            "APPLICATION_GRAPHICS_PITCH_OFFSET", 0x7a4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917e08
            "APPLICATION_GRAPHICS_PIXEL_ADDRESS_OFFSET", 0x7a8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917e34
            "BYTE_RGBA_COLOR_RED_OFFSET", 0x2, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917e50
            "BYTE_RGBA_COLOR_GREEN_OFFSET", 0x1, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917e70
            "BYTE_RGBA_COLOR_BLUE_OFFSET", 0x0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917e8c
            "BYTE_RGBA_COLOR_ALPHA_OFFSET", 0x3, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917eac
            "FP_32_DECIMAL_OFFSET", 0x0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917ec4
            "FP_32_INTEGER_OFFSET", 0x4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917edc
            "MATRIX_11", 0x0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917ee8
            "MATRIX_12", 0x4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917ef4
            "MATRIX_13", 0x8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917f00
            "MATRIX_14", 0xc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917f0c
            "MATRIX_21", 0x10, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917f18
            "MATRIX_22", 0x14, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917f24
            "MATRIX_23", 0x18, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917f30
            "MATRIX_24", 0x1c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917f3c
            "MATRIX_31", 0x20, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917f48
            "MATRIX_32", 0x24, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917f54
            "MATRIX_33", 0x28, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917f60
            "MATRIX_34", 0x2c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917f6c
            "MATRIX_41", 0x30, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917f78
            "MATRIX_42", 0x34, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917f84
            "MATRIX_43", 0x38, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917f90
            "MATRIX_44", 0x3c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917f9c
            "GRADIENT_DELTA_X_21", 0x0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917fb0
            "GRADIENT_DELTA_X_31", 0x4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917fc4
            "GRADIENT_DELTA_Y_21", 0x8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917fd8
            "GRADIENT_DELTA_Y_31", 0xc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00917fec
            "GRADIENT_X_GRADIENT_FACTOR", 0x10, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918008
            "GRADIENT_Y_GRADIENT_FACTOR", 0x14, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918024
            "GRADIENT_UNUSED_1", 0x18, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918038
            "GRADIENT_UNUSED_2", 0x1c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091804c
            "GRADIENT_DZ_DX", 0x20, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091805c
            "GRADIENT_DI_DX", 0x24, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091806c
            "GRADIENT_DR_DX", 0x28, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091807c
            "GRADIENT_DG_DX", 0x2c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091808c
            "GRADIENT_DB_DX", 0x30, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091809c
            "GRADIENT_DU_DX", 0x34, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009180ac
            "GRADIENT_DV_DX", 0x38, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009180bc
            "GRADIENT_DW_DX", 0x3c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009180cc
            "GRADIENT_DZ_DY", 0x40, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009180dc
            "GRADIENT_DI_DY", 0x44, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009180ec
            "GRADIENT_DR_DY", 0x48, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009180fc
            "GRADIENT_DG_DY", 0x4c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091810c
            "GRADIENT_DB_DY", 0x50, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091811c
            "GRADIENT_DU_DY", 0x54, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091812c
            "GRADIENT_DV_DY", 0x58, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091813c
            "GRADIENT_DW_DY", 0x5c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091814c
            "TRIANGLE_PIXEL_TOTAL_OFFSET", 0x0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918168
            "TRIANGLE_PITCH_OFFSET", 0x4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918180
            "TRIANGLE_SCREEN_START", 0x8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918198
            "TRIANGLE_Y_START", 0xc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009181ac
            "TRIANGLE_HEIGHT", 0x10, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009181bc
            "TRIANGLE_COLOR", 0x14, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009181cc
            "TRIANGLE_LEFT_X_DECIMAL", 0x18, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009181e4
            "TRIANGLE_LEFT_X_INTEGER", 0x1c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009181fc
            "TRIANGLE_LEFT_X_DECIMAL_INTERPOLATION_INCREMENT", 0x20, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091822c
            "TRIANGLE_LEFT_X_INTEGER_INTERPOLATION_INCREMENT", 0x24, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091825c
            "TRIANGLE_RIGHT_X_DECIMAL", 0x28, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918278
            "TRIANGLE_RIGHT_X_INTEGER", 0x2c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918294
            "TRIANGLE_RIGHT_X_DECIMAL_INTERPOLATION_INCREMENT", 0x30, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009182c8
            "TRIANGLE_RIGHT_X_INTEGER_INTERPOLATION_INCREMENT", 0x34, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009182fc
            "TRIANGLE_SCREEN_START_LINE_ADDRESS_OFFSET", 0x38, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918328
            "TRIANGLE_Z_PITCH", 0x3c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091833c
            "TRIANGLE_Z_START", 0x40, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918350
            "TRIANGLE_LEFT_EDGE_Z_DECIMAL", 0x44, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918370
            "TRIANGLE_LEFT_EDGE_Z_INTEGER", 0x48, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918390
            "TRIANGLE_LEFT_EDGE_Z_DECIMAL_INTERPOLATION_INCREMENT", 0x4c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009183c8
            "TRIANGLE_LEFT_EDGE_Z_INTEGER_INTERPOLATION_INCREMENT", 0x50, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918400
            "TRIANGLE_Z_DECIMAL", 0x54, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918414
            "TRIANGLE_Z_INTEGER", 0x58, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918428
            "TRIANGLE_DZ_DX_DECIMAL", 0x5c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918440
            "TRIANGLE_DZ_DX_INTEGER", 0x60, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918458
            "TRIANGLE_Z_LINE_START", 0x64, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918470
            "TRIANGLE_PIXEL_WIDTH", 0x68, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918488
            "TRIANGLE_R_DECIMAL", 0x8c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091849c
            "TRIANGLE_R_INTEGER", 0x90, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009184b0
            "TRIANGLE_DR_DX_DECIMAL", 0x94, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009184c8
            "TRIANGLE_DR_DX_INTEGER", 0x98, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009184e0
            "TRIANGLE_G_DECIMAL", 0x9c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009184f4
            "TRIANGLE_G_INTEGER", 0xa0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918508
            "TRIANGLE_DG_DX_DECIMAL", 0xa4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918520
            "TRIANGLE_DG_DX_INTEGER", 0xa8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918538
            "TRIANGLE_B_DECIMAL", 0xac, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091854c
            "TRIANGLE_B_INTEGER", 0xb0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918560
            "TRIANGLE_DB_DX_DECIMAL", 0xb4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918578
            "TRIANGLE_DB_DX_INTEGER", 0xb8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918590
            "TRIANGLE_LEFT_R_DECIMAL", 0xbc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009185a8
            "TRIANGLE_LEFT_R_INTEGER", 0xc0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009185c0
            "TRIANGLE_LEFT_R_DECIMAL_INTERPOLATION_INCREMENT", 0xc4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009185f0
            "TRIANGLE_LEFT_R_INTEGER_INTERPOLATION_INCREMENT", 0xc8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918620
            "TRIANGLE_LEFT_G_DECIMAL", 0xcc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918638
            "TRIANGLE_LEFT_G_INTEGER", 0xd0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918650
            "TRIANGLE_LEFT_G_DECIMAL_INTERPOLATION_INCREMENT", 0xd4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918680
            "TRIANGLE_LEFT_G_INTEGER_INTERPOLATION_INCREMENT", 0xd8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009186b0
            "TRIANGLE_LEFT_B_DECIMAL", 0xdc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009186c8
            "TRIANGLE_LEFT_B_INTEGER", 0xe0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009186e0
            "TRIANGLE_LEFT_B_DECIMAL_INTERPOLATION_INCREMENT", 0xe4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918710
            "TRIANGLE_LEFT_B_INTEGER_INTERPOLATION_INCREMENT", 0xe8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918740
            "TRIANGLE_U_DECIMAL", 0x6c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918754
            "TRIANGLE_U_INTEGER", 0x70, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918768
            "TRIANGLE_DU_DX_DECIMAL", 0x74, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918780
            "TRIANGLE_DU_DX_INTEGER", 0x78, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918798
            "TRIANGLE_V_DECIMAL", 0x7c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009187ac
            "TRIANGLE_V_INTEGER", 0x80, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009187c0
            "TRIANGLE_DV_DX_DECIMAL", 0x84, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009187d8
            "TRIANGLE_DV_DX_INTEGER", 0x88, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009187f0
            "TRIANGLE_LEFT_U_DECIMAL", 0xec, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918808
            "TRIANGLE_LEFT_U_INTEGER", 0xf0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918820
            "TRIANGLE_LEFT_U_DECIMAL_INTERPOLATION_INCREMENT", 0xf4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918850
            "TRIANGLE_LEFT_U_INTEGER_INTERPOLATION_INCREMENT", 0xf8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918880
            "TRIANGLE_LEFT_V_DECIMAL", 0xfc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918898
            "TRIANGLE_LEFT_V_INTEGER", 0x100, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009188b0
            "TRIANGLE_LEFT_V_DECIMAL_INTERPOLATION_INCREMENT", 0x104, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009188e0
            "TRIANGLE_LEFT_V_INTEGER_INTERPOLATION_INCREMENT", 0x108, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918910
            "TRIANGLE_TEXTURE_BIT_WIDTH", 0x10c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091892c
            "TRIANGLE_TEXTURE_WIDTH_MASK", 0x110, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918948
            "TRIANGLE_TEXTURE_HEIGHT_MASK", 0x114, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918968
            "TRIANGLE_TEXTURE_ADDRESS", 0x118, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918984
            "TRIANGLE_LINE_TEXTURE_ADDRESS", 0x130, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009189a4
            "TRIANGLE_DX_TABLE_ADDRESS_START", 0x134, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009189c4
            "TRIANGLE_DX_TABLE_ADDRESS", 0x138, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009189e0
            "TRIANGLE_PALETTE_ADDRESS", 0x11c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009189fc
            "TRIANGLE_SEMI_TRANSPARENT_PALETTE_ADDRESS", 0x120, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918a28
            "TRIANGLE_RED_TABLE", 0x124, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918a3c
            "TRIANGLE_GREEN_TABLE", 0x128, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918a54
            "TRIANGLE_BLUE_TABLE", 0x12c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918a68
            "TRIANGLE_HALF_COLOR_TABLE", 0x13c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918a84
            "TRIANGLE_SATURATION_TABLE", 0x140, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918aa0
            "TRIANGLE_CONSTANT_Z", 0x144, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918ab4
            "TRIANGLE_RGBA_COLOR", 0x14c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918ac8
            "TRIANGLE_PIXEL_TO_RGB_TABLE", 0x150, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918ae4
            "TRIANGLE_ALPHA_TABLE", 0x154, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918afc
            "TRIANGLE_SAVE_ESP", 0x158, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918b10
            "TRIANGLE_ESP", 0x15c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918b20
            "TRIANGLE_ESP_STRUCTURE_OFFSET", 0x15c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918b40
            "TRIANGLE_STACK_RED_TABLE", 0x168, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918b5c
            "TRIANGLE_STACK_GREEN_TABLE", 0x578, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918b78
            "TRIANGLE_STACK_BLUE_TABLE", 0x988, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918b94
            "VERTEX_X_OFFSET", 0x0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918ba4
            "VERTEX_Y_OFFSET", 0x4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918bb4
            "VERTEX_Z_OFFSET", 0x8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918bc4
            "VERTEX_W_OFFSET", 0xc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918bd4
            "SW_VERTEX_X_OFFSET", 0x0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918be8
            "SW_VERTEX_Y_OFFSET", 0x4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918bfc
            "SW_VERTEX_Z_OFFSET", 0x8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918c10
            "SW_VERTEX_W_OFFSET", 0xc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918c24
            "SW_VERTEX_HW_OFFSET", 0x10, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918c38
            "SW_VERTEX_VIEW_X", 0x14, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918c4c
            "SW_VERTEX_VIEW_Y", 0x18, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918c60
            "SW_VERTEX_VIEW_Z", 0x1c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918c74
            "SW_VERTEX_VISIBLE", 0x20, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918c88
            "SW_VERTEX_CLIP_OFFSET", 0x24, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918ca0
            "SW_VERTEX_PIXEL_X_DECIMAL", 0x28, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918cbc
            "SW_VERTEX_PIXEL_X_INTEGER", 0x2c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918cd8
            "SW_VERTEX_PIXEL_Y_DECIMAL", 0x30, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918cf4
            "SW_VERTEX_PIXEL_Y_INTEGER", 0x34, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918d10
            "SW_VERTEX_VERTEX_POINTER", 0x38, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918d2c
            "SW_VERTEX_CONVERTED", 0x3c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918d40
            "SW_VERTEX_U_OFFSET", 0x40, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918d54
            "SW_VERTEX_V_OFFSET", 0x44, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918d68
            "SW_VERTEX_R_OFFSET", 0x48, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918d7c
            "SW_VERTEX_G_OFFSET", 0x4c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918d90
            "SW_VERTEX_B_OFFSET", 0x50, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918da4
            "SW_VERTEX_COLOR_OFFSET", 0x54, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918dbc
            "VERTEX_RENDER_DATA_OR_CLIP", 0x0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918dd8
            "VERTEX_RENDER_DATA_AND_CLIP", 0x4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918df4
            "VERTEX_RENDER_DATA_MAXIMUM_VERTICES", 0x8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918e18
            "VERTEX_RENDER_DATA_SW_VERTEX_ARRAY", 0xc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918e3c
            "VERTEX_RENDER_DATA_VIEW_PIXEL_X", 0x14, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918e5c
            "VERTEX_RENDER_DATA_VIEW_PIXEL_Y", 0x18, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918e7c
            "VERTEX_RENDER_DATA_VIEW_PIXEL_WIDTH", 0x1c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918ea0
            "VERTEX_RENDER_DATA_VIEW_PIXEL_HEIGHT", 0x20, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918ec8
            "VERTEX_RENDER_DATA_VIEW_PIXEL_RIGHT", 0x24, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918eec
            "VERTEX_RENDER_DATA_VIEW_PIXEL_BOTTOM", 0x28, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918f14
            "VERTEX_RENDER_DATA_WORLD_MATRIX", 0x2c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918f34
            "VERTEX_RENDER_DATA_SCREEN_MATRIX", 0x30, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918f58
            "VERTEX_RENDER_DATA_Z_OFFSET", 0x34, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918f74
            "VERTEX_RENDER_DATA_Z_RESOLUTION", 0x38, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918f94
            "VERTEX_RENDER_DATA_FLOAT_TEXTURE_WIDTH", 0x3c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918fbc
            "VERTEX_RENDER_DATA_FLOAT_TEXTURE_HEIGHT", 0x40, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918fe4
            "TRIANGLE_TYPE_DATA_TYPE", 0x0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00918ffc
            "TRIANGLE_TYPE_DATA_PALETTE", 0x4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919018
            "TRIANGLE_TYPE_DATA_COLOR_KEY", 0x8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919038
            "TRIANGLE_TYPE_DATA_INITIALIZE_FUNCTION", 0xc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919060
            "TRIANGLE_TYPE_DATA_SETUP_FUNCTION", 0x10, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919084
            "TRIANGLE_TYPE_DATA_SETUP_FUNCTION_2", 0x14, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009190a8
            "TRIANGLE_TYPE_DATA_DRAW_FUNCTION", 0x18, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009190cc
            "TRIANGLE_TYPE_DATA_VISIBLE_VERTEX_CONVERSION_FUNCTION", 0x1c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919104
            "TRIANGLE_TYPE_DATA_VERTEX_CONVERSION_FUNCTION", 0x20, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919134
            "TRIANGLE_TYPE_DATA_CLIP_VERTEX_FUNCTION", 0x24, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091915c
            "TRIANGLE_TYPE_DATA_DRAW_LINE_FUNCTION", 0x28, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919184
            "TRIANGLE_TYPE_DATA_DRAW_TRIANGLE_FUNCTION", 0x2c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009191b0
            "TRIANGLE_TYPE_DATA_DYNAMIC_POLYGON_DATA", 0x30, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009191d8
            "TRIANGLE_EDGE_TOP_INDEX", 0x0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009191f0
            "TRIANGLE_EDGE_BOTTOM_INDEX", 0x4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091920c
            "TRIANGLE_EDGE_DELTA_X", 0x8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919224
            "TRIANGLE_EDGE_POSITIVE_DELTA_Y", 0xc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919244
            "TRIANGLE_EDGE_HEIGHT", 0x10, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091925c
            "TRIANGLE_EDGE_FLOAT_X_INCREMENT", 0x14, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x0091927c
            "TRIANGLE_EDGE_X_INCREMENT", 0x18, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919298
            "SW_TRIANGLE_SCREEN_COORDINATES", 0x0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009192b8
            "SW_TRIANGLE_TOTAL_LEFT_EDGES", 0xc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009192d8
            "SW_TRIANGLE_TOTAL_RIGHT_EDGES", 0x10, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009192f8
            "SW_TRIANGLE_LEFT_EDGES", 0x14, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919310
            "SW_TRIANGLE_RIGHT_EDGES", 0x5c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919328
            "SW_TRIANGLE_MAXIMUM_SCAN_WIDTH", 0xa4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919348
            "SW_TRIANGLE_COLOR_KEY_16", 0xa8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919364
            "SW_DATA_LEFT_EDGE_VERTEX_TABLE", 0x0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919384
            "SW_DATA_RIGHT_EDGE_VERTEX_TABLE", 0xc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009193a4
            "SW_DATA_MIDDLE_VERTEX_TABLE", 0x18, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009193c0
            "SW_DATA_SW_TRIANGLE", 0x48, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009193d4
            "SW_DATA_GRADIENT", 0xf4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009193e8
            "SW_DATA_TRIANGLE_TYPE_DATA", 0x154, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919404
            "SW_DATA_VERTEX_RENDER_DATA", 0x18c, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919420
            "SW_DATA_TRIANGLE_CLIP", 0x190, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919438
            "SW_DATA_TRIANGLE_CLIP_Z", 0x1484, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919450
            "SW_DATA_SCREEN_MATRIX", 0x2778, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919468
            "SW_DATA_INT_TO_FLOAT", 0x27b8, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919480
            "SW_DATA_ONE_OVER_FLOAT", 0x31bc, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x00919498
            "SW_DATA_DX_TABLE", 0x3bc0, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009194ac
            "SW_DATA_DIVIDE_BY_THREE", 0x45c4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009194c4
            "SW_DATA_PIXEL_TO_RGB_TABLE", 0x48c4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLine(
            // STRING: FF7 0x009194e0
            "SW_DATA_ALPHA_TABLE", 0x448c4, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        FormatDefineLineVertexSize(
            // STRING: FF7 0x009194f4
            (int)"SW_VERTEX_SIZE", 0x60, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        FormatDefineLineVertexSize(
            // STRING: FF7 0x00919504
            (int)"D3D_VERTEX_SIZE", 0x20, (void *)local.line);
        WriteDebugLog(local.line, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        WriteDebugLog(g_GfxLogNewline, local.file);
        Gfx_DestroyDebugLogTarget(local.file);
    }
}

/*
 * Nine identical guards, each wrapping a call to Gfx_DebugPrint with
 * g_GfxOffsetErrorMsg under the constant-false DEBUG_TRACE test, so the
 * function has no observable effect. The repetition is kept as the original
 * has it.
 */
// FUNCTION: FF7 0x006b1ca4
void __cdecl Gfx_RasterNoOpDispatchGuards(void)
{
    if (DEBUG_TRACE != 0) {
        Gfx_DebugPrint((const char *)&g_GfxOffsetErrorMsg);
    }
    if (DEBUG_TRACE != 0) {
        Gfx_DebugPrint((const char *)&g_GfxOffsetErrorMsg);
    }
    if (DEBUG_TRACE != 0) {
        Gfx_DebugPrint((const char *)&g_GfxOffsetErrorMsg);
    }
    if (DEBUG_TRACE != 0) {
        Gfx_DebugPrint((const char *)&g_GfxOffsetErrorMsg);
    }
    if (DEBUG_TRACE != 0) {
        Gfx_DebugPrint((const char *)&g_GfxOffsetErrorMsg);
    }
    if (DEBUG_TRACE != 0) {
        Gfx_DebugPrint((const char *)&g_GfxOffsetErrorMsg);
    }
    if (DEBUG_TRACE != 0) {
        Gfx_DebugPrint((const char *)&g_GfxOffsetErrorMsg);
    }
    if (DEBUG_TRACE != 0) {
        Gfx_DebugPrint((const char *)&g_GfxOffsetErrorMsg);
    }
    if (DEBUG_TRACE != 0) {
        Gfx_DebugPrint((const char *)&g_GfxOffsetErrorMsg);
    }
}
} /* extern "C" */
