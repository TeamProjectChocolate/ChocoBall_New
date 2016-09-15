#include "stdafx.h"
#include "Enemyjamp.h"
#include "EnemyManager.h"
#include "RenderContext.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "Player.h"
#include "InputManager.h"
#include "PlayerParam.h"
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
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/ENr.x");
	CGameObject::Initialize();
	SetRotation(D3DXVECTOR3(0, 1, 0), 0.1f);

	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_moveSpeed.x = 0.00f;
	m_moveSpeed.z = 0.00f;
	m_moveSpeed.y = 12.00f;
	m_radius = 0.1f;
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	SetAlive(true);	//死亡フラグ
	m_pModel->m_alpha = 1.0f;	//透明度？
	flg = true;
	//m_Rigidbody.Initialize(&m_transform.position, &m_transform.scale);

	m_Courcedef.SetStageID(m_StageID);
	m_Courcedef.Initialize();
	COURCE_BLOCK Cource = m_Courcedef.FindCource(m_initPosition);
}

void CEnemyjamp::SetInitPosition(D3DXVECTOR3 pos)
{
	m_initPosition = pos;
	m_transform.position = pos;
}
void CEnemyjamp::Update()
{
	float deltaTime = 1.0f / 60.0f;
	D3DXVECTOR3 gravity(0.0f, -9.8f, 0.0f);	//重力
	D3DXVECTOR3 addGravity = gravity;							
	addGravity *= (deltaTime);			//0.16秒事に加速		
	m_moveSpeed += addGravity;
	m_transform.position += m_moveSpeed*deltaTime;
	if (m_transform.position.y < m_initPosition.y)
	{
		m_moveSpeed.y=12.0f;
		m_transform.position.y = m_initPosition.y;
	}
	
	EnemyBase::Update();
}


void CEnemyjamp::Draw()
{
	if (GetAlive())
	{
		//IMAGE3D* img = GetImage();
		m_Rigidbody.Draw();
		SetUpTechnique();
		CGameObject::Draw();
	}
}

