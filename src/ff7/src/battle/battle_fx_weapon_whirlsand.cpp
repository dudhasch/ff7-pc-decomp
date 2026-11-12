// src/ff7/src/battle/battle_fx_weapon_whirlsand.cpp
/* Enemy Special Attack [149]: Ruby Weapon Whirlsand (orig. WEAPON/WEAPON2) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the second weapon attack: a camera-script interpreter, a rippling
//   ring-grid mesh with its height envelope and textured or shaded drawing, and surface particle
//   and actor spiral tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/graphics.h"
#include "ff7/enums/battle.h"
#include "lib/codegen.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/camera_script.h"
#include "lib/enums/graphics.h"
#include "lib/api/sound.h"
extern "C" {

#pragma intrinsic(memset)
// clang-format off
// GLOBAL: FF7 0x007e2b60
unsigned char g_WhirlsandSoundGroup[16] = {0x05, 0x01, 0x00, 0x00, 0x7f, 0x01, 0x00, 0x00, 0x7e, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007e2b70
struct SoundGroup *g_WhirlsandSoundGroupPointerList[2] = {(struct SoundGroup *)g_WhirlsandSoundGroup, 0};

// GLOBAL: FF7 0x007e2b78
int g_WhirlsandSpriteResourceRecords[10] = {
    1669180,
    (int)&g_WhirlsandTexturePath_ANT1,
    0,
    0,
    0,
    1669177,
    // STRING: FF7 0x007e2c8c
    (int)"ANT1.TIM",
    0,
    0,
    0
};

// GLOBAL: FF7 0x007e2ba0
int g_WhirlsandStageSwirlOffscreenSlotIndex = (-1);

// GLOBAL: FF7 0x007e2ba8
short g_WhirlsandEyeFocusCameraScript[41] = {137, 4, 73, 4, 65, (-3000), (-7000), 10000, 129, 0, 0, 1000, 64, 30, 128, 30, 66, 60, (-8000), (-3000), (-2000), 130, 60, 0, 1000, 0, 66, 35, (-10000), (-5000), (-2000), 130, 35, 0, 0, 0, 64, 10, 128, 10, (-1)};

// GLOBAL: FF7 0x007e2bfc
BattleFxSwirlGrid *g_WhirlsandStageSwirlGridBasePointer = (BattleFxSwirlGrid *)10519232;

// GLOBAL: FF7 0x007e2c00
PsxMatrix g_WhirlsandStageSwirlBaseMatrix = {{{1024, 0, 0}, {0, 0, (-2048)}, {0, 1024, 0}}, {0, 0, 8192}};

// GLOBAL: FF7 0x007e2c20
PsxMatrix g_WhirlsandSceneMeshTransform = {{{4096, 0, 0}, {0, 4096, 0}, {0, 0, 4096}}, {0, 0, 0}};

// GLOBAL: FF7 0x007e2c40
struct BattleFxWhirlsandStageRequestArgs g_WhirlsandSceneMeshDrawRectangle = {896, 0, 128, 255};

// GLOBAL: FF7 0x007e2c48
unsigned char g_WhirlsandSceneMeshDrawDescriptor[16] = {0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007e2c58
int g_WhirlsandTransientQuadColorCommand = 0x28408080;

unsigned char g_rawData_007e2c60[32] = {
    0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// GLOBAL: FF7 0x007e2c80
unsigned char g_WhirlsandTexturePath_ANT1[9] = "ANT1.TIM";

// GLOBAL: FF7 0x009f39d0
BattleFxWhirlsandParticleScratch *g_WhirlsandDataBlock0 = 0;

// GLOBAL: FF7 0x009f39d4
BattleFxWhirlsandRideScratch *g_WhirlsandDataBlock1 = 0;

unsigned char g_bssPad_009f39d8[20] = {0};

// GLOBAL: FF7 0x009f39ec
BattleFxWhirlsandGridScratch *g_WhirlsandDataBlock2 = 0;

// GLOBAL: FF7 0x009f39f0
int g_WhirlsandScale = 0;

unsigned char g_bssPad_009f39f4[4] = {0};

// GLOBAL: FF7 0x009f39f8
CameraScriptTrack g_WhirlsandCameraFocus = {0};

unsigned char g_bssPad_009f3a24[4] = {0};

unsigned char g_bssPad_009f3a28[83968] = {0};

// GLOBAL: FF7 0x00a08228
unsigned char g_WhirlsandDataBlock0InitFlag = 0;

// GLOBAL: FF7 0x00a0822c
BattleFxWhirlsandGridScratch *g_WhirlsandDataBlock3 = 0;

// GLOBAL: FF7 0x00a08230
short g_WhirlsandAngle = 0;

// GLOBAL: FF7 0x00a08234
unsigned char g_WhirlsandDataBlock5InitFlag = 0;

// GLOBAL: FF7 0x00a08238
int g_WhirlsandDataBlock4 = 0;

// GLOBAL: FF7 0x00a0823c
CameraScriptTrack *g_pWhirlsandCameraTrack = 0;

// GLOBAL: FF7 0x00a08240
PsxMatrix g_WhirlsandCompositeMatrix = {0};

// GLOBAL: FF7 0x00a08260
CameraScriptTrack *g_pWhirlsandCameraOther = 0;

// GLOBAL: FF7 0x00a08264
unsigned char g_WhirlsandDataBlock2InitFlag = 0;

// GLOBAL: FF7 0x00a08268
PsxMatrix g_WhirlsandBaseMatrix = {0};

// GLOBAL: FF7 0x00a08288
unsigned char g_WhirlsandDataBlock3InitFlag = 0;

unsigned char g_bssPad_00a0828c[4] = {0};

// GLOBAL: FF7 0x00a08290
CameraScriptTrack g_WhirlsandCameraEye = {0};

unsigned char g_bssPad_00a082bc[4] = {0};

// GLOBAL: FF7 0x00a082c0
unsigned char g_WhirlsandStageSwirlGridBuffer[86212] = {0};

// GLOBAL: FF7 0x00a1d384
unsigned char g_WhirlsandDataBlock4InitFlag = 0;

// GLOBAL: FF7 0x00a1d388
struct BattleFxWhirlsandSpiralOffset *g_WhirlsandDataBlock5 = 0;

unsigned char g_bssPad_00a1d38c[4] = {0};

// GLOBAL: FF7 0x00a1d390
short g_WhirlsandRotationVector[3] = {0};

// GLOBAL: FF7 0x00a1d398
unsigned char g_WhirlsandDataBlock1InitFlag = 0;

// GLOBAL: FF7 0x00a1d39c
struct GfxTableCursor *g_WhirlsandResourceHandle = 0;

// GLOBAL: FF7 0x00a1d3a0
int g_WhirlsandParticleRowIndex = 0;

// GLOBAL: FF7 0x00a1d3a4
int g_WhirlsandParticleSlotIndex = 0;

// GLOBAL: FF7 0x00a1d3a8
short g_WhirlsandModelRotationAngles1[3] = {0};

// GLOBAL: FF7 0x00a1d3b0
short g_WhirlsandModelRotationAngles2[3] = {0};

// GLOBAL: FF7 0x00a1d3b8
short g_WhirlsandActorOffsetAngles[3] = {0};

// GLOBAL: FF7 0x00a1d3c0
int g_WhirlsandEmissionAccumulator = 0;

unsigned char g_bssPad_00a1d3c4[4] = {0};
// clang-format on


// Native early group ends after451CA4; the next original group begins on 16-byte alignment.
// When the battle scene exists and g_WhirlsandResourceHandle is zero, initializes a default-duration resource
// record with type zero, builds WEAPON/WEAPON2 into the local path buffer and calls
// Gfx_LoadModelTableIfAbsent with resource ID 2, g_WhirlsandSpriteResourceRecords metadata, the global output slot and
// scene context at 0x4C. Then registers sound groups from g_WhirlsandSoundGroupPointerList. Otherwise does nothing.
// FUNCTION: FF7 0x0044ec10
void __cdecl BattleFx_WeaponLoadResWhirlsand(void)
{
    GfxModelTableBank *sceneContext;
    GfxResourceRequest requestBuffer;
    int resultId;
    char pathSize[200];
    GfxResourceRequest *pReq;
    int defsPtr;

    if (g_pBattle3dScene != 0 && g_WhirlsandResourceHandle == 0) {
        resultId = 2;
        defsPtr = (int)g_WhirlsandSpriteResourceRecords;
        sceneContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestBuffer;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007e2c98
        pReq->resourcePath = (char *)BattleFx_BuildResourcePath("WEAPON/WEAPON2", pathSize);
        Gfx_LoadModelTableIfAbsent(resultId, defsPtr, pReq, &g_WhirlsandResourceHandle, sceneContext);
        SfxRegisterSoundGroups(g_WhirlsandSoundGroupPointerList);
    }
}

// Clears three g_WhirlsandRotationVector entries and 30 bytes at g_WhirlsandBaseMatrix, loads Weapon2 resources, then
// calls BattleFx_WeaponBeginWhirlsand with the arguments.
// FUNCTION: FF7 0x0044ecb1
void __cdecl BattleFx_WeaponEnterWhirlsand(int targetMask, int actorIdx)
{
    g_WhirlsandRotationVector[0] = g_WhirlsandRotationVector[1] = g_WhirlsandRotationVector[2] = 0;
    memset(&g_WhirlsandBaseMatrix, 0, 30);
    BattleFx_WeaponLoadResWhirlsand();
    BattleFx_WeaponBeginWhirlsand(targetMask, actorIdx);
}

// Stores angle in g_WhirlsandAngle, writes -2000 to g_WhirlsandBaseMatrix.t[2], builds the rotation matrix, and
// consumes rand() before two identical BattleFx_WhirlsandSpawnCameraScript branches; then allocates BattleFx_WhirlsandTimelineTask and sets
// g_BattleScriptEffectFlag to 1.
// FUNCTION: FF7 0x0044ed00
void __cdecl BattleFx_WeaponBeginWhirlsand(int targetMask, int actorIdx)
{
    int padSlot[3]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    g_WhirlsandAngle = targetMask;
    g_WhirlsandBaseMatrix.t[2] = 0xFFFFF830;
    RotMatrix((SVECTOR *)g_WhirlsandRotationVector, &g_WhirlsandBaseMatrix);
    if ((rand() & 0x100) != 0) {
        BattleFx_WhirlsandSpawnCameraScript(g_WhirlsandEyeFocusCameraScript, &g_WhirlsandBaseMatrix, actorIdx);
    } else {
        BattleFx_WhirlsandSpawnCameraScript(g_WhirlsandEyeFocusCameraScript, &g_WhirlsandBaseMatrix, actorIdx);
    }
    Battle_AllocateResourceSlot(BattleFx_WhirlsandTimelineTask);
    g_BattleScriptEffectFlag = 1;
}

// Allocates a 0x20-byte task slot that runs BattleFx_WhirlsandRunCameraScript and fills in its
// record. cameraScript and matrix are stored as 32-bit values, and param as a 16-bit value.
// FUNCTION: FF7 0x0044ed80
void __cdecl BattleFx_WhirlsandSpawnCameraScript(short *cameraScript, PsxMatrix *matrix, int param)
{
    BattleFxWhirlsandCameraTask *entry;

    entry = (BattleFxWhirlsandCameraTask *)BATTLE_NEW_TASK_SLOT(BattleFx_WhirlsandRunCameraScript);
    entry->scriptPos = cameraScript;
    entry->rotMatrix = matrix;
    entry->actor = param;
}

// Interprets a shared short camera script for eye/focus tracks. While unpaused initializes their
// counters/attachment selectors on task count zero, then reads commands while either track counter
// is zero: mask 0x40 chooses eye and masks 0xC0 are removed from the command. Supports waits
// through BattleFx_WhirlsandReadCameraScriptOperand, absolute triples, linear target motion, changing
// distance from the other track, acceleration computed from target/duration squared, shaped
// interpolation through BattleFx_WhirlsandApplyCameraEasing, attachment subtraction/restoration
// through BattleFx_WhirlsandGetActorLocalPosition, g_BattleCameraScriptValue writes and command nine
// suppressing output; unknown commands retire and return immediately. Updates both tracks in
// eye-then-focus order, retaining case-four fallthrough into linear motion, then decrements both
// counters regardless of command. Publishes each nonsuppressed track directly as eight bytes when
// no task transform exists, otherwise transforms its triple with optional attachment offset into
// the camera globals. Paused calls skip script/track advancement but still publish retained
// positions.
// FUNCTION: FF7 0x0044edc0
void __cdecl BattleFx_WhirlsandRunCameraScript(void)
{
    BattleFxWhirlsandCameraTask *cameraScriptPtr;
    int tmpPtr;
    int stackWords[46]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int stackGap[6]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int slotValue[14]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    Vec3sPacked *otherOffset;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short opcode;
    BattleCameraScriptVec *buf;
    Vec3sPacked *offset;
    int eased;
    int trackIndex;
    int bits;

    cameraScriptPtr = (BattleFxWhirlsandCameraTask *)BATTLE_CURRENT_TASK_SLOT;
    offset = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    otherOffset = (Vec3sPacked *)((int)offset + 8);
    buf = (BattleCameraScriptVec *)((int)otherOffset + 8);
    tmpPtr = (int)buf + 0x10;
    if (g_BattleIsPaused == 0) {
        if (cameraScriptPtr->started == 0) {
            g_WhirlsandCameraEye.frames = 0;
            g_WhirlsandCameraFocus.frames = 0;
            g_WhirlsandCameraEye.actor = -1;
            g_WhirlsandCameraFocus.actor = -1;
            cameraScriptPtr->started = 1;
        }
        while (g_WhirlsandCameraEye.frames == 0 || g_WhirlsandCameraFocus.frames == 0) {
            opcode = *cameraScriptPtr->scriptPos;
            cameraScriptPtr->scriptPos += 1;
            if ((opcode & SUMMON_CAMERA_OP_EYE) != 0) {
                g_pWhirlsandCameraTrack = &g_WhirlsandCameraEye;
                g_pWhirlsandCameraOther = &g_WhirlsandCameraFocus;
            } else {
                g_pWhirlsandCameraTrack = &g_WhirlsandCameraFocus;
                g_pWhirlsandCameraOther = &g_WhirlsandCameraEye;
            }
            g_pWhirlsandCameraTrack->mode = opcode & ~0xC0;
            switch (g_pWhirlsandCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                g_pWhirlsandCameraTrack->frames = BattleFx_WhirlsandReadCameraScriptOperand(&cameraScriptPtr->scriptPos);
                break;
            case SUMMON_CAMERA_SET_POSITION:
                g_pWhirlsandCameraTrack->position.x = *cameraScriptPtr->scriptPos;
                cameraScriptPtr->scriptPos += 1;
                g_pWhirlsandCameraTrack->position.y = *cameraScriptPtr->scriptPos;
                cameraScriptPtr->scriptPos += 1;
                g_pWhirlsandCameraTrack->position.z = *cameraScriptPtr->scriptPos;
                cameraScriptPtr->scriptPos += 1;
                break;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_pWhirlsandCameraTrack->frames = BattleFx_WhirlsandReadCameraScriptOperand(&cameraScriptPtr->scriptPos);
                g_pWhirlsandCameraTrack->velocity.x = (*cameraScriptPtr->scriptPos - g_pWhirlsandCameraTrack->position.x) / g_pWhirlsandCameraTrack->frames;
                cameraScriptPtr->scriptPos += 1;
                g_pWhirlsandCameraTrack->velocity.y = (*cameraScriptPtr->scriptPos - g_pWhirlsandCameraTrack->position.y) / g_pWhirlsandCameraTrack->frames;
                cameraScriptPtr->scriptPos += 1;
                g_pWhirlsandCameraTrack->velocity.z = (*cameraScriptPtr->scriptPos - g_pWhirlsandCameraTrack->position.z) / g_pWhirlsandCameraTrack->frames;
                cameraScriptPtr->scriptPos += 1;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                offset->vx = g_pWhirlsandCameraOther->position.x - g_pWhirlsandCameraTrack->position.x;
                offset->vy = g_pWhirlsandCameraOther->position.y - g_pWhirlsandCameraTrack->position.y;
                offset->vz = g_pWhirlsandCameraOther->position.z - g_pWhirlsandCameraTrack->position.z;
                g_pWhirlsandCameraTrack->distance = SqrtToInt(offset->vx * offset->vx + offset->vy * offset->vy + offset->vz * offset->vz);
                g_pWhirlsandCameraTrack->base = g_pWhirlsandCameraTrack->position;
                g_pWhirlsandCameraTrack->frames = BattleFx_WhirlsandReadCameraScriptOperand(&cameraScriptPtr->scriptPos);
                offset->vx = *cameraScriptPtr->scriptPos;
                cameraScriptPtr->scriptPos += 1;
                offset->vy = *cameraScriptPtr->scriptPos;
                cameraScriptPtr->scriptPos += 1;
                offset->vz = *cameraScriptPtr->scriptPos;
                cameraScriptPtr->scriptPos += 1;
                g_pWhirlsandCameraTrack->velocity.x = (offset->vx - g_pWhirlsandCameraTrack->position.x) / g_pWhirlsandCameraTrack->frames;
                g_pWhirlsandCameraTrack->velocity.y = (offset->vy - g_pWhirlsandCameraTrack->position.y) / g_pWhirlsandCameraTrack->frames;
                g_pWhirlsandCameraTrack->velocity.z = (offset->vz - g_pWhirlsandCameraTrack->position.z) / g_pWhirlsandCameraTrack->frames;
                switch (g_pWhirlsandCameraOther->mode) {
                case SUMMON_CAMERA_MOVE_LINEAR:
                    otherOffset->vx = g_pWhirlsandCameraOther->position.x + g_pWhirlsandCameraOther->velocity.x * g_pWhirlsandCameraTrack->frames;
                    otherOffset->vy = g_pWhirlsandCameraOther->position.y + g_pWhirlsandCameraOther->velocity.y * g_pWhirlsandCameraTrack->frames;
                    otherOffset->vz = g_pWhirlsandCameraOther->position.z + g_pWhirlsandCameraOther->velocity.z * g_pWhirlsandCameraTrack->frames;
                    break;
                default:
                    otherOffset->vx = g_pWhirlsandCameraOther->position.x;
                    otherOffset->vy = g_pWhirlsandCameraOther->position.y;
                    otherOffset->vz = g_pWhirlsandCameraOther->position.z;
                }
                offset->vx -= otherOffset->vx;
                offset->vy -= otherOffset->vy;
                offset->vz -= otherOffset->vz;
                g_pWhirlsandCameraTrack->delta = (SqrtToInt(offset->vx * offset->vx + offset->vy * offset->vy + offset->vz * offset->vz) - g_pWhirlsandCameraTrack->distance) / g_pWhirlsandCameraTrack->frames;
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_pWhirlsandCameraTrack->frames = BattleFx_WhirlsandReadCameraScriptOperand(&cameraScriptPtr->scriptPos);
                offset->vx = *cameraScriptPtr->scriptPos - g_pWhirlsandCameraTrack->position.x;
                cameraScriptPtr->scriptPos += 1;
                offset->vy = *cameraScriptPtr->scriptPos - g_pWhirlsandCameraTrack->position.y;
                cameraScriptPtr->scriptPos += 1;
                offset->vz = *cameraScriptPtr->scriptPos - g_pWhirlsandCameraTrack->position.z;
                cameraScriptPtr->scriptPos += 1;
                g_pWhirlsandCameraTrack->base.x = (offset->vx - g_pWhirlsandCameraTrack->velocity.x * g_pWhirlsandCameraTrack->frames << 1) / (g_pWhirlsandCameraTrack->frames * g_pWhirlsandCameraTrack->frames);
                g_pWhirlsandCameraTrack->base.y = (offset->vy - g_pWhirlsandCameraTrack->velocity.y * g_pWhirlsandCameraTrack->frames << 1) / (g_pWhirlsandCameraTrack->frames * g_pWhirlsandCameraTrack->frames);
                g_pWhirlsandCameraTrack->base.z = (offset->vz - g_pWhirlsandCameraTrack->velocity.z * g_pWhirlsandCameraTrack->frames << 1) / (g_pWhirlsandCameraTrack->frames * g_pWhirlsandCameraTrack->frames);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_pWhirlsandCameraTrack->frames = BattleFx_WhirlsandReadCameraScriptOperand(&cameraScriptPtr->scriptPos);
                g_pWhirlsandCameraTrack->easing = *cameraScriptPtr->scriptPos;
                cameraScriptPtr->scriptPos += 1;
                g_pWhirlsandCameraTrack->distance = 0;
                g_pWhirlsandCameraTrack->delta = 0x1000 / g_pWhirlsandCameraTrack->frames;
                g_pWhirlsandCameraTrack->base.x = g_pWhirlsandCameraTrack->position.x;
                g_pWhirlsandCameraTrack->base.y = g_pWhirlsandCameraTrack->position.y;
                g_pWhirlsandCameraTrack->base.z = g_pWhirlsandCameraTrack->position.z;
                g_pWhirlsandCameraTrack->velocity.x = *cameraScriptPtr->scriptPos - g_pWhirlsandCameraTrack->position.x;
                cameraScriptPtr->scriptPos += 1;
                g_pWhirlsandCameraTrack->velocity.y = *cameraScriptPtr->scriptPos - g_pWhirlsandCameraTrack->position.y;
                cameraScriptPtr->scriptPos += 1;
                g_pWhirlsandCameraTrack->velocity.z = *cameraScriptPtr->scriptPos - g_pWhirlsandCameraTrack->position.z;
                cameraScriptPtr->scriptPos += 1;
                break;
            case SUMMON_CAMERA_ATTACH_BONE:
                if ((g_pWhirlsandCameraTrack->actor = *cameraScriptPtr->scriptPos++) == 0) {
                    g_pWhirlsandCameraTrack->actor = cameraScriptPtr->actor;
                }
                g_pWhirlsandCameraTrack->bone = *cameraScriptPtr->scriptPos;
                cameraScriptPtr->scriptPos += 1;
                BattleFx_WhirlsandGetActorLocalPosition(g_pWhirlsandCameraTrack->actor, g_pWhirlsandCameraTrack->bone, cameraScriptPtr->rotMatrix, offset);
                g_pWhirlsandCameraTrack->position.x -= offset->vx;
                g_pWhirlsandCameraTrack->position.y -= offset->vy;
                g_pWhirlsandCameraTrack->position.z -= offset->vz;
                break;
            case SUMMON_CAMERA_DETACH:
                BattleFx_WhirlsandGetActorLocalPosition(g_pWhirlsandCameraTrack->actor, g_pWhirlsandCameraTrack->bone, cameraScriptPtr->rotMatrix, offset);
                g_pWhirlsandCameraTrack->position.x += offset->vx;
                g_pWhirlsandCameraTrack->position.y += offset->vy;
                g_pWhirlsandCameraTrack->position.z += offset->vz;
                g_pWhirlsandCameraTrack->actor = -1;
                break;
            case SUMMON_CAMERA_WAIT_NO_WRITEBACK:
                g_pWhirlsandCameraTrack->frames = BattleFx_WhirlsandReadCameraScriptOperand(&cameraScriptPtr->scriptPos);
                break;
            case SUMMON_CAMERA_SET_SCRIPT_VALUE:
                g_BattleCameraScriptValue = *cameraScriptPtr->scriptPos;
                cameraScriptPtr->scriptPos += 1;
                break;
            default:
                cameraScriptPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        g_pWhirlsandCameraTrack = &g_WhirlsandCameraEye;
        g_pWhirlsandCameraOther = &g_WhirlsandCameraFocus;
        for (trackIndex = 0; trackIndex < 2; trackIndex++) {
            switch (g_pWhirlsandCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_pWhirlsandCameraTrack->velocity.x += g_pWhirlsandCameraTrack->base.x;
                g_pWhirlsandCameraTrack->velocity.y += g_pWhirlsandCameraTrack->base.y;
                g_pWhirlsandCameraTrack->velocity.z += g_pWhirlsandCameraTrack->base.z;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_pWhirlsandCameraTrack->position.x += g_pWhirlsandCameraTrack->velocity.x;
                g_pWhirlsandCameraTrack->position.y += g_pWhirlsandCameraTrack->velocity.y;
                g_pWhirlsandCameraTrack->position.z += g_pWhirlsandCameraTrack->velocity.z;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                g_pWhirlsandCameraTrack->base.x += g_pWhirlsandCameraTrack->velocity.x;
                g_pWhirlsandCameraTrack->base.y += g_pWhirlsandCameraTrack->velocity.y;
                g_pWhirlsandCameraTrack->base.z += g_pWhirlsandCameraTrack->velocity.z;
                g_pWhirlsandCameraTrack->distance += g_pWhirlsandCameraTrack->delta;
                ((BattleFxCameraVec *)buf)->x = g_pWhirlsandCameraOther->position.x - g_pWhirlsandCameraTrack->base.x;
                ((BattleFxCameraVec *)buf)->y = g_pWhirlsandCameraOther->position.y - g_pWhirlsandCameraTrack->base.y;
                ((BattleFxCameraVec *)buf)->z = g_pWhirlsandCameraOther->position.z - g_pWhirlsandCameraTrack->base.z;
                VectorNormalToSVector((int *)buf, (short *)offset);
                g_pWhirlsandCameraTrack->position.x = g_pWhirlsandCameraOther->position.x - (offset->vx * g_pWhirlsandCameraTrack->distance >> 12);
                g_pWhirlsandCameraTrack->position.y = g_pWhirlsandCameraOther->position.y - (offset->vy * g_pWhirlsandCameraTrack->distance >> 12);
                g_pWhirlsandCameraTrack->position.z = g_pWhirlsandCameraOther->position.z - (offset->vz * g_pWhirlsandCameraTrack->distance >> 12);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_pWhirlsandCameraTrack->distance += g_pWhirlsandCameraTrack->delta;
                eased = g_pWhirlsandCameraTrack->distance;
                eased = BattleFx_WhirlsandApplyCameraEasing(eased, g_pWhirlsandCameraTrack->easing);
                g_pWhirlsandCameraTrack->position.x = g_pWhirlsandCameraTrack->base.x + (g_pWhirlsandCameraTrack->velocity.x * eased >> 12);
                g_pWhirlsandCameraTrack->position.y = g_pWhirlsandCameraTrack->base.y + (g_pWhirlsandCameraTrack->velocity.y * eased >> 12);
                g_pWhirlsandCameraTrack->position.z = g_pWhirlsandCameraTrack->base.z + (g_pWhirlsandCameraTrack->velocity.z * eased >> 12);
            }
            (g_pWhirlsandCameraTrack->frames)--;
            g_pWhirlsandCameraTrack = &g_WhirlsandCameraFocus;
            g_pWhirlsandCameraOther = &g_WhirlsandCameraEye;
        }
    }
    if (cameraScriptPtr->rotMatrix != 0) {
        if (g_WhirlsandCameraEye.mode != 9) {
            SetRotMatrix(cameraScriptPtr->rotMatrix);
            SetTransMatrix(cameraScriptPtr->rotMatrix);
            if (g_WhirlsandCameraEye.actor == -1) {
                RotTransPoint(&g_WhirlsandCameraEye.position, (int *)buf, &bits);
            } else {
                BattleFx_WhirlsandGetActorLocalPosition(g_WhirlsandCameraEye.actor, g_WhirlsandCameraEye.bone, cameraScriptPtr->rotMatrix, offset);
                offset->vx += g_WhirlsandCameraEye.position.x;
                offset->vy += g_WhirlsandCameraEye.position.y;
                offset->vz += g_WhirlsandCameraEye.position.z;
                SetRotMatrix(cameraScriptPtr->rotMatrix);
                SetTransMatrix(cameraScriptPtr->rotMatrix);
                RotTransPoint((void *)offset, (int *)buf, &bits);
            }
            g_BattleCameraPosition.cameraPosX = buf->x;
            g_BattleCameraPosition.cameraPosY = buf->y;
            g_BattleCameraPosZ = buf->z;
        }
        if (g_WhirlsandCameraFocus.mode != 9) {
            if (g_WhirlsandCameraFocus.actor == -1) {
                RotTransPoint(&g_WhirlsandCameraFocus.position, (int *)buf, &bits);
            } else {
                BattleFx_WhirlsandGetActorLocalPosition(g_WhirlsandCameraFocus.actor, g_WhirlsandCameraFocus.bone, cameraScriptPtr->rotMatrix, offset);
                offset->vx += g_WhirlsandCameraFocus.position.x;
                offset->vy += g_WhirlsandCameraFocus.position.y;
                offset->vz += g_WhirlsandCameraFocus.position.z;
                SetRotMatrix(cameraScriptPtr->rotMatrix);
                SetTransMatrix(cameraScriptPtr->rotMatrix);
                RotTransPoint((void *)offset, (int *)buf, &bits);
            }
            g_BattleCameraFocalPoint.focalPointX = buf->x;
            g_BattleCameraFocalPoint.focalPointY = buf->y;
            g_BattleCameraFocusZ = buf->z;
        }
    } else {
        if (g_WhirlsandCameraEye.mode != 9) {
            *(int *)&g_BattleCameraPosition = *(int *)&g_WhirlsandCameraEye.position;
            *(int *)&g_BattleCameraPosZ = *(int *)&g_WhirlsandCameraEye.position.z;
        }
        if (g_WhirlsandCameraFocus.mode != 9) {
            *(int *)&g_BattleCameraFocalPoint = *(int *)&g_WhirlsandCameraFocus.position;
            *(int *)&g_BattleCameraFocusZ = *(int *)&g_WhirlsandCameraFocus.position.z;
        }
    }
}

// Script operand fetch: reads the signed 16-bit word at the cursor held in *cursor, advances that
// cursor by two bytes, and if the word is negative uses its negation as a short index into the
// g_WhirlsandDataBlock1 operand table, returning that table entry instead. Returns the resulting
// short, so a non-negative word is an immediate and a negative one is an indirect variable read.
// BattleFx_SaferSephirothDeathReadCameraScriptOperand at 0x0045325f is the same routine against a different
// table (g_SaferSephirothDeath_CameraScriptOperandPool[6]).
// FUNCTION: FF7 0x0044fd72
short __cdecl BattleFx_WhirlsandReadCameraScriptOperand(short **cursor)
{
    int value;

    value = **cursor;
    *cursor += 1;
    if (value < 0) {
        value = ((short *)((char *)&g_WhirlsandDataBlock1 + 2))[-value];
    }
    return (short)value;
}

// Returns value directly when easing is zero; otherwise repeatedly applies fixed sine to value divided
// by four using the magnitude high byte, complements the input and output for negative easing, and
// blends the result with the original input using the magnitude low byte plus one.
// FUNCTION: FF7 0x0044fdac
int __cdecl BattleFx_WhirlsandApplyCameraEasing(int value, int easing)
{
    int weight;
    int iter;
    int orig;

    orig = value;
    if (easing > 0) {
        weight = easing >> 8;
        for (iter = 0; iter < weight; iter++) {
            value = SinFixed(((value) / 4));
        }
    } else if (easing < 0) {
        easing = -easing;
        weight = easing >> 8;
        value = 0x1000 - value;
        for (iter = 0; iter < weight; iter++) {
            value = SinFixed(((value) / 4));
        }
        value = 0x1000 - value;
    } else {
        return value;
    }
    weight = (easing & 0xFF) + 1;
    value = value * weight + orig * (0x100 - weight) >> 8;
    return value;
}

// Expresses a battle actor point in the local space of a matrix. When bone is not -1 it takes that
// bone's world position from Battle_GetBoneWorldPos(actor, bone, offset). Otherwise it copies the
// actor's own posX, posY and posZ from its record, at a record stride of 0x1AEC. It then subtracts
// the matrix translateX, translateY and translateZ from the point, transposes the matrix, and
// rotates the delta by it with TransformVectorToInt16, writing the result back over offset. The
// unreferenced seven-int padding array preserves the retail VC5 /Od stack frame size.
// BattleFx_SaferSephirothDeathGetActorLocalPosition at 0x00453385 is a byte-identical duplicate.
// FUNCTION: FF7 0x0044fe98
void __cdecl BattleFx_WhirlsandGetActorLocalPosition(int actor, int bone, PsxMatrix *matrix, Vec3sPacked *offset)
{
    int transposed[8];

    if (bone != -1) {
        Battle_GetBoneWorldPos(actor, bone, (short *)offset);
    } else {
        offset->vx = g_BattleActorRecords[actor].posX;
        offset->vy = g_BattleActorRecords[actor].posY;
        offset->vz = g_BattleActorRecords[actor].posZ;
    }
    offset->vx -= (short)matrix->t[0];
    offset->vy -= (short)matrix->t[1];
    offset->vz -= (short)matrix->t[2];
    TransposeMatrix(matrix, transposed);
    TransformVectorToInt16(transposed, (void *)offset, (void *)offset);
}

// Composes g_WhirlsandBaseMatrix with g_BattleCameraTransform into g_WhirlsandCompositeMatrix before pause. While unpaused at
// counter five schedules BattleFx_WhirlsandRingGridHeightEnvelopeTask, BattleFx_WhirlsandShadedRingGridPairTask, BattleFx_WhirlsandTexturedRingGridDrawTask, a bit-pattern task with
// arguments (7,0,0,BattleFx_WeaponSpawnWhirlsandPerTarget), and BattleFx_WhirlsandSurfaceParticleEmitterTask, then dispatches sound command (0x22,0x40,0).
// Retains empty timing branches for the intervening ranges. At prior counts at least 125 clears
// g_BattleScriptEffectFlag, retires, releases a nonnegative retained offscreen slot and resets it
// to minus one, then releases cached sounds 0x105, 0x17F and 0x27E with argument fifteen.
// Increments count regardless.
// FUNCTION: FF7 0x0044ff63
void __cdecl BattleFx_WhirlsandTimelineTask(void)
{
    int localFrame;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    MultiplyMatrix(&g_BattleCameraTransform, &g_WhirlsandBaseMatrix, &g_WhirlsandCompositeMatrix);
    if (g_BattleIsPaused != 0) {
        return;
    }
    localFrame = task->frameCounter;
    if (localFrame < 5) { /* empty first arm: lib/codegen.h */
    } else {
        localFrame -= 5;
        if (localFrame < 30) {
            if (localFrame == 0) {
                Battle_AllocateResourceSlot(BattleFx_WhirlsandRingGridHeightEnvelopeTask);
                Battle_AllocateResourceSlot(BattleFx_WhirlsandShadedRingGridPairTask);
                Battle_AllocateResourceSlot(BattleFx_WhirlsandTexturedRingGridDrawTask);
                Battle_CreateBitPatternCallbackTask(BATTLE_ACTOR_MASK_PARTY, 0, 0, BattleFx_WeaponSpawnWhirlsandPerTarget);
                Battle_AllocateResourceSlot(BattleFx_WhirlsandSurfaceParticleEmitterTask);
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 0);
            }
        } else {
            localFrame -= 30;
            if (localFrame < 60) { /* empty first arm: lib/codegen.h */
            } else {
                localFrame -= 60;
                if (localFrame < 30) { /* empty first arm: lib/codegen.h */
                } else {
                    g_BattleScriptEffectFlag = 0;
                    task->ownerSlot = BATTLE_TASK_FREE;
                    if (g_WhirlsandStageSwirlOffscreenSlotIndex >= 0) {
                        Battle3d_ReleaseOffscreenSlot(g_WhirlsandStageSwirlOffscreenSlotIndex);
                        g_WhirlsandStageSwirlOffscreenSlotIndex = -1;
                    }
                    Sfx_ReleaseCachedBuffer(0x105, 0xF);
                    Sfx_ReleaseCachedBuffer(0x17F, 0xF);
                    Sfx_ReleaseCachedBuffer(0x27E, 0xF);
                }
            }
        }
    }
    (task->frameCounter)++;
}

// On first shared use sets mask one in g_WhirlsandDataBlock4InitFlag and saves Gfx_GetPsxScratchpadPtr(0). At
// task count zero calls BattleFx_WhirlsandBuildAndQueueStageOffscreenRequest and
// BattleFx_WhirlsandBuildRingGridMesh and increments count even while paused. Later below thirty
// publishes count times 4096 divided by thirty, shapes it through BattleFx_WhirlsandApplyCameraEasing
// with 0x3FF and calls BattleFx_WhirlsandSetRingGridRowHeights with shaped value times 1000, 128 plus
// the shaped value times minus eighty shifted by twelve and retained constant 128. Counts thirty
// through 89 publish 4096 without that call. Counts ninety through 119 publish the descending
// thirty-count fraction, shape it with argument zero and call the same helper; at counts at least
// 120 retires and returns before count advancement. Later active count increments occur only while
// unpaused, but fraction/helper updates and retirement occur while paused; retained zero
// multiplications and signed shifts remain.
// FUNCTION: FF7 0x004500bd
void __cdecl BattleFx_WhirlsandRingGridHeightEnvelopeTask(void)
{
    int age;
    BattleTaskSlot *task;
    int hBase;
    int unusedEntry[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int ratio;
    int levelB;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_WhirlsandDataBlock4InitFlag & 1) == 0) {
        g_WhirlsandDataBlock4InitFlag |= 1;
        g_WhirlsandDataBlock4 = Gfx_GetPsxScratchpadPtr(0);
    }
    if (task->frameCounter == 0) {
        BattleFx_WhirlsandBuildAndQueueStageOffscreenRequest();
        BattleFx_WhirlsandBuildRingGridMesh();
        (task->frameCounter)++;
    } else {
        age = task->frameCounter;
        if (age < 30) {
            ratio = (age << 12) / 30;
            g_WhirlsandScale = ratio;
            ratio = BattleFx_WhirlsandApplyCameraEasing(ratio, 0x3FF);
            hBase = (ratio * 0 >> 12) + 0x80;
            levelB = (ratio * -0x50 >> 12) + 0x80;
            BattleFx_WhirlsandSetRingGridRowHeights(ratio * 0x3E8 >> 12, levelB, hBase);
        } else {
            age -= 30;
            if (age < 60) {
                g_WhirlsandScale = 0x1000 /* 1.0 */;
            } else {
                age -= 60;
                if (age < 30) {
                    ratio = (0x1E - age << 12) / 30;
                    g_WhirlsandScale = ratio;
                    ratio = BattleFx_WhirlsandApplyCameraEasing(ratio, 0);
                    hBase = (ratio * 0 >> 12) + 0x80;
                    levelB = (ratio * -0x50 >> 12) + 0x80;
                    BattleFx_WhirlsandSetRingGridRowHeights(ratio * 0x3E8 >> 12, levelB, hBase);
                } else {
                    task->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
        if (g_BattleIsPaused == 0) {
            (task->frameCounter)++;
        }
    }
}

// Initializes g_WhirlsandSceneMeshDrawRectangle from the global coordinates with mode-dependent extents, copies the
// source matrix first short into the local scale field and first short, selects the sign of its
// translation z from the global flag, multiplies the two matrices, and passes the product
// and rectangle to BattleFx_WhirlsandQueueStageOffscreenRequest.
// FUNCTION: FF7 0x00450276
void __cdecl BattleFx_WhirlsandBuildAndQueueStageOffscreenRequest(void)
{
    int w[8];

    if (g_BattleUiScalingMode == 2) {
        (g_WhirlsandSceneMeshDrawRectangle.argA) = (short)g_BattleViewportOriginX;
        (g_WhirlsandSceneMeshDrawRectangle.argB) = (short)g_BattleViewportOriginY;
        (g_WhirlsandSceneMeshDrawRectangle.argC) = 0x80;
        (g_WhirlsandSceneMeshDrawRectangle.argD) = 0xFF;
    } else {
        (g_WhirlsandSceneMeshDrawRectangle.argA) = (short)g_BattleViewportOriginX;
        (g_WhirlsandSceneMeshDrawRectangle.argB) = (short)g_BattleViewportOriginY;
        (g_WhirlsandSceneMeshDrawRectangle.argC) = 0x40;
        (g_WhirlsandSceneMeshDrawRectangle.argD) = 0x80;
    }
    g_WhirlsandSceneMeshTransform.m[2][2] = g_WhirlsandBaseMatrix.m[0][0];
    g_WhirlsandSceneMeshTransform.m[0][0] = g_WhirlsandSceneMeshTransform.m[2][2];
    if (g_WhirlsandRotationVector[1] != 0) {
        g_WhirlsandSceneMeshTransform.t[2] = g_WhirlsandBaseMatrix.t[2];
    } else {
        g_WhirlsandSceneMeshTransform.t[2] = -g_WhirlsandBaseMatrix.t[2];
    }
    MultiplyMatrix(&g_WhirlsandStageSwirlBaseMatrix, &g_WhirlsandSceneMeshTransform, w);
    BattleFx_WhirlsandQueueStageOffscreenRequest((PsxMatrix *)w, &g_WhirlsandSceneMeshDrawRectangle);
}

// Installs the supplied rotation and translation matrices; when the stage context and its mesh
// pointer exist, acquires an offscreen request, fills its part-one mesh, converted matrix, draw
// fields and four signed rectangle coordinates, and saves the request's slotIndex in g_WhirlsandStageSwirlOffscreenSlotIndex.
// FUNCTION: FF7 0x0045034d
void __cdecl BattleFx_WhirlsandQueueStageOffscreenRequest(PsxMatrix *matrix, BattleFxWhirlsandStageRequestArgs *args)
{
    GfxOffscreenRequest *req;
    int gfxData;

    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    if (g_pBattle3dScene != 0 && g_pBattle3dScene->stageId != 0) {
        gfxData = g_pBattle3dScene->stageId;
        req = (GfxOffscreenRequest *)Battle3d_AcquireOffscreenRequest();
        if (req != 0) {
            req->kind = 1;
            req->mesh = (int)BattleStage_GetPartMesh(1, (Battle3dStageRecord *)gfxData);
            GtePsxMatrixToFloat4x4Transposed(matrix, req->modelMatrix);
            req->pageFillEnabled = 0;
            req->renderKind = 0xC;
            req->blendMode = BLEND_ADDITIVE;
            req->blendMode = BLEND_NONE;
            req->viewportX = args->argA;
            req->viewportY = args->argB;
            req->viewportW = args->argC;
            req->viewportH = args->argD;
            req->pageFillStepX = 1;
            req->pageFillStepY = 1;
            g_WhirlsandStageSwirlOffscreenSlotIndex = req->slotIndex;
        }
    }
}

// Initializes a sixteen-by-sixteen grid view over g_WhirlsandStageSwirlGridBasePointer. Each cell stores four pointers to
// adjacent eight-byte vertices with angular wrapping, initializes two primitive code words,
// texture shorts and circular UV coordinates from inner radius row times four and outer radius
// capped at 63. Then writes seventeen rings of sixteen signed X/Z vertices, Y zero, with radii row
// times 256 below row sixteen or 0xFC0 for the final row, using sine and negated cosine of equally
// spaced twelve-bit angles. Writes seventeen 0x3C808080 colour words and clears g_WhirlsandScale.
// Retains one-iteration primitive loops; fourth vertex shorts are not written here.
// FUNCTION: FF7 0x00450476
void __cdecl BattleFx_WhirlsandBuildRingGridMesh(void)
{
    int innerRadius;
    int outerRadius;
    BattleFxSwirlCell *entry;
    int i;
    int a0;
    int seg;
    int cap;
    int a1;
    int k;
    int ringLen;

    for (i = 0; i < 16; i++) {
        innerRadius = i << 2;
        if (innerRadius + 4 < 64) {
            cap = innerRadius + 4;
        } else {
            cap = 0x3F;
        }
        outerRadius = cap;
        for (seg = 0; seg < 16; seg++) {
            entry = &g_WhirlsandStageSwirlGridBasePointer->cells[i][seg];
            entry->corners.cornerRowCol = (int)&g_WhirlsandStageSwirlGridBasePointer->ring[i][seg];
            entry->corners.cornerNextCol = (int)&g_WhirlsandStageSwirlGridBasePointer->ring[i][(seg + 1) % 16];
            entry->corners.cornerNextRow = (int)&g_WhirlsandStageSwirlGridBasePointer->ring[i + 1][seg];
            entry->corners.cornerNextRowCol = (int)&g_WhirlsandStageSwirlGridBasePointer->ring[i + 1][(seg + 1) % 16];
            a0 = (seg << 12) / 16;
            a1 = ((seg + 1) << 12) / 16;
            for (k = 0; k < 1; k++) {
                entry->packets[k].code = 0xC000000;
                entry->packets[k].tpage = 0x12E;
            }
            for (k = 0; k < 1; k++) {
                (&entry->packets[2])[k].code = 0xC000000;
                (&entry->packets[2])[k].u0 = (innerRadius * SinFixed(a0) >> 11) + 0x80;
                (&entry->packets[2])[k].v0 = (innerRadius * CosFixed(a0) >> 11) + 0x80;
                (&entry->packets[2])[k].u1 = (innerRadius * SinFixed(a1) >> 11) + 0x80;
                (&entry->packets[2])[k].v1 = (innerRadius * CosFixed(a1) >> 11) + 0x80;
                (&entry->packets[2])[k].u2 = (outerRadius * SinFixed(a0) >> 11) + 0x80;
                (&entry->packets[2])[k].v2 = (outerRadius * CosFixed(a0) >> 11) + 0x80;
                (&entry->packets[2])[k].u3 = (outerRadius * SinFixed(a1) >> 11) + 0x80;
                (&entry->packets[2])[k].v3 = (outerRadius * CosFixed(a1) >> 11) + 0x80;
                (&entry->packets[2])[k].tpage = 0xAC;
                (&entry->packets[2])[k].clut = 0x3FF0;
            }
        }
    }
    for (i = 0; i < 17; i++) {
        if (i < 16) {
            ringLen = i << 2 << 6;
        } else {
            ringLen = 4032;
        }
        innerRadius = ringLen;
        for (seg = 0; seg < 16; seg++) {
            a0 = (seg << 12) / 16;
            g_WhirlsandStageSwirlGridBasePointer->ring[i][seg].x = innerRadius * SinFixed(a0) >> 12;
            g_WhirlsandStageSwirlGridBasePointer->ring[i][seg].y = 0;
            g_WhirlsandStageSwirlGridBasePointer->ring[i][seg].z = -innerRadius * CosFixed(a0) >> 12;
        }
        g_WhirlsandStageSwirlGridBasePointer->rowColor[i] = 0x3C808080;
    }
    g_WhirlsandScale = 0;
}

// For sixteen rows applies BattleFx_WhirlsandApplyCameraEasing to the evenly spaced fixed-point row parameter, fills
// sixteen short heights from height, and packs a color interpolated between colorFrom and colorTo into the
// row draw word with opcode 0x3C.
// FUNCTION: FF7 0x004508b1
void __cdecl BattleFx_WhirlsandSetRingGridRowHeights(int height, int colorFrom, int colorTo)
{
    int value;
    int rowScale;
    int column;
    int rowIdx;
    int rowParam;

    rowScale = (height << 12) / 1000;
    for (rowIdx = 0; rowIdx < 16; rowIdx++) {
        rowParam = (rowIdx << 12) / 15;
        rowParam = BattleFx_WhirlsandApplyCameraEasing(rowParam, 0xFFFFFC01);
        for (column = 0; column < 16; column++) {
            g_WhirlsandStageSwirlGridBasePointer->ring[rowIdx][column].y = height * (4096 - rowParam) >> 12;
        }
        value = colorFrom + ((colorTo - colorFrom) * (rowScale * rowParam >> 12) >> 12);
        g_WhirlsandStageSwirlGridBasePointer->rowColor[rowIdx] = value | 0x3C000000 | value << 8 | value << 16;
    }
}

// While the current slot's frameCounter is below 0x78, calls BattleFx_WhirlsandDrawTexturedRingGrid with g_WhirlsandCompositeMatrix and increments
// the counter only when unpaused; otherwise retires the slot and returns.
// FUNCTION: FF7 0x00450999
void __cdecl BattleFx_WhirlsandTexturedRingGridDrawTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (task->frameCounter < 120) {
        BattleFx_WhirlsandDrawTexturedRingGrid(&g_WhirlsandCompositeMatrix);
    } else {
        task->ownerSlot = BATTLE_TASK_FREE;
        return;
    }
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

// On first shared use sets mask one in g_WhirlsandDataBlock3InitFlag and saves a data-block pointer. Copies the
// supplied packed thirty-byte matrix, scales it uniformly by g_WhirlsandScale and installs it.
// Projects all sixteen-by-sixteen grid cells; when flags lack 0x60000 updates the selected
// primitive buffer UVs from circular radii and the retained fraction, halves all eight UV bytes
// when g_BattleUiScalingMode is not two, copies adjacent ring colours into the four corners, sets
// offscreen render-object opcode 0xF first to four then one and draws. Does not require a positive
// projection result and has no pause branch.
// FUNCTION: FF7 0x004509f6
void __cdecl BattleFx_WhirlsandDrawTexturedRingGrid(PsxMatrix *matrix)
{
    int radiusB;
    int level;
    int row;
    BattleFxSwirlCell *cellPtr;
    int angle;
    int column;
    int angleEnd;
    int clampedRadius;

    if ((g_WhirlsandDataBlock3InitFlag & 1) == 0) {
        g_WhirlsandDataBlock3InitFlag |= 1;
        g_WhirlsandDataBlock3 = (BattleFxWhirlsandGridScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_WhirlsandDataBlock3->scale[2] = g_WhirlsandScale;
    g_WhirlsandDataBlock3->scale[1] = g_WhirlsandScale;
    g_WhirlsandDataBlock3->scale[0] = g_WhirlsandScale;
    g_WhirlsandDataBlock3->matrix = *matrix;
    ScaleMatrix(&g_WhirlsandDataBlock3->matrix, g_WhirlsandDataBlock3->scale);
    SetRotMatrix(&g_WhirlsandDataBlock3->matrix);
    SetTransMatrix(&g_WhirlsandDataBlock3->matrix);
    RefreshGlobalTransformState();
    for (row = 0; row < 16; row++) {
        level = row << 2;
        if (level + 4 < 64) {
            clampedRadius = level + 4;
        } else {
            clampedRadius = 63;
        }
        radiusB = clampedRadius;
        for (column = 0; column < 16; column++) {
            cellPtr = &g_WhirlsandStageSwirlGridBasePointer->cells[row][column];
            g_WhirlsandDataBlock3->depth = Gfx_TransformQuadReturnDepth((void *)cellPtr->corners.cornerRowCol, (SVECTOR *)cellPtr->corners.cornerNextCol, (SVECTOR *)cellPtr->corners.cornerNextRow, (SVECTOR *)cellPtr->corners.cornerNextRowCol, &g_WhirlsandDataBlock3->projectDepth, &g_WhirlsandDataBlock3->projectFlags);
            if ((g_WhirlsandDataBlock3->projectFlags & 0x60000) == 0) {
                angle = (column << 12) / 16;
                angleEnd = ((column + 1) << 12) / 16;
                cellPtr->packets[g_WhirlsandParticleRowIndex].u0 = ((level * SinFixed(angle) >> 12) * g_WhirlsandScale >> 12) + 0x40;
                cellPtr->packets[g_WhirlsandParticleRowIndex].v0 = ((level * CosFixed(angle) >> 11) * g_WhirlsandScale >> 12) + 0x80;
                cellPtr->packets[g_WhirlsandParticleRowIndex].u1 = ((level * SinFixed(angleEnd) >> 12) * g_WhirlsandScale >> 12) + 0x40;
                cellPtr->packets[g_WhirlsandParticleRowIndex].v1 = ((level * CosFixed(angleEnd) >> 11) * g_WhirlsandScale >> 12) + 0x80;
                cellPtr->packets[g_WhirlsandParticleRowIndex].u2 = ((radiusB * SinFixed(angle) >> 12) * g_WhirlsandScale >> 12) + 0x40;
                cellPtr->packets[g_WhirlsandParticleRowIndex].v2 = ((radiusB * CosFixed(angle) >> 11) * g_WhirlsandScale >> 12) + 0x80;
                cellPtr->packets[g_WhirlsandParticleRowIndex].u3 = ((radiusB * SinFixed(angleEnd) >> 12) * g_WhirlsandScale >> 12) + 0x40;
                cellPtr->packets[g_WhirlsandParticleRowIndex].v3 = ((radiusB * CosFixed(angleEnd) >> 11) * g_WhirlsandScale >> 12) + 0x80;
                if (g_BattleUiScalingMode != 2) {
                    *(unsigned char *)&cellPtr->packets[g_WhirlsandParticleRowIndex].u0 >>= 1;
                    *(unsigned char *)&cellPtr->packets[g_WhirlsandParticleRowIndex].v0 >>= 1;
                    *(unsigned char *)&cellPtr->packets[g_WhirlsandParticleRowIndex].u1 >>= 1;
                    *(unsigned char *)&cellPtr->packets[g_WhirlsandParticleRowIndex].v1 >>= 1;
                    *(unsigned char *)&cellPtr->packets[g_WhirlsandParticleRowIndex].u2 >>= 1;
                    *(unsigned char *)&cellPtr->packets[g_WhirlsandParticleRowIndex].v2 >>= 1;
                    *(unsigned char *)&cellPtr->packets[g_WhirlsandParticleRowIndex].u3 >>= 1;
                    *(unsigned char *)&cellPtr->packets[g_WhirlsandParticleRowIndex].v3 >>= 1;
                }
                cellPtr->packets[g_WhirlsandParticleRowIndex].color1 = g_WhirlsandStageSwirlGridBasePointer->rowColor[row];
                cellPtr->packets[g_WhirlsandParticleRowIndex].color0 = cellPtr->packets[g_WhirlsandParticleRowIndex].color1;
                cellPtr->packets[g_WhirlsandParticleRowIndex].color3 = g_WhirlsandStageSwirlGridBasePointer->rowColor[row + 1];
                cellPtr->packets[g_WhirlsandParticleRowIndex].color2 = cellPtr->packets[g_WhirlsandParticleRowIndex].color3;
                Gfx_SetPsxPrimQuadState(0xF, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_WhirlsandStageSwirlOffscreenSlotIndex));
                Gfx_SetPsxPrimQuadState(0xF, 1, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_WhirlsandStageSwirlOffscreenSlotIndex));
                Gfx_DrawPsxPrimitive(&cellPtr->packets[g_WhirlsandParticleRowIndex]);
            }
        }
    }
}

// Derives a fraction descending from 4096 over counts below thirty, zero during thirty through 89
// and ascending over ninety through 119; retires and returns at counts at least 120 even while
// paused. Builds two zero-translation XYZ matrices from separate retained rotation triples,
// composes each with g_WhirlsandCompositeMatrix and calls BattleFx_WhirlsandDrawShadedRingGrid with 4096 plus the fraction-minus-4096
// term shifted left eleven/right twelve. Advances the first rotation Y by 256 and the second by
// 128 on every active call including paused calls. Advances task count only while unpaused.
// FUNCTION: FF7 0x00450f42
void __cdecl BattleFx_WhirlsandShadedRingGridPairTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_28[0x28]; int rampFrac; };
        struct { unsigned char pad_to_34[0x34]; int fraction; };
        struct { unsigned char pad_to_38[0x38]; BattleTaskSlot *ringPair; };
        struct { unsigned char pad_to_3C[0x3C]; int count; };
    } frame;
#pragma pack(pop)

    frame.ringPair = BATTLE_CURRENT_TASK_SLOT;
    frame.count = frame.ringPair->frameCounter;
    if (frame.count < 30) {
        frame.rampFrac = (frame.count << 12) / 30;
        frame.fraction = 0x1000 - frame.rampFrac;
    } else {
        frame.count -= 30;
        if (frame.count < 60) {
            frame.fraction = 0;
        } else {
            frame.count -= 60;
            if (frame.count < 30) {
                frame.rampFrac = (frame.count << 12) / 30;
                frame.fraction = frame.rampFrac;
            } else {
                frame.ringPair->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    RotMatrixXYZ(g_WhirlsandModelRotationAngles1, &frame.rotMatrix);
    frame.rotMatrix.t[2] = 0;
    frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
    frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
    MultiplyMatrix(&g_WhirlsandCompositeMatrix, &frame.rotMatrix, &frame.rotMatrix);
    BattleFx_WhirlsandDrawShadedRingGrid(&frame.rotMatrix, (frame.fraction - 0x1000 << 11 >> 12) + 0x1000);
    g_WhirlsandModelRotationAngles1[1] += 0x100 /* 22.5 deg */;
    RotMatrixXYZ(g_WhirlsandModelRotationAngles2, &frame.rotMatrix);
    frame.rotMatrix.t[2] = 0;
    frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
    frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
    MultiplyMatrix(&g_WhirlsandCompositeMatrix, &frame.rotMatrix, &frame.rotMatrix);
    BattleFx_WhirlsandDrawShadedRingGrid(&frame.rotMatrix, (frame.fraction - 0x1000 << 11 >> 12) + 0x1000);
    g_WhirlsandModelRotationAngles2[1] += 0x80;
    if (g_BattleIsPaused == 0) {
        (frame.ringPair->frameCounter)++;
    }
}

// On first shared use sets mask one in g_WhirlsandDataBlock2InitFlag and saves a data-block pointer. Copies the
// supplied packed thirty-byte matrix, scales it uniformly by g_WhirlsandScale and installs it.
// Projects all sixteen-by-sixteen grid cells; when flags lack 0x60000 sets black draw colour,
// shades adjacent ring colours by shade and copies them into paired corners of the selected
// primitive buffer, setting command byte 0x3E after the first shading call. Sets opcode fifteen to
// four on resource-zero render object, then to one on resource-one render object, and draws the
// selected primitive. Does not require a positive projection result and has no pause branch.
// FUNCTION: FF7 0x004510d1
void __cdecl BattleFx_WhirlsandDrawShadedRingGrid(PsxMatrix *matrix, int shade)
{
    int row;
    BattleFxSwirlCell *cell;
    int colIndex;
    if ((g_WhirlsandDataBlock2InitFlag & 1) == 0) {
        g_WhirlsandDataBlock2InitFlag |= 1;
        g_WhirlsandDataBlock2 = (BattleFxWhirlsandGridScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_WhirlsandDataBlock2->scale[2] = g_WhirlsandScale;
    g_WhirlsandDataBlock2->scale[1] = g_WhirlsandScale;
    g_WhirlsandDataBlock2->scale[0] = g_WhirlsandScale;
    g_WhirlsandDataBlock2->matrix = *matrix;
    ScaleMatrix(&g_WhirlsandDataBlock2->matrix, g_WhirlsandDataBlock2->scale);
    SetRotMatrix(&g_WhirlsandDataBlock2->matrix);
    SetTransMatrix(&g_WhirlsandDataBlock2->matrix);
    RefreshGlobalTransformState();
    for (row = 0; row < 16; row++) {
        for (colIndex = 0; colIndex < 16; colIndex++) {
            cell = &g_WhirlsandStageSwirlGridBasePointer->cells[row][colIndex];
            g_WhirlsandDataBlock2->depth = Gfx_TransformQuadReturnDepth((void *)cell->corners.cornerRowCol, (SVECTOR *)cell->corners.cornerNextCol, (SVECTOR *)cell->corners.cornerNextRow, (SVECTOR *)cell->corners.cornerNextRowCol, &g_WhirlsandDataBlock2->projectDepth, &g_WhirlsandDataBlock2->projectFlags);
            if ((g_WhirlsandDataBlock2->projectFlags & 0x60000) == 0) {
                SetDrawColorRGB(0, 0, 0);
                Gfx_ShadePsxVertexColor((CVECTOR *)&g_WhirlsandStageSwirlGridBasePointer->rowColor[row], shade, &g_WhirlsandDataBlock2->color);
                g_WhirlsandDataBlock2->color.cd = 0x3e;
                (&cell->packets[2])[g_WhirlsandParticleSlotIndex].color1 = *(int *)&g_WhirlsandDataBlock2->color;
                (&cell->packets[2])[g_WhirlsandParticleSlotIndex].color0 = (&cell->packets[2])[g_WhirlsandParticleSlotIndex].color1;
                Gfx_ShadePsxVertexColor((CVECTOR *)&g_WhirlsandStageSwirlGridBasePointer->rowColor[row + 1], shade, &g_WhirlsandDataBlock2->color);
                (&cell->packets[2])[g_WhirlsandParticleSlotIndex].color3 = *(int *)&g_WhirlsandDataBlock2->color;
                (&cell->packets[2])[g_WhirlsandParticleSlotIndex].color2 = (&cell->packets[2])[g_WhirlsandParticleSlotIndex].color3;
                Gfx_SetPsxPrimQuadState(15, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0, g_WhirlsandResourceHandle)));
                Gfx_SetPsxPrimQuadState(15, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(1, g_WhirlsandResourceHandle)));
                Gfx_DrawPsxPrimitive(&(&cell->packets[2])[g_WhirlsandParticleSlotIndex]);
            }
        }
    }
}

// Returns while paused. Computes emission amount rising toward 0x300 over counts below thirty,
// fixed 0x300 during thirty through 89 and descending during ninety through 119; retires and
// returns at counts at least 120. Adds this amount to g_WhirlsandEmissionAccumulator but emits from
// the local amount while it is at least 256, subtracting 256 each allocation without using the
// global accumulator remainder. Each BattleFx_WhirlsandSurfaceParticleTask receives random circular
// X/Z based on g_WhirlsandScale and a random radius fraction, Y from
// BattleFx_WhirlsandGetRingGridHeightAtPoint and negative vertical velocity from rand modulo 100 plus
// 200. Retains the constant-false comma-expression branch for velocity 300. Advances task count
// after emission.
// FUNCTION: FF7 0x00451381
void __cdecl BattleFx_WhirlsandSurfaceParticleEmitterTask(void)
{
    int angle;
    int age;
    BattleTaskSlot *task;
    int extent;
    int emitAmount;
    int ratio;
    int fallRoll;
    BattleFxWhirlsandSurfaceParticleTaskSlot *spawned;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    age = task->frameCounter;
    if (age < 30) {
        ratio = (age << 12) / 30;
        emitAmount = ratio * 0x300 /* 0.1875 */ >> 12;
    } else {
        age -= 30;
        if (age < 60) {
            emitAmount = 768;
        } else {
            age -= 60;
            if (age < 30) {
                ratio = (age << 12) / 30;
                emitAmount = (4096 - ratio) * 0x300 /* 0.1875 */ >> 12;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_WhirlsandEmissionAccumulator += emitAmount;
    while (emitAmount >= 256) {
        spawned = (BattleFxWhirlsandSurfaceParticleTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_WhirlsandSurfaceParticleTask);
        angle = rand() & 0xFFF;
        extent = (g_WhirlsandScale << 12 >> 12) * (rand() & 0xFFF) >> 12;
        spawned->posX = SinFixed(angle) * extent >> 12;
        spawned->posZ = CosFixed(angle) * extent >> 12;
        spawned->posY = BattleFx_WhirlsandGetRingGridHeightAtPoint((BattleFxRingGridPoint *)&spawned->posX);
        if (0 != 0) {
            fallRoll = 0x12C;
        } else {
            fallRoll = rand() % 0x64 + 0xC8;
        }
        spawned->velocityY = -fallRoll;
        emitAmount -= 256;
    }
    (task->frameCounter)++;
}

// Returns zero when g_WhirlsandScale is zero; otherwise derives a normalized radial index from the
// input X and Z relative to the source translation, linearly interpolates adjacent g_WhirlsandStageSwirlGridBasePointer
// row-height shorts when the index quotient is below sixteen, and returns zero outside that range.
// FUNCTION: FF7 0x0045153f
int __cdecl BattleFx_WhirlsandGetRingGridHeightAtPoint(BattleFxRingGridPoint *point)
{
    int idx;
    int fraction;
    int deltaZ;
    int lowHeight;
    int highHeight;

    if (g_WhirlsandScale == 0) {
        return 0;
    } else {
        deltaZ = point->posZ - g_WhirlsandBaseMatrix.t[2];
        idx = ((SqrtToInt(point->posX * point->posX + deltaZ * deltaZ)) / 64);
        idx = (idx << 12) / g_WhirlsandScale;
        if (idx / 4 < 16) {
            lowHeight = g_WhirlsandStageSwirlGridBasePointer->ring[idx / 4][0].y;
            highHeight = g_WhirlsandStageSwirlGridBasePointer->ring[idx / 4 + 1][0].y;
            fraction = ((idx % 4 << 12) / 4);
            return lowHeight + ((highHeight - lowHeight) * fraction >> 12);
        } else {
            return 0;
        }
    }
}

// On first shared use saves a data-block pointer. Uses retained colour below age ten, then shades
// it by (age minus ten) times 4096 divided by five and sets command 0x2A during ages ten through
// fourteen; at ages at least fifteen retires and returns before projection or pause. Projects task
// position through g_WhirlsandCompositeMatrix and when flags lack 0x60000 draws a two-by-two PSX quad
// with equal projected NDC Z/W at all corners, without requiring positive projection. While
// unpaused increments age, rotates the position with Y increased by vertical velocity around Y by
// 128 and scales that velocity by 0xE00 shifted by twelve.
// FUNCTION: FF7 0x00451628
void __cdecl BattleFx_WhirlsandSurfaceParticleTask(void)
{
    struct {
        BattleFxWhirlsandQuadPacket *packet;
        int unused84, unused80;
        float z4, w4;
        int unused68, unused64;
        float z2, w2;
        int t;
        int unused48, unused44;
        float z3, w3;
        int unused32, unused28;
        float z1, w1;
        int unused16;
        struct BattleFxWhirlsandSurfaceParticleTaskSlot *slot;
        int age;
        int size;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.slot = (struct BattleFxWhirlsandSurfaceParticleTaskSlot *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_WhirlsandDataBlock0InitFlag & 1) == 0) {
        g_WhirlsandDataBlock0InitFlag |= 1;
        g_WhirlsandDataBlock0 = (BattleFxWhirlsandParticleScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    f.packet = (BattleFxWhirlsandQuadPacket *)Gfx_GetPsxPacketScratchPtr();
    f.age = f.slot->age;
    if (f.age < 10) {
        *(int *)&f.packet->color = g_WhirlsandTransientQuadColorCommand;
    } else {
        f.age -= 10;
        if (f.age < 5) {
            f.t = (f.age << 12) / 5;
            Gfx_ShadePsxVertexColor((CVECTOR *)&g_WhirlsandTransientQuadColorCommand, f.t, &f.packet->color);
            f.packet->color.cd = 0x2a;
        } else {
            f.slot->ownerSlot = -1;
            return;
        }
    }
    SetRotMatrix(&g_WhirlsandCompositeMatrix);
    SetTransMatrix(&g_WhirlsandCompositeMatrix);
    RefreshGlobalTransformState();
    g_WhirlsandDataBlock0->projectResult = ProjectPointHardware(&f.slot->posX, &f.packet->x0, &g_WhirlsandDataBlock0->projectDepth, &g_WhirlsandDataBlock0->projectFlags);
    if ((g_WhirlsandDataBlock0->projectFlags & 0x60000) == 0) {
        f.size = 2;
        f.packet->code = 0x5000000;
        f.packet->x2 = f.packet->x0;
        f.packet->x3 = f.packet->x2 + f.size;
        f.packet->x1 = f.packet->x3;
        f.packet->y1 = f.packet->y0;
        f.packet->y3 = f.packet->y1 + f.size;
        f.packet->y2 = f.packet->y3;
        f.z1 = f.z3 = f.z2 = f.z4 = GetLastProjectedNdcZ();
        f.w1 = f.w3 = f.w2 = f.w4 = GetLastProjectedNdcW();
        Gfx_SetBlendFactorQuad(f.z1, f.w1, f.z2, f.w2, f.z3, f.w3, f.z4, f.w4);
        Gfx_DrawPsxPrimitive(f.packet);
    }
    if (g_BattleIsPaused == 0) {
        (f.slot->age)++;
        g_WhirlsandDataBlock0->angle[0] = 0;
        g_WhirlsandDataBlock0->angle[1] = 0x80;
        g_WhirlsandDataBlock0->angle[2] = 0;
        RotMatrixXYZ(g_WhirlsandDataBlock0->angle, &g_WhirlsandDataBlock0->matrix);
        g_WhirlsandDataBlock0->angle[0] = f.slot->posX;
        g_WhirlsandDataBlock0->angle[1] = f.slot->posY + f.slot->velocityY;
        g_WhirlsandDataBlock0->angle[2] = f.slot->posZ;
        TransformVectorToInt16(&g_WhirlsandDataBlock0->matrix, g_WhirlsandDataBlock0->angle, &f.slot->posX);
        f.slot->velocityY = f.slot->velocityY * 0xe00 /* 0.875 */ >> 12;
    }
}

// For an actor with flag 0x80 set and flags 4/2 clear, allocates BattleFx_WhirlsandActorSpiralAwayTask when its
// g_WhirlsandAngle bit is set or BattleFx_WhirlsandActorGroundRideTask otherwise, and stores the actor in the slot first word.
// FUNCTION: FF7 0x004518ea
void __cdecl BattleFx_WeaponSpawnWhirlsandPerTarget(int actor)
{
    BattleFxWhirlsandRideSlot *slotPtr;

    if ((*(unsigned char *)&g_BattleActorRecords[actor].drawFlags & 0x80) != 0 && (*(unsigned char *)&g_BattleActorRecords[actor].drawFlags & 4) == 0 && (*(unsigned char *)&g_BattleActorRecords[actor].drawFlags & 2) == 0) {
        if ((g_WhirlsandAngle & 1 << actor) != 0) {
            slotPtr = (BattleFxWhirlsandRideSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_WhirlsandActorSpiralAwayTask);
        } else {
            slotPtr = (BattleFxWhirlsandRideSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_WhirlsandActorGroundRideTask);
        }
        slotPtr->actorIndex = actor;
    }
}

// When unpaused initializes the slot from its actor position and saves actor flags, setting mask
// 0x10; on later counters below 120 calls BattleFx_WhirlsandSetActorGroundHeightAndDrawShadow, restores the saved position component and
// flag masks at counter 119, then advances the counter. Retires the slot on the next call at
// counter 120 or above.
// FUNCTION: FF7 0x0045199b
void __cdecl BattleFx_WhirlsandActorGroundRideTask(void)
{
    int localStep;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxWhirlsandRideSlot *ride;
    struct BattleActor *actor;

    ride = (BattleFxWhirlsandRideSlot *)BATTLE_CURRENT_TASK_SLOT;
    actor = &g_BattleActorRecords[ride->actorIndex];
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (ride->step == 0) {
        *(Int64Pair *)&ride->startOffset = *(Int64Pair *)&actor->posX;
        ride->actorFlags = (*(unsigned char *)&actor->drawFlags);
        actor->drawFlags |= 0x10;
        (ride->step)++;
    } else {
        localStep = ride->step;
        if (localStep < 120) {
            BattleFx_WhirlsandSetActorGroundHeightAndDrawShadow(actor, ride);
            if (localStep == 119) {
                actor->posY = ride->heightOffset;
                actor->drawFlags = ((*(unsigned char *)&actor->drawFlags) & ~0x14) | ride->actorFlags & 0x14;
            }
        } else {
            ride->actorIndex = 0xFFFF;
            return;
        }
        (ride->step)++;
    }
}

// On first shared use saves a data-block pointer. Sets g_WhirlsandSceneMeshDrawDescriptor to scene field eight or zero
// and writes the actor's posY as the slot's heightOffset plus BattleFx_WhirlsandGetRingGridHeightAtPoint of the actor's position. Unless
// slot status short has mask 0x10 builds a zero-rotation matrix: when the actor's modelSlot is minus one
// sets both X/Y translation to negative heightOffset and Z zero, otherwise uses bone0PosX and bone0PosZ
// plus the retained short. Scales with the actor's scaleX and scaleY as X/Z and zero Y, composes
// with the actor's rootNode matrix and submits the retained descriptor to destination 0x406C/argument zero.
// Scene absence supplies a zero resource pointer but does not suppress submission.
// No pause branch occurs.
// FUNCTION: FF7 0x00451aa2
void __cdecl BattleFx_WhirlsandSetActorGroundHeightAndDrawShadow(struct BattleActor *actor, BattleFxWhirlsandRideSlot *slot)
{
    if ((g_WhirlsandDataBlock1InitFlag & 1) == 0) {
        g_WhirlsandDataBlock1InitFlag |= 1;
        g_WhirlsandDataBlock1 = (BattleFxWhirlsandRideScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_pBattle3dScene != 0) {
        (*(int *)&g_WhirlsandSceneMeshDrawDescriptor) = g_pBattle3dScene->modelRequest;
    } else {
        (*(int *)&g_WhirlsandSceneMeshDrawDescriptor) = 0;
    }
    actor->posY = slot->heightOffset + BattleFx_WhirlsandGetRingGridHeightAtPoint((BattleFxRingGridPoint *)&actor->posX);
    if ((slot->actorFlags & 0x10) == 0) {
        g_WhirlsandDataBlock1->angle[2] = 0;
        g_WhirlsandDataBlock1->angle[1] = 0;
        g_WhirlsandDataBlock1->angle[0] = 0;
        RotMatrix((SVECTOR *)g_WhirlsandDataBlock1->angle, &g_WhirlsandDataBlock1->matrix);
        if (actor->modelSlot == -1) {
            g_WhirlsandDataBlock1->matrix.t[1] = -slot->heightOffset;
            g_WhirlsandDataBlock1->matrix.t[0] = g_WhirlsandDataBlock1->matrix.t[1];
            g_WhirlsandDataBlock1->matrix.t[2] = 0;
        } else {
            g_WhirlsandDataBlock1->matrix.t[0] = actor->bone0PosX;
            g_WhirlsandDataBlock1->matrix.t[1] = -slot->heightOffset;
            g_WhirlsandDataBlock1->matrix.t[2] = actor->bone0PosZ + actor->modelSlot;
        }
        g_WhirlsandDataBlock1->scale[0] = actor->scaleX;
        g_WhirlsandDataBlock1->scale[1] = 0;
        g_WhirlsandDataBlock1->scale[2] = actor->scaleY;
        ScaleMatrix(&g_WhirlsandDataBlock1->matrix, g_WhirlsandDataBlock1->scale);
        MultiplyMatrix(actor->rootNode, &g_WhirlsandDataBlock1->matrix, &g_WhirlsandDataBlock1->matrix);
        SetRotMatrix(&g_WhirlsandDataBlock1->matrix);
        SetTransMatrix(&g_WhirlsandDataBlock1->matrix);
        g_BattleOverlayDrawListHead = (int)BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_WhirlsandSceneMeshDrawDescriptor, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST, 0, g_BattleOverlayDrawListHead);
    }
}

// Selects actor/model records from task short zero and saves shared scratch on first use, then
// returns while paused. At count zero saves eight-byte actor position/rotation and status byte,
// sets actor flag mask 0x10 and advances count. Later below ninety calls BattleFx_WhirlsandSetActorGroundHeightAndDrawShadow; from
// fifteen through 74 allocates a typed record at fifteen, shapes a sixty-count fraction through
// BattleFx_WhirlsandApplyCameraEasing with -767, advances the shared Y rotation and rotates/scales the saved X/Z offset
// toward g_WhirlsandBaseMatrix translation while increasing the actor's yaw by 256. From 75
// through 89 clears three colour bytes, writes a fifteen-count fraction to the actor's tintPhase, sets
// mask eight across both signed-count actor/model byte arrays and advances the same rotation. At
// counts at least ninety restores saved position/rotation, ORs actor flag mask six and retires
// without restoring the saved status byte or clearing mask 0x10 here. Otherwise increments count.
// FUNCTION: FF7 0x00451ca4
void __cdecl BattleFx_WhirlsandActorSpiralAwayTask(void)
{
    int stage;
    int eased;
    BattleFxWhirlsandRideSlot *taskPtr;
    BattleActorTransformSlot *xformRecord;
    int tempRatio;
    int boneIndex;
    struct BattleActor *actor;

    taskPtr = (BattleFxWhirlsandRideSlot *)BATTLE_CURRENT_TASK_SLOT;
    actor = &g_BattleActorRecords[taskPtr->actorIndex];
    xformRecord = (BattleActorTransformSlot *)((taskPtr->actorIndex << 6) + (char *)&g_BattleActorTransformRecords);
    if ((g_WhirlsandDataBlock5InitFlag & 1) == 0) {
        g_WhirlsandDataBlock5InitFlag |= 1;
        g_WhirlsandDataBlock5 = (struct BattleFxWhirlsandSpiralOffset *)(Gfx_GetPsxScratchpadPtr(0));
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->step == 0) {
        *(Int64Pair *)&taskPtr->startOffset = *(Int64Pair *)&actor->posX;
        *(Int64Pair *)&taskPtr->savedRotX = *(Int64Pair *)&actor->rotX;
        taskPtr->actorFlags = *(unsigned char *)&actor->drawFlags;
        actor->drawFlags |= 0x10;
        (taskPtr->step)++;
    } else {
        stage = taskPtr->step;
        if (stage < 90) {
            BattleFx_WhirlsandSetActorGroundHeightAndDrawShadow(actor, taskPtr);
            if (stage < 15) { /* empty first arm: lib/codegen.h */
            } else {
                stage -= 15;
                if (stage < 60) {
                    if (stage == 0) {
                        Battle_PostActorHitOrHurtMessage(taskPtr->actorIndex);
                    }
                    tempRatio = (stage << 12) / 60;
                    eased = BattleFx_WhirlsandApplyCameraEasing(tempRatio, 0xFFFFFD01);
                    g_WhirlsandActorOffsetAngles[1] += (short)(tempRatio << 8 >> 12);
                    RotMatrixXYZ(&g_WhirlsandActorOffsetAngles, &g_WhirlsandDataBlock5->matrix);
                    g_WhirlsandDataBlock5->offsetX = taskPtr->startOffset * (0x1000 - eased) >> 12;
                    g_WhirlsandDataBlock5->offsetY = 0;
                    g_WhirlsandDataBlock5->offsetZ = (taskPtr->targetPos - g_WhirlsandBaseMatrix.t[2]) * (0x1000 - eased) >> 12;
                    TransformVectorToInt16(&g_WhirlsandDataBlock5->matrix, g_WhirlsandDataBlock5, &g_WhirlsandDataBlock5->posX);
                    actor->posX = g_WhirlsandDataBlock5->posX;
                    actor->posZ = g_WhirlsandDataBlock5->posZ + g_WhirlsandBaseMatrix.t[2];
                    actor->yaw += 0x100 /* 22.5 deg */;
                } else {
                    stage -= 60;
                    if (stage < 15) {
                        tempRatio = (stage << 12) / 15;
                        actor->tintB = 0;
                        actor->tintG = 0;
                        actor->tintR = 0;
                        actor->tintPhase = tempRatio;
                        for (boneIndex = 0; boneIndex < actor->boneCount; boneIndex++) {
                            (&actor->fadeFlags)[boneIndex] |= 8;
                        }
                        for (boneIndex = 0; boneIndex < xformRecord->boneCount; boneIndex++) {
                            (&xformRecord->fadeFlags)[boneIndex] |= 8;
                        }
                        actor->yaw += 0x100 /* 22.5 deg */;
                    }
                }
            }
        } else {
            *(Int64Pair *)&actor->posX = *(Int64Pair *)&taskPtr->startOffset;
            *(Int64Pair *)&actor->rotX = *(Int64Pair *)&taskPtr->savedRotX;
            actor->drawFlags |= 6;
            taskPtr->actorIndex = 0xFFFF;
            return;
        }
        (taskPtr->step)++;
    }
}
} /* extern "C" */
