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
	float m_AttackTime;	// �U�����ԁB
	float m_IntervalTime;	// �U���Ԋu�B
	float m_DamageIntervalTime;	// �U���I������_���[�W�܂ł̑҂����ԁB
	float m_TimeCounter = 0.0f;	// ��Ɨp(���Ԃ𐔂���)�B
	float m_SumTimeCounter = 0.0f;	// ���������Ԃ̍��v�B
	float m_DamageTimeCounter = 0.0f;	// �U���I������J�E���g�B
	int m_CreateCBNum = 2;	// ���x�`���R�{�[���𐶐����邩�B
	bool m_IsFirst = true;
	int m_Count = 0;
};

