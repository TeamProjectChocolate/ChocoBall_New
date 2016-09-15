#include "stdafx.h"
#include "Stage.h"
#include "ObjectManager.h"
#include "ShadowRender.h"
#include "ClearText.h"
#include "GameOver.h"
#include "StageManager.h"
#include "StageTable.h"
#include "ParticleEmitter.h"
#include "Icon.h"

CStage::CStage()
{
	m_pPlayer = nullptr;
	m_pAudio = nullptr; 
	m_pEmitter = nullptr;
}

CStage::~CStage()
{
}

void CStage::Initialize(CAudio* pAudio,STAGE_ID NowId)
{
	m_Stage_ID = NowId;

	ActivateObjects();

	m_pCamera = SINSTANCE(CObjectManager)->FindGameObject<CCourceCamera>(_T("Camera"));
	m_pCamera->SetStageID(m_Stage_ID);

	m_pEnemyManager = SINSTANCE(CObjectManager)->FindGameObject<CEnemyManager>(_T("EnemyManager"));
	m_pEnemyManager->SetStageID(m_Stage_ID);

	m_pPlayer = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("TEST3D"));
	m_pPlayer->SetStageID(m_Stage_ID);
	m_pPlayer->SetAudio(pAudio);

	SINSTANCE(CObjectManager)->FindGameObject<CField>(_T("TESTStage3D"))->SetStageID(m_Stage_ID);
	SINSTANCE(CObjectManager)->GenerationObject<Skybox>(_T("skybox"), PRIORTY::OBJECT3D, false);
	SINSTANCE(CObjectManager)->GenerationObject<CZBufferSphere>(_T("ZBufferSphere"), PRIORTY::PLAYER, false);

	SINSTANCE(CObjectManager)->Intialize();

	m_StageClearNum = SINSTANCE(CObjectManager)->FindGameObject<CNumber>(_T("StageNUMBER"));
	m_StageClearNum->SetPos(D3DXVECTOR3(230.0f, 130.0f, 0.0f));
	m_StageClearNum->SetValue(CStageManager::m_ClearNum);

	CIcon* obj = SINSTANCE(CObjectManager)->FindGameObject<CIcon>(_T("Clear_Icon"));
	obj->SetPos(D3DXVECTOR3(60.0f, 130.0f, 1.0f));
	obj->SetScale(D3DXVECTOR3(60.0f, 65.0f, 0.0f));

	CCourceDef* cource = m_pPlayer->GetCourceDef();
	COURCE_BLOCK block = cource->FindCource(cource->EndCource());
	D3DXVECTOR3 workVec = block.endPosition - block.startPosition;
	D3DXVec3Normalize(&workVec, &workVec);
	D3DXVec3Cross(&workVec, &workVec, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVec3Normalize(&workVec, &workVec);

	CParticleEmitter::EmitterCreate(_T("GoalParticle_Left"), PARTICLE_TYPE::STAR, block.endPosition - (workVec * 2.0f), m_pCamera->GetCamera(),m_Stage_ID,true);
	CParticleEmitter::EmitterCreate(_T("GoalParticle_Right"), PARTICLE_TYPE::STAR, block.endPosition + (workVec * 2.0f), m_pCamera->GetCamera(),m_Stage_ID,true);

	m_CLevelBuilder = SINSTANCE(CObjectManager)->GenerationObject<CLevelBuilder>(_T("LevelBuilder"), PRIORTY::EMITTER, false);
	m_CLevelBuilder->SetIsStage(m_Stage_ID);
	m_CLevelBuilder->Build(pAudio);

	SINSTANCE(CShadowRender)->Entry(m_pPlayer);
	m_pAudio = pAudio;

	m_pAudio->PlayCue(Stage_BGM[m_Stage_ID],false,this);	// 音楽再生
	m_GameState = GAMEEND_ID::CONTINUE;
	m_isGameContinue = true;
}

void CStage::Update()
{
	m_GameState = m_pPlayer->GetGameState();
	SINSTANCE(CObjectManager)->FindGameObject<CNumber>(_T("NUMBER"))->SetValue(CPlayerBullet::EnemyDownNum);
	m_score.Update();
	m_pCamera->SetGameState(m_GameState);
	if (m_isGameContinue){
		if (m_GameState == GAMEEND_ID::CLEAR)
		{
			SINSTANCE(CObjectManager)->GenerationObject<CClearText>(_T("Clear"), PRIORTY::OBJECT2D_ALPHA, false);
			SINSTANCE(CObjectManager)->FindGameObject<CClearText>(_T("Clear"))->Initialize();
			CStageManager::m_ClearNum++;
			m_StageClearNum->SetValue(CStageManager::m_ClearNum);
			m_isGameContinue = false;
			SINSTANCE(CStageManager)->SetIsContinue(true);
		}
		else if (m_GameState == GAMEEND_ID::OVER)
		{
			SINSTANCE(CObjectManager)->GenerationObject<CGameOver>(_T("GameOver"), PRIORTY::OBJECT2D_ALPHA, false);
			SINSTANCE(CObjectManager)->FindGameObject<CGameOver>(_T("GameOver"))->Initialize();
			m_isGameContinue = false;
			SINSTANCE(CStageManager)->SetIsContinue(true);
		}
	}
	else{
		if (m_GameState == GAMEEND_ID::CLEAR){
			if (SINSTANCE(CObjectManager)->FindGameObject<CClearText>(_T("Clear"))->GetIsEnd() && m_pCamera->GetIsEnd()){
				SINSTANCE(CStageManager)->SetContinueStage(static_cast<STAGE_ID>(m_Stage_ID + 1));
				m_pAudio->StopCue(Stage_BGM[m_Stage_ID],false,this);	// 音楽ストップ
				SINSTANCE(CGameManager)->ChangeScene(_T("Result"));
			}
		}
		else if (m_GameState == GAMEEND_ID::OVER){
			if (SINSTANCE(CObjectManager)->FindGameObject<CGameOver>(_T("GameOver"))->GetIsEnd()){
				m_pAudio->StopCue(Stage_BGM[m_Stage_ID],false,this);	// 音楽ストップ
				SINSTANCE(CGameManager)->ChangeScene(_T("Result"));
			}
		}
		
	}
	SINSTANCE(CObjectManager)->Update();
	SINSTANCE(CShadowRender)->Update();
	//m_pAudio->Run();		// 音楽更新
}

void CStage::Draw()
{
	SINSTANCE(CShadowRender)->Draw();
	SINSTANCE(CObjectManager)->Draw();
}


// ステージで使用するオブジェクトの情報を配列に追加したらここのif文を追加すること
void CStage::ActivateObjects(){
	LPCSTR* ObjectDataArray = StageArray[m_Stage_ID];
	int size = StageObjectNumArray[m_Stage_ID];

	for (int idx = 0; idx < size; idx++){
		if (strcmp(ObjectDataArray[idx], "TESTStage3D") == 0){
			SINSTANCE(CObjectManager)->GenerationObject<CField>(_T(ObjectDataArray[idx]), PRIORTY::OBJECT3D, false);
		}
		else if (strcmp(ObjectDataArray[idx], "BulletPhysics") == 0){
			SINSTANCE(CObjectManager)->GenerationObject<CBulletPhysics>(_T(ObjectDataArray[idx]), PRIORTY::CONFIG, false);
		}
		else if (strcmp(ObjectDataArray[idx], "Camera") == 0){
			SINSTANCE(CObjectManager)->GenerationObject<CCourceCamera>(_T(ObjectDataArray[idx]), PRIORTY::CONFIG, false);
		}
		else if (strcmp(ObjectDataArray[idx], "EnemyManager") == 0){
			SINSTANCE(CObjectManager)->GenerationObject<CEnemyManager>(_T(ObjectDataArray[idx]), PRIORTY::ENEMY, false);
		}
		else if (strcmp(ObjectDataArray[idx], "TEST3D") == 0){
			SINSTANCE(CObjectManager)->GenerationObject<CPlayer>(_T(ObjectDataArray[idx]), PRIORTY::PLAYER, false);
		}
		else if (strcmp(ObjectDataArray[idx], "NUMBER") == 0){
			SINSTANCE(CObjectManager)->GenerationObject<CNumber>(_T(ObjectDataArray[idx]), PRIORTY::OBJECT2D_ALPHA, false);
		}
		else if (strcmp(ObjectDataArray[idx], "StageNUMBER") == 0){
			SINSTANCE(CObjectManager)->GenerationObject<CNumber>(_T(ObjectDataArray[idx]), PRIORTY::OBJECT2D_ALPHA, false);
		}
	}
}