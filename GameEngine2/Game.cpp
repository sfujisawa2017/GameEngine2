//
// Game.cpp
//

#include "Game.h"
#include <WICTextureLoader.h>

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;
using namespace MyLibrary;

// パーティクル最大数
const int Game::PARTICLE_NUM_MAX = 20000;

const std::vector<D3D11_INPUT_ELEMENT_DESC> Game::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Game::Game(HINSTANCE hInstance, int nCmdShow)
	: Framework(hInstance, nCmdShow)
{
}

void Game::Initialize()
{
	DeviceResources* deviceResources = DeviceResources::GetInstance();

	// マウスライブラリにウィンドウハンドルを渡す
	MouseUtil::GetInstance()->SetWindow(DeviceResources::GetInstance()->GetWindow());
	
	// ウィンドウ矩形取得
	RECT windowRect = deviceResources->GetOutputSize();
	// デバッグカメラ作成
	m_Camera = std::make_unique<DebugCamera>(windowRect.right- windowRect.left, windowRect.bottom - windowRect.top);

	{ // Obj3D初期化
		// 設定
		Obj3D::CommonDef def;
		def.pCamera = m_Camera.get();
		def.pDevice = deviceResources->GetD3DDevice();
		def.pDeviceContext = deviceResources->GetD3DDeviceContext();
		// 設定を元に初期化
		Obj3D::InitializeCommon(def);
	}

	// 天球読み込み
	m_ObjSkydome = std::make_unique<Obj3D>();
	m_ObjSkydome->LoadModel(L"skydome");
	m_ObjSkydome->DisableLighting();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");

	ID3D11Device* device = deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = deviceResources->GetD3DDeviceContext();

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_VertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// プリミティブバッチ作成
	//  各頂点に含まれる情報が、位置・色・テクスチャUV
	m_PrimitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(context, 1, PARTICLE_NUM_MAX);

	// 頂点フォーマットを指定して入力レイアウト作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_InputLayout.GetAddressOf());

	//シェーダに共通データを渡す為の
	//コンスタントバッファー作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(Constants);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(device->CreateBuffer(&cb, NULL, m_ConstantBuffer.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateBuffer Failed.", NULL, MB_OK);
		return;
	}	

	{
		// １つ分の頂点データ
		VertexPositionColorTexture vertexData;
		vertexData.position = Vector3(0, -0.5f, 0);	// 下
		vertexData.color = Vector4(1, 1, 1, 1); // 白
		vertexData.textureCoordinate = Vector2(0, 1);	// テクスチャ画像の左下隅
		m_Vertices.push_back(vertexData);
	}

	{
		// １つ分の頂点データ
		VertexPositionColorTexture vertexData;
		vertexData.position = Vector3(0.5f, -0.5f, 0);	// 右下
		vertexData.color = Vector4(1, 1, 1, 1); // 白
		vertexData.textureCoordinate = Vector2(1, 1);	// テクスチャ画像の右下隅
		m_Vertices.push_back(vertexData);
	}

	{
		// １つ分の頂点データ
		VertexPositionColorTexture vertexData;
		vertexData.position = Vector3(0, 0, 0);	// 原点
		vertexData.color = Vector4(1, 1, 1, 1); // 白
		vertexData.textureCoordinate = Vector2(0, 0);	// テクスチャ画像の左上隅
		m_Vertices.push_back(vertexData);
	}

	{
		// １つ分の頂点データ
		VertexPositionColorTexture vertexData;
		vertexData.position = Vector3(0.5f, 0, 0);	// 右
		vertexData.color = Vector4(1, 1, 1, 1); // 白
		vertexData.textureCoordinate = Vector2(1, 0);	// テクスチャ画像の右下隅
		m_Vertices.push_back(vertexData);
	}

	m_CommonStates = std::make_unique<CommonStates>(device);

	// テクスチャ読み込み
	if (FAILED(CreateWICTextureFromFile(device, L"Resources/Textures/effect1.png", nullptr,
		m_Texture.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"ParticleTest Failed.", NULL, MB_OK);
		return;
	}

	// テクスチャサンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	if (FAILED(device->CreateSamplerState(&SamDesc, m_Sampler.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateSamplerState Failed.", NULL, MB_OK);
		return;
	}
}

void Game::Finalize()
{

}

#pragma region Frame Update
/// <summary>
/// 毎フレーム更新
/// </summary>
/// <param name="timer">時間情報</param>
void Game::Update(StepTimer const& timer)
{
	MouseUtil::GetInstance()->Update();

	//for (auto& vertex : m_Vertices)
	//{
	//	vertex.position.x += 0.01f;
	//}

	//for (auto& vertex : m_Vertices)
	//{
	//	vertex.color.x -= 0.01f;
	//	vertex.color.y -= 0.01f;
	//	vertex.color.z -= 0.01f;
	//}

	//for (auto& vertex : m_Vertices)
	//{
	//	vertex.textureCoordinate.x += 0.03f;
	//}

	m_Camera->Update();
}
#pragma endregion

#pragma region Frame Render
/// <summary>
/// 毎フレーム描画
/// </summary>
void Game::Render()
{
	m_ObjSkydome->Draw();

	DeviceResources* deviceResources = DeviceResources::GetInstance();
	ID3D11DeviceContext* context = deviceResources->GetD3DDeviceContext();

	// ビュー、プロジェクション行列を合成
	Matrix world = Matrix::Identity;
	Matrix view = Matrix::Identity;
	Matrix proj = Matrix::Identity;

	world = Matrix::CreateScale(3.0f);

	view = m_Camera->GetView();
	proj = m_Camera->GetProj();
	Matrix wvp = world * view * proj;

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(context->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		Constants constants;

		constants.WVP = wvp;

		//データを上書き
		memcpy_s(pData.pData, pData.RowPitch, &constants, sizeof(constants));
		context->Unmap(m_ConstantBuffer.Get(), 0);
	}
	//このコンスタントバッファーをどのシェーダーで使うか
	context->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 0, nullptr);

	context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

	context->IASetInputLayout(m_InputLayout.Get());

	// アルファブレンド描画
	context->OMSetBlendState(m_CommonStates->NonPremultiplied(), nullptr, 0xFFFFFFFF);

	// 深度バッファを更新しない
	context->OMSetDepthStencilState(m_CommonStates->DepthRead(), 0);

	// 時計回りの面を陰面消去
	context->RSSetState(m_CommonStates->CullNone());

	// テクスチャサンプラー設定
	context->PSSetSamplers(0, 1, m_Sampler.GetAddressOf());
	//テクスチャーをシェーダーに渡す		
	context->PSSetShaderResources(0, 1, m_Texture.GetAddressOf());

	// 頂点データをすべて渡して描画する
	m_PrimitiveBatch->Begin();
	m_PrimitiveBatch->Draw(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, &m_Vertices[0], m_Vertices.size());
	m_PrimitiveBatch->End();
}
#pragma endregion
