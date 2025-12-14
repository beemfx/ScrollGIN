/*
	Registry.cpp - routines for editing the registry

	Copyright (c) 2002, Blaine Meyrs
*/
#include "registry.h"

const TCHAR szRegister[] = TEXT("Software\\Beem\\ScrollEdit");

BOOL GetRegFilename(TCHAR szFilename[MAX_PATH]){
	HKEY hKey;
	DWORD dwDisp;
	//if((RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Beem\\ScrollEdit"), 0, KEY_ALL_ACCESS, &hKey))!=ERROR_SUCCESS){
	if((RegCreateKeyEx(HKEY_LOCAL_MACHINE, szRegister, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp))!=ERROR_SUCCESS){
		return FALSE;
	}
	DWORD dwSize=MAX_PATH;
	if((RegQueryValueEx(hKey, TEXT("Lastfile"), NULL, NULL, (BYTE*)szFilename, &dwSize))!=ERROR_SUCCESS){
		RegCloseKey(hKey);
		return FALSE;
	}
	//MessageBox(NULL, "Made it here", "Made it here", MB_OK);
	RegCloseKey(hKey);
	return TRUE;
}

BOOL SaveRegFilename(TCHAR szFilename[MAX_PATH]){
	HKEY hKey;
	DWORD dwDisp;
	//if((RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Beem\\ScrollEdit"), 0, KEY_WRITE, &hKey))!=ERROR_SUCCESS){
	if((RegCreateKeyEx(HKEY_LOCAL_MACHINE, szRegister, 0, NULL, 0, KEY_WRITE, NULL, &hKey, &dwDisp))!=ERROR_SUCCESS){
		return FALSE;
	}
	DWORD dwSize=_tcslen(szFilename)*sizeof(TCHAR);
	RegSetValueEx(hKey, TEXT("Lastfile"), NULL, REG_SZ, (BYTE*)szFilename, dwSize);
	RegCloseKey(hKey);
	return TRUE;
}