#pragma once
#include "stdafx.h"
#include "Infomation.h"
#include "Camera.h"

class CModel;
class CRender
{
public:
	CRender();
	~CRender();
	virtual void Initialize() = 0;
	virtual void Draw() = 0;
	virtual void Draw(const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){};
	virtual void SetModelData(CModel* pModel){
		m_pModel = pModel;
	};
	// �Ă΂ꂽ�u�Ԃɐݒ肳��Ă���e�N�j�b�N���𔽉f����֐�
	void SetUpTechnique(){
		m_pEffect->SetTechnique(m_pTechniqueName);
	}
	// �Ă΂ꂽ�u�Ԃɓn���ꂽ�����̃e�N�j�b�N�𔽉f���A�ݒ肳��Ă���e�N�j�b�N�����X�V����֐�
	void SetUpTechnique(LPCSTR name){
		strcpy(m_pTechniqueName, name);
		m_pEffect->SetTechnique(m_pTechniqueName);
	}
	virtual void SetVertexBuffer(LPDIRECT3DVERTEXBUFFER9){};
	void SetEffect(LPD3DXEFFECT effect){
		m_pEffect = effect;
	}
	LPD3DXEFFECT GetEffect(){
		return m_pEffect;
	}
	void SetSampleTexture(LPDIRECT3DTEXTURE9 tex){
		m_pSampleTexture = tex;
	}
	inline virtual void SetShadowCamera(CCamera* camera){};
protected:
	LPD3DXEFFECT m_pEffect;
	CHAR m_pTechniqueName[MAX_FILENAME + 1];
	CModel* m_pModel;
	LPDIRECT3DTEXTURE9 m_pSampleTexture;	// �`�悷��O�ɍŏ��Ɏ󂯎��e�N�X�`��
};

