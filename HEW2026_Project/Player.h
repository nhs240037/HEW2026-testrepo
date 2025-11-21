#ifndef _PLAYER_H
#define _PLAYER_H

#include"GameObject.h"
#include"Camera.h"
#include"CameraDebug.h"
#include"Geometory.h"
#include"Defines.h"
#include<DirectXMath.h>
#include"Sprite.h"
#include"Model.h"

class Player : public GameObject {
public:
	Player();
	~Player();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// カメラの設定
	void SetCamera(Camera* camera);

	void OnCollision(Collision::Result collision)override;

	void SetShadow(DirectX::XMFLOAT3 pos);
public:
	struct Index
	{
		int x;
		int z;
	};
	struct float2
	{
		float x;
		float y;
	};
private:
	// 球を打つ処理
	void UpdateShot();

	void UpdateControl();

	// 移動中の処理
	void UpdateMove();

	void UpdateWall();

private:
	Camera* m_pCamera;	//　カメラ情報
	DirectX::XMFLOAT3	m_move;		// 移動量
	bool		m_isStop;	// ボールの停止判定
	int		m_shotStep; // 
	float	m_power;		// speedにvLenを格納
	float2 m_f2pos;
	Index m_idx;
	float m_angle;

	Texture* m_pShadowTex;	// 影の見た目
	DirectX::XMFLOAT3	m_shadowPos;	// 影の位置

	// New!
	CsvData& csv;
	Model* m_pModel;
	DirectX::XMMATRIX m_dxpos;
	DirectX::XMFLOAT4X4 wvp[3];
};

#endif//_PLAYER_H