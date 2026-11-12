// src/ff7/src/wm/wm_world_light_groups.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Creates, sets and frees the world's primary and secondary three-light groups, and
//   converts a matrix diagonal into an ambient colour.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/codegen.h"
#include "ff7/world.h"
#include "lib/types/graphics.h"
#include "ff7/types/wm.h"
#include "ff7/api/wm.h"
#include "lib/api/graphics.h"
#include "lib/api/token.h"
#include "ff7/enums/wm.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00e360f0
struct WorldAmbientColor g_WorldAmbientColor = {0};
// GLOBAL: FF7 0x00e360f4
struct GfxLightOwner *g_pWorldPrimaryLightGroup = 0;
// GLOBAL: FF7 0x00e360f8
void *g_pWorldSecondaryLightGroup = 0;
unsigned char g_bssPad_00e360fc[4] = {0};
// clang-format on

// Gfx_LightCreateThreeLightOwner (dir0, dir1, dir2, color0, color1, color2, ambient) is called below;
// the primary group keeps its colours as BgraValue unions and passes their packed values, the
// secondary group keeps them as ints.

/* Rotates a light direction through the world axis matrix, through the temp vector. */
#define ROTATE_LIGHT_DIR(dir)                                    \
    lights.temp.x = lights.dir.x;                                \
    lights.temp.y = lights.dir.y;                                \
    lights.temp.z = lights.dir.z;                                \
    RotateVector3ColVec(lights.matrix, &lights.temp, &lights.dir)

/*
 * Builds and returns a three-light configuration after rotating all directions through the world
 * axis matrix. For setId other than 2, uses three specified fixed-point direction vectors,
 * grayscale light colors 0x6E, 0xAA and 0x3C and ambient RGB 0x40. Set 2 uses one white light in
 * the transformed positive-Y direction, two black zero-direction lights and ambient RGB 0x20,
 * 0x20, 0x30.
 */
// FUNCTION: FF7 0x0075e7a0
void *__cdecl World_CreateSecondaryLightGroup(int setId)
{
    struct {
        FloatVector3 dir_Light1;
        FloatVector3 dir_Light2;
        void *light;
        FloatVector3 dir_Light0;
        FloatVector3 temp;
        unsigned int col_Light0;
        BgraValue ambient;
        float matrix[4][4];
        unsigned int col_Light2;
        unsigned int col_Light1;
    } lights;

    World_SetQuarterTurnXMatrix4x4(lights.matrix);
    if (setId != 2) {
        lights.dir_Light0.x = 659.0f / 4096.0f;
        lights.dir_Light0.y = -411.0f / 4096.0f;
        lights.dir_Light0.z = -4034.0f / 4096.0f;
        lights.dir_Light1.x = -1536.0f / 4096.0f;
        lights.dir_Light1.y = 3349.0f / 4096.0f;
        lights.dir_Light1.z = 1839.0f / 4096.0f;
        lights.dir_Light2.x = 2920.0f / 4096.0f;
        lights.dir_Light2.y = 1474.0f / 4096.0f;
        lights.dir_Light2.z = 2484.0f / 4096.0f;

        ROTATE_LIGHT_DIR(dir_Light0);
        ROTATE_LIGHT_DIR(dir_Light1);
        ROTATE_LIGHT_DIR(dir_Light2);

        lights.col_Light0 = 0xff6e6e6e;
        lights.col_Light1 = 0xffaaaaaa;
        lights.col_Light2 = 0xff3c3c3c;
        lights.ambient.c.r = 0x40;
        lights.ambient.c.g = 0x40;
        lights.ambient.c.b = 0x40;
        lights.ambient.c.a = 0xff;
    } else {
        lights.dir_Light0.x = 0;
        lights.dir_Light0.y = 1.0f;
        lights.dir_Light0.z = 0;
        lights.dir_Light1.x = 0;
        lights.dir_Light1.y = 0;
        lights.dir_Light1.z = 0;
        lights.dir_Light2.x = 0;
        lights.dir_Light2.y = 0;
        lights.dir_Light2.z = 0;

        ROTATE_LIGHT_DIR(dir_Light0);
        ROTATE_LIGHT_DIR(dir_Light1);
        ROTATE_LIGHT_DIR(dir_Light2);

        lights.col_Light0 = 0xffffffff;
        lights.col_Light1 = 0xff000000;
        lights.col_Light2 = 0xff000000;
        lights.ambient.c.r = 0x20;
        lights.ambient.c.g = 0x20;
        lights.ambient.c.b = 0x30;
        lights.ambient.c.a = 0xff;
    }

    lights.light = (void *)Gfx_LightCreateThreeLightOwner(
        &lights.dir_Light0,
        &lights.dir_Light1,
        &lights.dir_Light2,
        lights.col_Light0,
        lights.col_Light1,
        lights.col_Light2,
        lights.ambient.bgra);
    return lights.light;
}

/*
 * Creates a three-light object through Gfx_LightCreateThreeLightOwner from stack vectors and packed
 * colors; preset is the stage's texture set (WorldTextureSet). The overworld and snow sets use
 * first direction (1,1,1) with gray 0xFF6E6E6E, zero other directions and black colors, and ambient
 * 0xFF404040. The underwater set uses first direction (0,1,0) and ambient 0xFF202020 with the same
 * light colors. Any other value leaves the stack values unset.
 */
// FUNCTION: FF7 0x0075e9b3
void *__cdecl World_CreatePrimaryLightGroup(int preset)
{
    struct {
        WorldLightVector dir_Light1;
        WorldLightVector dir_Light2;
        void *lightOwner;
        WorldLightVector dir_Light0;
        WorldLightColor col_Light0;
        WorldLightColor ambientColor;
        WorldLightColor col_Light2;
        WorldLightColor col_Light1;
    } lights;

    switch (preset) {
    case WORLD_TEXTURE_SET_OVERWORLD:
    case WORLD_TEXTURE_SET_SNOW:
        lights.dir_Light0.x = 1.0f;
        lights.dir_Light0.y = 1.0f;
        lights.dir_Light0.z = 1.0f;
        lights.dir_Light1.x = 0;
        lights.dir_Light1.y = 0;
        lights.dir_Light1.z = 0;
        lights.dir_Light2.x = 0;
        lights.dir_Light2.y = 0;
        lights.dir_Light2.z = 0;
        lights.col_Light0.bgra = 0xff6e6e6e;
        lights.col_Light1.bgra = 0xff000000;
        lights.col_Light2.bgra = 0xff000000;
        lights.ambientColor.c.r = 0x40;
        lights.ambientColor.c.g = 0x40;
        lights.ambientColor.c.b = 0x40;
        lights.ambientColor.c.a = 0xff;
        break;
    case WORLD_TEXTURE_SET_UNDERWATER:
        lights.dir_Light0.x = 0;
        lights.dir_Light0.y = 1.0f;
        lights.dir_Light0.z = 0;
        lights.dir_Light1.x = 0;
        lights.dir_Light1.y = 0;
        lights.dir_Light1.z = 0;
        lights.dir_Light2.x = 0;
        lights.dir_Light2.y = 0;
        lights.dir_Light2.z = 0;
        lights.col_Light0.bgra = 0xff6e6e6e;
        lights.col_Light1.bgra = 0xff000000;
        lights.col_Light2.bgra = 0xff000000;
        lights.ambientColor.c.r = 0x20;
        lights.ambientColor.c.g = 0x20;
        lights.ambientColor.c.b = 0x20;
        lights.ambientColor.c.a = 0xff;
        break;
    }
    lights.lightOwner = (void *)Gfx_LightCreateThreeLightOwner(&lights.dir_Light0, &lights.dir_Light1, &lights.dir_Light2,
                                                               lights.col_Light0.bgra, lights.col_Light1.bgra, lights.col_Light2.bgra,
                                                               lights.ambientColor.bgra);
    return lights.lightOwner;
}

/*
 * Releases the world lighting object g_pWorldPrimaryLightGroup through Gfx_LightFreeOwner(1, ...)
 * if one exists and nulls the pointer.
 */
// FUNCTION: FF7 0x0075eace
void __cdecl World_FreePrimaryLightGroup(void)
{
    if (g_pWorldPrimaryLightGroup != 0) {
        Gfx_LightFreeOwner(1, g_pWorldPrimaryLightGroup);
        g_pWorldPrimaryLightGroup = 0;
    }
}

/*
 * Releases the second world graphics object g_pWorldSecondaryLightGroup the same way
 * World_FreePrimaryLightGroup releases the first.
 */
// FUNCTION: FF7 0x0075eaf6
void __cdecl World_FreeSecondaryLightGroup(void)
{
    if (g_pWorldSecondaryLightGroup != 0) {
        Gfx_LightFreeOwner(1, (GfxLightOwner *)g_pWorldSecondaryLightGroup);
        g_pWorldSecondaryLightGroup = 0;
    }
}

/*
 * Releases both world graphics objects, g_pWorldSecondaryLightGroup then
 * g_pWorldPrimaryLightGroup.
 */
// FUNCTION: FF7 0x0075eb1e
void __cdecl World_FreeLightGroups(void)
{
    World_FreeSecondaryLightGroup();
    World_FreePrimaryLightGroup();
}

/*
 * Applies a lighting preset (WorldLightPreset) to the three lights of g_pWorldPrimaryLightGroup.
 * DIAGONAL sets the directions (1,1,1)/(0,0,0)/(0,0,0) with colours 0xFF6E6E6E, black, black;
 * OVERHEAD the same with the first direction (0,1,0); ZONE_COLOR takes the first colour from
 * g_WorldAmbientColor; RESET_COLOR restores 0xFF6E6E6E. Then the first light's float colour is rebuilt and the other two are set to
 * (0, 0, 0, 1).
 */
// FUNCTION: FF7 0x0075eb2d
void __cdecl World_SetPrimaryLightGroupPreset(int preset)
{
    if (g_pWorldPrimaryLightGroup != 0) {
        switch (preset) {
        case WORLD_LIGHT_PRESET_DIAGONAL:
            g_pWorldPrimaryLightGroup->light0->dirX = FLOAT_ONE_BITS;
            g_pWorldPrimaryLightGroup->light0->dirY = FLOAT_ONE_BITS;
            g_pWorldPrimaryLightGroup->light0->dirZ = FLOAT_ONE_BITS;
            g_pWorldPrimaryLightGroup->light1->dirX = 0;
            g_pWorldPrimaryLightGroup->light1->dirY = 0;
            g_pWorldPrimaryLightGroup->light1->dirZ = 0;
            g_pWorldPrimaryLightGroup->light2->dirX = 0;
            g_pWorldPrimaryLightGroup->light2->dirY = 0;
            g_pWorldPrimaryLightGroup->light2->dirZ = 0;
            g_pWorldPrimaryLightGroup->light0->color = 0xFF6E6E6E;
            g_pWorldPrimaryLightGroup->light1->color = 0xFF000000;
            g_pWorldPrimaryLightGroup->light2->color = 0xFF000000;
            break;
        case WORLD_LIGHT_PRESET_OVERHEAD:
            g_pWorldPrimaryLightGroup->light0->dirX = 0;
            g_pWorldPrimaryLightGroup->light0->dirY = FLOAT_ONE_BITS;
            g_pWorldPrimaryLightGroup->light0->dirZ = 0;
            g_pWorldPrimaryLightGroup->light1->dirX = 0;
            g_pWorldPrimaryLightGroup->light1->dirY = 0;
            g_pWorldPrimaryLightGroup->light1->dirZ = 0;
            g_pWorldPrimaryLightGroup->light2->dirX = 0;
            g_pWorldPrimaryLightGroup->light2->dirY = 0;
            g_pWorldPrimaryLightGroup->light2->dirZ = 0;
            g_pWorldPrimaryLightGroup->light0->color = 0xFF6E6E6E;
            g_pWorldPrimaryLightGroup->light1->color = 0xFF000000;
            g_pWorldPrimaryLightGroup->light2->color = 0xFF000000;
            break;
        case WORLD_LIGHT_PRESET_ZONE_COLOR:
            g_pWorldPrimaryLightGroup->light0->color = *(int *)&g_WorldAmbientColor;
            g_pWorldPrimaryLightGroup->light1->color = 0xFF000000;
            g_pWorldPrimaryLightGroup->light2->color = 0xFF000000;
            break;
        case WORLD_LIGHT_PRESET_RESET_COLOR:
            g_pWorldPrimaryLightGroup->light0->color = 0xFF6E6E6E;
            g_pWorldPrimaryLightGroup->light1->color = 0xFF000000;
            g_pWorldPrimaryLightGroup->light2->color = 0xFF000000;
        }
        Gfx_UnpackBgraToFloat4(g_pWorldPrimaryLightGroup->light0->color, (float *)&g_pWorldPrimaryLightGroup->light0->colorB);
        Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)&g_pWorldPrimaryLightGroup->light0->colorB);
        g_pWorldPrimaryLightGroup->light1->colorR = 0;
        g_pWorldPrimaryLightGroup->light1->colorG = 0;
        g_pWorldPrimaryLightGroup->light1->colorB = 0;
        g_pWorldPrimaryLightGroup->light1->colorA = FLOAT_ONE_BITS;
        g_pWorldPrimaryLightGroup->light2->colorR = 0;
        g_pWorldPrimaryLightGroup->light2->colorG = 0;
        g_pWorldPrimaryLightGroup->light2->colorB = 0;
        g_pWorldPrimaryLightGroup->light2->colorA = FLOAT_ONE_BITS;
        Gfx_LightRebuildCachedMatrices(g_pWorldPrimaryLightGroup);
    }
}

// Converts the diagonal of the 3x3 short matrix m into the ambient colour in g_WorldAmbientColor.
// Each diagonal value is shifted right by 4: diagonal0 is the red byte, diagonal1 green and
// diagonal2 blue; alpha is 0xFF.
// FUNCTION: FF7 0x0075ee10
void __cdecl World_SetAmbientColorFromMatrix(WorldAmbientMatrix *m)
{
    g_WorldAmbientColor.r = m->diagonal0 >> 4;
    g_WorldAmbientColor.g = m->diagonal1 >> 4;
    g_WorldAmbientColor.b = m->diagonal2 >> 4;
    g_WorldAmbientColor.a = 0xFF;
}
} /* extern "C" */
