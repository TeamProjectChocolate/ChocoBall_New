#pragma once
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include "GameObject.h"


namespace {
	// フィルターによる衝突ペアの生成を上書き。
	struct FilterCallback_Override : public btOverlapFilterCallback
	{
		// return true when pairs need collision
		virtual bool	needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
		{
			bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
			collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);

			// あたりを無視するかどうかを返却。
			return collides;
		}
	};
}

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

	FilterCallback_Override m_FilterCallback;
	//vector<btCollisionObject*> m_Collisions_Dynamic;	// 物理ワールドに登録されているコリジョン。
	//vector<btCollisionObject*> m_Collisions;			// コリジョンワールドに登録されているコリジョン。
	//CRender* m_DebugRender = nullptr;	// コリジョン描画用レンダー。
public:
	
	void Initialize()override;
	void Update() override;
	void Draw()override;

	/*!
	* @brief	コンストラクタ。
	*/
	CBulletPhysics();
	/*!
	* @brief	デストラクタ。
	*/
	~CBulletPhysics()
	{
		//for (auto rigid : m_Collisions_Dynamic) {
		//	m_dynamicWorld->removeCollisionObject(rigid);
		//}
		//m_Collisions_Dynamic.clear();
		//for (auto collision : m_Collisions) {
		//	m_CollisionWorld->removeCollisionObject(collision);
		//}
		//m_Collisions.clear();

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

	//void InitPysics();

	/*!
	* @brief	剛体を物理ワールドに追加。
	*@param[in]	body		剛体。
	*/
	void AddRigidBody_Dynamic(btRigidBody* body)
	{
		m_dynamicWorld->addRigidBody(body);
		//m_Collisions_Dynamic.push_back(body);
	}
	/*!
	* @brief	剛体を物理ワールドに追加。
	*@param[in]	body		剛体。
	*@param[in]	group		フィルターグループ。
	*@param[in]	mask		フィルターマスク。
	*/
	void AddRigidBody_Dynamic(btRigidBody* body, int group, int mask)
	{
		m_dynamicWorld->addRigidBody(body,group,mask);
		//m_Collisions_Dynamic.push_back(body);
	}
	/*!
	* @brief	剛体を物理ワールドから削除。
	*@param[in]	body		剛体
	*/
	void RemoveRigidBody_Dynamic(btRigidBody* body)
	{
		m_dynamicWorld->removeRigidBody(body);
		//RemoveCollision_Dynamic(body);
	}

	/*!
	* @brief	ゴーストを物理ワールドに追加。
	*@param[in]	ghost		ゴースト
	*/
	void AddCollisionObject_Dynamic(btGhostObject* ghost)
	{
		m_dynamicWorld->addCollisionObject(ghost);
		//m_Collisions_Dynamic.push_back(ghost);
	}
	/*!
	* @brief	ゴーストを物理ワールドに追加。
	*@param[in]	ghost		ゴースト。
	*@param[in]	group		フィルターグループ。
	*@param[in]	mask		フィルターマスク。
	*/
	void AddCollisionObject_Dynamic(btGhostObject* ghost, int group, int mask)
	{
		m_dynamicWorld->addCollisionObject(ghost, group, mask);
		//m_Collisions_Dynamic.push_back(ghost);
	}
	/*!
	* @brief	ゴーストを物理ワールドから削除。
	*@param[in]	body		剛体
	*/
	void RemoveCollisionObject_Dynamic(btGhostObject* ghost)
	{
		m_dynamicWorld->removeCollisionObject(ghost);
		//RemoveCollision_Dynamic(ghost);
	}

	/*!
	* @brief	コリジョンオブジェクトをコリジョンワールドに追加。
	*@param[in]	collision		コリジョン
	*/
	void AddCollisionObject(btCollisionObject* collision)
	{
		m_CollisionWorld->addCollisionObject(collision);
		//m_Collisions.push_back(collision);
	}
	/*!
	* @brief	コリジョンオブジェクトをコリジョンワールドに追加。
	*@param[in]	collision	コリジョン。
	*@param[in]	group		フィルターグループ。
	*@param[in]	mask		フィルターマスク。
	*/
	void AddCollisionObject(btCollisionObject* collision, int group, int mask)
	{
		m_dynamicWorld->addCollisionObject(collision, group, mask);
		//m_Collisions.push_back(collision);
	}
	/*!
	* @brief	コリジョンオブジェクトをコリジョンワールドから削除。
	*@param[in]	collision		コリジョン
	*/
	void RemoveCollisionObject(btCollisionObject* collision)
	{
		m_CollisionWorld->removeCollisionObject(collision);
		//RemoveCollision(collision);
	}



	// 指定した剛体が物理ワールドで何かに接触したか調べる関数。
	// ※この判定は物理ワールドに登録しているものの当たり判定である。
	void ContactTest_Dynamic(
		const btCollisionObject* Collision,
		btCollisionWorld::ContactResultCallback& resultCallback
	)
	{
		// 引数で指定されたコリジョンオブジェクトのフィルターグループとフィルターマスクをコールバックに設定。
		// ※これをしないとコールバックに最初から設定されているデフォルトの値が渡される。
		{
			resultCallback.m_collisionFilterGroup = Collision->getBroadphaseHandle()->m_collisionFilterGroup;
			resultCallback.m_collisionFilterMask = Collision->getBroadphaseHandle()->m_collisionFilterMask;
		}
		m_dynamicWorld->contactTest(const_cast<btCollisionObject*>(Collision), resultCallback);
	}

	// 指定した剛体同士が接触しているか調べる関数。
	void ContactPairTest_Dynamic(
		const btCollisionObject* CollisionA,
		const btCollisionObject* CollisionB,
		btCollisionWorld::ContactResultCallback& resultCallback
	)
	{
		//// 引数で指定されたコリジョンオブジェクトのフィルターグループとフィルターマスクをコールバックに設定。
		//// ※これをしないとコールバックに最初から設定されているデフォルトの値が渡される。
		//{
			//resultCallback.m_collisionFilterGroup = Collision->getBroadphaseHandle()->m_collisionFilterGroup;
			//resultCallback.m_collisionFilterMask = Collision->getBroadphaseHandle()->m_collisionFilterMask;
		//}
		m_dynamicWorld->contactPairTest(const_cast<btCollisionObject*>(CollisionA), const_cast<btCollisionObject*>(CollisionB), resultCallback);
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

	// 移動前のTransform情報と移動後のTransform情報を渡し、その間で当たりが発生したかを調べる。
	// ※この判定はコリジョンワールドに登録しているものの当たり判定である。
	void ConvexSweepTest(
		const btConvexShape* castShape,
		const btTransform& convexFromWorld,
		const btTransform& convexToWorld,
		btCollisionWorld::ConvexResultCallback& resultCallback,
		btScalar allowedCcdPenetration = 0.0f
	)
	{
		m_CollisionWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallback, allowedCcdPenetration);
	}



	btDiscreteDynamicsWorld* GetDynamicWorld() {
		return m_dynamicWorld.get();
	}

	btCollisionWorld* GetCollisionWorld() {
		return m_CollisionWorld.get();
	}

	//// 物理ワールドからコリジョンを削除。
	//void RemoveCollision_Dynamic(btCollisionObject* collision) {
	//	for (auto itr = m_Collisions_Dynamic.begin(); itr != m_Collisions_Dynamic.end();) {
	//		if ((*itr) == collision) {
	//			itr = m_Collisions_Dynamic.erase(itr);
	//		}
	//		else {
	//			itr++;
	//		}
	//	}
	//}

	//// コリジョンワールドからコリジョンを削除。
	//void RemoveCollision(btCollisionObject* collision) {
	//	for (auto itr = m_Collisions.begin(); itr != m_Collisions.end();) {
	//		if ((*itr) == collision) {
	//			itr = m_Collisions.erase(itr);
	//		}
	//		else {
	//			itr++;
	//		}
	//	}
	//}

};


// 物理エンジンを用いた衝突判定用のコールバック。
namespace {
	// コールバック(コリジョンワールド)
	struct SweepResult_Collision : public btCollisionWorld::ConvexResultCallback
	{
		SweepResult_Collision(CGameObject* userPointer) {
			UserPointer = userPointer;
			if (userPointer) {
				this->m_collisionFilterGroup = UserPointer->GetCollisionObject()->getBroadphaseHandle()->m_collisionFilterGroup;
				this->m_collisionFilterMask = UserPointer->GetCollisionObject()->getBroadphaseHandle()->m_collisionFilterMask;
			}
		}
		vector<bool> m_MaskCollisionTypes;	// このクラスのインスタンスを持つオブジェクトが、そのCollisionTypeのあたりを無視するかのフラグを格納(trueなら無視)。
		CGameObject* UserPointer = nullptr;		// isIntersectクラスのインスタンスを保持しているオブジェクトを格納せよ。
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			int idx = convexResult.m_hitCollisionObject->getUserIndex();
			if (m_MaskCollisionTypes[idx]) {
				// マスクの値なら無視。
				return 0.0f;
			}
			if (UserPointer) {
				UserPointer->OnTriggerStay(const_cast<btCollisionObject*>(convexResult.m_hitCollisionObject));
			}
			if (convexResult.m_hitCollisionObject->getUserPointer()) {
				static_cast<CGameObject*>(convexResult.m_hitCollisionObject->getUserPointer())->OnTriggerStay(UserPointer->GetCollisionObject());
			}
			return 0;
		}
	};

	// ワールドの何かと当たったか。
	struct ContactResult : public btCollisionWorld::ContactResultCallback {
		ContactResult()
		{
			isHit = false;
		}
		
		// 衝突点の位置。
		D3DXVECTOR3 hitPoint_A = Vector3::Zero;
		D3DXVECTOR3 hitPoint_B = Vector3::Zero;
		// 衝突点から衝突点への向きベクトル。
		D3DXVECTOR3 Direction_AtoB = Vector3::Zero;
		D3DXVECTOR3 Direction_BtoA = Vector3::Zero;
		// 衝突点と衝突点の距離。
		float Length = 0.0f;

		// 何かのコリジョンに当たったか。
		bool isHit = false;
		// 当たったコリジョンのタイプを格納。
		Collision::Type hitCollisionType_A;
		// 当たったコリジョンのタイプを格納。
		Collision::Type hitCollisionType_B;

		virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
		{
			//if(colObj1Wrap->getCollisionObject()->getUserIndex())
			//if (colObj1Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(CollisionType::Player)
			//	|| colObj1Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(CollisionType::Chocoball)
			//	|| colObj1Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(CollisionType::Enemy))
			//{
			//	//無視。
			//	return 0.0f;
			//}
			//else if (colObj1Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(CollisionType::Camera)) {
			//	//無視。
			//	return 0.0f;
			//}
			//else if (colObj1Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(CollisionType::Bullet) || colObj1Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(CollisionType::Boss)) {
			//	return 0.0f;
			//}

			//int idx = convexResult.m_hitCollisionObject->getUserIndex();
			//if (m_MaskCollisionTypes[idx]) {
			//	// マスクの値なら無視。
			//	return 0.0f;
			//}

			if ((colObj0Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(Collision::Type::AttackWall)) || (colObj1Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(Collision::Type::AttackWall))) {
				OutputDebugString("動く壁。\n");
			}

			void* UserPointer;
			UserPointer = colObj0Wrap->getCollisionObject()->getUserPointer();
			if (UserPointer) {
				static_cast<CGameObject*>(UserPointer)->OnTriggerStay(const_cast<btCollisionObject*>(colObj1Wrap->getCollisionObject()));
			}
			UserPointer = colObj1Wrap->getCollisionObject()->getUserPointer();
			if (UserPointer) {
				static_cast<CGameObject*>(UserPointer)->OnTriggerStay(const_cast<btCollisionObject*>(colObj0Wrap->getCollisionObject()));
			}

			isHit = true;

			// 衝突点を格納。
			//btVector3 point_A = cp.m_positionWorldOnA;
			//btVector3 point_B = cp.m_positionWorldOnB;
			btVector3 point_A = cp.m_localPointA;
			btVector3 point_B = cp.m_localPointB;
			btTransform InvTransformA = colObj0Wrap->getWorldTransform()/*.inverse()*/;
			btTransform InvTransformB = colObj1Wrap->getWorldTransform()/*.inverse()*/;

			point_A = InvTransformA.invXform(point_A);
			point_B = InvTransformB.invXform(point_B);

			D3DXVECTOR3 Vec = D3DXVECTOR3(point_B - point_A);
			Length = D3DXVec3Length(&Vec);
			D3DXVec3Normalize(&Direction_AtoB, &Vec);
			Direction_BtoA = Direction_AtoB * -1.0f;

			//point_A += InvTransformA.getOrigin();
			//point_A = InvTransformA.getBasis().transpose() * point_A;
			//point_B += InvTransformB.getOrigin();
			//point_B = InvTransformB.getBasis().transpose() * point_B;

			hitPoint_A = D3DXVECTOR3(point_A.getX(), point_A.getY(), point_A.getZ());
			hitPoint_B = D3DXVECTOR3(point_B.getX(), point_B.getY(), point_B.getZ());

			// 衝突したコリジョンオブジェクトの属性を格納。
			hitCollisionType_A = static_cast<Collision::Type>(colObj0Wrap->getCollisionObject()->getUserIndex());
			hitCollisionType_B = static_cast<Collision::Type>(colObj1Wrap->getCollisionObject()->getUserIndex());

			return 0.0f;
		}
	};

	// プレイヤー用コールバックY(物理ワールド)
	// ※移動した結果当たったかの判定に用いる。
	struct SweepResult_Y : public btCollisionWorld::ConvexResultCallback
	{
		SweepResult_Y(CGameObject* userPointer,bool isFirstCallBack)
		{
			UserPointer = userPointer;
			if (UserPointer) {
				this->m_collisionFilterGroup = UserPointer->GetCollisionObject()->getBroadphaseHandle()->m_collisionFilterGroup;
				this->m_collisionFilterMask = UserPointer->GetCollisionObject()->getBroadphaseHandle()->m_collisionFilterMask;
			}
			isFirstCallback = isFirstCallBack;
			isHit = false;
			//fMin = FLT_MAX;
		}

		// 何らかのコリジョンと衝突したか。
		bool isHit;
		// 衝突した点。
		D3DXVECTOR3 hitPos = D3DXVECTOR3(0.0f, -FLT_MAX, 0.0f);
		// 衝突した点の法線(Y成分)。
		D3DXVECTOR3 hitNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 
		D3DXVECTOR3 startPos;
		float dist = FLT_MAX;
		// 当たったコリジョンのタイプを格納。
		Collision::Type hitCollisionType;

		vector<bool>  m_MaskCollisionTypes;	// このクラスのインスタンスを持つオブジェクトが、そのCollisionTypeのあたりを無視するかのフラグを格納(trueなら無視)。
		CGameObject* UserPointer = nullptr;		// isIntersectクラスのインスタンスを保持しているオブジェクトを格納せよ。
		bool isFirstCallback = false;		// 一回の当たり判定で最初に呼ばれたコールバックか。

		const btCollisionObject* hitCollisionObject = nullptr;

		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{

			int idx = convexResult.m_hitCollisionObject->getUserIndex();
			if (m_MaskCollisionTypes[idx]) {
				return 0.0f;
			}

			// 衝突点の法線を引っ張ってくる。
			D3DXVECTOR3 hitPointNormal = convexResult.m_hitNormalLocal;
			// 上方向と法線のなす角度を算出。
			float angle = D3DXVec3Dot(&hitPointNormal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			angle = fabsf(acosf(angle));
			if (angle < fPI * 0.3f) {	// 地面の傾斜が54度より小さい場合地面とみなす。
										// 衝突している。
				isHit = true;
				D3DXVECTOR3 hitPosTmp = convexResult.m_hitPointLocal/*convexResult.m_hitCollisionObject->getWorldTransform().invXform(convexResult.m_hitPointLocal)*/;
				// 衝突点の距離を求める。
				D3DXVECTOR3 vDist;
				vDist = hitPosTmp - startPos;
				float fDistTmp = D3DXVec3Length(&vDist);
				if (dist > fDistTmp) {
					// この衝突点のほうが近いので、最も近い衝突点を更新する。
					hitPos = hitPosTmp;
					hitNormal = static_cast<D3DXVECTOR3>(convexResult.m_hitNormalLocal);
					dist = fDistTmp;
				}
			}

			hitCollisionType = static_cast<Collision::Type>(convexResult.m_hitCollisionObject->getUserIndex());
			hitCollisionObject = convexResult.m_hitCollisionObject;

			if (isFirstCallback) {
				// 一度の当たり判定で一度しかこの処理を行わない。
				if (UserPointer) {
					UserPointer->OnCollisionStay(const_cast<btCollisionObject*>(convexResult.m_hitCollisionObject));
				}
				if (convexResult.m_hitCollisionObject->getUserPointer()) {
					static_cast<CGameObject*>(convexResult.m_hitCollisionObject->getUserPointer())->OnCollisionStay(UserPointer->GetCollisionObject());
				}
			}
			return 0;
		}
	};

	// プレイヤー用コールバックXZ(物理ワールド)
	// ※移動した結果当たったかの判定に用いる。
	struct SweepResult_XZ : public btCollisionWorld::ConvexResultCallback
	{
		SweepResult_XZ(CGameObject* userPointer, bool isFirstCallBack)
		{
			UserPointer = userPointer;
			if (UserPointer) {
				this->m_collisionFilterGroup = UserPointer->GetCollisionObject()->getBroadphaseHandle()->m_collisionFilterGroup;
				this->m_collisionFilterMask = UserPointer->GetCollisionObject()->getBroadphaseHandle()->m_collisionFilterMask;
			}
			isFirstCallback = isFirstCallBack;
			isHit = false;
		}

		// 衝突した点の法線(XZ成分)。
		D3DXVECTOR3 hitNormalXZ;
		// 何かのコリジョンに当たったか。
		bool isHit = false;
		// 当たったコリジョンのタイプを格納。
		Collision::Type hitCollisionType;
		// 衝突点。
		D3DXVECTOR3 hitPos;

		vector<bool> m_MaskCollisionTypes;	// このクラスのインスタンスを持つオブジェクトが、そのCollisionTypeのあたりを無視するかのフラグを格納(trueなら無視)。
		CGameObject* UserPointer = nullptr;		// isIntersectクラスのインスタンスを保持しているオブジェクトを格納せよ。
		bool isFirstCallback = false;		// 一回の当たり判定で最初に呼ばれたコールバックか。

		const btCollisionObject* hitCollisionObject = nullptr;

		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (m_MaskCollisionTypes.size() > 0) {
				// マスクが設定されている。
				int idx = convexResult.m_hitCollisionObject->getUserIndex();
				if (m_MaskCollisionTypes[idx]) {
					return 0.0f;
				}
			}

			D3DXVECTOR3 hitPointNormal;
			hitPointNormal.x = convexResult.m_hitNormalLocal.x();
			hitPointNormal.y = convexResult.m_hitNormalLocal.y();
			hitPointNormal.z = convexResult.m_hitNormalLocal.z();

			float d = D3DXVec3Dot(&hitPointNormal, &CVec3Up);
			if (acosf(d) < fPI * 0.2) {
				//ホントは地面かどうかとかの属性を見るのがベストなんだけど、今回は角度で。
				return 0.0f;
			}
			isHit = true;
			//XZ平面での法線。
			hitNormalXZ.x = hitPointNormal.x;
			hitNormalXZ.y = 0.0f;
			hitNormalXZ.z = hitPointNormal.z;
			D3DXVec3Normalize(&hitNormalXZ, &hitNormalXZ);

			hitPos.x = convexResult.m_hitPointLocal.x();
			hitPos.y = convexResult.m_hitPointLocal.y();
			hitPos.z = convexResult.m_hitPointLocal.z();

			hitCollisionType = static_cast<Collision::Type>(convexResult.m_hitCollisionObject->getUserIndex());
			hitCollisionObject = convexResult.m_hitCollisionObject;

			if (isFirstCallback) {
				// 一度の当たり判定で一度しかこの処理を行わない。
				if (UserPointer) {
					UserPointer->OnCollisionStay(const_cast<btCollisionObject*>(convexResult.m_hitCollisionObject));
				}
				if (convexResult.m_hitCollisionObject->getUserPointer()) {
					static_cast<CGameObject*>(convexResult.m_hitCollisionObject->getUserPointer())->OnCollisionStay(UserPointer->GetCollisionObject());
				}
			}
			return 0.0f;
		}
	};


	// カメラ用コールバック
	struct SweepResultGround_Camera : public btCollisionWorld::ConvexResultCallback
	{
		SweepResultGround_Camera()
		{
			isHit = false;
			fMin = FLT_MAX;
		}

		bool isHit;
		D3DXVECTOR3 hitPos;
		D3DXVECTOR3 startPos;
		float fMin;

		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject->getUserIndex() != static_cast<int>(Collision::Type::Map))
			{
				//無視。
				return 0.0f;
			}

			D3DXVECTOR3 hitPointNormal;
			hitPointNormal.x = convexResult.m_hitNormalLocal.x();
			hitPointNormal.y = convexResult.m_hitNormalLocal.y();
			hitPointNormal.z = convexResult.m_hitNormalLocal.z();
			float d = D3DXVec3Dot(&hitPointNormal, &CVec3Up);
			if (d < 0.0f) {
				//当たってない。
				return 0.0f;
			}
			if (acosf(d) > fPI * 0.2) {
				//ホントは地面かどうかとかの属性を見るのがベストなんだけど、今回は角度で。
				return 0.0f;
			}
			isHit = true;
			D3DXVECTOR3 hitPosTmp;
			hitPosTmp.x = convexResult.m_hitPointLocal.x();
			hitPosTmp.y = convexResult.m_hitPointLocal.y();
			hitPosTmp.z = convexResult.m_hitPointLocal.z();
			D3DXVECTOR3 diff;
			diff = hitPosTmp - startPos;
			float len = D3DXVec3Length(&diff);
			if (len < fMin) {
				hitPos = hitPosTmp;
				fMin = len;
			}
			return 0.0f;
		}
	};

	struct SweepResultCeiling_Camera : public btCollisionWorld::ConvexResultCallback {
		SweepResultCeiling_Camera()
		{
			isHit = false;
			fMin = FLT_MAX;
		}

		bool isHit;
		D3DXVECTOR3 hitPos;
		D3DXVECTOR3 startPos;
		float fMin;

		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject->getUserIndex() != static_cast<int>(Collision::Type::Map))
			{
				//無視。
				return 0.0f;
			}
			return 0.0f;
		}
	};
}