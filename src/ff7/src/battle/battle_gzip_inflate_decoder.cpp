// src/ff7/src/battle/battle_gzip_inflate_decoder.cpp
// Contents: Decompresses gzip (deflate) data for battle binary sections, decoding stored,
//   fixed-Huffman and dynamic-Huffman blocks with a small scratch pool for the Huffman tables.

#include "ff7/api/battle.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "lib/codegen.h"
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
extern "C" {

// GLOBAL: FF7 0x007c0d08
int g_GzipBorderOrder[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

// GLOBAL: FF7 0x007c0d58
unsigned short g_GzipCopyLengths[31] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0};

// GLOBAL: FF7 0x007c0d98
unsigned short g_GzipCopyLengthExtraBits[31] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 99, 99};

// GLOBAL: FF7 0x007c0dd8
unsigned short g_GzipCopyDistances[30] = {1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577};

// GLOBAL: FF7 0x007c0e18
unsigned short g_GzipCopyDistanceExtraBits[30] = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};

// GLOBAL: FF7 0x007c0e58
unsigned short g_GzipMaskBits[17] = {0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767, 65535};

// GLOBAL: FF7 0x007c0e7c
int g_GzipLiteralTableBits = 9;

// GLOBAL: FF7 0x007c0e80
int g_GzipDistanceTableBits = 6;

// GLOBAL: FF7 0x009ada68
int g_GzipMemPool = 0;

// GLOBAL: FF7 0x009ada6c
unsigned char *g_GzipInputBuffer = 0;

// GLOBAL: FF7 0x009ada70
int g_GzipBitBuffer = 0;

// GLOBAL: FF7 0x009ada74
int g_GzipBitCount = 0;

// GLOBAL: FF7 0x009ada78
int g_GzipTotalHuftNodes = 0;

// GLOBAL: FF7 0x009ada7c
unsigned char *g_GzipOutputBuffer = 0;

// GLOBAL: FF7 0x009ada80
int g_GzipOutputOffset = 0;

// GLOBAL: FF7 0x009ada84
int g_GzipInputOffset = 0;

// GLOBAL: FF7 0x009ada88
int g_GzipMemPoolOffset = 0;

unsigned char g_bssPad_009ada8c[4] = {0};

/*
 * Inflates a gzip-compressed stream from input buffer to output buffer.
 * Verifies gzip magic bytes, deflate method, skips the 10-byte gzip header,
 * decodes compressed blocks, and checks uncompressed length against trailer.
 * Returns total uncompressed byte count on success, or -1 on error.
 */
// FUNCTION: FF7 0x0041d300
int __cdecl GzipInflate_BinSection(void *input, void *output)
{
    struct {
        int count;
        int scratch[4096];
        int trailerIndex;
        int status;
        unsigned char trailer[8];
        int crc;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.crc = 0;
    frame.count = 0;
    g_GzipInputBuffer = (unsigned char *)input;
    g_GzipOutputBuffer = (unsigned char *)output;
    g_GzipInputOffset = 0;
    g_GzipOutputOffset = 0;
    g_GzipMemPool = (int)frame.scratch;
    if ((g_GzipInputBuffer[g_GzipInputOffset++] != 0x1f) |
        (g_GzipInputBuffer[g_GzipInputOffset++] != 0x8b)) {
        return -1;
    }
    if (g_GzipInputBuffer[g_GzipInputOffset++] != 8) {
        return -1;
    }
    if (g_GzipInputBuffer[g_GzipInputOffset++] != 0) {
        return -1;
    }
    g_GzipInputOffset++;
    g_GzipInputOffset++;
    g_GzipInputOffset++;
    g_GzipInputOffset++;
    g_GzipInputOffset++;
    g_GzipInputOffset++;
    frame.status = GzipInflate_DecodeBlocks();
    if (frame.status == 3) {
        return -1;
    }
    if (frame.status != 0) {
        return -1;
    }
    for (frame.trailerIndex = 0; frame.trailerIndex < 8; frame.trailerIndex++) {
        frame.trailer[frame.trailerIndex] = g_GzipInputBuffer[g_GzipInputOffset];
        g_GzipInputOffset++;
    }
    frame.crc = (frame.trailer[0] | frame.trailer[1] << 8) | (frame.trailer[2] | frame.trailer[3] << 8) << 16;
    frame.count = (frame.trailer[4] | frame.trailer[5] << 8) | (frame.trailer[6] | frame.trailer[7] << 8) << 16;
    if (frame.count != g_GzipOutputOffset) {
        return -1;
    }
    return frame.count;
}

/*
 * Repeatedly decodes deflate blocks until the last block (final-block flag) is encountered.
 * Returns 0 on success, or nonzero error code.
 */
// FUNCTION: FF7 0x0041d570
int __cdecl GzipInflate_DecodeBlocks(void)
{
    int isLast;
    int state;

    g_GzipOutputOffset = 0;
    g_GzipBitCount = 0;
    g_GzipBitBuffer = 0;
    do {
        g_GzipTotalHuftNodes = 0;
        state = GzipInflate_DecodeBlock(&isLast);
        if (state != 0) {
            return state;
        }
    } while (isLast == 0);
    while ((unsigned int)g_GzipBitCount >= (unsigned int)8) {
        g_GzipBitCount -= 8;
        g_GzipInputOffset--;
    }
    return 0;
}

/*
 * Reads the 1-bit final-block flag and 2-bit block type from the bitstream,
 * then dispatches to stored (0), fixed Huffman (1), or dynamic Huffman (2) decoders.
 * Returns decoder status code.
 */
// FUNCTION: FF7 0x0041d5ed
int __cdecl GzipInflate_DecodeBlock(void *finalFlag)
{
    int type;
    unsigned int bitBuf;
    int bitsLeft;

    bitBuf = g_GzipBitBuffer;
    bitsLeft = g_GzipBitCount;
    while ((unsigned int)bitsLeft < (unsigned int)1) {
        bitBuf |= g_GzipInputBuffer[g_GzipInputOffset] << bitsLeft;
        g_GzipInputOffset++;
        bitsLeft += 8;
    }
    *(int *)finalFlag = bitBuf & 1;
    bitBuf >>= 1;
    bitsLeft--;
    while ((unsigned int)bitsLeft < (unsigned int)2) {
        bitBuf |= g_GzipInputBuffer[g_GzipInputOffset] << bitsLeft;
        g_GzipInputOffset++;
        bitsLeft += 8;
    }
    type = bitBuf & 3;
    bitBuf >>= 2;
    bitsLeft -= 2;
    g_GzipBitBuffer = bitBuf;
    g_GzipBitCount = bitsLeft;
    if (type == 2) {
        return GzipInflate_DecodeDynamicBlock();
    } else if (type == 0) {
        return GzipInflate_CopyStoredBlock();
    } else if (type == 1) {
        return GzipInflate_DecodeFixedBlock();
    } else {
        return 2;
    }
}

/*
 * Decodes an uncompressed (stored) deflate block. Discards bits to align to byte
 * boundary, validates length and one's complement length, then copies bytes directly
 * to the output stream. Returns 0 on success, or 1 on length mismatch.
 */
// FUNCTION: FF7 0x0041d6f7
int __cdecl GzipInflate_CopyStoredBlock(void)
{
    unsigned int bitBuf;
    int bitsLeft;
    int outPos;
    int len;

    bitBuf = g_GzipBitBuffer;
    bitsLeft = g_GzipBitCount;
    outPos = g_GzipOutputOffset;
    len = bitsLeft & 7;
    bitBuf >>= len;
    bitsLeft -= len;
    while ((unsigned int)bitsLeft < (unsigned int)0x10) {
        bitBuf |= g_GzipInputBuffer[g_GzipInputOffset] << bitsLeft;
        g_GzipInputOffset++;
        bitsLeft += 8;
    }
    len = bitBuf & 0xFFFF;
    bitBuf >>= 16;
    bitsLeft -= 0x10;
    while ((unsigned int)bitsLeft < (unsigned int)0x10) {
        bitBuf |= g_GzipInputBuffer[g_GzipInputOffset] << bitsLeft;
        g_GzipInputOffset++;
        bitsLeft += 8;
    }
    if (len != (~bitBuf & 0xFFFF)) {
        return 1;
    } else {
        bitBuf >>= 16;
        bitsLeft -= 0x10;
        while (len-- != 0) {
            while ((unsigned int)bitsLeft < (unsigned int)8) {
                bitBuf |= g_GzipInputBuffer[g_GzipInputOffset] << bitsLeft;
                g_GzipInputOffset++;
                bitsLeft += 8;
            }
            g_GzipOutputBuffer[outPos] = bitBuf;
            outPos++;
            bitBuf >>= 8;
            bitsLeft -= 8;
        }
        g_GzipOutputOffset = outPos;
        g_GzipBitBuffer = bitBuf;
        g_GzipBitCount = bitsLeft;
        return 0;
    }
}

/*
 * Decodes a block compressed with fixed (predefined) Huffman tables:
 * 288 literal/length symbols and 32 distance symbols.
 * Returns 0 on success, or nonzero on error.
 */
// FUNCTION: FF7 0x0041d88f
int __cdecl GzipInflate_DecodeFixedBlock(void)
{
    struct {
        unsigned int lengths[288];
        GzipHuffmanNode *distance;
        unsigned int literal_bits;
        int status;
        GzipHuffmanNode *literal;
        unsigned int distance_bits;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    for (frame.status = 0; frame.status < 0x90; frame.status++) {
        frame.lengths[frame.status] = 8;
    }
    for (; frame.status < 0x100; frame.status++) {
        frame.lengths[frame.status] = 9;
    }
    for (; frame.status < 0x118; frame.status++) {
        frame.lengths[frame.status] = 7;
    }
    for (; frame.status < 0x120; frame.status++) {
        frame.lengths[frame.status] = 8;
    }
    frame.literal_bits = 7;
    frame.status = GzipInflate_BuildHuffmanTable(frame.lengths, 288, 257, g_GzipCopyLengths, g_GzipCopyLengthExtraBits, &frame.literal, &frame.literal_bits);
    if (frame.status != 0) {
        return frame.status;
    } else {
        for (frame.status = 0; frame.status < 30; frame.status++) {
            frame.lengths[frame.status] = 5;
        }
        frame.distance_bits = 5;
        frame.status = GzipInflate_BuildHuffmanTable(frame.lengths, 30, 0, g_GzipCopyDistances, g_GzipCopyDistanceExtraBits, &frame.distance, &frame.distance_bits);
        if (frame.status > 1) {
            GzipInflate_WalkHuffmanChain(frame.literal);
            return frame.status;
        } else if (GzipInflate_DecodeHuffmanCodes(frame.literal, frame.distance, frame.literal_bits, frame.distance_bits) != 0) {
            return 1;
        } else {
            GzipInflate_WalkHuffmanChain(frame.literal);
            GzipInflate_WalkHuffmanChain(frame.distance);
            return 0;
        }
    }
}

/*
 * Builds a Huffman decoding table from given bit lengths using Canonical Huffman coding.
 * Allocates tables from the scratch memory pool, populating direct entries, extra-bit entries,
 * or sub-tables. Returns 0 on success, 1 on incomplete code, or 3 on allocation failure.
 */
// FUNCTION: FF7 0x0041da21
int __cdecl GzipInflate_BuildHuffmanTable(unsigned int *bitLengths, unsigned int n, unsigned int s, unsigned short *copyBases, unsigned short *extraBits, GzipHuffmanNode **t, unsigned int *m)
{
    struct {
        int zero;
        unsigned int lookupBits;
        int codeLen;
        unsigned int lenIdx, tableSize, codeWord;
        int dummyCodes, tableLevel;
        unsigned int bitOffsets[17];
        int maxCodeLen, prevBits;
        unsigned int *offsetPtr;
        unsigned int step;
        unsigned int values[288];
        GzipHuffmanNode *tableStack[16];
        unsigned int bitLengthCount[17];
        GzipHuffmanNode entry;
        unsigned int prefixCount;
        GzipHuffmanNode *subTable;
        unsigned int *valPtr;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    for (f.zero = 0; f.zero < 17; f.zero++)
        f.bitLengthCount[f.zero] = 0;
    f.valPtr = bitLengths;
    f.codeWord = n;
    do {
        f.bitLengthCount[*f.valPtr]++;
        f.valPtr++;
        f.codeWord--;
    } while (f.codeWord);
    if (f.bitLengthCount[0] == n) {
        *t = 0;
        *m = 0;
        return 0;
    }
    f.lookupBits = *m;
    for (f.lenIdx = 1; f.lenIdx <= 16; f.lenIdx++) {
        if (f.bitLengthCount[f.lenIdx])
            break;
    }
    f.codeLen = f.lenIdx;
    if (f.lookupBits < f.lenIdx)
        f.lookupBits = f.lenIdx;
    for (f.codeWord = 16; f.codeWord; f.codeWord--) {
        if (f.bitLengthCount[f.codeWord])
            break;
    }
    f.maxCodeLen = f.codeWord;
    if (f.lookupBits > f.codeWord)
        f.lookupBits = f.codeWord;
    *m = f.lookupBits;
    for (f.dummyCodes = 1 << f.lenIdx; f.lenIdx < f.codeWord; f.lenIdx++, f.dummyCodes <<= 1) {
        f.dummyCodes -= f.bitLengthCount[f.lenIdx];
        if (f.dummyCodes < 0)
            return 2;
    }
    f.dummyCodes -= f.bitLengthCount[f.codeWord];
    if (f.dummyCodes < 0)
        return 2;
    f.bitLengthCount[f.codeWord] += f.dummyCodes;
    f.lenIdx = 0;
    f.bitOffsets[1] = f.lenIdx;
    f.valPtr = f.bitLengthCount + 1;
    f.offsetPtr = f.bitOffsets + 2;
    while (--f.codeWord) {
        f.lenIdx += *f.valPtr;
        *f.offsetPtr = f.lenIdx;
        f.offsetPtr++;
        f.valPtr++;
    }
    f.valPtr = bitLengths;
    f.codeWord = 0;
    do {
        if ((f.lenIdx = *f.valPtr++) != 0) {
            f.values[f.bitOffsets[f.lenIdx]] = f.codeWord;
            f.bitOffsets[f.lenIdx]++;
        }
        f.codeWord++;
    } while (f.codeWord < n);
    f.codeWord = 0;
    f.bitOffsets[0] = f.codeWord;
    f.valPtr = f.values;
    f.tableLevel = -1;
    f.prevBits = -(int)f.lookupBits;
    f.tableStack[0] = 0;
    f.subTable = 0;
    f.tableSize = 0;
    for (; f.codeLen <= f.maxCodeLen; f.codeLen++) {
        f.prefixCount = f.bitLengthCount[f.codeLen];
        while (f.prefixCount--) {
            while (f.codeLen > f.prevBits + (int)f.lookupBits) {
                f.tableLevel++;
                f.prevBits += f.lookupBits;
                f.tableSize = f.maxCodeLen - f.prevBits;
                f.tableSize = f.tableSize > f.lookupBits ? f.lookupBits : f.tableSize;
                f.lenIdx = f.codeLen - f.prevBits;
                f.step = 1 << f.lenIdx;
                if (f.step > f.prefixCount + 1) {
                    f.step -= f.prefixCount + 1;
                    f.offsetPtr = f.bitLengthCount + f.codeLen;
                    while (++f.lenIdx < f.tableSize) {
                        f.step <<= 1;
                        f.offsetPtr++;
                        if (f.step <= *f.offsetPtr)
                            break;
                        f.step -= *f.offsetPtr;
                    }
                }
                f.tableSize = 1 << f.lenIdx;
                f.subTable = (GzipHuffmanNode *)GzipInflate_AllocFromPool((f.tableSize + 1) * 8);
                if (f.subTable == 0) {
                    if (f.tableLevel)
                        GzipInflate_WalkHuffmanChain(f.tableStack[0]);
                    return 3;
                }
                g_GzipTotalHuftNodes += f.tableSize + 1;
                *t = f.subTable + 1;
                t = &f.subTable->v.t;
                *t = 0;
                f.subTable++;
                f.tableStack[f.tableLevel] = f.subTable;
                if (f.tableLevel) {
                    f.bitOffsets[f.tableLevel] = f.codeWord;
                    f.entry.b = (unsigned char)f.lookupBits;
                    f.entry.e = (unsigned char)(f.lenIdx + 16);
                    f.entry.v.t = f.subTable;
                    f.lenIdx = f.codeWord >> (f.prevBits - f.lookupBits);
                    f.tableStack[f.tableLevel - 1][f.lenIdx] = f.entry;
                }
            }
            f.entry.b = (unsigned char)(f.codeLen - f.prevBits);
            if (f.valPtr >= f.values + n)
                f.entry.e = 99;
            else if (*f.valPtr < s) {
                f.entry.e = (unsigned char)((*f.valPtr < 256) + 15);
                f.entry.v.n = (unsigned short)*f.valPtr;
                f.valPtr++;
            } else {
                f.entry.e = (unsigned char)extraBits[*f.valPtr - s];
                f.entry.v.n = copyBases[*f.valPtr - s];
                f.valPtr++;
            }
            f.step = 1 << (f.codeLen - f.prevBits);
            for (f.lenIdx = f.codeWord >> f.prevBits; f.lenIdx < f.tableSize; f.lenIdx += f.step)
                f.subTable[f.lenIdx] = f.entry;
            for (f.lenIdx = 1 << (f.codeLen - 1); f.codeWord & f.lenIdx; f.lenIdx >>= 1)
                f.codeWord ^= f.lenIdx;
            f.codeWord ^= f.lenIdx;
            while ((f.codeWord & ((1 << f.prevBits) - 1)) != f.bitOffsets[f.tableLevel]) {
                f.tableLevel--;
                f.prevBits -= f.lookupBits;
            }
        }
    }
    return f.dummyCodes != 0 && f.maxCodeLen != 1;
}

/*
 * Allocates a block of memory from the scratch memory pool (g_GzipMemPool),
 * rounded up to 4-byte dwords. Resets to pool base if requested size exceeds remaining capacity.
 * Returns the pool pointer.
 */
// FUNCTION: FF7 0x0041e1ea
int __cdecl GzipInflate_AllocFromPool(int size)
{
    int result;
    int dwordCount;

    result = 0;
    dwordCount = ((unsigned int)size + 3) >> 2;
    if ((unsigned int)(g_GzipMemPoolOffset + dwordCount) <= (unsigned int)0x1000) {
        result = g_GzipMemPool + g_GzipMemPoolOffset * 4;
        g_GzipMemPoolOffset += dwordCount;
    } else {
        g_GzipMemPoolOffset = 0;
        result = GzipInflate_AllocFromPool(size);
    }
    return result;
}

/*
 * Traverses a chain of allocated Huffman tables backwards and frees them.
 * Always returns 0.
 */
// FUNCTION: FF7 0x0041e256
int __cdecl GzipInflate_WalkHuffmanChain(void *node)
{
    void *p;
    void *next;

    p = node;
    while (p != 0) {
        p = (char *)p - 8;
        next = *(void **)((char *)p + 4);
        p = next;
    }
    return 0;
}

/*
 * Inflates deflate compressed codes using literal/length and distance Huffman tables.
 * Decodes literal bytes to output or resolves length/distance back-references and copies
 * previously decoded bytes from the output sliding window. Returns 0 on end-of-block, or 1 on error.
 */
// FUNCTION: FF7 0x0041e288
int __cdecl GzipInflate_DecodeHuffmanCodes(GzipHuffmanNode *tl, GzipHuffmanNode *td, unsigned bl, unsigned bd)
{
    struct {
        unsigned copyLen, distMask, bitCount, outOffset, litMask, extraBits, copyOffset;
        GzipHuffmanNode *node;
        unsigned bitBuffer;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.bitBuffer = g_GzipBitBuffer;
    f.bitCount = g_GzipBitCount;
    f.outOffset = g_GzipOutputOffset;
    f.litMask = g_GzipMaskBits[bl];
    f.distMask = g_GzipMaskBits[bd];
    for (;;) {
        while (f.bitCount < bl) {
            f.bitBuffer |= g_GzipInputBuffer[g_GzipInputOffset] << f.bitCount;
            g_GzipInputOffset++;
            f.bitCount += 8;
        }
        f.node = tl + (f.bitBuffer & f.litMask);
        f.extraBits = f.node->e;
        if (f.extraBits > 16) {
            do {
                if (f.extraBits == 99)
                    return 1;
                f.bitBuffer >>= f.node->b;
                f.bitCount -= f.node->b;
                f.extraBits -= 16;
                while (f.bitCount < f.extraBits) {
                    f.bitBuffer |= g_GzipInputBuffer[g_GzipInputOffset] << f.bitCount;
                    g_GzipInputOffset++;
                    f.bitCount += 8;
                }
                f.node = f.node->v.t + (f.bitBuffer & g_GzipMaskBits[f.extraBits]);
                f.extraBits = f.node->e;
            } while (f.extraBits > 16);
        }
        f.bitBuffer >>= f.node->b;
        f.bitCount -= f.node->b;
        if (f.extraBits == 16) {
            g_GzipOutputBuffer[f.outOffset] = *(unsigned char *)&f.node->v.n;
            f.outOffset++;
        } else {
            if (f.extraBits == 15)
                break;
            while (f.bitCount < f.extraBits) {
                f.bitBuffer |= g_GzipInputBuffer[g_GzipInputOffset] << f.bitCount;
                g_GzipInputOffset++;
                f.bitCount += 8;
            }
            f.copyLen = f.node->v.n + (f.bitBuffer & g_GzipMaskBits[f.extraBits]);
            f.bitBuffer >>= f.extraBits;
            f.bitCount -= f.extraBits;
            while (f.bitCount < bd) {
                f.bitBuffer |= g_GzipInputBuffer[g_GzipInputOffset] << f.bitCount;
                g_GzipInputOffset++;
                f.bitCount += 8;
            }
            f.node = td + (f.bitBuffer & f.distMask);
            f.extraBits = f.node->e;
            if (f.extraBits > 16) {
                do {
                    if (f.extraBits == 99)
                        return 1;
                    f.bitBuffer >>= f.node->b;
                    f.bitCount -= f.node->b;
                    f.extraBits -= 16;
                    while (f.bitCount < f.extraBits) {
                        f.bitBuffer |= g_GzipInputBuffer[g_GzipInputOffset] << f.bitCount;
                        g_GzipInputOffset++;
                        f.bitCount += 8;
                    }
                    f.node = f.node->v.t + (f.bitBuffer & g_GzipMaskBits[f.extraBits]);
                    f.extraBits = f.node->e;
                } while (f.extraBits > 16);
            }
            f.bitBuffer >>= f.node->b;
            f.bitCount -= f.node->b;
            while (f.bitCount < f.extraBits) {
                f.bitBuffer |= g_GzipInputBuffer[g_GzipInputOffset] << f.bitCount;
                g_GzipInputOffset++;
                f.bitCount += 8;
            }
            f.copyOffset = f.outOffset - f.node->v.n - (f.bitBuffer & g_GzipMaskBits[f.extraBits]);
            f.bitBuffer >>= f.extraBits;
            f.bitCount -= f.extraBits;
            f.extraBits = f.copyLen;
            if (f.extraBits != 0) {
                do {
                    g_GzipOutputBuffer[f.outOffset] = g_GzipOutputBuffer[f.copyOffset];
                    f.outOffset++;
                    f.copyOffset++;
                    f.extraBits--;
                } while (f.extraBits != 0);
            }
            f.copyLen = 0;
        }
    }
    g_GzipOutputOffset = f.outOffset;
    g_GzipBitBuffer = f.bitBuffer;
    g_GzipBitCount = f.bitCount;
    return 0;
}

/*
 * Decodes a dynamic Huffman compressed block. Reads the number of literal, distance,
 * and code-length codes, builds the code-length Huffman table, decodes the literal
 * and distance code lengths, builds their respective tables, and inflates the block data.
 * Returns 0 on success, or nonzero error code.
 */
// FUNCTION: FF7 0x0041e6a8
int __cdecl GzipInflate_DecodeDynamicBlock(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[1320];
        int totalCodes;
        struct { unsigned char pad_to_4[0x4]; int tableMask; };
        struct { unsigned char pad_to_8[0x8]; int prevLength; };
        struct { unsigned char pad_to_C[0xC]; int tableEntry; };
        struct { unsigned char pad_to_10[0x10]; int bitCount; };
        struct { unsigned char pad_to_14[0x14]; int tmpIndex; };
        struct { unsigned char pad_to_18[0x18]; int tableBits; };
        struct { unsigned char pad_to_1C[0x1C]; int numLitCodes; };
        struct { unsigned char pad_to_20[0x20]; int status; };
        struct { unsigned char pad_to_24[0x24]; int litTable; };
        struct { unsigned char pad_to_28[0x28]; int distTableBits; };
        struct { unsigned char pad_to_2C[0x2C]; int numDistCodes; };
        struct { unsigned char pad_to_30[0x30]; int codeLengths[318]; };
        struct { unsigned char pad_to_520[0x520]; unsigned int bitBuffer; };
        struct { unsigned char pad_to_524[0x524]; int numBorderCodes; };
    } frame;
#pragma pack(pop)

    frame.bitBuffer = g_GzipBitBuffer;
    frame.bitCount = g_GzipBitCount;
    while ((unsigned int)frame.bitCount < (unsigned int)5) {
        frame.bitBuffer |= g_GzipInputBuffer[g_GzipInputOffset] << frame.bitCount;
        g_GzipInputOffset++;
        frame.bitCount += 8;
    }
    frame.numLitCodes = (frame.bitBuffer & 0x1F) + 0x101;
    frame.bitBuffer >>= 5;
    frame.bitCount -= 5;
    while ((unsigned int)frame.bitCount < (unsigned int)5) {
        frame.bitBuffer |= g_GzipInputBuffer[g_GzipInputOffset] << frame.bitCount;
        g_GzipInputOffset++;
        frame.bitCount += 8;
    }
    frame.numDistCodes = (frame.bitBuffer & 0x1F) + 1;
    frame.bitBuffer >>= 5;
    frame.bitCount -= 5;
    while ((unsigned int)frame.bitCount < (unsigned int)4) {
        frame.bitBuffer |= g_GzipInputBuffer[g_GzipInputOffset] << frame.bitCount;
        g_GzipInputOffset++;
        frame.bitCount += 8;
    }
    frame.numBorderCodes = (frame.bitBuffer & 0xF) + 4;
    frame.bitBuffer >>= 4;
    frame.bitCount -= 4;
    if ((unsigned int)frame.numLitCodes > (unsigned int)0x11E || (unsigned int)frame.numDistCodes > (unsigned int)0x1E) {
        return 1;
    } else {
        for (frame.tmpIndex = 0; (unsigned int)frame.tmpIndex < (unsigned int)frame.numBorderCodes; frame.tmpIndex++) {
            while ((unsigned int)frame.bitCount < (unsigned int)3) {
                frame.bitBuffer |= g_GzipInputBuffer[g_GzipInputOffset] << frame.bitCount;
                g_GzipInputOffset++;
                frame.bitCount += 8;
            }
            frame.codeLengths[g_GzipBorderOrder[frame.tmpIndex]] = frame.bitBuffer & 7;
            frame.bitBuffer >>= 3;
            frame.bitCount -= 3;
        }
        for (; (unsigned int)frame.tmpIndex < (unsigned int)0x13; frame.tmpIndex++) {
            frame.codeLengths[g_GzipBorderOrder[frame.tmpIndex]] = 0;
        }
        frame.tableBits = 7;
        frame.status = GzipInflate_BuildHuffmanTable((unsigned *)&frame.codeLengths[0], 19, 0x13, 0, 0, (GzipHuffmanNode **)&frame.litTable, (unsigned *)&frame.tableBits);
        if (frame.status != 0) {
            if (frame.status == 1) {
                GzipInflate_WalkHuffmanChain((GzipHuffmanNode *)frame.litTable);
            }
            return frame.status;
        } else {
            frame.totalCodes = frame.numLitCodes + frame.numDistCodes;
            frame.tableMask = g_GzipMaskBits[frame.tableBits];
            frame.prevLength = 0;
            frame.status = frame.prevLength;
            while ((unsigned int)frame.status < (unsigned int)frame.totalCodes) {
                while ((unsigned int)frame.bitCount < (unsigned int)frame.tableBits) {
                    frame.bitBuffer |= g_GzipInputBuffer[g_GzipInputOffset] << frame.bitCount;
                    g_GzipInputOffset++;
                    frame.bitCount += 8;
                }
                frame.tableEntry = frame.litTable + (frame.bitBuffer & frame.tableMask) * 8;
                frame.tmpIndex = ((BattleGzipEntry *)frame.tableEntry)->codeBits;
                frame.bitBuffer >>= frame.tmpIndex;
                frame.bitCount -= frame.tmpIndex;
                frame.tmpIndex = ((BattleGzipEntry *)frame.tableEntry)->codeValue;
                if ((unsigned int)frame.tmpIndex < (unsigned int)0x10) {
                    frame.prevLength = frame.tmpIndex;
                    frame.codeLengths[frame.status] = frame.prevLength;
                    frame.status++;
                } else if (frame.tmpIndex == 16) {
                    while ((unsigned int)frame.bitCount < (unsigned int)2) {
                        frame.bitBuffer |= g_GzipInputBuffer[g_GzipInputOffset] << frame.bitCount;
                        g_GzipInputOffset++;
                        frame.bitCount += 8;
                    }
                    frame.tmpIndex = (frame.bitBuffer & 3) + 3;
                    frame.bitBuffer >>= 2;
                    frame.bitCount -= 2;
                    if ((unsigned int)(frame.status + frame.tmpIndex) > (unsigned int)frame.totalCodes) {
                        return 1;
                    }
                    while (frame.tmpIndex-- != 0) {
                        frame.codeLengths[frame.status] = frame.prevLength;
                        frame.status++;
                    }
                } else if (frame.tmpIndex == 17) {
                    while ((unsigned int)frame.bitCount < (unsigned int)3) {
                        frame.bitBuffer |= g_GzipInputBuffer[g_GzipInputOffset] << frame.bitCount;
                        g_GzipInputOffset++;
                        frame.bitCount += 8;
                    }
                    frame.tmpIndex = (frame.bitBuffer & 7) + 3;
                    frame.bitBuffer >>= 3;
                    frame.bitCount -= 3;
                    if ((unsigned int)(frame.status + frame.tmpIndex) > (unsigned int)frame.totalCodes) {
                        return 1;
                    }
                    while (frame.tmpIndex-- != 0) {
                        frame.codeLengths[frame.status] = 0;
                        frame.status++;
                    }
                    frame.prevLength = 0;
                } else {
                    while ((unsigned int)frame.bitCount < (unsigned int)7) {
                        frame.bitBuffer |= g_GzipInputBuffer[g_GzipInputOffset] << frame.bitCount;
                        g_GzipInputOffset++;
                        frame.bitCount += 8;
                    }
                    frame.tmpIndex = (frame.bitBuffer & 0x7F) + 11;
                    frame.bitBuffer >>= 7;
                    frame.bitCount -= 7;
                    if ((unsigned int)(frame.status + frame.tmpIndex) > (unsigned int)frame.totalCodes) {
                        return 1;
                    }
                    while (frame.tmpIndex-- != 0) {
                        frame.codeLengths[frame.status] = 0;
                        frame.status++;
                    }
                    frame.prevLength = 0;
                }
            }
            GzipInflate_WalkHuffmanChain((GzipHuffmanNode *)frame.litTable);
            g_GzipBitBuffer = frame.bitBuffer;
            g_GzipBitCount = frame.bitCount;
            frame.tableBits = g_GzipLiteralTableBits;
            frame.status = GzipInflate_BuildHuffmanTable((unsigned *)&frame.codeLengths[0], frame.numLitCodes, 0x101, g_GzipCopyLengths, g_GzipCopyLengthExtraBits, (GzipHuffmanNode **)&frame.litTable, (unsigned *)&frame.tableBits);
            if (frame.status != 0) {
                if (frame.status == 1) {
                    return -1;
                }
                return frame.status;
            } else {
                // Empty conditions preserve the measured VC5 register cycle.
                if (REG_ALLOC_HINT) {
                }
                frame.distTableBits = g_GzipDistanceTableBits;
                frame.status = GzipInflate_BuildHuffmanTable((unsigned *)&frame.codeLengths[frame.numLitCodes], frame.numDistCodes, 0, g_GzipCopyDistances, g_GzipCopyDistanceExtraBits, (GzipHuffmanNode **)&frame.tableEntry, (unsigned *)&frame.distTableBits);
                if (frame.status != 0) {
                    if (frame.status == 1) {
                        return -1;
                    }
                    // Empty conditions preserve the measured VC5 register cycle.
                    if (REG_ALLOC_HINT) {
                    }
                    GzipInflate_WalkHuffmanChain((GzipHuffmanNode *)frame.litTable);
                    return frame.status;
                } else if (GzipInflate_DecodeHuffmanCodes((GzipHuffmanNode *)frame.litTable, (GzipHuffmanNode *)frame.tableEntry, frame.tableBits, frame.distTableBits) != 0) {
                    return 1;
                } else {
                    GzipInflate_WalkHuffmanChain((GzipHuffmanNode *)frame.litTable);
                    GzipInflate_WalkHuffmanChain((GzipHuffmanNode *)frame.tableEntry);
                    return 0;
                }
            }
        }
    }
}

} /* extern "C" */
