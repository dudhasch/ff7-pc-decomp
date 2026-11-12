// src/lib/src/sound/sound_soundfont_open_close.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Opens and closes a SoundFont MIDI device, saving the banks and configuration on open
//   and restoring them on close, and loads or clears a bank or a preset.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/sound.h"
#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include "lib/api/sound.h"
#include <stdio.h>
extern "C" {

#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x00de6610
int g_SoundFontDeviceIsOpen = 0;
// GLOBAL: FF7 0x00de6614
unsigned short g_SoundFontDeviceIndex = 0;
// clang-format on
#pragma intrinsic(strcpy, strcat, memset, strcmp)

/*
 * Bind SFManager, open the requested MIDI device (or the best device), and snapshot its banks/configuration.
 */
// FUNCTION: FF7 0x00749c50
unsigned int __cdecl SoundFontOpenDevice(int midi)
{
    // Names preserve the original VC5 stack slots.
    unsigned int freeBytes, totalBytes; // Free RAM and total RAM.
    int capResult; // Device-selection capability result.
    unsigned short devIndex; // SDK device index.
    int devCnt; // SDK device count.

    if (SoundFontBindSfman32()) {
        g_SoundFontDeviceIndex = 0xffff;
        if (midi == -1) {
            g_SoundFontDeviceIndex = SoundFontPickBestDevice(&totalBytes, &capResult);
        } else {
            devCnt = SfManGetDeviceCount();
            if (devCnt) {
                for (devIndex = 0; devIndex < devCnt; devIndex++) {
                    if (midi == SoundFontFindMidiOutIndex(devIndex)) {
                        g_SoundFontDeviceIndex = devIndex;
                    }
                }
            }
        }
        if (g_SoundFontDeviceIndex != 0xffff) {
            g_SoundFontDeviceIsOpen = !SfManOpenDevice(g_SoundFontDeviceIndex);
            if (g_SoundFontDeviceIsOpen) {
                SoundFontSaveLoadedBanks(g_SoundFontDeviceIndex);
                SfManSaveSynthConfig(g_SoundFontDeviceIndex);
                SfManClearAllBanks(g_SoundFontDeviceIndex);
                SfManGetSampleRamSize(g_SoundFontDeviceIndex, &totalBytes, &freeBytes);
                return totalBytes;
            }
        }
        SoundFontUnbindSfman32(g_SoundFontDeviceIndex);
    }
    return 0;
}

/*
 * Clear the game banks, restore the saved configuration and banks, then close and unload SFManager.
 */
// FUNCTION: FF7 0x00749d6c
void __cdecl SoundFontCloseDevice(void)
{
    if (g_SoundFontDeviceIsOpen != 0) {
        SfManClearAllBanks((short)g_SoundFontDeviceIndex);
        SfManRestoreSynthConfig((short)g_SoundFontDeviceIndex);
        SoundFontRestoreLoadedBanks((short)g_SoundFontDeviceIndex);
        SfManCloseDevice((short)g_SoundFontDeviceIndex);
        SoundFontUnbindSfman32((short)g_SoundFontDeviceIndex);
    }
}

/*
 * Clear the device banks and load the supplied file into bank 1; report SDK failures to the debugger.
 */
// FUNCTION: FF7 0x00749dc8
int __cdecl SoundFontLoadBank(char *path)
{
    int result;
    if (g_SoundFontDeviceIsOpen && path) {
        SfManClearAllBanks(g_SoundFontDeviceIndex);
        /* The device index is zero-extended to a full word, as in SoundFontRestoreLoadedBanks. */
        result = ((int(__cdecl *)(unsigned int, unsigned short, char *))SfManLoadBankFromFile)(g_SoundFontDeviceIndex, 1, path);
        if (!result) {
            return 1;
        }
        OutputDebugStringA(SfManErrorText(result));
    }
    return 0;
}

/*
 * Copy one preset between SDK bank 0x8000 references; report SDK failures to the debugger.
 */
// FUNCTION: FF7 0x00749e2b
int __cdecl SoundFontLoadPreset(char *path, unsigned short source, unsigned short destination)
{
    int result;
    if (g_SoundFontDeviceIsOpen && path) {
        result = SfManCopyPresetFromFile(g_SoundFontDeviceIndex, 0x8000, source, 0x8000, destination, path);
        if (!result) {
            return 1;
        }
        OutputDebugStringA(SfManErrorText(result));
    }
    return 0;
}

/*
 * Clear the SDK preset and translate a zero SDK result into game success.
 */
// FUNCTION: FF7 0x00749e8e
int __cdecl SoundFontClearPreset(void)
{
    int result;
    if (g_SoundFontDeviceIsOpen) {
        result = SfManClearPreset(g_SoundFontDeviceIndex);
        if (!result) {
            return 1;
        }
        OutputDebugStringA(SfManErrorText(result));
    }
    return 0;
}
} /* extern "C" */
