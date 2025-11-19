#ifndef _CAMERADEBUG_H
#define _CAMERADEBUG_H

#include "Camera.h"
#include "Input.h"
class CameraDebug : public Camera 
{
public:
  CameraDebug();
  ~CameraDebug();
  void Update() final;
	void SetPos(const DirectX::XMFLOAT3& pos)override;
private: 
	float m_radXZ;		// �J�����̉���]
	float m_radY;		// �J�����̏c��]
	float m_radius;	// �J�����ƒ����_�̋���

};

#endif//_CAMERADEBUG_H
