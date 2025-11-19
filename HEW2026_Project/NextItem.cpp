#include "NextItem.h"
#include <algorithm>
#include <random>
#include "Defines.h"

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
	std::vector<Item> arr{ Item(0),Item(1),Item(2),Item(3),Item(4)};
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(arr.begin(), arr.end(), gen);
	m_Queue.insert(m_Queue.end(), arr.begin(), arr.end());
}

void NextItem::Next()
{
	//ŽŸ‘—‚è
}