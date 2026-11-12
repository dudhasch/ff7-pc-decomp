// src/ff7/src/main/initpath.cpp
// Original: C:\FF7\Src\main\initpath.cpp
// Contents: Finds the game's installation and data directories in the Windows registry and builds
//   the paths to the data, minigame, MIDI, sound and LGP archive files. It also frees those path
//   buffers and reads and writes the saved music and sound volume settings.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include <windows.h>
#include "lib/api/mem.h"
#include "ff7/enums/main.h"
#include "ff7/api/menu.h"
#include "ff7/api/main.h"
#include "lib/api/sound.h"
#include "lib/enums/file.h"
extern "C" {

#pragma intrinsic(strcmp, strcpy, strcat, memset)

// GLOBAL: FF7 0x009a0598
char g_InitPath_ArchivePath[256] = {0};
// GLOBAL: FF7 0x009a0698
char *g_InitPath_AppPath = 0;
// GLOBAL: FF7 0x009a069c
char *g_InitPath_MoviePath = 0;
// GLOBAL: FF7 0x009a06a0
int g_InitPath_SoundEnabled = 0;
// GLOBAL: FF7 0x009a06a4
char *g_InitPath_MusicPath = 0;
// GLOBAL: FF7 0x009a06a8
char *g_InitPath_SoundPath = 0;
// GLOBAL: FF7 0x009a06ac
char *g_InitPath_MidiPath = 0;
// GLOBAL: FF7 0x009a06b0
int g_InitPath_MidiEnabled = 0;
// GLOBAL: FF7 0x009a06b4
char *g_InitPath_CdDataPath = 0;
// GLOBAL: FF7 0x009a06b8
char *g_InitPath_DataPath = 0;
// GLOBAL: FF7 0x009a06bc
int g_InitPath_FullInstall = 0;
// GLOBAL: FF7 0x009a06c0
char *g_InitPath_DataDrive = 0;
// GLOBAL: FF7 0x009a06c4
char *g_InitPath_MinigameDir = 0;

// GLOBAL: FF7 0x007b6550
static const char REG_APP_KEY[] = "Software\\Square Soft, Inc.\\Final Fantasy VII";
// GLOBAL: FF7 0x007b6580
static const char REG_APPPATH[] = "AppPath";
// GLOBAL: FF7 0x007b6588
static const char REG_MOVIEPATH[] = "MoviePath";
// GLOBAL: FF7 0x007b6598
static const char REG_DATADRIVE[] = "DataDrive";
// GLOBAL: FF7 0x007b65a8
static const char REG_DATAPATH[] = "DataPath";
// GLOBAL: FF7 0x007b65b8
static const char REG_MUSICVOLUME[] = "MusicVolume";
// GLOBAL: FF7 0x007b65c8
static const char REG_SFXVOLUME[] = "SFXVolume";
// GLOBAL: FF7 0x007b65d8
static const char REG_FULLINSTALL[] = "FullInstall";
// GLOBAL: FF7 0x007b65e8
static const char REG_SOUND_KEY[] = "Software\\Square Soft, Inc.\\Final Fantasy VII\\1.00\\Sound";
// GLOBAL: FF7 0x007b6620
static const char REG_MIDI_KEY[] = "Software\\Square Soft, Inc.\\Final Fantasy VII\\1.00\\Midi";

/*
 * Reads installation paths, audio flags, and drive letters from the Windows
 * registry under HKLM\Software\Square Soft, Inc.\Final Fantasy VII and initializes
 * the global path buffers.
 */
// FUNCTION: FF7 0x00406d10
void __cdecl InitPathsFromRegistry(void)
{
    char sysMsg[0x100];
    char valBuf[0x100];
    DWORD dataBytes;
    DWORD regType;
    // STRING: FF7 0x007ba748
    char soundName[6] = "Sound";
    // STRING: FF7 0x007ba750
    char midiValueName[5] = "Midi";
    LONG state;
    HKEY regKey;

    // STRING: FF7 0x007ba758
    g_InitPath_AppPath = (char *)GameHeapAlloc(0x100, 1, "C:\\FF7\\Src\\main\\initpath.cpp", 0x37);
    // STRING: FF7 0x007ba778
    g_InitPath_MoviePath = (char *)GameHeapAlloc(0x100, 1, "C:\\FF7\\Src\\main\\initpath.cpp", 0x38);
    // STRING: FF7 0x007ba798
    g_InitPath_DataDrive = (char *)GameHeapAlloc(0x100, 1, "C:\\FF7\\Src\\main\\initpath.cpp", 0x39);
    // STRING: FF7 0x007ba7b8
    g_InitPath_DataPath = (char *)GameHeapAlloc(0x100, 1, "C:\\FF7\\Src\\main\\initpath.cpp", 0x3A);
    // STRING: FF7 0x007ba7d8
    g_InitPath_SoundPath = (char *)GameHeapAlloc(0x100, 1, "C:\\FF7\\Src\\main\\initpath.cpp", 0x3B);
    // STRING: FF7 0x007ba7f8
    g_InitPath_MusicPath = (char *)GameHeapAlloc(0x100, 1, "C:\\FF7\\Src\\main\\initpath.cpp", 0x3C);
    // STRING: FF7 0x007ba818
    g_InitPath_MidiPath = (char *)GameHeapAlloc(0x100, 1, "C:\\FF7\\Src\\main\\initpath.cpp", 0x3D);
    // STRING: FF7 0x007ba838
    g_InitPath_MinigameDir = (char *)GameHeapAlloc(0x100, 1, "C:\\FF7\\Src\\main\\initpath.cpp", 0x3E);
    // STRING: FF7 0x007ba858
    g_InitPath_CdDataPath = (char *)GameHeapAlloc(0x100, 1, "C:\\FF7\\Src\\main\\initpath.cpp", 0x3F);

    state = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                              REG_APP_KEY, 0, KEY_READ, &regKey);
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, sysMsg, 0x100, 0);
    if (state == ERROR_SUCCESS) {
        dataBytes = 0x100;
        state = RegQueryValueExA(regKey,
                                     REG_DATADRIVE, 0, &regType, (BYTE *)valBuf, &dataBytes);
        strcpy(g_InitPath_DataDrive, valBuf);

        dataBytes = 0x100;
        state = RegQueryValueExA(regKey, soundName, 0, &regType, (BYTE *)valBuf, &dataBytes);
        if (state == ERROR_SUCCESS) {
            if (strcmp(valBuf,
                       // STRING: FF7 0x007ba878
                       "ON") == 0) {
                g_InitPath_SoundEnabled = 1;
            } else {
                g_InitPath_SoundEnabled = 0;
            }
        } else {
            g_InitPath_SoundEnabled = 1;
        }

        dataBytes = 0x100;
        state = RegQueryValueExA(regKey, midiValueName, 0, &regType, (BYTE *)valBuf, &dataBytes);
        if (state == ERROR_SUCCESS) {
            if (strcmp(valBuf,
                       // STRING: FF7 0x007ba87c
                       "ON") == 0) {
                g_InitPath_MidiEnabled = 1;
            } else {
                g_InitPath_MidiEnabled = 0;
            }
        } else {
            g_InitPath_MidiEnabled = 1;
        }

        dataBytes = 0x100;
        state = RegQueryValueExA(regKey,
                                     REG_APPPATH, 0, &regType, (BYTE *)valBuf, &dataBytes);
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, sysMsg, 0x100, 0);
        if (state == ERROR_SUCCESS) {
            strcpy(g_InitPath_AppPath, valBuf);
        } else {
            // The (int) cast keeps the intrinsic strcpy's generic copy: retail does not copy a known length.
            strcpy(g_InitPath_AppPath, (char *)(int)
                   // STRING: FF7 0x007ba880
                   "/ff7/");
        }

        dataBytes = 0x100;
        state = RegQueryValueExA(regKey,
                                     REG_DATAPATH, 0, &regType, (BYTE *)valBuf, &dataBytes);
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, sysMsg, 0x100, 0);
        if (state == ERROR_SUCCESS) {
            strcpy(g_InitPath_DataPath, valBuf);
        } else {
            strcpy(g_InitPath_DataPath, (char *)(int)
                   // STRING: FF7 0x007ba888
                   "/ff7/data/");
        }

        dataBytes = 0x100;
        state = RegQueryValueExA(regKey,
                                     REG_MOVIEPATH, 0, &regType, (BYTE *)valBuf, &dataBytes);
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, sysMsg, 0x100, 0);
        if (state == ERROR_SUCCESS) {
            strcpy(g_InitPath_MoviePath, valBuf);
        } else {
            strcpy(g_InitPath_MoviePath, (char *)(int)
                   // STRING: FF7 0x007ba894
                   "/ff7/data/movies/");
        }

        dataBytes = 4;
        state = RegQueryValueExA(regKey,
                                     REG_FULLINSTALL, 0, &regType, (BYTE *)&g_InitPath_FullInstall, &dataBytes);
        if (state == ERROR_SUCCESS) {
            if (g_InitPath_FullInstall != 0) {
                strcpy(g_InitPath_CdDataPath, g_InitPath_DataPath);
            } else {
                strcpy(g_InitPath_CdDataPath, g_InitPath_DataDrive);
                strcat(g_InitPath_CdDataPath,
                       // STRING: FF7 0x007ba8a8
                       "ff7/");
            }
        } else {
            strcpy(g_InitPath_CdDataPath, g_InitPath_DataDrive);
            strcat(g_InitPath_CdDataPath,
                   // STRING: FF7 0x007ba8b0
                   "ff7/");
        }

        strcpy(g_InitPath_SoundPath, g_InitPath_DataPath);
        strcat(g_InitPath_SoundPath,
               // STRING: FF7 0x007ba8b8
               "sound");
        strcpy(g_InitPath_MusicPath, g_InitPath_DataPath);
        strcat(g_InitPath_MusicPath,
               // STRING: FF7 0x007ba8c0
               "music");
        strcpy(g_InitPath_MidiPath, g_InitPath_DataPath);
        strcat(g_InitPath_MidiPath,
               // STRING: FF7 0x007ba8c8
               "midi");
        state = RegCloseKey(regKey);
    }
}

/*
 * Frees the path string buffers InitPathsFromRegistry allocated.
 */
// FUNCTION: FF7 0x004073f7
void __cdecl Engine_FreePathGlobals(void)
{
    // STRING: FF7 0x007ba8d0
    GameHeapFree(g_InitPath_AppPath, "C:\\FF7\\Src\\main\\initpath.cpp", 0xDF);
    // STRING: FF7 0x007ba8f0
    GameHeapFree(g_InitPath_MoviePath, "C:\\FF7\\Src\\main\\initpath.cpp", 0xE0);
    // STRING: FF7 0x007ba910
    GameHeapFree(g_InitPath_DataDrive, "C:\\FF7\\Src\\main\\initpath.cpp", 0xE1);
    // STRING: FF7 0x007ba930
    GameHeapFree(g_InitPath_DataPath, "C:\\FF7\\Src\\main\\initpath.cpp", 0xE2);
    // STRING: FF7 0x007ba950
    GameHeapFree(g_InitPath_SoundPath, "C:\\FF7\\Src\\main\\initpath.cpp", 0xE3);
    // STRING: FF7 0x007ba970
    GameHeapFree(g_InitPath_MusicPath, "C:\\FF7\\Src\\main\\initpath.cpp", 0xE4);
    // STRING: FF7 0x007ba990
    GameHeapFree(g_InitPath_MidiPath, "C:\\FF7\\Src\\main\\initpath.cpp", 0xE5);
    // STRING: FF7 0x007ba9b0
    GameHeapFree(g_InitPath_MinigameDir, "C:\\FF7\\Src\\main\\initpath.cpp", 0xE6);
    // STRING: FF7 0x007ba9d0
    GameHeapFree(g_InitPath_CdDataPath, "C:\\FF7\\Src\\main\\initpath.cpp", 0xE7);
}

/*
 * Writes MusicVolume and SFXVolume registry values under the Midi and Sound keys.
 * Returns 0 on success, or 1 if writing either value failed.
 */
// FUNCTION: FF7 0x004074da
int __cdecl Engine_RegWriteVolumeSettings(int music, int sound)
{
    int failed;
    LONG result;
    HKEY regKey;
    failed = 1;
    result = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_MIDI_KEY, 0, KEY_WRITE, &regKey);
    if (result == 0) {
        failed = 1;
        result = RegSetValueExA(regKey,
                           REG_MUSICVOLUME, 0, REG_DWORD, (BYTE *)&music, 4);
        if (result == 0) {
            failed = 0;
        }
        result = RegCloseKey(regKey);
    }
    result = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_SOUND_KEY, 0, KEY_WRITE, &regKey);
    if (result == 0) {
        failed = 1;
        result = RegSetValueExA(regKey,
                           REG_SFXVOLUME, 0, REG_DWORD, (BYTE *)&sound, 4);
        if (result == 0) {
            failed = 0;
        }
        result = RegCloseKey(regKey);
    }
    return failed;
}

/*
 * Queries MusicVolume and SFXVolume from the registry, clamps them to [0, 100],
 * and applies them via Engine_ApplyMusicAndSfxVolume.
 * Returns 1 if settings were successfully queried and applied, 0 otherwise.
 */
// FUNCTION: FF7 0x004075b0
int __cdecl ApplyRegistryVolumeSettings(void)
{
    DWORD dataBytes;
    int musicVolume;
    LONG state;
    DWORD type;
    int sfxVolume;
    HKEY hKey;
    int success;
    success = 1;
    state = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_MIDI_KEY, 0, KEY_QUERY_VALUE, &hKey);
    if (state == 0) {
        dataBytes = 4;
        state = RegQueryValueExA(hKey,
                             REG_MUSICVOLUME, 0, &type, (BYTE *)&musicVolume, &dataBytes);
        if (state == 0) {
            if (musicVolume < 0) {
                musicVolume = 0;
            }
            if (musicVolume > 100) {
                musicVolume = 100;
            }
            state = RegCloseKey(hKey);
        } else {
            success = 0;
        }
        RegCloseKey(hKey);
    }
    state = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_SOUND_KEY, 0, KEY_QUERY_VALUE, &hKey);
    if (state == 0) {
        dataBytes = 4;
        state = RegQueryValueExA(hKey,
                             REG_SFXVOLUME, 0, &type, (BYTE *)&sfxVolume, &dataBytes);
        if (state == 0) {
            if (sfxVolume < 0) {
                sfxVolume = 0;
            }
            if (sfxVolume > 100) {
                sfxVolume = 100;
            }
            state = RegCloseKey(hKey);
        } else {
            success = 0;
        }
        RegCloseKey(hKey);
    }
    if (success != 0) {
        Engine_ApplyMusicAndSfxVolume(musicVolume, sfxVolume);
    }
    return success;
}

/*
 * Returns the path to the main game data directory.
 */
// FUNCTION: FF7 0x004076ea
char *__cdecl GetDataPath(void)
{
    return g_InitPath_DataPath;
}

/*
 * Returns the path to the CD data directory.
 */
// FUNCTION: FF7 0x004076f4
char *__cdecl GetCdDataPath(void)
{
    return g_InitPath_CdDataPath;
}

/*
 * Returns the directory the chocobo races load from. It rebuilds the shared 256-byte buffer
 * g_InitPath_MinigameDir as the CD data path (the data path on a full install, else the data
 * drive plus "ff7/") followed by "minigame/" and returns it. The five other GetMinigameDirFor*
 * functions build the same string in the same buffer, so each call overwrites the last result
 * and callers copy it. Used for chocobo.lgp (LGP_ARCHIVE_CHOCOBO) and the chocobo asset
 * directory set at start-up.
 */
// FUNCTION: FF7 0x004076fe
char *__cdecl GetMinigameDirForChocobo(void)
{
    memset(g_InitPath_MinigameDir, 0, 256);
    strcpy(g_InitPath_MinigameDir, g_InitPath_CdDataPath);
    strcat(g_InitPath_MinigameDir,
           // STRING: FF7 0x007ba9f0
           "minigame/");
    return g_InitPath_MinigameDir;
}

/*
 * Returns the directory Fort Condor loads from: the shared buffer g_InitPath_MinigameDir, rebuilt
 * as the CD data path plus "minigame/", the same string as GetMinigameDirForChocobo. Used for
 * condor.lgp (LGP_ARCHIVE_CONDOR, BuildArchivePath) and the Fort Condor asset directory set at start-up.
 */
// FUNCTION: FF7 0x0040776f
char *__cdecl GetMinigameDirForCondor(void)
{
    memset(g_InitPath_MinigameDir, 0, 256);
    strcpy(g_InitPath_MinigameDir, g_InitPath_CdDataPath);
    strcat(g_InitPath_MinigameDir,
           // STRING: FF7 0x007ba9fc
           "minigame/");
    return g_InitPath_MinigameDir;
}

/*
 * Returns the directory the motorbike chase (highway) loads from: the shared buffer
 * g_InitPath_MinigameDir, rebuilt as the CD data path plus "minigame/", the same string as
 * GetMinigameDirForChocobo. Used for high-us.lgp (LGP_ARCHIVE_HIGHWAY, BuildArchivePath) and as the
 * highway's data directory.
 */
// FUNCTION: FF7 0x004077e0
char *__cdecl GetMinigameDirForHighway(void)
{
    memset(g_InitPath_MinigameDir, 0, 256);
    strcpy(g_InitPath_MinigameDir, g_InitPath_CdDataPath);
    strcat(g_InitPath_MinigameDir,
           // STRING: FF7 0x007baa08
           "minigame/");
    return g_InitPath_MinigameDir;
}

/*
 * Returns the directory the Speed Square roller coaster loads from: the shared buffer
 * g_InitPath_MinigameDir, rebuilt as the CD data path plus "minigame/", the same string as
 * GetMinigameDirForChocobo. Used for coaster.lgp (LGP_ARCHIVE_COASTER, BuildArchivePath); the coaster also
 * makes it the current directory while it loads.
 */
// FUNCTION: FF7 0x00407851
char *__cdecl GetMinigameDirForCoaster(void)
{
    memset(g_InitPath_MinigameDir, 0, 256);
    strcpy(g_InitPath_MinigameDir, g_InitPath_CdDataPath);
    strcat(g_InitPath_MinigameDir,
           // STRING: FF7 0x007baa14
           "minigame/");
    return g_InitPath_MinigameDir;
}

/*
 * Returns the directory the snowboard minigame loads from: the shared buffer
 * g_InitPath_MinigameDir, rebuilt as the CD data path plus "minigame/", the same string as
 * GetMinigameDirForChocobo. Used for snowboard-us.lgp (LGP_ARCHIVE_SNOWBOARD, BuildArchivePath), as the
 * directory of its load options and as the current directory while it runs.
 */
// FUNCTION: FF7 0x004078c2
char *__cdecl GetMinigameDirForSnowboard(void)
{
    memset(g_InitPath_MinigameDir, 0, 256);
    strcpy(g_InitPath_MinigameDir, g_InitPath_CdDataPath);
    strcat(g_InitPath_MinigameDir,
           // STRING: FF7 0x007baa20
           "minigame/");
    return g_InitPath_MinigameDir;
}

/*
 * Returns the directory the submarine minigame loads from: the shared buffer
 * g_InitPath_MinigameDir, rebuilt as the CD data path plus "minigame/", the same string as
 * GetMinigameDirForChocobo. Used for sub.lgp (LGP_ARCHIVE_SUBMARINE, BuildArchivePath) and copied into the
 * submarine's module state when it starts.
 */
// FUNCTION: FF7 0x00407933
char *__cdecl GetMinigameDirForSubmarine(void)
{
    memset(g_InitPath_MinigameDir, 0, 256);
    strcpy(g_InitPath_MinigameDir, g_InitPath_CdDataPath);
    strcat(g_InitPath_MinigameDir,
           // STRING: FF7 0x007baa2c
           "minigame/");
    return g_InitPath_MinigameDir;
}

/*
 * Returns the path to the MIDI directory.
 */
// FUNCTION: FF7 0x004079a4
char *__cdecl GetMidiDirPath(void)
{
    return g_InitPath_MidiPath;
}

/*
 * Returns the path to the sound directory.
 */
// FUNCTION: FF7 0x004079ae
char *__cdecl GetSoundPath(void)
{
    return g_InitPath_SoundPath;
}

/*
 * Formats full filesystem path for the specified LGP archive ID into
 * g_InitPath_ArchivePath and returns a pointer to it.
 */
// FUNCTION: FF7 0x004079b8
char *__cdecl BuildArchivePath(int archive)
{
    char *midiLgpPath;

    switch (archive) {
    case LGP_ARCHIVE_FIELD_CHAR:
        strcpy(g_InitPath_ArchivePath, g_InitPath_CdDataPath);
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007baa38
               "field/char.lgp");
        break;
    case LGP_ARCHIVE_FIELD_LEVEL:
        strcpy(g_InitPath_ArchivePath, g_InitPath_CdDataPath);
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007baa48
               "field/flevel.lgp");
        break;
    case LGP_ARCHIVE_BATTLE:
        strcpy(g_InitPath_ArchivePath, g_InitPath_DataPath);
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007baa5c
               "battle/battle.lgp");
        break;
    case LGP_ARCHIVE_MAGIC:
        strcpy(g_InitPath_ArchivePath, g_InitPath_DataPath);
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007baa70
               "battle/magic.lgp");
        break;
    case LGP_ARCHIVE_MENU:
        strcpy(g_InitPath_ArchivePath, g_InitPath_DataPath);
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007baa84
               "menu/menu_us.lgp");
        break;
    case LGP_ARCHIVE_WORLD:
        strcpy(g_InitPath_ArchivePath, g_InitPath_CdDataPath);
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007baa98
               "wm/world_us.lgp");
        break;
    case LGP_ARCHIVE_CONDOR:
        strcpy(g_InitPath_ArchivePath, GetMinigameDirForCondor());
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007baaa8
               "condor.lgp");
        break;
    case LGP_ARCHIVE_CHOCOBO:
        strcpy(g_InitPath_ArchivePath, GetMinigameDirForChocobo());
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007baab4
               "chocobo.lgp");
        break;
    case LGP_ARCHIVE_HIGHWAY:
        strcpy(g_InitPath_ArchivePath, GetMinigameDirForHighway());
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007baac0
               "high-us.lgp");
        break;
    case LGP_ARCHIVE_COASTER:
        strcpy(g_InitPath_ArchivePath, GetMinigameDirForCoaster());
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007baacc
               "coaster.lgp");
        break;
    case LGP_ARCHIVE_SNOWBOARD:
        strcpy(g_InitPath_ArchivePath, GetMinigameDirForSnowboard());
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007baad8
               "snowboard-us.lgp");
        break;
    case LGP_ARCHIVE_MIDI:
        midiLgpPath = (char *)GetMidiLgpPath();
        if (midiLgpPath != 0) {
            strcpy(g_InitPath_ArchivePath, midiLgpPath);
        }
        break;
    case LGP_ARCHIVE_MOVIE_CAMERA:
        strcpy(g_InitPath_ArchivePath, g_InitPath_DataPath);
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007baaec
               "cd/moviecam.lgp");
        break;
    case LGP_ARCHIVE_CREDITS:
        strcpy(g_InitPath_ArchivePath, g_InitPath_DataPath);
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007baafc
               "cd/cr_us.lgp");
        break;
    case LGP_ARCHIVE_DISC:
        strcpy(g_InitPath_ArchivePath, GetDataPath());
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007bab0c
               "cd/disc_us.lgp");
        break;
    case LGP_ARCHIVE_SUBMARINE:
        strcpy(g_InitPath_ArchivePath, GetMinigameDirForSubmarine());
        strcat(g_InitPath_ArchivePath,
               // STRING: FF7 0x007bab1c
               "sub.lgp");
        break;
    }
    return g_InitPath_ArchivePath;
}
} /* extern "C" */
