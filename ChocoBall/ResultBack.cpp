#include "stdafx.h"
#include "ResultBack.h"
#include "InputManager.h"


CResultBack::CResultBack()
{
}


CResultBack::~CResultBack()
{

}

void CResultBack::Initialize(){
	UseModel<C2DImage>();
	m_pModel->SetFileName("image/result.png");
	CGameObject::Initialize();
	m_transform.position = D3DXVECTOR3(480.0f, 270.0f, 1.0f);
	SetRotation(0.0f);
	m_transform.scale = D3DXVECTOR3(960, 570, 100);
	SetAlive(true);
}

void CResultBack::Update(){
	CGameObject::Update();
}

void CResultBack::Draw(){
	CGameObject::Draw();
}