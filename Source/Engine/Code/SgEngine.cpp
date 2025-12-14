// (c) 2025 Beem Media. All rights reserved.

#include "SgEngine.h"

#include "GameConfig.h"
#include "Renderer2/Renderer.h"
#include "ScrollGIN.h"

#include <stdio.h>
#include <memory>
#include <string>

#if defined(_WIN32)
#include <Windows.h>
#endif

static wchar_t* SgEngineRunner_WindowClassName = L"ScrollGINGameWindow";

class SgEngineRunner
{
public:
	SgEngineRunner(const wchar_t* AppName, std::uintptr_t OsInstance, SgObjectManager* InObjMan, sg_cpstr FirstMap)
		: m_OsInstance(OsInstance)
		, m_AppName(AppName)
	{
		PreInit();

		sgRendererInitParms InitParms;
		InitParms.Width = 1024;
		InitParms.Height = 768;
		InitParms.Windowed = m_bWindowed;
		InitParms.Wnd = reinterpret_cast<std::uintptr_t>(m_MainWnd);
		Renderer_Init(&InitParms);

		m_Game = std::make_unique<SgScrollGINGame>();
		if (m_Game)
		{
			m_Game->Init(VIEW_WIDTH, VIEW_HEIGHT, InObjMan, reinterpret_cast<std::uintptr_t>(m_MainWnd));
			m_Game->LoadMap(FirstMap);

			// Game Loop
			while (m_bEngineRunning)
			{
				OsLoop();

				m_Game->Update();
				Renderer_BeginFrame();
				m_Game->Render();
				Renderer_EndFrame();
			}

			m_Game->Deinit();
			m_Game.reset();
		}

		Renderer_Deinit();

		PostDestroy();
	}

private:
	std::unique_ptr<SgScrollGINGame> m_Game;
	std::uintptr_t m_OsInstance = 0;
	std::wstring m_AppName;
	bool m_bWindowed = true;
	bool m_bActiveApp = false;
	bool m_bEngineRunning = true;

#if defined(_WIN32)
	HWND m_MainWnd = NULL;
#endif

private:
	void OsLoop()
	{
		MSG msg = {};
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void PreInit()
	{
		ProcessCommandLine();

#if defined(_WIN32)
		HINSTANCE hInst = reinterpret_cast<HINSTANCE>(m_OsInstance);

		WNDCLASSEXW wc;
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.lpfnWndProc = MainWndProc;
		wc.hInstance = hInst;
		wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(DKGRAY_BRUSH));
		wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(101));
		wc.hIconSm = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = SgEngineRunner_WindowClassName;

		if (!RegisterClassExW(&wc))
		{
			MessageBoxW(NULL, L"This program requires Windows NT!", L"Notice", MB_OK | MB_ICONERROR);
			return;
		}

		m_MainWnd = CreateWindowExW(
			WS_EX_TOPMOST,
			SgEngineRunner_WindowClassName,
			m_AppName.c_str(),
			WS_POPUP | WS_SYSMENU | WS_VISIBLE | WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION | WS_DLGFRAME | WS_MINIMIZEBOX,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			512,
			512,
			NULL,
			NULL,
			hInst,
			reinterpret_cast<LPVOID>(this));

		ShowWindow(m_MainWnd, SW_SHOWNORMAL);
		SetFocus(m_MainWnd);
#endif
	}

	void PostDestroy()
	{
#if defined(_WIN32)
		DestroyWindow(m_MainWnd);
		m_MainWnd = NULL;

		HINSTANCE hInst = reinterpret_cast<HINSTANCE>(m_OsInstance);
		UnregisterClassW(SgEngineRunner_WindowClassName, hInst);
#endif
	}

	void ProcessCommandLine()
	{
#if defined(_WIN32)
		char* szCommandLine = GetCommandLineA();
		char* token = NULL;
		char steps[] = "-";
		char* NextToken = NULL;
		token = strtok_s(szCommandLine, steps, &NextToken);
		while (token != NULL)
		{
			if (_strnicmp("windowed", token, 7) == 0)
				m_bWindowed = true;

			if (_strnicmp("w", token, 2) == 0)
				m_bWindowed = true;

			if (_strnicmp("fullscreen", token, 7) == 0)
				m_bWindowed = true;

			if (_strnicmp("fs", token, 2) == 0)
				m_bWindowed = true;

			token = strtok_s(NULL, steps, &NextToken);
		}
#endif
	}

#if defined(_WIN32)
	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (WM_NCCREATE == msg)
		{
			SgEngineRunner* _this = reinterpret_cast<SgEngineRunner*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
			SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(_this));
			return DefWindowProcW(hWnd, msg, wParam, lParam);
		}

		SgEngineRunner* _this = reinterpret_cast<SgEngineRunner*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		switch (msg) {
		case WM_ACTIVATEAPP:
		{
			if (_this)
			{
				_this->m_bActiveApp = wParam == TRUE;
			}

			if (wParam)
			{
				Renderer_OnActivateApp();
				if (_this && _this->m_Game)
				{
					_this->m_Game->Pause(FALSE);
				}
			}
			else
			{
				if (_this && _this->m_Game)
				{
					_this->m_Game->Pause(TRUE);
				}
			}
		} break;
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_PAUSE:
				if (_this && _this->m_Game)
				{
					_this->m_Game->TogglePause();
				}
				break;
			case VK_ESCAPE:
				if (_this)
				{
					_this->m_bEngineRunning = false;
				}
				break;
			}
			break;
		}
		case WM_MOVE:
			Renderer_UpdateBounds();
			break;
		case WM_CLOSE:
		{
			if (_this)
			{
				_this->m_bEngineRunning = false;
			}
		} break;
		default:return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		return 0l;
	}
#endif
};

void SgEngine_Run(const wchar_t* AppName, std::uintptr_t OsInstance, SgObjectManager* InObjMan, sg_cpstr FirstMap)
{
	SgEngineRunner Runner(AppName, OsInstance, InObjMan, FirstMap);
}
