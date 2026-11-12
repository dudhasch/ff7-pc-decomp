// src/ff7/src/battle/battle_fx_fat_chocobo_summon.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Runs the Fat Chocobo summon sequence: resource loading, the camera script interpreter,
//   the timeline, animated sprites, model growth and fade tasks, camera shake and a screen fill.
//   Together they produce the whole effect.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "lib/codegen.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_summon.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "ff7/enums/battle.h"
#include <string.h>
#include <stdlib.h>
#include "lib/api/sound.h"
#include "lib/enums/graphics.h"
extern "C" {

struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x00898080
unsigned char g_BattleSummonFatChocoboSoundGroup[16] = {0xfb, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfd, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on
// GLOBAL: FF7 0x00898090
unsigned int g_BattleSummonFatChocoboSoundGroupDescriptor[2] = {(unsigned int)&g_BattleSummonFatChocoboSoundGroup, 0x00000000};
// GLOBAL: FF7 0x00898098
unsigned int g_BattleSummonFatChocoboResourceDefs[86] = {
    0x00c800b1,
    // STRING: FF7 0x00898360
    (unsigned int)"RING_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089836c
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x00898378
    (unsigned int)"SMOKE_X1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00898388
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x00898394
    (unsigned int)"SMOKE_X2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008983a4
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x008983b0
    (unsigned int)"SMOKE_X3.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008983c0
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x008983cc
    (unsigned int)"SMOKE_X4.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008983dc
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x008983e8
    (unsigned int)"SMOKE_X5.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008983f8
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x00898404
    (unsigned int)"SMOKE_X6.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00898414
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x00898420
    (unsigned int)"SMOKE_X7.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00898430
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x0089843c
    (unsigned int)"SMOKE_X8.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089844c
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x00898458
    (unsigned int)"SMOKE_X9.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00898468
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x00898474
    (unsigned int)"SMOKE_10.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00898484
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x00898490
    (unsigned int)"SMOKE_11.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008984a0
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x008984ac
    (unsigned int)"SMOKE_12.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008984bc
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x008984c8
    (unsigned int)"SMOKE_13.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008984d8
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x008984e4
    (unsigned int)"SMOKE_14.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008984f4
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x00898500
    (unsigned int)"SMOKE_15.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00898510
    (unsigned int)"DEBUCHO.TIM",
    // STRING: FF7 0x0089851c
    (unsigned int)"SMOKE_16.SPT", 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008981f0
int g_BattleSummonFatChocoboUnused1 = 136;
// clang-format off
unsigned char g_rawData_008981f4[4] = {0x48, 0x00, 0x00, 0x00};
unsigned char g_rawData_008981f8[120] = {
    0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x60, 0xf0, 0x60, 0xf0, 0x60, 0xf0, 0x42, 0x00, 0x1e, 0x00, 0xc0, 0xe0, 0xc0, 0xe0,
    0xc0, 0xe0, 0x80, 0x00, 0x1e, 0x00, 0x40, 0x00, 0x1e, 0x00, 0x81, 0x00, 0x00, 0x00, 0xf0, 0xd8, 0x00, 0x00, 0x41, 0x00, 0x78, 0xec, 0x00, 0x00,
    0x3c, 0xf6, 0x82, 0x00, 0x0c, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0x00, 0x00, 0x40, 0x00, 0x0c, 0x00, 0x81, 0x00, 0x00, 0x00, 0x18, 0xfc, 0x00, 0x00,
    0x41, 0x00, 0xc8, 0x32, 0x68, 0xc5, 0x38, 0xcd, 0x80, 0x00, 0x30, 0x00, 0x40, 0x00, 0x30, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x41, 0x00, 0xf0, 0xd8, 0x78, 0xec, 0x78, 0xec, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00, 0x0f, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00898270
unsigned char g_BattleSummonFatChocoboUnused2[128] = {0x88, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x60, 0xf0, 0x60, 0xf0, 0x60, 0xf0, 0x42, 0x00, 0x1e, 0x00, 0xc0, 0xe0, 0xc0, 0xe0, 0xc0, 0xe0, 0x80, 0x00, 0x1e, 0x00, 0x40, 0x00, 0x1e, 0x00, 0x81, 0x00, 0x00, 0x00, 0xf0, 0xd8, 0x00, 0x00, 0x41, 0x00, 0x78, 0xec, 0x00, 0x00, 0x3c, 0xf6, 0x82, 0x00, 0x0c, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0x00, 0x00, 0x40, 0x00, 0x0c, 0x00, 0x81, 0x00, 0x00, 0x00, 0x18, 0xfc, 0x00, 0x00, 0x41, 0x00, 0xc8, 0x32, 0x68, 0xc5, 0x38, 0xcd, 0x80, 0x00, 0x30, 0x00, 0x40, 0x00, 0x30, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0xf0, 0xd8, 0x78, 0xec, 0x78, 0xec, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00, 0x0f, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008982f0
struct BattleSummonFatChocoboEffectDescriptor g_BattleSummonFatChocoboEffectDescriptor = {0, {0x88, 0, 0, 0, 0, 0}, 0, {0x40, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_0089830c[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

// The sprite table index of each animation step of the animated sprites.
// GLOBAL: FF7 0x00898310
int g_BattleSummonFatChocoboSpriteConfig[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

// clang-format off
#undef g_BattleSummonFatChocoboSpriteRequest
// A BattleFxSpriteRequest (16 bytes, not 12: the last four zero bytes lie between this object and
// the literal pool, and VC5 pads neither, so they can only be its tail).
// GLOBAL: FF7 0x00898350
struct BattleFxSpriteRequest g_BattleSummonFatChocoboSpriteRequest = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x00b4f8a8
struct BattleSummonFatChocoboActorBonePos g_BattleSummonFatChocoboActorBonePos = {0};
unsigned char g_bssPad_00b4f8b0[20] = {0};
// GLOBAL: FF7 0x00b4f8c4
unsigned char g_BattleSummonFatChocoboDescentInitFlag = 0;
// GLOBAL: FF7 0x00b4f8c8
struct BattleFxFatChocoboTargetActor g_BattleSummonFatChocoboTargetActor = {0};
// GLOBAL: FF7 0x00b4f8f0
int g_BattleSummonFatChocoboTargetMask = 0;
// GLOBAL: FF7 0x00b4f8f4
void *g_BattleSummonFatChocoboFadeModelMatrix = 0;
// GLOBAL: FF7 0x00b4f8f8
unsigned char g_BattleSummonFatChocoboCameraFlag1 = 0;
// GLOBAL: FF7 0x00b4f8fc
struct BattleFxFatChocoboCamera *g_BattleSummonFatChocoboCameraState = 0;
// GLOBAL: FF7 0x00b4f900
BattleFxCameraBasisScratch *g_BattleSummonFatChocoboCameraScratch = 0;
unsigned char g_bssPad_00b4f904[4] = {0};
// GLOBAL: FF7 0x00b4f908
unsigned char g_BattleSummonFatChocoboTransformMatrix[30] = {0};
// GLOBAL: FF7 0x00b4f928
struct BattleFxFatChocoboActor g_BattleSummonFatChocoboActor = {0};
// GLOBAL: FF7 0x00b4f950
struct GfxTableCursor *g_BattleSummonFatChocoboModelTable = 0;
unsigned char g_bssPad_00b4f954[4] = {0};
// GLOBAL: FF7 0x00b4f958
struct BattleSummonFatChocoboLightMatrix g_BattleSummonFatChocoboLightMatrix = {0};
// GLOBAL: FF7 0x00b4f978
short g_BattleSummonFatChocoboAngles[3] = {0};
// GLOBAL: FF7 0x00b4f980
struct BattleSummonChocoMogBillboardMatrix g_BattleSummonFatChocoboBillboardMatrix = {0};
// GLOBAL: FF7 0x00b4f9a0
short g_BattleSummonFatChocoboFadeScaleVector[8] = {0};
// GLOBAL: FF7 0x00b4f9b0
short g_BattleSummonFatChocoboFadeScaleValue[7] = {0};
// GLOBAL: FF7 0x00b4f9c0
short g_BattleSummonFatChocoboGrowScaleVector[4] = {0};
// GLOBAL: FF7 0x00b4f9c8
short g_BattleSummonFatChocoboGrowScaleCopy[4] = {0};
// GLOBAL: FF7 0x00b4f9d0
short g_BattleSummonFatChocoboGrowScaleValue[7] = {0};
// clang-format on

// When the engine context exists and the Fat Chocobo cache is empty, builds SUMMON/CHOCO2 with a
// default-duration record, creates resource type 17 using g_BattleSummonFatChocoboResourceDefs, and registers g_BattleSummonFatChocoboSoundGroupDescriptor
// sound groups.
// FUNCTION: FF7 0x00507af0
void __cdecl BattleFx_SummonLoadResFatChocobo(void)
{
    GfxModelTableBank *stageSceneHandle;
    GfxResourceRequest reqBuffer;
    int kind;
    char resolvedName[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSummonFatChocoboModelTable == 0) {
        kind = 0x11;
        resourceDefsWord = (int)&g_BattleSummonFatChocoboResourceDefs;
        stageSceneHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089852c
        pRequest->resourcePath = BattleFx_BuildResourcePath("SUMMON/CHOCO2", resolvedName);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSummonFatChocoboModelTable, stageSceneHandle);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleSummonFatChocoboSoundGroupDescriptor);
    }
}

// Loads Fat Chocobo resources, forwards both arguments to BattleFx_SummonBeginFatChocobo, and returns zero.
// FUNCTION: FF7 0x00507b91
int __cdecl BattleFx_SummonEnterFatChocobo(int targetMask, int actorIdx)
{
    BattleFx_SummonLoadResFatChocobo();
    BattleFx_SummonBeginFatChocobo(targetMask, actorIdx);
    return 0;
}

// Finds the selected actors' bounding-box midpoint, turns the effect half a turn (0x800) when the
// midpoint Z is below the caster's Z, stores the midpoint Z and builds the XYZ rotation matrix.
// Schedules BattleFx_SummonFatChocoboTimelineTask, picks one of the two camera scripts
// (g_BattleSummonFatChocoboUnused1 or g_BattleSummonFatChocoboUnused2) with rand and sets bit 8
// of the stage hide flags. The midpoint Z is the third short of the centroid output; the original
// frame places `midZ` right after `centroid`, which separately declared C locals do not guarantee.
// FUNCTION: FF7 0x00507bad
void __cdecl BattleFx_SummonBeginFatChocobo(int targetMask, int actorIdx)
{
    SVECTOR centroid;

    Battle_GetActorMaskCentroid(g_BattleSummonFatChocoboTargetMask = targetMask, &centroid.vx);
    if (centroid.vz < g_BattleActorRecords[actorIdx].posZ) {
        g_BattleSummonFatChocoboAngles[1] = 0x800;
    }
    g_BattleSummonFatChocoboLightMatrix.value = centroid.vz;
    RotMatrixXYZ((int *)g_BattleSummonFatChocoboAngles, (int *)&g_BattleSummonFatChocoboLightMatrix);
    Battle_AllocateResourceSlot(BattleFx_SummonFatChocoboTimelineTask);
    if ((rand() & 0x100) != 0) {
        BattleFx_FatChocoboSpawnCameraScript(&g_BattleSummonFatChocoboUnused1, &g_BattleSummonFatChocoboLightMatrix, actorIdx);
    } else {
        BattleFx_FatChocoboSpawnCameraScript(&g_BattleSummonFatChocoboUnused2, &g_BattleSummonFatChocoboLightMatrix, actorIdx);
    }
    g_BattleStageHideFlags |= 8;
}

// Allocates a task slot for BattleFx_FatChocoboRunCameraScript with the script cursor (value), the
// camera matrix (value2) and the actor index (value3).
// FUNCTION: FF7 0x00507c64
void __cdecl BattleFx_FatChocoboSpawnCameraScript(void *value, void *value2, int value3)
{
    BattleFxFatChocoboCameraTask *slot;

    slot = (BattleFxFatChocoboCameraTask *)BATTLE_NEW_TASK_SLOT(BattleFx_FatChocoboRunCameraScript);
    slot->scriptCursor = (int)value;
    slot->matrix = (int)value2;
    slot->actorIndex = value3;
}

// Runs the Fat Chocobo camera script: reads opcodes for the caster and the target until both are
// waiting, steps both one frame (waits, linear, accelerated, orbiting and eased moves), then sets
// the camera position and focal point, through the task's matrix when it has one.
// FUNCTION: FF7 0x00507ca4
void __cdecl BattleFx_FatChocoboRunCameraScript(void)
{
    BattleFxTimelineScriptTask *taskSlot;
    int padLocal[42]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unusedStorage[6]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int spareSlots[16]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    Vec3sPacked *offsetPtr;
    short opcode;
    BattleFxDebrisSpawnTask *pVector;
    SVECTOR *delta;
    int eased;
    int i;
    int flag;

    taskSlot = (BattleFxTimelineScriptTask *)BATTLE_CURRENT_TASK_SLOT;
    delta = (SVECTOR *)Gfx_GetPsxScratchpadPtr(0);
    offsetPtr = (Vec3sPacked *)((int)delta + 8);
    pVector = (BattleFxDebrisSpawnTask *)((int)offsetPtr + 8);
    if (g_BattleIsPaused == 0) {
        if (taskSlot->initialized == 0) {
            g_BattleSummonFatChocoboActor.ready = 0;
            g_BattleSummonFatChocoboTargetActor.ready = 0;
            g_BattleSummonFatChocoboActor.boneIndex = 0xFFFF;
            g_BattleSummonFatChocoboTargetActor.boneIndex = 0xFFFF;
            taskSlot->initialized = 1;
        }
        while (g_BattleSummonFatChocoboActor.ready == 0 || g_BattleSummonFatChocoboTargetActor.ready == 0) {
            opcode = *(short *)taskSlot->scriptCursor;
            taskSlot->scriptCursor += 2;
            if ((opcode & 0x40) != 0) {
                g_BattleSummonFatChocoboCameraState = (struct BattleFxFatChocoboCamera *)&g_BattleSummonFatChocoboActor.state;
                g_BattleSummonFatChocoboCameraScratch = (struct BattleFxCameraBasisScratch *)&g_BattleSummonFatChocoboTargetActor.state;
            } else {
                g_BattleSummonFatChocoboCameraState = (struct BattleFxFatChocoboCamera *)&g_BattleSummonFatChocoboTargetActor.state;
                g_BattleSummonFatChocoboCameraScratch = (struct BattleFxCameraBasisScratch *)&g_BattleSummonFatChocoboActor.state;
            }
            g_BattleSummonFatChocoboCameraState->mode = opcode & ~0xC0;
            switch (g_BattleSummonFatChocoboCameraState->mode) {
            case SUMMON_CAMERA_WAIT:
                g_BattleSummonFatChocoboCameraState->frameCount = BattleFx_FatChocoboReadCameraScriptOperand(&taskSlot->scriptCursor);
                break;
            case SUMMON_CAMERA_SET_POSITION:
                g_BattleSummonFatChocoboCameraState->posX = *(short *)taskSlot->scriptCursor;
                taskSlot->scriptCursor += 2;
                g_BattleSummonFatChocoboCameraState->posY = *(short *)taskSlot->scriptCursor;
                taskSlot->scriptCursor += 2;
                g_BattleSummonFatChocoboCameraState->posZ = *(short *)taskSlot->scriptCursor;
                taskSlot->scriptCursor += 2;
                break;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_BattleSummonFatChocoboCameraState->frameCount = BattleFx_FatChocoboReadCameraScriptOperand(&taskSlot->scriptCursor);
                g_BattleSummonFatChocoboCameraState->stepX = (*(short *)taskSlot->scriptCursor - g_BattleSummonFatChocoboCameraState->posX) / g_BattleSummonFatChocoboCameraState->frameCount;
                taskSlot->scriptCursor += 2;
                g_BattleSummonFatChocoboCameraState->stepY = (*(short *)taskSlot->scriptCursor - g_BattleSummonFatChocoboCameraState->posY) / g_BattleSummonFatChocoboCameraState->frameCount;
                taskSlot->scriptCursor += 2;
                g_BattleSummonFatChocoboCameraState[1].mode = (*(short *)taskSlot->scriptCursor - g_BattleSummonFatChocoboCameraState->posZ) / g_BattleSummonFatChocoboCameraState->frameCount;
                taskSlot->scriptCursor += 2;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                delta->vx = g_BattleSummonFatChocoboCameraScratch->posX - g_BattleSummonFatChocoboCameraState->posX;
                delta->vy = g_BattleSummonFatChocoboCameraScratch->posY - g_BattleSummonFatChocoboCameraState->posY;
                delta->vz = g_BattleSummonFatChocoboCameraScratch->posZ - g_BattleSummonFatChocoboCameraState->posZ;
                g_BattleSummonFatChocoboCameraState->progress = SqrtToInt(delta->vx * delta->vx + delta->vy * delta->vy + delta->vz * delta->vz);
                *(Int64Pair *)((char *)g_BattleSummonFatChocoboCameraState + 0x18) = *(Int64Pair *)((char *)g_BattleSummonFatChocoboCameraState + 0x10);
                g_BattleSummonFatChocoboCameraState->frameCount = BattleFx_FatChocoboReadCameraScriptOperand(&taskSlot->scriptCursor);
                delta->vx = *(short *)taskSlot->scriptCursor;
                taskSlot->scriptCursor += 2;
                delta->vy = *(short *)taskSlot->scriptCursor;
                taskSlot->scriptCursor += 2;
                delta->vz = *(short *)taskSlot->scriptCursor;
                taskSlot->scriptCursor += 2;
                g_BattleSummonFatChocoboCameraState->stepX = (delta->vx - g_BattleSummonFatChocoboCameraState->posX) / g_BattleSummonFatChocoboCameraState->frameCount;
                g_BattleSummonFatChocoboCameraState->stepY = (delta->vy - g_BattleSummonFatChocoboCameraState->posY) / g_BattleSummonFatChocoboCameraState->frameCount;
                g_BattleSummonFatChocoboCameraState[1].mode = (delta->vz - g_BattleSummonFatChocoboCameraState->posZ) / g_BattleSummonFatChocoboCameraState->frameCount;
                switch (g_BattleSummonFatChocoboCameraScratch->mode) {
                case SUMMON_CAMERA_MOVE_LINEAR:
                    offsetPtr->vx = g_BattleSummonFatChocoboCameraScratch->posX + g_BattleSummonFatChocoboCameraScratch->frameCounter * g_BattleSummonFatChocoboCameraScratch->velocity;
                    offsetPtr->vy = g_BattleSummonFatChocoboCameraScratch->posY + g_BattleSummonFatChocoboCameraScratch[1].mode * g_BattleSummonFatChocoboCameraScratch->velocity;
                    offsetPtr->vz = g_BattleSummonFatChocoboCameraScratch->posZ + g_BattleSummonFatChocoboCameraScratch[1].velocity * g_BattleSummonFatChocoboCameraScratch->velocity;
                    break;
                default:
                    offsetPtr->vx = g_BattleSummonFatChocoboCameraScratch->posX;
                    offsetPtr->vy = g_BattleSummonFatChocoboCameraScratch->posY;
                    offsetPtr->vz = g_BattleSummonFatChocoboCameraScratch->posZ;
                }
                delta->vx -= offsetPtr->vx;
                delta->vy -= offsetPtr->vy;
                delta->vz -= offsetPtr->vz;
                g_BattleSummonFatChocoboCameraState->progressStep = (SqrtToInt(delta->vx * delta->vx + delta->vy * delta->vy + delta->vz * delta->vz) - g_BattleSummonFatChocoboCameraState->progress) / g_BattleSummonFatChocoboCameraState->frameCount;
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_BattleSummonFatChocoboCameraState->frameCount = BattleFx_FatChocoboReadCameraScriptOperand(&taskSlot->scriptCursor);
                delta->vx = *(short *)taskSlot->scriptCursor - g_BattleSummonFatChocoboCameraState->posX;
                taskSlot->scriptCursor += 2;
                delta->vy = *(short *)taskSlot->scriptCursor - g_BattleSummonFatChocoboCameraState->posY;
                taskSlot->scriptCursor += 2;
                delta->vz = *(short *)taskSlot->scriptCursor - g_BattleSummonFatChocoboCameraState->posZ;
                taskSlot->scriptCursor += 2;
                g_BattleSummonFatChocoboCameraState->deltaX = (delta->vx - g_BattleSummonFatChocoboCameraState->stepX * g_BattleSummonFatChocoboCameraState->frameCount << 1) / (g_BattleSummonFatChocoboCameraState->frameCount * g_BattleSummonFatChocoboCameraState->frameCount);
                g_BattleSummonFatChocoboCameraState->deltaY = (delta->vy - g_BattleSummonFatChocoboCameraState->stepY * g_BattleSummonFatChocoboCameraState->frameCount << 1) / (g_BattleSummonFatChocoboCameraState->frameCount * g_BattleSummonFatChocoboCameraState->frameCount);
                g_BattleSummonFatChocoboCameraState->deltaZ = (delta->vz - g_BattleSummonFatChocoboCameraState[1].mode * g_BattleSummonFatChocoboCameraState->frameCount << 1) / (g_BattleSummonFatChocoboCameraState->frameCount * g_BattleSummonFatChocoboCameraState->frameCount);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_BattleSummonFatChocoboCameraState->frameCount = BattleFx_FatChocoboReadCameraScriptOperand(&taskSlot->scriptCursor);
                g_BattleSummonFatChocoboCameraState->progress = 0;
                g_BattleSummonFatChocoboCameraState->progressStep = 0x1000 / g_BattleSummonFatChocoboCameraState->frameCount;
                g_BattleSummonFatChocoboCameraState->deltaX = g_BattleSummonFatChocoboCameraState->posX;
                g_BattleSummonFatChocoboCameraState->deltaY = g_BattleSummonFatChocoboCameraState->posY;
                g_BattleSummonFatChocoboCameraState->deltaZ = g_BattleSummonFatChocoboCameraState->posZ;
                g_BattleSummonFatChocoboCameraState->stepX = *(short *)taskSlot->scriptCursor - g_BattleSummonFatChocoboCameraState->posX;
                taskSlot->scriptCursor += 2;
                g_BattleSummonFatChocoboCameraState->stepY = *(short *)taskSlot->scriptCursor - g_BattleSummonFatChocoboCameraState->posY;
                taskSlot->scriptCursor += 2;
                g_BattleSummonFatChocoboCameraState[1].mode = *(short *)taskSlot->scriptCursor - g_BattleSummonFatChocoboCameraState->posZ;
                taskSlot->scriptCursor += 2;
                break;
            case SUMMON_CAMERA_ATTACH_BONE:
                if ((g_BattleSummonFatChocoboCameraState->scriptWord = *(*(short **)&taskSlot->scriptCursor)++) == 0) {
                    g_BattleSummonFatChocoboCameraState->scriptWord = taskSlot->scriptWord;
                }
                g_BattleSummonFatChocoboCameraState->scriptArgument = *(short *)taskSlot->scriptCursor;
                taskSlot->scriptCursor += 2;
                break;
            case SUMMON_CAMERA_DETACH:
                g_BattleSummonFatChocoboCameraState->scriptWord = 0xFFFF;
                break;
            case SUMMON_CAMERA_HOLD_OUTPUT:
                g_BattleSummonFatChocoboCameraState->frameCount = BattleFx_FatChocoboReadCameraScriptOperand(&taskSlot->scriptCursor);
                break;
            default:
                taskSlot->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        g_BattleSummonFatChocoboCameraState = (struct BattleFxFatChocoboCamera *)&g_BattleSummonFatChocoboActor.state;
        g_BattleSummonFatChocoboCameraScratch = (struct BattleFxCameraBasisScratch *)&g_BattleSummonFatChocoboTargetActor.state;
        for (i = 0; i < 2; i++) {
            switch (g_BattleSummonFatChocoboCameraState->mode) {
            case SUMMON_CAMERA_WAIT:
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_BattleSummonFatChocoboCameraState->stepX += g_BattleSummonFatChocoboCameraState->deltaX;
                g_BattleSummonFatChocoboCameraState->stepY += g_BattleSummonFatChocoboCameraState->deltaY;
                g_BattleSummonFatChocoboCameraState[1].mode += g_BattleSummonFatChocoboCameraState->deltaZ;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_BattleSummonFatChocoboCameraState->posX += g_BattleSummonFatChocoboCameraState->stepX;
                g_BattleSummonFatChocoboCameraState->posY += g_BattleSummonFatChocoboCameraState->stepY;
                g_BattleSummonFatChocoboCameraState->posZ += g_BattleSummonFatChocoboCameraState[1].mode;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                g_BattleSummonFatChocoboCameraState->deltaX += g_BattleSummonFatChocoboCameraState->stepX;
                g_BattleSummonFatChocoboCameraState->deltaY += g_BattleSummonFatChocoboCameraState->stepY;
                g_BattleSummonFatChocoboCameraState->deltaZ += g_BattleSummonFatChocoboCameraState[1].mode;
                g_BattleSummonFatChocoboCameraState->progress += g_BattleSummonFatChocoboCameraState->progressStep;
                pVector->offsetX = g_BattleSummonFatChocoboCameraScratch->posX - g_BattleSummonFatChocoboCameraState->deltaX;
                pVector->offsetY = g_BattleSummonFatChocoboCameraScratch->posY - g_BattleSummonFatChocoboCameraState->deltaY;
                pVector->offsetZ = g_BattleSummonFatChocoboCameraScratch->posZ - g_BattleSummonFatChocoboCameraState->deltaZ;
                VectorNormalToSVector((int *)pVector, (short *)delta);
                g_BattleSummonFatChocoboCameraState->posX = g_BattleSummonFatChocoboCameraScratch->posX - (delta->vx * g_BattleSummonFatChocoboCameraState->progress >> 0xC);
                g_BattleSummonFatChocoboCameraState->posY = g_BattleSummonFatChocoboCameraScratch->posY - (delta->vy * g_BattleSummonFatChocoboCameraState->progress >> 0xC);
                g_BattleSummonFatChocoboCameraState->posZ = g_BattleSummonFatChocoboCameraScratch->posZ - (delta->vz * g_BattleSummonFatChocoboCameraState->progress >> 0xC);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_BattleSummonFatChocoboCameraState->progress += g_BattleSummonFatChocoboCameraState->progressStep;
                eased = g_BattleSummonFatChocoboCameraState->progress;
                eased = SinFixed(SinFixed(eased / 4) / 4);
                g_BattleSummonFatChocoboCameraState->posX = g_BattleSummonFatChocoboCameraState->deltaX + (g_BattleSummonFatChocoboCameraState->stepX * eased >> 0xC);
                g_BattleSummonFatChocoboCameraState->posY = g_BattleSummonFatChocoboCameraState->deltaY + (g_BattleSummonFatChocoboCameraState->stepY * eased >> 0xC);
                g_BattleSummonFatChocoboCameraState->posZ = g_BattleSummonFatChocoboCameraState->deltaZ + (g_BattleSummonFatChocoboCameraState[1].mode * eased >> 0xC);
            }
            g_BattleSummonFatChocoboCameraState->frameCount--;
            g_BattleSummonFatChocoboCameraState = (struct BattleFxFatChocoboCamera *)&g_BattleSummonFatChocoboTargetActor.state;
            g_BattleSummonFatChocoboCameraScratch = (struct BattleFxCameraBasisScratch *)&g_BattleSummonFatChocoboActor.state;
        }
    }
    if (taskSlot->matrix != 0) {
        if (g_BattleSummonFatChocoboActor.state != SUMMON_CAMERA_HOLD_OUTPUT) {
            SetRotMatrix((void *)taskSlot->matrix);
            SetTransMatrix((void *)taskSlot->matrix);
            if (g_BattleSummonFatChocoboActor.boneIndex == -1) {
                RotTransPoint(&g_BattleSummonFatChocoboActor.vector, (int *)pVector, &flag);
                g_BattleCameraPosition.cameraPosX = *(short *)&pVector->offsetX;
                g_BattleCameraPosition.cameraPosY = *(short *)&pVector->offsetY;
                g_BattleCameraPosZ = *(short *)&pVector->offsetZ;
            } else {
                RotateSVectorByInstalledMatrix(&g_BattleSummonFatChocoboActor.vector, pVector);
                if (g_BattleSummonFatChocoboActor.boneSubIndex != -1) {
                    Battle_GetBoneWorldPos(g_BattleSummonFatChocoboActor.boneIndex, g_BattleSummonFatChocoboActor.boneSubIndex, (short *)delta);
                } else {
                    delta->vx = g_BattleActorRecords[g_BattleSummonFatChocoboActor.boneIndex].posX;
                    delta->vy = g_BattleActorRecords[g_BattleSummonFatChocoboActor.boneIndex].posY;
                    delta->vz = g_BattleActorRecords[g_BattleSummonFatChocoboActor.boneIndex].posZ;
                }
                g_BattleCameraPosition.cameraPosX = pVector->offsetX + delta->vx;
                g_BattleCameraPosition.cameraPosY = pVector->offsetY + delta->vy;
                g_BattleCameraPosZ = pVector->offsetZ + delta->vz;
                SetRotMatrix((void *)taskSlot->matrix);
                SetTransMatrix((void *)taskSlot->matrix);
            }
        }
        if (g_BattleSummonFatChocoboTargetActor.state != SUMMON_CAMERA_HOLD_OUTPUT) {
            if (g_BattleSummonFatChocoboTargetActor.boneIndex == -1) {
                RotTransPoint(&g_BattleSummonFatChocoboTargetActor.vector, (int *)pVector, &flag);
                g_BattleCameraFocalPoint.focalPointX = *(short *)&pVector->offsetX;
                g_BattleCameraFocalPoint.focalPointY = *(short *)&pVector->offsetY;
                g_BattleCameraFocusZ = *(short *)&pVector->offsetZ;
            } else {
                RotateSVectorByInstalledMatrix(&g_BattleSummonFatChocoboTargetActor.vector, pVector);
                if (g_BattleSummonFatChocoboTargetActor.boneSubIndex != -1) {
                    Battle_GetBoneWorldPos(g_BattleSummonFatChocoboTargetActor.boneIndex, g_BattleSummonFatChocoboTargetActor.boneSubIndex, (short *)delta);
                } else {
                    delta->vx = g_BattleActorRecords[g_BattleSummonFatChocoboTargetActor.boneIndex].posX;
                    delta->vy = g_BattleActorRecords[g_BattleSummonFatChocoboTargetActor.boneIndex].posY;
                    delta->vz = g_BattleActorRecords[g_BattleSummonFatChocoboTargetActor.boneIndex].posZ;
                }
                g_BattleCameraFocalPoint.focalPointX = pVector->offsetX + delta->vx;
                g_BattleCameraFocalPoint.focalPointY = pVector->offsetY + delta->vy;
                g_BattleCameraFocusZ = pVector->offsetZ + delta->vz;
            }
        }
    } else {
        if (g_BattleSummonFatChocoboActor.state != SUMMON_CAMERA_HOLD_OUTPUT) {
            *(int *)&g_BattleCameraPosition = g_BattleSummonFatChocoboActor.vector;
            *(int *)&g_BattleCameraPosZ = g_BattleSummonFatChocoboActor.cameraPosZ;
        }
        if (g_BattleSummonFatChocoboTargetActor.state != SUMMON_CAMERA_HOLD_OUTPUT) {
            *(int *)&g_BattleCameraFocalPoint = g_BattleSummonFatChocoboTargetActor.vector;
            *(int *)&g_BattleCameraFocusZ = g_BattleSummonFatChocoboTargetActor.cameraFocusZ;
        }
    }
}

// Reads one short through the script cursor pointer that cursor points to and advances that cursor by two bytes.
// A negative value is used as a negated index into the short table at
// &g_BattleSummonFatChocoboActorBonePos.operandTable and that entry is returned instead. Identical
// body to BattleFx_KjataReadCameraScriptOperand and BattleFx_AlexanderReadCameraScriptOperand,
// which differ only in the table base.
// FUNCTION: FF7 0x00508bb3
short __cdecl BattleFx_FatChocoboReadCameraScriptOperand(void *cursor)
{
    int value;

    value = *(short *)((char *)*(int *)cursor);
    *(int *)cursor += 2;
    if (value < 0) {
        value = (&g_BattleSummonFatChocoboActorBonePos.operandTable)[-value];
    }
    return (short)value;
}

// Timeline task for the Fat Chocobo summon. Every call multiplies the camera transform by the light
// matrix into the summon's transform matrix, before the pause check, so this runs while paused too.
// Otherwise it dispatches on stepIndex: step 1 starts the scaling fade model task and dispatches
// SFX opcode 0x22; step 30 allocates a ring record with lifetime 0xD8F0, transZ from the light
// matrix and angleY from the summon; step 31 starts the offset ramp and screen fill tasks; step 46
// starts the growing model, animated sprite and camera shake tasks; step 76 clears the tint of
// actor record 3 and sets bit 8 in the fade flags of its bones.
// FUNCTION: FF7 0x00508bed
void __cdecl BattleFx_SummonFatChocoboTimelineTask(void)
{
    int frame;
    BattleTaskSlot *task;
    int boneIdx;
    BattleFxRingWaveSlot *pRingRecord;
    int newTask;

    task = BATTLE_CURRENT_TASK_SLOT;
    MultiplyMatrix(&g_BattleCameraTransform, (int *)&g_BattleSummonFatChocoboLightMatrix, &g_BattleSummonFatChocoboTransformMatrix);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame = task->frameCounter;
    if (frame < 1) { /* empty first arm: lib/codegen.h */
    } else {
        frame--;
        if (frame < 0xF) {
            if (frame == 0) {
                newTask = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SummonFatChocoboScalingFadeModelTask);
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 0);
            }
        } else {
            frame -= 0xF;
            if (frame < 0xF) {
                if (frame == 0xE) {
                    pRingRecord = (BattleFxRingWaveSlot *)Battle_AllocSceneMessage(1);
                    pRingRecord->flags = 0;
                    pRingRecord->lifetime = 0xD8F0;
                    pRingRecord->transZ = *(short *)&g_BattleSummonFatChocoboLightMatrix.value;
                    pRingRecord->angleY = g_BattleSummonFatChocoboAngles[1];
                }
            } else {
                frame -= 0xF;
                if (frame < 0xC) {
                    if (frame == 0) {
                        Battle_AllocateResourceSlot(BattleFx_SummonFatChocoboOffsetRampTask);
                        Battle_AllocateResourceSlot(BattleFx_SummonFatChocoboScreenFillTask);
                    }
                } else {
                    frame -= 0xC;
                    if (frame < 0x30) {
                        if (frame < 3) { /* empty first arm: lib/codegen.h */
                        } else {
                            frame -= 3;
                            if (frame < 0x1E) {
                                if (frame == 0) {
                                    newTask = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SummonFatChocoboGrowingModelTask);
                                    newTask = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SummonFatChocoboSpawnAnimatedSpritesTask);
                                    newTask = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SummonFatChocoboCameraShakeTask);
                                }
                            } else {
                                frame -= 0x1E;
                                if (frame < 0xF) {
                                    if (frame == 0) {
                                        g_BattleActorRecords[3].tintB = 0;
                                        g_BattleActorRecords[3].tintG = g_BattleActorRecords[3].tintB;
                                        g_BattleActorRecords[3].tintR = g_BattleActorRecords[3].tintG;
                                        for (boneIdx = 0; boneIdx < g_BattleActorRecords[3].boneCount; boneIdx++) {
                                            (&g_BattleActorRecords[3].fadeFlags)[boneIdx] |= 8;
                                        }
                                    }
                                    g_BattleActorRecords[3].tintPhase = (frame << 0xC) / 0xF;
                                    if (frame == 0xE) {
                                        Battle_AllocSceneMessage(2);
                                    }
                                }
                            }
                        }
                    } else {
                        for (boneIdx = 0; boneIdx < 0xA; boneIdx++) {
                            if ((g_BattleSummonFatChocoboTargetMask & 1 << boneIdx) != 0) {
                                Battle_PostActorHitOrHurtMessage(boneIdx);
                            }
                        }
                        g_BattleStageHideFlags = 0;
                        task->ownerSlot = BATTLE_TASK_FREE;
                        Sfx_ReleaseCachedBuffer(0x1FB, 0xF);
                        Sfx_ReleaseCachedBuffer(0x1FC, 0xF);
                        Sfx_ReleaseCachedBuffer(0x1FD, 0xF);
                    }
                }
            }
        }
    }
    task->frameCounter++;
}

// Scales the Fat Chocobo model up over 0x2D frames, holds it for 0x1E and fades it out over the next
// 0xF, drawing it after the sorted models each frame.
// FUNCTION: FF7 0x00508eb4
void __cdecl BattleFx_SummonFatChocoboScalingFadeModelTask(void)
{
    int frame;
    BattleTaskSlot *task;
    int ratio;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonFatChocoboCameraFlag1 & 1) == 0) {
        g_BattleSummonFatChocoboCameraFlag1 |= 1;
        g_BattleSummonFatChocoboFadeModelMatrix = (void *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_pBattle3dScene != 0) {
        g_BattleSummonFatChocoboEffectDescriptor.modelRequest = g_pBattle3dScene->modelRequest;
    } else {
        g_BattleSummonFatChocoboEffectDescriptor.modelRequest = 0;
    }
    frame = task->frameCounter;
    if (frame < 0x2D) {
        ratio = (frame << 0xC) / 0x2D;
        g_BattleSummonFatChocoboFadeScaleValue[0] = (ratio * 0x6000 >> 0xC) + 0x1000;
        g_BattleSummonFatChocoboFadeScaleVector[0] = g_BattleSummonFatChocoboFadeScaleValue[0];
        g_BattleSummonFatChocoboEffectDescriptor.fadeInverse = 0x1000 - ratio;
    } else {
        frame -= 0x2D;
        if (frame < 0x1E) {
            g_BattleSummonFatChocoboFadeScaleValue[0] = 0x6000;
            g_BattleSummonFatChocoboFadeScaleVector[0] = g_BattleSummonFatChocoboFadeScaleValue[0];
            g_BattleSummonFatChocoboEffectDescriptor.fadeInverse = 0;
        } else {
            frame -= 0x1E;
            if (frame < 0xF) {
                ratio = (frame << 0xC) / 0xF;
                g_BattleSummonFatChocoboFadeScaleValue[0] = 0x6000;
                g_BattleSummonFatChocoboFadeScaleVector[0] = g_BattleSummonFatChocoboFadeScaleValue[0];
                g_BattleSummonFatChocoboEffectDescriptor.fadeInverse = ratio;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    MultiplyMatrix(&g_BattleSummonFatChocoboTransformMatrix, g_BattleSummonFatChocoboFadeScaleVector, g_BattleSummonFatChocoboFadeModelMatrix);
    SetRotMatrix(g_BattleSummonFatChocoboFadeModelMatrix);
    SetTransMatrix(g_BattleSummonFatChocoboFadeModelMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonFatChocoboEffectDescriptor, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_2, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        task->frameCounter++;
    }
}

// Per-frame task that grows the Fat Chocobo bone matrix. Its first call sets bit 1 of the descent
// init flag and stores Gfx_GetPsxScratchpadPtr(0) in the bone position dataBlockPtr. Each call sets
// modelDraw from Gfx_ModelTableGetRenderObject for the target actor index. Below frameCount 0xA
// growFraction is zero; from 0xA to 0xE it ramps up over five frames; after that stage is set to
// 0xFFFF and the task returns. The loop around SinFixed runs zero times, so the linear value
// (frameCount << 12) / 0xF is used unshaped in (value * 0x5000 >> 12) + 0x1000, which is
// transformed into the bone matrix. The model is drawn every call, and frameCount advances only
// while the game is not paused.
// FUNCTION: FF7 0x0050907e
void __cdecl BattleFx_SummonFatChocoboGrowingModelTask(void)
{
    BattleFxPhaseControllerTask *pSlot;
    int frame;
    int shapeIndex, scaleBase;

    pSlot = (BattleFxPhaseControllerTask *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonFatChocoboDescentInitFlag & 1) == 0) {
        g_BattleSummonFatChocoboDescentInitFlag |= 1;
        g_BattleSummonFatChocoboActorBonePos.dataBlockPtr = Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonFatChocoboEffectDescriptor.modelDraw = Gfx_ModelTableGetRenderObject(0, g_BattleSummonFatChocoboModelTable);
    frame = pSlot->frameCount;
    if (frame < 0xA) {
        g_BattleSummonFatChocoboEffectDescriptor.growFraction = 0;
    } else {
        frame -= 0xA;
        if (frame < 5) {
            g_BattleSummonFatChocoboEffectDescriptor.growFraction = (frame << 0xC) / 5;
        } else {
            pSlot->stage = 0xFFFF;
            return;
        }
    }
    scaleBase = (pSlot->frameCount << 0xC) / 0xF;
    for (shapeIndex = 0; shapeIndex < 0; shapeIndex++) {
        scaleBase = SinFixed(scaleBase / 4);
    }
    g_BattleSummonFatChocoboGrowScaleValue[0] = (scaleBase * 0x5000 >> 0xC) + 0x1000;
    g_BattleSummonFatChocoboGrowScaleCopy[0] = g_BattleSummonFatChocoboGrowScaleValue[0];
    g_BattleSummonFatChocoboGrowScaleVector[0] = g_BattleSummonFatChocoboGrowScaleCopy[0];
    MultiplyMatrix(&g_BattleSummonFatChocoboTransformMatrix, g_BattleSummonFatChocoboGrowScaleVector, (void *)(g_BattleSummonFatChocoboActorBonePos.dataBlockPtr));
    SetRotMatrix((void *)(g_BattleSummonFatChocoboActorBonePos.dataBlockPtr));
    SetTransMatrix((void *)(g_BattleSummonFatChocoboActorBonePos.dataBlockPtr));
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonFatChocoboEffectDescriptor.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        pSlot->frameCount++;
    }
}

// Spawns the Fat Chocobo animated sprites. Unless the game is paused, each call allocates fifteen
// animated sprite tasks and then advances the timer. Each sprite takes a random angle p from rand()
// & 0xFFF and a random horizontal speed r from rand() % 0xC8 + 0x64. Its position lies on a circle
// of radius 0xDAC at angle p, its velocityX and velocityZ are r times the sine and cosine of p, and
// velocityY is the negated rand() % 0x64 + 0x32. scale gets rand() % 0x4000 + 0x4000. The branches
// on 0 != 0 are never taken. Once the timer reaches six, the task sets ownerSlot to 0xFFFF and
// retires.
// FUNCTION: FF7 0x0050921e
void __cdecl BattleFx_SummonFatChocoboSpawnAnimatedSpritesTask(void)
{
    BattleTaskSlot *pCurrent;
    int heading;
    int speed, spriteIndex;
    BattleFxWeaponEmeraldBeamSprite *sprite;
    int pick, liftPick, bytesPick;

    pCurrent = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (spriteIndex = 0; spriteIndex < 0xF; spriteIndex++) {
        sprite = (BattleFxWeaponEmeraldBeamSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonFatChocoboAnimatedSpriteTask);
        heading = rand() & 0xFFF;
        if (DEAD_IN_RETAIL != 0) {
            pick = 0x12C;
        } else {
            pick = rand() % 0xC8 + 0x64;
        }
        speed = pick;
        sprite->posX = SinFixed(heading) * 0xDAC >> 0xC;
        sprite->posY = 0;
        sprite->posZ = CosFixed(heading) * 0xDAC >> 0xC;
        sprite->velocityX = speed * SinFixed(heading) >> 0xC;
        if (DEAD_IN_RETAIL != 0) {
            liftPick = 0x96;
        } else {
            liftPick = rand() % 0x64 + 0x32;
        }
        sprite->velocityY = -liftPick;
        sprite->velocityZ = speed * CosFixed(heading) >> 0xC;
        if (DEAD_IN_RETAIL != 0) {
            bytesPick = 0x8000;
        } else {
            bytesPick = rand() % 0x4000 + 0x4000;
        }
        sprite->scale = bytesPick;
    }
    pCurrent->frameCounter++;
    if (pCurrent->frameCounter >= 6) {
        pCurrent->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame task for one Fat Chocobo animated sprite. It installs the sprite matrix from slot+4
// with spriteIndex and sets the sprite request and value from the table entries for step. The
// sprite is drawn with mode 0xC on every call. Unless the game is paused, step advances, and at
// 0x10 the task sets ownerSlot to 0xFFFF and retires. Below step 0x10 posA, posB and posC move by
// velocityA, velocityB and velocityC, and the velocities are damped in 12-bit fixed point by 0xF00,
// 0x1100 and 0xF00, so x and z shrink about six percent per frame while y grows about six percent.
// FUNCTION: FF7 0x005093be
void __cdecl BattleFx_SummonFatChocoboAnimatedSpriteTask(void)
{
    BattleFxSummonFatChocobo *task;

    task = (BattleFxSummonFatChocobo *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_SummonFatChocoboInstallSpriteMatrix(&task->posA, (short)task->spriteIndex, 0);
    g_BattleSummonFatChocoboSpriteRequest.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(g_BattleSummonFatChocoboSpriteConfig[task->step], g_BattleSummonFatChocoboModelTable);
    g_BattleSummonFatChocoboSpriteRequest.drawObj = Gfx_ModelTableGetSpriteObject(g_BattleSummonFatChocoboSpriteConfig[task->step], g_BattleSummonFatChocoboModelTable);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonFatChocoboSpriteRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        task->step++;
        if (task->step >= 0x10) {
            task->ownerSlot = BATTLE_TASK_FREE;
        } else {
            task->posA += task->velocityA;
            task->posB += task->velocityB;
            task->posC += task->velocityC;
            task->velocityA = task->velocityA * 0xF00 >> 0xC;
            task->velocityB = task->velocityB * 0x1100 >> 0xC;
            task->velocityC = task->velocityC * 0xF00 >> 0xC;
        }
    }
}

// Sets the billboard matrix's diagonal to scale, projects vector through
// g_BattleSummonFatChocoboTransformMatrix into the billboard position (vecX, vecY, vecZ) and, when
// offset is nonzero, moves that position along its normalized direction by offset (12-bit fixed
// point). Installs the billboard matrix as the rotation/translation matrix and returns it.
// FUNCTION: FF7 0x0050951a
int __cdecl BattleFx_SummonFatChocoboInstallSpriteMatrix(void *vector, short scale, int offset)
{
    int flags;
    VECTOR normal;

    g_BattleSummonFatChocoboBillboardMatrix.scaleZ = scale;
    g_BattleSummonFatChocoboBillboardMatrix.scaleY = g_BattleSummonFatChocoboBillboardMatrix.scaleZ;
    g_BattleSummonFatChocoboBillboardMatrix.scaleX = g_BattleSummonFatChocoboBillboardMatrix.scaleY;
    SetRotMatrix(&g_BattleSummonFatChocoboTransformMatrix);
    SetTransMatrix(&g_BattleSummonFatChocoboTransformMatrix);
    RotTransPoint(vector, (int *)&g_BattleSummonFatChocoboBillboardMatrix.vecX, &flags);
    if (offset != 0) {
        NormalizeVectorFixed((int *)&g_BattleSummonFatChocoboBillboardMatrix.vecX, &normal);
        g_BattleSummonFatChocoboBillboardMatrix.vecX += normal.vx * offset >> 0xC;
        g_BattleSummonFatChocoboBillboardMatrix.vecY += normal.vy * offset >> 0xC;
        g_BattleSummonFatChocoboBillboardMatrix.vecZ += normal.vz * offset >> 0xC;
    }
    SetRotMatrix((short *)&g_BattleSummonFatChocoboBillboardMatrix);
    SetTransMatrix((short *)&g_BattleSummonFatChocoboBillboardMatrix);
    return (int)&g_BattleSummonFatChocoboBillboardMatrix;
}

// When unpaused subtracts 500 minus counter times 500 divided by 30 from two camera Y words on
// even counter values, advances the current-slot counter, and retires the slot at an updated
// counter of 30 or above.
// FUNCTION: FF7 0x005095f5
void __cdecl BattleFx_SummonFatChocoboCameraShakeTask(void)
{
    BattleTaskSlot *taskPtr;
    int shake;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if ((taskPtr->frameCounter & 1) == 0) {
        shake = taskPtr->frameCounter * -500 / 0x1E + 0x1F4;
        g_BattleCameraFocalPoint.focalPointY -= shake;
        g_BattleCameraPosition.cameraPosY -= shake;
    }
    taskPtr->frameCounter++;
    if (taskPtr->frameCounter >= 0x1E) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Lifts actor 3 for fifteen frames: each frame subtracts 0xFD66 from posY, which in sixteen bits is
// an increment of 0x29A. Frees the task after its frameCounter reaches 0xF; does nothing while
// g_BattleIsPaused is nonzero.
// FUNCTION: FF7 0x00509692
void __cdecl BattleFx_SummonFatChocoboOffsetRampTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleActorRecords[3].posY -= 0xFD66;
    task->frameCounter++;
    if (task->frameCounter >= 0xF) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame task that fills the battle viewport with black over the scene. Each call draws a
// primitive from the viewport origin, with the viewport width and g_BattleSceneViewportHeight as the far corner,
// using the blend factors 0x3F795810 and 0x3F800000. Unless the game is paused, frameCounter
// advances, and at 0xC the task frees its slot.
// FUNCTION: FF7 0x005096f3
void __cdecl BattleFx_SummonFatChocoboScreenFillTask(void)
{
    BattleTaskSlot *p;
    struct {
        int tag;
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char code;
        short x0;
        short y0;
        short x1;
        short y1;
    } prim;

    p = BATTLE_CURRENT_TASK_SLOT;
    prim.tag = 0x03000000;
    prim.r = 0;
    prim.g = 0;
    prim.b = 0;
    prim.code = PSX_PRIM_TILE;
    prim.x0 = 0;
    prim.y0 = 0;
    prim.x1 = 0x280;
    prim.y1 = 0x14C;
    prim.x0 = *(short *)&g_BattleViewportOriginX;
    prim.y0 = *(short *)&g_BattleViewportOriginY;
    prim.x1 = *(short *)&g_BattleViewportWidth;
    prim.y1 = *(short *)&g_BattleSceneViewportHeight;
    Gfx_SetBlendFactorPair(FLOAT_0_974_BITS, FLOAT_ONE_BITS);
    Gfx_DrawPsxPrimitive(&prim);
    if (g_BattleIsPaused == 0) {
        p->frameCounter++;
        if (p->frameCounter >= 0xC) {
            p->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
