#pragma once
#include "CollisionInterface.h"

class CRigidbody:public CCollisionInterface
{
public:
	CRigidbody();
	~CRigidbody();

	void Update(D3DXVECTOR3* pos, D3DXQUATERNION* rot)override;

	inline void SetLinearVelocity(const D3DXVECTOR3& vel)
	{
		static_cast<btRigidBody*>(m_collisionObject.get())->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
	}
	inline void SetAngularVelocity(const D3DXVECTOR3& vel)
	{
		static_cast<btRigidBody*>(m_collisionObject.get())->setAngularVelocity(btVector3(vel.x, vel.y, vel.z));
	}
	// 剛体に力を加える。
	// 引数は向き付きの力の大きさ。
	inline void ApplyForce(const D3DXVECTOR3& force)
	{
		static_cast<btRigidBody*>(m_collisionObject.get())->applyForce(btVector3(force.x, force.y, force.z), btVector3(1.0f, 1.0f, 1.0f));
	}
	// 剛体に力を加える。
	// 引数:	向き付きの力の大きさ。
	//			???。
	inline void ApplyForce(const D3DXVECTOR3& force,const D3DXVECTOR3& rel_pos)
	{
		static_cast<btRigidBody*>(m_collisionObject.get())->applyForce(btVector3(force.x, force.y, force.z), btVector3(rel_pos.x, rel_pos.y, rel_pos.z));
	}
	// 剛体に質量と回転のしやすさを設定。
	// 引数：	質量。
	//			第二引数は回転のしやすさ。
	inline void SetMassProps(const float mass, const D3DXVECTOR3& inertia) {
		static_cast<btRigidBody*>(m_collisionObject.get())->setMassProps(mass,btVector3(inertia.x, inertia.y, inertia.z));
	}
	// 剛体を動的化。
	inline void OnDynamic() {
		m_IsKinematic = false;
		m_collisionObject->setCollisionFlags(0);
	}
	// 剛体をキネマティック化。
	inline void OnKinematic() {
		m_IsKinematic = true;
		m_collisionObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		m_collisionObject->setActivationState(DISABLE_DEACTIVATION);
	}
	//// 剛体を静的化。
	//void OnStatic() {
	//	m_IsKinematic = false;
	//	m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	//}


	// この剛体オブジェクトを物理ワールドに追加する。
	void AddWorld()override;
	// この剛体オブジェクトを物理ワールドから削除する。
	void RemoveWorld()override;


	bool GetIsKinematic() {
		return m_IsKinematic;
	}
private:
	// パラメータを参照して剛体生成。
	// 引数：オブジェクトのトランスフォーム情報。
	//		 質量(isKinematicがtrueならば自動的に0.0fになる)。
	//		 キネマティック剛体にするか(このフラグがtrueなら物理挙動をしなくなる)。
	void Build(
		const btTransform& transform,
		float mass,
		bool isKinematic)override;
private:
	unique_ptr<btDefaultMotionState> m_myMotionState;	// モーションステート。
	bool m_IsKinematic = false;
};