// src/ff7/coaster/psxdata_c.cpp
// Original: C:\FF7\coaster\psxdata_c.cpp
// Contents: Loads and frees the PSX binary data set for the coaster minigame, locating its entries
//   by name in the LGP archive and releasing the heap buffers when the set is destroyed.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "ff7/types/coaster.h"
#include "ff7/coaster_psxdata.h"
#include "ff7/coaster_renderer.h"
#include <windows.h>
#include <ddraw.h>
#include "lib/crt.h"
#include <time.h>
#include "ff7/api/coaster.h"
#include <stdlib.h>
#include "ff7/enums/main.h"
#include "lib/api/file.h"
#include "lib/enums/file.h"

extern "C" {

// clang-format on
} // extern "C"

/*
 * Loads a coaster PSX dataset pair (binary model data and address table) from the minigame
 * archive, rebases the table entries relative to its first entry (words[2]), and marks the
 * dataset initialized (words[4]).
 */
// FUNCTION: FF7 0x005ee620
int CoasterPsxDataSet::Mini_CoasterLoadPsxDataSet(char *adrName, char *binName)
{
    int byteCount;

    if (isLoaded == 0) {
        byteCount = 0;
        binData = (int)Mini_CoasterLoadFileBlock(binName, &binSize);
        adrOffsets = Mini_CoasterLoadFileBlock(adrName, &byteCount);
        adrBaseOffset = *adrOffsets;
        for (unsigned int i = 0; i < ((unsigned int)byteCount >> 2); i++) {
            adrOffsets[i] -= adrBaseOffset;
        }
        isLoaded = 1;
    }
    return isLoaded;
}
extern "C" {

/*
 * Frees allocated heap buffers for coaster PSX dataset address table and binary data,
 * then zeroes out the state fields.
 */
// FUNCTION: FF7 0x005ee6cc
void __fastcall Mini_CoasterFreePsxDataSet(CoasterPsxDataSet *state)
{
    if (state->adrOffsets != 0) {
        // STRING: FF7 0x009014f0
        GameHeapFree(state->adrOffsets, "C:\\FF7\\coaster\\psxdata_c.cpp", 70);
    }
    if (state->binData != 0) {
        // STRING: FF7 0x00901510
        GameHeapFree((void *)state->binData, "C:\\FF7\\coaster\\psxdata_c.cpp", 72);
    }
    state->adrOffsets = 0;
    state->binData = 0;
    state->isLoaded = 0;
}
} // extern "C"

/*
 * Destructor: releases the loaded PSX dataset buffers.
 */
// FUNCTION: FF7 0x005ee730
CoasterPsxDataSet::~CoasterPsxDataSet()
{
    Mini_CoasterFreePsxDataSet(this);
}

/*
 * Locates an entry by name in LGP archive 9, allocates a heap buffer for it,
 * and reads its contents into memory.
 */
// FUNCTION: FF7 0x005ee743
int *CoasterPsxDataSet::Mini_CoasterLoadFileBlock(char *name, int *byteCount)
{
    int local[2];
    local[1] = 0;
    local[0] = 0;

    local[1] = (int)FindLgpFile(name, LGP_ARCHIVE_COASTER);
    if (local[1] != 0) {
        *byteCount = GetLgpEntrySize(local[1], LGP_ARCHIVE_COASTER);
        if (*byteCount != 0) {
            // STRING: FF7 0x00901530
            local[0] = (int)GameHeapAllocRaw(*byteCount, "C:\\FF7\\coaster\\psxdata_c.cpp", 100);
            if (local[0] != 0) {
                if (ReadLgpEntry((void *)local[1], LGP_ARCHIVE_COASTER, (void *)local[0], *byteCount) == 0) {
                    free((void *)local[0]);
                    local[0] = 0;
                }
            }
        }
    }
    return (int *)local[0];
}
