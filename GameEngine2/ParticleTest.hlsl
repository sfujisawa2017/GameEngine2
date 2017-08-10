//--------------------------------------------------------------------------------------
// Constant Buffers
//--------------------------------------------------------------------------------------
cbuffer global
{
	row_major matrix g_WVP; //���[���h����ˉe�܂ł̕ϊ��s��i��D��j
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
};

// �W�I���g���V�F�[�_�̏o��
struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

// ���_�V�F�[�_�G���g���[�|�C���g
VSOutput VSmain( float4 pos : POSITION)
{
	VSOutput output;
	output.pos = mul(pos, g_WVP);

	return output;
}

// �W�I���g���V�F�[�_�G���g���[�|�C���g
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
//// �s�N�Z���V�F�[�_�G���g���[�|�C���g
//float4 PSmain(GSOutput input) : SV_TARGET
//{
//	//return float4(In.UV.x, In.UV.y, 0, 1);
//	//return In.Color;
//	//return g_texture.Sample(g_sampler, float2(0.5f, 0.5f));
//	return g_texture.Sample(g_sampler, input.uv);
////return float4(1.0f, 1.0f, 0.0f, 1.0f);
//}