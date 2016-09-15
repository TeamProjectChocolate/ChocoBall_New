#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "Rigidbody.h"
#include "islntersect.h"
#include "CourceDef.h"
#include "Turn.h"
#include "EnemyBase.h"

class CEnemyFB : public EnemyBase
{
public:
	CEnemyFB();
	~CEnemyFB();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Setradius(float radius)
	{
		m_radius = radius;
	}
	void SetInitPosition(D3DXVECTOR3 pos);
private:
	float			m_radius;
	D3DXVECTOR3		m_moveSpeed;		//移動速度。
	D3DXVECTOR3		m_initPosition;		//初期座標。
	float			m_eCurrentAngleY;	//現在の方向
	float			m_eTargetAngleY;		//向きたい方向
	float			m_eTurnSpeed;		//回転速度
	bool			isTurn;				//回転フラグ
	CRigidbody		m_Rigidbody;
	bool			flg;
	bool			m_Hitflag;

	CTurn			m_Turn;				//CTurnのインスタンス
	CIsIntersect	m_IsIntersect;		//CIsIntersectのインスタンス
	CCourceDef		m_Courcedef;

	D3DXVECTOR3		m_V0;
	float V0;
	D3DXVECTOR3		V1;
	D3DXVECTOR3 m_V1;
	D3DXVECTOR3 m_Up;
};
