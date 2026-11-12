// src/ff7/src/battle/yama/init.cpp
// Original: C:\FF7\Src\Battle\yama\init.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "ff7/api/battle.h"
#include "lib/api/file.h"
#include "ff7/api/menu.h"
#include "lib/api/token.h"
#include "ff7/api/main.h"
#include "lib/api/movie.h"
#include "ff7/battle.h"
#include "ff7/types/battle.h"
#include "ff7/savemap.h"
#include <string.h>
#include "ff7/enums/battle.h"
#include "ff7/types/menu.h"
extern "C" {

/*
 * Clears the battle kernel load state flags.
 */
// FUNCTION: FF7 0x00401000
void __cdecl Kernel_ResetLoadState(void)
{
    g_BattleLoadState = 0;
    g_BattleResultWord = 0;
}

// clang-format off
// GLOBAL: FF7 0x007ba070
void * g_KernelBinSectionTargets[9] = {(unsigned char *)&g_KernelCommandTable[0].cursorAction, &g_BattleAttackTable, &g_BattleKernelData, gSavemap.chars, &g_MenuKernelItemBuffer, &g_KernelWeaponDataArray, (int *)g_KernelArmorDataArray[0].headerBytes, (int *)&g_KernelAccessoryDataArray[0], (int *)g_KernelMateriaDataArray[0].headerBytes};
// GLOBAL: FF7 0x0099ce08
int g_BattleLoadState = 0;
struct BattleCommandContext *g_pBattleCommandCtx = 0;
// GLOBAL: FF7 0x0099ce10
KernelBattleData g_BattleKernelData = {0};
unsigned char g_bssPad_0099dda4[4] = {0};
// GLOBAL: FF7 0x0099dda8
unsigned char g_BattleWindowData[1302] = {0};
// GLOBAL: FF7 0x0099e2c0
int g_BattleRewardExp = 0;
// GLOBAL: FF7 0x0099e2c4
int g_BattleRewardAp = 0;
// GLOBAL: FF7 0x0099e2c8
struct BattleRewardTotals g_BattleRewardTotals = {0};
// GLOBAL: FF7 0x0099e2f0
struct BattleRewardItem g_BattleRewardItems[4] = {0};
// GLOBAL: FF7 0x0099e308
struct BattleCommandContext g_BattleCommandCtxStorage = {0};
unsigned char g_bssPad_0099e708[3072] = {0};
// clang-format on

static __inline void __cdecl Kernel_LoadKernel2Bin(char *path);
static __inline void __cdecl Kernel_ParseWindowBinSections(void);

#pragma intrinsic(memcpy, strcpy, strcat)

/*
 * Loads the game's kernel data from the data directory, at start-up and again on game over
 * (both from main.cpp). It resets the kernel load state, points g_pBattleCommandCtx at its storage
 * and the text spacing table at g_BattleWindowData, and empties the kernel text pool, then:
 * - kernel/window.bin (Kernel_ParseWindowBinSections): section 1 into g_BattleWindowData; section 0 goes
 *   into a 64 KB scratch block that is freed unused;
 * - kernel/kernel.bin (Kernel_ParseKernelBin): each section by its type into the nine tables of
 *   g_KernelBinSectionTargets (commands, attacks, g_BattleKernelData, the savemap's character
 *   records, items, weapons, armour, party data, materia; a reload therefore resets the character
 *   records to the initial ones), type 9 sections into text banks;
 * - kernel/kernel2.bin (Kernel_LoadKernel2Bin): LZS-compressed, empties the text pool again and
 *   fills it with its 18 size-prefixed text banks, so these replace the kernel.bin text.
 * The file buffer is freed after each file. Always returns 1; both callers ignore it.
 */
// FUNCTION: FF7 0x00401018
int __cdecl Kernel_LoadKernelBinFiles(void)
{
    char path[256];
    int flag;
    Kernel_ResetLoadState();
    g_pBattleCommandCtx = &g_BattleCommandCtxStorage;
    g_pTextSpacing = g_BattleWindowData;
    Kernel_ResetTextPool();
    strcpy(path, g_InitPath_DataPath);
    strcat(path,
           // STRING: FF7 0x007ba0f4
           "kernel/window.bin");
    LoadFileBlock((int)path, 0, 0, (void **)&g_Battle_fileBuffer, Kernel_ParseWindowBinSections);
    GameHeapFree((void *)g_Battle_fileBuffer,
                 // STRING: FF7 0x007ba108
                 "C:\\FF7\\Src\\Battle\\yama\\init.cpp", 0xde);
    g_Battle_fileBuffer = 0;
    strcpy(path, g_InitPath_DataPath);
    strcat(path,
           // STRING: FF7 0x007ba128
           "kernel/kernel.bin");
    LoadFileBlock((int)path, 0, 0, (void **)&g_Battle_fileBuffer, Kernel_ParseKernelBin);
    GameHeapFree((void *)g_Battle_fileBuffer,
                 // STRING: FF7 0x007ba13c
                 "C:\\FF7\\Src\\Battle\\yama\\init.cpp", 0xea);
    g_Battle_fileBuffer = 0;
    strcpy(path, g_InitPath_DataPath);
    strcat(path,
           // STRING: FF7 0x007ba15c
           "kernel/kernel2.bin");
    flag = 1;
    if (flag)
        Kernel_LoadKernel2Bin(path);
    return 1;
}

/*
 * Loads and decompresses kernel2.bin, filling the battle text pool with its 18 size-prefixed banks.
 */
// FUNCTION: FF7 0x00401228
static __inline void __cdecl Kernel_LoadKernel2Bin(char *path)
{
    struct {
        void *dest;
        int size;
        char *cursor;
        int i;
        void *file;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.file = 0;
    frame.cursor = 0;
    LoadFileBlockLzs((int)path, 0x6c00, &frame.file, 0);
    frame.cursor = (char *)frame.file;
    Kernel_ResetTextPool();
    for (frame.i = 0; frame.i < 18; frame.i++) {
        frame.dest = 0;
        frame.size = *(int *)frame.cursor;
        frame.cursor += 4;
        frame.dest = Kernel_AllocTextBank(frame.size);
        memcpy(frame.dest, frame.cursor, frame.size);
        frame.cursor += frame.size;
    }
    GameHeapFree(frame.file,
                 // STRING: FF7 0x007ba094
                 "C:\\FF7\\Src\\Battle\\yama\\init.cpp", 0x57);
}

/*
 * Reads and distributes sections from kernel.bin into target memory buffers.
 */
// FUNCTION: FF7 0x004012da
void __cdecl Kernel_ParseKernelBin(void)
{
    struct {
        int result;
        int size;
        unsigned short type;
    } frame;
    BinSectionReaderInit((unsigned char *)((void *)g_Battle_fileBuffer));
    while ((frame.type = BinSectionPeekType()) != 0xffff) {
        /* Retail caller treats `BinSectionPeekSize` return (`short`) as `unsigned short` for caller zero/sign extension. */
        frame.size = ((unsigned short (__cdecl *)(void))BinSectionPeekSize)();
        if (frame.type == KERNEL_BIN_SECTION_TEXT)
            frame.result = BinSectionRead(Kernel_AllocTextBank(frame.size));
        else if (g_KernelBinSectionTargets[frame.type])
            frame.result = BinSectionRead(g_KernelBinSectionTargets[frame.type]);
    }
}

/*
 * Reads window layout sections from window.bin.
 */
// FUNCTION: FF7 0x00401372
static __inline void __cdecl Kernel_ParseWindowBinSections(void)
{
    int result;
    unsigned short type;
    g_Battle_sceneBuffer = (int)GameHeapAllocRaw(0x10000,
                                    // STRING: FF7 0x007ba0b4
                                    "C:\\FF7\\Src\\Battle\\yama\\init.cpp", 0x89);
    BinSectionReaderInit((unsigned char *)((void *)g_Battle_fileBuffer));
    while ((type = BinSectionPeekType()) != 0xffff) {
        switch (type) {
        case KERNEL_WINDOW_BIN_SECTION_DISCARDED:
            result = BinSectionRead((void *)g_Battle_sceneBuffer);
            break;
        case KERNEL_WINDOW_BIN_SECTION_WINDOW_DATA:
            result = BinSectionRead(g_BattleWindowData);
            break;
        }
    }
    GameHeapFree((void *)g_Battle_sceneBuffer,
                 // STRING: FF7 0x007ba0d4
                 "C:\\FF7\\Src\\Battle\\yama\\init.cpp", 0xad);
    g_Battle_sceneBuffer = 0;
}

/*
 * Reloads initial character record templates from kernel.bin into the savemap.
 */
// FUNCTION: FF7 0x00401422
void __cdecl Kernel_ReloadSavemapTemplate(void)
{
    char path[256];
    Kernel_ResetLoadState();
    strcpy(path, g_InitPath_DataPath);
    strcat(path,
           // STRING: FF7 0x007ba170
           "kernel/kernel.bin");
    LoadFileBlock((int)path, 0, 0, (void **)&g_Battle_fileBuffer, 0);
    BinExtractSectionByType((unsigned char *)((void *)g_Battle_fileBuffer), (unsigned char *)&gSavemap.chars[0], KERNEL_BIN_SECTION_CHARACTER_RECORDS);
    GameHeapFree((void *)g_Battle_fileBuffer,
                 // STRING: FF7 0x007ba184
                 "C:\\FF7\\Src\\Battle\\yama\\init.cpp", 0x13f);
    g_Battle_fileBuffer = 0;
}
} /* extern "C" */
