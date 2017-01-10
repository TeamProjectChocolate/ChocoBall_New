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
	void SetInitPosition(D3DXVECTOR3 pos);
	void EnemyBulletShot();
	void EnemyDeleteBullet(CEnemyBullet* bullet);
	void ExcuteDeleteBullets();

	bool GetShotflag()
	{
		return Enemyshotflag;
	}
	
private:
	CInterface*	m_pInput;
	float			m_radius;
	D3DXVECTOR3		m_initPosition;		//初期座標。
	float			m_eTurnSpeed;		//回転速度
	//CRigidbody		m_Rigidbody;
	bool			m_Hitflag;
	bool			Enemyshotflag;			//弾が発射されているのかのフラグ
	int				time;

	CCourceDef		m_Courcedef;
	vector<CEnemyBullet*> m_bullets;
	vector<CEnemyBullet*> m_Deletebullets;	// 削除リスト
	D3DXVECTOR3 RV0;
	D3DXMATRIX Rot;
	D3DXVECTOR4 RV1;

	D3DXVECTOR3 m_EnemyToPlayerVec;

	CPlayer* m_pPlayer;
};