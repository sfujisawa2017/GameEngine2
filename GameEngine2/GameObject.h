/// <summary>
/// ゲームオブジェクト
/// </summary>
#pragma once

#include "MyLibrary.h"

class GameObject
{
public:
	using Vector3 = DirectX::SimpleMath::Vector3;

	GameObject();
	void Update();
	// Obj3dを更新
	void UpdateObj3d();
	void Draw();

protected:
	MyLibrary::Obj3D obj;
	MyLibrary::Sphere sphere;
	// 座標
	Vector3 position;
	// 速度
	Vector3 velocity;
	// 半径
	float radius;
};