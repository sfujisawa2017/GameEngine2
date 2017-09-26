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
	static const int POINT_NUM;

	// コンスタントバッファ
	struct Constants
	{
		DirectX::SimpleMath::Matrix VP;	// ビュープロジェクション合成行列
		DirectX::SimpleMath::Matrix Billboard; // ビルボード行列
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
};