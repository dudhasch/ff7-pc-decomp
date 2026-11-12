/* include/lib/api/trans.h: the functions and objects lib/trans defines that other TUs use.
 * One declaration per name. */
#ifndef LIB_API_TRANS_H
#define LIB_API_TRANS_H

#ifdef __cplusplus
extern "C" {
#endif

/* trans.cpp */
void *__cdecl Trans_BuildScaledLut(double factor, int bits, struct GfxPixelFormat *format);
void *__cdecl Trans_BuildDoubleBrightLutSaturate(int bits, struct GfxPixelFormat *format);
void *__cdecl Trans_BuildDoubleBrightLutWrapToZero(int bits, struct GfxPixelFormat *format);

#ifdef __cplusplus
}
#endif

#endif /* LIB_API_TRANS_H */
