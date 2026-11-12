/* include/ff7/types/credits.h: the types ff7/credits TUs share. */
#ifndef FF7_TYPES_CREDITS_H
#define FF7_TYPES_CREDITS_H


struct GfxRenderObject;
/* One 0x40-byte people.bin credits record (credfile.cpp strides them by << 6); reset to its default
 * values by Credits_ResetRecordDefaults. */
typedef struct CreditsLineRecord {
    int drawerKind;
    int color;
    int posX;
    char textGlyph0;
    char textGlyph1;
    char textGlyph2;
    char textGlyph3;
    char textGlyph4;
    char textGlyph5;
    unsigned char pad_12[0x2a];
    int wrapWidth;
} CreditsLineRecord;

/* hub CreditsAnimationRecord begin */
#pragma pack(push, 1)
struct CreditsAnimationRecord {
    int positionX; /* +0x0 */
    int positionY; /* +0x4 */
    int current8; /* +0x8 */
    int currentC; /* +0xc */
    int velocity0; /* +0x10 */
    int velocity4; /* +0x14 */
    int velocity8; /* +0x18 */
    int velocityC; /* +0x1c */
    int pairIndexA; /* +0x20 */
    int pairIndexB; /* +0x24 */
    unsigned char pad_28[0x10];
    int target0; /* +0x38 */
    int target4; /* +0x3c */
    int target8; /* +0x40 */
    int targetC; /* +0x44 */
    int threshold; /* +0x48 */
    int maxDistance; /* +0x4c */
    int mode; /* +0x50 */
    int flag; /* +0x54 */
    int flags; /* +0x58 */
    unsigned char pad_5c[0x8];
    union { int uCoordBits; float uCoord; }; /* +0x64: texture u of the quad's left edge */
    union { int vBaseBits; float vBase; }; /* +0x68: texture v of the quad's top edge */
    union { int texWidthBits; float texWidth; }; /* +0x6c: texture width of the quad */
    union { int texHeightBits; float texHeight; }; /* +0x70: texture height of the quad */
    int shade; /* +0x74 */
};
#pragma pack(pop)
/* hub CreditsAnimationRecord end */

/* hub CreditsFadeOverlayBatch begin */
/* The credits fade overlay's render objects. */
#pragma pack(push, 1)
struct CreditsFadeOverlayBatch {
    union {
        int renderObject; /* +0x0 */
        struct GfxRenderObject *fadeOverlay;
    };
    union {
        struct {
            union {
                int backdropSlotA; /* +0x4 */
                struct GfxRenderObject *backdropA;
            };
            union {
                int packetSlotB; /* +0x8 */
                struct GfxRenderObject *backdropB;
            };
            union {
                int partA; /* +0xc */
                struct GfxRenderObject *backdropPartA;
            };
            union {
                int partB; /* +0x10 */
                struct GfxRenderObject *backdropPartB;
            };
            union {
                int partC; /* +0x14 */
                struct GfxRenderObject *backdropPartC;
            };
            union {
                int partD; /* +0x18 */
                struct GfxRenderObject *backdropPartD;
            };
        };
        int backdropSlots[6];
    };
};
#pragma pack(pop)
/* hub CreditsFadeOverlayBatch end */

/* Credits animation keyframe: four start values and four end values, scaled by the layout scale. */
typedef struct CreditsAnimationKey {
    int start0;
    int start1;
    int start2;
    int start3;
    int end0;
    int end1;
    int end2;
    int end3;
} CreditsAnimationKey;

/* One credits script entry (0x40 bytes) holding a text position and frame duration. */
typedef struct CreditsScriptRecord {
    int textOffset;
    int renderOffset;
    int posX;
    char text[0x30];
    int duration;
} CreditsScriptRecord;

/* One credits fade strip: its fade level (0xFF at the start, the low byte is the strip's alpha), the signed
 * step added to it each frame, and a flag that is 0 once the strip has faded out. stripCount is used only in
 * record 0: the number of strips still fading. */
typedef struct CreditsParticleRecord {
    union {
        int fadeLevel;
        char fadeAlpha;
    };
    int fadeDelta;
    int stripCount;
    int active;
} CreditsParticleRecord;

/* Credits fade overlay alpha accumulator and its low byte. */
typedef union {
    int value;
    char lowByte;
} CreditsAlphaValue;

/* Packed ARGB colour word written to credits quad vertices, with byte access to channels. */
typedef union {
    int value;
    struct {
        char blue;
        char green;
        char red;
        char alpha;
    };
} CreditsColorWord;

/* Credits timing range with a start and an end index. */
typedef struct CreditsTimingRange {
    int start;
    int end;
} CreditsTimingRange;

/* Credits glyph draw state: the three batch nodes the glyph drawers write quads into. */
typedef struct ChocoboCreditsGlyphBuffer {
    struct GfxRenderObject *batchNode0;
    struct GfxRenderObject *batchNodeA;
    struct GfxRenderObject *batchNodeB;
} ChocoboCreditsGlyphBuffer;

#pragma pack(push, 1)
typedef struct CreditsSectionFlags {
    int fadeActive; /* +0x0 */
    int fadeCounter; /* +0x4 */
} CreditsSectionFlags;
#pragma pack(pop)

#endif /* FF7_TYPES_CREDITS_H */
