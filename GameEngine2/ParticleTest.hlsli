//--------------------------------------------------------------------------------------
// Constant Buffers
//--------------------------------------------------------------------------------------
cbuffer global
{
	row_major matrix g_VP; //���[���h����ˉe�܂ł̕ϊ��s��i��D��j
	row_major matrix g_Billboard;	// �r���{�[�h�s��i��D��j
};

//-----------------------------------------------------------------------------------------
// Textures and Samplers
//-----------------------------------------------------------------------------------------
Texture2D g_texture: register(t0);
SamplerState g_sampler : register(s0);

// �o�[�e�b�N�X�V�F�[�_�̏o��
struct VSOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

// �W�I���g���V�F�[�_�̏o��
struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};