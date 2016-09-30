#include "stdafx.h"
#include "C3DObjectRender_S.h"
#include "Effect.h"
#include "ShadowRender.h"


C3DObjectRender_S::C3DObjectRender_S()
{
}


C3DObjectRender_S::~C3DObjectRender_S()
{
}

void C3DObjectRender_S::Initialize(){
	m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/ModelShader.hlsl"));	// 使用するshaderファイルを指定(デフォルト)
	m_hEyePosition = m_pEffect->GetParameterByName(nullptr, "g_EyePosition");
	m_hWorldMatrixArray = m_pEffect->GetParameterByName(nullptr, "g_WorldMatrixArray");
	m_hluminance = m_pEffect->GetParameterByName(nullptr, "g_luminance");
	m_hnumBone = m_pEffect->GetParameterByName(nullptr, "g_numBone");
	m_hAlpha = m_pEffect->GetParameterByName(nullptr, "Alpha");
	m_hRota = m_pEffect->GetParameterByName(nullptr, "Rota");
	m_hWorld = m_pEffect->GetParameterByName(nullptr, "World");
	m_hTexture = m_pEffect->GetParameterByName(nullptr, "g_Texture");
	m_hShadowMap = m_pEffect->GetParameterByName(nullptr, "g_ShadowMap");
}

void C3DObjectRender_S::Draw()
{
	DrawFrameSimple(m_pModel->GetImage_3D()->pModel->GetFrameRoot());
}

void C3DObjectRender_S::DrawFrameSimple(LPD3DXFRAME pFrame){
	LPD3DXMESHCONTAINER pMeshContainer;

	pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != nullptr){
		DrawMeshContainerSimple(pMeshContainer, pFrame);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != nullptr){
		DrawFrameSimple(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != nullptr){
		DrawFrameSimple(pFrame->pFrameFirstChild);
	}
}

void C3DObjectRender_S::DrawMeshContainerSimple(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase){
	// スキン情報なし
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	NonAnimationDrawSimple(pFrame);
}

void C3DObjectRender_S::NonAnimationDrawSimple(D3DXFRAME_DERIVED* pFrame){

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




	//ここで固定描画と同じように、ローカル座標に設定された頂点群をデバイスに渡す。通常と同じ方法。
	//	メッシュも同じく、マテリアルやテクスチャを設定
	//DrawSubset()を呼び出して描画

	// ワールドトランスフォーム(絶対座標変換)
	// ワールド行列生成


	m_pEffect->SetMatrix(m_hRota, &(m_pModel->GetRotation()));
	m_pEffect->SetMatrix(m_hWorld/*エフェクトファイル内の変数名*/, &World/*設定したい行列へのポインタ*/);

	// 視点をシェーダーに転送
	m_pEffect->SetVector(m_hEyePosition, reinterpret_cast<D3DXVECTOR4*>(&SINSTANCE(CRenderContext)->GetCurrentCamera()->GetPos()));
	m_pEffect->SetVector("g_EyeDir", reinterpret_cast<D3DXVECTOR4*>(&SINSTANCE(CRenderContext)->GetCurrentCamera()->GetDir()));

	m_pEffect->SetFloat(m_hAlpha, m_pModel->m_alpha);
	m_pEffect->SetFloat(m_hluminance, m_pModel->m_luminance);
	m_pEffect->SetFloat("g_Refractive", m_pModel->m_Refractive);

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
}
