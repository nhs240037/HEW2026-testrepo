#include "OrderManager.h"
using namespace std;

OrderManager::OrderManager()
{
	m_orderList.push_back(new COrder({Block::Patty, Block::Lettuce}, 20.f));
	m_orderList.push_back(new COrder({Block::Patty, Block::Lettuce, Block::Fried_egg}, 40.f));
	m_orderList.push_back(new COrder({Block::Patty, Block::Lettuce}, 30.f));
}

OrderManager::~OrderManager()
{
}

void OrderManager::Update()
{
	int index = 0;
	for (auto it = m_orderList.begin(); it != m_orderList.end(); ++it, ++index)
	{
		(*it)->SetOrderIndex(index);
		(*it)->Update();
	}
	for (auto it = m_orderList.begin(); it != m_orderList.end();)
	{
		if ((*it)->GetIsFailure())
			it = m_orderList.erase(it);
		else
			++it;
	}
}

void OrderManager::Draw()
{
	for (list<COrder *>::iterator it = m_orderList.begin(); it != m_orderList.end(); ++it)
		(*it)->Draw();
}

void OrderManager::Check(std::list<Block *> submit)
{
		//submit.sort()
		//OutputDebugString()
	// 受け取ったデータを元に納品できるか、
	// 納品可能ならオーダー削除、金額獲得。
}
