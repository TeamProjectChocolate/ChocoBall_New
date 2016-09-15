#pragma once
#include"BulletPhysics.h"

class  CRigidbody
{
public:
	CRigidbody();
	~CRigidbody();
	//ボックス形状で剛体を初期化
	void Initialize(D3DXVECTOR3* m_position, D3DXVECTOR3* size);
	//球形状で剛体を初期化
	void Initialize(D3DXVECTOR3* m_position, float radius);
	void Update(D3DXVECTOR3* m_position);
	void Draw();
	virtual void OnDestroy();
	void Build(const D3DXVECTOR3& size, const D3DXVECTOR3& pos);
	void Build(float radius, const D3DXVECTOR3& pos);
	void SetLinearVelocity(const D3DXVECTOR3& vel)
	{
		m_rigidBody->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
	}
	void SetAngularVelocity(const D3DXVECTOR3& vel)
	{
		m_rigidBody->setAngularVelocity(btVector3(vel.x, vel.y, vel.z));
	}
	void ApplyForce(const D3DXVECTOR3& force)
	{
		m_rigidBody->applyForce(btVector3(force.x, force.y, force.z), btVector3(1.0f, 1.0f, 1.0f));
	}
	void SetUserIndex(int index)
	{
		m_rigidBody->setUserIndex(index);
	}
private:
	D3DXMATRIX matWorld, m_projMatrix;
	//ここからbulletPhysicsの剛体を使用するために必要な変数。
	LPD3DXMESH m_mesh;
	btCollisionShape*	m_collisionShape;	//コリジョンの形状。
	btRigidBody*		m_rigidBody;		//剛体。
	btDefaultMotionState* m_myMotionState;
	D3DXQUATERNION	m_rotation;
	float m_life;
};