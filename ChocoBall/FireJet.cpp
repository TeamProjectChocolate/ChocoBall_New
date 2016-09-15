#include "stdafx.h"
#include "FireJet.h"
#include "CourceDef.h"

CFireJet::CFireJet()
{
}


CFireJet::~CFireJet()
{
}

void CFireJet::Initialize(){
	m_IntervalTime = 3.0f;
	m_JetTime = 3.0f;
	m_ParticleType = PARTICLE_TYPE::FIREGIMMICK;
	strcpy(m_SoundName, "Flame");
	strcpy(m_SoundEndName, "Flameend");

	CJetGimmick::Initialize();
}

void CFireJet::Update(){
	CJetGimmick::Update();
}

bool CFireJet::IsCollision(D3DXVECTOR3 pos, float radius){
	SParticleEmitParameter* param = m_pEmitter->GetEmitParameter();
	float timer;
	if (m_pEmitter->GetEmitFlg() && m_JetCounter <= param->life && m_pEmitter->GetResidual()/*エミットフラグのみだとこちらの更新の方が速いためnullアクセスが発生する*/){
		timer = m_JetCounter;
	}
	else if (m_pEmitter->GetResidual()){
		timer = param->life;
	}
	else{
		// パーティクルが生成されておらず、残留もしていないため当たり判定を行わない
		return false;
	}

	D3DXVECTOR3 work = D3DXVec3Length(&(param->initVelocity)) * m_pEmitter->GetDirection();
	D3DXVECTOR3 Vec = (m_pEmitter->GetEmitPos() + work * timer) - m_pEmitter->GetTailPos();
	D3DXVECTOR3 Vec2 = pos - m_pEmitter->GetEmitPos();
	D3DXVECTOR3 RealVec = pos - m_pEmitter->GetTailPos();
	float length = D3DXVec3Length(&Vec);
	//length -= D3DXVec3Length(&((m_pEmitter->GetEmitPos() + m_pEmitter->GetTailPos()) - m_pEmitter->GetEmitPos()));
	//D3DXVec3Normalize(&Vec, &Vec);
	float length2 = D3DXVec3Length(&RealVec);
	length2 -= radius / 2;
	if (length >= length2){
		D3DXVec3Normalize(&Vec, &Vec);
		D3DXVec3Normalize(&RealVec, &RealVec);
		float rad = fabsf(acosf(D3DXVec3Dot(&Vec, &RealVec)));
		if (rad <= D3DXToRadian(90.0f)){
			D3DXVECTOR3 work2;
			work2.x = work.x + (param->initVelocityVelocityRandomMargin.x * 0.6f);
			work2.y = work.y + (param->initVelocityVelocityRandomMargin.y * 0.6f);
			work2.z = work.z + (param->initVelocityVelocityRandomMargin.z * 0.6f);
			D3DXVECTOR3 Vec3 = (m_pEmitter->GetEmitPos() + work2) - m_pEmitter->GetEmitPos();
			D3DXVec3Normalize(&Vec3, &Vec3);
			D3DXVec3Normalize(&Vec2, &Vec2);
			float dot = fabsf(acosf(D3DXVec3Dot(&(m_pEmitter->GetDirection()), &Vec3)));
			float dot2 = fabsf(acosf(D3DXVec3Dot(&(m_pEmitter->GetDirection()), &Vec2)));
			if (dot >= dot2){
				return true;
			}
		}
	}
	return false;
}