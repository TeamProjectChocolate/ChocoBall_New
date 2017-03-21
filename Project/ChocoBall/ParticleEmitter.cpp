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
	m_ParticleList.clear();
}

void CParticleEmitter::Initialize(){
	m_count = 0;
	m_timer = 0.0f;
	m_random.Init(0.1);
	SetAlive(true);
	D3DXVec3Normalize(&m_emitDirection,&(m_param->initVelocity));
	strcpy(m_ParticleName, m_param->texturePath);
	m_CourceDef.SetStageID(m_Stage_ID);
	m_CourceDef.Initialize();
	m_pPlayer = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));
	m_TailPosition = nullptr;
	m_Residual = false;
	m_CourceLange = 0;
	m_ParticleList.clear();
}

void CParticleEmitter::Update(){
	// エミッター削除関連。
	DeathCount();

	// パーティクル生成。
	EmitParticle();

	list<CParticle*>::iterator itr = m_ParticleList.begin();
	while (itr != m_ParticleList.end()){
		if ((*itr)){
			itr = m_ParticleList.erase(itr);
			m_count--;
		}
		else{
			itr++;
		}
	}
	if (m_pTailParticle != nullptr) {
		m_Residual = false;
	}

	//// エミッター削除関連。
	//DeathCount();
}

// パーティクル生成。
void CParticleEmitter::EmitParticle() {

	if (m_IsUseCource) {
		m_CurrentCourceNo = m_CourceDef.FindCource(m_emitPosition).blockNo;
		m_NowPlayerCourceNo = m_CourceDef.FindCource(m_pPlayer->GetPos()).blockNo;
		if (m_CurrentCourceNo == -1 || abs(m_CurrentCourceNo - m_NowPlayerCourceNo) > m_CourceLange || m_NowPlayerCourceNo == -1) {
			return;
		}
	}
	if (m_EmitFlg) {
		if (m_timer >= m_param->intervalTime) {
			for (int idx = 0; idx < m_param->EmitNum; idx++) {
				CParticle* p = SINSTANCE(CObjectManager)->GenerationObject<CParticle>(static_cast<LPCSTR>(m_ParticleName), OBJECT::PRIORTY::PARTICLE_ALPHA, false);
				// パーティクルを発生させる方向を上書きする。
				p->InitParticle(m_random, *m_camera, m_param, m_emitPosition, m_emitDirection);
				m_timer = 0.0f;
				m_ParticleList.push_back(p);
				m_pTailParticle = p;
				m_pTailParticle->SetTailParticle(&m_pTailParticle);
				m_TailPosition = p->GetPosRef();
				m_Residual = true;
				m_count++;
			}
		}
		m_timer += 1.0f / 60.0f;
	}
	else {
		m_timer = 0.0f;
	}

}

// エミッター削除関連。
void CParticleEmitter::DeathCount() {
	if (m_IsUseDeathTime) {
		if (m_DeathCounter >= m_DeathTime) {
			// 指定された時間が過ぎたので自分を削除。
			SINSTANCE(CObjectManager)->DeleteGameObject(this);
		}
		m_DeathCounter += 1.0f / 60.0f;
	}
}

void CParticleEmitter::Draw(){

}

void CParticleEmitter::Draw_EM(CCamera* camera){

}

void CParticleEmitter::ApplyForce(const D3DXVECTOR3& applyForce){
	for (auto itr : m_ParticleList){
		itr->ApplyFource(applyForce);
	}
}
