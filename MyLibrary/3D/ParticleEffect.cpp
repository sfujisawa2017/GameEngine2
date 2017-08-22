#include "ParticleEffect.h"
#include "..\Common\DeviceResources.h"
#include "..\Utility\BinaryFile.h"
#include <WICTextureLoader.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

// �p�[�e�B�N���ő吔
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

	// �R���p�C���ς݃V�F�[�_�[�t�@�C���ǂݍ���
	// ���_�V�F�[�_
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	// �W�I���g���V�F�[�_
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");
	// �s�N�Z���V�F�[�_
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_VertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// �v���~�e�B�u�o�b�`�쐬
	m_PrimitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(context, 1, PARTICLE_NUM_MAX);

	// ���_�t�H�[�}�b�g���w�肵�ē��̓��C�A�E�g�쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_InputLayout.GetAddressOf());

	//�V�F�[�_�ɋ��ʃf�[�^��n���ׂ�
	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(Constants);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(device->CreateBuffer(&cb, NULL, m_pConstantBuffer.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateBuffer Failed.", NULL, MB_OK);
		return;
	}

	// �e�N�X�`���T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	if (FAILED(device->CreateSamplerState(&SamDesc, m_Sampler.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateSamplerState Failed.", NULL, MB_OK);
		return;
	}
}

void ParticleEffectManager::Load(int texnumber, const wchar_t * filename)
{
	ID3D11Device* device = DeviceResources::GetInstance()->GetD3DDevice();
	
	ParticleEffectGroup group;

	// �e�N�X�`���ǂݍ���
	if (FAILED(CreateWICTextureFromFile(device, filename, nullptr,
		group.m_Texture.ReleaseAndGetAddressOf())))
	{// �G���[
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
	int groupNumber,	// �O���[�v�ԍ�
	int frame,	// �t���[����
	Vector3 position,	// ���W
	Vector3 velocity,	// ���x
	Vector3 accel,	// �����x
	float start_rotation, float end_rotation, // ��]�E�J�n�p�x�ƏI���p�x
	float start_scale, float end_scale, // �g�k�E�J�n�{���ƏI���{��
	Color start_color, Color end_color	// �J�n���ƏI�����̐F
)
{
	// �e�N�X�`���ǂݍ��ݍς݂łȂ��̂ŁA�s��
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

	// �p�[�e�B�N����ǉ�
	m_ParticleGroups[groupNumber].m_ParticleEffects.push_back(std::move(effect));

}

void ParticleEffectManager::Update()
{
	// �S�Ẵp�[�e�B�N���O���[�v�X�V
	for (std::pair<const int, ParticleEffectGroup>& pair : m_ParticleGroups)
	{		
		// �����Ώۂ̃O���[�v
		ParticleEffectGroup& group = pair.second;
		group.m_Vertices.clear();

		// �S�Ẵp�[�e�B�N�����X�V
		std::list<ParticleEffect>& particleEffects = group.m_ParticleEffects;

		std::list<ParticleEffect>::iterator it;
		for (it = particleEffects.begin(); it != particleEffects.end(); )
		{
			// �X�V
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
				// �����ɒB�����p�[�e�B�N�����������
				it = particleEffects.erase(it);
			}
		}
	}
}

void ParticleEffectManager::Draw()
{
	ID3D11DeviceContext* context = DeviceResources::GetInstance()->GetD3DDeviceContext();
	CommonStates*	commonStates = DeviceResources::GetInstance()->GetCommonStates();

	// �r���[�A�v���W�F�N�V�����s�������
	Matrix view = Matrix::Identity;
	Matrix proj = Matrix::Identity;

	view = m_Camera->GetView();
	proj = m_Camera->GetProj();
	Matrix vp = view * proj;

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(context->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		Constants constants;

		constants.VP = vp;
		constants.Billboard = m_Camera->GetBillboard();

		//�f�[�^���㏑��
		memcpy_s(pData.pData, pData.RowPitch, &constants, sizeof(constants));
		context->Unmap(m_pConstantBuffer.Get(), 0);
	}
	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	context->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	context->GSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 0, nullptr);

	context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

	context->IASetInputLayout(m_InputLayout.Get());

	// ���v���̖ʂ��A�ʏ���
	context->RSSetState(commonStates->CullClockwise());
	// �[�x�o�b�t�@���X�V���Ȃ�
	context->OMSetDepthStencilState(commonStates->DepthRead(), 0);
	// ���Z�`��
	context->OMSetBlendState(commonStates->Additive(), nullptr, 0xFFFFFFFF);
	// �e�N�X�`���T���v���[�ݒ�
	context->PSSetSamplers(0, 1, m_Sampler.GetAddressOf());

	// �S�Ẵp�[�e�B�N���O���[�v�ɂ���
	for (std::pair<const int, ParticleEffectGroup>& pair : m_ParticleGroups)
	{
		// �����Ώۂ̃O���[�v
		ParticleEffectGroup& group = pair.second;

		if (group.m_Vertices.size() == 0) continue;

		//�e�N�X�`���[���V�F�[�_�[�ɓn��		
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
	// �o�߃t���[�������J�E���g
	frame++;
	if (frame > num_frame) return false;
	// �i�s�x��0�`1�͈̔͂Ɋ��Z
	float f = (float)num_frame / frame;

	// ���x�ɉ����x�����Z
	velocity += accel;

	// �d�͉����x�ɂ�����
	velocity += gravity;

	// ���x�ɂ��ړ�
	position += velocity;

	// �p�x�̐��`�⊮
	rotation = (e_rotation - s_rotation) / f;
	rotation += s_rotation;
	rotation = XMConvertToRadians(rotation);

	// �X�P�[���̐��`���
	scale = (e_scale - s_scale) / f;
	scale += s_scale;

	// �F�̐��`���
	color = (e_color - s_color) / f;
	color += s_color;

	//obj.SetTrans(position);
	//obj.SetRot(rotation);
	//obj.SetScale(scale);
	//obj.Calc();

	return true;
}
