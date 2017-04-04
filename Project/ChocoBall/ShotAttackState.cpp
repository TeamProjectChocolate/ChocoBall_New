#include "stdafx.h"
#include "ShotAttackState.h"
#include "TransState.h"

CShotAttackState::~CShotAttackState()
{
}

void CShotAttackState::Entry() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::FastsSwim);
	m_pObject->SetPlayingState(ANIMATION::PLAYING_STATE::REPEAT);

	m_ShotState = Shot_State::Ready;
	// ’e”­ŽË€”õ‚Ì–Â‚«º(ƒvƒŒƒCƒ„[‚Ö‚Ì‡})B
	m_pObject->GetBossAudio()->Play("BossDamageVoice1", true, this);

	ChangeLocalState(CEnemy_Boss::BOSS_STATE::Trans);
	static_cast<CTransState*>(m_pCurrentLocalState)->SetMoveSpeed(0.125f);
	static_cast<CTransState*>(m_pCurrentLocalState)->SetDirection(m_pObject->GetDirection() * -1);
	static_cast<CTransState*>(m_pCurrentLocalState)->SetLengthMax(10.0f);
}

bool CShotAttackState::Update() {

	if (m_pCurrentLocalState->Update()) {
		switch (m_ShotState) {
		case Shot_State::Ready:
			ChangeLocalState(CEnemy_Boss::BOSS_STATE::Shot);
			m_ShotState = Shot_State::Action;
			break;
		case Shot_State::Action:
			ChangeLocalState(CEnemy_Boss::BOSS_STATE::Trans);
			static_cast<CTransState*>(m_pCurrentLocalState)->SetMoveSpeed(0.125f);
			static_cast<CTransState*>(m_pCurrentLocalState)->SetLengthMax(10.0f);
			m_ShotState = Shot_State::Post;
			break;
		case Shot_State::Post:
			return true;
		}
	}
	return false;
}

void CShotAttackState::Exit(CEnemy_Boss::BOSS_STATE next) {
}