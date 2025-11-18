#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Scene.h"
#include "Model.h"
#include "Camera.h"
#include "Player.h"
#include "Block.h"
#include "Defines.h"
#include "UIObject.h"

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
	UIObject* m_pExampleUI;
};

#endif // __SCENE_GAME_H__