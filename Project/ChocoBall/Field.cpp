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

	// ��xTransform���g���ă��[���h�s��𐶐��B
	m_pModel->Update(m_transform);

	// ���_���ɉe�𗎂Ƃ������Ƃ��Ȃ����̋��E���𖄂ߍ��ށB
	// �t�B�[���h�̉����̉e��`�悵�Ȃ����߁B
	{
		LPDIRECT3DVERTEXBUFFER9 vb;
		m_pModel->GetImage_3D()->GetContainer()->MeshData.pMesh->GetVertexBuffer(&vb);
		D3DVERTEXBUFFER_DESC desc;
		vb->GetDesc(&desc);
		D3DXVECTOR3* pPosition;
		vb->Lock(0, desc.Size,(void**)&pPosition, D3DLOCK_DISCARD);

		// 1���_�̃o�C�g�P�ʂł̃T�C�Y���擾�B
		int stride = desc.Size / m_pModel->GetImage_3D()->GetContainer()->MeshData.pMesh->GetNumVertices();
		for (int idx = 0; idx < m_pModel->GetImage_3D()->GetContainer()->MeshData.pMesh->GetNumVertices(); idx++) {
			COURCE_BLOCK work;
			D3DXVECTOR3 HorizonVec;
			D3DXVECTOR3 WorldPos;
			// ���_�o�b�t�@�̒��_���W�̓��[�J�����W�Ȃ̂ŁA��x���[���h���W�n�ɕϊ�����B
			D3DXVec3Transform(&static_cast<D3DXVECTOR4>(WorldPos),pPosition,&(m_pModel->m_World));
			// �ڐG���Ă���R�[�X��T���B
			work = m_CourceDef.FindCource(WorldPos);
			if (work.blockNo != -1) {
				// �R�[�X�ɐڐG���Ă���B
				D3DXVECTOR3 CourceVec = work.endPosition - work.startPosition;
				D3DXVECTOR3 WorkVec = WorldPos - work.startPosition;
				D3DXVec3Normalize(&CourceVec, &CourceVec);
				// ���_�̍��W���R�[�X��`�̐����Ɏˉe���Ay���������E���ɂ���B
				float WorkLength = D3DXVec3Dot(&CourceVec,&WorkVec);
				HorizonVec = work.startPosition + (CourceVec * WorkLength);
			}
			else {
				// �R�[�X�ɐڐG���Ă��Ȃ��B
				HorizonVec.y = FLT_MAX;
			}
			m_Horizon.push_back(HorizonVec.y);
			// D3DXVECTOR3*�^��1�o�C�g�ł͂Ȃ��̂�1�o�C�g�P�ʂł���char*�^�ɕϊ����ă|�C���^��i�߂�B
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