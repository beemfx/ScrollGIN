
#define SG_POSITION  SV_POSITION
#define SG_PS_OUTPUT SV_TARGET

struct SgVert
{
	#if 1
	float4 Pos     : SG_POSITION;
	float2 Tex0    : TEXCOORD0;
	float4 Color   : COLOR0;
	#else
	float4 Pos     : SG_POSITION;
	float4 Norm    : NORMAL;
	float4 Tan     : TANGENT;
	float2 Tex0    : TEXCOORD0;
	float2 Tex1    : TEXCOORD1;
	float4 Color   : COLOR0;
	uint   Bone    : BLENDINDICES0;
	float  Weight  : BLENDWEIGHT0;
	uint   Bone2   : BLENDINDICES1;
	float  Weight2 : BLENDWEIGHT1;
	#endif
};

float4x4     g_mWVP  : register( vs , c0 ) : WORLDVIEWPROJECTION;
SamplerState g_samp0 : register( ps , s0 );
Texture2D    g_tex0  : register( ps , t0 );

SgVert VS_Color( SgVert IN )
{
	SgVert OUT = IN;
	OUT.Pos = mul( IN.Pos , g_mWVP );
	return OUT;
}

float4 PS_Color( SgVert IN ) : SG_PS_OUTPUT
{
	return IN.Color;
}

SgVert VS_Texture( SgVert IN )
{
	SgVert OUT = IN;
	OUT.Pos = mul( IN.Pos , g_mWVP );
	return OUT;
}

float4 PS_Texture( SgVert IN ) : SG_PS_OUTPUT
{
	return g_tex0.Sample( g_samp0 , IN.Tex0 ) * IN.Color;
}