#pragma once
#include "stdafx.h"
#include "Graphicsdevice.h"
#include "Model.h"
#include "SkinModelData.h"
#include "Animation.h"
#include "Primitive.h"
#include "C3DObjectRender.h"
#include "RenderContext.h"

extern UINT g_NumBoneMatricesMax;
extern D3DXMATRIXA16* g_pBoneMatrices;

class C3DImage :public CModel{
public:
	C3DImage(){
		m_pImage = nullptr;
		m_UseBorn = false;
		m_luminance = 0.0f;
	};
	~C3DImage(){};

	void Initialize()override;
	void Update(const TRANSFORM&)override;
	void SetImage()override;
	//DrawSubsetとSetTextureのみを行う描画関数。

	IMAGE3D* GetImage(){
		return m_pImage;
	}
	HRESULT ReSet();
	HRESULT LoadXFile();

	CAnimation* GetAnimation()override{
		return &m_animation;
	}
	void SetUseBorn(bool flg)override{
		m_UseBorn = flg;
	}
	bool GetUseBorn()override{
		return m_UseBorn;
	}
	void SetCurrentAnimNo(int no)override{
		m_currentAnimNo = no;
	}
	int GetCurrentAnimNo()override{
		return m_currentAnimNo;
	}
	const D3DXMATRIX& GetRotation()override{
		return m_Rota;
	}
	IMAGE3D* GetImage_3D()override{ return m_pImage; }
private:
	void AnimationUpdate(const TRANSFORM&);
public:
	// 3D描画用メンバ変数
	IMAGE3D* m_pImage;
	D3DXMATRIX m_Rota;
	// アニメーション関連
	CAnimation m_animation;
	int m_currentAnimNo;
	bool m_UseBorn;		// ボーンを使用するかどうかのフラグ(trueなら使用、falseなら使用しない)
};
