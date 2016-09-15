#include "stdafx.h"
#include "ResultSelect.h"
#include "InputManager.h"
#include "GameObject.h"
#include "ResultCursor.h"
#include "ObjectManager.h"
#include "C2DImage.h"


CResultSelect::CResultSelect()
{
}

CResultSelect::~CResultSelect()
{
	
}

void CResultSelect::Initialize(){
	UseModel<C2DImage>();
	m_pModel->SetFileName("image/result_NEXT.png");
	CGameObject::Initialize();
	m_transform.position = D3DXVECTOR3(810.0f, 300.0f, 1.0f);
	SetRotation(0.0f);
	m_transform.scale = D3DXVECTOR3(281, 98, 0);
	SetAlive(true);
	m_resultcursor = SINSTANCE(CObjectManager)->FindGameObject<CResultCursor>(_T("ResultCursor"));
}

void CResultSelect::Update(){
	if (m_transform.position.y == m_resultcursor->GetPos().y){
		m_pModel->m_alpha = 1.0f;
	}
	else{
		m_pModel->m_alpha = 0.7f;
	}
	CGameObject::Update();
}

void CResultSelect::Draw(){
	CGameObject::Draw();
}