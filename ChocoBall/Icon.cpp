#include "stdafx.h"
#include "Icon.h"
#include "C2DImage.h"


CIcon::CIcon()
{
	strcpy(m_pFileName, "Image/KILL_icon.png");
}


CIcon::~CIcon()
{
}

void CIcon::Initialize(){
	UseModel<C2DImage>();
	m_pModel->SetFileName(m_pFileName);
	CGameObject::Initialize();
	m_transform.position = D3DXVECTOR3(60.0f, 50.0f, 1.0f);
	SetRotation(0.0f);
	m_transform.scale = D3DXVECTOR3(60.0f, 65.0f, 0.0f);
	SetAlive(true);
	m_pModel->m_alpha = 1.0f;
}

void CIcon::Update(){
	CGameObject::Update();
}

void CIcon::Draw(){
	CGameObject::Draw();
}