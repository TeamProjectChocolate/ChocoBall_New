#include "stdafx.h"
#include "DivisionWall.h"
#include "CollisionType.h"
#include "ObjectManager.h"

CDivisionWall::CDivisionWall()
{
}

CDivisionWall::~CDivisionWall()
{
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->RemoveRigidBody_Dynamic(m_rigidBody.get());
}

void CDivisionWall::Build(const D3DXVECTOR3& pos, const D3DXQUATERNION& Rotation) {
	m_transform.position = pos;
	m_transform.angle = Rotation;
	m_transform.scale = D3DXVECTOR3(5.0f, 6.0f, 1.0f);

	// 剛体生成。
	{
		D3DXVECTOR3 size = D3DXVECTOR3(5.0f,6.0f,1.0f);

		//この引数に渡すのはボックスhalfsizeなので、0.5倍する。
		m_collisionShape.reset(new btBoxShape(btVector3(size.x*0.5f, size.y*0.5f, size.z*0.5f)));
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
		groundTransform.setRotation(btQuaternion(Rotation.x, Rotation.y, Rotation.z, Rotation.w));
		float mass = 0.0f;

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		m_myMotionState.reset(new btDefaultMotionState(groundTransform));
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_myMotionState.get(), m_collisionShape.get(), btVector3(0, 0, 0));
		m_rigidBody.reset(new btRigidBody(rbInfo));
		m_rigidBody->setUserIndex(CollisionType_Wall);
		//ワールドに追加。
		SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddRigidBody_Dynamic(m_rigidBody.get());
		m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
		m_rigidBody->setUserIndex(CollisionType_Wall);
	}
}

void CDivisionWall::Initialize() {
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/BR.x");
	CGameObject::Initialize();
	m_pModel->m_alpha = 0.7f;
	m_pModel->m_luminance = 0.0f;
	m_pModel->m_Refractive = g_RefractivesTable[REFRACTIVES::GLASS];
	m_moveSpeed = 0.5f;

	SetAlive(true);
}

void CDivisionWall::Update() {
	if (m_IsMove) {
		D3DXMATRIX Rota = m_pModel->GetRotation();
		D3DXVECTOR3 Local_X;
		Local_X.x = Rota.m[0][0];
		Local_X.y = Rota.m[0][1];
		Local_X.z = Rota.m[0][2];
		m_transform.position += Local_X * m_moveSpeed;
		m_Length += m_moveSpeed;
		btTransform& trans = m_rigidBody->getWorldTransform();
		trans.setOrigin(btVector3(m_transform.position.x, m_transform.position.y, m_transform.position.z));
		trans.setRotation(btQuaternion(m_transform.angle.x, m_transform.angle.y, m_transform.angle.z));
		if (m_Length > m_transform.scale.x) {
			// オブジェクトの横のサイズ分スライドしたら。
			m_IsMove = false;
		}
	}
	CGameObject::Update();
}

void CDivisionWall::Draw() {
	CGameObject::Draw();
}