/* include/ff7/api/snowboard.h: the functions and objects ff7/snowboard defines that other TUs use.
 * One declaration per name. */
#ifndef FF7_API_SNOWBOARD_H
#define FF7_API_SNOWBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

/* snobo_course_data1.cpp */
extern unsigned char g_SnoboCourse1Header[72];
extern unsigned char g_SnoboCourse1BlockIndexTable[8];
extern char g_SnoboCourse1ActionTable[8];
extern unsigned char g_SnoboCourse1EffectPosTable[7216];
extern unsigned char g_SnoboCourse1AngleTable[1208];
extern unsigned char g_SnoboCourse1ChainBitTable[608];
extern char g_SnoboCourse1ChainIndexTable[8];
extern unsigned char g_SnoboCourse1SegmentTable[8];
extern char *g_SnoboCourse1SectionNames[76];

/* snobo_course_data2.cpp */
extern int g_SnoboCourse2Header;
extern unsigned char g_SnoboCourse2BlockIndexTable[640];
extern char g_SnoboCourse2ActionTable[8];
extern unsigned char g_SnoboCourse2EffectPosTable[7632];
extern unsigned char g_SnoboCourse2AngleTable[1272];
extern unsigned char g_SnoboCourse2ChainBitTable[640];
extern char g_SnoboCourse2ChainIndexTable[8];
extern unsigned char g_SnoboCourse2SegmentTable[8];
extern char *g_SnoboCourse2SectionNames[58];

#ifdef __cplusplus
}
#endif

#endif /* FF7_API_SNOWBOARD_H */
