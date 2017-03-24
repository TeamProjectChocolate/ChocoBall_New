#pragma once
#include "stdafx.h"
#include "Enemy_People.h"
#include "Rigidbody.h"
#include "CourceDef.h"
#include "Turn.h"

class CEnemyLR : public CEnemy_People
{
public:
	CEnemyLR();
	~CEnemyLR();
	void Initialize()override; 
	void Update()override;
	void Draw()override;
	void Move()override;
	void Setradius(float radius);
	void SetInitPosition(D3DXVECTOR3 pos);

private:	
	D3DXVECTOR3     m_EnemyDir;
	float			m_radius;
	D3DXVECTOR3		m_initPosition;		//初期座標。
	float			m_eTurnSpeed;		//回転速度
	CRigidbody		m_Rigidbody;
	//bool			flg;
	bool			m_Hitflag;

	CIsIntersect	m_IsIntersect;		//CIsIntersectのインスタンス
	CCourceDef		m_Courcedef;
	float m_moveSpeed;
};
