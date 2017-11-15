#include "Octree.h"
#include <string>
#include <queue>
#include "Game.h"

using namespace DirectX;
using namespace MyLibrary;

/// <summary>
/// 八分木を特定の深さまで拡張する
/// </summary>
/// <param name="center">部分木の中心点</param>
/// <param name="halfWidth">領域幅の半分</param>
/// <param name="stopDepth">残り階層</param>
/// <returns></returns>
Octree::OctreeNode * Octree::OctreeNode::BuildOctree(Vector3 center, float halfWidth, int stopDepth)
{
	// 特定の深さに達したら拡張を止める
	if (stopDepth < 0)
	{
		return nullptr;
	}
	else
	{
		// この部分木のルートを構築して埋める
		OctreeNode* node = new OctreeNode;
		node->center = center;
		node->halfWidth = halfWidth;

		// 部分木の8つの子を再帰的に構築
		Vector3 offset;
		float step = halfWidth * 0.5f;
		for (int i = 0; i < DIVISION_NUM; i++)
		{
			// 交互にマイナスとプラスに割り振る
			offset.x = ((i & 1) ? step : -step);
			// ２つごとにマイナスとプラスに割り振る
			offset.y = ((i & 2) ? step : -step);
			// ４つごとにマイナスとプラスに割り振る
			offset.z = ((i & 4) ? step : -step);
			// 再帰呼び出し
			node->children[i] = BuildOctree(center + offset, step, stopDepth - 1);
		}
		return node;
	}
}

/// <summary>
/// オブジェクトを挿入
/// </summary>
/// <param name="object">挿入するオブジェクト</param>
void Octree::OctreeNode::InsertObject(OctreeObject * object)
{
	int index = 0;
	bool straddle = false;

	// XYZで3要素
	const int ELEMENT_NUM = 3;

	// Vector3のxyzを、floatの配列として解釈
	// w値に絶対アクセスしないように注意。
	XMVECTORF32* treeCenter = (XMVECTORF32*)&center;
	XMVECTORF32* objectCenter = (XMVECTORF32*)&object->center;

	// 木の中心部との位置関係から８つの子のどれに挿入するか[0..7]を決定
	for (int i = 0; i < ELEMENT_NUM; i++)
	{
		float delta = objectCenter->f[i] - treeCenter->f[i];

		// プラスとマイナスの領域をまたぐかどうか
		if (fabsf(delta) < object->radius)
		{
			straddle = true;
			break;
		}
		// 2進数でみて、X:第一ビット Y:第二ビット Z:第三ビットを決定する。
		// 最終的には2進数3桁分でインデックスを決定
		if (delta > 0.0f)
		{
			index |= (1 << i);
		}
	}
	//straddle = true;	// これを有効化すると、全てルートノードに追加される
	// 領域をまたいでいない場合は、子ノードでさらに再帰判定する
	if (!straddle && children[index])
	{
		children[index]->InsertObject(object);
	}
	// 領域をまたいでいるか、限界の深さに達したら、ツリーにオブジェクトを繋ぐ
	else
	{
		objList.push_front(object);
	}
}

/// <summary>
/// オブジェクトの数を再帰的に数える
/// </summary>
/// <returns>オブジェクトの数</returns>
int Octree::OctreeNode::CountObject()
{
	int count = objList.size();

	for (int i = 0; i < DIVISION_NUM; i++)
	{
		if (children[i])
		{
			count += children[i]->CountObject();
		}
	}

	return count;
}

/// <summary>
/// コンストラクタ
/// </summary>
Octree::Octree()
{
	center = Vector3(0, 0, 0);
	size = Vector3(1000, 1000, 1000);
	rootNode = OctreeNode::BuildOctree(center, size.x / 2.0f, MAX_DEPTH);

	ancestorStack.reserve(MAX_DEPTH);
}

/// <summary>
/// オブジェクトを挿入
/// </summary>
/// <param name="object">挿入するオブジェクト</param>
void Octree::InsertObject(OctreeObject * object)
{
	rootNode->InsertObject(object);
}

bool Octree::TestCollision(OctreeObject * objectA, OctreeObject * objectB)
{
	float distanceSQ = Vector3::DistanceSquared(objectA->center, objectB->center);

	float radiusSQ = objectA->radius + objectB->radius;
	radiusSQ = radiusSQ * radiusSQ;

	if (distanceSQ <= radiusSQ)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Octree::TestAllCollisions()
{	
	collisionList.clear();
	ancestorStack.clear();
	hitCount = 0;
	TestAllCollisions(rootNode, 0);

	Game::GetInstance()->GetDebugText()->AddText(DirectX::SimpleMath::Vector2(10,30), L"Hit:%d", hitCount);
}

void Octree::TestAllCollisions(OctreeNode * node, int depth)
{	
	ancestorStack.push_back(node);

	for (int n = 0; n < depth+1; n++)
	{
		for (OctreeObject* A : ancestorStack[n]->objList)
		{
			for (OctreeObject* B : node->objList)
			{
				if (A == B) break;

				if (TestCollision(A, B))
				{
					collisionList.push_back(std::make_pair(A, B));
					hitCount++;
				}
			}
		}
	}

	for (int i = 0; i < DIVISION_NUM; i++)
	{
		if (node->children[i])
		{
			TestAllCollisions(node->children[i], depth+1);
		}
	}

	ancestorStack.pop_back();
}

/// <summary>
/// ノードの数を数える
/// </summary>
void Octree::OutputNodeCounts()
{
	int count = rootNode->CountObject();

	std::string str = std::to_string(count);
	OutputDebugStringA(str.c_str());
}
