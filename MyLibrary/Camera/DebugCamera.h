#pragma once
// デバッグ用カメラクラス
#include "Camera.h"

namespace MyLibrary
{
	// デバッグ用カメラクラス
	class DebugCamera : public Camera
	{
		// カメラの距離
		static const float DEFAULT_CAMERA_DISTANCE;

		// 横回転
		float m_yAngle, m_yTmp;

		// 縦回転
		float m_xAngle, m_xTmp;

		// ドラッグされた座標
		int m_x, m_y;

		float m_sx, m_sy;

		// スクロールフォイール値
		int m_scrollWheelValue;

		// カメラ距離
		float cameraDistance;

	private:

		void Motion(int x, int y);

	public:
		// コンストラクタ
		DebugCamera(int w, int h);

		// 更新
		void Update() override;

		// カメラ距離
		void SetCameraDistance(float distance) { cameraDistance = distance; }
		// 横回転
		void SetYAngle(float yAngle) { m_yAngle = yAngle; m_yTmp = yAngle; }
		// 縦回転
		void SetXAngle(float xAngle) { m_xAngle = xAngle; m_xTmp = xAngle; }
	};
}
