#include "stdafx.h"
#include "BulletPhysics.h"


CBulletPhysics::CBulletPhysics()
{
	//null�������B
	//m_collisionConfig_Dynamic = nullptr;
	//m_collisionDispatcher_Dynamic = nullptr;
	//m_overlappingPairCache_Dynamic = nullptr;
	//m_constraintSolver = nullptr;
	//m_dynamicWorld = nullptr;
	// �_�C�i�~�b�N���[���h�쐬�B
	{
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		m_collisionConfig_Dynamic.reset(new btDefaultCollisionConfiguration());

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		m_collisionDispatcher_Dynamic.reset(new	btCollisionDispatcher(m_collisionConfig_Dynamic.get()));

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		m_overlappingPairCache_Dynamic.reset(new btDbvtBroadphase());

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		m_constraintSolver.reset(new btSequentialImpulseConstraintSolver);

		m_dynamicWorld.reset(new btDiscreteDynamicsWorld(
			m_collisionDispatcher_Dynamic.get(),
			m_overlappingPairCache_Dynamic.get(),
			m_constraintSolver.get(),
			m_collisionConfig_Dynamic.get()
		));
		m_dynamicWorld->setGravity(btVector3(0, -10, 0));
		//// �t�B���^�[�ɂ��Փ˃y�A�̐������㏑���B
		m_dynamicWorld->getPairCache()->setOverlapFilterCallback(&m_FilterCallback);
	}
	// �R���W�������[���h�쐬�B
	{
		m_CollisionConfig.reset(new btDefaultCollisionConfiguration);
		m_CollisionDispatcher.reset(new btCollisionDispatcher(m_CollisionConfig.get()));
		m_OverlappingPairCache.reset(new btDbvtBroadphase());
		m_CollisionWorld.reset(new btCollisionWorld(m_CollisionDispatcher.get(), m_OverlappingPairCache.get(), m_CollisionConfig.get()));
		//// �t�B���^�[�ɂ��Փ˃y�A�̐������㏑���B
		m_CollisionWorld->getPairCache()->setOverlapFilterCallback(&m_FilterCallback);
	}

	//// �f�o�b�O�p�̃����_�[�𐶐��B
	//m_DebugRender = SINSTANCE(CRenderContext)->SelectRender(RENDER::TYPE::Collision, _T(""), false, nullptr);
}

/*!
* @brief	�������B
*/
void CBulletPhysics::Initialize()
{
	SetAlive(true);
}

void CBulletPhysics::Update()
{
	// �R���W�������[���h�X�V�B
	m_CollisionWorld->updateAabbs();
	// �_�C�i�~�b�N���[���h�X�V�B
	m_dynamicWorld->stepSimulation(1.0f / 60.0f);
}

void CBulletPhysics::Draw() {
	//for (auto Dynamic : m_Collisions_Dynamic) {
	//	// �������[���h�̃R���W�����`��B
	//	btTransform btTr = Dynamic->getWorldTransform();

	//}
	//for (auto Collison : m_Collisions) {
	//	// �R���W�������[���h�̃R���W�����`��B

	//}
}
