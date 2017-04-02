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

	m_transform.Identity();

	SetAlive(true);

	m_pModel->m_alpha = 0.75f;//透明度
	m_pModel->m_luminance = 0.0f;
	m_pModel->m_Refractive = FRESNEL::g_RefractivesTable[FRESNEL::REFRACTIVES::GOLD];
	m_radius = 1.0f;

	float mass = 0.0f;
	ActivateCollision(Vector3::Zero, new btSphereShape(m_radius), Collision::Type::Bullet, Collision::FilterGroup::Chocoball, false, mass, true, true);
	m_CollisionObject->BitMask_AllOff();
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Chocoball);
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Enemy);
}

void Bullet::Update()
{
	//弾の向きベクトルに加算
	m_transform.position += m_dir * m_Speed;

	CGameObject::Update();
}

bool Bullet::IsDelete() {
	float length = D3DXVec3Length(&D3DXVECTOR3(m_transform.position - m_StartPos));
	if (length >= m_Range) {
		// 弾が飛距離を超えたら。
		SetAlive(false);
		return true;
	}
	return false;
}

void Bullet::Draw()
{
	CGameObject::Draw();
}

void Bullet::OnDestroy()
{

}
