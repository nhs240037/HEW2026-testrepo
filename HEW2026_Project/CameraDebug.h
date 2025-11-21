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
	float m_radXZ;		// カメラの横回転
	float m_radY;		// カメラの縦回転
	float m_radius;	// カメラと注視点の距離

};

#endif//_CAMERADEBUG_H