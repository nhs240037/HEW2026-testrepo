/*********************************************************************
 * \file   Player.cpp
 * \brief  プレイヤー Player
 * 
 * \author AT12C-41 Kotetsu Wakabayashi
 * \date   2025-11-11
 *********************************************************************/

//=====| Includes |=====//
#include "Player.h"
#include "Geometory.h"
#include "Input.h"
#include "Defines.h"

//=====| Enum Definition |=====//
enum eGolfBallShotStep
{
	SHOT_WAIT,		// 待機 Wait
	SHOT_KEEP,		// キー入力開始 Key pressed
	SHOT_RELEASE, // キー入力をやめた Key released
};

Player::Player() : m_pCamera(nullptr), m_isStop(false), m_shotStep(SHOT_WAIT), m_power(0.0f), m_move()
{
	m_collision.type = Collision::eBox;
	m_collision.box = {
		m_pos, DirectX::XMFLOAT3(1.f,1.f,1.f)
	};
};

/**
 * \brief 更新処理 Update process.
 * 
 */
void Player::Update()
{
	//===== カメラが設定されていない場合は処理しない Pass process unless exists camera. =====//
	if (!m_pCamera) {return;}

	//===== ボールが停止しているかどうかによって処理を変える Change process whether ball is stopped. =====//
	if(m_isStop)
		UpdateShot();
	else
	 UpdateMove();

	m_collision.box.center = m_pos;
}

/**
 * \brief 描画処理 Draw process.
 * 
 */
void Player::Draw()
{
	
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.f, 1.f, 1.f);
	DirectX::XMMATRIX mat = S * T;
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	Geometory::SetWorld(fMat);
	Geometory::DrawBox();
}

/**
 * \brief カメラの設定 Set camera.
 * 
 * \param camera カメラのポインタ Pointer to Camera.
 */
void Player::SetCamera(Camera *camera)
{
	m_pCamera = camera;
}

/**
 * \brief  球を打つ処理 Process of shot ball.
 * 
 */
void Player::UpdateShot()
{
	switch (m_shotStep) {
	//----- 球の打ち始め Begin shot ball. -----//
	case SHOT_WAIT:
	{
		if (IsKeyTrigger('Z'))
		{
			m_power = 0.0f;
			m_shotStep = SHOT_KEEP;
		}
		break;
	}
	
	//----- キー入力継続中 During press key. -----//
	case SHOT_KEEP:
	{
		m_power += 0.1f;
		if(IsKeyRelease('Z'))
		{
			m_shotStep = SHOT_RELEASE;
		}
		break;
	}

	//----- 球を打つ Shot ball. -----//
	case SHOT_RELEASE:
	{
		//--- 打ち出す計算 Calculate shot data.
		DirectX::XMFLOAT3 camPos = m_pCamera->GetPos();
		DirectX::XMVECTOR vCamPos	= DirectX::XMLoadFloat3(&camPos);
		DirectX::XMVECTOR vPos = DirectX::XMLoadFloat3(&m_pos);
		DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(vCamPos, vPos);
		vec = DirectX::XMVector3Normalize(vec);
		vec = DirectX::XMVectorScale(vec, -m_power);
		DirectX::XMStoreFloat3(&m_move, vec);

		//--- 打ち出し後の情報を設定 Set after shot data.
		m_isStop = false;
		m_shotStep = SHOT_WAIT;
		break;
	}
	}
}

/**
 * \brief 移動中の処理 Process of moving.
 */
void Player::UpdateMove()
{
	//===== 移動処理 Move process =====//
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	//===== 減速処理 Deceleration process =====//
	m_move.x *= .99f;
	m_move.y *= .99f;
	m_move.z *= .99f;

	//===== 重力 Gravity =====//
	m_move.y -= MSEC(GRAVITY);

	//===== 地面接触判定 Determine collision to ground =====//
	if (m_pos.y < 0.f)
	{
		//----- 接触時の減速処理 Decelerate on hit -----//
		m_move.x *= .95f;
		m_move.y *= .95f;
		m_move.z *= .95f;

		//----- バウンド処理 Bouncing process -----//
		m_move.y = -m_move.y; // Yの移動方向を反転 Inverse Y-axis moving direction
		if (m_move.y < METER(5.f))
		{ // バウンドが小さいか判定 If bounce is small
			m_move.y = 0.f;
			m_pos.y = 0.f;
		}
		else
		{
			//--- 地面にめり込んでいるので、バウンドした場合の位置に変更 Change position to bounced position due to penetrate ground.
			m_pos.y = -m_pos.y;
		}
	}

	//===== 停止判定 Determine stop =====//
	float speed;
	DirectX::XMVECTOR vMove = DirectX::XMLoadFloat3(&m_move);
	DirectX::XMVECTOR vLen = DirectX::XMVector3Length(vMove);
	DirectX::XMStoreFloat(&speed, vLen); // speed に vLen を格納
	if (speed < CMSEC(30.f))
	{	//--- 1秒間に30cmくらい進むスピードであれば停止
		m_isStop = true;
		m_shotStep = SHOT_WAIT;
	}
}


/**
 * \brief .
 * 
 * \param collision
 */
void Player::OnCollision(Collision::Result collision)
{
	// 計算に必要な情報を事前に計算
	DirectX::XMVECTOR vHitPos = DirectX::XMLoadFloat3(&collision.point);
	DirectX::XMVECTOR vNormal = DirectX::XMLoadFloat3(&collision.normal);
	DirectX::XMVECTOR vMove = DirectX::XMLoadFloat3(&m_move);
	vNormal = DirectX::XMVector3Normalize(vNormal);

	// 反射の計算
	DirectX::XMVECTOR vDot = DirectX::XMVector3Dot(vNormal, vMove);
	vDot = DirectX::XMVectorScale(vDot, 2.0f);
	vDot = DirectX::XMVectorMultiply(vNormal, DirectX::XMVectorAbs(vDot));
	vMove = DirectX::XMVectorAdd(vMove, vDot);
	DirectX::XMStoreFloat3(&m_move, vMove);

	// 反射後の補正
	if (collision.other.type == Collision::eBox) {
		// ボックスに衝突した場合、衝突位置の補正
		Collision::Box other = collision.other.box;
		if (collision.normal.x != 0.0f)
			m_pos.x =
			other.center.x + collision.normal.x * (other.size.x + 1.f) * 0.5f;
		else if (collision.normal.y != 0.0f)
			m_pos.y =
			other.center.y + collision.normal.y * (other.size.y + 1.f) * 0.5f;
		else
			m_pos.z =
			other.center.z + collision.normal.z * (other.size.z + 1.f) * 0.5f;
		// 反射後の移動速度の補正
		m_move.x *= 0.8f;
		m_move.y *= 0.6f;
		m_move.z *= 0.8f;
	}
	else {
		// 斜面に衝突した場合の位置の補正
		m_pos.x = collision.point.x + collision.normal.x * 1.f * 0.5f;
		m_pos.y = collision.point.y + collision.normal.y * 1.f * 0.5f;
		m_pos.z = collision.point.z + collision.normal.z * 1.f * 0.5f;
		// 反射後の移動速度の補正
		m_move.x *= 0.2f;
		m_move.y *= 0.5f;
		m_move.z *= 0.2f;
	}
}
