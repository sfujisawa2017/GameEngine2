﻿//
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