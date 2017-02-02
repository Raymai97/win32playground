#include "Dwm.h"

EXTERN_C
BOOL __stdcall
DwmIsSupported(void)
{
	return NULL != GetProcAddress(LoadLibraryA("dwmapi"),
		"DwmIsCompositionEnabled");
}

EXTERN_C
BOOL __stdcall
DwmIsEnabled(void)
{
	BOOL fEnabled = FALSE;
	FARPROC fn = GetProcAddress(LoadLibraryA("dwmapi"),
		"DwmIsCompositionEnabled");
	if (fn) {
		typedef HRESULT(WINAPI *fn_t)(BOOL*);
		((fn_t)fn)(&fEnabled);
	}
	return fEnabled;
}

EXTERN_C
HRESULT __stdcall
DwmExtendFrame(
	HWND hwnd, int cxLeft, int cxRight, int cyTop, int cyBottom)
{
	FARPROC fn = GetProcAddress(LoadLibraryA("dwmapi"),
		"DwmExtendFrameIntoClientArea");
	if (fn) {
		struct MARGINS_t { int cx1, cx2, cy1, cy2; }
		margins = { cxLeft, cxRight, cyTop, cyBottom };
		typedef HRESULT(WINAPI *fn_t)(HWND, struct MARGINS_t const *);
		return ((fn_t)fn)(hwnd, &margins);
	}
	return E_NOTIMPL;
}

EXTERN_C
HRESULT __stdcall
DwmEnableBlurBehind(
	HWND hwnd, BOOL fEnable, HRGN hrgnBlur)
{
	FARPROC fn = GetProcAddress(LoadLibraryA("dwmapi"),
		"DwmEnableBlurBehindWindow");
	if (fn) {
		struct BB_t {
			DWORD dwFlags;
			BOOL fEnable;
			HRGN hRgnBlur;
			BOOL fTransitionOnMaximized;
		} bb = { 0x3, fEnable, hrgnBlur };
		typedef HRESULT(WINAPI *fn_t)(HWND, struct BB_t const *);
		return ((fn_t)fn)(hwnd, &bb);
	}
	return E_NOTIMPL;
}

EXTERN_C
HRESULT  __stdcall
DwmRegisterThumb(
	HWND hwndDest, HWND hwndSource, HANDLE *phThumb) 
{ 
	FARPROC fn = GetProcAddress(LoadLibraryA("dwmapi"),
		"DwmRegisterThumbnail");
	if (fn) {
		typedef HRESULT(WINAPI *fn_t)(HWND, HWND, HANDLE*);
		return ((fn_t)fn)(hwndDest, hwndSource, phThumb);
	}
	return E_NOTIMPL;
}

EXTERN_C
HRESULT __stdcall
DwmUnregisterThumb(
	HANDLE hThumb)
{
	FARPROC fn = GetProcAddress(LoadLibraryA("dwmapi"),
		"DwmUnregisterThumbnail");
	if (fn) {
		typedef HRESULT(WINAPI *fn_t)(HANDLE);
		return ((fn_t)fn)(hThumb);
	}
	return E_NOTIMPL;
}

EXTERN_C
HRESULT __stdcall
DwmUpdateThumb(HANDLE hThumb,
	RECT const * prcDest,
	RECT const * prcSource,
	BYTE const * pOpacity,
	BOOL const * pfVisible,
	BOOL const * pfSourceClientAreaOnly)
{
	FARPROC fn = GetProcAddress(LoadLibraryA("dwmapi"),
		"DwmUpdateThumbnailProperties");
	if (fn) {
		struct PROP_t {
			DWORD dwFlags;
			RECT rcDest, rcSource;
			BYTE opacity;
			BOOL fVisible, fSourceClientAreaOnly;
		} prop = { 0 };
		typedef HRESULT(WINAPI *fn_t)(HANDLE, struct PROP_t const *);
		prop.dwFlags =
			(prcDest ? 0x1 : 0) |
			(prcSource ? 0x2 : 0) |
			(pOpacity ? 0x4 : 0) |
			(pfVisible ? 0x8 : 0) |
			(pfSourceClientAreaOnly ? 0x10 : 0);
		if (prcDest) { prop.rcDest = *prcDest; }
		if (prcSource) { prop.rcSource = *prcSource; }
		if (pOpacity) { prop.opacity = *pOpacity; }
		if (pfVisible) { prop.fVisible = *pfVisible; }
		if (pfSourceClientAreaOnly) {
			prop.fSourceClientAreaOnly = *pfSourceClientAreaOnly;
		}
		return ((fn_t)fn)(hThumb, &prop);
	}
	return E_NOTIMPL;
}

EXTERN_C
HRESULT __stdcall
DwmSetIconicThumb(
	HWND hwnd, HBITMAP hbmp, BOOL fDisplayFrame)
{
	FARPROC fn = GetProcAddress(LoadLibraryA("dwmapi"),
		"DwmSetIconicThumbnail");
	if (fn) {
		typedef HRESULT(WINAPI *fn_t)(HWND, HBITMAP, DWORD);
		return ((fn_t)fn)(hwnd, hbmp, fDisplayFrame ? 1 : 0);
	}
	return E_NOTIMPL;
}

EXTERN_C
HRESULT __stdcall
DwmSetIconicLivePreviewBmp(
	HWND hwnd, HBITMAP hbmp, BOOL fDisplayFrame)
{
	FARPROC fn = GetProcAddress(LoadLibraryA("dwmapi"),
		"DwmSetIconicLivePreviewBitmap");
	if (fn) {
		typedef HRESULT(WINAPI *fn_t)(HWND, HBITMAP, POINT*, DWORD);
		return ((fn_t)fn)(hwnd, hbmp, NULL, fDisplayFrame ? 1 : 0);
	}
	return E_NOTIMPL;
}

EXTERN_C
HRESULT __stdcall
DwmInvalidateIconic(
	HWND hwnd)
{
	FARPROC fn = GetProcAddress(LoadLibraryA("dwmapi"),
		"DwmInvalidateIconicBitmaps");
	if (fn) {
		typedef HRESULT(WINAPI *fn_t)(HWND);
		return ((fn_t)fn)(hwnd);
	}
	return E_NOTIMPL;
}

EXTERN_C
HRESULT __stdcall
DwmGetWindowAttr(
	HWND hwnd, DWORD dwAttr, PVOID pvAttr, DWORD cbAttr)
{
	FARPROC fn = GetProcAddress(LoadLibraryA("dwmapi"),
		"DwmGetWindowAttribute");
	if (fn) {
		typedef HRESULT(WINAPI *fn_t)(HWND, DWORD, PVOID, DWORD);
		return ((fn_t)fn)(hwnd, dwAttr, pvAttr, cbAttr);
	}
	return E_NOTIMPL;
}

EXTERN_C
HRESULT __stdcall
DwmSetWindowAttr(
	HWND hwnd, DWORD dwAttr, LPCVOID pvAttr, DWORD cbAttr)
{
	FARPROC fn = GetProcAddress(LoadLibraryA("dwmapi"),
		"DwmSetWindowAttribute");
	if (fn) {
		typedef HRESULT(WINAPI *fn_t)(HWND, DWORD, LPCVOID, DWORD);
		return ((fn_t)fn)(hwnd, dwAttr, pvAttr, cbAttr);
	}
	return E_NOTIMPL;
}

EXTERN_C
BOOL __stdcall
DwmIsNcRenderingEnabled(
	HWND hwnd)
{
	BOOL fEnable = FALSE;
	DwmGetWindowAttr(hwnd, 1, (PVOID)&fEnable, sizeof(BOOL));
	return fEnable;
}

EXTERN_C
HRESULT __stdcall
DwmSetNcRenderingPolicy(
	HWND hwnd, BOOL fUseDefault, BOOL fEnable)
{
	DWORD policy = fUseDefault ? 0 : fEnable ? 2 : 1;
	return DwmSetWindowAttr(hwnd, 2, (LPCVOID)&policy, sizeof(DWORD));
}

EXTERN_C
HRESULT __stdcall
DwmForceDisableTransition(
	HWND hwnd, BOOL fForce)
{
	return DwmSetWindowAttr(hwnd, 3, (LPCVOID)&fForce, sizeof(BOOL));
}

EXTERN_C
HRESULT __stdcall
DwmAllowNcPaint(
	HWND hwnd, BOOL fAllow)
{
	return DwmSetWindowAttr(hwnd, 4, (LPCVOID)&fAllow, sizeof(BOOL));
}

EXTERN_C
HRESULT __stdcall
DwmForceIconicRepresentation(
	HWND hwnd, BOOL fForce)
{
	return DwmSetWindowAttr(hwnd, 7, (LPCVOID)&fForce, sizeof(BOOL));
}

EXTERN_C
HRESULT __stdcall
DwmHasIconicBitmap(
	HWND hwnd, BOOL fHas)
{
	return DwmSetWindowAttr(hwnd, 10, (LPCVOID)&fHas, sizeof(BOOL));
}

EXTERN_C
HRESULT __stdcall
DwmSetFlip3DPolicy(
	HWND hwnd, BOOL fUseDefault, BOOL fBelowFlip3d)
{
	DWORD policy = fUseDefault ? 0 : fBelowFlip3d ? 1 : 2;
	return DwmSetWindowAttr(hwnd, 8, (LPCVOID)&policy, sizeof(DWORD));
}

EXTERN_C
HRESULT __stdcall
DwmDisallowPeek(
	HWND hwnd, BOOL fDisallow)
{
	return DwmSetWindowAttr(hwnd, 11, (LPCVOID)&fDisallow, sizeof(BOOL));
}

EXTERN_C
HRESULT __stdcall
DwmExcludeFromPeek(HWND hwnd, BOOL fExclude)
{
	return DwmSetWindowAttr(hwnd, 12, (LPCVOID)&fExclude, sizeof(BOOL));
}

