#include "Particle.hlsli"

/// <summary>
/// 頂点シェーダー
/// 
/// ビュープロジェクション変換はジオメトリシェーダで行うので、
/// ここではなにもせずそのまま数値を受け渡す
/// </summary>
VSOutput main( float4 pos : POSITION, float4 color : COLOR, float2 other : TEXCOORD)
{
	VSOutput output;

	output.pos = pos;
	output.color = color;
	output.other = other;

	return output;
}