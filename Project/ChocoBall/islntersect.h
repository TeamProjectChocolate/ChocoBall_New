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
	void Initialize(btRigidBody*);
	void Intersect(D3DXVECTOR3* position, D3DXVECTOR3* m_moveSpeed, bool Jumpflag);
	void IntersectCamera(D3DXVECTOR3* position, D3DXVECTOR3* moveSpeed);
	inline bool GetIsHitGround()
	{
		return m_isHitGround;
	}
	inline btRigidBody* GetRigidBody()
	{
		return m_rigidBody;
	}
	inline btCollisionShape* GetSphereShape()
	{
		return m_collisionShape;
	}
	void OnMask(CollisionType type) {
		m_MaskCollisionTypes[static_cast<int>(type)] = true;
	}
	void OffMask(CollisionType type) {
		m_MaskCollisionTypes[static_cast<int>(type)] = false;
	}
	const vector<bool>& GetMasks() const
	{
		return m_MaskCollisionTypes;
	}
private:
	//ここからBulletPhysicsで衝突判定を行うためのメンバ変数。
	btGhostObject*		m_ghostObject = nullptr;		//!<ゴースト。剛体の変わりになるもの。完全に物理挙動に任せたいものは剛体を使う。
	btCollisionShape*		m_collisionShape = nullptr;	//!<コリジョン形状。
	btRigidBody*			m_rigidBody = nullptr;
	btDefaultMotionState*	m_myMotionState = nullptr;

	//D3DXVECTOR3		m_moveSpeed;		//移動速度。
	bool			m_isHitGround;		//地面に当たった？
	bool			m_Jumpflag;			//ジャンプフラグ
	bool m_isFirstCallback;		// 一回の当たり判定で最初に呼ばれたコールバックか。
	vector<bool> m_MaskCollisionTypes;	// このクラスのインスタンスを持つオブジェクトが、そのCollisionTypeを無視するかのフラグを格納。
};
