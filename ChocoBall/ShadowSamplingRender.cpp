#include "stdafx.h"
#include "ShadowSamplingRender.h"
#include "Model.h"
#include "C3DImage.h"
#include "Camera.h"

CShadowSamplingRender::CShadowSamplingRender()
{
	m_pShadowCamera = nullptr;
}

CShadowSamplingRender::~CShadowSamplingRender()
{
}

void CShadowSamplingRender::Initialize(){
	m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/ShadowTex.hlsl"));	// �g�p����shader�t�@�C�����w��(�f�t�H���g)
}

void CShadowSamplingRender::Draw(){
	DrawFrame(m_pModel->GetImage_3D()->pModel->GetFrameRoot());
}

void CShadowSamplingRender::DrawFrame(LPD3DXFRAME pFrame){
	LPD3DXMESHCONTAINER pMeshContainer;

	pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != nullptr){
		DrawMeshContainer(pMeshContainer);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != nullptr){
		DrawFrame(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != nullptr){
		DrawFrame(pFrame->pFrameFirstChild);
	}
}

void CShadowSamplingRender::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase){
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = static_cast<D3DXMESHCONTAINER_DERIVED*>(pMeshContainerBase);

	if (pMeshContainer->pSkinInfo != nullptr){
		// �{�[������̉e�̕`��
		AnimationDraw(pMeshContainer);
	}
	else{
		// �{�[���Ȃ��̉e�̕`��
		NonAnimationDraw();
	}
}

void CShadowSamplingRender::AnimationDraw(D3DXMESHCONTAINER_DERIVED* pMeshContainer){
	LPD3DXBONECOMBINATION pBoneComb;

	pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
	for (unsigned int iattrib = 0; iattrib < pMeshContainer->NumAttributeGroups; iattrib++){
		for (DWORD iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry){
			DWORD iMatrixIndex = pBoneComb[iattrib].BoneId[iPaletteEntry];
			if (iMatrixIndex != UINT_MAX){
				D3DXMatrixMultiply(
					&g_pBoneMatrices[iPaletteEntry],
					&pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
					pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]
					);
			}
		}

		m_pEffect->SetTechnique("BoneShadowMapping");
		m_pEffect->Begin(NULL, 0);
		m_pEffect->BeginPass(0);

		m_pShadowCamera->SetCamera(m_pEffect);

		m_pEffect->SetMatrixArray("g_WorldMatrixArray", g_pBoneMatrices, pMeshContainer->NumPaletteEntries);

		// �{�[���̐�
		m_pEffect->SetFloat("g_numBone", pMeshContainer->NumInfl);

		m_pEffect->CommitChanges();
		pMeshContainer->MeshData.pMesh->DrawSubset(iattrib);
	}
	m_pEffect->EndPass();
	m_pEffect->End();
}

void CShadowSamplingRender::NonAnimationDraw(){
	m_pEffect->SetTechnique("BonelessShadowMapping");
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pShadowCamera->SetCamera(m_pEffect);
	m_pEffect->SetMatrix("World"/*�G�t�F�N�g�t�@�C�����̕ϐ���*/, &(m_pModel->m_World)/*�ݒ肵�����s��ւ̃|�C���^*/);
	// ���_�t�H�[�}�b�g���Z�b�g

	D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->pModel->GetContainer();

	m_pEffect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B

	for (DWORD i = 0; i < container->NumMaterials; i++){
		container->MeshData.pMesh->DrawSubset(i);						// ���b�V����`��
	}
	m_pEffect->EndPass();
	m_pEffect->End();
}

