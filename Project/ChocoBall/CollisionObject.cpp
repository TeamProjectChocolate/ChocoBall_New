#include "stdafx.h"
#include "CollisionObject.h"
#include "ObjectManager.h"
#include "BulletPhysics.h"



CCollisionObject::CCollisionObject()
{
}

CCollisionObject::~CCollisionObject()
{
	// 絶対に消すな。
	// ※共通処理だが継承クラスのデストラクタが先に呼ばれるため、ここに書く。
	// 禁止事項の詳細は基底クラスのデストラクタに記載。
	this->RemoveWorld();
}

void CCollisionObject::Update(D3DXVECTOR3* pos, D3DXQUATERNION* rot) {

}

void CCollisionObject::Build(const btTransform& transform, float mass, bool flg) {
	m_collisionObject.reset(new btCollisionObject);
	m_collisionObject->setCollisionShape(m_collisionShape.get());
	m_collisionObject->setWorldTransform(transform);
}

// このコリジョンオブジェクトをコリジョンワールドに追加する。
void CCollisionObject::AddWorld() {
	if (m_collisionObject.get()) {
		m_pBulletPhysics->AddCollisionObject(m_collisionObject.get(), m_MyBitGroup, m_LayerMask);
	}
}
// このコリジョンオブジェクトをコリジョンワールドから削除する。
void CCollisionObject::RemoveWorld() {
	if (m_collisionObject->getActivationState()) {
		m_pBulletPhysics->RemoveCollisionObject(m_collisionObject.get());
	}
}
