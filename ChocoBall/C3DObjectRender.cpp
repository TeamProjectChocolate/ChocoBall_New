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
	m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/ModelShader.hlsl"));	// 使用するshaderファイルを指定(デフォルト)
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
		// スキン情報あり
		AnimationDraw(pMeshContainer, pFrame);
	}
	else{
		// スキン情報なし
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

		// ボーンの数
		m_pEffect->SetFloat(m_hnumBone, pMeshContainer->NumInfl);
		m_pEffect->SetTexture(m_hTexture, pMeshContainer->ppTextures[pBoneComb[iattrib].AttribId]);

		// 深度を書き込むのに必要
		m_pEffect->SetVector("g_PintoPoint", &(static_cast<D3DXVECTOR4>(m_pModel->GetPintoPos())));
		m_pEffect->SetVector("g_DepthFarNear", &(static_cast<D3DXVECTOR4>(m_DepthFarNear)));
		m_pEffect->SetMatrix("g_PintoWorld", &m_pModel->GetPintoWorld());// ピントを合わせるポイントを行列変換するためのワールド行列

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
	m_pEffect->Begin(&numPass/*テクニック内に定義されているパスの数が返却される*/, 0);
	m_pEffect->BeginPass(0);	//パスの番号を指定してどのパスを使用するか指定

	// 透明度有効化
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 現在のプロジェクション行列とビュー行列をシェーダーに転送
	SINSTANCE(CRenderContext)->GetCurrentCamera()->SetCamera(m_pEffect);
	SINSTANCE(CRenderContext)->GetCurrentLight()->SetLight(m_pEffect);
	// 視点をシェーダーに転送
	m_pEffect->SetVector(m_hEyePosition, reinterpret_cast<D3DXVECTOR4*>(&SINSTANCE(CRenderContext)->GetCurrentCamera()->GetPos()));


	SINSTANCE(CShadowRender)->SetShadowCamera(m_pEffect);

	// ワールドトランスフォーム(絶対座標変換)
	// ワールド行列生成


	m_pEffect->SetMatrix(m_hRota, &(m_pModel->GetRotation()));
	m_pEffect->SetMatrix(m_hWorld/*エフェクトファイル内の変数名*/, &World/*設定したい行列へのポインタ*/);

	m_pEffect->SetFloat(m_hAlpha, m_pModel->m_alpha);
	m_pEffect->SetFloat(m_hluminance, m_pModel->m_luminance);

	// 深度を書き込むのに必要
	m_pEffect->SetVector("g_PintoPoint", &(static_cast<D3DXVECTOR4>(m_pModel->GetPintoPos())));
	m_pEffect->SetVector("g_DepthFarNear", &(static_cast<D3DXVECTOR4>(m_DepthFarNear)));
	m_pEffect->SetMatrix("g_PintoWorld", &m_pModel->GetPintoWorld());// ピントを合わせるポイントを行列変換するためのワールド行列


	for (DWORD i = 0; i < container->NumMaterials; i++){
		m_pEffect->SetTexture(m_hShadowMap, SINSTANCE(CShadowRender)->GetTexture());	// テクスチャ情報をセット
		m_pEffect->SetTexture(m_hTexture, container->ppTextures[i]);	// テクスチャ情報をセット
		//m_pEffect->SetTexture("g_normalMap", SINSTANCE(CImageManager)->Find2DImage(_T("image/normal.jpg"))->pTex);
		m_pEffect->CommitChanges();						//この関数を呼び出すことで、データの転送が確定する。
		container->MeshData.pMesh->DrawSubset(i);						// メッシュを描画
	}
	m_pEffect->EndPass();
	m_pEffect->End();
}
