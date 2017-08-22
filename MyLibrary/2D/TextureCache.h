/// <summary>
/// �e�N�X�`���̊i�[��
/// </summary>
#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <memory>
#include <map>
#include <string>
#include <d3d11_1.h>

namespace MyLibrary
{
	// �e�N�X�`��
	struct Texture
	{
		// �e�N�X�`�����
		CD3D11_TEXTURE2D_DESC desc;
		// DirectX11�V�F�[�_�[���\�[�X�r���[
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;
	};

	// �e�N�X�`���̊i�[��
	class TextureCache
	{
	public:
		static const std::wstring RESOURCE_DIRECTORY;
		static const std::wstring RESOURCE_EXT;

		static TextureCache* GetInstance();
	private:
		static std::unique_ptr<TextureCache> m_Instance;
	public:
		TextureCache();
		// ������
		void Initialize(ID3D11Device* d3dDevice);
		// �e�N�X�`���ǂݍ���
		Texture* LoadTexture(const wchar_t* filename);
	private:
		// DirectX11�f�o�C�X
		ID3D11Device* m_d3dDevice;

		// �e�N�X�`���z��
		std::map<std::wstring, std::unique_ptr<Texture>> m_Textures;
	};
}