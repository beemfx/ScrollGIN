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
#include <windows.h>
#include "GameConfig.h"
#include "Renderer2/Renderer.h"


#include "ScrollGIN.h"
#include "Joes2.h"
static SgScrollGINGame Game;
static CJoes2ObjMan*   Joes2ObjMan = 0;

#define WM_USER_ACTIVEAPP (WM_USER+1)

static void Main_Game_Init(HWND hWnd, BOOL bWindowed, HINSTANCE hInstance)
{
	sgRendererInitParms InitParms;
	InitParms.Width = 1024;
	InitParms.Height = 768;
	InitParms.Windowed = FALSE != bWindowed;
	InitParms.Wnd = hWnd;

	Renderer_Init( &InitParms );

	Joes2ObjMan = new CJoes2ObjMan(256); 
	Game.Init(VIEW_WIDTH,VIEW_HEIGHT,Joes2ObjMan,hWnd);
}


static void Main_Game_Deinit()
{
	Game.Deinit();
	delete Joes2ObjMan;
	
	//we need to release the screen buffers.
	Renderer_Deinit();
}

static void Main_Game_Loop()
{
	Game.Update();
	Renderer_BeginFrame();
	Game.Render();
	Renderer_EndFrame();
}


static LRESULT CALLBACK Main_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg){
	case WM_ACTIVATEAPP:
		if(wParam)
		{
			Renderer_OnActivateApp();
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

static void Main_ProcessCommandLine(BOOL* lpWindowed)
{
	LPTSTR szCommandLine=GetCommandLine();
	char* token=NULL;
	char steps[]="-";
	char* NextToken = NULL;
	token=strtok_s(szCommandLine, steps, &NextToken );
	while(token != NULL)
	{
		if(_strnicmp("windowed", token, 7)==0)
			*lpWindowed=TRUE;
		
		if(_strnicmp("w", token, 2)==0)
			*lpWindowed=TRUE;

		if(_strnicmp("fullscreen", token, 7)==0)
			*lpWindowed=FALSE;
		
		if(_strnicmp("fs", token, 2)==0)
			*lpWindowed=FALSE;

		token=strtok_s(NULL, steps, &NextToken );
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MSG msg;
	HWND hWnd=NULL;
	WNDCLASSEX wc;
	HACCEL hAccel=NULL;

	BOOL bActiveApp=TRUE;

	static char szAppName[] = TEXT("ScrollGIN");

	wc.cbSize=sizeof(wc);
	wc.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.lpfnWndProc=Main_WndProc;
	wc.hInstance=hInstance;
	wc.hbrBackground=(HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101));
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName=NULL;
	wc.lpszClassName=szAppName;

	if(!RegisterClassEx(&wc)){
		MessageBox(NULL, TEXT("This program requires Windows NT!"), TEXT("Notice"), MB_OK|MB_ICONERROR);
		return 0;
	}

	hWnd = CreateWindowEx(
		WS_EX_TOPMOST, 
		szAppName,
		szAppName,
		WS_POPUP|WS_SYSMENU|WS_VISIBLE|WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION | WS_DLGFRAME | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		512,
		512,
		NULL,
		NULL,
		hInstance,
		NULL);

	if(hWnd==NULL)return -1;

	ShowWindow( hWnd , SW_SHOWNORMAL);
	SetFocus(hWnd);

	BOOL bWindow=TRUE;
	Main_ProcessCommandLine(&bWindow);


	Main_Game_Init(hWnd, bWindow , hInstance);

	Game.LoadMap(TEXT("joeslevel1.map"));

	bool ShouldQuit = false;

	while(true)
	{
		while( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			if(msg.message==WM_QUIT)
				ShouldQuit = true;

			if(msg.message==WM_USER_ACTIVEAPP)
				bActiveApp=msg.wParam;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if( ShouldQuit )break;
		
		Main_Game_Loop();

	}
	Main_Game_Deinit();
	return msg.wParam;
}