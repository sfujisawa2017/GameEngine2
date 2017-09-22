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

// 左上が0,0　右下が1,1
static const float2 uv_array[vnum] =
{
	float2(0, 1),	// 左下
	float2(1, 1),	// 右下
	float2(0, 0),	// 左上
	float2(1, 0) 	// 右上
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

	float4 offset = float4(0,0,0,0);

	// 回転角
	float rotation = input[0].other.y;

	for (uint i = 0; i < vnum; i++)
	{
		offset.x = offset_array[i].x * cos(rotation) + offset_array[i].y * +sin(rotation);
		offset.y = offset_array[i].x * -sin(rotation) + offset_array[i].y * +cos(rotation);
		offset = mul(offset * input[0].other.x, g_Billboard);
		element.pos = mul(input[0].pos + offset, g_VP);
		element.uv = uv_array[i];
		output.Append(element);
	}
}