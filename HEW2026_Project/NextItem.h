#pragma once
#include <list>
#include "Sprite.h"
#include "Texture.h"
#include "UIObject.h"
#include "Block.h"

class Dummy
{
public:
	Dummy(int n) { m_nId = n; };
	void DummyFunction();
protected:
	int m_nId;
};

class Item : public Dummy
{
public:
	Item(int n) : Dummy(n) {};
};

class NextItem
{
public:
	NextItem();
	~NextItem();
	Block::Block_Color Next();
	void Draw();

public:
	enum Color
	{
		BUN_TOP,	//
		BUN_BOTTOM,	//
		LETTUCE,	//
		EGG,		//
		BACON,		//
		CHEESE,		//
		PATTY,		//
	};

private:
	void AddLootTable();
private:
	std::vector<Item> m_Queue;
	UIObject* m_NextItem;
	UIObject* m_NextFrame;
	UIObject* m_QueueItem[5];
	UIObject* m_QueueFrame[5];
	//UIObject* m_NextText;
	//UIObject* m_NextPan;
	//UIObject* m_NextArrow;
};

