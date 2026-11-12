/* Private U50 inline renderer; include after this TU's provider/global declarations.
 * Keeping the helper here preserves VC5's 16-bit source-line lookup in the large TU.
 * Original source partition and helper names are not preserved by retail debug paths.
 */
#ifndef FF7_NANNTOKA_RENDER_H
#define FF7_NANNTOKA_RENDER_H

typedef struct ParticlePoint8 {
    short x, y, z, age;
} ParticlePoint8;
typedef struct Particle20 {
    ParticlePoint8 point;
    short vx, vy, vz, step;
    int color;
} Particle20;
typedef GfxHomogeneousVec4 Vector16;

static __inline void NanntokaRenderPool(int arg1, int arg2, int *index, int r, int *step, Vector16 *p)
{
    Vector16 t;
    float y;
    float z;
    for (*index = 0; *index < 300; (*index)++) {
        if (*(short *)((char *)&g_BattleBluePandorasBoxFragmentAge + (*index) * 0x14) == 0) {
            continue;
        }
        if (*(short *)((char *)&g_BattleBluePandorasBoxFragmentAge + (*index) * 0x14) < 10) {
            if (g_BattleIsPaused == 0) {
                *(short *)((char *)&g_BattleBluePandorasBoxFragmentRecords + (*index) * 0x14) += *(short *)((char *)((char *)&g_BattleBluePandorasBoxFragmentAge + 2) + (*index) * 0x14);
                *(short *)((char *)&g_BattleBluePandorasBoxFragmentPosY + (*index) * 0x14) += *(short *)((char *)&g_BattleBluePandorasBoxFragmentVelocityY + (*index) * 0x14);
                *(short *)((char *)&g_BattleBluePandorasBoxFragmentPosZ + (*index) * 0x14) += *(short *)((char *)&g_BattleBluePandorasBoxFragmentVelocityZ + (*index) * 0x14);
                (*(short *)((char *)&g_BattleBluePandorasBoxFragmentAge + (*index) * 0x14))++;
            }
            continue;
        }
        *(ParticlePoint8 *)((char *)arg1 + 0x10) = ((Particle20 *)&g_BattleBluePandorasBoxFragmentRecords)[(*index)].point;
        *(int *)((char *)arg1 + 0x220) = *(short *)((char *)arg1 + 0x28) - *(short *)((char *)arg1 + 0x10);
        *(int *)((char *)arg1 + 0x224) = *(short *)((char *)arg1 + 0x2A) - *(short *)((char *)arg1 + 0x12);
        *(int *)((char *)arg1 + 0x228) = *(short *)((char *)arg1 + 0x2C) - *(short *)((char *)arg1 + 0x14);
        Gfx_SetPendingVertexInput((void *)(arg1 + 0x10));
        Gfx_TransformPendingVertexSlot0();
        Gfx_ReadVertexSlotVector(0, p);
        (*step) = *(short *)((char *)&g_BattleBluePandorasBoxFragmentStep + (*index) * 0x14);
        *(short *)((char *)arg1 + 0x18) = *(short *)((char *)arg1 + 0x10) + ((*step) * *(int *)((char *)arg1 + 0x220) >> 12);
        *(short *)((char *)arg1 + 0x1A) = *(short *)((char *)arg1 + 0x12) + ((*step) * *(int *)((char *)arg1 + 0x224) >> 12);
        *(short *)((char *)arg1 + 0x1C) = *(short *)((char *)arg1 + 0x14) + ((*step) * *(int *)((char *)arg1 + 0x228) >> 12);
        Gfx_PsxStoreClearFlag((int *)(arg1 + 0xC));
        if (p->w > 0.0f) {
            z = p->x / p->w;
            y = p->y / p->w;
            if (z < -8e+01f || (z > 7.2e+02f || (y < -8e+01f || (y > 4e+02f || (*(int *)((char *)arg1 + 0xC) & 0x60000) != 0)))) {
                *(short *)((char *)&g_BattleBluePandorasBoxFragmentAge + (*index) * 0x14) = 0;
                continue;
            }
        } else {
            *(short *)((char *)&g_BattleBluePandorasBoxFragmentAge + (*index) * 0x14) = 0;
            continue;
        }
        Gfx_SetPendingVertexInput((void *)(arg1 + 0x18));
        Gfx_TransformPendingVertexSlot0();
        Gfx_ReadVertexSlotVector(0, &t);
        *(int *)((char *)r + 4) = *(int *)((char *)&g_BattleBluePandorasBoxFragmentColor + (*index) * 0x14);
        *(int *)((char *)r + 0xC) = 0x10304;
        if (g_BattleIsPaused == 0) {
            *(short *)((char *)&g_BattleBluePandorasBoxFragmentRecords + (*index) * 0x14) += *(short *)((char *)((char *)&g_BattleBluePandorasBoxFragmentAge + 2) + (*index) * 0x14);
            *(short *)((char *)&g_BattleBluePandorasBoxFragmentPosY + (*index) * 0x14) += *(short *)((char *)&g_BattleBluePandorasBoxFragmentVelocityY + (*index) * 0x14);
            *(short *)((char *)&g_BattleBluePandorasBoxFragmentPosZ + (*index) * 0x14) += *(short *)((char *)&g_BattleBluePandorasBoxFragmentVelocityZ + (*index) * 0x14);
            *(short *)((char *)&g_BattleBluePandorasBoxFragmentVelocityZ + (*index) * 0x14) += (*(short *)((char *)&g_BattleBluePandorasBoxFragmentVelocityZ + (*index) * 0x14) >> 2);
            *(short *)((char *)&g_BattleBluePandorasBoxFragmentVelocityZ + (*index) * 0x14) -= (short)(arg2 >> 1);
            (*step) = (*step) + (((*step) >> 5) + (arg2 >> 1));
        }
        *(short *)((char *)&g_BattleBluePandorasBoxFragmentStep + (*index) * 0x14) = (*step);
        *(int *)((char *)r) = *(int *)((char *)r) & 0xFFFFFF | 0x4000000;
        *(char *)((char *)r + 7) = 0x52;
        Gfx_SetVertexSlotClipRect(0, p);
        Gfx_SetVertexSlotClipRect(1, &t);
        Gfx_DrawPsxPrimitive((void *)r);
    }
}
#endif
