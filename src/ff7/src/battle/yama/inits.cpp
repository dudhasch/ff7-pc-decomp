// src/ff7/src/battle/yama/inits.cpp
// Original: C:\FF7\src\Battle\yama\inits.cpp
// Contents: Battle scene initialization: loads and decompresses the battle scene data across three
//   steps, finds the scene file chunk, and provides a clamped stat-scaling helper.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"
#include "lib/api/mem.h"
#include "lib/api/file.h"
#include "lib/api/movie.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/enums/main.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "lib/codegen.h"
extern "C" {

// Layout type for each scene layout.
// GLOBAL: FF7 0x007b76f0
extern const unsigned char g_BattleLayoutTypes[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x03, 0x03, 0x03, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format off
// GLOBAL: FF7 0x00c069b8
int g_BattleSceneSubIndex = 0;
// GLOBAL: FF7 0x00c069bc
int g_BattleSceneLoadStep = 0;
// clang-format on

/*
 * Loads the formation `sceneId` from scene.bin, as a three-step state machine on
 * g_BattleSceneLoadStep that the caller runs once per frame. A scene holds four formations: the
 * scene is sceneId / 4, the formation sceneId % 4.
 * - Step 0 finds the 0x2000-byte chunk of scene.bin that holds the scene (Battle_FindSceneFileChunk
 *   and the kernel's scene lookup table), reads that chunk into g_Battle_sceneBuffer from the
 *   battle data directory and keeps the scene's index within the chunk.
 * - Step 1 calls `callback`, when there is one, each time until it sets g_BattleSequencePlayDone.
 * - Step 2 inflates the scene's gzip entry (GzipInflate_BinSection), frees the chunk and copies the
 *   blocks out: the 8-byte header into g_BattleScene; the formation's 0x14-byte setup record, 0x30
 *   bytes of formation camera and 0x60-byte enemy placement; the three enemy records, the 32
 *   attack records, the 32 attack ids and three more blocks of 0x400, 0x200 and 0x1000 bytes.
 * Then it adjusts the setup by the field's battle flags (g_BattleScene.flags, FieldBattleFlag):
 * FIELD_BATTLE_FLAG_PREEMPTIVE with setup flag 0x10 turns a normal layout into a pre-emptive strike.
 * The layout picks g_BattleState.layoutType from g_BattleLayoutTypes. FIELD_BATTLE_FLAG_BATTLE_SQUARE sets
 * setup flag 4, location 0x25 and a random value 0x60..0x63 in `cameraScriptIndex`, scene mode 1, doubles each
 * enemy's HP and scales its strength and magic by 1.25 (Battle_ScaleStatBy125Clamped); otherwise
 * flag 8 clears setup flag 4. Without setup flag 4 it sets battle flag 8. Scene mode comes from
 * the setup, forced to 1 for the pre-emptive and side-attack layout types. Last it resets the step and sets g_BattleSceneRecordLoaded
 * to report the load done; an unknown step only does that.
 */
// FUNCTION: FF7 0x005d1050
void __cdecl Battle_LoadSceneRecord(int sceneId, void *callback)
{
    struct {
        int directory[51];
        int enemy;
        unsigned char scene[7808];
        int compressed;
        int chunk;
        int inflatedSize;
        int sceneIndex;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    switch (g_BattleSceneLoadStep) {
    case BATTLE_SCENE_LOAD_READ_CHUNK:
        frame.sceneIndex = sceneId / 4;
        frame.chunk = Battle_FindSceneFileChunk(frame.sceneIndex);
        Battle_PushDataDirectory(&frame.directory[0]);
        // STRING: FF7 0x008ff038
        LoadFileBlock((int)"scene.bin", frame.chunk << 0xD, 0x2000, (void **)&g_Battle_sceneBuffer, 0);
        Battle_PopDataDirectory(&frame.directory[0]);
        g_BattleSceneSubIndex = frame.sceneIndex - g_BattleKernelData.sceneLookup[frame.chunk];
        g_BattleSceneLoadStep = BATTLE_SCENE_LOAD_WAIT_CALLBACK;
    case BATTLE_SCENE_LOAD_WAIT_CALLBACK:
        if (callback != 0) {
            /* Invokes untyped callback slot `callback` with its runtime (void) -> void signature. */
            ((void(__cdecl *)(void))callback)();
            if (g_BattleSequencePlayDone != 0) {
                g_BattleSequencePlayDone = 0;
                g_BattleSceneLoadStep = BATTLE_SCENE_LOAD_INFLATE;
            }
            return;
        }
        g_BattleSceneLoadStep = BATTLE_SCENE_LOAD_INFLATE;
    case BATTLE_SCENE_LOAD_INFLATE:
        frame.compressed = g_Battle_sceneBuffer;
        g_BattleSceneSubIndex = *(int *)((char *)frame.compressed + g_BattleSceneSubIndex * 4);
        frame.inflatedSize = GzipInflate_BinSection((void *)(frame.compressed + g_BattleSceneSubIndex * 4), &frame.scene[0]);
        // STRING: FF7 0x008ff044
        GameHeapFree((void *)g_Battle_sceneBuffer, "C:\\FF7\\src\\Battle\\yama\\inits.cpp", 0xAB);
        g_Battle_sceneBuffer = 0;
        g_BattleSceneSubIndex = sceneId % 4;
        Battle_CopySceneDataBlock(&g_BattleScene, &frame.scene[0], 8);
        Battle_CopySceneDataBlock(&g_BattleScene.locationId, &frame.scene[8 + g_BattleSceneSubIndex * 0x14], 0x14);
        Battle_CopySceneDataBlock(g_BattleScene.formationCamera, &frame.scene[88 + g_BattleSceneSubIndex * 0x30], 0x30);
        Battle_CopySceneDataBlock((int *)&g_BattleScene.enemies[0], &frame.scene[280 + g_BattleSceneSubIndex * 0x60], 0x60);
        Battle_CopySceneDataBlock((unsigned char *)g_BattleState.enemyRecords, &frame.scene[664], 0x228);
        Battle_CopySceneDataBlock((unsigned char *)g_BattleState.attackEntries, &frame.scene[1216], 0x380);
        Battle_CopySceneDataBlock(g_BattleState.attackIds, &frame.scene[2112], 0x40);
        Battle_CopySceneDataBlock(g_BattleState.sceneRecordBlockD, &frame.scene[2176], 0x400);
        Battle_CopySceneDataBlock(g_BattleState.sceneRecordBlockE, &frame.scene[3200], 0x200);
        Battle_CopySceneDataBlock(&g_BattleState.sceneRecordBlockE[0x200], &frame.scene[3712], 0x1000);
        if ((g_BattleScene.flags & FIELD_BATTLE_FLAG_PREEMPTIVE) != 0 && (g_BattleScene.setupFlags & 0x10) != 0 && g_BattleScene.layout == BATTLE_LAYOUT_NORMAL) {
            g_BattleScene.layout = BATTLE_LAYOUT_PREEMPTIVE;
        }
        g_BattleState.layoutType = g_BattleLayoutTypes[g_BattleScene.layout];
        if ((g_BattleScene.flags & FIELD_BATTLE_FLAG_BATTLE_SQUARE) != 0) {
            g_BattleScene.setupFlags |= 4;
            g_BattleScene.locationId = 0x25;
            /* Retail caller treats `Battle_NextRandomByte` return (`unsigned char`) as `int` for caller zero/sign extension. */
            g_BattleScene.cameraScriptIndex = (((int (__cdecl *)(void))Battle_NextRandomByte)() & 0xFF & 3) + 0x60;
            g_BattleScene.setupSceneMode = 1;
            for (frame.enemy = 0; frame.enemy < 3; frame.enemy++) {
                g_BattleState.enemyRecords[frame.enemy].hp <<= 1;
                g_BattleState.enemyRecords[frame.enemy].strength = Battle_ScaleStatBy125Clamped(*(unsigned char *)&g_BattleState.enemyRecords[frame.enemy].strength);
                g_BattleState.enemyRecords[frame.enemy].magic = Battle_ScaleStatBy125Clamped(*(unsigned char *)&g_BattleState.enemyRecords[frame.enemy].magic);
            }
        } else if ((g_BattleScene.flags & 8) != 0) {
            g_BattleScene.setupFlags &= 0xFFFB;
        }
        if ((g_BattleScene.setupFlags & 4) == 0) {
            g_BattleScene.flags |= 8;
        }
        g_BattleState.sceneMode = g_BattleScene.setupSceneMode;
        if (g_BattleState.layoutType == BATTLE_LAYOUT_PREEMPTIVE || g_BattleState.layoutType == BATTLE_LAYOUT_SIDE_ATTACK_1) {
            g_BattleState.sceneMode = 1;
        }
        g_BattleState.scenePrevMode = g_BattleState.sceneMode;
        g_BattleSceneLoadStep = BATTLE_SCENE_LOAD_READ_CHUNK;
        g_BattleSceneRecordLoaded = 1;
        break;
    default:
        g_BattleSceneLoadStep = BATTLE_SCENE_LOAD_READ_CHUNK;
        g_BattleSceneRecordLoaded = 1;
        break;
    }
}

/*
 * Scans chunk index table until blockIndex is less than entry and returns preceding index.
 */
// FUNCTION: FF7 0x005d14a9
int __cdecl Battle_FindSceneFileChunk(int blockIndex)
{
    int result;
    unsigned int i;

    for (i = 1; i < 0x40; i++) {
        if (blockIndex < g_BattleKernelData.sceneLookup[i]) {
            break;
        }
    }
    result = i - 1;
    return result;
}

/*
 * Scales a stat by 125% (x 125 / 100) and clamps the result to 255.
 */
// FUNCTION: FF7 0x005d14eb
int __cdecl Battle_ScaleStatBy125Clamped(int stat)
{
    /* The parameter doubles as the scratch variable; a separate local would change the frame. */
    stat = stat * 0x7D / 0x64;
    if (stat > 0xFF) {
        stat = 0xFF;
    }
    return stat;
}
} /* extern "C" */
