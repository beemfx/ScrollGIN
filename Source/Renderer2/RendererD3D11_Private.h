#pragma once

struct SgVert
{
	sg_vec4  Pos;   // : POSITION;
	sg_vec2  Tex0;  // : TEXCOORD0;
	sg_color Color; // : COLOR0;
};

class IRendererD3D11
{
	public: virtual void DrawQuad( struct ID3D11ShaderResourceView* Texture , float x , float y , float Width , float Height )=0;
};