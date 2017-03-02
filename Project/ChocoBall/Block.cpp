#include "stdafx.h"
#include "Block.h"
#include "ShadowRender.h"
#include "BulletPhysics.h"
#include "RenderContext.h"

void CBlock::OnDestroy(){
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->RemoveRigidBody_Dynamic(m_rigidBody);
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
	m_pModel->m_Refractive = g_RefractivesTable[REFRACTIVES::CHOCOLATE];
#endif
	m_transform.position = pos; //D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	SetRotation(D3DXVECTOR3(0, 0, 0), 0.1f);
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_transform.angle = rot;
	//m_RigitBody.Initialize(&m_transform.position, &m_transform.scale);

	this->Build(D3DXVECTOR3(1.0f, 1.0f, 1.0f), m_transform.position);

	m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
	m_rigidBody->setUserIndex(CollisionType_Wall);
	//m_moveSpeed.x = 0.0f;
	//m_moveSpeed.z = 0.0f;
	//m_moveSpeed.y = 0.0f;

	//m_radius = 1.0f;

	SetAlive(true);

#ifdef NOT_INSTANCING
	SINSTANCE(CShadowRender)->Entry(this);
#endif
	//SetAlpha(1.0f);
	//m_IsIntersect.CollisitionInitialize(&m_transform.position, m_radius);
	//m_hShaderTecnique = m_pEffect->GetTechniqueByName("NotNormalMapNonAnimationFresnelTec");
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
		m_rigidBody->activate();
		if (m_transform.position.y <= m_fallPosY){
			m_transform.position.y = m_fallPosY;
			m_eState = enState_Normal;
		}
		break;
	}

	m_rigidBody->getWorldTransform().setOrigin(btVector3(m_transform.position.x, m_transform.position.y, m_transform.position.z));
	m_rigidBody->getWorldTransform().setRotation(btQuaternion(m_transform.angle.x, m_transform.angle.y, m_transform.angle.z));

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
void CBlock::Build(const D3DXVECTOR3& size, const D3DXVECTOR3& pos)
{

	//���̈����ɓn���̂̓{�b�N�Xhalfsize�Ȃ̂ŁA0.5�{����B
	m_collisionShape = new btBoxShape(btVector3(size.x*0.5f, size.y*0.5f, size.z*0.5f));
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	float mass = 0.0f;

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	m_myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_myMotionState, m_collisionShape, btVector3(0, 0, 0));
	m_rigidBody = new btRigidBody(rbInfo);
	m_rigidBody->setUserIndex(CollisionType_Wall);
	//���[���h�ɒǉ��B
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddRigidBody_Dynamic(m_rigidBody);
}