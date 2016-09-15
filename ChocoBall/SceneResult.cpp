#include "stdafx.h"
#include "SceneResult.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "GameManager.h"
#include "Icon.h"
#include "StageManager.h"


CSceneResult::CSceneResult()
{
}


CSceneResult::~CSceneResult()
{
}

void CSceneResult::Initialize(){
	m_pInput = SINSTANCE(CInputManager)->GetCurrentInput();
	SINSTANCE(CObjectManager)->AddObject(&m_rBack, _T("ResultBack"),PRIORTY::OBJECT2D, false);
	SINSTANCE(CObjectManager)->AddObject(&m_rSelect[0], _T("Continue"), PRIORTY::OBJECT2D,false);
	//m_rSelect[1].SetFileName(_T("image/result_EXIT.png"));
	//SINSTANCE(CObjectManager)->AddObject(&m_rSelect[1], _T("GoTitle"), false);
	SINSTANCE(CObjectManager)->AddObject(&m_rCursor, _T("ResultCursor"), PRIORTY::OBJECT2D,false); 
	m_rCursor.SetAudio(m_pAudio);
	num = SINSTANCE(CObjectManager)->GenerationObject<CNumber>(_T("Number"), PRIORTY::OBJECT2D_ALPHA, false);
	StageNum = SINSTANCE(CObjectManager)->GenerationObject<CNumber>(_T("StageNumber"), PRIORTY::OBJECT2D_ALPHA, false);
	SINSTANCE(CObjectManager)->Intialize();
	StageNum->SetPos(D3DXVECTOR3(230.0f, 130.0f, 0.0f));
	CIcon* obj = SINSTANCE(CObjectManager)->FindGameObject<CIcon>(_T("Clear_Icon"));
	obj->SetFileName(_T("Image/CLEAR_icon.png"));
	obj->SetPos(D3DXVECTOR3(60.0f, 130.0f, 1.0f));
	obj->SetScale(D3DXVECTOR3(60.0f, 65.0f, 0.0f));

	//SINSTANCE(CObjectManager)->FindGameObject<CResultSelect>(_T("GoTitle"))->SetPos(D3DXVECTOR3(790.0f, 400.0f, 0.0f));//Exit‚Ìƒ|ƒWƒVƒ‡ƒ“
	m_IsEnd = false;
}

void CSceneResult::Update(){
	num->SetValue(CPlayerBullet::EnemyDownNum);
	StageNum->SetValue(CStageManager::m_ClearNum);
	SINSTANCE(CObjectManager)->Update();
}
void CSceneResult::Draw(){
	SINSTANCE(CObjectManager)->Draw();
}
