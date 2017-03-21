#include "stdafx.h"
#include "Enemyjamp.h"


CEnemyjamp::CEnemyjamp()
{
	m_initPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_transform.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
};

CEnemyjamp::~CEnemyjamp(){ }

void CEnemyjamp::Initialize()
{
	CEnemy_People::Initialize();
	SetRotation(D3DXVECTOR3(0, 1, 0), 0.1f);

	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_moveSpeed = 12.0f;
	m_radius = 0.1f;
	SetAlive(true);	//死亡フラグ
	m_pModel->m_alpha = 1.0f;	//透明度？
	flg = true;
	//m_Rigidbody.Initialize(&m_transform.position, &m_transform.scale);

	m_Courcedef.SetStageID(m_StageID);
	m_Courcedef.Initialize();
	Cource::COURCE_BLOCK Cource = m_Courcedef.FindCource(m_initPosition);
	m_DeltaTime = 0.0f;
	m_WaitTime = 0.5f;
	m_State = MOVE::STATE::Wait;

	m_pPlayer = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));
}

void CEnemyjamp::SetInitPosition(D3DXVECTOR3 pos)
{
	m_initPosition = pos;
	m_transform.position = pos;
}
void CEnemyjamp::Update()
{
	if (m_State == MOVE::STATE::Wait) {
		m_DeltaTime += 1.0f / 60.0f;
		if (m_DeltaTime >= m_WaitTime) {
			m_State = MOVE::STATE::Walk;
			m_moveSpeed = 0.65f;
		}
	}
	CEnemy_People::Update();
}


void CEnemyjamp::Draw()
{
	if (GetAlive())
	{
		CEnemy_People::Draw();
	}
}

void CEnemyjamp::Move(){
	m_moveSpeed -= GRAVITY / 5.0f;
	m_transform.position.y += m_moveSpeed;
	if (m_transform.position.y < m_initPosition.y)
	{
		m_transform.position = m_initPosition;
		m_State = MOVE::STATE::Wait;
		m_moveSpeed = 0.0f;
		m_DeltaTime = 0.0f;
	}

	//エネミーの向きベクトルを計算
	D3DXVECTOR3 EnemyToPlayerVec = m_pPlayer->GetPos() - m_transform.position;
	D3DXVec3Normalize(&EnemyToPlayerVec, &EnemyToPlayerVec);

	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross, &EnemyToPlayerVec, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	m_TargetAngleY = acosf(D3DXVec3Dot(&EnemyToPlayerVec, &D3DXVECTOR3(0.0f, 0.0f, -1.0f)));
	if (cross.y > 0.0f) {
		m_TargetAngleY *= -1.0f;
	}

	//m_TargetAngleY = acosf(D3DXVec3Dot(&m_Direction, &m_EnemyToPlayerVec));
	m_CurrentAngleY = m_Turn.Update(true, m_TargetAngleY);

}