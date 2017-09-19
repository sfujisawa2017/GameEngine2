#include "Particle.hlsli"

PSInput main( float4 pos : POSITION, float4 color : COLOR, float2 uv : TEXCOORD )
{
	PSInput Out;
	Out.pos = mul(pos, g_WVP);
	Out.color = color;
	Out.uv = uv;

	return Out;
}