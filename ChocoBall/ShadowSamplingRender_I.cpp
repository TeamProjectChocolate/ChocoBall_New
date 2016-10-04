#include "stdafx.h"
#include "ShadowSamplingRender_I.h"
#include "C3DImage.h"
#include "Camera.h"

CShadowSamplingRender_I::CShadowSamplingRender_I()
{
	m_WorldMatrixBuffer = nullptr;
	m_worldMatrix.clear();
	m_IsFirst = true;
}


CShadowSamplingRender_I::~CShadowSamplingRender_I()
{
	m_worldMatrix.clear();
	SAFE_DELETE(m_WorldMatrixBuffer);
	SAFE_DELETE(m_VertexDecl);
}

void CShadowSamplingRender_I::CopyMatrixToVertexBuffer()
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

void CShadowSamplingRender_I::Initialize(){
	if (m_IsFirst){
		m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/ShadowTex.hlsl"));	// �g�p����shader�t�@�C�����w��(�f�t�H���g)

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
						num + 1 /*UsageIndex�v�f(Usage���d�����Ă�����̂ɂ��āA�ŗL�ԍ���U���Ď��ʂ������)*/
					};
					offset += sizeof(float)* 4;
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

void CShadowSamplingRender_I::CreateMatrixBuffer(unsigned int MaxNum){
	//���[���h�s��p�̃o�b�t�@�̍쐬�B
	if (m_MaxNum < MaxNum){
		(*graphicsDevice()).CreateVertexBuffer(sizeof(D3DXMATRIX)* MaxNum, 0, 0, D3DPOOL_MANAGED, &m_WorldMatrixBuffer, 0);
		m_MaxNum = MaxNum;
	}
}

void CShadowSamplingRender_I::AddWorldMatrix(const D3DXMATRIX& mat){
	m_worldMatrix.push_back(mat);
}

void CShadowSamplingRender_I::Draw(){
	if (m_pModel){
		if (m_worldMatrix.size() <= 0){
			// �`�悷��I�u�W�F�N�g��������݂��Ȃ���Ε`�悵�Ȃ��B��������Ȃ��ƕK����`�悳��Ă��܂��B
			return;
		}
		DrawFrame(m_pModel->GetImage_3D()->pModel->GetFrameRoot());
	}
}

void CShadowSamplingRender_I::DrawFrame(LPD3DXFRAME pFrame){
	LPD3DXMESHCONTAINER pMeshContainer;

	pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != nullptr){
		DrawMeshContainer(pMeshContainer, pFrame);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != nullptr){
		DrawFrame(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != nullptr){
		DrawFrame(pFrame->pFrameFirstChild);
	}
}

void CShadowSamplingRender_I::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase){
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = static_cast<D3DXMESHCONTAINER_DERIVED*>(pMeshContainerBase);

	if (pMeshContainer->pSkinInfo != nullptr){
		// �X�L����񂠂�
		AnimationDraw(pMeshContainer, pFrame);
	}
	else{
		// �X�L�����Ȃ�
		NonAnimationDraw(pFrame);
	}
}

void CShadowSamplingRender_I::AnimationDraw(D3DXMESHCONTAINER_DERIVED* pMeshContainer, D3DXFRAME_DERIVED* pFrame){

	//LPD3DXBONECOMBINATION pBoneComb;
	//SetUpTechniqueAnimation();
	//pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

	//m_pEffect->Begin(0, D3DXFX_DONOTSAVESTATE);
	//m_pEffect->BeginPass(0);

	//SINSTANCE(CRenderContext)->GetCurrentCamera()->SetCamera(m_pEffect);
	//SINSTANCE(CRenderContext)->GetCurrentLight()->SetLight(m_pEffect);
	//SINSTANCE(CShadowRender)->SetShadowCamera(m_pEffect);
	//m_pEffect->SetFloat(m_hluminance, m_pModel->m_luminance);

	//// �{�[���̐�
	//m_pEffect->SetFloat(m_hnumBone, pMeshContainer->NumInfl);
	//m_pEffect->SetTexture(m_hTexture, pMeshContainer->ppTextures[pBoneComb[iattrib].AttribId]);

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
	//	m_pEffect->SetMatrixArray(m_hWorldMatrixArray, g_pBoneMatrices, pMeshContainer->NumPaletteEntries);

	//	//DrawSubset���g�p����ƃC���X�^���V���O�`�悪�s���Ȃ��̂�
	//	//g_pMesh���璸�_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�����������Ă��āA���ڕ`�悷��B
	//	LPDIRECT3DVERTEXBUFFER9 vb;
	//	LPDIRECT3DINDEXBUFFER9 ib;
	//	pMeshContainer->MeshData.pMesh->GetVertexBuffer(&vb);
	//	pMeshContainer->MeshData.pMesh->GetIndexBuffer(&ib);

	//	DWORD fvf = pMeshContainer->MeshData.pMesh->GetFVF();
	//	DWORD stride = D3DXGetFVFVertexSize(fvf);

	//	(*graphicsDevice()).SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_worldMatrix.size());
	//	(*graphicsDevice()).SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	//	(*graphicsDevice()).SetVertexDeclaration(m_VertexDecl);

	//	(*graphicsDevice()).SetStreamSource(0, vb, 0, stride);							//���_�o�b�t�@���X�g���[��0�Ԗڂɐݒ�
	//	(*graphicsDevice()).SetStreamSource(1, m_WorldMatrixBuffer, 0, sizeof(D3DXMATRIX));	//���[���h�s��p�̃o�b�t�@���X�g���[��1�Ԗڂɐݒ�B
	//	//���[���h�s��𒸓_�o�b�t�@�ɃR�s�[�B
	//	CopyWorldMatrixToVertexBuffer();
	//	(*graphicsDevice()).SetIndices(ib);
	//	m_pEffect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B
	//	(*graphicsDevice()).DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pMeshContainer->MeshData.pMesh->GetNumVertices(), 0, pMeshContainer->MeshData.pMesh->GetNumFaces());
	//	vb->Release();
	//	ib->Release();
	//}
	////m_pEffect->CommitChanges();
	////pMeshContainer->MeshData.pMesh->DrawSubset(iattrib);
	//m_pEffect->EndPass();
	//m_pEffect->End();

	//// �g���I��������[���h�s��Q��j��
	//m_worldMatrix.clear();
}


void CShadowSamplingRender_I::NonAnimationDraw(D3DXFRAME_DERIVED* pFrame){
	m_pEffect->SetTechnique("BonelessShadowMapping_I");
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pShadowCamera->SetCamera(m_pEffect);
	D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->pModel->GetContainer();

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
		(*graphicsDevice()).SetStreamSource(1, m_WorldMatrixBuffer, 0, sizeof(D3DXMATRIX));	//���[���h�s��p�̃o�b�t�@���X�g���[��1�Ԗڂɐݒ�B
		//���[���h�s��𒸓_�o�b�t�@�ɃR�s�[�B
		CopyMatrixToVertexBuffer();
		(*graphicsDevice()).SetIndices(ib);
		m_pEffect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B
		(*graphicsDevice()).DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, container->MeshData.pMesh->GetNumVertices(), 0, container->MeshData.pMesh->GetNumFaces());
		vb->Release();
		ib->Release();
	}

	m_pEffect->EndPass();
	m_pEffect->End();

	m_worldMatrix.clear();
	m_pModel = nullptr;
}
