// src/lib/src/graphics/dx_stat.cpp
// Original: C:\lib\src\graphics\dx_stat.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include <stdio.h>
#include <string.h>
extern "C" {

/*
 * Frees a graphics statistics record block.
 */
// FUNCTION: FF7 0x00685110
void __cdecl Gfx_StatsFreeRecord(int recordBlock)
{
    // STRING: FF7 0x0090f984
    GameHeapFree((void *)recordBlock, "C:\\lib\\src\\graphics\\dx_stat.cpp", 0xE);
}

/*
 * Allocates and initializes a new graphics statistics record.
 */
// FUNCTION: FF7 0x00685128
int __cdecl Gfx_StatsCreateRecord(void)
{
    GfxStatsRecord *rec;

    // STRING: FF7 0x0090f9a4
    rec = (GfxStatsRecord *)GameHeapAlloc(1, 0xFC, "C:\\lib\\src\\graphics\\dx_stat.cpp", 0x15);
    if (rec != 0) {
        rec->enabled = 1;
    }
    return (int)rec;
}
#pragma intrinsic(memset)

/*
 * Zeroes out a graphics statistics record block.
 */
// FUNCTION: FF7 0x0068515b
void __cdecl Gfx_StatsClearRecord(void *recordBlock)
{
    int savedWord;
    if (recordBlock != 0) {
        savedWord = ((GfxStatsRecord *)recordBlock)->enabled;
        memset(recordBlock, 0, 0xfc);
        ((GfxStatsRecord *)recordBlock)->enabled = savedWord;
    }
}

/*
 * Formats and renders diagnostic statistics text rows via callback.
 */
// FUNCTION: FF7 0x00685187
void __cdecl Gfx_StatsDrawTextRows(int context, int baseY, int callbackArg, void(__cdecl *arg4)(int, int, const char *, int), GfxStatsRecord *statsRecord)
{
    DebugFrame debugFrame;
    if (statsRecord != 0 && statsRecord->enabled != 0 && arg4 != 0) {
        debugFrame.timeF = CombineSplitValueToFloat(&statsRecord->timeF);
        debugFrame.timeM3 = CombineSplitValueToFloat(&statsRecord->timeM3);
        debugFrame.timeM43 = CombineSplitValueToFloat(&statsRecord->timeM43);
        debugFrame.timeM4 = CombineSplitValueToFloat(&statsRecord->timeM4);
        debugFrame.timeMr = CombineSplitValueToFloat(&statsRecord->timeMr);
        debugFrame.timeMs = CombineSplitValueToFloat(&statsRecord->timeMs);
        if (statsRecord->countM3 > 0)
            debugFrame.avgTimeM3 = debugFrame.timeM3 / statsRecord->countM3;
        else
            debugFrame.avgTimeM3 = 0.0;
        if (statsRecord->countM43 > 0)
            debugFrame.avgTimeM43 = debugFrame.timeM43 / statsRecord->countM43;
        else
            debugFrame.avgTimeM43 = 0.0;
        if (statsRecord->countM4 > 0)
            debugFrame.avgTimeM4 = debugFrame.timeM4 / statsRecord->countM4;
        else
            debugFrame.avgTimeM4 = 0.0;
        if (statsRecord->countMr > 0)
            debugFrame.avgTimeMr = debugFrame.timeMr / statsRecord->countMr;
        else
            debugFrame.avgTimeMr = 0.0;
        if (statsRecord->countMs > 0)
            debugFrame.avgTimeMs = debugFrame.timeMs / statsRecord->countMs;
        else
            debugFrame.avgTimeMs = 0.0;
        sprintf(debugFrame.text,
                // STRING: FF7 0x0090f9c4
                "G %d P %d V %d M3 %d %.0f M43 %d %.0f M4 %d %.0f MR %d %.0f MS %d %.0f S2 %d S3 %d     ", statsRecord->countG, statsRecord->countP, statsRecord->countV, statsRecord->countM3, debugFrame.avgTimeM3, statsRecord->countM43, debugFrame.avgTimeM43, statsRecord->countM4, debugFrame.avgTimeM4, statsRecord->countMr, debugFrame.avgTimeMr, statsRecord->countMs, debugFrame.avgTimeMs, statsRecord->countS2, statsRecord->countS3);
        arg4(context, baseY, debugFrame.text, callbackArg);
        debugFrame.avgTimeE = 0.0;
        if (statsRecord->countP > 0)
            debugFrame.avgTimeE = CombineSplitValueToFloat(&statsRecord->timeE) / statsRecord->countP;
        if (statsRecord->timeA.hi < 0)
            debugFrame.timeA = 0.0;
        else
            debugFrame.timeA = CombineSplitValueToFloat(&statsRecord->timeA);
        sprintf(debugFrame.text,
                // STRING: FF7 0x0090fa1c
                "C %.0f E %.0f RL %.0f PA %.0f P %.0f F %.0f A %.0f GDI %.0f     ", CombineSplitValueToFloat(&statsRecord->timeC), CombineSplitValueToFloat(&statsRecord->timeE), CombineSplitValueToFloat(&statsRecord->timeRl), debugFrame.avgTimeE, CombineSplitValueToFloat(&statsRecord->timeP), debugFrame.timeF, debugFrame.timeA, CombineSplitValueToFloat(&statsRecord->timeGdi));
        arg4(context, baseY + 20, debugFrame.text, callbackArg);
        if (debugFrame.timeF > 0.0) {
            debugFrame.timeGdi = CombineSplitValueToFloat(&statsRecord->timeGdi);
            sprintf(debugFrame.text,
                    // STRING: FF7 0x0090fa60
                    "C %.2f RL %.2f A %.2f GDI %.2f C %.2f RL %.2f A %.2f M3 %.2f M43 %.2f MR %.2f MS %.2f     ", CombineSplitValueToFloat(&statsRecord->timeC) / debugFrame.timeF, CombineSplitValueToFloat(&statsRecord->timeRl) / debugFrame.timeF, debugFrame.timeA / debugFrame.timeF, debugFrame.timeGdi / debugFrame.timeF, CombineSplitValueToFloat(&statsRecord->timeC) / (debugFrame.timeF - debugFrame.timeGdi), CombineSplitValueToFloat(&statsRecord->timeRl) / (debugFrame.timeF - debugFrame.timeGdi), debugFrame.timeA / (debugFrame.timeF - debugFrame.timeGdi), debugFrame.timeM3 / (debugFrame.timeF - debugFrame.timeGdi), debugFrame.timeM43 / (debugFrame.timeF - debugFrame.timeGdi), debugFrame.timeMr / (debugFrame.timeF - debugFrame.timeGdi), debugFrame.timeMs / (debugFrame.timeF - debugFrame.timeGdi));
            arg4(context, baseY + 40, debugFrame.text, callbackArg);
        }
        debugFrame.timeLvc = CombineSplitValueToFloat(&statsRecord->timeLvc);
        if (statsRecord->level > 0)
            debugFrame.avgTimeLvc = debugFrame.timeLvc / statsRecord->level;
        else
            debugFrame.avgTimeLvc = 0.0;
        debugFrame.timeSst = CombineSplitValueToFloat(&statsRecord->timeSst);
        debugFrame.timeSut = CombineSplitValueToFloat(&statsRecord->timeSut);
        if (statsRecord->countTss > 0)
            debugFrame.avgTimeSst = debugFrame.timeSst / statsRecord->countTss;
        else
            debugFrame.avgTimeSst = 0.0;
        if (statsRecord->countTsu > 0)
            debugFrame.avgTimeSut = debugFrame.timeSut / statsRecord->countTsu;
        else
            debugFrame.avgTimeSut = 0.0;
        sprintf(debugFrame.text,
                // STRING: FF7 0x0090fabc
                "LV %d LVC %.0f LVA %.0f  TSS %03d SSA %.0f SST %.0f TSU %03d SUA %.0f SUT %.0f PC %03d PM %03d     ", statsRecord->level, debugFrame.timeLvc, debugFrame.avgTimeLvc, statsRecord->countTss, debugFrame.avgTimeSst, debugFrame.timeSst, statsRecord->countTsu, debugFrame.avgTimeSut, debugFrame.timeSut, statsRecord->countPc, statsRecord->countPm);
        arg4(context, baseY + 60, debugFrame.text, callbackArg);
        debugFrame.culledPolyCount = statsRecord->wcCount + statsRecord->bfCount + statsRecord->wcCountAlt + statsRecord->hcCount;
        debugFrame.timeVt = CombineSplitValueToFloat(&statsRecord->timeVt);
        debugFrame.timeTs = CombineSplitValueToFloat(&statsRecord->timeTs);
        debugFrame.timeTd = CombineSplitValueToFloat(&statsRecord->timeTd);
        debugFrame.timeTc = CombineSplitValueToFloat(&statsRecord->timeTc);
        debugFrame.swTotalTime = debugFrame.timeVt + debugFrame.timeTs + debugFrame.timeTd + debugFrame.timeTc;
    rounded_sum:
        if (debugFrame.swTotalTime <= 0.0f)
            debugFrame.swTotalTime = 1.0f;
        sprintf(debugFrame.text,
                // STRING: FF7 0x0090fb20
                "WC %d BF %d WC %d HC %d TC %d DR %d   SWT %.0f  D %.0f     ", statsRecord->wcCount, statsRecord->bfCount, statsRecord->wcCountAlt, statsRecord->hcCount, debugFrame.culledPolyCount, statsRecord->countP - debugFrame.culledPolyCount, debugFrame.swTotalTime, CombineSplitValueToFloat(&statsRecord->timeD));
        arg4(context, baseY + 80, debugFrame.text, callbackArg);
        debugFrame.avgTimeTd = 0.0;
        if (statsRecord->rasterFrameCount > 0)
            debugFrame.avgTimeTd = CombineSplitValueToFloat(&statsRecord->timeTd) / statsRecord->rasterFrameCount;
        debugFrame.avgTimeVt = 0.0;
        if (statsRecord->countV > 0)
            debugFrame.avgTimeVt = CombineSplitValueToFloat(&statsRecord->timeVt) / statsRecord->countV;
        sprintf(debugFrame.text,
                // STRING: FF7 0x0090fb5c
                "VT %.0f VA %.1f TS %.0f TD %.0f TC %.0f TP %d AP %.1f   TP %.1f SP %.1f DP %.1f CP %.1f   ", debugFrame.timeVt * 0.001f, debugFrame.avgTimeVt, debugFrame.timeTs * 0.001f, debugFrame.timeTd * 0.001f, debugFrame.timeTc * 0.001f, statsRecord->rasterFrameCount, debugFrame.avgTimeTd, debugFrame.timeVt / debugFrame.swTotalTime * 100.0f, debugFrame.timeTs / debugFrame.swTotalTime * 100.0f, debugFrame.timeTd / debugFrame.swTotalTime * 100.0f, debugFrame.timeTc / debugFrame.swTotalTime * 100.0f);
        arg4(context, baseY + 100, debugFrame.text, callbackArg);
    }
}

/*
 * Increments the M3 model draw count statistic.
 */
// FUNCTION: FF7 0x00685ac4
void __cdecl Gfx_StatsBumpM3Count(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (ctx != 0 && ctx->statsRecord != 0) {
        (ctx->statsRecord->countM3)++;
    }
}

/*
 * Increments the M43 model draw count statistic.
 */
// FUNCTION: FF7 0x00685b01
void __cdecl Gfx_StatsBumpM43Count(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (ctx != 0 && ctx->statsRecord != 0) {
        (ctx->statsRecord->countM43)++;
    }
}

/*
 * Increments the M4 model draw count statistic.
 */
// FUNCTION: FF7 0x00685b3e
void __cdecl Gfx_StatsBumpM4Count(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (ctx != 0 && ctx->statsRecord != 0) {
        (ctx->statsRecord->countM4)++;
    }
}

/*
 * Increments the MR model draw count statistic.
 */
// FUNCTION: FF7 0x00685b7b
void __cdecl Gfx_StatsBumpMrCount(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (ctx != 0 && ctx->statsRecord != 0) {
        (ctx->statsRecord->countMr)++;
    }
}

/*
 * Increments the MS model draw count statistic.
 */
// FUNCTION: FF7 0x00685bb8
void __cdecl Gfx_StatsBumpMsCount(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (ctx != 0 && ctx->statsRecord != 0) {
        (ctx->statsRecord->countMs)++;
    }
}

/*
 * Increments the S2 sprite draw count statistic.
 */
// FUNCTION: FF7 0x00685bf5
void __cdecl Gfx_StatsBumpS2Count(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (ctx != 0 && ctx->statsRecord != 0) {
        (ctx->statsRecord->countS2)++;
    }
}

/*
 * Increments the S3 sprite draw count statistic.
 */
// FUNCTION: FF7 0x00685c32
void __cdecl Gfx_StatsBumpS3Count(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (ctx != 0 && ctx->statsRecord != 0) {
        (ctx->statsRecord->countS3)++;
    }
}

/*
 * Accumulates execution time for M3 model rendering.
 */
// FUNCTION: FF7 0x00685c6f
void __cdecl Gfx_StatsAddM3Time(void *accumulator)
{
    GfxEngineContext *engineContext;

    engineContext = GetEngineContext();
    if (engineContext != 0 && engineContext->statsRecord != 0) {
        Add64((struct Timestamp64 *)accumulator, &engineContext->statsRecord->timeM3);
    }
}

/*
 * Accumulates execution time for M43 model rendering.
 */
// FUNCTION: FF7 0x00685caa
void __cdecl Gfx_StatsAddM43Time(void *accumulator)
{
    GfxEngineContext *engineContext;

    engineContext = GetEngineContext();
    if (engineContext != 0 && engineContext->statsRecord != 0) {
        Add64((struct Timestamp64 *)accumulator, &engineContext->statsRecord->timeM43);
    }
}

/*
 * Accumulates execution time for M4 model rendering.
 */
// FUNCTION: FF7 0x00685ce5
void __cdecl Gfx_StatsAddM4Time(void *accumulator)
{
    GfxEngineContext *engineContext;

    engineContext = GetEngineContext();
    if (engineContext != 0 && engineContext->statsRecord != 0) {
        Add64((struct Timestamp64 *)accumulator, &engineContext->statsRecord->timeM4);
    }
}

/*
 * Accumulates execution time for MR model rendering.
 */
// FUNCTION: FF7 0x00685d20
void __cdecl Gfx_StatsAddMrTime(void *accumulator)
{
    GfxEngineContext *engineContext;

    engineContext = GetEngineContext();
    if (engineContext != 0 && engineContext->statsRecord != 0) {
        Add64((struct Timestamp64 *)accumulator, &engineContext->statsRecord->timeMr);
    }
}

/*
 * Accumulates execution time for MS model rendering.
 */
// FUNCTION: FF7 0x00685d5b
void __cdecl Gfx_StatsAddMsTime(void *accumulator)
{
    GfxEngineContext *engineContext;

    engineContext = GetEngineContext();
    if (engineContext != 0 && engineContext->statsRecord != 0) {
        Add64((struct Timestamp64 *)accumulator, &engineContext->statsRecord->timeMs);
    }
}
} /* extern "C" */
