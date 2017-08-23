//
// Game.cpp
//

#include "Game.h"

extern void ExitGame();

using namespace DirectX;
using Microsoft::WRL::ComPtr;
using namespace MyLibrary;

Game::Game(HINSTANCE hInstance, int nCmdShow)
	: Framework(hInstance, nCmdShow)
{
}

void Game::Initialize()
{
	DeviceResources* deviceResources = DeviceResources::GetInstance();

	// マウスライブラリにウィンドウハンドルを渡す
	MouseUtil::GetInstance()->SetWindow(DeviceResources::GetInstance()->GetWindow());
	
	// ウィンドウ矩形取得
	RECT windowRect = deviceResources->GetOutputSize();
	// デバッグカメラ作成
	m_Camera = std::make_unique<DebugCamera>(windowRect.right- windowRect.left, windowRect.bottom - windowRect.top);

	{ // Obj3D初期化
		// 設定
		Obj3D::CommonDef def;
		def.pCamera = m_Camera.get();
		def.pDevice = deviceResources->GetD3DDevice();
		def.pDeviceContext = deviceResources->GetD3DDeviceContext();
		// 設定を元に初期化
		Obj3D::InitializeCommon(def);
	}

	// 天球読み込み
	m_ObjSkydome = std::make_unique<Obj3D>();
	m_ObjSkydome->LoadModel(L"skydome");
	m_ObjSkydome->DisableLighting();
}

void Game::Finalize()
{

}

#pragma region Frame Update
/// <summary>
/// 毎フレーム更新
/// </summary>
/// <param name="timer">時間情報</param>
void Game::Update(StepTimer const& timer)
{
	MouseUtil::GetInstance()->Update();

	m_Camera->Update();
}
#pragma endregion

#pragma region Frame Render
/// <summary>
/// 毎フレーム描画
/// </summary>
void Game::Render()
{
	m_ObjSkydome->Draw();

}
#pragma endregion
