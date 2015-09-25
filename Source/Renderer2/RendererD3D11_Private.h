#if __DXVERSION__ == 11
#pragma once

struct SgVert
{
	#if 1
	sg_vec4  Pos;   // : POSITION;
	sg_vec2  Tex0;  // : TEXCOORD0;
	sg_color Color; // : COLOR0;
	#else
	sg_vec4   Pos;
	sg_vec4   Norm;
	sg_vec4   Tan;
	sg_vec2   Tex0;
	sg_vec2   Tex1;
	sg_color  Color;
	sg_uint32 Bone;
	float     Weight;
	sg_uint32 Bone2;
	float     Weight2;
	#endif
};

class IRendererD3D11
{
	public: virtual void DrawQuad( struct ID3D11ShaderResourceView* Texture , float x , float y , float Width , float Height )=0;
};
#endif __DXVERSION__ == 11