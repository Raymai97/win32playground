#ifndef _MYWINAPI_CREATE32BITBMP_H
#define _MYWINAPI_CREATE32BITBMP_H

#include <Windows.h>

EXTERN_C
HBITMAP __stdcall
Create32bitBmp(
	HDC, long cx, long cy, void **ppvBits
);

#endif
