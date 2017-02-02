#include "Uxtheme.h"
#include "Create32bitBmp.h"

EXTERN_C
BOOL __stdcall
UxtIsSupported(void) {
	FARPROC fn = GetProcAddress(LoadLibraryA("uxtheme"),
		"IsThemeActive");
	return fn != NULL;
}

EXTERN_C
BOOL __stdcall
UxtIsActive(void) {
	FARPROC fn = GetProcAddress(LoadLibraryA("uxtheme"),
		"IsThemeActive");
	if (fn) {
		typedef BOOL(WINAPI *fn_t)(VOID);
		return ((fn_t)fn)();
	}
	return FALSE;
}

EXTERN_C
BOOL __stdcall
UxtIsAppThemed(void) {
	FARPROC fn = GetProcAddress(LoadLibraryA("uxtheme"),
		"IsAppThemed");
	if (fn) {
		typedef BOOL(WINAPI *fn_t)(VOID);
		return ((fn_t)fn)();
	}
	return FALSE;
}

EXTERN_C
HANDLE __stdcall
UxtOpenThemeData(
	HWND hwnd, LPCWSTR pszClassList)
{
	FARPROC fn = GetProcAddress(LoadLibraryA("uxtheme"),
		"OpenThemeData");
	if (fn) {
		typedef HANDLE(WINAPI *fn_t)(HWND, LPCWSTR);
		return ((fn_t)fn)(hwnd, pszClassList);
	}
	return NULL;
}

EXTERN_C
HRESULT __stdcall
UxtCloseThemeData(
	HANDLE hTheme)
{
	FARPROC fn = GetProcAddress(LoadLibraryA("uxtheme"),
		"CloseThemeData");
	if (fn) {
		typedef HRESULT(WINAPI *fn_t)(HANDLE);
		return ((fn_t)fn)(hTheme);
	}
	return E_NOTIMPL;
}

EXTERN_C
HRESULT __stdcall
UxtDrawThemeTextEx(
	struct UXTDRAWTHEMETEXTEX const * p)
{
	FARPROC fn = GetProcAddress(LoadLibraryA("uxtheme"),
		"DrawThemeTextEx");
	if (fn) {
		typedef HRESULT(WINAPI *fn_t)(HANDLE, HDC, int, int,
			LPCWSTR, int, DWORD, LPRECT, void const *);
		return ((fn_t)fn)(
			p->hTheme, p->hdc, p->iPartId, p->iStateId,
			p->pszText, p->cchText, p->dwTextFlags,
			p->pRect,
			&(p->opt));
	}
	return E_NOTIMPL;
}

EXTERN_C
HRESULT __stdcall
UxtDrawTextOnDwmFrame(
	HDC hdc, LPCWSTR szText,
	int x, int y, int cx, int cy,
	DWORD dwTextFlags,
	int iGlowSize,
	COLORREF crText)
{
	HRESULT hr = 0;
	HDC hdcMem = NULL;
	HGDIOBJ hbm = NULL, old_hbm = NULL;
	HGDIOBJ hfo = NULL, old_hfo = NULL;

	/* Create offscreen DC and Bitmap */
	hdcMem = CreateCompatibleDC(hdc);
	if (!hdcMem) { return E_FAIL; }
	hbm = Create32bitBmp(hdc, cx, cy, NULL);
	if (!hbm) { DeleteDC(hdcMem); return E_FAIL; }

	/* Let hdcMem use hFont of hdc */
	hfo = (HFONT)GetCurrentObject(hdc, OBJ_FONT);
	old_hfo = SelectObject(hdcMem, hfo);
	old_hbm = SelectObject(hdcMem, hbm);

	hr = UxtDrawTextOnDwmFrameOffscreen(
		hdcMem, szText, x, y, cx, cy,
		dwTextFlags, iGlowSize, crText);
	if (SUCCEEDED(hr)) {
		BitBlt(hdc, 0, 0, cx, cy, hdcMem, 0, 0, SRCCOPY | CAPTUREBLT);
	}

	/* Clean-up */
	SelectObject(hdc, old_hfo);
	SelectObject(hdc, old_hbm);
	DeleteObject(hbm);
	DeleteDC(hdcMem);
	return hr;
}

EXTERN_C
HRESULT __stdcall
UxtDrawTextOnDwmFrameOffscreen(
	HDC hdcMem, LPCWSTR szText,
	int x, int y, int cx, int cy,
	DWORD dwTextFlags,
	int iGlowSize,
	COLORREF crText)
{
	HRESULT hr = 0;
	HANDLE hTheme = NULL;
	struct UXTDRAWTHEMETEXTEX a = { 0 };
	RECT dtt_rect = { x, y, x + cx, y + cy };

	/* Obtain theme handle */
	hTheme = UxtOpenThemeData(GetDesktopWindow(), L"TextStyle");
	if (!hTheme) { return E_FAIL; }

	/* Prepare parameter for DrawThemeTextEx */	
	a.hTheme = hTheme;
	a.hdc = hdcMem;
	a.pszText = szText;
	a.cchText = -1; /* Draw all text */
	a.dwTextFlags = dwTextFlags;
	a.pRect = &dtt_rect;
	a.opt.dwSize = sizeof(a.opt);
	a.opt.dwFlags = DTT_GLOWSIZE | DTT_COMPOSITED | DTT_TEXTCOLOR;
	a.opt.iGlowSize = iGlowSize;
	a.opt.crText = crText;
	hr = UxtDrawThemeTextEx(&a);

	/* Clean-up */
	UxtCloseThemeData(hTheme);
	return hr;
}
