#include "NextItem.h"
#include <algorithm>
#include <random>
#include "Defines.h"

using namespace std;

NextItem::NextItem()
	:m_QueueItem{nullptr}, m_NextItem(nullptr), m_NextFrame(nullptr), m_QueueFrame{nullptr}
{
	m_NextItem = new UIObject("Item_1.png", 1200.f, 180.f, 90.f, 90.f);
	m_NextFrame = new UIObject("Frame.png", 1200.f, 180.f, 95.f, 95.f);
	for (int i = 0; i < 5; i++)
	{
		m_QueueItem[i] = new UIObject("Item_3.png", 1200.f, 300.f + 90.f * i, 80.f, 80.f);
		m_QueueFrame[i] = new UIObject("Frame.png", 1200.f, 300.f + 90.f * i, 85.f, 85.f);
	}
	
	AddLootTable();
}

NextItem::~NextItem()
{
	SAFE_DELETE(m_NextItem);
	SAFE_DELETE(m_NextFrame);
	for (int i = 0; i < 5; i++)
	{
		SAFE_DELETE(m_QueueItem[i]);
		SAFE_DELETE(m_QueueFrame[i]);
	}
}

void NextItem::Draw()
{
	vector<Item*>::iterator it;
		
	it = m_Queue.begin();

	//--- Block::Block_colorの数値を取り出す (とんでもない処理)
	int TextureNum = int((*it)->GetColor());

	string TextureName = "Item_" + to_string(TextureNum) + ".png";
	m_NextItem->SetTexture(TextureName);		
		
	m_NextFrame->Draw();
	m_NextItem->Draw();
	for (int i = 0; i < 5; i++)
	{
			it++;
			TextureNum = int((*it)->GetColor());

			TextureName = "Item_" + to_string(TextureNum) + ".png";
			m_QueueItem[i]->SetTexture(TextureName);
		m_QueueFrame[i]->Draw();
		m_QueueItem[i]->Draw();
	}
}

void NextItem::AddLootTable()
{
	vector<Item*> ItemList;
	for (Block::Block_Color color :
	{
		//Block::Block_Color::Buns_Button,
			Block::Block_Color::Bacon,
			Block::Block_Color::Cheese,
			Block::Block_Color::Fried_egg,
			Block::Block_Color::Patty,
			Block::Block_Color::Lettuce,
			Block::Block_Color::Tomato,
	})
		ItemList.push_back(new Item(color));
	random_device rd;
	mt19937 gen(rd());
	shuffle(ItemList.begin(), ItemList.end(), gen);
	m_Queue.insert(m_Queue.end(), ItemList.begin(), ItemList.end());
}

/**
 * \brief 材料キュー先送り.
 * 
 * \note 
 * 
 */
Block::Block_Color NextItem::Next()
{
	vector<Item*>::iterator dropItemIt;
	Item* pDropItem;
	
	//--- キューの先頭を取り出し
	dropItemIt = m_Queue.begin();

	//--- 先頭のデータを保管
	pDropItem = (*dropItemIt);
	
	//--- 先頭を消し飛ばす
	m_Queue.erase(dropItemIt);
	
	//--- キューが表示分より少なくなったら追加抽選
	if (m_Queue.size() < 6)
		AddLootTable();

	return pDropItem->GetColor();
}