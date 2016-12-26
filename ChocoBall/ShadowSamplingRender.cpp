#include "stdafx.h"
#include "ShadowSamplingRender.h"
#include "Model.h"
#include "C3DImage.h"
#include "Camera.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Player.h"

CShadowSamplingRender::CShadowSamplingRender()
{
	m_pShadowCamera = nullptr;
	m_pHorizonBuffer = nullptr;
	m_VertexDecl_Override = nullptr;
	m_IsFirst = true;
}

CShadowSamplingRender::~CShadowSamplingRender()
{
	if (m_IsFirst) {
		SAFE_DELETE(m_VertexDecl_Override);
		SAFE_DELETE(m_pHorizonBuffer);
	}
}

void CShadowSamplingRender::Initialize(){
	m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/ShadowTex.hlsl"));	// �g�p����shader�t�@�C�����w��(�f�t�H���g)
}

void CShadowSamplingRender::Draw(){
	if (m_pModel->m_IsHorizon) {
		// ���E���ȏ�̕`����Ȃ��ꍇ�͂������ʂ�B
		this->ActivateHorizon();
	}
	DrawFrame(m_pModel->GetImage_3D()->GetFrameRoot());
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
	for (unsigned int iattrib = 0; iattrib < pMeshContainer->NumAttributeGroups; iattrib++) {
		for (DWORD iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry) {
			DWORD iMatrixIndex = pBoneComb[iattrib].BoneId[iPaletteEntry];
			if (iMatrixIndex != UINT_MAX) {
				D3DXMatrixMultiply(
					&m_pModel->m_pBoneMatrices[iPaletteEntry],
					&pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
					pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]
				);
			}
		}

		m_pEffect->SetTechnique(m_pTechniqueName);
		m_pEffect->Begin(NULL, 0);
		m_pEffect->BeginPass(0);

		m_pShadowCamera->SetCamera(m_pEffect);

		m_pEffect->SetMatrixArray("g_WorldMatrixArray", m_pModel->m_pBoneMatrices, pMeshContainer->NumPaletteEntries);

		// �{�[���̐�
		m_pEffect->SetFloat("g_numBone", pMeshContainer->NumInfl);

		if (m_pModel->m_IsHorizon) {
			// �㏑���������_�f�[�^���g�p���ĕ`�悷��̂ŁADrawSubset�֐��͎g�p�ł��Ȃ��B
			this->DrawHorizon();
		}
		else {
			m_pEffect->CommitChanges();
			pMeshContainer->MeshData.pMesh->DrawSubset(iattrib);
		}
	}
	m_pEffect->EndPass();
	m_pEffect->End();
}

void CShadowSamplingRender::NonAnimationDraw(){
	m_pEffect->SetTechnique(m_pTechniqueName);
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pShadowCamera->SetCamera(m_pEffect);
	m_pEffect->SetMatrix("World"/*�G�t�F�N�g�t�@�C�����̕ϐ���*/, &(m_pModel->m_World)/*�ݒ肵�����s��ւ̃|�C���^*/);
	D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->GetContainer();

	if (m_pModel->m_IsHorizon) {
		// �㏑���������_�f�[�^���g�p���ĕ`�悷��̂ŁADrawSubset�֐��͎g�p�ł��Ȃ��B
		this->DrawHorizon();
	}
	else {
		m_pEffect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B
		for (DWORD i = 0; i < container->NumMaterials; i++) {
			container->MeshData.pMesh->DrawSubset(i);						// ���b�V����`��
		}
	}

	m_pEffect->EndPass();
	m_pEffect->End();
}

void CShadowSamplingRender::CopyBuffer() {
	D3DVERTEXBUFFER_DESC desc;
	m_pHorizonBuffer->GetDesc(&desc);

	float* pWork;
	m_pHorizonBuffer->Lock(0, desc.Size, (void**)&pWork, D3DLOCK_DISCARD);
	for (int idx = 0; idx < m_pModel->GetImage_3D()->GetContainer()->MeshData.pMesh->GetNumVertices(); idx++) {
		if (m_pModel->m_IsVerticesNumHorizon) {
			*pWork = m_pModel->m_Horizon[idx];
		}
		else {
			*pWork = m_pModel->m_HorizonSingle;
		}
		pWork++;
	}
	m_pHorizonBuffer->Unlock();
}

void CShadowSamplingRender::DrawHorizon() {
	D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->GetContainer();
	//g_pMesh���璸�_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�����������Ă��āA���ڕ`�悷��B
	LPDIRECT3DVERTEXBUFFER9 vb;
	container->MeshData.pMesh->GetVertexBuffer(&vb);
	LPDIRECT3DINDEXBUFFER9 ib;
	container->MeshData.pMesh->GetIndexBuffer(&ib);

	m_pEffect->SetFloat("g_PlayerHorizon", SINSTANCE(CObjectManager)->FindGameObject<CGameObject>(_T("TEST3D"))->GetPos().y - 0.5f);

	// �X�g���C�h���擾�B
	// ���X�g���C�h��1���_�̃T�C�Y�������B
	// ��FVF�͖{���A�Œ�@�\�`����g�p����ۂɕK�v�ƂȂ邪�A�����ł̓X�g���C�h�����߂邽�߂Ɏg�p����B
	DWORD fvf = container->MeshData.pMesh->GetFVF();
	DWORD stride = D3DXGetFVFVertexSize(fvf);

	(*graphicsDevice()).SetStreamSourceFreq(0,D3DSTREAMSOURCE_INDEXEDDATA | 1);
	(*graphicsDevice()).SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	(*graphicsDevice()).SetVertexDeclaration(m_VertexDecl_Override);
	(*graphicsDevice()).SetStreamSource(0, vb, 0, stride);
	(*graphicsDevice()).SetStreamSource(1, m_pHorizonBuffer, 0,sizeof(float));

	// �C���f�b�N�X�o�b�t�@���Z�b�g�B
	(*graphicsDevice()).SetIndices(ib);

	m_pEffect->CommitChanges();
	(*graphicsDevice()).DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, container->MeshData.pMesh->GetNumVertices(), 0, container->MeshData.pMesh->GetNumFaces());

	// �����Ńo�b�t�@�[���폜���Ȃ��ƃ��������[�N����������B
	vb->Release();
	ib->Release();

	(*graphicsDevice()).SetStreamSourceFreq(0, 1);
	(*graphicsDevice()).SetStreamSourceFreq(1, 1);
}

void CShadowSamplingRender::ActivateHorizon() {
	// ���_��`�̏㏑����o�b�t�@�̒�`�͈�x�����s���B
	if (m_IsFirst) {
		// ���b�V���R���e�i�擾�B
		D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->GetContainer();
		// ���݂̒��_�錾�I�u�W�F�N�g���擾(�����_�錾�I�u�W�F�N�g�Ƃ́A���_�f�[�^���ǂ̂悤�ȍ\���ɂȂ��Ă��邩��錾�������̂ł���)�B
		D3DVERTEXELEMENT9 DeclElement[MAX_FVF_DECL_SIZE];
		container->MeshData.pMesh->GetDeclaration(DeclElement);
		// ���_�̐錾�̏I�[��T��
		int ElementIdx = 0;
		while (true) {
			if (DeclElement[ElementIdx].Type == D3DDECLTYPE_UNUSED) {
				// �I�[�����B
				// ���_�錾���g���B
				// ��stream�v�f�̓R�[�X���C���̍����p�̃o�b�t�@���g�p���邽��1�ԁB
				// ��UsageIndex�v�f��TEXCOORD0�͊��Ɏg�p����Ă��邽��1���g�p����B
				DeclElement[ElementIdx] = { 1,0,D3DDECLTYPE_FLOAT1,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,1 };
				DeclElement[ElementIdx + 1] = D3DDECL_END();	// ���_�錾�̏I�[(�ԕ�)��ݒ�(���m�ɂ́A���_�錾�̍Ō�̗v�f�����������Ă���)�B
				break;
			}
			ElementIdx++;
		}
		//��Ő錾�������_���ŐV���ɒ��_��`���쐬�B
		(*graphicsDevice()).CreateVertexDeclaration(DeclElement, &m_VertexDecl_Override);
		// ���E���p�̃o�b�t�@�[�����B
		(*graphicsDevice()).CreateVertexBuffer(sizeof(float) * container->MeshData.pMesh->GetNumVertices(), 0, 0, D3DPOOL_MANAGED, &m_pHorizonBuffer, 0);

		this->CopyBuffer();

		m_IsFirst = false;
	}
}