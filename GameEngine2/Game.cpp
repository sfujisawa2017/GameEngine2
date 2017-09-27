//
// Game.cpp
//

#include "Game.h"
#include <WICTextureLoader.h>

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;
using namespace MyLibrary;

// 指定範囲の乱数を返す関数
float RandomRange(float min_value, float max_value) {
	// 0～1
	float value = (float)rand() / RAND_MAX;
	value = min_value + (max_value - min_value) * value;
	return value;
};

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

	// パーティクルマネージャの初期化
	ParticleEffectManager* particleMan = ParticleEffectManager::getInstance();
	particleMan->Initialize();
	// カメラをセット
	particleMan->SetCamera(m_Camera.get());
	// 0番スロットにテクスチャをロード
	particleMan->Load(0, L"Resources/Textures/effect2.png");	
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

	//ParticleEffectManager::getInstance()->Entry(
	//	0,	// テクスチャ番号
	//	(int)RandomRange(60, 150),	// 生存時間
	//	Vector3(0, 0, 0), // 座標
	//	Vector3(0, 0.015f, 0), // 速度
	//	Vector3(0, 0, 0), // 加速度
	//	0.0f, 0.0f,	// 開始／終了角度
	//	1.0f, 0.0f,	// 開始／終了スケール
	//	Color(
	//		RandomRange(0.7f, 1.0f), 
	//		RandomRange(0.0f, 0.03f),
	//		0, 1),
	//	Color(
	//		RandomRange(0.7f, 1.0f),
	//		RandomRange(0.0f, 0.1f),
	//		0, 0)
	//);	// 開始／終了カラー

	float angle = RandomRange(-XM_PI, XM_PI);
	float x, y, z;
	x = 0.3f * cosf(angle);
	y = 0;
	z = 0.3f * sinf(angle);

	for (int i = 0; i < 5; i++)
	{
		float startAngle = RandomRange(0, 360);
		ParticleEffectManager::getInstance()->Entry(
			0,	// テクスチャ番号
			(int)RandomRange(60, 120),	// 生存時間
			Vector3(x, y, z), // 座標
			Vector3(0, 0.010f, 0), // 速度
			Vector3(0, 0, 0), // 加速度
			startAngle, startAngle + 180.0f,	// 開始／終了角度
			1.5f, 1.5f,	// 開始／終了スケール
			Color(
				RandomRange(0.1f, 0.3f),
				RandomRange(0.0f, 0.15f),
				0, 1),
			Color(
				RandomRange(0.1f, 0.15f),
				RandomRange(0.0f, 0.0f),
				0, 0)
		);	// 開始／終了カラー
	}

	// パーティクルマネージャの更新
	ParticleEffectManager::getInstance()->Update();
}
#pragma endregion

#pragma region Frame Render
/// <summary>
/// 毎フレーム描画
/// </summary>
void Game::Render()
{
	m_ObjSkydome->Draw();

	ParticleEffectManager::getInstance()->Draw();
}
#pragma endregion
