#pragma once
#include "BulletPhysics.h"
#include "CollisionType.h"
#include "Audio.h"


class CIsIntersect
{
public:
	CIsIntersect();
	~CIsIntersect();
	void CIsIntersect::CollisitionInitialize(D3DXVECTOR3*,float,CollisionType);
	void CIsIntersect::Intersect(D3DXVECTOR3* position, D3DXVECTOR3* m_moveSpeed, bool Jumpflag);
	void CIsIntersect::Intersect2(D3DXVECTOR3* position, D3DXVECTOR3* m_moveSpeed);
	void CIsIntersect::IntersectCamera(D3DXVECTOR3* position, D3DXVECTOR3* moveSpeed);
	bool IsHitGround();
	btRigidBody* GetRigidBody()
	{
		return m_rigidBody;
	}
	btSphereShape* GetSphereShape()
	{
		return m_collisionShape;
	}
	void SetAudio(CAudio* audio){
		m_pAudio = audio;
	}
private:
	//ここからBulletPhysicsで衝突判定を行うためのメンバ変数。
	btGhostObject*		m_ghostObject;		//!<ゴースト。剛体の変わりになるもの。完全に物理挙動に任せたいものは剛体を使う。
	btSphereShape*		m_collisionShape;	//!<コリジョン形状。
	btRigidBody*			m_rigidBody;
	btDefaultMotionState*	m_myMotionState;
	CAudio* m_pAudio;

	float			m_radius;			//半径
	D3DXVECTOR3		m_moveSpeed;		//移動速度。
	bool			m_isHitGround;		//地面に当たった？
	bool			m_Jumpflag;			//ジャンプフラグ
};
