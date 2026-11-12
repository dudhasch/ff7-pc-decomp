// src/lib/src/graphics/dx_dbg.cpp
// Original: C:\lib\src\graphics\dx_dbg.cpp
// Contents: Maps DirectDraw and Direct3D result codes to diagnostic strings, and creates and
//   destroys the debug log target used for graphics diagnostics.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
// Also: Debug log target creation and writing: a truncating log-target constructor, and the
//   dispatch that writes a message to the active target's log file when logging is enabled.
#include "lib/api/mem.h"
#include "lib/crt.h"
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/enums/graphics.h"
#include <stdio.h>
#include <windows.h>
extern "C" {

/*
 * Maps explicitly listed DirectDraw/Direct3D HRESULT values to literal diagnostic strings; returns
 * the no-error literal for zero and an unrecognized-error literal for all unlisted values. The
 * complete switch has no other side effects.
 */
// FUNCTION: FF7 0x00664160
char *__cdecl Gfx_GetDirectXErrorString(int result)
{
    switch (result) {
    case (int)0x00000000:
        // STRING: FF7 0x0090ab64
        return "No error.";
    case DDERR_ALREADYINITIALIZED:
        // STRING: FF7 0x0090ab70
        return "This object is already initialized.";
    case DDERR_BLTFASTCANTCLIP:
        // STRING: FF7 0x0090ab94
        return "Return if a clipper object is attached to the source surface passed into a BltFast call.";
    case DDERR_CANNOTATTACHSURFACE:
        // STRING: FF7 0x0090abf0
        return "This surface can not be attached to the requested surface.";
    case DDERR_CANNOTDETACHSURFACE:
        // STRING: FF7 0x0090ac2c
        return "This surface can not be detached from the requested surface.";
    case DDERR_CANTCREATEDC:
        // STRING: FF7 0x0090ac6c
        return "Windows can not create any more DCs.";
    case DDERR_CANTDUPLICATE:
        // STRING: FF7 0x0090ac94
        return "Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.";
    case DDERR_CLIPPERISUSINGHWND:
        // STRING: FF7 0x0090ace4
        return "An attempt was made to set a cliplist for a clipper object that is already monitoring an hwnd.";
    case DDERR_COLORKEYNOTSET:
        // STRING: FF7 0x0090ad44
        return "No src color key specified for this operation.";
    case DDERR_CURRENTLYNOTAVAIL:
        // STRING: FF7 0x0090ad74
        return "Support is currently not available.";
    case DDERR_DIRECTDRAWALREADYCREATED:
        // STRING: FF7 0x0090ad98
        return "A DirectDraw object representing this driver has already been created for this process.";
    case DDERR_EXCEPTION:
        // STRING: FF7 0x0090adf0
        return "An exception was encountered while performing the requested operation.";
    case DDERR_EXCLUSIVEMODEALREADYSET:
        // STRING: FF7 0x0090ae38
        return "An attempt was made to set the cooperative level when it was already set to exclusive.";
    case (int)0x80004005:
        // STRING: FF7 0x0090ae90
        return "Generic failure.";
    case DDERR_HEIGHTALIGN:
        // STRING: FF7 0x0090aea4
        return "Height of rectangle provided is not a multiple of reqd alignment.";
    case DDERR_HWNDALREADYSET:
        // STRING: FF7 0x0090aee8
        return "The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces or palettes created.";
    case DDERR_HWNDSUBCLASSED:
        // STRING: FF7 0x0090af60
        return "HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring state.";
    case DDERR_IMPLICITLYCREATED:
        // STRING: FF7 0x0090afd0
        return "This surface can not be restored because it is an implicitly created surface.";
    case DDERR_INCOMPATIBLEPRIMARY:
        // STRING: FF7 0x0090b020
        return "Unable to match primary surface creation request with existing primary surface.";
    case DDERR_INVALIDCAPS:
        // STRING: FF7 0x0090b070
        return "One or more of the caps bits passed to the callback are incorrect.";
    case DDERR_INVALIDCLIPLIST:
        // STRING: FF7 0x0090b0b4
        return "DirectDraw does not support the provided cliplist.";
    case DDERR_INVALIDDIRECTDRAWGUID:
        // STRING: FF7 0x0090b0e8
        return "The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.";
    case DDERR_INVALIDMODE:
        // STRING: FF7 0x0090b13c
        return "DirectDraw does not support the requested mode.";
    case DDERR_INVALIDOBJECT:
        // STRING: FF7 0x0090b16c
        return "DirectDraw received a pointer that was an invalid DIRECTDRAW object.";
    case (int)0x80070057:
        // STRING: FF7 0x0090b1b4
        return "One or more of the parameters passed to the function are incorrect.";
    case DDERR_INVALIDPIXELFORMAT:
        // STRING: FF7 0x0090b1f8
        return "The pixel format was invalid as specified.";
    case DDERR_INVALIDPOSITION:
        // STRING: FF7 0x0090b224
        return "Returned when the position of the overlay on the destination is no longer legal for that destination.";
    case DDERR_INVALIDRECT:
        // STRING: FF7 0x0090b28c
        return "Rectangle provided was invalid.";
    case DDERR_LOCKEDSURFACES:
        // STRING: FF7 0x0090b2ac
        return "Operation could not be carried out because one or more surfaces are locked.";
    case DDERR_NO3D:
        // STRING: FF7 0x0090b2f8
        return "There is no 3D present.";
    case DDERR_NOALPHAHW:
        // STRING: FF7 0x0090b310
        return "Operation could not be carried out because there is no alpha accleration hardware present or available.";
    case DDERR_NOBLTHW:
        // STRING: FF7 0x0090b378
        return "No blitter hardware present.";
    case DDERR_NOCLIPLIST:
        // STRING: FF7 0x0090b398
        return "No cliplist available.";
    case DDERR_NOCLIPPERATTACHED:
        // STRING: FF7 0x0090b3b0
        return "No clipper object attached to surface object.";
    case DDERR_NOCOLORCONVHW:
        // STRING: FF7 0x0090b3e0
        return "Operation could not be carried out because there is no color conversion hardware present or available.";
    case DDERR_NOCOLORKEY:
        // STRING: FF7 0x0090b448
        return "Surface doesn't currently have a color key";
    case DDERR_NOCOLORKEYHW:
        // STRING: FF7 0x0090b474
        return "Operation could not be carried out because there is no hardware support of the destination color key.";
    case DDERR_NOCOOPERATIVELEVELSET:
        // STRING: FF7 0x0090b4dc
        return "Create function called without DirectDraw object method SetCooperativeLevel being called.";
    case DDERR_NODC:
        // STRING: FF7 0x0090b538
        return "No DC was ever created for this surface.";
    case DDERR_NODDROPSHW:
        // STRING: FF7 0x0090b564
        return "No DirectDraw ROP hardware.";
    case DDERR_NODIRECTDRAWHW:
        // STRING: FF7 0x0090b580
        return "A hardware-only DirectDraw object creation was attempted but the driver did not support any hardware.";
    case DDERR_NOEMULATION:
        // STRING: FF7 0x0090b5e8
        return "Software emulation not available.";
    case DDERR_NOEXCLUSIVEMODE:
        // STRING: FF7 0x0090b60c
        return "Operation requires the application to have exclusive mode but the application does not have exclusive mode.";
    case DDERR_NOFLIPHW:
        // STRING: FF7 0x0090b678
        return "Flipping visible surfaces is not supported.";
    case DDERR_NOGDI:
        // STRING: FF7 0x0090b6a4
        return "There is no GDI present.";
    case DDERR_NOHWND:
        // STRING: FF7 0x0090b6c0
        return "Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.";
    case DDERR_NOMIRRORHW:
        // STRING: FF7 0x0090b728
        return "Operation could not be carried out because there is no hardware present or available.";
    case DDERR_NOOVERLAYDEST:
        // STRING: FF7 0x0090b780
        return "Returned when GetOverlayPosition is called on an overlay that UpdateOverlay has never been called on to establish a destination.";
    case DDERR_NOOVERLAYHW:
        // STRING: FF7 0x0090b804
        return "Operation could not be carried out because there is no overlay hardware present or available.";
    case DDERR_NOPALETTEATTACHED:
        // STRING: FF7 0x0090b864
        return "No palette object attached to this surface.";
    case DDERR_NOPALETTEHW:
        // STRING: FF7 0x0090b890
        return "No hardware support for 16 or 256 color palettes.";
    case DDERR_NORASTEROPHW:
        // STRING: FF7 0x0090b8c4
        return "Operation could not be carried out because there is no appropriate raster op hardware present or available.";
    case DDERR_NOROTATIONHW:
        // STRING: FF7 0x0090b930
        return "Operation could not be carried out because there is no rotation hardware present or available.";
    case DDERR_NOSTRETCHHW:
        // STRING: FF7 0x0090b990
        return "Operation could not be carried out because there is no hardware support for stretching.";
    case DDERR_NOT4BITCOLOR:
        // STRING: FF7 0x0090b9e8
        return "DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.";
    case DDERR_NOT4BITCOLORINDEX:
        // STRING: FF7 0x0090ba54
        return "DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit color index palette.";
    case DDERR_NOT8BITCOLOR:
        // STRING: FF7 0x0090bacc
        return "DirectDrawSurface is not in 8 bit color mode and the requested operation requires 8 bit color.";
    case DDERR_NOTAOVERLAYSURFACE:
        // STRING: FF7 0x0090bb2c
        return "Returned when an overlay member is called for a non-overlay surface.";
    case DDERR_NOTEXTUREHW:
        // STRING: FF7 0x0090bb74
        return "Operation could not be carried out because there is no texture mapping hardware present or available.";
    case DDERR_NOTFLIPPABLE:
        // STRING: FF7 0x0090bbdc
        return "An attempt has been made to flip a surface that is not flippable.";
    case DDERR_NOTFOUND:
        // STRING: FF7 0x0090bc20
        return "Requested item was not found.";
    case DDERR_NOTLOCKED:
        // STRING: FF7 0x0090bc40
        return "Surface was not locked.  An attempt to unlock a surface that was not locked at all, or by this process, has been attempted.";
    case DDERR_NOTPALETTIZED:
        // STRING: FF7 0x0090bcbc
        return "The surface being used is not a palette-based surface.";
    case DDERR_NOVSYNCHW:
        // STRING: FF7 0x0090bcf4
        return "Operation could not be carried out because there is no hardware support for vertical blank synchronized operations.";
    case DDERR_NOZBUFFERHW:
        // STRING: FF7 0x0090bd68
        return "Operation could not be carried out because there is no hardware support for zbuffer blitting.";
    case DDERR_NOZOVERLAYHW:
        // STRING: FF7 0x0090bdc8
        return "Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support z layering of overlays.";
    case DDERR_OUTOFCAPS:
        // STRING: FF7 0x0090be48
        return "The hardware needed for the requested operation has already been allocated.";
    case (int)0x8007000e:
        // STRING: FF7 0x0090be94
        return "DirectDraw does not have enough memory to perform the operation.";
    case DDERR_OUTOFVIDEOMEMORY:
        // STRING: FF7 0x0090bed8
        return "DirectDraw does not have enough memory to perform the operation.";
    case DDERR_OVERLAYCANTCLIP:
        // STRING: FF7 0x0090bf1c
        return "The hardware does not support clipped overlays.";
    case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
        // STRING: FF7 0x0090bf4c
        return "Can only have ony color key active at one time for overlays.";
    case DDERR_OVERLAYNOTVISIBLE:
        // STRING: FF7 0x0090bf8c
        return "Returned when GetOverlayPosition is called on a hidden overlay.";
    case DDERR_PALETTEBUSY:
        // STRING: FF7 0x0090bfcc
        return "Access to this palette is being refused because the palette is already locked by another thread.";
    case DDERR_PRIMARYSURFACEALREADYEXISTS:
        // STRING: FF7 0x0090c030
        return "This process already has created a primary surface.";
    case DDERR_REGIONTOOSMALL:
        // STRING: FF7 0x0090c064
        return "Region passed to Clipper::GetClipList is too small.";
    case DDERR_SURFACEALREADYATTACHED:
        // STRING: FF7 0x0090c098
        return "This surface is already attached to the surface it is being attached to.";
    case DDERR_SURFACEALREADYDEPENDENT:
        // STRING: FF7 0x0090c0e4
        return "This surface is already a dependency of the surface it is being made a dependency of.";
    case DDERR_SURFACEBUSY:
        // STRING: FF7 0x0090c13c
        return "Access to this surface is being refused because the surface is already locked by another thread.";
    case DDERR_SURFACEISOBSCURED:
        // STRING: FF7 0x0090c1a0
        return "Access to surface refused because the surface is obscured.";
    case DDERR_SURFACELOST:
        // STRING: FF7 0x0090c1dc
        return "Access to this surface is being refused because the surface memory is gone.";
    case DDERR_SURFACENOTATTACHED:
        // STRING: FF7 0x0090c228
        return "The requested surface is not attached.";
    case DDERR_TOOBIGHEIGHT:
        // STRING: FF7 0x0090c250
        return "Height requested by DirectDraw is too large.";
    case DDERR_TOOBIGSIZE:
        // STRING: FF7 0x0090c280
        return "Size requested by DirectDraw is too large, but the individual height and width are OK.";
    case DDERR_TOOBIGWIDTH:
        // STRING: FF7 0x0090c2d8
        return "Width requested by DirectDraw is too large.";
    case (int)0x80004001:
        // STRING: FF7 0x0090c304
        return "Action not supported.";
    case DDERR_UNSUPPORTEDFORMAT:
        // STRING: FF7 0x0090c31c
        return "FOURCC format requested is unsupported by DirectDraw.";
    case DDERR_UNSUPPORTEDMASK:
        // STRING: FF7 0x0090c354
        return "Bitmask in the pixel format requested is unsupported by DirectDraw.";
    case DDERR_VERTICALBLANKINPROGRESS:
        // STRING: FF7 0x0090c398
        return "Vertical blank is in progress.";
    case DDERR_WASSTILLDRAWING:
        // STRING: FF7 0x0090c3b8
        return "Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is incomplete.";
    case DDERR_WRONGMODE:
        // STRING: FF7 0x0090c42c
        return "This surface can not be restored because it was created in a different mode.";
    case DDERR_XALIGN:
        // STRING: FF7 0x0090c47c
        return "Rectangle provided was not horizontally aligned on required boundary.";
    case D3DERR_BADMAJORVERSION:
        // STRING: FF7 0x0090c4c4
        return "D3DERR_BADMAJORVERSION";
    case D3DERR_BADMINORVERSION:
        // STRING: FF7 0x0090c4dc
        return "D3DERR_BADMINORVERSION";
    case D3DERR_EXECUTE_LOCKED:
        // STRING: FF7 0x0090c4f4
        return "D3DERR_EXECUTE_LOCKED";
    case D3DERR_EXECUTE_NOT_LOCKED:
        // STRING: FF7 0x0090c50c
        return "D3DERR_EXECUTE_NOT_LOCKED";
    case D3DERR_EXECUTE_CREATE_FAILED:
        // STRING: FF7 0x0090c528
        return "D3DERR_EXECUTE_CREATE_FAILED";
    case D3DERR_EXECUTE_DESTROY_FAILED:
        // STRING: FF7 0x0090c548
        return "D3DERR_EXECUTE_DESTROY_FAILED";
    case D3DERR_EXECUTE_LOCK_FAILED:
        // STRING: FF7 0x0090c568
        return "D3DERR_EXECUTE_LOCK_FAILED";
    case D3DERR_EXECUTE_UNLOCK_FAILED:
        // STRING: FF7 0x0090c584
        return "D3DERR_EXECUTE_UNLOCK_FAILED";
    case D3DERR_EXECUTE_FAILED:
        // STRING: FF7 0x0090c5a4
        return "D3DERR_EXECUTE_FAILED";
    case D3DERR_EXECUTE_CLIPPED_FAILED:
        // STRING: FF7 0x0090c5bc
        return "D3DERR_EXECUTE_CLIPPED_FAILED";
    case D3DERR_TEXTURE_NO_SUPPORT:
        // STRING: FF7 0x0090c5dc
        return "D3DERR_TEXTURE_NO_SUPPORT";
    case D3DERR_TEXTURE_NOT_LOCKED:
        // STRING: FF7 0x0090c5f8
        return "D3DERR_TEXTURE_NOT_LOCKED";
    case D3DERR_TEXTURE_LOCKED:
        /* the retail string lacks the underscore the code name has */
        // STRING: FF7 0x0090c614
        return "D3DERR_TEXTURELOCKED";
    case D3DERR_TEXTURE_CREATE_FAILED:
        // STRING: FF7 0x0090c62c
        return "D3DERR_TEXTURE_CREATE_FAILED";
    case D3DERR_TEXTURE_DESTROY_FAILED:
        // STRING: FF7 0x0090c64c
        return "D3DERR_TEXTURE_DESTROY_FAILED";
    case D3DERR_TEXTURE_LOCK_FAILED:
        // STRING: FF7 0x0090c66c
        return "D3DERR_TEXTURE_LOCK_FAILED";
    case D3DERR_TEXTURE_UNLOCK_FAILED:
        // STRING: FF7 0x0090c688
        return "D3DERR_TEXTURE_UNLOCK_FAILED";
    case D3DERR_TEXTURE_LOAD_FAILED:
        // STRING: FF7 0x0090c6a8
        return "D3DERR_TEXTURE_LOAD_FAILED";
    case D3DERR_MATRIX_CREATE_FAILED:
        // STRING: FF7 0x0090c6c4
        return "D3DERR_MATRIX_CREATE_FAILED";
    case D3DERR_MATRIX_DESTROY_FAILED:
        // STRING: FF7 0x0090c6e0
        return "D3DERR_MATRIX_DESTROY_FAILED";
    case D3DERR_MATRIX_SETDATA_FAILED:
        // STRING: FF7 0x0090c700
        return "D3DERR_MATRIX_SETDATA_FAILED";
    case D3DERR_SETVIEWPORTDATA_FAILED:
        // STRING: FF7 0x0090c720
        return "D3DERR_SETVIEWPORTDATA_FAILED";
    case D3DERR_MATERIAL_CREATE_FAILED:
        // STRING: FF7 0x0090c740
        return "D3DERR_MATERIAL_CREATE_FAILED";
    case D3DERR_MATERIAL_DESTROY_FAILED:
        // STRING: FF7 0x0090c760
        return "D3DERR_MATERIAL_DESTROY_FAILED";
    case D3DERR_MATERIAL_SETDATA_FAILED:
        // STRING: FF7 0x0090c780
        return "D3DERR_MATERIAL_SETDATA_FAILED";
    case D3DERR_LIGHT_SET_FAILED:
        // STRING: FF7 0x0090c7a0
        return "D3DERR_LIGHT_SET_FAILED";
    default:
        // STRING: FF7 0x0090c7b8
        return "Unrecognized error value.";
    }
}

/*
 * Returns one for result zero. For nonzero result, obtains the engine context, formats the result
 * code, its Gfx_GetDirectXErrorString description and file/line into a 512-byte buffer, reports it with
 * ReportEngineError and DD ERROR, and returns zero. No formatting bound is checked.
 */
// FUNCTION: FF7 0x00664c09
int __cdecl CheckDirectDrawResult(int result, const char *file, int line)
{
    struct {
        int ok;
        int context;
        char message[512];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.ok = 1;
    if (result != 0) {
        frame.context = (int)GetEngineContext();
        // STRING: FF7 0x0090c7d4
        sprintf(frame.message, "ERROR: %X %s in FILE %s LINE %d \n", result, Gfx_GetDirectXErrorString(result), file, line);
        // STRING: FF7 0x0090c7f8
        ReportEngineError((struct GfxEngineContext *)frame.context, frame.message, "DD ERROR", 0);
        frame.ok = 0;
    }
    return frame.ok;
}

/*
 * Destroys a debug log target, closing its file handle if active and freeing its allocation.
 */
// FUNCTION: FF7 0x00664c8b
void __cdecl Gfx_DestroyDebugLogTarget(GfxDebugLogTarget *logTarget)
{
    if (logTarget != 0) {
        /* a truncating target's file was closed when it was created (the then-arm is empty in
           the original) */
        if (logTarget->truncate != 0) { /* empty first arm: lib/codegen.h */
        } else {
            if (logTarget->file != 0) {
                fclose((FILE *)logTarget->file);
            }
        }
        // STRING: FF7 0x0090c804
        GameHeapFree(logTarget, "C:\\lib\\src\\graphics\\dx_dbg.cpp", 320);
    }
}

/*
 * Creates a debug log target: a GfxDebugLogTarget with logToFile and enabled set, the truncate
 * flag and the path (passed as an int). The path is opened with mode "w+"; with the flag set the
 * file is closed again at once (the open only truncated it), otherwise the handle stays in the
 * file member. A failed open frees the record and returns 0.
 */
// FUNCTION: FF7 0x00664ccf
int __cdecl Gfx_CreateDebugLogTarget(int truncate, void *path)
{
    int file;
    GfxDebugLogTarget *target;

    // STRING: FF7 0x0090c824
    target = (GfxDebugLogTarget *)GameHeapAlloc(1, 20, "C:\\lib\\src\\graphics\\dx_dbg.cpp", 329);
    if (target != 0) {
        target->logToFile = 1;
        target->enabled = 1;
        target->truncate = truncate;
        target->fileName = (char *)path;
        // STRING: FF7 0x0090c844
        file = (int)fopen(target->fileName, "w+");
        if (file != 0) {
            if (target->truncate != 0) {
                fclose((FILE *)file);
            } else {
                target->file = (void *)file;
            }
        } else {
            // STRING: FF7 0x0090c848
            GameHeapFree(target, "C:\\lib\\src\\graphics\\dx_dbg.cpp", 351);
            target = 0;
        }
    }
    return (int)target;
}
} /* extern "C" */



extern "C" {

/*
 * Creates a debug log target for the log file logName that reopens the file for every write.
 */
// FUNCTION: FF7 0x00664d80
int __cdecl Gfx_CreateDebugLogTargetTruncating(int logName)
{
    return Gfx_CreateDebugLogTarget(1, (void *)logName);
}

/*
 * With a target, writes message (as the fprintf format) to its log file when logToFile is set: a
 * truncating target reopens its file name with "a+" and closes it again, the other kind writes to
 * the FILE it keeps open. It also passes message to OutputDebugStringA when the target is enabled
 * and the engine context's activeA flag is set. Without a target it only calls
 * OutputDebugStringA.
 */
// FUNCTION: FF7 0x00664d93
void __cdecl WriteDebugLog(const char *message, GfxDebugLogTarget *target)
{
    FILE *ctx;

    if (target != 0) {
        if (target->logToFile != 0) {
            if (target->truncate != 0) {
                ctx = fopen(target->fileName,
                          // STRING: FF7 0x0090c868
                          "a+");
            } else {
                ctx = (FILE *)target->file;
            }
            if (ctx != 0) {
                fprintf(ctx, message);
                if (target->truncate != 0) {
                    fclose(ctx);
                }
            }
        }
        if (target->enabled != 0) {
            if (GetEngineContext()->activeA != 0) {
                OutputDebugStringA(message);
            }
        }
    } else {
        OutputDebugStringA(message);
    }
}

/*
 * The debug-log print: with an engine context, writes message through WriteDebugLog to the
 * context's debugLogTarget when activeB is set, else to OutputDebugStringA alone when activeA is
 * set.
 */
// FUNCTION: FF7 0x00664e30
void __cdecl Gfx_DebugPrint(const char *message)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (ctx != 0) {
        if (ctx->activeB != 0) {
            WriteDebugLog(message, (GfxDebugLogTarget *)(ctx->debugLogTarget));
        } else if (ctx->activeA != 0) {
            WriteDebugLog(message, 0);
        }
    }
}
} /* extern "C" */
