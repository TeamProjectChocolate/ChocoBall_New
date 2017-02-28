#pragma once
#include "stdafx.h"
#include "Bullet.h"
#include "LockOn.h"
#include "Audio.h"
#include "Number.h"

class CEnemyManager;

class CPlayerBullet:public Bullet
{
public:
	CPlayerBullet();
	~CPlayerBullet();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void OnDestroy()override;
	void Build();
	void BulletCollision()override;
	void SetAudio(CAudio* audio){
		m_pAudio = audio;
	}
	int GetDownEnemyNum()
	{
		return EnemyDownNum;
	}

private:
	bool BulletEnemyCollision();
	bool BulletBlockCollision();
public:
	static int		EnemyDownNum;
private:
	CLockOn          m_LockOn;
	CEnemyManager* m_pEnemyManager;
	CBuildBlock* m_pBlockManager;
	CAudio* m_pAudio;
	CNumber* m_pNumber;
};

