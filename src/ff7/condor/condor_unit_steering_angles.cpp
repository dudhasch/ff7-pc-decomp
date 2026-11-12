// src/ff7/condor/condor_unit_steering_angles.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Angle math and obstacle steering for Fort Condor units: computes headings and sector
//   tests, scans accepted path regions, and steers units around region edges and peer units.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/condor.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/condor.h"
#include "ff7/enums/condor.h"
extern "C" {

#pragma intrinsic(strcpy)
#pragma intrinsic(strcat)
#pragma intrinsic(strlen)

#pragma intrinsic(memset)

#pragma intrinsic(abs)

/*
 * Computes angular heading from origin to target coordinates.
 */
// FUNCTION: FF7 0x0060a450
short __cdecl Mini_CondorAngleBetweenPoints(CondorCursor *from, CondorCursor *to)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        CondorCursor vecX;
        struct { unsigned char pad_to_4[0x4]; short tmpZero; };
        struct { unsigned char pad_to_8[0x8]; short angle; };
    } frame;
#pragma pack(pop)

    Mini_CondorBeginLinearMotion(from->x + 0x4000, from->y + 0x4000, to->x + 0x4000, to->y + 0x4000);
    frame.vecX.x = (*(short *)&g_CondorVecX);
    frame.vecX.y = *(short *)&g_CondorVecY;
    frame.tmpZero = 0;
    frame.angle = Mini_CondorVectorToAngle(&frame.vecX);
    return (short)frame.angle;
}

/*
 * Tests whether an angle falls outside a specified angular arc.
 */
// FUNCTION: FF7 0x0060a4c6
short __cdecl Mini_CondorAngleOutsideShortArc(short angle, short arcA, short arcB)
{
    short upper;
    short lower;

    if (arcA >= arcB) {
        upper = arcA;
        lower = arcB;
    } else {
        upper = arcB;
        lower = arcA;
    }
    if (upper - lower < 0x800) {
        if (angle > upper || angle < lower) {
            return 1;
        }
    } else if (angle < upper && angle > lower) {
        return 1;
    }
    return 0;
}

/*
 * Tests whether a vertex falls within an angular wedge region.
 */
// FUNCTION: FF7 0x0060a550
short __cdecl Mini_CondorIsPointOutsideRegionTriangle(CondorRegionRecord *regionRecord, short vertexX, short vertexY)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[8];
        CondorCursor vertexX;
        struct { unsigned char pad_to_4[0x4]; short tmpZero; };
    } frame;
#pragma pack(pop)

    frame.vertexX.x = vertexX;
    frame.vertexX.y = vertexY;
    frame.tmpZero = 0;
    g_CondorWedgeVertexAngle = Mini_CondorAngleBetweenPoints((CondorCursor *)&regionRecord->corner0X, &frame.vertexX);
    g_CondorWedgeSpanAngle = Mini_CondorAngleBetweenPoints((CondorCursor *)&regionRecord->corner0X, (CondorCursor *)&regionRecord->corner1X);
    g_CondorWedgeTestAngle = Mini_CondorAngleBetweenPoints((CondorCursor *)&regionRecord->corner0X, (CondorCursor *)&regionRecord->corner2X);
    g_CondorWedgeTestAngle += 8;
    if ((short)Mini_CondorAngleOutsideShortArc(g_CondorWedgeVertexAngle, g_CondorWedgeSpanAngle, g_CondorWedgeTestAngle) != 0) {
        return 1;
    } else {
        g_CondorWedgeVertexAngle = Mini_CondorAngleBetweenPoints((CondorCursor *)&regionRecord->corner1X, &frame.vertexX);
        g_CondorWedgeSpanAngle = Mini_CondorAngleBetweenPoints((CondorCursor *)&regionRecord->corner1X, (CondorCursor *)&regionRecord->corner2X);
        g_CondorWedgeTestAngle = Mini_CondorAngleBetweenPoints((CondorCursor *)&regionRecord->corner1X, (CondorCursor *)&regionRecord->corner0X);
        if ((short)Mini_CondorAngleOutsideShortArc(g_CondorWedgeVertexAngle, g_CondorWedgeSpanAngle, g_CondorWedgeTestAngle) != 0) {
            return 1;
        } else {
            return 0;
        }
    }
}

/*
 * Scans traversal regions and identifies valid path sectors.
 */
// FUNCTION: FF7 0x0060a682
short __cdecl Mini_CondorScanAcceptedRegions(CondorRecord *record)
{
    short posX;
    int regionAddr;
    short yOffset;
    short pointY;
    short index;
    short xOffset;
    posX = record->worldX;
    pointY = record->worldY;
    xOffset = posX + 0x4000;
    yOffset = pointY + 0x4000;
    regionAddr = (int)&g_CondorUnitTable;
    for (index = 0; index < g_CondorMatchListState.regionCount; index++, regionAddr += 76) {
        if (xOffset < ((CondorRegionRecord *)regionAddr)->minX)
            continue;
        if (xOffset > ((CondorRegionRecord *)regionAddr)->maxX)
            continue;
        if (yOffset < ((CondorRegionRecord *)regionAddr)->minY)
            continue;
        if (yOffset > ((CondorRegionRecord *)regionAddr)->maxY)
            continue;
        if (Mini_CondorIsPointOutsideRegionTriangle((CondorRegionRecord *)regionAddr, posX, pointY) != 0)
            continue;
        record->rotX = ((CondorRegionRecord *)regionAddr)->vertexX;
        record->rotZ = ((CondorRegionRecord *)regionAddr)->vertexY;
        return index;
    }
    return -1;
}

/*
 * Checks if specified region index is in accepted state.
 */
// FUNCTION: FF7 0x0060a77e
short __cdecl Mini_CondorTestRegionIndexAccepted(CondorRecord *record, short regionIndex)
{
    CondorRegionRecord *regionPtrPtr;
    short biasedY;
    short worldX;
    short worldY;
    short xOffset;
    worldX = record->worldX;
    worldY = record->worldY;
    xOffset = worldX + 0x4000;
    biasedY = worldY + 0x4000;
    regionPtrPtr = (CondorRegionRecord *)(regionIndex * 76 + (char *)&g_CondorUnitTable);
    if (xOffset < regionPtrPtr->minX)
        return -1;
    if (xOffset > regionPtrPtr->maxX)
        return -1;
    if (biasedY < regionPtrPtr->minY)
        return -1;
    if (biasedY > regionPtrPtr->maxY)
        return -1;
    if (Mini_CondorIsPointOutsideRegionTriangle(regionPtrPtr, worldX, worldY) != 0)
        return -1;
    return regionIndex;
}

/*
 * Searches for first valid accepted region index.
 */
// FUNCTION: FF7 0x0060a844
short __cdecl Mini_CondorFindAcceptedRegionIndex(CondorRecord *record)
{
    short result;

    result = Mini_CondorTestRegionIndexAccepted(record, g_CondorUnitRecords[record->targetIndex].edge0);
    if (result != -1) {
        return (short)result;
    } else {
        result = Mini_CondorTestRegionIndexAccepted(record, (short)g_CondorUnitRecords[record->targetIndex].regionIndex);
        if (result != -1) {
            return (short)result;
        } else {
            result = Mini_CondorTestRegionIndexAccepted(record, g_CondorUnitRecords[record->targetIndex].edge2);
            if (result != -1) {
                return (short)result;
            } else {
                return Mini_CondorScanAcceptedRegions(record);
            }
        }
    }
}

/*
 * Finds first unblocked region boundary edge for navigation.
 */
// FUNCTION: FF7 0x0060a8f1
short __cdecl Mini_CondorFindFirstOpenRegionEdge(CondorRecord *record, CondorRegionRecord *region)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        short globalWordA;
        struct { unsigned char pad_to_2[0x2]; short globalWordB; };
        struct { unsigned char pad_to_4[0x4]; short tmpZeroB; };
        struct { unsigned char pad_to_8[0x8]; short recordX; };
        struct { unsigned char pad_to_A[0xA]; short recordY; };
        struct { unsigned char pad_to_C[0xC]; short tmpZeroA; };
    } frame;
#pragma pack(pop)

    frame.recordX = record->worldX;
    frame.recordY = record->worldY;
    frame.tmpZeroA = 0;
    frame.globalWordA = g_CondorScratchPair;
    frame.globalWordB = (*(short *)((char *)&g_CondorScratchPair + 2));
    frame.tmpZeroB = 0;
    if (region->edge0 == -1) {
        return CONDOR_REGION_OPEN_EDGE_0;
    } else if (region->edge1 == -1) {
        return CONDOR_REGION_OPEN_EDGE_1;
    } else if (region->edge2 == -1) {
        return CONDOR_REGION_OPEN_EDGE_2;
    } else {
        return CONDOR_REGION_OPEN_EDGE_NONE;
    }
}

/*
 * Tests whether absolute difference between values is within limit.
 */
// FUNCTION: FF7 0x0060a96c
short __cdecl Mini_CondorTestAbsDeltaWithinLimit(short valueA, short valueB)
{
    short delta;

    delta = valueA - valueB;
    if (delta < 0) {
        delta = -delta;
    }
    if (delta <= 0x400) {
        return 1;
    } else {
        return -1;
    }
}

/*
 * Steers unit movement trajectory along obstacle region boundary.
 */
// FUNCTION: FF7 0x0060a9aa
short __cdecl Mini_CondorSteerRecordAlongRegionEdge(CondorRecord *record)
{
    CondorRegionRecord *regionPtr;
    short edge;
    short angle;
    short edgeAngle;
    short relAngle;

    regionPtr = (CondorRegionRecord *)(record->targetIndex * 0x4C + (char *)&g_CondorUnitTable);
    edge = Mini_CondorFindFirstOpenRegionEdge(record, regionPtr);
    switch (edge) {
    case CONDOR_REGION_OPEN_EDGE_0:
        edgeAngle = Mini_CondorAngleBetweenPoints((CondorCursor *)&regionPtr->corner0X, (CondorCursor *)&regionPtr->corner1X);
        break;
    case CONDOR_REGION_OPEN_EDGE_1:
        edgeAngle = Mini_CondorAngleBetweenPoints((CondorCursor *)&regionPtr->corner1X, (CondorCursor *)&regionPtr->corner2X);
        break;
    case CONDOR_REGION_OPEN_EDGE_2:
        edgeAngle = Mini_CondorAngleBetweenPoints((CondorCursor *)&regionPtr->corner2X, (CondorCursor *)&regionPtr->corner0X);
    }
    if (edge != CONDOR_REGION_OPEN_EDGE_NONE) {
        angle = (short)Mini_CondorVectorToAngle((CondorCursor *)((int)record + 0x58)) & 0xFFF;
        relAngle = 0x1000 - (angle & 0xFFF);
        relAngle = relAngle + edgeAngle + 0x400 & 0xFFF;
        if (relAngle >= 0x800) {
            edgeAngle = edgeAngle + 0x800 & 0xFFF;
            edgeAngle -= 0x40;
        } else {
            edgeAngle += 0x40;
        }
        /* Retail caller reads residual EAX/AX as `short` after `void Mini_CondorAngleToStepComponents` returns. */
        ((short(__cdecl *)(short))Mini_CondorAngleToStepComponents)((short)edgeAngle);
        record->velX = (*(short *)&g_CondorVecX);
        record->velY = *(short *)&g_CondorVecY;
        return 1;
    } else {
        return 0;
    }
}

/*
 * Evaluates blocked movement directions caused by peer units.
 */
// FUNCTION: FF7 0x0060ab0a
short __cdecl Mini_CondorTestPeerBlockDirections(CondorBlockRecord *block, CondorRecord *peer, short dir)
{
    unsigned short sizeY;
    short result;
    unsigned short peerPosX;
    unsigned short sizeSum;
    unsigned short y5;
    unsigned short otherY;
    short xFlags;
    unsigned short otherHeight;
    unsigned short posX;

    posX = block->posX;
    y5 = block->posY;
    peerPosX = peer->posX;
    otherY = peer->posY;
    sizeSum = block->sizeX + peer->extentX >> 1;
    sizeY = block->sizeY;
    otherHeight = peer->extentY;
    result = dir & (CONDOR_BLOCK_DIR_NEG_Y | CONDOR_BLOCK_DIR_POS_Y);
    xFlags = dir & (CONDOR_BLOCK_DIR_NEG_X | CONDOR_BLOCK_DIR_POS_X);
    switch (dir) {
    case CONDOR_BLOCK_DIR_NEG_Y:
        result = Mini_CondorTestAdjacentPeerLowerY((short)posX, (short)y5, (short)peerPosX, (short)otherY, (short)sizeSum, (short)sizeY, (short)otherHeight);
        break;
    case CONDOR_BLOCK_DIR_POS_Y:
        result = Mini_CondorTestAdjacentPeerHigherY((short)posX, (short)y5, (short)peerPosX, (short)otherY, (short)sizeSum, (short)sizeY, (short)otherHeight);
        break;
    case CONDOR_BLOCK_DIR_NEG_X:
        result = Mini_CondorTestAdjacentPeerLowerX((short)posX, (short)y5, (short)peerPosX, (short)otherY, (short)sizeSum, (short)sizeY, (short)otherHeight);
        break;
    case CONDOR_BLOCK_DIR_POS_X:
        result = Mini_CondorCheckRightContact((short)posX, (short)y5, (short)peerPosX, (short)otherY, (short)sizeSum, (short)sizeY, (short)otherHeight);
        break;
    case CONDOR_BLOCK_DIR_NEG_Y | CONDOR_BLOCK_DIR_NEG_X:
        result = Mini_CondorTestAdjacentPeerLowerY((short)posX, (short)y5, (short)peerPosX, (short)otherY, (short)sizeSum, (short)sizeY, (short)otherHeight);
        xFlags = Mini_CondorTestAdjacentPeerLowerX((short)posX, (short)y5, (short)peerPosX, (short)otherY, (short)sizeSum, (short)sizeY, (short)otherHeight);
        break;
    case CONDOR_BLOCK_DIR_NEG_Y | CONDOR_BLOCK_DIR_POS_X:
        result = Mini_CondorTestAdjacentPeerLowerY((short)posX, (short)y5, (short)peerPosX, (short)otherY, (short)sizeSum, (short)sizeY, (short)otherHeight);
        xFlags = Mini_CondorCheckRightContact((short)posX, (short)y5, (short)peerPosX, (short)otherY, (short)sizeSum, (short)sizeY, (short)otherHeight);
        break;
    case CONDOR_BLOCK_DIR_POS_Y | CONDOR_BLOCK_DIR_NEG_X:
        result = Mini_CondorTestAdjacentPeerHigherY((short)posX, (short)y5, (short)peerPosX, (short)otherY, (short)sizeSum, (short)sizeY, (short)otherHeight);
        xFlags = Mini_CondorTestAdjacentPeerLowerX((short)posX, (short)y5, (short)peerPosX, (short)otherY, (short)sizeSum, (short)sizeY, (short)otherHeight);
        break;
    case CONDOR_BLOCK_DIR_POS_Y | CONDOR_BLOCK_DIR_POS_X:
        result = Mini_CondorTestAdjacentPeerHigherY((short)posX, (short)y5, (short)peerPosX, (short)otherY, (short)sizeSum, (short)sizeY, (short)otherHeight);
        xFlags = Mini_CondorCheckRightContact((short)posX, (short)y5, (short)peerPosX, (short)otherY, (short)sizeSum, (short)sizeY, (short)otherHeight);
    }
    return result | xFlags;
}

/*
 * Tests peer collision contact along lower Y boundary.
 */
// FUNCTION: FF7 0x0060ae30
short __cdecl Mini_CondorTestAdjacentPeerLowerY(short posX, short posY, short peerX, short peerY, short span, short height, short peerHeight)
{
    if (posY > peerY && peerY == posY - height) {
        if (posX >= peerX && peerX > posX - span) {
            return 0;
        }
        if (posX < peerX && posX > peerX - span) {
            return 0;
        }
    }
    return CONDOR_BLOCK_DIR_NEG_Y;
}

/*
 * Tests peer collision contact along upper Y boundary.
 */
// FUNCTION: FF7 0x0060ae9d
short __cdecl Mini_CondorTestAdjacentPeerHigherY(short posX, short posY, short peerX, short peerY, short span, short height, short peerHeight)
{
    if (posY < peerY && posY == peerY - peerHeight) {
        if (posX >= peerX && peerX > posX - span) {
            return 0;
        }
        if (posX < peerX && posX > peerX - span) {
            return 0;
        }
    }
    return CONDOR_BLOCK_DIR_POS_Y;
}

/*
 * Tests peer collision contact along left X boundary.
 */
// FUNCTION: FF7 0x0060af0a
short __cdecl Mini_CondorTestAdjacentPeerLowerX(short posX, short posY, short peerX, short peerY, short span, short height, short peerHeight)
{
    if (posX > peerX && peerX == posX - span) {
        if (posY >= peerY && peerY > posY - height) {
            return 0;
        }
        if (posY < peerY && posY > peerY - peerHeight) {
            return 0;
        }
    }
    return CONDOR_BLOCK_DIR_NEG_X;
}

/*
 * Tests peer collision contact along right X boundary.
 */
// FUNCTION: FF7 0x0060af77
short __cdecl Mini_CondorCheckRightContact(short posX, short posY, short peerX, short peerY, short span, short height, short peerHeight)
{
    if (posX < peerX && posX == peerX - span) {
        if (posY >= peerY && peerY > posY - height) {
            return 0;
        }
        if (posY < peerY && posY > peerY - peerHeight) {
            return 0;
        }
    }
    return CONDOR_BLOCK_DIR_POS_X;
}

/*
 * Resolves navigation heading around directional obstacle blocks.
 */
// FUNCTION: FF7 0x0060afe4
short __cdecl Mini_CondorResolveDirectionalBlock(CondorBlockRecord *block)
{
    short didMove;
    short cnt;
    short dirMask;
    short dir;

    didMove = dir = dirMask = cnt = 0;
    if (block->dirY != 0) {
        if (block->dirY < 0) {
            dir = CONDOR_BLOCK_DIR_NEG_Y;
        } else {
            dir = CONDOR_BLOCK_DIR_POS_Y;
        }
    }
    if (block->dirX != 0) {
        if (block->dirX < 0) {
            dir |= CONDOR_BLOCK_DIR_NEG_X;
        } else {
            dir |= CONDOR_BLOCK_DIR_POS_X;
        }
    }
    for (didMove = 0; didMove < g_CondorMatchCount; didMove++) {
        dirMask |= Mini_CondorTestPeerBlockDirections(block, g_CondorMatchListState.entries[didMove].peer, dir);
        if (dirMask != 0) {
            cnt++;
        }
    }
    if (dirMask == 0 && cnt == 0) {
        if ((block->flags & 8) != 0) {
            dirMask = block->dirMask;
        } else {
            return 0x80;
        }
    }
    if ((dirMask & (CONDOR_BLOCK_DIR_NEG_Y | CONDOR_BLOCK_DIR_POS_Y)) != 0 && (dirMask & (CONDOR_BLOCK_DIR_NEG_X | CONDOR_BLOCK_DIR_POS_X)) != 0) {
        return 0;
    }
    if (dir != dirMask) {
        didMove = 1;
        switch (dirMask) {
        case CONDOR_BLOCK_DIR_NEG_Y:
        case CONDOR_BLOCK_DIR_POS_Y:
            if (block->dirY < 0) {
                block->posY -= 1;
                block->offsetY -= 1;
            }
            if (block->dirY > 0) {
                block->posY += 1;
                block->offsetY += 1;
            }
            break;
        case CONDOR_BLOCK_DIR_NEG_X:
        case CONDOR_BLOCK_DIR_POS_X:
            if (block->dirX < 0) {
                block->posX -= 1;
                block->offsetX -= 1;
            }
            if (block->dirX > 0) {
                block->posX += 1;
                block->offsetX += 1;
            }
            break;
        default:
            didMove = 0;
            break;
        }
    } else {
        return 0;
    }
    if (didMove != 0) {
        block->flags = block->flags | 8;
        block->dirMask = (unsigned char)dirMask;
    }
    return didMove;
}
} /* extern "C" */
