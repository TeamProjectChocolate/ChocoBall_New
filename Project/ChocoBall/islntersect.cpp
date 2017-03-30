#pragma once

#include "stdafx.h"
#include "islntersect.h"
#include "ObjectManager.h"

#define ORIGIN_CENTER	//定義で起点が足元。

CIsIntersect::CIsIntersect()
{
	m_isHitGround = false;
	m_Jumpflag = false;
	for (int idx = 0; idx < static_cast<int>(Collision::Type::Max); idx++) {
		m_MaskCollisionTypes.push_back(false);
	}
}

CIsIntersect::~CIsIntersect()
{
	m_MaskCollisionTypes.clear();
}

//剛体(当たり判定のある物体)の初期化
void CIsIntersect::Initialize(CCollisionInterface* Collision)
{
	m_CollisionObject = Collision;
}

//物理エンジンを使った当たり判定処理&ジャンプ処理
void CIsIntersect::Intersect(D3DXVECTOR3* position,const D3DXVECTOR3& moveSpeed,bool Jumpflag)
{
	m_isFirstCallback = true;
	m_isHitGround = false;
	m_Jumpflag = Jumpflag;
	static float deltaTime = 1.0f / 60.0f;
	static D3DXVECTOR3 gravity(0.0f, -40.0f, 0.0f);	//重力	
	D3DXVECTOR3 addGravity = gravity;	
	addGravity *= (deltaTime);			//0.16秒事に加速(1フレームの重力加速度)。
	D3DXVECTOR3 addPos = moveSpeed;
	// 速度に重力加速度を加算。
	addPos += (addGravity);
	addPos *= (deltaTime);	
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);

	// コリジョンワールドでの当たり判定。
	{
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();

		D3DXVECTOR3 Pos = m_CollisionObject->GetPos();
		start.setOrigin(btVector3(Pos.x, Pos.y, Pos.z));
		//start.setOrigin(btVector3(position->x, position->y, position->z));

		D3DXVECTOR3 newPos;
		SweepResult_Collision callback(m_CollisionObject);
		callback.m_MaskCollisionTypes = m_MaskCollisionTypes;
		if (D3DXVec3Length(&addPos) > 0.0001f) {
			m_CollisionObject->Activate();
			newPos = (Pos + addPos);
			//newPos = (*position + addPos);

			end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));
			SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest(static_cast<btConvexShape*>(m_CollisionObject->GetCollisionShape()), start, end, callback);
		}
	}
	// 物理ワールドでの当たり判定。
	{
		//XZ平面を調べる。
		{
			int loopCount = 0;
			while (true) {
				D3DXVECTOR3 OriginScale = m_CollisionObject->GetScale();
				D3DXVECTOR3 addPosXZ = addPos;
				addPosXZ.y = 0.0f;
				if (D3DXVec3Length(&addPosXZ) > 0.0001f) {
					// このクラスに登録されている剛体が自発的に移動している場合。
					// 移動前と移動後のTransform情報を生成。
					btTransform start, end;
					start.setIdentity();
					end.setIdentity();
					start.setOrigin(btVector3(position->x, position->y, position->z));
					D3DXVECTOR3 newPos;
					newPos = (*position + addPosXZ);
					end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));
					// 衝突した場合のコールバックを定義。
					SweepResult_XZ callback(m_CollisionObject, m_isFirstCallback);
					// 無視する当たりの属性を設定。
					callback.m_MaskCollisionTypes = GetMasks();
					// 生成した情報で当たり判定。
					SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest_Dynamic(static_cast<btConvexShape*>(m_CollisionObject->GetCollisionShape()), start, end, callback);
					if (callback.isHit) {
						//当たった。
						if (callback.hitCollisionObject->getCollisionFlags() == 0) {
							return;
						}

						//XZ。
						addPos.x = callback.hitPos.x - position->x;
						addPos.z = callback.hitPos.z - position->z;

						D3DXVECTOR3 t;
						t.x = -addPos.x;
						t.y = 0.0f;
						t.z = -addPos.z;
						D3DXVec3Normalize(&t, &t);
						//D3DXVec3Normalize(&t, &addPos);
						//半径分押し戻す。
						float radius = m_CollisionObject->GetCollisionShape()->getLocalScaling().getX();
						t *= radius * 0.5f;
						addPos += t;
						//続いて壁に沿って滑らせる。
						//滑らせる方向を計算。
						D3DXVec3Cross(&t, &callback.hitNormalXZ, &Up);
						D3DXVec3Normalize(&t, &t);
						//D3DXVec3Normalize(&t, &addPos);
						t *= D3DXVec3Dot(&t, &addPosXZ);
						addPos += t;	//滑らせるベクトルを加算。

						// オブジェクトのコールバックが一度呼ばれたのでフラグをオフ。
						m_isFirstCallback = false;
					}
					else {
						//どことも当たらないので終わり。
						break;
					}
				}
				loopCount++;
				if (loopCount <= 5) {
					// 無限ループにならないようある程度で処理を中断。
					break;
				}
			}
		}
		//下方向を調べる。
		{
			if (fabsf(addPos.y) > 0.0001f) {
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
#ifdef ORIGIN_CENTER
				start.setOrigin(btVector3(position->x, position->y, position->z));
#else
				start.setOrigin(btVector3(position->x, position->y + m_radius, position->z));
#endif
				D3DXVECTOR3 endPos;
				SweepResult_Y callback(m_CollisionObject, m_isFirstCallback);
				callback.m_MaskCollisionTypes = GetMasks();

				//D3DXVECTOR3 pos = m_rigidBody->getWorldTransform().getOrigin();

				//callback.startPos = endPos = pos;

				callback.startPos = endPos = *position;
#ifdef ORIGIN_CENTER
				endPos.y += addPos.y;
#else
				newPos.y += addPos.y + m_radius;
#endif
				if (m_Jumpflag)
				{
					//ジャンプ中
					if (addPos.y > 0.0f) {
						// 上昇中。
						// 上昇中でもXZに移動した結果めり込んでいる可能性があるので下を調べる。
						endPos.y -= addPos.y * 0.01f;
					}
					else {
						// 落下している場合はそのまま下を調べる。
						endPos.y += addPos.y;
					}
					//end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));
				}
				else
				{
					// 地面上にいない場合は1m下を見る。
					endPos.y -= 1.0f;
					////ジャンプ中以外は地面にプレイヤーをくっ付ける
					//end.setOrigin(btVector3(newPos.x, newPos.y - 1.0f, newPos.z));
				}
				end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));

				SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest_Dynamic(static_cast<btConvexShape*>(m_CollisionObject->GetCollisionShape()), start, end, callback);
				if (callback.isHit) {
					//当たった。
					m_isHitGround = true;

					if (callback.hitCollisionObject->getCollisionFlags() == 0) {
						// 物理挙動している剛体からは押し戻されない。
						return;
					}
					
					//Y。

					D3DXVECTOR3 Circle;
					float x = 0.0f;
					float offset = 0.0f;	//押し戻す量。
					Circle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//D3DXVECTOR3 Test = m_rigidBody->getWorldTransform().getOrigin();
					//Circle = Test;
					Circle = *position;
					Circle.y = callback.hitPos.y;//円の中心
					D3DXVECTOR3 v = Circle - callback.hitPos;
					x = D3DXVec3Length(&v);//物体の角とプレイヤーの中心との距離が求まる。

					float radius = m_CollisionObject->GetCollisionShape()->getLocalScaling().getY();
					//offset = radius - x;
					offset = sqrt(radius * radius - x * x);//yの平方根を求める。

					//moveSpeed->y = 0.0f;
					addPos.y = callback.hitPos.y - position->y;
#ifdef ORIGIN_CENTER
					addPos.y += offset;
#endif
					// オブジェクトのコールバックが一度呼ばれたのでフラグをオフ。
					m_isFirstCallback = false;
				}
			}
		}
	}

	*position += addPos;

	m_CollisionObject->SetPos(*position);
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
			SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest_Dynamic(static_cast<btConvexShape*>(m_CollisionObject->GetCollisionShape()), start, end, callback);
		}
		if (callback.isHit) {
			//当たった。
			//地面。

			D3DXVECTOR3 Circle;
			float x = 0.0f;
			float offset = 0.0f;	//押し戻す量。

			Circle = *position;
			Circle.y = callback.hitPos.y;//円の中心
			D3DXVECTOR3 v = Circle - callback.hitPos;
			x = D3DXVec3Length(&v);//物体の角とプレイヤーの間の横幅の距離が求まる。

			float radius = m_CollisionObject->GetCollisionShape()->getLocalScaling().getX();
			offset = sqrt(radius * radius - x * x);//yの平方根を求める。

			moveSpeed->y = 0.0f;
			addPos.y = callback.hitPos.y - position->y;
#ifdef ORIGIN_CENTER
			addPos.y += offset;
#endif
		}

	}

	*position += addPos;

	m_CollisionObject->SetPos(*position);
}