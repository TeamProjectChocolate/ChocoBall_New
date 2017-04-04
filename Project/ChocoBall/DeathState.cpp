#include "stdafx.h"
#include "DeathState.h"
#include "ObjectManager.h"
#include "CBManager.h"

CDeathState::~CDeathState()
{
}

void CDeathState::Entry() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::Wait);
	m_pObject->SetPlayingState(ANIMATION::PLAYING_STATE::REPEAT);

	m_pObject->GetBarrier()->OffBarrier();

	m_DeathState = DeathState::Dummy;
	m_TimeCounter = 0.0f;
	m_Interval = 1.0f;

	m_ChocoBallPositions.push_back(D3DXVECTOR3(-3.0f, 3.0f, 0.0f));
	m_ChocoBallPositions.push_back(D3DXVECTOR3(0.0f, 5.0f, -6.0f));
	m_ChocoBallPositions.push_back(D3DXVECTOR3(6.0f, 3.0f, 6.0f));
	CPlayer* player = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));
	// �v���C���[���Q�[���G���h���[�h�ɂ���B
	player->ConfigEndGame();
	// �v���C���[�����˂Ȃ��悤�ɂ���B
	player->SetIsPossibleDeath(false);
	// �v���C���[���~�B
	player->SetIsStop(true);

	// �{�X���N���A���o�p�̈ʒu�Ɉړ��B
	m_pObject->SetPos(m_pObject->GetPos() + -(m_pObject->GetDirection()) * 10.0f);

	// �N���A���o�p�̃J�����쐬�B
	m_ClearCamera = SINSTANCE(CObjectManager)->GenerationObject<CGameCamera>(_T("ClearCamera"), OBJECT::PRIORTY::CONFIG, false);
	m_ClearCamera->Initialize();
	// �J�����̈ʒu����ύX�B
	m_ClearCamera->SetPos(m_pObject->GetPos() + D3DXVECTOR3(10.0f, 4.0f, 14.0f)/*D3DXVECTOR3(10.0f, 8.0f, 14.0f)*/);
	// �J�������{�X�Ɍ�����B
	m_ClearCamera->SetTarget(m_pObject->GetPos() + +D3DXVECTOR3(0.0f, -6.0f, 0.0f));
}

bool CDeathState::Update() {
	// �J�������{�X�Ɍ�����B
	m_ClearCamera->SetTarget(m_pObject->GetPos());

	float mass = 1.0f;

	switch (m_DeathState) {
	case DeathState::Dummy:
		if (m_TimeCounter >= m_Interval) {
			m_TimeCounter = 0.0f;

			// ���o�p�`���R�{�[�������B
			m_pObject->GetAudio()->PlayCue("Chocoball", true, nullptr);//�`���R����Audio
			CCBManager* CBM = SINSTANCE(CObjectManager)->GenerationObject<CCBManager>(_T("BossDamageChocoBall"), OBJECT::PRIORTY::OBJECT3D, false);
			CBM->Initialize();
			CBM->SetStartPosition(m_pObject->GetPos() + m_ChocoBallPositions.back());
			CBM->SetEndPosition(m_pObject->GetPos());
			CBM->SetIsBossDamage(false);
			CBM->SetAlive(true);
			m_ChocoBallPositions.pop_back();

			if (m_ChocoBallPositions.size() <= 0) {
				// �`���R�{�[�����񐔕������������B
				m_DeathState = DeathState::Favorte;
			}
		}

		break;
	case DeathState::Favorte:
		if (m_TimeCounter >= m_Interval) {
			m_TimeCounter = 0.0f;

			// �{�X�̓���Ƀ`���R�{�[������(����)�B
			m_pObject->GetAudio()->PlayCue("Chocoball", true, nullptr);//�`���R����Audio
			CCBManager* CBM = SINSTANCE(CObjectManager)->GenerationObject<CCBManager>(_T("BossDamageChocoBall"), OBJECT::PRIORTY::OBJECT3D, false);
			CBM->Initialize();
			CBM->SetStartPosition(m_pObject->GetPos() + D3DXVECTOR3(0.0f, 6.0f, 0.0f));
			CBM->SetEndPosition(m_pObject->GetPos());
			CBM->SetIsBossDamage(false);
			CBM->SetAlive(true);

			m_DeathState = DeathState::OnDynamic;
		}
		break;
	case DeathState::OnDynamic:
		if (m_TimeCounter >= m_Interval) {
			// �{�X�ߖĐ��B
			m_pObject->GetBossAudio()->Play("BossDamageVoice3", false, this);

			m_TimeCounter = 0.0f;

			// ���������ɔC����B
			static_cast<CRigidbody*>(m_pObject->GetCollision())->OnDynamic();
			static_cast<CRigidbody*>(m_pObject->GetCollision())->SetMassProps(mass, Vector3::One/*D3DXVECTOR3(0.1f, 0.1f, 0.1f)*/);

			m_DeathState = DeathState::Interval;
		}
		break;
	case DeathState::Interval:
		if (m_TimeCounter >= m_Interval) {
			CPlayer* player = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));
			// �Q�[���N���A�ɂ���B
			player->RequestGameClear();
			m_DeathState = DeathState::GameEnd;
		}
		break;
	case DeathState::GameEnd:
		// �{�X�ߖ�~�B
		m_pObject->GetBossAudio()->Stop("BossDamageVoice3", false, this);

		break;
	}
	m_TimeCounter += 1.0f / 60.0f;

	return false;
}

void CDeathState::Exit(CEnemy_Boss::BOSS_STATE) {

}
