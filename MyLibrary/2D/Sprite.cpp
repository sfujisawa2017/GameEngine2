#include "Sprite.h"

using namespace DirectX;
using namespace MyLibrary;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="manager">�X�v���C�g�����_���[</param>
Sprite::Sprite()
{
	m_Rotation = 0.0f;
	m_Scale = Vector2(1, 1);
	m_Color = Colors::White;
	m_Texture = nullptr;
	m_Rect = RECT{0,0,0,0};
	m_AnchorPoint = Vector2(0.5f, 0.5f);
	m_Parent = nullptr;
	m_SpriteManager = nullptr;
}

void Sprite::Draw()
{
	ID3D11ShaderResourceView*	shaderResourceView = nullptr;

	Vector2 origin(0, 0);

	if (m_Texture)
	{
		shaderResourceView = m_Texture->shaderResourceView.Get();

		origin = m_AnchorPoint;
		origin.x *= m_Texture->desc.Width;
		origin.y *= m_Texture->desc.Height;
	}

	float rotation = XMConvertToRadians(m_Rotation);

	m_SpriteManager->Draw(shaderResourceView, m_Position, &m_Rect, m_Color, rotation, origin, m_Scale );
}

void Sprite::SetParent(Sprite* parent)
{
	// �e�I�u�W�F�N�g�ݒ�
	this->m_Parent = parent;
	// �q�I�u�W�F�N�g���X�g�ɒǉ�
	parent->m_Children.push_back(this);
}

void Sprite::AddChild(Sprite* child)
{
	// �e�I�u�W�F�N�g�ݒ�
	child->m_Parent = this;
	// �q�I�u�W�F�N�g���X�g�ɒǉ�
	this->m_Children.push_back(child);
}

SpriteFactory::SpriteFactory(SpriteRenderer * spriteRenderer, TextureCache* textureCache)
{
	m_SpriteRenderer = spriteRenderer;	
	m_TextureCache = textureCache;
}

std::unique_ptr<Sprite> SpriteFactory::CreateFromFile(const wchar_t* filename)
{
	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();

	sprite->SetSpriteManager(m_SpriteRenderer);
	
	// �e�N�X�`�����[�h
	Texture* texture = m_TextureCache->LoadTexture(filename);
	sprite->SetTexture(texture);

	RECT rect = RECT{0,0,0,0};
	rect.right = texture->desc.Width;
	rect.bottom = texture->desc.Height;
	sprite->SetRect(rect);

	return std::move(sprite);
}