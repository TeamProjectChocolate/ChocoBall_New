#include "stdafx.h"
#include "Barrier.h"
#include "CollisionType.h"
#include "ObjectManager.h"

CBarrier::CBarrier()
{
}


CBarrier::~CBarrier()
{
}

void CBarrier::Initialize()
{
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/ball.x");
	CGameObject::Initialize();
	SetRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.0f); 
	m_pModel->m_alpha = 0.0f;
	m_pModel->m_luminance = 0.0f;
	m_pModel->m_Refractive = FRESNEL::g_RefractivesTable[FRESNEL::REFRACTIVES::GLASS];

	SetAlive(false);
}

void CBarrier::Build(const D3DXVECTOR3& pos,float radius) {
	m_transform.position = pos;
	m_transform.scale = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_MaxRadius = radius;
	// 剛体生成。
	{
		btScalar Radius = radius * 0.5f * 0.5f;
		ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 1.0f),new btSphereShape(Radius), CollisionType::Boss_Barrier,false,0.0f,true,false);
		m_CollisionObject->BitMask_AllOn();
		m_CollisionObject->BitMask_Off(CollisionType::Chocoball);
	}
}

void CBarrier::Update()
{
	if (m_IsBarrierOn) {
		// バリアがオンになった。
		if (m_pModel->GetAlpha() < m_InitAlpha) {
			m_pModel->SetAlpha(m_pModel->GetAlpha() + 0.05f);
		}
		else {
			m_pModel->SetAlpha(m_InitAlpha);
		}
		if (m_Radius < m_MaxRadius) {
			m_Radius += (m_AddRadius * 2.0f);
		}
		else {
			m_Radius = m_MaxRadius;
		}
		m_transform.scale = D3DXVECTOR3(m_Radius, m_Radius, m_Radius);
	}
	else {
		if (m_pModel->GetAlpha() > 0.0f) {
			// アルファが0より大きければ今の値から減算。
			m_pModel->SetAlpha(m_pModel->GetAlpha() - 0.05f);
			m_Radius += m_AddRadius * 2.0f;
			m_transform.scale = D3DXVECTOR3(m_Radius, m_Radius, m_Radius);
		}
		else {
			// バリアが完全に透明になったらバリアオフ。
			SetAlive(false);
		}
	}
	CGameObject::Update();
}

// バリアを切る。
void CBarrier::OffBarrier() {
	if (m_IsBarrierOn) {
		m_IsBarrierOn = false;
		m_CollisionObject->RemoveWorld();
	}
}

// バリア起動。
void CBarrier::OnBarrier() {
	if (!m_IsBarrierOn) {
		m_IsBarrierOn = true;
		SetAlive(true);
		m_Radius = 27.0f;
		m_transform.scale = D3DXVECTOR3(m_Radius, m_Radius, m_Radius);
		m_pModel->SetAlpha(0.0f);
		m_CollisionObject->AddWorld();
	}
}
