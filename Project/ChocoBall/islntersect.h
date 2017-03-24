#pragma once
#include "stdafx.h"
#include "CollisionType.h"
#include "BulletPhysics.h"
#include "CollisionInterface.h"

class CAudio;

class CIsIntersect
{
public:
	CIsIntersect();
	~CIsIntersect();
	void Initialize(CCollisionInterface*);
	void Intersect(D3DXVECTOR3* position, const D3DXVECTOR3& m_moveSpeed, bool Jumpflag);
	void IntersectCamera(D3DXVECTOR3* position, D3DXVECTOR3* moveSpeed);
	inline bool GetIsHitGround()
	{
		return m_isHitGround;
	}
	inline CCollisionInterface* GetRigidBody()
	{
		return m_CollisionObject;
	}
	inline const btCollisionShape* GetSphereShape()
	{
		return m_CollisionObject->GetCollisionShape();
	}
	void OnMask(Collision::Type type) {
		m_MaskCollisionTypes[static_cast<int>(type)] = true;
	}
	void OffMask(Collision::Type type) {
		m_MaskCollisionTypes[static_cast<int>(type)] = false;
	}
	const vector<bool>& GetMasks() const
	{
		return m_MaskCollisionTypes;
	}
private:
	//��������BulletPhysics�ŏՓ˔�����s�����߂̃����o�ϐ��B
	CCollisionInterface*			m_CollisionObject = nullptr;
	btDefaultMotionState*	m_myMotionState = nullptr;

	//D3DXVECTOR3		m_moveSpeed;		//�ړ����x�B
	bool			m_isHitGround;		//�n�ʂɓ��������H
	bool			m_Jumpflag;			//�W�����v�t���O
	bool m_isFirstCallback;		// ���̓����蔻��ōŏ��ɌĂ΂ꂽ�R�[���o�b�N���B
	vector<bool> m_MaskCollisionTypes;	// ���̃N���X�̃C���X�^���X�����I�u�W�F�N�g���A����CollisionType�𖳎����邩�̃t���O���i�[�B
};
