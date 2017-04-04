#include "stdafx.h"
#include "MoveState.h"
#include "Enemy_Boss.h"
#include "TransState.h"
#include "RotateState.h"

CMoveState::~CMoveState()
{
}

void CMoveState::Entry() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::Swim);
	m_pObject->SetPlayingState(ANIMATION::PLAYING_STATE::REPEAT);

	// 移動モードに移行したため接触コースの判定を再開。
	m_pObject->SetIsUpdateCource(true);

	// コース定義から回転ステートのパラメータを決定。
	vector<Cource::BOSS_COURCE_BLOCK*> nowCource = m_pObject->GetNowCource();
	D3DXVECTOR3 dir = D3DXVECTOR3(0.0f,0.0f,0.0f);
	if (nowCource.size() == 1 && nowCource[0]->BlockType != Cource::Boss_Cource::BOSS_COURCE_TYPE::Move) {
		// アクションを行う位置への向きを取得。
		D3DXVec3Normalize(&dir, &(nowCource[0]->ActionPos - m_pObject->GetPos()));
		// 接触コースの更新を行わないようにする。
		// ※アクションする位置に向かう途中でコースの切り替えを発生させないため。
		m_pObject->SetIsUpdateCource(false);
	}
	else {
		int idx = 0;
		for (auto now : nowCource) {
			D3DXVECTOR3 work;
			D3DXVec3Normalize(&work, &(now->endPosition - now->startPosition));
			dir += work;
			idx++;
		}
		// 現在複数のコースに重なっている場合はハーフベクトルを算出。
		dir /= idx;
	}

	// 回転ステートにパラメータを設定。
	this->ChangeLocalState(CEnemy_Boss::BOSS_STATE::Rotate);
	// コースの向きをターゲットに設定。
	static_cast<CRotateState*>(m_pCurrentLocalState)->SetTargetDirection(dir);
	static_cast<CRotateState*>(m_pCurrentLocalState)->SetRotationPower(180.0f);

	// バリア起動。
	m_pObject->GetBarrier()->OnBarrier();
}

bool CMoveState::Update() {
	bool IsEnd = m_pCurrentLocalState->Update();
	if (IsEnd) {
		// ローカルステートの処理が完了した。
		switch (m_NowLocalState) {
		case CEnemy_Boss::BOSS_STATE::Rotate:
			this->ChangeLocalState(CEnemy_Boss::BOSS_STATE::Trans);
			static_cast<CTransState*>(m_pCurrentLocalState)->SetMoveSpeed(0.275f);
			if (m_pObject->GetNowCource().size() == 1) {
				// 接触しているコースが一つのみ。
				Cource::BOSS_COURCE_BLOCK* block = m_pObject->GetNowCource()[0];
				if (block->BlockType != Cource::Boss_Cource::BOSS_COURCE_TYPE::Move) {
					// コースの属性が移動ではない。
					float length = D3DXVec3Length(&(m_pObject->GetPos() - block->ActionPos));
					static_cast<CTransState*>(m_pCurrentLocalState)->SetLengthMax(length);
				}
			}
			break;
		case CEnemy_Boss::BOSS_STATE::Trans:
			m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::BWait);
			return true;
			break;
		}
	}
	else {
		if (m_pObject->GetIsNowCourceChange()) {
			// 移動した結果現在のコースに変更があった。
			m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::BMove);
		}
	}
	return false;
}

void CMoveState::Exit(CEnemy_Boss::BOSS_STATE next) {

}

