#include "GameObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

// 静的変数の実体
const float GameObject::GRAVITY = 9.8f;
const float GameObject::WALL_RESTITUTION = 0.18f;
const float GameObject::SPEED_MAX = 1.0f;

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

	const float range = 100;

	// 地面上のランダムな座標に配置
	position.x = RandomRange(-range, range);
	position.y = RandomRange(10.0f, 60.0f);
	position.z = RandomRange(-range, range);

	//velocity.x = 0.1f;

	// 色をランダムに決定
	float red = RandomRange(0, 1);
	float green = RandomRange(0, 1);
	float blue = RandomRange(0, 1);
	// ベーシックエフェクトを作成し、デフォルトのライティングをONに
	std::shared_ptr<BasicEffect> basicEffect = std::make_shared<BasicEffect>(DeviceResources::GetInstance()->GetD3DDevice());
	basicEffect->EnableDefaultLighting();
	basicEffect->SetColorAndAlpha(Color(red, green, blue));
	//basicEffect->SetTexture(TextureCache::GetInstance()->LoadTexture(L"cat")->shaderResourceView.Get());
	//basicEffect->SetTextureEnabled(true);
	std::shared_ptr<IEffect> effect = basicEffect;

	obj.LoadModel(L"SphereNode", false);
	obj.SetTrans(position);
	obj.SetScale(radius);
	// 作成したエフェクトをセット
	obj.SetEffect(effect);
	obj.Update();

	// 球の情報をセット
	sphere.center = position;
	sphere.radius = radius;

	onGround = false;
}

void GameObject::Update()
{
	// 重力処理 1フレームで9.8/60(m/s)加速
	velocity.y -= GRAVITY / 60;
	// 最大落下速度制限
	velocity.y = max(velocity.y, -SPEED_MAX);

	// 速度による移動処理
	position = position + velocity;

	MyLibrary::Plane plane;
	plane.Normal = Vector3(0, 1, 0);
	plane.Distance = 0;
	if (ReflectPlane(plane))
	{

	}

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
	if (!obj.GetCamera()->TestInFrustum(position, radius)) return;

	obj.Draw();
}

/// <summary>
/// 平面での跳ね返り
/// </summary>
/// <param name="plane"></param>
bool GameObject::ReflectPlane(const MyLibrary::Plane& plane)
{
	Vector3 inter;

	// 球の中心とワールド原点の法線方向での距離
	float sphereDistance = position.Dot(plane.Normal);

	// 平面に接触していない
	if (sphereDistance - radius > plane.Distance) return false;

	// めりこんだ量
	float overDistance = plane.Distance - (sphereDistance - radius);

	// 法線方向の速度（正）
	float normalSpeed = -velocity.Dot(plane.Normal);
	// めりこんだ時間
	float overTime = overDistance / normalSpeed;

	overTime = std::min<float>(overTime, 1.0f);

	// めりこんだ分巻き戻す
	position -= velocity * overTime;

	// 跳ね返り速度
	velocity = GetRefrectVelocity(plane.Normal, WALL_RESTITUTION);

	// めりこんだ時間分、跳ね返り速度方向に進める
	position += velocity * overTime;

	// オブジェクト更新
	UpdateObj3d();

	// 球の情報を更新
	sphere.center = position;

	return true;
}

Vector3 GameObject::GetRefrectVelocity(const Vector3 &normal, float e)
{
	// 壁の法線の逆ベクトル（壁に入っていく方向）
	// をコピーして、正規化
	Vector3 revNormal = -normal;
	revNormal.Normalize();

	// 壁に入っていく方向の速度を計算
	float dot = velocity.Dot(revNormal);
	// 壁に入っていく方向の速度
	Vector3 inVelocity = revNormal * dot;
	// 壁から出ていく方向の速度
	Vector3 outVelocity = -inVelocity * e;

	// 元の速度をコピー
	Vector3 refVelocity = velocity;

	// 壁に入っていく成分を打ち消す
	refVelocity -= inVelocity;
	// 壁から出ていく成分を足す
	refVelocity += outVelocity;

	return refVelocity;
	// 短く書くとこうなる（意味は分かりにくい）
	//return velocity - revNormal * velocity.Dot(revNormal) * (1 + e);
}