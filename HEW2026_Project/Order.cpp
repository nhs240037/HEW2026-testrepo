#include "Order.h"
#include "Defines.h"
#include <iostream>
#include <string>

OrderTimer::OrderTimer() : OrderTimer(20.0f)
{
}

OrderTimer::OrderTimer(float Limit): OrderTimer(Limit, 0.65f, 0.25f) 
{
}

OrderTimer::OrderTimer(float Limit, float WarnLine, float DangerLine)
{
		m_limitTime = (double)Limit;
		m_fWarnRatio = WarnLine;
		m_fDangerRatio = DangerLine;
		CTimer::InitializeTimer();
		double _scale = 0.55;
		m_pUIOrderTimerFill = new UIObject("LimitGauge.png");
		m_pUIOrderTimerPlaceholder = new UIObject("LimitGauge.png", 0.f, 0.f, 64.f * _scale, 256.f * _scale);
		m_pUILineWarn = new UIObject("LimitLine.png", 0.f, 0.f, 70.f * _scale, 64.f * _scale);
		m_pUILineDanger = new UIObject("LimitLine.png", 0.f, 0.f, 70.f * _scale, 64.f * _scale);

		m_pUILineWarn->SetColor(1.0f, 1.0f, 0.0f, 1.0f);
		m_pUILineDanger->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
		m_pUIOrderTimerPlaceholder->SetColor(0.91f, 0.75f, 0.62f, 1.0f);
		m_pUIOrderTimerFill->SetColor(0.22f, 0.87f, 0.88f, 1.f);



		m_pUILineWarn->SetPosition(
						m_pUIOrderTimerPlaceholder->GetPosition().x,
						m_pUIOrderTimerPlaceholder->GetPosition().y - m_pUIOrderTimerPlaceholder->GetSize().y * .5f + m_pUIOrderTimerPlaceholder->GetSize().y * m_fWarnRatio
				);
		
		m_pUILineDanger->SetPosition(
						m_pUIOrderTimerPlaceholder->GetPosition().x,
						m_pUIOrderTimerPlaceholder->GetPosition().y - m_pUIOrderTimerPlaceholder->GetSize().y * .5f + m_pUIOrderTimerPlaceholder->GetSize().y * m_fDangerRatio
		);

		m_pUIOrderTimerFill->SetSize(m_pUIOrderTimerPlaceholder->GetSize());
		m_pUIOrderTimerFill->SetPosition(m_pUIOrderTimerPlaceholder->GetPosition());
}

OrderTimer::~OrderTimer()
{
		SAFE_DELETE(m_pUIOrderTimerFill);
		SAFE_DELETE(m_pUIOrderTimerPlaceholder);
		SAFE_DELETE(m_pUILineWarn);
		SAFE_DELETE(m_pUILineDanger);
}

void OrderTimer::SetPosition(float x, float y)
{
		m_pos = { x, y };
		m_pUIOrderTimerPlaceholder->SetPosition(x, y);
}

void OrderTimer::Update()
{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);

		LONGLONG elapsedTicks = currentTime.QuadPart - m_lastTime.QuadPart;
		double deltaTime = (double)elapsedTicks / m_cpuFrequency.QuadPart;

		m_totalTime -= deltaTime;
		
		if (m_totalTime < 0.0)
		{
				m_totalTime = 0.0;
		}

		m_lastTime = currentTime;

		float progressRatio = GetProgressRatio();
		if (progressRatio < m_fWarnRatio && progressRatio > m_fDangerRatio)
				m_pUIOrderTimerFill->SetColor(1.f, 0.65f, 0.31f, 1.f);
		else if (progressRatio < m_fDangerRatio)
				m_pUIOrderTimerFill->SetColor(0.82f, 0.28f, 0.12f, 1.f);
		m_pUIOrderTimerFill->SetUVScale(1.0f, progressRatio);
		m_pUIOrderTimerFill->SetUVPosition(0.0f, 1.0f - progressRatio);
		m_pUIOrderTimerFill->SetSize(m_pUIOrderTimerPlaceholder->GetSize().x, m_pUIOrderTimerPlaceholder->GetSize().y * progressRatio);
		m_pUIOrderTimerFill->SetPosition(m_pUIOrderTimerPlaceholder->GetPosition().x, m_pUIOrderTimerPlaceholder->GetPosition().y + m_pUIOrderTimerPlaceholder->GetSize().y * .5f - m_pUIOrderTimerPlaceholder->GetSize().y * .5f * progressRatio);
		
}

float OrderTimer::GetProgressRatio()
{
		return (float)(m_totalTime / m_limitTime);
}

void OrderTimer::Draw()
{
		m_pUILineWarn->SetPosition(
				m_pUIOrderTimerPlaceholder->GetPosition().x,
				m_pUIOrderTimerPlaceholder->GetPosition().y + m_pUIOrderTimerPlaceholder->GetSize().y * .5f - m_pUIOrderTimerPlaceholder->GetSize().y * m_fWarnRatio
		);

		m_pUILineDanger->SetPosition(
				m_pUIOrderTimerPlaceholder->GetPosition().x,
				m_pUIOrderTimerPlaceholder->GetPosition().y + m_pUIOrderTimerPlaceholder->GetSize().y * .5f - m_pUIOrderTimerPlaceholder->GetSize().y * m_fDangerRatio
		);
		m_pUIOrderTimerPlaceholder->Draw();
		m_pUIOrderTimerFill->Draw();
		m_pUILineWarn->Draw();
		m_pUILineDanger->Draw();
}

OrderTimer::State OrderTimer::GetState()
{
	float progressRatio = GetProgressRatio();
	if (progressRatio < m_fWarnRatio && progressRatio > m_fDangerRatio)
		return State::Warn;
	else if (progressRatio < m_fDangerRatio)
		return State::Danger;
	else
		return State::Safe;
}

COrder::COrder() 
		: COrder({ Block::Lettuce, Block::Patty })
{
}
COrder::COrder(std::list<Block::Block_Color> Recipe)
		: COrder(Recipe, 20.f)
{
}
COrder::COrder(std::list<Block::Block_Color> Recipe, float limit)
	: COrder(Recipe, limit, 10)
{

}
COrder::COrder(std::list<Block::Block_Color> Recipe, float limit, int price)
{
	m_nBaseScore = price;
		m_Order = Recipe;
		m_pos = { 300.f, 100.f };
		double _scale = 0.7;
		m_pUIOrderBubble = new UIObject("OrderBubble.png", 0.f, 0.f, 256.f*_scale, 256.f*_scale);
		std::string filename = "Burger_";
		m_Order.sort([](Block::Block_Color a, Block::Block_Color b)
		{
				return (int)a < (int)b;
		});
		for (auto it = m_Order.begin(); it != m_Order.end(); ++it)
		{
				if (it != m_Order.begin())
						filename += "-";
				switch ((*it))
				{
					case Block::Patty:
							filename += "Patty"; break;
					case Block::Lettuce:
							filename += "Lettuce"; break;
					case Block::Fried_egg:
							filename += "Egg"; break;
					case Block::Bacon:
							filename += "Bacon"; break;
					case Block::Cheese:
							filename += "Cheese"; break;
					case Block::Tomato:
							filename += "Tomato"; break;
					default:
							break;
				}
		}

		filename += ".png";
		
		m_pUIOrderBurger = new UIObject(filename, 0.f, 0.f, 128.f * _scale, 128.f * _scale);
		m_pOrderTimer = new OrderTimer(limit);
}

COrder::~COrder()
{
		SAFE_DELETE(m_pUIOrderBubble);
		SAFE_DELETE(m_pOrderTimer);
}

void COrder::Update()
{
		m_pUIOrderBubble->SetPosition(m_pos);
		m_pOrderTimer->SetPosition(m_pUIOrderBubble->GetPosition().x + 48.f, m_pUIOrderBubble->GetPosition().y);
		m_pUIOrderBurger->SetPosition(m_pUIOrderBubble->GetPosition().x - 30.f, m_pUIOrderBubble->GetPosition().y);
		m_pOrderTimer->Update();
		if (m_pOrderTimer->GetProgressRatio() == 0.0f)
				m_bFailure = true;
}

void COrder::Draw()
{

		m_pUIOrderBubble->Draw();
		m_pUIOrderBurger->Draw();
		m_pOrderTimer->Draw();
}


void COrder::SetOrderIndex(int num)
{
		m_nOrderIndex = num;
		m_pos.x = 350.f + m_nOrderIndex * 300.f;

}

int COrder::SellScore()
{
	OrderTimer::State timerState = m_pOrderTimer->GetState();
	switch (timerState)
	{
	case (OrderTimer::State::Safe):
		return int(m_nBaseScore);
		break;
	case (OrderTimer::State::Warn):
		return int(m_nBaseScore * .6f);
		break;
	case (OrderTimer::State::Danger):
		return int(m_nBaseScore * .25f);
		break;
	default: 
		OutputDebugString("みえちゃいけない\n");
		break;
	}


}

bool COrder::GetIsFailure()
{
		return m_bFailure;
}

bool COrder::IsCorrect(std::list<Block::Block_Color> burger)
{
	m_Order.sort([](Block::Block_Color a, Block::Block_Color b)
		{
			return (int)a < (int)b;
		});

	burger.sort([](Block::Block_Color a, Block::Block_Color b)
		{
			return (int)a < (int)b;
		});

	return (m_Order == burger);
}


float COrder::GetProgress()
{
	return m_pOrderTimer->GetProgressRatio();
}
