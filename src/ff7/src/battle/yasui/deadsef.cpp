// src/ff7/src/battle/yasui/deadsef.cpp
/* Safer Sephiroth Death & Dissolve Sequence (orig. deadsef.cpp, SUMMON/LASTBOSS, ENEMY331.DAT) */
// Original: C:\FF7\src\battle\yasui\deadsef.cpp
// Contents: Battle summon effect for the Last Boss sequence: a camera script, a timed flash
//   sequence, and the enemy model shattering into fragments that are drawn as PSX primitives, along
//   with fading models and billboards.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "lib/enums/graphics.h"
#include "ff7/enums/battle.h"
#include "lib/api/mem.h"
#include "ff7/battle_tasks.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/camera_script.h"
#include "lib/api/sound.h"
#include "lib/api/file.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(memset)
// clang-format off
// GLOBAL: FF7 0x007e2ca8
unsigned char g_SaferSephirothDeath_SoundGroup[16] = {0xda, 0x02, 0x00, 0x00, 0xdb, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007e2cb8
void *g_SaferSephirothDeathSoundGroupPointerList[2] = {&g_SaferSephirothDeath_SoundGroup, 0};
// GLOBAL: FF7 0x007e2cc0
SaferSephirothDeathResourceRecord g_SaferSephirothDeath_ResourceTable[6] = {
    {0x40b1, 200, &g_SaferSephirothDeathCircleRsdName, 0, 0, 0},
    {
        0x40b1,
        200,
        // STRING: FF7 0x007e2e14
        "LINE.RSD",
        0,
        0,
        0
    },
    {
        0x7c3c,
        25,
        // STRING: FF7 0x007e2e20
        "ENEMY331.TIM",
        0,
        0,
        0
    },
    {
        0x7c39,
        25,
        // STRING: FF7 0x007e2e30
        "ENEMY331.TIM",
        0,
        0,
        0
    },
    {
        0x783c,
        25,
        // STRING: FF7 0x007e2e40
        "ENEMY331.TIM",
        0,
        0,
        0
    },
    {
        0x7839,
        25,
        // STRING: FF7 0x007e2e50
        "ENEMY331.TIM",
        0,
        0,
        0
    }
};
// GLOBAL: FF7 0x007e2d38
short g_SaferSephirothDeath_CameraScript[27] = {129, 0, (-300), 0, 65, 0, (-300), 3000, 128, 40, 64, 40, 130, 199, 0, 0, 0, 66, 199, (-1000), (-1000), 5000, 128, 510, 64, 510, (-1)};
// GLOBAL: FF7 0x007e2d70
SaferSephirothDeathMultiPartDrawReq g_SaferSephirothDeathMultiPartDrawRequest = {0, BATTLE_FX_BZ_MORPH_FACE_GREY, {0, 0}, 0, 0x20};
// GLOBAL: FF7 0x007e2d80
short g_SaferSephirothDeathFlashFrameThresholds[18] = {0, 10, 40, 100, 105, 130, 180, 200, 267, 287, 292, 327, 349, 362, 432, 482, (-1), 0};
// GLOBAL: FF7 0x007e2da8
struct SaferSephirothDeath_BlackModelDrawReq g_SaferSephirothDeathBlackModelDrawRequest = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x007e2db8
struct PsxMatrix g_SaferSephirothDeathViewTransform = {{{4096, 0, 0}, {0, 4096, 0}, {0, 0, 4096}}, {0, 0, 0}};
// GLOBAL: FF7 0x007e2dd8
struct PsxMatrix g_SaferSephirothDeathSecondaryTransform = {{{4096, 0, 0}, {0, 4096, 0}, {0, 0, 4096}}, {0, -500, 0}};
// GLOBAL: FF7 0x007e2df8
SaferSephirothDeathBillboardDrawReq g_SaferSephirothDeathBillboardDrawRequest = {0, 0xa8, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x007e2e08
unsigned char g_SaferSephirothDeathCircleRsdName[12] = "CIRCLE1.RSD";
// GLOBAL: FF7 0x00a1d3c8
int g_SaferSephirothDeath_SubTaskCount = 0;
unsigned char g_bssPad_00a1d3cc[4] = {0};
// GLOBAL: FF7 0x00a1d3d0
struct SaferSephirothDeathFragmentQuad g_SaferSephirothDeath_FragmentBuffer[0x97C] = {0};
// GLOBAL: FF7 0x00a30350
struct SaferSephirothDeathOperandPool g_SaferSephirothDeath_CameraScriptOperandPool = {0};
unsigned char g_bssPad_00a30358[24] = {0};
// GLOBAL: FF7 0x00a30370
DeadSefCameraTrack g_SaferSephirothDeathCameraFocus = {0};
// GLOBAL: FF7 0x00a3039c
int g_SaferSephirothDeath_BillboardSpawnAccum = 0;
// GLOBAL: FF7 0x00a303a0
int *g_SaferSephirothDeath_ModelTablePtr = 0;
// GLOBAL: FF7 0x00a303a4
struct PsxMatrix *g_pSaferSephirothDeathScratchMatrix = 0;
// GLOBAL: FF7 0x00a303a8
struct BattleFxSaferSephirothDeathTimelineStep *g_pSaferSephirothDeathTransformedVertices = 0;
// GLOBAL: FF7 0x00a303ac
unsigned char g_SaferSephirothDeath_FlashActive = 0;
// GLOBAL: FF7 0x00a303b0
DeadSefCameraTrack *g_SaferSephirothDeathCameraTrack = 0;
unsigned char g_bssPad_00a303b4[4] = {0};
// GLOBAL: FF7 0x00a303b8
struct PsxMatrix g_SaferSephirothDeath_TransformMatrix = {0};
// GLOBAL: FF7 0x00a303d8
unsigned char g_SaferSephirothDeath_FragmentStateInitDone = 0;
// GLOBAL: FF7 0x00a303dc
struct SVECTOR *g_SaferSephirothDeathShiftedVertices = 0;
// GLOBAL: FF7 0x00a303e0
DeadSefCameraTrack *g_SaferSephirothDeathCameraOther = 0;
unsigned char g_bssPad_00a303e4[4] = {0};
// GLOBAL: FF7 0x00a303e8
struct Vec3sPacked g_SaferSephirothDeath_BoneWorldPos = {0};
// GLOBAL: FF7 0x00a303f0
unsigned char g_SaferSephirothDeath_TimelineStepInitDone = 0;
unsigned char g_bssPad_00a303f4[4] = {0};
// GLOBAL: FF7 0x00a303f8
struct Vec3sPacked g_SaferSephirothDeath_ActorPos = {0};
// GLOBAL: FF7 0x00a30400
DeadSefCameraTrack g_SaferSephirothDeathCameraEye = {0};
unsigned char g_bssPad_00a3042c[4] = {0};
// GLOBAL: FF7 0x00a30430
struct SVECTOR g_SaferSephirothDeath_FragmentVertexBuf[0x97C] = {0};
// GLOBAL: FF7 0x00a35010
struct SaferSephirothDeathFragmentMotion g_SaferSephirothDeath_FragmentNormalBuf[0x97C] = {0};
// GLOBAL: FF7 0x00a37600
short g_SaferSephirothDeath_FragmentColorBuf[0x97C] = {0};
// GLOBAL: FF7 0x00a388f8
int g_SaferSephirothDeath_EnemyDatHandle = 0;
// GLOBAL: FF7 0x00a388fc
struct BattleFxSefFragmentState *g_pSaferSephirothDeath_FragmentState = 0;
// GLOBAL: FF7 0x00a38900
unsigned char g_SaferSephirothDeathShiftedVerticesInitDone = 0;
unsigned char g_bssPad_00a38904[4] = {0};
// GLOBAL: FF7 0x00a38908
struct PsxMatrix g_SaferSephirothDeath_SavedMatrix = {0};
// GLOBAL: FF7 0x00a38928
int g_SaferSephirothDeath_TaskFinished = 0;
// GLOBAL: FF7 0x00a3892c
struct GfxTableCursor *g_SaferSephirothDeath_Loaded = 0;
// clang-format on


/*
 * Initializes and loads the SPECIAL/LASTBOSS effect resource and registers
 * its sound group list.
 */
// FUNCTION: FF7 0x00452020
void __cdecl BattleFx_SpecialLoadResSaferSephirothDeath(void)
{
    GfxModelTableBank *bits;
    GfxResourceRequest requestStorage;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_SaferSephirothDeath_Loaded == 0) {
        kind = 6;
        resourceTable = (int)g_SaferSephirothDeath_ResourceTable;
        bits = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestStorage;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x007e2e60
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("SUMMON/LASTBOSS", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_SaferSephirothDeath_Loaded, bits);
        SfxRegisterSoundGroups((struct SoundGroup **)g_SaferSephirothDeathSoundGroupPointerList);
    }
}

/*
 * Initializes transformation matrix, clears state globals, loads LastBoss resources,
 * reads enemy model table, and spawns the main effect timeline task.
 */
// FUNCTION: FF7 0x004520c1
void __cdecl BattleFx_SaferSephirothDeathLoadAndBegin(int targetMask, int actorIdx)
{
    memset(&g_SaferSephirothDeath_TransformMatrix, 0, 30);
    g_SaferSephirothDeath_TransformMatrix.m[0][0] = g_SaferSephirothDeath_TransformMatrix.m[1][1] = g_SaferSephirothDeath_TransformMatrix.m[2][2] = 0x1000;
    g_SaferSephirothDeath_ActorPos.vx = g_SaferSephirothDeath_ActorPos.vy = g_SaferSephirothDeath_ActorPos.vz = 0;
    g_SaferSephirothDeath_SavedMatrix = g_SaferSephirothDeath_TransformMatrix;
    g_SaferSephirothDeath_SubTaskCount = 0;
    g_SaferSephirothDeath_TaskFinished = 0;
    g_SaferSephirothDeath_BillboardSpawnAccum = 0;
    BattleFx_SpecialLoadResSaferSephirothDeath();
    g_SaferSephirothDeath_ModelTablePtr = (int *)BattleFx_SaferSephirothDeathLoadEnemy331ModelTable();
    BattleFx_SaferSephirothDeathSpawnMainTaskAndCamera(targetMask, actorIdx);
}

/*
 * Acquires actor 4 bone coordinates into camera state, spawns the timeline task,
 * sets actor state flags, triggers the camera script, and dispatches sound effect.
 */
// FUNCTION: FF7 0x00452170
void __cdecl BattleFx_SaferSephirothDeathSpawnMainTaskAndCamera(int targetMask, int actorIdx)
{
    int i;

    Battle_GetBoneWorldPos(4, g_BattleActorRecords[4].targetBone, &g_SaferSephirothDeath_BoneWorldPos.vx);
    g_SaferSephirothDeath_ActorPos.vx = g_SaferSephirothDeath_BoneWorldPos.vx;
    g_SaferSephirothDeath_TransformMatrix.t[0] = g_SaferSephirothDeath_ActorPos.vx;
    g_SaferSephirothDeath_ActorPos.vy = g_SaferSephirothDeath_BoneWorldPos.vy;
    g_SaferSephirothDeath_TransformMatrix.t[1] = g_SaferSephirothDeath_ActorPos.vy;
    g_SaferSephirothDeath_ActorPos.vz = g_SaferSephirothDeath_BoneWorldPos.vz;
    g_SaferSephirothDeath_TransformMatrix.t[2] = g_SaferSephirothDeath_ActorPos.vz;
    Battle_AllocateResourceSlot(BattleFx_SaferSephirothDeathTimelineTask);
    for (i = 0; i < 3; i++) {
        g_BattleActorRecords[i].drawFlags |= 2;
        g_BattleActorRecords[i].drawFlags |= 4;
    }
    BattleFx_SaferSephirothDeathSpawnCameraScript(g_SaferSephirothDeath_CameraScript, &g_SaferSephirothDeath_TransformMatrix, 0);
    g_BattleScriptEffectFlag = 1;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_2, 0x40, 0);
}

/*
 * Allocates a resource task slot for BattleFx_SaferSephirothDeathRunCameraScript and configures
 * its script pointer, transformation matrix, and camera flag.
 */
// FUNCTION: FF7 0x0045226d
void __cdecl BattleFx_SaferSephirothDeathSpawnCameraScript(short *scriptAddr, PsxMatrix *matrixAddr, int cameraFlag)
{
    BattleFxSaferSephirothDeathCameraSlot *slot;

    slot = (BattleFxSaferSephirothDeathCameraSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SaferSephirothDeathRunCameraScript);
    slot->scriptAddr = scriptAddr;
    slot->matrixAddr = matrixAddr;
    slot->actor = cameraFlag;
}

/*
 * Camera script interpreter task driving eye and focus tracks. Parses script opcodes
 * for linear motion, distance adjustment, acceleration, easing, and actor tracking.
 */
// FUNCTION: FF7 0x004522ad
void __cdecl BattleFx_SaferSephirothDeathRunCameraScript(void)
{
    BattleFxSaferSephirothDeathCameraSlot *script;
    int tailPtr;
    short opcode;
    int padField[46]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int padHole[6]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    Vec3sPacked *offsetPointer;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unusedPad[14]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxCameraVec *vec;
    Vec3sPacked *ofsPtr;
    int easedDistance;
    int i;
    int tmpPoint;

    script = (BattleFxSaferSephirothDeathCameraSlot *)BATTLE_CURRENT_TASK_SLOT;
    ofsPtr = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    offsetPointer = (Vec3sPacked *)((int)ofsPtr + 8);
    vec = (BattleFxCameraVec *)((int)offsetPointer + 8);
    tailPtr = (int)vec + 0x10;
    if (g_BattleIsPaused == 0) {
        if (script->started == 0) {
            g_SaferSephirothDeathCameraEye.frames = 0;
            g_SaferSephirothDeathCameraFocus.frames = 0;
            g_SaferSephirothDeathCameraEye.actor = 0xFFFF;
            g_SaferSephirothDeathCameraFocus.actor = 0xFFFF;
            script->started = 1;
        }
        while (g_SaferSephirothDeathCameraEye.frames == 0 || g_SaferSephirothDeathCameraFocus.frames == 0) {
            opcode = *script->scriptAddr;
            script->scriptAddr++;
            if ((opcode & 0x40) != 0) {
                g_SaferSephirothDeathCameraTrack = &g_SaferSephirothDeathCameraEye;
                g_SaferSephirothDeathCameraOther = &g_SaferSephirothDeathCameraFocus;
            } else {
                g_SaferSephirothDeathCameraTrack = &g_SaferSephirothDeathCameraFocus;
                g_SaferSephirothDeathCameraOther = &g_SaferSephirothDeathCameraEye;
            }
            g_SaferSephirothDeathCameraTrack->mode = opcode & ~0xC0;
            switch (g_SaferSephirothDeathCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                g_SaferSephirothDeathCameraTrack->frames = BattleFx_SaferSephirothDeathReadCameraScriptOperand(&script->scriptAddr);
                break;
            case SUMMON_CAMERA_SET_POSITION:
                g_SaferSephirothDeathCameraTrack->u.position.x = *script->scriptAddr;
                script->scriptAddr++;
                g_SaferSephirothDeathCameraTrack->u.position.y = *script->scriptAddr;
                script->scriptAddr++;
                g_SaferSephirothDeathCameraTrack->u.position.z = *script->scriptAddr;
                script->scriptAddr++;
                break;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_SaferSephirothDeathCameraTrack->frames = BattleFx_SaferSephirothDeathReadCameraScriptOperand(&script->scriptAddr);
                g_SaferSephirothDeathCameraTrack->velocity.x = (*script->scriptAddr - g_SaferSephirothDeathCameraTrack->u.position.x) / g_SaferSephirothDeathCameraTrack->frames;
                script->scriptAddr++;
                g_SaferSephirothDeathCameraTrack->velocity.y = (*script->scriptAddr - g_SaferSephirothDeathCameraTrack->u.position.y) / g_SaferSephirothDeathCameraTrack->frames;
                script->scriptAddr++;
                g_SaferSephirothDeathCameraTrack->velocity.z = (*script->scriptAddr - g_SaferSephirothDeathCameraTrack->u.position.z) / g_SaferSephirothDeathCameraTrack->frames;
                script->scriptAddr++;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                ofsPtr->vx = g_SaferSephirothDeathCameraOther->u.position.x - g_SaferSephirothDeathCameraTrack->u.position.x;
                ofsPtr->vy = g_SaferSephirothDeathCameraOther->u.position.y - g_SaferSephirothDeathCameraTrack->u.position.y;
                ofsPtr->vz = g_SaferSephirothDeathCameraOther->u.position.z - g_SaferSephirothDeathCameraTrack->u.position.z;
                g_SaferSephirothDeathCameraTrack->distance = SqrtToInt(ofsPtr->vx * ofsPtr->vx + ofsPtr->vy * ofsPtr->vy + ofsPtr->vz * ofsPtr->vz);
                g_SaferSephirothDeathCameraTrack->base = g_SaferSephirothDeathCameraTrack->u.position;
                g_SaferSephirothDeathCameraTrack->frames = BattleFx_SaferSephirothDeathReadCameraScriptOperand(&script->scriptAddr);
                ofsPtr->vx = *script->scriptAddr;
                script->scriptAddr++;
                ofsPtr->vy = *script->scriptAddr;
                script->scriptAddr++;
                ofsPtr->vz = *script->scriptAddr;
                script->scriptAddr++;
                g_SaferSephirothDeathCameraTrack->velocity.x = (ofsPtr->vx - g_SaferSephirothDeathCameraTrack->u.position.x) / g_SaferSephirothDeathCameraTrack->frames;
                g_SaferSephirothDeathCameraTrack->velocity.y = (ofsPtr->vy - g_SaferSephirothDeathCameraTrack->u.position.y) / g_SaferSephirothDeathCameraTrack->frames;
                g_SaferSephirothDeathCameraTrack->velocity.z = (ofsPtr->vz - g_SaferSephirothDeathCameraTrack->u.position.z) / g_SaferSephirothDeathCameraTrack->frames;
                switch (g_SaferSephirothDeathCameraOther->mode) {
                case SUMMON_CAMERA_MOVE_LINEAR:
                    offsetPointer->vx = g_SaferSephirothDeathCameraOther->u.position.x + g_SaferSephirothDeathCameraOther->velocity.x * g_SaferSephirothDeathCameraTrack->frames;
                    offsetPointer->vy = g_SaferSephirothDeathCameraOther->u.position.y + g_SaferSephirothDeathCameraOther->velocity.y * g_SaferSephirothDeathCameraTrack->frames;
                    offsetPointer->vz = g_SaferSephirothDeathCameraOther->u.position.z + g_SaferSephirothDeathCameraOther->velocity.z * g_SaferSephirothDeathCameraTrack->frames;
                    break;
                default:
                    offsetPointer->vx = g_SaferSephirothDeathCameraOther->u.position.x;
                    offsetPointer->vy = g_SaferSephirothDeathCameraOther->u.position.y;
                    offsetPointer->vz = g_SaferSephirothDeathCameraOther->u.position.z;
                }
                ofsPtr->vx -= offsetPointer->vx;
                ofsPtr->vy -= offsetPointer->vy;
                ofsPtr->vz -= offsetPointer->vz;
                g_SaferSephirothDeathCameraTrack->delta = (SqrtToInt(ofsPtr->vx * ofsPtr->vx + ofsPtr->vy * ofsPtr->vy + ofsPtr->vz * ofsPtr->vz) - g_SaferSephirothDeathCameraTrack->distance) / g_SaferSephirothDeathCameraTrack->frames;
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_SaferSephirothDeathCameraTrack->frames = BattleFx_SaferSephirothDeathReadCameraScriptOperand(&script->scriptAddr);
                ofsPtr->vx = *script->scriptAddr - g_SaferSephirothDeathCameraTrack->u.position.x;
                script->scriptAddr++;
                ofsPtr->vy = *script->scriptAddr - g_SaferSephirothDeathCameraTrack->u.position.y;
                script->scriptAddr++;
                ofsPtr->vz = *script->scriptAddr - g_SaferSephirothDeathCameraTrack->u.position.z;
                script->scriptAddr++;
                g_SaferSephirothDeathCameraTrack->base.x = (ofsPtr->vx - g_SaferSephirothDeathCameraTrack->velocity.x * g_SaferSephirothDeathCameraTrack->frames << 1) / (g_SaferSephirothDeathCameraTrack->frames * g_SaferSephirothDeathCameraTrack->frames);
                g_SaferSephirothDeathCameraTrack->base.y = (ofsPtr->vy - g_SaferSephirothDeathCameraTrack->velocity.y * g_SaferSephirothDeathCameraTrack->frames << 1) / (g_SaferSephirothDeathCameraTrack->frames * g_SaferSephirothDeathCameraTrack->frames);
                g_SaferSephirothDeathCameraTrack->base.z = (ofsPtr->vz - g_SaferSephirothDeathCameraTrack->velocity.z * g_SaferSephirothDeathCameraTrack->frames << 1) / (g_SaferSephirothDeathCameraTrack->frames * g_SaferSephirothDeathCameraTrack->frames);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_SaferSephirothDeathCameraTrack->frames = BattleFx_SaferSephirothDeathReadCameraScriptOperand(&script->scriptAddr);
                g_SaferSephirothDeathCameraTrack->easing = *script->scriptAddr;
                script->scriptAddr++;
                g_SaferSephirothDeathCameraTrack->distance = 0;
                g_SaferSephirothDeathCameraTrack->delta = 0x1000 / g_SaferSephirothDeathCameraTrack->frames;
                g_SaferSephirothDeathCameraTrack->base.x = g_SaferSephirothDeathCameraTrack->u.position.x;
                g_SaferSephirothDeathCameraTrack->base.y = g_SaferSephirothDeathCameraTrack->u.position.y;
                g_SaferSephirothDeathCameraTrack->base.z = g_SaferSephirothDeathCameraTrack->u.position.z;
                g_SaferSephirothDeathCameraTrack->velocity.x = *script->scriptAddr - g_SaferSephirothDeathCameraTrack->u.position.x;
                script->scriptAddr++;
                g_SaferSephirothDeathCameraTrack->velocity.y = *script->scriptAddr - g_SaferSephirothDeathCameraTrack->u.position.y;
                script->scriptAddr++;
                g_SaferSephirothDeathCameraTrack->velocity.z = *script->scriptAddr - g_SaferSephirothDeathCameraTrack->u.position.z;
                script->scriptAddr++;
                break;
            case SUMMON_CAMERA_ATTACH_BONE:
                if ((g_SaferSephirothDeathCameraTrack->actor = *script->scriptAddr++) == 0) {
                    g_SaferSephirothDeathCameraTrack->actor = script->actor;
                }
                g_SaferSephirothDeathCameraTrack->bone = *script->scriptAddr;
                script->scriptAddr++;
                BattleFx_SaferSephirothDeathGetActorLocalPosition(g_SaferSephirothDeathCameraTrack->actor, g_SaferSephirothDeathCameraTrack->bone, (BattleFxActorMatrix *)script->matrixAddr, ofsPtr);
                g_SaferSephirothDeathCameraTrack->u.position.x -= ofsPtr->vx;
                g_SaferSephirothDeathCameraTrack->u.position.y -= ofsPtr->vy;
                g_SaferSephirothDeathCameraTrack->u.position.z -= ofsPtr->vz;
                break;
            case SUMMON_CAMERA_DETACH:
                BattleFx_SaferSephirothDeathGetActorLocalPosition(g_SaferSephirothDeathCameraTrack->actor, g_SaferSephirothDeathCameraTrack->bone, (BattleFxActorMatrix *)script->matrixAddr, ofsPtr);
                g_SaferSephirothDeathCameraTrack->u.position.x += ofsPtr->vx;
                g_SaferSephirothDeathCameraTrack->u.position.y += ofsPtr->vy;
                g_SaferSephirothDeathCameraTrack->u.position.z += ofsPtr->vz;
                g_SaferSephirothDeathCameraTrack->actor = 0xFFFF;
                break;
            case SUMMON_CAMERA_WAIT_NO_WRITEBACK:
                g_SaferSephirothDeathCameraTrack->frames = BattleFx_SaferSephirothDeathReadCameraScriptOperand(&script->scriptAddr);
                break;
            case SUMMON_CAMERA_SET_SCRIPT_VALUE:
                g_BattleCameraScriptValue = *script->scriptAddr;
                script->scriptAddr++;
                break;
            default:
                script->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        g_SaferSephirothDeathCameraTrack = &g_SaferSephirothDeathCameraEye;
        g_SaferSephirothDeathCameraOther = &g_SaferSephirothDeathCameraFocus;
        for (i = 0; i < 2; i++) {
            switch (g_SaferSephirothDeathCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_SaferSephirothDeathCameraTrack->velocity.x += g_SaferSephirothDeathCameraTrack->base.x;
                g_SaferSephirothDeathCameraTrack->velocity.y += g_SaferSephirothDeathCameraTrack->base.y;
                g_SaferSephirothDeathCameraTrack->velocity.z += g_SaferSephirothDeathCameraTrack->base.z;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_SaferSephirothDeathCameraTrack->u.position.x += g_SaferSephirothDeathCameraTrack->velocity.x;
                g_SaferSephirothDeathCameraTrack->u.position.y += g_SaferSephirothDeathCameraTrack->velocity.y;
                g_SaferSephirothDeathCameraTrack->u.position.z += g_SaferSephirothDeathCameraTrack->velocity.z;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                g_SaferSephirothDeathCameraTrack->base.x += g_SaferSephirothDeathCameraTrack->velocity.x;
                g_SaferSephirothDeathCameraTrack->base.y += g_SaferSephirothDeathCameraTrack->velocity.y;
                g_SaferSephirothDeathCameraTrack->base.z += g_SaferSephirothDeathCameraTrack->velocity.z;
                g_SaferSephirothDeathCameraTrack->distance += g_SaferSephirothDeathCameraTrack->delta;
                vec->x = g_SaferSephirothDeathCameraOther->u.position.x - g_SaferSephirothDeathCameraTrack->base.x;
                vec->y = g_SaferSephirothDeathCameraOther->u.position.y - g_SaferSephirothDeathCameraTrack->base.y;
                vec->z = g_SaferSephirothDeathCameraOther->u.position.z - g_SaferSephirothDeathCameraTrack->base.z;
                VectorNormalToSVector((int *)vec, (short *)ofsPtr);
                g_SaferSephirothDeathCameraTrack->u.position.x = g_SaferSephirothDeathCameraOther->u.position.x - (ofsPtr->vx * g_SaferSephirothDeathCameraTrack->distance >> 0xC);
                g_SaferSephirothDeathCameraTrack->u.position.y = g_SaferSephirothDeathCameraOther->u.position.y - (ofsPtr->vy * g_SaferSephirothDeathCameraTrack->distance >> 0xC);
                g_SaferSephirothDeathCameraTrack->u.position.z = g_SaferSephirothDeathCameraOther->u.position.z - (ofsPtr->vz * g_SaferSephirothDeathCameraTrack->distance >> 0xC);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_SaferSephirothDeathCameraTrack->distance += g_SaferSephirothDeathCameraTrack->delta;
                easedDistance = g_SaferSephirothDeathCameraTrack->distance;
                easedDistance = BattleFx_SaferSephirothDeathApplyCameraEasing(easedDistance, g_SaferSephirothDeathCameraTrack->easing);
                g_SaferSephirothDeathCameraTrack->u.position.x = g_SaferSephirothDeathCameraTrack->base.x + (g_SaferSephirothDeathCameraTrack->velocity.x * easedDistance >> 0xC);
                g_SaferSephirothDeathCameraTrack->u.position.y = g_SaferSephirothDeathCameraTrack->base.y + (g_SaferSephirothDeathCameraTrack->velocity.y * easedDistance >> 0xC);
                g_SaferSephirothDeathCameraTrack->u.position.z = g_SaferSephirothDeathCameraTrack->base.z + (g_SaferSephirothDeathCameraTrack->velocity.z * easedDistance >> 0xC);
            }
            (g_SaferSephirothDeathCameraTrack->frames)--;
            g_SaferSephirothDeathCameraTrack = &g_SaferSephirothDeathCameraFocus;
            g_SaferSephirothDeathCameraOther = &g_SaferSephirothDeathCameraEye;
        }
    }
    if (script->matrixAddr != 0) {
        if (g_SaferSephirothDeathCameraEye.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            SetRotMatrix(script->matrixAddr);
            SetTransMatrix(script->matrixAddr);
            if (g_SaferSephirothDeathCameraEye.actor == -1) {
                RotTransPoint((SVECTOR *)&g_SaferSephirothDeathCameraEye.u.position, &vec->x, &tmpPoint);
            } else {
                BattleFx_SaferSephirothDeathGetActorLocalPosition(g_SaferSephirothDeathCameraEye.actor, g_SaferSephirothDeathCameraEye.bone, (BattleFxActorMatrix *)script->matrixAddr, ofsPtr);
                ofsPtr->vx += g_SaferSephirothDeathCameraEye.u.position.x;
                ofsPtr->vy += g_SaferSephirothDeathCameraEye.u.position.y;
                ofsPtr->vz += g_SaferSephirothDeathCameraEye.u.position.z;
                SetRotMatrix(script->matrixAddr);
                SetTransMatrix(script->matrixAddr);
                RotTransPoint((SVECTOR *)ofsPtr, &vec->x, &tmpPoint);
            }
            g_BattleCameraPosition.cameraPosX = vec->x;
            g_BattleCameraPosition.cameraPosY = vec->y;
            g_BattleCameraPosZ = vec->z;
        }
        if (g_SaferSephirothDeathCameraFocus.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            if (g_SaferSephirothDeathCameraFocus.actor == -1) {
                RotTransPoint((SVECTOR *)&g_SaferSephirothDeathCameraFocus.u.position, &vec->x, &tmpPoint);
            } else {
                BattleFx_SaferSephirothDeathGetActorLocalPosition(g_SaferSephirothDeathCameraFocus.actor, g_SaferSephirothDeathCameraFocus.bone, (BattleFxActorMatrix *)script->matrixAddr, ofsPtr);
                ofsPtr->vx += g_SaferSephirothDeathCameraFocus.u.position.x;
                ofsPtr->vy += g_SaferSephirothDeathCameraFocus.u.position.y;
                ofsPtr->vz += g_SaferSephirothDeathCameraFocus.u.position.z;
                SetRotMatrix(script->matrixAddr);
                SetTransMatrix(script->matrixAddr);
                RotTransPoint((SVECTOR *)ofsPtr, &vec->x, &tmpPoint);
            }
            g_BattleCameraFocalPoint.focalPointX = vec->x;
            g_BattleCameraFocalPoint.focalPointY = vec->y;
            g_BattleCameraFocusZ = vec->z;
        }
    } else {
        if (g_SaferSephirothDeathCameraEye.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            *(int *)&g_BattleCameraPosition = g_SaferSephirothDeathCameraEye.u.positionWords[0];
            *(int *)&g_BattleCameraPosZ = g_SaferSephirothDeathCameraEye.u.positionWords[1];
        }
        if (g_SaferSephirothDeathCameraFocus.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            *(int *)&g_BattleCameraFocalPoint = g_SaferSephirothDeathCameraFocus.u.positionWords[0];
            *(int *)&g_BattleCameraFocusZ = g_SaferSephirothDeathCameraFocus.u.positionWords[1];
        }
    }
}

/*
 * Fetches the next 16-bit word from the camera script; if negative, resolves
 * operand from the camera script operand pool.
 */
// FUNCTION: FF7 0x0045325f
short __cdecl BattleFx_SaferSephirothDeathReadCameraScriptOperand(short **pScriptPtr)
{
    int val;

    val = **pScriptPtr;
    (*pScriptPtr)++;
    if (val < 0) {
        val = (&g_SaferSephirothDeath_CameraScriptOperandPool.u.operandWords[1])[-val];
    }
    return (short)val;
}

/*
 * Applies sinusoidal camera easing interpolation to a fixed-point value based
 * on magnitude and directional parameters.
 */
// FUNCTION: FF7 0x00453299
int __cdecl BattleFx_SaferSephirothDeathApplyCameraEasing(int value, int easingParam)
{
    int iteration;
    int origValue;
    int passCount;

    origValue = value;
    if (easingParam > 0) {
        passCount = easingParam >> 8;
        for (iteration = 0; iteration < passCount; iteration++) {
            value = SinFixed(((value) / 4));
        }
    } else if (easingParam < 0) {
        easingParam = -easingParam;
        passCount = easingParam >> 8;
        value = 0x1000 - value;
        for (iteration = 0; iteration < passCount; iteration++) {
            value = SinFixed(((value) / 4));
        }
        value = 0x1000 - value;
    } else {
        return value;
    }
    passCount = (easingParam & 0xFF) + 1;
    value = value * passCount + origValue * (0x100 - passCount) >> 8;
    return value;
}

/*
 * Computes an actor or bone position in world space and transforms it into the
 * local coordinate frame of the specified matrix.
 */
// FUNCTION: FF7 0x00453385
void __cdecl BattleFx_SaferSephirothDeathGetActorLocalPosition(int actorIdx, int boneIdx, BattleFxActorMatrix *pMatrix, Vec3sPacked *pOutPos)
{
    int w[8];

    if (boneIdx != -1) {
        Battle_GetBoneWorldPos(actorIdx, boneIdx, &pOutPos->vx);
    } else {
        pOutPos->vx = g_BattleActorRecords[actorIdx].posX;
        pOutPos->vy = g_BattleActorRecords[actorIdx].posY;
        pOutPos->vz = g_BattleActorRecords[actorIdx].posZ;
    }
    pOutPos->vx -= pMatrix->translationX;
    pOutPos->vy -= pMatrix->translationY;
    pOutPos->vz -= pMatrix->translationZ;
    TransposeMatrix(pMatrix, w);
    TransformVectorToInt16(w, (void *)pOutPos, (void *)pOutPos);
}

/*
 * Master timeline task sequencing flash sequences, fading models, billboards,
 * shatter mesh generation, and eventual cleanup of LastBoss resources.
 */
// FUNCTION: FF7 0x00453450
void __cdecl BattleFx_SaferSephirothDeathTimelineTask(void)
{
    int frame;
    BattleTaskSlot *task;
    int child;
    int slotIdx;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame = task->frameCounter;
    if (frame < 0xC8) {
        if (frame == 0) {
            g_BattleFxTimelineState.red = 0;
            g_BattleFxTimelineState.channel1 = 0;
            g_BattleFxTimelineState.channel2 = 0;
            Battle_StartFixedPointRamp(0x400, 0xA);
            g_BattleActorStates[4].flags |= 2;
            Battle_AllocateResourceSlot(BattleFx_SaferSephirothDeathFlashSequenceTask);
            g_BattleActorRecords[4].tintR = 0xFF;
            g_BattleActorRecords[4].tintG = 0xFF;
            g_BattleActorRecords[4].tintB = 0xFF;
            g_BattleActorRecords[4].tintPhase = 0;
        }
        if (frame == 0x3C) {
            Battle_AllocateResourceSlot(BattleFx_SaferSephirothDeathSpawnFadingModelsTask);
        }
        if (frame == 0x3C) {
            Battle_AllocateResourceSlot(BattleFx_SaferSephirothDeathSpawnBillboardsTask);
        }
        if (frame == 0xC7) {
            BattleFx_SaferSephirothDeathBuildShatterFragments();
            g_BattleActorRecords[4].drawFlags |= 2;
            g_BattleActorRecords[4].drawFlags |= 4;
            g_BattleActorStates[4].flags &= (unsigned short)~2;
        }
        g_BattleActorRecords[4].tintPhase = ((SinFixed((frame << 0xC) * 3 / 0xC8 + 0xC00) + 0x1000) / 2) << 0xB >> 0xC;
    } else {
        frame -= 0xC8;
        if (frame < 0x1FE) {
            if (frame == 0) {
                child = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SaferSephirothDeathShatterDrawTask);
            }
            if (frame == 0x1C2) {
                Battle_AllocateSecondaryResourceSlot(BattleFx_SaferSephirothDeathWhiteFlashRampTask);
            }
        } else {
            g_BattleActorRecords[4].drawFlags &= 0xFD;
            g_BattleActorRecords[4].drawFlags |= 4;
            for (slotIdx = 0; slotIdx < 3; slotIdx++) {
                g_BattleActorRecords[slotIdx].drawFlags &= 0xFD;
                g_BattleActorRecords[slotIdx].drawFlags |= 4;
            }
            g_BattleScriptEffectFlag = 0;
            task->ownerSlot = BATTLE_TASK_FREE;
            // STRING: FF7 0x007e2e70
            GameHeapFree((void *)g_SaferSephirothDeath_EnemyDatHandle, "C:\\FF7\\src\\battle\\yasui\\deadsef.cpp", 0x2CF);
            // STRING: FF7 0x007e2e94
            GameHeapFree(g_SaferSephirothDeath_ModelTablePtr, "C:\\FF7\\src\\battle\\yasui\\deadsef.cpp", 0x2D0);
            g_SaferSephirothDeath_EnemyDatHandle = 0;
            g_SaferSephirothDeath_ModelTablePtr = 0;
            Sfx_ReleaseCachedBuffer(0x2DA, 0xF);
            Sfx_ReleaseCachedBuffer(0x2DB, 0xF);
        }
    }
    (task->frameCounter)++;
}

/*
 * Computes attachment matrices relative to camera for enemy model parts, extracts
 * shatter mesh fragments, and initializes random vertex velocities.
 */
// FUNCTION: FF7 0x004536e5
void __cdecl BattleFx_SaferSephirothDeathBuildShatterFragments(void)
{
    int partIdx;
    int fragmentCount;
    PsxMatrix viewMatrix;
    struct BattleActor *actorRec;

    actorRec = &g_BattleActorRecords[4];
    fragmentCount = 0;
    for (partIdx = 0; partIdx < actorRec->boneCount; partIdx++) {
        if (g_SaferSephirothDeath_ModelTablePtr[partIdx] != 0) {
            Battle3d_GetActorAttachMatrix(4, partIdx + 1, (PsxMatrix *)(actorRec->boneNodes + partIdx * 0x34));
            Battle_ComposeMatrixRelativeToCamera(g_BattleCameraTransform.matrix, actorRec->boneNodes + partIdx * 0x34, &viewMatrix);
            SetRotMatrix(&viewMatrix);
            SetTransMatrix(&viewMatrix);
            fragmentCount = BattleFx_SaferSephirothDeathExtractModelFragments((SaferSephirothDeathModelPart *)g_SaferSephirothDeath_ModelTablePtr[partIdx], fragmentCount);
        }
    }
    for (partIdx = 0; partIdx < 0x97C; partIdx++) {
        g_SaferSephirothDeath_FragmentNormalBuf[partIdx].startFrame = (g_SaferSephirothDeath_FragmentVertexBuf[partIdx].vz + rand() % 0xC8 + 0xE74) * 0x1C2 / 0xE74;
        g_SaferSephirothDeath_FragmentNormalBuf[partIdx].deltaZ = -(rand() % 5);
        g_SaferSephirothDeath_FragmentColorBuf[partIdx] = 0;
    }
}

/*
 * Decomposes model polygon vertices and normals into individual fragment triangles
 * and quads with relative offsets.
 */
// FUNCTION: FF7 0x00453829
int __cdecl BattleFx_SaferSephirothDeathExtractModelFragments(SaferSephirothDeathModelPart *modelData, int fragIndex)
{
    int padSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int vertexBase;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short *polyPointer;
    int padGap; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int count;
    int vertIndex;
    int polyIdx;

    if ((g_SaferSephirothDeath_TimelineStepInitDone & 1) == 0) {
        g_SaferSephirothDeath_TimelineStepInitDone |= 1;
        g_pSaferSephirothDeathTransformedVertices = (struct BattleFxSaferSephirothDeathTimelineStep *)(Gfx_GetPsxScratchpadPtr(0xC0));
    }
    vertexBase = (int)modelData->vertices;
    polyPointer = (short *)(vertexBase + modelData->primOffset);
    count = *polyPointer;
    polyPointer += 2;
    for (polyIdx = 0; polyIdx < count; polyIdx++) {
        for (vertIndex = 0; vertIndex < 3; vertIndex++) {
            RotTransPoint((SVECTOR *)(vertexBase + polyPointer[vertIndex]), &g_pSaferSephirothDeathTransformedVertices[vertIndex].x, &g_pSaferSephirothDeathTransformedVertices[4].x);
        }
        g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vx = (g_pSaferSephirothDeathTransformedVertices->x + g_pSaferSephirothDeathTransformedVertices[1].x + g_pSaferSephirothDeathTransformedVertices[2].x) / 3;
        g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vy = (g_pSaferSephirothDeathTransformedVertices->y + g_pSaferSephirothDeathTransformedVertices[1].y + g_pSaferSephirothDeathTransformedVertices[2].y) / 3;
        g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vz = (g_pSaferSephirothDeathTransformedVertices->z + g_pSaferSephirothDeathTransformedVertices[1].z + g_pSaferSephirothDeathTransformedVertices[2].z) / 3;
        for (vertIndex = 0; vertIndex < 3; vertIndex++) {
            g_SaferSephirothDeath_FragmentBuffer[fragIndex].corner[vertIndex].vx = g_pSaferSephirothDeathTransformedVertices[vertIndex].x - g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vx;
            g_SaferSephirothDeath_FragmentBuffer[fragIndex].corner[vertIndex].vy = g_pSaferSephirothDeathTransformedVertices[vertIndex].y - g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vy;
            g_SaferSephirothDeath_FragmentBuffer[fragIndex].corner[vertIndex].vz = g_pSaferSephirothDeathTransformedVertices[vertIndex].z - g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vz;
        }
        polyPointer += 8;
        fragIndex++;
    }
    count = *polyPointer;
    polyPointer += 2;
    for (polyIdx = 0; polyIdx < count; polyIdx++) {
        for (vertIndex = 0; vertIndex < 4; vertIndex++) {
            RotTransPoint((SVECTOR *)(vertexBase + polyPointer[vertIndex]), &g_pSaferSephirothDeathTransformedVertices[vertIndex].x, &g_pSaferSephirothDeathTransformedVertices[4].x);
        }
        g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vx = (g_pSaferSephirothDeathTransformedVertices->x + g_pSaferSephirothDeathTransformedVertices[1].x + g_pSaferSephirothDeathTransformedVertices[2].x + g_pSaferSephirothDeathTransformedVertices[3].x) / 4;
        g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vy = (g_pSaferSephirothDeathTransformedVertices->y + g_pSaferSephirothDeathTransformedVertices[1].y + g_pSaferSephirothDeathTransformedVertices[2].y + g_pSaferSephirothDeathTransformedVertices[3].y) / 4;
        g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vz = (g_pSaferSephirothDeathTransformedVertices->z + g_pSaferSephirothDeathTransformedVertices[1].z + g_pSaferSephirothDeathTransformedVertices[2].z + g_pSaferSephirothDeathTransformedVertices[3].z) / 4;
        for (vertIndex = 0; vertIndex < 4; vertIndex++) {
            g_SaferSephirothDeath_FragmentBuffer[fragIndex].corner[vertIndex].vx = g_pSaferSephirothDeathTransformedVertices[vertIndex].x - g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vx;
            g_SaferSephirothDeath_FragmentBuffer[fragIndex].corner[vertIndex].vy = g_pSaferSephirothDeathTransformedVertices[vertIndex].y - g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vy;
            g_SaferSephirothDeath_FragmentBuffer[fragIndex].corner[vertIndex].vz = g_pSaferSephirothDeathTransformedVertices[vertIndex].z - g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vz;
        }
        polyPointer += 0xA;
        fragIndex++;
    }
    count = *polyPointer;
    polyPointer += 2;
    for (polyIdx = 0; polyIdx < count; polyIdx++) {
        for (vertIndex = 0; vertIndex < 3; vertIndex++) {
            RotTransPoint((SVECTOR *)(vertexBase + polyPointer[vertIndex]), &g_pSaferSephirothDeathTransformedVertices[vertIndex].x, &g_pSaferSephirothDeathTransformedVertices[4].x);
        }
        g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vx = (g_pSaferSephirothDeathTransformedVertices->x + g_pSaferSephirothDeathTransformedVertices[1].x + g_pSaferSephirothDeathTransformedVertices[2].x) / 3;
        g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vy = (g_pSaferSephirothDeathTransformedVertices->y + g_pSaferSephirothDeathTransformedVertices[1].y + g_pSaferSephirothDeathTransformedVertices[2].y) / 3;
        g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vz = (g_pSaferSephirothDeathTransformedVertices->z + g_pSaferSephirothDeathTransformedVertices[1].z + g_pSaferSephirothDeathTransformedVertices[2].z) / 3;
        for (vertIndex = 0; vertIndex < 3; vertIndex++) {
            g_SaferSephirothDeath_FragmentBuffer[fragIndex].corner[vertIndex].vx = g_pSaferSephirothDeathTransformedVertices[vertIndex].x - g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vx;
            g_SaferSephirothDeath_FragmentBuffer[fragIndex].corner[vertIndex].vy = g_pSaferSephirothDeathTransformedVertices[vertIndex].y - g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vy;
            g_SaferSephirothDeath_FragmentBuffer[fragIndex].corner[vertIndex].vz = g_pSaferSephirothDeathTransformedVertices[vertIndex].z - g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vz;
        }
        polyPointer += 0xA;
        fragIndex++;
    }
    count = *polyPointer;
    polyPointer += 2;
    for (polyIdx = 0; polyIdx < count; polyIdx++) {
        for (vertIndex = 0; vertIndex < 4; vertIndex++) {
            RotTransPoint((SVECTOR *)(vertexBase + polyPointer[vertIndex]), &g_pSaferSephirothDeathTransformedVertices[vertIndex].x, &g_pSaferSephirothDeathTransformedVertices[4].x);
        }
        g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vx = (g_pSaferSephirothDeathTransformedVertices->x + g_pSaferSephirothDeathTransformedVertices[1].x + g_pSaferSephirothDeathTransformedVertices[2].x + g_pSaferSephirothDeathTransformedVertices[3].x) / 4;
        g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vy = (g_pSaferSephirothDeathTransformedVertices->y + g_pSaferSephirothDeathTransformedVertices[1].y + g_pSaferSephirothDeathTransformedVertices[2].y + g_pSaferSephirothDeathTransformedVertices[3].y) / 4;
        g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vz = (g_pSaferSephirothDeathTransformedVertices->z + g_pSaferSephirothDeathTransformedVertices[1].z + g_pSaferSephirothDeathTransformedVertices[2].z + g_pSaferSephirothDeathTransformedVertices[3].z) / 4;
        for (vertIndex = 0; vertIndex < 4; vertIndex++) {
            g_SaferSephirothDeath_FragmentBuffer[fragIndex].corner[vertIndex].vx = g_pSaferSephirothDeathTransformedVertices[vertIndex].x - g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vx;
            g_SaferSephirothDeath_FragmentBuffer[fragIndex].corner[vertIndex].vy = g_pSaferSephirothDeathTransformedVertices[vertIndex].y - g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vy;
            g_SaferSephirothDeath_FragmentBuffer[fragIndex].corner[vertIndex].vz = g_pSaferSephirothDeathTransformedVertices[vertIndex].z - g_SaferSephirothDeath_FragmentVertexBuf[fragIndex].vz;
        }
        polyPointer += 0xC;
        fragIndex++;
    }
    return fragIndex;
}

/*
 * Advances shatter fragment physical motion and submits PSX primitive drawing
 * commands for active fragments across multiple model components.
 */
// FUNCTION: FF7 0x00453fbf
void __cdecl BattleFx_SaferSephirothDeathShatterDrawTask(void)
{
    SVECTOR *verts;
    short *colorBuffer;
    BattleTaskSlot *pTaskSlot;
    int partIndex;
    SVECTOR *fragmentBuf;
    struct BattleActor *actorRec;

    pTaskSlot = BATTLE_CURRENT_TASK_SLOT;
    if ((g_SaferSephirothDeath_FlashActive & 1) == 0) {
        g_SaferSephirothDeath_FlashActive |= 1;
        g_pSaferSephirothDeathScratchMatrix = (struct PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused == 0) {
        BattleFx_SaferSephirothDeathStepFragmentMotion(pTaskSlot->frameCounter);
    }
    fragmentBuf = g_SaferSephirothDeath_FragmentBuffer[0].corner;
    verts = g_SaferSephirothDeath_FragmentVertexBuf;
    colorBuffer = g_SaferSephirothDeath_FragmentColorBuf;
    actorRec = &g_BattleActorRecords[4];
    MultiplyMatrix(&g_BattleCameraTransform, &g_SaferSephirothDeath_SavedMatrix, g_pSaferSephirothDeathScratchMatrix);
    SetRotMatrix(g_pSaferSephirothDeathScratchMatrix);
    SetTransMatrix(g_pSaferSephirothDeathScratchMatrix);
    RefreshGlobalTransformState();
    Gfx_SetPsxPrimQuadState(GFX_PSX_PRIM_POLY_FT3, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(2, g_SaferSephirothDeath_Loaded)));
    Gfx_SetPsxPrimQuadState(GFX_PSX_PRIM_POLY_FT3, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(3, g_SaferSephirothDeath_Loaded)));
    Gfx_SetPsxPrimQuadState(GFX_PSX_PRIM_POLY_FT4, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(4, g_SaferSephirothDeath_Loaded)));
    Gfx_SetPsxPrimQuadState(GFX_PSX_PRIM_POLY_FT4, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(5, g_SaferSephirothDeath_Loaded)));
    SetDrawColorRGB(0xFF, 0xFF, 0xFF);
    for (partIndex = 0; partIndex < actorRec->boneCount; partIndex++) {
        g_SaferSephirothDeathMultiPartDrawRequest.meshData = g_SaferSephirothDeath_ModelTablePtr[partIndex];
        if (g_SaferSephirothDeathMultiPartDrawRequest.meshData != 0) {
            g_BattleOverlayDrawListHead = BattleFx_SaferSephirothDeathDrawFragmentsPsxMesh((BattleFxSaferSephirothDeathMeshDraw *)&g_SaferSephirothDeathMultiPartDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead, &fragmentBuf, &verts, &colorBuffer);
        }
    }
    if (g_BattleIsPaused == 0) {
        g_SaferSephirothDeath_SavedMatrix.t[2] -= 0x1E;
        g_SaferSephirothDeath_ActorPos.vz -= 0x1E;
        (pTaskSlot->frameCounter)++;
        if (pTaskSlot->frameCounter >= 0x1FE) {
            pTaskSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Renders shatter fragments as PSX triangle and quad primitives with shading,
 * texture coordinates, and depth ordering into ordering tables.
 */
// FUNCTION: FF7 0x004541c7
int __cdecl BattleFx_SaferSephirothDeathDrawFragmentsPsxMesh(BattleFxSaferSephirothDeathMeshDraw *meshDraw, int orderingTable, int otShift, int drawListHead, SVECTOR **vertexOutPtr, SVECTOR **offsetCursorPtr, short **shadeCursorPtr)
{
    int padSlot[3]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int cursor;
    int colorWord[2];
    int queryFlags[2];
    SVECTOR *vertexSource;
    DeadSefPrimStream *primCursor;
    int index;
    short *shadeCursor;
    int stackHole; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short attrMask;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int count;
    BattleFxSaferSephirothDeathFragmentPacket *triPacket;
    SVECTOR *ofsPtr;
    BattleFxSaferSephirothDeathFragmentPacket *packetPtr;
    BattleFxSaferSephirothDeathFragmentCursorQuad *primBuffer;
    int elementSize;
    BattleFxSaferSephirothDeathFragmentCursorQuad *quadCursor;

    if ((g_SaferSephirothDeathShiftedVerticesInitDone & 1) == 0) {
        g_SaferSephirothDeathShiftedVerticesInitDone |= 1;
        g_SaferSephirothDeathShiftedVertices = (struct SVECTOR *)(Gfx_GetPsxScratchpadPtr(0xC0));
    }
    cursor = meshDraw->meshData + 4;
    primCursor = (DeadSefPrimStream *)(cursor + ((SaferSephirothDeathModelPart *)meshDraw->meshData)->primOffset);
    vertexSource = *vertexOutPtr;
    ofsPtr = *offsetCursorPtr;
    shadeCursor = *shadeCursorPtr;
    primBuffer = (BattleFxSaferSephirothDeathFragmentCursorQuad *)Gfx_GetPsxPacketScratchPtr();
    count = primCursor->count;
    primCursor = (DeadSefPrimStream *)((int)primCursor + 2);
    attrMask = primCursor->count & 0x19F | meshDraw->attrMask;
    primCursor = (DeadSefPrimStream *)((int)primCursor + 2);
    for (index = 0; index < count; index++) {
        g_SaferSephirothDeathShiftedVertices->vx = vertexSource->vx + ofsPtr->vx;
        g_SaferSephirothDeathShiftedVertices->vy = vertexSource->vy + ofsPtr->vy;
        g_SaferSephirothDeathShiftedVertices->vz = vertexSource->vz + ofsPtr->vz;
        g_SaferSephirothDeathShiftedVertices[1].vx = vertexSource[1].vx + ofsPtr->vx;
        g_SaferSephirothDeathShiftedVertices[1].vy = vertexSource[1].vy + ofsPtr->vy;
        g_SaferSephirothDeathShiftedVertices[1].vz = vertexSource[1].vz + ofsPtr->vz;
        g_SaferSephirothDeathShiftedVertices[2].vx = vertexSource[2].vx + ofsPtr->vx;
        g_SaferSephirothDeathShiftedVertices[2].vy = vertexSource[2].vy + ofsPtr->vy;
        g_SaferSephirothDeathShiftedVertices[2].vz = vertexSource[2].vz + ofsPtr->vz;
        StoreDeferredDrawArgs(g_SaferSephirothDeathShiftedVertices, g_SaferSephirothDeathShiftedVertices + 1, g_SaferSephirothDeathShiftedVertices + 2);
        ApplyDeferredDrawArgs();
        vertexSource += 4;
        ofsPtr++;
        Gfx_PsxStoreClearFlag(queryFlags);
        if ((queryFlags[0] & 0x60000) == 0) {
            primBuffer->tagWord = 0x7000000;
            Gfx_ReadTransformOrderingDepth2(&elementSize);
            primBuffer->colorCmd = primCursor->texTri.colorCmd;
            primBuffer->texUv1 = primCursor->texTri.texCoord1;
            primBuffer->texUv2 = primCursor->texTri.texCoord2;
            primBuffer->attrMask = attrMask;
            if ((meshDraw->flags & BATTLE_FX_BZ_MORPH_FACE_GREY) != 0) {
                colorWord[0] = primCursor->texTri.color & 0xFF;
                colorWord[0] = colorWord[0] << 0x10 | 0x24000000 | colorWord[0] << 8 | colorWord[0];
                if (*shadeCursor != 0) {
                    Gfx_ShadePsxVertexColor((CVECTOR *)colorWord, *shadeCursor, &primBuffer->u.colorVec);
                    primBuffer->u.code.primCode = PSX_PRIM_POLY_FT3;
                } else {
                    primBuffer->u.color = colorWord[0];
                }
            } else {
                primBuffer->u.color = colorWord[0] << 0x10 | 0x24000000 | colorWord[0] << 8 | colorWord[0];
            }
            if ((meshDraw->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                primBuffer->u.code.primCode |= GFX_PSX_PRIM_CODE_SEMI_TRANSPARENT;
            }
            Gfx_DrawPsxPrimitive(primBuffer);
        }
        shadeCursor++;
        primCursor = (DeadSefPrimStream *)((int)primCursor + 0x10);
    }
    count = primCursor->count;
    primCursor = (DeadSefPrimStream *)((int)primCursor + 4);
    quadCursor = primBuffer;
    for (index = 0; index < count; index++) {
        g_SaferSephirothDeathShiftedVertices->vx = vertexSource->vx + ofsPtr->vx;
        g_SaferSephirothDeathShiftedVertices->vy = vertexSource->vy + ofsPtr->vy;
        g_SaferSephirothDeathShiftedVertices->vz = vertexSource->vz + ofsPtr->vz;
        g_SaferSephirothDeathShiftedVertices[1].vx = vertexSource[1].vx + ofsPtr->vx;
        g_SaferSephirothDeathShiftedVertices[1].vy = vertexSource[1].vy + ofsPtr->vy;
        g_SaferSephirothDeathShiftedVertices[1].vz = vertexSource[1].vz + ofsPtr->vz;
        g_SaferSephirothDeathShiftedVertices[2].vx = vertexSource[2].vx + ofsPtr->vx;
        g_SaferSephirothDeathShiftedVertices[2].vy = vertexSource[2].vy + ofsPtr->vy;
        g_SaferSephirothDeathShiftedVertices[2].vz = vertexSource[2].vz + ofsPtr->vz;
        StoreDeferredDrawArgs(g_SaferSephirothDeathShiftedVertices, g_SaferSephirothDeathShiftedVertices + 1, g_SaferSephirothDeathShiftedVertices + 2);
        ApplyDeferredDrawArgs();
        Gfx_PsxStoreClearFlag(queryFlags);
        if ((queryFlags[0] & 0x60000) == 0) {
            g_SaferSephirothDeathShiftedVertices[2].vx = vertexSource[3].vx + ofsPtr->vx;
            g_SaferSephirothDeathShiftedVertices[2].vy = vertexSource[3].vy + ofsPtr->vy;
            g_SaferSephirothDeathShiftedVertices[2].vz = vertexSource[3].vz + ofsPtr->vz;
            SetPendingResourceHandle((GfxPsxShortVertexSlot *)&g_SaferSephirothDeathShiftedVertices[2]);
            ApplyPendingResourceHandle();
            Gfx_ReadTransformOrderingDepth2(&elementSize);
            quadCursor->tagWord = 0x9000000;
            quadCursor->colorCmd = primCursor->texQuad.colorCmd;
            quadCursor->texUv1 = primCursor->texQuad.texCoord1;
            quadCursor->texUv2 = primCursor->texQuad.texCoord2;
            quadCursor->texUv3 = primCursor->texQuad.texCoord3;
            quadCursor->attrMask = attrMask;
            if ((meshDraw->flags & BATTLE_FX_BZ_MORPH_FACE_GREY) != 0) {
                colorWord[0] = primCursor->texQuad.color & 0xFF;
                colorWord[0] = colorWord[0] << 0x10 | 0x2C000000 | colorWord[0] << 8 | colorWord[0];
                if (*shadeCursor != 0) {
                    Gfx_ShadePsxVertexColor((CVECTOR *)colorWord, *shadeCursor, &quadCursor->u.colorVec);
                    quadCursor->u.code.primCode = PSX_PRIM_POLY_FT4;
                } else {
                    quadCursor->u.color = colorWord[0];
                }
            } else {
                colorWord[0] = meshDraw->colorWord & 0xFF;
                quadCursor->u.color = colorWord[0] << 0x10 | 0x2C000000 | colorWord[0] << 8 | colorWord[0];
            }
            if ((meshDraw->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                quadCursor->u.code.primCode |= GFX_PSX_PRIM_CODE_SEMI_TRANSPARENT;
            }
            Gfx_DrawPsxPrimitive(quadCursor);
        }
        vertexSource += 4;
        ofsPtr++;
        shadeCursor++;
        primCursor = (DeadSefPrimStream *)((int)primCursor + 0x14);
    }
    count = primCursor->count;
    primCursor = (DeadSefPrimStream *)((int)primCursor + 4);
    triPacket = (BattleFxSaferSephirothDeathFragmentPacket *)quadCursor;
    for (index = 0; index < count; index++) {
        g_SaferSephirothDeathShiftedVertices->vx = vertexSource->vx + ofsPtr->vx;
        g_SaferSephirothDeathShiftedVertices->vy = vertexSource->vy + ofsPtr->vy;
        g_SaferSephirothDeathShiftedVertices->vz = vertexSource->vz + ofsPtr->vz;
        g_SaferSephirothDeathShiftedVertices[1].vx = vertexSource[1].vx + ofsPtr->vx;
        g_SaferSephirothDeathShiftedVertices[1].vy = vertexSource[1].vy + ofsPtr->vy;
        g_SaferSephirothDeathShiftedVertices[1].vz = vertexSource[1].vz + ofsPtr->vz;
        g_SaferSephirothDeathShiftedVertices[2].vx = vertexSource[2].vx + ofsPtr->vx;
        g_SaferSephirothDeathShiftedVertices[2].vy = vertexSource[2].vy + ofsPtr->vy;
        g_SaferSephirothDeathShiftedVertices[2].vz = vertexSource[2].vz + ofsPtr->vz;
        StoreDeferredDrawArgs(g_SaferSephirothDeathShiftedVertices, g_SaferSephirothDeathShiftedVertices + 1, g_SaferSephirothDeathShiftedVertices + 2);
        ApplyDeferredDrawArgs();
        vertexSource += 4;
        ofsPtr++;
        Gfx_PsxStoreClearFlag(queryFlags);
        if ((queryFlags[0] & 0x60000) == 0) {
            triPacket->tag = 0x6000000;
            Gfx_ReadTransformOrderingDepth2(&elementSize);
            if (*shadeCursor != 0) {
                Gfx_ShadePsxTriVertexColors(&primCursor->gouraudTri.colors.cvec[0], &primCursor->gouraudTri.colors.cvec[1], &primCursor->gouraudTri.colors.cvec[2], *shadeCursor, &triPacket->u.colorVec0, (CVECTOR *)&triPacket->colorCmd, (CVECTOR *)&triPacket->vertexColor2);
                triPacket->u.code.primCode = PSX_PRIM_POLY_G3;
            } else {
                triPacket->u.color0 = primCursor->gouraudTri.colors.words[0];
                triPacket->colorCmd = primCursor->gouraudTri.colors.words[1];
                triPacket->vertexColor2 = primCursor->gouraudTri.colors.words[2];
            }
            if ((meshDraw->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                triPacket->u.code.primCode |= GFX_PSX_PRIM_CODE_SEMI_TRANSPARENT;
            }
            Gfx_DrawPsxPrimitive(triPacket);
        }
        shadeCursor++;
        primCursor = (DeadSefPrimStream *)((int)primCursor + 0x14);
    }
    count = primCursor->count;
    primCursor = (DeadSefPrimStream *)((int)primCursor + 4);
    packetPtr = triPacket;
    for (index = 0; index < count; index++) {
        g_SaferSephirothDeathShiftedVertices->vx = vertexSource->vx + ofsPtr->vx;
        g_SaferSephirothDeathShiftedVertices->vy = vertexSource->vy + ofsPtr->vy;
        g_SaferSephirothDeathShiftedVertices->vz = vertexSource->vz + ofsPtr->vz;
        g_SaferSephirothDeathShiftedVertices[1].vx = vertexSource[1].vx + ofsPtr->vx;
        g_SaferSephirothDeathShiftedVertices[1].vy = vertexSource[1].vy + ofsPtr->vy;
        g_SaferSephirothDeathShiftedVertices[1].vz = vertexSource[1].vz + ofsPtr->vz;
        g_SaferSephirothDeathShiftedVertices[2].vx = vertexSource[2].vx + ofsPtr->vx;
        g_SaferSephirothDeathShiftedVertices[2].vy = vertexSource[2].vy + ofsPtr->vy;
        g_SaferSephirothDeathShiftedVertices[2].vz = vertexSource[2].vz + ofsPtr->vz;
        StoreDeferredDrawArgs(g_SaferSephirothDeathShiftedVertices, g_SaferSephirothDeathShiftedVertices + 1, g_SaferSephirothDeathShiftedVertices + 2);
        ApplyDeferredDrawArgs();
        Gfx_PsxStoreClearFlag(queryFlags);
        if ((queryFlags[0] & 0x60000) == 0) {
            g_SaferSephirothDeathShiftedVertices[2].vx = vertexSource[3].vx + ofsPtr->vx;
            g_SaferSephirothDeathShiftedVertices[2].vy = vertexSource[3].vy + ofsPtr->vy;
            g_SaferSephirothDeathShiftedVertices[2].vz = vertexSource[3].vz + ofsPtr->vz;
            SetPendingResourceHandle((GfxPsxShortVertexSlot *)&g_SaferSephirothDeathShiftedVertices[2]);
            ApplyPendingResourceHandle();
            Gfx_ReadTransformOrderingDepth2(&elementSize);
            packetPtr->tag = 0x8000000;
            if (*shadeCursor != 0) {
                Gfx_ShadePsxTriVertexColors(&primCursor->gouraudQuad.colors.cvec[0], &primCursor->gouraudQuad.colors.cvec[1], &primCursor->gouraudQuad.colors.cvec[2], *shadeCursor, &packetPtr->u.colorVec0, (CVECTOR *)&packetPtr->colorCmd, (CVECTOR *)&packetPtr->vertexColor2);
                Gfx_ShadePsxVertexColor(&primCursor->gouraudQuad.colors.cvec[3], *shadeCursor, (CVECTOR *)&packetPtr->vertexColor3);
                packetPtr->u.code.primCode = PSX_PRIM_POLY_G4;
            } else {
                packetPtr->u.color0 = primCursor->gouraudQuad.colors.words[0];
                packetPtr->colorCmd = primCursor->gouraudQuad.colors.words[1];
                packetPtr->vertexColor2 = primCursor->gouraudQuad.colors.words[2];
                packetPtr->vertexColor3 = primCursor->gouraudQuad.colors.words[3];
            }
            if ((meshDraw->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                packetPtr->u.code.primCode |= GFX_PSX_PRIM_CODE_SEMI_TRANSPARENT;
            }
            Gfx_DrawPsxPrimitive(packetPtr);
        }
        vertexSource += 4;
        ofsPtr++;
        shadeCursor++;
        primCursor = (DeadSefPrimStream *)((int)primCursor + 0x18);
    }
    *vertexOutPtr = vertexSource;
    *offsetCursorPtr = ofsPtr;
    *shadeCursorPtr = shadeCursor;
    return (int)packetPtr;
}

/*
 * Updates positions, rotation angles, velocities, and color shading of shatter
 * fragments over time.
 */
// FUNCTION: FF7 0x00454ce6
void __cdecl BattleFx_SaferSephirothDeathStepFragmentMotion(int frameCount)
{
    int fragIdx;
    int elapsed;
    int cornerIdx;

    if ((g_SaferSephirothDeath_FragmentStateInitDone & 1) == 0) {
        g_SaferSephirothDeath_FragmentStateInitDone |= 1;
        g_pSaferSephirothDeath_FragmentState = (struct BattleFxSefFragmentState *)(Gfx_GetPsxScratchpadPtr(0xC0));
    }
    g_pSaferSephirothDeath_FragmentState->delta.vy = 0;
    g_pSaferSephirothDeath_FragmentState->delta.vx = 0;
    g_pSaferSephirothDeath_FragmentState->delta.vz = 0;
    RotMatrix(&g_pSaferSephirothDeath_FragmentState->delta, &g_pSaferSephirothDeath_FragmentState->matrix);
    g_pSaferSephirothDeath_FragmentState->scaleY = 0x1000;
    g_pSaferSephirothDeath_FragmentState->scaleX = 0x1000;
    g_pSaferSephirothDeath_FragmentState->scaleZ = 0x1000;
    ScaleMatrix(&g_pSaferSephirothDeath_FragmentState->matrix, (VECTOR *)&g_pSaferSephirothDeath_FragmentState->scaleX);
    for (fragIdx = 0; fragIdx < 0x97C; fragIdx++) {
        elapsed = frameCount - g_SaferSephirothDeath_FragmentNormalBuf[fragIdx].startFrame;
        if (elapsed >= 0) {
            if (elapsed < 0x28) {
                g_SaferSephirothDeath_FragmentNormalBuf[fragIdx].deltaZ = (g_SaferSephirothDeath_FragmentNormalBuf[fragIdx].deltaZ * 0xF00 >> 0xC) - 5;
                g_SaferSephirothDeath_FragmentVertexBuf[fragIdx].vz += g_SaferSephirothDeath_FragmentNormalBuf[fragIdx].deltaZ;
                g_pSaferSephirothDeath_FragmentState->delta.vx = g_SaferSephirothDeath_FragmentVertexBuf[fragIdx].vx - (g_SaferSephirothDeath_BoneWorldPos.vx);
                g_pSaferSephirothDeath_FragmentState->delta.vy = g_SaferSephirothDeath_FragmentVertexBuf[fragIdx].vy - g_SaferSephirothDeath_BoneWorldPos.vy;
                TransformVectorToInt16(&g_pSaferSephirothDeath_FragmentState->matrix, &g_pSaferSephirothDeath_FragmentState->delta, &g_pSaferSephirothDeath_FragmentState->delta.vx);
                g_SaferSephirothDeath_FragmentVertexBuf[fragIdx].vx = g_pSaferSephirothDeath_FragmentState->delta.vx + (g_SaferSephirothDeath_BoneWorldPos.vx);
                g_SaferSephirothDeath_FragmentVertexBuf[fragIdx].vy = g_pSaferSephirothDeath_FragmentState->delta.vy + g_SaferSephirothDeath_BoneWorldPos.vy;
                for (cornerIdx = 0; cornerIdx < 4; cornerIdx++) {
                    g_SaferSephirothDeath_FragmentBuffer[fragIdx].corner[cornerIdx].vx = g_SaferSephirothDeath_FragmentBuffer[fragIdx].corner[cornerIdx].vx * 0xF00 >> 0xC;
                    g_SaferSephirothDeath_FragmentBuffer[fragIdx].corner[cornerIdx].vy = g_SaferSephirothDeath_FragmentBuffer[fragIdx].corner[cornerIdx].vy * 0xF00 >> 0xC;
                }
                g_SaferSephirothDeath_FragmentBuffer[fragIdx].corner[0].vz += 0x14;
                g_SaferSephirothDeath_FragmentBuffer[fragIdx].corner[2].vz -= 0x14;
                g_SaferSephirothDeath_FragmentColorBuf[fragIdx] = (elapsed << 0xC) / 0x28;
            } else {
                elapsed -= 0x28;
                if (elapsed < 0x14) {
                    g_SaferSephirothDeath_FragmentVertexBuf[fragIdx].vz -= 0x1F4;
                } else if (elapsed == 0x14) {
                    for (cornerIdx = 0; cornerIdx < 4; cornerIdx++) {
                        g_SaferSephirothDeath_FragmentBuffer[fragIdx].corner[cornerIdx].vz = 0;
                        g_SaferSephirothDeath_FragmentBuffer[fragIdx].corner[cornerIdx].vy = 0;
                        g_SaferSephirothDeath_FragmentBuffer[fragIdx].corner[cornerIdx].vx = 0;
                    }
                }
            }
        }
    }
}

/*
 * Steps through the screen flash frame threshold sequence, triggering white flash
 * ramps and fades at designated frames.
 */
// FUNCTION: FF7 0x0045500c
void __cdecl BattleFx_SaferSephirothDeathFlashSequenceTask(void)
{
    BattleFxSaferSephirothDeathFlashState *task;

    task = (BattleFxSaferSephirothDeathFlashState *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frame == 0) {
        task->stage = 0;
    }
    if (task->frame == g_SaferSephirothDeathFlashFrameThresholds[task->stage]) {
        BattleFx_StartScreenFlashRamp(2);
    }
    if (task->frame == g_SaferSephirothDeathFlashFrameThresholds[task->stage] + 2) {
        BattleFx_FadeOutScreenFlashRamp(6);
        (task->stage)++;
        if (g_SaferSephirothDeathFlashFrameThresholds[task->stage] == -1) {
            task->stage = 0xFFFF;
        }
    }
    (task->frame)++;
}

/*
 * Periodically spawns fading model sub-tasks with randomized angles and
 * offsets for LastBoss effect.
 */
// FUNCTION: FF7 0x004550d0
void __cdecl BattleFx_SaferSephirothDeathSpawnFadingModelsTask(void)
{
    BattleFxSaferSephirothDeathFadingTask *modelPtr;
    int unusedSpace[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleTaskSlot *taskPtr;
    int unusedField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int scaleValue;
    int yOfs;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_SaferSephirothDeath_SubTaskCount += 0xA0;
    while (g_SaferSephirothDeath_SubTaskCount >= 0x100) {
        modelPtr = (BattleFxSaferSephirothDeathFadingTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SaferSephirothDeathFadingModelTask);
        modelPtr->rot.vx = 0;
        modelPtr->rot.vy = 0;
        g_SaferSephirothDeath_TaskFinished = g_SaferSephirothDeath_TaskFinished + (rand() & 0x7FF) + 0x400;
        modelPtr->rot.vz = (short)g_SaferSephirothDeath_TaskFinished;
        if (DEAD_IN_RETAIL != 0) {
            scaleValue = 0x800;
        } else {
            scaleValue = rand() % 0x400 + 0x400;
        }
        modelPtr->scaleX = scaleValue;
        if (DEAD_IN_RETAIL != 0) {
            yOfs = 0x12C;
        } else {
            yOfs = rand() % 0xC8 + 0x64;
        }
        modelPtr->offsetY = -yOfs;
        g_SaferSephirothDeath_SubTaskCount -= 0x100;
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter >= 0x8C) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates view-space transformation, calculates alpha fade curve, and submits
 * fading black silhouette model draw request.
 */
// FUNCTION: FF7 0x00455219
void __cdecl BattleFx_SaferSephirothDeathFadingModelTask(void)
{
    BattleFxSaferSephirothDeathFadingTask *taskPtr;
    int time;
    int fade;
    int ratio;

    taskPtr = (BattleFxSaferSephirothDeathFadingTask *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_SaferSephirothDeath_CameraScriptOperandPool.initDone & 1) == 0) {
        g_SaferSephirothDeath_CameraScriptOperandPool.initDone |= 1;
        g_SaferSephirothDeath_CameraScriptOperandPool.u.fadeBlock = (struct SaferSephirothDeathFadeBlock *)Gfx_GetPsxScratchpadPtr(0);
    }
    time = taskPtr->frame;
    if (time < 0x14) {
        ratio = (time << 0xC) / 0x14;
        fade = 0x1000 - ratio;
    } else {
        time -= 0x14;
        if (time < 5) {
            fade = 0;
        } else {
            time -= 5;
            if (time < 0x14) {
                ratio = (time << 0xC) / 0x14;
                fade = ratio;
            } else {
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    BattleFx_MakeViewSpacePointPushed(&g_SaferSephirothDeath_ActorPos, 0, (Vec3sPacked *)&g_SaferSephirothDeathViewTransform);
    RotMatrix(&taskPtr->rot, &g_SaferSephirothDeath_CameraScriptOperandPool.u.fadeBlock->matrix);
    g_SaferSephirothDeath_CameraScriptOperandPool.u.fadeBlock->matrix.t[2] = 0;
    g_SaferSephirothDeath_CameraScriptOperandPool.u.fadeBlock->matrix.t[1] = 0;
    g_SaferSephirothDeath_CameraScriptOperandPool.u.fadeBlock->matrix.t[0] = 0;
    MultiplyMatrix(&g_SaferSephirothDeathViewTransform, &g_SaferSephirothDeath_CameraScriptOperandPool.u.fadeBlock->matrix, &g_SaferSephirothDeath_CameraScriptOperandPool.u.fadeBlock->matrix);
    g_SaferSephirothDeathSecondaryTransform.m[0][0] = taskPtr->scaleX;
    g_SaferSephirothDeathSecondaryTransform.t[1] = taskPtr->offsetY;
    MultiplyMatrix(&g_SaferSephirothDeath_CameraScriptOperandPool.u.fadeBlock->matrix, &g_SaferSephirothDeathSecondaryTransform, &g_SaferSephirothDeath_CameraScriptOperandPool.u.fadeBlock->matrix);
    SetRotMatrix(&g_SaferSephirothDeath_CameraScriptOperandPool.u.fadeBlock->matrix);
    SetTransMatrix(&g_SaferSephirothDeath_CameraScriptOperandPool.u.fadeBlock->matrix);
    BattleFx_SaferSephirothDeathSubmitFadingModelDraw(fade);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frame)++;
    }
}

/*
 * Submits the black silhouette model draw request to the ordering table with
 * specified alpha transparency.
 */
// FUNCTION: FF7 0x004553d7
void __cdecl BattleFx_SaferSephirothDeathSubmitFadingModelDraw(int alpha)
{
    (g_SaferSephirothDeathBlackModelDrawRequest.model) = Gfx_ModelTableGetRenderObject(1, g_SaferSephirothDeath_Loaded);
    SetDrawColorRGB(0, 0, 0);
    g_SaferSephirothDeathBlackModelDrawRequest.fadeLevel = alpha;
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_SaferSephirothDeathBlackModelDrawRequest, g_BattleCurrentOrderingTable + 0x78, 0, g_BattleOverlayDrawListHead);
}

/*
 * Periodically spawns rotating billboard sprite sub-tasks with randomized
 * coordinates around the target bone.
 */
// FUNCTION: FF7 0x0045542e
void __cdecl BattleFx_SaferSephirothDeathSpawnBillboardsTask(void)
{
    int unusedSpace[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleTaskSlot *taskPtr;
    int unusedField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSaferSephirothDeathBillboardRecord *sprite;
    int angle;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_SaferSephirothDeath_BillboardSpawnAccum += 0x80;
    while (g_SaferSephirothDeath_BillboardSpawnAccum >= 0x100) {
        sprite = (BattleFxSaferSephirothDeathBillboardRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SaferSephirothDeathBillboardTask);
        sprite->posX = rand() % 0x640 + (g_SaferSephirothDeath_BoneWorldPos.vx) + -0x320;
        sprite->posY = rand() % 0x640 + g_SaferSephirothDeath_BoneWorldPos.vy + -0x320;
        sprite->posZ = rand() % 0x640 + g_SaferSephirothDeath_BoneWorldPos.vz + -0x320;
        if (DEAD_IN_RETAIL != 0) {
            angle = 0x800;
        } else {
            angle = rand() % 0x800;
        }
        sprite->startValue = angle;
        g_SaferSephirothDeath_BillboardSpawnAccum -= 0x100;
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter >= 0x8C) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Computes scaling with camera easing, installs billboard transform matrix,
 * and submits billboard model draw request.
 */
// FUNCTION: FF7 0x0045556b
void __cdecl BattleFx_SaferSephirothDeathBillboardTask(void)
{
    int scale;
    int frame;
    BattleFxSaferSephirothDeathBillboardRecord *task;
    int ratio;
    int easedValue;

    task = (BattleFxSaferSephirothDeathBillboardRecord *)BATTLE_CURRENT_TASK_SLOT;
    g_SaferSephirothDeathBillboardDrawRequest.modelGroup = Gfx_ModelTableGetRenderObject(0, g_SaferSephirothDeath_Loaded);
    frame = task->frameCount;
    if (frame < 2) {
        ratio = (frame << 0xC) / 2;
        easedValue = BattleFx_SaferSephirothDeathApplyCameraEasing(ratio, 0x1FF);
        scale = task->startValue * easedValue >> 0xC;
    } else {
        frame -= 2;
        if (frame < 2) {
            ratio = (frame << 0xC) / 2;
            easedValue = BattleFx_SaferSephirothDeathApplyCameraEasing(ratio, 0xFFFFFE01);
            scale = task->startValue + ((0 - task->startValue) * easedValue >> 0xC);
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    BattleFx_InstallBillboardMatrix(&task->posX, (short)scale, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_SaferSephirothDeathBillboardDrawRequest, g_BattleCurrentOrderingTable + 0x74, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCount)++;
    }
}

/*
 * Ramps screen brightness to full white over 60 frames, maintaining full white flash
 * thereafter until retired.
 */
// FUNCTION: FF7 0x00455695
void __cdecl BattleFx_SaferSephirothDeathWhiteFlashRampTask(void)
{
    int frame;
    struct BattleTaskSlot *task;
    int ratio;
    int shade;

    task = (struct BattleTaskSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    frame = task->frameCounter;
    if (frame < 0x3C) {
        ratio = (frame << 0xC) / 0x3C;
        shade = ratio * 0xFF >> 0xC;
    } else {
        shade = 0xFF;
    }
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite((char)shade, (char)shade, (char)shade);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/*
 * Loads ENEMY331.DAT from disk, extracts model part data blocks, and builds
 * an allocated table of model fragment pointers.
 */
// FUNCTION: FF7 0x00455720
int __cdecl BattleFx_SaferSephirothDeathLoadEnemy331ModelTable(void)
{
    int size;
    int unusedSpace[35]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unusedField[58]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unusedHole[7]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int entryIdx;
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    DeadSefDatStream *startPtr;
    DeadSefDatStream *readPointer;
    int blockOfs;
    int partCount;
    int partBase;
    int *partTable;
    int unit;
    int unusedValue[50]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int baseFlag;
    int entryCnt;
    int *hdrEnd;
    int *baseOffsetPtr;
    int dataBuf;
    int dirState[51];

    Battle_PushDataDirectory(dirState);
    baseFlag = 1;
    // STRING: FF7 0x007e2eb8
    g_SaferSephirothDeath_EnemyDatHandle = (int)LoadDiskFile(&size, "ENEMY331.DAT");
    if (g_SaferSephirothDeath_EnemyDatHandle != 0) {
        dataBuf = g_SaferSephirothDeath_EnemyDatHandle;
        hdrEnd = (int *)dataBuf;
        entryCnt = *hdrEnd;
        hdrEnd++;
        baseOffsetPtr = hdrEnd;
        hdrEnd += entryCnt;
        unit = 1;
        partBase = dataBuf + *baseOffsetPtr * unit;
        readPointer = (DeadSefDatStream *)partBase;
        partCount = readPointer->dword;
        readPointer = (DeadSefDatStream *)((int)readPointer + 4);
        if (partCount > 0) {
            // STRING: FF7 0x007e2ec8
            partTable = (int *)GameHeapAlloc(partCount, 4, "C:\\FF7\\src\\battle\\yasui\\deadsef.cpp", 0x342);
        } else {
            // STRING: FF7 0x007e2eec
            partTable = (int *)GameHeapAlloc(partCount + 1, 4, "C:\\FF7\\src\\battle\\yasui\\deadsef.cpp", 0x346);
        }
        if (partCount > 0) {
            readPointer = (DeadSefDatStream *)((int)readPointer + 2);
            readPointer = (DeadSefDatStream *)((int)readPointer + 2);
            blockOfs = readPointer->dword;
            readPointer = (DeadSefDatStream *)((int)readPointer + 4);
            startPtr = readPointer;
            for (entryIdx = 0; entryIdx < partCount; entryIdx++) {
                readPointer = (DeadSefDatStream *)((int)readPointer + 2);
                readPointer = (DeadSefDatStream *)((int)readPointer + 2);
                blockOfs = readPointer->dword;
                readPointer = (DeadSefDatStream *)((int)readPointer + 4);
                if (blockOfs != 0) {
                    partTable[entryIdx] = partBase + blockOfs;
                }
            }
        } else {
            readPointer = (DeadSefDatStream *)((int)readPointer + 2);
            readPointer = (DeadSefDatStream *)((int)readPointer + 2);
            blockOfs = readPointer->dword;
            readPointer = (DeadSefDatStream *)((int)readPointer + 4);
            if (blockOfs != 0) {
                *partTable = partBase + blockOfs;
            }
        }
    }
    Battle_PopDataDirectory(dirState);
    return (int)partTable;
}
} /* extern "C" */
