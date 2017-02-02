#ifndef _MYWINAPI_DWM_H
#define _MYWINAPI_DWM_H

#include <Windows.h>

#ifndef WM_DWMCOMPOSITIONCHANGED
#define WM_DWMCOMPOSITIONCHANGED		0x031E
#endif
#ifndef WM_DWMNCRENDERINGCHANGED
#define WM_DWMNCRENDERINGCHANGED		0x031F
#endif
#ifndef WM_DWMCOLORIZATIONCOLORCHANGED
#define WM_DWMCOLORIZATIONCOLORCHANGED	0x0320
#endif
#ifndef WM_DWMWINDOWMAXIMIZEDCHANGE
#define WM_DWMWINDOWMAXIMIZEDCHANGE		0x0321
#endif
#ifndef WM_DWMSENDICONICTHUMBNAIL
#define WM_DWMSENDICONICTHUMBNAIL		0x0323
#endif
#ifndef WM_DWMSENDICONICTHUMBNAIL
#define WM_DWMSENDICONICTHUMBNAIL		0x0326
#endif


EXTERN_C
BOOL __stdcall
DwmIsSupported(void);

EXTERN_C
BOOL __stdcall
DwmIsEnabled(void);

EXTERN_C
HRESULT __stdcall
DwmExtendFrame(
	HWND, int cxLeft, int cxRight, int cyTop, int cyBottom
);

EXTERN_C
HRESULT __stdcall
DwmEnableBlurBehind(
	HWND, BOOL fEnable, HRGN
);

EXTERN_C
HRESULT  __stdcall
DwmRegisterThumb(
	HWND hwndDest, HWND hwndSource, HANDLE *phThumb
);

EXTERN_C
HRESULT __stdcall
DwmUnregisterThumb(HANDLE hThumb);

/* Update rendering options of hThumb. */
EXTERN_C
HRESULT __stdcall
DwmUpdateThumb(HANDLE hThumb,
	RECT const * prcDest,
	RECT const * prcSource, /* Set this explicitly to avoid weird problem */
	BYTE const * pOpacity, /* NULL for opaque */
	BOOL const * pfVisible,
	BOOL const * pfSourceClientAreaOnly /* if FALSE, DWM scale ugly */
);

/* Set Thumbnail bitmap. Vista not supported. */
EXTERN_C
HRESULT __stdcall
DwmSetIconicThumb(
	HWND, HBITMAP, BOOL fDisplayFrame
);

/* Set Peek Preview bitmap. Vista not supported. */
EXTERN_C
HRESULT __stdcall
DwmSetIconicLivePreviewBmp(
	HWND, HBITMAP, BOOL fDisplayFrame
);

/* Vista not supported. */
EXTERN_C
HRESULT __stdcall
DwmInvalidateIconic(HWND);

/* ------------------------------------- */
/* DWM Window Attribute ---------------- */

/* For dwAttr, refers DWMWINDOWATTRIBUTE enum in dwmapi.h */
EXTERN_C
HRESULT __stdcall
DwmGetWindowAttr(
	HWND, DWORD dwAttr, PVOID pvAttr, DWORD cbAttr
);

/* For dwAttr, refers DWMWINDOWATTRIBUTE enum in dwmapi.h */
EXTERN_C
HRESULT __stdcall
DwmSetWindowAttr(
	HWND, DWORD dwAttr, LPCVOID pvAttr, DWORD cbAttr
);

/* Is DWM rendering non-client area? */
EXTERN_C
BOOL __stdcall
DwmIsNcRenderingEnabled(HWND);

/* Specify if DWM should render non-client area */
EXTERN_C
HRESULT __stdcall
DwmSetNcRenderingPolicy(
	HWND, BOOL fUseDefault, BOOL fEnable
);
 
/* Force disable DWM transition/animation */
EXTERN_C
HRESULT __stdcall
DwmForceDisableTransition(
	HWND, BOOL fForce
);

/* Allow DWM displays content rendered in non-client area. */
EXTERN_C
HRESULT __stdcall
DwmAllowNcPaint(
	HWND, BOOL fAllow
);

/* Force override thumbnail / live preview */
EXTERN_C
HRESULT __stdcall
DwmForceIconicRepresentation(
	HWND, BOOL fForce
);

/* Specify that window will provide bitmap on:
 * WM_DWM-SEND-ICONIC-LIVE-PREVIEW-BITMAP
 * WM_DWM-SEND-ICONIC-THUMBNAIL
 Vista not supported. */
EXTERN_C
HRESULT __stdcall
DwmHasIconicBitmap(
	HWND, BOOL fHas
);

/* Win8 and above not supported. */
EXTERN_C
HRESULT __stdcall
DwmSetFlip3DPolicy(
	HWND, BOOL fUseDefault, BOOL fBelowFlip3d
);

/* Disallow Aero Peek. Vista not supported. */
EXTERN_C
HRESULT __stdcall
DwmDisallowPeek(
	HWND, BOOL fDisallow
);

/* Never fade to glass. Vista not supported. */
EXTERN_C
HRESULT __stdcall
DwmExcludeFromPeek(
	HWND, BOOL fExclude
);

/* ---------------- DWM Window Attribute */

#endif/*_MYWINAPI_DWM_H*/
