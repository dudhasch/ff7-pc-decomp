/* include/lib/api/movie.h: the functions and objects lib/movie defines that other TUs use.
 * One declaration per name. */
#ifndef LIB_API_MOVIE_H
#define LIB_API_MOVIE_H

#ifdef __cplusplus
extern "C" {
struct GfxTlVertex;
#endif

/* movie.cpp */
extern struct GfxTlVertex g_Movie_quadVertices2[4];
extern struct GfxTlVertex g_Movie_quadVertices1[4];
extern int g_Movie_isPlaying;
void __cdecl Movie_ResetPlaybackState(void);
void __cdecl Movie_SelectScalingTables(int movieWidth, int movieHeight);
void __cdecl Movie_SetFrameDropForMovie(char *moviePath);
int __cdecl Movie_Open(int moviePath, int loopFlag, int pDirectDraw, int pDirectDraw2);
void __cdecl Movie_ReleaseGraph(void);
int __cdecl Movie_RestoreSurface(void);
int __cdecl Movie_Run(void);
int __cdecl Movie_Stop(void);
int __cdecl Movie_BltSampleSurface(int pTargetSurface);
int __cdecl MovieBltFrameToBackBuffer(int pTargetSurface);
int __cdecl Movie_ProbeMultiMediaStreamInfo(int unused, int pMediaStream);
int __cdecl Movie_GetCurrentFrame(void);
int __cdecl Movie_IsNotPlaying(void);
int __cdecl Movie_IsStopRequested(void);
int __cdecl Movie_IsGraphOpen(void);

/* movie_directdraw_d3d_setup.cpp */
extern int g_Battle_fileBuffer;
extern int g_Battle_sceneBuffer;
extern int g_Battle_partyVal;
extern int g_Battle_partyStat;
extern int g_BattleCameraGlobalScripts;
int __cdecl Movie_CreateDirectDrawObject(void);
int __cdecl Movie_CreateFullscreenFlipSurfaces(void);
void __cdecl Movie_ReleasePrimarySurface(void);
void __cdecl Movie_ReleaseDirectDrawObjects(void);
int __cdecl Movie_RestorePrimarySurface(void);
int __cdecl Movie_FlipPrimarySurface(void);
int __cdecl Movie_CreateDirect3DObjects(void);
void __cdecl Movie_ReleaseDirect3DObjects(void);

struct GfxTlVertex;
extern struct GfxTlVertex g_Movie_quad_Mode2_224_Left[4];
extern struct GfxTlVertex g_Movie_quad_Mode2_224_Right[4];
extern struct GfxTlVertex g_Movie_quad_Mode2_240_Left[4];
extern struct GfxTlVertex g_Movie_quad_Mode2_240_Right[4];
extern struct GfxTlVertex g_Movie_quad_Mode1_224_Left[4];
extern struct GfxTlVertex g_Movie_quad_Mode1_224_Right[4];
extern struct GfxTlVertex g_Movie_quad_Mode1_240_Left[4];
extern struct GfxTlVertex g_Movie_quad_Mode1_240_Right[4];
extern struct GfxTlVertex g_Movie_quad_Mode0_224_Left[4];
extern struct GfxTlVertex g_Movie_quad_Mode0_224_Right[4];
extern struct GfxTlVertex g_Movie_quad_Mode0_240_Left[4];
extern struct GfxTlVertex g_Movie_quad_Mode0_240_Right[4];

#ifdef __cplusplus
}
#endif

#endif /* LIB_API_MOVIE_H */
