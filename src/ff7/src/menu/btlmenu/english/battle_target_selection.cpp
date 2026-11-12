// src/ff7/src/menu/btlmenu/english/battle_target_selection.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Handles the battle target cursor: it loads the availability mask for the chosen target
//   mode, finds the nearest enabled target in a direction, and updates the selection from
//   directional input. It also reads the repeated direction keys.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/enums/battle.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include <stdlib.h>
#include "ff7/types/menu.h"
#include "ff7/api/menu.h"
#include "lib/api/file.h"
#include "lib/api/sound.h"
#include "ff7/api/battle.h"
#include "ff7/types/battle.h"
extern "C" {

#pragma intrinsic(abs)

// GLOBAL: FF7 0x00dc3c54
short g_BtlListCursorIndex = 0;
// GLOBAL: FF7 0x00dc3c58
unsigned char g_BtlTargetGroupIndex = 0;
// GLOBAL: FF7 0x00dc3c5c
int g_BtlMenuOption = 0;
// GLOBAL: FF7 0x00dc3c60
short g_BtlTargetAvailableMask = 0;
// GLOBAL: FF7 0x00dc3c64
unsigned char g_BtlEnemyIndex = 0;
// GLOBAL: FF7 0x00dc3c68
short g_BattleMenuCursorX = 0;
// GLOBAL: FF7 0x00dc3c6c
short g_BattleMenuCursorY = 0;
// GLOBAL: FF7 0x00dc3c70
unsigned char g_BtlIssuedCommandId = 0;
// GLOBAL: FF7 0x00dc3c74
unsigned char g_BtlSelectedCommand = 0;
// GLOBAL: FF7 0x00dc3c78
short g_BtlIssuedActionId = 0;
// GLOBAL: FF7 0x00dc3c7c
unsigned char g_BtlActiveActor = 0;
// GLOBAL: FF7 0x00dc3c80
unsigned char g_BtlActivePartyMember = 0;
// clang-format off
// GLOBAL: FF7 0x00dc3c84
short g_BtlTargetFlags = 0;
// GLOBAL: FF7 0x00dc3c88
unsigned char g_BattleCommandMenuPhase = 0;
// GLOBAL: FF7 0x00dc3c8c
unsigned char g_BtlTargetMode = 0;
// GLOBAL: FF7 0x00dc3c90
unsigned char g_BtlTargetSelectMode = BTL_TARGET_ONE_ANY;
// GLOBAL: FF7 0x00dc3c94
unsigned char g_BtlSingleTargetIndex = 0;
// GLOBAL: FF7 0x00dc3c98
unsigned char g_BtlGroupCycleIndex = 0;
// GLOBAL: FF7 0x00dc3c9c
unsigned char g_BtlPointedTarget = 0;
// clang-format on


/*
 * Copies the scene's target mask for g_BtlTargetMode BTL_TARGET_MASK_VALID (targetValidMask) or
 * BTL_TARGET_MASK_EXTRA (targetExtraMask) into g_BtlTargetAvailableMask. Other modes leave it
 * unchanged.
 */
// FUNCTION: FF7 0x006e5980
void __cdecl Battle_LoadTargetMaskForMode(void)
{
    switch (g_BtlTargetMode) {
    case BTL_TARGET_MASK_VALID:
        g_BtlTargetAvailableMask = g_BattleScene.targetValidMask;
        break;
    case BTL_TARGET_MASK_EXTRA:
        g_BtlTargetAvailableMask = g_BattleScene.targetExtraMask;
        break;
    }
}

/*
 * Moves g_BtlSingleTargetIndex to the next available target; returns at once when none is
 * available. The low two bits of mode pick where: BTL_TARGET_ONE_PARTY cycles slots 0 to 3,
 * BTL_TARGET_ONE_ENEMY slots 4 to 11, and BTL_TARGET_ONE_ANY stays on the cursor's side,
 * falling back to the party side when no enemy is available.
 */
// FUNCTION: FF7 0x006e59bc
void Battle_AdvanceTargetCursorInGroup(unsigned int mode)
{
    int i;

    if ((unsigned short)g_BtlTargetAvailableMask == 0) {
        return;
    }

    switch (mode & 3) {
    case BTL_TARGET_ONE_ANY:
        if (g_BtlSingleTargetIndex < 4 && ((unsigned short)g_BtlTargetAvailableMask & BATTLE_ACTOR_MASK_ALLIES) != 0) {
        scanLowSlots:
            for (i = 0; i < 20; i++) {
                g_BtlSingleTargetIndex++;
                g_BtlSingleTargetIndex %= 4;
                if (((unsigned short)g_BtlTargetAvailableMask & (1 << g_BtlSingleTargetIndex)) != 0) {
                    break;
                }
            }
        } else {
            for (i = 0; i < 20; i++) {
                g_BtlSingleTargetIndex++;
                if (g_BtlSingleTargetIndex >= 0xc) {
                    g_BtlSingleTargetIndex = 4;
                }
                if (((unsigned short)g_BtlTargetAvailableMask & (1 << g_BtlSingleTargetIndex)) != 0) {
                    break;
                }
            }
            if (((unsigned short)g_BtlTargetAvailableMask & (1 << g_BtlSingleTargetIndex)) == 0) {
                /* goto kept: the else jumps back into the then-branch's low-slot loop, which is laid out first */
                goto scanLowSlots;
            }
        }
        break;
    case BTL_TARGET_ONE_PARTY:
        if (((unsigned short)g_BtlTargetAvailableMask & BATTLE_ACTOR_MASK_ALLIES) == 0) {
            break;
        }
        {
            for (i = 0; i < 20; i++) {
                g_BtlSingleTargetIndex++;
                g_BtlSingleTargetIndex %= 4;
                if (((unsigned short)g_BtlTargetAvailableMask & (1 << g_BtlSingleTargetIndex)) != 0) {
                    break;
                }
            }
        }
        break;
    case BTL_TARGET_ONE_ENEMY:
        if (((unsigned short)g_BtlTargetAvailableMask & ~0xf) == 0) {
            break;
        }
        {
            for (i = 0; i < 20; i++) {
                g_BtlSingleTargetIndex++;
                if (g_BtlSingleTargetIndex >= 0xc) {
                    g_BtlSingleTargetIndex = 4;
                }
                if (((unsigned short)g_BtlTargetAvailableMask & (1 << g_BtlSingleTargetIndex)) != 0) {
                    break;
                }
            }
        }
        break;
    }

    g_BtlGroupCycleIndex = g_BtlSingleTargetIndex;
    g_BtlPointedTarget = g_BtlGroupCycleIndex;
}

/*
 * Returns the first target group (0..2) whose mask in the scene's targetMaskTable holds the
 * cursor's target (g_BtlSingleTargetIndex) among the available ones, or 0.
 */
// FUNCTION: FF7 0x006e5bfd
int __cdecl Battle_FindTargetGroupOfCursor(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (((1 << g_BtlSingleTargetIndex) & (unsigned short)g_BtlTargetAvailableMask & g_BattleScene.targetMaskTable[i]) != 0) {
            return i;
        }
    }
    return 0;
}

/*
 * Chooses the first selection mode and cursor target from the command's target flags and the
 * short result of Battle_GetWindowState(1) (2 inverts the starting side). DEFAULT_MULTIPLE
 * starts on a whole side, or on everyone with ALL_ROWS (BTL_TARGET_ALL, the cursor left as it
 * was); otherwise ONE_SIDE_ONLY keeps the cursor to one side, and START_ON_ENEMIES picks which.
 * An unavailable first target moves the cursor on.
 */
// FUNCTION: FF7 0x006e5c52
void __cdecl Battle_BeginTargetSelection(void)
{
    int p;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    p = (short)Battle_GetWindowState(1);
    Battle_LoadTargetMaskForMode();
    if (((unsigned short)g_BtlTargetFlags & BTL_TARGETF_DEFAULT_MULTIPLE) != 0) {
        if (((unsigned short)g_BtlTargetFlags & BTL_TARGETF_ALL_ROWS) == 0) {
            if (p == 2) {
                if (((unsigned short)g_BtlTargetFlags & BTL_TARGETF_START_ON_ENEMIES) != 0) {
                    g_BtlTargetSelectMode = BTL_TARGET_ONE_ENEMY;
                    g_BtlSingleTargetIndex = 4;
                } else {
                    g_BtlTargetSelectMode = BTL_TARGET_ONE_PARTY;
                    g_BtlSingleTargetIndex = g_BtlActiveActor;
                }
            } else if (((unsigned short)g_BtlTargetFlags & BTL_TARGETF_START_ON_ENEMIES) != 0) {
                g_BtlTargetSelectMode = BTL_TARGET_ONE_PARTY;
                g_BtlSingleTargetIndex = 0;
            } else {
                g_BtlTargetSelectMode = BTL_TARGET_ONE_ENEMY;
                g_BtlSingleTargetIndex = 4;
            }
            g_BtlTargetGroupIndex = Battle_FindTargetGroupOfCursor();
            g_BtlTargetSelectMode |= BTL_TARGET_MULTIPLE;
        } else {
            g_BtlTargetSelectMode = BTL_TARGET_ALL;
        }
    } else if (((unsigned short)g_BtlTargetFlags & BTL_TARGETF_START_ON_ENEMIES) != 0) {
        if (p == 2) {
            if (((unsigned short)g_BtlTargetFlags & BTL_TARGETF_ONE_SIDE_ONLY) != 0) {
                g_BtlTargetSelectMode = BTL_TARGET_ONE_ENEMY;
            } else {
                g_BtlTargetSelectMode = BTL_TARGET_ONE_ANY;
            }
            g_BtlSingleTargetIndex = 4;
        } else {
            if (((unsigned short)g_BtlTargetFlags & BTL_TARGETF_ONE_SIDE_ONLY) != 0) {
                g_BtlTargetSelectMode = BTL_TARGET_ONE_PARTY;
            } else {
                g_BtlTargetSelectMode = BTL_TARGET_ONE_ANY;
            }
            g_BtlSingleTargetIndex = 0;
        }
    } else if (p == 2) {
        if (((unsigned short)g_BtlTargetFlags & BTL_TARGETF_ONE_SIDE_ONLY) != 0) {
            g_BtlTargetSelectMode = BTL_TARGET_ONE_PARTY;
        } else {
            g_BtlTargetSelectMode = BTL_TARGET_ONE_ANY;
        }
        g_BtlSingleTargetIndex = g_BtlActiveActor;
    } else {
        if (((unsigned short)g_BtlTargetFlags & BTL_TARGETF_ONE_SIDE_ONLY) != 0) {
            g_BtlTargetSelectMode = BTL_TARGET_ONE_ENEMY;
        } else {
            g_BtlTargetSelectMode = BTL_TARGET_ONE_ANY;
        }
        g_BtlSingleTargetIndex = g_BtlEnemyIndex;
    }
    if (((unsigned short)g_BtlTargetAvailableMask & 1 << g_BtlSingleTargetIndex) == 0) {
        Battle_AdvanceTargetCursorInGroup(g_BtlTargetSelectMode);
        g_BtlTargetGroupIndex = Battle_FindTargetGroupOfCursor();
    }
    g_BtlGroupCycleIndex = g_BtlSingleTargetIndex;
    g_BtlPointedTarget = g_BtlGroupCycleIndex;
}

/*
 * Returns the nearest available target (by screen position) in the requested one of
 * eight directions, among the twelve slots other than current; current when none
 * qualifies. group is the selection mode: BTL_TARGET_ONE_PARTY keeps to slots 0 to 2,
 * BTL_TARGET_ONE_ENEMY to slots 3 to 11.
 */
// FUNCTION: FF7 0x006e5e3b
short __cdecl Battle_FindNearestTargetInDirection(short current, short group, short direction)
{
    struct {
        short indices[10];
        short xs[10];
        short bestIndex;
        short padBestIndex;
        short qualifies;
        short padQualifies;
        short candidateY;
        short padCandidateY;
        short candidateX;
        short padCandidateX;
        short bestDistance;
        short padBestDistance;
        short i;
        short padI;
        short count;
        short padCount;
        short distance;
        short padDistance;
        short currentY;
        short padCurrentY;
        short currentX;
        short padCurrentX;
        short ys[10];
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    s.count = 0;
    s.currentX = g_BattleActorScreenPositions[current].prevX;
    s.currentY = g_BattleActorScreenPositions[current].prevY;
    for (s.i = 0; s.i < 12; s.i++) {
        if (s.i == current) {
            continue;
        }
        if (group == BTL_TARGET_ONE_PARTY && s.i > 2) {
            continue;
        }
        if (group == BTL_TARGET_ONE_ENEMY && s.i < 3) {
            continue;
        }
        if (((unsigned short)g_BtlTargetAvailableMask & (1 << s.i)) == 0) {
            continue;
        }

        s.candidateX = g_BattleActorScreenPositions[s.i].prevX;
        s.candidateY = g_BattleActorScreenPositions[s.i].prevY;
        switch (direction) {
        case BTL_DIR_UP:
            s.qualifies = abs(s.currentX - s.candidateX) <= abs(s.currentY - s.candidateY) && s.currentY >= s.candidateY;
            break;
        case BTL_DIR_DOWN:
            s.qualifies = abs(s.currentX - s.candidateX) <= abs(s.currentY - s.candidateY) && s.currentY <= s.candidateY;
            break;
        case BTL_DIR_RIGHT:
            s.qualifies = abs(s.currentX - s.candidateX) >= abs(s.currentY - s.candidateY) && s.currentX <= s.candidateX;
            break;
        case BTL_DIR_LEFT:
            s.qualifies = abs(s.currentX - s.candidateX) >= abs(s.currentY - s.candidateY) && s.currentX >= s.candidateX;
            break;
        case BTL_DIR_UP_RIGHT:
            s.qualifies = s.currentY >= s.candidateY && s.currentX <= s.candidateX;
            break;
        case BTL_DIR_UP_LEFT:
            s.qualifies = s.currentY >= s.candidateY && s.currentX >= s.candidateX;
            break;
        case BTL_DIR_DOWN_RIGHT:
            s.qualifies = s.currentY <= s.candidateY && s.currentX <= s.candidateX;
            break;
        case BTL_DIR_DOWN_LEFT:
            s.qualifies = s.currentY <= s.candidateY && s.currentX >= s.candidateX;
            break;
        }
        if (s.qualifies != 0) {
            s.indices[s.count] = s.i;
            s.xs[s.count] = s.candidateX;
            s.ys[s.count] = s.candidateY;
            s.count++;
        }
    }
    if (s.count == 0) {
        return current;
    }

    s.bestDistance = 32000;
    for (s.i = 0; s.i < s.count; s.i++) {
        s.distance = (short)((abs(s.currentX - s.xs[s.i]) + abs(s.currentY - s.ys[s.i])) >> 1);
        if (s.distance < s.bestDistance) {
            s.bestDistance = s.distance;
            s.bestIndex = s.indices[s.i];
        }
    }
    return s.bestIndex;
}

/*
 * The four direction keys, asked for one at a time and reassembled into the same
 * four bits they were asked with. The original asks in the order up, down, left,
 * right.
 */
// FUNCTION: FF7 0x006e6217
unsigned int __cdecl Battle_ReadDirectionRepeatMask(void)
{
    unsigned int keys;

    keys = 0;
    if (Input_GetRepeat(INPUT_UP) != 0) {
        keys |= INPUT_UP;
    }
    if (Input_GetRepeat(INPUT_DOWN) != 0) {
        keys |= INPUT_DOWN;
    }
    if (Input_GetRepeat(INPUT_LEFT) != 0) {
        keys |= INPUT_LEFT;
    }
    if (Input_GetRepeat(INPUT_RIGHT) != 0) {
        keys |= INPUT_RIGHT;
    }
    return keys;
}

/*
 * Updates the target selection from the direction keys, the target flags and the
 * available targets. In the BTL_TARGET_ALL_PARTY and _ALL_ENEMIES modes, left and right
 * first move between the target groups of a side or pincer attack. The single-target
 * modes move the cursor to the nearest target in the pressed direction (eight
 * directions, diagonals as two keys); the multiple-target modes cycle the highlighted
 * target through everyone, the party side or the enemy side, at most twenty steps, once
 * the text subsystem is ready. L1 or R1 toggles single and multiple when the flags
 * allow it.
 */
// FUNCTION: FF7 0x006e6291
void Battle_UpdateTargetSelection(void)
{
    struct {
        short direction, padding;
        unsigned int pressed;
        int attempts;
        unsigned short mask, maskPadding;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#define TARGET_FLAGS ((unsigned short)g_BtlTargetFlags)
#define TARGET_MASK ((unsigned short)g_BtlTargetAvailableMask)
    s.pressed = Battle_ReadDirectionRepeatMask();
    switch (g_BtlTargetSelectMode) {
    case BTL_TARGET_ALL_PARTY:
        if (BtlMenu_GetInputRepeat(INPUT_RIGHT) || BtlMenu_GetInputRepeat(INPUT_LEFT)) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
            switch (g_BattleScene.layout) {
            case BATTLE_LAYOUT_SIDE_ATTACK_1:
            case BATTLE_LAYOUT_SIDE_ATTACK_2:
            case BATTLE_LAYOUT_SIDE_ATTACK_3:
            case BATTLE_LAYOUT_SIDE_ATTACK_4:
                if (!(TARGET_FLAGS & BTL_TARGETF_ONE_SIDE_ONLY) && (TARGET_FLAGS & BTL_TARGETF_SELECTABLE)) {
                    if (BtlMenu_GetInputRepeat(INPUT_RIGHT)) {
                        if (g_BtlTargetGroupIndex == 0) {
                            if (TARGET_MASK & g_BattleScene.targetMaskTable[2])
                                g_BtlTargetGroupIndex = 2;
                        } else if (TARGET_MASK & g_BattleScene.targetMaskTable[1]) {
                            g_BtlTargetSelectMode = BTL_TARGET_ALL_ENEMIES;
                            g_BtlTargetGroupIndex = 1;
                        }
                    } else if (BtlMenu_GetInputRepeat(INPUT_LEFT)) {
                        if (g_BtlTargetGroupIndex == 0) {
                            if (TARGET_MASK & g_BattleScene.targetMaskTable[1]) {
                                g_BtlTargetGroupIndex = 1;
                                g_BtlTargetSelectMode = BTL_TARGET_ALL_ENEMIES;
                            }
                        } else if (TARGET_MASK & g_BattleScene.targetMaskTable[0])
                            g_BtlTargetGroupIndex = 0;
                    }
                } else if (TARGET_FLAGS & BTL_TARGETF_SELECTABLE) {
                    if (g_BtlTargetGroupIndex == 0) {
                        if (TARGET_MASK & g_BattleScene.targetMaskTable[2])
                            g_BtlTargetGroupIndex = 2;
                    } else if (TARGET_MASK & g_BattleScene.targetMaskTable[0])
                        g_BtlTargetGroupIndex = 0;
                }
                break;
            case BATTLE_LAYOUT_PINCER_ATTACK:
                if (!(TARGET_FLAGS & BTL_TARGETF_ONE_SIDE_ONLY) && (TARGET_FLAGS & BTL_TARGETF_SELECTABLE)) {
                    if (BtlMenu_GetInputRepeat(INPUT_LEFT)) {
                        if (TARGET_MASK & g_BattleScene.targetMaskTable[0]) {
                            g_BtlTargetSelectMode = BTL_TARGET_ALL_ENEMIES;
                            g_BtlTargetGroupIndex = 0;
                        }
                    } else if (BtlMenu_GetInputRepeat(INPUT_RIGHT)) {
                        if (TARGET_MASK & g_BattleScene.targetMaskTable[2]) {
                            g_BtlTargetSelectMode = BTL_TARGET_ALL_ENEMIES;
                            g_BtlTargetGroupIndex = 2;
                        }
                    }
                }
                break;
            default:
                if (!(TARGET_FLAGS & BTL_TARGETF_ONE_SIDE_ONLY) && (TARGET_FLAGS & BTL_TARGETF_SELECTABLE))
                    g_BtlTargetSelectMode = BTL_TARGET_ALL_ENEMIES;
                break;
            }
        }
        break;
    case BTL_TARGET_ALL_ENEMIES:
        if (BtlMenu_GetInputRepeat(INPUT_RIGHT) || BtlMenu_GetInputRepeat(INPUT_LEFT)) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
            switch (g_BattleScene.layout) {
            case BATTLE_LAYOUT_SIDE_ATTACK_1:
            case BATTLE_LAYOUT_SIDE_ATTACK_2:
            case BATTLE_LAYOUT_SIDE_ATTACK_3:
            case BATTLE_LAYOUT_SIDE_ATTACK_4:
                if (!(TARGET_FLAGS & BTL_TARGETF_ONE_SIDE_ONLY) && (TARGET_FLAGS & BTL_TARGETF_SELECTABLE)) {
                    if (BtlMenu_GetInputRepeat(INPUT_RIGHT)) {
                        if (TARGET_MASK & g_BattleScene.targetMaskTable[0]) {
                            g_BtlTargetSelectMode = BTL_TARGET_ALL_PARTY;
                            g_BtlTargetGroupIndex = 0;
                        }
                    } else if (BtlMenu_GetInputRepeat(INPUT_LEFT)) {
                        if (TARGET_MASK & g_BattleScene.targetMaskTable[2]) {
                            g_BtlTargetSelectMode = BTL_TARGET_ALL_PARTY;
                            g_BtlTargetGroupIndex = 2;
                        }
                    }
                }
                break;
            case BATTLE_LAYOUT_PINCER_ATTACK:
                if (!(TARGET_FLAGS & BTL_TARGETF_ONE_SIDE_ONLY) && (TARGET_FLAGS & BTL_TARGETF_SELECTABLE)) {
                    if (BtlMenu_GetInputRepeat(INPUT_LEFT)) {
                        if (g_BtlTargetGroupIndex == 0) {
                            if (TARGET_MASK & g_BattleScene.targetMaskTable[2])
                                g_BtlTargetGroupIndex = 2;
                        } else if (TARGET_MASK & g_BattleScene.targetMaskTable[1]) {
                            g_BtlTargetGroupIndex = 1;
                            g_BtlTargetSelectMode = BTL_TARGET_ALL_PARTY;
                        }
                    } else if (BtlMenu_GetInputRepeat(INPUT_RIGHT)) {
                        if (g_BtlTargetGroupIndex == 0) {
                            if (TARGET_MASK & g_BattleScene.targetMaskTable[1]) {
                                g_BtlTargetGroupIndex = 1;
                                g_BtlTargetSelectMode = BTL_TARGET_ALL_PARTY;
                            }
                        } else if (TARGET_MASK & g_BattleScene.targetMaskTable[0])
                            g_BtlTargetGroupIndex = 0;
                    }
                } else if (TARGET_FLAGS & BTL_TARGETF_SELECTABLE) {
                    if (g_BtlTargetGroupIndex == 0) {
                        if (TARGET_MASK & g_BattleScene.targetMaskTable[2])
                            g_BtlTargetGroupIndex = 2;
                    } else if (TARGET_MASK & g_BattleScene.targetMaskTable[0])
                        g_BtlTargetGroupIndex = 0;
                }
                break;
            default:
                if (!(TARGET_FLAGS & BTL_TARGETF_ONE_SIDE_ONLY) && (TARGET_FLAGS & BTL_TARGETF_SELECTABLE))
                    g_BtlTargetSelectMode = BTL_TARGET_ALL_PARTY;
                break;
            }
        }
        break;
    }
    switch (g_BtlTargetSelectMode) {
    case BTL_TARGET_ONE_ANY:
    case BTL_TARGET_ONE_PARTY:
    case BTL_TARGET_ONE_ENEMY:
        if ((TARGET_MASK & (1 << g_BtlSingleTargetIndex)) == 0)
            Battle_AdvanceTargetCursorInGroup(g_BtlTargetSelectMode);
        else if (s.pressed != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
            switch (s.pressed) {
            case INPUT_UP:
                s.direction = BTL_DIR_UP;
                break;
            case INPUT_DOWN:
                s.direction = BTL_DIR_DOWN;
                break;
            case INPUT_RIGHT:
                s.direction = BTL_DIR_RIGHT;
                break;
            case INPUT_LEFT:
                s.direction = BTL_DIR_LEFT;
                break;
            case INPUT_UP | INPUT_RIGHT:
                s.direction = BTL_DIR_UP_RIGHT;
                break;
            case INPUT_UP | INPUT_LEFT:
                s.direction = BTL_DIR_UP_LEFT;
                break;
            case INPUT_DOWN | INPUT_RIGHT:
                s.direction = BTL_DIR_DOWN_RIGHT;
                break;
            case INPUT_DOWN | INPUT_LEFT:
                s.direction = BTL_DIR_DOWN_LEFT;
                break;
            }
            if (TARGET_FLAGS & BTL_TARGETF_SELECTABLE)
                g_BtlSingleTargetIndex = Battle_FindNearestTargetInDirection(g_BtlSingleTargetIndex, g_BtlTargetSelectMode, s.direction);
        }
        g_BtlPointedTarget = g_BtlSingleTargetIndex;
        break;
    case BTL_TARGET_ALL:
        if (!Menu_IsTextSubsystemReady())
            break;
        if (!TARGET_MASK)
            break;
        for (s.attempts = 0; s.attempts < 20; s.attempts++) {
            g_BtlGroupCycleIndex++;
            g_BtlGroupCycleIndex %= 12;
            if (TARGET_MASK & (1 << g_BtlGroupCycleIndex))
                break;
        }
        g_BtlPointedTarget = g_BtlGroupCycleIndex;
        Battle_FindTargetGroupOfCursor();
        break;
    case BTL_TARGET_ALL_PARTY:
        s.mask = TARGET_MASK;
        switch (g_BattleScene.layout) {
        case BATTLE_LAYOUT_SIDE_ATTACK_1:
        case BATTLE_LAYOUT_PINCER_ATTACK:
        case BATTLE_LAYOUT_SIDE_ATTACK_2:
        case BATTLE_LAYOUT_SIDE_ATTACK_3:
        case BATTLE_LAYOUT_SIDE_ATTACK_4:
            s.mask &= g_BattleScene.targetMaskTable[g_BtlTargetGroupIndex];
            break;
        }
        if (!Menu_IsTextSubsystemReady())
            break;
        if (!(s.mask & BATTLE_ACTOR_MASK_ALLIES))
            break;
        for (s.attempts = 0; s.attempts < 20; s.attempts++) {
            g_BtlGroupCycleIndex++;
            g_BtlGroupCycleIndex %= 4;
            if (s.mask & (1 << g_BtlGroupCycleIndex))
                break;
        }
        g_BtlPointedTarget = g_BtlGroupCycleIndex;
        Battle_FindTargetGroupOfCursor();
        break;
    case BTL_TARGET_ALL_ENEMIES:
        s.mask = TARGET_MASK;
        switch (g_BattleScene.layout) {
        case BATTLE_LAYOUT_SIDE_ATTACK_1:
        case BATTLE_LAYOUT_PINCER_ATTACK:
        case BATTLE_LAYOUT_SIDE_ATTACK_2:
        case BATTLE_LAYOUT_SIDE_ATTACK_3:
        case BATTLE_LAYOUT_SIDE_ATTACK_4:
            s.mask &= g_BattleScene.targetMaskTable[g_BtlTargetGroupIndex];
            break;
        }
        if (!Menu_IsTextSubsystemReady())
            break;
        if (!(s.mask & ~15))
            break;
        for (s.attempts = 0; s.attempts < 20; s.attempts++) {
            g_BtlGroupCycleIndex++;
            if (g_BtlGroupCycleIndex >= 12)
                g_BtlGroupCycleIndex = 4;
            if (s.mask & (1 << g_BtlGroupCycleIndex))
                break;
        }
        g_BtlPointedTarget = g_BtlGroupCycleIndex;
        Battle_FindTargetGroupOfCursor();
        break;
    }
    if ((BtlMenu_GetInputRepeat(INPUT_L1) || BtlMenu_GetInputRepeat(INPUT_R1)) &&
        (TARGET_FLAGS & BTL_TARGETF_TOGGLE_MULTIPLE) && (TARGET_FLAGS & BTL_TARGETF_SELECTABLE) && !(TARGET_FLAGS & BTL_TARGETF_ALL_ROWS)) {
        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
        if (g_BtlTargetSelectMode & BTL_TARGET_MULTIPLE) {
            g_BtlTargetSelectMode &= (unsigned char)~BTL_TARGET_MULTIPLE;
            if (!(TARGET_FLAGS & BTL_TARGETF_ONE_SIDE_ONLY))
                g_BtlTargetSelectMode = BTL_TARGET_ONE_ANY;
        } else {
            g_BtlTargetSelectMode |= 4;
            if (g_BtlTargetSelectMode == BTL_TARGET_ALL) {
                if (g_BtlPointedTarget < 4)
                    g_BtlTargetSelectMode = BTL_TARGET_ALL_PARTY;
                else
                    g_BtlTargetSelectMode = BTL_TARGET_ALL_ENEMIES;
            }
            g_BtlTargetGroupIndex = Battle_FindTargetGroupOfCursor();
        }
    }
#undef TARGET_FLAGS
#undef TARGET_MASK
}
} /* extern "C" */
