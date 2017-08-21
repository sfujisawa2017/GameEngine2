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
	ParticleTest();

	void Update();

	void Draw();
	
protected:
	std::unique_ptr<MyLibrary::DebugCamera> m_Camera;
};