#ifndef _MYWINAPI_PIDLFROMPATH_H
#define _MYWINAPI_PIDLFROMPATH_H

#include <Windows.h>
#include <ShlObj.h>

EXTERN_C
LPITEMIDLIST __stdcall
PidlFromPath(LPCTSTR);

#endif/*_MYWINAPI_PIDLFROMPATH_H*/
