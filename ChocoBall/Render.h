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
	// 呼ばれた瞬間に設定されているテクニック名を反映する関数
	void SetUpTechnique(){
		m_pEffect->SetTechnique(m_pTechniqueName);
	}
	// 呼ばれた瞬間に渡された引数のテクニックを反映し、設定されているテクニック名を更新する関数
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
	LPDIRECT3DTEXTURE9 m_pSampleTexture;	// 描画する前に最初に受け取るテクスチャ
};

