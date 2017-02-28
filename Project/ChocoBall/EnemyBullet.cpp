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

void CEnemyBullet::Initialize(){
	Bullet::Initialize();
}

void CEnemyBullet::Update(){
	Bullet::Update();
}

void CEnemyBullet::Draw(){
	Bullet::Draw();
}


void CEnemyBullet::BulletCollision(){
	D3DXVECTOR3 dist;
	dist = g_player->GetPos() - Bullet::GetPos();
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
