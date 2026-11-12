// src/ff7/field/src/field_file_load_state.cpp
// Data-only object: .bss 0x00cff5b0..0x00cff5b8. No code.
// A data-only member of the field library, which its neighbour ad_cdr.cpp never references: the field file
// load request flag and parameter.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/crt.h"
#include "ff7/api/field.h"
#include "lib/api/mem.h"
#include <stdlib.h>
#include "lib/api/file.h"
#include "ff7/enums/main.h"
#include "lib/enums/file.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00cff5b0
int g_FieldFileLoadPending = 0;
// GLOBAL: FF7 0x00cff5b4
int g_FieldFileLoadParam = 0;
// clang-format on

} /* extern "C" */
