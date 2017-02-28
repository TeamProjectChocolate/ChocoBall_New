#pragma once
#include "stdafx.h"
#include "EnemyFB.h"


CEnemyFB::CEnemyFB()
{
	m_initPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_transform.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
};

CEnemyFB::~CEnemyFB(){ }

void CEnemyFB::Initialize()
{
	CEnemy_People::Initialize();
	SetRotation(D3DXVECTOR3(0, 1, 0), 0.1f);
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_moveSpeed = 0.05f;
	m_radius = 0.1f;
	SetAlive(true);	//死亡フラグ
	m_pModel->m_alpha = 1.0f;	//透明度。
	flg = true;

	m_Courcedef.Initialize();
	COURCE_BLOCK Cource = m_Courcedef.FindCource(m_initPosition);

	D3DXVECTOR3 CourceVec;
	CourceVec = Cource.endPosition - Cource.startPosition;					//スタートからゴールに向けてのベクトル
	D3DXVec3Normalize(&m_EnemyDir, &CourceVec);										//上で求めたベクトルの正規化
}

void CEnemyFB::SetInitPosition(D3DXVECTOR3 pos)
{
	m_initPosition = pos;
	m_transform.position = pos;
}
void CEnemyFB::Update()
{
	if (m_State != MOVE_STATE::Fly){
		m_State = MOVE_STATE::Walk;
	}
	CEnemy_People::Update();
}


void CEnemyFB::Draw()
{
	if (GetAlive())
	{
		//IMAGE3D* img = GetImage();
		m_Rigidbody.Draw();
		SetUpTechnique();
		CEnemy_People::Draw();
	}
}

void CEnemyFB::Move(){
	m_IsTurn = true;

	/*D3DXVECTOR3 workVec*/m_transform.position += m_EnemyDir * m_moveSpeed;
	float length = D3DXVec3Length(&(m_transform.position - m_initPosition));
	if (length > 2.5){
		m_EnemyDir *= -1.0f;
	}

	m_TargetAngleY = acos(V0);

	D3DXVECTOR3 Vec;
	D3DXVec3Cross(&Vec, &D3DXVECTOR3(0.0f, 0.0f, -1.0f), &m_EnemyDir);
	if (Vec.y < 0)
	{
		m_TargetAngleY *= -1.0f;
	}
}