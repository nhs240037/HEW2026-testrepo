#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Scene.h"
#include "Model.h"
#include "Camera.h"
#include "Player.h"
#include "Block.h"
#include "Defines.h"
#include "Score.h"
#include "Timer.h"
#include "NextItem.h"
#include "OrderManager.h"
#include "Field.h"

class SceneGame : public Scene
{
public:
	SceneGame();
	~SceneGame();
	void Update() final;
	void Draw() final;
	float RandomFloat(float min, float max)
	{
		return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max - min);
	}

private:
	Model* m_pModel;
	Camera* m_pCamera;
	Player* m_pPlayer;

	std::list<Block*> m_pBlock;         // dynamic block list
	

	int m_menu[5];
	CsvData &csv;
	CScore *m_pScore;
	CTimer *m_pTimer;
	NextItem* m_pNextItem;
	float min;
	float max;
	OrderManager* m_pOrderManager;

	// New!
	Field* m_pField;
};

#endif // __SCENE_GAME_H__