#include "stdafx.h"
#include "ChocoBall.h"
#include "CollisionType.h"
#include "RenderContext.h"
#include "ShadowRender.h"

void CChocoBall::Initialize(const D3DXVECTOR3& Spos, const D3DXVECTOR3& Epos)
{
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/ball.x");
	CGameObject::Initialize();

#ifdef NOT_INSTANCING
 	m_pModel->m_alpha = 1.0f;
	m_pModel->m_luminance = 0.0f;
	m_pModel->m_Refractive = g_RefractivesTable[REFRACTIVES::CHOCOLATE];
#endif
	m_transform.position = Spos; 
	SetVector(m_transform.position, Epos);
	SetRotation(D3DXVECTOR3(0, 0, 0), 0.1f);
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Rigidbody.Initialize(&m_transform.position, 0.3f);
	m_Rigidbody.SetUserIndex(CollisionType_Chocoball);

	m_moveSpeed.x = 0.05f;
	m_moveSpeed.z = 0.05f;
	m_moveSpeed.y = 0.05f;


	SetAlive(true);

	D3DXVec3Normalize(&m_Vector2, &GetVector());
}


void CChocoBall::Update()
{

	m_Rigidbody.ApplyForce(m_Vector2 * 5.0f);
	m_Rigidbody.Update(&m_transform.position);
	if (m_IsBurst) {
		if (m_TimeCounter >= m_DeathTime) {
			// �`���R�{�[����j�􂳂���B
			m_pEmitter = CParticleEmitter::EmitterCreate(_T("ChocoParticle"), PARTICLE_TYPE::CHOCOBALL_BURST, m_transform.position, SINSTANCE(CRenderContext)->GetCurrentCamera(), m_StageID, false, true);
			m_pEmitter->SetEmitPos(m_transform.position);
			m_pEmitter->SetEmitFlg(true);
			m_pEmitter->SetIsUseCource(false);
			m_pEmitter->SentenceOfDeath(50.0f);
			SetAlive(false);
		}
		m_TimeCounter += 1.0f / 60.0f;
	}
	CGameObject::Update();
}

void CChocoBall::BeginDraw()
{
	SetUpTechnique();
	UINT numPass;
	m_pRender->GetEffect()->Begin(&numPass/*�e�N�j�b�N���ɒ�`����Ă���p�X�̐����ԋp�����*/, 0);
	m_pRender->GetEffect()->BeginPass(0);	//�p�X�̔ԍ����w�肵�Ăǂ̃p�X���g�p���邩�w��

	// ���݂̃v���W�F�N�V�����s��ƃr���[�s����V�F�[�_�[�ɓ]��
	SINSTANCE(CRenderContext)->GetCurrentCamera()->SetCamera(m_pRender->GetEffect());
	SINSTANCE(CRenderContext)->GetCurrentLight()->SetLight(m_pRender->GetEffect());
	// ���_���V�F�[�_�[�ɓ]��
	m_pRender->GetEffect()->SetVector("EyePosition", reinterpret_cast<D3DXVECTOR4*>(&SINSTANCE(CRenderContext)->GetCurrentCamera()->GetPos()));


	SINSTANCE(CShadowRender)->SetShadowCamera(m_pRender->GetEffect());
}
void CChocoBall::EndDraw()
{
	m_pRender->GetEffect()->EndPass();
	m_pRender->GetEffect()->End();
}

void CChocoBall::Draw()
{
	m_pRender->SetModelData(m_pModel);
	m_pRender->Draw();
}

void CChocoBall::OnDestroy()
{
	m_Rigidbody.OnDestroy();
	if (m_pEmitter) {
		if (!(m_pEmitter->GetIsUseDeathTime())) {
			SINSTANCE(CObjectManager)->DeleteGameObjectImmediate(m_pEmitter);
		}
	}
	m_pEmitter = nullptr;
}
