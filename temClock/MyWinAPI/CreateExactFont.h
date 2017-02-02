#ifndef _MYWINAPI_CREATEEXACTFONT_H
#define _MYWINAPI_CREATEEXACTFONT_H

#include <Windows.h>

EXTERN_C
HFONT __stdcall
CreateExactFont(LOGFONT const *);

EXTERN_C
HFONT __stdcall
CreateExactFont2(LPCTSTR szFontFace, int fontHeight);

#endif/*_MYWINAPI_CREATEEXACTFONT_H*/
