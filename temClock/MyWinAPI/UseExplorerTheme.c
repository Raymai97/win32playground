#include "UseExplorerTheme.h"
#include <tchar.h>

EXTERN_C
HRESULT __stdcall
UseExplorerTheme(HWND hwnd, BOOL set) {
	FARPROC fn = GetProcAddress(LoadLibraryA("uxtheme"), "SetWindowTheme");
	if (fn) {
		typedef HRESULT(WINAPI *fn_t)(HWND, LPCTSTR, LPCTSTR);
		return ((fn_t)fn)(hwnd, set ? _T("explorer") : NULL, NULL);
	}
	return E_NOTIMPL;
}
