#include "CreateCtl.h"

#if defined(_WIN64)
#define HMENU_UINT_PTR(x)	(HMENU)(UINT_PTR)(x)
#else
#define HMENU_UINT_PTR(x)	(HMENU)(x)
#endif

EXTERN_C
HWND __stdcall
CreateCtl(int id, LPCTSTR szClass, LPCTSTR szText,
	DWORD style, DWORD exstyle, HWND hwndParent, int x, int y, int cx, int cy)
{
	return CreateWindowEx(exstyle, szClass, szText,
		WS_CHILD | WS_VISIBLE | style,
		x, y, cx, cy, hwndParent, HMENU_UINT_PTR(id), NULL, NULL);
}
