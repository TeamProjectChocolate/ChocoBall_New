#include "stdafx.h"
#include "Enemyjamp.h"
#include "EnemyManager.h"
#include "RenderContext.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "Player.h"
#include "InputManager.h"
#include "CBManager.h"
#include "CollisionType.h"



CEnemyjamp::CEnemyjamp()
{
	m_initPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_transform.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
};

CEnemyjamp::~CEnemyjamp(){ }

void CEnemyjamp::Initialize()
{
	EnemyBase::Initialize();
	SetRotation(D3DXVECTOR3(0, 1, 0), 0.1f);

	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_moveSpeed = 12.0f;
	m_Direction = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_radius = 0.1f;
	SetAlive(true);	//死亡フラグ
	m_pModel->m_alpha = 1.0f;	//透明度？
	flg = true;
	//m_Rigidbody.Initialize(&m_transform.position, &m_transform.scale);

	m_Courcedef.SetStageID(m_StageID);
	m_Courcedef.Initialize();
	COURCE_BLOCK Cource = m_Courcedef.FindCource(m_initPosition);
	m_DeltaTime = 0.0f;
	m_WaitTime = 0.5f;
	m_State = MOVE_STATE::Wait;
}

void CEnemyjamp::SetInitPosition(D3DXVECTOR3 pos)
{
	m_initPosition = pos;
	m_transform.position = pos;
}
void CEnemyjamp::Update()
{
	if (m_State == MOVE_STATE::Wait) {
		m_DeltaTime += 1.0f / 60.0f;
		if (m_DeltaTime >= m_WaitTime) {
			m_State = MOVE_STATE::Walk;
			m_moveSpeed = 0.65f;
		}
	}
	EnemyBase::Update();
}


void CEnemyjamp::Draw()
{
	if (GetAlive())
	{
		CGameObject::Draw();
	}
}

void CEnemyjamp::Move(){
	m_moveSpeed -= GRAVITY / 5.0f;
	if (m_transform.position.y < m_initPosition.y)
	{
		m_transform.position = m_initPosition;
		m_State = MOVE_STATE::Wait;
		m_moveSpeed = 0.0f;
		m_DeltaTime = 0.0f;
	}
}