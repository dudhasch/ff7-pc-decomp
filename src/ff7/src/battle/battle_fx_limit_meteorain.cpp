// src/ff7/src/battle/battle_fx_limit_meteorain.cpp
// Limit Break [4] (Cloud Lv 3-1): Meteorain (Japanese: メテオレイン, dev tag: "limit2/meteo_r" / MeteoR)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Meteo R limit break effect: PSX-style textured and colored mesh drawing, a projectile,
//   a rising sprite quad, spinning and fading models, and sprite particle pools. Texture gradient
//   helpers and a target timeline complete it.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/codegen.h"
#include "lib/enums/sound.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(memset)

// clang-format off
// GLOBAL: FF7 0x0082bd88
int g_BattleLimitMeteorainMeshHeader = 384;
unsigned char g_rawData_0082bd90[880] = {
0x0c, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xfb, 0xfe, 0xea, 0xfc, 0x00, 0x00, 0xcc, 0x00, 0xfb, 0xfe,
0x05, 0xfd, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x1e, 0xfe, 0x00, 0x00, 0x8b, 0x01, 0xfb, 0xfe,
0x54, 0xfd, 0x00, 0x00, 0xfa, 0x00, 0x00, 0x00, 0x4f, 0xfe, 0x00, 0x00, 0x2e, 0x02, 0xfb, 0xfe,
0xd2, 0xfd, 0x00, 0x00, 0x61, 0x01, 0x00, 0x00, 0x9f, 0xfe, 0x00, 0x00, 0xac, 0x02, 0xfb, 0xfe,
0x75, 0xfe, 0x00, 0x00, 0xb1, 0x01, 0x00, 0x00, 0x06, 0xff, 0x00, 0x00, 0xfb, 0x02, 0xfb, 0xfe,
0x34, 0xff, 0x00, 0x00, 0xe2, 0x01, 0x00, 0x00, 0x7f, 0xff, 0x00, 0x00, 0x16, 0x03, 0xfb, 0xfe,
0x00, 0x00, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb, 0x02, 0xfb, 0xfe,
0xcc, 0x00, 0x00, 0x00, 0xe2, 0x01, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0xac, 0x02, 0xfb, 0xfe,
0x8b, 0x01, 0x00, 0x00, 0xb1, 0x01, 0x00, 0x00, 0xfa, 0x00, 0x00, 0x00, 0x2e, 0x02, 0xfb, 0xfe,
0x2e, 0x02, 0x00, 0x00, 0x61, 0x01, 0x00, 0x00, 0x61, 0x01, 0x00, 0x00, 0x8b, 0x01, 0xfb, 0xfe,
0xac, 0x02, 0x00, 0x00, 0xfa, 0x00, 0x00, 0x00, 0xb1, 0x01, 0x00, 0x00, 0xcc, 0x00, 0xfb, 0xfe,
0xfb, 0x02, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0xe2, 0x01, 0x00, 0x00, 0x00, 0x00, 0xfb, 0xfe,
0x16, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0x34, 0xff, 0xfb, 0xfe,
0xfb, 0x02, 0x00, 0x00, 0x7f, 0xff, 0x00, 0x00, 0xe2, 0x01, 0x00, 0x00, 0x75, 0xfe, 0xfb, 0xfe,
0xac, 0x02, 0x00, 0x00, 0x06, 0xff, 0x00, 0x00, 0xb1, 0x01, 0x00, 0x00, 0xd2, 0xfd, 0xfb, 0xfe,
0x2e, 0x02, 0x00, 0x00, 0x9f, 0xfe, 0x00, 0x00, 0x61, 0x01, 0x00, 0x00, 0x54, 0xfd, 0xfb, 0xfe,
0x8b, 0x01, 0x00, 0x00, 0x4f, 0xfe, 0x00, 0x00, 0xf9, 0x00, 0x00, 0x00, 0x05, 0xfd, 0xfb, 0xfe,
0xcc, 0x00, 0x00, 0x00, 0x1e, 0xfe, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0xea, 0xfc, 0xfb, 0xfe,
0x00, 0x00, 0x00, 0x00, 0x0c, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0xfd, 0xfb, 0xfe,
0x34, 0xff, 0x00, 0x00, 0x1e, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0x00, 0x00, 0x54, 0xfd, 0xfb, 0xfe,
0x75, 0xfe, 0x00, 0x00, 0x4f, 0xfe, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0xd2, 0xfd, 0xfb, 0xfe,
0xd2, 0xfd, 0x00, 0x00, 0x9f, 0xfe, 0x00, 0x00, 0x9f, 0xfe, 0x00, 0x00, 0x75, 0xfe, 0xfb, 0xfe,
0x54, 0xfd, 0x00, 0x00, 0x06, 0xff, 0x00, 0x00, 0x4f, 0xfe, 0x00, 0x00, 0x34, 0xff, 0xfb, 0xfe,
0x05, 0xfd, 0x00, 0x00, 0x7f, 0xff, 0x00, 0x00, 0x1e, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x2f, 0x00,
0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x18, 0x00, 0x10, 0x00, 0x7f, 0xfb, 0x88, 0x79,
0x7f, 0x80, 0x93, 0xfb, 0x9f, 0x80, 0x80, 0x00, 0x18, 0x00, 0x10, 0x00, 0x28, 0x00, 0x20, 0x00,
0x93, 0xfb, 0x88, 0x79, 0x9f, 0x80, 0xa6, 0xfb, 0xbd, 0x80, 0x80, 0x00, 0x28, 0x00, 0x20, 0x00,
0x38, 0x00, 0x30, 0x00, 0xa6, 0xfb, 0x88, 0x79, 0xbd, 0x80, 0xb7, 0xfb, 0xd9, 0x80, 0x80, 0x00,
0x38, 0x00, 0x30, 0x00, 0x48, 0x00, 0x40, 0x00, 0xb7, 0xfb, 0x88, 0x79, 0xd9, 0x80, 0xc4, 0xfb,
0xeb, 0x80, 0x80, 0x00, 0x48, 0x00, 0x40, 0x00, 0x58, 0x00, 0x50, 0x00, 0xc4, 0xfb, 0x88, 0x79,
0xeb, 0x80, 0xcb, 0xfb, 0xf9, 0x80, 0x80, 0x00, 0x58, 0x00, 0x50, 0x00, 0x68, 0x00, 0x60, 0x00,
0xcb, 0xfb, 0x88, 0x79, 0xf9, 0x80, 0xce, 0xfb, 0xfe, 0x80, 0x80, 0x00, 0x68, 0x00, 0x60, 0x00,
0x78, 0x00, 0x70, 0x00, 0xce, 0xfb, 0x88, 0x79, 0xfe, 0x80, 0xcb, 0xfb, 0xf9, 0x80, 0x80, 0x00,
0x78, 0x00, 0x70, 0x00, 0x88, 0x00, 0x80, 0x00, 0xcb, 0xfb, 0x88, 0x79, 0xf9, 0x80, 0xc4, 0xfb,
0xeb, 0x80, 0x80, 0x00, 0x88, 0x00, 0x80, 0x00, 0x98, 0x00, 0x90, 0x00, 0xc4, 0xfb, 0x88, 0x79,
0xeb, 0x80, 0xb7, 0xfb, 0xd9, 0x80, 0x80, 0x00, 0x98, 0x00, 0x90, 0x00, 0xa8, 0x00, 0xa0, 0x00,
0xb7, 0xfb, 0x88, 0x79, 0xd9, 0x80, 0xa6, 0xfb, 0xbd, 0x80, 0x80, 0x00, 0xa8, 0x00, 0xa0, 0x00,
0xb8, 0x00, 0xb0, 0x00, 0xa6, 0xfb, 0x88, 0x79, 0xbd, 0x80, 0x93, 0xfb, 0x9f, 0x80, 0x80, 0x00,
0xb8, 0x00, 0xb0, 0x00, 0xc8, 0x00, 0xc0, 0x00, 0x93, 0xfb, 0x88, 0x79, 0x9f, 0x80, 0x7d, 0xfb,
0x7d, 0x80, 0x80, 0x00, 0xc8, 0x00, 0xc0, 0x00, 0xd8, 0x00, 0xd0, 0x00, 0x7d, 0xfb, 0x88, 0x79,
0x7d, 0x80, 0x69, 0xfb, 0x5d, 0x80, 0x80, 0x00, 0xd8, 0x00, 0xd0, 0x00, 0xe8, 0x00, 0xe0, 0x00,
0x69, 0xfb, 0x88, 0x79, 0x5d, 0x80, 0x56, 0xfb, 0x3e, 0x80, 0x80, 0x00, 0xe8, 0x00, 0xe0, 0x00,
0xf8, 0x00, 0xf0, 0x00, 0x56, 0xfb, 0x88, 0x79, 0x3e, 0x80, 0x45, 0xfb, 0x23, 0x80, 0x80, 0x00,
0xf8, 0x00, 0xf0, 0x00, 0x08, 0x01, 0x00, 0x01, 0x45, 0xfb, 0x88, 0x79, 0x23, 0x80, 0x37, 0xfb,
0x10, 0x80, 0x80, 0x00, 0x08, 0x01, 0x00, 0x01, 0x18, 0x01, 0x10, 0x01, 0x37, 0xfb, 0x88, 0x79,
0x10, 0x80, 0x31, 0xfb, 0x03, 0x80, 0x80, 0x00, 0x18, 0x01, 0x10, 0x01, 0x28, 0x01, 0x20, 0x01,
0x31, 0xfb, 0x88, 0x79, 0x03, 0x80, 0x2d, 0xfb, 0x00, 0x80, 0x80, 0x00, 0x28, 0x01, 0x20, 0x01,
0x38, 0x01, 0x30, 0x01, 0x2d, 0xfb, 0x88, 0x79, 0x00, 0x80, 0x31, 0xfb, 0x03, 0x80, 0x80, 0x00,
0x38, 0x01, 0x30, 0x01, 0x48, 0x01, 0x40, 0x01, 0x31, 0xfb, 0x88, 0x79, 0x03, 0x80, 0x37, 0xfb,
0x10, 0x80, 0x80, 0x00, 0x48, 0x01, 0x40, 0x01, 0x58, 0x01, 0x50, 0x01, 0x37, 0xfb, 0x88, 0x79,
0x10, 0x80, 0x45, 0xfb, 0x23, 0x80, 0x80, 0x00, 0x58, 0x01, 0x50, 0x01, 0x68, 0x01, 0x60, 0x01,
0x45, 0xfb, 0x88, 0x79, 0x23, 0x80, 0x56, 0xfb, 0x3e, 0x80, 0x80, 0x00, 0x68, 0x01, 0x60, 0x01,
0x78, 0x01, 0x70, 0x01, 0x56, 0xfb, 0x88, 0x79, 0x3e, 0x80, 0x69, 0xfb, 0x5d, 0x80, 0x80, 0x00,
0x78, 0x01, 0x70, 0x01, 0x00, 0x00, 0x08, 0x00, 0x69, 0xfb, 0x88, 0x79, 0x5d, 0x80, 0x7f, 0xfb,
0x7f, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082c100
unsigned int g_BattleLimitMeteorainResourceDef[55] = {0x00192a39, (unsigned int)"BOMB_A.TIM", (unsigned int)"BOMB_A.SPT", 0x00000000, 0x00000000, 0x006400b1, (unsigned int)"EXP_1.rsd", 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)"EXP_2.rsd", 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)"EXP_3.rsd", 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)"FLASH_1.rsd", 0x00000000, 0x00000000, 0x00000000, 0x006400b4, (unsigned int)"METEO_1.rsd", 0x00000000, 0x00000000, 0x00000000, 0x006440b1, (unsigned int)"METEO_2.rsd", 0x00000000, 0x00000000, 0x00000000, 0x00197c3c, (unsigned int)"air_1.TIM", 0x00000000, 0x00000000, 0x00000000, 0x00197c39, (unsigned int)"air_1.TIM", 0x00000000, 0x00000000, 0x00000000, 0x0019783c, (unsigned int)"air_1.TIM", 0x00000000, 0x00000000, 0x00000000, 0x00197839, (unsigned int)"air_1.TIM", 0x00000000, 0x00000000, 0x00000000};
/* The model draw requests of the projectile: METEO_1 (table entry 5, drawn spinning) and the head of
 * METEO_2's request (entry 6), whose state flags and texture page continue in the next four bytes. */
typedef struct {
    BattleFxModelDrawHead meteo1Draw;
    int meteo2ModelGroup;
    int meteo2Flags;
    short meteo2UvScroll; /* toggled by 0x80 each unpaused frame */
    short meteo2FadeLevel;
} BattleLimitMeteoRProjectileDraws;
// GLOBAL: FF7 0x0082c1e0
BattleLimitMeteoRProjectileDraws g_BattleLimitMeteorainSpriteReq = {{0, 0x80, 0, 0, 0, 0}, 0, 0xa8, 0, 0};
unsigned char g_rawData_0082c1fc[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082c200
int g_BattleLimitMeteorainScaleX = 2560;
// GLOBAL: FF7 0x0082c204
int g_BattleLimitMeteorainScaleY = 2560;
// GLOBAL: FF7 0x0082c208
int g_BattleLimitMeteorainScaleZ = 2560;
// GLOBAL: FF7 0x0082c210
short g_BattleLimitMeteorainScaleMatrix[4] = {4096, 0, 0, 0};
// GLOBAL: FF7 0x0082c218
short g_BattleLimitMeteorainScaleMatrixM11 = 4096;
/* The rest of the packed scale matrix that starts at g_BattleLimitMeteorainScaleMatrix: its m[2][2] and
 * its translation, which the projectile task sets to its ground X/Z. */
#pragma pack(push, 1)
typedef struct {
    short m22;
    int transX;
    int transY;
    int transZ;
    unsigned char pad_e[0x2];
} BattleLimitMeteoRScaleMatrixTail;
#pragma pack(pop)
// GLOBAL: FF7 0x0082c220
BattleLimitMeteoRScaleMatrixTail g_BattleLimitMeteorainModelReq20 = {0x1000, 0, 0, 0, {0, 0}};
/* The first two words of the scene model's draw request; its uvScroll, fade level and state flags follow
 * in the next bytes. */
typedef struct {
    int modelGroup;
    int flags;
} BattleLimitMeteoRSceneModelDraw;
// GLOBAL: FF7 0x0082c230
BattleLimitMeteoRSceneModelDraw g_BattleLimitMeteorainModelReq30 = {0, 0x88};
unsigned char g_rawData_0082c238[24] = {0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x68, 0x68, 0x2e, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082c250
BattleFxSpriteRequest g_BattleLimitMeteorainModelReq50 = {0, 0, 0x60, 0x60, 0x60, 0x2e, 0, 0};
// GLOBAL: FF7 0x0082c260
BattleFxSpriteQuadRequest g_BattleLimitMeteorainParam60 = {-16, -16, 0, 0xa0, 0x1f, 0x1f, 0x80, 0x80, 0x80, {0x2e}, 0x3a, 0x7a07, 0};
// GLOBAL: FF7 0x00b01448
BattleMeteorainParticle g_BattleLimitMeteorainParticlePools[6][64] = {0};
// GLOBAL: FF7 0x00b03848
struct Int64Pair g_BattleLimitMeteorainSourcePos = {0};
// GLOBAL: FF7 0x00b03850
int g_BattleLimitMeteorainTargetMask = 0;
// GLOBAL: FF7 0x00b03854
int g_BattleLimitMeteorainSourceActor = 0;
// The sort records of the angular texture gradient, one per quad of the mesh.
// GLOBAL: FF7 0x00b03858
struct BattleFxMeteorainQuadSortEntry g_BattleLimitMeteorainQuadSortEntries[24] = {0};
// GLOBAL: FF7 0x00b038b8
struct GfxTableCursor *g_BattleLimitMeteorainModelTable = 0;
unsigned char g_bssPad_00b038bc[4] = {0};
// GLOBAL: FF7 0x00b038c0
struct BattleLimitMeteorainStateC0 g_BattleLimitMeteorainSpinRotXY = {0};
// GLOBAL: FF7 0x00b038c4
short g_BattleLimitMeteorainSpinRotZ = 0;
// clang-format on
#pragma intrinsic(memcpy)
#pragma function(memcpy)

/* Loads the limit2/meteo_r model table once, when the 3D scene exists and the table is not loaded yet. */
// FUNCTION: FF7 0x004c15c0
void __cdecl BattleFx_LimitLoadResMeteorain(void)
{
    GfxModelTableBank *drawDev;
    GfxResourceRequest recordBuf;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int tempResDef;

    if (g_pBattle3dScene != 0 && g_BattleLimitMeteorainModelTable == 0) {
        kind = 0xB;
        tempResDef = (int)&g_BattleLimitMeteorainResourceDef;
        drawDev = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recordBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0082c304
        pRequest->resourcePath = BattleFx_BuildResourcePath("limit2/meteo_r", pathText);
        Gfx_LoadModelTableIfAbsent(kind, tempResDef, pRequest, &g_BattleLimitMeteorainModelTable, drawDev);
    }
}

/* Entry point: loads the Meteo R resources and starts the effect for the target mask and source actor. Returns zero. */
// FUNCTION: FF7 0x004c1650
int __cdecl BattleFx_LimitEnterMeteorain(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResMeteorain();
    BattleFx_LimitBeginMeteorain(targetMask, actorIdx);
    return 0;
}

/* Starts the draw-list buffer task and the spawn task, and records the target mask and source actor. */
// FUNCTION: FF7 0x004c166c
void __cdecl BattleFx_LimitBeginMeteorain(int targetMask, int actorIdx)
{
    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_LimitMeteorainSpawnTimeline);
    g_BattleLimitMeteorainTargetMask = targetMask;
    g_BattleLimitMeteorainSourceActor = actorIdx;
}

/* Reads the source actor's bone position and radius, starts the timeline task and builds the angular texture gradient of the mesh. */
// FUNCTION: FF7 0x004c16ad
void __cdecl BattleFx_LimitMeteorainSpawnTimeline(void)
{
    BattleFxMeteorainGradientState *gradient;
    BattleFxYuffieWeaponTimeline2 *task;

    task = (BattleFxYuffieWeaponTimeline2 *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_BattleLimitMeteorainSourceActor, g_BattleActorRecords[g_BattleLimitMeteorainSourceActor].targetBone, ((short *)&g_BattleLimitMeteorainSourcePos));
    ((short *)&g_BattleLimitMeteorainSourcePos)[3] = g_BattleActorRecords[g_BattleLimitMeteorainSourceActor].radius;
    task = (BattleFxYuffieWeaponTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMeteorainTimelineTask);
    task->subCounter = 0;
    gradient = (BattleFxMeteorainGradientState *)Gfx_GetPsxScratchpadPtr(0);
    gradient->meshHeader = &g_BattleLimitMeteorainMeshHeader;
    BattleFx_LimitMeteorainAssignMeshTextureGradient(gradient);
}

/* Reads the high-word mask of the mesh header into the gradient state and assigns the angular texture gradient. */
// FUNCTION: FF7 0x004c1759
int __cdecl BattleFx_LimitMeteorainAssignMeshTextureGradient(BattleFxMeteorainGradientState *gradState)
{
    int cursor;
    int base;

    base = (int)gradState->meshHeader + 4;
    cursor = base + *gradState->meshHeader;
    gradState->highWordMask = *(int *)cursor & 0xFFFF0000;
    cursor += 4;
    cursor = BattleFx_LimitMeteorainAssignAngularTextureGradient(gradState, base, cursor);
    return gradState->unusedReturn;
}

/*
 * Gives each quad of the mesh a texture gradient by the angle of its centre around the vertical axis.
 * The quads are sorted by that angle and get U values from 255 down to 0 in sorted order, with the
 * division remainder spread over the last ones. Returns the end of the quad records. A quad count of
 * zero divides by zero.
 */
// FUNCTION: FF7 0x004c17b1
int __cdecl BattleFx_LimitMeteorainAssignAngularTextureGradient(BattleFxMeteorainGradientState *gradientState, int vertexBase, int descriptor)
{
    int totalY;
    int weightX;
    BattleFxMeteorainGradientQuad *quad;
    int quadPosition;
    Vec3sPacked *vertPtr;
    int totalZ;
    int prevGradient;
    int count;
    int remainder;
    int recordIndex;
    int recordCount;

    gradientState->refDir.vx = 0;
    gradientState->refDir.vy = 0;
    gradientState->refDir.vz = 0xFFFFF000;
    recordCount = *(int *)descriptor;
    prevGradient = 0;
    recordIndex = 0;
    quad = (BattleFxMeteorainGradientQuad *)(descriptor + 4);
    for (quadPosition = 0; quadPosition < recordCount; quadPosition++, quad++) {
        vertPtr = (Vec3sPacked *)(vertexBase + quad->vertex0Offset);
        weightX = vertPtr->vx;
        totalY = vertPtr->vy;
        totalZ = vertPtr->vz;
        vertPtr = (Vec3sPacked *)(vertexBase + quad->vertex1Offset);
        weightX += vertPtr->vx;
        totalY += vertPtr->vy;
        totalZ += vertPtr->vz;
        vertPtr = (Vec3sPacked *)(vertexBase + quad->vertex2Offset);
        weightX += vertPtr->vx;
        totalY += vertPtr->vy;
        totalZ += vertPtr->vz;
        vertPtr = (Vec3sPacked *)(vertexBase + quad->vertex3Offset);
        weightX += vertPtr->vx;
        totalY += vertPtr->vy;
        totalZ += vertPtr->vz;
        quad->u3 = 0;
        quad->u2 = 0;
        quad->u1 = 0;
        quad->u0 = 0;
        weightX /= 4;
        totalY /= 4;
        totalZ /= 4;
        gradientState->centroid.vx = weightX;
        gradientState->centroid.vy = 0;
        gradientState->centroid.vz = totalZ;
        NormalizeVectorFixed(&gradientState->centroid.vx, &gradientState->centroid.vx);
        count = ComputeAngleBetweenVectors(&gradientState->refDir, &gradientState->centroid, &gradientState->cross);
        if (gradientState->cross.vy > 0) {
            count = 4096 - count;
        }
        g_BattleLimitMeteorainQuadSortEntries[recordIndex].angle = count;
        g_BattleLimitMeteorainQuadSortEntries[recordIndex].quadPosition = quadPosition;
        g_BattleLimitMeteorainQuadSortEntries[recordIndex].cleared = 0;
        recordIndex++;
    }
    count = recordCount;
    for (quadPosition = 0; quadPosition < count - 1; quadPosition++) {
        for (prevGradient = quadPosition + 1; prevGradient < count; prevGradient++) {
            if (g_BattleLimitMeteorainQuadSortEntries[quadPosition].angle > g_BattleLimitMeteorainQuadSortEntries[prevGradient].angle) {
                recordIndex = *(int *)&g_BattleLimitMeteorainQuadSortEntries[quadPosition];
                *(int *)&g_BattleLimitMeteorainQuadSortEntries[quadPosition] = *(int *)&g_BattleLimitMeteorainQuadSortEntries[prevGradient];
                *(int *)&g_BattleLimitMeteorainQuadSortEntries[prevGradient] = recordIndex;
            }
        }
    }
    prevGradient = 0xFF;
    remainder = 0x100 % count;
    quad = (BattleFxMeteorainGradientQuad *)(descriptor + 4);
    for (quadPosition = 0; quadPosition < count; quadPosition++) {
        totalY = g_BattleLimitMeteorainQuadSortEntries[quadPosition].quadPosition;
        if (quadPosition < count - remainder) {
            recordIndex = prevGradient - 256 / count;
        } else {
            recordIndex = prevGradient - 256 / count - 1;
            if (recordIndex < 0) {
                recordIndex = 0;
            }
        }
        quad[totalY].u1 = prevGradient;
        quad[totalY].u0 = prevGradient;
        quad[totalY].u3 = recordIndex;
        quad[totalY].u2 = recordIndex;
        quad[totalY].v2 = 0xFF;
        quad[totalY].v0 = 0xFF;
        quad[totalY].v3 = 0x80;
        quad[totalY].v1 = 0x80;
        prevGradient = recordIndex;
    }
    return (int)quad;
}

/*
 * The Meteo R timeline, unpaused only. Step 0 loads two sounds; step 1 starts the growing mesh under
 * the source; step 25 starts eight rising sprites around the source, each further along Y;
 * step 28 the shrinking black model at the source; step 40 a target timeline at a random point near
 * each hit target. The screen tint ramps up over 16 steps, holds, and fades from step 67. From step
 * 50, every third step queues a ring record of kind 0xA for the next hit target. At step 75 the tint
 * is cleared, the task retires and its three sounds are released.
 */
// FUNCTION: FF7 0x004c1ba2
void __cdecl BattleFx_LimitMeteorainTimelineTask(void)
{
    short level;
    BattleFxPoint3s *ringRec;
    int rad;
    int targetActor;
    BattleFxMeteorainTimeline *task;
    BattleFxLimitMeteorainTimeline *pNewTask;
    int limit;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxMeteorainTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (task->step == 0) {
        SfxLoadIntoBuffer(0x188, 0);
        SfxLoadIntoBuffer(0xFA, 0);
    }
    if (task->step == 1) {
        rad = ((short *)&g_BattleLimitMeteorainSourcePos)[3] + 0x120;
        pNewTask = (BattleFxLimitMeteorainTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMeteorainGrowingPsxMeshTask);
        *(Int64Pair *)&pNewTask->posX = *(Int64Pair *)&g_BattleLimitMeteorainSourcePos;
        pNewTask->posY = 0;
        pNewTask->rotYOrSeqStart = 0;
        pNewTask->rotStepOrActor = 0xFEE0;
        pNewTask->size = (rad << 12) / 750;
        pNewTask->sizeStep = pNewTask->size / 9;
        pNewTask->ramp = 0;
        pNewTask->sizeStepX2 = pNewTask->size / 9 << 1;
        SfxLoadIntoBuffer(0x30, 0);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->posX), 0x21B);
    }
    if (task->step == 25) {
        Battle_GetBoneWorldPos(g_BattleLimitMeteorainSourceActor, g_BattleActorRecords[g_BattleLimitMeteorainSourceActor].targetBone, ((short *)&g_BattleLimitMeteorainSourcePos));
        for (rad = 0; rad < 8; rad++) {
            pNewTask = (BattleFxLimitMeteorainTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMeteorainRisingSpriteQuadTask);
            *(Int64Pair *)&pNewTask->posX = *(Int64Pair *)&g_BattleLimitMeteorainSourcePos;
            pNewTask->posX += (short)(rand() % 400 - 200);
            pNewTask->posZ += (short)(rand() % 400 - 200);
            ((short *)&g_BattleLimitMeteorainSourcePos)[1] += (short)(rand() % 60 + rad * 4 + 150);
            pNewTask->scale = 0x2000 - (rad << 9);
            pNewTask->velYOrScaleStep = rand() % 0x1E + rad + 0x50;
        }
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&task->posX), 0x30);
    }
    if (task->step == 28) {
        rad = ((short *)&g_BattleLimitMeteorainSourcePos)[3] + 0x240;
        targetActor = rad << 3;
        pNewTask = (BattleFxLimitMeteorainTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMeteorainShrinkingBlackModelTask);
        Battle_GetBoneWorldPos(g_BattleLimitMeteorainSourceActor, g_BattleActorRecords[g_BattleLimitMeteorainSourceActor].targetBone, &pNewTask->posX);
        pNewTask->scale = (targetActor << 12) / 1000;
        pNewTask->velYOrScaleStep = (((targetActor - rad) << 12) / 1000) / 8;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->posX), 0x188);
    }
    if (task->step == 40) {
        Battle_GetBoneWorldPos(g_BattleLimitMeteorainSourceActor, g_BattleActorRecords[g_BattleLimitMeteorainSourceActor].targetBone, ((short *)&g_BattleLimitMeteorainSourcePos));
        for (rad = 0; rad < 8; rad++) {
            targetActor = g_BattleHitTargetActors[rad];
            if (targetActor == 0xFF) {
                break;
            }
            pNewTask = (BattleFxLimitMeteorainTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMeteorainTargetTimelineTask);
            Battle_GetBoneWorldPos(targetActor, g_BattleActorRecords[targetActor].targetBone, &pNewTask->posX);
            limit = g_BattleActorRecords[targetActor].radius + 288;
            if (limit > 1900) {
                limit = 1900;
            }
            pNewTask->posX += (short)(rand() % (limit << 1) - limit);
            pNewTask->posY = 0;
            pNewTask->posZ += (short)(rand() % (limit << 1) - limit);
            pNewTask->countLimit = rand() % 9 + (rad >> 1) + 1;
            pNewTask->rotYOrSeqStart = rad;
            pNewTask->rotStepOrActor = targetActor;
            pNewTask->scale = rand() % 0x300 + 0x500;
            pNewTask->count = 0;
        }
    }
    if (task->step < 16) {
        g_BattleFxTimelineState.red = 0x54;
        g_BattleFxTimelineState.channel1 = 0x1C;
        g_BattleFxTimelineState.channel2 = 4;
        level = task->step * 144;
    } else if (task->step >= 67) {
        level = 0x900 - (task->step - 0x43) * 288;
    } else {
        level = 0x900;
    }
    if (task->step >= 50 && task->step % 3 == 0) {
        rad = g_BattleHitTargetActors[task->sequenceIndex];
        if (rad != 0xFF) {
            ringRec = (BattleFxPoint3s *)Battle_AllocSceneMessage(0xA);
            ringRec->targetIndex = rad;
            ringRec->sequenceIndex = task->sequenceIndex;
            (task->sequenceIndex)++;
        }
    }
    if (task->step >= 75) {
        level = 0;
        task->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x30, 0xF);
        Sfx_ReleaseCachedBuffer(0x188, 0xF);
        Sfx_ReleaseCachedBuffer(0xFA, 0xF);
    }
    g_BattleFxTimelineState.startValue = level;
    (task->step)++;
}

/*
 * One frame of the shrinking black model at the task position: a billboard of the scale held in the
 * task, the model of table entry 4 drawn black, with a fade level that ramps down until frame 6, stays
 * zero until frame 17 and ramps up after. The task retires at frame 27; unpaused, it turns the
 * rotation by its step for the first eight frames and advances the frame.
 */
// FUNCTION: FF7 0x004c218f
void __cdecl BattleFx_LimitMeteorainShrinkingBlackModelTask(void)
{
    int p;
    int q;
    BattleFxModelDraw *requestPtr;
    int s;
    int t;
    int u;
    int v;
    int w;
    int x;
    int y;
    int z;
    int a;
    int b;
    int c;
    BattleFxMeteorainShrinkRecord *taskSlot;
    int e;

    taskSlot = (BattleFxMeteorainShrinkRecord *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&taskSlot->posX, (short)taskSlot->rotation, 0xFFFFFDC0);
    requestPtr = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    requestPtr->modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleLimitMeteorainModelTable);
    requestPtr->flags = 0x88;
    requestPtr->uvScroll = 0;
    requestPtr->fadeLevel = 0;
    requestPtr->stateFlags = 0;
    requestPtr->texturePageBits = 0;
    if (taskSlot->frame < 6) {
        requestPtr->fadeLevel = 4096 - taskSlot->frame * 682;
    } else if (taskSlot->frame >= 17) {
        requestPtr->fadeLevel = (taskSlot->frame - 17) * 409;
    } else {
        requestPtr->fadeLevel = 0;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(requestPtr, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (taskSlot->frame >= 27) {
        taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskSlot->frame < 8) {
        taskSlot->rotation -= taskSlot->rotationStep;
    }
    (taskSlot->frame)++;
}

/*
 * Draws the growing mesh: a matrix from the stored Y rotation and separate X/Z and Y scales, composed
 * with the camera, with four sprite states set from the model table, through
 * BattleFx_LimitMeteorainDrawPsxMesh. The shade weight falls from frame 12 and the task retires at frame
 * 20, also while paused. Unpaused, it turns and damps the rotation, grows the X/Z scale, grows the Y
 * scale before frame 9 and advances the frame.
 */
// FUNCTION: FF7 0x004c22f5
void __cdecl BattleFx_LimitMeteorainGrowingPsxMeshTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxMeteoGrowRecord *growRecord; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxLimitMeteorainMeshRecord *meshRecord; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.growRecord = (BattleFxMeteoGrowRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.growRecord->rotY;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.growRecord->posX;
    frame.matrix.t[1] = frame.growRecord->posY;
    frame.matrix.t[2] = frame.growRecord->posZ;
    frame.scaleX.vz = frame.growRecord->scaleXZ;
    frame.scaleX.vx = frame.scaleX.vz;
    frame.scaleX.vy = frame.growRecord->scaleY;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    RefreshGlobalTransformState();
    Gfx_SetPsxPrimQuadState(9, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(7, g_BattleLimitMeteorainModelTable)));
    Gfx_SetPsxPrimQuadState(9, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(8, g_BattleLimitMeteorainModelTable)));
    Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(9, g_BattleLimitMeteorainModelTable)));
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(10, g_BattleLimitMeteorainModelTable)));
    frame.meshRecord = (BattleFxLimitMeteorainMeshRecord *)Gfx_GetPsxScratchpadPtr(0);
    frame.meshRecord->meshHeader = &g_BattleLimitMeteorainMeshHeader;
    frame.meshRecord->flags = 0x28;
    frame.meshRecord->clearedWordA = 0;
    frame.meshRecord->shadeWeight = 0x80;
    frame.meshRecord->drawMode = 0x20;
    frame.meshRecord->clearedWordB = 0;
    if (frame.growRecord->timer >= 12) {
        frame.meshRecord->shadeWeight -= (short)(frame.growRecord->timer - 0xC << 4);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_LimitMeteorainDrawPsxMesh(frame.meshRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.growRecord->timer >= 20) {
        frame.growRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.growRecord->rotY += frame.growRecord->rotYStep;
    frame.growRecord->rotYStep -= (short)(frame.growRecord->rotYStep >> 6);
    frame.growRecord->scaleXZ += frame.growRecord->scaleXZStep;
    if (frame.growRecord->timer < 9) {
        frame.growRecord->scaleY += frame.growRecord->scaleYStep;
    }
    (frame.growRecord->timer)++;
}

/*
 * Draws the four counted record streams of the mesh (textured triangles, textured quads, colored
 * triangles and colored quads) as PSX primitives. Primitives whose transform flags include 0x60000
 * are skipped; the first stream needs a front-facing result above zero and the others at least zero,
 * unless the draw is forced. Colors are copied or shaded, the optional semi-transparency bit is set,
 * and quads get their fourth vertex. Returns the render-state pointer after the last primitive. In
 * the first stream, when the color flag is clear the color word is read before it is set.
 */
// FUNCTION: FF7 0x004c2553
int __cdecl BattleFx_LimitMeteorainDrawPsxMesh(BattleFxLimitMeteorainMeshRecord *mesh, int orderingTable, int depthShift, int packetCursor)
{
    int startVertex;
    unsigned short renderFlags;
    int d;
    int cnt;
    int bits;
    int support;
    BattleFxMeteorTailPrim *tailTexPrim;
    BattleFxMeteorainQuadPrim *packetPointer;
    int elemCount;
    BattleFxMeteorainQuadPrim *texQuadPacket;
    int x;
    int a;
    int vert1;
    int vert2;
    int meshOrigin;
    BattleFxMeteorTailPrim *currTriPacket;
    int color;
    int primIdx;
    BattleFxMeteoRMeshCursor *curPtr;

    meshOrigin = (int)mesh->meshHeader + 4;
    curPtr = (BattleFxMeteoRMeshCursor *)(meshOrigin + *mesh->meshHeader);
    packetPointer = (BattleFxMeteorainQuadPrim *)Gfx_GetPsxPacketScratchPtr();
    cnt = curPtr->header;
    curPtr = (BattleFxMeteoRMeshCursor *)((char *)curPtr + 2);
    renderFlags = curPtr->header & 0x19F | mesh->drawMode;
    curPtr = (BattleFxMeteoRMeshCursor *)((char *)curPtr + 2);
    startVertex = meshOrigin + curPtr->texTri[0].vertex0;
    vert1 = meshOrigin + curPtr->texTri[0].vertex1;
    vert2 = meshOrigin + curPtr->texTri[0].vertex2;
    for (primIdx = 0; primIdx < cnt; primIdx++) {
        StoreDeferredDrawArgs((const void *)startVertex, (const void *)vert1, (const void *)vert2);
        ApplyDeferredDrawArgs();
        startVertex = meshOrigin + curPtr->texTri[1].vertex0;
        vert1 = meshOrigin + curPtr->texTri[1].vertex1;
        vert2 = meshOrigin + curPtr->texTri[1].vertex2;
        Gfx_PsxStoreClearFlag(&bits);
        if ((bits & 0x60000) == 0) {
            if ((mesh->flags & 0x20) != 0) {
                support = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&support);
            }
            if (support > 0) {
                packetPointer->tag = 0x7000000;
                Gfx_ReadTransformOrderingDepth2(&elemCount);
                packetPointer->uv0Clut = curPtr->texTri[0].uv0Clut;
                packetPointer->uv1 = curPtr->texTri[0].uv1;
                packetPointer->uv2 = curPtr->texTri[0].uv2;
                packetPointer->tpage = renderFlags;
                if ((mesh->flags & 0x80) != 0) {
                    color = curPtr->texTri[0].color & 0xFF;
                    color = color << 0x10 | 0x24000000 | color << 8 | color;
                    if (mesh->shadeWeight != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)&color, mesh->shadeWeight, (CVECTOR *)&packetPointer->colorCmd);
                        packetPointer->code = 0x24;
                    } else {
                        packetPointer->colorCmd = color;
                    }
                } else {
                    packetPointer->colorCmd = color << 0x10 | 0x24000000 | color << 8 | color;
                }
                if ((mesh->flags & 8) != 0) {
                    packetPointer->code |= 2;
                }
                Gfx_DrawPsxPrimitive(packetPointer);
            }
        }
        curPtr = (BattleFxMeteoRMeshCursor *)&curPtr->texTri[1];
    }
    cnt = curPtr->header;
    curPtr = (BattleFxMeteoRMeshCursor *)((char *)curPtr + 4);
    texQuadPacket = packetPointer;
    startVertex = meshOrigin + curPtr->texQuad[0].vertex0;
    vert1 = meshOrigin + curPtr->texQuad[0].vertex1;
    vert2 = meshOrigin + curPtr->texQuad[0].vertex2;
    for (primIdx = 0; primIdx < cnt; primIdx++) {
        StoreDeferredDrawArgs((const void *)startVertex, (const void *)vert1, (const void *)vert2);
        ApplyDeferredDrawArgs();
        startVertex = meshOrigin + curPtr->texQuad[1].vertex0;
        vert1 = meshOrigin + curPtr->texQuad[1].vertex1;
        vert2 = meshOrigin + curPtr->texQuad[1].vertex2;
        Gfx_PsxStoreClearFlag(&bits);
        if ((bits & 0x60000) == 0) {
            if ((mesh->flags & 0x20) != 0) {
                support = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&support);
            }
            if (support >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(meshOrigin + curPtr->texQuad[0].vertex3));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&elemCount);
                texQuadPacket->tag = 0x9000000;
                texQuadPacket->uv0Clut = curPtr->texQuad[0].uv0Clut;
                texQuadPacket->uv1 = curPtr->texQuad[0].uv1;
                texQuadPacket->uv2 = curPtr->texQuad[0].uv2;
                texQuadPacket->uv3 = curPtr->texQuad[0].uv3;
                texQuadPacket->tpage = renderFlags;
                if ((mesh->flags & 0x80) != 0) {
                    color = curPtr->texQuad[0].color & 0xFF;
                    color = color << 0x10 | 0x2C000000 | color << 8 | color;
                    if (mesh->shadeWeight != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)&color, mesh->shadeWeight, (CVECTOR *)&texQuadPacket->colorCmd);
                        texQuadPacket->code = 0x2C;
                    } else {
                        texQuadPacket->colorCmd = color;
                    }
                } else {
                    color = mesh->shadeWeight & 0xFF;
                    texQuadPacket->colorCmd = color << 0x10 | 0x2C000000 | color << 8 | color;
                }
                if ((mesh->flags & 8) != 0) {
                    texQuadPacket->code |= 2;
                }
                Gfx_DrawPsxPrimitive(texQuadPacket);
            }
        }
        curPtr = (BattleFxMeteoRMeshCursor *)&curPtr->texQuad[1];
    }
    cnt = curPtr->header;
    curPtr = (BattleFxMeteoRMeshCursor *)((char *)curPtr + 4);
    currTriPacket = (BattleFxMeteorTailPrim *)texQuadPacket;
    startVertex = meshOrigin + curPtr->shadedTri[0].vertex0;
    vert1 = meshOrigin + curPtr->shadedTri[0].vertex1;
    vert2 = meshOrigin + curPtr->shadedTri[0].vertex2;
    for (primIdx = 0; primIdx < cnt; primIdx++) {
        StoreDeferredDrawArgs((const void *)startVertex, (const void *)vert1, (const void *)vert2);
        ApplyDeferredDrawArgs();
        startVertex = meshOrigin + curPtr->shadedTri[1].vertex0;
        vert1 = meshOrigin + curPtr->shadedTri[1].vertex1;
        vert2 = meshOrigin + curPtr->shadedTri[1].vertex2;
        Gfx_PsxStoreClearFlag(&bits);
        if ((bits & 0x60000) == 0) {
            if ((mesh->flags & 0x20) != 0) {
                support = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&support);
            }
            if (support >= 0) {
                currTriPacket->tag = 0x6000000;
                Gfx_ReadTransformOrderingDepth2(&elemCount);
                if (mesh->shadeWeight != 0) {
                    Gfx_ShadePsxTriVertexColors((CVECTOR *)&curPtr->shadedTri[0].color0, (CVECTOR *)&curPtr->shadedTri[0].color1, (CVECTOR *)&curPtr->shadedTri[0].color2, mesh->shadeWeight, (CVECTOR *)&currTriPacket->color0, (CVECTOR *)&currTriPacket->color1, (CVECTOR *)&currTriPacket->color2);
                } else {
                    currTriPacket->color0 = curPtr->shadedTri[0].color0;
                    currTriPacket->color1 = curPtr->shadedTri[0].color1;
                    currTriPacket->color2 = curPtr->shadedTri[0].color2;
                }
                if ((mesh->flags & 8) != 0) {
                    currTriPacket->code |= 2;
                }
                Gfx_DrawPsxPrimitive(currTriPacket);
            }
        }
        curPtr = (BattleFxMeteoRMeshCursor *)&curPtr->shadedTri[1];
    }
    cnt = curPtr->header;
    curPtr = (BattleFxMeteoRMeshCursor *)((char *)curPtr + 4);
    tailTexPrim = currTriPacket;
    startVertex = meshOrigin + curPtr->shadedQuad[0].vertex0;
    vert1 = meshOrigin + curPtr->shadedQuad[0].vertex1;
    vert2 = meshOrigin + curPtr->shadedQuad[0].vertex2;
    for (primIdx = 0; primIdx < cnt; primIdx++) {
        StoreDeferredDrawArgs((const void *)startVertex, (const void *)vert1, (const void *)vert2);
        ApplyDeferredDrawArgs();
        startVertex = meshOrigin + curPtr->shadedQuad[1].vertex0;
        vert1 = meshOrigin + curPtr->shadedQuad[1].vertex1;
        vert2 = meshOrigin + curPtr->shadedQuad[1].vertex2;
        Gfx_PsxStoreClearFlag(&bits);
        if ((bits & 0x60000) == 0) {
            if ((mesh->flags & 0x20) != 0) {
                support = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&support);
            }
            if (support >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(meshOrigin + curPtr->shadedQuad[0].vertex3));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&elemCount);
                tailTexPrim->tag = 0x8000000;
                if (mesh->shadeWeight != 0) {
                    Gfx_ShadePsxTriVertexColors((CVECTOR *)&curPtr->shadedQuad[0].color0, (CVECTOR *)&curPtr->shadedQuad[0].color1, (CVECTOR *)&curPtr->shadedQuad[0].color2, mesh->shadeWeight, (CVECTOR *)&tailTexPrim->color0, (CVECTOR *)&tailTexPrim->color1, (CVECTOR *)&tailTexPrim->color2);
                    Gfx_ShadePsxVertexColor((CVECTOR *)&curPtr->shadedQuad[0].color3, mesh->shadeWeight, (CVECTOR *)&tailTexPrim->color3);
                } else {
                    tailTexPrim->color0 = curPtr->shadedQuad[0].color0;
                    tailTexPrim->color1 = curPtr->shadedQuad[0].color1;
                    tailTexPrim->color2 = curPtr->shadedQuad[0].color2;
                    tailTexPrim->color3 = curPtr->shadedQuad[0].color3;
                }
                if ((mesh->flags & 8) != 0) {
                    tailTexPrim->code |= 2;
                }
                Gfx_DrawPsxPrimitive(tailTexPrim);
            }
        }
        curPtr = (BattleFxMeteoRMeshCursor *)&curPtr->shadedQuad[1];
    }
    return (int)tailTexPrim;
}

/*
 * One frame of a rising sprite quad: sets the texture step and CLUT of the quad request, draws it as a
 * billboard at the task position and retires at step 7. Unpaused, the vertical velocity is added to posY
 * and decays by a sixteenth each frame.
 */
// FUNCTION: FF7 0x004c2d5f
void __cdecl BattleFx_LimitMeteorainRisingSpriteQuadTask(void)
{
    BattleFxMeteorainRisingSprite *task;

    task = (BattleFxMeteorainRisingSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitMeteorainParam60.cellX = task->step << 5;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, -(task->scale >> 3));
    g_BattleLimitMeteorainParam60.quadState = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteQuadDraw(&g_BattleLimitMeteorainParam60, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->step >= 7) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posY += task->velocityY;
    task->velocityY -= (short)(task->velocityY >> 4);
    (task->step)++;
}

/*
 * One meteor's timeline at a target point, unpaused only. After a random delay, step 0 starts a
 * projectile from the source that reaches the point in six steps; step 5 starts two counter-spinning
 * fading models and a growing black model there and plays sound 0xFA; step 7 starts a sprite particle
 * pool there. The task retires at three times its sequence start plus 20.
 */
// FUNCTION: FF7 0x004c2e4b
void __cdecl BattleFx_LimitMeteorainTargetTimelineTask(void)
{
    int p;
    int size;
    BattleFxLimitMeteorainTargetSlot *shotPtr;
    int s;
    int t;
    BattleFxMeteorainTargetSlot *effect;
    BattleFxMeteorainTarget *task;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxMeteorainTarget *)BATTLE_CURRENT_TASK_SLOT;
    if (task->count < task->countLimit) {
        (task->count)++;
    } else {
        if (task->step == 0) {
            shotPtr = (BattleFxLimitMeteorainTargetSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMeteorainProjectileTask);
            *(Int64Pair *)&shotPtr->posX = *(Int64Pair *)&g_BattleLimitMeteorainSourcePos;
            shotPtr->stepX = (task->targetX - shotPtr->posX) / 6;
            shotPtr->stepY = (task->targetY - shotPtr->posY) / 6;
            shotPtr->stepZ = (task->targetZ - shotPtr->posZ) / 6;
            shotPtr->scale = task->scale;
        }
        if (task->step == 5) {
            size = task->scale * 700 / 2560;
            size += size >> 1;
            effect = (BattleFxMeteorainTargetSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMeteorainSpinningFadingModelEntry1Task);
            *(Int64Pair *)&effect->posX = *(Int64Pair *)&task->targetX;
            effect->phase = rand() % 4096;
            effect->velocity = rand() % 240 + 300;
            effect->size = 0;
            effect->targetSize = ((size << 12) / 600) / 2;
            effect = (BattleFxMeteorainTargetSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMeteorainSpinningFadingModelEntry2Task);
            *(Int64Pair *)&effect->posX = *(Int64Pair *)&task->targetX;
            effect->phase = rand() % 4096;
            effect->velocity = -(rand() % 240 + 300);
            effect->size = 0;
            effect->targetSize = ((size << 12) / 600) / 2;
            effect = (BattleFxMeteorainTargetSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMeteorainGrowingModelTask);
            *(Int64Pair *)&effect->posX = *(Int64Pair *)&task->targetX;
            effect->targetSize = (size << 12) / 700 / 3;
            effect->size = effect->targetSize;
            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->targetX), 0xFA);
        }
        if (task->step == 7) {
            effect = (BattleFxMeteorainTargetSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMeteorainSpriteParticlePool64Task);
            *(Int64Pair *)&effect->posX = *(Int64Pair *)&task->targetX;
            effect->size = task->scale * 1280 / 2560;
            effect->sequenceStart = task->sequenceStart;
        }
        if (task->step >= task->sequenceStart * 3 + 0x14) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
        (task->step)++;
    }
}

/*
 * Draws the falling meteor: METEO_2 in a look basis facing against the velocity, METEO_1 in that
 * basis turned by the shared spin, and the 3D scene's model at the projectile's ground X/Z through
 * the scale matrix (no model without a scene). Drawing also happens while paused. Unpaused, it starts
 * one spark dot at a random point on a circle around the projectile, moving inward, toggles METEO_2's
 * UV scroll, advances the spin, ages the task (retiring at 6) and moves it by its velocity.
 */
// FUNCTION: FF7 0x004c3198
void __cdecl BattleFx_LimitMeteorainProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[52];
        short modelScale;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix *rotMatrix; };
        struct { unsigned char pad_to_8[0x8]; PsxMatrix *modelMatrix; };
        struct { unsigned char pad_to_C[0xC]; BattleFxMeteorainProjectile *sparkDot; };
        struct { unsigned char pad_to_10[0x10]; PsxMatrix *lookMatrix; };
        struct { unsigned char pad_to_14[0x14]; BattleFxMeteorainOffsetRecord *offsetRecord; };
        struct { unsigned char pad_to_18[0x18]; int sectorIndex; };
        struct { unsigned char pad_to_1C[0x1C]; SVECTOR inwardX; };
        struct { unsigned char pad_to_24[0x24]; int depth; };
        struct { unsigned char pad_to_28[0x28]; BattleFxMeteorainProjectile *projectile; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxPosition3s *posRecord; };
        struct { unsigned char pad_to_30[0x30]; int angle; };
    } frame;
#pragma pack(pop)

    frame.projectile = (BattleFxMeteorainProjectile *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitMeteorainScaleZ = frame.projectile->scale;
    g_BattleLimitMeteorainScaleY = g_BattleLimitMeteorainScaleZ;
    g_BattleLimitMeteorainScaleX = g_BattleLimitMeteorainScaleY;
    frame.modelScale = (frame.projectile->scale << 12) / 2560;
    g_BattleLimitMeteorainModelReq20.m22 = frame.modelScale;
    g_BattleLimitMeteorainScaleMatrixM11 = g_BattleLimitMeteorainModelReq20.m22;
    g_BattleLimitMeteorainScaleMatrix[0] = g_BattleLimitMeteorainScaleMatrixM11;
    frame.lookMatrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelMatrix = frame.lookMatrix + 1;
    frame.rotMatrix = frame.modelMatrix + 1;
    frame.offsetRecord = (BattleFxMeteorainOffsetRecord *)(frame.rotMatrix + 1);
    frame.posRecord = (BattleFxPosition3s *)((int)frame.offsetRecord + 8);
    frame.inwardX.vx = -frame.projectile->velocityX;
    frame.inwardX.vy = -frame.projectile->velocityY;
    frame.inwardX.vz = -frame.projectile->velocityZ;
    Gte_BuildLookBasisFromDirection(&frame.inwardX.vx, (short *)frame.lookMatrix);
    frame.lookMatrix->t[0] = frame.projectile->posX;
    frame.lookMatrix->t[1] = frame.projectile->posY;
    frame.lookMatrix->t[2] = frame.projectile->posZ;
    ScaleMatrix(frame.lookMatrix, &g_BattleLimitMeteorainScaleX);
    MultiplyMatrix(&g_BattleCameraTransform, frame.lookMatrix, frame.modelMatrix);
    SetRotMatrix(frame.modelMatrix);
    SetTransMatrix(frame.modelMatrix);
    g_BattleLimitMeteorainSpriteReq.meteo2ModelGroup = Gfx_ModelTableGetRenderObject(6, g_BattleLimitMeteorainModelTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitMeteorainSpriteReq.meteo2ModelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    RotMatrix((SVECTOR *)&g_BattleLimitMeteorainSpinRotXY, (int *)frame.rotMatrix);
    MultiplyMatrix(frame.modelMatrix, frame.rotMatrix, frame.rotMatrix);
    SetRotMatrix(frame.rotMatrix);
    g_BattleLimitMeteorainSpriteReq.meteo1Draw.modelGroup = Gfx_ModelTableGetRenderObject(5, g_BattleLimitMeteorainModelTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitMeteorainSpriteReq.meteo1Draw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    g_BattleLimitMeteorainModelReq20.transX = frame.projectile->posX;
    g_BattleLimitMeteorainModelReq20.transZ = frame.projectile->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, g_BattleLimitMeteorainScaleMatrix, frame.modelMatrix);
    SetRotMatrix(frame.modelMatrix);
    SetTransMatrix(frame.modelMatrix);
    if (g_pBattle3dScene != 0) {
        g_BattleLimitMeteorainModelReq30.modelGroup = g_pBattle3dScene->modelRequest;
    } else {
        g_BattleLimitMeteorainModelReq30.modelGroup = 0;
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitMeteorainModelReq30, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_2, 1, g_BattleFxActiveDrawListPtr);
    if (g_BattleIsPaused == 0) {
        SetRotMatrix(frame.lookMatrix);
        SetTransMatrix(frame.lookMatrix);
        for (frame.sectorIndex = 0; frame.sectorIndex < 1; frame.sectorIndex++) {
            frame.angle = (frame.sectorIndex << 12) + (rand() % 4096);
            frame.offsetRecord->offsetX = SinFixed(frame.angle) * 0x3E8 >> 12;
            frame.offsetRecord->offsetY = CosFixed(frame.angle) * 0x3E8 >> 12;
            frame.offsetRecord->jitter = rand() % 1000;
            RotTransPoint((void *)frame.offsetRecord, (int *)frame.posRecord, &frame.depth);
            frame.sparkDot = (BattleFxMeteorainProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMeteorainLivewireDotTask);
            frame.sparkDot->posX = frame.posRecord->posX;
            frame.sparkDot->posY = frame.posRecord->posY;
            frame.sparkDot->posZ = frame.posRecord->posZ;
            frame.offsetRecord->jitter = 0;
            RotateSVectorByInstalledMatrix((void *)frame.offsetRecord, (void *)frame.posRecord);
            frame.sparkDot->velocityX = -((BattleFxVelocityVec3 *)frame.posRecord)->velocityX / 3;
            frame.sparkDot->velocityY = -((BattleFxVelocityVec3 *)frame.posRecord)->velocityY / 3;
            frame.sparkDot->velocityZ = -((BattleFxVelocityVec3 *)frame.posRecord)->velocityZ / 3;
        }
        g_BattleLimitMeteorainSpriteReq.meteo2UvScroll ^= 0x80;
        (g_BattleLimitMeteorainSpinRotXY.rotX) += 0x80;
        g_BattleLimitMeteorainSpinRotXY.rotY += 0x100 /* 22.5 deg */;
        g_BattleLimitMeteorainSpinRotZ += 0x180;
        (frame.projectile->age)++;
        if (frame.projectile->age >= 6) {
            frame.projectile->ownerSlot = BATTLE_TASK_FREE;
        }
        frame.projectile->posX += frame.projectile->velocityX;
        frame.projectile->posY += frame.projectile->velocityY;
        frame.projectile->posZ += frame.projectile->velocityZ;
    }
}

/*
 * Draws the spark as a one-pixel primitive, red with a random green, at its projected position
 * unless the projection clipped it. Unpaused, it moves by its velocity for the first three frames and
 * retires at frame 5.
 */
// FUNCTION: FF7 0x004c362b
void __cdecl BattleFx_LimitMeteorainLivewireDotTask(void)
{
    int screenPosition;
    int depth;
    int cullBits;
    BattleFxLivewireDotSlot *taskPtr;
    BattleFxMeteorainLivewireDot2 *prim;

    taskPtr = (BattleFxLivewireDotSlot *)BATTLE_CURRENT_TASK_SLOT;
    prim = (BattleFxMeteorainLivewireDot2 *)Gfx_GetPsxPacketScratchPtr();
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    depth = Gfx_TransformVertexSlot0ReturnDepth(&taskPtr->posX, &screenPosition, &cullBits);
    if ((cullBits & 0x60000) == 0) {
        prim->tag = prim->tag & 0xFFFFFF | 0x2000000;
        prim->command = 0x68;
        prim->colorR = 0xFF;
        prim->colorG = rand() & 0xFF;
        prim->colorB = 0;
        Gfx_DrawPsxPrimitive(prim);
    }
    if (g_BattleIsPaused == 0) {
        if (taskPtr->step < 3) {
            taskPtr->posX += taskPtr->velocityX;
            taskPtr->posY += taskPtr->velocityY;
            taskPtr->posZ += taskPtr->velocityZ;
        }
        (taskPtr->step)++;
        if (taskPtr->step >= 5) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws the live particles of the task's 64-particle pool with the BOMB_A sprite (table entry 0),
 * each with its stage as the frame; drawing also happens while paused. Unpaused, a particle dies after
 * stage 7, and before task frame 6 up to two free particles start at random points within the spread
 * radius around the origin, above it. The task retires at frame 16.
 */
// FUNCTION: FF7 0x004c3757
void __cdecl BattleFx_LimitMeteorainSpriteParticlePool64Task(void)
{
    int idx;
    int freeSlot;
    int r;
    BattleMeteorainParticle *pParticlePool;
    int t;
    int spread;
    int fullSpread;
    BattleFxMeteorainParticlePool *task;

    task = (BattleFxMeteorainParticlePool *)BATTLE_CURRENT_TASK_SLOT;
    pParticlePool = (BattleMeteorainParticle *)(task->resourceIndex * 0x600 + (char *)&g_BattleLimitMeteorainParticlePools);
    g_BattleLimitMeteorainModelReq50.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleLimitMeteorainModelTable);
    g_BattleLimitMeteorainModelReq50.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleLimitMeteorainModelTable);
    for (idx = 0; idx < 64; idx++) {
        if (pParticlePool[idx].active != 0) {
            g_BattleLimitMeteorainModelReq50.frame = pParticlePool[idx].stage;
            BattleFx_InstallBillboardMatrix(&pParticlePool[idx].posX, (short)pParticlePool[idx].scale, -(pParticlePool[idx].scale >> 3));
            g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleLimitMeteorainModelReq50, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
            if (g_BattleIsPaused == 0) {
                if (pParticlePool[idx].stage >= 7) {
                    pParticlePool[idx].active = 0;
                }
                (pParticlePool[idx].stage)++;
            }
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->timer >= 16) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (task->timer < 6) {
        spread = task->spreadRadius;
        fullSpread = spread << 1;
        for (idx = 0; idx < 2; idx++) {
            for (freeSlot = 0; freeSlot < 64 && pParticlePool[freeSlot].active != 0; freeSlot++) {
            }
            if (freeSlot >= 64) {
                break;
            }
            pParticlePool[freeSlot].active = 1;
            pParticlePool[freeSlot].stage = 0;
            pParticlePool[freeSlot].posX = task->originX + (rand() % fullSpread - spread);
            pParticlePool[freeSlot].posY = task->originY - rand() % spread;
            pParticlePool[freeSlot].posZ = task->originZ + (rand() % fullSpread - spread);
            pParticlePool[freeSlot].scale = ((rand()) % 2048) + 0x2000;
        }
    }
    (task->timer)++;
}

/*
 * Draws EXP_1 (table entry 1) at the task position, turned about Y by the task angle and scaled
 * uniformly, with draw flags 8 and a fade level falling from 0x80 after frame 8. Drawing and the
 * retirement at frame 16 also happen while paused. Unpaused, the angle advances by a velocity that
 * loses a fifth each frame, and the scale grows by a step that loses a quarter.
 */
// FUNCTION: FF7 0x004c3a07
void __cdecl BattleFx_LimitMeteorainSpinningFadingModelEntry1Task(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxMeteorainSpinFadeEntry *spinEntry; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.spinEntry = (BattleFxMeteorainSpinFadeEntry *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.spinEntry->angle;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.spinEntry->posX;
    frame.matrix.t[1] = frame.spinEntry->posY;
    frame.matrix.t[2] = frame.spinEntry->posZ;
    frame.scaleX.vz = frame.spinEntry->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleLimitMeteorainModelTable);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.spinEntry->frameCount >= 8) {
        frame.modelDraw->fadeLevel = 128 - (frame.spinEntry->frameCount - 8 << 4);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.spinEntry->frameCount >= 16) {
        frame.spinEntry->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spinEntry->angle += frame.spinEntry->angularVelocity;
    frame.spinEntry->angularVelocity -= (short)(frame.spinEntry->angularVelocity / 5);
    frame.spinEntry->scale += frame.spinEntry->scaleStep;
    frame.spinEntry->scaleStep -= (short)(frame.spinEntry->scaleStep >> 2);
    (frame.spinEntry->frameCount)++;
}

/*
 * Draws EXP_2 (table entry 2) like BattleFx_LimitMeteorainSpinningFadingModelEntry1Task: turned about Y
 * by the task angle, scaled uniformly, fading after frame 8, retiring at frame 16.
 */
// FUNCTION: FF7 0x004c3bef
void __cdecl BattleFx_LimitMeteorainSpinningFadingModelEntry2Task(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxMeteorainSpinFadeEntry *spinEntry; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.spinEntry = (BattleFxMeteorainSpinFadeEntry *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.spinEntry->angle;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.spinEntry->posX;
    frame.matrix.t[1] = frame.spinEntry->posY;
    frame.matrix.t[2] = frame.spinEntry->posZ;
    frame.scaleX.vz = frame.spinEntry->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleLimitMeteorainModelTable);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.spinEntry->frameCount >= 8) {
        frame.modelDraw->fadeLevel = 128 - (frame.spinEntry->frameCount - 8 << 4);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.spinEntry->frameCount >= 16) {
        frame.spinEntry->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spinEntry->angle += frame.spinEntry->angularVelocity;
    frame.spinEntry->angularVelocity -= (short)(frame.spinEntry->angularVelocity / 5);
    frame.spinEntry->scale += frame.spinEntry->scaleStep;
    frame.spinEntry->scaleStep -= (short)(frame.spinEntry->scaleStep >> 2);
    (frame.spinEntry->frameCount)++;
}

/*
 * Draws EXP_3 (table entry 3) black at the task position, scaled uniformly, with draw flags 0x88 and
 * a fade level that rises from frame 4. Drawing and the retirement at frame 12 also happen while
 * paused; unpaused, the scale grows by its step.
 */
// FUNCTION: FF7 0x004c3dd7
void __cdecl BattleFx_LimitMeteorainGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxMeteorainGrowRecord *growRecord; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.growRecord = (BattleFxMeteorainGrowRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.growRecord->posX;
    frame.matrix.t[1] = frame.growRecord->posY;
    frame.matrix.t[2] = frame.growRecord->posZ;
    frame.scaleX.vz = frame.growRecord->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleLimitMeteorainModelTable);
    frame.modelDraw->flags = 0x88;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.growRecord->frame >= 4) {
        frame.modelDraw->fadeLevel = frame.growRecord->frame - 4 << 9;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.growRecord->frame >= 12) {
        frame.growRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.growRecord->scale += frame.growRecord->scaleStep;
    (frame.growRecord->frame)++;
}
} /* extern "C" */
