#include "Octree.h"

Octree::Octree()
{
	m_HitCount = 0;

	m_CollisionList.reserve(1000);
}

void Octree::InsertObject(OctreeObject * object)
{
	m_ObjList.push_back(object);	
}

void Octree::TestAllCollisions()
{
	m_CollisionList.clear();
	m_HitCount = 0;

	// オブジェクト同士の衝突リスト作成
	for (OctreeObject* A : m_ObjList)
	{
		for (OctreeObject* B : m_ObjList)
		{
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
