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
	// ��]�ɂ����鎞�Ԃ�ݒ�
	// �����͕b�B
	void SetRotationTime(float time) {
		m_RotationTime = time;
	}
private:
	D3DXVECTOR3 m_TargetDirection;
	float m_RotationTime;	// ��]�ɂ����鎞�ԁB
	float m_TimeCounter;
	bool m_IsFirst = true;
private:
	// ���������Ɨp�ϐ��B
	D3DXQUATERNION m_InitQuat;
	float m_Offset;
	float m_Angle;
	D3DXVECTOR3 m_Axis;
};
