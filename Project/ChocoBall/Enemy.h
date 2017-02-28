#pragma once
#include "stdafx.h"
#include "Enemy_People.h"
#include "CourceDef.h"
#include "Turn.h"
#include "EnemyBullet.h"
#include "Player.h"


class CEnemy : public CEnemy_People
{
public:
	CEnemy();
	~CEnemy();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Move()override;
	void Setradius(float radius);
	void EnemyBulletShot();
	bool GetShotflag()
	{
		return Enemyshotflag;
	}
	
private:
	CInterface*	m_pInput;
	float			m_radius;
	float			m_eTurnSpeed;		//‰ñ“]‘¬“x
	//CRigidbody		m_Rigidbody;
	bool			m_Hitflag;
	bool			Enemyshotflag;			//’e‚ª”­ŽË‚³‚ê‚Ä‚¢‚é‚Ì‚©‚Ìƒtƒ‰ƒO
	int				time;

	CCourceDef		m_Courcedef;
	D3DXVECTOR3 RV0;
	D3DXMATRIX Rot;
	D3DXVECTOR4 RV1;

	D3DXVECTOR3 m_EnemyToPlayerVec;

	CPlayer* m_pPlayer;
};