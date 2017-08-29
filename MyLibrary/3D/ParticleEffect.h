/// <summary>
/// パーティクルエフェクト
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <vector>
#include <list>
#include <map>
#include "Obj3D.h"
#include "..\Camera\Camera.h"

namespace MyLibrary
{
	class ParticleEffectManager
	{
	public:
		using Vector3 = DirectX::SimpleMath::Vector3;
		using Color = DirectX::SimpleMath::Color;

		static const int PARTICLE_NUM_MAX;

		// 入力レイアウトの定義.
		static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

		// コンスタントバッファ
		struct Constants
		{
			DirectX::SimpleMath::Matrix VP;
			DirectX::SimpleMath::Matrix Billboard;
		};

		// シングルトンインスタンスの取得
		static ParticleEffectManager* getInstance();
		// 初期化
		void Initialize();
		// テクスチャロード
		void Load(int texnumber, const wchar_t* filename);
		// 全破棄
		void DisposeAll();
		// 指定番号のパーティクルグループを破棄
		void Dispose(int groupNumber);
		// 発生させる
		void Entry(int groupNumber, int frame, Vector3 position, Vector3 velocity, Vector3 accel, float start_rotation, float end_rotation, float start_scale, float end_scale, Color start_color, Color end_color);
		void Update();
		void Draw();

		// アクセッサ
		void SetCamera(Camera* camera) { m_Camera = camera; }
	private:
		ParticleEffectManager();

		// エフェクト1個分
		class ParticleEffect
		{
		public:
			ParticleEffect()
			{
				frame = 0;
				num_frame = 0;
			}

			bool Update(const Vector3& gravity);

			//// オブジェクト
			//Obj3D obj;
			// 座標
			Vector3 position;
			// 速度
			Vector3 velocity;
			// 加速度
			Vector3 accel;
			// 色
			Color color;
			// 回転
			float rotation;
			// スケール
			float scale;
			// 初期値
			float s_rotation;
			float s_scale;
			Color s_color;
			// 最終値
			float e_rotation;
			float e_scale;
			Color e_color;
			// 現在フレーム
			int   frame;
			// 終了フレーム
			int   num_frame;
		};

		// 同じテクスチャを使用するエフェクトグループ
		struct ParticleEffectGroup
		{
			// テクスチャ
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;
			// モデルエフェクトの配列
			std::list<ParticleEffect> m_ParticleEffects;
			// 頂点データ配列
			std::vector<DirectX::VertexPositionColorTexture> m_Vertices;
		};

		// 頂点シェーダ
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		// ジオメトリシェーダ
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_GeometryShader;
		// ピクセルシェーダ
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		// コンスタントバッファ
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
		// プリミティブバッチ
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_PrimitiveBatch;
		// 入力レイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
		//テクスチャーサンプラー
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_Sampler;

		// カメラ
		Camera*	m_Camera;

		// パーティクルグループの配列
		std::map<int, ParticleEffectGroup> m_ParticleGroups;
		// 重力加速度
		Vector3 gravity;
		
	};
}