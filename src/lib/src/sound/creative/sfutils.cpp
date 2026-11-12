// src/lib/src/sound/creative/sfutils.cpp
// Original: C:\lib\src\sound\creative\sfutils.cpp
// Contents: SoundFont manager interface: binds the SFMAN32 module, picks and opens a MIDI device,
//   saves and restores its synthesizer configuration and loaded banks, and loads or copies banks
//   and presets.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include <windows.h>
#include <string.h>
#include "lib/crt.h"
#include "lib/types/sound.h"
#include "lib/api/sound.h"
#include <stdio.h>
extern "C" {

#pragma intrinsic(memset)
// clang-format off
// GLOBAL: FF7 0x00de6774
HMODULE g_SfManagerModule = {0};
// GLOBAL: FF7 0x00de6778
unsigned short g_SfDeviceCount = 0;
// GLOBAL: FF7 0x00de677c
unsigned short g_SfSynthConfig = 0;
unsigned char g_bssPad_00de6780[4] = {0};
// GLOBAL: FF7 0x00de6784
int * g_SfUtilsManagerApi = 0;
unsigned char g_bssPad_00de6788[4] = {0};
// clang-format on
// The SoundFont manager's entry point, loaded by SoundFontBindSfman32.
// GLOBAL: FF7 0x00de678c
SfManagerProc *g_sfManager = 0;

/*
 * Loads SFMAN32.DLL and dereferences its exported SFManager entry to obtain interface version 0x10001.
 */
// FUNCTION: FF7 0x0074ade0
int __cdecl SoundFontBindSfman32(void)
{
    int result;
    // STRING: FF7 0x00969198
    g_SfManagerModule = LoadLibraryA("SFMAN32.DLL");
    if (!g_SfManagerModule) {
        return 0;
    }
    // STRING: FF7 0x009691a4
    g_sfManager = (SfManagerProc *)GetProcAddress(g_SfManagerModule, "SFManager");
    if (!g_sfManager) {
        FreeLibrary(g_SfManagerModule);
        return 0;
    }
    result = (*g_sfManager)(0x10001, &g_SfUtilsManagerApi);
    if (!g_SfUtilsManagerApi) {
        FreeLibrary(g_SfManagerModule);
        return 0;
    }
    return 1;
}

/*
 * Probes the best device, maps it to WinMM, and snapshots its banks and synthesizer configuration.
 */
// FUNCTION: FF7 0x0074ae6b
unsigned short __cdecl SoundFontProbeDevice(unsigned int *ram, unsigned short *midi, int *hardware)
{
    unsigned short device;
    if (SoundFontBindSfman32()) {
        device = SoundFontPickBestDevice(ram, hardware);
        if (device == 0xffff) {
            return device;
        }
        *midi = SoundFontFindMidiOutIndex(device);
        SfManOpenDevice(device);
        SoundFontSaveLoadedBanks(device);
        SfManSaveSynthConfig(device);
        SfManClearAllBanks(device);
        SfManCloseDevice(device);
        return device;
    }
    return 0xffff;
}

/*
 * Queries the synthesizer configuration and retains its word identifier for restoration.
 */
// FUNCTION: FF7 0x0074af09
int __cdecl SfManSaveSynthConfig(unsigned short device)
{
    int result;
    char q[256];
    struct {
        int size;
        char *buffer;
        int unused;
        unsigned short config;
    } request;
    request.size = 255;
    request.buffer = q;
    /* Invokes untyped callback slot `g_SfUtilsManagerApi[9]` with its runtime (unsigned short, void *) -> int signature. */
    result = ((int(__cdecl *)(unsigned short, void *))g_SfUtilsManagerApi[9])(device, &request);
    g_SfSynthConfig = request.config;
    return result;
}

/*
 * Restores the previously saved synthesizer configuration identifier.
 */
// FUNCTION: FF7 0x0074af57
int __cdecl SfManRestoreSynthConfig(unsigned short device)
{
    int result;
    /* Invokes untyped callback slot `g_SfUtilsManagerApi[10]` with its runtime (unsigned short, unsigned short) -> int signature. */
    result = ((int(__cdecl *)(unsigned short, unsigned short))g_SfUtilsManagerApi[10])(device, g_SfSynthConfig);
    return result;
}

/*
 * Opens an SDK device using the word-width device argument.
 */
// FUNCTION: FF7 0x0074af7d
int __cdecl SfManOpenDevice(unsigned short device)
{
    /* Invokes untyped callback slot `g_SfUtilsManagerApi[4]` with its runtime (unsigned short) -> int signature. */
    return ((int(__cdecl *)(unsigned short))g_SfUtilsManagerApi[4])(device);
}

/*
 * Closes an SDK device using the word-width device argument.
 */
// FUNCTION: FF7 0x0074af93
int __cdecl SfManCloseDevice(unsigned short device)
{
    /* Invokes untyped callback slot `g_SfUtilsManagerApi[5]` with its runtime (unsigned short) -> int signature. */
    return ((int(__cdecl *)(unsigned short))g_SfUtilsManagerApi[5])(device);
}

/*
 * Releases the loaded SDK module; the device parameter is unused.
 */
// FUNCTION: FF7 0x0074afa9
void __cdecl SoundFontUnbindSfman32(unsigned short device)
{
    FreeLibrary(g_SfManagerModule);
}

/*
 * Scores SDK devices by capability bits and sample RAM, then tries opening them in score order.
 */
// FUNCTION: FF7 0x0074afba
unsigned short __cdecl SoundFontPickBestDevice(unsigned int *ram, int *hardware)
{
    // Names preserve VC5's local-variable bucket order and the original frame.
    int enumResult; // SDK enumeration result (unused).
    unsigned int softwareScore8, hardwareScore; // Fixed scores for the two capability classes.
    unsigned short attempts; // Remaining open attempts.
    unsigned int sumRam, minScore, maxRam, freeBytes; // Total RAM, special score, largest RAM, free RAM.
    unsigned short bestDevice; // Best device.
    unsigned int pointBytes; // RAM bytes per score point.
    unsigned short iterDevice; // Device being scored.
    unsigned char capsBuf[102]; // Packed SDK device capabilities, not an SF2 record.
    unsigned int scores[8]; // Device scores (the original has no bounds check).

    softwareScore8 = 128;
    hardwareScore = 64;
    minScore = 32;
    pointBytes = 0x80000;
    /* Invokes untyped callback slot `g_SfUtilsManagerApi[0]` with its runtime (unsigned short *) -> int signature. */
    enumResult = ((int(__cdecl *)(unsigned short *))g_SfUtilsManagerApi[0])(&g_SfDeviceCount);
    if (!g_SfDeviceCount) {
        return 0xffff;
    }
    maxRam = 0;
    bestDevice = 0xffff;
    for (iterDevice = 0; iterDevice < g_SfDeviceCount; iterDevice++) {
        scores[iterDevice] = 0;
        memset(capsBuf, 0, 102);
        *(unsigned short *)capsBuf = 102;
        ((int(__cdecl *)(unsigned short, void *))g_SfUtilsManagerApi[1])(iterDevice, capsBuf);
        if (!(*(unsigned int *)(capsBuf + 6) & 0x40000000)) {
            if (!(*(unsigned int *)(capsBuf + 6) & 0x80000000)) {
                if (SfManOpenDevice(iterDevice)) {
                    continue;
                }
                /* Invokes untyped callback slot `g_SfUtilsManagerApi[7]` with its runtime (unsigned short, unsigned int *, unsigned int *) -> int signature. */
                ((int(__cdecl *)(unsigned short, unsigned int *, unsigned int *))g_SfUtilsManagerApi[7])(iterDevice, &sumRam, &freeBytes);
                if (sumRam > maxRam) {
                    maxRam = sumRam;
                }
                scores[iterDevice] = (sumRam / pointBytes) & 0xff;
                scores[iterDevice] += hardwareScore;
                if (scores[iterDevice] == hardwareScore) {
                    scores[iterDevice] = 0;
                }
                SfManCloseDevice(iterDevice);
            } else {
                scores[iterDevice] = softwareScore8 & 0xff;
            }
        } else {
            scores[iterDevice] = 0;
        }
    }
    attempts = g_SfDeviceCount;
    while (attempts > 0) {
        bestDevice = 0;
        attempts--;
        for (iterDevice = 0; iterDevice < g_SfDeviceCount; iterDevice++) {
            if (scores[iterDevice] > scores[bestDevice]) {
                bestDevice = iterDevice;
            }
        }
        if (SfManOpenDevice(bestDevice)) {
            scores[bestDevice] = 0;
        } else {
            attempts = 0;
        }
    }
    if (minScore == scores[bestDevice]) {
        *hardware = 0;
        *ram = 0;
    } else {
        *hardware = 1;
        *ram = maxRam;
    }
    SfManCloseDevice(bestDevice);
    return bestDevice;
}
// These are SFManager SDK records, not the SoundFont RIFF records in SFSPEC21.
#pragma intrinsic(memset)

typedef struct SavedSfBank {
    unsigned short bank;
    unsigned short reserved;
    char filename[0x410];
    struct SavedSfBank *next;
} SavedSfBank;

// GLOBAL: FF7 0x00de6790
SavedSfBank *g_savedSfBanks = 0;
// clang-format off
unsigned char g_bssPad_00de6794[4] = {0};
// clang-format on

// This TU passes the device as a full dword; the bank remains a word.

/*
 * Snapshots banks 1..127 while preserving allocation-failure teardown order.
 */
// FUNCTION: FF7 0x0074b275
int __cdecl SoundFontSaveLoadedBanks(unsigned short device)
{
    // Names preserve the original VC5 stack slots.
    char *fileName; // Filename within the new node.
    SavedSfBank *tail; // Tail of the saved list.
    unsigned short bankOut; // Bank returned by the SDK.
    SavedSfBank *nodePtr; // Newly allocated node / teardown cursor.
    unsigned short bank; // Bank being queried.

    tail = 0;
    for (bank = 1; bank < 128; bank++) {
        bankOut = bank;
        /* Invokes untyped callback slot `g_SfUtilsManagerApi[6]` with its runtime (unsigned short, unsigned short *) -> int signature. */
        if (((int(__cdecl *)(unsigned short, unsigned short *))g_SfUtilsManagerApi[6])(device, &bankOut) != -1 && bank == bankOut) {
            // STRING: FF7 0x009691b0
            nodePtr = (SavedSfBank *)GameHeapAllocRaw(sizeof(SavedSfBank), "C:\\lib\\src\\sound\\creative\\sfutils.cpp", 313);
            if (!nodePtr) {
                nodePtr = g_savedSfBanks;
                while (nodePtr) {
                    // STRING: FF7 0x009691d8
                    GameHeapFree(nodePtr, "C:\\lib\\src\\sound\\creative\\sfutils.cpp", 321);
                    nodePtr = nodePtr->next;
                }
                return 0;
            }
            nodePtr->next = 0;
            nodePtr->bank = bankOut;
            fileName = nodePtr->filename;
            SfManGetBankFileName(device, bankOut, fileName);
            if (!g_savedSfBanks) {
                g_savedSfBanks = nodePtr;
            }
            if (tail) {
                tail->next = nodePtr;
            }
            tail = nodePtr;
        }
    }
    return 1;
}

/*
 * Clears the default bank, then banks 1..127, stopping on the first SDK error.
 */
// FUNCTION: FF7 0x0074b3af
int __cdecl SfManClearAllBanks(unsigned short device)
{
    struct {
        unsigned short bank, preset;
    } args;
    int res;
    unsigned short bankIndex;

    memset(&args, 0, sizeof(args));
    args.bank = 0;
    /* Invokes untyped callback slot `g_SfUtilsManagerApi[14]` with its runtime (unsigned short, void *) -> int signature. */
    res = ((int(__cdecl *)(unsigned short, void *))g_SfUtilsManagerApi[14])(device, &args);
    if (res) {
        return res;
    }
    bankIndex = 1;
    while (bankIndex < 128) {
        args.bank = bankIndex;
        bankIndex++;
        /* Invokes untyped callback slot `g_SfUtilsManagerApi[14]` with its runtime (unsigned short, void *) -> int signature. */
        res = ((int(__cdecl *)(unsigned short, void *))g_SfUtilsManagerApi[14])(device, &args);
        if (res) {
            return res;
        }
    }
    return res;
}

/*
 * Reloads saved bank filenames and releases each successfully restored node.
 */
// FUNCTION: FF7 0x0074b438
int __cdecl SoundFontRestoreLoadedBanks(unsigned short device)
{
    SavedSfBank *next; // Next node, saved before freeing the current one.
    SavedSfBank *nodePtr; // Current node.

    if (!g_savedSfBanks) {
        return 0;
    }
    nodePtr = g_savedSfBanks;
    while (nodePtr) {
        /* Retail caller prototype (unsigned int, unsigned short, char *) -> int preserved for /Od argument promotion on `SfManLoadBankFromFile`. */
        if (((int(__cdecl *)(unsigned int, unsigned short, char *))SfManLoadBankFromFile)(device, nodePtr->bank, nodePtr->filename)) {
            return 0;
        }
        next = nodePtr->next;
        // STRING: FF7 0x00969200
        GameHeapFree(nodePtr, "C:\\lib\\src\\sound\\creative\\sfutils.cpp", 405);
        nodePtr = next;
    }
    return 1;
}
#pragma intrinsic(memset, strcpy, strcat, strcmp)


__declspec(dllimport) void __stdcall OutputDebugStringA(const char *text);

/*
 * Prepares file and bank descriptors, then calls the SDK load-bank function.
 */
// FUNCTION: FF7 0x0074b4b4
int __cdecl SfManLoadBankFromFile(unsigned short device, unsigned short bankIndex, char *path)
{
    SfManFile file;
    SfManBank bank;

    file.size = 128;
    file.reserved2 = 0;
    file.path = path;
    memset(&bank, 0, sizeof(bank));
    bank.bank = bankIndex;
    return ((SfManLoadBankProc *)g_SfUtilsManagerApi)[0x2c / 4](device, &bank, &file);
}

/*
 * Copies preset from file using the SDK copyPresetFromFile method.
 */
// FUNCTION: FF7 0x0074b4f6
int __cdecl SfManCopyPresetFromFile(unsigned short device, unsigned short sourceBank, unsigned short sourcePreset,
                                    unsigned short destinationBank, unsigned short destinationPreset, char *path)
{
    SfManPresetRef src;
    SfManPresetRef dst;
    SfManFileRequest request;
    char filename[516];
    int result;

    src.bank = sourceBank;
    src.preset = sourcePreset;
    dst.bank = destinationBank;
    dst.preset = destinationPreset;
    request.size = 516;
    request.zero = 0;
    request.path = filename;
    strcpy(request.path, path);
    result = ((SfManApi *)g_SfUtilsManagerApi)->copyPresetFromFile(device, &src, &dst, &request);
    return result;
}

/*
 * Attempts to copy presets 0..9 in bank 0x8000 via SDK slot 15.
 */
// FUNCTION: FF7 0x0074b5a4
int __cdecl SfManCopyPresetRange(unsigned short dev, int unused, char *path)
{
    struct {
        int size;
        char *path;
        int unknown;
        unsigned short zero;
    } copyRequest;
    struct {
        unsigned short bank;
        unsigned short preset;
    } srcPreset, destPreset;
    int result;

    copyRequest.size = 128;
    copyRequest.zero = 0;
    copyRequest.path = path;
    memset(&srcPreset, 0, sizeof(srcPreset));
    memset(&destPreset, 0, sizeof(destPreset));
    srcPreset.bank = 0x8000;
    destPreset.bank = 0x8000;
    srcPreset.preset = 0;
    for (; srcPreset.preset < 10; srcPreset.preset++) {
        destPreset.preset = srcPreset.preset;
        /* Invokes untyped callback slot `((int *)g_SfUtilsManagerApi)[15]` with its runtime (unsigned short, void *, void *, void *) -> int signature. */
        result = ((int(__cdecl *)(unsigned short, void *, void *, void *))((int *)g_SfUtilsManagerApi)[15])(
            dev, &srcPreset, &destPreset, &copyRequest);
        if (result == 0) {
            return result;
        }
    }
    return result;
}

/*
 * Prepares alternate file request and invokes SDK slot 11.
 */
// FUNCTION: FF7 0x0074b631
int __cdecl SfManLoadBankFromFileAlt(unsigned short device, int fileOffset, int unused, unsigned short bank)
{
    int result;
    struct {
        int f0;
        int f4;
        int f8;
        unsigned short fc;
        unsigned short fe;
    } fileRequest;
    int bankWord;

    fileRequest.f0 = 0x80;
    fileRequest.fc = 0;
    fileRequest.f4 = fileOffset;
    memset(&bankWord, 0, sizeof(bankWord));
    *(unsigned short *)&bankWord = bank;
    return result = ((int(__cdecl *)(unsigned short, int *, int *))((int *)g_SfUtilsManagerApi)[11])(
               device, (int *)&bankWord, (int *)&fileRequest);
}

/*
 * Queries the bank filename from SDK slot 0x34.
 */
// FUNCTION: FF7 0x0074b679
int __cdecl SfManGetBankFileName(unsigned short device, unsigned short bank, char *name)
{
    int res;
    SfManPresetRef bankRef;
    SfManNameRequest info;

    if (bank == 0)
        return 0;
    memset(&info, 0, sizeof(info));
    memset(&bankRef, 0, sizeof(bankRef));
    info.path = name;
    info.size = 128;
    bankRef.bank = bank;
    res = ((int(__cdecl *)(unsigned short, SfManPresetRef *, SfManNameRequest *))g_SfUtilsManagerApi[0x34 / 4])(device, &bankRef, &info);
    OutputDebugStringA(SfManErrorText(res));
    return res;
}

/*
 * Queries total and free sample RAM via SDK slot 7.
 */
// FUNCTION: FF7 0x0074b6ee
int __cdecl SfManGetSampleRamSize(unsigned short dev, unsigned int *total, unsigned int *free)
{
    unsigned char caps[102];

    memset(caps, 0, 0x66);

    if (*(unsigned int *)(caps + 6) & 0x80000000) {
        return 0;
    }

    /* Invokes untyped callback slot `((int *)g_SfUtilsManagerApi)[7]` with its runtime (unsigned short, unsigned int *, unsigned int *) -> int signature. */
    ((int(__cdecl *)(unsigned short, unsigned int *, unsigned int *))((int *)g_SfUtilsManagerApi)[7])(
        dev, total, free);
    if (*free == *total) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Scans WinMM MIDI output devices for a matching SoundFont device name.
 */
// FUNCTION: FF7 0x0074b746
int __cdecl SoundFontFindMidiOutIndex(unsigned short device)
{
    int i;
    unsigned int numDevs;
    char name[52];
    char card[104];

    /* Invokes untyped callback slot `((int *)g_SfUtilsManagerApi)[1]` with its runtime (unsigned short, unsigned char *) -> int signature. */
    if (((int(__cdecl *)(unsigned short, unsigned char *))((int *)g_SfUtilsManagerApi)[1])(
            device, (unsigned char *)card) != 0) {
        return 0xffff;
    }

    numDevs = midiOutGetNumDevs();
    for (i = 0; i < numDevs; i++) {
        if (midiOutGetDevCapsA(i, (MIDIOUTCAPSA *)name, 0x34) != 0) {
            return 0xffff;
        }

        if (strcmp(name + 8, card + 0x16) == 0) {
            return i;
        }
    }

    return 0xffff;
}

/*
 * Queries device count via SDK slot 0.
 */
// FUNCTION: FF7 0x0074b860
int __cdecl SfManGetDeviceCount(void)
{
    int count;

    ((void(__cdecl **)(int *))g_SfUtilsManagerApi)[0](&count);
    return count & 0xffff;
}

/*
 * Clears preset 0 in bank 0x8000 via SDK slot 0x44.
 */
// FUNCTION: FF7 0x0074b87f
int __cdecl SfManClearPreset(unsigned short device)
{
    int result;
    SfManPresetRef presetRef;

    memset(&presetRef, 0, sizeof(presetRef));
    presetRef.bank = 0x8000;
    presetRef.preset = 0;
    result = ((int(__cdecl *)(unsigned short, SfManPresetRef *))g_SfUtilsManagerApi[0x44 / 4])(device, &presetRef);
    return result;
}
} /* extern "C" */
