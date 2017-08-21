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
	ParticleTest(MyLibrary::Camera* camera);

	void Update();

	void Draw();
	
protected:
	MyLibrary::Camera* m_Camera;
};