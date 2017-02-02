#ifndef _MYWINAPI_UXTHEME_H
#define _MYWINAPI_UXTHEME_H

#include <Windows.h>

#ifndef WM_THEMECHANGED
#define WM_THEMECHANGED		(0x031A)
#endif
#ifndef DTT_TEXTCOLOR
#define DTT_TEXTCOLOR       (1UL << 0)
#endif
#ifndef DTT_BORDERCOLOR
#define DTT_BORDERCOLOR     (1UL << 1)
#endif
#ifndef DTT_SHADOWCOLOR
#define DTT_SHADOWCOLOR     (1UL << 2)
#endif
#ifndef DTT_SHADOWTYPE
#define DTT_SHADOWTYPE      (1UL << 3)
#endif
#ifndef DTT_SHADOWOFFSET
#define DTT_SHADOWOFFSET    (1UL << 4)
#endif
#ifndef DTT_BORDERSIZE
#define DTT_BORDERSIZE      (1UL << 5)
#endif
#ifndef DTT_FONTPROP
#define DTT_FONTPROP        (1UL << 6)
#endif
#ifndef DTT_COLORPROP
#define DTT_COLORPROP       (1UL << 7)
#endif
#ifndef DTT_STATEID
#define DTT_STATEID         (1UL << 8)
#endif
#ifndef DTT_CALCRECT
#define DTT_CALCRECT        (1UL << 9)
#endif
#ifndef DTT_APPLYOVERLAY
#define DTT_APPLYOVERLAY    (1UL << 10)
#endif
#ifndef DTT_GLOWSIZE
#define DTT_GLOWSIZE        (1UL << 11)
#endif
#ifndef DTT_CALLBACK
#define DTT_CALLBACK        (1UL << 12)
#endif
#ifndef DTT_COMPOSITED
#define DTT_COMPOSITED      (1UL << 13)
#endif
#ifndef CAPTUREBLT
#define CAPTUREBLT		(0x40000000) /* Include layered windows */
#endif

EXTERN_C
BOOL __stdcall
UxtIsSupported(void);

EXTERN_C
BOOL __stdcall
UxtIsActive(void);

EXTERN_C
BOOL __stdcall
UxtIsAppThemed(void);

EXTERN_C
HANDLE __stdcall
UxtOpenThemeData(
	HWND, LPCWSTR pszClassList
);

EXTERN_C
HRESULT __stdcall
UxtCloseThemeData(
	HANDLE hTheme
);

typedef int (WINAPI *UXT_DDT_CALLBACK) (
	HDC hdc, LPWSTR pszText, int cchText, LPRECT prc,
	UINT dwFlags, LPARAM lParam);

struct UXTDRAWTHEMETEXTEX {
	HANDLE hTheme;
	HDC hdc;
	int iPartId, iStateId, cchText;
	LPCWSTR pszText;
	DWORD dwTextFlags;
	LPRECT pRect;
	struct {
		DWORD		dwSize;
		DWORD		dwFlags;
		COLORREF	crText;
		COLORREF	crBorder;
		COLORREF	crShadow;
		int		iTextShadowType;	/* TST_SINGLE or TST_CONTINUOUS */
		POINT	ptShadowOffset;		/* where shadow is drawn (relative to text) */
		int		iBorderSize;		/* Border radius around text */
		int		iFontPropId;		/* Font property to use for the text instead of TMT_FONT */
		int		iColorPropId;		/* Color property to use for the text instead of TMT_TEXTCOLOR */
		int		iStateId;			/* Alternate state id */
		BOOL	fApplyOverlay;		/* Overlay text on top of any text effect? */
		int		iGlowSize;			/* Glow radious around text */
		UXT_DDT_CALLBACK	pfnDrawTextCallback;
		LPARAM	lParam;	/* for callback */
	} opt;
};

/* Usually used to draw text on DWM frame. */
EXTERN_C
HRESULT __stdcall
UxtDrawThemeTextEx(
	struct UXTDRAWTHEMETEXTEX const *
);

/* Vista/Win7: Draw glowing text on DWM frame
 Win8 and above: Draw text on DWM frame properly */
EXTERN_C
HRESULT __stdcall
UxtDrawTextOnDwmFrame(
	HDC hdc, LPCWSTR szText,
	int x, int y, int cx, int cy,
	DWORD dwTextFlags, /* Such as DT_SINGLELINE */
	int iGlowSize, /* Recommended 10 */
	COLORREF crText /* Such as RGB(0,0,255) */
);

/* Same as UxtDrawTextOnDwmFrame(), but use caller-provided
 offscreen-HDC. After calling this, caller should BitBlt()
 the offscreen-HDC to real window HDC. */
EXTERN_C
HRESULT __stdcall
UxtDrawTextOnDwmFrameOffscreen(
	HDC hdcMem, LPCWSTR szText,
	int x, int y, int cx, int cy,
	DWORD dwTextFlags, /* Such as DT_SINGLELINE */
	int iGlowSize, /* Recommended 10 */
	COLORREF crText /* Such as RGB(0,0,255) */
);

#endif/*_MYWINAPI_UXTHEME_H*/
