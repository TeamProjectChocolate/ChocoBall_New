#pragma once
#include "stdafx.h"
#include "Graphicsdevice.h"
#include "Model.h"
#include "SkinModelData.h"
#include "Animation.h"
#include "Primitive.h"
#include "C3DObjectRender.h"
#include "RenderContext.h"


class C3DImage :public CModel{
public:
	C3DImage(){
		m_pData = nullptr;
		m_UseBorn = false;
		m_luminance = 0.0f;
	};
	~C3DImage(){};

	void Initialize()override;
	void Update(const SH_ENGINE::TRANSFORM&)override;
	void SetImage()override;
	//DrawSubset��SetTexture�݂̂��s���`��֐��B

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
	CSkinModelData* GetImage_3D()override{ return m_pData; }
private:
	void AnimationUpdate(const SH_ENGINE::TRANSFORM&);
public:
	// 3D�`��p�����o�ϐ�
	CSkinModelData* m_pData;
	D3DXMATRIX m_Rota;
	// �A�j���[�V�����֘A�B
	CAnimation m_animation;
	int m_currentAnimNo;
	bool m_UseBorn;		// �{�[�����g�p���邩�ǂ����̃t���O(true�Ȃ�g�p�Afalse�Ȃ�g�p���Ȃ�)
};
