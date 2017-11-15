#include "GameObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

GameObject::GameObject()
{
	// メンバ変数初期化
	position = Vector3(0, 0, 0);
	velocity = Vector3(0, 0, 0);
	radius = 1.0f;

	obj.LoadModel(L"SphereNode", false);
	obj.SetTrans(position);
	obj.SetScale(radius);
	obj.Update();

	// 球の情報をセット
	sphere.center = position;
	sphere.radius = radius;
}

void GameObject::Update()
{
	// 速度による移動処理
	position = position + velocity;

	// オブジェクト更新
	UpdateObj3d();

	// 球の情報を更新
	sphere.center = position;
}

void GameObject::UpdateObj3d()
{
	obj.SetTrans(position);
	obj.SetScale(radius);
	obj.Update();
}

void GameObject::Draw()
{
	obj.Draw();
}