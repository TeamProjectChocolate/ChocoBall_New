#pragma once

#include "stdafx.h"
#include "Infomation.h"
#include "Animation.h"


class CSkinModelData
{
public:
	CSkinModelData();
	~CSkinModelData();
	void LoadModelData(LPCSTR,CAnimation*);
	void SetUpBoneMatrixPointers(LPD3DXFRAME, LPD3DXFRAME);
	void SetUpBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER,LPD3DXFRAME);
	LPD3DXFRAME GetFrameRoot()
	{
		return m_frameRoot;
	}
	ID3DXAnimationController* GetAnimationController()
	{
		return m_pAnimController;
	}
	D3DXMESHCONTAINER_DERIVED* GetContainer(){
		return m_pMeshContainer;
	}
	void UpdateBoneMatrix(const D3DXMATRIX* matWorld);
	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, const D3DXMATRIX* pParentMatrix);
private:
	LPD3DXFRAME m_frameRoot = NULL;		// フレームルート
	ID3DXAnimationController* m_pAnimController;	// アニメーションコントローラ
	D3DXMESHCONTAINER_DERIVED* m_pMeshContainer;
};

