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
	m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/ShadowTex.hlsl"));	// 使用するshaderファイルを指定(デフォルト)
}

void CShadowSamplingRender::Draw(){
	if (m_pModel->m_IsHorizon) {
		// 境界線以上の描画を省く場合はこちらを通る。
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
		// ボーンありの影の描画
		AnimationDraw(pMeshContainer);
	}
	else{
		// ボーンなしの影の描画
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

		// ボーンの数
		m_pEffect->SetFloat("g_numBone", pMeshContainer->NumInfl);

		if (m_pModel->m_IsHorizon) {
			// 上書きした頂点データを使用して描画するので、DrawSubset関数は使用できない。
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
	m_pEffect->SetMatrix("World"/*エフェクトファイル内の変数名*/, &(m_pModel->m_World)/*設定したい行列へのポインタ*/);
	D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->GetContainer();

	if (m_pModel->m_IsHorizon) {
		// 上書きした頂点データを使用して描画するので、DrawSubset関数は使用できない。
		this->DrawHorizon();
	}
	else {
		m_pEffect->CommitChanges();						//この関数を呼び出すことで、データの転送が確定する。
		for (DWORD i = 0; i < container->NumMaterials; i++) {
			container->MeshData.pMesh->DrawSubset(i);						// メッシュを描画
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
	//g_pMeshから頂点バッファ、インデックスバッファを引っ張ってきて、直接描画する。
	LPDIRECT3DVERTEXBUFFER9 vb;
	container->MeshData.pMesh->GetVertexBuffer(&vb);
	LPDIRECT3DINDEXBUFFER9 ib;
	container->MeshData.pMesh->GetIndexBuffer(&ib);

	m_pEffect->SetFloat("g_PlayerHorizon", SINSTANCE(CObjectManager)->FindGameObject<CGameObject>(_T("TEST3D"))->GetPos().y - 0.5f);

	// ストライドを取得。
	// ※ストライドは1頂点のサイズを示す。
	// ※FVFは本来、固定機能描画を使用する際に必要となるが、ここではストライドを求めるために使用する。
	DWORD fvf = container->MeshData.pMesh->GetFVF();
	DWORD stride = D3DXGetFVFVertexSize(fvf);

	(*graphicsDevice()).SetStreamSourceFreq(0,D3DSTREAMSOURCE_INDEXEDDATA | 1);
	(*graphicsDevice()).SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	(*graphicsDevice()).SetVertexDeclaration(m_VertexDecl_Override);
	(*graphicsDevice()).SetStreamSource(0, vb, 0, stride);
	(*graphicsDevice()).SetStreamSource(1, m_pHorizonBuffer, 0,sizeof(float));

	// インデックスバッファをセット。
	(*graphicsDevice()).SetIndices(ib);

	m_pEffect->CommitChanges();
	(*graphicsDevice()).DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, container->MeshData.pMesh->GetNumVertices(), 0, container->MeshData.pMesh->GetNumFaces());

	// ここでバッファーを削除しないとメモリリークが発生する。
	vb->Release();
	ib->Release();

	(*graphicsDevice()).SetStreamSourceFreq(0, 1);
	(*graphicsDevice()).SetStreamSourceFreq(1, 1);
}

void CShadowSamplingRender::ActivateHorizon() {
	// 頂点定義の上書きやバッファの定義は一度だけ行う。
	if (m_IsFirst) {
		// メッシュコンテナ取得。
		D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->GetContainer();
		// 現在の頂点宣言オブジェクトを取得(※頂点宣言オブジェクトとは、頂点データがどのような構成になっているかを宣言したものである)。
		D3DVERTEXELEMENT9 DeclElement[MAX_FVF_DECL_SIZE];
		container->MeshData.pMesh->GetDeclaration(DeclElement);
		// 頂点の宣言の終端を探索
		int ElementIdx = 0;
		while (true) {
			if (DeclElement[ElementIdx].Type == D3DDECLTYPE_UNUSED) {
				// 終端発見。
				// 頂点宣言を拡張。
				// ※stream要素はコースラインの高さ用のバッファを使用するため1番。
				// ※UsageIndex要素はTEXCOORD0は既に使用されているため1を使用する。
				DeclElement[ElementIdx] = { 1,0,D3DDECLTYPE_FLOAT1,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,1 };
				DeclElement[ElementIdx + 1] = D3DDECL_END();	// 頂点宣言の終端(番兵)を設定(正確には、頂点宣言の最後の要素を初期化している)。
				break;
			}
			ElementIdx++;
		}
		//上で宣言した頂点情報で新たに頂点定義を作成。
		(*graphicsDevice()).CreateVertexDeclaration(DeclElement, &m_VertexDecl_Override);
		// 境界線用のバッファー生成。
		(*graphicsDevice()).CreateVertexBuffer(sizeof(float) * container->MeshData.pMesh->GetNumVertices(), 0, 0, D3DPOOL_MANAGED, &m_pHorizonBuffer, 0);

		this->CopyBuffer();

		m_IsFirst = false;
	}
}