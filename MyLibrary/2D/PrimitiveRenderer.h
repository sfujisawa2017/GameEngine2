/// <summary>
/// プリミティブ描画クラス
/// </summary>
#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <memory>
#include <d3d11_1.h>

#include <Effects.h>
#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <SimpleMath.h>
#include <CommonStates.h>

namespace MyLibrary
{
	class PrimitiveRenderer
	{
	public:
		using Vector2 = DirectX::SimpleMath::Vector2;
		using Color = DirectX::SimpleMath::Color;
	public:
		static PrimitiveRenderer* GetInstance();
	private:
		static std::unique_ptr<PrimitiveRenderer> m_Instance;
	public:
		// 初期化
		void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
		// 線を描画
		void DrawLine(Vector2 start, Vector2 end, Color color );
		// 四角形を描画
		void DrawBox(Vector2 top_left, Vector2 bottom_right, Color color = Color(1,1,1,1), bool fill_flag = true);
		// 円を描画
		void DrawCircle(Vector2 center, float radius, Color color = Color(1,1,1,1), bool fill_flag = true);
		// 三角形を描画
		void DrawTriangle(Vector2 p1, Vector2 p2, Vector2 p3, Color color = Color(1, 1, 1, 1), bool fill_flag = true);
	private:
		// プリミティブバッチ
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>	m_PrimitiveBatch;
		// 汎用描画設定
		std::unique_ptr<DirectX::CommonStates>	m_CommonStates;
		// 標準エフェクト
		std::unique_ptr<DirectX::BasicEffect>	m_BasicEffect;
		// デバイスコンテキスト
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;
		// 入力レイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	};
}