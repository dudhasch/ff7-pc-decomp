#ifndef FF7_HIGHWAY_PALETTE_H
#define FF7_HIGHWAY_PALETTE_H

// The highway minigame's palette sampling and index lookup members: one original code group,
// 0x0065DC20..0x0065E512.
// One 12-byte step of a channel's record stream.
struct HighwayPaletteRecord {
    short group;
    short repeatCount;
    short entry;
    short offset;
    short flags;
    short angle;
};

// The 0x4E0-byte palette block shared by the field block's reset() and the palette's samplers:
// ten channels, each walking its record stream and sampling colour triplets from a group.
struct HighwayPaletteBlock {
    int channelRecords[10];
    HighwayPaletteRecord *record;
    unsigned char *counts;
    unsigned char pad_30[4];
    int groupBase[256];
    unsigned char groupCount;
    unsigned char needRecord[10];
    unsigned char pad_43F;
    short recordIndex[10];
    short group[10];
    short repeatCount[10];
    short entry[10];
    short offset[10];
    short flags[10];
    short angle[10];
    unsigned char sampleCursor[10];
    unsigned char groupLength[10];
};

struct HighwayPaletteFieldBlock : HighwayPaletteBlock {
    void reset();
};

// The PSX data set's header: the archive base, the end of its offsets and the offset table.
struct HighwayOffsetHeader {
    char *base;
    int limit;
    int reserved;
    int *offsets;
};

// The data set as reset() reads it. lookup() is an inline member: under /Ob1 VC5 expands the
// calls in source order until the expanded callees would exceed its per-caller budget, so reset()
// has 16 expansions and 7 calls, and the out-of-line body is a COMDAT in
// highway_palette_sampling.obj. The body's form (the header struct, the else, the char * base)
// sets its size and so the count; any change to it or to reset() changes the count.
struct HighwayOffsetTable {
    HighwayOffsetHeader hdr;
    /*
     * Resolves a bounded offset from the index table: returns zero when offsets[index] is at least
     * limit, otherwise base + offset. No index or negative-offset check is present.
     */
    // FUNCTION: FF7 0x0065e520
    int lookup(int index)
    {
        int offset = hdr.offsets[index];
        if (offset >= hdr.limit) {
            return 0;
        } else {
            return (int)(hdr.base + offset);
        }
    }
};

struct HighwayPalette : HighwayPaletteBlock {
    char sample(unsigned char group, unsigned char entry, unsigned char *first, unsigned char *second, int unused);
    void sampleFrame(unsigned char index, int *kind, short *entry, short *offset,
                     short *flags, short *angle);
};

#endif
