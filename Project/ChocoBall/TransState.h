#pragma once
#include "State.h"

class CPlayer;

class CTransState :
	public CState
{
public:
	CTransState(CEnemy_Boss* obj);
	~CTransState();
	void Entry()override;
	bool Update()override;
	void Exit(CEnemy_Boss::BOSS_STATE)override;
	void SetMoveSpeed(float speed) {
		m_MoveSpeed = speed;
	}
	void SetLengthMax(float max) {
		m_LengthMax = max;
		m_IsUseLength = true;
	}
	void SetDirection(const D3DXVECTOR3& dir) {
		D3DXVec3Normalize(&m_Direction, &dir);
	}
private:
	CPlayer* m_pPlayer = nullptr;
	D3DXVECTOR3 m_InitPos;		// ステートが呼ばれた際の初期位置。
	D3DXVECTOR3 m_Direction;	// 向き。
	float m_MoveSpeed;	// 移動速度。
	float m_LengthMax;		// このステートで移動する量。
	bool m_IsUseLength;
};

