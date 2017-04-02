#pragma once
#include "State.h"
#include "GameCamera.h"

// ボス死亡処理クラス。
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
	DeathState m_DeathState;	// ボスが死ぬまでの流れ。
	// 作業用。
	short m_Counter = 0;
	float m_TimeCounter = 0.0f;
	float m_Interval;	// いい感じに見せるための待ち時間。
	vector<D3DXVECTOR3> m_ChocoBallPositions;	// 演出用チョコボールを発生させる位置(ボスの位置からの距離)。
	CGameCamera* m_ClearCamera = nullptr;	// クリア演出カメラ。
};

