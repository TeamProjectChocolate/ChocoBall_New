#include "stdafx.h"
#include "TitleSelect.h"
#include "InputManager.h"
#include "GameObject.h"
#include "TitleCursor.h"
#include "ObjectManager.h"
#include "C2DImage.h"

CTitleSelect::CTitleSelect()
{
}


CTitleSelect::~CTitleSelect()
{

}

void CTitleSelect::Initialize(){
	UseModel<C2DImage>();
	m_pModel->SetFileName("image/TAI_START.png");
	CGameObject::Initialize();
	m_transform.position = D3DXVECTOR3(595.0f, 300.0f, 1.0f);
	SetRotation(0.0f);
	m_transform.scale = D3DXVECTOR3(310, 84, 0);
	SetAlive(true);
	dir = -1;
	m_cursor = SINSTANCE(CObjectManager)->FindGameObject<CTitleCursor>(_T("Cursor"));
	t = 1.0f;
	m_FlashingSpeed = 0.015f;
}

void CTitleSelect::Update(){
	if (m_transform.position.y == m_cursor->GetPos().y || m_IsFlashing){
		if (t >= 1.0f){
			dir = -1;
		}
		else if (t <= 0.0f)
		{
			dir = 1;
		}
		t += m_FlashingSpeed * dir;
		m_pModel->m_alpha = t;
	}
	else{
		dir = -1;
		t = 1.0f;
		m_pModel->m_alpha = 1.0f;
	}
	CGameObject::Update();
}

void CTitleSelect::Draw(){
	//C2DImage::SetupMatrices();
	CGameObject::Draw();
}