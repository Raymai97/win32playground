#ifndef _MYWINAPI_GETMONITORRECT_H
#define _MYWINAPI_GETMONITORRECT_H

#include <Windows.h>

EXTERN_C
RECT __stdcall
GetMonitorRect(HWND, BOOL excludeTaskbar);

#endif/*_MYWINAPI_GETMONITORRECT_H*/
