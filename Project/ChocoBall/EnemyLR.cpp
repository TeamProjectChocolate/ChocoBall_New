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
	SetAlive(true);	//���S�t���O
	m_pModel->m_alpha = 1.0f;	//�����x�H
	//flg = true;

	m_Courcedef.SetStageID(m_StageID);
	m_Courcedef.Initialize();
	COURCE_BLOCK Cource = m_Courcedef.FindCource(m_initPosition);
	
	D3DXVECTOR3 CourceVec = Cource.endPosition - Cource.startPosition;					//�X�^�[�g����S�[���Ɍ����Ẵx�N�g��
	D3DXVec3Normalize(&CourceVec, &CourceVec);										//��ŋ��߂��x�N�g���̐��K��
	D3DXVec3Cross(&m_EnemyDir, &CourceVec, &D3DXVECTOR3(0.0f,1.0f,0.0f));
	D3DXVec3Normalize(&m_EnemyDir, &m_EnemyDir);
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
		CEnemy_People::Draw();
	}
}

void CEnemyLR::Move(){
	m_IsTurn = true;

	/*D3DXVECTOR3 workVec*/m_transform.position += m_EnemyDir * m_moveSpeed;
	float length = D3DXVec3Length(&(m_transform.position - m_initPosition));
	if (length > 2.5)
	{
		m_EnemyDir *= -1.0f;
	}

	float rad = D3DXVec3Dot(&D3DXVECTOR3(0.0f, 0.0f, -1.0f), &m_EnemyDir);
	m_TargetAngleY = acos(rad);
	D3DXVECTOR3 Vec;
	D3DXVec3Cross(&Vec, &D3DXVECTOR3(0.0f, 0.0f, -1.0f), &m_EnemyDir);
	if (Vec.y < 0)
	{
		m_TargetAngleY *= -1.0f;
	}
}
