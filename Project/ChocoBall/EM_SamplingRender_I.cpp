#include "stdafx.h"
#include "EM_SamplingRender_I.h"
#include "C3DImage.h"
#include "Effect.h"
#include "ShadowRender.h"


CEM_SamplingRender_I::CEM_SamplingRender_I()
{
	m_worldMatrix.clear();
	m_RotationMatrix.clear();
	m_IsFirst = true;
	m_MaxNum = 0;
}


CEM_SamplingRender_I::~CEM_SamplingRender_I()
{
	m_worldMatrix.clear();
	m_RotationMatrix.clear();

	m_WorldMatrixBuffer->Release();
	m_RotationMatrixBuffer->Release();
	m_VertexDecl->Release();
	//SAFE_DELETE(m_WorldMatrixBuffer);
	//SAFE_DELETE(m_RotationMatrixBuffer);
	//SAFE_DELETE(m_VertexDecl);
}

void CEM_SamplingRender_I::CopyMatrixToVertexBuffer()
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

	D3DVERTEXBUFFER_DESC desc2;
	m_RotationMatrixBuffer->GetDesc(&desc2);
	D3DXMATRIX* pData2;
	m_RotationMatrixBuffer->Lock(0, desc2.Size, (void**)&pData2, D3DLOCK_DISCARD);

	for (int i = 0; i < m_RotationMatrix.size(); i++) {
		*pData2 = m_RotationMatrix[i];
		pData2++;
	}
	m_RotationMatrixBuffer->Unlock();

}

void CEM_SamplingRender_I::Initialize(){
	m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/ModelShader.hlsl"));
	m_hEyePosition = m_pEffect->GetParameterByName(nullptr, "g_EyePosition");
	m_hWorldMatrixArray = m_pEffect->GetParameterByName(nullptr, "g_WorldMatrixArray");
	m_hluminance = m_pEffect->GetParameterByName(nullptr, "g_luminance");
	m_hnumBone = m_pEffect->GetParameterByName(nullptr, "g_numBone");
	m_hAlpha = m_pEffect->GetParameterByName(nullptr, "Alpha");
	m_hRota = m_pEffect->GetParameterByName(nullptr, "Rota");
	m_hWorld = m_pEffect->GetParameterByName(nullptr, "World");
	m_hTexture = m_pEffect->GetParameterByName(nullptr, "g_Texture");
	m_hShadowMap = m_pEffect->GetParameterByName(nullptr, "g_ShadowMap");

	//�C���X�^���V���O�`��p�̏������B
	D3DVERTEXELEMENT9 declElement[MAX_FVF_DECL_SIZE];
	ANIMATION::D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->GetContainer();
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
					static_cast<BYTE>(num + 1) /*UsageIndex�v�f(Usage���d�����Ă�����̂ɂ��āA�ŗL�ԍ���U���Ď��ʂ������)*/
				};
				offset += sizeof(float)* 4;
				elementIndex++;
			}
			WORD offset2 = 0;
			for (short num = 0; num < 4; num++){
				declElement[elementIndex] = {
					2,
					offset2,
					D3DDECLTYPE_FLOAT4,
					D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_TEXCOORD,
					static_cast<BYTE>(4 + num + 1)
				};
				offset2 += sizeof(float)* 4;
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

void CEM_SamplingRender_I::CreateMatrixBuffer(unsigned int MaxNum){
	//���[���h�s��p�̃o�b�t�@�̍쐬�B
	if (m_MaxNum < MaxNum){
		(*graphicsDevice()).CreateVertexBuffer(sizeof(D3DXMATRIX)* MaxNum, 0, 0, D3DPOOL_MANAGED, &m_WorldMatrixBuffer, 0);
		(*graphicsDevice()).CreateVertexBuffer(sizeof(D3DXMATRIX)* MaxNum, 0, 0, D3DPOOL_MANAGED, &m_RotationMatrixBuffer, 0);
		m_MaxNum = MaxNum;
	}
}

void CEM_SamplingRender_I::AddWorldMatrix(const D3DXMATRIX& mat){
	m_worldMatrix.push_back(mat);
}

void CEM_SamplingRender_I::AddRotationMatrix(const D3DXMATRIX& mat){
	m_RotationMatrix.push_back(mat);
}

void CEM_SamplingRender_I::Draw(){
	if (m_pModel){
		if (m_worldMatrix.size() <= 0){
			// �`�悷��I�u�W�F�N�g��������݂��Ȃ���Ε`�悵�Ȃ��B��������Ȃ��ƕK����`�悳��Ă��܂��B
			return;
		}
		DrawFrame(m_pModel->GetImage_3D()->GetFrameRoot());
	}
}

void CEM_SamplingRender_I::DrawFrame(LPD3DXFRAME pFrame){
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

void CEM_SamplingRender_I::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase){
	ANIMATION::D3DXFRAME_DERIVED* pFrame = (ANIMATION::D3DXFRAME_DERIVED*)pFrameBase;
	ANIMATION::D3DXMESHCONTAINER_DERIVED* pMeshContainer = static_cast<ANIMATION::D3DXMESHCONTAINER_DERIVED*>(pMeshContainerBase);

	if (pMeshContainer->pSkinInfo != nullptr){
		// �X�L����񂠂�
		AnimationDraw(pMeshContainer, pFrame);
	}
	else{
		// �X�L�����Ȃ�
		NonAnimationDraw(pFrame);
	}
}

void CEM_SamplingRender_I::AnimationDraw(ANIMATION::D3DXMESHCONTAINER_DERIVED* pMeshContainer, ANIMATION::D3DXFRAME_DERIVED* pFrame){

	//LPD3DXBONECOMBINATION pBoneComb;
	//m_pEffect->SetTechnique(m_pTechniqueName);
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
	//	m_pEffect->Begin(0, D3DXFX_DONOTSAVESTATE);
	//	m_pEffect->BeginPass(0);

	//	m_pCamera->SetCamera(m_pEffect);
	//	SINSTANCE(CRenderContext)->GetCurrentLight()->SetLight(m_pEffect);
	//	m_pEffect->SetMatrixArray(m_hWorldMatrixArray, g_pBoneMatrices, pMeshContainer->NumPaletteEntries);

	//	// ���_���V�F�[�_�[�ɓ]��
	//	m_pEffect->SetVector(m_hEyePosition, &static_cast<D3DXVECTOR4>(m_pCamera->GetPos()));
	//	m_pEffect->SetVector("g_EyeDir", &static_cast<D3DXVECTOR4>(m_pCamera->GetDir()));

	//	SINSTANCE(CShadowRender)->SetShadowCamera(m_pEffect);

	//	m_pEffect->SetFloat(m_hAlpha, m_pModel->m_alpha);
	//	m_pEffect->SetFloat(m_hluminance, m_pModel->m_luminance);

	//	// �{�[���̐�
	//	m_pEffect->SetFloat(m_hnumBone, pMeshContainer->NumInfl);
	//	m_pEffect->SetTexture(m_hTexture, pMeshContainer->ppTextures[pBoneComb[iattrib].AttribId]);

	//	m_pEffect->CommitChanges();
	//	pMeshContainer->MeshData.pMesh->DrawSubset(iattrib);
	//	m_pEffect->EndPass();
	//	m_pEffect->End();
	//}
}


void CEM_SamplingRender_I::NonAnimationDraw(ANIMATION::D3DXFRAME_DERIVED* pFrame){

	ANIMATION::D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->GetContainer();

	m_pEffect->SetTechnique(m_pTechniqueName);

	UINT numPass;
	m_pEffect->Begin(&numPass/*�e�N�j�b�N���ɒ�`����Ă���p�X�̐����ԋp�����*/, 0);
	m_pEffect->BeginPass(0);	//�p�X�̔ԍ����w�肵�Ăǂ̃p�X���g�p���邩�w��

	// ���݂̃v���W�F�N�V�����s��ƃr���[�s����V�F�[�_�[�ɓ]��
	m_pCamera->SetCamera(m_pEffect);
	SINSTANCE(CRenderContext)->GetCurrentLight()->SetLight(m_pEffect);
	// ���_���V�F�[�_�[�ɓ]��
	m_pEffect->SetVector(m_hEyePosition, &static_cast<D3DXVECTOR4>(m_pCamera->GetPos()));
	m_pEffect->SetVector("g_EyeDir", &static_cast<D3DXVECTOR4>(m_pCamera->GetDir()));

	SINSTANCE(CShadowRender)->SetShadowCamera(m_pEffect);

	m_pEffect->SetFloat(m_hAlpha, m_pModel->m_alpha);
	m_pEffect->SetFloat(m_hluminance, m_pModel->m_luminance);
	m_pEffect->SetTexture(m_hTexture, container->ppTextures[0]);	// �e�N�X�`�������Z�b�g


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
		(*graphicsDevice()).SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);

		(*graphicsDevice()).SetVertexDeclaration(m_VertexDecl);

		(*graphicsDevice()).SetStreamSource(0, vb, 0, stride);							//���_�o�b�t�@���X�g���[��0�Ԗڂɐݒ�
		(*graphicsDevice()).SetStreamSource(1, m_WorldMatrixBuffer, 0, sizeof(D3DXMATRIX));	//���[���h�s��p�̃o�b�t�@���X�g���[��1�Ԗڂɐݒ�B
		(*graphicsDevice()).SetStreamSource(2, m_RotationMatrixBuffer, 0, sizeof(D3DXMATRIX));
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

	// �g���I������s��Q��j��
	m_worldMatrix.clear();
	m_RotationMatrix.clear();

	m_pModel = nullptr;
}
