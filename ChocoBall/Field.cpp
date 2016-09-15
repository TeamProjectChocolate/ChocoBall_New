#include "stdafx.h"
#include "Field.h"
#include "CollisionType.h"
#include "ObjectManager.h"
#include "StageTable.h"
#include "ZBufferSphere.h"
#include "RenderContext.h"
#include "Camera.h"
#include "C3DImage.h"

CField::~CField()
{
}

void CField::Initialize(){
	m_czbuffersphere = NULL;
	m_transform.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXToRadian(90.0f));
	//m_transform.angle = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//剛体を初期化。
	{
		//この引数に渡すのはボックスのhalfsizeなので、0.5倍する。
		SCollisionInfo* Table = collisionInfoTableArray[m_StageID];
		int arraySize = collisionInfoTableSizeArray[m_StageID];	//配列の要素数を返す。
		for (int i = 0; i < arraySize; i++) {
			SCollisionInfo& collision = Table[i];
			btBoxShape* work = new btBoxShape(btVector3(collision.scale.x*0.5f, collision.scale.y*0.5f, collision.scale.z*0.5f));
			m_groundShape.push_back(work);
			btTransform groundTransform;
			groundTransform.setIdentity();
			groundTransform.setOrigin(btVector3(-collision.pos.x, collision.pos.y, -collision.pos.z));
			groundTransform.setRotation(btQuaternion(-collision.rotation.x, collision.rotation.y, -collision.rotation.z, collision.rotation.w));
			float mass = 0.0f;

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			m_myMotionState = new btDefaultMotionState(groundTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_myMotionState, work, btVector3(0, 0, 0));
			btRigidBody* work2 = new btRigidBody(rbInfo);
			m_rigidBody.push_back(work2);
			work2->activate();
			m_rigidBody[i]->setUserIndex(CollisionType_Map);
			//ワールドに追加。
			SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddRigidBody(work2);
		}
	}

	SetAlive(true);

	UseModel<C3DImage>();
	m_pModel->SetFileName(FieldNameArray[m_StageID]);
	CGameObject::Initialize();
}

void CField::Update(){
	CGameObject::Update();
}

void CField::Draw(){
	if (m_czbuffersphere == NULL){
		m_czbuffersphere = SINSTANCE(CObjectManager)->FindGameObject<CZBufferSphere>(_T("ZBufferSphere"));
	}
	if (m_czbuffersphere){
		m_pRender->GetEffect()->SetTexture("g_ZMask", m_czbuffersphere->GetTexture());
	}
	CGameObject::Draw();
}

void CField::DrawDepth(const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){
	D3DXMATRIX World;
	World = m_pModel->m_World;

	LPD3DXEFFECT effect = SINSTANCE(CRenderContext)->FindRender<CDofRender>(RENDER_STATE::Dof, "")->GetEffect();

	effect->SetTechnique("DepthSampling_NonAnimation_Z");
	effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	effect->BeginPass(0);


	// 現在のプロジェクション行列とビュー行列をシェーダーに転送
	effect->SetVector("g_FarNear", &(static_cast<D3DXVECTOR4>(FarNear)));

	if (m_czbuffersphere == NULL){
		m_czbuffersphere = SINSTANCE(CObjectManager)->FindGameObject<CZBufferSphere>(_T("ZBufferSphere"));
	}
	if (m_czbuffersphere){
		effect->SetTexture("g_ZMaskSample", m_czbuffersphere->GetTexture());
	}

	effect->SetVector("g_PintoPoint", &(static_cast<D3DXVECTOR4>(PintoPos)));
	effect->SetMatrix("g_Proj", &(SINSTANCE(CRenderContext)->GetCurrentCamera()->GetProj()));
	effect->SetMatrix("g_View", &(SINSTANCE(CRenderContext)->GetCurrentCamera()->GetView()));
	effect->SetMatrix("g_PintoWorld", &PintoWorld);// ピントを合わせるポイントを行列変換するためのワールド行列
	effect->SetMatrix("g_World", &World/*設定したい行列へのポインタ*/);

	D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->pModel->GetContainer();

	for (DWORD i = 0; i < container->NumMaterials; i++){
		effect->CommitChanges();						//この関数を呼び出すことで、データの転送が確定する。
		container->MeshData.pMesh->DrawSubset(i);						// メッシュを描画
	}
	effect->EndPass();
	effect->End();


}