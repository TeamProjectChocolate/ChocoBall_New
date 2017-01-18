#pragma once
#include "GameObject.h"
#include "BulletPhysics.h"
#include "StageTable.h"
#include "C3DObjectRender.h"
#include "Infomation.h"
#include "CourceDef.h"

class CZBufferSphere;

class CField :
	public CGameObject
{
public:
	CField(){
		m_Horizon.clear();
		UseModel<C3DImage>();
	};
	~CField();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void SetUpTechnique()override{
#ifdef NOT_VSM
		if (m_IsFresnel) {
				m_pRender->SetUpTechnique("Boneless_Tex_Shadow_ZMask_Fresnel");
		}
		else {
			m_pRender->SetUpTechnique("Boneless_Tex_Shadow_ZMask");
		}
#else
		if (m_IsFresnel) {
			m_pRender->SetUpTechnique("Boneless_Tex_Shadow_VSM_ZMask_Fresnel");
		}
		else {
			m_pRender->SetUpTechnique("Boneless_Tex_Shadow_VSM_ZMask");
		}
#endif
	}
	void EM_SetUpTechnique()override{
#ifdef NOT_VSM
		m_pEMSamplingRender->SetUpTechnique("Boneless_Tex_Shadow");
#else
		m_pEMSamplingRender->SetUpTechnique("Boneless_Tex_Shadow_VSM");
#endif
	}
	void SetUpShadowTechnique()override {
#ifdef NOT_VSM
		m_pShadowRender->SetUpTechnique("BonelessShadowMapping");
#else
		m_pShadowRender->SetUpTechnique("BonelessShadowMapping_Horizon");
#endif
	}

	void Is_DrawShadow_Use_Horizon()override;

	void SetZBufferSphere(CZBufferSphere* pSphere) {
		m_czbuffersphere = pSphere;
	}
	void SetStageID(STAGE_ID id) {
		m_StageID = id;
	}
	void SetIsFresnel(bool flg) {
		m_IsFresnel = flg;
	}
private:
	STAGE_ID m_StageID;
	CZBufferSphere* m_czbuffersphere;
	vector<float> m_Horizon;
	CCourceDef m_CourceDef;
	bool m_IsFresnel;	// フレネル反射を適応するか。
};

