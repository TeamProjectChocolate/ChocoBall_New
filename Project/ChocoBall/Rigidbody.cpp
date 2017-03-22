#include "stdafx.h"
#include "Rigidbody.h"
#include "ObjectManager.h"
#include "BulletPhysics.h"



CRigidbody::CRigidbody()
{
	testDelete++;
}

CRigidbody::~CRigidbody()
{
	testDelete--;
	if (testDelete <= 0) {
		OutputDebugString("デストラクタきちんと呼ばれてる。");
	}
	// 絶対に消すな。
	// ※共通処理だが継承クラスのデストラクタが先に呼ばれるため、ここに書く。
	// 禁止事項の詳細は基底クラスのデストラクタに記載。
	//this->RemoveWorld();
}

void CRigidbody::Build(const btTransform& transform, float mass, bool isKinematic) 
{
	m_IsKinematic = isKinematic;
	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	m_myMotionState.reset(new btDefaultMotionState(transform));
	if (isKinematic) {
		mass = 0.0f;
	}
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_myMotionState.get(), m_collisionShape.get(), btVector3(0, 0, 0));
	m_collisionObject.reset(new btRigidBody(rbInfo));

	if (isKinematic) {
		// 剛体をキャラクター剛体(Unityでいうキネマティック?)に設定。
		m_collisionObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	}
}

void CRigidbody::Update(D3DXVECTOR3* pos,D3DXQUATERNION* rot)
{
	if (m_collisionObject.get()) {
		// 剛体が生成されている。
		if (m_IsKinematic) {
			// オブジェクトが物理挙動しない。
			// 受け取ったオブジェクトの位置情報と回転情報を剛体に格納。
			m_collisionObject->getWorldTransform().setOrigin(btVector3(pos->x, pos->y, pos->z) + m_OriginOffset);
			m_collisionObject->getWorldTransform().setRotation(btQuaternion(rot->x, rot->y, rot->z, rot->w));
		}
		else {
			// オブジェクトが物理挙動する。
			// 剛体の位置情報と回転情報を受け取ったオブジェクトの位置情報と回転情報に格納。
			const btVector3& origin = m_collisionObject->getWorldTransform().getOrigin();
			*pos = D3DXVECTOR3(origin.getX(), origin.getY(), origin.getZ());
			*pos -= D3DXVECTOR3(m_OriginOffset);
			const btQuaternion& quat = m_collisionObject->getWorldTransform().getRotation();
			*rot = D3DXQUATERNION(quat.getX(), quat.getY(), quat.getZ(), quat.getW());
		}
	}
}

// この剛体オブジェクトを物理ワールドに追加する。
void CRigidbody::AddWorld(){
	if (m_collisionObject.get()) {
		//m_pBulletPhysics->AddRigidBody_Dynamic(static_cast<btRigidBody*>(m_collisionObject.get()),m_MyBitGroup,m_LayerMask);
		m_pBulletPhysics->AddRigidBody_Dynamic(static_cast<btRigidBody*>(m_collisionObject.get()));
		ConfigCollisionFilterGroup();
		ConfigCollisionFilterMask();
	}
}
// この剛体オブジェクトを物理ワールドから削除する。
void CRigidbody::RemoveWorld(){
	if (m_collisionObject.get()) {
		if (m_pBulletPhysics) {
			m_pBulletPhysics->RemoveRigidBody_Dynamic(static_cast<btRigidBody*>(m_collisionObject.get()));
		}
	}
}
