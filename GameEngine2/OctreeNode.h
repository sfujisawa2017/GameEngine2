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

	// 分割数
	static const int DIVISION_NUM = 8;

	// XYZで3要素
	static const int ELEMENT_NUM = 3;

	struct float3
	{
		float f[ELEMENT_NUM];
	};

	// コンストラクタ
	OctreeNode(int depth, float3 center, float3 m_HalfSize);

	// オブジェクトを挿入
	void InsertObject(OctreeObject* object, int maxDepth);
	// オブジェクトを除外
	void RemoveObject(OctreeObject* object);
private:
	// 深さ
	int m_Depth;
	// 領域の中心座標
	float3 m_Center;
	// 領域の半径
	float3 m_HalfSize;

	// ８つの子ノードに対するポインタ
	std::unique_ptr<OctreeNode> m_Children[DIVISION_NUM];
	// このノードに含まれているオブジェクトのリンクリスト
	std::list<OctreeObject*> m_ObjList;
};