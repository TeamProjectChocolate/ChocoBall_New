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
protected:
	CAudio* m_pAudio;
	STAGE_ID m_Stage_ID = STAGE_ID::STAGE_NONE;
private:
	CPlayer* m_pPlayer;
	CParticleEmitter* m_pEmitter;
	CLevelBuilder* m_CLevelBuilder;
	bool m_isGameContinue = true;
	GAMEEND_ID m_GameState = GAMEEND_ID::CONTINUE;

	CScore m_score;	
	CCourceCamera* m_pCamera;
	CEnemyManager* m_pEnemyManager;
	Skybox m_skybox;
	CZBufferSphere m_zbuffersphere;
	CNumber* m_StageClearNum;
};

