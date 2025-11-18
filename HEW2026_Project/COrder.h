#pragma once
#include <vector>

class COrder
{
public:
	//

	COrder();
	COrder(std::vector<Item> Resipi);
	~COrder();

	void apdeta();
	void drwe();
	void setprogrs(float ratio);


private:
	std::vector<Item> m_Orter;
	float m_twotime;

};

