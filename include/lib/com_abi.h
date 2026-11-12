#ifndef LIB_COM_ABI_H
#define LIB_COM_ABI_H

/* 32-bit COM vtable ABI interfaces for DirectX 5 (DirectDraw, Direct3D, Direct3D2)
 * and DirectShow (MultiMediaStream, DirectDrawMediaStream, StreamSample, MediaSeeking).
 *
 * Each pure-virtual C++ struct mirrors the exact retail vtable slot order; uncalled
 * methods keep empty parameter lists since only their vtable slot position matters,
 * while called methods carry their 32-bit stack argument signatures. Virtual __stdcall
 * dispatch preserves VC5's evaluation order of `this` after the vtable load without
 * emitting any vtable data or executable thunks. */

struct ComUnknown {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
};

struct ComDirectDraw {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall Compact() = 0;
    virtual int __stdcall CreateClipper(int dwFlags, void *lplpDDClipper, int pUnkOuter) = 0;
    virtual int __stdcall CreatePalette(int dwFlags, void *lpColorTable, void *lplpDDPalette, int pUnkOuter) = 0;
    virtual int __stdcall CreateSurface(void *lpDDSurfaceDesc, void *lplpDDSurface, int pUnkOuter) = 0;
    virtual int __stdcall DuplicateSurface() = 0;
    virtual int __stdcall EnumDisplayModes(int dwFlags, int lpDDSurfaceDesc, int lpContext, int lpEnumModesCallback) = 0;
    virtual int __stdcall EnumSurfaces() = 0;
    virtual int __stdcall FlipToGDISurface() = 0;
    virtual int __stdcall GetCaps(void *lpDDDriverCaps, void *lpDDHELCaps) = 0;
    virtual int __stdcall GetDisplayMode(void *lpDDSurfaceDesc) = 0;
    virtual int __stdcall GetFourCCCodes() = 0;
    virtual int __stdcall GetGDISurface() = 0;
    virtual int __stdcall GetMonitorFrequency() = 0;
    virtual int __stdcall GetScanLine() = 0;
    virtual int __stdcall GetVerticalBlankStatus() = 0;
    virtual int __stdcall Initialize() = 0;
    virtual int __stdcall RestoreDisplayMode() = 0;
    virtual int __stdcall SetCooperativeLevel(int hWnd, int dwFlags) = 0;
    virtual int __stdcall SetDisplayMode(int dwWidth, int dwHeight, int dwBPP) = 0;
    virtual int __stdcall WaitForVerticalBlank() = 0;
    /* IDirectDraw2 extension slot (slot 23). */
    virtual int __stdcall GetAvailableVidMem(void *lpDDSCaps, void *lpdwTotal, void *lpdwFree) = 0;
};

struct ComDirectDrawSurface {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall AddAttachedSurface(int lpDDSAttachedSurface) = 0;
    virtual int __stdcall AddOverlayDirtyRect() = 0;
    virtual int __stdcall Blt(void *lpDestRect, int lpDDSrcSurface, void *lpSrcRect, int dwFlags, void *lpDDBltFx) = 0;
    virtual int __stdcall BltBatch() = 0;
    virtual int __stdcall BltFast(int dwX, int dwY, int lpDDSrcSurface, int lpSrcRect, int dwTrans) = 0;
    virtual int __stdcall DeleteAttachedSurface() = 0;
    virtual int __stdcall EnumAttachedSurfaces() = 0;
    virtual int __stdcall EnumOverlayZOrders() = 0;
    virtual int __stdcall Flip(int lpDDSurfaceTargetOverride, int dwFlags) = 0;
    virtual int __stdcall GetAttachedSurface(void *lpDDSCaps, void *lplpDDAttachedSurface) = 0;
    virtual int __stdcall GetBltStatus() = 0;
    virtual int __stdcall GetCaps(void *lpDDSCaps) = 0;
    virtual int __stdcall GetClipper() = 0;
    virtual int __stdcall GetColorKey() = 0;
    virtual int __stdcall GetDC(void *lphDC) = 0;
    virtual int __stdcall GetFlipStatus() = 0;
    virtual int __stdcall GetOverlayPosition() = 0;
    virtual int __stdcall GetPalette() = 0;
    virtual int __stdcall GetPixelFormat() = 0;
    virtual int __stdcall GetSurfaceDesc(void *lpDDSurfaceDesc) = 0;
    virtual int __stdcall Initialize() = 0;
    virtual int __stdcall IsLost() = 0;
    virtual int __stdcall Lock(void *lpDestRect, void *lpDDSurfaceDesc, int dwFlags, int hEvent) = 0;
    virtual int __stdcall ReleaseDC(void *hDC) = 0;
    virtual int __stdcall Restore() = 0;
    virtual int __stdcall SetClipper(int lpDDClipper) = 0;
    virtual int __stdcall SetColorKey(int dwFlags, void *lpDDColorKey) = 0;
    virtual int __stdcall SetOverlayPosition() = 0;
    virtual int __stdcall SetPalette(int lpDDPalette) = 0;
    virtual int __stdcall Unlock(int lpSurfaceData) = 0;
};

struct ComDirectDrawPalette {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall GetCaps() = 0;
    virtual int __stdcall GetEntries() = 0;
    virtual int __stdcall Initialize() = 0;
    virtual int __stdcall SetEntries(int dwFlags, int dwStartingEntry, int dwCount, int lpEntries) = 0;
};

struct ComDirectDrawClipper {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall GetClipList() = 0;
    virtual int __stdcall GetHWnd() = 0;
    virtual int __stdcall Initialize() = 0;
    virtual int __stdcall IsClipListChanged() = 0;
    virtual int __stdcall SetClipList() = 0;
    virtual int __stdcall SetHWnd(int dwFlags, int hWnd) = 0;
};

struct ComDirect3D {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall Initialize() = 0;
    virtual int __stdcall EnumDevices(int lpEnumDevicesCallback, int lpUserArg) = 0;
    virtual int __stdcall CreateLight() = 0;
    virtual int __stdcall CreateMaterial(void *lplpD3DMaterial, int pUnkOuter) = 0;
    virtual int __stdcall CreateViewport(void *lplpD3DViewport, int pUnkOuter) = 0;
};

struct ComDirect3D2 {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall EnumDevices() = 0;
    virtual int __stdcall CreateLight() = 0;
    virtual int __stdcall CreateMaterial() = 0;
    virtual int __stdcall CreateViewport(void *lplpD3DViewport2, int pUnkOuter) = 0;
    virtual int __stdcall FindDevice() = 0;
    virtual int __stdcall CreateDevice(const void *rclsid, int lpDDS, void *lplpD3DDevice2) = 0;
};

struct ComDirect3DDevice {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall Initialize() = 0;
    virtual int __stdcall GetCaps(void *lpD3DHWDevDesc, void *lpD3DHELDevDesc) = 0;
    virtual int __stdcall SwapTextureHandles() = 0;
    virtual int __stdcall CreateExecuteBuffer(void *lpDesc, void *lplpDirect3DExecuteBuffer, int pUnkOuter) = 0;
    virtual int __stdcall GetStats() = 0;
    virtual int __stdcall Execute(int lpDirect3DExecuteBuffer, int lpDirect3DViewport, int dwFlags) = 0;
    virtual int __stdcall AddViewport(int lpDirect3DViewport) = 0;
    virtual int __stdcall DeleteViewport(int lpDirect3DViewport) = 0;
    virtual int __stdcall NextViewport() = 0;
    virtual int __stdcall Pick() = 0;
    virtual int __stdcall GetPickRecords() = 0;
    virtual int __stdcall EnumTextureFormats(int lpd3dEnumTextureProc, int lpArg) = 0;
    virtual int __stdcall CreateMatrix(void *lpd3dMatrixHandle) = 0;
    virtual int __stdcall SetMatrix(int d3dMatrixHandle, int lpD3DMatrix) = 0;
    virtual int __stdcall GetMatrix(int d3dMatrixHandle, int lpD3DMatrix) = 0;
    virtual int __stdcall DeleteMatrix(int d3dMatrixHandle) = 0;
    virtual int __stdcall BeginScene() = 0;
    virtual int __stdcall EndScene() = 0;
};

struct ComDirect3DDevice2 {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall GetCaps() = 0;
    virtual int __stdcall SwapTextureHandles() = 0;
    virtual int __stdcall GetStats() = 0;
    virtual int __stdcall AddViewport(int lpDirect3DViewport2) = 0;
    virtual int __stdcall DeleteViewport(int lpDirect3DViewport2) = 0;
    virtual int __stdcall NextViewport() = 0;
    virtual int __stdcall EnumTextureFormats() = 0;
    virtual int __stdcall BeginScene() = 0;
    virtual int __stdcall EndScene() = 0;
    virtual int __stdcall GetDirect3D() = 0;
    virtual int __stdcall SetCurrentViewport(int lpd3dViewport2) = 0;
    virtual int __stdcall GetCurrentViewport() = 0;
    virtual int __stdcall SetRenderTarget(int lpNewRenderTarget, int dwFlags) = 0;
    virtual int __stdcall GetRenderTarget() = 0;
    virtual int __stdcall Begin() = 0;
    virtual int __stdcall BeginIndexed() = 0;
    virtual int __stdcall Vertex() = 0;
    virtual int __stdcall Index() = 0;
    virtual int __stdcall End() = 0;
    virtual int __stdcall GetRenderState() = 0;
    virtual int __stdcall SetRenderState(int dwRenderStateType, int dwRenderState) = 0;
    virtual int __stdcall GetLightState() = 0;
    virtual int __stdcall SetLightState(int dwLightStateType, int dwLightState) = 0;
    virtual int __stdcall SetTransform(int dtstTransformStateType, int lpD3DMatrix) = 0;
    virtual int __stdcall GetTransform() = 0;
    virtual int __stdcall MultiplyTransform() = 0;
    virtual int __stdcall DrawPrimitive(int dptPrimitiveType, int dvtVertexType, int lpvVertices, int dwVertexCount, int dwFlags) = 0;
    virtual int __stdcall DrawIndexedPrimitive(int dptPrimitiveType, int dvtVertexType, int lpvVertices, int dwVertexCount, int lpwIndices, int dwIndexCount, int dwFlags) = 0;
};

struct ComDirect3DExecuteBuffer {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall Initialize() = 0;
    virtual int __stdcall Lock(void *lpDesc) = 0;
    virtual int __stdcall Unlock() = 0;
    virtual int __stdcall SetExecuteData(void *lpData) = 0;
};

struct ComDirect3DViewport {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall Initialize() = 0;
    virtual int __stdcall GetViewport() = 0;
    virtual int __stdcall SetViewport(int lpData) = 0;
    virtual int __stdcall TransformVertices() = 0;
    virtual int __stdcall LightElements() = 0;
    virtual int __stdcall SetBackground(int hMat) = 0;
    virtual int __stdcall GetBackground() = 0;
    virtual int __stdcall SetBackgroundDepth() = 0;
    virtual int __stdcall GetBackgroundDepth() = 0;
    virtual int __stdcall Clear(int dwCount, int lpRects, int dwFlags) = 0;
    virtual int __stdcall AddLight() = 0;
    virtual int __stdcall DeleteLight() = 0;
    virtual int __stdcall NextLight() = 0;
    /* IDirect3DViewport2 extension slots (slots 16..17). */
    virtual int __stdcall GetViewport2() = 0;
    virtual int __stdcall SetViewport2(int lpData) = 0;
};

typedef ComDirect3DViewport ComDirect3DViewport2;

struct ComDirect3DMaterial {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall Initialize() = 0;
    virtual int __stdcall SetMaterial(void *lpMat) = 0;
    virtual int __stdcall GetMaterial() = 0;
    virtual int __stdcall GetHandle(int lpDirect3DDevice, void *lphMat) = 0;
};

struct ComDirect3DTexture2 {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall GetHandle(int lpDirect3DDevice2, void *lphTex) = 0;
    virtual int __stdcall PaletteChanged(int dwStart, int dwCount) = 0;
    virtual int __stdcall Load(int lpD3DTexture2) = 0;
};

struct ComAMMultiMediaStream {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall GetInformation(void *pdwFlags, void *pStreamType) = 0;
    virtual int __stdcall GetMediaStream() = 0;
    virtual int __stdcall EnumMediaStreams() = 0;
    virtual int __stdcall GetState() = 0;
    virtual int __stdcall SetState(int newState) = 0;
    virtual int __stdcall GetTime() = 0;
    virtual int __stdcall GetDuration() = 0;
    virtual int __stdcall Seek(int seekLo, int seekHi) = 0;
    virtual int __stdcall GetEndOfStreamEventHandle() = 0;
    virtual int __stdcall Initialize(int streamType, int dwFlags, int pFilterGraph) = 0;
    virtual int __stdcall GetFilterGraph(void *ppGraphBuilder) = 0;
    virtual int __stdcall GetFilter() = 0;
    virtual int __stdcall AddMediaStream(int pStreamObject, const void *pPurposeId, int dwFlags, void *ppNewStream) = 0;
    virtual int __stdcall OpenFile(const void *pszFileName, int dwFlags) = 0;
};

typedef ComAMMultiMediaStream ComMultiMediaStream;

struct ComDirectDrawMediaStream {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall GetMultiMediaStream() = 0;
    virtual int __stdcall GetInformation() = 0;
    virtual int __stdcall SetSameFormat() = 0;
    virtual int __stdcall AllocateSample() = 0;
    virtual int __stdcall CreateSharedSample() = 0;
    virtual int __stdcall SendEndOfStream() = 0;
    virtual int __stdcall GetFormat(void *pDDSDCurrent, void *ppDirectDrawPalette, void *pDDSDDesired, void *pdwFlags) = 0;
    virtual int __stdcall SetFormat() = 0;
    virtual int __stdcall GetDirectDraw() = 0;
    virtual int __stdcall SetDirectDraw() = 0;
    virtual int __stdcall CreateSample(int pSurface, void *pRect, int dwFlags, void *ppSample) = 0;
};

struct ComStreamSample {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall GetMediaStream() = 0;
    virtual int __stdcall GetSampleTimes() = 0;
    virtual int __stdcall SetSampleTimes() = 0;
    virtual int __stdcall Update(int dwFlags, int hEvent, int pfnAPC, int dwAPCData) = 0;
    virtual int __stdcall CompletionStatus(int dwFlags, int dwMilliseconds) = 0;
};

struct ComMediaSeeking {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual int __stdcall Release() = 0;
    virtual int __stdcall GetCapabilities() = 0;
    virtual int __stdcall CheckCapabilities() = 0;
    virtual int __stdcall IsFormatSupported() = 0;
    virtual int __stdcall QueryPreferredFormat() = 0;
    virtual int __stdcall GetTimeFormat() = 0;
    virtual int __stdcall IsUsingTimeFormat() = 0;
    virtual int __stdcall SetTimeFormat(const void *pFormat) = 0;
    virtual int __stdcall GetDuration() = 0;
    virtual int __stdcall GetStopPosition() = 0;
    virtual int __stdcall GetCurrentPosition(void *pCurrent) = 0;
    virtual int __stdcall ConvertTimeFormat() = 0;
    virtual int __stdcall SetPositions(void *pCurrent, int dwCurrentFlags, void *pStop, int dwStopFlags) = 0;
};

#ifdef DIRECTINPUT_VERSION
struct ComDirectInputDevice {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
    virtual int __stdcall GetCapabilities(DIDEVCAPS *lpDIDevCaps) = 0;
    virtual int __stdcall EnumObjects() = 0;
    virtual int __stdcall GetProperty() = 0;
    virtual int __stdcall SetProperty(const GUID &rguidProp, const DIPROPHEADER *pdiph) = 0;
    virtual int __stdcall Acquire() = 0;
    virtual int __stdcall Unacquire() = 0;
    virtual int __stdcall GetDeviceState(int cbData, void *lpvData) = 0;
    virtual int __stdcall GetDeviceData(int cbObjectData, void *rgdod, int *pdwInOut, int dwFlags) = 0;
    virtual int __stdcall SetDataFormat(const DIDATAFORMAT *lpdf) = 0;
    virtual int __stdcall SetEventNotification() = 0;
    virtual int __stdcall SetCooperativeLevel(HWND hwnd, unsigned long dwFlags) = 0;
    virtual int __stdcall GetObjectInfo() = 0;
    virtual int __stdcall GetDeviceInfo() = 0;
    virtual int __stdcall RunControlPanel() = 0;
    virtual int __stdcall Initialize() = 0;
};

struct ComDirectInput {
    virtual int __stdcall QueryInterface(const void *riid, void *ppvObj) = 0;
    virtual int __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
    virtual int __stdcall CreateDevice(const GUID &rguid, ComDirectInputDevice **lplpDirectInputDevice, IUnknown *pUnkOuter) = 0;
    virtual int __stdcall EnumDevices() = 0;
    virtual int __stdcall GetDeviceStatus() = 0;
    virtual int __stdcall RunControlPanel() = 0;
    virtual int __stdcall Initialize() = 0;
};
#endif /* DIRECTINPUT_VERSION */

#endif /* LIB_COM_ABI_H */
