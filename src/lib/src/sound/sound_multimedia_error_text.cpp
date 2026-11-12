// src/lib/src/sound/sound_multimedia_error_text.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Maps multimedia, MIDI and audio compression error codes to static diagnostic text, with
//   a helper that tests for success.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/sound.h"
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <msacm.h>
#include <string.h>
#include <stdio.h>
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(memcpy)

#pragma intrinsic(strcpy, strcat, memset, strcmp)

/*
 * Returns static diagnostic text for recognized multimedia, MIDI and audio compression error
 * codes, including success, common errors 1 through 20 and special codes 0x41, 0x44 and 0x200
 * through 0x202. Returns the unrecognized-error string for every other code and changes no state.
 */
// FUNCTION: FF7 0x00749ee0
char *__cdecl MmGetErrorText(int err)
{
    switch (err) {
    case MMSYSERR_NOERROR:
        // STRING: FF7 0x0096805c
        return "No error.";
    case MMSYSERR_INVALFLAG:
        // STRING: FF7 0x00968068
        return "At least one flag is invalid.";
    case MMSYSERR_INVALHANDLE:
        // STRING: FF7 0x00968088
        return "The specified handle is invalid.";
    case MMSYSERR_INVALPARAM:
        // STRING: FF7 0x009680ac
        return "At least one parameter is invalid.";
    case MMSYSERR_NOMEM:
        // STRING: FF7 0x009680d0
        return "The system is unable to allocate resources.";
    case MMSYSERR_BADDEVICEID:
        // STRING: FF7 0x009680fc
        return "The specified device identifier is out of range.";
    case MMSYSERR_NOTSUPPORTED:
        // STRING: FF7 0x00968130
        return "The function is not supported.";
    case ACMERR_BUSY:
        // STRING: FF7 0x00968150
        return "The conversion stream cannot be closed because an asynchronous conversion is still in progress.";
    case ACMERR_NOTPOSSIBLE:
        // STRING: FF7 0x009681b0
        return "The requested operation cannot be performed.";
    case ACMERR_UNPREPARED:
        // STRING: FF7 0x009681e0
        return "The stream header specified in pash is currently not prepared by the acmStreamPrepareHeader function.";
    case MIDIERR_STILLPLAYING:
        // STRING: FF7 0x00968248
        return "The buffer pointed to by lpMidiOutHdr is still in the queue.";
    case MIDIERR_NODEVICE:
        // STRING: FF7 0x00968288
        return "No MIDI port was found. This error occurs only when the mapper is opened.";
    case MMSYSERR_ALLOCATED:
        // STRING: FF7 0x009682d4
        return "The specified resource is already allocated.";
    case MMSYSERR_ERROR:
        // STRING: FF7 0x00968304
        return "unspecified error";
    case MMSYSERR_NOTENABLED:
        // STRING: FF7 0x00968318
        return "driver failed enable";
    case MMSYSERR_NODRIVER:
        // STRING: FF7 0x00968330
        return "no device driver present";
    case MMSYSERR_BADERRNUM:
        // STRING: FF7 0x0096834c
        return "error value out of range";
    case MMSYSERR_HANDLEBUSY:
        // STRING: FF7 0x00968368
        return "handle being used simultaneously on another thread (eg callback)";
    case MMSYSERR_INVALIDALIAS:
        // STRING: FF7 0x009683ac
        return "specified alias not found";
    case MMSYSERR_BADDB:
        // STRING: FF7 0x009683c8
        return "bad registry database";
    case MMSYSERR_KEYNOTFOUND:
        // STRING: FF7 0x009683e0
        return "registry key not found";
    case MMSYSERR_READERROR:
        // STRING: FF7 0x009683f8
        return "registry read error";
    case MMSYSERR_WRITEERROR:
        // STRING: FF7 0x0096840c
        return "registry write error";
    case MMSYSERR_DELETEERROR:
        // STRING: FF7 0x00968424
        return "registry delete error";
    case MMSYSERR_VALNOTFOUND:
        // STRING: FF7 0x0096843c
        return "registry value not found";
    case MMSYSERR_NODRIVERCB:
        // STRING: FF7 0x00968458
        return "driver does not call DriverCallback";
    }
    // STRING: FF7 0x0096847c
    return "Unrecognized error value.";
}

/*
 * Returns 1 for multimedia error code zero. For a nonzero code, formats its translated error text
 * with the supplied filename and line into a 512-byte local buffer, dispatches the message to the
 * active target and returns 0. sprintf does not bound the message length.
 */
// FUNCTION: FF7 0x0074a071
int __cdecl MmCheckError(int error, const char *file, int line)
{
    struct {
        char message[512];
        int result;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.result = 1;
    if (error != 0) {
        // STRING: FF7 0x00968498
        sprintf(frame.message, "MULTIMEDIA ERROR: %s in FILE %s LINE %d \n", MmGetErrorText(error), file, line);
        Gfx_DebugPrint(frame.message);
        frame.result = 0;
    }
    return frame.result;
}
} /* extern "C" */
