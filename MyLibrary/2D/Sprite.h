/// <summary>
/// ２Ｄ画像表示クラス
/// </summary>
#pragma once

#include <memory>
#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <vector>

#include "TextureCache.h"
#include "SpriteRenderer.h"

namespace MyLibrary
{
	// ２Ｄ画像表示クラス
	class Sprite
	{
	public:
		friend SpriteRenderer;
		using Vector2 = DirectX::SimpleMath::Vector2;
		using Vector3 = DirectX::SimpleMath::Vector3;
		using Quaternion = DirectX::SimpleMath::Quaternion;
		using Matrix = DirectX::SimpleMath::Matrix;
		using Color = DirectX::SimpleMath::Color;

		// コンストラクタ
		Sprite();
		void Update(bool recursive = true);
		// 描画
		void Draw(bool recursive = true);

		void SetParent(Sprite * parent);

		void AddChild(Sprite * child);

		/// <summary>
		/// property
		/// </summary>
		void SetPosition(const Vector2& position) { m_Position = position; }
		void SetRotation(float rotation) { m_Rotation = rotation; }
		void SetScale(float scale) { m_Scale = Vector2(scale, scale); }
		void SetScale(const Vector2& scale) { m_Scale = scale; }
		void SetColor(const Color& color) { m_Color = color; }
		void SetTexture(Texture* texture) { m_Texture = texture; }
		void SetTextureRect(const RECT& rect) { m_TextureRect = rect; }
		void SetAnchorPoint(const Vector2& anchor) { m_AnchorPoint = anchor; }
		void SetSpriteManager(SpriteRenderer* manager) { m_SpriteManager = manager; }

		const DirectX::SimpleMath::Matrix& GetLocalWorld() { return m_LocalWorld; }

	private:
		// 座標
		Vector2 m_Position;
		// 回転
		float m_Rotation;
		// スケーリング
		Vector2 m_Scale;
		// ワールド行列
		Matrix m_LocalWorld;
		// 色
		Color m_Color;
		// テクスチャ
		Texture* m_Texture;
		// テクスチャ矩形
		RECT m_TextureRect;
		// 画像中心
		Vector2 m_AnchorPoint;
		// 親オブジェクトへのポインタ
		Sprite* m_Parent;
		// 子オブジェクトへのポインタ配列
		std::vector<Sprite*>	m_Children;
		// スプライトレンダラーへのポインタ
		SpriteRenderer* m_SpriteManager;
	};

	// スプライトファクトリー
	class SpriteFactory
	{
	public:
		SpriteFactory(SpriteRenderer * spriteRenderer, TextureCache* textureCache);
		std::unique_ptr<Sprite> CreateFromFile(const wchar_t* filename);
	private:
		// スプライトレンダラーへのポインタ
		SpriteRenderer* m_SpriteRenderer;
		// テクスチャキャッシュ
		TextureCache* m_TextureCache;
	};
}