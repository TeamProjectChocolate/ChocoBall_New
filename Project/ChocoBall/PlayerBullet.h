#pragma once
#include "stdafx.h"
#include "Bullet.h"
#include "LockOn.h"
#include "Audio.h"
#include "Number.h"

class CEnemyManager;

class CPlayerBullet:public CGameObject
{
public:
	CPlayerBullet();
	~CPlayerBullet();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void OnDestroy()override;
	void Build();
	void BulletCollision();
	void SetAudio(CAudio* audio){
		m_pAudio = audio;
	}
	int GetDownEnemyNum()
	{
		return EnemyDownNum;
	}

	inline void SetStartPos(const D3DXVECTOR3& pos) {
		m_Bullet->SetStartPos(pos);
	}
	inline void SetDir(const D3DXVECTOR3& dir) {
		m_Bullet->SetDir(dir);
	}
	inline void SetBulletSpeed(float speed) {
		m_Bullet->SetBulletSpeed(speed);
	}
	inline void SetRange(float range) {
		m_Bullet->SetRange(range);
	}
	inline void SetPintoPos(const D3DXVECTOR3& pos)override {
		m_Bullet->SetPintoPos(pos);
	}
	inline void SetPintoWorld(const D3DXMATRIX& mat)override {
		m_Bullet->SetPintoWorld(mat);
	}

private:
	bool BulletEnemyCollision();
	bool BulletBlockCollision();
public:
	static int		EnemyDownNum;
private:
	CLockOn          m_LockOn;
	CEnemyManager* m_pEnemyManager = nullptr;;
	CBuildBlock* m_pBlockManager = nullptr;;
	CAudio* m_pAudio = nullptr;;
	CNumber* m_pNumber = nullptr;
	unique_ptr<Bullet> m_Bullet;
};

