#include "Create32bitBmp.h"

EXTERN_C
HBITMAP __stdcall
Create32bitBmp(
	HDC hdc, long cx, long cy, void **ppvBits)
{
	HBITMAP hbmp = NULL;
	void *pvBits = NULL;
	BITMAPINFO bi = { 0 };
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biWidth = cx;
	bi.bmiHeader.biHeight = -cy;
	hbmp = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &pvBits, NULL, 0);
	if (ppvBits) { *ppvBits = pvBits; }
	return hbmp;
}
