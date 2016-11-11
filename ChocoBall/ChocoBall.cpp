#include "stdafx.h"
#include "ChocoBall.h"
#include "CollisionType.h"
#include "RenderContext.h"
#include "ShadowRender.h"

void CChocoBall::Initialize(D3DXVECTOR3 Spos, D3DXVECTOR3 Epos)
{
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/ball.x");
	CGameObject::Initialize();

#ifdef NOT_INSTANCING
 	m_pModel->m_alpha = 1.0f;
	m_pModel->m_luminance = 0.0f;
	m_pModel->m_Refractive = g_RefractivesTable[REFRACTIVES::CHOCOLATE];
#endif
	m_transform.position = Spos; //D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	SetVector(m_transform.position, Epos);
	SetRotation(D3DXVECTOR3(0, 0, 0), 0.1f);
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Rigidbody.Initialize(&m_transform.position, 0.3f);
	m_Rigidbody.SetUserIndex(CollisionType_Chocoball);

	//m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
	m_moveSpeed.x = 0.05f;
	m_moveSpeed.z = 0.05f;
	m_moveSpeed.y = 0.05f;

	//m_radius = 1.0f;

	SetAlive(true);

	//m_IsIntersect.CollisitionInitialize(&m_transform.position, m_radius);
	D3DXVec3Normalize(&m_Vector2, &GetVector());
}


void CChocoBall::Update()
{
	//物理エンジンで計算された剛体の位置と回転を反映させる。
	//const btVector3& rPos = m_rigidBody->getWorldTransform().getOrigin();
	//const btQuaternion& rRot = m_rigidBody->getWorldTransform().getRotation();
	//D3DXVECTOR3 pos(rPos.x(), rPos.y(), rPos.z());
	//D3DXQUATERNION rot(rRot.x(), rRot.y(), rRot.z(), rRot.w());
	//
	//m_transform.position.x += rPos.x();
	//m_transform.position.y += rPos.y();
	//m_transform.position.z += rPos.z();

	m_Rigidbody.ApplyForce(m_Vector2 * 5.0f);
//	m_Rigidbody.SetAngularVelocity(D3DXVECTOR3(10.0f, 10.f, 10.0f));
	//SetVector(m_transform.position, m_Vector);
	//m_box.SetPosition(pos);
	//m_box.SetRotation(rot);
	//m_box.UpdateWorldMatrix();
	//m_shadowModel.SetWorldMatrix(m_box.GetWorldMatrix());
	//ShadowMap().Entry(&m_shadowModel);
	//m_life += 1.0f / 60.0f;
	//m_transform.position.y -= 0.05f;
	//btVector3 a(0.0f, 0.0f, 1.0f);



	m_Rigidbody.Update(&m_transform.position);
	CGameObject::Update();
}

void CChocoBall::BeginDraw()
{
	SetUpTechnique();
	UINT numPass;
	m_pRender->GetEffect()->Begin(&numPass/*テクニック内に定義されているパスの数が返却される*/, 0);
	m_pRender->GetEffect()->BeginPass(0);	//パスの番号を指定してどのパスを使用するか指定

	// 現在のプロジェクション行列とビュー行列をシェーダーに転送
	SINSTANCE(CRenderContext)->GetCurrentCamera()->SetCamera(m_pRender->GetEffect());
	SINSTANCE(CRenderContext)->GetCurrentLight()->SetLight(m_pRender->GetEffect());
	// 視点をシェーダーに転送
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
}
