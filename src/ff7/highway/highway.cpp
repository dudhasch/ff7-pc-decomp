// src/ff7/highway/highway.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Implements the highway's actor and index classes: actor transforms and rendering, the
//   index that creates and chains actors, and the slot free lists that back them.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/highway_palette.h"
#include "ff7/highway_psxdata.h"
#include <math.h>
#include <stdlib.h>
#include "ff7/types/highway.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/codegen.h"
#include "ff7/api/highway.h"
#include "lib/api/graphics.h"
#include "lib/api/polygon.h"
#include "lib/pfile.h"

#pragma intrinsic(strlen, strcpy, strcat, strcmp)
extern "C" {
#pragma intrinsic(memset)
#pragma intrinsic(atan2, sqrt)

// clang-format on

#pragma intrinsic(abs)
} // extern "C"

/*
 * VC5 emits the ECX receiver and ret 4 only for a genuine C++ member.
 * Installs an identity PSX matrix, makes the pool root the parent, stores value as the slot and clears
 * the model index and the chain links.
 */
// FUNCTION: FF7 0x00659770
void HighwayModelNode::set(short value)
{
    SetIdentityPsxMatrix(&this->psx);
    this->parent = (HighwayModelNode *)g_pHighwayIndexBlock;
    this->slot = value;
    this->modelIndex = 0;
    this->prev = 0;
    this->next = 0;
}
/*
 * Rotates the actor matrix and copies its unaligned position dwords.
 */
// FUNCTION: FF7 0x006597bd
void HighwayModelNode::rotate(const int *position, short *rotation)
{
    RotMatrix((SVECTOR *)rotation, &this->psx);
    this->psx.t[0] = position[0];
    this->psx.t[1] = position[1];
    this->psx.t[2] = position[2];
}

/*
 * Clones the actor's owner slot and inserts it into the new index chain.
 */
// FUNCTION: FF7 0x006597fd
void HighwayModelNode::clone(const HighwayModelNode *other)
{
    this->parent = (HighwayModelNode *)other;
    short slot = other->depth + 1;
    this->depth = slot;
    g_pHighwayIndexBlock->attach(this, slot);
}
extern "C" {

/*
 * Unlinks an ECX-receiver node from a doubly linked list.
 */
// FUNCTION: FF7 0x00659842
void __fastcall Mini_HighwayListNodeUnlink(HighwayModelNode *self)
{
    self->prev->next = self->next;
    self->next->prev = self->prev;
}

/*
 * Unlinks an actor and returns its slot to the ECX receiver's free list.
 */
// FUNCTION: FF7 0x0065986b
void __fastcall Mini_HighwayActorUnlinkAndRelease(void *self)
{
    Mini_HighwayListNodeUnlink((HighwayModelNode *)self);
    g_pHighwayIndexBlock->set(((HighwayModelNode *)self)->slot);
}
} // extern "C"

/*
 * Resolves an indexed actor and stores its slot and list index.
 */
// FUNCTION: FF7 0x00659891
void HighwayModelNode::select(short index, short value)
{
    this->model = (PolygonRsdModelHandle *)g_HighwayRosterBlock->get(index);
    this->slot = value;
    this->modelIndex = index;
}

/*
 * Resolves an indexed actor and stores its list index.
 */
// FUNCTION: FF7 0x006598ca
void HighwayModelNode::select(short index)
{
    this->model = (PolygonRsdModelHandle *)g_HighwayRosterBlock->get(index);
    this->modelIndex = index;
}

/*
 * Copies the actor matrix, composes its parent and subtracts the scene origin.
 * Complete 283-byte extent: checked 100%; named scene slot fixes load order.
 */
// FUNCTION: FF7 0x006598f8
HighwayModelNode *HighwayModelNode::adjustedMatrix(void *output)
{
    PsxMatrix *rec = &this->psx;
    ((PsxMatrix *)output)->m[0][0] = rec->m[0][0];
    ((PsxMatrix *)output)->m[0][1] = rec->m[0][1];
    ((PsxMatrix *)output)->m[0][2] = rec->m[0][2];
    ((PsxMatrix *)output)->m[1][0] = rec->m[1][0];
    ((PsxMatrix *)output)->m[1][1] = rec->m[1][1];
    ((PsxMatrix *)output)->m[1][2] = rec->m[1][2];
    ((PsxMatrix *)output)->m[2][0] = rec->m[2][0];
    ((PsxMatrix *)output)->m[2][1] = rec->m[2][1];
    ((PsxMatrix *)output)->m[2][2] = rec->m[2][2];
    ((PsxMatrix *)output)->t[0] = rec->t[0];
    ((PsxMatrix *)output)->t[1] = rec->t[1];
    ((PsxMatrix *)output)->t[2] = rec->t[2];
    if ((int)this->parent != ((int)((int)g_pHighwayIndexBlock))) {
        MultiplyMatrix(this->parent, output, output);
    }
    ((PsxMatrix *)output)->t[0] -= g_pHighwaySceneBlock->offsetX;
    ((PsxMatrix *)output)->t[1] -= g_pHighwaySceneBlock->offsetY;
    ((PsxMatrix *)output)->t[2] -= g_pHighwaySceneBlock->offsetZ;
    return this;
}

/*
 * Applies the actor transform, configures its draw spec and renders its polygon set.
 * Returns when the actor has no model. Otherwise builds the float matrix and optionally
 * composes the parent, subtracts the three global origin coordinates from the float translation and
 * composes the global matrix. Applies it to the model, initializes draw spec with two callbacks
 * and global matrix pointer, applies that spec to its polygon set and submits the model. Flag is
 * unused.
 */
// FUNCTION: FF7 0x00659a13
void HighwayModelNode::render(int flag)
{
    GfxEngineContext *ctx;
    HighwayMat64 parentMtx;
    GfxGroupDrawSpec drawSpec;

    ctx = GetEngineContext();
    if (this->model == 0) {
        return;
    }
    Mini_HighwayPsxMatrixToFloatTransposed((int)this, (struct PolygonRootMatrix *)this->matrix4x4);
    if ((int)this->parent != ((int)((int)g_pHighwayIndexBlock))) {
        Mini_HighwayPsxMatrixToFloatTransposed((int)this->parent, (struct PolygonRootMatrix *)&parentMtx);
        PostMultiplyMatrix4x4InPlace((int)&parentMtx, (int)this->matrix4x4);
    }
    this->matrix4x4[12] -=
        (float)g_pHighwaySceneBlock->offsetX * 1.0f;
    this->matrix4x4[13] -=
        (float)g_pHighwaySceneBlock->offsetY * 1.0f;
    this->matrix4x4[14] -=
        (float)g_pHighwaySceneBlock->offsetZ * 1.0f;
    PostMultiplyMatrix4x4InPlace((int)g_pHighwayFileBlock->baseMatrix, (int)this->matrix4x4);
    Gfx_SetModelMatrix((int)this->matrix4x4, (int)this->model, ctx);
    Gfx_InitDrawSpec(0x12, &drawSpec.spec);
    drawSpec.spec.vertexAlphaCallback = (int)Mini_HighwayScaleColorTriplet;
    drawSpec.spec.mode = 1;
    drawSpec.spec.callback = (int)Mini_HighwaySetRecordMatrix;
    g_pHighwayFileBlock->recordMatrix = (int)this->matrix4x4;
    Gfx_ApplyDrawSpecToPolygonSet(&drawSpec.spec, (GfxPolygonSet *)this->model->polygonSet);
    Gfx_DrawMeshPolygonSet((int)this->model, (int)ctx);
}
extern "C" {

// Converts a PSX-style matrix into a transposed float 4x4 through GtePsxMatrixToFloat4x4Transposed.
// The follow-up that would scale the translate fields by 1.0f sits under if (DEAD_IN_RETAIL != 0) and never
// runs; it is kept because the original compiled the same dead branch.
// FUNCTION: FF7 0x00659b9f
void __cdecl Mini_HighwayPsxMatrixToFloatTransposed(int psxMatrix, PolygonRootMatrix *dst)
{
    GtePsxMatrixToFloat4x4Transposed((void *)psxMatrix, dst);
    if (DEAD_IN_RETAIL != 0) {
        dst->translateX *= 1.0f;
        dst->translateY *= 1.0f;
        dst->translateZ *= 1.0f;
    }
}
} // extern "C"

/*
 * Builds the float matrix, copies the same global matrix for either low-byte flag value, clears its
 * translation/W and subtracts 1200 from float word 13. Composes it with this matrix, applies to the
 * model and submits it. First two arguments are unused.
 */
// FUNCTION: FF7 0x00659bf0
void HighwayModelNode::drawLayer(int unused1, int unused2, char flag)
{
    // VC5's name buckets place these at -4, -0x44 and -0x48; the ternary
    // produces the original unnamed temporary at -0x50 after the receiver.
    GfxEngineContext *ctx;
    HighwayMat64 mat;
    int src;

    ctx = GetEngineContext();
    Mini_HighwayPsxMatrixToFloatTransposed((int)this, (struct PolygonRootMatrix *)this->matrix4x4);
    // The caller passes only the low byte; the original callee reads the
    // entire stack word before masking it back to that byte.
    src = ((*(int *)&flag) & 0xFF) ? (int)g_pHighwayFileBlock->baseMatrix : (int)g_pHighwayFileBlock->baseMatrix;
    mat = *(HighwayMat64 *)src;
    ClearMatrixTranslationAndW(&mat);
    *(float *)&mat.words[13] -= 1200.0f;
    PostMultiplyMatrix4x4InPlace((int)&mat, (int)this->matrix4x4);
    Gfx_SetModelMatrix((int)this->matrix4x4, (int)this->model, ctx);
    Gfx_DrawMeshPolygonSet((int)this->model, (int)ctx);
}

/*
 * Sets the root actor with slot zero, clears the root depth and the free-list head and chains the 400
 * free slots. Links each of the ten head/tail pairs to each other with zero outer ends.
 */
// FUNCTION: FF7 0x00659cad
HighwayIndex::HighwayIndex()
{
    int recordIndex;

    this->root.set(0);
    this->root.depth = 0;
    this->freeHead = 0;
    {
        int i;
        for (i = 0; i < 0x190; i++) {
            this->nextFree[i] = i + 1;
        }
    }
    for (recordIndex = 0; recordIndex < 10; recordIndex++) {
        this->heads[recordIndex].prev = 0;
        this->heads[recordIndex].next = &this->tails[recordIndex];
        this->tails[recordIndex].prev = &this->heads[recordIndex];
        this->tails[recordIndex].next = 0;
    }
}
extern "C" {

/*
 * Pops the first free actor slot off the pool's free list.
 */
// FUNCTION: FF7 0x00659d8c
short __fastcall Mini_HighwayIndexPopFreeSlot(HighwayIndex *self)
{
    short val;

    val = self->freeHead;
    self->freeHead = self->nextFree[self->freeHead];
    return val;
}
} // extern "C"

/*
 * VC5 emits the ECX receiver and ret 4 from a genuine C++ member.
 * Pushes a slot back on the free list: its successor becomes the current head, then it becomes the head.
 * No index bound is checked.
 */
// FUNCTION: FF7 0x00659dca
void HighwayIndex::set(short value)
{
    this->nextFree[(int)value] = this->freeHead;
    this->freeHead = value;
}

/*
 * Takes a free slot from Mini_HighwayIndexPopFreeSlot, clones other into that slot's actor, selects
 * index/slot and rotates it with the supplied integer position and short angles.
 * Returns it without validating the slot.
 */
// FUNCTION: FF7 0x00659dfe
HighwayModelNode *HighwayIndex::createRaw(short index, const HighwayModelNode *other, int x, int y, int z,
                              short rx, short ry, short rz)
{
    short rot[4] = {rx, ry, rz};
    HighwayModelNode *actor;
    int pos[4] = {x, y, z};
    short slot;

    slot = Mini_HighwayIndexPopFreeSlot(this);
    actor = &this->actors[slot];
    actor->clone(other);
    actor->select(index, slot);
    actor->rotate(pos, rot);
    return actor;
}

/*
 * Creates one indexed actor, chains it, and copies its transform and position.
 * Takes a free slot, clones other into that slot's actor, selects index/slot and
 * rotates with the two supplied arrays. Returns the actor without slot or pointer validation.
 */
// FUNCTION: FF7 0x00659e93
HighwayModelNode *HighwayIndex::create(short index, const HighwayModelNode *other, const int *position, short *rotation)
{
    short slot = Mini_HighwayIndexPopFreeSlot(this);
    HighwayModelNode *actor = &this->actors[slot];
    actor->clone(other);
    actor->select(index, slot);
    actor->rotate(position, rotation);
    return actor;
}

/*
 * Inserts a node into the chain for the given depth.
 * Inserts node before the chain's tail: sets node previous/next pointers from the tail's
 * previous node, updates that node's next pointer, and makes node the tail's previous.
 */
// FUNCTION: FF7 0x00659ef3
void HighwayIndex::attach(void *node, int slot)
{
    ((HighwayModelNode *)node)->prev = this->tails[slot].prev;
    ((HighwayModelNode *)node)->next = ((HighwayModelNode *)node)->prev->next;
    this->tails[slot].prev->next = (HighwayModelNode *)node;
    this->tails[slot].prev = (HighwayModelNode *)node;
}
