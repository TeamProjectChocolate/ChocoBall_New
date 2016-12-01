#include "stdafx.h"
#include "GameManager.h"
#include "ObjectManager.h"
#include "ShadowRender.h"
#include "RenderContext.h"
#include "Audio.h"
#include "DirectFont.h"

CGameManager* CGameManager::m_instance = nullptr;

void CGameManager::AddScene(CScene* SceneInstance, LPCSTR SceneName){
	CH_ASSERT(strlen(SceneName) <= SCENENAME_MAX);
	if (FindScene(SceneName) != nullptr){
		MessageBox(NULL, "‚»‚Ì–¼‘O‚ÌSceneƒCƒ“ƒXƒ^ƒ“ƒX‚Í‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·", 0, 0);
		return;
	}
	SCENE_DATA* SceneData = new SCENE_DATA;
	strcpy(SceneData->SceneName, SceneName);
	SceneData->Scene = SceneInstance;
	Add(SceneData);
}

void CGameManager::Add(SCENE_DATA* data){
	m_Scenes.push_back(data);
}

CScene* CGameManager::FindScene(LPCSTR SceneName){
	CH_ASSERT(strlen(SceneName) <= SCENENAME_MAX);
	int size = m_Scenes.size();
	for (int idx = 0; idx < size; idx++){
		if (!strcmp(m_Scenes[idx]->SceneName, SceneName)){
			return m_Scenes[idx]->Scene;
		}
	}
	return nullptr;
}

void CGameManager::ChangeScene(LPCSTR SceneName){
	CH_ASSERT(strlen(SceneName) <= SCENENAME_MAX);
	strcpy(m_NextSceneName, SceneName);
}

void CGameManager::SetNextScene(){
	if (!strcmp(m_NowSceneName, m_NextSceneName)){
		return;
	}
	SINSTANCE(CShadowRender)->CleanManager();
	SINSTANCE(CObjectManager)->CleanManager();
	SINSTANCE(CShadowRender)->ExcuteDeleteObjects();
	SINSTANCE(CObjectManager)->ExcuteDeleteObjects();
	SINSTANCE(CRenderContext)->ChangeScenedProcess();
	m_NowScene = FindScene(m_NextSceneName);
	if (m_NowScene == nullptr){
		MessageBox(nullptr, _T("ƒV[ƒ“‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"), _T("error"), MB_OK);
		abort();
	}
	m_pAudio->DeleteAll();
	m_NowScene->SetAudio(m_pAudio);
	m_NowScene->Initialize();
	strcpy(m_NowSceneName, m_NextSceneName);
}

void CGameManager::Initialize(){
	m_NowScene->Initialize();
}

void CGameManager::Update(){
	m_NowScene->Update();
}

void CGameManager::Draw(){
	m_NowScene->Draw();
}

void CGameManager::DeleteAll(){
	for (int idx = 0,size = m_Scenes.size(); idx < size; idx++){
		SAFE_DELETE(m_Scenes[idx]->Scene);
		SAFE_DELETE(m_Scenes[idx]);
	}
	m_Scenes.clear();
}