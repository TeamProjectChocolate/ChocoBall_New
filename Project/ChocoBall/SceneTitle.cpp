#include "stdafx.h"
#include "SceneTitle.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "GameManager.h"
#include "Audio.h"
#include "ParticleEmitter.h"
#include "StageManager.h"
#include "Icon.h"


CSceneTitle::CSceneTitle()
{
}


CSceneTitle::~CSceneTitle()
{
}

void CSceneTitle::Initialize(){
	SINSTANCE(CObjectManager)->GenerationObject<CTitleBack>(_T("TitleBack"),OBJECT::PRIORTY::OBJECT2D,false);
	CTitleSelect* start = SINSTANCE(CObjectManager)->GenerationObject<CTitleSelect>(_T("NewGame"), OBJECT::PRIORTY::OBJECT2D_ALPHA,false);
	//CTitleSelect* Continue = SINSTANCE(CObjectManager)->GenerationObject<CTitleSelect>(_T("continue"), PRIORTY::OBJECT2D_ALPHA, false);
	//Continue->SetFileName(_T("image/TAI_EXIT.png"));
	CTitleCursor* cursor = SINSTANCE(CObjectManager)->GenerationObject<CTitleCursor>(_T("Cursor"), OBJECT::PRIORTY::OBJECT2D, false);

	CTitleSelect* PleaseAButton = SINSTANCE(CObjectManager)->GenerationObject<CTitleSelect>(_T("NewGame"), OBJECT::PRIORTY::OBJECT2D_ALPHA, false);

	cursor->SetAudio(m_pAudio);
	//num = SINSTANCE(CObjectManager)->GenerationObject<CNumber>(_T("Number"), PRIORTY::OBJECT2D_ALPHA, false);

	//StageNum = SINSTANCE(CObjectManager)->GenerationObject<CNumber>(_T("StageNumber"), PRIORTY::OBJECT2D_ALPHA, false);
	SINSTANCE(CObjectManager)->Intialize();

	//num->SetPos(D3DXVECTOR3(230.0f, 350.0f, 1.0f));
	//CIcon* obj = SINSTANCE(CObjectManager)->FindGameObject<CIcon>(_T("Clear_Icon"));
	//obj->SetPos(D3DXVECTOR3(60.0f, 430.0f, 1.0f));
	//obj = SINSTANCE(CObjectManager)->FindGameObject<CIcon>(_T("Kill_Icon"));
	//obj->SetPos(D3DXVECTOR3(60.0f, 350.0f, 1.0f));

	//StageNum->SetPos(D3DXVECTOR3(230.0f, 430.0f, 0.0f));

	//SINSTANCE(CObjectManager)->FindGameObject<CIcon>(_T("Kill_Icon"));
	//Continue->SetPos(D3DXVECTOR3(655.0f, 430.0f, 0.0f));//Exitのポジション
	PleaseAButton->SetFileName(_T("image/PreaseButton.png"));
	PleaseAButton->GetModel()->SetImage();
	PleaseAButton->SetPos(D3DXVECTOR3(500.0f, 450.0f, 0.0f));//PleaseAButtonのポジション
	PleaseAButton->SetScale(PleaseAButton->GetScale() + D3DXVECTOR3(0.0f,160.0f,0.0f));
	// 点滅させる。
	PleaseAButton->SetsFlashing(true);
	PleaseAButton->SetFlashingSpeed(0.03);
}

void CSceneTitle::Update(){
	int i = CPlayerBullet::EnemyDownNum;
	//num->SetValue(i);
	//StageNum->SetValue(CStageManager::m_ClearNum);
	SINSTANCE(CObjectManager)->Update();
}

void CSceneTitle::Draw(){
	SINSTANCE(CObjectManager)->Draw();
}

void CSceneTitle::Release(){

}