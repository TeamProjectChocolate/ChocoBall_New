#include "stdafx.h"
#include "CollisionObject.h"
#include "ObjectManager.h"
#include "BulletPhysics.h"



CCollisionObject::CCollisionObject()
{
}

CCollisionObject::~CCollisionObject()
{
	// ��΂ɏ����ȁB
	// �����ʏ��������p���N���X�̃f�X�g���N�^����ɌĂ΂�邽�߁A�����ɏ����B
	// �֎~�����̏ڍׂ͊��N���X�̃f�X�g���N�^�ɋL�ځB
	this->RemoveWorld();
}

void CCollisionObject::Update(D3DXVECTOR3* pos, D3DXQUATERNION* rot) {

}

void CCollisionObject::Build(const btTransform& transform, float mass, bool flg) {
	m_collisionObject.reset(new btCollisionObject);
	m_collisionObject->setCollisionShape(m_collisionShape.get());
	m_collisionObject->setWorldTransform(transform);
}

// ���̃R���W�����I�u�W�F�N�g���R���W�������[���h�ɒǉ�����B
void CCollisionObject::AddWorld() {
	if (m_collisionObject.get()) {
		m_pBulletPhysics->AddCollisionObject(m_collisionObject.get(), m_MyBitGroup, m_LayerMask);
	}
}
// ���̃R���W�����I�u�W�F�N�g���R���W�������[���h����폜����B
void CCollisionObject::RemoveWorld() {
	if (m_collisionObject->getActivationState()) {
		m_pBulletPhysics->RemoveCollisionObject(m_collisionObject.get());
	}
}
