#pragma once
#include <list>
#include "Order.h"
#include "Score.h"

class OrderManager
{
public:
		OrderManager();
		~OrderManager();
		void Update();
		void Draw();
		void Check(std::list<Block *> submit, CScore* pScore);
		void Order(std::list<Block::Block_Color> Recipe);
		void Order(std::list<Block::Block_Color> Recipe, int price);
		void Order(std::list<Block::Block_Color> Recipe, int price, float limit);
private:
		std::list<COrder*> m_orderList;
};