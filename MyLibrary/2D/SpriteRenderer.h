/// <summary>
/// �X�v���C�g�`��N���X
/// </summary>
#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <memory>
#include <map>
#include <string>
#include <d3d11_1.h>

#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <DirectXColors.h>
#include <CommonStates.h>

#include "TextureCache.h"

namespace MyLibrary
{
	class SpriteRenderer
	{
	public:
		static SpriteRenderer* GetInstance();
	private:
		static std::unique_ptr<SpriteRenderer> m_Instance;
	public:
		// ������
		void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
		
		// �`���t�J�n
		void Begin();
		// �`���t�I��
		void End();
		// �`��
		void Draw(ID3D11ShaderResourceView* texture, DirectX::XMFLOAT2 const& position, RECT const* sourceRectangle, DirectX::FXMVECTOR color = DirectX::Colors::White, float rotation = 0, const DirectX::XMFLOAT2& origin = DirectX::SimpleMath::Vector2(0,0), const DirectX::XMFLOAT2& scale = DirectX::SimpleMath::Vector2(1,1), DirectX::SpriteEffects effects = DirectX::SpriteEffects_None, float layerDepth = 0);
	private:
		// �X�v���C�g�o�b�`
		std::unique_ptr<DirectX::SpriteBatch>	m_SpriteBatch;
		// �ėp�`��ݒ�
		std::unique_ptr<DirectX::CommonStates>	m_CommonStates;
	};
}