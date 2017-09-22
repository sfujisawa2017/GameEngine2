/// <summary>
/// パーティクル用ジオメトリシェーダ
/// </summary>
#include "Particle.hlsli"

// 四角形の頂点数
static const uint vnum = 4;

// センターからのオフセット
// 左下が-1,-1 右上が1,1
static const float4 offset_array[vnum] =
{
	float4(-0.5f,-0.5f, 0, 0),	// 左下
	float4(0.5f,-0.5f, 0, 0),	// 右下
	float4(-0.5f, 0.5f, 0, 0),	// 左上
	float4(0.5f, 0.5f, 0, 0)	// 右上
};

[maxvertexcount(4)]
void main(
	point GSInput input[1],
	inout TriangleStream< PSInput > output
)
{
	PSInput element;	

	// 共通
	element.color = input[0].color;
	element.uv = float2(0.5f, 0.5f);

	for (uint i = 0; i < vnum; i++)
	{
		element.pos = mul(input[0].pos + offset_array[i] * input[0].other.x, g_WVP);
		output.Append(element);
	}
}