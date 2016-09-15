#include "stdafx.h"
#include "InstancingRender_D.h"
#include "C3DImage.h"
#include "Camera.h"

CInstancingRender_D::CInstancingRender_D()
{
	m_worldMatrix.clear();
	m_IsFirst = true;
}


CInstancingRender_D::~CInstancingRender_D()
{
	SAFE_DELETE(m_WorldMatrixBuffer);
	SAFE_DELETE(m_VertexDecl);
}

void CInstancingRender_D::CopyMatrixToVertexBuffer()
{
	D3DVERTEXBUFFER_DESC desc;
	m_WorldMatrixBuffer->GetDesc(&desc);
	D3DXMATRIX* pData;
	m_WorldMatrixBuffer->Lock(0, desc.Size, (void**)&pData, D3DLOCK_DISCARD);

	for (int i = 0; i < m_worldMatrix.size(); i++) {
		*pData = m_worldMatrix[i];
		pData++;
	}
	m_WorldMatrixBuffer->Unlock();
}

void CInstancingRender_D::Initialize(){
	if (m_IsFirst){
		m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/DepthOfField.hlsl"));	// �g�p����shader�t�@�C�����w��(�f�t�H���g)
		//�C���X�^���V���O�`��p�̏������B
		D3DVERTEXELEMENT9 declElement[MAX_FVF_DECL_SIZE];
		D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->pModel->GetContainer();
		// ���݂̒��_�錾�I�u�W�F�N�g���擾(�����_�錾�I�u�W�F�N�g�Ƃ́A���_�f�[�^���ǂ̂悤�ȍ\���ɂȂ��Ă��邩��錾�������̂ł���)
		container->MeshData.pMesh->GetDeclaration(declElement);
		// ���_�̐錾�̏I�[��T��
		int elementIndex = 0;
		while (true) {
			if (declElement[elementIndex].Type == D3DDECLTYPE_UNUSED) {
				//�I�[�𔭌��B
				//��������C���X�^���V���O�p�̒��_���C�A�E�g�𖄂ߍ��ށB
				WORD offset = 0;
				for (short num = 0; num < 4; num++){
					// WORLD�s���float4x4�Ȃ̂ŁA1�s����float4�Œ�`���Ă���
					declElement[elementIndex] = {
						1,	// �X�g���[���ԍ�
						offset,	/*offset�v�f(�e�X�g���[���̐擪���牽�Ԗڂɒ�`����Ă��邩��\������[sizeof(�^��) * ����]�̏�����)*/
						D3DDECLTYPE_FLOAT4,	// ���_�����ǂ̌����g�p���Đ錾���邩(�s���float4�̗v�f���l�g�p���č\������)
						D3DDECLMETHOD_DEFAULT, // �e�b�Z���[�V����(�|���S������)�̕��@���w��B���ʃe�b�Z���[�V�����͂��܂�g��Ȃ����߁A�f�t�H���g��(D3DDECLMETHOD_DEFAULT)�ŏ\��
						D3DDECLUSAGE_TEXCOORD,	// usage�v�f
						num + 1/*UsageIndex�v�f(Usage���d�����Ă�����̂ɂ��āA�ŗL�ԍ���U���Ď��ʂ������)*/
					};
					offset += sizeof(float) * 4;
					elementIndex++;
				}
				declElement[elementIndex] = D3DDECL_END();	// ���_�錾�̏I�[(�ԕ�)��ݒ�(���m�ɂ́A���_�錾�̍Ō�̗v�f�����������Ă���)
				break;
			}
			elementIndex++;
		}
		//��Ő錾�������_���Œ��_��`���쐬�B
		(*graphicsDevice()).CreateVertexDeclaration(declElement, &m_VertexDecl);
		m_IsFirst = false;
	}
}

void CInstancingRender_D::CreateMatrixBuffer(int MaxNum){
	//���[���h�s��p�̃o�b�t�@�̍쐬�B
	if (m_WorldMatrixBuffer == nullptr){
		(*graphicsDevice()).CreateVertexBuffer(sizeof(D3DXMATRIX)* MaxNum, 0, 0, D3DPOOL_MANAGED, &m_WorldMatrixBuffer, 0);
	}
}

void CInstancingRender_D::AddWorldMatrix(const D3DXMATRIX& mat){
	m_worldMatrix.push_back(mat);
}

void CInstancingRender_D::Draw(const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){
	if (m_pModel){
		DrawFrameDepth(m_pModel->GetImage_3D()->pModel->GetFrameRoot(), m_pEffect, FarNear, PintoPos, PintoWorld);
	}
}

void CInstancingRender_D::DrawFrameDepth(LPD3DXFRAME pFrame, LPD3DXEFFECT effect, const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){
	LPD3DXMESHCONTAINER pMeshContainer;

	pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != nullptr){
		DrawMeshContainerDepth(pMeshContainer, pFrame, effect, FarNear, PintoPos, PintoWorld);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != nullptr){
		DrawFrameDepth(pFrame->pFrameSibling, effect, FarNear, PintoPos, PintoWorld);
	}

	if (pFrame->pFrameFirstChild != nullptr){
		DrawFrameDepth(pFrame->pFrameFirstChild, effect, FarNear, PintoPos, PintoWorld);
	}
}

void CInstancingRender_D::DrawMeshContainerDepth(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase, LPD3DXEFFECT effect, const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = static_cast<D3DXMESHCONTAINER_DERIVED*>(pMeshContainerBase);

	if (pMeshContainer->pSkinInfo != nullptr){
		// �X�L����񂠂�
		AnimationDrawDepth(pMeshContainer, pFrame, effect, FarNear, PintoPos, PintoWorld);
	}
	else{
		// �X�L�����Ȃ�
		NonAnimationDrawDepth(pFrame, effect, FarNear, PintoPos, PintoWorld);
	}
}

void CInstancingRender_D::AnimationDrawDepth(D3DXMESHCONTAINER_DERIVED* pMeshContainer, D3DXFRAME_DERIVED* pFrame, LPD3DXEFFECT effect, const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){

	//LPD3DXBONECOMBINATION pBoneComb;
	//effect->SetTechnique("DepthSampling_Animation");
	//pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

	//for (unsigned int iattrib = 0; iattrib < pMeshContainer->NumAttributeGroups; iattrib++){
	//	for (DWORD iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry){
	//		DWORD iMatrixIndex = pBoneComb[iattrib].BoneId[iPaletteEntry];
	//		if (iMatrixIndex != UINT_MAX){
	//			D3DXMatrixMultiply(
	//				&g_pBoneMatrices[iPaletteEntry],
	//				&pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
	//				pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]
	//				);
	//		}
	//	}
	//	effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	//	effect->BeginPass(0);

	//	effect->SetVector("g_PintoPoint", &(static_cast<D3DXVECTOR4>(PintoPos)));
	//	effect->SetVector("g_FarNear", &(static_cast<D3DXVECTOR4>(FarNear)));
	//	effect->SetMatrix("g_PintoWorld", &PintoWorld);// �s���g�����킹��|�C���g���s��ϊ����邽�߂̃��[���h�s��
	//	effect->SetMatrix("g_Proj", &(SINSTANCE(CRenderContext)->GetCurrentCamera()->GetProj()));
	//	effect->SetMatrix("g_View", &(SINSTANCE(CRenderContext)->GetCurrentCamera()->GetView()));
	//	effect->SetMatrixArray("g_WorldMatrixArray", g_pBoneMatrices, pMeshContainer->NumPaletteEntries);

	//	// �{�[���̐�
	//	effect->SetFloat("g_numBone", pMeshContainer->NumInfl);

	//	effect->CommitChanges();
	//	pMeshContainer->MeshData.pMesh->DrawSubset(iattrib);
	//	effect->EndPass();
	//	effect->End();
	//}

}

void CInstancingRender_D::NonAnimationDrawDepth(D3DXFRAME_DERIVED* pFrame, LPD3DXEFFECT effect, const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){

	D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->pModel->GetContainer();

	(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, TRUE);

	effect->SetTechnique("DepthSampling_NonAnimation_I");
	m_pEffect->Begin(nullptr, D3DXFX_DONOTSAVESHADERSTATE);
	m_pEffect->BeginPass(0);	//�p�X�̔ԍ����w�肵�Ăǂ̃p�X���g�p���邩�w��

	// ���݂̃v���W�F�N�V�����s��ƃr���[�s����V�F�[�_�[�ɓ]��
	effect->SetMatrix("g_Proj", &(SINSTANCE(CRenderContext)->GetCurrentCamera()->GetProj()));
	effect->SetMatrix("g_View", &(SINSTANCE(CRenderContext)->GetCurrentCamera()->GetView()));

	effect->SetVector("g_FarNear", &(static_cast<D3DXVECTOR4>(FarNear)));
	effect->SetVector("g_PintoPoint", &(static_cast<D3DXVECTOR4>(PintoPos)));
	effect->SetMatrix("g_PintoWorld", &PintoWorld);// �s���g�����킹��|�C���g���s��ϊ����邽�߂̃��[���h�s��

	// �C���X�^���V���O�`��
	{
		//DrawSubset���g�p����ƃC���X�^���V���O�`�悪�s���Ȃ��̂�
		//g_pMesh���璸�_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�����������Ă��āA���ڕ`�悷��B
		LPDIRECT3DVERTEXBUFFER9 vb;
		LPDIRECT3DINDEXBUFFER9 ib;
		container->MeshData.pMesh->GetVertexBuffer(&vb);
		container->MeshData.pMesh->GetIndexBuffer(&ib);

		DWORD fvf = container->MeshData.pMesh->GetFVF();
		DWORD stride = D3DXGetFVFVertexSize(fvf);

		(*graphicsDevice()).SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_worldMatrix.size());
		(*graphicsDevice()).SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

		(*graphicsDevice()).SetVertexDeclaration(m_VertexDecl);

		(*graphicsDevice()).SetStreamSource(0, vb, 0, stride);							//���_�o�b�t�@���X�g���[��0�Ԗڂɐݒ�
		//���[���h�s��𒸓_�o�b�t�@�ɃR�s�[�B
		CopyMatrixToVertexBuffer();
		(*graphicsDevice()).SetStreamSource(1, m_WorldMatrixBuffer, 0, sizeof(D3DXMATRIX));	//���[���h�s��p�̃o�b�t�@���X�g���[��1�Ԗڂɐݒ�B
		(*graphicsDevice()).SetIndices(ib);
		m_pEffect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B
		(*graphicsDevice()).DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, container->MeshData.pMesh->GetNumVertices(), 0, container->MeshData.pMesh->GetNumFaces());
		vb->Release();
		ib->Release();
	}

	m_pEffect->EndPass();
	m_pEffect->End();

	// �g���I��������[���h�s��Q��j��
	m_worldMatrix.clear();

	m_pModel = nullptr;
}