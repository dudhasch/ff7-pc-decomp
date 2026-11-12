// src/ff7/highway/highway_load_hrc.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Loads the highway's skeletons and their animations from HRC and animation files, creates and
//   releases the players, and drives animation selection, drawing and ticking for the skeleton lists.

#include <string.h>
#include "ff7/api/snobo.h"
#include "ff7/highway_palette.h"
#include "lib/codegen.h"
#include "ff7/highway_psxdata.h"
#include <math.h>
#include <stdlib.h>
#include "ff7/types/highway.h"
#include "lib/types/file.h"
#include "lib/types/polygon.h"
#include "lib/types/mem.h"
#include "lib/types/graphics.h"
#include "ff7/api/highway.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "lib/pfile.h"

#pragma intrinsic(strlen, strcpy, strcat, strcmp)
struct FileOpenRequest;
struct PolygonAnimation;
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
extern "C" {
// The ten skeleton rows HighwaySkelList::load sums (stride 0x3C) and HighwaySkelItem::setup reads: the row count, the
// HRC name and its option, the animation option, ten animation names and the player model. The names are
// initializer literals, emitted after the table.
typedef struct HighwaySkelRow {
    int count;
    const char *hrcName;
    int hrcOption;
    int animationOption;
    const char *animationNames[10];
    int playerModel;
} HighwaySkelRow;
// clang-format off
// GLOBAL: FF7 0x0090a280
HighwaySkelRow g_HighwaySkelTable[10] = {
    {
        1,
        // STRING: FF7 0x0090a4d8
        "caaa.hrc",
        0,
        0,
        {
            // STRING: FF7 0x0090a4e4
            "dgaa",
            // STRING: FF7 0x0090a4ec
            "egaa",
            // STRING: FF7 0x0090a4f4
            "fgaa",
            // STRING: FF7 0x0090a4fc
            "ggaa",
            // STRING: FF7 0x0090a504
            "hgaa",
            // STRING: FF7 0x0090a50c
            "igaa",
            // STRING: FF7 0x0090a514
            "jgaa",
            0,
            0,
            0,
        },
        30,
    },
    {
        1,
        // STRING: FF7 0x0090a51c
        "pbaa.hrc",
        0,
        0,
        {
            // STRING: FF7 0x0090a528
            "kgaa",
            // STRING: FF7 0x0090a530
            "lgaa",
            // STRING: FF7 0x0090a538
            "mgaa",
            // STRING: FF7 0x0090a540
            "ngaa",
            // STRING: FF7 0x0090a548
            "ogaa",
            // STRING: FF7 0x0090a550
            "pgaa",
            0,
            0,
            0,
            0,
        },
        30,
    },
    {
        1,
        // STRING: FF7 0x0090a558
        "ybaa.hrc",
        0,
        0,
        {
            // STRING: FF7 0x0090a564
            "qgaa",
            // STRING: FF7 0x0090a56c
            "rgaa",
            // STRING: FF7 0x0090a574
            "sgaa",
            // STRING: FF7 0x0090a57c
            "tgaa",
            0,
            0,
            0,
            0,
            0,
            0,
        },
        15,
    },
    {
        1,
        // STRING: FF7 0x0090a584
        "ycaa.hrc",
        0,
        0,
        {
            // STRING: FF7 0x0090a590
            "qgaa",
            // STRING: FF7 0x0090a598
            "rgaa",
            // STRING: FF7 0x0090a5a0
            "sgaa",
            // STRING: FF7 0x0090a5a8
            "tgaa",
            0,
            0,
            0,
            0,
            0,
            0,
        },
        15,
    },
    {
        1,
        // STRING: FF7 0x0090a5b0
        "zcaa.hrc",
        0,
        0,
        {
            // STRING: FF7 0x0090a5bc
            "ugaa",
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        },
        15,
    },
    {
        3,
        // STRING: FF7 0x0090a5c4
        "ydaa.hrc",
        0,
        0,
        {
            // STRING: FF7 0x0090a5d0
            "vgaa",
            // STRING: FF7 0x0090a5d8
            "wgaa",
            // STRING: FF7 0x0090a5e0
            "xgaa",
            // STRING: FF7 0x0090a5e8
            "ygaa",
            // STRING: FF7 0x0090a5f0
            "zgaa",
            // STRING: FF7 0x0090a5f8
            "ahaa",
            0,
            0,
            0,
            0,
        },
        15,
    },
    {
        3,
        // STRING: FF7 0x0090a600
        "veaa.hrc",
        0,
        0,
        {
            // STRING: FF7 0x0090a60c
            "bhaa",
            // STRING: FF7 0x0090a614
            "chaa",
            // STRING: FF7 0x0090a61c
            "dhaa",
            // STRING: FF7 0x0090a624
            "ehaa",
            // STRING: FF7 0x0090a62c
            "fhaa",
            0,
            0,
            0,
            0,
            0,
        },
        15,
    },
    {
        3,
        // STRING: FF7 0x0090a634
        "efaa.hrc",
        0,
        0,
        {
            // STRING: FF7 0x0090a640
            "ghaa",
            // STRING: FF7 0x0090a648
            "hhaa",
            // STRING: FF7 0x0090a650
            "ihaa",
            // STRING: FF7 0x0090a658
            "jhaa",
            // STRING: FF7 0x0090a660
            "khaa",
            0,
            0,
            0,
            0,
            0,
        },
        15,
    },
    {
        3,
        // STRING: FF7 0x0090a668
        "nfaa.hrc",
        0,
        0,
        {
            // STRING: FF7 0x0090a674
            "lhaa",
            // STRING: FF7 0x0090a67c
            "mhaa",
            // STRING: FF7 0x0090a684
            "nhaa",
            // STRING: FF7 0x0090a68c
            "ohaa",
            // STRING: FF7 0x0090a694
            "phaa",
            0,
            0,
            0,
            0,
            0,
        },
        15,
    },
    {
        1,
        // STRING: FF7 0x0090a69c
        "wfaa.hrc",
        0,
        0,
        {
            // STRING: FF7 0x0090a6a8
            "qhaa",
            // STRING: FF7 0x0090a6b0
            "rhaa",
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        },
        15,
    },
};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(atan2, sqrt)


#pragma intrinsic(abs)
} // extern "C"

/*
 * Loads a hierarchy from disk or memory and initializes its visible parts.
 * Loads the HRC from disk or from the context's archive, according to the archive global. Stores
 * the result in the skeleton member; when nonzero builds hierarchy/parts, enables parts and
 * returns one, otherwise zero. Does not test the subsequent hierarchy operation here.
 */
// FUNCTION: FF7 0x0065bce0
int HighwaySkelItem::loadHrc(int path, int metadata, int context, int parts)
{
    int unused = 0;
    if (!g_HighwayUseLgpArchive) {
        this->skeleton = (PolygonSkelHierarchy *)Skel_LoadHrcFromDisk(unused, path);
    } else {
        this->skeleton = (PolygonSkelHierarchy *)Skel_LoadHrc(context + 0x40, unused, path);
    }
    if (this->skeleton) {
        Skel_BuildHierarchyAndLoadParts(metadata, this->skeleton, (struct PfileLoadOptions *)context, parts);
        Skel_SetPartsVisible(1, this->skeleton);
        return 1;
    }
    return 0;
}

/*
 * Creates and binds an animation player for this skeleton item.
 * Creates an animation player into this->player. For a nonzero result binds the supplied
 * animation, clears the player's catchUpEnabled and returns one; otherwise returns zero.
 */
// FUNCTION: FF7 0x0065bd6f
int HighwaySkelItem::makePlayer(int model, int options, int animation)
{
    this->player = (HighwayPlayerState *)AnimPlayer_Create(model, 0, this->skeleton, options);
    if (this->player) {
        AnimPlayer_BindAnimation(0, animation, (PolygonAnimPlayer *)(this->player));
        this->player->catchUpEnabled = 0;
        return 1;
    }
    return 0;
}

/*
 * Copies path into two 256-byte arrays and appends .anm and .A. Returns zero if text compilation
 * fails; otherwise loads binary from disk or from the context's archive and returns that result.
 */
// FUNCTION: FF7 0x0065bdce
int HighwaySkelItem::compileAndLoadAnimation(int context, const char *path)
{
    char animationText[256];
    char binPath[256];
    PolygonAnimation *animPtr;

    strcpy(animationText, path);
    // STRING: FF7 0x0090a6b8
    strcat(animationText, ".anm");
    strcpy(binPath, path);
    // STRING: FF7 0x0090a6c0
    strcat(binPath, ".A");
    if (!Anim_CompileTextToBinary((int)animationText, (int)binPath)) {
        return 0;
    }
    if (!g_HighwayUseLgpArchive) {
        /* Retail caller reads residual EAX/AX as `int` after `void Anim_LoadBinaryFromDisk` returns. */
        animPtr = (PolygonAnimation *)((int (__cdecl *)(int))Anim_LoadBinaryFromDisk)((int)binPath);
    } else {
        animPtr = (PolygonAnimation *)Anim_LoadBinary((FileOpenRequest *)(context + 0x40), (int)binPath);
    }
    return (int)animPtr;
}

/*
 * Counts the null-terminated animation-name array, allocates that many pointers and skips names
 * starting with asterisk. Optionally prefixes each name with the count argument interpreted as
 * directory string, compiles/loads and packs successful pointers. Any failure returns zero without
 * cleanup or correcting the original count; full success replaces count with loaded number.
 */
// FUNCTION: FF7 0x0065beda
int HighwaySkelItem::loadAnimations(int count, int array, int context)
{
    char path[256];
    int animation;
    int i;
    int loaded;

    loaded = 0;
    animation = 0;
    while (((int *)array)[loaded] != 0) {
        loaded++;
    }
    this->animationCount = loaded;
    this->animations =
        (int *)operator new(this->animationCount * 4);
    if (this->animations == 0) {
        return 0;
    }
    loaded = 0;
    for (i = 0; i < this->animationCount; i++) {
        if (*(char *)((int *)array)[i] == '*') {
            continue;
        }
        if (count != 0 && strlen((char *)count) > 0) {
            strcpy(path, (char *)count);
            // STRING: FF7 0x0090a6c4
            strcat(path, "\\");
            strcat(path, (char *)((int *)array)[i]);
        } else {
            strcpy(path, (char *)((int *)array)[i]);
        }
        animation = compileAndLoadAnimation(context, path);
        if (animation != 0) {
            this->animations[loaded] = animation;
            loaded++;
        } else {
            return 0;
        }
    }
    this->animationCount = loaded;
    return 1;
}

/*
 * Loads the HRC, its animations and animation player in that order.
 */
// FUNCTION: FF7 0x0065c0eb
int HighwaySkelItem::setup(const int *record, int context, int parts)
{
    if (!loadHrc(record[1], record[2], context, parts)) {
        return 0;
    }
    if (!loadAnimations(record[3], (int)(record + 4), context)) {
        return 0;
    }
    if (!makePlayer(record[14], parts, *this->animations)) {
        return 0;
    }
    return 1;
}

/*
 * Frees one skeleton item, its animations, player and backing array.
 * Frees skeleton and clears its member, frees each count-listed animation and clears entries,
 * clears two player fields, destroys player and clears it, then releases the animation array. Does
 * not clear array pointer/count and dereferences player without a null check.
 */
// FUNCTION: FF7 0x0065c169
void HighwaySkelItem::release()
{
    Skel_Free(this->skeleton);
    this->skeleton = 0;
    int animArray;
    int p;
    for (p = 0; p < this->animationCount; p++) {
        Anim_Free((PolygonAnimBuffers *)((void *)this->animations[p]));
        this->animations[p] = 0;
    }
    this->player->animation = 0;
    this->player->skel = 0;
    AnimPlayer_Destroy((PolygonAnimPlayer *)this->player);
    this->player = 0;
    animArray = (int)this->animations;
    operator delete((void *)animArray);
}

/*
 * Stores value in the player's isEnabled, calls Skel_SetPartsVisible with value and the player's
 * skeleton, and returns this.
 */
// FUNCTION: FF7 0x0065c21c
HighwayPart *HighwayPart::visible(int value)
{
    (*(HighwayPlayerState **)((char *)this + 0x14))->isEnabled = value;
    Skel_SetPartsVisible(value, (PolygonSkelHierarchy *)(*(HighwayPlayerState **)((char *)this + 0x14))->skel);
    return this;
}

/*
 * Sets animation index or adds nonzero delta, lower-clamps zero then upper-clamps count-minus-one
 * and indexes animation table. Nonnull animation resets playback fields, timestamps and scales
 * root translation, optionally bakes matrices. Zero count yields minus-one index after clamp; null
 * animation preserves playback state.
 */
// FUNCTION: FF7 0x0065c250
void HighwayFieldItem::setAnimation(int value, int delta, int bake)
{
    PolygonAnimation *rec;

    if (delta != 0) {
        this->index += delta;
    } else {
        this->index = value;
    }
    if (this->index < 0) {
        this->index = 0;
    }
    if (this->index >= this->count) {
        this->index = this->count - 1;
    }
    rec = (PolygonAnimation *)(((int *)this->animations)[this->index]);
    if (rec != 0) {
        this->player->isEnabled = 1;
        this->player->resetPosition = 0;
        this->player->speed = 1;
        this->player->catchUpEnabled = 0;
        this->player->delay = 1;
        this->player->holdTimerStart = 1;
        this->player->position = this->currentFrame;
        this->player->animation = (int)rec;
        this->player->positionLimit = rec->frameCount;
        ReadTimestampCounter(&this->player->nextTime);
        Add64(&this->player->interval,
              &this->player->nextTime);
        Anim_ScaleRootTranslation(this->player->rootScale,
                                  rec);
        if (bake != 0) {
            Anim_BakeAllFrameMatrices((PolygonAnimBake *)(this->player->animation),
                                      (PolygonAnimModel *)(this->player->skel));
        }
    }
}

/*
 * Resets invalid selected animation index to zero, computes its frame limit as stored count minus
 * one and clamps current frame first below zero then above that limit. Stores it into the player's
 * position and draws only when the player is enabled. Inverted frame range can produce minus one.
 */
// FUNCTION: FF7 0x0065c3b3
void HighwayFieldItem::draw(int value)
{
    int lastFrame;
    HighwayPlayerState *player;
    HighwayPlayerState *playerBase;

    if (this->index < 0 ||
        this->index >= this->count) {
        this->index = 0;
    }
    lastFrame = *(int *)(*(int *)(this->animations +
                          this->index * 4) +
                 4) -
        1;
    if (this->currentFrame < 0) {
        this->currentFrame = 0;
    }
    if (this->currentFrame > lastFrame) {
        this->currentFrame = lastFrame;
    }
    player = this->player;
    player->position = this->currentFrame;
    if (this->player->isEnabled != 0) {
        playerBase = this->player;
        Anim_DrawSkeletonFrame(
            this->currentFrame,
            (PolygonSkeletonAnim *)(*(int *)(this->animations + this->index * 4)),
            (PolygonBoneTransform *)((char *)playerBase + 0x38), (PolygonAnimDraw *)this->skeleton, (GfxEngineContext *)value);
    }
}

/*
 * Sums counts from ten stride 0x3C table rows, allocates count*0x18 item storage and calls setup
 * once per row count using the same row record. Returns zero on allocation or any setup failure
 * without cleanup, otherwise one.
 */
// FUNCTION: FF7 0x0065c47b
int HighwaySkelList::load(int context, int parts)
{
    int added;
    int row;
    HighwaySkelRow *table;
    int j;
    int limit;
    int createOk;

    table = g_HighwaySkelTable;
    limit = 10;
    count = 0;
    for (row = 0; row < limit; row++) {
        count += table[row].count;
    }
    items = (char *)operator new(count * 0x18);
    if (items == 0) {
        return 0;
    }
    added = 0;
    for (row = 0; row < limit; row++) {
        for (j = 0; j < table[row].count; j++) {
            createOk = ((HighwaySkelItem *)(items + added * 0x18))
                           ->setup(&table[row].count, context, parts);
            if (createOk == 0) {
                return 0;
            }
            added++;
        }
    }
    return 1;
}

/*
 * Releases each skeleton item and then its backing array.
 * Calls release on count entries of stride 0x18 in items, then passes items to
 * operator delete. It does not clear items or count.
 */
// FUNCTION: FF7 0x0065c584
void HighwaySkelList::release()
{
    int itemArray;
    int p;
    for (p = 0; p < count; p++) {
        ((HighwaySkelItem *)(items + p * 0x18))->release();
    }
    itemArray = (int)items;
    operator delete((void *)itemArray);
}

/*
 * Timestamps once and visits every valid skeleton player in the list. When its scheduled timestamp
 * is due, advances its frame if not paused and updates timing either by catch-up additions or by
 * resetting an overly stale schedule; stale comparison uses only the unsigned low dword of elapsed
 * time against a context double. Handles endpoint by holding the last frame, resetting to the
 * start for looping or remaining repeats, invoking a callback, or hiding parts. Draws when
 * scheduling or endpoint logic permits. Catch-up loops may repeatedly advance frame; zero timing
 * intervals require caller guarantees. Explicit eight-byte records back all timestamps.
 */
// FUNCTION: FF7 0x0065c5d7
void HighwaySkelList::tick(int context)
{
    int skelIndex;
    struct Timestamp64 *nowPtr8;
    struct Timestamp64 time;
    HighwayPlayerState *player;
    int active;
    int draw;
    struct Timestamp64 dt;

    nowPtr8 = (struct Timestamp64 *)&time;
    ReadTimestampCounter(nowPtr8);
    for (skelIndex = 0; skelIndex < count; skelIndex++) {
        draw = 0;
        player = ((HighwayFieldItem *)(items + skelIndex * 0x18))->player;
        if (player == 0 || player->isEnabled == 0) {
            continue;
        }
        if (Compare64(nowPtr8, &player->nextTime) >= 0) {
            if (player->catchUpEnabled != 0) {
                active = 1;
                if (player->isActive == 0) {
                    player->position += player->speed;
                }
                Sub64(nowPtr8, &player->nextTime, (struct Timestamp64 *)&dt);
                if ((double)(unsigned int)dt.lo > *(double *)(context + 0x30)) {
                    active = 0;
                    player->nextTime = *(struct Timestamp64 *)nowPtr8;
                    Add64(&player->interval, &player->nextTime);
                }
                if (active != 0) {
                    while (Compare64(nowPtr8, &player->nextTime) >= 0) {
                        Add64(&player->interval, &player->nextTime);
                        if (player->isActive == 0) {
                            player->position += player->speed;
                        } else {
                            break;
                        }
                    }
                }
            } else {
                if (Compare64(nowPtr8, &player->nextTime) >= 0) {
                    player->nextTime = *(struct Timestamp64 *)nowPtr8;
                    Add64(&player->interval, &player->nextTime);
                }
                if (player->isActive == 0) {
                    player->position += player->speed;
                }
            }
            if (player->position >= player->positionLimit) {
                if (player->holdLastFrame != 0) {
                    player->position = player->positionLimit - 1;
                    draw = 1;
                } else {
                    player->position = player->resetPosition;
                    if (player->isLooping != 0) {
                        draw = 1;
                    } else if (player->delay > 0) {
                        player->delay -= 1;
                        draw = 1;
                    } else if (player->endCallback != 0) {
                        /* Invokes untyped callback slot `player->endCallback` with its runtime (int) -> int signature. */
                        draw = ((int(__cdecl *)(int))player->endCallback)((int)player);
                    } else {
                        Skel_SetPartsVisible(0, (PolygonSkelHierarchy *)(player->skel));
                    }
                }
            } else {
                draw = 1;
            }
        } else {
            draw = 1;
        }
        if (draw != 0) {
            ((HighwayFieldItem *)(items + skelIndex * 0x18))->draw(context);
        }
    }
}
