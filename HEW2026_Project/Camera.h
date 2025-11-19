/**
 * @file Camera.h
 * @author AT12C192-41 若林虎徹
 * @brief カメラの基本機能
 * @date 2025-10-28
 *
 * @version 0.1
 */
#pragma once

//====| インクルード |====//
#include <DirectXMath.h>
//====| /インクルード |====//

//====| 定数・マクロ定義 |====//

//====| /定数・マクロ定義 |====//

//====| クラス・構造体定義 |====//
class Camera
{
public:
  Camera();
  virtual ~Camera() {}
  virtual void Update() = 0;
  DirectX::XMFLOAT4X4 GetViewMatrix(bool transpose = true);
  DirectX::XMFLOAT4X4 GetProjectionMatrix(bool transpose = true);
  DirectX::XMFLOAT3 GetPos();
  DirectX::XMFLOAT3 GetLook();
  void SetLook(DirectX::XMFLOAT3 look);
protected:
  DirectX::XMFLOAT3 m_pos;  // 座標
  DirectX::XMFLOAT3 m_look; // 注視点
  DirectX::XMFLOAT3 m_up;   // 上方ベクトル
  float m_fovy;             // 画角
  float m_aspect;           // アスペクト比
  float m_near;             // ニアクリップ
  float m_far;              // ファークリップ
};

//====| /クラス・構造体定義 |====//

//====| プロトタイプ宣言 |====//

//====| /プロトタイプ宣言 |====//

//====| グローバル宣言 |====//

//====| /グローバル宣言 |====//
