#pragma once
#include "State.h"

class CShotAttackState :
	public CState
{
private:
	enum Shot_State { Ready = 0, Action, Post };
public:
	CShotAttackState(CEnemy_Boss* obj) :CState(obj) {};
	~CShotAttackState();
	void Entry()override;
	bool Update()override;
	void Exit(CEnemy_Boss::BOSS_STATE)override;
	virtual bool IsPossibleDamage()override {
		return true;
	}
private:
	Shot_State m_ShotState;	// ’e‚ğŒ‚‚ÂUŒ‚‚Ì’iŠKB
};


