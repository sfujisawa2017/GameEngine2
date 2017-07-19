﻿#include "Obj3D.h"
#include "VertexTypes.h"
#include <CommonStates.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

// 定数
// リソースディレクトリパス
const std::wstring Obj3D::RESOURCE_DIRECTORY = L"Resources/CMO/";
// ファイル拡張子
const std::wstring Obj3D::RESOURCE_EXT = L".cmo";
// 静的メンバ変数の実体
Obj3D::Common Obj3D::s_Common;

void Obj3D::InitializeCommon(CommonDef def)
{
	s_Common.device = def.pDevice;
	s_Common.deviceContext = def.pDeviceContext;
	s_Common.camera = def.pCamera;

	// エフェクトファクトリ生成
	s_Common.effectFactory = std::make_unique<EffectFactory>(def.pDevice);
	// テクスチャ読み込みパス指定
	s_Common.effectFactory->SetDirectory(RESOURCE_DIRECTORY.c_str());

	// 汎用ステート生成
	s_Common.states = std::make_unique<CommonStates>(def.pDevice);

	// 減算描画用のブレンドステートを作成
	D3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_REV_SUBTRACT;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT ret = def.pDevice->CreateBlendState(&desc, &s_Common.blendStateSubtract);
}

void Obj3D::UnloadAll()
{
	s_Common.modelarray.clear();
}

void Obj3D::SetSubtractive()
{
	// 減算描画を設定
	s_Common.deviceContext->OMSetBlendState(s_Common.blendStateSubtract, nullptr, 0xffffff);
}

/// <summary>
/// コンストラクタ
/// </summary>
Obj3D::Obj3D()
: m_pParent(nullptr)
, m_pModel(nullptr)
, m_UseQuternion(false)
{
	// スケールは1倍がデフォルト
	m_Scale = Vector3(1, 1, 1);
}

/// <summary>
/// ファイルからモデルを読み込む
/// </summary>
/// <param name="filename">CMOファイル名</param>
void Obj3D::LoadModel(const wchar_t*filename)
{
	assert(s_Common.effectFactory);

	// 指定ファイルを読み込み済みでないか？
	if (s_Common.modelarray.count(filename) == 0 )
	{
		// フルパスに補完
		std::wstring fullpath_bin = RESOURCE_DIRECTORY + filename + RESOURCE_EXT;
		// モデルを読み込み、コンテナに登録（キーはファイル名）
		s_Common.modelarray[filename] = Model::CreateFromCMO(s_Common.device, fullpath_bin.c_str(), *s_Common.effectFactory);
	}
	m_pModel = s_Common.modelarray[filename].get();
}

/**
*	@brief オブジェクトのライティングを無効にする
*/
void Obj3D::DisableLighting()
{
	if (m_pModel)
	{
		// モデル内の全メッシュ分回す
		ModelMesh::Collection::const_iterator it_mesh = m_pModel->meshes.begin();
		for (; it_mesh != m_pModel->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// メッシュ内の全メッシュパーツ分回す
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// メッシュパーツにセットされたエフェクトをBasicEffectとして取得
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff != nullptr)
				{
					// 自己発光を最大値に
					eff->SetEmissiveColor(Vector3(1,1,1));

					// エフェクトに含む全ての平行光源分について処理する
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						// ライトを無効にする
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
	}
}

void Obj3D::EnableAlpha()
{
	if (m_pModel)
	{
		// モデル内の全メッシュ分回す
		ModelMesh::Collection::const_iterator it_mesh = m_pModel->meshes.begin();
		for (; it_mesh != m_pModel->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// メッシュ内の全メッシュパーツ分回す
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// メッシュパーツにセットされたエフェクトをBasicEffectとして取得
				std::shared_ptr<IEffect>& ieff = meshpart->effect;
				meshpart->ModifyEffect(s_Common.device, ieff, true);
			}
		}
	}
}

void Obj3D::Update()
{
	Matrix scalem;
	Matrix rotm;
	Matrix transm;

	scalem = Matrix::CreateScale(m_Scale);

	if (m_UseQuternion)
	{
		rotm = Matrix::CreateFromQuaternion(m_RotQuat);
	}
	else
	{
		rotm = Matrix::CreateFromYawPitchRoll(m_Rot.y, m_Rot.x, m_Rot.z);
	}
	
	transm = Matrix::CreateTranslation(m_Trans);

	// ワールド行列をSRTの順に合成
	m_LocalWorld = Matrix::Identity;
	m_LocalWorld *= scalem;
	m_LocalWorld *= rotm;
	m_LocalWorld *= transm;

	// 親行列があれば
	if ( m_pParent )
	{
		// 親行列を掛ける
		m_LocalWorld = m_LocalWorld * m_pParent->GetLocalWorld();
	}
}

void Obj3D::Draw()
{
	if ( m_pModel )
	{
		assert(s_Common.camera);
		const Matrix& view = s_Common.camera->GetView();
		const Matrix& projection = s_Common.camera->GetProj();

		assert(s_Common.deviceContext);
		assert(s_Common.states);

		m_pModel->Draw(s_Common.deviceContext, *s_Common.states, m_LocalWorld, view, projection);
	}
}

void Obj3D::DrawSubtractive()
{
	if (m_pModel)
	{
		assert(s_Common.camera);
		const Matrix& view = s_Common.camera->GetView();
		const Matrix& projection = s_Common.camera->GetProj();

		assert(s_Common.deviceContext);
		assert(s_Common.states);

		// 減算描画用の設定関数を渡して描画
		m_pModel->Draw(s_Common.deviceContext, *s_Common.states, m_LocalWorld, view, projection, false, Obj3D::SetSubtractive);
	}
}

/// <summary>
///  ビルボード描画
/// </summary>
void Obj3D::DrawBillboard()
{
	if (m_pModel)
	{
		assert(s_Common.camera);
		const Matrix& view = s_Common.camera->GetView();
		const Matrix& projection = s_Common.camera->GetProj();

		// ビルボード行列をワールド行列に合成
		Matrix world = s_Common.camera->GetBillboard() * m_LocalWorld;

		assert(s_Common.deviceContext);
		assert(s_Common.states);

		// 減算描画用の設定関数を渡して描画
		m_pModel->Draw(s_Common.deviceContext, *s_Common.states, world, view, projection);
	}
}

/// <summary>
///  ビルボード描画
/// </summary>
void Obj3D::DrawBillboardConstrainY()
{
	if (m_pModel)
	{
		assert(s_Common.camera);
		const Matrix& view = s_Common.camera->GetView();
		const Matrix& projection = s_Common.camera->GetProj();

		// ビルボード行列をワールド行列に合成
		Matrix world = s_Common.camera->GetBillboardConstrainY() * m_LocalWorld;

		assert(s_Common.deviceContext);
		assert(s_Common.states);

		// 減算描画用の設定関数を渡して描画
		m_pModel->Draw(s_Common.deviceContext, *s_Common.states, world, view, projection);
	}
}