#pragma once
#include <vector>
#include <list>
#include "Block.h"
#include "Timer.h"
#include "UIObject.h"


class OrderTimer : public CTimer
{
public:
	enum State
	{
		Safe,
		Warn,
		Danger
	};
public: 
		OrderTimer();
		OrderTimer(float Limit);
		OrderTimer(float Limit, float WarnLine, float DangerLine);
		~OrderTimer();
		void Update() override;
		void Draw() override;
		State GetState();

		float GetProgressRatio();
		void SetPosition(float x, float y);
private:
		float m_fWarnRatio = 0.65f;
		float m_fDangerRatio = 0.25f;
		UIObject* m_pUIOrderTimerFill = nullptr;
		UIObject* m_pUIOrderTimerPlaceholder = nullptr;
		UIObject* m_pUILineWarn = nullptr;
		UIObject* m_pUILineDanger = nullptr;
		DirectX::XMFLOAT2 m_pos = {0.0f, 0.0f};
};

class COrder
{
public:
	COrder();
	COrder(std::list<Block::Block_Color> Recipe);
	COrder(std::list<Block::Block_Color> Recipe, float limit);
	COrder(std::list<Block::Block_Color> Recipe, float limit, int price);
	~COrder();
	void Update();
	void Draw();
	void SetOrderIndex(int num);
	int SellScore();
	float GetProgress();
	bool GetIsFailure();
	bool IsCorrect(std::list<Block::Block_Color> burger);

private:
	std::list<Block::Block_Color> m_Order;
	int m_nBaseScore = 10;
	int m_nOrderIndex = 0;
	OrderTimer* m_pOrderTimer = nullptr;
	UIObject* m_pUIOrderBubble = nullptr;
	UIObject* m_pUIOrderBurger = nullptr;
	DirectX::XMFLOAT2 m_pos;
	bool m_bFailure = false; 
};