/// <summary>
/// 八分木ノード
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <memory>
#include <list>
#include <vector>
#include "MyLibrary.h"

class OctreeObject;

class OctreeNode
{	
public:
	using Vector3 = DirectX::SimpleMath::Vector3;

	// オブジェクトを挿入
	void InsertObject(OctreeObject* object);
	// オブジェクトを除外
	void OnRemoveObject(OctreeObject* object);
	// オブジェクトリストを取得
	std::list<OctreeObject*>& GetObjList() { return objList; }
private:
	// このノードに含まれているオブジェクトのリンクリスト
	std::list<OctreeObject*> objList;
};