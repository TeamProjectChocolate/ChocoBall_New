#include "stdafx.h"
#include "TestObject2D.h"
#include "InputManager.h"


CTestObject2D::CTestObject2D()
{
	strcpy(m_pFileName, "image/test.png");
}


CTestObject2D::~CTestObject2D()
{
}

void CTestObject2D::Initialize(){
	CGameObject::Initialize();
	m_Input = SINSTANCE(CInputManager)->GetCurrentInput();
	m_transform.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f);
	m_transform.scale = D3DXVECTOR3(96, 80, 1);
	m_pModel->m_alpha = 0.5f;
	SetAlive(true);
	m_angle = 0.0f;
}

void CTestObject2D::Update(){ 
	if (m_Input->IsPressUp()){
		m_transform.position.y--/* -= 0.1f*/;
	}
	if (m_Input->IsPressDown()){
		m_transform.position.y++/* += 0.1f*/;
	}
	if (m_Input->IsPressLeft()){
		m_transform.position.x--/* -= 0.1f*/;
	}
	if (m_Input->IsPressRight()){
		m_transform.position.x++/* += 0.1f*/;
	}
	m_angle++;
	SetRotation(D3DXToRadian(m_angle));
	CGameObject::Update();
}

void CTestObject2D::Draw(){
	CGameObject::Draw();
}
