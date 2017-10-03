#include "PrimitiveRenderer.h"
#include "..\Common\DeviceResources.h"

#include <assert.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

// 静的メンバ変数の実体
std::unique_ptr<PrimitiveRenderer> PrimitiveRenderer::m_Instance;

PrimitiveRenderer * PrimitiveRenderer::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance.reset(new PrimitiveRenderer);
	}

	return m_Instance.get();
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="device">DirectX11デバイス</param>
/// <param name="context">DirectX11デバイスコンテキスト</param>
void PrimitiveRenderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	assert(m_PrimitiveBatch == nullptr);

	m_d3dContext = context;

	m_PrimitiveBatch = std::make_unique<PrimitiveBatch<DirectX::VertexPositionColor>>(context);

	m_CommonStates = std::make_unique<CommonStates>(device);

	m_BasicEffect = std::make_unique<BasicEffect>(device);
	// 頂点カラーを有効化
	m_BasicEffect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	// 描画設定を元に適切なシェーダを取得
	m_BasicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	// 入力レイアウトを作成
	device->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, shaderByteCode, byteCodeLength, m_InputLayout.ReleaseAndGetAddressOf());

	// 画面サイズを取得
	RECT rect = DeviceResources::GetInstance()->GetOutputSize();
	// 各種変換行列をセット
	m_BasicEffect->SetWorld(Matrix::Identity);
	m_BasicEffect->SetView(Matrix::CreateLookAt(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0)));
	m_BasicEffect->SetProjection(Matrix::CreateOrthographicOffCenter(rect.left, rect.right, rect.bottom, rect.top, 0.1f, 10.0f));
}

/// <summary>
/// 線を描画
/// </summary>
/// <param name="start">描画する線の起点座標</param>
/// <param name="end">描画する線の終点座標</param>
/// <param name="color">描く線の色</param>
void PrimitiveRenderer::DrawLine(Vector2 start, Vector2 end, Color color)
{
	DirectX::VertexPositionColor v1, v2;

	v1.position = Vector3(start.x, start.y, 0);
	v2.position = Vector3(end.x, end.y, 0);

	v1.color = color;
	v2.color = color;

	m_BasicEffect->Apply(m_d3dContext.Get());

	m_d3dContext->OMSetDepthStencilState(m_CommonStates->DepthNone(), 0);
	m_d3dContext->OMSetBlendState(m_CommonStates->AlphaBlend(), nullptr, 0xffffffff);
	m_d3dContext->RSSetState(m_CommonStates->CullNone());
	m_d3dContext->IASetInputLayout(m_InputLayout.Get());

	m_PrimitiveBatch->Begin();
	m_PrimitiveBatch->DrawLine(v1, v2);
	m_PrimitiveBatch->End();
}

/// <summary>
/// 四角形を描画
/// </summary>
/// <param name="top_left">描画する四角形の左上の頂点座標</param>
/// <param name="bottom_right">描画する四角形の右下の頂点座標</param>
/// <param name="color">描画する四角形の色</param>
/// <param name="fill_flag">内部を塗りつぶすか</param>
void PrimitiveRenderer::DrawBox(Vector2 top_left, Vector2 bottom_right, Color color, bool fill_flag)
{
	m_BasicEffect->Apply(m_d3dContext.Get());

	m_d3dContext->OMSetDepthStencilState(m_CommonStates->DepthNone(), 0);
	m_d3dContext->OMSetBlendState(m_CommonStates->AlphaBlend(), nullptr, 0xffffffff);
	m_d3dContext->RSSetState(m_CommonStates->CullNone());
	m_d3dContext->IASetInputLayout(m_InputLayout.Get());

	// 塗りつぶす？
	if (fill_flag)
	{
		DirectX::VertexPositionColor v1, v2, v3, v4;

		// 左上
		v1.position = Vector3(top_left.x, top_left.y, 0);
		// 右上
		v2.position = Vector3(bottom_right.x, top_left.y, 0);
		// 右下
		v3.position = Vector3(bottom_right.x, bottom_right.y, 0);
		// 左下
		v4.position = Vector3(top_left.x, bottom_right.y, 0);

		v1.color = color;
		v2.color = color;
		v3.color = color;
		v4.color = color;		

		m_PrimitiveBatch->Begin();
		m_PrimitiveBatch->DrawQuad(v1, v2, v3, v4);
		m_PrimitiveBatch->End();
	}
	else
	{
		DirectX::VertexPositionColor vertices[5];

		// 左上
		vertices[0].position = Vector3(top_left.x, top_left.y, 0);
		// 右上
		vertices[1].position = Vector3(bottom_right.x, top_left.y, 0);
		// 右下
		vertices[2].position = Vector3(bottom_right.x, bottom_right.y, 0);
		// 左下
		vertices[3].position = Vector3(top_left.x, bottom_right.y, 0);
		// 左上
		vertices[4].position = vertices[0].position;

		for (int i = 0; i < 5; i++)
		{
			vertices[i].color = color;
		}

		m_PrimitiveBatch->Begin();
		m_PrimitiveBatch->Draw(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, vertices, 5);
		m_PrimitiveBatch->End();
	}
}

/// <summary>
/// 円を描画
/// </summary>
/// <param name="center">描く円の中心座標</param>
/// <param name="radius">描く円の半径</param>
/// <param name="color">円の色</param>
/// <param name="fill_flag">内部を塗りつぶすか</param>
void PrimitiveRenderer::DrawCircle(Vector2 center, float radius, Color color, bool fill_flag)
{
	// 角度分割数
	const int DIV = 30;

	Vector3 center3d = Vector3(center.x, center.y, 0);

	m_BasicEffect->Apply(m_d3dContext.Get());

	m_d3dContext->OMSetDepthStencilState(m_CommonStates->DepthNone(), 0);
	m_d3dContext->OMSetBlendState(m_CommonStates->AlphaBlend(), nullptr, 0xffffffff);
	m_d3dContext->RSSetState(m_CommonStates->CullCounterClockwise());
	m_d3dContext->IASetInputLayout(m_InputLayout.Get());

	// 塗りつぶす？
	if (fill_flag)
	{
		uint16_t indices[DIV*3];
		DirectX::VertexPositionColor vertices[DIV+1];

		// インデックステーブル
		for (int i = 0; i < DIV; i++)
		{
			indices[i * 3 + 0] = DIV; // 最後の点
			indices[i * 3 + 1] = i;
			indices[i * 3 + 2] = i+1;
		}
		indices[(DIV-1) * 3 + 2] = 0;

		// 頂点座標を一旦全て中心点の座標で設定
		for (int i = 0; i < DIV+1; i++)
		{
			vertices[i].position = center3d;
			vertices[i].color = color;
		}
		// 円の外周分ずらす
		for (int i = 0; i < DIV; i++)
		{
			vertices[i].position.x += radius * cosf(XM_2PI / DIV * i);
			vertices[i].position.y += radius * sinf(XM_2PI / DIV * i);
		}

		m_PrimitiveBatch->Begin();
		m_PrimitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, DIV * 3, vertices, DIV + 1);
		m_PrimitiveBatch->End();
	}
	else
	{
		DirectX::VertexPositionColor vertices[DIV+1];

		// 円の外周分ずらす
		for (int i = 0; i < DIV; i++)
		{
			vertices[i].position.x = center.x + radius * cosf(XM_2PI / DIV * i);
			vertices[i].position.y = center.y + radius * sinf(XM_2PI / DIV * i);
			vertices[i].position.z = 0;
		}
		// 最後は最初の点に戻る
		vertices[DIV].position = vertices[0].position;

		for (int i = 0; i < DIV+1; i++)
		{
			vertices[i].color = color;
		}

		m_PrimitiveBatch->Begin();
		m_PrimitiveBatch->Draw(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, vertices, DIV+1);
		m_PrimitiveBatch->End();
	}
}

/// <summary>
/// 三角形を描画
/// </summary>
/// <param name="p1">三角形を描く座標の１点目</param>
/// <param name="p2">三角形を描く座標の２点目</param>
/// <param name="p3">三角形を描く座標の３点目</param>
/// <param name="color">描画に使う色</param>
/// <param name="fill_flag">内部を塗りつぶすか</param>
void PrimitiveRenderer::DrawTriangle(Vector2 p1, Vector2 p2, Vector2 p3, Color color, bool fill_flag)
{
	m_BasicEffect->Apply(m_d3dContext.Get());

	m_d3dContext->OMSetDepthStencilState(m_CommonStates->DepthNone(), 0);
	m_d3dContext->OMSetBlendState(m_CommonStates->AlphaBlend(), nullptr, 0xffffffff);
	m_d3dContext->RSSetState(m_CommonStates->CullNone());
	m_d3dContext->IASetInputLayout(m_InputLayout.Get());

	// 塗りつぶす？
	if (fill_flag)
	{
		DirectX::VertexPositionColor v1, v2, v3;

		v1.position = Vector3(p1.x, p1.y, 0);
		v2.position = Vector3(p2.x, p2.y, 0);
		v3.position = Vector3(p3.x, p3.y, 0);

		v1.color = color;
		v2.color = color;
		v3.color = color;

		m_PrimitiveBatch->Begin();
		m_PrimitiveBatch->DrawTriangle(v1, v2, v3);
		m_PrimitiveBatch->End();
	}
	else
	{
		DirectX::VertexPositionColor vertices[4];

		vertices[0].position = Vector3(p1.x, p1.y, 0);
		vertices[1].position = Vector3(p2.x, p2.y, 0);
		vertices[2].position = Vector3(p3.x, p3.y, 0);
		vertices[3].position = vertices[0].position;

		for (int i = 0; i < 4; i++)
		{
			vertices[i].color = color;
		}

		m_PrimitiveBatch->Begin();
		m_PrimitiveBatch->Draw(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, vertices, 4);
		m_PrimitiveBatch->End();
	}
}
