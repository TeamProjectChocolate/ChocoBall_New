#pragma once 
#include "stdafx.h"
#include "C3DImage.h"
#include "StageTable.h"
#include "islntersect.h"
#include "Turn.h"

class CEnemyManager;

class EnemyBase : public CGameObject
{
public:
	virtual void OnDestroy();
	virtual void Initialize()override;
	virtual void Update()override;
	void Build();
	virtual void Move() = 0;
	void PlayerBulletHit(D3DXVECTOR3);
	void RollingEnemy();
	void SetUpTechnique()override{
		m_pRender->SetUpTechnique("Skin_Tex_Lim");
	}
	void EM_SetUpTechnique()override{
		m_pEMSamplingRender->SetUpTechnique("Skin_Tex_Lim");
	}

	void SetStageID(STAGE_ID id){
		m_StageID = id;
	}

	MOVE_STATE GetMoveState(){
		return m_State;
	}
private:
	enum ENEMY_ANIMATION{ Down = 0, Wait, Walk, Stance,WaitNow, MAX };
private:
	CIsIntersect m_IsIntersect;		//CIsIntersectのインスタンス
	float m_deadTimer;
	CEnemyManager* m_pEnemyManager;
	ENEMY_ANIMATION m_AnimState;
	PLAYING_STATE m_PlayingState;
protected:
	STAGE_ID m_StageID;
	// 挙動制御関連。
	MOVE_STATE m_State;
	float m_CurrentAngleY;
	float m_TargetAngleY;
	bool m_IsTurn;
	CTurn m_Turn;				//CTurnのインスタンス
};

static double EnemyAnimationTime[] = {
	21.0 / 30.0,
	101.0 / 30.0,
	16.0 / 30.0,
	9.0 / 30.0,
	101.0 / 30.0,
};