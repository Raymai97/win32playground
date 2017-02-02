#include "GetMonitorDpi.h"

EXTERN_C
int __stdcall
GetMonitorDpi(HWND hwnd) {
	int dpi = 96;
	FARPROC fnGDFM = GetProcAddress(LoadLibraryA("SHCore"), "GetDpiForMonitor");
	if (fnGDFM) {
		FARPROC fnMFW = GetProcAddress(LoadLibraryA("user32"), "MonitorFromWindow");
		if (fnMFW) {
			typedef HANDLE(WINAPI *fnMFW_t)(HWND, DWORD);
			typedef HRESULT(WINAPI *fnGDFM_t)(HANDLE, int, UINT*, UINT*);
			((fnGDFM_t)fnGDFM)(
				((fnMFW_t)fnMFW)(hwnd, 1 /*MONITOR_DEFAULTTOPRIMARY*/),
				0 /*MDT_EFFECTIVE_DPI*/,
				&dpi, &dpi);
		}
	}
	else {
		HDC hdc0 = GetDC(NULL);
		dpi = GetDeviceCaps(hdc0, LOGPIXELSY);
		ReleaseDC(NULL, hdc0);
	}
	return dpi;
}
