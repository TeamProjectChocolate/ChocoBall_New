#pragma once
#include "CollisionInterface.h"

class CRigidbody:public CCollisionInterface
{
public:
	CRigidbody();
	~CRigidbody();

	void Update(D3DXVECTOR3* pos, D3DXQUATERNION* rot)override;

	inline void SetLinearVelocity(const D3DXVECTOR3& vel)
	{
		static_cast<btRigidBody*>(m_collisionObject.get())->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
	}
	inline void SetAngularVelocity(const D3DXVECTOR3& vel)
	{
		static_cast<btRigidBody*>(m_collisionObject.get())->setAngularVelocity(btVector3(vel.x, vel.y, vel.z));
	}
	// ���̂ɗ͂�������B
	// �����͌����t���̗͂̑傫���B
	inline void ApplyForce(const D3DXVECTOR3& force)
	{
		static_cast<btRigidBody*>(m_collisionObject.get())->applyForce(btVector3(force.x, force.y, force.z), btVector3(1.0f, 1.0f, 1.0f));
	}
	// ���̂ɗ͂�������B
	// ����:	�����t���̗͂̑傫���B
	//			???�B
	inline void ApplyForce(const D3DXVECTOR3& force,const D3DXVECTOR3& rel_pos)
	{
		static_cast<btRigidBody*>(m_collisionObject.get())->applyForce(btVector3(force.x, force.y, force.z), btVector3(rel_pos.x, rel_pos.y, rel_pos.z));
	}
	// ���̂Ɏ��ʂƉ�]�̂��₷����ݒ�B
	// �����F	���ʁB
	//			�������͉�]�̂��₷���B
	inline void SetMassProps(const float mass, const D3DXVECTOR3& inertia) {
		static_cast<btRigidBody*>(m_collisionObject.get())->setMassProps(mass,btVector3(inertia.x, inertia.y, inertia.z));
	}
	// ���̂𓮓I���B
	inline void OnDynamic() {
		m_IsKinematic = false;
		m_collisionObject->setCollisionFlags(0);
	}
	// ���̂��L�l�}�e�B�b�N���B
	inline void OnKinematic() {
		m_IsKinematic = true;
		m_collisionObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		m_collisionObject->setActivationState(DISABLE_DEACTIVATION);
	}
	//// ���̂�ÓI���B
	//void OnStatic() {
	//	m_IsKinematic = false;
	//	m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	//}


	// ���̍��̃I�u�W�F�N�g�𕨗����[���h�ɒǉ�����B
	void AddWorld()override;
	// ���̍��̃I�u�W�F�N�g�𕨗����[���h����폜����B
	void RemoveWorld()override;


	bool GetIsKinematic() {
		return m_IsKinematic;
	}
private:
	// �p�����[�^���Q�Ƃ��č��̐����B
	// �����F�I�u�W�F�N�g�̃g�����X�t�H�[�����B
	//		 ����(isKinematic��true�Ȃ�Ύ����I��0.0f�ɂȂ�)�B
	//		 �L�l�}�e�B�b�N���̂ɂ��邩(���̃t���O��true�Ȃ畨�����������Ȃ��Ȃ�)�B
	void Build(
		const btTransform& transform,
		float mass,
		bool isKinematic)override;
private:
	unique_ptr<btDefaultMotionState> m_myMotionState;	// ���[�V�����X�e�[�g�B
	bool m_IsKinematic = false;
};