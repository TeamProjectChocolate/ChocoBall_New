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
	SetAlive(true);	//死亡フラグ
	m_pModel->m_alpha = 1.0f;	//透明度？

	m_Courcedef.SetStageID(m_StageID);
	m_Courcedef.Initialize();
	COURCE_BLOCK Cource = m_Courcedef.FindCource(m_initPosition);

	D3DXVECTOR3 CourceVec = Cource.endPosition - Cource.startPosition;					//スタートからゴールに向けてのベクトル
	D3DXVec3Normalize(&m_Direction, &CourceVec);										//上で求めたベクトルの正規化

	m_pPlayer = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("TEST3D"));
}

void CEnemy::Update()
{
	if (m_State != MOVE_STATE::Fly){
		m_EnemyToPlayerVec = g_player->GetPos() - this->m_transform.position;
		float E;
		E = D3DXVec3Length(&m_EnemyToPlayerVec);//ベクトルの長さを計算
		time++;

		if (E <= 20)
		{
			m_State = MOVE_STATE::RockOn;
		}
		else{
			m_State = MOVE_STATE::Wait;
		}
	}

	//エネミーと弾の距離が10mになると弾が自動でDeleteする。
	int size = m_bullets.size();
	for (int idx = 0; idx < size; idx++){
		D3DXVECTOR3 V5;
		V5 = m_bullets[idx]->GetPos() - m_transform.position;
		float length = D3DXVec3Length(&V5);
		length = fabs(length);
		if (length > 10.0f)
		{
			EnemyDeleteBullet(m_bullets[idx]);
		}
		else{
			m_bullets[idx]->Update();
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

	int size = m_bullets.size();
	for (int idx = 0; idx < size; idx++){
		m_bullets[idx]->Draw();
	}
	ExcuteDeleteBullets();
}


void CEnemy::EnemyBulletShot()
{
	if (time >= 180)
	{
		time = 0;
		CEnemyBullet* bullet = new CEnemyBullet;
		bullet->Initialize();
		bullet->SetPos(m_transform.position);
		D3DXVECTOR4 work;
		work.x = m_EnemyToPlayerVec.x;
		work.y = m_EnemyToPlayerVec.y;
		work.z = m_EnemyToPlayerVec.z;
		work.w = 0.0f;
		bullet->SetDir(work);
		bullet->SetBulletSpeed(0.05f);//敵の弾の速度
		m_bullets.push_back(bullet);
	}
}

void CEnemy::EnemyDeleteBullet(CEnemyBullet* bullet){
	m_Deletebullets.push_back(bullet);
}

void CEnemy::ExcuteDeleteBullets(){
	vector<CEnemyBullet*>::iterator itr;
	int size = m_Deletebullets.size();
	for (int idx = 0; idx < size; idx++){
		for (itr = m_bullets.begin(); itr != m_bullets.end();){
			if (m_Deletebullets[idx] == *itr){
				(*itr)->OnDestroy();
				SAFE_DELETE(*itr);
				itr = m_bullets.erase(itr);
				break;
			}
			else{
				itr++;
			}
		}
	}
	m_Deletebullets.clear();
}

void CEnemy::Move(){
	//エネミーの向きベクトルを計算
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