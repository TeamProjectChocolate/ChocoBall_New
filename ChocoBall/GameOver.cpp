#include "stdafx.h"
#include "GameOver.h"


CGameOver::CGameOver()
{
}


CGameOver::~CGameOver()
{

}

void CGameOver::Initialize(){
	UseModel<C2DImage>();
	m_pModel->SetFileName("image/over.png");
	CGameObject::Initialize();
	m_transform.position = D3DXVECTOR3(480.0f, 0.0f, 1.0f);
	SetRotation(0.0f);
	m_transform.scale = D3DXVECTOR3(420, 420, 100);
	SetAlive(true);
	m_IsEnd = false;	
}

void CGameOver::Update(){
	if (m_transform.position.y <= 250){
		m_transform.position.y+=2;
	}
	else{
		m_IsEnd = true;
	}
	CGameObject::Update();
}

void CGameOver::Draw(){
	CGameObject::Draw();
}
