// src/ff7/src/battle/battle_fx_builtin_hit_sparks.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Built-in flash effects, each keyed to a sound, that spawn hit-spark emitters on the
//   target's bones. Shared spawn, emitter and particle tasks place the sprites, while variant
//   emitters differ in colour, texture and lifetime.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "ff7/battle_tasks.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008ff078
unsigned char g_BattleFxHitSparkAngleOffsets[16] = {0x00, 0x20, 0x40, 0x60, 0x80, 0xa0, 0xc0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008ff088
unsigned char g_BattleFxHitSparkRandomTexCoords[16] = {0x07, 0x78, 0x47, 0x78, 0x47, 0x79, 0x87, 0x78, 0xc7, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on


/*
 * Creates callback BattleFx_SpawnHitSparkEmitter with argument zero for fewer than five selected
 * bits, otherwise ten; then dispatches sound opcode 0x20 with mask-derived pan and identifier six.
 */
// FUNCTION: FF7 0x005d4390
void __cdecl BattleFx_BuiltinFlashSnd06(unsigned int targetMask, int casterIndex)
{
    Battle_CreateBitPatternCallbackTask(
        targetMask, casterIndex,
        (Battle_PopCountActorMask(targetMask) < 5) ? 0 : 10, BattleFx_SpawnHitSparkEmitter);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 6);
}

/*
 * Allocates hit-spark emitter task record and initializes short parameter 0x12, short 0x14 zero,
 * byte 0xD zero, short 0xE0x7807 and short 0x160x3A.
 */
// FUNCTION: FF7 0x005d43df
void __cdecl BattleFx_SpawnHitSparkEmitter(short actor)
{
    BattleFxHitLivewireEmitter *rec;

    rec = (BattleFxHitLivewireEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_HitSparkEmitterTask);
    rec->actorIndex = actor;
    rec->boneIndex = 0;
    rec->cellRow = 0;
    rec->texCoord = 0x7807;
    rec->spriteFlags = 0x3A;
}

/*
 * Unpaused emits two hit particles at successive randomized actor bones with modulo 500 jitter and
 * copied parent selector/frame/parameter plus random scale. Advances counter and at thirty or more
 * allocates typed actor and retires.
 */
// FUNCTION: FF7 0x005d442f
void __cdecl BattleFx_HitSparkEmitterTask(void)
{
    BattleFxHitLivewireEmitter *pParent;
    int q;
    BattleFxHitLivewireParticle *particle;
    int i;

    pParent = (BattleFxHitLivewireEmitter *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused == 0) {
        for (i = 0; i < 2; i++) {
            pParent->boneIndex =
                (pParent->boneIndex + (rand() & 3) + 1) %
                g_BattleActorRecords[pParent->actorIndex].boneCount;
            particle = (BattleFxHitLivewireParticle *)&g_BattleTaskSlotBuffer.mainSlots[Battle_AllocateResourceSlot(BattleFx_HitSparkParticleTask)];
            Battle_GetBoneWorldPos(pParent->actorIndex, pParent->boneIndex,
                                   (short *)((int)particle + 4));
            particle->posX += rand() % 0x1F4 - 0xFA;
            particle->posY += rand() % 0x1F4 - 0xFA;
            particle->posZ += rand() % 0x1F4 - 0xFA;
            particle->cellRow = pParent->cellRow;
            particle->texCoord = pParent->texCoord;
            particle->scale = (rand() & 0xFFF) + 0x1000;
            particle->spriteFlags = pParent->spriteFlags;
        }
        (pParent->frameCount)++;
        if (pParent->frameCount >= 0x1E) {
            Battle_PostActorHitOrHurtMessage(pParent->actorIndex);
            pParent->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Installs billboard matrix with depth push minus 500 and submits centered 31-square sprite using
 * table UV and packed color. Unpaused increments frame and retires at eight when sprite selector
 * is 0x80, otherwise nine; draw precedes increment and returned chain is ignored.
 */
// FUNCTION: FF7 0x005d45ca
void __cdecl BattleFx_HitSparkParticleTask(void)
{
    BattleFxSpriteQuadRequest *request;
    BattleFxHitLivewireSlot *task;

    task = (BattleFxHitLivewireSlot *)BATTLE_CURRENT_TASK_SLOT;
    request = (BattleFxSpriteQuadRequest *)Gfx_GetPsxScratchpadPtr(0);
    BattleFx_InstallBillboardMatrix((char *)task + 4, (short)task->scale, 0xFFFFFE0C);
    request->offsetY = 0xFFF0;
    request->offsetX = 0xFFF0;
    request->cellX = *(char *)(((char *)&g_BattleFxHitSparkAngleOffsets) + task->timer);
    request->cellY = task->cellRow;
    request->widthUnits = 0x1F;
    request->heightUnits = 0x1F;
    request->flags = task->flags;
    request->cellWord = task->cellWord;
    *(int *)((char *)request + 8) = 0x2E808080;
    request->quadState = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)request, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
    if (g_BattleIsPaused == 0) {
        (task->timer)++;
        if (task->timer >= (*(unsigned char *)&task->cellRow != 0x80) + 8) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Built-in flash effect with sound 8: spawns the hit-spark record task BattleFx_SpawnHitSparkEmitterSnd08 on each target
 * (delayed 10 frames when five or more targets) and plays sound 8 panned to the acting entity.
 */
// FUNCTION: FF7 0x005d46e9
void __cdecl BattleFx_BuiltinFlashSnd08(unsigned int targetMask, int casterIndex)
{
    Battle_CreateBitPatternCallbackTask(
        targetMask, casterIndex,
        (Battle_PopCountActorMask(targetMask) < 5) ? 0 : 10, BattleFx_SpawnHitSparkEmitterSnd08);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 8);
}

/*
 * Allocates a hit-spark emitter record for target actor: colour byte 0x40, texture word 0x7947,
 * lifetime 0x3A.
 */
// FUNCTION: FF7 0x005d4738
void BattleFx_SpawnHitSparkEmitterSnd08(short actor)
{
    BattleFxHitLivewireEmitter *rec;

    rec = (BattleFxHitLivewireEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_HitSparkEmitterTask);
    rec->actorIndex = actor;
    rec->boneIndex = 0;
    rec->cellRow = 0x40;
    rec->texCoord = 0x7947;
    rec->spriteFlags = 0x3A;
}

/*
 * Built-in flash effect with sound 0x45: hit sparks with a random row (BattleFx_SpawnHitSparkEmitterRandomRow) and sound
 * 0x45.
 */
// FUNCTION: FF7 0x005d4788
void __cdecl BattleFx_BuiltinFlashSnd45(unsigned int targetMask, int casterIndex)
{
    Battle_CreateBitPatternCallbackTask(
        targetMask, casterIndex,
        (Battle_PopCountActorMask(targetMask) < 5) ? 0 : 10, BattleFx_SpawnHitSparkEmitterRandomRow);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x45);
}

/*
 * Allocates a random-row hit-spark emitter record for target actor with lifetime 0x3A.
 */
// FUNCTION: FF7 0x005d47d7
void BattleFx_SpawnHitSparkEmitterRandomRow(short actor)
{
    BattleHitLivewireShortEmitter *rec;

    rec = (BattleHitLivewireShortEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_HitSparkEmitterTaskRandomRow);
    rec->targetIndex = actor;
    rec->boneIndex = 0;
    rec->lifetime = 0x3A;
}

/*
 * Unpaused emits two particles at randomized successive actor bones, adds modulo 500 position
 * jitter, chooses one of three UV rows and five frame values, assigns random scale and copied
 * parameter. Counter advances and retires with typed actor at thirty.
 */
// FUNCTION: FF7 0x005d4817
void __cdecl BattleFx_HitSparkEmitterTaskRandomRow(void)
{
    BattleFxHitLivewireRowParticle *particle;
    int i;
    int unusedWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxHitLivewireEmitter *pEmitter;

    pEmitter = (BattleFxHitLivewireEmitter *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused == 0) {
        for (i = 0; i < 2; i++) {
            particle = (BattleFxHitLivewireRowParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_HitSparkParticleTask);
            pEmitter->boneIndex = (pEmitter->boneIndex + (rand() & 3) + 1) % g_BattleActorRecords[pEmitter->actorIndex].boneCount;
            Battle_GetBoneWorldPos(pEmitter->actorIndex, pEmitter->boneIndex, (short *)((char *)particle + 4));
            particle->posX += (short)(rand() % 0x1F4 - 0xFA);
            particle->posY += (short)(rand() % 0x1F4 - 0xFA);
            particle->posZ += (short)(rand() % 0x1F4 - 0xFA);
            particle->texRow = rand() % 3 << 5;
            particle->texCoord = ((short *)&g_BattleFxHitSparkRandomTexCoords)[rand() % 5];
            particle->scale = (rand() & 0xFFF) + 0x1000;
            particle->spriteFlags = pEmitter->spriteFlags;
        }
        (pEmitter->frameCount)++;
        if (pEmitter->frameCount >= 0x1E) {
            Battle_PostActorHitOrHurtMessage(pEmitter->actorIndex);
            pEmitter->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Built-in flash effect with sound 0x5C: short hit sparks (BattleFx_SpawnHitSparkEmitterShort) and sound 0x5C.
 */
// FUNCTION: FF7 0x005d49c9
void __cdecl BattleFx_BuiltinFlashSnd5C(unsigned int targetMask, int casterIndex)
{
    Battle_CreateBitPatternCallbackTask(
        targetMask, casterIndex,
        (Battle_PopCountActorMask(targetMask) < 5) ? 0 : 10, BattleFx_SpawnHitSparkEmitterShort);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x5c);
}

/*
 * Allocates a short hit-spark emitter record for target actor with lifetime 0x3A.
 */
// FUNCTION: FF7 0x005d4a18
void BattleFx_SpawnHitSparkEmitterShort(short actor)
{
    BattleHitLivewireShortEmitter *rec;

    rec = (BattleHitLivewireShortEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_HitSparkEmitterTaskShort);
    rec->targetIndex = actor;
    rec->boneIndex = 0;
    rec->lifetime = 0x3A;
}

/*
 * Unpaused emits two hit particles per call using randomized next bone, bitmask 0x1f4 position
 * jitter, selector 0x20/0x80, fixed frame 0x7887 and random scale. Postincrement counter
 * retires/allocates typed actor when old value at least fifteen, so normal start emits sixteen
 * pairs.
 */
// FUNCTION: FF7 0x005d4a58
void __cdecl BattleFx_HitSparkEmitterTaskShort(void)
{
    BattleFxHitLivewireEmitter *pParent;
    int q;
    BattleFxHitLivewireShortParticle *particle;
    int i;

    pParent = (BattleFxHitLivewireEmitter *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused == 0) {
        for (i = 0; i < 2; i++) {
            particle = (BattleFxHitLivewireShortParticle *)&g_BattleTaskSlotBuffer.mainSlots[Battle_AllocateResourceSlot(BattleFx_HitSparkParticleTask)];
            pParent->boneIndex =
                (pParent->boneIndex + (rand() & 3) + 1) %
                g_BattleActorRecords[pParent->actorIndex].boneCount;
            Battle_GetBoneWorldPos(pParent->actorIndex, pParent->boneIndex,
                                   (short *)((int)particle + 4));
            particle->posX += (rand() & 0x1F4) - 0xFA;
            particle->posY += (rand() & 0x1F4) - 0xFA;
            particle->posZ += (rand() & 0x1F4) - 0xFA;
            particle->selector = (rand() & 1) * 0x60 + 0x20;
            particle->texCoord = 0x7887;
            particle->scale = (rand() & 0xFFF) + 0x1000;
            particle->spriteFlags = pParent->spriteFlags;
        }
        if ((pParent->frameCount)++ >= 0xF) {
            Battle_PostActorHitOrHurtMessage(pParent->actorIndex);
            pParent->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Built-in flash effect with sound 0x5B: hit sparks (BattleFx_SpawnHitSparkEmitterSnd5B) and sound 0x5B.
 */
// FUNCTION: FF7 0x005d4be9
void __cdecl BattleFx_BuiltinFlashSnd5B(unsigned int targetMask, int casterIndex)
{
    Battle_CreateBitPatternCallbackTask(
        targetMask, casterIndex,
        (Battle_PopCountActorMask(targetMask) < 5) ? 0 : 10, BattleFx_SpawnHitSparkEmitterSnd5B);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x5b);
}

/*
 * Allocates a hit-spark emitter record for target actor: colour byte 0x60, texture word 0x7887,
 * lifetime 0x3A.
 */
// FUNCTION: FF7 0x005d4c38
void BattleFx_SpawnHitSparkEmitterSnd5B(short actor)
{
    BattleFxHitLivewireEmitter *rec;

    rec = (BattleFxHitLivewireEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_HitSparkEmitterTask);
    rec->actorIndex = actor;
    rec->boneIndex = 0;
    rec->cellRow = 0x60;
    rec->texCoord = 0x7887;
    rec->spriteFlags = 0x3A;
}

/*
 * Built-in flash effect with sound 0x1F, variant a: hit sparks (BattleFx_SpawnHitSparkEmitterSnd1Fa) and sound 0x1F.
 */
// FUNCTION: FF7 0x005d4c88
void __cdecl BattleFx_BuiltinFlashSnd1Fa(unsigned int targetMask, int actingActor)
{
    Battle_CreateBitPatternCallbackTask(
        targetMask, actingActor,
        (Battle_PopCountActorMask(targetMask) < 5) ? 0 : 10, BattleFx_SpawnHitSparkEmitterSnd1Fa);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x1f);
}

/*
 * Allocates a hit-spark emitter record for target actor: colour byte 0, texture word 0x7A47,
 * lifetime 0x3A.
 */
// FUNCTION: FF7 0x005d4cd7
void BattleFx_SpawnHitSparkEmitterSnd1Fa(short actor)
{
    BattleFxHitLivewireEmitter *rec;

    rec = (BattleFxHitLivewireEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_HitSparkEmitterTask);
    rec->actorIndex = actor;
    rec->boneIndex = 0;
    rec->cellRow = 0;
    rec->texCoord = 0x7a47;
    rec->spriteFlags = 0x3A;
}

/*
 * Built-in flash effect with sound 0x1F, variant b: hit sparks (BattleFx_SpawnHitSparkEmitterSnd1Fb) and sound 0x1F.
 */
// FUNCTION: FF7 0x005d4d27
void __cdecl BattleFx_BuiltinFlashSnd1Fb(unsigned int targetMask, int actingActor)
{
    Battle_CreateBitPatternCallbackTask(
        targetMask, actingActor,
        (Battle_PopCountActorMask(targetMask) < 5) ? 0 : 10, BattleFx_SpawnHitSparkEmitterSnd1Fb);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x1f);
}

/*
 * Allocates a hit-spark emitter record for target actor: colour byte 0x20, texture word 0x7907,
 * lifetime 0x3A.
 */
// FUNCTION: FF7 0x005d4d76
void BattleFx_SpawnHitSparkEmitterSnd1Fb(short actor)
{
    BattleFxHitLivewireEmitter *rec;

    rec = (BattleFxHitLivewireEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_HitSparkEmitterTask);
    rec->actorIndex = actor;
    rec->boneIndex = 0;
    rec->cellRow = 0x20;
    rec->texCoord = 0x7907;
    rec->spriteFlags = 0x3A;
}

/*
 * Built-in flash effect with sound 0x17E: hit sparks (BattleFx_SpawnHitSparkEmitterSnd17E) and sound 0x17E.
 */
// FUNCTION: FF7 0x005d4dc6
void __cdecl BattleFx_BuiltinFlashSnd17E(unsigned int targetMask, int actingActor)
{
    Battle_CreateBitPatternCallbackTask(
        targetMask, actingActor,
        (Battle_PopCountActorMask(targetMask) < 5) ? 0 : 10, BattleFx_SpawnHitSparkEmitterSnd17E);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x17e);
}

/*
 * Allocates a hit-spark emitter record for target actor: colour byte 0x20, texture word 0x78C7,
 * lifetime 0x3A.
 */
// FUNCTION: FF7 0x005d4e18
void BattleFx_SpawnHitSparkEmitterSnd17E(short actor)
{
    BattleFxHitLivewireEmitter *rec;

    rec = (BattleFxHitLivewireEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_HitSparkEmitterTask);
    rec->actorIndex = actor;
    rec->boneIndex = 0;
    rec->cellRow = 0x20;
    rec->texCoord = 0x78c7;
    rec->spriteFlags = 0x3A;
}

/*
 * Built-in flash effect with sound 0x166: hit sparks (BattleFx_SpawnHitSparkEmitterSnd166) with no delay and sound 0x166.
 */
// FUNCTION: FF7 0x005d4e68
void BattleFx_BuiltinFlashSnd166(unsigned int targetMask, int actingActor)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actingActor, 0, BattleFx_SpawnHitSparkEmitterSnd166);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x166);
}

/*
 * Allocates a hit-spark emitter record for target actor: colour byte 0, texture word 0x7A07,
 * lifetime 0x3A.
 */
// FUNCTION: FF7 0x005d4ea0
void BattleFx_SpawnHitSparkEmitterSnd166(short actor)
{
    BattleFxHitLivewireEmitter *rec;

    rec = (BattleFxHitLivewireEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_HitSparkEmitterTask);
    rec->actorIndex = actor;
    rec->boneIndex = 0;
    rec->cellRow = 0;
    rec->texCoord = 0x7a07;
    rec->spriteFlags = 0x3A;
}

/*
 * Built-in flash effect with sound 7: hit sparks (BattleFx_SpawnHitSparkEmitterSnd07) and sound 7.
 */
// FUNCTION: FF7 0x005d4ef0
void __cdecl BattleFx_BuiltinFlashSnd07(unsigned int targetMask, int actingActor)
{
    Battle_CreateBitPatternCallbackTask(
        targetMask, actingActor,
        (Battle_PopCountActorMask(targetMask) < 5) ? 0 : 10, BattleFx_SpawnHitSparkEmitterSnd07);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 7);
}

/*
 * Allocates a hit-spark emitter record for target actor: colour byte 0x20, texture word 0x7847,
 * lifetime 0x3A.
 */
// FUNCTION: FF7 0x005d4f3f
void BattleFx_SpawnHitSparkEmitterSnd07(short actor)
{
    BattleFxHitLivewireEmitter *rec;

    rec = (BattleFxHitLivewireEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_HitSparkEmitterTask);
    rec->actorIndex = actor;
    rec->boneIndex = 0;
    rec->cellRow = 0x20;
    rec->texCoord = 0x7847;
    rec->spriteFlags = 0x3A;
}
} /* extern "C" */
