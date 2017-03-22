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
		OutputDebugString("�f�X�g���N�^������ƌĂ΂�Ă�B");
	}
	// ��΂ɏ����ȁB
	// �����ʏ��������p���N���X�̃f�X�g���N�^����ɌĂ΂�邽�߁A�����ɏ����B
	// �֎~�����̏ڍׂ͊��N���X�̃f�X�g���N�^�ɋL�ځB
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
		// ���̂��L�����N�^�[����(Unity�ł����L�l�}�e�B�b�N?)�ɐݒ�B
		m_collisionObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	}
}

void CRigidbody::Update(D3DXVECTOR3* pos,D3DXQUATERNION* rot)
{
	if (m_collisionObject.get()) {
		// ���̂���������Ă���B
		if (m_IsKinematic) {
			// �I�u�W�F�N�g�������������Ȃ��B
			// �󂯎�����I�u�W�F�N�g�̈ʒu���Ɖ�]�������̂Ɋi�[�B
			m_collisionObject->getWorldTransform().setOrigin(btVector3(pos->x, pos->y, pos->z) + m_OriginOffset);
			m_collisionObject->getWorldTransform().setRotation(btQuaternion(rot->x, rot->y, rot->z, rot->w));
		}
		else {
			// �I�u�W�F�N�g��������������B
			// ���̂̈ʒu���Ɖ�]�����󂯎�����I�u�W�F�N�g�̈ʒu���Ɖ�]���Ɋi�[�B
			const btVector3& origin = m_collisionObject->getWorldTransform().getOrigin();
			*pos = D3DXVECTOR3(origin.getX(), origin.getY(), origin.getZ());
			*pos -= D3DXVECTOR3(m_OriginOffset);
			const btQuaternion& quat = m_collisionObject->getWorldTransform().getRotation();
			*rot = D3DXQUATERNION(quat.getX(), quat.getY(), quat.getZ(), quat.getW());
		}
	}
}

// ���̍��̃I�u�W�F�N�g�𕨗����[���h�ɒǉ�����B
void CRigidbody::AddWorld(){
	if (m_collisionObject.get()) {
		//m_pBulletPhysics->AddRigidBody_Dynamic(static_cast<btRigidBody*>(m_collisionObject.get()),m_MyBitGroup,m_LayerMask);
		m_pBulletPhysics->AddRigidBody_Dynamic(static_cast<btRigidBody*>(m_collisionObject.get()));
		ConfigCollisionFilterGroup();
		ConfigCollisionFilterMask();
	}
}
// ���̍��̃I�u�W�F�N�g�𕨗����[���h����폜����B
void CRigidbody::RemoveWorld(){
	if (m_collisionObject.get()) {
		if (m_pBulletPhysics) {
			m_pBulletPhysics->RemoveRigidBody_Dynamic(static_cast<btRigidBody*>(m_collisionObject.get()));
		}
	}
}
