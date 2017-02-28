#include "stdafx.h"
#include "ShotState.h"
#include "Player.h"
#include "EnemyBullet.h"
#include "Enemy_Boss.h"

CShotState::CShotState(CEnemy_Boss* obj) : CState(obj) {
	m_pPlayer = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("TEST3D"));
}

CShotState::~CShotState()
{
}

void CShotState::Entry() {
	m_TimeCounter = 0.0f;
}

bool CShotState::Update() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::FastsSwim);
	m_pObject->SetPlayingState(PLAYING_STATE::ONCE);
	m_TimeCounter += 1.0f / 60.0f;
	if (m_TimeCounter >= m_IntervalTime) {
		for (int idx = 0; idx < 4; idx++) {
			CEnemyBullet* bullet = SINSTANCE(CObjectManager)->GenerationObject<CEnemyBullet>(_T("EnemyBaullet"), PRIORTY::OBJECT3D, false);
			bullet->Initialize();
			D3DXVECTOR3 pos = m_pObject->GetPos() + (BuletShotOffsetDir[idx]) * 3.0f;
			bullet->SetStartPos(pos);
			bullet->SetBulletSpeed(0.07f);
			D3DXVECTOR3 work;
			D3DXVec3Normalize(&work, &D3DXVECTOR3(m_pPlayer->GetPos() - pos));
			bullet->SetDir(work);
			bullet->SetRange(50.0f);
			m_TimeCounter = 0.0f;
		}
	}
	if (m_pObject->GetAnimation()->GetIsOnceEnd()) {
		return true;
	}
	return false;
}

void CShotState::Exit(CEnemy_Boss::BOSS_STATE next) {

}

