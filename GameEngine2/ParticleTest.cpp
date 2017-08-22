#include "ParticleTest.h"
#include "MyLibrary.h"
#include <WICTextureLoader.h>

using namespace MyLibrary;
using namespace DirectX;
using namespace DirectX::SimpleMath;

ParticleTest::ParticleTest(Camera* camera)
{
	ID3D11Device* device = DeviceResources::GetInstance()->GetD3DDevice();
	ID3D11DeviceContext* context = DeviceResources::GetInstance()->GetD3DDeviceContext();

	m_Camera = camera;

	ParticleEffectManager::getInstance()->Initialize();
	ParticleEffectManager::getInstance()->Load(0, L"Resources/Textures/effect2.png");
	ParticleEffectManager::getInstance()->SetCamera(m_Camera);
}

void ParticleTest::Update()
{
	static float angle = 0;
	angle += 0.05f;

	static int rest[2] = { 0, 2 };

	for (int j = 0; j < 2; j++)
	{
		if (--rest[j] < 0)
		{
			rest[j] = 8;
		}
		else
		{
			continue;
		}

		for (int i = 0; i < 20; i++)
		{
			float angle_add = XM_2PI / 20.0f * i;
		
			Vector3 position = Vector3(0.05f, 0, 0);
			if ( j == 0)
				position = Vector3::Transform(position, Matrix::CreateRotationY(angle + angle_add));
			else
				position = Vector3::Transform(position, Matrix::CreateRotationY(angle + angle_add));
			Vector3 velocity = position *0.5f;
			Vector3 accel = Vector3(0, 0, 0);
			Color s_color = Color(1, 0, 0, 1);
			Color e_color = Color(1, 0, 0, 1);
			if (j == 1)
			{
				s_color = Color(0, 1, 0, 1);
				e_color = Color(0, 1, 0, 1);
			}

			ParticleEffectManager::getInstance()->Entry(0, 120,
				position, velocity, accel, 0, 360, 2.0f, 0.0f,
				s_color, e_color);
		}
	}

	m_Camera->Update();

	ParticleEffectManager::getInstance()->Update();
}

void ParticleTest::Draw()
{
	ParticleEffectManager::getInstance()->Draw();
}
