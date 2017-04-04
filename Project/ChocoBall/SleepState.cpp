#include "stdafx.h"
#include "SleepState.h"
#include "Player.h"

CSleepState::~CSleepState()
{
}

void CSleepState::Entry() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::Up);
	m_pObject->SetPlayingState(ANIMATION::PLAYING_STATE::ONCE);
	// �����G�t�F�N�g�o�^�B
	// �p�[�e�B�N�������ʒu�����������̏ꏊ�ɐݒ�B
	D3DXVECTOR3 pos = m_pObject->GetPos() + (Vector3::Up * 1.5f);
	pos += m_pObject->GetDirection() * 10.0f;
	pos += m_pObject->GetTransform().right * 1.0f;
	m_SleepEmitter = CParticleEmitter::EmitterCreate(
			_T("SleepEmitter"),
			PARTICLE_TYPE::SLEEP,
			pos,
			SINSTANCE(CRenderContext)->GetCurrentCamera(),
			SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"))->GetStageID(),
			true,
			true);
	m_SleepEmitter->SetIsUseCource(false);
	m_pObject->GetBarrier()->OffBarrier();

	m_IntervalTime = 0.8f;
	m_TimeCounter = 0.0f;
	m_SleepVoice = SleepVoice::Suck;
	m_pObject->GetBossAudio()->Play("BossSleep1", false, nullptr);
}

bool CSleepState::Update() {
	if (m_TimeCounter >= m_IntervalTime) {
		if (m_SleepVoice == SleepVoice::Suck) {
			// ���z���Ă���B
			// �f���{�C�X�ɕύX�B
			m_pObject->GetBossAudio()->Stop("BossSleep1", false, nullptr);
			m_pObject->GetBossAudio()->Play("BossSleep2",false,nullptr);
			m_SleepVoice = SleepVoice::Spit;
		}
		else {
			// ���f���Ă���B
			// �z���{�C�X�ɕύX�B
			m_pObject->GetBossAudio()->Stop("BossSleep2", false, nullptr);
			m_pObject->GetBossAudio()->Play("BossSleep1", false, nullptr);
			m_SleepVoice = SleepVoice::Suck;
		}
		m_TimeCounter = 0.0f;
	}

	float deltaTime = 1.0f / 60.0f;
	m_TimeCounter += deltaTime;

	m_SleepEmitter->SetEmitFlg(true);

	return false;
}

void CSleepState::Exit(CEnemy_Boss::BOSS_STATE next) {
	if (m_SleepVoice == SleepVoice::Suck) {
		// ���z���Ă���B
		m_pObject->GetBossAudio()->Stop("BossSleep1", false, nullptr);
	}
	else {
		// ���f���Ă���B
		m_pObject->GetBossAudio()->Stop("BossSleep2", false, nullptr);
	}

	// �G�~�b�^�[�j���B
	SINSTANCE(CObjectManager)->DeleteGameObject(m_SleepEmitter);

	// �R�[�X�̑�����Move�ɕύX�B
	vector<Cource::BOSS_COURCE_BLOCK*> now = m_pObject->GetNowCource();
	now[0]->BlockType = Cource::Boss_Cource::BOSS_COURCE_TYPE::Move;
}