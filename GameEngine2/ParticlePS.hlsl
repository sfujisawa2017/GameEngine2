#include "Particle.hlsli"

float4 main(PSInput In) : SV_TARGET
{
	float4 color = g_texture.Sample(g_sampler, In.uv) * In.color;

	return color;
}