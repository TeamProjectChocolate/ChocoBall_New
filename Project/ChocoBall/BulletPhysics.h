#pragma once
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include "GameObject.h"


namespace {
	// �t�B���^�[�ɂ��Փ˃y�A�̐������㏑���B
	struct FilterCallback_Override : public btOverlapFilterCallback
	{
		// return true when pairs need collision
		virtual bool	needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
		{
			bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
			collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);

			// ������𖳎����邩�ǂ�����ԋp�B
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

	FilterCallback_Override m_FilterCallback;
	//vector<btCollisionObject*> m_Collisions_Dynamic;	// �������[���h�ɓo�^����Ă���R���W�����B
	//vector<btCollisionObject*> m_Collisions;			// �R���W�������[���h�ɓo�^����Ă���R���W�����B
	//CRender* m_DebugRender = nullptr;	// �R���W�����`��p�����_�[�B
public:
	
	void Initialize()override;
	void Update() override;
	void Draw()override;

	/*!
	* @brief	�R���X�g���N�^�B
	*/
	CBulletPhysics();
	/*!
	* @brief	�f�X�g���N�^�B
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

	//void InitPysics();

	/*!
	* @brief	���̂𕨗����[���h�ɒǉ��B
	*@param[in]	body		���́B
	*/
	void AddRigidBody_Dynamic(btRigidBody* body)
	{
		m_dynamicWorld->addRigidBody(body);
		//m_Collisions_Dynamic.push_back(body);
	}
	/*!
	* @brief	���̂𕨗����[���h�ɒǉ��B
	*@param[in]	body		���́B
	*@param[in]	group		�t�B���^�[�O���[�v�B
	*@param[in]	mask		�t�B���^�[�}�X�N�B
	*/
	void AddRigidBody_Dynamic(btRigidBody* body, int group, int mask)
	{
		m_dynamicWorld->addRigidBody(body,group,mask);
		//m_Collisions_Dynamic.push_back(body);
	}
	/*!
	* @brief	���̂𕨗����[���h����폜�B
	*@param[in]	body		����
	*/
	void RemoveRigidBody_Dynamic(btRigidBody* body)
	{
		m_dynamicWorld->removeRigidBody(body);
		//RemoveCollision_Dynamic(body);
	}

	/*!
	* @brief	�S�[�X�g�𕨗����[���h�ɒǉ��B
	*@param[in]	ghost		�S�[�X�g
	*/
	void AddCollisionObject_Dynamic(btGhostObject* ghost)
	{
		m_dynamicWorld->addCollisionObject(ghost);
		//m_Collisions_Dynamic.push_back(ghost);
	}
	/*!
	* @brief	�S�[�X�g�𕨗����[���h�ɒǉ��B
	*@param[in]	ghost		�S�[�X�g�B
	*@param[in]	group		�t�B���^�[�O���[�v�B
	*@param[in]	mask		�t�B���^�[�}�X�N�B
	*/
	void AddCollisionObject_Dynamic(btGhostObject* ghost, int group, int mask)
	{
		m_dynamicWorld->addCollisionObject(ghost, group, mask);
		//m_Collisions_Dynamic.push_back(ghost);
	}
	/*!
	* @brief	�S�[�X�g�𕨗����[���h����폜�B
	*@param[in]	body		����
	*/
	void RemoveCollisionObject_Dynamic(btGhostObject* ghost)
	{
		m_dynamicWorld->removeCollisionObject(ghost);
		//RemoveCollision_Dynamic(ghost);
	}

	/*!
	* @brief	�R���W�����I�u�W�F�N�g���R���W�������[���h�ɒǉ��B
	*@param[in]	collision		�R���W����
	*/
	void AddCollisionObject(btCollisionObject* collision)
	{
		m_CollisionWorld->addCollisionObject(collision);
		//m_Collisions.push_back(collision);
	}
	/*!
	* @brief	�R���W�����I�u�W�F�N�g���R���W�������[���h�ɒǉ��B
	*@param[in]	collision	�R���W�����B
	*@param[in]	group		�t�B���^�[�O���[�v�B
	*@param[in]	mask		�t�B���^�[�}�X�N�B
	*/
	void AddCollisionObject(btCollisionObject* collision, int group, int mask)
	{
		m_dynamicWorld->addCollisionObject(collision, group, mask);
		//m_Collisions.push_back(collision);
	}
	/*!
	* @brief	�R���W�����I�u�W�F�N�g���R���W�������[���h����폜�B
	*@param[in]	collision		�R���W����
	*/
	void RemoveCollisionObject(btCollisionObject* collision)
	{
		m_CollisionWorld->removeCollisionObject(collision);
		//RemoveCollision(collision);
	}



	// �w�肵�����̂��������[���h�ŉ����ɐڐG���������ׂ�֐��B
	// �����̔���͕������[���h�ɓo�^���Ă�����̂̓����蔻��ł���B
	void ContactTest_Dynamic(
		const btCollisionObject* Collision,
		btCollisionWorld::ContactResultCallback& resultCallback
	)
	{
		// �����Ŏw�肳�ꂽ�R���W�����I�u�W�F�N�g�̃t�B���^�[�O���[�v�ƃt�B���^�[�}�X�N���R�[���o�b�N�ɐݒ�B
		// ����������Ȃ��ƃR�[���o�b�N�ɍŏ�����ݒ肳��Ă���f�t�H���g�̒l���n�����B
		{
			resultCallback.m_collisionFilterGroup = Collision->getBroadphaseHandle()->m_collisionFilterGroup;
			resultCallback.m_collisionFilterMask = Collision->getBroadphaseHandle()->m_collisionFilterMask;
		}
		m_dynamicWorld->contactTest(const_cast<btCollisionObject*>(Collision), resultCallback);
	}

	// �w�肵�����̓��m���ڐG���Ă��邩���ׂ�֐��B
	void ContactPairTest_Dynamic(
		const btCollisionObject* CollisionA,
		const btCollisionObject* CollisionB,
		btCollisionWorld::ContactResultCallback& resultCallback
	)
	{
		//// �����Ŏw�肳�ꂽ�R���W�����I�u�W�F�N�g�̃t�B���^�[�O���[�v�ƃt�B���^�[�}�X�N���R�[���o�b�N�ɐݒ�B
		//// ����������Ȃ��ƃR�[���o�b�N�ɍŏ�����ݒ肳��Ă���f�t�H���g�̒l���n�����B
		//{
			//resultCallback.m_collisionFilterGroup = Collision->getBroadphaseHandle()->m_collisionFilterGroup;
			//resultCallback.m_collisionFilterMask = Collision->getBroadphaseHandle()->m_collisionFilterMask;
		//}
		m_dynamicWorld->contactPairTest(const_cast<btCollisionObject*>(CollisionA), const_cast<btCollisionObject*>(CollisionB), resultCallback);
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

	// �ړ��O��Transform���ƈړ����Transform����n���A���̊Ԃœ����肪�����������𒲂ׂ�B
	// �����̔���̓R���W�������[���h�ɓo�^���Ă�����̂̓����蔻��ł���B
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

	//// �������[���h����R���W�������폜�B
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

	//// �R���W�������[���h����R���W�������폜�B
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


// �����G���W����p�����Փ˔���p�̃R�[���o�b�N�B
namespace {
	// �R�[���o�b�N(�R���W�������[���h)
	struct SweepResult_Collision : public btCollisionWorld::ConvexResultCallback
	{
		SweepResult_Collision(CGameObject* userPointer) {
			UserPointer = userPointer;
			if (userPointer) {
				this->m_collisionFilterGroup = UserPointer->GetCollisionObject()->getBroadphaseHandle()->m_collisionFilterGroup;
				this->m_collisionFilterMask = UserPointer->GetCollisionObject()->getBroadphaseHandle()->m_collisionFilterMask;
			}
		}
		vector<bool> m_MaskCollisionTypes;	// ���̃N���X�̃C���X�^���X�����I�u�W�F�N�g���A����CollisionType�̂�����𖳎����邩�̃t���O���i�[(true�Ȃ疳��)�B
		CGameObject* UserPointer = nullptr;		// isIntersect�N���X�̃C���X�^���X��ێ����Ă���I�u�W�F�N�g���i�[����B
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			int idx = convexResult.m_hitCollisionObject->getUserIndex();
			if (m_MaskCollisionTypes[idx]) {
				// �}�X�N�̒l�Ȃ疳���B
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

	// ���[���h�̉����Ɠ����������B
	struct ContactResult : public btCollisionWorld::ContactResultCallback {
		ContactResult()
		{
			isHit = false;
		}
		
		// �Փ˓_�̈ʒu�B
		D3DXVECTOR3 hitPoint_A = Vector3::Zero;
		D3DXVECTOR3 hitPoint_B = Vector3::Zero;
		// �Փ˓_����Փ˓_�ւ̌����x�N�g���B
		D3DXVECTOR3 Direction_AtoB = Vector3::Zero;
		D3DXVECTOR3 Direction_BtoA = Vector3::Zero;
		// �Փ˓_�ƏՓ˓_�̋����B
		float Length = 0.0f;

		// �����̃R���W�����ɓ����������B
		bool isHit = false;
		// ���������R���W�����̃^�C�v���i�[�B
		Collision::Type hitCollisionType_A;
		// ���������R���W�����̃^�C�v���i�[�B
		Collision::Type hitCollisionType_B;

		virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
		{
			//if(colObj1Wrap->getCollisionObject()->getUserIndex())
			//if (colObj1Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(CollisionType::Player)
			//	|| colObj1Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(CollisionType::Chocoball)
			//	|| colObj1Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(CollisionType::Enemy))
			//{
			//	//�����B
			//	return 0.0f;
			//}
			//else if (colObj1Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(CollisionType::Camera)) {
			//	//�����B
			//	return 0.0f;
			//}
			//else if (colObj1Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(CollisionType::Bullet) || colObj1Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(CollisionType::Boss)) {
			//	return 0.0f;
			//}

			//int idx = convexResult.m_hitCollisionObject->getUserIndex();
			//if (m_MaskCollisionTypes[idx]) {
			//	// �}�X�N�̒l�Ȃ疳���B
			//	return 0.0f;
			//}

			if ((colObj0Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(Collision::Type::AttackWall)) || (colObj1Wrap->getCollisionObject()->getUserIndex() == static_cast<int>(Collision::Type::AttackWall))) {
				OutputDebugString("�����ǁB\n");
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

			// �Փ˓_���i�[�B
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

			// �Փ˂����R���W�����I�u�W�F�N�g�̑������i�[�B
			hitCollisionType_A = static_cast<Collision::Type>(colObj0Wrap->getCollisionObject()->getUserIndex());
			hitCollisionType_B = static_cast<Collision::Type>(colObj1Wrap->getCollisionObject()->getUserIndex());

			return 0.0f;
		}
	};

	// �v���C���[�p�R�[���o�b�NY(�������[���h)
	// ���ړ��������ʓ����������̔���ɗp����B
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

		// ���炩�̃R���W�����ƏՓ˂������B
		bool isHit;
		// �Փ˂����_�B
		D3DXVECTOR3 hitPos = D3DXVECTOR3(0.0f, -FLT_MAX, 0.0f);
		// �Փ˂����_�̖@��(Y����)�B
		D3DXVECTOR3 hitNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 
		D3DXVECTOR3 startPos;
		float dist = FLT_MAX;
		// ���������R���W�����̃^�C�v���i�[�B
		Collision::Type hitCollisionType;

		vector<bool>  m_MaskCollisionTypes;	// ���̃N���X�̃C���X�^���X�����I�u�W�F�N�g���A����CollisionType�̂�����𖳎����邩�̃t���O���i�[(true�Ȃ疳��)�B
		CGameObject* UserPointer = nullptr;		// isIntersect�N���X�̃C���X�^���X��ێ����Ă���I�u�W�F�N�g���i�[����B
		bool isFirstCallback = false;		// ���̓����蔻��ōŏ��ɌĂ΂ꂽ�R�[���o�b�N���B

		const btCollisionObject* hitCollisionObject = nullptr;

		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{

			int idx = convexResult.m_hitCollisionObject->getUserIndex();
			if (m_MaskCollisionTypes[idx]) {
				return 0.0f;
			}

			// �Փ˓_�̖@�������������Ă���B
			D3DXVECTOR3 hitPointNormal = convexResult.m_hitNormalLocal;
			// ������Ɩ@���̂Ȃ��p�x���Z�o�B
			float angle = D3DXVec3Dot(&hitPointNormal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			angle = fabsf(acosf(angle));
			if (angle < fPI * 0.3f) {	// �n�ʂ̌X�΂�54�x��菬�����ꍇ�n�ʂƂ݂Ȃ��B
										// �Փ˂��Ă���B
				isHit = true;
				D3DXVECTOR3 hitPosTmp = convexResult.m_hitPointLocal/*convexResult.m_hitCollisionObject->getWorldTransform().invXform(convexResult.m_hitPointLocal)*/;
				// �Փ˓_�̋��������߂�B
				D3DXVECTOR3 vDist;
				vDist = hitPosTmp - startPos;
				float fDistTmp = D3DXVec3Length(&vDist);
				if (dist > fDistTmp) {
					// ���̏Փ˓_�̂ق����߂��̂ŁA�ł��߂��Փ˓_���X�V����B
					hitPos = hitPosTmp;
					hitNormal = static_cast<D3DXVECTOR3>(convexResult.m_hitNormalLocal);
					dist = fDistTmp;
				}
			}

			hitCollisionType = static_cast<Collision::Type>(convexResult.m_hitCollisionObject->getUserIndex());
			hitCollisionObject = convexResult.m_hitCollisionObject;

			if (isFirstCallback) {
				// ��x�̓����蔻��ň�x�������̏������s��Ȃ��B
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

	// �v���C���[�p�R�[���o�b�NXZ(�������[���h)
	// ���ړ��������ʓ����������̔���ɗp����B
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

		// �Փ˂����_�̖@��(XZ����)�B
		D3DXVECTOR3 hitNormalXZ;
		// �����̃R���W�����ɓ����������B
		bool isHit = false;
		// ���������R���W�����̃^�C�v���i�[�B
		Collision::Type hitCollisionType;
		// �Փ˓_�B
		D3DXVECTOR3 hitPos;

		vector<bool> m_MaskCollisionTypes;	// ���̃N���X�̃C���X�^���X�����I�u�W�F�N�g���A����CollisionType�̂�����𖳎����邩�̃t���O���i�[(true�Ȃ疳��)�B
		CGameObject* UserPointer = nullptr;		// isIntersect�N���X�̃C���X�^���X��ێ����Ă���I�u�W�F�N�g���i�[����B
		bool isFirstCallback = false;		// ���̓����蔻��ōŏ��ɌĂ΂ꂽ�R�[���o�b�N���B

		const btCollisionObject* hitCollisionObject = nullptr;

		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (m_MaskCollisionTypes.size() > 0) {
				// �}�X�N���ݒ肳��Ă���B
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
				//�z���g�͒n�ʂ��ǂ����Ƃ��̑���������̂��x�X�g�Ȃ񂾂��ǁA����͊p�x�ŁB
				return 0.0f;
			}
			isHit = true;
			//XZ���ʂł̖@���B
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
				// ��x�̓����蔻��ň�x�������̏������s��Ȃ��B
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


	// �J�����p�R�[���o�b�N
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
				//�����B
				return 0.0f;
			}

			D3DXVECTOR3 hitPointNormal;
			hitPointNormal.x = convexResult.m_hitNormalLocal.x();
			hitPointNormal.y = convexResult.m_hitNormalLocal.y();
			hitPointNormal.z = convexResult.m_hitNormalLocal.z();
			float d = D3DXVec3Dot(&hitPointNormal, &CVec3Up);
			if (d < 0.0f) {
				//�������ĂȂ��B
				return 0.0f;
			}
			if (acosf(d) > fPI * 0.2) {
				//�z���g�͒n�ʂ��ǂ����Ƃ��̑���������̂��x�X�g�Ȃ񂾂��ǁA����͊p�x�ŁB
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
				//�����B
				return 0.0f;
			}
			return 0.0f;
		}
	};
}