#include "Octree.h"

/// <summary>
/// 
/// </summary>
/// <param name="depth">深度</param>
/// <param name="minimum">領域の最小座標</param>
/// <param name="maximum">領域の最大座標</param>
Octree::Octree(int depth, Vector3 minimum, Vector3 maximum)
{
	m_Min = minimum;
	m_Max = maximum;
	m_MaxDepth = depth;
	// 領域サイズ
	Vector3 size = m_Max - m_Min;
	// ルートノードを生成する
	Vector3 center = (m_Min + m_Max) / 2.0f;
	OctreeNode::float3 centerF3;
	OctreeNode::float3 halfSizeF3;

	centerF3.f[0] = center.x;
	centerF3.f[1] = center.y;
	centerF3.f[2] = center.z;

	halfSizeF3.f[0] = size.x / 2.0f;
	halfSizeF3.f[1] = size.y / 2.0f;
	halfSizeF3.f[2] = size.z / 2.0f;

	m_RootNode = std::make_unique<OctreeNode>(0, centerF3, halfSizeF3);

	m_HitCount = 0;

	// 衝突リスト用のメモリを確保
	m_CollisionList.reserve(1000);
}

void Octree::InsertObject(OctreeObject * object)
{
	// ルートノードに挿入
	m_RootNode->InsertObject(object, m_MaxDepth);
}

void Octree::RemoveObject(OctreeObject * object)
{
	// ルートノードに挿入
	//m_RootNode->InsertObject(object, m_MaxDepth);
}

void Octree::TestAllCollisions()
{
	m_CollisionList.clear();
	m_HitCount = 0;

	// 親ノードスタック（探索用）
	std::vector<OctreeNode*> ancestorStack;

	TestAllCollisionsRecursive(ancestorStack, m_RootNode.get());
}

void Octree::TestAllCollisionsRecursive(std::vector<OctreeNode*>& ancestorStack, OctreeNode* node)
{
	ancestorStack.push_back(node);

	int count = ancestorStack.size();

	for (int i = 0; i < count; i++)
	{
		for (OctreeObject* A : ancestorStack[i]->m_ObjList)
		{
			for (OctreeObject* B : node->m_ObjList)
			{
				// A→BおよびB→A両方の判定を回避する
				if (A == B) break;

				// 当たっていたら
				if (TestCollision(A, B))
				{
					// 衝突リストに挿入
					m_CollisionList.push_back(std::make_pair(A, B));
					m_HitCount++;
				}
			}
		}
	}

	// 全ての子ノードに対して再帰的に判定を行う
	for (int i = 0; i < OctreeNode::DIVISION_NUM; i++)
	{
		if (node->m_Children[i])
		{
			TestAllCollisionsRecursive(ancestorStack, node->m_Children[i].get());
		}
	}

	ancestorStack.pop_back();
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
