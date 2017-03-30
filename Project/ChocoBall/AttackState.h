#pragma once
#include "State.h"
#include "EnemyBullet.h"

class CAttackState :
	public CState
{
private:
	enum class Attack_State { Shot = 0,Rush,Max};
	enum class ActionState{Action = 0,Interval};
public:
	CAttackState(CEnemy_Boss* obj) :CState(obj) {};
	~CAttackState();
	void Entry()override;
	bool Update()override;
	void Exit(CEnemy_Boss::BOSS_STATE)override;
private:
	bool BossRush();
private:
	Attack_State m_State;
	ActionState m_ActionState;
	float m_IntervalTime;
	float m_TimeCounter = 0.0f;
};

