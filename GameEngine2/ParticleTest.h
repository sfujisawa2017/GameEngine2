#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>

#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <SimpleMath.h>
#include <vector>

#include "MyLibrary.h"

class ParticleTest
{
public:
	static const int PARTICLE_NUM_MAX;

	// ���̓��C�A�E�g�̒�`.
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// �R���X�^���g�o�b�t�@
	struct Constants
	{
		DirectX::SimpleMath::Matrix VP;
		DirectX::SimpleMath::Matrix Billboard;
	};

public:
	ParticleTest();

	void Update();

	void Draw();
	
protected:
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_GeometryShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	// ���[���h�r���[�v���W�F�N�V�����s��p�R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pWVPConstantBuffer;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_PrimitiveBatch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

	std::vector<DirectX::VertexPositionColorTexture> m_Vertices;
	DirectX::SimpleMath::Matrix m_WVP;
	//�e�N�X�`���[�̃T���v���[
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_Sampler;
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;

	std::unique_ptr<MyLibrary::DebugCamera> m_Camera;

	std::unique_ptr<DirectX::CommonStates> m_CommonStates;
};