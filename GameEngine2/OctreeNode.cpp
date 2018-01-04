#include "OctreeNode.h"

void OctreeNode::InsertObject(OctreeObject * object)
{
	// リストにオブジェクトを登録
	m_ObjList.push_back(object);
}

void OctreeNode::RemoveObject(OctreeObject * object)
{
	// リストからオブジェクトを排除
	m_ObjList.remove(object);
}
