// src/ff7/src/battle/battle_actor_buffer_pointers.cpp
// Data-only object: .data 0x008fe2ac..0x008fe2b8. No code.
// A data-only member of the battle library, which its neighbour battle_actor_effect_sprites.cpp never
// references: the battle actor animation buffer and effect primitive pointers.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/file.h"
#include "lib/api/movie.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// The head of the pointer table {g_BattleActorAnimStateBuffer, 0xC06228, 0xC06328}
// (lasboss3.cpp and battle_actor_anim_effect_tasks.cpp read it).
// GLOBAL: FF7 0x008fe2ac
struct BattleActorAnimState *g_BattleActor_BufferPtr = (struct BattleActorAnimState *)g_BattleActorAnimStateBuffer;
// GLOBAL: FF7 0x008fe2b0
int g_BattleActor_Offset00c06228 = 0x00C06228;
// GLOBAL: FF7 0x008fe2b4
struct BattleEffectPrimitive *g_BattleActor_Offset00c06328 = (struct BattleEffectPrimitive *)12608296;
// clang-format on

} /* extern "C" */
