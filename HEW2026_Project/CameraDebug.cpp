/*********************************************************************
 * \file   CameraDebug.cpp
 * \brief  
 * 
 * \author AT12C-41 Kotetsu Wakabayashi
 * \date   2025-11-11
 *********************************************************************/
 //=====| Includes |=====//
#include "CameraDebug.h"
#include "Input.h"
//=====| /Includes |=====//

CameraDebug::CameraDebug()
:m_radXZ(0.f), m_radY(0.f), m_radius(10.f)
{
}

CameraDebug::~CameraDebug()
{}

void CameraDebug::Update()
{
	//===== 注視点の移動 =====//
	float camSpeed = .2f;
	float camrot = .12f;
	float camzoom = 1.f;
	if(IsKeyPress(VK_UP))
		m_look.z += camSpeed;
	if(IsKeyPress(VK_DOWN))
		m_look.z -= camSpeed;
	if(IsKeyPress(VK_RIGHT))
		m_look.x += camSpeed;
	if(IsKeyPress(VK_LEFT))
		m_look.x -= camSpeed;
	if(IsKeyPress(VK_LSHIFT))
		m_look.y += camSpeed;
	if(IsKeyPress(VK_LCONTROL))
		m_look.y -= camSpeed;

	//===== カメラ位置の移動 =====//
	//----- 回り込みの処理 -----//
	if(IsKeyPress('A')) 
		m_radXZ += camrot;
	if(IsKeyPress('D'))
		m_radXZ -= camrot;
	if(IsKeyPress('W'))
		m_radY += camrot;
	if(IsKeyPress('S'))
		m_radY -= camrot;
	//----- カメラの距離を変える処理 -----//
	if (IsKeyPress('E'))
		m_radius += camzoom;
	if (IsKeyPress('Q')&&m_radius > 1.f)
		m_radius -= camzoom;

	//===== カメラ位置の計算 =====//
	m_pos.x = cosf(m_radY) * sinf(m_radXZ) * m_radius + m_look.x;
	m_pos.y = sinf(m_radY) * m_radius + m_look.y;
	m_pos.z = cosf(m_radY) * cosf(m_radXZ) * m_radius + m_look.z;
}
