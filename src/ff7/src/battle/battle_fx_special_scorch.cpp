// src/ff7/src/battle/battle_fx_special_scorch.cpp
/* Enemy Special Attack [109]: Scorch / Scorching Heat (orig. SPECIAL/SYAKU, 灼熱) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect Syaku: loads its resources, runs a timeline that spawns model
//   callbacks and sounds, and runs the screen flash, model, sprite and trail tasks that draw it.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle_special.h"
extern "C" {
// clang-format off
#define g_BattleSpecialScorchResHadoHandle (((BattleFxResourceSlot12 *)&g_BattleSpecialScorchResFilenameHado)->marker)
#define g_BattleSpecialScorchResTamaHandle (((BattleFxResourceSlot12 *)&g_BattleSpecialScorchResFilenameTama)->marker)
#define g_BattleSpecialScorchResMotoHandle (((BattleFxResourceSlot12 *)&g_BattleSpecialScorchResFilenameMoto)->marker)
#define g_BattleSpecialScorchResSenkouHandle (((BattleFxResourceSlot12 *)&g_BattleSpecialScorchResFilenameSenkou)->marker)
#define g_BattleSpecialScorchResIsi1Handle (((BattleFxResourceSlot12 *)&g_BattleSpecialScorchResFilenameIsi1)->marker)
#define g_BattleSpecialScorchResIsi2Handle (((BattleFxResourceSlot12 *)&g_BattleSpecialScorchResFilenameIsi2)->marker)
#define g_BattleSpecialScorchResShak1Handle (((BattleFxResourceSlot12 *)&g_BattleSpecialScorchResFilenameShak1)->marker)
#define g_BattleSpecialScorchResShak2Handle (((BattleFxResourceSlot12 *)&g_BattleSpecialScorchResFilenameShak2)->marker)
#define g_BattleSpecialScorchResBakuHandle (((BattleFxResourceSlot12 *)&g_BattleSpecialScorchResFilenameBaku)->marker)
// clang-format off
// GLOBAL: FF7 0x00830078
unsigned int g_BattleSpecialScorchResourceDefs[73] = {0x001900b1, (unsigned int)&g_BattleSpecialScorchResFilenameHado, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialScorchResHadoHandle, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialScorchResFilenameTama, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialScorchResTamaHandle, 0x00000000, 0x00000000, 0x00000000, 0x003200b1, (unsigned int)&g_BattleSpecialScorchResFilenameMoto, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialScorchResMotoHandle, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialScorchResFilenameSenkou, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialScorchResSenkouHandle, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialScorchResFilenameIsi1, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialScorchResIsi1Handle, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialScorchResFilenameIsi2, 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialScorchResIsi2Handle, (unsigned int)&g_BattleSpecialScorchResFilenameKou, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialScorchResFilenameShak1, (unsigned int)&g_BattleSpecialScorchResShak1Handle, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialScorchResFilenameShak2, (unsigned int)&g_BattleSpecialScorchResShak2Handle, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialScorchResFilenameBaku, (unsigned int)&g_BattleSpecialScorchResBakuHandle};
// GLOBAL: FF7 0x008301a0
struct BattleSpecialScorchBasisMatrix g_BattleSpecialScorchBasisMatrix = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x40, 0x40, 0x40, 0x2e, 0, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x008301c0
BattleSpecialScorchColor1 g_BattleSpecialScorchColor1 = {(char)0x80, (char)0x80, (char)0x80, 0x2c, 0, {0, 0}};
// GLOBAL: FF7 0x008301c8
int g_BattleSpecialScorchScaleTama[4] = {0x3000, 0x3000, 0x3000, 0};
// GLOBAL: FF7 0x008301d8
BattleSpecialScorchVector1 g_BattleSpecialScorchVector1 = {0, 0, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x008301e8
int g_BattleSpecialScorchModelConfig[4] = {7, 8, 9, 10};
// GLOBAL: FF7 0x008301f8
struct GfxRotationAngles2 g_BattleSpecialScorchRotationTama = {-1024, 0, 0};
// GLOBAL: FF7 0x00830200
BattleSpecialScorchModelReqIsi g_BattleSpecialScorchModelReqTama = {0, 0xa8, 0, 0};
unsigned char g_rawData_0083020c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00830210
struct GfxRotationAngles2 g_BattleSpecialScorchRotationMoto = {-1024, 0, 0};
// GLOBAL: FF7 0x00830218
struct BattleSpecialScorchParamVector1 g_BattleSpecialScorchParamVector1 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x00830228
struct BattleSpecialTriangleAttackSpriteReq2 g_BattleSpecialScorchParamVector2 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// the sprite draw record the code fills and submits by address
// GLOBAL: FF7 0x00830238
BattleFxSpriteRequest g_BattleSpecialScorchModelReqMoto = {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2e, 0x80, 0};
// GLOBAL: FF7 0x00830248
BattleSpecialScorchModelReqIsi g_BattleSpecialScorchModelReqIsi = {0, 0xa8, 0, 0x80};
unsigned char g_rawData_00830254[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00830258
struct GfxRotationAngles2 g_BattleSpecialScorchRotationIsi = {-1024, 0, 0};
// GLOBAL: FF7 0x00830260
int g_BattleSpecialScorchScaleTrajectory[4] = {0x3000, 0x3000, 0x3000, 0};
// GLOBAL: FF7 0x00830270
BattleSpecialScorchVectorTrajectory g_BattleSpecialScorchVectorTrajectory = {0, 0x28, 0, 0x80, 0x20, 0, 0, 0};
// GLOBAL: FF7 0x00830288
BattleSpecialScorchColor1 g_BattleSpecialScorchColor2 = {(char)0x80, (char)0x80, (char)0x80, 0x2e, 0, {0, 0}};
// GLOBAL: FF7 0x00830290
struct GfxRotationAngles2 g_BattleSpecialScorchRotationTrajectory = {-1024, 0, 0};
// GLOBAL: FF7 0x00830298
unsigned char g_BattleSpecialScorchResFilenameHado[22] =
    "HADO.RSD\0\0\0\0"
    "HADO1.RSD";
// GLOBAL: FF7 0x008302b0
unsigned char g_BattleSpecialScorchResFilenameTama[22] =
    "TAMA1.RSD\0\0\0"
    "TAMA2.RSD";
// GLOBAL: FF7 0x008302c8
unsigned char g_BattleSpecialScorchResFilenameMoto[21] =
    "MOTO.RSD\0\0\0\0"
    "SOTO.RSD";
// GLOBAL: FF7 0x008302e0
unsigned char g_BattleSpecialScorchResFilenameSenkou[21] =
    "SENKOU1.RSD\0"
    "ISI1.RSD";
// GLOBAL: FF7 0x008302f8
unsigned char g_BattleSpecialScorchResFilenameIsi1[21] =
    "ISI2.RSD\0\0\0\0"
    "ISI3.RSD";
// GLOBAL: FF7 0x00830310
unsigned char g_BattleSpecialScorchResFilenameIsi2[24] =
    "ISI4.RSD\0\0\0\0"
    "SHAKU00.TIM";
// GLOBAL: FF7 0x00830328
unsigned char g_BattleSpecialScorchResFilenameKou[8] = "KOU.SPT";
// GLOBAL: FF7 0x00830330
unsigned char g_BattleSpecialScorchResFilenameShak1[23] =
    "SHAKU03.TIM\0"
    "KEMU11.SPT";
// GLOBAL: FF7 0x00830348
unsigned char g_BattleSpecialScorchResFilenameShak2[23] =
    "SHAKU00.TIM\0"
    "HAHEN1.SPT";
// GLOBAL: FF7 0x00830360
unsigned char g_BattleSpecialScorchResFilenameBaku[22] =
    "BAKU1.TIM\0\0\0"
    "BAKU1.SPT";
// GLOBAL: FF7 0x00b44ed0
PsxMatrix g_BattleSpecialScorchDrawListHandle = {0};
// GLOBAL: FF7 0x00b44ef0
struct BattleSpecialScorchCentroidPos g_BattleSpecialScorchCentroidPos = {0};
// GLOBAL: FF7 0x00b44ef4
short g_BattleSpecialScorchCentroidZ = 0;
// GLOBAL: FF7 0x00b44ef8
int g_BattleSpecialScorchLoadedResTable = 0;
// GLOBAL: FF7 0x00b44efc
short g_BattleSpecialScorchFlashIntensity = 0;
// GLOBAL: FF7 0x00b44f00
short g_BattleSpecialScorchRandomAngle = 0;
// GLOBAL: FF7 0x00b44f04
short g_BattleSpecialScorchTargetMask = 0;
// GLOBAL: FF7 0x00b44f08
short g_BattleSpecialScorchScaleCounter = 0;
// GLOBAL: FF7 0x00b44f0c
short g_BattleSpecialScorchLifetimeCounter = 0;
// GLOBAL: FF7 0x00b44f10
struct GfxTableCursor *g_BattleSpecialScorchModelToggle = 0;
unsigned char g_bssPad_00b44f14[4] = {0};
// clang-format on
#pragma function(memcpy)
#pragma data_seg(".data$u4c_orbit")
#pragma bss_seg(".data$u4c_orbit")
#pragma data_seg(".bss$u4c_orbit_slot")
#pragma bss_seg(".bss$u4c_orbit_slot")
#pragma data_seg(".bss$u4c_orbit_flag")
#pragma bss_seg(".bss$u4c_orbit_flag")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(memset)
#pragma function(memset)

// If the shared renderer exists and the cached effect resource is absent, initializes a
// duration/options record, builds the SPECIAL/SYAKU resource path and creates resource selector
// fifteen using the retained descriptor and renderer field. Existing resources are untouched. The
// options record is rooted at separate scalar requestRecord and the path helper receives scalar
// pathBuffer.
// FUNCTION: FF7 0x004dceb0
void __cdecl BattleFx_SpecialLoadResScorch(void)
{
    GfxModelTableBank *rendererField;
    GfxResourceRequest requestRecord;
    int typeId;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int resDescList;

    if (g_pBattle3dScene != 0 && g_BattleSpecialScorchModelToggle == 0) {
        typeId = 0xF;
        resDescList = (int)&g_BattleSpecialScorchResourceDefs;
        rendererField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x00830378
        request->resourcePath = BattleFx_BuildResourcePath("SPECIAL/SYAKU", pathBuffer);
        Gfx_LoadModelTableIfAbsent(typeId, resDescList, request, &g_BattleSpecialScorchModelToggle, rendererField);
    }
}

/*
 * Setup wrapper: runs BattleFx_SpecialLoadResScorch, then sets g_BattleSpecialScorchRandomAngle to 0 and both g_BattleSpecialScorchLifetimeCounter and
 * g_BattleSpecialScorchScaleCounter to 8, and forwards targetMask and actorIdx to BattleFx_SpecialBeginScorch.
 */
// FUNCTION: FF7 0x004dcf40
void __cdecl BattleFx_SpecialEnterScorch(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResScorch();
    g_BattleSpecialScorchRandomAngle = 0;
    g_BattleSpecialScorchLifetimeCounter = 8;
    g_BattleSpecialScorchScaleCounter = 8;
    BattleFx_SpecialBeginScorch(targetMask, actorIdx);
}

/*
 * Stores the target mask, creates BattleFx_SpecialScorchTimelineTask at the source actor bone,
 * obtains the target-mask bone centroid, builds its shared look basis and stores normalized
 * velocity scaled by six hundred toward that position. Records the source actor, sets shared flash
 * color black and starts a sixteen-step ramp.
 */
// FUNCTION: FF7 0x004dcf75
void __cdecl BattleFx_SpecialBeginScorch(int targetMask, int actorIdx)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[28];
        VECTOR dirX;
        struct { unsigned char pad_to_10[0x10]; BattleFxScorchParticle *particle; };
        struct { unsigned char pad_to_14[0x14]; SVECTOR toCentroidX; };
    } frame;
#pragma pack(pop)

    g_BattleSpecialScorchFlashIntensity = targetMask;
    frame.particle = (BattleFxScorchParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialScorchTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone33, (short *)((int)frame.particle + 4));
    Battle_GetTargetMaskBoneCentroid((short)targetMask, (Vec3sPacked *)&g_BattleSpecialScorchCentroidPos);
    frame.toCentroidX.vx = (g_BattleSpecialScorchCentroidPos.centroidX) - frame.particle->posX;
    frame.dirX.vx = frame.toCentroidX.vx;
    frame.toCentroidX.vy = g_BattleSpecialScorchCentroidPos.y - frame.particle->posY;
    frame.dirX.vy = frame.toCentroidX.vy;
    frame.toCentroidX.vz = g_BattleSpecialScorchCentroidZ - frame.particle->posZ;
    frame.dirX.vz = frame.toCentroidX.vz;
    Gte_BuildLookBasisFromDirection(&frame.toCentroidX.vx, (short *)&g_BattleSpecialScorchDrawListHandle);
    NormalizeVectorFixed(&frame.dirX, &frame.dirX);
    frame.particle->velocityX = frame.dirX.vx * 0x258 >> 0xC;
    frame.particle->velocityY = frame.dirX.vy * 0x258 >> 0xC;
    frame.particle->velocityZ = frame.dirX.vz * 0x258 >> 0xC;
    frame.particle->actorIdx = actorIdx;
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0x1000, 0x10);
}

/*
 * When unpaused at zero, creates two model callbacks at the task position and dispatches opcode
 * 0x174. At twenty creates two more callbacks, one inheriting task velocity, starts another
 * callback and dispatches opcode 0x176. During counts 37 through 43 emits rotated-velocity
 * secondary callbacks for target-mask actors; creates their typed records and dispatches opcode
 * 0xFA at forty. Starts a fade at 54, alternates a shared short after twenty, advances and retires
 * at seventy.
 */
// FUNCTION: FF7 0x004dd0bd
void __cdecl BattleFx_SpecialScorchTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int depth; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxSpecialEnergyOrbSlot *effectSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short actorIdx; };
        struct { unsigned char pad_to_44[0x44]; BattleFxScorchTimeline *timeline; };
    } frame;
#pragma pack(pop)

    frame.timeline = (BattleFxScorchTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timeline->timer == 0) {
        frame.effectSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialScorchGrowingModelTask);
        frame.effectSlot->posX = frame.timeline->posX;
        frame.effectSlot->posY = frame.timeline->posY;
        frame.effectSlot->posZ = frame.timeline->posZ;
        frame.effectSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialScorchPulsingSpriteTask);
        frame.effectSlot->posX = frame.timeline->posX;
        frame.effectSlot->posY = frame.timeline->posY;
        frame.effectSlot->posZ = frame.timeline->posZ;
        Battle_PlaySfxPannedToActor(0x174, (short)frame.timeline->level, 0x40);
    }
    if (frame.timeline->timer == 0x14) {
        frame.effectSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialScorchStretchingModelsTask);
        frame.effectSlot->posX = frame.timeline->posX;
        frame.effectSlot->posY = frame.timeline->posY;
        frame.effectSlot->posZ = frame.timeline->posZ;
        frame.effectSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialScorchProjectileTask);
        frame.effectSlot->posX = frame.timeline->posX;
        frame.effectSlot->posY = frame.timeline->posY;
        frame.effectSlot->posZ = frame.timeline->posZ;
        frame.effectSlot->velocityX = frame.timeline->velocityX;
        frame.effectSlot->velocityY = frame.timeline->velocityY;
        frame.effectSlot->velocityZ = frame.timeline->velocityZ;
        Battle_PlaySfxPannedToActor(0x176, (short)frame.timeline->level, 0x40);
    }
    if (frame.timeline->timer == 0x14) {
        Battle_AllocateResourceSlot(BattleFx_SpecialScorchScreenFlashTask);
    }
    if (frame.timeline->timer == 0x36) {
        Battle_StartFixedPointRamp(0, 0x10);
    }
    if (frame.timeline->timer > 0x24 && frame.timeline->timer < 0x2C) {
        for (frame.actorIdx = 0; frame.actorIdx < 0xA; frame.actorIdx++) {
            if ((g_BattleSpecialScorchFlashIntensity & 1 << frame.actorIdx) != 0) {
                frame.effectSlot = (BattleFxSpecialEnergyOrbSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialScorchDriftingAnimatedSpriteTask)];
                Battle_GetBoneWorldPos(frame.actorIdx, g_BattleActorRecords[frame.actorIdx].targetBone, (short *)((int)frame.effectSlot + 4));
                frame.offsetX.vy = 0x50;
                frame.offsetX.vz = 0;
                frame.offsetX.vx = frame.offsetX.vz;
                frame.rotX.vx = (rand() % 4096);
                frame.rotX.vz = (rand() % 4096);
                frame.rotX.vy = 0;
                RotMatrix(&frame.rotX, &frame.rotMatrix);
                frame.rotMatrix.t[2] = 0;
                frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
                frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
                SetRotMatrix(&frame.rotMatrix);
                SetTransMatrix(&frame.rotMatrix);
                RotTransPointToInt16(&frame.offsetX, (short *)((int)frame.effectSlot + 0xC), &frame.depth);
            }
        }
    }
    if (frame.timeline->timer == 0x28) {
        for (frame.actorIdx = 0; frame.actorIdx < 0xA; frame.actorIdx++) {
            if ((g_BattleSpecialScorchFlashIntensity & 1 << frame.actorIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.actorIdx);
            }
        }
        Battle_PlaySfxPannedToActor(0xFA, -1, 0x40);
    }
    if (frame.timeline->timer > 0x14) {
        g_BattleCameraFocalPoint.focalPointY += (short)(0x10 - ((frame.timeline->timer % 2) << 5));
    }
    (frame.timeline->timer)++;
    if (frame.timeline->timer == 0x46) {
        frame.timeline->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a screen flash with red from g_BattleSpecialScorchRandomAngle and the other channels zero before checking
 * pause. When unpaused increases that shared channel by fifteen below counter sixteen, reduces it
 * by fifteen above counter thirty-four without clamping, advances the counter and retires exactly
 * at fifty.
 */
// FUNCTION: FF7 0x004dd4a6
void __cdecl BattleFx_SpecialScorchScreenFlashTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(*(char *)&g_BattleSpecialScorchRandomAngle, 0, 0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 0x10) {
        g_BattleSpecialScorchRandomAngle += 0xF;
    }
    if (task->frameCounter > 0x22) {
        g_BattleSpecialScorchRandomAngle -= 0xF;
    }
    (task->frameCounter)++;
    if (task->frameCounter == 0x32) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws model entry six at the task position with shared orientation concatenation, uniform scale
 * count times 1500 and a count-shifted descriptor field using black draw color. Rendering
 * continues while paused; unpaused calls increment the counter and retire at twenty.
 */
// FUNCTION: FF7 0x004dd548
void __cdecl BattleFx_SpecialScorchGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxCosmoMemoryTriSpin *task; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.task = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialScorchModelReqTama.modelDraw = Gfx_ModelTableGetRenderObject(6, g_BattleSpecialScorchModelToggle);
    RotMatrix((SVECTOR *)&g_BattleSpecialScorchRotationTama, &frame.modelMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialScorchDrawListHandle, &frame.modelMatrix, &frame.modelMatrix);
    frame.scaleX.vz = frame.task->step * 0x5DC;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleX);
    frame.modelMatrix.t[0] = frame.task->posX;
    frame.modelMatrix.t[1] = frame.task->posY;
    frame.modelMatrix.t[2] = frame.task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialScorchModelReqTama.fadeLevel = frame.task->step << 8;
    g_BattleSpecialScorchLoadedResTable = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialScorchModelReqTama.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialScorchLoadedResTable);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.task->step)++;
    if (frame.task->step == 0x14) {
        frame.task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Same per-frame moving-sprite tick as BattleFx_SpecialSuperOrbDriftingAnimatedSpriteTask on the same
 * slot at &g_BattleTaskSlotBuffer.secondarySlots + (g_BattleSecondaryTaskActiveSlotIdx << 5), with
 * its own descriptor g_BattleSpecialScorchParamVector2 / g_BattleSpecialScorchParamVector2.drawObj /
 * g_BattleSpecialScorchParamVector2.frame, key 0xE on the handle g_BattleSpecialScorchModelToggle,
 * and billboard scale 0x4000. Position at slot+4..+8 is integrated by the velocity at
 * slot+0xC..+0x10 and the slot is killed with 0xFFFF once the counter reaches
 * [[g_BattleSpecialScorchParamVector2]+4]. Position integration requires an unpaused call; counter
 * advancement and frame-count retirement additionally require g_BattleSpecialScorchParamVector2 to
 * be nonnull.
 */
// FUNCTION: FF7 0x004dd68e
void __cdecl BattleFx_SpecialScorchDriftingAnimatedSpriteTask(void)
{
    BattleFxPulseOfLifeSprite *task;

    task = (BattleFxPulseOfLifeSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    (g_BattleSpecialScorchParamVector2.animTable) = Gfx_ModelTableGetLoadedRecord(0xE, g_BattleSpecialScorchModelToggle);
    g_BattleSpecialScorchParamVector2.drawObj = Gfx_ModelTableGetSpriteObject(0xE, g_BattleSpecialScorchModelToggle);
    BattleFx_InstallBillboardMatrix((char *)task + 4, 0x4000, 0);
    g_BattleSpecialScorchParamVector2.frame = task->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialScorchParamVector2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
        if ((g_BattleSpecialScorchParamVector2.animTable) != 0) {
            (task->frame)++;
            if (task->frame >= *(int *)((char *)*(int *)(g_BattleSpecialScorchParamVector2.animTable) + 4)) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

/*
 * Per-frame billboard-sprite handler for the current slot at g_BattleHitLivewireEmitterRecords +
 * g_BattleActiveTaskSlotIdx * 0x20. Loads entry 0xB of the table at g_BattleSpecialScorchModelToggle
 * into the sprite request at g_BattleSpecialScorchModelReqMoto (0x00830238)/0x0083023C with
 * Gfx_ModelTableGetLoadedRecord/Gfx_ModelTableGetSpriteObject, installs a billboard matrix at slot +4 with
 * scale 0x1000 + ((SinFixed(frame * 0x33) << 7 >> 12) << 7), writes frame % 2 to the short at
 * 0x00830244, and submits the request through BattleFx_SubmitSpriteDraw into the OT bucket
 * g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED with the packet cursor kept at
 * g_BattleSpecialScorchLoadedResTable (0x00B44EF8). Unless g_BattleIsPaused is set, advances the
 * frame counter at +2 and retires the slot (0xFFFF at +0) when it reaches 0x28.
 */
// FUNCTION: FF7 0x004dd79e
void __cdecl BattleFx_SpecialScorchPulsingSpriteTask(void)
{
    short pulse;
    BattleEnergyOrbPulsingSprite *sprite;

    sprite = (BattleEnergyOrbPulsingSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialScorchModelReqMoto.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0xB, g_BattleSpecialScorchModelToggle);
    g_BattleSpecialScorchModelReqMoto.drawObj = Gfx_ModelTableGetSpriteObject(0xB, g_BattleSpecialScorchModelToggle);
    pulse = (SinFixed(sprite->frameIndex * 0x33) << 7) >> 0xC;
    BattleFx_InstallBillboardMatrix(&sprite->billboardMatrix, (pulse << 7) + 0x1000, 0);
    g_BattleSpecialScorchModelReqMoto.frame = sprite->frameIndex % 2;
    g_BattleSpecialScorchLoadedResTable = BattleFx_SubmitSpriteDraw(&g_BattleSpecialScorchModelReqMoto, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialScorchLoadedResTable);
    if (g_BattleIsPaused == 0) {
        (sprite->frameIndex)++;
        if (sprite->frameIndex == 0x28) {
            sprite->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws model entries zero and one at the task position with shared orientation, shrinking X/Z
 * scale, growing Y scale and a descriptor field derived from shared intensity. Rendering continues
 * while paused. Unpaused calls lower shared intensity after twelve, increment the counter and
 * retire at twenty.
 */
// FUNCTION: FF7 0x004dd891
void __cdecl BattleFx_SpecialScorchStretchingModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxEnergyOrbTrailModel *task; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.task = (BattleFxEnergyOrbTrailModel *)BATTLE_CURRENT_TASK_SLOT;
    RotMatrix((SVECTOR *)&g_BattleSpecialScorchRotationIsi, &frame.modelMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialScorchDrawListHandle, &frame.modelMatrix, &frame.modelMatrix);
    frame.scaleX.vz = 0x4000 - frame.task->frameCount * 0x12C;
    frame.scaleX.vx = frame.scaleX.vz;
    frame.scaleX.vy = frame.task->frameCount * 0x384;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleX);
    frame.modelMatrix.t[0] = frame.task->posX;
    frame.modelMatrix.t[1] = frame.task->posY;
    frame.modelMatrix.t[2] = frame.task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialScorchModelReqIsi.fadeLevel = 0x1000 - (g_BattleSpecialScorchLifetimeCounter << 8);
    g_BattleSpecialScorchModelReqIsi.modelDraw = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialScorchModelToggle);
    g_BattleSpecialScorchLoadedResTable = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialScorchModelReqIsi.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialScorchLoadedResTable);
    g_BattleSpecialScorchModelReqIsi.modelDraw = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialScorchModelToggle);
    g_BattleSpecialScorchLoadedResTable = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialScorchModelReqIsi.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialScorchLoadedResTable);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.task->frameCount > 0xC) {
        g_BattleSpecialScorchLifetimeCounter--;
    }
    (frame.task->frameCount)++;
    if (frame.task->frameCount == 0x14) {
        frame.task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws model entries two, three and four plus a two-frame sprite at the task position with shared
 * orientation/scale and shared-intensity descriptor/color fields. Unpaused calls move XYZ, emit a
 * trail callback on even counts and, while Y is negative, create a randomized ground callback plus
 * another model on counts divisible by three. Lowers shared intensity after twelve, advances and
 * retires at twenty. Drawing continues while paused.
 */
// FUNCTION: FF7 0x004dda44
void __cdecl BattleFx_SpecialScorchProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        struct { unsigned char pad_to_4[0x4]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_24[0x24]; BattleFxEnergyOrbProjectile *spawnedSlot; };
        struct { unsigned char pad_to_28[0x28]; BattleFxEnergyOrbProjectile *projectile; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.projectile = (BattleFxEnergyOrbProjectile *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialScorchVectorTrajectory.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0xB, g_BattleSpecialScorchModelToggle);
    g_BattleSpecialScorchVectorTrajectory.drawObj = Gfx_ModelTableGetSpriteObject(0xB, g_BattleSpecialScorchModelToggle);
    RotMatrix((SVECTOR *)&g_BattleSpecialScorchRotationTrajectory, &frame.modelMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialScorchDrawListHandle, &frame.modelMatrix, &frame.modelMatrix);
    ScaleMatrix(&frame.modelMatrix, g_BattleSpecialScorchScaleTrajectory);
    frame.modelMatrix.t[0] = frame.projectile->posX;
    frame.modelMatrix.t[1] = frame.projectile->posY;
    frame.modelMatrix.t[2] = frame.projectile->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialScorchVectorTrajectory.fadeLevel = g_BattleSpecialScorchScaleCounter << 4;
    g_BattleSpecialScorchVectorTrajectory.modelDraw = Gfx_ModelTableGetRenderObject(2, g_BattleSpecialScorchModelToggle);
    g_BattleSpecialScorchLoadedResTable = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialScorchVectorTrajectory.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialScorchLoadedResTable);
    g_BattleSpecialScorchVectorTrajectory.modelDraw = Gfx_ModelTableGetRenderObject(3, g_BattleSpecialScorchModelToggle);
    g_BattleSpecialScorchLoadedResTable = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialScorchVectorTrajectory.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialScorchLoadedResTable);
    g_BattleSpecialScorchVectorTrajectory.modelDraw = Gfx_ModelTableGetRenderObject(4, g_BattleSpecialScorchModelToggle);
    g_BattleSpecialScorchLoadedResTable = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialScorchVectorTrajectory.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialScorchLoadedResTable);
    BattleFx_InstallBillboardMatrix((char *)frame.projectile + 4, 0x4000, 0);
    g_BattleSpecialScorchColor2.colorB = g_BattleSpecialScorchScaleCounter * 0x14;
    g_BattleSpecialScorchColor2.colorG = g_BattleSpecialScorchColor2.colorB;
    g_BattleSpecialScorchColor2.colorR = g_BattleSpecialScorchColor2.colorG;
    g_BattleSpecialScorchColor2.frame = (frame.projectile->frameCount % 2);
    g_BattleSpecialScorchLoadedResTable = BattleFx_SubmitSpriteDraw(&g_BattleSpecialScorchVectorTrajectory.spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialScorchLoadedResTable);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.projectile->posX += frame.projectile->velocityX;
    frame.projectile->posY += frame.projectile->velocityY;
    frame.projectile->posZ += frame.projectile->velocityZ;
    if ((frame.projectile->frameCount % 2) == 0) {
        frame.spawnedSlot = (BattleFxEnergyOrbProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialScorchTrailModelTask);
        frame.spawnedSlot->posX = frame.projectile->posX;
        frame.spawnedSlot->posY = frame.projectile->posY;
        frame.spawnedSlot->posZ = frame.projectile->posZ;
    }
    if (frame.projectile->posY < 0) {
        frame.spawnedSlot = (BattleFxEnergyOrbProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialScorchBouncingSpriteTask);
        frame.spawnedSlot->posX = frame.projectile->posX;
        frame.spawnedSlot->posZ = frame.projectile->posZ;
        frame.spawnedSlot->posY = 0;
        g_BattleSpecialScorchTargetMask = (rand() % 4096);
        frame.spawnedSlot->velocityX = CosFixed(g_BattleSpecialScorchTargetMask) * 0x78 >> 0xC;
        frame.spawnedSlot->velocityZ = SinFixed(g_BattleSpecialScorchTargetMask) * 0x78 >> 0xC;
        frame.spawnedSlot->velocityY = -rand() % 0x12C - 0x32;
        frame.spawnedSlot->variant = rand() % 7;
        if (frame.projectile->frameCount % 3 == 0) {
            frame.spawnedSlot = (BattleFxEnergyOrbProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialScorchFallingFadeModelTask);
            frame.spawnedSlot->posX = frame.projectile->posX + rand() % 0xFA0 + -0x7D0;
            frame.spawnedSlot->posZ = frame.projectile->posZ + rand() % 0xFA0 + -0x7D0;
            frame.spawnedSlot->posY = 0;
            frame.spawnedSlot->variant = (rand() % 4);
        }
    }
    if (frame.projectile->frameCount > 0xC) {
        g_BattleSpecialScorchScaleCounter--;
    }
    (frame.projectile->frameCount)++;
    if (frame.projectile->frameCount == 0x14) {
        frame.projectile->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws sprite entry thirteen with task-selected frame, fixed doubled scale, stored Z rotation and
 * gray holding eight before twelve then twenty minus count. Unpaused calls move XYZ, clamp
 * positive Y to zero and reverse Y velocity, add gravity, rotate by 0x200, advance and retire at
 * twenty. Drawing continues while paused.
 */
// FUNCTION: FF7 0x004dde9f
void __cdecl BattleFx_SpecialScorchBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int shade;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short shadeShort; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix spriteMatrix; };
        struct { unsigned char pad_to_50[0x50]; short rowIdx; };
        struct { unsigned char pad_to_54[0x54]; int depth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxScorchBounceSprite *sprite; };
    } frame;
#pragma pack(pop)

    frame.sprite = (BattleFxScorchBounceSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialScorchBasisMatrix.spriteRecord = Gfx_ModelTableGetLoadedRecord(0xD, g_BattleSpecialScorchModelToggle);
    g_BattleSpecialScorchBasisMatrix.spriteObject = Gfx_ModelTableGetSpriteObject(0xD, g_BattleSpecialScorchModelToggle);
    for (frame.rowIdx = 0; frame.rowIdx < 3; frame.rowIdx++) {
        frame.spriteMatrix.m[2][frame.rowIdx] = 0;
        frame.spriteMatrix.m[1][frame.rowIdx] = 0;
        frame.spriteMatrix.m[0][frame.rowIdx] = 0;
    }
    if (frame.sprite->frameCount < 0xC) {
        frame.shade = 8;
    } else {
        frame.shade = 0x14 - frame.sprite->frameCount;
    }
    frame.shadeShort = *(short *)&frame.shade;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.sprite->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((char *)frame.sprite + 4, frame.spriteMatrix.t, &frame.depth);
    frame.spriteMatrix.m[2][2] = 0x2000;
    frame.spriteMatrix.m[1][1] = frame.spriteMatrix.m[2][2];
    frame.spriteMatrix.m[0][0] = frame.spriteMatrix.m[1][1];
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.spriteMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.spriteMatrix);
    SetTransMatrix(&frame.spriteMatrix);
    g_BattleSpecialScorchColor1.frame = frame.sprite->spriteFrame;
    g_BattleSpecialScorchColor1.code = (frame.shadeShort - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecialScorchColor1.colorB = frame.shadeShort << 4;
    g_BattleSpecialScorchColor1.colorG = g_BattleSpecialScorchColor1.colorB;
    g_BattleSpecialScorchColor1.colorR = g_BattleSpecialScorchColor1.colorG;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialScorchBasisMatrix.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.sprite->posX += frame.sprite->velocityX;
    frame.sprite->posY += frame.sprite->velocityY;
    frame.sprite->posZ += frame.sprite->velocityZ;
    if (frame.sprite->posY > 0) {
        frame.sprite->posY = 0;
        frame.sprite->velocityY *= -1;
    }
    frame.sprite->velocityY += 0x14;
    frame.sprite->angle += 0x200;
    (frame.sprite->frameCount)++;
    if (frame.sprite->frameCount == 0x14) {
        frame.sprite->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a task-selector-indexed model at the task position using stored Z rotation, shared scale
 * and gray 128 minus six times count. Unpaused calls lower Y by two hundred, rotate by 0x100,
 * advance and retire at twenty. Drawing continues while paused.
 */
// FUNCTION: FF7 0x004de119
void __cdecl BattleFx_SpecialScorchFallingFadeModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxSpecialEnergyOrbFallingSlot *task; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.task = (BattleFxSpecialEnergyOrbFallingSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.task->angleZ;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, g_BattleSpecialScorchScaleTama);
    frame.rotMatrix.t[0] = frame.task->posX;
    frame.rotMatrix.t[1] = frame.task->posY;
    frame.rotMatrix.t[2] = frame.task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialScorchVector1.fadeLevel = 0x80 - frame.task->frame * 6;
    g_BattleSpecialScorchVector1.modelDraw = Gfx_ModelTableGetRenderObject(g_BattleSpecialScorchModelConfig[frame.task->modelIndex], g_BattleSpecialScorchModelToggle);
    g_BattleSpecialScorchLoadedResTable = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialScorchVector1.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialScorchLoadedResTable);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.task->posY -= 0xC8;
    frame.task->angleZ += 0x100;
    (frame.task->frame)++;
    if (frame.task->frame == 0x14) {
        frame.task->slotTag = 0xFFFF;
    }
}

/*
 * Loads model entry five and, only after count two, draws it at the task position with shared
 * orientation, shrinking uniform scale and a count-times-two-hundred descriptor field. Rendering
 * continues while paused; unpaused calls increment the counter and retire at twenty even when the
 * draw branch was skipped.
 */
// FUNCTION: FF7 0x004de273
void __cdecl BattleFx_SpecialScorchTrailModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxEnergyOrbTrailModel *trailModel; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.trailModel = (BattleFxEnergyOrbTrailModel *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialScorchParamVector1.loadedModel) = Gfx_ModelTableGetRenderObject(5, g_BattleSpecialScorchModelToggle);
    if (frame.trailModel->frameCount > 2) {
        RotMatrix((SVECTOR *)&g_BattleSpecialScorchRotationMoto, &frame.modelMatrix);
        ConcatMatrixPreservingTranslation(&g_BattleSpecialScorchDrawListHandle, &frame.modelMatrix, &frame.modelMatrix);
        frame.scaleX.vz = 0x4000 - frame.trailModel->frameCount * 0x190;
        frame.scaleX.vy = frame.scaleX.vz;
        frame.scaleX.vx = frame.scaleX.vy;
        ScaleMatrix(&frame.modelMatrix, &frame.scaleX);
        frame.modelMatrix.t[0] = frame.trailModel->posX;
        frame.modelMatrix.t[1] = frame.trailModel->posY;
        frame.modelMatrix.t[2] = frame.trailModel->posZ;
        MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
        SetRotMatrix(&frame.drawMatrix);
        SetTransMatrix(&frame.drawMatrix);
        SetDrawColorRGB(0, 0, 0);
        g_BattleSpecialScorchParamVector1.alpha = frame.trailModel->frameCount * 0xC8;
        g_BattleSpecialScorchLoadedResTable = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialScorchParamVector1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialScorchLoadedResTable);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.trailModel->frameCount)++;
    if (frame.trailModel->frameCount == 0x14) {
        frame.trailModel->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
