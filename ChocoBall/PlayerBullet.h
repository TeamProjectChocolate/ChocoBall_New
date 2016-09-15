#pragma once
#include "stdafx.h"
#include "Bullet.h"
#include "LockOn.h"
#include "Audio.h"
#include "Number.h"

class CEnemyManager;

class CPlayerBullet
{
public:
	CPlayerBullet();
	~CPlayerBullet();
	void Initialize();
	bool Update();
	void Draw();
	void OnDestroy();
	void Build();
	void SetDir(D3DXVECTOR4 dir){
		m_bullet->SetDir(dir);
	}
	bool BulletEnemyCollision();
	bool BulletBlockCollision();
	void SetPos(D3DXVECTOR3 pos){
		m_bullet->SetPos(pos);
	}
	D3DXVECTOR3 GetPos(){
		return m_bullet->GetPos();
	}
	void SetBulletSpeed(float s){
		m_bullet->SetBulletSpeed(s);
	}

	void SetAudio(CAudio* audio){
		m_pAudio = audio;
	}

	int GetDownEnemyNum()
	{
		return EnemyDownNum;
	}

public:
	static int		EnemyDownNum;
private:
	CLockOn          m_LockOn;
	int				m_lockonEnemyIndex;	//ロックオンしている敵のインデックス。
	CEnemyManager* m_pEnemyManager;
	CBuildBlock* m_pBlockManager;
	Bullet* m_bullet;
	CAudio* m_pAudio;
	CNumber* m_pNumber;
};

