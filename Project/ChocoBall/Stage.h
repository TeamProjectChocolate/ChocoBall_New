#pragma once
#include "stdafx.h"
#include "Audio.h"
#include "Player.h"
#include "SceneResult.h"
#include "Score.h"
#include "CourceCamera.h"
#include "EnemyManager.h"
#include "Skybox.h"
#include "ZBufferSphere.h"
#include "GameAudio.h"



class CStage
{
public:
	CStage();
	~CStage();
 	virtual void Initialize(CAudio*,STAGE_ID);
	virtual void Update();
	virtual void Draw();
	void ActivateObjects();
	CAudio* GetAudio(){
		if (m_pAudio == nullptr){
			return nullptr;
		}
		else{
			return m_pAudio;
		}
	}
private:
	void ConfigLight();
protected:
	CAudio* m_pAudio;
	STAGE_ID m_Stage_ID = STAGE_ID::STAGE_NONE;
private:
	CLight m_Light;
	CPlayer* m_pPlayer = nullptr;
	CParticleEmitter* m_pEmitter = nullptr;
	CLevelBuilder* m_CLevelBuilder = nullptr;
	bool m_isGameContinue = true;
	GAMEEND::ID m_GameState = GAMEEND::ID::CONTINUE;

	CScore m_score;	
	CCourceCamera* m_pCamera = nullptr;
	CEnemyManager* m_pEnemyManager = nullptr;
	Skybox m_skybox;
	CZBufferSphere m_zbuffersphere;
	CNumber* m_StageClearNum = nullptr;
	CGameAudio* m_BossAudio = nullptr;
};

