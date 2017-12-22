#include "GameObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

const float GameObject::GRAVITY = 9.8f;
const float GameObject::WALL_RESTITUTION =  0.18f;
const float GameObject::SPEED_MAX = 1.0f;

MyLibrary::Plane GameObject::planes[PLANE_NUM];

const GameObject::PlaneDef GameObject::planeDefs[PLANE_NUM] = 
{
	{ Vector3( 0, 1, 0),  0 },
	{ Vector3(+1, 0, 0),-50 },
	{ Vector3(-1, 0, 0),-50 },
	{ Vector3( 0, 0,+1),-50 },
	{ Vector3( 0, 0,-1),-50 }
};

void GameObject::StaticInitialize()
{
	for (int i = 0; i < PLANE_NUM; i++)
	{
		planes[i].Normal = planeDefs[i].normal;
		planes[i].Distance = planeDefs[i].distance;
	}
}

GameObject::GameObject()
{
	float yaw = XM_2PI * rand() / RAND_MAX;
	float r = 10.0f * rand() / RAND_MAX;

	Vector3 pos = Vector3(cosf(yaw) * r, 0, sinf(yaw) * r);
	pos.y = 10.0f + 50.0f * rand() / RAND_MAX;

	// メンバ変数初期化
	position = pos;
	velocity = Vector3(-cosf(yaw) * r, 0, -sinf(yaw) * r);
	radius = 1.0f;
	weight = 1.0f;

	float red = (float)rand() / RAND_MAX;
	float green = (float)rand() / RAND_MAX;
	float blue = (float)rand() / RAND_MAX;

	std::shared_ptr<BasicEffect> basicEffect = std::make_shared<BasicEffect>(DeviceResources::GetInstance()->GetD3DDevice());
	basicEffect->EnableDefaultLighting();
	basicEffect->SetColorAndAlpha(Color(red, green, blue));
	std::shared_ptr<IEffect> effect = basicEffect;

	obj.LoadModel(L"SphereNode", false);
	obj.SetTrans(position);
	obj.SetScale(radius);
	obj.SetEffect(effect);
	obj.Update();

	// 球の情報をセット
	sphere.center = position;
	sphere.radius = radius;

	// 中心座標をセット
	OctreeObject::center = position;
	OctreeObject::radius = radius;
}

void GameObject::Update()
{
	// 重力処理 1フレームで9.8/60(m/s)加速
	velocity.y -= GRAVITY / 60;
	// 最大落下速度制限
	if (velocity.y < -SPEED_MAX)
	{
		velocity.y = -SPEED_MAX;
	}

	// 速度による移動処理
	position = position + velocity;

	// 地面との衝突判定と跳ね返り
	for (int i = 0; i < PLANE_NUM; i++)
	{
		ReflectPlane(planes[i]);
	}

	// オブジェクト更新
	UpdateObj3d();

	// 球の情報を更新
	sphere.center = position;

	// 中心座標を更新
	OctreeObject::center = position;
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

/// <summary>
/// 平面での跳ね返り
/// </summary>
/// <param name="plane"></param>
void GameObject::ReflectPlane(const MyLibrary::Plane& plane)
{
	Vector3 inter;

	// 球の中心とワールド原点の法線方向での距離
	float sphereDistance = position.Dot(plane.Normal);

	// 平面に接触していない
	if (sphereDistance - radius > plane.Distance) return;

	// めりこんだ量
	float overDistance = plane.Distance - (sphereDistance - radius);

	// 法線方向の速度（正）
	float normalSpeed = -velocity.Dot(plane.Normal);
	// めりこんだ時間
	float overTime = overDistance / normalSpeed;

	overTime = std::min<float>(overTime, 1.0f);

	// めりこんだ分巻き戻す
	position -= velocity * overTime;

	Vector3 vel_bef = velocity;
	// 跳ね返り速度
	velocity = GetRefrectVelocity(plane.Normal, WALL_RESTITUTION);

	// めりこんだ時間分、跳ね返り速度方向に進める
	position += velocity * overTime;

	// オブジェクト更新
	UpdateObj3d();

	// 球の情報を更新
	sphere.center = position;
	// 中心座標を更新
	OctreeObject::center = position;
}

/// <summary>
/// 2球の跳ね返り
/// </summary>
/// <param name="o1"></param>
/// <param name="o2"></param>
void GameObject::ReflectObjects(GameObject * o1, GameObject * o2)
{
	Vector3 inter;

	// 当たっていれば衝突点を算出　当たっていなければ終了
	if (!MyLibrary::CheckSphere2Sphere(o1->sphere, o2->sphere, &inter)) return;

	// 衝突時のオブジェクトの位置の差
	Vector3 cc = o2->position - o1->position;
	float distance = cc.Length();
	// めり込み量
	float overlap_length = o1->sphere.radius + o2->sphere.radius - distance;
	cc.Normalize();
	// めり込み排除
	o1->position -= cc * (overlap_length / 2.0f);
	o2->position += cc * (overlap_length / 2.0f);

	Vector3 va1 = o1->velocity.Dot(cc) * cc;
	Vector3 va2 = o2->velocity.Dot(cc) * cc;
	Vector3 vb1 = o1->velocity - va1;
	Vector3 vb2 = o2->velocity - va2;

	// 完全弾性衝突
	o1->velocity = va2 + vb1;
	o2->velocity = va1 + vb2;
}
