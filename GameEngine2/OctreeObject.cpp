#include "OctreeObject.h"
#include "OctreeNode.h"

using namespace DirectX;
using namespace MyLibrary;

/// <summary>
/// ノードから外す
/// </summary>
void OctreeObject::RemoveFromNode()
{
	// 未登録
	if (!node) return;

	// 親の登録を解除
	node->OnRemoveObject(this);
	node = nullptr;
}
