#pragma once
#include "stdafx.h"
#include "Effect.h"
#include "Primitive.h"
#include "Render.h"
#include "RenderTarget.h"

//#define USE_LOW_QUALITY_BLOOM		//定義で低クォリティのブルーム処理が有効になる。品質は劣化する。

typedef struct int2{
	int w;
	int h;
};

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
	CRenderTarget m_DownSamplingRenderTarget[4];	//!<輝度をダウンサンプリングするためのレンダリングターゲット。
	bool			m_isEnable;						//!<有効？
	float				m_weights[NUM_WEIGHTS];
	CPrimitive* m_Primitive;
	int2 m_size[4];
};

