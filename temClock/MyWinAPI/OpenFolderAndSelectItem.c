#include "OpenFolderAndSelectItem.h"
#include "PidlFromPath.h"
#include <tchar.h>

EXTERN_C
HRESULT __stdcall
OpenFolderAndSelectItem(LPCTSTR szPath) {
	HRESULT hr = E_FAIL;
	LPITEMIDLIST pidl = PidlFromPath(szPath);
	CoInitialize(NULL);
	if (pidl) {
		FARPROC fnOFASI = GetProcAddress(LoadLibraryA("shell32"), "SHOpenFolderAndSelectItems");
		if (fnOFASI) {
			/* It is possible to select multiple items in a folder, */
			/* but this function only select 1 item, so */
			typedef HRESULT(WINAPI *fnOFASI_t)(
				LPCITEMIDLIST, /* should be the item to select, */
				UINT, LPCITEMIDLIST*, DWORD); /* the other should be zero/nullptr */
			hr = ((fnOFASI_t)fnOFASI)(pidl, 0, NULL, 0);
		}
		else {
			/* I've tried the IWebBrowserApp IShellFolderViewDual SelectItem() nonsense */
			/* Too many codes yet the result is not perfect, so fxxx it */
			STARTUPINFO si = { 0 }; PROCESS_INFORMATION pi = { 0 };
			TCHAR szCmdLine[999] = { 0 }, szWinDir[MAX_PATH] = { 0 };
			GetWindowsDirectory(szWinDir, MAX_PATH);
			wsprintf(szCmdLine, _T("%s\\explorer.exe /select,%s"), szWinDir, szPath);
			if (CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
				hr = S_OK;
			}
		}
	}
	CoUninitialize();
	return hr;
}
