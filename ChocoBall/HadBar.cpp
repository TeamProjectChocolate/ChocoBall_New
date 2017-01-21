#include "stdafx.h"
#include "HadBar.h"


CHadBar::CHadBar()
{
	strcpy(m_pFileName, "Image/KILL_icon.png");
}


CHadBar::~CHadBar()
{
}

void CHadBar::Initialize() {
	UseModel<C2DImage>();
	m_pModel->SetFileName(m_pFileName);
	CGameObject::Initialize();
	m_transform.position = D3DXVECTOR3(60.0f, 50.0f, 1.0f);
	SetRotation(0.0f);
	m_transform.scale = D3DXVECTOR3(60.0f, 65.0f, 0.0f);
	SetAlive(true);
	m_pModel->m_alpha = 1.0f;
}

void CHadBar::Update() {
	CGameObject::Update();
}

void CHadBar::Draw() {
	CGameObject::Draw();
}
