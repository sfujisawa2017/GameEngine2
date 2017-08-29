#include "ParticleEffect.h"
#include "..\Common\DeviceResources.h"
#include "..\Utility\BinaryFile.h"
#include <WICTextureLoader.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

// パーティクル最大数
const int ParticleEffectManager::PARTICLE_NUM_MAX = 10000;

const std::vector<D3D11_INPUT_ELEMENT_DESC> ParticleEffectManager::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

ParticleEffectManager * ParticleEffectManager::getInstance()
{
	static ParticleEffectManager instance;

	return &instance;
}

void ParticleEffectManager::Initialize()
{
	ID3D11Device* device = DeviceResources::GetInstance()->GetD3DDevice();
	ID3D11DeviceContext* context = DeviceResources::GetInstance()->GetD3DDeviceContext();

	// コンパイル済みシェーダーファイル読み込み
	// 頂点シェーダ
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	// ジオメトリシェーダ
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");
	// ピクセルシェーダ
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_VertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// プリミティブバッチ作成
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

	if (FAILED(device->CreateBuffer(&cb, NULL, m_pConstantBuffer.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateBuffer Failed.", NULL, MB_OK);
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

void ParticleEffectManager::Load(int texnumber, const wchar_t * filename)
{
	ID3D11Device* device = DeviceResources::GetInstance()->GetD3DDevice();
	
	ParticleEffectGroup group;

	// テクスチャ読み込み
	if (FAILED(CreateWICTextureFromFile(device, filename, nullptr,
		group.m_Texture.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"ParticleTest Failed.", NULL, MB_OK);
		return;
	}

	m_ParticleGroups[texnumber] = std::move(group);
}

void ParticleEffectManager::DisposeAll()
{
	m_ParticleGroups.clear();
}

void ParticleEffectManager::Dispose(int groupNumber)
{
	m_ParticleGroups.erase(groupNumber);
}

void ParticleEffectManager::Entry(
	int groupNumber,	// グループ番号
	int frame,	// フレーム数
	Vector3 position,	// 座標
	Vector3 velocity,	// 速度
	Vector3 accel,	// 加速度
	float start_rotation, float end_rotation, // 回転・開始角度と終了角度
	float start_scale, float end_scale, // 拡縮・開始倍率と終了倍率
	Color start_color, Color end_color	// 開始時と終了時の色
)
{
	// テクスチャ読み込み済みでないので、不正
	if (m_ParticleGroups.count(groupNumber) == 0)
	{
		return;
	}

	ParticleEffect effect;

	effect.position = position;
	effect.velocity = velocity;
	effect.accel = accel;
	effect.s_rotation = start_rotation;
	effect.e_rotation = end_rotation;
	effect.s_scale = start_scale;
	effect.e_scale = end_scale;
	effect.s_color = start_color;
	effect.e_color = end_color;
	effect.frame = 0;
	effect.num_frame = frame;

	// パーティクルを追加
	m_ParticleGroups[groupNumber].m_ParticleEffects.push_back(std::move(effect));

}

void ParticleEffectManager::Update()
{
	// 全てのパーティクルグループ更新
	for (std::pair<const int, ParticleEffectGroup>& pair : m_ParticleGroups)
	{		
		// 処理対象のグループ
		ParticleEffectGroup& group = pair.second;
		group.m_Vertices.clear();

		// 全てのパーティクルを更新
		std::list<ParticleEffect>& particleEffects = group.m_ParticleEffects;

		std::list<ParticleEffect>::iterator it;
		for (it = particleEffects.begin(); it != particleEffects.end(); )
		{
			// 更新
			if (it->Update(gravity))
			{
				VertexPositionColorTexture vertex;

				vertex.position = it->position;
				vertex.color = it->color;
				vertex.textureCoordinate.x = it->rotation;
				vertex.textureCoordinate.y = it->scale;

				group.m_Vertices.push_back(vertex);
				it++;
			}
			else
			{
				// 寿命に達したパーティクルを解放する
				it = particleEffects.erase(it);
			}
		}
	}
}

void ParticleEffectManager::Draw()
{
	ID3D11DeviceContext* context = DeviceResources::GetInstance()->GetD3DDeviceContext();
	CommonStates*	commonStates = DeviceResources::GetInstance()->GetCommonStates();

	// ビュー、プロジェクション行列を合成
	Matrix view = Matrix::Identity;
	Matrix proj = Matrix::Identity;

	view = m_Camera->GetView();
	proj = m_Camera->GetProj();
	Matrix vp = view * proj;

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(context->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		Constants constants;

		constants.VP = vp;
		constants.Billboard = m_Camera->GetBillboard();

		//データを上書き
		memcpy_s(pData.pData, pData.RowPitch, &constants, sizeof(constants));
		context->Unmap(m_pConstantBuffer.Get(), 0);
	}
	//このコンスタントバッファーをどのシェーダーで使うか
	context->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	context->GSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 0, nullptr);

	context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

	context->IASetInputLayout(m_InputLayout.Get());

	// 時計回りの面を陰面消去
	context->RSSetState(commonStates->CullClockwise());
	// 深度バッファを更新しない
	context->OMSetDepthStencilState(commonStates->DepthRead(), 0);
	// 加算描画
	context->OMSetBlendState(commonStates->Additive(), nullptr, 0xFFFFFFFF);
	// テクスチャサンプラー設定
	context->PSSetSamplers(0, 1, m_Sampler.GetAddressOf());

	// 全てのパーティクルグループについて
	for (std::pair<const int, ParticleEffectGroup>& pair : m_ParticleGroups)
	{
		// 処理対象のグループ
		ParticleEffectGroup& group = pair.second;

		if (group.m_Vertices.size() == 0) continue;

		//テクスチャーをシェーダーに渡す		
		context->PSSetShaderResources(0, 1, group.m_Texture.GetAddressOf());

		m_PrimitiveBatch->Begin();
		m_PrimitiveBatch->Draw(D3D_PRIMITIVE_TOPOLOGY_POINTLIST, &group.m_Vertices[0], group.m_Vertices.size());
		m_PrimitiveBatch->End();
	}

	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

ParticleEffectManager::ParticleEffectManager()
{
	m_Camera = nullptr;
}

bool ParticleEffectManager::ParticleEffect::Update(const Vector3& gravity)
{
	// 経過フレーム数をカウント
	frame++;
	if (frame > num_frame) return false;
	// 進行度を0～1の範囲に換算
	float f = (float)num_frame / frame;

	// 速度に加速度を加算
	velocity += accel;

	// 重力加速度による加速
	velocity += gravity;

	// 速度による移動
	position += velocity;

	// 角度の線形補完
	rotation = (e_rotation - s_rotation) / f;
	rotation += s_rotation;
	rotation = XMConvertToRadians(rotation);

	// スケールの線形補間
	scale = (e_scale - s_scale) / f;
	scale += s_scale;

	// 色の線形補間
	color = (e_color - s_color) / f;
	color += s_color;

	//obj.SetTrans(position);
	//obj.SetRot(rotation);
	//obj.SetScale(scale);
	//obj.Calc();

	return true;
}
