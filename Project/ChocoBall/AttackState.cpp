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
	//	// 最後のコース定義(ランダムでいずれかの攻撃)。
	//	m_IntervalTime = 0.5f;
	//	m_TimeCounter = 0.0f;
	//	m_ActionState = ActionState::Interval;
	//	ChangeLocalState(CEnemy_Boss::BOSS_STATE::NonAction);
	//}
	//else {
		// バリアオフ。
		m_pObject->GetBarrier()->OffBarrier();
		m_ActionState = ActionState::Action;

		ChangeLocalState(CEnemy_Boss::BOSS_STATE::ShotAttack);
	//}
	// コース判定を一時的にオフ。
	m_pObject->SetIsUpdateCource(false);

}

bool CAttackState::Update() {
	if (m_pCurrentLocalState->Update()) {
		if (m_ActionState == ActionState::Interval) {
			// いい感じに見えるように少し間を開ける。
			if (m_TimeCounter >= m_IntervalTime) {
				m_ActionState = ActionState::Action;
				// バリアオフ。
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
				// 最後のコース。
				// バリアオン。
				m_pObject->GetBarrier()->OnBarrier();
				// もう一度待ち状態に戻った後、攻撃に移行。
				m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::BWait);
			}
			else {
				// 攻撃が終わったのでコースの属性をMoveに変更して再び移動開始。
				vector<Cource::BOSS_COURCE_BLOCK*> now = m_pObject->GetNowCource();
				now[0]->BlockType = Cource::Boss_Cource::BOSS_COURCE_TYPE::Move;
				m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::BMove);
			}
			return true;
		}
	}
	return false;
}

// 突進。
bool CAttackState::BossRush() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::FastsSwim);
	m_pObject->SetPlayingState(ANIMATION::PLAYING_STATE::REPEAT);
	return true;
}

void CAttackState::Exit(CEnemy_Boss::BOSS_STATE next) {
	if (next == CEnemy_Boss::BOSS_STATE::BMove) {
		// 攻撃を終了して移動を始めたら壁を開ける。
		m_pObject->DivisionWallOpen();
	}
	else if (next == CEnemy_Boss::BOSS_STATE::Escape) {
		// 攻撃中にダメージを受けた。
		vector<Cource::BOSS_COURCE_BLOCK*> now = m_pObject->GetNowCource();
		if (now.size() == 1 && !(now[0]->IsEnd)) {
			// 最後のコースではないのでコースの属性をMoveに変更。
			now[0]->BlockType = Cource::Boss_Cource::BOSS_COURCE_TYPE::Move;
		}
	}
	// コース判定を再起動。
	m_pObject->SetIsUpdateCource(true);
}

