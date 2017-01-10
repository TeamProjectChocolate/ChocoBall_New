#include "stdafx.h"
#include "EnemyBase.h"
#include "islntersect.h"
#include "EnemyManager.h"
#include "ObjectManager.h"
#include "ActreLight.h"

void EnemyBase::Build(){
	m_pEnemyManager = SINSTANCE(CObjectManager)->FindGameObject<CEnemyManager>(_T("EnemyManager"));
}

void EnemyBase::Initialize(){
	UseModel<C3DImage>();
	SINSTANCE(CShadowRender)->Entry(this);
}

void EnemyBase::Update(){
	// アニメーション再生関数を呼び出す
	m_pModel->SetCurrentAnimNo(m_AnimState);
	if (m_PlayingState == PLAYING_STATE::REPEAT){
		m_pModel->GetAnimation()->PlayAnimation(m_pModel->GetCurrentAnimNo(), 0.1f);

	}
	else if (m_PlayingState == PLAYING_STATE::ONCE){
		m_pModel->GetAnimation()->PlayAnimation_Once(m_pModel->GetCurrentAnimNo(), 0.1f);
	}
	CGameObject::Update();
}

void EnemyBase::OnDestroy()
{
	SINSTANCE(CShadowRender)->DeleteObjectImidieit(this);
}

void EnemyBase::PlayerBulletHit(D3DXVECTOR3 moveDir)
{
	HitReaction();
}

void EnemyBase::ConfigLight() {
	if (!m_pLight) {
		m_pLight = new CActreLight;
	}
	CGameObject::ConfigLight();
}