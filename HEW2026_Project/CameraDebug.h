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
	float m_radXZ;		// ƒJƒƒ‰‚Ì‰¡‰ñ“]
	float m_radY;		// ƒJƒƒ‰‚Ìc‰ñ“]
	float m_radius;	// ƒJƒƒ‰‚Æ’‹“_‚Ì‹——£

};

#endif//_CAMERADEBUG_H