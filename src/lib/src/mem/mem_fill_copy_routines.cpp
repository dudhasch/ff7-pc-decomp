// src/lib/src/mem/mem_fill_copy_routines.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Memory fill and copy routines: byte, word, dword and RGB fills and copies with aligned
//   fast paths and 2D row variants, plus small value-replication helpers.
//   Bodies preserve retail's inline x86 string/register sequences.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "lib/crt.h"
#include "lib/api/mem.h"
extern "C" {

#pragma data_seg(".u69str")
#pragma data_seg(".u69ptr")
#pragma data_seg()

/*
 * Fills a destination memory buffer with a repeated byte value.
 */
// FUNCTION: FF7 0x0069bc10
void __cdecl Mem_FillBytes(int value, int byteCount, int dst)
{
    int p;
    __asm {
        mov eax, value
        mov ecx, byteCount
        mov edi, dst
        rep stosb
        mov p, edi
        mov eax, p
    }
}

/*
 * Fills memory with a repeated 16-bit word value using aligned DWORD stores.
 */
// FUNCTION: FF7 0x0069bc2f
int __cdecl Mem_FillWordsFast(int value, int count, int destination)
{
    int p;
    __asm {
        mov eax, value
        mov ecx, count
        mov edi, destination
        cmp ecx, 32
        jle words_only
        mov ebx, eax
        shl eax, 16
        mov ax, bx
        mov edx, ecx
        mov ebx, edi
        and ebx, 2
        jz aligned
        sub edx, 2
        mov word ptr [edi], ax
        add edi, 2
    aligned:
        mov ecx, edx
        shr ecx, 2
        rep stosd
        and edx, 2
        jz finished
        mov word ptr [edi], ax
        add edi, 2
        jmp finished
    words_only:
        shr ecx, 1
        rep stosw
    finished:
        mov p, edi
    }
    return p;
}

/*
 * Fills memory with a repeated byte value using aligned DWORD stores.
 */
// FUNCTION: FF7 0x0069bc84
int __cdecl Mem_FillBytesFast(int value, int count, int destination)
{
    int p;
    __asm {
        mov eax, value
        mov ecx, count
        mov edi, destination
        jcxz finished
        mov ah, al
        test edi, 1
        jz aligned
        mov byte ptr [edi], al
        inc edi
        dec ecx
    aligned:
        shr ecx, 1
        jnc even_count
        jcxz last_byte
        rep stosw
    last_byte:
        mov byte ptr [edi], al
        inc edi
        jmp finished
    even_count:
        jcxz finished
        rep stosw
    finished:
        mov p, edi
    }
    return p;
}

/*
 * Stores repeated 24-bit RGB triplets into a destination buffer.
 */
// FUNCTION: FF7 0x0069bcc7
unsigned char *__cdecl Mem_FillRgbTriplets(unsigned char r, unsigned char g, unsigned char b, int count, unsigned char *destination)
{
    unsigned char *result;

    __asm {
        mov ah, r
        mov al, g
        mov bl, b
        mov ecx, count
        mov edi, destination
fill_next:
        mov byte ptr [edi], ah
        mov byte ptr [edi + 1], al
        mov byte ptr [edi + 2], bl
        add edi, 3
        dec ecx
        jne fill_next
        mov result, edi
    }
    return result;
}

/*
 * Fills destination memory with a 32-bit DWORD value over a byte length.
 */
// FUNCTION: FF7 0x0069bcf8
void __cdecl Mem_FillDwordsByByteCount(int value, int byteCount, int dst)
{
    int p;
    __asm {
        mov eax, value
        mov ecx, byteCount
        mov edi, dst
        shr ecx, 2
        rep stosd
        mov p, edi
        mov eax, p
    }
}

/*
 * Copies memory using aligned DWORD transfers when size exceeds threshold.
 */
// FUNCTION: FF7 0x0069bd1a
void __cdecl Mem_CopyBytesFast(int count, void *dst, const void *src)
{
    __asm {
        mov ecx, count
        mov edi, dst
        mov esi, src
        cmp ecx, 32
        jle copy_bytes
        mov edx, ecx
        mov ebx, edi
        and ebx, 3
        jz aligned
        neg ebx
        add ebx, 4
        sub edx, ebx
        mov ecx, ebx
        rep movsb
    aligned:
        mov ecx, edx
        shr ecx, 2
        rep movsd
        and edx, 3
        jz finished
        mov ecx, edx
    copy_bytes:
        rep movsb
    finished:
    }
}

/*
 * Copies memory in 16-bit word increments with alignment handling.
 */
// FUNCTION: FF7 0x0069bd57
void __cdecl Mem_CopyWordsAlignedByByteCount(int byteCount, void *destination, void *source)
{
    __asm {
        mov ecx, byteCount
        mov edi, destination
        mov esi, source
        cmp ecx, 0x20
        jle copy_words
        mov edx, ecx
        mov ebx, edi
        and ebx, 2
        je copy_dwords
        sub edx, 2
        movsw
    copy_dwords:
        mov ecx, edx
        shr ecx, 2
        rep movsd
        and edx, 2
        je copy_done
        movsw
        jmp copy_done
    copy_words:
        shr ecx, 1
        rep movsw
    copy_done:
    }
}

/*
 * Copies memory in 32-bit DWORD increments for a given byte count.
 */
// FUNCTION: FF7 0x0069bd93
void __cdecl Mem_CopyDwordsByByteCount(int byteCount, void *destination, void *source)
{
    __asm {
        mov ecx, byteCount
        mov edi, destination
        mov esi, source
        shr ecx, 2
        rep movsd
    }
}

/*
 * Copies a specified count of 32-bit DWORDs from source to destination.
 */
// FUNCTION: FF7 0x0069bdac
void __cdecl Mem_CopyDwords(int dwordCount, void *destination, void *source)
{
    __asm {
        mov ecx, dwordCount
        mov edi, destination
        mov esi, source
        rep movsd
    }
}

/*
 * Fills 2D byte rows with a repeated byte value using row stride.
 */
// FUNCTION: FF7 0x0069bdc2
void *__cdecl Mem_FillByteRows(int value, int width, int rows, int stride, void *destination)
{
    void *result;

    __asm {
        mov eax, value
        mov ebx, width
        mov edx, rows
        mov esi, stride
        mov edi, destination
fill_byte_row:
        mov ecx, ebx
        rep stosb
        add edi, esi
        dec edx
        jne fill_byte_row
        mov result, edi
    }
    return result;
}

/*
 * Fills 2D word rows with a repeated 16-bit word value using row stride.
 */
// FUNCTION: FF7 0x0069bdee
void *__cdecl Mem_FillWordRows(int value, int width, int rows, int stride, void *destination)
{
    void *result;

    __asm {
        mov eax, value
        mov ebx, width
        mov edx, rows
        mov esi, stride
        mov edi, destination
        shr ebx, 1
fill_word_row:
        mov ecx, ebx
        rep stosw
        add edi, esi
        dec edx
        jne fill_word_row
        mov result, edi
    }
    return result;
}

/*
 * Fills 2D RGB image rows with 24-bit color triplets using row stride.
 */
// FUNCTION: FF7 0x0069be1d
int __cdecl Mem_FillRgbRows(int red, int green, int blue, int width, int height, int stride, int destination)
{
    int p;
    __asm {
        mov ah, byte ptr red
        mov al, byte ptr green
        mov bl, byte ptr blue
        mov ecx, width
        mov edx, height
        mov esi, stride
        mov edi, destination
        push ebp
        mov ebp, ecx
    next_row:
        mov ecx, ebp
    next_pixel:
        mov byte ptr [edi], ah
        mov byte ptr [edi + 1], al
        mov byte ptr [edi + 2], bl
        add edi, 3
        dec ecx
        jnz next_pixel
        add edi, esi
        dec edx
        jnz next_row
        pop ebp
        mov p, edi
    }
    return p;
}

/*
 * Fills 2D DWORD rows with a repeated 32-bit value using row stride.
 */
// FUNCTION: FF7 0x0069be5f
void *__cdecl Mem_FillDwordRows(int value, int width, int rows, int stride, void *destination)
{
    void *result;

    __asm {
        mov eax, value
        mov ebx, width
        mov edx, rows
        mov esi, stride
        mov edi, destination
        shr ebx, 2
fill_dword_row:
        mov ecx, ebx
        rep stosd
        add edi, esi
        dec edx
        jne fill_dword_row
        mov result, edi
    }
    return result;
}

/*
 * Copies 2D byte rows between source and destination using individual strides.
 */
// FUNCTION: FF7 0x0069be8e
unsigned char *__cdecl Mem_CopyByteRows(int width, int rows, int sourceStride, int destinationStride, unsigned char *source, unsigned char *destination)
{
    unsigned char *result;

    __asm {
        mov ecx, width
        mov edx, rows
        mov eax, sourceStride
        mov ebx, destinationStride
        mov esi, source
        mov edi, destination
        push ebp
        mov ebp, ecx
copy_byte_row:
        mov ecx, ebp
        rep movsb
        add esi, eax
        add edi, ebx
        dec edx
        jne copy_byte_row
        pop ebp
        mov result, esi
    }
    return result;
}

/*
 * Copies 2D word rows between source and destination using individual strides.
 */
// FUNCTION: FF7 0x0069bec3
unsigned short *__cdecl Mem_CopyWordRows(int width, int rows, int sourceStride, int destinationStride, unsigned short *source, unsigned short *destination)
{
    unsigned short *result;

    __asm {
        mov ecx, width
        mov edx, rows
        mov eax, sourceStride
        mov ebx, destinationStride
        mov esi, source
        mov edi, destination
        push ebp
        shr ecx, 1
        mov ebp, ecx
copy_word_row:
        mov ecx, ebp
        rep movsw
        add esi, eax
        add edi, ebx
        dec edx
        jne copy_word_row
        pop ebp
        mov result, esi
    }
    return result;
}

/*
 * Copies 2D DWORD rows between source and destination using individual strides.
 */
// FUNCTION: FF7 0x0069befb
void *__cdecl Mem_CopyDwordRows(int byteWidth, int rows, int sourceStride, int destinationStride, void *source, void *destination)
{
    void *result;

    __asm {
        mov ecx, byteWidth
        mov edx, rows
        mov eax, sourceStride
        mov ebx, destinationStride
        mov esi, source
        mov edi, destination
        push ebp
        shr ecx, 2
        mov ebp, ecx
    copy_row:
        mov ecx, ebp
        rep movsd
        add esi, eax
        add edi, ebx
        dec edx
        jne copy_row
        pop ebp
        mov result, esi
    }
    return result;
}

/*
 * Multiplies an integer value by three using LEA arithmetic.
 */
// FUNCTION: FF7 0x0069bf33
int __cdecl Blit_MultiplyByThree(int value)
{
    int result;

    __asm {
        mov eax, value
        lea eax, [eax + eax * 2]
        mov result, eax
    }
    return result;
}

/*
 * Replicates a byte value across low and high 16-bit halves of a DWORD.
 */
// FUNCTION: FF7 0x0069bf4d
int __cdecl Blit_ReplicateByteToDword(int value)
{
    int p;
    __asm {
        mov eax, value
        mov ah, al
        mov ecx, eax
        shl eax, 0x10
        or eax, ecx
        mov p, eax
    }
    return p;
}

/*
 * Replicates a 16-bit word into both words of a 32-bit DWORD.
 */
// FUNCTION: FF7 0x0069bf6d
int __cdecl Blit_ReplicateWordToDword(int value)
{
    int p;
    __asm {
        mov eax, value
        mov ecx, eax
        shl eax, 0x10
        or eax, ecx
        mov p, eax
    }
    return p;
}
} /* extern "C" */
