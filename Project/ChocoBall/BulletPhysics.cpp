#include "stdafx.h"
#include "BulletPhysics.h"


CBulletPhysics::CBulletPhysics()
{
	//null初期化。
	//m_collisionConfig_Dynamic = nullptr;
	//m_collisionDispatcher_Dynamic = nullptr;
	//m_overlappingPairCache_Dynamic = nullptr;
	//m_constraintSolver = nullptr;
	//m_dynamicWorld = nullptr;
	// ダイナミックワールド作成。
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
		//// フィルターによる衝突ペアの生成を上書き。
		m_dynamicWorld->getPairCache()->setOverlapFilterCallback(&m_FilterCallback);
	}
	// コリジョンワールド作成。
	{
		m_CollisionConfig.reset(new btDefaultCollisionConfiguration);
		m_CollisionDispatcher.reset(new btCollisionDispatcher(m_CollisionConfig.get()));
		m_OverlappingPairCache.reset(new btDbvtBroadphase());
		m_CollisionWorld.reset(new btCollisionWorld(m_CollisionDispatcher.get(), m_OverlappingPairCache.get(), m_CollisionConfig.get()));
		//// フィルターによる衝突ペアの生成を上書き。
		m_CollisionWorld->getPairCache()->setOverlapFilterCallback(&m_FilterCallback);
	}

	//// デバッグ用のレンダーを生成。
	//m_DebugRender = SINSTANCE(CRenderContext)->SelectRender(RENDER::TYPE::Collision, _T(""), false, nullptr);
}

/*!
* @brief	初期化。
*/
void CBulletPhysics::Initialize()
{
	SetAlive(true);
}

void CBulletPhysics::Update()
{
	// コリジョンワールド更新。
	m_CollisionWorld->updateAabbs();
	// ダイナミックワールド更新。
	m_dynamicWorld->stepSimulation(1.0f / 60.0f);
}

void CBulletPhysics::Draw() {
	//for (auto Dynamic : m_Collisions_Dynamic) {
	//	// 物理ワールドのコリジョン描画。
	//	btTransform btTr = Dynamic->getWorldTransform();

	//}
	//for (auto Collison : m_Collisions) {
	//	// コリジョンワールドのコリジョン描画。

	//}
}
