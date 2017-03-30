#include "stdafx.h"
#include "WaitState.h"
#include "Enemy_Boss.h"
#include "RotateState.h"
#include "ObjectManager.h"
#include "Player.h"


CWaitState::~CWaitState()
{
}

void CWaitState::Entry() {
	m_pPlayer = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::Wait);
	m_pObject->SetPlayingState(ANIMATION::PLAYING_STATE::REPEAT);

	m_WaitStage = State_Wait::WaitStage::Rotate;

	ChangeLocalState(CEnemy_Boss::BOSS_STATE::Rotate);
	// �҂������^�[�Q�b�g�Ɏw��B
	static_cast<CRotateState*>(m_pCurrentLocalState)->SetTargetDirection(m_pObject->GetNowCource()[0]->ActionTargetDir);
	static_cast<CRotateState*>(m_pCurrentLocalState)->SetRotationTime(1.0f);

	// �o���A�N���B
	m_pObject->GetBarrier()->OnBarrier();
}

bool CWaitState::Update() {
	if (m_pCurrentLocalState->Update()) {
		switch (m_WaitStage) {
		case State_Wait::WaitStage::Rotate:
			m_WaitStage = State_Wait::WaitStage::WaitNow;
			ChangeLocalState(CEnemy_Boss::BOSS_STATE::NonAction);
			break;
		case State_Wait::WaitStage::WaitNow:
			if (m_pObject->GetNowCource()[0]->BlockType == Cource::Boss_Cource::BOSS_COURCE_TYPE::Wait) {
				if (D3DXVec3Length(&(m_pPlayer->GetPos() - m_pObject->GetPos())) <= WAIT_RANGE) {
					// �v���C���[�����͈͂܂ŋ߂Â������B
					m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::BMove);
					return true;
				}
				else {
					return false;
				}
			}
			else if (m_pObject->GetNowCource()[0]->BlockType == Cource::Boss_Cource::BOSS_COURCE_TYPE::Attack) {
				if (D3DXVec3Length(&(m_pPlayer->GetPos() - m_pObject->GetPos())) <= WAIT_RANGE) {
					// �v���C���[�����͈͂܂ŋ߂Â������B
					// �R�[�X�̑������U���B
					m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::Attack);
					return true;
				}
				else {
					return false;
				}
			}
			else if (m_pObject->GetNowCource()[0]->BlockType == Cource::Boss_Cource::BOSS_COURCE_TYPE::Sleep) {
				m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::Sleep);
				return false;
			}
			break;
		}
	}
	return false;
}

void CWaitState::Exit(CEnemy_Boss::BOSS_STATE next) {
	//if (next == CEnemy_Boss::BOSS_STATE::Escape) {
	//	m_IsFirstChocoHit = false;
	//}
	if(next == CEnemy_Boss::BOSS_STATE::BMove) {
		m_pObject->DivisionWallOpen();	// �����ӂ����ł����ǂ��J���鏈���B
	}
}

