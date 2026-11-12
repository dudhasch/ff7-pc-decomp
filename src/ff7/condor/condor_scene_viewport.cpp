// src/ff7/condor/condor_scene_viewport.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Sets the viewport rectangle and projection parameters for the Fort Condor battlefield
//   scene, applying the viewport presets used by the scene.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/condor.h"
#include "lib/codegen.h"
extern "C" {

// clang-format on

/*
 * Configures the Fort Condor viewport rectangle, scale, offsets and inset according
 * to the specified preset mode (0, 1 or 2).
 */
// FUNCTION: FF7 0x005f4630
void __cdecl Mini_CondorApplyViewportPreset(int state, int presetMode)
{
    g_CondorUsesSoftwareDriver = state;
    g_CondorPresetMode = presetMode;
    if (presetMode == 2) {
        Mini_CondorSetViewportRect(0, 0x14, 0x280, 0x1B8);
        g_CondorViewportScale = 2;
        g_CondorScreenReferenceScale = FLOAT_220_BITS;
        g_CondorViewportOffsetX = 0;
        g_CondorViewportOffsetY = 0;
        g_CondorViewportInset = 0x14;
    } else {
        g_CondorViewportScale = 1;
        g_CondorScreenReferenceScale = FLOAT_220_BITS;
        g_CondorViewportInset = 0xA;
        if (presetMode == 0) {
            Mini_CondorSetViewportRect(0, 0xA, 0x140, 0xDC);
            g_CondorViewportOffsetX = 0;
            g_CondorViewportOffsetY = 0;
        } else {
            Mini_CondorSetViewportRect(0xA0, 0x82, 0x140, 0xDC);
            g_CondorViewportOffsetX = 0xA0;
            g_CondorViewportOffsetY = 0x78;
        }
    }
}

/*
 * Stores the Fort Condor viewport rectangle.
 */
// FUNCTION: FF7 0x005f4719
void __cdecl Mini_CondorSetViewportRect(int x, int y, int width, int height)
{
    g_CondorViewportX = x;
    g_CondorViewportWidth = width;
    g_CondorViewportY = y;
    g_CondorViewportHeight = height;
}
} /* extern "C" */
