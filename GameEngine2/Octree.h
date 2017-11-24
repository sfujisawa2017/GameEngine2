/// <summary>
/// 八分木
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <memory>
#include <list>
#include <vector>
#include "MyLibrary.h"

#include "OctreeObject.h"
#include "OctreeNode.h"

class Octree
{
public:
	using Vector3 = DirectX::SimpleMath::Vector3;

	// 分割数
	static const int DIVISION_NUM;
	// 最大深度
	static const int MAX_DEPTH;
	// 領域の最小値
	Vector3 m_Min;
	// 領域の最大値
	Vector3 m_Max;
	// 最小領域の辺の長さ
	Vector3 m_Unit;
	// 深さ
	int m_Depth;
	// 各レベルでのノード数
	std::vector<int> m_NodeCounts;
	// ノード配列
	std::vector<std::unique_ptr<OctreeNode>> m_Nodes;
	// 衝突スタック
	std::vector<OctreeNode*> m_CollisionStack;
	// 衝突リスト
	std::vector<std::pair<OctreeObject*, OctreeObject*>> m_CollisionList;
	// 1フレームでの衝突数
	int hitCount;
private:
	Octree();
public:
	// コンストラクタ
	Octree(int depth, Vector3 minimum, Vector3 maximum);
	DWORD BitSeparateFor3D(BYTE n);
	DWORD Get3DMortonNumber(BYTE x, BYTE y, BYTE z);
	DWORD GetPointElem(Vector3& p);
	DWORD GetMortonNumber(Vector3& min, Vector3& max);
	// オブジェクトを挿入
	void InsertObject(OctreeObject* object);
	// オブジェクト同士の当たり判定
	bool TestCollision(OctreeObject* objectA, OctreeObject* objectB);
	// 全オブジェクトを総当たりで判定する
	void TestAllCollisions();
	// 衝突リストを取得
	const std::vector<std::pair<OctreeObject*, OctreeObject*>>& GetCollisionList() { return m_CollisionList; }

protected:
	void TestCollisionsRecursive(DWORD Elem);
};
