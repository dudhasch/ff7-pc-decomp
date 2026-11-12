#ifndef FF7_GTE_H
#define FF7_GTE_H

#include "lib/types/graphics.h"

/* Both defined in src/lib/src/graphics/psx.cpp, where the matched bodies
 * carry the scaffold's prototypes; these are the shapes their callers
 * push (VECTOR: lib/types/graphics.h). NormalizeVectorFixed scales `in`
 * to unit length in 12-bit fixed point and writes `out`; CrossProductFixed12
 * writes a x b to `out`. */
#ifdef __cplusplus
extern "C" {
#endif
extern int __cdecl NormalizeVectorFixed(void *input, void *output);
extern void __cdecl CrossProductFixed12(struct VECTOR *vecA, struct VECTOR *vecB, struct VECTOR *result);
#ifdef __cplusplus
}
#endif

#endif /* FF7_GTE_H */
