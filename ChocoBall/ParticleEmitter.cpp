#include "stdafx.h"
#include "ParticleEmitter.h"
#include "ObjectManager.h"
#include "RenderContext.h"


CParticleEmitter::CParticleEmitter()
{
	m_camera = nullptr;
	m_param = nullptr;
	m_timer = 0.0f;
	m_count = 0;
}


CParticleEmitter::~CParticleEmitter()
{
}

void CParticleEmitter::Initialize(){
	m_count = 0;
	m_timer = 0.0f;
	m_random.Init(0.1);
	SetAlive(true);
	D3DXVec3Normalize(&m_dir,&(m_param->initVelocity));
	strcpy(m_ParticleName, m_param->texturePath);
	m_CourceDef.SetStageID(m_Stage_ID);
	m_CourceDef.Initialize();
	m_pPlayer = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("TEST3D"));
	m_TailPosition = nullptr;
	m_Residual = false;
	m_CourceLange = 0;
}

void CParticleEmitter::Update(){
	m_CurrentCourceNo = m_CourceDef.FindCource(m_emitPosition).blockNo;
	m_NowPlayerCourceNo = m_CourceDef.FindCource(m_pPlayer->GetPos()).blockNo;
	if (abs(m_CurrentCourceNo - m_NowPlayerCourceNo) <= m_CourceLange && m_CurrentCourceNo != -1 && m_NowPlayerCourceNo != -1){
		if (m_EmitFlg){
			if (m_timer >= m_param->intervalTime){
				for (int idx = 0; idx < m_param->EmitNum; idx++){
					CParticle* p = SINSTANCE(CObjectManager)->GenerationObject<CParticle>(static_cast<LPCSTR>(m_ParticleName), PRIORTY::PARTICLE_ALPHA, false);
					p->InitParticle(m_random, *m_camera, m_param, m_emitPosition, m_dir);
					m_timer = 0.0f;
					m_ParticleList.push_back(p);
					m_pTailParticle = p;
					m_TailPosition = p->GetPosRef();
					m_Residual = true;
					m_count++;
				}
			}
			m_timer += 1.0f / 60.0f;
		}
		else{
			m_timer = 0.0f;
		}
	}

	list<CParticle*>::iterator itr = m_ParticleList.begin();
	while (itr != m_ParticleList.end()){
		if ((*itr)->GetIsDead()){
			itr = m_ParticleList.erase(itr);
			m_count--;
		}
		else{
			itr++;
		}
	}
	if (m_pTailParticle == nullptr){
		return;
	}
	else if (m_pTailParticle->GetIsDead()){
		m_pTailParticle = nullptr;
		m_Residual = false;
	}
}

void CParticleEmitter::Draw(){

}

void CParticleEmitter::ApplyForce(const D3DXVECTOR3& applyForce){
	for (auto itr : m_ParticleList){
		itr->ApplyFource(applyForce);
	}
}
