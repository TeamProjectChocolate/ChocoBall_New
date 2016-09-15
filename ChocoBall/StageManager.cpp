#include "stdafx.h"
#include "StageManager.h"
#include "GameManager.h"
#include "ShadowRender.h"


CStageManager* CStageManager::m_instance = nullptr;

int CStageManager::m_ClearNum = 0;

void CStageManager::Initialize()
{
	m_NowStage = STAGE_ID::STAGE_NONE;
	ChangeStage(STAGE_ID::FIRST);
	SetNextStage();
}

void CStageManager::Update(){
	m_pStage->Update();
}

void CStageManager::Draw()
{
	m_pStage->Draw();
}

void CStageManager::ChangeStage(STAGE_ID id){
	m_NextStage = id;
}

void CStageManager::SetNextStage(){
	if (m_IsContinue){
		if (m_ContinueStage >= STAGE_ID::MAX && m_ContinueStage != STAGE_ID::STAGE_NONE){
			// ÅŒã‚Ü‚ÅƒNƒŠƒA‚µ‚Ä‚¢‚½‚ç‚±‚Ìˆ—‚ð’Ê‚é
			SINSTANCE(CShadowRender)->CleanManager();
			SINSTANCE(CObjectManager)->CleanManager();
			SAFE_DELETE(m_pStage);
			m_pStage = nullptr;
			m_IsContinue = false;
			SINSTANCE(CGameManager)->ChangeScene(_T("Title"));
			SINSTANCE(CGameManager)->SetNextScene();
			return;
		}
	}

	if (m_NowStage != m_NextStage){
		if (m_IsContinue){
			m_NowStage = m_ContinueStage;
		}
		else{
			m_NowStage = m_NextStage;
		}
		SINSTANCE(CShadowRender)->CleanManager();
		SINSTANCE(CObjectManager)->CleanManager();
		SAFE_DELETE(m_pStage);
		m_pStage = new CStage;
		m_pAudio->DeleteAll();
		m_pStage->Initialize(m_pAudio,m_NowStage);
		m_ContinueStage = m_NowStage;
	}
}

void CStageManager::DeleteAll(){
}

