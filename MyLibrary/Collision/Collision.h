//--------------------------------------------------------------------------------------
// �t�@�C����: Collision.h
// �쐬��: ���V�@�L��
// �쐬��: 2016/06/07
// ����:�����蔻�胉�C�u����
//      �w�ǂ̃A���S���Y����
//		�u�Q�[���v���O���~���O�̂��߂̃��A���^�C���Փ˔���vhttp://www.borndigital.co.jp/book/164.html�@������p
//--------------------------------------------------------------------------------------
#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>

namespace MyLibrary
{
	//----------------------------------------------
	//	define
	//----------------------------------------------


	//----------------------------------------------
	//	class
	//----------------------------------------------
	//// �~
	//class Circle
	//{
	//public:
	//	DirectX::SimpleMath::Vector3	center;	// ���S���W
	//	float	radius;	// ���a
	//	// �R���X�g���N�^
	//	Circle()
	//	{
	//		radius = 1.0f;
	//	}
	//};

	//// ����
	//class Line
	//{
	//public:
	//	DirectX::SimpleMath::Vector3	point0;	// �_0
	//	DirectX::SimpleMath::Vector3	point1;	// �_1
	//};

	// ����
	class Segment
	{
	public:
		DirectX::SimpleMath::Vector3	start;	// �n�_���W
		DirectX::SimpleMath::Vector3	end;	// �I�_���W
	};

	//// �����s���E�{�b�N�X(AABB)
	//class AABB
	//{
	//public:
	//	Point	Max;	// X,Y,Z���W���ꂼ��̍ŏ��l
	//	Point	Min;	// X,Y,Z���W���ꂼ��̍ő�l
	//};

	//// �L�����E�{�b�N�X(OBB)
	//class OBB
	//{
	//public:
	//	Point	center;		// ���S���W
	//	Vector	Axis[3];	// ���[�J��X,Y,Z��
	//	Vector	Size;		// �傫���iX,Y,Z���ꂼ��1�ӂ̒����̔����j
	//};

	// ��
	class Sphere
	{
	public:
		DirectX::SimpleMath::Vector3	center;	// ���S���W
		float	radius;	// ���a
		// �R���X�g���N�^
		Sphere()
		{
			radius = 1.0f;
		}
	};

	// �J�v�Z��(�����X�E�B�[�v�����`��j
	class Capsule
	{
	public:
		Segment	segment;	// �c��
		float	radius;	// ���a
		// �R���X�g���N�^
		Capsule()
		{
			segment.start = DirectX::SimpleMath::Vector3(0, 0, 0);
			segment.end = DirectX::SimpleMath::Vector3(0, 1, 0);
			radius = 1.0f;
		}
	};

	//// ����
	//class Plane
	//{
	//public:
	//	Vector	Normal;		// �@���x�N�g��
	//	float	Distance;	// ���_����̋����i���ʏ�̂���^����ꂽ�_p�ɑ΂���d = Dot(n,p)�������j
	//	// �R���X�g���N�^
	//	Plane()
	//	{
	//		Distance = 0.0f;
	//	}
	//};

	// �@���t���O�p�`�i�����v��肪�\�ʁj
	class Triangle
	{
	public:
		DirectX::SimpleMath::Vector3	P0;
		DirectX::SimpleMath::Vector3	P1;
		DirectX::SimpleMath::Vector3	P2;
		DirectX::SimpleMath::Vector3	Normal;	// �@���x�N�g��
	};

	//----------------------------------------------
	//	global functions prototype
	//----------------------------------------------

	// �ŏ��l�ƍő�l�̊ԂɃN�����v����
	inline float Clamp(float _x, float _min, float _max)
	{
		return min(max(_x, _min), _max);
	}
	// �x�N�g���̃X�J���[�O�d�ς��v�Z
	//float VectorTriple(const Vector &_v0, const Vector &_v1, const Vector &_v2);

	////-- ����
	//// �_�Ɛ����̋����̓����擾
	//float GetSqDistancePoint2Segment(const Point& _point, const Segment& _segment);
	//// �����Ɛ����̋����̓����擾
	//float GetSqDistanceSegment2Segment(const Segment& _segment0, const Segment& _segment1);

	//-- �Őڋߓ_�̎擾
	// �_�Ɛ���
	//void ClosestPtPoint2Segment(const Point& _point, const Segment& _segment, Point* _closest);
	//// �_�ƎO�p�`
	//void ClosestPtPoint2Triangle(const Point& _point, const Triangle& _triangle, Point* _closest);
	//// �����Ɛ����̍ŋߐړ_���v�Z
	//void ClosestPtSegment2Segment(const Segment& _segment0, const Segment& _segment1, Point* _closest0, Point* _closest1);
	//// �_��AABB
	//void ClosestPtPoint2AABB(const Point& _point, const AABB& _aabb, Point *_closest);
	//// �_��OBB
	//void ClosestPtPoint2OBB(const Point& _point, const OBB& _obb, Point *_closest);
	//
	//// �R�_����O�p�`���v�Z�i�����v���Ɏw��j
	void ComputeTriangle(const DirectX::SimpleMath::Vector3& _p0, const DirectX::SimpleMath::Vector3& _p1, const DirectX::SimpleMath::Vector3& _p2, Triangle* _triangle);
	//
	//// �����蔻��
	//bool CheckAABB2AABB(const AABB& _box0, const AABB& _box1, Point *_inter = nullptr);					// AABB��AABB
	//bool CheckOBB2OBB(const OBB& a, const OBB& b, Point *_inter = nullptr);								// OBB��OBB
	//bool CheckSphere2Sphere(const Sphere& _sphere0, const Sphere& _sphere1, Point *_inter = nullptr);	// ���Ƌ�
	//bool CheckSphere2Plane(const Sphere& _sphere, const Plane& _plane, Point *_inter = nullptr);		// ���ƕ���
	//bool CheckSphere2Capsule(const Sphere& _sphere, const Capsule& _capsule, Point *_inter = nullptr);	// ���ƃJ�v�Z��
	//bool CheckCapsule2Capsule(const Capsule& _capsule0, const Capsule& _capsule1, Point *_inter = nullptr);	// �J�v�Z���ƃJ�v�Z��
	//bool CheckLine2Triangle(const Line& _lay, const Triangle& _triangle, Point *_inter = nullptr);		// �����Ɩ@���t���O�p�`// �O�p�`�Ɠ_�̓����蔻��
	//bool CheckPoint2Triangle(const Point& _point, const Triangle& _triangle);	// �_���O�p�`�̒��ɂ��邩����
	bool CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, DirectX::SimpleMath::Vector3 *_inter = nullptr);	// �����Ɩ@���t���O�p�`
	bool CheckSphere2Triangle(const Sphere& _sphere, const Triangle& _triangle, DirectX::SimpleMath::Vector3 *_inter = nullptr); //���Ɩ@���t���O�p�`�̓�����`�F�b�N
	bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::SimpleMath::Vector3* _inter = nullptr);
	bool CheckCapsule2Capsule(const Capsule& _capsule0, const Capsule& _capsule1, DirectX::SimpleMath::Vector3* _inter = nullptr);
	bool CheckSphere2Capsule(const Sphere& _sphere, const Capsule& _capsule, DirectX::SimpleMath::Vector3* _inter = nullptr);
}