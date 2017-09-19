//
// Game.h
//

#pragma once

#include <DirectXMath.h>
#include <DirectXColors.h>

#include <PrimitiveBatch.h>
#include <VertexTypes.h>

#include "MyLibrary.h"

class Game : public MyLibrary::Framework
{
public:
	static const int PARTICLE_NUM_MAX;
	// 入力レイアウトの定義.
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// コンスタントバッファ
	struct Constants
	{
		DirectX::SimpleMath::Matrix WVP;	// ワールドビュープロジェクション合成行列
	};

	Game(HINSTANCE hInstance, int nCmdShow);

    void Initialize() override;

	void Finalize() override;

private:

    void Update(MyLibrary::StepTimer const& timer);
    void Render();

	// デバッグカメラ
	std::unique_ptr<MyLibrary::DebugCamera> m_Camera;
	// 天球
	std::unique_ptr<MyLibrary::Obj3D> m_ObjSkydome;

	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_PrimitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	// 頂点データ配列
	std::vector<DirectX::VertexPositionColorTexture> m_Vertices;
	// 汎用描画設定
	std::unique_ptr<DirectX::CommonStates> m_CommonStates;
};