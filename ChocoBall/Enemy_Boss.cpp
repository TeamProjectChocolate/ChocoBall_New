#include "stdafx.h"
#include "Enemy_Boss.h"
#include "ActreLight.h"

CEnemy_Boss::CEnemy_Boss()
{
}


CEnemy_Boss::~CEnemy_Boss()
{
}

void CEnemy_Boss::Initialize() {
	EnemyBase::Initialize();
	m_pModel->SetFileName("image/enem.x");
	CGameObject::Initialize();
	m_AnimState = ENEMY_ANIMATION::Wait;
	m_State = MOVE_STATE::Wait;
	m_pModel->SetUseBorn(true);
	//for (int idx = 0; idx < m_pModel->GetAnimation()->GetNumAnimationSet(); idx++) {
	//	m_pModel->GetAnimation()->SetAnimationEndtime(idx, EnemyAnimationTime[Enemy_ModelType::People][idx]);
	//}
	//m_pModel->GetAnimation()->PlayAnimation(-1, 0.0f);
	ConfigLight();
	SetAlive(true);
	m_HP = 100;
}

void CEnemy_Boss::Update() {
	switch (m_State) {
	case MOVE_STATE::Wait:
		break;
	case MOVE_STATE::Walk:
		break;
	case MOVE_STATE::RockOn:
		break;
	case MOVE_STATE::Fly:
		break;
	}
	EnemyBase::Update();
}

void CEnemy_Boss::Draw() {
	CGameObject::Draw();
}

void CEnemy_Boss::HitReaction() {
	m_HP -= 10;
}

void CEnemy_Boss::ConfigLight() {
	EnemyBase::ConfigLight();

	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(0, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(1, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(2, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(3, D3DXVECTOR3(1.0f, 0.0f, 0.0f));

	//static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(1, D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	//static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(2, D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	//static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(3, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

	m_pLight->SetDiffuseLightColor(0, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pLight->SetDiffuseLightColor(1, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pLight->SetDiffuseLightColor(2, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pLight->SetDiffuseLightColor(3, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

	m_pLight->SetLimLightDirection(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	m_pLight->SetLimLightColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

	m_pLight->SetAmbientLight(D3DXVECTOR3(0.1f, 0.1f, 0.1f));
}