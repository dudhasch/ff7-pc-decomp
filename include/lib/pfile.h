/* PfileLoadOptions -- the 0x74-byte block the model, texture and sprite loaders
 * (Pfile_LoadRsdModelEx, Gfx_CreateRenderObject, Gfx_SptLoadRecord, the skeleton
 * and animation loaders) are configured with.
 *
 * The size is not inferred: Pfile_InitLoadOptions and
 * Pfile_InitLoadOptionsWithBlend clear the block with an explicit `push 0x74`.
 * Field summary by offset:
 *
 *   +0x00  flags: seeded from Pfile_GetDefaultModelFlags; loaders that set a
 *          directory also set 0x10.
 *   +0x04  copied to the loaded polygon set's drawEnabled (1 by default).
 *   +0x08  the asset source mode (Pfile_GetAssetSourceMode).
 *   +0x0C  non-zero makes Pfile_LoadRsdModelEx create a matrix array.
 *   +0x14  that array's matrix count; Gfx_CreateRenderObject enables the
 *          object when it is above zero.
 *   +0x18  the draw pool a render object takes instead of the engine
 *          context's (+0x300).
 *   +0x1C  the start of the block Pfile_ApplyBlendModeToHundreds reads
 *          (flags, then +0x20 blendMode).
 *   +0x24  the directory PushFileContextDirectory enters before loading
 *          (field "." , the minigame directories, the world texture paths).
 *   +0x28  a texture the caller owns: when set, the render object does not
 *          release its texture.
 *   +0x2C  the flags GfxSelectPrimitiveEmitters picks the emitters by.
 *   +0x30  the light group (Gfx_LightCreateThreeLightOwner's result in the
 *          field, the world's secondary light group).
 *   +0x38  the model scale, a float (seeded 0x3F800000, 1.0f).
 *   +0x40  the FileOpenRequest the loaders open files with: fromArchive 1 and
 *          archiveId the LGP index (0 char, 1 flevel, 2 battle, 3 magic,
 *          5 world, 6 condor, 7 chocobo, 8 high, 9 coaster, 10 snowboard,
 *          15 cr, 16 disc).
 */
#ifndef LIB_PFILE_H
#define LIB_PFILE_H

#include "lib/types/file.h"

typedef struct PfileLoadOptions {
    /* +0x00 */ int flags;
    /* +0x04 */ int drawEnabled;
    /* +0x08 */ int sourceMode;
    /* +0x0C */ int createMatrices;
    /* +0x10 */ union {
        int polygonFlags;
        int colorKeyEnabled;
    };
    /* +0x14 */ int matrixCount;
    /* +0x18 */ int drawPool;
    /* +0x1C */ int blendFlags;
    /* +0x20 */ int blendMode;
    /* +0x24 */ char *directory;
    union {
        /* +0x28 */ int texture;
        struct GfxTexHeader *texHeader;
    };
    /* +0x2C */ int emitterFlags;
    /* +0x30 */ int lightGroup;
    /* +0x34 */ int lightFlags;
    /* +0x38 */ float scale;
    /* +0x3C */ int loadBoneParts;
    /* +0x40 */ FileOpenRequest file;
    /* +0x50 */ int extraFlags;
    /* +0x54 */ int colorKey;
    /* +0x58 */ int paletteFlags;
    /* +0x5C */ int paletteRow;
    /* +0x60 */ unsigned char pad_60[0x10];
    /* +0x70 */ int renderFlags;
} PfileLoadOptions;

#ifdef __cplusplus
extern "C" {
#endif
extern void __cdecl Pfile_InitLoadOptions(PfileLoadOptions *opts);
extern void __cdecl Pfile_InitLoadOptionsWithBlend(int blend,
                                                   PfileLoadOptions *opts);
extern void __cdecl Pfile_SetLoadOptionsBlendMode(int blend,
                                                  PfileLoadOptions *opts);
extern void *__cdecl Gfx_CreateRenderObject(int drawUnsorted, int kind,
                                            PfileLoadOptions *opts,
                                            char *name, void *device);
#ifdef __cplusplus
}
#endif

#endif /* LIB_PFILE_H */
