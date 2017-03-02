#pragma once
#include "stdafx.h"
#include "Enemy_People.h"
#include "Rigidbody.h"
#include "CourceDef.h"
#include "Turn.h"

class CEnemyFB : public CEnemy_People
{
public:
	CEnemyFB();
	~CEnemyFB();
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
	D3DXVECTOR3     m_EnemyDir;
	float			m_radius;
	D3DXVECTOR3		m_initPosition;		//�������W�B
	float			m_eTurnSpeed;		//��]���x
	CRigidbody		m_Rigidbody;
	bool			flg;
	bool			m_Hitflag;

	CIsIntersect	m_IsIntersect;		//CIsIntersect�̃C���X�^���X
	CCourceDef		m_Courcedef;

	float V0;
};