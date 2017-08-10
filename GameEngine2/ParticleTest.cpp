#include "ParticleTest.h"
#include "MyLibrary.h"
#include <WICTextureLoader.h>

using namespace MyLibrary;
using namespace DirectX;
using namespace DirectX::SimpleMath;

const int ParticleTest::PARTICLE_NUM_MAX = 10000;

const std::vector<D3D11_INPUT_ELEMENT_DESC> ParticleTest::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3)+ sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

ParticleTest::ParticleTest()
{
	ID3D11Device* device = DeviceResources::GetInstance()->GetD3DDevice();
	ID3D11DeviceContext* context = DeviceResources::GetInstance()->GetD3DDeviceContext();

	m_CommonStates = std::make_unique<CommonStates>(device);

	// ファイル読み込み
	// 頂点シェーダ
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleTestVS.cso");
	// ジオメトリシェーダ
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleTestGS.cso");
	// ピクセルシェーダ
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleTestPS.cso");
	
	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_VertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"ParticleTest Failed.", NULL, MB_OK);
		return;
	}

	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"ParticleTest Failed.", NULL, MB_OK);
		return;
	}

	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"ParticleTest Failed.", NULL, MB_OK);
		return;
	}

	// プリミティブバッチ作成
	m_PrimitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(context, 1, PARTICLE_NUM_MAX);

	// 頂点フォーマットを指定して入力レイアウト作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_InputLayout.GetAddressOf());

	for ( int i = 0;  i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			VertexPositionColorTexture vertex;
			vertex.position = Vector3(i * 0.5f - 2.5f, j * 0.5f - 2.5f, 0);
			vertex.color = Vector4(0.2f, 0, 0, 0);
			vertex.textureCoordinate = Vector2(0, 0);

			m_Vertices.push_back(vertex);
		}
	}

	//シェーダに共通データを渡す為の
	//コンスタントバッファー作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(Constants);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(device->CreateBuffer(&cb, NULL, m_pWVPConstantBuffer.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"ParticleTest Failed.", NULL, MB_OK);
		return;
	}

	m_Camera = std::make_unique<DebugCamera>(960,640);

	// テクスチャサンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	if (FAILED(device->CreateSamplerState(&SamDesc, m_Sampler.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"ParticleTest Failed.", NULL, MB_OK);
		return;
	}
	// テクスチャ読み込み
	if (FAILED(CreateWICTextureFromFile(device, L"Resources/Images/test.png", nullptr,
		m_Texture.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"ParticleTest Failed.", NULL, MB_OK);
		return;
	}

	ParticleEffectManager::getInstance()->Initialize();
	ParticleEffectManager::getInstance()->Load(0, L"Resources/Images/test.png");
	ParticleEffectManager::getInstance()->SetCamera(m_Camera.get());

}

void ParticleTest::Update()
{
	static float angle = 0;
	angle += 0.05f;

	static int rest[2] = { 0, 2 };

	for (int j = 0; j < 1; j++)
	{
		if (--rest[j] < 0)
		{
			rest[j] = 8;
		}
		else
		{
			continue;
		}

		for (int i = 0; i < 1; i++)
		{
			float angle_add = XM_2PI / 20.0f * i;
		
			//VertexPositionColorTexture vertex;
			//vertex.position = Vector3(i * 0.5f - 2.5f, j * 0.5f - 2.5f, 0);
			//vertex.color = Vector4(0.2f, 0, 0, 0);
			//vertex.textureCoordinate = Vector2(0, 0);

			//m_Vertices.push_back(vertex);

			//Vector3 position = Vector3(i * 0.5f - 2.5f, j * 0.5f - 2.5f, 0);
			Vector3 position = Vector3(0.05f, 0, 0);
			if ( j == 0)
				position = Vector3::Transform(position, Matrix::CreateRotationY(angle + angle_add));
			else
				position = Vector3::Transform(position, Matrix::CreateRotationY(angle + angle_add));
			Vector3 velocity = position *0.5f;
			Vector3 accel = Vector3(0, 0, 0);
			Color s_color = Color(1, 0, 0, 0);
			Color e_color = Color(1, 0, 0, 0);
			if (j == 1)
			{
				s_color = Color(0, 1, 0, 0);
				e_color = Color(0, 1, 0, 0);
			}

			ParticleEffectManager::getInstance()->Entry(0, 120,
				position, velocity, accel, 0, 0, 1.0f, 1.0f,
				s_color, e_color);
		}
	}
	//for (unsigned int i = 0; i < m_Vertices.size(); i++)
	//{
	//	m_Vertices[i].position.x += 0.001f;
	//}

	m_Camera->Update();

	ParticleEffectManager::getInstance()->Update();
}

void ParticleTest::Draw()
{
	ParticleEffectManager::getInstance()->Draw();
#if 0
	ID3D11DeviceContext* context = DeviceResources::GetInstance()->GetD3DDeviceContext();

	// ビュー、プロジェクション行列を合成
	Matrix view = Matrix::Identity;
	Matrix proj = Matrix::Identity;

	view = m_Camera->GetView();
	proj = m_Camera->GetProj();
	Matrix vp = view * proj;

	/*XMMatrixLookToLH(m_Camera->GetEyepos(), m_Camera->GetRefpos());
	Matrix view2 = Matrix::CreateLookAt(Vector3(0, 0, -2.0f), Vector3(0, 0, 0), Vector3::UnitY);
	Matrix proj2 = Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(60.0f), 960.0f / 640.0f, 0.1f, 1000.0f);

	if (MyLibrary::MouseUtil::GetInstance()->IsTriggered(MyLibrary::MouseUtil::Middle))
	{
		OutputDebugStringA("Visible\n");
	}
	if (MyLibrary::MouseUtil::GetInstance()->IsTriggered(MyLibrary::MouseUtil::Right))
	{
		OutputDebugStringA("Invisible\n");
	}

	if (MyLibrary::MouseUtil::GetInstance()->IsTriggered(MyLibrary::MouseUtil::Left))
	{
		OutputDebugStringA("Primitive Pos\n");
		for (unsigned int i = 0; i < m_Vertices.size(); i++)
		{
			Vector4 pos;
			Vector4 posWorld;
			Vector4 posView;
			Vector4 posProj1;
			Vector4 posProj2;
			pos.x = m_Vertices[i].position.x;
			pos.y = m_Vertices[i].position.y;
			pos.z = m_Vertices[i].position.z;
			pos.w = 1.0f;

			posWorld = Vector4::Transform(pos, world);
			posView = Vector4::Transform(posWorld, view);
			posProj1 = Vector4::Transform(posView, proj);

			posProj2 = Vector4::Transform(pos, wvp);

			char log_str[256];
			//sprintf_s(log_str, "VertexNum:%d  IndexNum:%d\n", landshape->m_Vertices.size(), landshape->m_Indices.size());
			sprintf_s(log_str, "(%.3f,%.3f,%.3f,%.3f)\n", posProj1.x, posProj1.y, posProj1.z, posProj1.w);
			OutputDebugStringA(log_str);

			static int a = 0;
			a++;
		}
	}*/

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(context->Map(m_pWVPConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		Constants constants;

		constants.VP = vp;
		constants.Billboard = m_Camera->GetBillboard();

		//ワールド、カメラ、射影行列を渡す
		memcpy_s(pData.pData, pData.RowPitch, &constants, sizeof(constants));
		context->Unmap(m_pWVPConstantBuffer.Get(), 0);
	}
	//このコンスタントバッファーをどのシェーダーで使うか
	context->VSSetConstantBuffers(0, 1, m_pWVPConstantBuffer.GetAddressOf());
	context->GSSetConstantBuffers(0, 1, m_pWVPConstantBuffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 0, nullptr);

	context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

	context->IASetInputLayout(m_InputLayout.Get());

	//テクスチャーをシェーダーに渡す
	context->PSSetSamplers(0, 1, m_Sampler.GetAddressOf());
	context->PSSetShaderResources(0, 1, m_Texture.GetAddressOf());

	context->RSSetState(m_CommonStates->CullClockwise());
	context->OMSetBlendState(m_CommonStates->Additive(), nullptr, 0xFFFFFFFF);

	m_PrimitiveBatch->Begin();
	m_PrimitiveBatch->Draw(D3D_PRIMITIVE_TOPOLOGY_POINTLIST, &m_Vertices[0], m_Vertices.size());
	m_PrimitiveBatch->End();
#endif
}
