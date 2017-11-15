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

class Octree
{
public:
	using Vector3 = DirectX::SimpleMath::Vector3;

	// 分割数
	static const int DIVISION_NUM = 8;
	// 最大深度
	static const int MAX_DEPTH = 2;

	class OctreeObject
	{
	public:
		virtual ~OctreeObject() {}
		Vector3 center;
		float radius;
	};

	/// <summary>
	/// 八分木ノード
	/// </summary>
	class OctreeNode
	{
	public:
		// 八分木を特定の深さまで拡張する
		static OctreeNode* BuildOctree(Vector3 center, float halfWidth, int stopDepth);
		// オブジェクトを挿入
		void InsertObject(OctreeObject* object);
		// オブジェクトの数を再帰的に数える
		int CountObject();
		// ノードの中心点座標
		Vector3 center;
		// ノードのボリュームの幅の半分
		float halfWidth;
		// ８つの子ノードに対するポインタ
		OctreeNode* children[DIVISION_NUM];
		// このノードに含まれているオブジェクトのリンクリスト
		std::list<OctreeObject*> objList;
	};

	// 八分木全体の中心点座標
	Vector3 center;
	// 各軸方向へのサイズ
	Vector3 size;
	// ルートノード
	OctreeNode* rootNode;
	// 親ノードスタック（探索用）
	std::vector<OctreeNode*> ancestorStack;
	// 衝突リスト
	std::vector<std::pair<OctreeObject*, OctreeObject*>> collisionList;
	int hitCount;
public:
	// コンストラクタ
	Octree();
	// オブジェクトを挿入
	void InsertObject(OctreeObject* object);
	// オブジェクト同士の当たり判定
	bool TestCollision(OctreeObject* objectA, OctreeObject* objectB);
	// 全オブジェクトを総当たりで判定する
	void TestAllCollisions();
	
	// ノードの数を数える
	void OutputNodeCounts();
	// 衝突リストを取得
	const std::vector<std::pair<OctreeObject*, OctreeObject*>>& GetCollisionList() { return collisionList; }

protected:
	void TestAllCollisions(OctreeNode* node, int depth);
};