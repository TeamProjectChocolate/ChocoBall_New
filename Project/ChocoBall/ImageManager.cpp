#include "stdafx.h"
#include "ImageManager.h"
#include "GraphicsDevice.h"

CImageManager* CImageManager::m_instance = nullptr;

void CImageManager::Add2D(MODEL::IMAGE2D* image){
	// 新しく読み込んだXファイルを登録
	m_ImageList.push_back(image);		// IMAGE2D情報配列に追加
}

MODEL::IMAGE2D* CImageManager::LoadTextureFile(LPCSTR pFileName){
	MODEL::IMAGE2D* image;
	image = new MODEL::IMAGE2D;
	strcpy(image->pFileName,pFileName);

	D3DXIMAGE_INFO imgInfo;										//画像情報格納用構造体
	D3DXCreateTextureFromFileEx(graphicsDevice(), pFileName, 0, 0, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0xff000000, &imgInfo, NULL, &image->pTex);	//テクスチャ読込
	RECT rec = { 0, 0, imgInfo.Width, imgInfo.Height };			//描画領域
	image->RealSize.x = imgInfo.Width;
	image->RealSize.y = imgInfo.Height;
	IDirect3DSurface9* surface;
	image->pTex->GetSurfaceLevel(0, &surface);
	D3DSURFACE_DESC desc;
	surface->GetDesc(&desc);
	image->UnRealSize.x = desc.Width;
	image->UnRealSize.y = desc.Height;
	surface->Release();
	SINSTANCE(CImageManager)->Add2D(image);
	return image;
}

MODEL::IMAGE2D* CImageManager::Find2DImage(LPCSTR pFileName){
	int size = m_ImageList.size();
	// すでに使用するテクスチャファイルがあればそれを返す
	for (int idx = 0; idx < size; idx++){
		if (!strcmp(m_ImageList[idx]->pFileName,pFileName)){
			return m_ImageList[idx];
		}
	}
	return LoadTextureFile(pFileName);
}

void CImageManager::Add3D(LPCSTR pFileName,CSkinModelData* ModelData)
{
	// 新しく読み込んだXファイルを登録
	MODEL::IMAGE3D* image;
	image = new MODEL::IMAGE3D;
	strcpy(image->pFileName,pFileName);
	image->pModel = ModelData;
	m_OriginalMeshDataList.push_back(image);		// IMAGE3D情報配列に追加
}

void CImageManager::Push_CloneModelData(CSkinModelData* Clone){
	m_CloneModelDatas.push_back(Clone);
}

CSkinModelData* CImageManager::Find3DImage(LPCSTR pFileName){
	int size = m_OriginalMeshDataList.size();
	// すでに使用するXファイルがあればそれを返す
	for (int idx = 0; idx < size; idx++){
		if (!strcmp(m_OriginalMeshDataList[idx]->pFileName,pFileName)){
			return m_OriginalMeshDataList[idx]->pModel;
		}
	}
	return nullptr;
}

void CImageManager::DeleteAll(){
	for (CSkinModelData* data : m_CloneModelDatas){
		SAFE_DELETE(data);
	}
	m_CloneModelDatas.clear();
	for (int idx = 0,size = m_OriginalMeshDataList.size(); idx < size; idx++){
		SAFE_DELETE(m_OriginalMeshDataList[idx]->pModel);
	}
	m_OriginalMeshDataList.clear();
	for (int idx = 0, size = m_ImageList.size(); idx < size; idx++){
		SAFE_DELETE(m_ImageList[idx]->pTex);
		SAFE_DELETE(m_ImageList[idx]);
	}
	m_ImageList.clear();
}