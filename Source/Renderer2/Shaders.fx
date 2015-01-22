struct SgVert
{
	float4 Pos     : SV_POSITION;
	float2 Tex0    : TEXCOORD0;
	float4 Color   : COLOR0;
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

float4 PS_Color( SgVert IN ) : SV_Target
{
	return IN.Color;
}

SgVert VS_Texture( SgVert IN )
{
	SgVert OUT = IN;
	OUT.Pos = mul( IN.Pos , g_mWVP );
	return OUT;
}

float4 PS_Texture( SgVert IN ) : SV_Target
{
	return g_tex0.Sample( g_samp0 , IN.Tex0 );// * IN.Color;
}