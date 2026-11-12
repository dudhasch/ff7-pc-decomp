/* include/lib/api/token.h: the functions and objects lib/token defines that other TUs use.
 * One declaration per name. */
#ifndef LIB_API_TOKEN_H
#define LIB_API_TOKEN_H

#ifdef __cplusplus
extern "C" {
#endif
struct KernelCommand;
struct BattleAttackEntry;
struct PolygonTokenFile;

/* token.cpp */
extern unsigned char g_GlVertexScratchBuffer[26688];
extern int g_TokenState;
extern short g_FieldRequestedBattleId;
#define g_BattleResultWord (*(unsigned short *)((char *)&g_FieldRequestedBattleId + 2))
extern unsigned char *g_pTextSpacing;
int __cdecl CountCharInString(char value, char *text);
int __cdecl FindCharIndexInString(char value, char *text);
int __cdecl ParseHexString(int prefix, char *text);
float __cdecl StringToFloat(const char *text);
int __cdecl StringToInt(int value);
void __cdecl FreeTokenFile(struct PolygonTokenFile *file);
struct PolygonTokenFile *__cdecl TokenizeTextFile(void (__cdecl *callback)(int, char *), void *context, char *filename);
struct PolygonTokenFile *__cdecl TokenizeTextFileFromDisk(int callback, void *fileName);
int __cdecl TokenAsInt(int index, struct PolygonTokenFile *file);
float __cdecl TokenAsFloat(int index, struct PolygonTokenFile *file);
char *__cdecl TokenAsString(int index, struct PolygonTokenFile *file);
void __cdecl SplitLineAtCommas(int length, char *buffer);
void __cdecl SplitLineAtCommasAndEquals(int length, char *buffer);
int __cdecl LoadBraceHexByteListFromFile(int callback, int *countOut, int fileName);

extern struct BattleAttackEntry g_BattleAttackTable[128];
extern struct KernelCommand g_KernelCommandTable[32];
#ifdef __cplusplus
}
#endif

#endif /* LIB_API_TOKEN_H */
