#include "TextureCache.h"

#include <WICTextureLoader.h>

using namespace DirectX;
using namespace MyLibrary;

// �萔
// ���\�[�X�f�B���N�g���p�X
const std::wstring TextureCache::RESOURCE_DIRECTORY = L"Resources/Textures/";
// �t�@�C���g���q
const std::wstring TextureCache::RESOURCE_EXT = L".png";

// �ÓI�����o�ϐ��̎���
std::unique_ptr<TextureCache> TextureCache::m_Instance;

TextureCache * MyLibrary::TextureCache::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance.reset(new TextureCache);
	}

	return m_Instance.get();
}

TextureCache::TextureCache()
{
	m_d3dDevice = nullptr;
}

void TextureCache::Initialize(ID3D11Device * d3dDevice)
{
	m_d3dDevice = d3dDevice;
}

/// <summary>
/// �t�@�C�������w�肵�ăe�N�X�`���ǂݍ���
/// </summary>
/// <param name="filename">�e�N�X�`���t�@�C����</param>
/// <returns>�ǂݍ��񂾃e�N�X�`��</returns>
Texture* TextureCache::LoadTexture(const wchar_t * filename)
{
	HRESULT res;

	// �w��t�@�C����ǂݍ��ݍς݂łȂ����H
	if (m_Textures.count(filename) == 0)
	{
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> resourceTexture;

		// �t���p�X�ɕ⊮
		std::wstring fullpath_bin = RESOURCE_DIRECTORY + filename + RESOURCE_EXT;
		// �e�N�X�`����ǂݍ��݁A�R���e�i�ɓo�^�i�L�[�̓t�@�C�����j
		res = CreateWICTextureFromFile(m_d3dDevice, fullpath_bin.c_str(), resource.GetAddressOf(),
			shaderResourceView.ReleaseAndGetAddressOf());

		// �ǂݍ��ݎ��s
		if (FAILED(res)) 	goto FAILED;

		// ���\�[�X���e�N�X�`���Ƃ��ĉ���
		res = resource.As(&resourceTexture);

		// �e�N�X�`���ł͂Ȃ�
		if (FAILED(res)) 	goto FAILED;

		// �e�N�X�`�����擾
		CD3D11_TEXTURE2D_DESC texDesc;
		resourceTexture->GetDesc(&texDesc);

		// �ǂݍ��񂾏����i�[
		std::unique_ptr<Texture> texture = std::make_unique<Texture>();
		texture->desc = texDesc;
		texture->shaderResourceView = shaderResourceView;

		m_Textures[filename] = std::move(texture);
	}

	return m_Textures[filename].get();

FAILED:
	return nullptr;
}