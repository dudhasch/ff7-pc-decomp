// src/lib/src/sound/sound_directsound_error_text.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Maps DirectSound HRESULT values to diagnostic strings, with a helper that tests for
//   success.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <windows.h>
#include <string.h>
#include "lib/crt.h"
#include "lib/types/sound.h"
#include "lib/api/sound.h"
extern "C" {

#pragma intrinsic(memset)

/*
 * Maps a DirectSound HRESULT to its diagnostic string; unrecognized values give "Unrecognized
 * error value."
 */
// FUNCTION: FF7 0x0074ab90
char *__cdecl DsErrorDescription(int error)
{
    switch (error) {
    case (int)0x8878000A:
        // STRING: FF7 0x00968cf4
        return "The request failed because resources, such as a priority level, were already in use by another caller.";
    case (int)0x88780082:
        // STRING: FF7 0x00968d5c
        return "The object is already initialized.";
    case (int)0x88780064:
        // STRING: FF7 0x00968d80
        return "The specified wave format is not supported.";
    case (int)0x88780096:
        // STRING: FF7 0x00968dac
        return "The buffer memory has been lost and must be restored.";
    case (int)0x8878001E:
        // STRING: FF7 0x00968de4
        return "The control (volume, pan, and so forth) requested by the caller is not available.";
    case (int)0x80004005:
        // STRING: FF7 0x00968e38
        return "An undetermined error occurred inside the DirectSound subsystem.";
    case (int)0x88780032:
        // STRING: FF7 0x00968e7c
        return "This function is not valid for the current state of this object.";
    case (int)0x80070057:
        // STRING: FF7 0x00968ec0
        return "An invalid parameter was passed to the returning function.";
    case (int)0x80040110:
        // STRING: FF7 0x00968efc
        return "The object does not support aggregation.";
    case (int)0x88780078:
        // STRING: FF7 0x00968f28
        return "No sound driver is available for use.";
    case (int)0x80004002:
        // STRING: FF7 0x00968f50
        return "The requested COM interface is not available.";
    case (int)0x887800A0:
        // STRING: FF7 0x00968f80
        return "Another application has a higher priority level, preventing this call from succeeding";
    case (int)0x8007000E:
        // STRING: FF7 0x00968fd8
        return "The DirectSound subsystem could not allocate sufficient memory to complete the caller's request.";
    case (int)0x88780046:
        // STRING: FF7 0x0096903c
        return "The caller does not have the priority level required for the function to succeed.";
    case (int)0x887800AA:
        // STRING: FF7 0x00969090
        return "The IDirectSound::Initialize method has not been called or has not been called successfully before other methods were called.";
    case (int)0x80004001:
        // STRING: FF7 0x00969110
        return "The function called is not supported at this time.";
    default:
        // STRING: FF7 0x00969144
        return "Unrecognized error value.";
    }
}
/*
 * Returns 1 for a zero DirectSound result. For any nonzero result, logs DIRECT SOUND ERROR and a
 * formatted description, source file and line through OutputDebugStringA, then returns 0.
 */
// FUNCTION: FF7 0x0074ad6d
int __cdecl DsCheckError(int error, char *file, int line)
{
    struct {
        char message[512];
        int result;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.result = 1;
    if (error != 0) {
        // STRING: FF7 0x00969160
        OutputDebugStringA("DIRECT SOUND ERROR\n");
        // STRING: FF7 0x00969174
        sprintf(frame.message, "   ERROR: %s in FILE %s LINE %d \n", DsErrorDescription(error), file, line);
        OutputDebugStringA(frame.message);
        frame.result = 0;
    }
    return frame.result;
}
} /* extern "C" */
