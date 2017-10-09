/// <summary>
/// 炎エフェクトのエミッター
/// </summary>
#pragma once

#include "Emitter.h"

class FlameEmitter : public Emitter
{
public:
	static void LoadTexture(int texslot);
	static void DisposeTexture();

	FlameEmitter(DirectX::SimpleMath::Vector3 position);
	void Update() override;
protected:
	static int s_TextureSlot;	
	float m_param;
};