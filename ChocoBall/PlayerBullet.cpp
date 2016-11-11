#include "stdafx.h"
#include "PlayerBullet.h"
#include "EnemyManager.h"

int CPlayerBullet::EnemyDownNum = 0;

CPlayerBullet::CPlayerBullet()
{
	m_bullet = nullptr;
}


CPlayerBullet::~CPlayerBullet()
{

}

void CPlayerBullet::Initialize(){
	m_bullet = SINSTANCE(CObjectManager)->GenerationObject<Bullet>(_T("PlayerBullet"), PRIORTY::OBJECT3D_ALPHA, false);
	m_bullet->Initialize();

	m_pEnemyManager = SINSTANCE(CObjectManager)->FindGameObject<CEnemyManager>(_T("EnemyManager"));
	m_pBlockManager = SINSTANCE(CObjectManager)->FindGameObject<CBuildBlock>(_T("B_Block"));
	m_pNumber = SINSTANCE(CObjectManager)->FindGameObject<CNumber>(_T("NUMBER"));
}

bool CPlayerBullet::Update(){
	//�e�ƓG�Ƃ̏Փ˔���
	if (BulletEnemyCollision()){
		m_pAudio->PlayCue("�X�|�b�P", true,this);//�Փˉ�
		return true;
	}
	// �e�ƕǃM�~�b�N�̓����蔻��
	if (BulletBlockCollision()){
		return true;
	}
	return false;
}

void CPlayerBullet::Draw(){
}

void CPlayerBullet::OnDestroy(){
	SINSTANCE(CObjectManager)->DeleteGameObject(m_bullet);
	m_bullet->OnDestroy();
}

void CPlayerBullet::Build(){
	m_bullet->Build();
}

bool CPlayerBullet::BulletEnemyCollision(){
	EnemyBase* NearEnemy = m_LockOn.FindNearEnemy(m_bullet->GetPos());
	if (NearEnemy){
		D3DXVECTOR3 dist;
		dist = NearEnemy->GetPos() - m_bullet->GetPos();
		float L;
		L = D3DXVec3Length(&dist);//�x�N�g���̒������v�Z
		

		if (L <= 1)
		{
			if (NearEnemy->GetMoveState() != MOVE_STATE::Fly){
				EnemyDownNum++;
			}
			NearEnemy->PlayerBulletHit(m_bullet->GetDirection());
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
			float bulletWidth = m_bullet->GetWidth();

			D3DXVECTOR3 dist;
			for (int idx_Y = 0; idx_Y < max_Y; idx_Y++){
				for (int idx_X = 0; idx_X < max_X; idx_X++){
					CBlock* pBlock;
					pBlock = m_pBlockManager->GetBlocks(idx_X, idx_Y);
					if (pBlock->GetAlive()){
						dist = pBlock->GetPos() - m_bullet->GetPos();
						float L;
						L = D3DXVec3Length(&dist);//�x�N�g���̒������v�Z
						L -= blockWidth / 2;
						L -= 1.73 / 2;
						if (L <= 0.0f)
						{
							pBlock->SetAlive(false);
							m_pAudio->PlayCue("sei_ge_touki_hibiware01", true,this);//���鉹
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
