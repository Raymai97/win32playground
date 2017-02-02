#include "SetWindowClientSize.h"

EXTERN_C
BOOL __stdcall
SetWindowClientSize(HWND hwnd, int cx, int cy) {
	RECT rc = { 0, 0, cx, cy };
	DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	AdjustWindowRect(&rc, dwStyle, GetMenu(hwnd) != NULL);
	cx = rc.right - rc.left;
	cy = rc.bottom - rc.top;
	return SetWindowPos(hwnd, NULL, 0, 0, cx, cy,
		SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
}
