#include "Emitter.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;
using namespace MyLibrary;

Emitter::Emitter(Vector3 position)
{
	m_Position = position;
	m_ParamCircular = 0;
	// 無限
	m_LifeTime = -1;
}

void Emitter::Update()
{
	if (m_LifeTime > 0)
	{
		m_LifeTime--;

	}
}

void Emitter::MoveCircular()
{
	m_ParamCircular += 0.1f;
	m_Position.x = cosf(m_ParamCircular);
	m_Position.y = sinf(m_ParamCircular);
}
