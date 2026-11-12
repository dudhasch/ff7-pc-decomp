// src/ff7/src/main/main_registry_key_access.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Opens and closes the machine-wide game registry key and reads and writes the sound, MIDI,
//   DirectDraw device and option values under it.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/main.h"
#include "lib/types/graphics.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#include "lib/codegen.h"
#include "ff7/api/main.h"
#include "ff7/api/menu.h"
#include "ff7/savemap.h"
#include "lib/api/graphics.h"
#include "lib/enums/graphics.h"
extern "C" {

#pragma intrinsic(strcpy, strcat, strlen)

// clang-format off
unsigned char g_bssPad_009a06e8[4] = {0};
// GLOBAL: FF7 0x009a06ec
HKEY g_Reg_AppKey = 0;
// clang-format on

int __cdecl RegQuerySoundOptionsValue(HKEY key, void *value);
int __cdecl RegQuerySoundGuidValue(HKEY key, unsigned char *value);
int __cdecl RegQueryMidiDeviceIdValue(HKEY key, void *value);
int __cdecl RegQueryMidiOptionsValue(HKEY key, void *value);
int __cdecl RegWriteSoundGuidValue(HKEY key, void *value);
int __cdecl RegWriteMidiDeviceIdValue(HKEY key, void *value);
int __cdecl RegQueryMidiDataValue(HKEY key, char *value);
int __cdecl RegWriteMidiDataValue(HKEY key, char *value);
int __cdecl RegWriteSoundOptionsValue(HKEY key, void *value);
int __cdecl RegWriteMidiOptionsValue(HKEY key, void *value);

// GLOBAL: FF7 0x007b6730
static const char REG_APP_KEY[] = "Software\\Square Soft, Inc.\\Final Fantasy VII";
// GLOBAL: FF7 0x007b6760
static const char REG_MIDI_DEVICEID[] = "MIDI_DeviceID";
// GLOBAL: FF7 0x007b6770
static const char REG_SOUND_GUID[] = "Sound_GUID";
// GLOBAL: FF7 0x007b6780
static const char REG_OPTIONS[] = "Options";
// GLOBAL: FF7 0x007b6788
static const char REG_DD_GUID[] = "DD_GUID";
// GLOBAL: FF7 0x007b6790
static const char REG_MIDI_DATA[] = "MIDI_data";
// GLOBAL: FF7 0x007b67a0
static const char REG_OPTIONS_2[] = "Options";
// GLOBAL: FF7 0x007b67a8
static const char REG_SOUND_KEY[] = "Software\\Square Soft, Inc.\\Final Fantasy VII\\1.00\\Sound";
// GLOBAL: FF7 0x007b67e0
static const char REG_MIDI_KEY[] = "Software\\Square Soft, Inc.\\Final Fantasy VII\\1.00\\Midi";

/*
 * Opens the machine-wide Final Fantasy VII registry key for reading into g_Reg_AppKey and returns
 * whether RegOpenKeyExA succeeded.
 */
// FUNCTION: FF7 0x0040a460
int __cdecl RegOpenFf7Key(void)
{
    return RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                         REG_APP_KEY, 0, KEY_READ, &g_Reg_AppKey) == 0;
}

/*
 * Closes the registry handle g_Reg_AppKey and returns whether RegCloseKey succeeded.
 */
// FUNCTION: FF7 0x0040a486
int __cdecl RegCloseFf7Key(void)
{
    return RegCloseKey(g_Reg_AppKey) == 0;
}

/*
 * Opens the machine-wide Final Fantasy VII 1.00 Sound registry subkey with KEY_READ; on success
 * calls RegQuerySoundOptionsValue with value, closes the key, and returns the helper result;
 * returns zero if opening fails.
 */
// FUNCTION: FF7 0x0040a49c
int __cdecl RegGetSoundOptions(void *value)
{
    int result;
    HKEY regKey;
    result = 0;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_SOUND_KEY, 0, KEY_READ, &regKey) == 0) {
        result = RegQuerySoundOptionsValue(regKey, value);
        RegCloseKey(regKey);
    }
    return result;
}

/*
 * Opens the machine-wide Final Fantasy VII 1.00 Sound registry subkey with KEY_READ; on success
 * calls RegQuerySoundGuidValue with value, closes the key, and returns the helper result; returns
 * zero if opening fails.
 */
// FUNCTION: FF7 0x0040a4ec
int __cdecl RegGetSoundDeviceGuid(void *value)
{
    int result;
    HKEY regKey;
    result = 1;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_SOUND_KEY, 0, KEY_READ, &regKey) == 0) {
        result = RegQuerySoundGuidValue(regKey, (unsigned char *)value);
        RegCloseKey(regKey);
    } else {
        result = 0;
    }
    return result;
}

/*
 * Opens the machine-wide Final Fantasy VII 1.00 Midi registry subkey with KEY_READ; on success
 * calls RegQueryMidiDeviceIdValue with value, closes the key, and returns the helper result;
 * returns zero if opening fails.
 */
// FUNCTION: FF7 0x0040a545
int __cdecl RegGetMidiDeviceId(void *value)
{
    int result;
    HKEY hKey;
    result = 1;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_MIDI_KEY, 0, KEY_READ, &hKey) == 0) {
        result = RegQueryMidiDeviceIdValue(hKey, value);
        RegCloseKey(hKey);
    } else {
        result = 0;
    }
    return result;
}

/*
 * Opens the machine-wide Final Fantasy VII 1.00 Midi registry subkey with KEY_READ; on success
 * calls RegQueryMidiOptionsValue with value, closes the key, and returns the helper result;
 * returns zero if opening fails.
 */
// FUNCTION: FF7 0x0040a59e
int __cdecl RegGetMidiOptions(void *value)
{
    int result;
    HKEY hKey;
    result = 0;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_MIDI_KEY, 0, KEY_READ, &hKey) == 0) {
        result = RegQueryMidiOptionsValue(hKey, value);
        RegCloseKey(hKey);
    }
    return result;
}

/*
 * Opens the machine-wide Final Fantasy VII 1.00 Sound registry subkey with KEY_READ; on success
 * calls RegWriteSoundGuidValue with value, closes the key, and returns the helper result; returns
 * zero if opening fails.
 */
// FUNCTION: FF7 0x0040a5ee
int __cdecl RegSetSoundDeviceGuid(void *value)
{
    int result;
    HKEY hKey;
    result = 1;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_SOUND_KEY, 0, KEY_READ, &hKey) == 0) {
        result = RegWriteSoundGuidValue(hKey, value);
        RegCloseKey(hKey);
    } else {
        result = 0;
    }
    return result;
}

/*
 * Writes the nonnull string as a NUL-terminated DD_GUID REG_SZ value through g_Reg_AppKey, or deletes DD_GUID when value is null; returns API success.
 */
// FUNCTION: FF7 0x0040a647
int __cdecl RegSetDdrawDeviceGuid(char *value)
{
    if (value) {
        return RegSetValueExA(g_Reg_AppKey,
                              REG_DD_GUID, 0, REG_SZ, (BYTE *)value, strlen(value) + 1) == 0;
    } else {
        return RegDeleteValueA(g_Reg_AppKey,
                               REG_DD_GUID) == 0;
    }
}

/*
 * Opens the machine-wide Final Fantasy VII 1.00 Midi registry subkey with KEY_READ; on success
 * calls RegWriteMidiDeviceIdValue with value, closes the key, and returns the helper result;
 * returns zero if opening fails.
 */
// FUNCTION: FF7 0x0040a699
int __cdecl RegSetMidiDeviceId(void *value)
{
    int result;
    HKEY hKey;
    result = 1;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_MIDI_KEY, 0, KEY_READ, &hKey) == 0) {
        result = RegWriteMidiDeviceIdValue(hKey, value);
        RegCloseKey(hKey);
    } else {
        result = 0;
    }
    return result;
}

/*
 * Queries DD_GUID into a 256-byte local buffer; on success copies it into value and returns 1,
 * otherwise returns 0 without clearing the caller buffer.
 */
// FUNCTION: FF7 0x0040a6f2
int __cdecl RegGetDdrawDeviceGuid(char *value)
{
    DWORD dataBytes;
    char buf[256];
    DWORD regType;
    LONG state;
    dataBytes = 256;
    state = RegQueryValueExA(g_Reg_AppKey,
                         REG_DD_GUID, 0, &regType, (BYTE *)buf, &dataBytes);
    if (state == 0) {
        strcpy(value, buf);
        return 1;
    } else {
        value = 0;
        return 0;
    }
}

/*
 * Opens the machine-wide Final Fantasy VII 1.00 Midi registry subkey with KEY_READ; on success
 * calls RegQueryMidiDataValue with value, closes the key, and returns the helper result; returns
 * zero if opening fails.
 */
// FUNCTION: FF7 0x0040a771
int __cdecl RegGetMidiDataType(char *value)
{
    int result;
    HKEY hKey;
    result = 1;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_MIDI_KEY, 0, KEY_READ, &hKey) == 0) {
        result = RegQueryMidiDataValue(hKey, value);
        RegCloseKey(hKey);
    } else {
        result = 0;
    }
    return result;
}

/*
 * Opens the machine-wide Final Fantasy VII 1.00 Midi registry subkey with KEY_READ; on success
 * calls RegWriteMidiDataValue with value, closes the key, and returns the helper result; returns
 * zero if opening fails.
 */
// FUNCTION: FF7 0x0040a7ca
int __cdecl RegSetMidiDataType(char *value)
{
    int result;
    HKEY hKey;
    result = 1;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_MIDI_KEY, 0, KEY_READ, &hKey) == 0) {
        result = RegWriteMidiDataValue(hKey, value);
        RegCloseKey(hKey);
    } else {
        result = 0;
    }
    return result;
}

/*
 * Queries Sound_GUID with capacity sixteen and returns one after ordinary success. On successful
 * zero-length data with a nonnull output fills sixteen bytes with 255 and returns zero; a query
 * failure also returns zero.
 */
// FUNCTION: FF7 0x0040a823
int __cdecl RegQuerySoundGuidValue(HKEY key, unsigned char *value)
{
    DWORD dataBytes;
    DWORD regType;
    int res;
    unsigned int byteIdx;
    res = 1;
    dataBytes = 16;
    if (RegQueryValueExA(key,
                         REG_SOUND_GUID, 0, &regType, value, &dataBytes) == 0) {
        if (value && dataBytes == 0) {
            for (byteIdx = 0; byteIdx < 16; byteIdx++) {
                value[byteIdx] = 255;
            }
            res = 0;
        }
    } else {
        res = 0;
    }
    return res;
}

/*
 * Writes 16 bytes from a nonnull value as Sound_GUID REG_BINARY, or deletes Sound_GUID when value
 * is null; returns API success.
 */
// FUNCTION: FF7 0x0040a89e
int __cdecl RegWriteSoundGuidValue(HKEY key, void *value)
{
    if (value) {
        return RegSetValueExA(key,
                              REG_SOUND_GUID, 0, REG_BINARY, (BYTE *)value, 16) == 0;
    } else {
        return RegDeleteValueA(key,
                               REG_SOUND_GUID) == 0;
    }
}

/*
 * Queries the Options registry value into the supplied buffer with a four-byte capacity and
 * returns whether the query succeeded.
 */
// FUNCTION: FF7 0x0040a8dd
int __cdecl RegQuerySoundOptionsValue(HKEY key, void *value)
{
    DWORD dataSize;
    DWORD valueType;
    dataSize = 4;
    return RegQueryValueExA(key,
                            REG_OPTIONS, 0, &valueType, (BYTE *)value, &dataSize) == 0;
}

/*
 * Writes four bytes from the supplied buffer to the Options registry value as REG_DWORD and
 * returns whether the write succeeded.
 */
// FUNCTION: FF7 0x0040a910
int __cdecl RegWriteSoundOptionsValue(HKEY key, void *value)
{
    return RegSetValueExA(key,
                          REG_OPTIONS, 0, REG_DWORD, (BYTE *)value, 4) == 0;
}

/*
 * Queries the Options registry value into the supplied buffer with a four-byte capacity and
 * returns whether the query succeeded.
 */
// FUNCTION: FF7 0x0040a933
int __cdecl RegQueryMidiOptionsValue(HKEY key, void *value)
{
    DWORD dataSize;
    DWORD valueType;
    dataSize = 4;
    return RegQueryValueExA(key,
                            REG_OPTIONS_2, 0, &valueType, (BYTE *)value, &dataSize) == 0;
}

/*
 * Writes four bytes from the supplied buffer to the Options registry value as REG_DWORD and
 * returns whether the write succeeded.
 */
// FUNCTION: FF7 0x0040a966
int __cdecl RegWriteMidiOptionsValue(HKEY key, void *value)
{
    return RegSetValueExA(key,
                          REG_OPTIONS_2, 0, REG_DWORD, (BYTE *)value, 4) == 0;
}

/*
 * Queries the MIDI_DeviceID registry value into the supplied buffer with a four-byte capacity and
 * returns whether the query succeeded.
 */
// FUNCTION: FF7 0x0040a989
int __cdecl RegQueryMidiDeviceIdValue(HKEY key, void *value)
{
    DWORD dataSize;
    DWORD valueType;
    dataSize = 4;
    return RegQueryValueExA(key,
                            REG_MIDI_DEVICEID, 0, &valueType, (BYTE *)value, &dataSize) == 0;
}

/*
 * Writes four bytes from value as the MIDI_DeviceID REG_DWORD value and returns whether the write
 * succeeded.
 */
// FUNCTION: FF7 0x0040a9bc
int __cdecl RegWriteMidiDeviceIdValue(HKEY key, void *value)
{
    if (RegSetValueExA(key,
                       REG_MIDI_DEVICEID, 0, REG_DWORD, (BYTE *)value, 4) == 0) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Queries MIDI_data into a 256-byte local buffer; on success copies it into value and returns 1,
 * otherwise returns 0 without clearing the caller buffer.
 */
// FUNCTION: FF7 0x0040a9e7
int __cdecl RegQueryMidiDataValue(HKEY key, char *value)
{
    DWORD dataBytes;
    char buf[256];
    DWORD regType;
    LONG state;
    dataBytes = 256;
    state = RegQueryValueExA(key,
                         REG_MIDI_DATA, 0, &regType, (BYTE *)buf, &dataBytes);
    if (state == 0) {
        strcpy(value, buf);
        return 1;
    } else {
        value = 0;
        return 0;
    }
}

/*
 * Writes the nonnull string as a NUL-terminated MIDI_data REG_SZ value, or deletes MIDI_data when
 * value is null; returns API success.
 */
// FUNCTION: FF7 0x0040aa64
int __cdecl RegWriteMidiDataValue(HKEY key, char *value)
{
    if (value) {
        return RegSetValueExA(key,
                              REG_MIDI_DATA, 0, REG_SZ, (BYTE *)value, strlen(value) + 1) == 0;
    } else {
        return RegDeleteValueA(key,
                               REG_MIDI_DATA) == 0;
    }
}

} /* extern "C" */
