#pragma once
#include <vector>
#include "Sprite.h"
#include "Texture.h"

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
	Texture* m_pFrameTex;
	Texture* m_pItemIconTex[5];
};

