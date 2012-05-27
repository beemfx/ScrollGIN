/*
	ScrollGIN, a 32bit scrolling based engine.

	main.cpp - entry point and functions for ScrollGIN technologies

	Copyright (c) 2002, 2003 Blaine Myers
*/
/*
	All files specified in the ScrollGIN project/workspace are:
		Copyright (c) 2002, 2003 Blaine Myers
		unless otherwise specified.
*/
#include <ddraw.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "defines.h"
#include "error.h"
#include "../GFXG7/gfxg7.h"
#include "resource.h"


#define TESTENGINE2
#ifdef TESTENGINE2
#include "ScrollGIN.h"
#include "Joes2.h"
CScrollGINGame Game;
#endif //TESTENGINE2

#define WM_USER_ACTIVEAPP (WM_USER+1)


//directdraw 7 interfaces
LPDIRECTDRAW7 g_lpDD=NULL;
LPDIRECTDRAWSURFACE7 g_lpPrimary=NULL;
LPDIRECTDRAWSURFACE7 g_lpBackBuffer=NULL;


//saved parameters
SAVEDPARAMETERS g_spSavedParams;
/*
DWORD g_nDeviceWidth=SCREEN_WIDTH;
DWORD g_nDeviceHeight=SCREEN_HEIGHT;
DWORD g_nColorDepth=COLOR_DEPTH;
DWORD g_nTransparentColor=TRANSPARENT_COLOR;

HWND g_hwnd;

RECT g_rcWindow;

BOOL g_bWindowed=TRUE;
*/


HRESULT GameInit(HWND hWnd, BOOL bWindowed, HINSTANCE hInstance, int nShowCmd){
	//Initialize DirectDraw.
	SetSavedParameters(
		&g_spSavedParams,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		COLOR_DEPTH,
		TRANSPARENT_COLOR,
		hWnd,
		NULL,
		bWindowed);

	if(FAILED(InitDirectDrawEx(
		&g_lpDD, 
		&g_lpPrimary, 
		&g_lpBackBuffer,
		&g_spSavedParams)))return E_FAIL;

	ShowWindow(hWnd, nShowCmd);
	SetFocus(hWnd);

	HMENU hMenu=LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));	
	AdjustWindowSize(hWnd, g_spSavedParams.dwWidth, g_spSavedParams.dwHeight, bWindowed, hMenu);
	if(!bWindowed)ShowCursor(FALSE);
	
	
	#ifdef TESTENGINE2
	Game.GameInit(
		g_spSavedParams.dwWidth,
		g_spSavedParams.dwHeight,
		g_spSavedParams.dwTransparentColor,
		new CJoes2ObjMan(256),
		hWnd,
		g_lpDD);
		
	#endif //TESTENGINE2
	

	return S_OK;
}


HRESULT GameShutdown(){
	
	#ifdef TESTENGINE2
	Game.Release();
	Game.Shutdown();
	#endif //TESTENGINE2

	
	//we need to release the screen buffers.
	SAFE_RELEASE(g_lpBackBuffer);
	SAFE_RELEASE(g_lpPrimary);
	//it is necessary to return the cooperative level (actually if we
	//don't do this it will happen anyway)
	g_lpDD->SetCooperativeLevel(NULL, DDSCL_NORMAL);
	//with all surfaces release we can release the object
	
	SAFE_RELEASE(g_lpDD);
	

	if(!g_spSavedParams.bWindowed)ShowCursor(TRUE);

	//we also need to shutdown the sound engine, 
	//unless it shuts down itself (once it's made).
	
	return S_OK;
}


HRESULT RestoreGraphics(){
	
	//restore all surfaces
	g_lpDD->RestoreAllSurfaces();

	#ifdef TESTENGINE2
	Game.ReloadImages();
	#endif //TESTENGINE2
	
	return S_OK;
}


LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case ID_FILE_EXIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;	
		}
		break;
	}
	case WM_ACTIVATEAPP:
		if(wParam)
		{
			#ifdef TESTENGINE2
			Game.Pause(FALSE);
			#endif //TESTENGINE2
			PostMessage(hWnd, WM_USER_ACTIVEAPP, TRUE, 0);
		}
		else
		{
			#ifdef TESTENGINE2
			Game.Pause(TRUE);
			#endif //TESTENGINE2
			PostMessage(hWnd, WM_USER_ACTIVEAPP, FALSE, 0);
		}
		break;
	case WM_KEYDOWN:
	{
		switch(wParam)
		{
		case VK_PAUSE:
			#ifdef TESTENGINE2
			Game.TogglePause();
			#endif //TESTENGINE2
			break;
		case VK_ESCAPE:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}		
		break;
	}
	case WM_MOVE:
		UpdateBounds(g_spSavedParams.bWindowed, (g_spSavedParams.hWnd), &(g_spSavedParams.rcWindow));
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0l;
}



/*
	RenderObjects first renders the mapboard
	following that the Object Manager animates
	all the objects (Proccesses AI, input, and movement)
	and draws them.
*/
HRESULT RenderObjects(LPVOID lpBuffer){
	#ifdef TESTENGINE2
	Game.Render(lpBuffer);
	#endif //TESTENGINE2
	return S_OK;
}

HRESULT DDrawRender(){
	if(!g_lpBackBuffer){
		SetError(TEXT("Cannot render because there is no buffer"));
		return E_FAIL;
	}
	if(!g_lpDD){
		SetError(TEXT("Cannot render because there is no object"));
		return E_FAIL;
	}
	//first we fill a solid color into the background
	DDBLTFX ddbltfx;
   ZeroMemory(&ddbltfx, sizeof(ddbltfx));
   ddbltfx.dwSize = sizeof(ddbltfx);
   ddbltfx.dwFillColor = RGB(255, 255, 255);
   g_lpBackBuffer->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );

	RenderObjects(g_lpBackBuffer);

	return S_OK;
}



HRESULT ComposeFrame()
{
	#ifdef TESTENGINE2
	Game.PreRenderProcess();
	#endif //TESTENGINE2
	return DDrawRender();
}


HRESULT GameLoop()
{
	ComposeFrame();
	return PageFlip(
		g_lpPrimary, 
		g_lpBackBuffer, 
		g_spSavedParams.bWindowed, 
		&g_spSavedParams.rcWindow,
		RestoreGraphics);
}

BOOL ProcessCommandLine(BOOL* lpWindowed)
{
	LPTSTR szCommandLine=GetCommandLine();
	char* token=NULL;
	char steps[]="-";
	token=strtok(szCommandLine, steps);
	while(token != NULL)
	{
		if(strnicmp("window", token, 7)==0)
			*lpWindowed=TRUE;
		
		if(strnicmp("w", token, 2)==0)
			*lpWindowed=TRUE;

		token=strtok(NULL, steps);
	}
	return TRUE;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
	MSG msg;
	HWND hWnd=NULL;
	WNDCLASSEX wc;
	HACCEL hAccel=NULL;

	BOOL bActiveApp=TRUE;

	static TCHAR szAppName[] = TEXT("ScrollGIN");

	wc.cbSize=sizeof(wc);
	wc.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.lpfnWndProc=MainWndProc;
	wc.hInstance=hInstance;
	wc.hbrBackground=(HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
	wc.hIconSm = NULL;//LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName=NULL;
	wc.lpszClassName=szAppName;

	if(!RegisterClassEx(&wc)){
		MessageBox(NULL, TEXT("This program requires Windows NT!"), TEXT("Notice"), MB_OK|MB_ICONERROR);
		return 0;
	}

	hWnd = CreateWindowEx(WS_EX_TOPMOST, 
								szAppName,
								szAppName,
								WS_POPUP|WS_SYSMENU|WS_VISIBLE,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								512,
								512,
								NULL,
								NULL,
								hInstance,
								NULL);

	if(hWnd==NULL)return -1;

	BOOL bWindow=0;
	ProcessCommandLine(&bWindow);

	#if defined(_DEBUG)
	bWindow = TRUE;
	#endif

	GameInit(hWnd, bWindow , hInstance, nShowCmd);

	#ifdef TESTENGINE2
	Game.LoadMap(TEXT("joeslevel1.map"));
	#endif //TESTENGINE2

	while(TRUE){
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){

			if(msg.message==WM_QUIT)
				break;

			
			if(msg.message==WM_USER_ACTIVEAPP)
				bActiveApp=msg.wParam;
			

			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}else{
			if(bActiveApp)GameLoop();else WaitMessage();
		}
	}
	GameShutdown();
	return msg.wParam;
}