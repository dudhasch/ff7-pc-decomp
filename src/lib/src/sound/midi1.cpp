// src/lib/src/sound/midi1.cpp
// Original: C:\lib\src\sound\midi1.cpp
// Contents: MIDI song playback: initializes the song data, starts, stops, pauses and resumes
//   playback, streams events into buffers, and runs volume, tempo, pitch and pan changes with
//   fades.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/sound.h"
#include "lib/enums/sound.h"
#include <stdio.h>
#include <string.h>
#include "lib/api/mem.h"
#include "lib/api/file.h"
/* Retail MIDI init reads device/data/option settings from main's registry helpers. */
#include "ff7/api/main.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
#include "lib/enums/win32.h"
#include "lib/codegen.h"
#include "lib/enums/file.h"
extern "C" {

/* Minimum views: 42 observed active tracks and 31 loop-saved tracks; the
 * original capacity is unknown. */
// clang-format off
// GLOBAL: FF7 0x00966b48
int g_MidiVolumeCurve[128] = {0, 0x1dbb, 0x368d, 0x4bb8, 0x5dcb, 0x6d42, 0x7a83, 0x85e6, 0x8fb5, 0x982f, 0x9f87, 0xa5eb, 0xab81, 0xb068, 0xb4bc, 0xb893, 0xbc00, 0xbf14, 0xc1dd, 0xc465, 0xc6b8, 0xc8de, 0xcadd, 0xccbb, 0xce7d, 0xd029, 0xd1c1, 0xd348, 0xd4c1, 0xd62e, 0xd791, 0xd83f, 0xd8eb, 0xd996, 0xda3f, 0xdae6, 0xdb8c, 0xdc31, 0xdcd4, 0xdd77, 0xde18, 0xdeb8, 0xdf58, 0xdff7, 0xe095, 0xe132, 0xe1cf, 0xe26c, 0xe308, 0xe3a3, 0xe43e, 0xe4d8, 0xe572, 0xe60c, 0xe6a6, 0xe73f, 0xe7d8, 0xe871, 0xe909, 0xe9a2, 0xea3a, 0xead2, 0xeb69, 0xec01, 0xec99, 0xed30, 0xedc7, 0xee5f, 0xeef6, 0xeef6, 0xef8d, 0xef8d, 0xf024, 0xf024, 0xf0ba, 0xf0ba, 0xf151, 0xf151, 0xf1e8, 0xf1e8, 0xf27f, 0xf27f, 0xf315, 0xf315, 0xf3ac, 0xf3ac, 0xf442, 0xf442, 0xf4d9, 0xf4d9, 0xf56f, 0xf56f, 0xf605, 0xf605, 0xf69c, 0xf69c, 0xf732, 0xf732, 0xf7c8, 0xf7c8, 0xf85f, 0xf85f, 0xf8f5, 0xf8f5, 0xf98b, 0xf98b, 0xfa21, 0xfa21, 0xfab8, 0xfab8, 0xfb4e, 0xfb4e, 0xfbe4, 0xfbe4, 0xfc7a, 0xfc7a, 0xfd10, 0xfd10, 0xfda6, 0xfda6, 0xfe3d, 0xfe3d, 0xfed3, 0xfed3, 0xff69, 0xff69, 0xffff, 0xffff};
// GLOBAL: FF7 0x00966d48
unsigned char g_MidiGmResetSysex[6] = {0xf0, 0x7e, 0x7f, 0x09, 0x01, 0xf7};
// GLOBAL: FF7 0x00ddd6b0
int g_MidiMasterVolume = 0;
// GLOBAL: FF7 0x00ddd6b4
int g_MidiBaseTempo = 0;
// GLOBAL: FF7 0x00ddd6b8
int g_MidiDeviceIsMid125 = 0;
// Set while a stream stop is pending (MidiStreamStopChecked), cleared by the stop's return or by
// MidiStreamCallback after the sixth buffer comes back; MidiWaitForStop spins on it.
// GLOBAL: FF7 0x00ddd6bc
int g_MidiStopPending = 0;
// GLOBAL: FF7 0x00ddd6c0
int g_MidiUseVolumeCurve = 0;
unsigned char g_bssPad_00ddd6c4[4] = {0};
// The pitch-bend reset event block MidiPrimePitchBendReset builds (16 channels x 12 bytes) and sends.
// GLOBAL: FF7 0x00ddd6c8
unsigned char g_MidiPitchBendResetEvents[192] = {0};
// GLOBAL: FF7 0x00ddd788
int g_MidiVolume = 0;
// GLOBAL: FF7 0x00ddd78c
int g_MidiCrossFadeActive = 0;
// GLOBAL: FF7 0x00ddd790
int g_MidiStreamBuffer = 0;
// GLOBAL: FF7 0x00ddd794
int g_MidiReadCursor = 0;
// The size of the loaded song (GetLgpEntrySize, capped at 0x14C00).
// GLOBAL: FF7 0x00ddd798
int g_MidiSongSize = 0;
// The MIDI data directory: a 256-byte C-string path buffer at 0x00DDD79C (4 mod 8). Kept as a 1-byte
// head + 252-byte tail because VC5 8-byte-aligns arrays >= 8 bytes under #pragma bss_seg, which would
// shift 0x00DDD79C (and g_MidiDataPath at 0x00DDD89C) forward by 4 bytes.
// GLOBAL: FF7 0x00ddd79c
char g_MidiDataDir = 0;
unsigned char g_bssPad_00ddd7a0[252] = {0};
// The path of the MIDI LGP archive: the first word of a 256-byte buffer (g_bssPad_00ddd8a0 is the rest of
// it), used as a C string through (char *)&g_MidiDataPath.
// GLOBAL: FF7 0x00ddd89c
int g_MidiDataPath = 0;
unsigned char g_bssPad_00ddd8a0[252] = {0};
// GLOBAL: FF7 0x00ddd99c
int g_MidiSoundFontLoaded = 0;
// GLOBAL: FF7 0x00ddd9a0
char g_MidiSoundFontPath[128] = {0};
// The song header's format word (big-endian in the file); its track count, 0xDDDA22, is the registry
// object midiTrackCount.
// GLOBAL: FF7 0x00ddda20
short g_MidiFormatAndTrackCount[2] = {0};
// The song header's division word.
// GLOBAL: FF7 0x00ddda24
short g_MidiTimeDivision = 0;
// GLOBAL: FF7 0x00ddda28
MidiTrack midiTrackState[42] = {0};
unsigned char g_bssPad_00dddec0[640] = {0};
// GLOBAL: FF7 0x00dde140
int g_MidiPlaybackState = 0;
// GLOBAL: FF7 0x00dde144
int g_MidiCurrentSong = 0;
// A MidiDataType value.
// GLOBAL: FF7 0x00dde148
int g_MidiDataType = 0;
// GLOBAL: FF7 0x00dde14c
int g_MidiLastTick = 0;
// GLOBAL: FF7 0x00dde150
int g_MidiCurrentTick = 0;
// GLOBAL: FF7 0x00dde154
int g_MidiCurrentTrack = 0;
unsigned char g_bssPad_00dde158[4] = {0};
// GLOBAL: FF7 0x00dde15c
int midiSavedTrackState = 0;
unsigned char g_bssPad_00dde160[864] = {0};
unsigned char g_bssPad_00dde4c0[28] = {0};
// GLOBAL: FF7 0x00dde4dc
int g_MidiLoopFlag = 0;
// GLOBAL: FF7 0x00dde4e0
int g_MidiState = 0;
unsigned char g_bssPad_00dde4e4[4] = {0};
// GLOBAL: FF7 0x00dde4e8
int g_MidiLoopStart = 0;
// GLOBAL: FF7 0x00dde4ec
int g_MidiLoopEnd = 0;
// The song queued behind a volume fade-out, played when the fade ends.
// GLOBAL: FF7 0x00dde4f0
int g_MidiPendingSong = 0;
// GLOBAL: FF7 0x00dde4f4
int g_MidiFailed = 0;
// GLOBAL: FF7 0x00dde4f8
int g_MusicTrack1Requested = 0;
unsigned char g_bssPad_00dde4fc[4] = {0};
// The six 0x448-byte stream buffers (MIDIHDR at +0, events from +0x40, the buffer's index at +0x440):
// MidiStreamCallback, the prime functions and MidiUnprepareBuffers address idx * 0x448 + 0xDDE500.
// GLOBAL: FF7 0x00dde500
MidiStreamBuffer midiStreamBuffers[6] = {0};
// The timeGetTime stamp of the last volume-fade step.
// GLOBAL: FF7 0x00ddfeb0
int g_MidiVolumeFadeLastTime = 0;
// GLOBAL: FF7 0x00ddfeb4
int g_MidiFreeBufferCount = 0;
// The current volume-fade duration.
// GLOBAL: FF7 0x00ddfeb8
int g_MidiFadeDuration = 0;
// GLOBAL: FF7 0x00ddfebc
int g_MidiDriverVolume = 0;
// The expression reset event block MidiPrimeExpressionReset builds (16 channels x 12 bytes) and sends.
// GLOBAL: FF7 0x00ddfec0
unsigned char g_MidiExpressionResetEvents[192] = {0};
// GLOBAL: FF7 0x00ddff80
int g_MidiClosePending = 0;
unsigned char g_bssPad_00ddff84[4] = {0};
// GLOBAL: FF7 0x00ddff88
unsigned char g_MidiCriticalSection[24] = {0};
// GLOBAL: FF7 0x00ddffa0
int g_MidiDebugLog = 0;
// GLOBAL: FF7 0x00ddffa4
int g_MidiVolumeControl = 0;
// GLOBAL: FF7 0x00ddffa8
int g_MidiDeviceId = 0;
// GLOBAL: FF7 0x00ddffac
int g_MidiStreamHandle = 0;
// The index of the next stream buffer to come back (0..5).
// GLOBAL: FF7 0x00ddffb0
int g_MidiBufferIndex = 0;
// Set while the stream is stopping: returned buffers are not refilled.
// GLOBAL: FF7 0x00ddffb4
int g_MidiStreamEnded = 0;
// GLOBAL: FF7 0x00ddffb8
int g_MidiSoundFontActive = 0;
// GLOBAL: FF7 0x00ddffbc
int g_MidiSoundFontMemory = 0;
// GLOBAL: FF7 0x00ddffc0
int g_MidiOptions = 0;
// GLOBAL: FF7 0x00ddffc4
int g_MidiInitialized = 0;
// GLOBAL: FF7 0x00ddffc8
int g_MidiTargetVolume = 0;
// GLOBAL: FF7 0x00ddffcc
int g_MidiVolumeFadeActive = 0;
// The volume change per second of a volume fade.
// GLOBAL: FF7 0x00ddffd0
int g_MidiVolumeFadeRate = 0;
// GLOBAL: FF7 0x00ddffd4
int g_MidiCrossFadePending = 0;
// clang-format on
#define midiTrackCount (g_MidiFormatAndTrackCount[1])

#pragma intrinsic(strcpy)

__declspec(dllimport) void __stdcall EnterCriticalSection(void *);
__declspec(dllimport) void __stdcall LeaveCriticalSection(void *);
__declspec(dllimport) void __stdcall DeleteCriticalSection(void *);
__declspec(dllimport) int __stdcall midiOutSetVolume(int, int);
__declspec(dllimport) int __stdcall midiStreamProperty(int, void *, unsigned int);
__declspec(dllimport) int __stdcall midiStreamOpen(int *, int *, int, void *, int, int);
__declspec(dllimport) int __stdcall midiStreamStop(int);
__declspec(dllimport) int __stdcall midiStreamClose(int);
__declspec(dllimport) int __stdcall midiStreamPause(int);
__declspec(dllimport) int __stdcall midiStreamRestart(int);
#define MIDI_LOCK ((void *)&g_MidiCriticalSection)

#pragma intrinsic(memcpy)
__declspec(dllimport) int __stdcall midiOutPrepareHeader(int, void *, int);
__declspec(dllimport) int __stdcall midiOutUnprepareHeader(int, void *, int);
__declspec(dllimport) int __stdcall midiStreamOut(int, void *, int);
__declspec(dllimport) void __stdcall OutputDebugStringA(const char *);

#pragma intrinsic(strcmp)
#pragma intrinsic(strcat)
__declspec(dllimport) int __stdcall timeGetTime(void);
__declspec(dllimport) int __stdcall midiOutOpen(int *, int, int, int, int);
__declspec(dllimport) int __stdcall midiOutLongMsg(int, void *, int);
__declspec(dllimport) int __stdcall midiOutClose(int);
#pragma pack(push, 4)
struct MidiTempoFrame {
    double factor;
    int tempo;
    double value;
};
#pragma pack(pop)

// Frees the stream buffer g_MidiStreamBuffer and clears it: an inline function (original line 0x49B) that VC5 expands
// in its six callers; its literal is one, emitted after the TU's functions (retail 0x967644).
__inline void MidiFreeStreamBuffer(void)
{
    // STRING: FF7 0x00967644
    GameHeapFree((void *)g_MidiStreamBuffer, "C:\\lib\\src\\sound\\midi1.cpp", 1179);
    g_MidiStreamBuffer = 0;
}

// These are views inside the existing 13-byte bank-path literal copied below.
// STRING: FF7 0x00967074
typedef char MidiBankStrSlice01[sizeof("\\lb2.sf2")];
// STRING: FF7 0x00967078
typedef char MidiBankStrSlice02[sizeof(".sf2")];

__declspec(dllimport) int __stdcall midiOutGetDevCapsA(int, void *, int);
__declspec(dllimport) int __stdcall midiOutGetVolume(int, void *);
__declspec(dllimport) void __stdcall InitializeCriticalSection(void *);

/*
 * Initializes MIDI playback from the files under dataDir. The caller passes a second argument
 * (the engine context's word +0x5C) that the body never reads.
 */
// FUNCTION: FF7 0x00741780
int __cdecl MidiInit(char *dataDir, int unused)
{
    struct {
        char archiveLog[128];
        char bankLog[128];
        unsigned int bankSize;
        char bankPath[128];
        char caps[52];
        char mode[64];
        char log[256];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    g_MidiDebugLog = 1;
    Gfx_DebugPrint(
        // STRING: FF7 0x00966d50
        "INITIALIZING MIDI...\n");
    MidiSetDataDir(dataDir);
    g_MusicTrack1Requested = 0;
    if (g_MidiInitialized) {
        if (g_MidiDebugLog)
            Gfx_DebugPrint(
                // STRING: FF7 0x00966d68
                "...Again!!!\n");
        return 1;
    }
    if (!RegGetMidiDeviceId(&g_MidiDeviceId)) {
        if (g_MidiDebugLog)
            Gfx_DebugPrint(
                // STRING: FF7 0x00966d78
                "no registry entry... selecting MIDI_MAPPER\n");
        g_MidiDeviceId = -1;
    }
    if (RegGetMidiOptions(&g_MidiOptions))
        g_MidiUseVolumeCurve = g_MidiOptions & 1;
    MmCheckError(midiOutGetDevCapsA(g_MidiDeviceId, frame.caps, 52),
                 // STRING: FF7 0x00966da4
                 "C:\\lib\\src\\sound\\midi1.cpp", 180);
    if (g_MidiDebugLog) {
        sprintf(frame.log,
                // STRING: FF7 0x00966dc0
                "selecting device %d:%s, mid=%d, pid=%d,\n", g_MidiDeviceId, frame.caps + 8, *(int *)frame.caps & 0xffff, *(int *)(frame.caps + 2) & 0xffff);
        Gfx_DebugPrint(frame.log);
    }
    g_MidiDeviceIsMid125 = (*(int *)frame.caps & 0xffff) == 125;
    if ((*(int *)frame.caps & 0xffff) == 2) {
        strcpy(frame.bankPath, dataDir);
        frame.bankSize = SoundFontOpenDevice(g_MidiDeviceId);
        if (frame.bankSize) {
            g_MidiSoundFontActive = 1;
            g_MidiSoundFontMemory = frame.bankSize;
            g_MidiSoundFontLoaded = 0;
            if (frame.bankSize >= 0x400000) {
                strcat(frame.bankPath,
                       // STRING: FF7 0x00966dec
                       "\\sf2\\ff7.sf2");
                strcpy(g_MidiSoundFontPath, frame.bankPath);
            } else if (frame.bankSize >= 0x200000) {
                strcat(frame.bankPath,
                       // STRING: FF7 0x00966dfc
                       "\\sf2\\ff7m.sf2");
                strcpy(g_MidiSoundFontPath, frame.bankPath);
            } else if (frame.bankSize >= 0x80000) {
                strcat(frame.bankPath,
                       // STRING: FF7 0x00966e0c
                       "\\sf2\\ff7s.sf2");
                strcpy(g_MidiSoundFontPath, frame.bankPath);
            } else
                goto after_soundfont;
            if (SoundFontLoadBank(frame.bankPath) && g_MidiDebugLog) {
                sprintf(frame.bankLog,
                        // STRING: FF7 0x00966e1c
                        "total sound memory: %d MB (%d bytes)\n", frame.bankSize >> 20, frame.bankSize);
                Gfx_DebugPrint(frame.bankLog);
                sprintf(frame.bankLog,
                        // STRING: FF7 0x00966e44
                        "loading soundfont %s\n", frame.bankPath);
                Gfx_DebugPrint(frame.bankLog);
            }
        }
    }
after_soundfont:
    if (&g_MidiDataDir)
        strcpy((char *)&g_MidiDataPath, &g_MidiDataDir);
    g_MidiDataType = MIDI_DATA_GENERAL_MIDI;
    if (RegGetMidiDataType(frame.mode)) {
        if (!strcmp(frame.mode,
                    // STRING: FF7 0x00966e5c
                    "GENERAL_MIDI"))
            g_MidiDataType = MIDI_DATA_GENERAL_MIDI;
        else if (!strcmp(frame.mode,
                         // STRING: FF7 0x00966e6c
                         "SOUNDFONT_MIDI"))
            g_MidiDataType = MIDI_DATA_SOUNDFONT_MIDI;
        else if (!strcmp(frame.mode,
                         // STRING: FF7 0x00966e7c
                         "YAMAHA_XG"))
            g_MidiDataType = MIDI_DATA_YAMAHA_XG;
        else if (!strcmp(frame.mode,
                         // STRING: FF7 0x00966e88
                         "YAMAHA_GM"))
            g_MidiDataType = MIDI_DATA_YAMAHA_GM;
    }
    switch (g_MidiDataType) {
    case MIDI_DATA_SOUNDFONT_MIDI:
        strcat((char *)&g_MidiDataPath,
               // STRING: FF7 0x00966e94
               "\\awe.lgp");
        break;
    case MIDI_DATA_GENERAL_MIDI:
        if (!strcmp(frame.caps + 8,
                    // STRING: FF7 0x00966ea0
                    "YAMAHA SXG70 Driver")) {
            g_MidiDataType = MIDI_DATA_YAMAHA_GM;
            strcat((char *)&g_MidiDataPath,
                   // STRING: FF7 0x00966eb4
                   "\\ygm.lgp");
        } else {
            strcat((char *)&g_MidiDataPath,
                   // STRING: FF7 0x00966ec0
                   "\\midi.lgp");
            MidiSendGmReset();
        }
        break;
    case MIDI_DATA_YAMAHA_GM:
        strcat((char *)&g_MidiDataPath,
               // STRING: FF7 0x00966ecc
               "\\ygm.lgp");
        break;
    case MIDI_DATA_YAMAHA_XG:
        strcat((char *)&g_MidiDataPath,
               // STRING: FF7 0x00966ed8
               "\\xg.lgp");
        break;
    }
    switch (g_MidiDataType) {
    case MIDI_DATA_GENERAL_MIDI:
        Gfx_DebugPrint(
            // STRING: FF7 0x00966ee0
            "midi data type: GENERAL MIDI\n");
        break;
    case MIDI_DATA_SOUNDFONT_MIDI:
        Gfx_DebugPrint(
            // STRING: FF7 0x00966f00
            "midi data type: AWE SOUNDFONT MIDI\n");
        break;
    case MIDI_DATA_YAMAHA_XG:
        Gfx_DebugPrint(
            // STRING: FF7 0x00966f24
            "midi data type: YAMAHA XG MIDI\n");
        break;
    case MIDI_DATA_YAMAHA_GM:
        Gfx_DebugPrint(
            // STRING: FF7 0x00966f44
            "midi data type: YAMAHA GM\n");
        break;
    }
    if (g_MidiDebugLog) {
        sprintf(frame.archiveLog,
                // STRING: FF7 0x00966f60
                "using midi data file: %s\n", (char *)&g_MidiDataPath);
        Gfx_DebugPrint(frame.archiveLog);
    }
    if (!OpenLgpArchive((char *)&g_MidiDataPath, LGP_ARCHIVE_MIDI)) {
        sprintf(frame.log,
                // STRING: FF7 0x00966f7c
                "Failed to load: %s\n", (char *)&g_MidiDataPath);
        Gfx_DebugPrint(frame.log);
        return 0;
    }
    InitializeCriticalSection(MIDI_LOCK);
    g_MidiTargetVolume = 127;
    g_MidiVolume = g_MidiTargetVolume;
    g_MidiMasterVolume = 100;
    g_MidiBaseTempo = 64;
    g_MidiCrossFadePending = 0;
    g_MidiCrossFadeActive = 0;
    g_MidiVolumeFadeActive = 0;
    g_MidiFailed = 0;
    g_MidiFreeBufferCount = 6;
    g_MidiClosePending = 0;
    if (MidiStreamOpenDevice()) {
        midiOutGetVolume(g_MidiStreamHandle, &g_MidiDriverVolume);
        if (!g_MidiDriverVolume)
            g_MidiDriverVolume = -1;
        g_MidiVolumeControl = MmCheckError(midiOutSetVolume(g_MidiStreamHandle, g_MidiDriverVolume),
                                    // STRING: FF7 0x00966f90
                                    "C:\\lib\\src\\sound\\midi1.cpp", 356);
        sprintf(frame.log,
                // STRING: FF7 0x00966fac
                "midiOutGetVolume returned: %x\n", g_MidiDriverVolume);
        Gfx_DebugPrint(frame.log);
        if (!g_MidiVolumeControl)
            Gfx_DebugPrint(
                // STRING: FF7 0x00966fcc
                "Volume control unsupported\n");
        g_MidiDriverVolume &= 0xffff;
        g_MidiInitialized = 1;
        MidiStreamCloseDevice();
        g_MidiStreamHandle = 0;
        g_MidiPlaybackState = MIDI_PLAYBACK_STOPPED;
        if (g_MidiDebugLog)
            Gfx_DebugPrint(
                // STRING: FF7 0x00966fe8
                "MIDI INITIALIZED\n");
    } else if (g_MidiDebugLog)
        Gfx_DebugPrint(
            // STRING: FF7 0x00966ffc
            "MIDI INIT FAILED - SADNESS :(\n");
    return g_MidiInitialized;
}

/*
 * Shuts the MIDI player down: stops the song, writes the driver volume back, frees the song buffer,
 * closes the SoundFont device when one was open, deletes the MIDI lock and closes the MIDI LGP
 * archive (11). Does nothing when MIDI is not initialised.
 */
// FUNCTION: FF7 0x00741f5f
void __cdecl MidiShutdown(void)
{
    if (g_MidiInitialized == 0) {
        return;
    }
    MidiStop();
    // STRING: FF7 0x0096701c
    Gfx_DebugPrint("resetting MIDI driver volume");
    MidiStreamOpenDevice();
    g_MidiDriverVolume |= g_MidiDriverVolume << 16;
    MidiApplyDriverVolume(g_MidiDriverVolume);
    MidiStreamCloseDevice();
    // STRING: FF7 0x0096703c
    Gfx_DebugPrint(" - OK\n");
    if (g_MidiStreamBuffer != 0) {
        MidiFreeStreamBuffer();
    }
    if (g_MidiSoundFontActive != 0) {
        SoundFontCloseDevice();
    }
    DeleteCriticalSection(MIDI_LOCK);
    CloseLgpArchive(LGP_ARCHIVE_MIDI);
    g_MidiInitialized = 0;
}

/*
 * Sets the MIDI data directory the SoundFont paths start from (`<dir>\sf2\lb2.sf2`); a null pointer
 * clears it.
 */
// FUNCTION: FF7 0x00742018
void __cdecl MidiSetDataDir(char *dataDir)
{
    if (dataDir != 0) {
        strcpy(&g_MidiDataDir, dataDir);
    } else {
        g_MidiDataDir = 0;
    }
}

/*
 * Plays song `song` (song 1 plays as 2) and returns at once when that song is already playing. On a
 * SoundFont device with at least 2 MB it loads `sf2\lb2.sf2` for song 81, and the default bank
 * again for the next song. Then it stops and closes the stream, reads the song from the MIDI LGP
 * and starts it, logging the result.
 */
// FUNCTION: FF7 0x00742055
void __cdecl MidiPlaySong(int song, int unused1, int unused2)
{
    struct {
        char reloadLog[128];
        char bankLog[128];
        char path[128];
        char log[128];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (!g_MidiInitialized || g_MidiFailed)
        return;
    // STRING: FF7 0x00967044
    sprintf(frame.log, "MIDI play: %d\n", song);
    Gfx_DebugPrint(frame.log);
    if (song == SOUND_SONG_OA)
        song = SOUND_SONG_OB;
    if (g_MidiCurrentSong == song && g_MidiPlaybackState == MIDI_PLAYBACK_PLAYING) {
        // STRING: FF7 0x00967054
        Gfx_DebugPrint("song is already playing...\n");
        return;
    }
    if (g_MidiSoundFontActive && g_MidiSoundFontMemory >= 0x200000) {
        if (song == SOUND_SONG_LB2) {
            strcpy(frame.path, &g_MidiDataDir);
            // STRING: FF7 0x00967070
            strcat(frame.path, "\\sf2\\lb2.sf2");
            if (g_MidiDebugLog) {
                // STRING: FF7 0x00967080
                sprintf(frame.bankLog, "loading soundfont %s\n", frame.path);
                Gfx_DebugPrint(frame.bankLog);
            }
            if (SoundFontLoadBank(frame.path)) {
                g_MidiSoundFontLoaded = 1;
            } else if (g_MidiDebugLog) {
                // STRING: FF7 0x00967098
                Gfx_DebugPrint("error loading lb2.sf2");
            }
        } else if (g_MidiSoundFontLoaded) {
            if (g_MidiDebugLog) {
                // STRING: FF7 0x009670b0
                sprintf(frame.reloadLog, "loading soundfont %s\n", g_MidiSoundFontPath);
                Gfx_DebugPrint(frame.reloadLog);
            }
            if (SoundFontLoadBank(g_MidiSoundFontPath))
                g_MidiSoundFontLoaded = 0;
        }
    }
    MidiStreamStopChecked();
    MidiStreamCloseDevice();
    if (g_MidiStreamBuffer) {
        MidiFreeStreamBuffer();
    }
    if (MidiLoadSongFromLgp(song)) {
        if (MidiStartPlayback()) {
            g_MidiCurrentSong = song;
            // STRING: FF7 0x009670c8
            Gfx_DebugPrint("MIDI is playing SUCCESSFULLY!!!\n");
            return;
        } else {
            // STRING: FF7 0x009670ec
            Gfx_DebugPrint("MIDI ERROR - playback aborted\n");
            MidiFreeStreamBuffer();
        }
    } else {
        // STRING: FF7 0x0096710c
        Gfx_DebugPrint("ERROR reading MIDI data\n");
    }
}

/*
 * Reads `<name>.mid` of song `song` from the MIDI LGP archive (11) into a new song buffer of at
 * most 0x14C00 bytes; LB2 reads `LB2_AWE` while the lb2 SoundFont is loaded. Returns 1 on success;
 * on failure the buffer is freed.
 */
// FUNCTION: FF7 0x007422b6
int __cdecl MidiLoadSongFromLgp(int song)
{
    struct {
        int result;
        char alternate[8];
        int file;
        char *name;
        char path[256];
        char log[128];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.result = 0;
    // STRING: FF7 0x00967128
    memcpy(frame.alternate, "LB2_AWE", sizeof(frame.alternate));
    frame.name = MidiGetSongName(song);
    // STRING: FF7 0x00967130
    if (!strcmp(frame.name, "LB2") && g_MidiSoundFontActive && g_MidiSoundFontLoaded == 1)
        frame.name = frame.alternate;
    strcpy(frame.path, frame.name);
    // STRING: FF7 0x00967134
    strcat(frame.path, ".mid");
    if (g_MidiDebugLog) {
        // STRING: FF7 0x0096713c
        sprintf(frame.log, "reading midi file: %s\n", frame.path);
        Gfx_DebugPrint(frame.log);
    }
    if (g_MidiStreamBuffer) {
        MidiFreeStreamBuffer();
    }
    frame.file = (int)FindLgpFile(frame.path, LGP_ARCHIVE_MIDI);
    if (frame.file) {
        g_MidiSongSize = GetLgpEntrySize(frame.file, LGP_ARCHIVE_MIDI);
        if (g_MidiSongSize > 84992) {
            // STRING: FF7 0x00967154
            Gfx_DebugPrint("MIDI LGP ERROR: file size exceeds maxmimum MIDI file size\n");
            g_MidiSongSize = 0x14C00;
        }
        // STRING: FF7 0x00967190
        g_MidiStreamBuffer = (int)GameHeapAllocRaw(g_MidiSongSize, "C:\\lib\\src\\sound\\midi1.cpp", 581);
        if (g_MidiStreamBuffer) {
            if (ReadLgpEntry((void *)frame.file, LGP_ARCHIVE_MIDI, (void *)g_MidiStreamBuffer, g_MidiSongSize)) {
                frame.result = 1;
                g_MidiReadCursor = g_MidiStreamBuffer;
            } else {
                MidiFreeStreamBuffer();
            }
        }
    }
    return frame.result;
}

/*
 * Bump-allocates size bytes from the MIDI file buffer cursor g_MidiReadCursor and returns the old
 * cursor.
 */
// FUNCTION: FF7 0x007424b7
int __cdecl MidiFileTake(int size)
{
    int cursor;

    cursor = g_MidiReadCursor;
    g_MidiReadCursor += size;
    return cursor;
}

/*
 * Parses the song buffer's MThd header and MTrk chunks into the track table, opens the stream
 * device, applies the volume and the time division, primes the first two stream buffers with an
 * expression reset and a pitch-bend reset on all 16 channels, fills and submits the other four and
 * restarts the stream. Returns 1 when the song plays.
 */
// FUNCTION: FF7 0x007424d9
int __cdecl MidiStartPlayback(void)
{
    struct {
        int scaledVolume;
        int volumeWord;
        char debugMsg[128];
        int propertySize;
        int timeDivision;
        int primedCount;
        SoundMidiChunkHeader *chunkHeader;
        int trackOrBufIdx;
        SoundMidiHeaderWords *headerWords;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    g_MidiLastTick = 0;
    g_MidiCurrentTick = 0;
    g_MidiReadCursor = g_MidiStreamBuffer;
    g_MidiBufferIndex = 0;
    g_MidiStreamEnded = 0;
    g_MidiLoopFlag = 0;
    g_MidiState = 0;
    g_MidiLoopStart = 0;
    g_MidiLoopEnd = 0;
    frame.chunkHeader = (SoundMidiChunkHeader *)MidiFileTake(8);
    if (frame.chunkHeader->chunkId != MAKE_FOURCC('M', 'T', 'h', 'd')) {
        return 0;
    } else {
        frame.headerWords = (SoundMidiHeaderWords *)MidiFileTake(6);
        g_MidiFormatAndTrackCount[0] = frame.headerWords->format >> 8 | frame.headerWords->format << 8 & 0xFF00;
        midiTrackCount = frame.headerWords->trackCount >> 8 | frame.headerWords->trackCount << 8 & 0xFF00;
        g_MidiTimeDivision = frame.headerWords->division >> 8 | frame.headerWords->division << 8 & 0xFF00;
        for (frame.trackOrBufIdx = 0; frame.trackOrBufIdx < *(unsigned short *)&midiTrackCount; frame.trackOrBufIdx++) {
            frame.chunkHeader = (SoundMidiChunkHeader *)MidiFileTake(8);
            if (frame.chunkHeader->chunkId != MAKE_FOURCC('M', 'T', 'r', 'k')) {
                midiTrackCount = frame.trackOrBufIdx;
                break;
            }
            midiTrackState[frame.trackOrBufIdx].length = frame.chunkHeader->chunkLength >> 0x18 | frame.chunkHeader->chunkLength >> 8 & 0xFF00 | *(int *)&frame.chunkHeader->chunkLength << 8 & 0xFF0000 | *(int *)&frame.chunkHeader->chunkLength << 0x18 & 0xFF000000;
            midiTrackState[frame.trackOrBufIdx].data = MidiFileTake(midiTrackState[frame.trackOrBufIdx].length);
            midiTrackState[frame.trackOrBufIdx].cursor = (unsigned char *)midiTrackState[frame.trackOrBufIdx].data;
            midiTrackState[frame.trackOrBufIdx].runningStatus = 0;
            midiTrackState[frame.trackOrBufIdx].index = frame.trackOrBufIdx;
            midiTrackState[frame.trackOrBufIdx].ended = 0;
            midiTrackState[frame.trackOrBufIdx].tick = MidiReadVarLen(&midiTrackState[frame.trackOrBufIdx]);
        }
        MidiSelectNextTrack();
        if (MidiStreamOpenDevice() != 0) {
            // STRING: FF7 0x009671ac
            sprintf(frame.debugMsg, "current volume: %d\n", g_MidiVolume);
            Gfx_DebugPrint(frame.debugMsg);
            if (g_MidiVolumeControl != 0) {
                frame.volumeWord = (unsigned int)(g_MidiVolume * g_MidiMasterVolume) / (unsigned int)0x64;
                if (g_MidiUseVolumeCurve != 0) {
                    frame.scaledVolume = g_MidiVolumeCurve[frame.volumeWord];
                } else {
                    frame.scaledVolume = (unsigned int)(frame.volumeWord * 65535) / (unsigned int)0x7F;
                }
                frame.volumeWord = frame.scaledVolume;
                frame.volumeWord |= frame.volumeWord << 16;
                if (MidiApplyDriverVolume(frame.volumeWord) == 0) {
                    MidiStreamCloseDevice();
                    return 0;
                }
            }
            frame.propertySize = 8;
            frame.timeDivision = *(unsigned short *)&g_MidiTimeDivision;
            // STRING: FF7 0x009671c0
            if (MmCheckError(midiStreamProperty(g_MidiStreamHandle, (void *)&frame.propertySize, 0x80000001), "C:\\lib\\src\\sound\\midi1.cpp", 682) == 0) {
                MidiStreamCloseDevice();
                return 0;
            }
            frame.primedCount = 0;
            midiStreamBuffers[frame.primedCount].index = frame.primedCount;
            if (MidiPrimeExpressionReset(&g_MidiStreamHandle, &midiStreamBuffers[frame.primedCount]) != 0) {
                frame.primedCount++;
                g_MidiFreeBufferCount--;
            } else {
                g_MidiFreeBufferCount = 6;
                return 0;
            }
            midiStreamBuffers[frame.primedCount].index = frame.primedCount;
            if (MidiPrimePitchBendReset(&g_MidiStreamHandle, &midiStreamBuffers[frame.primedCount]) != 0) {
                frame.primedCount++;
                g_MidiFreeBufferCount--;
            } else {
                MidiUnprepareBuffers(frame.primedCount);
                return 0;
            }
            for (frame.trackOrBufIdx = frame.primedCount; frame.trackOrBufIdx < 6; frame.trackOrBufIdx++) {
                midiStreamBuffers[frame.trackOrBufIdx].index = frame.trackOrBufIdx;
                if (MidiFillStreamBuffer(&midiStreamBuffers[frame.trackOrBufIdx]) != 0) {
                    if (MidiSubmitStreamBuffer(&midiStreamBuffers[frame.trackOrBufIdx]) != 0) {
                        g_MidiFreeBufferCount--;
                    } else {
                        MidiStop();
                        return 0;
                    }
                } else {
                    break;
                }
            }
            if (g_MidiCrossFadePending != 0) {
                g_MidiCrossFadePending = 0;
                MidiSetVolumeTrans(0x7F, g_MidiFadeDuration);
            }
            g_MidiPlaybackState = MIDI_PLAYBACK_PLAYING;
            // STRING: FF7 0x009671dc
            if (MmCheckError(midiStreamRestart(g_MidiStreamHandle), "C:\\lib\\src\\sound\\midi1.cpp", 743) == 0) {
                MidiUnprepareBuffers(frame.primedCount);
                return 0;
            }
        } else {
            return 0;
        }
        return 1;
    }
}

/*
 * Prepares a stream buffer's MIDIHDR (data at +0x40, 0x400 bytes) and queues it with midiStreamOut,
 * preparing and queueing it once more when that fails. Returns 0 when the first prepare fails, else
 * 1.
 */
// FUNCTION: FF7 0x007429d0
int __cdecl MidiSubmitStreamBuffer(MidiStreamBuffer *buffer)
{
    MidiStreamBuffer *header;
    MidiStreamBuffer *retryHdr;

    header = buffer;
    header->dataPtr = (int)buffer + 0x40;
    header->dataSize = 1024;
    header->bytesRecorded = buffer->filledBytes;
    header->flags = 0;
    header->bufferBase = (int)buffer;
    if (midiOutPrepareHeader(g_MidiStreamHandle, (void *)header, 0x40) == 0) {
        if (midiStreamOut(g_MidiStreamHandle, (void *)header, 0x40) != 0) {
            midiOutUnprepareHeader(g_MidiStreamHandle, (void *)header, 0x40);
            retryHdr = buffer;
            retryHdr->dataPtr = (int)buffer + 0x40;
            retryHdr->dataSize = 1024;
            retryHdr->bytesRecorded = buffer->filledBytes;
            retryHdr->flags = 0;
            retryHdr->bufferBase = (int)buffer;
            if (midiOutPrepareHeader(g_MidiStreamHandle, (void *)retryHdr, 0x40) == 0) {
                if (midiStreamOut(g_MidiStreamHandle, (void *)retryHdr, 0x40) != 0) {
                    midiOutUnprepareHeader(g_MidiStreamHandle, (void *)retryHdr, 0x40);
                }
            }
        }
    } else {
        midiOutUnprepareHeader(g_MidiStreamHandle, (void *)header, 0x40);
        return 0;
    }
    return 1;
}

/*
 * Makes the unfinished track with the smallest pending delta time the current track
 * (g_MidiCurrentTrack) and records that delta. Returns 0 when no track is left.
 */
// FUNCTION: FF7 0x00742af9
int __cdecl MidiSelectNextTrack(void)
{
    int best;
    int trackIndex;
    int smallest;
    int found;

    best = 0;
    smallest = 0xFFFFFFFF;
    found = 0;
    for (trackIndex = 0; trackIndex < *(unsigned short *)&midiTrackCount; trackIndex++) {
        if (midiTrackState[trackIndex].ended != 0) {
            continue;
        }
        if ((unsigned int)midiTrackState[trackIndex].tick < (unsigned int)smallest) {
            best = trackIndex;
            g_MidiCurrentTick = midiTrackState[best].tick;
            g_MidiCurrentTrack = best;
            found = 1;
            smallest = midiTrackState[trackIndex].tick;
        }
    }
    return found;
}

/*
 * Reads a MIDI variable-length number (seven bits a byte, the high bit set on all but the last) at
 * a track's read cursor and advances the cursor.
 */
// FUNCTION: FF7 0x00742b96
int __cdecl MidiReadVarLen(MidiTrack *track)
{
    unsigned char byte;
    int val;

    val = 0;
    do {
        byte = *track->cursor;
        track->cursor++;
        val = val << 7 | byte & 0x7F;
    } while ((byte & 0x80) != 0);
    return val;
}

/*
 * Cross-fades to song `song` over `duration`: song 0 fades out and stops, songs 1 and 2 keep
 * whichever of the two is playing. Without driver volume control it stops or plays at once. A song
 * that is already playing fades up; while another one plays, that one fades down over half the time
 * and the new one is queued; otherwise the new song starts quiet and fades up.
 */
// FUNCTION: FF7 0x00742bee
void __cdecl MidiCrossFade(int song, int duration)
{
    char log[128];
    if (!g_MidiInitialized || g_MidiFailed)
        return;
    // STRING: FF7 0x009671f8
    sprintf(log, "MIDI cross fade: %d, time: %d\n", song, duration);
    Gfx_DebugPrint(log);
    g_MidiCrossFadePending = 0;
    g_MidiCrossFadeActive = 0;
    if (song == 0) {
        if (!g_MidiVolumeControl) {
            MidiStop();
            return;
        }
        g_MidiCrossFadeActive = 1;
        MidiSetVolumeTrans(0, duration);
        return;
    }
    switch (song) {
    case SOUND_SONG_OA:
        song = g_MidiCurrentSong == SOUND_SONG_OB ? SOUND_SONG_OB : SOUND_SONG_OA;
        break;
    case SOUND_SONG_OB:
        song = g_MidiCurrentSong == SOUND_SONG_OA ? SOUND_SONG_OA : SOUND_SONG_OB;
        break;
    }
    if (!g_MidiVolumeControl) {
        MidiPlaySong(song, 0, 0);
        return;
    }
    if (g_MidiCurrentSong == song && g_MidiPlaybackState == MIDI_PLAYBACK_PLAYING) {
        g_MidiFadeDuration = duration;
        MidiSetVolumeTrans(127, g_MidiFadeDuration);
    } else if (MidiIsPlaying()) {
        g_MidiCrossFadePending = 1;
        g_MidiFadeDuration = duration / 2;
        g_MidiPendingSong = song;
        MidiSetVolumeTrans(15, g_MidiFadeDuration);
    } else {
        g_MidiFadeDuration = duration;
        g_MidiVolume = 15;
        MidiSetVolumeTrans(127, g_MidiFadeDuration);
        MidiPlaySong(song, 0, 0);
    }
}

/*
 * Pauses the MIDI stream (MIDI_PLAYBACK_PAUSED).
 */
// FUNCTION: FF7 0x00742d7b
void __cdecl MidiPause(void)
{
    if (g_MidiInitialized == 0 || g_MidiFailed != 0) { /* empty first arm: lib/codegen.h */
    } else {
        // STRING: FF7 0x00967218
        Gfx_DebugPrint("MIDI pause\n");
        if (g_MidiStreamHandle != 0) {
            // STRING: FF7 0x00967224
            MmCheckError(midiStreamPause(g_MidiStreamHandle), "C:\\lib\\src\\sound\\midi1.cpp", 924);
            g_MidiPlaybackState = MIDI_PLAYBACK_PAUSED;
        }
    }
}

/*
 * Restarts the paused MIDI stream (MIDI_PLAYBACK_PLAYING).
 */
// FUNCTION: FF7 0x00742dd3
void __cdecl MidiResume(void)
{
    if (g_MidiInitialized == 0 || g_MidiFailed != 0) { /* empty first arm: lib/codegen.h */
    } else {
        // STRING: FF7 0x00967240
        Gfx_DebugPrint("MIDI resume\n");
        if (g_MidiStreamHandle != 0) {
            // STRING: FF7 0x00967250
            MmCheckError(midiStreamRestart(g_MidiStreamHandle), "C:\\lib\\src\\sound\\midi1.cpp", 938);
            g_MidiPlaybackState = MIDI_PLAYBACK_PLAYING;
        }
    }
}

/*
 * Stops MIDI playback when a song is loaded and no stop is pending: logs, clears the playing flag,
 * stops and closes the stream, frees the song buffer g_MidiStreamBuffer, and resets the volume words to
 * 0x7F.
 */
// FUNCTION: FF7 0x00742e2b
void __cdecl MidiStop(void)
{
    if (g_MidiInitialized == 0 || g_MidiFailed != 0) { /* empty first arm: lib/codegen.h */
    } else {
        // STRING: FF7 0x0096726c
        Gfx_DebugPrint("MIDI stop");
        g_MidiCrossFadePending = 0;
        MidiStreamStopChecked();
        MidiStreamCloseDevice();
        if (g_MidiStreamBuffer != 0) {
            MidiFreeStreamBuffer();
        }
        // STRING: FF7 0x00967278
        Gfx_DebugPrint(" - OK\n");
        g_MidiTargetVolume = 0x7F;
        g_MidiVolume = g_MidiTargetVolume;
    }
}

/*
 * 0x0061FC8B tests the return in eax, so it is not void.
 */
// FUNCTION: FF7 0x00742eb3
int __cdecl MidiIsPlaying(void)
{
    int val = g_MidiPlaybackState;
    if (val != MIDI_PLAYBACK_PLAYING && val != 3) { /* empty first arm: lib/codegen.h */
    } else {
        return 1;
    }
    return 0;
}

/*
 * Sets the master volume percentage (at most 100) and applies it to the current volume.
 */
// FUNCTION: FF7 0x00742eda
void __cdecl MidiSetMasterVolume(unsigned int volume)
{
    char buffer[128];
    if (g_MidiInitialized == 0)
        return;
    if (volume > 100)
        volume = 100;
    // STRING: FF7 0x00967280
    sprintf(buffer, "MIDI set master volume: %d\n", volume);
    Gfx_DebugPrint(buffer);
    g_MidiMasterVolume = volume;
    MidiSetVolume(g_MidiVolume);
}

/*
 * Sets the song volume (0..127) at once: scales it by the master percentage, maps it through
 * g_MidiVolumeCurve when the curve is on (else linearly to 0..0xFFFF) and writes it to both driver
 * channels. Needs driver volume control.
 */
// FUNCTION: FF7 0x00742f37
void __cdecl MidiSetVolume(int level)
{
    struct {
        int selected;
        char buffer[128];
        int volume;
    } frame;
    if (g_MidiInitialized == 0 || (g_MidiVolumeControl == 0 || g_MidiFailed != 0)) { /* empty first arm: lib/codegen.h */
    } else {
        // STRING: FF7 0x0096729c
        sprintf(frame.buffer, "MIDI set volume: %d\n", level);
        Gfx_DebugPrint(frame.buffer);
        if ((unsigned int)level > (unsigned int)0x7F) {
            level = 0x7F;
        }
        g_MidiTargetVolume = level;
        g_MidiVolume = g_MidiTargetVolume;
        frame.volume = (unsigned int)(g_MidiVolume * g_MidiMasterVolume) / (unsigned int)0x64;
        // STRING: FF7 0x009672b4
        sprintf(frame.buffer, "%d%% of %d = %d\n", g_MidiMasterVolume, level, frame.volume);
        Gfx_DebugPrint(frame.buffer);
        if (g_MidiUseVolumeCurve != 0) {
            frame.selected = g_MidiVolumeCurve[frame.volume];
        } else {
            frame.selected = (unsigned int)(frame.volume * 65535) / (unsigned int)0x7F;
        }
        frame.volume = frame.selected;
        frame.volume |= frame.volume << 16;
        MidiApplyDriverVolume(frame.volume);
    }
}

/*
 * Ramps the volume from the current one to `volume` over `step` sixtieths of a second, or sets it
 * at once for a step below 10. A zero step sets it at once and stops the song at volume 0.
 */
// FUNCTION: FF7 0x0074304c
void __cdecl MidiSetVolumeTrans(int volume, int step)
{
    char buffer[128];
    if (g_MidiInitialized == 0 || g_MidiFailed != 0) { /* empty first arm: lib/codegen.h */
    } else if (step == 0) {
        if (volume == 0) {
            MidiStop();
            return;
        }
        MidiSetVolume(volume);
    } else {
        if ((unsigned int)volume > (unsigned int)0x7F) {
            volume = 0x7F;
        }
        // STRING: FF7 0x009672c8
        sprintf(buffer, "MIDI set volume trans: %d->%d; step=%d\n", g_MidiVolume, volume, step);
        Gfx_DebugPrint(buffer);
        g_MidiTargetVolume = volume;
        if (step >= 10) {
            g_MidiVolumeFadeRate = (int)((g_MidiTargetVolume - g_MidiVolume) / (step / 60.0));
        } else {
            MidiSetVolume(volume);
            if (volume == 0) {
                MidiStop();
            }
        }
        g_MidiVolumeFadeLastTime = timeGetTime();
        g_MidiVolumeFadeActive = 1;
        MidiUpdateVolumeFade();
    }
}

/*
 * Sets the volume to `start` and ramps it to `end` over `duration` (both at most 127).
 */
// FUNCTION: FF7 0x0074313d
void __cdecl MidiFadeVolume(int duration, unsigned int start, unsigned int end)
{
    char buffer[128];
    if (g_MidiInitialized == 0 || g_MidiFailed != 0) { /* empty first arm: lib/codegen.h */
    } else {
        // STRING: FF7 0x009672f0
        sprintf(buffer, "MIDI fade volume: %d->%d, step=%d\n", start, end, duration);
        Gfx_DebugPrint(buffer);
        if (start > 127)
            start = 127;
        if (end > 127)
            end = 127;
        MidiSetVolume(start);
        MidiSetVolumeTrans(end, duration);
    }
}

/*
 * Applies a tempo level: the stream tempo (microseconds per quarter note) is the song's base tempo
 * times 1 - level/128 for a negative level (up to twice as slow) and 1 - level/254 for a positive
 * one (up to twice as fast).
 */
// FUNCTION: FF7 0x007431bd
void __cdecl MidiSetTempo(char tempoLevel)
{
    struct MidiTempoFrame frame;
    if (!g_MidiInitialized || g_MidiFailed)
        return;
    frame.factor = (frame.value = tempoLevel) < 0.0 ? frame.value / -128.0 + 1.0 : 1.0 - 0.5 * frame.value / 127.0;
    frame.tempo = (int)(g_MidiBaseTempo * frame.factor);
    // STRING: FF7 0x00967314
    Gfx_DebugPrint("MIDI set tempo");
    if (MidiStreamSetTempo(frame.tempo)) {
        // STRING: FF7 0x00967324
        Gfx_DebugPrint(" - OK\n");
    } else {
        // STRING: FF7 0x0096732c
        Gfx_DebugPrint(" - FAILED\n");
    }
}

/*
 * When MIDI is up (g_MidiInitialized) and not suspended (g_MidiFailed), logs the format string at
 * 0x00967338 (a NOT IMPLEMENTED notice) to the active debug target and sets the tempo to tempo.
 */
// FUNCTION: FF7 0x0074327a
void __cdecl MidiSetTempoTrans(int tempo)
{
    if (g_MidiInitialized == 0 || g_MidiFailed != 0) { /* empty first arm: lib/codegen.h */
    } else {
        // STRING: FF7 0x00967338
        Gfx_DebugPrint("MIDI set tempo trans - NOT IMPLEMENTED\n");
        MidiSetTempo((char)tempo);
    }
}

/*
 * When MIDI is up and not suspended, logs the literal at 0x00967360 (NOT IMPLEMENTED) and sets the tempo to
 * tempo outright. duration and fadeMode are never read: there is no fade in the image, only the final
 * tempo.
 */
// FUNCTION: FF7 0x007432ac
void __cdecl MidiFadeTempo(int duration, char fadeMode, char tempo)
{
    if (g_MidiInitialized == 0 || g_MidiFailed != 0) { /* empty first arm: lib/codegen.h */
    } else {
        // STRING: FF7 0x00967360
        Gfx_DebugPrint("MIDI fade tempo - NOT IMPLEMENTED\n");
        MidiSetTempo((char)tempo);
    }
}

/*
 * MidiSetPitch is not implemented: it only logs its name.
 */
// FUNCTION: FF7 0x007432de
void __cdecl MidiSetPitch(void)
{
    // STRING: FF7 0x00967384
    Gfx_DebugPrint("MIDI set pitch - NOT IMPLEMENTED\n");
}

/*
 * MidiSetPitchTrans is not implemented: it only logs its name.
 */
// FUNCTION: FF7 0x007432f0
void __cdecl MidiSetPitchTrans(void)
{
    // STRING: FF7 0x009673a8
    Gfx_DebugPrint("MIDI set pitch trans - NOT IMPLEMENTED\n");
}

/*
 * MidiFadePitch is not implemented: it only logs its name.
 */
// FUNCTION: FF7 0x00743302
void __cdecl MidiFadePitch(void)
{
    // STRING: FF7 0x009673d0
    Gfx_DebugPrint("MIDI fade pitch - NOT IMPLEMENTED\n");
}

/*
 * MidiSetPan is not implemented: it only logs its name.
 */
// FUNCTION: FF7 0x00743314
void __cdecl MidiSetPan(void)
{
    // STRING: FF7 0x009673f4
    Gfx_DebugPrint("MIDI set pan - NOT IMPLEMENTED\n");
}

/*
 * MidiSetPanTrans is not implemented: it only logs its name.
 */
// FUNCTION: FF7 0x00743326
void __cdecl MidiSetPanTrans(void)
{
    // STRING: FF7 0x00967414
    Gfx_DebugPrint("MIDI set pan trans - NOT IMPLEMENTED\n");
}

/*
 * MidiFadePan is not implemented: it only logs its name.
 */
// FUNCTION: FF7 0x00743338
void __cdecl MidiFadePan(void)
{
    // STRING: FF7 0x0096743c
    Gfx_DebugPrint("MIDI fade pan - NOT IMPLEMENTED\n");
}

/*
 * Fills a stream buffer with converted events until 0x338 bytes are used or no track is left (then
 * it marks the song finished). Returns whether any event was written.
 */
// FUNCTION: FF7 0x0074334a
int __cdecl MidiFillStreamBuffer(MidiStreamBuffer *stream)
{
    int result;

    result = 0;
    stream->filledBytes = 0;
    if (g_MidiStreamEnded != 0) {
        return result;
    } else {
        while (stream->filledBytes < 824) {
            if (MidiConvertNextEvent(stream) != 0) {
                result = 1;
            }
            if (MidiSelectNextTrack() == 0) {
                g_MidiStreamEnded = 1;
                break;
            }
        }
        return result;
    }
}

/*
 * Converts the current track's next event into a stream event at the buffer's fill offset: sysex
 * as a long event, a tempo meta event as a tempo change, the `loopStart`/`loopEnd`
 * markers as the loop, and channel messages with running status. The end-of-track meta event marks
 * the track finished. Returns whether it wrote an event.
 */
// FUNCTION: FF7 0x007433b4
int __cdecl MidiConvertNextEvent(MidiStreamBuffer *stream)
{

    struct {
        int track_index;
        char text[64];
        int *length_word;
        unsigned int byte_index;
        int padded;
        int event_size;
        int event[4];
        unsigned char *out;
        int emitted;
        unsigned char bytes[4];
        unsigned int length;
        unsigned char status;
        MidiTrack *track;
        int *output;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.emitted = 0;
    frame.track = &midiTrackState[g_MidiCurrentTrack];
    if (frame.track->ended != 0)
        return 0;
    frame.status = *frame.track->cursor;
    frame.track->cursor++;
    if (frame.status < 0x80) {
        if (frame.track->runningStatus == 0)
            return 0;
        frame.status = frame.track->runningStatus;
        frame.track->cursor--;
    }
    frame.track->runningStatus = frame.status;
    frame.output = (int *)&stream->data[stream->filledBytes];
    if (frame.status == MIDI_STATUS_SYSEX) {
        frame.length = MidiReadVarLen(frame.track);
        *frame.output = frame.track->tick - g_MidiLastTick;
        frame.output++;
        *frame.output = 0;
        frame.output++;
        frame.length_word = frame.output;
        *frame.output = (frame.length & 0xffffff) | 0x80000000;
        frame.output++;
        frame.out = (unsigned char *)frame.output;
        *frame.out = frame.status;
        frame.out++;
        for (frame.byte_index = 0; frame.byte_index < frame.length; frame.byte_index++) {
            if ((*frame.out++ = *frame.track->cursor++) == MIDI_STATUS_SYSEX_END)
                break;
        }
        frame.padded = frame.length + 1;
        while (frame.padded % 4) {
            *frame.out = 0;
            frame.out++;
            frame.padded++;
        }
        *frame.length_word = (frame.padded & 0xffffff) | 0x80000000;
        stream->filledBytes = stream->filledBytes + frame.padded + 12;
        g_MidiLastTick = frame.track->tick;
        frame.track->tick += MidiReadVarLen(frame.track);
        return 1;
    } else if (frame.status == MIDI_STATUS_META) {
        frame.bytes[0] = frame.status;
        frame.bytes[1] = *frame.track->cursor;
        frame.track->cursor++;
        frame.length = MidiReadVarLen(frame.track);
        frame.status = frame.bytes[1];
        switch ((int)frame.status) {
        case MIDI_META_END_OF_TRACK:
            frame.track->ended = 1;
            frame.track->cursor += frame.length;
            return 0;
        case MIDI_META_SET_TEMPO:
            g_MidiBaseTempo = (frame.track->cursor[0] << 16) | (frame.track->cursor[1] << 8) | frame.track->cursor[2];
            frame.event[2] = g_MidiBaseTempo;
            frame.event[2] |= 0x1000000;
            frame.event_size = 12;
            frame.emitted = 1;
            break;
        case MIDI_META_MARKER:
            strncpy(frame.text, (char *)frame.track->cursor, frame.length);
            frame.text[frame.length] = 0;
            // STRING: FF7 0x00967460
            if (strncmp(frame.text, "loopEnd", 7) == 0) {
                g_MidiLoopEnd = frame.track->tick;
                MidiEmitLoopRestart(stream);
                return 0;
            }
            // STRING: FF7 0x00967468
            if (strcmp(frame.text, "loopStart") == 0) {
                g_MidiLoopFlag = 1;
                g_MidiLoopStart = frame.track->tick;
                frame.track->cursor += frame.length;
                frame.track->tick += MidiReadVarLen(frame.track);
                for (frame.track_index = 0; frame.track_index < *(unsigned short *)&midiTrackCount; frame.track_index++) {
                    ((MidiTrack *)&midiSavedTrackState)[frame.track_index] = midiTrackState[frame.track_index];
                }
                return 0;
            }
            break;
        case MIDI_META_COPYRIGHT:
            break;
        default:
            break;
        }
        frame.track->cursor += frame.length;
    } else {
        frame.bytes[0] = frame.status;
        frame.track->runningStatus = frame.status;
        frame.status &= 0xf0;
        frame.bytes[1] = *frame.track->cursor;
        frame.track->cursor++;
        frame.bytes[2] = 0;
        if (frame.status == MIDI_STATUS_PROGRAM_CHANGE) {
            frame.event[2] = frame.bytes[0] | (frame.bytes[1] << 8) | (frame.bytes[2] << 16);
            frame.event_size = 12;
            frame.emitted = 1;
        } else if (frame.status == MIDI_STATUS_CHANNEL_PRESSURE) { /* empty first arm: lib/codegen.h */
        } else {
            frame.bytes[2] = *frame.track->cursor;
            frame.track->cursor++;
            // Separate empty arms retain VC5's original hidden BYTE temporary.
            switch (frame.status) {
            case MIDI_STATUS_NOTE_OFF:
                break;
            case MIDI_STATUS_NOTE_ON:
                break;
            case MIDI_STATUS_POLY_PRESSURE:
                break;
            case MIDI_STATUS_CONTROL_CHANGE:
                break;
            case MIDI_STATUS_PITCH_BEND:
                break;
            default:
                break;
            }
            frame.event[2] = frame.bytes[0] | (frame.bytes[1] << 8) | (frame.bytes[2] << 16);
            frame.event_size = 12;
            frame.emitted = 1;
        }
    }
    if (frame.emitted) {
        frame.event[0] = frame.track->tick - g_MidiLastTick;
        frame.event[1] = 0;
        memcpy(&stream->data[stream->filledBytes], frame.event, frame.event_size);
        stream->filledBytes += frame.event_size;
        g_MidiLastTick = frame.track->tick;
    }
    frame.track->tick += MidiReadVarLen(frame.track);
    return frame.emitted;
}

/*
 * Jumps back to the loop start: restores every track from the saved track state, sets the tick to
 * the loop start and writes an all-notes-off controller change for each of the 16 channels.
 */
// FUNCTION: FF7 0x0074399b
void __cdecl MidiEmitLoopRestart(MidiStreamBuffer *stream)
{
    struct {
        int size;
        int event[3];
        int reserved;
        int emitted;
        int index;
        MidiTrack *track;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.emitted = 0;
    frame.track = &midiTrackState[g_MidiCurrentTrack];
    frame.event[0] = g_MidiLoopEnd - g_MidiLastTick;
    frame.event[1] = 0;
    g_MidiLastTick = g_MidiLoopStart;
    for (frame.index = 0; frame.index < *(unsigned short *)&midiTrackCount; frame.index++) {
        midiTrackState[frame.index] = ((MidiTrack *)&midiSavedTrackState)[frame.index];
    }
    frame.size = 12;
    frame.event[2] = MIDI_STATUS_CONTROL_CHANGE;
    frame.event[2] |= MIDI_CONTROLLER_ALL_NOTES_OFF << 8;
    frame.event[2] = frame.event[2];
    memcpy(&stream->data[stream->filledBytes], frame.event, frame.size);
    stream->filledBytes += frame.size;
    frame.event[0] = 0;
    for (frame.index = 1; frame.index < 16; frame.index++) {
        frame.event[2] = MIDI_STATUS_CONTROL_CHANGE + frame.index;
        frame.event[2] |= MIDI_CONTROLLER_ALL_NOTES_OFF << 8;
        frame.event[2] = frame.event[2];
        memcpy(&stream->data[stream->filledBytes], frame.event, frame.size);
        stream->filledBytes += frame.size;
    }
}

/*
 * The stream's callback. On MOM_CLOSE it clears the stream; on MOM_DONE it
 * unprepares the finished buffer and, while the song runs, refills and requeues it (a failed
 * requeue marks MIDI dead, "KABOOM!"). After the song ends it only counts the buffers back and
 * clears the stop flag at six.
 */
// FUNCTION: FF7 0x00743aee
void __stdcall MidiStreamCallback(int handle, int message, int instance, int parameter1, int parameter2)
{
    MidiStreamBuffer *header;
    switch (message) {
    case MOM_OPEN:
        break;
    case MOM_CLOSE:
        g_MidiPlaybackState = MIDI_PLAYBACK_STOPPED;
        g_MidiStreamHandle = 0;
        g_MidiClosePending = 0;
        break;
    case MOM_DONE:
        header = &midiStreamBuffers[g_MidiBufferIndex];
        midiOutUnprepareHeader(g_MidiStreamHandle, (void *)header, 64);
        g_MidiFreeBufferCount++;
        if (g_MidiStreamEnded) {
            g_MidiBufferIndex++;
            g_MidiBufferIndex %= 6;
            if (g_MidiFreeBufferCount == 6) {
                g_MidiStopPending = 0;
                g_MidiPlaybackState = MIDI_PLAYBACK_STOPPED;
            }
            break;
        }
        EnterCriticalSection(MIDI_LOCK);
        if (MidiFillStreamBuffer(header)) {
            if (MidiSubmitStreamBuffer(header)) {
                g_MidiFreeBufferCount--;
            } else {
                g_MidiFailed = 1;
                g_MidiStreamEnded = 1;
                // STRING: FF7 0x00967474
                Gfx_DebugPrint("KABOOM!: MIDI is HOSED - unable to recover.\n");
                break;
            }
        }
        g_MidiBufferIndex++;
        g_MidiBufferIndex %= 6;
        LeaveCriticalSection(MIDI_LOCK);
        break;
    }
}

/*
 * Advances a running volume ramp by the time since it started and writes the new driver volume. At
 * the target the ramp ends: a cross-fade to silence stops and closes the stream, a pending song
 * change stops the old song and plays the queued one.
 */
// FUNCTION: FF7 0x00743c57
void __cdecl MidiUpdateVolumeFade(void)
{
    int change;
    int scaledVolume;
    int now;
    int curveVal;

    now = timeGetTime();
    if (g_MidiInitialized == 0 || g_MidiFailed != 0) { /* empty first arm: lib/codegen.h */
    } else {
        if (g_MidiVolumeFadeActive != 0) {
            change = (now - g_MidiVolumeFadeLastTime) * g_MidiVolumeFadeRate / 1000;
            if (change == 0) {
                change = g_MidiVolumeFadeRate > 0 ? 1 : -1;
            }
            g_MidiVolume += change;
            if ((change < 0 && g_MidiVolume < g_MidiTargetVolume) || (change > 0 && g_MidiVolume > g_MidiTargetVolume)) {
                g_MidiVolume = g_MidiTargetVolume;
            }
            scaledVolume = (unsigned int)(g_MidiVolume * g_MidiMasterVolume) / (unsigned int)0x64;
            if (g_MidiUseVolumeCurve != 0) {
                curveVal = g_MidiVolumeCurve[scaledVolume];
            } else {
                curveVal = (unsigned int)(scaledVolume * 65535) / (unsigned int)0x7F;
            }
            scaledVolume = curveVal;
            scaledVolume |= scaledVolume << 16;
            MidiApplyDriverVolume(scaledVolume);
            if (g_MidiVolume == g_MidiTargetVolume) {
                g_MidiVolumeFadeActive = 0;
                if (g_MidiCrossFadeActive != 0) {
                    MidiStreamStopChecked();
                    MidiStreamCloseDevice();
                    g_MidiCrossFadeActive = 0;
                    g_MidiTargetVolume = 0x7F;
                    g_MidiVolume = g_MidiTargetVolume;
                    return;
                }
                if (g_MidiCrossFadePending != 0) {
                    MidiStreamStopChecked();
                    MidiStreamCloseDevice();
                    if (g_MidiPendingSong != 0) {
                        MidiPlaySong(g_MidiPendingSong, 0, 0);
                        g_MidiPendingSong = 0;
                    }
                }
                if (g_MidiTargetVolume == 0) {
                    MidiStop();
                }
            }
        }
        g_MidiVolumeFadeLastTime = now;
    }
}

/*
 * Writes a packed left/right volume to the stream device (midiOutSetVolume) under the MIDI lock.
 * Returns 0 without driver volume control.
 */
// FUNCTION: FF7 0x00743e08
int __cdecl MidiApplyDriverVolume(int value)
{
    int result = 0;
    if (g_MidiVolumeControl == 0)
        return 0;
    EnterCriticalSection(MIDI_LOCK);
    if (g_MidiStreamHandle != 0) {
        // STRING: FF7 0x009674a4
        result = MmCheckError(midiOutSetVolume(g_MidiStreamHandle, value), "C:\\lib\\src\\sound\\midi1.cpp", 1600);
    }
    LeaveCriticalSection(MIDI_LOCK);
    return result;
}

/*
 * Sets the open stream's tempo property under the MIDI lock; returns whether that succeeded.
 */
// FUNCTION: FF7 0x00743e6d
int __cdecl MidiStreamSetTempo(int tempo)
{
    struct {
        int result;
        int size;
        int tempo;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.result = 0;
    frame.size = 8;
    frame.tempo = tempo;
    EnterCriticalSection(MIDI_LOCK);
    if (g_MidiStreamHandle != 0) {
        // STRING: FF7 0x009674c0
        frame.result = MmCheckError(midiStreamProperty(g_MidiStreamHandle, &frame.size, 0x80000002), "C:\\lib\\src\\sound\\midi1.cpp", 1616);
    }
    LeaveCriticalSection(MIDI_LOCK);
    return frame.result;
}

/*
 * Opens the MIDI stream on g_MidiDeviceId with MidiStreamCallback under the MIDI lock; returns 1 on
 * success.
 */
// FUNCTION: FF7 0x00743ed9
int __cdecl MidiStreamOpenDevice(void)
{
    int result = 0;
    EnterCriticalSection(MIDI_LOCK);
    // STRING: FF7 0x009674dc
    if (MmCheckError(midiStreamOpen(&g_MidiStreamHandle, &g_MidiDeviceId, 1, MidiStreamCallback, 0, 0x30000), "C:\\lib\\src\\sound\\midi1.cpp", 1628))
        result = 1;
    LeaveCriticalSection(MIDI_LOCK);
    return result;
}

/*
 * Stops the stream when it plays with buffers queued and waits until the callback has every buffer
 * back. Returns the midiStreamStop result, 0 when nothing played.
 */
// FUNCTION: FF7 0x00743f3d
int __cdecl MidiStreamStopChecked(void)
{
    struct {
        int result;
        int state;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.result = 0;
    MidiWaitForStop();
    MidiWaitForClose();
    frame.state = g_MidiPlaybackState;
    if (frame.state == MIDI_PLAYBACK_STOPPED)
        return 0;
    EnterCriticalSection(MIDI_LOCK);
    if (g_MidiStreamHandle != 0 && g_MidiFreeBufferCount < 6) {
        g_MidiStreamEnded = 1;
        g_MidiStopPending = 1;
        // STRING: FF7 0x009674f8
        frame.result = MmCheckError(midiStreamStop(g_MidiStreamHandle), "C:\\lib\\src\\sound\\midi1.cpp", 1655);
        if (frame.result == 0)
            g_MidiStopPending = 0;
    }
    LeaveCriticalSection(MIDI_LOCK);
    MidiWaitForStop();
    return frame.result;
}

/*
 * Closes the stream device and waits for the callback to see the close.
 */
// FUNCTION: FF7 0x00743fe1
int __cdecl MidiStreamCloseDevice(void)
{
    int result;
    MidiWaitForStop();
    MidiWaitForClose();
    EnterCriticalSection(MIDI_LOCK);
    if (g_MidiStreamHandle != 0) {
        g_MidiStreamEnded = 1;
        g_MidiClosePending = 1;
        // STRING: FF7 0x00967514
        result = MmCheckError(midiStreamClose(g_MidiStreamHandle), "C:\\lib\\src\\sound\\midi1.cpp", 1676);
        if (result == 0)
            g_MidiClosePending = 0;
    }
    LeaveCriticalSection(MIDI_LOCK);
    MidiWaitForClose();
    return result;
}

/*
 * Spins until the stream callback has seen the close (g_MidiClosePending clear).
 */
// FUNCTION: FF7 0x00744060
void __cdecl MidiWaitForClose(void)
{
    int val;
    while ((val = g_MidiClosePending) != 0) {
    }
}

/*
 * Spins until the stream callback has every buffer back after a stop (g_MidiStopPending clear).
 */
// FUNCTION: FF7 0x00744078
void __cdecl MidiWaitForStop(void)
{
    int val;
    while ((val = g_MidiStopPending) != 0) {
    }
}

/*
 * Returns the MIDI-available flag g_MidiVolumeControl.
 */
// FUNCTION: FF7 0x00744090
int __cdecl MidiGetVolumeControlFlag(void)
{
    return g_MidiVolumeControl;
}

/*
 * Opens the MIDI output device, sends the six-byte GM reset sysex (g_MidiGmResetSysex) and closes
 * the device.
 */
// FUNCTION: FF7 0x0074409a
void __cdecl MidiSendGmReset(void)
{
    struct {
        int handle;
        int header[16];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    // STRING: FF7 0x00967530
    if (MmCheckError(midiOutOpen(&frame.handle, g_MidiDeviceId, 0, 0, 0), "C:\\lib\\src\\sound\\midi1.cpp", 1709)) {
        frame.header[0] = ((int)&g_MidiGmResetSysex);
        frame.header[1] = 6;
        frame.header[2] = 0;
        frame.header[3] = 0;
        frame.header[4] = 0;
        frame.header[5] = 0;
        frame.header[6] = 0;
        // STRING: FF7 0x0096754c
        MmCheckError(midiOutPrepareHeader(frame.handle, frame.header, 64), "C:\\lib\\src\\sound\\midi1.cpp", 1719);
        // STRING: FF7 0x00967568
        MmCheckError(midiOutLongMsg(frame.handle, frame.header, 64), "C:\\lib\\src\\sound\\midi1.cpp", 1720);
        // STRING: FF7 0x00967584
        MmCheckError(midiOutUnprepareHeader(frame.handle, frame.header, 64), "C:\\lib\\src\\sound\\midi1.cpp", 1721);
        // STRING: FF7 0x009675a0
        MmCheckError(midiOutClose(frame.handle), "C:\\lib\\src\\sound\\midi1.cpp", 1722);
        frame.handle = 0;
    }
}

/*
 * Queues one stream buffer that sets controller 11 (expression) to 127 on all 16 channels.
 */
// FUNCTION: FF7 0x00744193
int __cdecl MidiPrimeExpressionReset(void *handle, void *header)
{
    struct {
        int length;
        int event[3];
        int reserved;
        int channel;
    } frame;
    frame.length = 12;
    frame.event[1] = 0;
    frame.event[0] = 0;
    for (frame.channel = 0; frame.channel < 16; frame.channel++) {
        frame.event[2] = frame.channel + 176;
        frame.event[2] |= 0xb00;
        frame.event[2] |= 0x7f0000;
        frame.event[2] = frame.event[2];
        memcpy((void *)((int)g_MidiExpressionResetEvents + frame.channel * frame.length), frame.event, frame.length);
    }
    return MidiSendEventBlock((int *)handle, (int *)header, (int)g_MidiExpressionResetEvents, 192);
}

/*
 * Queues one stream buffer that centres the pitch bend on all 16 channels.
 */
// FUNCTION: FF7 0x00744231
int __cdecl MidiPrimePitchBendReset(void *handle, void *header)
{
    struct {
        int length;
        int event[3];
        int reserved;
        int channel;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.length = 12;
    frame.event[1] = 0;
    frame.event[0] = 0;
    for (frame.channel = 0; frame.channel < 16; frame.channel++) {
        frame.event[2] = frame.channel + 224;
        frame.event[2] |= 0x400000;
        frame.event[2] = frame.event[2];
        memcpy((void *)((int)g_MidiPitchBendResetEvents + frame.channel * frame.length), frame.event, frame.length);
    }
    return MidiSendEventBlock((int *)handle, (int *)header, (int)g_MidiPitchBendResetEvents, 192);
}

/*
 * Prepares a stream header for a block of events and queues it with midiStreamOut; on failure it
 * unprepares the header and returns 0.
 */
// FUNCTION: FF7 0x007442c7
int __cdecl MidiSendEventBlock(int *handle, int *header, int data, int size)
{
    header[0] = data;
    header[1] = size;
    header[2] = size;
    header[4] = 0;
    header[3] = 0;
    // STRING: FF7 0x009675bc
    if (!MmCheckError(midiOutPrepareHeader(*handle, header, 64), "C:\\lib\\src\\sound\\midi1.cpp", 1776))
        return 0;
    // STRING: FF7 0x009675d8
    if (!MmCheckError(midiStreamOut(*handle, header, 64), "C:\\lib\\src\\sound\\midi1.cpp", 1782)) {
        // STRING: FF7 0x009675f4
        OutputDebugStringA("unpreparing header...\n");
        // STRING: FF7 0x0096760c
        MmCheckError(midiOutUnprepareHeader(*handle, header, 64), "C:\\lib\\src\\sound\\midi1.cpp", 1785);
        return 0;
    }
    return 1;
}

/*
 * Unprepares `count` stream buffers and marks all six free. As in the original, every pass names
 * the buffer at index `count` rather than the loop index and passes the address of the stream
 * handle, not the handle.
 */
// FUNCTION: FF7 0x00744389
void __cdecl MidiUnprepareBuffers(int count)
{
    int i;

    for (i = 0; i < count; i++) {
        // STRING: FF7 0x00967628
        MmCheckError(midiOutUnprepareHeader((int)&g_MidiStreamHandle, (void *)&midiStreamBuffers[count], 0x40), "C:\\lib\\src\\sound\\midi1.cpp", 1797);
    }
    g_MidiFreeBufferCount = 6;
}

// Verified views within the existing intrinsic strcat literal payloads.
// STRING: FF7 0x00966df0
typedef char MidiBankStrSlice03[sizeof("\\ff7.sf2")];
// STRING: FF7 0x00966df4
typedef char MidiBankStrSlice04[sizeof(".sf2")];
// STRING: FF7 0x00966e00
typedef char MidiBankStrSlice05[sizeof("\\ff7m.sf2")];
// STRING: FF7 0x00966e04
typedef char MidiBankStrSlice06[sizeof("m.sf2")];
// STRING: FF7 0x00966e08
typedef char MidiBankStrSlice07[sizeof("2")];
// STRING: FF7 0x00966e10
typedef char MidiBankStrSlice08[sizeof("\\ff7s.sf2")];
// STRING: FF7 0x00966e14
typedef char MidiBankStrSlice09[sizeof("s.sf2")];
// STRING: FF7 0x00966e18
typedef char MidiBankStrSlice10[sizeof("2")];
// STRING: FF7 0x00966e98
typedef char MidiBankStrSlice11[sizeof(".lgp")];
// STRING: FF7 0x00966eb8
typedef char MidiBankStrSlice12[sizeof(".lgp")];
// STRING: FF7 0x00966ec4
typedef char MidiBankStrSlice13[sizeof("i.lgp")];
// STRING: FF7 0x00966ec8
typedef char MidiBankStrSlice14[sizeof("p")];
// STRING: FF7 0x00966ed0
typedef char MidiBankStrSlice15[sizeof(".lgp")];
// STRING: FF7 0x00966edc
typedef char MidiBankStrSlice16[sizeof("lgp")];

/*
 * Returns the address of g_MidiDataPath, the stored MIDI LGP path buffer, or 0 when g_MidiInitialized is
 * zero. g_MidiInitialized is the flag the MIDI code tests before doing any MIDI work, so a null return
 * means the caller must not use the path.
 */
// FUNCTION: FF7 0x007443e7
int __cdecl GetMidiLgpPath(void)
{
    if (g_MidiInitialized == 0) {
        return 0;
    } else {
        return (int)&g_MidiDataPath;
    }
}
} /* extern "C" */
