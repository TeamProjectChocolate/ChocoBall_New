#include "stdafx.h"
#include "GameObject.h"
#include "Effect.h"
#include "RenderContext.h"
#include "Camera.h"
#include "EM_SamplingRender.h"
#include "EM_SamplingRender_I.h"
#include "ActreLight.h"

CGameObject::~CGameObject()
{
	if (m_UseModel){
		SAFE_DELETE(m_pModel);
	}
	if (m_HasMyLight) {
		SAFE_DELETE(m_pLight);
	}
}

void CGameObject::Initialize(){
	m_pModel->Initialize();
	SetRenderState();
	m_pRender = SINSTANCE(CRenderContext)->SelectRender(m_RenderingState,m_pRenderName,false,m_pModel);
#ifdef NOT_EM
#else
	EM_SetRenderState();
	m_pEMSamplingRender = SINSTANCE(CRenderContext)->SelectRender(m_EMRenderingState, m_pRenderName, false, m_pModel);
#endif
}

void CGameObject::InitInstancing(int num,bool isNull){
	if (isNull){
		if (static_cast<CInstancingRender*>(m_pRender)->GetWorldMatrixBuffer() == nullptr){
			static_cast<CInstancingRender*>(m_pRender)->CreateMatrixBuffer(num);
		}
	}
	else{
		static_cast<CInstancingRender*>(m_pRender)->CreateMatrixBuffer(num);
	}
#ifdef NOT_EM
#else
	static_cast<CEM_SamplingRender_I*>(m_pEMSamplingRender)->CreateMatrixBuffer(num);
#endif

}

void CGameObject::Update(){
	m_pModel->Update(m_transform);
	if (m_HasMyLight) {
		// 女優ライトを使用する場合はワールド行列でライトを更新する。
		static_cast<CActreLight*>(m_pLight)->Update(m_pModel->m_World);
	}
}

void CGameObject::Draw(){
	SetUpTechnique();
	m_pRender->SetModelData(m_pModel);
	m_pRender->SetMyLight(m_pLight);
	m_pRender->Draw();
}

void CGameObject::DrawShadow(CCamera* camera){
	SetUpShadowTechnique();
	m_pShadowRender->SetShadowCamera(camera);
	m_pShadowRender->SetModelData(m_pModel);
	Is_DrawShadow_Use_Horizon();
	m_pShadowRender->Draw();
}

void CGameObject::Draw_EM(CCamera* camera){
#ifdef NOT_EM
#else
	EM_SetUpTechnique();
	static_cast<CEM_SamplingRender*>(m_pEMSamplingRender)->SetCamera(camera);
	m_pEMSamplingRender->SetModelData(m_pModel);
	m_pEMSamplingRender->Draw();
#endif
}

void CGameObject::Is_DrawShadow_Use_Horizon(){
	m_pModel->SetIsHorizon(false);
}