/**
 * @file Camera.h
 * @author AT12C192-41 ��ьՓO
 * @brief �J�����̊�{�@�\
 * @date 2025-10-28
 *
 * @version 0.1
 */
#pragma once

//====| �C���N���[�h |====//
#include <DirectXMath.h>
//====| /�C���N���[�h |====//

//====| �萔�E�}�N����` |====//

//====| /�萔�E�}�N����` |====//

//====| �N���X�E�\���̒�` |====//
class Camera
{
public:
	// �R���X�g���N�^
  Camera();
  // �f�X�g���N�^
  virtual ~Camera() {}
  // �X�V���� (�p����ŕK������)
  virtual void Update() = 0;

	// �r���[�s��̎擾�i�f�t�H���g�ł͓]�u�ς݂̍s���v�Z����
  DirectX::XMFLOAT4X4 GetViewMatrix(bool transpose = true);

	// �v���W�F�N�V�����s��̎擾�i�f�t�H���g�ł͓]�u�ς݂̍s���v�Z����
  DirectX::XMFLOAT4X4 GetProjectionMatrix(bool transpose = true);

	// ���W�̎擾
  DirectX::XMFLOAT3 GetPos();
	// �����_�̎擾
  DirectX::XMFLOAT3 GetLook();

	virtual void SetPos(const DirectX::XMFLOAT3& pos) = 0;

  void SetLook(DirectX::XMFLOAT3 look);
protected:
  DirectX::XMFLOAT3 m_pos;  // ���W
  DirectX::XMFLOAT3 m_look; // �����_
  DirectX::XMFLOAT3 m_up;   // ����x�N�g��
  float m_fovy;             // ��p
  float m_aspect;           // �A�X�y�N�g��
  float m_near;             // �j�A�N���b�v
  float m_far;              // �t�@�[�N���b�v
};
