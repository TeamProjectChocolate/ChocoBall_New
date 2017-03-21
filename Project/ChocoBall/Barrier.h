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

	// バリアを切る。
	void OffBarrier();
	// バリア起動。
	void OnBarrier();
private:
	// 作業用。
	bool m_IsBarrierOn = false;		// バリアが起動されたか。
	float m_MaxRadius;	// バリアの最大半径。
	float m_Radius;		// バリアの実半径。
	float m_AddRadius = 0.25f;	// マイフレームこの値ずつ実半径に加算。
	float m_InitAlpha = 0.5f;	// 初期不透明度。
};

