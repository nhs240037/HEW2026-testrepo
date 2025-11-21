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
};

#endif // __SCENE_GAME_H__