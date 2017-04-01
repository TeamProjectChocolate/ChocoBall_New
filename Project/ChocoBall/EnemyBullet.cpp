#include "stdafx.h"
#include "EnemyBullet.h"
#include "Bullet.h"
#include"Player.h"


CEnemyBullet::CEnemyBullet()
{
}

CEnemyBullet::~CEnemyBullet()
{

}

void CEnemyBullet::Initialize() {
	m_Bullet.reset(new Bullet);
	m_Bullet->Initialize();
	SetAlive(true);
}

void CEnemyBullet::Update(){
	m_Bullet->Update();
	BulletCollision();
	if (m_Bullet->IsDelete()) {
		SINSTANCE(CObjectManager)->DeleteGameObject(this);
	}
}

void CEnemyBullet::Draw(){
	m_Bullet->Draw();
}


void CEnemyBullet::BulletCollision(){
	D3DXVECTOR3 dist;
	dist = g_player->GetPos() - m_Bullet->GetPos();
	float E;
	E = D3DXVec3Length(&dist);//ベクトルの長さを計算
	if (E <= 1){
		if (!g_player->GetGameover())
		{
			dist /= E;
			g_player->EnemyBulletHit(dist);
			SINSTANCE(CObjectManager)->DeleteGameObject(this);
		}
	}
}
