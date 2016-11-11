#include "stdafx.h"
#include "C3DImage.h"
#include "Effect.h"
#include "ImageManager.h"
#include "RenderContext.h"
#include "Camera.h"
#include "GraphicsDevice.h"
#include "ShadowRender.h"
#include "SkinModelData.h"
#include "AllocateHierarchy.h"
#include "ObjectManager.h"
#include "Assert.h"


	//他で使うな。
	#define MultipyMatrix(mOut, m1)										\
	{																	\
		D3DXMATRIX m0 = mOut;											\
		for (int i = 0; i<4; i++) {										\
			for (int j = 0; j<4; j++) {									\
				mOut.m[i][j] = 0.0f;									\
				for (int k = 0; k<4; k++) {								\
					mOut.m[i][j] += m0.m[i][k] * m1.m[k][j];			\
				}														\
			}															\
		}																\
																		\
	}

//他で使うな。
	#define MultipyMatrix2(mOut, m0, m1)										\
	{																	\
		for (int i = 0; i<4; i++) {										\
			for (int j = 0; j<4; j++) {									\
				mOut.m[i][j] = 0.0f;									\
				for (int k = 0; k<4; k++) {								\
					mOut.m[i][j] += m0.m[i][k] * m1.m[k][j];			\
				}														\
			}															\
		}																\
																		\
	}


void C3DImage::Initialize(){
	CModel::Initialize();
}

void C3DImage::SetImage(){
	CSkinModelData* pModel = SINSTANCE(CImageManager)->Find3DImage(m_pFileName);
	if (pModel){
		CSkinModelData* NewModelData = new CSkinModelData;
		NewModelData->CloneModelData(*pModel, &m_animation);
		m_pData = new CSkinModelData;
		m_pData = NewModelData;
		SINSTANCE(CImageManager)->Push_CloneModelData(NewModelData);
		//if (m_pImage->pModel->GetAnimationController() != nullptr){
		//	m_animation.Initialize(m_pImage->pModel->GetAnimationController());
		//}
	}
	else{
		LoadXFile();
	}
	//m_currentAnimNo = 0;
	//m_animation.PlayAnimation(m_currentAnimNo, 1.0f);
}

HRESULT C3DImage::LoadXFile(){
	m_pData = new CSkinModelData;
	m_pData->LoadModelData(m_pFileName, &m_animation);
	SINSTANCE(CImageManager)->Add3D(m_pFileName, m_pData);
	return S_OK;
}

void C3DImage::Update(const TRANSFORM& transform){
	AnimationUpdate(transform);
	if (m_animation.IsHasAnimationController()){
		m_animation.Update(DELTA_TIME);
	}
}

void C3DImage::AnimationUpdate(const TRANSFORM& transform){
	D3DXMATRIX Trans;	// 移動行列
	D3DXMATRIX Scale;	// 拡大・縮小行列
	//D3DXMatrixIdentity(&m_World);	// 行列初期化

	D3DXMatrixRotationQuaternion(&m_Rota, &transform.angle);	// クォータニオンによる回転行列の作成
	//m_Rota = m_World;

	D3DXMatrixScaling(&Scale, transform.scale.x, transform.scale.y, transform.scale.z);
	MultipyMatrix2(m_World, Scale, m_Rota);

	Trans.m[0][0] = 1.0f;
	Trans.m[0][1] = 0.0f;
	Trans.m[0][2] = 0.0f;
	Trans.m[0][3] = 0.0f;

	Trans.m[1][0] = 0.0f;
	Trans.m[1][1] = 1.0f;
	Trans.m[1][2] = 0.0f;
	Trans.m[1][3] = 0.0f;

	Trans.m[2][0] = 0.0f;
	Trans.m[2][1] = 0.0f;
	Trans.m[2][2] = 1.0f;
	Trans.m[2][3] = 0.0f;

	Trans.m[3][0] = transform.position.x;
	Trans.m[3][1] = transform.position.y;
	Trans.m[3][2] = transform.position.z;
	Trans.m[3][3] = 1.0f;
//	D3DXMatrixTranslation(&Trans, m_transform.position.x, m_transform.position.y, m_transform.position.z);
	MultipyMatrix(m_World, Trans);
//	D3DXMatrixMultiply(&m_World, &m_World, &Trans);

	
	if (m_pData){
		m_pData->UpdateBoneMatrix(&m_World);	//ボーン行列を更新。
	}
}