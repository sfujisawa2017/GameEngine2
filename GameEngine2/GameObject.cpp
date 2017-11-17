#include "GameObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

float RandomRange(float min_value, float max_value)
{
	return min_value + (max_value - min_value) * rand() / RAND_MAX;
}

GameObject::GameObject()
{
	// メンバ変数初期化
	position = Vector3(0, 0, 0);
	velocity = Vector3(0, 0, 0);
	radius = 1.0f;

	// 地面上のランダムな座標に配置
	position.x = RandomRange(-10, 10);
	position.y = 1.0f;
	position.z = RandomRange(-10, 10);

	velocity.x = 0.1f;

	// 色をランダムに決定
	float red = RandomRange(0, 1);
	float green = RandomRange(0, 1);
	float blue = RandomRange(0, 1);
	// ベーシックエフェクトを作成し、デフォルトのライティングをONに
	std::shared_ptr<BasicEffect> basicEffect = std::make_shared<BasicEffect>(DeviceResources::GetInstance()->GetD3DDevice());
	basicEffect->EnableDefaultLighting();
	basicEffect->SetColorAndAlpha(Color(red, green, blue));
	basicEffect->SetTexture(TextureCache::GetInstance()->LoadTexture(L"cat")->shaderResourceView.Get());
	basicEffect->SetTextureEnabled(true);
	std::shared_ptr<IEffect> effect = basicEffect;

	obj.LoadModel(L"Cube", false);
	obj.SetTrans(position);
	obj.SetScale(radius);
	// 作成したエフェクトをセット
	obj.SetEffect(effect);
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