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

	// 入力レイアウトの定義.
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// コンスタントバッファ
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
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_GeometryShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	// ワールドビュープロジェクション行列用コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pWVPConstantBuffer;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_PrimitiveBatch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

	std::vector<DirectX::VertexPositionColorTexture> m_Vertices;
	DirectX::SimpleMath::Matrix m_WVP;
	//テクスチャーのサンプラー
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_Sampler;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;

	std::unique_ptr<MyLibrary::DebugCamera> m_Camera;

	std::unique_ptr<DirectX::CommonStates> m_CommonStates;
};