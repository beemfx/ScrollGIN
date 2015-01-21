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
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "defines.h"
#include "error.h"
#include "../Renderer2/Renderer.h"
#include "resource.h"


#include "ScrollGIN.h"
#include "Joes2.h"
CScrollGINGame Game;

#define WM_USER_ACTIVEAPP (WM_USER+1)

HRESULT GameInit(HWND hWnd, BOOL bWindowed, HINSTANCE hInstance, int nShowCmd)
{
	sgRendererInitParms InitParms;
	InitParms.Width = 640;
	InitParms.Height = 480;
	InitParms.Windowed = true;
	InitParms.Wnd = hWnd;

	Renderer_Init( &InitParms );

	ShowWindow(hWnd, nShowCmd);
	SetFocus(hWnd);
	
	Game.GameInit(
		640,
		480,
		TRANSPARENT_COLOR,
		new CJoes2ObjMan(256),
		hWnd);

	return S_OK;
}


static void GameShutdown()
{
	Game.Release();
	Game.Shutdown();
	
	//we need to release the screen buffers.
	Renderer_Deinit();
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
			Game.Pause(FALSE);
			PostMessage(hWnd, WM_USER_ACTIVEAPP, TRUE, 0);
		}
		else
		{
			Game.Pause(TRUE);
			PostMessage(hWnd, WM_USER_ACTIVEAPP, FALSE, 0);
		}
		break;
	case WM_KEYDOWN:
	{
		switch(wParam)
		{
		case VK_PAUSE:
			Game.TogglePause();
			break;
		case VK_ESCAPE:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}		
		break;
	}
	case WM_MOVE:
		Renderer_UpdateBounds();
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

static void GameLoop()
{
	
	Game.PreRenderProcess();
	Renderer_BeginFrame();
	Game.Render();
	Renderer_EndFrame();
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

	Game.LoadMap(TEXT("joeslevel1.map"));

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