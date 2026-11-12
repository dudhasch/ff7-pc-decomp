#ifndef FF7_SNOBO_PRIMITIVES_H
#define FF7_SNOBO_PRIMITIVES_H

struct GfxEngineContext;
struct GfxRenderObject;

// The snowboard minigame's primitive classes: the colour, the texture loader, the primitive
// emitter (SnowboardTmdRenderer) and the clipper, and the C declarations their TUs share.
struct SnowboardColor {
    int value;
    // An inline member: tmd.cpp's call through SnowboardTextureLoader::shade is not expanded, so
    // the body is a COMDAT after tmd.obj's .text.
    /*
     * Stores the packed colour value.
     */
    // FUNCTION: FF7 0x00733340
    SnowboardColor(int color) { value = color; }
};
struct SnowboardTextureLoader {
    int modelCount;
    int modelTable;
    unsigned char pad_08[0x4];
    struct GfxRenderObject *batchList; /* +0x0C: the untextured render object (SnowboardTmdRenderer::batchList) */
    int createRenderObject(int, int, int, int);
    int shade(SnowboardColor, float, int);
    unsigned char loadTextures(int);
    void releaseTextures();
};
// The race sprite renderer: the course loader as tmd.cpp's draw() reads it, the view scales at +0x14.
struct SnowboardRaceSpriteRenderer {
    unsigned char pad_00[0x14];
    float widthScale;
    float heightScale;
    void draw(int sprite, int advance);
};
struct SnowboardUvPair {
    float x, y;
};
struct SnowboardUvBits {
    int x, y;
};
struct SnowboardTextureRecord76 {
    int id;
    int bppMode;
    int pageX;
    int pageY;
    int vramX;
    int vramY;
    int widthWords;
    int height;
    int localX;
    int localY;
    int used;
    char *filename;
    int objectType;
    int blendMode;
    struct GfxRenderObject *renderObject;
    union {
        int originU;
        unsigned short originULo;
    };
    union {
        int originV;
        unsigned short originVLo;
    };
    union {
        int scaleU;
        unsigned short scaleULo;
    };
    union {
        int scaleV;
        unsigned short scaleVLo;
    };
};
struct SnowboardTmdRenderer : SnowboardTextureLoader {
    void model(int matrixPtr, int modelIndex, int mode, int flags);
    void triangle(int, int, int);
    void shaded(int, int, int);
    void colored(int, int, int);
    int lookup(int);
    int uv(int, unsigned short, int);
    void textured(int, int, int);
    void emit();
};
struct GfxEngineContext;
struct GfxRenderObject;
struct SnowboardMatrixWords64 {
    int m[16];
};
// The clipper: two render objects (triangles and quads) with a "has batched primitives" flag each,
// the model/view/screen matrix chain and the engine context they come from.
struct SnowboardClipper {
    GfxRenderObject *triangleObject;
    unsigned char triangleDirty;
    unsigned char pad_05[0x3];
    GfxRenderObject *quadObject;
    unsigned char quadDirty;
    unsigned char pad_0D[0x3];
    SnowboardMatrixWords64 modelMatrix;
    int cameraMatrix;
    SnowboardMatrixWords64 viewMatrix;
    int screenTransform;
    SnowboardMatrixWords64 viewScreenMatrix;
    SnowboardMatrixWords64 inverseScreenMatrix;
    GfxEngineContext *ctx;
    int d3dDevice2;
    SnowboardClipper();
    ~SnowboardClipper();
    void setMatrix(int matrixPtr);
    void transform(int pointPtr, int outVertexPtr);
    void transformProject(int pointPtr, int projectedPtr);
    int clip(int verticesPtr, int colorsPtr, int outVerticesPtr, int outColorsPtr, int reciprocalWPtr);
    int clipPositions(int inVertsPtr, int outVertsPtr);
    void project(int inputPtr, int outputPtr);
    void emitQuad(int vertex0Ptr, int vertex2Ptr, int vertex1Ptr, int vertex3Ptr, int color);
    void emitTriangle(int verticesPtr, int colorsPtr);
    void emitVertices(int vertex0Ptr, int vertex1Ptr, int vertex2Ptr, int color0, int color1, int color2);
    void flush();
};
extern "C" {
extern int Gfx_GetPsxScratchpadPtr(int offset);
extern int AcquireBatchNode(int count, void *batchContext);
struct GfxEngineContext;
extern struct GfxEngineContext *GetEngineContext(void);
struct SnowboardRaceState;
extern struct SnowboardRaceState *g_SnoboStatePtr;

extern SnowboardTextureRecord76 g_SnoboTextureRecords[16];
extern SnowboardUvPair g_SnoboUvPairs[4];
extern int g_SnoboTextureVariantColors[16];
void Gfx_SetScenePassCall(int pass, struct GfxEngineContext *context);
#include "lib/api/graphics.h"
void InvalidateBatchGeneration(void *renderObject);
}
#endif
