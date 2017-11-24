/// <summary>
/// 何かのゲームオブジェクト
/// </summary>
#pragma once

#include "MyLibrary.h"
#include "OctreeObject.h"

//class Wall
//{
//public:
//	using Vector3 = DirectX::SimpleMath::Vector3;
//	// 法線
//	Vector3 normal;
//	// 壁上の一点の座標
//	Vector3 position;
//	// 反発係数
//	float restitution;
//};

class GameObject : public OctreeObject
{
public:
	using Vector3 = DirectX::SimpleMath::Vector3;
	// 重力
	static const float GRAVITY;
	// 球と壁の反発係数
	static const float WALL_RESTITUTION;
	// 速さ最大値
	static const float SPEED_MAX;
	static const int PLANE_NUM = 5;

	struct PlaneDef
	{
		Vector3 normal;
		float distance;
	};

	static const PlaneDef planeDefs[PLANE_NUM];
	static MyLibrary::Plane planes[PLANE_NUM];

	static void StaticInitialize();

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
	// 質量
	float weight;
	//// スケール
	//float scale;

public:
	/// <summary>
	/// 壁と球の反射ベクトルを計算
	/// </summary>
	/// <param name="normal">壁の法線</param>
	/// <param name="velocity">衝突前の速度</param>
	/// <param name="e">壁の反発係数</param>
	/// <returns></returns>
	Vector3 GetRefrectVelocity(const Vector3 &normal, float e);
	void ReflectPlane(const MyLibrary::Plane &plane);
	static void ReflectObjects(GameObject* o1, GameObject* o2);
};