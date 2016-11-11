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
	bool LockOnflag;		//���b�N�I���t���O
	EnemyBase* m_NearEnemy = nullptr;
	int	m_lockonEnemyIndex;	//���b�N�I�����Ă���G�B
	CBuildBlock* m_pBuildBlock2;
	int m_lockonBlockIndexW;		//���b�N�I������u���b�N�̉��̃C���f�b�N�X
	int m_lockonBlockIndexH;		//���b�N�I������u���b�N�̏c�̃C���f�b�N�X
};
