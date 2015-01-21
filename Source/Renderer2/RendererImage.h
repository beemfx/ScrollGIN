////////////////////////////////////////////////////////////////////////////
//
// SgSprite - The main drawing object for the Renderer.
//
// (c) 2015 Blaine Myers
//
////////////////////////////////////////////////////////////////////////////
#pragma once


union sgColor
{
	unsigned __int32 Color;
	struct
	{
		unsigned __int8 a;
		unsigned __int8 r;
		unsigned __int8 g;
		unsigned __int8 b;
	};
};

enum RENDERER_IMAGE_T
{
	RENDERER_IMAGE_UNK,
	RENDERER_IMAGE_BITMAP,
	RENDERER_IMAGE_COLOR,
};

static const int IMAGE_CREATE_DEFAULT = -1;

struct sgRendererImageCreateParms
{
	RENDERER_IMAGE_T Type;
	int              Width;
	int              Height;
	sgColor          Color;
	int              BmX;
	int              BmY;
	int              BmWidth;
	int              BmHeight;
	char             BmFile[256];
	int              BmFileOffset;
};

struct sgRendererData
{
	struct IDirectDraw7*        Dd;
	struct IDirectDrawSurface7* BackSurface;
};

class SgRendererImage
{
public:
	SgRendererImage( const sgRendererImageCreateParms* CreateParms , const sgRendererData* RendererData );
	~SgRendererImage();

	void Draw( int x , int y );
	void Restore();
private:
	void CreateBitmap();
	void CreateColor();
private:
	struct sgData;
	sgData* m_D;
};