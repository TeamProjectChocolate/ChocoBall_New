#pragma once
#include "stdafx.h"
#include "Effect.h"
#include "Primitive.h"
#include "Render.h"
#include "RenderTarget.h"


class CDofRender:public CRender
{
public:
	CDofRender();
	~CDofRender();
	/*!
	* @brief	�`��B
	*/
	void Draw()override;
	/*!
	* @brief	�쐬�B
	*/
	void Initialize()override;
	void MathPinto();

	LPDIRECT3DSURFACE9 GetSurface(){
		return m_DepthSamplingTarget.GetSurface();
	}
	LPDIRECT3DTEXTURE9 GetDepthTex(){
		return m_DepthSamplingTarget.GetTexture();
	}
	void SetPintoObject(CGameObject* object){
		m_pPintoObject = object;
	}
	CGameObject* GetPintoObject(){
		return m_pPintoObject;
	}
	const D3DXMATRIX& GetPintWorld(){
		return m_PintoWorld;
	}
private:
	/*!
	* @brief	�K�E�X�t�B���^�̏d�݂��X�V�B
	*/
	void UpdateWeight(float dispersion);
private:
	static const int NUM_WEIGHTS = 8;
	CRenderTarget m_DepthSamplingTarget;	// �[�x��񒊏o�p�����_�����O�^�[�Q�b�g
	CRenderTarget	m_BlurTarget[2];	//!<�ڂ������߂̃����_�����O�^�[�Q�b�g�B
	D3DXVECTOR2 m_size[2];	// �`��T�C�Y	
	bool			m_isEnable;						//!<�L���H
	float				m_weights[NUM_WEIGHTS];
	CPrimitive* m_Primitive;
	D3DXVECTOR2 m_FarNear;
	CGameObject* m_pPintoObject;	// �s���g�����킹��I�u�W�F�N�g
	D3DXMATRIX m_PintoWorld;	// �s���g�����킹��I�u�W�F�N�g�̃��[���h�s��
	bool m_UsePintoObject;	// �s���g�����킹��̂ɃI�u�W�F�N�g���g�p���邩
};

