#pragma once
#include "stdafx.h"
#include "State.h"
#include "ParticleEmitter.h"

class CSleepState :
	public CState
{
public:
	CSleepState(CEnemy_Boss* obj) :CState(obj) {};
	~CSleepState();
	void Entry()override;
	bool Update()override;
	void Exit(CEnemy_Boss::BOSS_STATE)override;
	bool IsPossibleDamage() {
		return true;
	}
private:
	CParticleEmitter* m_SleepEmitter = nullptr;	// 睡眠時エフェクト。
};

