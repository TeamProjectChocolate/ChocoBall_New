#pragma once
#include "State.h"
#include "GameCamera.h"

// �{�X���S�����N���X�B
class CDeathState :
	public CState
{
private: enum class DeathState{Dummy = 0,Favorte,OnDynamic,Interval,GameEnd};
public:
	CDeathState(CEnemy_Boss* obj) :CState(obj) {};
	~CDeathState();
	void Entry()override;
	bool Update()override;
	void Exit(CEnemy_Boss::BOSS_STATE)override;
private:
	DeathState m_DeathState;	// �{�X�����ʂ܂ł̗���B
	// ��Ɨp�B
	short m_Counter = 0;
	float m_TimeCounter = 0.0f;
	float m_Interval;	// ���������Ɍ����邽�߂̑҂����ԁB
	vector<D3DXVECTOR3> m_ChocoBallPositions;	// ���o�p�`���R�{�[���𔭐�������ʒu(�{�X�̈ʒu����̋���)�B
	CGameCamera* m_ClearCamera = nullptr;	// �N���A���o�J�����B
};

