#include "ParticleTest.hlsli"

/// <summary>
/// ���_�V�F�[�_�[
/// 
/// �r���[�v���W�F�N�V�����ϊ��̓W�I���g���V�F�[�_�ōs���̂ŁA
/// �����ł͂Ȃɂ��������̂܂ܐ��l���󂯓n��
/// </summary>
VSOutput main( float4 pos : POSITION, float4 color : COLOR, float2 other : TEXCOORD)
{
	VSOutput output;

	output.pos = pos;
	//output.pos = mul(pos, g_WVP);
	output.color = color;
	output.other = other;

	return output;
}