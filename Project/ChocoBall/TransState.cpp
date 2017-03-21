#include "stdafx.h"
#include "TransState.h"
#include "ObjectManager.h"
#include "Player.h"

CTransState::CTransState(CEnemy_Boss* obj):CState(obj){
	m_pPlayer = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));
}

CTransState::~CTransState()
{
}

void CTransState::Entry() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::Swim);
	m_pObject->SetPlayingState(ANIMATION::PLAYING_STATE::REPEAT);
	m_Direction = m_pObject->GetDirection();	// ボスの向き。
	m_InitPos = m_pObject->GetPos();
	m_MoveSpeed = 0.0f;
	m_LengthMax = 0.0f;
	m_IsUseLength = false;
}

bool CTransState::Update() {
	D3DXVECTOR3 pos = m_pObject->GetPos() + (m_Direction * m_MoveSpeed);
	m_pObject->SetPos(pos);
	if (m_IsUseLength) {
		if (D3DXVec3Length(&D3DXVECTOR3(m_InitPos - pos)) >= m_LengthMax) {
			// 一定量移動したらステート終了。
			return true;
		}
	}
	return false;
}

void CTransState::Exit(CEnemy_Boss::BOSS_STATE next) {

}
