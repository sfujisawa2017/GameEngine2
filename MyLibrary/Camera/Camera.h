#pragma once

#include	<d3d11_1.h>
#include	<SimpleMath.h>

#include "..\Collision\Collision.h"

namespace MyLibrary
{
	// カメラ
	class Camera
	{
	public:
		using Vector3 = DirectX::SimpleMath::Vector3;
		using Vector2 = DirectX::SimpleMath::Vector2;
		//static const int VIEW_FRUSTUM_PLANE_NUM = 6;
		// 視錘台の判定平面数
		enum VIEW_FRUSTUM_PLANE
		{
			VIEW_FRUSTUM_PLANE_NEAR,	// 手前
			VIEW_FRUSTUM_PLANE_FAR,		// 奥
			VIEW_FRUSTUM_PLANE_LEFT,	// 左
			VIEW_FRUSTUM_PLANE_RIGHT,	// 右
			VIEW_FRUSTUM_PLANE_TOP,		// 上
			VIEW_FRUSTUM_PLANE_BOTTOM,	// 下

			VIEW_FRUSTUM_PLANE_NUM
		};
	protected:
		// ビュー行列
		DirectX::SimpleMath::Matrix m_View;
		// カメラ座標
		DirectX::SimpleMath::Vector3 m_Eyepos;
		// ターゲット座標
		DirectX::SimpleMath::Vector3 m_Refpos;
		// 上方向ベクトル
		DirectX::SimpleMath::Vector3 m_Upvec;

		// プロジェクション行列
		DirectX::SimpleMath::Matrix m_Proj;
		// 縦方向視野角
		float m_FovY;
		// アスペクト比
		float m_Aspect;
		// ニアクリップ
		float m_NearClip;
		// ファークリップ
		float m_FarClip;

		// ビルボード行列
		DirectX::SimpleMath::Matrix m_Billboard;
		// ビルボード行列(Y軸周り限定）
		DirectX::SimpleMath::Matrix m_BillboardConstrainY;
		// 視錘台判定平面
		Plane m_ViewFrustumPlanes[VIEW_FRUSTUM_PLANE_NUM];
	public:
		// コンストラクタ
		Camera(int width, int height);
		// デストラクタ
		virtual ~Camera();
		// 更新
		virtual void Update();
		// アクセッサ
		void SetEyepos(const DirectX::SimpleMath::Vector3 pos) { m_Eyepos = pos; }
		void SetRefpos(const DirectX::SimpleMath::Vector3 pos) { m_Refpos = pos; }
		void SetUpvec(const DirectX::SimpleMath::Vector3 vec) { m_Upvec = vec; }
		const DirectX::SimpleMath::Vector3& GetEyepos() const { return m_Eyepos; }
		const DirectX::SimpleMath::Vector3& GetRefpos() const { return m_Refpos; }
		const DirectX::SimpleMath::Vector3& GetUpvec() const { return m_Upvec; }
		const DirectX::SimpleMath::Matrix& GetView() const { return m_View; }
		const DirectX::SimpleMath::Matrix& GetProj() const { return m_Proj; }
		const DirectX::SimpleMath::Matrix& GetBillboard() const { return m_Billboard; }
		const DirectX::SimpleMath::Matrix& GetBillboardConstrainY() const { return m_BillboardConstrainY; }

		bool Project(const Vector3& worldPos, Vector2* screenPos);
		void UnProject(const Vector2& screenPos, Segment* worldSegment);
		void UnProject(const Vector2& screenPos, float distance, Vector3 * worldPos);
		void CalcBillboard();
		void CalcViewFrustumPlanes();
		bool TestInFrustum(Vector3& p, float radius);
	};
}