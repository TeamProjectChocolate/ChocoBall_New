#pragma once
#include "stdafx.h"
#include "CollisionType.h"
#include "BulletPhysics.h"

class CAudio;

class CIsIntersect
{
public:
	CIsIntersect();
	~CIsIntersect();
	void CollisitionInitialize(D3DXVECTOR3*,float,CollisionType);
	void Intersect(D3DXVECTOR3* position, D3DXVECTOR3* m_moveSpeed, bool Jumpflag);
	void Intersect2(D3DXVECTOR3* position, D3DXVECTOR3* m_moveSpeed);
	void IntersectCamera(D3DXVECTOR3* position, D3DXVECTOR3* moveSpeed);
	inline bool GetIsHitGround()
	{
		return m_isHitGround;
	}
	inline btRigidBody* GetRigidBody()
	{
		return m_rigidBody;
	}
	inline btSphereShape* GetSphereShape()
	{
		return m_collisionShape;
	}
	void SetAudio(CAudio* audio){
		m_pAudio = audio;
	}
private:
	//��������BulletPhysics�ŏՓ˔�����s�����߂̃����o�ϐ��B
	btGhostObject*		m_ghostObject;		//!<�S�[�X�g�B���̂̕ς��ɂȂ���́B���S�ɕ��������ɔC���������͍̂��̂��g���B
	btSphereShape*		m_collisionShape;	//!<�R���W�����`��B
	btRigidBody*			m_rigidBody;
	btDefaultMotionState*	m_myMotionState;
	CAudio* m_pAudio;

	float			m_radius;			//���a
	D3DXVECTOR3		m_moveSpeed;		//�ړ����x�B
	bool			m_isHitGround;		//�n�ʂɓ��������H
	bool			m_Jumpflag;			//�W�����v�t���O
};
