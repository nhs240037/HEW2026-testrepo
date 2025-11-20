#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include<DirectXMath.h>
#include"Collision.h"

class GameObject {
public:
	GameObject();
	virtual ~GameObject();
	virtual void Update();
	virtual void Draw();
	DirectX::XMFLOAT3 GetPos();

	Collision::Info GetCollision();

	virtual void OnCollision(Collision::Result collision);
protected:
	DirectX::XMFLOAT3 m_pos;
	Collision::Info m_collision;// �����蔻��̏��
};

#endif//_GAMEOBJECT_H
