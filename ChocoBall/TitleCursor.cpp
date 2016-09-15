#include "stdafx.h"
#include "TitleCursor.h"
#include "InputManager.h"
#include "DirectX.h"
#include "GameManager.h"
#include "TitleSelect.h"
#include "ObjectManager.h"
#include "Player.h"
#include "C2DImage.h"


CTitleCursor::CTitleCursor()
{
	isup = false;
	isdown = false;
}


CTitleCursor::~CTitleCursor()
{
	m_pAudio->DeleteAll();
}

void CTitleCursor::Initialize(){
	UseModel<C2DImage>();
	m_pModel->SetFileName("image/arrow.png");
	CGameObject::Initialize();
	m_pInput = SINSTANCE(CInputManager)->GetCurrentInput();
	m_transform.position = D3DXVECTOR3(510.0f, 300.0f, 1.0f);
	SetRotation(0.0f);
	m_transform.scale = D3DXVECTOR3(75, 80, 10);
	SetAlive(true);
	m_pAudio->PlayCue("Title", false,this);	// 音楽再生
}

void CTitleCursor::Update(){
	SINSTANCE(CInputManager)->IsInputChanged(&m_pInput);
	if (m_pInput->IsTriggerDecsion() && m_transform.position.y == 300.0f){
		m_pAudio->PlayCue("オケヒット", false, this);
		m_pAudio->StopCue("Title",false,this);
		m_pAudio->DeleteAll();
		SINSTANCE(CGameManager)->ChangeScene(_T("Main"));
	}
	//if (m_pInput->IsTriggerDecsion() && m_transform.position.y == 430.0f){
	//	// ここの中身の処理はセーブを実装し手続きから遊ぶの処理にする
	//	m_pAudio->PlayCue("オケヒット", false, this);
	//	PostQuitMessage(0);
	//}
	
	//float Y = m_pInput->GetStickL_YFloat();
	//if (Y>0){
	//	m_transform.position.y = 300.0f;
	//	m_transform.position.x = 510.0f;
	//	if (!isup)
	//	{
	//		m_pAudio->PlayCue("LAPUTA_counter_2", true,this);
	//		isup = true;
	//	}
	//}
	//else
	//{
	//	isup = false;
	//}
	//if (Y<0){
	//	m_transform.position.y = 430.0f;
	//	m_transform.position.x = 592.0f;
	//	if (!isdown)
	//	{
	//		m_pAudio->PlayCue("LAPUTA_counter_2", true,this);
	//		isdown = true;
	//	}
	//	
	//}
	//else
	//{
	//	isdown = false;
	//}
	m_pAudio->Run();
	CGameObject::Update();
}

void CTitleCursor::Draw(){
	CGameObject::Draw();
}

void CTitleCursor::Release(){
	SAFE_DELETE(m_pAudio);
}