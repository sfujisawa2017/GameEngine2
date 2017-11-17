//
// Game.cpp
//

#include "Game.h"
#include "ParticleTest.h"

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
	m_Camera->SetCameraDistance(20.0f);
	m_Camera->SetXAngle(XMConvertToRadians(30.0f));

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
	// 地面読み込み
	m_ObjGround = std::make_unique<Obj3D>();
	m_ObjGround->LoadModel(L"ground200m");
	m_ObjGround->DisableLighting();
	// サウンド読み込み
	ADX2Le::GetInstance()->Initialize(L"ADX2_samples.acf");
	ADX2Le::GetInstance()->ADX2Le::LoadAcb(L"Basic.acb", L"Basic.awb");

	TextureCache::GetInstance()->LoadTexture(L"cat");

	for (int i = 0; i < 10; i++)
	{
		std::unique_ptr<GameObject> gameObj = std::make_unique<GameObject>();

		gameObjects.push_back(std::move(gameObj));
	}
}

void Game::Finalize()
{
	ADX2Le::GetInstance()->Finalize();
}

#pragma region Frame Update
/// <summary>
/// 毎フレーム更新
/// </summary>
/// <param name="timer">時間情報</param>
void Game::Update(StepTimer const& timer)
{
	MouseUtil::GetInstance()->Update();
	ADX2Le::GetInstance()->Update();

	m_Camera->Update();

	for (std::unique_ptr<GameObject>& obj : gameObjects)
	{
		obj->Update();
	}

}
#pragma endregion

#pragma region Frame Render
/// <summary>
/// 毎フレーム描画
/// </summary>
void Game::Render()
{
	m_ObjSkydome->Draw();
	m_ObjGround->Draw();

	for (std::unique_ptr<GameObject>& obj : gameObjects)
	{
		obj->Draw();
	}
}
#pragma endregion
