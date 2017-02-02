#include "PidlFromPath.h"

EXTERN_C
LPITEMIDLIST __stdcall
PidlFromPath(LPCTSTR szPath) {
	HRESULT hr = 0;
	IShellLink *psl = NULL;
	LPITEMIDLIST pidl = NULL;

	CoInitialize(NULL);
	hr = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLink, (LPVOID)&psl);
	if (SUCCEEDED(hr)) {
		hr = psl->lpVtbl->SetPath(psl, szPath);
		if (SUCCEEDED(hr)) {
			psl->lpVtbl->GetIDList(psl, &pidl);
		}
		psl->lpVtbl->Release(psl);
	}
	CoUninitialize();
	return pidl;
}
