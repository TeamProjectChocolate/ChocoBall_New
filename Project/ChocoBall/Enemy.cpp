#include "stdafx.h"
#include "Enemy.h"
#include "ObjectManager.h"
#include "Player.h"

extern CEnemyManager g_enemyMgr;
extern CPlayer* g_player;

CEnemy::CEnemy()
{
	time = 0;
	m_initPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_transform.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
};

CEnemy::~CEnemy(){ }

void CEnemy::Setradius(float radius)
{
	m_radius = radius;
}

void CEnemy::Initialize()
{
	CEnemy_People::Initialize();
	SetRotation(D3DXVECTOR3(0, 1, 0), 0.1f);
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_moveSpeed = 0.0f;
	m_radius = 0.1f;
	SetAlive(true);	//���S�t���O
	m_pModel->m_alpha = 1.0f;	//�����x�H

	m_pPlayer = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("TEST3D"));
}

void CEnemy::Update()
{
	if (m_State != MOVE_STATE::Fly){
		m_EnemyToPlayerVec = g_player->GetPos() - this->m_transform.position;
		float E;
		E = D3DXVec3Length(&m_EnemyToPlayerVec);//�x�N�g���̒������v�Z
		time++;

		if (E <= 20)
		{
			m_State = MOVE_STATE::RockOn;
		}
		else{
			m_State = MOVE_STATE::Wait;
		}
	}
	CEnemy_People::Update();
}


void CEnemy::Draw()
{
	if (GetAlive())
	{
		SetUpTechnique();
		CGameObject::Draw();
	}
}


void CEnemy::EnemyBulletShot()
{
	if (time >= 180)
	{
		time = 0;
		CEnemyBullet* bullet = SINSTANCE(CObjectManager)->GenerationObject<CEnemyBullet>(_T("EnemyBullet"), PRIORTY::OBJECT3D, false);
		bullet->Initialize();
		bullet->SetStartPos(m_transform.position);
		D3DXVECTOR3 work;
		work.x = m_EnemyToPlayerVec.x;
		work.y = m_EnemyToPlayerVec.y;
		work.z = m_EnemyToPlayerVec.z;
		bullet->SetDir(work);
		bullet->SetBulletSpeed(0.05f);//�G�̒e�̑��x
		bullet->SetRange(10.0f);
	}
}

void CEnemy::Move(){
	//�G�l�~�[�̌����x�N�g�����v�Z
	D3DXVec3Normalize(&m_EnemyToPlayerVec, &m_EnemyToPlayerVec);
	EnemyBulletShot();

	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross, &m_EnemyToPlayerVec, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	m_TargetAngleY = acosf(D3DXVec3Dot(&m_EnemyToPlayerVec, &D3DXVECTOR3(0.0f, 0.0f, -1.0f)));
	if (cross.y > 0.0f){
		m_TargetAngleY *= -1.0f;
	}

	//m_TargetAngleY = acosf(D3DXVec3Dot(&m_Direction, &m_EnemyToPlayerVec));
	m_CurrentAngleY = m_Turn.Update(true, m_TargetAngleY);

	//m_CurrentAngleY = acosf(D3DXVec3Dot(&m_EnemyToPlayerVec,&m_Direction));
	//m_Direction = m_EnemyToPlayerVec;
}