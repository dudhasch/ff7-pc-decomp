# Movie (lib): ActiveMovie playback onto DirectDraw surfaces

lib/movie plays one `.avi` file at a time through the ActiveMovie multimedia stream API (DirectShow,
`amstream`). It opens a file into a DirectDraw surface, starts and stops the stream, and copies each
new frame into the back buffer: by DirectDraw blit, by a software copy, or through Direct3D
textures. The game side asks for movies from three places: the main dispatcher (`ending2.avi`), the
credits module, and field through [ff7/movie](../../../ff7/src/movie/README.md). There is no module:
the caller blits a frame each frame while `Movie_IsNotPlaying` is 0, and stops or releases the graph when
`Movie_IsStopRequested` says the stream has ended.

## Where the code is

`src/lib/src/movie/`: 2 TUs, 25 functions. TU boundaries follow retail's objects.

| TU | span | functions | holds |
|---|---|---:|---|
| `movie.cpp` | 0x00414f00-0x004186e0 | 15 | open, run, stop, blit, release, state getters |
| `movie_directdraw_d3d_setup.cpp` | 0x004186e0-0x00419210 | 10 | surface logging; a standalone DirectDraw/Direct3D setup |

Both TUs make 87 calls to `Trace_Printf` (0x00414ee0, `src/ff7/src/main/main_no_op_hooks.cpp`), the release build's trace
printf: the movie code's log lines call it, and it ignores them and returns 0. It sits right
before `movie.cpp`. See
[main](../../../ff7/src/main/README.md).

`movie_directdraw_d3d_setup.cpp` also defines five battle globals by their retail span
(`g_Battle_fileBuffer`, `g_Battle_sceneBuffer`, `g_Battle_partyVal`,
`g_Battle_partyStat`, `g_BattleCameraGlobalScripts`); only battle TUs use them.

## Entry points

| function | address | what it does | called from |
|---|---|---|---|
| `Movie_Open` | 0x00416995 | builds the stream, surface, sample and (with Direct3D) four texture surfaces; sets `g_Movie_isGraphOpen` | `Movie_LoadCameraData`, `Credits_OpenMovie`, `GameModeDispatcher_Frame` |
| `Movie_Run` | 0x00417ab0 | sets the stream running and updates the first sample; sets `g_Movie_isPlaying` | ff7/movie, credits, dispatcher |
| `MovieBltFrameToBackBuffer` | 0x00417f39 | waits for the next sample and puts it on the target surface | dispatcher, credits, `Field_RenderModels` |
| `Movie_Stop` | 0x00417bb2 | stops the stream; clears playing and stop-request flags | dispatcher, `App_OnActivateApp`, `Movie_ReleaseGraph` |
| `Movie_ReleaseGraph` | 0x00417759 | stops and releases every COM object and texture | ff7/movie, credits, dispatcher, `App_Shutdown` |
| `Movie_GetCurrentFrame` | 0x00418613 | current position from `IMediaSeeking` (time format frame) | `Movie_FetchCameraRecord`, credits, field, the blit code |
| `Movie_IsNotPlaying` | 0x004186b6 | `g_Movie_isPlaying == 0` | dispatcher, field, ff7/movie |
| `Movie_IsStopRequested` | 0x004186c7 | `g_Movie_isStopRequested` | dispatcher, credits, ff7/movie |
| `Movie_IsGraphOpen` | 0x004186d1 | `g_Movie_isGraphOpen` | dispatcher, ff7/movie |
| `Movie_RestoreSurface` | 0x00417a4c | restores the movie surface when it was lost | `App_OnActivateApp` |

Seven functions of `movie_directdraw_d3d_setup.cpp` are not called by name anywhere:
`Movie_CreateDirectDrawObject` (tries the 3Dfx DirectDraw GUID first), `Movie_CreateFullscreenFlipSurfaces`,
`Movie_ReleasePrimarySurface`, `Movie_RestorePrimarySurface`, `Movie_FlipPrimarySurface`,
`Movie_CreateDirect3DObjects` and `Movie_ReleaseDirect3DObjects`. `Movie_LogSurfaceDescription` is
called by `Movie_Open`.

## Main structures

| type or global | header | what it holds |
|---|---|---|
| `g_MovieAMMultiMediaStream`, `g_MoviePrimaryVideoStream`, `g_MovieDirectDrawMediaStream` | `movie.cpp` | the stream objects |
| `g_Movie_pAmSurface`, `g_Movie_pStreamSample` | `movie.cpp` | the DirectDraw surface the stream decodes into, and its sample |
| `g_Movie_pGraphBuilder`, `g_MovieMediaSeeking` | `movie.cpp` | filter graph and seeking interface |
| `g_Movie_pSts1`, `g_Movie_pSts2`, `g_Movie_pVts1`, `g_Movie_pVts2`, `g_Movie_pTex*`, `g_Movie_hTex1`, `g_Movie_hTex2` | `movie.cpp` | Direct3D texture surfaces and handles (256x256 and 64x256) |
| `g_Movie_isPlaying`, `g_Movie_isStopRequested`, `g_Movie_isGraphOpen` | `movie.cpp`; `include/lib/api/movie.h` declares `g_Movie_isPlaying` | playback state read by the getters |
| `g_Movie_movieRect_*`, `g_Movie_targetRect_*` | `movie.cpp` | source and centred destination rectangles |
| `g_Movie_quad_Mode{0,1,2}_{224,240}_{Left,Right}`, `g_Movie_quadVertices1`/`2` | `movie.cpp` | four D3DTLVERTEX (`GfxTlVertex`) per UI mode and movie height; the chosen pair is copied into the two quad buffers |
| `g_Movie_loopFlag`, `g_Movie_allowFrameDrop`, `g_Movie_useD3D`, `g_Movie_scalingEnabled` | `movie.cpp` | open-time options |
| `ComAMMultiMediaStream`, `ComDirectDrawMediaStream`, `ComStreamSample`, `ComMediaSeeking` | `lib/com_abi.h` | pure-virtual DirectShow and DirectDraw/Direct3D COM vtable interfaces called by `movie.cpp` |

## Data flow

1. `Movie_Open(path, loopFlag, pDirectDraw, pDirectDraw2)` returns 1 at once when the engine
   context's driver word (+0xA60) is 3. Otherwise it calls `Movie_SetFrameDropForMovie` (sets
   `g_Movie_allowFrameDrop` for 24 named movies under `<MoviePath>`) and
   `Movie_ResetPlaybackState`.
2. It creates the `AMMultiMediaStream`, adds the primary video stream on `pDirectDraw` and the
   primary audio stream (a failure here is only logged), opens the file, and gets the
   `IDirectDrawMediaStream` and its format.
3. `Movie_SelectScalingTables` picks the quad tables for the UI mode and a height of 224 or 240
   lines. The target rectangle is centred.
4. It creates the movie surface, retrying in system memory when video memory runs out, and the
   stream sample. With Direct3D (`g_Movie_useD3D`) it creates the four texture surfaces on
   `pDirectDraw2` and their texture handles.
5. It gets the filter graph and `IMediaSeeking`, sets the time format to frames, and sets
   `g_Movie_isGraphOpen`. A failed step logs and returns 0; most steps up to the stream sample also
   release what they made so far, the Direct3D texture steps do not.
6. `Movie_Run` starts the stream. Each frame the caller calls `MovieBltFrameToBackBuffer`. Driver 1
   uses `Movie_BltSampleSurface`: a software copy with `Gfx_CopySurfaceRectDoubled16` (UI mode 2)
   or `Gfx_CopySurfaceRectDwords`. Otherwise the sample is blitted to the target when scaling
   without Direct3D, or, for Direct3D, its rows are copied into the `sts1` texture surface. The
   rest of this 1,654-byte function is not described here.
7. At end of stream (sample status 0x40003) a looping movie seeks to 0; any other movie sets
   `g_Movie_isStopRequested`. Under 15 frames per second (context +0x48) and past frame 0x32 or
   0x19, the movies marked by `Movie_SetFrameDropForMovie` update their sample with flag 3 instead
   of 0.
8. `Movie_Stop` and `Movie_ReleaseGraph` end it. `Movie_Stop` does nothing unless a movie plays;
   `Movie_ReleaseGraph` does nothing unless a sample exists, and nothing with driver 3.

## Notes

- `movie.cpp` calls the DirectShow streams through named pure-virtual COM interfaces in
  `include/lib/com_abi.h` (`ComAMMultiMediaStream`, `ComDirectDrawMediaStream`, `ComStreamSample`,
  `ComMediaSeeking`) with 32-bit `int` stack arguments; `movie_directdraw_d3d_setup.cpp` uses `ddraw.h`/`d3d.h` directly.
- The engine context is read by offset: +0xA60 driver, +0x2E0 Direct3D device, +0x48 frame rate.
- `Movie_Open` keeps VC5's frame with `unusedNN[]` arrays.
- The movie-file names in `Movie_SetFrameDropForMovie` are 24 inline string compares through a
  macro.
