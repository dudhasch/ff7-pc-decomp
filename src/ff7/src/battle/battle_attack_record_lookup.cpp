// src/ff7/src/battle/battle_attack_record_lookup.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Looks up the 28-byte battle attack record for an attack id, subject to a range check on
//   the id.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"

#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "lib/codegen.h"
#include "ff7/types/battle.h"
#include "lib/api/token.h"
extern "C" {

/*
 * Returns the 28-byte attack record indexed by attackId when its signed interpretation is below
 * 256; otherwise searches the 32 scene attack ids in g_BattleState.attackIds and returns the matching
 * 28-byte record from g_BattleState.attackEntries, or null when none matches. IDs with the high bit
 * set also enter the direct-index branch.
 */
// FUNCTION: FF7 0x005df080
unsigned char *__cdecl Battle_FindAttackRecord(unsigned int attackId)
{
    unsigned char *record;
    int index;
    record = 0;
    if ((int)attackId < 0x100) {
        record = &g_BattleAttackTable[attackId].accuracy;
    } else {
        for (index = 0; index < 32; index++) {
            if (g_BattleState.attackIds[index] == attackId) {
                record = (unsigned char *)&g_BattleState.attackEntries[index * 0x1C];
                break;
            }
        }
    }
    return record;
}
} /* extern "C" */
