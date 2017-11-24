#include "Octree.h"
#include "OctreeObject.h"
#include <string>
#include <queue>
#include "Game.h"

using namespace DirectX;
using namespace MyLibrary;

// 分割数
const int Octree::DIVISION_NUM = 8;
// 最大深度
const int Octree::MAX_DEPTH = 8;

/// <summary>
/// コンストラクタ
/// </summary>
Octree::Octree(int depth, Vector3 minimum, Vector3 maximum)
{
	assert(depth <= MAX_DEPTH);

	m_Min = minimum;
	m_Max = maximum;
	m_Depth = depth;
	Vector3 size = m_Max - m_Min;
	int maxNodeCount = 1 << depth;
	m_Unit = size / ((float)maxNodeCount);

	// 各レベルのノード数を計算（深さ+1まで）
	m_NodeCounts.resize(depth+1);
	m_NodeCounts[0] = 1;
	for (int i = 1; i < depth+1; i++)
	{
		// ８のべき乗
		m_NodeCounts[i] = m_NodeCounts[i - 1] * DIVISION_NUM;
	}

	// 全レベルでのノード数の合計を計算
	int nodeNum = 0;
	for (int i = 0; i < depth; i++)
	{
		nodeNum += m_NodeCounts[i];
	}
	
	// 全ノードを生成
	m_Nodes.resize(nodeNum);
	for (auto& node : m_Nodes)
	{
		node = std::make_unique<OctreeNode>();
	}

	// 衝突リスト用のメモリを適当に確保しておく
	m_CollisionList.reserve(100);
	// 衝突スタック用のメモリを確保しておく
	m_CollisionStack.reserve(MAX_DEPTH);
}

/// <summary>
/// ビット分割関数
/// 8ビットの入力を3ビット刻みのビット列に変換する
/// 原理上、出力の下位22ビット分にデータが収納される
/// </summary>
/// <param name="n"></param>
/// <returns></returns>
DWORD Octree::BitSeparateFor3D(BYTE n)
{
	// s = ------------------------76543210 : 入力値
	// s = ----------------7654321076543210 : s | s << 8
	// m = ----------------||||--------||||	: 0x0000f00f
	// s = ----------------7654--------3210 : ビットAND
	// s = ------------76547654----32103210 : s | s << 4
	// m = ------------||----||----||----|| : 0x000c30c3
	// s = ------------76----54----32----10	: ビットAND
	// s = ----------7676--5454--3232--1010 : s | s << 2
	// m = ----------|--|--|--|--|--|--|--| : 0x00249249
	// s = ----------7--6--5--4--3--2--1--0 : ビットAND（出力値）
	DWORD s = n;
	s = (s | s << 8) & 0x0000f00f;
	s = (s | s << 4) & 0x000c30c3;
	s = (s | s << 2) & 0x00249249;
	return s;
}

/// <summary>
/// 3Dモートン空間番号算出関数
/// 3つの8ビットの数字を入力として、それらをビットとして挟み込んで1つの数字にする
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <returns></returns>
DWORD Octree::Get3DMortonNumber(BYTE x, BYTE y, BYTE z)
{
	// ----------x--x--x--x--x--x--x--x : BitSeparateFor3D(x)
	// ---------y--y--y--y--y--y--y--y- : BitSeparateFor3D(y) << 1
	// --------z--z--z--z--z--z--z--z-- : BitSeparateFor3D(z) << 2
	return BitSeparateFor3D(x) | BitSeparateFor3D(y) << 1 | BitSeparateFor3D(z) << 2;
}

/// <summary>
/// 座標→線形8分木要素番号変換関数
/// x,y,z座標をそれぞれツリー内のノード番号に変換し、そこからモートン空間番号を得る
/// </summary>
/// <param name="p"></param>
/// <returns></returns>
DWORD Octree::GetPointElem(Vector3& p)
{
	return Get3DMortonNumber(
		(BYTE)((p.x - m_Min.x) / m_Unit.x),
		(BYTE)((p.y - m_Min.y) / m_Unit.y),
		(BYTE)((p.z - m_Min.z) / m_Unit.z)
	);
}

// 座標から空間番号を算出
DWORD Octree::GetMortonNumber(Vector3& minimun, Vector3& maximum)
{
	if (m_Nodes.size() <= 0)
		return 0xffffffff;

	// 最小レベルにおける各軸位置を算出
	DWORD LT = GetPointElem(minimun);
	DWORD RB = GetPointElem(maximum);

	// 空間番号を引き算して
	// 最上位区切りから所属レベルを算出
	DWORD Def = RB ^ LT;
	int HiLevel = 1;
	for (int i = 0; i<m_Depth; i++)
	{
		DWORD Check = (Def >> (i * 3)) & 0x7;
		if (Check != 0)
			HiLevel = i + 1;
	}
	DWORD SpaceNum = RB >> (HiLevel * 3);
	DWORD AddNum = (m_NodeCounts[m_Depth - HiLevel] - 1) / 7;
	SpaceNum += AddNum;

	if (SpaceNum > m_Nodes.size())
		return 0xffffffff;

	return SpaceNum;
}

/// <summary>
/// オブジェクトを挿入
/// </summary>
/// <param name="object">挿入するオブジェクト</param>
void Octree::InsertObject(OctreeObject * object)
{
	Vector3 minimum(object->center.x - object->radius, object->center.y - object->radius, object->center.z - object->radius);
	Vector3 maximum(object->center.x + object->radius, object->center.y + object->radius, object->center.z + object->radius);
	// オブジェクトの境界範囲から登録モートン番号を算出
	DWORD elem = GetMortonNumber(minimum, maximum);
	if (elem >= m_Nodes.size())
		return;

	// ノードにオブジェクトを挿入
	m_Nodes[elem]->InsertObject(object);
	object->node = m_Nodes[elem].get();
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
	m_CollisionList.clear();
	hitCount = 0;

	// ルート空間の存在をチェック
	if (m_Nodes.size()==0)
		return;	// 空間が存在していない

	m_CollisionStack.clear();
	// 再帰で判定を取る
	TestCollisionsRecursive(0);

	Game::GetInstance()->GetDebugText()->AddText(DirectX::SimpleMath::Vector2(10,30), L"Hit:%d", hitCount);
}

// 空間内で衝突リストを作成する
void Octree::TestCollisionsRecursive(DWORD Elem)
{
	assert(0 <= Elem && Elem < m_Nodes.size());

	OctreeNode* node = m_Nodes[Elem].get();

	// ① 空間内のオブジェクト同士の衝突リスト作成
	for (OctreeObject* A : node->GetObjList())
	{
		for (OctreeObject* B : node->GetObjList())
		{
			if (A == B) break;

			if (TestCollision(A, B))
			{
				m_CollisionList.push_back(std::make_pair(A, B));
				hitCount++;
			}			
		}
	}

	// ② 衝突スタックとの衝突リスト作成
	for (OctreeObject* A : node->GetObjList())
	{
		for (OctreeNode* stackNode : m_CollisionStack)
		{
			for (OctreeObject* B : stackNode->GetObjList())
			{
				if (TestCollision(A, B))
				{
					m_CollisionList.push_back(std::make_pair(A, B));
					hitCount++;
				}
			}
		}
	}

	bool childExists = false;
	// ③ 子空間に移動
	DWORD ObjNum = 0;
	DWORD NextElem;
	for (int i = 0; i < 8; i++) {
		NextElem = Elem * 8 + 1 + i;
		if (NextElem < m_Nodes.size()) {
			if (!childExists) {
				// ④ ノードをスタックに追加
				m_CollisionStack.push_back(node);
			}
			childExists = true;
			TestCollisionsRecursive(Elem * 8 + 1 + i);	// 子空間へ
		}
	}

	// ⑤ スタックからオブジェクトを外す
	if (childExists) {
		m_CollisionStack.pop_back();
	}
}

