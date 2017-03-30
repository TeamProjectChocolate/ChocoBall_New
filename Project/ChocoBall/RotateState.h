#pragma once
#include "State.h"
#include "Turn.h"

class CRotateState :
	public CState
{
public:
	CRotateState(CEnemy_Boss* obj) :CState(obj) {};
	~CRotateState();
	void Entry()override;
	bool Update()override;
	void Exit(CEnemy_Boss::BOSS_STATE)override;
	void SetTargetDirection(const D3DXVECTOR3& dir) {
		D3DXVec3Normalize(&m_TargetDirection, &dir);
	}
	// 回転にかかる力を設定
	// 引数は度。
	void SetRotationPower(float Power) {
		m_RotationPower = Power;
	}
private:
	D3DXVECTOR3 m_TargetDirection;
	float m_RotationPower;	// 一秒間に回転する量(度)。
	bool m_IsFirst = true;
private:
	// ここから作業用変数。
	D3DXQUATERNION m_InitQuat;
	float m_Offset;
	float m_Angle;
	D3DXVECTOR3 m_Axis;
};

