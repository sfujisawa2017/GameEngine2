//
// Game.h
//

#pragma once

#include <DirectXMath.h>
#include <DirectXColors.h>

#include "MyLibrary.h"
#include "ParticleTest.h"

class Game : public MyLibrary::Framework
{
public:
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
	std::unique_ptr<MyLibrary::Obj3D> m_ObjTest;
	// パーティクル表示テスト
	std::unique_ptr<ParticleTest> m_ParticleTest;
	
	std::unique_ptr<MyLibrary::Sprite> m_Spr;
	std::unique_ptr<MyLibrary::Sprite> m_Spr2;

	std::unique_ptr<MyLibrary::Sprite> m_Spr3;
};