/*********************************************************************
 * \file   Collision.h
 * \brief  �Փ˔��� Collision
 *
 * \author AT12C-41 Kotetsu Wakabayashi
 * \date   2025-11-13
 *********************************************************************/
#pragma once
//=====| Includes |=====//
#include <DirectXMath.h>
#include <math.h>
#include <cmath>
#include <algorithm>

//=====| Class Definition |=====//
class Collision
{
public:
	//--- �����蔻��`��̎�� Type of collision shapes.
	enum Type
	{
		eNone,
		eBox,		 // �{�b�N�X
		eSphere, // ��
		ePlane,	 // ����
		eRay,		 // ���C
		eLine,	 // ��
		ePoint,
		eTriangle,
	};

	//--- �����蔻��̌`�� shape of collision.
	struct Box
	{
		DirectX::XMFLOAT3 center; // ���S���W
		DirectX::XMFLOAT3 size;		// �T�C�Y
	};
	struct Sphere
	{
		DirectX::XMFLOAT3 center; // ���S���W
		float radius;							// ���a
	};
	struct Plane
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
	};
	struct Ray
	{
		DirectX::XMFLOAT3 origin;
		DirectX::XMFLOAT3 dir;
	};
	struct Line
	{
		DirectX::XMFLOAT3 start;
		DirectX::XMFLOAT3 end;
	};
	struct Point
	{
		DirectX::XMFLOAT3 pos;
	};
	struct Triangle
	{
		DirectX::XMFLOAT3 point[3];
	};

	struct Info
	{
		Type type;
		union
		{
			Box box;
			Sphere sphere;
			Plane plane;
			Ray ray;
			Line line;
			Point point;
			Triangle triangle;
		};
	};

	struct Result
	{
		bool isHit;								// �����������ǂ���
		DirectX::XMFLOAT3 point;	// �q�b�g�ʒu
		DirectX::XMFLOAT3 normal; // �q�b�g����
		Info other;								// �����蔻��I�u�W�F�N�g
	};

public:
	// Info�^�ł̓����蔻��
	static Result Hit(Info a, Info b);

	// �l�p���m�̓����蔻��
	static Result Hit(Box a, Box b);
	static Result Hit(Sphere z, Sphere b);
	static Result Hit(Plane plane, Ray ray, float lenght);
	static Result Hit(Plane plane, Line line);
	static Result Hit(Point point, Triangle triangle);
};