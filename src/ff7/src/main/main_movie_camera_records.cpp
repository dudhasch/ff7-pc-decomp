// Contents: Reads the movie camera record stream: it tracks the cursor position in the stream and
//   fetches the camera record for the current movie frame.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/movie.h"
#include "lib/api/movie.h"
#include "ff7/api/field.h"
#include "ff7/field.h"
extern "C" {

/*
 * Returns the movie camera record cursor (offset in words).
 */
// FUNCTION: FF7 0x0040ac90
int __cdecl Movie_GetCameraRecordCursor(void)
{
    return g_MovieCameraRecordCursor;
}

/*
 * Retrieves the 40-byte (10 int) camera record for the current movie frame.
 * If movie playback is disabled, inactive, or exhausted, returns the cleared buffer.
 */
// FUNCTION: FF7 0x0040ac9a
int *__cdecl Movie_FetchCameraRecord(void)
{
    unsigned int frameIndex;
    int *camRecord;
    int stopped;
    unsigned int index;

    for (index = 0; index < 10; index++) {
        ((int *)&g_MovieCameraRecord)[index] = 0;
    }
    if (g_MovieCameraSuppressed != 0) {
        return (int *)&g_MovieCameraRecord;
    }
    if ((*(char *)&g_FieldActorState.state.movieCameraDisabled) == 1) {
        return (int *)&g_MovieCameraRecord;
    }
    stopped = Movie_IsNotPlaying();
    if (stopped != 0) {
        return (int *)&g_MovieCameraRecord;
    }
    if ((*(char *)&g_FieldActorState.state.movieCameraDisabled) != 1) {
        if (g_MovieCameraRecordCursor > g_MovieCameraRecordTotalWords) {
            return (int *)&g_MovieCameraRecord;
        }
        if (g_MovieCameraDataBuffer == 0) {
            return (int *)&g_MovieCameraRecord;
        }
        frameIndex = Movie_GetCurrentFrame();
        camRecord = (int *)g_MovieCameraDataBuffer + frameIndex * 40 / 4;
        if (stopped == 0 && camRecord != 0) {
            for (index = 0; index < 10; index++) {
                ((int *)&g_MovieCameraRecord)[index] = *camRecord++;
            }
            g_MovieCameraRecordCursor += index;
        }
    }
    return (int *)&g_MovieCameraRecord;
}
} /* extern "C" */
