#ifndef __DISKDLG_H__
#define __DISKDLG_H__

#include <windows.h>

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

BOOL GetSaveFilename(LPTSTR title, LPTSTR strings, LPTSTR szExt, HWND hWnd, LPTSTR filename);
BOOL GetOpenFilename(LPTSTR title, LPTSTR strings, HWND hWnd, LPTSTR filename);

#ifdef __cplusplus
} //extern "C"
#endif //__cplusplus

#endif //__DISKDLG_H__