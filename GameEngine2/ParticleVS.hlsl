#include "Particle.hlsli"

PSInput main( float4 pos : POSITION, float4 color : COLOR )
{
	PSInput Out;
	Out.pos = mul(pos, g_WVP);
	Out.color = color;

	return Out;
}