// src/ff7/src/menu/english/menu_item_sort_screen.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Sorting of the party item inventory by type, quantity, name, price, battle use and
//   throwability, with the item menu screen's setup, frame and icons. A group of savemap routines
//   that stash, restore and swap materia while equipment changes is also here.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/enums/battle.h"
#include "ff7/types/menu.h"
#include "ff7/enums/menu.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include <string.h>
#include "lib/api/file.h"
#include "lib/api/sound.h"
#include <stdlib.h>
#include "ff7/savemap.h"
#include "ff7/enums/main.h"
extern "C" {





struct EnglishWindowFrame;

// clang-format off
// Text: "Use" "Arrange" "Key Items" "Customize" "Field" "Battle" "Throw" "Type" "Name" "Most"
//       "Least"
// GLOBAL: FF7 0x00921168
unsigned char g_MenuItemLabelTable[132] = {
    0x35, 0x53, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x52, 0x52, 0x41, 0x4e, 0x47, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x2b, 0x45, 0x59, 0x00, 0x29, 0x54, 0x45, 0x4d, 0x53, 0xff, 0x00, 0x00, 0x23, 0x55, 0x53, 0x54, 0x4f, 0x4d, 0x49, 0x5a, 0x45, 0xff, 0x00, 0x00,
    0x26, 0x49, 0x45, 0x4c, 0x44, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x41, 0x54, 0x54, 0x4c, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x34, 0x48, 0x52, 0x4f, 0x57, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x59, 0x50, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2e, 0x41, 0x4d, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x4f, 0x53, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2c, 0x45, 0x41, 0x53, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// Text: "OK! Got it!" "I'm not sure but…" "Don't really care." "Heh! No sweat!"
//       "D-d-d-damn! My head hurts!" "Got nuthin' to do with me!" "Hmm, so that's it."
//       "I'm not too sure but…" "I've got the wrong person." "This should be okay."
//       "Not quite ready yet." "It isn't me." "Right, I've got it!" "I don't get it."
//       "It has nothing to do with me." "Cool! This should do it!" "Nope! I don't get it!"
//       "Oh wow, I'm totally clueless!" "I'm getting less human…" "I'm still not ready."
//       "It has nothing to do with me." "Hey I like this! Lemme use it!"
//       "Hmph. Don't quite get it but…" "Not me! Give it to someone else!"
// GLOBAL: FF7 0x009211f0
unsigned char g_MenuItemManualText[816] = {0x2f, 0x2b, 0x01, 0x00, 0x27, 0x4f, 0x54, 0x00, 0x49, 0x54, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x07, 0x4d, 0x00, 0x4e, 0x4f, 0x54, 0x00, 0x53, 0x55, 0x52, 0x45, 0x00, 0x42, 0x55, 0x54, 0xa9, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x4f, 0x4e, 0x07, 0x54, 0x00, 0x52, 0x45, 0x41, 0x4c, 0x4c, 0x59, 0x00, 0x43, 0x41, 0x52, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x45, 0x48, 0x01, 0x00, 0x2e, 0x4f, 0x00, 0x53, 0x57, 0x45, 0x41, 0x54, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x0d, 0x44, 0x0d, 0x44, 0x0d, 0x44, 0x41, 0x4d, 0x4e, 0x01, 0x00, 0x2d, 0x59, 0x00, 0x48, 0x45, 0x41, 0x44, 0x00, 0x48, 0x55, 0x52, 0x54, 0x53, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x4f, 0x54, 0x00, 0x4e, 0x55, 0x54, 0x48, 0x49, 0x4e, 0x07, 0x00, 0x54, 0x4f, 0x00, 0x44, 0x4f, 0x00, 0x57, 0x49, 0x54, 0x48, 0x00, 0x4d, 0x45, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x4d, 0x4d, 0x0c, 0x00, 0x53, 0x4f, 0x00, 0x54, 0x48, 0x41, 0x54, 0x07, 0x53, 0x00, 0x49, 0x54, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x07, 0x4d, 0x00, 0x4e, 0x4f, 0x54, 0x00, 0x54, 0x4f, 0x4f, 0x00, 0x53, 0x55, 0x52, 0x45, 0x00, 0x42, 0x55, 0x54, 0xa9, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x07, 0x56, 0x45, 0x00, 0x47, 0x4f, 0x54, 0x00, 0x54, 0x48, 0x45, 0x00, 0x57, 0x52, 0x4f, 0x4e, 0x47, 0x00, 0x50, 0x45, 0x52, 0x53, 0x4f, 0x4e, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x48, 0x49, 0x53, 0x00, 0x53, 0x48, 0x4f, 0x55, 0x4c, 0x44, 0x00, 0x42, 0x45, 0x00, 0x4f, 0x4b, 0x41, 0x59, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x4f, 0x54, 0x00, 0x51, 0x55, 0x49, 0x54, 0x45, 0x00, 0x52, 0x45, 0x41, 0x44, 0x59, 0x00, 0x59, 0x45, 0x54, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x54, 0x00, 0x49, 0x53, 0x4e, 0x07, 0x54, 0x00, 0x4d, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x49, 0x47, 0x48, 0x54, 0x0c, 0x00, 0x29, 0x07, 0x56, 0x45, 0x00, 0x47, 0x4f, 0x54, 0x00, 0x49, 0x54, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x44, 0x4f, 0x4e, 0x07, 0x54, 0x00, 0x47, 0x45, 0x54, 0x00, 0x49, 0x54, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x54, 0x00, 0x48, 0x41, 0x53, 0x00, 0x4e, 0x4f, 0x54, 0x48, 0x49, 0x4e, 0x47, 0x00, 0x54, 0x4f, 0x00, 0x44, 0x4f, 0x00, 0x57, 0x49, 0x54, 0x48, 0x00, 0x4d, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x23, 0x4f, 0x4f, 0x4c, 0x01, 0x00, 0x34, 0x48, 0x49, 0x53, 0x00, 0x53, 0x48, 0x4f, 0x55, 0x4c, 0x44, 0x00, 0x44, 0x4f, 0x00, 0x49, 0x54, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x4f, 0x50, 0x45, 0x01, 0x00, 0x29, 0x00, 0x44, 0x4f, 0x4e, 0x07, 0x54, 0x00, 0x47, 0x45, 0x54, 0x00, 0x49, 0x54, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2f, 0x48, 0x00, 0x57, 0x4f, 0x57, 0x0c, 0x00, 0x29, 0x07, 0x4d, 0x00, 0x54, 0x4f, 0x54, 0x41, 0x4c, 0x4c, 0x59, 0x00, 0x43, 0x4c, 0x55, 0x45, 0x4c, 0x45, 0x53, 0x53, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x29, 0x07, 0x4d, 0x00, 0x47, 0x45, 0x54, 0x54, 0x49, 0x4e, 0x47, 0x00, 0x4c, 0x45, 0x53, 0x53, 0x00, 0x48, 0x55, 0x4d, 0x41, 0x4e, 0xa9, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x07, 0x4d, 0x00, 0x53, 0x54, 0x49, 0x4c, 0x4c, 0x00, 0x4e, 0x4f, 0x54, 0x00, 0x52, 0x45, 0x41, 0x44, 0x59, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x54, 0x00, 0x48, 0x41, 0x53, 0x00, 0x4e, 0x4f, 0x54, 0x48, 0x49, 0x4e, 0x47, 0x00, 0x54, 0x4f, 0x00, 0x44, 0x4f, 0x00, 0x57, 0x49, 0x54, 0x48, 0x00, 0x4d, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x28, 0x45, 0x59, 0x00, 0x29, 0x00, 0x4c, 0x49, 0x4b, 0x45, 0x00, 0x54, 0x48, 0x49, 0x53, 0x01, 0x00, 0x2c, 0x45, 0x4d, 0x4d, 0x45, 0x00, 0x55, 0x53, 0x45, 0x00, 0x49, 0x54, 0x01, 0xff, 0x00, 0x00, 0x00, 0x28, 0x4d, 0x50, 0x48, 0x0e, 0x00, 0x24, 0x4f, 0x4e, 0x07, 0x54, 0x00, 0x51, 0x55, 0x49, 0x54, 0x45, 0x00, 0x47, 0x45, 0x54, 0x00, 0x49, 0x54, 0x00, 0x42, 0x55, 0x54, 0xa9, 0xff, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x4f, 0x54, 0x00, 0x4d, 0x45, 0x01, 0x00, 0x27, 0x49, 0x56, 0x45, 0x00, 0x49, 0x54, 0x00, 0x54, 0x4f, 0x00, 0x53, 0x4f, 0x4d, 0x45, 0x4f, 0x4e, 0x45, 0x00, 0x45, 0x4c, 0x53, 0x45, 0x01, 0xff, 0x00};
// Text: "It's got nothing to do with me."
// GLOBAL: FF7 0x00921520
unsigned char g_MenuItemManualNoUseText[40] = {0x29, 0x54, 0x07, 0x53, 0x00, 0x47, 0x4f, 0x54, 0x00, 0x4e, 0x4f, 0x54, 0x48, 0x49, 0x4e, 0x47, 0x00, 0x54, 0x4f, 0x00, 0x44, 0x4f, 0x00, 0x57, 0x49, 0x54, 0x48, 0x00, 0x4d, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00921548
unsigned short g_ItemMateriaSortKeys[320] = {195, 121, 292, 80, 271, 76, 158, 187, 10, 240, 152, 46, 74, 125, 268, 212, 237, 243, 120, 275, 108, 229, 214, 124, 136, 245, 68, 170, 281, 143, 86, 88, 9, 127, 24, 257, 71, 73, 57, 148, 251, 277, 97, 276, 141, 147, 164, 123, 20, 66, 129, 230, 81, 166, 219, 2, 105, 259, 131, 56, 65, 36, 258, 209, 161, 54, 182, 261, 137, 112, 262, 196, 109, 151, 162, 244, 146, 295, 33, 194, 199, 139, 221, 145, 186, 17, 265, 178, 35, 84, 106, 47, 7, 40, 122, 0, 254, 154, 222, 44, 16, 95, 59, 72, 111, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 29, 176, 115, 30, 78, 179, 53, 90, 218, 169, 177, 294, 11, 118, 207, 273, 140, 159, 173, 104, 264, 61, 64, 51, 168, 191, 135, 291, 198, 156, 100, 201, 96, 12, 31, 13, 119, 38, 160, 3, 280, 69, 241, 216, 77, 188, 157, 165, 174, 62, 232, 102, 5, 49, 149, 192, 37, 114, 224, 19, 248, 142, 110, 175, 93, 253, 203, 15, 289, 290, 83, 274, 202, 242, 236, 269, 155, 183, 279, 133, 99, 167, 67, 223, 89, 247, 278, 1, 26, 189, 208, 116, 50, 287, 272, 246, 255, 150, 215, 180, 45, 293, 107, 22, 210, 52, 285, 21, 234, 270, 103, 18, 252, 113, 206, 228, 227, 138, 286, 185, 28, 144, 235, 239, 256, 181, 58, 153, 27, 132, 266, 172, 32, 231, 101, 60, 48, 190, 217, 75, 288, 4, 98, 130, 6, 92, 282, 226, 225, 91, 85, 14, 23, 63, 163, 79, 171, 296, 200, 42, 197, 205, 70, 260, 41, 8, 39, 193, 267, 43, 250, 233, 117, 82, 134, 284, 249, 184, 213, 87, 128, 25, 263, 220, 94, 55, 204, 34, 211, 283, 238, 126};
// GLOBAL: FF7 0x009217c8
unsigned char g_MateriaKindById[384] = {0x1d, 0x1e, 0x21, 0x22, 0x23, 0x45, 0x2b, 0x24, 0x20, 0x26, 0x27, 0x28, 0x54, 0x2a, 0x46, 0x47, 0x1c, 0x04, 0x3f, 0x55, 0x56, 0x41, 0x09, 0x1f, 0x53, 0x4e, 0x30, 0x32, 0x33, 0x36, 0x38, 0x39, 0x3b, 0x3c, 0x3d, 0x50, 0x0f, 0x10, 0x05, 0x35, 0x3e, 0x2d, 0x3a, 0x4f, 0x48, 0x01, 0x02, 0x03, 0x59, 0x0a, 0x0b, 0x12, 0x0c, 0x0d, 0x0e, 0x13, 0x14, 0x15, 0x16, 0x17, 0x11, 0x1a, 0x18, 0x08, 0x37, 0x19, 0x06, 0x07, 0x1b, 0x40, 0x4b, 0x4c, 0x4d, 0x58, 0x25, 0x29, 0x2c, 0x2e, 0x2f, 0x31, 0x34, 0x42, 0x43, 0x44, 0x49, 0x4a, 0x51, 0x52, 0x57, 0x5a, 0x5b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x92, 0x36, 0xe0, 0x2f, 0x0f, 0x2a, 0x50, 0x2e, 0x2f, 0x32, 0xcc, 0x29, 0xe0, 0x03, 0xad, 0x31, 0xc5, 0x10, 0x8a, 0x21, 0x07, 0x19, 0x48, 0x1d, 0xe0, 0x03, 0xed, 0x29, 0x4e, 0x2e, 0x0c, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x10, 0x00, 0x20, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0x3a, 0xa6, 0xaa, 0x6c, 0xc6, 0xe9, 0xff, 0xff, 0xef, 0x6a, 0xca, 0xaa, 0xcc, 0xa3, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xaa, 0xaa, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x6c, 0xc6, 0xcb, 0xac, 0xca, 0xe9, 0xff, 0xff, 0xef, 0x6a, 0xca, 0xcc, 0xbc, 0xcc, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xae, 0xcc, 0xcc, 0xcc, 0xcc, 0xea, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0x6b, 0xb6, 0xcc, 0x6c, 0x8a, 0xab, 0xff, 0xff, 0xaf, 0x36, 0xba, 0xcb, 0xbc, 0xbb, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xae, 0xcc, 0xac, 0xcb, 0xaa, 0xca, 0xcc, 0xea, 0xff, 0xff, 0xff, 0xff, 0xaf, 0xab, 0xa6, 0x63, 0xcb, 0x56, 0x66, 0x66, 0x66, 0x66, 0x33, 0x3c, 0xca, 0xba, 0xbc, 0xfa, 0xff, 0xff, 0xff, 0xae, 0xcc, 0x6a, 0xbb, 0xcc, 0xac, 0x6c, 0xa6, 0xcc, 0xfa, 0xff, 0xff, 0xff, 0xef, 0xcb, 0x36, 0x63, 0xba, 0x66, 0xa6, 0xcc, 0x6a, 0x66, 0x68, 0x55, 0x44, 0x63, 0xbc, 0xfe, 0xff, 0xff, 0xef, 0xca, 0x6a, 0xa3, 0xcb, 0xaa, 0xbc, 0x5c, 0x63, 0xa6, 0xac, 0xff, 0xff, 0xff, 0xff, 0xba, 0x6a, 0x63, 0x66, 0x36, 0x63, 0xca, 0xac, 0x83, 0xa6, 0x86, 0x45, 0xa3, 0xab, 0xff, 0xff, 0xff, 0xae, 0xac, 0x56, 0x33, 0xc5, 0xcc, 0xac, 0x66, 0xa6, 0x63, 0xca, 0xfa, 0xff, 0xff, 0xff, 0xbe, 0xac, 0xa6, 0x68, 0x83, 0x36, 0xb6, 0x6b, 0x66, 0x63, 0x8a, 0x33, 0xca, 0xeb, 0xff};
unsigned char g_MenuItemBitmapBytes[720] = {
    0xff, 0xef, 0xca, 0x6a, 0x63, 0x38, 0xa5, 0xca, 0xac, 0xaa, 0xaa, 0x66, 0xa6, 0xac, 0xff, 0xff, 0xff, 0xaf, 0xcb, 0x6a, 0x8a, 0x66, 0xc3, 0xba,
    0x6b, 0xca, 0x63, 0x38, 0xa3, 0xbc, 0xfa, 0xff, 0xff, 0xaf, 0xac, 0x36, 0xa6, 0x58, 0xa3, 0xcc, 0xac, 0xaa, 0xca, 0xcc, 0x66, 0xca, 0xfa, 0xff,
    0xff, 0xff, 0xba, 0xac, 0x86, 0x36, 0x66, 0xca, 0xab, 0xac, 0x36, 0x33, 0xca, 0xab, 0xff, 0xff, 0xff, 0xce, 0x6a, 0x66, 0x66, 0x38, 0xca, 0xaa,
    0xca, 0xaa, 0x6a, 0xcc, 0x6c, 0xa6, 0xec, 0xff, 0xff, 0xff, 0xaf, 0xcb, 0x6a, 0x66, 0xca, 0xac, 0xac, 0xc6, 0x6c, 0xa6, 0xbc, 0xfa, 0xff, 0xff,
    0xff, 0xca, 0x36, 0xaa, 0x86, 0x68, 0xac, 0x63, 0xa6, 0xcc, 0x86, 0xa6, 0xac, 0x66, 0xac, 0xff, 0xff, 0xff, 0xff, 0xba, 0xac, 0xc6, 0xcc, 0xca,
    0x6b, 0xa6, 0xaa, 0xbc, 0xab, 0xff, 0xff, 0xff, 0xef, 0xac, 0x36, 0xba, 0x6a, 0x63, 0xaa, 0xaa, 0xaa, 0xcc, 0x38, 0x86, 0x68, 0x46, 0xca, 0xfe,
    0xff, 0xff, 0xff, 0xaf, 0xbb, 0xac, 0xaa, 0xb6, 0xab, 0x33, 0xc6, 0xbb, 0xfa, 0xff, 0xff, 0xff, 0x6f, 0xac, 0xbb, 0xcc, 0xcc, 0xac, 0x9a, 0x99,
    0xc9, 0x8c, 0x58, 0x86, 0x66, 0x45, 0xc3, 0xfa, 0xff, 0xff, 0xff, 0xff, 0xae, 0xbb, 0xcc, 0xaa, 0xaa, 0xcc, 0xbb, 0xea, 0xff, 0xff, 0xff, 0xff,
    0xaf, 0x6c, 0xcb, 0xac, 0xac, 0xaa, 0xa9, 0xee, 0xee, 0xae, 0x8a, 0x66, 0xc3, 0xaa, 0xc3, 0xfa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xae, 0xbb, 0xcc,
    0xcc, 0xbb, 0xea, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0x6a, 0xba, 0xca, 0xcc, 0xca, 0xe9, 0xff, 0xff, 0xef, 0x8c, 0xca, 0xa5, 0xab, 0xa4, 0xfc,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xca, 0xcc, 0xea, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0x6a, 0xc6, 0xac, 0x6a, 0xc6, 0xe9, 0xff,
    0xff, 0xef, 0x6a, 0xca, 0xac, 0xac, 0xa3, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xcf, 0x3a, 0xa6, 0xaa, 0x6c, 0xc6, 0xe9, 0xff, 0xff, 0xef, 0x6a, 0xca, 0xaa, 0xcc, 0xa3, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x6c, 0xc6, 0xcb, 0xac, 0xca, 0xe9, 0xff, 0xff, 0xef, 0x6a, 0xca, 0xcc, 0xbc, 0xcc, 0xfb,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xaa, 0xaa, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0x6b, 0xb6, 0xcc, 0x6c, 0x8a, 0xab, 0xff,
    0xff, 0xaf, 0x36, 0xba, 0xcb, 0xbc, 0xbb, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xae, 0xcc, 0xcc, 0xcc, 0xcc, 0xea, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xaf, 0xab, 0xa6, 0x63, 0xcb, 0x56, 0x66, 0x66, 0x66, 0x66, 0x33, 0x3c, 0xca, 0xba, 0xbc, 0xfa, 0xff, 0xff, 0xff, 0xff, 0xae, 0xcc, 0xac, 0xcb,
    0xaa, 0xca, 0xcc, 0xea, 0xff, 0xff, 0xff, 0xff, 0xef, 0xcb, 0x36, 0x63, 0xba, 0x66, 0xa6, 0xcc, 0x6a, 0x66, 0x68, 0x55, 0x44, 0x63, 0xbc, 0xfe,
    0xff, 0xff, 0xff, 0xae, 0xcc, 0x6a, 0xbb, 0xcc, 0xac, 0x6c, 0xa6, 0xcc, 0xfa, 0xff, 0xff, 0xff, 0xff, 0xba, 0x6a, 0x63, 0x66, 0x36, 0x63, 0xca,
    0xac, 0x83, 0xa6, 0x86, 0x45, 0xa3, 0xab, 0xff, 0xff, 0xff, 0xef, 0xca, 0x6a, 0xa3, 0xcb, 0xaa, 0xbc, 0x5c, 0x63, 0xa6, 0xac, 0xff, 0xff, 0xff,
    0xff, 0xbe, 0xac, 0xa6, 0x68, 0x83, 0x36, 0xb6, 0x6b, 0x66, 0x63, 0x8a, 0x33, 0xca, 0xeb, 0xff, 0xff, 0xff, 0xae, 0xac, 0x56, 0x33, 0xc5, 0xcc,
    0xac, 0x66, 0xa6, 0x63, 0xca, 0xfa, 0xff, 0xff, 0xff, 0xaf, 0xcb, 0x6a, 0x8a, 0x66, 0xc3, 0xba, 0x6b, 0xca, 0x63, 0x38, 0xa3, 0xbc, 0xfa, 0xff,
    0xff, 0xef, 0xca, 0x6a, 0x63, 0x38, 0xa5, 0xca, 0xac, 0xaa, 0xaa, 0x66, 0xa6, 0xac, 0xff, 0xff, 0xff, 0xff, 0xba, 0xac, 0x86, 0x36, 0x66, 0xca,
    0xab, 0xac, 0x36, 0x33, 0xca, 0xab, 0xff, 0xff, 0xff, 0xaf, 0xac, 0x36, 0xa6, 0x58, 0xa3, 0xcc, 0xac, 0xaa, 0xca, 0xcc, 0x66, 0xca, 0xfa, 0xff,
    0xff, 0xff, 0xaf, 0xcb, 0x6a, 0x66, 0xca, 0xac, 0xac, 0xc6, 0x6c, 0xa6, 0xbc, 0xfa, 0xff, 0xff, 0xff, 0xce, 0x6a, 0x66, 0x66, 0x38, 0xca, 0xaa,
    0xca, 0xaa, 0x6a, 0xcc, 0x6c, 0xa6, 0xec, 0xff, 0xff, 0xff, 0xff, 0xba, 0xac, 0xc6, 0xcc, 0xca, 0x6b, 0xa6, 0xaa, 0xbc, 0xab, 0xff, 0xff, 0xff,
    0xff, 0xca, 0x36, 0xaa, 0x86, 0x68, 0xac, 0x63, 0xa6, 0xcc, 0x86, 0xa6, 0xac, 0x66, 0xac, 0xff, 0xff, 0xff, 0xff, 0xaf, 0xbb, 0xac, 0xaa, 0xb6,
    0xab, 0x33, 0xc6, 0xbb, 0xfa, 0xff, 0xff, 0xff, 0xef, 0xac, 0x36, 0xba, 0x6a, 0x63, 0xaa, 0xaa, 0xaa, 0xcc, 0x38, 0x86, 0x68, 0x46, 0xca, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xae, 0xbb, 0xcc, 0xaa, 0xaa, 0xcc, 0xbb, 0xea, 0xff, 0xff, 0xff, 0xff, 0x6f, 0xac, 0xbb, 0xcc, 0xcc, 0xac, 0x9a, 0x99,
    0xc9, 0x8c, 0x58, 0x86, 0x66, 0x45, 0xc3, 0xfa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xae, 0xbb, 0xcc, 0xcc, 0xbc, 0xea, 0xff, 0xff, 0xff, 0xff, 0xff
};
// The last 8 bytes of the bitmap rows before g_MenuItemBitmapTailAndOwners; ITEM_LIMIT_MANUAL_OWNER
// indexes from its second byte and reaches the owner table at the end of that array.
// GLOBAL: FF7 0x00921c18
unsigned char g_MenuItemBitmapTail[8] = {0xaf, 0x6c, 0xcb, 0xac, 0xac, 0xaa, 0xa9, 0xee};
// Which character a limit manual (ITEM_OMNISLASH .. ITEM_HIGHWIND) belongs to, indexed by item id.
#define ITEM_LIMIT_MANUAL_OWNER(item) (((signed char *)(g_MenuItemBitmapTail + 1))[item])
unsigned char g_MenuItemBitmapTailAndOwners[88] = {
    0xee, 0xae, 0x8a, 0x66, 0xc3, 0xaa, 0xc3, 0xfa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xaa, 0xaa, 0xea, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xcf, 0x6a, 0xba, 0xca, 0xcc, 0xca, 0xe9, 0xff, 0xff, 0xef, 0x8c, 0xca, 0xa5, 0xab, 0xa4, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0x6a, 0xc6, 0xac, 0x6a, 0xc6, 0xe9, 0xff, 0xff, 0xef, 0x6a, 0xca, 0xac, 0xac, 0xa3, 0xfc,
    0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x07, 0x08
};
// GLOBAL: FF7 0x00921c78
unsigned char g_MenuItemWindowRects2x[32] = {0x00, 0x00, 0x00, 0x00, 0x80, 0x02, 0x33, 0x00, 0x00, 0x00, 0x33, 0x00, 0x80, 0x02, 0x33, 0x00, 0x00, 0x00, 0x5e, 0x00, 0x80, 0x02, 0x82, 0x01, 0xdc, 0x00, 0x1a, 0x00, 0x91, 0x00, 0xe3, 0x00};
// GLOBAL: FF7 0x00921c98
unsigned char g_MenuItemWindowRects1x[32] = {0x00, 0x00, 0x00, 0x00, 0x40, 0x01, 0x1a, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x40, 0x01, 0x1a, 0x00, 0x00, 0x00, 0x2f, 0x00, 0x40, 0x01, 0xc1, 0x00, 0x6e, 0x00, 0x0d, 0x00, 0x49, 0x00, 0x72, 0x00};
/* Contiguous 7-short scroll-bar descriptor passed to Menu_DrawScrollBar2x/1x via
 * &g_MenuItemScrollBar.visibleRows; split across per-dword GLOBALs for direct field addressing. */
// GLOBAL: FF7 0x00dd17f0
MenuScrollBarRows g_MenuItemScrollBar = {0};
// GLOBAL: FF7 0x00dd17f4
MenuScrollBarPos g_MenuItemScrollBarPos = {0};
// GLOBAL: FF7 0x00dd17f8
MenuScrollBarSize g_MenuItemScrollBarSize = {0};
// GLOBAL: FF7 0x00dd17fc
short g_MenuItemScrollBarHeight = 0;
// GLOBAL: FF7 0x00dd1800
unsigned int g_MateriaWork[0x30] = {0};
// GLOBAL: FF7 0x00dd18c0
struct EnglishWindowFrame *g_MenuItemWindowPtr = 0;
unsigned char g_bssPad_00dd18c4[4] = {0};
/* Linker symbol; code accesses this popup buffer as g_MenuItemPopupText. */
// GLOBAL: FF7 0x00dd18c8
unsigned char g_MenuItemPopupText[80] = {0};
unsigned char g_bssPad_00dd1918[176] = {0};
// GLOBAL: FF7 0x00dd19c8
int g_MenuItemState = 0;
unsigned char g_bssPad_00dd19cc[4] = {0};
// GLOBAL: FF7 0x00dd19d0
unsigned char g_MenuItemBuffer[0x40] = {0};
// The column and row (in the item list) of the first slot picked in the Arrange swap list.
// GLOBAL: FF7 0x00dd1a10
int g_MenuItemSavedColRow[2] = {0};
// The Item screen's six MenuCursor blocks, one per MenuItemScreenState: the command bar, the item list,
// the target, the key items, the Arrange choice and the swap list. The cursor of the current state is
// g_MenuItemCursors[g_MenuItemState].
// GLOBAL: FF7 0x00dd1a18
MenuCursor g_MenuItemCursors[6] = {0};
// Counts down the frames of the item list's slide while a target is chosen.
// GLOBAL: FF7 0x00dd1b68
int g_MenuItemDelay = 0;
// enum MenuItemSwapState
// GLOBAL: FF7 0x00dd1b6c
int g_MenuItemSwapState = 0;
unsigned char g_bssPad_00dd1b70[40] = {0};
// clang-format on

#pragma intrinsic(abs, memcpy, memset)

void __cdecl Savemap_AddItem(int itemRef);

/* What Savemap_BackupPartyAndEquipToStash keeps in gSavemap.stolenMateria while a character's
   equipment is swapped out. */
typedef struct MateriaStash {
    unsigned char party[3];
    unsigned char pad_03;
    unsigned char weapon;
    unsigned char armor;
    unsigned char pad_06[2];
    int weaponMateria[8];
    int armorMateria[8];
    int materia[3];
} MateriaStash;

/*
 * Sorts the item inventory by the Arrange choice mode (MenuItemArrangeMode); Customize sorts nothing.
 */
// FUNCTION: FF7 0x00714640
void __cdecl Menu_ItemArrangeBySortMode(int mode)
{
    switch (mode) {
    case MENU_ITEM_ARRANGE_CUSTOMIZE:
        break;
    case MENU_ITEM_ARRANGE_FIELD:
        Menu_ItemSortQuicksort(gSavemap.items, 0x140, Menu_CompareItemSellability, Menu_ItemSortSwapSlots);
        break;
    case MENU_ITEM_ARRANGE_BATTLE:
        Menu_ItemSortQuicksort(gSavemap.items, 0x140, Menu_ItemSortCompareByBattle, Menu_ItemSortSwapSlots);
        break;
    case MENU_ITEM_ARRANGE_THROW:
        Menu_ItemSortQuicksort(gSavemap.items, 0x140, Menu_ItemSortCompareByThrow, Menu_ItemSortSwapSlots);
        break;
    case MENU_ITEM_ARRANGE_TYPE:
        Menu_ItemSortQuicksort(gSavemap.items, 0x140, Menu_ItemSortCompareByType, Menu_ItemSortSwapSlots);
        break;
    case MENU_ITEM_ARRANGE_NAME:
        Menu_ItemSortQuicksort(gSavemap.items, 0x140, Menu_CompareItemsByName, Menu_ItemSortSwapSlots);
        break;
    case MENU_ITEM_ARRANGE_MOST:
        Menu_ItemSortQuicksort(gSavemap.items, 0x140, Menu_ItemSortCompareByMost, Menu_ItemSortSwapSlots);
        break;
    case MENU_ITEM_ARRANGE_LEAST:
        Menu_ItemSortQuicksort(gSavemap.items, 0x140, Menu_ItemSortCompareByLeast, Menu_ItemSortSwapSlots);
        break;
    }
}

/*
 * Quicksort implementation for inventory item rearrangement.
 */
// FUNCTION: FF7 0x0071475d
short __cdecl Menu_ItemSortQuicksort(unsigned short *base, unsigned int count, short(__cdecl *cmp)(short, short, unsigned short **), void(__cdecl *swap)(short, short, unsigned short **))
{
    unsigned int rightStack[64];
    unsigned int stackTop;
    unsigned int lower;
    unsigned int scanRight;
    unsigned int scanLeft;
    unsigned int stackLo[64];

    if (count < 2) {
        return 1;
    }
    stackTop = 1;
    stackLo[0] = 0;
    rightStack[0] = count - 1;
    while (stackTop--) {
        lower = stackLo[stackTop];
        scanLeft = lower + 1;
        scanRight = rightStack[stackTop];
        count = scanRight;
        while (scanLeft < scanRight) {
            while (scanLeft < scanRight && cmp((short)scanLeft, (short)lower, &base) < 1) {
                scanLeft++;
            }
            while (scanLeft <= scanRight && cmp((short)lower, (short)scanRight, &base) < 1) {
                scanRight--;
            }
            if (scanLeft < scanRight) {
                swap((short)scanLeft++, (short)scanRight--, &base);
            }
        }
        if (cmp((short)lower, (short)scanRight, &base) > 0) {
            swap((short)lower, (short)scanRight, &base);
        }
        if (scanRight > lower) {
            scanRight--;
        }
        if (scanRight > lower && count > scanLeft && scanRight - lower < count - scanLeft) {
            Menu_ItemSortSwapRangeBounds((int *)&scanRight, (int *)&count);
            Menu_ItemSortSwapRangeBounds((int *)&lower, (int *)&scanLeft);
        }
        if (scanRight > lower) {
            stackLo[stackTop] = lower;
            rightStack[stackTop] = scanRight;
            stackTop++;
        }
        if (count > scanLeft) {
            stackLo[stackTop] = scanLeft;
            rightStack[stackTop] = count;
            stackTop++;
        }
        if (stackTop >= 64) {
            return 0;
        }
    }
    return 1;
}

/*
 * Swaps sort boundary bounds during item partitioning.
 */
// FUNCTION: FF7 0x00714a39
void __cdecl Menu_ItemSortSwapRangeBounds(int *first, int *second)
{
    int tmp;

    tmp = *first;
    *first = *second;
    *second = tmp;
}

/*
 * Compares two items by category/type precedence.
 */
// FUNCTION: FF7 0x00714a5b
short __cdecl Menu_ItemSortCompareByType(short idx0, short idx1, unsigned short **base)
{
    unsigned short typeA;
    unsigned short typeB;

    /* The 0x1ff mask is applied when each type is stored, not to the difference. */
    typeA = (*base)[idx0] & 0x1FF;
    typeB = (*base)[idx1] & 0x1FF;
    return Menu_ItemSortSign(typeA - typeB);
}

/*
 * Returns signum (-1, 0, 1) of integer comparison value.
 */
// FUNCTION: FF7 0x00714ab3
int __cdecl Menu_ItemSortSign(int val)
{
    /* Written as `!= 0` with a ternary in this order because that is the code VC5 emits for it. */
    if (val != 0) {
        return (val < 0) ? -1 : 1;
    }
    return 0;
}

/*
 * Compares two items by quantity descending.
 */
// FUNCTION: FF7 0x00714ad1
short __cdecl Menu_ItemSortCompareByMost(short idx0, short idx1, unsigned short **base)
{
    short ka;
    short kb;

    if ((*base)[idx0] == MENU_ITEM_SLOT_EMPTY) {
        ka = 0;
    } else {
        ka = (*base)[idx0] >> 9;
    }
    if ((*base)[idx1] == MENU_ITEM_SLOT_EMPTY) {
        kb = 0;
    } else {
        kb = (*base)[idx1] >> 9;
    }
    return Menu_ItemSortSign(kb - ka);
}

/*
 * Compares two items by quantity ascending.
 */
// FUNCTION: FF7 0x00714b58
short __cdecl Menu_ItemSortCompareByLeast(short idx0, short idx1, unsigned short **base)
{
    /* The keys are 16-bit (unsigned short) because int locals would change the code. */
    unsigned short ka;
    unsigned short kb;

    if ((*base)[idx0] == MENU_ITEM_SLOT_EMPTY) {
        ka = 20000;
    } else {
        ka = (*base)[idx0] >> 9;
    }
    if ((*base)[idx1] == MENU_ITEM_SLOT_EMPTY) {
        kb = 20000;
    } else {
        kb = (*base)[idx1] >> 9;
    }
    return Menu_ItemSortSign(ka - kb);
}

/*
 * Compares two items alphabetically by name.
 */
// FUNCTION: FF7 0x00714be8
short __cdecl Menu_CompareItemsByName(short idx0, short idx1, unsigned short **base)
{
    /* The keys are 16-bit (unsigned short) because int locals would change the code. */
    unsigned short ka;
    unsigned short kb;

    if ((*base)[idx0] == MENU_ITEM_SLOT_EMPTY) {
        ka = 20000;
    } else {
        ka = g_ItemMateriaSortKeys[(*base)[idx0] & 0x1FF];
    }
    if ((*base)[idx1] == MENU_ITEM_SLOT_EMPTY) {
        kb = 20000;
    } else {
        kb = g_ItemMateriaSortKeys[(*base)[idx1] & 0x1FF];
    }
    return Menu_ItemSortSign(ka - kb);
}

/*
 * Compares items for Arrange > Field: items usable from the menu sort first.
 */
// FUNCTION: FF7 0x00714c8d
short __cdecl Menu_CompareItemSellability(short idx0, short idx1, unsigned short **base)
{
    short ka;
    short kb;

    if ((*base)[idx0] == MENU_ITEM_SLOT_EMPTY) {
        ka = 0;
    } else {
        ka = (Kernel_GetEquipRestrictions((*base)[idx0] & 0x1FF) & MENU_ITEM_RESTRICT_NO_MENU_USE) ? 1 : 2;
    }
    if ((*base)[idx1] == MENU_ITEM_SLOT_EMPTY) {
        kb = 0;
    } else {
        kb = (Kernel_GetEquipRestrictions((*base)[idx1] & 0x1FF) & MENU_ITEM_RESTRICT_NO_MENU_USE) ? 1 : 2;
    }
    return Menu_ItemSortSign(kb - ka);
}

/*
 * Compares items for Arrange > Battle: items usable in battle sort first.
 */
// FUNCTION: FF7 0x00714d3f
short __cdecl Menu_ItemSortCompareByBattle(short idx0, short idx1, unsigned short **base)
{
    short ka;
    short kb;

    if ((*base)[idx0] == MENU_ITEM_SLOT_EMPTY) {
        ka = 0;
    } else {
        ka = (Kernel_GetEquipRestrictions((*base)[idx0] & 0x1FF) & MENU_ITEM_RESTRICT_NO_BATTLE_USE) ? 1 : 2;
    }
    if ((*base)[idx1] == MENU_ITEM_SLOT_EMPTY) {
        kb = 0;
    } else {
        kb = (Kernel_GetEquipRestrictions((*base)[idx1] & 0x1FF) & MENU_ITEM_RESTRICT_NO_BATTLE_USE) ? 1 : 2;
    }
    return Menu_ItemSortSign(kb - ka);
}

/*
 * Compares items for Arrange > Throw: throwable items sort first.
 */
// FUNCTION: FF7 0x00714df1
short __cdecl Menu_ItemSortCompareByThrow(short idx0, short idx1, unsigned short **base)
{
    short ka;
    short kb;

    if ((*base)[idx0] == MENU_ITEM_SLOT_EMPTY) {
        ka = 0;
    } else {
        ka = (Kernel_GetEquipRestrictions((*base)[idx0] & 0x1FF) & MENU_ITEM_RESTRICT_NO_THROW) ? 1 : 2;
    }
    if ((*base)[idx1] == MENU_ITEM_SLOT_EMPTY) {
        kb = 0;
    } else {
        kb = (Kernel_GetEquipRestrictions((*base)[idx1] & 0x1FF) & MENU_ITEM_RESTRICT_NO_THROW) ? 1 : 2;
    }
    return Menu_ItemSortSign(kb - ka);
}

/*
 * Swaps two item inventory slots in place.
 */
// FUNCTION: FF7 0x00714ea3
void __cdecl Menu_ItemSortSwapSlots(short idx0, short idx1, unsigned short **base)
{
    Menu_SwapU16Values(&(*base)[idx0], &(*base)[idx1]);
}

/*
 * Helper routine to swap two 16-bit values in place.
 */
// FUNCTION: FF7 0x00714eca
void __cdecl Menu_SwapU16Values(unsigned short *first, unsigned short *second)
{
    unsigned short tmp;

    tmp = *first;
    *first = *second;
    *second = tmp;
}

/*
 * Initializes item menu screen state, cursors, and views.
 */
// FUNCTION: FF7 0x00714ef2
void __cdecl Menu_ItemScreenInit(int arg)
{
    g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
    MenuCursor_Init(g_MenuItemCursors, 0, 0, 3, 1, 0, 0, 3, 1, 0, 0, 1, 0, 0,
                    0);
    MenuCursor_Init(&g_MenuItemCursors[1], 0, 0, 1, 10, 0, 0, 1, 320, 0, 0, 0,
                    0, 0, 1);
    MenuCursor_Init(&g_MenuItemCursors[2], 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 1, 0,
                    0);
    Menu_BuildOwnedKeyItemList();
    if (g_MenuIsSmallViewport320x240 != 1) {
        g_MenuItemWindowPtr = (EnglishWindowFrame *)g_MenuItemWindowRects2x;
    } else {
        g_MenuItemWindowPtr = (EnglishWindowFrame *)g_MenuItemWindowRects1x;
    }
}

/*
 * Builds list of currently owned key items.
 */
// FUNCTION: FF7 0x00714fa3
void __cdecl Menu_BuildOwnedKeyItemList(void)
{
    int n;
    int bit;

    for (n = bit = 0; bit < 0x40; bit++) {
        if ((gSavemap.keyItems[bit / 8] & (1 << (bit & 7))) != 0) {
            g_MenuItemBuffer[n] = bit;
            n++;
        }
    }
    for (; n < 64; n++) {
        g_MenuItemBuffer[n] = 0xff;
    }
}

/*
 * Verifies character limit mask against attack slots.
 */
// FUNCTION: FF7 0x00715026
int __cdecl Menu_CharacterLimitMaskMatchesAttackSlots(int charIdx)
{
    int mask;
    int n;

    for (mask = n = 0; n < 10; n++) {
        if (Kernel_GetLimitAttackId(charIdx, n) != 0x7f) {
            mask |= 1 << n;
        }
    }
    mask &= ~0x200;
    if (gSavemap.chars[charIdx].limits == mask) {
        return 1;
    }
    return 0;
}

/*
 * Returns the item's restriction bits, adding MENU_ITEM_RESTRICT_NO_MENU_USE for the Save Crystal
 * (unless saveCrystalFlags bit 1 allows it) and for the Tent while Save is locked.
 */
// FUNCTION: FF7 0x007150a0
int __cdecl Menu_GetItemUseRestrictions(int item)
{
    int flags;

    flags = Kernel_GetEquipRestrictions(item);
    switch (item) {
    case ITEM_SAVE_CRYSTAL:
        if ((gSavemap.saveCrystalFlags & 2) == 0) {
            flags |= MENU_ITEM_RESTRICT_NO_MENU_USE;
        }
        break;
    case ITEM_TENT:
        if ((g_MenuLockedMask & 1 << MENU_MAIN_COMMAND_SAVE) != 0) {
            flags |= MENU_ITEM_RESTRICT_NO_MENU_USE;
        }
        break;
    }
    return flags;
}

/*
 * Main per-frame update and input dispatcher for item menu.
 */
// FUNCTION: FF7 0x00715105
void __cdecl Menu_ItemScreenFrame(unsigned int frameCounter)
{
    /* Unprototyped retail call pushes 32-bit int amount without narrow short sign-extension; callee reads 16-bit low word. */
    void __cdecl Savemap_HealMp(int, int);
    struct {
        int index;
        int actor;
        int item;
        int rows;
        int row;
        int y;
        int column;
        int quantity;
        int canHeal;
        int i;
        int restrictions;
        unsigned short swap;
        unsigned short unused;
        int cursor;
        short rect[4];
        unsigned char *text;
        int colour;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (g_MenuIsSmallViewport320x240 != 1) {
        Menu_DrawCommandWindow2x(g_MenuModuleId);
        if (g_MenuItemState == MENU_ITEM_STATE_SELECT_TARGET) {
            if (g_MenuItemCursors[1].col == 0)
                v.index = g_MenuItemDelay * 64 + 320;
            else
                v.index = g_MenuItemDelay * -64;
            if (g_MenuItemDelay == 0) {
                switch (gSavemap.items[g_MenuItemCursors[1].row + g_MenuItemCursors[1].scrollTop] & 0x1ff) {
                case ITEM_MEGALIXIR:
                case ITEM_TENT:
                    Menu_DrawMoreTextArrow2x(0, ((int)frameCounter % 3) * 120 + 161, 0.0f);
                    break;
                default:
                    Menu_DrawMoreTextArrow2x(0, g_MenuItemCursors[2].row * 120 + 161, 0.0f);
                    break;
                }
            }
            if (frameCounter & 2)
                Menu_DrawMoreTextArrow2x(298, g_MenuItemCursors[1].row * 37 + 109, 0.1f);
            if (g_MenuItemDelay != 0)
                g_MenuItemDelay--;
        }
        Menu_EmptyTextHook(128);
        switch (g_MenuItemState) {
        case MENU_ITEM_STATE_COMMAND_BAR:
            Menu_DrawMoreTextArrow2x(g_MenuItemCursors[0].col * 93 + 13, 26, 0.1f);
            break;
        case MENU_ITEM_STATE_ITEM_LIST:
            if (frameCounter & 2)
                Menu_DrawMoreTextArrow2x(g_MenuItemCursors[0].col * 93 + 13, 26, 0.1f);
            Menu_DrawMoreTextArrow2x(298, g_MenuItemCursors[1].row * 37 + 109, 0.1f);
            v.item = g_MenuItemCursors[1].row + g_MenuItemCursors[1].scrollTop;
            if (gSavemap.items[v.item] != MENU_ITEM_SLOT_EMPTY)
                Menu_DrawEncodedString2x(27, 64, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, gSavemap.items[v.item] & 0x1ff, 0), 7, 0.101f);
            break;
        case MENU_ITEM_STATE_SELECT_TARGET:
            if (frameCounter & 2)
                Menu_DrawMoreTextArrow2x(g_MenuItemCursors[0].col * 93 + 13, 26, 0.0f);
            v.item = g_MenuItemCursors[1].row + g_MenuItemCursors[1].scrollTop;
            if (gSavemap.items[v.item] != MENU_ITEM_SLOT_EMPTY)
                Menu_DrawEncodedString2x(27, 64, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, gSavemap.items[v.item] & 0x1ff, 0), 7, 0.101f);
            break;
        case MENU_ITEM_STATE_KEY_ITEMS:
            if (frameCounter & 2)
                Menu_DrawMoreTextArrow2x(g_MenuItemCursors[0].col * 93 + 13, 26, 0.001f);
            Menu_DrawMoreTextArrow2x(g_MenuItemCursors[3].col * 293 + 5, g_MenuItemCursors[3].row * 36 + 129, 0.001f);
            v.item = g_MenuItemCursors[3].col + g_MenuItemCursors[3].scrollTop * 2 + g_MenuItemCursors[3].row * 2;
            if (g_MenuItemBuffer[v.item] != 255)
                Menu_DrawEncodedString2x(27, 64, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_KEY_ITEM, g_MenuItemBuffer[v.item], 0), 7, 0.101f);
            break;
        case MENU_ITEM_STATE_ARRANGE_CHOICE:
            if (frameCounter & 2)
                Menu_DrawMoreTextArrow2x(g_MenuItemCursors[0].col * 93 + 13, 26, 0.001f);
            Menu_DrawMoreTextArrow2x(g_MenuItemWindowPtr[3].x - 30, g_MenuItemWindowPtr[3].y + 17 + g_MenuItemCursors[4].row * 26, 0.001f);
            for (v.i = 0; v.i < 8; v.i++)
                Menu_DrawEncodedString2x(g_MenuItemWindowPtr[3].x + 13, g_MenuItemWindowPtr[3].y + 13 + v.i * 26, g_MenuItemLabelTable + (v.i + 3) * 12, 7, 0.01f);
            Menu_DrawWindowFrame2x(&g_MenuItemWindowPtr[3], 0.01f);
            break;
        case MENU_ITEM_STATE_SWAP_LIST:
            if (frameCounter & 2)
                Menu_DrawMoreTextArrow2x(g_MenuItemCursors[0].col * 93 + 13, 26, 0.0f);
            v.item = g_MenuItemCursors[5].row + g_MenuItemCursors[5].scrollTop;
            if (gSavemap.items[v.item] != MENU_ITEM_SLOT_EMPTY)
                Menu_DrawEncodedString2x(27, 64, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, gSavemap.items[v.item] & 0x1ff, 0), 7, 0.101f);
            break;
        }
        if (g_MenuItemCursors[0].col != 2) {
            for (v.i = 0; v.i < 3; v.i++) {
                if (gSavemap.party[v.i] != PARTY_CHAR_NONE) {
                    Menu_DrawCharStatBlock2x(133, v.i * 120 + 126, v.i, 0.1f);
                    Menu_DrawCharPortrait2x(37, v.i * 120 + 116, v.i, 0.1f);
                }
            }
            Menu_SetRectShorts(v.rect, 0, 96, 300, 384);
            Menu_DrawWindowFrame2x((EnglishWindowFrame *)v.rect, 0.1f);
        }
        for (v.row = 0; v.row < 3; v.row++)
            Menu_DrawEncodedString2x(v.row * 93 + 57, 17, g_MenuItemLabelTable + v.row * 12, 7, 0.101f);
        Menu_FlushAndSetScissor2x(0, 102, 640, 372);
        if (g_MenuItemCursors[0].col != 2) {
            if (g_MenuItemState == MENU_ITEM_STATE_SWAP_LIST) {
                if (g_MenuItemSwapState != MENU_ITEM_SWAP_PICK_FIRST && (frameCounter & 2)) {
                    v.y = g_MenuItemSavedColRow[1] * 37 - g_MenuItemCursors[5].scrollTop * 37 - 9 + g_MenuItemCursors[5].scrollAnim * 9;
                    if (v.y > -37 && v.y < 1369)
                        Menu_DrawMoreTextArrow2x(291, v.y + 113, 0.0f);
                }
                Menu_DrawMoreTextArrow2x(298, g_MenuItemCursors[5].row * 37 + 113, 0.01f);
                v.cursor = 5;
            } else
                v.cursor = 1;
            g_MenuItemScrollBar.visibleRows = 10;
            g_MenuItemScrollBar.totalRows = 320;
            g_MenuItemScrollBarPos.scrollTop = (short)g_MenuItemCursors[v.cursor].scrollTop;
            g_MenuItemScrollBarPos.x = 618;
            g_MenuItemScrollBarSize.y = 102;
            g_MenuItemScrollBarSize.width = 17;
            g_MenuItemScrollBarHeight = 372;
            Menu_DrawScrollBar2x(&g_MenuItemScrollBar.visibleRows, 0.1f);
            v.rows = (g_MenuItemCursors[v.cursor].scrollState != 0) + 10;
            Menu_EmptyTextHook(9);
            for (v.row = 0; v.row < v.rows; v.row++) {
                v.index = g_MenuItemCursors[v.cursor].scrollTop + v.row;
                if (gSavemap.items[v.index] != MENU_ITEM_SLOT_EMPTY) {
                    v.item = gSavemap.items[v.index] & 0x1ff;
                    v.restrictions = Menu_GetItemUseRestrictions(v.item);
                    v.colour = (v.restrictions & MENU_ITEM_RESTRICT_NO_MENU_USE) ? 0 : 7;
                    v.text = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, v.item, 8);
                    Menu_DrawEncodedString2x(373, v.row * 37 + 109 + g_MenuItemCursors[v.cursor].scrollAnim * 9, v.text, (unsigned char)v.colour, 0.101f);
                }
            }
            for (v.row = 0; v.row < v.rows; v.row++) {
                v.index = g_MenuItemCursors[v.cursor].scrollTop + v.row;
                if (gSavemap.items[v.index] != MENU_ITEM_SLOT_EMPTY) {
                    v.item = gSavemap.items[v.index] & 0x1ff;
                    v.quantity = gSavemap.items[v.index] >> 9;
                    v.restrictions = Menu_GetItemUseRestrictions(v.item);
                    v.colour = (v.restrictions & MENU_ITEM_RESTRICT_NO_MENU_USE) ? 0 : 7;
                    Menu_DrawItemScreenTypeIcon2x(343, v.row * 37 + 105 + g_MenuItemCursors[v.cursor].scrollAnim * 9, v.item, 0, 0.101f);
                    DrawIconGlyph16_2x(548, v.row * 37 + 114 + g_MenuItemCursors[v.cursor].scrollAnim * 9, 0xd5, (unsigned char)v.colour, 0.101f);
                    Menu_DrawDecimalNumber2x(550, v.row * 37 + 112 + g_MenuItemCursors[v.cursor].scrollAnim * 9, v.quantity, 3, (unsigned char)v.colour, 0.101f);
                }
            }
        } else {
            g_MenuItemScrollBar.visibleRows = 10;
            g_MenuItemScrollBar.totalRows = 32;
            g_MenuItemScrollBarPos.scrollTop = (short)(g_MenuItemCursors[3].scrollTop);
            g_MenuItemScrollBarPos.x = 618;
            g_MenuItemScrollBarSize.y = 102;
            g_MenuItemScrollBarSize.width = 17;
            g_MenuItemScrollBarHeight = 372;
            Menu_DrawScrollBar2x(&g_MenuItemScrollBar.visibleRows, 0.1f);
            for (v.row = 0; v.row < 12; v.row++) {
                v.index = g_MenuItemCursors[3].scrollTop * 2 + v.row * 2;
                for (v.column = 0; v.column < 2; v.column++) {
                    v.item = v.index + v.column;
                    if (g_MenuItemBuffer[v.item] != 255) {
                        v.text = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_KEY_ITEM, g_MenuItemBuffer[v.item], 8);
                        Menu_DrawEncodedString2x(v.column * 293 + 53, v.row * 36 + 124 + g_MenuItemCursors[3].scrollAnim * 9, v.text, 7, 0.101f);
                    }
                }
            }
        }
        Menu_FlushAndResetScissor2x();
        Menu_DrawWindowFrame2x(g_MenuItemWindowPtr, 0.111f);
        Menu_DrawWindowFrame2x(&g_MenuItemWindowPtr[1], 0.2f);
        Menu_DrawWindowFrame2x(&g_MenuItemWindowPtr[2], 0.3f);
    } else {
        Menu_DrawCommandWindow1x(g_MenuModuleId);
        if (g_MenuItemState == MENU_ITEM_STATE_SELECT_TARGET) {
            if (g_MenuItemCursors[1].col == 0)
                v.index = g_MenuItemDelay * 32 + 160;
            else
                v.index = g_MenuItemDelay * -32;
            if (g_MenuItemDelay == 0) {
                switch (gSavemap.items[g_MenuItemCursors[1].row + g_MenuItemCursors[1].scrollTop] & 0x1ff) {
                case ITEM_MEGALIXIR:
                case ITEM_TENT:
                    Menu_DrawMoreTextArrow1x(0, ((int)frameCounter % 3) * 60 + 81, 0.0f);
                    break;
                default:
                    Menu_DrawMoreTextArrow1x(0, g_MenuItemCursors[2].row * 60 + 81, 0.0f);
                    break;
                }
            }
            if (frameCounter & 2)
                Menu_DrawMoreTextArrow1x(149, g_MenuItemCursors[1].row * 19 + 55, 0.1f);
            if (g_MenuItemDelay != 0)
                g_MenuItemDelay--;
        }
        Menu_EmptyTextHook(128);
        switch (g_MenuItemState) {
        case MENU_ITEM_STATE_COMMAND_BAR:
            Menu_DrawMoreTextArrow1x(g_MenuItemCursors[0].col * 47 + 7, 13, 0.1f);
            break;
        case MENU_ITEM_STATE_ITEM_LIST:
            if (frameCounter & 2)
                Menu_DrawMoreTextArrow1x(g_MenuItemCursors[0].col * 47 + 7, 13, 0.1f);
            Menu_DrawMoreTextArrow1x(149, g_MenuItemCursors[1].row * 19 + 55, 0.1f);
            v.item = g_MenuItemCursors[1].row + g_MenuItemCursors[1].scrollTop;
            if (gSavemap.items[v.item] != MENU_ITEM_SLOT_EMPTY)
                Menu_DrawEncodedString1x(14, 32, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, gSavemap.items[v.item] & 0x1ff, 0), 7, 0.101f);
            break;
        case MENU_ITEM_STATE_SELECT_TARGET:
            if (frameCounter & 2)
                Menu_DrawMoreTextArrow1x(g_MenuItemCursors[0].col * 47 + 7, 13, 0.0f);
            v.item = g_MenuItemCursors[1].row + g_MenuItemCursors[1].scrollTop;
            if (gSavemap.items[v.item] != MENU_ITEM_SLOT_EMPTY)
                Menu_DrawEncodedString1x(14, 32, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, gSavemap.items[v.item] & 0x1ff, 0), 7, 0.101f);
            break;
        case MENU_ITEM_STATE_KEY_ITEMS:
            if (frameCounter & 2)
                Menu_DrawMoreTextArrow1x(g_MenuItemCursors[0].col * 47 + 7, 13, 0.0f);
            Menu_DrawMoreTextArrow1x(g_MenuItemCursors[3].col * 146 + 3, g_MenuItemCursors[3].row * 18 + 65, 0.0f);
            v.item = g_MenuItemCursors[3].col + g_MenuItemCursors[3].scrollTop * 2 + g_MenuItemCursors[3].row * 2;
            if (g_MenuItemBuffer[v.item] != 255)
                Menu_DrawEncodedString1x(14, 32, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_KEY_ITEM, g_MenuItemBuffer[v.item], 0), 7, 0.101f);
            break;
        case MENU_ITEM_STATE_ARRANGE_CHOICE:
            if (frameCounter & 2)
                Menu_DrawMoreTextArrow1x(g_MenuItemCursors[0].col * 47 + 7, 13, 0.0f);
            Menu_DrawMoreTextArrow1x(g_MenuItemWindowPtr[3].x - 15, g_MenuItemWindowPtr[3].y + 9 + g_MenuItemCursors[4].row * 13, 0.0f);
            for (v.i = 0; v.i < 8; v.i++)
                Menu_DrawEncodedString1x(g_MenuItemWindowPtr[3].x + 7, g_MenuItemWindowPtr[3].y + 7 + v.i * 13, g_MenuItemLabelTable + (v.i + 3) * 12, 7, 0.01f);
            Menu_DrawWindowFrame1x(&g_MenuItemWindowPtr[3], 0.01f);
            break;
        case MENU_ITEM_STATE_SWAP_LIST:
            if (frameCounter & 2)
                Menu_DrawMoreTextArrow1x(g_MenuItemCursors[0].col * 47 + 7, 13, 0.0f);
            v.item = g_MenuItemCursors[5].row + g_MenuItemCursors[5].scrollTop;
            if (gSavemap.items[v.item] != MENU_ITEM_SLOT_EMPTY)
                Menu_DrawEncodedString1x(14, 32, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, gSavemap.items[v.item] & 0x1ff, 0), 7, 0.101f);
            break;
        }
        Menu_EmptyTextHook(8);
        if (g_MenuItemCursors[0].col != 2) {
            for (v.i = 0; v.i < 3; v.i++) {
                if (gSavemap.party[v.i] != PARTY_CHAR_NONE) {
                    Menu_DrawCharStatBlock1x(67, v.i * 60 + 63, v.i, 0.1f);
                    Menu_DrawCharPortrait1x(19, v.i * 60 + 58, v.i, 0.1f);
                }
            }
            Menu_SetRectShorts(v.rect, 0, 48, 150, 192);
            Menu_DrawWindowFrame1x((EnglishWindowFrame *)v.rect, 0.1f);
        }
        for (v.row = 0; v.row < 3; v.row++)
            Menu_DrawEncodedString1x(v.row * 47 + 29, 9, g_MenuItemLabelTable + v.row * 12, 7, 0.101f);
        Menu_FlushAndSetScissor1x(0, 51, 320, 186);
        if (g_MenuItemCursors[0].col != 2) {
            if (g_MenuItemState == MENU_ITEM_STATE_SWAP_LIST) {
                if (g_MenuItemSwapState != MENU_ITEM_SWAP_PICK_FIRST && (frameCounter & 2)) {
                    v.y = g_MenuItemSavedColRow[1] * 19 - g_MenuItemCursors[5].scrollTop * 19 - 5 + g_MenuItemCursors[5].scrollAnim * 5;
                    if (v.y > -19 && v.y < 361)
                        Menu_DrawMoreTextArrow1x(145, v.y + 57, 0.0f);
                }
                Menu_DrawMoreTextArrow1x(149, g_MenuItemCursors[5].row * 19 + 57, 0.01f);
                v.cursor = 5;
            } else
                v.cursor = 1;
            g_MenuItemScrollBar.visibleRows = 10;
            g_MenuItemScrollBar.totalRows = 320;
            g_MenuItemScrollBarPos.scrollTop = (short)g_MenuItemCursors[v.cursor].scrollTop;
            g_MenuItemScrollBarPos.x = 309;
            g_MenuItemScrollBarSize.y = 51;
            g_MenuItemScrollBarSize.width = 9;
            g_MenuItemScrollBarHeight = 186;
            Menu_DrawScrollBar1x(&g_MenuItemScrollBar.visibleRows, 0.1f);
            v.rows = (g_MenuItemCursors[v.cursor].scrollState != 0) + 10;
            Menu_EmptyTextHook(9);
            for (v.row = 0; v.row < v.rows; v.row++) {
                v.index = g_MenuItemCursors[v.cursor].scrollTop + v.row;
                if (gSavemap.items[v.index] != MENU_ITEM_SLOT_EMPTY) {
                    v.item = gSavemap.items[v.index] & 0x1ff;
                    v.restrictions = Menu_GetItemUseRestrictions(v.item);
                    v.colour = (v.restrictions & MENU_ITEM_RESTRICT_NO_MENU_USE) ? 0 : 7;
                    v.text = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, v.item, 8);
                    Menu_DrawEncodedString1x(186, v.row * 19 + 55 + g_MenuItemCursors[v.cursor].scrollAnim * 5, v.text, (unsigned char)v.colour, 0.101f);
                }
            }
            for (v.row = 0; v.row < v.rows; v.row++) {
                v.index = g_MenuItemCursors[v.cursor].scrollTop + v.row;
                if (gSavemap.items[v.index] != MENU_ITEM_SLOT_EMPTY) {
                    v.item = gSavemap.items[v.index] & 0x1ff;
                    v.quantity = gSavemap.items[v.index] >> 9;
                    v.restrictions = Menu_GetItemUseRestrictions(v.item);
                    v.colour = (v.restrictions & MENU_ITEM_RESTRICT_NO_MENU_USE) ? 0 : 7;
                    Menu_DrawItemScreenTypeIcon1x(171, v.row * 19 + 53 + g_MenuItemCursors[v.cursor].scrollAnim * 5, v.item, 0, 0.101f);
                    DrawIconGlyph16_1x(274, v.row * 19 + 57 + g_MenuItemCursors[v.cursor].scrollAnim * 5, 0xd5, (unsigned char)v.colour, 0.101f);
                    Menu_DrawDecimalNumber1x(275, v.row * 19 + 56 + g_MenuItemCursors[v.cursor].scrollAnim * 5, v.quantity, 3, (unsigned char)v.colour, 0.101f);
                }
            }
        } else {
            g_MenuItemScrollBar.visibleRows = 10;
            g_MenuItemScrollBar.totalRows = 32;
            g_MenuItemScrollBarPos.scrollTop = (short)(g_MenuItemCursors[3].scrollTop);
            g_MenuItemScrollBarPos.x = 309;
            g_MenuItemScrollBarSize.y = 51;
            g_MenuItemScrollBarSize.width = 9;
            g_MenuItemScrollBarHeight = 186;
            Menu_DrawScrollBar1x(&g_MenuItemScrollBar.visibleRows, 0.1f);
            Menu_EmptyTextHook(9);
            for (v.row = 0; v.row < 12; v.row++) {
                v.index = g_MenuItemCursors[3].scrollTop * 2 + v.row * 2;
                for (v.column = 0; v.column < 2; v.column++) {
                    v.item = v.index + v.column;
                    if (g_MenuItemBuffer[v.item] != 255) {
                        v.text = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_KEY_ITEM, g_MenuItemBuffer[v.item], 8);
                        Menu_DrawEncodedString1x(v.column * 146 + 27, v.row * 18 + 62 + g_MenuItemCursors[3].scrollAnim * 5, v.text, 7, 0.101f);
                    }
                }
            }
        }
        Menu_FlushAndResetScissor1x();
        Menu_DrawWindowFrame1x(g_MenuItemWindowPtr, 0.111f);
        Menu_DrawWindowFrame1x(&g_MenuItemWindowPtr[1], 0.2f);
        Menu_DrawWindowFrame1x(&g_MenuItemWindowPtr[2], 0.3f);
    }
    if (Menu_GetPhase() == 0) {
        MenuCursor_Update(&g_MenuItemCursors[g_MenuItemState]);
        switch (g_MenuItemState) {
        case MENU_ITEM_STATE_COMMAND_BAR:
            if (Input_GetPressed(INPUT_CIRCLE)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                switch (g_MenuItemCursors[0].col) {
                case MENU_ITEM_COMMAND_USE:
                    g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                    break;
                case MENU_ITEM_COMMAND_ARRANGE:
                    MenuCursor_Init(&g_MenuItemCursors[4], 0, 0, 1, 8, 0, 0, 1, 8, 0, 0, 0, 1, 0, 0);
                    g_MenuItemState = MENU_ITEM_STATE_ARRANGE_CHOICE;
                    break;
                case MENU_ITEM_COMMAND_KEY_ITEMS:
                    MenuCursor_Init(&g_MenuItemCursors[3], 0, 0, 2, 10, 0, 0, 2, 32, 0, 0, 2, 0, 0, 1);
                    g_MenuItemState = MENU_ITEM_STATE_KEY_ITEMS;
                    break;
                }
            } else if (BtlMenu_GetInputRepeat(INPUT_CROSS)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                Menu_SetPhase(5, 0);
                Menu_SetScreen(0);
            }
            break;
        case MENU_ITEM_STATE_ITEM_LIST:
            if (g_MenuItemCursors[1].scrollState == 0) {
                if (Input_GetPressed(INPUT_CIRCLE)) {
                    v.item = g_MenuItemCursors[1].row + g_MenuItemCursors[1].scrollTop;
                    if (gSavemap.items[v.item] != MENU_ITEM_SLOT_EMPTY) {
                        v.item = gSavemap.items[v.item] & 0x1ff;
                        v.restrictions = Menu_GetItemUseRestrictions(v.item);
                        if (!(v.restrictions & MENU_ITEM_RESTRICT_NO_MENU_USE)) {
                            switch (v.item) {
                            case ITEM_EARTH_HARP:
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                                gSavemap.chars[6].level = 1;
                                gSavemap.chars[6].id = PARTY_CHAR_CAIT_SITH;
                                gSavemap.chars[6].limitLevel = 1;
                                gSavemap.chars[6].limitBar = 255;
                                gSavemap.chars[6].exp = 0x00ffffff;
                                gSavemap.chars[7].level = 1;
                                gSavemap.chars[7].id = PARTY_CHAR_VINCENT;
                                gSavemap.chars[7].limitLevel = 1;
                                gSavemap.chars[7].limitBar = 255;
                                gSavemap.chars[7].exp = 0x00ffffff;
                                break;
                            case ITEM_SAVE_CRYSTAL:
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                                gSavemap.saveCrystalFlags |= 1;
                                Menu_SetPhase(5, 0);
                                Menu_SetScreen(0);
                                Menu_BeginClosingTopLevelMenu();
                                break;
                            default:
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                                g_MenuItemDelay = 0;
                                g_MenuItemState = MENU_ITEM_STATE_SELECT_TARGET;
                                break;
                            }
                        } else
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    } else
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                } else if (Input_GetPressed(INPUT_CROSS)) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                    g_MenuItemState = MENU_ITEM_STATE_COMMAND_BAR;
                }
            }
            break;
        case MENU_ITEM_STATE_SELECT_TARGET:
            if (g_MenuItemDelay == 0) {
                if (Input_GetPressed(INPUT_CIRCLE)) {
                    v.item = gSavemap.items[g_MenuItemCursors[1].row + g_MenuItemCursors[1].scrollTop] & 0x1ff;
                    v.actor = gSavemap.party[g_MenuItemCursors[2].row];
                    if (v.actor == 255 && v.item != ITEM_MEGALIXIR && v.item != ITEM_TENT) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    } else {
                        switch (v.item) {
                        case ITEM_HYPER:
                            if (gSavemap.chars[v.actor].status & BATTLE_STATUS_FURY) {
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                break;
                            }
                            if (gSavemap.chars[v.actor].status & BATTLE_STATUS_SADNESS)
                                gSavemap.chars[v.actor].status &= 0xef;
                            else
                                gSavemap.chars[v.actor].status |= BATTLE_STATUS_FURY;
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            Savemap_RemoveItem(v.item | MENU_ITEM_QUANTITY_ONE);
                            if (Savemap_FindItem((unsigned short)v.item) == MENU_ITEM_SLOT_EMPTY)
                                g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                            break;
                        case ITEM_TRANQUILIZER:
                            if (gSavemap.chars[v.actor].status & BATTLE_STATUS_FURY)
                                gSavemap.chars[v.actor].status &= 0xdf;
                            else {
                                if (gSavemap.chars[v.actor].status & BATTLE_STATUS_SADNESS) {
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                    break;
                                }
                                gSavemap.chars[v.actor].status |= BATTLE_STATUS_SADNESS;
                            }
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            Savemap_RemoveItem(v.item | MENU_ITEM_QUANTITY_ONE);
                            if (Savemap_FindItem((unsigned short)v.item) == MENU_ITEM_SLOT_EMPTY)
                                g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                            break;
                        case ITEM_OMNISLASH:
                        case ITEM_CATASTROPHE:
                        case ITEM_FINAL_HEAVEN:
                        case ITEM_GREAT_GOSPEL:
                        case ITEM_COSMO_MEMORY:
                        case ITEM_ALL_CREATION:
                        case ITEM_CHAOS:
                        case ITEM_HIGHWIND:
                            if (v.actor == ITEM_LIMIT_MANUAL_OWNER(v.item)) {
                                if (Menu_CharacterLimitMaskMatchesAttackSlots(ITEM_LIMIT_MANUAL_OWNER(v.item))) {
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_HEAL);
                                    gSavemap.chars[ITEM_LIMIT_MANUAL_OWNER(v.item)].limits |= 0x200;
                                    Savemap_RemoveItem(v.item | MENU_ITEM_QUANTITY_ONE);
                                    if (Savemap_FindItem((unsigned short)v.item) == MENU_ITEM_SLOT_EMPTY)
                                        g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                                    Menu_CopyPopupTextBuffer(g_MenuItemManualText + (v.item - ITEM_OMNISLASH) * 3 * 34);
                                    Menu_OpenPopupMessage(g_MenuItemPopupText, 7);
                                } else {
                                    Menu_CopyPopupTextBuffer(g_MenuItemManualText + ((v.item - ITEM_OMNISLASH) * 3 + 1) * 34);
                                    Menu_OpenPopupMessage(g_MenuItemPopupText, 7);
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                }
                            } else if (v.actor == 6) {
                                Menu_CopyPopupTextBuffer(g_MenuItemManualNoUseText);
                                Menu_OpenPopupMessage(g_MenuItemPopupText, 7);
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                            } else {
                                Menu_CopyPopupTextBuffer(g_MenuItemManualText + ((v.actor < 6 ? v.actor : v.actor - 1) * 3 + 2) * 34);
                                Menu_OpenPopupMessage(g_MenuItemPopupText, 7);
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                            }
                            break;
                        case ITEM_POWER_SOURCE:
                        case ITEM_GUARD_SOURCE:
                        case ITEM_MAGIC_SOURCE:
                        case ITEM_MIND_SOURCE:
                        case ITEM_SPEED_SOURCE:
                        case ITEM_LUCK_SOURCE:
                            switch (v.item - ITEM_POWER_SOURCE) {
                            case MENU_SOURCE_STRENGTH:
                                if (gSavemap.chars[v.actor].strengthBonus < 255)
                                    gSavemap.chars[v.actor].strengthBonus++;
                                else {
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                    goto item_seed_done;
                                }
                                break;
                            case MENU_SOURCE_VITALITY:
                                if (gSavemap.chars[v.actor].vitalityBonus < 255)
                                    gSavemap.chars[v.actor].vitalityBonus++;
                                else {
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                    goto item_seed_done;
                                }
                                break;
                            case MENU_SOURCE_MAGIC:
                                if (gSavemap.chars[v.actor].magicBonus < 255)
                                    gSavemap.chars[v.actor].magicBonus++;
                                else {
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                    goto item_seed_done;
                                }
                                break;
                            case MENU_SOURCE_SPIRIT:
                                if (gSavemap.chars[v.actor].spiritBonus < 255)
                                    gSavemap.chars[v.actor].spiritBonus++;
                                else {
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                    goto item_seed_done;
                                }
                                break;
                            case MENU_SOURCE_DEXTERITY:
                                if (gSavemap.chars[v.actor].dexterityBonus < 255)
                                    gSavemap.chars[v.actor].dexterityBonus++;
                                else {
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                    goto item_seed_done;
                                }
                                break;
                            case MENU_SOURCE_LUCK:
                                if (gSavemap.chars[v.actor].luckBonus < 255)
                                    gSavemap.chars[v.actor].luckBonus++;
                                else {
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                    goto item_seed_done;
                                }
                                break;
                            }
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            Savemap_BuildPartyStatsFromRecord(g_MenuItemCursors[2].row);
                            Battle_LoadCombatantFromSavemap(g_MenuItemCursors[2].row);
                            Savemap_RemoveItem(v.item | MENU_ITEM_QUANTITY_ONE);
                            if (Savemap_FindItem((unsigned short)v.item) == MENU_ITEM_SLOT_EMPTY)
                                g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                        item_seed_done:
                            break;
                        case ITEM_POTION:
                            if (Menu_ItemScreenIsPartyMemberHpFull(g_MenuItemCursors[2].row) || g_MenuPartyStats[g_MenuItemCursors[2].row].hp == 0) {
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                break;
                            }
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            Savemap_HealHp(g_MenuItemCursors[2].row, 100);
                            Savemap_RemoveItem(v.item | MENU_ITEM_QUANTITY_ONE);
                            if (Savemap_FindItem((unsigned short)v.item) == MENU_ITEM_SLOT_EMPTY)
                                g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                            break;
                        case ITEM_HI_POTION:
                            if (Menu_ItemScreenIsPartyMemberHpFull(g_MenuItemCursors[2].row) || g_MenuPartyStats[g_MenuItemCursors[2].row].hp == 0) {
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                break;
                            }
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            Savemap_HealHp(g_MenuItemCursors[2].row, 500);
                            Savemap_RemoveItem(v.item | MENU_ITEM_QUANTITY_ONE);
                            if (Savemap_FindItem((unsigned short)v.item) == MENU_ITEM_SLOT_EMPTY)
                                g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                            break;
                        case ITEM_ETHER:
                            if (Menu_ItemScreenIsPartyMemberMpFull(g_MenuItemCursors[2].row) || g_MenuPartyStats[g_MenuItemCursors[2].row].hp == 0) {
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                break;
                            }
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            Savemap_HealMp(g_MenuItemCursors[2].row, 100);
                            Savemap_RemoveItem(v.item | MENU_ITEM_QUANTITY_ONE);
                            if (Savemap_FindItem((unsigned short)v.item) == MENU_ITEM_SLOT_EMPTY)
                                g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                            break;
                        case ITEM_TURBO_ETHER:
                            if (Menu_ItemScreenIsPartyMemberMpFull(g_MenuItemCursors[2].row) || g_MenuPartyStats[g_MenuItemCursors[2].row].hp == 0) {
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                break;
                            }
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            Savemap_HealMp(g_MenuItemCursors[2].row, 10000);
                            Savemap_RemoveItem(v.item | MENU_ITEM_QUANTITY_ONE);
                            if (Savemap_FindItem((unsigned short)v.item) == MENU_ITEM_SLOT_EMPTY)
                                g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                            break;
                        case ITEM_PHOENIX_DOWN:
                            if (g_MenuPartyStats[g_MenuItemCursors[2].row].hp != 0) {
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                break;
                            }
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            Savemap_HealHp(g_MenuItemCursors[2].row, g_MenuPartyStats[g_MenuItemCursors[2].row].maxHp / 4);
                            Savemap_RemoveItem(v.item | MENU_ITEM_QUANTITY_ONE);
                            if (Savemap_FindItem((unsigned short)v.item) == MENU_ITEM_SLOT_EMPTY)
                                g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                            break;
                        case ITEM_TENT:
                            v.i = 0;
                            v.canHeal = v.i;
                            for (; v.i < 3; v.i++) {
                                if (gSavemap.party[v.i] != PARTY_CHAR_NONE && (!Menu_ItemScreenIsPartyMemberHpFull(v.i) || !Menu_ItemScreenIsPartyMemberMpFull(v.i)))
                                    v.canHeal = 1;
                            }
                            if (v.canHeal != 0) {
                                for (v.i = 0; v.i < 3; v.i++) {
                                    if (g_MenuPartyStats[v.i].hp == 0 || gSavemap.party[v.i] == PARTY_CHAR_NONE)
                                        continue;
                                    Savemap_HealHp(v.i, 10000);
                                    Savemap_HealMp(v.i, 10000);
                                }
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                                Savemap_RemoveItem(v.item | MENU_ITEM_QUANTITY_ONE);
                                if (Savemap_FindItem((unsigned short)v.item) == MENU_ITEM_SLOT_EMPTY)
                                    g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                            } else
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                            break;
                        case ITEM_X_POTION:
                            if (Menu_ItemScreenIsPartyMemberHpFull(g_MenuItemCursors[2].row) || g_MenuPartyStats[g_MenuItemCursors[2].row].hp == 0) {
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                break;
                            }
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            Savemap_HealHp(g_MenuItemCursors[2].row, 10000);
                            Savemap_RemoveItem(v.item | MENU_ITEM_QUANTITY_ONE);
                            if (Savemap_FindItem((unsigned short)v.item) == MENU_ITEM_SLOT_EMPTY)
                                g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                            break;
                        case ITEM_ELIXIR:
                            if ((Menu_ItemScreenIsPartyMemberHpFull(g_MenuItemCursors[2].row) && Menu_ItemScreenIsPartyMemberMpFull(g_MenuItemCursors[2].row)) || g_MenuPartyStats[g_MenuItemCursors[2].row].hp == 0) {
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                break;
                            }
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            Savemap_HealHp(g_MenuItemCursors[2].row, 10000);
                            Savemap_HealMp(g_MenuItemCursors[2].row, 10000);
                            Savemap_RemoveItem(v.item | MENU_ITEM_QUANTITY_ONE);
                            if (Savemap_FindItem((unsigned short)v.item) == MENU_ITEM_SLOT_EMPTY)
                                g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                            break;
                        case ITEM_MEGALIXIR:
                            v.i = 0;
                            v.canHeal = v.i;
                            for (; v.i < 3; v.i++) {
                                if (gSavemap.party[v.i] != PARTY_CHAR_NONE && (!Menu_ItemScreenIsPartyMemberHpFull(v.i) || !Menu_ItemScreenIsPartyMemberMpFull(v.i)))
                                    v.canHeal = 1;
                            }
                            if (v.canHeal != 0) {
                                for (v.i = 0; v.i < 3; v.i++) {
                                    if (g_MenuPartyStats[v.i].hp == 0 || gSavemap.party[v.i] == PARTY_CHAR_NONE)
                                        continue;
                                    Savemap_HealHp(v.i, 10000);
                                    Savemap_HealMp(v.i, 10000);
                                }
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                                Savemap_RemoveItem(v.item | MENU_ITEM_QUANTITY_ONE);
                                if (Savemap_FindItem((unsigned short)v.item) == MENU_ITEM_SLOT_EMPTY)
                                    g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                            } else
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                            break;
                        }
                    }
                } else if (BtlMenu_GetInputRepeat(INPUT_CROSS)) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                    g_MenuItemState = MENU_ITEM_STATE_ITEM_LIST;
                }
            }
            break;
        case MENU_ITEM_STATE_KEY_ITEMS:
            if (Input_GetPressed(INPUT_CROSS)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_MenuItemState = MENU_ITEM_STATE_COMMAND_BAR;
            }
            break;
        case MENU_ITEM_STATE_ARRANGE_CHOICE:
            if (Input_GetPressed(INPUT_CIRCLE)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                if (g_MenuItemCursors[4].row == 0) {
                    MenuCursor_Init(&g_MenuItemCursors[5], 0, 0, 1, 10, 0, 0, 1, 320, 0, 0, 0, 0, 0, 1);
                    g_MenuItemSwapState = MENU_ITEM_SWAP_PICK_FIRST;
                    g_MenuItemSavedColRow[0] = 0;
                    g_MenuItemSavedColRow[1] = 0;
                    g_MenuItemState = MENU_ITEM_STATE_SWAP_LIST;
                } else {
                    Menu_ItemArrangeBySortMode(g_MenuItemCursors[4].row);
                    g_MenuItemState = MENU_ITEM_STATE_COMMAND_BAR;
                }
            } else if (Input_GetPressed(INPUT_CROSS)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_MenuItemState = MENU_ITEM_STATE_COMMAND_BAR;
            }
            break;
        case MENU_ITEM_STATE_SWAP_LIST:
            if (Input_GetPressed(INPUT_CIRCLE)) {
                switch (g_MenuItemSwapState) {
                case MENU_ITEM_SWAP_PICK_FIRST:
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuItemSavedColRow[0] = g_MenuItemCursors[5].col;
                    g_MenuItemSavedColRow[1] = g_MenuItemCursors[5].row + g_MenuItemCursors[5].scrollTop;
                    g_MenuItemSwapState++;
                    break;
                case MENU_ITEM_SWAP_PICK_SECOND:
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    v.swap = gSavemap.items[g_MenuItemSavedColRow[1]];
                    gSavemap.items[g_MenuItemSavedColRow[1]] = gSavemap.items[g_MenuItemCursors[5].row + g_MenuItemCursors[5].scrollTop];
                    gSavemap.items[g_MenuItemCursors[5].row + g_MenuItemCursors[5].scrollTop] = v.swap;
                    g_MenuItemSwapState = MENU_ITEM_SWAP_PICK_FIRST;
                    break;
                }
            } else if (Input_GetPressed(INPUT_CROSS)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_MenuItemState = MENU_ITEM_STATE_COMMAND_BAR;
            }
            break;
        }
    }
    return;
}

/*
 * Draws item type icon glyph on item screen in 2x mode.
 */
// FUNCTION: FF7 0x00717691
void __cdecl Menu_DrawItemScreenTypeIcon2x(int x, int y, int itemId, int unused, float z)
{
    int item;
    int su;
    int sv;
    int kind;

    if (itemId < 0x80) {
        kind = 0;
    } else if (itemId < 0x100) {
        item = itemId - 0x80;
        if (item < 16) {
            kind = 1;
        } else if (item < 32) {
            kind = 3;
        } else if (item < 48) {
            kind = 2;
        } else if (item < 62) {
            kind = 5;
        } else if (item < 73) {
            kind = 4;
        } else if (item < 87) {
            kind = 9;
        } else if (item < 101) {
            kind = 6;
        } else if (item < 114) {
            kind = 7;
        } else {
            kind = 8;
        }
    } else if (itemId < 0x120) {
        kind = 0xa;
    } else {
        kind = 0xb;
    }
    su = (kind & 1) * 16 + 0x60;
    sv = kind / 2 * 16 + 0x70;
    Menu_DrawAtlasQuad2x(x, y, su, sv, 16, 16, 1, 0, 0, z);
}

/*
 * Draws item type icon glyph on item screen in 1x mode.
 */
// FUNCTION: FF7 0x007177a8
void __cdecl Menu_DrawItemScreenTypeIcon1x(int x, int y, int itemId, int unused, float z)
{
    int item;
    int su;
    int sv;
    int kind;

    if (itemId < 0x80) {
        kind = 0;
    } else if (itemId < 0x100) {
        item = itemId - 0x80;
        if (item < 16) {
            kind = 1;
        } else if (item < 32) {
            kind = 3;
        } else if (item < 48) {
            kind = 2;
        } else if (item < 62) {
            kind = 5;
        } else if (item < 73) {
            kind = 4;
        } else if (item < 87) {
            kind = 9;
        } else if (item < 101) {
            kind = 6;
        } else if (item < 114) {
            kind = 7;
        } else {
            kind = 8;
        }
    } else if (itemId < 0x120) {
        kind = 0xa;
    } else {
        kind = 0xb;
    }
    su = (kind & 1) * 16 + 0x60;
    sv = kind / 2 * 16 + 0x70;
    Menu_DrawAtlasQuad1x(x, y, su, sv, 16, 16, 1, 0, 0, z);
}

/*
 * Returns non-zero if party member HP is currently at maximum.
 */
// FUNCTION: FF7 0x007178bf
int __cdecl Menu_ItemScreenIsPartyMemberHpFull(int slot)
{
    if (g_MenuPartyStats[slot].maxHp == g_MenuPartyStats[slot].hp) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Returns non-zero if party member MP is currently at maximum.
 */
// FUNCTION: FF7 0x007178f1
int __cdecl Menu_ItemScreenIsPartyMemberMpFull(int slot)
{
    if (g_MenuPartyStats[slot].maxMp == g_MenuPartyStats[slot].mp) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Copies encoded message text into active popup buffer.
 */
// FUNCTION: FF7 0x00717923
void __cdecl Menu_CopyPopupTextBuffer(unsigned char *src)
{
    int i;

    for (i = 0; i < 80; i++) {
        g_MenuItemPopupText[i] = *src;
        src++;
    }
}

/*
 * Empty no-op function in item menu screen.
 */
// FUNCTION: FF7 0x0071795c
void __cdecl Menu_UnusedItemSortNoOp(void)
{
    /* The original is empty. */
}

/*
 * Removes all equipped materia from party members into stash.
 */
// FUNCTION: FF7 0x00717961
void __cdecl Savemap_HarvestAllMateriaToStash(void)
{
    int stockIndex;
    int workIndex;
    int charIdx;
    int weaponIdx;
    int armorIndex;

    for (workIndex = 0; workIndex < 48; workIndex++) {
        g_MateriaWork[workIndex] = -1;
    }
    for (charIdx = 0; charIdx < 9; charIdx++) {
        if ((gSavemap.phsVisibilityMask & (1 << charIdx)) == 0) {
            continue;
        }
        for (weaponIdx = 0; weaponIdx < 8; weaponIdx++) {
            Savemap_StashOneMateria(
                (unsigned int *)&gSavemap.chars[charIdx].weaponMateria[weaponIdx]);
        }
        for (armorIndex = 0; armorIndex < 8; armorIndex++) {
            Savemap_StashOneMateria(
                (unsigned int *)&gSavemap.chars[charIdx].armorMateria[armorIndex]);
        }
    }
    for (stockIndex = 0; stockIndex < 200; stockIndex++) {
        Savemap_StashOneMateria((unsigned int *)&gSavemap.materia[stockIndex]);
    }
    Savemap_CommitMateriaStash();
}

/*
 * Moves a single materia item into the stash buffer.
 */
// FUNCTION: FF7 0x00717a6f
void __cdecl Savemap_StashOneMateria(unsigned int *pMateriaEntry)
{
    int lowest;
    int n;

    if (*pMateriaEntry == -1) {
        return;
    }
    for (n = 0; n < 48; n++) {
        if (g_MateriaWork[n] == -1) {
            g_MateriaWork[n] = *pMateriaEntry;
            return;
        }
    }
    lowest = Savemap_GetLowestMateriaKind();
    if (g_MateriaKindById[*pMateriaEntry & 0xff] < lowest) {
        return;
    }
    Savemap_ReplaceMateriaOfKind(*pMateriaEntry, lowest);
}

/*
 * Replaces materia of specified kind in party equipment.
 */
// FUNCTION: FF7 0x00717aef
void __cdecl Savemap_ReplaceMateriaOfKind(unsigned int materia,
                                          int kind)
{
    int i;

    for (i = 0; i < 48; i++) {
        if (g_MateriaKindById[g_MateriaWork[i] & 0xff] == kind) {
            g_MateriaWork[i] = materia;
            break;
        }
    }
}

/*
 * Finds lowest materia kind present in stashed inventory.
 */
// FUNCTION: FF7 0x00717b3d
int __cdecl Savemap_GetLowestMateriaKind(void)
{
    int i;
    int lowest;

    lowest = 0xff;
    for (i = 0; i < 48; i++) {
        if (g_MateriaKindById[g_MateriaWork[i] & 0xff] < lowest) {
            lowest = g_MateriaKindById[g_MateriaWork[i] & 0xff];
        }
    }
    return lowest;
}

/*
 * Finalizes stashed materia placements to savemap stock.
 */
// FUNCTION: FF7 0x00717ba3
void __cdecl Savemap_CommitMateriaStash(void)
{
    int i;

    for (i = 0; i < 48; i++) {
        if (g_MateriaWork[i] == -1) {
            continue;
        }
        Savemap_EraseMateriaValueAnywhere(g_MateriaWork[i]);
    }
    for (i = 0; i < 48; i++) {
        gSavemap.stolenMateria[i] = g_MateriaWork[i];
    }
}

/*
 * Removes matching materia value from stock and equipment.
 */
// FUNCTION: FF7 0x00717c15
void __cdecl Savemap_EraseMateriaValueAnywhere(unsigned int materiaEntry)
{
    int stockIndex;
    int charIdx;
    int weaponIdx;
    int armorIndex;

    for (charIdx = 0; charIdx < 9; charIdx++) {
        if ((gSavemap.phsVisibilityMask & (1 << charIdx)) == 0) {
            continue;
        }
        for (weaponIdx = 0; weaponIdx < 8; weaponIdx++) {
            if (gSavemap.chars[charIdx].weaponMateria[weaponIdx] ==
                materiaEntry) {
                gSavemap.chars[charIdx].weaponMateria[weaponIdx] = -1;
                return;
            }
        }
        for (armorIndex = 0; armorIndex < 8; armorIndex++) {
            if (gSavemap.chars[charIdx].armorMateria[armorIndex] ==
                materiaEntry) {
                gSavemap.chars[charIdx].armorMateria[armorIndex] = -1;
                return;
            }
        }
    }
    for (stockIndex = 0; stockIndex < 200; stockIndex++) {
        if (gSavemap.materia[stockIndex] == materiaEntry) {
            gSavemap.materia[stockIndex] = -1;
            return;
        }
    }
}

/*
 * Restores stashed materia back to original party equipment.
 */
// FUNCTION: FF7 0x00717d32
void __cdecl Savemap_ReturnMateriaStash(void)
{
    int i;

    for (i = 0; i < 48; i++) {
        if ((unsigned int)gSavemap.stolenMateria[i] == 0xFFFFFFFF) {
            continue;
        }
        if (Savemap_EquipMateriaInAnyFreeSlot((unsigned int)gSavemap.stolenMateria[i]) != 0) {
            Savemap_AddMateria((unsigned int)gSavemap.stolenMateria[i]);
        }
    }
}

/*
 * Equips materia in first available free weapon/armour slot.
 */
// FUNCTION: FF7 0x00717d8d
int __cdecl Savemap_EquipMateriaInAnyFreeSlot(unsigned int materiaEntry)
{
    int slot;
    int charIndex;
    int armorIndex;

    for (charIndex = 8; charIndex != -1; charIndex--) {
        if ((gSavemap.phsVisibilityMask & (1 << charIndex)) == 0) {
            continue;
        }
        for (slot = 0; slot < 8; slot++) {
            if (gSavemap.chars[charIndex].weaponMateria[slot] ==
                    -1 &&
                g_KernelWeaponDataArray[gSavemap.chars[charIndex].weapon].materiaSlotTable[slot] != 0) {
                gSavemap.chars[charIndex].weaponMateria[slot] =
                    materiaEntry;
                return 0;
            }
        }
        for (armorIndex = 0; armorIndex < 8; armorIndex++) {
            if (gSavemap.chars[charIndex].armorMateria[armorIndex] == -1 &&
                g_KernelArmorDataArray[gSavemap.chars[charIndex].armor].materiaSlotBytes[armorIndex] != 0) {
                gSavemap.chars[charIndex].armorMateria[armorIndex] =
                    materiaEntry;
                return 0;
            }
        }
    }
    return 1;
}

/*
 * Strips all equipment and materia from designated character record.
 */
// FUNCTION: FF7 0x00717eb6
void __cdecl Savemap_StripCharacterEquipment(int recordIndex)
{
    int weaponIdx;
    int armorIndex;

    for (weaponIdx = 0; weaponIdx < 8; weaponIdx++) {
        if (gSavemap.chars[recordIndex].weaponMateria[weaponIdx] ==
            -1) {
            continue;
        }
        Savemap_AddMateria(
            gSavemap.chars[recordIndex].weaponMateria[weaponIdx]);
        gSavemap.chars[recordIndex].weaponMateria[weaponIdx] =
            -1;
    }
    for (armorIndex = 0; armorIndex < 8; armorIndex++) {
        if (gSavemap.chars[recordIndex].armorMateria[armorIndex] ==
            -1) {
            continue;
        }
        Savemap_AddMateria(
            gSavemap.chars[recordIndex].armorMateria[armorIndex]);
        gSavemap.chars[recordIndex].armorMateria[armorIndex] =
            -1;
    }
    if (gSavemap.chars[recordIndex].accessory != 0xff) {
        Savemap_AddItem((gSavemap.chars[recordIndex].accessory + 0x120) |
                        MENU_ITEM_QUANTITY_ONE);
        gSavemap.chars[recordIndex].accessory = 0xff;
    }
}

/*
 * Backs up party composition and equipment to temporary stash.
 */
// FUNCTION: FF7 0x00717fd6
void __cdecl Savemap_BackupPartyAndEquipToStash(int recordIndex)
{
    MateriaStash *src;
    int i;

    src = (MateriaStash *)gSavemap.stolenMateria;
    for (i = 0; i < 3; i++) {
        src->party[i] = gSavemap.party[i];
    }
    src->weapon = gSavemap.chars[recordIndex].weapon;
    src->armor = gSavemap.chars[recordIndex].armor;
    for (i = 0; i < 3; i++) {
        src->materia[i] = gSavemap.materia[i];
        gSavemap.materia[i] = -1;
    }
    for (i = 0; i < 8; i++) {
        src->weaponMateria[i] =
            gSavemap.chars[recordIndex].weaponMateria[i];
        gSavemap.chars[recordIndex].weaponMateria[i] = -1;
        src->armorMateria[i] =
            gSavemap.chars[recordIndex].armorMateria[i];
        gSavemap.chars[recordIndex].armorMateria[i] = -1;
    }
    gSavemap.chars[recordIndex].weapon = 0;
}

/*
 * Restores party composition and equipment from temporary stash.
 */
// FUNCTION: FF7 0x0071810c
void __cdecl Savemap_RestorePartyAndEquipFromStash(int recordIndex)
{
    MateriaStash *src;
    int i;

    src = (MateriaStash *)gSavemap.stolenMateria;
    for (i = 0; i < 3; i++) {
        gSavemap.party[i] = src->party[i];
    }
    gSavemap.chars[recordIndex].weapon = src->weapon;
    gSavemap.chars[recordIndex].armor = src->armor;
    for (i = 0; i < 3; i++) {
        gSavemap.materia[i] = src->materia[i];
    }
    for (i = 0; i < 8; i++) {
        gSavemap.chars[recordIndex].weaponMateria[i] = src->weaponMateria[i];
        gSavemap.chars[recordIndex].armorMateria[i] = src->armorMateria[i];
    }
}
} /* extern "C" */
