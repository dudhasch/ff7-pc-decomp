// Contents: Field movie playback: loads the camera data and movie path for a movie id, starts and
//   ends playback, services start and stop requests from the field state, and frees the movie
//   camera buffer.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include <string.h>
#include "ff7/types/movie.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "ff7/enums/main.h"
#include "ff7/api/main.h"
#include "lib/api/graphics.h"
#include "ff7/api/field.h"
#include "ff7/api/movie.h"
#include "lib/api/movie.h"
#include "ff7/savemap.h"
#include "lib/api/file.h"
#include "lib/enums/file.h"
#include "ff7/field.h"
extern "C" {

#pragma intrinsic(strcpy, strcat)

/*
 * sm_movie.cpp - Movie playback and camera metadata loader for FF7 PC.
 */

typedef struct MovieCameraLoadFrame {
    int index;
    int size;
    char *camera;
    int directDraw;
    int directDraw2;
    int entry;
    int engine;
    int disc;
    int flag;
    char *movie;
    char unused[256];
    char path[256];
} MovieCameraLoadFrame;

typedef struct FieldEndMovieFrame {
    int scale;
    float color[4];
    int context;
} FieldEndMovieFrame;

// GLOBAL: FF7 0x007bae80
char *g_MovieNames_Common[20] = {
    // STRING: FF7 0x007bb378
    "fship2.avi",
    // STRING: FF7 0x007bb384
    "fship2n.avi",
    // STRING: FF7 0x007bb390
    "d_ropego.avi",
    // STRING: FF7 0x007bb3a0
    "d_ropein.avi",
    // STRING: FF7 0x007bb3b0
    "u_ropein.avi",
    // STRING: FF7 0x007bb3c0
    "u_ropego.avi",
    // STRING: FF7 0x007bb3d0
    "gold2.avi",
    // STRING: FF7 0x007bb3dc
    "gold3.avi",
    // STRING: FF7 0x007bb3e8
    "gold4.avi",
    // STRING: FF7 0x007bb3f4
    "gold6.avi",
    // STRING: FF7 0x007bb400
    "gold5.avi",
    // STRING: FF7 0x007bb40c
    "boogup.avi",
    // STRING: FF7 0x007bb418
    "boogdown.avi",
    // STRING: FF7 0x007bb428
    "junair_u.avi",
    // STRING: FF7 0x007bb438
    "junair_d.avi",
    // STRING: FF7 0x007bb448
    "junelein.avi",
    // STRING: FF7 0x007bb458
    "junelego.avi",
    // STRING: FF7 0x007bb468
    "junin_in.avi",
    // STRING: FF7 0x007bb478
    "junin_go.avi",
    // STRING: FF7 0x007bb488
    "moriya.avi",
};
// GLOBAL: FF7 0x007baed0
char *g_MovieNames_Disc1[34] = {
    // STRING: FF7 0x007bb494
    "mkup.avi",
    // STRING: FF7 0x007bb4a0
    "northmk.avi",
    // STRING: FF7 0x007bb4ac
    "mk8.avi",
    // STRING: FF7 0x007bb4b4
    "ontrain.avi",
    // STRING: FF7 0x007bb4c0
    "mainplr.avi",
    // STRING: FF7 0x007bb4cc
    "smk.avi",
    // STRING: FF7 0x007bb4d4
    "southmk.avi",
    // STRING: FF7 0x007bb4e0
    "plrexp.avi",
    // STRING: FF7 0x007bb4ec
    "fallpl.avi",
    // STRING: FF7 0x007bb4f8
    "monitor.avi",
    // STRING: FF7 0x007bb504
    "bike.avi",
    // STRING: FF7 0x007bb510
    "mtnvl.avi",
    // STRING: FF7 0x007bb51c
    "mtnvl2.avi",
    // STRING: FF7 0x007bb528
    "brgnvl.avi",
    // STRING: FF7 0x007bb534
    "nvlmk.avi",
    // STRING: FF7 0x007bb540
    "nivlsfs.avi",
    // STRING: FF7 0x007bb54c
    "jenova_e.avi",
    // STRING: FF7 0x007bb55c
    "junon.avi",
    // STRING: FF7 0x007bb568
    "hiwind0.avi",
    // STRING: FF7 0x007bb574
    "mtcrl.avi",
    // STRING: FF7 0x007bb580
    "gold1.avi",
    // STRING: FF7 0x007bb58c
    "biskdead.avi",
    // STRING: FF7 0x007bb59c
    "boogdemo.avi",
    // STRING: FF7 0x007bb5ac
    "boogstar.avi",
    // STRING: FF7 0x007bb5bc
    "setogake.avi",
    // STRING: FF7 0x007bb5cc
    "rcktfail.avi",
    // STRING: FF7 0x007bb5dc
    "jairofly.avi",
    // STRING: FF7 0x007bb5ec
    "jairofal.avi",
    // STRING: FF7 0x007bb5fc
    "gold7.avi",
    // STRING: FF7 0x007bb608
    "gold7_2.avi",
    // STRING: FF7 0x007bb614
    "earithdd.avi",
    // STRING: FF7 0x007bb624
    "funeral.avi",
    // STRING: FF7 0x007bb630
    "car_1209.avi",
    // STRING: FF7 0x007bb640
    "opening.avi",
};
// GLOBAL: FF7 0x007baf58
char *g_MovieNames_Disc2[42] = {
    // STRING: FF7 0x007bb64c
    "greatpit.avi",
    // STRING: FF7 0x007bb65c
    "c_scene1.avi",
    // STRING: FF7 0x007bb66c
    "c_scene2.avi",
    // STRING: FF7 0x007bb67c
    "c_scene3.avi",
    // STRING: FF7 0x007bb68c
    "biglight.avi",
    // STRING: FF7 0x007bb69c
    "meteosky.avi",
    // STRING: FF7 0x007bb6ac
    "weapon0.avi",
    // STRING: FF7 0x007bb6b8
    "weapon1.avi",
    // STRING: FF7 0x007bb6c4
    "weapon2.avi",
    // STRING: FF7 0x007bb6d0
    "weapon3.avi",
    // STRING: FF7 0x007bb6dc
    "weapon4.avi",
    // STRING: FF7 0x007bb6e8
    "weapon5.avi",
    // STRING: FF7 0x007bb6f4
    "hwindfly.avi",
    // STRING: FF7 0x007bb704
    "phoenix.avi",
    // STRING: FF7 0x007bb710
    "nrcrl.avi",
    // STRING: FF7 0x007bb71c
    "nrcrl_b.avi",
    // STRING: FF7 0x007bb728
    "dumcrush.avi",
    // STRING: FF7 0x007bb738
    "zmind01.avi",
    // STRING: FF7 0x007bb744
    "zmind02.avi",
    // STRING: FF7 0x007bb750
    "zmind03.avi",
    // STRING: FF7 0x007bb75c
    "gelnica.avi",
    // STRING: FF7 0x007bb768
    "rcketoff.avi",
    // STRING: FF7 0x007bb778
    "white2.avi",
    // STRING: FF7 0x007bb784
    "junsea.avi",
    // STRING: FF7 0x007bb790
    "rckethit0.avi",
    // STRING: FF7 0x007bb7a0
    "rckethit1.avi",
    // STRING: FF7 0x007bb7b0
    "meteofix.avi",
    // STRING: FF7 0x007bb7c0
    "canonon.avi",
    // STRING: FF7 0x007bb7cc
    "feelwin0.avi",
    // STRING: FF7 0x007bb7dc
    "feelwin1.avi",
    // STRING: FF7 0x007bb7ec
    "canonht1.avi",
    // STRING: FF7 0x007bb7fc
    "canonht2.avi",
    // STRING: FF7 0x007bb80c
    "canonh3f.avi",
    // STRING: FF7 0x007bb81c
    "parashot.avi",
    // STRING: FF7 0x007bb82c
    "hwindjet.avi",
    // STRING: FF7 0x007bb83c
    "canonht0.avi",
    // STRING: FF7 0x007bb84c
    "wh2e2.avi",
    // STRING: FF7 0x007bb858
    "loslake1.avi",
    // STRING: FF7 0x007bb868
    "lslmv.avi",
    // STRING: FF7 0x007bb874
    "canonh1p.avi",
    // STRING: FF7 0x007bb884
    "canon.avi",
    0,
};
// GLOBAL: FF7 0x007bb000
char *g_MovieNames_Disc3[10] = {
    // STRING: FF7 0x007bb890
    "last4_2.avi",
    // STRING: FF7 0x007bb89c
    "last4_3.avi",
    // STRING: FF7 0x007bb8a8
    "last4_4.avi",
    // STRING: FF7 0x007bb8b4
    "lastmap.avi",
    // STRING: FF7 0x007bb8c0
    "lastflor.avi",
    // STRING: FF7 0x007bb8d0
    "ending1.avi",
    // STRING: FF7 0x007bb8dc
    "ending3.avi",
    // STRING: FF7 0x007bb8e8
    "fcar.avi",
    // STRING: FF7 0x007bb8f4
    "white2.avi",
    // STRING: FF7 0x007bb900
    "ending2.avi",
};
// GLOBAL: FF7 0x007bb028
char *g_CameraNames_Common[20] = {
    // STRING: FF7 0x007bb90c
    "fship2.cam",
    // STRING: FF7 0x007bb918
    "fship2n.cam",
    // STRING: FF7 0x007bb924
    "dropego.cam",
    // STRING: FF7 0x007bb930
    "dropein.cam",
    // STRING: FF7 0x007bb93c
    "uropein.cam",
    // STRING: FF7 0x007bb948
    "uropego.cam",
    // STRING: FF7 0x007bb954
    "gold2.cam",
    // STRING: FF7 0x007bb960
    "gold3.cam",
    // STRING: FF7 0x007bb96c
    "gold4.cam",
    // STRING: FF7 0x007bb978
    "gold6.cam",
    // STRING: FF7 0x007bb984
    "gold5.cam",
    // STRING: FF7 0x007bb990
    "boogup.cam",
    // STRING: FF7 0x007bb99c
    "boogdown.cam",
    // STRING: FF7 0x007bb9ac
    "junaird.cam",
    // STRING: FF7 0x007bb9b8
    "junairu.cam",
    // STRING: FF7 0x007bb9c4
    "junelein.cam",
    // STRING: FF7 0x007bb9d4
    "junelego.cam",
    // STRING: FF7 0x007bb9e4
    "junin_in.cam",
    // STRING: FF7 0x007bb9f4
    "junin_go.cam",
    // STRING: FF7 0x007bba04
    "moriya.cam",
};
// GLOBAL: FF7 0x007bb078
char *g_CameraNames_Disc1[34] = {
    // STRING: FF7 0x007bba10
    "mkup.cam",
    // STRING: FF7 0x007bba1c
    "northmk.cam",
    // STRING: FF7 0x007bba28
    "mk8.cam",
    // STRING: FF7 0x007bba30
    "ontrain.cam",
    // STRING: FF7 0x007bba3c
    "mainplr.cam",
    // STRING: FF7 0x007bba48
    "smk.cam",
    // STRING: FF7 0x007bba50
    "southmk.cam",
    // STRING: FF7 0x007bba5c
    "plrexp.cam",
    // STRING: FF7 0x007bba68
    "fallpl.cam",
    // STRING: FF7 0x007bba74
    "monitor.cam",
    // STRING: FF7 0x007bba80
    "bikeget.cam",
    // STRING: FF7 0x007bba8c
    "mtnvl.cam",
    // STRING: FF7 0x007bba98
    "mtnvl2.cam",
    // STRING: FF7 0x007bbaa4
    "brgnvl.cam",
    // STRING: FF7 0x007bbab0
    "nvlmk.cam",
    // STRING: FF7 0x007bbabc
    "nivlsfs.cam",
    // STRING: FF7 0x007bbac8
    "nivljnv.cam",
    // STRING: FF7 0x007bbad4
    "junon.cam",
    // STRING: FF7 0x007bbae0
    "hiwind0.cam",
    // STRING: FF7 0x007bbaec
    "mtcrl.cam",
    // STRING: FF7 0x007bbaf8
    "gold1.cam",
    // STRING: FF7 0x007bbb04
    "biskdead.cam",
    // STRING: FF7 0x007bbb14
    "boogdemo.cam",
    // STRING: FF7 0x007bbb24
    "boogstar.cam",
    // STRING: FF7 0x007bbb34
    "setogake.cam",
    // STRING: FF7 0x007bbb44
    "rcktfail.cam",
    // STRING: FF7 0x007bbb54
    "jairofly.cam",
    // STRING: FF7 0x007bbb64
    "jairofal.cam",
    // STRING: FF7 0x007bbb74
    "gold7.cam",
    // STRING: FF7 0x007bbb80
    "gold7_2.cam",
    // STRING: FF7 0x007bbb8c
    "earithdd.cam",
    // STRING: FF7 0x007bbb9c
    "funeral.cam",
    // STRING: FF7 0x007bbba8
    "car_1209.cam",
    // STRING: FF7 0x007bbbb8
    "opening.cam",
};
// GLOBAL: FF7 0x007bb100
char *g_CameraNames_Disc2[42] = {
    // STRING: FF7 0x007bbbc4
    "greatpit.cam",
    // STRING: FF7 0x007bbbd4
    "cscene1.cam",
    // STRING: FF7 0x007bbbe0
    "cscene2.cam",
    // STRING: FF7 0x007bbbec
    "cscene3.cam",
    // STRING: FF7 0x007bbbf8
    "biglight.cam",
    // STRING: FF7 0x007bbc08
    "meteosky.cam",
    // STRING: FF7 0x007bbc18
    "weapon0.cam",
    // STRING: FF7 0x007bbc24
    "weapon1.cam",
    // STRING: FF7 0x007bbc30
    "weapon2.cam",
    // STRING: FF7 0x007bbc3c
    "weapon3.cam",
    // STRING: FF7 0x007bbc48
    "weapon4.cam",
    // STRING: FF7 0x007bbc54
    "weapon5.cam",
    // STRING: FF7 0x007bbc60
    "hwindfly.cam",
    // STRING: FF7 0x007bbc70
    "phoenix.cam",
    // STRING: FF7 0x007bbc7c
    "nrcrl.cam",
    // STRING: FF7 0x007bbc88
    "nrcrlb.cam",
    // STRING: FF7 0x007bbc94
    "dumcrush.cam",
    // STRING: FF7 0x007bbca4
    "zmind11.cam",
    // STRING: FF7 0x007bbcb0
    "zmind21.cam",
    // STRING: FF7 0x007bbcbc
    "zmind31.cam",
    // STRING: FF7 0x007bbcc8
    "gelnica.cam",
    // STRING: FF7 0x007bbcd4
    "rcketoff.cam",
    // STRING: FF7 0x007bbce4
    "white2.cam",
    // STRING: FF7 0x007bbcf0
    "junsea.cam",
    // STRING: FF7 0x007bbcfc
    "rckthit.cam",
    // STRING: FF7 0x007bbd08
    "rckthit2.cam",
    // STRING: FF7 0x007bbd18
    "meteofix.cam",
    // STRING: FF7 0x007bbd28
    "canonon.cam",
    // STRING: FF7 0x007bbd34
    "feelwin0.cam",
    // STRING: FF7 0x007bbd44
    "feelwin1.cam",
    // STRING: FF7 0x007bbd54
    "canonht1.cam",
    // STRING: FF7 0x007bbd64
    "canonht2.cam",
    // STRING: FF7 0x007bbd74
    "canonh3f.cam",
    // STRING: FF7 0x007bbd84
    "parashot.cam",
    // STRING: FF7 0x007bbd94
    "hwindjet.cam",
    // STRING: FF7 0x007bbda4
    "canonht0.cam",
    // STRING: FF7 0x007bbdb4
    "wh2e2.cam",
    // STRING: FF7 0x007bbdc0
    "loslake1.cam",
    // STRING: FF7 0x007bbdd0
    "lslmv.cam",
    // STRING: FF7 0x007bbddc
    "canonh1p.cam",
    // STRING: FF7 0x007bbdec
    "canon.cam",
    0,
};
// GLOBAL: FF7 0x007bb1a8
char *g_CameraNames_Disc3[10] = {
    // STRING: FF7 0x007bbdf8
    "last4_2.cam",
    // STRING: FF7 0x007bbe04
    "last4_3.cam",
    // STRING: FF7 0x007bbe10
    "last4_4.cam",
    // STRING: FF7 0x007bbe1c
    "lastmap.cam",
    // STRING: FF7 0x007bbe28
    "lastflor.cam",
    // STRING: FF7 0x007bbe38
    "ending1.cam",
    // STRING: FF7 0x007bbe44
    "ending3.cam",
    // STRING: FF7 0x007bbe50
    "fcar.cam",
    // STRING: FF7 0x007bbe5c
    "white2.cam",
    // STRING: FF7 0x007bbe68
    "ending2.cam",
};
// GLOBAL: FF7 0x009a0710
int g_MovieCameraDataBuffer = 0;
unsigned char g_bssPad_009a0714[4] = {0};
// GLOBAL: FF7 0x009a0718
int g_MovieFlag = 0;
// GLOBAL: FF7 0x009a071c
int g_MovieCameraRecordCursor = 0;
// GLOBAL: FF7 0x009a0720
int g_MovieCameraRecordTotalWords = 0;
// GLOBAL: FF7 0x009a0724
char *g_MovieCameraAllocatedBuffer = 0;
unsigned char g_bssPad_009a0728[4] = {0};
// GLOBAL: FF7 0x009a072c
int g_MovieCameraSuppressed = 0;
// GLOBAL: FF7 0x009a0730
unsigned char g_MovieCameraRecord[40] = {0};

/*
 * Loads camera metadata and movie path for the specified movie ID and active disc,
 * reads the camera data from LGP archive into an allocated heap buffer, and initiates
 * movie playback via Movie_Open.
 */
// FUNCTION: FF7 0x0040adb0
int __cdecl Movie_LoadCameraData(int unusedResult, int movieId)
{
    MovieCameraLoadFrame f;

    f.directDraw = 0;
    f.directDraw2 = 0;
    f.flag = 0;
    g_MovieCameraRecordCursor = 0;
    g_MovieCameraSuppressed = 0;
    f.disc = gSavemap.discChangeRequest;
    f.disc--;
    if (f.disc == 0 && movieId == 0x24)
        g_MovieCameraSuppressed = 1;
    if (f.disc == 1 && movieId == 0x2a)
        g_MovieCameraSuppressed = 1;
    if (f.disc == 2 && (movieId == 0x14 || movieId == 0x15 || movieId == 0x17 || movieId == 0x1c))
        g_MovieCameraSuppressed = 1;
    if (g_MovieCameraSuppressed == 0) {
        if (movieId >= 0 && movieId <= 0x13)
            f.camera = g_CameraNames_Common[movieId];
        else {
            f.index = movieId;
            f.index -= 0x14;
            if (f.disc == 0)
                f.camera = g_CameraNames_Disc1[f.index];
            else if (f.disc == 1)
                f.camera = g_CameraNames_Disc2[f.index];
            else if (f.disc == 2)
                f.camera = g_CameraNames_Disc3[f.index];
        }
        strcpy(f.path, f.camera);
        f.entry = (int)FindLgpFile(f.path, LGP_ARCHIVE_MOVIE_CAMERA);
        if (f.entry != 0) {
            f.size = GetLgpEntrySize(f.entry, LGP_ARCHIVE_MOVIE_CAMERA);
            // STRING: FF7 0x007bbe74
            g_MovieCameraAllocatedBuffer = (char *)GameHeapAllocRaw(f.size + 2, "C:\\FF7\\Src\\movie\\sm_movie.cpp", 0xe5);
            ReadLgpEntry((void *)f.entry, LGP_ARCHIVE_MOVIE_CAMERA, g_MovieCameraAllocatedBuffer, f.size);
            g_MovieCameraAllocatedBuffer[f.size] = 0;
        }
        g_MovieCameraDataBuffer = (int)g_MovieCameraAllocatedBuffer;
        g_MovieCameraRecordTotalWords = f.size / 4;
        g_MovieCameraRecordTotalWords -= 2;
    }
    unusedResult = 1;
    if (f.disc == 1) {
        if (movieId == 0x2a) {
            f.flag = 1;
            g_MovieFlag = 1;
        } else
            g_MovieFlag = 0;
    }
    if (f.disc == 2) {
        if (movieId == 0x14 || movieId == 0x15 || movieId == 0x17 || movieId == 0x1c) {
            f.flag = 1;
            g_MovieFlag = 1;
        } else
            g_MovieFlag = 0;
    }
    if (movieId >= 0 && movieId <= 0x13)
        f.movie = g_MovieNames_Common[movieId];
    else {
        movieId -= 0x14;
        if (f.disc == 0)
            f.movie = g_MovieNames_Disc1[movieId];
        else if (f.disc == 1)
            f.movie = g_MovieNames_Disc2[movieId];
        else if (f.disc == 2)
            f.movie = g_MovieNames_Disc3[movieId];
    }
    strcpy(f.path, g_InitPath_MoviePath);
    strcat(f.path, f.movie);
    // STRING: FF7 0x007bbe94
    Trace_Printf("Opening movie: %s\n", f.path);
    f.engine = (int)GetEngineContext();
    f.directDraw = ((GfxEngineContext *)f.engine)->ddObject;
    f.directDraw2 = ((GfxEngineContext *)f.engine)->ddObject2;
    if (Movie_Open((int)f.path, f.flag, f.directDraw, f.directDraw2) == 0)
        Field_EndMoviePlayback();
    return unusedResult;
}

/*
 * Starts the movie that Movie_LoadCameraData opened, for the field's play request
 * (FIELD_REQUEST_PLAY_MOVIE): runs the DirectShow graph (Movie_Run) unless a movie is already
 * playing, so the request, which Field_ServiceMovieRequest sees on every update, starts it only
 * once. The engine context it fetches is not used. Always returns 1; the caller ignores it.
 */
// FUNCTION: FF7 0x0040b19d
int __cdecl Field_StartMovieIfIdle(void)
{
    GfxEngineContext *engineContext;

    engineContext = GetEngineContext();
    if (Movie_IsNotPlaying() != 0) {
        Movie_Run();
    }
    return 1;
}

/*
 * Frees the heap copy of the current movie's .cam camera file, which Movie_LoadCameraData made:
 * when g_MovieCameraDataBuffer is set it frees g_MovieCameraAllocatedBuffer and clears both, so
 * Movie_FetchCameraRecord hands the field zero records from then on. Field_EndMoviePlayback calls
 * it when a movie ends, and App_Shutdown on exit.
 */
// FUNCTION: FF7 0x0040b1c0
void __cdecl Movie_FreeCameraDataBuffer(void)
{
    if (g_MovieCameraDataBuffer != 0) {
        // STRING: FF7 0x007bbea8
        GameHeapFree(g_MovieCameraAllocatedBuffer, "C:\\FF7\\Src\\movie\\sm_movie.cpp", 0x169);
        g_MovieCameraAllocatedBuffer = 0;
        g_MovieCameraDataBuffer = 0;
    }
}

// GLOBAL: FF7 0x007bb1d0
int g_MovieDurationsCommon[20] = {
    120, 0, 168, 161, 146, 135, 202, 168, 480, 150, 147, 60, 60, 66, 66, 90, 90, 105, 105, 0
};
// GLOBAL: FF7 0x007bb220
int g_MovieDurationsDisc1[34] = {
    137, 200, 51, 150, 195, 36, 240, 150, 804, 74, 749, 300,
    300, 199, 315, 300, 0, 480, 285, 60, 675, 150, 0, 1251,
    135, 450, 630, 226, 294, 351, 1046, 540, 1079, 1789
};
// GLOBAL: FF7 0x007bb2a8
int g_MovieDurationsDisc2[42] = {
    506, 214, 182, 205, 456, 265, 441, 306, 381, 456, 186, 666,
    759, 396, 287, 136, 351, 139, 139, 229, 307, 826, 30, 66,
    217, 394, 376, 675, 305, 170, 390, 211, 282, 512, 0, 216,
    652, 241, 115, 109, 1161, 0
};
// GLOBAL: FF7 0x007bb350
int g_MovieDurationsDisc3[10] = {
    20, 20, 454, 0, 124, 856, 1322, 1079, 30, 7981
};

/*
 * Ends a field movie: frees the camera data (Movie_FreeCameraDataBuffer), releases the DirectShow
 * graph (Movie_ReleaseGraph), sets the previous game mode to GAME_MODE_FIELD_SERVICE and restarts
 * the field's frame count (Field_ResetFrameCounter). Field_ServiceMovieRequest calls it for a stop
 * request or when lib/movie reports the end of the movie, and Movie_LoadCameraData when the .avi
 * does not open. The block that would clear the screen to black and present it never runs: its
 * condition is the constant DEAD_IN_RETAIL.
 */
// FUNCTION: FF7 0x0040b1fa
void __cdecl Field_EndMoviePlayback(void)
{
    FieldEndMovieFrame frame;

    Movie_FreeCameraDataBuffer();
    Movie_ReleaseGraph();
    if (DEAD_IN_RETAIL != 0) {
        frame.context = (int)GetEngineContext();
        frame.scale = GetUiScalingMode();
        frame.color[2] = 0.0f;
        frame.color[1] = 0.0f;
        frame.color[0] = 0.0f;
        frame.color[3] = 1.0f;
        GfxSetBackgroundColorCall(frame.color, (struct GfxEngineContext *)frame.context);
        PresentFrame((GfxEngineContext *)frame.context);
        GfxClearAllCall((struct GfxEngineContext *)frame.context);
    }
    g_EnginePreviousMode = GAME_MODE_FIELD_SERVICE;
    Field_ResetFrameCounter();
}

/*
 * Unused stub hook; performs no operation.
 */
// FUNCTION: FF7 0x0040b276
void __cdecl Field_UnusedNoOp_1(void)
{
}

/*
 * Carries out the field script's movie requests; Field_UpdateLogic calls it on every field update.
 * The request is g_FieldActorState.state.requestCode, the movie id its eventCmdParam, and
 * state.requestArg the movie phase the script waits on (1 running, 2 done).
 * - lib/movie asks to stop (a non-looping movie reached its end) while a movie plays, or the
 *   request is FIELD_REQUEST_STOP_MOVIE: ends playback (Field_EndMoviePlayback), clears
 *   g_FieldLogicActive, sets the phase to 2 and returns.
 * - FIELD_REQUEST_LOAD_MOVIE with phase 0: loads the .cam camera data and opens the .avi
 *   (Movie_LoadCameraData) unless a graph is already open; the load request then reports phase 2
 *   in the same call.
 * - When the previous call started playback (g_FieldLogicActive 1) it clears that and sets the
 *   phase to 2.
 * - FIELD_REQUEST_PLAY_MOVIE: sets the phase to 1, starts the movie if none is playing
 *   (Field_StartMovieIfIdle) and sets g_FieldLogicActive.
 */
// FUNCTION: FF7 0x0040b27b
void __cdecl Field_ServiceMovieRequest(void)
{
    int unused;

    unused = 0;
    if (Movie_IsStopRequested() != 0 && Movie_IsNotPlaying() == 0) {
        Field_EndMoviePlayback();
        g_FieldLogicActive = 0;
        g_FieldActorState.state.requestArg = 2;
        return;
    }
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_STOP_MOVIE) {
        Field_EndMoviePlayback();
        g_FieldLogicActive = 0;
        g_FieldActorState.state.requestArg = 2;
        return;
    }
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_LOAD_MOVIE && g_FieldActorState.state.requestArg == 0) {
        if (Movie_IsGraphOpen() == 0) {
            Movie_LoadCameraData(unused, g_FieldActorState.state.eventCmdParam);
        }
        g_FieldActorState.state.requestArg = 1;
    }
    if (g_FieldLogicActive == 1) {
        g_FieldLogicActive = 0;
        g_FieldActorState.state.requestArg = 2;
    }
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_LOAD_MOVIE) {
        g_FieldActorState.state.requestArg = 2;
    }
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_PLAY_MOVIE) {
        g_FieldActorState.state.requestArg = 1;
        Field_StartMovieIfIdle();
        g_FieldLogicActive = 1;
    }
}

/*
 * Looks up the movie duration (in frames) from indexed duration tables for the given movie ID.
 */
// FUNCTION: FF7 0x0040b373
int __cdecl Movie_GetDuration(int movieId)
{
    int disc;
    int frames;

    frames = 0;
    movieId -= 0x14;
    disc = 0;
    if (g_Movie_isPlaying != 0) {
        if (movieId >= 0 && movieId <= 0x13) {
            frames = g_MovieDurationsCommon[movieId];
        } else if (disc == 0) {
            frames = g_MovieDurationsDisc1[movieId];
        } else if (disc == 1) {
            frames = g_MovieDurationsDisc2[movieId];
        } else if (disc == 2) {
            frames = g_MovieDurationsDisc3[movieId];
        }
    }
    return frames;
}
} /* extern "C" */
