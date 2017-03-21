#pragma once

#include "stdafx.h"
#include "Infomation.h"

class CAllocateHierarchy :public ID3DXAllocateHierarchy{
public:
	CAllocateHierarchy();
	~CAllocateHierarchy();
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA* pMeshData,
		CONST D3DXMATERIAL* pMaterials,
		CONST D3DXEFFECTINSTANCE* pEffectInstance,
		DWORD NumMaterials,
		CONST DWORD* pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER* ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
	HRESULT AllocateName(LPCSTR, LPSTR*);
	HRESULT GenerateSkinnedMesh(LPDIRECT3DDEVICE9, ANIMATION::D3DXMESHCONTAINER_DERIVED*);
};