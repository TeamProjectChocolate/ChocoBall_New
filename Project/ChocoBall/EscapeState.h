#pragma once
#include "State.h"
#include "Vibration.h"


class CEscapeState :
	public CState
{
private:
	enum Escape_Stage { Damage = 0, ReturnActionPoint };
public:
	CEscapeState(CEnemy_Boss* obj) :CState(obj) {};
	~CEscapeState();
	void Entry()override;
	bool Update()override;
	void Exit(CEnemy_Boss::BOSS_STATE)override;
	bool IsPossibleDamage()override {
		return false;
	}
	void SetHitCBManager(CCBManager* pHit) {
		m_HitCBManager = pHit;
	}
private:
	Escape_Stage m_EStage = Escape_Stage::Damage;
	float m_IntervalTime = 1.0f;	// ダメージを受けた時の演出時間。
	float m_TimeCounter;
	CCBManager* m_HitCBManager = nullptr;
};

