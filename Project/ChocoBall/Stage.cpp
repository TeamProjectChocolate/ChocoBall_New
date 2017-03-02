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
#include "MapObjectManager.h"

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

	// �X�e�[�W�𐶐����Ă����B
	// ���ׂẴX�e�[�W�ɕK�v�ȃQ�[���I�u�W�F�N�g���A�N�e�B�u���B
	ActivateObjects();

	// �ǂ̃X�e�[�W���ʒm�B
	m_pCamera = SINSTANCE(CObjectManager)->FindGameObject<CCourceCamera>(_T("Camera"));
	m_pCamera->SetStageID(m_Stage_ID);
	m_pEnemyManager = SINSTANCE(CObjectManager)->FindGameObject<CEnemyManager>(_T("EnemyManager"));
	m_pEnemyManager->SetStageID(m_Stage_ID);
	m_pPlayer = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("TEST3D"));
	m_pPlayer->SetStageID(m_Stage_ID);
	SINSTANCE(CObjectManager)->FindGameObject<CMapObjectManager>(_T("TESTStage3D"))->SetStageID(m_Stage_ID);
	SINSTANCE(CRenderContext)->GetEMRender()->SetPos(EM_CameraPosArray[m_Stage_ID]);

	m_pPlayer->SetAudio(pAudio);

	// ObjectManager�ɃQ�[���I�u�W�F�N�g�Q��������������B
	SINSTANCE(CObjectManager)->Intialize();

	// GUI�̃|�W�V������ݒ�B
	//m_StageClearNum = SINSTANCE(CObjectManager)->FindGameObject<CNumber>(_T("StageNUMBER"));
	//m_StageClearNum->SetPos(D3DXVECTOR3(230.0f, 350.0f, 1.0f));
	//m_StageClearNum->SetValue(CStageManager::m_ClearNum);
	//CIcon* obj = SINSTANCE(CObjectManager)->FindGameObject<CIcon>(_T("Clear_Icon"));
	//obj->SetPos(D3DXVECTOR3(60.0f, 430.0f, 1.0f));
	//obj->SetScale(D3DXVECTOR3(60.0f, 65.0f, 0.0f));

	// �v���C���[�Ŏg�p����R�[�X��`�B
	CCourceDef* cource = m_pPlayer->GetCourceDef();
	COURCE_BLOCK block = cource->FindCource(cource->EndCource());
	D3DXVECTOR3 workVec = block.endPosition - block.startPosition;
	D3DXVec3Normalize(&workVec, &workVec);
	D3DXVec3Cross(&workVec, &workVec, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVec3Normalize(&workVec, &workVec);

	// �S�[���p�[�e�B�N���B
	CParticleEmitter::EmitterCreate(_T("GoalParticle_Left"), PARTICLE_TYPE::STAR, block.endPosition - (workVec * 2.0f), m_pCamera->GetCamera(),m_Stage_ID,true,true);
	CParticleEmitter::EmitterCreate(_T("GoalParticle_Right"), PARTICLE_TYPE::STAR, block.endPosition + (workVec * 2.0f), m_pCamera->GetCamera(),m_Stage_ID,true,true);

	// ���x�������B
	m_CLevelBuilder = SINSTANCE(CObjectManager)->GenerationObject<CLevelBuilder>(_T("LevelBuilder"), PRIORTY::EMITTER, false);
	m_CLevelBuilder->SetIsStage(m_Stage_ID);
	m_CLevelBuilder->Build(pAudio);

	// �v���C���[���V���h�E�L���X�^�[�ɐݒ�B
	SINSTANCE(CShadowRender)->Entry(m_pPlayer);

	// ���ʃ��C�g�ݒ�B
	this->ConfigLight();

	m_pAudio = pAudio;
	m_pAudio->PlayCue(Stage_BGM[m_Stage_ID],false,this);	// ���y�Đ�
	m_GameState = GAMEEND_ID::CONTINUE;
	m_isGameContinue = true;
}

void CStage::Update()
{
	m_GameState = m_pPlayer->GetGameState();
	//SINSTANCE(CObjectManager)->FindGameObject<CNumber>(_T("NUMBER"))->SetValue(CPlayerBullet::EnemyDownNum);
	//m_score.Update();
	m_pCamera->SetGameState(m_GameState);
	if (m_isGameContinue){
		if (m_GameState == GAMEEND_ID::CLEAR)
		{
			SINSTANCE(CObjectManager)->GenerationObject<CClearText>(_T("Clear"), PRIORTY::OBJECT2D_ALPHA, false);
			SINSTANCE(CObjectManager)->FindGameObject<CClearText>(_T("Clear"))->Initialize();
			CStageManager::m_ClearNum++;
			//m_StageClearNum->SetValue(CStageManager::m_ClearNum);
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
				m_pAudio->StopCue(Stage_BGM[m_Stage_ID],false,this);	// ���y�X�g�b�v
				SINSTANCE(CGameManager)->ChangeScene(_T("Result"));
			}
		}
		else if (m_GameState == GAMEEND_ID::OVER){
			if (SINSTANCE(CObjectManager)->FindGameObject<CGameOver>(_T("GameOver"))->GetIsEnd()){
				m_pAudio->StopCue(Stage_BGM[m_Stage_ID],false,this);	// ���y�X�g�b�v
				SINSTANCE(CGameManager)->ChangeScene(_T("Result"));
			}
		}
		
	}
	SINSTANCE(CObjectManager)->Update();
	SINSTANCE(CShadowRender)->Update();
	//m_pAudio->Run();		// ���y�X�V
}

void CStage::Draw()
{
	SINSTANCE(CShadowRender)->Draw();
	SINSTANCE(CObjectManager)->Draw();
}


void CStage::ActivateObjects() {

	// ���ׂẴX�e�[�W���ʂ̃I�u�W�F�N�g�B
	SINSTANCE(CObjectManager)->GenerationObject<Skybox>(_T("skybox"), PRIORTY::OBJECT3D, false);
	SINSTANCE(CObjectManager)->GenerationObject<CZBufferSphere>(_T("ZBufferSphere"), PRIORTY::PLAYER, false);
	SINSTANCE(CObjectManager)->GenerationObject<CCourceCamera>(_T("Camera"), PRIORTY::CONFIG, false);
	SINSTANCE(CObjectManager)->GenerationObject<CMapObjectManager>(_T("TESTStage3D"), PRIORTY::CONFIG, false);
	SINSTANCE(CObjectManager)->GenerationObject<CEnemyManager>(_T("EnemyManager"), PRIORTY::ENEMY, false);
	SINSTANCE(CObjectManager)->GenerationObject<CPlayer>(_T("TEST3D"), PRIORTY::PLAYER, false);
	//SINSTANCE(CObjectManager)->GenerationObject<CNumber>(_T("NUMBER"), PRIORTY::OBJECT2D_ALPHA, false);
	//SINSTANCE(CObjectManager)->GenerationObject<CNumber>(_T("StageNUMBER"), PRIORTY::OBJECT2D_ALPHA, false);
}

void CStage::ConfigLight() {
	D3DXVECTOR3* LightDirection;
	LightDirection = LightDirectionTableArray[m_Stage_ID];
	// �f�B�t���[�Y���C�g�̌����ݒ�(���C�g1�`4)
	for (short idx = 0; idx < NUM_DIFFUSE_LIGHT; idx++) {
		m_Light.SetDiffuseLightDirection(idx, LightDirection[idx]);
	}

	D3DXVECTOR4* LightColor;
	LightColor = LightColorTableArray[m_Stage_ID];
	// �f�B�t���[�Y���C�g�̐F�ݒ�(���C�g1�`4)
	for (short idx = 0; idx < NUM_DIFFUSE_LIGHT; idx++) {
		m_Light.SetDiffuseLightColor(idx, LightColor[idx]);
	}

	// �A���r�G���g���C�g(����)�̋����ݒ�
	m_Light.SetAmbientLight(D3DXVECTOR3(0.1f, 0.1f, 0.1f));

	// �������C�g�̐F�ƌ����ݒ�B
	m_Light.SetLimLightColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_Light.SetLimLightDirection(D3DXVECTOR3(0.0f, 0.0f, -1.0f));

	// ���ʃ��C�g�ɐݒ�B
	SINSTANCE(CRenderContext)->SetCurrentLight(&m_Light);
}