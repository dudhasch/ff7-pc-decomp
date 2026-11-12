// Contents: Field block and palette code for the highway minigame: the field block resolves its
//   resource pointers, and the palette samples colour triplets and advances animated palette
//   records.
#include "ff7/highway_palette.h"
#include "ff7/api/highway.h"

extern "C" {
/* Typed as HighwayOffsetTable here so calls below resolve to mangled HighwayOffsetTable::lookup. */
extern HighwayOffsetTable g_HighwayPsxDataSet;
}

#pragma code_seg(".text")
/*
 * Resolves and stores eleven field-resource pointers from either indexes 4 through 14 or 18
 * through 28 according to the global mode; VC5 expands the first 16 lookups inline and calls the
 * other 7 (the inline budget, highway_palette.h). Reads a byte count from the first resource and constructs that many internal
 * pointers from little-endian offsets beginning after count plus one, resolving the base again for
 * each entry. Finally clears recordIndex and sets needRecord for the ten channels.
 */
// FUNCTION: FF7 0x0065dc20
void HighwayPaletteFieldBlock::reset()
{
    int stateIndex;
    int offsetCursor;
    int entryIdx;
    int baseAddr;

    if (g_HighwayGameMode == 0) {
        this->counts = (unsigned char *)g_HighwayPsxDataSet.lookup(4);
        this->channelRecords[0] = g_HighwayPsxDataSet.lookup(5);
        this->channelRecords[1] = g_HighwayPsxDataSet.lookup(6);
        this->channelRecords[2] = g_HighwayPsxDataSet.lookup(7);
        this->channelRecords[3] = g_HighwayPsxDataSet.lookup(8);
        this->channelRecords[4] = g_HighwayPsxDataSet.lookup(9);
        this->channelRecords[5] = g_HighwayPsxDataSet.lookup(0xA);
        this->channelRecords[6] = g_HighwayPsxDataSet.lookup(0xB);
        this->channelRecords[7] = g_HighwayPsxDataSet.lookup(0xC);
        this->channelRecords[8] = g_HighwayPsxDataSet.lookup(0xD);
        this->channelRecords[9] = g_HighwayPsxDataSet.lookup(0xE);
    } else {
        this->counts = (unsigned char *)g_HighwayPsxDataSet.lookup(0x12);
        this->channelRecords[0] = g_HighwayPsxDataSet.lookup(0x13);
        this->channelRecords[1] = g_HighwayPsxDataSet.lookup(0x14);
        this->channelRecords[2] = g_HighwayPsxDataSet.lookup(0x15);
        this->channelRecords[3] = g_HighwayPsxDataSet.lookup(0x16);
        this->channelRecords[4] = g_HighwayPsxDataSet.lookup(0x17);
        this->channelRecords[5] = g_HighwayPsxDataSet.lookup(0x18);
        this->channelRecords[6] = g_HighwayPsxDataSet.lookup(0x19);
        this->channelRecords[7] = g_HighwayPsxDataSet.lookup(0x1A);
        this->channelRecords[8] = g_HighwayPsxDataSet.lookup(0x1B);
        this->channelRecords[9] = g_HighwayPsxDataSet.lookup(0x1C);
    }
    this->groupCount = *this->counts;
    offsetCursor = this->groupCount + 1;
    for (entryIdx = 0; entryIdx < this->groupCount; entryIdx++) {
        baseAddr = g_HighwayPsxDataSet.lookup(g_HighwayGameMode ? 0x12 : 4);
        this->groupBase[entryIdx] =
            baseAddr + this->counts[offsetCursor] +
            (this->counts[offsetCursor + 1] << 8);
        offsetCursor += 2;
    }
    for (stateIndex = 0; stateIndex < 0xA; stateIndex++) {
        this->recordIndex[stateIndex] = 0;
        this->needRecord[stateIndex] = 1;
    }
}

/*
 * Reads the first two bytes of a palette triplet from the selected group.
 * Selects groupBase[group] and a stride-three entry using entry&0xFF; copies
 * its first two bytes through first and second and returns zero. unused is not read.
 */
// FUNCTION: FF7 0x0065e1cc
char HighwayPalette::sample(unsigned char group, unsigned char entry, unsigned char *first, unsigned char *second, int unused)
{
    unsigned char *rec = (unsigned char *)(this->groupBase[group] + entry * 3);
    *first = *rec;
    rec++;
    *second = *rec;
    rec++;
    return 0;
}

/*
 * Copies six SHORT fields from the current twelve-byte record when advancing a slot, then
 * samples two palette bytes, advances its byte cursor and writes the selected outputs.
 * The count resource begins with its group count; its per-group byte counts start at +1.
 */
// FUNCTION: FF7 0x0065e223
void HighwayPalette::sampleFrame(unsigned char index, int *kind, short *entry, short *offset,
                             short *flags, short *angle)
{
    unsigned char p;
    unsigned char a;
    unsigned char b;

    if (this->needRecord[index] != 0) {
        this->record = (HighwayPaletteRecord *)(this->channelRecords[index] +
                                                this->recordIndex[index] * 0xC);
        ++this->recordIndex[index];
        this->group[index] = this->record->group;
        this->repeatCount[index] = this->record->repeatCount;
        this->entry[index] = this->record->entry;
        this->offset[index] = this->record->offset;
        this->flags[index] = this->record->flags;
        this->angle[index] = this->record->angle;
        this->groupLength[index] = (this->counts + 1)[this->record->group];
        this->sampleCursor[index] = 0;
        this->needRecord[index] = 0;
    }

    sample(this->group[index], this->sampleCursor[index], &p, &a, (int)&b);
    ++this->sampleCursor[index];
    if (this->sampleCursor[index] == this->groupLength[index]) {
        this->sampleCursor[index] = 0;
        --this->repeatCount[index];
        if (this->repeatCount[index] == 0) {
            this->needRecord[index] = 1;
        }
    }
    *(unsigned char *)kind = p;
    *entry = this->entry[index];
    *offset = this->offset[index];
    *flags = this->flags[index];
    *angle = this->angle[index];
}

#pragma code_seg()
