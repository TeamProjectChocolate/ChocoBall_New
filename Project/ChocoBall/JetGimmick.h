#pragma once
#include "GameObject.h"
#include "ParticleEmitter.h"
#include "Audio.h"
#include "Camera.h"

class CJetGimmick :
	public CGameObject
{
public:
	CJetGimmick();
	~CJetGimmick();
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Draw()override;
	void Draw_EM(CCamera*)override;
	void SoundPlay();
	void SetAudio(CAudio* audio){
		m_pAudio = audio;
	}
	void SetCamera(CCamera* pCamera){
		m_pCamera = pCamera;
	}
	void SetDirection(D3DXVECTOR3 dir){
		m_pEmitter->SetDirection(dir);
	}
	void SetPos(const D3DXVECTOR3& pos)override{
		m_pEmitter->SetEmitPos(pos);
	}
	const D3DXVECTOR3& GetPos()override{
		return m_pEmitter->GetEmitPos();
	}
	void SetEmitterName(LPCSTR name){
		strcpy(m_pFileName, name);
	}
	void SetStageID(STAGE_ID id){
		m_Stage_ID = id;
	}
	void SetPlayerCource(int no){
		m_PlayerCourceNo = no;
	}
private:
	CAudio* m_pAudio;
	CCamera* m_pCamera;
	float m_IntervalCounter;
protected:
	CParticleEmitter* m_pEmitter = nullptr;
	float m_JetTime;
	PARTICLE_TYPE m_ParticleType;
	char m_SoundName[FILENAME_MAX];
	char m_SoundEndName[FILENAME_MAX];
	bool m_IsPlay;
	float m_JetCounter;
	float m_IntervalTime;
	bool m_JetFlg;
	STAGE_ID m_Stage_ID;
	int m_PlayerCourceNo;
	int m_EmitCourceNo;
	CPlayer* m_pPlayer;
};

