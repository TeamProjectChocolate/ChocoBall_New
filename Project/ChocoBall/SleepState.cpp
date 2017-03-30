#include "stdafx.h"
#include "SleepState.h"
#include "Player.h"

CSleepState::~CSleepState()
{
}

void CSleepState::Entry() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::Up);
	m_pObject->SetPlayingState(ANIMATION::PLAYING_STATE::ONCE);
	// 睡眠エフェクト登録。
	m_SleepEmitter = (
		CParticleEmitter::EmitterCreate(
		_T("SleepEmitter"),
		PARTICLE_TYPE::STAR, 
		m_pObject->GetPos(),
		SINSTANCE(CRenderContext)->GetCurrentCamera(),
		SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"))->GetStageID(),
		true,
		true)
	);
	m_pObject->GetBarrier()->OffBarrier();
}

bool CSleepState::Update() {
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pos = static_cast<D3DXVECTOR3>(m_pObject->Vec3Transform(pos));
	m_SleepEmitter->SetEmitPos(pos);
	return false;
}

void CSleepState::Exit(CEnemy_Boss::BOSS_STATE next) {
	// エミッター破棄。
	SINSTANCE(CObjectManager)->DeleteGameObject(m_SleepEmitter);

	// コースの属性をMoveに変更。
	vector<Cource::BOSS_COURCE_BLOCK*> now = m_pObject->GetNowCource();
	now[0]->BlockType = Cource::Boss_Cource::BOSS_COURCE_TYPE::Move;
}