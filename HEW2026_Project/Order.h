#pragma once
#include <vector>
#include "Block.h"

class COrder
{
public:
	//

	COrder();
	COrder(std::vector<Block*> Recipe);
	~COrder();

	void Update();
	void Draw();
	void SetProgress(float ratio);


private:
	std::vector<std::vector<Block*>> m_Order;
	float m_limit;

};

