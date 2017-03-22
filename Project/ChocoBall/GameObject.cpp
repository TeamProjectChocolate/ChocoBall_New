#include "stdafx.h"
#include "GameObject.h"
#include "Effect.h"
#include "RenderContext.h"
#include "Camera.h"
#include "EM_SamplingRender.h"
#include "EM_SamplingRender_I.h"
#include "ActreLight.h"
#include "BulletPhysics.h"
#include "ObjectManager.h"
#include "CollisionObject.h"
#include "Rigidbody.h"

CGameObject::~CGameObject()
{
	if (m_UseModel){
		SAFE_DELETE(m_pModel);
	}
	if (m_HasMyLight) {
		SAFE_DELETE(m_pLight);
	}
}

void CGameObject::Initialize(){
	//if (m_UseModel) {
		m_pModel->Initialize();
		SetRenderState();
		m_pRender = SINSTANCE(CRenderContext)->SelectRender(m_RenderingState, m_pRenderName, false, m_pModel);
#ifdef NOT_EM
#else
		EM_SetRenderState();
		m_pEMSamplingRender = SINSTANCE(CRenderContext)->SelectRender(m_EMRenderingState, m_pRenderName, false, m_pModel);
#endif
	//}
}

// コリジョンを生成して当たり判定を開始。
void CGameObject::ActivateCollision(const D3DXVECTOR3& offset, btCollisionShape* pShape,Collision::Type Type,Collision::FilterGroup group, bool isTrigger,float mass,bool isKinematic,bool isAddWorld) {

	if(isTrigger){
		// コリジョンのみ生成。
		m_CollisionObject.reset(new CCollisionObject);
	}
	else {
		// 剛体生成。
		m_CollisionObject.reset(new CRigidbody);
	}
	m_CollisionObject->InitCollision(this, m_transform, offset, pShape, Type, group, mass, isKinematic, isAddWorld);
}

void CGameObject::InitInstancing(int num,bool isNull){
	if (isNull){
		if (static_cast<CInstancingRender*>(m_pRender)->GetWorldMatrixBuffer() == nullptr){
			static_cast<CInstancingRender*>(m_pRender)->CreateMatrixBuffer(num);
		}
	}
	else{
		static_cast<CInstancingRender*>(m_pRender)->CreateMatrixBuffer(num);
	}
#ifdef NOT_EM
#else
	static_cast<CEM_SamplingRender_I*>(m_pEMSamplingRender)->CreateMatrixBuffer(num);
#endif

}

void CGameObject::Update(){
	if (m_CollisionObject.get()) {
		m_CollisionObject->Update(&(m_transform.position), &(m_transform.angle));
	}
	m_pModel->Update(m_transform);
	// 向きベクトルに回転行列のZ成分を格納。
	D3DXMATRIX rota = m_pModel->GetRotation();
	m_Direction = D3DXVECTOR3(rota.m[2][0], rota.m[2][1], rota.m[2][2]);
	if (m_HasMyLight) {
		// 女優ライトを使用する場合はワールド行列でライトを更新する。
		static_cast<CActreLight*>(m_pLight)->Update(m_pModel->m_World);
	}
}

void CGameObject::Draw(){
	SetUpTechnique();
	m_pRender->SetModelData(m_pModel);
	if (m_HasMyLight) {
		m_pRender->SetMyLight(m_pLight);
	}
	m_pRender->Draw();
}

void CGameObject::DrawShadow(CCamera* camera){
	SetUpShadowTechnique();
	m_pShadowRender->SetShadowCamera(camera);
	m_pShadowRender->SetModelData(m_pModel);
	Is_DrawShadow_Use_Horizon();
	m_pShadowRender->Draw();
}

void CGameObject::Draw_EM(CCamera* camera){
#ifdef NOT_EM
#else
	EM_SetUpTechnique();
	static_cast<CEM_SamplingRender*>(m_pEMSamplingRender)->SetCamera(camera);
	m_pEMSamplingRender->SetModelData(m_pModel);
	m_pEMSamplingRender->Draw();
#endif
}

void CGameObject::Is_DrawShadow_Use_Horizon(){
	m_pModel->SetIsHorizon(false);
}