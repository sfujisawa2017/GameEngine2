/// <summary>
/// ゲームオブジェクト
/// </summary>
#pragma once

#include "MyLibrary.h"

class GameObject
{
public:
	using Vector3 = DirectX::SimpleMath::Vector3;

	// 重力
	static const float GRAVITY;
	// 球と壁の反発係数
	static const float WALL_RESTITUTION;
	// 速さ最大値
	static const float SPEED_MAX;

	GameObject();
	// 更新
	void Update();
	// Obj3dを更新
	void UpdateObj3d();
	// 描画
	void Draw();
	// 平面との跳ね返り
	bool ReflectPlane(const MyLibrary::Plane & plane);
	// 法線と反発係数から、跳ね返り後の速度を求める
	Vector3 GetRefrectVelocity(const Vector3 & normal, float e);

protected:
	MyLibrary::Obj3D obj;
	MyLibrary::Sphere sphere;
	// 座標
	Vector3 position;
	// 速度
	Vector3 velocity;
	// 半径
	float radius;
	// 接地フラグ
	bool onGround;
};