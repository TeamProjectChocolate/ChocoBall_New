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
	enum Enemy_ModelType { People = 0, Boss};
	virtual void OnDestroy();
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void ConfigLight()override;
	void Build();
	void PlayerBulletHit(D3DXVECTOR3);
	void SetInitPosition(D3DXVECTOR3 pos);
	virtual void HitReaction(D3DXVECTOR3) = 0;
	void SetUpShadowTechnique()override {
		m_pShadowRender->SetUpTechnique("BoneShadowMapping");
	}


	void SetStageID(STAGE_ID id){
		m_StageID = id;
	}

	MOVE::STATE GetMoveState(){
		return m_State;
	}

	Enemy_ModelType GetMyType() {
		return m_MyModelType;
	}
protected:
	enum ENEMY_ANIMATION{ Down = 0, Wait, Walk, Stance,WaitNow, MAX };
private:
	CEnemyManager* m_pEnemyManager;
protected:
	D3DXVECTOR3		m_initPosition;		//初期座標。
	int m_AnimState;
	ANIMATION::PLAYING_STATE m_PlayingState;
	STAGE_ID m_StageID;
	// 挙動制御関連。
	MOVE::STATE m_State;
	float m_CurrentAngleY;
	float m_TargetAngleY;
	bool m_IsTurn;
	CTurn m_Turn;				//CTurnのインスタンス
	Enemy_ModelType m_MyModelType;
};

#define Max_AnimNum 6

static double EnemyAnimationTime[][Max_AnimNum] = {
	// 人型雑魚エネミー。
	{
		21.0 / 30.0,
		101.0 / 30.0,
		16.0 / 30.0,
		9.0 / 30.0,
		101.0 / 30.0
	},
};