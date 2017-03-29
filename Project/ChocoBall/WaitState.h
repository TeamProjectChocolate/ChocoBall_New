#pragma once
#include "State.h"

#define WAIT_RANGE 30.0f	// ���̋����܂Ńv���C���[���߂Â��Α҂̂���߂�B

namespace State_Wait {
	enum WaitStage {Rotate = 0,WaitNow};
}

class CPlayer;

class CWaitState :
	public CState
{
public:
	CWaitState(CEnemy_Boss* obj) :CState(obj) {};
	~CWaitState();
	void Entry()override;
	bool Update()override;
	void Exit(CEnemy_Boss::BOSS_STATE)override;
	bool IsPossibleDamage()override {
		return false;
	}
private:
	//bool m_IsFirstChocoHit = true;	// �ŏ��̈��݂̂��̃X�e�[�g�Ń_���[�W���󂯂�B
	State_Wait::WaitStage m_WaitStage;
	CPlayer* m_pPlayer;
};

