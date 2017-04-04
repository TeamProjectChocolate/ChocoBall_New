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
	// プレイヤーをゲームエンドモードにする。
	player->ConfigEndGame();
	// プレイヤーを死ねないようにする。
	player->SetIsPossibleDeath(false);
	// プレイヤーを停止。
	player->SetIsStop(true);

	// ボスをクリア演出用の位置に移動。
	m_pObject->SetPos(m_pObject->GetPos() + -(m_pObject->GetDirection()) * 10.0f);

	// クリア演出用のカメラ作成。
	m_ClearCamera = SINSTANCE(CObjectManager)->GenerationObject<CGameCamera>(_T("ClearCamera"), OBJECT::PRIORTY::CONFIG, false);
	m_ClearCamera->Initialize();
	// カメラの位置情報を変更。
	m_ClearCamera->SetPos(m_pObject->GetPos() + D3DXVECTOR3(10.0f, 4.0f, 14.0f)/*D3DXVECTOR3(10.0f, 8.0f, 14.0f)*/);
	// カメラをボスに向ける。
	m_ClearCamera->SetTarget(m_pObject->GetPos() + +D3DXVECTOR3(0.0f, -6.0f, 0.0f));
}

bool CDeathState::Update() {
	// カメラをボスに向ける。
	m_ClearCamera->SetTarget(m_pObject->GetPos());

	float mass = 1.0f;

	switch (m_DeathState) {
	case DeathState::Dummy:
		if (m_TimeCounter >= m_Interval) {
			m_TimeCounter = 0.0f;

			// 演出用チョコボール生成。
			m_pObject->GetAudio()->PlayCue("Chocoball", true, nullptr);//チョコ落下Audio
			CCBManager* CBM = SINSTANCE(CObjectManager)->GenerationObject<CCBManager>(_T("BossDamageChocoBall"), OBJECT::PRIORTY::OBJECT3D, false);
			CBM->Initialize();
			CBM->SetStartPosition(m_pObject->GetPos() + m_ChocoBallPositions.back());
			CBM->SetEndPosition(m_pObject->GetPos());
			CBM->SetIsBossDamage(false);
			CBM->SetAlive(true);
			m_ChocoBallPositions.pop_back();

			if (m_ChocoBallPositions.size() <= 0) {
				// チョコボールを回数分発生させた。
				m_DeathState = DeathState::Favorte;
			}
		}

		break;
	case DeathState::Favorte:
		if (m_TimeCounter >= m_Interval) {
			m_TimeCounter = 0.0f;

			// ボスの頭上にチョコボール生成(自爆)。
			m_pObject->GetAudio()->PlayCue("Chocoball", true, nullptr);//チョコ落下Audio
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
			// ボス悲鳴再生。
			m_pObject->GetBossAudio()->Play("BossDamageVoice3", false, this);

			m_TimeCounter = 0.0f;

			// 物理挙動に任せる。
			static_cast<CRigidbody*>(m_pObject->GetCollision())->OnDynamic();
			static_cast<CRigidbody*>(m_pObject->GetCollision())->SetMassProps(mass, Vector3::One/*D3DXVECTOR3(0.1f, 0.1f, 0.1f)*/);

			m_DeathState = DeathState::Interval;
		}
		break;
	case DeathState::Interval:
		if (m_TimeCounter >= m_Interval) {
			CPlayer* player = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));
			// ゲームクリアにする。
			player->RequestGameClear();
			m_DeathState = DeathState::GameEnd;
		}
		break;
	case DeathState::GameEnd:
		// ボス悲鳴停止。
		m_pObject->GetBossAudio()->Stop("BossDamageVoice3", false, this);

		break;
	}
	m_TimeCounter += 1.0f / 60.0f;

	return false;
}

void CDeathState::Exit(CEnemy_Boss::BOSS_STATE) {

}
