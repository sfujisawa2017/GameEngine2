/// <summary>
/// �Q�c�摜�\���N���X
/// </summary>
#pragma once

#include <memory>
#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <vector>

#include "TextureCache.h"
#include "SpriteRenderer.h"

namespace MyLibrary
{
	// �Q�c�摜�\���N���X
	class Sprite
	{
	public:
		friend SpriteRenderer;
		using Vector2 = DirectX::SimpleMath::Vector2;
		using Matrix = DirectX::SimpleMath::Matrix;
		using Color = DirectX::SimpleMath::Color;

		// �R���X�g���N�^
		Sprite();
		// �`��
		void Draw();

		void SetParent(Sprite * parent);

		void AddChild(Sprite * child);

		/// <summary>
		/// property
		/// </summary>
		void SetPosition(const Vector2& position) { m_Position = position; }
		void SetRotation(float rotation) { m_Rotation = rotation; }
		void SetScale(float scale) { m_Scale = Vector2(scale, scale); }
		void SetScale(const Vector2& scale) { m_Scale = scale; }
		void SetColor(const Color& color) { m_Color = color; }
		void SetTexture(Texture* texture) { m_Texture = texture; }
		void SetRect(const RECT& rect) { m_Rect = rect; }
		void SetAnchorPoint(const Vector2& anchor) { m_AnchorPoint = anchor; }
		void SetSpriteManager(SpriteRenderer* manager) { m_SpriteManager = manager; }

	private:
		// ���W
		Vector2 m_Position;
		// ��]
		float m_Rotation;
		// �X�P�[�����O
		Vector2 m_Scale;
		// ���[���h�s��
		Matrix m_LocalWorld;
		// �F
		Color m_Color;
		// �e�N�X�`��
		Texture* m_Texture;
		// �e�N�X�`����`
		RECT m_Rect;
		// �摜���S
		Vector2 m_AnchorPoint;
		// �e�I�u�W�F�N�g�ւ̃|�C���^
		Sprite* m_Parent;
		// �q�I�u�W�F�N�g�ւ̃|�C���^�z��
		std::vector<Sprite*>	m_Children;
		// �X�v���C�g�����_���[�ւ̃|�C���^
		SpriteRenderer* m_SpriteManager;
	};

	// �X�v���C�g�t�@�N�g���[
	class SpriteFactory
	{
	public:
		SpriteFactory(SpriteRenderer * spriteRenderer, TextureCache* textureCache);
		std::unique_ptr<Sprite> CreateFromFile(const wchar_t* filename);
	private:
		// �X�v���C�g�����_���[�ւ̃|�C���^
		SpriteRenderer* m_SpriteRenderer;
		// �e�N�X�`���L���b�V��
		TextureCache* m_TextureCache;
	};
}