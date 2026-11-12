// src/ff7/src/battle/battle_hit_damage_resolution.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle hit and damage resolution: it queues attack animation and HP/MP transfer steps,
//   shows damage, applies cover redirects and elemental and status effects, and holds a few AI
//   opcode helpers that feed them.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/enums/battle.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
#include "ff7/savemap.h"
#include "ff7/types/menu.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008ff1e8
struct BattleAnimPoseTranslation g_BattleStatusAnimPoseTranslation[7] = {{0x14, 21}, {0x22, 35}, {0x24, 37}, {0x26, 39}, {0x28, 41}, {0x2a, 43}, {0x2c, 45}};
// GLOBAL: FF7 0x008ff1f8
BattleDmgFormulaFn g_BattleDamageFormulaTable[32] = {
    Battle_DmgFormula00_NoDamage, Battle_DmgFormula01_Physical, Battle_DmgFormula02_Magical, Battle_DmgFormula03_PercentOfCurrent,
    Battle_DmgFormula04_PercentOfMax, Battle_DmgFormula05_MagicalNoDefence, Battle_DmgFormula06_PowerTimes20, Battle_DmgFormula07_PowerVsDefence,
    Battle_DmgFormula08_DeathOrFullCure, Battle_DmgFormula09_WeaponPowerPhysical, Battle_DmgFormula0A_PowerSplitByTargets, 0,
    0, 0, 0, 0,
    Battle_DmgFormula10_AttackerHp, Battle_DmgFormula11_AttackerHpMissing, Battle_DmgFormula12_Stub, Battle_DmgFormula13_Stub,
    Battle_DmgFormula14_Stub, Battle_DmgFormula15_Stub, Battle_DmgFormula16_Stub, Battle_DmgFormula17_Stub,
    Battle_DmgFormula18_DiceRoll, Battle_DmgFormula19_SavemapCounter, Battle_DmgFormula1A_TargetHpMinusOne, Battle_DmgFormula1B_PlayTime,
    Battle_DmgFormula1C_KillCountTimes10, Battle_DmgFormula1D_MateriaCountTimes1111, 0, 0};
// GLOBAL: FF7 0x00c3f338
int g_BattleAiVmActorIndex = 0;
// GLOBAL: FF7 0x00c3f33c
int g_BattleAiVmActionContext = 0;
// GLOBAL: FF7 0x00c3f340
unsigned short g_BattleActorCurrentTargetMask[10] = {0};
unsigned char g_BattleHitAlignPad[4] = {0};
// GLOBAL: FF7 0x00c3f358
char g_BattleActorReflectSource[10] = {0};
// GLOBAL: FF7 0x00c3f364
int g_BattleAttackOpStateFlags = 0;
// GLOBAL: FF7 0x00c3f368
short g_BattleActorNextTargetMask[10] = {0};
// GLOBAL: FF7 0x00c3f37c
unsigned short g_BattleAttackReactedActorMask = 0;
// clang-format on

// The opposed status pairs: inflicting one status of a pair cures the other (Slow and Haste, Sadness and Fury).
// GLOBAL: FF7 0x007b7738
extern const int g_BattleOpposedStatusPairs[2][2] = {{BATTLE_STATUS_SLOW, BATTLE_STATUS_HASTE}, {BATTLE_STATUS_SADNESS, BATTLE_STATUS_FURY}};

/*
 * Gives an AI script its view of the battle before it runs: the script's own bit, and the two
 * sides' actors split into those still able to act and the incapacitated ones
 * (g_BattleScene.incapacitatedMask), seen from the actor's side. Battle_GetActorSide flips the side
 * of a confused or manipulated actor, so its script sees friend and foe swapped. The default target
 * mask (lastActorMask) is the first of these masks without the absent actors; the party's gil is
 * copied in too, for scripts that test it.
 *
 * The VM actor and context are stored even for a negative actor, which then returns early. The read
 * of the actor's status is unused.
 */
// FUNCTION: FF7 0x005d9550
void __cdecl Battle_AiVm_PrepareGlobals(int actorIndex, int vmActor, int actionContext)
{
    int otherMask;
    int activeMask;
    int sideMask;
    int sideBMatch;
    int sideAMatch;
    int taskOutsideA;
    int oppositeB;
    int statusWord;
    int swap;

    g_BattleAiVmActorIndex = vmActor;
    g_BattleAiVmActionContext = actionContext;
    if (actorIndex < 0) {
        return;
    }
    statusWord = g_aCombatants[actorIndex].status;
    sideMask = (g_BattleScene.incapacitatedMask);
    activeMask = gBattleAiGlobals.targetMask & (*(unsigned short *)&g_BattleScene.targetExtraMask);
    otherMask = *(unsigned short *)&gBattleAiGlobals.enemyMask & (*(unsigned short *)&g_BattleScene.targetExtraMask);
    taskOutsideA = activeMask & ~sideMask;
    sideAMatch = activeMask & sideMask;
    oppositeB = otherMask & ~sideMask;
    sideBMatch = otherMask & sideMask;
    if (Battle_GetActorSide(actorIndex) != 0) {
        swap = taskOutsideA;
        taskOutsideA = oppositeB;
        oppositeB = swap;
        swap = sideAMatch;
        sideAMatch = sideBMatch;
        sideBMatch = swap;
    }
    taskOutsideA &= ~g_BattleState.absentActorMask;
    gBattleAiGlobals.actorBit = 1 << actorIndex;
    gBattleAiGlobals.oppositeB = oppositeB;
    gBattleAiGlobals.sideBMatch = sideBMatch;
    gBattleAiGlobals.opponentMask = taskOutsideA;
    gBattleAiGlobals.sideAMatch = sideAMatch;
    gBattleAiGlobals.lastActorMask = taskOutsideA;
    gBattleAiGlobals.targetableActorMask = g_BattleScene.targetMask & *(unsigned short *)&gBattleAiGlobals.actorMask;
    gBattleAiGlobals.gil = gSavemap.gil;
}

/*
 * The kernel's attack ids: the summons start at 0x38 and the enemy skills at 0x48 (well
 * established; Battle_LearnEnemySkill makes the same subtraction).
 */
#define BATTLE_KERNEL_SUMMON_FIRST 0x38
#define BATTLE_KERNEL_ENEMY_SKILL_FIRST 0x48

/*
 * An AI script queues an action: "use attack actionArg with command actionType on the current
 * target mask". A summon or enemy-skill attack id becomes the command's own index (id - 0x38 or
 * id - 0x48), an enemy's own attack its index in the scene's attack list. The target mask the
 * script left in lastActorMask (AI address 0x2070: byte 0xE of gBattleAiGlobals) goes both into
 * the combatant's postedActorMask and into the queued action.
 *
 * The packed union explicitly represents the overlapping descriptor fields.
 */
// FUNCTION: FF7 0x005d969c
void __cdecl Battle_AiVm_PostAction(short actorIndex, unsigned short actionType, short actionArg)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[24];
        short actorMask;
        struct { unsigned char pad_to_4[0x4]; int remapIndex; };
        struct { unsigned char pad_to_8[0x8]; char remapOffset0; };
        struct { unsigned char pad_to_9[0x9]; char remapOffset1; };
        struct { unsigned char pad_to_C[0xC]; char remapType0; };
        struct { unsigned char pad_to_D[0xD]; char remapType1; };
        struct { unsigned char pad_to_10[0x10]; char actionByte; };
        struct { unsigned char pad_to_12[0x12]; char actorByte; };
        struct { unsigned char pad_to_13[0x13]; char typeByte; };
        struct { unsigned char pad_to_14[0x14]; short remappedArg; };
        struct { unsigned char pad_to_16[0x16]; short descriptorMask; };
    } frame;
#pragma pack(pop)

    frame.remapType0 = BATTLE_COMMAND_SUMMON;
    frame.remapType1 = BATTLE_COMMAND_ENEMY_SKILL;
    frame.remapOffset0 = BATTLE_KERNEL_SUMMON_FIRST;
    frame.remapOffset1 = BATTLE_KERNEL_ENEMY_SKILL_FIRST;
    for (frame.remapIndex = 0; (unsigned int)frame.remapIndex < (unsigned int)2; frame.remapIndex++) {
        if ((short)actionType == *(unsigned char *)(((char *)&frame.remapType0 + frame.remapIndex * 1))) {
            actionArg -= (short)*(unsigned char *)(((char *)&frame.remapOffset0 + frame.remapIndex * 1));
        }
    }
    if ((short)actionType == BATTLE_COMMAND_ENEMY_ATTACK) {
        actionArg = Battle_FindSceneAttackIndex(actionArg);
    }
    frame.actorMask = gBattleAiGlobals.lastActorMask;
    g_aCombatants[actorIndex].postedActorMask = frame.actorMask;
    frame.actionByte = *(char *)&g_BattleAiVmActorIndex;
    frame.actorByte = actorIndex;
    frame.typeByte = actionType;
    frame.remappedArg = actionArg;
    frame.descriptorMask = frame.actorMask;
    Battle_EnqueueAction(&frame.actionByte);
}

/*
 * Battle_AiVm_GetAttackMpCost -- AI opcode 0x86; the MP cost field of an attack record, 0 if unknown
 */
// FUNCTION: FF7 0x005d975b
int __cdecl Battle_AiVm_GetAttackMpCost(int attackId)
{
    /* An int, not an unsigned short, as the original keeps it; the 16-bit MP cost field is widened on load. */
    int cost;
    BattleAttackEntry *attack;

    cost = 0;
    if (attackId < 0xffff) {
        attack = (BattleAttackEntry *)Battle_FindAttackRecord(attackId);
        if (attack != (BattleAttackEntry *)0) {
            cost = attack->mpCost;
        }
    }
    return cost;
}

/*
 * Battle_AiVm_AccessSavemapByte -- AI opcode 0x95; move one savemap byte to or from the AI global at bit address 0x2010
 */
// FUNCTION: FF7 0x005d9799
void __cdecl Battle_AiVm_AccessSavemapByte(int mode, int byteOffset)
{
    switch (mode) {
    case BATTLE_AI_SAVEMAP_READ:
        gBattleAiGlobals.savemapByte = gSavemap.scriptBanks[byteOffset];
        return;
    case BATTLE_AI_SAVEMAP_WRITE:
        gSavemap.scriptBanks[byteOffset] = gBattleAiGlobals.savemapByte;
        return;
    }
}

/*
 * An AI opcode that makes dstActor take over srcActor's condition: HP, MP, status, slip HP rate,
 * Reflect count, timing flags, status timers and stat percentages; then dstActor's ATB rates follow
 * the new status and its running status effect stops. The source keeps a link to its copy
 * (linkedActor, which target resolution follows) and becomes immune to Death (bit 0 of its status
 * immunity mask). Max HP, max MP and the stats themselves are not copied.
 */
// FUNCTION: FF7 0x005d97d5
void __cdecl Battle_AiVm_CopyCombatantState(int dstActor, int srcActor)
{
    int index;

    g_BattleState.actors[srcActor].linkedActor = dstActor;
    g_BattleState.actors[srcActor].statusImmunityMask |= 1;
    g_aCombatants[dstActor].curHp = g_aCombatants[srcActor].curHp;
    g_aCombatants[dstActor].curMp = (*(short *)&g_aCombatants[srcActor].curMp);
    g_aCombatants[dstActor].status = g_aCombatants[srcActor].status;
    g_aCombatants[dstActor].prevStatusFlags = g_aCombatants[srcActor].prevStatusFlags;
    g_BattleState.actors[dstActor].slipRate = g_BattleState.actors[srcActor].slipRate;
    g_BattleState.actors[dstActor].reflectState = g_BattleState.actors[srcActor].reflectState;
    g_BattleState.actors[dstActor].timingFlags = *(char *)&g_BattleState.actors[srcActor].timingFlags;
    for (index = 0; index < 0x10; index++) {
        (&g_BattleState.actors[dstActor].statusTimers[0])[index] = (&g_BattleState.actors[srcActor].statusTimers[0])[index];
    }
    for (index = 0; index < 8; index++) {
        g_BattleState.actors[dstActor].statPercent[index] = g_BattleState.actors[srcActor].statPercent[index];
    }
    Battle_RefreshActorTickRates(dstActor);
    Battle_StopActorStatusEffect(dstActor);
}

/*
 * Attack op 9: carries out the action against its targets, from the cast to the last bounce.
 *
 * First the gates: an action without a step (stepId -1) does nothing, one that already carries an
 * abort message shows it and ends, and a command up to 0x20 (the menu commands and an enemy's own
 * attack) must pass Battle_ChargeMpAndCheckCastable, which refuses it under Silence or Frog or
 * without the MP, and charges the MP. Support materia flags are applied.
 *
 * Then one round per repeat (repeatRemaining; a count of 0 runs none and leaves -1). A round may
 * first switch to the next queued follow-up attack (followUpAttackIds; bit 0x80 clears
 * BATTLE_SPECIAL_NO_FORCED_CRITICAL, so that hit is a critical one). It resolves the targets, drops
 * the enemies a melee attack cannot reach (Battle_FilterTargetsByEnemyLevel) and, with
 * BATTLE_ATTACK_FLAG_SKIP_KO_TARGETS, the KO'd ones; no target left ends the action.
 *
 * A round runs in passes. The first pass hits from the attacker; every target that bounces the
 * attack with Reflect becomes the source of a hit on its bounce target in the next pass, so passes
 * run until nothing bounces. In a pass every source's hits repeat countdown times (multi-hit
 * attacks), and each hit goes to one random target when the attack hits several times, when a
 * spell that does not target all by default is repeat-cast, or when the attack targets randomly;
 * a summon always hits all its targets. After each source's hits the AFTER_HITS added effect runs;
 * after each pass every actor's pooled HP and MP changes (drain, HP/MP Absorb) are shown.
 *
 * A pass with no bounce counts down the repeat casts (repeatCasts, from a support code; well
 * established: Quadra Magic's four casts). The next cast goes to the same targets, or for a summon
 * to all enemies, minus the KO'd ones; when none are left the casting stops.
 */
// FUNCTION: FF7 0x005d9940
void __cdecl Battle_AttackOpExecute(void)
{
    int idx;
    int queuedByte;
    int victim;
    int pendingMask;
    int actorIndex;
    int pickRandom;
    int actorStatus;
    int recastMask;

    g_BattleAttackOpStateFlags = 0;
    if (g_pBattleCommandCtx->stepId == -1) {
        return;
    }
    if (g_pBattleCommandCtx->messageId != -1) {
        Battle_AbortActionWithMessage(g_pBattleCommandCtx->messageId);
        return;
    }
    if (g_pBattleCommandCtx->stepFlags <= 0x20) {
        if (Battle_ChargeMpAndCheckCastable() != 0) {
            return;
        }
    }
    if (g_pBattleCommandCtx->supportBits != 0) {
        Battle_ApplySupportMateriaFlags(g_pBattleCommandCtx->supportBits);
    }
    g_pBattleCommandCtx->mask = g_pBattleCommandCtx->targetMask;
    g_pBattleCommandCtx->basePower = g_pBattleCommandCtx->power;
    while ((g_pBattleCommandCtx->repeatRemaining)-- != 0) {
        for (idx = 0; idx < 8; idx++) {
            if (g_pBattleCommandCtx->followUpAttackIds[idx] != 0xFF) {
                queuedByte = g_pBattleCommandCtx->followUpAttackIds[idx];
                g_pBattleCommandCtx->followUpAttackIds[idx] = 0xFF;
                g_pBattleCommandCtx->attackId = queuedByte & 0xFFFFFF7F;
                g_pBattleCommandCtx->stepPower = g_pBattleCommandCtx->attackId;
                g_pBattleCommandCtx->targetFlags = 0xFF;
                Battle_ReapplyAttackRecord();
                if ((queuedByte & 0x80) != 0) {
                    g_pBattleCommandCtx->specialFlags &= ~BATTLE_SPECIAL_NO_FORCED_CRITICAL;
                }
                break;
            }
        }
        g_BattleAttackReactedActorMask = 0;
        g_pBattleCommandCtx->targetMask = g_pBattleCommandCtx->mask;
        g_pBattleCommandCtx->savedCountdown = g_pBattleCommandCtx->countdown;
        Battle_ResolveTargetMask();
        Battle_FilterTargetsByEnemyLevel();
        if ((g_pBattleCommandCtx->attackFlags & BATTLE_ATTACK_FLAG_SKIP_KO_TARGETS) != 0) {
            for (idx = 0; idx < 0xA; idx++) {
                if ((g_pBattleCommandCtx->targetMask & 1 << idx) != 0 && (g_aCombatants[idx].status & BATTLE_STATUS_DEATH) != 0) {
                    g_pBattleCommandCtx->targetMask &= ~(1 << idx);
                }
            }
            if (g_pBattleCommandCtx->targetMask == 0) {
                Battle_QueueCommandEffectStep(g_pBattleCommandCtx->stepId);
                break;
            }
        }
        if (g_pBattleCommandCtx->targetMask == 0) {
            Battle_AbortActionWithMessage(g_pBattleCommandCtx->messageId);
            break;
        }
        for (idx = 0; idx < 0xA; idx++) {
            g_BattleActorNextTargetMask[idx] = 0;
            g_BattleActorReflectSource[idx] = 0xFF;
        }
        Battle_DecodeDamageCalcByte();
        g_BattleActorNextTargetMask[g_pBattleCommandCtx->attackerSlot] = *(short *)&g_pBattleCommandCtx->targetMask;
        g_BattleAttackOpStateFlags |= BATTLE_ATTACK_OP_RUNNING | BATTLE_ATTACK_OP_STARTED;
        while ((g_BattleAttackOpStateFlags & BATTLE_ATTACK_OP_RUNNING) != 0) {
            if ((g_BattleAttackOpStateFlags & (BATTLE_ATTACK_OP_REFLECTED | BATTLE_ATTACK_OP_REPEAT_CAST)) != 0) {
                g_BattleAttackOpStateFlags &= 0xFFFFFFFD; /* ~BATTLE_ATTACK_OP_REFLECTED (unsigned literal kept) */
                if (g_pBattleCommandCtx->stepFlags != BATTLE_COMMAND_SUMMON) {
                    g_pBattleCommandCtx->actionType = BATTLE_SEQ_STEP_CHAINED_ACTION;
                }
            }
            // This pass's sources and targets: last pass's bounces, or the attacker's targets.
            for (idx = 0; idx < 0xA; idx++) {
                g_BattleActorCurrentTargetMask[idx] = g_BattleActorNextTargetMask[idx];
                g_BattleActorNextTargetMask[idx] = 0;
            }
            for (idx = 0; idx < 0xA; idx++) {
                pendingMask = g_BattleActorCurrentTargetMask[idx];
                if (pendingMask != 0) {
                    g_pBattleCommandCtx->actorIndex = idx;
                    g_pBattleCommandCtx->hitCount = 0;
                    g_pBattleCommandCtx->countdown = g_pBattleCommandCtx->savedCountdown;
                    Battle_SnapshotSequenceCursors();
                    Battle_QueueAttackAnimStep(pendingMask);
                    while (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
                        pickRandom = 0;
                        g_pBattleCommandCtx->targetMask = pendingMask;
                        if (g_pBattleCommandCtx->countdown != 0) {
                            pickRandom = 1;
                        }
                        if (g_pBattleCommandCtx->repeatCasts != 0 && (g_pBattleCommandCtx->targetFlags & (BTL_TARGETF_DEFAULT_MULTIPLE | BTL_TARGETF_TOGGLE_MULTIPLE)) != BTL_TARGETF_DEFAULT_MULTIPLE) {
                            pickRandom = 1;
                        }
                        if ((g_pBattleCommandCtx->targetFlags & BTL_TARGETF_RANDOM) != 0) {
                            pickRandom = 1;
                        }
                        if (g_pBattleCommandCtx->stepFlags == BATTLE_COMMAND_SUMMON) {
                            pickRandom = 0;
                        }
                        if (pickRandom != 0) {
                            g_pBattleCommandCtx->targetMask = Battle_PickRandomBitFromMask(g_pBattleCommandCtx->targetMask);
                        }
                        for (victim = 0; victim < 0xA; victim++) {
                            if ((g_pBattleCommandCtx->targetMask & 1 << victim) != 0) {
                                Battle_ResolveHitOnTarget(idx, victim);
                            }
                        }
                        if (g_pBattleCommandCtx->countdown != 0) {
                            (g_pBattleCommandCtx->countdown)--;
                        }
                        if (g_pBattleCommandCtx->countdown == 0) {
                            break;
                        }
                    }
                    Battle_PushEmptyHitRecord();
                    Battle_RunSpecialAttackEffect(BATTLE_ADDED_FX_STAGE_AFTER_HITS);
                }
            }
            for (idx = 0; idx < 0xA; idx++) {
                if (g_BattleState.actors[idx].pendingHpAmount != 0) {
                    Battle_QueueActorHpMpFlushStep(idx, 0);
                }
                if (g_BattleState.actors[idx].pendingMpAmount != 0) {
                    Battle_QueueActorHpMpFlushStep(idx, 1);
                }
            }
            if ((g_BattleAttackOpStateFlags & BATTLE_ATTACK_OP_REFLECTED) == 0) {
                if (g_pBattleCommandCtx->repeatCasts != 0) {
                    (g_pBattleCommandCtx->repeatCasts)--;
                }
                if (g_pBattleCommandCtx->repeatCasts != 0) {
                    recastMask = g_pBattleCommandCtx->targetMask;
                    if (g_pBattleCommandCtx->stepFlags == BATTLE_COMMAND_SUMMON) {
                        recastMask = g_BattleScene.targetMaskBits & BATTLE_ACTOR_MASK_ENEMIES;
                    }
                    if (recastMask != 0) {
                        for (actorIndex = 0; actorIndex < 0xA; actorIndex++) {
                            actorStatus = g_aCombatants[actorIndex].status;
                            if ((actorStatus & BATTLE_STATUS_DEATH) != 0) {
                                recastMask &= ~(1 << actorIndex);
                            }
                        }
                        g_BattleActorNextTargetMask[g_pBattleCommandCtx->attackerSlot] = recastMask;
                        if (recastMask == 0) {
                            g_pBattleCommandCtx->repeatCasts = 0;
                        }
                    }
                }
                if (g_pBattleCommandCtx->repeatCasts != 0) {
                    g_BattleAttackOpStateFlags |= BATTLE_ATTACK_OP_REPEAT_CAST;
                } else {
                    g_BattleAttackOpStateFlags &= 0xFFFFFFFE; /* ~BATTLE_ATTACK_OP_RUNNING (unsigned literal kept) */
                }
            }
            if ((g_pBattleCommandCtx->attackFlags & BATTLE_ATTACK_FLAG_ALT_RETURN_POSE) != 0) {
                g_pBattleCommandCtx->animParam = 0xFFFFFFFC;
            } else {
                g_pBattleCommandCtx->animParam = 0xFFFFFFFF;
            }
            g_pBattleCommandCtx->animParamAlt = g_pBattleCommandCtx->animParam;
        }
        if (g_pBattleCommandCtx->stepId == 0x18) {
            g_pBattleCommandCtx->stepId = 0x19;
        } else if (g_pBattleCommandCtx->stepId == 0x19) {
            g_pBattleCommandCtx->stepId = 0x1A;
        } else if (g_pBattleCommandCtx->stepId == 0x1A) {
            g_pBattleCommandCtx->stepId = 0x1B;
        } else if (g_pBattleCommandCtx->stepId == 0x16) {
            g_pBattleCommandCtx->stepId = 0x17;
            g_pBattleCommandCtx->mask = g_pBattleCommandCtx->targetMask;
        }
    }
    if ((g_pBattleCommandCtx->attackFlags & BATTLE_ATTACK_FLAG_POST_EFFECT_STEP) != 0) {
        Battle_QueueCommandEffectStep(0x47);
    }
    if (g_pBattleCommandCtx->payload != -1) {
        Battle_PostBattleMessage(g_pBattleCommandCtx->payload);
    }
}

/*
 * Queues the animation step of one source's hits on targetMask (and counts the targets, which the
 * hit routines read as targetCount).
 *
 * An enemy turned into a Frog loses its own animations: its plain enemy attack plays animation
 * 0x14 and its other commands up to 0x20 play 0x1D (animation 0x34 is kept). A party member that
 * hits several targets, or reaches out-of-reach enemies (attack flag 0x20000, set by
 * Battle_FilterTargetsByEnemyLevel), plays the multi-target form of its animation
 * (g_BattleStatusAnimPoseTranslation). The step takes the multi-target camera parameter
 * (animParamAlt) for several targets, else animParam.
 */
// FUNCTION: FF7 0x005da0e7
void __cdecl Battle_QueueAttackAnimStep(unsigned int targetMask)
{
    int idx;
    BattleSeqStep *stepPtr;
    int param;

    stepPtr = (BattleSeqStep *)Battle_AllocSequenceStep();
    g_pBattleCommandCtx->targetCount = Battle_PopCountMask(targetMask);
    stepPtr->actorIndex = *(char *)&g_pBattleCommandCtx->attackerSlot;
    stepPtr->actionType = *(char *)&g_pBattleCommandCtx->actionType;
    stepPtr->animId = *(char *)&g_pBattleCommandCtx->stepId;
    if (g_pBattleCommandCtx->attackerSlot >= 4 && (g_pBattleCommandCtx->attackerStatus & BATTLE_STATUS_FROG) != 0 && stepPtr->animId != 0x34) {
        if (g_pBattleCommandCtx->stepFlags == BATTLE_COMMAND_ENEMY_ATTACK && g_pBattleCommandCtx->resolvedAttackId == 0xFF) {
            stepPtr->animId = 0x14;
        } else if (g_pBattleCommandCtx->stepFlags <= 0x20) {
            stepPtr->animId = 0x1D;
        }
    }
    if (g_pBattleCommandCtx->attackerSlot < 3 && (g_pBattleCommandCtx->targetCount > 1 || (g_pBattleCommandCtx->attackFlags & 0x20000) != 0)) {
        for (idx = 0; idx < 7; idx++) {
            if (stepPtr->animId == g_BattleStatusAnimPoseTranslation[idx].fromAnimId) {
                stepPtr->animId = g_BattleStatusAnimPoseTranslation[idx].toAnimId;
                break;
            }
        }
    }
    stepPtr->stepFlags = *(char *)&g_pBattleCommandCtx->stepFlags;
    stepPtr->effectId = *(char *)&g_pBattleCommandCtx->resolvedAttackId;
    if (g_pBattleCommandCtx->targetCount > 1) {
        param = g_pBattleCommandCtx->animParamAlt;
    } else {
        param = g_pBattleCommandCtx->animParam;
    }
    stepPtr->targetIndex = param;
    stepPtr->power = *(short *)&g_pBattleCommandCtx->stepPower;
    stepPtr->clearedFlag = 0;
}

/*
 * The elements of a melee attack: Cut, Hit and Punch (element bits 10 to 12 in the kernel's
 * element order, well established).
 */
#define BATTLE_ELEMENTS_CUT_HIT_PUNCH 0x1C00

/*
 * Battle message 0x77, which the code shows when no target is left in reach ("Can't reach!!" in
 * the kernel's battle text).
 */
#define BATTLE_MSG_CANT_REACH 0x77

/*
 * Keeps a party member's melee attack and Steal off the enemies it cannot reach: those whose row
 * byte is 0x10 or more. It applies to an attack with the Cut, Hit or Punch element, or to the Steal
 * command (5), unless the attack flags have 0x10. A member with party stat flag 4 is not held back:
 * the attack keeps its targets and gets attack flag 0x20000 instead (Battle_QueueAttackAnimStep then
 * plays the multi-target animation). Otherwise the far enemies are dropped, and with none left the
 * action ends on message 0x77.
 *
 * The name says "level", but the test reads the enemy's row byte.
 */
// FUNCTION: FF7 0x005da278
void __cdecl Battle_FilterTargetsByEnemyLevel(void)
{
    int i;
    int mask;

    if ((g_pBattleCommandCtx->attackFlags & 0x10) != 0) {
        return;
    }
    if (g_pBattleCommandCtx->attackerSlot < 3 && ((g_pBattleCommandCtx->elementMask & BATTLE_ELEMENTS_CUT_HIT_PUNCH) != 0 || g_pBattleCommandCtx->stepFlags == BATTLE_COMMAND_STEAL)) {
        mask = g_pBattleCommandCtx->targetMask;
        for (i = 4; i < 0xA; i++) {
            if (g_aCombatants[i].row >= 0x10) {
                mask &= ~(1 << i);
            }
        }
        if (mask != g_pBattleCommandCtx->targetMask) {
            if ((g_MenuPartyStats[g_pBattleCommandCtx->attackerSlot].flags & 4) != 0) {
                g_pBattleCommandCtx->attackFlags |= 0x20000;
            } else {
                g_pBattleCommandCtx->targetMask = mask;
                if (mask == 0) {
                    g_pBattleCommandCtx->messageId = BATTLE_MSG_CANT_REACH;
                }
            }
        }
    }
}

/*
 * Shows an actor's pooled HP or MP change (flushMode 0 HP, 1 MP) as a step of its own with
 * animation 0x2E, the number appearing over the actor; a KO'd actor gets none and its pool stays.
 */
// FUNCTION: FF7 0x005da380
void __cdecl Battle_QueueActorHpMpFlushStep(int actorIndex, int flushMode)
{
    BattleSeqStep *step;

    if ((g_aCombatants[actorIndex].status & BATTLE_STATUS_DEATH) == 0) {
        step = (BattleSeqStep *)Battle_AllocSequenceStep();
        step->actorIndex = actorIndex;
        step->actionType = BATTLE_SEQ_STEP_ACTION;
        step->animId = 0x2E;
        step->stepFlags = 0;
        step->effectId = 0;
        step->targetIndex = 0xFFFF;
        step->power = 0;
        step->clearedFlag = 0;
        Battle_FlushHpMpTransferPool(actorIndex, flushMode);
        Battle_PushEmptyHitRecord();
    }
}

/*
 * Applies an actor's pooled HP or MP change (poolSelect 0 HP, nonzero MP): the drain and HP/MP
 * Absorb amounts the pass collected for it. A negative pool heals. A target that takes no damage
 * (immune to the attack's kind, Petrify, Peerless) gets 0, even for a heal. If the change KOs the
 * actor it joins hpMpPoolMask and the hit record says so (BATTLE_HIT_RECORD_TARGET_KO). The
 * number is shown with the actor's new HP and MP.
 */
// FUNCTION: FF7 0x005da3f5
void __cdecl Battle_FlushHpMpTransferPool(int actorIndex, int poolSelect)
{
    BattleHitRecord *recordPtr;
    int amount;
    int bits;

    bits = 1;
    if (poolSelect != 0) {
        amount = g_BattleState.actors[actorIndex].pendingMpAmount;
        g_BattleState.actors[actorIndex].pendingMpAmount = 0;
    } else {
        amount = g_BattleState.actors[actorIndex].pendingHpAmount;
        g_BattleState.actors[actorIndex].pendingHpAmount = 0;
    }
    g_pBattleCommandCtx->targetSlot = actorIndex;
    g_pBattleCommandCtx->displayFlags = poolSelect != 0 ? BATTLE_DISPLAY_MP : 0;
    if (amount < 0) {
        amount = -amount;
        g_pBattleCommandCtx->displayFlags |= BATTLE_DISPLAY_HEAL;
    }
    if (Battle_TargetTakesNoDamage(actorIndex) != 0) {
        amount = 0;
    }
    g_pBattleCommandCtx->damage = amount;
    Battle_CommitHpMpChange();
    if ((g_aCombatants[actorIndex].status & BATTLE_STATUS_DEATH) != 0) {
        bits |= BATTLE_HIT_RECORD_TARGET_KO;
        g_pBattleCommandCtx->hpMpPoolMask |= 1 << actorIndex;
    }
    recordPtr = (BattleHitRecord *)Battle_AllocHitRecord();
    recordPtr->targetActor = actorIndex;
    recordPtr->sourceActor = actorIndex;
    recordPtr->animId = 0x2E;
    recordPtr->flags = bits;
    recordPtr->status = g_aCombatants[actorIndex].status;
    Battle_PushDamageDisplay(recordPtr, *(short *)&g_pBattleCommandCtx->damage, *(short *)&g_pBattleCommandCtx->displayFlags, -1, -1);
}

/*
 * Queues the number that pops up over a hit's target: the damage, or a sentinel (-2 for an
 * elemental instant death, -3 for a full recovery, -1 from Battle_PushStatusOnlyDisplay for a hit
 * that only tried a status), its display flags (heal, critical, MP), two display values, and the
 * target's HP and MP after the hit. Returns 0.
 */
// FUNCTION: FF7 0x005da562
int __cdecl Battle_PushDamageDisplay(BattleHitRecord *hitRecord, short displayParamA, short displayParamB, short displayParamC, short displayParamD)
{
    int actorIndex;
    BattleDamageDisplayRecord *displayRecord;

    actorIndex = hitRecord->targetActor;
    displayRecord = (BattleDamageDisplayRecord *)Battle_AllocDamageDisplayRecord(hitRecord);
    displayRecord->damage = displayParamA;
    displayRecord->displayFlags = displayParamB;
    displayRecord->displayValue = displayParamC;
    displayRecord->secondaryValue = displayParamD;
    displayRecord->hp = (*(short *)&g_aCombatants[actorIndex].curHp);
    displayRecord->mp = (*(short *)&g_aCombatants[actorIndex].curMp);
    return 0;
}

/*
 * The damage cap on an enemy target: 9999 HP, 999 MP. A party target uses its own caps
 * (BattlePartyActor hpCap/mpCap).
 */
#define BATTLE_ENEMY_HP_DAMAGE_CAP 0x270F
#define BATTLE_ENEMY_MP_DAMAGE_CAP 0x3E7

/* resultFlags bit 23: the hit left the target's status as it was (missed, or nothing to change). */
#define BATTLE_RESULT_STATUS_UNCHANGED 0x800000

/*
 * One hit of the action on one target, from the roll to the number over its head.
 *
 * 1. Cover: an ally with Cover may step in (Battle_ApplyCoverRedirect); the hit then goes to the
 *    coverer and plays the cover pose. Battle_BeginTargetHit loads the target and rolls the status
 *    chance.
 * 2. Elements: the target's affinities decide the reaction (Battle_ResolveElementAffinity); an
 *    absorbed revive becomes an instant death and an absorbed Death a full recovery
 *    (Battle_ApplyElementalPreDamage). Restorative attacks display as heals, attacks that are not
 *    HP attacks as MP. A target that is always missed is missed.
 * 3. Damage: unless missed, the damage program runs (hit roll and formula); the AFTER_DAMAGE added
 *    effect follows. A target immune to the attack's kind (physical or magical) nullifies it. Weak
 *    doubles, half halves, absorb heals (Battle_ApplyElementalPostDamage).
 * 4. Reflect and revive: an attack that can be reflected bounces off a target with Reflect. A
 *    normal Reflect wears off on its fourth bounce; one the target cannot lose (Reflect in its
 *    status immunity mask) bounces once a round, so two such targets cannot bounce a spell
 *    between them forever. A KO-only attack (the revives) misses a living target unless it bounces
 *    or an element turns it into an instant death, an absorb or a full recovery.
 * 5. The landed hit counts, runs the TARGET_HIT added effect and may steal (Steal as well).
 * 6. Caps: damage over the cap is cut (enemy 9999 HP / 999 MP, party its own caps); a target immune
 *    to the kind, petrified or Peerless takes 0; while the attacker has exactly 7777 HP every
 *    nonzero hit does 7777 (All Lucky 7s, well established).
 * 7. Status: Haste and Slow cancel each other, as do Sadness and Fury; then
 *    status = ((status | inflict) & ~cure) ^ toggle, each mask without the target's immunities.
 * 8. HP and MP change (Battle_CommitHpMpChange), the number is queued, then drains and HP/MP Absorb
 *    (a tenth of the damage back as HP, a hundredth as MP; well established) go to the pools. A
 *    party member hit by an enemy's enemy skill may learn it.
 */
// FUNCTION: FF7 0x005da5da
void __cdecl Battle_ResolveHitOnTarget(int sourceActor, int targetActor)
{
    int partyHpMax;
    int opposingMask;
    int reflectSource;
    int returned;
    int mpCap;
    int clearMask;
    int rowIndex;
    int status;
    int workCanReflect;
    int savedState;
    int sideIndex;
    int damageCap;
    BattleHitRecord *record;

    record = (BattleHitRecord *)Battle_AllocHitRecord();
    record->targetActor = targetActor;
    record->sourceActor = sourceActor;
    record->flags = 0;
    g_aCombatants[targetActor].coverSlot = 0xFF;
    Battle_ApplyCoverRedirect(record);
    Battle_BeginTargetHit(record->targetActor);
    if (record->targetActor != targetActor) {
        Battle_ClearSequenceStepTargetWord();
        g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_COVERED;
    }
    targetActor = g_pBattleCommandCtx->targetSlot;
    Battle_ResolveElementAffinity();
    Battle_ApplyElementalPreDamage();
    if ((g_pBattleCommandCtx->elementMask & BATTLE_ELEMENT_RESTORATIVE) != 0) {
        g_pBattleCommandCtx->displayFlags |= BATTLE_DISPLAY_HEAL;
    }
    if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_MP_DAMAGE) == 0) {
        g_pBattleCommandCtx->displayFlags |= BATTLE_DISPLAY_MP;
    }
    if ((g_aCombatants[targetActor].flags & BATTLE_COMBATANT_ALWAYS_MISSED) != 0) {
        g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_MISS;
    }
    if ((g_pBattleCommandCtx->resultFlags & BATTLE_RESULT_MISS) == 0) {
        Battle_RunDamageProgram();
    }
    Battle_RunSpecialAttackEffect(BATTLE_ADDED_FX_STAGE_AFTER_DAMAGE);
    if (g_pBattleCommandCtx->power == 0) {
        g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_NO_DAMAGE_DISPLAY;
    }
    // Immune to physical (or magical) attacks: the whole reaction becomes a nullify.
    if (Battle_IsTargetImmuneToAttackKind(targetActor) != 0) {
        g_pBattleCommandCtx->elemReaction = BATTLE_ELEM_REACTION_NULLIFY;
    }
    Battle_ApplyElementalPostDamage();
    if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_REFLECTABLE) == 0 && (g_BattleAttackReactedActorMask & 1 << targetActor) == 0 && (g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_REFLECT) != 0) {
        workCanReflect = 1;
    } else {
        workCanReflect = 0;
    }
    returned = workCanReflect;
    // A KO-only attack (a revive) on a living target misses.
    if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_KO_ONLY) == 0 && returned == 0 && (g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_DEATH) == 0 && (g_pBattleCommandCtx->elemReaction & (BATTLE_ELEM_REACTION_DEATH | BATTLE_ELEM_REACTION_ABSORB | BATTLE_ELEM_REACTION_FULL_RECOVERY)) == 0) {
        g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_MISS;
    }
    if ((g_pBattleCommandCtx->resultFlags & BATTLE_RESULT_MISS) == 0) {
        (g_pBattleCommandCtx->hitCount)++;
        record->flags |= BATTLE_HIT_RECORD_HIT;
        Battle_RunSpecialAttackEffect(BATTLE_ADDED_FX_STAGE_TARGET_HIT);
        if (g_pBattleCommandCtx->attackerSlot != targetActor) {
            g_pBattleCommandCtx->hitMask |= 1 << targetActor;
        }
        // Steal as well: resultFlags starts as a copy of the attack flags (BATTLE_ATTACK_FLAG_STEAL_AS_WELL).
        if ((g_pBattleCommandCtx->resultFlags & 4) != 0 && g_pBattleCommandCtx->countdown <= 8) {
            Battle_TryStealOnHit();
        }
        if (returned != 0) {
            // The bounce. Battle_AreActorsOnSameSide is nonzero for actors on OPPOSITE sides: a
            // spell cast at a foe flies back at its caster, one cast at a reflecting ally goes to a
            // random foe of that ally, the same one for every bounce off it this round.
            if (Battle_AreActorsOnSameSide(sourceActor, targetActor) != 0) {
                reflectSource = sourceActor;
            } else {
                if (g_BattleActorReflectSource[targetActor] == -1) {
                    /* BUG: with no foe of the reflector left in g_BattleScene.targetMaskBits the
                       pick returns 0 and Battle_HighestSetBitIndex(0) is 0, so the spell bounces
                       onto slot 0, the first party member, whichever side the reflector is on. */
                    opposingMask = Battle_PickRandomOpposingActorMask(targetActor);
                    g_BattleActorReflectSource[targetActor] = Battle_HighestSetBitIndex(opposingMask);
                }
                reflectSource = g_BattleActorReflectSource[targetActor];
            }
            g_BattleAttackOpStateFlags |= BATTLE_ATTACK_OP_REFLECTED;
            g_BattleActorNextTargetMask[targetActor] |= (short)(1 << reflectSource);
            Battle_ClearHitScratch();
            /* hitTargetPtr is the target's BattleActorRuntime: flags is its statusImmunityMask,
               hitCounter its reflectState (3 when Reflect was cast, Battle_OnStatusReflectSet). */
            if ((g_pBattleCommandCtx->hitTargetPtr->flags & BATTLE_STATUS_REFLECT) != 0) {
                // Reflect it cannot lose: not used up, but no second bounce off it this round.
                g_BattleAttackReactedActorMask |= (short)(1 << targetActor);
            } else if (g_pBattleCommandCtx->hitTargetPtr->hitCounter != 0) {
                (*(char *)&g_pBattleCommandCtx->hitTargetPtr->hitCounter)--;
            } else {
                // The fourth bounce wears Reflect off.
                g_pBattleCommandCtx->workCureMask |= BATTLE_STATUS_REFLECT;
            }
            g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_NO_DAMAGE_DISPLAY;
            record->flags |= BATTLE_HIT_RECORD_REFLECTED;
            if (targetActor < 3) {
                g_pBattleCommandCtx->animId = 0xA;
            }
        }
        if ((g_pBattleCommandCtx->resultFlags & BATTLE_RESULT_BARRIER) != 0) {
            record->flags |= BATTLE_HIT_RECORD_BARRIER;
        }
        if ((g_pBattleCommandCtx->resultFlags & BATTLE_RESULT_MBARRIER) != 0) {
            record->flags |= BATTLE_HIT_RECORD_MBARRIER;
        }
    } else {
        Battle_ClearHitScratch();
    }
    // The damage cap.
    if (targetActor < 3) {
        partyHpMax = (*(unsigned short *)&g_BattleState.partyStats[targetActor].hpCap);
        mpCap = (*(unsigned short *)&g_BattleState.partyStats[targetActor].mpCap);
    } else {
        partyHpMax = BATTLE_ENEMY_HP_DAMAGE_CAP;
        mpCap = BATTLE_ENEMY_MP_DAMAGE_CAP;
    }
    if ((g_pBattleCommandCtx->displayFlags & BATTLE_DISPLAY_MP) != 0) {
        damageCap = mpCap;
    } else {
        damageCap = partyHpMax;
    }
    if (g_pBattleCommandCtx->damage > damageCap) {
        g_pBattleCommandCtx->damage = damageCap;
    }
    if (Battle_TargetTakesNoDamage(targetActor) != 0) {
        g_pBattleCommandCtx->damage = 0;
    }
    // All Lucky 7s.
    if (g_pBattleCommandCtx->damage != 0 && g_BattleState.actors[g_pBattleCommandCtx->attackerSlot].hp == BATTLE_LUCKY_7S_HP) {
        g_pBattleCommandCtx->damage = BATTLE_LUCKY_7S_HP;
    }
    if ((g_pBattleCommandCtx->resultFlags & (BATTLE_RESULT_MISS | BATTLE_RESULT_NO_DAMAGE_DISPLAY)) == 0) {
        if ((g_pBattleCommandCtx->displayFlags & BATTLE_DISPLAY_HEAL) == 0 && g_pBattleCommandCtx->attackerSlot != targetActor) {
            g_pBattleCommandCtx->hitTargetBits |= 1 << targetActor;
        }
        if (g_pBattleCommandCtx->hitDamage == -1) {
            g_pBattleCommandCtx->hitDamage = g_pBattleCommandCtx->damage;
        }
        g_pBattleCommandCtx->hitSecondaryValue = g_pBattleCommandCtx->secondaryDisplayValue;
        if ((g_pBattleCommandCtx->displayFlags & BATTLE_DISPLAY_CRITICAL) != 0) {
            g_pBattleCommandCtx->hitDisplayValue = g_pBattleCommandCtx->displayValueB;
        } else {
            g_pBattleCommandCtx->hitDisplayValue = g_pBattleCommandCtx->displayValueA;
        }
        if ((g_pBattleCommandCtx->displayFlags & BATTLE_DISPLAY_HEAL) != 0 || g_pBattleCommandCtx->hitDamage == 0) {
            g_pBattleCommandCtx->animId = 0x33;
        } else {
            Battle_SelectHitReactionPose(0);
        }
    } else if ((g_pBattleCommandCtx->resultFlags & BATTLE_RESULT_MISS) != 0) {
        g_pBattleCommandCtx->hitDisplayValue = g_pBattleCommandCtx->displayValueC;
    } else {
        g_pBattleCommandCtx->hitDisplayValue = g_pBattleCommandCtx->displayValueA;
        if ((g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_DEATH) != 0) {
            Battle_SelectHitReactionPose(0);
        }
    }
    // The status change, on a hit only.
    if ((g_pBattleCommandCtx->resultFlags & BATTLE_RESULT_MISS) == 0) {
        clearMask = ~g_pBattleCommandCtx->immunityMask;
        status = g_pBattleCommandCtx->targetStatus;
        savedState = status;
        for (rowIndex = 0; rowIndex < 2; rowIndex++) {
            for (sideIndex = 0; sideIndex < 2; sideIndex++) {
                if ((g_pBattleCommandCtx->workInflictMask & g_BattleOpposedStatusPairs[rowIndex][sideIndex]) != 0) {
                    // Haste on a slowed target cures Slow and hastes it; Sadness on a furious
                    // target (or Fury on a sad one) only cures the other.
                    if (rowIndex == 1 && (status & g_BattleOpposedStatusPairs[rowIndex][sideIndex ^ 1]) != 0) {
                        g_pBattleCommandCtx->workInflictMask &= ~g_BattleOpposedStatusPairs[rowIndex][sideIndex];
                    }
                    g_pBattleCommandCtx->workCureMask |= g_BattleOpposedStatusPairs[rowIndex][sideIndex ^ 1];
                }
            }
        }
        // An elemental instant death (hitDamage -2) kills even a Death-immune target.
        if (g_pBattleCommandCtx->hitDamage == -2) {
            clearMask |= BATTLE_STATUS_DEATH;
        }
        status |= g_pBattleCommandCtx->workInflictMask & clearMask;
        status &= ~(g_pBattleCommandCtx->workCureMask & clearMask);
        status ^= g_pBattleCommandCtx->workToggleMask & clearMask;
        g_pBattleCommandCtx->targetStatus = status;
        g_aCombatants[targetActor].status = status;
        if (savedState != status) {
            if ((status & (g_pBattleCommandCtx->workStatusUnion)) != 0 && g_pBattleCommandCtx->attackerSlot != targetActor) {
                g_pBattleCommandCtx->hitTargetBits |= 1 << targetActor;
            }
            if (((savedState ^ status) & BATTLE_STATUS_DEATH) != 0) {
                Battle_SelectHitReactionPose(savedState & BATTLE_STATUS_DEATH);
            } else {
                record->flags |= BATTLE_HIT_RECORD_STATUS_CHANGED;
            }
        } else {
            g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_STATUS_UNCHANGED;
        }
    } else {
        g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_STATUS_UNCHANGED;
    }
    Battle_CommitHpMpChange();
    if ((g_pBattleCommandCtx->resultFlags & 0x40001) == 0x40001) {
        g_pBattleCommandCtx->resultFlags &= ~BATTLE_RESULT_NO_DAMAGE_DISPLAY;
    }
    if ((g_pBattleCommandCtx->resultFlags & BATTLE_RESULT_NO_DAMAGE_DISPLAY) == 0) {
        Battle_PushDamageDisplay(record, *(short *)&g_pBattleCommandCtx->hitDamage, *(short *)&g_pBattleCommandCtx->displayFlags, *(short *)&g_pBattleCommandCtx->hitDisplayValue, *(short *)&g_pBattleCommandCtx->hitSecondaryValue);
    } else if ((g_pBattleCommandCtx->resultFlags & BATTLE_RESULT_STATUS_UNCHANGED) != 0) {
        Battle_PushStatusOnlyDisplay(record);
    }
    if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_DRAIN) == 0) {
        Battle_QueueHitHpMpTransfer();
    }
    // MP Absorb returns a hundredth of the damage as MP, HP Absorb a tenth as HP.
    if ((g_pBattleCommandCtx->attackFlags & BATTLE_ATTACK_FLAG_MP_ABSORB) != 0) {
        Battle_AddHpMpTransferToPool(g_pBattleCommandCtx->actorIndex, g_pBattleCommandCtx->targetSlot, g_pBattleCommandCtx->damage / 0x64, 1);
    }
    if ((g_pBattleCommandCtx->attackFlags & BATTLE_ATTACK_FLAG_HP_ABSORB) != 0) {
        Battle_AddHpMpTransferToPool(g_pBattleCommandCtx->actorIndex, g_pBattleCommandCtx->targetSlot, g_pBattleCommandCtx->damage / 0xA, 2);
    }
    // A party member hit by an enemy's Enemy Skill attack.
    if (targetActor < 3 && g_pBattleCommandCtx->attackerSlot >= 4 && ((BattleCommandTargetState *)(g_pBattleCommandCtx->targetStatePtr))->learnMask != 0 && g_pBattleCommandCtx->stepFlags == BATTLE_COMMAND_ENEMY_SKILL) {
        Battle_LearnEnemySkill();
    }
    record->status = g_aCombatants[targetActor].status;
    record->animId = *(char *)&g_pBattleCommandCtx->animId;
    // The coverer plays the cover pose.
    if ((g_pBattleCommandCtx->resultFlags & BATTLE_RESULT_COVERED) != 0) {
        record->animId = 9;
    }
    if ((g_aCombatants[targetActor].status & BATTLE_STATUS_DEATH) != 0) {
        record->flags |= BATTLE_HIT_RECORD_TARGET_KO;
        *(unsigned short *)&record->flags &= 0xFFF7;
        g_pBattleCommandCtx->hpMpPoolMask |= 1 << targetActor;
        // A target the Flash command KOs.
        if (g_pBattleCommandCtx->stepFlags == BATTLE_COMMAND_FLASH) {
            if (g_BattleScene.actors[targetActor].state < 0x11) {
                g_BattleScene.actors[targetActor].state = 8;
            }
            Battle_PushDamageDisplay(record, -2, 0, *(short *)&g_pBattleCommandCtx->hitDisplayValue, *(short *)&g_pBattleCommandCtx->secondaryDisplayValue);
        }
    }
}

/*
 * Battle_AreActorsOnSameSide -- parity of "how many of the two slots are on the party side".
 * Slots 0..3 are the party, 4..9 the enemies, so the result is 0 when both actors are on the
 * same side and 1 when they are on opposite sides.
 */
// FUNCTION: FF7 0x005db166
int __cdecl Battle_AreActorsOnSameSide(int slotA, int slotB)
{
    int count;

    count = 0;
    if (slotA < 4) {
        count++;
    }
    if (slotB < 4) {
        count++;
    }
    return count & 1;
}

/*
 * Picks the target of a spell bounced off a reflecting ally: one random actor of the other side
 * still in g_BattleScene.targetMaskBits (an enemy for slots 0..3, a party member for slots 4..9),
 * returned as its bit. A negative actor index counts as the party side.
 */
// FUNCTION: FF7 0x005db199
int __cdecl Battle_PickRandomOpposingActorMask(int actorIndex)
{
    int mask;

    /* VC5 emits `set<inverse of the written condition>` and adds the ELSE
       arm's constant. The target has `setge al ... add eax, 0xf`, so the
       written test is `< 4` and 0xF is its else arm -- writing it the other
       way round gives `setl` and `add eax, 0x3f0`. */
    mask = (actorIndex < 4) ? BATTLE_ACTOR_MASK_ENEMIES : BATTLE_ACTOR_MASK_ALLIES;
    mask = Battle_PickRandomTargetFromMask((unsigned short)(g_BattleScene.targetMaskBits & mask));
    return mask;
}

/*
 * The statuses that keep a party member from covering: Death, Sleep, Confusion, Stop, Frog, Small,
 * Petrify, Berserk, Paralysis, Darkness and the capture bit (31).
 */
#define BATTLE_COVER_BLOCKING_STATUS 0x86805C45

/*
 * Cover (well established: the Cover materia's chance to take a hit meant for an ally).
 *
 * Only a damaging, single-target, physical attack on a party member, by someone else, can be
 * covered; not one whose status change includes Frog, and of an enemy's own attacks only the one
 * without a scene attack id (resolvedAttackId 0xFF). Every other party member in the target's group
 * (g_BattleScene.targetMaskTable) who has a cover chance, is not the attacker, has no blocking
 * status and no timing flag 8 rolls it out of 100; one random winner (within
 * gBattleAiGlobals.targetMask) takes the hit: the record's target becomes the coverer, its source
 * the covered member, and the coverer's coverSlot remembers whom it covers.
 *
 * A target in none of the three groups is treated as in the last group tested (index 2).
 */
// FUNCTION: FF7 0x005db1d6
void __cdecl Battle_ApplyCoverRedirect(BattleHitRecord *hitRecord)
{
    int group;
    int groupMask;
    int targetActor;
    int stateBits;
    int coverChance;
    int partyPos;
    int coverCandidates;

    targetActor = hitRecord->targetActor;
    groupMask = 0;
    for (group = 0; group < 3; group++) {
        groupMask = g_BattleScene.targetMaskTable[group];
        if ((groupMask & 1 << targetActor) != 0) {
            break;
        }
    }
    if (targetActor < 3 && g_pBattleCommandCtx->power != 0 && targetActor != g_pBattleCommandCtx->attackerSlot && g_pBattleCommandCtx->targetCount == 1) {
        stateBits = g_pBattleCommandCtx->statusMaskA | g_pBattleCommandCtx->statusMaskB | g_pBattleCommandCtx->statusMaskC;
        if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_PHYSICAL) == 0 && ((stateBits & BATTLE_STATUS_FROG) == 0 && (g_pBattleCommandCtx->stepFlags != BATTLE_COMMAND_ENEMY_ATTACK || g_pBattleCommandCtx->resolvedAttackId == 0xFF))) {
            coverCandidates = 0;
            for (partyPos = 0; partyPos < 3; partyPos++) {
                if ((g_BattleState.actors[partyPos].timingFlags & 8) == 0 && (groupMask & 1 << partyPos) != 0) {
                    coverChance = g_MenuPartyStats[partyPos].coverChance;
                    if (coverChance != 0 && g_pBattleCommandCtx->attackerSlot != partyPos && targetActor != partyPos && (g_aCombatants[partyPos].status & BATTLE_COVER_BLOCKING_STATUS) == 0) {
                        /* Retail caller treats `Battle_RandBelow` return (`int`) as `unsigned char` for caller zero/sign extension. */
                        if (coverChance > ((unsigned char (__cdecl *)(int limit))Battle_RandBelow)(0x64)) {
                            coverCandidates |= 1 << partyPos;
                        }
                    }
                }
            }
            coverCandidates = Battle_PickRandomBitFromMask(coverCandidates & gBattleAiGlobals.targetMask);
            if (coverCandidates != 0) {
                hitRecord->targetActor = Battle_HighestSetBitIndex(coverCandidates);
                hitRecord->sourceActor = targetActor;
                g_aCombatants[hitRecord->targetActor].coverSlot = g_aCombatants[targetActor].sideSlot;
            }
        }
    }
}

/*
 * The elemental reaction before the damage formula runs.
 *
 * Absorb turns the attack's status change around: what it would inflict it cures and the reverse.
 * A revive (cure Death) on a target that absorbs its element so becomes an instant death, and a
 * Death on such a target a full recovery; either replaces the whole reaction, the full recovery
 * winning if both apply. So a Restorative revive kills a target that absorbs Restorative (and
 * Battle_ApplyElementalPostDamage makes its Cure hurt it).
 *
 * With a percent-of-HP formula (3 or 4) the instant death must also beat a roll: it lands when a
 * random 0..31 is below the attack's power, i.e. with power/32 odds, and is nullified otherwise.
 *
 * For an attack without power (a status-only spell) Weak doubles and Half halves the accuracy
 * instead of the damage.
 */
// FUNCTION: FF7 0x005db3e4
void __cdecl Battle_ApplyElementalPreDamage(void)
{
    int tmp;
    int n;

    if ((g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_ABSORB) != 0) {
        tmp = g_pBattleCommandCtx->workCureMask;
        g_pBattleCommandCtx->workCureMask = g_pBattleCommandCtx->workInflictMask;
        g_pBattleCommandCtx->workInflictMask = tmp;
        if ((g_pBattleCommandCtx->workInflictMask & BATTLE_STATUS_DEATH) != 0) {
            g_pBattleCommandCtx->workInflictMask &= ~BATTLE_STATUS_DEATH;
            g_pBattleCommandCtx->elemReaction = BATTLE_ELEM_REACTION_DEATH;
        }
        if ((g_pBattleCommandCtx->workCureMask & BATTLE_STATUS_DEATH) != 0) {
            g_pBattleCommandCtx->workCureMask &= ~BATTLE_STATUS_DEATH;
            g_pBattleCommandCtx->elemReaction = BATTLE_ELEM_REACTION_FULL_RECOVERY;
        }
    }
    if ((g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_DEATH) != 0) {
        n = g_pBattleCommandCtx->damageFormulaIndex;
        if (n >= 3) {
            if (n > 4) { /* empty first arm: lib/codegen.h */
            } else if (((unsigned char (__cdecl *)(int limit))Battle_RandBelow)(0x20) >= g_pBattleCommandCtx->power) {
                g_pBattleCommandCtx->elemReaction &= ~BATTLE_ELEM_REACTION_DEATH;
                g_pBattleCommandCtx->elemReaction |= BATTLE_ELEM_REACTION_NULLIFY;
            }
        }
    }
    if (g_pBattleCommandCtx->power == 0) {
        if ((g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_WEAK) != 0) {
            g_pBattleCommandCtx->accuracyWork <<= 1;
        }
        if ((g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_HALF) != 0) {
            g_pBattleCommandCtx->accuracyWork >>= 1;
        }
    }
}

/* Element bit 3, Earth, in the kernel's element order (well established). */
#define BATTLE_ELEMENT_EARTH 8

/*
 * The elemental reaction after the damage formula.
 *
 * Absorb turns the hit into a heal (or a heal into damage: Cure hurts a target that absorbs
 * Restorative). Otherwise Weak doubles the damage and Half halves it, rounding up.
 *
 * Then the strongest outcome, in this order: an instant death KOs a living target (the number
 * shows sentinel -2) and misses a KO'd one; a full recovery refills HP and MP (sentinel -3); a
 * nullify wipes the hit's damage and status change, and a status attack or an Earth attack is
 * shown as a miss rather than a 0.
 *
 * The damage is not capped here; Battle_ResolveHitOnTarget caps it.
 */
// FUNCTION: FF7 0x005db593
void __cdecl Battle_ApplyElementalPostDamage(void)
{
    if ((g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_ABSORB) != 0) {
        g_pBattleCommandCtx->displayFlags ^= BATTLE_DISPLAY_HEAL;
    } else {
        if ((g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_WEAK) != 0) {
            g_pBattleCommandCtx->damage <<= 1;
        }
        if ((g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_HALF) != 0) {
            (g_pBattleCommandCtx->damage)++;
            g_pBattleCommandCtx->damage >>= 1;
        }
    }
    if ((g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_DEATH) != 0) {
        if ((g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_DEATH) != 0) {
            g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_MISS | BATTLE_RESULT_NO_DAMAGE_DISPLAY;
            Battle_ClearHitScratch();
        } else {
            g_pBattleCommandCtx->workInflictMask |= BATTLE_STATUS_DEATH;
            g_pBattleCommandCtx->workCureMask &= ~BATTLE_STATUS_DEATH;
            g_pBattleCommandCtx->resultFlags &= ~BATTLE_RESULT_NO_DAMAGE_DISPLAY;
            g_pBattleCommandCtx->displayFlags &= ~BATTLE_DISPLAY_HEAL;
            g_pBattleCommandCtx->hitDamage = 0xFFFFFFFE;
        }
    } else if ((g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_FULL_RECOVERY) != 0) {
        g_aCombatants[g_pBattleCommandCtx->targetSlot].curHp = g_aCombatants[g_pBattleCommandCtx->targetSlot].maxHp;
        g_aCombatants[g_pBattleCommandCtx->targetSlot].curMp = (*(short *)&g_aCombatants[g_pBattleCommandCtx->targetSlot].maxMp);
        g_pBattleCommandCtx->displayFlags = BATTLE_DISPLAY_HEAL;
        g_pBattleCommandCtx->resultFlags &= ~BATTLE_RESULT_NO_DAMAGE_DISPLAY;
        g_pBattleCommandCtx->workInflictMask &= ~BATTLE_STATUS_DEATH;
        g_pBattleCommandCtx->hitDamage = 0xFFFFFFFD;
    } else if ((g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_NULLIFY) != 0) {
        if (g_pBattleCommandCtx->workStatusUnion != 0 || (g_pBattleCommandCtx->elementMask & BATTLE_ELEMENT_EARTH) != 0) {
            g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_MISS;
        }
        Battle_ClearHitScratch();
    }
}

/*
 * Battle message 0x73, the notice that a member learned an enemy skill ("Learned Enemy-Skill" in
 * the kernel's battle text).
 */
#define BATTLE_MSG_LEARNED_ENEMY_SKILL 0x73

/*
 * The Enemy Skill materia learns the skill that just hit its wearer: bit (attack id - 0x48) of the
 * target's learnMask. A skill already known, or a battle with scene flag 0x40, teaches nothing.
 * Learning shows message 0x73 with the skill's name, rebuilds the member's ability lists (so the
 * skill can be used in this battle) and plays pose 0xA.
 */
// FUNCTION: FF7 0x005db812
void __cdecl Battle_LearnEnemySkill(void)
{
    unsigned short attackId;
    int skillBit;

    skillBit = 1 << g_pBattleCommandCtx->attackId - BATTLE_KERNEL_ENEMY_SKILL_FIRST;
    if ((g_BattleScene.flags & 0x40) == 0 && (((BattleCommandTargetState *)(g_pBattleCommandCtx->targetStatePtr))->learnMask & skillBit) == 0) {
        ((BattleCommandTargetState *)(g_pBattleCommandCtx->targetStatePtr))->learnMask |= skillBit;
        attackId = *(short *)&g_pBattleCommandCtx->attackId;
        Battle_PostBattleMessageFormatted(g_pBattleCommandCtx->targetSlot, BATTLE_MSG_LEARNED_ENEMY_SKILL, 1, &attackId);
        Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, g_pBattleCommandCtx->targetSlot, BATTLE_ACTION_EVENT_REBUILD_ABILITY_LISTS, attackId);
        g_pBattleCommandCtx->animId = 0xA;
    }
}

/*
 * Picks how the target reacts to a hit. A nonzero altPose (the target was KO'd before and the hit
 * changed that, i.e. a revive) gives an enemy pose 0x39 and leaves a party member's alone. Otherwise
 * a stopped target plays pose 0x30, a frog pose 5, anyone else its default hit reaction.
 */
// FUNCTION: FF7 0x005db8d6
void __cdecl Battle_SelectHitReactionPose(int altPose)
{
    if (altPose != 0) {
        if (g_pBattleCommandCtx->targetSlot >= 4) {
            g_pBattleCommandCtx->animId = 0x39;
        }
    } else if ((g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_STOP) != 0) {
        g_pBattleCommandCtx->animId = 0x30;
    } else if ((g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_FROG) != 0) {
        g_pBattleCommandCtx->animId = 5;
    } else {
        g_pBattleCommandCtx->animId = Battle_GetDefaultHitReactionPose();
    }
}

/*
 * Sets up one hit on a target: its slot, status, level, HP and MP, its hit pose, and fresh
 * per-hit results (resultFlags starts as the attack flags, the accuracy as the attack's, the
 * number fields at -1). An attack without power plays pose 0x33 on the target.
 *
 * The status change is decided here, once per hit: when Battle_RollStatusChangeSuccess succeeds,
 * the attack's inflict, cure and toggle masks become the hit's working masks, except that a
 * petrified target can be inflicted nothing; when it fails they stay empty, and a status-only
 * attack (power 0) misses outright. The target's status immunities follow; a revive may pass a
 * party member's Death immunity.
 *
 * Defence: an attack with BATTLE_SPECIAL_NOT_PIERCING set uses the target's Defense (physical) or
 * Magic Defense (magical) with its stat percentages; a piercing attack ignores it. Defence is
 * capped at 512, where the formulas' (512 - defence) reaches 0. An enemy target also records its
 * scene enemy index.
 */
// FUNCTION: FF7 0x005db960
void __cdecl Battle_BeginTargetHit(int targetIndex)
{
    int i;
    int mem;

    g_pBattleCommandCtx->targetSlot = targetIndex;
    g_pBattleCommandCtx->enemyIndex = -1;
    g_pBattleCommandCtx->clearedHitState = 0;
    g_pBattleCommandCtx->displayFlags = 0;
    g_pBattleCommandCtx->defence = 0;
    g_pBattleCommandCtx->damage = 0;
    g_pBattleCommandCtx->modifierFlags = 0;
    g_pBattleCommandCtx->resultFlags = g_pBattleCommandCtx->attackFlags;
    g_pBattleCommandCtx->accuracyWork = g_pBattleCommandCtx->accuracy;
    g_pBattleCommandCtx->animId =
        g_aCombatants[targetIndex].hitAnimId;
    g_pBattleCommandCtx->targetStatus = g_aCombatants[targetIndex].status;
    g_pBattleCommandCtx->targetLevel =
        g_aCombatants[targetIndex].level;
    g_pBattleCommandCtx->hp = g_aCombatants[targetIndex].curHp;
    g_pBattleCommandCtx->cap =
        g_aCombatants[targetIndex].curMp;
    g_pBattleCommandCtx->hitTargetPtr = (BattleHitTarget *)&g_BattleState.actors[targetIndex];
    if (targetIndex < 3) {
        mem = (int)&g_BattleState.partyStats[targetIndex];
    } else {
        mem = -1;
    }
    g_pBattleCommandCtx->targetStatePtr = mem;
    g_pBattleCommandCtx->hitDisplayValue = -1;
    g_pBattleCommandCtx->hitSecondaryValue = -1;
    g_pBattleCommandCtx->hitDamage = -1;
    Battle_ClearHitScratch();
    if (g_pBattleCommandCtx->power == 0) {
        g_pBattleCommandCtx->animId = 0x33;
    }
    if (Battle_RollStatusChangeSuccess()) {
        g_pBattleCommandCtx->workStatusUnion = 0;
        for (i = 0; i < 3; i++) {
            (&g_pBattleCommandCtx->workInflictMask)[i] =
                (&g_pBattleCommandCtx->statusMaskA)[i];
            if (i == 0 && (g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_PETRIFY) != 0) {
                (&g_pBattleCommandCtx->workInflictMask)[i] = 0;
            }
            g_pBattleCommandCtx->workStatusUnion |=
                (&g_pBattleCommandCtx->workInflictMask)[i];
        }
    } else if (g_pBattleCommandCtx->power == 0) {
        g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_MISS;
    }
    g_pBattleCommandCtx->immunityMask = Battle_BuildStatusImmunityMask(
        g_pBattleCommandCtx->targetSlot, 1, (g_pBattleCommandCtx->workCureMask & BATTLE_STATUS_DEATH) != 0);
    if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_PIERCING) != 0) {
        if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_PHYSICAL) == 0) {
            g_pBattleCommandCtx->defence = Battle_ApplyStatPercentModifier(
                targetIndex, g_aCombatants[targetIndex].defense,
                2);
        } else {
            g_pBattleCommandCtx->defence = Battle_ApplyStatPercentModifier(
                targetIndex, g_aCombatants[targetIndex].magicDefense,
                3);
        }
    }
    if (g_pBattleCommandCtx->defence > 0x200) {
        g_pBattleCommandCtx->defence = 0x200;
    }
    if (targetIndex >= 4) {
        g_pBattleCommandCtx->enemyIndex =
            g_BattleScene.enemies[targetIndex - 4].enemyIndex;
    }
}

/*
 * Battle_ClearHitScratch -- wipe the per-hit working state: the three status masks, their union,
 * the element reaction code and the damage. Used when a hit turns out to have no effect.
 */
// FUNCTION: FF7 0x005dbcdd
void __cdecl Battle_ClearHitScratch(void)
{
    g_pBattleCommandCtx->workInflictMask = 0;
    g_pBattleCommandCtx->workCureMask = 0;
    g_pBattleCommandCtx->workToggleMask = 0;
    g_pBattleCommandCtx->workStatusUnion = 0;
    g_pBattleCommandCtx->elemReaction = 0;
    g_pBattleCommandCtx->damage = 0;
}

/*
 * Ends an action that cannot happen, with a message unless messageId is -1. A party member still
 * plays the end of its command: the step of Magic (0x38), Summon (0x36), Enemy Skill (0x37) or
 * Limit (0x35), else step 3 followed by step 4; a message is shown as step 0x3B. An enemy only gets
 * the message, which names it (with its letter when several of its kind are in the battle). Does
 * not itself clear command state.
 */
// FUNCTION: FF7 0x005dbd40
void __cdecl Battle_AbortActionWithMessage(int messageId)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[8];
        short actorSlot;
        struct { unsigned char pad_to_2[0x2]; short nameSlot; };
        struct { unsigned char pad_to_4[0x4]; int effectStep; };
    } frame;
#pragma pack(pop)

    frame.effectStep = 3;
    switch (g_pBattleCommandCtx->stepFlags) {
    case BATTLE_COMMAND_MAGIC:
        frame.effectStep = 0x38;
        break;
    case BATTLE_COMMAND_SUMMON:
        frame.effectStep = 0x36;
        break;
    case BATTLE_COMMAND_ENEMY_SKILL:
        frame.effectStep = 0x37;
        break;
    case BATTLE_COMMAND_LIMIT:
        frame.effectStep = 0x35;
    }
    if (g_pBattleCommandCtx->attackerSlot < 3) {
        Battle_QueueCommandEffectStep(frame.effectStep);
        if (messageId != -1) {
            Battle_PostBattleMessage(messageId);
            Battle_QueueCommandEffectStep(0x3B);
        }
        if (frame.effectStep == 3) {
            Battle_QueueCommandEffectStep(4);
        }
    } else if (messageId != -1) {
        frame.actorSlot = *(short *)&g_pBattleCommandCtx->attackerSlot;
        frame.nameSlot = 0xFFFF;
        if ((*(unsigned char *)&g_BattleState.actors[g_pBattleCommandCtx->attackerSlot].nameSuffixIndex) != 0xFF) {
            frame.nameSlot = (*(unsigned char *)&g_BattleState.actors[g_pBattleCommandCtx->attackerSlot].nameSuffixIndex);
        }
        Battle_PostBattleMessageFormatted(g_pBattleCommandCtx->attackerSlot, messageId, 1, &frame.actorSlot);
    }
}

/*
 * Battle message 0x5B, "not enough MP", for a party member; an enemy gets the next one, 0x5C ("its
 * skill power is used up").
 */
#define BATTLE_MSG_NOT_ENOUGH_MP 0x5B

/*
 * Whether the attacker may use the command now, and its MP. Returns 1 when the action is aborted.
 *
 * Silence blocks Magic, Summon, Enemy Skill, W-Magic and W-Summon, and an enemy's own attack that
 * costs MP. A Frog may still Attack, use an Item, cast the one spell with attack id 0xA (Toad, if
 * the well-established rule that a frog can still cast Toad is what this encodes; the id is not
 * proven here) and use an enemy attack without MP cost; anything else is blocked.
 * A blocked command aborts without a message. Otherwise the MP cost is paid, or the action
 * aborts on message 0x5B (party) or 0x5C (enemy) when the MP is short.
 *
 * A combatant with BATTLE_COMBATANT_NO_CAST_CHECK, or step 0x34, skips all of it and pays nothing.
 * The MP cost field is cleared in every case.
 */
// FUNCTION: FF7 0x005dbe6f
int __cdecl Battle_ChargeMpAndCheckCastable(void)
{
    int result;
    int restricted;
    int messageId;

    result = 0;
    if ((g_aCombatants[g_pBattleCommandCtx->attackerSlot].flags & BATTLE_COMBATANT_NO_CAST_CHECK) == 0 && g_pBattleCommandCtx->stepId != 0x34) {
        restricted = 0;
        if ((g_pBattleCommandCtx->attackerStatus & BATTLE_STATUS_SILENCE) != 0) {
            switch (g_pBattleCommandCtx->stepFlags) {
            case BATTLE_COMMAND_MAGIC:
            case BATTLE_COMMAND_SUMMON:
            case BATTLE_COMMAND_ENEMY_SKILL:
            case BATTLE_COMMAND_W_MAGIC:
            case BATTLE_COMMAND_W_SUMMON:
                restricted = 1;
                break;
            case BATTLE_COMMAND_ENEMY_ATTACK:
                if (g_pBattleCommandCtx->mpCost != 0) {
                    restricted = 1;
                }
            }
        }
        if ((g_pBattleCommandCtx->attackerStatus & BATTLE_STATUS_FROG) != 0) {
            switch (g_pBattleCommandCtx->stepFlags) {
            case BATTLE_COMMAND_ATTACK:
            case BATTLE_COMMAND_ITEM:
                break;
            case BATTLE_COMMAND_MAGIC:
            case BATTLE_COMMAND_W_MAGIC:
                if (g_pBattleCommandCtx->attackId != 0xA) {
                    restricted = 1;
                }
                break;
            case BATTLE_COMMAND_ENEMY_ATTACK:
                if (g_pBattleCommandCtx->mpCost != 0) {
                    restricted = 1;
                }
                break;
            default:
                restricted = 1;
            }
        }
        if (restricted != 0) {
            Battle_AbortActionWithMessage(-1);
            result = 1;
        } else if (g_aCombatants[g_pBattleCommandCtx->attackerSlot].curMp >= g_pBattleCommandCtx->mpCost) {
            *(short *)&g_aCombatants[g_pBattleCommandCtx->attackerSlot].curMp -= *(short *)&g_pBattleCommandCtx->mpCost;
        } else {
            messageId = (g_pBattleCommandCtx->attackerSlot >= 3) + BATTLE_MSG_NOT_ENOUGH_MP;
            Battle_AbortActionWithMessage(messageId);
            result = 1;
        }
    }
    g_pBattleCommandCtx->mpCost = 0;
    return result;
}

/*
 * Returns whether the target is immune to the attack's kind: BATTLE_COMBATANT_IMMUNE_MAGIC when the
 * attack is not physical (BATTLE_SPECIAL_NOT_PHYSICAL), BATTLE_COMBATANT_IMMUNE_PHYSICAL otherwise.
 */
// FUNCTION: FF7 0x005dc08e
int __cdecl Battle_IsTargetImmuneToAttackKind(int targetSlot)
{
    int result;

    result = 0;
    if (g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_PHYSICAL) {
        if (g_aCombatants[targetSlot].flags & BATTLE_COMBATANT_IMMUNE_MAGIC) {
            result = 1;
        }
    } else if (g_aCombatants[targetSlot].flags & BATTLE_COMBATANT_IMMUNE_PHYSICAL) {
        result = 1;
    }
    return result;
}

/*
 * Battle_TargetTakesNoDamage -- true when the target is immune to this attack's kind, or is
 * petrified or peerless. The caller forces the damage to zero.
 */
// FUNCTION: FF7 0x005dc0ea
int __cdecl Battle_TargetTakesNoDamage(int targetSlot)
{
    int result;

    result = 0;
    if (Battle_IsTargetImmuneToAttackKind(targetSlot) != 0 ||
        (g_aCombatants[targetSlot].status & (BATTLE_STATUS_PETRIFY | BATTLE_STATUS_PEERLESS)) != 0) {
        result = 1;
    }
    return result;
}

/* A status chance of 0xFC (252) or more always lands. */
#define BATTLE_STATUS_CHANCE_ALWAYS 0xFC

/*
 * The status roll: does this hit's status change land? Returns 1 when it does.
 *
 * The chance is the attack's (statusChangeId). It always lands when the change is exactly Frog on
 * a frog or exactly Small on a small target (turning it back always works), and when Haste, Shield
 * or Berserk is put on a party member. Otherwise the chance grows by bonusPercent percent, drops to
 * 2/3 when an attack that does not target all by default is spread over several targets, and is
 * halved on a repeat cast (Quadra Magic); it lands when a random 1..100 is below it, so a chance of
 * N lands N - 1 times in 100. A change that includes Death never lands on an actor in
 * g_BattleState.excludedActorMask.
 *
 * Neither the level nor Magic Attack against Magic Defense matters: both differences are computed
 * and never used. With no status at all (an empty union) the Frog test matches a non-frog target
 * and the roll succeeds, which does nothing.
 */
// FUNCTION: FF7 0x005dc129
int __cdecl Battle_RollStatusChangeSuccess(void)
{
    int result;
    int chance;
    int statusUnion;
    int levelDiff;
    int statDiff;

    chance = g_pBattleCommandCtx->statusChangeId;
    levelDiff = g_pBattleCommandCtx->attackerLevel - g_pBattleCommandCtx->targetLevel;
    statDiff = g_aCombatants[g_pBattleCommandCtx->attackerSlot].magicAttack - g_aCombatants[g_pBattleCommandCtx->targetSlot].magicDefense;
    statusUnion = g_pBattleCommandCtx->statusMaskA | g_pBattleCommandCtx->statusMaskB | g_pBattleCommandCtx->statusMaskC;
    result = 1;
    if (g_pBattleCommandCtx->attackerLevel < g_pBattleCommandCtx->targetLevel) {
        levelDiff <<= 1;
        statDiff = statDiff / 16;
    } else {
        levelDiff = levelDiff / 2;
        statDiff = statDiff / 8;
    }
    // Undoing Frog or Small always works.
    if ((g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_FROG) == statusUnion) {
        chance = BATTLE_STATUS_CHANCE_ALWAYS;
    }
    if ((g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_SMALL) == statusUnion) {
        chance = BATTLE_STATUS_CHANCE_ALWAYS;
    }
    if (g_pBattleCommandCtx->targetSlot < 3 && (statusUnion & (BATTLE_STATUS_HASTE | BATTLE_STATUS_SHIELD | BATTLE_STATUS_BERSERK)) != 0) {
        chance = BATTLE_STATUS_CHANCE_ALWAYS;
    }
    if (chance < BATTLE_STATUS_CHANCE_ALWAYS) {
        chance += chance * g_pBattleCommandCtx->bonusPercent / 0x64;
        if ((g_pBattleCommandCtx->targetFlags & (BTL_TARGETF_DEFAULT_MULTIPLE | BTL_TARGETF_TOGGLE_MULTIPLE)) != BTL_TARGETF_DEFAULT_MULTIPLE && g_pBattleCommandCtx->targetCount > 1) {
            chance = (chance << 1) / 3;
        }
        if (g_pBattleCommandCtx->repeatCasts != 0) {
            chance >>= 1;
        }
        /* Retail caller treats `Battle_RandBelow` return (`int`) as `unsigned char` for caller zero/sign extension. */
        if (((unsigned char (__cdecl *)(int limit))Battle_RandBelow)(0x64) + 1 >= chance) {
            result = 0;
        }
    }
    if ((statusUnion & BATTLE_STATUS_DEATH) != 0 && (g_BattleState.excludedActorMask & 1 << g_pBattleCommandCtx->targetSlot) != 0) {
        result = 0;
    }
    return result;
}

/*
 * The number for a hit that only tried a status and changed nothing: when the attack carries a
 * status the target is not immune to and the hit record has no number yet, queues one with
 * damage -1.
 */
// FUNCTION: FF7 0x005dc323
void __cdecl Battle_PushStatusOnlyDisplay(BattleHitRecord *displayRecord)
{
    int statusBits;
    int invertedMask;

    statusBits = g_pBattleCommandCtx->statusMaskA | g_pBattleCommandCtx->statusMaskB | g_pBattleCommandCtx->statusMaskC;
    invertedMask = ~g_pBattleCommandCtx->immunityMask;
    if ((statusBits & invertedMask) != 0 && displayRecord->damageDisplayIndex == -1) {
        Battle_PushDamageDisplay(displayRecord, -1, 0, -1, -1);
    }
}
} /* extern "C" */
