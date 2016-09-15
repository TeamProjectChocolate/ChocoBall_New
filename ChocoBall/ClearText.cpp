
#include "stdafx.h"

#include "ClearText.h"
#include "C2DImage.h"

CClearText::CClearText()
{
}


CClearText::~CClearText()
{

}

void CClearText::Initialize(){
	UseModel<C2DImage>();
	m_pModel->SetFileName("image/clear.png");
	CGameObject::Initialize();
	m_transform.position = D3DXVECTOR3(450.0f, 250.0f, 1.0f);
	SetRotation(0.0f);
	m_transform.scale = D3DXVECTOR3(420, 420, 100);
	SetAlive(true);
	dir = -1;
	t = 0.0f;
	m_IsEnd = false;
}

void CClearText::Update(){
	if (t <= 1.0f)
	{
		t += 0.015f;
	}
	else{
		m_IsEnd = true;
	}
	m_pModel->m_alpha = t;
	CGameObject::Update();
}

void CClearText::Draw(){
	CGameObject::Draw();
}
