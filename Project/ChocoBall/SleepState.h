#pragma once
#include "stdafx.h"
#include "State.h"
#include "ParticleEmitter.h"

class CSleepState :
	public CState
{
public:
	enum class SleepVoice{ Suck = 0, Spit};
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
	SleepVoice m_SleepVoice;	// 吸って吐くボイスを再生。
	float m_IntervalTime;
	float m_TimeCounter = 0.0f;
	CParticleEmitter* m_SleepEmitter = nullptr;	// 睡眠時エフェクト。
};

