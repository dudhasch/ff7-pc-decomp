# Movie (ff7): field movie requests and movie camera data

This is the game side of full-motion video in the field. A field script posts a movie request; the
field logic calls `Field_ServiceMovieRequest` every update. That turns the request into calls to
the DirectShow player in [lib/movie](../../../lib/src/movie/README.md): it picks the `.avi` by
movie id and disc, loads the matching `.cam` camera file from `cd/moviecam.lgp`, starts and stops
playback. Field reads the camera record for the current movie frame through
`Movie_FetchCameraRecord`. There is no module of its own: the field module stays active while a
movie plays.

## Where the code is

`src/ff7/src/movie/sm_movie.cpp`: 1 TU, 779 lines, 7 functions, 0x0040adb0-0x0040b400. It keeps
its retail name from the `__FILE__` string `C:\FF7\Src\movie\sm_movie.cpp`. TU boundaries follow
retail's objects.

Code of this subsystem in another folder: `src/ff7/src/main/main_movie_camera_records.cpp`
(0x0040ac90-0x0040adb0) holds `Movie_FetchCameraRecord` and `Movie_GetCameraRecordCursor`. They read
this TU's camera globals (and the field movie phase word). See [main](../main/README.md).

## Entry points

| function | address | what it does | called from |
|---|---|---|---|
| `Field_ServiceMovieRequest` | 0x0040b27b | acts on the field request byte: load (3), play (4), stop (0x14), and lib/movie's stop request (end of a non-looping movie) | `Field_UpdateLogic` (0x0063c17f) |
| `Movie_LoadCameraData` | 0x0040adb0 | loads the `.cam` file and opens the `.avi` | `Field_ServiceMovieRequest` |
| `Field_StartMovieIfIdle` | 0x0040b19d | `Movie_Run` when no movie is playing | `Field_ServiceMovieRequest` |
| `Field_EndMoviePlayback` | 0x0040b1fa | frees the camera buffer, releases the graph, sets the previous mode to 0x0D, `Field_ResetFrameCounter` | service, failed open |
| `Movie_FreeCameraDataBuffer` | 0x0040b1c0 | frees the camera buffer | `Field_EndMoviePlayback`, `App_Shutdown` |
| `Movie_FetchCameraRecord` | 0x0040ac9a | the 40-byte camera record of the current movie frame, or zeros | `Field_UpdateMovieCamera` (0x0063a4c8) |

`Field_UnusedNoOp_1` (empty) and `Movie_GetDuration` are not called by name
anywhere.

## Main structures

| type or global | header | what it holds |
|---|---|---|
| `g_MovieNames_Common`, `g_MovieNames_Disc1`, `g_MovieNames_Disc2`, `g_MovieNames_Disc3` | `sm_movie.cpp` | `.avi` names: ids 0..0x13 are common, ids from 0x14 index the table of the current disc |
| `g_CameraNames_*` | `sm_movie.cpp` | the matching `.cam` names |
| `g_MovieDurationsCommon`, `...Disc1`, `...Disc2`, `...Disc3` | `sm_movie.cpp` | per-movie values, one `int` table per disc |
| `g_MovieCameraDataBuffer`, `g_MovieCameraAllocatedBuffer` | `include/ff7/api/movie.h` (`g_MovieCameraAllocatedBuffer` only in `sm_movie.cpp`) | heap copy of the `.cam` file |
| `g_MovieCameraRecordCursor`, `g_MovieCameraRecordTotalWords` | `include/ff7/api/movie.h` | words consumed and words available |
| `g_MovieCameraRecord` | `include/ff7/api/movie.h` | 10 dwords handed to field each frame |
| `g_MovieCameraSuppressed`, `g_MovieFlag` | `include/ff7/api/movie.h` | camera skipped; set for the same movies as the loop flag passed to `Movie_Open`, and read by field |
| `MovieCameraLoadFrame`, `FieldEndMovieFrame` | `sm_movie.cpp` | stack-frame layouts of two functions, not data types |

## Data flow

1. Field script posts request 3 (`FIELD_REQUEST_LOAD_MOVIE`) with the movie id in
   `g_FieldActorState.state.eventCmdParam`. While the movie phase word (`g_FieldActorState.state.requestArg`) is 0,
   `Field_ServiceMovieRequest` calls `Movie_LoadCameraData` unless a graph is already open.
2. `Movie_LoadCameraData` takes the disc from the savemap byte +0xEA4. It finds `<name>.cam` in LGP
   archive 14 (`cd/moviecam.lgp`, opened by `App_OpenCoreArchives`), copies it to the heap and sets the
   cursor to 0 and the total to size/4 - 2. For six (disc, id) pairs it skips the camera file; five
   of them open with the loop flag set. It then calls `Movie_Open("<MoviePath><name>.avi", flag,
   ctx+0x74, ctx+0x78)`; a failed open ends playback at once.
3. Request 4 (`FIELD_REQUEST_PLAY_MOVIE`) sets the phase to 1 and calls `Field_StartMovieIfIdle`.
4. Each field update, `Field_UpdateLogic` calls `Field_ServiceMovieRequest` and then
   `Field_UpdateMovieCamera`, and `Field_RenderModels` blits the movie frame
   (`MovieBltFrameToBackBuffer`). `Field_UpdateMovieCamera` takes the camera record from
   `Movie_FetchCameraRecord`. That copies 10
   dwords at `Movie_GetCurrentFrame() * 40` from the camera buffer and advances the cursor; it
   returns zeros when the camera is suppressed, no movie plays, the buffer is gone or the cursor is
   past the end.
5. Request 0x14 (`FIELD_REQUEST_STOP_MOVIE`), or a stop request from lib/movie while a movie plays,
   calls `Field_EndMoviePlayback` and sets the phase to 2.

## Named codes

`sm_movie.cpp` includes `ff7/enums/main.h` and `lib/enums/file.h`, testing the field request byte against
`FIELD_REQUEST_LOAD_MOVIE`, `FIELD_REQUEST_PLAY_MOVIE` and `FIELD_REQUEST_STOP_MOVIE`.

## Notes

- The request byte, its argument and the movie phase are members of `g_FieldActorState`
  (`state.requestCode`, `state.eventCmdParam`, `state.requestArg`); the request codes are
  `FIELD_REQUEST_*`.
- `Field_EndMoviePlayback` writes the previous-mode word through its field alias `g_EnginePreviousMode`
  and keeps a dead `if (0 != 0)` block.
- `Movie_GetDuration` returns 0 unless a movie plays, subtracts 0x14 before its
  range test and never changes its disc index, so ids 0x14..0x27 read the common table and all
  other ids the disc 1 table. What the values measure is not known; the
  comment says frames.
- The function names mix `Field_` and `Movie_` prefixes for one TU.
