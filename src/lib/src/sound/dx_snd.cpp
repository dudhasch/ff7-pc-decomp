// src/lib/src/sound/dx_snd.cpp
// Original: C:\lib\src\sound\dx_snd.cpp
// Contents: DirectSound wrapper: enumerates devices, opens the device, and creates, duplicates,
//   locks, unlocks, plays and stops sound buffers, with position, pan, volume and frequency
//   controls.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/types/sound.h"
#include <windows.h>
#include <dsound.h>
#include <string.h>
#include "lib/api/sound.h"

typedef IDirectSoundBuffer DsNativeBuffer;
#pragma intrinsic(strlen, strcpy, memcpy)
extern "C" {
// The enumerated devices: a copy of each device GUID, its description and its driver module name.
// GLOBAL: FF7 0x00de6618
struct SoundDsDeviceSlot g_SoundDsDeviceSlots[20] = {0};
// GLOBAL: FF7 0x00de6708
int g_SoundDsDeviceCount = 0;
unsigned char g_bssPad_00de670c[4] = {0};
// GLOBAL: FF7 0x00de6710
DSCAPS g_SoundDsCaps = {0};
// GLOBAL: FF7 0x00de6770
IDirectSound *g_SoundDirectSound = 0;

BOOL __stdcall DsEnumDevicesCallback(GUID *guid, const char *description, const char *module, void *context);
int DsOpenDevice(HWND window, GUID *guid);
DSCAPS *DsGetDeviceCaps(void);
IDirectSoundBuffer *DsCreatePrimaryBuffer(WAVEFORMATEX *format);
IDirectSoundBuffer *DsCreateSoundBuffer(unsigned int size, WAVEFORMATEX *format);
IDirectSoundBuffer *DsDuplicateSoundBuffer(IDirectSoundBuffer *source);
void __cdecl DsReleaseBuffer(DsNativeBuffer *buffer);
int DsLockBuffer(IDirectSoundBuffer *buffer, unsigned int size, void **first, unsigned long *firstSize, void **second, unsigned long *secondSize, unsigned int offset);
void __cdecl DsUnlockBuffer(DsNativeBuffer *buffer, void *first, unsigned int firstSize, void *second, unsigned int secondSize);
void __cdecl DsSetCurrentPosition(DsNativeBuffer *buffer, unsigned int position);
unsigned int __cdecl DsGetCurrentPlayPosition(DsNativeBuffer *buffer);
int DsPlayBuffer(IDirectSoundBuffer *buffer, unsigned int flags);
void __cdecl DsStopBuffer(DsNativeBuffer *buffer);
int DsSetPanMillibels(IDirectSoundBuffer *buffer, int pan);
int DsSetVolumeMillibels(IDirectSoundBuffer *buffer, int volume);
int DsSetFrequency(IDirectSoundBuffer *buffer, unsigned int frequency);

/*
 * Enumerates DirectSound capture/render devices via DirectSoundEnumerateA.
 */
// FUNCTION: FF7 0x0074a0f0
int DsEnumerateDevices(void **out)
{
    *out = g_SoundDsDeviceSlots;
    g_SoundDsDeviceCount = 0;
    DsCheckError(DirectSoundEnumerateA(DsEnumDevicesCallback, 0),
                 // STRING: FF7 0x00968920
                 "C:\\lib\\src\\sound\\dx_snd.cpp", 17);
    return g_SoundDsDeviceCount;
}

/*
 * Frees allocated buffers for enumerated sound devices.
 */
// FUNCTION: FF7 0x0074a129
void __cdecl DsFreeEnumeratedDevices(void)
{
    int i;

    if (g_SoundDsDeviceCount == 0) {
        return;
    }
    for (i = 0; i < g_SoundDsDeviceCount; i++) {
        if (g_SoundDsDeviceSlots[i].buffer != 0) {
            // STRING: FF7 0x0096893c
            GameHeapFree(g_SoundDsDeviceSlots[i].buffer, "C:\\lib\\src\\sound\\dx_snd.cpp", 30);
            g_SoundDsDeviceSlots[i].buffer = 0;
        }
        if (g_SoundDsDeviceSlots[i].description != 0) {
            // STRING: FF7 0x00968958
            GameHeapFree(g_SoundDsDeviceSlots[i].description, "C:\\lib\\src\\sound\\dx_snd.cpp", 35);
            g_SoundDsDeviceSlots[i].description = 0;
        }
        if (g_SoundDsDeviceSlots[i].module != 0) {
            // STRING: FF7 0x00968974
            GameHeapFree(g_SoundDsDeviceSlots[i].module, "C:\\lib\\src\\sound\\dx_snd.cpp", 40);
            g_SoundDsDeviceSlots[i].module = 0;
        }
    }
}

/*
 * Callback invoked for each enumerated DirectSound device.
 */
// FUNCTION: FF7 0x0074a216
BOOL __stdcall DsEnumDevicesCallback(GUID *guid, const char *description, const char *module, void *context)
{
    g_SoundDsDeviceSlots[g_SoundDsDeviceCount].buffer = 0;
    if (guid) {
        g_SoundDsDeviceSlots[g_SoundDsDeviceCount].buffer = (GUID *)GameHeapAllocRaw(16,
                                                                                         // STRING: FF7 0x00968990
                                                                                         "C:\\lib\\src\\sound\\dx_snd.cpp", 51);
        memcpy(g_SoundDsDeviceSlots[g_SoundDsDeviceCount].buffer, guid, sizeof(GUID));
    }
    g_SoundDsDeviceSlots[g_SoundDsDeviceCount].description = (char *)GameHeapAllocRaw(strlen(description) + 1,
                                                                                     // STRING: FF7 0x009689ac
                                                                                     "C:\\lib\\src\\sound\\dx_snd.cpp", 54);
    if (g_SoundDsDeviceSlots[g_SoundDsDeviceCount].description)
        strcpy(g_SoundDsDeviceSlots[g_SoundDsDeviceCount].description, description);
    g_SoundDsDeviceSlots[g_SoundDsDeviceCount].module = (char *)GameHeapAllocRaw(strlen(module) + 1,
                                                                                     // STRING: FF7 0x009689c8
                                                                                     "C:\\lib\\src\\sound\\dx_snd.cpp", 56);
    if (g_SoundDsDeviceSlots[g_SoundDsDeviceCount].module)
        strcpy(g_SoundDsDeviceSlots[g_SoundDsDeviceCount].module, module);
    g_SoundDsDeviceCount++;
    return 1;
}

/*
 * Creates and initializes the DirectSound device interface.
 */
// FUNCTION: FF7 0x0074a36b
int DsOpenDevice(HWND window, GUID *guid)
{
    int result = 0;
    if (DsCheckError(DirectSoundCreate(guid, &g_SoundDirectSound, 0),
                     // STRING: FF7 0x009689e4
                     "C:\\lib\\src\\sound\\dx_snd.cpp", 68)) {
        if (DsCheckError(g_SoundDirectSound->SetCooperativeLevel(window, DSSCL_EXCLUSIVE),
                         // STRING: FF7 0x00968a00
                         "C:\\lib\\src\\sound\\dx_snd.cpp", 73))
            result = 1;
        else {
            g_SoundDirectSound->Release();
            g_SoundDirectSound = 0;
        }
    } else
        g_SoundDirectSound = 0;
    return result;
}

/*
 * Releases the active DirectSound device interface.
 */
// FUNCTION: FF7 0x0074a3fe
void DsReleaseDevice(void)
{
    if (g_SoundDirectSound) {
        g_SoundDirectSound->Release();
        g_SoundDirectSound = 0;
    }
}

/*
 * Queries and returns device capabilities for the active DirectSound device.
 */
// FUNCTION: FF7 0x0074a427
DSCAPS *DsGetDeviceCaps(void)
{
    g_SoundDsCaps.dwSize = sizeof(DSCAPS);
    if (g_SoundDirectSound) {
        if (!DsCheckError(g_SoundDirectSound->GetCaps(&g_SoundDsCaps),
                          // STRING: FF7 0x00968a1c
                          "C:\\lib\\src\\sound\\dx_snd.cpp", 108))
            return 0;
    }
    return &g_SoundDsCaps;
}

/*
 * Creates the primary sound buffer with the specified wave format.
 */
// FUNCTION: FF7 0x0074a472
IDirectSoundBuffer *DsCreatePrimaryBuffer(WAVEFORMATEX *format)
{
    struct {
        DSBUFFERDESC desc;
        IDirectSoundBuffer *buffer;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.buffer = 0;
    if (g_SoundDirectSound) {
        frame.desc.dwSize = sizeof(frame.desc);
        frame.desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
        frame.desc.dwBufferBytes = 0;
        frame.desc.lpwfxFormat = 0;
        frame.desc.dwReserved = 0;
        if (DsCheckError(g_SoundDirectSound->CreateSoundBuffer(&frame.desc, &frame.buffer, 0),
                         // STRING: FF7 0x00968a38
                         "C:\\lib\\src\\sound\\dx_snd.cpp", 131)) {
            if (format)
                DsCheckError(frame.buffer->SetFormat(format),
                             // STRING: FF7 0x00968a54
                             "C:\\lib\\src\\sound\\dx_snd.cpp", 135);
            DsCheckError(frame.buffer->Play(0, 0, DSBPLAY_LOOPING),
                         // STRING: FF7 0x00968a70
                         "C:\\lib\\src\\sound\\dx_snd.cpp", 139);
        } else
            frame.buffer = 0;
    }
    return frame.buffer;
}

/*
 * Creates a secondary sound buffer of the specified size and format.
 */
// FUNCTION: FF7 0x0074a540
IDirectSoundBuffer *DsCreateSoundBuffer(unsigned int size, WAVEFORMATEX *format)
{
    struct {
        DSBUFFERDESC desc;
        IDirectSoundBuffer *buffer;
    } frame;
    frame.buffer = 0;
    if (g_SoundDirectSound) {
        frame.desc.dwSize = sizeof(frame.desc);
        frame.desc.dwFlags = DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN |
                              DSBCAPS_CTRLVOLUME | DSBCAPS_GETCURRENTPOSITION2;
        frame.desc.dwBufferBytes = size;
        frame.desc.lpwfxFormat = format;
        frame.desc.dwReserved = 0;
        if (!DsCheckError(g_SoundDirectSound->CreateSoundBuffer(&frame.desc, &frame.buffer, 0),
                          // STRING: FF7 0x00968a8c
                          "C:\\lib\\src\\sound\\dx_snd.cpp", 165))
            frame.buffer = 0;
    }
    return frame.buffer;
}

/*
 * Duplicates an existing DirectSound buffer.
 */
// FUNCTION: FF7 0x0074a5b7
IDirectSoundBuffer *DsDuplicateSoundBuffer(IDirectSoundBuffer *source)
{
    IDirectSoundBuffer *buffer;
    if (source && g_SoundDirectSound) {
        if (DsCheckError(g_SoundDirectSound->DuplicateSoundBuffer(source, &buffer),
                         // STRING: FF7 0x00968aa8
                         "C:\\lib\\src\\sound\\dx_snd.cpp", 182))
            return buffer;
    }
    return 0;
}

/*
 * Releases a DirectSound buffer.
 */
// FUNCTION: FF7 0x0074a606
void __cdecl DsReleaseBuffer(DsNativeBuffer *buffer)
{
    if (buffer != 0) {
        DsCheckError(buffer->Release(),
                     // STRING: FF7 0x00968ac4
                     "C:\\lib\\src\\sound\\dx_snd.cpp", 196);
    }
}

/*
 * Locks a region of a DirectSound buffer for writing audio samples.
 */
// FUNCTION: FF7 0x0074a630
int DsLockBuffer(IDirectSoundBuffer *buffer, unsigned int size, void **first, unsigned long *firstSize, void **second, unsigned long *secondSize, unsigned int offset)
{
    HRESULT result;
    if (buffer) {
        result = buffer->Lock(offset, size, first, firstSize, second, secondSize, 0);
        switch (result) {
        case DSERR_BUFFERLOST:
            if (DsCheckError(buffer->Restore(),
                             // STRING: FF7 0x00968ae0
                             "C:\\lib\\src\\sound\\dx_snd.cpp", 211)) {
                DsCheckError(buffer->Lock(offset, size, first, firstSize, second, secondSize, 0),
                             // STRING: FF7 0x00968afc
                             "C:\\lib\\src\\sound\\dx_snd.cpp", 214);
                return 1;
            }
        case DS_OK:
            return 1;
        }
    }
    return 0;
}

/*
 * Unlocks a previously locked DirectSound buffer.
 */
// FUNCTION: FF7 0x0074a6f0
void __cdecl DsUnlockBuffer(DsNativeBuffer *buffer, void *first, unsigned int firstSize, void *second, unsigned int secondSize)
{
    if (buffer != 0) {
        DsCheckError(buffer->Unlock(first, firstSize, second, secondSize),
                     // STRING: FF7 0x00968b18
                     "C:\\lib\\src\\sound\\dx_snd.cpp", 232);
    }
}

/*
 * Sets the current play position in the DirectSound buffer.
 */
// FUNCTION: FF7 0x0074a72a
void __cdecl DsSetCurrentPosition(DsNativeBuffer *buffer, unsigned int position)
{
    if (buffer != 0) {
        DsCheckError(buffer->SetCurrentPosition(position),
                     // STRING: FF7 0x00968b34
                     "C:\\lib\\src\\sound\\dx_snd.cpp", 243);
    }
}

/*
 * Returns the current play cursor position in the DirectSound buffer.
 */
// FUNCTION: FF7 0x0074a758
unsigned int __cdecl DsGetCurrentPlayPosition(DsNativeBuffer *buffer)
{
    unsigned long position;

    position = 0;
    if (buffer != 0) {
        DsCheckError(buffer->GetCurrentPosition(&position, 0),
                     // STRING: FF7 0x00968b50
                     "C:\\lib\\src\\sound\\dx_snd.cpp", 253);
    }
    return position;
}

/*
 * Starts playback on the specified DirectSound buffer with flags.
 */
// FUNCTION: FF7 0x0074a795
int DsPlayBuffer(IDirectSoundBuffer *buffer, unsigned int flags)
{
    HRESULT result;
    if (buffer) {
        result = buffer->Play(0, 0, flags);
        switch (result) {
        case DSERR_BUFFERLOST:
            DsCheckError(buffer->Restore(),
                         // STRING: FF7 0x00968b6c
                         "C:\\lib\\src\\sound\\dx_snd.cpp", 270);
            return -1;
        case DS_OK:
            return 1;
        }
    }
    return 0;
}

/*
 * Stops playback on the specified DirectSound buffer.
 */
// FUNCTION: FF7 0x0074a800
void __cdecl DsStopBuffer(DsNativeBuffer *buffer)
{
    if (buffer != 0) {
        DsCheckError(buffer->Stop(),
                     // STRING: FF7 0x00968b88
                     "C:\\lib\\src\\sound\\dx_snd.cpp", 285);
    }
}

/*
 * Sets panning attenuation in hundredths of a decibel.
 */
// FUNCTION: FF7 0x0074a82a
int DsSetPanMillibels(IDirectSoundBuffer *buffer, int pan)
{
    if (pan < -10000 || pan > 10000)
        return 0;
    if (buffer)
        return DsCheckError(buffer->SetPan(pan),
                            // STRING: FF7 0x00968ba4
                            "C:\\lib\\src\\sound\\dx_snd.cpp", 296);
    return 0;
}

/*
 * Sets volume attenuation in hundredths of a decibel.
 */
// FUNCTION: FF7 0x0074a872
int DsSetVolumeMillibels(IDirectSoundBuffer *buffer, int volume)
{
    if (volume < -10000 || volume > 0)
        return 0;
    if (buffer)
        return DsCheckError(buffer->SetVolume(volume),
                            // STRING: FF7 0x00968bc0
                            "C:\\lib\\src\\sound\\dx_snd.cpp", 309);
    return 0;
}

/*
 * Sets playback frequency for the DirectSound buffer in Hertz.
 */
// FUNCTION: FF7 0x0074a8b7
int DsSetFrequency(IDirectSoundBuffer *buffer, unsigned int frequency)
{
    if (frequency < 100 || frequency > 100000)
        return 0;
    if (buffer)
        return DsCheckError(buffer->SetFrequency(frequency),
                            // STRING: FF7 0x00968bdc
                            "C:\\lib\\src\\sound\\dx_snd.cpp", 322);
    return 0;
}
} // extern "C"
