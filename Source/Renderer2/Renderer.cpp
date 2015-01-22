#if __DXVERSION__ == 7
#include "Renderer.h"
#include "RendererImage.h"
#include "GameConfig.h"
#include <ddraw.h>
#include <windowsx.h>

#pragma comment(lib, "../DXLIB/ddraw.lib")

static class SgRenderer
{
private:
	sgRendererInitParms  m_InitParms;
	HWND                 m_hwnd;
	IDirectDraw7*        m_Dd;
	IDirectDrawSurface7* m_PrimarySurface;
	IDirectDrawSurface7* m_BackSurface;
	RECT                 m_RcWindow;
private:
	void ReleaseAll()
	{
		if( m_BackSurface )
		{
			m_BackSurface->Release();
			m_BackSurface = 0;
		}

		if (m_PrimarySurface)
		{
			m_PrimarySurface->Release();
			m_PrimarySurface = 0;
		}

		if (m_Dd)
		{
			m_Dd->Release();
			m_Dd = 0;
		}

	}

	void AdjustWindowSize()
	{
		if( m_InitParms.Windowed )
		{
			RECT rc;

			DWORD DeskWidth = GetSystemMetrics( SM_CXSCREEN );
			DWORD DeskHeight = GetSystemMetrics( SM_CYSCREEN );

			DWORD Width  = m_InitParms.Width;
			DWORD Height = m_InitParms.Height;
			DWORD X = (DeskWidth-Width)/2;
			DWORD Y = (DeskHeight-Height)/2;

			SetRect( &rc , X , Y , X+Width, Y+Height );

			AdjustWindowRectEx( &rc , GetWindowStyle(m_hwnd) , false, GetWindowExStyle(m_hwnd) );
			SetWindowPos(m_hwnd, NULL, X, Y, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
			SetWindowPos(m_hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
		}
	}

	void Restore()
	{
		//restore all surfaces
		m_Dd->RestoreAllSurfaces();
		//TODO: Reload all graphics...
	}

public:
	SgRenderer()
	: m_Dd( 0 )
	, m_hwnd( 0 )
	, m_PrimarySurface( 0 )
	, m_BackSurface( 0 )
	{

	}

	void Init(  const sgRendererInitParms* InitParms )
	{
		m_InitParms = *InitParms;
		m_InitParms.Width = VIEW_WIDTH;
		m_InitParms.Height = VIEW_HEIGHT;
		m_InitParms.Windowed = true;
		m_hwnd = static_cast<HWND>(m_InitParms.Wnd);

		AdjustWindowSize();

		HRESULT Res;
		Res = DirectDrawCreateEx( 0 , (void**)&m_Dd , IID_IDirectDraw7 , 0 );
		if( FAILED( Res ) )
		{
			ReleaseAll();
			return;
		}

		if( m_InitParms.Windowed )
		{
			Res = m_Dd->SetCooperativeLevel( m_hwnd , DDSCL_NORMAL );
		}
		else
		{
			Res = m_Dd->SetCooperativeLevel( m_hwnd , DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN );
		}
		if (FAILED(Res))
		{
			ReleaseAll();
			return;
		}

		if( m_InitParms.Windowed )
		{
			// Primary Surface and Clipper
			DDSURFACEDESC2 Desc;
			memset( &Desc , 0 , sizeof(Desc) );
			Desc.dwSize = sizeof( Desc );
			Desc.dwFlags = DDSD_CAPS;
			Desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

			Res = m_Dd->CreateSurface( &Desc , &m_PrimarySurface , 0 );
			if (FAILED(Res))
			{
				ReleaseAll();
				return;
			}

			IDirectDrawClipper* Clipper = 0;
			Res = m_Dd->CreateClipper( 0 , &Clipper , 0 );
			if (FAILED(Res))
			{
				ReleaseAll();
				return;
			}
			Clipper->SetHWnd( 0 , m_hwnd );
			Res = m_PrimarySurface->SetClipper( Clipper );
			Clipper->Release();
			Clipper = 0;
			if( FAILED( Res ) )
			{
				ReleaseAll();
				return;
			}
			//Back Surface
			Desc.dwFlags        = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
			Desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_3DDEVICE;
			Desc.dwWidth        = m_InitParms.Width;
			Desc.dwHeight       = m_InitParms.Height;
			Res = m_Dd->CreateSurface( &Desc , &m_BackSurface , 0 );
			if (FAILED(Res))
			{
				ReleaseAll();
				return;
			}

		}
		else
		{
			Res = m_Dd->SetDisplayMode( m_InitParms.Width , m_InitParms.Height , 32 , 0 , 0 );
			DDSURFACEDESC2 Desc;
			memset(&Desc, 0, sizeof(Desc));

			Desc.dwSize            = sizeof(Desc);
			Desc.dwFlags           = DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
			Desc.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE|DDSCAPS_FLIP|DDSCAPS_COMPLEX|DDSCAPS_3DDEVICE;
			Desc.dwBackBufferCount = 1;

			Res = m_Dd->CreateSurface( &Desc , &m_PrimarySurface , 0 );
			if (FAILED(Res))
			{
				ReleaseAll();
				return;
			}
			DDSCAPS2 Caps;
			memset( &Caps , 0 , sizeof(Caps) );
			Caps.dwCaps=DDSCAPS_BACKBUFFER;
			Res = m_PrimarySurface->GetAttachedSurface( &Caps , &m_BackSurface );
			if (FAILED(Res))
			{
				ReleaseAll();
				return;
			}
		}

		UpdateBounds();

		if(m_InitParms.Windowed)ShowCursor(FALSE);
	}

	void Deinit()
	{
		m_Dd->SetCooperativeLevel(NULL, DDSCL_NORMAL);
		ReleaseAll();
		if(m_InitParms.Windowed)ShowCursor(TRUE);
	}

	void UpdateBounds()
	{
		if (m_InitParms.Windowed)
		{
			GetClientRect(m_hwnd, &m_RcWindow);
			ClientToScreen(m_hwnd, (POINT*)&m_RcWindow);
			ClientToScreen(m_hwnd, (POINT*)&m_RcWindow + 1);
		}
		else
		{
			SetRect(&m_RcWindow, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		}
	}

	void BeginFrame()
	{
		DDBLTFX ddbltfx;
		ZeroMemory(&ddbltfx, sizeof(ddbltfx));
		ddbltfx.dwSize = sizeof(ddbltfx);
		ddbltfx.dwFillColor = RGB(0, 255, 255);
		m_BackSurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
	}

	void EndFrame()
	{
		HRESULT Res;
		RECT rcSource;

		/* Bail if buffers do not exist as we cannot do anything. */
		if (!m_PrimarySurface || !m_BackSurface)
			return;


		if (m_InitParms.Windowed)
		{
			DDSURFACEDESC2 desc;
			desc.dwSize = sizeof(DDSURFACEDESC2);
			desc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT;
			m_BackSurface->GetSurfaceDesc(&desc);

			rcSource.top = rcSource.left = 0;
			rcSource.bottom = desc.dwHeight;
			rcSource.right = desc.dwWidth;

			Res = m_PrimarySurface->Blt(&m_RcWindow, m_BackSurface, &rcSource, DDBLT_WAIT, 0);
			if (Res == DDERR_SURFACELOST)
			{
				Restore();
				return;
			}
		}
		else{
			Res = m_PrimarySurface->Flip(0, DDFLIP_WAIT);
			if (DDERR_SURFACELOST == Res)
			{
				Restore();
				return;
			}
		}
	}

	class SgRendererImage* CreateSprite( const sgRendererImageCreateParms* CreateParms )
	{
		sgRendererData Data;
		Data.Dd = m_Dd;
		Data.BackSurface = m_BackSurface;
		SgRendererImage* NewImg = new SgRendererImage( CreateParms , &Data );
		return NewImg;
	}

	void DestroySprite(class SgRendererImage* Sprite)
	{
		if( Sprite )delete Sprite;
	}

} Renderer;

void Renderer_Init( const sgRendererInitParms* InitParms )
{
	Renderer.Init( InitParms );
}

void Renderer_Deinit()
{
	Renderer.Deinit();
}

void Renderer_BeginFrame()
{

	Renderer.BeginFrame();
}

void Renderer_EndFrame()
{
	Renderer.EndFrame();
}

class SgRendererImage* Renderer_CreateSprite(const sgRendererImageCreateParms* CreateParms)
{
	return Renderer.CreateSprite( CreateParms );
}

void Renderer_DestroySprite(class SgRendererImage* Sprite)
{
	Renderer.DestroySprite( Sprite );
}

void Renderer_UpdateBounds()
{
	Renderer.UpdateBounds();
}

#endif __DXVERSION__ == 7