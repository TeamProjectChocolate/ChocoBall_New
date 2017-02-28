#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "islntersect.h"
#include "Rigidbody.h"
#include "BuildBlock.h"

//#define BULLET_LENG 20.0f

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
#ifdef NOT_EM
		m_pRender->SetUpTechnique("Boneless_Tex_Lim");
#else
		m_pRender->SetUpTechnique("Boneless_Tex_Fresnel");
#endif
	};
	void EM_SetUpTechnique()override{
		m_pEMSamplingRender->SetUpTechnique("Boneless_Tex_Lim");
	}
	void OnDestroy();
	void Build();
	virtual void BulletCollision() {};
	inline void SetDir(const D3DXVECTOR3& dir){
		m_dir = dir;
	}
	inline const D3DXVECTOR3& GetDirection(){
		return m_dir;
	}
	inline void SetBulletSpeed(float Speed)
	{
		m_Speed = Speed;
	}
	inline float GetHeight(){
		return BULLET_HEIGHT;
	}
	inline float GetWidth(){
		return BULLET_WIDTH;
	}
	inline void SetStartPos(const D3DXVECTOR3& pos) {
		m_transform.position = m_StartPos = pos;
	}
	inline const D3DXVECTOR3& GetStartPos() {
		return m_StartPos;
	}
	inline void SetRange(float range) {
		m_Range = range;
	}
	inline float GetRange() {
		return m_Range;
	}

private:
	D3DXVECTOR3 m_StartPos;	// 初期位置。
	D3DXVECTOR3		m_moveSpeed;//移動速度。
	float m_Range;	// 弾の飛距離。
	bool			Shotflag;	//弾が発射されているのかのフラグ
	CIsIntersect	m_IsIntersect;	//CIsIntersectのインスタンス
	float			m_radius;
	D3DXVECTOR3		m_dir;

	float m_Speed;//弾のスピード
};

