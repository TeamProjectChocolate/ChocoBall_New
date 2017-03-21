#pragma once
#include "stdafx.h"
#include "Infomation.h"

class CImageManager
{
	SINGLETON_DECL(CImageManager)
public:
	void Add2D(MODEL::IMAGE2D*);
	MODEL::IMAGE2D* LoadTextureFile(LPCSTR);
	void Add3D(LPCSTR,CSkinModelData*);
	MODEL::IMAGE2D* Find2DImage(LPCSTR);
	CSkinModelData* Find3DImage(LPCSTR);
	void Push_CloneModelData(CSkinModelData*);
private:
	vector<MODEL::IMAGE3D*> m_OriginalMeshDataList;
	vector<CSkinModelData*> m_CloneModelDatas;
	vector<MODEL::IMAGE2D*> m_ImageList;

	//vector�ɓo�^���ꂽ�v�f�����ׂč폜����֐�
	//���f�X�g���N�^�ɂČĂяo�����
	void DeleteAll();
};

