#pragma once

#include "stdafx.h"
#include "islntersect.h"
#include "CBManager.h"
#include "CollisionType.h"

#define ORIGIN_CENTER	//定義で起点が足元。
void GenelateChocoBall(CCBManager* mgr, btGhostObject* m_hitCollisionObject,CAudio* pAudio);

// プレイヤー用コールバック
struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
{
	bool isHit;
	D3DXVECTOR3 hitPos;
	D3DXVECTOR3 startPos;
	float fMin;

	SweepResultGround()
	{
		isHit = false;
		fMin = FLT_MAX;
	}

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_ChocoballTrigger) {

			CCBManager* mgr = (CCBManager*)convexResult.m_hitCollisionObject->getUserPointer();
			GenelateChocoBall(mgr, (btGhostObject*)convexResult.m_hitCollisionObject,m_pAudio);
			return 0.0f;
		}

		if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Player 
			|| convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Chocoball
			|| convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Enemy) {
			//無視。
			return 0.0f;
		}
		else if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Camera) {
			//無視。
			return 0.0f;
		}
		else if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Bullet){
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
		if (len < fMin){
			hitPos = hitPosTmp;
			fMin = len;
		}
		return 0.0f;
	}
	CAudio* m_pAudio;
};
struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
{
	D3DXVECTOR3 hitNormalXZ;
	bool isHit;
	D3DXVECTOR3 hitPos;
	SweepResultWall()
	{
		isHit = false;
	}

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_ChocoballTrigger) {
			CCBManager* mgr = (CCBManager*)convexResult.m_hitCollisionObject->getUserPointer();
			GenelateChocoBall(mgr, (btGhostObject*)convexResult.m_hitCollisionObject,m_pAudio);
			return 0.0f;
		}

		if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Player
			|| convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Chocoball
			|| convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Enemy)
		{
			//無視。
			return 0.0f;
		}
		else if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Camera) {
			//無視。
			return 0.0f;
		}
		else if (convexResult.m_hitCollisionObject->getUserIndex() == CollisionType_Bullet){
			return 0.0f;
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

		btTransform transform = convexResult.m_hitCollisionObject->getWorldTransform();


		hitPos.x = convexResult.m_hitPointLocal.x();
		hitPos.y = convexResult.m_hitPointLocal.y();
		hitPos.z = convexResult.m_hitPointLocal.z();
		return 0.0f;
	}
	CAudio* m_pAudio;
};


void GenelateChocoBall(CCBManager* mgr, btGhostObject* m_hitCollisionObject,CAudio* pAudio){
	if (!mgr->GetAlive()){
		pAudio->PlayCue("Chocoball", true,nullptr);//チョコ落下Audio
		mgr->Initialize();
		mgr->SetAlive(true);
		SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->RemoveCollisionObject(m_hitCollisionObject);
		g_player->SetCBM(mgr);
	}
}

// カメラ用コールバック
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
		if (len < fMin){
			hitPos = hitPosTmp;
			fMin = len;
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

//剛体(当たり判定のある物体)の初期化
void CIsIntersect::CollisitionInitialize(D3DXVECTOR3* position,float radius,CollisionType type)
{
	//コリジョン初期化。
	m_radius = radius;
	//Box(立方体),sphere(球体)などで当たり範囲を決める。
	m_collisionShape = new btSphereShape(m_radius);//ここで剛体の形状を決定

	float mass = 0.1f;

	btTransform rbTransform;
	rbTransform.setIdentity();
	rbTransform.setOrigin(btVector3(position->x, position->y, position->z));
	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	m_myMotionState = new btDefaultMotionState(rbTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_myMotionState, m_collisionShape, btVector3(0, 0, 0));
	m_rigidBody = new btRigidBody(rbInfo);
	m_rigidBody->setUserIndex(type);
	//ワールドに追加。
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddRigidBody(m_rigidBody);
}

bool CIsIntersect::IsHitGround()
{
	return m_isHitGround;
}
//物理エンジンを使った当たり判定処理&ジャンプ処理
void CIsIntersect::Intersect(D3DXVECTOR3* position, D3DXVECTOR3* moveSpeed,bool Jumpflag)
{
	m_isHitGround = false;
	m_Jumpflag = Jumpflag;
	static float deltaTime = 1.0f / 60.0f;						/************/
	static D3DXVECTOR3 gravity(0.0f, -40.0f, 0.0f);	//重力		/*  ジ		*/
	D3DXVECTOR3 addGravity = gravity;							/*  ャ		*/
	addGravity *= (deltaTime);			//0.16秒事に加速		/*  ン		*/
	*moveSpeed += (addGravity);	//落下速度						/*  プ		*/
	D3DXVECTOR3 addPos;											/*  処		*/
	addPos = *moveSpeed;										/*  理		*/
	addPos *= (deltaTime);										/*			*/
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);							/************/

	//XZ平面を調べる。
	{
		int loopCount = 0;
		while (true) {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			start.setOrigin(btVector3(position->x, position->y, position->z));
			D3DXVECTOR3 newPos;
			SweepResultWall callback;
			callback.m_pAudio = m_pAudio;
			D3DXVECTOR3 addPosXZ = addPos;
			addPosXZ.y = 0.0f;
			if (D3DXVec3Length(&addPosXZ) > 0.0001f) {
				newPos = (*position + addPosXZ);
				end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));
				SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest(m_collisionShape, start, end, callback);
			}
			if (callback.isHit) {
				//当たった。
				//壁。
 				addPos.x = callback.hitPos.x - position->x;
				addPos.z = callback.hitPos.z - position->z;

				D3DXVECTOR3 t;
				t.x = -addPos.x;
				t.y = 0.0f;
				t.z = -addPos.z;
				D3DXVec3Normalize(&t, &t);
				//D3DXVec3Normalize(&t, &addPos);
				//半径分押し戻す。
				t *= m_radius;
				addPos += t;
				//続いて壁に沿って滑らせる。
				//滑らせる方向を計算。
				D3DXVec3Cross(&t, &callback.hitNormalXZ, &Up);
				D3DXVec3Normalize(&t, &t);
				//D3DXVec3Normalize(&t, &addPos);
				t *= D3DXVec3Dot(&t, &addPosXZ);
				addPos += t;	//滑らせるベクトルを加算。
			}
			else {
				//どことも当たらないので終わり。
				break;
			}
			loopCount++;
			if (loopCount == 5) {
				break;
			}
		}
	}
	//下方向を調べる。
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
		SweepResultGround callback;
		callback.m_pAudio = m_pAudio;
		callback.startPos = *position;
		if (fabsf(addPos.y) > 0.0001f) {
			newPos = *position;
#ifdef ORIGIN_CENTER
			newPos.y += addPos.y;
#else
			newPos.y += addPos.y + m_radius;
#endif
			if (m_Jumpflag)
			{
				//ジャンプ中
				end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));
			}
			else
			{
				//ジャンプ中以外は地面にプレイヤーをくっ付ける
				end.setOrigin(btVector3(newPos.x, newPos.y - 1.0f, newPos.z));
			}

			SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest(m_collisionShape, start, end, callback);
		}
		if (callback.isHit) {
			//当たった。
			//地面。
			
			D3DXVECTOR3 Circle;
			float x = 0.0f;
			float offset = 0.0f;	//押し戻す量。
			Circle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			
			Circle = *position;
			Circle.y = callback.hitPos.y;//円の中心
			D3DXVECTOR3 v = Circle - callback.hitPos;
			x = D3DXVec3Length(&v);//物体の角とプレイヤーの間の横幅の距離が求まる。

			offset = sqrt(m_radius*m_radius - x*x);//yの平方根を求める。
		
			moveSpeed->y = 0.0f;
			addPos.y = callback.hitPos.y - position->y;
			m_isHitGround = true;
#ifdef ORIGIN_CENTER
			addPos.y += offset;
			
#endif
		}
		
	}

	*position += addPos;

	const btVector3& rPos = m_rigidBody->getWorldTransform().getOrigin();

	m_rigidBody->getWorldTransform().setOrigin(btVector3(position->x, position->y, position->z));
}

void CIsIntersect::Intersect2(D3DXVECTOR3* position, D3DXVECTOR3* moveSpeed)
{

	static float deltaTime = 1.0f / 60.0f;						/************/
	static D3DXVECTOR3 gravity(0.0f, 0.0f, 0.0f);	//重力		/*  ジ		*/
	D3DXVECTOR3 addGravity = gravity;							/*  ャ		*/
	addGravity *= (deltaTime);			//0.16秒事に加速		/*  ン		*/
	*moveSpeed += (addGravity);	//落下速度						/*  プ		*/
	D3DXVECTOR3 addPos;											/*  処		*/
	addPos = *moveSpeed;										/*  理		*/
	addPos *= (deltaTime);										/*			*/
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);							/************/

	//XZ平面を調べる。
	{
		int loopCount = 0;
		while (true) {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			start.setOrigin(btVector3(position->x, position->y, position->z));
			D3DXVECTOR3 newPos;
			SweepResultWall callback;
			callback.m_pAudio = m_pAudio;
			D3DXVECTOR3 addPosXZ = addPos;
			addPosXZ.y = 0.0f;
			if (D3DXVec3Length(&addPosXZ) > 0.0001f) {
				newPos = (*position + addPosXZ);
				end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));
				SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest(m_collisionShape, start, end, callback);
			}
			if (callback.isHit) {
				//当たった。
				//壁。
				addPos.x = callback.hitPos.x - position->x;
				addPos.z = callback.hitPos.z - position->z;

				D3DXVECTOR3 t;
				t.x = -addPos.x;
				t.y = 0.0f;
				t.z = -addPos.z;
				D3DXVec3Normalize(&t, &t);
				//D3DXVec3Normalize(&t, &addPos);
				//半径分押し戻す。
				t *= m_radius;
				addPos += t;
				//続いて壁に沿って滑らせる。
				//滑らせる方向を計算。
				D3DXVec3Cross(&t, &callback.hitNormalXZ, &Up);
				D3DXVec3Normalize(&t, &t);
				//D3DXVec3Normalize(&t, &addPos);
				t *= D3DXVec3Dot(&t, &addPosXZ);
				addPos += t;	//滑らせるベクトルを加算。
			}
			else {
				//どことも当たらないので終わり。
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
	//下方向を調べる。
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
			SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest(m_collisionShape, start, end, callback);
		}
		if (callback.isHit) {
			//当たった。
			//地面。

			D3DXVECTOR3 Circle;
			float x = 0.0f;
			float offset = 0.0f;	//押し戻す量。
			Circle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			Circle = *position;
			Circle.y = callback.hitPos.y;//円の中心
			D3DXVECTOR3 v = Circle - callback.hitPos;
			x = D3DXVec3Length(&v);//物体の角とプレイヤーの間の横幅の距離が求まる。

			offset = sqrt(m_radius*m_radius - x*x);//yの平方根を求める。

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