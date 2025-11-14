#pragma once
#include "Texture.h"
#include "Sprite.h"

class CScore
{
public:
	CScore();
	~CScore();
	void Update();
	void Draw();
private:
	Texture* m_pScoreTex;
	Texture* m_pGaugeTex;
	float m_rate;
};