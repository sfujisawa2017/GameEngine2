/// <summary>
/// 八分木
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <memory>
#include <list>
#include <vector>
#include "OctreeNode.h"
#include "OctreeObject.h"

class Octree
{
public:
	using Vector3 = DirectX::SimpleMath::Vector3;

	Octree(int depth, Vector3 minimum, Vector3 maximum);

	// オブジェクトを挿入
	void InsertObject(OctreeObject* object);
	// オブジェクトを除外
	void RemoveObject(OctreeObject * object);

	// 総当たりで衝突リストを生成
	void TestAllCollisions();
	void TestAllCollisionsRecursive(std::vector<OctreeNode*>& ancestorStack, OctreeNode * node);
	// 衝突テスト
	bool TestCollision(OctreeObject * objectA, OctreeObject * objectB);

	// 衝突リストを取得
	const std::vector<std::pair<OctreeObject*, OctreeObject*>>& GetCollisionList() { return m_CollisionList; }

	int GetHitCount() { return m_HitCount; }

protected:
	// 領域の最小座標
	Vector3 m_Min;
	// 領域の最大座標
	Vector3 m_Max;
	// 深さ
	int m_MaxDepth;
	// ルートノード
	std::unique_ptr<OctreeNode> m_RootNode;

	// 衝突リスト
	std::vector<std::pair<OctreeObject*, OctreeObject*>> m_CollisionList;

	// 1フレームでの衝突数
	int m_HitCount;
};