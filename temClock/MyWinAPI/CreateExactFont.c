#include "CreateExactFont.h"

EXTERN_C
HFONT __stdcall
CreateExactFont(LOGFONT const * plf) {
	HFONT hfo = CreateFontIndirect(plf);
	if (hfo) {
		HDC hdc0, hdcTmp;
		HGDIOBJ old_hfo;
		TCHAR szActual[LF_FACESIZE] = { 0 };
		hdc0 = GetDC(NULL);
		hdcTmp = CreateCompatibleDC(hdc0);
		old_hfo = SelectObject(hdcTmp, hfo);
		GetTextFace(hdcTmp, LF_FACESIZE, szActual);
		if (lstrcmpi(szActual, plf->lfFaceName) != 0) { hfo = NULL; }
		SelectObject(hdcTmp, old_hfo);
		DeleteDC(hdcTmp);
		ReleaseDC(NULL, hdc0);
	}
	return hfo;
}

EXTERN_C
HFONT __stdcall
CreateExactFont2(LPCTSTR szFontFace, int fontHeight) {
	LOGFONT lf = { 0 };
	lstrcpyn(lf.lfFaceName, szFontFace, LF_FACESIZE);
	lf.lfHeight = fontHeight;
	return CreateExactFont(&lf);
}
