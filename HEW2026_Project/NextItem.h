#pragma once
#include <vector>
#include "Sprite.h"
#include "Texture.h"
#include "UIObject.h"

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
	void Next();
	void Draw();
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

