#include "ParticleTest.hlsli"

/// <summary>
/// ピクセルシェーダー
/// </summary>

float4 main(GSOutput input) : SV_TARGET
{
	//return float4(In.UV.x, In.UV.y, 0, 1);
	//return In.Color;
	//return g_texture.Sample(g_sampler, float2(0.5f, 0.5f));
	// テクスチャマッピングによる色に、頂点カラーを掛け算
	return g_texture.Sample(g_sampler, input.uv) * input.color;
	//return float4(1.0f, 1.0f, 0.0f, 1.0f);
}