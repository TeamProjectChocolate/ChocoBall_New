#pragma once
#include "stdafx.h"
#include "Effect.h"
#include "Primitive.h"
#include "Render.h"
#include "RenderTarget.h"

//#define USE_LOW_QUALITY_BLOOM		//定義で低クォリティのブルーム処理が有効になる。品質は劣化する。

#define MGF_DOWN_SAMPLE_COUNT 4

class CBloomRender:public CRender
{
public:
	CBloomRender();
	~CBloomRender();
	/*!
	* @brief	描画。
	*/
	void Draw()override;
	/*!
	* @brief	作成。
	*/
	void Initialize()override;

	LPDIRECT3DTEXTURE9 GetTexture(){
		return m_luminanceRenderTarget.GetTexture();
	}
private:
	/*!
	* @brief	ガウスフィルタの重みを更新。
	*/
	void UpdateWeight(float dispersion);
private:
#if defined(USE_LOW_QUALITY_BLOOM)
	static const int NUM_WEIGHTS = 4;
#else
	static const int NUM_WEIGHTS = 8;
#endif
	LPD3DXEFFECT	m_pEffect;						//!<エフェクト。
	CRenderTarget m_luminanceRenderTarget;		//!<輝度を抽出するレンダリングターゲット。
	CRenderTarget m_DownSamplingRenderTarget[MGF_DOWN_SAMPLE_COUNT][2];	//!<輝度をダウンサンプリングするためのレンダリングターゲット(縦と横のダウンサンプリングを四回分)。
	bool			m_isEnable;						//!<有効？
	float				m_weights[NUM_WEIGHTS];
	CPrimitive* m_Primitive;
	D3DXVECTOR2 m_size[MGF_DOWN_SAMPLE_COUNT][2];
};

