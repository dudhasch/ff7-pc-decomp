// src/ff7/src/battle/battle3d/amptoanm.cpp
// Original: C:\FF7\src\battle\battle3d\amptoanm.cpp
// Contents: Battle animation decoding: reads compressed animation frames bit by bit into bone angle
//   and position deltas, loads animation banks and single animations from disk, and holds debug
//   converters and dump helpers for animation files.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/api/file.h"
#include "lib/api/polygon.h"
#include "lib/enums/file.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "lib/types/polygon.h"
extern "C" {

#pragma intrinsic(memset)
struct PolygonAnimBuffers;

/*
 * Decodes a compressed battle animation frame using the supplied bit cursor.
 */
// FUNCTION: FF7 0x005e7680
int __cdecl BattleModel_DecodeAnimFrame(Battle3dAnimAngles *animFrame, int boneCount, int startBit, BattleModelAnimBank *frameHeader)
{
    float fB;
    BattleModelAnimBank *stream;
    int angleX;
    int shift;
    int count;
    int angleY;
    int angleZ;
    float floatA;
    unsigned short activeByteLimit;
    int bitIndex;
    int index;
    float floatC;

    bitIndex = startBit;
    count = boneCount;
    stream = frameHeader;
    activeByteLimit = stream->byteLimit;
    shift = stream->shift;
    stream = (BattleModelAnimBank *)((char *)stream + 5);
    if (bitIndex == 0) {
        animFrame->accumA = BattleModel_ReadSignedBits(stream, &bitIndex, 0x10);
        animFrame->accumB = BattleModel_ReadSignedBits(stream, &bitIndex, 0x10);
        animFrame->accumC = BattleModel_ReadSignedBits(stream, &bitIndex, 0x10);
        floatA = (float)animFrame->accumA / 1.0f;
        fB = (float)animFrame->accumB / 1.0f;
        floatC = (float)animFrame->accumC / 1.0f;
        animFrame->decodedA = floatA;
        animFrame->decodedB = fB;
        animFrame->decodedC = floatC;
        for (index = 0; index < count; index++) {
            animFrame[index].deltaX = BattleModel_ReadSignedBits(stream, &bitIndex, 0xC - shift) << shift;
            animFrame[index].deltaY = BattleModel_ReadSignedBits(stream, &bitIndex, 0xC - shift) << shift;
            animFrame[index].deltaZ = BattleModel_ReadSignedBits(stream, &bitIndex, 0xC - shift) << shift;
            if (animFrame[index].deltaX < 0) {
                angleX = animFrame[index].deltaX + 0x1000;
            } else {
                angleX = animFrame[index].deltaX;
            }
            if (animFrame[index].deltaY < 0) {
                angleY = animFrame[index].deltaY + 0x1000;
            } else {
                angleY = animFrame[index].deltaY;
            }
            if (animFrame[index].deltaZ < 0) {
                angleZ = animFrame[index].deltaZ + 0x1000;
            } else {
                angleZ = animFrame[index].deltaZ;
            }
            animFrame[index].angleX = angleX;
            animFrame[index].angleY = angleY;
            animFrame[index].angleZ = angleZ;
        }
    } else {
        animFrame->accumA += (short)BattleModel_ReadSignedDeltaFromBitstream(stream, &bitIndex);
        animFrame->accumB += (short)BattleModel_ReadSignedDeltaFromBitstream(stream, &bitIndex);
        animFrame->accumC += (short)BattleModel_ReadSignedDeltaFromBitstream(stream, &bitIndex);
        floatA = (float)animFrame->accumA / 1.0f;
        fB = (float)animFrame->accumB / 1.0f;
        floatC = (float)animFrame->accumC / 1.0f;
        animFrame->decodedA = floatA;
        animFrame->decodedB = fB;
        animFrame->decodedC = floatC;
        for (index = 0; index < count; index++) {
            animFrame[index].deltaX += (short)BattleModel_ReadAngleDeltaFromBitstream(stream, &bitIndex, shift);
            animFrame[index].deltaY += (short)BattleModel_ReadAngleDeltaFromBitstream(stream, &bitIndex, shift);
            animFrame[index].deltaZ += (short)BattleModel_ReadAngleDeltaFromBitstream(stream, &bitIndex, shift);
            if (animFrame[index].deltaX < 0) {
                angleX = animFrame[index].deltaX + 0x1000;
            } else {
                angleX = animFrame[index].deltaX;
            }
            if (animFrame[index].deltaY < 0) {
                angleY = animFrame[index].deltaY + 0x1000;
            } else {
                angleY = animFrame[index].deltaY;
            }
            if (animFrame[index].deltaZ < 0) {
                angleZ = animFrame[index].deltaZ + 0x1000;
            } else {
                angleZ = animFrame[index].deltaZ;
            }
            animFrame[index].angleX = angleX;
            animFrame[index].angleY = angleY;
            animFrame[index].angleZ = angleZ;
        }
    }
    if (((bitIndex + 7) / 8 & 0xFFFF) >= activeByteLimit) {
        return 0;
    } else {
        return bitIndex;
    }
}

/*
 * Reads a variable-width signed delta value from the bitstream.
 */
// FUNCTION: FF7 0x005e7b7b
short __cdecl BattleModel_ReadSignedDeltaFromBitstream(BattleModelAnimBank *stream, int *bitPos)
{
    short value;
    int bitIdx;
    int byteIndex;
    int bits;
    int flagsLeft;

    byteIndex = *bitPos / 8;
    bitIdx = *bitPos & 7;
    flagsLeft = 7 - bitIdx;
    bits = *((unsigned char *)stream + byteIndex) << 8 | *((unsigned char *)stream + byteIndex + 1);
    if ((bits & 1 << flagsLeft + 8) == 0) {
        value = (short)(bits << bitIdx + 1) >> 9;
        *bitPos += 8;
        return (short)value;
    } else {
        bits = bits << 8 | *((unsigned char *)stream + byteIndex + 2);
        value = (unsigned int)(bits << bitIdx + 1) >> 8;
        *bitPos += 0x11;
        return (short)value;
    }
}

/*
 * Reads the specified number of bits MSB-first from byte buffer with sign extension.
 */
// FUNCTION: FF7 0x005e7c40
int __cdecl BattleModel_ReadSignedBits(BattleModelAnimBank *stream, int *bitPos, int count)
{
    int value;
    int i;

    value = 0;
    for (i = 0; i < count; i++) {
        value <<= 1;
        if ((*((unsigned char *)stream + *bitPos / 8) & 1 << 7 - (*bitPos & 7)) != 0) {
            value++;
        }
        (*bitPos)++;
    }
    value <<= 0x20 - count;
    value >>= 0x20 - count;
    return value;
}

/*
 * Reads an optional angle delta from the bitstream according to selector encoding.
 */
// FUNCTION: FF7 0x005e7ce4
short __cdecl BattleModel_ReadAngleDeltaFromBitstream(BattleModelAnimBank *stream, int *bitPos, int shift)
{
    int value;
    int selector;

    if (BattleModel_ReadSignedBits(stream, bitPos, 1) == 0) {
        return 0;
    } else {
        selector = BattleModel_ReadSignedBits(stream, bitPos, 3) & 7;
        /* Numeric bitstream width code (0..7; 1..6 equal bit width). */
        switch (selector) {
        case 0:
            return -1 << shift;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            value = BattleModel_ReadSignedBits(stream, bitPos, selector);
            if (value >= 0) {
                value += 1 << selector - 1;
            } else {
                value -= 1 << selector - 1;
            }
            return value << shift;
        case 7:
            value = BattleModel_ReadSignedBits(stream, bitPos, 0xC - shift);
            return value << shift;
        default:
            return 0;
        }
    }
}

/*
 * Creates an animation and repeatedly decodes frames into the scratch buffer.
 */
// FUNCTION: FF7 0x005e7de4
int __cdecl BattleModel_DecodeAnimation(int mirror, int boneCount, int frameCount, BattleModelAnimBank *packedData)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[2068];
        float boneVecX[3];
        struct { unsigned char pad_to_4[0x4]; float boneVecY; };
        struct { unsigned char pad_to_8[0x8]; float boneVecZ; };
        struct { unsigned char pad_to_C[0xC]; int boneIndex; };
        struct { unsigned char pad_to_10[0x10]; float tmpFloat; };
        struct { unsigned char pad_to_14[0x14]; float rootAngleX[6]; };
        struct { unsigned char pad_to_18[0x18]; float rootAngleY; };
        struct { unsigned char pad_to_1C[0x1C]; float rootAngleZ; };
        struct { unsigned char pad_to_20[0x20]; int frameCode; };
        struct { unsigned char pad_to_24[0x24]; float mirrorRootX; };
        struct { unsigned char pad_to_28[0x28]; int rootAlt; };
        struct { unsigned char pad_to_2C[0x2C]; int animFrame; };
        struct { unsigned char pad_to_30[0x30]; int createFlag; };
        struct { unsigned char pad_to_31[0x31]; char flagByteA; };
        struct { unsigned char pad_to_32[0x32]; char flagByteB; };
        struct { unsigned char pad_to_34[0x34]; unsigned char scratchBuffer[2000]; };
        struct { unsigned char pad_to_44[0x44]; int rawRootX; };
        struct { unsigned char pad_to_48[0x48]; int rawRootY; };
        struct { unsigned char pad_to_4C[0x4C]; int rawRootZ; };
        struct { unsigned char pad_to_50[0x50]; int decodedFrameCode; };
        struct { unsigned char pad_to_54[0x54]; float decodedRootX; };
        struct { unsigned char pad_to_58[0x58]; int decodedRootAlt; };
        struct { unsigned char pad_to_804[0x804]; float angleScale; };
        struct { unsigned char pad_to_808[0x808]; int animFrameCount; };
        struct { unsigned char pad_to_80C[0x80C]; int decodeCursor; };
        struct { unsigned char pad_to_810[0x810]; PolygonAnimation *animation; };
    } frame;
#pragma pack(pop)

    frame.createFlag = 0;
    *(char *)&frame.createFlag = 1;
    frame.flagByteA = 0;
    frame.flagByteB = 2;
    frame.angleScale = 4096.0f;
    memset(frame.scratchBuffer, 0, sizeof(frame.scratchBuffer));
    frame.animFrameCount = frameCount;
    frame.animation = (PolygonAnimation *)Anim_Create(frame.animFrameCount, boneCount - 1, frame.createFlag);
    if (frame.animation != 0) {
        frame.animFrame = 0;
        frame.decodeCursor = 0;
        frameCount = 0;
        do {
            frame.decodeCursor = BattleModel_DecodeAnimFrame((Battle3dAnimAngles *)&frame.scratchBuffer, boneCount, frame.decodeCursor, packedData);
            frame.frameCode = frame.decodedFrameCode;
            if (mirror != 0) {
                frame.mirrorRootX = -frame.decodedRootX;
            } else {
                frame.mirrorRootX = frame.decodedRootX;
            }
            frame.rootAlt = frame.decodedRootAlt;
            frame.rootAngleX[0] = (frame.tmpFloat = (float)frame.rawRootX) / frame.angleScale * 3.6e+02f;
            frame.rootAngleY = (frame.tmpFloat = (float)frame.rawRootY) / frame.angleScale * 3.6e+02f;
            frame.rootAngleZ = (frame.tmpFloat = (float)frame.rawRootZ) / frame.angleScale * 3.6e+02f;
            Anim_SetRootChannel(6, frame.animFrame, (PolygonAnimRootPose *)frame.rootAngleX, frame.animation);
            for (frame.boneIndex = 0; frame.boneIndex < boneCount - 1; frame.boneIndex++) {
                frame.boneVecX[0] = (frame.tmpFloat = (float)((Battle3dAnimAngles *)frame.scratchBuffer)[frame.boneIndex + 1].angleX) / frame.angleScale * 3.6e+02f;
                frame.boneVecY = (frame.tmpFloat = (float)((Battle3dAnimAngles *)frame.scratchBuffer)[frame.boneIndex + 1].angleY) / frame.angleScale * 3.6e+02f;
                frame.boneVecZ = (frame.tmpFloat = (float)((Battle3dAnimAngles *)frame.scratchBuffer)[frame.boneIndex + 1].angleZ) / frame.angleScale * 3.6e+02f;
                Anim_SetBoneChannel(0, frame.animFrame, frame.boneIndex, (PolygonBoneVector *)frame.boneVecX, frame.animation);
                Anim_SetBoneChannel(1, frame.animFrame, frame.boneIndex, (PolygonBoneVector *)frame.boneVecX, frame.animation);
                Anim_SetBoneChannel(2, frame.animFrame, frame.boneIndex, (PolygonBoneVector *)frame.boneVecX, frame.animation);
            }
            frameCount++;
            frame.animFrame++;
        } while (frame.decodeCursor != 0);
    }
    return (int)frame.animation;
}

/*
 * Decodes all animation frames and optionally converts and dumps them to file.
 */
// FUNCTION: FF7 0x005e806a
int __cdecl BattleModel_ConvertPackedAnimation(void *frameCountOut, int flipFlag, int boneCount, int byteCount, Battle3dDescriptorFile *packedData, int dumpToFile, int animClass, char *animIndex)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[2032];
        unsigned char fileContext[16];
        struct { unsigned char pad_to_10[0x10]; unsigned char scratchBuffer[2000]; };
        struct { unsigned char pad_to_7E0[0x7E0]; int packedHeaderWord; };
        struct { unsigned char pad_to_7E4[0x7E4]; int decodeCursor; };
        struct { unsigned char pad_to_7E8[0x7E8]; int frameCount; };
        struct { unsigned char pad_to_7EC[0x7EC]; PolygonAnimation *animation; };
    } frame;
#pragma pack(pop)

    frame.animation = 0;
    frame.packedHeaderWord = packedData->packedHeaderWord;
    memset(frame.scratchBuffer, 0, sizeof(frame.scratchBuffer));
    frame.decodeCursor = 0;
    frame.frameCount = 0;
    do {
        frame.decodeCursor = BattleModel_DecodeAnimFrame((Battle3dAnimAngles *)&frame.scratchBuffer, boneCount, frame.decodeCursor, (BattleModelAnimBank *)packedData);
        frame.frameCount++;
    } while (frame.decodeCursor != 0);
    *(int *)frameCountOut = frame.frameCount;
    if (dumpToFile != 0) {
        BattleModel_DumpAnimRecordToFile(boneCount, frame.frameCount, byteCount, packedData, animClass, animIndex);
        InitFileContext(0, &frame.fileContext);
        frame.animation = (PolygonAnimation *)BattleModel_LoadSingleAnimation(flipFlag, animClass, &frame.fileContext, animIndex);
    } else {
        frame.animation = (PolygonAnimation *)BattleModel_DecodeAnimation(flipFlag, boneCount, frame.frameCount, (BattleModelAnimBank *)packedData);
    }
    return (int)frame.animation;
}

/*
 * Debug helper that loads KNIGHT07.AMP, converts it, and writes KNIGHT12/ANM/IDLE.A.
 */
// FUNCTION: FF7 0x005e8152
void __cdecl BattleModel_ConvertKnight07AmpToIdleAnim(void)
{
    Battle3dDescriptorFile *pAmpFile;
    int sourceBytes;
    int convertedBytes;
    char *ampName;
    int boneTotal;
    PolygonAnimation *animationPtr;
    int savedMode;

    // STRING: FF7 0x00900084
    ampName = "KNIGHT07.AMP";
    boneTotal = 0x2A;
    pAmpFile = (Battle3dDescriptorFile *)LoadDiskFile(&sourceBytes, ampName);
    if (pAmpFile != 0) {
        animationPtr = (PolygonAnimation *)BattleModel_ConvertPackedAnimation(&convertedBytes, 0, boneTotal, sourceBytes, pAmpFile, 0, 0, ampName);
        if (animationPtr != 0) {
            savedMode = Pfile_GetAssetSourceMode();
            Pfile_SetAssetSourceMode(1);
            // STRING: FF7 0x00900094
            Anim_WriteBinary(animationPtr, (int)"KNIGHT12/ANM/IDLE.A");
            Pfile_SetAssetSourceMode(savedMode);
            Anim_Free((PolygonAnimBuffers *)animationPtr);
        }
        // STRING: FF7 0x009000a8
        GameHeapFree((void *)pAmpFile, "C:\\FF7\\src\\battle\\battle3d\\amptoanm.cpp", 0x1FD);
    }
}

/*
 * Opens an animation file for reading and verifies the file header.
 */
// FUNCTION: FF7 0x005e81ff
void *BattleModel_OpenAnimFileWithHeader(int animSubIndex, int headerValue, char *modelName)
{
    char fileName[0xC8];
    int header;
    int fileHandle;

    BattleModel_MakeAnimName(animSubIndex, modelName, fileName);
    fileHandle = OpenFileSimple(FILE_OPEN_CREATE, fileName);
    if (fileHandle != 0) {
        header = headerValue;
        WriteFileData(4, &header, (struct FileHandle *)fileHandle);
    }
    return (void *)fileHandle;
}

/*
 * Builds a battle model animation file name using the 'A' index format.
 */
// FUNCTION: FF7 0x005e826a
void __cdecl BattleModel_MakeAnimName(int index, char *modelName, char *outPath)
{
    MakeIndexedAssetName(0x41, index, modelName, outPath);
}

/*
 * Closes an open animation file handle.
 */
// FUNCTION: FF7 0x005e8285
void __cdecl BattleModel_CloseAnimFile(int value)
{
    CloseFileEx((struct FileHandle *)value);
}

/*
 * Writes an animation dump record header and payload to a file.
 */
// FUNCTION: FF7 0x005e8296
void __cdecl Battle_WriteAnimDumpRecord(unsigned int frameCount, unsigned int boneCount, int size, Battle3dDescriptorFile *buffer, int fileHandle)
{
    int sizeCopy;
    int frameCountCopy;
    int headerValue;

    if (fileHandle != 0) {
        headerValue = boneCount;
        frameCountCopy = frameCount;
        sizeCopy = size;
        WriteFileData(0xC, &headerValue, (struct FileHandle *)fileHandle);
        WriteFileData(size, buffer, (struct FileHandle *)fileHandle);
    }
}

/*
 * Loads an animation bank from file into the provided animation array.
 */
// FUNCTION: FF7 0x005e82de
void __cdecl BattleModel_LoadAnimBank(int negateRootY, int animSubIndex, void *animArray, void *fileCtx, char *modelName)
{
    char pathName[0xC8];
    BattleModelAnimBank *bankHeader;
    BattleModelAnimBank *bankData;
    BattleModelAnimBank *body;
    int fileSize;
    int *entryHeader;
    int animIdx;
    int *pReadPtr;

    BattleModel_MakeAnimName(animSubIndex, modelName, pathName);
    bankData = (BattleModelAnimBank *)LoadWholeFile(fileCtx, &fileSize, pathName);
    if (bankData != 0) {
        pReadPtr = (int *)bankData;
        bankHeader = (BattleModelAnimBank *)pReadPtr;
        pReadPtr++;
        for (animIdx = 0; animIdx < *(int *)bankHeader; animIdx++) {
            entryHeader = pReadPtr;
            pReadPtr += 3;
            body = (BattleModelAnimBank *)pReadPtr;
            *((void **)animArray + animIdx) = (void *)BattleModel_DecodeAnimation(negateRootY, entryHeader[0], entryHeader[1], body);
            pReadPtr = (int *)((char *)pReadPtr + entryHeader[2]);
        }
        // STRING: FF7 0x009000d0
        GameHeapFree((int *)bankData, "C:\\FF7\\src\\battle\\battle3d\\amptoanm.cpp", 0x253);
    }
}

/*
 * Loads and returns a single animation from file.
 */
// FUNCTION: FF7 0x005e840d
void *BattleModel_LoadSingleAnimation(int negateRootY, int animSubIndex, void *fileCtx, char *modelName)
{
    char pathName[0xC8];
    BattleModelAnimBank *bankData;
    void *animationPtr;
    int *body;
    int fileSize;
    int *entryHeader;

    BattleModel_MakeAnimName(animSubIndex, modelName, pathName);
    animationPtr = 0;
    bankData = (BattleModelAnimBank *)LoadWholeFile(fileCtx, &fileSize, pathName);
    if (bankData != 0) {
        entryHeader = (int *)bankData;
        body = (int *)bankData;
        body += 3;
        animationPtr = (void *)BattleModel_DecodeAnimation(negateRootY, entryHeader[0], entryHeader[1], (BattleModelAnimBank *)body);
        // STRING: FF7 0x009000f8
        GameHeapFree((int *)bankData, "C:\\FF7\\src\\battle\\battle3d\\amptoanm.cpp", 0x270);
    }
    return animationPtr;
}

/*
 * Dumps an animation record to disk for debugging/export.
 */
// FUNCTION: FF7 0x005e84d6
void __cdecl BattleModel_DumpAnimRecordToFile(int boneCount, int frameCount, int byteCount, Battle3dDescriptorFile *buffer, int animClass, char *animIndex)
{
    int fileName[50];
    int fileHandle;

    BattleModel_MakeAnimName(animClass, animIndex, (char *)fileName);
    fileHandle = OpenFileSimple(FILE_OPEN_CREATE, fileName);
    if (fileHandle != 0) {
        Battle_WriteAnimDumpRecord(frameCount, boneCount, byteCount, buffer, fileHandle);
        CloseFileEx((struct FileHandle *)fileHandle);
    }
}
} /* extern "C" */
