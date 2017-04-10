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
	void Build(const D3DXVECTOR3&, const D3DXQUATERNION&,const D3DXVECTOR3& scale);

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
	inline void SetScale(const D3DXVECTOR3& scale)override {
		m_transform.scale.x = scale.x * m_OriginSize.x;
		m_transform.scale.y = scale.y * m_OriginSize.y;
		m_transform.scale.z = scale.z * m_OriginSize.z;
	}
private:
	bool m_IsMove = false;
private:
	// ��Ɨp�ϐ��B
	float m_Length;	// �ړ������ʁB
	float m_moveSpeed;
	D3DXVECTOR3 m_OriginSize;	// ���̃T�C�Y�B
};

