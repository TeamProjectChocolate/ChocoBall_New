#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "BulletPhysics.h"

class CDivisionWall :
	public CGameObject
{
public:
	CDivisionWall();
	~CDivisionWall();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Build(const D3DXVECTOR3&, const D3DXQUATERNION&);

	void SetUpTechnique()override {
#ifdef NOT_EM
		m_pRender->SetUpTechnique("Boneless_Tex_Lim");
#else
		m_pRender->SetUpTechnique("Boneless_Tex_Fresnel");
#endif
	}
	void EM_SetUpTechnique()override {
		m_pEMSamplingRender->SetUpTechnique("Boneless_Tex_Lim");
	}

	void SetIsMove(bool flg) {
		m_IsMove = flg;
	}
private:
	bool m_IsMove = false;
private:
	// 作業用変数。
	float m_Length;	// 移動した量。
	float m_moveSpeed;
};

