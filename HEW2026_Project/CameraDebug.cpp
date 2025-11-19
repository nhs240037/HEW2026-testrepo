/*********************************************************************
 * \file   CameraDebug.cpp
 * \brief  
 * 
 * \author AT12C-41 Kotetsu Wakabayashi, �R�{���
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
	// �����_�̈ړ��@��(-x)�A��(+x)�A��(+z)�A��(-z)�AShift(+y)�ACtrl(-y)
		//--- �J�����ʒu�̈ړ�(��q)

	static float count;

	//��荞�݂̏���
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

	// �J�����̋�����ς��鏈��
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

	// �J�����ʒu�̌v�Z
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

