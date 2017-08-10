//--------------------------------------------------------------------------------------
// Constant Buffers
//--------------------------------------------------------------------------------------
cbuffer global
{
	row_major matrix g_WVP; //ワールドから射影までの変換行列（列優先）
};

//-----------------------------------------------------------------------------------------
// Textures and Samplers
//-----------------------------------------------------------------------------------------
Texture2D g_texture: register(t0);
SamplerState g_sampler : register(s0);

// バーテックスシェーダの出力
struct VSOutput
{
	float4 pos : SV_POSITION;
};

// ジオメトリシェーダの出力
struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

// 頂点シェーダエントリーポイント
VSOutput VSmain( float4 pos : POSITION)
{
	VSOutput output;
	output.pos = mul(pos, g_WVP);

	return output;
}

// ジオメトリシェーダエントリーポイント
//[maxvertexcount(3)]
//void GSmain(
//	triangle float4 input[3] : POSITION,
//	inout TriangleStream< GSOutput > output
//)
//{
//	for (uint i = 0; i < 3; i++)
//	{
//		GSOutput element;
//		element.pos = input[i];
//		output.Append(element);
//	}
//}
//
//// ピクセルシェーダエントリーポイント
//float4 PSmain(GSOutput input) : SV_TARGET
//{
//	//return float4(In.UV.x, In.UV.y, 0, 1);
//	//return In.Color;
//	//return g_texture.Sample(g_sampler, float2(0.5f, 0.5f));
//	return g_texture.Sample(g_sampler, input.uv);
////return float4(1.0f, 1.0f, 0.0f, 1.0f);
//}