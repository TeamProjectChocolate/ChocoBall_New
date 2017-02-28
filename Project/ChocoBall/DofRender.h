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
	* @brief	描画。
	*/
	void Draw()override;
	/*!
	* @brief	作成。
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
	* @brief	ガウスフィルタの重みを更新。
	*/
	void UpdateWeight(float dispersion);
private:
	static const int NUM_WEIGHTS = 8;
	CRenderTarget m_DepthSamplingTarget;	// 深度情報抽出用レンダリングターゲット
	CRenderTarget	m_BlurTarget[2];	//!<ぼかすためのレンダリングターゲット。
	D3DXVECTOR2 m_size[2];	// 描画サイズ	
	bool			m_isEnable;						//!<有効？
	float				m_weights[NUM_WEIGHTS];
	CPrimitive* m_Primitive;
	D3DXVECTOR2 m_FarNear;
	CGameObject* m_pPintoObject;	// ピントを合わせるオブジェクト
	D3DXMATRIX m_PintoWorld;	// ピントを合わせるオブジェクトのワールド行列
	bool m_UsePintoObject;	// ピントを合わせるのにオブジェクトを使用するか
};

