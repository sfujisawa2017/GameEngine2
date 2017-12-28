#include "OctreeNode.h"

OctreeNode::OctreeNode(int depth, float3 center, float3 m_HalfSize)
{
	m_Depth = depth;
	m_Center = center;
	m_HalfSize = m_HalfSize;
}

void OctreeNode::InsertObject(OctreeObject * object, int maxDepth)
{
	int index = 0;
	bool straddle = false;	

	// Vector3のxyzを、floatの配列として解釈
	float3* objectCenter = (float3*)&object->center;

	// 木の中心部との位置関係から８つの子のどれに挿入するか[0..7]を決定
	for (int i = 0; i < ELEMENT_NUM; i++)
	{
		float delta = objectCenter->f[i] - m_Center.f[i];

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
	// 領域をまたいでいない場合は、子ノードでさらに再帰判定する
	if (!straddle && m_Depth+1 < maxDepth)
	{
		// 子ノードがなければ生成する
		if (!m_Children[index])
		{
			float3 center;
			float3 halfSize;

			for (int i = 0; i < ELEMENT_NUM; i++)
			{
				halfSize.f[i] = m_HalfSize.f[i] / 2.0f;
			}

			for (int i = 0; i < ELEMENT_NUM; i++)
			{
				// 交互にマイナスとプラスに割り振る
				float offset = ((i & 1 << i) ? halfSize.f[i] : -halfSize.f[i]);
				center.f[i] = m_Center.f[i] + offset;
			}

			m_Children[index] = std::make_unique<OctreeNode>(m_Depth + 1, center, halfSize);
		}

		m_Children[index]->InsertObject(object, m_Depth+1);
	}
	// 領域をまたいでいるか、限界の深さに達したら、ツリーにオブジェクトを繋ぐ
	else
	{
		m_ObjList.push_front(object);
	}
}

void OctreeNode::RemoveObject(OctreeObject * object)
{
	std::list<OctreeObject*>::iterator it = find(m_ObjList.begin(), m_ObjList.end(), object);

	// リストに含まれる？
	if (it != m_ObjList.end())
	{
		// 除外して終了
		m_ObjList.erase(it);
		return;
	}

	// 子ノードに対して再帰的に繰り返す
	for (int i = 0; i < DIVISION_NUM; i++)
	{
		if (m_Children[i] == nullptr) continue;

		m_Children[i]->RemoveObject(object);
	}
}
