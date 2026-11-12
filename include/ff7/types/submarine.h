/* include/ff7/types/submarine.h: the types the submarine TUs share.
 * One definition per type. Most submarine code is filed under src/ff7/chocobo/
 */
#ifndef FF7_TYPES_SUBMARINE_H
#define FF7_TYPES_SUBMARINE_H
#include "lib/types/graphics.h"

/* One submarine model: vertex, triangle and color arrays and the triangle count
 * (g_SubmarineModelDescriptors in sub_record_update_and_draw.cpp). */
typedef struct SubmarineModelDescriptor {
    void *vertices;
    void *triangles;
    void *colors;
    int count;
} SubmarineModelDescriptor;

/* hub SubmarineFlightRecord begin */
typedef struct SubmarineFlightRecord {
    int posX;
    int posY;
    int posZ;
    unsigned char pad_0C[0x4];
    int targetX;
    int targetY;
    int targetZ;
    unsigned char pad_1C[0x4];
    short angleA;
    short angleB;
    short heightOffset;
    unsigned char pad_26[0x2];
    short vecX;
    short vecY;
    short vecZ;
    unsigned char pad_2E[0xa];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_3E[0x2];
    short aimPitch;
    short aimYaw;
    short aimRoll;
    unsigned char pad_46[0x2];
    short aimDirX;
    short aimDirY;
    short aimDirZ;
    unsigned char pad_4E[0x2];
    short offsetX; /* offset from the player, in whole units */
    short offsetY;
    short offsetZ;
    unsigned char pad_56[0x2];
    int playerDistance; /* normalized length of that offset */
    int dotRight;
    int dotForward;
    int elapsed;
    int aimDotProduct;
    int workingLevel;
    int speed;
    int stepsLeft;
    int limit;
    int state;
    int gridCellIndex;
    int target;
    int timer;
    int flightPointPtrA;
    int flightPointPtrB;
    int flightPointPtrC;
    int connectorPtrA;
    int connectorPtrB;
    short shortPosX;
    short shortPosY;
    short shortPosZ;
    unsigned char pad_A6[0x2];
    int flightBValue;
    int flightBParam;
    int flightCValue;
    int flightCParam;
    int connectorAValue;
    int connectorAParam;
    int connectorBValue;
    int connectorBParam;
} SubmarineFlightRecord;
/* hub SubmarineFlightRecord end */

/* hub SubmarineGridCell begin */
/* A cell of the submarine's 64 x 64 terrain grid. */
typedef struct SubmarineGridCell {
    unsigned short currentHeight; /* +0x0: stepped toward height, or raisedHeight while flag 0x1000 (Submarine_StepGridCellHeights) */
    short southHeight;            /* +0x2: the neighbours' copies of currentHeight the step writes */
    short westHeight;             /* +0x4 */
    short eastHeight;             /* +0x6 */
    short northHeight;            /* +0x8 */
    unsigned short flags;         /* +0xA: 0x1000 raised, 0x2000 flagged, 0x8000 timed (the low byte counts down) */
    unsigned short raisedHeight;  /* +0xC */
    unsigned short height;        /* +0xE: the rest height */
} SubmarineGridCell;
/* hub SubmarineGridCell end */

/* Radial terrain batch entry: four frame-derived ints and one frame word. */
typedef struct SubmarineRadialTerrainBatch {
    int x;
    int y;
    int z;
    int w;
    int color;
} SubmarineRadialTerrainBatch;

/* Grid batch entry in batch list A: four frame values plus one grid vertex word. */
typedef struct SubmarineGridBatchA {
    int x;
    int y;
    int z;
    int w;
    int gridValue;
} SubmarineGridBatchA;

/* Radial terrain vertex record: three values scaled by one factor, which is stored in scale. */
typedef struct SubmarineRadialTerrainScaledA {
    float x;
    float y;
    float z;
    float scale;
    int color;
} SubmarineRadialTerrainScaledA;

/* One camera-grid batch entry, a four-word copy of a grid vertex. */
typedef struct SubmarineGridBatchEntryA {
    int x;
    int y;
    int z;
    unsigned char pad_0C[0x4];
    int color;
} SubmarineGridBatchEntryA;

/* One 16-byte record of g_SubmarineGridVertexBuffer (64 by 64, rows 0x400 bytes apart). x, y and z hold
   float bit patterns (copied as ints); the colour's low two bytes carry the cell height shading. */
typedef struct SubmarineGridVertex {
    int x;
    int y;
    int z;
    unsigned int color;
} SubmarineGridVertex;

/* Submarine direction vector built from two angles, three shorts. */
typedef struct SubmarineDirectionVector {
    short x;
    short y;
    short z;
} SubmarineDirectionVector;

/* Three-int vector that Submarine_NormalizeVectorInPlace shifts right in place. */
typedef struct SubmarineVec3Int {
    int x;
    int y;
    int z;
} SubmarineVec3Int;

/* Scratch record (in the GTE data block) of Submarine_ComputeRecordPitchYaw: the direction from an
 * object to its waypoint, scaled down by 4096 and normalized, and the normalized side vector
 * crossed from it. */
typedef struct SubmarineRelativeRecord {
    SubmarineVec3Int offset;
    unsigned char pad_0C[0x4];
    SubmarineVec3Int side;
} SubmarineRelativeRecord;

/* Offset table entry (8 bytes) used by the radial band terrain batches. */
typedef struct SubmarineRadialOffset {
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_06[0x2];
} SubmarineRadialOffset;

/* Three-short vertex record read by the submarine object triangle batch emitter. */
typedef struct SubmarineEmitVertex {
    short x;
    short y;
    short z;
    unsigned char pad_06[0x2];
} SubmarineEmitVertex;

/* Keyframe of a timed submarine effect; the active flag advances to the next key. */
typedef struct SubmarineTimedEffectKey {
    int waypointX;
    int waypointY;
    int waypointZ;
    unsigned char pad_0C[0x4];
    int time;
    int timerValue;
    struct SubmarineTimedEffectKey *nextKey;
} SubmarineTimedEffectKey;

/* One flagged submarine grid cell: x and y position and the source record index. */
typedef struct SubmarineFlaggedCell {
    short cellX;
    short cellHeight;
    short cellY;
    short recordIndex;
} SubmarineFlaggedCell;

/* Flight parameters of a submarine record, reached through a pointer stored at offset 0x84. */
typedef struct SubmarineFlightParams {
    int posX;
    int posY;
    int posZ;
    unsigned char pad_0C[0x4];
    int countdown;
} SubmarineFlightParams;

/* Byte-indexed lookup cell in the submarine grid buffer. */
typedef struct BitCountTableEntry {
    char tableValue;
    unsigned char pad_01[0x3];
} BitCountTableEntry;

/* Anchor record holding submarine position as 12-bit fixed-point values. */
typedef struct SubmarineTriangleAnchor {
    short posX;
    short posY;
    short posZ;
} SubmarineTriangleAnchor;

/* Submarine object record: fixed-point anchor, triangle batch parameters and active flags. */
typedef struct SubmarineObjectState {
    int posX; /* +0x0 */
    int posY;
    int posZ;
    unsigned char pad_0C[0x10];
    short renderState;
    short angleB;
    short triangleCount;
    unsigned char pad_22[0x12];
    int flags;
    int statusFlags;
    int frameCount;
    int limit;
    unsigned char pad_44[0x10];
    int batch;
} SubmarineObjectState;

/* One 32-byte row of a level's route table (g_SubmarineLevel0RouteTable..5): a position in grid
 * units and the index of the next row, copied into a route record. */
typedef struct SubmarineRouteNode {
    int x;
    int y;
    int z;
    unsigned char pad_0C[0xc];
    int index;
    unsigned char pad_1C[0x4];
} SubmarineRouteNode;

/* The model Submarine_EmitObjectTriangleBatch draws: its vertices, the packed triangle indices
 * (three bytes of vertex indices per entry), three packed colour words per triangle and the triangle count. */
typedef struct SubmarineAngleSet {
    SubmarineEmitVertex *vertexArray;
    int *triIndexList;
    unsigned int *colorCursor;
    int triangleCount;
} SubmarineAngleSet;

/* Two fixed-point angles that build a direction vector. */
typedef struct SubmarineDirectionAngles {
    short angleFirst;
    short angleSecond;
} SubmarineDirectionAngles;

/* Submarine object record (0x70 bytes): position, angles, flags, timed-effect key and four effect slots. */
typedef struct SubmarineContactRecord {
    int posX;
    int posY;
    int posZ;
    unsigned char pad_0C[0x4];
    int slotWeight;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_1A[0x2];
    short angleA;
    short angleB;
    short heightOffset;
    unsigned char pad_22[0x2];
    short angleC;
    short angleD;
    short angleE;
    unsigned char pad_2A[0x2];
    int speed;
    int timer;
    int flags;
    int state;
    int frameCount;
    int magnitude;
    int dotResultA;
    int dotResultB;
    short playerDx;
    short playerDy;
    short playerDz;
    unsigned char pad_52[0x2];
    SubmarineModelDescriptor *modelDescriptor;
    struct SubmarineTimedEffectKey *timedKey;
    int elapsed;
    int effectSlots[4];
} SubmarineContactRecord;

/* Submarine record connector quads: corner shorts, deltas, primitive tag and vertex data. */
typedef struct SubmarineRecordConnector {
    short corner0X;
    short corner0Y;
    short corner0Z;
    unsigned char pad_06[0x2];
    short corner1X;
    short corner1Y;
    short corner1Z;
    unsigned char pad_0E[0x2];
    short corner2X;
    short corner2Y;
    short corner2Z;
    unsigned char pad_16[0x2];
    short corner3X;
    short corner3Y;
    short corner3Z;
    unsigned char pad_1E[0x2];
    short corner4X;
    short corner4Y;
    short corner4Z;
    unsigned char pad_26[0x2];
    short corner5X;
    short corner5Y;
    short corner5Z;
    unsigned char pad_2E[0x2];
    short corner6X;
    short corner6Y;
    short corner6Z;
    unsigned char pad_36[0x2];
    short corner7X;
    short corner7Y;
    short corner7Z;
    unsigned char pad_3E[0x2];
    short corner8X;
    short corner8Y;
    short corner8Z;
    unsigned char pad_46[0x2];
    short corner9X;
    short corner9Y;
    short corner9Z;
    unsigned char pad_4E[0x2];
    short corner10X;
    short corner10Y;
    short corner10Z;
    unsigned char pad_56[0x2];
    short corner11X;
    short corner11Y;
    short corner11Z;
    unsigned char pad_5E[0x1a];
    int cameraBatch;
    unsigned char pad_7C[0x10];
    int primTag;
    unsigned char pad_90[0x4];
    int vertexData;
    struct SubmarineConnectorQuadValues *quadColors; /* the six colors of the record being drawn, copied from g_SubmarineConnectorColor1 */
    int colorSteering;
    unsigned char pad_A0[0xc];
    int colorCoasting;
    unsigned char pad_B0[0xc];
    int colorExpired;
    unsigned char pad_C0[0xc];
    int colorHitTarget;
    unsigned char pad_D0[0xc];
    int colorHitGround;
    unsigned char pad_E0[0x24];
    short deltaX;
    short deltaY;
    short deltaZ;
} SubmarineRecordConnector;

/* Only the fields the code reads; the size is not known. */
typedef struct SubmarineHudRecord {
    unsigned char pad_00[0x28];
    short pad_28;
    short pad_2A;
    short pad_2C;
    short pad_2E;
    short pad_30;
    short pad_32;
    short pad_34;
    unsigned char pad_36[0x2];
    int pad_38;
    int pad_3C;
    int pad_40;
    int pad_44;
    int pad_48;
    short pad_4C;
    short pad_4E;
    short pad_50;
    short pad_52;
    short pad_54;
    short pad_56;
    short pad_58;
    unsigned char pad_5A[0x2];
    int pad_5C;
    int pad_60;
    int pad_64;
    int pad_68;
    int pad_6C;
    short pad_70;
    short pad_72;
    short pad_74;
    short pad_76;
    short pad_78;
    short pad_7A;
    short pad_7C;
    unsigned char pad_7E[0x2];
    int pad_80;
    int pad_84;
    int pad_88;
    int pad_8C;
    int pad_90;
    short pad_94;
    short pad_96;
    short pad_98;
    short pad_9A;
    short pad_9C;
    short pad_9E;
    short pad_A0;
    unsigned char pad_A2[0x2];
    int pad_A4;
    int pad_A8;
    int pad_AC;
    int pad_B0;
    int pad_B4;
} SubmarineHudRecord;

/* Submarine radial terrain band state in data block 0: table pointers, level count and band references. */
typedef struct SubmarineRadialBandState {
    unsigned char pad_00[0x24];
    struct SubmarineFlaggedCell *pairTable;
    struct SubmarineRadialOffset *offsetTable;
    int *indexTable;
    int levelCount;
    unsigned char pad_34[0xc];
    int originX;
    int originY;
    int originZ;
    int bandOuterDistance;
    int bandStep;
    int savedLevel;
    int bandBase;
} SubmarineRadialBandState;

/* Submarine aim record: per-axis deltas and the two components used for aim angles. */
typedef struct SubmarineAimRecord {
    int deltaX;
    int deltaY;
    int deltaZ;
    unsigned char pad_0C[0x4];
    int componentB;
    int componentY;
    int componentA;
} SubmarineAimRecord;

/* Six int values per connector record, copied into the +0x10 word of each connector quad slot. */
typedef struct SubmarineConnectorQuadValues {
    int colorA;
    int colorB;
    int colorC;
    int colorD;
    int colorE;
    int colorF;
} SubmarineConnectorQuadValues;

/* View block from Gfx_GetPsxScratchpadPtr(0): 16-bit and 32-bit terms, negated on write. */
typedef struct SubmarineViewBlock {
    short rotationX;
    short rotationY;
    short rotationZ;
    unsigned char pad_06[0x2];
    int translationX;
    int translationY;
    int translationZ;
} SubmarineViewBlock;

/* One atlas quad of the submarine HUD (0x24 bytes): the arguments Submarine_DrawAtlasQuad takes,
 * in the order the HUD stores them. Every HUD quad table is an array of these. */
typedef struct SubmarineHudQuad {
    short x; /* +0x00: screen position, relative to the viewport centre */
    short y; /* +0x02 */
    short width; /* +0x04 */
    short height; /* +0x06 */
    short srcX; /* +0x08: the source rectangle in the HUD atlas */
    short srcY; /* +0x0A */
    short alpha; /* +0x0C: Submarine_DrawAtlasQuad's alpha, stored in the batch entry's first byte */
    unsigned char pad_0E[0x2];
    int depth; /* +0x10: float bits */
    int batches[4]; /* +0x14: the batch of each 256x256 atlas page: x and y < 256, x >= 256, y >= 256, both */
} SubmarineHudQuad;

/* An atlas quad drawn with its own four corner colours (0x34 bytes): the colours come before the
 * depth (two of the quads Submarine_DrawHud draws). */
typedef struct SubmarineHudColorQuad {
    short x; /* +0x00 */
    short y; /* +0x02 */
    short width; /* +0x04 */
    short height; /* +0x06 */
    short srcX; /* +0x08 */
    short srcY; /* +0x0A */
    short alpha; /* +0x0C */
    unsigned char pad_0E[0x2];
    int colors[4]; /* +0x10: Submarine_DrawAtlasQuad's color0..color3 */
    int depth; /* +0x20 */
    int batches[4]; /* +0x24 */
} SubmarineHudColorQuad;

/* g_SubmarineHudElements: two quads, the colour-quad bar, six quads (0x154 bytes). */
typedef struct SubmarineHudElements {
    SubmarineHudQuad quads[2];
    SubmarineHudColorQuad bar;
    SubmarineHudQuad quads2[6];
} SubmarineHudElements;

/* g_SubmarineHudLayout: two quads and the colour-quad bar (0x7C bytes). */
typedef struct SubmarineHudLayout {
    SubmarineHudQuad quads[2];
    SubmarineHudColorQuad bar;
} SubmarineHudLayout;

/* g_SubmarineHudFifteenElements: the fifteen quads Submarine_DrawFifteenAtlasQuadRecords draws and
 * four bytes nothing reads (0x220 bytes). */
typedef struct SubmarineHudFifteenQuads {
    SubmarineHudQuad quads[15];
    unsigned char pad_21C[0x4];
} SubmarineHudFifteenQuads;

/* g_SubmarineHudTallyElements: the eleven quads of the target tally panel, four bytes nothing reads
 * and one more quad (0x1B4 bytes). */
typedef struct SubmarineHudTallyQuads {
    SubmarineHudQuad quads[11];
    unsigned char pad_18C[0x4];
    SubmarineHudQuad single;
} SubmarineHudTallyQuads;

/* g_SubmarineHudFiveQuadRecords: the five quads Submarine_DrawFiveAtlasQuadRecords draws and four
 * bytes nothing reads (0xB8 bytes). */
typedef struct SubmarineHudFiveQuads {
    SubmarineHudQuad quads[5];
    unsigned char pad_B4[0x4];
} SubmarineHudFiveQuads;

/* Submarine HUD gouraud-rect gauge: position, size and gauge value. */
typedef struct SubmarineHudGauge {
    int x;
    int y;
    int width;
    int height;
    int colorTopLeft;
    int colorTopRight;
    int gaugeValue;
    int frameValue;
    int colorBottomLeft;
    int colorBottomRight;
} SubmarineHudGauge;

/* One 32-byte row of g_SubmarineWorkingLevelRouteTable as Submarine_FillRouteRecordTable writes it:
 * the fixed-point position and the link to the next row (read back as a SubmarineTimedEffectKey). */
typedef struct SubmarineRouteRecord {
    int posX;
    int posY;
    int posZ;
    unsigned char pad_0C[0xc];
    struct SubmarineTimedEffectKey *nextKey;
    unsigned char pad_1C[0x4];
} SubmarineRouteRecord;

/* Source object record that Submarine_CopyObjectRecordTable copies into the active table. */
typedef struct SubmarineObjectRecord {
    int posX;
    int posY;
    int posZ;
    unsigned char pad_0C[0x4];
    int slotWeight;
    short angleA;
    short angleB;
    short heightOffset;
    unsigned char pad_1A[0x2];
    int speed;
    int timer;
    int flags;      /* bit 0: the actor is active */
    int modelIndex; /* entry of g_SubmarineModelDescriptors */
    int routeIndex; /* row of g_SubmarineWorkingLevelRouteTable */
} SubmarineObjectRecord;

/* Saved submarine state: position, orientation angles (pitch/yaw/roll) and resource. */
typedef struct SubmarineStateRecord {
    int posX;
    int posY;
    int posZ;
    unsigned char pad_0C[0x4];
    int playerResource;
    short angle;
    short yaw;
    short roll;
    unsigned char pad_1A[0x2];
} SubmarineStateRecord;

/* One 0x38-byte submarine table entry, copied into the submarine globals on load. */
typedef struct SubmarineTableEntry {
    short rotation;
    unsigned char pad_02[0x2];
    short currentAngleA;
    unsigned char pad_06[0x2];
    short currentAngleB;
    unsigned char pad_0A[0x6];
    int camOffset;
    int geomScreen;
    int controlState;
    int stateFlag;
    int torpedoLaunchOffsetFactor;
    int torpedoForwardOffsetFactor;
    int bandBase;
    int bandStep;
    int bandOffset;
    unsigned char pad_34[0x4];
} SubmarineTableEntry;

/* Point of a submarine flight record, used to draw record connector quads. */
typedef struct SubmarineFlightRecordPoint {
    short anchorX;
    short anchorY;
    short zCoord;
} SubmarineFlightRecordPoint;

/* Submarine flight record connector: three shorts read when emitting connector quads. */
typedef struct SubmarineConnectorRecord {
    short connectorX;
    short baseY;
    short connectorZVal;
} SubmarineConnectorRecord;

/* Submarine HUD element table of byte values, filled by Submarine_BuildHudElementTable. */
typedef struct SubmarineHudAnimTable {
    char anim0SrcX;
    char anim0SrcY;
    char anim1SrcX;
    char anim1SrcY;
    char anim2SrcX;
    char anim2SrcY;
    char anim3SrcX;
    char anim3SrcY;
    char anim4SrcX;
    char anim4SrcY;
    char anim5SrcX;
    char anim5SrcY;
} SubmarineHudAnimTable;

/* One row of a submarine record's rotation basis, three shorts written from the angles. */
typedef struct SubmarineRecordMatrixRow {
    short elemX;
    short elemY;
    short elemZ;
    unsigned char pad_06[0x2];
} SubmarineRecordMatrixRow;

/* Three 16-bit angles used to build a submarine rotation basis. */
typedef struct SubmarineRotationAngles {
    short angle0;
    short angle1;
    short angle2;
} SubmarineRotationAngles;

/* Three 16-bit angles of a submarine record, used to build its rotation basis. */
typedef struct SubmarineRecordAngles {
    short angleX;
    short angleY;
    short angleZ;
} SubmarineRecordAngles;

/* hub SubmarineState begin */
/* The submarine minigame's state: the speed scale and grid lookup tables, five table entries the state is copied from, the player record, the camera and motion state. */
#pragma pack(push, 1)
struct SubmarineState {
    int geomScreen; /* +0x0: passed to SetGeomScreen; 128 on module entry */
    unsigned char pad_4[0x4];
    int seedVector; /* +0x8 */
    int seedVectorY; /* +0xc */
    int seedVectorZ; /* +0x10 */
    unsigned char pad_14[0x4];
    int hudVector; /* +0x18 */
    int hudScalarA; /* +0x1c */
    int hudScalarB; /* +0x20 */
    unsigned char pad_24[0x4];
    int copyVector; /* +0x28 */
    int copyVectorY; /* +0x2c */
    int copyVectorZ; /* +0x30 */
    unsigned char pad_34[0x4];
    int speedScaleTable[0x13]; /* +0x38: indexed by speedScaleIndex: -32 .. 32 */
    unsigned char pad_84[0x4];
    short lookupTableY[0x21]; /* +0x88: 33 entries, read from the middle (+0x20) by a signed grid index; copied whole (66 bytes) on module entry */
    unsigned char pad_ca[0x6];
    short lookupTableX[0x21]; /* +0xd0 */
    unsigned char pad_112[0x6];
    short lookupTableZ[0x21]; /* +0x118 */
    unsigned char pad_15a[0x6];
    short batchCountTable[0x21]; /* +0x160 */
    unsigned char pad_1a2[0x6];
    SubmarineTableEntry tableEntries[5]; /* +0x1a8: the current state takes the entry of the target it moves to */
    SubmarineStateRecord player; /* +0x2c0: the player submarine's position, resource and pitch/yaw/roll */
    short rightBasisX; /* +0x2dc */
    short rightBasisY; /* +0x2de */
    short rightBasisZ; /* +0x2e0 */
    unsigned char pad_2e2[0x2];
    short probeOffsetX; /* +0x2e4 */
    short probeOffsetY; /* +0x2e6 */
    short probeOffsetZ; /* +0x2e8 */
    unsigned char pad_2ea[0x2];
    short directionX; /* +0x2ec */
    short directionY; /* +0x2ee */
    short directionZ; /* +0x2f0 */
    unsigned char pad_2f2[0x2];
    short gridIndexY; /* +0x2f4 */
    short gridIndexJ; /* +0x2f6 */
    short gridIndexZ; /* +0x2f8 */
    unsigned char pad_2fa[0x2];
    short axisTermX; /* +0x2fc */
    short axisTermY; /* +0x2fe */
    short axisTermZ; /* +0x300 */
    unsigned char pad_302[0x2];
    short rotationStatePointer; /* +0x304 */
    short currentAngleA; /* +0x306 */
    short currentAngleB; /* +0x308 */
    unsigned char pad_30a[0x2];
    short camRotX; /* +0x30c */
    short camRotY; /* +0x30e */
    short camRotZ; /* +0x310 */
    unsigned char pad_312[0xa];
    short height; /* +0x31c */
    short aimYaw; /* +0x31e */
    short gridIndexX; /* +0x320 */
    unsigned char pad_322[0x2];
    short aimDirX; /* +0x324 */
    short orientationY; /* +0x326 */
    short orientationZ; /* +0x328 */
    unsigned char pad_32a[0x2];
    int tableIndex; /* +0x32c */
    int auxFlags; /* +0x330 */
    int camOffset; /* +0x334 */
    unsigned char pad_338[0x10];
    short referencePointX; /* +0x348 */
    short referencePointY; /* +0x34a */
    short referencePointZ; /* +0x34c */
    unsigned char pad_34e[0x2];
    int currentCellIndex; /* +0x350 */
    int controlState; /* +0x354 */
    int currentFlag; /* +0x358 */
    int motionFlags; /* +0x35c */
    int speedScaleIndex; /* +0x360 */
    int speedScale; /* +0x364 */
    int modelDescriptor; /* +0x368 */
    int cooldownTable[0x5]; /* +0x36c */
    int currentBandBase; /* +0x380 */
    int currentBandStep; /* +0x384 */
    int currentBandOffset; /* +0x388 */
};
#pragma pack(pop)
/* hub SubmarineState end */

#pragma pack(push, 1)
/* g_SubmarineHudRenderState (0x130 bytes): the selected HUD record, the eight swap-frame quads
 * Submarine_DrawSwapFrameAtlasQuads draws and the two HUD render objects. */
typedef struct SubmarineHudRenderState {
    int hudRecordPtr; /* +0x0 */
    SubmarineHudQuad swapQuads[8]; /* +0x4 */
    int renderObject; /* +0x124 */
    int renderObjectAlt; /* +0x128 */
    unsigned char pad_12c[0x4];
} SubmarineHudRenderState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct SubmarineLargeGridBuffer {
    unsigned char pad_0[0x60000];
    unsigned char initZero_60000; /* +0x60000 */
    unsigned char pad_60001[0x7];
    unsigned char initZero_60008; /* +0x60008 */
    unsigned char pad_60009[0x7];
} SubmarineLargeGridBuffer;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct SubmarineTargetDims {
    int targetWidth; /* +0x0 */
    int targetHeight; /* +0x4 */
} SubmarineTargetDims;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct SubmarineHudConfig {
    short pitchStep; /* +0x0 */
    short pitchAngle; /* +0x2 */
    short yawAngle; /* +0x4 */
    short rollAngle; /* +0x6 */
    int accumulator; /* +0x8 */
    short projectionOffsetX; /* +0xc */
    short projectionOffsetY; /* +0xe */
} SubmarineHudConfig;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct SubmarineHudParams {
    int musicVolume; /* +0x0 */
    unsigned char pad_4[0x4];
    int screenShakeTimer; /* +0x8 */
} SubmarineHudParams;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct SubmarineHudRange {
    short pitch; /* +0x0 */
    short max; /* +0x2 */
} SubmarineHudRange;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct SubmarineTimeOffsets {
    int baseTime; /* +0x0 */
    int countdownStep; /* +0x4 */
} SubmarineTimeOffsets;
#pragma pack(pop)


/* src/ff7/submarine/sub_hud_draw_and_update.cpp Submarine_UpdateTorpedoAudio: what p points to, from its reads at 4 offsets. */
#pragma pack(push, 1)
typedef struct SubmarineTorpedoAudioScratch {
    unsigned char pad_0[0x58];
    int playerDistance; /* +0x58 */
    int dotRight; /* +0x5c */
    int dotForward; /* +0x60 */
    unsigned char pad_64[0x18];
    int state; /* +0x7c */
} SubmarineTorpedoAudioScratch;
#pragma pack(pop)

/* src/ff7/submarine/sub_hud_draw_and_update.cpp Submarine_UpdateEnemyAudio: what p points to, from its reads at 5 offsets. */
#pragma pack(push, 1)
typedef struct SubmarineEnemyAudioScratch {
    unsigned char pad_0[0x34];
    int flags; /* +0x34 */
    int state; /* +0x38 */
    unsigned char pad_3c[0x4];
    int magnitude; /* +0x40 */
    int dotResultA; /* +0x44 */
    int dotResultB; /* +0x48 */
} SubmarineEnemyAudioScratch;
#pragma pack(pop)

/* One 0x70-byte target record Submarine_LaunchTorpedoRecord scans: a lock-on position and its flags. */
#pragma pack(push, 1)
typedef struct SubmarineLaunchTorpedoRecordRecord {
    int posX; /* +0x0 */
    int posY; /* +0x4 */
    int posZ; /* +0x8 */
    unsigned char pad_c[0x2c];
    int flags; /* +0x38: 0x800 marks a target a launch can lock on */
    unsigned char pad_3C[0x34];
} SubmarineLaunchTorpedoRecordRecord;
#pragma pack(pop)

/* The submarine level record of D_0098A6E8.obj (src/ff7/submarine/sub_terrain_data2.cpp): 0x800 bytes, which
 * sub_module_lifecycle.cpp copies whole with Mem_CopyBytesFast(2048, ...). Six of them follow the six maps. */
typedef struct SubmarineLevelBlock {
    int header[21];     /* +0x000: level parameters */
    int actors[12][12]; /* +0x054: actor definitions (Submarine_CopyObjectRecordTable) */
    int unused[347];    /* +0x294: zero */
} SubmarineLevelBlock;

#ifdef __cplusplus
#pragma pack(push, 1)
typedef struct {
    short f0;
    short f1;
    short f2;
    short f3;
} SubInitData0;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    SubInitData0 f0;
} SubInitData1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    unsigned char f0;
    unsigned char f1;
    unsigned char f2;
    unsigned char f3;
} SubInitData2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    SubInitData2 f0;
} SubInitData3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    short f0;
    short f1;
    short f2;
    short f3;
    short f4;
    short f5;
    short f6;
    short f7;
    short f8;
    int f9;
    int f10;
    int f11;
} SubInitData4;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    int f0;
    int f1;
    int f2;
    int f3;
} SubInitData5;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    SubInitData5 f0;
} SubInitData6;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    SubInitData0 f0;
    SubInitData0 f1;
    SubInitData0 f2;
    SubInitData0 f3;
    SubInitData0 f4;
    SubInitData0 f5;
} SubInitData7;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    unsigned char f0;
    unsigned char f1;
    unsigned char f2;
    unsigned char f3;
    unsigned char f4;
    unsigned char f5;
    unsigned char f6;
    unsigned char f7;
    unsigned char f8;
    unsigned char f9;
    unsigned char f10;
    unsigned char f11;
    unsigned char f12;
    unsigned char f13;
    unsigned char f14;
    unsigned char f15;
    unsigned char f16;
    unsigned char f17;
    unsigned char f18;
    unsigned char f19;
    unsigned char f20;
    unsigned char f21;
    unsigned char f22;
    unsigned char f23;
    unsigned char f24;
    unsigned char f25;
    unsigned char f26;
    unsigned char f27;
    unsigned char f28;
    unsigned char f29;
    unsigned char f30;
    unsigned char f31;
} SubInitData8;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    int f0;
    int f1;
    int f2;
    int f3;
    int f4;
    int f5;
    int f6;
    int f7;
} SubInitData9;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    int f0;
    short f1;
    short f2;
    short f3;
    short f4;
} SubInitData10;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    int f0;
    int f1;
    int f2;
    int f3;
    int f4;
    int f5;
    int f6;
    int f7;
    short f8;
    short f9;
    short f10;
    short f11;
} SubInitData11;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    int f0;
    int f1;
    int f2;
    int f3;
    int f4;
    int f5;
    int f6;
    int f7;
    int f8;
    int f9;
    int f10;
} SubInitData12;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    SubInitData5 f0;
    SubInitData10 f1;
    SubInitData0 f2;
    SubInitData0 f3;
    SubInitData11 f4;
    SubInitData12 f5;
    SubInitData10 f6;
    SubInitData0 f7;
    SubInitData0 f8;
    SubInitData11 f9;
    SubInitData12 f10;
    SubInitData10 f11;
    SubInitData0 f12;
    SubInitData0 f13;
    SubInitData11 f14;
    SubInitData12 f15;
    SubInitData10 f16;
    SubInitData0 f17;
    SubInitData0 f18;
    SubInitData11 f19;
    SubInitData12 f20;
    SubInitData10 f21;
    SubInitData0 f22;
    SubInitData0 f23;
    SubInitData11 f24;
    SubInitData12 f25;
    SubInitData10 f26;
    SubInitData0 f27;
    SubInitData0 f28;
    SubInitData11 f29;
    SubInitData12 f30;
    SubInitData10 f31;
    SubInitData0 f32;
    SubInitData0 f33;
    SubInitData11 f34;
    SubInitData12 f35;
    SubInitData10 f36;
    SubInitData0 f37;
    SubInitData0 f38;
    SubInitData11 f39;
    SubInitData12 f40;
    SubInitData10 f41;
    SubInitData0 f42;
    SubInitData0 f43;
    SubInitData11 f44;
    SubInitData12 f45;
    SubInitData10 f46;
    SubInitData0 f47;
    SubInitData0 f48;
    SubInitData11 f49;
    SubInitData12 f50;
    SubInitData10 f51;
    SubInitData0 f52;
    SubInitData0 f53;
    SubInitData11 f54;
    SubInitData12 f55;
    SubInitData10 f56;
    SubInitData0 f57;
    SubInitData0 f58;
    SubInitData11 f59;
    int f60;
    int f61;
    int f62;
    int f63;
    int f64;
    int f65;
    int f66;
} SubInitData13;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    int f0;
    int f1;
    int f2;
    int f3;
    int f4;
    int f5;
    int f6;
    int f7;
    int f8;
    int f9;
    int f10;
    int f11;
    int f12;
    int f13;
    int f14;
} SubInitData14;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    int f0;
    int f1;
    int f2;
    int f3;
    int f4;
    int f5;
    int f6;
} SubInitData15;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    int f0;
    int f1;
    int f2;
    int f3;
    int f4;
    int f5;
    int f6;
    int f7;
    int f8;
} SubInitData16;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    SubInitData14 f0;
    SubInitData10 f1;
    SubInitData15 f2;
    SubInitData10 f3;
    SubInitData16 f4;
    SubInitData10 f5;
    SubInitData16 f6;
    SubInitData10 f7;
    SubInitData16 f8;
    SubInitData10 f9;
    SubInitData16 f10;
    SubInitData10 f11;
    SubInitData16 f12;
    SubInitData10 f13;
    SubInitData16 f14;
    SubInitData10 f15;
    SubInitData16 f16;
    SubInitData10 f17;
    SubInitData16 f18;
    SubInitData10 f19;
    SubInitData16 f20;
    SubInitData10 f21;
    SubInitData16 f22;
    SubInitData10 f23;
    SubInitData16 f24;
    SubInitData10 f25;
    int f26;
    int f27;
    int f28;
    int f29;
    int f30;
    int f31[347];
} SubInitData17;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    SubInitData5 f0;
    SubInitData9 f1;
    SubInitData9 f2;
    SubInitData9 f3;
    SubInitData9 f4;
    SubInitData9 f5;
    SubInitData9 f6;
    SubInitData9 f7;
    SubInitData9 f8;
    SubInitData9 f9;
    SubInitData9 f10;
    SubInitData9 f11;
    SubInitData9 f12;
    SubInitData9 f13;
    SubInitData9 f14;
    SubInitData9 f15;
    SubInitData9 f16;
    SubInitData9 f17;
    SubInitData9 f18;
    SubInitData9 f19;
    SubInitData9 f20;
    SubInitData9 f21;
    SubInitData9 f22;
    SubInitData9 f23;
    SubInitData9 f24;
    SubInitData9 f25;
    SubInitData9 f26;
    SubInitData9 f27;
    SubInitData9 f28;
    SubInitData9 f29;
    SubInitData9 f30;
    SubInitData9 f31;
    SubInitData9 f32;
    SubInitData9 f33;
    SubInitData9 f34;
    SubInitData9 f35;
    SubInitData9 f36;
    SubInitData9 f37;
    SubInitData9 f38;
    SubInitData9 f39;
    SubInitData9 f40;
    SubInitData9 f41;
    SubInitData9 f42;
    SubInitData9 f43;
    SubInitData9 f44;
    SubInitData9 f45;
    SubInitData9 f46;
    SubInitData9 f47;
    SubInitData9 f48;
    SubInitData9 f49;
    SubInitData9 f50;
    SubInitData9 f51;
    SubInitData9 f52;
    SubInitData9 f53;
    SubInitData9 f54;
    SubInitData9 f55;
    SubInitData9 f56;
    SubInitData9 f57;
    SubInitData9 f58;
    SubInitData9 f59;
    SubInitData9 f60;
    SubInitData9 f61;
    SubInitData9 f62;
    SubInitData9 f63;
    SubInitData9 f64;
    SubInitData9 f65;
    SubInitData9 f66;
    SubInitData9 f67;
    SubInitData9 f68;
    SubInitData9 f69;
    SubInitData9 f70;
    SubInitData9 f71;
    SubInitData9 f72;
    SubInitData9 f73;
    SubInitData9 f74;
    SubInitData9 f75;
    SubInitData9 f76;
    SubInitData9 f77;
    SubInitData9 f78;
    SubInitData9 f79;
    SubInitData9 f80;
    SubInitData9 f81;
    SubInitData9 f82;
    SubInitData9 f83;
    SubInitData9 f84;
    SubInitData9 f85;
    SubInitData9 f86;
    SubInitData9 f87;
    SubInitData9 f88;
    SubInitData9 f89;
    SubInitData9 f90;
    SubInitData9 f91;
    SubInitData9 f92;
    SubInitData9 f93;
    SubInitData9 f94;
    SubInitData9 f95;
    SubInitData9 f96;
    SubInitData9 f97;
    SubInitData9 f98;
    SubInitData9 f99;
    SubInitData9 f100;
    SubInitData9 f101;
    int f102;
    int f103;
    int f104;
    int f105;
    int f106[208];
} SubInitData18;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    SubInitData5 f0;
    SubInitData9 f1;
    SubInitData9 f2;
    SubInitData9 f3;
    SubInitData9 f4;
    SubInitData9 f5;
    SubInitData9 f6;
    SubInitData9 f7;
    SubInitData9 f8;
    SubInitData9 f9;
    SubInitData9 f10;
    SubInitData9 f11;
    SubInitData9 f12;
    SubInitData9 f13;
    SubInitData9 f14;
    SubInitData9 f15;
    SubInitData9 f16;
    SubInitData9 f17;
    SubInitData9 f18;
    SubInitData9 f19;
    SubInitData9 f20;
    SubInitData9 f21;
    SubInitData9 f22;
    SubInitData9 f23;
    SubInitData9 f24;
    SubInitData9 f25;
    SubInitData9 f26;
    SubInitData9 f27;
    SubInitData9 f28;
    SubInitData9 f29;
    SubInitData9 f30;
    SubInitData9 f31;
    SubInitData9 f32;
    SubInitData9 f33;
    SubInitData9 f34;
    SubInitData9 f35;
    SubInitData9 f36;
    SubInitData9 f37;
    SubInitData9 f38;
    SubInitData9 f39;
    SubInitData9 f40;
    SubInitData9 f41;
    SubInitData9 f42;
    SubInitData9 f43;
    SubInitData9 f44;
    SubInitData9 f45;
    SubInitData9 f46;
    SubInitData9 f47;
    SubInitData9 f48;
    SubInitData9 f49;
    SubInitData9 f50;
    SubInitData9 f51;
    SubInitData9 f52;
    SubInitData9 f53;
    SubInitData9 f54;
    SubInitData9 f55;
    SubInitData9 f56;
    SubInitData9 f57;
    SubInitData9 f58;
    SubInitData9 f59;
    SubInitData9 f60;
    SubInitData9 f61;
    SubInitData9 f62;
    SubInitData9 f63;
    SubInitData9 f64;
    int f65;
    int f66;
    int f67;
    int f68;
    int f69[504];
} SubInitData19;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    SubInitData5 f0;
    SubInitData9 f1;
    SubInitData9 f2;
    SubInitData9 f3;
    SubInitData9 f4;
    SubInitData9 f5;
    SubInitData9 f6;
    SubInitData9 f7;
    SubInitData9 f8;
    SubInitData9 f9;
    SubInitData9 f10;
    SubInitData9 f11;
    SubInitData9 f12;
    SubInitData9 f13;
    SubInitData9 f14;
    SubInitData9 f15;
    SubInitData9 f16;
    SubInitData9 f17;
    SubInitData9 f18;
    SubInitData9 f19;
    SubInitData9 f20;
    SubInitData9 f21;
    SubInitData9 f22;
    SubInitData9 f23;
    SubInitData9 f24;
    SubInitData9 f25;
    SubInitData9 f26;
    SubInitData9 f27;
    SubInitData9 f28;
    SubInitData9 f29;
    SubInitData9 f30;
    SubInitData9 f31;
    SubInitData9 f32;
    SubInitData9 f33;
    SubInitData9 f34;
    SubInitData9 f35;
    SubInitData9 f36;
    SubInitData9 f37;
    SubInitData9 f38;
    SubInitData9 f39;
    SubInitData9 f40;
    SubInitData9 f41;
    SubInitData9 f42;
    int f43;
    int f44;
    int f45;
    int f46;
    int f47[680];
} SubInitData20;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    SubInitData5 f0;
    SubInitData9 f1;
    SubInitData9 f2;
    SubInitData9 f3;
    SubInitData9 f4;
    SubInitData9 f5;
    SubInitData9 f6;
    SubInitData9 f7;
    SubInitData9 f8;
    SubInitData9 f9;
    SubInitData9 f10;
    SubInitData9 f11;
    SubInitData9 f12;
    SubInitData9 f13;
    SubInitData9 f14;
    SubInitData9 f15;
    SubInitData9 f16;
    SubInitData9 f17;
    SubInitData9 f18;
    SubInitData9 f19;
    SubInitData9 f20;
    SubInitData9 f21;
    SubInitData9 f22;
    SubInitData9 f23;
    SubInitData9 f24;
    SubInitData9 f25;
    SubInitData9 f26;
    SubInitData9 f27;
    SubInitData9 f28;
    SubInitData9 f29;
    SubInitData9 f30;
    SubInitData9 f31;
    SubInitData9 f32;
    SubInitData9 f33;
    SubInitData9 f34;
    SubInitData9 f35;
    SubInitData9 f36;
    SubInitData9 f37;
    SubInitData9 f38;
    SubInitData9 f39;
    SubInitData9 f40;
    SubInitData9 f41;
    SubInitData9 f42;
    SubInitData9 f43;
    SubInitData9 f44;
    SubInitData9 f45;
    SubInitData9 f46;
    SubInitData9 f47;
    SubInitData9 f48;
    SubInitData9 f49;
    SubInitData9 f50;
    SubInitData9 f51;
    SubInitData9 f52;
    SubInitData9 f53;
    SubInitData9 f54;
    SubInitData9 f55;
    SubInitData9 f56;
    int f57;
    int f58;
    int f59;
    int f60;
    int f61[568];
} SubInitData21;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    int f0;
    int f1;
    int f2;
    int f3;
    int f4;
    int f5;
    int f6;
    int f7;
    int f8;
    int f9;
    int f10;
    int f11;
    int f12;
    int f13;
    int f14;
    int f15;
    int f16;
    int f17;
    int f18;
} SubInitData22;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    short f0;
    short f1;
    short f2;
    short f3;
    short f4;
    short f5;
    short f6;
    short f7;
    short f8;
    short f9;
    short f10;
    short f11;
    short f12;
    short f13;
    short f14;
    short f15;
    short f16;
    short f17;
    short f18;
    short f19;
    short f20;
    short f21;
    short f22;
    short f23;
    short f24;
    short f25;
    short f26;
    short f27;
    short f28;
    short f29;
    short f30;
    short f31;
    short f32;
} SubInitData23;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    int f0;
    int f1;
    int f2;
    int f3;
    int f4;
    int f5;
    int f6;
    int f7;
    int f8;
    int f9;
} SubInitData24;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    SubInitData5 f0;
    SubInitData24 f1;
    SubInitData5 f2;
    SubInitData24 f3;
    SubInitData5 f4;
    SubInitData24 f5;
    SubInitData5 f6;
    SubInitData24 f7;
    SubInitData5 f8;
    SubInitData24 f9;
} SubInitData25;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    SubInitData5 f0;
    SubInitData10 f1;
    SubInitData0 f2;
    SubInitData0 f3;
    SubInitData0 f4;
    SubInitData0 f5;
    SubInitData0 f6;
    SubInitData0 f7;
    SubInitData0 f8;
    SubInitData0 f9;
    SubInitData0 f10;
    SubInitData0 f11;
    SubInitData15 f12;
    SubInitData0 f13;
    int f14;
    int f15;
    int f16;
    int f17;
    int f18;
    int f19;
    int f20;
    int f21;
    int f22;
    int f23;
    int f24;
} SubInitData26;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    int f0;
    int f1;
    int f2;
    int f3;
    int f4;
    int f5;
    int f6;
    int f7;
    int f8;
    int f9;
    int f10;
    int f11;
    int f12;
    int f13;
    int f14;
    int f15;
    int f16;
    int f17;
    int f18;
    int f19;
    int f20;
    int f21;
    int f22;
    int f23;
    int f24;
    int f25;
} SubInitData27;
#pragma pack(pop)

#endif


#endif /* FF7_TYPES_SUBMARINE_H */
