// Contents: Flushes the snowboard's queued draw records through the shared draw path, after setting
//   the white draw colour, and submits single model records to the active model object.
#include "ff7/snobo_primitives.h"
#include "ff7/api/snobo.h"
#include "lib/api/graphics.h"
#include "ff7/types/snobo.h"
extern "C" {
// clang-format off
// GLOBAL: FF7 0x00dd8708
struct SnowboardMatrixRecord g_SnoboMatrixRecords[128] = {0};
// GLOBAL: FF7 0x00ddad08
short g_SnoboMatrixRecordCount = 0;
// clang-format on
}
#pragma code_seg(".text")

/*
 * Sets white draw color and processes the queued matrix records through the shared
 * state block. Copies the full matrix for kind zero or only the translation for kind one, submits
 * the model record, then emits campaign batches and clears the queue count. Returns result; unused is
 * unused.
 */
// FUNCTION: FF7 0x00735220
extern "C" int Mini_SnowboardFlushMatrixRecords(int mode, int unused, int result)
{
    struct {
        SnowboardMatrixRecord *record;
        int count;
        SnowboardModelRecord *state;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    local.state = (SnowboardModelRecord *)Gfx_GetPsxScratchpadPtr(0);
    SetDrawColorRGB(255, 255, 255);
    local.state->mode = mode;
    local.count = g_SnoboMatrixRecordCount;
    for (local.record = g_SnoboMatrixRecords; local.count--; local.record++) {
        local.state->modelId = local.record->actionCode;
        local.state->animFrame = local.record->animFrame;
        local.state->drawFlags = local.record->drawFlags;
        if (local.record->kind == 0) {
            local.state->matrix = local.record->matrix;
        } else if (local.record->kind == 1) {
            local.state->matrix.v[12] = local.record->matrix.v[12];
            local.state->matrix.v[13] = local.record->matrix.v[13];
            local.state->matrix.v[14] = local.record->matrix.v[14];
        }
        Gfx_SubmitModelRecord(local.state);
    }
    ((SnowboardTmdRenderer *)*(int *)g_SnoboStatePtr)->emit();
    g_SnoboMatrixRecordCount = 0;
    return result;
}

/*
 * Calls the model method on the object held by g_SnoboStatePtr, passing the record's matrix, model id,
 * animation frame and draw flags.
 */
// FUNCTION: FF7 0x00735332
extern "C" void Gfx_SubmitModelRecord(SnowboardModelRecord *record)
{
    ((SnowboardTmdRenderer *)*(int *)g_SnoboStatePtr)->model((int)&record->matrix, record->modelId, record->animFrame, record->drawFlags);
}
#pragma code_seg()
