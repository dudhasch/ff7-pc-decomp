// src/lib/src/sound/acm.cpp
// Original: C:\lib\src\sound\acm.cpp
// Contents: Wrapper around the Windows Audio Compression Manager: sets up PCM output formats,
//   queries decoded and source sizes, and opens, converts, prepares and closes conversion streams.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <windows.h>
#include <mmsystem.h>
#include "lib/api/sound.h"
#include "lib/types/sound.h"
#include "lib/enums/sound.h"
#include <string.h>
extern "C" {

struct SoundAcmStreamHeader;
void __cdecl AcmMakePcm16Format(unsigned int bytes, void *source, void *destination);

/* Five-argument stdcall callback that does nothing. */
// FUNCTION: FF7 0x0074a900
void __stdcall AcmUnusedNoOpCallback(int context, int param, int thirdParam, int fourthParam, int fifthParam)
{
    int value;

    value = param;
}

/*
 * Copies source into destination, then sets format tag WAVE_FORMAT_PCM and 16 bits per sample, recomputes
 * block alignment and average bytes per second from channels/sample rate, and clears cbSize.
 */
// FUNCTION: FF7 0x0074a910
void __cdecl AcmMakePcm16Format(unsigned int bytes, void *source, void *destination)
{
    *(WAVEFORMATEX *)destination = *(WAVEFORMATEX *)source;
    ((WAVEFORMATEX *)destination)->wFormatTag = SOUND_WAVE_FORMAT_PCM;
    ((WAVEFORMATEX *)destination)->wBitsPerSample = 16;
    ((WAVEFORMATEX *)destination)->nBlockAlign = (((WAVEFORMATEX *)destination)->nChannels * ((WAVEFORMATEX *)destination)->wBitsPerSample) / 8;
    ((WAVEFORMATEX *)destination)->nAvgBytesPerSec = ((WAVEFORMATEX *)destination)->nSamplesPerSec * ((WAVEFORMATEX *)destination)->nBlockAlign;
    ((WAVEFORMATEX *)destination)->cbSize = 0;
}
#pragma intrinsic(memset)

/* Win32 MSACM stream imports declared with int handle parameters matching this TU. */
extern int __stdcall acmStreamUnprepareHeader(int stream, int header, unsigned int flags);
extern int __stdcall acmStreamSize(int stream, unsigned int inputSize, unsigned int *outputSize, unsigned int flags);
extern int __stdcall acmStreamOpen(int output, int driver, int sourceFormat, int destinationFormat, int filter, int callback,
                                   int context, unsigned int flags);
extern int __stdcall acmStreamConvert(int stream, void *header, unsigned int flags);
extern int __stdcall acmStreamPrepareHeader(int stream, void *header, unsigned int flags);
extern int __stdcall acmStreamClose(int stream, unsigned int flags);

/*
 * Queries output buffer size needed for decoding given input size via acmStreamSize.
 */
// FUNCTION: FF7 0x0074a992
unsigned int __cdecl AcmGetDecodedSize(int stream, unsigned int inputSize)
{
    unsigned int outputSize;

    if (MmCheckError(acmStreamSize(stream, inputSize, &outputSize, SOUND_ACM_STREAMSIZEF_SOURCE),
                     // STRING: FF7 0x00968bf8
                     "C:\\lib\\src\\sound\\acm.cpp", 78)) {
        return outputSize;
    }
    return 0;
}

/*
 * Queries source buffer size needed to produce decoded size via acmStreamSize.
 */
// FUNCTION: FF7 0x0074a9c8
unsigned int __cdecl AcmGetSourceSizeForDecoded(int stream, unsigned int inputSize)
{
    unsigned int outputSize;

    if (MmCheckError(acmStreamSize(stream, inputSize, &outputSize, SOUND_ACM_STREAMSIZEF_DESTINATION),
                     // STRING: FF7 0x00968c14
                     "C:\\lib\\src\\sound\\acm.cpp", 91)) {
        return outputSize;
    }
    return 0;
}

/*
 * Opens an Audio Compression Manager conversion stream, asynchronous in SOUND_ACM_OPEN_ASYNC mode.
 */
// FUNCTION: FF7 0x0074a9fe
int __cdecl AcmStreamOpen(int output, int sourceFormat, int destinationFormat, int mode)
{
    switch (mode) {
    case SOUND_ACM_OPEN_SYNC:
        return MmCheckError(acmStreamOpen(output, 0, sourceFormat, destinationFormat, 0, 0, 0, 0),
                            // STRING: FF7 0x00968c30
                            "C:\\lib\\src\\sound\\acm.cpp", 106);
    case SOUND_ACM_OPEN_ASYNC:
        return MmCheckError(acmStreamOpen(output, 0, sourceFormat, destinationFormat, 0, 0, 0, SOUND_ACM_STREAMOPENF_ASYNC),
                            // STRING: FF7 0x00968c4c
                            "C:\\lib\\src\\sound\\acm.cpp", 109);
    }
    return 0;
}

/*
 * Performs conversion on an ACM stream, block-aligned in SOUND_ACM_CONVERT_BLOCK_ALIGN mode.
 */
// FUNCTION: FF7 0x0074aa76
void __cdecl AcmStreamConvert(int stream, void *header, int mode)
{
    switch (mode) {
    case SOUND_ACM_CONVERT_WHOLE:
        MmCheckError(acmStreamConvert(stream, header, 0),
                     // STRING: FF7 0x00968c68
                     "C:\\lib\\src\\sound\\acm.cpp", 147);
        break;
    case SOUND_ACM_CONVERT_BLOCK_ALIGN:
        MmCheckError(acmStreamConvert(stream, header, SOUND_ACM_STREAMCONVERTF_BLOCKALIGN),
                     // STRING: FF7 0x00968c84
                     "C:\\lib\\src\\sound\\acm.cpp", 150);
        break;
    }
}

/*
 * Initializes ACMSTREAMHEADER structure and prepares it for stream conversion.
 */
// FUNCTION: FF7 0x0074aad8
int __cdecl AcmStreamPrepare(int stream, SoundAcmStreamHeader *header, int source, int sourceBytes, int destination, int destinationBytes)
{
    memset(header, 0, 0x54);
    header->structSize = 84;
    header->source = source;
    header->sourceBytes = sourceBytes;
    header->destination = destination;
    header->destinationBytes = destinationBytes;
    return MmCheckError(acmStreamPrepareHeader(stream, header, 0),
                        // STRING: FF7 0x00968ca0
                        "C:\\lib\\src\\sound\\acm.cpp", 165);
}

/*
 * Unprepares an ACM stream header.
 */
// FUNCTION: FF7 0x0074ab3a
int __cdecl AcmStreamUnprepare(int stream, int header)
{
    return MmCheckError(acmStreamUnprepareHeader(stream, header, 0),
                        // STRING: FF7 0x00968cbc
                        "C:\\lib\\src\\sound\\acm.cpp", 170);
}

/*
 * Closes an open Audio Compression Manager conversion stream.
 */
// FUNCTION: FF7 0x0074ab61
int __cdecl AcmStreamClose(int stream)
{
    return MmCheckError(acmStreamClose(stream, 0),
                        // STRING: FF7 0x00968cd8
                        "C:\\lib\\src\\sound\\acm.cpp", 175);
}
} /* extern "C" */
