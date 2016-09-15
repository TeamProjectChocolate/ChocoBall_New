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
		m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/DepthOfField.hlsl"));	// 使用するshaderファイルを指定(デフォルト)
		//インスタンシング描画用の初期化。
		D3DVERTEXELEMENT9 declElement[MAX_FVF_DECL_SIZE];
		D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->pModel->GetContainer();
		// 現在の頂点宣言オブジェクトを取得(※頂点宣言オブジェクトとは、頂点データがどのような構成になっているかを宣言したものである)
		container->MeshData.pMesh->GetDeclaration(declElement);
		// 頂点の宣言の終端を探索
		int elementIndex = 0;
		while (true) {
			if (declElement[elementIndex].Type == D3DDECLTYPE_UNUSED) {
				//終端を発見。
				//ここからインスタンシング用の頂点レイアウトを埋め込む。
				WORD offset = 0;
				for (short num = 0; num < 4; num++){
					// WORLD行列はfloat4x4なので、1行ずつfloat4で定義していく
					declElement[elementIndex] = {
						1,	// ストリーム番号
						offset,	/*offset要素(各ストリームの先頭から何番目に定義されているかを表す数字[sizeof(型名) * 数字]の書き方)*/
						D3DDECLTYPE_FLOAT4,	// 頂点情報をどの肩を使用して宣言するか(行列はfloat4の要素を四つ使用して構成する)
						D3DDECLMETHOD_DEFAULT, // テッセレーション(ポリゴン分割)の方法を指定。普通テッセレーションはあまり使わないため、デフォルト設(D3DDECLMETHOD_DEFAULT)で十分
						D3DDECLUSAGE_TEXCOORD,	// usage要素
						num + 1/*UsageIndex要素(Usageが重複しているものについて、固有番号を振って識別するもの)*/
					};
					offset += sizeof(float) * 4;
					elementIndex++;
				}
				declElement[elementIndex] = D3DDECL_END();	// 頂点宣言の終端(番兵)を設定(正確には、頂点宣言の最後の要素を初期化している)
				break;
			}
			elementIndex++;
		}
		//上で宣言した頂点情報で頂点定義を作成。
		(*graphicsDevice()).CreateVertexDeclaration(declElement, &m_VertexDecl);
		m_IsFirst = false;
	}
}

void CInstancingRender_D::CreateMatrixBuffer(int MaxNum){
	//ワールド行列用のバッファの作成。
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
		// スキン情報あり
		AnimationDrawDepth(pMeshContainer, pFrame, effect, FarNear, PintoPos, PintoWorld);
	}
	else{
		// スキン情報なし
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
	//	effect->SetMatrix("g_PintoWorld", &PintoWorld);// ピントを合わせるポイントを行列変換するためのワールド行列
	//	effect->SetMatrix("g_Proj", &(SINSTANCE(CRenderContext)->GetCurrentCamera()->GetProj()));
	//	effect->SetMatrix("g_View", &(SINSTANCE(CRenderContext)->GetCurrentCamera()->GetView()));
	//	effect->SetMatrixArray("g_WorldMatrixArray", g_pBoneMatrices, pMeshContainer->NumPaletteEntries);

	//	// ボーンの数
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
	m_pEffect->BeginPass(0);	//パスの番号を指定してどのパスを使用するか指定

	// 現在のプロジェクション行列とビュー行列をシェーダーに転送
	effect->SetMatrix("g_Proj", &(SINSTANCE(CRenderContext)->GetCurrentCamera()->GetProj()));
	effect->SetMatrix("g_View", &(SINSTANCE(CRenderContext)->GetCurrentCamera()->GetView()));

	effect->SetVector("g_FarNear", &(static_cast<D3DXVECTOR4>(FarNear)));
	effect->SetVector("g_PintoPoint", &(static_cast<D3DXVECTOR4>(PintoPos)));
	effect->SetMatrix("g_PintoWorld", &PintoWorld);// ピントを合わせるポイントを行列変換するためのワールド行列

	// インスタンシング描画
	{
		//DrawSubsetを使用するとインスタンシング描画が行えないので
		//g_pMeshから頂点バッファ、インデックスバッファを引っ張ってきて、直接描画する。
		LPDIRECT3DVERTEXBUFFER9 vb;
		LPDIRECT3DINDEXBUFFER9 ib;
		container->MeshData.pMesh->GetVertexBuffer(&vb);
		container->MeshData.pMesh->GetIndexBuffer(&ib);

		DWORD fvf = container->MeshData.pMesh->GetFVF();
		DWORD stride = D3DXGetFVFVertexSize(fvf);

		(*graphicsDevice()).SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_worldMatrix.size());
		(*graphicsDevice()).SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

		(*graphicsDevice()).SetVertexDeclaration(m_VertexDecl);

		(*graphicsDevice()).SetStreamSource(0, vb, 0, stride);							//頂点バッファをストリーム0番目に設定
		//ワールド行列を頂点バッファにコピー。
		CopyMatrixToVertexBuffer();
		(*graphicsDevice()).SetStreamSource(1, m_WorldMatrixBuffer, 0, sizeof(D3DXMATRIX));	//ワールド行列用のバッファをストリーム1番目に設定。
		(*graphicsDevice()).SetIndices(ib);
		m_pEffect->CommitChanges();						//この関数を呼び出すことで、データの転送が確定する。描画を行う前に一回だけ呼び出す。
		(*graphicsDevice()).DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, container->MeshData.pMesh->GetNumVertices(), 0, container->MeshData.pMesh->GetNumFaces());
		vb->Release();
		ib->Release();
	}

	m_pEffect->EndPass();
	m_pEffect->End();

	// 使い終わったワールド行列群を破棄
	m_worldMatrix.clear();

	m_pModel = nullptr;
}