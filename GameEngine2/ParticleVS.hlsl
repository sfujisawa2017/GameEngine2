/// <summary>
/// パーティクル用頂点シェーダ
/// </summary>
#include "Particle.hlsli"

GSInput main( float4 pos : POSITION, float4 color : COLOR, float2 uv : TEXCOORD )
{
	GSInput Out;
	//Out.pos = mul(pos, g_WVP);
	Out.pos = pos;
	Out.color = color;
	Out.uv = uv;

	return Out;
}