#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Scene.h"
#include "Model.h"
#include "Camera.h"
#include "Player.h"
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
	Camera* m_pCamera = nullptr;
	Player* m_pPlayer = nullptr;
	NextItem* m_pNext = nullptr;
};

#endif // __SCENE_GAME_H__