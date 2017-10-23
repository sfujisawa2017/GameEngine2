#include "GameObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

GameObject::GameObject()
{
	float yaw = XM_2PI * rand() / RAND_MAX;
	float pitch = XM_2PI * rand() / RAND_MAX;
	float roll = XM_2PI * rand() / RAND_MAX;
	float rad = 100.0f * rand() / RAND_MAX - 50.0f;

	Vector3 pos = Vector3(0, 0, rad);
	Matrix RotM = Matrix::CreateRotationX(pitch) * Matrix::CreateRotationY(yaw);
	pos = Vector3::Transform(pos, RotM);
	radius = 0.1f;

	Quaternion q = Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
	obj.LoadModel(L"SphereNode");
	obj.SetRotQuat(q);
	obj.SetTrans(pos);
	obj.SetScale(1.0f);

	// 中心座標をセット
	center = pos;
}

void GameObject::Update()
{
	// ランダムに追加回転を加える
	float pitch = XM_2PI / 180.0f * rand() / RAND_MAX;
	float yaw = XM_2PI / 180.0f * rand() / RAND_MAX;

	Quaternion q = Quaternion::CreateFromYawPitchRoll(yaw, pitch, 0);

	q = q * obj.GetRotQuat();
	obj.SetRotQuat(q);

	// 回転後のZ軸の向きに動く
	Vector3 velocity = Vector3(0, 0, 0.1f);
	velocity = Vector3::Transform(velocity, q);

	Vector3 pos = obj.GetTrans() + velocity;

	// 位置、回転をセット
	obj.SetTrans(pos);
	obj.SetScale(radius);
	obj.Update();

	// 中心座標を更新
	center = pos;
}

void GameObject::Draw()
{
	obj.Draw();
}
