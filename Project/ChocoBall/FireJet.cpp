#include "stdafx.h"
#include "FireJet.h"
#include "CourceDef.h"

static int hoge = 0;

CFireJet::CFireJet()
{
	this->hogehoge = hoge;
	hoge++;
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
	if(m_pEmitter == nullptr){
		return false;
	}
	SParticleEmitParameter param = m_pEmitter->GetEmitParameter();
	float timer;
	if (m_pEmitter->GetEmitFlg() && m_JetCounter <= param.life && m_pEmitter->GetTailParticle()/*�G�~�b�g�t���O�݂̂��Ƃ�����̍X�V�̕�����������null�A�N�Z�X����������*/){
		timer = m_JetCounter;
	}
	else if (m_pEmitter->GetTailParticle()){
		timer = param.life;
	}
	else{
		// �p�[�e�B�N������������Ă��炸�A�c�������Ă��Ȃ����ߓ����蔻����s��Ȃ�
		return false;
	}

	D3DXVECTOR3 work = D3DXVec3Length(&(param.initVelocity)) * m_pEmitter->GetDirection();
	D3DXVECTOR3 Vec = (m_pEmitter->GetEmitPos() + work * timer) - m_pEmitter->GetTailPos();
	D3DXVECTOR3 Vec2 = pos - m_pEmitter->GetEmitPos();
	D3DXVECTOR3 RealVec = pos - m_pEmitter->GetTailPos();

	// �v���C���[���Ō�ɐ��������p�[�e�B�N����艜�ɂ��邩���ׂ�B
	D3DXVECTOR3 Dir1, Dir2;
	D3DXVec3Normalize(&Dir1, &Vec);
	D3DXVec3Normalize(&Dir2, &RealVec);
	float rad =  fabsf(acosf(D3DXVec3Dot(&Dir1, &Dir2)));
	if (rad > D3DXToRadian(90.0f)) {
		// �v���C���[����O�B
		return false;
	}
	else {
		// �v���C���[�����B

		// �v���C���[�����̎˒��ɂ��邩���ׂ�B
		float Length1 = D3DXVec3Length(&Vec);
		float Length2 = D3DXVec3Length(&RealVec);
		if (Length2 >= Length1) {
			// �v���C���[���˒��O�B
			return false;
		}
		else {
			// �v���C���[���˒����B

			// �����_�����̍ő�l�����Z�����x�N�g���B
			D3DXVECTOR3 work2;
			work2.x = work.x + (param.initVelocityVelocityRandomMargin.x * 0.6f);
			work2.y = work.y + (param.initVelocityVelocityRandomMargin.y * 0.6f);
			work2.z = work.z + (param.initVelocityVelocityRandomMargin.z * 0.6f);

			D3DXVECTOR3 MaxDir, PlayerDir;
			D3DXVec3Normalize(&MaxDir, &work2);
			D3DXVec3Normalize(&PlayerDir, &Vec2);
			// ���̖{���̎˒����Z�o
			float RangeRad = fabsf(acosf(D3DXVec3Dot(&MaxDir, &(m_pEmitter->GetDirection()))));
			float PlayerRad = fabsf(acosf(D3DXVec3Dot(&(m_pEmitter->GetDirection()), &PlayerDir)));
			if (PlayerRad >= RangeRad) {
				// �������ĂȂ��B
				return false;
			}
			else {
				// �������Ă�B
				return true;
			}
		}
	}

	//float length = D3DXVec3Length(&Vec);
	////length -= D3DXVec3Length(&((m_pEmitter->GetEmitPos() + m_pEmitter->GetTailPos()) - m_pEmitter->GetEmitPos()));
	////D3DXVec3Normalize(&Vec, &Vec);
	//float length2 = D3DXVec3Length(&RealVec);
	//length2 -= radius / 2;
	//if (length >= length2){
	//	D3DXVec3Normalize(&Vec, &Vec);
	//	D3DXVec3Normalize(&RealVec, &RealVec);
	//	float rad = fabsf(acosf(D3DXVec3Dot(&Vec, &RealVec)));
	//	if (rad <= D3DXToRadian(90.0f)){
	//		D3DXVECTOR3 work2;
	//		work2.x = work.x + (param.initVelocityVelocityRandomMargin.x * 0.6f);
	//		work2.y = work.y + (param.initVelocityVelocityRandomMargin.y * 0.6f);
	//		work2.z = work.z + (param.initVelocityVelocityRandomMargin.z * 0.6f);
	//		D3DXVECTOR3 Vec3 = (m_pEmitter->GetEmitPos() + work2) - m_pEmitter->GetEmitPos();
	//		D3DXVec3Normalize(&Vec3, &Vec3);
	//		D3DXVec3Normalize(&Vec2, &Vec2);
	//		float dot = fabsf(acosf(D3DXVec3Dot(&(m_pEmitter->GetDirection()), &Vec3)));
	//		float dot2 = fabsf(acosf(D3DXVec3Dot(&(m_pEmitter->GetDirection()), &Vec2)));
	//		if (dot >= dot2){
	//			return true;
	//		}
	//	}
	//}
	return false;
}