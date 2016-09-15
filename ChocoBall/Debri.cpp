#include "stdafx.h"
#include "Debri.h"

void CDebri::Initialize()
{
	CGameObject::Initialize();
	m_transform.position = D3DXVECTOR3(0.0f,5.0f,0.0f);

	SetRotation(D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f);
	m_transform.scale = D3DXVECTOR3(1.0f,1.0f,1.0f);
	SetAlive(true);
	
	m_moveSpeed.x = 0.05f;
	m_moveSpeed.z = 0.0f;
	m_moveSpeed.y = 0.0f;

	//m_Rigidbody.Initialize(&m_transform.position, &m_transform.scale);

	this->Build();

	//float mass = 1000.0f;

	//btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_myMotionState, m_collisionShape, btVector3(0, 0, 0));
	//m_rigidBody = new btRigidBody(rbInfo);
	//m_rigidBody->setUserIndex(1);

	//int defState = m_rigidBody->getActivationState();
	//m_rigidBody->setActivationState(defState | DISABLE_DEACTIVATION);
}


void CDebri::Update()
{

	//m_Rigidbody.Update(&m_transform.position);
	CGameObject::Update();
}
void CDebri::Draw()
{
	SetUpTechnique();
	//CGameObject::Draw();
}
void CDebri::OnDestroy()
{
	//m_Rigidbody.OnDestroy();
}

void CDebri::Build()
{
	//m_Rigidbody.Build(m_transform.scale,m_transform.position);
}
