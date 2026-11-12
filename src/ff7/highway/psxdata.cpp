// src/ff7/highway/psxdata.cpp
// Original: C:\FF7\highway\psxdata.cpp
// Contents: Loads the highway minigame's asset files from the LGP archive or directly from disk
//   into allocated buffers, and loads and frees the address and binary PSX datasets the minigame
//   uses.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "ff7/api/snobo.h"
#include "ff7/api/highway.h"
#include "lib/api/file.h"
#include "ff7/highway_psxdata.h"
#include "ff7/enums/main.h"
#include "ff7/types/highway.h"
#include "lib/enums/file.h"

 /* extern "C" */
// Observed minimum receiver view: five DWORD fields.

/*
 * Loads an asset file from the LGP archive or directly from disk into an allocated buffer.
 */
// FUNCTION: FF7 0x006523b0
int HighwayFileLoader::load(int byteCount, char *name)
{
    int buf;
    int lgpFile;
    int entryBytes;
    char *pDiskPath;
    char *lgpNamePtr;
    int freeBuf;
    ChangeDirectorySaving(g_HighwayDataDirectory, 0);
    if (g_HighwayUseLgpArchive != 0) {
        lgpNamePtr = name;
        lgpFile = (int)FindLgpFile(lgpNamePtr, LGP_ARCHIVE_HIGHWAY);
        if (lgpFile == 0)
            return 0;
        entryBytes = GetLgpEntrySize(lgpFile, LGP_ARCHIVE_HIGHWAY);
        buf = (int)operator new(entryBytes);
        if (buf == 0)
            return 0;
        if (ReadLgpEntry((void *)lgpFile, LGP_ARCHIVE_HIGHWAY, (void *)buf, entryBytes) == 0) {
            freeBuf = buf;
            operator delete((void *)freeBuf);
            return 0;
        }
        *(int *)byteCount = entryBytes;
    } else {
        pDiskPath = name;
        buf = (int)LoadDiskFile((void *)byteCount, pDiskPath);
    }
    return buf;
}
/*
 * Loads address and binary dataset files for the highway minigame PSX data.
 */
// FUNCTION: FF7 0x00652483
int HighwayPsxDataSet20::Mini_HighwayLoadPsxDataSet(char *adrName, char *binName)
{
    unsigned int adrSize;
    unsigned int i;
    if (isLoaded == 0) {
        adrSize = 0;
        binData = ((HighwayFileLoader *)this)->load((int)&binSize, binName);
        if (binData == 0)
            return 0;
        adrOffsets = (int *)((HighwayFileLoader *)this)->load((int)&adrSize, adrName);
        if (adrOffsets == 0) {
            // STRING: FF7 0x0090930c
            GameHeapFree((void *)binData, "C:\\FF7\\highway\\psxdata.cpp", 83);
            binData = 0;
            return 0;
        }
        adrBaseOffset = *adrOffsets;
        for (i = 0; i < (adrSize >> 2); i++)
            adrOffsets[i] -= adrBaseOffset;
        isLoaded = 1;
    }
    return isLoaded;
}
/*
 * Releases allocated memory for loaded PSX dataset address and binary buffers.
 */
// FUNCTION: FF7 0x00652569
void HighwayPsxDataSet20::Mini_HighwayFreePsxDataSet()
{
    int adrBuf;
    int binBuffer;
    if (g_HighwayUseLgpArchive != 0) {
        adrBuf = (int)adrOffsets;
        operator delete((void *)adrBuf);
        binBuffer = binData;
        operator delete((void *)binBuffer);
    } else {
        if (adrOffsets != 0)
            // STRING: FF7 0x00909328
            GameHeapFree(adrOffsets, "C:\\FF7\\highway\\psxdata.cpp", 116);
        if (binData != 0)
            // STRING: FF7 0x00909344
            GameHeapFree((void *)binData, "C:\\FF7\\highway\\psxdata.cpp", 118);
    }
    adrOffsets = 0;
    binData = 0;
    isLoaded = 0;
}

/*
 * Destructor for HighwayPsxDataSet20, releasing any allocated dataset buffers.
 */
// FUNCTION: FF7 0x00652605
HighwayPsxDataSet20::~HighwayPsxDataSet20()
{
    if (adrOffsets != 0)
        // STRING: FF7 0x00909360
        GameHeapFree(adrOffsets, "C:\\FF7\\highway\\psxdata.cpp", 132);
    if (binData != 0)
        // STRING: FF7 0x0090937c
        GameHeapFree((void *)binData, "C:\\FF7\\highway\\psxdata.cpp", 134);
    adrOffsets = 0;
    binData = 0;
}
