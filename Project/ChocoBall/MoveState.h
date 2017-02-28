#pragma once
#include "State.h"
class CMoveState :
	public CState
{
public:
	CMoveState(CEnemy_Boss* obj) :CState(obj) {};
	~CMoveState();
	void Entry()override;
	bool Update()override;
	void Exit(CEnemy_Boss::BOSS_STATE)override;
	bool IsPossibleDamage()override {
		return false;
	}
};

