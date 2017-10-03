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

	m_ObjTest = std::make_unique<Obj3D>();
	m_ObjTest->LoadModel(L"SphereNode");
	m_ObjSkydome->AddChild(m_ObjTest.get());

	// パーティクルテスト
	m_ParticleTest = std::make_unique<ParticleTest>(m_Camera.get());

	m_Spr = m_SpriteFactory->CreateFromFile(L"cat");
	m_Spr->SetColor(DirectX::SimpleMath::Color(1, 0, 0, 1));
	m_Spr->SetAnchorPoint(DirectX::SimpleMath::Vector2(0, 0));
	m_Spr->SetTextureRect(RECT{ 50,50,100,100 });
	m_Spr->SetScale(DirectX::SimpleMath::Vector2(2, 2));

	m_Spr2 = m_SpriteFactory->CreateFromFile(L"cat");
	m_Spr2->SetColor(DirectX::SimpleMath::Color(0, 1, 0, 1));

	m_Spr2->SetPosition(DirectX::SimpleMath::Vector2(50, 50));
	m_Spr2->SetAnchorPoint(DirectX::SimpleMath::Vector2(0, 0));
	//m_Spr2->SetScale(0.5f);

	m_Spr->AddChild(m_Spr2.get());

	m_Spr3 = m_SpriteFactory->CreateFromFile(L"effect1");
	m_Spr3->SetPosition(DirectX::SimpleMath::Vector2(100, 100));
	m_Spr3->SetScale(0.5f);
	m_Spr2->AddChild(m_Spr3.get());

	ADX2Le::GetInstance()->Initialize(L"ADX2_samples.acf");
	ADX2Le::GetInstance()->ADX2Le::LoadAcb(L"Basic.acb", L"Basic.awb");
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

	m_ParticleTest->Update();

	if (MouseUtil::GetInstance()->IsTriggered(MyLibrary::MouseUtil::Button::Left))
	{
		ADX2Le::GetInstance()->Play(5);
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

	m_ParticleTest->Draw();

	m_Spr->SetPosition(DirectX::SimpleMath::Vector2(800, 450));
	static float rot = 0;
	rot += 1.1f;
	m_Spr->SetRotation(rot);
	//m_Spr->SetRotation(45);
	
	
	//m_Spr->SetAnchorPoint(DirectX::SimpleMath::Vector2(1, 1));

	m_Spr2->SetRotation(rot);

	m_Spr3->SetRotation(rot);

	m_Spr->Draw();

	//m_Spr2->Draw();
}
#pragma endregion
