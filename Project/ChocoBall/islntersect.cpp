#pragma once

#include "stdafx.h"
#include "islntersect.h"
#include "CBManager.h"
#include "Audio.h"


#define ORIGIN_CENTER	//��`�ŋN�_�������B

void GenelateChocoBall(CCBManager* mgr, btGhostObject* m_hitCollisionObject, CAudio* pAudio);

// �v���C���[�p�R�[���o�b�N
struct SweepResult_Y : public btCollisionWorld::ConvexResultCallback
{
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
	CollisionType hitCollisionType;
	CAudio* m_pAudio;

	SweepResult_Y()
	{
		isHit = false;
		//fMin = FLT_MAX;
	}

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_ChocoballTrigger) {

			CCBManager* mgr = (CCBManager*)convexResult.m_hitCollisionObject->getUserPointer();
			GenelateChocoBall(mgr, (btGhostObject*)convexResult.m_hitCollisionObject, m_pAudio);
			return 0.0f;
		}

		if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Player
			|| convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Chocoball
			|| convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Enemy) {
			//�����B
			return 0.0f;
		}
		else if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Camera) {
			//�����B
			return 0.0f;
		}
		else if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Bullet) {
			return 0.0f;
		}

		// �Փ˓X�̖@�������������Ă���B
		D3DXVECTOR3 hitPointNormal = convexResult.m_hitNormalLocal;
		// ������Ɩ@���̂Ȃ��p�x���Z�o�B
		float angle = D3DXVec3Dot(&hitPointNormal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		angle = fabsf(acosf(angle));
		if (angle < fPI * 0.3f) {	// �n�ʂ̌X�΂�54�x��菬�����ꍇ�n�ʂƂ݂Ȃ��B
									// �Փ˂��Ă���B
			isHit = true;
			D3DXVECTOR3 hitPosTmp = convexResult.m_hitPointLocal;
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
		hitCollisionType = static_cast<CollisionType>(convexResult.m_hitCollisionObject->getUserIndex());

		return 0;

		//	float d = D3DXVec3Dot(&hitPointNormal, &CVec3Up);
		//	if (d < 0.0f) {
		//		//�������ĂȂ��B
		//		return 0.0f;
		//	}
		//	if (acosf(d) > fPI * 0.2) {
		//		//�z���g�͒n�ʂ��ǂ����Ƃ��̑���������̂��x�X�g�Ȃ񂾂��ǁA����͊p�x�ŁB
		//		return 0.0f;
		//	}
		//	isHit = true;
		//	D3DXVECTOR3 hitPosTmp;
		//	hitPosTmp.x = convexResult.m_hitPointLocal.x();
		//	hitPosTmp.y = convexResult.m_hitPointLocal.y();
		//	hitPosTmp.z = convexResult.m_hitPointLocal.z();
		//	D3DXVECTOR3 diff;
		//	diff = hitPosTmp - startPos;
		//	float len = D3DXVec3Length(&diff);
		//	if (len < fMin){
		//		hitPos = hitPosTmp;
		//		fMin = len;
		//	}
		//	return 0.0f;
	}
};


struct SweepResult_XZ : public btCollisionWorld::ConvexResultCallback
{
	// �Փ˂����_�̖@��(XZ����)�B
	D3DXVECTOR3 hitNormalXZ;
	// �����̃R���W�����ɓ����������B
	bool isHit = false;
	// ���������R���W�����̃^�C�v���i�[�B
	CollisionType hitCollisionType;
	// �Փ˓_�B
	D3DXVECTOR3 hitPos;
	CAudio* m_pAudio;
	SweepResult_XZ()
	{
		isHit = false;
	}

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_ChocoballTrigger) {
			CCBManager* mgr = (CCBManager*)convexResult.m_hitCollisionObject->getUserPointer();
			GenelateChocoBall(mgr, (btGhostObject*)convexResult.m_hitCollisionObject, m_pAudio);
			return 0.0f;
		}

		if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Player
			|| convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Chocoball
			|| convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Enemy)
		{
			//�����B
			return 0.0f;
		}
		else if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Camera) {
			//�����B
			return 0.0f;
		}
		else if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Bullet) {
			return 0.0f;
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

		btTransform transform = convexResult.m_hitCollisionObject->getWorldTransform();


		hitPos.x = convexResult.m_hitPointLocal.x();
		hitPos.y = convexResult.m_hitPointLocal.y();
		hitPos.z = convexResult.m_hitPointLocal.z();

		hitCollisionType = static_cast<CollisionType>(convexResult.m_hitCollisionObject->getUserIndex());

		return 0.0f;
	}
};


void GenelateChocoBall(CCBManager* mgr, btGhostObject* m_hitCollisionObject, CAudio* pAudio) {
	if (!mgr->GetAlive()) {
		pAudio->PlayCue("Chocoball", true, nullptr);//�`���R����Audio
		mgr->Initialize();
		mgr->SetAlive(true);
		SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->RemoveCollisionObject_Dynamic(m_hitCollisionObject);
	}
};

// �J�����p�R�[���o�b�N
struct SweepResultGround_Camera : public btCollisionWorld::ConvexResultCallback
{
	bool isHit;
	D3DXVECTOR3 hitPos;
	D3DXVECTOR3 startPos;
	float fMin;

	SweepResultGround_Camera()
	{
		isHit = false;
		fMin = FLT_MAX;
	}

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() != CollisionType_Map
			|| convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Enemy)
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
		if (len < fMin){
			hitPos = hitPosTmp;
			fMin = len;
		}
		return 0.0f;
	}
};

struct SweepResultCeiling_Camera : public btCollisionWorld::ConvexResultCallback {
	bool isHit;
	D3DXVECTOR3 hitPos;
	D3DXVECTOR3 startPos;
	float fMin;

	SweepResultCeiling_Camera()
	{
		isHit = false;
		fMin = FLT_MAX;
	}

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() != CollisionType_Map)
		{
			//�����B
			return 0.0f;
		}


		return 0.0f;
	}
};

CIsIntersect::CIsIntersect()
{
	m_isHitGround = false;
	m_Jumpflag = false;
}

CIsIntersect::~CIsIntersect()
{
}

//����(�����蔻��̂��镨��)�̏�����
void CIsIntersect::CollisitionInitialize(D3DXVECTOR3* position,float radius,CollisionType type)
{
	//�R���W�����������B
	m_radius = radius;
	//Box(������),sphere(����)�Ȃǂœ�����͈͂����߂�B
	m_collisionShape = new btSphereShape(m_radius);//�����ō��̂̌`�������

	float mass = 0.1f;

	btTransform rbTransform;
	rbTransform.setIdentity();
	rbTransform.setOrigin(btVector3(position->x, position->y, position->z));
	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	m_myMotionState = new btDefaultMotionState(rbTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_myMotionState, m_collisionShape, btVector3(0, 0, 0));
	m_rigidBody = new btRigidBody(rbInfo);
	m_rigidBody->setUserIndex(type);
	//���[���h�ɒǉ��B
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddRigidBody_Dynamic(m_rigidBody);
}

//�����G���W�����g���������蔻�菈��&�W�����v����
void CIsIntersect::Intersect(D3DXVECTOR3* position, D3DXVECTOR3* moveSpeed,bool Jumpflag)
{
	m_isHitGround = false;
	m_Jumpflag = Jumpflag;
	static float deltaTime = 1.0f / 60.0f;						/************/
	static D3DXVECTOR3 gravity(0.0f, -40.0f, 0.0f);	//�d��		/*  �W		*/
	D3DXVECTOR3 addGravity = gravity;							/*  ��		*/
	addGravity *= (deltaTime);			//0.16�b���ɉ���		/*  ��		*/
	*moveSpeed += (addGravity);	//�������x						/*  �v		*/
	D3DXVECTOR3 addPos;											/*  ��		*/
	addPos = *moveSpeed;										/*  ��		*/
	addPos *= (deltaTime);										/*			*/
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);							/************/

	//XZ���ʂ𒲂ׂ�B
	{
		int loopCount = 0;
		while (true) {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			start.setOrigin(btVector3(position->x, position->y, position->z));
			D3DXVECTOR3 newPos;
			SweepResult_XZ callback;
			callback.m_pAudio = m_pAudio;
			D3DXVECTOR3 addPosXZ = addPos;
			addPosXZ.y = 0.0f;
			if (D3DXVec3Length(&addPosXZ) > 0.0001f) {
				newPos = (*position + addPosXZ);
				end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));
				SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest_Dynamic(m_collisionShape, start, end, callback);
			}
			if (callback.isHit) {
				//���������B
				//�ǁB
 				addPos.x = callback.hitPos.x - position->x;
				addPos.z = callback.hitPos.z - position->z;

				D3DXVECTOR3 t;
				t.x = -addPos.x;
				t.y = 0.0f;
				t.z = -addPos.z;
				D3DXVec3Normalize(&t, &t);
				//D3DXVec3Normalize(&t, &addPos);
				//���a�������߂��B
				t *= m_radius;
				addPos += t;
				//�����ĕǂɉ����Ċ��点��B
				//���点��������v�Z�B
				D3DXVec3Cross(&t, &callback.hitNormalXZ, &Up);
				D3DXVec3Normalize(&t, &t);
				//D3DXVec3Normalize(&t, &addPos);
				t *= D3DXVec3Dot(&t, &addPosXZ);
				addPos += t;	//���点��x�N�g�������Z�B

				//if (callback.hitCollisionType == CollisionType::CollisionType_Boss) {
				//	// �����������̂��{�X�Ȃ炵�тꂳ����B
				//	SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("TEST3D"))->EnemyHit();
				//}
			}
			else {
				//�ǂ��Ƃ�������Ȃ��̂ŏI���B
				break;
			}
			loopCount++;
			if (loopCount == 5) {
				break;
			}
		}
	}
	//�������𒲂ׂ�B
	{
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
#ifdef ORIGIN_CENTER
		start.setOrigin(btVector3(position->x, position->y, position->z));
#else
		start.setOrigin(btVector3(position->x, position->y + m_radius, position->z));
#endif
		D3DXVECTOR3 endPos;
		SweepResult_Y callback;
		callback.m_pAudio = m_pAudio;
		callback.startPos = *position;
		if (fabsf(addPos.y) > 0.0001f) {
			endPos = *position;
#ifdef ORIGIN_CENTER
			endPos.y += addPos.y;
#else
			newPos.y += addPos.y + m_radius;
#endif
			if (m_Jumpflag)
			{
				//�W�����v��
				if (addPos.y > 0.0f) {
					// �㏸���B
					// �㏸���ł�XZ�Ɉړ��������ʂ߂荞��ł���\��������̂ŉ��𒲂ׂ�B
					endPos.y -= addPos.y * 0.01f;
				}
				else {
					// �������Ă���ꍇ�͂��̂܂܉��𒲂ׂ�B
					endPos.y += addPos.y;
				}
				//end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));
			}
			else
			{
				// �n�ʏ�ɂ��Ȃ��ꍇ��1m��������B
				endPos.y -= 1.0f;
				////�W�����v���ȊO�͒n�ʂɃv���C���[�������t����
				//end.setOrigin(btVector3(newPos.x, newPos.y - 1.0f, newPos.z));
			}
			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));

			SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest_Dynamic(m_collisionShape, start, end, callback);
		}
		if (callback.isHit) {
			//���������B
			//�n�ʁB
			
			D3DXVECTOR3 Circle;
			float x = 0.0f;
			float offset = 0.0f;	//�����߂��ʁB
			Circle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			
			Circle = *position;
			Circle.y = callback.hitPos.y;//�~�̒��S
			D3DXVECTOR3 v = Circle - callback.hitPos;
			x = D3DXVec3Length(&v);//���̂̊p�ƃv���C���[�̊Ԃ̉����̋��������܂�B

			offset = sqrt(m_radius*m_radius - x*x);//y�̕����������߂�B
		
			moveSpeed->y = 0.0f;
			addPos.y = callback.hitPos.y - position->y;
			m_isHitGround = true;
#ifdef ORIGIN_CENTER
			addPos.y += offset;
#endif

			//if (callback.hitCollisionType == CollisionType::CollisionType_Boss) {
			//	// �����������̂��{�X�Ȃ炵�тꂳ����B
			//	SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("TEST3D"))->EnemyHit();
			//}
		}
		
	}

	*position += addPos;

	const btVector3& rPos = m_rigidBody->getWorldTransform().getOrigin();

	m_rigidBody->getWorldTransform().setOrigin(btVector3(position->x, position->y, position->z));
}

void CIsIntersect::Intersect2(D3DXVECTOR3* position, D3DXVECTOR3* moveSpeed)
{

	static float deltaTime = 1.0f / 60.0f;						/************/
	static D3DXVECTOR3 gravity(0.0f, 0.0f, 0.0f);	//�d��		/*  �W		*/
	D3DXVECTOR3 addGravity = gravity;							/*  ��		*/
	addGravity *= (deltaTime);			//0.16�b���ɉ���		/*  ��		*/
	*moveSpeed += (addGravity);	//�������x						/*  �v		*/
	D3DXVECTOR3 addPos;											/*  ��		*/
	addPos = *moveSpeed;										/*  ��		*/
	addPos *= (deltaTime);										/*			*/
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);							/************/

	//XZ���ʂ𒲂ׂ�B
	{
		int loopCount = 0;
		while (true) {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			start.setOrigin(btVector3(position->x, position->y, position->z));
			D3DXVECTOR3 newPos;
			SweepResult_XZ callback;
			callback.m_pAudio = m_pAudio;
			D3DXVECTOR3 addPosXZ = addPos;
			addPosXZ.y = 0.0f;
			if (D3DXVec3Length(&addPosXZ) > 0.0001f) {
				newPos = (*position + addPosXZ);
				end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));
				SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest_Dynamic(m_collisionShape, start, end, callback);
			}
			if (callback.isHit) {
				//���������B
				//�ǁB
				addPos.x = callback.hitPos.x - position->x;
				addPos.z = callback.hitPos.z - position->z;

				D3DXVECTOR3 t;
				t.x = -addPos.x;
				t.y = 0.0f;
				t.z = -addPos.z;
				D3DXVec3Normalize(&t, &t);
				//D3DXVec3Normalize(&t, &addPos);
				//���a�������߂��B
				t *= m_radius;
				addPos += t;
				//�����ĕǂɉ����Ċ��点��B
				//���点��������v�Z�B
				D3DXVec3Cross(&t, &callback.hitNormalXZ, &Up);
				D3DXVec3Normalize(&t, &t);
				//D3DXVec3Normalize(&t, &addPos);
				t *= D3DXVec3Dot(&t, &addPosXZ);
				addPos += t;	//���点��x�N�g�������Z�B

				//if (callback.hitCollisionType == CollisionType::CollisionType_Boss) {
				//	// �����������̂��{�X�Ȃ炵�тꂳ����B
				//	SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("TEST3D"))->EnemyHit();
				//}
			}
			else {
				//�ǂ��Ƃ�������Ȃ��̂ŏI���B
				break;
			}
			loopCount++;
			if (loopCount == 5) {
				break;
			}
		}
	}
	
	*position += addPos;

	const btVector3& rPos = m_rigidBody->getWorldTransform().getOrigin();

	m_rigidBody->getWorldTransform().setOrigin(btVector3(position->x, position->y, position->z));
}

void CIsIntersect::IntersectCamera(D3DXVECTOR3* position,D3DXVECTOR3* moveSpeed)
{
	D3DXVECTOR3 addPos = *moveSpeed;
	//�������𒲂ׂ�B
	{
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
#ifdef ORIGIN_CENTER
		start.setOrigin(btVector3(position->x, position->y, position->z));
#else
		start.setOrigin(btVector3(position->x, position->y + m_radius, position->z));
#endif
		D3DXVECTOR3 newPos;
		SweepResultGround_Camera callback;
		callback.startPos = *position;
		if (fabsf(addPos.y) > 0.0001f) {
			newPos = *position;
#ifdef ORIGIN_CENTER
			newPos.y += addPos.y;
#else
			newPos.y += addPos.y + m_radius;
#endif
			end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));
			SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest_Dynamic(m_collisionShape, start, end, callback);
		}
		if (callback.isHit) {
			//���������B
			//�n�ʁB

			D3DXVECTOR3 Circle;
			float x = 0.0f;
			float offset = 0.0f;	//�����߂��ʁB

			Circle = *position;
			Circle.y = callback.hitPos.y;//�~�̒��S
			D3DXVECTOR3 v = Circle - callback.hitPos;
			x = D3DXVec3Length(&v);//���̂̊p�ƃv���C���[�̊Ԃ̉����̋��������܂�B

			offset = sqrt(m_radius*m_radius - x*x);//y�̕����������߂�B

			moveSpeed->y = 0.0f;
			addPos.y = callback.hitPos.y - position->y;
#ifdef ORIGIN_CENTER
			addPos.y += offset;
#endif
		}

	}
	*position += addPos;

	const btVector3& rPos = m_rigidBody->getWorldTransform().getOrigin();

	m_rigidBody->getWorldTransform().setOrigin(btVector3(position->x, position->y, position->z));
}