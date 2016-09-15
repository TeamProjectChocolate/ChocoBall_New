#pragma once
#include "GraphicsDevice.h"
#include "Scene.h"
#include "Player.h"
#include "EnemyBase.h"
#include "Bullet.h"
#include "EnemyManager.h"
#include "CBManager.h"
#include "LevelBuilder.h"

#define ENEMYNUM 20

class CMainScene:public CScene{
public:
	CMainScene();
	~CMainScene();
	void Initialize();
	void Update();
	void Draw();
private:
	CPlayer* m_Player;
	EnemyBase m_Enemy;	//ìGÇÃïœêîÅB
	Bullet m_Bullet;
	GAMEEND_ID m_GameState = GAMEEND_ID::CONTINUE;
	bool m_isGameContinue = true;
	CLevelBuilder m_CLevelBuilder;
};