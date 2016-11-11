#pragma once
#include "stdafx.h"
#include "BuildBlock.h"

class CLockOn
{
public:
	CLockOn();
	~CLockOn();
	float LockOnRotation(float _X, D3DXVECTOR3 position,EnemyBase* enemy);
	EnemyBase* FindNearEnemy(D3DXVECTOR3 position);
	float LockOnRotation2(float _X, D3DXVECTOR3 position, int lockonBlockIndexW,int lockonBlockIndexH);
	int FindNearBlock(D3DXVECTOR3 position);
private:
	bool LockOnflag;		//ロックオンフラグ
	EnemyBase* m_NearEnemy = nullptr;
	int	m_lockonEnemyIndex;	//ロックオンしている敵。
	CBuildBlock* m_pBuildBlock2;
	int m_lockonBlockIndexW;		//ロックオンするブロックの横のインデックス
	int m_lockonBlockIndexH;		//ロックオンするブロックの縦のインデックス
};
