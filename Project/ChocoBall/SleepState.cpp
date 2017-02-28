#include "stdafx.h"
#include "SleepState.h"

CSleepState::~CSleepState()
{
}

void CSleepState::Entry() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::Up);
	m_pObject->SetPlayingState(PLAYING_STATE::ONCE);
	// �����G�t�F�N�g�o�^�B
	m_SleepEmitter = (
		CParticleEmitter::EmitterCreate(
		_T("SleepEmitter"),
		PARTICLE_TYPE::STAR, 
		m_pObject->GetPos(),
		SINSTANCE(CRenderContext)->GetCurrentCamera(),
		SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("TEST3D"))->GetStageID(),
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
	// �G�~�b�^�[�j���B
	SINSTANCE(CObjectManager)->DeleteGameObject(m_SleepEmitter);
}