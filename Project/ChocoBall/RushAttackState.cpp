#include "stdafx.h"
#include "RushAttackState.h"
#include "BuildBlock.h"
#include "CBManager.h"

CRushAttackState::~CRushAttackState()
{
}

void CRushAttackState::Entry() {
	m_TimeCounter = 0.0f;
	m_IntervalTime = 0.5f;
	m_AttackTime = 9.0f;
	m_IsFirst = true;
}

bool CRushAttackState::Update() {

	m_TimeCounter += 1.0f / 60.0f;
	if ((m_TimeCounter / m_IntervalTime) - static_cast<int>(m_TimeCounter / m_IntervalTime) <= 0.01f) {
		// 攻撃処理。
		// チョコ壁を生成していく。
		string txt = "B_Block";
		char num[100];
		_itoa(CBuildBlock::GetBBManagerNum(), num, 10);
		txt += num;
		CBuildBlock* BB = SINSTANCE(CObjectManager)->GenerationObject<CBuildBlock>(txt.c_str(), OBJECT::PRIORTY::OBJECT3D, false);
		D3DXVECTOR3 pos = m_pObject->GetPos();
		pos.y -= BLOCK_H * BUILD_H;
		BB->Initialize(
			pos + (m_pObject->GetDirection() * 2.0f),
			m_pObject->GetQuaternion()
		);
		// チョコ壁を飛ばす。
		BB->ThrowBlock(m_pObject->GetDirection(), 0.1f);
	}
	if (m_TimeCounter >= m_AttackTime) {
		if (m_IsFirst) {
			// ボスの頭上にチョコボール生成(自爆)。
			m_pObject->GetAudio()->PlayCue("Chocoball", true, nullptr);//チョコ落下Audio
			CCBManager* CBM = SINSTANCE(CObjectManager)->GenerationObject<CCBManager>(_T("BossDamageChocoBall"), OBJECT::PRIORTY::OBJECT3D, false);
			CBM->Initialize();
			CBM->SetStartPosition(m_pObject->GetPos() + D3DXVECTOR3(0.0f, 3.0f, 0.0f));
			CBM->SetEndPosition(m_pObject->GetPos());
			CBM->SetAlive(true);
			m_IsFirst = false;
			return false;
		}
	}
	return false;
}

void CRushAttackState::Exit(CEnemy_Boss::BOSS_STATE next) {
}