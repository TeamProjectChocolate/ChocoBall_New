#pragma once
#include "Enemy_Boss.h"

class CState
{
public:
	CState() {};
	CState(CEnemy_Boss* obj){
		m_pObject = obj;
	}
	virtual ~CState() {}
	virtual void Entry() {};
	// �߂�l�͏������I���������B
	virtual bool Update() { return true; };
	virtual void Exit(CEnemy_Boss::BOSS_STATE) {};
	// ���̃X�e�[�g�Ń_���[�W��^�����邩�B
	virtual bool IsPossibleDamage() {
		return true;
	}
protected:
	// ���[�J���X�e�[�g�������̒l�ɑΉ������X�e�[�g�ɕύX����B
	void ChangeLocalState(CEnemy_Boss::BOSS_STATE next) {
		if (m_pCurrentLocalState) {
			m_pCurrentLocalState->Exit(next);
		}
		m_pCurrentLocalState = m_pObject->GetState(next);
		m_pCurrentLocalState->Entry();
		m_NowLocalState = next;
	}
protected:
	CEnemy_Boss* m_pObject = nullptr;	// �{�X�̃|�C���^(�X�e�[�g�����Ń{�X���ړ�������ۂȂǂɎg�p)�B
	CEnemy_Boss::BOSS_STATE m_NowLocalState;
	CState* m_pCurrentLocalState = nullptr;	// ���݂̃��[�J���X�e�[�g�B
};

