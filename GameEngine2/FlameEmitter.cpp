#include "FlameEmitter.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;
using namespace MyLibrary;

int FlameEmitter::s_TextureSlot = 0;

// 指定範囲の乱数を返す関数
static float RandomRange(float min_value, float max_value) {
	// 0～1
	float value = (float)rand() / RAND_MAX;
	value = min_value + (max_value - min_value) * value;
	return value;
};

/// <summary>
/// テクスチャを読み込む
/// </summary>
void FlameEmitter::LoadTexture(int texslot)
{
	s_TextureSlot = texslot;
	// 0番スロットにテクスチャをロード
	ParticleEffectManager::getInstance()->Load(s_TextureSlot, L"Resources/Textures/effect2.png");
}

/// <summary>
/// テクスチャを破棄する
/// </summary>
void FlameEmitter::DisposeTexture()
{
	ParticleEffectManager::getInstance()->Dispose(s_TextureSlot);
}

FlameEmitter::FlameEmitter(DirectX::SimpleMath::Vector3 position)
{
	m_Position = position;
	m_param = 0;
}

/// <summary>
/// 更新
/// </summary>
void FlameEmitter::Update()
{
	m_param += 0.1f;
	m_Position.x = cosf(m_param);
	m_Position.y = sinf(m_param);
	//ParticleEffectManager::getInstance()->Entry(
	//	0,	// テクスチャ番号
	//	(int)RandomRange(60, 150),	// 生存時間
	//	Vector3(0, 0, 0), // 座標
	//	Vector3(0, 0.015f, 0), // 速度
	//	Vector3(0, 0, 0), // 加速度
	//	0.0f, 0.0f,	// 開始／終了角度
	//	1.0f, 0.0f,	// 開始／終了スケール
	//	Color(
	//		RandomRange(0.7f, 1.0f), 
	//		RandomRange(0.0f, 0.03f),
	//		0, 1),
	//	Color(
	//		RandomRange(0.7f, 1.0f),
	//		RandomRange(0.0f, 0.1f),
	//		0, 0)
	//);	// 開始／終了カラー

	float angle = RandomRange(-XM_PI, XM_PI);
	float x, y, z;
	x = 0.3f * cosf(angle);
	y = 0;
	z = 0.3f * sinf(angle);

	for (int i = 0; i < 5; i++)
	{
		float startAngle = RandomRange(0, 360);
		ParticleEffectManager::getInstance()->Entry(
			0,	// テクスチャ番号
			(int)RandomRange(60, 120),	// 生存時間
			m_Position + Vector3(x, y, z), // 座標
			Vector3(0, 0.010f, 0), // 速度
			Vector3(0, 0, 0), // 加速度
			startAngle, startAngle + 180.0f,	// 開始／終了角度
			1.5f, 1.5f,	// 開始／終了スケール
			Color(
				RandomRange(0.1f, 0.3f),
				RandomRange(0.0f, 0.15f),
				0, 1),
			Color(
				RandomRange(0.1f, 0.15f),
				RandomRange(0.0f, 0.0f),
				0, 0)
		);	// 開始／終了カラー
	}
}
