#include "stdafx.h"
#include "Skybox.h"
#include "RenderContext.h"
#include "tkStopwatch.h"
#include "C3DImage.h"

Skybox::Skybox(){
	light.SetAmbientLight(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}

Skybox::~Skybox(){}

void Skybox::Initialize()
{
	m_transform.position = D3DXVECTOR3(0.0f, -100.0f, 0.0f);
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/sky.x");
	CGameObject::Initialize();
	SetAlive(true);
}

void Skybox::Update()
{
	CGameObject::Update();
}

void Skybox::Draw()
{
	m_pModel->m_luminance = 1.0f;
	CLight* plight;
	SetUpTechnique();
	plight = SINSTANCE(CRenderContext)->GetCurrentLight();
	SINSTANCE(CRenderContext)->SetCurrentLight(&light);
	CGameObject::Draw();
	SINSTANCE(CRenderContext)->SetCurrentLight(plight);
}

void Skybox::Draw_EM(CCamera* camera)
{
	m_pModel->m_luminance = 1.0f;
	CLight* plight;
	EM_SetUpTechnique();
	plight = SINSTANCE(CRenderContext)->GetCurrentLight();
	SINSTANCE(CRenderContext)->SetCurrentLight(&light);
	CGameObject::Draw_EM(camera);
	SINSTANCE(CRenderContext)->SetCurrentLight(plight);
}