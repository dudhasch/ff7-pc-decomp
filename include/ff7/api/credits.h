/* include/ff7/api/credits.h: the functions and objects ff7/credits defines that other TUs use.
 * One declaration per name. */
#ifndef FF7_API_CREDITS_H
#define FF7_API_CREDITS_H

#ifdef __cplusplus
extern "C" {
struct GfxRenderObject;
#endif
struct CreditsFadeOverlayBatch;
struct CreditsAnimationRecord;
struct CreditsLineRecord;
struct ChocoboCreditsGlyphBuffer;
struct CreditsScriptRecord;

/* credfile.cpp */
void __cdecl Credits_ReleaseRenderObjectsAndCreateComesoon(void);
void __cdecl Credits_CreateRenderSetsAndObject(void);
void __cdecl Credits_LoadPeopleBinAndCreateRenderObjects(void);
void __cdecl Credits_ReleasePeopleBinAndRenderObjects(void);
extern unsigned char g_CreditsOpbackTimNameA[13];
extern unsigned char g_CreditsOpbackTimNameB[13];
extern unsigned char g_CreditsOpbackTimNameC[13];
extern unsigned char g_CreditsOpbackTimNameD[13];
extern unsigned char g_CreditsOpbackTimNameE[13];
extern unsigned char g_CreditsOpbackTimNameF[13];
extern unsigned char g_CreditsLogoTimNameA[14];
extern unsigned char g_CreditsLogoTimNameB[14];
extern unsigned char g_CreditsLogoTimNameC[14];
extern unsigned char g_CreditsLogoTimNameD[14];
extern unsigned char g_CreditsNameTimName[12];
extern char g_CreditsMoviePath[256];
extern int g_CreditsRenderObjectHandle;
extern char g_CreditsRenderFlag0;
extern char g_CreditsRenderFlag1;
extern char g_CreditsRenderFlag2;
void __cdecl Credits_ReleaseRenderObjectSets(void);
void __cdecl Credits_ReleaseRenderObjectSetsAndBackground(void);
void __cdecl Credits_CreateFourRenderObjects(void);
void __cdecl Credits_ReleaseFourRenderObjects(void);
void __cdecl Credits_CreateRenderObjectSets(void);
void __cdecl Credits_ClearState(void);
void __cdecl Credits_InitMovieTimingVariant0(void);
void __cdecl Credits_InitMovieTimingVariantNonZero(void);
void __cdecl Credits_OpenMovie(int index);
void __cdecl Credits_CreateFiveRenderObjects(void);
int __cdecl Credits_ReadLgpFileByName(char *path, int size, void *buffer);
void __cdecl Credits_ResetRecordDefaults(struct CreditsLineRecord *record);
void __cdecl Credits_CenterRecordTexts(void);

/* credits_fade_overlay_screen.cpp */
extern int g_CreditsFadeOverlayLeft;
extern int g_CreditsFadeOverlayTop;
extern int g_CreditsFadeOverlayRight;
extern int g_CreditsFadeOverlayBottom;
extern int g_CreditsParticleStartX;
extern int g_CreditsParticleStartY;
extern int g_CreditsParticleStepX;
extern int g_CreditsParticleHeight;
extern int g_CreditsFadeOverlayStep;
extern int g_CreditsFadeOverlayActive;
void __cdecl Credits_SetFadeOverlayRect(int left, int top, int width, int height);
void __cdecl Credits_StartFadeOverlayFromOpaque(int step);
void __cdecl Credits_StartFadeOverlayFromClear(int value);
int __cdecl Credits_IsFadeOverlayActive(void);
void __cdecl Credits_UpdateAndDrawFadeOverlay(void);
void __cdecl Credits_DrawBackground2x(void);
void __cdecl Credits_DrawBackground1x(void);
void __cdecl Credits_DrawLogoStrip2x(void);
void __cdecl Credits_DrawLogoStrip1x(void);
void __cdecl Credits_DrawAfterTextQuads2x(void);
void __cdecl Credits_DrawAfterTextQuads1x(void);
void __cdecl Credits_InitParticleRecords(void);
int __cdecl Credits_UpdateFadeStrips(void);

/* credits_glyph_string_draw.cpp */
int __cdecl Credits_MeasureGlyphStringWidth(char *text, int shift);
void __cdecl Credits_DrawGlyphString32x40(int x, int y, char *s, int color);
void __cdecl Credits_DrawGlyphString32x40TextBufferNodeA(int x, int y, char *s, int color);
void __cdecl Credits_DrawGlyphString32x40TextBufferNode0(int x, int y, char *s, int color);
void __cdecl Credits_DrawGlyphString16x20(int x, int y, char *s, int color);
void __cdecl Credits_DrawGlyphString16x20VHalf(int x, int y, char *s, int color);
void __cdecl Credits_DrawGlyphString16x20UHalf(int x, int y, char *s, int color);
void __cdecl Credits_DrawGlyphString8x10UVHalf(int x, int y, char *s, int color);
void __cdecl Credits_DrawGlyphString8x10UHalfV3Quarter(int x, int y, char *s, int color);
void __cdecl Credits_DrawGlyphString8x10U3QuarterVHalf(int x, int y, char *s, int color);

/* credits_animation_frame_loop.cpp */
extern int g_CreditsQuadBatchListA;
extern int g_CreditsQuadBatchListB;
extern struct GfxRenderObject *g_CreditsLogoQuadBatches;
#define g_CreditsQuadBatchListsA (&g_CreditsQuadBatchListA)
#define g_CreditsQuadBatchListsB (&g_CreditsQuadBatchListB)
#define g_CreditsLogoQuadBatchList (&g_CreditsLogoQuadBatches)
extern struct GfxRenderObject *g_CreditsUnusedFlagE8;
extern struct GfxRenderObject *g_CreditsLogoDrawState;
extern struct GfxRenderObject *g_CreditsUnusedFlagF0;
extern struct GfxRenderObject *g_CreditsQuadBatchListD;
#define g_CreditsQuadBatchListsD (&g_CreditsQuadBatchListD)
extern struct ChocoboCreditsGlyphBuffer g_CreditsTextBuffer;
extern struct GfxRenderObject *g_CreditsTextState;
extern int g_CreditsVariant;
extern int g_CreditsStartSection;
extern int g_CreditsSequenceState;
extern int g_CreditsTextPhase;
extern int g_CreditsViewportX;
extern int g_CreditsViewportY;
extern int g_CreditsViewportWidth;
extern struct CreditsScriptRecord *g_CreditsScriptDataBase;
extern int g_CreditsUnusedFlag34;
extern int g_CreditsUnusedFlag38;
extern int g_CreditsSmallViewport;
extern int g_CreditsMovieRunning;
extern int g_CreditsEndOverlayFlag;
extern int g_CreditsTextScrollMode;
extern int g_CreditsMoviePlaybackDone;
extern int g_CreditsTextDisplayTimer;
extern int g_CreditsWindowTimer;
extern int g_CreditsFrameDurationDivisorLo;
extern int g_CreditsFrameDurationDivisorHi;
void __cdecl Credits_StoreScaledTickDuration(double scale);
void __cdecl Credits_ClearFadeCounterIfFlagged(void);
void __cdecl Credits_AdvanceAnimationRecord(int index);
void __cdecl Credits_StepFadeColorPair(int *colorA, int *colorB);
void __cdecl Credits_EmitRecordQuadPairs(void);
void __cdecl Credits_InitAnimationRecords(void);
void __cdecl Credits_BusyWaitTicks(double multiplier);
void __cdecl Credits_ReopenMovieAndRecreateRenderSets(void);
void __cdecl Credits_DrawBackgroundForLayout(void);
void __cdecl Credits_TickSequenceState(void);
void __cdecl Credits_DrawRecordWindow(void);
void __cdecl Credits_TickTextPhase(void);
void __cdecl Credits_TickSelectedMode(void);
void __cdecl Credits_ModuleEnter(struct GfxEngineContext *ctx);
void __cdecl Credits_ModuleExit(void);
void __cdecl Credits_ModuleMouse(void);
void __cdecl Credits_ModuleKeyboard(void);
void __cdecl Credits_ModuleFrame(struct GfxEngineContext *renderContext);
void __cdecl Credits_EmitRenderPasses(struct GfxEngineContext *renderContext);
void __cdecl Credits_WaitForFrameDuration(struct GfxEngineContext *context);
void __cdecl Credits_SetVariant(int variant);
void __cdecl Credits_SetStartSection(int startSection);
void __cdecl Credits_SetFrameDurationDivisor(int divisorLo, int divisorHi);

extern struct CreditsAnimationRecord g_CreditsAnimationRecords[60];
extern struct CreditsFadeOverlayBatch g_CreditsFadeOverlayBatch;
#ifdef __cplusplus
}
#endif

#endif /* FF7_API_CREDITS_H */
