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

	SetAlive(true);

	//UseModel<C3DImage>();
	//m_pModel->SetFileName(FieldNameArray[m_StageID]);

	CGameObject::Initialize();
#ifdef NOT_VSM
#else
	SINSTANCE(CShadowRender)->Entry(this);
	m_CourceDef.SetStageID(m_StageID);
	m_CourceDef.Initialize();

	// 一度Transformを使ってワールド行列を生成。
	m_pModel->Update(m_transform);

	// 頂点情報に影を落とすか落とさないかの境界線を埋め込む。
	// フィールドの屋根の影を描画しないため。
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
			D3DXVECTOR3 HorizonVec;
			D3DXVECTOR3 WorldPos;
			// 頂点バッファの頂点座標はローカル座標なので、一度ワールド座標系に変換する。
			D3DXVec3Transform(&static_cast<D3DXVECTOR4>(WorldPos),pPosition,&(m_pModel->m_World));
			// 接触しているコースを探索。
			work = m_CourceDef.FindCource(WorldPos);
			if (work.blockNo != -1) {
				// コースに接触している。
				D3DXVECTOR3 CourceVec = work.endPosition - work.startPosition;
				D3DXVECTOR3 WorkVec = WorldPos - work.startPosition;
				D3DXVec3Normalize(&CourceVec, &CourceVec);
				// 頂点の座標をコース定義の線分に射影し、y成分を境界線にする。
				float WorkLength = D3DXVec3Dot(&CourceVec,&WorkVec);
				HorizonVec = work.startPosition + (CourceVec * WorkLength);
			}
			else {
				// コースに接触していない。
				HorizonVec.y = FLT_MAX;
			}
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
	if (m_czbuffersphere){
		m_pRender->GetEffect()->SetTexture("g_ZMask", m_czbuffersphere->GetTexture());
	}
	CGameObject::Draw();
}

void CField::Is_DrawShadow_Use_Horizon() {
	m_pModel->SetIsHorizon(true);
}