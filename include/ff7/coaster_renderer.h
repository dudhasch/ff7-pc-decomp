#pragma once

struct GfxRenderObject;
struct GfxBatchPrimitiveCursors;

class CoasterRenderer
{
  public:
    CoasterRenderer();
    ~CoasterRenderer();
    void *load(int kind, int blend, char *name, int flags);
    void drawReticle();
    void drawShot();
    void drawScorePanel();
    void DrawFadeOverlay(unsigned char level);
    void submitBatches();
    void drawNumber(int value, short x, short y, short leadingZeroes);
    void drawCharge();
    void drawPanel5F1838();
    void drawPanel5F1A71();
    void drawPanel5F1CB5();
    void drawRail(void *left, void *right);
    void drawScenery(void *model);
    void drawSceneryParts(void *model, void *first, void *second, void *third,
                          void *fourth, void *firstData, void *secondData);
    void setMatrix(void *matrix);
    void projectEight(void *points, int *out);
    void projectThree(void *points, int *out);
    void drawActor(void *model);
    void drawPopup(void *model);
    void projectPoint(short *point, void *matrix, float *out);
    int transformVisible(short *point, void *matrix, float *out);
    float transformDepth(short *point, float *matrix);
    float depthFactor(float depth);
    int shadeColor(void *color, float factor);

  private:
    /* Two 4x4 float matrices: scratchMatrix points at the first, projectionMatrix at the second.
       The members below are the render objects load() makes (partData* and the rail/part pairs
       are batch/call objects and cursor blocks). */
    float matrices[2][16];
    float *scratchMatrix;
    float *projectionMatrix;
    struct GfxRenderObject *partBatchesBlend4;
    struct GfxRenderObject *partCallsBlend4;
    struct GfxBatchPrimitiveCursors *partDataFirst;
    struct GfxRenderObject *partBatchesBlend1;
    struct GfxRenderObject *partCallsBlend1;
    struct GfxBatchPrimitiveCursors *partDataSecond;
    struct GfxRenderObject *railBatches;
    struct GfxRenderObject *railCalls;
    struct GfxBatchPrimitiveCursors *partDataThird;
    struct GfxRenderObject *reticleTim;
    struct GfxRenderObject *currentBeamTim;
    struct GfxRenderObject *beamTim[3];
    struct GfxRenderObject *scoreTim;
    struct GfxRenderObject *co1Tim;
    struct GfxRenderObject *pauseTim;
    struct GfxRenderObject *chargeObject;
    struct GfxRenderObject *waku2Tim;
    struct GfxRenderObject *fadeObject;
};
