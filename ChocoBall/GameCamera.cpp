#include "stdafx.h"
#include "GameCamera.h"
#include "RenderContext.h"
#include "ObjectManager.h"
#include "Player.h"


CGameCamera::CGameCamera()
{
}


CGameCamera::~CGameCamera()
{
}

void CGameCamera::Initialize(){
	m_camera.Initialize();
	m_camera.SetAspect(static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT);
	SetAlive(true);
}

void CGameCamera::Update(){
	m_camera.Update();
}

void CGameCamera::Draw(){
	SINSTANCE(CRenderContext)->SetCurrentCamera(&m_camera);
}
