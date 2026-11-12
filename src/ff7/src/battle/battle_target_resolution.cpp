// src/ff7/src/battle/battle_target_resolution.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Resolves the battle target mask from the current valid and eligible masks, extra
//   targets and attack flags.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"

#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "ff7/enums/battle.h"

#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "lib/codegen.h"
extern "C" {

/*
 * Resolves a battle target mask from current valid/eligible masks, extra targets and the attack's
 * targeting byte (BattleTargetFlag). Remaps invalid selected actors, handles self (no flags) and
 * all-rows targets, side changes (START_ON_ENEMIES, the attacker's side, Confusion or Manipulate
 * outside summons and limits) and Confusion's random held target, then intersects eligibility
 * (ONE_SIDE_ONLY keeps the chosen side, SHORT_RANGE the scene's targetValidMask). Multi-target selection expands through side/row
 * masks; fallback and single-target rules choose random surviving bits. Records the first resolved
 * mask when its saved mask is zero.
 */
// FUNCTION: FF7 0x005dc390
void Battle_ResolveTargetMask(void)
{
    int valid;
    int flags;
    int hit;
    int extra;
    int i;
    int multi;
    int sel;
    int tgt;
    int flip;
    int held;

    valid = g_BattleScene.targetMask;
    flags = g_pBattleCommandCtx->targetFlags;
    if ((g_pBattleCommandCtx->attackFlags & 0x1000000) == 0) {
        valid &= g_BattleScene.targetMaskBits;
        if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_KO_ONLY) == 0 ||
            (g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_KO_TARGETABLE) == 0) {
            extra = ((unsigned short *)&g_BattleScene.targetExtraMask)[4];
            extra &= ~g_BattleState.excludedActorMask;
            valid |= extra;
        }
    } else {
        valid |= ((unsigned short *)&g_BattleScene.targetExtraMask)[4];
    }
    hit = (g_pBattleCommandCtx->targetMask ^ valid) & g_pBattleCommandCtx->targetMask;
    if (hit != 0) {
        for (i = 0; i < 10; i++) {
            if ((hit & (1 << i)) != 0 &&
                g_BattleState.actors[i].linkedActor != 0xff) {
                g_pBattleCommandCtx->targetMask |=
                    1 << g_BattleState.actors[i].linkedActor;
            }
        }
    }
    if (flags == 0) {
        g_pBattleCommandCtx->targetMask = 1 << g_pBattleCommandCtx->attackerSlot;
    } else if ((flags & BTL_TARGETF_ALL_ROWS) != 0) {
        g_pBattleCommandCtx->targetMask = g_BattleScene.targetMaskBits & ~((unsigned short *)&g_BattleScene.targetExtraMask)[4];
    } else {
        multi = 0;
        flip = 0;
        held = 0;
        if ((flags & BTL_TARGETF_START_ON_ENEMIES) != 0) {
            flip ^= 1;
        }
        if (g_pBattleCommandCtx->attackerSlot < 4) {
            flip ^= 1;
        }
        switch (g_pBattleCommandCtx->stepFlags) {
        case BATTLE_COMMAND_SUMMON:
        case BATTLE_COMMAND_LIMIT:
            break;
        default:
            if ((g_aCombatants[g_pBattleCommandCtx->attackerSlot].status & (BATTLE_STATUS_CONFUSION | BATTLE_STATUS_MANIPULATE)) != 0) {
                flip ^= 1;
            }
            if ((g_aCombatants[g_pBattleCommandCtx->attackerSlot].status & BATTLE_STATUS_CONFUSION) != 0) {
                held = 1;
            }
            break;
        }
        if ((flags & (BTL_TARGETF_DEFAULT_MULTIPLE | BTL_TARGETF_TOGGLE_MULTIPLE)) == BTL_TARGETF_DEFAULT_MULTIPLE) {
            multi = 1;
        }
        if (g_pBattleCommandCtx->repeatCasts != 0) {
            multi = 1;
            flags |= BTL_TARGETF_DEFAULT_MULTIPLE;
        }
        tgt = flip ? BATTLE_ACTOR_MASK_ALLIES : BATTLE_ACTOR_MASK_ENEMIES;
        if (held != 0) {
            g_pBattleCommandCtx->targetMask = tgt;
            if ((g_pBattleCommandCtx->attackFlags & 0x200) == 0) {
                g_pBattleCommandCtx->targetMask =
                    Battle_PickRandomBitFromMask(g_pBattleCommandCtx->targetMask);
            }
        }
        if ((flags & BTL_TARGETF_ONE_SIDE_ONLY) != 0) {
            valid &= tgt;
        } else if ((flags & BTL_TARGETF_SHORT_RANGE) != 0) {
            valid &= (*(unsigned short *)&g_BattleScene.targetValidMask);
        }
        g_pBattleCommandCtx->targetMask &= valid;
        tgt &= valid;
        if ((g_pBattleCommandCtx->attackFlags & 0x200000) == 0) {
            if ((g_pBattleCommandCtx->targetMask != 0 &&
                 Battle_PopCountMask(*(unsigned int *)&g_pBattleCommandCtx->resolvedTargetMask) >
                     1) ||
                multi != 0) {
                valid &= (g_pBattleCommandCtx->targetMask & BATTLE_ACTOR_MASK_ALLIES) ? BATTLE_ACTOR_MASK_ALLIES : BATTLE_ACTOR_MASK_ENEMIES;
                if ((g_pBattleCommandCtx->targetMask & g_BattleScene.targetMaskTable[0]) == 0) {
                    valid &= ~g_BattleScene.targetMaskTable[0];
                }
                if ((g_pBattleCommandCtx->targetMask & g_BattleScene.targetMaskTable[2]) == 0) {
                    valid &= ~g_BattleScene.targetMaskTable[2];
                }
                g_pBattleCommandCtx->targetMask = valid;
            }
            if (g_pBattleCommandCtx->targetMask == 0) {
                if ((flags & BTL_TARGETF_DEFAULT_MULTIPLE) != 0 && (flags & BTL_TARGETF_TOGGLE_MULTIPLE) != 0 &&
                    (g_pBattleCommandCtx->attackFlags & 0x200) == 0) {
                    tgt = Battle_PickRandomBitFromMask(tgt);
                }
                g_pBattleCommandCtx->targetMask = tgt;
            }
            if ((flags & BTL_TARGETF_DEFAULT_MULTIPLE) == 0 ||
                (g_pBattleCommandCtx->attackFlags & 0x100200) == 0x100200) {
                g_pBattleCommandCtx->targetMask =
                    Battle_PickRandomBitFromMask(g_pBattleCommandCtx->targetMask);
            }
        }
        if ((g_pBattleCommandCtx->targetMask & g_BattleScene.targetMaskTable[0]) != 0 &&
            (g_pBattleCommandCtx->targetMask & g_BattleScene.targetMaskTable[2]) != 0) {
            sel = Battle_NextRandomByte() & 2;
            g_pBattleCommandCtx->targetMask &= g_BattleScene.targetMaskTable[sel];
            *(unsigned int *)&g_pBattleCommandCtx->mask &= g_BattleScene.targetMaskTable[sel];
        }
    }
    if (g_pBattleCommandCtx->resolvedTargetMask == 0) {
        g_pBattleCommandCtx->resolvedTargetMask = g_pBattleCommandCtx->targetMask;
    }
}
} /* extern "C" */
