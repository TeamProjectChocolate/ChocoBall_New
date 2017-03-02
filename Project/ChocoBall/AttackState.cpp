#include "stdafx.h"
#include "AttackState.h"
#include "Enemy_Boss.h"
#include "Player.h"
#include "TransState.h"

CAttackState::~CAttackState()
{
}

void CAttackState::Entry() {
	//ChangeLocalState(CEnemy_Boss::BOSS_STATE::ShotAttack);
	ChangeLocalState(CEnemy_Boss::BOSS_STATE::RushAttack);

	// �o���A�I�t�B
	m_pObject->GetBarrier()->OffBarrier();
	// �R�[�X������ꎞ�I�ɃI�t�B
	m_pObject->SetIsUpdateCource(false);
}

bool CAttackState::Update() {
	// �b�菈���B
	if (m_pCurrentLocalState->Update()) {
		if (m_pObject->GetNowCource().size() == 1 && m_pObject->GetNowCource()[0]->IsEnd) {
			// �Ō�̃R�[�X�B
			// ������x�҂���Ԃɖ߂�����A�U���Ɉڍs�B
			m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::BWait);
		}
		else {
			// �U�����I������̂ŃR�[�X�̑�����Move�ɕύX���čĂшړ��J�n�B
			vector<BOSS_COURCE_BLOCK*> now = m_pObject->GetNowCource();
			now[0]->BlockType = Boss_Cource::BOSS_COURCE_TYPE::Move;
			m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::BMove);
		}
		return true;
	}
	return false;
}

// �ːi�B
bool CAttackState::BossRush() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::FastsSwim);
	m_pObject->SetPlayingState(PLAYING_STATE::REPEAT);
	return true;
}

void CAttackState::Exit(CEnemy_Boss::BOSS_STATE next) {
	if (next == CEnemy_Boss::BOSS_STATE::BMove) {
		// �U�����I�����Ĉړ����n�߂���ǂ��J����B
		m_pObject->DivisionWallOpen();
	}
	else if (next == CEnemy_Boss::BOSS_STATE::Escape) {
		// �U�����Ƀ_���[�W���󂯂��B
		vector<BOSS_COURCE_BLOCK*> now = m_pObject->GetNowCource();
		if (now.size() == 1 && !(now[0]->IsEnd)) {
			// �Ō�̃R�[�X�ł͂Ȃ��̂ŃR�[�X�̑�����Move�ɕύX�B
			now[0]->BlockType = Boss_Cource::BOSS_COURCE_TYPE::Move;
		}
	}
	// �R�[�X������ċN���B
	m_pObject->SetIsUpdateCource(true);
}
