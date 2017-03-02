#pragma once

#include "GameObject.h"
#include "CourceCamera.h"
#include "Primitive.h"
#include "Random.h"

class CRandom;
class CCourceCamera;
struct SParticleEmitParameter;

class CParticle:public CGameObject
{
public:
	CParticle();
	~CParticle();
	void SetUpTechnique()override{
		if (m_alphaBlendMode == 0){
			m_pRender->SetUpTechnique("BasicTec");
		}
		else{
			m_pRender->SetUpTechnique("ColorTexPrimAdd");
		}
	}
	void EM_SetUpTechnique()override{
		if (m_alphaBlendMode == 0){
			m_pEMSamplingRender->SetUpTechnique("BasicTec");
		}
		else{
			m_pEMSamplingRender->SetUpTechnique("ColorTexPrimAdd");
		}
	}


	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Draw_EM(CCamera*)override;
	void SetupMatrices();
	void InitParticle(CRandom&, CCamera&, const SParticleEmitParameter*, const D3DXVECTOR3&,D3DXVECTOR3);
	// �p�[�e�B�N���ɗ͂�������֐�
	void ApplyFource(const D3DXVECTOR3& applyFource){
		this->m_applyFource = applyFource;
	}
	bool GetIsDead(){
		return m_state == EMIT_STATE::DEAD;
	}
	void SetFileName(LPCSTR name){
		CH_ASSERT(strlen(name) <= MAX_FILENAME);
		strcpy(m_pFileName, name);
	}
	D3DXVECTOR3* GetPosRef(){
		return &(m_ParticleData.position);
	}
private:
	CPrimitive m_Primitive;
	D3DXMATRIX mWorldViewProj;
	CCamera* m_camera;
	CRandom* m_random;
	float m_life;
	float m_timer;
	PARTICLE_DATA m_ParticleData;
	float m_rotateZ;
	D3DXVECTOR3 m_addVelocityRandomMargin;	// ���x�̐ϕ��̂Ƃ��̃����_����
	bool m_isFade;		// ���ʂƂ��Ƀt�F�[�h�A�E�g���邩�̃t���O
	float m_fadeTime;		// �t�F�[�h�̎���
	EMIT_STATE m_state;	// ���
	float m_initAlpha;	// �����A���t�@
	bool m_isBillboard;	// �r���{�[�h���ǂ����̃t���O
	D3DXVECTOR3 m_applyFource;	// �O�����������
	float m_brightness;		// �P�x�B�u���[�����L���Ȃ狭������ƌ������ӂ��
	int m_alphaBlendMode;		// �����������A1���Z����
	float m_deltaTime;
};
