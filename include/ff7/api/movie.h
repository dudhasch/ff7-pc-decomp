/* include/ff7/api/movie.h: the functions and objects ff7/movie defines that other TUs use.
 * One declaration per name. */
#ifndef FF7_API_MOVIE_H
#define FF7_API_MOVIE_H

#ifdef __cplusplus
extern "C" {
#endif

/* sm_movie.cpp */
extern int g_MovieCameraDataBuffer;
extern int g_MovieFlag;
extern int g_MovieCameraRecordCursor;
extern int g_MovieCameraRecordTotalWords;
extern int g_MovieCameraSuppressed;
extern unsigned char g_MovieCameraRecord[40];
int __cdecl Movie_LoadCameraData(int unusedResult, int movieId);
int __cdecl Field_StartMovieIfIdle(void);
void __cdecl Movie_FreeCameraDataBuffer(void);
void __cdecl Field_EndMoviePlayback(void);
void __cdecl Field_UnusedNoOp_1(void);
void __cdecl Field_ServiceMovieRequest(void);
int __cdecl Movie_GetDuration(int movieId);

#ifdef __cplusplus
}
#endif

#endif /* FF7_API_MOVIE_H */
