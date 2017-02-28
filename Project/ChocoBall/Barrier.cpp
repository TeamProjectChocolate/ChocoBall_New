#include "stdafx.h"
#include "Barrier.h"
#include "CollisionType.h"
#include "ObjectManager.h"

CBarrier::CBarrier()
{
}


CBarrier::~CBarrier()
{
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->RemoveRigidBody_Dynamic(m_pRigidBody.get());
}

void CBarrier::Initialize()
{
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/ball.x");
	CGameObject::Initialize();
	SetRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.0f); 
	m_pModel->m_alpha = 0.0f;
	m_pModel->m_luminance = 0.0f;
	m_pModel->m_Refractive = g_RefractivesTable[REFRACTIVES::GLASS];

	SetAlive(false);
}

void CBarrier::Build(const D3DXVECTOR3& pos,float radius) {
	m_transform.position = pos;
	m_transform.scale = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_MaxRadius = radius;
	// 剛体生成。
	{
		btScalar Radius = radius * 0.5f * 0.5f;
		m_pSphereShape.reset(new btSphereShape(Radius));//ここで剛体の形状を決定
		btTransform rbTransform;
		rbTransform.setIdentity();
		rbTransform.setOrigin(btVector3(m_transform.position.x, m_transform.position.y, m_transform.position.z) + btVector3(0.0f, 0.0f, 1.0f));
		//rbTransform.setRotation(btQuaternion(m_transform.angle.x, m_transform.angle.y, m_transform.angle.z, m_transform.angle.w));

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		m_myMotionState.reset(new btDefaultMotionState(rbTransform));
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, m_myMotionState.get(), m_pSphereShape.get(), btVector3(0, 0, 0));
		m_pRigidBody.reset(new btRigidBody(rbInfo));
		m_pRigidBody->setUserIndex(CollisionType::CollisionType_Boss_Barrier);
		m_pRigidBody->activate();
		//ワールドに追加。
		//SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddCollisionObject_Dynamic();
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
	m_pRigidBody->getWorldTransform().setOrigin(btVector3(m_transform.position.x, m_transform.position.y, m_transform.position.z) + btVector3(0.0f, 0.0f, 1.0f));
	CGameObject::Update();
}

// バリアを切る。
void CBarrier::OffBarrier() {
	if (m_IsBarrierOn) {
		m_IsBarrierOn = false;
		SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->RemoveRigidBody_Dynamic(m_pRigidBody.get());
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
		SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddRigidBody_Dynamic(m_pRigidBody.get());
	}
}
