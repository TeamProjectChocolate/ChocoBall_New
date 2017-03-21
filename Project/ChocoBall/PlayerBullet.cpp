#include "stdafx.h"
#include "PlayerBullet.h"
#include "EnemyManager.h"

int CPlayerBullet::EnemyDownNum = 0;

CPlayerBullet::CPlayerBullet()
{
}


CPlayerBullet::~CPlayerBullet()
{

}

void CPlayerBullet::Initialize(){
	Bullet::Initialize();
	m_pEnemyManager = SINSTANCE(CObjectManager)->FindGameObject<CEnemyManager>(_T("EnemyManager"));
	m_pBlockManager = SINSTANCE(CObjectManager)->FindGameObject<CBuildBlock>(_T("B_Block"));
	m_pNumber = SINSTANCE(CObjectManager)->FindGameObject<CNumber>(_T("NUMBER"));
}

void CPlayerBullet::Update(){
	Bullet::Update();
}

void CPlayerBullet::Draw(){
	Bullet::Draw();
}

void CPlayerBullet::OnDestroy(){
	Bullet::OnDestroy();
}

void CPlayerBullet::Build(){
	Bullet::Build();
}

void CPlayerBullet::BulletCollision() {
	if (this->BulletEnemyCollision()) {
		m_pAudio->PlayCue("スポッ１", true, this);//衝突音
		SINSTANCE(CObjectManager)->DeleteGameObject(this);
	}
	else if(this->BulletBlockCollision())
	{
		SINSTANCE(CObjectManager)->DeleteGameObject(this);
	}
}

bool CPlayerBullet::BulletEnemyCollision(){
	EnemyBase* NearEnemy = m_LockOn.FindNearEnemy(Bullet::GetPos());
	if (NearEnemy){
		D3DXVECTOR3 dist;
		dist = NearEnemy->GetPos() - Bullet::GetPos();
		float L;
		L = D3DXVec3Length(&dist);//ベクトルの長さを計算
		

		if (L <= 1)
		{
			if (NearEnemy->GetMoveState() != MOVE::STATE::Fly){
				if(NearEnemy->GetMyType() != EnemyBase::Enemy_ModelType::Boss)
				EnemyDownNum++;
			}
			NearEnemy->PlayerBulletHit(Bullet::GetDirection());
			return true;
		}
	}
	return false;
}

bool CPlayerBullet::BulletBlockCollision(){
	for (int idx = 0;; idx++){
		string str = "B_Block";

		char num[100];
		_itoa(idx, num, 10);
		str += num;
		m_pBlockManager = SINSTANCE(CObjectManager)->FindGameObject<CBuildBlock>(_T(str.c_str()));
		if (m_pBlockManager != nullptr){
			int max_X = m_pBlockManager->GetNum_X();
			int max_Y = m_pBlockManager->GetNum_Y();

			float blockWidth = m_pBlockManager->GetBlockWidth();
			float bulletWidth = Bullet::GetWidth();

			D3DXVECTOR3 dist;
			for (int idx_Y = 0; idx_Y < max_Y; idx_Y++){
				for (int idx_X = 0; idx_X < max_X; idx_X++){
					CBlock* pBlock;
					pBlock = m_pBlockManager->GetBlocks(idx_X, idx_Y);
					if (pBlock->GetAlive()){
						dist = pBlock->GetPos() - Bullet::GetPos();
						float L;
						L = D3DXVec3Length(&dist);//ベクトルの長さを計算
						L -= blockWidth / 2;
						L -= 1.73 / 2;
						if (L <= 0.0f)
						{
							pBlock->SetAlive(false);
							m_pAudio->PlayCue("sei_ge_touki_hibiware01", true,this);//壊れる音
							return true;
						}
					}
				}
			}
		}
		else{
			return false;
		}
	}
}
