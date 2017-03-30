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
	// 待つ方向をターゲットに指定。
	static_cast<CRotateState*>(m_pCurrentLocalState)->SetTargetDirection(m_pObject->GetNowCource()[0]->ActionTargetDir);
	static_cast<CRotateState*>(m_pCurrentLocalState)->SetRotationTime(1.0f);

	// バリア起動。
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
					// プレイヤーが一定範囲まで近づいたか。
					m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::BMove);
					return true;
				}
				else {
					return false;
				}
			}
			else if (m_pObject->GetNowCource()[0]->BlockType == Cource::Boss_Cource::BOSS_COURCE_TYPE::Attack) {
				if (D3DXVec3Length(&(m_pPlayer->GetPos() - m_pObject->GetPos())) <= WAIT_RANGE) {
					// プレイヤーが一定範囲まで近づいたか。
					// コースの属性が攻撃。
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
		m_pObject->DivisionWallOpen();	// 道をふさいでいた壁を開ける処理。
	}
}

