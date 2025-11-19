/*********************************************************************
 * \file   CameraDebug.cpp
 * \brief  
 * 
 * \author AT12C-41 Kotetsu Wakabayashi, 山本郁也
 * \date   2025-11-11
 *********************************************************************/
 //=====| Includes |=====//

#include "CameraDebug.h"
#include "Input.h"
//=====| /Includes |=====//
#include"Defines.h"


CameraDebug::CameraDebug()
	:m_radXZ(0.0f)
	,m_radY(0.5f * DEBUG_DISTANCE)
	,m_radius(10.0f * DEBUG_DISTANCE)
{
	m_radius = 115.f;
}

CameraDebug::~CameraDebug()
{
}

void CameraDebug::Update()
{
	// 注視点の移動　←(-x)、→(+x)、↑(+z)、↓(-z)、Shift(+y)、Ctrl(-y)
		//--- カメラ位置の移動(後述)

	static float count;

	//回り込みの処理
	if (IsKeyPress('E'))
	{
		m_radXZ += IIKANJINOTEISU + (IsKeyPress(VK_SHIFT) * IIKANJINOTEISU2);
		count += IIKANJINOTEISU;
	}
	if (IsKeyPress('Q'))
	{
		m_radXZ -= IIKANJINOTEISU - (IsKeyPress(VK_SHIFT) * IIKANJINOTEISU2);
		count -= IIKANJINOTEISU;
	}

	// カメラの距離を変える処理
	if (IsKeyPress(VK_DOWN))
	{
		m_radius += (IsKeyPress(VK_SHIFT) * IIKANJINOTEISU2);
		m_radY += IIKANJINOTEISU + (IsKeyPress(VK_CONTROL) * IIKANJINOTEISU * 10.0f);
	}
	if (IsKeyPress(VK_UP))
	{
		m_radius -= (IsKeyPress(VK_SHIFT) * IIKANJINOTEISU2);
		m_radY -= IIKANJINOTEISU + (IsKeyPress(VK_CONTROL) * IIKANJINOTEISU * 10.0f);
	}

	// カメラ位置の計算
	m_pos.x = m_radius * cosf(m_radXZ);
	m_pos.y = m_radius * sinf(m_radY);
	m_pos.z = m_radius * sinf(m_radXZ);

	m_pos.x = 0.0f;
	m_pos.y = m_radius;
	m_pos.z = -m_radius;
	//m_look.x = count;
}

void CameraDebug::SetPos(const DirectX::XMFLOAT3& pos)
{
	m_pos = pos;
}

