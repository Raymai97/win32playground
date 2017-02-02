#include "SetWindowFontRecursive.h"

BOOL CALLBACK SetWindowFontRecursive_EnumProc(HWND hwnd, LPARAM l) {
	SetWindowFontRecursive(hwnd, (HFONT)l);
	return TRUE;
}

EXTERN_C
void __stdcall
SetWindowFontRecursive(HWND hwnd, HFONT hfo) {
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hfo, (LPARAM)TRUE);
	EnumChildWindows(hwnd, SetWindowFontRecursive_EnumProc, (LPARAM)hfo);
}
