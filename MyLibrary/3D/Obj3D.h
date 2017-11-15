/// <summary>
/// ３Ｄオブジェクトクラス
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>

#include "..\Camera\Camera.h"
#include <map>

namespace MyLibrary
{
	// 3Dオブジェクト
	class Obj3D
	{
		/// <summary>
		/// static member
		/// </summary>
	public:
		using Color = DirectX::SimpleMath::Color;
		using Vector3 = DirectX::SimpleMath::Vector3;
		using Matrix = DirectX::SimpleMath::Matrix;
		using Quaternion = DirectX::SimpleMath::Quaternion;

		static const std::wstring RESOURCE_DIRECTORY;
		static const std::wstring RESOURCE_EXT;

		// 設定
		struct CommonDef
		{
			ID3D11Device* pDevice;
			ID3D11DeviceContext* pDeviceContext;
			Camera* pCamera;

			CommonDef()
			{
				pDevice = nullptr;
				pDeviceContext = nullptr;
				pCamera = nullptr;
			}
		};
		// 共用データ
		struct Common
		{
			ID3D11Device*	device;
			// デバイスコンテキストへのポインタ
			ID3D11DeviceContext*	deviceContext;
			// 描画ステートへのポインタ
			std::unique_ptr<DirectX::CommonStates>	states;
			// 共用のエフェクトファクトリ
			std::unique_ptr<DirectX::EffectFactory>	effectFactory;
			// 共用のカメラ（描画時に使用）
			Camera* camera;
			// 読み込み済みモデルコンテナ
			std::map<std::wstring, std::shared_ptr<DirectX::Model>> modelarray;
			// 減算描画ステート
			Microsoft::WRL::ComPtr<ID3D11BlendState> blendStateSubtract;
		};
		// 静的メンバ関数
		// システム初期化
		static void InitializeCommon(CommonDef def);
		// 全モデル破棄
		static void UnloadAll();

		// 減算描画設定をセット
		static void SetSubtractive();

		//getter
		static Camera* GetCamera() { return s_Common.camera; }

	private:
		// 静的メンバ変数（全オブジェクトで共有）
		static Common s_Common;

		/// <summary>
		/// non-static member
		/// </summary>
	public:
		Obj3D();
		virtual ~Obj3D();
		// ファイルからモデルを読み込む
		void LoadModel(const wchar_t*filename = nullptr, bool share = true);
		// 行列の更新
		void Update(bool recursive = true);
		// 描画
		void Draw(bool recursive = true);
		// 減算描画での描画（影用）
		void DrawSubtractive(bool recursive = true);
		void DrawBillboard(bool recursive = true);
		void DrawBillboardConstrainY(bool recursive = true);
		// オブジェクトのライティングを無効にする
		void DisableLighting();
		// エフェクトをセット
		void SetEffect(std::shared_ptr<DirectX::IEffect>& effect);
		// アルファブレンディングを有効にする
		void EnableAlpha();
		// 親オブジェクトをセット
		void SetParent(Obj3D* parent);
		// 子オブジェクトを追加
		void AddChild(Obj3D* child);
		// ダーティフラグを立てる
		void SetDirty();
		// ダーティフラグチェック
		bool CheckDirty();

		//setter
		void SetTrans(const Vector3& trans);
		void SetRot(const Vector3& rot);
		void SetRotQuat(const Quaternion& quat);
		void SetScale(float scale);
		void SetScale(const Vector3& scale);
		void SetLocalWorld(const Matrix& mat);
		//getter
		const Vector3& GetTrans() { return m_Trans; }
		const Vector3& GetRot() { return m_RotEuler; }
		const Quaternion& GetRotQuat() { return m_RotQuat; }
		const Vector3& GetScale() { return m_Scale; }
		const Matrix& GetLocalWorld() { return m_LocalWorld; }

	private:
		// モデルデータへのポインタ
		std::shared_ptr<DirectX::Model> m_pModel;
		// 平行移動
		Vector3 m_Trans;
		// 回転
		bool m_UseQuternion;
		union
		{
			// クォータニオン
			Quaternion m_RotQuat;
			// オイラー角
			Vector3 m_RotEuler;
		};
		// スケーリング
		Vector3 m_Scale;
		// ワールド行列
		Matrix m_LocalWorld;
		// 親オブジェクトへのポインタ
		Obj3D* m_pParent;
		// 子オブジェクトへのポインタ配列
		std::vector<Obj3D*>	m_Children;
		// 行列ダーティフラグ
		bool m_IsDirty;
	};
}
