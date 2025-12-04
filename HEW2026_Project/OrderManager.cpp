#include "OrderManager.h"
#include"Sound.h"
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

void OrderManager::Order(list<Block::Block_Color> Recipe)
{
	Order(Recipe, 10);
}

void OrderManager::Order(list<Block::Block_Color> Recipe, int price)
{
	Order(Recipe, price, 30.f);
}

void OrderManager::Order(list<Block::Block_Color> Recipe, int price, float limit)
{
		OutputDebugString("Trying to Order\n");
	if (m_orderList.size() < 3)
	{
		m_orderList.push_back(new COrder(Recipe, limit, price));
		OutputDebugString("Ordered!\n");
	}
}

void OrderManager::Draw()
{
	for (list<COrder *>::iterator it = m_orderList.begin(); it != m_orderList.end(); ++it)
		(*it)->Draw();
}

void OrderManager::Check(list<Block *> submit, CScore* pScore)
{
	submit.erase(submit.begin());
	if (submit.size() == 0)
	{
		OutputDebugString("Empty burger was submitted\n");
		return;
	}
	submit.sort([](Block* a, Block* b)
		{
			return (int)(a->GetColor()) < (int)(b->GetColor());
		});
	list<Block::Block_Color> BurgerColor;
	string _dbgstr = "";
	for (auto block : submit)
	{
		_dbgstr += to_string(block->GetColor());
			BurgerColor.push_back(block->GetColor());
	}
			OutputDebugString(("Submitted: " + _dbgstr + "\n").c_str());

	list<COrder*> CorrectOrder;
	for (auto order : m_orderList)
	{
		if (order->IsCorrect(BurgerColor))
			CorrectOrder.push_back(order);
	}
	
	// 注文と一致しない場合
	if (CorrectOrder.size() == 0)
	{
		OutputDebugString("Same order is not found.\n");
		return;
	}

	CorrectOrder.sort([](COrder* a, COrder* b)
	{
		return a->GetProgress() < b->GetProgress();
	});

	auto TargetOrder = find(m_orderList.begin(), m_orderList.end(), *CorrectOrder.begin());

	if (TargetOrder == m_orderList.end())
	{
		OutputDebugString("おこっちゃいけないことがおきた\n");
		return;
	}

	pScore->AddScore((*TargetOrder)->SellScore());
	m_orderList.erase(TargetOrder);

	// 成功音声再生
	SE_INS_So.PlaySE(0);
	

		//submit.sort()
		//OutputDebugString()
	// 受け取ったデータを元に納品できるか、
	// 納品可能ならオーダー削除、金額獲得。
}
