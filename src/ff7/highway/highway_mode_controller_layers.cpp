// src/ff7/highway/highway_mode_controller_layers.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Mode controller for the highway minigame: it clears the mode's state block and creates
//   the layer actors the mode draws, then draws the layers that are switched on.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/highway_palette.h"
#include "ff7/highway_psxdata.h"
#include <math.h>
#include <stdlib.h>
#include "ff7/types/highway.h"
#include "ff7/api/highway.h"
#include "lib/pfile.h"

#pragma intrinsic(strlen, strcpy, strcat, strcmp)
extern "C" {
#pragma intrinsic(memset)
#pragma intrinsic(atan2, sqrt)

#pragma intrinsic(abs)

} /* extern "C" */

/*
 * Clears layerArg, the four layer pointers and the four visible flags.
 */
// FUNCTION: FF7 0x006595e0
HighwayModeController::HighwayModeController()
{
    int i;

    this->layerArg = 0;
    for (i = 0; i < 4; i++) {
        this->layers[i] = 0;
        this->visible[i] = 0;
    }
}

// Creates four actors from the global highway index, with identifiers 0xAE, 0xA5, 0xA3 and 0xA7,
// and stores them in the four layers. Returns one regardless of the creation results.
// FUNCTION: FF7 0x00659628
char HighwayModeController::createLayers()
{
    HighwayModelNode *other;

    other = (HighwayModelNode *)g_pHighwayIndexBlock;
    this->layers[0] =
        g_pHighwayIndexBlock->createRaw(174, other, 0, 0, 0, 0, 0, 0);
    this->layers[1] =
        g_pHighwayIndexBlock->createRaw(165, other, 0, 0, 0, 0, 0, 0);
    this->layers[2] =
        g_pHighwayIndexBlock->createRaw(163, other, 0, 0, 0, 0, 0, 0);
    this->layers[3] =
        g_pHighwayIndexBlock->createRaw(167, other, 0, 0, 0, 0, 0, 0);
    return 1;
}

/*
 * Draws each layer whose visible flag is set, with offsets -500, -300, 0 and -500 and the final
 * layer argument 1 only for the last. Passes layerArg to every call and does not check the layer
 * pointers.
 */
// FUNCTION: FF7 0x006596d7
void HighwayModeController::drawLayers()
{
    if (this->visible[0]) {
        this->layers[0]->drawLayer(-500, 0, this->layerArg);
    }
    if (this->visible[1]) {
        this->layers[1]->drawLayer(-300, 0, this->layerArg);
    }
    if (this->visible[2]) {
        this->layers[2]->drawLayer(0, 0, this->layerArg);
    }
    if (this->visible[3]) {
        this->layers[3]->drawLayer(-500, 1, this->layerArg);
    }
}
