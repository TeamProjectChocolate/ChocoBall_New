#include "stdafx.h"
#include "C3DRender_D.h"
#include "C3DImage.h"
#include "RenderContext.h"
#include "Camera.h" 

C3DRender_D::C3DRender_D()
{
}


C3DRender_D::~C3DRender_D()
{
}

void C3DRender_D::Initialize(){
	m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/DepthOfField.hlsl"));	// �g�p����shader�t�@�C�����w��(�f�t�H���g)
}

void C3DRender_D::Draw(const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){
	DrawFrameDepth(m_pModel->GetImage_3D()->pModel->GetFrameRoot(), m_pEffect, FarNear, PintoPos, PintoWorld);
}

void C3DRender_D::DrawFrameDepth(LPD3DXFRAME pFrame, LPD3DXEFFECT effect, const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){
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

void C3DRender_D::DrawMeshContainerDepth(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase, LPD3DXEFFECT effect, const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){
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

void C3DRender_D::AnimationDrawDepth(D3DXMESHCONTAINER_DERIVED* pMeshContainer, D3DXFRAME_DERIVED* pFrame, LPD3DXEFFECT effect, const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){

	LPD3DXBONECOMBINATION pBoneComb;
	effect->SetTechnique("DepthSampling_Animation");
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
		effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
		effect->BeginPass(0);

		effect->SetVector("g_PintoPoint", &(static_cast<D3DXVECTOR4>(PintoPos)));
		effect->SetVector("g_FarNear", &(static_cast<D3DXVECTOR4>(FarNear)));
		effect->SetMatrix("g_PintoWorld", &PintoWorld);// �s���g�����킹��|�C���g���s��ϊ����邽�߂̃��[���h�s��
		effect->SetMatrix("g_Proj", &(SINSTANCE(CRenderContext)->GetCurrentCamera()->GetProj()));
		effect->SetMatrix("g_View", &(SINSTANCE(CRenderContext)->GetCurrentCamera()->GetView()));
		effect->SetMatrixArray("g_WorldMatrixArray", g_pBoneMatrices, pMeshContainer->NumPaletteEntries);

		// �{�[���̐�
		effect->SetFloat("g_numBone", pMeshContainer->NumInfl);

		effect->CommitChanges();
		pMeshContainer->MeshData.pMesh->DrawSubset(iattrib);
		effect->EndPass();
		effect->End();
	}

}

void C3DRender_D::NonAnimationDrawDepth(D3DXFRAME_DERIVED* pFrame, LPD3DXEFFECT effect, const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){

	D3DXMATRIX World;
	if (pFrame != nullptr){
		if (m_pModel->GetUseBorn()){
			World = pFrame->CombinedTransformationMatrix;
		}
		else{
			World = m_pModel->m_World;
		}
	}

	D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->pModel->GetContainer();

	effect->SetTechnique("DepthSampling_NonAnimation");
	effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	effect->BeginPass(0);


	// ���݂̃v���W�F�N�V�����s��ƃr���[�s����V�F�[�_�[�ɓ]��
	effect->SetVector("g_FarNear", &(static_cast<D3DXVECTOR4>(FarNear)));

	effect->SetVector("g_PintoPoint", &(static_cast<D3DXVECTOR4>(PintoPos)));
	effect->SetMatrix("g_Proj", &(SINSTANCE(CRenderContext)->GetCurrentCamera()->GetProj()));
	effect->SetMatrix("g_View", &(SINSTANCE(CRenderContext)->GetCurrentCamera()->GetView()));
	effect->SetMatrix("g_PintoWorld", &PintoWorld);// �s���g�����킹��|�C���g���s��ϊ����邽�߂̃��[���h�s��
	effect->SetMatrix("g_World", &World/*�ݒ肵�����s��ւ̃|�C���^*/);

	for (DWORD i = 0; i < container->NumMaterials; i++){
		effect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B
		container->MeshData.pMesh->DrawSubset(i);						// ���b�V����`��
	}
	effect->EndPass();
	effect->End();

}