// src/ff7/highway/highway_field_frames_motion.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Frame and field code for the highway minigame: world-space frames with their quad
//   geometry, a field context that plays motion commands and draws its render groups, and a field
//   manager that samples and interpolates path positions. A few fixed-point math helpers and a slot
//   list sit alongside.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/highway_palette.h"
#include "ff7/highway_psxdata.h"
#include <math.h>
#include <stdlib.h>
#include "ff7/types/highway.h"
#include "ff7/enums/highway.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "ff7/api/highway.h"
#include "ff7/api/snobo.h"
#include "lib/api/graphics.h"
#include "lib/pfile.h"

#pragma intrinsic(strlen, strcpy, strcat, strcmp)
extern "C" {
// clang-format off
// The two null-terminated TIM name lists HighwayFieldContext passes to the road and wall HighwayRenderGroups.
// GLOBAL: FF7 0x00909568
const char *g_HighwayTrackTimNames1[12] = {
    // STRING: FF7 0x009095bc
    "fpaa.tim",
    // STRING: FF7 0x009095c8
    "ptaa.tim",
    // STRING: FF7 0x009095d4
    "qtaa.tim",
    // STRING: FF7 0x009095e0
    "rtaa.tim",
    // STRING: FF7 0x009095ec
    "epaa.tim",
    // STRING: FF7 0x009095f8
    "staa.tim",
    // STRING: FF7 0x00909604
    "ttaa.tim",
    // STRING: FF7 0x00909610
    "utaa.tim",
    // STRING: FF7 0x0090961c
    "vtaa.tim",
    // STRING: FF7 0x00909628
    "wtaa.tim",
    // STRING: FF7 0x00909634
    "xtaa.tim",
    0,
};
// GLOBAL: FF7 0x00909598
const char *g_HighwayTrackTimNames2[9] = {
    // STRING: FF7 0x00909640
    "ytaa.tim",
    // STRING: FF7 0x0090964c
    "ztaa.tim",
    // STRING: FF7 0x00909658
    "auaa.tim",
    // STRING: FF7 0x00909664
    "buaa.tim",
    // STRING: FF7 0x00909670
    "cuaa.tim",
    // STRING: FF7 0x0090967c
    "duaa.tim",
    // STRING: FF7 0x00909688
    "euaa.tim",
    // STRING: FF7 0x00909694
    "fuaa.tim",
    0,
};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(atan2, sqrt)


#pragma intrinsic(abs)
} // extern "C"

/*
 * Zeroes the position, the three angles, the speed, mode and road width, the ten info shorts, the
 * twenty actor pointers and the actor count, and sets the matrix to identity. Padding is untouched.
 */
// FUNCTION: FF7 0x00656eb0
HighwayTrackFrame::HighwayTrackFrame()
{
    int i;

    this->posX = 0;
    this->posY = 0;
    this->posZ = 0;
    this->angleX = 0;
    this->angleY = 0;
    this->angleZ = 0;
    SetIdentityPsxMatrix(&this->matrix);
    this->speed = 0;
    this->mode = 0;
    this->roadWidth = 0;
    for (i = 0; i < 10; i++) {
        this->info[i] = 0;
    }
    for (i = 0; i < 20; i++) {
        this->actors[i] = 0;
    }
    this->actorCount = 0;
}

/*
 * Copies the four-int global base record, subtracts its first three ints from this position,
 * converts those differences through float back to int and stores them as the matrix translation.
 * Converts the matrix to float4x4, copies it to result and returns result.
 */
// FUNCTION: FF7 0x00656f7d
HighwayMat64 *HighwayTrackFrame::toWorldMatrix(HighwayMat64 *result)
{
    HighwayMat64 s;
    int base[4];

    *(HighwayWords4 *)base = *(HighwayWords4 *)(((int)g_pHighwaySceneBlock) + 8);
    this->matrix.t[0] = (int)((float)(this->posX - base[0]) * 1.0f);
    this->matrix.t[1] = (int)((float)(this->posY - base[1]) * 1.0f);
    this->matrix.t[2] = (int)((float)(this->posZ - base[2]) * 1.0f);
    GtePsxMatrixToFloat4x4ColVec(&this->matrix, &s);
    *result = s;
    return result;
}

/*
 * Stores translation relative to the three global origin ints, constructs four short points using
 * integer halves of the road width and fixed y values zero/minus 90, installs this
 * rotation/translation matrices and transforms the points. Writes three float coordinates per
 * point; transform flags are ignored and installed matrices remain active.
 */
// FUNCTION: FF7 0x00657031
void HighwayTrackFrame::buildQuad(float *result)
{
    int origin[3];
    int a6;
    int i;
    short localPts[4][4];
    int outPts[4][4];
    int currTransformFlag;

    origin[0] = g_pHighwaySceneBlock->offsetX;
    origin[1] = g_pHighwaySceneBlock->offsetY;
    origin[2] = g_pHighwaySceneBlock->offsetZ;
    this->matrix.t[0] = this->posX - origin[0];
    this->matrix.t[1] = this->posY - origin[1];
    this->matrix.t[2] = this->posZ - origin[2];

    memset(localPts, 0, sizeof(localPts));
    localPts[0][0] = -(this->roadWidth / 2 - 30);
    localPts[1][0] = this->roadWidth / 2 - 30;
    localPts[2][0] = -(this->roadWidth / 2);
    localPts[2][1] = -90;
    localPts[3][0] = this->roadWidth / 2;
    localPts[3][1] = -90;

    SetRotMatrix(&this->matrix);
    SetTransMatrix(&this->matrix);
    for (i = 0; i < 4; i++) {
        RotTransPoint(localPts[i], outPts[i], &currTransformFlag);
        ((HighwayQuadVertex *)result)[i].x = (float)outPts[i][0] * 1.0f;
        ((HighwayQuadVertex *)result)[i].y = (float)outPts[i][1] * 1.0f;
        ((HighwayQuadVertex *)result)[i].z = (float)outPts[i][2] * 1.0f;
    }
}

// Returns the psxdata resource at table index, or null when its offset is past the data size.
static __inline unsigned char *HighwayResolveFieldResource(int index)
{
    unsigned char *resource;
    int tableOffset;
    tableOffset = g_HighwayPsxDataSet.adrOffsets[index];
    if (tableOffset >= g_HighwayPsxDataSet.binSize) {
        resource = 0;
    } else {
        resource = (unsigned char *)(g_HighwayPsxDataSet.binData + tableOffset);
    }
    return resource;
}
/*
 * Resolves resource 0xF or 0x1D according to the global flag, allocates 80 frames, creates render
 * groups of 80 and 160 using the two global resource pointers, allocates a 0x4E0 block and calls
 * reset. Failure handling is not present here.
 */
// FUNCTION: FF7 0x006571a3
HighwayFieldContext::HighwayFieldContext()
{
    if (g_HighwayGameMode == 0) {
        this->dataBase = (HighwayMotionCommand *)HighwayResolveFieldResource(0xF);
    } else {
        this->dataBase = (HighwayMotionCommand *)HighwayResolveFieldResource(0x1D);
    }
    this->frames = new HighwayTrackFrame[0x50];
    this->roadGroup = new HighwayRenderGroup((int *)g_HighwayTrackTimNames1, 0x50);
    this->wallGroup = new HighwayRenderGroup((int *)g_HighwayTrackTimNames2, 0xA0);
    this->fieldBlock = (HighwayPaletteFieldBlock *)operator new(0x4E0);
    reset();
}

// Releases the frame block.
static __inline void HighwayFreeFieldScratch(HighwayFieldContext *context)
{
    int u = (int)context->frames;
    operator delete((void *)u);
}
/*
 * Releases the palette field block, deletes the road and wall render groups in order, then
 * releases the frame block through HighwayFreeFieldScratch.
 */
// FUNCTION: FF7 0x00657382
HighwayFieldContext::~HighwayFieldContext()
{
    int p = (int)this->fieldBlock;
    operator delete((void *)p);
    delete this->roadGroup;
    delete this->wallGroup;
    HighwayFreeFieldScratch(this);
}

/*
 * Resets field-context state, frame slots and active events, then refreshes each frame.
 * Resets the owned field block, copies the data base into the command cursor, and clears the
 * positions, counters, arrays and flags while setting commandPending to one.
 * Selects resource offset-table entry 16 or 30 according to the global mode, using null when its
 * signed offset is at or beyond the global size. Clears the ten info shorts of each of the 80
 * frames and the position of each of the ten anchors. Finally assigns each frame's segment base
 * to its index modulo eight, invokes update once per frame, and increments the global counter
 * 80 times.
 */
// FUNCTION: FF7 0x00657440
void HighwayFieldContext::reset()
{
    int frameIndex;
    int recordIdx;
    int fieldIdx;
    int sampleIdx;
    int tableOffset;
    unsigned char *tablePtr;
    int altRawOffset;
    unsigned char *altTableAddress;

    this->fieldBlock->reset();
    this->cursor = this->dataBase;
    this->angle = 0;
    this->angleTotal = 0;
    this->offsetX = 0;
    this->offsetY = 0;
    this->offsetZ = 0;
    this->commandPending = 1;
    this->counter = 0;
    this->angleStep = 0;
    this->angle = 0;
    this->targetB = 0;
    this->prevTargetB = 0;
    this->stepB = 0;
    this->posB = 0;
    this->targetA = 0;
    this->prevTargetA = 0;
    this->stepA = 0;
    this->posA = 0;

    if (g_HighwayGameMode == 0) {
        tableOffset = *(int *)((char *)g_HighwayPsxDataSet.adrOffsets + (UNFOLDED(0x10) << 2));
        if (tableOffset >= g_HighwayPsxDataSet.binSize) {
            tablePtr = 0;
        } else {
            tablePtr = (unsigned char *)(g_HighwayPsxDataSet.binData + tableOffset);
        }
        this->table = tablePtr;
    } else {
        altRawOffset = *(int *)((char *)g_HighwayPsxDataSet.adrOffsets + (UNFOLDED(0x1E) << 2));
        if (altRawOffset >= g_HighwayPsxDataSet.binSize) {
            altTableAddress = 0;
        } else {
            altTableAddress = (unsigned char *)(g_HighwayPsxDataSet.binData + altRawOffset);
        }
        this->table = altTableAddress;
    }

    this->frameIndex = 0;
    this->skipLatch = 0;
    this->entryCount = 0;
    this->entryCountCopy = 0;
    this->cycleIndex = 0;
    this->cycleIndexB = 0;
    for (sampleIdx = 0; sampleIdx < 0x50; sampleIdx++) {
        for (fieldIdx = 0; fieldIdx < 0xA; fieldIdx++) {
            this->frames[sampleIdx].info[fieldIdx] = 0;
        }
    }
    for (recordIdx = 0; recordIdx < 0xA; recordIdx++) {
        this->anchors[recordIdx].x = 0;
        this->anchors[recordIdx].y = 0;
        this->anchors[recordIdx].z = 0;
    }
    for (frameIndex = 0; frameIndex < 0x50; frameIndex++) {
        this->frames[frameIndex].info[HIGHWAY_FRAME_INFO_SEGMENT_BASE] = frameIndex % 8;
        update();
        g_HighwayTrackProgress++;
    }
}

/*
 * Advances a field context through 12-byte motion commands, initializing interpolation velocities,
 * angular motion and animation-table cursors when a new command is pending. Both animation cursors
 * wrap using the first sequence length; the current frame can latch the skip flag on value 4.
 * Integrates angular and linear motion, transforms a forward vector into accumulated position and
 * writes one HighwayTrackFrame with position, metadata, orientation and cleared translation
 * fields. Advances actors and wraps the frame index through 80 entries. Command duration and
 * animation sequence lengths are unchecked division/modulo divisors.
 */
// FUNCTION: FF7 0x006576b3
void HighwayFieldContext::update()
{
    short forwardVector[4] = {0, 0, 0x100};
    short movedVec[4] = {0, 0, 0x100};
    short angles[4] = {0, 0, 0};
    int pitchAngleScalar;
    char mtx[32];
    int lean;
    int nextSpeed;
    HighwayTrackFrame *pRecord;
    int velocity;
    int flatSpeed;
    short entryOffset;
    Matrix30SetIdentity((struct SnoboMatrix *)mtx);
    if (this->commandPending) {
        this->prevTargetA = this->targetA;
        this->posA = 0;
        this->prevTargetB = this->targetB;
        this->posB = 0;
        this->stepCount = this->cursor->stepCount;
        this->mode = this->cursor->mode;
        this->roadWidth = this->cursor->roadWidth;
        this->targetB = this->cursor->targetB;
        this->targetA = this->cursor->targetA;
        this->tableIndex = this->cursor->tableIndex;
        velocity = this->cursor->speed;
        nextSpeed = (velocity * velocity) << 12;
        flatSpeed = nextSpeed > 0x400 ? Mini_HighwaySqrtFixed12(nextSpeed - 0x400) : 0;
        this->stepB = ((this->targetB - this->prevTargetB) << 16) / this->stepCount;
        this->stepA = ((this->targetA - this->prevTargetA) << 16) / this->stepCount;
        this->angleStep = Mini_HighwayAtan2Angle4096(0x800, flatSpeed) << 1;
        this->commandPending = 0;
        this->angleTotal = 0;
        this->speed = velocity;
        this->cycleIndex = 0;
        this->cycleIndexB = 0;
        this->entryCount = *(this->table + this->tableIndex + 1);
        this->entryCountCopy = *(this->table + this->tableIndex + 1);
        entryOffset = *(this->table + *this->table + this->tableIndex * 2 + 1);
        this->entry = this->table + entryOffset;
        this->entryBase = this->table + entryOffset;
        this->cursor++;
        (this->counter)++;
    }
    (this->cycleIndex)++;
    this->cycleIndex %= this->entryCount;
    (this->cycleIndexB)++;
    this->cycleIndexB %= this->entryCount;
    this->frames[this->frameIndex].info[HIGHWAY_FRAME_INFO_SEQUENCE_ENTRY] = *(this->entry + this->cycleIndex);
    if (this->skipLatch) {
        this->frames[this->frameIndex].info[HIGHWAY_FRAME_INFO_SKIPPED] = 1;
    }
    if (this->frames[this->frameIndex].info[HIGHWAY_FRAME_INFO_SEQUENCE_ENTRY] == 4) {
        this->skipLatch = 1;
    }
    if (this->mode == 1) {
        this->angle += this->angleStep;
        this->angleTotal += this->angleStep;
    }
    if (this->mode == 2) {
        this->angle -= this->angleStep;
        this->angleTotal += this->angleStep;
    }
    (this->stepCount)--;
    if (!this->stepCount) {
        this->commandPending = 1;
    }
    if (this->angle < 0) {
        this->angle += 0x1000;
    }
    this->angle %= 0x1000;
    this->posA += this->stepA;
    lean = this->prevTargetA + (this->posA >> 16);
    this->posB += this->stepB;
    pitchAngleScalar = this->prevTargetB + (this->posB >> 16);
    angles[0] = pitchAngleScalar;
    angles[1] = (short)this->angle;
    angles[2] = 0;
    Matrix30SetRotationFromAnglesYXZ((struct SnowboardAngles *)angles, mtx);
    PsxApplyMatrix33ToShortVector(mtx, forwardVector, movedVec);
    this->offsetX += movedVec[0];
    this->offsetY += movedVec[1];
    this->offsetZ += movedVec[2];
    pRecord = &this->frames[this->frameIndex];
    pRecord->posX = this->offsetX;
    pRecord->posY = this->offsetY;
    pRecord->posZ = this->offsetZ;
    pRecord->speed = this->speed;
    pRecord->mode = this->mode;
    pRecord->roadWidth = this->roadWidth;
    pRecord->angleX = 0;
    pRecord->angleY = (short)this->angle;
    pRecord->angleZ = lean;
    Matrix30SetRotationFromAngles((struct SnowboardAngles *)&pRecord->angleX, &pRecord->matrix);
    pRecord->matrix.t[0] = 0;
    pRecord->matrix.t[1] = 0;
    pRecord->matrix.t[2] = 0;
    pRecord->angleX = pitchAngleScalar;
    ((HighwayFieldManager *)this)->advanceActors();
    (this->frameIndex)++;
    this->frameIndex %= 0x50;
}
extern "C" {

/*
 * Computes a square root in 12-bit fixed-point form.
 */
// FUNCTION: FF7 0x00657c42
int __cdecl Mini_HighwaySqrtFixed12(int fixedValue)
{
    return (int)(sqrt(fixedValue / 4096.0) * 4096.0);
}

/*
 * Converts an integer-vector angle from atan2 into a 4096-unit turn.
 */
// FUNCTION: FF7 0x00657c60
int __cdecl Mini_HighwayAtan2Angle4096(int dy, int dx)
{
    return (int)(atan2((double)dy, (double)dx) * 4096.0 / 6.28318530718);
}
} // extern "C"

/*
 * Selects the active frame, calls Mini_HighwayActorUnlinkAndRelease for each of its actors, then
 * clears the actor count.
 */
// FUNCTION: FF7 0x00657c81
void HighwaySlotList::clearActive()
{
    HighwayTrackFrame *entry;
    int i;

    entry = this->slots + this->activeIndex;
    for (i = 0; i < entry->actorCount; i++) {
        Mini_HighwayActorUnlinkAndRelease(entry->actors[i]);
    }
    entry->actorCount = 0;
}

/*
 * Draws successive field frames through two render groups.
 * Builds starting quad then walks next 79 cyclic frames. Skips frames with a nonzero skipped
 * flag without updating previous quad; others draw three segment faces and render listed actors, then
 * copy current quad to previous. Near flag uses loop index below 40. Resets both render groups
 * afterward.
 */
// FUNCTION: FF7 0x00657ce2
void HighwayFieldContext::draw()
{
    // These spellings reproduce VC5's two array blocks and five scalar slots.
    int frameIndex;
    HighwayFloat3 lastQuad[4];
    HighwayTrackFrame *pFrame = 0;
    HighwayFloat3 curQuad[4];
    int actorIndex;
    int vertIndex;
    char nearFlag;

    pFrame = &this->frames[g_HighwayTrackProgress % 0x50];
    pFrame->buildQuad(&lastQuad[0].x);
    for (frameIndex = 1; frameIndex < 0x50; frameIndex++) {
        nearFlag = frameIndex < 0x28;
        pFrame = &this->frames[(g_HighwayTrackProgress + frameIndex) % 0x50];
        if (pFrame->info[HIGHWAY_FRAME_INFO_SKIPPED] != 0) {
            continue;
        }
        pFrame->buildQuad(&curQuad[0].x);
        this->roadGroup
            ->drawSegment(pFrame->info[HIGHWAY_FRAME_INFO_SEQUENCE_ENTRY], frameIndex - 1, curQuad, lastQuad, curQuad + 1,
                          lastQuad + 1, 0, nearFlag);
        this->wallGroup
            ->drawSegment(pFrame->info[HIGHWAY_FRAME_INFO_SEGMENT_BASE], frameIndex - 1, curQuad, curQuad + 2, lastQuad,
                          lastQuad + 2, 0, nearFlag);
        this->wallGroup
            ->drawSegment(pFrame->info[HIGHWAY_FRAME_INFO_SEGMENT_BASE], frameIndex + 0x4F, lastQuad + 1,
                          lastQuad + 3, curQuad + 1, curQuad + 3, 1, nearFlag);
        for (actorIndex = 0; actorIndex < pFrame->actorCount; actorIndex++) {
            pFrame->actors[actorIndex]->render(0);
        }
        for (vertIndex = 0; vertIndex < 4; vertIndex++) {
            lastQuad[vertIndex] = curQuad[vertIndex];
        }
    }
    this->roadGroup->reset();
    this->wallGroup->reset();
}

/*
 * Uses path high bits and low byte to select successive modulo 80 frames. Transforms a vector with
 * first component unused shifted 18 through each matrix, shifts transformed components 18 and adds
 * frame position. Interpolates each coordinate by the low-byte fraction shifted eight and writes
 * position. Despite its name unused affects output.
 */
// FUNCTION: FF7 0x00657e9e
void HighwayFieldManager::samplePosition(int path, int unused, int *position)
{
    int fr37;
    int tf29[4];
    int ts6[4];
    HighwayTrackFrame *s2;
    int v;
    HighwayTrackFrame *f8;
    int x;
    int ps15[3];
    int pf4[3];
    int in19[4] = {unused << 18, 0, 0};
    int fx4;

    fr37 = path >> 8;
    fx4 = path & 0xFF;
    f8 = (HighwayTrackFrame *)this->frames + fr37 % 0x50;
    s2 = (HighwayTrackFrame *)this->frames + (fr37 + 1) % 0x50;
    PsxApplyMatrix33ToIntVector((struct SnoboMatrix *)&f8->matrix, (struct SnowboardSpawnPos *)in19, (struct SnowboardVec *)tf29);
    PsxApplyMatrix33ToIntVector((struct SnoboMatrix *)&s2->matrix, (struct SnowboardSpawnPos *)in19, (struct SnowboardVec *)ts6);
    pf4[0] = f8->posX + (tf29[0] >> 18);
    pf4[1] = f8->posY + (tf29[1] >> 18);
    pf4[2] = f8->posZ + (tf29[2] >> 18);
    ps15[0] = s2->posX + (ts6[0] >> 18);
    ps15[1] = s2->posY + (ts6[1] >> 18);
    ps15[2] = s2->posZ + (ts6[2] >> 18);
    position[0] = (((ps15[0] - pf4[0]) * fx4) >> 8) + pf4[0];
    position[1] = (((ps15[1] - pf4[1]) * fx4) >> 8) + pf4[1];
    position[2] = (((ps15[2] - pf4[2]) * fx4) >> 8) + pf4[2];
}

/*
 * Interpolates position and three SHORT angles between adjacent track frames in an 80-entry
 * ring using the low byte of path as a fraction over 256. Transforms [unused shifted left 18, 0,
 * 0] by each sample matrix and adds the shifted results to sample positions before interpolation.
 * Angle differences greater than 2048 adjust one endpoint by 4096; each output begins with the
 * first sample angle and adds the adjusted difference fraction. Negative path indexes are not
 * guarded.
 */
// FUNCTION: FF7 0x00657feb
void HighwayFieldManager::load(int path, int unused, int *position, int *result)
{
    // VC5 local-name buckets reproduce the original nested sample layout.
    int fr37;
    int tf29[4];
    int ts6[4];
    short as0;
    short af5;
    HighwayTrackFrame *s2;
    int v;
    HighwayTrackFrame *f8;
    int x;
    int ps15[3];
    int j;
    int pf4[3];
    int in19[3];
    int fx4;

    fr37 = path >> 8;
    fx4 = path & 0xFF;
    f8 = (HighwayTrackFrame *)this->frames + fr37 % 0x50;
    s2 = (HighwayTrackFrame *)this->frames + (fr37 + 1) % 0x50;
    in19[0] = unused << 18;
    in19[1] = 0;
    in19[2] = 0;
    PsxApplyMatrix33ToIntVector((struct SnoboMatrix *)&f8->matrix, (struct SnowboardSpawnPos *)in19, (struct SnowboardVec *)tf29);
    PsxApplyMatrix33ToIntVector((struct SnoboMatrix *)&s2->matrix, (struct SnowboardSpawnPos *)in19, (struct SnowboardVec *)ts6);
    pf4[0] = f8->posX + (tf29[0] >> 18);
    pf4[1] = f8->posY + (tf29[1] >> 18);
    pf4[2] = f8->posZ + (tf29[2] >> 18);
    ps15[0] = s2->posX + (ts6[0] >> 18);
    ps15[1] = s2->posY + (ts6[1] >> 18);
    ps15[2] = s2->posZ + (ts6[2] >> 18);
    position[0] = (((ps15[0] - pf4[0]) * fx4) >> 8) + pf4[0];
    position[1] = (((ps15[1] - pf4[1]) * fx4) >> 8) + pf4[1];
    position[2] = (((ps15[2] - pf4[2]) * fx4) >> 8) + pf4[2];
    ((short *)result)[0] = f8->angleX;
    ((short *)result)[1] = f8->angleY;
    ((short *)result)[2] = f8->angleZ;

    as0 = s2->angleX;
    af5 = f8->angleX;
    if (as0 - af5 > 0x800) {
        af5 += 0x1000;
    }
    if (af5 - as0 > 0x800) {
        as0 += 0x1000;
    }
    ((short *)result)[0] += ((as0 - af5) * fx4) >> 8;

    as0 = s2->angleY;
    af5 = f8->angleY;
    if (as0 - af5 > 0x800) {
        af5 += 0x1000;
    }
    if (af5 - as0 > 0x800) {
        as0 += 0x1000;
    }
    ((short *)result)[1] += ((as0 - af5) * fx4) >> 8;

    as0 = s2->angleZ;
    af5 = f8->angleZ;
    if (as0 - af5 > 0x800) {
        af5 += 0x1000;
    }
    if (af5 - as0 > 0x800) {
        as0 += 0x1000;
    }
    ((short *)result)[2] += ((as0 - af5) * fx4) >> 8;
}

/*
 * Interpolates positions between cyclic neighboring frames using transformed lateral offset and
 * low-byte fraction. Outputs two angle shorts with wrap correction when difference exceeds 2048 and
 * zero third angle. Despite parameter name unused, it forms input offset shifted 16.
 */
// FUNCTION: FF7 0x006582b9
void HighwayFieldManager::loadPlanar(int path, int unused, int *position, int *result)
{
    int fr37;
    int tf29[4];
    int ts6[4];
    short as0;
    short af5;
    HighwayTrackFrame *s2;
    int v;
    HighwayTrackFrame *f8;
    int x;
    int ps15[3];
    int pf4[3];
    int in19[4] = {unused << 16, 0, 0};
    int fx4;

    fr37 = path >> 8;
    fx4 = path & 0xFF;
    f8 = (HighwayTrackFrame *)this->frames + fr37 % 0x50;
    s2 = (HighwayTrackFrame *)this->frames + (fr37 + 1) % 0x50;
    PsxApplyMatrix33ToIntVector((struct SnoboMatrix *)&f8->matrix, (struct SnowboardSpawnPos *)in19, (struct SnowboardVec *)tf29);
    PsxApplyMatrix33ToIntVector((struct SnoboMatrix *)&s2->matrix, (struct SnowboardSpawnPos *)in19, (struct SnowboardVec *)ts6);
    pf4[0] = f8->posX + (tf29[0] >> 16);
    pf4[1] = f8->posY + (tf29[1] >> 16);
    pf4[2] = f8->posZ + (tf29[2] >> 16);
    ps15[0] = s2->posX + (ts6[0] >> 16);
    ps15[1] = s2->posY + (ts6[1] >> 16);
    ps15[2] = s2->posZ + (ts6[2] >> 16);
    position[0] = (((ps15[0] - pf4[0]) * fx4) >> 8) + pf4[0];
    position[1] = (((ps15[1] - pf4[1]) * fx4) >> 8) + pf4[1];
    position[2] = (((ps15[2] - pf4[2]) * fx4) >> 8) + pf4[2];
    ((short *)result)[0] = f8->angleX;
    ((short *)result)[1] = f8->angleY;
    ((short *)result)[2] = 0;

    as0 = s2->angleX;
    af5 = f8->angleX;
    if (as0 - af5 > 0x800) {
        af5 += 0x1000;
    }
    if (af5 - as0 > 0x800) {
        as0 += 0x1000;
    }
    ((short *)result)[0] += ((as0 - af5) * fx4) >> 8;

    as0 = s2->angleY;
    af5 = f8->angleY;
    if (as0 - af5 > 0x800) {
        af5 += 0x1000;
    }
    if (af5 - as0 > 0x800) {
        as0 += 0x1000;
    }
    ((short *)result)[1] += ((as0 - af5) * fx4) >> 8;
}

/*
 * Samples ten palette channels. Zero code with flag two stores planar anchor; positive code loads
 * planar for first six or full for rest, adjusts angles/Y from flags and cached anchor, creates
 * actor and appends pointer/count to current frame. Count increments even null creation; list not
 * reset here.
 */
// FUNCTION: FF7 0x00658512
void HighwayFieldManager::advanceActors()
{
    short sampleBits;
    int chan;
    int createdCount;
    short ang[4];
    short yawAdd;
    int type;
    short localLateral;
    int w;
    short tempHeightAdjust;
    int pos[3];

    createdCount = 0;
    for (chan = 0; chan < 10; chan++) {
        this->palette->sampleFrame((unsigned char)chan, &type, &localLateral, &tempHeightAdjust, &sampleBits, &yawAdd);
        if ((type & 0xFF) == 0 && (sampleBits & 2) != 0) {
            loadPlanar(this->frameIndex << 8, localLateral, pos, (int *)ang);
            this->anchors[chan].x = pos[0];
            this->anchors[chan].y = pos[1];
            this->anchors[chan].z = pos[2];
        }
        if ((type & 0xFF) > 0) {
            if (chan < 6) {
                loadPlanar(this->frameIndex << 8, localLateral, pos, (int *)ang);
                ang[0] = 0;
                ang[1] += yawAdd;
                ang[2] = 0;
            } else {
                load(this->frameIndex << 8, localLateral, pos, (int *)ang);
                ang[1] += yawAdd;
            }
            if (sampleBits & 1) {
                pos[1] = this->anchors[chan].y - tempHeightAdjust;
            } else {
                pos[1] -= tempHeightAdjust;
            }
            ((HighwayTrackFrame *)this->frames)[this->frameIndex].actors[createdCount] =
                g_pHighwayIndexBlock->create((unsigned char)type, (HighwayModelNode *)g_pHighwayIndexBlock, pos, ang);
            ++((HighwayTrackFrame *)this->frames)[this->frameIndex].actorCount;
            createdCount++;
        }
    }
}
