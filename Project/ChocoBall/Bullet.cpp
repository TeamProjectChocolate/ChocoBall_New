#include "stdafx.h"
#include "Bullet.h"
#include "CollisionType.h"

Bullet::~Bullet()
{

}

void Bullet::Initialize()
{
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/ball.x");
	CGameObject::Initialize();
	m_transform.position = D3DXVECTOR3(0.0f,0.0f,0.0f);
	SetRotation(D3DXVECTOR3(0, 0, 1), 0.0f);//�e��Z����]����
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	SetAlive(true);
	m_pModel->m_alpha = 0.75f;//�����x
	m_pModel->m_luminance = 0.0f;
	m_pModel->m_Refractive = FRESNEL::g_RefractivesTable[FRESNEL::REFRACTIVES::GOLD];
	m_radius = 1.0f;
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	m_moveSpeed.y = 0.0f;
	this->Build();

	float mass = 0.0f;
	ActivateCollision(Vector3::Zero, new btSphereShape(m_radius), Collision::Type::Bullet, Collision::FilterGroup::Chocoball, false, mass, true, true);
	m_CollisionObject->BitMask_AllOff();
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Chocoball);
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Enemy);
}

void Bullet::Update()
{
	//�v���C���[�̌����Ă���x�N�g����e�ɉ��Z
	m_transform.position.x = m_transform.position.x + m_dir.x*m_Speed;
	m_transform.position.y = m_transform.position.y + m_dir.y*m_Speed;
	m_transform.position.z = m_transform.position.z + m_dir.z*m_Speed;


	float length = D3DXVec3Length(&D3DXVECTOR3(m_transform.position - m_StartPos));
	if (length >= m_Range) {
		// �e���򋗗��𒴂�����B
		SINSTANCE(CObjectManager)->DeleteGameObject(this);
		return;
	}

	BulletCollision();

	CGameObject::Update();
}
void Bullet::Draw()
{
	CGameObject::Draw();
}

void Bullet::OnDestroy()
{

}

void Bullet::Build()
{

}
