#include "stdafx.h"
#include "SkinModelData.h"
#include "GraphicsDevice.h"
#include "AllocateHierarchy.h"
#include "Assert.h"

HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName);

CSkinModelData::CSkinModelData()
{
	m_isClone = false;
	m_pAnimController = nullptr;
	m_pMeshContainer = nullptr;
}


CSkinModelData::~CSkinModelData()
{
	if (m_isClone){
		DeleteCloneSkeleton(m_frameRoot);
	}
	else{
		DeleteSkeleton(m_frameRoot);
	}
	SAFE_RELEASE(m_pAnimController);
}

void CSkinModelData::LoadModelData(LPCSTR pFileName,CAnimation* anim){
	CAllocateHierarchy alloc;

	if (!strcmp(pFileName, "image/humpback_whale_model23.X")) {
		m_frameRoot = nullptr;
	}
	// モデルデータロード
	HRESULT hr = 
		D3DXLoadMeshHierarchyFromX(
		pFileName,
		D3DXMESH_VB_MANAGED,
		graphicsDevice(),
		&alloc,
		nullptr,
		&m_frameRoot,
		&m_pAnimController
		);

	CH_ASSERT(!FAILED(hr));
	SetUpBoneMatrixPointers(m_frameRoot, m_frameRoot);
	if (anim && m_pAnimController != nullptr){
		anim->Initialize(m_pAnimController);
	}
}

void CSkinModelData::SetUpBoneMatrixPointers(LPD3DXFRAME pFrame, LPD3DXFRAME pRootFrame /*ツリー構造のもっとも根のノードを使用するので渡している*/){
	// ボーンのツリー構造のいずれかのノードが持っているメッシュ情報を探索

	if (pFrame->pMeshContainer != nullptr){
		// メッシュ情報があればボーンに関連付ける関数を呼び出す
		SetUpBoneMatrixPointersOnMesh(pFrame->pMeshContainer, pRootFrame);
	}
	// ノードに兄弟がいれば探索を続行する
	if (pFrame->pFrameSibling != nullptr){
		SetUpBoneMatrixPointers(pFrame->pFrameSibling, pRootFrame);
	}
	// ノードに子がいれば探索を続行する
	if (pFrame->pFrameFirstChild != nullptr){
		SetUpBoneMatrixPointers(pFrame->pFrameFirstChild, pRootFrame);
	}
}

void CSkinModelData::SetUpBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME rootFrame){
	unsigned int iBone, cBones;
	ANIMATION::D3DXFRAME_DERIVED* pFrame;
	m_pMeshContainer = static_cast<ANIMATION::D3DXMESHCONTAINER_DERIVED*>(pMeshContainerBase);

	if (m_pMeshContainer->pSkinInfo != nullptr){
		cBones = m_pMeshContainer->pSkinInfo->GetNumBones();
		m_pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
		if (m_pMeshContainer->ppBoneMatrixPtrs == nullptr){
			return;
		}

		for (iBone = 0; iBone < cBones; iBone++){
			pFrame = static_cast<ANIMATION::D3DXFRAME_DERIVED*>(D3DXFrameFind(rootFrame,
			m_pMeshContainer->pSkinInfo->GetBoneName(iBone)));
			if (pFrame == nullptr){
				return;
			}
			m_pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
		}
	}
}

void CSkinModelData::UpdateBoneMatrix(const D3DXMATRIX* matWorld){
	UpdateFrameMatrices(m_frameRoot, matWorld);
}

void CSkinModelData::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, const D3DXMATRIX* pParentMatrix)
{
	ANIMATION::D3DXFRAME_DERIVED* pFrame = (ANIMATION::D3DXFRAME_DERIVED*)pFrameBase;

	if (pParentMatrix != NULL)
		// ボーンのワールド変換?
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	else
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

	if (pFrame->pFrameSibling != NULL)
	{
		// ボーンに兄弟がいればそれも更新
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		// ボーンに子供がいればそれも更新
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}

void CSkinModelData::CloneModelData(const CSkinModelData& modelData, CAnimation* anim)
{
	//スケルトンの複製を作成。。
	m_isClone = true;
	m_frameRoot = new ANIMATION::D3DXFRAME_DERIVED;
	m_frameRoot->pFrameFirstChild = nullptr;
	m_frameRoot->pFrameSibling = nullptr;
	m_frameRoot->pMeshContainer = nullptr;
	CloneSkeleton(m_frameRoot, modelData.m_frameRoot);
	//アニメーションコントローラを作成して、スケルトンと関連付けを行う。
	if (modelData.m_pAnimController) {
		modelData.m_pAnimController->CloneAnimationController(
			modelData.m_pAnimController->GetMaxNumAnimationOutputs(),
			modelData.m_pAnimController->GetMaxNumAnimationSets(),
			modelData.m_pAnimController->GetMaxNumTracks(),
			modelData.m_pAnimController->GetMaxNumEvents(),
			&m_pAnimController
			);

		SetUpOutputAnimationRegist(m_frameRoot, m_pAnimController);

		if (anim && m_pAnimController) {
			anim->Initialize(m_pAnimController);
		}
	}
	SetUpBoneMatrixPointers(m_frameRoot, m_frameRoot);
}

void CSkinModelData::CloneSkeleton(LPD3DXFRAME& dstFrame, LPD3DXFRAME srcFrame)
{
	//名前と行列をコピー。
	dstFrame->TransformationMatrix = srcFrame->TransformationMatrix;
	//メッシュコンテナをコピー。メッシュは使いまわす。
	if (srcFrame->pMeshContainer) {
		dstFrame->pMeshContainer = new ANIMATION::D3DXMESHCONTAINER_DERIVED;
		memcpy(dstFrame->pMeshContainer, srcFrame->pMeshContainer, sizeof(ANIMATION::D3DXMESHCONTAINER_DERIVED));
	}
	else {
		dstFrame->pMeshContainer = NULL;
	}
	AllocateName(srcFrame->Name, &dstFrame->Name);

	if (srcFrame->pFrameSibling != nullptr) {
		//兄弟がいるので、兄弟のためのメモリを確保。
		dstFrame->pFrameSibling = new ANIMATION::D3DXFRAME_DERIVED;
		dstFrame->pFrameSibling->pFrameFirstChild = nullptr;
		dstFrame->pFrameSibling->pFrameSibling = nullptr;
		dstFrame->pFrameSibling->pMeshContainer = nullptr;
		CloneSkeleton(dstFrame->pFrameSibling, srcFrame->pFrameSibling);
	}
	if (srcFrame->pFrameFirstChild != nullptr)
	{
		//子供がいるので、子供のためのメモリを確保。
		dstFrame->pFrameFirstChild = new ANIMATION::D3DXFRAME_DERIVED;
		dstFrame->pFrameFirstChild->pFrameFirstChild = nullptr;
		dstFrame->pFrameFirstChild->pFrameSibling = nullptr;
		dstFrame->pFrameFirstChild->pMeshContainer = nullptr;

		CloneSkeleton(dstFrame->pFrameFirstChild, srcFrame->pFrameFirstChild);
	}
}

void CSkinModelData::SetUpOutputAnimationRegist(LPD3DXFRAME frame, ID3DXAnimationController* animCtr)
{
	if (animCtr == nullptr) {
		return;
	}
	HRESULT hr = animCtr->RegisterAnimationOutput(frame->Name, &frame->TransformationMatrix, nullptr, nullptr, nullptr);
	if (frame->pFrameSibling != nullptr) {
		SetUpOutputAnimationRegist(frame->pFrameSibling, animCtr);
	}
	if (frame->pFrameFirstChild != nullptr)
	{
		SetUpOutputAnimationRegist(frame->pFrameFirstChild, animCtr);
	}
}

D3DXMATRIX* CSkinModelData::FindBoneWorldMatrix(LPCSTR name) {
	return this->FindBoneWorldMatrix(name, m_frameRoot);
}

D3DXMATRIX* CSkinModelData::FindBoneWorldMatrix(LPCSTR name, LPD3DXFRAME frame) {
	// 再起処理でボーンを検索する。
	if (frame->Name && !strcmp(frame->Name, name)) {
		// 発見。
		ANIMATION::D3DXFRAME_DERIVED* work = static_cast<ANIMATION::D3DXFRAME_DERIVED*>(frame);
		if (work) {
			return &work->CombinedTransformationMatrix;
		}
	}
	else if (frame->pFrameSibling) {
		// 兄弟を探索。
		D3DXMATRIX* ret = this->FindBoneWorldMatrix(name, frame->pFrameSibling);
		if (ret) {
			return ret;
		}
	}
	else if (frame->pFrameFirstChild) {
		// 子どもを探索。
		D3DXMATRIX* ret = this->FindBoneWorldMatrix(name, frame->pFrameFirstChild);
		if (ret) {
			return ret;
		}
	}
	return nullptr;
}

void CSkinModelData::DeleteCloneSkeleton(LPD3DXFRAME frame)
{

	if (frame->pFrameSibling != nullptr) {
		//兄弟
		DeleteCloneSkeleton(frame->pFrameSibling);
	}
	if (frame->pFrameFirstChild != nullptr)
	{
		//子供。
		DeleteCloneSkeleton(frame->pFrameFirstChild);
	}
	ANIMATION::D3DXMESHCONTAINER_DERIVED* pMeshContainer = (ANIMATION::D3DXMESHCONTAINER_DERIVED*)(frame->pMeshContainer);
	if (pMeshContainer) {
		SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
		SAFE_DELETE(pMeshContainer);
	}
	SAFE_DELETE_ARRAY(frame->Name);
	SAFE_DELETE(frame);
}

void CSkinModelData::DeleteSkeleton(LPD3DXFRAME frame)
{
	if (!frame) {
		return;
	}
	if (frame->pMeshContainer != NULL)
	{
		//メッシュコンテナがある。
		InnerDestroyMeshContainer(frame->pMeshContainer);
	}

	if (frame->pFrameSibling != NULL)
	{
		//兄弟がいる。
		DeleteSkeleton(frame->pFrameSibling);
	}

	if (frame->pFrameFirstChild != NULL)
	{
		//子供がいる。
		DeleteSkeleton(frame->pFrameFirstChild);
	}
	SAFE_DELETE_ARRAY(frame->Name);
	SAFE_DELETE(frame);
}

void CSkinModelData::InnerDestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT iMaterial;
	ANIMATION::D3DXMESHCONTAINER_DERIVED* pMeshContainer = (ANIMATION::D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->pAttributeTable);
	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
	SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);

	// release all the allocated textures
	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
		}
	}

	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_RELEASE(pMeshContainer->pOrigMesh);
	SAFE_DELETE(pMeshContainer);
}


HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName)
{
	UINT cbLength;

	if (Name != NULL)
	{
		cbLength = (UINT)strlen(Name) + 1;
		*pNewName = new CHAR[cbLength];
		if (*pNewName == NULL)
			return E_OUTOFMEMORY;
		memcpy(*pNewName, Name, cbLength * sizeof(CHAR));
	}
	else
	{
		*pNewName = NULL;
	}

	return S_OK;
}
