#include "OctreeNode.h"
#include "OctreeObject.h"

/// <summary>
/// オブジェクトを挿入
/// </summary>
/// <param name="object"></param>
void OctreeNode::InsertObject(OctreeObject * object)
{
	// リストにオブジェクトを登録
	objList.push_back(object);
}

/// <summary>
/// オブジェクトを除外
/// </summary>
/// <param name="object"></param>
void OctreeNode::OnRemoveObject(OctreeObject * object)
{
	// リストからオブジェクトを排除
	objList.remove(object);
}
