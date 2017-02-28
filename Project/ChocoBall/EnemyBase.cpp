#include "stdafx.h"
#include "EnemyBase.h"
#include "islntersect.h"
#include "EnemyManager.h"
#include "ObjectManager.h"
#include "ActreLight.h"

void EnemyBase::Build(){
	m_pEnemyManager = SINSTANCE(CObjectManager)->FindGameObject<CEnemyManager>(_T("EnemyManager"));
}

void EnemyBase::SetInitPosition(D3DXVECTOR3 pos)
{
	m_initPosition = pos;
	m_transform.position = pos;
}

void EnemyBase::Initialize(){
	UseModel<C3DImage>();
	SINSTANCE(CShadowRender)->Entry(this);

	m_transform.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SetRotation(D3DXVECTOR3(0, 1, 0), 0.1f);
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void EnemyBase::Update(){
	// アニメーション再生関数を呼び出す
	m_pModel->SetCurrentAnimNo(m_AnimState);
	if (m_PlayingState == PLAYING_STATE::REPEAT){
		m_pModel->GetAnimation()->Play(m_pModel->GetCurrentAnimNo(), 0.1f,true);

	}
	else if (m_PlayingState == PLAYING_STATE::ONCE){
		m_pModel->GetAnimation()->Play(m_pModel->GetCurrentAnimNo(), 0.1f,false);
	}
	CGameObject::Update();
}

void EnemyBase::OnDestroy()
{
	SINSTANCE(CShadowRender)->DeleteObjectImidieit(this);
}

void EnemyBase::PlayerBulletHit(D3DXVECTOR3 moveDir)
{
	HitReaction(moveDir);
}

void EnemyBase::ConfigLight() {
	if (!m_pLight) {
		m_pLight = new CActreLight;
	}
	CGameObject::ConfigLight();
}