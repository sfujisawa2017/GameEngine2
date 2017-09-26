#include "Particle.hlsli"

/// <summary>
/// ピクセルシェーダー
/// </summary>

float4 main(GSOutput input) : SV_TARGET
{
	// テクスチャマッピングによる色に、頂点カラーを掛け算
	float4 color = g_texture.Sample(g_sampler, input.uv) * input.color;

	return color;
}