/* Macro and helper functions that're often used by Win32 apps */

#ifndef _MYWIN32_H
#define _MYWIN32_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Windows.h>

#define CX(rc)		(rc.right - rc.left)
#define CY(rc)		(rc.bottom - rc.top)
#define LOGICAL(x)	(-MulDiv(x, 96, 72)) /* LogicalFromPoint for LOGFONT and etc... */

int AutoX(int);
int AutoY(int);
void CenterWindow(HWND);
int GetMonitorDpi(HWND);
void SetAutoXYFactor(double);
void SetDpiAware();
void SetRect2(LPRECT, int x, int y, int cx, int cy);
void SetRectAuto(LPRECT, int x, int y, int cx, int cy);
void SetWindowClientSize(HWND, int cx, int cy);

#ifdef __cplusplus
}
#endif
#endif/*_MAIWIN32_H*/

