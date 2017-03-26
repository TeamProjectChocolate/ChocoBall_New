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
	float m_AttackTime;	// 攻撃時間。
	float m_IntervalTime;	// 攻撃間隔。
	float m_DamageIntervalTime;	// 攻撃終了からダメージまでの待ち時間。
	float m_TimeCounter = 0.0f;	// 作業用(時間を数える)。
	float m_SumTimeCounter = 0.0f;	// 数えた時間の合計。
	float m_DamageTimeCounter = 0.0f;	// 攻撃終了からカウント。
	int m_CreateCBNum = 2;	// 何度チョコボールを生成するか。
	bool m_IsFirst = true;
	int m_Count = 0;
};

