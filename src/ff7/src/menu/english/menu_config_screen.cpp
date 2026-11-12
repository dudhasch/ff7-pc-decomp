// src/ff7/src/menu/english/menu_config_screen.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Config screen: its layout at both UI scales, the volume sliders and how they are
//   applied to music and sound, saving the volumes and input configuration, and the four-corner
//   window colour editor.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/menu.h"
#include "lib/types/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "ff7/api/main.h"
#include "lib/api/file.h"
#include "lib/api/sound.h"
#include "lib/crt.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include "lib/codegen.h"
#include "ff7/savemap.h"
#include "ff7/enums/menu.h"
extern "C" {

// clang-format off
// clang-format on

struct EnglishWindowFrame;

// clang-format off
// Text: "Window color" "Sound" "Controller" "Cursor" "ATB" "Battle speed" "Battle message"
//       "Field message" "Camera angle" "Select" "Cancel" "Menu" "Window OFF" "Pause" "Battle help"
//       "Mono" "Stereo" "Wide" "Normal" "Customize" "Initial" "Memory" "Active" "Recommended"
// 0x30-byte label records. The draw code indexes on past the 24 declared here into the tables
// that follow: records 0x18-0x1B are the ATB labels, 0x1C "Slow", 0x1D "Fast" and 0x21-0x25 the
// magic-order labels.
// GLOBAL: FF7 0x00919ea8
unsigned char g_MenuConfigOptionLabels[24][0x30] = {0x37, 0x49, 0x4e, 0x44, 0x4f, 0x57, 0x00, 0x43, 0x4f, 0x4c, 0x4f, 0x52, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x4f, 0x55, 0x4e, 0x44, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x4f, 0x4e, 0x54, 0x52, 0x4f, 0x4c, 0x4c, 0x45, 0x52, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x55, 0x52, 0x53, 0x4f, 0x52, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x34, 0x22, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x41, 0x54, 0x54, 0x4c, 0x45, 0x00, 0x53, 0x50, 0x45, 0x45, 0x44, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x41, 0x54, 0x54, 0x4c, 0x45, 0x00, 0x4d, 0x45, 0x53, 0x53, 0x41, 0x47, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x49, 0x45, 0x4c, 0x44, 0x00, 0x4d, 0x45, 0x53, 0x53, 0x41, 0x47, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x41, 0x4d, 0x45, 0x52, 0x41, 0x00, 0x41, 0x4e, 0x47, 0x4c, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x45, 0x4c, 0x45, 0x43, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x41, 0x4e, 0x43, 0x45, 0x4c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x45, 0x4e, 0x55, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x49, 0x4e, 0x44, 0x4f, 0x57, 0x00, 0x2f, 0x26, 0x26, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x41, 0x55, 0x53, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x41, 0x54, 0x54, 0x4c, 0x45, 0x00, 0x48, 0x45, 0x4c, 0x50, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x4f, 0x4e, 0x4f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x54, 0x45, 0x52, 0x45, 0x4f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x49, 0x44, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x4f, 0x52, 0x4d, 0x41, 0x4c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x55, 0x53, 0x54, 0x4f, 0x4d, 0x49, 0x5a, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x4e, 0x49, 0x54, 0x49, 0x41, 0x4c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x45, 0x4d, 0x4f, 0x52, 0x59, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x43, 0x54, 0x49, 0x56, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x45, 0x43, 0x4f, 0x4d, 0x4d, 0x45, 0x4e, 0x44, 0x45, 0x44, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Text: "Wait" "Auto" "Fixed"
// GLOBAL: FF7 0x0091a328
unsigned char g_MenuConfigAtbModeLabels[192] = {0x37, 0x41, 0x49, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x55, 0x54, 0x4f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x49, 0x58, 0x45, 0x44, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Text: "Slow", for the three speed sliders.
// GLOBAL: FF7 0x0091a3e8
unsigned char g_MenuConfigSlowLabel[48] = {0x33, 0x4c, 0x4f, 0x57, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Text: "Fast" (for the three speed sliders) "RED" "GREEN" "BLUE"
// GLOBAL: FF7 0x0091a418
unsigned char g_MenuConfigFastLabels[192] = {0x26, 0x41, 0x53, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x25, 0x24, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x32, 0x25, 0x25, 0x2e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x2c, 0x35, 0x25, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Text: "Magic order" "restore" "attack" "indirect" "forbidden"
// GLOBAL: FF7 0x0091a4d8
unsigned char g_MenuConfigMagicOrderLabels[240] = {0x2d, 0x41, 0x47, 0x49, 0x43, 0x00, 0x4f, 0x52, 0x44, 0x45, 0x52, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 0x45, 0x53, 0x54, 0x4f, 0x52, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x54, 0x54, 0x41, 0x43, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x4e, 0x44, 0x49, 0x52, 0x45, 0x43, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x4f, 0x52, 0x42, 0x49, 0x44, 0x44, 0x45, 0x4e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Text: "No." "Left2" "Right2" "Left1" "Right1" "Menu" "Select"
// GLOBAL: FF7 0x0091a5c8
unsigned char g_MenuConfigControlsLabels[292] = {0x2e, 0x4f, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2c, 0x45, 0x46, 0x54, 0x12, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x49, 0x47, 0x48, 0x54, 0x12, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2c, 0x45, 0x46, 0x54, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x49, 0x47, 0x48, 0x54, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x45, 0x4e, 0x55, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x45, 0x4c, 0x45};
unsigned char g_rawData_0091a6ec[4] = {0x43, 0x54, 0xff, 0x00};
// Text: (+0x28) "Cancel"
unsigned char g_rawData_0091a6f0[84] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x41, 0x4e, 0x43, 0x45, 0x4c, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// Text: "EXT"
unsigned char g_rawData_0091a748[48] = {
    0x25, 0x38, 0x34, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// Text: "Help"
unsigned char g_rawData_0091a778[44] = {
    0x28, 0x45, 0x4c, 0x50, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// Text: "Press “START” to customize." (+0x30) "Press Directional button Left or Right to end."
unsigned char g_rawData_0091a7a8[92] = {
    0x30, 0x52, 0x45, 0x53, 0x53, 0x00, 0xb2, 0x33, 0x34, 0x21, 0x32, 0x34, 0xb3, 0x00, 0x54, 0x4f, 0x00, 0x43, 0x55, 0x53, 0x54, 0x4f, 0x4d, 0x49,
    0x5a, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x30, 0x52, 0x45, 0x53, 0x53, 0x00, 0x24, 0x49, 0x52, 0x45, 0x43, 0x54, 0x49, 0x4f, 0x4e, 0x41, 0x4c, 0x00, 0x42, 0x55, 0x54, 0x54, 0x4f, 0x4e,
    0x00, 0x2c, 0x45, 0x46, 0x54, 0x00, 0x4f, 0x52, 0x00, 0x32, 0x49, 0x47, 0x48, 0x54, 0x00, 0x54, 0x4f, 0x00, 0x45, 0x4e
};
unsigned char g_rawData_0091a804[4] = {0x44, 0x0e, 0xff, 0x00};
// Text: "Pause"
unsigned char g_rawData_0091a808[48] = {
    0x30, 0x41, 0x55, 0x53, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// "red", "green" and "blue" in the icon font, 6 bytes each; the colour editor draws each one's
// first glyph beside its slider.
// GLOBAL: FF7 0x0091a838
unsigned char g_MenuConfigColorChannelNames[3][6] = {{0xc5, 0xb8, 0xb7, 0xff, 0x00, 0x00}, {0xba, 0xc5, 0xb8, 0xb8, 0xc1, 0xff}, {0xb5, 0xbf, 0xc8, 0xb8, 0xff, 0x00}};
// GLOBAL: FF7 0x0091a850
EnglishWindowFrame g_MenuConfigFrames2x[2] = {{0, 0, 640, 51}, {0, 51, 640, 429}};
// GLOBAL: FF7 0x0091a860
EnglishWindowFrame g_MenuConfigFrames1x[2] = {{0, 0, 320, 26}, {0, 26, 320, 214}};
// For each of the six magic orders, the three label records (counted from record 0x22, "restore")
// in the order that setting lists them.
// GLOBAL: FF7 0x0091a870
char g_MenuMagicOrderLabelSlots[20] = {0x00, 0x01, 0x02, 0x00, 0x02, 0x01, 0x01, 0x02, 0x00, 0x01, 0x00, 0x02, 0x02, 0x00, 0x01, 0x02, 0x01, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0091a884
int g_MenuConfigState = (-1);
// Two strings: the volume panel's header "Set Sound & Music Volume" (25 bytes) and, at 0x19,
// "Not supported.", drawn in place of the music slider; 16 zero bytes follow.
// GLOBAL: FF7 0x0091a888
unsigned char g_MenuSoundVolumeHeaderText[56] = {0x33, 0x45, 0x54, 0x00, 0x33, 0x4f, 0x55, 0x4e, 0x44, 0x00, 0x06, 0x00, 0x2d, 0x55, 0x53, 0x49, 0x43, 0x00, 0x36, 0x4f, 0x4c, 0x55, 0x4d, 0x45, 0xff, 0x2e, 0x4f, 0x54, 0x00, 0x53, 0x55, 0x50, 0x50, 0x4f, 0x52, 0x54, 0x45, 0x44, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00dc1088
int g_MenuConfigVolumeCursor = 0;
// GLOBAL: FF7 0x00dc108c
int g_MenuConfigVolumeCursorRow = 0;
unsigned char g_bssPad_00dc1090[48] = {0};
// GLOBAL: FF7 0x00dc10c0
struct EnglishWindowFrame *g_MenuConfigFrames = 0;
// GLOBAL: FF7 0x00dc10c4
int g_MenuConfigClearedWord = 0;
// GLOBAL: FF7 0x00dc10c8
int g_MenuWindowColorColumn = 0;
// GLOBAL: FF7 0x00dc10cc
int g_MenuConfigEditorMode = 0;
// GLOBAL: FF7 0x00dc10d0
int g_MenuMusicVolume = 0;
// GLOBAL: FF7 0x00dc10d4
int g_MenuWindowColorRow = 0;
// g_MenuConfigWindowColors is indexed by the code as an array (read by Menu_ConfigScreenInit,
// MenuConfig_RunWindowColorEditor). The span runs to the next named definition of the TU; the TU's
// pieces inside 0xdc10d8..0xdc10e8 are aliases.
// GLOBAL: FF7 0x00dc10d8
unsigned char g_MenuConfigWindowColors[0x10] = {0};
// GLOBAL: FF7 0x00dc10e8
int g_MenuSfxVolume = 0;
// GLOBAL: FF7 0x00dc10ec
int g_MenuColorEditorCursor = 0;
// GLOBAL: FF7 0x00dc10f0
int g_MenuConfigCursor = 0;
// GLOBAL: FF7 0x00dc10f4
int g_MenuMidiVolumeControlFlag = 0;
unsigned char g_bssPad_00dc10f8[4] = {0};
// GLOBAL: FF7 0x00dc10fc
int g_MenuConfigEditorActive = 0;
// clang-format on

/*
 * Initialises the Config screen: a cursor over 2 x 2 entries at g_MenuConfigVolumeCursor, the
 * four colour words at g_MenuConfigWindowColors from the defaults at g_MenuCornerColorTL, the window
 * frame table pointer g_MenuConfigFrames for 1x or 2x scaling, the music-available flag
 * g_MenuMidiVolumeControlFlag from MidiGetVolumeControlFlag with the music slider from the master
 * volume (or 100 when there is no music), and the sound slider from the sound
 * master volume.
 */
// FUNCTION: FF7 0x006c1f60
void __cdecl Menu_ConfigScreenInit(void)
{
    int i;

    MenuCursor_Init((MenuCursor *)&g_MenuConfigVolumeCursor, 0, 0, 1, 2, 0, 0, 1, 2, 0, 0, 0, 1, 0, 0);
    g_MenuConfigState = MENU_CONFIG_STATE_BROWSE;
    g_MenuConfigCursor = 0;
    for (i = 0; i < 4; i++) {
        ((int *)g_MenuConfigWindowColors)[i] = ((int *)&g_MenuCornerColorTL)[i];
    }
    if (g_MenuIsSmallViewport320x240 != 1) {
        g_MenuConfigFrames = g_MenuConfigFrames2x;
    } else {
        g_MenuConfigFrames = g_MenuConfigFrames1x;
    }
    g_MenuMidiVolumeControlFlag = MidiGetVolumeControlFlag();
    if (g_MenuMidiVolumeControlFlag != 0) {
        g_MenuMusicVolume = MusicGetMasterVolumePercent();
    } else {
        g_MenuMusicVolume = 0x64;
    }
    g_MenuSfxVolume = SfxGetMasterVolumePercent();
}

/*
 * Draws and updates the configuration screen in either 1x or 2x layout. Dispatches
 * the window- color editor, input-binding screen or music/SFX volume panel by
 * substate; ordinary substates draw configuration rows and a preview with
 * saved/restored window colors.
 */
// FUNCTION: FF7 0x006c2025
void __cdecl Menu_ConfigScreenFrame(int flags)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[36];
        int inputExitFlag;
        struct { unsigned char pad_to_4[0x4]; int optionValue; };
        struct { unsigned char pad_to_8[0x8]; int rowY; };
        struct { unsigned char pad_to_C[0xC]; int valueX; };
        struct { unsigned char pad_to_10[0x10]; int windowIndex; };
        struct { unsigned char pad_to_14[0x14]; EnglishWindowFrame windowRect; };
        struct { unsigned char pad_to_1C[0x1C]; EnglishWindowFrame box; };
    } frame;
#pragma pack(pop)

    if (g_MenuIsSmallViewport320x240 != 1) {
        Menu_DrawCommandWindow2x(g_MenuModuleId);
        switch (g_MenuConfigState) {
        case MENU_CONFIG_STATE_BROWSE:
            Menu_DrawEncodedString2x(0x10, 0xD, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, g_MenuConfigCursor + 0x37, 8), 7, 0.1f);
            Menu_DrawMoreTextArrow2x(6, g_MenuConfigCursor * 0x28 + g_MenuConfigFrames[1].y + 0x20, 0.1f);
            break;
        case MENU_CONFIG_STATE_WINDOW_COLOR_CORNER:
        case MENU_CONFIG_STATE_WINDOW_COLOR_EDIT:
            if ((flags & 2) != 0) {
                Menu_DrawMoreTextArrow2x(6, g_MenuConfigCursor * 0x28 + g_MenuConfigFrames[1].y + 0x20, 0.1f);
            }
            MenuConfig_RunWindowColorEditor(flags);
            break;
        case MENU_CONFIG_STATE_CONTROLLER:
            frame.inputExitFlag = 0;
            InputConfig_ScreenFrame(&frame.inputExitFlag);
            break;
        case MENU_CONFIG_STATE_VOLUME:
            if ((flags & 2) != 0) {
                Menu_DrawMoreTextArrow2x(6, g_MenuConfigCursor * 0x28 + g_MenuConfigFrames[1].y + 0x20, 0.1f);
            }
            frame.valueX = g_MenuConfigFrames[1].x + 0x115;
            frame.rowY = g_MenuConfigFrames[1].y + 0x4B;
            Menu_SetRectShorts(&frame.windowRect.x, g_MenuConfigFrames[1].x + 0x108, g_MenuConfigFrames[1].y + 0x3E, 0x13A, 0x5C);
            Menu_DrawWindowFrame2x(&frame.windowRect, 0.01f);
            if (g_MenuMidiVolumeControlFlag != 0) {
                frame.box.x = 0x162;
                frame.box.y = *(short *)&frame.rowY;
                frame.box.width = 0xD6;
                frame.box.height = 0x16;
                Menu_DrawEtchedBox2x(&frame.box, 0.001f);
                frame.box.x = g_MenuMusicVolume * 2 + 0x162;
                frame.box.y = *(short *)&frame.rowY;
                frame.box.width = 0xD;
                frame.box.height = 0x16;
                Menu_DrawRaisedBox2x(&frame.box.x, 0.0001f);
                Menu_DrawDecimalNumberZeroPad2x(frame.valueX + 0x24, frame.rowY + 4, g_MenuMusicVolume, 3, 7, 0);
            } else {
                // No music volume control: "Not supported." in place of the music slider.
                Menu_DrawEncodedString2x(0x162, frame.rowY, &g_MenuSoundVolumeHeaderText[0x19], 0, 0);
            }
            frame.box.x = 0x162;
            frame.box.y = frame.rowY + 0x26;
            frame.box.width = 0xD6;
            frame.box.height = 0x16;
            Menu_DrawEtchedBox2x(&frame.box, 0.001f);
            frame.box.x = g_MenuSfxVolume * 2 + 0x162;
            frame.box.y = frame.rowY + 0x26;
            frame.box.width = 0xD;
            frame.box.height = 0x16;
            Menu_DrawRaisedBox2x(&frame.box.x, 0.0001f);
            Menu_DrawDecimalNumberZeroPad2x(frame.valueX + 0x24, frame.rowY + 0x2A, g_MenuSfxVolume, 3, 7, 0);
            Menu_DrawAtlasQuad2x(g_MenuConfigFrames[1].x + 0x110, frame.rowY - 0xA, 0x32, 0x40, 0x10, 0x10, 0xB, 0, 0, 0);
            Menu_DrawAtlasQuad2x(g_MenuConfigFrames[1].x + 0x110, frame.rowY + 0x1A, 0x42, 0x40, 0x10, 0x10, 0xB, 0, 0, 0);
            Menu_DrawMoreTextArrow2x(g_MenuConfigFrames[1].x + 0xE0, frame.rowY + g_MenuConfigVolumeCursorRow * 0x26, 0);
        }
        if (g_MenuConfigState != MENU_CONFIG_STATE_CONTROLLER) {
            MenuConfig_DrawRows2x();
            Menu_SaveWindowColors();
            Menu_SetWindowColors((int *)g_MenuConfigWindowColors);
            Menu_SetRectShorts(&frame.windowRect.x, g_MenuConfigFrames[1].x + 0x108, g_MenuConfigFrames[1].y + 9, 0x66, 0x33);
            Menu_DrawWindowFrame2x(&frame.windowRect, 0.1f);
            Menu_RestoreWindowColors();
            for (frame.windowIndex = 0; frame.windowIndex < 2; frame.windowIndex++) {
                Menu_DrawWindowFrame2x(&g_MenuConfigFrames[frame.windowIndex], 0.2f);
            }
        } else if (frame.inputExitFlag != 0) {
            g_MenuConfigState = MENU_CONFIG_STATE_BROWSE;
        }
    } else {
        Menu_DrawCommandWindow1x(g_MenuModuleId);
        switch (g_MenuConfigState) {
        case MENU_CONFIG_STATE_BROWSE:
            Menu_DrawEncodedString1x(8, 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, g_MenuConfigCursor + 0x37, 8), 7, 0.1f);
            Menu_DrawMoreTextArrow1x(3, g_MenuConfigCursor * 0x14 + g_MenuConfigFrames[1].y + 0x10, 0.1f);
            break;
        case MENU_CONFIG_STATE_WINDOW_COLOR_CORNER:
        case MENU_CONFIG_STATE_WINDOW_COLOR_EDIT:
            if ((flags & 2) != 0) {
                Menu_DrawMoreTextArrow1x(3, g_MenuConfigCursor * 0x14 + g_MenuConfigFrames[1].y + 0x10, 0.1f);
            }
            MenuConfig_RunWindowColorEditor(flags);
            break;
        case MENU_CONFIG_STATE_CONTROLLER:
            frame.inputExitFlag = 0;
            InputConfig_ScreenFrame(&frame.inputExitFlag);
            break;
        case MENU_CONFIG_STATE_VOLUME:
            if ((flags & 2) != 0) {
                Menu_DrawMoreTextArrow1x(3, g_MenuConfigCursor * 0x14 + g_MenuConfigFrames[1].y + 0x10, 0.1f);
            }
            frame.valueX = g_MenuConfigFrames[1].x + 0x8B;
            frame.rowY = g_MenuConfigFrames[1].y + 0x26;
            Menu_SetRectShorts(&frame.windowRect.x, g_MenuConfigFrames[1].x + 0x84, g_MenuConfigFrames[1].y + 0x1F, 0x9D, 0x2E);
            Menu_DrawWindowFrame1x(&frame.windowRect, 0.01f);
            if (g_MenuMidiVolumeControlFlag != 0) {
                frame.box.x = 0xB1;
                frame.box.y = *(short *)&frame.rowY;
                frame.box.width = 0x6B;
                frame.box.height = 0xB;
                Menu_DrawEtchedBox1x(&frame.box, 0.001f);
                frame.box.x = g_MenuMusicVolume + 0xB1;
                frame.box.y = *(short *)&frame.rowY;
                frame.box.width = 7;
                frame.box.height = 0xB;
                Menu_DrawRaisedBox1x(&frame.box.x, 0.0001f);
                Menu_DrawDecimalNumberZeroPad1x(frame.valueX + 0x12, frame.rowY + 2, g_MenuMusicVolume, 3, 7, 0);
            } else {
                // No music volume control: "Not supported." in place of the music slider.
                Menu_DrawEncodedString1x(0xB1, frame.rowY, &g_MenuSoundVolumeHeaderText[0x19], 0, 0);
            }
            frame.box.x = 0xB1;
            frame.box.y = frame.rowY + 0x13;
            frame.box.width = 0x6B;
            frame.box.height = 0xB;
            Menu_DrawEtchedBox1x(&frame.box, 0.001f);
            frame.box.x = g_MenuSfxVolume + 0xB1;
            frame.box.y = frame.rowY + 0x13;
            frame.box.width = 7;
            frame.box.height = 0xB;
            Menu_DrawRaisedBox1x(&frame.box.x, 0.0001f);
            Menu_DrawDecimalNumberZeroPad1x(frame.valueX + 0x12, frame.rowY + 0x15, g_MenuSfxVolume, 3, 7, 0);
            Menu_DrawAtlasQuad1x(g_MenuConfigFrames[1].x + 0x88, frame.rowY - 5, 0x32, 0x40, 0x10, 0x10, 0xB, 0, 0, 0);
            Menu_DrawAtlasQuad1x(g_MenuConfigFrames[1].x + 0x88, frame.rowY + 0xD, 0x42, 0x40, 0x10, 0x10, 0xB, 0, 0, 0);
            Menu_DrawMoreTextArrow1x(g_MenuConfigFrames[1].x + 0x70, frame.rowY + g_MenuConfigVolumeCursorRow * 0x13, 0);
        }
        if (g_MenuConfigState != MENU_CONFIG_STATE_CONTROLLER) {
            MenuConfig_DrawRows1x();
            Menu_SaveWindowColors();
            Menu_SetWindowColors((int *)g_MenuConfigWindowColors);
            Menu_SetRectShorts(&frame.windowRect.x, g_MenuConfigFrames[1].x + 0x84, g_MenuConfigFrames[1].y + 5, 0x33, 0x1A);
            Menu_DrawWindowFrame1x(&frame.windowRect, 0.1f);
            Menu_RestoreWindowColors();
            for (frame.windowIndex = 0; frame.windowIndex < 2; frame.windowIndex++) {
                Menu_DrawWindowFrame1x(&g_MenuConfigFrames[frame.windowIndex], 0.2f);
            }
        } else if (frame.inputExitFlag != 0) {
            g_MenuConfigState = MENU_CONFIG_STATE_BROWSE;
        }
    }
    if (Menu_GetPhase() == 0) {
        switch (g_MenuConfigState) {
        case MENU_CONFIG_STATE_BROWSE:
            if (MenuCursor_ReadRepeat(INPUT_UP, MOUSE_DIR_UP) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_MenuConfigCursor--;
                if (g_MenuConfigCursor < 0) {
                    g_MenuConfigCursor = MENU_CONFIG_ROW_MAGIC_ORDER;
                }
            } else if (MenuCursor_ReadRepeat(INPUT_DOWN, MOUSE_DIR_DOWN) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_MenuConfigCursor++;
                g_MenuConfigCursor = g_MenuConfigCursor % MENU_CONFIG_ROW_COUNT;
            } else {
                switch (g_MenuConfigCursor) {
                case MENU_CONFIG_ROW_CONTROLLER:
                    *(short *)&frame.optionValue = gSavemap.options >> 2 & 3;
                    if (MenuCursor_ReadRepeat(INPUT_LEFT, MOUSE_DIR_LEFT) != 0 && (frame.optionValue & 0xFFFF) != MENU_CONFIG_CONTROLLER_NORMAL) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        (*(short *)&frame.optionValue)--;
                        gSavemap.options &= (unsigned short)~0xC;
                        gSavemap.options |= (short)((frame.optionValue & 0xFFFF) << 2);
                        Input_LoadKeyboardDefaults();
                    }
                    if (MenuCursor_ReadRepeat(INPUT_RIGHT, MOUSE_DIR_RIGHT) != 0 && (frame.optionValue & 0xFFFF) != MENU_CONFIG_CONTROLLER_CUSTOMIZE) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        (*(short *)&frame.optionValue)++;
                        gSavemap.options &= (unsigned short)~0xC;
                        gSavemap.options |= (short)((frame.optionValue & 0xFFFF) << 2);
                        InputConfig_RevertToSaved();
                    }
                    break;
                case MENU_CONFIG_ROW_CURSOR:
                    *(short *)&frame.optionValue = gSavemap.options >> 4 & 3;
                    if (MenuCursor_ReadRepeat(INPUT_LEFT, MOUSE_DIR_LEFT) != 0 && (frame.optionValue & 0xFFFF) != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        (*(short *)&frame.optionValue)--;
                        gSavemap.options &= (unsigned short)~0x30;
                        gSavemap.options |= (short)((frame.optionValue & 0xFFFF) << 4);
                    }
                    if (MenuCursor_ReadRepeat(INPUT_RIGHT, MOUSE_DIR_RIGHT) != 0 && (frame.optionValue & 0xFFFF) != 1) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        (*(short *)&frame.optionValue)++;
                        gSavemap.options &= (unsigned short)~0x30;
                        gSavemap.options |= (short)((frame.optionValue & 0xFFFF) << 4);
                    }
                    break;
                case MENU_CONFIG_ROW_ATB:
                    *(short *)&frame.optionValue = gSavemap.options >> 6 & 3;
                    if (MenuCursor_ReadRepeat(INPUT_LEFT, MOUSE_DIR_LEFT) != 0 && (frame.optionValue & 0xFFFF) != 0) {
                        (*(short *)&frame.optionValue)--;
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        gSavemap.options &= (unsigned short)~0xC0;
                        gSavemap.options |= (short)((frame.optionValue & 0xFFFF) << 6);
                    }
                    if (MenuCursor_ReadRepeat(INPUT_RIGHT, MOUSE_DIR_RIGHT) != 0 && (frame.optionValue & 0xFFFF) != 2) {
                        (*(short *)&frame.optionValue)++;
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        gSavemap.options &= (unsigned short)~0xC0;
                        gSavemap.options |= (short)((frame.optionValue & 0xFFFF) << 6);
                    }
                    break;
                case MENU_CONFIG_ROW_BATTLE_SPEED:
                    if (MenuCursor_ReadHeld(INPUT_LEFT, MOUSE_DIR_LEFT) != 0 && *(unsigned char *)&gSavemap.battleSpeed != 0) {
                        if ((flags & 4) != 0) {
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        }
                        gSavemap.battleSpeed--;
                    }
                    if (MenuCursor_ReadHeld(INPUT_RIGHT, MOUSE_DIR_RIGHT) != 0 && *(unsigned char *)&gSavemap.battleSpeed != 0xFF) {
                        if ((flags & 4) != 0) {
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        }
                        gSavemap.battleSpeed++;
                    }
                    break;
                case MENU_CONFIG_ROW_BATTLE_MESSAGE:
                    if (MenuCursor_ReadHeld(INPUT_LEFT, MOUSE_DIR_LEFT) != 0 && *(unsigned char *)&gSavemap.battleMessageSpeed != 0) {
                        if ((flags & 4) != 0) {
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        }
                        gSavemap.battleMessageSpeed--;
                    }
                    if (MenuCursor_ReadHeld(INPUT_RIGHT, MOUSE_DIR_RIGHT) != 0 && *(unsigned char *)&gSavemap.battleMessageSpeed != 0xFF) {
                        if ((flags & 4) != 0) {
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        }
                        gSavemap.battleMessageSpeed++;
                    }
                    break;
                case MENU_CONFIG_ROW_FIELD_MESSAGE:
                    if (MenuCursor_ReadHeld(INPUT_LEFT, MOUSE_DIR_LEFT) != 0 && gSavemap.fieldMessageSpeed != 0) {
                        if ((flags & 4) != 0) {
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        }
                        gSavemap.fieldMessageSpeed--;
                    }
                    if (MenuCursor_ReadHeld(INPUT_RIGHT, MOUSE_DIR_RIGHT) != 0 && gSavemap.fieldMessageSpeed != 0xFF) {
                        if ((flags & 4) != 0) {
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        }
                        gSavemap.fieldMessageSpeed++;
                    }
                    break;
                case MENU_CONFIG_ROW_CAMERA_ANGLE:
                    *(short *)&frame.optionValue = gSavemap.options >> 8 & 3;
                    if (MenuCursor_ReadRepeat(INPUT_LEFT, MOUSE_DIR_LEFT) != 0 && (frame.optionValue & 0xFFFF) != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        (*(short *)&frame.optionValue)--;
                        gSavemap.options &= (unsigned short)~0x300;
                        gSavemap.options |= (short)((frame.optionValue & 0xFFFF) << 8);
                    }
                    if (MenuCursor_ReadRepeat(INPUT_RIGHT, MOUSE_DIR_RIGHT) != 0 && (frame.optionValue & 0xFFFF) != 1) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        (*(short *)&frame.optionValue)++;
                        gSavemap.options &= (unsigned short)~0x300;
                        gSavemap.options |= (short)((frame.optionValue & 0xFFFF) << 8);
                    }
                    break;
                case MENU_CONFIG_ROW_MAGIC_ORDER:
                    *(short *)&frame.optionValue = gSavemap.options >> 0xA & 7;
                    if (MenuCursor_ReadRepeat(INPUT_LEFT, MOUSE_DIR_LEFT) != 0 && (frame.optionValue & 0xFFFF) != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        (*(short *)&frame.optionValue)--;
                        gSavemap.options &= (unsigned short)~0x1C00;
                        gSavemap.options |= (short)((frame.optionValue & 0xFFFF) << 0xA);
                    }
                    if (MenuCursor_ReadRepeat(INPUT_RIGHT, MOUSE_DIR_RIGHT) != 0 && (frame.optionValue & 0xFFFF) != 5) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        (*(short *)&frame.optionValue)++;
                        gSavemap.options &= (unsigned short)~0x1C00;
                        gSavemap.options |= (short)((frame.optionValue & 0xFFFF) << 0xA);
                    }
                }
            }
            if (Input_GetPressed(INPUT_CIRCLE) != 0) {
                switch (g_MenuConfigCursor) {
                case MENU_CONFIG_ROW_WINDOW_COLOR:
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuConfigState = MENU_CONFIG_STATE_WINDOW_COLOR_CORNER;
                    break;
                case MENU_CONFIG_ROW_SOUND:
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuConfigState = MENU_CONFIG_STATE_VOLUME;
                    break;
                case MENU_CONFIG_ROW_CONTROLLER:
                    *(short *)&frame.optionValue = gSavemap.options >> 2 & 3;
                    if ((frame.optionValue & 0xFFFF) == MENU_CONFIG_CONTROLLER_CUSTOMIZE) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_MenuConfigState = MENU_CONFIG_STATE_CONTROLLER;
                        InputConfig_ScreenEnter();
                    }
                }
            } else if (Input_GetPressed(INPUT_CROSS) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                MenuConfig_SaveVolumeAndInputConfig();
                Menu_SetPhase(5, 0);
                Menu_SetScreen(0);
            }
            break;
        case MENU_CONFIG_STATE_VOLUME:
            if (Input_GetPressed(INPUT_CROSS) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_MenuConfigState = MENU_CONFIG_STATE_BROWSE;
            }
            MenuCursor_Update((MenuCursor *)&g_MenuConfigVolumeCursor);
            switch (g_MenuConfigVolumeCursorRow) {
            case MENU_CONFIG_VOLUME_MUSIC:
                if (BtlMenu_GetInputRepeat(INPUT_LEFT) != 0) {
                    if (g_MenuMusicVolume != 0 && g_MenuMidiVolumeControlFlag != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_MenuMusicVolume--;
                        MusicSetMasterVolumePercent(g_MenuMusicVolume);
                        MidiSetMasterVolume(g_MenuMusicVolume);
                    }
                } else if (BtlMenu_GetInputRepeat(INPUT_RIGHT) != 0 && g_MenuMusicVolume != 0x64 && g_MenuMidiVolumeControlFlag != 0) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuMusicVolume++;
                    MusicSetMasterVolumePercent(g_MenuMusicVolume);
                    MidiSetMasterVolume(g_MenuMusicVolume);
                }
                break;
            case MENU_CONFIG_VOLUME_SFX:
                if (BtlMenu_GetInputRepeat(INPUT_LEFT) != 0) {
                    if (g_MenuSfxVolume != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_MenuSfxVolume--;
                        SfxSetMasterVolumePercent(g_MenuSfxVolume);
                    }
                } else if (BtlMenu_GetInputRepeat(INPUT_RIGHT) != 0 && g_MenuSfxVolume != 0x64) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuSfxVolume++;
                    SfxSetMasterVolumePercent(g_MenuSfxVolume);
                }
            }
        }
    }
}

/*
 * Draws double-scale configuration rows from the fixed text table and panel-
 * relative Y origin. Colors the selected radio options from two-bit fields at
 * shifts 2, 4, 6 and 8, with three alternatives for shift 6.
 */
// FUNCTION: FF7 0x006c3197
void __cdecl MenuConfig_DrawRows2x(void)
{
    EnglishWindowFrame rect;
    int baseY;
    int rowIndex;

    baseY = g_MenuConfigFrames[1].y + 0x42;
    Menu_DrawEncodedString2x(0x3e, g_MenuConfigFrames[1].y + 0x1c,
                      g_MenuConfigOptionLabels[0], 5, 0.1f);
    for (rowIndex = 1; rowIndex < 9; rowIndex++) {
        Menu_DrawEncodedString2x(0x3e, baseY + (rowIndex - 1) * 0x28,
                          g_MenuConfigOptionLabels[rowIndex], 5, 0.1f);
    }
    Menu_DrawEncodedString2x(0x3e, baseY + (rowIndex - 1) * 0x28, g_MenuConfigMagicOrderLabels, 5,
                      0.1f);
    for (rowIndex = 0; rowIndex < 2; rowIndex++) {
        Menu_DrawEncodedString2x(0x108, baseY, g_MenuSoundVolumeHeaderText, 7, 0.1f);
        Menu_DrawEncodedString2x(rowIndex * 0xd * 8 + 0x108, baseY + 0x28,
                          g_MenuConfigOptionLabels[rowIndex + 0x12],
                          ((gSavemap.options >> 2) & 3) == rowIndex ? 7 : 0, 0.1f);
        Menu_DrawEncodedString2x(rowIndex * 0xd * 8 + 0x108, baseY + 0x50,
                          g_MenuConfigOptionLabels[rowIndex + 0x14],
                          ((gSavemap.options >> 4) & 3) == rowIndex ? 7 : 0, 0.1f);
        Menu_DrawEncodedString2x(rowIndex * 0xd * 8 + 0x108, baseY + 0x118,
                          g_MenuConfigOptionLabels[rowIndex + 0x19],
                          ((gSavemap.options >> 8) & 3) == rowIndex ? 7 : 0, 0.1f);
    }
    for (rowIndex = 0; rowIndex < 2; rowIndex++) {
        Menu_DrawEncodedString2x(rowIndex * 0xb * 8 + 0x108, baseY + 0x78,
                          g_MenuConfigOptionLabels[rowIndex + 0x16],
                          ((gSavemap.options >> 6) & 3) == rowIndex ? 7 : 0, 0.1f);
    }
    Menu_DrawEncodedString2x(0x212, baseY + 0x78, g_MenuConfigAtbModeLabels,
                      ((gSavemap.options >> 6) & 3) == 2 ? 7 : 0, 0.1f);
    for (rowIndex = 0; rowIndex < 3; rowIndex++) {
        Menu_DrawEncodedString2x(rowIndex * 0x58 + 0x155, baseY + 0x140,
                          g_MenuConfigOptionLabels[g_MenuMagicOrderLabelSlots[((gSavemap.options >> 0xa) & 7) * 3 + rowIndex] + 0x22],
                          7, 0.1f);
    }
    Menu_DrawEncodedString2x(0x108, baseY + 0x140, g_MenuConfigControlsLabels, 7, 0.1f);
    Menu_DrawDecimalNumberZeroPad2x(0x130, baseY + 0x144,
                               ((gSavemap.options >> 0xa) & 7) + 1, 1, 7, 0.1f);
    rect.x = (unsigned char)gSavemap.battleSpeed + 0x126;
    rect.y = baseY + 0xa0;
    rect.width = 0xd;
    rect.height = 0x16;
    Menu_DrawRaisedBox2x(&rect.x, 0.1f);
    rect.x = (unsigned char)gSavemap.battleMessageSpeed + 0x126;
    rect.y = baseY + 0xc8;
    rect.width = 0xd;
    rect.height = 0x16;
    Menu_DrawRaisedBox2x(&rect.x, 0.1f);
    rect.x = gSavemap.fieldMessageSpeed + 0x126;
    rect.y = baseY + 0xf0;
    rect.width = 0xd;
    rect.height = 0x16;
    Menu_DrawRaisedBox2x(&rect.x, 0.1f);
    for (rowIndex = 0; rowIndex < 3; rowIndex++) {
        Menu_DrawEncodedString2x(0xf3, baseY + (rowIndex + 4) * 0x28 + 4,
                          g_MenuConfigFastLabels, 7, 0.1f);
        Menu_DrawEncodedString2x(0x238, baseY + (rowIndex + 4) * 0x28 + 4,
                          g_MenuConfigSlowLabel, 7, 0.1f);
        rect.x = 0x126;
        rect.y = baseY + (rowIndex + 4) * 0x28;
        rect.width = 0x10c;
        rect.height = 0x16;
        Menu_DrawEtchedBox2x(&rect, 0.101f);
    }
}

/*
 * Draws single-scale configuration rows from the fixed text table and panel-
 * relative Y origin. Colors radio choices from two-bit fields at shifts 2, 4, 6
 * and 8, with a third alternative for shift 6.
 */
// FUNCTION: FF7 0x006c3624
void __cdecl MenuConfig_DrawRows1x(void)
{
    EnglishWindowFrame rect;
    int baseY;
    int rowIndex;

    baseY = g_MenuConfigFrames[1].y + 0x21;
    Menu_DrawEncodedString1x(0x1f, g_MenuConfigFrames[1].y + 0xe,
                         g_MenuConfigOptionLabels[0], 5, 0.1f);
    for (rowIndex = 1; rowIndex < 9; rowIndex++) {
        Menu_DrawEncodedString1x(0x1f, baseY + (rowIndex - 1) * 0x14,
                             g_MenuConfigOptionLabels[rowIndex], 5, 0.1f);
    }
    Menu_DrawEncodedString1x(0x1f, baseY + (rowIndex - 1) * 0x14, g_MenuConfigMagicOrderLabels,
                         5, 0.1f);
    for (rowIndex = 0; rowIndex < 2; rowIndex++) {
        Menu_DrawEncodedString1x(0x84, baseY, g_MenuSoundVolumeHeaderText, 7, 0.1f);
        Menu_DrawEncodedString1x(rowIndex * 0xd * 4 + 0x84, baseY + 0x14,
                             g_MenuConfigOptionLabels[rowIndex + 0x12],
                             ((gSavemap.options >> 2) & 3) == rowIndex ? 7 : 0, 0.1f);
        Menu_DrawEncodedString1x(rowIndex * 0xd * 4 + 0x84, baseY + 0x28,
                             g_MenuConfigOptionLabels[rowIndex + 0x14],
                             ((gSavemap.options >> 4) & 3) == rowIndex ? 7 : 0, 0.1f);
        Menu_DrawEncodedString1x(rowIndex * 0xd * 4 + 0x84, baseY + 0x8c,
                             g_MenuConfigOptionLabels[rowIndex + 0x19],
                             ((gSavemap.options >> 8) & 3) == rowIndex ? 7 : 0, 0.1f);
    }
    for (rowIndex = 0; rowIndex < 2; rowIndex++) {
        Menu_DrawEncodedString1x(rowIndex * 0xb * 4 + 0x84, baseY + 0x3c,
                             g_MenuConfigOptionLabels[rowIndex + 0x16],
                             ((gSavemap.options >> 6) & 3) == rowIndex ? 7 : 0, 0.1f);
    }
    Menu_DrawEncodedString1x(0x109, baseY + 0x3c, g_MenuConfigAtbModeLabels,
                         ((gSavemap.options >> 6) & 3) == 2 ? 7 : 0, 0.1f);
    for (rowIndex = 0; rowIndex < 3; rowIndex++) {
        Menu_DrawEncodedString1x(rowIndex * 0x30 + 0xaa, baseY + 0xa0,
                             g_MenuConfigOptionLabels[g_MenuMagicOrderLabelSlots[((gSavemap.options >> 0xa) & 7) * 3 + rowIndex] + 0x22],
                             7, 0.1f);
    }
    Menu_DrawEncodedString1x(0x84, baseY + 0xa0, g_MenuConfigControlsLabels, 7, 0.1f);
    Menu_DrawDecimalNumberZeroPad1x(0x98, baseY + 0xa2,
                               ((gSavemap.options >> 0xa) & 7) + 1, 1, 7, 0.1f);
    rect.x = ((unsigned char)gSavemap.battleSpeed >> 1) + 0x93;
    rect.y = baseY + 0x50;
    rect.width = 7;
    rect.height = 0xb;
    Menu_DrawRaisedBox1x(&rect.x, 0.1f);
    rect.x = ((unsigned char)gSavemap.battleMessageSpeed >> 1) + 0x93;
    rect.y = baseY + 0x64;
    rect.width = 7;
    rect.height = 0xb;
    Menu_DrawRaisedBox1x(&rect.x, 0.1f);
    rect.x = (gSavemap.fieldMessageSpeed >> 1) + 0x93;
    rect.y = baseY + 0x78;
    rect.width = 7;
    rect.height = 0xb;
    Menu_DrawRaisedBox1x(&rect.x, 0.1f);
    for (rowIndex = 0; rowIndex < 3; rowIndex++) {
        Menu_DrawEncodedString1x(0x7a, baseY + (rowIndex + 4) * 0x14 + 1,
                             g_MenuConfigFastLabels, 7, 0.1f);
        Menu_DrawEncodedString1x(0x11c, baseY + (rowIndex + 4) * 0x14 + 1,
                             g_MenuConfigSlowLabel, 7, 0.1f);
        rect.x = 0x93;
        rect.y = baseY + (rowIndex + 4) * 0x14;
        rect.width = 0x86;
        rect.height = 0xb;
        Menu_DrawEtchedBox1x(&rect, 0.101f);
    }
}

/*
 * Saves the Config screen's volumes to the registry and the input configuration to
 * disk.
 */
// FUNCTION: FF7 0x006c3aae
void __cdecl MenuConfig_SaveVolumeAndInputConfig(void)
{
    Engine_RegWriteVolumeSettings(g_MenuMusicVolume, g_MenuSfxVolume);
    InputConfig_SaveToFile();
}

/*
 * Runs the four-corner window color editor, initializing corner/channel cursors on
 * entry and drawing the selected corner plus RGB values, sliders and color preview
 * at either UI scale. Corner-selection mode toggles the two row/column
 * coordinates; editing mode cycles RGB channels and changes the selected byte
 * within 0 through 255 from held horizontal input, with periodic sound.
 */
// FUNCTION: FF7 0x006c3acd
void __cdecl MenuConfig_RunWindowColorEditor(unsigned int frameCounter)
{
    struct {
        int rgbOffset;
        int posY;
        int posX;
        int chanIdx;
        EnglishWindowFrame rect;
        EnglishWindowFrame box;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_MenuConfigEditorActive == 0) {
        g_MenuConfigEditorMode = MENU_CONFIG_COLOR_EDITOR_SELECT_CORNER;
        g_MenuWindowColorColumn = 0;
        g_MenuWindowColorRow = 0;
        g_MenuConfigClearedWord = 0;
        g_MenuColorEditorCursor = MENU_CONFIG_COLOR_RED;
        g_MenuConfigEditorActive = 1;
    }

    if (g_MenuIsSmallViewport320x240 != 1) {
        switch (g_MenuConfigEditorMode) {
        case MENU_CONFIG_COLOR_EDITOR_SELECT_CORNER:
            Menu_DrawMoreTextArrow2x(g_MenuWindowColorColumn * 0x66 + 0xD8 + g_MenuConfigFrames[1].x,
                                g_MenuWindowColorRow * 0x2A + 8 + g_MenuConfigFrames[1].y, 0.0f);
            break;
        case MENU_CONFIG_COLOR_EDITOR_EDIT_COLOR:
            if ((frameCounter & 2) != 0) {
                Menu_DrawMoreTextArrow2x(g_MenuWindowColorColumn * 0x66 + 0xD8 + g_MenuConfigFrames[1].x,
                                    g_MenuWindowColorRow * 0x2A + 8 + g_MenuConfigFrames[1].y, 0.0001f);
            }
            local.posX = g_MenuConfigFrames[1].x + 0x115;
            local.posY = g_MenuConfigFrames[1].y + 0x4B;
            DrawIconGlyph16_2x(local.posX, local.posY, g_MenuConfigColorChannelNames[0][0], 2, 0.001f);
            DrawIconGlyph16_2x(local.posX, local.posY + 0x1A, g_MenuConfigColorChannelNames[1][0], 4, 0.001f);
            DrawIconGlyph16_2x(local.posX, local.posY + 0x33, g_MenuConfigColorChannelNames[2][0], 1, 0.001f);
            for (local.chanIdx = 0; local.chanIdx < 3; local.chanIdx++) {
                switch (local.chanIdx) {
                case MENU_CONFIG_COLOR_RED:
                    Menu_DrawDecimalNumberZeroPad2x(local.posX + 0x20, local.posY + local.chanIdx * 0x1A,
                                               g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 2],
                                               3, 7, 0.001f);
                    local.box.x = g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 2] + 0x162;
                    local.box.y = g_MenuConfigFrames[1].y + local.chanIdx * 0x1A + 0x47;
                    local.box.width = 0xD;
                    local.box.height = 0x16;
                    Menu_DrawRaisedBox2x(&local.box.x, 0.0001f);
                    break;
                case MENU_CONFIG_COLOR_GREEN:
                    Menu_DrawDecimalNumberZeroPad2x(local.posX + 0x20, local.posY + local.chanIdx * 0x1A,
                                               g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 1],
                                               3, 7, 0.001f);
                    local.box.x = g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 1] + 0x162;
                    local.box.y = g_MenuConfigFrames[1].y + local.chanIdx * 0x1A + 0x47;
                    local.box.width = 0xD;
                    local.box.height = 0x16;
                    Menu_DrawRaisedBox2x(&local.box.x, 0.0001f);
                    break;
                case MENU_CONFIG_COLOR_BLUE:
                    Menu_DrawDecimalNumberZeroPad2x(local.posX + 0x20, local.posY + local.chanIdx * 0x1A,
                                               g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4],
                                               3, 7, 0.001f);
                    local.box.x = g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4] + 0x162;
                    local.box.y = g_MenuConfigFrames[1].y + local.chanIdx * 0x1A + 0x47;
                    local.box.width = 0xD;
                    local.box.height = 0x16;
                    Menu_DrawRaisedBox2x(&local.box.x, 0.0001f);
                    break;
                }
                local.box.x = 0x162;
                local.box.y = g_MenuConfigFrames[1].y + local.chanIdx * 0x1A + 0x47;
                local.box.width = 0x10C;
                local.box.height = 0x16;
                Menu_DrawEtchedBox2x(&local.box, 0.001f);
            }
            Menu_DrawMoreTextArrow2x(0xE2, g_MenuColorEditorCursor * 0x1A + 0x48 + g_MenuConfigFrames[1].y, 0.0f);
            Menu_SetRectShorts(&local.rect.x, g_MenuConfigFrames[1].x + 0x108,
                          g_MenuConfigFrames[1].y + 0x3E, 0x170, 0x5C);
            Menu_DrawWindowFrame2x(&local.rect, 0.01f);
            Menu_SetRectShorts(&local.rect.x, g_MenuConfigFrames[1].x + 0x18D,
                          g_MenuConfigFrames[1].y + 0xE, 0x2D, 0x2A);
            Menu_FillRectShorts2x(&local.rect.x, ((int *)g_MenuConfigWindowColors)[g_MenuWindowColorRow + g_MenuWindowColorColumn * 2], 0);
            Menu_SetRectShorts(&local.rect.x, g_MenuConfigFrames[1].x + 0x188,
                          g_MenuConfigFrames[1].y + 8, 0x33, 0x33);
            Menu_DrawWindowFrame2x(&local.rect, 0.0f);
            break;
        }
    } else {
        switch (g_MenuConfigEditorMode) {
        case MENU_CONFIG_COLOR_EDITOR_SELECT_CORNER:
            Menu_DrawMoreTextArrow1x(g_MenuWindowColorColumn * 0x33 + 0x6C + g_MenuConfigFrames[1].x,
                                g_MenuWindowColorRow * 0x15 + 4 + g_MenuConfigFrames[1].y, 0.0f);
            break;
        case MENU_CONFIG_COLOR_EDITOR_EDIT_COLOR:
            if ((frameCounter & 2) != 0) {
                Menu_DrawMoreTextArrow1x(g_MenuWindowColorColumn * 0x33 + 0x6C + g_MenuConfigFrames[1].x,
                                    g_MenuWindowColorRow * 0x15 + 4 + g_MenuConfigFrames[1].y, 0.0001f);
            }
            local.posX = g_MenuConfigFrames[1].x + 0x8B;
            local.posY = g_MenuConfigFrames[1].y + 0x27;
            DrawIconGlyph16_1x(local.posX, local.posY, g_MenuConfigColorChannelNames[0][0], 2, 0.001f);
            DrawIconGlyph16_1x(local.posX, local.posY + 0xD, g_MenuConfigColorChannelNames[1][0], 4, 0.001f);
            DrawIconGlyph16_1x(local.posX, local.posY + 0x1A, g_MenuConfigColorChannelNames[2][0], 1, 0.001f);
            for (local.chanIdx = 0; local.chanIdx < 3; local.chanIdx++) {
                switch (local.chanIdx) {
                case MENU_CONFIG_COLOR_RED:
                    Menu_DrawDecimalNumberZeroPad1x(local.posX + 0x10, local.posY + local.chanIdx * 0xD,
                                               g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 2],
                                               3, 7, 0.001f);
                    local.box.x = (g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 2] >> 1) + 0xB1;
                    local.box.y = g_MenuConfigFrames[1].y + local.chanIdx * 0xD + 0x24;
                    local.box.width = 7;
                    local.box.height = 0xB;
                    Menu_DrawRaisedBox1x(&local.box.x, 0.0001f);
                    break;
                case MENU_CONFIG_COLOR_GREEN:
                    Menu_DrawDecimalNumberZeroPad1x(local.posX + 0x10, local.posY + local.chanIdx * 0xD,
                                               g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 1],
                                               3, 7, 0.001f);
                    local.box.x = (g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 1] >> 1) + 0xB1;
                    local.box.y = g_MenuConfigFrames[1].y + local.chanIdx * 0xD + 0x24;
                    local.box.width = 7;
                    local.box.height = 0xB;
                    Menu_DrawRaisedBox1x(&local.box.x, 0.0001f);
                    break;
                case MENU_CONFIG_COLOR_BLUE:
                    Menu_DrawDecimalNumberZeroPad1x(local.posX + 0x10, local.posY + local.chanIdx * 0xD,
                                               g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4],
                                               3, 7, 0.001f);
                    local.box.x = (g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4] >> 1) + 0xB1;
                    local.box.y = g_MenuConfigFrames[1].y + local.chanIdx * 0xD + 0x24;
                    local.box.width = 7;
                    local.box.height = 0xB;
                    Menu_DrawRaisedBox1x(&local.box.x, 0.0001f);
                    break;
                }
                local.box.x = 0xB1;
                local.box.y = g_MenuConfigFrames[1].y + local.chanIdx * 0xD + 0x24;
                local.box.width = 0x86;
                local.box.height = 0xB;
                Menu_DrawEtchedBox1x(&local.box, 0.001f);
            }
            Menu_DrawMoreTextArrow1x(0x71, g_MenuColorEditorCursor * 0xD + 0x24 + g_MenuConfigFrames[1].y, 0.0f);
            Menu_SetRectShorts(&local.rect.x, g_MenuConfigFrames[1].x + 0x84,
                          g_MenuConfigFrames[1].y + 0x1F, 0xB8, 0x2E);
            Menu_DrawWindowFrame1x(&local.rect, 0.01f);
            Menu_SetRectShorts(&local.rect.x, g_MenuConfigFrames[1].x + 0xC7,
                          g_MenuConfigFrames[1].y + 7, 0x17, 0x15);
            Menu_FillRectShorts2x(&local.rect.x, ((int *)g_MenuConfigWindowColors)[g_MenuWindowColorRow + g_MenuWindowColorColumn * 2], 0);
            Menu_SetRectShorts(&local.rect.x, g_MenuConfigFrames[1].x + 0xC4,
                          g_MenuConfigFrames[1].y + 4, 0x1A, 0x1A);
            Menu_DrawWindowFrame1x(&local.rect, 0.0f);
            break;
        }
    }

    if (MenuCursor_ReadRepeat(INPUT_UP, MOUSE_DIR_UP) != 0) {
        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
        switch (g_MenuConfigEditorMode) {
        case MENU_CONFIG_COLOR_EDITOR_SELECT_CORNER:
            g_MenuWindowColorRow ^= 1;
            break;
        case MENU_CONFIG_COLOR_EDITOR_EDIT_COLOR:
            g_MenuColorEditorCursor--;
            if (g_MenuColorEditorCursor < 0) {
                g_MenuColorEditorCursor = MENU_CONFIG_COLOR_BLUE;
            }
            break;
        }
    } else if (MenuCursor_ReadRepeat(INPUT_DOWN, MOUSE_DIR_DOWN) != 0) {
        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
        switch (g_MenuConfigEditorMode) {
        case MENU_CONFIG_COLOR_EDITOR_SELECT_CORNER:
            g_MenuWindowColorRow ^= 1;
            break;
        case MENU_CONFIG_COLOR_EDITOR_EDIT_COLOR:
            g_MenuColorEditorCursor++;
            g_MenuColorEditorCursor %= 3;
            break;
        }
    } else {
        switch (g_MenuConfigEditorMode) {
        case MENU_CONFIG_COLOR_EDITOR_SELECT_CORNER:
            if (MenuCursor_ReadRepeat(INPUT_LEFT, MOUSE_DIR_LEFT) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_MenuWindowColorColumn ^= 1;
            }
            if (MenuCursor_ReadRepeat(INPUT_RIGHT, MOUSE_DIR_RIGHT) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_MenuWindowColorColumn ^= 1;
            }
            break;
        case MENU_CONFIG_COLOR_EDITOR_EDIT_COLOR:
            switch (g_MenuColorEditorCursor) {
            case MENU_CONFIG_COLOR_RED:
                if (MenuCursor_ReadHeld(INPUT_LEFT, MOUSE_DIR_LEFT) != 0 &&
                    g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 2] != 0) {
                    if ((frameCounter & 4) != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    }
                    g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 2]--;
                }
                if (MenuCursor_ReadHeld(INPUT_RIGHT, MOUSE_DIR_RIGHT) != 0 &&
                    g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 2] != 0xFF) {
                    if ((frameCounter & 4) != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    }
                    g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 2]++;
                }
                break;
            case MENU_CONFIG_COLOR_GREEN:
                if (MenuCursor_ReadHeld(INPUT_LEFT, MOUSE_DIR_LEFT) != 0 &&
                    g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 1] != 0) {
                    if ((frameCounter & 4) != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    }
                    g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 1]--;
                }
                if (MenuCursor_ReadHeld(INPUT_RIGHT, MOUSE_DIR_RIGHT) != 0 &&
                    g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 1] != 0xFF) {
                    if ((frameCounter & 4) != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    }
                    g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4 + 1]++;
                }
                break;
            case MENU_CONFIG_COLOR_BLUE:
                if (MenuCursor_ReadHeld(INPUT_LEFT, MOUSE_DIR_LEFT) != 0 &&
                    g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4] != 0) {
                    if ((frameCounter & 4) != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    }
                    g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4]--;
                }
                if (MenuCursor_ReadHeld(INPUT_RIGHT, MOUSE_DIR_RIGHT) != 0 &&
                    g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4] != 0xFF) {
                    if ((frameCounter & 4) != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    }
                    g_MenuConfigWindowColors[(g_MenuWindowColorRow + g_MenuWindowColorColumn * 2) * 4]++;
                }
                break;
            }
            break;
        }
    }

    if (Input_GetPressed(INPUT_CIRCLE) != 0) {
        switch (g_MenuConfigEditorMode) {
        case MENU_CONFIG_COLOR_EDITOR_SELECT_CORNER:
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
            g_MenuConfigEditorMode = MENU_CONFIG_COLOR_EDITOR_EDIT_COLOR;
            g_MenuConfigState = MENU_CONFIG_STATE_WINDOW_COLOR_EDIT;
            break;
        }
    } else if (Input_GetPressed(INPUT_CROSS) != 0) {
        SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
        switch (g_MenuConfigEditorMode) {
        case MENU_CONFIG_COLOR_EDITOR_SELECT_CORNER:
            g_MenuConfigEditorMode = MENU_CONFIG_COLOR_EDITOR_CLOSED;
            for (local.chanIdx = 0; local.chanIdx < 4; local.chanIdx++) {
                local.rgbOffset = local.chanIdx * 3;
                g_FieldWindowColours[local.rgbOffset] = g_MenuConfigWindowColors[local.chanIdx * 4 + 2];
                g_FieldWindowColours[local.rgbOffset + 1] = g_MenuConfigWindowColors[local.chanIdx * 4 + 1];
                g_FieldWindowColours[local.rgbOffset + 2] = g_MenuConfigWindowColors[local.chanIdx * 4];
            }
            Menu_SetWindowColors((int *)g_MenuConfigWindowColors);
            g_MenuConfigEditorActive = 0;
            g_MenuConfigState = MENU_CONFIG_STATE_BROWSE;
            Input_Update(0);
            break;
        case MENU_CONFIG_COLOR_EDITOR_EDIT_COLOR:
            g_MenuConfigEditorMode = MENU_CONFIG_COLOR_EDITOR_SELECT_CORNER;
            g_MenuConfigState = MENU_CONFIG_STATE_WINDOW_COLOR_CORNER;
            break;
        case MENU_CONFIG_COLOR_EDITOR_REENTER_EDIT:
            g_MenuConfigState = MENU_CONFIG_STATE_WINDOW_COLOR_CORNER;
            g_MenuConfigEditorMode = MENU_CONFIG_COLOR_EDITOR_EDIT_COLOR;
            break;
        }
    }
}

/*
 * Applies the Config screen's sliders: music master volume (both the music and
 * MIDI paths) to musicVolume and sound effect master volume to sfxVolume.
 */
// FUNCTION: FF7 0x006c4946
void __cdecl Engine_ApplyMusicAndSfxVolume(int musicVolume, int sfxVolume)
{
    MusicSetMasterVolumePercent(musicVolume);
    MidiSetMasterVolume(musicVolume);
    SfxSetMasterVolumePercent(sfxVolume);
}
} /* extern "C" */
