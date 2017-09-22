/// <summary>
/// パーティクル用ジオメトリシェーダ
/// </summary>
#include "Particle.hlsli"

static const float PI = 3.14159265f;

[maxvertexcount(1)]
void main(
	point GSInput input[1],
	inout PointStream< PSInput > output
)
{
	PSInput element;	

	element.pos = mul(input[0].pos, g_WVP);
	element.color = input[0].color;
	element.uv = input[0].uv;
	output.Append(element);
}