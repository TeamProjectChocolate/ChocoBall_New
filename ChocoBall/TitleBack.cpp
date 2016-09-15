#include "stdafx.h"
#include "TitleBack.h"
#include "InputManager.h"
#include "C2DImage.h"


CTitleBack::CTitleBack()
{
}


CTitleBack::~CTitleBack()
{
	SAFE_DELETE(m_pModel);
}

void CTitleBack::Initialize(){
	UseModel<C2DImage>();
	m_pModel->SetFileName("image/title.png");
	CGameObject::Initialize();
	m_transform.position = D3DXVECTOR3(480.0f, 270.0f, 1.0f);
	SetRotation(0.0f);
	m_transform.scale = D3DXVECTOR3(960, 540, 100);
	SetAlive(true);
}

void CTitleBack::Update(){
	CGameObject::Update();
}

void CTitleBack::Draw(){
	CGameObject::Draw();
}