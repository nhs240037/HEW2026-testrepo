/*********************************************************************
 * \file   CameraDebug.cpp
 * 概要：
 *
 * 制作者 山本郁也
 * \date　October 2025
 *********************************************************************/

#include "CameraDebug.h"
#include "Transfer.h"

CameraDebug::CameraDebug()
	:m_radXZ(0.0f)
	, m_radY(0.5f * DEBUG_DISTANCE)
	, m_radius(10.0f * DEBUG_DISTANCE)
	, m_radius_Z(-10.0f * DEBUG_DISTANCE)
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
	if (IsKeyPress(VK_LEFT))
	{
		m_radius_Z += (IsKeyPress(VK_SHIFT) * IIKANJINOTEISU2);
	}
	if (IsKeyPress(VK_RIGHT))
	{
		m_radius_Z -= (IsKeyPress(VK_SHIFT) * IIKANJINOTEISU2);
	}

	// カメラ位置の計算
	m_pos.x = m_radius * cosf(m_radXZ);
	m_pos.y = m_radius * sinf(m_radY);
	m_pos.z = m_radius * sinf(m_radXZ);

	TRAN_INS

	m_pos.x = tran.camera.eyePos.x;
	m_pos.y = tran.camera.eyePos.y;
	m_pos.z = tran.camera.eyePos.z;
	m_look.x = tran.camera.lookPos.x;
	m_look.y = tran.camera.lookPos.y;
	m_look.z = tran.camera.lookPos.z;
	//m_look.x = count;
}

void CameraDebug::SetPos(const DirectX::XMFLOAT3& pos)
{
	m_pos = pos;
}

