/// <summary>
/// 八分木ノード
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <memory>
#include <list>

#include "OctreeObject.h"

class Octree;

class OctreeNode
{
public:
	using Vector3 = DirectX::SimpleMath::Vector3;

	friend Octree;

	// オブジェクトを挿入
	void InsertObject(OctreeObject* object);
	// オブジェクトを除外
	void RemoveObject(OctreeObject* object);
private:
	// このノードに含まれているオブジェクトのリンクリスト
	std::list<OctreeObject*> m_ObjList;
};