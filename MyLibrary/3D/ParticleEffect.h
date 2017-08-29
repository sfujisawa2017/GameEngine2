/// <summary>
/// �p�[�e�B�N���G�t�F�N�g
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <vector>
#include <list>
#include <map>
#include "Obj3D.h"
#include "..\Camera\Camera.h"

namespace MyLibrary
{
	class ParticleEffectManager
	{
	public:
		using Vector3 = DirectX::SimpleMath::Vector3;
		using Color = DirectX::SimpleMath::Color;

		static const int PARTICLE_NUM_MAX;

		// ���̓��C�A�E�g�̒�`.
		static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

		// �R���X�^���g�o�b�t�@
		struct Constants
		{
			DirectX::SimpleMath::Matrix VP;
			DirectX::SimpleMath::Matrix Billboard;
		};

		// �V���O���g���C���X�^���X�̎擾
		static ParticleEffectManager* getInstance();
		// ������
		void Initialize();
		// �e�N�X�`�����[�h
		void Load(int texnumber, const wchar_t* filename);
		// �S�j��
		void DisposeAll();
		// �w��ԍ��̃p�[�e�B�N���O���[�v��j��
		void Dispose(int groupNumber);
		// ����������
		void Entry(int groupNumber, int frame, Vector3 position, Vector3 velocity, Vector3 accel, float start_rotation, float end_rotation, float start_scale, float end_scale, Color start_color, Color end_color);
		void Update();
		void Draw();

		// �A�N�Z�b�T
		void SetCamera(Camera* camera) { m_Camera = camera; }
	private:
		ParticleEffectManager();

		// �G�t�F�N�g1��
		class ParticleEffect
		{
		public:
			ParticleEffect()
			{
				frame = 0;
				num_frame = 0;
			}

			bool Update(const Vector3& gravity);

			//// �I�u�W�F�N�g
			//Obj3D obj;
			// ���W
			Vector3 position;
			// ���x
			Vector3 velocity;
			// �����x
			Vector3 accel;
			// �F
			Color color;
			// ��]
			float rotation;
			// �X�P�[��
			float scale;
			// �����l
			float s_rotation;
			float s_scale;
			Color s_color;
			// �ŏI�l
			float e_rotation;
			float e_scale;
			Color e_color;
			// ���݃t���[��
			int   frame;
			// �I���t���[��
			int   num_frame;
		};

		// �����e�N�X�`�����g�p����G�t�F�N�g�O���[�v
		struct ParticleEffectGroup
		{
			// �e�N�X�`��
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;
			// ���f���G�t�F�N�g�̔z��
			std::list<ParticleEffect> m_ParticleEffects;
			// ���_�f�[�^�z��
			std::vector<DirectX::VertexPositionColorTexture> m_Vertices;
		};

		// ���_�V�F�[�_
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		// �W�I���g���V�F�[�_
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_GeometryShader;
		// �s�N�Z���V�F�[�_
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		// �R���X�^���g�o�b�t�@
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
		// �v���~�e�B�u�o�b�`
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_PrimitiveBatch;
		// ���̓��C�A�E�g
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
		//�e�N�X�`���[�T���v���[
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_Sampler;

		// �J����
		Camera*	m_Camera;

		// �p�[�e�B�N���O���[�v�̔z��
		std::map<int, ParticleEffectGroup> m_ParticleGroups;
		// �d�͉����x
		Vector3 gravity;
		
	};
}