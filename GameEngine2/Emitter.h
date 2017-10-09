/// <summary>
/// エミッター基底クラス
/// </summary>
#pragma once

#include "MyLibrary.h"

class Emitter
{
public:
	using Vector3 = DirectX::SimpleMath::Vector3;
	Emitter(Vector3 position);
	virtual void Update() = 0;
	void MoveCircular();
protected:
	// 座標
	Vector3 m_Position;
	// 速度
	Vector3 m_Velocity;
	// 円運動パラメータ
	float m_ParamCircular;
	// 寿命
	int m_LifeTime;
};