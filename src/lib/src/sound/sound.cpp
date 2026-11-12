// src/lib/src/sound/sound.cpp
// Original: C:\lib\src\sound\sound.cpp

// GUID and WAVEFORMATEX use the period SDK layout.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/enums/file.h"
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <string.h>
#include <stdio.h>
#include "lib/api/sound.h"
#include "ff7/api/main.h"
#include "lib/types/sound.h"
#include "lib/api/graphics.h"
#include "lib/enums/sound.h"
#include "lib/api/file.h"
#include "lib/codegen.h"

struct SoundAcmStreamHeader;
extern "C" {
// clang-format off
// Timer handle for background music service callbacks,
// defined at the head of sound.cpp's .bss span.
// GLOBAL: FF7 0x00ddffd8
unsigned int g_MusicServiceTimerId = 0;
// GLOBAL: FF7 0x00ddffdc
long g_MusicServiceTimerBusy = 0;
// GLOBAL: FF7 0x00ddffe0
int g_MusicServiceTimerActive = 0;
// GLOBAL: FF7 0x00ddffe4
int g_MusicWaveRegistrySetting = 0;
// GLOBAL: FF7 0x00ddffe8
int g_MusicFadeDuration = 0;
// GLOBAL: FF7 0x00ddffec
int g_MusicPendingTrack = 0;
// GLOBAL: FF7 0x00ddfff0
int g_SoundGroupSlots[4] = {0};
// GLOBAL: FF7 0x00de0000
int g_MusicCrossPlayActive = 0;
unsigned char g_bssPad_00de0004[4] = {0};
// GLOBAL: FF7 0x00de0008
CRITICAL_SECTION g_SoundLock = {0};
// GLOBAL: FF7 0x00de0020
struct SoundGroup g_SoundGroups[16] = {0};
// The sound system's state block 0xDE0120..0xDE6568 (25,672 B: the initialized flag, the channel
// records, the format header at 0xDE0E84, and the volume words). Defined as one SoundChannels
// struct in this TU so interior fields at 4 mod 8 (such as g_SoundChannels.channels) keep their
// exact offsets without VC5 8-byte-aligning separate definitions. MusicWaveOpen loads the
// address of g_MusicWavHeaderBuffer (0xDE62BC) as a direct symbol immediate, so that word
// and the trailing buffer are split into separate definitions below.
//
// GLOBAL: FF7 0x00de0120
struct SoundChannels g_SoundChannels = {0};
// GLOBAL: FF7 0x00de62bc
int g_MusicWavHeaderBuffer = 0;
// GLOBAL: FF7 0x00de62c0
struct SoundData g_SoundData = {0};
#define g_SoundAudioDataDirectory (*(char(*)[128])((char *)&g_SoundData + 0x198))
#define g_SoundMusicWaveDirectory (*(char(*)[128])((char *)&g_SoundData + 0x218))
// GLOBAL: FF7 0x00de6568
unsigned char g_SoundCriticalSectionMusic[24] = {0};
// GLOBAL: FF7 0x00de6580
int g_MusicFadeStarted = 0;
// GLOBAL: FF7 0x00de6584
int g_MusicVolume = 0;
// GLOBAL: FF7 0x00de6588
int g_MusicTargetVolume = 0;
// GLOBAL: FF7 0x00de658c
int g_MusicFadeRate = 0;
// GLOBAL: FF7 0x00de6590
int g_MusicFadeElapsed = 0;
// GLOBAL: FF7 0x00de6594
int g_MusicPlayAfterFade = 0;
// GLOBAL: FF7 0x00de6598
unsigned char g_SoundLoopBufferLock[24] = {0};
// GLOBAL: FF7 0x00de65b0
unsigned int g_SfxStreamServiceTimerId = 0;
// GLOBAL: FF7 0x00de65b4
long g_SfxStreamServiceBusy = 0;
// GLOBAL: FF7 0x00de65b8
int g_SfxStreamServiceTimerActive = 0;
// GLOBAL: FF7 0x00de65bc
int g_SoundLastTick = 0;
// clang-format on
// The head of sound.cpp's .data span 0x967660..0x967AA4: the volume and pan millibel tables (128 words
// each: g_SoundVolumeMillibelTable and g_SoundPanMillibelTable) followed by the 17 startup
// preload sound IDs (g_SoundStaticPreloadIds) at 0x00967660..0x00967AA4.
// Volume and pan indices 0..127 map directly to DirectSound attenuation/pan millibels.
int g_SoundVolumeMillibelTable[128] = {
    -10000, -9205, -9800, -8559, -8250, -7958, -7650, -7398,
    -7100, -6878, -6600, -6393, -6150, -5942, -5750, -5523,
    -5300, -5132, -4950, -4769, -4431, -4250, -4116, -3975,
    -3824, -3700, -3551, -3425, -3298, -3150, -3062, -2950,
    -2842, -2750, -2638, -2550, -2448, -2271, -2190, -2106,
    -1975, -1953, -1880, -1810, -1740, -1678, -1610, -1554,
    -1490, -1439, -1380, -1333, -1280, -1233, -1190, -1140,
    -1100, -1060, -1030, -1000, -970, -940, -910, -890,
    -870, -850, -830, -797, -765, -735, -705, -677,
    -649, -623, -597, -573, -549, -526, -504, -482,
    -462, -442, -423, -404, -386, -369, -353, -337,
    -321, -306, -292, -278, -265, -252, -239, -227,
    -216, -205, -194, -183, -173, -164, -154, -145,
    -137, -128, -120, -112, -105, -98, -91, -84,
    -77, -71, -65, -59, -53, -48, -43, -38,
    -33, -28, -24, -19, -15, -11, -7, 0,
};
int g_SoundPanMillibelTable[128] = {
    -10000, -9205, -8559, -7958, -7398, -6878, -6393, -5942,
    -5523, -5132, -4769, -4431, -4116, -3824, -3551, -3298,
    -3062, -2842, -2638, -2448, -2271, -2106, -1953, -1810,
    -1678, -1554, -1439, -1333, -1233, -1140, -1054, -974,
    -900, -830, -765, -705, -649, -597, -549, -504,
    -462, -423, -386, -353, -321, -292, -265, -239,
    -216, -194, -173, -154, -137, -120, -105, -91,
    -77, -65, -53, -43, -33, -24, -15, -7,
    0, 15, 24, 33, 43, 53, 65, 77,
    91, 105, 120, 137, 154, 173, 194, 216,
    239, 265, 292, 321, 353, 386, 423, 462,
    504, 549, 597, 649, 705, 765, 830, 900,
    974, 1054, 1140, 1233, 1333, 1439, 1554, 1678,
    1810, 1953, 2106, 2271, 2448, 2638, 2842, 3062,
    3298, 3551, 3824, 4116, 4431, 4769, 5132, 5523,
    5942, 6393, 6878, 7398, 7958, 8559, 9205, 10000,
};
int g_SoundStaticPreloadIds[17] = {
    1, 2, 3, 4, 41, 43, 44, 45,
    208, 263, 264, 384, 447, 451, 481, 467,
    483,
};
// The registry key read by MusicReadWaveMusicRegistrySetting (0x00749512, in the service below): VC5 emits a TU's const array definitions into .rdata ahead of its floating-point literal pool
// (0x007b7ea0..0x007b7ec0 in sound.cpp).
// GLOBAL: FF7 0x007b7e70
extern const char s_ff7RegistryKey[] = "Software\\Square Soft, Inc.\\Final Fantasy VII";
#pragma intrinsic(memset, strcpy)
/* Internal DirectSound wrapper prototypes (dx_snd.cpp) with int handle parameters for this C++ TU. */
extern int __cdecl DsOpenDevice(int, GUID *), DsCreatePrimaryBuffer(WAVEFORMATEX *);
extern void __cdecl DsReleaseBuffer(int buffer); /* dx_snd.cpp helper. */
#pragma intrinsic(strlen)
typedef void(__stdcall *SoundTimeCallback)(unsigned int, unsigned int, unsigned long, unsigned long, unsigned long);
__declspec(dllimport) unsigned int __stdcall timeSetEvent(unsigned int, unsigned int, SoundTimeCallback, unsigned long, unsigned int);
__declspec(dllimport) unsigned int __stdcall timeBeginPeriod(unsigned int);
__declspec(dllimport) long __stdcall InterlockedExchange(long *, long);

// Original timer setup and InterlockedExchange access one DWORD each.

__declspec(dllimport) void __stdcall OutputDebugStringA(const char *);

__declspec(dllimport) unsigned int __stdcall timeKillEvent(unsigned int event);
__declspec(dllimport) unsigned int __stdcall timeEndPeriod(unsigned int period);

// Each original timer/state operand reads or writes one four-byte scalar.
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)
#pragma intrinsic(strcpy, strcat)
struct FileHandle;
/* Internal DirectSound buffer lock/create/write helpers taking int buffer handles in this C++ TU. */
extern int __cdecl DsLockBuffer(int, int size, void *first, void *firstSize, int second, int secondSize, int offset);
extern void __cdecl DsUnlockBuffer(int buffer, int first, int firstSize, int second, int secondSize); /* dx_snd.cpp helper. */
extern int __cdecl DsCreateSoundBuffer(int size, void *format); /* dx_snd.cpp helper returning int handle. */

extern void __cdecl DsWriteIntoBufferRegion(char *source, int sourceBytes, IDirectSoundBuffer *buffer, int offset, int bytes); /* dx_snd.cpp helper. */

#pragma intrinsic(memcpy, memset)
int __cdecl DsDuplicateSoundBuffer(int source);
int __cdecl DsPlayBuffer(int, int flags);
void __cdecl DsSetCurrentPosition(int buffer, int position);
int __cdecl DsSetFrequency(int, int);
int __cdecl DsSetVolumeMillibels(int, int);
void __cdecl DsStopBuffer(int buffer);
void __stdcall SfxLoopSwitchTimerProc(UINT timer, UINT message, DWORD user, DWORD one, DWORD two);
#pragma intrinsic(memcpy)
__declspec(dllimport) void __stdcall OutputDebugStringA(const char *text);
#pragma intrinsic(strcpy, strcat, memset, strcmp)
// Prototypes of callees the C sources used implicitly (definitions elsewhere), required in C++.
extern "C" unsigned int __cdecl DsGetCurrentPlayPosition(int buffer);
extern "C" int __cdecl DsSetPanMillibels(int buffer, int pan);

/*
 * Initializes sound state and returns the initialization flag. Returns 1 immediately if already
 * initialized; otherwise clears state, copies optional paths, reads device/options settings and
 * initializes the selected sound device. Creates a stereo 44.1 kHz 16-bit PCM primary buffer,
 * loads the format table and preloads 17 static sounds without checking their individual results.
 * Device, primary-buffer or format-table failure returns 0; the latter failures release the
 * initialized device, and format failure also releases the primary buffer. On success initializes
 * volume/fade state, channels 1 through 5, three critical sections and wave-music registry
 * settings. Path copies are unbounded.
 */
// FUNCTION: FF7 0x00744400
int __cdecl SoundInit(const char *first, const char *directory, int third)
{
    struct {
        int preloadIndex;
        GUID guid;
        GUID *selected;
        int channelIndex;
        WAVEFORMATEX format;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    // STRING: FF7 0x00967aa4
    Gfx_DebugPrint("initializing sound...\n");
    g_MusicTrack1Requested = 0;
    if ((g_SoundChannels.initialized) != 0) {
        // STRING: FF7 0x00967abc
        Gfx_DebugPrint("...Again!!!\n");
        return 1;
    }
    memset((int *)&g_SoundChannels, 0, 0x6448);
    memset(&g_SoundGroupSlots, 0, 16);
    if (first != 0)
        strcpy(g_SoundAudioDataDirectory, first);
    else
        g_SoundAudioDataDirectory[0] = 0;
    if (directory != 0)
        strcpy(g_SoundMusicWaveDirectory, directory);
    else
        g_SoundMusicWaveDirectory[0] = 0;
    MusicLoadWaveNameIndex();
    frame.selected = 0;
    if (RegGetSoundDeviceGuid(&frame.guid))
        frame.selected = &frame.guid;
    if (RegGetSoundOptions(&g_SoundData.registryOptionsWord))
        g_SoundData.panInvertScale = (g_SoundData.registryOptionsWord & 1) ? -1 : 1;
    if (!DsOpenDevice(third, frame.selected))
        return 0;
    // STRING: FF7 0x00967acc
    Gfx_DebugPrint("creating dsound primary buffer\n");
    frame.format.wFormatTag = 1;
    frame.format.nChannels = 2;
    frame.format.nSamplesPerSec = 44100;
    frame.format.nAvgBytesPerSec = 176400;
    frame.format.nBlockAlign = 4;
    frame.format.wBitsPerSample = 16;
    frame.format.cbSize = 0;
    g_SoundChannels.primaryBuffer = DsCreatePrimaryBuffer(&frame.format);
    if (g_SoundChannels.primaryBuffer == 0) {
        DsReleaseDevice();
        return 0;
    }
    // STRING: FF7 0x00967aec
    Gfx_DebugPrint("reading audio file\n");
    if (SfxLoadFormatTable()) {
        (g_SoundChannels.initialized) = 1;
        // STRING: FF7 0x00967b00
        Gfx_DebugPrint("loading static sounds\n");
        for (frame.preloadIndex = 0; (unsigned int)frame.preloadIndex < 17; frame.preloadIndex++)
            SfxLoadIntoBuffer(g_SoundStaticPreloadIds[frame.preloadIndex], 0);
    } else {
        if (g_SoundChannels.primaryBuffer != 0) {
            DsReleaseBuffer(g_SoundChannels.primaryBuffer);
            g_SoundChannels.primaryBuffer = 0;
        }
        DsReleaseDevice();
        return 0;
    }
    g_SoundData.volumeFadeActiveFlag = 0;
    g_MusicTargetVolume = 127;
    g_MusicVolume = g_MusicTargetVolume;
    g_MusicFadeStarted = 0;
    SoundUpdateFades();
    for (frame.channelIndex = 1; frame.channelIndex <= 5; frame.channelIndex++)
        SfxResetChannel(frame.channelIndex, 64, *(int *)(g_SoundChannels.resetFormatRecordPtr + 4));
    InitializeCriticalSection(&g_SoundLock);
    InitializeCriticalSection((LPCRITICAL_SECTION)g_SoundCriticalSectionMusic);
    InitializeCriticalSection((LPCRITICAL_SECTION)g_SoundLoopBufferLock);
    g_SoundData.masterVolume = 100;
    g_SoundData.musicMasterVolumePercent = 100;
    g_MusicCrossPlayActive = 0;
    // STRING: FF7 0x00967b18
    Gfx_DebugPrint("sound initialized\n");
    MusicReadWaveMusicRegistrySetting();
    return (g_SoundChannels.initialized);
}

/*
 * Shuts the sound system down: stops the stream service timers, releases the cached and the channel
 * DirectSound buffers (logging `SOUND NOT UNLOADED` for each sound still loaded), frees the sample
 * and track-name buffers, closes both streams, the primary buffer, audio.dat and the device, and
 * deletes the three sound locks.
 */
// FUNCTION: FF7 0x007446d7
void __cdecl SoundShutdown(void)
{
    struct {
        int index;
        char message[128];
        SoundChannel *channel;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if ((g_SoundChannels.initialized) != 0) {
        SfxStreamStopServiceTimer();
        MusicWaveStopServiceTimer();
        for (frame.index = 0; (unsigned int)frame.index < 17; frame.index++)
            Sfx_ReleaseCachedBuffer(g_SoundStaticPreloadIds[frame.index], 15);
        for (frame.index = 0; frame.index < 5; frame.index++) {
            frame.channel = &g_SoundChannels.channels[frame.index];
            if (frame.channel->buffer != 0) {
                DsReleaseBuffer(frame.channel->buffer);
                frame.channel->buffer = 0;
            }
            if (frame.channel->loopBuffer != 0) {
                DsReleaseBuffer(frame.channel->loopBuffer);
                frame.channel->loopBuffer = 0;
            }
        }
        for (frame.index = 0; frame.index < 750; frame.index++) {
            if (((SoundCachedSample *)&g_SoundChannels.cachedSamples)[frame.index].data != 0)
                // STRING: FF7 0x00967b2c
                GameHeapFree((void *)((SoundCachedSample *)&g_SoundChannels.cachedSamples)[frame.index].data, "C:\\lib\\src\\sound\\sound.cpp", 0x136);
            if (g_SoundChannels.bufferHandles[frame.index] != 0) {
                // STRING: FF7 0x00967b48
                sprintf(frame.message, "SOUND NOT UNLOADED: %d\n", frame.index + 1);
                OutputDebugStringA(frame.message);
                DsReleaseBuffer(g_SoundChannels.bufferHandles[frame.index]);
                g_SoundChannels.bufferHandles[frame.index] = 0;
            }
        }
        for (frame.index = 0; frame.index < 98; frame.index++) {
            if (((int *)&g_SoundChannels.trackNameBuffers)[frame.index] != 0)
                // STRING: FF7 0x00967b60
                GameHeapFree((void *)((int *)&g_SoundChannels.trackNameBuffers)[frame.index], "C:\\lib\\src\\sound\\sound.cpp", 0x143);
        }
        if (g_SoundData.musicStreamBufferPtr != 0)
            // STRING: FF7 0x00967b7c
            GameHeapFree((void *)g_SoundData.musicStreamBufferPtr, "C:\\lib\\src\\sound\\sound.cpp", 0x147);
        SfxStreamClose((struct SoundStreamPlayer *)&g_SoundData.sfxStreamObject);
        SfxStreamClose((struct SoundStreamPlayer *)&g_SoundData.musicStreamObject);
        if (g_SoundChannels.primaryBuffer != 0) {
            DsReleaseBuffer(g_SoundChannels.primaryBuffer);
            g_SoundChannels.primaryBuffer = 0;
        }
        CloseFileEx((struct FileHandle *)g_SoundData.sfxDataFileHandle);
        DsReleaseDevice();
        DeleteCriticalSection(&g_SoundLock);
        DeleteCriticalSection((struct _RTL_CRITICAL_SECTION *)(g_SoundCriticalSectionMusic));
        DeleteCriticalSection((struct _RTL_CRITICAL_SECTION *)(g_SoundLoopBufferLock));
        (g_SoundChannels.initialized) = 0;
    }
}

/*
 * Reads `music.idx` (from the music wave directory when one is set): 98 newline-terminated track
 * names, each copied into a new heap string.
 */
// FUNCTION: FF7 0x0074496d
void __cdecl MusicLoadWaveNameIndex(void)
{
    struct {
        char path[128];
        int file, length, index;
        char name[20];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.index = 0;
    frame.length = 0;
    if ((char *)(int)g_SoundMusicWaveDirectory) {
        strcpy(frame.path, (char *)(int)g_SoundMusicWaveDirectory);
        // STRING: FF7 0x00967b98
        strcat(frame.path, "\\music.idx");
    } else {
        // STRING: FF7 0x00967ba4
        strcpy(frame.path, (char *)(int)"music.idx");
    }
    frame.file = OpenFileSimple(FILE_OPEN_READ_TEXT, frame.path);
    if (frame.file != 0) {
        for (frame.index = 0; frame.index < 98; frame.index++) {
            frame.length = 0;
            do {
                if (!ReadFileDataPartial(1, &frame.name[frame.length++], (struct FileHandle *)frame.file))
                    break;
            } while (frame.name[frame.length - 1] != 10);
            frame.name[frame.length - 1] = 0;
            // STRING: FF7 0x00967bb0
            ((int *)&g_SoundChannels.trackNameBuffers)[frame.index] = (int)GameHeapAllocRaw(strlen(frame.name) + 1, "C:\\lib\\src\\sound\\sound.cpp", 0x181);
            if (((int *)&g_SoundChannels.trackNameBuffers)[frame.index] != 0)
                strcpy((char *)((int *)&g_SoundChannels.trackNameBuffers)[frame.index], frame.name);
        }
    }
    CloseFileEx((struct FileHandle *)(frame.file));
}

/*
 * The per-frame sound update: advances the MIDI volume ramp, the music wave volume ramp (whose end
 * can start a queued track or stop a cross-played one), and the volume, pan and pitch ramps of the
 * five effect channels.
 */
// FUNCTION: FF7 0x00744b13
void __cdecl SoundUpdateFades(void)
{
    int currentTick;
    int dsVolume;
    int pan;
    int fadeStep;
    int freq;
    int channelIndex;
    SoundChannel *pChan;
    int musicIndex;
    int effectIdx;

    MidiUpdateVolumeFade();
    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    currentTick = (int)(GetTickCount());
    if (g_SoundData.volumeFadeActiveFlag != 0) {
        g_SoundData.volumeFadeActiveFlag = 0;
        if (g_MusicVolume != g_MusicTargetVolume) {
            if (g_MusicFadeStarted != 0) {
                g_MusicFadeElapsed += currentTick - g_SoundLastTick;
                fadeStep = g_MusicFadeElapsed * g_MusicFadeRate / 1000;
                if (fadeStep != 0) {
                    g_MusicFadeElapsed = 0;
                }
                g_MusicVolume += fadeStep;
                if ((fadeStep < 0 && g_MusicVolume < g_MusicTargetVolume) || (fadeStep > 0 && g_MusicVolume > g_MusicTargetVolume)) {
                    g_MusicVolume = g_MusicTargetVolume;
                }
                if (g_MusicVolume == g_MusicTargetVolume) {
                    g_MusicFadeStarted = 0;
                    if (g_MusicPlayAfterFade != 0) {
                        MusicPlayWaveTrack(g_MusicPendingTrack, 0);
                    }
                    if (g_MusicCrossPlayActive != 0) {
                        MusicWaveStop();
                        MusicWaveSetVolume(0x7F);
                    }
                }
                musicIndex = g_SoundData.musicMasterVolumePercent * g_MusicVolume / 100;
                musicIndex = musicIndex;
                dsVolume = g_SoundVolumeMillibelTable[musicIndex];
                EnterCriticalSection((struct _RTL_CRITICAL_SECTION *)(g_SoundCriticalSectionMusic));
                if (g_SoundData.musicStreamObject != 0) {
                    DsSetVolumeMillibels(g_SoundData.musicStreamObject, dsVolume);
                }
                LeaveCriticalSection((struct _RTL_CRITICAL_SECTION *)(g_SoundCriticalSectionMusic));
            } else {
                g_MusicFadeStarted = 1;
            }
        }
        for (channelIndex = 0; channelIndex < 5; channelIndex++) {
            pChan = &g_SoundChannels.channels[channelIndex];
            if (pChan->volume != pChan->targetVolume) {
                if (pChan->volumeFadeStarted != 0) {
                    pChan->volumeFadeElapsed += currentTick - g_SoundLastTick;
                    fadeStep = pChan->volumeFadeElapsed * pChan->volumeFadeRate / 1000;
                    if (fadeStep != 0) {
                        pChan->volumeFadeElapsed = 0;
                    }
                    pChan->volume += fadeStep;
                    if ((fadeStep < 0 && pChan->volume < pChan->targetVolume) || (fadeStep > 0 && pChan->volume > pChan->targetVolume)) {
                        pChan->volume = pChan->targetVolume;
                    }
                    if (pChan->volume == pChan->targetVolume) {
                        pChan->volumeFadeStarted = 0;
                        if (pChan->volume == 0) {
                            if (pChan->buffer != 0) {
                                DsStopBuffer(pChan->buffer);
                                DsSetVolumeMillibels(pChan->buffer, dsVolume);
                            }
                            if (pChan->loopBuffer != 0) {
                                EnterCriticalSection((struct _RTL_CRITICAL_SECTION *)(g_SoundLoopBufferLock));
                                pChan->loopSwitchEnabled = 0;
                                DsStopBuffer(pChan->loopBuffer);
                                LeaveCriticalSection((struct _RTL_CRITICAL_SECTION *)(g_SoundLoopBufferLock));
                            }
                        }
                    }
                    effectIdx = g_SoundData.masterVolume * pChan->volume / 100;
                    dsVolume = g_SoundVolumeMillibelTable[effectIdx];
                    if (pChan->buffer != 0) {
                        DsSetVolumeMillibels(pChan->buffer, dsVolume);
                    }
                    if (pChan->loopBuffer != 0) {
                        EnterCriticalSection((struct _RTL_CRITICAL_SECTION *)(g_SoundLoopBufferLock));
                        pChan->loopSwitchEnabled = 0;
                        DsSetVolumeMillibels(pChan->loopBuffer, dsVolume);
                        LeaveCriticalSection((struct _RTL_CRITICAL_SECTION *)(g_SoundLoopBufferLock));
                    }
                    if (channelIndex == 0) {
                        EnterCriticalSection(&g_SoundLock);
                        if (g_SoundData.sfxStreamObject != 0) {
                            DsSetVolumeMillibels(g_SoundData.sfxStreamObject, dsVolume);
                        }
                        LeaveCriticalSection(&g_SoundLock);
                    }
                } else {
                    pChan->volumeFadeStarted = 1;
                }
            }
            if (pChan->pan != pChan->targetPan) {
                if (pChan->panFadeStarted != 0) {
                    pChan->panFadeElapsed += currentTick - g_SoundLastTick;
                    fadeStep = pChan->panFadeElapsed * pChan->panFadeRate / 1000;
                    if (fadeStep != 0) {
                        pChan->panFadeElapsed = 0;
                    }
                    pChan->pan += fadeStep;
                    if ((fadeStep < 0 && pChan->pan < pChan->targetPan) || (fadeStep > 0 && pChan->pan > pChan->targetPan)) {
                        pChan->pan = pChan->targetPan;
                    }
                    if (pChan->pan == pChan->targetPan) {
                        pChan->panFadeStarted = 0;
                    }
                    pan = g_SoundPanMillibelTable[pChan->pan] * g_SoundData.panInvertScale;
                    if (pChan->buffer != 0) {
                        DsSetPanMillibels(pChan->buffer, pan);
                    }
                    if (channelIndex == 0) {
                        EnterCriticalSection(&g_SoundLock);
                        if (g_SoundData.sfxStreamObject != 0) {
                            DsSetPanMillibels(g_SoundData.sfxStreamObject, pan);
                        }
                        LeaveCriticalSection(&g_SoundLock);
                    }
                } else {
                    pChan->panFadeStarted = 1;
                }
            }
            if (pChan->transpose != pChan->targetTranspose) {
                if (pChan->transposeFadeStarted != 0) {
                    pChan->transposeFadeElapsed += currentTick - g_SoundLastTick;
                    fadeStep = pChan->transposeFadeElapsed * pChan->transposeFadeRate / 1000;
                    if (fadeStep != 0) {
                        pChan->transposeFadeElapsed = 0;
                    }
                    pChan->transpose += fadeStep;
                    if ((fadeStep < 0 && pChan->transpose < pChan->targetTranspose) || (fadeStep > 0 && pChan->transpose > pChan->targetTranspose)) {
                        pChan->transpose = pChan->targetTranspose;
                    }
                    if (pChan->transpose == pChan->targetTranspose) {
                        pChan->transposeFadeStarted = 0;
                    }
                    freq = SfxFrequencyFromTranspose(pChan->transpose, pChan->sampleRate);
                    if (pChan->buffer != 0) {
                        DsSetFrequency(pChan->buffer, freq);
                    }
                    if (channelIndex == 0) {
                        EnterCriticalSection(&g_SoundLock);
                        if (g_SoundData.sfxStreamObject != 0) {
                            DsSetFrequency(g_SoundData.sfxStreamObject, freq);
                        }
                        LeaveCriticalSection(&g_SoundLock);
                    }
                } else {
                    pChan->transposeFadeStarted = 1;
                }
            }
            if (pChan->volume != pChan->targetVolume || (pChan->pan != pChan->targetPan || (pChan->transpose != pChan->targetTranspose || g_MusicVolume != g_MusicTargetVolume))) {
                g_SoundData.volumeFadeActiveFlag = 1;
            }
        }
    }
    g_SoundLastTick = currentTick;
}

/*
 * Plays sound effect `soundId` (1..751) on effect channel `channel` (1..5) at pan `pan` (0..127),
 * or with `soundId` 0 stops the channel. `SoundDispatchCommand`'s SFX play commands and
 * `SfxPreloadAndPlayOnChannel5` come here. It first waits until the battle sound-group preload
 * has finished group 15 (`SfxWaitForGroupLoaded`). A sound over 0x20000 bytes without its loop
 * flag is handed to `SfxPlayUpToFour`, which streams it.
 *
 * Stop: the channel's buffer and loop buffer are stopped and rewound, the loop switch is turned
 * off, the effect stream is closed, and the channel volume is set back to 127.
 *
 * Play: an id outside 1..751 or with an empty `audio.fmt` record is ignored. When the channel
 * already holds this sound, a looping sound that is still playing is left alone; otherwise the
 * buffer is rewound. A different sound replaces the channel's buffers: a duplicate of the cached
 * buffer, or, when it is not cached and no effect stream runs, the sound decoded from
 * `audio.dat` straight into the channel (`SfxLoadIntoBuffer`), with a loop buffer for records
 * with loop data. The buffer then gets the pan, the volume (master percent times channel volume
 * / 100, through the millibel table) and the frequency for the channel's transpose. A sound with
 * a loop point plays once while a one-shot timer (`SfxLoopSwitchTimerProc`) fires when playback
 * reaches the loop point, after loopInterval * 8 / (sample rate * 16 / 1000) ms, and switches to
 * the loop buffer; any other sound plays looping when the record's loop flag is set.
 *
 * Defects: the size and loop flag of record `soundId` - 1 are read before the id is checked, so
 * the stop call (0) reads the word before the table and an id above 751 reads past it; `channel`
 * is not checked, and a value outside 1..5 indexes outside the five channels.
 */
// FUNCTION: FF7 0x00745160
void __cdecl SfxPlayOnChannel(char pan, int soundId, int channel)
{
    SoundChannel *chan;
    int soundIdx;
    int streamBusy;
    int state;
    int startTick;
    SfxLoopInfo *loopInformation;
    int volumeIndex;
    int intervalMs;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    SfxWaitForGroupLoaded(0xF);
    chan = &g_SoundChannels.channels[channel - 1];
    if ((unsigned int)((SoundRecordEntry *)&g_SoundChannels.soundRecords)[soundId - 1].dataSize > (unsigned int)0x20000 && ((SoundRecordStreamFlag *)&g_SoundChannels.streamFlags)[soundId - 1].loopFlag == 0) {
        /* Retail call-site prototype (char, int, int, int, int) -> void preserved for /Od caller codegen. */
        ((void(__cdecl *)(char, int, int, int, int))SfxPlayUpToFour)((char)pan, soundId, 0, 0, 0);
    }
    if (soundId == 0) {
        if (chan->buffer != 0) {
            DsStopBuffer(chan->buffer);
            DsSetCurrentPosition(chan->buffer, 0);
        }
        if (chan->loopBuffer != 0) {
            EnterCriticalSection((LPCRITICAL_SECTION)(int)&g_SoundLoopBufferLock);
            chan->loopSwitchEnabled = 0;
            DsStopBuffer(chan->loopBuffer);
            DsSetCurrentPosition(chan->loopBuffer, 0);
            LeaveCriticalSection((LPCRITICAL_SECTION)(int)&g_SoundLoopBufferLock);
        }
        EnterCriticalSection(&g_SoundLock);
        if (g_SoundData.sfxStreamObject != 0) {
            SfxStreamStopServiceTimer();
            SfxStreamClose((struct SoundStreamPlayer *)&g_SoundData.sfxStreamObject);
        }
        LeaveCriticalSection(&g_SoundLock);
        chan->targetVolume = 0x7F;
        chan->volume = 0x7F;
    } else {
        soundIdx = soundId - 1;
        if (soundIdx < 0 || soundIdx > 750) {
            return;
        } else if (((SoundRecordEntry *)&g_SoundChannels.soundRecords)[soundIdx].dataSize == 0) {
            return;
        }
        if (chan->soundId == soundId && chan->buffer != 0) {
            if (((SoundRecordStreamFlag *)&g_SoundChannels.streamFlags)[soundIdx].loopFlag != 0) {
                ((LPDIRECTSOUNDBUFFER) chan->buffer)->GetStatus((LPDWORD)&state);
                if ((state & 1) != 0) {
                    return;
                }
                if (chan->loopBuffer != 0) {
                    ((LPDIRECTSOUNDBUFFER) chan->loopBuffer)->GetStatus((LPDWORD)&state);
                    if ((state & 1) != 0) {
                        return;
                    }
                    DsSetCurrentPosition(chan->loopBuffer, 0);
                }
            }
            DsStopBuffer(chan->buffer);
            DsSetCurrentPosition(chan->buffer, 0);
        } else {
            if (chan->buffer != 0) {
                DsReleaseBuffer(chan->buffer);
                chan->buffer = 0;
            }
            if (chan->loopBuffer != 0) {
                DsReleaseBuffer(chan->loopBuffer);
                chan->loopBuffer = 0;
            }
            if (g_SoundChannels.bufferHandles[soundIdx] != 0) {
                chan->buffer = DsDuplicateSoundBuffer(g_SoundChannels.bufferHandles[soundIdx]);
            } else {
                EnterCriticalSection(&g_SoundLock);
                streamBusy = g_SoundData.sfxStreamObject != 0;
                LeaveCriticalSection(&g_SoundLock);
                if (streamBusy != 0) {
                    return;
                }
                startTick = timeGetTime();
                SfxLoadIntoBuffer(soundId, (int)&chan->buffer);
                if (*(int *)((char *)&g_SoundChannels.recordLoopCount + soundIdx * 0x1C) != 0) {
                    SfxBuildLoopBuffer((SoundRecordEntry *)(soundIdx * 0x1C + (char *)&g_SoundChannels.soundRecords), chan);
                }
            }
            chan->soundId = soundId;
        }
        if (chan->buffer != 0) {
            chan->sampleRate = *(int *)((char *)*(int *)((char *)&g_SoundChannels.cachedSamples + soundIdx * 0x1C) + 4);
            SfxSetChannelPan((char)pan, channel);
            volumeIndex = g_SoundData.masterVolume * g_SoundChannels.channels[channel - 1].volume / 100;
            DsSetVolumeMillibels(chan->buffer, g_SoundVolumeMillibelTable[volumeIndex]);
            DsSetFrequency(chan->buffer, SfxFrequencyFromTranspose(g_SoundChannels.channels[channel - 1].transpose, chan->sampleRate));
            loopInformation = (SfxLoopInfo *)(soundIdx * 0x1C + (char *)&g_SoundChannels.soundRecords);
            if (loopInformation->loopInterval != 0 && chan->loopBuffer != 0) {
                DsPlayBuffer(chan->buffer, 0);
                intervalMs = loopInformation->loopInterval << 3;
                intervalMs = (unsigned int)intervalMs / (unsigned int)((unsigned int)(*(int *)((char *)loopInformation->waveFormat + 4) << 4) / (unsigned int)1000);
                DsSetVolumeMillibels(chan->loopBuffer, g_SoundVolumeMillibelTable[volumeIndex]);
                DsSetFrequency(chan->loopBuffer, SfxFrequencyFromTranspose(g_SoundChannels.channels[channel - 1].transpose, chan->sampleRate));
                chan->loopSwitchEnabled = 1;
                if (timeSetEvent(intervalMs, 5, SfxLoopSwitchTimerProc, (int)chan, 0) == 0) {
                    // STRING: FF7 0x00967bcc
                    Gfx_DebugPrint("ERROR: could not create loop callback\n");
                }
            } else {
                DsPlayBuffer(chan->buffer, ((SoundRecordStreamFlag *)&g_SoundChannels.streamFlags)[soundIdx].loopFlag);
            }
        }
    }
}

// Native SDK virtual calls live in sound_native.cpp with C linkage.

/*
 * Plays up to four sounds at one pan on channels 1 to 4. A single sound over 0x20000 bytes whose
 * stream flag is clear is streamed instead: the effect stream is opened on it, given its frequency,
 * pan and volume, and started.
 */
// FUNCTION: FF7 0x00745606
void __cdecl SfxPlayUpToFour(int pan, int soundId, int soundId2, int soundId3, int soundId4)
{
    struct SoundSfxStream *streamSlot;
    int volumeIndex;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    SfxStreamStopServiceTimer();
    SfxStreamCloseLocked();
    if (soundId2 != 0) {
        SfxWaitForGroupLoaded(0xF);
        SfxPlayOnChannel((char)pan, soundId, 1);
        SfxPlayOnChannel((char)pan, soundId2, 2);
        if (soundId3 != 0) {
            SfxPlayOnChannel((char)pan, soundId3, 3);
        }
        if (soundId4 != 0) {
            SfxPlayOnChannel((char)pan, soundId4, 4);
        }
    } else if ((unsigned int)((SoundRecordEntry *)&g_SoundChannels.soundRecords)[soundId - 1].dataSize <= (unsigned int)0x20000) {
        SfxPlayOnChannel((char)pan, soundId, 1);
    } else if (*(unsigned char *)((char *)&g_SoundChannels.streamFlags + (soundId - 1) * 0x1C) != 0) {
        SfxPlayOnChannel((char)pan, soundId, 1);
    } else {
        if (((SoundRecordEntry *)&g_SoundChannels.soundRecords)[soundId - 1].dataSize == 0) {
            return;
        }
        streamSlot = (struct SoundSfxStream *)&g_SoundData.sfxStreamObject;
        if (SfxStreamOpen(soundId, streamSlot) != 0) {
            g_SoundData.currentSoundId = soundId;
            DsReleaseBuffer(g_SoundChannels.channels[0].buffer);
            g_SoundChannels.channels[0].buffer = 0;
            DsReleaseBuffer(g_SoundChannels.channels[0].loopBuffer);
            g_SoundChannels.channels[0].loopBuffer = 0;
            g_SoundChannels.channels[0].sampleRate = *(int *)((char *)*(int *)((char *)&g_SoundChannels.cachedSamples + (soundId - 1) * 0x1C) + 4);
            DsSetFrequency(streamSlot->dsBuffer, SfxFrequencyFromTranspose(g_SoundChannels.channels[0].transpose, g_SoundChannels.channels[0].sampleRate));
            DsSetPanMillibels(g_SoundData.sfxStreamObject, g_SoundPanMillibelTable[pan & 0xFF] * g_SoundData.panInvertScale);
            volumeIndex = g_SoundData.masterVolume * g_SoundChannels.channels[0].volume / 100;
            DsSetVolumeMillibels(streamSlot->dsBuffer, g_SoundVolumeMillibelTable[volumeIndex]);
            DsPlayBuffer(streamSlot->dsBuffer, 1);
            g_SoundData.sfxStreamActiveFlag = 1;
            SfxStreamStartServiceTimer();
        }
    }
}

/*
 * Plays sound effect soundId on channel 5 at volume 0x40 when the sound system is up
 * (g_SoundChannels), loading it into a buffer first when its slot at g_SoundChannels.bufferHandles
 * is empty. Ids outside 1..751 are ignored. Declared int and returns nothing, as in the original; p
 * is computed and never used.
 */
// FUNCTION: FF7 0x0074580a
void __cdecl SfxPreloadAndPlayOnChannel5(int soundId)
{
    int recordPtr;
    int index;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    index = soundId - 1;
    if (index < 0 || index > 750) { /* empty first arm: lib/codegen.h */
    } else {
        recordPtr = index * 0x1C + (int)&g_SoundChannels.soundRecords;
        if (g_SoundChannels.bufferHandles[index] == 0) {
            SfxLoadIntoBuffer(soundId, 0);
        }
        SfxPlayOnChannel(0x40, soundId, 5);
    }
}

/*
 * Pauses all sound effects: the effect stream, both buffers of the five channels and the four group
 * slots.
 */
// FUNCTION: FF7 0x00745873
void __cdecl SfxPauseAll(void)
{
    int index;
    int handleIndex;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    // STRING: FF7 0x00967bf4
    Gfx_DebugPrint("pause_sound\n");
    EnterCriticalSection(&g_SoundLock);
    if (g_SoundData.sfxStreamObject != 0) {
        g_SoundData.sfxPauseAllFlag = 1;
        DsStopBuffer(g_SoundData.sfxStreamObject);
    }
    LeaveCriticalSection(&g_SoundLock);
    for (index = 0; index < 5; index++) {
        if (g_SoundChannels.channels[index].buffer != 0) {
            DsStopBuffer(g_SoundChannels.channels[index].buffer);
        }
        if (g_SoundChannels.channels[index].loopBuffer != 0) {
            DsStopBuffer(g_SoundChannels.channels[index].loopBuffer);
        }
    }
    for (index = 0; index < 4; index++) {
        if (g_SoundGroupSlots[index] != 0) {
            handleIndex = g_SoundGroupSlots[index] - 1;
            if (g_SoundChannels.bufferHandles[handleIndex] != 0 && g_SoundChannels.bufferHandles[handleIndex] != 0) {
                DsStopBuffer(g_SoundChannels.bufferHandles[handleIndex]);
            }
        }
    }
}

/*
 * Resumes what SfxPauseAll stopped, restarting only buffers whose play position is not at the
 * start.
 */
// FUNCTION: FF7 0x00745995
void __cdecl SfxResumeAll(void)
{
    SfxStreamObject *stream;
    int index;
    int soundIndex;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    // STRING: FF7 0x00967c04
    Gfx_DebugPrint("resume_sound ()\n");
    EnterCriticalSection(&g_SoundLock);
    stream = (SfxStreamObject *)&g_SoundData.sfxStreamObject;
    if (stream->bufferHandle != 0) {
        stream->isPaused = 0;
        DsPlayBuffer(stream->bufferHandle, 1);
    }
    LeaveCriticalSection(&g_SoundLock);
    for (index = 0; index < 5; index++) {
        if (g_SoundChannels.channels[index].buffer != 0) {
            soundIndex = g_SoundChannels.channels[index].soundId - 1;
            if (*(unsigned char *)((char *)&g_SoundChannels.streamFlags + soundIndex * 0x1C) != 0) {
                if (g_SoundChannels.channels[index].loopBuffer != 0) {
                    if (DsGetCurrentPlayPosition(g_SoundChannels.channels[index].loopBuffer) != 0) {
                        DsPlayBuffer(g_SoundChannels.channels[index].loopBuffer, 1);
                    }
                } else if (DsGetCurrentPlayPosition(g_SoundChannels.channels[index].buffer) != 0) {
                    DsPlayBuffer(g_SoundChannels.channels[index].buffer, 1);
                }
            } else if (DsGetCurrentPlayPosition(g_SoundChannels.channels[index].buffer) != 0) {
                DsPlayBuffer(g_SoundChannels.channels[index].buffer, 0);
            }
        }
    }
    for (index = 0; index < 4; index++) {
        if (g_SoundGroupSlots[index] != 0) {
            soundIndex = g_SoundGroupSlots[index] - 1;
            if (g_SoundChannels.bufferHandles[soundIndex] != 0) {
                if (DsGetCurrentPlayPosition(g_SoundChannels.bufferHandles[soundIndex]) != 0) {
                    DsPlayBuffer(g_SoundChannels.bufferHandles[soundIndex], 0);
                }
            }
        }
    }
}

/*
 * Silences every sound effect: `SoundDispatchCommand`'s SOUND_SFX_STOP_ALL, battle when it ends,
 * the world map's ambience switch, the main dispatcher before the ending movie and one field map
 * (0x185) call it. It stops the effect stream's service timer, then on each of the five channels
 * stops and rewinds the buffer, turns the loop switch off, stops and rewinds the loop buffer, and
 * sets the volume back to 127 (pan and transpose stay). The cached buffers of the four sound-group
 * slots are stopped and rewound, the effect stream is closed, and the current streamed sound id is
 * cleared. Nothing happens before the sound system is up.
 */
// FUNCTION: FF7 0x00745b64
void __cdecl SfxStopAll(void)
{
    int index;
    int handleIndex;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    // STRING: FF7 0x00967c18
    Gfx_DebugPrint("stop_sound\n");
    SfxStreamStopServiceTimer();
    for (index = 0; index < 5; index++) {
        if (g_SoundChannels.channels[index].buffer != 0) {
            DsStopBuffer(g_SoundChannels.channels[index].buffer);
            DsSetCurrentPosition(g_SoundChannels.channels[index].buffer, 0);
        }
        if (g_SoundChannels.channels[index].loopBuffer != 0) {
            EnterCriticalSection((struct _RTL_CRITICAL_SECTION *)(g_SoundLoopBufferLock));
            g_SoundChannels.channels[index].loopSwitchEnabled = 0;
            DsStopBuffer(g_SoundChannels.channels[index].loopBuffer);
            DsSetCurrentPosition(g_SoundChannels.channels[index].loopBuffer, 0);
            LeaveCriticalSection((struct _RTL_CRITICAL_SECTION *)(g_SoundLoopBufferLock));
        }
        g_SoundChannels.channels[index].targetVolume = 0x7F;
        g_SoundChannels.channels[index].volume = 0x7F;
    }
    for (index = 0; index < 4; index++) {
        if (g_SoundGroupSlots[index] != 0) {
            handleIndex = g_SoundGroupSlots[index] - 1;
            if (g_SoundChannels.bufferHandles[handleIndex] != 0 && g_SoundChannels.bufferHandles[handleIndex] != 0) {
                DsStopBuffer(g_SoundChannels.bufferHandles[handleIndex]);
                DsSetCurrentPosition(g_SoundChannels.bufferHandles[handleIndex], 0);
            }
        }
    }
    SfxStreamCloseLocked();
    g_SoundData.currentSoundId = 0;
}

/*
 * Loads sound `soundId` from audio.dat into a static DirectSound buffer, kept in the sound's cache
 * slot or in `destSlot`. Returns 1 when the sound is loaded, also when it already was.
 */
// FUNCTION: FF7 0x00745cf3
int __cdecl SfxLoadIntoBuffer(int soundId, int destSlot)
{
    int idx;
    int slotPtr;
    int target;

    idx = soundId - 1;
    if (idx < 0 || idx > 750) {
        return 0;
    } else if ((g_SoundChannels.initialized) == 0) {
        return 0;
    } else if (g_SoundChannels.bufferHandles[idx] != 0) {
        return 1;
    } else if (*(int *)((char *)&g_SoundChannels.soundRecords + idx * 0x1C) == 0) {
        return 0;
    } else {
        if (destSlot != 0) {
            target = destSlot;
        } else {
            target = idx * 4 + (int)&g_SoundChannels.bufferHandles;
        }
        slotPtr = target;
        SetFilePosition((struct FileHandle *)(g_SoundData.sfxDataFileHandle), *(int *)((char *)&g_SoundChannels.channelFileOffset + idx * 0x1C));
        *(int *)slotPtr = SfxDecodeToStaticBuffer((SoundSfxDecodeRecord *)(idx * 0x1C + (int)&g_SoundChannels.soundRecords), g_SoundData.sfxDataFileHandle);
        return *(int *)slotPtr != 0;
    }
}

/*
 * Frees the cached DirectSound buffer of sound effect `soundId` (1..751), which
 * `SfxLoadIntoBuffer` or the sound-group preload decoded from `audio.dat`. Battle effects and the
 * minigames (chocobo, coaster, condor, highway, snowboard) call it for the sounds they preloaded,
 * the minigames with group 15. It first waits until the preload job of sound
 * group `groupId` is done (`SfxWaitForGroupLoaded`), so a buffer still being loaded is not freed
 * under the loader. Channels that play a duplicate of the buffer keep their copy. Ids outside
 * 1..751 and calls before the sound system is up are ignored.
 */
// FUNCTION: FF7 0x00745dbb
void __cdecl Sfx_ReleaseCachedBuffer(int soundId, int groupId)
{
    int index;

    index = soundId - 1;
    if (index < 0 || index > 750) { /* empty first arm: lib/codegen.h */
    } else {
        if ((g_SoundChannels.initialized) == 0) {
            return;
        }
        SfxWaitForGroupLoaded(groupId);
        if (g_SoundChannels.bufferHandles[index] != 0) {
            DsReleaseBuffer(g_SoundChannels.bufferHandles[index]);
            g_SoundChannels.bufferHandles[index] = 0;
        }
    }
}

/*
 * Reads one audio.dat record and makes a static DirectSound buffer of it, decoding ADPCM (format 2)
 * to 16-bit PCM through ACM first. Returns the buffer, or 0.
 */
// FUNCTION: FF7 0x00745e22
int __cdecl SfxDecodeToStaticBuffer(SoundSfxDecodeRecord *record, int size)
{
    struct {
        int acmStream, bufferBytes, lockedBytes2;
        char streamHeader[84];
        char pcmFormat[20];
        int decodedSize, lockedPtr, srcBuf, soundBuffer, pcmBuf;
    } frame;
    frame.srcBuf = 0;
    frame.pcmBuf = 0;
    if (record->allocSize == 0) {
        return 0;
    } else {
        switch (*(unsigned short *)record[1].allocSize) {
        case SOUND_WAVE_FORMAT_PCM:
            break;
        case SOUND_WAVE_FORMAT_ADPCM:
            // STRING: FF7 0x00967c24
            frame.srcBuf = (int)(GameHeapAllocRaw(record->allocSize, "C:\\lib\\src\\sound\\sound.cpp", 0x4A4));
            if (frame.srcBuf != 0) {
                ReadFileDataPartial(record->allocSize, (void *)frame.srcBuf, (struct FileHandle *)size);
                AcmMakePcm16Format(record->allocSize, (void *)record[1].allocSize, &frame.pcmFormat);
                if (AcmStreamOpen((int)(&frame.acmStream), record[1].allocSize, (int)&frame.pcmFormat, 0) == 0) {
                    break;
                }
                frame.decodedSize = AcmGetDecodedSize(frame.acmStream, record->allocSize);
                if (frame.decodedSize == 0) {
                    break;
                }
                if (record->looping != 0) {
                    // STRING: FF7 0x00967c40
                    frame.pcmBuf = (int)(GameHeapAllocRaw(frame.decodedSize, "C:\\lib\\src\\sound\\sound.cpp", 0x4B9));
                    if (frame.pcmBuf != 0) {
                        AcmStreamPrepare(frame.acmStream, (SoundAcmStreamHeader *)(&frame.streamHeader), frame.srcBuf, record->allocSize, frame.pcmBuf, frame.decodedSize);
                        AcmStreamConvert(frame.acmStream, &frame.streamHeader, 0);
                        AcmStreamUnprepare(frame.acmStream, (int)(&frame.streamHeader));
                        AcmStreamClose(frame.acmStream);
                    }
                    frame.bufferBytes = record->endOffset;
                    frame.soundBuffer = DsCreateSoundBuffer(frame.bufferBytes, &frame.pcmFormat);
                    if (frame.soundBuffer != 0) {
                        if (DsLockBuffer(frame.soundBuffer, frame.bufferBytes, &frame.lockedPtr, &frame.lockedBytes2, 0, 0, 0) != 0) {
                            memcpy((void *)(frame.lockedPtr), (const void *)(frame.pcmBuf), frame.bufferBytes);
                            DsUnlockBuffer(frame.soundBuffer, frame.lockedPtr, frame.bufferBytes, 0, 0);
                        }
                    }
                } else {
                    frame.soundBuffer = DsCreateSoundBuffer(frame.decodedSize, &frame.pcmFormat);
                    if (frame.soundBuffer != 0) {
                        if (DsLockBuffer(frame.soundBuffer, frame.decodedSize, &frame.lockedPtr, &frame.lockedBytes2, 0, 0, 0) != 0) {
                            AcmStreamPrepare(frame.acmStream, (SoundAcmStreamHeader *)(&frame.streamHeader), frame.srcBuf, record->allocSize, frame.lockedPtr, frame.decodedSize);
                            AcmStreamConvert(frame.acmStream, &frame.streamHeader, 0);
                            AcmStreamUnprepare(frame.acmStream, (int)(&frame.streamHeader));
                            AcmStreamClose(frame.acmStream);
                            DsUnlockBuffer(frame.soundBuffer, frame.lockedPtr, frame.decodedSize, 0, 0);
                        }
                    }
                }
                if (frame.srcBuf != 0) {
                    // STRING: FF7 0x00967c5c
                    GameHeapFree((void *)(frame.srcBuf), "C:\\lib\\src\\sound\\sound.cpp", 0x4E0);
                }
                if (frame.pcmBuf != 0) {
                    // STRING: FF7 0x00967c78
                    GameHeapFree((void *)(frame.pcmBuf), "C:\\lib\\src\\sound\\sound.cpp", 0x4E1);
                }
                return frame.soundBuffer;
            }
            break;
        }
        if (frame.srcBuf != 0) {
            // STRING: FF7 0x00967c94
            GameHeapFree((void *)(frame.srcBuf), "C:\\lib\\src\\sound\\sound.cpp", 0x4E9);
        }
        return 0;
    }
}

/*
 * Opens audio.fmt and audio.dat (in the audio directory when one is set) and reads the 750 sound
 * records of audio.fmt. Returns 0 when either file is missing.
 */
// FUNCTION: FF7 0x00746139
int __cdecl SfxLoadFormatTable(void)
{
    struct Pcm16 {
        unsigned short format, channels;
        int rate, bytes;
        unsigned short block, bits;
    };
    struct {
        int file;
        struct {
            struct Pcm16 pcm;
            unsigned short pcmExtra;
        } format;
        union {
            unsigned short value;
            int alignedSlot;
        } extra;
        char dataPath[128];
        int index, data;
        char formatPath[128];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if ((char *)(int)g_SoundAudioDataDirectory) {
        strcpy(frame.formatPath, (char *)(int)g_SoundAudioDataDirectory);
        // STRING: FF7 0x00967cb0
        strcat(frame.formatPath, "\\audio.fmt");
        strcpy(frame.dataPath, (char *)(int)g_SoundAudioDataDirectory);
        // STRING: FF7 0x00967cbc
        strcat(frame.dataPath, "\\audio.dat");
    } else {
        // STRING: FF7 0x00967cc8
        strcpy(frame.formatPath, (char *)(int)"audio.fmt");
        // STRING: FF7 0x00967cd4
        strcpy(frame.dataPath, (char *)(int)"audio.dat");
    }
    frame.file = OpenFileSimple(FILE_OPEN_READ, frame.formatPath);
    if (frame.file == 0)
        return 0;
    g_SoundData.sfxDataFileHandle = OpenFileSimple(FILE_OPEN_READ, frame.dataPath);
    if (g_SoundData.sfxDataFileHandle == 0)
        return 0;
    for (frame.index = 0; frame.index < 750; frame.index++) {
        frame.extra.value = 0;
        ReadFileDataPartial(24, (void *)((int)&g_SoundChannels.soundRecords + frame.index * 28), (struct FileHandle *)frame.file);
        ReadFileDataPartial(18, &frame.format.pcm, (struct FileHandle *)frame.file);
        if (frame.format.pcm.format == 2)
            frame.extra.value = frame.format.pcmExtra;
        // STRING: FF7 0x00967ce0
        frame.data = (int)GameHeapAllocRaw(frame.extra.value + 18, "C:\\lib\\src\\sound\\sound.cpp", 0x515);
        if (frame.data == 0)
            return 0;
        *(struct Pcm16 *)frame.data = frame.format.pcm;
        *(unsigned short *)(frame.data + 16) = frame.extra.value;
        if (frame.extra.value != 0)
            ReadFileDataPartial(frame.extra.value, (void *)(frame.data + 18), (struct FileHandle *)frame.file);
        *(int *)((char *)&g_SoundChannels.cachedSamples + frame.index * 28) = frame.data;
    }
    CloseFileEx((struct FileHandle *)frame.file);
    return 1;
}

/*
 * Starts the 80 ms multimedia timer that refills the effect stream (SfxStreamServiceTimerProc),
 * unless it runs already.
 */
// FUNCTION: FF7 0x00746404
int __cdecl SfxStreamStartServiceTimer(void)
{
    if (g_SfxStreamServiceTimerActive != 0)
        return 1;
    g_SfxStreamServiceBusy = 0;
    g_SfxStreamServiceTimerId = timeSetEvent(0x50, 5, SfxStreamServiceTimerProc, 0, 1);
    g_SfxStreamServiceTimerActive = g_SfxStreamServiceTimerId != 0;
    return g_SfxStreamServiceTimerActive;
}

/*
 * Kills the effect stream's service timer.
 */
// FUNCTION: FF7 0x00746451
void __cdecl SfxStreamStopServiceTimer(void)
{
    if (g_SfxStreamServiceTimerId != 0) {
        timeKillEvent(g_SfxStreamServiceTimerId);
        g_SfxStreamServiceTimerId = 0;
    }
    g_SfxStreamServiceTimerActive = 0;
}

/*
 * Timer callback: refills the effect stream under its lock (guarded against re-entry) and stops the
 * timer when the stream is gone or done.
 */
// FUNCTION: FF7 0x0074647f
void __stdcall SfxStreamServiceTimerProc(unsigned int timer, unsigned int message, unsigned long user, unsigned long one, unsigned long two)
{
    if (InterlockedExchange(&g_SfxStreamServiceBusy, 1) != 0)
        return;
    EnterCriticalSection(&g_SoundLock);
    if (g_SoundData.sfxStreamObject != 0) {
        if (SfxStreamService((SoundStreamPlayer *)&g_SoundData.sfxStreamObject))
            SfxStreamStopServiceTimer();
    } else {
        SfxStreamStopServiceTimer();
    }
    LeaveCriticalSection(&g_SoundLock);
    InterlockedExchange(&g_SfxStreamServiceBusy, 0);
}

/*
 * Starts the 80 ms timer, with an 80 ms timer resolution, that refills the music wave stream,
 * unless it runs already.
 */
// FUNCTION: FF7 0x007464e2
int __cdecl MusicWaveStartServiceTimer(void)
{
    if (g_MusicServiceTimerActive != 0)
        return 1;
    if (timeBeginPeriod(0x50) == 0x61)
        return 0;
    g_MusicServiceTimerBusy = 0;
    g_MusicServiceTimerId = timeSetEvent(0x50, 5, MusicWaveStreamServiceTimerProc, 0, 1);
    g_MusicServiceTimerActive = g_MusicServiceTimerId != 0;
    return g_MusicServiceTimerActive;
}

/*
 * Kills the music wave stream's timer and ends its timer resolution.
 */
// FUNCTION: FF7 0x00746540
void __cdecl MusicWaveStopServiceTimer(void)
{
    if (g_MusicServiceTimerId != 0) {
        timeKillEvent(g_MusicServiceTimerId);
        timeEndPeriod(0x50);
    }
    g_MusicServiceTimerActive = 0;
}

/*
 * Timer callback: refills the music wave stream under its lock (guarded against re-entry) and stops
 * the timer when the stream is gone or done.
 */
// FUNCTION: FF7 0x0074656c
void __stdcall MusicWaveStreamServiceTimerProc(unsigned int timer, unsigned int message, unsigned long user, unsigned long one, unsigned long two)
{
    if (InterlockedExchange(&g_MusicServiceTimerBusy, 1) != 0)
        return;
    EnterCriticalSection((struct _RTL_CRITICAL_SECTION *)(g_SoundCriticalSectionMusic));
    if (g_SoundData.musicStreamObject != 0) {
        if (SfxStreamService((SoundStreamPlayer *)&g_SoundData.musicStreamObject))
            MusicWaveStopServiceTimer();
    } else {
        MusicWaveStopServiceTimer();
    }
    LeaveCriticalSection((struct _RTL_CRITICAL_SECTION *)(g_SoundCriticalSectionMusic));
    InterlockedExchange(&g_MusicServiceTimerBusy, 0);
}

/*
 * Sets a stream up for playback: for an ADPCM source it opens the ACM converter, sizes the decode
 * chunk and the stream buffer (a looping sound rounds them to its loop span) and allocates the
 * source buffer.
 */
// FUNCTION: FF7 0x007465cf
int __cdecl SfxStreamPrime(SoundStreamState *streamState, int fileHandle)
{
    struct {
        int decodedBytes, primePass, copyBytes, remainingBytes, loopSpan, totalDecoded;
        char pcmFormat[20];
        int chunkCount, primedOk, bytesRead, toReadBytes, primePassCount;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.primedOk = 0;
    frame.primePassCount = 3;
    switch (streamState->source->formatInfo->format) {
    case SOUND_WAVE_FORMAT_PCM:
        break;
    case SOUND_WAVE_FORMAT_ADPCM:
        AcmMakePcm16Format(streamState->source->dataBytes, (void *)streamState->source->sourceFormat, frame.pcmFormat);
        if (AcmStreamOpen((int)streamState + 0x44, streamState->source->sourceFormat, (int)frame.pcmFormat, 0) == 0) {
            break;
        }
        frame.chunkCount = 0x5622 / streamState->source->formatInfo->rateDivisor;
        if (0x5622 % streamState->source->formatInfo->rateDivisor != 0) {
            frame.chunkCount++;
        }
        streamState->chunkSize = frame.chunkCount * streamState->source->formatInfo->rateDivisor;
        streamState->bufferSize = streamState->chunkSize << 2;
        if (streamState->state != 1 && streamState->source->looping != 0 && streamState->source->startOffset != 0) {
            frame.loopSpan = streamState->source->endOffset - streamState->source->startOffset;
            if (frame.loopSpan < streamState->chunkSize) {
                streamState->chunkSize = frame.loopSpan * (streamState->chunkSize / frame.loopSpan);
            } else {
                streamState->chunkSize = frame.loopSpan;
            }
            streamState->bufferSize = streamState->chunkSize << 1;
            while (streamState->bufferSize < streamState->source->endOffset) {
                streamState->bufferSize += streamState->chunkSize;
            }
            streamState->srcBytes = streamState->source->dataBytes;
            streamState->source->filePos = GetFilePosition((struct FileHandle *)fileHandle);
            // STRING: FF7 0x00967cfc
            streamState->srcBuffer = (int)(GameHeapAllocRaw(streamState->srcBytes, "C:\\lib\\src\\sound\\sound.cpp", 0x5D6));
            if (streamState->srcBuffer == 0) {
                break;
            }
            // STRING: FF7 0x00967d18
            streamState->pcmBuffer = (int)(GameHeapAllocRaw(streamState->bufferSize, "C:\\lib\\src\\sound\\sound.cpp", 0x5D9));
            if (streamState->pcmBuffer == 0) {
                break;
            }
            streamState->dsBuffer = DsCreateSoundBuffer(streamState->bufferSize, frame.pcmFormat);
            if (streamState == 0) {
                break;
            }
            frame.primedOk = 1;
            AcmStreamPrepare(streamState->acmStream, (SoundAcmStreamHeader *)((char *)streamState + 0x48), streamState->srcBuffer, streamState->srcBytes, streamState->pcmBuffer, streamState->bufferSize);
            frame.bytesRead = ReadFileDataPartial(streamState->srcBytes, (void *)streamState->srcBuffer, (struct FileHandle *)fileHandle);
            AcmStreamConvert(streamState->acmStream, (void *)((int)streamState + 0x48), 0);
            AcmStreamUnprepare(streamState->acmStream, (int)streamState + 0x48);
            AcmStreamClose(streamState->acmStream);
            DsWriteIntoBufferRegion((char *)(streamState->pcmBuffer), streamState->source->endOffset, (struct IDirectSoundBuffer *)(streamState->dsBuffer), 0, streamState->source->endOffset);
            streamState->writePos = streamState->source->endOffset;
            frame.remainingBytes = streamState->bufferSize - streamState->writePos;
            streamState->readPos = streamState->source->endOffset;
            while (frame.remainingBytes != 0) {
                frame.copyBytes = frame.loopSpan;
                if (frame.remainingBytes < frame.copyBytes) {
                    frame.copyBytes = frame.remainingBytes;
                    streamState->readPos = frame.loopSpan - frame.remainingBytes;
                }
                DsWriteIntoBufferRegion((char *)(streamState->pcmBuffer + streamState->source->startOffset), frame.copyBytes, (struct IDirectSoundBuffer *)(streamState->dsBuffer), streamState->writePos, frame.copyBytes);
                frame.remainingBytes -= frame.copyBytes;
                streamState->writePos += frame.copyBytes;
            }
        } else {
            frame.totalDecoded = AcmGetDecodedSize(streamState->acmStream, streamState->source->dataBytes);
            if (frame.totalDecoded == 0) {
                break;
            }
            streamState->srcBytes = AcmGetSourceSizeForDecoded(streamState->acmStream, streamState->chunkSize);
            if (streamState->srcBytes == 0) {
                break;
            }
            if (streamState->srcBytes > streamState->source->dataBytes) {
                streamState->srcBytes = streamState->source->dataBytes;
                streamState->primed = 1;
            }
            // STRING: FF7 0x00967d34
            streamState->srcBuffer = (int)(GameHeapAllocRaw(streamState->srcBytes, "C:\\lib\\src\\sound\\sound.cpp", 0x61E));
            if (streamState->srcBuffer == 0) {
                break;
            }
            // STRING: FF7 0x00967d50
            streamState->pcmBuffer = (int)(GameHeapAllocRaw(streamState->chunkSize, "C:\\lib\\src\\sound\\sound.cpp", 0x622));
            if (streamState->pcmBuffer == 0) {
                break;
            }
            streamState->dsBuffer = DsCreateSoundBuffer(streamState->bufferSize, frame.pcmFormat);
            if (streamState != 0) {
                AcmStreamPrepare(streamState->acmStream, (SoundAcmStreamHeader *)((char *)streamState + 0x48), streamState->srcBuffer, streamState->srcBytes, streamState->pcmBuffer, streamState->chunkSize);
                if (streamState->source->looping != 0) {
                    streamState->source->filePos = 0;
                }
                for (frame.primePass = 0; frame.primePass < frame.primePassCount; frame.primePass++) {
                    frame.toReadBytes = streamState->srcBytes;
                    if (streamState->readPos < streamState->srcBytes) {
                        frame.toReadBytes = streamState->readPos;
                        memset((void *)(streamState->srcBuffer + frame.toReadBytes), 0, streamState->srcBytes - frame.toReadBytes);
                        streamState->primed = 1;
                    }
                    streamState[1].filePos = GetFilePosition((struct FileHandle *)fileHandle);
                    frame.bytesRead = ReadFileDataPartial(frame.toReadBytes, (void *)streamState->srcBuffer, (struct FileHandle *)fileHandle);
                    streamState->readPos -= frame.bytesRead;
                    AcmStreamConvert(streamState->acmStream, (void *)((int)streamState + 0x48), 0);
                    frame.decodedBytes = streamState->pcmBytesDecoded;
                    DsWriteIntoBufferRegion((char *)(streamState->pcmBuffer), frame.decodedBytes, (struct IDirectSoundBuffer *)(streamState->dsBuffer), streamState->writePos, frame.decodedBytes);
                    streamState->bytesWritten += frame.decodedBytes;
                    streamState->writePos += frame.decodedBytes;
                    streamState->lastWritePos += frame.decodedBytes;
                    if (streamState->source->looping != 0 && streamState->bytesWritten >= streamState->source->startOffset && streamState->source->filePos == 0) {
                        streamState->source->filePos = streamState[1].filePos;
                        streamState[1].bufferSize = streamState->source->startOffset - (streamState->bytesWritten - frame.decodedBytes);
                        streamState[1].chunkSize = streamState->bytesWritten - frame.decodedBytes;
                    }
                    if (streamState->primed != 0) {
                        streamState->chunkDecoded = 1;
                        streamState->lastWritePos = streamState->writePos;
                        AcmStreamUnprepare(streamState->acmStream, (int)streamState + 0x48);
                        AcmStreamClose(streamState->acmStream);
                        break;
                    }
                }
                frame.toReadBytes = streamState->srcBytes;
                if (streamState->readPos < streamState->srcBytes) {
                    frame.toReadBytes = streamState->readPos;
                    memset((void *)(streamState->srcBuffer + frame.toReadBytes), 0, streamState->srcBytes - frame.toReadBytes);
                    streamState->primed = 1;
                }
                streamState[1].filePos = GetFilePosition((struct FileHandle *)fileHandle);
                frame.bytesRead = ReadFileDataPartial(frame.toReadBytes, (void *)streamState->srcBuffer, (struct FileHandle *)fileHandle);
                streamState->readPos -= frame.bytesRead;
                AcmStreamConvert(streamState->acmStream, (void *)((int)streamState + 0x48), 0);
                frame.primedOk = 1;
            }
        }
    }
    return frame.primedOk;
}

// Closes the sound stream. It clears isOpen (the playing flag), releases the DirectSound buffer
// dsBuffer and clears it, then frees the heap blocks srcBuffer and pcmBuffer and clears each
// pointer after its free.
// FUNCTION: FF7 0x00746d3d
void __cdecl SfxStreamClose(SoundStreamPlayer *stream)
{
    stream->isOpen = 0;
    if (stream->dsBuffer != 0) {
        DsReleaseBuffer(stream->dsBuffer);
        stream->dsBuffer = 0;
    }
    if (stream->srcBuffer != 0) {
        // STRING: FF7 0x00967d6c
        GameHeapFree((void *)(stream->srcBuffer), "C:\\lib\\src\\sound\\sound.cpp", 0x692);
        stream->srcBuffer = 0;
    }
    if (stream->pcmBuffer != 0) {
        // STRING: FF7 0x00967d88
        GameHeapFree((void *)(stream->pcmBuffer), "C:\\lib\\src\\sound\\sound.cpp", 0x698);
        stream->pcmBuffer = 0;
    }
}

/*
 * Points an effect stream at sound `index`'s record, clears its state, seeks audio.dat to the sound
 * and primes the stream.
 */
// FUNCTION: FF7 0x00746dc3
int __cdecl SfxStreamOpen(int index, SoundSfxStream *stream)
{
    int result;

    result = 0;
    if (index > 0 && index < 750) {
        stream->dsBuffer = 0;
        stream->entryPtr = (index - 1) * 0x1C + (int)&g_SoundChannels.soundRecords;
        stream->chunkDecoded = 0;
        stream->bytesToRead = ((SoundSfxEntry *)(stream->entryPtr))->dataSize;
        stream->writePos = 0;
        stream->cursor = 0;
        stream[1].bytesQueued = 0;
        stream->bytesQueued = 0;
        stream->state = 0;
        stream->loopTailDone = 0;
        stream->looping = ((SoundSfxEntry *)(stream->entryPtr))->looping;
        stream[1].loopTailDone = 0;
        stream[1].writePos = 0;
        stream->isPaused = 0;
        SetFilePosition((struct FileHandle *)(g_SoundData.sfxDataFileHandle), ((SoundSfxEntry *)(stream->entryPtr))->filePosition);
        result = SfxStreamPrime((SoundStreamState *)stream, g_SoundData.sfxDataFileHandle);
    }
    return result;
}

/*
 * Opens the music wave file (g_SoundChannels.wavePath), parses its WAV header and primes the music
 * stream. Returns 0 when the file is missing or not a WAV file.
 */
// FUNCTION: FF7 0x00746ec4
int __cdecl MusicWaveStreamOpen(void)
{
    int result;
    SoundWaveStream *stream;

    result = 0;
    stream = (SoundWaveStream *)&g_SoundData.musicStreamObject;
    if (g_SoundData.musicStreamBufferPtr != 0) {
        // STRING: FF7 0x00967da4
        GameHeapFree((void *)(g_SoundData.musicStreamBufferPtr), "C:\\lib\\src\\sound\\sound.cpp", 0x6C2);
    }
    g_SoundData.musicStreamBufferPtr = 0;
    if (g_SoundData.musicFileHandle != 0) {
        CloseFileEx((struct FileHandle *)g_SoundData.musicFileHandle);
        g_SoundData.musicFileHandle = 0;
    }
    g_SoundData.musicFileHandle = OpenFileSimple(FILE_OPEN_READ, &g_SoundChannels.wavePath);
    if (g_SoundData.musicFileHandle == 0) {
        return 0;
    } else {
        stream->wavBuffer = (int)&g_MusicWavHeaderBuffer;
        if (MusicWaveParseWavHeader((SoundWaveInfo *)stream->wavBuffer) == 0) {
            CloseFileEx((struct FileHandle *)g_SoundData.musicFileHandle);
            return 0;
        } else {
            stream->dsBuffer = 0;
            stream->chunkDecoded = 0;
            stream->dataSize = ((SoundWavBufferHeader *)(stream->wavBuffer))->dataSize;
            stream->writePos = 0;
            stream->cursor = 0;
            stream->primed = 0;
            stream->bytesWritten = 0;
            stream->bytesQueued = 0;
            stream->state = 1;
            stream->loopTailDone = 0;
            stream->looping = ((SoundWavBufferHeader *)(stream->wavBuffer))->looping;
            stream->loopWrapped = 0;
            stream->bytesWritten = 0;
            result = SfxStreamPrime((SoundStreamState *)stream, g_SoundData.musicFileHandle);
            return result;
        }
    }
}

/*
 * Decodes the next chunk of a stream's source into its DirectSound buffer, seeking back to the loop
 * point of a looping sound.
 */
// FUNCTION: FF7 0x00747031
int __cdecl SfxStreamDecodeChunk(SoundStreamPlayer *stream, int fileHandle)
{
    int written;
    int chunkBytes;
    int readLen;
    int got;
    int pcmPointer;

    switch (*(unsigned short *)stream->source->formatData) {
    case SOUND_WAVE_FORMAT_PCM:
        break;
    case SOUND_WAVE_FORMAT_ADPCM:
        pcmPointer = stream->pcmBuffer;
        chunkBytes = stream->chunkBytes;
        written = chunkBytes;
        stream[1].looping += chunkBytes;
        if (stream->source->filePosition == 0 && stream[1].looping >= stream->source->loopOffset) {
            stream->source->filePosition = stream[1].srcBuffer;
            stream[1].pcmBuffer = stream->source->loopOffset - (stream[1].looping - chunkBytes);
            stream[1].acmStream = stream[1].looping - chunkBytes;
        }
        if (stream->looping != 0) {
            if (stream[1].cursor != 0) {
                written = stream->source->decodedSize - (stream[1].looping - chunkBytes);
            }
            if (stream[1].readLength != 0) {
                written = chunkBytes - stream[1].pcmBuffer;
                pcmPointer += stream[1].pcmBuffer;
                stream[1].readLength = 0;
            }
        }
        DsWriteIntoBufferRegion((char *)(pcmPointer), chunkBytes, (struct IDirectSoundBuffer *)(stream->dsBuffer), stream->writePos, written);
        stream->bytesQueued += written;
        if (stream[1].cursor != 0) {
            AcmStreamUnprepare(stream->acmStream, (int)stream + 0x48);
            if (stream->looping != 0) {
                SetFilePosition((struct FileHandle *)(fileHandle), stream->source->filePosition);
                stream->bytesToRead = stream->source->dataSize - (stream->source->filePosition - stream->source->dataStart);
                stream[1].cursor = 0;
                stream[1].looping = stream[1].acmStream;
                stream[1].readLength = 1;
                AcmStreamPrepare(stream->acmStream, (SoundAcmStreamHeader *)((char *)stream + 0x48), stream->srcBuffer, stream->readLength, stream->pcmBuffer, stream->chunkSize);
            } else {
                AcmStreamClose(stream->acmStream);
                stream->chunkDecoded = 1;
                break;
            }
        }
        readLen = stream->readLength;
        if (stream->bytesToRead < stream->readLength) {
            readLen = stream->bytesToRead;
            memset((void *)(stream->srcBuffer + readLen), 0, stream->readLength - readLen);
        }
        stream[1].srcBuffer = GetFilePosition((struct FileHandle *)fileHandle);
        got = ReadFileDataPartial(readLen, (void *)stream->srcBuffer, (struct FileHandle *)fileHandle);
        stream->bytesToRead -= got;
        if (stream->bytesToRead == 0) {
            stream[1].cursor = 1;
        }
        AcmStreamConvert(stream->acmStream, (void *)((int)stream + 0x48), 0);
    }
    stream->writePos += written;
    if (stream->writePos >= stream->bufferSize) {
        stream->writePos -= stream->bufferSize;
    }
    return written;
}

/*
 * Audio routine DsWriteIntoBufferRegion.
 */
// FUNCTION: FF7 0x00747357
void __cdecl DsWriteIntoBufferRegion(char *source, int sourceBytes, IDirectSoundBuffer *buffer, int offset, int bytes)
{
    DWORD size2;
    char *ptr1;
    DWORD size1;
    char *ptr2;
    if (sourceBytes == 0 && bytes == 0)
        return;
    // STRING: FF7 0x00967dc0
    if (!DsCheckError(buffer->Lock(offset, bytes, (void **)&ptr1, &size1, (void **)&ptr2, &size2, 0), "C:\\lib\\src\\sound\\sound.cpp", 0x773))
        return;
    if (sourceBytes < bytes) {
        if ((int)size1 >= bytes) {
            memcpy(ptr1, source, sourceBytes);
            memset(ptr1 + sourceBytes, 0, bytes - sourceBytes);
            if (ptr2)
                memset(ptr2, 0, size2);
        } else {
            memcpy(ptr1, source, size1);
            if (ptr2) {
                memcpy(ptr2, source + size1, sourceBytes - size1);
                memset(ptr2 + (sourceBytes - size1), 0, bytes - sourceBytes);
            }
        }
    } else {
        memcpy(ptr1, source, size1);
        if (ptr2)
            memcpy(ptr2, source + size1, size2);
    }
    // STRING: FF7 0x00967ddc
    DsCheckError(buffer->Unlock(ptr1, size1, ptr2, size2), "C:\\lib\\src\\sound\\sound.cpp", 0x791);
}

// Native compiler-generated implementation lives in sound_native.cpp.

/*
 * Reads the RIFF/WAVE header of the music file: checks the RIFF, WAVE and fmt tags and copies the
 * format into a new heap buffer.
 */
// FUNCTION: FF7 0x007474e0
int __cdecl MusicWaveParseWavHeader(SoundWaveInfo *waveInfo)
{
    struct Wave16 {
        unsigned short format, channels;
        int sampleRate, byteRate;
        unsigned short blockAlign, bits;
    };
    struct {
        int scale, riffSize;
        union {
            unsigned short value;
            int alignedSlot;
        } extra;
        int riffTag, chunkTag, chunkSize;
        struct Wave16 wave;
        int data;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.extra.value = 0;
    frame.data = 0;
    if (g_SoundData.musicFileHandle != 0) {
        ReadFileDataPartial(4, &frame.riffTag, (struct FileHandle *)g_SoundData.musicFileHandle);
        if (frame.riffTag != MAKE_FOURCC('R', 'I', 'F', 'F'))
            return 0;
        ReadFileDataPartial(4, &frame.riffSize, (struct FileHandle *)g_SoundData.musicFileHandle);
        frame.riffSize += 4;
        ReadFileDataPartial(4, &frame.riffTag, (struct FileHandle *)g_SoundData.musicFileHandle);
        if (frame.riffTag != MAKE_FOURCC('W', 'A', 'V', 'E'))
            return 0;
        ReadFileDataPartial(4, &frame.chunkTag, (struct FileHandle *)g_SoundData.musicFileHandle);
        if (frame.chunkTag != MAKE_FOURCC('f', 'm', 't', ' '))
            return 0;
        ReadFileDataPartial(4, &frame.chunkSize, (struct FileHandle *)g_SoundData.musicFileHandle);
        ReadFileDataPartial(16, &frame.wave, (struct FileHandle *)g_SoundData.musicFileHandle);
        switch (frame.wave.format) {
        case SOUND_WAVE_FORMAT_PCM:
            // STRING: FF7 0x00967df8
            waveInfo->buffer = (int)GameHeapAllocRaw(0x12, "C:\\lib\\src\\sound\\sound.cpp", 0x7b4);
            if (waveInfo->buffer != 0) {
                break;
            } else {
                return 0;
            }
        case SOUND_WAVE_FORMAT_ADPCM:
            ReadFileDataPartial(2, &frame.extra.value, (struct FileHandle *)g_SoundData.musicFileHandle);
            // STRING: FF7 0x00967e14
            waveInfo->buffer = (int)GameHeapAllocRaw(frame.extra.value + 0x12, "C:\\lib\\src\\sound\\sound.cpp", 0x7bb);
            if (waveInfo->buffer == 0)
                return 0;
            ((MusicWaveInfoHeader *)(waveInfo->buffer))->extraValue = frame.extra.value;
            ReadFileDataPartial(frame.extra.value, (void *)(waveInfo->buffer + 0x12), (struct FileHandle *)g_SoundData.musicFileHandle);
            ReadFileDataPartial(4, &frame.chunkTag, (struct FileHandle *)g_SoundData.musicFileHandle);
            ReadFileDataPartial(4, &frame.chunkSize, (struct FileHandle *)g_SoundData.musicFileHandle);
            // STRING: FF7 0x00967e30
            frame.data = (int)GameHeapAllocRaw(frame.chunkSize, "C:\\lib\\src\\sound\\sound.cpp", 0x7c2);
            if (frame.data != 0)
                ReadFileDataPartial(frame.chunkSize, (void *)frame.data, (struct FileHandle *)g_SoundData.musicFileHandle);
            // STRING: FF7 0x00967e4c
            GameHeapFree((void *)frame.data, "C:\\lib\\src\\sound\\sound.cpp", 0x7c6);
            break;
        }
        *(struct Wave16 *)waveInfo->buffer = frame.wave;
        ReadFileDataPartial(4, &frame.chunkTag, (struct FileHandle *)g_SoundData.musicFileHandle);
        if (frame.chunkTag != MAKE_FOURCC('d', 'a', 't', 'a'))
            return 0;
        ReadFileDataPartial(4, &frame.chunkSize, (struct FileHandle *)g_SoundData.musicFileHandle);
        waveInfo->chunkSize = frame.chunkSize;
        waveInfo->filePos = GetFilePosition((struct FileHandle *)g_SoundData.musicFileHandle);
        SetFilePosition((struct FileHandle *)(g_SoundData.musicFileHandle), waveInfo->filePos + frame.chunkSize);
        waveInfo->ready = 0;
        while (ReadFileDataPartial(4, &frame.chunkTag, (struct FileHandle *)g_SoundData.musicFileHandle) != 0) {
            ReadFileDataPartial(4, &frame.chunkSize, (struct FileHandle *)g_SoundData.musicFileHandle);
            // STRING: FF7 0x00967e68
            frame.data = (int)GameHeapAllocRaw(frame.chunkSize, "C:\\lib\\src\\sound\\sound.cpp", 0x7d8);
            ReadFileDataPartial(frame.chunkSize, (void *)frame.data, (struct FileHandle *)g_SoundData.musicFileHandle);
            if (frame.chunkTag == MAKE_FOURCC('s', 'm', 'p', 'l')) {
                waveInfo->ready = 1;
                waveInfo->dataSize = *(int *)(frame.data + 0x2c);
                waveInfo->rawSize = *(int *)(frame.data + 0x30);
                switch (*(unsigned short *)waveInfo->buffer) {
                case SOUND_WAVE_FORMAT_PCM:
                    waveInfo->dataSize = ((MusicWaveInfoHeader *)(waveInfo->buffer))->blockAlign * waveInfo->dataSize;
                    break;
                case SOUND_WAVE_FORMAT_ADPCM:
                    frame.scale = ((MusicWaveInfoHeader *)(waveInfo->buffer))->channels << 1;
                    waveInfo->scaledSize = waveInfo->dataSize * frame.scale;
                    waveInfo->rawSize = waveInfo->rawSize * frame.scale;
                    waveInfo->dataSize = 0;
                    break;
                }
            }
            // STRING: FF7 0x00967e84
            GameHeapFree((void *)frame.data, "C:\\lib\\src\\sound\\sound.cpp", 0x7f1);
        }
        SetFilePosition((struct FileHandle *)(g_SoundData.musicFileHandle), waveInfo->filePos);
        return 1;
    } else {
        return 0;
    }
}

/*
 * Native cdecl ABI: two DWORD arguments; all three original callers clean eight bytes.
 * DE6214 is a byte view within the reset owner, with no filename capacity claim.
 * When sound is enabled and track is 1 through 0x62, sets the track-1 flag and preserves the
 * special mutual exclusion of active tracks 1 and 2. Logs and returns for the already-active
 * track; otherwise stops the service timer, closes the stream and stores the requested track
 * before checking its filename entry. Builds the wave path with the fixed directory pointer branch
 * and opens it. On success applies global percentage volume through the millibel table, marks
 * playback active, handles the pending transition flag, loops the buffer and starts its timer. On
 * failure closes the stream handle. The `unused` parameter is ignored.
 */
// FUNCTION: FF7 0x007478bd
void __cdecl MusicPlayWaveTrack(int track, int unused)
{
    int vol;

    if ((g_SoundChannels.initialized) == 0 || track > SOUND_SONG_LAST || track <= 0) {
        return;
    }
    switch (track) {
    case SOUND_SONG_OA:
        g_MusicTrack1Requested = 1;
        if (g_SoundData.musicPlayingFlag != 0 && g_SoundChannels.track == SOUND_SONG_OB) {
            return;
        }
        break;
    case SOUND_SONG_OB:
        if (g_SoundData.musicPlayingFlag != 0 && g_SoundChannels.track == SOUND_SONG_OA) {
            return;
        }
        break;
    }
    if (g_SoundData.musicPlayingFlag != 0 && g_SoundChannels.track == track) {
        // STRING: FF7 0x00967ea0
        Gfx_DebugPrint("song is already playing...\n");
        return;
    }
    MusicWaveStopServiceTimer();
    MusicWaveStreamCloseLocked();
    g_SoundChannels.track = track;
    if (((int *)&g_SoundChannels.trackNames)[track] == 0) {
        return;
    }
    if (g_SoundMusicWaveDirectory) {
        strcpy(&g_SoundChannels.wavePath, g_SoundMusicWaveDirectory);
        // STRING: FF7 0x00967ebc
        strcat(&g_SoundChannels.wavePath, "\\");
        strcat(&g_SoundChannels.wavePath, (char *)((int *)&g_SoundChannels.trackNames)[track]);
    } else {
        strcpy(&g_SoundChannels.wavePath, (char *)((int *)&g_SoundChannels.trackNames)[track]);
    }
    // STRING: FF7 0x00967ec0
    strcat(&g_SoundChannels.wavePath, ".wav");
    if (MusicWaveStreamOpen()) {
        g_MusicCrossPlayActive = 0;
        vol = g_SoundData.musicMasterVolumePercent * g_MusicVolume / 100;
        vol = vol;
        DsSetVolumeMillibels(g_SoundData.musicStreamObject, g_SoundVolumeMillibelTable[vol]);
        g_SoundData.musicPlayingFlag = 1;
        if (g_MusicPlayAfterFade != 0) {
            g_MusicPlayAfterFade = 0;
            MusicWaveSetVolumeTrans(0x7F, *(unsigned char *)&g_MusicFadeDuration);
        }
        DsPlayBuffer(g_SoundData.musicStreamObject, 1);
        MusicWaveStartServiceTimer();
    } else {
        SfxStreamClose((struct SoundStreamPlayer *)&g_SoundData.musicStreamObject);
    }
}

/*
 * Stops streamed wave music when the sound system is up (g_SoundChannels): stops the service timer,
 * closes the stream, clears g_MusicPlayAfterFade and g_MusicCrossPlayActive and closes the open file
 * g_SoundData.musicFileHandle.
 */
// FUNCTION: FF7 0x00747aed
void __cdecl MusicWaveStop(void)
{
    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    MusicWaveStopServiceTimer();
    MusicWaveStreamCloseLocked();
    g_MusicPlayAfterFade = 0;
    g_MusicCrossPlayActive = 0;
    if (g_SoundData.musicFileHandle != 0) {
        CloseFileEx((struct FileHandle *)(g_SoundData.musicFileHandle));
        g_SoundData.musicFileHandle = 0;
    }
}

/*
 * Returns zero when the sound system enabled flag g_SoundChannels is clear; otherwise returns
 * g_SoundData.musicPlayingFlag.
 */
// FUNCTION: FF7 0x00747b3c
int __cdecl MusicWaveIsPlaying(void)
{
    if ((g_SoundChannels.initialized) == 0) {
        return 0;
    } else {
        return g_SoundData.musicPlayingFlag;
    }
}

/*
 * When g_SoundChannels is enabled, enters g_SoundCriticalSectionMusic's critical section and, for a
 * nonzero music buffer g_SoundData.musicStreamObject, sets g_SoundData.musicPausedFlag to 1 and
 * stops the buffer, then leaves the critical section.
 */
// FUNCTION: FF7 0x00747b53
void __cdecl MusicWavePause(void)
{
    if ((g_SoundChannels.initialized) == 0)
        return;
    EnterCriticalSection((struct _RTL_CRITICAL_SECTION *)(&g_SoundCriticalSectionMusic));
    if (g_SoundData.musicStreamObject != 0) {
        g_SoundData.musicPausedFlag = 1;
        DsStopBuffer(g_SoundData.musicStreamObject);
    }
    LeaveCriticalSection((struct _RTL_CRITICAL_SECTION *)(&g_SoundCriticalSectionMusic));
}

/*
 * When g_SoundChannels is enabled, enters g_SoundCriticalSectionMusic's critical section and, for a
 * nonzero music buffer g_SoundData.musicStreamObject, plays it looping and clears
 * g_SoundData.musicPausedFlag, then leaves the critical section.
 */
// FUNCTION: FF7 0x00747b9a
void __cdecl MusicWaveResume(void)
{
    if ((g_SoundChannels.initialized) == 0)
        return;
    EnterCriticalSection((struct _RTL_CRITICAL_SECTION *)(&g_SoundCriticalSectionMusic));
    if (g_SoundData.musicStreamObject != 0) {
        DsPlayBuffer(g_SoundData.musicStreamObject, 1);
        g_SoundData.musicPausedFlag = 0;
    }
    LeaveCriticalSection((struct _RTL_CRITICAL_SECTION *)(&g_SoundCriticalSectionMusic));
}

/*
 * Applies volume to sound channels 1 through 5 through SfxSetChannelVolume. Channel 0 is left alone.
 */
// FUNCTION: FF7 0x00747be3
void __cdecl SfxSetAllChannelsVolume(char volume)
{
    int channel;

    for (channel = 1; channel <= 5; channel++) {
        SfxSetChannelVolume((char)volume, channel);
    }
}

/*
 * Calls SfxSetChannelVolumeTrans(volume,channel,transition) for all five channels 1..5, preserving char-
 * width forwarding.
 */
// FUNCTION: FF7 0x00747c15
void __cdecl SfxSetAllChannelsVolumeTrans(char volume, char transition)
{
    int channel;

    for (channel = 1; channel <= 5; channel++) {
        SfxSetChannelVolumeTrans((char)volume, channel, (char)transition);
    }
}

/*
 * When sound is enabled and channel is 1..5, clamps volume to 127, scales by g_SoundData.masterVolume/100,
 * resets that channel's volume transition and stores current/target volume. Applies the
 * g_SoundVolumeMillibelTable millibel lookup to its available buffers, with channel 1's shared buffer protected
 * by g_SoundLock, and logs the volume.
 */
// FUNCTION: FF7 0x00747c4b
void __cdecl SfxSetChannelVolume(unsigned char volume, int channel)
{
    char message[128];
    int index;
    if (!(g_SoundChannels.initialized))
        return;
    if (volume > 127)
        volume = 127;
    if (channel < 1 || channel > 5)
        return;
    index = channel - 1;
    volume = g_SoundData.masterVolume * volume / 100;
    g_SoundChannels.channels[index].volumeFadeStarted = 0;
    g_SoundChannels.channels[index].volume = (int)volume;
    g_SoundChannels.channels[index].targetVolume = (int)volume;
    if (index == 0) {
        EnterCriticalSection(&g_SoundLock);
        if (g_SoundData.sfxStreamObject)
            DsSetVolumeMillibels(g_SoundData.sfxStreamObject, g_SoundVolumeMillibelTable[volume]);
        LeaveCriticalSection(&g_SoundLock);
    }
    if (g_SoundChannels.channels[index].buffer)
        DsSetVolumeMillibels(g_SoundChannels.channels[index].buffer, g_SoundVolumeMillibelTable[volume]);
    if (g_SoundChannels.channels[index].loopBuffer)
        DsSetVolumeMillibels(g_SoundChannels.channels[index].loopBuffer, g_SoundVolumeMillibelTable[volume]);
    // STRING: FF7 0x00967ec8
    sprintf(message, "SET VOLUME %d: %d\n", channel, volume);
    Gfx_DebugPrint(message);
}

/*
 * When sound is enabled, duration zero delegates to SfxSetChannelVolume. Otherwise clamps volume
 * to 127, validates channel 1..5 and, when different from current, records target volume, resets
 * transition accumulators, derives an integer step from delta/(duration/60.0) and enables
 * g_SoundData.volumeFadeActiveFlag; logs the transition even when current equals target.
 */
// FUNCTION: FF7 0x00747dda
void __cdecl SfxSetChannelVolumeTrans(unsigned char volume, int channel, unsigned char duration)
{
    int p;
    char message[128];
    int index;
    if (!(g_SoundChannels.initialized))
        return;
    if (duration == 0)
        SfxSetChannelVolume(volume, channel);
    else {
        if (volume > 127)
            volume = 127;
        if (channel < 1 || channel > 5)
            return;
        index = channel - 1;
        if (g_SoundChannels.channels[index].volume != (int)volume) {
            g_SoundChannels.channels[index].targetVolume = (int)volume;
            g_SoundChannels.channels[index].volumeFadeElapsed = 0;
            p = (int)volume - g_SoundChannels.channels[index].volume;
            g_SoundChannels.channels[index].volumeFadeRate = (int)((double)p / (duration / 60.0));
            g_SoundChannels.channels[index].volumeFadeStarted = 0;
            g_SoundData.volumeFadeActiveFlag = 1;
        }
        // STRING: FF7 0x00967edc
        sprintf(message, "SET VOLUME TRANS %d: %d, step=%d\n", channel, volume, duration);
        Gfx_DebugPrint(message);
    }
}

/*
 * Applies pan to sound channels 1 through 5 through SfxSetChannelPan. Channel 0 is left alone.
 */
// FUNCTION: FF7 0x00747f31
void __cdecl SfxSetAllChannelsPan(char pan)
{
    int channel;

    for (channel = 1; channel <= 5; channel++) {
        SfxSetChannelPan((char)pan, channel);
    }
}

/*
 * Calls SfxSetChannelPanTrans(pan,channel,transition) for all five channels 1..5, preserving char-width
 * forwarding.
 */
// FUNCTION: FF7 0x00747f63
void __cdecl SfxSetAllChannelsPanTrans(char pan, char transition)
{
    int channel;

    for (channel = 1; channel <= 5; channel++) {
        SfxSetChannelPanTrans((char)pan, channel, (char)transition);
    }
}

/*
 * When sound is enabled, pan is at most 127 and channel is 1..5, stores current/target pan and
 * clears its transition. Applies g_SoundPanMillibelTable[pan]*g_SoundData.panInvertScale to channel 1's shared
 * buffer under g_SoundLock and to the available primary channel buffer.
 */
// FUNCTION: FF7 0x00747f99
void __cdecl SfxSetChannelPan(unsigned char pan, int channel)
{
    int index;
    if (!(g_SoundChannels.initialized))
        return;
    if (pan < 0 || pan > 127) { /* empty first arm: lib/codegen.h */
    } else if (channel < 1 || channel > 5) { /* empty first arm: lib/codegen.h */
    } else {
        index = channel - 1;
        if (index == 0) {
            EnterCriticalSection(&g_SoundLock);
            DsSetPanMillibels(g_SoundData.sfxStreamObject, g_SoundPanMillibelTable[pan] * g_SoundData.panInvertScale);
            LeaveCriticalSection(&g_SoundLock);
        }
        g_SoundChannels.channels[index].pan = (int)pan;
        g_SoundChannels.channels[index].targetPan = (int)pan;
        g_SoundChannels.channels[index].panFadeStarted = 0;
        if (g_SoundChannels.channels[index].buffer)
            DsSetPanMillibels(g_SoundChannels.channels[index].buffer, g_SoundPanMillibelTable[pan] * g_SoundData.panInvertScale);
    }
}

/*
 * When sound is enabled, duration zero delegates to SfxSetChannelPan. Otherwise validates
 * pan/channel and, when different from current, records target pan, resets accumulators, derives
 * the integer step delta/(duration/60.0), clears its transition field and enables
 * g_SoundData.volumeFadeActiveFlag.
 */
// FUNCTION: FF7 0x0074809e
void __cdecl SfxSetChannelPanTrans(unsigned char pan, int channel, unsigned char duration)
{
    int difference;
    int channelIndex;
    if (!(g_SoundChannels.initialized))
        return;
    if (duration == 0) {
        SfxSetChannelPan(pan, channel);
    } else {
        if (pan < 0 || pan > 127)
            return;
        if (channel < 1 || channel > 5)
            return;
        channelIndex = channel - 1;
        if (g_SoundChannels.channels[channelIndex].pan != (int)pan) {
            g_SoundChannels.channels[channelIndex].targetPan = (int)pan;
            g_SoundChannels.channels[channelIndex].panFadeElapsed = 0;
            difference = (int)pan - g_SoundChannels.channels[channelIndex].pan;
            g_SoundChannels.channels[channelIndex].panFadeRate = (int)((double)difference / (duration / 60.0));
            g_SoundChannels.channels[channelIndex].panFadeStarted = 0;
            g_SoundData.volumeFadeActiveFlag = 1;
        }
    }
}

/*
 * Applies transpose to sound channels 1 through 5 through SfxSetChannelTranspose. Channel 0 is left
 * alone.
 */
// FUNCTION: FF7 0x007481ad
void __cdecl SfxSetAllChannelsTranspose(char transpose)
{
    int channel;

    for (channel = 1; channel <= 5; channel++) {
        SfxSetChannelTranspose((char)transpose, channel);
    }
}

/*
 * Sets the transpose of sound channels 1..5 to transpose with transition transition.
 */
// FUNCTION: FF7 0x007481df
void __cdecl SfxSetAllChannelsTransposeTrans(char transpose, char transition)
{
    int channel;

    for (channel = 1; channel <= 5; channel++) {
        SfxSetChannelTransposeTrans((char)transpose, channel, (char)transition);
    }
}

/*
 * When sound is enabled and channel is 1..5, stores current/target signed transpose, clears its
 * transition and computes frequency through SfxFrequencyFromTranspose using the channel base
 * frequency. Applies it to available shared/primary buffers, protecting the shared channel-1
 * buffer with g_SoundLock.
 */
// FUNCTION: FF7 0x00748215
void __cdecl SfxSetChannelTranspose(char transpose, int channel)
{
    int freq;
    int index;
    if (!(g_SoundChannels.initialized))
        return;
    freq = transpose;
    if (freq < -128 || freq > 127) { /* empty first arm: lib/codegen.h */
    } else if (channel < 1 || channel > 5) { /* empty first arm: lib/codegen.h */
    } else {
        index = channel - 1;
        g_SoundChannels.channels[index].transpose = freq;
        g_SoundChannels.channels[index].targetTranspose = freq;
        g_SoundChannels.channels[index].transposeFadeStarted = 0;
        freq = SfxFrequencyFromTranspose(freq, g_SoundChannels.channels[index].sampleRate);
        if (index == 0) {
            EnterCriticalSection(&g_SoundLock);
            if (g_SoundData.sfxStreamObject)
                DsSetFrequency(g_SoundData.sfxStreamObject, freq);
            LeaveCriticalSection(&g_SoundLock);
        }
        if (g_SoundChannels.channels[index].buffer)
            DsSetFrequency(g_SoundChannels.channels[index].buffer, freq);
    }
}

/*
 * When sound is enabled, duration zero delegates to SfxSetChannelTranspose. Otherwise validates
 * channel and, when transpose differs from current, records target signed transpose, resets
 * accumulators, derives the integer step delta/(duration/60.0), clears its transition field and
 * enables g_SoundData.volumeFadeActiveFlag.
 */
// FUNCTION: FF7 0x00748309
void __cdecl SfxSetChannelTransposeTrans(char transpose, int channel, unsigned char duration)
{
    int value;
    int transposeDelta;
    int channelIndex;
    if (!(g_SoundChannels.initialized))
        return;
    if (duration == 0)
        SfxSetChannelTranspose(transpose, channel);
    else {
        value = transpose;
        if (value < -128 || value > 127)
            return;
        if (channel < 1 || channel > 5)
            return;
        channelIndex = channel - 1;
        if (g_SoundChannels.channels[channelIndex].transpose != value) {
            g_SoundChannels.channels[channelIndex].targetTranspose = value;
            g_SoundChannels.channels[channelIndex].transposeFadeElapsed = 0;
            transposeDelta = value - g_SoundChannels.channels[channelIndex].transpose;
            g_SoundChannels.channels[channelIndex].transposeFadeRate = (int)((double)transposeDelta / (duration / 60.0));
            g_SoundChannels.channels[channelIndex].transposeFadeStarted = 0;
            g_SoundData.volumeFadeActiveFlag = 1;
        }
    }
}

/*
 * Returns frequency plus a truncated double adjustment: transpose/128 times half the frequency for
 * negative transpose, or transpose/127 times the full frequency otherwise.
 */
// FUNCTION: FF7 0x007483fa
int __cdecl SfxFrequencyFromTranspose(int transpose, int frequency)
{
    int result;
    if (transpose < 0)
        result = frequency + (int)(((double)transpose / 128.0) * ((double)frequency / 2.0));
    else
        result = frequency + (int)(((double)transpose / 127.0) * frequency);
    return result;
}

/*
 * Resets channel channelNum (1..5): volume and target volume to 0x7F, pan and target pan to pan
 * masked to eight bits, transpose and target transpose to 0, and the sample rate.
 */
// FUNCTION: FF7 0x00748447
void __cdecl SfxResetChannel(int channelNum, int pan, int sampleRate)
{
    SoundChannel *channelRecord;

    channelRecord = &g_SoundChannels.channels[channelNum - 1];
    channelRecord->targetVolume = 0x7F;
    channelRecord->volume = 0x7F;
    channelRecord->targetPan = pan & 0xFF;
    channelRecord->pan = channelRecord->targetPan;
    channelRecord->targetTranspose = 0;
    channelRecord->transpose = 0;
    channelRecord->sampleRate = sampleRate;
}

/*
 * Copies a nonnull dataDir to g_SoundData + 0x198 with strcpy (no length check), otherwise clears the
 * buffer's first byte.
 */
// FUNCTION: FF7 0x007484ab
void __cdecl SfxSetAudioDataDirectory(char *dataDir)
{
    if (dataDir)
        strcpy(g_SoundAudioDataDirectory, dataDir);
    else
        g_SoundAudioDataDirectory[0] = 0;
}

/*
 * Copies a nonnull dataDir string into g_SoundData + 0x218 with strcpy; a null argument clears the
 * first byte.
 */
// FUNCTION: FF7 0x007484e8
void __cdecl MusicSetWaveDirectory(char *dataDir)
{
    if (dataDir)
        strcpy(g_SoundMusicWaveDirectory, dataDir);
    else
        g_SoundMusicWaveDirectory[0] = 0;
}

/*
 * Returns when wave music is disabled, otherwise logs the requested cross-play and rejects tracks
 * above 98. Clears transition flags and records duration; track zero fades to silence. Maps tracks
 * one/two according to remembered track, restores volume for the same active track or starts a
 * transition, halving duration before fading an active old track to 15 or starting a new track at
 * volume zero.
 */
// FUNCTION: FF7 0x00748525
void __cdecl MusicWaveCrossPlay(int track, int duration)
{
    char message[128];
    if ((g_SoundChannels.initialized) == 0)
        return;
    // STRING: FF7 0x00967f00
    sprintf(message, "cross play music: %d\n", track);
    Gfx_DebugPrint(message);
    if (track > SOUND_SONG_LAST)
        return;
    g_MusicPlayAfterFade = 0;
    g_MusicCrossPlayActive = 0;
    g_MusicFadeDuration = duration;
    if (track == 0) {
        g_MusicCrossPlayActive = 1;
        MusicWaveSetVolumeTrans(0, (unsigned char)g_MusicFadeDuration);
        return;
    }
    switch (track) {
    case SOUND_SONG_OA:
        track = g_SoundChannels.track == SOUND_SONG_OB ? SOUND_SONG_OB : SOUND_SONG_OA;
        break;
    case SOUND_SONG_OB:
        track = g_SoundChannels.track == SOUND_SONG_OA ? SOUND_SONG_OA : SOUND_SONG_OB;
        break;
    }
    if (g_SoundData.musicPlayingFlag != 0 && g_SoundChannels.track == track) {
        MusicWaveSetVolumeTrans(127, (unsigned char)g_MusicFadeDuration);
    } else {
        g_MusicPlayAfterFade = 1;
        if (g_SoundData.musicPlayingFlag != 0) {
            g_MusicPendingTrack = track;
            g_MusicFadeDuration = g_MusicFadeDuration / 2;
            MusicWaveSetVolumeTrans(15, (unsigned char)g_MusicFadeDuration);
        } else {
            MusicWaveSetVolume(0);
            MusicPlayWaveTrack(track, 0);
        }
    }
}

/*
 * When sound is enabled and volume is at most 127, logs and stores current/target music volume,
 * clears its transition, scales volume by g_SoundData.musicMasterVolumePercent/100 and applies the
 * g_SoundVolumeMillibelTable lookup to a nonzero g_SoundData.musicStreamObject under
 * g_SoundCriticalSectionMusic's critical section.
 */
// FUNCTION: FF7 0x00748669
void __cdecl MusicWaveSetVolume(unsigned char volume)
{
    char message[128];
    if (!(g_SoundChannels.initialized))
        return;
    if (volume < 0 || volume > 127)
        return;
    // STRING: FF7 0x00967f18
    sprintf(message, "set music volume: %d\n", volume);
    Gfx_DebugPrint(message);
    g_MusicVolume = volume;
    g_MusicTargetVolume = volume;
    g_MusicFadeStarted = 0;
    volume = g_SoundData.musicMasterVolumePercent * volume / 100;
    volume = (unsigned char)volume;
    EnterCriticalSection((struct _RTL_CRITICAL_SECTION *)(&g_SoundCriticalSectionMusic));
    if (g_SoundData.musicStreamObject)
        DsSetVolumeMillibels(g_SoundData.musicStreamObject, g_SoundVolumeMillibelTable[volume]);
    LeaveCriticalSection((struct _RTL_CRITICAL_SECTION *)(&g_SoundCriticalSectionMusic));
}

/*
 * When sound is enabled, zero step delegates to MusicWaveSetVolume. Otherwise validates targetVolume<=127,
 * logs the transition and, if current differs, stores its target/step delta/(step/60.0), resets
 * accumulators, enables g_SoundData.volumeFadeActiveFlag and immediately calls SoundUpdateFades.
 */
// FUNCTION: FF7 0x00748750
void __cdecl MusicWaveSetVolumeTrans(unsigned char targetVolume, unsigned char step)
{
    char message[128];
    if ((g_SoundChannels.initialized) == 0)
        return;
    if (step == 0) {
        MusicWaveSetVolume(targetVolume);
    } else if (targetVolume < 0 || targetVolume > 127) { /* empty first arm: lib/codegen.h */
    } else {
        // STRING: FF7 0x00967f30
        sprintf(message, "set music volume trans: %d->%d, step=%d\n", g_MusicVolume, targetVolume, step);
        Gfx_DebugPrint(message);
        if (g_MusicVolume != targetVolume) {
            g_MusicTargetVolume = targetVolume;
            g_MusicFadeElapsed = 0;
            g_MusicFadeRate = (int)((targetVolume - g_MusicVolume) / (step / 60.0));
            g_MusicFadeStarted = 0;
            g_SoundData.volumeFadeActiveFlag = 1;
            SoundUpdateFades();
        }
    }
}

/*
 * When sound is enabled, duration's low byte zero sets the ending volume immediately. Otherwise
 * validates the low bytes of start/end, logs the fade, sets its starting volume and schedules its
 * ending transition using the narrowed duration.
 */
// FUNCTION: FF7 0x00748861
void __cdecl MusicWaveFadeVolume(int duration, int start, int end)
{
    char message[128];
    if ((g_SoundChannels.initialized) == 0)
        return;
    if ((duration & 255) == 0)
        MusicWaveSetVolume((unsigned char)end);
    else if ((start & 255) < 0 || (start & 255) > 127) { /* empty first arm: lib/codegen.h */
    } else if ((end & 255) < 0 || (end & 255) > 127) { /* empty first arm: lib/codegen.h */
    } else {
        // STRING: FF7 0x00967f5c
        sprintf(message, "fade music volume: %d->%d, step=%d\n", start & 255, end & 255, duration & 255);
        Gfx_DebugPrint(message);
        MusicWaveSetVolume((unsigned char)start);
        MusicWaveSetVolumeTrans((unsigned char)end, (unsigned char)duration);
    }
}

/*
 * Empty. A sound hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x00748928
void __cdecl Sfx_UnusedNoOp_1(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x00748928,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Empty. A sound hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x0074892d
void __cdecl Sfx_UnusedNoOp_2(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x0074892d,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Empty. A sound hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x00748932
void __cdecl Sfx_UnusedNoOp_3(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x00748932,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Empty. A sound hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x00748937
void __cdecl Sfx_UnusedNoOp_4(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x00748937,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Empty. A sound hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x0074893c
void __cdecl Sfx_UnusedNoOp_5(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x0074893c,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Empty. A sound hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x00748941
void __cdecl Sfx_UnusedNoOp_6(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x00748941,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Empty. A sound hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x00748946
void __cdecl Sfx_UnusedNoOp_7(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x00748946,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Empty. A sound hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x0074894b
void __cdecl Sfx_UnusedNoOp_8(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x0074894b,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}
} /* extern "C" */

/*
 * The sound effect and music service (0x00748950..0x00749940): services DirectSound streams, plays,
 * stops, rewinds and tunes cached buffers, sets master volumes, plays sound groups and saves and
 * restores channel loops. C linkage keeps the calls from the C sources and the timer callbacks.
 */
#pragma intrinsic(memset)
extern "C" {
void DsUnlockBuffer(int buffer, int first, int firstSize, int second, int secondSize);

/* Internal DirectSound buffer helpers (dx_snd.cpp) taking int buffer handles in this C++ TU. */
extern int __cdecl DsPlayBuffer(int buffer, int flags);
extern void __cdecl DsStopBuffer(int buffer); /* dx_snd.cpp helper. */
#pragma intrinsic(memcpy)
__declspec(dllimport) void __stdcall OutputDebugStringA(const char *text);
#pragma intrinsic(strcpy, strcat, memset, strcmp)
void __cdecl SfxSetCachedBufferVolume(int soundId, int volume);
void __stdcall SfxLoopSwitchTimerProc(UINT timer, UINT message, DWORD user, DWORD one, DWORD two);
#pragma intrinsic(memcpy, memset)
void __cdecl DsReleaseBuffer(int buffer);
void __cdecl DsSetCurrentPosition(int buffer, int position);
int __cdecl DsSetFrequency(int, int);
int __cdecl DsSetVolumeMillibels(int, int);
// Prototypes of callees the C sources used implicitly (definitions elsewhere), required in C++.
extern "C" unsigned int __cdecl DsGetCurrentPlayPosition(int buffer);
extern "C" int __cdecl DsSetPanMillibels(int buffer, int pan);

/*
 * Services a DirectSound streaming record unless its disabled flag or completed loop-tail
 * condition blocks work. A close request optionally unprepares/closes the ACM stream, closes the
 * sound stream and returns 1. For an active buffer, computes play-cursor movement with wraparound;
 * the special looping path copies its retained tail region into the buffer after the threshold and
 * marks it complete. Otherwise updates the last cursor and remaining bytes and decodes chunks
 * until more than three chunk sizes lie ahead. At nonlooping end, closes and returns 1 when
 * remaining bytes are exhausted, or locks and zeroes one chunk including both wrap regions and
 * returns 0. Unsupported stream kinds leave the decoded-size local uninitialized, and a zero
 * loop-tail span can prevent loop progress.
 */
// FUNCTION: FF7 0x00748950
int __cdecl SfxStreamService(SoundStreamPlayer *player)
{
    int cursor;
    SoundStreamSource *streamSource;
    int played;
    int headBytes;
    int sourcePtr;
    int ptr1;
    int curLen;
    int bytes1;
    int moveAmount;
    int ptr2;
    int queued;
    int decoded;
    int writeOffset;
    int bytesAhead;
    int loopSize;

    if (player->isPaused != 0) {
        return 0;
    } else {
        streamSource = player->streamSource;
        if (player->looping != 0 && streamSource->startOffset != 0 && player->chunkDecoded != 0) {
            return 0;
        } else if (player->loopTailDone != 0) {
            if (player->isOpen != 0 && *(unsigned short *)player->streamSource->sourceFormat == 2) {
                AcmStreamUnprepare(player->acmStream, (int)player + 0x48);
                AcmStreamClose(player->acmStream);
            }
            SfxStreamClose(player);
            return 1;
        } else {
            if (player->isOpen != 0) {
                ((LPDIRECTSOUNDBUFFER)player->dsBuffer)->GetCurrentPosition((DWORD *)&cursor, (DWORD *)&writeOffset);
                if (player->cursor < cursor) {
                    moveAmount = cursor - player->cursor;
                } else {
                    moveAmount = player->bufferSize - player->cursor + cursor;
                }
                played = moveAmount;
                if (player->state != SOUND_STREAM_MUSIC && player->looping != 0 && streamSource->startOffset != 0) {
                    if (played >= streamSource->startOffset) {
                        player->chunkDecoded = 1;
                        loopSize = streamSource->endOffset - streamSource->startOffset;
                        headBytes = streamSource->startOffset;
                        sourcePtr = player->pcmBuffer + streamSource->endOffset - player->bytesToRead;
                        player->writePos = 0;
                        if (headBytes < player->bytesToRead) {
                            DsWriteIntoBufferRegion((char *)sourcePtr, headBytes, (IDirectSoundBuffer *)player->dsBuffer, player->writePos, headBytes);
                            headBytes = 0;
                        } else {
                            DsWriteIntoBufferRegion((char *)sourcePtr, player->bytesToRead, (IDirectSoundBuffer *)player->dsBuffer, player->writePos, player->bytesToRead);
                            headBytes -= player->bytesToRead;
                            player->writePos += player->bytesToRead;
                            sourcePtr = player->pcmBuffer + streamSource->startOffset;
                        }
                        while (headBytes != 0) {
                            if (headBytes < loopSize) {
                                DsWriteIntoBufferRegion((char *)sourcePtr, headBytes, (IDirectSoundBuffer *)player->dsBuffer, player->writePos, headBytes);
                                headBytes = 0;
                            } else {
                                DsWriteIntoBufferRegion((char *)sourcePtr, loopSize, (IDirectSoundBuffer *)player->dsBuffer, player->writePos, loopSize);
                                headBytes -= loopSize;
                                player->writePos += loopSize;
                            }
                        }
                    }
                    return 0;
                }
                if (player->writePos < cursor) {
                    queued = player->bufferSize - cursor + player->writePos;
                } else {
                    queued = player->writePos - cursor;
                }
                bytesAhead = queued;
                player->cursor = cursor;
                player->bytesQueued -= played;
                while (bytesAhead <= player->chunkSize * 3) {
                    if (player->chunkDecoded != 0 && player->looping == 0) {
                        if (player->bytesQueued <= 0) {
                            SfxStreamClose(player);
                            return 1;
                        }
                        if (DsLockBuffer(player->dsBuffer, player->chunkSize, &ptr1, &bytes1, (int)&ptr2, (int)&curLen, player->writePos) != 0) {
                            memset((void *)(ptr1), 0, bytes1);
                            if (ptr2 != 0) {
                                memset((void *)(ptr2), 0, curLen);
                            }
                            DsUnlockBuffer(player->dsBuffer, ptr1, bytes1, ptr2, curLen);
                            player->writePos += player->chunkSize;
                            if (player->writePos >= player->bufferSize) {
                                player->writePos -= player->bufferSize;
                            }
                        }
                        return 0;
                    }
                    switch (player->state) {
                    case SOUND_STREAM_SFX:
                        decoded = SfxStreamDecodeChunk(player, g_SoundData.sfxDataFileHandle);
                        break;
                    case SOUND_STREAM_MUSIC:
                        decoded = SfxStreamDecodeChunk(player, g_SoundData.musicFileHandle);
                    }
                    bytesAhead += decoded;
                }
            }
            return 0;
        }
    }
}

/*
 * When sound is enabled and id-1 is in 0 through 0x2EE with a nonzero cache record, lazily loads
 * its DirectSound buffer, rewinds it, and applies the pan table, the byte-truncated product of
 * global and requested volume divided by 100, and transposed base frequency. Plays with the record
 * flags. On a -1 play result, releases and clears the buffer, reloads it, reapplies pan, volume
 * and frequency, then retries without rewinding or checking the reload result; ignores the retry
 * result.
 */
// FUNCTION: FF7 0x00748d69
void __cdecl SfxPlayCachedBuffer(int id, unsigned char pan, unsigned char volume,
                                 unsigned char trans)
{
    SoundRecordEntry *p;
    int q;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    q = id - 1;
    if (q < 0 || q > 0x2EE) {
        return;
    }
    p = &((SoundRecordEntry *)&g_SoundChannels.soundRecords)[q];
    if (p->dataSize == 0) {
        return;
    }
    if (g_SoundChannels.bufferHandles[q] == 0) {
        SfxLoadIntoBuffer(id, 0);
    }
    if (g_SoundChannels.bufferHandles[q] != 0) {
        DsSetCurrentPosition(g_SoundChannels.bufferHandles[q], 0);
        DsSetPanMillibels(g_SoundChannels.bufferHandles[q],
                          g_SoundPanMillibelTable[pan] * g_SoundData.panInvertScale);
        volume = g_SoundData.masterVolume * volume / 0x64;
        DsSetVolumeMillibels(g_SoundChannels.bufferHandles[q], g_SoundVolumeMillibelTable[volume]);
        DsSetFrequency(g_SoundChannels.bufferHandles[q],
                       SfxFrequencyFromTranspose(
                           trans, ((WAVEFORMATEX *)((SoundRecordEntry *)&g_SoundChannels.soundRecords)[q].waveFormat)->nSamplesPerSec));
        if (DsPlayBuffer(g_SoundChannels.bufferHandles[q], p->loopFlag) == -1) {
            DsReleaseBuffer(g_SoundChannels.bufferHandles[q]);
            g_SoundChannels.bufferHandles[q] = 0;
            SfxLoadIntoBuffer(id, 0);
            DsSetPanMillibels(g_SoundChannels.bufferHandles[q],
                              g_SoundPanMillibelTable[pan] * g_SoundData.panInvertScale);
            DsSetVolumeMillibels(g_SoundChannels.bufferHandles[q], g_SoundVolumeMillibelTable[volume]);
            DsSetFrequency(g_SoundChannels.bufferHandles[q],
                           SfxFrequencyFromTranspose(
                               trans,
                               ((WAVEFORMATEX *)((SoundRecordEntry *)&g_SoundChannels.soundRecords)[q].waveFormat)->nSamplesPerSec));
            DsPlayBuffer(g_SoundChannels.bufferHandles[q], p->loopFlag);
        }
    }
}

/*
 * When sound is enabled and soundId-1 is 0..750 with valid metadata, loads a missing cached buffer,
 * rewinds it and plays using the metadata loop byte. On playback result -1 releases/clears it,
 * reloads and retries playback without checking the reloaded pointer.
 */
// FUNCTION: FF7 0x00748f8f
void __cdecl SfxRetriggerCachedBuffer(int soundId)
{
    SoundRecordEntry *meta;
    int index;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    index = soundId - 1;
    if (index < 0 || index > 0x2EE) { /* empty first arm: lib/codegen.h */
    } else {
        meta = &((SoundRecordEntry *)&g_SoundChannels.soundRecords)[index];
        if (meta->dataSize == 0) {
            return;
        }
        if (g_SoundChannels.bufferHandles[index] == 0) {
            SfxLoadIntoBuffer(soundId, 0);
        }
        if (g_SoundChannels.bufferHandles[index] != 0) {
            DsSetCurrentPosition(g_SoundChannels.bufferHandles[index], 0);
            if (DsPlayBuffer(g_SoundChannels.bufferHandles[index], meta->loopFlag) == -1) {
                DsReleaseBuffer(g_SoundChannels.bufferHandles[index]);
                g_SoundChannels.bufferHandles[index] = 0;
                SfxLoadIntoBuffer(soundId, 0);
                DsPlayBuffer(g_SoundChannels.bufferHandles[index], meta->loopFlag);
            }
        }
    }
}

/*
 * Stops cached sound buffer soundId (1..0x2EF) and rewinds it to the start.
 */
// FUNCTION: FF7 0x0074908d
void __cdecl SfxStopAndRewindCachedBuffer(int soundId)
{
    int index;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    index = soundId - 1;
    if (index < 0 || index > 0x2EE) { /* empty first arm: lib/codegen.h */
    } else if (g_SoundChannels.bufferHandles[index] != 0) {
        DsStopBuffer(g_SoundChannels.bufferHandles[index]);
        DsSetCurrentPosition(g_SoundChannels.bufferHandles[index], 0);
    }
}

/*
 * Stops cached sound buffer soundId (1..0x2EF).
 */
// FUNCTION: FF7 0x007490ef
void __cdecl SfxStopCachedBuffer(int soundId)
{
    int index;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    index = soundId - 1;
    if (index < 0 || index > 0x2EE) { /* empty first arm: lib/codegen.h */
    } else if (g_SoundChannels.bufferHandles[index] != 0) {
        DsStopBuffer(g_SoundChannels.bufferHandles[index]);
    }
}

/*
 * Resumes cached sound buffer soundId: looping buffers (the record's loopFlag) play
 * looped, others play once when they were stopped mid-way.
 */
// FUNCTION: FF7 0x0074913c
void __cdecl SfxResumeCachedBuffer(int soundId)
{
    int index;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    index = soundId - 1;
    if (index < 0 || index > 0x2EE) { /* empty first arm: lib/codegen.h */
    } else if (g_SoundChannels.bufferHandles[index] != 0) {
        if (((SoundRecordEntry *)&g_SoundChannels.soundRecords)[index].loopFlag != 0) {
            DsPlayBuffer(g_SoundChannels.bufferHandles[index], 1);
        } else if (DsGetCurrentPlayPosition(g_SoundChannels.bufferHandles[index]) != 0) {
            DsPlayBuffer(g_SoundChannels.bufferHandles[index], 0);
        }
    }
}

/*
 * When sound is enabled, rejects invalid cache indices and clamps the low-byte volume to 127.
 * Scales volume and applies the original lookup to a nonzero cached buffer.
 */
// FUNCTION: FF7 0x007491cb
void __cdecl SfxSetCachedBufferVolume(int soundId, int volume)
{
    int index;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    index = soundId - 1;
    if (index < 0 || index > 0x2EE) {
        return;
    }
    if ((volume & 0xFF) > 0x7F) {
        *(char *)&volume = 0x7F;
    }
    if (g_SoundChannels.bufferHandles[index] != 0) {
        *(char *)&volume = g_SoundData.masterVolume * (volume & 0xFF) / 0x64;
        DsSetVolumeMillibels(g_SoundChannels.bufferHandles[index], g_SoundVolumeMillibelTable[volume & 0xFF]);
    }
}

/*
 * When sound is enabled and sound-1 is 0..750, clamps pan to 127 and applies
 * g_SoundPanMillibelTable[pan]*g_SoundData.panInvertScale to the cached buffer when nonzero.
 */
// FUNCTION: FF7 0x00749257
void __cdecl SfxSetCachedBufferPan(int sound, unsigned char pan)
{
    int val;
    if (!(g_SoundChannels.initialized))
        return;
    val = sound - 1;
    if (val < 0 || val > 750)
        return;
    if (pan > 127)
        pan = 127;
    if (g_SoundChannels.bufferHandles[val])
        DsSetPanMillibels(g_SoundChannels.bufferHandles[val], g_SoundPanMillibelTable[pan] * g_SoundData.panInvertScale);
}

/*
 * When sound is enabled and soundId-1 is 0..750 with a cached buffer, applies
 * SfxFrequencyFromTranspose using transpose's low unsigned byte and the metadata base-frequency word;
 * preserves unsigned low-byte transpose interpretation.
 */
// FUNCTION: FF7 0x007492cd
void __cdecl SfxSetCachedBufferTranspose(int soundId, int transpose)
{
    int index;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    index = soundId - 1;
    if (index < 0 || index > 0x2EE) { /* empty first arm: lib/codegen.h */
    } else if (g_SoundChannels.bufferHandles[index] != 0) {
        DsSetFrequency(g_SoundChannels.bufferHandles[index], SfxFrequencyFromTranspose(transpose & 0xFF, ((WAVEFORMATEX *)((SoundRecordEntry *)&g_SoundChannels.soundRecords)[index].waveFormat)->nSamplesPerSec));
    }
}

/*
 * Sets the sound-effect master volume, the config menu's SFX slider: `g_SoundData.masterVolume`
 * (`g_SoundData.masterVolume`, 100 after `SoundInit`). Every later volume set on an effect channel
 * or the effect stream uses master percent * volume / 100 as the index into the 128-entry
 * millibel table; sounds already playing keep their volume until it is set again. The value is
 * not checked: above 100 the index can pass 127 and read past the table.
 */
// FUNCTION: FF7 0x0074933d
void __cdecl SfxSetMasterVolumePercent(int value)
{
    g_SoundData.masterVolume = value;
}

/*
 * Sets the music master volume, the config menu's music slider:
 * `g_SoundData.musicMasterVolumePercent` (100 after `SoundInit`), then re-applies the current
 * wave music volume with `MusicWaveSetVolume`, which scales it by percent / 100. Only wave music
 * reads the percentage; MIDI music does not. `MusicWaveSetVolume` also resets the volume target
 * to the current volume, so a running music fade stops here. Above 100 the scaled index can pass
 * 127 and read past the 128-entry millibel table.
 */
// FUNCTION: FF7 0x0074934a
void __cdecl MusicSetMasterVolumePercent(int percent)
{
    g_SoundData.musicMasterVolumePercent = percent;
    MusicWaveSetVolume(g_MusicVolume);
}

/*
 * Returns the sound effect master volume percentage.
 */
// FUNCTION: FF7 0x00749366
int __cdecl SfxGetMasterVolumePercent(void)
{
    return g_SoundData.masterVolume;
}

/*
 * Returns the music master volume percentage.
 */
// FUNCTION: FF7 0x00749370
int __cdecl MusicGetMasterVolumePercent(void)
{
    return g_SoundData.musicMasterVolumePercent;
}

/*
 * Always 0: a sound hook with no work in the shipped build.
 */
// FUNCTION: FF7 0x0074937a
int __cdecl Sfx_UnusedReturnZero_1(void)
{
    return 0;
}

/*
 * Always 0: a sound hook with no work in the shipped build.
 */
// FUNCTION: FF7 0x00749381
int __cdecl Sfx_UnusedReturnZero_2(void)
{
    return 0;
}

/*
 * Empty. A sound hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x00749388
void __cdecl Sfx_UnusedNoOp_9(void)
{
    /* The original is empty. */
}

/*
 * Zeroes the 256-byte sound-group table at g_SoundGroups, copies up to sixteen supplied SoundGroup
 * records until the first null pointer and passes the table to SfxStartGroupPreload.
 */
// FUNCTION: FF7 0x0074938d
void __cdecl SfxRegisterSoundGroups(struct SoundGroup **groups)
{
    int i;
    memset((int *)&g_SoundGroups, 0, 256);
    for (i = 0; i < 16; i++) {
        if (groups[i] == 0)
            break;
        g_SoundGroups[i] = *groups[i];
    }
    SfxStartGroupPreload(g_SoundGroups);
}

/*
 * Plays sound group group: waits for its four buffers (from the group table g_SoundGroups) to load,
 * then plays each with pan pan and volume 0x7F.
 */
// FUNCTION: FF7 0x00749404
void __cdecl SfxPlaySoundGroup(char pan, int group)
{
    int i;

    SfxWaitForGroupLoaded(group);
    for (i = 0; i < 4; i++) {
        g_SoundGroupSlots[i] = g_SoundGroups[group].sound[i];
        SfxPlayCachedBuffer(g_SoundGroupSlots[i], pan, 0x7F, 0);
    }
}
/*
 * Converts the one-based sound ID to an index and returns zero outside indices 0 through 750 or
 * for a null buffer. Otherwise calls the DirectSound buffer's GetStatus and returns bit zero of
 * the local status word; the call's result is ignored.
 */
// FUNCTION: FF7 0x00749467
int __cdecl SfxIsSoundPlaying(int soundId)
{
    DWORD status;
    int index;
    index = soundId - 1;
    if (index < 0 || index > 750)
        return 0;
    if (g_SoundChannels.bufferHandles[index] != 0) {
        ((LPDIRECTSOUNDBUFFER)g_SoundChannels.bufferHandles[index])->GetStatus(&status);
        return status & 1;
    }
    return 0;
}

/*
 * Enters g_SoundLock's critical section, closes the stream through
 * SfxStreamClose(&g_SoundData.sfxStreamObject) and leaves the critical section.
 */
// FUNCTION: FF7 0x007494c2
void __cdecl SfxStreamCloseLocked(void)
{
    EnterCriticalSection(&g_SoundLock);
    SfxStreamClose((struct SoundStreamPlayer *)&g_SoundData.sfxStreamObject);
    LeaveCriticalSection(&g_SoundLock);
}

/*
 * Enters g_SoundCriticalSectionMusic's critical section, closes the music stream through
 * SfxStreamClose(&g_SoundData.musicStreamObject) and leaves the critical section.
 */
// FUNCTION: FF7 0x007494ea
void __cdecl MusicWaveStreamCloseLocked(void)
{
    EnterCriticalSection((CRITICAL_SECTION *)&g_SoundCriticalSectionMusic);
    SfxStreamClose((struct SoundStreamPlayer *)&g_SoundData.musicStreamObject);
    LeaveCriticalSection((CRITICAL_SECTION *)&g_SoundCriticalSectionMusic);
}


/*
 * Defaults g_MusicWaveRegistrySetting to zero and reads the named value from the Final Fantasy VII registry key.
 * DISABLED leaves zero and ENABLED sets one; closes an opened key. Preserves the original query-
 * size 256 with the smaller local data buffer and does not validate the value type.
 */
// FUNCTION: FF7 0x00749512
void __cdecl MusicReadWaveMusicRegistrySetting(void)
{
    HKEY key;

    g_MusicWaveRegistrySetting = 0;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, s_ff7RegistryKey, 0, KEY_READ, &key) == 0) {
        DWORD size = 256;
        // STRING: FF7 0x00967f80
        char str[] = "wave_music";
        DWORD type;
        char buffer[20];

        if (RegQueryValueExA(key, str, 0, &type, (BYTE *)buffer, &size) == 0) {
            // STRING: FF7 0x00967f8c
            if (strcmp(buffer, "DISABLED") == 0)
                g_MusicWaveRegistrySetting = 0;
            // STRING: FF7 0x00967f98
            if (strcmp(buffer, "ENABLED") == 0)
                g_MusicWaveRegistrySetting = 1;
        }
        RegCloseKey(key);
    }
}

/*
 * Returns 0 when sound is disabled, 1 when g_SoundData.sfxStreamObject is nonzero, otherwise scans
 * five primary channel buffers and returns 1 for any nonzero DsGetCurrentPlayPosition result, or 0
 * when none qualifies.
 */
// FUNCTION: FF7 0x007495dd
int __cdecl SfxIsAnyChannelPlaying(void)
{
    int idx;
    int unused;

    unused = 0;
    if ((g_SoundChannels.initialized) == 0) {
        return 0;
    } else if (g_SoundData.sfxStreamObject != 0) {
        return 1;
    } else {
        for (idx = 0; idx < 5; idx++) {
            if (g_SoundChannels.channels[idx].buffer != 0) {
                if (DsGetCurrentPlayPosition(g_SoundChannels.channels[idx].buffer) != 0) {
                    return 1;
                }
            }
        }
        return 0;
    }
}

/*
 * Locks the channel's source buffer, computes the record's loop interval, creates a PCM16 loop
 * buffer (channel->loopBuffer) and copies that interval from the locked source when the loop buffer
 * can be locked. Unlocks the loop when locked and always unlocks the source after a successful source
 * lock.
 */
// FUNCTION: FF7 0x00749656
void __cdecl SfxBuildLoopBuffer(SoundRecordEntry *metadata, SoundChannel *channel)
{
    struct {
        WAVEFORMATEX format;
        unsigned int length;
        void *source;
        unsigned int loopSize;
        unsigned int sourceSize;
        SoundRecordEntry *meta;
        void *loop;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.meta = metadata;
    if (DsLockBuffer(channel->buffer, frame.meta->dataSize, (int *)(&frame.source), (int *)(&frame.sourceSize), 0, 0, 0)) {
        frame.length = frame.meta->loopEnd - frame.meta->loopStart;
        AcmMakePcm16Format(frame.meta->dataSize, (WAVEFORMATEX *)metadata->waveFormat, &frame.format);
        channel->loopBuffer = DsCreateSoundBuffer(frame.length, &frame.format);
        if (channel->loopBuffer != 0) {
            if (DsLockBuffer(channel->loopBuffer, frame.length, (int *)(&frame.loop), (int *)(&frame.loopSize), 0, 0, 0)) {
                memcpy(frame.loop, (char *)frame.source + frame.meta->loopStart, frame.length);
                DsUnlockBuffer(channel->loopBuffer, (int)(frame.loop), frame.loopSize, 0, 0);
            }
        }
        DsUnlockBuffer(channel->buffer, (int)(frame.source), frame.sourceSize, 0, 0);
    }
}

// Timer callback on a channel (user). Under the g_SoundLoopBufferLock critical section, if
// loopSwitchEnabled and loopBuffer are both nonzero, it plays the loop buffer looped and stops the
// channel's first buffer. The other callback arguments are unused.
// FUNCTION: FF7 0x00749754
void __stdcall SfxLoopSwitchTimerProc(UINT timer, UINT message, DWORD user, DWORD one, DWORD two)
{
    SoundChannel *channel;
    channel = (SoundChannel *)user;
    EnterCriticalSection((struct _RTL_CRITICAL_SECTION *)((CRITICAL_SECTION *)&g_SoundLoopBufferLock));
    if (channel->loopSwitchEnabled != 0 && channel->loopBuffer != 0) {
        DsPlayBuffer(channel->loopBuffer, 1);
        DsStopBuffer(channel->buffer);
    }
    LeaveCriticalSection((struct _RTL_CRITICAL_SECTION *)((CRITICAL_SECTION *)&g_SoundLoopBufferLock));
}

/*
 * When sound is enabled, clears the five channels' savedSoundId. A channel faded to volume 0 is
 * stopped (SfxPlayOnChannel(64, 0, channel)); otherwise, when its buffer exists and the stream flag
 * byte of sound record p is set, the channel's soundId is saved while its buffer or loop buffer
 * reports a nonzero play position.
 */
// FUNCTION: FF7 0x007497ac
void __cdecl SfxSaveChannelLoopRecords(void)
{
    int i;
    if (!(g_SoundChannels.initialized))
        return;
    for (i = 0; i < 5; i++) {
        g_SoundChannels.channels[i].savedSoundId = 0;
        if (g_SoundChannels.channels[i].targetVolume == 0) {
            SfxPlayOnChannel(64, 0, i + 1);
            continue;
        }
        if (g_SoundChannels.channels[i].buffer != 0 && ((SoundRecordEntry *)&g_SoundChannels.soundRecords)[i].loopFlag != 0) {
            if (DsGetCurrentPlayPosition(g_SoundChannels.channels[i].buffer) != 0)
                g_SoundChannels.channels[i].savedSoundId = g_SoundChannels.channels[i].soundId;
            else if (g_SoundChannels.channels[i].loopBuffer != 0) {
                if (DsGetCurrentPlayPosition(g_SoundChannels.channels[i].loopBuffer) != 0)
                    g_SoundChannels.channels[i].savedSoundId = g_SoundChannels.channels[i].soundId;
            }
        }
    }
}

/*
 * Restarts each channel's savedSoundId with pan 0x40 (SfxPlayOnChannel's first argument is the
 * pan).
 */
// FUNCTION: FF7 0x007498af
void __cdecl SfxRestoreChannelLoopRecords(void)
{
    int i;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    for (i = 0; i < 5; i++) {
        SfxPlayOnChannel(0x40, g_SoundChannels.channels[i].savedSoundId, i + 1);
    }
}

/*
 * Clears the five channels' savedSoundId.
 */
// FUNCTION: FF7 0x007498fa
void __cdecl SfxClearChannelLoopRecords(void)
{
    int i;

    if ((g_SoundChannels.initialized) == 0) {
        return;
    }
    for (i = 0; i < 5; i++) {
        g_SoundChannels.channels[i].savedSoundId = 0;
    }
}
} /* extern "C" */
