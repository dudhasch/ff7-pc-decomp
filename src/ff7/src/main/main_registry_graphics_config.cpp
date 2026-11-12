// src/ff7/src/main/main_registry_graphics_config.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Reads the debug log flag and the graphics settings from the registry, such as the
//   device GUID, driver, mode and options. It then applies those settings to the engine context.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include "lib/pfile.h"
#include "ff7/api/main.h"
#include "ff7/types/main.h"
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/enums/graphics.h"
#include "ff7/enums/main.h"
extern "C" {

// GLOBAL: FF7 0x007b64a0
static const char REG_FF7[] = "Software\\Square Soft, Inc.\\Final Fantasy VII";
// GLOBAL: FF7 0x007b64d0
static const char REG_GRAPHICS[] = "Software\\Square Soft, Inc.\\Final Fantasy VII\\1.00\\Graphics";
// GLOBAL: FF7 0x007b6510
static const char REG_DD_GUID[] = "DD_GUID";
// GLOBAL: FF7 0x007b6518
static const char REG_DRIVER[] = "Driver";
// GLOBAL: FF7 0x007b6520
static const char REG_DRIVERPATH[] = "DriverPath";
// GLOBAL: FF7 0x007b6530
static const char REG_MODE[] = "Mode";
// GLOBAL: FF7 0x007b6538
static const char REG_OPTIONS[] = "Options";
// GLOBAL: FF7 0x007b6540
static const char REG_SSI_DEBUG[] = "SSI_DEBUG";
#pragma intrinsic(strcmp, memcmp, memset, strcpy, strcat)

// The SSI_DEBUG value that enables the application log: a 16-byte array, so the TU's .data is 8-aligned
// and LINK pads 0x7BA734 after main_game_over_screen.cpp's literals, as in retail.
// GLOBAL: FF7 0x007ba738
static char SHOWMETHEAPPLOG[] = "SHOWMETHEAPPLOG";

/*
 * Reads the SSI_DEBUG registry value to check if the application log is enabled.
 * Returns 1 if SSI_DEBUG equals SHOWMETHEAPPLOG (case-insensitive), 0 otherwise.
 */
// FUNCTION: FF7 0x004067a0
int __cdecl RegReadShowMeTheAppLogFlag(void)
{
    struct {
        char value[256];
        DWORD type;
        int result;
        DWORD size;
        HKEY key;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.result = 0;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, REG_FF7, 0, KEY_QUERY_VALUE, &f.key) == 0) {
        f.size = 16;
        if (RegQueryValueExA(f.key, REG_SSI_DEBUG, 0, &f.type, (unsigned char *)f.value, &f.size) == 0) {
            if (f.size == 16) {
                if (_strcmpi(SHOWMETHEAPPLOG, f.value) == 0)
                    f.result = 1;
            }
        }
        RegCloseKey(f.key);
    }
    return f.result;
}

/*
 * Queries graphics settings (DD_GUID, Driver, DriverPath, Mode, Options) from the registry.
 * Populates the provided configuration buffer and returns 1 on success, 0 on failure.
 */
// FUNCTION: FF7 0x0040682d
int __cdecl ReadGraphicsRegistryConfig(MainGraphicsConfig *config)
{
    DWORD p;
    HKEY a;
    DWORD r;
    int c;
    char t[0x104];
#define cbData     p
#define hKey       a
#define valType    r
#define success    c
#define driverPath t

    success = 1;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_GRAPHICS, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
        cbData = 0x10;
        if (RegQueryValueExA(hKey,
                             REG_DD_GUID, 0, &valType, (BYTE *)config->ddGuid, &cbData) == ERROR_SUCCESS) {
            int curZeroBlockPack[4];

            memset(curZeroBlockPack, 0, 0x10);
            if (memcmp(config->ddGuid, curZeroBlockPack, 0x10) != 0) {
                config->isEnabled = 1;
            } else {
                config->isEnabled = 0;
            }
        } else {
            success = 0;
        }
        cbData = 4;
        if (RegQueryValueExA(hKey,
                             REG_DRIVER, 0, &valType, (BYTE *)&config->driver, &cbData) != ERROR_SUCCESS) {
            success = 0;
        }
        if (config->driver >= MAIN_REGISTRY_DRIVER_DLL) {
            cbData = 0x104;
            if (RegQueryValueExA(hKey,
                                 REG_DRIVERPATH, 0, &valType, (BYTE *)driverPath, &cbData) != ERROR_SUCCESS) {
                success = 0;
                config->driverDllPath[0] = 0;
            } else {
                strcpy(config->driverDllPath, driverPath);
            }
        }
        cbData = 4;
        if (RegQueryValueExA(hKey,
                             REG_MODE, 0, &valType, (BYTE *)&config->mode, &cbData) != ERROR_SUCCESS) {
            success = 0;
        }
        cbData = 4;
        if (RegQueryValueExA(hKey,
                             REG_OPTIONS, 0, &valType, (BYTE *)&config->options, &cbData) != ERROR_SUCCESS) {
            success = 0;
        }
        RegCloseKey(hKey);
    } else {
        success = 0;
    }
    return success;
#undef cbData
#undef hKey
#undef valType
#undef success
#undef driverPath
}

/*
 * Reads graphics configuration from the registry and applies settings to the engine context.
 * Sets resolution, driver index, full-screen/windowed mode, and debug logging flags.
 */
// FUNCTION: FF7 0x004069fd
int __cdecl ApplyConfigToContext(GfxEngineContext *ctx)
{
    MainGraphicsConfig p;
    int a;
    int r;
    unsigned char c[0x24];
#define regConfig      p
#define configSuccess  a
#define logFlag        r
#define driverDesc     c

    configSuccess = ReadGraphicsRegistryConfig(&regConfig);
    if (configSuccess != 0) {
        switch (regConfig.mode) {
        case MAIN_GRAPHICS_MODE_320X240:
            ctx->displayWidth = 0x140;
            ctx->displayHeight = 0xF0;
            g_CdCheckDisplayMode = MAIN_GRAPHICS_MODE_320X240;
            break;
        case MAIN_GRAPHICS_MODE_320X240_CENTERED:
            ctx->displayWidth = 0x280;
            ctx->displayHeight = 0x1E0;
            g_CdCheckDisplayMode = MAIN_GRAPHICS_MODE_320X240_CENTERED;
            break;
        case MAIN_GRAPHICS_MODE_640X480:
            ctx->displayWidth = 0x280;
            ctx->displayHeight = 0x1E0;
            g_CdCheckDisplayMode = MAIN_GRAPHICS_MODE_640X480;
            break;
        }
        switch (regConfig.isEnabled) {
        case REG_MODE_ENABLED:
            ctx->modeActive = 1;
            break;
        case REG_MODE_DISABLED:
            ctx->modeActive = 0;
            break;
        }
        ctx->modeSetupDone = 0;
        if ((regConfig.options & 1) != 0) {
            ctx->modeTransition = 1;
        } else {
            ctx->modeTransition = 0;
        }
        ctx->configApplied = 0;
        if ((regConfig.options & 2) != 0) {
            ctx->fallbackFlag = 1;
            if ((regConfig.options & 8) != 0) {
                ctx->fallbackUsed = 0;
            }
            if ((regConfig.options & 0x10) != 0) {
                ctx->fallbackUsed = 0;
                ctx->configApplied = 1;
            }
        } else {
            ctx->fallbackFlag = 0;
        }
        ctx->fullScreen = 1;
        ctx->deviceIndex = 0;
        ctx->alphaOption = 1;
        ctx->useDirect3D5 = 1;
        switch (regConfig.driver) {
        case MAIN_REGISTRY_DRIVER_SOFTWARE:
            ctx->driverType = GFX_DRIVER_SOFTWARE;
            break;
        case MAIN_REGISTRY_DRIVER_DIRECT3D:
            ctx->driverType = GFX_DRIVER_DIRECTX;
            break;
        case MAIN_REGISTRY_DRIVER_OPENGL:
            ctx->driverType = GFX_DRIVER_OPENGL;
            break;
        case MAIN_REGISTRY_DRIVER_DLL:
            ctx->driverType = GFX_DRIVER_DLL;
            ctx->driverName = (int)GameHeapStrDup(regConfig.driverDllPath);
            InitGraphicsDriverDesc(GFX_DRIVER_DLL, (GfxDriverDesc *)driverDesc);
            ((GfxDriverLoadSpec *)driverDesc)->libraryName = (void *)ctx->driverName;
            if (InstallGraphicsDriver((GfxDriverLoadSpec *)driverDesc, ctx) != 0) { /* empty first arm: lib/codegen.h */
            } else {
                ctx->driverType = GFX_DRIVER_SOFTWARE;
            }
            break;
        default:
            ctx->driverType = GFX_DRIVER_SOFTWARE;
            break;
        }
        logFlag = RegReadShowMeTheAppLogFlag();
        ctx->activeB = logFlag;
        ctx->activeA = logFlag;
        ctx->configPending = 1;
    }
    return configSuccess;
#undef regConfig
#undef configSuccess
#undef logFlag
#undef driverDesc
}
} /* extern "C" */
