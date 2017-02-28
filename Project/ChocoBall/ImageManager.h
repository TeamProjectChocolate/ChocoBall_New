#pragma once
#include "stdafx.h"
#include "Infomation.h"

class CImageManager
{
	SINGLETON_DECL(CImageManager)
public:
	void Add2D(IMAGE2D*);
	IMAGE2D* LoadTextureFile(LPCSTR);
	void Add3D(LPCSTR,CSkinModelData*);
	IMAGE2D* Find2DImage(LPCSTR);
	CSkinModelData* Find3DImage(LPCSTR);
	void Push_CloneModelData(CSkinModelData*);
private:
	vector<IMAGE3D*> m_OriginalMeshDataList;
	vector<CSkinModelData*> m_CloneModelDatas;
	vector<IMAGE2D*> m_ImageList;

	//vectorに登録された要素をすべて削除する関数
	//※デストラクタにて呼び出される
	void DeleteAll();
};

