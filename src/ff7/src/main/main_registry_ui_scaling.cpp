// Contents: Reads the UI scaling mode from the graphics settings in the registry.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <windows.h>
extern "C" {

// GLOBAL: FF7 0x007b6448
static const char REG_GFX_KEY[] = "Software\\Square Soft, Inc.\\Final Fantasy VII\\1.00\\Graphics";
// GLOBAL: FF7 0x007b6488
static const char REG_GFX_MODE[] = "Mode";

/*
 * Reads the UI scaling mode from the registry Graphics key.
 * Returns the queried Mode value on success, or defaults to 2.
 */
// FUNCTION: FF7 0x00404d80
int __cdecl GetUiScalingMode(void)
{
    DWORD p;
    DWORD a;
    LONG r;
    HKEY c;
    int t;
    DWORD e;
#define cbData    p
#define valType   a
#define status    r
#define hKey      c
#define mode      t
#define modeValue e

    mode = 2;
    status = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                           REG_GFX_KEY, 0, KEY_READ, &hKey);
    if (status == ERROR_SUCCESS) {
        cbData = 0x100;
        status = RegQueryValueExA(hKey,
                                  REG_GFX_MODE, 0, &valType, (BYTE *)&modeValue, &cbData);
        if (status == ERROR_SUCCESS) {
            mode = modeValue;
        }
        RegCloseKey(hKey);
    }
    return mode;
#undef cbData
#undef valType
#undef status
#undef hKey
#undef mode
#undef modeValue
}
} /* extern "C" */
