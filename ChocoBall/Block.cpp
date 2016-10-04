#include "stdafx.h"
#include "Block.h"
#include "ShadowRender.h"
#include "BulletPhysics.h"
#include "RenderContext.h"

void CBlock::OnDestroy(){
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->RemoveRigidBody(m_rigidBody);
	//子供に死亡したことを通知。
	if (m_child){
		m_child->OnDestroyParent();
	}
	m_isDead = true;
}


//親が死んだときに呼ばれる処理。
void CBlock::OnDestroyParent()
{
	m_eState = enState_Fall;
	m_fallPosY = m_parent->GetPos().y;
	SetParent(m_parent->m_parent);
	//自分の子供を落下させる。
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
	SetRenderState();
	CGameObject::Initialize();
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

	//SetAlpha(1.0f);
	//m_IsIntersect.CollisitionInitialize(&m_transform.position, m_radius);
	//m_hShaderTecnique = m_pEffect->GetTechniqueByName("NotNormalMapNonAnimationFresnelTec");
}


void CBlock::Update()
{
	switch (m_eState){
	case enState_Normal:{
		//物理エンジンで計算された剛体の位置と回転を反映させる。
		//const btVector3& rPos = m_rigidBody->getWorldTransform().getOrigin();
		//const btQuaternion& rRot = m_rigidBody->getWorldTransform().getRotation();
		//D3DXVECTOR3 pos(rPos.x(), rPos.y(), rPos.z());
		//D3DXQUATERNION rot(rRot.x(), rRot.y(), rRot.z(), rRot.w());
		//m_transform.position.x = rPos.x();
		//m_transform.position.y = rPos.y();
		//m_transform.position.z = rPos.z();
		//m_box.SetPosition(pos);
		//m_box.SetRotation(rot);
		//m_box.UpdateWorldMatrix();
		//m_shadowModel.SetWorldMatrix(m_box.GetWorldMatrix());
		//ShadowMap().Entry(&m_shadowModel);
		//m_life += 1.0f / 60.0f;
		////m_transform.position.y -= 0.05f;

		//btVector3 a(0.0f, 0.0f, 1.0f);

		//if (GetAsyncKeyState('A')){
		//	m_rigidBody->setLinearVelocity(a);
		//}
		btTransform& trans = m_rigidBody->getWorldTransform();
		trans.setOrigin(btVector3(m_transform.position.x, m_transform.position.y, m_transform.position.z));
		trans.setRotation(btQuaternion(m_transform.angle.x, m_transform.angle.y, m_transform.angle.z));
	}break;
	case enState_Broken:{
	}break;
	case enState_Fall:{
		m_transform.position.y -= 0.1f;
		if (m_fallPosY > m_transform.position.y){
			m_transform.position.y = m_fallPosY;
			m_eState = enState_Normal;
		}
	}break;
	}

	CGameObject::Update();
}

void CBlock::Draw()
{
}

void CBlock::BeginDraw()
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
void CBlock::EndDraw()
{
	m_pRender->GetEffect()->EndPass();
	m_pRender->GetEffect()->End();
}
void CBlock::Build(const D3DXVECTOR3& size, const D3DXVECTOR3& pos)
{

	//この引数に渡すのはボックスhalfsizeなので、0.5倍する。
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
	//ワールドに追加。
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddRigidBody(m_rigidBody);
}