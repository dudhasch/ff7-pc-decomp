#ifndef LIB_CODEGEN_H
#define LIB_CODEGEN_H

/* Spellings the period compiler needs, named for the reader.
 * Each macro expands to exactly the tokens the code had before, so no object changes. */

/* A constant that VC5 must not fold. `(0, x)` is a comma expression, which the compiler does not
 * treat as a constant: `(UNFOLDED(6) << 2)` loads 6 into a register and shifts it, as the original
 * code does, where a plain `6 << 2` would fold into an address offset; `while (UNFOLDED(1))` and
 * `UNFOLDED(0) ? a : b` keep their test. What the original source wrote there is not known; a comma
 * is the only C found that compiles to those bytes. */
#define UNFOLDED(x) (0, x)

/* FRAME UNION. Many functions keep their locals in one local `union { ... } frame;`, one arm per
 * stack slot: `struct { unsigned char pad_to_E8[0xE8]; T *name; };` puts `name` at byte 0xE8 of
 * the frame. VC5 /Od gives each named local its own slot in an order it derives from a hash of the
 * name, so separate locals could not reach the slot layout the retail code has: overlapping slots, a short at an odd offset, one
 * slot read at two widths. The union is one local, so its arms name the original slots without
 * moving any of them. Read `frame.name` as an ordinary local; the `pad_to_XX` members only place it.
 * Local names are chosen in the same 16-bucket VC5 identifier hash so each spelling keeps the exact
 * stack slot the retail code uses. For the same reason a multi-local frame union is not always called
 * `frame`: about 190 functions name it `local`, `f`, `s` or `v`, because changing its hash bucket
 * would move it and every other local of the function. */

/* EMPTY FIRST ARMS. `if (x != 0) {} else { body; }` is how VC5 /Od emits the retail
 * `je body; jmp join` pair: a jump into the body and a second jump past it. The one-arm spelling
 * `if (x == 0) { body; }` gives a single jump, which is not the retail code. Read it as
 * `if (!(x)) { body; }`.
 *
 * /Op PAIRS. `#pragma optimize("p", off)` ... `#pragma optimize("p", on)` switches VC5's
 * "improve float consistency" option (/Op) for the functions between them. With /Op every float
 * result is stored to memory and loaded back; without it values stay on the x87 stack. The retail
 * objects mixed the two settings function by function, so a TU built with /Op (see
 * config/objects.tsv) turns it off around the functions the retail build compiled without
 * it, and a TU built without /Op turns it on after such a pair for the functions that had it. */

/* Conditions that are always false. VC5 does not fold a constant condition at /Od: `if (0)`
 * compiles to a test of a zeroed register and a jump past the block, and the block is in the image.
 * Three kinds:
 *   DEBUG_TRACE     the block is the original's debug output, switched off in the retail build;
 *                   its format strings are in the retail data (their STRING markers);
 *   REG_ALLOC_HINT  the block is empty: it is there for the register choices VC5 makes around
 *                   it;
 *   DEAD_IN_RETAIL  any other block: code the retail image carries and never runs. */
#define DEBUG_TRACE 0
#define REG_ALLOC_HINT 0
#define DEAD_IN_RETAIL 0

/* The bits of 1.0f, where the code stores or compares a float through an int (an int member or
 * a cast); the original's code there is an integer store of this constant. */
#define FLOAT_ONE_BITS 0x3F800000

/* Little-endian four-character chunk or tag constant (RIFF, WAVE, MThd, MTrk, ...), typed as signed int. */
#define MAKE_FOURCC(a, b, c, d) \
    ((int)(unsigned char)(a) | ((int)(unsigned char)(b) << 8) | ((int)(unsigned char)(c) << 16) | ((int)(unsigned char)(d) << 24))

/* The bits of IEEE-754 single-precision floats stored or compared through integer fields.
 * The name spells the value, `_` for the decimal point. */
#define FLOAT_MINUS_ONE_BITS 0xBF800000
#define FLOAT_0_001_BITS 0x3A83126F
#define FLOAT_0_005_BITS 0x3BA3D70A
#define FLOAT_0_01_BITS 0x3C23D70A
#define FLOAT_0_1_BITS 0x3DCCCCCD
#define FLOAT_0_125_BITS 0x3E000000
#define FLOAT_0_15_BITS 0x3E19999A
#define FLOAT_0_18_BITS 0x3E3851EC
#define FLOAT_0_2_BITS 0x3E4CCCCD
#define FLOAT_0_222_BITS 0x3E6353F8
#define FLOAT_0_25_BITS 0x3E800000
#define FLOAT_0_3_BITS 0x3E99999A
#define FLOAT_0_375_BITS 0x3EC00000
#define FLOAT_0_4_BITS 0x3ECCCCCD
#define FLOAT_0_401_BITS 0x3ECD4FDF
#define FLOAT_0_402_BITS 0x3ECDD2F2
#define FLOAT_HALF_BITS 0x3F000000
#define FLOAT_0_6_BITS 0x3F19999A
#define FLOAT_0_625_BITS 0x3F200000
#define FLOAT_0_75_BITS 0x3F400000
#define FLOAT_0_875_BITS 0x3F600000
#define FLOAT_0_9_BITS 0x3F666666
#define FLOAT_0_95_BITS 0x3F733333
#define FLOAT_0_952_BITS 0x3F73B646
#define FLOAT_0_974_BITS 0x3F795810
#define FLOAT_0_995_BITS 0x3F7EB852
#define FLOAT_0_997_BITS 0x3F7F3B64
#define FLOAT_0_999_BITS 0x3F7FBE77
#define FLOAT_0_9999_BITS 0x3F7FF972
#define FLOAT_0_99999_BITS 0x3F7FFF58
#define FLOAT_1_875_BITS 0x3FF00000
#define FLOAT_2_BITS 0x40000000
#define FLOAT_12_BITS 0x41400000
#define FLOAT_16_BITS 0x41800000
#define FLOAT_20_BITS 0x41A00000
#define FLOAT_90_BITS 0x42B40000
#define FLOAT_100_BITS 0x42C80000
#define FLOAT_120_BITS 0x42F00000
#define FLOAT_128_BITS 0x43000000
#define FLOAT_132_BITS 0x43040000
#define FLOAT_160_BITS 0x43200000
#define FLOAT_166_BITS 0x43260000
#define FLOAT_176_BITS 0x43300000
#define FLOAT_180_BITS 0x43340000
#define FLOAT_185_BITS 0x43390000
#define FLOAT_220_BITS 0x435C0000
#define FLOAT_224_BITS 0x43600000
#define FLOAT_228_BITS 0x43640000
#define FLOAT_232_BITS 0x43680000
#define FLOAT_255_BITS 0x437F0000
#define FLOAT_256_BITS 0x43800000
#define FLOAT_272_BITS 0x43880000
#define FLOAT_276_BITS 0x438A0000
#define FLOAT_288_BITS 0x43900000
#define FLOAT_304_BITS 0x43980000
#define FLOAT_320_BITS 0x43A00000
#define FLOAT_340_BITS 0x43AA0000
#define FLOAT_348_BITS 0x43AE0000
#define FLOAT_354_BITS 0x43B10000
#define FLOAT_376_BITS 0x43BC0000
#define FLOAT_408_BITS 0x43CC0000
#define FLOAT_426_BITS 0x43D50000
#define FLOAT_432_BITS 0x43D80000
#define FLOAT_440_BITS 0x43DC0000
#define FLOAT_464_BITS 0x43E80000
#define FLOAT_480_BITS 0x43F00000
#define FLOAT_485_BITS 0x43F28000
#define FLOAT_501_BITS 0x43FA8000
#define FLOAT_512_BITS 0x44000000
#define FLOAT_625_BITS 0x441C4000
#define FLOAT_640_BITS 0x44200000
#define FLOAT_1250_BITS 0x449C4000
#define FLOAT_1875_BITS 0x44EA6000
#define FLOAT_2500_BITS 0x451C4000
#define FLOAT_3072_BITS 0x45400000
#define FLOAT_4096_BITS 0x45800000
#define FLOAT_39950_BITS 0x471C0E00
#define FLOAT_80000_BITS 0x479C4000
#define FLOAT_16777216_BITS 0x4B800000

/* Aggregate-copy wrappers where VC5 /Od emits fixed-width struct moves instead of scalar loads/stores. */
typedef struct Int64Pair {
    int lo;
    int hi;
} Int64Pair;

typedef struct Int32Value {
    int value;
} Int32Value;

typedef struct ShortQuad {
    short v[4];
} ShortQuad;

/* 4-byte wrapper struct forcing VC5's aggregate copy path instead of a scalar register load/store. */
typedef struct DirectWord {
    int v;
} DirectWord;


#endif /* LIB_CODEGEN_H */
