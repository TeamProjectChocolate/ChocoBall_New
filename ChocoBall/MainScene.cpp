#include "stdafx.h"
#include "MainScene.h"
#include "ObjectManager.h"
#include "Audio.h"
#include "StageManager.h"


CMainScene::CMainScene(){
}

CMainScene::~CMainScene(){
}

void CMainScene::Initialize(){
	srand((unsigned int)time(NULL));
	SINSTANCE(CStageManager)->Initialize();
}

void CMainScene::Update(){
	SINSTANCE(CStageManager)->Update();
}

void CMainScene::Draw(){
	SINSTANCE(CStageManager)->Draw();
	SINSTANCE(CObjectManager)->FindGameObject<CEnemyManager>(_T("EnemyManager"))->ExcuteDeleteObjects();
}
