#pragma once
#include "stdafx.h"
#include "Effect.h"
#include "Primitive.h"
#include "Render.h"
#include "RenderTarget.h"

//#define USE_LOW_QUALITY_BLOOM		//��`�Œ�N�H���e�B�̃u���[���������L���ɂȂ�B�i���͗򉻂���B

#define MGF_DOWN_SAMPLE_COUNT 4

class CBloomRender:public CRender
{
public:
	CBloomRender();
	~CBloomRender();
	/*!
	* @brief	�`��B
	*/
	void Draw()override;
	/*!
	* @brief	�쐬�B
	*/
	void Initialize()override;

	LPDIRECT3DTEXTURE9 GetTexture(){
		return m_luminanceRenderTarget.GetTexture();
	}
private:
	/*!
	* @brief	�K�E�X�t�B���^�̏d�݂��X�V�B
	*/
	void UpdateWeight(float dispersion);
private:
#if defined(USE_LOW_QUALITY_BLOOM)
	static const int NUM_WEIGHTS = 4;
#else
	static const int NUM_WEIGHTS = 8;
#endif
	LPD3DXEFFECT	m_pEffect;						//!<�G�t�F�N�g�B
	CRenderTarget m_luminanceRenderTarget;		//!<�P�x�𒊏o���郌���_�����O�^�[�Q�b�g�B
	CRenderTarget m_DownSamplingRenderTarget[MGF_DOWN_SAMPLE_COUNT][2];	//!<�P�x���_�E���T���v�����O���邽�߂̃����_�����O�^�[�Q�b�g(�c�Ɖ��̃_�E���T���v�����O���l��)�B
	bool			m_isEnable;						//!<�L���H
	float				m_weights[NUM_WEIGHTS];
	CPrimitive* m_Primitive;
	D3DXVECTOR2 m_size[MGF_DOWN_SAMPLE_COUNT][2];
};

