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
	//ここからBulletPhysicsで衝突判定を行うためのメンバ変数。
	CCollisionInterface*			m_CollisionObject = nullptr;
	btDefaultMotionState*	m_myMotionState = nullptr;

	//D3DXVECTOR3		m_moveSpeed;		//移動速度。
	bool			m_isHitGround;		//地面に当たった？
	bool			m_Jumpflag;			//ジャンプフラグ
	bool m_isFirstCallback;		// 一回の当たり判定で最初に呼ばれたコールバックか。
	vector<bool> m_MaskCollisionTypes;	// このクラスのインスタンスを持つオブジェクトが、そのCollisionTypeを無視するかのフラグを格納。
};
