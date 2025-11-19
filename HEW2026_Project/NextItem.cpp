#include "NextItem.h"
#include <algorithm>
#include <random>
#include "Defines.h"
#include "Block.h"

NextItem::NextItem()
	:m_QueueItem{nullptr}, m_NextItem(nullptr), m_NextFrame(nullptr), m_QueueFrame{nullptr}
{
	m_NextItem = new UIObject("Placeholder.png", 1200.f, 180.f, 90.f, 90.f);
	m_NextFrame = new UIObject("Frame.png", 1200.f, 180.f, 95.f, 95.f);
	for (int i = 0; i < 5; i++)
	{
		m_QueueItem[i] = new UIObject("Placeholder.png", 1200.f, 300.f + 90.f * i, 80.f, 80.f);
		m_QueueFrame[i] = new UIObject("Frame.png", 1200.f, 300.f + 90.f * i, 85.f, 85.f);
	}
	
	AddLootTable();
}

NextItem::~NextItem()
{
	SAFE_DELETE_ARRAY(*m_QueueItem);
	SAFE_DELETE_ARRAY(*m_QueueFrame);
	SAFE_DELETE(m_NextItem);
	SAFE_DELETE(m_NextFrame);
}

void NextItem::Draw()
{
	m_NextFrame->Draw();
	m_NextItem->Draw();
	for (int i = 0; i < 5; i++)
	{
		m_QueueFrame[i]->Draw();
		m_QueueItem[i]->Draw();
	}
}

void NextItem::AddLootTable()
{
	std::list<Item> ItemList{ Item(1),Item(2),Item(3),Item(4), Item(5), Item(6), Item(7)};
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(ItemList.begin(), ItemList.end(), gen);
	m_Queue.insert(m_Queue.end(), ItemList.begin(), ItemList.end());
}

/**
 * \brief çﬁóøÉLÉÖÅ[êÊëóÇË.
 * 
 * \note 
 * 
 */
void NextItem::Next()
{
	//éüëóÇË
}