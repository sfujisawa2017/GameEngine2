#include "ParticleTest.hlsli"

[maxvertexcount(4)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element = (GSOutput)0;

	float4 basepos = mul(input[0].pos, g_VP);
	float4 addpos;

	addpos = float4(-0.1f, 0.1f, 0, 0);
	addpos = mul(addpos, g_Billboard);
	element.pos = mul(input[0].pos + addpos, g_VP);
	element.color = input[0].color;
	element.uv = float2(0, 1);
	output.Append(element);

	addpos = float4(0.1f, 0.1f, 0, 0);
	addpos = mul(addpos, g_Billboard);
	element.pos = mul(input[0].pos + addpos, g_VP);
	element.color = input[0].color;
	element.uv = float2(0, 0);
	output.Append(element);

	addpos = float4(-0.1f, -0.1f, 0, 0);
	addpos = mul(addpos, g_Billboard);
	element.pos = mul(input[0].pos + addpos, g_VP);
	element.color = input[0].color;
	element.uv = float2(1, 1);
	output.Append(element);

	addpos = float4(0.1f, -0.1f, 0, 0);
	addpos = mul(addpos, g_Billboard);
	element.pos = mul(input[0].pos + addpos, g_VP);
	element.color = input[0].color;
	element.uv = float2(1, 0);
	output.Append(element);

	//現在のストリップを終了
	output.RestartStrip();
}