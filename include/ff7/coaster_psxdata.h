#pragma once


// Five dword slots observed at offsets 0..0x10 in the original loader. The coaster TU's global
// instance (0x00c3f738) proves the inline constructor: its dynamic initializer $E1 (0x005e915f)
// clears words[0] and words[4] in that order, and its at-exit thunk $E2 (0x005e918a) calls the
// out-of-line destructor CoasterPsxDataSet::~CoasterPsxDataSet (0x005ee730).
typedef struct CoasterPsxDataSet CoasterPsxDataSet;
struct CoasterPsxDataSet {
    int binData;
    int binSize;
    int adrBaseOffset;
    int *adrOffsets;
    int isLoaded;

    CoasterPsxDataSet()
    {
        binData = 0;
        isLoaded = 0;
    }
    ~CoasterPsxDataSet();
    int *Mini_CoasterLoadFileBlock(char *name, int *byteCount);
    int Mini_CoasterLoadPsxDataSet(char *adrName, char *binName);
};
