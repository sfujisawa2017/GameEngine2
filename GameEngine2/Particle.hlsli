//--------------------------------------------------------------------------------------
// Constant Buffers
//--------------------------------------------------------------------------------------
cbuffer global
{
	row_major matrix g_VP; //ワールドから射影までの変換行列（列優先）
	row_major matrix g_Billboard;	// ビルボード行列（列優先）
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
	float4 color : COLOR;
	float2 other : TEXCOORD;
};

// ジオメトリシェーダの出力
struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};