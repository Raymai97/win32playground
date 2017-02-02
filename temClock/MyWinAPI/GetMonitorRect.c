#include "GetMonitorRect.h"

EXTERN_C
RECT __stdcall
GetMonitorRect(HWND hwnd, BOOL excludeTaskbar) {
	FARPROC fnMFW = GetProcAddress(LoadLibraryA("user32"), "MonitorFromWindow");
	if (fnMFW) {
		FARPROC fnGMI = GetProcAddress(LoadLibraryA("user32"), "GetMonitorInfo");
		if (fnGMI) {
			typedef struct { DWORD cbSize; RECT rcMonitor; RECT rcWork; DWORD dwFlags; } MI;
			typedef HANDLE(WINAPI *fnMFW_t)(HWND, DWORD);
			typedef BOOL(WINAPI *fnGMI_t)(HANDLE, MI*);
			HANDLE hmon = NULL;
			MI mi = { 0 };
			mi.cbSize = sizeof(mi);
			hmon = ((fnMFW_t)fnMFW)(hwnd, 1 /*MONITOR_DEFAULTTOPRIMARY*/);
			if (((fnGMI_t)fnGMI)(hmon, &mi)) {
				return excludeTaskbar ? mi.rcWork : mi.rcMonitor;
			}
		}
	}
	if (excludeTaskbar) {
		RECT rcWork = { 0 };
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
		return rcWork;
	}
	else {
		RECT rcMon = { 0 };
		rcMon.right = GetSystemMetrics(SM_CXSCREEN);
		rcMon.bottom = GetSystemMetrics(SM_CYSCREEN);
		return rcMon;
	}
}
