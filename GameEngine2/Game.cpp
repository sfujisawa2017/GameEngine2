//
// Game.cpp
//

#include "Game.h"
#include "ParticleTest.h"

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;
using namespace MyLibrary;

Game* Game::s_instance = nullptr;

Game * Game::GetInstance()
{
	return s_instance;
}

Game::Game(HINSTANCE hInstance, int nCmdShow)
	: Framework(hInstance, nCmdShow)
{
	s_instance = this;
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
	m_Camera->SetCameraDistance(70.0f);

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

	ADX2Le::GetInstance()->Initialize(L"ADX2_samples.acf");
	ADX2Le::GetInstance()->ADX2Le::LoadAcb(L"Basic.acb", L"Basic.awb");

	GameObject::StaticInitialize();

	const float halfWidth = 30.0f;
	m_Octree = std::make_unique<Octree>(3, Vector3(-halfWidth, -halfWidth, -halfWidth), Vector3(halfWidth, halfWidth, halfWidth));

	for (int i = 0; i < 150; i++)
	{
		std::unique_ptr<GameObject> gameObj = std::make_unique<GameObject>();

		m_Octree->InsertObject(gameObj.get());

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

	for ( std::unique_ptr<GameObject>& obj : gameObjects)
	{
		obj->Update();
		// ツリーへの登録を更新
		obj->RemoveFromNode();
		m_Octree->InsertObject(obj.get());
	}

	PerformanceCounter* pc = PerformanceCounter::GetInstance();
	pc->Begin();
	m_Octree->TestAllCollisions();
	pc->End();
	double time = pc->GetElapsedTime();

	m_DebugText->AddText(DirectX::SimpleMath::Vector2(10, 10), L"time:%f", time);

	const std::vector<std::pair<OctreeObject*, OctreeObject*>>& collisionList = m_Octree->GetCollisionList();
	for (const std::pair<OctreeObject*, OctreeObject*>& collision : collisionList)
	{
		GameObject* obj1 = dynamic_cast<GameObject*>(collision.first);
		if (obj1 == nullptr) continue;

		GameObject* obj2 = dynamic_cast<GameObject*>(collision.second);
		if (obj2 == nullptr) continue;
		
		GameObject::ReflectObjects(obj1, obj2);
	}

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

	for (std::unique_ptr<GameObject>& obj : gameObjects)
	{
		obj->Draw();
	}
}
#pragma endregion
