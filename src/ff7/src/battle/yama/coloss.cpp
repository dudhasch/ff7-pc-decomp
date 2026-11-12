// src/ff7/src/battle/yama/coloss.cpp
/* Battle Square / Colosseum Opponent Chooser (orig. coloss.cpp) */
// Original: C:\FF7\src\Battle\yama\coloss.cpp
// Contents: Runs the Battle Square and Colosseum arena state machine, which loads the arena data
//   and updates combatants for the next arena battle.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include <string.h>
#include <stdio.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/file.h"
#include "lib/api/movie.h"
#include "ff7/savemap.h"
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"
extern "C" {

#pragma intrinsic(memcpy)
// clang-format off
// GLOBAL: FF7 0x009ae14c
int g_BattleSquareOpponentIndex = 0;
// GLOBAL: FF7 0x009ae150
int g_BattleSquareCandidateMask = 0;
// GLOBAL: FF7 0x009ae154
int g_BattleSquareState = 0;
// clang-format on

/*
 * Advances the Battle Square / Colosseum arena state machine:
 * loads CO.BIN, updates combatant HP/MP back to savemap character records,
 * and executes slot penalty selection between consecutive arena rounds.
 */
// FUNCTION: FF7 0x00431890
unsigned int __cdecl Battle_RunArenaNextBattleChooser(void)
{
    int result;
    int blockBytes;
    int charSlot;
    int slotIndex;
    int sectionId;
    int partyCharacterId;
    int dstAddr;
    char pathBuf[200];
    int penaltyResult;

    result = 0xFFFF;
    switch (g_BattleSquareState) {
    case BATTLE_ARENA_CHOOSER_COLLECT:
        g_BattleSquareCandidateMask = 0;
        for (slotIndex = 0; slotIndex < 4; slotIndex++) {
            if (g_BattleScene.arenaOpponents[slotIndex] != 0xFFFF) {
                g_BattleSquareCandidateMask |= 1 << slotIndex;
            }
        }
        if (g_BattleSquareCandidateMask == 0) {
            g_BattleArenaChooserDone = 1;
            return result;
        }
        g_BattleSquareState = BATTLE_ARENA_CHOOSER_PLAY_SEQUENCE;
    case BATTLE_ARENA_CHOOSER_PLAY_SEQUENCE:
        Battle_PlaySequence();
        if (g_BattleSequencePlayDone != 0) {
            g_BattleSequencePlayDone = 0;
            g_BattleSquareState = BATTLE_ARENA_CHOOSER_LOAD;
        }
        g_BattleArenaChooserPending = 1;
        break;
    case BATTLE_ARENA_CHOOSER_LOAD:
        sprintf(pathBuf,
                // STRING: FF7 0x007c28c0
                "%s/CO.BIN", Battle_GetDataDirectory());
        LoadFileBlock((int)pathBuf, 0, 0, (void **)&g_Battle_sceneBuffer, 0);
        g_Battle_fileBuffer = (int)GameHeapAllocRaw(65536,
                                        // STRING: FF7 0x007c28cc
                                        "C:\\FF7\\src\\Battle\\yama\\coloss.cpp", 87);
        BinSectionReaderInit((unsigned char *)g_Battle_sceneBuffer);
        while ((*(short *)&sectionId = BinSectionPeekType()), (sectionId & 0xFFFF) != 0xFFFF) {
            dstAddr = g_Battle_fileBuffer + ((sectionId & 0xFFFF) << 15);
            blockBytes = BinSectionRead((void *)dstAddr);
            if ((sectionId & 0xFFFF) == 0 && blockBytes > 0) {
                if (g_pBtlMenuUiTextures != 0) {
                    GameHeapFree(g_pBtlMenuUiTextures,
                                 // STRING: FF7 0x007c28f0
                                 "C:\\FF7\\src\\Battle\\yama\\coloss.cpp", 105);
                }
                g_pBtlMenuUiTextures = (int *)GameHeapAllocRaw(blockBytes << 2,
                                                // STRING: FF7 0x007c2914
                                                "C:\\FF7\\src\\Battle\\yama\\coloss.cpp", 107);
                if (g_pBtlMenuUiTextures != 0) {
                    memcpy(g_pBtlMenuUiTextures, (void *)g_Battle_fileBuffer, blockBytes);
                }
            }
        }
        GameHeapFree((void *)g_Battle_sceneBuffer,
                     // STRING: FF7 0x007c2938
                     "C:\\FF7\\src\\Battle\\yama\\coloss.cpp", 115);
        g_Battle_sceneBuffer = 0;
        GameHeapFree((void *)g_Battle_fileBuffer,
                     // STRING: FF7 0x007c295c
                     "C:\\FF7\\src\\Battle\\yama\\coloss.cpp", 118);
        g_Battle_fileBuffer = 0;
        g_BattleSquareOpponentIndex = Battle_HighestSetBitIndex(Battle_PickRandomBitFromMask(g_BattleSquareCandidateMask));
        for (slotIndex = 0; slotIndex < 3; slotIndex++) {
            partyCharacterId = gSavemap.party[slotIndex];
            for (charSlot = 0; charSlot < 9; charSlot++) {
                if (gSavemap.chars[charSlot].id == partyCharacterId) {
                    gSavemap.chars[charSlot].hp = *(short *)&gBattleAiGlobals.combatants[slotIndex].curHp;
                    gSavemap.chars[charSlot].mp = gBattleAiGlobals.combatants[slotIndex].curMp;
                    break;
                }
            }
        }
        g_BattleScene.roundCounter++;
        if (g_BattleScene.roundCounter < 8) {
            g_BattleSquareState = BATTLE_ARENA_CHOOSER_SLOT_PENALTY;
        } else {
            g_BattleSquareState = BATTLE_ARENA_CHOOSER_COLLECT;
            g_BattleArenaChooserDone = 1;
            break;
        }
    case BATTLE_ARENA_CHOOSER_SLOT_PENALTY:
        penaltyResult = Battle_RunArenaSlotPenaltySelection();
        if (g_BattleArenaSlotPenaltyDone != 0) {
            g_BattleArenaSlotPenaltyDone = 0;
            g_BattleSquareState = BATTLE_ARENA_CHOOSER_COLLECT;
            g_BattleArenaChooserDone = 1;
            if (penaltyResult != 0) {
                result = g_BattleScene.arenaOpponents[g_BattleSquareOpponentIndex];
            } else {
                *(short *)&gBattleAiGlobals.flags |= BATTLE_OUTCOME_FLED;
            }
        }
        if (g_BattleArenaSlotSequenceActive != 0) {
            g_BattleArenaSlotSequenceActive = 0;
            g_BattleArenaChooserPending = 1;
        }
        break;
    default:
        g_BattleSquareState = BATTLE_ARENA_CHOOSER_COLLECT;
        g_BattleArenaChooserDone = 1;
    }
    return result;
}
} /* extern "C" */
