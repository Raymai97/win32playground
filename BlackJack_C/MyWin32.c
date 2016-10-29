#include "MyWin32.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GetWindowLongPtr
#define GetWindowLongPtr GetWindowLong
#endif

double g_xy_factor = 1.0;

int AutoX(int x) {
	return (int)(x * g_xy_factor);
}

int AutoY(int y) {
	return (int)(y * g_xy_factor);
}

void CenterWindow(HWND hWnd) {
	RECT rc;
	int res_cx, res_cy, new_x, new_y;
	HDC hdc = GetDC(NULL);
	res_cx = GetDeviceCaps(hdc, HORZRES);
	res_cy = GetDeviceCaps(hdc, VERTRES);
	GetWindowRect(hWnd, &rc);
	new_x = (res_cx - CX(rc)) / 2;
	new_y = (res_cy - CY(rc)) / 2;
	SetWindowPos(hWnd, NULL, new_x, new_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	ReleaseDC(NULL, hdc);
}

int GetMonitorDpi(HWND hWnd) {
	int dpi = 96;
	FARPROC x, y;
	x = GetProcAddress(LoadLibraryA("user32.dll"), "MonitorFromWindow");
	y = GetProcAddress(LoadLibraryA("SHCore.dll"), "GetDpiForMonitor");
	if (x && y) {
		HANDLE hMonitor = ( (HANDLE (WINAPI*)(HWND, DWORD)) x)
			(hWnd, 1 /* MONITOR_DEFAULTTOPRIMARY */);
		( (HRESULT (WINAPI *)(HANDLE, INT, LPINT, LPINT)) y)
			(hMonitor, 0, &dpi, &dpi );
	}
	else {
		HDC hdc = GetDC(NULL);
		dpi = GetDeviceCaps(hdc, LOGPIXELSY);
		ReleaseDC(NULL, hdc);
	}
	return dpi;
}

void SetAutoXYFactor(double val) {
	g_xy_factor = val;
}

void SetDpiAware() {
	FARPROC x;
	x = GetProcAddress(LoadLibraryA("shcore.dll"), "SetProcessDpiAwareness");
	if (x) {
		/* HRESULT WINAPI SetProcessDpiAwareness(PROCESS_DPI_AWARENESS); */
		((HRESULT(WINAPI*)(int))x) (1 /* PROCESS_SYSTEM_DPI_AWARE */);
	}
	else {
		/* BOOL WINAPI SetProcessDPIAware(); */
		x = GetProcAddress(LoadLibraryA("user32.dll"), "SetProcessDPIAware");
		if (x) {
			((BOOL(WINAPI*)())x) ();
		}
	}
}

void SetRect2(LPRECT p_rc, int x, int y, int cx, int cy) {
	SetRect(p_rc, x, y, x + cx, y + cy);
}

void SetRectAuto(LPRECT p_rc, int x, int y, int cx, int cy) {
	SetRect(p_rc, AutoX(x), AutoY(y), AutoX(x + cx), AutoY(y + cy));
}

void SetWindowClientSize(HWND hWnd, int cx, int cy) {
	RECT rc = { 0, 0, cx, cy };
	DWORD dwStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
	AdjustWindowRect(&rc, dwStyle, (GetMenu(hWnd) != NULL));
	SetWindowPos(hWnd, NULL, 0, 0, CX(rc), CY(rc), SWP_NOMOVE | SWP_NOZORDER);
}

#ifdef __cplusplus
}
#endif
