// src/lib/src/graphics/render.cpp
// Original: C:\lib\src\graphics\render.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "lib/pfile.h"
#include "lib/api/file.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00db2c0c
struct GfxModelTable *g_GfxLastCreatedModelTable = 0;
// clang-format on

/*
 * Retrieves the last created model table pointer and clears the cache slot.
 */
// FUNCTION: FF7 0x00689820
int __cdecl Gfx_TakeLastCreatedModelTable(void)
{
    GfxModelTable *modelTable;

    modelTable = g_GfxLastCreatedModelTable;
    g_GfxLastCreatedModelTable = 0;
    return (int)modelTable;
}
typedef struct {
    int value;
} RenderWord;
// The local prototypes below take this TU's argument types, which differ from other TUs' declarations.

/*
 * Releases all polygon sets, matrices, and resources in a model table.
 */
// FUNCTION: FF7 0x0068983d
void __cdecl Gfx_FreeModelTable(GfxModelTable *table)
{
    GfxEngineContext *ctx;
    GfxModelTableEntry *grp;
    GfxModelTableRecord *row;
    int i;

    if (table != 0) {
        if (table->entries != 0) {
            ctx = GetEngineContext();
            row = (GfxModelTableRecord *)table->records;
            grp = (GfxModelTableEntry *)table->entries;
            for (i = 0; i < table->count; i++) {
                if ((row->flags & 0x800) != 0) {
                    DestroyRenderObjectAndClear(&grp->renderObject);
                    Gfx_SptFreeRecordAndNull((int)&grp->loadedRecord);
                } else {
                    if ((row->flags & 0x100) != 0) {
                        Gfx_DestroyTexturedHundred(grp->loadedRecord);
                    }
                    Pfile_FreeModelAndNull((int)&grp->renderObject, ctx);
                }
                row++;
                grp++;
            }
            // STRING: FF7 0x00910658
            GameHeapFree((void *)table->entries, "C:\\lib\\src\\graphics\\render.cpp", 0x3A);
            if (table->ownerRef != 0) {
                *(int *)table->ownerRef = 0;
            }
        }
        // STRING: FF7 0x00910678
        GameHeapFree(table, "C:\\lib\\src\\graphics\\render.cpp", 0x42);
    }
}

/*
 * Frees a model table and clears caller reference pointer.
 */
// FUNCTION: FF7 0x0068994e
void __cdecl Gfx_FreeModelTableAndClear(void *tableRef)
{
    if (tableRef != 0 && *(int *)tableRef != 0) {
        Gfx_FreeModelTable((GfxModelTable *)(*(int *)tableRef));
        *(int *)tableRef = 0;
    }
}

/*
 * Constructs and populates a model table from resource record descriptors.
 */
// FUNCTION: FF7 0x00689978
int __cdecl Gfx_LoadModelTableFromRecords(int groupTag, int lifetime, int fileContext, int count, GfxModelTableRecord *records, int ownerRef, GfxModelTableCreateDesc *defaults)
{
    PfileLoadOptions options;
    GfxEngineContext *ctx;
    int failed;
    GfxModelTable *pTable;
    GfxModelTableEntry *entryCursor;
    PfileLoadOptions *optionsPtr;
    GfxModelTableRecord *recordCursor;
    int zero;
    int useState;
    int rendBits;
    int i;
    char pathBuffer[200];
    int obj;

    failed = 1;
    // STRING: FF7 0x00910698
    pTable = (GfxModelTable *)GameHeapAlloc(1, 0x24, "C:\\lib\\src\\graphics\\render.cpp", 0x55);
    if (pTable != 0) {
        pTable->fileContext = fileContext;
        pTable->count = count;
        pTable->records = (int)records;
        pTable->groupTag = groupTag;
        pTable->lifetime = lifetime;
        pTable->ownerRef = ownerRef;
        // STRING: FF7 0x009106b8
        pTable->entries = (int)GameHeapAlloc(pTable->count, 0xC, "C:\\lib\\src\\graphics\\render.cpp", 0x61);
        if (pTable->entries != 0) {
            ctx = GetEngineContext();
            optionsPtr = (PfileLoadOptions *)&options;
            recordCursor = records;
            entryCursor = (GfxModelTableEntry *)pTable->entries;
            for (i = 0; i < pTable->count; i++) {
                Pfile_InitLoadOptions(optionsPtr);
                optionsPtr->directory = (char *)fileContext;
                optionsPtr->file = *(FileOpenRequest *)((int)defaults + 0xC);
                optionsPtr->matrixCount = 0;
                if ((recordCursor->flags & 0x10) != 0) {
                    optionsPtr->matrixCount = 1;
                }
                if ((recordCursor->flags & 0x20) != 0) {
                    optionsPtr->polygonFlags = 1;
                }
                if ((recordCursor->flags & 0x8000) != 0) {
                    optionsPtr->extraFlags |= 1;
                    if (recordCursor->optionValue != 0) {
                        optionsPtr->colorKey = recordCursor->optionValue;
                    } else {
                        *(RenderWord *)&optionsPtr->colorKey = *(RenderWord *)((char *)defaults + 8);
                    }
                }
                rendBits = ctx->renderObjectBank;
                if (defaults != 0) {
                    if (defaults->drawPool != 0) {
                        optionsPtr->drawPool = defaults->drawPool;
                    }
                    if (defaults->renderBits != 0) {
                        rendBits = defaults->renderBits;
                    }
                }
                if (ctx->driverType == GFX_DRIVER_SOFTWARE) {
                    optionsPtr->paletteFlags |= 1;
                    optionsPtr->paletteFlags |= 2;
                }
                optionsPtr->scale = (float)recordCursor->scale;
                Pfile_SetLoadOptionsBlendMode(recordCursor->flags & GFX_MODEL_RECORD_BLEND_MASK, optionsPtr);
                if ((recordCursor->flags & 8) != 0) {
                    optionsPtr->blendFlags |= 1;
                }
                if ((recordCursor->flags & 0x80) != 0) {
                    optionsPtr->flags |= 0x2000000;
                }
                if ((recordCursor->flags & GFX_MODEL_RECORD_RENDER_OBJECT) != 0) {
                    if ((recordCursor->flags & 0x2000) != 0) {
                        useState = 1;
                    } else {
                        useState = 0;
                    }
                    if ((recordCursor->flags & GFX_MODEL_RECORD_SCREEN_2D) != 0) {
                        entryCursor->kind = GFX_MODEL_ENTRY_BATCH;
                        if (recordCursor->name != 0) {
                            optionsPtr->emitterFlags |= 2;
                            if ((recordCursor->flags & GFX_MODEL_RECORD_TRIANGLES) != 0) {
                                if ((recordCursor->flags & GFX_MODEL_RECORD_GOURAUD) != 0) {
                                    entryCursor->renderObject = (int)Gfx_CreateRenderObject(useState, GFX_DRAW_TEXTURED_GOURAUD_TRI_2D, optionsPtr, (char *)recordCursor->name, (void *)rendBits);
                                } else {
                                    entryCursor->renderObject = (int)Gfx_CreateRenderObject(useState, GFX_DRAW_TEXTURED_TRI_2D, optionsPtr, (char *)recordCursor->name, (void *)rendBits);
                                }
                            } else if ((recordCursor->flags & GFX_MODEL_RECORD_GOURAUD) != 0) {
                                entryCursor->renderObject = (int)Gfx_CreateRenderObject(useState, GFX_DRAW_TEXTURED_GOURAUD_QUAD_2D, optionsPtr, (char *)recordCursor->name, (void *)rendBits);
                            } else {
                                entryCursor->renderObject = (int)Gfx_CreateRenderObject(useState, GFX_DRAW_TEXTURED_QUAD_2D, optionsPtr, (char *)recordCursor->name, (void *)rendBits);
                            }
                        }
                    } else {
                        entryCursor->kind = GFX_MODEL_ENTRY_RENDER_OBJECT;
                        if (recordCursor->name != 0) {
                            if ((recordCursor->flags & GFX_MODEL_RECORD_TRIANGLES) != 0) {
                                if ((recordCursor->flags & GFX_MODEL_RECORD_GOURAUD) != 0) {
                                    entryCursor->renderObject = (int)Gfx_CreateRenderObject(useState, GFX_DRAW_TEXTURED_GOURAUD_TRI_3D, optionsPtr, (char *)recordCursor->name, (void *)rendBits);
                                } else {
                                    entryCursor->renderObject = (int)Gfx_CreateRenderObject(useState, GFX_DRAW_TEXTURED_TRI_3D, optionsPtr, (char *)recordCursor->name, (void *)rendBits);
                                }
                            } else if ((recordCursor->flags & GFX_MODEL_RECORD_GOURAUD) != 0) {
                                entryCursor->renderObject = (int)Gfx_CreateRenderObject(useState, GFX_DRAW_TEXTURED_GOURAUD_QUAD_3D, optionsPtr, (char *)recordCursor->name, (void *)rendBits);
                            } else {
                                entryCursor->renderObject = (int)Gfx_CreateRenderObject(useState, GFX_DRAW_TEXTURED_QUAD_3D, optionsPtr, (char *)recordCursor->name, (void *)rendBits);
                            }
                        }
                    }
                    if ((recordCursor->flags & 0x200) != 0 && recordCursor->sptRecordId != 0) {
                        entryCursor->loadedRecord = Gfx_SptLoadRecord(recordCursor->sptRecordId, optionsPtr);
                    }
                } else {
                    entryCursor->kind = GFX_MODEL_ENTRY_MESH;
                    if (recordCursor->name != 0) {
                        entryCursor->renderObject = LoadRsdModel(optionsPtr, (const char *)recordCursor->name, ctx);
                    }
                    if (recordCursor->sptRecordId != 0 && (recordCursor->flags & 0x100) != 0) {
                        zero = 0;
                        if (optionsPtr != 0 && optionsPtr->directory != 0) {
                            PushFileContextDirectory((FileOpenRequest *)&optionsPtr->file, optionsPtr->directory, &pathBuffer);
                        }
                        entryCursor->loadedRecord = Gfx_AllocTexturedHundred(1, zero, recordCursor->sptRecordId, (int)optionsPtr, (int)ctx);
                        if (optionsPtr != 0 && optionsPtr->directory != 0) {
                            PushFileContextDirectory((FileOpenRequest *)&optionsPtr->file, &pathBuffer, 0);
                        }
                        obj = entryCursor->renderObject;
                        if (obj != 0 && *(int *)((char *)obj + 4) != 0) {
                            *(int *)((char *)*(int *)((char *)obj + 4) + 0xA0) = entryCursor->loadedRecord;
                        }
                    }
                }
                switch (recordCursor->flags & GFX_MODEL_RECORD_BLEND_MASK) {
                case BLEND_AVERAGE:
                case BLEND_ADDITIVE:
                case BLEND_SUBTRACTIVE:
                case BLEND_QUARTER_ADDITIVE:
                    entryCursor->kind += GFX_MODEL_ENTRY_TRANSLUCENT;
                }
                recordCursor++;
                entryCursor++;
            }
            failed = 0;
        }
    }
    if (failed != 0) {
        Gfx_FreeModelTable(pTable);
        pTable = 0;
    }
    g_GfxLastCreatedModelTable = (GfxModelTable *)pTable;
    return (int)pTable;
}

/*
 * Extracts field at offset 4 from indexed table entry.
 */
// FUNCTION: FF7 0x0068a115
int __cdecl Gfx_ModelTableGetRenderObject(int index, GfxTableCursor *cursor)
{
    int value;

    value = 0;
    if (cursor != 0 && cursor->entries != 0) {
        value = ((GfxTableEntry *)(cursor->entries))[index].fieldAt4;
        cursor->current = cursor->initial;
    }
    return value;
}

/*
 * Extracts field at offset 4 from indexed table entry alternative path.
 */
// FUNCTION: FF7 0x0068a155
int __cdecl Gfx_ModelTableGetSpriteObject(int index, GfxTableCursor *cursor)
{
    int value;

    value = 0;
    if (cursor != 0 && cursor->entries != 0) {
        value = ((GfxTableEntry *)(cursor->entries))[index].fieldAt4;
        cursor->current = cursor->initial;
    }
    return value;
}

/*
 * Extracts field at offset 8 from indexed table entry.
 */
// FUNCTION: FF7 0x0068a195
int __cdecl Gfx_ModelTableGetLoadedRecord(int index, GfxTableCursor *cursor)
{
    int value;

    value = 0;
    if (cursor != 0 && cursor->entries != 0) {
        value = ((GfxTableEntry *)(cursor->entries))[index].fieldAt8;
        cursor->current = cursor->initial;
    }
    return value;
}

/*
 * Renders a layer of model table entries or expires elapsed entries.
 */
// FUNCTION: FF7 0x0068a1d5
int __cdecl Gfx_ModelTableDrawLayerOrExpire(int advanceTick, int layer, GfxModelTable *table)
{
    GfxModelTableEntry *entryCursor;
    int entryIndex;
    int record;
    int res;
    int ticksElapsed;
    int expired;
    GfxEngineContext *engineContext;
    int recordBits;
    int doDraw;

    res = GFX_MODEL_TABLE_NONE;
    if (table != 0) {
        doDraw = 0;
        expired = 0;
        ticksElapsed = table->tick - table->tickStart;
        if (ticksElapsed > 5) {
            expired = 1;
            res = GFX_MODEL_TABLE_HIDDEN;
        }
        if (expired != 0) {
            /* Numeric render-group tag (0). */
            switch (table->groupTag) {
            case GFX_MODEL_GROUP_DEFAULT:
                if (ticksElapsed > table->lifetime + 5) {
                    res = GFX_MODEL_TABLE_EXPIRED;
                }
            }
        } else {
            doDraw = 1;
            res = GFX_MODEL_TABLE_DRAWN;
        }
        if (doDraw != 0) {
            engineContext = GetEngineContext();
            record = table->records;
            entryCursor = (GfxModelTableEntry *)table->entries;
            for (entryIndex = 0; entryIndex < table->count; entryIndex++) {
                recordBits = *(unsigned short *)record;
                switch (layer) {
                case GFX_MODEL_LAYER_OPAQUE:
                    switch (entryCursor->kind) {
                    case GFX_MODEL_ENTRY_MESH:
                        if ((recordBits & GFX_MODEL_RECORD_NO_CULL) != 0) {
                            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 1, engineContext);
                            Gfx_DrawMeshPolygonSet(entryCursor->renderObject, (int)engineContext);
                            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 0, engineContext);
                        } else {
                            Gfx_DrawMeshPolygonSet(entryCursor->renderObject, (int)engineContext);
                        }
                        break;
                    case GFX_MODEL_ENTRY_RENDER_OBJECT:
                        if ((recordBits & GFX_MODEL_RECORD_NO_CULL) != 0) {
                            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 1, engineContext);
                            Gfx_DrawRenderObjectCall(entryCursor->renderObj, engineContext);
                            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 0, engineContext);
                        } else {
                            Gfx_DrawRenderObjectCall(entryCursor->renderObj, engineContext);
                        }
                        break;
                    case GFX_MODEL_ENTRY_BATCH:
                        if ((recordBits & GFX_MODEL_RECORD_NO_CULL) != 0) {
                            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 1, engineContext);
                            Gfx_EmitPrimitiveBatchList((void *)entryCursor->renderObject, engineContext);
                            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 0, engineContext);
                        } else {
                            Gfx_EmitPrimitiveBatchList((void *)entryCursor->renderObject, engineContext);
                        }
                    }
                    break;
                case GFX_MODEL_LAYER_TRANSLUCENT:
                    switch (entryCursor->kind) {
                    case GFX_MODEL_ENTRY_MESH:
                        break;
                    case GFX_MODEL_ENTRY_TRANSLUCENT_MESH:
                        if ((recordBits & GFX_MODEL_RECORD_NO_CULL) != 0) {
                            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 1, engineContext);
                            Gfx_DrawMeshPolygonSet(entryCursor->renderObject, (int)engineContext);
                            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 0, engineContext);
                        } else {
                            Gfx_DrawMeshPolygonSet(entryCursor->renderObject, (int)engineContext);
                        }
                        break;
                    case GFX_MODEL_ENTRY_TRANSLUCENT_RENDER_OBJECT:
                        if ((recordBits & GFX_MODEL_RECORD_NO_CULL) != 0) {
                            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 1, engineContext);
                            Gfx_DrawRenderObjectCall(entryCursor->renderObj, engineContext);
                            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 0, engineContext);
                        } else {
                            Gfx_DrawRenderObjectCall(entryCursor->renderObj, engineContext);
                        }
                        break;
                    case GFX_MODEL_ENTRY_TRANSLUCENT_BATCH:
                        if ((recordBits & GFX_MODEL_RECORD_NO_CULL) != 0) {
                            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 1, engineContext);
                            Gfx_EmitPrimitiveBatchList((void *)entryCursor->renderObject, engineContext);
                            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 0, engineContext);
                        } else {
                            Gfx_EmitPrimitiveBatchList((void *)entryCursor->renderObject, engineContext);
                        }
                    }
                }
                entryCursor++;
                record += 0x14;
            }
        }
        if (advanceTick != 0) {
            (table->tick)++;
        }
    }
    return res;
}

/*
 * Frees all model tables registered within a model table bank.
 */
// FUNCTION: FF7 0x0068a567
void __cdecl Gfx_FreeModelTableBank(GfxModelTableBank *bank)
{
    int tablePointer;
    int i;

    if (bank != 0) {
        for (i = 0; i < bank->count; i++) {
            tablePointer = ((int *)(bank->table))[i];
            Gfx_FreeModelTable((GfxModelTable *)tablePointer);
            tablePointer += 0x24;
        }
        Gfx_DestroyPrimitiveMeshBank((struct GfxPrimitiveMeshBank *)bank->primitiveMeshBank);
        bank->primitiveMeshBank = 0;
        Gfx_DestroyDrawPool(bank->drawPool);
        bank->drawPool = 0;
        // STRING: FF7 0x009106d8
        GameHeapFree((void *)bank->slots, "C:\\lib\\src\\graphics\\render.cpp", 0x264);
        // STRING: FF7 0x009106f8
        GameHeapFree((void *)bank->table, "C:\\lib\\src\\graphics\\render.cpp", 0x265);
        // STRING: FF7 0x00910718
        GameHeapFree(bank, "C:\\lib\\src\\graphics\\render.cpp", 0x267);
    }
}

/*
 * Allocates and initializes a new model table bank container.
 */
// FUNCTION: FF7 0x0068a638
int __cdecl Gfx_CreateModelTableBank(int count, GfxModelTableDesc *desc)
{
    int failed;
    GfxModelTableBank *tableBank;

    // STRING: FF7 0x00910738
    tableBank = (GfxModelTableBank *)GameHeapAlloc(1, 0x2C, "C:\\lib\\src\\graphics\\render.cpp", 0x270);
    if (tableBank != 0) {
        failed = 0;
        tableBank->count = count;
        // STRING: FF7 0x00910758
        tableBank->slots = (int)GameHeapAlloc(count, 4, "C:\\lib\\src\\graphics\\render.cpp", 0x276);
        // STRING: FF7 0x00910778
        tableBank->table = (int)GameHeapAlloc(count, 4, "C:\\lib\\src\\graphics\\render.cpp", 0x277);
        tableBank->fileOpenMode = 0;
        if (tableBank->slots == 0) {
            failed = 1;
        }
        if (tableBank->table == 0) {
            failed = 1;
        }
        if (desc != 0) {
            if (desc->meshBankEnabled != 0) {
                tableBank->primitiveMeshBank = Gfx_CreatePrimitiveMeshBank(desc->recordCount, desc->vertexCount, desc->indexCount, desc->entryCount, (int)GetEngineContext());
            } else {
                failed = 1;
            }
            if (desc->drawPoolEnabled != 0) {
                tableBank->drawPool = Gfx_CreateDrawPool(desc->nodeCount);
            } else {
                failed = 1;
            }
            *(FileOpenRequest *)((int)tableBank + 0x1C) = *(FileOpenRequest *)((int)desc + 0x1C);
        }
        if (failed != 0) {
            Gfx_FreeModelTableBank(tableBank);
            tableBank = 0;
        }
    }
    return (int)tableBank;
}

/*
 * Adds a model table to the active model table bank.
 */
// FUNCTION: FF7 0x0068a78b
int __cdecl Gfx_ModelTableBankRegisterTable(GfxModelTable *table, GfxModelTableBank *bank)
{
    GfxModelTable *result;
    int slot;

    result = table;
    if (table != 0 && bank != 0) {
        if (bank->tableCount < bank->count) {
            if (bank->expiredCount > 0) {
                (bank->expiredCount)--;
                slot = ((int *)(bank->slots))[bank->expiredCount];
            } else {
                slot = bank->tableCount;
            }
            ((int *)(bank->table))[slot] = (int)table;
            (bank->tableCount)++;
        } else {
            Gfx_FreeModelTable(table);
            result = 0;
        }
    }
    return (int)result;
}

/*
 * Removes and frees a model table from the model table bank.
 */
// FUNCTION: FF7 0x0068a81c
void __cdecl Gfx_ModelTableBankRemoveTable(int index, GfxModelTable *table, GfxModelTableBank *bank)
{
    if (bank != 0) {
        Gfx_FreeModelTable(table);
        ((int *)(bank->table))[index] = 0;
        ((int *)(bank->slots))[bank->expiredCount] = index;
        (bank->expiredCount)++;
        (bank->tableCount)--;
    }
}

/*
 * Resets per-frame draw allocation pools across the model table bank.
 */
// FUNCTION: FF7 0x0068a871
void __cdecl Gfx_ModelTableBankResetFramePools(GfxModelTableBank *bank)
{
    if (bank != 0) {
        Gfx_ResetRenderObjectPool(bank->primitiveMeshBank);
        Gfx_AdvanceDrawPoolGeneration(bank->drawPool);
    }
}

/*
 * Draws a layer across bank tables and evicts expired model tables.
 */
// FUNCTION: FF7 0x0068a89a
void __cdecl Gfx_ModelTableBankDrawLayerAndRemoveExpired(int advance, int layer, GfxModelTableBank *bank)
{
    int result;
    int seen;
    int cnt;
    GfxModelTable *ent;
    int i;

    if (bank != 0) {
        seen = 0;
        cnt = bank->tableCount;
        for (i = 0; i < bank->count; i++) {
            ent = (GfxModelTable *)(((int *)(bank->table))[i]);
            if (ent != 0) {
                result = Gfx_ModelTableDrawLayerOrExpire(advance, layer, ent);
                switch (result) {
                case GFX_MODEL_TABLE_NONE:
                    break;
                case GFX_MODEL_TABLE_EXPIRED:
                    Gfx_ModelTableBankRemoveTable(i, ent, bank);
                }
                seen++;
            }
            if (seen >= cnt) {
                return;
            }
            ent++;
        }
    }
}

/*
 * Removes all model tables matching a specified group tag.
 */
// FUNCTION: FF7 0x0068a951
void __cdecl Gfx_ModelTableBankRemoveTablesByGroupTag(int groupTag, GfxModelTableBank *bank)
{
    int tablePointer;
    int i;

    if (bank != 0) {
        for (i = 0; i < bank->count; i++) {
            tablePointer = ((int *)(bank->table))[i];
            if (tablePointer != 0 && *(int *)((char *)tablePointer + 0x10) == groupTag) {
                Gfx_ModelTableBankRemoveTable(i, (GfxModelTable *)tablePointer, bank);
            }
            tablePointer += 0x24;
        }
    }
}

/*
 * Initializes model table creation descriptor with default settings.
 */
// FUNCTION: FF7 0x0068a9bd
void __cdecl Gfx_InitModelTableCreateDescriptor(GfxModelTableCreateDesc *desc)
{
    if (desc != 0) {
        ZeroFillMemory32(0x1C, desc);
        desc->color = 0xFF242424;
    }
}

/*
 * Initializes record header with default duration and state flags.
 */
// FUNCTION: FF7 0x0068a9e0
void __cdecl Gfx_InitResourceRequest(GfxResourceRequest *record)
{
    ZeroFillMemory32(0xC, record);
    record->groupTag = 0;
    record->duration = 0x5DC;
}

/*
 * Loads a model table from the records into the table slot when the slot is empty.
 */
// FUNCTION: FF7 0x0068aa06
void __cdecl Gfx_LoadModelTableIfAbsent(int flags, int records, GfxResourceRequest *request, void *tableSlot, GfxModelTableBank *bank)
{
    GfxModelTableCreateDesc *descriptor;
    GfxModelTable *table;
    int descriptorBuf[7];

    if (*(int *)tableSlot == 0 && bank != 0) {
        descriptor = (GfxModelTableCreateDesc *)descriptorBuf;
        Gfx_InitModelTableCreateDescriptor(descriptor);
        descriptor->renderBits = bank->primitiveMeshBank;
        descriptor->drawPool = bank->drawPool;
        *(FileOpenRequest *)((int)descriptor + 0xC) = *(FileOpenRequest *)((int)bank + 0x1C);
        table = (GfxModelTable *)(Gfx_LoadModelTableFromRecords(request->groupTag, request->duration, (int)request->resourcePath, flags, (GfxModelTableRecord *)records, (int)tableSlot, descriptor));
        *(int *)tableSlot = Gfx_ModelTableBankRegisterTable(table, bank);
    }
}
} /* extern "C" */
