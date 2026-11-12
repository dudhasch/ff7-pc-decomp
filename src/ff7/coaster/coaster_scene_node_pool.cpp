// src/ff7/coaster/coaster_scene_node_pool.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Scene node pool for the coaster: hands out and returns scene node slots, creates and
//   destroys scene objects, and links nodes into the parent hierarchy and depth lists.
#include "ff7/types/coaster.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/coaster_psxdata.h"
#include "ff7/coaster_renderer.h"
#include "ff7/api/coaster.h"
#include "lib/api/graphics.h"
#include "lib/pfile.h"


#pragma intrinsic(abs)
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
extern "C" {

#pragma intrinsic(memset)
// clang-format off
// GLOBAL: FF7 0x00c5d360
CoasterNode g_CoasterDepthTailNodes[10] = {0};
// GLOBAL: FF7 0x00c5d590
unsigned char g_CoasterSceneNodePool[5600] = {0};
unsigned char g_bssPad_00c5eb70[5600] = {0};
// GLOBAL: FF7 0x00c60150
CoasterNode g_CoasterRootSceneNode = {0};
// GLOBAL: FF7 0x00c60188
short g_CoasterSceneNodeFreeHead = 0;
unsigned char g_bssPad_00c6018c[4] = {0};
// GLOBAL: FF7 0x00c60190
short g_CoasterSceneNodeFreeLinks[200] = {0};
// GLOBAL: FF7 0x00c60320
CoasterNode g_CoasterDepthHeadNodes[10] = {0};
// clang-format on

/*
 * Initializes root scene node, populates the free-list links for all 200 scene nodes,
 * and sets up circular head/tail sentinel links for the 10 scene depth levels.
 */
// FUNCTION: FF7 0x005ef1c0
void __cdecl Mini_CoasterResetSceneNodePool(void)
{
    int i;

    Mini_CoasterInitLinkedNode(&g_CoasterRootSceneNode, 0);
    g_CoasterRootSceneNode.depth = 0;
    g_CoasterSceneNodeFreeHead = 0;
    for (i = 0; i < 200; i++) {
        g_CoasterSceneNodeFreeLinks[i] = i + 1;
    }
    for (i = 0; i < 10; i++) {
        g_CoasterDepthHeadNodes[i].prev = 0;
        g_CoasterDepthHeadNodes[i].next = (CoasterNode *)(i * 0x38 + (int)&g_CoasterDepthTailNodes);
        g_CoasterDepthTailNodes[i].prev = (CoasterNode *)(i * 0x38 + (int)&g_CoasterDepthHeadNodes);
        g_CoasterDepthTailNodes[i].next = 0;
    }
}
/*
 * Initializes a scene node's transformation matrix to identity (scale 4096),
 * zeroes out translation and links, sets parent link to root, and stores node flags.
 */
// FUNCTION: FF7 0x005ef281
void __cdecl Mini_CoasterInitLinkedNode(CoasterNode *value, int flags)
{
    value->worldMatrix.t[0] = 0;
    value->worldMatrix.t[1] = 0;
    value->worldMatrix.t[2] = 0;
    value->worldMatrix.m[0][0] = 4096;
    value->worldMatrix.m[0][1] = 0;
    value->worldMatrix.m[0][2] = 0;
    value->worldMatrix.m[1][0] = 0;
    value->worldMatrix.m[1][1] = 4096;
    value->worldMatrix.m[1][2] = 0;
    value->worldMatrix.m[2][0] = 0;
    value->worldMatrix.m[2][1] = 0;
    value->worldMatrix.m[2][2] = 4096;
    value->parentNode = &g_CoasterRootSceneNode;
    value->index = (short)flags;
    value->prev = 0;
    value->next = 0;
}

// Coaster scene-node pool of 56-byte records indexed by the g_CoasterSceneNodeFreeHead free list. Minimum
// at most one root plus 99 live actors hold nodes, so indices stay below 100.
/*
 * Allocates a scene node from the pool, links it under the specified parent,
 * assigns model table entry, rotation (from ZYX Euler angles), and translation.
 */
// FUNCTION: FF7 0x005ef31e
int __cdecl Mini_CoasterCreateSceneObject(short modelId, void *parentNode, int posX, int posY, int posZ, short rotX, short rotY, short rotZ)
{
    short slot;
    CoasterNode *pNode;
    MFVec16 euler;

    slot = Mini_CoasterPopSceneNode();
    pNode = (CoasterNode *)(slot * 0x38 + (int)g_CoasterSceneNodePool);
    Mini_CoasterSceneNodeAttachChild(pNode, (CoasterNode *)parentNode);
    pNode->model = g_CoasterModelHandles[modelId];
    pNode->modelId = modelId;
    pNode->index = slot;
    euler.x = rotX;
    euler.y = rotY;
    euler.z = rotZ;
    RotMatrixZYX((SVECTOR *)&euler, &pNode->worldMatrix);
    pNode->worldMatrix.t[0] = posX;
    pNode->worldMatrix.t[1] = posY;
    pNode->worldMatrix.t[2] = posZ;
    return (int)pNode;
}
/*
 * Unlinks a scene node from its hierarchy and returns its slot index to the free list.
 */
// FUNCTION: FF7 0x005ef3bf
void __cdecl Mini_CoasterDestroySceneObject(void *handle)
{
    Mini_CoasterSceneNodeUnlink((CoasterNode *)handle);
    Mini_CoasterPushSceneNode(((CoasterNode *)handle)->index);
}
/*
 * Pops and returns the next free scene node slot index from the free list.
 */
// FUNCTION: FF7 0x005ef3e0
short __cdecl Mini_CoasterPopSceneNode(void)
{
    short val;

    val = g_CoasterSceneNodeFreeHead;
    g_CoasterSceneNodeFreeHead = g_CoasterSceneNodeFreeLinks[g_CoasterSceneNodeFreeHead];
    return (short)val;
}
/*
 * Pushes a released scene node slot index onto the free list.
 */
// FUNCTION: FF7 0x005ef40c
void __cdecl Mini_CoasterPushSceneNode(short index)
{
    g_CoasterSceneNodeFreeLinks[index] = g_CoasterSceneNodeFreeHead;
    g_CoasterSceneNodeFreeHead = index;
}
/*
 * Links a scene node as a child of parent, sets its depth level, and inserts
 * it into the doubly linked list for that depth level.
 */
// FUNCTION: FF7 0x005ef42f
void __cdecl Mini_CoasterSceneNodeAttachChild(CoasterNode *node, CoasterNode *parent)
{
    short depth;
    node->parentNode = parent;
    depth = parent->depth + 1;
    node->depth = depth;
    node->prev = g_CoasterDepthTailNodes[depth].prev;
    node->next = node->prev->next;
    g_CoasterDepthTailNodes[depth].prev->next = node;
    g_CoasterDepthTailNodes[depth].prev = node;
}
/*
 * Unlinks a scene node from its doubly linked depth level list.
 */
// FUNCTION: FF7 0x005ef49e
void __cdecl Mini_CoasterSceneNodeUnlink(CoasterNode *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}
} /* extern "C" */
