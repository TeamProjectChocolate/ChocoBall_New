#include "stdafx.h"
#include "C3DObjectRender.h"
#include "Effect.h"
#include "RenderContext.h"
#include "GraphicsDevice.h"
#include "ShadowRender.h"

extern UINT                        g_NumBoneMatricesMax;
extern D3DXMATRIXA16*              g_pBoneMatrices;

C3DObjectRender::C3DObjectRender()
{
	m_UseBorn = false;
}


C3DObjectRender::~C3DObjectRender()
{

}

void C3DObjectRender::Initialize(){
	m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/ModelShader.hlsl"));	// �g�p����shader�t�@�C�����w��(�f�t�H���g)
	m_hEyePosition = m_pEffect->GetParameterByName(nullptr, "EyePosition");
	m_hWorldMatrixArray = m_pEffect->GetParameterByName(nullptr, "g_WorldMatrixArray");
	m_hluminance = m_pEffect->GetParameterByName(nullptr, "g_luminance");
	m_hnumBone = m_pEffect->GetParameterByName(nullptr, "g_numBone");
	m_hAlpha = m_pEffect->GetParameterByName(nullptr, "Alpha");
	m_hRota = m_pEffect->GetParameterByName(nullptr, "Rota");
	m_hWorld = m_pEffect->GetParameterByName(nullptr, "World");
	m_hTexture = m_pEffect->GetParameterByName(nullptr, "g_Texture");
	m_hShadowMap = m_pEffect->GetParameterByName(nullptr, "g_ShadowMap");
}

void C3DObjectRender::Draw(){
	DrawFrame(m_pModel->GetImage_3D()->pModel->GetFrameRoot());
}

void C3DObjectRender::DrawFrame(LPD3DXFRAME pFrame){
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

void C3DObjectRender::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase){
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

void C3DObjectRender::AnimationDraw(D3DXMESHCONTAINER_DERIVED* pMeshContainer, D3DXFRAME_DERIVED* pFrame){

	LPD3DXBONECOMBINATION pBoneComb;
	SetUpTechniqueAnimation();
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
		m_pEffect->Begin(0, D3DXFX_DONOTSAVESTATE);
		m_pEffect->BeginPass(0);

		SINSTANCE(CRenderContext)->GetCurrentCamera()->SetCamera(m_pEffect);
		SINSTANCE(CRenderContext)->GetCurrentLight()->SetLight(m_pEffect);
		m_pEffect->SetMatrixArray(m_hWorldMatrixArray, g_pBoneMatrices, pMeshContainer->NumPaletteEntries);

		SINSTANCE(CShadowRender)->SetShadowCamera(m_pEffect);
		m_pEffect->SetFloat(m_hluminance, m_pModel->m_luminance);

		// �{�[���̐�
		m_pEffect->SetFloat(m_hnumBone, pMeshContainer->NumInfl);
		m_pEffect->SetTexture(m_hTexture, pMeshContainer->ppTextures[pBoneComb[iattrib].AttribId]);

		// �[�x���������ނ̂ɕK�v
		m_pEffect->SetVector("g_PintoPoint", &(static_cast<D3DXVECTOR4>(m_pModel->GetPintoPos())));
		m_pEffect->SetVector("g_DepthFarNear", &(static_cast<D3DXVECTOR4>(m_DepthFarNear)));
		m_pEffect->SetMatrix("g_PintoWorld", &m_pModel->GetPintoWorld());// �s���g�����킹��|�C���g���s��ϊ����邽�߂̃��[���h�s��

		m_pEffect->CommitChanges();
		pMeshContainer->MeshData.pMesh->DrawSubset(iattrib);
		m_pEffect->EndPass();
		m_pEffect->End();
	}
}


void C3DObjectRender::NonAnimationDraw(D3DXFRAME_DERIVED* pFrame){

	D3DXMATRIX World;
	if (pFrame != nullptr){
		if (m_UseBorn){
			World = pFrame->CombinedTransformationMatrix;
		}
		else{
			World = m_pModel->m_World;
		}
	}

	D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->pModel->GetContainer();
	if (container->ppTextures == nullptr){
		m_pEffect->SetTechnique("NotNormalMapBasicTec");
	}

	m_pEffect->SetTechnique(m_pTechniqueName);

	UINT numPass;
	m_pEffect->Begin(&numPass/*�e�N�j�b�N���ɒ�`����Ă���p�X�̐����ԋp�����*/, 0);
	m_pEffect->BeginPass(0);	//�p�X�̔ԍ����w�肵�Ăǂ̃p�X���g�p���邩�w��

	// �����x�L����
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���݂̃v���W�F�N�V�����s��ƃr���[�s����V�F�[�_�[�ɓ]��
	SINSTANCE(CRenderContext)->GetCurrentCamera()->SetCamera(m_pEffect);
	SINSTANCE(CRenderContext)->GetCurrentLight()->SetLight(m_pEffect);
	// ���_���V�F�[�_�[�ɓ]��
	m_pEffect->SetVector(m_hEyePosition, reinterpret_cast<D3DXVECTOR4*>(&SINSTANCE(CRenderContext)->GetCurrentCamera()->GetPos()));


	SINSTANCE(CShadowRender)->SetShadowCamera(m_pEffect);

	// ���[���h�g�����X�t�H�[��(��΍��W�ϊ�)
	// ���[���h�s�񐶐�


	m_pEffect->SetMatrix(m_hRota, &(m_pModel->GetRotation()));
	m_pEffect->SetMatrix(m_hWorld/*�G�t�F�N�g�t�@�C�����̕ϐ���*/, &World/*�ݒ肵�����s��ւ̃|�C���^*/);

	m_pEffect->SetFloat(m_hAlpha, m_pModel->m_alpha);
	m_pEffect->SetFloat(m_hluminance, m_pModel->m_luminance);

	// �[�x���������ނ̂ɕK�v
	m_pEffect->SetVector("g_PintoPoint", &(static_cast<D3DXVECTOR4>(m_pModel->GetPintoPos())));
	m_pEffect->SetVector("g_DepthFarNear", &(static_cast<D3DXVECTOR4>(m_DepthFarNear)));
	m_pEffect->SetMatrix("g_PintoWorld", &m_pModel->GetPintoWorld());// �s���g�����킹��|�C���g���s��ϊ����邽�߂̃��[���h�s��


	for (DWORD i = 0; i < container->NumMaterials; i++){
		m_pEffect->SetTexture(m_hShadowMap, SINSTANCE(CShadowRender)->GetTexture());	// �e�N�X�`�������Z�b�g
		m_pEffect->SetTexture(m_hTexture, container->ppTextures[i]);	// �e�N�X�`�������Z�b�g
		//m_pEffect->SetTexture("g_normalMap", SINSTANCE(CImageManager)->Find2DImage(_T("image/normal.jpg"))->pTex);
		m_pEffect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B
		container->MeshData.pMesh->DrawSubset(i);						// ���b�V����`��
	}
	m_pEffect->EndPass();
	m_pEffect->End();
}
