/**
 * @file Camera.cpp
 * @author AT12C192-41 若林虎徹
 * @brief カメラの基本機能
 * @date 2025-10-28
 *
 * @version 0.1
 */
//====| Includes |====//
#include "Camera.h"
#include "Defines.h"
//====| /Includes |====//

//====| Constant, Macro definition |====//

//====| /Constant, Macro definition |====//

//====| Class, Structure definition |====//

//====| /Class, Structure definition |====//

//====| Function prototype definition |====//

//====| /Function prototype definition |====//

//====| Global Variable definition |====//

//====| /Global Variable definition |====//

/**
 * @brief Camera のコンストラクタ
 */
Camera::Camera()
    : m_pos(5.f, 5.f, 5.f), m_look(0.f, .5f, 0.f), m_up(0.f, 1.f, 0.f), m_fovy(80.f), m_aspect(16 / 9.f), m_near(CMETER(0.1f)), m_far(METER(1000.f))
{
}

/**
 * @brief ViewMatrixを取得
 *
 * @param transpose 転置
 * @return DirectX::XMFLOAT4X4
 */
DirectX::XMFLOAT4X4 Camera::GetViewMatrix(bool transpose)
{
  DirectX::XMFLOAT4X4 mat;
  DirectX::XMMATRIX view;
  view = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 0.f), DirectX::XMVectorSet(m_look.x, m_look.y, m_look.z, 0.f), DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 0.f));;
  if (transpose)
  {
    view = DirectX::XMMatrixTranspose(view);
  }
  DirectX::XMStoreFloat4x4(&mat, view);
  return mat;
}

/**
 * \brief プロジェクション行列の取得.
 * 
 * \param transpose 転置
 * \return DirectX::XMFLOAT4X4
 */
DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix(bool transpose)
{
  DirectX::XMFLOAT4X4 mat;
  DirectX::XMMATRIX proj;

  proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_fovy), m_aspect, m_near, m_far);
  if (transpose)
  {
    proj = DirectX::XMMatrixTranspose(proj);
  }
  DirectX::XMStoreFloat4x4(&mat, proj);
  return mat;
}

DirectX::XMFLOAT3 Camera::GetPos()
{
  return m_pos;
}

DirectX::XMFLOAT3 Camera::GetLook()
{
  return m_look;
}

void Camera::SetLook(DirectX::XMFLOAT3 look)
{
  m_look = look;
}
