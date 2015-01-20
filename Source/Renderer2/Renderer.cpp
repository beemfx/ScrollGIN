#include "Renderer.h"
#include "RendererImage.h"
#include <ddraw.h>
#include <windowsx.h>

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

	void UpdateBounds()
	{
		if( m_InitParms.Windowed )
		{
			GetClientRect(m_hwnd, &m_RcWindow);
			ClientToScreen(m_hwnd, (POINT*)&m_RcWindow);
			ClientToScreen(m_hwnd, (POINT*)&m_RcWindow + 1);
		}
		else
		{
			SetRect( &m_RcWindow, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) );
		}
	}

	void AdjustWindowSize()
	{
		if( m_InitParms.Windowed )
		{
			RECT rcWork;
			RECT rc;
			DWORD dwStyle;

			dwStyle = GetWindowStyle(m_hwnd);
			dwStyle &= ~WS_POPUP;
			dwStyle |= WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION | WS_DLGFRAME | WS_MINIMIZEBOX;
			SetWindowLong(m_hwnd, GWL_STYLE, dwStyle);
			SetRect( &rc , 0 , 0 , m_InitParms.Width, m_InitParms.Height );
			AdjustWindowRectEx( &rc , GetWindowStyle(m_hwnd) , false, GetWindowExStyle(m_hwnd) );
			SetWindowPos(m_hwnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			SetWindowPos(m_hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

			/*  Make sure our window does not hang outside of the work area. */
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
			GetWindowRect(m_hwnd, &rc);
			if (rc.left < rcWork.left) rc.left = rcWork.left;
			if (rc.top < rcWork.top)  rc.top = rcWork.top;
			SetWindowPos(m_hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
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
		m_hwnd = static_cast<HWND>(m_InitParms.Wnd);
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
			m_PrimarySurface->GetAttachedSurface( &Caps , &m_BackSurface );
			if (FAILED(Res))
			{
				ReleaseAll();
				return;
			}

			SetWindowLong( m_hwnd , GWL_STYLE , WS_POPUP );
		}

		UpdateBounds();
		AdjustWindowSize();
	}

	void Deinit()
	{
		ReleaseAll();
	}

	void BeginFrame()
	{

	}

	void EndFrame()
	{
		HRESULT Res;
		RECT rcSource;

		/* Bail if buffers do not exist as we cannot do anything. */
		if (!m_PrimarySurface || !m_BackSurface)
			return;

		while (TRUE){
			if (m_InitParms.Windowed)
			{
				DDSURFACEDESC2 desc;
				desc.dwSize = sizeof(DDSURFACEDESC2);
				desc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT;
				m_BackSurface->GetSurfaceDesc(&desc);

				rcSource.top = rcSource.left = 0;
				rcSource.bottom = desc.dwHeight;
				rcSource.right = desc.dwWidth;

				Res = m_PrimarySurface->Blt(&m_RcWindow,m_BackSurface,&rcSource,DDBLT_WAIT,0);
				if( Res == DDERR_SURFACELOST )
				{
					Restore();
					return;
				}
			}
			else{
				Res = m_PrimarySurface->Flip( 0 , DDFLIP_WAIT );
				if( DDERR_SURFACELOST == Res )
				{
					Restore();
					return;
				}
			}
		}
	}

	class SgRendererImage* CreateSprite(const sgRendererImageCreateParms* CreateParms)
	{
		return 0;
	}

	void DestroySprite(class SgRendererImage* Sprite)
	{
		
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