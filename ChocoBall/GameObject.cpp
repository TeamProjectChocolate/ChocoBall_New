#include "stdafx.h"
#include "GameObject.h"
#include "Effect.h"
#include "RenderContext.h"
#include "Camera.h"

CGameObject::~CGameObject()
{
	if (m_UseModel){
		SAFE_DELETE(m_pModel);
	}
}

void CGameObject::Initialize(){
	m_pModel->Initialize();
	SetRenderState();
	m_pRender = SINSTANCE(CRenderContext)->SelectRender(m_RenderingState,m_pRenderName,false,m_pModel);
	SetDepthRenderState();
	m_pDepthRender = SINSTANCE(CRenderContext)->SelectRender(m_DepthRenderingState, m_pRenderName, false, m_pModel);
}

void CGameObject::Update(){
	m_pModel->Update(m_transform);
}

void CGameObject::Draw(){
	SetUpTechnique();
	m_pRender->SetModelData(m_pModel);
	m_pRender->Draw();
}

void CGameObject::DrawSimple(){
	m_pRender->SetModelData(m_pModel);
	m_pRender->Draw();
}

void CGameObject::DrawDepth(const D3DXVECTOR2& FarNear,const D3DXVECTOR3& PintoPos,const D3DXMATRIX& PintoWorld){
	m_pDepthRender->SetModelData(m_pModel);
	m_pDepthRender->Draw(FarNear,PintoPos,PintoWorld);
}

void CGameObject::DrawShadow(CCamera* camera){
	m_pShadowRender->SetShadowCamera(camera);
	m_pShadowRender->SetModelData(m_pModel);
	m_pShadowRender->Draw();
}
