#include "stdafx.h"
#include "EnemyBullet.h"
#include "Bullet.h"
#include"Player.h"


CEnemyBullet::CEnemyBullet()
{
	m_enemybullet = nullptr;
}

CEnemyBullet::~CEnemyBullet()
{

}

void CEnemyBullet::Initialize(){
	m_enemybullet = SINSTANCE(CObjectManager)->GenerationObject<Bullet>(_T("EnemyBullet"), PRIORTY::BULLET, false);
	m_enemybullet->Initialize();
	m_Range = 20.0f;
}

void CEnemyBullet::Update(){
	BulletPlayerCollision();
}

void CEnemyBullet::Draw(){
}


void CEnemyBullet::BulletPlayerCollision(){
	D3DXVECTOR3 dist;
	dist = g_player->GetPos() - m_enemybullet->GetPos();
	float E;
	E = D3DXVec3Length(&dist);//ベクトルの長さを計算
	if (E <= 1){
		//ゲームオーバーつけてねノボリン
		if (!g_player->GetGameover())
		{
			dist /= E;
			g_player->EnemyBulletHit(dist);
		}
	}
}
