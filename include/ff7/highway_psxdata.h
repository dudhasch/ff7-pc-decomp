#pragma once

// Five dword slots observed at offsets 0..0x10 in the highway psxdata methods. The global instance
// (g_HighwayPsxDataSet, 0x00d858e8) proves the inline constructor and the destructor: its dynamic
// initializer $E1 (0x006504cf) clears words[0] and words[4] in that order, and its at-exit thunk
// $E2 (0x006504fa) calls the out-of-line destructor HighwayPsxDataSet20::~HighwayPsxDataSet20
// (0x00652605).
typedef struct HighwayPsxDataSet20 HighwayPsxDataSet20;
struct HighwayPsxDataSet20 {
    int binData;
    int binSize;
    int adrBaseOffset;
    int *adrOffsets;
    int isLoaded;

    HighwayPsxDataSet20()
    {
        binData = 0;
        isLoaded = 0;
    }
    ~HighwayPsxDataSet20();
    int Mini_HighwayLoadPsxDataSet(char *adrName, char *binName);
    void Mini_HighwayFreePsxDataSet();
};

extern "C" HighwayPsxDataSet20 g_HighwayPsxDataSet;
