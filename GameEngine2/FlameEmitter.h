/// <summary>
/// 炎エフェクトのエミッター
/// </summary>
#pragma once

#include "MyLibrary.h"

class FlameEmitter
{
public:
	static void LoadTexture(int texslot);
	static void DisposeTexture();

	FlameEmitter(DirectX::SimpleMath::Vector3 position);
	void Update();
protected:
	static int s_TextureSlot;
	DirectX::SimpleMath::Vector3 m_Position;
	float m_param;
};