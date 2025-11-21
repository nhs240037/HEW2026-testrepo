#include "Order.h"

COrder::COrder() :COrder({NULL})
{

}

COrder::COrder(std::vector<Block> Recipe)
{
	m_Order = Recipe;
}

COrder::~COrder()
{
}

void COrder::Update()
{
}

void COrder::Draw()
{
}

void COrder::SetProgress(float ratio)
{
	m_limit = ratio;
}
