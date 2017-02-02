#ifndef _MYWINAPI_CREATECTL_H
#define _MYWINAPI_CREATECTL_H

#include <Windows.h>

EXTERN_C
HWND __stdcall
CreateCtl(int id, LPCTSTR szClass, LPCTSTR szText,
	DWORD style, DWORD exstyle, HWND hwndParent, int x, int y, int cx, int cy);

#endif/*_MYWINAPI_CREATECTL_H*/
