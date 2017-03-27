#pragma once
#include "BulletPhysics.h"
#include "EnemyBase.h"
#include "Audio.h"
#include "DivisionWall.h"

#define MaxCollision 100

class CCBManager;

class CLevelBuilder:public CGameObject{
public:
	CLevelBuilder();
	~CLevelBuilder();
	void Build(CAudio*);
	void Initialize()override{};
	void Update()override{};
	void Draw()override{};
	void SetIsStage(STAGE_ID now){
		m_IsStage = now;
	}
private:
	std::vector<CCBManager*>	m_chocoballMgrList;			//チョコボールマネージャのリスト。
	vector<CCollisionObject*> m_BGM_Changers;	// BGM変更判定用コリジョン。
	STAGE_ID m_IsStage;
	int m_ChocoWallNum;
	int m_FireJetNum;
	int m_SmokeJetNum;
	vector<CDivisionWall*> m_DivisionWalls;
};