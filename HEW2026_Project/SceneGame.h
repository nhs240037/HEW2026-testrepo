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
	Block* m_pBlock[MAX_BLOCK];
	int m_menu[5];
	CsvData &csv;
	CScore *m_pScore;
	CTimer *m_pTimer;
	NextItem* m_pNextItem;
	float min;
	float max;
};

#endif // __SCENE_GAME_H__