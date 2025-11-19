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
	// コンストラクタ
  Camera();
  // デストラクタ
  virtual ~Camera() {}
  // 更新処理 (継承先で必ず実装)
  virtual void Update() = 0;

	// ビュー行列の取得（デフォルトでは転置済みの行列を計算する
  DirectX::XMFLOAT4X4 GetViewMatrix(bool transpose = true);

	// プロジェクション行列の取得（デフォルトでは転置済みの行列を計算する
  DirectX::XMFLOAT4X4 GetProjectionMatrix(bool transpose = true);

	// 座標の取得
  DirectX::XMFLOAT3 GetPos();
	// 注視点の取得
  DirectX::XMFLOAT3 GetLook();

	virtual void SetPos(const DirectX::XMFLOAT3& pos) = 0;

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