#include "stdafx.h"
#include "EnemyLR.h"

CEnemyLR::CEnemyLR()
{
	m_initPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_transform.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
};

CEnemyLR::~CEnemyLR(){ }

void CEnemyLR::Setradius(float radius)
{
	m_radius = radius;
}
void CEnemyLR::SetInitPosition(D3DXVECTOR3 pos)
{
	m_initPosition = pos;
	m_transform.position = pos;
}
void CEnemyLR::Initialize()
{
	CEnemy_People::Initialize();
	SetRotation(D3DXVECTOR3(0, 1, 0), 0.1f);
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_moveSpeed = 0.05f;
	m_radius = 0.1f;
	//m_Hitflag = false;
	SetAlive(true);	//死亡フラグ
	m_pModel->m_alpha = 1.0f;	//透明度？
	flg = true;

	m_Courcedef.SetStageID(m_StageID);
	m_Courcedef.Initialize();
	COURCE_BLOCK Cource = m_Courcedef.FindCource(m_initPosition);
	
	D3DXVECTOR3 CourceVec = Cource.endPosition - Cource.startPosition;					//スタートからゴールに向けてのベクトル
	D3DXVec3Normalize(&CourceVec, &CourceVec);										//上で求めたベクトルの正規化
	D3DXVec3Cross(&m_Direction, &CourceVec, &D3DXVECTOR3(0.0f,1.0f,0.0f));
	D3DXVec3Normalize(&m_Direction, &m_Direction);
}

void CEnemyLR::Update()
{
	if (m_State != MOVE_STATE::Fly){
		m_State = MOVE_STATE::Walk;
	}
	CEnemy_People::Update();
}


void CEnemyLR::Draw()
{
	if (GetAlive())
	{
		m_Rigidbody.Draw();
		SetUpTechnique();
		CGameObject::Draw();
	}
}

void CEnemyLR::Move(){
	m_IsTurn = true;

	D3DXVECTOR3 workVec = m_transform.position + m_Direction * m_moveSpeed;
	workVec -= m_initPosition;
	float length = D3DXVec3Length(&workVec);
	if (length > 2.5)
	{
		m_Direction *= -1.0f;
	}

	float rad = D3DXVec3Dot(&D3DXVECTOR3(0.0f, 0.0f, -1.0f), &m_Direction);
	m_TargetAngleY = acos(rad);
	D3DXVECTOR3 Vec;
	D3DXVec3Cross(&Vec, &D3DXVECTOR3(0.0f, 0.0f, -1.0f), &m_Direction);
	if (Vec.y < 0)
	{
		m_TargetAngleY *= -1.0f;
	}
}
