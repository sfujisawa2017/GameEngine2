//--------------------------------------------------------------------------------------
// コンスタントバッファ
//--------------------------------------------------------------------------------------
cbuffer global
{
	row_major matrix g_WVP; //ワールドから射影までの変換行列（列優先）
};

//-----------------------------------------------------------------------------------------
// テクスチャとサンプラー
//-----------------------------------------------------------------------------------------
Texture2D g_texture: register(t0);
SamplerState g_sampler : register(s0);

//--------------------------------------------------------------------------------------
// ジオメトリシェーダへの入力
//--------------------------------------------------------------------------------------
struct GSInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 other : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// ピクセルシェーダへの入力
//--------------------------------------------------------------------------------------
struct PSInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};