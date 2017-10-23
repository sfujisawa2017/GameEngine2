/// <summary>
/// 何かのゲームオブジェクト
/// </summary>
#pragma once

#include "MyLibrary.h"
#include "Octree.h"

class GameObject : public Octree::OctreeObject
{
public:
	GameObject();
	void Update();
	void Draw();

protected:
	MyLibrary::Obj3D obj;
};