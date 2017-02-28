#pragma once
#include "State.h"

class CRushAttackState :
	public CState
{
public:
	CRushAttackState(CEnemy_Boss* obj) :CState(obj) {};
	~CRushAttackState();
	void Entry()override;
	bool Update()override;
	void Exit(CEnemy_Boss::BOSS_STATE)override;
	virtual bool IsPossibleDamage()override {
		return true;
	}
private:
	float m_AttackTime;	// UŒ‚ŠÔB
	float m_IntervalTime;	// UŒ‚ŠÔŠuB
	float m_TimeCounter;	// ì‹Æ—p(ŠÔ‚ğ”‚¦‚é)B
	bool m_IsFirst = true;
};

