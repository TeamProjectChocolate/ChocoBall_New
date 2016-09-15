#pragma once
#include "stdafx.h"
#include "EnemyFB.h"
#include "EnemyManager.h"
#include "RenderContext.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "Player.h"
#include "InputManager.h"


CEnemyFB::CEnemyFB()
{
	m_initPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_transform.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
};

CEnemyFB::~CEnemyFB(){ }

void CEnemyFB::Initialize()
{
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/ENr.x");
	CGameObject::Initialize();
	SetRotation(D3DXVECTOR3(0, 1, 0), 0.1f);
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_V0 = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_moveSpeed.x = 0.05f;
	m_moveSpeed.z = 0.05f;
	m_moveSpeed.y = 0.05f;
	m_radius = 0.1f;
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//m_Hitflag = false;
	SetAlive(true);	//死亡フラグ
	m_pModel->m_alpha - 1.0f;	//透明度？
	flg = true;
	//m_Rigidbody.Initialize(&m_transform.position, &m_transform.scale);

	m_Courcedef.Initialize();
	COURCE_BLOCK Cource = m_Courcedef.FindCource(m_initPosition);

	m_V1 = Cource.endPosition - Cource.startPosition;					//スタートからゴールに向けてのベクトル
	D3DXVec3Normalize(&V1, &m_V1);										//上で求めたベクトルの正規化
}

void CEnemyFB::SetInitPosition(D3DXVECTOR3 pos)
{
	m_initPosition = pos;
	m_transform.position = pos;
}
void CEnemyFB::Update()
{
	isTurn = true;

	m_transform.position += V1*0.05f;
	D3DXVECTOR3 v = m_transform.position - m_initPosition;
	float v1 = D3DXVec3Length(&v);
	if (v1 > 2.5){
		V1 *= -1.0f;
	}

	m_eTargetAngleY = acos(V0);

	D3DXVECTOR3 V4;
	D3DXVec3Cross(&V4, &m_V0, &V1);
	if (V4.y < 0)
	{
		m_eTargetAngleY *= -1.0f;
	}


	m_eCurrentAngleY = m_Turn.Update(isTurn, m_eTargetAngleY);
	//回転行列
	SetRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_eCurrentAngleY);

	
	EnemyBase::Update();
}


void CEnemyFB::Draw()
{
	if (GetAlive())
	{
		//IMAGE3D* img = GetImage();
		m_Rigidbody.Draw();
		SetUpTechnique();
		CGameObject::Draw();
	}
}
