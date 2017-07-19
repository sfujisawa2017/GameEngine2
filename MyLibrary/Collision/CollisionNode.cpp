//--------------------------------------------------------------------------------------
// �t�@�C����: CollisionNode.cpp
// �쐬��:
// �쐬��:
// ����: �Փ˔���p�m�[�h
//--------------------------------------------------------------------------------------
#include "CollisionNode.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

// �f�o�b�O�\����ON/OFF�t���O�̎���
bool CollisionNode::s_DebugVisible = false;

void CollisionNode::SetParent(Obj3D* pParent)
{
	m_Obj.SetParent(pParent);
}

SphereNode::SphereNode()
{
	// �����l�i���a1m�j
	m_LocalRadius = 1.0f;
}

void SphereNode::Initialize()
{
	// �f�o�b�O�\���pCMO���f���̓ǂݍ���
	// ���f���͔��a1m�ō쐬���邱�ƁB
	m_Obj.LoadModel(L"SphereNode");
}

void SphereNode::Update()
{
	// �I�u�W�F�N�g�̍s����X�V
	{
		// �e�ƂȂ�I�u�W�F�N�g����̈ʒu
		m_Obj.SetTrans(m_Trans);
		Vector3 local_scale;
		// ���{�Ŕ��a1m�̋����f���Ȃ̂ŁA���a�����̂܂܃X�P�[�����O�ɂȂ�
		local_scale.x = m_LocalRadius;
		local_scale.y = m_LocalRadius;
		local_scale.z = m_LocalRadius;
		m_Obj.SetScale(local_scale);
		// �e�q�֌W���܂߂��s��v�Z
		m_Obj.Update();
	}

	// ���[���h���W�n�ł̓����蔻�苅���v�Z
	{
		// �v�Z�ς݃��[���h�s����擾
		Matrix worldm = m_Obj.GetLocalWorld();
		// ���f�����W�n�ł̒��S
		Vector3 center(0, 0, 0);
		// ���f�����W�n�ł̉E�[
		Vector3 right(1, 0, 0);

		// ���[���h���W�ɕϊ�
		center = Vector3::Transform(center, worldm);
		right = Vector3::Transform(right, worldm);

		// ���[���h���W�n�ł̒��S���W��ݒ�
		Sphere::center = center;
		// ���[���h���W�n�ł̔��a���v�Z
		Sphere::radius = Vector3::Distance(center, right);
	}
}

void SphereNode::Draw()
{
	// �f�o�b�O�\����ON�Ȃ�
	if (CollisionNode::GetDebugVisible())
	{
		m_Obj.Draw();
	}
}

//--------------------------------------------------------------
CapsuleNode::CapsuleNode()
{
	// �����l�i���a1m�j
	m_LocalRadius = 1.0f;
	// �����l�i������3m�j
	m_LocalLength = 1.0f;
}

void CapsuleNode::Initialize()
{
	// �f�o�b�O�\���p���f���̓ǂݍ���
	// ���f���͔��a1m�A������1m�̉~���ō쐬���邱�ƁB
	m_Obj.LoadModel(L"CapsuleNode");
}

void CapsuleNode::Update()
{
	// �I�u�W�F�N�g�̍s����X�V
	{
		// �e�ƂȂ�I�u�W�F�N�g����̈ʒu
		m_Obj.SetTrans(m_Trans);
		Vector3 local_scale;
		// ���{�Ŕ��a1m�̉~�����f���Ȃ̂ŁA���a�����̂܂܃X�P�[�����O�ɂȂ�
		local_scale.x = m_LocalRadius;
		local_scale.z = m_LocalRadius;
		// ���{�Ŏ�������1m�̉~�����f���Ȃ̂ŁA���������̂܂܃X�P�[�����O�ɂȂ�
		local_scale.y = m_LocalLength;
		m_Obj.SetScale(local_scale);
		// �e�q�֌W���܂߂��s��v�Z
		m_Obj.Update();
	}

	// ���[���h���W�n�ł̓����蔻��J�v�Z�����v�Z
	{
		// �v�Z�ς݃��[���h�s����擾
		Matrix worldm = m_Obj.GetLocalWorld();	
		// ���f�����W�n�ł̒��S
		Vector3 center(0, 0, 0);
		// ���f�����W�n�ł̏�[
		Vector3 up(0, 1, 0);
		// ���f�����W�n�ł̉E�[
		Vector3 right(1, 0, 0);

		// ���[���h���W�ɕϊ�
		center = Vector3::Transform(center, worldm);
		up = Vector3::Transform(up, worldm);
		right = Vector3::Transform(right, worldm);

		// ���[���h���W�n�ł̎������W��ݒ�
		Capsule::segment.start = center;
		Capsule::segment.end = up;
		// ���[���h���W�n�ł̔��a���v�Z
		Capsule::radius = Vector3::Distance(center, right);
	}
}

void CapsuleNode::Draw()
{
	// �f�o�b�O�\����ON�Ȃ�
	if (CollisionNode::GetDebugVisible())
	{
		m_Obj.Draw();
	}
}

void CapsuleNode::SetRot(const Vector3& rot)
{
	m_Obj.SetRot(rot);
}