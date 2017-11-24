/// <summary>
/// 8分木オブジェクト
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <memory>
#include <list>
#include <vector>
#include "MyLibrary.h"

class OctreeNode;

class OctreeObject
{
public:
	using Vector3 = DirectX::SimpleMath::Vector3;

	OctreeObject()
	{
		radius = 1.0f;
		node = nullptr;
	}

	virtual ~OctreeObject() {}

	// ノードから外す
	void RemoveFromNode();

	// 中心点
	Vector3 center;
	// 半径
	float radius;
	// 所属ノード
	OctreeNode* node;
};