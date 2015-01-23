#if __DXVERSION__ == 11
#include "Renderer.h"
#include "RendererImage.h"
#include "RendererD3D11_Private.h"
#include "GameConfig.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <windowsx.h>

//Stupid DirectXMath.h is including assert.h
#undef assert
#if defined( DEBUG )
#define assert( x ) if ( !(x) ){ __debugbreak(); }
#else
#define assert( x )
#endif

#pragma comment(lib, "d3d11.lib")

#include "VS_Color.h"
#include "PS_Color.h"
#include "VS_Texture.h"
#include "PS_Texture.h"

static const D3D11_INPUT_ELEMENT_DESC SgRenderer_VertexFormat[] = 
{
	{ "SV_POSITION", 0 , DXGI_FORMAT_R32G32B32A32_FLOAT , 0 ,  0 , D3D11_INPUT_PER_VERTEX_DATA , 0 },
	{ "TEXCOORD"   , 0 , DXGI_FORMAT_R32G32_FLOAT       , 0 , 16 , D3D11_INPUT_PER_VERTEX_DATA , 0 },
	{ "COLOR"      , 0 , DXGI_FORMAT_R32G32B32A32_FLOAT , 0 , 24 , D3D11_INPUT_PER_VERTEX_DATA , 0 },
};

static class SgRenderer: public IRendererD3D11
{
private:
	struct sgVsConsts
	{
		DirectX::XMMATRIX mWVP; //vs c0
	};

private:
	IDXGISwapChain*         m_SwapChain;
	ID3D11Device*           m_Device;
	ID3D11DeviceContext*    m_Context;
	ID3D11Texture2D*        m_RtTex;
	ID3D11RenderTargetView* m_RtView;
	ID3D11VertexShader*     m_VS_Color;
	ID3D11VertexShader*     m_VS_Texture;
	ID3D11PixelShader*      m_PS_Color;
	ID3D11PixelShader*      m_PS_Texture;
	ID3D11InputLayout*      m_InputLayout;
	ID3D11Buffer*           m_VbQuad;
	ID3D11Buffer*           m_VsConstsBuffer;
	ID3D11BlendState*       m_BlendState;
	ID3D11SamplerState*     m_Sampler;
	ID3D11RasterizerState*  m_RsState;

	D3D_FEATURE_LEVEL       m_FeatureLevel;
	sgRendererInitParms     m_InitParms;
	HWND                    m_hwnd;
	sgVsConsts              m_VsConsts;
	
private:
	void ReleaseAll()
	{
		m_SwapChain->SetFullscreenState(false, NULL);

		SAFE_RELEASE( m_RsState );
		SAFE_RELEASE( m_Sampler );
		SAFE_RELEASE( m_BlendState );
		SAFE_RELEASE( m_VbQuad );
		SAFE_RELEASE( m_VsConstsBuffer );

		SAFE_RELEASE( m_InputLayout );
		
		SAFE_RELEASE( m_VS_Color );
		SAFE_RELEASE( m_VS_Texture );
		SAFE_RELEASE( m_PS_Color );
		SAFE_RELEASE( m_PS_Texture );

		SAFE_RELEASE( m_RtView );
		SAFE_RELEASE( m_RtTex );
		SAFE_RELEASE( m_Context );

		SAFE_RELEASE(m_SwapChain);

		if (m_Device)
		{
			ULONG NumLeft = m_Device->Release();
			m_Device = 0;
			assert(0 == NumLeft);
			NumLeft = NumLeft;
		}
	}

	void AdjustWindowSize()
	{
		//if( m_InitParms.Windowed )
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

public:
	SgRenderer()
	: m_Device( 0 )
	, m_hwnd( 0 )
	, m_SwapChain( 0 )
	, m_Context( 0 )
	, m_RtTex( 0 )
	, m_RtView( 0 )
	, m_VS_Color(0)
	, m_VS_Texture(0)
	, m_PS_Color(0)
	, m_PS_Texture(0)
	, m_InputLayout(0)
	, m_VbQuad( 0 )
	, m_VsConstsBuffer( 0 )
	, m_BlendState( 0 )
	, m_Sampler( 0 )
	, m_RsState( 0 )
	{
		m_VsConsts.mWVP = DirectX::XMMatrixIdentity();
	}

	void Init(  const sgRendererInitParms* InitParms )
	{
		m_InitParms = *InitParms;
		m_hwnd = static_cast<HWND>(m_InitParms.Wnd);
		HRESULT Res;
		static const D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 , D3D_FEATURE_LEVEL_11_1 };
		DXGI_SWAP_CHAIN_DESC Sd;
		memset( &Sd , 0 , sizeof(Sd) );
		Sd.BufferDesc.Width = InitParms->Width;
		Sd.BufferDesc.Height = InitParms->Height;
		Sd.BufferDesc.RefreshRate.Numerator = 0;
		Sd.BufferDesc.RefreshRate.Denominator = 1;
		Sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		Sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		Sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		Sd.SampleDesc.Count = 1;
		Sd.SampleDesc.Quality = 0;
		Sd.BufferUsage = DXGI_USAGE_BACK_BUFFER|DXGI_USAGE_RENDER_TARGET_OUTPUT;
		Sd.BufferCount = 1;
		Sd.OutputWindow = m_hwnd;
		Sd.Windowed = TRUE;//InitParms->Windowed;
		Sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		Sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		Res = D3D11CreateDeviceAndSwapChain( NULL , D3D_DRIVER_TYPE_HARDWARE, NULL , 0 , FeatureLevels , countof(FeatureLevels), D3D11_SDK_VERSION , &Sd , &m_SwapChain , &m_Device , &m_FeatureLevel , &m_Context );
		if( FAILED( Res ) )
		{
			ReleaseAll();
			return;
		}

		if( m_InitParms.Windowed )
		{

		}
		else
		{
			m_SwapChain->SetFullscreenState( TRUE , NULL );
		}

		m_SwapChain->GetBuffer( 0 , __uuidof(m_RtTex) , reinterpret_cast<void**>(&m_RtTex) );
		Res = m_Device->CreateRenderTargetView( m_RtTex , NULL , &m_RtView );
		assert( SUCCEEDED(Res) );

		//Create Pixel Shaders
		Res = m_Device->CreatePixelShader( g_PS_Color , sizeof(g_PS_Color) , NULL , &m_PS_Color );
		assert( SUCCEEDED(Res) );
		Res = m_Device->CreatePixelShader( g_PS_Texture , sizeof(g_PS_Texture) , NULL , &m_PS_Texture );
		assert( SUCCEEDED(Res) );
		//Create Vertex Shaders
		Res = m_Device->CreateVertexShader( g_VS_Texture , sizeof(g_VS_Texture) , NULL , &m_VS_Texture );
		assert( SUCCEEDED(Res) );
		Res = m_Device->CreateVertexShader( g_VS_Color , sizeof(g_VS_Color) , NULL , &m_VS_Color );
		assert( SUCCEEDED(Res) );
		//Create Vertex Layouts
		Res = m_Device->CreateInputLayout( SgRenderer_VertexFormat , countof(SgRenderer_VertexFormat) , g_VS_Texture , sizeof(g_VS_Texture) , &m_InputLayout );
		assert( SUCCEEDED(Res) );
		//Verify other shaders input values
		Res = m_Device->CreateInputLayout( SgRenderer_VertexFormat , countof(SgRenderer_VertexFormat) , g_VS_Color , sizeof(g_VS_Color) , NULL );
		assert( SUCCEEDED(Res) );


		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth           = sizeof( sgVsConsts );
		cbDesc.Usage               = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags           = 0;
		cbDesc.StructureByteStride = 0;

		Res = m_Device->CreateBuffer( &cbDesc , NULL , &m_VsConstsBuffer );
		assert( SUCCEEDED( Res ) );

		
		const SgVert Verts[] =
		{
			{ { -0.5f, -0.5f, 0.0f , 1.0f } , { 0 , 1 } , { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ { -0.5f,  0.5f, 0.0f , 1.0f } , { 0 , 0 } , { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ {  0.5f, -0.5f, 0.0f , 1.0f } , { 1 , 1 } , { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ {  0.5f,  0.5f, 0.0f , 1.0f } , { 1 , 0 } , { 1.0f, 1.0f, 1.0f, 1.0f } },
		};

		// Fill in a buffer description.
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage            = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth        = sizeof( Verts );
		bufferDesc.BindFlags        = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags   = 0;
		bufferDesc.MiscFlags        = 0;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = Verts;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		// Create the vertex buffer.
		Res = m_Device->CreateBuffer( &bufferDesc, &InitData, &m_VbQuad );
		assert( SUCCEEDED( Res ) );


		D3D11_SAMPLER_DESC SampDesc;
		SampDesc.Filter   = false ? D3D11_FILTER_MIN_MAG_MIP_POINT : D3D11_FILTER_ANISOTROPIC;
		SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		SampDesc.MipLODBias = 0;
		SampDesc.MaxAnisotropy = 16;
		SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		SampDesc.BorderColor[0] = 0.0f;
		SampDesc.BorderColor[1] = 0.0f;
		SampDesc.BorderColor[2] = 0.0f;
		SampDesc.BorderColor[3] = 0.0f;
		SampDesc.MinLOD = -D3D11_FLOAT32_MAX;
		SampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		Res = m_Device->CreateSamplerState( &SampDesc , &m_Sampler );
		assert( SUCCEEDED(Res) );

		D3D11_BLEND_DESC BlendDesc;
		BlendDesc.AlphaToCoverageEnable = FALSE;
		BlendDesc.IndependentBlendEnable = FALSE;
		BlendDesc.RenderTarget[0].BlendEnable = TRUE;
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Res = m_Device->CreateBlendState( &BlendDesc , &m_BlendState );
		assert( SUCCEEDED(Res) );

		D3D11_RASTERIZER_DESC RsDesc;
		RsDesc.FillMode = D3D11_FILL_SOLID;
		RsDesc.CullMode = D3D11_CULL_BACK;
		RsDesc.FrontCounterClockwise = FALSE;
		RsDesc.DepthBias = 0;
		RsDesc.SlopeScaledDepthBias = 0;
		RsDesc.DepthBiasClamp = 0;
		RsDesc.DepthClipEnable = TRUE;
		RsDesc.ScissorEnable = FALSE;
		RsDesc.MultisampleEnable = FALSE;
		RsDesc.AntialiasedLineEnable = FALSE;
		Res = m_Device->CreateRasterizerState( &RsDesc , &m_RsState );
		assert( SUCCEEDED(Res) );

		if( m_InitParms.Windowed )
		{
			AdjustWindowSize();
		}
		else
		{
			m_SwapChain->SetFullscreenState( TRUE , NULL );
		}

		ShowCursor(FALSE);
	}

	void Deinit()
	{
		ReleaseAll();
		ShowCursor(TRUE);
	}

	void UpdateBounds()
	{
		//m_SwapChain->ResizeBuffers( 1, m_InitParms.Width , m_InitParms.Height , DXGI_FORMAT_R8G8B8A8_UNORM , DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH );
		//DXGI_MODE_DESC Desc;
		//Desc.Width = m_InitParms.Width;
		//Desc.Height = m_InitParms.Height;
		//Desc.Format  = DXGI_FORMAT_R8G8B8A8_UNORM;
		//Desc.RefreshRate.Denominator = 0;
		//Desc.RefreshRate.Numerator = 0;
		//Desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		//Desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		//m_SwapChain->ResizeTarget( &Desc );
		//m_SwapChain->SetFullscreenState( !m_InitParms.Windowed , NULL );
	}

	void OnActivateApp()
	{
		if( !m_InitParms.Windowed )
		{
			m_SwapChain->SetFullscreenState( TRUE , NULL );
		}
	}

	void UpdloadConsts()
	{
		D3D11_MAPPED_SUBRESOURCE Resource;
		m_Context->Map( m_VsConstsBuffer , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &Resource );
		memcpy( Resource.pData , &m_VsConsts , sizeof(m_VsConsts) );
		m_Context->Unmap( m_VsConstsBuffer , 0 );
		m_Context->VSSetConstantBuffers( 0 , 1 , &m_VsConstsBuffer );
	}

	void BeginFrame()
	{
		D3D11_VIEWPORT Vp;
		memset(&Vp, 0, sizeof(Vp));
		
		Vp.TopLeftX = 0;
		Vp.TopLeftY = 0;
		Vp.Width    = static_cast<float>(m_InitParms.Width);
		Vp.Height   = static_cast<float>(m_InitParms.Height);
		Vp.MinDepth = 0;
		Vp.MaxDepth = 1.0f;
		
		m_Context->OMSetRenderTargets( 1 , &m_RtView , NULL );
		m_Context->OMSetBlendState( m_BlendState , NULL , 0xFFFFFFFF );
		m_Context->RSSetViewports(1, &Vp);
		FLOAT Color[] = { 0.25f , 0.25f, 1.0f, 1.0f };
		m_Context->ClearRenderTargetView( m_RtView , Color );
		m_Context->PSSetSamplers( 0 , 1 , &m_Sampler );
		m_Context->RSSetState( m_RsState );
		m_Context->IASetInputLayout( m_InputLayout );
	}

	void EndFrame()
	{
		m_SwapChain->Present(0,0);
		m_Context->ClearState();
	}

	virtual void DrawQuad( struct ID3D11ShaderResourceView* Texture , float x , float y , float Width , float Height )
	{
		//Test Draw!
		UINT Stride = sizeof(SgVert);
		UINT Offset = 0;
		m_Context->IASetVertexBuffers( 0 , 1 , &m_VbQuad , &Stride , &Offset );
		m_Context->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

		DirectX::XMMATRIX WVP = DirectX::XMMatrixIdentity();
		WVP *= DirectX::XMMatrixScaling( Width/VIEW_WIDTH , Height/VIEW_HEIGHT , 1.0f );
		WVP *= DirectX::XMMatrixTranslation( ((x+Width/2.0f)-VIEW_WIDTH/2.0f)/VIEW_WIDTH , -((y+Height/2.0f)-VIEW_HEIGHT/2.0f)/VIEW_HEIGHT , 0 );
		WVP *= DirectX::XMMatrixScaling( 2.0f , 2.0f , 1.0f );
		//WVP *= DirectX::XMMatrixScaling( ((float)m_InitParms.Height/(float)m_InitParms.Width) , 1.0f , 1.0f );
		m_VsConsts.mWVP = DirectX::XMMatrixTranspose( WVP );
		UpdloadConsts();
		m_Context->VSSetShader( m_VS_Texture , 0 , NULL );
		m_Context->PSSetShader( m_PS_Texture , 0 , NULL );
		m_Context->PSSetShaderResources( 0 , 1 , &Texture );
		m_Context->Draw( 4 , 0 );
	}

	class SgRendererImage* CreateSprite( const sgRendererImageCreateParms* CreateParms )
	{
		sgRendererData Data;
		Data.Dd = 0;
		Data.BackSurface = 0;
		Data.Dev11 = m_Device;
		Data.DevContext11 = m_Context;
		Data.Renderer = this;
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

void Renderer_OnActivateApp()
{
	Renderer.OnActivateApp();
}

#endif __DXVERSION__ == 11