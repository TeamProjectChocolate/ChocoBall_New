#pragma once
#include "State.h"

class CPlayer;

class CShotState :
	public CState
{
public:
	CShotState(CEnemy_Boss* obj);
	~CShotState();
	void Entry()override;
	bool Update()override;
	void Exit(CEnemy_Boss::BOSS_STATE)override;
private:
	float m_IntervalTime = 1.0f;	// 弾発射間のインターバル。
	float m_TimeCounter = 0.0f;
	CPlayer* m_pPlayer = nullptr;
	int m_RotationDir = 1;
};

static D3DXVECTOR3 BuletShotOffsetDir[4] = {
	D3DXVECTOR3(0.5f,0.5f,0.0f),
	D3DXVECTOR3(-0.5f,0.5f,0.0f),
	D3DXVECTOR3(0.5f,-0.5f,0.0f),
	D3DXVECTOR3(-0.5f,-0.5f,0.0f),
};
