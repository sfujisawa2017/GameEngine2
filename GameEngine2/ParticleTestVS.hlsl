#include "ParticleTest.hlsli"

// ���_�V�F�[�_�[
VSOutput main( float4 pos : POSITION, float4 color : COLOR)
{
	VSOutput output;
	output.pos = pos;
	//output.pos = mul(pos, g_WVP);
	output.color = color;

	return output;
}