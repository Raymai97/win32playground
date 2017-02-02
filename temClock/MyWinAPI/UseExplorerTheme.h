#ifndef _MYWINAPI_USEEXPLORERTHEME_H
#define _MYWINAPI_USEEXPLORERTHEME_H

#include <Windows.h>

/* Allows ListView/TreeView to gain Vista selection style */
EXTERN_C
HRESULT __stdcall
UseExplorerTheme(HWND, BOOL set);

#endif/*_MYWINAPI_USEEXPLORERTHEME_H*/
