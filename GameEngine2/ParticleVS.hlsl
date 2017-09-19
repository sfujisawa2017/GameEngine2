#include "Particle.hlsli"

PSInput main( float4 pos : POSITION, float4 color : COLOR )
{
	PSInput Out;

	Out.pos = pos;
	Out.color = color;

	return Out;
}