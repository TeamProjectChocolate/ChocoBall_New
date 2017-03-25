#include "stdafx.h"
#include "EscapeState.h"
#include "Enemy_Boss.h"
#include "TransState.h"
#include "CBManager.h"

CEscapeState::~CEscapeState()
{
}

void CEscapeState::Entry() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::Up);
	m_pObject->SetPlayingState(ANIMATION::PLAYING_STATE::ONCE);
	this->ChangeLocalState(CEnemy_Boss::BOSS_STATE::NonAction);
	m_EStage = Escape_Stage::Damage;
	m_TimeCounter = 0.0f;

	// �R�[�X������ꎞ�I�ɃI�t�B
	m_pObject->SetIsUpdateCource(false);
}

bool CEscapeState::Update() {
	switch (m_EStage) {
	case Escape_Stage::Damage:
		if ((m_TimeCounter / 0.01f) - (int)(m_TimeCounter / 0.01f) <= 0.001f) {
			// �_���[�W���󂯂Ă���\���B
			if (m_pObject->GetAlpha() <= 0.0f) {
				m_pObject->SetAlpha(1.0f);
			}
			else {
				m_pObject->SetAlpha(0.0f);
			}
		}
		if (m_TimeCounter >= m_IntervalTime/*m_pObject->GetAnimation()->GetIsOnceEnd()*/) {
			// �����A�j���[�V�������Đ����I������B
			vector<Cource::BOSS_COURCE_BLOCK*> now = m_pObject->GetNowCource();
			this->ChangeLocalState(CEnemy_Boss::BOSS_STATE::Trans);
			if (now.size() == 1) {
				if (!(now[0]->IsEnd)) {
					// �Ō�̃R�[�X�ł͂Ȃ������ꍇ�B
					m_pObject->DivisionWallOpen();	// �����ӂ����ł����ǂ��J���鏈���B
				}
				else if (m_pObject->GetIsBreak()) {
					// �Ō�̃R�[�X��`����HP�o�[���u���C�N�������ߕǂ��J���ē����B
					m_pObject->DivisionWallOpen();
					// �������邽�߂ɃR�[�X�̑�����Move�ɕύX�B
					vector<Cource::BOSS_COURCE_BLOCK*> now = m_pObject->GetNowCource();
					now[0]->BlockType = Cource::Boss_Cource::BOSS_COURCE_TYPE::Move;
					m_pObject->SetIsBreak(false);
				}
				D3DXVECTOR3 dir = now[0]->ActionPos - m_pObject->GetPos();
				float length = D3DXVec3Length(&dir);
				static_cast<CTransState*>(m_pCurrentLocalState)->SetLengthMax(length);
				D3DXVec3Normalize(&dir, &dir);
				static_cast<CTransState*>(m_pCurrentLocalState)->SetDirection(dir);
			}
			static_cast<CTransState*>(m_pCurrentLocalState)->SetMoveSpeed(0.125f);
			m_pObject->SetAlpha(1.0f);
			m_EStage = Escape_Stage::ReturnActionPoint;
			if (m_HitCBManager) {
				// ���ԍ��Ń`���R�{�[����j�􂳂���B
				m_HitCBManager->OnBurst(0.4f);
				m_HitCBManager = nullptr;
			}
			// �o���A�N���B
			m_pObject->GetBarrier()->OnBarrier();
		}
		m_TimeCounter += 1.0f / 60.0f;
		break;
	case Escape_Stage::ReturnActionPoint:
		// �{�X���A�����B
		if (m_pCurrentLocalState->Update()) {
			if (m_pObject->GetNowCource().size() == 1 && m_pObject->GetNowCource()[0]->IsEnd) {
				// �Ō�̃R�[�X�B
				// ������x�҂���Ԃɖ߂�����A�U���Ɉڍs�B
				m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::BWait);
			}
			else {
				// �{�X���A�I���B
				m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::BMove);
				return true;
			}
		}
		break;
	}
	return false;
}


void CEscapeState::Exit(CEnemy_Boss::BOSS_STATE next) {
	// �R�[�X������ċN���B
	m_pObject->SetIsUpdateCource(true);
}

