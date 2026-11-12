// src/lib/src/graphics/graphics_app_config_loader.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Parses the graphics driver and display configuration tokens from the application's
//   config file.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "lib/crt.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/api/token.h"
extern "C" {
// The 17 option names the configuration parser compares against; the names are the initializer literals.
// GLOBAL: FF7 0x00912088
char *g_GfxConfigOptionNames[17] = {
    // STRING: FF7 0x009120cc
    "p",
    // STRING: FF7 0x009120d0
    "np",
    // STRING: FF7 0x009120d4
    "sw",
    // STRING: FF7 0x009120d8
    "hw",
    // STRING: FF7 0x009120dc
    "sys",
    // STRING: FF7 0x009120e0
    "full",
    // STRING: FF7 0x009120e8
    "caps",
    // STRING: FF7 0x009120f0
    "alpha",
    // STRING: FF7 0x009120f8
    "d3d2",
    // STRING: FF7 0x00912100
    "dx",
    // STRING: FF7 0x00912104
    "sw",
    // STRING: FF7 0x00912108
    "dll",
    // STRING: FF7 0x0091210c
    "gl",
    // STRING: FF7 0x00912110
    "log",
    // STRING: FF7 0x00912114
    "debug",
    // STRING: FF7 0x0091211c
    "sw_lock",
    // STRING: FF7 0x00912124
    "riva",
};

/*
 * Reads the display size, window/driver mode and debug options from the configuration file named by
 * fileName into config.
 */
// FUNCTION: FF7 0x0069b660
void __cdecl Gfx_LoadAppConfigFile(char *fileName, GfxEngineContext *config)
{
    struct PolygonTokenFile *subFile;
    char *text;
    GfxDriverLoadSpec information;
    int baseVal;
    struct PolygonTokenFile *tokens;
    int firstNumber;

    if (config != 0) {
        tokens = TokenizeTextFileFromDisk(0, fileName);
        if (tokens != 0) {
            text = TokenAsString(0, tokens);
            if (*text != 0) {
                subFile = TokenizeTextFileFromDisk(0, text);
                if (subFile != 0) {
                    text = TokenAsString(0, subFile);
                    if (*text != 0) {
                        firstNumber = atoi(text);
                        config->displayWidth = firstNumber;
                    }
                    text = TokenAsString(1, subFile);
                    if (*text != 0) {
                        baseVal = atoi(text);
                        config->displayHeight = baseVal;
                    }
                    text = TokenAsString(2, subFile);
                    if (*text != 0) {
                        if (_stricmp(text, g_GfxConfigOptionNames[0]) == 0) {
                            config->modeActive = 1;
                        }
                        if (_stricmp(text, g_GfxConfigOptionNames[1]) == 0) {
                            config->modeActive = 0;
                        }
                    }
                    text = TokenAsString(3, subFile);
                    if (*text != 0) {
                        if (_stricmp(text, g_GfxConfigOptionNames[2]) == 0) {
                            config->modeSetupDone = 1;
                        }
                        if (_stricmp(text, g_GfxConfigOptionNames[3]) == 0) {
                            config->modeSetupDone = 0;
                        }
                    }
                    config->modeSetupDone = 0;
                    text = TokenAsString(4, subFile);
                    if (*text != 0) {
                        if (_stricmp(text, g_GfxConfigOptionNames[4]) == 0) {
                            config->modeTransition = 1;
                        } else {
                            config->modeTransition = 0;
                        }
                    }
                    text = TokenAsString(5, subFile);
                    if (*text != 0) {
                        if (_stricmp(text, g_GfxConfigOptionNames[5]) == 0) {
                            config->fullScreen = 1;
                        } else {
                            config->fullScreen = 0;
                        }
                    }
                    text = TokenAsString(6, subFile);
                    if (*text != 0) {
                        if (_stricmp(text, g_GfxConfigOptionNames[6]) == 0) {
                            config->deviceIndex = 1;
                        } else {
                            config->deviceIndex = 0;
                        }
                    }
                    text = TokenAsString(7, subFile);
                    if (*text != 0) {
                        if (_stricmp(text, g_GfxConfigOptionNames[7]) == 0) {
                            config->alphaOption = 1;
                        } else {
                            config->alphaOption = 0;
                        }
                    }
                    text = TokenAsString(8, subFile);
                    if (*text != 0) {
                        if (_stricmp(text, g_GfxConfigOptionNames[8]) == 0) {
                            config->useDirect3D5 = 1;
                        } else {
                            config->useDirect3D5 = 0;
                        }
                    }
                    text = TokenAsString(9, subFile);
                    if (*text != 0) {
                        if (_stricmp(text, g_GfxConfigOptionNames[9]) == 0) {
                            config->driverType = GFX_DRIVER_DIRECTX;
                        }
                        if (_stricmp(text, g_GfxConfigOptionNames[10]) == 0) {
                            config->driverType = GFX_DRIVER_SOFTWARE;
                        }
                        if (_stricmp(text, g_GfxConfigOptionNames[11]) == 0) {
                            text = TokenAsString(13, subFile);
                            if (*text != 0) {
                                config->driverType = GFX_DRIVER_DLL;
                                config->driverName = (int)GameHeapStrDup(text);
                                InitGraphicsDriverDesc(GFX_DRIVER_DLL, (GfxDriverDesc *)&information);
                                information.libraryName = (void *)config->driverName;
                                if (InstallGraphicsDriver(&information, config) != 0) { /* empty first arm: lib/codegen.h */
                                } else {
                                    config->driverType = GFX_DRIVER_SOFTWARE;
                                }
                            }
                        }
                        if (_stricmp(text, g_GfxConfigOptionNames[12]) == 0) {
                            config->driverType = GFX_DRIVER_OPENGL;
                        }
                    }
                    text = TokenAsString(10, subFile);
                    if (*text != 0) {
                        if (_stricmp(text, g_GfxConfigOptionNames[13]) == 0) {
                            config->activeB = 1;
                        } else {
                            config->activeB = 0;
                        }
                    }
                    text = TokenAsString(11, subFile);
                    if (*text != 0) {
                        if (_stricmp(text, g_GfxConfigOptionNames[14]) == 0) {
                            config->activeA = 1;
                        } else {
                            config->activeA = 0;
                        }
                    }
                    text = TokenAsString(12, subFile);
                    if (*text != 0) {
                        if (_stricmp(text, g_GfxConfigOptionNames[15]) == 0) {
                            config->configPending = 1;
                        } else {
                            config->configPending = 0;
                        }
                    }
                    config->fallbackFlag = 0;
                    text = TokenAsString(14, subFile);
                    if (*text != 0) {
                        if (_stricmp(text, g_GfxConfigOptionNames[16]) == 0) {
                            config->fallbackFlag = 1;
                        } else {
                            config->fallbackFlag = 0;
                        }
                    }
                    FreeTokenFile(subFile);
                }
            }
            FreeTokenFile(tokens);
        }
    }
}
} /* extern "C" */
