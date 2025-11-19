/*********************************************************************
 * \file   CameraDebug.h
 * \brief  
 * 
 * \author AT12C-41 Kotetsu Wakabayashi
 * \date   2025-11-11
 *********************************************************************/
#pragma once
 //=====| Includes |=====//
#include "Camera.h"
//=====| /Includes |=====//

//=====| ClassDefinition |=====//
class CameraDebug:
    public Camera
{
public:
  CameraDebug();
  ~CameraDebug();
  void Update() final;
private: 
  float m_radXZ;  // Rorizontal rotation
  float m_radY;   // Vertical rotation
  float m_radius; // Distance between camera and look
};