#include "stdafx.h"
#include "Rigidbody.h"
#include "Objectmanager.h"

CRigidbody::CRigidbody()
{

}

CRigidbody::~CRigidbody()
{

}

void CRigidbody::Initialize(D3DXVECTOR3* m_position, D3DXVECTOR3* size)
{
	D3DXMatrixIdentity(&matWorld);
	m_life = 0.0f;
	Build(*size, *m_position);
	m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
}
void CRigidbody::Initialize(D3DXVECTOR3* m_position, float radius)
{
	D3DXMatrixIdentity(&matWorld);
	m_life = 0.0f;
	Build(radius, *m_position);
	m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
}
void CRigidbody::Update(D3DXVECTOR3* m_position)
{
	//物理エンジンで計算された剛体の位置と回転を反映させる。
	const btVector3& rPos = m_rigidBody->getWorldTransform().getOrigin();
	const btQuaternion& rRot = m_rigidBody->getWorldTransform().getRotation();
	m_position->x = rPos.x();
	m_position->y = rPos.y();
	m_position->z = rPos.z();
	//D3DXVECTOR3 axis(0.707f, -0.707f, 0.0f);
	//D3DXQuaternionRotationAxis(&m_rotation, &axis, D3DXToRadian(45.0f));
	m_rotation.x = rRot.x();
	m_rotation.y = rRot.y();
	m_rotation.z = rRot.z();
	m_rotation.w = rRot.w();
}

void CRigidbody::Draw()
{
	D3DXMATRIX rotMatrix;
	D3DXMatrixRotationQuaternion(
		&rotMatrix,
		&m_rotation
		);
	D3DXMatrixMultiply(&matWorld, &rotMatrix, &matWorld);
}

void CRigidbody::OnDestroy()
{
	if (m_rigidBody){
		SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->RemoveRigidBody(m_rigidBody);
	}
	delete m_myMotionState;
	delete m_collisionShape;
	delete m_rigidBody;
	m_myMotionState = NULL;
	m_collisionShape = NULL;
	m_rigidBody = NULL;
}
/*!
*@brief	構築処理。
*@param[in]	size	箱のサイズ。
*@param[in]	pos		箱の座標。
*/
void CRigidbody::Build(const D3DXVECTOR3& size, const D3DXVECTOR3& pos)
{
	//この引数に渡すのはボックスhalfsizeなので、0.5倍する。
	m_collisionShape = new btBoxShape(btVector3(size.x*0.5f, size.y*0.5f, size.z*0.5f));
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	float mass = 1.0f;

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	m_myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_myMotionState, m_collisionShape, btVector3(0, 0, 0));
	m_rigidBody = new btRigidBody(rbInfo);
	m_rigidBody->setUserIndex(1);
	//ワールドに追加。
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddRigidBody(m_rigidBody);

}
/*!
*@brief	構築処理。
*@param[in]	radius	球の半径。
*@param[in]	pos		球の座標。
*/
void CRigidbody::Build(float radius, const D3DXVECTOR3& pos)
{
	//この引数に渡すのはボックスhalfsizeなので、0.5倍する。
	m_collisionShape = new btSphereShape(radius);
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	float mass = 1.0f;

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	m_myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_myMotionState, m_collisionShape, btVector3(0, 0, 0));
	m_rigidBody = new btRigidBody(rbInfo);
	m_rigidBody->setUserIndex(1);
	//ワールドに追加。
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddRigidBody(m_rigidBody);

}