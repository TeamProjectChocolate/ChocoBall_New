#include "stdafx.h"
#include "RushAttackState.h"
#include "BuildBlock.h"
#include "CBManager.h"

CRushAttackState::~CRushAttackState()
{
}

void CRushAttackState::Entry() {
	m_TimeCounter = 0.0f;
	m_SumTimeCounter = 0.0f;
	m_IntervalTime = 1.0f;
	m_AttackTime = 5.0f;
	m_IsFirst = true;
}

bool CRushAttackState::Update() {
	const float deltaTime = 1.0f / 60.0f;
	m_TimeCounter += deltaTime;
	m_SumTimeCounter += deltaTime;
	if (m_TimeCounter >= m_IntervalTime) {
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
		BB->ThrowBlock(m_pObject->GetDirection(), 0.3f);
		m_TimeCounter = 0.0f;
	}
	if (m_SumTimeCounter >= m_AttackTime) {
		if (m_IsFirst) {
			// ボスの頭上にチョコボール生成(自爆)。
			m_pObject->GetAudio()->PlayCue("Chocoball", true, nullptr);//チョコ落下Audio
			CCBManager* CBM = SINSTANCE(CObjectManager)->GenerationObject<CCBManager>(_T("BossDamageChocoBall"), OBJECT::PRIORTY::OBJECT3D, false);
			CBM->Initialize();
			CBM->SetStartPosition(m_pObject->GetPos() + D3DXVECTOR3(0.0f, 3.0f, 0.0f));
			CBM->SetEndPosition(m_pObject->GetPos());
			CBM->FindCource();
			CBM->SetAlive(true);
			m_IsFirst = false;
			m_SumTimeCounter = 0.0f;
			return false;
		}
	}
	return false;
}

void CRushAttackState::Exit(CEnemy_Boss::BOSS_STATE next) {
}