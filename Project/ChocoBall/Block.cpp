#include "stdafx.h"
#include "Block.h"
#include "ShadowRender.h"
#include "BulletPhysics.h"
#include "RenderContext.h"
#include "Infomation.h"

void CBlock::OnDestroy(){
	m_CollisionObject->RemoveWorld();
	//�q���Ɏ��S�������Ƃ�ʒm�B
	if (m_child){
		m_child->OnDestroyParent();
	}
	m_isDead = true;
}

//�e�����񂾂Ƃ��ɌĂ΂�鏈���B
void CBlock::OnDestroyParent()
{
	m_eState = enState_Fall;
	m_fallPosY = m_parent->GetPos().y;
	SetParent(m_parent->m_parent);
	//�����̎q���𗎉�������B
	CBlock* child = m_child;
	while (child != NULL){
		child->m_fallPosY = child->m_parent->GetPos().y;
		child->m_eState = enState_Fall;
		child = child->m_child;
	}
}


void CBlock::Initialize(D3DXVECTOR3 pos, D3DXQUATERNION rot)
{
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/BR.x");
	CGameObject::Initialize();
#ifdef NOT_INSTANCING
	m_pModel->m_alpha = 1.0f;
	m_pModel->m_luminance = 0.0f;
	m_pModel->m_Refractive = FRESNEL::g_RefractivesTable[FRESNEL::REFRACTIVES::CHOCOLATE];
#endif
	m_transform.position = pos;
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_transform.angle = rot;

	//���̈����ɓn���̂̓{�b�N�Xhalfsize�Ȃ̂ŁA0.5�{����B
	ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), new btBoxShape(btVector3(1.0f*0.5f, 1.0f*0.5f, 1.0f*0.5f)), CollisionType::Wall, false, 0.0f, true,true);
	m_CollisionObject->BitMask_AllOn();
	m_CollisionObject->BitMask_Off(CollisionType::Player);
	SetAlive(true);

#ifdef NOT_INSTANCING
	SINSTANCE(CShadowRender)->Entry(this);
#endif
}


void CBlock::Update()
{
	switch (m_eState){
	case enState_Normal:
		break;
	case enState_Broken:
		break;
	case enState_Fall:
		m_transform.position.y -= 0.1f;
		//m_RigidBody->activate();
		if (m_transform.position.y <= m_fallPosY){
			m_transform.position.y = m_fallPosY;
			m_eState = enState_Normal;
		}
		break;
	}

	CGameObject::Update();
}

void CBlock::Draw()
{
	m_pRender->SetModelData(m_pModel);
	m_pRender->Draw();
}

void CBlock::BeginDraw()
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
void CBlock::EndDraw()
{
	m_pRender->GetEffect()->EndPass();
	m_pRender->GetEffect()->End();
}
