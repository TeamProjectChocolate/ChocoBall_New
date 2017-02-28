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
	// ダイナミックワールド用(物理演算を行うワールド)。
	unique_ptr<btDefaultCollisionConfiguration>		m_collisionConfig_Dynamic;		//!<コリジョン設定。
	unique_ptr<btCollisionDispatcher>					m_collisionDispatcher_Dynamic;	//!<衝突解決処理。
	unique_ptr<btBroadphaseInterface>					m_overlappingPairCache_Dynamic;	//!<ブロードフェーズ。衝突判定の枝切り。
	unique_ptr<btSequentialImpulseConstraintSolver>	m_constraintSolver;		//!<コンストレイントソルバー。拘束条件の解決処理。
	unique_ptr<btDiscreteDynamicsWorld>				m_dynamicWorld;			//!<ダイナミックワールド。	
	// コリジョンワールド用(物理演算は行わず当たり判定のみを行うワールド)。
	unique_ptr<btCollisionWorld>			m_CollisionWorld;
	unique_ptr<btDefaultCollisionConfiguration>	m_CollisionConfig;
	unique_ptr<btCollisionDispatcher>		m_CollisionDispatcher;	//!<衝突解決処理。
	unique_ptr<btBroadphaseInterface>		m_OverlappingPairCache;	//!<ブロードフェーズ。衝突判定の枝切り。

public:
	
	void Initialize()override;
	void Update() override;
	void Draw()override{};

	/*!
	* @brief	コンストラクタ。
	*/
	CBulletPhysics();
	/*!
	* @brief	デストラクタ。
	*/
	~CBulletPhysics()
	{
		// 物理ワールド。
		m_dynamicWorld.reset(nullptr);
		m_constraintSolver.reset(nullptr);
		m_overlappingPairCache_Dynamic.reset(nullptr);
		m_collisionDispatcher_Dynamic.reset(nullptr);
		m_collisionConfig_Dynamic.reset(nullptr);
		// コリジョンワールド。
		m_CollisionWorld.reset(nullptr);
		m_OverlappingPairCache.reset(nullptr);
		m_CollisionDispatcher.reset(nullptr);
		m_CollisionConfig.reset(nullptr);
	}

	void InitPysics();

	/*!
	* @brief	剛体を物理ワールドに追加。
	*@param[in]	body		剛体
	*/
	void AddRigidBody_Dynamic(btRigidBody* body)
	{
		m_dynamicWorld->addRigidBody(body);
	}
	/*!
	* @brief	剛体を物理ワールドから削除。
	*@param[in]	body		剛体
	*/
	void RemoveRigidBody_Dynamic(btRigidBody* body)
	{
		m_dynamicWorld->removeRigidBody(body);
	}
	/*!
	* @brief	コリジョンオブジェクトを物理ワールドに追加。
	*@param[in]	body		剛体
	*/
	void AddCollisionObject_Dynamic(btGhostObject* gost)
	{
		m_dynamicWorld->addCollisionObject(gost);
	}
	/*!
	* @brief	コリジョンオブジェクトを物理ワールドから削除。
	*@param[in]	body		剛体
	*/
	void RemoveCollisionObject_Dynamic(btGhostObject* gost)
	{
		m_dynamicWorld->removeCollisionObject(gost);
	}


	/*!
	* @brief	コリジョンオブジェクトをコリジョンワールドに追加。
	*@param[in]	body		剛体
	*/
	void AddCollisionObject(btCollisionObject* gost)
	{
		m_CollisionWorld->addCollisionObject(gost);
	}
	/*!
	* @brief	コリジョンオブジェクトをコリジョンワールドから削除。
	*@param[in]	body		剛体
	*/
	void RemoveCollisionObject(btCollisionObject* collision)
	{
		m_CollisionWorld->removeCollisionObject(collision);
	}

	// 移動前のTransform情報と移動後のTransform情報を渡し、その間で当たりが発生したかを調べる。
	// ※この判定は物理ワールドに登録しているものの当たり判定である。
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

