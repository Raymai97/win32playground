#include "InitComCtlEx.h"

EXTERN_C
BOOL __stdcall
InitComCtlEx(DWORD dwICC) {
	FARPROC fn = GetProcAddress(LoadLibraryA("comctl32"), "InitCommonControlsEx");
	if (fn) {
		typedef struct { DWORD dwSize, dwICC; } iccex_t;
		typedef BOOL(WINAPI *fn_t)(iccex_t const *);
		iccex_t icc = { 0 };
		icc.dwSize = sizeof(icc);
		icc.dwICC = dwICC;
		return ((fn_t)fn)(&icc);
	}
	return FALSE;
}
