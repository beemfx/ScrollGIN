/*
	ImageEdit - ImageLibrary Utility

	Copyright (c) 2003, Blaine Myers
*/

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "../ImageLib/ImageLib.h"
#include "DiskDlg.h"
#include "resource.h"


#ifdef UNICODE
#define VERSIONTEXT TEXT("version 2.xx  UNICODE")
#else //unicode
#define VERSIONTEXT TEXT("version 2.xx  ANSI")
#endif //unicode

#define LIBRARYVERSION TEXT("ImageLibrary version 2.00")

typedef enum tagPRIORITY{P_LOW=0, P_MEDIUM, P_HIGH}PRIORITY;
typedef enum tagERRORLEVEL{EL_SUCCESS=0, EL_FAIL}ERRORLEVEL;

CEditImageLibrary ILibrary(256);

int GetScrollPosEx(HWND hwndScrollBar, int nBar){
	SCROLLINFO si;
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize=sizeof(SCROLLINFO);
	si.fMask=SIF_POS;
	GetScrollInfo(hwndScrollBar, nBar, &si);
	return si.nPos;
}



BOOL NotifyUser(HWND hwnd, TCHAR szMessage[], PRIORITY nPriority, ERRORLEVEL nErrorLevel){
	switch(nPriority)
	{
	case P_LOW:
		SetDlgItemText(hwnd, IDC_ERRORSTRING, szMessage);
		break;
	case P_MEDIUM:
		if(nErrorLevel==EL_FAIL){
			PlaySound(TEXT("SystemDefaultBeep"), NULL, SND_ASYNC|SND_ALIAS);
			SetDlgItemText(hwnd, IDC_ERRORSTRING, szMessage);
		}else{
			PlaySound(TEXT("SystemAsterisk"), NULL, SND_ASYNC|SND_ALIAS);
			SetDlgItemText(hwnd, IDC_ERRORSTRING, szMessage);
		}
		break;
	case P_HIGH:
		if(nErrorLevel==EL_FAIL){
			SetDlgItemText(hwnd, IDC_ERRORSTRING, szMessage);
			MessageBox(hwnd, szMessage, TEXT("ImageEdit"), MB_OK|MB_ICONERROR);
		}else{
			SetDlgItemText(hwnd, IDC_ERRORSTRING, szMessage);
			MessageBox(hwnd, szMessage, TEXT("ImageEdit"), MB_OK|MB_ICONINFORMATION);
		}
		break;
	}
	return TRUE;
}

BOOL EditUpdate(HWND hwnd){

	if(hwnd==NULL)return FALSE;

	DWORD dwCurrentImage=GetScrollPosEx(GetDlgItem(hwnd, IDC_IMAGESCROLL), SB_CTL);

	if((dwCurrentImage<1)||(dwCurrentImage>ILibrary.GetNumEntries())||(ILibrary.GetNumEntries()<1)){

		SetDlgItemInt(hwnd, IDC_ENTRYNUMBER, 0, FALSE);
		SetDlgItemText(hwnd, IDC_BITMAPNAME, TEXT(""));
		
		SetDlgItemText(hwnd, IDC_LABEL, TEXT(""));
		
		SetDlgItemInt(hwnd, IDC_BMPNUMBER, 0, FALSE);
		SetDlgItemInt(hwnd, IDC_X, 0, FALSE);
		SetDlgItemInt(hwnd, IDC_Y, 0 , FALSE);
		SetDlgItemInt(hwnd, IDC_SRCWIDTH, 0, FALSE);
		SetDlgItemInt(hwnd, IDC_SRCHEIGHT, 0, FALSE);
		SetDlgItemInt(hwnd, IDC_WIDTH, 0, FALSE);
		SetDlgItemInt(hwnd, IDC_HEIGHT, 0, FALSE);
		SetDlgItemInt(hwnd, IDC_FRAMES, 0, FALSE);
		
	}else{
		//set values to currently selected image
		TCHAR szTemp[MAX_PATH];

		IMAGEDATA id;
		if(!ILibrary.GetImageData(dwCurrentImage, &id))return FALSE;

		SetDlgItemInt(hwnd, IDC_ENTRYNUMBER, dwCurrentImage, FALSE);

		ILibrary.GetBitmapName(szTemp, countof(szTemp), id.nBitmap);
		SetDlgItemText(hwnd, IDC_BITMAPNAME, szTemp);
		
		ILibrary.GetImageName(szTemp, countof(szTemp), dwCurrentImage);
		SetDlgItemText(hwnd, IDC_LABEL, szTemp);
		
		SetDlgItemInt(hwnd, IDC_BMPNUMBER, id.nBitmap, FALSE);
		SetDlgItemInt(hwnd, IDC_X, id.nX, FALSE);
		SetDlgItemInt(hwnd, IDC_Y, id.nY , FALSE);
		SetDlgItemInt(hwnd, IDC_SRCWIDTH, id.nWidthSrc, FALSE);
		SetDlgItemInt(hwnd, IDC_SRCHEIGHT, id.nHeightSrc, FALSE);
		SetDlgItemInt(hwnd, IDC_WIDTH, id.nWidth, FALSE);
		SetDlgItemInt(hwnd, IDC_HEIGHT, id.nHeight, FALSE);
		SetDlgItemInt(hwnd, IDC_FRAMES, id.nFrames, FALSE);
		
	}
	return TRUE;
}

BOOL SetValues(HWND hwnd){
	DWORD dwCurrentImage=GetScrollPosEx(GetDlgItem(hwnd, IDC_IMAGESCROLL), SB_CTL);

	if((dwCurrentImage<1)||(dwCurrentImage>ILibrary.GetNumEntries())||(ILibrary.GetNumEntries()<1)){
		return FALSE;
	}else{
		//attempt to apply the current values
		IMAGEDATA id;
		ZeroMemory(&id, sizeof(IMAGEDATA));

		TCHAR szTemp[IMAGE_NAME_LENGTH];
		BOOL bTranslated=FALSE;

		id.nBitmap=GetDlgItemInt(hwnd, IDC_BMPNUMBER, &bTranslated, FALSE);
		if(!bTranslated)return FALSE;
		id.nX=GetDlgItemInt(hwnd, IDC_X, &bTranslated, FALSE);
		if(!bTranslated)return FALSE;
		id.nY=GetDlgItemInt(hwnd, IDC_Y, &bTranslated, FALSE);
		if(!bTranslated)return FALSE;
		id.nWidthSrc=GetDlgItemInt(hwnd, IDC_SRCWIDTH, &bTranslated, FALSE);
		if(!bTranslated)return FALSE;
		id.nHeightSrc=GetDlgItemInt(hwnd, IDC_SRCHEIGHT, &bTranslated, FALSE);
		if(!bTranslated)return FALSE;
		id.nWidth=GetDlgItemInt(hwnd, IDC_WIDTH, &bTranslated, FALSE);
		if(!bTranslated)return FALSE;
		id.nHeight=GetDlgItemInt(hwnd, IDC_HEIGHT, &bTranslated, FALSE);
		if(!bTranslated)return FALSE;
		id.nFrames=GetDlgItemInt(hwnd, IDC_FRAMES, &bTranslated, FALSE);
		if(!bTranslated)return FALSE;

		GetDlgItemText(hwnd, IDC_LABEL, szTemp, IMAGE_NAME_LENGTH);

		if(!(ILibrary.SetEntry(
			dwCurrentImage,
			id.nX,
			id.nY,
			id.nWidthSrc,
			id.nHeightSrc,
			id.nWidth,
			id.nHeight,
			id.nBitmap,
			id.nFrames,
			szTemp)))return FALSE;
	}
	return TRUE;
}

BOOL UpdateEditScroll(HWND hwndSlider){
	SCROLLINFO si;
	ZeroMemory(&si, sizeof(SCROLLINFO));
	if( (ILibrary.GetNumEntries()<1)){
		si.fMask=SIF_ALL;
		si.cbSize=sizeof(SCROLLINFO);
		si.nMax=0;
		si.nMin=0;
		si.nPage=0;
		si.nPos=0;
		si.nTrackPos=0;
		SetScrollInfo(hwndSlider, SB_CTL, &si, TRUE);
	}else{
		si.fMask=SIF_ALL;
		si.cbSize=sizeof(SCROLLINFO);
		si.nMax=ILibrary.GetNumEntries();
		si.nMin=1;
		si.nPos=1;
		si.nPage=1;
		si.nTrackPos=1;
		SetScrollInfo(hwndSlider, SB_CTL, &si, TRUE);
	}
	return TRUE;
}

BOOL EditScrollProc(int nScrollCode, short int nPos, HWND hwndScrollBar){
	SCROLLINFO si;
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.fMask=SIF_ALL;
	si.cbSize=sizeof(SCROLLINFO);
	GetScrollInfo(hwndScrollBar, SB_CTL, &si);

	switch (nScrollCode)
	{
	case SB_LEFT:
		si.nPos=si.nMin;
		break;
	case SB_RIGHT:
		si.nPos=si.nMax;
		break;
	case SB_LINELEFT:
		si.nPos--;
		break;
	case SB_LINERIGHT:
		si.nPos++;
		break;
	case SB_PAGELEFT:
		si.nPos--;
		break;
	case SB_PAGERIGHT:
		si.nPos++;
		break;
	case SB_THUMBPOSITION:
		si.nPos=nPos;
		break;
	case SB_THUMBTRACK:
		si.nPos=nPos;
		break;
	}

	si.fMask=SIF_POS;
	si.cbSize=sizeof(SCROLLINFO);
	SetScrollInfo(hwndScrollBar, SB_CTL, &si, TRUE);
	return TRUE;
}

BOOL ViewScrollProc(int nScrollCode, short int nPos, HWND hwnd, int fnBar){
	SCROLLINFO si;
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.fMask=SIF_ALL;
	si.cbSize=sizeof(SCROLLINFO);
	GetScrollInfo(hwnd, fnBar, &si);


	const WORD SCROLLAMOUNT=5;

	switch(fnBar)
	{
	case SB_HORZ:
	{
		switch (nScrollCode)
		{
		case SB_LEFT:
			si.nPos=si.nMin;
			break;
		case SB_RIGHT:
			si.nPos=si.nMax;
			break;
		case SB_LINELEFT:
			si.nPos-=SCROLLAMOUNT;
			break;
		case SB_LINERIGHT:
			si.nPos+=SCROLLAMOUNT;
			break;
		case SB_PAGELEFT:
			si.nPos-=si.nPage;
			break;
		case SB_PAGERIGHT:
			si.nPos+=si.nPage;
			break;
		case SB_THUMBPOSITION:
			si.nPos=nPos;
			break;
		case SB_THUMBTRACK:
			si.nPos=nPos;
			break;
		}
		break;
	}
	case SB_VERT:
	{
		switch (nScrollCode)
		{
		case SB_TOP:
			si.nPos=si.nMin;
			break;
		case SB_BOTTOM:
			si.nPos=si.nMax;
			break;
		case SB_LINEUP:
			si.nPos-=SCROLLAMOUNT;
			break;
		case SB_LINEDOWN:
			si.nPos+=SCROLLAMOUNT;
			break;
		case SB_PAGEUP:
			si.nPos-=si.nPage;
			break;
		case SB_PAGEDOWN:
			si.nPos+=si.nPage;
			break;
		case SB_THUMBPOSITION:
			si.nPos=nPos;
			break;
		case SB_THUMBTRACK:
			si.nPos=nPos;
			break;
		}
		break;
	}
	default:return FALSE;
	}

	si.fMask=SIF_POS;
	si.cbSize=sizeof(SCROLLINFO);
	SetScrollInfo(hwnd, fnBar, &si, TRUE);
	si.fMask=SIF_POS;
	return TRUE;
}

BOOL UpdateViewScroll(HWND hwndScrollWindow){
	DWORD dwCurrentImage=GetScrollPosEx(GetDlgItem(GetParent(hwndScrollWindow), IDC_IMAGESCROLL), SB_CTL);
	int nWidth=0, nHeight=0;

	IMAGEDATA id;
	ILibrary.GetImageData(dwCurrentImage, &id);

	if(dwCurrentImage>0){
		nWidth=id.nWidth-1;
		nHeight=id.nHeight-1;
	}else{
		nWidth=0;
		nHeight=0;
	}

	RECT rcWindow;
	GetWindowRect(hwndScrollWindow, &rcWindow);

	UINT nHorzPage=rcWindow.right-rcWindow.left;
	UINT nVertPage=rcWindow.bottom-rcWindow.top;

	if(id.nWidth > nHorzPage)nVertPage-=GetSystemMetrics(SM_CXVSCROLL);
	if(id.nHeight > nVertPage)nHorzPage-=GetSystemMetrics(SM_CYHSCROLL);

	SCROLLINFO si;
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize=sizeof(SCROLLINFO);
	si.fMask=SIF_ALL;
	si.nMax=nWidth;
	si.nMin=0;
	si.nPage=nHorzPage;
	si.nPos=0;
	si.nTrackPos=0;
	SetScrollInfo(hwndScrollWindow, SB_HORZ, &si, TRUE);

	si.cbSize=sizeof(SCROLLINFO);
	si.fMask=SIF_ALL;
	si.nMax=nHeight;
	si.nMin=0;
	si.nPage=nVertPage;
	si.nPos=0;
	si.nTrackPos=0;
	SetScrollInfo(hwndScrollWindow, SB_VERT, &si, TRUE);
	
	return TRUE;
}

LRESULT CALLBACK ViewProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg)
	{
	case WM_CREATE:
		break;
	case WM_HSCROLL:
	{
		int nXPrev=0, nX=0;
		nXPrev=GetScrollPosEx(hwnd, SB_HORZ);
		ViewScrollProc((int)LOWORD(wParam), (short int)HIWORD(wParam), hwnd, SB_HORZ);
		nX=GetScrollPosEx(hwnd, SB_HORZ);
		ScrollWindowEx(hwnd, nXPrev-nX, 0, NULL, NULL, NULL, NULL, SW_ERASE|SW_INVALIDATE);
		//RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
		break;
	}
	case WM_VSCROLL:
	{
		int nYPrev=0, nY=0;
		nYPrev=GetScrollPosEx(hwnd, SB_VERT);
		ViewScrollProc((int)LOWORD(wParam), (short int)HIWORD(wParam), hwnd, SB_VERT);
		nY=GetScrollPosEx(hwnd, SB_VERT);
		ScrollWindowEx(hwnd, 0, nYPrev-nY, NULL, NULL, NULL, NULL, SW_ERASE|SW_INVALIDATE);
		//RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
		break;
	}
	case WM_PAINT:
	{
		DWORD dwCurrentImage=GetScrollPos(GetDlgItem(GetParent(hwnd), IDC_IMAGESCROLL), SB_CTL);
		if(
			(dwCurrentImage<1)||
			(dwCurrentImage>ILibrary.GetNumEntries())||
			(ILibrary.GetNumEntries()<1)
			)
			break;

		int nXAdjust=GetScrollPosEx(hwnd, SB_HORZ);
		int nYAdjust=GetScrollPosEx(hwnd, SB_VERT);

		PAINTSTRUCT ps;
		HDC hdc=BeginPaint(hwnd, &ps);
		ILibrary.CopyImageToDC(hdc, dwCurrentImage, -nXAdjust, -nYAdjust, FALSE);
		EndPaint(hwnd, &ps);
		DeleteDC(hdc);
		
		break;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0l;
}

void SetEditWndText(HWND hwnd, TCHAR szSaveFile[MAX_PATH]){
	TCHAR szText[MAX_PATH+13];

	_stprintf_s(szText, countof(szText), TEXT("ImageEdit [%s]"), szSaveFile);
	SetWindowText(hwnd, szText);
}

//Populates a list box with using specified string
BOOL PopulateListBoxWithFileType(HWND hwnd, int nDlgItem, TCHAR szSearchParam[MAX_PATH]){
	HANDLE hFind;
	WIN32_FIND_DATA sFindData;
	hFind = FindFirstFile(szSearchParam, &sFindData);
	if(hFind == INVALID_HANDLE_VALUE){
		SendDlgItemMessage(hwnd, nDlgItem, CB_ADDSTRING, 0, (LPARAM)TEXT("Could not find an appropriate file!"));
		return FALSE;
	}else{
		SendDlgItemMessage(hwnd, nDlgItem, CB_ADDSTRING, 0, (LPARAM)TEXT(""));
		SendDlgItemMessage(hwnd, nDlgItem, CB_ADDSTRING, 0, (LPARAM)sFindData.cFileName);
		while(FindNextFile(hFind, &sFindData)){
			SendDlgItemMessage(hwnd, nDlgItem, CB_ADDSTRING, 0, (LPARAM)sFindData.cFileName);
		}
		FindClose(hFind);
	}
	return TRUE;
}

BOOL CALLBACK AboutProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hwnd, IDC_VERSION, VERSIONTEXT);
		SetDlgItemText(hwnd, IDC_LIBRARYVERSION, LIBRARYVERSION);
		break;
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case IDOK:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		default:break;
		}
		break;
	}
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default:return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK GenerateProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg)
	{
	case WM_INITDIALOG:
	{
		break;
	}
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case ID_CANCEL:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case ID_GENERATE:
		{
			WORD nBitmap=0, nWidth=0, nHeight=0, nXSpacing=0, nYSpacing=0;
			BOOL bTranslated=FALSE;
			nWidth=GetDlgItemInt(hwnd, IDC_WIDTH, &bTranslated, FALSE);
			if(!bTranslated){
				NotifyUser(hwnd, TEXT("A value was invalid"), P_HIGH, EL_FAIL);
				break;
			}
			nHeight=GetDlgItemInt(hwnd, IDC_HEIGHT, &bTranslated, FALSE);
			if(!bTranslated){
				NotifyUser(hwnd, TEXT("A value was invalid"), P_HIGH, EL_FAIL);
				break;
			}
			nXSpacing=GetDlgItemInt(hwnd, IDC_XSPACING, &bTranslated, FALSE);
			if(!bTranslated){
				NotifyUser(hwnd, TEXT("A value was invalid"), P_HIGH, EL_FAIL);
				break;
			}
			nYSpacing=GetDlgItemInt(hwnd, IDC_YSPACING, &bTranslated, FALSE);
			if(!bTranslated){
				NotifyUser(hwnd, TEXT("A value was invalid"), P_HIGH, EL_FAIL);
				break;
			}
			nBitmap=GetDlgItemInt(hwnd, IDC_BITMAPS, &bTranslated, FALSE);
			if(!bTranslated){
				NotifyUser(hwnd, TEXT("A value was invalid"), P_HIGH, EL_FAIL);
				break;
			}
			BOOL bClear=FALSE;
			if(IsDlgButtonChecked(hwnd, IDC_CLEARDATA))bClear=TRUE;
			if(ILibrary.AutoGenerateData(nBitmap, nWidth, nHeight, nXSpacing, nYSpacing, bClear))
			{
				EndDialog(hwnd, 1);
				break;
			}else{
				NotifyUser(hwnd, TEXT("A value was invalid"), P_HIGH, EL_FAIL);
				break;
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
	default:return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK BitmapProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg)
	{
	case WM_INITDIALOG:
	{
		//populate listbox with bitmap names
		PopulateListBoxWithFileType(hwnd, IDC_BITMAPFILES, TEXT("*.bmp"));
		TCHAR szBitmapName[MAX_PATH];
		TCHAR szFinalString[MAX_PATH+3];
		for(int i=1; i<=ILibrary.GetNumBitmaps(); i++){
			ILibrary.GetBitmapName(szBitmapName, countof(szBitmapName), i);
			_stprintf_s(szFinalString, countof(szFinalString), TEXT("%i: %s"), i, szBitmapName);
			SendDlgItemMessage(hwnd, IDC_CURRENTBITMAPS, LB_ADDSTRING, 0, (LPARAM)szFinalString);
		}
		//SendDlgItemMessage(hwnd, IDC_CURRENTBITMAPS, CB_ADDSTRING, 0, (LPARAM)"Howdy");
		break;
	}
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case ID_FINISHED:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case ID_ADD:
		{
			TCHAR szNewBitmap[MAX_PATH];
			GetDlgItemText(hwnd, IDC_BITMAPFILES, szNewBitmap, MAX_PATH);
			if(!(ILibrary.AddBitmap(szNewBitmap))){
				NotifyUser(hwnd, TEXT("Could not load selected image!"), P_HIGH, EL_FAIL);
			}else{
				TCHAR szFinalString[MAX_PATH+3];
				_stprintf_s(szFinalString, countof(szFinalString), TEXT("%i: %s"), ILibrary.GetNumBitmaps(), szNewBitmap);
				SendDlgItemMessage(hwnd, IDC_CURRENTBITMAPS, LB_ADDSTRING, 0, (LPARAM)szFinalString);
			}
			break;
		}
		default:break;
		}
		break;
	}
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default:return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	static HWND hwndView=NULL;
	static TCHAR szLastFile[MAX_PATH]=TEXT("");
	static HINSTANCE hInstance=NULL;
	
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		TCHAR szViewClass[]=TEXT("ViewClass");

		WNDCLASS wc;
		ZeroMemory(&wc, sizeof(WNDCLASS));
		wc.cbClsExtra=0;
		wc.cbWndExtra=0;
		wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
		wc.hCursor=LoadCursor(NULL, IDC_ARROW);
		wc.hIcon=NULL;
		wc.hInstance=(HINSTANCE)lParam;
		wc.lpfnWndProc=ViewProc;
		wc.lpszClassName=szViewClass;
		wc.lpszMenuName=NULL;
		wc.style=CS_VREDRAW|CS_HREDRAW;
		RegisterClass(&wc);

		UpdateEditScroll(GetDlgItem(hwnd, IDC_IMAGESCROLL));

		hwndView=CreateWindow(
			szViewClass,
			szViewClass,
			WS_CHILD|WS_HSCROLL|WS_VSCROLL,
			10,
			200,
			420,
			160,
			hwnd,
			NULL,
			(HINSTANCE)lParam,
			NULL);

		hInstance=(HINSTANCE)lParam;


		UpdateWindow(hwndView);
		ShowWindow(hwndView, SW_SHOWNORMAL);
	
		EditUpdate(hwnd);
		UpdateViewScroll(hwndView);
		break;
	}
	
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		//The file menu
		case ID_FILENEW:
			if(MessageBox(hwnd, TEXT("Are you sure you want to create new database?"), TEXT("ImageEdit"), MB_YESNO|MB_ICONQUESTION)==IDYES){
				ILibrary.ClearDataBase();
				ILibrary.AddEntry(0,0,0,0,0,0,1,1,TEXT(""));
				szLastFile[0]=0;
				SetEditWndText(hwnd, TEXT("*"));
				UpdateEditScroll(GetDlgItem(hwnd, IDC_IMAGESCROLL));
				EditUpdate(hwnd);
				UpdateViewScroll(hwndView);
				RedrawWindow(hwndView, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				NotifyUser(hwnd, TEXT("Successfully started new data base!"), P_LOW, EL_SUCCESS);
			}
			break;
		case ID_FILEOPEN:
			if(!GetOpenFilename(
				TEXT("Load Image Database..."),
				TEXT("Image Database (*.ida)\0*.ida\0All Files (*.*)\0*.*\0"),
				hwnd,
				szLastFile))break;

			if(ILibrary.LoadData(szLastFile))
			{
				SetEditWndText(hwnd, szLastFile);
				UpdateEditScroll(GetDlgItem(hwnd, IDC_IMAGESCROLL));
				EditUpdate(hwnd);
				UpdateViewScroll(hwndView);
				RedrawWindow(hwndView, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);

				NotifyUser(hwnd, TEXT("Successfully loaded database!"), P_MEDIUM, EL_SUCCESS);
			}else{
				NotifyUser(hwnd, TEXT("Error: Could not load database!"), P_MEDIUM, EL_FAIL);
			}
			break;
		case ID_FILESAVEAS:
			if(!GetSaveFilename(
				TEXT("Save Image Database..."), 
				TEXT("Image Database (*.ida)\0*.ida\0All Files (*.*)\0*.*\0"), 
				TEXT("ida"),
				hwnd, 
				szLastFile))break;
				//got filename so fall through and save.
		case ID_FILESAVE:
		{
			if(szLastFile[0]==NULL)
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_FILESAVEAS, 0), 0);
			else
				if(!(ILibrary.SaveData(szLastFile)))
					NotifyUser(hwnd, TEXT("Error: Could not save data!"), P_MEDIUM, EL_FAIL);
				else{
					SetEditWndText(hwnd, szLastFile);
					NotifyUser(hwnd, TEXT("Successfully saved data!"), P_MEDIUM, EL_SUCCESS);
				}
			break;
		}
		case ID_FILEEXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			NotifyUser(hwnd, TEXT("Quiting..."), P_LOW, EL_SUCCESS);
			break;
		//the edit menu
		case ID_EDIT_BITMAPS:
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_BITMAPDIALOG), hwnd, BitmapProc);
			break;
		case ID_EDIT_AUTOGENERATE:
			if(DialogBox(hInstance, MAKEINTRESOURCE(IDD_AUTOGENERATE), hwnd, GenerateProc)){
				UpdateEditScroll(GetDlgItem(hwnd, IDC_IMAGESCROLL));
				EditUpdate(hwnd);
				UpdateViewScroll(hwndView);
				RedrawWindow(hwndView, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				NotifyUser(hwnd, TEXT("Successfully generated data!"), P_MEDIUM, EL_SUCCESS);
			}
			break;
		//The library 
		case ID_LIBRARY_BUILD:
		{
			TCHAR szBuildFile[MAX_PATH];
			szBuildFile[0]=NULL;
			if(!GetSaveFilename(
				TEXT("Build Library..."), 
				TEXT("Image Library (*.ilb)\0*.ilb\0All Files (*.*)\0*.*\0"), 
				TEXT("ilb"),
				hwnd, 
				szBuildFile))break;
			else
			{
				if(ILibrary.BuildLibrary(szBuildFile))
				{
					NotifyUser(hwnd, TEXT("Successfully built library!"), P_MEDIUM, EL_SUCCESS);
				}
				else
				{
					NotifyUser(hwnd, TEXT("Could not build library!"), P_MEDIUM, EL_FAIL);
				}
			}

			break;
		}
		case ID_LIBRARY_IMPORT:
		{
			TCHAR szImportFile[MAX_PATH];
			szImportFile[0]=NULL;
			if(!GetOpenFilename(
				TEXT("ImportLibrary..."), 
				TEXT("Image Library (*.ilb)\0*.ilb\0All Files (*.*)\0*.*\0"), 
				hwnd, 
				szImportFile))break;
			else{
				if(ILibrary.ImportLibrary(szImportFile))
				{
					NotifyUser(hwnd, TEXT("Successfully Imported Library!"), P_MEDIUM, EL_SUCCESS);
					UpdateEditScroll(GetDlgItem(hwnd, IDC_IMAGESCROLL));
					EditUpdate(hwnd);
					UpdateViewScroll(hwndView);
					RedrawWindow(hwndView, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				}
				else
				{
					NotifyUser(hwnd, TEXT("Could not import library"), P_MEDIUM, EL_FAIL);
				}
			}

			break;
		}
		//The help menu
		case ID_HELP_ABOUT:
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutProc);
			break;
		//The buttons
		case IDC_CLOSE:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			NotifyUser(hwnd, TEXT("Quiting..."), P_LOW, EL_SUCCESS);

			break;
		case IDC_APPLY:
			if(ILibrary.GetNumEntries()<1){
				NotifyUser(hwnd, TEXT("There is no entry to change, add an entry!"), P_MEDIUM, EL_FAIL);
				break;
			}
			if(MessageBox(hwnd, TEXT("Are you sure you want to change the current settings?"), TEXT("ImageEdit"), MB_YESNO|MB_ICONQUESTION)==IDYES){
				if(!SetValues(hwnd)){
					NotifyUser(hwnd, TEXT("Error: Could not apply specified values!"), P_MEDIUM, EL_FAIL);
				}else{
					EditUpdate(hwnd);
					UpdateViewScroll(hwndView);
					RedrawWindow(hwndView, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
					NotifyUser(hwnd, TEXT("Applied specified values!"), P_LOW, EL_SUCCESS);
				}
			}
			break;
		case IDC_ADDENTRY:
			if(MessageBox(hwnd, TEXT("Are you sure you want to add another entry?"), TEXT("ImageEdit"), MB_YESNO|MB_ICONQUESTION)==IDYES){
				if(FAILED(ILibrary.AddEntry(0,0,0,0,0,0,1,1,TEXT("")))){
					NotifyUser(hwnd, TEXT("Error: Could not add another entry!"), P_HIGH, EL_FAIL);
					break;
				}else{
					UpdateEditScroll(GetDlgItem(hwnd, IDC_IMAGESCROLL));
					SendMessage(hwnd, WM_HSCROLL, MAKEWPARAM(SB_RIGHT, 0), (LPARAM)GetDlgItem(hwnd, IDC_IMAGESCROLL));
					//The hscroll message will update everything else
					NotifyUser(hwnd, TEXT("Successfully added entry!"), P_MEDIUM, EL_SUCCESS);
				}
			}
			break;
		default:break;
		}

		break;
	}
	case WM_HSCROLL:
		EditScrollProc((int)LOWORD(wParam), (short int) HIWORD(wParam), (HWND)lParam);
		EditUpdate(hwnd);
		UpdateViewScroll(hwndView);
		RedrawWindow(hwndView, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:return FALSE;
	}
	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
	MSG msg;

	//this is just test stuff for now

	//Because of problems occuring I had to add a default
	//entry:
	ILibrary.AddEntry(0,0,0,0,0,0,1,1,TEXT(""));


	

	HWND hwnd=CreateDialogParam(
		hInstance, 
		MAKEINTRESOURCE(IDD_IMAGEEDIT), 
		NULL, 
		EditProc, 
		(LPARAM)hInstance);

	if(hwnd==NULL){
		MessageBoxA(NULL, "This program requires Windows NT!", "ImageEidt", MB_OK|MB_ICONERROR);
		return 0;
	}

	SetEditWndText(hwnd, TEXT("*"));

	ShowWindow(hwnd, nShowCmd);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(!IsDialogMessage(hwnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}