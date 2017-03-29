#pragma once
#include "stdafx.h"
#include "Rigidbody.h"
#include "islntersect.h"
#include "RenderContext.h"
#include "C3DObjectRender_S.h"
#include "ParticleEmitter.h"

class CChocoBall : public CGameObject {
public:
	CChocoBall(){}
	~CChocoBall(){
	}

	void Initialize(const D3DXVECTOR3& Spos, const D3DXVECTOR3& Epos);
	void Update()override;
	void Draw()override;

	void SetUpTechnique()override{
#ifdef NOT_EM
		m_pRender->SetUpTechnique("Boneless_Tex_Lim");
#else
		m_pRender->SetUpTechnique("Boneless_Tex_Fresnel");
#endif
	}
	void EM_SetUpTechnique()override{
		m_pEMSamplingRender->SetUpTechnique("Boneless_Tex_Lim");
	}

	void SetRenderState()override{
		m_RenderingState = RENDER::TYPE::_3D_Simple;
	}

	void BeginDraw();
	void EndDraw();
	void OnDestroy();

	//�����_�ƖړI�n��n���ƃx�N�g�����Z�b�g���܂��B
	//GetVector()�ŌĂяo���Ă��������B
	inline void SetVector(const D3DXVECTOR3& Spos, const D3DXVECTOR3& Epos){
		m_Vector = Epos - Spos;
	}

	//�i�s�����̃x�N�g����Ԃ�
	inline const D3DXVECTOR3& GetVector()
	{
		return m_Vector;
	}
	inline void SetStageID(STAGE_ID id) {
		m_StageID = id;
	}
	inline void SetIsBossDamage(bool flg) {
		m_IsBossDamage = flg;
	}
	inline bool GetIsBossDamage()const {
		return m_IsBossDamage;
	}
	inline void SetDeathTime(float t) {
		m_DeathTime = t;
		m_TimeCounter = 0.0f;
	}
	inline void SetIsBurst(bool flg) {
		m_IsBurst = flg;
	}
private:
	//��������bulletPhysics�̍��̂��g�p���邽�߂ɕK�v�ȕϐ��B
	D3DXVECTOR3				m_EndPosition;		//�ړI�n
	D3DXVECTOR3				m_Vector;			//�i�s�������i�[�B
	D3DXVECTOR3				m_Vector2;
	float					m_life;
	float					m_radius;
	D3DXVECTOR3				m_moveSpeed;		//�ړ����x�B

	bool m_IsBossDamage = true;	// �{�X�Ƀ_���[�W��^�����邩�B

	// �`���R�{�[�������ԍ��Ŕj��B
	float m_DeathTime;
	float m_TimeCounter;
	bool m_IsBurst = false;
	STAGE_ID m_StageID;
	CParticleEmitter* m_pEmitter = nullptr;
};