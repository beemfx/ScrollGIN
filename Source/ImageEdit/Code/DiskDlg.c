#include "diskdlg.h"

BOOL GetSaveFilename(LPTSTR title, LPTSTR strings, LPTSTR szExt, HWND hWnd, LPTSTR filename)
{
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = strings;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = title;
	ofn.lpstrDefExt=szExt;
	ofn.Flags = OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY|OFN_NONETWORKBUTTON;

	return GetSaveFileName(&ofn);
}


BOOL GetOpenFilename(LPTSTR title, LPTSTR strings, HWND hWnd, LPTSTR filename)
{
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = hWnd; // An invalid hWnd causes non-modality
	ofn.lpstrFilter = strings;
	ofn.lpstrFile = filename;  // Stores the result in this variable
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = title;// Title for dialog
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;

	return GetOpenFileName(&ofn);
}
