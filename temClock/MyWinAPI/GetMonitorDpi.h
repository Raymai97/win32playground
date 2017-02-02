#ifndef _MYWINAPI_GETMONITORDPI_H
#define _MYWINAPI_GETMONITORDPI_H

#include <Windows.h>

/* Assume dpiX and dpiY always the same */
EXTERN_C
int __stdcall
GetMonitorDpi(HWND);

#endif/*_MYWINAPI_GETMONITORDPI_H*/
