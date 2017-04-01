#include "stdafx.h"
#include "AttackState.h"
#include "Enemy_Boss.h"
#include "Player.h"
#include "TransState.h"

CAttackState::~CAttackState()
{
}

void CAttackState::Entry() {

	//if (m_pObject->GetNowCource().size() == 1 && m_pObject->GetNowCource()[0]->IsEnd) {
	//	// �Ō�̃R�[�X��`(�����_���ł����ꂩ�̍U��)�B
	//	m_IntervalTime = 0.5f;
	//	m_TimeCounter = 0.0f;
	//	m_ActionState = ActionState::Interval;
	//	ChangeLocalState(CEnemy_Boss::BOSS_STATE::NonAction);
	//}
	//else {
		// �o���A�I�t�B
		m_pObject->GetBarrier()->OffBarrier();
		m_ActionState = ActionState::Action;

		ChangeLocalState(CEnemy_Boss::BOSS_STATE::ShotAttack);
	//}
	// �R�[�X������ꎞ�I�ɃI�t�B
	m_pObject->SetIsUpdateCource(false);

}

bool CAttackState::Update() {
	if (m_pCurrentLocalState->Update()) {
		if (m_ActionState == ActionState::Interval) {
			// ���������Ɍ�����悤�ɏ����Ԃ��J����B
			if (m_TimeCounter >= m_IntervalTime) {
				m_ActionState = ActionState::Action;
				// �o���A�I�t�B
				m_pObject->GetBarrier()->OffBarrier();

				int rnd = rand() % static_cast<int>(Attack_State::Max);
				switch (static_cast<Attack_State>(rnd)) {
				case Attack_State::Rush:
					ChangeLocalState(CEnemy_Boss::BOSS_STATE::RushAttack);
					break;
				case Attack_State::Shot:
					ChangeLocalState(CEnemy_Boss::BOSS_STATE::ShotAttack);
					break;
				}
			}
			m_TimeCounter += 1.0f / 60.0f;
		}
		else {
			if (m_pObject->GetNowCource().size() == 1 && m_pObject->GetNowCource()[0]->IsEnd) {
				// �Ō�̃R�[�X�B
				// �o���A�I���B
				m_pObject->GetBarrier()->OnBarrier();
				// ������x�҂���Ԃɖ߂�����A�U���Ɉڍs�B
				m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::BWait);
			}
			else {
				// �U�����I������̂ŃR�[�X�̑�����Move�ɕύX���čĂшړ��J�n�B
				vector<Cource::BOSS_COURCE_BLOCK*> now = m_pObject->GetNowCource();
				now[0]->BlockType = Cource::Boss_Cource::BOSS_COURCE_TYPE::Move;
				m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::BMove);
			}
			return true;
		}
	}
	return false;
}

// �ːi�B
bool CAttackState::BossRush() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::FastsSwim);
	m_pObject->SetPlayingState(ANIMATION::PLAYING_STATE::REPEAT);
	return true;
}

void CAttackState::Exit(CEnemy_Boss::BOSS_STATE next) {
	if (next == CEnemy_Boss::BOSS_STATE::BMove) {
		// �U�����I�����Ĉړ����n�߂���ǂ��J����B
		m_pObject->DivisionWallOpen();
	}
	else if (next == CEnemy_Boss::BOSS_STATE::Escape) {
		// �U�����Ƀ_���[�W���󂯂��B
		vector<Cource::BOSS_COURCE_BLOCK*> now = m_pObject->GetNowCource();
		if (now.size() == 1 && !(now[0]->IsEnd)) {
			// �Ō�̃R�[�X�ł͂Ȃ��̂ŃR�[�X�̑�����Move�ɕύX�B
			now[0]->BlockType = Cource::Boss_Cource::BOSS_COURCE_TYPE::Move;
		}
	}
	// �R�[�X������ċN���B
	m_pObject->SetIsUpdateCource(true);
}

