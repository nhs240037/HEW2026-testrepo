/*********************************************************************
 * \file   GameObject.h
 * \brief  
 * 
 * \author AT12C-41 Kotetsu Wakabayashi
 * \date   2025-11-11
 *********************************************************************/
#pragma once
//=====| Includes |=====//
#include <DirectXMath.h>
#include "Collision.h"

//=====| Class Definition |=====//
class GameObject
{
public:
	GameObject();
	virtual ~GameObject() {}
	virtual void Update() {}
	virtual void Draw() {}
	DirectX::XMFLOAT3 GetPos() {return m_pos;}
	Collision::Info GetCollision(){return m_collision;}
	virtual void OnCollision(Collision::Result collision){}
protected:
	DirectX::XMFLOAT3 m_pos;
	Collision::Info m_collision;
};