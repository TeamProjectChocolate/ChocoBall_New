#pragma once
#include"stdafx.h"
#include "GameObject.h"

class CTurn
{
public:
	CTurn()
	{
		D3DXToRadian(10.0f);
	};
	~CTurn();
	//回転処理の初期化
	void CTurn::Initialize();		
	//回転処理の更新
	float CTurn::Update(bool isTurn, float currentAngleY);

	float Getm_currentAngleY()
	{
		return m_currentAngleY;
		
	}

private:
	float	m_currentAngleY;	//現在の方向
	float	m_targetAngleY;		//向きたい方向
	float	m_turnSpeed;		//回転速度
	float   m_cTurnSpeed;
	bool	isTurn;				//回転フラグ
	bool    LockOnflag;			//ロックオンフラグ
};