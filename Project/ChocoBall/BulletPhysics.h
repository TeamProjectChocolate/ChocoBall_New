#pragma once
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include "GameObject.h"

/*!
* @brief	BulletPhysics
*/

class CBulletPhysics : public CGameObject
{
private:
	// �_�C�i�~�b�N���[���h�p(�������Z���s�����[���h)�B
	unique_ptr<btDefaultCollisionConfiguration>		m_collisionConfig_Dynamic;		//!<�R���W�����ݒ�B
	unique_ptr<btCollisionDispatcher>					m_collisionDispatcher_Dynamic;	//!<�Փˉ��������B
	unique_ptr<btBroadphaseInterface>					m_overlappingPairCache_Dynamic;	//!<�u���[�h�t�F�[�Y�B�Փ˔���̎}�؂�B
	unique_ptr<btSequentialImpulseConstraintSolver>	m_constraintSolver;		//!<�R���X�g���C���g�\���o�[�B�S�������̉��������B
	unique_ptr<btDiscreteDynamicsWorld>				m_dynamicWorld;			//!<�_�C�i�~�b�N���[���h�B	
	// �R���W�������[���h�p(�������Z�͍s�킸�����蔻��݂̂��s�����[���h)�B
	unique_ptr<btCollisionWorld>			m_CollisionWorld;
	unique_ptr<btDefaultCollisionConfiguration>	m_CollisionConfig;
	unique_ptr<btCollisionDispatcher>		m_CollisionDispatcher;	//!<�Փˉ��������B
	unique_ptr<btBroadphaseInterface>		m_OverlappingPairCache;	//!<�u���[�h�t�F�[�Y�B�Փ˔���̎}�؂�B

public:
	
	void Initialize()override;
	void Update() override;
	void Draw()override{};

	/*!
	* @brief	�R���X�g���N�^�B
	*/
	CBulletPhysics();
	/*!
	* @brief	�f�X�g���N�^�B
	*/
	~CBulletPhysics()
	{
		// �������[���h�B
		m_dynamicWorld.reset(nullptr);
		m_constraintSolver.reset(nullptr);
		m_overlappingPairCache_Dynamic.reset(nullptr);
		m_collisionDispatcher_Dynamic.reset(nullptr);
		m_collisionConfig_Dynamic.reset(nullptr);
		// �R���W�������[���h�B
		m_CollisionWorld.reset(nullptr);
		m_OverlappingPairCache.reset(nullptr);
		m_CollisionDispatcher.reset(nullptr);
		m_CollisionConfig.reset(nullptr);
	}

	void InitPysics();

	/*!
	* @brief	���̂𕨗����[���h�ɒǉ��B
	*@param[in]	body		����
	*/
	void AddRigidBody_Dynamic(btRigidBody* body)
	{
		m_dynamicWorld->addRigidBody(body);
	}
	/*!
	* @brief	���̂𕨗����[���h����폜�B
	*@param[in]	body		����
	*/
	void RemoveRigidBody_Dynamic(btRigidBody* body)
	{
		m_dynamicWorld->removeRigidBody(body);
	}
	/*!
	* @brief	�R���W�����I�u�W�F�N�g�𕨗����[���h�ɒǉ��B
	*@param[in]	body		����
	*/
	void AddCollisionObject_Dynamic(btGhostObject* gost)
	{
		m_dynamicWorld->addCollisionObject(gost);
	}
	/*!
	* @brief	�R���W�����I�u�W�F�N�g�𕨗����[���h����폜�B
	*@param[in]	body		����
	*/
	void RemoveCollisionObject_Dynamic(btGhostObject* gost)
	{
		m_dynamicWorld->removeCollisionObject(gost);
	}


	/*!
	* @brief	�R���W�����I�u�W�F�N�g���R���W�������[���h�ɒǉ��B
	*@param[in]	body		����
	*/
	void AddCollisionObject(btCollisionObject* gost)
	{
		m_CollisionWorld->addCollisionObject(gost);
	}
	/*!
	* @brief	�R���W�����I�u�W�F�N�g���R���W�������[���h����폜�B
	*@param[in]	body		����
	*/
	void RemoveCollisionObject(btCollisionObject* collision)
	{
		m_CollisionWorld->removeCollisionObject(collision);
	}

	// �ړ��O��Transform���ƈړ����Transform����n���A���̊Ԃœ����肪�����������𒲂ׂ�B
	// �����̔���͕������[���h�ɓo�^���Ă�����̂̓����蔻��ł���B
	void ConvexSweepTest_Dynamic(
		const btConvexShape* castShape,
		const btTransform& convexFromWorld,
		const btTransform& convexToWorld,
		btCollisionWorld::ConvexResultCallback& resultCallback,
		btScalar allowedCcdPenetration = 0.0f
		)
	{
		m_dynamicWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallback, allowedCcdPenetration);
	}

	btDiscreteDynamicsWorld* GetDynamicWorld() {
		return m_dynamicWorld.get();
	}

	btCollisionWorld* GetCollisionWorld() {
		return m_CollisionWorld.get();
	}
};

