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

	//インスタンシング描画用の初期化。
	D3DVERTEXELEMENT9 declElement[MAX_FVF_DECL_SIZE];
	ANIMATION::D3DXMESHCONTAINER_DERIVED* container = m_pModel->GetImage_3D()->GetContainer();
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
					static_cast<BYTE>(num + 1) /*UsageIndex要素(Usageが重複しているものについて、固有番号を振って識別するもの)*/
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
			declElement[elementIndex] = D3DDECL_END();	// 頂点宣言の終端(番兵)を設定(正確には、頂点宣言の最後の要素を初期化している)
			break;
		}
		elementIndex++;
	}
	//上で宣言した頂点情報で頂点定義を作成。
	(*graphicsDevice()).CreateVertexDeclaration(declElement, &m_VertexDecl);
	m_IsFirst = false;
}

void CEM_SamplingRender_I::CreateMatrixBuffer(unsigned int MaxNum){
	//ワールド行列用のバッファの作成。
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
			// 描画するオブジェクトが一つも存在しなければ描画しない。これをしないと必ず一つ描画されてしまう。
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
		// スキン情報あり
		AnimationDraw(pMeshContainer, pFrame);
	}
	else{
		// スキン情報なし
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

	//	// 視点をシェーダーに転送
	//	m_pEffect->SetVector(m_hEyePosition, &static_cast<D3DXVECTOR4>(m_pCamera->GetPos()));
	//	m_pEffect->SetVector("g_EyeDir", &static_cast<D3DXVECTOR4>(m_pCamera->GetDir()));

	//	SINSTANCE(CShadowRender)->SetShadowCamera(m_pEffect);

	//	m_pEffect->SetFloat(m_hAlpha, m_pModel->m_alpha);
	//	m_pEffect->SetFloat(m_hluminance, m_pModel->m_luminance);

	//	// ボーンの数
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
	m_pEffect->Begin(&numPass/*テクニック内に定義されているパスの数が返却される*/, 0);
	m_pEffect->BeginPass(0);	//パスの番号を指定してどのパスを使用するか指定

	// 現在のプロジェクション行列とビュー行列をシェーダーに転送
	m_pCamera->SetCamera(m_pEffect);
	SINSTANCE(CRenderContext)->GetCurrentLight()->SetLight(m_pEffect);
	// 視点をシェーダーに転送
	m_pEffect->SetVector(m_hEyePosition, &static_cast<D3DXVECTOR4>(m_pCamera->GetPos()));
	m_pEffect->SetVector("g_EyeDir", &static_cast<D3DXVECTOR4>(m_pCamera->GetDir()));

	SINSTANCE(CShadowRender)->SetShadowCamera(m_pEffect);

	m_pEffect->SetFloat(m_hAlpha, m_pModel->m_alpha);
	m_pEffect->SetFloat(m_hluminance, m_pModel->m_luminance);
	m_pEffect->SetTexture(m_hTexture, container->ppTextures[0]);	// テクスチャ情報をセット


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
		(*graphicsDevice()).SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);

		(*graphicsDevice()).SetVertexDeclaration(m_VertexDecl);

		(*graphicsDevice()).SetStreamSource(0, vb, 0, stride);							//頂点バッファをストリーム0番目に設定
		(*graphicsDevice()).SetStreamSource(1, m_WorldMatrixBuffer, 0, sizeof(D3DXMATRIX));	//ワールド行列用のバッファをストリーム1番目に設定。
		(*graphicsDevice()).SetStreamSource(2, m_RotationMatrixBuffer, 0, sizeof(D3DXMATRIX));
		//ワールド行列を頂点バッファにコピー。
		CopyMatrixToVertexBuffer();
		(*graphicsDevice()).SetIndices(ib);
		m_pEffect->CommitChanges();						//この関数を呼び出すことで、データの転送が確定する。描画を行う前に一回だけ呼び出す。

		(*graphicsDevice()).DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, container->MeshData.pMesh->GetNumVertices(), 0, container->MeshData.pMesh->GetNumFaces());
		vb->Release();
		ib->Release();
	}

	m_pEffect->EndPass();
	m_pEffect->End();

	// 使い終わった行列群を破棄
	m_worldMatrix.clear();
	m_RotationMatrix.clear();

	m_pModel = nullptr;
}
