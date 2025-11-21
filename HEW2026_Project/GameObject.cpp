#include "GameObject.h"

GameObject::GameObject() 
	: m_pos(0.0f, 0.0f, 0.0f) 
{

}

GameObject::~GameObject()
{

}

void GameObject::Update()
{

}

DirectX::XMFLOAT3 GameObject::GetPos()
{
	 return m_pos; 
}

void GameObject::Draw()
{

}


Collision::Info GameObject::GetCollision()
{
	return m_collision;
}

void GameObject::OnCollision(Collision::Result collision)
{
}
