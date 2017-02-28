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
	SetRotation(D3DXVECTOR3(0, 0, 1), 0.0f);//’e‚ªZŽ²‰ñ“]‚·‚é
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	SetAlive(true);
	m_pModel->m_alpha = 0.75f;//“§–¾“x
	m_pModel->m_luminance = 0.0f;
	m_pModel->m_Refractive = g_RefractivesTable[REFRACTIVES::GOLD];
	m_radius = 1.0f;
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	m_moveSpeed.y = 0.0f;
	this->Build();
	m_IsIntersect.CollisitionInitialize(&m_transform.position, m_radius,CollisionType_Bullet);
}

void Bullet::Update()
{
	//ƒvƒŒƒCƒ„[‚ÌŒü‚¢‚Ä‚¢‚éƒxƒNƒgƒ‹‚ð’e‚É‰ÁŽZ
	m_transform.position.x = m_transform.position.x + m_dir.x*m_Speed;
	m_transform.position.y = m_transform.position.y + m_dir.y*m_Speed;
	m_transform.position.z = m_transform.position.z + m_dir.z*m_Speed;

	m_IsIntersect.Intersect2(&m_transform.position, &m_moveSpeed);

	float length = D3DXVec3Length(&D3DXVECTOR3(m_transform.position - m_StartPos));
	if (length >= m_Range) {
		// ’e‚ª”ò‹——£‚ð’´‚¦‚½‚çB
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
