#include "COrder.h"

COrder::COrder() :COrder({NULL})
{

}

COrder::COrder(std::vector<Item> Resipi)
{
	m_Orter = Resipi;
}

COrder::~COrder()
{
}

void COrder::apdeta()
{
}

void COrder::drwe()
{
}

void COrder::setprogrs(float ratio)
{
	m_twotime = ratio;
}
