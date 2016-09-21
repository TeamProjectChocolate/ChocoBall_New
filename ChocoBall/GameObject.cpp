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
}

void CGameObject::Update(){
	m_pModel->Update(m_transform);
}

void CGameObject::Draw(){
	SetUpTechnique();
	m_pRender->SetModelData(m_pModel);
	m_pRender->Draw();
}

void CGameObject::DrawShadow(CCamera* camera){
	m_pShadowRender->SetShadowCamera(camera);
	m_pShadowRender->SetModelData(m_pModel);
	m_pShadowRender->Draw();
}
