#pragma once
#include "GameObject.h"
#include "BulletPhysics.h"

class CBarrier :
	public CGameObject
{
public:
	CBarrier();
	~CBarrier();
	void Initialize()override;
	void Build(const D3DXVECTOR3&,float);
	void Update()override;

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

	// �o���A��؂�B
	void OffBarrier();
	// �o���A�N���B
	void OnBarrier();
private:
	// ��Ɨp�B
	bool m_IsBarrierOn = false;		// �o���A���N�����ꂽ���B
	float m_MaxRadius;	// �o���A�̍ő唼�a�B
	float m_Radius;		// �o���A�̎����a�B
	float m_AddRadius = 0.25f;	// �}�C�t���[�����̒l�������a�ɉ��Z�B
	float m_InitAlpha = 0.5f;	// �����s�����x�B
};

