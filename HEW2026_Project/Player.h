/*********************************************************************
 * \file   Player.h
 * \brief  プレイヤー Player
 * 
 * \author AT12C-41 Kotetsu Wakabayashi
 * \date   2025-11-11
 *********************************************************************/
#pragma once
//=====| Includes |=====//
#include "GameObject.h"
#include "Camera.h"

//=====| Class Definition |=====//
class Player : public GameObject
{
public:
	Player();
	~Player() {};

	void Update();
	void Draw();
	void SetCamera(Camera* camera);
	void OnCollision(Collision::Result collision);

private:
	void UpdateShot();
	void UpdateMove();

private: 
	Camera* m_pCamera;				// カメラ情報 Camera data
	DirectX::XMFLOAT3 m_move;	// 移動量 Displacement
	bool m_isStop;						// ボールの停止判定 Determine ball stop
	int m_shotStep;						// 状態
	float m_power;						// 打つ力
};
