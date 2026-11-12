// src/ff7/field/src/ad_human.cpp
// Original: C:\FF7\field\src\ad_human.CPP

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
// Also: Field entity logic: clearing the human buffers, placing the player on the walkmesh,
//   entity heading, animation and movement, and the gateway, script line and door trigger tests.
#include "ff7/api/field.h"
#include "lib/api/mem.h"
#include "ff7/types/field.h"
#include "ff7/enums/field.h"
#include "ff7/enums/main.h"
#include "ff7/field.h"
#include "ff7/window.h"
#include "ff7/savemap.h"
#include "ff7/enums/menu.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include "lib/pfile.h"
#include "lib/types/graphics.h"
#include "ff7/gte.h"
#include "lib/types/mem.h"
#include "lib/codegen.h"
#include "lib/types/file.h"
#include "lib/types/polygon.h"
#include "lib/enums/graphics.h"
#include <string.h>
#include "lib/api/graphics.h"
#include "lib/crt.h"

extern "C" {

// The 130-entry arcsine ramp table (values 0..0x20) that this TU's direction code
// indexes by a slope (+-q, +-p).
// GLOBAL: FF7 0x009067c0
short g_FieldArcsinTable[130] = {
    0x0, 0x0, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x4, 0x4, 0x4, 0x4, 0x5,
    0x5, 0x5, 0x6, 0x6, 0x6, 0x7, 0x7, 0x7, 0x8, 0x8, 0x8, 0x9, 0x9, 0x9, 0xa, 0xa,
    0xa, 0xb, 0xb, 0xb, 0xc, 0xc, 0xc, 0xd, 0xd, 0xd, 0xe, 0xe, 0xe, 0xf, 0xf, 0xf,
    0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x13, 0x13, 0x14, 0x14, 0x14, 0x15, 0x15,
    0x15, 0x16, 0x16, 0x16, 0x17, 0x17, 0x18, 0x18, 0x18, 0x19, 0x19, 0x1a, 0x1a, 0x1a, 0x1b, 0x1b,
    0x1c, 0x1c, 0x1c, 0x1d, 0x1d, 0x1e, 0x1e, 0x1e, 0x1f, 0x1f, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20};
// GLOBAL: FF7 0x00cff5b8
WindowData g_FieldWindows[4] = {0};
unsigned char g_bssPad_00cff678[192] = {0};
// GLOBAL: FF7 0x00cff738
struct FieldModelSlot *g_pFieldModelSlots = 0;
// The upper 16 bits hold modelListCountCopy; the lower 16 bits are unused alignment padding.
// GLOBAL: FF7 0x00cff73c
struct FieldHumanAnimState g_FieldHumanAnimState = {0};
// GLOBAL: FF7 0x00cff740
unsigned short g_FieldHumanAnimFlag = 0;
// GLOBAL: FF7 0x00cff744
FieldWalkmeshVertex *g_pFieldWalkmeshVertices = 0;
// GLOBAL: FF7 0x00cff748
short *g_pFieldWalkmeshAccessTable = 0;
// GLOBAL: FF7 0x00cff74c
struct FieldHumanActorBuffer0 *g_pHumanActorBuffer0 = 0;
// GLOBAL: FF7 0x00cff750
struct FieldHumanActorBuffer0 *g_pHumanActorBuffer1 = 0;
// GLOBAL: FF7 0x00cff754
struct FieldHumanActorBuffer0 *g_pHumanActorBuffer2 = 0;
// GLOBAL: FF7 0x00cff758
struct FieldHumanActorBuffer0 *g_pHumanActorBuffer3 = 0;
unsigned char g_bssPad_00cff75c[4] = {0};
unsigned char g_bssPad_00cff760[8] = {0};
// GLOBAL: FF7 0x00cff768
struct FieldModelSortRecord g_FieldModelSortRecords[18] = {0};
// GLOBAL: FF7 0x00cff888
unsigned short g_FieldModelSortCount = 0;
unsigned char g_bssPad_00cff88c[4] = {0};
// GLOBAL: FF7 0x00cff890
double g_FieldFrameTimeDelta = 0.0;
// GLOBAL: FF7 0x00cff898
unsigned char g_FieldModelRenderState[64] = {0};
// GLOBAL: FF7 0x00cff8d8
Timestamp64 g_FieldTimestampCounter = {0};

/*
 * Allocates the four 0xA0-byte human-actor buffers without testing the
 * results; a failed allocation leaves a null buffer pointer (original
 * defect).
 */
// FUNCTION: FF7 0x006339c0
void __cdecl Field_AllocHumanBuffers(void)
{
    // BUG: none of the four allocations is tested; a failed one leaves a null human-actor
    // buffer pointer behind.
    // STRING: FF7 0x009068c4
    g_pHumanActorBuffer0 = (struct FieldHumanActorBuffer0 *)GameHeapAllocRaw(160, "C:\\FF7\\field\\src\\ad_human.CPP", 47);
    // STRING: FF7 0x009068e4
    g_pHumanActorBuffer1 = (struct FieldHumanActorBuffer0 *)GameHeapAllocRaw(160, "C:\\FF7\\field\\src\\ad_human.CPP", 48);
    // STRING: FF7 0x00906904
    g_pHumanActorBuffer2 = (struct FieldHumanActorBuffer0 *)GameHeapAllocRaw(160, "C:\\FF7\\field\\src\\ad_human.CPP", 49);
    // STRING: FF7 0x00906924
    g_pHumanActorBuffer3 = (struct FieldHumanActorBuffer0 *)GameHeapAllocRaw(160, "C:\\FF7\\field\\src\\ad_human.CPP", 50);
}

/*
 * Frees the four human-actor buffers allocated by Field_AllocHumanBuffers
 * and clears their pointers.
 */
// FUNCTION: FF7 0x00633a29
void __cdecl Field_FreeHumanBuffers(void)
{
    if (g_pHumanActorBuffer0 != 0) {
        // STRING: FF7 0x00906944
        GameHeapFree(g_pHumanActorBuffer0, "C:\\FF7\\field\\src\\ad_human.CPP", 57);
        g_pHumanActorBuffer0 = 0;
    }
    if (g_pHumanActorBuffer1 != 0) {
        // STRING: FF7 0x00906964
        GameHeapFree(g_pHumanActorBuffer1, "C:\\FF7\\field\\src\\ad_human.CPP", 62);
        g_pHumanActorBuffer1 = 0;
    }
    if (g_pHumanActorBuffer2 != 0) {
        // STRING: FF7 0x00906984
        GameHeapFree(g_pHumanActorBuffer2, "C:\\FF7\\field\\src\\ad_human.CPP", 67);
        g_pHumanActorBuffer2 = 0;
    }
    if (g_pHumanActorBuffer3 != 0) {
        // STRING: FF7 0x009069a4
        GameHeapFree(g_pHumanActorBuffer3, "C:\\FF7\\field\\src\\ad_human.CPP", 72);
        g_pHumanActorBuffer3 = 0;
    }
}
} /* extern "C" */

extern "C" {

struct FieldPointTo;
// clang-format on

short __cdecl Field_CosineByteAngle(int angle);
// Distinct C symbol from Mini_CondorSeedRandomFromContext (0x005F5153) and Mini_ChocoboSeedRandomFromContext,
// which compile identical seeding logic in separate modules.

/*
 * Zeroes the first three ints of each of ten 16-byte records in each of the four buffers pointed
 * to by g_pHumanActorBuffer3, g_pHumanActorBuffer0, g_pHumanActorBuffer1 and g_pHumanActorBuffer2, in that order. Those are the
 * four 0xA0-byte allocations made in ad_human.cpp, and 0xA0 is exactly ten records of 16 bytes;
 * the fourth word of every record is left untouched. The counter is an unsigned short and the
 * stride comes from a shift by 4.
 */
// FUNCTION: FF7 0x00633ad0
void __cdecl Field_ClearHumanBufferRecords(void)
{
    unsigned short i;

    for (i = 0; i < 10; i++) {
        g_pHumanActorBuffer3[i].clearedWord0 = 0;
        g_pHumanActorBuffer3[i].clearedWord1 = 0;
        g_pHumanActorBuffer3[i].clearedWord2 = 0;
        g_pHumanActorBuffer0[i].clearedWord0 = 0;
        g_pHumanActorBuffer0[i].clearedWord1 = 0;
        g_pHumanActorBuffer0[i].clearedWord2 = 0;
        g_pHumanActorBuffer1[i].clearedWord0 = 0;
        g_pHumanActorBuffer1[i].clearedWord1 = 0;
        g_pHumanActorBuffer1[i].clearedWord2 = 0;
        g_pHumanActorBuffer2[i].clearedWord0 = 0;
        g_pHumanActorBuffer2[i].clearedWord1 = 0;
        g_pHumanActorBuffer2[i].clearedWord2 = 0;
    }
}

/*
 * Places the saved player on the field walkmesh when player placement is enabled. Restores the
 * player and triangle indices, using the triangle centroid when saved X is 0x7FFF or saved X/Y and
 * a triangle height calculation otherwise, with coordinates scaled by 4096. Restores two
 * differently scaled heading/speed fields, sets the animation step to 16, and clears every entity
 * heading offset.
 */
// FUNCTION: FF7 0x00633c2f
void __cdecl Field_PlacePlayerOnWalkmesh(void)
{
    int v1[4];
    int v2[4];
    int v3[4];
    int cnt;
    short i;

    cnt = *(unsigned short *)g_pFieldWalkmesh;
    g_pFieldWalkmeshVertices = (FieldWalkmeshVertex *)((char *)g_pFieldWalkmesh + 4);
    g_pFieldWalkmeshAccessTable = (short *)(cnt * 0x18 + (char *)g_pFieldWalkmesh + 4);
    if ((char)g_FieldActorState.state.characterLock == 0) {
        g_FieldPlayerEntityIndex = g_FieldActorState.state.playerModel;
        g_FieldEntityState[g_FieldPlayerEntityIndex].posI = g_FieldActorState.state.pcWalkMeshId;
        if (g_FieldActorState.state.pcTileX == 0x7FFF) {
            g_FieldEntityState[g_FieldPlayerEntityIndex].posX =
                (g_pFieldWalkmeshVertices[g_FieldEntityState[g_FieldPlayerEntityIndex].posI * 3].x +
                 g_pFieldWalkmeshVertices[g_FieldEntityState[g_FieldPlayerEntityIndex].posI * 3 + 1].x +
                 g_pFieldWalkmeshVertices[g_FieldEntityState[g_FieldPlayerEntityIndex].posI * 3 + 2].x) /
                    3
                << 12;
            g_FieldEntityState[g_FieldPlayerEntityIndex].posY =
                (g_pFieldWalkmeshVertices[g_FieldEntityState[g_FieldPlayerEntityIndex].posI * 3].y +
                 g_pFieldWalkmeshVertices[g_FieldEntityState[g_FieldPlayerEntityIndex].posI * 3 + 1].y +
                 g_pFieldWalkmeshVertices[g_FieldEntityState[g_FieldPlayerEntityIndex].posI * 3 + 2].y) /
                    3
                << 12;
            g_FieldEntityState[g_FieldPlayerEntityIndex].posZ =
                (g_pFieldWalkmeshVertices[g_FieldEntityState[g_FieldPlayerEntityIndex].posI * 3].z +
                 g_pFieldWalkmeshVertices[g_FieldEntityState[g_FieldPlayerEntityIndex].posI * 3 + 1].z +
                 g_pFieldWalkmeshVertices[g_FieldEntityState[g_FieldPlayerEntityIndex].posI * 3 + 2].z) /
                    3
                << 12;
        } else {
            g_FieldEntityState[g_FieldPlayerEntityIndex].posX = g_FieldActorState.state.pcTileX << 12;
            g_FieldEntityState[g_FieldPlayerEntityIndex].posY = g_FieldActorState.state.pcTileY << 12;
            Field_VecSub16To32(
                (FieldVec3i *)v2,
                (FieldVec16 *)&g_pFieldWalkmeshVertices[g_FieldEntityState[g_FieldPlayerEntityIndex].posI * 3 + 1],
                (FieldVec16 *)&g_pFieldWalkmeshVertices[g_FieldEntityState[g_FieldPlayerEntityIndex].posI * 3]);
            Field_VecSub16To32(
                (FieldVec3i *)v3,
                (FieldVec16 *)&g_pFieldWalkmeshVertices[g_FieldEntityState[g_FieldPlayerEntityIndex].posI * 3 + 2],
                (FieldVec16 *)&g_pFieldWalkmeshVertices[g_FieldEntityState[g_FieldPlayerEntityIndex].posI * 3 + 1]);
            v1[0] = g_FieldActorState.state.pcTileX;
            v1[1] = g_FieldActorState.state.pcTileY;
            g_FieldEntityState[g_FieldPlayerEntityIndex].posZ =
                Field_PointHeightInTriangle(
                    (FieldVec3i *)v2, (FieldVec3i *)v3, (FieldPointTo *)v1,
                    (FieldVec16 *)&g_pFieldWalkmeshVertices[g_FieldEntityState[g_FieldPlayerEntityIndex].posI * 3])
                << 12;
        }
        g_FieldEntityState[g_FieldPlayerEntityIndex].solidRange = g_FieldActorState.state.fieldScale * 0x22 >> 9;
        g_FieldEntityState[g_FieldPlayerEntityIndex].moveSpeed = g_FieldActorState.state.fieldScale << 10 >> 9;
        g_FieldEntityState[g_FieldPlayerEntityIndex].animSpeed = 16;
    }
    for (i = 0; i < g_FieldHumanAnimState.modelListCountCopy; i++) {
        g_FieldEntityState[i].moveDirAdd = 0;
    }
}

/*
 * Loads the first int of the record pointed to by g_pFieldWalkmesh into a local that is never read
 * again. With the assumed void signature that load is the whole 18-byte body, so this is most
 * likely a getter whose return value the prototype drops.
 */
// FUNCTION: FF7 0x00634032
void __cdecl Field_UnusedReadWalkmeshHeader(void)
{
    int val;

    val = *(int *)g_pFieldWalkmesh;
}

/*
 * Empty: prologue and epilogue only.
 */
// FUNCTION: FF7 0x00634044
void __cdecl Field_UnusedNoOp_5(void)
{
}

/*
 * When g_FieldActorState.state.characterLock is zero, writes 0xE0, 0x20 or zero to the
 * moveDirAdd of g_FieldEntityState[0] according to global flag bits eight
 * then four. Arg1 is unused.
 */
// FUNCTION: FF7 0x00634049
void __cdecl Field_SetPlayerHeadingOffsetFromInput(int unused, short entityIndex)
{
    if ((char)g_FieldActorState.state.characterLock == 0) {
        if ((g_FieldActorState.state.activeKeys2_32 & INPUT_R1) != 0) {
            g_FieldEntityState[entityIndex].moveDirAdd = 0xE0;
        } else if ((g_FieldActorState.state.activeKeys2_32 & INPUT_L1) != 0) {
            g_FieldEntityState[entityIndex].moveDirAdd = 0x20;
        } else {
            g_FieldEntityState[entityIndex].moveDirAdd = 0;
        }
    }
}

/*
 * Animation byte mismatch selects model animation and resets frame accumulator even when animation
 * progression paused. Progress gate conditionally adds signed step excluding selected actor, then
 * player mode loops to zero beyond last-frame threshold while other modes clamp. Does not subtract
 * period or clamp negative values.
 */
// FUNCTION: FF7 0x006340a9
void __cdecl Field_AdvanceEntityAnimation(short entityIndex)
{
    GfxEngineContext *engineContext;

    if ((char)g_FieldEntityState[entityIndex].activeAnimId != g_pFieldModelSlots[entityIndex].animId) {
        Field_SetModelAnimation((FieldModel *)&g_pFieldModelSlots[entityIndex], (char)g_FieldEntityState[entityIndex].activeAnimId);
        g_FieldEntityState[entityIndex].animCurrentFrame = 0;
    }
    if (g_FieldActorState.state.entityMode == 0) {
        engineContext = GetEngineContext();
        if (g_FieldAnimPausedEntity == -1) {
            g_FieldEntityState[entityIndex].animCurrentFrame += g_FieldEntityState[entityIndex].animSpeed;
        } else if (g_FieldRenderActive != 0) {
            if (entityIndex != g_FieldAnimPausedEntity) {
                g_FieldEntityState[entityIndex].animCurrentFrame += g_FieldEntityState[entityIndex].animSpeed;
            }
        } else {
            g_FieldEntityState[entityIndex].animCurrentFrame += g_FieldEntityState[entityIndex].animSpeed;
        }
        if (entityIndex == g_FieldPlayerEntityIndex && (char)g_FieldActorState.state.characterLock == 0) {
            g_FieldEntityState[entityIndex].animLastFrame = g_pFieldModelSlots[entityIndex].model->lastFrame - 1;
            if (g_FieldEntityState[entityIndex].animCurrentFrame > g_FieldEntityState[entityIndex].animLastFrame << 4) {
                g_FieldEntityState[entityIndex].animCurrentFrame = 0;
            }
        } else if (g_FieldEntityState[entityIndex].animCurrentFrame > g_FieldEntityState[entityIndex].animLastFrame << 4) {
            g_FieldEntityState[entityIndex].animCurrentFrame = g_FieldEntityState[entityIndex].animLastFrame << 4;
        }
    }
}

/*
 * Updates entity visibility, heading interpolation and position-offset interpolation before
 * separate movement-mode passes. Ordinary player movement decodes directional input, applies field
 * and entity heading offsets, steps the walkmesh and checks encounters after a successful step;
 * disabled player control still advances animation. Scripted walking can be globally paused before
 * animation, jumping advances a linear horizontal path with a parabolic vertical term, and the
 * two ladder modes interpolate a distance-derived path with reversible player progress and animation or
 * automatic forward progress. Completion updates movement state and destination triangle, while
 * player offset or movement changes reset line-talk flags. Vector helpers receive addresses of
 * scalar locals, so this reconstruction relies on their surrounding frame storage.
 */
// FUNCTION: FF7 0x006342c6
void __cdecl Field_UpdateEntities(int input)
{
    int result;
    int spareValue[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int padSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short entityIdx;
    FieldVec3i vector;
    int unusedEntry; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    FieldVec3i deltaX;
    int stepLen;
    FieldPointTo posX;

    for (entityIdx = 0; entityIdx < g_FieldHumanAnimState.modelListCountCopy; entityIdx++) {
        if ((char)g_FieldEntityState[entityIdx].visible == 1) {
            g_pFieldModelSlots[entityIdx].entityVisible = 1;
        } else {
            g_pFieldModelSlots[entityIdx].entityVisible = 0;
        }
    }
    for (entityIdx = 0; entityIdx < g_FieldHumanAnimState.modelListCountCopy; entityIdx++) {
        switch (g_FieldEntityState[entityIdx].turnType) {
        case FIELD_ENTITY_INTERP_NONE:
            break;
        case FIELD_ENTITY_INTERP_LINEAR:
            g_FieldEntityState[entityIdx].dir = Field_InterpolateLinear(g_FieldEntityState[entityIdx].turnStart, g_FieldEntityState[entityIdx].turnEnd, g_FieldEntityState[entityIdx].turnSteps, g_FieldEntityState[entityIdx].turnStep) & 0xFF;
            if (g_FieldEntityState[entityIdx].turnStep == g_FieldEntityState[entityIdx].turnSteps) {
                g_FieldEntityState[entityIdx].turnType = FIELD_ENTITY_INTERP_DONE;
            } else {
                g_FieldEntityState[entityIdx].turnStep++;
            }
            break;
        case FIELD_ENTITY_INTERP_SMOOTH:
            g_FieldEntityState[entityIdx].dir = Field_InterpolateCosine(g_FieldEntityState[entityIdx].turnStart, g_FieldEntityState[entityIdx].turnEnd, g_FieldEntityState[entityIdx].turnSteps, g_FieldEntityState[entityIdx].turnStep) & 0xFF;
            if (g_FieldEntityState[entityIdx].turnStep == g_FieldEntityState[entityIdx].turnSteps) {
                g_FieldEntityState[entityIdx].turnType = FIELD_ENTITY_INTERP_DONE;
            } else {
                g_FieldEntityState[entityIdx].turnStep++;
            }
        }
    }
    for (entityIdx = 0; entityIdx < g_FieldHumanAnimState.modelListCountCopy; entityIdx++) {
        switch (g_FieldEntityState[entityIdx].ofsType) {
        case FIELD_ENTITY_INTERP_NONE:
            break;
        case FIELD_ENTITY_INTERP_LINEAR:
            g_FieldEntityState[entityIdx].offsetX = Field_InterpolateLinear(g_FieldEntityState[entityIdx].offsetStartX, g_FieldEntityState[entityIdx].offsetEndX, g_FieldEntityState[entityIdx].offsetSteps, g_FieldEntityState[entityIdx].offsetStep);
            g_FieldEntityState[entityIdx].offsetY = Field_InterpolateLinear(g_FieldEntityState[entityIdx].offsetStartY, g_FieldEntityState[entityIdx].offsetEndY, g_FieldEntityState[entityIdx].offsetSteps, g_FieldEntityState[entityIdx].offsetStep);
            g_FieldEntityState[entityIdx].offsetZ = Field_InterpolateLinear(g_FieldEntityState[entityIdx].offsetStartZ, g_FieldEntityState[entityIdx].offsetEndZ, g_FieldEntityState[entityIdx].offsetSteps, g_FieldEntityState[entityIdx].offsetStep);
            if (g_FieldEntityState[entityIdx].offsetStep == g_FieldEntityState[entityIdx].offsetSteps) {
                g_FieldEntityState[entityIdx].ofsType = FIELD_ENTITY_INTERP_DONE;
            } else {
                g_FieldEntityState[entityIdx].offsetStep++;
            }
            if (entityIdx == g_FieldPlayerEntityIndex) {
                Field_ResetScriptLineTalkFlags((int)g_FieldLines);
            }
            break;
        case FIELD_ENTITY_INTERP_SMOOTH:
            g_FieldEntityState[entityIdx].offsetX = Field_InterpolateCosine(g_FieldEntityState[entityIdx].offsetStartX, g_FieldEntityState[entityIdx].offsetEndX, g_FieldEntityState[entityIdx].offsetSteps, g_FieldEntityState[entityIdx].offsetStep);
            g_FieldEntityState[entityIdx].offsetY = Field_InterpolateCosine(g_FieldEntityState[entityIdx].offsetStartY, g_FieldEntityState[entityIdx].offsetEndY, g_FieldEntityState[entityIdx].offsetSteps, g_FieldEntityState[entityIdx].offsetStep);
            g_FieldEntityState[entityIdx].offsetZ = Field_InterpolateCosine(g_FieldEntityState[entityIdx].offsetStartZ, g_FieldEntityState[entityIdx].offsetEndZ, g_FieldEntityState[entityIdx].offsetSteps, g_FieldEntityState[entityIdx].offsetStep);
            if (g_FieldEntityState[entityIdx].offsetStep == g_FieldEntityState[entityIdx].offsetSteps) {
                g_FieldEntityState[entityIdx].ofsType = FIELD_ENTITY_INTERP_DONE;
            } else {
                g_FieldEntityState[entityIdx].offsetStep++;
            }
            if (entityIdx == g_FieldPlayerEntityIndex) {
                Field_ResetScriptLineTalkFlags((int)g_FieldLines);
            }
        }
    }
    for (entityIdx = 0; entityIdx < g_FieldHumanAnimState.modelListCountCopy; entityIdx++) {
        if ((char)g_FieldEntityState[entityIdx].scriptedMoveMode == FIELD_SCRIPTED_MOVE_NONE) {
            if (entityIdx == g_FieldPlayerEntityIndex) {
                if ((char)g_FieldActorState.state.characterLock == 1) {
                    Field_AdvanceEntityAnimation(entityIdx);
                    continue;
                }
                g_FieldEntityState[g_FieldPlayerEntityIndex].activeAnimId = g_FieldActorState.state.idleAnimIdLo;
                Field_SetPlayerHeadingOffsetFromInput(input, (short)(g_FieldPlayerEntityIndex));
                if ((input & INPUT_CROSS) != 0) {
                    if ((char)g_FieldActorState.state.speedBoost == 0) {
                        g_FieldEntityState[g_FieldPlayerEntityIndex].moveSpeed = g_FieldActorState.state.fieldScale << 12 >> 9;
                    } else {
                        g_FieldEntityState[g_FieldPlayerEntityIndex].moveSpeed = g_FieldActorState.state.fieldScale * 0x1800 >> 9;
                    }
                } else if ((char)g_FieldActorState.state.speedBoost == 0) {
                    g_FieldEntityState[g_FieldPlayerEntityIndex].moveSpeed = g_FieldActorState.state.fieldScale << 10 >> 9;
                } else {
                    g_FieldEntityState[g_FieldPlayerEntityIndex].moveSpeed = g_FieldActorState.state.fieldScale * 0xC00 >> 10;
                }
                if ((input & (INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT)) != 0) {
                    if ((input & INPUT_UP) != 0) {
                        g_FieldEntityState[entityIdx].moveDir = 0;
                        if ((input & INPUT_LEFT) != 0) {
                            g_FieldEntityState[entityIdx].moveDir = 0x20;
                        }
                        if ((input & INPUT_RIGHT) != 0) {
                            g_FieldEntityState[entityIdx].moveDir = 0xE0;
                        }
                    } else if ((input & INPUT_DOWN) != 0) {
                        g_FieldEntityState[entityIdx].moveDir = 0x80;
                        if ((input & INPUT_LEFT) != 0) {
                            g_FieldEntityState[entityIdx].moveDir = 0x60;
                        }
                        if ((input & INPUT_RIGHT) != 0) {
                            g_FieldEntityState[entityIdx].moveDir = 0xA0;
                        }
                    } else {
                        if ((input & INPUT_RIGHT) != 0) {
                            g_FieldEntityState[entityIdx].moveDir = 0xC0;
                        }
                        if ((input & INPUT_LEFT) != 0) {
                            g_FieldEntityState[entityIdx].moveDir = 0x40;
                        }
                    }
                    g_FieldEntityState[entityIdx].moveDir += ((FieldCameraScrollRecord *)g_pFieldScrollBounds)->headingDelta + g_FieldEntityState[entityIdx].moveDirAdd;
                    result = Field_StepEntityOnWalkmesh(entityIdx);
                    if (g_FieldEntityState[entityIdx].dirLock == 0) {
                        g_FieldEntityState[entityIdx].dir = g_FieldEntityState[entityIdx].moveDir;
                    }
                    if (g_FieldActorState.state.requestCode != FIELD_REQUEST_MAP_JUMP && result == 1) {
                        Field_CheckRandomEncounter();
                    }
                }
            }
            Field_AdvanceEntityAnimation(entityIdx);
        }
    }
    for (entityIdx = 0; entityIdx < g_FieldHumanAnimState.modelListCountCopy; entityIdx++) {
        if ((char)g_FieldEntityState[entityIdx].scriptedMoveMode == FIELD_SCRIPTED_MOVE_WALK) {
            if (g_FieldActorState.state.entityMode == 1) {
                continue;
            }
            g_FieldEntityState[entityIdx].moveDirAdd = 0;
            result = Field_AimEntityAtWalkTarget((FieldEntityWalkAim *)&g_FieldEntityState[entityIdx], g_FieldEntityState[entityIdx].actionArg);
            if (result == 0) {
                g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_DONE;
            } else {
                g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_RUNNING;
                Field_StepEntityOnWalkmesh(entityIdx);
                if (g_FieldEntityState[entityIdx].dirLock == 0) {
                    g_FieldEntityState[entityIdx].dir = g_FieldEntityState[entityIdx].moveDir;
                }
            }
            Field_AdvanceEntityAnimation(entityIdx);
            if (entityIdx == g_FieldPlayerEntityIndex) {
                Field_ResetScriptLineTalkFlags((int)g_FieldLines);
            }
        }
    }
    for (entityIdx = 0; entityIdx < g_FieldHumanAnimState.modelListCountCopy; entityIdx++) {
        if ((char)g_FieldEntityState[entityIdx].scriptedMoveMode == FIELD_SCRIPTED_MOVE_JUMP) {
            if (g_FieldEntityState[entityIdx].actionState == FIELD_MOVE_STATE_START) {
                g_FieldEntityState[entityIdx].moveDirAdd = 0;
                g_FieldEntityState[entityIdx].moveStartX = g_FieldEntityState[entityIdx].posX;
                g_FieldEntityState[entityIdx].moveStartY = g_FieldEntityState[entityIdx].posY;
                g_FieldEntityState[entityIdx].moveStartZ = g_FieldEntityState[entityIdx].posZ;
                Field_VecSub16To32(&deltaX, (FieldVec16 *)&g_pFieldWalkmeshVertices[(unsigned short)g_FieldEntityState[entityIdx].moveEndI * 3 + 1], (FieldVec16 *)&g_pFieldWalkmeshVertices[(unsigned short)g_FieldEntityState[entityIdx].moveEndI * 3]);
                Field_VecSub16To32(&vector, (FieldVec16 *)&g_pFieldWalkmeshVertices[(unsigned short)g_FieldEntityState[entityIdx].moveEndI * 3 + 2], (FieldVec16 *)&g_pFieldWalkmeshVertices[(unsigned short)g_FieldEntityState[entityIdx].moveEndI * 3 + 1]);
                posX.x = (g_FieldEntityState[entityIdx].moveEndX) / 4096;
                posX.y = (g_FieldEntityState[entityIdx].moveEndY) / 4096;
                g_FieldEntityState[entityIdx].moveEndZ = Field_PointHeightInTriangle(&deltaX, &vector, &posX, (FieldVec16 *)&g_pFieldWalkmeshVertices[(unsigned short)g_FieldEntityState[entityIdx].moveEndI * 3]) << 12;
                g_FieldEntityState[entityIdx].moveB = (g_FieldEntityState[entityIdx].moveEndZ - g_FieldEntityState[entityIdx].moveStartZ) / g_FieldEntityState[entityIdx].moveSteps - (g_FieldEntityState[entityIdx].moveSteps * -16000) / 2;
                g_FieldEntityState[entityIdx].moveStep = 0;
                g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_RUNNING;
            } else if (g_FieldEntityState[entityIdx].moveSteps == g_FieldEntityState[entityIdx].moveStep) {
                g_FieldEntityState[entityIdx].posI = g_FieldEntityState[entityIdx].moveEndI;
                g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_DONE;
            } else {
                (g_FieldEntityState[entityIdx].moveStep)++;
                g_FieldEntityState[entityIdx].posX = Field_InterpolateLinear(g_FieldEntityState[entityIdx].moveStartX, g_FieldEntityState[entityIdx].moveEndX, g_FieldEntityState[entityIdx].moveSteps, g_FieldEntityState[entityIdx].moveStep);
                g_FieldEntityState[entityIdx].posY = Field_InterpolateLinear(g_FieldEntityState[entityIdx].moveStartY, g_FieldEntityState[entityIdx].moveEndY, g_FieldEntityState[entityIdx].moveSteps, g_FieldEntityState[entityIdx].moveStep);
                g_FieldEntityState[entityIdx].posZ = g_FieldEntityState[entityIdx].moveB * g_FieldEntityState[entityIdx].moveStep + (g_FieldEntityState[entityIdx].moveStep * -16000 * g_FieldEntityState[entityIdx].moveStep) / 2 + g_FieldEntityState[entityIdx].moveStartZ;
            }
            Field_AdvanceEntityAnimation(entityIdx);
            if (entityIdx == g_FieldPlayerEntityIndex) {
                Field_ResetScriptLineTalkFlags((int)g_FieldLines);
            }
        }
    }
    for (entityIdx = 0; entityIdx < g_FieldHumanAnimState.modelListCountCopy; entityIdx++) {
        if ((char)g_FieldEntityState[entityIdx].scriptedMoveMode == FIELD_SCRIPTED_MOVE_LADDER_UP_DOWN) {
            if (g_FieldEntityState[entityIdx].actionState == FIELD_MOVE_STATE_START) {
                g_FieldEntityState[entityIdx].moveDirAdd = 0;
                g_FieldEntityState[entityIdx].moveStartX = g_FieldEntityState[entityIdx].posX;
                g_FieldEntityState[entityIdx].moveStartY = g_FieldEntityState[entityIdx].posY;
                g_FieldEntityState[entityIdx].moveStartZ = g_FieldEntityState[entityIdx].posZ;
                deltaX.x = (g_FieldEntityState[entityIdx].moveEndX - g_FieldEntityState[entityIdx].moveStartX) / 4096;
                deltaX.y = (g_FieldEntityState[entityIdx].moveEndY - g_FieldEntityState[entityIdx].moveStartY) / 4096;
                deltaX.z = (g_FieldEntityState[entityIdx].moveEndZ - g_FieldEntityState[entityIdx].moveStartZ) / 4096;
                stepLen = SqrtToInt(deltaX.x * deltaX.x + deltaX.y * deltaX.y + deltaX.z * deltaX.z);
                g_FieldEntityState[entityIdx].moveSteps = stepLen / 4;
                g_FieldEntityState[entityIdx].moveStep = 0;
                g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_RUNNING;
                Field_SetModelAnimation((FieldModel *)&g_pFieldModelSlots[entityIdx], (char)g_FieldEntityState[entityIdx].activeAnimId);
                g_FieldEntityState[entityIdx].animLastFrame = g_pFieldModelSlots[entityIdx].model->lastFrame - 1;
                g_FieldEntityState[entityIdx].animCurrentFrame = 0;
                if (entityIdx == g_FieldPlayerEntityIndex) {
                    Field_ResetScriptLineTalkFlags((int)g_FieldLines);
                }
            } else {
                if (entityIdx == g_FieldPlayerEntityIndex && (char)g_FieldActorState.state.characterLock == 0) {
                    if (g_FieldEntityState[entityIdx].actionArg == 0) {
                        if ((input & INPUT_UP) != 0) {
                            if (g_FieldEntityState[entityIdx].moveStep == 0) {
                                g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_DONE;
                            } else {
                                (g_FieldEntityState[entityIdx].moveStep)--;
                                g_FieldEntityState[entityIdx].animCurrentFrame -= g_FieldEntityState[entityIdx].animSpeed;
                                if (g_FieldEntityState[entityIdx].animCurrentFrame < 0) {
                                    g_FieldEntityState[entityIdx].animCurrentFrame = g_FieldEntityState[entityIdx].animLastFrame << 4;
                                }
                            }
                        }
                        if ((input & INPUT_DOWN) != 0) {
                            if (g_FieldEntityState[entityIdx].moveStep == g_FieldEntityState[entityIdx].moveSteps) {
                                g_FieldEntityState[entityIdx].posI = g_FieldEntityState[entityIdx].moveEndI;
                                g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_DONE;
                            } else {
                                (g_FieldEntityState[entityIdx].moveStep)++;
                                g_FieldEntityState[entityIdx].animCurrentFrame += g_FieldEntityState[entityIdx].animSpeed;
                                if (g_FieldEntityState[entityIdx].animCurrentFrame > g_FieldEntityState[entityIdx].animLastFrame << 4) {
                                    g_FieldEntityState[entityIdx].animCurrentFrame = 0;
                                }
                            }
                        }
                    } else {
                        if ((input & INPUT_DOWN) != 0) {
                            if (g_FieldEntityState[entityIdx].moveStep == 0) {
                                g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_DONE;
                            } else {
                                (g_FieldEntityState[entityIdx].moveStep)--;
                                g_FieldEntityState[entityIdx].animCurrentFrame -= g_FieldEntityState[entityIdx].animSpeed;
                                if (g_FieldEntityState[entityIdx].animCurrentFrame < 0) {
                                    g_FieldEntityState[entityIdx].animCurrentFrame = g_FieldEntityState[entityIdx].animLastFrame << 4;
                                }
                            }
                        }
                        if ((input & INPUT_UP) != 0) {
                            if (g_FieldEntityState[entityIdx].moveStep == g_FieldEntityState[entityIdx].moveSteps) {
                                g_FieldEntityState[entityIdx].posI = g_FieldEntityState[entityIdx].moveEndI;
                                g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_DONE;
                            } else {
                                (g_FieldEntityState[entityIdx].moveStep)++;
                                g_FieldEntityState[entityIdx].animCurrentFrame += g_FieldEntityState[entityIdx].animSpeed;
                                if (g_FieldEntityState[entityIdx].animCurrentFrame > g_FieldEntityState[entityIdx].animLastFrame << 4) {
                                    g_FieldEntityState[entityIdx].animCurrentFrame = 0;
                                }
                            }
                        }
                    }
                } else if (g_FieldEntityState[entityIdx].moveStep == g_FieldEntityState[entityIdx].moveSteps) {
                    g_FieldEntityState[entityIdx].posI = g_FieldEntityState[entityIdx].moveEndI;
                    g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_DONE;
                } else {
                    (g_FieldEntityState[entityIdx].moveStep)++;
                    g_FieldEntityState[entityIdx].animCurrentFrame += g_FieldEntityState[entityIdx].animSpeed;
                    if (g_FieldEntityState[entityIdx].animCurrentFrame > g_FieldEntityState[entityIdx].animLastFrame << 4) {
                        g_FieldEntityState[entityIdx].animCurrentFrame = 0;
                    }
                }
                g_FieldEntityState[entityIdx].posX = Field_InterpolateLinear(g_FieldEntityState[entityIdx].moveStartX, g_FieldEntityState[entityIdx].moveEndX, g_FieldEntityState[entityIdx].moveSteps, g_FieldEntityState[entityIdx].moveStep);
                g_FieldEntityState[entityIdx].posY = Field_InterpolateLinear(g_FieldEntityState[entityIdx].moveStartY, g_FieldEntityState[entityIdx].moveEndY, g_FieldEntityState[entityIdx].moveSteps, g_FieldEntityState[entityIdx].moveStep);
                g_FieldEntityState[entityIdx].posZ = Field_InterpolateLinear(g_FieldEntityState[entityIdx].moveStartZ, g_FieldEntityState[entityIdx].moveEndZ, g_FieldEntityState[entityIdx].moveSteps, g_FieldEntityState[entityIdx].moveStep);
            }
        }
    }
    for (entityIdx = 0; entityIdx < g_FieldHumanAnimState.modelListCountCopy; entityIdx++) {
        if ((char)g_FieldEntityState[entityIdx].scriptedMoveMode == FIELD_SCRIPTED_MOVE_LADDER_LEFT_RIGHT) {
            if (g_FieldEntityState[entityIdx].actionState == FIELD_MOVE_STATE_START) {
                g_FieldEntityState[entityIdx].moveDirAdd = 0;
                g_FieldEntityState[entityIdx].moveStartX = g_FieldEntityState[entityIdx].posX;
                g_FieldEntityState[entityIdx].moveStartY = g_FieldEntityState[entityIdx].posY;
                g_FieldEntityState[entityIdx].moveStartZ = g_FieldEntityState[entityIdx].posZ;
                deltaX.x = (g_FieldEntityState[entityIdx].moveEndX - g_FieldEntityState[entityIdx].moveStartX) / 4096;
                deltaX.y = (g_FieldEntityState[entityIdx].moveEndY - g_FieldEntityState[entityIdx].moveStartY) / 4096;
                deltaX.z = (g_FieldEntityState[entityIdx].moveEndZ - g_FieldEntityState[entityIdx].moveStartZ) / 4096;
                stepLen = SqrtToInt(deltaX.x * deltaX.x + deltaX.y * deltaX.y + deltaX.z * deltaX.z);
                g_FieldEntityState[entityIdx].moveSteps = stepLen / 4;
                g_FieldEntityState[entityIdx].moveStep = 0;
                g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_RUNNING;
                Field_SetModelAnimation((FieldModel *)&g_pFieldModelSlots[entityIdx], (char)g_FieldEntityState[entityIdx].activeAnimId);
                g_FieldEntityState[entityIdx].animLastFrame = g_pFieldModelSlots[entityIdx].model->lastFrame - 1;
                g_FieldEntityState[entityIdx].animCurrentFrame = 0;
                if (entityIdx == g_FieldPlayerEntityIndex) {
                    Field_ResetScriptLineTalkFlags((int)g_FieldLines);
                }
            } else {
                if (entityIdx == g_FieldPlayerEntityIndex && (char)g_FieldActorState.state.characterLock == 0) {
                    if (g_FieldEntityState[entityIdx].actionArg == 0) {
                        if ((input & INPUT_LEFT) != 0) {
                            if (g_FieldEntityState[entityIdx].moveStep == 0) {
                                g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_DONE;
                            } else {
                                (g_FieldEntityState[entityIdx].moveStep)--;
                                g_FieldEntityState[entityIdx].animCurrentFrame -= g_FieldEntityState[entityIdx].animSpeed;
                                if (g_FieldEntityState[entityIdx].animCurrentFrame < 0) {
                                    g_FieldEntityState[entityIdx].animCurrentFrame = g_FieldEntityState[entityIdx].animLastFrame << 4;
                                }
                            }
                        }
                        if ((input & INPUT_RIGHT) != 0) {
                            if (g_FieldEntityState[entityIdx].moveStep == g_FieldEntityState[entityIdx].moveSteps) {
                                g_FieldEntityState[entityIdx].posI = g_FieldEntityState[entityIdx].moveEndI;
                                g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_DONE;
                            } else {
                                (g_FieldEntityState[entityIdx].moveStep)++;
                                g_FieldEntityState[entityIdx].animCurrentFrame += g_FieldEntityState[entityIdx].animSpeed;
                                if (g_FieldEntityState[entityIdx].animCurrentFrame > g_FieldEntityState[entityIdx].animLastFrame << 4) {
                                    g_FieldEntityState[entityIdx].animCurrentFrame = 0;
                                }
                            }
                        }
                    } else {
                        if ((input & INPUT_RIGHT) != 0) {
                            if (g_FieldEntityState[entityIdx].moveStep == 0) {
                                g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_DONE;
                            } else {
                                (g_FieldEntityState[entityIdx].moveStep)--;
                                g_FieldEntityState[entityIdx].animCurrentFrame -= g_FieldEntityState[entityIdx].animSpeed;
                                if (g_FieldEntityState[entityIdx].animCurrentFrame < 0) {
                                    g_FieldEntityState[entityIdx].animCurrentFrame = g_FieldEntityState[entityIdx].animLastFrame << 4;
                                }
                            }
                        }
                        if ((input & INPUT_LEFT) != 0) {
                            if (g_FieldEntityState[entityIdx].moveStep == g_FieldEntityState[entityIdx].moveSteps) {
                                g_FieldEntityState[entityIdx].posI = g_FieldEntityState[entityIdx].moveEndI;
                                g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_DONE;
                            } else {
                                (g_FieldEntityState[entityIdx].moveStep)++;
                                g_FieldEntityState[entityIdx].animCurrentFrame += g_FieldEntityState[entityIdx].animSpeed;
                                if (g_FieldEntityState[entityIdx].animCurrentFrame > g_FieldEntityState[entityIdx].animLastFrame << 4) {
                                    g_FieldEntityState[entityIdx].animCurrentFrame = 0;
                                }
                            }
                        }
                    }
                } else if (g_FieldEntityState[entityIdx].moveStep == g_FieldEntityState[entityIdx].moveSteps) {
                    g_FieldEntityState[entityIdx].posI = g_FieldEntityState[entityIdx].moveEndI;
                    g_FieldEntityState[entityIdx].actionState = FIELD_MOVE_STATE_DONE;
                } else {
                    (g_FieldEntityState[entityIdx].moveStep)++;
                    g_FieldEntityState[entityIdx].animCurrentFrame += g_FieldEntityState[entityIdx].animSpeed;
                    if (g_FieldEntityState[entityIdx].animCurrentFrame > g_FieldEntityState[entityIdx].animLastFrame << 4) {
                        g_FieldEntityState[entityIdx].animCurrentFrame = 0;
                    }
                }
                g_FieldEntityState[entityIdx].posX = Field_InterpolateLinear(g_FieldEntityState[entityIdx].moveStartX, g_FieldEntityState[entityIdx].moveEndX, g_FieldEntityState[entityIdx].moveSteps, g_FieldEntityState[entityIdx].moveStep);
                g_FieldEntityState[entityIdx].posY = Field_InterpolateLinear(g_FieldEntityState[entityIdx].moveStartY, g_FieldEntityState[entityIdx].moveEndY, g_FieldEntityState[entityIdx].moveSteps, g_FieldEntityState[entityIdx].moveStep);
                g_FieldEntityState[entityIdx].posZ = Field_InterpolateLinear(g_FieldEntityState[entityIdx].moveStartZ, g_FieldEntityState[entityIdx].moveEndZ, g_FieldEntityState[entityIdx].moveSteps, g_FieldEntityState[entityIdx].moveStep);
            }
        }
    }
}

// Sets g_FieldActorState.state.requestCode to 1 and copies four shorts from the gateway record into
// the actor state: eventCmdParam, pcTileX, pcTileY and walkMeshId. The gateway's entryId, an
// unsigned byte, is widened to the short at g_FieldActorState.state.pcDirection.
// FUNCTION: FF7 0x00636233
void __cdecl Field_TriggerGatewayExit(FieldGatewayRecord *gateway)
{
    g_FieldActorState.state.requestCode = FIELD_REQUEST_MAP_JUMP;
    g_FieldActorState.state.eventCmdParam = gateway->eventCmdParam;
    g_FieldActorState.state.pcTileX = gateway->pcTileX;
    g_FieldActorState.state.pcTileY = gateway->pcTileY;
    g_FieldActorState.state.pcWalkMeshId = gateway->walkMeshId;
    g_FieldActorState.state.entryId = gateway->entryId;
}

/*
 * On a rising edge of input flag 32, computes the selected entity integer position and ranks
 * eligible other entities by wrapped heading difference. Excludes hidden entities, identical XY
 * positions, Z differences outside strict -256 through 256, and distances at least the sum of two
 * stored radii. Chooses the first strictly smallest angle below 64 and sets that entity byte flag
 * to one. The local angle array has 16 entries but loops use the unbounded signed global entity
 * count; equal best angles preserve the earlier selection and no flags are cleared.
 */
// FUNCTION: FF7 0x00636284
void __cdecl Field_FlagNearestFacedEntityOnInputEdge(void)
{
    struct {
        short selected;
        int index;
        short angles[16];
        VECTOR other;
        int distance;
        VECTOR current;
        short best;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if ((g_FieldActorState.state.activeKeys2_32 & INPUT_CIRCLE) != 0 && (g_FieldActorState.state.oldActiveKeys2_32 & INPUT_CIRCLE) == 0) {
        frame.current.vx = g_FieldEntityState[g_FieldPlayerEntityIndex].posX >> 12;
        frame.current.vy = g_FieldEntityState[g_FieldPlayerEntityIndex].posY >> 12;
        frame.current.vz = g_FieldEntityState[g_FieldPlayerEntityIndex].posZ >> 12;
        for (frame.index = 0; frame.index < g_FieldActorState.state.modelCount; frame.index++) {
            frame.angles[frame.index] = 0x100 /* 22.5 deg */;
            if (frame.index != g_FieldPlayerEntityIndex && (char)g_FieldEntityState[frame.index].talkOff == 0) {
                frame.other.vx = g_FieldEntityState[frame.index].posX >> 12;
                frame.other.vy = g_FieldEntityState[frame.index].posY >> 12;
                frame.other.vz = g_FieldEntityState[frame.index].posZ >> 12;
                if (frame.current.vx == frame.other.vx && frame.current.vy == frame.other.vy) {
                    continue;
                }
                if (frame.current.vz - frame.other.vz > -256 && frame.current.vz - frame.other.vz < 256) {
                    frame.angles[frame.index] = g_FieldEntityState[g_FieldPlayerEntityIndex].dir - (Field_HeadingBetweenPoints((FieldPointTo *)&frame.current, (FieldPointTo *)&frame.other, &frame.distance) & 0xFF) & 0xFF;
                    if (frame.angles[frame.index] > 0x80) {
                        frame.angles[frame.index] = 0x100 - frame.angles[frame.index];
                    }
                    if (frame.distance >= g_FieldEntityState[frame.index].talkRange + g_FieldEntityState[g_FieldPlayerEntityIndex].solidRange) {
                        frame.angles[frame.index] = 0x100 /* 22.5 deg */;
                    }
                }
            }
        }
        frame.selected = g_FieldPlayerEntityIndex;
        frame.best = 0x40;
        for (frame.index = 0; frame.index < g_FieldActorState.state.modelCount; frame.index++) {
            if (frame.angles[frame.index] < frame.best) {
                frame.best = frame.angles[frame.index];
                frame.selected = (short)frame.index;
            }
        }
        if (frame.selected != g_FieldPlayerEntityIndex && frame.best != 0x40) {
            g_FieldEntityState[frame.selected].requestTalkScript = 1;
        }
    }
}

/*
 * Returns the short at g_FieldHeadTrigTable+(angle&0xFF)*4. Table payload and sine interpretation are
 * separate evidence.
 */
// FUNCTION: FF7 0x006364eb
short __cdecl Field_SineByteAngle(int angle)
{
    return *(short *)((char *)&g_FieldHeadTrigTable + (angle & 0xFF) * 4);
}

/*
 * Returns the short at g_FieldHeadTrigTable.cosineTable+(angle&0xFF)*4. Table payload and cosine
 * interpretation are separate evidence.
 */
// FUNCTION: FF7 0x00636500
short __cdecl Field_CosineByteAngle(int angle)
{
    return *(short *)((char *)&g_FieldHeadTrigTable.cosineTable + (angle & 0xFF) * 4);
}

/*
 * Computes first-two-coordinate difference, stores integer square-root distance through distance,
 * scales differences by 4096/distance then/32 and selects a quadrant-folded table heading by signs
 * and squared-component comparison. Returns the resulting byte minus 0xC0. Coincident points
 * divide by zero.
 */
// FUNCTION: FF7 0x00636515
int __cdecl Field_HeadingBetweenPoints(FieldPointTo *from, FieldPointTo *to, int *distance)
{
    int dirX;
    unsigned char angle;
    int dirY;

    dirX = to->x - from->x;
    dirY = to->y - from->y;
    *distance = dirX * dirX + dirY * dirY;
    *distance = SqrtToInt(*distance);
    dirX = ((dirX << 12) / *distance) / 32;
    dirY = ((dirY << 12) / *distance) / 32;
    if (dirX * dirX > dirY * dirY) {
        if (dirX > 0) {
            if (dirY > 0) {
                angle = (char)g_FieldArcsinTable[dirY];
            } else {
                angle = 0x100 - g_FieldArcsinTable[-dirY];
            }
        } else if (dirY > 0) {
            angle = 0x80 - g_FieldArcsinTable[dirY];
        } else {
            angle = g_FieldArcsinTable[-dirY] + 0x80;
        }
    } else if (dirY > 0) {
        if (dirX > 0) {
            angle = 0x40 - g_FieldArcsinTable[dirX];
        } else {
            angle = 0x80 - (0x40 - g_FieldArcsinTable[-dirX]);
        }
    } else if (dirX > 0) {
        angle = 0x100 - (0x40 - g_FieldArcsinTable[dirX]);
    } else {
        angle = 0x40 - g_FieldArcsinTable[-dirX] + 0x80;
    }
    return angle - 0xC0;
}

/*
 * Compares integer-coordinate squared separation between current and target fixed-point fields.
 * With nonzero radiusBias returns zero inside the radius-derived limit; otherwise snaps both
 * current fields to target when within squared speed or below four. Else writes table-derived
 * heading minus headingOffset to the heading member and returns one.
 */
// FUNCTION: FF7 0x006366a1
int __cdecl Field_AimEntityAtWalkTarget(FieldEntityWalkAim *aim, short radiusBias)
{
    struct {
        int deltaY, distance, speedSquared, deltaX, limit;
        VECTOR to, from;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.from.vx = aim->fromX >> 12;
    frame.from.vy = aim->fromY >> 12;
    frame.to.vx = aim->toX >> 12;
    frame.to.vy = aim->toY >> 12;
    frame.deltaX = frame.to.vx - frame.from.vx;
    frame.deltaY = frame.to.vy - frame.from.vy;
    frame.distance = frame.deltaX * frame.deltaX + frame.deltaY * frame.deltaY;
    frame.limit = (aim->radius + radiusBias) * (aim->radius + radiusBias) + 4096;
    if (radiusBias != 0 && frame.distance <= frame.limit) {
        return 0;
    } else {
        frame.speedSquared = aim->speed * aim->speed >> 16;
        if (frame.distance < frame.speedSquared || frame.distance < 4) {
            aim->fromX = aim->toX;
            aim->fromY = aim->toY;
            return 0;
        } else {
            aim->heading = (Field_HeadingBetweenPoints((FieldPointTo *)&frame.from.vx, (FieldPointTo *)&frame.to.vx, &frame.distance) & 0xFF) - aim->headingOffset;
            return 1;
        }
    }
}

/*
 * Traverses walkmesh triangles toward the integer point obtained by dividing position X/Y by 4096.
 * Tests the three edge half-planes in order, following an enabled neighbor across the first failed
 * edge; a missing or disabled neighbor records the blocking triangle/edge, copies its vertices to
 * blockEdge and returns 8 or -8 according to the edge-direction dot product. A contained point returns
 * 0. Every exit replaces position Z with the triangle-height result without rescaling it by 4096.
 * Traversal has no iteration limit.
 */
// FUNCTION: FF7 0x006367b7
int __cdecl Field_WalkmeshSlideToPoint(unsigned short *triangle, FieldVec3i *position, FieldSlideNormal *normal, FieldVec3i *blockEdge)
{
    int xy[4];
    int edgeBC[4];
    int edgeC[4];
    int edge01[4];
    int sideTest1;
    int sideTest2;
    int retVal;
    int side0;
    short neighbor;

    neighbor = 0;
    side0 = 0;
    sideTest1 = 0;
    sideTest2 = 0;
    retVal = 0;
    retVal = 0;
    xy[0] = position->x / 4096;
    xy[1] = position->y / 4096;
    xy[2] = 0;
    g_FieldWalkmeshEdge = -1;
    while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
        Field_VecSub16To32(
            (FieldVec3i *)edge01,
            (FieldVec16 *)&g_pFieldWalkmeshVertices[*triangle * 3 + 1],
            (FieldVec16 *)&g_pFieldWalkmeshVertices[*triangle * 3]);
        Field_VecSub16To32(
            (FieldVec3i *)edgeBC,
            (FieldVec16 *)&g_pFieldWalkmeshVertices[*triangle * 3 + 2],
            (FieldVec16 *)&g_pFieldWalkmeshVertices[*triangle * 3 + 1]);
        Field_VecSub16To32(
            (FieldVec3i *)edgeC,
            (FieldVec16 *)&g_pFieldWalkmeshVertices[*triangle * 3],
            (FieldVec16 *)&g_pFieldWalkmeshVertices[*triangle * 3 + 2]);
        side0 = edge01[1] * (xy[0] - g_pFieldWalkmeshVertices[*triangle * 3].x) -
            edge01[0] * (xy[1] - g_pFieldWalkmeshVertices[*triangle * 3].y);
        sideTest1 = edgeBC[1] * (xy[0] - g_pFieldWalkmeshVertices[*triangle * 3 + 1].x) -
            edgeBC[0] * (xy[1] - g_pFieldWalkmeshVertices[*triangle * 3 + 1].y);
        sideTest2 = edgeC[1] * (xy[0] - g_pFieldWalkmeshVertices[*triangle * 3 + 2].x) -
            edgeC[0] * (xy[1] - g_pFieldWalkmeshVertices[*triangle * 3 + 2].y);
        if (side0 >= 0 && sideTest1 >= 0 && sideTest2 >= 0) {
            break;
        }
        if (side0 < 0) {
            neighbor = g_pFieldWalkmeshAccessTable[*triangle * 3];
            if (neighbor >= 0 && (g_FieldActorState.state.blockedAccesses[neighbor >> 3] >> (neighbor % 8) & 1) == 0) {
                *triangle = neighbor;
            } else {
                blockEdge->x = edge01[0];
                blockEdge->y = edge01[1];
                blockEdge->z = edge01[2];
                if (edge01[0] * normal->normalX + edge01[1] * normal->normalY >= 0) {
                    retVal = 8;
                } else {
                    retVal = -8;
                }
                g_FieldWalkmeshEdge = (short)*triangle;
                g_FieldWalkmeshAxis = 0;
                break;
            }
        } else if (sideTest1 < 0) {
            neighbor = g_pFieldWalkmeshAccessTable[*triangle * 3 + 1];
            if (neighbor >= 0 && (g_FieldActorState.state.blockedAccesses[neighbor >> 3] >> (neighbor % 8) & 1) == 0) {
                *triangle = neighbor;
            } else {
                blockEdge->x = edgeBC[0];
                blockEdge->y = edgeBC[1];
                blockEdge->z = edgeBC[2];
                if (edgeBC[0] * normal->normalX + edgeBC[1] * normal->normalY >= 0) {
                    retVal = 8;
                } else {
                    retVal = -8;
                }
                g_FieldWalkmeshEdge = (short)*triangle;
                g_FieldWalkmeshAxis = 1;
                break;
            }
        } else if (sideTest2 < 0) {
            neighbor = g_pFieldWalkmeshAccessTable[*triangle * 3 + 2];
            if (neighbor >= 0 && (g_FieldActorState.state.blockedAccesses[neighbor >> 3] >> (neighbor % 8) & 1) == 0) {
                *triangle = neighbor;
            } else {
                blockEdge->x = edgeC[0];
                blockEdge->y = edgeC[1];
                blockEdge->z = edgeC[2];
                if (edgeC[0] * normal->normalX + edgeC[1] * normal->normalY >= 0) {
                    retVal = 8;
                } else {
                    retVal = -8;
                }
                g_FieldWalkmeshEdge = (short)*triangle;
                g_FieldWalkmeshAxis = 2;
                break;
            }
        }
    }
    position->z = Field_PointHeightInTriangle(
        (FieldVec3i *)edge01, (FieldVec3i *)edgeBC, (FieldPointTo *)xy,
        (FieldVec16 *)&g_pFieldWalkmeshVertices[*triangle * 3]);
    return retVal;
}

/*
 * Attempts an entity movement step using slope-adjusted heading/speed, testing center and two
 * offset points against the walkmesh and entity collisions. Retries with heading changes of 8,
 * bounded to 16 attempts or 2 for the special player line flag, then performs the final walkmesh
 * projection and player line, gateway and door tests. Field 0x107/entity 1 and field
 * 0x22E/nonplayer bypass the accumulated collision results. Only an entirely clear result commits
 * X/Y and height scaled by 4096, selects a valid player movement animation and returns 1; blockage
 * returns 0. Vector operations use scalar-local addresses and therefore depend on surrounding
 * frame storage.
 */
// FUNCTION: FF7 0x00636c41
int __cdecl Field_StepEntityOnWalkmesh(short entityIndex)
{
    int rightHit;
    int leftSlide;
    int bumpStatus;
    FieldVec3i probePos;
    int padEntry; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int slideCenter;
    int loopCount;
    int leftBump;
    int slideRightAmt;
    int padBlock; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    FieldSlideNormal normSlideX;
    FieldVec3i posX;
    FieldVec3i gradientX;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int absNormalX8;
    int finalResult;
    int absNy;
    unsigned short tri;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unusedSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    FieldVec3i edgeX12;
    FieldVec3i edge01X6;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    GfxEngineContext *stepCtx;

    tri = g_FieldEntityState[entityIndex].posI;
    edge01X6.x = g_pFieldWalkmeshVertices[tri * 3 + 1].x - g_pFieldWalkmeshVertices[tri * 3].x;
    edge01X6.y = g_pFieldWalkmeshVertices[tri * 3 + 1].y - g_pFieldWalkmeshVertices[tri * 3].y;
    edge01X6.z = g_pFieldWalkmeshVertices[tri * 3 + 1].z - g_pFieldWalkmeshVertices[tri * 3].z;
    edgeX12.x = g_pFieldWalkmeshVertices[tri * 3 + 2].x - g_pFieldWalkmeshVertices[tri * 3 + 1].x;
    edgeX12.y = g_pFieldWalkmeshVertices[tri * 3 + 2].y - g_pFieldWalkmeshVertices[tri * 3 + 1].y;
    edgeX12.z = g_pFieldWalkmeshVertices[tri * 3 + 2].z - g_pFieldWalkmeshVertices[tri * 3 + 1].z;
    Field_VecCross32(&edge01X6, &edgeX12, &gradientX);
    gradientX.x = gradientX.x / 256;
    gradientX.y = gradientX.y / 256;
    gradientX.z = gradientX.z / 256;
    NormalizeVectorFixed(&gradientX, &gradientX);
    gradientX.x = (gradientX.z << 12) / SquareRoot12((gradientX.x * gradientX.x / 4096) + (gradientX.z * gradientX.z / 4096));
    gradientX.y = (gradientX.z << 12) / SquareRoot12((gradientX.y * gradientX.y / 4096) + (gradientX.z * gradientX.z / 4096));
    if (gradientX.x > 4096) {
        gradientX.x = 4096;
    }
    if (gradientX.x < -4096) {
        gradientX.x = -4096;
    }
    if (gradientX.y > 4096) {
        gradientX.y = 4096;
    }
    if (gradientX.y < -4096) {
        gradientX.y = -4096;
    }
    if (gradientX.z > 4096) {
        gradientX.z = 4096;
    }
    if (gradientX.z < -4096) {
        gradientX.z = -4096;
    }
    absNormalX8 = gradientX.x;
    if (absNormalX8 < 0) {
        absNormalX8 = -absNormalX8;
    }
    absNy = gradientX.y;
    if (absNy < 0) {
        absNy = -absNy;
    }
    loopCount = 0;
    stepCtx = GetEngineContext();
    while (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        loopCount++;
        if (entityIndex == g_FieldPlayerEntityIndex && (unsigned char)g_FieldPlayerCrossedLine == 1) {
            if ((unsigned int)loopCount > (unsigned int)2) {
                g_FieldPlayerCrossedLine = 0;
                break;
            }
        } else if ((unsigned int)loopCount > (unsigned int)0x10) {
            break;
        }
        /* Retail caller treats `Field_SineByteAngle` return (`short`) as `int` for caller zero/sign extension. */
        posX.x = ((short)((int(__cdecl *)(char))Field_SineByteAngle)(g_FieldEntityState[entityIndex].moveDir) * absNormalX8) / 4096;
        /* Retail call-site prototype (char) -> int preserved for /Od caller codegen. */
        posX.y = (-((short)((int(__cdecl *)(char))Field_CosineByteAngle)(g_FieldEntityState[entityIndex].moveDir) * absNy)) / 4096;
        posX.x = (posX.x * g_FieldEntityState[entityIndex].moveSpeed) / 256;
        posX.y = (posX.y * g_FieldEntityState[entityIndex].moveSpeed) / 256;
        posX.x = g_FieldEntityState[entityIndex].posX + posX.x;
        posX.y = g_FieldEntityState[entityIndex].posY + posX.y;
        posX.z = g_FieldEntityState[entityIndex].posZ;
        normSlideX.normalX = (short)Field_SineByteAngle(g_FieldEntityState[entityIndex].moveDir + 32) * g_FieldEntityState[entityIndex].solidRange;
        normSlideX.normalY = -(short)Field_CosineByteAngle(g_FieldEntityState[entityIndex].moveDir + 32) * g_FieldEntityState[entityIndex].solidRange;
        probePos.x = posX.x + normSlideX.normalX;
        probePos.y = posX.y + normSlideX.normalY;
        probePos.z = posX.z;
        leftSlide = Field_WalkmeshSlideToPoint(&tri, &probePos, &normSlideX, &edgeX12);
        if (Field_TestEntityCollision(entityIndex, (FieldTriggerCrossingPoint *)&probePos) != 0) {
            leftBump = 1;
        } else {
            leftBump = 0;
        }
        tri = g_FieldEntityState[entityIndex].posI;
        normSlideX.normalX = (short)Field_SineByteAngle(g_FieldEntityState[entityIndex].moveDir - 32) * g_FieldEntityState[entityIndex].solidRange;
        normSlideX.normalY = -(short)Field_CosineByteAngle(g_FieldEntityState[entityIndex].moveDir - 32) * g_FieldEntityState[entityIndex].solidRange;
        probePos.x = posX.x + normSlideX.normalX;
        probePos.y = posX.y + normSlideX.normalY;
        probePos.z = posX.z;
        slideRightAmt = Field_WalkmeshSlideToPoint(&tri, &probePos, &normSlideX, &gradientX);
        if (Field_TestEntityCollision(entityIndex, (FieldTriggerCrossingPoint *)&probePos) != 0) {
            rightHit = 1;
        } else {
            rightHit = 0;
        }
        tri = g_FieldEntityState[entityIndex].posI;
        /* Retail caller treats `Field_SineByteAngle` return (`short`) as `int` for caller zero/sign extension. */
        normSlideX.normalX = (short)((int(__cdecl *)(char))Field_SineByteAngle)(g_FieldEntityState[entityIndex].moveDir) * g_FieldEntityState[entityIndex].solidRange;
        /* Retail call-site prototype (char) -> int preserved for /Od caller codegen. */
        normSlideX.normalY = -(short)((int(__cdecl *)(char))Field_CosineByteAngle)(g_FieldEntityState[entityIndex].moveDir) * g_FieldEntityState[entityIndex].solidRange;
        probePos.x = posX.x + normSlideX.normalX;
        probePos.y = posX.y + normSlideX.normalY;
        probePos.z = posX.z;
        slideCenter = Field_WalkmeshSlideToPoint(&tri, &probePos, &normSlideX, &edge01X6);
        if (Field_TestEntityCollision(entityIndex, (FieldTriggerCrossingPoint *)&probePos) != 0 && slideCenter == 0) {
            bumpStatus = 8;
        } else {
            bumpStatus = 0;
        }
        if (slideCenter == 0 && leftSlide == 0 && slideRightAmt == 0 && bumpStatus == 0 && leftBump == 0 && rightHit == 0) {
            break;
        }
        if (entityIndex == g_FieldPlayerEntityIndex && (char)g_FieldActorState.state.characterLock == 0) {
            if (bumpStatus != 0 || (leftBump != 0 || rightHit != 0)) {
                break;
            }
        } else if (slideCenter != 0 && leftSlide == 0 && slideRightAmt == 0) {
            g_FieldEntityState[entityIndex].moveDir -= slideCenter;
        } else if (bumpStatus != 0 && leftBump == 0 && rightHit == 0) {
            g_FieldEntityState[entityIndex].moveDir -= bumpStatus;
        }
        if (leftSlide != 0 && slideRightAmt != 0) {
            break;
        }
        if (leftSlide != 0 || leftBump != 0) {
            g_FieldEntityState[entityIndex].moveDir -= 8;
        } else if (slideRightAmt != 0 || rightHit != 0) {
            g_FieldEntityState[entityIndex].moveDir += 8;
        }
    }
    finalResult = Field_WalkmeshSlideToPoint(&g_FieldEntityState[entityIndex].posI, &posX, &normSlideX, &edge01X6);
    if (entityIndex == g_FieldPlayerEntityIndex && (char)g_FieldActorState.state.characterLock == 0) {
        g_FieldPlayerCrossedLine = Field_TestScriptLineTriggersWithCrossing((FieldScriptTriggerLine *)&g_FieldEntityState[entityIndex], g_FieldLines, (FieldFixedPosition *)&posX);
        if ((char)g_FieldActorState.state.gatewaysDisabled == 0) {
            Field_TestGateways(&g_FieldEntityState[entityIndex], (FieldGateway *)((char *)g_pFieldScrollBounds + 0x38), (FieldPointTo *)&posX);
        }
        Field_TestDoorTriggers(&g_FieldEntityState[entityIndex], (FieldDoorTriggerRecord *)((char *)g_pFieldScrollBounds + 0x158), (FieldPointTo *)&posX);
    }
    if (g_FieldCurrentMapId == 0x107 && entityIndex == 1) {
        slideCenter = 0;
        leftSlide = 0;
        slideRightAmt = 0;
        bumpStatus = 0;
        leftBump = 0;
        rightHit = 0;
        finalResult = 0;
    }
    if (g_FieldCurrentMapId == 0x22E && entityIndex != g_FieldPlayerEntityIndex) {
        slideCenter = 0;
        leftSlide = 0;
        slideRightAmt = 0;
        bumpStatus = 0;
        leftBump = 0;
        rightHit = 0;
        finalResult = 0;
    }
    if (slideCenter == 0 && leftSlide == 0 && slideRightAmt == 0 && bumpStatus == 0 && leftBump == 0 && rightHit == 0 && finalResult == 0) {
        g_FieldEntityState[entityIndex].posX = posX.x;
        g_FieldEntityState[entityIndex].posY = posX.y;
        g_FieldEntityState[entityIndex].posZ = posX.z << 12;
        if ((char)g_FieldEntityState[entityIndex].scriptedMoveMode == FIELD_SCRIPTED_MOVE_NONE && entityIndex == g_FieldPlayerEntityIndex) {
            g_FieldEntityState[entityIndex].animSpeed = 16;
            if ((g_FieldActorState.state.activeKeys32 & INPUT_CROSS) != 0) {
                if (g_pFieldModelSlots[entityIndex].animationCount > (short)g_FieldActorState.state.runAnimId) {
                    g_FieldEntityState[entityIndex].activeAnimId = g_FieldActorState.state.runAnimIdLo;
                } else {
                    g_FieldEntityState[entityIndex].activeAnimId = 0;
                }
            } else if (g_pFieldModelSlots[entityIndex].animationCount > (short)g_FieldActorState.state.walkAnimId) {
                g_FieldEntityState[entityIndex].activeAnimId = g_FieldActorState.state.walkAnimIdLo;
            } else {
                g_FieldEntityState[entityIndex].activeAnimId = 0;
            }
        }
        return 1;
    } else {
        return 0;
    }
}

/*
 * Scans other enabled field records, requires height difference strictly between -127 and 128,
 * compares squared horizontal distance with the square of half the combined unsigned-short radii
 * and reports one for any strict overlap. When entityIndex is the selected record also sets each
 * overlapping record byte flag. Does not stop after first overlap.
 */
// FUNCTION: FF7 0x00637724
int __cdecl Field_TestEntityCollision(short entityIndex, FieldTriggerCrossingPoint *position)
{
    int result;
    int radiusSq;
    int deltaX;
    int distSq;
    short otherIdx;
    int deltaY;
    int dH;
    int selfRadius;

    result = 0;
    selfRadius = g_FieldEntityState[entityIndex].solidRange;
    for (otherIdx = 0; otherIdx < g_FieldActorState.state.modelCount; otherIdx++) {
        if (otherIdx != entityIndex && (char)g_FieldEntityState[otherIdx].solidOff == 0) {
            dH = (g_FieldEntityState[otherIdx].posZ >> 12) - position->posZ;
            if (dH > -127 && dH < 128) {
                deltaX = g_FieldEntityState[otherIdx].posX - position->posX >> 12;
                deltaY = g_FieldEntityState[otherIdx].posY - position->posY >> 12;
                radiusSq = (selfRadius + g_FieldEntityState[otherIdx].solidRange) / 2;
                radiusSq *= radiusSq;
                distSq = deltaX * deltaX + deltaY * deltaY;
                if (radiusSq > distSq) {
                    if (entityIndex == g_FieldPlayerEntityIndex) {
                        g_FieldEntityState[otherIdx].requestPushScript = 1;
                    }
                    result = 1;
                }
            }
        }
    }
    return result;
}

/*
 * Projects the point onto the line through six short endpoints using an eight-bit fixed-point
 * parameter and writes all three projected coordinates. Returns squared three-dimensional distance
 * only when projected x AND y lie between endpoint components; otherwise -1. It does not clamp to
 * endpoints or test z bounds, and coincident endpoints divide by zero.
 */
// FUNCTION: FF7 0x00637879
int __cdecl Field_ClosestPointOnSegment(short *line, FieldTriggerCrossingPoint *point, FieldTriggerCrossingPoint *closest)
{
    int result;

    result = -(((line[0] - point->posX) * (line[3] - line[0]) + (line[1] - point->posY) * (line[4] - line[1]) + (line[2] - point->posZ) * (line[5] - line[2])) << 8) / ((line[3] - line[0]) * (line[3] - line[0]) + (line[4] - line[1]) * (line[4] - line[1]) + (line[5] - line[2]) * (line[5] - line[2]));
    closest->posX = (result * (line[3] - line[0]) >> 8) + line[0];
    closest->posY = (result * (line[4] - line[1]) >> 8) + line[1];
    closest->posZ = (result * (line[5] - line[2]) >> 8) + line[2];
    if (((line[0] - closest->posX >= 0 && line[3] - closest->posX <= 0) || (line[0] - closest->posX <= 0 && line[3] - closest->posX >= 0)) && ((line[1] - closest->posY >= 0 && line[4] - closest->posY <= 0) || (line[1] - closest->posY <= 0 && line[4] - closest->posY >= 0))) {
        result = (closest->posX - point->posX) * (closest->posX - point->posX) + (closest->posY - point->posY) * (closest->posY - point->posY) + (closest->posZ - point->posZ) * (closest->posZ - point->posZ);
    } else {
        result = -1;
    }
    return result;
}

/*
 * Updates 32 enabled 0x18-byte field segment records using the actor current and proposed X/Y
 * positions shifted down by 12 bits, retaining current Z. A valid closest-point squared distance
 * strictly below the actor radius squared sets proximity, entry, and optional crossed-result flags;
 * otherwise sets exit on prior proximity and clears proximity. Within proximity, compares signed
 * cross products for side changes including one zero endpoint to set crossing byte 0xF, then sets
 * facing flags if the closest-point heading falls within the actor half-turn test. The heading
 * branch compares current Y with itself, so only X difference controls whether it computes a
 * heading; equal X immediately sets facing flags. Event flags are mostly latched rather than
 * cleared, while byte 0x15 resets for enabled records. Returns the accumulated optional flag.
 */
// FUNCTION: FF7 0x00637abb
char __cdecl Field_TestScriptLineTriggersWithCrossing(FieldScriptTriggerLine *triggerLine, FieldLine *segments, FieldFixedPosition *movedPos)
{
    struct {
        char crossed;
        FieldGeometryFrame state;
    } all;

    all.crossed = 0;
    all.state.currentPtr = (FieldTriggerCrossingPoint *)&all.state.current;
    all.state.movedPtr = (FieldTriggerCrossingPoint *)&all.state.moved;
    all.state.closestPtr = (FieldTriggerCrossingPoint *)&all.state.closest;
    all.state.currentPtr->posX = triggerLine->posX >> 12;
    all.state.currentPtr->posY = triggerLine->posY >> 12;
    all.state.currentPtr->posZ = triggerLine->posZ >> 12;
    all.state.movedPtr->posX = movedPos->posX >> 12;
    all.state.movedPtr->posY = movedPos->posY >> 12;
    all.state.movedPtr->posZ = triggerLine->posZ >> 12;
    for (all.state.index = 0; all.state.index < 32; all.state.index++, segments++) {
        if ((char)segments->isActive == 1) {
            segments->isOnLine = 0;
            all.state.distance = Field_ClosestPointOnSegment((short *)segments, all.state.currentPtr, all.state.closestPtr);
            if (all.state.distance != -1 && all.state.distance < triggerLine->radius * triggerLine->radius) {
                if (segments->slipDisabled == 1) {
                    all.crossed = 1;
                }
                if (segments->touch == 0) {
                    segments->requestTouchOnScript = 1;
                }
                segments->touch = 1;
                all.state.currentSide = (segments->pos.x2 - segments->pos.x1) * (all.state.currentPtr->posY - segments->pos.y1) - (all.state.currentPtr->posX - segments->pos.x1) * (segments->pos.y2 - segments->pos.y1);
                all.state.movedSide = (segments->pos.x2 - segments->pos.x1) * (all.state.movedPtr->posY - segments->pos.y1) - (all.state.movedPtr->posX - segments->pos.x1) * (segments->pos.y2 - segments->pos.y1);
                if ((all.state.currentSide >= 0 && all.state.movedSide < 0) || (all.state.movedSide >= 0 && all.state.currentSide < 0) || (all.state.currentSide > 0 && all.state.movedSide <= 0) || (all.state.movedSide > 0 && all.state.currentSide <= 0)) {
                    segments->across = 1;
                }
                if (all.state.currentPtr->posX != all.state.closestPtr->posX || all.state.currentPtr->posY != all.state.currentPtr->posY) {
                    segments->proximityAngle = Field_HeadingBetweenPoints((FieldPointTo *)all.state.currentPtr, (FieldPointTo *)all.state.closestPtr, &all.state.distance);
                    if ((segments->proximityAngle - triggerLine->direction + 0x40 & 0xFF) < 128) {
                        segments->requestPushScript = 1;
                        segments->isOnLine = 1;
                    }
                } else {
                    segments->requestPushScript = 1;
                    segments->isOnLine = 1;
                }
            } else {
                if (segments->touch == 1) {
                    segments->requestTouchOffScript = 1;
                }
                segments->touch = 0;
            }
        }
    }
    return (char)all.crossed;
}

/*
 * Unless global/player movement gate blocks, tests 32 active script line records against entity
 * point and squared radius, latching enter/leave flags on state transitions. Independently latches
 * action flag for enabled line with facing difference under 64 and button edge, even without
 * proximity condition. Disabled lines preserve state.
 */
// FUNCTION: FF7 0x00637d35
void __cdecl Field_TestScriptLineTriggers(FieldScriptLineTrigger *triggerLine, int segments)
{
    struct {
        int index;
        VECTOR closest, current;
        FieldTriggerCrossingPoint *closestPtr, *currentPtr;
        int distance;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.currentPtr = (FieldTriggerCrossingPoint *)&frame.current;
    frame.closestPtr = (FieldTriggerCrossingPoint *)&frame.closest;
    if (g_FieldLockedSkipsLines != 0 && (char)g_FieldActorState.state.characterLock == 1) { /* empty first arm: lib/codegen.h */
    } else {
        frame.currentPtr->posX = triggerLine->x >> 12;
        frame.currentPtr->posY = triggerLine->y >> 12;
        frame.currentPtr->posZ = triggerLine->z >> 12;
        for (frame.index = 0; frame.index < 32; frame.index++, segments += 0x18) {
            if ((char)((FieldLine *)segments)->isActive == 1) {
                frame.distance = Field_ClosestPointOnSegment((short *)segments, frame.currentPtr, frame.closestPtr);
                if (frame.distance != -1 && frame.distance < triggerLine->radius * triggerLine->radius) {
                    if (((FieldLine *)segments)->touch == 0) {
                        ((FieldLine *)segments)->requestTouchOnScript = 1;
                    }
                    ((FieldLine *)segments)->touch = 1;
                } else {
                    if (((FieldLine *)segments)->touch == 1) {
                        ((FieldLine *)segments)->requestTouchOffScript = 1;
                    }
                    ((FieldLine *)segments)->touch = 0;
                }
                if (((FieldLine *)segments)->isOnLine == 1 && (((FieldLine *)segments)->proximityAngle - triggerLine->angle + 0x20 & 0xFF) < 0x40 && (g_FieldActorState.state.activeKeys2_32 & INPUT_CIRCLE) != 0 && (g_FieldActorState.state.oldActiveKeys2_32 & INPUT_CIRCLE) == 0) {
                    ((FieldLine *)segments)->requestTalkScript = 1;
                }
            }
        }
    }
}

/*
 * Clears isOnLine in each of the 32 FieldLine records starting at records.
 * Mechanical; nothing else in the records is read or written.
 */
// FUNCTION: FF7 0x00637e88
void __cdecl Field_ResetScriptLineTalkFlags(int records)
{
    int index;

    for (index = 0; index < 32; index++, records += 24) {
        ((FieldLine *)records)->isOnLine = 0;
    }
}

/*
 * Tests twelve nonsentinel gateways near current entity point within squared radius, computes
 * signed side products for current and proposed XY, and triggers for crossing or touching boundary
 * from either nonzero side. Both zero sides do not trigger; continues after trigger so multiple
 * exits possible.
 */
// FUNCTION: FF7 0x00637eba
void __cdecl Field_TestGateways(FieldEntity *entity, FieldGateway *gateways, FieldPointTo *movedPos)
{
    FieldGeometryFrame frame;

    frame.currentPtr = (FieldTriggerCrossingPoint *)&frame.current;
    frame.movedPtr = (FieldTriggerCrossingPoint *)&frame.moved;
    frame.closestPtr = (FieldTriggerCrossingPoint *)&frame.closest;
    frame.currentPtr->posX = entity->posX >> 12;
    frame.currentPtr->posY = entity->posY >> 12;
    frame.currentPtr->posZ = entity->posZ >> 12;
    frame.movedPtr->posX = movedPos->x >> 12;
    frame.movedPtr->posY = movedPos->y >> 12;
    frame.movedPtr->posZ = entity->posZ >> 12;
    for (frame.index = 0; frame.index < 12; frame.index++, gateways++) {
        if (gateways->eventCmdParam != 0x7FFF) {
            frame.distance = Field_ClosestPointOnSegment((short *)gateways, frame.currentPtr, frame.closestPtr);
            if (frame.distance != -1 && frame.distance < entity->solidRange * entity->solidRange) {
                frame.currentSide = (gateways->endX - gateways->startX) * (frame.currentPtr->posY - gateways->startY) - (frame.currentPtr->posX - gateways->startX) * (gateways->endY - gateways->startY);
                frame.movedSide = (gateways->endX - gateways->startX) * (frame.movedPtr->posY - gateways->startY) - (frame.movedPtr->posX - gateways->startX) * (gateways->endY - gateways->startY);
                if ((frame.currentSide >= 0 && frame.movedSide < 0) || (frame.movedSide >= 0 && frame.currentSide < 0) || (frame.currentSide > 0 && frame.movedSide <= 0) || (frame.movedSide > 0 && frame.currentSide <= 0)) {
                    Field_TriggerGatewayExit((FieldGatewayRecord *)gateways);
                }
            }
        }
    }
}

/*
 * For the setting modes (FIELD_DOOR_TRIGGER_SET and its WHILE_NEAR variants) sets the trigger's bit and
 * returns one when it was clear; for the clearing modes clears it and returns one when it was set.
 * Other modes return zero unchanged.
 */
// FUNCTION: FF7 0x00638074
short __cdecl Field_SetDoorTriggerBit(FieldDoorTriggerRecord *doorTrigger, int mode)
{
    short result;
    unsigned char mask;

    result = 0;
    switch (mode & 0xFF) {
    case FIELD_DOOR_TRIGGER_SET:
    case FIELD_DOOR_TRIGGER_SET_WHILE_NEAR:
    case FIELD_DOOR_TRIGGER_SET_WHILE_NEAR_SIDED:
        mask = 1 << doorTrigger->bitIndex;
        if ((g_FieldActorState.state.backgroundLayerVisibility[doorTrigger->byteIndex] & mask) == 0) {
            result = 1;
        }
        g_FieldActorState.state.backgroundLayerVisibility[doorTrigger->byteIndex] |= mask;
        break;
    case FIELD_DOOR_TRIGGER_CLEAR:
    case FIELD_DOOR_TRIGGER_CLEAR_WHILE_NEAR:
    case FIELD_DOOR_TRIGGER_CLEAR_WHILE_NEAR_SIDED:
        mask = 1 << doorTrigger->bitIndex ^ 0xFF;
        if ((g_FieldActorState.state.backgroundLayerVisibility[doorTrigger->byteIndex] | mask) == 0xFF) {
            result = 1;
        }
        g_FieldActorState.state.backgroundLayerVisibility[doorTrigger->byteIndex] &= mask;
    }
    return (short)result;
}

/*
 * Tests 12 enabled 16-byte door segments against the actor current position shifted down by 12 bits
 * and its squared radius. Inside proximity, activates the descriptor mode when exactly at the
 * closest point or facing it within the half-turn test; outside proximity, the SET_WHILE_NEAR modes
 * clear the bit and the CLEAR_WHILE_NEAR modes set it, except that the SIDED modes skip changes on
 * the positive cross-product side. Plays one of sound identifiers 0, 54, 122, or 298 only when
 * Field_SetDoorTriggerBit reports a change. Computes proposed position fields but never uses them
 * afterward.
 */
// FUNCTION: FF7 0x00638177
void __cdecl Field_TestDoorTriggers(FieldEntity *entity, FieldDoorTriggerRecord *doors, FieldPointTo *movedPos)
{
    struct {
        int index;
        VECTOR moved, closest;
        short soundIds[4];
        int angle;
        VECTOR current;
        FieldTriggerCrossingPoint *closestPtr, *currentPtr, *movedPtr;
        int distance;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.soundIds[0] = 0;
    frame.soundIds[1] = 0x36;
    frame.soundIds[2] = 0x7A;
    frame.soundIds[3] = 0x12A;
    frame.currentPtr = (FieldTriggerCrossingPoint *)&frame.current;
    frame.movedPtr = (FieldTriggerCrossingPoint *)&frame.moved;
    frame.closestPtr = (FieldTriggerCrossingPoint *)&frame.closest;
    frame.currentPtr->posX = entity->posX >> 12;
    frame.currentPtr->posY = entity->posY >> 12;
    frame.currentPtr->posZ = entity->posZ >> 12;
    frame.movedPtr->posX = movedPos->x >> 12;
    frame.movedPtr->posY = movedPos->y >> 12;
    frame.movedPtr->posZ = entity->posZ >> 12;
    for (frame.index = 0; frame.index < 12; frame.index++, doors++) {
        if (doors->byteIndex != 0xFF) {
            frame.distance = Field_ClosestPointOnSegment((short *)doors, frame.currentPtr, frame.closestPtr);
            if (frame.distance != -1 && frame.distance < entity->solidRange * entity->solidRange) {
                if (frame.currentPtr->posX == frame.closestPtr->posX && frame.currentPtr->posY == frame.closestPtr->posY) {
                    /* Retail caller prototype (int, char) -> short preserved for /Od argument promotion on `Field_SetDoorTriggerBit`. */
                    if ((short)((short(__cdecl *)(int, char))Field_SetDoorTriggerBit)((int)doors, *(char *)&doors->mode) == 1) {
                        Field_PlayDoorTriggerSoundEffect(frame.soundIds[doors->soundIndex]);
                    }
                } else {
                    *(char *)&frame.angle = Field_HeadingBetweenPoints((FieldPointTo *)frame.currentPtr, (FieldPointTo *)frame.closestPtr, &frame.distance);
                    if (((frame.angle & 0xFF) - entity->moveDir + 0x40 & 0xFF) < 128) {
                        /* Retail caller prototype (int, char) -> short preserved for /Od argument promotion on `Field_SetDoorTriggerBit`. */
                        if ((short)((short(__cdecl *)(int, char))Field_SetDoorTriggerBit)((int)doors, *(char *)&doors->mode) == 1) {
                            Field_PlayDoorTriggerSoundEffect(frame.soundIds[doors->soundIndex]);
                        }
                    }
                }
            } else {
                if (doors->mode > FIELD_DOOR_TRIGGER_CLEAR_WHILE_NEAR && (doors->endX - doors->startX) * (frame.currentPtr->posY - doors->startY) - (doors->endY - doors->startY) * (frame.currentPtr->posX - doors->startX) > 0) {
                    continue;
                }
                if (doors->mode == FIELD_DOOR_TRIGGER_SET_WHILE_NEAR || doors->mode == FIELD_DOOR_TRIGGER_SET_WHILE_NEAR_SIDED) {
                    if ((short)Field_SetDoorTriggerBit(doors, FIELD_DOOR_TRIGGER_CLEAR) == 1) {
                        Field_PlayDoorTriggerSoundEffect(frame.soundIds[doors->soundIndex]);
                    }
                }
                if (doors->mode == FIELD_DOOR_TRIGGER_CLEAR_WHILE_NEAR || doors->mode == FIELD_DOOR_TRIGGER_CLEAR_WHILE_NEAR_SIDED) {
                    if ((short)Field_SetDoorTriggerBit(doors, FIELD_DOOR_TRIGGER_SET) == 1) {
                        Field_PlayDoorTriggerSoundEffect(frame.soundIds[doors->soundIndex]);
                    }
                }
            }
        }
    }
}

/*
 * Walks twelve door trigger records, skips byteIndex 0xFF and starts each bit in the state its mode
 * does not set: cleared for the setting modes, set for the clearing modes. Other modes do nothing.
 */
// FUNCTION: FF7 0x00638420
void __cdecl Field_InitDoorTriggers(int doors)
{
    int index;

    for (index = 0; index < 12; index++, doors += 0x10) {
        if (((FieldDoorTriggerRecord *)doors)->byteIndex != 0xFF) {
            switch (((FieldDoorTriggerRecord *)doors)->mode) {
            case FIELD_DOOR_TRIGGER_SET:
            case FIELD_DOOR_TRIGGER_SET_WHILE_NEAR:
            case FIELD_DOOR_TRIGGER_SET_WHILE_NEAR_SIDED:
                Field_SetDoorTriggerBit((struct FieldDoorTriggerRecord *)doors, FIELD_DOOR_TRIGGER_CLEAR);
                break;
            case FIELD_DOOR_TRIGGER_CLEAR:
            case FIELD_DOOR_TRIGGER_CLEAR_WHILE_NEAR:
            case FIELD_DOOR_TRIGGER_CLEAR_WHILE_NEAR_SIDED:
                Field_SetDoorTriggerBit((struct FieldDoorTriggerRecord *)doors, FIELD_DOOR_TRIGGER_SET);
            }
        }
    }
}
} /* extern "C" */
