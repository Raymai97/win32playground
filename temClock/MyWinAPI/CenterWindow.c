#include "CenterWindow.h"
#include "GetMonitorRect.h"

EXTERN_C
BOOL __stdcall
CenterWindow(HWND hwnd, BOOL excludeTaskbar) {
	RECT rcMon = { 0 }, rcWnd = { 0 };
	rcMon = GetMonitorRect(hwnd, excludeTaskbar);
	if (GetWindowRect(hwnd, &rcWnd)) {
		struct { int x, y, cx, cy; } mon = { 0 }, wnd = { 0 };
		wnd.cx = rcWnd.right - rcWnd.left;
		wnd.cy = rcWnd.bottom - rcWnd.top;
		mon.cx = rcMon.right - rcMon.left;
		mon.cy = rcMon.bottom - rcMon.top;
		wnd.x = (mon.cx - wnd.cx) / 2;
		wnd.y = (mon.cy - wnd.cy) / 2;
		if (SetWindowPos(hwnd, NULL, wnd.x, wnd.y, wnd.cx, wnd.cy, SWP_NOZORDER | SWP_NOACTIVATE)) {
			return TRUE;
		}
	}
	return FALSE;
}
