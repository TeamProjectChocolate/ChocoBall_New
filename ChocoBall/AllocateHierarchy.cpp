#include "stdafx.h"
#include "AllocateHierarchy.h"
#include "GraphicsDevice.h"


UINT                        g_NumBoneMatricesMax = 0;
D3DXMATRIXA16*              g_pBoneMatrices = NULL;

CAllocateHierarchy::CAllocateHierarchy(){}

CAllocateHierarchy::~CAllocateHierarchy(){}

HRESULT CAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame){
	HRESULT hr = S_OK;
	D3DXFRAME_DERIVED* pFrame;

	*ppNewFrame = nullptr;
	pFrame = new D3DXFRAME_DERIVED;
	if (pFrame == nullptr){
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	hr = AllocateName(Name, &pFrame->Name);
	if (FAILED(hr))
		goto e_Exit;

	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	pFrame->pMeshContainer = nullptr;
	pFrame->pFrameSibling = nullptr;
	pFrame->pFrameFirstChild = nullptr;

	*ppNewFrame = pFrame;
	pFrame = nullptr;

e_Exit:
	delete pFrame;
	return hr;
}

HRESULT CAllocateHierarchy::AllocateName(LPCSTR Name, LPSTR* pNewName){
	unsigned int cbLength;

	if (Name != nullptr){
		cbLength = static_cast<unsigned int>(strlen(Name)) + 1;
		*pNewName = new char[cbLength];
		if (*pNewName == nullptr){
			return E_OUTOFMEMORY;
		}
		memcpy(*pNewName, Name, cbLength * sizeof(char));
	}
	else{
		*pNewName = nullptr;
	}
	return S_OK;
}

HRESULT CAllocateHierarchy::CreateMeshContainer(
	LPCSTR Name,
	CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials,
	CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD* pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER* ppNewMeshContainer){
	
	HRESULT hr;
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = nullptr;
	unsigned int NumFaces;
	unsigned int iMaterial;
	unsigned int iBone, cBones;
	LPDIRECT3DDEVICE9 Device = nullptr;

	LPD3DXMESH pMesh = nullptr;
	*ppNewMeshContainer = nullptr;

	if (pMeshData->Type != D3DXMESHTYPE_MESH){
		hr = E_FAIL;
		goto e_Exit;
	}

	pMesh = pMeshData->pMesh;

	if (pMesh->GetFVF() == 0){
		hr = E_FAIL;
		goto e_Exit;
	}

	pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	if (pMeshContainer == nullptr){
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}
	memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

	hr = AllocateName(Name, &pMeshContainer->Name);
	if (FAILED(hr))
		goto e_Exit;

	Device = nullptr;
	pMesh->GetDevice(&Device);
	NumFaces = pMesh->GetNumFaces();

	if (!(pMesh->GetFVF() & D3DFVF_NORMAL)){
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF() | D3DFVF_NORMAL, Device, &pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
			goto e_Exit;

		pMesh = pMeshContainer->MeshData.pMesh;

		D3DXComputeNormals(pMesh, nullptr);
	}
	else{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef();
	}

	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
	if ((pMeshContainer->pAdjacency == nullptr) || (pMeshContainer->pMaterials == nullptr)){
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD)* NumFaces * 3);
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9)* pMeshContainer->NumMaterials);

	if (NumMaterials > 0){
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL)* NumMaterials);
		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++){
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != nullptr){
				char filePath[64];
				strcpy(filePath, pMeshContainer->pMaterials[iMaterial].pTextureFilename);
				if(FAILED(D3DXCreateTextureFromFile(Device, filePath, &pMeshContainer->ppTextures[iMaterial]))){
					pMeshContainer->ppTextures[iMaterial] = nullptr;
				}
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = nullptr;
			}
		}
	}
	else{
		pMeshContainer->pMaterials[0].pTextureFilename = nullptr;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	if (pSkinInfo != nullptr){
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshContainer->pOrigMesh = pMesh;
		pMesh->AddRef();

		cBones = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
		if (pMeshContainer->pBoneOffsetMatrices == nullptr){
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		for (iBone = 0; iBone < cBones; iBone++){
			pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
		}


		// 頂点宣言
		const D3DVERTEXELEMENT9 vertexDecl[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
			{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
			{ 0, 56, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			D3DDECL_END()
		};

		LPD3DXMESH pCloneMesh = nullptr;
		pMeshContainer->MeshData.pMesh->CloneMesh(pMeshContainer->MeshData.pMesh->GetOptions(), vertexDecl, &(*graphicsDevice()), &pCloneMesh);

		D3DXComputeTangentFrameEx(pCloneMesh,
		D3DDECLUSAGE_TEXCOORD,
		0,
		D3DDECLUSAGE_TANGENT,
		0,
		D3DDECLUSAGE_BINORMAL,
		0,
		D3DDECLUSAGE_NORMAL,
		0,
		0,
		NULL,
		0.01f,    //ボケ具合.値をおおきくするとぼけなくなる
		0.25f,
		0.01f,
		&pCloneMesh,
		NULL
		);
		pMeshContainer->MeshData.pMesh->Release();
		pMeshContainer->MeshData.pMesh = pCloneMesh;
		

		hr = GenerateSkinnedMesh(Device, pMeshContainer);
		if (FAILED(hr))
			goto e_Exit;
	}

	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = nullptr;

e_Exit:

	if (pMeshContainer != nullptr){
		DestroyMeshContainer(pMeshContainer);
	}

	return hr;
}

HRESULT CAllocateHierarchy::GenerateSkinnedMesh(LPDIRECT3DDEVICE9 Device, D3DXMESHCONTAINER_DERIVED* pMeshContainer){
	HRESULT hr = S_OK;
	D3DCAPS9 d3dCaps;
	Device->GetDeviceCaps(&d3dCaps);

	if (pMeshContainer->pSkinInfo == nullptr)
		return hr;
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);

	unsigned int MaxMatrices = 26;
	pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

	DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
	if (d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1)){
		pMeshContainer->UseSoftwareVP = false;
		Flags |= D3DXMESH_MANAGED;
	}
	else{
		pMeshContainer->UseSoftwareVP = true;
		Flags |= D3DXMESH_SYSTEMMEM;
	}

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
		(pMeshContainer->pOrigMesh,
		Flags,
		pMeshContainer->NumPaletteEntries,
		pMeshContainer->pAdjacency,
		nullptr,
		nullptr,
		nullptr,
		&pMeshContainer->NumInfl,
		&pMeshContainer->NumAttributeGroups,
		&pMeshContainer->pBoneCombinationBuf,
		&pMeshContainer->MeshData.pMesh);
	if (FAILED(hr))
		goto e_Exit;

	DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL |
		D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
	if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF()){
		LPD3DXMESH pMesh;
		hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF, Device, &pMesh);
		if (!FAILED(hr)){
			pMeshContainer->MeshData.pMesh->Release();
			pMeshContainer->MeshData.pMesh = pMesh;
			pMesh = nullptr;
		}
	}

	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	LPD3DVERTEXELEMENT9 pDeclCur;
	hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
	if (FAILED(hr))
		goto e_Exit;

	pDeclCur = pDecl;
	while(pDeclCur->Stream != 0xff){
		if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0)){
			pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
		}
		pDeclCur++;
	}

	hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
	if (FAILED(hr)){
		goto e_Exit;
	}

	if (g_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones()){
		g_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

		delete[] g_pBoneMatrices;
		g_pBoneMatrices = new D3DXMATRIXA16[g_NumBoneMatricesMax];
		if (g_pBoneMatrices == nullptr){
			hr = SP_OUTOFMEMORY;
			goto e_Exit;
		}
	}
e_Exit:
	return hr;
}

HRESULT CAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree){
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);
	return S_OK;
}

HRESULT CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase){
	unsigned int iMaterial;
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
	SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);

	if (pMeshContainer->ppTextures != nullptr){
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++){
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
	return S_OK;
}
