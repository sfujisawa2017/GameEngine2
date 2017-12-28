/// <summary>
/// 八分木
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
//#include <memory>
#include <list>
#include <vector>
#include "OctreeObject.h"

class Octree
{
public:
	using Vector3 = DirectX::SimpleMath::Vector3;

	Octree();

	// オブジェクトを挿入
	void InsertObject(OctreeObject* object);

	// 総当たりで衝突リストを生成
	void TestAllCollisions();
	// 衝突テスト
	bool TestCollision(OctreeObject * objectA, OctreeObject * objectB);

	// 衝突リストを取得
	const std::vector<std::pair<OctreeObject*, OctreeObject*>>& GetCollisionList() { return m_CollisionList; }

	int GetHitCount() { return m_HitCount; }

protected:
	// オブジェクトのリスト
	std::list<OctreeObject*> m_ObjList;

	// 衝突リスト
	std::vector<std::pair<OctreeObject*, OctreeObject*>> m_CollisionList;

	// 1フレームでの衝突数
	int m_HitCount;
};