// src/ff7/field/src/field_head_mesh_commands.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Head mesh commands for field models: the eye textures of a blink applied to a model
//   skeleton record.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "lib/types/graphics.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "lib/api/graphics.h"
#include "ff7/api/field.h"
#include "ff7/enums/field.h"
extern "C" {

// The name Field_ApplyHeadMeshCommands compares the records with: the first definition of the TU's data stream,
// before the sine table (retail 0x908E28; after field_model_init_tables.cpp's data, so not ad_pal.cpp's).
// GLOBAL: FF7 0x00908e28
char gFieldHeadName[5] = "head";
// clang-format off
// clang-format on
#pragma intrinsic(abs)

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Processes head mesh commands (the eye textures of a blink) on a model skeleton record.
 */
// FUNCTION: FF7 0x00649b50
int __cdecl Field_ApplyHeadMeshCommands(FieldActorRecord *actor, unsigned char *commands)
{
    struct {
        int recordIndex;
        FieldHeadBoneRecord *record;
        int index;
        GfxPolygonSet *mesh;
        FieldHeadResource *resource;
        int byte3;
        int command;
        FieldHeadPlayer *player;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.byte3 = commands[3];
    frame.player = (FieldHeadPlayer *)actor->player;
    if (frame.player != 0) {
        frame.resource = frame.player->resource;
        if (frame.resource != 0) {
            frame.recordIndex = 0;
            frame.record = frame.player->resource->records;
            if (actor->partyId >= 0x21)
                return 1;
            while (frame.recordIndex < frame.resource->recordCount && _stricmp(frame.record->name, gFieldHeadName) != 0) {
                frame.recordIndex++;
                frame.record++;
            }
            if (frame.recordIndex == frame.resource->recordCount)
                return 0;
            frame.mesh = (GfxPolygonSet *)frame.record->link->next->next;
            D3DLinkPolygonSetGroupsIdentity((void *)frame.mesh);
            frame.command = commands[0];
            frame.index = 0;
            while (frame.index < frame.mesh->groupCount && ((FieldHeadMeshGroup *)frame.mesh->groupState)[frame.index].drawState == 0) {
                frame.index++;
            }
            if (frame.index == frame.mesh->groupCount)
                return 0;
            switch (frame.command) {
            case FIELD_HEAD_MESH_EYE_OPEN:
                D3DSetPolygonSetGroupSlot(frame.index, frame.mesh->groupState + frame.index * 0x64, (void *)frame.mesh);
                break;
            case FIELD_HEAD_MESH_EYE_CLOSED:
                frame.mesh->useGroupStates = 1;
                if (actor->groupSlot != 0) {
                    D3DSetPolygonSetGroupSlot(frame.index, actor->groupSlot, (void *)frame.mesh);
                } else {
                    D3DSetPolygonSetGroupSlot(frame.index, frame.mesh->groupState + frame.index * 0x64, (void *)frame.mesh);
                    return 0;
                }
                break;
            default:
                frame.mesh->useGroupStates = 0;
                return 0;
            }
            frame.command = commands[1];
            frame.index++;
            if (frame.index == frame.mesh->groupCount)
                return 0;
            switch (frame.command) {
            case FIELD_HEAD_MESH_EYE_OPEN:
                D3DSetPolygonSetGroupSlot(frame.index, frame.mesh->groupState + frame.index * 0x64, (void *)frame.mesh);
                break;
            case FIELD_HEAD_MESH_EYE_CLOSED:
                frame.mesh->useGroupStates = 1;
                if (*(int *)((int)actor + frame.command * 4 + 0x184) != 0) {
                    D3DSetPolygonSetGroupSlot(frame.index, actor->headSlot, (void *)frame.mesh);
                } else {
                    D3DSetPolygonSetGroupSlot(frame.index, frame.mesh->groupState + frame.index * 0x64, (void *)frame.mesh);
                    return 0;
                }
                break;
            default:
                frame.mesh->useGroupStates = 0;
                return 0;
            }
            return 1;
        }
    }
    return 0;
}
} /* extern "C" */
