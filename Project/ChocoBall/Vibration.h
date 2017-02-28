#pragma once
#include "stdafx.h"
#include "CourceDef.h"

class CVibration
{
public:
	CVibration();
	~CVibration();
	void Initialize();
	void Update();
	void ThisVibration(D3DXVECTOR3*,D3DXVECTOR3, float,float);
	bool GetIsVibration(){
		return m_IsVibration;
	}
private:
	D3DXVECTOR3 m_currentDir;
	bool m_IsVibration;
	float m_VibrationCounter;
	float m_ReflectionCounter;
	CCourceDef m_CourceDef;
public:
	D3DXVECTOR3 m_dir;
	D3DXVECTOR3 m_VibrationAmount;	// 振動量
	D3DXVECTOR3 m_OrgPos;
	float m_VibrationTime;			// 振動時間
	float m_ReflectionTime;			// もとのポジションに戻るのにかかる時間
private:
	float m_length;
	float m_MoveSpeed;
	D3DXVECTOR3* m_Target;			// 振動させるオブジェクトの移動量を格納
};

