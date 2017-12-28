/// <summary>
/// 八分木判定対象オブジェクト
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

class OctreeObject
{
public:
	using Vector3 = DirectX::SimpleMath::Vector3;

	// コンストラクタ
	OctreeObject()
	{
		radius = 1.0f;
	}

	// デストラクタ
	virtual ~OctreeObject() {}

	// 中心点
	Vector3 center;
	// 半径
	float radius;
};