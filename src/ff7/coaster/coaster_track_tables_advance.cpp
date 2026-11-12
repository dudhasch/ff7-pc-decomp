// src/ff7/coaster/coaster_track_tables_advance.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Track resolution for the coaster: resolves the track and rail tables, selects the rail
//   set, and advances the cart position and speed along the track.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/coaster.h"
#include "ff7/coaster_psxdata.h"
#include "ff7/coaster_renderer.h"
#include <windows.h>
#include <ddraw.h>
#include "ff7/api/coaster.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/crt.h"
#include <time.h>

extern "C" {


// GLOBAL: FF7 0x00c3f888
CoasterRenderer *g_pCoasterRenderer = 0;
unsigned char g_bssPad_00c3f88c[4] = {0};
// GLOBAL: FF7 0x00c3f890
char g_CoasterRideMode = 0;
// GLOBAL: FF7 0x00c3f894
int g_CoasterTrackLength = 0;
// GLOBAL: FF7 0x00c3f898
int g_CoasterRailSetTable = 0;
unsigned char g_bssPad_00c3f89c[4] = {0};
// GLOBAL: FF7 0x00c3f8a0
PsxMatrix32 g_CoasterCameraMatrixBuf = {0};
// clang-format off
// GLOBAL: FF7 0x00c3f8c0
int *g_pCoasterTrackData = 0;
unsigned char g_bssPad_00c3f8c4[4] = {0};
unsigned char g_bssPad_00c3f8c8[8] = {0};
// GLOBAL: FF7 0x00c3f8d0
SVECTOR *g_pCoasterRailSet = 0;
unsigned char g_bssPad_00c3f8d4[4] = {0};
// GLOBAL: FF7 0x00c3f8d8
int g_CoasterCameraPositionXCopy = 0;
// GLOBAL: FF7 0x00c3f8dc
int g_CoasterCameraPositionYCopy = 0;
// GLOBAL: FF7 0x00c3f8e0
int g_CoasterCameraPositionZCopy = 0;
// GLOBAL: FF7 0x00c3f8e4
int g_CoasterCameraPositionPadCopy = 0;
// GLOBAL: FF7 0x00c3f8e8
VECTOR g_CoasterCameraPosition = {0};
// GLOBAL: FF7 0x00c3f8f8
int g_CoasterRailInitX = 0;
// GLOBAL: FF7 0x00c3f8fc
int g_CoasterRailInitY = 0;
// GLOBAL: FF7 0x00c3f900
int g_CoasterRailInitZ = 0;
unsigned char g_bssPad_00c3f904[4] = {0};
// GLOBAL: FF7 0x00c3f908
int g_CoasterDebugTuneSquareCircle = 0;
// GLOBAL: FF7 0x00c3f90c
int g_CoasterDebugTuneR1R2 = 0;
// GLOBAL: FF7 0x00c3f910
int g_CoasterDebugTuneCrossTriangle = 0;
unsigned char g_bssPad_00c3f914[4] = {0};
// clang-format on

/*
 * Resolves track and rail tables from the PSX data set and initializes rail cursors.
 */
// FUNCTION: FF7 0x005ea8c0
void __cdecl Mini_CoasterResolveTrackTables(void)
{
    int trackOffset;
    int *trackTable;
    int railTableUnit;
    int railOffset;

    trackOffset = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(6) << 2));
    if (trackOffset >= g_CoasterPsxDataSet.binSize) {
        trackTable = 0;
    } else {
        trackTable = (int *)(g_CoasterPsxDataSet.binData + trackOffset);
    }
    g_pCoasterTrackData = trackTable;
    railOffset = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(5) << 2));
    if (railOffset >= g_CoasterPsxDataSet.binSize) {
        railTableUnit = 0;
    } else {
        railTableUnit = g_CoasterPsxDataSet.binData + railOffset;
    }
    g_CoasterRailSetTable = railTableUnit;
    g_CoasterRailInitX = 0;
    g_CoasterRailInitY = 0xFFFFE48A;
    g_CoasterRailInitZ = 200;
    Mini_CoasterSelectRailSet(0);
    g_CoasterRideMode = 1;
}

/*
 * Advances the roller coaster cart position and speed along the track.
 */
// FUNCTION: FF7 0x005ea973
void __cdecl Mini_CoasterAdvanceRide(void)
{
    struct {
        VECTOR cameraPosition;
        PsxMatrix32 rotationMatrix;
        SVECTOR cameraRotation;
        SVECTOR inverseRotation;
        int gravity;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.rotationMatrix.m[0][0] = 0x1000 /* 360 deg */;
    local.rotationMatrix.m[0][1] = 0;
    local.rotationMatrix.m[0][2] = 0;
    local.rotationMatrix.m[1][0] = 0;
    local.rotationMatrix.m[1][1] = 0x1000 /* 360 deg */;
    local.rotationMatrix.m[1][2] = 0;
    local.rotationMatrix.m[2][0] = 0;
    local.rotationMatrix.m[2][1] = 0;
    local.rotationMatrix.m[2][2] = 0x1000 /* 360 deg */;
    local.rotationMatrix.t[0] = 0;
    local.rotationMatrix.t[1] = 0;
    local.rotationMatrix.t[2] = 0;

    Mini_CoasterSampleTrack(g_CoasterRidePosition, -100, &local.cameraPosition,
                            &local.cameraRotation);
    g_CoasterRidePosition += g_CoasterRideSpeed;

    g_CoasterCameraPosition = local.cameraPosition;
    g_CoasterCameraPositionXCopy = g_CoasterCameraPosition.vx;
    g_CoasterCameraPositionYCopy = g_CoasterCameraPosition.vy;
    g_CoasterCameraPositionZCopy = g_CoasterCameraPosition.vz;
    g_CoasterCameraPositionPadCopy = g_CoasterCameraPosition.pad;

    if (local.cameraRotation.vx < 0)
        local.cameraRotation.vx += 0x1000 /* 360 deg */;
    local.gravity = SinFixed(local.cameraRotation.vx) / 15;
    if (local.gravity > 0 && g_CoasterRideSpeed > 43000)
        g_CoasterRideSpeed -= local.gravity;
    if (local.gravity < 0 && g_CoasterRideSpeed < 120000)
        g_CoasterRideSpeed -= local.gravity;

    local.inverseRotation.vx = -local.cameraRotation.vx;
    local.inverseRotation.vy = -local.cameraRotation.vy;
    local.inverseRotation.vz = -local.cameraRotation.vz;
    RotMatrix((SVECTOR *)&local.inverseRotation, &local.rotationMatrix);
    Mini_CoasterConcatCameraMatrix((PsxMatrix32 *)&local.rotationMatrix, (PsxMatrix32 *)&g_CoasterCameraMatrixBuf,
                                   (PsxMatrix32 *)&g_CoasterCameraMatrixBuf);
}

/*
 * Points g_pCoasterRailSet at the rail set chosen by railSetIndex and caches the track length.
 */
// FUNCTION: FF7 0x005eaaf3
void __cdecl Mini_CoasterSelectRailSet(unsigned int railSetIndex)
{
    int ofs;
    int rawOffset;
    int railBase;

    ofs = *(int *)((char *)g_CoasterRailSetTable + (railSetIndex & 0xFF) * 4);
    /* () keeps the table index out of constant folding, as in the original. */
    rawOffset = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(4) << 2));
    if (rawOffset >= g_CoasterPsxDataSet.binSize) {
        railBase = 0;
    } else {
        railBase = g_CoasterPsxDataSet.binData + rawOffset;
    }
    g_pCoasterRailSet = (SVECTOR *)(railBase + ofs);
    g_CoasterTrackLength = *g_pCoasterTrackData;
}

/*
 * Empty event hook stub in the coaster subsystem.
 */
// FUNCTION: FF7 0x005eab5e
void Mini_CoasterNullStub(void)
{
    /* The original function body is empty. */
}
} /* extern "C" */
