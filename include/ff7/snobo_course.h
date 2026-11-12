#ifndef FF7_SNOBO_COURSE_H
#define FF7_SNOBO_COURSE_H

// The snowboard minigame's course classes: the TMD and texture loader, the renderer (course,
// HUD, menus, fades), the course assets (animation frames) and the bone matrix state.

struct SnowboardMatrix64 {
    float v[16];
};

struct SnowboardVec3 {
    float x, y, z;
};

struct TmdHeader;
struct GfxRenderObject;

struct SnowboardCourseLoader {
    ~SnowboardCourseLoader();
    int modelCount;
    struct TmdHeader *tmdBuffer;
    int unused08;
    struct GfxRenderObject *batchList;
    int unused10;
    float viewScaleX; // the engine context's view width over 640
    float viewScaleY; // its view height over 480
    SnowboardCourseLoader()
    {
        modelCount = 0;
        tmdBuffer = 0;
        unused08 = 0;
        batchList = 0;
        unused10 = 0;
    }
    unsigned char load(int tmdData, int textureData);
    unsigned char loadTmd(int arg1);
    void freeTmd();
};

struct GfxRenderObject;

typedef struct SnowboardRenderer SnowboardRenderer;
struct SnowboardRenderer {
    ~SnowboardRenderer();
    SnowboardRenderer();
    void buildCourse();
    void prim(struct SnowboardScreenQuadSrc *quad, int page);
    void strip(struct SnowboardStripVertexSrc *band);
    void panel(unsigned char *, int, int, int, int, int);
    void drawTime(int, short, short, int);
    void family0();
    void family1();
    void family2();
    void drawRect(int, int, int, int);
    void drawText(char *, int, int, int, int);
    void drawNumber(unsigned short, short, short, int);
    void select0(short);
    void select1(short);
    void select2(short);
    void select3(short);
    void select4(short);
    int fadeModel;                        // full-screen fade panel; also the HUD's last pass-1 object
    int fontModel;                        // cfont.tim: the HUD text
    int menuFontModel;                    // second cfont.tim: menu and result text
    GfxRenderObject *wakuModel;           // waku.tim: the menu frame rectangles
    int rectPanelModel;                   // inset panel under the menu rectangles; the pause overlay in mode 0
    GfxRenderObject *stripModel;          // the course's background strips
    GfxRenderObject *backgroundModels[8]; // bg0A.tim through bg0H.tim (event course only)
    GfxRenderObject *time1Model;          // time1.tim
    GfxRenderObject *time2Model;          // time2.tim
    unsigned char initialize();
    void emitHud();
    void emitCourse();
    void emitOverlay();
    int buildModel(int type, int blendMode, int name, int flags);
    void packDigits(int output, int digits, char base);
    void splitTime(int source, int output);
    void showMenu(int data, int frame);
    void renderRaceState(int state, int phase, int frame);
    void selectMenu(short selection);
    void renderStateMenu(int selection);
    void fadeWhite(unsigned char alpha);
    void fadeBlack(unsigned char alpha);
    void fillColor(int rgb);
};

struct SnowboardTmdRenderer;

// One bone of a course: three 4-by-4 matrices, the parent bone indices and the bone length.
struct SnowboardMatrixState {
    unsigned char flag;
    unsigned char pad_01[3];
    float length;
    short parentA, parentB;
    SnowboardMatrix64 a, b, c;
    void loadMatrices(int boneDefs);
    SnowboardMatrixState();
};

// The source and destination bone of an attached model, and its two model arguments.
struct SnowboardAttachment {
    short a, b, bone, parent;
};

struct SnowboardCourseAssets {
    ~SnowboardCourseAssets();
    SnowboardTmdRenderer *tmdRenderer;
    int courseIndex;
    int boneCount;
    SnowboardMatrix64 *spareMatrices;     // allocated and released, never read by the course assets
    SnowboardMatrix64 *worldMatrices;     // per-bone matrices built by transform
    SnowboardVec3 *boneEnds;              // per-bone end offsets built by transform
    SnowboardVec3 *angles;                // current per-bone angles
    SnowboardVec3 *anglesFrom;            // angles at the start of a blend
    SnowboardVec3 *anglesTo;              // angles at the end of a blend
    SnowboardVec3 position;
    SnowboardVec3 positionFrom;
    SnowboardVec3 positionTo;
    SnowboardMatrixState *bones;
    int attachmentCount;
    SnowboardVec3 *attachmentPositions;
    SnowboardAttachment *attachments;
    SnowboardVec3 **frames;               // 209 poses of boneCount angle triples each
    float timeLeft;
    float duration;
    float(__cdecl *ease)(float);
    SnowboardCourseAssets(SnowboardCourseLoader *loader) { tmdRenderer = (SnowboardTmdRenderer *)loader; }
    void clear();
    void step(float delta);
    void initialize(int courseIndex);
    void initializeFrames();
    void copyVectors(int arg1, int arg2);
    void reset(int frame);
    void selectFrame(int frameIndex, int duration, int easing);
    void blendFrames(int fromFrame, int toFrame, float t);
    void transform(SnowboardMatrix64 *input);
};

#endif
