#include "GameObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

// 静的変数の実体
const float GameObject::GRAVITY = 9.8f;
const float GameObject::WALL_RESTITUTION = 0.8f;
const float GameObject::SPEED_MAX = 1.0f;
const float GameObject::ONGROUND_EPSILON = 0.5f;

const GameObject::PlaneDef GameObject::planeDefs[PLANE_NUM] =
{
	{ Vector3(0, 1, 0),  0 },
	{ Vector3(+1, 0, 0),-50 },
	{ Vector3(-1, 0, 0),-50 },
	{ Vector3(0, 0,+1),-50 },
	{ Vector3(0, 0,-1),-50 }
};

MyLibrary::Plane GameObject::planes[PLANE_NUM];

float RandomRange(float min_value, float max_value)
{
	return min_value + (max_value - min_value) * rand() / RAND_MAX;
}

void GameObject::StaticInitialize()
{
	for (int i = 0; i < PLANE_NUM; i++)
	{
		planes[i].Normal = planeDefs[i].normal;
		planes[i].Distance = planeDefs[i].distance;
	}
}

/// <summary>
/// 2球の跳ね返り
/// </summary>
/// <param name="o1"></param>
/// <param name="o2"></param>
void GameObject::ReflectObjects(GameObject * o1, GameObject * o2)
{
	// 当たっていれば衝突点を算出　当たっていなければ終了
	if (!MyLibrary::CheckSphere2Sphere(o1->sphere, o2->sphere)) return;

	// 衝突時のオブジェクトの位置の差
	Vector3 cc = o2->center - o1->center;
	float distance = cc.Length();
	// めり込み量
	float overlap_length = o1->sphere.radius + o2->sphere.radius - distance;
	cc.Normalize();
	// めり込み排除
	o1->center -= cc * (overlap_length / 2.0f);
	o2->center += cc * (overlap_length / 2.0f);

	Vector3 va1 = o1->velocity.Dot(cc) * cc;
	Vector3 va2 = o2->velocity.Dot(cc) * cc;
	Vector3 vb1 = o1->velocity - va1;
	Vector3 vb2 = o2->velocity - va2;

	// 完全弾性衝突
	o1->velocity = va2 + vb1;
	o2->velocity = va1 + vb2;
}

GameObject::GameObject()
{
	// メンバ変数初期化
	center = Vector3(0, 0, 0);
	velocity = Vector3(0, 0, 0);
	radius = 1.0f;

	const float range = 30;

	// 地面上のランダムな座標に配置
	center.x = RandomRange(-range, range);
	center.y = RandomRange(10.0f, 60.0f);
	center.z = RandomRange(-range, range);

	const float vel_range = 1.0f;
	velocity.x = RandomRange(-vel_range, vel_range);
	velocity.z = RandomRange(-vel_range, vel_range);

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
	obj.SetTrans(center);
	obj.SetScale(radius);
	// 作成したエフェクトをセット
	obj.SetEffect(effect);
	obj.Update();

	// 球の情報をセット
	sphere.center = center;
	sphere.radius = radius;

	onGround = false;
}

void GameObject::Update()
{
	if (onGround)
	{
		if (center.y > radius + 0.05f)
		{
			onGround = false;
		}
	}

	if (!onGround)
	{
		// 重力処理 1フレームで9.8/60(m/s)加速
		velocity.y -= GRAVITY / 60;
		// 最大落下速度制限
		velocity.y = max(velocity.y, -SPEED_MAX);
	}

	// 速度による移動処理
	center = center + velocity;

	// 地面との衝突判定と跳ね返り
	for (int i = 0; i < PLANE_NUM; i++)
	{
		if (ReflectPlane(planes[i]))
		{
			if (i == 0)
			{
				if (velocity.y < ONGROUND_EPSILON)
				{
					velocity.y = 0;
					center.y = radius;
					onGround = true;
				}
			}
		}
	}

	// オブジェクト更新
	UpdateObj3d();

	// 球の情報を更新
	sphere.center = center;
}

void GameObject::UpdateObj3d()
{
	obj.SetTrans(center);
	obj.SetScale(radius);
	obj.Update();
}

void GameObject::Draw()
{
	if (!obj.GetCamera()->TestInFrustum(center, radius)) return;

	obj.Draw();
}

/// <summary>
/// 平面での跳ね返り
/// </summary>
/// <param name="plane"></param>
bool GameObject::ReflectPlane(const MyLibrary::Plane& plane)
{
	// 球の中心とワールド原点の法線方向での距離
	float sphereDistance = center.Dot(plane.Normal);

	// 平面に接触していない
	if (sphereDistance - radius >= plane.Distance) return false;

	// めりこんだ量
	float overDistance = plane.Distance - (sphereDistance - radius);

	// 法線方向の速度（正）
	float normalSpeed = -velocity.Dot(plane.Normal);
	// めりこんだ時間
	float overTime = overDistance / normalSpeed;

	overTime = std::min<float>(overTime, 1.0f);

	// めりこんだ分巻き戻す
	center -= velocity * overTime;

	// 跳ね返り速度
	velocity = GetRefrectVelocity(plane.Normal, WALL_RESTITUTION);

	// めりこんだ時間分、跳ね返り速度方向に進める
	center += velocity * overTime;

	// オブジェクト更新
	UpdateObj3d();

	// 球の情報を更新
	sphere.center = center;

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