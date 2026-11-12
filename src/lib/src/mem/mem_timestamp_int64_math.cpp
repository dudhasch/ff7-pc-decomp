// src/lib/src/mem/mem_timestamp_int64_math.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: 64-bit arithmetic on split dword values, the timestamp counter read, and the timestamp
//   frequency calibration.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/mem.h"
#include <string.h>
#include "lib/api/mem.h"
extern "C" {

#pragma intrinsic(strlen)
__declspec(dllimport) unsigned int __stdcall timeGetTime(void);

/*
 * Reads the CPU timestamp counter (RDTSC, emitted as raw bytes) into value.
 */
// FUNCTION: FF7 0x00660370
void __cdecl ReadTimestampCounter(Timestamp64 *value)
{
    __asm {
        mov ebx, value
        _emit 0x0f
        _emit 0x31
        mov dword ptr [ebx], eax
        mov dword ptr [ebx + 4], edx
    }
}

/*
 * Adds addend into accumulator as a 64-bit sum: lo first, then hi with carry.
 */
// FUNCTION: FF7 0x00660385
void __cdecl Add64(Timestamp64 *addend, Timestamp64 *accumulator)
{
    __asm {
        mov ebx, addend
        mov ecx, accumulator
        mov eax, dword ptr [ebx]
        mov edx, dword ptr [ebx + 4]
        add dword ptr [ecx], eax
        adc dword ptr [ecx + 4], edx
    }
}

/*
 * Subtracts right from left as 64-bit values and stores the difference in result.
 */
// FUNCTION: FF7 0x006603a0
void __cdecl Sub64(Timestamp64 *left, Timestamp64 *right, Timestamp64 *result)
{
    __asm {
        mov ebx, left
        mov ecx, result
        mov eax, dword ptr [ebx]
        mov edx, dword ptr [ebx + 4]
        mov ebx, right
        sub eax, dword ptr [ebx]
        sbb edx, dword ptr [ebx + 4]
        mov dword ptr [ecx], eax
        mov dword ptr [ecx + 4], edx
    }
}

/*
 * Orders two 64-bit keys: by the signed hi first, then by the unsigned lo. Returns 1, 0 or -1
 * in the strcmp convention.
 */
// FUNCTION: FF7 0x006603c3
int __cdecl Compare64(Timestamp64 *keyA, Timestamp64 *keyB)
{
    int result;

    if (keyA->hi == keyB->hi) {
        if (keyA->lo == keyB->lo) {
            result = 0;
        } else if (keyA->lo > keyB->lo) {
            result = 1;
        } else {
            result = 0xFFFFFFFF;
        }
    } else if (keyA->hi > keyB->hi) {
        result = 1;
    } else {
        result = 0xFFFFFFFF;
    }
    return result;
}

/*
 * Returns the signed high dword times 4294967296.0 plus the unsigned low dword as a double.
 */
// FUNCTION: FF7 0x0066042d
double __cdecl CombineSplitValueToFloat(Timestamp64 *value)
{
    return value->hi * 4294967296.0 + value->lo;
}

/*
 * Stores value/4294967296.0 converted to int in parts->hi, then stores the residual after
 * subtracting that high contribution converted to int in parts->lo.
 */
// FUNCTION: FF7 0x00660457
void __cdecl SplitFloatToSplitValue(double value, Timestamp64 *parts)
{
    parts->hi = (int)(value / 4294967296.0);
    parts->lo = (int)(value - (double)parts->hi * 4294967296.0);
}

/*
 * Waits until timeGetTime is sufficiently far from wrap, waits for a tick change, reads a
 * timestamp, busy-waits until more than 1000 milliseconds have elapsed, reads a second timestamp,
 * writes their two-dword difference through result and returns its double conversion. It returns
 * the measured count, not a count divided by actual elapsed time.
 */
// FUNCTION: FF7 0x00660489
double __cdecl Timer_CalibrateTscFrequency(Timestamp64 *result)
{
    unsigned long time;
    Timestamp64 endStamp;
    double freq;
    unsigned int divisor;
    unsigned int threshold;
    Timestamp64 startCnt;

    threshold = 1000;
    divisor = 1;
    if (divisor > 0) {
        threshold /= divisor;
    }
    time = timeGetTime();
    while (0xffffffff - time <= threshold * 2) {
        time = timeGetTime();
    }
    time = timeGetTime();
    while (time == timeGetTime()) {
    }
    time = timeGetTime();
    ReadTimestampCounter(&startCnt);
    while (timeGetTime() - time <= threshold) {
    }
    ReadTimestampCounter(&endStamp);
    Sub64(&endStamp, &startCnt, result);
    freq = CombineSplitValueToFloat(result);
    return freq;
}
} /* extern "C" */
