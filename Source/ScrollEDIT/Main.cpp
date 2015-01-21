/*
	ScrollEdit - An editor for ScrollGIN maps

	Main.cpp - Entry point for ScrollEdit

	Copyright (c) 2002, Blaine Myers
*/

#include <windows.h>
#include <htmlhelp.h>
#include <stdio.h>
#include <tchar.h>

#include "ConstLibrary.h"
#include "../ImageLib/ImageLib.h"
#include "../MapBoard/Mapboard.h"
#include "resource.h"
#include "defines.h"
#include "registry.h"

/* Identifiers for the child windows. */
#define ID_PALETTE_WINDOW 0x12340001
#define ID_EDIT_WINDOW    0x12340002

#define PALETTEDIM (MAP_TILEDIM*PALETTEWIDTH+GetSystemMetrics(SM_CXHSCROLL))
#define PALETTEWIDTH 3

#ifdef UNICODE
#define VERSIONTEXT TEXT("version x.xx  UNICODE")
#else //unicode
#define VERSIONTEXT TEXT("version x.xx  ANSI")
#endif //unicode

CEditMapBoard g_cEditMapboard;
CImageArchive ITileArchive;
CConstantArchive g_cArchLibrary(MAKEINTRESOURCE(IDB_ARCHTILES), 28, MAP_TILEDIM, MAP_TILEDIM);

BOOL g_bQueryForSave=FALSE;


//This function removes everything, but the actual filename from a string
BOOL RemoveDir(TCHAR szOutput[MAX_PATH], TCHAR szFilename[MAX_PATH])
{
	int nLength = _tcslen(szFilename);
	TCHAR szTempString[MAX_PATH];
	int nPos;
	for(int i=nLength; i!=0; i--)
	{
		nPos=i;
		if(szFilename[i]=='\\')
			break;
	}
	nPos++;
	int j=0;
	int i=0;
	for(i=0, j=nPos; i<nLength-nPos; i++, j++)
	{
		szTempString[i]=szFilename[j];
	}
	szTempString[i]=NULL;
	_tcscpy_s(szOutput, MAX_PATH, szTempString);
	return TRUE;
}



//Use common dlg to get save filename.
BOOL GetSaveFilename(
	LPTSTR title, 
	LPTSTR strings, 
	LPTSTR szDefaultExt, 
	HWND hWnd, 
	LPTSTR filename)
{
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = strings;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = title;
	ofn.lpstrDefExt = szDefaultExt;
	ofn.Flags = OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY|OFN_NONETWORKBUTTON;

	return GetSaveFileName(&ofn);
}

//Use common dlg to get open filename.
BOOL GetOpenFilename(
	LPTSTR title, 
	LPTSTR strings, 
	HWND hWnd, 
	LPTSTR filename)
{
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = hWnd; // An invalid hWnd causes non-modality
	ofn.lpstrFilter = strings;
	ofn.lpstrFile = filename;  // Stores the result in this variable
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = title;// Title for dialog
	ofn.Flags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;
	return GetOpenFileName(&ofn);
}

//Querys to save returns true if program should terminate
BOOL QueryForSave(HWND hwnd, TCHAR szMapName[MAX_PATH])
{
	TCHAR szQuestion[MAX_PATH+20];
	TCHAR szSaveFilename[MAX_PATH];
	_stprintf_s(szQuestion, countof(szQuestion), TEXT("Save changes to %s?"), szMapName);
		
	//we should only query on exit if the last move in someway edited the map
	
	switch(MessageBox(hwnd, szQuestion, TEXT("ScrollEdit"), MB_YESNOCANCEL|MB_ICONWARNING))
	{
	case IDYES:
	{
		if(szMapName[0]!=NULL){
			g_cEditMapboard.SaveMap(szMapName);
			return TRUE;
		}
		else
		{ 
			_tcscpy_s(szSaveFilename, countof(szSaveFilename), TEXT("Map name"));
			if(GetSaveFilename(
				TEXT("Save map..."), 
				TEXT("ScrollGIN Map (*.map)\0*.map\0All Files (*.*)\0*.*\0"), 
				TEXT("map"), 
				hwnd, 
				szSaveFilename))
			{
				_tcscpy_s(szMapName, countof(szMapName), szSaveFilename);
				g_cEditMapboard.SaveMap(szMapName);
				return TRUE;
			}
		}

		break;
	}
	case IDNO:
		return TRUE;
	case IDCANCEL:
		return FALSE;
	default:
		break;
	}

	return FALSE;
}

//Procedure for about box.
BOOL CALLBACK AboutProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch(uMsg)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hwnd, IDC_VERSION, VERSIONTEXT);
		break;
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hwnd, 0);
			break;
		default:
			break;
		}

		break;
	}
	case WM_CLOSE: 
		EndDialog(hwnd, 0);
		break;
	default: 
		return FALSE;
	}
	
	return TRUE;
}

//Populates a list box with using specified string.
BOOL PopulateListBoxWithFileType(HWND hwnd, int nDlgItem, TCHAR szSearchParam[MAX_PATH])
{
	HANDLE hFind;
	WIN32_FIND_DATA sFindData;
	hFind = FindFirstFile(szSearchParam, &sFindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		SendDlgItemMessage(
			hwnd, 
			nDlgItem, 
			CB_ADDSTRING, 
			0, 
			(LPARAM)TEXT("Could not find an appropriate file!"));
		return FALSE;
	}
	else
	{
		SendDlgItemMessage(
			hwnd, 
			nDlgItem, 
			CB_ADDSTRING, 
			0, 
			(LPARAM)TEXT(""));

		SendDlgItemMessage(
			hwnd, 
			nDlgItem, 
			CB_ADDSTRING, 
			0, 
			(LPARAM)sFindData.cFileName);

		while(FindNextFile(hFind, &sFindData))
		{
			SendDlgItemMessage(hwnd, nDlgItem, CB_ADDSTRING, 0, (LPARAM)sFindData.cFileName);
		}
		FindClose(hFind);
	}
	return TRUE;
}

//Procedure for changing the library.
BOOL CALLBACK LibDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR szTempName[MAX_PATH];

	switch(uMsg)
	{
	case WM_INITDIALOG:
		SetWindowText(hwnd, TEXT("Change Library"));
		PopulateListBoxWithFileType(hwnd, IDC_NEWFILENAME, TEXT("*ilb"));
		break;
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		case IDAPPLY:
		{
			GetDlgItemText(hwnd, IDC_NEWFILENAME, szTempName, MAX_PATH);
			if(MessageBox(
				hwnd, 
				TEXT("Are you sure you want to change the library?"), 
				TEXT("ScrollEdit"), 
				MB_YESNO|MB_ICONQUESTION)==IDYES)
			{
				if(!(ITileArchive.LoadArchive(szTempName)))
				{
					MessageBox(
						hwnd, 
						TEXT("Library was invalid."), 
						TEXT("ScrollEdit"), 
						MB_OK|MB_ICONWARNING);
					break;
				}
				g_cEditMapboard.ChangeLibrary(szTempName);
				EndDialog(hwnd, 1);
			}

			break;
		}
		default:
			break;
		}

		break;
	}
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default: 
		return FALSE;
	}
	return TRUE;
}

//procedure for change bg.
BOOL CALLBACK BGDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR szTempName[MAX_PATH];

	switch(uMsg)
	{
	case WM_INITDIALOG:
		SetWindowText(hwnd, TEXT("Change Background"));
		PopulateListBoxWithFileType(hwnd, IDC_NEWFILENAME, TEXT("*.bmp"));
		break;

	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case IDCANCEL:EndDialog(hwnd, 0);break;
		case IDAPPLY:
		{
			GetDlgItemText(hwnd, IDC_NEWFILENAME, szTempName, MAX_PATH);
			if(MessageBox(
				hwnd, 
				TEXT("Are you sure you want to change the background?"), 
				TEXT("ScrollEdit"), 
				MB_YESNO|MB_ICONQUESTION)==IDYES)
			{
				g_cEditMapboard.ChangeBackground(szTempName);
				EndDialog(hwnd, 1);
			}

			break;
		}
		default: 
			break;
		}

		break;
	}
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default: 
		return FALSE;
	}
	return TRUE;
}

//procudure for change dimension dlg proc
BOOL CALLBACK DimDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg)
	{
	case WM_INITDIALOG:
		SetDlgItemInt(hwnd, IDC_NEWWIDTH, g_cEditMapboard.GetMapWidth(), FALSE);
		SetDlgItemInt(hwnd, IDC_NEWHEIGHT, g_cEditMapboard.GetMapHeight(), FALSE);
		break;

	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case IDCANCEL:EndDialog(hwnd, 0);break;
		case IDAPPLY:
		{
			BOOL bSuccess=FALSE;
			int nWidth=0;
			int nHeight=0;

			//Get new dimensions and make sure they are valid
			nWidth=GetDlgItemInt(hwnd, IDC_NEWWIDTH, &bSuccess, FALSE);
			if(!bSuccess)
			{
				MessageBox(
					hwnd, 
					TEXT("Width or Height was invalid!"), 
					TEXT("ScrollEdit"), 
					MB_OK|MB_ICONWARNING);
					
				break;
			}
			nHeight=GetDlgItemInt(hwnd, IDC_NEWHEIGHT, &bSuccess, FALSE);
			if(!bSuccess)
			{
				MessageBox(
					hwnd, 
					TEXT("Width or Height was invalid!"), 
					TEXT("ScrollEdit"), 
					MB_OK|MB_ICONWARNING);
						
				break;
			}
			//make sure dimensions are large enough
			if(nWidth<16||nHeight<12)
			{
				MessageBox(
					hwnd, 
					TEXT("Width and/or Height was too small!"), 
					TEXT("ScrollEdit"), 
					MB_OK|MB_ICONWARNING);
						
				break;
			}
				
			//if map size is smaller confirm the change
			if(nWidth<g_cEditMapboard.GetMapWidth() || nHeight<g_cEditMapboard.GetMapHeight())
			{
				if(MessageBox(hwnd, TEXT("The new map size is smaller, some information may be lost!\n\tContinue with operation?"), TEXT("ScrollEdit"), MB_YESNO|MB_ICONQUESTION)==IDNO)break;
			}
					
			//Change perameters and EndDialog with a nonzero value
			g_cEditMapboard.ChangeMapDimensions(nWidth, nHeight);
			EndDialog(hwnd, 1);

			break;
		}

		default:
			break;
		}

		break;
	}

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

//procedrue for statistics dlg
BOOL CALLBACK StatsDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
	{
		const TCHAR* szTempName = g_cEditMapboard.GetLibraryName();
		SetDlgItemText(hwnd, IDC_LIBRARY , szTempName);
		szTempName = g_cEditMapboard.GetBGName();
		SetDlgItemText(hwnd, IDC_BACKGROUND, szTempName);
		SetDlgItemInt(hwnd, IDC_MAPWIDTH, g_cEditMapboard.GetMapWidth(), FALSE);
		SetDlgItemInt(hwnd, IDC_MAPHEIGHT, g_cEditMapboard.GetMapHeight(), FALSE);
	} break;
	
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hwnd, 0);
			break;
		default:
			break;
		}

		break;
	}
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default: 
		return FALSE;
	}
	
	return TRUE;
}


//procedure for new map dlg
BOOL CALLBACK NewDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		PopulateListBoxWithFileType(
			hwnd, 
			IDC_LIBRARYNAME, 
			TEXT("*.ilb"));
		PopulateListBoxWithFileType(
			hwnd, 
			IDC_BGFILENAME, 
			TEXT("*.bmp"));
		break;

	case WM_CLOSE:
		SendMessage(hwnd, WM_COMMAND, ID_CANCEL, NULL);
		break;
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case ID_CANCEL:
			EndDialog(hwnd, 1);
			break;
		case ID_OK:
		{
			//Lets get width and height and make sure they are valid
			BOOL bSuccess;
			int nWidth, nHeight;
			nWidth=GetDlgItemInt(hwnd, IDC_MAPWIDTH, &bSuccess, FALSE);
			if(bSuccess)
				nHeight=GetDlgItemInt(hwnd, IDC_MAPHEIGHT, &bSuccess, FALSE);
			if(!bSuccess)
			{ 
				MessageBox(
					hwnd, 
					TEXT("Invalid Width and/or Height!"), 
					TEXT("ScrollEdit"), 
					MB_OK|MB_ICONWARNING);
				break;
			}
			if(nWidth<16 || nHeight < 12)
			{
				MessageBox(
					hwnd, 
					TEXT("Width must be 16 or greater,\nHeight must be 12 or greater."), 
					TEXT("ScrollEdit"), 
					MB_OK|MB_ICONWARNING);

				break;
			}
					
			//Now we know the width and height are valid parameters
			//So now get the library and background
			TCHAR szLibraryName[MAX_PATH];
			TCHAR szBGName[MAX_PATH];
			GetDlgItemText(hwnd, IDC_LIBRARYNAME, szLibraryName, MAX_PATH);
			GetDlgItemText(hwnd, IDC_BGFILENAME, szBGName, MAX_PATH);
			if(!(ITileArchive.LoadArchive(szLibraryName)))
			{
				MessageBox(hwnd, TEXT("Library was invalid."), TEXT("ScrollEdit"), MB_OK|MB_ICONWARNING);
				break;
			}
					
			//We don't bother to check whether the background is valid or not,
			//because we don't actually need a background
			if((g_cEditMapboard.GenerateNewMap(nWidth, nHeight, szLibraryName, szBGName)))
			{
				EndDialog(hwnd, 0);
				break;
			}
			else 
			{
				MessageBox(
					hwnd, 
					TEXT("A problem occured while attempting to generate map!\nPossibly ran out of memory."), 
					TEXT("ScrollEdit"), 
					MB_OK|MB_ICONERROR);
			}
			
			//If the program made it here, 
			//chances are there was an error and the dialog will continue
			break;
		}

		default:
			break;
		}

		break;
	}
	default:
		return FALSE;
	}

	return TRUE;
}

//Finds which layer is selected.
LAYER GetSelectedLayer(HWND hwnd)
{
	MENUITEMINFO mi;
	mi.cbSize=sizeof(mi);
	mi.fMask=MIIM_STATE;
	
	GetMenuItemInfo(GetMenu(hwnd), ID_LAYERTILE, FALSE, &mi);
	if((mi.fState&MFS_CHECKED)==MFS_CHECKED)
		return LAYER_TILE;
	
	GetMenuItemInfo(GetMenu(hwnd), ID_LAYERARCH, FALSE, &mi);
	if((mi.fState&MFS_CHECKED)==MFS_CHECKED)
		return LAYER_ARCH;
	
	GetMenuItemInfo(GetMenu(hwnd), ID_LAYEROBJECT, FALSE, &mi);
	if((mi.fState&MFS_CHECKED)==MFS_CHECKED)
		return LAYER_OBJECT;
	
	
	return LAYER_ERROR;
}

//Update the scroll bar for the palette (image library).
void UpdatePalScroll(HWND hwnd, int nBlockDim, int nVPos)
{
	RECT rect;
	GetClientRect(hwnd, &rect);

	int nNumRows;

	LAYER nLayer=GetSelectedLayer(GetParent(hwnd));

	if(nLayer==LAYER_TILE)
	{
		nNumRows=ITileArchive.GetNumEntries()/PALETTEWIDTH;
		if(ITileArchive.GetNumEntries()%PALETTEWIDTH)
			nNumRows++;

	}
	else if(nLayer==LAYER_ARCH)
	{
		nNumRows=g_cArchLibrary.GetNumEntries()/PALETTEWIDTH;
		if(g_cArchLibrary.GetNumEntries()%PALETTEWIDTH)
			nNumRows++;
	}

	SCROLLINFO si;
	si.cbSize=sizeof(si);
	si.fMask=SIF_POS;
	GetScrollInfo(hwnd, SB_VERT, &si);
	si.fMask=SIF_ALL|SIF_DISABLENOSCROLL;
	si.nMin=1;
	si.nMax=nNumRows;
	si.nPage=rect.bottom/nBlockDim;
	if(nVPos==NULL)si.nPos=si.nPos;
	else si.nPos=nVPos;

	SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
}

//Update the scroll bars for teh edit window.
void UpdateEditScroll(HWND hwnd, int nBlockDim, int nHPos, int nVPos)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	
	SCROLLINFO si;
	si.cbSize=sizeof(si);
	si.fMask=SIF_POS;
	GetScrollInfo(hwnd, SB_HORZ, &si);
	si.fMask=SIF_ALL;
	si.nMin=0;
	si.nMax=(g_cEditMapboard.GetMapWidth())*nBlockDim-1;
	si.nPage=rect.right+1;
	if(nHPos==(-1))si.nPos=si.nPos;
	else si.nPos=nHPos;

	SetScrollInfo(hwnd, SB_HORZ, &si, FALSE);

	si.fMask=SIF_POS;
	GetScrollInfo(hwnd, SB_VERT, &si);
	si.fMask=SIF_ALL;
	si.nMin=0;
	si.nMax=(g_cEditMapboard.GetMapHeight())*nBlockDim-1;
	si.nPage=rect.bottom+1;
	if(nVPos==(-1))si.nPos=si.nPos;
	else si.nPos=nVPos;

	SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
}

BOOL SetSelectedLayer(HWND hwnd, HWND hwndPal, LAYER nNewLayer)
{
	switch(nNewLayer)
	{
	case LAYER_TILE:
	{
		MENUITEMINFO mi;
		mi.cbSize = sizeof(mi);
		mi.fMask = MIIM_STATE;
		mi.fState=MFS_CHECKED;
		SetMenuItemInfo(GetMenu(hwnd), ID_LAYERTILE, FALSE, &mi);
		mi.fState=MFS_UNCHECKED;
		SetMenuItemInfo(GetMenu(hwnd), ID_LAYERARCH, FALSE, &mi);
		SetMenuItemInfo(GetMenu(hwnd), ID_LAYEROBJECT, FALSE, &mi);
		UpdatePalScroll(hwndPal, MAP_TILEDIM, NULL);
		RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE);
	
		break;
	}
	case LAYER_ARCH:
	{
		MENUITEMINFO mi;
		mi.cbSize = sizeof(mi);
		mi.fMask = MIIM_STATE;
		mi.fState=MFS_CHECKED;
		SetMenuItemInfo(GetMenu(hwnd), ID_LAYERARCH, FALSE, &mi);
		mi.fState=MFS_UNCHECKED;
		SetMenuItemInfo(GetMenu(hwnd), ID_LAYERTILE, FALSE, &mi);
		SetMenuItemInfo(GetMenu(hwnd), ID_LAYEROBJECT, FALSE, &mi);
		UpdatePalScroll(hwndPal, MAP_TILEDIM, NULL);
		RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE);
	
		break;
	}
	case LAYER_OBJECT:
	{
		MENUITEMINFO mi;
		mi.cbSize = sizeof(mi);
		mi.fMask = MIIM_STATE;
		mi.fState=MFS_CHECKED;
		SetMenuItemInfo(GetMenu(hwnd), ID_LAYEROBJECT, FALSE, &mi);
		mi.fState=MFS_UNCHECKED;
		SetMenuItemInfo(GetMenu(hwnd), ID_LAYERARCH, FALSE, &mi);
		SetMenuItemInfo(GetMenu(hwnd), ID_LAYERTILE, FALSE, &mi);
		UpdatePalScroll(hwndPal, MAP_TILEDIM, NULL);
		RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE);
	
		break;
	}
	}

	return TRUE;
}


//convert a screen coordinate to a tile coordinate
void WndPosToTile(HWND hwnd, POINT *ps, int nBlockDim)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	
	//Find out how many tiles wide the grid is
	int nTilesWide=rect.right/nBlockDim;
	int nTilesHigh=rect.bottom/nBlockDim;
	//Check to see if there is only a portion of a tile near the edge,
	//by using modulus.  If there is we add another tile to the end.
	if(rect.right%nBlockDim)nTilesWide++;
	if(rect.bottom%nBlockDim)nTilesHigh++;

	//Find out the size of each block
	//Should be the same as MAP_TILEDIM
	int nxBlock = nBlockDim;
	int nyBlock = nBlockDim;

	//Convert Window coordinate to tile
	//This algorithm seems to work quite well.
	ps->x = ps->x/nxBlock+1;
	ps->y = ps->y/nyBlock+1;
	
}

//Draws a thick rectangle on the screen
//to represent the cursor for the selected tile.
void DrawPaletteCursor(HDC hDc, int x, int y)
{
	HPEN hPen=CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
	HBRUSH hBrush=NULL;
	
	LOGBRUSH lbBrush;
	lbBrush.lbColor=NULL;
	lbBrush.lbHatch=NULL;
	lbBrush.lbStyle=BS_NULL;

	hBrush = CreateBrushIndirect(&lbBrush);

	SelectObject(hDc, hBrush);
	SelectObject(hDc, hPen);

	
	Rectangle(
		hDc, 
		x*MAP_TILEDIM-MAP_TILEDIM+2, 
		y*MAP_TILEDIM-MAP_TILEDIM+2, 
		x*MAP_TILEDIM, 
		y*MAP_TILEDIM);

	DeleteObject(SelectObject(hDc, GetStockObject(WHITE_BRUSH)));
	DeleteObject(SelectObject(hDc, GetStockObject(BLACK_PEN)));
}

//Procedure for painting the palette window.
BOOL PalWndPaint(HWND hwnd, HDC hDc)
{
	PAINTSTRUCT ps;
	RECT rcWindow;
	GetClientRect(hwnd, &rcWindow);

	hDc=BeginPaint(hwnd, &ps);

	SCROLLINFO si;
	si.cbSize=sizeof(si);
	si.fMask=SIF_POS;
	GetScrollInfo(hwnd, SB_VERT, &si);
	int nYOffset=si.nPos;

	LAYER nLayer=LAYER_ERROR;
	nLayer=GetSelectedLayer(GetParent(hwnd));

	//Draw available tiles

	int x, y;//, nCurRef;
	DWORD nCurRef=0;

	if(nLayer==LAYER_TILE)
	{
		for(y=1, nCurRef=(nYOffset-1)*PALETTEWIDTH+1; nCurRef<=(int)ITileArchive.GetNumEntries(); y++)
		{
			for(x=1; x<=PALETTEWIDTH; x++, nCurRef++)
			{
				ITileArchive.StretchImageToDC(
					hDc, 
					nCurRef, 
					x*MAP_TILEDIM-MAP_TILEDIM, 
					y*MAP_TILEDIM-MAP_TILEDIM, 
					MAP_TILEDIM, 
					MAP_TILEDIM, 
					FALSE);

				//Now draw a rectangle around selected tile.
				if(ITileArchive.GetSelectedEntry()==nCurRef)
				{
					DrawPaletteCursor(hDc, x, y);
				}
			}
		}
	}
	else if(nLayer==LAYER_ARCH)
	{
		for(y=1, nCurRef=(nYOffset-1)*PALETTEWIDTH+1; nCurRef<=g_cArchLibrary.GetNumEntries(); y++)
		{
			for(x=1; x<=PALETTEWIDTH; x++, nCurRef++)
			{
				g_cArchLibrary.StretchImageToDC(
					hDc, 
					nCurRef, 
					x*MAP_TILEDIM-MAP_TILEDIM, 
					y*MAP_TILEDIM-MAP_TILEDIM,
					MAP_TILEDIM, 
					MAP_TILEDIM, 
					FALSE);
				
				if(g_cArchLibrary.GetSelectedEntry()==nCurRef)
				{
					DrawPaletteCursor(hDc, x, y);
				}
			}
		}
	}	

	
	//#define DRAWPALGRID
	#ifdef DRAWPALGRID
	HPEN hPen=NULL;
	hPen=CreatePen(PS_SOLID, 0, RGB(0, 0, 0));

	SelectObject(hDc, hPen);
	for(int i=0; i<rcWindow.right; i+=MAP_TILEDIM)
	{
		MoveToEx(hDc, i, 0, NULL);
		LineTo(hDc, i, rcWindow.bottom);
	}
	for(i=0; i<g_cTileLibrary.GetNumEntries()/PALETTEWIDTH; i++)
	{
		MoveToEx(hDc, 0, i*MAP_TILEDIM-MAP_TILEDIM, NULL);
		LineTo(hDc, rcWindow.right, i);
	}

	DeleteObject(SelectObject(hDc, GetStockObject(BLACK_PEN)));
	#endif //DRAWPALGRID

	
	EndPaint(hwnd, &ps);
	return TRUE;
}

//palette widnow procedure
LRESULT CALLBACK PalWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE:
		UpdatePalScroll(hwnd, MAP_TILEDIM, 1);
		break;
	case WM_DESTROY:
		break;
	case WM_SIZE:
	case WM_SIZING:
		UpdatePalScroll(hwnd, MAP_TILEDIM, NULL);
		break;
	case WM_VSCROLL:
	{
		SCROLLINFO si;
		si.cbSize=sizeof(si);
		si.fMask=SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &si);

		switch(LOWORD(wParam))
		{
		case SB_PAGEDOWN:
			si.nPos+=si.nPage/2;
			break;
		case SB_LINEDOWN:
			si.nPos++;
			break;
		case SB_PAGEUP:
			si.nPos-=si.nPage/2;
			break;
		case SB_LINEUP:
			si.nPos--;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			si.nPos=si.nTrackPos;
			break;
		case SB_TOP:
			si.nPos=si.nMin;
			break;
		case SB_BOTTOM:
			si.nPos=si.nMax;
			break;
		}
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
		RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);

		break;
	}
	case WM_LBUTTONDBLCLK:
	{
		POINT ps;
		ps.x = LOWORD(lParam);
		ps.y = HIWORD(lParam);
		WndPosToTile(hwnd, &ps, MAP_TILEDIM);

		SCROLLINFO si;
		si.cbSize=sizeof(si);
		si.fMask=SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &si);
		int nYOffset=si.nPos;

		LAYER nLayer=GetSelectedLayer(GetParent(hwnd));
	
		int nNewEntry=ps.x+PALETTEWIDTH*(ps.y-1)+(nYOffset-1)*PALETTEWIDTH;
		
		if(nLayer==LAYER_TILE)
		{
			if(nNewEntry>(int)ITileArchive.GetNumEntries())
				nNewEntry=ITileArchive.GetSelectedEntry();
			ITileArchive.SetSelectedEntry(nNewEntry); 
		}
		else if(nLayer==LAYER_ARCH)
		{
			if(nNewEntry>(int)g_cArchLibrary.GetNumEntries())
				nNewEntry=g_cArchLibrary.GetSelectedEntry();
			g_cArchLibrary.SetSelectedEntry(nNewEntry);
		}
		
		RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE);

		break;
	}
	case WM_PAINT:
		PalWndPaint(hwnd, (HDC)wParam);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0l;
}

//Procedure for painting the edit window
BOOL EditWndPaint(HWND hwnd, HDC hDc)
{
	PAINTSTRUCT ps;
	RECT rcWindow;
	MENUITEMINFO mi;
	GetClientRect(hwnd, &rcWindow);
	
	hDc=BeginPaint(hwnd, &ps);

	//Get the offset values.
	POINT psOffset;
	SCROLLINFO si;
	si.cbSize=sizeof(si);
	si.fMask=SIF_POS;
	
	GetScrollInfo(hwnd, SB_VERT, &si);
	psOffset.y=si.nPos;
	
	GetScrollInfo(hwnd, SB_HORZ, &si);
	psOffset.x=si.nPos;

	
	//The following double loops will also map architecture if architecture is
	//checked.  When architecture is selected, both Tiles and Arch are displayed.

	LAYER nLayer=GetSelectedLayer(GetParent(hwnd));

	int nXStart, nYStart, nXEnd, nYEnd;

	//we determine where the drawing should start and end to avoid
	//drawing offscreen (which will just waste time)
	nXStart=psOffset.x/MAP_TILEDIM;
	nYStart=psOffset.y/MAP_TILEDIM;
	nXEnd=nXStart+rcWindow.right/MAP_TILEDIM+MAP_TILEDIM;//g_cEditMapboard.GetMapWidth();
	nYEnd=nYStart+rcWindow.bottom/MAP_TILEDIM+MAP_TILEDIM;//g_cEditMapboard.GetMapHeight();

	if(nXEnd>g_cEditMapboard.GetMapWidth())
		nXEnd=g_cEditMapboard.GetMapWidth();
	if(nYEnd>g_cEditMapboard.GetMapHeight())
		nYEnd=g_cEditMapboard.GetMapHeight();
	
	for(int x=nXStart; x<=nXEnd; x++)
	{
		for(int y=nYStart; y<=nYEnd; y++)
		{
			
			//These aer the coordinates where the specified tile/arch/object should be painted
			const int nXPaint=(x*MAP_TILEDIM-MAP_TILEDIM-psOffset.x);
			const int nYPaint=(y*MAP_TILEDIM-MAP_TILEDIM-psOffset.y);
			
			//we check to see if we should paint with transparency
			BOOL bTransp=FALSE;
			
			mi.cbSize=sizeof(mi);
			mi.fMask=MIIM_STATE;
			GetMenuItemInfo(GetMenu(GetParent(hwnd)), ID_VIEWUSETRANSPARENT, FALSE, &mi);

			
			if((mi.fState&MFS_CHECKED)==MFS_CHECKED)
			{
				bTransp=TRUE;
			}
			else 
				bTransp=FALSE;

			//we always paint the map
			ITileArchive.StretchImageToDC(
				hDc, 
				g_cEditMapboard.GetTile(x, y), 
				nXPaint,
				nYPaint,
				MAP_TILEDIM,
				MAP_TILEDIM,
				bTransp);

			switch(nLayer)
			{
			case LAYER_OBJECT:
				break;
			case LAYER_ARCH:
				g_cArchLibrary.StretchImageToDC(
					hDc, 
					g_cEditMapboard.GetArchSmart(x, y), 
					nXPaint,
					nYPaint,
					MAP_TILEDIM,
					MAP_TILEDIM, 
					TRUE);

				break;

			case LAYER_TILE:
				break;
			}
		}
	}

	//If the Show Grid is checked on the view menu then show the grid
	mi.cbSize=sizeof(mi);
	mi.fMask=MIIM_STATE;
	GetMenuItemInfo(GetMenu(GetParent(hwnd)), ID_VIEWSHOWGRID, FALSE, &mi);

	if((mi.fState&MFS_CHECKED)==MFS_CHECKED)
	{
		for(int i=0; i<=g_cEditMapboard.GetMapWidth()*MAP_TILEDIM; i+=MAP_TILEDIM)
		{
			MoveToEx(hDc, i-psOffset.x, 0, NULL);
			LineTo(hDc, i-psOffset.x, g_cEditMapboard.GetMapHeight()*MAP_TILEDIM);
		}
		for(int i=0; i<=g_cEditMapboard.GetMapHeight()*MAP_TILEDIM; i+=MAP_TILEDIM)
		{
			MoveToEx(hDc, 0, i-psOffset.y, NULL);
			LineTo(hDc, g_cEditMapboard.GetMapWidth()*MAP_TILEDIM, i-psOffset.y);
		}
	}
	
	EndPaint(hwnd, &ps);

	return TRUE;
}

//Procedure for edit window.
LRESULT CALLBACK EditWndProc(
	HWND hwnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam)
{
	SCROLLINFO siPrevInfo;
	#define MOVEDIST (MAP_TILEDIM/2)
	
	switch(msg)
	{
	case WM_CREATE:
		UpdateEditScroll(hwnd, MAP_TILEDIM, 0, 0);
		break;
	case WM_DESTROY:
		break;
	case WM_VSCROLL:
	{
		SCROLLINFO si;
		si.cbSize=sizeof(si);
		si.fMask=SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &si);
		if(si.nPage>(UINT)si.nMax)break;
		//Save previous info
		siPrevInfo=si;

		switch(LOWORD(wParam)){
		case SB_PAGEDOWN:
			si.nPos+=si.nPage/2;break;
		case SB_LINEDOWN:
			si.nPos+=MOVEDIST;break;
		case SB_PAGEUP:
			si.nPos-=si.nPage/2;break;
		case SB_LINEUP:
			si.nPos-=MOVEDIST;break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			si.nPos=si.nTrackPos;break;
		case SB_TOP:
			si.nPos=si.nMin;break;
		case SB_BOTTOM:
			si.nPos=si.nMax;break;
		}
		if(si.nPos<0)si.nPos=0;
		if(si.nPos>(int)(si.nMax-si.nPage+1))si.nPos=si.nMax-si.nPage+1;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
		//Check to see if info has changed, if it has scroll the window.
		if(si.nPos!=siPrevInfo.nPos)
		{
			ScrollWindowEx(
				hwnd, 
				0, 
				siPrevInfo.nPos-si.nPos, 
				NULL, 
				NULL, 
				NULL, 
				NULL, 
				SW_INVALIDATE|SW_ERASE);	
		}
		break;
	}
	case WM_HSCROLL:
	{
		SCROLLINFO si;
		si.cbSize=sizeof(si);
		si.fMask=SIF_ALL;
		GetScrollInfo(hwnd, SB_HORZ, &si);
		if(si.nPage>(UINT)si.nMax)break;
		//Save the info in the prev info thing.
		siPrevInfo=si;

		switch(LOWORD(wParam)){
		case SB_PAGERIGHT:
			si.nPos+=si.nPage/2;break;
		case SB_LINERIGHT: 
			si.nPos+=MOVEDIST;break;
		case SB_PAGELEFT:
			si.nPos-=si.nPage/2;break;
		case SB_LINELEFT:
			si.nPos-=MOVEDIST;break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			si.nPos=si.nTrackPos;break;
		case SB_LEFT:
			si.nPos=si.nMin;break;
		case SB_RIGHT:
			si.nPos=si.nMax;break;
		}
		if(si.nPos<0)si.nPos=0;
		if(si.nPos>(int)(si.nMax-si.nPage+1))
			si.nPos=si.nMax-si.nPage+1;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
			
		//Check to see if info has change
		if(si.nPos!=siPrevInfo.nPos)
		{
			ScrollWindowEx(
				hwnd, 
				siPrevInfo.nPos-si.nPos, 
				0, 
				NULL, 
				NULL, 
				NULL, 
				NULL, 
				SW_INVALIDATE|SW_ERASE);
		}
		break;
	}
	case WM_SIZE:
	case WM_SIZING:
		UpdateEditScroll(hwnd, MAP_TILEDIM, -1, -1);
		break;
	case WM_MOUSEMOVE:
		//If the mouse button is held down
		//and we drag, fall through to pressing the left button
		if(wParam==MK_LBUTTON){/*Fall through*/}
		else if(wParam==MK_RBUTTON){SendMessage(hwnd, WM_RBUTTONDOWN, wParam, lParam);break;}
		else break;
	case WM_LBUTTONDOWN:
	{
		POINT ps;
		ps.x = LOWORD(lParam);
		ps.y = HIWORD(lParam);
		

		POINT psOffset;
		SCROLLINFO si;
		si.cbSize=sizeof(si);
		si.fMask=SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &si);
		psOffset.y=si.nPos;
		GetScrollInfo(hwnd, SB_HORZ, &si);
		psOffset.x=si.nPos;

		ps.x+=psOffset.x;
		ps.y+=psOffset.y;
		WndPosToTile(hwnd, &ps, MAP_TILEDIM);


		LAYER nLayer=GetSelectedLayer(GetParent(hwnd));
		switch(nLayer){
		case LAYER_TILE:
			if(g_cEditMapboard.SetTile(ps.x, ps.y, (BYTE)ITileArchive.GetSelectedEntry()))
			{
				//Set update region and have the WM_PAINT Redraw
				RECT rect;
				rect.left=ps.x*MAP_TILEDIM-MAP_TILEDIM-psOffset.x;
				rect.right=ps.x*MAP_TILEDIM-psOffset.x;
				rect.top=ps.y*MAP_TILEDIM-MAP_TILEDIM-psOffset.y;
				rect.bottom=ps.y*MAP_TILEDIM-psOffset.y;
				RedrawWindow(hwnd, &rect, NULL, RDW_INVALIDATE|RDW_ERASE);
				g_bQueryForSave=TRUE;
			}
			break;
		case LAYER_ARCH:
			if(g_cEditMapboard.SetArchSmart(ps.x, ps.y, (BYTE)g_cArchLibrary.GetSelectedEntry()))
			{
				//Set update region and have the WM_PAINT Redraw
				RECT rect;
				rect.left=ps.x*MAP_TILEDIM-MAP_TILEDIM-psOffset.x;
				rect.right=ps.x*MAP_TILEDIM-psOffset.x;
				rect.top=ps.y*MAP_TILEDIM-MAP_TILEDIM-psOffset.y;
				rect.bottom=ps.y*MAP_TILEDIM-psOffset.y;
				RedrawWindow(hwnd, &rect, NULL, RDW_INVALIDATE|RDW_ERASE);
				g_bQueryForSave=TRUE;
			}
			break;
		case LAYER_OBJECT:
			break;
		default:
			break;
		}

		break;
	}
	case WM_RBUTTONDOWN:
	{
		POINT ps;
		ps.x = LOWORD(lParam);
		ps.y = HIWORD(lParam);
		
		POINT psOffset;
		SCROLLINFO si;
		si.cbSize=sizeof(si);
		si.fMask=SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &si);
		psOffset.y=si.nPos;
		GetScrollInfo(hwnd, SB_HORZ, &si);
		psOffset.x=si.nPos;

		ps.x+=psOffset.x;
		ps.y+=psOffset.y;
		WndPosToTile(hwnd, &ps, MAP_TILEDIM);

		LAYER nLayer=GetSelectedLayer(GetParent(hwnd));

		switch(nLayer){
		case LAYER_TILE:
			if(g_cEditMapboard.SetTile(ps.x, ps.y, 0))
			{
				RECT rect;
				rect.left=ps.x*MAP_TILEDIM-MAP_TILEDIM-psOffset.x;
				rect.right=ps.x*MAP_TILEDIM-psOffset.x;
				rect.top=ps.y*MAP_TILEDIM-MAP_TILEDIM-psOffset.y;
				rect.bottom=ps.y*MAP_TILEDIM-psOffset.y;
				RedrawWindow(hwnd, &rect, NULL, RDW_INVALIDATE|RDW_ERASE);
				g_bQueryForSave=TRUE;
			}

			break;
			
		case LAYER_ARCH:
			if(g_cEditMapboard.SetArch(ps.x, ps.y, 0x00))
			{
				RECT rect;
				rect.left=ps.x*MAP_TILEDIM-MAP_TILEDIM-psOffset.x;
				rect.right=ps.x*MAP_TILEDIM-psOffset.x;
				rect.top=ps.y*MAP_TILEDIM-MAP_TILEDIM-psOffset.y;
				rect.bottom=ps.y*MAP_TILEDIM-psOffset.y;
				RedrawWindow(hwnd, &rect, NULL, RDW_INVALIDATE|RDW_ERASE);
				g_bQueryForSave=TRUE;
			}
			break;
		case LAYER_OBJECT:
			break;
		}
		
		break;		
	}
	case WM_PAINT:
		EditWndPaint(hwnd, (HDC)wParam);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0l;
}

//Procedure for loading a map and then the library.
BOOL LoadMap(
	HWND hwnd, 
	HWND hwndPal, 
	HWND hwndEdit, 
	LPTSTR szFilename, 
	TCHAR szCurrentMap[MAX_PATH])
{
	TCHAR szWindowText[MAX_PATH+14];
	if(g_cEditMapboard.LoadMap(szFilename))
	{
		UpdateEditScroll(hwndEdit, MAP_TILEDIM, 0, 0);

		_tcscpy_s(szCurrentMap, MAX_PATH, szFilename);
		_stprintf_s(szWindowText, countof(szWindowText), TEXT("ScrollEdit [%s]"), szCurrentMap);
		SetWindowText(hwnd, szWindowText);

		const TCHAR* szLibraryName = g_cEditMapboard.GetLibraryName();

		if(!(ITileArchive.LoadArchive(szLibraryName)))
		{
			MessageBox(
				hwnd, 
				TEXT("Could not load map's library!"), 
				TEXT("ScrollEdit"), 
				MB_OK|MB_ICONERROR);
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE);

			return FALSE;
		}
		UpdatePalScroll(hwndPal, MAP_TILEDIM, 1);
		RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE);
	}else{
		MessageBox(
			hwnd, 
			TEXT("Failed to open map!"), 
			TEXT("ScrollEdit"), 
			MB_OK|MB_ICONERROR);

		return FALSE;
	}
	return TRUE;
}

//Procedure for main window's commands.
BOOL MainCommandProc(
	HWND hwnd, 
	HWND hwndPal, 
	HWND hwndEdit, 
	WORD wNotifyCode, 
	WORD wID, 
	HWND hwndCtl, 
	TCHAR szCurrentMap[MAX_PATH])
{
	TCHAR szWindowText[MAX_PATH+14];
	switch(wID)
	{
	//FILE menu.
	//FILE|NEW...
	case ID_FILENEW:
	{
		if(g_bQueryForSave)
		{
			if(QueryForSave(hwnd, szCurrentMap))
			{
				g_bQueryForSave=FALSE;
			}
			else 
				break;
		}
		
		if(DialogBox(
			GetModuleHandle(NULL), 
			MAKEINTRESOURCE(IDD_NEWDIALOG), 
			hwnd, 
			NewDlgProc)==0)
		{
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE);
			UpdateEditScroll(hwndEdit, MAP_TILEDIM, 0, 0);
			UpdatePalScroll(hwndPal, MAP_TILEDIM, 0);
			szCurrentMap[0]=NULL;
			_stprintf_s(szWindowText, countof(szWindowText), TEXT("ScrollEdit [%s]"), szCurrentMap);
			SetWindowText(hwnd, szWindowText);
			MessageBox(hwnd, TEXT("New map successfully genereated"), TEXT("ScrollEdit"), MB_OK|MB_ICONINFORMATION);
			g_bQueryForSave=TRUE;
		}
		break;
	}
	//FILE|OPEN...
	case ID_FILEOPEN:
	{
		if(g_bQueryForSave)
		{
			if(QueryForSave(hwnd, szCurrentMap))
			{
				g_bQueryForSave=FALSE;
			}
			else 
				break;
		}

		TCHAR szOpenFilename[MAX_PATH];
		_tcscpy_s(szOpenFilename, countof(szOpenFilename), TEXT("Map name"));
		if(GetOpenFilename(
			TEXT("Open map..."), 
			TEXT("ScrollGIN Map (*.map)\0*.map\0All Files (*.*)\0*.*\0"), 
			hwnd, 
			szOpenFilename))
		{
			if(LoadMap(hwnd, hwndPal, hwndEdit, szOpenFilename, szCurrentMap))
			{
				SaveRegFilename(szOpenFilename);
				g_bQueryForSave=FALSE;
			}
		}
		break;
	}
	//FILE|SAVE AS...
	case ID_FILESAVEAS:
	{
		TCHAR szSaveFilename[MAX_PATH];
		_tcscpy_s(szSaveFilename, countof(szSaveFilename), szCurrentMap);
		if(GetSaveFilename(
			TEXT("Save map..."), 
			TEXT("ScrollGIN Map (*.map)\0*.map\0All Files (*.*)\0*.*\0"), 
			TEXT("map"), 
			hwnd, 
			szSaveFilename))
		{
			_tcscpy_s(szCurrentMap, MAX_PATH, szSaveFilename);
		}
		else 
			break;
	}
	//Fall through and save.
	//FILE|SAVE...
	case ID_FILESAVE:
		//If there is currently no file loaded save as is called.
		if(szCurrentMap[0]==NULL)
		{
			SendMessage(
				hwnd, 
				WM_COMMAND, 
				ID_FILESAVEAS, 
				NULL);

			_stprintf_s(szWindowText, countof(szWindowText), TEXT("ScrollEdit [%s]"), szCurrentMap);

			SetWindowText(hwnd, szWindowText);
			break;
		}

		if((g_cEditMapboard.SaveMap(szCurrentMap)))
		{
			SaveRegFilename(szCurrentMap);
			_stprintf_s(szWindowText, countof(szWindowText), TEXT("ScrollEdit [%s]"), szCurrentMap);
			SetWindowText(hwnd, szWindowText);
			g_bQueryForSave=FALSE;
			MessageBox(
				hwnd, 
				TEXT("Successfully saved map!"), 
				TEXT("ScrollEdit"), 
				MB_OK|MB_ICONINFORMATION);
		}
		else
			MessageBox(
				hwnd, 
				TEXT("An error occured while saving!"), 
				TEXT("ScrollEdit"), 
				MB_OK|MB_ICONERROR);

		break;
	//FILE|EXIT...
	case ID_FILEEXIT:
		PostMessage(hwnd, WM_CLOSE, NULL, NULL);
		break;
	//EDIT menu.
	//EDIT|MAP DIMESNIONS...
	case ID_EDITMAPDIM:
		if(DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MAPDIMS), hwnd, DimDlgProc))
		{
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE);
			UpdateEditScroll(hwndEdit, MAP_TILEDIM, 0, 0);
			UpdatePalScroll(hwndPal, MAP_TILEDIM, 0);
			g_bQueryForSave=TRUE;
		}
		break;
	//EDIT|LIBRARY...
	case ID_EDITLIBRARY:
	{
		if(DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CHANGEFILENAME), hwnd, LibDlgProc))
		{
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE);
			UpdateEditScroll(hwndEdit, MAP_TILEDIM, 0, 0);
			UpdatePalScroll(hwndPal, MAP_TILEDIM, 0);
			g_bQueryForSave=TRUE;
		}
		break;
	}
	//EDIT|BACKGROUND...
	case ID_EDITBG:
	{
		if(DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CHANGEFILENAME), hwnd, BGDlgProc))
		{
			g_bQueryForSave=TRUE;
		}
		break;
	}
	//EDIT|MAP STATISTICS...
	case ID_EDITSTATS:
		DialogBox(
			GetModuleHandle(NULL), 
			MAKEINTRESOURCE(IDD_MAPSTATS), 
			hwnd, 
			StatsDlgProc);
		break;
	//LAYER menu.
	//LAYER|TILE
	case ID_LAYERTILE:
		SetSelectedLayer(hwnd, hwndPal, LAYER_TILE);
		break;
	//LAYER|ARCHICTECTURE
	case ID_LAYERARCH:
		SetSelectedLayer(hwnd, hwndPal, LAYER_ARCH);
		break;
	//LAYER|OBJECT
	case ID_LAYEROBJECT:
		SetSelectedLayer(hwnd, hwndPal, LAYER_OBJECT);
		break;
	//LAYER|CLEAR LAYER
	case ID_LAYERCLEARLAYER:
	{
		if(MessageBox(
			hwnd, 
			TEXT("Are you sure you want to clear the current layer?"), 
			TEXT("Warning"), 
			MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
		{
			switch(GetSelectedLayer(hwnd))
			{
			case LAYER_TILE:
				g_cEditMapboard.ClearTile();
				break;
			case LAYER_ARCH:
				g_cEditMapboard.ClearArch();
				break;
			case LAYER_OBJECT:
				g_cEditMapboard.ClearObject();
				break;
			default:
				break;
			}
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE);
			g_bQueryForSave=TRUE;
		}
		break;
	}
	//VIEW menu.
	//VIEW|SHOW GRID
	case ID_VIEWSHOWGRID:
	{
		//We need to change the state of the check to 
		//opposite of what it was before, the Edit Window
		//Paint function will take care of everything else.
		MENUITEMINFO mi;
		mi.cbSize=sizeof(mi);
		mi.fMask=MIIM_STATE;
		GetMenuItemInfo(
			GetMenu(hwnd), 
			ID_VIEWSHOWGRID, 
			FALSE, 
			&mi);

		//Change the state of the menu check.
		mi.fState = 
			((mi.fState&MFS_CHECKED)==MFS_CHECKED) 
			? mi.fState=mi.fState&(~MFS_CHECKED) 
			: mi.fState=mi.fState|MFS_CHECKED;

		
		SetMenuItemInfo(GetMenu(hwnd), ID_VIEWSHOWGRID, FALSE, &mi);
		RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);

		break;
	}
	//VIEW|SHOW TRANSPARENCY
	case ID_VIEWUSETRANSPARENT:
	{
		MENUITEMINFO mi;
		mi.cbSize=sizeof(mi);
		mi.fMask=MIIM_STATE;
		GetMenuItemInfo(GetMenu(hwnd), ID_VIEWUSETRANSPARENT, FALSE, &mi);
		
		//Change the state of the menu check.
		mi.fState = 
			((mi.fState&MFS_CHECKED)==MFS_CHECKED) 
			? mi.fState=mi.fState&(~MFS_CHECKED) 
			: mi.fState=mi.fState|MFS_CHECKED;

		SetMenuItemInfo(GetMenu(hwnd), ID_VIEWUSETRANSPARENT, FALSE, &mi);
		RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
		
		break;
	}
	//The help menu
	case ID_HELPTOPICS:
		HtmlHelp(
			hwnd, 
			TEXT("ScrollEditHelp.chm"), 
			HH_DISPLAY_TOPIC, 
			NULL);

		break;
	case ID_HELPABOUT:
		DialogBox(
			GetModuleHandle(NULL), 
			MAKEINTRESOURCE(IDD_ABOUTDLG), 
			hwnd, 
			AboutProc);

		break;
	default: 
		return FALSE;
	}
	return TRUE;
}



//Procedure for the main window.
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hPalWnd=NULL, hEditWnd=NULL;
	static TCHAR szCurrentMap[MAX_PATH];

	switch(msg)
	{
	case WM_KEYDOWN:
		switch((int)wParam)
		{		
		case VK_DOWN:SendMessage(hEditWnd, WM_VSCROLL, SB_LINEDOWN, NULL);break;
		case VK_UP:SendMessage(hEditWnd, WM_VSCROLL, SB_LINEUP, NULL);break;
		case VK_LEFT:SendMessage(hEditWnd, WM_HSCROLL, SB_LINELEFT, NULL);break;
		case VK_RIGHT:SendMessage(hEditWnd, WM_HSCROLL, SB_LINERIGHT, NULL);break;
		default:break;
		}
		break;

	case WM_COMMAND:
		MainCommandProc(hwnd, hPalWnd, hEditWnd, HIWORD(wParam), LOWORD(wParam), (HWND)lParam, szCurrentMap);
		break;

	case WM_SIZING:
	case WM_SIZE:
	{
		RECT rcMain;
		GetClientRect(hwnd, &rcMain);
		MoveWindow(hPalWnd, 0, 0, PALETTEDIM, rcMain.bottom, TRUE);
		MoveWindow(hEditWnd, PALETTEDIM, 0, rcMain.right-PALETTEDIM, rcMain.bottom, TRUE);
		break;
	}

	case WM_CREATE:
	{
		const TCHAR szPalWndName[]=TEXT("PalWnd");
		const TCHAR szEditWndName[]=TEXT("EditWnd");
		szCurrentMap[0]=NULL;
		
		RECT rcMain;
		GetClientRect(hwnd, &rcMain);
		
		WNDCLASSEX wc;

		//Create palette window.
		wc.cbSize=sizeof(wc);
		wc.cbClsExtra=0;
		wc.cbWndExtra=0;
		wc.hIcon=NULL;
		wc.hIconSm=NULL;
		wc.hInstance=((LPCREATESTRUCT)lParam)->hInstance;
		wc.lpszClassName=szPalWndName;
		wc.lpfnWndProc=PalWndProc;
		wc.hbrBackground=(HBRUSH)GetStockObject(LTGRAY_BRUSH);
		wc.hCursor=LoadCursor(NULL, IDC_ARROW);
		wc.lpszMenuName=NULL;
		wc.style=CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;

		RegisterClassEx(&wc);

		hPalWnd=CreateWindowEx(
			NULL, 
			szPalWndName, 
			szPalWndName, 
			WS_CHILD|WS_VSCROLL,
			rcMain.left, 
			rcMain.top, 
			PALETTEDIM, 
			rcMain.bottom, 
			hwnd, 
			(HMENU)ID_PALETTE_WINDOW, 
			((LPCREATESTRUCT)lParam)->hInstance, 
			NULL);


			//Create edit window
		wc.lpszClassName=szEditWndName;
		wc.lpfnWndProc=EditWndProc;
		wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
		wc.hCursor=LoadCursor(NULL, IDC_ARROW);
		wc.lpszMenuName=NULL;
		wc.style=CS_HREDRAW|CS_VREDRAW;

		RegisterClassEx(&wc);

		hEditWnd=CreateWindowEx(
			NULL, 
			szEditWndName, 
			szEditWndName, 
			WS_CHILD|WS_HSCROLL|WS_VSCROLL,
			PALETTEDIM, 
			0, 
			rcMain.right-PALETTEDIM, 
			rcMain.bottom, 
			hwnd, 
			(HMENU)ID_EDIT_WINDOW, 
			((LPCREATESTRUCT)lParam)->hInstance, 
			NULL);

		ShowWindow(hPalWnd, SW_SHOWNORMAL);
		ShowWindow(hEditWnd, SW_SHOWNORMAL);

		//Now that everything is created lets check to see which map was last used.
		TCHAR szTempFilename[MAX_PATH];
		if(!GetRegFilename(szTempFilename))
		{
			//Generate a map with default settings.
			g_cEditMapboard.GenerateNewMap(
				16, 
				12, 
				TEXT(""), 
				NULL);
		}
		else
		{
			//Attempt to load the last saved map, if it fails generate a default map.
			if(!LoadMap(hwnd, hPalWnd, hEditWnd, szTempFilename, szCurrentMap))
				g_cEditMapboard.GenerateNewMap(16, 12, TEXT(""), NULL);
		}
		break;
	}
	case WM_CLOSE:
	{
		if(g_bQueryForSave)
		{
			if(QueryForSave(hwnd, szCurrentMap))
			{
				SaveRegFilename(szCurrentMap);
				DestroyWindow(hwnd);
			}
		}
		else 
			DestroyWindow(hwnd);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0l;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
	HWND hMainWnd=NULL;
	MSG msg;
	HACCEL hAccel;

	const TCHAR szMainWndName[]=TEXT("ScrollEdit");
	
	//Register and create the main window.  The main window will create
	//the palette and edit windows on it's creation.
	WNDCLASSEX wc;
	wc.cbClsExtra=0;
	wc.cbSize=sizeof(wc);
	wc.cbWndExtra=0;
	wc.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);
	wc.hIcon=LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONMAIN));
	wc.hIconSm=NULL;
	wc.hInstance=hInstance;
	wc.lpfnWndProc=MainWndProc;
	wc.lpszClassName=szMainWndName;
	wc.style=NULL;//CS_HREDRAW|CS_VREDRAW;
	wc.lpszMenuName=MAKEINTRESOURCE(IDR_MAINMENU);

	if(!RegisterClassEx(&wc))
	{
		//This will fail if running unicode version under a non NT environment.
		MessageBox(
			NULL, 
			TEXT("This program requires Windows NT!"), 
			szMainWndName, 
			MB_ICONERROR|MB_OK);

		return 0; 
	}
	
	hMainWnd=CreateWindowEx(
		NULL, 
		szMainWndName, 
		szMainWndName, 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		640+GetSystemMetrics(SM_CXVSCROLL)+PALETTEDIM+GetSystemMetrics(SM_CXFRAME)*2, 
		480+GetSystemMetrics(SM_CYHSCROLL)+GetSystemMetrics(SM_CYMENU)+GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYFRAME)*2,
		NULL, 
		NULL, 
		hInstance, 
		NULL);
	
	//If for some reason the main window failed to be created we bail.
	if(hMainWnd==NULL)
		return 0;

	ShowWindow(hMainWnd, nShowCmd);
	
	hAccel=LoadAccelerators(
		hInstance, 
		MAKEINTRESOURCE(IDR_ACCELERATOR));

	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(!TranslateAccelerator(msg.hwnd, hAccel, &msg))
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}