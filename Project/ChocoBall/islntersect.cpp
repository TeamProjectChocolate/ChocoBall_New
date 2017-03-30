#pragma once

#include "stdafx.h"
#include "islntersect.h"
#include "ObjectManager.h"

#define ORIGIN_CENTER	//��`�ŋN�_�������B

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

//����(�����蔻��̂��镨��)�̏�����
void CIsIntersect::Initialize(CCollisionInterface* Collision)
{
	m_CollisionObject = Collision;
}

//�����G���W�����g���������蔻�菈��&�W�����v����
void CIsIntersect::Intersect(D3DXVECTOR3* position,const D3DXVECTOR3& moveSpeed,bool Jumpflag)
{
	m_isFirstCallback = true;
	m_isHitGround = false;
	m_Jumpflag = Jumpflag;
	static float deltaTime = 1.0f / 60.0f;
	static D3DXVECTOR3 gravity(0.0f, -40.0f, 0.0f);	//�d��	
	D3DXVECTOR3 addGravity = gravity;	
	addGravity *= (deltaTime);			//0.16�b���ɉ���(1�t���[���̏d�͉����x)�B
	D3DXVECTOR3 addPos = moveSpeed;
	// ���x�ɏd�͉����x�����Z�B
	addPos += (addGravity);
	addPos *= (deltaTime);	
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);

	// �R���W�������[���h�ł̓����蔻��B
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
	// �������[���h�ł̓����蔻��B
	{
		//XZ���ʂ𒲂ׂ�B
		{
			int loopCount = 0;
			while (true) {
				D3DXVECTOR3 OriginScale = m_CollisionObject->GetScale();
				D3DXVECTOR3 addPosXZ = addPos;
				addPosXZ.y = 0.0f;
				if (D3DXVec3Length(&addPosXZ) > 0.0001f) {
					// ���̃N���X�ɓo�^����Ă��鍄�̂������I�Ɉړ����Ă���ꍇ�B
					// �ړ��O�ƈړ����Transform���𐶐��B
					btTransform start, end;
					start.setIdentity();
					end.setIdentity();
					start.setOrigin(btVector3(position->x, position->y, position->z));
					D3DXVECTOR3 newPos;
					newPos = (*position + addPosXZ);
					end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));
					// �Փ˂����ꍇ�̃R�[���o�b�N���`�B
					SweepResult_XZ callback(m_CollisionObject, m_isFirstCallback);
					// �������铖����̑�����ݒ�B
					callback.m_MaskCollisionTypes = GetMasks();
					// �����������œ����蔻��B
					SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest_Dynamic(static_cast<btConvexShape*>(m_CollisionObject->GetCollisionShape()), start, end, callback);
					if (callback.isHit) {
						//���������B
						if (callback.hitCollisionObject->getCollisionFlags() == 0) {
							return;
						}

						//XZ�B
						addPos.x = callback.hitPos.x - position->x;
						addPos.z = callback.hitPos.z - position->z;

						D3DXVECTOR3 t;
						t.x = -addPos.x;
						t.y = 0.0f;
						t.z = -addPos.z;
						D3DXVec3Normalize(&t, &t);
						//D3DXVec3Normalize(&t, &addPos);
						//���a�������߂��B
						float radius = m_CollisionObject->GetCollisionShape()->getLocalScaling().getX();
						t *= radius * 0.5f;
						addPos += t;
						//�����ĕǂɉ����Ċ��点��B
						//���点��������v�Z�B
						D3DXVec3Cross(&t, &callback.hitNormalXZ, &Up);
						D3DXVec3Normalize(&t, &t);
						//D3DXVec3Normalize(&t, &addPos);
						t *= D3DXVec3Dot(&t, &addPosXZ);
						addPos += t;	//���点��x�N�g�������Z�B

						// �I�u�W�F�N�g�̃R�[���o�b�N����x�Ă΂ꂽ�̂Ńt���O���I�t�B
						m_isFirstCallback = false;
					}
					else {
						//�ǂ��Ƃ�������Ȃ��̂ŏI���B
						break;
					}
				}
				loopCount++;
				if (loopCount <= 5) {
					// �������[�v�ɂȂ�Ȃ��悤������x�ŏ����𒆒f�B
					break;
				}
			}
		}
		//�������𒲂ׂ�B
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

				SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest_Dynamic(static_cast<btConvexShape*>(m_CollisionObject->GetCollisionShape()), start, end, callback);
				if (callback.isHit) {
					//���������B
					m_isHitGround = true;

					if (callback.hitCollisionObject->getCollisionFlags() == 0) {
						// �����������Ă��鍄�̂���͉����߂���Ȃ��B
						return;
					}
					
					//Y�B

					D3DXVECTOR3 Circle;
					float x = 0.0f;
					float offset = 0.0f;	//�����߂��ʁB
					Circle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//D3DXVECTOR3 Test = m_rigidBody->getWorldTransform().getOrigin();
					//Circle = Test;
					Circle = *position;
					Circle.y = callback.hitPos.y;//�~�̒��S
					D3DXVECTOR3 v = Circle - callback.hitPos;
					x = D3DXVec3Length(&v);//���̂̊p�ƃv���C���[�̒��S�Ƃ̋��������܂�B

					float radius = m_CollisionObject->GetCollisionShape()->getLocalScaling().getY();
					//offset = radius - x;
					offset = sqrt(radius * radius - x * x);//y�̕����������߂�B

					//moveSpeed->y = 0.0f;
					addPos.y = callback.hitPos.y - position->y;
#ifdef ORIGIN_CENTER
					addPos.y += offset;
#endif
					// �I�u�W�F�N�g�̃R�[���o�b�N����x�Ă΂ꂽ�̂Ńt���O���I�t�B
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
			SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest_Dynamic(static_cast<btConvexShape*>(m_CollisionObject->GetCollisionShape()), start, end, callback);
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

			float radius = m_CollisionObject->GetCollisionShape()->getLocalScaling().getX();
			offset = sqrt(radius * radius - x * x);//y�̕����������߂�B

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