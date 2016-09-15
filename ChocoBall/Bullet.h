#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "islntersect.h"
#include "Rigidbody.h"
#include "BuildBlock.h"

#define BULLET_LENG 20.0f

#define BULLET_HEIGHT 1.0f
#define BULLET_WIDTH 1.0f


class Bullet : public CGameObject
{
public:
	Bullet()
	{
	};
	~Bullet();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void SetUpTechnique()override
	{
		m_pRender->SetUpTechnique("NotNormalMapNonAnimationFresnelBloomTec");
	};
	void OnDestroy();
	void Build();
	void SetDir(D3DXVECTOR4 dir){
		m_dir = dir;
	}
	D3DXVECTOR3 GetDirection(){
		return m_dir;
	}
	void SetBulletSpeed(float Speed)
	{
		m_Speed = Speed;
	}
	float GetHeight(){
		return BULLET_HEIGHT;
	}
	float GetWidth(){
		return BULLET_WIDTH;
	}

private:
	D3DXVECTOR3		m_position;	//弾の座標
	D3DXVECTOR3		m_moveSpeed;//移動速度。
	bool			Shotflag;	//弾が発射されているのかのフラグ
	CIsIntersect	m_IsIntersect;	//CIsIntersectのインスタンス
	float			m_radius;
	D3DXVECTOR4		m_dir;

	float m_Speed;//弾のスピード
};

