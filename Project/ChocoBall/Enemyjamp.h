#pragma once
#include "stdafx.h"
#include "Enemy_People.h"
#include "Rigidbody.h"
#include "CourceDef.h"
#include "Turn.h"
#include "Player.h"

class CEnemyjamp : public CEnemy_People
{
public:
	CEnemyjamp();
	~CEnemyjamp();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Move()override;
	void Setradius(float radius)
	{
		m_radius = radius;
	}
	void SetInitPosition(D3DXVECTOR3 pos);
private:
	CPlayer* m_pPlayer = nullptr;
	float			m_radius;
	D3DXVECTOR3		m_initPosition;		//初期座標。
	bool			flg;
	bool			m_Hitflag;

	CIsIntersect	m_IsIntersect;		//CIsIntersectのインスタンス
	CCourceDef		m_Courcedef;

	float m_DeltaTime;
	float m_WaitTime;
};
