/*********************************************************************
 * \file   Collision.h
 * \brief  衝突判定 Collision
 * 
 * \author AT12C-41 Kotetsu Wakabayashi
 * \date   2025-11-13
 *********************************************************************/
#pragma once
 //=====| Includes |=====//
#include <DirectXMath.h>
//=====| Class Definition |=====//
class Collision
{
public:
	//--- 当たり判定形状の種類 Type of collision shapes.
	enum Type
	{
		eNone,
		eBox,			// ボックス
		eSphere,	// 球
		ePlane,		// 平面
		eRay,			// レイ
		eLine,		// 線
		ePoint,
		eTriangle,
	};

	//--- 当たり判定の形状 shape of collision.
	struct Box
	{
		DirectX::XMFLOAT3 center;		// 中心座標
		DirectX::XMFLOAT3 size;			// サイズ
	};
	struct Sphere
	{
		DirectX::XMFLOAT3 center;		// 中心座標
		float radius;								// 半径
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
		bool isHit;								// 当たったかどうか
		DirectX::XMFLOAT3 point;	// ヒット位置
		DirectX::XMFLOAT3 normal;	// ヒット平面
		Info other;								// 当たり判定オブジェクト
	};

public:
	static Result Hit(Info a, Info b);
	static Result Hit(Box a, Box b);
	static Result Hit(Sphere z, Sphere b);
	static Result Hit(Plane plane, Ray ray, float lenght);
	static Result Hit(Plane plane, Line line);
	static Result Hit(Point point, Triangle triangle);
};