#include "stdafx.h"
#include "Field.h"
#include "CollisionType.h"
#include "ObjectManager.h"
#include "StageTable.h"
#include "ZBufferSphere.h"
#include "RenderContext.h"
#include "Camera.h"
#include "C3DImage.h"
#include "ShadowRender.h"

CField::~CField()
{
	m_Horizon.clear();
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
#ifdef NOT_VSM
#else
	SINSTANCE(CShadowRender)->Entry(this);
	m_CourceDef.SetStageID(m_StageID);
	m_CourceDef.Initialize();
	{
		LPDIRECT3DVERTEXBUFFER9 vb;
		m_pModel->GetImage_3D()->GetContainer()->MeshData.pMesh->GetVertexBuffer(&vb);
		D3DVERTEXBUFFER_DESC desc;
		vb->GetDesc(&desc);
		D3DXVECTOR3* pPosition;
		vb->Lock(0, desc.Size,(void**)&pPosition, D3DLOCK_DISCARD);

		// 1頂点のバイト単位でのサイズを取得。
		int stride = desc.Size / m_pModel->GetImage_3D()->GetContainer()->MeshData.pMesh->GetNumVertices();
		for (int idx = 0; idx < m_pModel->GetImage_3D()->GetContainer()->MeshData.pMesh->GetNumVertices(); idx++) {
			COURCE_BLOCK work;
			work = m_CourceDef.FindCource(*pPosition);
			D3DXVECTOR3 CourceVec = work.endPosition - work.startPosition;
			D3DXVECTOR3 WorkVec = *pPosition - work.startPosition;
			D3DXVec3Normalize(&CourceVec, &CourceVec);
			// 頂点の座標をコース定義の線分に射影し、y成分を境界線にする。
			float WorkLength = D3DXVec3Dot(&WorkVec, &CourceVec);
			D3DXVECTOR3 HorizonVec = work.startPosition + CourceVec * WorkLength;
			m_Horizon.push_back(HorizonVec.y);
			// D3DXVECTOR3*型は1バイトではないので1バイト単位であるchar*型に変換してポインタを進める。
			char* OneByte = reinterpret_cast<char*>(pPosition);
			OneByte += stride;
			pPosition = reinterpret_cast<D3DXVECTOR3*>(OneByte);
		}
		vb->Unlock();

		m_pModel->CopyHorizon(m_Horizon);
	}

#endif

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

void CField::Is_DrawShadow_Use_Horizon() {
	m_pModel->SetIsHorizon(true);
}