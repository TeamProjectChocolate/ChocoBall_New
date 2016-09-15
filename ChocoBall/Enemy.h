#pragma once
#include "stdafx.h"
#include "islntersect.h"
#include "CourceDef.h"
#include "Turn.h"
#include "EnemyBase.h"
#include "EnemyBullet.h"
#include "Player.h"


class CEnemy : public EnemyBase
{
public:
	CEnemy();
	~CEnemy();
	void Initialize()override;
	void Update()override;
	void Draw()override;
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
	D3DXVECTOR3		m_moveSpeed;		//移動速度。
	D3DXVECTOR3		m_initPosition;		//初期座標。
	float			m_eCurrentAngleY;	//現在の方向
	float			m_eTargetAngleY;		//向きたい方向
	float			m_eTurnSpeed;		//回転速度
	bool			isTurn;				//回転フラグ
	//CRigidbody		m_Rigidbody;
	bool			flg;
	bool			m_Hitflag;
	bool			Enemyshotflag;			//弾が発射されているのかのフラグ
	float			m_currentAngleY;	//現在の方向
	int				time;

	CTurn			m_Turn;				//CTurnのインスタンス
	CCourceDef		m_Courcedef;
	vector<CEnemyBullet*> m_bullets;
	vector<CEnemyBullet*> m_Deletebullets;	// 削除リスト
	D3DXVECTOR3 RV0;
	D3DXMATRIX Rot;
	D3DXVECTOR4 RV1;

	CPlayer* m_pPlayer;


	D3DXVECTOR3		m_V0;
	float V0;
	D3DXVECTOR3		V1;
	D3DXVECTOR3 m_V1;
	D3DXVECTOR3		V2;
	D3DXVECTOR3		m_V2;
	D3DXVECTOR3 m_V3;
	float V3;
	D3DXVECTOR3 m_Up;
};